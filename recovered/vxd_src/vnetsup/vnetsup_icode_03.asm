; vnetsup_icode_03.asm -- VNETSUP object 3 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_3_0000:
        sub    eax, eax
        db  0BFh, 04Ch, 020h, 000h, 000h
        db  0B9h, 04Ch, 020h, 000h, 000h
        sub    ecx, edi
        shr    ecx, 2
        rep stosd
        db  089h, 01Dh, 014h, 011h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 040h, 000h
        jb     SHORT loc_3_0043
        cmp    eax, 22h
        jne    SHORT loc_3_0043
        call   sub_3_01B8
        db  068h, 07Ch, 037h, 000h, 000h
        push   86h
        push   30ah
        db  0E8h, 099h, 0D4h, 0FFh, 0FFh
        add    esp, 0ch
        ret
loc_3_0043:
        int3
        clc
        ret
loc_3_0046:
        db  0BEh, 02Ah, 001h, 000h, 000h
        mov    eax, 400024h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        db  089h, 035h, 019h, 011h, 000h, 000h
        clc
        ret
        db  0CDh, 020h, 02Ch, 080h, 040h, 000h
sub_3_0064:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        db  0BEh, 0ABh, 030h, 000h, 000h
        mov    edx, dword ptr [esp + 10h]
        db  0CDh, 020h, 0B3h, 000h, 001h, 000h
        mov    eax, edx
        sbb    edx, edx
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [ecx], edx
        pop    edi
        pop    esi
        ret
sub_3_0086:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        db  0BEh, 0ABh, 030h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        sbb    edx, edx
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [ecx], edx
        or     edx, 1
        and    eax, edx
        pop    edi
        pop    esi
        ret
loc_3_00AB:
        dec    esi
        je     NEAR PTR loc_3_0126
        outsd
        db  072h, 06Bh
        db  000h, 08Bh, 00Dh, 01Dh, 011h, 000h
        add    bh, bh
        db  005h, 01Dh, 011h, 000h, 000h
        mov    eax, dword ptr [esp + 4]
        db  089h, 004h, 08Dh, 021h, 011h, 000h, 000h
        ret
sub_3_00CB:
        db  0CDh, 020h, 0B7h, 000h, 001h, 000h
        mov    eax, edx
        ret
sub_3_00D4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        sub    esi, esi
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, 8
        ja     NEAR PTR loc_3_018B
        db  0FFh, 024h, 085h, 094h, 031h, 000h, 000h
        db  08Dh, 049h, 000h
loc_3_00F4:
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 0C0h, 03Ch, 000h, 000h
        push   dword ptr [ebp + 8]
        call   sub_3_0064
        add    esp, 0ch
        mov    edx, eax
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_3_018B
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], 1
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
loc_3_0126:
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [ebp + 14h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], ecx
        mov    esi, 1
        mov    eax, esi
        pop    edi
        pop    esi
        leave
        ret
loc_3_0158:
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   dword ptr [ebp + 8]
        call   sub_3_0086
        add    esp, 0ch
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_3_018B
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], 3
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], 4
        mov    esi, 1
loc_3_018B:
        mov    eax, esi
        pop    edi
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h, 0F4h, 030h, 000h, 000h, 0F4h, 030h, 000h, 000h, 0F4h, 030h, 000h, 000h, 08Bh
        db  031h, 000h, 000h, 08Bh, 031h, 000h, 000h, 058h, 031h, 000h, 000h, 058h, 031h, 000h, 000h, 0F4h
        db  030h, 000h, 000h, 0F4h, 030h, 000h, 000h
sub_3_01B8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0c8h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 78h], 0
        mov    dword ptr [ebp - 74h], 2
        mov    dword ptr [ebp - 70h], 1
        lea    eax, [ebp - 58h]
        mov    dword ptr [ebp - 6ch], eax
        db  0C7h, 045h, 098h, 0A8h, 037h, 000h, 000h
        mov    dword ptr [ebp - 64h], 80000002h
        push   dword ptr [ebp - 6ch]
        push   dword ptr [ebp - 68h]
        push   dword ptr [ebp - 64h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_3_0228
        db  068h, 0A8h, 037h, 000h, 000h
        push   17dbh
        call   sub_3_0734
        add    esp, 8
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_3_0228:
        db  0BBh, 080h, 039h, 000h, 000h
loc_3_022D:
        sub    eax, eax
        mov    dword ptr [ebp - 10h], 50h
        lea    edi, [ebp - 0c8h]
        mov    ecx, 14h
        rep stosd
        test   byte ptr [ebx], 1
        je     SHORT loc_3_0270
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0c8h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebx + 0ch]
        push   dword ptr [ebx + 8]
        call   sub_3_00D4
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_3_0270
        sub    esi, esi
        jmp    NEAR PTR loc_3_035F
loc_3_0270:
        cmp    dword ptr [ebx + 4], 0
        je     NEAR PTR loc_3_0314
        lea    eax, [ebp - 14h]
        mov    dword ptr [ebp - 54h], eax
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ebp - 50h], eax
        mov    dword ptr [ebp - 4ch], 80000002h
        push   dword ptr [ebp - 54h]
        push   dword ptr [ebp - 50h]
        push   dword ptr [ebp - 4ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        mov    esi, eax
        or     esi, esi
        jne    NEAR PTR loc_3_035F
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 48h], eax
        lea    eax, [ebp - 0c8h]
        mov    dword ptr [ebp - 44h], eax
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 40h], eax
        mov    dword ptr [ebp - 3ch], 0
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [ebp - 38h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 34h], eax
        push   dword ptr [ebp - 48h]
        push   dword ptr [ebp - 44h]
        push   dword ptr [ebp - 40h]
        push   dword ptr [ebp - 3ch]
        push   dword ptr [ebp - 38h]
        push   dword ptr [ebp - 34h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    esi, eax
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 30h], eax
        push   dword ptr [ebp - 30h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        jmp    SHORT loc_3_035F
loc_3_0314:
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 2ch], eax
        lea    eax, [ebp - 0c8h]
        mov    dword ptr [ebp - 28h], eax
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 24h], eax
        mov    dword ptr [ebp - 20h], 0
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 58h]
        mov    dword ptr [ebp - 18h], eax
        push   dword ptr [ebp - 2ch]
        push   dword ptr [ebp - 28h]
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    esi, eax
loc_3_035F:
        mov    edx, dword ptr [ebx + 0ch]
        cmp    edx, 8
        ja     NEAR PTR loc_3_053E
        db  0FFh, 024h, 095h, 0FCh, 036h, 000h, 000h
        db  08Bh, 0FFh
loc_3_0374:
        cmp    dword ptr [ebp - 8], 1
        jne    SHORT loc_3_0388
        or     esi, esi
        jne    SHORT loc_3_0388
        lea    esi, [ebp - 0c8h]
        jmp    SHORT loc_3_0393
        db  08Bh, 0FFh
loc_3_0388:
        or     edx, edx
        je     NEAR PTR loc_3_05C8
        mov    esi, dword ptr [ebx + 18h]
loc_3_0393:
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx - 1]
        mov    dword ptr [ebp - 0ch], eax
        mov    ecx, eax
        cmp    dword ptr [ebx + 10h], ecx
        ja     NEAR PTR loc_3_05D8
        mov    eax, dword ptr [ebx + 14h]
        cmp    eax, ecx
        jae    SHORT loc_3_03C4
        cmp    edx, 2
        jne    NEAR PTR loc_3_0630
        mov    dword ptr [ebp - 0ch], eax
loc_3_03C4:
        or     edx, edx
        jne    SHORT loc_3_03CC
        jmp    SHORT loc_3_03CF
        db  08Bh, 0FFh
loc_3_03CC:
        mov    eax, dword ptr [ebp - 0ch]
loc_3_03CF:
        inc    eax
        push   eax
        db  0E8h, 022h, 0D0h, 0FFh, 0FFh
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     NEAR PTR loc_3_063C
        mov    eax, dword ptr [ebx + 1ch]
        mov    edi, dword ptr [ebp - 4]
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebp - 0ch]
        mov    edi, dword ptr [ebp - 4]
        mov    byte ptr [eax + edi], 0
        jmp    NEAR PTR loc_3_053E
loc_3_040C:
        cmp    dword ptr [ebp - 8], 3
        jne    SHORT loc_3_0420
        or     esi, esi
        jne    SHORT loc_3_0420
        mov    eax, dword ptr [ebp - 0c8h]
        jmp    SHORT loc_3_0423
        db  08Bh, 0FFh
loc_3_0420:
        mov    eax, dword ptr [ebx + 18h]
loc_3_0423:
        cmp    dword ptr [ebx + 10h], eax
        ja     NEAR PTR loc_3_0648
        cmp    dword ptr [ebx + 14h], eax
        jb     NEAR PTR loc_3_0654
        mov    ecx, dword ptr [ebx + 1ch]
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_3_053E
        db  090h
loc_3_0440:
        cmp    dword ptr [ebp - 8], 3
        jne    SHORT loc_3_0454
        or     esi, esi
        jne    SHORT loc_3_0454
        mov    ecx, dword ptr [ebp - 0c8h]
        jmp    SHORT loc_3_046F
        db  08Bh, 0FFh
loc_3_0454:
        cmp    dword ptr [ebp - 8], 1
        jne    SHORT loc_3_046C
        or     esi, esi
        jne    SHORT loc_3_046C
        movzx  ecx, byte ptr [ebp - 0c8h]
        sub    ecx, 30h
        jmp    SHORT loc_3_046F
        db  08Bh, 0FFh
loc_3_046C:
        mov    ecx, dword ptr [ebx + 18h]
loc_3_046F:
        cmp    dword ptr [ebx + 10h], ecx
        ja     NEAR PTR loc_3_0660
        cmp    dword ptr [ebx + 14h], ecx
        jb     NEAR PTR loc_3_0660
        cmp    edx, 6
        jne    SHORT loc_3_048A
        mov    ecx, dword ptr [ebp + ecx*4 - 78h]
loc_3_048A:
        mov    eax, dword ptr [ebx + 1ch]
        mov    byte ptr [eax], cl
        jmp    NEAR PTR loc_3_053E
loc_3_0494:
        mov    edi, dword ptr [ebx + 1ch]
        mov    dword ptr [ebp - 4], edi
loc_3_049A:
        cmp    dword ptr [ebp - 8], 1
        jne    SHORT loc_3_04AA
        or     esi, esi
        lea    eax, [ebp - 0c8h]
        je     SHORT loc_3_04AD
loc_3_04AA:
        mov    eax, dword ptr [ebx + 18h]
loc_3_04AD:
        sub    esi, esi
        nop
loc_3_04B0:
        cmp    byte ptr [eax], 0
        je     SHORT loc_3_0534
loc_3_04B5:
        mov    cl, byte ptr [eax]
        cmp    cl, 20h
        je     SHORT loc_3_04C1
        cmp    cl, 9
        jne    SHORT loc_3_04C4
loc_3_04C1:
        inc    eax
        jmp    SHORT loc_3_04B5
loc_3_04C4:
        mov    cl, byte ptr [eax]
        cmp    cl, 30h
        jl     NEAR PTR loc_3_0684
        cmp    cl, 39h
        jg     NEAR PTR loc_3_0684
        movsx  ecx, cl
        sub    ecx, 30h
        inc    eax
        cmp    dword ptr [ebx + 10h], ecx
        ja     NEAR PTR loc_3_066C
        cmp    dword ptr [ebx + 14h], ecx
        jb     NEAR PTR loc_3_0678
        cmp    edx, 7
        jne    SHORT loc_3_0508
        inc    esi
        cmp    esi, 8
        jge    SHORT loc_3_0511
        mov    edi, dword ptr [ebp - 4]
        mov    byte ptr [edi], cl
        inc    dword ptr [ebp - 4]
        jmp    SHORT loc_3_0511
        db  08Bh, 0FFh
loc_3_0508:
        mov    edi, 1
        shl    edi, cl
        or     esi, edi
loc_3_0511:
        mov    cl, byte ptr [eax]
        cmp    cl, 20h
        je     SHORT loc_3_051D
        cmp    cl, 9
        jne    SHORT loc_3_0520
loc_3_051D:
        inc    eax
        jmp    SHORT loc_3_0511
loc_3_0520:
        mov    cl, byte ptr [eax]
        or     cl, cl
        je     SHORT loc_3_0534
        inc    eax
        cmp    cl, 2ch
        je     SHORT loc_3_04B0
        jmp    NEAR PTR loc_3_0684
        db  08Dh, 049h, 000h
loc_3_0534:
        cmp    edx, 8
        jne    SHORT loc_3_053E
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [eax], esi
loc_3_053E:
        add    ebx, 20h
        db  081h, 0FBh, 0C0h, 03Ch, 000h, 000h
        jb     NEAR PTR loc_3_022D
        mov    eax, dword ptr [ebp - 58h]
        mov    dword ptr [ebp - 5ch], eax
        push   dword ptr [ebp - 5ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        db  083h, 03Dh, 034h, 011h, 000h, 000h, 000h
        je     SHORT loc_3_057C
        db  0A1h, 034h, 011h, 000h, 000h
        cmp    byte ptr [eax], 0
        jne    NEAR PTR loc_3_06D2
loc_3_057C:
        call   sub_3_00CB
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_3_06D2
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    ebx, [ecx - 1]
        or     ebx, ebx
        jle    NEAR PTR loc_3_06D2
        push   ebx
        db  0E8h, 04Fh, 0CEh, 0FFh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_3_06B8
        push   8
        call   sub_3_0720
        add    esp, 4
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_3_05C8:
        push   dword ptr [ebx + 8]
        push   17d5h
        jmp    NEAR PTR loc_3_068C
        db  08Dh, 049h, 000h
loc_3_05D8:
        db  0BFh, 0C4h, 03Ch, 000h, 000h
        mov    ecx, 2
        mov    esi, dword ptr [ebx + 8]
        repe cmpsd
        jne    SHORT loc_3_0600
        mov    ax, word ptr [esi]
        cmp    ax, word ptr [edi]
        jne    SHORT loc_3_0600
        push   dword ptr [ebx + 8]
        push   1874h
        jmp    NEAR PTR loc_3_068C
        db  08Bh, 0FFh
loc_3_0600:
        db  0BFh, 0D0h, 03Ch, 000h, 000h
        mov    ecx, 3
        mov    esi, dword ptr [ebx + 8]
        repe cmpsd
        jne    SHORT loc_3_0624
        mov    al, byte ptr [esi]
        cmp    al, byte ptr [edi]
        jne    SHORT loc_3_0624
        push   dword ptr [ebx + 8]
        push   1875h
        jmp    SHORT loc_3_068C
        db  08Dh, 049h, 000h
loc_3_0624:
        push   dword ptr [ebx + 8]
        push   17d6h
        jmp    SHORT loc_3_068C
        db  08Bh, 0FFh
loc_3_0630:
        push   dword ptr [ebx + 8]
        push   17d7h
        jmp    SHORT loc_3_068C
        db  08Bh, 0FFh
loc_3_063C:
        push   8
        call   sub_3_0720
        add    esp, 4
        jmp    SHORT loc_3_0694
loc_3_0648:
        push   dword ptr [ebx + 8]
        push   17d8h
        jmp    SHORT loc_3_068C
        db  08Bh, 0FFh
loc_3_0654:
        push   dword ptr [ebx + 8]
        push   17d9h
        jmp    SHORT loc_3_068C
        db  08Bh, 0FFh
loc_3_0660:
        push   dword ptr [ebx + 8]
        push   17dah
        jmp    SHORT loc_3_068C
        db  08Bh, 0FFh
loc_3_066C:
        push   dword ptr [ebx + 8]
        push   17d8h
        jmp    SHORT loc_3_068C
        db  08Bh, 0FFh
loc_3_0678:
        push   dword ptr [ebx + 8]
        push   17d9h
        jmp    SHORT loc_3_068C
        db  08Bh, 0FFh
loc_3_0684:
        push   dword ptr [ebx + 8]
        push   17e9h
loc_3_068C:
        call   sub_3_0734
        add    esp, 8
loc_3_0694:
        mov    eax, dword ptr [ebp - 58h]
        mov    dword ptr [ebp - 60h], eax
        push   dword ptr [ebp - 60h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_3_06B8:
        mov    edi, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        mov    byte ptr [ebx + eax - 1], 0
        db  0A3h, 034h, 011h, 000h, 000h
loc_3_06D2:
        db  083h, 03Dh, 040h, 011h, 000h, 000h, 000h
        je     SHORT loc_3_06F2
        db  08Bh, 03Dh, 040h, 011h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        db  089h, 00Dh, 044h, 011h, 000h, 000h
loc_3_06F2:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h, 074h, 033h, 000h, 000h, 074h, 033h, 000h, 000h, 074h, 033h, 000h, 000h, 00Ch
        db  034h, 000h, 000h, 00Ch, 034h, 000h, 000h, 040h, 034h, 000h, 000h, 040h, 034h, 000h, 000h, 094h
        db  034h, 000h, 000h, 09Ah, 034h, 000h, 000h
sub_3_0720:
        push   ebp
        mov    ebp, esp
        db  0C6h, 005h, 018h, 011h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 07Ch, 037h, 000h, 000h
        leave
        ret
sub_3_0734:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_3_0720
        add    esp, 4
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        db  066h, 001h, 00Dh, 084h, 037h, 000h, 000h
        mov    eax, ecx
        db  0BFh, 088h, 037h, 000h, 000h
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  0C6h, 005h, 087h, 037h, 000h, 000h, 001h
        pop    edi
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 080h, 004h, 000h, 000h
loc_3_0784:
        or     al, 0
        add    byte ptr [eax], al
loc_3_0788:
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
loc_3_07A8:
        push   ebx
        db  079h, 073h
        je     SHORT loc_3_0812
        insd
        pop    esp
        inc    ebx
        jne    SHORT loc_3_0824
        jb     SHORT loc_3_0819
        outsb
        je     SHORT loc_3_07FA
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        push   ebx
        je     NEAR PTR loc_3_081D
        push   ebx
        jb     NEAR PTR loc_3_083B
        imul   esp, dword ptr [ebx + 65h], 78565c73h
        inc    esp
        pop    esp
        push   esi
        dec    esi
        je     NEAR PTR loc_3_0846
        db  075h, 070h
        add    byte ptr [eax], al
        add    byte ptr [ecx + eax*2 + 4eh], cl
        push   edx
        dec    edi
        dec    edi
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 79h], dl
        db  073h, 074h
        insd
        pop    esp
        inc    ebx
        jne    SHORT loc_3_0860
        jb     SHORT loc_3_0855
        outsb
        je     SHORT loc_3_0836
        outsd
        outsb
        je     SHORT loc_3_0869
        outsd
        insb
        push   ebx
loc_3_07FA:
        je     NEAR PTR loc_3_0859
        arpl   word ptr [edi + 6eh], bp
        je     SHORT loc_3_0874
        outsd
        insb
        pop    esp
        arpl   word ptr [edi + 6dh], bp
        db  070h, 075h
        je     SHORT loc_3_0871
        jb     SHORT loc_3_087C
        popad
        insd
        pop    esp
loc_3_0812:
        arpl   word ptr [edi + 6dh], bp
        db  070h, 075h
        je     SHORT loc_3_087E
loc_3_0819:
        jb     SHORT loc_3_0889
        popad
        insd
loc_3_081D:
        add    byte ptr gs:[eax], al
loc_3_0820:
        inc    ebx
        dec    edi
        dec    ebp
        push   eax
loc_3_0824:
        push   ebp
        push   esp
        inc    ebp
        push   edx
        dec    esi
        inc    ecx
        dec    ebp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0830:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0834:
        push   edi
        dec    edi
loc_3_0836:
        push   edx
        dec    ebx
        inc    edi
        push   edx
        dec    edi
loc_3_083B:
        push   ebp
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 4fh], al
loc_3_0846:
        dec    ebp
        dec    ebp
        inc    ebp
        dec    esi
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 55h], cl
        dec    ebp
        inc    esp
        inc    edi
loc_3_0855:
        push   edx
        inc    ecx
        dec    ebp
        inc    edx
loc_3_0859:
        push   ebp
        inc    esi
        add    byte ptr [esi + 55h], cl
        dec    ebp
        push   edi
loc_3_0860:
        dec    edi
        push   edx
        dec    ebx
        inc    edx
        push   ebp
        inc    esi
        add    byte ptr [eax], al
loc_3_0868:
        push   ebx
loc_3_0869:
        dec    ecx
        pop    edx
        push   edi
        dec    edi
        push   edx
        dec    ebx
        inc    edx
        push   ebp
loc_3_0871:
        inc    esi
        add    byte ptr [eax], al
loc_3_0874:
        dec    ebx
        inc    ebp
        inc    ebp
        push   eax
        inc    ebx
        dec    edi
        dec    esi
        dec    esi
loc_3_087C:
        add    byte ptr [eax], al
loc_3_087E:
        add    byte ptr [eax], al
loc_3_0880:
        push   ebx
        inc    ebp
        push   ebx
        push   ebx
        push   esp
        dec    ecx
        dec    ebp
        inc    ebp
        dec    edi
loc_3_0889:
        push   ebp
        push   esp
        add    byte ptr [ebx + 48h], al
        inc    ecx
        push   edx
        inc    ebx
        dec    edi
        push   ebp
        dec    esi
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [ebx + 48h], al
        inc    ecx
        push   edx
        push   esp
        dec    ecx
        dec    ebp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_08A4:
        inc    ebp
        pop    eax
        inc    ebx
        dec    esp
        push   ebp
        inc    esp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx + 41h], dl
        push   edi
        inc    edx
        push   ebp
        inc    esi
        add    byte ptr [eax], al
loc_3_08B8:
        push   eax
        push   edx
        dec    ecx
        dec    edi
        push   edx
        dec    ecx
        push   esp
        pop    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_08C4:
        dec    ebp
        inc    ecx
        dec    ecx
        dec    esi
        push   esp
        inc    ecx
        dec    ecx
        dec    esi
        push   ebx
        inc    ebp
        push   edx
        push   esi
        inc    ebp
        push   edx
        dec    esp
        dec    ecx
        push   ebx
        push   esp
        add    byte ptr [eax], al
loc_3_08D8:
        dec    esp
        dec    ebp
        inc    ecx
        dec    esi
        dec    esi
        dec    edi
        push   ebp
        dec    esi
        inc    ebx
        inc    ebp
        add    byte ptr [eax], al
loc_3_08E4:
        inc    ecx
        push   ebp
        inc    esp
        dec    ecx
        push   esp
        inc    ebp
        dec    esi
        inc    ecx
        inc    edx
        dec    esp
        inc    ebp
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_08F4:
        inc    ecx
        push   ebp
        inc    esp
        dec    ecx
        push   esp
        inc    ebp
        push   esi
        inc    ebp
        dec    esi
        push   esp
        push   ebx
        add    byte ptr [ecx + 55h], al
        inc    esp
        dec    ecx
        push   esp
        dec    esp
        dec    edi
        inc    edi
        push   ebx
        dec    ecx
        pop    edx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0910:
        push   eax
        push   edx
        inc    ebp
        inc    esi
        push   esp
        push   edx
        inc    ecx
        dec    esi
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 4ch], dl
        dec    edi
        push   edi
        dec    esp
        inc    ecx
        dec    esi
        inc    ecx
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx + ecx*2 + 52h], al
        inc    ebp
        inc    ebx
        push   esp
        dec    eax
        dec    edi
        push   ebx
        push   esp
        add    byte ptr [eax], al
loc_3_093C:
        inc    ecx
        push   ebp
        push   esp
        dec    edi
        inc    esp
        dec    ecx
        push   ebx
        inc    ebx
        dec    edi
        dec    esi
        dec    esi
        inc    ebp
        inc    ebx
        push   esp
        add    byte ptr [eax], al
loc_3_094C:
        inc    esp
        push   edx
        dec    edi
        push   eax
        push   esp
        dec    ecx
        dec    ebp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0958:
        dec    esi
        dec    edi
        dec    esi
        push   eax
        dec    esi
        push   eax
        push   esp
        push   edx
        inc    ecx
        dec    esi
        push   ebx
        push   eax
        dec    edi
        push   edx
        push   esp
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_096C:
        dec    esi
        dec    edi
        dec    esi
        push   eax
        dec    esi
        push   eax
        push   eax
        dec    ebp
        push   esp
        push   edx
        inc    ecx
        dec    esi
        push   ebx
        push   eax
        dec    edi
        push   edx
        push   esp
        push   ebx
        add    byte ptr [eax], al
loc_3_0980:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        fdiv   dword ptr [edi]
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    esi
        add    byte ptr [eax], al
        db  000h, 0E0h
        aaa
        add    byte ptr [eax], al
        xor    al, 11h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        in     al, 37h
        add    byte ptr [eax], al
        and    byte ptr [eax], bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        sldt   word ptr [eax]
        db  000h, 030h
        cmp    byte ptr [eax], al
        db  000h, 038h
        adc    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 034h, 038h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        sldt   word ptr [eax]
        db  000h, 040h, 038h
        add    byte ptr [eax], al
        cmp    al, 11h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esp
        cmp    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dh
        add    byte ptr [eax], al
        db  000h, 04Ch, 038h, 000h
        db  000h, 040h, 011h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   eax
        cmp    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        db  000h, 048h, 011h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pop    esp
        cmp    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        dec    esp
        adc    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 068h, 038h
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    al, 0bh
        add    byte ptr [eax], al
        push   eax
        adc    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 074h, 038h, 000h
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, ch
        std
        add    byte ptr [eax], al
        pop    eax
        add    al, byte ptr [eax]
        db  000h, 054h, 011h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 080h, 038h, 000h, 000h, 003h
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], dh
        jne    SHORT loc_3_0A97
loc_3_0A97:
        db  000h, 02Dh, 000h, 000h, 000h, 058h
        adc    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 08Ch, 038h, 000h, 000h, 003h, 000h
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        db  000h, 05Ch, 011h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 098h, 038h, 000h, 000h, 003h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, ch
        std
        add    byte ptr [eax], al
        cli
        add    byte ptr [eax], al
        db  000h, 060h, 011h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        movsb
        cmp    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        db  000h, 0ACh, 038h, 000h, 000h, 064h, 011h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    al, 38h
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   11h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0B8h, 038h, 000h, 000h, 003h
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], ch
        and    eax, dword ptr [eax]
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        insb
        adc    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0C4h
        cmp    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        db  000h, 079h, 011h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        fdivr  dword ptr [eax]
        add    byte ptr [eax], al
        add    eax, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 07Ah, 011h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        in     al, 38h
        add    byte ptr [eax], al
        add    eax, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 07Bh, 011h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        hlt
        cmp    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    dword ptr [ebx]
        add    byte ptr [eax], al
        inc    dword ptr [ebx]
        add    byte ptr [eax], al
        db  07Ch, 011h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], bh
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        out    3, eax
        add    byte ptr [eax], al
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
        adc    byte ptr [ecx], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 010h
        cmp    dword ptr [eax], eax
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], bl
        add    byte ptr [eax], al
        db  000h, 01Ch, 039h
        add    byte ptr [eax], al
        mov    byte ptr [ecx], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        and    byte ptr [ecx], bh
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pop    ds
        add    byte ptr [eax], al
        db  000h, 02Ch, 039h
        add    byte ptr [eax], al
        test   byte ptr [ecx], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        xor    byte ptr [ecx], bh
        add    byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        db  000h, 078h, 011h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cmp    al, 39h
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0E8h, 0FDh, 000h, 000h, 03Ch
        add    byte ptr [eax], al
        db  000h, 070h, 011h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    esp
        cmp    dword ptr [eax], eax
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [40000000h], al
        or     eax, dword ptr [eax]
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        db  000h, 074h, 011h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 058h, 039h
        add    byte ptr [eax], al
        add    eax, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        db  000h, 090h, 011h, 000h, 000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 06Ch, 039h, 000h
        add    byte ptr [0], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 091h, 011h, 000h, 000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [edi + 4fh], dl
        push   edx
        dec    ebx
        inc    edi
        push   edx
        dec    edi
        push   ebp
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [ebx + 4fh], al
        dec    ebp
        push   eax
        push   ebp
        push   esp
        inc    ebp
        push   edx
        dec    esi
        inc    ecx
        dec    ebp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_ITEXT ends
        end
