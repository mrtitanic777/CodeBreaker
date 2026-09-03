; vdd_pcode_14.asm -- VDD object 14 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_14_0000:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cld
        mov    edi, dword ptr [ebp + 8]
        mov    eax, 12345678h
        stosd
        mov    esi, 449h
        mov    ecx, 1eh
        rep movsb
        mov    esi, 484h
        mov    ecx, 7
        rep movsb
        mov    esi, 4a8h
        mov    ecx, 4
        rep movsb
        mov    esi, 410h
        mov    ecx, 2
        rep movsb
        mov    esi, 0
        movsd
        mov    esi, 40h
        movsd
        mov    esi, 7ch
        movsd
        mov    esi, 0bch
        movsd
        mov    esi, 10ch
        movsd
        mov    esi, 1b4h
        movsd
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_14_0069:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cld
        mov    esi, dword ptr [ebp + 8]
        lodsd
        cmp    eax, 12345678h
        jne    SHORT loc_14_00CF
        mov    edi, 449h
        mov    ecx, 1eh
        rep movsb
        mov    edi, 484h
        mov    ecx, 7
        rep movsb
        mov    edi, 4a8h
        mov    ecx, 4
        rep movsb
        mov    edi, 410h
        mov    ecx, 2
        rep movsb
        mov    edi, 0
        movsd
        mov    edi, 40h
        movsd
        mov    edi, 7ch
        movsd
        mov    edi, 0bch
        movsd
        mov    edi, 10ch
        movsd
        mov    edi, 1b4h
        movsd
loc_14_00CF:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_14_00E0:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 1
        jb     SHORT loc_14_00FA
        db  03Bh, 005h, 020h, 000h, 000h, 000h
        ja     SHORT loc_14_00FA
        db  08Bh, 004h, 085h, 038h, 006h, 000h, 000h
        jmp    SHORT loc_14_0108
loc_14_00FA:
        test   eax, 0ffff0000h
        je     SHORT loc_14_0106
        cmp    eax, -1
        jne    SHORT loc_14_0108
loc_14_0106:
        sub    eax, eax
loc_14_0108:
        ret
sub_14_0109:
        sub    esp, 4
        mov    dword ptr [esp], 0
        mov    ecx, dword ptr [esp + 8]
        cmp    ecx, 1
        jb     SHORT loc_14_0132
        db  039h, 00Dh, 020h, 000h, 000h, 000h
        jb     SHORT loc_14_0132
        db  08Bh, 004h, 08Dh, 0B0h, 010h, 000h, 000h
        mov    dword ptr [esp], eax
        jmp    SHORT loc_14_0146
loc_14_0132:
        or     ecx, ecx
        je     SHORT loc_14_0146
        lea    eax, [esp]
        push   0
        push   eax
        push   ecx
        call   sub_14_19A8
        add    esp, 0ch
loc_14_0146:
        mov    eax, dword ptr [esp]
        add    esp, 4
        ret
sub_14_014E:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        cmp    esi, -1
        jne    SHORT loc_14_015A
        sub    esi, esi
loc_14_015A:
        push   dword ptr [esp + 8]
        call   sub_14_0109
        add    esp, 4
        or     eax, eax
        je     SHORT loc_14_0170
        mov    dword ptr [eax + 22ch], esi
loc_14_0170:
        pop    esi
        ret
sub_14_0172:
        sub    esp, 8
        push   3
        lea    eax, [esp + 8]
        push   dword ptr [esp + 10h]
        push   eax
        call   sub_14_1968
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_14_018E
        jmp    SHORT loc_14_01F9
loc_14_018E:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [esp], eax
loc_14_0196:
        push   0
        push   0
        push   2
        lea    eax, [esp + 0ch]
        push   dword ptr [esp + 0ch]
        push   eax
        call   sub_14_1970
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_14_01F9
        mov    eax, dword ptr [esp]
        mov    cx, word ptr [eax + 4]
        cmp    cx, 3b0h
        jb     SHORT loc_14_01C7
        cmp    cx, 3bbh
        jbe    SHORT loc_14_01EB
loc_14_01C7:
        mov    ax, word ptr [eax + 6]
        cmp    ax, 3b0h
        jb     SHORT loc_14_01D7
        cmp    ax, 3bbh
        jbe    SHORT loc_14_01F2
loc_14_01D7:
        cmp    cx, 3b0h
        jae    SHORT loc_14_0196
        cmp    ax, 3bbh
        jbe    SHORT loc_14_0196
        mov    eax, 1
        jmp    SHORT loc_14_01FB
loc_14_01EB:
        mov    eax, 1
        jmp    SHORT loc_14_01FB
loc_14_01F2:
        mov    eax, 1
        jmp    SHORT loc_14_01FB
loc_14_01F9:
        sub    eax, eax
loc_14_01FB:
        add    esp, 8
        ret
sub_14_01FF:
        sub    esp, 8
        push   ebx
        push   dword ptr [esp + 10h]
        call   sub_14_00E0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_14_0215
        jmp    SHORT loc_14_0287
loc_14_0215:
        push   2
        push   eax
        lea    eax, [esp + 10h]
        push   eax
        call   sub_14_1968
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_14_022B
        jmp    SHORT loc_14_0287
loc_14_022B:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esp + 4], eax
        mov    ebx, 1
loc_14_0238:
        push   0
        push   0
        push   ebx
        lea    eax, [esp + 10h]
        push   dword ptr [esp + 10h]
        push   eax
        call   sub_14_1970
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_14_0287
        mov    edx, dword ptr [esp + 4]
        test   byte ptr [edx + 0ch], bl
        je     SHORT loc_14_0238
        mov    eax, dword ptr [edx + 8]
        sub    eax, dword ptr [edx + 4]
        cmp    eax, 7ffffh
        jb     SHORT loc_14_0238
        mov    eax, dword ptr [edx + 4]
        mov    ecx, dword ptr [esp + 18h]
        or     ecx, ecx
        je     SHORT loc_14_027B
        mov    edx, dword ptr [edx + 8]
        sub    edx, eax
        inc    edx
        mov    dword ptr [ecx], edx
loc_14_027B:
        mov    ecx, dword ptr [esp + 14h]
        or     ecx, ecx
        je     SHORT loc_14_0289
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_14_0289
loc_14_0287:
        sub    eax, eax
loc_14_0289:
        pop    ebx
        add    esp, 8
        ret
loc_14_028E:
        push   0
        push   0
        push   dword ptr [esp + 0ch]
        call   sub_14_01FF
        add    esp, 0ch
        ret
sub_14_029F:
        sub    esp, 4
        push   0
        lea    eax, [esp + 4]
        push   4
        push   eax
        push   dword ptr [esp + 18h]
        push   0
        push   dword ptr [esp + 1ch]
        call   sub_14_19C8
        mov    eax, dword ptr [esp + 18h]
        add    esp, 18h
        add    esp, 4
        ret
sub_14_02C5:
        push   0
        lea    eax, [esp + 10h]
        push   4
        push   eax
        push   dword ptr [esp + 14h]
        push   1
        push   dword ptr [esp + 18h]
        call   sub_14_19C8
        add    esp, 18h
        ret
sub_14_02E1:
        sub    esp, 4
        push   0
        lea    eax, [esp + 6]
        push   2
        push   eax
        push   dword ptr [esp + 18h]
        push   0
        push   dword ptr [esp + 1ch]
        call   sub_14_19C8
        mov    ax, word ptr [esp + 1ah]
        add    esp, 18h
        add    esp, 4
        ret
sub_14_0308:
        push   0
        lea    eax, [esp + 10h]
        push   2
        push   eax
        push   dword ptr [esp + 14h]
        push   1
        push   dword ptr [esp + 18h]
        call   sub_14_19C8
        add    esp, 18h
        ret
sub_14_0324:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        push   4
        push   esi
        call   sub_14_029F
        add    esp, 8
        mov    edi, eax
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, eax
        not    ecx
        and    ecx, edi
        mov    edx, dword ptr [esp + 10h]
        and    edx, eax
        or     ecx, edx
        push   ecx
        push   4
        push   esi
        call   sub_14_02C5
        add    esp, 0ch
        mov    eax, edi
        pop    edi
        pop    esi
        ret
sub_14_035A:
        push   7
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        call   sub_14_0324
        add    esp, 0ch
        ret
sub_14_036D:
        sub    esp, 4
        push   ebx
        push   esi
        push   0
        lea    eax, [esp + 0ch]
        push   dword ptr [esp + 14h]
        push   eax
        call   sub_14_1938
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_14_0404
loc_14_0389:
        push   0ah
        push   dword ptr [esp + 0ch]
        call   sub_14_02E1
        add    esp, 8
        movzx  ebx, ax
        cmp    ebx, 1
        db  0A1h, 000h, 000h, 000h, 000h
        je     SHORT loc_14_03A9
        cmp    bh, 3
        jne    SHORT loc_14_03D9
loc_14_03A9:
        lea    esi, [eax*4]
        mov    eax, dword ptr [esp + 8]
        push   0
        db  089h, 086h, 004h, 000h, 000h, 000h
        push   dword ptr [esp + 0ch]
        call   sub_14_035A
        add    esp, 8
        db  089h, 086h, 054h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        inc    eax
        db  0A3h, 000h, 000h, 000h, 000h
loc_14_03D9:
        cmp    ebx, 604h
        jne    SHORT loc_14_03ED
        push   dword ptr [esp + 8]
        call   sub_14_036D
        add    esp, 4
loc_14_03ED:
        push   0
        lea    eax, [esp + 0ch]
        push   dword ptr [esp + 0ch]
        push   eax
        call   sub_14_1940
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_14_0389
loc_14_0404:
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_14_040A:
        push   esi
        push   edi
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_14_042D
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 035h, 0A0h, 011h, 000h, 000h
        call   sub_14_036D
        add    esp, 4
        jmp    SHORT loc_14_0459
loc_14_042D:
        sub    edi, edi
        db  039h, 03Dh, 000h, 000h, 000h, 000h
        jle    SHORT loc_14_0459
        sub    esi, esi
loc_14_0439:
        db  0FFh, 0B6h, 054h, 000h, 000h, 000h
        db  0FFh, 0B6h, 004h, 000h, 000h, 000h
        call   sub_14_035A
        add    esp, 8
        add    esi, 4
        inc    edi
        db  03Bh, 03Dh, 000h, 000h, 000h, 000h
        jl     SHORT loc_14_0439
loc_14_0459:
        pop    edi
        pop    esi
        ret
sub_14_045C:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        mov    esi, dword ptr [esp + 18h]
        push   ebx
        lea    eax, [esp + 14h]
        push   esi
        sub    edi, edi
        push   eax
        call   sub_14_1938
        add    esp, 0ch
        cmp    eax, ebx
        jne    SHORT loc_14_04DB
        mov    ebp, dword ptr [esp + 1ch]
loc_14_0482:
        push   0ah
        push   dword ptr [esp + 14h]
        call   sub_14_02E1
        add    esp, 8
        movzx  eax, ax
        cmp    ebp, dword ptr [esp + 10h]
        jne    SHORT loc_14_049E
        mov    edi, 1
loc_14_049E:
        cmp    eax, 604h
        jne    SHORT loc_14_04B4
        push   ebp
        push   dword ptr [esp + 14h]
        call   sub_14_045C
        add    esp, 8
        mov    edi, eax
loc_14_04B4:
        push   4
        push   dword ptr [esp + 14h]
        call   sub_14_029F
        add    esp, 8
        push   0
        or     ebx, eax
        lea    eax, [esp + 14h]
        push   dword ptr [esp + 14h]
        push   eax
        call   sub_14_1940
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_14_0482
loc_14_04DB:
        db  03Bh, 035h, 0A0h, 011h, 000h, 000h
        je     SHORT loc_14_0532
        push   4
        push   esi
        call   sub_14_029F
        add    esp, 8
        or     ebx, eax
        or     edi, edi
        je     SHORT loc_14_04FC
        or     ebx, 2
        or     ebx, 1
        jmp    SHORT loc_14_04FF
loc_14_04FC:
        or     ebx, 2
loc_14_04FF:
        push   ebx
        push   esi
        call   sub_14_035A
        add    esp, 8
        push   3eh
        push   esi
        call   sub_14_02E1
        add    esp, 8
        or     edi, edi
        je     SHORT loc_14_0522
        or     ax, 4
        or     ax, 8
        jmp    SHORT loc_14_0526
loc_14_0522:
        and    ax, 0fff7h
loc_14_0526:
        push   eax
        push   3eh
        push   esi
        call   sub_14_0308
        add    esp, 0ch
loc_14_0532:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  056h, 08Bh, 074h, 024h, 008h, 00Bh, 0F6h, 074h, 01Bh, 06Ah, 001h, 0E8h, 0BEh, 0FEh, 0FFh, 0FFh
        db  083h, 0C4h, 004h, 068h, 0FFh, 000h, 000h, 000h, 056h, 0E8h, 000h, 0FEh, 0FFh, 0FFh, 083h, 0C4h
        db  008h, 056h, 0EBh, 010h, 06Ah, 000h, 0E8h, 0A3h, 0FEh, 0FFh, 0FFh, 083h, 0C4h, 004h, 0FFh, 035h
        db  03Ch, 006h, 000h, 000h, 0FFh, 035h, 0A0h, 011h, 000h, 000h, 0E8h, 0E1h, 0FEh, 0FFh, 0FFh, 083h
        db  0C4h, 008h, 05Eh, 0C3h
sub_14_0580:
        sub    esp, 4
        mov    dword ptr [esp], 0
        push   0
        lea    eax, [esp + 4]
        push   4
        push   eax
        push   0
        push   3
        push   dword ptr [esp + 1ch]
        call   sub_14_19C8
        mov    eax, dword ptr [esp + 18h]
        add    esp, 18h
        add    esp, 4
        ret
sub_14_05AC:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 1ch]
        push   0
        mov    edi, dword ptr [esp + 1ch]
        push   ebx
        push   edi
        push   0
        mov    esi, dword ptr [esp + 24h]
        push   4
        push   esi
        call   sub_14_19C8
        add    esp, 18h
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_14_0623
        mov    eax, dword ptr [edi]
        and    eax, 0ffffh
        cmp    eax, 0aa55h
        je     SHORT loc_14_0623
        push   0
        push   esi
        call   sub_14_029F
        add    esp, 8
        cmp    eax, 41581002h
        jne    SHORT loc_14_0623
        push   0
        push   ebx
        push   0fffd0000h
        call   sub_14_1890
        add    esp, 0ch
        mov    ecx, dword ptr [eax]
        and    ecx, 0ffffh
        cmp    ecx, 0aa55h
        jne    SHORT loc_14_0623
        mov    esi, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
loc_14_0623:
        cmp    ebp, 1
        sbb    eax, eax
        neg    eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_14_062F:
        sub    esp, 8
        push   0
        push   8000h
        push   dword ptr [esp + 14h]
        call   sub_14_1890
        add    esp, 0ch
        mov    dword ptr [esp + 4], eax
        mov    ecx, dword ptr [esp + 4]
        in     al, 21h
        mov    dl, byte ptr [ecx + 2]
        in     al, 21h
        mov    dh, byte ptr [ecx + 3]
        shl    edx, 10h
        in     al, 21h
        mov    dl, byte ptr [ecx]
        in     al, 21h
        mov    dh, byte ptr [ecx + 1]
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        add    esp, 8
        ret
sub_14_066F:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   1
        call   sub_14_040A
        add    esp, 4
        mov    edx, 3d4h
        in     al, 21h
        in     al, dx
        mov    byte ptr [esp + 0fh], al
        push   0a0000h
        call   sub_14_062F
        add    esp, 4
        mov    edi, eax
        push   0b0000h
        call   sub_14_062F
        add    esp, 4
        mov    ebx, eax
        push   0b8000h
        call   sub_14_062F
        add    esp, 4
        mov    esi, eax
        push   0
        call   sub_14_040A
        add    esp, 4
        cmp    byte ptr [esp + 0fh], 0ffh
        jne    SHORT loc_14_06DD
        cmp    edi, -1
        jne    SHORT loc_14_06DD
        cmp    ebx, -1
        jne    SHORT loc_14_06DD
        cmp    esi, -1
        mov    eax, 1
        je     SHORT loc_14_06DF
loc_14_06DD:
        sub    eax, eax
loc_14_06DF:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_14_06E6:
        call   sub_14_18F4
        or     eax, eax
        jne    SHORT loc_14_06FF
        push   dword ptr [esp + 4]
        push   0
        call   sub_14_18FC
        call   sub_14_1908
loc_14_06FF:
        ret
sub_14_0700:
        push   esi
        push   edi
        sub    edi, edi
        db  08Bh, 00Dh, 020h, 000h, 000h, 000h
        cmp    ecx, edi
        jle    SHORT loc_14_0734
        db  0BEh, 0B4h, 010h, 000h, 000h
loc_14_0713:
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_14_072C
        push   0
        push   dword ptr [eax + 4]
        call   sub_14_035A
        add    esp, 8
        db  08Bh, 00Dh, 020h, 000h, 000h, 000h
loc_14_072C:
        add    esi, 4
        inc    edi
        cmp    ecx, edi
        jg     SHORT loc_14_0713
loc_14_0734:
        pop    edi
        pop    esi
        ret
sub_14_0737:
        push   esi
        push   edi
        sub    edi, edi
        db  08Bh, 00Dh, 020h, 000h, 000h, 000h
        cmp    ecx, edi
        jle    SHORT loc_14_076C
        db  0BEh, 0B4h, 010h, 000h, 000h
loc_14_074A:
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_14_0764
        push   dword ptr [eax + 0ch]
        push   dword ptr [eax + 4]
        call   sub_14_035A
        add    esp, 8
        db  08Bh, 00Dh, 020h, 000h, 000h, 000h
loc_14_0764:
        add    esi, 4
        inc    edi
        cmp    edi, ecx
        jl     SHORT loc_14_074A
loc_14_076C:
        pop    edi
        pop    esi
        ret
sub_14_076F:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    ebx, dword ptr [esi + 2c9h]
        or     ebx, ebx
        je     SHORT loc_14_079D
        mov    dx, 3ceh
        in     al, dx
        mov    cl, al
        mov    al, 6
        out    dx, al
        mov    dx, 3cfh
        in     al, dx
        and    al, 0f3h
        and    bl, 0ch
        or     al, bl
        out    dx, al
        mov    dx, 3ceh
        mov    al, cl
        out    dx, al
loc_14_079D:
        mov    eax, dword ptr [esi + 14h]
        and    eax, 0ffffh
        cmp    eax, 1013h
        je     SHORT loc_14_07B8
        mov    dx, 3cch
        in     al, dx
        or     al, 2
        mov    dx, 3c2h
        out    dx, al
loc_14_07B8:
        pop    esi
        pop    ebx
        ret
sub_14_07BB:
        push   esi
        mov    dx, 3ceh
        in     al, dx
        mov    cl, al
        mov    al, 6
        out    dx, al
        mov    dx, 3cfh
        in     al, dx
        mov    esi, dword ptr [esp + 8]
        movzx  eax, al
        mov    dword ptr [esi + 2c9h], eax
        and    al, 0fbh
        or     al, 8
        out    dx, al
        mov    dx, 3ceh
        mov    al, cl
        out    dx, al
        mov    eax, dword ptr [esi + 14h]
        and    eax, 0ffffh
        cmp    eax, 1013h
        je     SHORT loc_14_07FF
        mov    dx, 3cch
        in     al, dx
        and    al, 0fdh
        mov    dx, 3c2h
        out    dx, al
loc_14_07FF:
        pop    esi
        ret
sub_14_0801:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 03Dh, 0A4h, 011h, 000h, 000h
        mov    esi, dword ptr [esp + 14h]
        cmp    edi, esi
        jne    SHORT loc_14_0818
        jmp    NEAR PTR loc_14_0953
loc_14_0818:
        or     edi, edi
        je     SHORT loc_14_0883
        and    dword ptr [edi + 0ch], 0ffffffcfh
        db  03Bh, 03Dh, 0B4h, 010h, 000h, 000h
        je     SHORT loc_14_083B
        db  0E8h, 047h, 0F9h, 0F1h, 0FFh
        db  0FFh, 035h, 0A4h, 011h, 000h, 000h
        call   sub_14_07BB
        add    esp, 4
loc_14_083B:
        call   sub_14_0737
        db  08Bh, 01Dh, 020h, 000h, 000h, 000h
        or     ebx, ebx
        jle    SHORT loc_14_0881
        db  0BDh, 0B4h, 010h, 000h, 000h
        db  08Bh, 03Dh, 0A4h, 011h, 000h, 000h
loc_14_0855:
        mov    edx, dword ptr [ebp]
        or     edx, edx
        je     SHORT loc_14_087B
        mov    eax, dword ptr [edx + 22ch]
        or     eax, eax
        je     SHORT loc_14_087B
        cmp    edx, edi
        je     SHORT loc_14_087B
        mov    cl, byte ptr [eax]
        and    cl, 0efh
        mov    byte ptr [eax], cl
        mov    dl, byte ptr [edx + 230h]
        or     dl, cl
        mov    byte ptr [eax], dl
loc_14_087B:
        add    ebp, 4
        dec    ebx
        jne    SHORT loc_14_0855
loc_14_0881:
        sub    edi, edi
loc_14_0883:
        db  089h, 03Dh, 0A4h, 011h, 000h, 000h
        or     esi, esi
        je     SHORT loc_14_090C
        db  089h, 035h, 0A4h, 011h, 000h, 000h
        or     dword ptr [esi + 0ch], 30h
        call   sub_14_0700
        db  0A1h, 0A4h, 011h, 000h, 000h
        push   0ffh
        push   dword ptr [eax + 4]
        call   sub_14_035A
        add    esp, 8
        db  08Bh, 03Dh, 0A4h, 011h, 000h, 000h
        db  03Bh, 03Dh, 0B4h, 010h, 000h, 000h
        je     SHORT loc_14_08CA
        db  0E8h, 074h, 0F8h, 0F1h, 0FFh
        db  08Bh, 03Dh, 0A4h, 011h, 000h, 000h
loc_14_08CA:
        db  0A1h, 020h, 000h, 000h, 000h
        or     eax, eax
        jle    SHORT loc_14_090C
        db  0B9h, 0B4h, 010h, 000h, 000h
loc_14_08D8:
        mov    esi, dword ptr [ecx]
        or     esi, esi
        je     SHORT loc_14_0906
        mov    ebp, dword ptr [esi + 22ch]
        or     ebp, ebp
        je     SHORT loc_14_0906
        cmp    esi, edi
        je     SHORT loc_14_0906
        mov    dl, byte ptr [ebp]
        mov    byte ptr [esi + 230h], dl
        mov    bl, dl
        and    bl, 10h
        mov    byte ptr [esi + 230h], bl
        or     dl, 10h
        mov    byte ptr [ebp], dl
loc_14_0906:
        add    ecx, 4
        dec    eax
        jne    SHORT loc_14_08D8
loc_14_090C:
        db  089h, 03Dh, 0A4h, 011h, 000h, 000h
        or     edi, edi
        je     SHORT loc_14_093F
        push   dword ptr [edi + 4]
        db  0FFh, 035h, 0A0h, 011h, 000h, 000h
        call   sub_14_045C
        add    esp, 8
        db  0A1h, 0A4h, 011h, 000h, 000h
        db  03Bh, 005h, 0B4h, 010h, 000h, 000h
        je     SHORT loc_14_0953
        push   eax
        call   sub_14_076F
        add    esp, 4
        jmp    SHORT loc_14_0953
loc_14_093F:
        db  0FFh, 035h, 03Ch, 006h, 000h, 000h
        db  0FFh, 035h, 0A0h, 011h, 000h, 000h
        call   sub_14_045C
        add    esp, 8
loc_14_0953:
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_14_095D:
        push   esi
        sub    esi, esi
        mov    ecx, dword ptr [esp + 10h]
        cmp    ecx, -1
        je     NEAR PTR loc_14_09F1
        cmp    ecx, esi
        jne    SHORT loc_14_0979
        db  08Bh, 035h, 03Ch, 006h, 000h, 000h
        jmp    SHORT loc_14_09F1
loc_14_0979:
        cmp    byte ptr [ecx], 5ch
        jne    SHORT loc_14_09F1
        cmp    byte ptr [ecx + 1], 5ch
        jne    SHORT loc_14_09F1
        cmp    byte ptr [ecx + 2], 2eh
        jne    SHORT loc_14_09F1
        cmp    byte ptr [ecx + 3], 5ch
        jne    SHORT loc_14_09F1
        mov    al, byte ptr [ecx + 4]
        or     al, 20h
        cmp    al, 64h
        jne    SHORT loc_14_09F1
        mov    al, byte ptr [ecx + 5]
        or     al, 20h
        cmp    al, 69h
        jne    SHORT loc_14_09F1
        mov    al, byte ptr [ecx + 6]
        or     al, 20h
        cmp    al, 73h
        jne    SHORT loc_14_09F1
        mov    al, byte ptr [ecx + 7]
        or     al, 20h
        cmp    al, 70h
        jne    SHORT loc_14_09F1
        mov    al, byte ptr [ecx + 8]
        or     al, 20h
        cmp    al, 6ch
        jne    SHORT loc_14_09F1
        mov    al, byte ptr [ecx + 9]
        or     al, 20h
        cmp    al, 61h
        jne    SHORT loc_14_09F1
        mov    al, byte ptr [ecx + 0ah]
        or     al, 20h
        cmp    al, 79h
        jne    SHORT loc_14_09F1
        mov    al, byte ptr [ecx + 0bh]
        cmp    al, 30h
        jl     SHORT loc_14_09F1
        cmp    al, 39h
        jg     SHORT loc_14_09F1
        cmp    byte ptr [ecx + 0ch], 0
        jne    SHORT loc_14_09F1
        movsx  eax, al
        sub    eax, 30h
        push   eax
        call   sub_14_00E0
        add    esp, 4
        mov    esi, eax
loc_14_09F1:
        or     esi, esi
        je     SHORT loc_14_0A08
        push   esi
        call   sub_14_0109
        add    esp, 4
        or     eax, eax
        je     SHORT loc_14_0A08
        inc    dword ptr [eax + 231h]
loc_14_0A08:
        mov    eax, esi
        pop    esi
        ret
loc_14_0A0C:
        push   dword ptr [esp + 4]
        call   sub_14_0109
        add    esp, 4
        or     eax, eax
        je     SHORT loc_14_0A2D
        mov    ecx, dword ptr [eax + 231h]
        or     ecx, ecx
        jle    SHORT loc_14_0A2D
        dec    ecx
        mov    dword ptr [eax + 231h], ecx
loc_14_0A2D:
        sub    eax, eax
        ret
sub_14_0A30:
        sub    esp, 80h
        push   ebx
        push   esi
        push   edi
        push   dword ptr [esp + 90h]
        call   sub_14_00E0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_14_0A51
        jmp    NEAR PTR loc_14_0ADA
loc_14_0A51:
        mov    ebx, dword ptr [esp + 98h]
        or     ebx, ebx
        je     SHORT loc_14_0ADA
        cmp    ebx, -1
        je     SHORT loc_14_0ADA
        cmp    dword ptr [esp + 9ch], 4
        je     SHORT loc_14_0A6D
        jmp    SHORT loc_14_0ADA
loc_14_0A6D:
        sub    ecx, ecx
        db  08Bh, 035h, 020h, 000h, 000h, 000h
        cmp    esi, ecx
        jle    SHORT loc_14_0A8A
        db  0BAh, 03Ch, 006h, 000h, 000h
loc_14_0A7E:
        cmp    dword ptr [edx], eax
        je     SHORT loc_14_0A8A
        add    edx, 4
        inc    ecx
        cmp    ecx, esi
        jl     SHORT loc_14_0A7E
loc_14_0A8A:
        or     ecx, ecx
        jne    SHORT loc_14_0A9E
        db  0BEh, 068h, 001h, 000h, 000h
        lea    edi, [esp + 0ch]
        movsd
        movsd
        movsd
        movsd
        movsb
        jmp    SHORT loc_14_0AB1
loc_14_0A9E:
        push   ecx
        lea    eax, [esp + 10h]
        db  068h, 07Ch, 001h, 000h, 000h
        push   eax
        call   sub_14_18B4
        add    esp, 0ch
loc_14_0AB1:
        cmp    dword ptr [esp + 94h], 0
        lea    eax, [esp + 0ch]
        push   ebx
        push   eax
        push   80000005h
        je     SHORT loc_14_0AD0
        call   sub_14_1880
        add    esp, 0ch
        jmp    SHORT loc_14_0ADF
loc_14_0AD0:
        call   sub_14_1870
        add    esp, 0ch
        jmp    SHORT loc_14_0ADF
loc_14_0ADA:
        mov    eax, 57h
loc_14_0ADF:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 80h
        ret
sub_14_0AE9:
        sub    esp, 28h
        mov    eax, dword ptr [esp + 38h]
        or     eax, eax
        je     SHORT loc_14_0B0D
        push   eax
        push   dword ptr [esp + 38h]
        lea    eax, [esp + 8]
        db  068h, 094h, 001h, 000h, 000h
        push   eax
        call   sub_14_18B4
        add    esp, 10h
        jmp    SHORT loc_14_0B23
loc_14_0B0D:
        push   dword ptr [esp + 34h]
        lea    eax, [esp + 4]
        db  068h, 09Ch, 001h, 000h, 000h
        push   eax
        call   sub_14_18B4
        add    esp, 0ch
loc_14_0B23:
        push   eax
        lea    eax, [esp + 4]
        push   eax
        push   1
        push   0
        push   dword ptr [esp + 40h]
        push   dword ptr [esp + 40h]
        call   sub_14_1888
        add    esp, 40h
        ret
loc_14_0B3E:
        sub    esp, 4
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 1ch]
        cmp    eax, 4ah
        jae    SHORT loc_14_0B56
        mov    eax, 57h
        jmp    NEAR PTR loc_14_0C25
loc_14_0B56:
        mov    esi, dword ptr [esp + 18h]
        movzx  ecx, word ptr [esi]
        cmp    ecx, eax
        je     SHORT loc_14_0B6B
        mov    eax, 57h
        jmp    NEAR PTR loc_14_0C25
loc_14_0B6B:
        lea    eax, [esp + 8]
        push   4
        push   eax
        push   1
        push   dword ptr [esp + 1ch]
        call   sub_14_0A30
        add    esp, 10h
        mov    edi, eax
        or     edi, edi
        jne    NEAR PTR loc_14_0C23
        push   0
        movzx  eax, byte ptr [esi + 1fh]
        push   eax
        db  068h, 0A0h, 001h, 000h, 000h
        push   dword ptr [esp + 14h]
        call   sub_14_0AE9
        add    esp, 10h
        movzx  eax, word ptr [esi + 1ah]
        push   eax
        movzx  eax, word ptr [esi + 18h]
        push   eax
        db  068h, 0B0h, 001h, 000h, 000h
        push   dword ptr [esp + 14h]
        call   sub_14_0AE9
        add    esp, 10h
        push   0
        push   dword ptr [esi + 32h]
        db  068h, 0BCh, 001h, 000h, 000h
        push   dword ptr [esp + 14h]
        call   sub_14_0AE9
        add    esp, 10h
        push   0
        push   dword ptr [esi + 36h]
        db  068h, 0C8h, 001h, 000h, 000h
        push   dword ptr [esp + 14h]
        call   sub_14_0AE9
        add    esp, 10h
        push   dword ptr [esi + 42h]
        push   dword ptr [esi + 42h]
        db  068h, 0D8h, 001h, 000h, 000h
        push   dword ptr [esp + 14h]
        call   sub_14_0AE9
        add    esp, 10h
        push   dword ptr [esi + 3ah]
        push   dword ptr [esi + 3ah]
        db  068h, 0E4h, 001h, 000h, 000h
        push   dword ptr [esp + 14h]
        call   sub_14_0AE9
        add    esp, 10h
        push   dword ptr [esp + 8]
        call   sub_14_1878
        add    esp, 4
loc_14_0C23:
        mov    eax, edi
loc_14_0C25:
        pop    edi
        pop    esi
        add    esp, 4
        ret
sub_14_0C2B:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  083h, 03Dh, 024h, 000h, 000h, 000h, 001h
        jg     SHORT loc_14_0C43
        mov    ebp, dword ptr [esp + 18h]
        mov    eax, ebp
        jmp    NEAR PTR loc_14_0D74
loc_14_0C43:
        push   dword ptr [esp + 14h]
        call   sub_14_0109
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     NEAR PTR loc_14_0D6F
        cmp    dword ptr [edi], 42414611h
        jne    NEAR PTR loc_14_0D6F
        call   sub_14_18CC
        mov    esi, eax
        call   sub_14_18A8
        cmp    esi, eax
        je     SHORT loc_14_0C82
        db  039h, 03Dh, 0B4h, 010h, 000h, 000h
        je     SHORT loc_14_0C82
        jmp    NEAR PTR loc_14_0D6F
loc_14_0C82:
        test   byte ptr [edi + 8], 1
        mov    ebp, dword ptr [esp + 18h]
        jne    SHORT loc_14_0C93
        mov    eax, ebp
        jmp    NEAR PTR loc_14_0D74
loc_14_0C93:
        test   ebp, 0ffffff00h
        je     SHORT loc_14_0CA0
        jmp    NEAR PTR loc_14_0D6F
loc_14_0CA0:
        test   ebp, 10h
        je     SHORT loc_14_0CAB
        or     ebp, 1
loc_14_0CAB:
        test   ebp, 20h
        je     SHORT loc_14_0CB6
        or     ebp, 2
loc_14_0CB6:
        db  039h, 03Dh, 0B4h, 010h, 000h, 000h
        jne    SHORT loc_14_0CC1
        or     ebp, 3
loc_14_0CC1:
        mov    ebx, dword ptr [edi + 0ch]
        cmp    ebx, ebp
        jne    SHORT loc_14_0CCF
        mov    eax, ebp
        jmp    NEAR PTR loc_14_0D74
loc_14_0CCF:
        mov    eax, ebx
        xor    eax, ebp
        and    eax, 3
        xor    eax, ebx
        mov    dword ptr [edi + 0ch], eax
        mov    esi, ebp
        and    esi, 30h
        je     SHORT loc_14_0D1C
        db  0A1h, 0A4h, 011h, 000h, 000h
        or     eax, eax
        je     SHORT loc_14_0D07
        cmp    eax, edi
        je     SHORT loc_14_0D07
        db  083h, 03Dh, 0A8h, 010h, 000h, 000h, 000h
        je     SHORT loc_14_0CFA
        jmp    SHORT loc_14_0D6F
loc_14_0CFA:
        db  039h, 03Dh, 0B4h, 010h, 000h, 000h
        jne    SHORT loc_14_0D12
loc_14_0D02:
        db  0A3h, 0A8h, 010h, 000h, 000h
loc_14_0D07:
        push   edi
        call   sub_14_0801
        add    esp, 4
        jmp    SHORT loc_14_0D3C
loc_14_0D12:
        db  03Bh, 005h, 0B4h, 010h, 000h, 000h
        jne    SHORT loc_14_0D6F
        jmp    SHORT loc_14_0D02
loc_14_0D1C:
        db  039h, 03Dh, 0A4h, 011h, 000h, 000h
        jne    SHORT loc_14_0D3C
        db  0FFh, 035h, 0A8h, 010h, 000h, 000h
        call   sub_14_0801
        add    esp, 4
        db  0C7h, 005h, 0A8h, 010h, 000h, 000h, 000h, 000h, 000h, 000h
loc_14_0D3C:
        test   ebp, 80h
        je     SHORT loc_14_0D47
        push   edi
        jmp    SHORT loc_14_0D51
loc_14_0D47:
        db  039h, 03Dh, 0ACh, 010h, 000h, 000h
        jne    SHORT loc_14_0D59
        push   0
loc_14_0D51:
        db  0E8h, 05Ah, 0F4h, 0F1h, 0FFh
        add    esp, 4
loc_14_0D59:
        or     esi, esi
        jne    SHORT loc_14_0D6B
        push   dword ptr [edi + 0ch]
        push   dword ptr [edi + 4]
        call   sub_14_035A
        add    esp, 8
loc_14_0D6B:
        mov    eax, ebx
        jmp    SHORT loc_14_0D74
loc_14_0D6F:
        mov    eax, 0ffffffffh
loc_14_0D74:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_14_0D79:
        sub    esp, 80h
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 90h]
        shr    esi, 0ch
        lea    eax, [esp + 8]
        push   0
        push   eax
        mov    edi, dword ptr [esp + 94h]
        push   esi
        mov    eax, edi
        shr    eax, 0ch
        push   eax
        call   sub_14_1910
        add    esp, 10h
        sub    ecx, ecx
        cmp    esi, ecx
        jle    SHORT loc_14_0DCB
loc_14_0DAE:
        mov    eax, dword ptr [esp + ecx*4 + 8]
        test   al, 1
        je     SHORT loc_14_0DCB
        and    eax, 0fffff000h
        mov    edx, ecx
        shl    edx, 0ch
        sub    eax, edx
        cmp    eax, edi
        jne    SHORT loc_14_0DCB
        inc    ecx
        cmp    ecx, esi
        jl     SHORT loc_14_0DAE
loc_14_0DCB:
        sub    ecx, esi
        cmp    ecx, 1
        sbb    eax, eax
        neg    eax
        pop    edi
        pop    esi
        add    esp, 80h
        ret
sub_14_0DDD:
        sub    esp, 10h
        push   esi
        mov    dword ptr [esp + 0ch], 4
        mov    eax, dword ptr [esp + 18h]
        mov    esi, 4
loc_14_0DF2:
        push   0
        push   eax
        lea    eax, [esp + 0ch]
        push   eax
        call   sub_14_1930
        add    esp, 0ch
        lea    eax, [esp + 10h]
        push   0
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 14h]
        call   sub_14_19A0
        add    esp, 14h
        cmp    dword ptr [esp + 8], esi
        mov    eax, dword ptr [esp + 4]
        jne    SHORT loc_14_0E2B
        jmp    SHORT loc_14_0DF2
loc_14_0E2B:
        pop    esi
        add    esp, 10h
        ret
sub_14_0E30:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebp, ebp
        mov    ebx, 13h
        push   1
        push   2cdh
        call   sub_14_1898
        add    esp, 8
        mov    esi, eax
        cmp    esi, ebp
        jne    SHORT loc_14_0E61
        mov    ebx, 2
        mov    edi, dword ptr [esp + 18h]
        jmp    NEAR PTR loc_14_1054
loc_14_0E61:
        push   0
        mov    edi, dword ptr [esp + 1ch]
        push   esi
        push   edi
        call   sub_14_19B0
        add    esp, 0ch
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [esi + 10h], 0
        mov    dword ptr [esp + 10h], 4
        push   0
        lea    eax, [esi + 18h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esi + 10h]
        push   eax
        push   edi
        call   sub_14_19A0
        add    esp, 14h
        cmp    dword ptr [esi + 10h], 4
        jne    SHORT loc_14_0EDF
        db  083h, 03Dh, 0A0h, 011h, 000h, 000h, 000h
        jne    SHORT loc_14_0EB8
        push   edi
        call   sub_14_0DDD
        add    esp, 4
        db  0A3h, 0A0h, 011h, 000h, 000h
loc_14_0EB8:
        push   0
        push   edi
        call   sub_14_029F
        add    esp, 8
        mov    dword ptr [esi + 14h], eax
        push   0ah
        push   edi
        call   sub_14_02E1
        add    esp, 8
        movzx  ebp, ax
        push   4
        push   edi
        call   sub_14_02E1
        add    esp, 8
loc_14_0EDF:
        lea    eax, [esi + 20h]
        push   eax
        lea    eax, [esi + 1ch]
        push   eax
        push   edi
        call   sub_14_01FF
        add    esp, 0ch
        push   edi
        call   sub_14_0172
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_14_0F90
        db  083h, 03Dh, 03Ch, 006h, 000h, 000h, 000h
        jne    NEAR PTR loc_14_1054
        db  0F6h, 005h, 01Bh, 00Bh, 000h, 000h, 040h
        je     SHORT loc_14_0F21
        mov    ebx, 22h
        jmp    NEAR PTR loc_14_1054
loc_14_0F21:
        push   edi
        db  0E8h, 056h, 0FCh, 0F1h, 0FFh
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    NEAR PTR loc_14_1054
        mov    dword ptr [esi + 0ch], 83h
        mov    eax, dword ptr [esi + 8]
        or     eax, 1
        mov    dword ptr [esi + 8], eax
        or     eax, 2
        mov    dword ptr [esi + 8], eax
        db  089h, 03Dh, 03Ch, 006h, 000h, 000h
        db  089h, 035h, 0B4h, 010h, 000h, 000h
        db  089h, 035h, 0ACh, 010h, 000h, 000h
        db  0C7h, 005h, 0A4h, 011h, 000h, 000h, 000h, 000h, 000h, 000h
        cmp    dword ptr [esi + 10h], 4
        jne    SHORT loc_14_0F6C
loc_14_0F6C:
        db  0FFh, 005h, 020h, 000h, 000h, 000h
        test   byte ptr [esi + 8], 1
        je     SHORT loc_14_0F83
        db  0A1h, 024h, 000h, 000h, 000h
        inc    eax
        db  0A3h, 024h, 000h, 000h, 000h
loc_14_0F83:
        mov    dword ptr [esi], 42414611h
        sub    eax, eax
        jmp    NEAR PTR loc_14_1095
loc_14_0F90:
        db  083h, 03Dh, 03Ch, 006h, 000h, 000h, 000h
        jne    SHORT loc_14_0F9E
        jmp    NEAR PTR loc_14_104F
loc_14_0F9E:
        db  083h, 03Dh, 020h, 000h, 000h, 000h, 008h
        jl     SHORT loc_14_0FAC
        jmp    NEAR PTR loc_14_104F
loc_14_0FAC:
        db  0A1h, 020h, 000h, 000h, 000h
        shl    eax, 2
        db  089h, 0B8h, 03Ch, 006h, 000h, 000h
        db  089h, 0B0h, 0B4h, 010h, 000h, 000h
        cmp    dword ptr [esi + 10h], 4
        jne    NEAR PTR loc_14_1090
        cmp    ebp, 300h
        je     SHORT loc_14_0FF5
        cmp    ebp, 301h
        je     SHORT loc_14_0FF5
        cmp    ebp, 1
        je     SHORT loc_14_0FF5
        push   4
        push   edi
        call   sub_14_02E1
        add    esp, 8
        and    eax, 3
        mov    dword ptr [esi + 0ch], eax
        jmp    NEAR PTR loc_14_0F6C
loc_14_0FF5:
        or     dword ptr [esi + 8], 1
        mov    ebx, dword ptr [esi + 4]
        push   0
        push   ebx
        call   sub_14_035A
        add    esp, 8
        push   20h
        push   0
        db  0FFh, 035h, 03Ch, 006h, 000h, 000h
        call   sub_14_0324
        add    esp, 0ch
        push   20h
        push   0
        push   ebx
        call   sub_14_0324
        add    esp, 0ch
        call   sub_14_066F
        or     eax, eax
        jne    SHORT loc_14_103E
        db  068h, 078h, 002h, 000h, 000h
        call   sub_14_06E6
        add    esp, 4
        jmp    SHORT loc_14_104F
loc_14_103E:
        push   esi
        db  0E8h, 0E3h, 0F2h, 0F1h, 0FFh
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_14_0F6C
loc_14_104F:
        mov    ebx, 13h
loc_14_1054:
        db  0A1h, 020h, 000h, 000h, 000h
        shl    eax, 2
        or     esi, esi
        db  0C7h, 080h, 0B4h, 010h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 03Ch, 006h, 000h, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_14_108C
        push   0
        push   esi
        call   sub_14_18A0
        add    esp, 8
        push   0
        push   0
        push   edi
        call   sub_14_19B0
        add    esp, 0ch
loc_14_108C:
        mov    eax, ebx
        jmp    SHORT loc_14_1095
loc_14_1090:
        mov    eax, 13h
loc_14_1095:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_14_109D:
        push   dword ptr [esp + 4]
        call   sub_14_0109
        add    esp, 4
        or     eax, eax
        je     SHORT loc_14_10CF
        test   byte ptr [eax + 8], 2
        je     SHORT loc_14_10B5
        jmp    SHORT loc_14_10CF
loc_14_10B5:
        cmp    dword ptr [eax + 231h], 0
        je     SHORT loc_14_10C0
        jmp    SHORT loc_14_10CF
loc_14_10C0:
        cmp    dword ptr [eax + 0ch], 0
        je     SHORT loc_14_10C8
        jmp    SHORT loc_14_10CF
loc_14_10C8:
        mov    eax, 1
        jmp    SHORT loc_14_10D1
loc_14_10CF:
        sub    eax, eax
loc_14_10D1:
        ret
sub_14_10D2:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   dword ptr [esp + 18h]
        call   sub_14_109D
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_14_10F3
        mov    eax, 13h
        jmp    NEAR PTR loc_14_11A3
loc_14_10F3:
        mov    dword ptr [esp + 10h], 0
        sub    ebx, ebx
        db  08Bh, 00Dh, 020h, 000h, 000h, 000h
        cmp    ecx, ebx
        jle    SHORT loc_14_117F
        sub    edi, edi
        sub    esi, esi
loc_14_110B:
        db  08Bh, 086h, 03Ch, 006h, 000h, 000h
        cmp    dword ptr [esp + 18h], eax
        jne    SHORT loc_14_115E
        db  08Bh, 0AEh, 0B4h, 010h, 000h, 000h
        test   byte ptr [ebp + 8], 1
        je     SHORT loc_14_1129
        db  0FFh, 00Dh, 024h, 000h, 000h, 000h
loc_14_1129:
        mov    eax, dword ptr [ebp + 28h]
        or     eax, eax
        je     SHORT loc_14_113B
        push   0
        push   eax
        call   sub_14_18C4
        add    esp, 8
loc_14_113B:
        push   0
        push   ebp
        call   sub_14_18A0
        add    esp, 8
        push   0
        push   0
        push   dword ptr [esp + 20h]
        call   sub_14_19B0
        add    esp, 0ch
        db  08Bh, 00Dh, 020h, 000h, 000h, 000h
        jmp    SHORT loc_14_1177
loc_14_115E:
        db  089h, 087h, 03Ch, 006h, 000h, 000h
        db  08Bh, 086h, 0B4h, 010h, 000h, 000h
        db  089h, 087h, 0B4h, 010h, 000h, 000h
        add    edi, 4
        inc    dword ptr [esp + 10h]
loc_14_1177:
        add    esi, 4
        inc    ebx
        cmp    ecx, ebx
        jg     SHORT loc_14_110B
loc_14_117F:
        mov    eax, dword ptr [esp + 10h]
        shl    eax, 2
        dec    ecx
        db  0C7h, 080h, 03Ch, 006h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 080h, 0B4h, 010h, 000h, 000h, 000h, 000h, 000h, 000h
        db  089h, 00Dh, 020h, 000h, 000h, 000h
        sub    eax, eax
loc_14_11A3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_14_11AB:
        sub    esp, 1a4h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 1b8h]
        push   esi
        call   sub_14_0109
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_14_11D5
        mov    eax, 1
        jmp    NEAR PTR loc_14_130B
loc_14_11D5:
        test   byte ptr [edi + 8], 1
        je     SHORT loc_14_1251
        mov    ebp, 13h
        push   0
        push   esi
        db  0E8h, 0F3h, 0EEh, 0F1h, 0FFh
        add    esp, 8
        or     eax, eax
        je     SHORT loc_14_1231
        push   0ffh
        push   esi
        call   sub_14_0C2B
        add    esp, 8
        mov    ebx, eax
        cmp    ebx, -1
        je     SHORT loc_14_1226
        lea    eax, [edi + 2b5h]
        push   eax
        lea    eax, [edi + 235h]
        push   eax
        db  0E8h, 0CCh, 008h, 0F2h, 0FFh
        add    esp, 8
        mov    ebp, eax
        push   ebx
        push   esi
        call   sub_14_0C2B
        add    esp, 8
loc_14_1226:
        push   0
        push   esi
        db  0E8h, 0D0h, 0EFh, 0F1h, 0FFh
        add    esp, 8
loc_14_1231:
        or     ebp, ebp
        jne    SHORT loc_14_123B
        or     dword ptr [edi + 8], 4
        jmp    SHORT loc_14_123F
loc_14_123B:
        and    dword ptr [edi + 8], 0fffffffbh
loc_14_123F:
        cmp    ebp, 20h
        jne    SHORT loc_14_1251
        push   30h
        push   0
        push   esi
        db  0E8h, 00Bh, 0EEh, 0F1h, 0FFh
        add    esp, 0ch
loc_14_1251:
        mov    eax, dword ptr [edi + 8]
        test   al, 4
        jne    SHORT loc_14_1288
        test   al, 2
        je     SHORT loc_14_1288
        lea    eax, [esp + 14h]
        push   0
        push   eax
        push   -1
        call   sub_14_19C0
        add    esp, 0ch
        push   dword ptr [esp + 14h]
        lea    eax, [esp + 0f0h]
        db  068h, 0C8h, 002h, 000h, 000h
        push   eax
        call   sub_14_18B4
        add    esp, 0ch
        jmp    SHORT loc_14_12C6
loc_14_1288:
        push   0
        lea    eax, [esp + 0f0h]
        push   0c8h
        push   eax
        push   esi
        call   sub_14_1948
        add    esp, 10h
        sub    edx, edx
        cmp    byte ptr [esp + 0ech], dl
        je     SHORT loc_14_12C6
        sub    ecx, ecx
loc_14_12AD:
        lea    eax, [esp + edx + 0ech]
        cmp    byte ptr [eax], 5ch
        jne    SHORT loc_14_12BC
        mov    byte ptr [eax], 5fh
loc_14_12BC:
        inc    edx
        cmp    byte ptr [esp + edx + 0ech], cl
        jne    SHORT loc_14_12AD
loc_14_12C6:
        test   byte ptr [edi + 8], 4
        je     SHORT loc_14_12D4
        add    edi, 2b5h
        jmp    SHORT loc_14_12D9
loc_14_12D4:
        db  0BFh, 0D0h, 002h, 000h, 000h
loc_14_12D9:
        lea    eax, [esp + 0ech]
        push   eax
        push   edi
        lea    eax, [esp + 74h]
        db  068h, 0E0h, 002h, 000h, 000h
        push   eax
        call   sub_14_18B4
        add    esp, 10h
        push   0
        lea    eax, [esp + 70h]
        push   esi
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_14_1950
        add    esp, 10h
        sub    eax, eax
loc_14_130B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1a4h
        ret
sub_14_1316:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        movzx  eax, word ptr [esi]
        mov    cl, byte ptr [esi + 4]
        mov    dl, cl
        and    edx, 0f0h
        and    ecx, 0fh
        add    edx, ecx
        shl    edx, 4
        movzx  ecx, byte ptr [esi + 2]
        add    edx, ecx
        movzx  ecx, byte ptr [esi + 3]
        lea    edi, [edx + ecx]
        mov    dl, byte ptr [esi + 7]
        mov    bl, dl
        and    ebx, 0f0h
        shl    ebx, 4
        movzx  ecx, byte ptr [esi + 5]
        add    ebx, ecx
        movzx  ecx, byte ptr [esi + 6]
        add    ebx, ecx
        and    edx, 0fh
        lea    ecx, [ebx + edx]
        or     edi, edi
        je     SHORT loc_14_1377
        or     ecx, ecx
        je     SHORT loc_14_1377
        imul   eax, eax, 2710h
        imul   ecx, edi
        sub    edx, edx
        div    ecx
        jmp    SHORT loc_14_1379
loc_14_1377:
        sub    eax, eax
loc_14_1379:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_14_137D:
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   0
        lea    eax, [esp + 2ch]
        push   dword ptr [esp + 34h]
        push   eax
        call   sub_14_1930
        add    esp, 0ch
        push   dword ptr [esp + 28h]
        call   sub_14_0109
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_14_13B0
        sub    eax, eax
        jmp    NEAR PTR loc_14_15C1
loc_14_13B0:
        test   byte ptr [edi + 8], 4
        jne    SHORT loc_14_13BD
        sub    eax, eax
        jmp    NEAR PTR loc_14_15C1
loc_14_13BD:
        mov    ebx, dword ptr [edi + 258h]
        and    ebx, 80ffffh
        mov    dword ptr [esp + 18h], 0
        mov    dword ptr [esp + 14h], 0
        sub    esi, esi
        mov    dword ptr [esp + 10h], 7fffffffh
        db  039h, 035h, 030h, 000h, 000h, 000h
        je     SHORT loc_14_1437
        db  0BDh, 030h, 000h, 000h, 000h
loc_14_13F0:
        test   dword ptr [ebp + 0ch], ebx
        je     SHORT loc_14_142E
        mov    eax, dword ptr [ebp]
        mov    ecx, dword ptr [ebp + 4]
        mov    edx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [esp + 34h]
        jne    SHORT loc_14_141A
        cmp    ecx, dword ptr [esp + 38h]
        jne    SHORT loc_14_141A
        cmp    esi, edx
        jge    SHORT loc_14_1410
        mov    esi, edx
loc_14_1410:
        cmp    dword ptr [esp + 10h], edx
        jle    SHORT loc_14_141A
        mov    dword ptr [esp + 10h], edx
loc_14_141A:
        cmp    eax, dword ptr [esp + 18h]
        jle    SHORT loc_14_1424
        mov    dword ptr [esp + 18h], eax
loc_14_1424:
        cmp    ecx, dword ptr [esp + 14h]
        jle    SHORT loc_14_142E
        mov    dword ptr [esp + 14h], ecx
loc_14_142E:
        add    ebp, 10h
        cmp    dword ptr [ebp], 0
        jne    SHORT loc_14_13F0
loc_14_1437:
        lea    ecx, [edi + 25bh]
        mov    dword ptr [esp + 24h], 8
loc_14_1445:
        movzx  ebx, word ptr [ecx]
        or     ebx, ebx
        je     NEAR PTR loc_14_14F5
        cmp    ebx, 101h
        je     NEAR PTR loc_14_14F5
        mov    eax, ebx
        and    eax, 0ffh
        lea    ebp, [eax*8 + 0f8h]
        mov    eax, ebx
        and    eax, 0c000h
        cmp    eax, 4000h
        jne    SHORT loc_14_1488
        mov    eax, ebp
        imul   eax, eax, 3
        cdq
        and    edx, 3
        add    eax, edx
        sar    eax, 2
        jmp    SHORT loc_14_14B9
loc_14_1488:
        cmp    eax, 8000h
        jne    SHORT loc_14_14A5
        lea    eax, [ebp*4]
        mov    dword ptr [esp + 1ch], 5
        cdq
        idiv   dword ptr [esp + 1ch]
        jmp    SHORT loc_14_14B9
loc_14_14A5:
        or     eax, eax
        mov    eax, ebp
        je     SHORT loc_14_14B9
        mov    eax, ebp
        imul   eax, eax, 9
        cdq
        and    edx, 0fh
        add    eax, edx
        sar    eax, 4
loc_14_14B9:
        and    ebx, 1f00h
        shr    ebx, 8
        add    ebx, 3ch
        cmp    ebp, dword ptr [esp + 34h]
        jne    SHORT loc_14_14E1
        cmp    eax, dword ptr [esp + 38h]
        jne    SHORT loc_14_14E1
        cmp    esi, ebx
        jge    SHORT loc_14_14D7
        mov    esi, ebx
loc_14_14D7:
        cmp    dword ptr [esp + 10h], ebx
        jle    SHORT loc_14_14E1
        mov    dword ptr [esp + 10h], ebx
loc_14_14E1:
        cmp    dword ptr [esp + 18h], ebp
        jge    SHORT loc_14_14EB
        mov    dword ptr [esp + 18h], ebp
loc_14_14EB:
        cmp    eax, dword ptr [esp + 14h]
        jle    SHORT loc_14_14F5
        mov    dword ptr [esp + 14h], eax
loc_14_14F5:
        add    ecx, 2
        dec    dword ptr [esp + 24h]
        jne    NEAR PTR loc_14_1445
        add    edi, 26bh
        mov    dword ptr [esp + 20h], 4
loc_14_1510:
        cmp    dword ptr [edi], 0
        je     SHORT loc_14_1573
        mov    cl, byte ptr [edi + 4]
        and    ecx, 0f0h
        shl    ecx, 4
        movzx  eax, byte ptr [edi + 2]
        lea    ebx, [ecx + eax]
        mov    al, byte ptr [edi + 7]
        and    eax, 0f0h
        shl    eax, 4
        movzx  ecx, byte ptr [edi + 5]
        lea    ebp, [eax + ecx]
        push   edi
        call   sub_14_1316
        add    esp, 4
        cmp    ebx, dword ptr [esp + 34h]
        jne    SHORT loc_14_155F
        cmp    ebp, dword ptr [esp + 38h]
        jne    SHORT loc_14_155F
        cmp    esi, eax
        jge    SHORT loc_14_1555
        mov    esi, eax
loc_14_1555:
        cmp    dword ptr [esp + 10h], eax
        jle    SHORT loc_14_155F
        mov    dword ptr [esp + 10h], eax
loc_14_155F:
        cmp    ebx, dword ptr [esp + 18h]
        jle    SHORT loc_14_1569
        mov    dword ptr [esp + 18h], ebx
loc_14_1569:
        cmp    ebp, dword ptr [esp + 14h]
        jle    SHORT loc_14_1573
        mov    dword ptr [esp + 14h], ebp
loc_14_1573:
        add    edi, 12h
        dec    dword ptr [esp + 20h]
        jne    SHORT loc_14_1510
        cmp    dword ptr [esp + 34h], 0
        jne    SHORT loc_14_15A0
        cmp    dword ptr [esp + 38h], 0
        jne    SHORT loc_14_15A0
        mov    ecx, dword ptr [esp + 3ch]
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 40h]
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_14_15BC
loc_14_15A0:
        or     esi, esi
        jne    SHORT loc_14_15AC
        mov    dword ptr [esp + 10h], 0
loc_14_15AC:
        mov    ecx, dword ptr [esp + 3ch]
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 40h]
        mov    dword ptr [ecx], esi
loc_14_15BC:
        mov    eax, 1
loc_14_15C1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
sub_14_15C9:
        sub    esp, 88h
        push   esi
        push   edi
        push   dword ptr [esp + 94h]
        call   sub_14_0109
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_14_15F0
        mov    eax, 1
        jmp    NEAR PTR loc_14_16E6
loc_14_15F0:
        test   byte ptr [esi + 8], 4
        je     NEAR PTR loc_14_16E4
        push   0
        push   80h
        lea    eax, [esi + 235h]
        push   eax
        push   3
        db  068h, 0F0h, 002h, 000h, 000h
        mov    edi, dword ptr [esp + 0ach]
        push   0
        push   edi
        call   sub_14_1998
        add    esp, 1ch
        lea    eax, [esi + 2b5h]
        push   eax
        lea    eax, [esp + 14h]
        db  068h, 0F8h, 002h, 000h, 000h
        push   eax
        call   sub_14_18B4
        add    esp, 0ch
        push   0
        inc    eax
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   1
        db  068h, 00Ch, 003h, 000h, 000h
        push   0
        push   edi
        call   sub_14_1998
        add    esp, 1ch
        push   0
        push   8
        db  068h, 018h, 003h, 000h, 000h
        push   1
        db  068h, 020h, 003h, 000h, 000h
        push   0
        push   edi
        call   sub_14_1998
        lea    eax, [esp + 28h]
        add    esp, 1ch
        push   eax
        lea    eax, [esp + 0ch]
        push   eax
        push   0
        push   0
        push   edi
        call   sub_14_137D
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_14_16C0
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        lea    eax, [esp + 18h]
        db  068h, 028h, 003h, 000h, 000h
        push   eax
        call   sub_14_18B4
        add    esp, 10h
        push   0
        inc    eax
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   1
        db  068h, 030h, 003h, 000h, 000h
        push   0
        push   edi
        call   sub_14_1998
        add    esp, 1ch
loc_14_16C0:
        test   byte ptr [esi + 24dh], 0e0h
        je     SHORT loc_14_16E4
        push   0
        push   2
        db  068h, 040h, 003h, 000h, 000h
        push   1
        db  068h, 044h, 003h, 000h, 000h
        push   0
        push   edi
        call   sub_14_1998
        add    esp, 1ch
loc_14_16E4:
        sub    eax, eax
loc_14_16E6:
        pop    edi
        pop    esi
        add    esp, 88h
        ret
loc_14_16EF:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0ch]
        cmp    eax, 5
        je     SHORT loc_14_1706
        cmp    eax, 6
        je     SHORT loc_14_172F
        mov    eax, 1
        jmp    SHORT loc_14_173F
loc_14_1706:
        mov    esi, dword ptr [esp + 14h]
        push   0
        push   esi
        call   sub_14_1980
        add    esp, 8
        push   esi
        call   sub_14_11AB
        add    esp, 4
        mov    edi, eax
        push   0
        push   esi
        call   sub_14_1988
        add    esp, 8
        mov    eax, edi
        jmp    SHORT loc_14_173F
loc_14_172F:
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        call   sub_14_15C9
        add    esp, 8
loc_14_173F:
        pop    edi
        pop    esi
        ret
loc_14_1742:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 1
        je     SHORT loc_14_1761
        cmp    eax, 2
        je     SHORT loc_14_179C
        cmp    eax, 3
        je     SHORT loc_14_176C
        cmp    eax, 4
        je     SHORT loc_14_1790
        mov    eax, 1
        jmp    SHORT loc_14_17A8
loc_14_1761:
        push   dword ptr [esp + 0ch]
        call   sub_14_0E30
        jmp    SHORT loc_14_17A5
loc_14_176C:
        cmp    dword ptr [esp + 8], 1
        jbe    SHORT loc_14_177A
        mov    eax, 1
        jmp    SHORT loc_14_17A8
loc_14_177A:
        push   dword ptr [esp + 0ch]
        call   sub_14_109D
        add    esp, 4
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 13h
        jmp    SHORT loc_14_17A8
loc_14_1790:
        cmp    dword ptr [esp + 8], 1
        mov    eax, 0
        je     SHORT loc_14_17A8
loc_14_179C:
        push   dword ptr [esp + 0ch]
        call   sub_14_10D2
loc_14_17A5:
        add    esp, 4
loc_14_17A8:
        ret
loc_14_17A9:
        push   0
        push   dword ptr [esp + 8]
        call   sub_14_1978
        add    esp, 8
        ret
sub_14_17B8:
        push   esi
        push   edi
        cmp    dword ptr [esp + 14h], 1
        je     SHORT loc_14_17C8
        mov    eax, 13h
        jmp    SHORT loc_14_182C
loc_14_17C8:
        mov    esi, dword ptr [esp + 10h]
        push   esi
        call   sub_14_0172
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_14_17F6
        push   0
        push   dword ptr [esp + 10h]
        db  068h, 0A9h, 017h, 000h, 000h
        call   sub_14_19B8
        add    esp, 0ch
loc_14_17EE:
        or     edi, edi
        je     SHORT loc_14_1806
        push   0
        jmp    SHORT loc_14_1808
loc_14_17F6:
        db  083h, 03Dh, 03Ch, 006h, 000h, 000h, 000h
        jne    SHORT loc_14_17EE
        mov    eax, 21h
        jmp    SHORT loc_14_182C
loc_14_1806:
        push   3
loc_14_1808:
        push   0
        db  068h, 042h, 017h, 000h, 000h
        push   esi
        call   sub_14_1958
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_14_182C
        push   1
        db  068h, 0EFh, 016h, 000h, 000h
        push   esi
        call   sub_14_1960
        add    esp, 0ch
loc_14_182C:
        pop    edi
        pop    esi
        ret    0ch
sub_14_1831:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     SHORT loc_14_1843
        cmp    eax, -1
        mov    eax, 32h
        jne    SHORT loc_14_1845
loc_14_1843:
        sub    eax, eax
loc_14_1845:
        ret    10h
sub_14_1848:
        db  0CDh, 020h, 00Eh, 080h, 017h, 000h
        add    byte ptr [eax], al
sub_14_1850:
        db  0CDh, 020h, 018h, 080h, 017h, 000h
        add    byte ptr [eax], al
sub_14_1858:
        db  0CDh, 020h, 00Fh, 080h, 017h, 000h
        add    byte ptr [eax], al
sub_14_1860:
        db  0CDh, 020h, 015h, 080h, 017h, 000h
        add    byte ptr [eax], al
sub_14_1868:
        db  0CDh, 020h, 017h, 080h, 017h, 000h
        add    byte ptr [eax], al
sub_14_1870:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_14_1878:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_14_1880:
        db  0CDh, 020h, 04Ah, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_14_1888:
        db  0CDh, 020h, 052h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_14_1890:
        db  0CDh, 020h, 06Ch, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_14_1898:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_14_18A0:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_14_18A8:
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
        db  000h
sub_14_18B4:
        db  0CDh, 020h, 074h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_14_18BC:
        db  0CDh, 020h, 053h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_14_18C4:
        db  0CDh, 020h, 055h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_14_18CC:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
        db  000h
sub_14_18D8:
        db  0CDh, 020h, 063h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_14_18E0:
        db  0CDh, 020h, 064h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_14_18E8:
        db  0CDh, 020h, 0B7h, 000h, 001h, 000h
        mov    eax, edx
        ret
        db  000h, 000h, 000h
sub_14_18F4:
        db  0CDh, 020h, 088h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_14_18FC:
        pop    eax
        pop    ecx
        pop    edx
        db  0CDh, 020h, 089h, 001h, 001h, 000h
        jmp    eax
        db  000h
sub_14_1908:
        db  0CDh, 020h, 08Ah, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_14_1910:
        db  0CDh, 020h, 061h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_14_1918:
        db  0CDh, 020h, 05Eh, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_14_1920:
        db  0CDh, 020h, 062h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_14_1928:
        db  0CDh, 020h, 054h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_14_1930:
        db  0CDh, 020h, 003h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_14_1938:
        db  0CDh, 020h, 004h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_14_1940:
        db  0CDh, 020h, 005h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_14_1948:
        db  0CDh, 020h, 007h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_14_1950:
        db  0CDh, 020h, 00Bh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_14_1958:
        db  0CDh, 020h, 00Eh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_14_1960:
        db  0CDh, 020h, 00Fh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_14_1968:
        db  0CDh, 020h, 01Ah, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_14_1970:
        db  0CDh, 020h, 01Fh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_14_1978:
        db  0CDh, 020h, 032h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_14_1980:
        db  0CDh, 020h, 035h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_14_1988:
        db  0CDh, 020h, 037h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_14_1990:
        db  0CDh, 020h, 03Eh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_14_1998:
        db  0CDh, 020h, 03Fh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_14_19A0:
        db  0CDh, 020h, 044h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_14_19A8:
        db  0CDh, 020h, 048h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_14_19B0:
        db  0CDh, 020h, 049h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_14_19B8:
        db  0CDh, 020h, 04Eh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_14_19C0:
        db  0CDh, 020h, 052h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_14_19C8:
        db  0CDh, 020h, 054h, 080h, 033h, 000h
_PTEXT ends
        end
