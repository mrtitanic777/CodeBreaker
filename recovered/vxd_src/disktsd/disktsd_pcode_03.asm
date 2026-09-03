; disktsd_pcode_03.asm -- DISKTSD object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        call   sub_3_0236
        push   ebp
        sub    esp, 0ch
        mov    ebp, esp
        mov    word ptr [ebp], 0
        mov    word ptr [ebp + 4], 90h
        push   ebp
        db  0FFh, 015h, 050h, 000h, 000h, 000h
        mov    edi, dword ptr [ebp + 6]
        add    esp, 10h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    eax, 803h
        mov    word ptr [ebp + 1ch], 803h
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  ecx, word ptr [ebp + 3ch]
        shl    ecx, 4
        movzx  eax, word ptr [ebp]
        add    ecx, eax
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        add    ecx, dword ptr [ebx + 4]
        db  089h, 00Dh, 0B3h, 002h, 000h, 000h
        pop    ebp
        ret
        db  000h
loc_3_008C:
        mov    esi, dword ptr [ebx + 0ch]
        test   dword ptr [esi + 20h], 6000h
        jne    NEAR PTR loc_3_0154
        cmp    byte ptr [esi + 40h], 0
        je     SHORT loc_3_00AC
        cmp    byte ptr [esi + 40h], 0ah
        jne    NEAR PTR loc_3_0154
loc_3_00AC:
        test   dword ptr [esi + 1ch], 23h
        je     SHORT loc_3_00BD
        mov    eax, dword ptr [esi + 1ch]
        jmp    NEAR PTR loc_3_0154
loc_3_00BD:
        push   ebp
        sub    esp, 18h
        mov    ebp, esp
        mov    dword ptr [ebp + 4], esi
        db  0C7h, 045h, 008h, 084h, 000h, 000h, 000h
        mov    al, byte ptr [ebx + 8]
        mov    byte ptr [ebp + 16h], al
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ebp + 0ch], eax
        mov    word ptr [ebp + 10h], 4
        mov    eax, dword ptr [esi + 1ch]
        and    eax, 0ffffffe3h
        mov    dword ptr [ebp + 12h], eax
        mov    word ptr [ebp], 5
        push   ebp
        db  0FFh, 015h, 050h, 000h, 000h, 000h
        add    esp, 1ch
        pop    ebp
        or     byte ptr [esi + 20h], 2
        mov    dword ptr [esi + 30h], 0
        cmp    byte ptr [esi + 40h], 0ah
        jne    SHORT loc_3_0144
        call   sub_3_02AB
        xor    eax, eax
        bt     dword ptr [esi + 24h], 6
        jae    SHORT loc_3_011D
        call   sub_3_027D
loc_3_011D:
        test   byte ptr [esi + 17h], 2
        je     SHORT loc_3_0154
        call   sub_3_0565
        test   byte ptr [esi + 1dh], 20h
        je     SHORT loc_3_0154
        mov    eax, dword ptr [esi + 12dh]
        push   dword ptr [esi + 30h]
        mov    dword ptr [esi + 30h], eax
        call   sub_3_0565
        pop    dword ptr [esi + 30h]
        jmp    SHORT loc_3_0154
loc_3_0144:
        db  0FEh, 005h, 0B0h, 002h, 000h, 000h
        call   sub_3_027D
        call   sub_3_05DE
loc_3_0154:
        ret
loc_3_0155:
        mov    esi, dword ptr [ebx + 0ch]
        test   byte ptr [esi + 21h], 20h
        jne    NEAR PTR loc_3_0214
        cmp    byte ptr [esi + 40h], 0ah
        je     SHORT loc_3_017C
        test   byte ptr [esi + 21h], 40h
        jne    NEAR PTR loc_3_0214
        cmp    byte ptr [esi + 40h], 0
        je     NEAR PTR loc_3_0215
loc_3_017C:
        db  0FEh, 00Dh, 0B1h, 002h, 000h, 000h
        movzx  eax, byte ptr [esi + 15h]
        db  00Fh, 0B3h, 005h, 09Fh, 002h, 000h, 000h
        test   byte ptr [esi + 21h], 40h
        je     SHORT loc_3_01F9
        movzx  eax, byte ptr [esi + 14h]
        db  00Fh, 0B3h, 005h, 0A3h, 002h, 000h, 000h
        test   byte ptr [esi + 1dh], 20h
        je     SHORT loc_3_01C8
        sub    esp, 8
        mov    edx, esp
        mov    word ptr [edx], 0fh
        mov    dword ptr [edx + 4], 1
        push   edx
        db  0FFh, 015h, 050h, 000h, 000h, 000h
        add    esp, 0ch
        db  080h, 025h, 0A3h, 002h, 000h, 000h, 0FDh
        jmp    SHORT loc_3_01F9
loc_3_01C8:
        cmp    eax, 1
        jne    SHORT loc_3_01F9
        db  0F6h, 005h, 0A3h, 002h, 000h, 000h, 001h
        je     SHORT loc_3_01F9
        push   esi
        xor    eax, eax
        call   sub_3_200B
        or     eax, eax
        jne    SHORT loc_3_01F8
        or     byte ptr [esi + 1dh], 20h
        xor    edx, edx
        inc    dl
        call   sub_3_0A5F
        mov    byte ptr [esi + 14h], 0
        call   sub_3_1FAD
loc_3_01F8:
        pop    esi
loc_3_01F9:
        test   byte ptr [esi + 17h], 2
        je     SHORT loc_3_0214
        xor    eax, eax
        xchg   dword ptr [esi + 30h], eax
        or     eax, eax
        je     SHORT loc_3_0214
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_3_0214:
        ret
loc_3_0215:
        db  0FEh, 00Dh, 0B0h, 002h, 000h, 000h
        jmp    SHORT loc_3_01F9
loc_3_021D:
        db  080h, 03Dh, 0B1h, 002h, 000h, 000h, 000h
        jne    SHORT loc_3_0235
        db  080h, 03Dh, 0B0h, 002h, 000h, 000h, 000h
        jne    SHORT loc_3_0235
        mov    word ptr [ebx + 2], 0ffffh
loc_3_0235:
        ret
sub_3_0236:
        xor    ecx, ecx
        xor    edi, edi
loc_3_023A:
        mov    al, 1
        call   sub_3_0258
        je     SHORT loc_3_0257
        cmp    byte ptr [edi + 0ch], 10h
        jne    SHORT loc_3_0254
        movzx  eax, byte ptr [edi + 5]
        db  00Fh, 0ABh, 005h, 0A7h, 002h, 000h, 000h
loc_3_0254:
        inc    ecx
        jmp    SHORT loc_3_023A
loc_3_0257:
        ret
sub_3_0258:
        or     edi, edi
        jne    SHORT loc_3_026D
        db  08Bh, 03Dh, 06Ch, 000h, 000h, 000h
        mov    edi, dword ptr [edi + 2ch]
        or     edi, edi
        je     SHORT loc_3_027A
loc_3_0269:
        cmp    byte ptr [edi], al
        je     SHORT loc_3_027A
loc_3_026D:
        cmp    dword ptr [edi + 1], 0
        je     SHORT loc_3_0278
        add    edi, dword ptr [edi + 1]
        jmp    SHORT loc_3_0269
loc_3_0278:
        xor    edi, edi
loc_3_027A:
        or     edi, edi
        ret
sub_3_027D:
        push   eax
        test   dword ptr [esi + 1ch], 40000h
        je     SHORT loc_3_0292
        xor    ebx, ebx
        mov    dx, 6
        call   sub_3_1EDB
loc_3_0292:
        xor    ebx, ebx
        mov    dx, 0ch
        call   sub_3_1EDB
        pop    eax
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_02AB:
        test   byte ptr [esi + 23h], 10h
        je     NEAR PTR loc_3_0397
        or     byte ptr [esi + 23h], 2
        db  0FEh, 005h, 0B1h, 002h, 000h, 000h
        movzx  edx, byte ptr [esi + 15h]
        or     byte ptr [esi + 20h], 1
        call   sub_3_0A72
        db  00Fh, 0ABh, 015h, 09Fh, 002h, 000h, 000h
        call   sub_3_210C
        jb     NEAR PTR loc_3_037C
        push   esi
        movzx  eax, byte ptr [ecx + 5]
loc_3_02E3:
        mov    edx, eax
        call   sub_3_200B
        or     eax, eax
        mov    eax, esi
        pop    esi
        jne    SHORT loc_3_030F
        cmp    eax, esi
        or     byte ptr [eax + 0b8h], 40h
        test   byte ptr [eax + 22h], 40h
        jne    SHORT loc_3_0359
        test   byte ptr [eax + 1dh], 20h
        jne    SHORT loc_3_035A
loc_3_0306:
        push   esi
        mov    esi, eax
        call   sub_3_2070
        pop    esi
loc_3_030F:
        and    byte ptr [esi + 23h], 0fdh
        or     byte ptr [esi + 21h], 40h
        and    byte ptr [esi + 1dh], 0dfh
        db  00Fh, 0ABh, 015h, 0A3h, 002h, 000h, 000h
        call   sub_3_0A72
        call   sub_3_1FAD
        cmp    dl, 1
        ja     SHORT loc_3_0359
        je     SHORT loc_3_0341
        db  0F6h, 005h, 0A3h, 002h, 000h, 000h, 002h
        jne    SHORT loc_3_0359
        jmp    NEAR PTR loc_3_0539
loc_3_0341:
        db  0F6h, 005h, 0A3h, 002h, 000h, 000h, 001h
        je     SHORT loc_3_0359
        xor    eax, eax
        call   sub_3_200B
        or     eax, eax
        jne    SHORT loc_3_0359
        and    byte ptr [esi + 1dh], 0dfh
loc_3_0359:
        ret
loc_3_035A:
        cmp    edx, 1
        jne    SHORT loc_3_0306
        sub    esp, 8
        mov    eax, esp
        mov    word ptr [eax], 0fh
        mov    dword ptr [eax + 4], 1
        push   eax
        db  0FFh, 015h, 050h, 000h, 000h, 000h
        add    esp, 0ch
        jmp    SHORT loc_3_030F
loc_3_037C:
        call   sub_3_03B9
        jae    SHORT loc_3_0391
        xor    eax, eax
        call   sub_3_202D
        or     ah, ah
        jne    SHORT loc_3_0359
        movzx  eax, al
loc_3_0391:
        push   esi
        jmp    NEAR PTR loc_3_02E3
loc_3_0397:
        test   byte ptr [esi + 24h], 40h
        je     SHORT loc_3_037C
        db  00Fh, 0BCh, 005h, 0A7h, 002h, 000h, 000h
        je     SHORT loc_3_037C
        db  00Fh, 0B3h, 005h, 0A7h, 002h, 000h, 000h
        mov    byte ptr [esi + 15h], al
        or     byte ptr [esi + 23h], 10h
        jmp    NEAR PTR sub_3_02AB
sub_3_03B9:
        enter  2ch, 0
        push   esi
        push   edi
        lea    eax, [ebp - 4]
        push   0
        push   dword ptr [esi + 70h]
        push   eax
        db  0CDh, 020h, 003h, 000h, 033h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_3_04E8
        db  08Bh, 03Dh, 06Ch, 000h, 000h, 000h
        mov    edi, dword ptr [edi + 4ch]
        or     edi, edi
        je     NEAR PTR loc_3_04E8
loc_3_03EA:
        mov    dword ptr [ebp - 18h], edi
        mov    edi, dword ptr [edi]
loc_3_03EF:
        or     edi, edi
        je     NEAR PTR loc_3_04CE
        call   sub_3_051F
        mov    dword ptr [ebp - 14h], edi
        cmp    byte ptr [edi], 8
        jne    NEAR PTR loc_3_04C3
        mov    edi, dword ptr [edi + 6]
        or     edi, edi
        je     NEAR PTR loc_3_04C3
        call   sub_3_051F
        lea    eax, [ebp - 0ch]
        push   2
        push   dword ptr [ebp - 4]
        push   eax
        db  0CDh, 020h, 01Ah, 000h, 033h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_3_04C3
        push   edi
        lea    edi, [ebp - 2ch]
        xor    eax, eax
        cld
        mov    ecx, 5
        rep stosd
        lea    edi, [ebp - 2ch]
        mov    ecx, 5
loc_3_0448:
        lea    eax, [ebp - 8]
        lea    edx, [ebp - 10h]
        push   ecx
        push   0
        push   edx
        push   2
        push   dword ptr [ebp - 0ch]
        push   eax
        db  0CDh, 020h, 01Fh, 000h, 033h, 000h
        add    esp, 14h
        pop    ecx
        or     eax, eax
        jne    SHORT loc_3_0483
        mov    ebx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], ebx
        dec    ecx
        je     SHORT loc_3_0483
        mov    ax, word ptr [ebx + 4]
        mov    word ptr [edi], ax
        mov    ax, word ptr [ebx + 6]
        mov    word ptr [edi + 2], ax
        add    edi, 4
        jmp    SHORT loc_3_0448
loc_3_0483:
        pop    edi
        jecxz  loc_3_04B6
loc_3_0486:
        lea    ebx, [ebp - 2ch]
        call   sub_3_04ED
        jb     SHORT loc_3_04B6
        movzx  ecx, byte ptr [edi + 20h]
        jecxz  loc_3_04B6
        lea    ebx, [edi + 21h]
loc_3_0499:
        mov    al, byte ptr [ebx + 1]
        cmp    byte ptr [esi + 7eh], al
        jne    SHORT loc_3_04B1
        mov    al, byte ptr [ebx + 2]
        cmp    byte ptr [esi + 7ch], al
        jne    SHORT loc_3_04B1
        mov    al, byte ptr [ebx + 3]
        cmp    byte ptr [esi + 7dh], al
        je     SHORT loc_3_04E2
loc_3_04B1:
        add    ebx, 6
        loop   loc_3_0499
loc_3_04B6:
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     SHORT loc_3_04C3
        call   sub_3_051F
        jmp    SHORT loc_3_0486
loc_3_04C3:
        mov    edi, dword ptr [ebp - 14h]
        mov    edi, dword ptr [edi + 1]
        jmp    NEAR PTR loc_3_03EF
loc_3_04CE:
        mov    edi, dword ptr [ebp - 18h]
        mov    edi, dword ptr [edi + 4]
        or     edi, edi
        je     SHORT loc_3_04E8
        call   sub_3_051F
        jmp    NEAR PTR loc_3_03EA
loc_3_04E2:
        movzx  eax, byte ptr [ebx]
        clc
        jmp    SHORT loc_3_04E9
loc_3_04E8:
        stc
loc_3_04E9:
        pop    edi
        pop    esi
        leave
        ret
sub_3_04ED:
        push   ecx
        push   edi
        mov    ecx, dword ptr [edi + 14h]
        jecxz  loc_3_051B
        mov    edi, dword ptr [edi + 18h]
        or     edi, edi
        je     SHORT loc_3_051B
        call   sub_3_051F
loc_3_0500:
        movzx  eax, word ptr [ebx]
        or     eax, eax
        je     SHORT loc_3_051C
        push   ecx
loc_3_0508:
        cmp    dword ptr [edi], eax
        je     SHORT loc_3_0511
        add    edi, 0ch
        loop   loc_3_0508
loc_3_0511:
        or     ecx, ecx
        pop    ecx
        je     SHORT loc_3_051B
        add    ebx, 4
        jmp    SHORT loc_3_0500
loc_3_051B:
        stc
loc_3_051C:
        pop    edi
        pop    ecx
        ret
sub_3_051F:
        push   ebx
        movzx  ebx, di
        rol    edi, 10h
        movzx  edi, di
        shl    edi, 4
        add    edi, ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        add    edi, dword ptr [ebx + 4]
        pop    ebx
        ret
loc_3_0539:
        or     byte ptr [esi + 1dh], 20h
        push   esi
        mov    al, 1
        movzx  eax, al
        call   sub_3_200B
        or     eax, eax
        jne    SHORT loc_3_0551
        call   sub_3_2070
loc_3_0551:
        pop    esi
        xor    edx, edx
        inc    dl
        call   sub_3_0A5F
        mov    byte ptr [esi + 14h], 0
        call   sub_3_1FAD
        ret
sub_3_0565:
        push   edi
        push   esi
        mov    ecx, 6
        lea    edi, [esi + 0d3h]
        lea    esi, [esi + 54h]
        rep movsd
        mov    edi, dword ptr [esp]
        test   byte ptr [edi + 24h], 40h
        jne    SHORT loc_3_0596
        call   sub_3_1B35
loc_3_0585:
        pop    esi
        pop    edi
        mov    eax, dword ptr [esi + 64h]
        mov    ecx, dword ptr [esi + 30h]
        mov    word ptr [ecx + 41h], ax
        or     byte ptr [esi + 21h], 10h
        ret
loc_3_0596:
        call   sub_3_1C34
        jmp    SHORT loc_3_0585
        db  000h, 000h, 000h, 000h
loc_3_05A1:
        dec    ecx
        outsb
        db  074h, 031h
        xor    eax, dword ptr [eax]
        db  000h
sub_3_05A8:
        enter  8, 0
        mov    dword ptr [ebp - 8], edi
        xor    ebx, ebx
        mov    dx, 0ch
        call   sub_3_1EDB
        mov    edi, dword ptr [ebp - 8]
        mov    esi, dword ptr [edi + 4]
        and    word ptr [esi + 16h], 0fffeh
        call   sub_3_05DE
        mov    edi, dword ptr [ebp - 8]
        mov    word ptr [edi + 6ah], ax
        test   byte ptr [esi + 24h], 40h
        je     SHORT loc_3_05DC
        call   sub_3_124D
loc_3_05DC:
        leave
        ret
sub_3_05DE:
        sub    esp, 14h
        mov    dword ptr [esp + 4], 0
        mov    dword ptr [esp], 0
        and    byte ptr [esi + 24h], 0efh
        mov    edx, dword ptr [esi + 0dbh]
        add    edx, 44h
        call   sub_3_1A35
        or     ebx, ebx
        je     NEAR PTR loc_3_09F0
        mov    dword ptr [esp + 0ch], ebx
        add    ebx, 44h
        mov    dword ptr [esp + 10h], ebx
        xor    eax, eax
        mov    dx, 0
        call   sub_3_1EDB
        cmp    ax, 10h
        jb     NEAR PTR loc_3_06D6
        jmp    NEAR PTR loc_3_09F4
loc_3_062F:
        test   byte ptr [esi + 16h], 20h
        jne    NEAR PTR loc_3_0857
        db  08Dh, 015h, 0A0h, 005h, 000h, 000h
        sub    esp, 18h
        mov    ecx, esp
        mov    word ptr [ecx], 11h
        mov    dword ptr [ecx + 4], esi
        mov    dword ptr [ecx + 0ch], 3
        mov    dword ptr [ecx + 10h], 1
        mov    dword ptr [ecx + 14h], edx
        db  0C7h, 041h, 008h, 0A1h, 005h, 000h, 000h
        push   ecx
        db  0FFh, 015h, 050h, 000h, 000h, 000h
        movzx  ecx, word ptr [ecx + 2]
        add    esp, 1ch
        or     ecx, ecx
        jne    NEAR PTR loc_3_0857
        db  080h, 03Dh, 0A0h, 005h, 000h, 000h, 000h
        je     NEAR PTR loc_3_0857
        cmp    dword ptr [esi + 0e7h], 0
        je     NEAR PTR loc_3_0857
        cmp    dword ptr [esi + 0dfh], 0
        je     NEAR PTR loc_3_0857
        cmp    dword ptr [esi + 0e3h], 0
        je     NEAR PTR loc_3_0857
        mov    ecx, dword ptr [esp]
        mov    al, 7fh
        or     ecx, ecx
        je     SHORT loc_3_06C0
        mov    cl, byte ptr [ecx]
        test   cl, 80h
        je     SHORT loc_3_06C0
        mov    al, cl
loc_3_06C0:
        db  0FEh, 005h, 0A7h, 005h, 000h, 000h
        db  002h, 005h, 0A7h, 005h, 000h, 000h
        or     word ptr [esi + 16h], 48h
        jmp    NEAR PTR loc_3_0787
loc_3_06D6:
        cmp    dword ptr [esi + 0dbh], 200h
        je     SHORT loc_3_071F
        mov    ebx, dword ptr [esp + 10h]
        cmp    dword ptr [ebx + 20h], 70616441h
        jne    SHORT loc_3_071F
        cmp    dword ptr [ebx + 24h], 2c636574h
        jne    SHORT loc_3_071F
        or     word ptr [esi + 16h], 1
        or     byte ptr [esi + 24h], 10h
        db  0A1h, 06Ch, 000h, 000h, 000h
        test   byte ptr [eax + 2ah], 10h
        je     NEAR PTR loc_3_0862
        cmp    dword ptr [esi + 10h], 0
        jne    NEAR PTR loc_3_0862
        jmp    NEAR PTR loc_3_09DB
loc_3_071F:
        bts    dword ptr [esi + 20h], 18h
        jb     NEAR PTR loc_3_0857
        cmp    byte ptr [esi + 40h], 0ah
        je     NEAR PTR loc_3_0857
        mov    ebx, dword ptr [esp + 10h]
        db  0A1h, 06Ch, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 38h]
loc_3_0740:
        mov    ecx, dword ptr [ebx + 0dch]
        cmp    byte ptr [eax], 0ffh
        je     NEAR PTR loc_3_062F
        mov    dword ptr [esp], eax
        cmp    dword ptr [eax + 1], ecx
        jne    SHORT loc_3_077C
        test   word ptr [eax + 9], 2
        jne    SHORT loc_3_077C
        test   word ptr [eax + 9], 1
        je     SHORT loc_3_0781
        xor    edx, edx
        mov    ecx, 200h
loc_3_076E:
        add    edx, dword ptr [ecx + ebx - 4]
        sub    ecx, 4
        jne    SHORT loc_3_076E
        cmp    dword ptr [eax + 5], edx
        je     SHORT loc_3_0781
loc_3_077C:
        add    eax, 0bh
        jmp    SHORT loc_3_0740
loc_3_0781:
        or     byte ptr [eax + 9], 2
        mov    al, byte ptr [eax]
loc_3_0787:
        mov    byte ptr [esi + 15h], al
        sub    esp, 0ch
        mov    ecx, esp
        mov    byte ptr [ecx + 8], al
        mov    byte ptr [ecx + 9], 0
        mov    dword ptr [ecx + 4], esi
        mov    word ptr [ecx], 6
        push   ecx
        db  0FFh, 015h, 050h, 000h, 000h, 000h
        add    esp, 10h
        or     dword ptr [esi + 20h], 1
        test   word ptr [esi + 16h], 8
        jne    NEAR PTR loc_3_0857
        db  08Bh, 03Dh, 06Ch, 000h, 000h, 000h
        mov    edi, dword ptr [edi + 2ch]
        or     edi, edi
        je     SHORT loc_3_0825
        mov    al, byte ptr [esi + 15h]
loc_3_07C8:
        cmp    byte ptr [edi], 0
        jne    SHORT loc_3_07D2
        cmp    al, byte ptr [edi + 5]
        je     SHORT loc_3_07DD
loc_3_07D2:
        cmp    dword ptr [edi + 1], 0
        je     SHORT loc_3_0825
        add    edi, dword ptr [edi + 1]
        jmp    SHORT loc_3_07C8
loc_3_07DD:
        test   word ptr [esi + 16h], 8000h
        je     SHORT loc_3_07E9
        cmp    al, 80h
        jne    SHORT loc_3_0825
loc_3_07E9:
        movzx  eax, word ptr [edi + 6]
        mov    dword ptr [esi + 0e3h], eax
        movzx  eax, word ptr [edi + 8]
        mov    dword ptr [esi + 0dfh], eax
        movzx  eax, word ptr [edi + 0ah]
        mov    dword ptr [esi + 0e7h], eax
        mul    dword ptr [esi + 0dfh]
        mul    dword ptr [esi + 0e3h]
        mov    dword ptr [esi + 0d3h], eax
        mov    dword ptr [esi + 0d7h], edx
        or     word ptr [esi + 16h], 1000h
loc_3_0825:
        test   word ptr [esi + 16h], 4000h
        jne    SHORT loc_3_0857
        mov    eax, dword ptr [esi + 0e3h]
        mov    dword ptr [esi + 64h], eax
        mov    eax, dword ptr [esi + 0dfh]
        mov    dword ptr [esi + 60h], eax
        mov    eax, dword ptr [esi + 0e7h]
        mov    dword ptr [esi + 68h], eax
        mov    eax, dword ptr [esi + 0d3h]
        mov    dword ptr [esi + 54h], eax
        or     word ptr [esi + 16h], 2000h
loc_3_0857:
        cmp    word ptr [ebx + 1feh], 0aa55h
        je     SHORT loc_3_0880
loc_3_0862:
        or     word ptr [esi + 16h], 1
        cmp    byte ptr [esi + 40h], 0ah
        je     NEAR PTR loc_3_09DB
        xor    ebx, ebx
        mov    edi, dword ptr [esi + 10h]
        call   sub_3_0BAA
        jmp    NEAR PTR loc_3_09DB
loc_3_0880:
        cmp    byte ptr [esi + 40h], 0ah
        je     SHORT loc_3_08D3
        mov    eax, esi
        mov    edi, dword ptr [esp + 0ch]
        mov    esi, ebx
        add    esi, 1beh
        mov    ecx, 42h
        rep movsb
        mov    esi, eax
        add    ebx, 1beh
        mov    al, 2
        call   sub_3_1208
        or     eax, eax
        je     SHORT loc_3_08B0
        jmp    SHORT loc_3_0904
loc_3_08B0:
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esp + 8], eax
        mov    ebx, dword ptr [esp + 10h]
        mov    dx, 0
        call   sub_3_1EDB
        cmp    ax, 10h
        jb     SHORT loc_3_08CF
        jmp    NEAR PTR loc_3_09DB
loc_3_08CF:
        mov    ebx, dword ptr [esp + 10h]
loc_3_08D3:
        test   byte ptr [esi + 21h], 40h
        je     SHORT loc_3_0904
        test   byte ptr [esi + 23h], 2
        jne    SHORT loc_3_0904
        push   esi
        push   edi
        mov    edi, dword ptr [esi + 30h]
        lea    esi, [ebx + 0bh]
        lea    edi, [edi + 6]
        mov    ecx, 19h
        cmp    word ptr [esi + 0bh], 0
        jne    SHORT loc_3_08FB
        mov    ecx, 35h
loc_3_08FB:
        rep movsb
        pop    edi
        pop    esi
        jmp    NEAR PTR loc_3_09DB
loc_3_0904:
        cmp    byte ptr [esi + 40h], 0ah
        je     NEAR PTR loc_3_09DB
        mov    ebx, dword ptr [esp + 0ch]
        mov    al, 0
        call   sub_3_1208
        mov    edi, dword ptr [esi + 10h]
        or     eax, eax
        jne    SHORT loc_3_093D
        xor    eax, eax
        call   sub_3_0BAA
        or     byte ptr [esp + 4], 1
        or     edi, edi
        je     SHORT loc_3_0985
        mov    edi, dword ptr [edi + 10h]
        or     edi, edi
        je     NEAR PTR loc_3_09DB
        jmp    SHORT loc_3_0985
loc_3_093D:
        mov    ebx, dword ptr [esp + 0ch]
        mov    ecx, 4
loc_3_0946:
        mov    al, 1
        call   sub_3_120D
        or     eax, eax
        je     SHORT loc_3_095B
        bts    dword ptr [esp + 4], 0
        jae    SHORT loc_3_0985
        jmp    SHORT loc_3_09A0
loc_3_095B:
        push   ecx
        push   ebx
        xor    eax, eax
        call   sub_3_0BAA
        pop    ebx
        pop    ecx
        or     edi, edi
        je     SHORT loc_3_0971
        mov    edi, dword ptr [edi + 10h]
        or     edi, edi
        je     SHORT loc_3_09DB
loc_3_0971:
        mov    byte ptr [ebx + 4], 0ffh
        bts    dword ptr [esp + 4], 0
        jae    SHORT loc_3_0985
        add    ebx, 10h
        dec    ecx
        je     SHORT loc_3_09A0
        jmp    SHORT loc_3_0946
loc_3_0985:
        mov    ebx, dword ptr [esp + 0ch]
        call   sub_3_11E9
        or     eax, eax
        jne    SHORT loc_3_093D
        call   sub_3_0AA0
        mov    edi, eax
        cmp    eax, -1
        jne    SHORT loc_3_093D
        xor    edi, edi
loc_3_09A0:
        or     edi, edi
        je     SHORT loc_3_09B6
        or     word ptr [edi + 16h], 81h
        mov    dword ptr [edi + 28h], 0
        mov    edi, dword ptr [edi + 10h]
        jmp    SHORT loc_3_09A0
loc_3_09B6:
        test   byte ptr [esi + 20h], 4
        je     SHORT loc_3_09DB
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     SHORT loc_3_09CF
        test   byte ptr [edi + 16h], 1
        je     SHORT loc_3_09DB
        cmp    dword ptr [edi + 28h], 0
        jne    SHORT loc_3_09DB
loc_3_09CF:
        mov    ebx, 0ffffffffh
        xor    eax, eax
        call   sub_3_0BAA
loc_3_09DB:
        call   sub_3_0A1A
        call   sub_3_10DA
        xor    eax, eax
loc_3_09E7:
        mov    edi, dword ptr [esp + 0ch]
        call   sub_3_1A61
loc_3_09F0:
        add    esp, 14h
        ret
loc_3_09F4:
        or     word ptr [esi + 16h], 21h
        cmp    byte ptr [esi + 40h], 0ah
        je     SHORT loc_3_0A0E
        xor    ebx, ebx
        mov    edi, dword ptr [esi + 10h]
        call   sub_3_0BAA
        call   sub_3_0A1A
loc_3_0A0E:
        cmp    ax, 1ch
        je     SHORT loc_3_09E7
        mov    ax, 18h
        jmp    SHORT loc_3_09E7
sub_3_0A1A:
        test   byte ptr [esi + 20h], 4
        je     SHORT loc_3_0A5E
        bts    word ptr [esi + 16h], 1
        jb     SHORT loc_3_0A5E
        cmp    byte ptr [esi + 40h], 0ah
        je     SHORT loc_3_0A5E
loc_3_0A2E:
        mov    al, 1
        call   sub_3_202D
        or     ah, ah
        jne    SHORT loc_3_0A5E
        push   eax
        call   sub_3_2088
        pop    eax
        mov    word ptr [edi + 16h], 81h
        call   sub_3_2051
        lea    eax, [esi + 10h]
loc_3_0A4E:
        cmp    dword ptr [eax], 0
        je     SHORT loc_3_0A5A
        mov    eax, dword ptr [eax]
        lea    eax, [eax + 10h]
        jmp    SHORT loc_3_0A4E
loc_3_0A5A:
        mov    dword ptr [eax], edi
        jmp    SHORT loc_3_0A2E
loc_3_0A5E:
        ret
sub_3_0A5F:
        test   dword ptr [esi + 20h], 4000h
        jne    SHORT sub_3_0A72
        or     dword ptr [esi + 20h], 4000h
        mov    byte ptr [esi + 15h], dl
sub_3_0A72:
        sub    esp, 0ch
        mov    ebx, esp
        mov    byte ptr [ebx + 8], dl
        mov    dword ptr [ebx + 4], esi
        mov    word ptr [ebx], 6
        mov    byte ptr [ebx + 9], 0
        test   byte ptr [esi + 1dh], 20h
        je     SHORT loc_3_0A95
        cmp    dl, 1
        jne    SHORT loc_3_0A95
        or     byte ptr [ebx + 9], 1
loc_3_0A95:
        push   ebx
        db  0FFh, 015h, 050h, 000h, 000h, 000h
        add    esp, 10h
        ret
sub_3_0AA0:
        sub    esp, 24h
        mov    dword ptr [esp + 18h], edi
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esp + 14h], eax
        mov    dword ptr [esp + 10h], 0
        mov    ecx, ebx
        mov    edx, dword ptr [esi + 0dbh]
        call   sub_3_1A35
        or     ebx, ebx
        je     NEAR PTR loc_3_0BA2
        mov    dword ptr [esp + 1ch], ebx
loc_3_0ACF:
        mov    edi, esi
        mov    ebx, dword ptr [esp + 1ch]
        mov    eax, dword ptr [esp + 14h]
        add    eax, dword ptr [esp + 10h]
        mov    dx, 0
        call   sub_3_1EDB
        cmp    ax, 10h
        jae    SHORT loc_3_0AF7
        cmp    word ptr [ebx + 1feh], 0aa55h
        je     SHORT loc_3_0B21
loc_3_0AF7:
        mov    ebx, ecx
        mov    ch, byte ptr [ebx + 1]
        mov    cl, byte ptr [ebx + 2]
        mov    al, cl
        and    cl, 3fh
        and    ax, 0c0h
        shl    ax, 2
        mov    al, byte ptr [ebx + 3]
        call   sub_3_1A7B
        mov    ebx, dword ptr [esp + 1ch]
        mov    dx, 0
        call   sub_3_1EDB
loc_3_0B21:
        cmp    ax, 10h
        jae    SHORT loc_3_0B97
        add    ebx, 1beh
        push   ebx
        call   sub_3_11E9
        mov    dword ptr [esp + 24h], eax
        push   esi
        push   edi
        mov    esi, ebx
        lea    edi, [esp + 0ch]
        mov    ecx, 10h
        rep movsb
        pop    edi
        pop    esi
        pop    ebx
        mov    al, 2
        call   sub_3_1208
        or     eax, eax
        jne    SHORT loc_3_0B81
        mov    edi, dword ptr [esp + 18h]
        or     edi, edi
        je     SHORT loc_3_0B74
        cmp    edi, -1
        je     SHORT loc_3_0B97
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esp + 18h], eax
        or     eax, eax
        jne    SHORT loc_3_0B74
        mov    dword ptr [esp + 18h], 0ffffffffh
loc_3_0B74:
        mov    eax, dword ptr [esp + 14h]
        add    eax, dword ptr [esp + 10h]
        call   sub_3_0BAA
loc_3_0B81:
        cmp    dword ptr [esp + 20h], 0
        jne    SHORT loc_3_0B97
        lea    ecx, [esp]
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [esp + 10h], eax
        jmp    NEAR PTR loc_3_0ACF
loc_3_0B97:
        push   edi
        mov    edi, dword ptr [esp + 20h]
        call   sub_3_1A61
        pop    edi
loc_3_0BA2:
        mov    eax, dword ptr [esp + 18h]
        add    esp, 24h
        ret
sub_3_0BAA:
        sub    esp, 1ch
        push   edi
        mov    dword ptr [esp + 18h], esi
        mov    dword ptr [esp + 14h], ebx
        mov    dword ptr [esp + 0ch], eax
        mov    byte ptr [esp + 8], 0ffh
        mov    byte ptr [esp + 4], 0
        mov    dword ptr [esp + 1ch], 0
        cmp    dword ptr [esp + 14h], 0
        je     NEAR PTR loc_3_0D1E
        push   ebx
        mov    edx, dword ptr [esi + 0dbh]
        call   sub_3_1A35
        mov    dword ptr [esp + 20h], ebx
        or     ebx, ebx
        pop    ebx
        je     NEAR PTR loc_3_103C
        mov    ebx, dword ptr [esp + 1ch]
        mov    eax, dword ptr [esp + 14h]
        cmp    eax, -1
        jne    SHORT loc_3_0C01
        xor    eax, eax
        jmp    SHORT loc_3_0C08
loc_3_0C01:
        mov    eax, dword ptr [eax + 8]
        add    eax, dword ptr [esp + 0ch]
loc_3_0C08:
        mov    dword ptr [esp + 10h], eax
        mov    dx, 0
        call   sub_3_1EDB
        cmp    ax, 10h
        jae    SHORT loc_3_0C24
        call   sub_3_19DC
        or     eax, eax
        je     SHORT loc_3_0C6B
loc_3_0C24:
        mov    ebx, dword ptr [esp + 14h]
        cmp    ebx, -1
        je     NEAR PTR loc_3_0D1E
        mov    ch, byte ptr [ebx + 1]
        mov    cl, byte ptr [ebx + 2]
        mov    al, cl
        and    cl, 3fh
        and    ax, 0c0h
        shl    ax, 2
        mov    al, byte ptr [ebx + 3]
        xchg   esi, edi
        call   sub_3_1A7B
        xchg   esi, edi
        mov    dword ptr [esp + 10h], eax
        mov    ebx, dword ptr [esp + 1ch]
        mov    dx, 0
        call   sub_3_1EDB
        cmp    ax, 10h
        jae    NEAR PTR loc_3_0D1E
loc_3_0C6B:
        or     edi, edi
        jne    NEAR PTR loc_3_0EC4
        mov    ebx, dword ptr [esp + 1ch]
        call   sub_3_19DC
        or     eax, eax
        jne    NEAR PTR loc_3_0D1E
        mov    edi, dword ptr [esp + 10h]
        cmp    word ptr [ebx + 16h], 0
        jne    SHORT loc_3_0C9C
        cmp    byte ptr [ebx + 42h], 29h
        jne    SHORT loc_3_0D08
        mov    eax, dword ptr [ebx + 43h]
        or     eax, eax
        jne    SHORT loc_3_0CA9
loc_3_0C9C:
        cmp    byte ptr [ebx + 26h], 29h
        jne    SHORT loc_3_0D08
        mov    eax, dword ptr [ebx + 27h]
        or     eax, eax
        je     SHORT loc_3_0D08
loc_3_0CA9:
        db  08Bh, 01Dh, 06Ch, 000h, 000h, 000h
        mov    ebx, dword ptr [ebx + 34h]
loc_3_0CB2:
        cmp    byte ptr [ebx], 0ffh
        je     NEAR PTR loc_3_0E4C
        cmp    dword ptr [ebx + 2], eax
        jne    SHORT loc_3_0D03
        cmp    byte ptr [ebx + 1], 0ffh
        je     SHORT loc_3_0CD3
        mov    cl, byte ptr [esi + 15h]
        cmp    byte ptr [ebx + 1], cl
        jne    SHORT loc_3_0D03
        cmp    edi, dword ptr [ebx + 6]
        jne    SHORT loc_3_0D03
loc_3_0CD3:
        mov    cx, word ptr [esi + 7ch]
        cmp    byte ptr [ebx + 0ah], 0ffh
        je     SHORT loc_3_0CE7
        cmp    byte ptr [ebx + 0ah], cl
        jne    SHORT loc_3_0D03
        cmp    byte ptr [ebx + 0bh], ch
        jne    SHORT loc_3_0D03
loc_3_0CE7:
        cmp    byte ptr [ebx + 0dh], 0ffh
        je     SHORT loc_3_0CF7
        cmp    byte ptr [ebx + 0dh], cl
        jne    SHORT loc_3_0D03
        cmp    byte ptr [ebx + 0eh], ch
        jne    SHORT loc_3_0D03
loc_3_0CF7:
        bts    word ptr [ebx + 10h], 0
        jae    NEAR PTR loc_3_0E20
loc_3_0D03:
        add    ebx, 12h
        jmp    SHORT loc_3_0CB2
loc_3_0D08:
        xor    eax, eax
        mov    edx, dword ptr [esp + 1ch]
        mov    ecx, 200h
loc_3_0D13:
        add    eax, dword ptr [ecx + edx - 4]
        sub    ecx, 4
        jne    SHORT loc_3_0D13
        jmp    SHORT loc_3_0CA9
loc_3_0D1E:
        or     edi, edi
        jne    NEAR PTR loc_3_0EC4
        mov    esi, dword ptr [esp + 18h]
        mov    al, 3
        call   sub_3_202D
        or     ah, ah
        jne    SHORT loc_3_0D3D
        movzx  eax, al
        jmp    NEAR PTR loc_3_0E23
loc_3_0D3D:
        db  08Bh, 01Dh, 06Ch, 000h, 000h, 000h
        mov    ebx, dword ptr [ebx + 34h]
loc_3_0D46:
        cmp    byte ptr [ebx], 0ffh
        je     NEAR PTR loc_3_0DF0
        test   byte ptr [ebx + 10h], 2
        je     NEAR PTR loc_3_0DE8
        cmp    byte ptr [esi + 74h], 1
        jne    SHORT loc_3_0D6B
        cmp    byte ptr [ebx + 0dh], 0ffh
        jne    SHORT loc_3_0D70
        cmp    byte ptr [ebx + 0ah], 0ffh
        jne    SHORT loc_3_0D70
loc_3_0D6B:
        or     byte ptr [esp + 4], 2
loc_3_0D70:
        test   byte ptr [esi + 20h], 1
        je     SHORT loc_3_0D9A
        cmp    byte ptr [ebx + 1], 0ffh
        je     SHORT loc_3_0D9A
        mov    cl, byte ptr [esi + 15h]
        cmp    byte ptr [ebx + 1], cl
        jne    SHORT loc_3_0DE8
loc_3_0D84:
        movzx  eax, byte ptr [ebx]
        bts    word ptr [ebx + 10h], 0
        setb   ah
        or     ah, ah
        je     NEAR PTR loc_3_0E23
        jmp    SHORT loc_3_0DE8
loc_3_0D9A:
        cmp    byte ptr [esi + 74h], 1
        jne    SHORT loc_3_0DE8
        mov    cx, word ptr [esi + 7ch]
        cmp    byte ptr [ebx + 0dh], 0ffh
        je     SHORT loc_3_0DC6
        cmp    byte ptr [ebx + 0dh], cl
        jne    SHORT loc_3_0DE8
        cmp    byte ptr [ebx + 0eh], ch
        jne    SHORT loc_3_0DE8
        mov    al, 3
        call   sub_3_1045
        jb     SHORT loc_3_0DE8
        mov    cl, byte ptr [ebx + 0fh]
        cmp    byte ptr [eax], cl
        jne    SHORT loc_3_0DE8
        jmp    SHORT loc_3_0D84
loc_3_0DC6:
        cmp    byte ptr [ebx + 0ah], 0ffh
        je     SHORT loc_3_0DE8
        cmp    byte ptr [ebx + 0ah], cl
        jne    SHORT loc_3_0DE8
        cmp    byte ptr [ebx + 0bh], ch
        jne    SHORT loc_3_0DE8
        mov    al, 1
        call   sub_3_1045
        jb     SHORT loc_3_0DE8
        mov    cl, byte ptr [ebx + 0ch]
        cmp    byte ptr [eax], cl
        jne    SHORT loc_3_0DE8
        jmp    SHORT loc_3_0D84
loc_3_0DE8:
        add    ebx, 12h
        jmp    NEAR PTR loc_3_0D46
loc_3_0DF0:
        test   byte ptr [esp + 4], 2
        jne    NEAR PTR loc_3_102F
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_3_0E0C
        test   byte ptr [esi + 20h], 4
        je     NEAR PTR loc_3_102F
loc_3_0E0C:
        xor    al, al
        call   sub_3_202D
        or     ah, ah
        jne    NEAR PTR loc_3_102F
        movzx  eax, al
        jmp    SHORT loc_3_0E23
loc_3_0E20:
        movzx  eax, byte ptr [ebx]
loc_3_0E23:
        mov    byte ptr [esp + 8], al
        db  08Bh, 035h, 06Ch, 000h, 000h, 000h
        call   sub_3_200B
        xchg   esi, edi
        or     eax, eax
        jne    SHORT loc_3_0E4C
        test   dword ptr [edi + 20h], 400000h
        jne    NEAR PTR loc_3_102F
        mov    esi, edi
        call   sub_3_2070
loc_3_0E4C:
        mov    esi, dword ptr [esp + 18h]
        call   sub_3_2088
        mov    al, byte ptr [esp + 8]
        cmp    al, 0ffh
        jne    SHORT loc_3_0E89
        bts    word ptr [esi + 16h], 2
        jb     SHORT loc_3_0E76
        mov    al, 1
        call   sub_3_202D
        or     ah, ah
        jne    SHORT loc_3_0E76
        or     byte ptr [esi + 16h], 10h
        jmp    SHORT loc_3_0E89
loc_3_0E76:
        xor    al, al
        test   byte ptr [esi + 16h], 10h
        je     SHORT loc_3_0E80
        mov    al, 1
loc_3_0E80:
        call   sub_3_202D
        or     ah, ah
        jne    SHORT loc_3_0E94
loc_3_0E89:
        call   sub_3_2051
        db  0FEh, 005h, 0B2h, 002h, 000h, 000h
loc_3_0E94:
        mov    esi, dword ptr [esi + 8]
        call   sub_3_20C9
        mov    dword ptr [edi + 8], edx
        mov    esi, dword ptr [esp + 18h]
        mov    dword ptr [edi], esi
        lea    eax, [esi + 10h]
loc_3_0EA8:
        cmp    dword ptr [eax], 0
        je     SHORT loc_3_0EB4
        mov    eax, dword ptr [eax]
        lea    eax, [eax + 10h]
        jmp    SHORT loc_3_0EA8
loc_3_0EB4:
        mov    dword ptr [eax], edi
        mov    dword ptr [edi + 10h], 0
        mov    esi, edi
        call   sub_3_1FAD
loc_3_0EC4:
        cmp    dword ptr [esp + 14h], 0
        je     NEAR PTR loc_3_0FE4
        mov    ecx, dword ptr [edi + 30h]
        mov    ebx, dword ptr [esp + 14h]
        cmp    ebx, -1
        je     SHORT loc_3_0F2F
        mov    al, byte ptr [ebx + 4]
        mov    byte ptr [edi + 3dh], al
        mov    eax, dword ptr [ebx + 8]
        test   byte ptr [edi + 20h], 4
        jne    SHORT loc_3_0EF0
        test   byte ptr [edi + 17h], 2
        je     SHORT loc_3_0EF6
loc_3_0EF0:
        mov    dword ptr [ecx + 17h], eax
        mov    dword ptr [ecx + 54h], eax
loc_3_0EF6:
        mov    word ptr [ecx + 0eh], 0
        mov    word ptr [ecx + 4bh], 0
        mov    dword ptr [ecx + 1bh], 0
        mov    dword ptr [ecx + 58h], 0
        mov    ax, word ptr [ebx + 0eh]
        or     ax, ax
        je     SHORT loc_3_0F24
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [ecx + 1bh], eax
        mov    dword ptr [ecx + 58h], eax
        jmp    SHORT loc_3_0F2F
loc_3_0F24:
        mov    eax, dword ptr [ebx + 0ch]
        mov    word ptr [ecx + 0eh], ax
        mov    word ptr [ecx + 4bh], ax
loc_3_0F2F:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [edi + 28h], eax
        and    word ptr [edi + 16h], 0fffeh
        mov    ebx, dword ptr [esp + 1ch]
        call   sub_3_19DC
        or     eax, eax
        jne    NEAR PTR loc_3_0FE4
        and    word ptr [edi + 16h], 0ff7fh
        test   byte ptr [edi + 20h], 4
        jne    SHORT loc_3_0F62
        test   byte ptr [edi + 17h], 2
        je     NEAR PTR loc_3_102F
loc_3_0F62:
        push   edi
        lea    esi, [ebx + 0bh]
        mov    edi, dword ptr [edi + 30h]
        lea    edi, [edi + 6]
        mov    ecx, 19h
        cmp    word ptr [esi + 0bh], 0
        jne    SHORT loc_3_0F7D
        mov    ecx, 35h
loc_3_0F7D:
        rep movsb
        pop    edi
        mov    esi, dword ptr [esp + 18h]
        push   edi
        mov    edi, dword ptr [edi + 30h]
        mov    ecx, dword ptr [esi + 0e7h]
        or     ecx, ecx
        je     SHORT loc_3_0FDB
        mov    word ptr [edi + 13h], cx
        mov    ecx, dword ptr [esi + 0dfh]
        mov    word ptr [edi + 15h], cx
        or     ecx, ecx
        je     SHORT loc_3_0FDB
        xor    edx, edx
        mov    ax, word ptr [edi + 0eh]
        or     ax, ax
        jne    SHORT loc_3_0FB7
        mov    dx, word ptr [edi + 1dh]
        mov    ax, word ptr [edi + 1bh]
loc_3_0FB7:
        push   edx
        push   eax
        mov    ax, word ptr [edi + 15h]
        mul    word ptr [edi + 13h]
        mov    ecx, eax
        pop    eax
        pop    edx
        push   eax
        mov    eax, edx
        xor    edx, edx
        div    cx
        pop    eax
        div    cx
        or     dx, dx
        je     SHORT loc_3_0FD7
        inc    eax
loc_3_0FD7:
        mov    word ptr [edi + 41h], ax
loc_3_0FDB:
        pop    edi
        test   byte ptr [edi + 17h], 2
        jne    SHORT loc_3_102A
        jmp    SHORT loc_3_102F
loc_3_0FE4:
        or     edi, edi
        je     SHORT loc_3_102F
        mov    ax, word ptr [edi + 16h]
        or     ax, 1
        and    ax, 201h
        mov    word ptr [edi + 16h], ax
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_3_102A
        mov    dword ptr [edi + 28h], 0
        mov    eax, dword ptr [edi + 30h]
        mov    word ptr [eax + 0eh], 0
        mov    dword ptr [eax + 1bh], 0
        mov    word ptr [eax + 4bh], 0
        mov    dword ptr [eax + 58h], 0
        mov    dword ptr [eax + 54h], 0
loc_3_102A:
        call   sub_3_1C34
loc_3_102F:
        mov    edi, dword ptr [esp + 1ch]
        or     edi, edi
        je     SHORT loc_3_103C
        call   sub_3_1A61
loc_3_103C:
        mov    esi, dword ptr [esp + 18h]
        pop    edi
        add    esp, 1ch
        ret
sub_3_1045:
        sub    esp, 4
        mov    byte ptr [esp], al
        pushad
        db  08Bh, 00Dh, 06Ch, 000h, 000h, 000h
        mov    edi, dword ptr [ecx + 50h]
loc_3_1055:
        or     edi, edi
        je     SHORT loc_3_10A8
        mov    al, byte ptr [esp + 20h]
        cmp    byte ptr [edi], al
        jne    SHORT loc_3_10A3
        mov    edx, dword ptr [edi + 5]
loc_3_1064:
        or     edx, edx
        je     SHORT loc_3_10A3
        cmp    byte ptr [edx + 4], 0
        jne    SHORT loc_3_109E
        mov    cx, word ptr [esi + 7ch]
        cmp    word ptr [edx + 1], cx
        jne    SHORT loc_3_109E
        lea    ebx, [esi + 82h]
        movzx  eax, byte ptr [ebx]
        mov    ecx, 23h
loc_3_1086:
        add    al, byte ptr [ecx + ebx]
        adc    ah, 0
        loop   loc_3_1086
        cmp    word ptr [edx + 0ch], ax
        jne    SHORT loc_3_10A3
        clc
        mov    dword ptr [esp + 1ch], edx
loc_3_1099:
        popad
        add    esp, 4
        ret
loc_3_109E:
        mov    edx, dword ptr [edx + 10h]
        jmp    SHORT loc_3_1064
loc_3_10A3:
        mov    edi, dword ptr [edi + 1]
        jmp    SHORT loc_3_1055
loc_3_10A8:
        stc
        jmp    SHORT loc_3_1099
sub_3_10AB:
        movzx  ecx, byte ptr [edi + 15h]
        push   ecx
        movzx  ecx, byte ptr [edi + 14h]
        push   ecx
        push   dword ptr [edi + 0ch]
        push   edi
        push   esi
        mov    ecx, 50h
        rep movsb
        pop    esi
        pop    edi
        pop    dword ptr [edi + 0ch]
        pop    ecx
        mov    byte ptr [edi + 14h], cl
        pop    ecx
        mov    byte ptr [edi + 15h], cl
        xor    dword ptr [edi + 20h], 0c000h
        and    byte ptr [edi + 21h], 0dfh
        ret
sub_3_10DA:
        sub    esp, 0ch
        push   ebx
        push   edi
        push   esi
        mov    word ptr [esp + 0ch], 0bh
        mov    dword ptr [esp + 10h], esi
        lea    eax, [esp + 0ch]
        push   eax
        db  0FFh, 015h, 050h, 000h, 000h, 000h
        add    esp, 4
        cmp    word ptr [esp + 0eh], 0
        jne    NEAR PTR loc_3_11E2
loc_3_1105:
        bsf    eax, dword ptr [esp + 14h]
        je     NEAR PTR loc_3_11E2
        btr    dword ptr [esp + 14h], eax
        db  08Bh, 01Dh, 06Ch, 000h, 000h, 000h
        mov    ebx, dword ptr [ebx + 34h]
loc_3_111E:
        cmp    byte ptr [ebx], 0ffh
        je     SHORT loc_3_1105
        cmp    byte ptr [ebx], al
        je     SHORT loc_3_112C
        add    ebx, 12h
        jmp    SHORT loc_3_111E
loc_3_112C:
        test   word ptr [ebx + 10h], 2
        bts    word ptr [ebx + 10h], 0
        jb     SHORT loc_3_1105
        db  08Bh, 00Dh, 06Ch, 000h, 000h, 000h
        bts    dword ptr [ecx + 44h], eax
        call   sub_3_200B
        or     eax, eax
        jne    SHORT loc_3_1105
        mov    edi, esi
        db  0FEh, 005h, 0B2h, 002h, 000h, 000h
        test   byte ptr [edi + 22h], 40h
        jne    SHORT loc_3_1105
        db  08Bh, 035h, 06Ch, 000h, 000h, 000h
        mov    esi, dword ptr [esi + 3ch]
        mov    eax, dword ptr [esi + 10h]
loc_3_1167:
        or     eax, eax
        je     SHORT loc_3_1105
        cmp    eax, edi
        je     SHORT loc_3_1176
        mov    esi, eax
        mov    eax, dword ptr [esi + 10h]
        jmp    SHORT loc_3_1167
loc_3_1176:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
        mov    esi, dword ptr [edi + 8]
loc_3_117F:
        or     esi, esi
        je     SHORT loc_3_119A
        cmp    byte ptr [esi + 13h], 1fh
        je     SHORT loc_3_1195
        push   0
        push   esi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_3_1195:
        mov    esi, dword ptr [esi + 0ch]
        jmp    SHORT loc_3_117F
loc_3_119A:
        mov    esi, dword ptr [esp + 10h]
        call   sub_3_10AB
        mov    esi, dword ptr [esi + 8]
        call   sub_3_20C9
        mov    dword ptr [edi + 8], edx
        mov    esi, dword ptr [esp + 10h]
        mov    dword ptr [edi], esi
        lea    eax, [esi + 10h]
loc_3_11B7:
        mov    ecx, dword ptr [eax]
        jecxz  loc_3_11C0
        lea    eax, [ecx + 10h]
        jmp    SHORT loc_3_11B7
loc_3_11C0:
        mov    dword ptr [eax], edi
        mov    dword ptr [edi + 10h], ecx
        mov    esi, edi
        call   sub_3_1FAD
        test   byte ptr [edi + 17h], 2
        je     SHORT loc_3_11D7
        call   sub_3_1C34
loc_3_11D7:
        and    word ptr [edi + 16h], 0ff7eh
        jmp    NEAR PTR loc_3_1105
loc_3_11E2:
        pop    esi
        pop    edi
        pop    ebx
        add    esp, 0ch
        ret
sub_3_11E9:
        mov    ecx, 4
loc_3_11EE:
        cmp    byte ptr [ebx + 4], 5
        je     SHORT loc_3_11FA
        cmp    byte ptr [ebx + 4], 0fh
        jne    SHORT loc_3_11FD
loc_3_11FA:
        xor    eax, eax
        ret
loc_3_11FD:
        add    ebx, 10h
        loop   loc_3_11EE
        mov    eax, 1
        ret
sub_3_1208:
        mov    ecx, 4
sub_3_120D:
        cmp    al, 2
        je     SHORT loc_3_1221
        cmp    al, 0
        jne    SHORT loc_3_121C
        cmp    byte ptr [ebx], 0
        je     SHORT loc_3_1245
        jmp    SHORT loc_3_1221
loc_3_121C:
        cmp    byte ptr [ebx], 0
        jne    SHORT loc_3_1245
loc_3_1221:
        mov    dl, byte ptr [ebx + 4]
        cmp    dl, 1
        je     SHORT loc_3_1242
        cmp    dl, 4
        je     SHORT loc_3_1242
        cmp    dl, 6
        je     SHORT loc_3_1242
        cmp    dl, 0bh
        je     SHORT loc_3_1242
        cmp    dl, 0ch
        je     SHORT loc_3_1242
        cmp    dl, 0eh
        jne    SHORT loc_3_1245
loc_3_1242:
        xor    eax, eax
        ret
loc_3_1245:
        add    ebx, 10h
        loop   sub_3_120D
        mov    al, 1
        ret
sub_3_124D:
        test   byte ptr [esi + 16h], 1
        je     SHORT loc_3_127F
        mov    eax, dword ptr [esi + 5ch]
        mov    dword ptr [esi + 0dbh], eax
        bsf    eax, eax
        mov    byte ptr [esi + 3ch], al
        mov    eax, dword ptr [esi + 60h]
        mov    dword ptr [esi + 0dfh], eax
        mov    eax, dword ptr [esi + 64h]
        mov    dword ptr [esi + 0e3h], eax
        mov    eax, dword ptr [esi + 68h]
        mov    dword ptr [esi + 0e7h], eax
        jmp    SHORT loc_3_12C6
loc_3_127F:
        mov    ecx, dword ptr [esi + 30h]
        jecxz  loc_3_12C6
        lea    ecx, [ecx + 6]
        movzx  eax, word ptr [ecx]
        mov    dword ptr [esi + 0dbh], eax
        bsf    eax, eax
        mov    byte ptr [esi + 3ch], al
        movzx  eax, word ptr [ecx + 0fh]
        mov    dword ptr [esi + 0dfh], eax
        movzx  eax, word ptr [ecx + 0dh]
        mov    dword ptr [esi + 0e7h], eax
        mul    dword ptr [esi + 0dfh]
        mov    ecx, eax
        mov    eax, dword ptr [esi + 0d3h]
        mov    edx, dword ptr [esi + 0d7h]
        div    ecx
        mov    dword ptr [esi + 0e3h], eax
loc_3_12C6:
        and    byte ptr [esi + 1eh], 0dfh
        cmp    dword ptr [esi + 0d7h], 0
        jne    SHORT loc_3_12E3
        cmp    dword ptr [esi + 0d3h], 1680h
        ja     SHORT loc_3_12E3
        or     byte ptr [esi + 1eh], 20h
loc_3_12E3:
        and    byte ptr [esi + 1eh], 0feh
        ret
sub_3_12E8:
        mov    dword ptr [eax], ebx
        add    byte ptr [eax], al
        mov    dword ptr [eax], ebx
        add    byte ptr [eax], al
        mov    dword ptr [eax], ebx
        add    byte ptr [eax], al
        mov    dword ptr [eax], ebx
        add    byte ptr [eax], al
        mov    dword ptr [eax], ebx
        add    byte ptr [eax], al
        mov    dword ptr [eax], ebx
        add    byte ptr [eax], al
        mov    dword ptr [eax], ebx
        add    byte ptr [eax], al
        mov    dword ptr [eax], ebx
        add    byte ptr [eax], al
        mov    dword ptr [eax], ebx
        add    byte ptr [eax], al
        mov    dword ptr [eax], ebx
        add    byte ptr [eax], al
        mov    dword ptr [eax], ebx
        add    byte ptr [eax], al
        mov    dword ptr [eax], ebx
        add    byte ptr [eax], al
        mov    dword ptr [eax], ebx
        add    byte ptr [eax], al
        push   ebx
        adc    eax, dword ptr [eax]
        db  000h, 089h, 018h, 000h, 000h, 089h
        sbb    byte ptr [eax], al
        db  000h, 089h, 018h, 000h, 000h, 07Fh
        adc    eax, dword ptr [eax]
loc_3_132F:
        add    byte ptr [eax + 68h], al
        pushad
        inc    edx
        inc    ecx
        db  062h, 061h
loc_3_1337:
        retf
        db  016h, 000h, 000h, 072h, 013h, 000h, 000h, 0EDh, 018h, 000h, 000h, 0A1h, 013h, 000h, 000h, 0D0h
        db  014h, 000h, 000h, 056h, 016h, 000h, 000h, 0D6h, 014h, 000h, 000h
loc_3_1353:
        xor    ecx, ecx
        mov    cl, 7
        mov    ebx, dword ptr [edi + 94h]
loc_3_135D:
        db  03Ah, 099h, 02Fh, 013h, 000h, 000h
        je     SHORT loc_3_136A
        loop   loc_3_135D
        mov    al, 2
        ret
loc_3_136A:
        dec    ecx
        db  0FFh, 024h, 08Dh, 037h, 013h, 000h, 000h
loc_3_1372:
        mov    dword ptr [edi + 0a0h], 1ah
        mov    al, 2
        ret
loc_3_137F:
        mov    ecx, 7
        mov    ebx, dword ptr [edi + 94h]
loc_3_138A:
        db  03Ah, 099h, 02Fh, 013h, 000h, 000h
        je     SHORT loc_3_1397
        loop   loc_3_138A
        mov    al, 2
        ret
loc_3_1397:
        dec    ecx
        mov    word ptr [edi + 6ah], 0
        xor    al, al
        ret
loc_3_13A1:
        push   esi
        mov    edx, dword ptr [edi + 98h]
        mov    esi, dword ptr [edi + 8]
        test   byte ptr [edx], 1
        jne    NEAR PTR loc_3_1444
        cmp    byte ptr [esi + 40h], 0
        je     NEAR PTR loc_3_1473
        test   byte ptr [esi + 24h], 40h
        je     SHORT loc_3_13E1
        cmp    dword ptr [esi + 0d7h], 0
        jne    NEAR PTR loc_3_1473
        cmp    dword ptr [esi + 0d3h], 1680h
        ja     NEAR PTR loc_3_1473
loc_3_13E1:
        mov    ebx, dword ptr [esi + 2ch]
        or     ebx, ebx
        je     NEAR PTR loc_3_14A9
loc_3_13EC:
        mov    ecx, dword ptr [esi + 0e7h]
        movzx  eax, word ptr [edx + 1]
        movzx  edx, word ptr [edx + 3]
        mov    dword ptr [edi + 60h], ecx
        or     ecx, ecx
        jne    SHORT loc_3_1404
        int3
        mov    cl, 1
loc_3_1404:
        mov    byte ptr [ebx + ecx*4 - 4], dl
        mov    byte ptr [ebx + ecx*4 - 3], al
        loop   loc_3_1404
        mov    word ptr [edi + 68h], 0eh
        mov    dword ptr [edi + 0ach], ebx
        mov    byte ptr [edi + 5ch], al
        mov    word ptr [edi + 5eh], dx
        mov    eax, dword ptr [edi + 14h]
        mov    ebx, dword ptr [edi + 10h]
        movsx  ebx, word ptr [ebx + 10h]
        add    ebx, edi
        mov    dword ptr [eax + 4], ebx
        mov    word ptr [ebx], 2
        db  0C7h, 000h, 06Fh, 001h, 000h, 000h
        add    dword ptr [edi + 14h], 8
        pop    esi
        mov    al, 2
        cld
        ret
loc_3_1444:
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [ecx + 68h]
        cmp    dword ptr [ecx + 0e7h], eax
        ja     SHORT loc_3_1494
        mov    eax, dword ptr [ecx + 54h]
        cmp    dword ptr [ecx + 0d3h], eax
        ja     SHORT loc_3_1494
        mov    dword ptr [edi + 0a0h], 0
        mov    byte ptr [edx], 0
        mov    word ptr [edi + 6ah], 0
loc_3_146F:
        pop    esi
        xor    al, al
        ret
loc_3_1473:
        mov    esi, dword ptr [edi + 4]
        test   byte ptr [esi + 24h], 10h
        je     SHORT loc_3_148E
        mov    dword ptr [edi + 0a0h], 5
        mov    word ptr [edi + 6ah], 10h
        jmp    SHORT loc_3_146F
loc_3_148E:
        pop    esi
        jmp    NEAR PTR loc_3_1656
loc_3_1494:
        mov    dword ptr [edi + 0a0h], 1
        mov    word ptr [edi + 6ah], 10h
        mov    byte ptr [edx], 1
        jmp    SHORT loc_3_146F
loc_3_14A9:
        push   edi
        mov    edx, 0fch
        call   sub_3_1A35
        mov    dword ptr [esi + 2ch], ebx
        mov    edi, ebx
        mov    ecx, dword ptr [esi + 0e7h]
loc_3_14BF:
        mov    byte ptr [edi + ecx*4 - 2], cl
        mov    byte ptr [edi + ecx*4 - 3], 2
        loop   loc_3_14BF
        pop    edi
        jmp    NEAR PTR loc_3_13EC
loc_3_14D0:
        mov    cx, 1
        jmp    SHORT loc_3_14DA
loc_3_14D6:
        mov    cx, 0
loc_3_14DA:
        mov    word ptr [edi + 68h], cx
        db  0BEh, 00Fh, 015h, 000h, 000h
loc_3_14E3:
        mov    edx, edi
        db  0CDh, 020h, 045h, 001h, 001h, 000h
        xor    eax, eax
        inc    eax
        mov    ebx, dword ptr [edx + 1ch]
        jecxz  loc_3_1508
        cmp    edi, ebx
        jne    SHORT loc_3_1508
        mov    ecx, 100h
loc_3_14FC:
        or     ecx, 8
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        mov    al, 1
        ret
loc_3_1508:
        mov    ecx, 102h
        jmp    SHORT loc_3_14FC
loc_3_150F:
        push   dword ptr [edx + 6ch]
        push   dword ptr [edx + 14h]
        lea    esi, [edx + 2ch]
        mov    ecx, 30h
        sub    esp, ecx
        mov    edi, esp
        rep movsb
        mov    edi, edx
        mov    byte ptr [edi + 0b0h], 1
        mov    esi, dword ptr [edi + 8]
        xchg   esi, edi
        mov    edx, dword ptr [esi + 98h]
        mov    ax, word ptr [edx + 3]
        mov    ch, byte ptr [edx + 1]
        mov    cl, byte ptr [edx + 5]
        inc    cl
        mov    ebx, dword ptr [edi]
        xchg   edi, ebx
        call   sub_3_1A7B
        mov    ecx, dword ptr [esi + 98h]
        add    eax, dword ptr [ebx + 28h]
        mov    dword ptr [esi + 74h], eax
        movzx  eax, word ptr [ecx + 7]
        mov    dword ptr [esi + 7ch], eax
        mul    dword ptr [edi + 0dbh]
        push   eax
        mov    edi, ebx
        mov    eax, dword ptr [ecx + 9]
        test   byte ptr [esi + 6ch], 10h
        mov    dword ptr [esi + 6ch], 0d00h
        je     SHORT loc_3_158B
        movzx  eax, word ptr [ecx + 0bh]
        movzx  ebx, word ptr [ecx + 9]
        mov    ecx, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [ecx + 14h]
        call   sub_3_20B9
loc_3_158B:
        mov    dword ptr [esi + 80h], eax
        pop    ecx
        dec    ecx
        mov    edx, eax
        and    edx, 0fffh
        add    ecx, edx
        shr    ecx, 0ch
        inc    ecx
        shr    eax, 0ch
        push   40000000h
        push   ecx
        push   edx
        push   0c0000000h
        push   ecx
        push   eax
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        pop    edx
        test   eax, eax
        je     NEAR PTR loc_3_164D
        add    edx, eax
        shr    eax, 0ch
        push   eax
        mov    dword ptr [esi + 80h], edx
        push   esi
        db  0FFh, 015h, 064h, 000h, 000h, 000h
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_1646
loc_3_15DE:
        mov    ebx, edi
        mov    edi, esi
        db  0BAh, 084h, 000h, 000h, 000h
        db  0FFh, 015h, 05Ch, 000h, 000h, 000h
        test   dword ptr [edi + 6ch], 40h
        jne    SHORT loc_3_15FF
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
loc_3_15FF:
        mov    dword ptr [edi + 0a0h], 0
        movzx  ecx, word ptr [edi + 6ah]
        cmp    ecx, 2
        jbe    SHORT loc_3_161E
        db  0CDh, 020h, 00Fh, 000h, 010h, 000h
        mov    dword ptr [edi + 0a0h], eax
loc_3_161E:
        mov    edx, edi
        lea    edi, [edx + 2ch]
        mov    ecx, 30h
        mov    ebx, ecx
        mov    esi, esp
        rep movsb
        add    esp, ebx
        mov    edi, edx
        pop    dword ptr [edi + 14h]
        pop    dword ptr [edi + 6ch]
        mov    word ptr [edi + 68h], 0dh
        push   ebx
        push   esi
        push   edi
        db  0E9h, 072h, 0EAh, 0FCh, 0FFh
loc_3_1646:
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        pop    eax
loc_3_164D:
        add    esp, 8
        or     dword ptr [esi + 6ch], 40h
        jmp    SHORT loc_3_15DE
loc_3_1656:
        db  0BEh, 060h, 016h, 000h, 000h
        jmp    NEAR PTR loc_3_14E3
loc_3_1660:
        push   dword ptr [edx + 6ch]
        push   dword ptr [edx + 14h]
        lea    esi, [edx + 2ch]
        mov    ecx, 30h
        sub    esp, ecx
        mov    edi, esp
        rep movsb
        mov    edi, edx
        mov    word ptr [edi + 68h], 2
        mov    dword ptr [edi + 6ch], 0d00h
        mov    byte ptr [edi + 0b0h], 0
        mov    esi, dword ptr [edi + 4]
        xchg   esi, edi
        mov    edx, dword ptr [esi + 98h]
        mov    ax, word ptr [edx + 3]
        mov    ch, byte ptr [edx + 1]
        mov    cl, 1
        call   sub_3_1A7B
        mov    edx, edi
        mov    edi, dword ptr [esi + 8]
        add    eax, dword ptr [edi + 28h]
        mov    dword ptr [esi + 74h], eax
        mov    eax, dword ptr [edx + 0e7h]
        mov    dword ptr [esi + 7ch], eax
        mov    ebx, edi
        mov    edi, esi
        db  0BAh, 084h, 000h, 000h, 000h
        db  0FFh, 015h, 05Ch, 000h, 000h, 000h
        jmp    NEAR PTR loc_3_15FF
loc_3_16CB:
        enter  4, 0
        push   esi
        mov    dword ptr [ebp - 4], edi
        mov    eax, dword ptr [edi + 8]
        mov    ebx, dword ptr [eax + 2ch]
        or     ebx, ebx
        je     NEAR PTR loc_3_1835
loc_3_16E1:
        mov    ecx, dword ptr [edi + 98h]
        test   byte ptr [ecx], 2
        jne    NEAR PTR loc_3_17C2
        push   ecx
        mov    edx, dword ptr [eax + 30h]
        or     edx, edx
        je     SHORT loc_3_1726
        mov    si, word ptr [edx + 3fh]
        and    si, 0fdf7h
        test   byte ptr [edx + 4], 80h
        jne    SHORT loc_3_1722
        and    si, 0fffch
        test   byte ptr [eax + 20h], 4
        jne    SHORT loc_3_1715
        or     si, 1
loc_3_1715:
        test   dword ptr [eax + 20h], 30000h
        je     SHORT loc_3_1722
        or     si, 2
loc_3_1722:
        mov    word ptr [edx + 3fh], si
loc_3_1726:
        mov    esi, dword ptr [edi + 4]
        test   byte ptr [ecx], 1
        je     NEAR PTR loc_3_185B
        or     byte ptr [eax + 45h], 1
        or     byte ptr [esi + 0b8h], 4
        mov    edi, dword ptr [eax + 30h]
        lea    edi, [edi + 6]
        lea    esi, [ecx + 7]
        cmp    byte ptr [eax + 40h], 0
        je     SHORT loc_3_1796
        movzx  ecx, word ptr [esi]
        mov    dword ptr [eax + 0dbh], ecx
        bsf    ecx, ecx
        mov    byte ptr [eax + 3ch], cl
        movzx  ecx, word ptr [esi + 8]
        or     ecx, ecx
        jne    SHORT loc_3_1766
        mov    ecx, dword ptr [esi + 15h]
loc_3_1766:
        mov    dword ptr [eax + 0d3h], ecx
        push   ebx
        mov    ebx, eax
        mov    eax, ecx
        movzx  ecx, word ptr [esi + 0dh]
        mov    dword ptr [ebx + 0e7h], ecx
        xor    edx, edx
        div    ecx
        movzx  ecx, word ptr [esi + 0fh]
        mov    dword ptr [ebx + 0dfh], ecx
        xor    edx, edx
        div    ecx
        mov    dword ptr [ebx + 0e3h], eax
        mov    eax, ebx
        pop    ebx
loc_3_1796:
        mov    ecx, 19h
        cmp    byte ptr [eax + 3dh], 0bh
        je     SHORT loc_3_17A7
        cmp    byte ptr [eax + 3dh], 0ch
        jne    SHORT loc_3_17BE
loc_3_17A7:
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 94h]
        cmp    ch, 48h
        jne    NEAR PTR loc_3_1874
        mov    ecx, 35h
loc_3_17BE:
        cld
        rep movsb
        pop    ecx
loc_3_17C2:
        mov    esi, dword ptr [eax + 30h]
        and    word ptr [esi + 3fh], 0fff7h
        test   byte ptr [ecx], 4
        je     SHORT loc_3_17D4
        or     word ptr [esi + 3fh], 8
loc_3_17D4:
        lea    esi, [ecx + 28h]
        mov    edi, ebx
        movzx  ecx, word ptr [ecx + 26h]
        cmp    ecx, 3fh
        ja     SHORT loc_3_181F
        or     ecx, ecx
        je     SHORT loc_3_1807
        cmp    byte ptr [eax + 40h], 0
        je     SHORT loc_3_17F2
        mov    dword ptr [eax + 0e7h], ecx
loc_3_17F2:
        cld
loc_3_17F3:
        add    edi, 2
        lodsw
        stosb
        lodsw
        mov    al, 3
        cmp    ah, 2
        jg     SHORT loc_3_1804
        mov    al, ah
loc_3_1804:
        stosb
        loop   loc_3_17F3
loc_3_1807:
        mov    edi, dword ptr [ebp - 4]
        mov    dword ptr [edi + 0a0h], 0
        mov    word ptr [edi + 6ah], 0
loc_3_181A:
        xor    al, al
        pop    esi
        leave
        ret
loc_3_181F:
        mov    eax, 0ch
        mov    edi, dword ptr [ebp - 4]
        mov    dword ptr [edi + 0a0h], eax
        mov    word ptr [edi + 6ah], 10h
        jmp    SHORT loc_3_181A
loc_3_1835:
        mov    edx, 0fch
        call   sub_3_1A35
        mov    dword ptr [eax + 2ch], ebx
        mov    ecx, 3fh
        mov    edx, 2000000h
loc_3_184C:
        mov    dword ptr [ebx + ecx*4 - 4], edx
        mov    byte ptr [ebx + ecx*4 - 2], cl
        loop   loc_3_184C
        jmp    NEAR PTR loc_3_16E1
loc_3_185B:
        and    byte ptr [eax + 45h], 0feh
        and    byte ptr [esi + 0b8h], 0fbh
        mov    edi, dword ptr [eax + 30h]
        lea    edi, [edi + 43h]
        lea    esi, [ecx + 7]
        jmp    NEAR PTR loc_3_1796
loc_3_1874:
        mov    edi, dword ptr [ebp - 4]
        mov    dword ptr [edi + 0a0h], 16h
        mov    word ptr [edi + 6ah], 16h
        jmp    SHORT loc_3_181A
loc_3_1889:
        mov    al, 2
        ret
        db  080h, 002h, 0D0h, 002h, 0A0h, 005h, 060h, 009h, 040h, 00Bh, 080h, 016h
loc_3_1898:
        add    eax, 7070706h
        pop    es
        pop    es
        or     byte ptr [edi], al
        pop    es
        inc    dword ptr [eax]
        add    byte ptr [edx], al
        add    dword ptr [edi], eax
        db  009h
sub_3_18A9:
        push   edi
        movzx  eax, byte ptr [ebx + 40h]
        cmp    eax, 0bh
        jg     SHORT loc_3_18E5
        db  08Ah, 080h, 098h, 018h, 000h, 000h
        cmp    al, 0ffh
        jne    SHORT loc_3_18E1
        mov    eax, edx
        db  08Dh, 03Dh, 096h, 018h, 000h, 000h
        mov    ecx, 6
        std
        repne scasw
        jne    SHORT loc_3_18E5
        cmp    ecx, 6
        jge    SHORT loc_3_18E5
        db  00Fh, 0B6h, 081h, 0A3h, 018h, 000h, 000h
loc_3_18E1:
        clc
loc_3_18E2:
        cld
        pop    edi
        ret
loc_3_18E5:
        mov    eax, 7
        stc
        jmp    SHORT loc_3_18E2
loc_3_18ED:
        enter  4, 0
        push   esi
        mov    dword ptr [ebp - 4], edi
        mov    ebx, dword ptr [edi + 8]
        mov    edx, dword ptr [ebx + 54h]
        call   sub_3_18A9
        mov    ebx, dword ptr [edi + 8]
        mov    edx, dword ptr [edi + 98h]
        mov    byte ptr [edx + 1], al
        xor    ecx, ecx
        mov    eax, dword ptr [ebx + 20h]
        test   eax, 30000h
        je     SHORT loc_3_191C
        mov    cx, 2
loc_3_191C:
        test   eax, 4
        jne    SHORT loc_3_1926
        or     cl, 1
loc_3_1926:
        test   byte ptr [edx], 1
        je     SHORT loc_3_1937
        test   byte ptr [ebx + 24h], 4
        je     SHORT loc_3_1937
        or     ecx, 800h
loc_3_1937:
        mov    word ptr [edx + 2], cx
        mov    ecx, dword ptr [ebx + 30h]
        mov    ax, word ptr [ecx + 41h]
        mov    word ptr [edx + 4], ax
        xor    al, al
        mov    byte ptr [edx + 6], al
        mov    esi, dword ptr [ebx + 30h]
        lea    ecx, [esi + 6]
        test   byte ptr [edx], 1
        jne    SHORT loc_3_1959
        lea    ecx, [esi + 43h]
loc_3_1959:
        mov    esi, ecx
        lea    edi, [edx + 7]
        mov    ecx, dword ptr [ebp - 4]
        cmp    byte ptr [ecx + 95h], 48h
        mov    ecx, 35h
        je     SHORT loc_3_197B
        mov    ecx, 19h
        cmp    word ptr [esi + 0bh], 0
        je     SHORT loc_3_1994
loc_3_197B:
        cld
        rep movsb
loc_3_197E:
        mov    edi, dword ptr [ebp - 4]
        mov    dword ptr [edi + 0a0h], 0
        mov    word ptr [edi + 6ah], 0
loc_3_1991:
        pop    esi
        leave
        ret
loc_3_1994:
        db  083h, 03Dh, 0BCh, 002h, 000h, 000h, 000h
        je     SHORT loc_3_19C5
        mov    ecx, 19h
        cld
        push   edi
        rep movsb
        pop    edi
        mov    word ptr [edi + 6], 200h
        mov    esi, dword ptr [ebx + 30h]
        mov    ecx, dword ptr [esi + 1fh]
        cmp    ecx, 0ffffh
        jbe    SHORT loc_3_19BF
        mov    cx, 0ffffh
loc_3_19BF:
        mov    word ptr [edi + 0bh], cx
        jmp    SHORT loc_3_197E
loc_3_19C5:
        mov    edi, dword ptr [ebp - 4]
        mov    dword ptr [edi + 0a0h], 16h
        mov    word ptr [edi + 6ah], 16h
        jmp    SHORT loc_3_1991
        db  000h, 000h
sub_3_19DC:
        push   esi
        lea    esi, [ebx + 0bh]
        mov    al, byte ptr [esi + 0ah]
        and    al, 0f0h
        cmp    al, 0f0h
        je     SHORT loc_3_19EB
        jmp    SHORT loc_3_1A2D
loc_3_19EB:
        mov    al, byte ptr [esi + 2]
        or     al, al
        je     SHORT loc_3_1A00
        shr    al, 1
loc_3_19F4:
        jb     SHORT loc_3_19FA
        shr    al, 1
        jmp    SHORT loc_3_19F4
loc_3_19FA:
        or     al, al
        je     SHORT loc_3_1A00
        jmp    SHORT loc_3_1A2D
loc_3_1A00:
        xor    dx, dx
        movzx  eax, word ptr [esi + 8]
        or     eax, eax
        jne    SHORT loc_3_1A16
        mov    eax, dword ptr [esi + 15h]
        or     eax, eax
        jne    SHORT loc_3_1A16
        jmp    SHORT loc_3_1A2D
        db  0EBh, 017h
loc_3_1A16:
        movzx  ecx, word ptr [esi + 0bh]
        or     ecx, ecx
        je     SHORT loc_3_1A31
        shl    ecx, 1
        sub    eax, ecx
        movzx  ecx, word ptr [esi + 6]
        shr    ecx, 4
        sub    eax, ecx
        jae    SHORT loc_3_1A31
loc_3_1A2D:
        mov    al, 1
        pop    esi
        ret
loc_3_1A31:
        pop    esi
        xor    eax, eax
        ret
sub_3_1A35:
        pushad
        sub    esp, 10h
        mov    eax, esp
        mov    word ptr [eax + 4], dx
        mov    word ptr [eax + 0ah], 9
        mov    word ptr [eax], 3
        mov    byte ptr [eax + 0ch], 8
        push   eax
        db  0FFh, 015h, 050h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 6]
        add    esp, 14h
        mov    dword ptr [esp + 10h], eax
        popad
        ret
sub_3_1A61:
        pushad
        sub    esp, 8
        mov    eax, esp
        mov    dword ptr [eax + 4], edi
        mov    word ptr [eax], 4
        push   eax
        db  0FFh, 015h, 050h, 000h, 000h, 000h
        add    esp, 0ch
        popad
        ret
sub_3_1A7B:
        push   ebx
        movzx  ebx, ax
        mov    eax, ebx
        mov    ebx, dword ptr [edi + 0dfh]
        mul    ebx
        movzx  ebx, ch
        add    eax, ebx
        mov    ebx, dword ptr [edi + 0e7h]
        mul    ebx
        movzx  ebx, cl
        dec    ebx
        add    eax, ebx
        pop    ebx
        ret
        db  000h, 002h, 002h, 001h, 000h, 002h, 070h, 000h, 0D0h, 002h, 0FDh, 002h, 000h, 009h, 000h, 002h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 090h, 000h, 002h, 001h, 001h, 000h, 002h
        db  0E0h, 000h, 060h, 009h, 0F9h, 007h, 000h, 00Fh, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 090h, 000h, 002h, 002h, 001h, 000h, 002h, 070h, 000h, 0A0h, 005h, 0F9h, 003h
        db  000h, 009h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 090h, 000h, 002h
        db  002h, 001h, 000h, 002h, 0F0h, 000h, 080h, 016h, 0F0h, 009h, 000h, 024h, 000h, 002h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 090h, 000h, 002h, 001h, 001h, 000h, 002h, 0E0h, 000h
        db  000h, 000h, 0F0h, 002h, 000h, 009h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h
loc_3_1B1F:
        sahf
        sbb    al, byte ptr [eax]
        db  000h, 0B8h, 01Ah, 000h, 000h, 0D2h
        sbb    al, byte ptr [eax]
        db  000h, 0ECh
        sbb    al, byte ptr [eax]
        db  000h
loc_3_1B2F:
        add    al, byte ptr [eax]
loc_3_1B31:
        add    eax, dword ptr [eax]
loc_3_1B33:
        add    byte ptr [edx], al
sub_3_1B35:
        mov    eax, edi
        mov    edx, esi
        db  08Dh, 035h, 006h, 01Bh, 000h, 000h
        mov    edi, dword ptr [edi + 30h]
        mov    ecx, 19h
        rep movsb
        mov    edi, eax
        mov    esi, edx
        mov    ecx, dword ptr [edi + 30h]
        mov    eax, dword ptr [edi + 60h]
        mov    word ptr [ecx + 0fh], ax
        mov    eax, dword ptr [edi + 68h]
        mov    word ptr [ecx + 0dh], ax
        mov    eax, dword ptr [edi + 54h]
        cmp    eax, 0ffffh
        ja     SHORT loc_3_1B6E
        mov    word ptr [edi + 38h], ax
        jmp    SHORT loc_3_1B71
loc_3_1B6E:
        mov    dword ptr [edi + 45h], eax
loc_3_1B71:
        mov    edx, 0
        cmp    dword ptr [edi + 64h], 28h
        jne    SHORT loc_3_1BC1
        cmp    dword ptr [edi + 68h], 9
        jbe    SHORT loc_3_1BE1
loc_3_1B82:
        mov    eax, dword ptr [edi + 54h]
        dec    eax
        mov    dl, 1
loc_3_1B88:
        cmp    eax, 0ff6h
        jb     SHORT loc_3_1B95
        shr    eax, 1
        shl    dl, 1
        jmp    SHORT loc_3_1B88
loc_3_1B95:
        cmp    dl, 1
        je     SHORT loc_3_1BA0
        mov    word ptr [ecx + 6], 0f0h
loc_3_1BA0:
        mov    byte ptr [ecx + 2], dl
        db  066h, 0F7h, 025h, 031h, 01Bh, 000h, 000h
        db  066h, 0F7h, 035h, 02Fh, 01Bh, 000h, 000h
        xor    edx, edx
        db  066h, 0F7h, 035h, 033h, 01Bh, 000h, 000h
        inc    eax
        mov    word ptr [ecx + 0bh], ax
        jmp    SHORT loc_3_1BF7
loc_3_1BC1:
        cmp    dword ptr [edi + 64h], 50h
        jne    SHORT loc_3_1B82
        mov    dl, 3
        cmp    dword ptr [edi + 68h], 24h
        je     SHORT loc_3_1BE1
        cmp    dword ptr [edi + 68h], 0fh
        je     SHORT loc_3_1BDF
        cmp    dword ptr [edi + 68h], 9
        jne    SHORT loc_3_1B82
        mov    dl, 2
        jmp    SHORT loc_3_1BE1
loc_3_1BDF:
        mov    dl, 1
loc_3_1BE1:
        push   edi
        push   esi
        db  0BEh, 01Fh, 01Bh, 000h, 000h
        mov    esi, dword ptr [esi + edx*4]
        mov    edi, dword ptr [edi + 30h]
        mov    ecx, 19h
        rep movsb
        pop    esi
        pop    edi
loc_3_1BF7:
        ret
loc_3_1BF8:
        test   al, 7fh
        add    ecx, dword ptr [eax]
        db  0FFh, 0FFh, 002h, 004h
loc_3_1C00:
        add    eax, dword ptr [eax]
        add    al, byte ptr [edi + eax]
        add    byte ptr [ebx], al
        or     byte ptr [edi], cl
        add    byte ptr [eax + edx], al
        pop    ds
        add    byte ptr [6003f20h], al
        inc    eax
        jg     SHORT loc_3_1C16
loc_3_1C16:
        pop    es
        cmp    bh, 0ffh
        pop    es
        db  080h
loc_3_1C1C:
        or     byte ptr [eax], al
        add    byte ptr [ecx], al
        and    byte ptr [eax], al
        add    al, byte ptr [eax + eax*8]
        add    byte ptr [ebx], al
        or     byte ptr [eax], al
        add    al, byte ptr [eax + edx]
        add    byte ptr [eax + 6ffff20h], al
        inc    eax
sub_3_1C34:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [edi]
        mov    edi, dword ptr [edi + 30h]
        mov    esi, dword ptr [eax + 0e7h]
        mov    word ptr [edi + 50h], si
        mov    esi, dword ptr [eax + 0dfh]
        mov    word ptr [edi + 52h], si
        mov    word ptr [edi + 43h], 200h
        mov    byte ptr [edi + 45h], 1
        mov    word ptr [edi + 46h], 1
        mov    byte ptr [edi + 48h], 2
        mov    word ptr [edi + 49h], 0e0h
        mov    ecx, eax
        movzx  eax, word ptr [edi + 0eh]
        or     eax, eax
        jne    SHORT loc_3_1CBA
        mov    eax, dword ptr [edi + 1bh]
        or     eax, eax
        jne    SHORT loc_3_1C88
        mov    eax, dword ptr [ecx + 0d3h]
        cmp    eax, 0ffffh
        jbe    SHORT loc_3_1CBA
loc_3_1C88:
        mov    dword ptr [edi + 58h], eax
        mov    word ptr [edi + 4bh], 0
        db  08Dh, 035h, 01Ch, 01Ch, 000h, 000h
        push   eax
        cmp    eax, 400000h
        ja     SHORT loc_3_1CB5
        mov    ebx, dword ptr [esp + 4]
        cmp    byte ptr [ebx + 3dh], 0bh
        je     SHORT loc_3_1CB5
        cmp    byte ptr [ebx + 3dh], 0ch
        je     SHORT loc_3_1CB5
        db  08Dh, 035h, 000h, 01Ch, 000h, 000h
loc_3_1CB5:
        rol    eax, 10h
        jmp    SHORT loc_3_1CCC
loc_3_1CBA:
        push   eax
        mov    dword ptr [edi + 58h], 0
        mov    word ptr [edi + 4bh], ax
        db  08Dh, 035h, 0F8h, 01Bh, 000h, 000h
loc_3_1CCC:
        cmp    ax, word ptr [esi]
        jbe    SHORT loc_3_1CD6
        add    esi, 4
        jmp    SHORT loc_3_1CCC
loc_3_1CD6:
        pop    eax
        mov    ebx, eax
        rol    ebx, 10h
        mov    cx, word ptr [esi + 2]
        mov    esi, dword ptr [esp]
        cmp    eax, 400000h
        ja     NEAR PTR loc_3_1DD6
        cmp    byte ptr [esi + 3dh], 0bh
        je     NEAR PTR loc_3_1DD6
        cmp    byte ptr [esi + 3dh], 0ch
        je     NEAR PTR loc_3_1DD6
        mov    esi, dword ptr [esi]
        mov    dx, 200h
        mov    word ptr [edi + 49h], dx
        mov    byte ptr [edi + 45h], ch
        mov    byte ptr [edi + 4dh], 0f8h
        push   eax
        mov    eax, dword ptr [esi + 0dbh]
        mov    word ptr [edi + 43h], ax
        cmp    eax, 200h
        pop    eax
        jne    SHORT loc_3_1D64
        mov    word ptr [edi + 43h], 200h
        or     bx, bx
        jne    SHORT loc_3_1D6B
        cmp    ax, 7fa8h
        ja     SHORT loc_3_1D6B
        mov    bl, ch
        dec    bx
        add    bx, ax
        shr    bx, cl
        inc    bx
        and    bl, 0feh
        add    bx, 2
        mov    si, bx
        shr    bx, 1
        add    bx, si
        add    bx, 1ffh
        shr    bh, 1
        xor    bl, bl
        xchg   bl, bh
        mov    word ptr [edi + 4eh], bx
        jmp    SHORT loc_3_1DD2
loc_3_1D64:
        call   sub_3_1E3A
        jmp    SHORT loc_3_1DD2
loc_3_1D6B:
        shr    dx, 4
        sub    ax, dx
        sbb    bx, 0
        sub    ax, 1
        sbb    bx, 0
        xchg   bx, dx
        push   eax
        push   edx
        mov    bl, 2
        mov    bh, byte ptr [edi + 45h]
        add    ax, bx
        adc    dx, 0
        sub    ax, 1
        sbb    dx, 0
        div    bx
        mov    word ptr [edi + 4eh], ax
        pop    edx
        pop    ebx
        shl    ax, 1
        xchg   bx, ax
        sub    ax, bx
        sbb    dx, 0
        xor    bh, bh
        mov    bl, ch
        shl    bx, 1
        add    ax, bx
        adc    dx, 0
        shl    bx, 7
        div    bx
        or     dx, dx
        je     SHORT loc_3_1DC8
        inc    ax
loc_3_1DC8:
        cmp    ax, word ptr [edi + 4eh]
        jbe    SHORT loc_3_1DD2
        mov    word ptr [edi + 4eh], ax
loc_3_1DD2:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_1DD6:
        mov    esi, dword ptr [esi]
        push   eax
        push   ecx
        sub    eax, 20h
        mov    ebx, dword ptr [esi + 0dbh]
        mov    edx, ebx
        shl    edx, cl
        add    edx, 8
        mov    ecx, edx
        mul    ebx
        div    ecx
        shl    eax, 2
        lea    eax, [ebx + eax - 1]
        xor    edx, edx
        mov    ebx, dword ptr [esi + 0dbh]
        div    ebx
        pop    ecx
        pop    edx
        and    word ptr [edi + 49h], 0
        and    word ptr [edi + 4eh], 0
        mov    word ptr [edi + 46h], 20h
        mov    byte ptr [edi + 48h], 2
        mov    dword ptr [edi + 64h], 2
        mov    word ptr [edi + 68h], 1
        mov    word ptr [edi + 6ah], 6
        mov    byte ptr [edi + 45h], ch
        mov    byte ptr [edi + 4dh], 0f8h
        mov    word ptr [edi + 43h], bx
        mov    dword ptr [edi + 5ch], eax
        jmp    SHORT loc_3_1DD2
sub_3_1E3A:
        or     bx, bx
        jne    SHORT loc_3_1E6F
        cmp    ax, 7fa8h
        ja     SHORT loc_3_1E6F
        movzx  ebx, ch
        dec    ebx
        movzx  eax, ax
        add    ebx, eax
        shr    ebx, cl
        inc    ebx
        and    bl, 0feh
        add    ebx, 2
        mov    eax, ebx
        shr    ebx, 1
        add    ebx, eax
        movzx  eax, word ptr [edi + 43h]
        dec    eax
        xchg   ebx, eax
        add    eax, ebx
        inc    ebx
        xor    edx, edx
        div    ebx
        mov    word ptr [edi + 4eh], ax
        ret
loc_3_1E6F:
        rol    eax, 10h
        mov    ax, bx
        rol    eax, 10h
        push   ecx
        movzx  ecx, word ptr [edi + 43h]
        shr    ecx, 5
        push   eax
        movzx  eax, dx
        xor    edx, edx
        div    ecx
        mov    ecx, eax
        pop    eax
        sub    eax, ecx
        sub    eax, 2
        push   eax
        movzx  ecx, word ptr [edi + 43h]
        shr    ecx, 1
        movzx  eax, byte ptr [edi + 45h]
        mul    ecx
        add    eax, 2
        mov    ebx, eax
        pop    eax
        push   eax
        xor    edx, edx
        div    ebx
        inc    eax
        mov    word ptr [edi + 4eh], ax
        pop    edx
        dec    edx
        shl    eax, 1
        sub    edx, eax
        pop    ecx
        movzx  ebx, ch
        shl    ebx, 1
        add    edx, ebx
        movzx  eax, word ptr [edi + 43h]
        shr    eax, 2
        mov    ecx, edx
        xor    edx, edx
        mul    ebx
        xchg   ecx, eax
        div    ecx
        or     edx, edx
        je     SHORT loc_3_1ED0
        inc    eax
loc_3_1ED0:
        cmp    ax, word ptr [edi + 43h]
        jbe    SHORT loc_3_1EDA
        mov    word ptr [edi + 43h], ax
loc_3_1EDA:
        ret
sub_3_1EDB:
        push   edi
        push   esi
        push   edx
        push   ebx
        push   ecx
        mov    ecx, dword ptr [esi + 4]
        add    ecx, 64h
        sub    esp, 10h
        mov    edi, esp
        mov    dword ptr [edi + 6], ecx
        add    ecx, 68h
        mov    word ptr [edi + 4], cx
        mov    byte ptr [edi + 0eh], 0
        mov    word ptr [edi], 2
        push   edi
        db  0FFh, 015h, 050h, 000h, 000h, 000h
        mov    edi, dword ptr [edi + 0ah]
        add    esp, 14h
        or     edi, edi
        je     NEAR PTR loc_3_1FA7
        add    edi, dword ptr [esi + 4]
        mov    word ptr [edi + 68h], dx
        xor    edx, edx
        mov    dword ptr [edi + 78h], edx
        mov    dword ptr [edi + 0a8h], edx
        mov    dword ptr [edi + 64h], 0
        mov    dword ptr [edi + 6ch], 0d01h
        or     ebx, ebx
        je     SHORT loc_3_1F38
        inc    edx
loc_3_1F38:
        mov    dword ptr [edi + 7ch], edx
        mov    dword ptr [edi + 74h], eax
        mov    dword ptr [edi + 0ach], edi
        add    dword ptr [edi + 0ach], 0bch
        mov    dword ptr [edi + 80h], ebx
        or     ebx, ebx
        je     SHORT loc_3_1F6D
        mov    dword ptr [edi + 8], esi
        push   edi
        db  0FFh, 015h, 064h, 000h, 000h, 000h
        add    esp, 4
        or     eax, eax
        je     SHORT loc_3_1F6D
        or     dword ptr [edi + 6ch], 40h
loc_3_1F6D:
        mov    dl, byte ptr [esi + 15h]
        mov    byte ptr [edi + 0b1h], dl
        mov    ebx, esi
        db  0BAh, 084h, 000h, 000h, 000h
        db  0FFh, 015h, 05Ch, 000h, 000h, 000h
        movzx  eax, word ptr [edi + 6ah]
        sub    edi, dword ptr [ebx + 4]
        sub    esp, 8
        mov    esi, esp
        mov    dword ptr [esi + 4], edi
        mov    word ptr [esi], 4
        push   esi
        db  0FFh, 015h, 050h, 000h, 000h, 000h
        add    esp, 0ch
loc_3_1FA1:
        pop    ecx
        pop    ebx
        pop    edx
        pop    esi
        pop    edi
        ret
loc_3_1FA7:
        mov    ax, 1dh
        jmp    SHORT loc_3_1FA1
sub_3_1FAD:
        push   ebx
        push   eax
        push   ecx
        db  0A1h, 0B3h, 002h, 000h, 000h
        mov    cl, byte ptr [esi + 14h]
loc_3_1FB8:
        cmp    byte ptr [eax + 5], cl
        je     SHORT loc_3_1FF6
        mov    eax, dword ptr [eax]
        cmp    ax, -1
        je     SHORT loc_3_1FE0
        push   ecx
        movzx  ecx, ax
        rol    eax, 10h
        movzx  eax, ax
        shl    eax, 4
        add    eax, ecx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        add    eax, dword ptr [ebx + 4]
        pop    ecx
        jmp    SHORT loc_3_1FB8
loc_3_1FE0:
        push   1
        push   96h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        or     byte ptr [esi + 17h], 2
loc_3_1FF6:
        cmp    dword ptr [esi + 30h], 0
        jne    SHORT loc_3_2003
        mov    dword ptr [esi + 30h], eax
loc_3_1FFF:
        pop    ecx
        pop    eax
        pop    ebx
        ret
loc_3_2003:
        mov    dword ptr [esi + 12dh], eax
        jmp    SHORT loc_3_1FFF
sub_3_200B:
        push   ebp
        sub    esp, 0ch
        mov    ebp, esp
        mov    word ptr [ebp], 7
        mov    dword ptr [ebp + 8], eax
        push   ebp
        db  0FFh, 015h, 050h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp + 4]
        movzx  eax, word ptr [ebp + 2]
        add    esp, 10h
        pop    ebp
        ret
sub_3_202D:
        sub    esp, 0ch
        mov    ebx, esp
        mov    dword ptr [ebx + 4], esi
        mov    byte ptr [ebx + 0ah], al
        mov    word ptr [ebx], 10h
        push   ebx
        db  0FFh, 015h, 050h, 000h, 000h, 000h
        mov    ax, word ptr [ebx + 2]
        or     ah, al
        mov    al, byte ptr [ebx + 8]
        add    esp, 10h
        ret
sub_3_2051:
        sub    esp, 0ch
        mov    ebx, esp
        mov    byte ptr [ebx + 8], al
        mov    byte ptr [ebx + 9], 0
        mov    dword ptr [ebx + 4], edi
        mov    word ptr [ebx], 6
        push   ebx
        db  0FFh, 015h, 050h, 000h, 000h, 000h
        add    esp, 10h
        ret
sub_3_2070:
        sub    esp, 8
        mov    eax, esp
        mov    word ptr [eax], 0ah
        mov    dword ptr [eax + 4], esi
        push   eax
        db  0FFh, 015h, 050h, 000h, 000h, 000h
        add    esp, 0ch
        ret
sub_3_2088:
        sub    esp, 0ch
        mov    edi, esp
        mov    word ptr [edi], 1
        mov    word ptr [edi + 4], 50h
        push   edi
        db  0FFh, 015h, 050h, 000h, 000h, 000h
        mov    edi, dword ptr [edi + 6]
        add    esp, 10h
        call   sub_3_10AB
        mov    dword ptr [edi + 0ch], 0
        mov    dword ptr [edi + 10h], 0
        ret
sub_3_20B9:
        push   ecx
        push   ebx
        shl    eax, 4
        movzx  ebx, bx
        add    eax, ebx
        add    eax, dword ptr [ecx + 4]
        pop    ebx
        pop    ecx
        ret
sub_3_20C9:
        push   edi
        push   esi
        push   ebx
        xor    edx, edx
        or     esi, esi
        je     SHORT loc_3_2104
        xor    ebx, ebx
loc_3_20D4:
        push   edx
        push   0
        push   14h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        pop    edx
        or     eax, eax
        je     SHORT loc_3_2109
        mov    edi, eax
        mov    ecx, 5
        rep movsd
        or     edx, edx
        jne    SHORT loc_3_20F8
        mov    edx, eax
        jmp    SHORT loc_3_20FB
loc_3_20F8:
        mov    dword ptr [ebx + 0ch], eax
loc_3_20FB:
        mov    ebx, eax
        mov    esi, dword ptr [esi - 8]
        or     esi, esi
        jne    SHORT loc_3_20D4
loc_3_2104:
        clc
loc_3_2105:
        pop    ebx
        pop    esi
        pop    edi
        ret
loc_3_2109:
        stc
        jmp    SHORT loc_3_2105
sub_3_210C:
        db  08Bh, 00Dh, 0B3h, 002h, 000h, 000h
loc_3_2112:
        cmp    byte ptr [ecx + 4], dl
        je     SHORT loc_3_2139
        mov    eax, dword ptr [ecx]
        cmp    ax, -1
        je     SHORT loc_3_2138
        movzx  ecx, ax
        rol    eax, 10h
        movzx  eax, ax
        shl    eax, 4
        add    ecx, eax
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        add    ecx, dword ptr [ebx + 4]
        jmp    SHORT loc_3_2112
loc_3_2138:
        stc
loc_3_2139:
        ret
_PTEXT ends
        end
