; es688_pcode_05.asm -- ES688 object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  083h, 07Ch, 024h, 008h, 000h, 08Ah, 044h, 024h, 004h, 074h, 00Ah, 03Ch, 030h, 07Ch, 025h, 03Ch
        db  039h, 07Fh, 021h, 0EBh, 018h, 03Ch, 030h, 07Ch, 004h, 03Ch, 039h, 07Eh, 010h, 03Ch, 061h, 07Ch
        db  004h, 03Ch, 066h, 07Eh, 008h, 03Ch, 041h, 07Ch, 00Bh, 03Ch, 046h, 07Fh, 007h, 0B8h, 001h, 000h
        db  000h, 000h, 0EBh, 002h, 033h, 0C0h, 0C3h
sub_5_0037:
        mov    al, byte ptr [esp + 4]
        cmp    al, 30h
        jl     SHORT loc_5_004D
        cmp    al, 39h
        jg     SHORT loc_5_004D
        movsx  ax, al
        sub    ax, 30h
        jmp    SHORT loc_5_0067
loc_5_004D:
        cmp    al, 61h
        jl     SHORT loc_5_005F
        cmp    al, 66h
        jg     SHORT loc_5_005F
        movsx  ax, al
        sub    ax, 57h
        jmp    SHORT loc_5_0067
loc_5_005F:
        movsx  ax, al
        sub    ax, 37h
loc_5_0067:
        ret
sub_5_0068:
        sub    esp, 0ch
        xor    eax, eax
        mov    dword ptr [esp + 8], eax
        push   ebx
        mov    dword ptr [esp + 8], eax
        push   esi
        push   edi
        mov    esi, 1
        mov    edi, dword ptr [esp + 1ch]
        push   ebp
        cmp    byte ptr [edi], al
        je     NEAR PTR loc_5_012A
        mov    eax, dword ptr [esp + 24h]
        mov    ebx, dword ptr [esp + 2ch]
        mov    dword ptr [esp + 10h], eax
loc_5_0096:
        push   ebx
        mov    al, byte ptr [edi]
        push   eax
        call   0
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_5_00AE
        cmp    byte ptr [edi], 2ch
        je     SHORT loc_5_00B3
        inc    edi
        jmp    SHORT loc_5_0121
loc_5_00AE:
        cmp    byte ptr [edi], 2ch
        jne    SHORT loc_5_00C5
loc_5_00B3:
        inc    edi
        mov    eax, esi
        mov    esi, 1
        test   eax, eax
        je     SHORT loc_5_0121
        mov    bp, 0fffeh
        jmp    SHORT loc_5_010A
loc_5_00C5:
        xor    bp, bp
        cmp    ebx, 1
        sbb    esi, esi
        and    esi, 6
        add    si, 0ah
loc_5_00D4:
        mov    eax, edi
        inc    edi
        imul   bp, si
        mov    al, byte ptr [eax]
        push   eax
        call   sub_5_0037
        add    bp, ax
        add    esp, 4
        cmp    bp, word ptr [esp + 28h]
        ja     SHORT loc_5_015C
        push   ebx
        mov    al, byte ptr [edi]
        push   eax
        call   0
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_5_00D4
        mov    dword ptr [esp + 14h], 1
        xor    esi, esi
loc_5_010A:
        mov    eax, dword ptr [esp + 10h]
        cmp    word ptr [eax], 0ffffh
        je     SHORT loc_5_0163
        add    dword ptr [esp + 10h], 2
        mov    word ptr [eax], bp
        inc    dword ptr [esp + 18h]
loc_5_0121:
        cmp    byte ptr [edi], 0
        jne    NEAR PTR loc_5_0096
loc_5_012A:
        mov    eax, dword ptr [esp + 24h]
        mov    ecx, dword ptr [esp + 18h]
        lea    eax, [eax + ecx*2]
        cmp    word ptr [eax], 0ffffh
        je     SHORT loc_5_014B
loc_5_013C:
        mov    word ptr [eax], 0fffeh
        add    eax, 2
        cmp    word ptr [eax], 0ffffh
        jne    SHORT loc_5_013C
loc_5_014B:
        cmp    dword ptr [esp + 14h], 1
        sbb    eax, eax
        neg    eax
loc_5_0154:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_5_015C:
        mov    eax, 1
        jmp    SHORT loc_5_0154
loc_5_0163:
        mov    eax, 1
        jmp    SHORT loc_5_0154
sub_5_016A:
        push   ebp
        mov    ebp, esp
        sub    esp, 250h
        push   ebx
        lea    eax, [ebp - 150h]
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        push   edi
        push   0
        push   100h
        push   eax
        push   0
        push   ebx
        call   sub_5_2014
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_5_03D3
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        mov    esi, edi
        mov    ecx, 0ffffffffh
        lea    edi, [ebp - 150h]
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 150h]
        push   eax
        push   ecx
        push   80000002h
        call   sub_5_2004
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_01F1
        xor    eax, eax
        jmp    NEAR PTR loc_5_03D3
loc_5_01F1:
        push   1
        lea    eax, [ebp - 250h]
        push   100h
        push   eax
        push   0
        push   ebx
        call   sub_5_2014
        add    esp, 14h
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_5_0218
        push   dword ptr [ebp - 8]
        jmp    NEAR PTR loc_5_03C9
loc_5_0218:
        lea    eax, [ebp - 10h]
        lea    ecx, [ebp - 250h]
        push   eax
        push   ecx
        push   80000002h
        call   sub_5_2004
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_023E
        mov    esi, 1dh
        jmp    NEAR PTR loc_5_039E
loc_5_023E:
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 50h]
        mov    dword ptr [ebp - 0ch], 40h
        push   eax
        push   ecx
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        call   sub_5_200C
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_5_026F
        mov    esi, 1dh
        jmp    NEAR PTR loc_5_039E
loc_5_026F:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_5_02D4
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 10h]
        call   sub_5_2004
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_02A1
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 10h]
        call   sub_5_1FFC
        add    esp, 0ch
loc_5_02A1:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_5_0305
        push   dword ptr [ebp - 0ch]
        lea    ecx, [ebp - 50h]
        push   ecx
        push   1
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        call   sub_5_1FF4
        add    esp, 18h
        push   dword ptr [ebp - 4]
        call   sub_5_1FE4
        mov    dword ptr [ebp - 4], 0
        add    esp, 4
loc_5_02D4:
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 50h]
        mov    dword ptr [ebp - 0ch], 40h
        push   eax
        push   ecx
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        call   sub_5_200C
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_5_030F
        mov    esi, 1dh
        jmp    NEAR PTR loc_5_039E
loc_5_0305:
        mov    esi, 1dh
        jmp    NEAR PTR loc_5_039E
loc_5_030F:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_5_0378
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 10h]
        call   sub_5_2004
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_0341
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 10h]
        call   sub_5_1FFC
        add    esp, 0ch
loc_5_0341:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_5_03DA
        push   dword ptr [ebp - 0ch]
        lea    ecx, [ebp - 50h]
        push   ecx
        push   1
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        call   sub_5_1FF4
        add    esp, 18h
        push   dword ptr [ebp - 4]
        call   sub_5_1FE4
        mov    dword ptr [ebp - 4], 0
        add    esp, 4
loc_5_0378:
        push   dword ptr [ebp - 8]
        call   sub_5_1FE4
        add    esp, 4
        lea    eax, [ebp - 150h]
        mov    dword ptr [ebp - 8], 0
        push   eax
        push   80000002h
        call   sub_5_1FEC
        add    esp, 8
loc_5_039E:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_5_03AF
        push   dword ptr [ebp - 8]
        call   sub_5_1FE4
        add    esp, 4
loc_5_03AF:
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_5_03C0
        push   dword ptr [ebp - 10h]
        call   sub_5_1FE4
        add    esp, 4
loc_5_03C0:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_5_03D1
        push   dword ptr [ebp - 4]
loc_5_03C9:
        call   sub_5_1FE4
        add    esp, 4
loc_5_03D1:
        mov    eax, esi
loc_5_03D3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_03DA:
        mov    esi, 1dh
        jmp    SHORT loc_5_039E
sub_5_03E1:
        push   ebp
        mov    ebp, esp
        sub    esp, 10ch
        test   byte ptr [ebp + 14h], 50h
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_5_03FA
        xor    eax, eax
        jmp    NEAR PTR loc_5_05A5
loc_5_03FA:
        push   1
        lea    eax, [ebp - 10ch]
        push   100h
        push   eax
        push   0
        push   dword ptr [ebp + 8]
        call   sub_5_2014
        add    esp, 14h
        mov    esi, eax
        test   esi, esi
        jne    NEAR PTR loc_5_05A3
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 10ch]
        push   eax
        push   ecx
        push   80000002h
        call   sub_5_2004
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_0445
        mov    eax, 13h
        jmp    NEAR PTR loc_5_05A5
loc_5_0445:
        lea    eax, [ebp - 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 0ch]
        call   sub_5_2004
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_0472
        push   dword ptr [ebp - 0ch]
        call   sub_5_1FE4
        add    esp, 4
        mov    eax, 13h
        jmp    NEAR PTR loc_5_05A5
loc_5_0472:
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ebx, 100h
loc_5_047C:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 10ch]
        push   eax
        mov    dword ptr [ebp - 4], ebx
        push   ecx
        push   0
        push   0
        push   dword ptr [edi]
        push   dword ptr [ebp - 8]
        call   sub_5_200C
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_5_04A5
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_5_04BA
loc_5_04A5:
        add    edi, 4
        db  081h, 0FFh, 00Ch, 000h, 000h, 000h
        jb     SHORT loc_5_047C
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_5_0588
loc_5_04BA:
        push   1
        mov    edi, dword ptr [ebp + 0ch]
        push   0fh
        lea    eax, [ebp - 10ch]
        push   edi
        push   eax
        call   sub_5_0068
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_5_0588
        cmp    word ptr [edi], 0ffffh
        je     SHORT loc_5_04F5
loc_5_04E0:
        cmp    word ptr [edi], 2
        jne    SHORT loc_5_04EB
        mov    word ptr [edi], 9
loc_5_04EB:
        add    edi, 2
        cmp    word ptr [edi], 0ffffh
        jne    SHORT loc_5_04E0
loc_5_04F5:
        push   dword ptr [ebp - 8]
        call   sub_5_1FE4
        add    esp, 4
        lea    eax, [ebp - 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 0ch]
        call   sub_5_2004
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_052A
        push   dword ptr [ebp - 0ch]
        call   sub_5_1FE4
        add    esp, 4
        mov    eax, 13h
        jmp    SHORT loc_5_05A5
loc_5_052A:
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ebx, 100h
loc_5_0534:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 10ch]
        push   eax
        mov    dword ptr [ebp - 4], ebx
        push   ecx
        push   0
        push   0
        push   dword ptr [edi]
        push   dword ptr [ebp - 8]
        call   sub_5_200C
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_5_055D
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_5_056E
loc_5_055D:
        add    edi, 4
        db  081h, 0FFh, 00Ch, 000h, 000h, 000h
        jb     SHORT loc_5_0534
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_5_0588
loc_5_056E:
        push   1
        lea    ecx, [ebp - 10ch]
        push   3
        push   dword ptr [ebp + 10h]
        push   ecx
        call   sub_5_0068
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_5_058D
loc_5_0588:
        mov    esi, 13h
loc_5_058D:
        push   dword ptr [ebp - 8]
        call   sub_5_1FE4
        add    esp, 4
        push   dword ptr [ebp - 0ch]
        call   sub_5_1FE4
        add    esp, 4
loc_5_05A3:
        mov    eax, esi
loc_5_05A5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_05AC:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        db  0CDh, 020h, 088h, 001h, 001h, 000h
        test   eax, eax
        jne    SHORT loc_5_05F9
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
loc_5_05F9:
        mov    esp, ebp
        pop    ebp
        ret
sub_5_05FD:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 130h
        test   eax, eax
        push   ebx
        push   esi
        push   edi
        je     NEAR PTR loc_5_093D
        cmp    eax, 1
        je     SHORT loc_5_0651
        cmp    eax, 2
        je     NEAR PTR loc_5_0928
        cmp    eax, 3
        je     NEAR PTR loc_5_089A
        cmp    eax, 4
        je     NEAR PTR loc_5_08E6
        cmp    eax, 8
        je     NEAR PTR loc_5_08EE
        cmp    eax, 0dh
        je     NEAR PTR loc_5_0928
        mov    eax, 1
        jmp    NEAR PTR loc_5_093F
loc_5_0651:
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
        call   sub_5_202C
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_5_093F
        movzx  eax, word ptr [ebp - 0d4h]
        mov    di, 0ffffh
        cmp    eax, 1
        je     SHORT loc_5_06BB
        cmp    eax, 2
        je     SHORT loc_5_06DC
        mov    eax, 13h
        jmp    NEAR PTR loc_5_093F
loc_5_06BB:
        cmp    word ptr [ebp - 0aah], 10h
        je     SHORT loc_5_06CF
        mov    eax, 13h
        jmp    NEAR PTR loc_5_093F
loc_5_06CF:
        mov    ax, word ptr [ebp - 0d2h]
        mov    word ptr [ebp - 2], ax
        jmp    SHORT loc_5_0718
loc_5_06DC:
        cmp    word ptr [ebp - 0aah], 10h
        je     SHORT loc_5_06F0
        mov    eax, 13h
        jmp    NEAR PTR loc_5_093F
loc_5_06F0:
        movzx  ecx, word ptr [ebp - 0a8h]
        mov    ax, word ptr [ebp - 0d2h]
        cmp    ecx, 4
        mov    word ptr [ebp - 2], ax
        je     SHORT loc_5_0711
        mov    eax, 13h
        jmp    NEAR PTR loc_5_093F
loc_5_0711:
        mov    di, word ptr [ebp - 0d0h]
loc_5_0718:
        cmp    word ptr [ebp - 82h], 1
        jb     NEAR PTR loc_5_0890
        cmp    word ptr [ebp - 72h], 1
        jb     NEAR PTR loc_5_0890
        movzx  ax, byte ptr [ebp - 80h]
        movzx  cx, byte ptr [ebp - 70h]
        mov    word ptr [ebp - 4], ax
        cmp    word ptr [ebp - 2], 0ffffh
        mov    word ptr [ebp - 6], cx
        jne    SHORT loc_5_0755
        mov    eax, 13h
        jmp    NEAR PTR loc_5_093F
loc_5_0755:
        cmp    di, 0ffffh
        je     SHORT loc_5_0760
        mov    di, word ptr [ebp - 2]
loc_5_0760:
        xor    ebx, ebx
        lea    eax, [ebp - 24h]
        push   ebx
        lea    ecx, [ebp - 0ch]
        push   ebx
        mov    dword ptr [ebp - 24h], ebx
        push   eax
        push   ecx
        push   esi
        call   sub_5_2024
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_5_077F
        mov    dword ptr [ebp - 0ch], ebx
loc_5_077F:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    eax, 8
        je     SHORT loc_5_0793
        cmp    eax, 10h
        je     SHORT loc_5_079A
        cmp    eax, 40h
        je     SHORT loc_5_07A1
        jmp    SHORT loc_5_07A6
loc_5_0793:
        mov    ebx, 20h
        jmp    SHORT loc_5_07A6
loc_5_079A:
        mov    ebx, 10h
        jmp    SHORT loc_5_07A6
loc_5_07A1:
        mov    ebx, 40h
loc_5_07A6:
        push   esi
        call   sub_5_016A
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_5_093F
        push   ebx
        lea    eax, [ebp - 2ch]
        push   eax
        lea    ecx, [ebp - 38h]
        push   ecx
        push   esi
        call   sub_5_03E1
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_5_093F
        push   ebx
        lea    eax, [ebp - 2ch]
        push   20h
        lea    ecx, [ebp - 38h]
        push   eax
        push   ecx
        push   dword ptr [ebp - 6]
        push   dword ptr [ebp - 4]
        push   edi
        push   dword ptr [ebp - 2]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_07EC:
        add    esp, 24h
        mov    ebx, eax
        test   ebx, ebx
        jne    NEAR PTR loc_5_087C
        sub    di, 0ffffh
        mov    dword ptr [ebp - 18h], esi
        db  0C7h, 045h, 0E4h, 000h, 000h, 000h, 000h
        cmp    di, 1
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
        test   ebx, ebx
        jne    SHORT loc_5_0889
        cmp    word ptr [ebp - 4], 9
        mov    eax, 2
        je     SHORT loc_5_083E
        movzx  eax, word ptr [ebp - 4]
loc_5_083E:
        movzx  ecx, word ptr [ebp - 6]
        movzx  edx, word ptr [ebp - 2]
        push   ecx
        push   eax
        push   edx
        lea    ecx, [ebp - 58h]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_5_201C
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
loc_5_087C:
        test   ebx, ebx
        jne    SHORT loc_5_0889
        push   esi
        call   sub_5_17BD
        add    esp, 4
loc_5_0889:
        mov    eax, ebx
        jmp    NEAR PTR loc_5_093F
loc_5_0890:
        mov    eax, 13h
        jmp    NEAR PTR loc_5_093F
loc_5_089A:
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        je     SHORT loc_5_08B0
        cmp    eax, 1
        je     SHORT loc_5_08CB
        mov    eax, 1
        jmp    NEAR PTR loc_5_093F
loc_5_08B0:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_08B8:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 13h
        jmp    SHORT loc_5_093F
loc_5_08CB:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_08D3:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 13h
        jmp    SHORT loc_5_093F
loc_5_08E6:
        cmp    dword ptr [ebp + 0ch], 1
        jne    SHORT loc_5_0928
        jmp    SHORT loc_5_093D
loc_5_08EE:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 7fffffffh
        cmp    eax, 4
        jb     SHORT loc_5_0921
        cmp    eax, 5
        jbe    SHORT loc_5_090C
        cmp    eax, 6
        jb     SHORT loc_5_0921
        cmp    eax, 8
        jbe    SHORT loc_5_0916
        jmp    SHORT loc_5_0921
loc_5_090C:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0914:
        jmp    SHORT loc_5_091E
loc_5_0916:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_091E:
        add    esp, 4
loc_5_0921:
        mov    eax, 1
        jmp    SHORT loc_5_093F
loc_5_0928:
        mov    esi, dword ptr [ebp + 10h]
        push   esi
        call   sub_5_186C
        add    esp, 4
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_093A:
        add    esp, 4
loc_5_093D:
        xor    eax, eax
loc_5_093F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  0CCh, 0CCh
sub_5_0948:
        push   eax
        push   edx
        push   ebx
        push   ecx
        movzx  edx, word ptr [edi + 8]
        mov    ebx, 105h
        mov    al, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_095C:
        mov    ebx, 4
        mov    al, 60h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0968:
        mov    ebx, 104h
        mov    al, 3fh
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0974:
        mov    ebx, 8
        xor    al, al
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0980:
        mov    ebx, 0bdh
        mov    al, 0c0h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_098C:
        mov    ecx, 15h
        mov    ebx, 40h
        mov    al, 3fh
loc_5_0998:
        xor    bh, bh
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_099F:
        mov    bh, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_09A6:
        inc    bl
        loop   loc_5_0998
        mov    ecx, 8
        mov    ebx, 0b0h
        xor    eax, eax
loc_5_09B6:
        xor    bh, bh
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_09BD:
        mov    bh, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_09C4:
        inc    bl
        loop   loc_5_09B6
        mov    ebx, 104h
        xor    eax, eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_09D4:
        mov    ebx, 105h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_09DE:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_09E3:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_09E8:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_09ED:
        pop    ecx
        pop    ebx
        pop    edx
        pop    eax
        ret
        db  08Bh, 0C0h
loc_5_09F4:
        push   eax
        mov    eax, dword ptr [esi + 2ch]
        cmp    eax, ebx
        je     SHORT loc_5_0A14
        or     eax, eax
        jne    SHORT loc_5_0A27
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 1
        call   sub_5_0D64
        pop    edx
        pop    ecx
        pop    ebx
        jb     SHORT loc_5_0A27
loc_5_0A14:
        pop    eax
        cmp    ecx, 4
        jbe    SHORT loc_5_0A20
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_5_0A20:
        je     SHORT loc_5_0A25
        in     al, dx
        jmp    SHORT loc_5_0A26
loc_5_0A25:
        out    dx, al
loc_5_0A26:
        ret
loc_5_0A27:
        mov    eax, 1
        mov    edi, esi
        call   sub_5_0BC2
        pop    eax
        xor    eax, eax
        dec    eax
        ret
loc_5_0A38:
        push   eax
        mov    eax, dword ptr [esi + 34h]
        cmp    eax, ebx
        je     SHORT loc_5_0A5D
        or     eax, eax
        jne    SHORT loc_5_0A70
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 2
        call   sub_5_0D64
        pop    edx
        pop    ecx
        pop    ebx
        jb     SHORT loc_5_0A70
        call   sub_5_0948
loc_5_0A5D:
        pop    eax
        cmp    ecx, 4
        jbe    SHORT loc_5_0A69
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_5_0A69:
        je     SHORT loc_5_0A6E
        in     al, dx
        jmp    SHORT loc_5_0A6F
loc_5_0A6E:
        out    dx, al
loc_5_0A6F:
        ret
loc_5_0A70:
        mov    eax, 2
        mov    edi, esi
        call   sub_5_0BC2
        pop    eax
        xor    eax, eax
        dec    eax
        ret
sub_5_0A81:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 0fh
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_0A8F:
        cmp    ecx, 3
        jbe    SHORT loc_5_0AA4
        cmp    ecx, 8
        je     SHORT loc_5_0AA4
        cmp    ecx, 9
        je     SHORT loc_5_0AA4
        db  0CDh, 020h, 098h, 000h, 001h, 000h
loc_5_0AA4:
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_0AAF
        inc    cx
        jmp    SHORT loc_5_0A8F
loc_5_0AAF:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_0AB5:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 0fh
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_0AC3:
        cmp    ecx, 3
        jbe    SHORT loc_5_0AD8
        cmp    ecx, 8
        je     SHORT loc_5_0AD8
        cmp    ecx, 9
        je     SHORT loc_5_0AD8
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
loc_5_0AD8:
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_0AE3
        inc    cx
        jmp    SHORT loc_5_0AC3
loc_5_0AE3:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_0AE9:
        push   eax
        push   ebx
        push   edx
        movzx  edx, word ptr [edi + 8]
        mov    ebx, 4
        mov    eax, 60h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0AFF:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0B04:
        pop    edx
        pop    ebx
        pop    eax
        ret
sub_5_0B08:
        push   esi
        push   eax
        push   ecx
        movzx  edx, word ptr [edi + 6]
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        inc    dx
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        inc    dx
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        inc    dx
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        movzx  edx, word ptr [edi + 6]
        add    dx, 8
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        inc    dx
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        mov    si, 3
        xor    ecx, ecx
        mov    edx, 388h
loc_5_0B4E:
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_0B5F
        inc    cx
        jmp    SHORT loc_5_0B4E
loc_5_0B5F:
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_0B64:
        push   esi
        push   eax
        push   ecx
        push   edx
        movzx  edx, word ptr [edi + 6]
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        inc    dx
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        inc    dx
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        inc    dx
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        movzx  edx, word ptr [edi + 6]
        add    dx, 8
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        inc    dx
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        mov    si, 3
        xor    ecx, ecx
        mov    edx, 388h
loc_5_0BAB:
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_0BBC
        inc    cx
        jmp    SHORT loc_5_0BAB
loc_5_0BBC:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_0BC2:
        test   word ptr [edi + 4], 1
        jne    SHORT loc_5_0C17
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0BCF:
        jb     SHORT loc_5_0C17
        cmp    eax, 2
        je     SHORT loc_5_0BF0
        test   dword ptr [esi], 1
        jne    SHORT loc_5_0C17
        or     dword ptr [esi], 1
        db  00Fh, 0B7h, 00Dh, 00Ch, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        jmp    SHORT loc_5_0C08
loc_5_0BF0:
        test   dword ptr [esi], 2
        jne    SHORT loc_5_0C17
        or     dword ptr [esi], 2
        db  00Fh, 0B7h, 00Dh, 00Eh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
loc_5_0C08:
        mov    eax, 30h
        xor    esi, esi
        xor    edi, edi
        db  0CDh, 020h, 004h, 000h, 017h, 000h
loc_5_0C17:
        ret
loc_5_0C18:
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_0C65
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        sti
        je     SHORT loc_5_0C65
loc_5_0C2C:
        push   eax
        mov    edi, dword ptr [eax]
        xor    eax, eax
        cmp    dword ptr [edi + 2ch], ebx
        jne    SHORT loc_5_0C40
        or     eax, 1
        mov    dword ptr [edi + 30h], 0ffffffffh
loc_5_0C40:
        cmp    dword ptr [edi + 34h], ebx
        jne    SHORT loc_5_0C4F
        or     eax, 2
        mov    dword ptr [edi + 38h], 0ffffffffh
loc_5_0C4F:
        or     eax, eax
        je     SHORT loc_5_0C58
        call   sub_5_0CDD
loc_5_0C58:
        pop    eax
        cli
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        sti
        je     SHORT loc_5_0C65
        jmp    SHORT loc_5_0C2C
loc_5_0C65:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        xor    esi, esi
        xchg   dword ptr [edi + ebx], esi
        or     esi, esi
        je     SHORT loc_5_0C7A
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_5_0C7A:
        clc
        ret
loc_5_0C7C:
        pushad
        pushfd
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    eax, 5
        mov    ecx, 24h
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_5_0CD5
        mov    dword ptr [edi + ebx], esi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_0CD9
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        sti
        je     SHORT loc_5_0CD9
loc_5_0CAD:
        mov    edi, dword ptr [eax]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0CB4:
        jb     SHORT loc_5_0CC2
        cli
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        sti
        je     SHORT loc_5_0CD9
        jmp    SHORT loc_5_0CAD
loc_5_0CC2:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     SHORT loc_5_0CD5
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_5_0CD5:
        popfd
        stc
        jmp    SHORT loc_5_0CDB
loc_5_0CD9:
        popfd
        clc
loc_5_0CDB:
        popad
        ret
sub_5_0CDD:
        push   eax
        push   edx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0CE5:
        jb     SHORT loc_5_0D5F
        test   eax, 2
        je     SHORT loc_5_0D07
        cmp    dword ptr [edi + 34h], ebx
        jne    SHORT loc_5_0D07
        call   sub_5_0AE9
        call   sub_5_0B08
        mov    dword ptr [edi + 34h], 0
        and    dword ptr [esi], 0fffffffdh
loc_5_0D07:
        test   eax, 1
        je     SHORT loc_5_0D43
        cmp    dword ptr [edi + 2ch], ebx
        jne    SHORT loc_5_0D43
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_5_0D24
        mov    eax, dword ptr [edi + 3ch]
        db  0CDh, 020h, 014h, 000h, 004h, 000h
loc_5_0D24:
        call   sub_5_0A81
        movzx  edx, word ptr [edi + 6]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0D32:
        jb     SHORT loc_5_0D39
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0D39:
        mov    dword ptr [edi + 2ch], 0
        and    dword ptr [esi], 0fffffffeh
loc_5_0D43:
        mov    eax, dword ptr [edi + 2ch]
        or     eax, eax
        jne    SHORT loc_5_0D5F
        mov    eax, dword ptr [edi + 28h]
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        db  0CDh, 020h, 003h, 000h, 003h, 000h
loc_5_0D5F:
        pop    esi
        pop    edx
        pop    eax
        clc
        ret
sub_5_0D64:
        push   edx
        push   ecx
        push   eax
        test   eax, 2
        je     SHORT loc_5_0D8D
        cmp    word ptr [edi + 8], -1
        je     SHORT loc_5_0D9F
        cmp    dword ptr [edi + 34h], 0
        je     SHORT loc_5_0D82
        cmp    dword ptr [edi + 34h], ebx
        je     SHORT loc_5_0D8D
        jmp    SHORT loc_5_0D9F
loc_5_0D82:
        mov    dword ptr [edi + 34h], ebx
        call   sub_5_0B64
        mov    dword ptr [edi + 38h], ebx
loc_5_0D8D:
        test   eax, 1
        je     SHORT loc_5_0DD1
        cmp    dword ptr [edi + 2ch], 0
        je     SHORT loc_5_0DA2
        cmp    dword ptr [edi + 2ch], ebx
        je     SHORT loc_5_0DFA
loc_5_0D9F:
        stc
        jmp    SHORT loc_5_0DFB
loc_5_0DA2:
        mov    dword ptr [edi + 2ch], ebx
        cmp    dword ptr [edi + 30h], ebx
        je     SHORT loc_5_0DB3
        movzx  edx, word ptr [edi + 6]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0DB3:
        mov    dword ptr [edi + 30h], ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0DBB:
        call   sub_5_0AB5
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_5_0DD1
        mov    eax, dword ptr [edi + 3ch]
        db  0CDh, 020h, 013h, 000h, 004h, 000h
loc_5_0DD1:
        mov    eax, dword ptr [esp]
        test   eax, 1
        je     SHORT loc_5_0DFA
        mov    eax, dword ptr [edi + 28h]
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   ecx, 10h
        je     SHORT loc_5_0DF4
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        jmp    SHORT loc_5_0DFA
loc_5_0DF4:
        db  0CDh, 020h, 009h, 000h, 003h, 000h
loc_5_0DFA:
        clc
loc_5_0DFB:
        pop    eax
        pop    ecx
        pop    edx
        ret
sub_5_0DFF:
        mov    eax, 400h
        clc
        ret
sub_5_0E06:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E14:
        add    esp, 8
        xor    eax, eax
        or     edi, edi
        je     SHORT loc_5_0E20
        mov    eax, dword ptr [edi + 6ch]
loc_5_0E20:
        pop    edi
        pop    ebp
        ret
sub_5_0E23:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E31:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_0E3E
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [edi + 6ch], eax
loc_5_0E3E:
        pop    edi
        pop    ebp
        ret
        db  0CCh, 0CCh, 0CCh
AUDDRV_PM_API:
        movzx  eax, word ptr [ebp + 14h]
        cmp    ah, 3
        jae    SHORT loc_5_0E6E
        movzx  ecx, ah
        shl    ecx, 3
        db  08Dh, 089h, 000h, 000h, 000h, 000h
        movzx  eax, al
        cmp    eax, dword ptr [ecx]
        jae    SHORT loc_5_0E6E
        mov    ecx, dword ptr [ecx + 4]
        call   dword ptr [ecx + eax*4]
        jb     SHORT loc_5_0E6E
        and    word ptr [ebp + 2ch], 0fffeh
        ret
loc_5_0E6E:
        or     word ptr [ebp + 2ch], 1
        ret
loc_5_0E74:
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E80:
        add    esp, 8
        jb     SHORT loc_5_0EA6
        movzx  eax, word ptr [ebp + 10h]
        test   eax, 0ffffffffh
        jne    SHORT loc_5_0EA6
        mov    eax, dword ptr [edi + 3ch]
        or     eax, eax
        je     SHORT loc_5_0EA6
        push   ecx
        db  0CDh, 020h, 01Ch, 000h, 004h, 000h
        mov    word ptr [ebp + 1ch], cx
        pop    ecx
        clc
        jmp    SHORT loc_5_0EAD
loc_5_0EA6:
        mov    word ptr [ebp + 1ch], 0ffffh
        stc
loc_5_0EAD:
        pop    edi
        ret
loc_5_0EAF:
        call   sub_5_0DFF
        mov    word ptr [ebp + 1ch], ax
        clc
        ret
loc_5_0EBA:
        movzx  eax, word ptr [ebp + 10h]
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_0EFC
        test   eax, 1
        je     SHORT loc_5_0EE1
        push   2
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0ED5:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_0EFC
        mov    ecx, dword ptr [edi + 30h]
        jmp    SHORT loc_5_0F04
loc_5_0EE1:
        test   eax, 2
        je     SHORT loc_5_0EFC
        push   3
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0EF0:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_0EFC
        mov    ecx, dword ptr [edi + 38h]
        jmp    SHORT loc_5_0F04
loc_5_0EFC:
        mov    word ptr [ebp + 1ch], 1
loc_5_0F02:
        stc
        ret
loc_5_0F04:
        mov    word ptr [ebp + 1ch], 0
        call   sub_5_0D64
        jb     SHORT loc_5_0F12
        ret
loc_5_0F12:
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_5_0F02
loc_5_0F1A:
        movzx  eax, word ptr [ebp + 10h]
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_0F76
        test   eax, 1
        je     SHORT loc_5_0F5B
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_5_0F47
        push   2
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0F3D:
        add    esp, 8
        or     edi, edi
        jne    SHORT loc_5_0F56
        add    edx, 4
loc_5_0F47:
        push   2
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0F4F:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_0F76
loc_5_0F56:
        mov    ecx, dword ptr [edi + 2ch]
        jmp    SHORT loc_5_0F7E
loc_5_0F5B:
        test   eax, 2
        je     SHORT loc_5_0F76
        push   3
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0F6A:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_0F76
        mov    ecx, dword ptr [edi + 34h]
        jmp    SHORT loc_5_0F7E
loc_5_0F76:
        mov    word ptr [ebp + 1ch], 1
loc_5_0F7C:
        stc
        ret
loc_5_0F7E:
        cmp    ecx, ebx
        je     SHORT loc_5_0F8A
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_5_0F7C
loc_5_0F8A:
        call   sub_5_0CDD
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
loc_5_0F97:
        pushfd
        mov    ax, word ptr [ebp + 1ch]
        and    ax, 0fffeh
        jne    SHORT loc_5_1005
        xor    ecx, ecx
        mov    ax, word ptr [ebp + 1ch]
        and    ax, 1
        je     SHORT loc_5_0FB1
        mov    ecx, dword ptr [ebp + 18h]
loc_5_0FB1:
        cli
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_1005
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_5_1005
        mov    edi, dword ptr [eax]
        jecxz  loc_5_0FD9
loc_5_0FC8:
        cmp    dword ptr [edi + 44h], ecx
        je     SHORT loc_5_0FD9
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_5_1005
        mov    edi, dword ptr [eax]
        jmp    SHORT loc_5_0FC8
loc_5_0FD9:
        popfd
        pushfd
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cmp    edi, -1
        je     SHORT loc_5_1005
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     SHORT loc_5_1005
        cmp    ecx, 72h
        ja     SHORT loc_5_1005
        cld
        rep movsb
        mov    word ptr [ebp + 1ch], 1
        popfd
        clc
        ret
loc_5_1005:
        mov    word ptr [ebp + 1ch], 0
        popfd
        stc
        ret
loc_5_100E:
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_1043
        push   3
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_101E:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_1043
        mov    ecx, dword ptr [edi + 38h]
        mov    word ptr [ebp + 1ch], 0
        mov    eax, 2
        call   sub_5_0D64
        jb     SHORT loc_5_103B
        ret
loc_5_103B:
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_5_1049
loc_5_1043:
        mov    word ptr [ebp + 1ch], 1
loc_5_1049:
        stc
        ret
loc_5_104B:
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_1083
        push   3
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_105B:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_1083
        mov    ecx, dword ptr [edi + 34h]
        cmp    ecx, ebx
        je     SHORT loc_5_1071
        mov    word ptr [ebp + 1ch], 3
        jmp    SHORT loc_5_1089
loc_5_1071:
        mov    eax, 2
        call   sub_5_0CDD
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
loc_5_1083:
        mov    word ptr [ebp + 1ch], 1
loc_5_1089:
        stc
        ret
loc_5_108B:
        pushfd
        mov    ax, word ptr [ebp + 1ch]
        cmp    ax, 1
        jne    SHORT loc_5_1101
        mov    ecx, dword ptr [ebp + 18h]
        cli
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_1101
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_5_1101
loc_5_10AC:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 44h], ecx
        je     SHORT loc_5_10BD
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_5_1101
        jmp    SHORT loc_5_10AC
loc_5_10BD:
        popfd
        pushfd
        cmp    word ptr [edi + 8], -1
        je     SHORT loc_5_1101
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cmp    edi, -1
        je     SHORT loc_5_1101
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     SHORT loc_5_1101
        cmp    ecx, 1ch
        ja     SHORT loc_5_1101
        mov    word ptr [edi + 4], 1
        mov    ax, word ptr [esi + 8]
        mov    word ptr [edi + 6], ax
        mov    eax, dword ptr [esi + 44h]
        mov    dword ptr [edi + 0ch], eax
        mov    word ptr [ebp + 1ch], 1
        popfd
        clc
        ret
loc_5_1101:
        mov    word ptr [ebp + 1ch], 0
        popfd
        stc
        ret
loc_5_110A:
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
loc_5_1162:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_118E
        lea    eax, [edi + 40h]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   eax
        push   dword ptr [esi]
        call   sub_5_12C1
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_5_118E
        mov    word ptr [ebp + 1ch], ax
        shr    eax, 10h
        mov    word ptr [ebp + 14h], ax
        clc
        ret
loc_5_118E:
        mov    word ptr [ebp + 1ch], 0
        mov    word ptr [ebp + 14h], 0
        stc
        ret
loc_5_119C:
        xchg   esi, eax
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        mov    eax, dword ptr [esi]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_11B2:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_11C8
        lea    eax, [edi + 40h]
        push   dword ptr [esi + 4]
        push   eax
        call   sub_5_1451
        add    esp, 8
loc_5_11C8:
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
sub_5_11D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 6ch
        mov    eax, 0ffffffffh
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   esi, esi
        je     NEAR PTR loc_5_12BA
        lea    eax, [ebp - 6ch]
        push   eax
        call   sub_5_14E6
        add    esp, 4
        call   sub_5_14D4
        call   sub_5_1523
        mov    edi, dword ptr [eax + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_1515
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_1515
        add    esp, 4
        mov    ebx, dword ptr [ebp + 10h]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_1515
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_1515
        add    esp, 4
        mov    ebx, dword ptr [ebp + 14h]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_1515
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_1515
        add    esp, 4
        mov    ebx, dword ptr [ebp + 18h]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_1515
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_1515
        movzx  ecx, si
        shr    esi, 10h
        add    esp, 4
        movzx  eax, si
        push   ecx
        push   eax
        call   sub_5_1506
        add    esp, 8
        call   sub_5_14E0
        movzx  esi, word ptr [edi + 14h]
        shl    esi, 10h
        movzx  eax, word ptr [edi + 1ch]
        or     esi, eax
        call   sub_5_14DA
        lea    ecx, [ebp - 6ch]
        push   ecx
        call   sub_5_14F6
        add    esp, 4
        mov    eax, esi
loc_5_12BA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_12C1:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        push   ebp
        mov    ebp, dword ptr [esi]
        test   ebp, ebp
        jne    SHORT loc_5_12E7
        push   15h
        push   4
        call   sub_5_2078
        mov    ebp, eax
        test   ebp, ebp
        jne    SHORT loc_5_12E5
        xor    eax, eax
        jmp    NEAR PTR loc_5_144C
loc_5_12E5:
        mov    dword ptr [esi], ebp
loc_5_12E7:
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 8
        jbe    SHORT loc_5_1304
        mov    eax, dword ptr [esp + 1ch]
        mov    byte ptr [eax + 8], 0
loc_5_1304:
        push   ebp
        call   sub_5_2068
        mov    ebx, eax
        test   ebx, ebx
        je     NEAR PTR loc_5_13F8
loc_5_1314:
        mov    ecx, dword ptr [esp + 1ch]
        lea    edx, [ebx + 0ch]
loc_5_131B:
        mov    al, byte ptr [ecx]
        cmp    al, byte ptr [edx]
        jne    SHORT loc_5_133B
        or     al, al
        je     SHORT loc_5_1337
        mov    al, byte ptr [ecx + 1]
        cmp    al, byte ptr [edx + 1]
        jne    SHORT loc_5_133B
        add    ecx, 2
        add    edx, 2
        or     al, al
        jne    SHORT loc_5_131B
loc_5_1337:
        xor    eax, eax
        jmp    SHORT loc_5_1340
loc_5_133B:
        sbb    eax, eax
        sbb    eax, -1
loc_5_1340:
        test   eax, eax
        je     SHORT loc_5_1351
        push   ebx
        push   ebp
        call   sub_5_2058
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_5_1314
loc_5_1351:
        test   ebx, ebx
        je     NEAR PTR loc_5_13F8
        cmp    dword ptr [ebx + 4], 0
        je     SHORT loc_5_1366
        xor    eax, eax
        jmp    NEAR PTR loc_5_144C
loc_5_1366:
        push   0
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_11D0
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_5_1393
        push   ebx
        mov    dword ptr [ebx], 0
        push   esi
        call   sub_5_1451
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_5_144C
loc_5_1393:
        mov    eax, dword ptr [esp + 20h]
        push   0
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebx + 4], ecx
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   ecx
        call   sub_5_11D0
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_5_13CA
        mov    dword ptr [ebx + 4], 0
        push   ebx
        push   esi
        call   sub_5_1451
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_5_144C
loc_5_13CA:
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx + 8]
        push   10000h
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_11D0
        add    esp, 14h
        push   dword ptr [ebx]
        push   dword ptr [ebx + 8]
        push   10000h
        push   ebx
        push   dword ptr [ebx + 4]
        call   sub_5_11D0
        add    esp, 14h
        jmp    SHORT loc_5_144A
loc_5_13F8:
        push   ebp
        call   sub_5_2044
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_5_1408
        xor    eax, eax
        jmp    SHORT loc_5_144C
loc_5_1408:
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
        call   sub_5_2034
loc_5_144A:
        mov    eax, ebx
loc_5_144C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_1451:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        test   edi, edi
        je     SHORT loc_5_149D
        mov    eax, dword ptr [edi]
        test   eax, eax
        je     SHORT loc_5_147B
        push   0
        push   dword ptr [edi + 8]
        push   40000h
        push   edi
        push   eax
        call   sub_5_11D0
        add    esp, 14h
        mov    dword ptr [edi], 0
loc_5_147B:
        mov    eax, dword ptr [edi + 4]
        test   eax, eax
        je     SHORT loc_5_149D
        push   0
        push   dword ptr [edi + 8]
        push   40000h
        push   edi
        push   eax
        call   sub_5_11D0
        mov    dword ptr [edi + 4], 0
        add    esp, 14h
loc_5_149D:
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     SHORT loc_5_14CE
        push   edi
        push   eax
        call   sub_5_20AC
        push   edi
        push   dword ptr [esi]
        call   sub_5_209C
        push   dword ptr [esi]
        call   sub_5_2068
        test   eax, eax
        jne    SHORT loc_5_14C8
        push   dword ptr [esi]
        call   sub_5_208C
loc_5_14C8:
        mov    dword ptr [esi], 0
loc_5_14CE:
        pop    edi
        pop    esi
        ret
        db  0CCh, 0CCh, 0CCh
sub_5_14D4:
        db  0CDh, 020h, 083h, 080h, 001h, 000h
sub_5_14DA:
        db  0CDh, 020h, 086h, 080h, 001h, 000h
sub_5_14E0:
        db  0CDh, 020h, 085h, 080h, 001h, 000h
sub_5_14E6:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        pop    ebp
        ret
sub_5_14F6:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        pop    ebp
        ret
sub_5_1506:
        movzx  ecx, word ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        db  0CDh, 020h, 048h, 080h, 001h, 000h
sub_5_1515:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        pop    ebp
        ret
sub_5_1523:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
loc_5_152E:
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        push   edi
        mov    ecx, 10h
        db  0BFh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb
        je     SHORT loc_5_1563
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        mov    esi, eax
        repe cmpsb
        mov    eax, 80004002h
        jne    SHORT loc_5_1571
loc_5_1563:
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_1571:
        pop    edi
        pop    esi
        ret    0ch
loc_5_1576:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0ch]
        inc    eax
        mov    dword ptr [ecx + 0ch], eax
        ret    4
loc_5_1584:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0ch]
        dec    eax
        mov    dword ptr [ecx + 0ch], eax
        ret    4
loc_5_1592:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        push   0ffh
        db  068h, 000h, 000h, 000h, 000h
        lea    esi, [edi + 104h]
        push   esi
        mov    dword ptr [edi], 9
        call   sub_5_20C0
        add    esp, 0ch
        call   sub_5_1B30
        push   0ffh
        lea    ecx, [edi + 4]
        push   eax
        push   ecx
        call   sub_5_20C0
        mov    edx, dword ptr [esp + 18h]
        add    esp, 0ch
        xor    ecx, ecx
        mov    dword ptr [edi + 210h], ecx
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [edi + 204h], eax
        mov    dword ptr [edi + 20ch], ecx
        mov    word ptr [edi + 208h], 33f4h
        mov    eax, 0b3h
        cmp    byte ptr [esi], cl
        je     SHORT loc_5_1606
loc_5_15FB:
        movsx  ecx, byte ptr [esi]
        add    eax, ecx
        inc    esi
        cmp    byte ptr [esi], 0
        jne    SHORT loc_5_15FB
loc_5_1606:
        mov    ecx, 0f7h
        sub    edx, edx
        div    ecx
        mov    word ptr [edi + 20ah], dx
        xor    eax, eax
        pop    edi
        pop    esi
        ret    8
loc_5_161D:
        push   esi
        push   edi
        mov    edi, 8878000ah
        mov    esi, dword ptr [esp + 0ch]
        cmp    dword ptr [esi + 0ch], 0
        jne    SHORT loc_5_1642
        push   dword ptr [esi + 4]
        call   sub_5_1C11
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_5_1642
        xor    edi, edi
        inc    dword ptr [esi + 0ch]
loc_5_1642:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    4
loc_5_1649:
        mov    eax, dword ptr [esp + 8]
        xor    ecx, ecx
        mov    dword ptr [eax], 0fh
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
loc_5_1694:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   ebp
        mov    edi, 8007000eh
        mov    esi, dword ptr [esp + 2ch]
        test   byte ptr [esp + 1ch], 0f0h
        mov    dword ptr [esi], ebx
        je     SHORT loc_5_16B6
        mov    eax, 8878001eh
        jmp    NEAR PTR loc_5_177C
loc_5_16B6:
        mov    ebp, dword ptr [esp + 14h]
        cmp    dword ptr [ebp + 10h], 0
        jne    NEAR PTR loc_5_175B
        push   201h
        push   31h
        call   sub_5_20D0
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_5_1754
        db  0C7h, 003h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 4], ebp
        push   dword ptr [ebp + 4]
        push   ebx
        call   sub_5_1C53
        add    esp, 8
        mov    eax, dword ptr [ebx + 14h]
        cmp    eax, 1000h
        jbe    SHORT loc_5_1760
        cmp    eax, 0ffffh
        jae    SHORT loc_5_1760
        cmp    dword ptr [ebx + 0ch], 0
        je     SHORT loc_5_1760
        cmp    dword ptr [ebx + 10h], 0
        je     SHORT loc_5_1760
        cmp    dword ptr [ebx + 18h], 0
        je     SHORT loc_5_1760
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 2ch]
        push   ebx
        xor    edi, edi
        call   sub_5_18D4
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
        call   sub_5_1C9A
        add    esp, 14h
        jmp    SHORT loc_5_1760
loc_5_1754:
        mov    edi, 8007000eh
        jmp    SHORT loc_5_1760
loc_5_175B:
        mov    edi, 8878000ah
loc_5_1760:
        test   edi, edi
        je     SHORT loc_5_1775
        test   ebx, ebx
        je     SHORT loc_5_177A
        push   0
        push   ebx
        call   sub_5_20C8
        add    esp, 8
        jmp    SHORT loc_5_177A
loc_5_1775:
        mov    dword ptr [esi], ebx
        inc    dword ptr [ebp + 10h]
loc_5_177A:
        mov    eax, edi
loc_5_177C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    1ch
loc_5_1783:
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], 0
        mov    eax, 80004001h
        ret    0ch
loc_5_1795:
        mov    eax, 80004005h
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 0ch]
        dec    edx
        mov    dword ptr [ecx + 0ch], edx
        jne    SHORT loc_5_17BA
        cmp    dword ptr [ecx + 10h], 0
        jne    SHORT loc_5_17BA
        push   dword ptr [ecx + 4]
        call   sub_5_1BF4
        add    esp, 4
        xor    eax, eax
loc_5_17BA:
        ret    4
sub_5_17BD:
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
        call   sub_5_20D0
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_5_183C
        mov    edi, dword ptr [ebp + 8]
        push   1
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 14h]
        push   ecx
        mov    dword ptr [esi + 4], edi
        db  0C7h, 006h, 000h, 000h, 000h, 000h
        call   sub_5_20EC
        test   eax, eax
        jne    SHORT loc_5_183C
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
        jne    SHORT loc_5_183C
        push   esi
        mov    ebx, 1
        push   edi
        call   sub_5_0E23
        add    esp, 8
loc_5_183C:
        test   ebx, ebx
        jne    SHORT loc_5_1865
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_5_1856
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 0bh]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_5_20D8
loc_5_1856:
        test   esi, esi
        je     SHORT loc_5_1865
        push   0
        push   esi
        call   sub_5_20C8
        add    esp, 8
loc_5_1865:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_186C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_5_0E06
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_5_18CE
        push   0
        push   edi
        call   sub_5_0E23
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
        call   sub_5_20D8
        push   0
        push   esi
        call   sub_5_20C8
        add    esp, 8
loc_5_18CE:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_5_18D4:
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 14h]
        cmp    dword ptr [edx], eax
        jbe    SHORT loc_5_18E6
        mov    dword ptr [edx], eax
loc_5_18E6:
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
loc_5_1906:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 20h]
        dec    esi
        mov    dword ptr [eax + 20h], esi
        jne    SHORT loc_5_192B
        cmp    dword ptr [eax + 8], 0
        jne    SHORT loc_5_192B
        mov    ecx, dword ptr [eax + 4]
        push   0
        push   eax
        dec    dword ptr [ecx + 10h]
        call   sub_5_20C8
        add    esp, 8
loc_5_192B:
        mov    eax, esi
        pop    esi
        ret    4
loc_5_1931:
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        push   edi
        mov    ecx, 10h
        db  0BFh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb
        je     SHORT loc_5_1966
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        mov    esi, eax
        repe cmpsb
        mov    eax, 80004002h
        jne    SHORT loc_5_1974
loc_5_1966:
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_1974:
        pop    edi
        pop    esi
        ret    0ch
loc_5_1979:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 20h]
        inc    eax
        mov    dword ptr [ecx + 20h], eax
        ret    4
loc_5_1987:
        mov    eax, 80004001h
        ret    20h
loc_5_198F:
        mov    eax, 80004001h
        ret    14h
loc_5_1997:
        mov    eax, dword ptr [esp + 8]
        test   eax, eax
        je     SHORT loc_5_19F0
        cmp    word ptr [eax], 1
        jne    SHORT loc_5_19F0
        mov    edx, dword ptr [eax + 4]
        cmp    edx, 0afc8h
        jae    SHORT loc_5_19F0
        mov    cx, word ptr [eax + 0eh]
        cmp    cx, 8
        je     SHORT loc_5_19C0
        cmp    cx, 10h
        jne    SHORT loc_5_19F0
loc_5_19C0:
        mov    ax, word ptr [eax + 2]
        cmp    ax, 1
        je     SHORT loc_5_19D0
        cmp    ax, 2
        jne    SHORT loc_5_19F0
loc_5_19D0:
        movzx  eax, ax
        movzx  ecx, cx
        push   eax
        push   ecx
        mov    eax, dword ptr [esp + 0ch]
        push   edx
        movzx  ecx, word ptr [eax + 2eh]
        push   1
        push   ecx
        call   sub_5_1C9A
        add    esp, 14h
        xor    eax, eax
        jmp    SHORT loc_5_19F5
loc_5_19F0:
        mov    eax, 88780064h
loc_5_19F5:
        ret    8
loc_5_19F8:
        mov    eax, dword ptr [esp + 8]
        test   eax, eax
        je     SHORT loc_5_1A1D
        cmp    eax, 0afc8h
        jae    SHORT loc_5_1A1D
        mov    ecx, dword ptr [esp + 4]
        push   eax
        movzx  edx, word ptr [ecx + 2eh]
        push   edx
        call   sub_5_1D0E
        add    esp, 8
        xor    eax, eax
        jmp    SHORT loc_5_1A22
loc_5_1A1D:
        mov    eax, 88780064h
loc_5_1A22:
        ret    8
loc_5_1A25:
        mov    eax, 80004001h
        ret    8
loc_5_1A2D:
        mov    eax, 80004001h
        ret    8
loc_5_1A35:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 8], 1
        jne    SHORT loc_5_1A67
        push   dword ptr [esi + 18h]
        call   sub_5_1B3E
        add    esp, 4
        inc    eax
        cmp    byte ptr [esi + 30h], 3
        jbe    SHORT loc_5_1A54
        add    eax, eax
loc_5_1A54:
        mov    ecx, dword ptr [esi + 1ch]
        mov    edx, dword ptr [esp + 10h]
        sub    ecx, eax
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx], ecx
        jmp    SHORT loc_5_1A7B
loc_5_1A67:
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx], 0
loc_5_1A7B:
        xor    eax, eax
        pop    esi
        ret    0ch
loc_5_1A81:
        push   esi
        mov    eax, 80004005h
        mov    esi, dword ptr [esp + 8]
        push   edi
        cmp    dword ptr [esi + 8], 0
        jne    SHORT loc_5_1AF6
        push   dword ptr [esi + 18h]
        movzx  edi, word ptr [esi + 2eh]
        call   sub_5_1B59
        add    esp, 4
        push   dword ptr [esi + 1ch]
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi + 18h]
        call   sub_5_1B7D
        add    esp, 0ch
        push   dword ptr [esi + 18h]
        call   sub_5_1B9E
        add    esp, 4
        push   dword ptr [esi + 18h]
        call   sub_5_1B67
        mov    ax, word ptr [esi + 1ch]
        add    esp, 4
        cmp    dword ptr [esi + 24h], 10h
        jne    SHORT loc_5_1AD5
        shr    ax, 1
loc_5_1AD5:
        cmp    dword ptr [esi + 28h], 2
        jne    SHORT loc_5_1ADE
        shr    ax, 1
loc_5_1ADE:
        dec    ax
        movzx  ecx, ax
        push   ecx
        push   edi
        call   sub_5_1D72
        add    esp, 8
        xor    eax, eax
        mov    dword ptr [esi + 8], 1
loc_5_1AF6:
        pop    edi
        pop    esi
        ret    10h
loc_5_1AFB:
        push   esi
        mov    eax, 80004005h
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 8], 0
        je     SHORT loc_5_1B2C
        movzx  eax, word ptr [esi + 2eh]
        push   eax
        call   sub_5_1F50
        add    esp, 4
        push   dword ptr [esi + 18h]
        call   sub_5_1B59
        add    esp, 4
        xor    eax, eax
        mov    dword ptr [esi + 8], 0
loc_5_1B2C:
        pop    esi
        ret    4
sub_5_1B30:
        db  00Fh, 0B7h, 005h, 010h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        ret
sub_5_1B3E:
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
sub_5_1B59:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 018h, 000h, 004h, 000h
        pop    ebp
        ret
sub_5_1B67:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 019h, 000h, 004h, 000h
        pop    ebx
        pop    ebp
        ret
sub_5_1B7D:
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
sub_5_1B9E:
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
loc_5_1BD4:
        xor    eax, eax
        jae    SHORT loc_5_1BDD
        mov    eax, 1
loc_5_1BDD:
        pop    edx
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    ah, byte ptr [ebp + 0ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1BEF:
        xor    ah, ah
        pop    edx
        pop    ebp
        ret
sub_5_1BF4:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1C04:
        add    esp, 8
        mov    word ptr [edi + 70h], 0
        pop    ebx
        pop    edi
        pop    ebp
        ret
sub_5_1C11:
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
loc_5_1C27:
        add    esp, 8
        mov    eax, dword ptr [edi + 2ch]
        or     eax, eax
        je     SHORT loc_5_1C3D
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        cmp    eax, ebx
        je     SHORT loc_5_1C3D
        jmp    SHORT loc_5_1C4C
loc_5_1C3D:
        mov    word ptr [edi + 70h], 1
        movzx  edx, word ptr [edi + 6]
        mov    edx, 0
loc_5_1C4C:
        mov    eax, edx
        pop    ebx
        pop    edx
        pop    edi
        pop    ebp
        ret
sub_5_1C53:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1C66:
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
sub_5_1C9A:
        push   ebp
        mov    ebp, esp
        push   eax
        push   edi
        cmp    word ptr [ebp + 0ch], 1
        jne    SHORT loc_5_1D09
        cmp    word ptr [ebp + 10h], 0fa0h
        jb     SHORT loc_5_1D09
        cmp    word ptr [ebp + 10h], 0afc8h
        ja     SHORT loc_5_1D09
        cmp    word ptr [ebp + 14h], 8
        je     SHORT loc_5_1CC4
        cmp    word ptr [ebp + 14h], 10h
        jne    SHORT loc_5_1D09
loc_5_1CC4:
        cmp    word ptr [ebp + 18h], 1
        jb     SHORT loc_5_1D09
        cmp    word ptr [ebp + 18h], 2
        ja     SHORT loc_5_1D09
        db  066h, 083h, 025h, 000h, 000h, 000h, 000h, 0F9h
        cmp    word ptr [ebp + 14h], 10h
        jne    SHORT loc_5_1CE9
        db  066h, 083h, 00Dh, 000h, 000h, 000h, 000h, 002h
loc_5_1CE9:
        cmp    word ptr [ebp + 18h], 2
        jne    SHORT loc_5_1CF8
        db  066h, 083h, 00Dh, 000h, 000h, 000h, 000h, 004h
loc_5_1CF8:
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 8]
        call   sub_5_1D0E
        add    esp, 8
        clc
        jmp    SHORT loc_5_1D0A
loc_5_1D09:
        stc
loc_5_1D0A:
        pop    edi
        pop    eax
        pop    ebp
        ret
sub_5_1D0E:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ebx
        push   edx
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    al, 0c6h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1D1F:
        jb     SHORT loc_5_1D6B
        mov    bx, word ptr [ebp + 0ch]
        push   edx
        mov    dx, 6
        mov    ax, 1198h
        div    bx
        mov    ah, al
        mov    al, 80h
        sub    al, ah
        pop    edx
        push   eax
        mov    al, 0a1h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1D40:
        pop    eax
        jb     SHORT loc_5_1D6B
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1D48:
        jb     SHORT loc_5_1D6B
        push   edx
        mov    dx, 3
        mov    ax, 0b9eh
        div    bx
        neg    al
        pop    edx
        push   eax
        mov    al, 0a2h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1D61:
        pop    eax
        jb     SHORT loc_5_1D6B
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1D69:
        jmp    SHORT loc_5_1D6C
loc_5_1D6B:
        stc
loc_5_1D6C:
        pop    edi
        pop    edx
        pop    ebx
        pop    eax
        pop    ebp
        ret
sub_5_1D72:
        push   ebp
        mov    ebp, esp
        push   eax
        push   edx
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    al, 0d1h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1D82:
        jb     NEAR PTR loc_5_1F4A
        add    edx, 6
        mov    al, 2
        out    dx, al
        in     al, dx
        in     al, dx
        in     al, dx
        sub    edx, 6
        jmp    SHORT loc_5_1D96
loc_5_1D96:
        jmp    SHORT loc_5_1D98
loc_5_1D98:
        add    edx, 6
        xor    al, al
        out    dx, al
        sub    edx, 6
        mov    al, 0c6h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1DA8:
        jb     NEAR PTR loc_5_1F4A
        mov    al, 0a4h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1DB5:
        jb     NEAR PTR loc_5_1F4A
        mov    eax, dword ptr [ebp + 0ch]
        neg    ax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1DC6:
        jb     NEAR PTR loc_5_1F4A
        push   eax
        mov    al, 0a5h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1DD4:
        pop    eax
        jb     NEAR PTR loc_5_1F4A
        mov    al, ah
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1DE2:
        jb     NEAR PTR loc_5_1F4A
        mov    al, 0a8h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1DEF:
        jb     NEAR PTR loc_5_1F4A
        mov    al, 0f5h
        db  066h, 0F7h, 005h, 000h, 000h, 000h, 000h, 004h, 000h
        jne    SHORT loc_5_1E04
        mov    al, 0f6h
loc_5_1E04:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1E09:
        jb     NEAR PTR loc_5_1F4A
        mov    al, 0c0h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1E16:
        jb     NEAR PTR loc_5_1F4A
        mov    al, 0b1h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1E23:
        jb     NEAR PTR loc_5_1F4A
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1E2E:
        jb     NEAR PTR loc_5_1F4A
        and    al, 0fh
        or     al, 50h
        push   eax
        mov    al, 0b1h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1E40:
        pop    eax
        jb     NEAR PTR loc_5_1F4A
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1E4C:
        jb     NEAR PTR loc_5_1F4A
        mov    al, 0c0h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1E59:
        jb     NEAR PTR loc_5_1F4A
        mov    al, 0b2h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1E66:
        jb     NEAR PTR loc_5_1F4A
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1E71:
        jb     NEAR PTR loc_5_1F4A
        and    al, 0fh
        or     al, 50h
        push   eax
        mov    al, 0b2h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1E83:
        pop    eax
        jb     NEAR PTR loc_5_1F4A
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1E8F:
        jb     NEAR PTR loc_5_1F4A
        db  066h, 0F7h, 005h, 000h, 000h, 000h, 000h, 002h, 000h
        je     SHORT loc_5_1EEF
        mov    al, 0b6h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1EA7:
        jb     NEAR PTR loc_5_1F4A
        xor    al, al
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1EB4:
        jb     NEAR PTR loc_5_1F4A
        mov    al, 0b7h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1EC1:
        jb     NEAR PTR loc_5_1F4A
        mov    al, 71h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1ECE:
        jb     SHORT loc_5_1F4A
        mov    al, 0b7h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1ED7:
        jb     SHORT loc_5_1F4A
        mov    al, 0bch
        db  066h, 0F7h, 005h, 000h, 000h, 000h, 000h, 004h, 000h
        jne    SHORT loc_5_1EE8
        mov    al, 0f4h
loc_5_1EE8:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1EED:
        jb     SHORT loc_5_1F4A
loc_5_1EEF:
        mov    al, 0b7h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1EF6:
        jb     SHORT loc_5_1F4A
        mov    al, 0d0h
        db  066h, 0F7h, 005h, 000h, 000h, 000h, 000h, 004h, 000h
        je     SHORT loc_5_1F07
        mov    al, 98h
loc_5_1F07:
        db  066h, 0F7h, 005h, 000h, 000h, 000h, 000h, 002h, 000h
        je     SHORT loc_5_1F14
        or     al, 24h
loc_5_1F14:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1F19:
        jb     SHORT loc_5_1F4A
        mov    al, 0c0h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1F22:
        jb     SHORT loc_5_1F4A
        mov    al, 0b8h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1F2B:
        jb     SHORT loc_5_1F4A
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1F32:
        jb     SHORT loc_5_1F4A
        and    al, 30h
        or     al, 5
        push   eax
        mov    al, 0b8h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1F40:
        pop    eax
        jb     SHORT loc_5_1F4A
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1F48:
        jmp    SHORT loc_5_1F4B
loc_5_1F4A:
        stc
loc_5_1F4B:
        pop    edi
        pop    edx
        pop    eax
        pop    ebp
        ret
sub_5_1F50:
        push   ebp
        mov    ebp, esp
        push   eax
        push   edx
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    al, 0d3h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1F60:
        jb     SHORT loc_5_1F98
        mov    al, 0c0h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1F69:
        jb     SHORT loc_5_1F98
        mov    al, 0b8h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1F72:
        jb     SHORT loc_5_1F98
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1F79:
        jb     SHORT loc_5_1F98
        and    al, 30h
        push   eax
        mov    al, 0b8h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1F85:
        pop    eax
        jb     SHORT loc_5_1F98
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1F8D:
        jmp    SHORT loc_5_1F99
        db  0B0h, 0D0h, 0E8h, 000h, 000h, 000h, 000h, 0EBh, 001h
loc_5_1F98:
        stc
loc_5_1F99:
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
        call   sub_5_1D72
        add    esp, 0ch
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_5_1F50
        add    esp, 8
        pop    ebp
        ret
        db  050h, 052h, 0C1h, 0E8h, 008h, 024h, 0F0h, 0C1h, 0EBh, 00Ch, 00Ah, 0C3h, 083h, 0C2h, 004h, 050h
        db  0B0h, 014h, 0EEh, 058h, 066h, 042h, 0EEh, 0F8h, 05Ah, 058h, 0C3h, 0CCh, 0CCh
sub_5_1FE4:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        int3
        int3
sub_5_1FEC:
        db  0CDh, 020h, 04Bh, 081h, 001h, 000h
        int3
        int3
sub_5_1FF4:
        db  0CDh, 020h, 052h, 081h, 001h, 000h
        int3
        int3
sub_5_1FFC:
        db  0CDh, 020h, 04Ah, 081h, 001h, 000h
        int3
        int3
sub_5_2004:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        int3
        int3
sub_5_200C:
        db  0CDh, 020h, 051h, 081h, 001h, 000h
        int3
        int3
sub_5_2014:
        db  0CDh, 020h, 03Dh, 080h, 033h, 000h
        int3
        int3
sub_5_201C:
        db  0CDh, 020h, 074h, 081h, 001h, 000h
        int3
        int3
sub_5_2024:
        db  0CDh, 020h, 044h, 080h, 033h, 000h
        int3
        int3
sub_5_202C:
        db  0CDh, 020h, 03Bh, 080h, 033h, 000h
        int3
        int3
sub_5_2034:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_2044:
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
sub_5_2058:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_2068:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_2078:
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
sub_5_208C:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_209C:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_20AC:
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
sub_5_20C0:
        db  0CDh, 020h, 07Bh, 081h, 001h, 000h
        int3
        int3
sub_5_20C8:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        int3
        int3
sub_5_20D0:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        int3
        int3
sub_5_20D8:
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
sub_5_20EC:
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 0ch]
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        jb     SHORT loc_5_210A
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_5_210A:
        ret    10h
_PTEXT ends
        end
