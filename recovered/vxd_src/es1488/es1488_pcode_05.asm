; es1488_pcode_05.asm -- ES1488 object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_5_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        db  0CDh, 020h, 088h, 001h, 001h, 000h
        test   eax, eax
        jne    SHORT loc_5_004D
        mov    dword ptr [ebp - 0ch], 0
        db  0C7h, 045h, 0F0h, 000h, 000h, 000h, 000h
        push   ecx
        mov    edx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 0ch]
        db  0CDh, 020h, 089h, 001h, 001h, 000h
        pop    ecx
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        push   ecx
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        db  0CDh, 020h, 089h, 001h, 001h, 000h
        pop    ecx
        db  0CDh, 020h, 08Ah, 001h, 001h, 000h
loc_5_004D:
        mov    esp, ebp
        pop    ebp
        ret
sub_5_0051:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 130h
        test   eax, eax
        push   ebx
        push   esi
        push   edi
        je     NEAR PTR loc_5_036A
        cmp    eax, 1
        je     SHORT loc_5_00A5
        cmp    eax, 2
        je     NEAR PTR loc_5_0355
        cmp    eax, 3
        je     NEAR PTR loc_5_02C7
        cmp    eax, 4
        je     NEAR PTR loc_5_0313
        cmp    eax, 8
        je     NEAR PTR loc_5_031B
        cmp    eax, 0dh
        je     NEAR PTR loc_5_0355
        mov    eax, 1
        jmp    NEAR PTR loc_5_036C
loc_5_00A5:
        mov    eax, 0fffeh
        mov    ecx, 0ffffh
        mov    word ptr [ebp - 38h], ax
        mov    word ptr [ebp - 36h], ax
        mov    word ptr [ebp - 34h], ax
        mov    word ptr [ebp - 32h], ax
        mov    word ptr [ebp - 30h], cx
        push   0
        mov    word ptr [ebp - 2ch], ax
        mov    esi, dword ptr [ebp + 10h]
        mov    word ptr [ebp - 2ah], ax
        push   esi
        mov    word ptr [ebp - 28h], ax
        lea    eax, [ebp - 130h]
        mov    word ptr [ebp - 26h], cx
        push   eax
        call   sub_5_16E8
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_5_036C
        movzx  eax, word ptr [ebp - 0d4h]
        mov    bx, 0ffffh
        cmp    eax, 1
        je     SHORT loc_5_010F
        cmp    eax, 2
        je     SHORT loc_5_0130
        mov    eax, 13h
        jmp    NEAR PTR loc_5_036C
loc_5_010F:
        cmp    word ptr [ebp - 0aah], 10h
        je     SHORT loc_5_0123
        mov    eax, 13h
        jmp    NEAR PTR loc_5_036C
loc_5_0123:
        mov    ax, word ptr [ebp - 0d2h]
        mov    word ptr [ebp - 2], ax
        jmp    SHORT loc_5_016C
loc_5_0130:
        cmp    word ptr [ebp - 0aah], 10h
        je     SHORT loc_5_0144
        mov    eax, 13h
        jmp    NEAR PTR loc_5_036C
loc_5_0144:
        movzx  ecx, word ptr [ebp - 0a8h]
        mov    ax, word ptr [ebp - 0d2h]
        cmp    ecx, 2
        mov    word ptr [ebp - 2], ax
        je     SHORT loc_5_0165
        mov    eax, 13h
        jmp    NEAR PTR loc_5_036C
loc_5_0165:
        mov    bx, word ptr [ebp - 0d0h]
loc_5_016C:
        cmp    word ptr [ebp - 82h], 1
        jb     NEAR PTR loc_5_02BD
        cmp    word ptr [ebp - 72h], 1
        jb     NEAR PTR loc_5_02BD
        movzx  ax, byte ptr [ebp - 80h]
        movzx  cx, byte ptr [ebp - 70h]
        mov    word ptr [ebp - 4], ax
        cmp    word ptr [ebp - 2], 0ffffh
        mov    word ptr [ebp - 6], cx
        jne    SHORT loc_5_01A9
        mov    eax, 13h
        jmp    NEAR PTR loc_5_036C
loc_5_01A9:
        cmp    bx, 0ffffh
        je     SHORT loc_5_01B8
        mov    bx, word ptr [ebp - 2]
        add    bx, 8
loc_5_01B8:
        xor    edi, edi
        lea    eax, [ebp - 24h]
        push   edi
        lea    ecx, [ebp - 0ch]
        push   edi
        mov    dword ptr [ebp - 24h], edi
        push   eax
        push   ecx
        push   esi
        call   sub_5_16E0
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_5_01D7
        mov    dword ptr [ebp - 0ch], edi
loc_5_01D7:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    eax, 8
        je     SHORT loc_5_01EB
        cmp    eax, 10h
        je     SHORT loc_5_01F2
        cmp    eax, 40h
        je     SHORT loc_5_01F9
        jmp    SHORT loc_5_01FE
loc_5_01EB:
        mov    edi, 20h
        jmp    SHORT loc_5_01FE
loc_5_01F2:
        mov    edi, 10h
        jmp    SHORT loc_5_01FE
loc_5_01F9:
        mov    edi, 40h
loc_5_01FE:
        push   edi
        lea    eax, [ebp - 2ch]
        push   20h
        lea    ecx, [ebp - 38h]
        push   eax
        push   ecx
        push   dword ptr [ebp - 6]
        push   dword ptr [ebp - 4]
        push   ebx
        push   dword ptr [ebp - 2]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0219:
        add    esp, 24h
        mov    edi, eax
        test   edi, edi
        jne    NEAR PTR loc_5_02A9
        sub    bx, 0ffffh
        mov    dword ptr [ebp - 18h], esi
        db  0C7h, 045h, 0E4h, 000h, 000h, 000h, 000h
        cmp    bx, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [ebp - 20h], eax
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 001h, 000h, 04Ah, 004h
        add    esp, 0ch
        test   edi, edi
        jne    SHORT loc_5_02B6
        cmp    word ptr [ebp - 4], 9
        mov    eax, 2
        je     SHORT loc_5_026B
        movzx  eax, word ptr [ebp - 4]
loc_5_026B:
        movzx  ecx, word ptr [ebp - 6]
        movzx  edx, word ptr [ebp - 2]
        push   ecx
        push   eax
        push   edx
        lea    ecx, [ebp - 58h]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_5_16D8
        add    esp, 14h
        lea    ecx, [ebp - 58h]
        db  0C7h, 045h, 0F0h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 14h], ecx
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 10h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 002h, 000h, 04Ah, 004h
        add    esp, 8
loc_5_02A9:
        test   edi, edi
        jne    SHORT loc_5_02B6
        push   esi
        call   sub_5_10E9
        add    esp, 4
loc_5_02B6:
        mov    eax, edi
        jmp    NEAR PTR loc_5_036C
loc_5_02BD:
        mov    eax, 13h
        jmp    NEAR PTR loc_5_036C
loc_5_02C7:
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        je     SHORT loc_5_02DD
        cmp    eax, 1
        je     SHORT loc_5_02F8
        mov    eax, 1
        jmp    NEAR PTR loc_5_036C
loc_5_02DD:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_02E5:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 13h
        jmp    SHORT loc_5_036C
loc_5_02F8:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0300:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 13h
        jmp    SHORT loc_5_036C
loc_5_0313:
        cmp    dword ptr [ebp + 0ch], 1
        jne    SHORT loc_5_0355
        jmp    SHORT loc_5_036A
loc_5_031B:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 7fffffffh
        cmp    eax, 4
        jb     SHORT loc_5_034E
        cmp    eax, 5
        jbe    SHORT loc_5_0339
        cmp    eax, 6
        jb     SHORT loc_5_034E
        cmp    eax, 8
        jbe    SHORT loc_5_0343
        jmp    SHORT loc_5_034E
loc_5_0339:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0341:
        jmp    SHORT loc_5_034B
loc_5_0343:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_034B:
        add    esp, 4
loc_5_034E:
        mov    eax, 1
        jmp    SHORT loc_5_036C
loc_5_0355:
        mov    esi, dword ptr [ebp + 10h]
        push   esi
        call   sub_5_1198
        add    esp, 4
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0367:
        add    esp, 4
loc_5_036A:
        xor    eax, eax
loc_5_036C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  0CCh
loc_5_0374:
        push   eax
        mov    eax, dword ptr [esi + 2ch]
        cmp    eax, ebx
        je     SHORT loc_5_0394
        or     eax, eax
        jne    SHORT loc_5_03A7
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 1
        call   sub_5_0691
        pop    edx
        pop    ecx
        pop    ebx
        jb     SHORT loc_5_03A7
loc_5_0394:
        pop    eax
        cmp    ecx, 4
        jbe    SHORT loc_5_03A0
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_5_03A0:
        je     SHORT loc_5_03A5
        in     al, dx
        jmp    SHORT loc_5_03A6
loc_5_03A5:
        out    dx, al
loc_5_03A6:
        ret
loc_5_03A7:
        mov    eax, 1
        mov    edi, esi
        call   sub_5_04EF
        pop    eax
        xor    eax, eax
        dec    eax
        ret
loc_5_03B8:
        push   eax
        mov    eax, dword ptr [esi + 34h]
        cmp    eax, ebx
        je     SHORT loc_5_03D8
        or     eax, eax
        jne    SHORT loc_5_03EB
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 2
        call   sub_5_0691
        pop    edx
        pop    ecx
        pop    ebx
        jb     SHORT loc_5_03EB
loc_5_03D8:
        pop    eax
        cmp    ecx, 4
        jbe    SHORT loc_5_03E4
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_5_03E4:
        je     SHORT loc_5_03E9
        in     al, dx
        jmp    SHORT loc_5_03EA
loc_5_03E9:
        out    dx, al
loc_5_03EA:
        ret
loc_5_03EB:
        mov    eax, 2
        mov    edi, esi
        call   sub_5_04EF
        pop    eax
        xor    eax, eax
        dec    eax
        ret
sub_5_03FC:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 0fh
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_040A:
        cmp    ecx, 8
        je     SHORT loc_5_041A
        cmp    ecx, 9
        je     SHORT loc_5_041A
        db  0CDh, 020h, 098h, 000h, 001h, 000h
loc_5_041A:
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_0425
        inc    cx
        jmp    SHORT loc_5_040A
loc_5_0425:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_042B:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 0fh
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_0439:
        cmp    ecx, 8
        je     SHORT loc_5_0449
        cmp    ecx, 9
        je     SHORT loc_5_0449
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
loc_5_0449:
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_0454
        inc    cx
        jmp    SHORT loc_5_0439
loc_5_0454:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_045A:
        push   eax
        push   ebx
        push   edx
        movzx  edx, word ptr [edi + 8]
        mov    ebx, 4
        mov    eax, 60h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0470:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0475:
        pop    edx
        pop    ebx
        pop    eax
        ret
sub_5_0479:
        push   esi
        push   eax
        push   ecx
        movzx  edx, word ptr [edi + 6]
        add    dx, 8
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        inc    dx
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        mov    si, 1
        xor    ecx, ecx
        mov    edx, 388h
loc_5_049D:
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_04AE
        inc    cx
        jmp    SHORT loc_5_049D
loc_5_04AE:
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_04B3:
        push   esi
        push   eax
        push   ecx
        push   edx
        movzx  edx, word ptr [edi + 6]
        add    dx, 8
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        inc    dx
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        mov    si, 1
        xor    ecx, ecx
        mov    edx, 388h
loc_5_04D8:
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_04E9
        inc    cx
        jmp    SHORT loc_5_04D8
loc_5_04E9:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_04EF:
        test   word ptr [edi + 4], 1
        jne    SHORT loc_5_0544
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_04FC:
        jb     SHORT loc_5_0544
        cmp    eax, 2
        je     SHORT loc_5_051D
        test   dword ptr [esi], 1
        jne    SHORT loc_5_0544
        or     dword ptr [esi], 1
        db  00Fh, 0B7h, 00Dh, 00Ch, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        jmp    SHORT loc_5_0535
loc_5_051D:
        test   dword ptr [esi], 2
        jne    SHORT loc_5_0544
        or     dword ptr [esi], 2
        db  00Fh, 0B7h, 00Dh, 00Eh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
loc_5_0535:
        mov    eax, 30h
        xor    esi, esi
        xor    edi, edi
        db  0CDh, 020h, 004h, 000h, 017h, 000h
loc_5_0544:
        ret
loc_5_0545:
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_0592
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        sti
        je     SHORT loc_5_0592
loc_5_0559:
        push   eax
        mov    edi, dword ptr [eax]
        xor    eax, eax
        cmp    dword ptr [edi + 2ch], ebx
        jne    SHORT loc_5_056D
        or     eax, 1
        mov    dword ptr [edi + 30h], 0ffffffffh
loc_5_056D:
        cmp    dword ptr [edi + 34h], ebx
        jne    SHORT loc_5_057C
        or     eax, 2
        mov    dword ptr [edi + 38h], 0ffffffffh
loc_5_057C:
        or     eax, eax
        je     SHORT loc_5_0585
        call   sub_5_060A
loc_5_0585:
        pop    eax
        cli
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        sti
        je     SHORT loc_5_0592
        jmp    SHORT loc_5_0559
loc_5_0592:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        xor    esi, esi
        xchg   dword ptr [edi + ebx], esi
        or     esi, esi
        je     SHORT loc_5_05A7
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_5_05A7:
        clc
        ret
loc_5_05A9:
        pushad
        pushfd
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    eax, 5
        mov    ecx, 24h
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_5_0602
        mov    dword ptr [edi + ebx], esi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_0606
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        sti
        je     SHORT loc_5_0606
loc_5_05DA:
        mov    edi, dword ptr [eax]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_05E1:
        jb     SHORT loc_5_05EF
        cli
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        sti
        je     SHORT loc_5_0606
        jmp    SHORT loc_5_05DA
loc_5_05EF:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     SHORT loc_5_0602
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_5_0602:
        popfd
        stc
        jmp    SHORT loc_5_0608
loc_5_0606:
        popfd
        clc
loc_5_0608:
        popad
        ret
sub_5_060A:
        push   eax
        push   edx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0612:
        jb     SHORT loc_5_068C
        test   eax, 2
        je     SHORT loc_5_0634
        cmp    dword ptr [edi + 34h], ebx
        jne    SHORT loc_5_0634
        call   sub_5_045A
        call   sub_5_0479
        mov    dword ptr [edi + 34h], 0
        and    dword ptr [esi], 0fffffffdh
loc_5_0634:
        test   eax, 1
        je     SHORT loc_5_0670
        cmp    dword ptr [edi + 2ch], ebx
        jne    SHORT loc_5_0670
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_5_0651
        mov    eax, dword ptr [edi + 3ch]
        db  0CDh, 020h, 014h, 000h, 004h, 000h
loc_5_0651:
        call   sub_5_03FC
        movzx  edx, word ptr [edi + 6]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_065F:
        jb     SHORT loc_5_0666
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0666:
        mov    dword ptr [edi + 2ch], 0
        and    dword ptr [esi], 0fffffffeh
loc_5_0670:
        mov    eax, dword ptr [edi + 2ch]
        or     eax, eax
        jne    SHORT loc_5_068C
        mov    eax, dword ptr [edi + 28h]
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        db  0CDh, 020h, 003h, 000h, 003h, 000h
loc_5_068C:
        pop    esi
        pop    edx
        pop    eax
        clc
        ret
sub_5_0691:
        push   edx
        push   ecx
        push   eax
        test   eax, 2
        je     SHORT loc_5_06BA
        cmp    word ptr [edi + 8], -1
        je     SHORT loc_5_06CC
        cmp    dword ptr [edi + 34h], 0
        je     SHORT loc_5_06AF
        cmp    dword ptr [edi + 34h], ebx
        je     SHORT loc_5_06BA
        jmp    SHORT loc_5_06CC
loc_5_06AF:
        mov    dword ptr [edi + 34h], ebx
        call   sub_5_04B3
        mov    dword ptr [edi + 38h], ebx
loc_5_06BA:
        test   eax, 1
        je     SHORT loc_5_06FE
        cmp    dword ptr [edi + 2ch], 0
        je     SHORT loc_5_06CF
        cmp    dword ptr [edi + 2ch], ebx
        je     SHORT loc_5_0727
loc_5_06CC:
        stc
        jmp    SHORT loc_5_0728
loc_5_06CF:
        mov    dword ptr [edi + 2ch], ebx
        cmp    dword ptr [edi + 30h], ebx
        je     SHORT loc_5_06E0
        movzx  edx, word ptr [edi + 6]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_06E0:
        mov    dword ptr [edi + 30h], ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_06E8:
        call   sub_5_042B
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_5_06FE
        mov    eax, dword ptr [edi + 3ch]
        db  0CDh, 020h, 013h, 000h, 004h, 000h
loc_5_06FE:
        mov    eax, dword ptr [esp]
        test   eax, 1
        je     SHORT loc_5_0727
        mov    eax, dword ptr [edi + 28h]
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   ecx, 10h
        je     SHORT loc_5_0721
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        jmp    SHORT loc_5_0727
loc_5_0721:
        db  0CDh, 020h, 009h, 000h, 003h, 000h
loc_5_0727:
        clc
loc_5_0728:
        pop    eax
        pop    ecx
        pop    edx
        ret
sub_5_072C:
        mov    eax, 400h
        clc
        ret
sub_5_0733:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0741:
        add    esp, 8
        xor    eax, eax
        or     edi, edi
        je     SHORT loc_5_074D
        mov    eax, dword ptr [edi + 6ah]
loc_5_074D:
        pop    edi
        pop    ebp
        ret
sub_5_0750:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_075E:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_076B
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [edi + 6ah], eax
loc_5_076B:
        pop    edi
        pop    ebp
        ret
        db  0CCh, 0CCh
AUDDRV_PM_API:
        movzx  eax, word ptr [ebp + 14h]
        cmp    ah, 3
        jae    SHORT loc_5_079A
        movzx  ecx, ah
        shl    ecx, 3
        db  08Dh, 089h, 000h, 000h, 000h, 000h
        movzx  eax, al
        cmp    eax, dword ptr [ecx]
        jae    SHORT loc_5_079A
        mov    ecx, dword ptr [ecx + 4]
        call   dword ptr [ecx + eax*4]
        jb     SHORT loc_5_079A
        and    word ptr [ebp + 2ch], 0fffeh
        ret
loc_5_079A:
        or     word ptr [ebp + 2ch], 1
        ret
loc_5_07A0:
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_07AC:
        add    esp, 8
        jb     SHORT loc_5_07D2
        movzx  eax, word ptr [ebp + 10h]
        test   eax, 0ffffffffh
        jne    SHORT loc_5_07D2
        mov    eax, dword ptr [edi + 3ch]
        or     eax, eax
        je     SHORT loc_5_07D2
        push   ecx
        db  0CDh, 020h, 01Ch, 000h, 004h, 000h
        mov    word ptr [ebp + 1ch], cx
        pop    ecx
        clc
        jmp    SHORT loc_5_07D9
loc_5_07D2:
        mov    word ptr [ebp + 1ch], 0ffffh
        stc
loc_5_07D9:
        pop    edi
        ret
loc_5_07DB:
        call   sub_5_072C
        mov    word ptr [ebp + 1ch], ax
        clc
        ret
loc_5_07E6:
        movzx  eax, word ptr [ebp + 10h]
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_0828
        test   eax, 1
        je     SHORT loc_5_080D
        push   2
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0801:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_0828
        mov    ecx, dword ptr [edi + 30h]
        jmp    SHORT loc_5_0830
loc_5_080D:
        test   eax, 2
        je     SHORT loc_5_0828
        push   3
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_081C:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_0828
        mov    ecx, dword ptr [edi + 38h]
        jmp    SHORT loc_5_0830
loc_5_0828:
        mov    word ptr [ebp + 1ch], 1
loc_5_082E:
        stc
        ret
loc_5_0830:
        mov    word ptr [ebp + 1ch], 0
        call   sub_5_0691
        jb     SHORT loc_5_083E
        ret
loc_5_083E:
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_5_082E
loc_5_0846:
        movzx  eax, word ptr [ebp + 10h]
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_08A2
        test   eax, 1
        je     SHORT loc_5_0887
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_5_0873
        push   2
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0869:
        add    esp, 8
        or     edi, edi
        jne    SHORT loc_5_0882
        add    edx, 4
loc_5_0873:
        push   2
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_087B:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_08A2
loc_5_0882:
        mov    ecx, dword ptr [edi + 2ch]
        jmp    SHORT loc_5_08AA
loc_5_0887:
        test   eax, 2
        je     SHORT loc_5_08A2
        push   3
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0896:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_08A2
        mov    ecx, dword ptr [edi + 34h]
        jmp    SHORT loc_5_08AA
loc_5_08A2:
        mov    word ptr [ebp + 1ch], 1
loc_5_08A8:
        stc
        ret
loc_5_08AA:
        cmp    ecx, ebx
        je     SHORT loc_5_08B6
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_5_08A8
loc_5_08B6:
        call   sub_5_060A
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
loc_5_08C3:
        pushfd
        mov    ax, word ptr [ebp + 1ch]
        and    ax, 0fffeh
        jne    SHORT loc_5_0931
        xor    ecx, ecx
        mov    ax, word ptr [ebp + 1ch]
        and    ax, 1
        je     SHORT loc_5_08DD
        mov    ecx, dword ptr [ebp + 18h]
loc_5_08DD:
        cli
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_0931
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_5_0931
        mov    edi, dword ptr [eax]
        jecxz  loc_5_0905
loc_5_08F4:
        cmp    dword ptr [edi + 44h], ecx
        je     SHORT loc_5_0905
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_5_0931
        mov    edi, dword ptr [eax]
        jmp    SHORT loc_5_08F4
loc_5_0905:
        popfd
        pushfd
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cmp    edi, -1
        je     SHORT loc_5_0931
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     SHORT loc_5_0931
        cmp    ecx, 70h
        ja     SHORT loc_5_0931
        cld
        rep movsb
        mov    word ptr [ebp + 1ch], 1
        popfd
        clc
        ret
loc_5_0931:
        mov    word ptr [ebp + 1ch], 0
        popfd
        stc
        ret
loc_5_093A:
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_096F
        push   3
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_094A:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_096F
        mov    ecx, dword ptr [edi + 38h]
        mov    word ptr [ebp + 1ch], 0
        mov    eax, 2
        call   sub_5_0691
        jb     SHORT loc_5_0967
        ret
loc_5_0967:
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_5_0975
loc_5_096F:
        mov    word ptr [ebp + 1ch], 1
loc_5_0975:
        stc
        ret
loc_5_0977:
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_09AF
        push   3
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0987:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_09AF
        mov    ecx, dword ptr [edi + 34h]
        cmp    ecx, ebx
        je     SHORT loc_5_099D
        mov    word ptr [ebp + 1ch], 3
        jmp    SHORT loc_5_09B5
loc_5_099D:
        mov    eax, 2
        call   sub_5_060A
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
loc_5_09AF:
        mov    word ptr [ebp + 1ch], 1
loc_5_09B5:
        stc
        ret
loc_5_09B7:
        pushfd
        mov    ax, word ptr [ebp + 1ch]
        cmp    ax, 1
        jne    SHORT loc_5_0A2D
        mov    ecx, dword ptr [ebp + 18h]
        cli
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_0A2D
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_5_0A2D
loc_5_09D8:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 44h], ecx
        je     SHORT loc_5_09E9
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_5_0A2D
        jmp    SHORT loc_5_09D8
loc_5_09E9:
        popfd
        pushfd
        cmp    word ptr [edi + 8], -1
        je     SHORT loc_5_0A2D
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cmp    edi, -1
        je     SHORT loc_5_0A2D
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     SHORT loc_5_0A2D
        cmp    ecx, 1ch
        ja     SHORT loc_5_0A2D
        mov    word ptr [edi + 4], 0
        mov    ax, word ptr [esi + 8]
        mov    word ptr [edi + 6], ax
        mov    eax, dword ptr [esi + 44h]
        mov    dword ptr [edi + 0ch], eax
        mov    word ptr [ebp + 1ch], 1
        popfd
        clc
        ret
loc_5_0A2D:
        mov    word ptr [ebp + 1ch], 0
        popfd
        stc
        ret
loc_5_0A36:
        xchg   esi, eax
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        push   dword ptr [ebp + 3ch]
        push   dword ptr [ebp + 14h]
        mov    eax, dword ptr [esi + 4]
        mov    word ptr [ebp + 14h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 3ch], ax
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [ebp + 14h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 3ch], ax
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    dword ptr [esi + 8], eax
        pop    dword ptr [ebp + 14h]
        pop    dword ptr [ebp + 3ch]
        mov    eax, dword ptr [esi]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0A8E:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_0ABA
        lea    eax, [edi + 40h]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   eax
        push   dword ptr [esi]
        call   sub_5_0BED
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_5_0ABA
        mov    word ptr [ebp + 1ch], ax
        shr    eax, 10h
        mov    word ptr [ebp + 14h], ax
        clc
        ret
loc_5_0ABA:
        mov    word ptr [ebp + 1ch], 0
        mov    word ptr [ebp + 14h], 0
        stc
        ret
loc_5_0AC8:
        xchg   esi, eax
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        mov    eax, dword ptr [esi]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0ADE:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_0AF4
        lea    eax, [edi + 40h]
        push   dword ptr [esi + 4]
        push   eax
        call   sub_5_0D7D
        add    esp, 8
loc_5_0AF4:
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
sub_5_0AFC:
        push   ebp
        mov    ebp, esp
        sub    esp, 6ch
        mov    eax, 0ffffffffh
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   esi, esi
        je     NEAR PTR loc_5_0BE6
        lea    eax, [ebp - 6ch]
        push   eax
        call   sub_5_0E12
        add    esp, 4
        call   sub_5_0E00
        call   sub_5_0E4F
        mov    edi, dword ptr [eax + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_0E41
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_0E41
        add    esp, 4
        mov    ebx, dword ptr [ebp + 10h]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_0E41
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_0E41
        add    esp, 4
        mov    ebx, dword ptr [ebp + 14h]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_0E41
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_0E41
        add    esp, 4
        mov    ebx, dword ptr [ebp + 18h]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_0E41
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_0E41
        movzx  ecx, si
        shr    esi, 10h
        add    esp, 4
        movzx  eax, si
        push   ecx
        push   eax
        call   sub_5_0E32
        add    esp, 8
        call   sub_5_0E0C
        movzx  esi, word ptr [edi + 14h]
        shl    esi, 10h
        movzx  eax, word ptr [edi + 1ch]
        or     esi, eax
        call   sub_5_0E06
        lea    ecx, [ebp - 6ch]
        push   ecx
        call   sub_5_0E22
        add    esp, 4
        mov    eax, esi
loc_5_0BE6:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_0BED:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        push   ebp
        mov    ebp, dword ptr [esi]
        test   ebp, ebp
        jne    SHORT loc_5_0C13
        push   15h
        push   4
        call   sub_5_1734
        mov    ebp, eax
        test   ebp, ebp
        jne    SHORT loc_5_0C11
        xor    eax, eax
        jmp    NEAR PTR loc_5_0D78
loc_5_0C11:
        mov    dword ptr [esi], ebp
loc_5_0C13:
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 8
        jbe    SHORT loc_5_0C30
        mov    eax, dword ptr [esp + 1ch]
        mov    byte ptr [eax + 8], 0
loc_5_0C30:
        push   ebp
        call   sub_5_1724
        mov    ebx, eax
        test   ebx, ebx
        je     NEAR PTR loc_5_0D24
loc_5_0C40:
        mov    ecx, dword ptr [esp + 1ch]
        lea    edx, [ebx + 0ch]
loc_5_0C47:
        mov    al, byte ptr [ecx]
        cmp    al, byte ptr [edx]
        jne    SHORT loc_5_0C67
        or     al, al
        je     SHORT loc_5_0C63
        mov    al, byte ptr [ecx + 1]
        cmp    al, byte ptr [edx + 1]
        jne    SHORT loc_5_0C67
        add    ecx, 2
        add    edx, 2
        or     al, al
        jne    SHORT loc_5_0C47
loc_5_0C63:
        xor    eax, eax
        jmp    SHORT loc_5_0C6C
loc_5_0C67:
        sbb    eax, eax
        sbb    eax, -1
loc_5_0C6C:
        test   eax, eax
        je     SHORT loc_5_0C7D
        push   ebx
        push   ebp
        call   sub_5_1714
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_5_0C40
loc_5_0C7D:
        test   ebx, ebx
        je     NEAR PTR loc_5_0D24
        cmp    dword ptr [ebx + 4], 0
        je     SHORT loc_5_0C92
        xor    eax, eax
        jmp    NEAR PTR loc_5_0D78
loc_5_0C92:
        push   0
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_0AFC
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_5_0CBF
        push   ebx
        mov    dword ptr [ebx], 0
        push   esi
        call   sub_5_0D7D
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_5_0D78
loc_5_0CBF:
        mov    eax, dword ptr [esp + 20h]
        push   0
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebx + 4], ecx
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   ecx
        call   sub_5_0AFC
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_5_0CF6
        mov    dword ptr [ebx + 4], 0
        push   ebx
        push   esi
        call   sub_5_0D7D
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_5_0D78
loc_5_0CF6:
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx + 8]
        push   10000h
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_0AFC
        add    esp, 14h
        push   dword ptr [ebx]
        push   dword ptr [ebx + 8]
        push   10000h
        push   ebx
        push   dword ptr [ebx + 4]
        call   sub_5_0AFC
        add    esp, 14h
        jmp    SHORT loc_5_0D76
loc_5_0D24:
        push   ebp
        call   sub_5_1700
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_5_0D34
        xor    eax, eax
        jmp    SHORT loc_5_0D78
loc_5_0D34:
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 20h]
        mov    edi, dword ptr [esp + 1ch]
        mov    dword ptr [ebx + 8], eax
        mov    edx, dword ptr [ecx + 4]
        sub    eax, eax
        mov    ecx, 0ffffffffh
        mov    dword ptr [ebx], edx
        mov    dword ptr [ebx + 4], 0
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        lea    edi, [ebx + 0ch]
        rep movsd
        mov    ecx, eax
        push   ebx
        and    ecx, 3
        push   ebp
        rep movsb
        call   sub_5_16F0
loc_5_0D76:
        mov    eax, ebx
loc_5_0D78:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_0D7D:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        test   edi, edi
        je     SHORT loc_5_0DC9
        mov    eax, dword ptr [edi]
        test   eax, eax
        je     SHORT loc_5_0DA7
        push   0
        push   dword ptr [edi + 8]
        push   40000h
        push   edi
        push   eax
        call   sub_5_0AFC
        add    esp, 14h
        mov    dword ptr [edi], 0
loc_5_0DA7:
        mov    eax, dword ptr [edi + 4]
        test   eax, eax
        je     SHORT loc_5_0DC9
        push   0
        push   dword ptr [edi + 8]
        push   40000h
        push   edi
        push   eax
        call   sub_5_0AFC
        mov    dword ptr [edi + 4], 0
        add    esp, 14h
loc_5_0DC9:
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     SHORT loc_5_0DFA
        push   edi
        push   eax
        call   sub_5_1768
        push   edi
        push   dword ptr [esi]
        call   sub_5_1758
        push   dword ptr [esi]
        call   sub_5_1724
        test   eax, eax
        jne    SHORT loc_5_0DF4
        push   dword ptr [esi]
        call   sub_5_1748
loc_5_0DF4:
        mov    dword ptr [esi], 0
loc_5_0DFA:
        pop    edi
        pop    esi
        ret
        db  0CCh, 0CCh, 0CCh
sub_5_0E00:
        db  0CDh, 020h, 083h, 080h, 001h, 000h
sub_5_0E06:
        db  0CDh, 020h, 086h, 080h, 001h, 000h
sub_5_0E0C:
        db  0CDh, 020h, 085h, 080h, 001h, 000h
sub_5_0E12:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        pop    ebp
        ret
sub_5_0E22:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        pop    ebp
        ret
sub_5_0E32:
        movzx  ecx, word ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        db  0CDh, 020h, 048h, 080h, 001h, 000h
sub_5_0E41:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        pop    ebp
        ret
sub_5_0E4F:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
loc_5_0E5A:
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        push   edi
        mov    ecx, 10h
        db  0BFh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb
        je     SHORT loc_5_0E8F
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        mov    esi, eax
        repe cmpsb
        mov    eax, 80004002h
        jne    SHORT loc_5_0E9D
loc_5_0E8F:
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_0E9D:
        pop    edi
        pop    esi
        ret    0ch
loc_5_0EA2:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0ch]
        inc    eax
        mov    dword ptr [ecx + 0ch], eax
        ret    4
loc_5_0EB0:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0ch]
        dec    eax
        mov    dword ptr [ecx + 0ch], eax
        ret    4
loc_5_0EBE:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        push   0ffh
        db  068h, 000h, 000h, 000h, 000h
        lea    esi, [edi + 104h]
        push   esi
        mov    dword ptr [edi], 9
        call   sub_5_177C
        add    esp, 0ch
        call   sub_5_1450
        push   0ffh
        lea    ecx, [edi + 4]
        push   eax
        push   ecx
        call   sub_5_177C
        mov    edx, dword ptr [esp + 18h]
        add    esp, 0ch
        xor    ecx, ecx
        mov    dword ptr [edi + 210h], ecx
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [edi + 204h], eax
        mov    dword ptr [edi + 20ch], ecx
        mov    word ptr [edi + 208h], 33f5h
        mov    eax, 0b3h
        cmp    byte ptr [esi], cl
        je     SHORT loc_5_0F32
loc_5_0F27:
        movsx  ecx, byte ptr [esi]
        add    eax, ecx
        inc    esi
        cmp    byte ptr [esi], 0
        jne    SHORT loc_5_0F27
loc_5_0F32:
        mov    ecx, 0f7h
        sub    edx, edx
        div    ecx
        mov    word ptr [edi + 20ah], dx
        xor    eax, eax
        pop    edi
        pop    esi
        ret    8
loc_5_0F49:
        push   esi
        push   edi
        mov    edi, 8878000ah
        mov    esi, dword ptr [esp + 0ch]
        cmp    dword ptr [esi + 0ch], 0
        jne    SHORT loc_5_0F6E
        push   dword ptr [esi + 4]
        call   sub_5_1531
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_5_0F6E
        xor    edi, edi
        inc    dword ptr [esi + 0ch]
loc_5_0F6E:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    4
loc_5_0F75:
        mov    eax, dword ptr [esp + 8]
        xor    ecx, ecx
        mov    dword ptr [eax], 5
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [eax + 10h], ecx
        mov    dword ptr [eax + 14h], ecx
        mov    dword ptr [eax + 18h], ecx
        mov    dword ptr [eax + 1ch], ecx
        mov    dword ptr [eax + 20h], ecx
        mov    dword ptr [eax + 24h], ecx
        mov    dword ptr [eax + 28h], ecx
        mov    dword ptr [eax + 2ch], ecx
        mov    dword ptr [eax + 30h], ecx
        mov    dword ptr [eax + 34h], ecx
        mov    dword ptr [eax + 38h], ecx
        mov    dword ptr [eax + 3ch], ecx
        mov    dword ptr [eax + 40h], ecx
        mov    dword ptr [eax + 44h], ecx
        mov    dword ptr [eax + 48h], ecx
        mov    dword ptr [eax + 0ch], 1
        xor    eax, eax
        ret    8
loc_5_0FC0:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   ebp
        mov    edi, 8007000eh
        mov    esi, dword ptr [esp + 2ch]
        test   byte ptr [esp + 1ch], 0f0h
        mov    dword ptr [esi], ebx
        je     SHORT loc_5_0FE2
        mov    eax, 8878001eh
        jmp    NEAR PTR loc_5_10A8
loc_5_0FE2:
        mov    ebp, dword ptr [esp + 14h]
        cmp    dword ptr [ebp + 10h], 0
        jne    NEAR PTR loc_5_1087
        push   201h
        push   31h
        call   sub_5_178C
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_5_1080
        db  0C7h, 003h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 4], ebp
        push   dword ptr [ebp + 4]
        push   ebx
        call   sub_5_1573
        add    esp, 8
        mov    eax, dword ptr [ebx + 14h]
        cmp    eax, 1000h
        jbe    SHORT loc_5_108C
        cmp    eax, 0ffffh
        jae    SHORT loc_5_108C
        cmp    dword ptr [ebx + 0ch], 0
        je     SHORT loc_5_108C
        cmp    dword ptr [ebx + 10h], 0
        je     SHORT loc_5_108C
        cmp    dword ptr [ebx + 18h], 0
        je     SHORT loc_5_108C
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 2ch]
        push   ebx
        xor    edi, edi
        call   sub_5_1200
        mov    ecx, dword ptr [esp + 24h]
        add    esp, 0ch
        movzx  eax, word ptr [ecx + 0eh]
        mov    dword ptr [ebx + 24h], eax
        movzx  edx, word ptr [ecx + 2]
        mov    dword ptr [ebx + 28h], edx
        movzx  eax, word ptr [ecx + 2]
        movzx  edx, word ptr [ecx + 0eh]
        push   eax
        push   edx
        push   dword ptr [ecx + 4]
        push   1
        movzx  eax, word ptr [ebx + 2eh]
        push   eax
        call   sub_5_15BA
        add    esp, 14h
        jmp    SHORT loc_5_108C
loc_5_1080:
        mov    edi, 8007000eh
        jmp    SHORT loc_5_108C
loc_5_1087:
        mov    edi, 8878000ah
loc_5_108C:
        test   edi, edi
        je     SHORT loc_5_10A1
        test   ebx, ebx
        je     SHORT loc_5_10A6
        push   0
        push   ebx
        call   sub_5_1784
        add    esp, 8
        jmp    SHORT loc_5_10A6
loc_5_10A1:
        mov    dword ptr [esi], ebx
        inc    dword ptr [ebp + 10h]
loc_5_10A6:
        mov    eax, edi
loc_5_10A8:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    1ch
loc_5_10AF:
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], 0
        mov    eax, 80004001h
        ret    0ch
loc_5_10C1:
        mov    eax, 80004005h
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 0ch]
        dec    edx
        mov    dword ptr [ecx + 0ch], edx
        jne    SHORT loc_5_10E6
        cmp    dword ptr [ecx + 10h], 0
        jne    SHORT loc_5_10E6
        push   dword ptr [ecx + 4]
        call   sub_5_1514
        add    esp, 4
        xor    eax, eax
loc_5_10E6:
        ret    4
sub_5_10E9:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   201h
        mov    dword ptr [ebp - 4], ebx
        push   14h
        call   sub_5_178C
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_5_1168
        mov    edi, dword ptr [ebp + 8]
        push   1
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 14h]
        push   ecx
        mov    dword ptr [esi + 4], edi
        db  0C7h, 006h, 000h, 000h, 000h, 000h
        call   sub_5_17A8
        test   eax, eax
        jne    SHORT loc_5_1168
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 10h], ebx
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [ebp - 0ch], esi
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        db  0CDh, 020h, 001h, 000h, 07Eh, 035h
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], ebx
        jne    SHORT loc_5_1168
        push   esi
        mov    ebx, 1
        push   edi
        call   sub_5_0750
        add    esp, 8
loc_5_1168:
        test   ebx, ebx
        jne    SHORT loc_5_1191
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_5_1182
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 0bh]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_5_1794
loc_5_1182:
        test   esi, esi
        je     SHORT loc_5_1191
        push   0
        push   esi
        call   sub_5_1784
        add    esp, 8
loc_5_1191:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_1198:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_5_0733
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_5_11FA
        push   0
        push   edi
        call   sub_5_0750
        add    esp, 8
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [ebp - 8], 0
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        db  0CDh, 020h, 002h, 000h, 07Eh, 035h
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [esi + 8]
        push   dword ptr [eax + 0bh]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_5_1794
        push   0
        push   esi
        call   sub_5_1784
        add    esp, 8
loc_5_11FA:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_5_1200:
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 14h]
        cmp    dword ptr [edx], eax
        jbe    SHORT loc_5_1212
        mov    dword ptr [edx], eax
loc_5_1212:
        mov    eax, dword ptr [esp + 0ch]
        and    dword ptr [edx], 0fffch
        mov    ecx, dword ptr [esi + 10h]
        mov    dword ptr [eax], ecx
        mov    edx, dword ptr [edx]
        mov    dword ptr [esi + 1ch], edx
        inc    dword ptr [esi + 20h]
        mov    dword ptr [esi + 8], 0
        pop    esi
        ret
loc_5_1232:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 20h]
        dec    esi
        mov    dword ptr [eax + 20h], esi
        jne    SHORT loc_5_1257
        cmp    dword ptr [eax + 8], 0
        jne    SHORT loc_5_1257
        mov    ecx, dword ptr [eax + 4]
        push   0
        push   eax
        dec    dword ptr [ecx + 10h]
        call   sub_5_1784
        add    esp, 8
loc_5_1257:
        mov    eax, esi
        pop    esi
        ret    4
loc_5_125D:
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        push   edi
        mov    ecx, 10h
        db  0BFh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb
        je     SHORT loc_5_1292
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        mov    esi, eax
        repe cmpsb
        mov    eax, 80004002h
        jne    SHORT loc_5_12A0
loc_5_1292:
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_12A0:
        pop    edi
        pop    esi
        ret    0ch
loc_5_12A5:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 20h]
        inc    eax
        mov    dword ptr [ecx + 20h], eax
        ret    4
loc_5_12B3:
        mov    eax, 80004001h
        ret    20h
loc_5_12BB:
        mov    eax, 80004001h
        ret    14h
loc_5_12C3:
        mov    eax, dword ptr [esp + 8]
        test   eax, eax
        je     SHORT loc_5_1310
        cmp    word ptr [eax], 1
        jne    SHORT loc_5_1310
        mov    edx, dword ptr [eax + 4]
        cmp    edx, 5622h
        ja     SHORT loc_5_1310
        mov    cx, word ptr [eax + 0eh]
        cmp    cx, 8
        jne    SHORT loc_5_1310
        mov    ax, word ptr [eax + 2]
        cmp    ax, 1
        jne    SHORT loc_5_1310
        movzx  eax, ax
        movzx  ecx, cx
        push   eax
        push   ecx
        mov    eax, dword ptr [esp + 0ch]
        push   edx
        movzx  ecx, word ptr [eax + 2eh]
        push   1
        push   ecx
        call   sub_5_15BA
        add    esp, 14h
        xor    eax, eax
        jmp    SHORT loc_5_1315
loc_5_1310:
        mov    eax, 88780064h
loc_5_1315:
        ret    8
loc_5_1318:
        mov    eax, dword ptr [esp + 8]
        test   eax, eax
        je     SHORT loc_5_133D
        cmp    eax, 0afc8h
        jae    SHORT loc_5_133D
        mov    ecx, dword ptr [esp + 4]
        push   eax
        movzx  edx, word ptr [ecx + 2eh]
        push   edx
        call   sub_5_15EC
        add    esp, 8
        xor    eax, eax
        jmp    SHORT loc_5_1342
loc_5_133D:
        mov    eax, 88780064h
loc_5_1342:
        ret    8
loc_5_1345:
        mov    eax, 80004001h
        ret    8
loc_5_134D:
        mov    eax, 80004001h
        ret    8
loc_5_1355:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 8], 1
        jne    SHORT loc_5_1387
        push   dword ptr [esi + 18h]
        call   sub_5_145E
        add    esp, 4
        inc    eax
        cmp    byte ptr [esi + 30h], 3
        jbe    SHORT loc_5_1374
        add    eax, eax
loc_5_1374:
        mov    ecx, dword ptr [esi + 1ch]
        mov    edx, dword ptr [esp + 10h]
        sub    ecx, eax
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx], ecx
        jmp    SHORT loc_5_139B
loc_5_1387:
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx], 0
loc_5_139B:
        xor    eax, eax
        pop    esi
        ret    0ch
loc_5_13A1:
        push   esi
        mov    eax, 80004005h
        mov    esi, dword ptr [esp + 8]
        push   edi
        cmp    dword ptr [esi + 8], 0
        jne    SHORT loc_5_1416
        push   dword ptr [esi + 18h]
        movzx  edi, word ptr [esi + 2eh]
        call   sub_5_1479
        add    esp, 4
        push   dword ptr [esi + 1ch]
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi + 18h]
        call   sub_5_149D
        add    esp, 0ch
        push   dword ptr [esi + 18h]
        call   sub_5_14BE
        add    esp, 4
        push   dword ptr [esi + 18h]
        call   sub_5_1487
        mov    ax, word ptr [esi + 1ch]
        add    esp, 4
        cmp    dword ptr [esi + 24h], 10h
        jne    SHORT loc_5_13F5
        shr    ax, 1
loc_5_13F5:
        cmp    dword ptr [esi + 28h], 2
        jne    SHORT loc_5_13FE
        shr    ax, 1
loc_5_13FE:
        dec    ax
        movzx  ecx, ax
        push   ecx
        push   edi
        call   sub_5_1627
        add    esp, 8
        xor    eax, eax
        mov    dword ptr [esi + 8], 1
loc_5_1416:
        pop    edi
        pop    esi
        ret    10h
loc_5_141B:
        push   esi
        mov    eax, 80004005h
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 8], 0
        je     SHORT loc_5_144C
        movzx  eax, word ptr [esi + 2eh]
        push   eax
        call   sub_5_1672
        add    esp, 4
        push   dword ptr [esi + 18h]
        call   sub_5_1479
        add    esp, 4
        xor    eax, eax
        mov    dword ptr [esi + 8], 0
loc_5_144C:
        pop    esi
        ret    4
sub_5_1450:
        db  00Fh, 0B7h, 005h, 010h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        ret
sub_5_145E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 01Ch, 000h, 004h, 000h
        movzx  eax, cx
        pop    ecx
        pop    ebx
        pop    ebp
        ret
sub_5_1479:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 018h, 000h, 004h, 000h
        pop    ebp
        ret
sub_5_1487:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 019h, 000h, 004h, 000h
        pop    ebx
        pop    ebp
        ret
sub_5_149D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        xor    bx, bx
        mov    bh, 1
        xor    esi, esi
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 003h, 000h, 004h, 000h
        pop    ecx
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_5_14BE:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edx
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        xor    dx, dx
        or     dl, 8
        or     dl, 10h
        or     dl, 40h
        db  0CDh, 020h, 006h, 000h, 004h, 000h
        pop    edx
        pop    ebx
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    ah, byte ptr [ebp + 0ch]
        mov    al, byte ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_14F4:
        xor    eax, eax
        jae    SHORT loc_5_14FD
        mov    eax, 1
loc_5_14FD:
        pop    edx
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    ah, byte ptr [ebp + 0ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_150F:
        xor    ah, ah
        pop    edx
        pop    ebp
        ret
sub_5_1514:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1524:
        add    esp, 8
        mov    word ptr [edi + 6eh], 0
        pop    ebx
        pop    edi
        pop    ebp
        ret
sub_5_1531:
        push   ebp
        mov    ebp, esp
        push   edi
        push   edx
        push   ebx
        mov    edx, 0ffffh
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1547:
        add    esp, 8
        mov    eax, dword ptr [edi + 2ch]
        or     eax, eax
        je     SHORT loc_5_155D
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        cmp    eax, ebx
        je     SHORT loc_5_155D
        jmp    SHORT loc_5_156C
loc_5_155D:
        mov    word ptr [edi + 6eh], 1
        movzx  edx, word ptr [edi + 6]
        mov    edx, 0
loc_5_156C:
        mov    eax, edx
        pop    ebx
        pop    edx
        pop    edi
        pop    ebp
        ret
sub_5_1573:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1586:
        add    esp, 8
        mov    ax, word ptr [edi + 6]
        mov    word ptr [esi + 2eh], ax
        mov    ah, byte ptr [edi + 0bh]
        mov    byte ptr [esi + 30h], ah
        mov    eax, dword ptr [edi + 18h]
        mov    dword ptr [esi + 0ch], eax
        mov    eax, dword ptr [edi + 1ch]
        mov    dword ptr [esi + 10h], eax
        mov    eax, dword ptr [edi + 20h]
        mov    dword ptr [esi + 14h], eax
        mov    eax, dword ptr [edi + 3ch]
        mov    dword ptr [esi + 18h], eax
        xor    ax, ax
        mov    word ptr [esi + 2ch], ax
        pop    esi
        pop    edi
        pop    ebp
        ret
sub_5_15BA:
        push   ebp
        mov    ebp, esp
        push   eax
        push   edi
        db  066h, 083h, 025h, 000h, 000h, 000h, 000h, 0F9h
        cmp    word ptr [ebp + 14h], 10h
        jne    SHORT loc_5_15D6
        db  066h, 083h, 00Dh, 000h, 000h, 000h, 000h, 002h
loc_5_15D6:
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 8]
        call   sub_5_15EC
        add    esp, 8
        clc
        jmp    SHORT loc_5_15E8
        db  0F9h
loc_5_15E8:
        pop    edi
        pop    eax
        pop    ebp
        ret
sub_5_15EC:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ebx
        push   edx
        push   edi
        mov    edx, dword ptr [ebp + 8]
        push   edx
        mov    eax, 0f4240h
        movzx  ebx, word ptr [ebp + 0ch]
        xor    edx, edx
        idiv   ebx
        shr    ebx, 1
        cmp    edx, ebx
        jle    SHORT loc_5_160B
        inc    eax
loc_5_160B:
        neg    al
        pop    edx
        push   eax
        mov    al, 40h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1616:
        pop    eax
        jb     SHORT loc_5_1620
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_161E:
        jmp    SHORT loc_5_1621
loc_5_1620:
        stc
loc_5_1621:
        pop    edi
        pop    edx
        pop    ebx
        pop    eax
        pop    ebp
        ret
sub_5_1627:
        push   ebp
        mov    ebp, esp
        push   eax
        push   edx
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    al, 0d1h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1637:
        jb     SHORT loc_5_166C
        mov    al, 48h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1640:
        jb     SHORT loc_5_166C
        mov    eax, dword ptr [ebp + 0ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_164A:
        jb     SHORT loc_5_166C
        mov    al, ah
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1653:
        jb     SHORT loc_5_166C
        mov    al, 1ch
        db  066h, 0F7h, 005h, 000h, 000h, 000h, 000h, 002h, 000h
        je     SHORT loc_5_1665
        or     eax, 1
loc_5_1665:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_166A:
        jmp    SHORT loc_5_166D
loc_5_166C:
        stc
loc_5_166D:
        pop    edi
        pop    edx
        pop    eax
        pop    ebp
        ret
sub_5_1672:
        push   ebp
        mov    ebp, esp
        push   eax
        push   edx
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    al, 0d3h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1682:
        jb     SHORT loc_5_168D
        mov    al, 0d0h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_168B:
        jmp    SHORT loc_5_168E
loc_5_168D:
        stc
loc_5_168E:
        pop    edi
        pop    edx
        pop    eax
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_5_1627
        add    esp, 0ch
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_5_1672
        add    esp, 8
        pop    ebp
        ret
        db  050h, 052h, 0C1h, 0E8h, 008h, 024h, 0F0h, 0C1h, 0EBh, 00Ch, 00Ah, 0C3h, 083h, 0C2h, 002h, 050h
        db  0B0h, 014h, 0EEh, 058h, 066h, 042h, 0EEh, 0F8h, 05Ah, 058h, 0C3h, 0CCh
sub_5_16D8:
        db  0CDh, 020h, 074h, 081h, 001h, 000h
        int3
        int3
sub_5_16E0:
        db  0CDh, 020h, 044h, 080h, 033h, 000h
        int3
        int3
sub_5_16E8:
        db  0CDh, 020h, 03Bh, 080h, 033h, 000h
        int3
        int3
sub_5_16F0:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_1700:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        mov    esi, ecx
        cmc
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
        db  0CCh
sub_5_1714:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_1724:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_1734:
        pop    edx
        pop    eax
        pop    ecx
        push   esi
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        cmc
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
sub_5_1748:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_1758:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_1768:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        mov    esi, ecx
        sbb    eax, eax
        inc    eax
        jmp    edx
        db  0CCh, 0CCh
sub_5_177C:
        db  0CDh, 020h, 07Bh, 081h, 001h, 000h
        int3
        int3
sub_5_1784:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        int3
        int3
sub_5_178C:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        int3
        int3
sub_5_1794:
        pop    ecx
        pop    eax
        pop    edx
        push   ebx
        mov    ebx, eax
        db  0CDh, 020h, 002h, 000h, 027h, 000h
        sbb    edx, edx
        and    eax, edx
        pop    ebx
        jmp    ecx
        db  0CCh
sub_5_17A8:
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 0ch]
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        jb     SHORT loc_5_17C6
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_5_17C6:
        ret    10h
_PTEXT ends
        end
