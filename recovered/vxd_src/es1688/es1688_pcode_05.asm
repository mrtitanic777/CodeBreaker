; es1688_pcode_05.asm -- ES1688 object 5 segment _PTEXT
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
        call   sub_5_2668
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_5_051D
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
        lea    eax, [ebp - 10h]
        lea    ecx, [ebp - 150h]
        push   eax
        push   ecx
        push   80000002h
        call   sub_5_2658
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_01F1
        xor    eax, eax
        jmp    NEAR PTR loc_5_051D
loc_5_01F1:
        push   1
        lea    eax, [ebp - 250h]
        push   100h
        push   eax
        push   0
        push   ebx
        call   sub_5_2668
        add    esp, 14h
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_5_0218
        push   dword ptr [ebp - 10h]
        jmp    NEAR PTR loc_5_0513
loc_5_0218:
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 250h]
        push   eax
        push   ecx
        push   80000002h
        call   sub_5_2658
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_023E
        mov    esi, 1dh
        jmp    NEAR PTR loc_5_04E8
loc_5_023E:
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_5_037E
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 50h]
        mov    dword ptr [ebp - 8], 40h
        push   eax
        push   ecx
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 10h]
        call   sub_5_2660
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_5_0279
        mov    esi, 1dh
        jmp    NEAR PTR loc_5_04E8
loc_5_0279:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_5_02DE
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 0ch]
        call   sub_5_2658
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_02AB
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 0ch]
        call   sub_5_2650
        add    esp, 0ch
loc_5_02AB:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_5_030F
        push   dword ptr [ebp - 8]
        lea    ecx, [ebp - 50h]
        push   ecx
        push   1
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        call   sub_5_2648
        add    esp, 18h
        push   dword ptr [ebp - 4]
        call   sub_5_2638
        mov    dword ptr [ebp - 4], 0
        add    esp, 4
loc_5_02DE:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 50h]
        mov    dword ptr [ebp - 8], 40h
        push   eax
        push   ecx
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 10h]
        call   sub_5_2660
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_5_0319
        mov    esi, 1dh
        jmp    NEAR PTR loc_5_04E8
loc_5_030F:
        mov    esi, 1dh
        jmp    NEAR PTR loc_5_04E8
loc_5_0319:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_5_037E
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 0ch]
        call   sub_5_2658
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_034B
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 0ch]
        call   sub_5_2650
        add    esp, 0ch
loc_5_034B:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_5_03AF
        push   dword ptr [ebp - 8]
        lea    ecx, [ebp - 50h]
        push   ecx
        push   1
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        call   sub_5_2648
        add    esp, 18h
        push   dword ptr [ebp - 4]
        call   sub_5_2638
        mov    dword ptr [ebp - 4], 0
        add    esp, 4
loc_5_037E:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 50h]
        mov    dword ptr [ebp - 8], 40h
        push   eax
        push   ecx
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 10h]
        call   sub_5_2660
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_5_03B9
        mov    esi, 1dh
        jmp    NEAR PTR loc_5_04E8
loc_5_03AF:
        mov    esi, 1dh
        jmp    NEAR PTR loc_5_04E8
loc_5_03B9:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_5_041E
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 0ch]
        call   sub_5_2658
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_03EB
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 0ch]
        call   sub_5_2650
        add    esp, 0ch
loc_5_03EB:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_5_044F
        push   dword ptr [ebp - 8]
        lea    ecx, [ebp - 50h]
        push   ecx
        push   1
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        call   sub_5_2648
        add    esp, 18h
        push   dword ptr [ebp - 4]
        call   sub_5_2638
        mov    dword ptr [ebp - 4], 0
        add    esp, 4
loc_5_041E:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 50h]
        mov    dword ptr [ebp - 8], 40h
        push   eax
        push   ecx
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 10h]
        call   sub_5_2660
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_5_0459
        mov    esi, 1dh
        jmp    NEAR PTR loc_5_04E8
loc_5_044F:
        mov    esi, 1dh
        jmp    NEAR PTR loc_5_04E8
loc_5_0459:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_5_04C2
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 0ch]
        call   sub_5_2658
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_048B
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 0ch]
        call   sub_5_2650
        add    esp, 0ch
loc_5_048B:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_5_0524
        push   dword ptr [ebp - 8]
        lea    ecx, [ebp - 50h]
        push   ecx
        push   1
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        call   sub_5_2648
        add    esp, 18h
        push   dword ptr [ebp - 4]
        call   sub_5_2638
        mov    dword ptr [ebp - 4], 0
        add    esp, 4
loc_5_04C2:
        push   dword ptr [ebp - 10h]
        call   sub_5_2638
        add    esp, 4
        lea    eax, [ebp - 150h]
        mov    dword ptr [ebp - 10h], 0
        push   eax
        push   80000002h
        call   sub_5_2640
        add    esp, 8
loc_5_04E8:
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_5_04F9
        push   dword ptr [ebp - 10h]
        call   sub_5_2638
        add    esp, 4
loc_5_04F9:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_5_050A
        push   dword ptr [ebp - 0ch]
        call   sub_5_2638
        add    esp, 4
loc_5_050A:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_5_051B
        push   dword ptr [ebp - 4]
loc_5_0513:
        call   sub_5_2638
        add    esp, 4
loc_5_051B:
        mov    eax, esi
loc_5_051D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_0524:
        mov    esi, 1dh
        jmp    SHORT loc_5_04E8
sub_5_052B:
        push   ebp
        mov    ebp, esp
        sub    esp, 110h
        test   byte ptr [ebp + 20h], 50h
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_5_0544
        xor    eax, eax
        jmp    NEAR PTR loc_5_0855
loc_5_0544:
        push   1
        lea    eax, [ebp - 110h]
        push   100h
        push   eax
        push   0
        push   dword ptr [ebp + 8]
        call   sub_5_2668
        add    esp, 14h
        mov    dword ptr [ebp - 10h], eax
        test   eax, eax
        jne    NEAR PTR loc_5_0855
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 110h]
        push   eax
        push   ecx
        push   80000002h
        call   sub_5_2658
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_0590
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0855
loc_5_0590:
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_5_06F4
        lea    eax, [ebp - 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 0ch]
        call   sub_5_2658
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_05C7
        push   dword ptr [ebp - 0ch]
        call   sub_5_2638
        add    esp, 4
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0855
loc_5_05C7:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, 100h
        xor    ebx, ebx
loc_5_05D3:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 110h]
        push   eax
        mov    dword ptr [ebp - 4], edi
        push   ecx
        push   ebx
        push   ebx
        push   dword ptr [esi]
        push   dword ptr [ebp - 8]
        call   sub_5_2660
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_5_05F9
        cmp    dword ptr [ebp - 4], ebx
        jne    SHORT loc_5_0604
loc_5_05F9:
        add    esi, 4
        db  081h, 0FEh, 00Ch, 000h, 000h, 000h
        jb     SHORT loc_5_05D3
loc_5_0604:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_5_0835
        push   0
        lea    ecx, [ebp - 110h]
        push   0fffh
        push   dword ptr [ebp + 0ch]
        push   ecx
        call   sub_5_0068
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_5_0835
        push   dword ptr [ebp - 8]
        call   sub_5_2638
        add    esp, 4
        lea    eax, [ebp - 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 0ch]
        call   sub_5_2658
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_0667
        push   dword ptr [ebp - 0ch]
        call   sub_5_2638
        add    esp, 4
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0855
loc_5_0667:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, 100h
        xor    ebx, ebx
loc_5_0673:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 110h]
        push   eax
        mov    dword ptr [ebp - 4], edi
        push   ecx
        push   ebx
        push   ebx
        push   dword ptr [esi]
        push   dword ptr [ebp - 8]
        call   sub_5_2660
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_5_0699
        cmp    dword ptr [ebp - 4], ebx
        jne    SHORT loc_5_06A4
loc_5_0699:
        add    esi, 4
        db  081h, 0FEh, 00Ch, 000h, 000h, 000h
        jb     SHORT loc_5_0673
loc_5_06A4:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_5_0835
        push   1
        mov    esi, dword ptr [ebp + 10h]
        push   0fh
        lea    eax, [ebp - 110h]
        push   esi
        push   eax
        call   sub_5_0068
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_5_0835
        cmp    word ptr [esi], 0ffffh
        je     SHORT loc_5_06E9
loc_5_06D4:
        cmp    word ptr [esi], 2
        jne    SHORT loc_5_06DF
        mov    word ptr [esi], 9
loc_5_06DF:
        add    esi, 2
        cmp    word ptr [esi], 0ffffh
        jne    SHORT loc_5_06D4
loc_5_06E9:
        push   dword ptr [ebp - 8]
        call   sub_5_2638
        add    esp, 4
loc_5_06F4:
        lea    eax, [ebp - 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 0ch]
        call   sub_5_2658
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_0721
        push   dword ptr [ebp - 0ch]
        call   sub_5_2638
        add    esp, 4
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0855
loc_5_0721:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, 100h
        xor    ebx, ebx
loc_5_072D:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 110h]
        push   eax
        mov    dword ptr [ebp - 4], edi
        push   ecx
        push   ebx
        push   ebx
        push   dword ptr [esi]
        push   dword ptr [ebp - 8]
        call   sub_5_2660
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_5_0753
        cmp    dword ptr [ebp - 4], ebx
        jne    SHORT loc_5_075E
loc_5_0753:
        add    esi, 4
        db  081h, 0FEh, 00Ch, 000h, 000h, 000h
        jb     SHORT loc_5_072D
loc_5_075E:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_5_0835
        push   1
        mov    esi, dword ptr [ebp + 18h]
        push   0fh
        lea    eax, [ebp - 110h]
        push   esi
        push   eax
        call   sub_5_0068
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_5_0835
        cmp    word ptr [esi], 0ffffh
        je     SHORT loc_5_07A3
loc_5_078E:
        cmp    word ptr [esi], 2
        jne    SHORT loc_5_0799
        mov    word ptr [esi], 9
loc_5_0799:
        add    esi, 2
        cmp    word ptr [esi], 0ffffh
        jne    SHORT loc_5_078E
loc_5_07A3:
        push   dword ptr [ebp - 8]
        call   sub_5_2638
        add    esp, 4
        lea    eax, [ebp - 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 0ch]
        call   sub_5_2658
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_07D8
        push   dword ptr [ebp - 0ch]
        call   sub_5_2638
        add    esp, 4
        mov    eax, 13h
        jmp    SHORT loc_5_0855
loc_5_07D8:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, 100h
        xor    ebx, ebx
loc_5_07E4:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 110h]
        push   eax
        mov    dword ptr [ebp - 4], edi
        push   ecx
        push   ebx
        push   ebx
        push   dword ptr [esi]
        push   dword ptr [ebp - 8]
        call   sub_5_2660
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_5_080A
        cmp    dword ptr [ebp - 4], ebx
        jne    SHORT loc_5_0815
loc_5_080A:
        add    esi, 4
        db  081h, 0FEh, 00Ch, 000h, 000h, 000h
        jb     SHORT loc_5_07E4
loc_5_0815:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_5_0835
        push   1
        lea    ecx, [ebp - 110h]
        push   3
        push   dword ptr [ebp + 1ch]
        push   ecx
        call   sub_5_0068
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_5_083C
loc_5_0835:
        mov    dword ptr [ebp - 10h], 13h
loc_5_083C:
        push   dword ptr [ebp - 8]
        call   sub_5_2638
        add    esp, 4
        push   dword ptr [ebp - 0ch]
        call   sub_5_2638
        add    esp, 4
        mov    eax, dword ptr [ebp - 10h]
loc_5_0855:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_085C:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        db  0CDh, 020h, 088h, 001h, 001h, 000h
        test   eax, eax
        jne    SHORT loc_5_08A9
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
loc_5_08A9:
        mov    esp, ebp
        pop    ebp
        ret
sub_5_08AD:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 158h
        test   eax, eax
        push   ebx
        push   esi
        push   edi
        je     NEAR PTR loc_5_0D26
        cmp    eax, 1
        je     SHORT loc_5_0901
        cmp    eax, 2
        je     NEAR PTR loc_5_0D11
        cmp    eax, 3
        je     NEAR PTR loc_5_0C83
        cmp    eax, 4
        je     NEAR PTR loc_5_0CCF
        cmp    eax, 8
        je     NEAR PTR loc_5_0CD7
        cmp    eax, 0dh
        je     NEAR PTR loc_5_0D11
        mov    eax, 1
        jmp    NEAR PTR loc_5_0D28
loc_5_0901:
        mov    eax, 0fffeh
        mov    ecx, 0ffffh
        mov    word ptr [ebp - 54h], ax
        mov    word ptr [ebp - 52h], ax
        mov    word ptr [ebp - 50h], ax
        mov    word ptr [ebp - 4eh], ax
        mov    word ptr [ebp - 4ch], cx
        mov    word ptr [ebp - 60h], ax
        mov    word ptr [ebp - 5eh], ax
        mov    word ptr [ebp - 5ch], ax
        mov    word ptr [ebp - 5ah], ax
        mov    word ptr [ebp - 58h], ax
        mov    word ptr [ebp - 56h], cx
        mov    word ptr [ebp - 48h], ax
        mov    word ptr [ebp - 46h], ax
        mov    word ptr [ebp - 44h], ax
        mov    word ptr [ebp - 42h], ax
        mov    word ptr [ebp - 40h], cx
        push   0
        mov    word ptr [ebp - 3ch], ax
        mov    esi, dword ptr [ebp + 10h]
        mov    word ptr [ebp - 3ah], ax
        push   esi
        mov    word ptr [ebp - 38h], ax
        lea    eax, [ebp - 158h]
        mov    word ptr [ebp - 36h], cx
        push   eax
        call   sub_5_2680
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_5_0D28
        mov    eax, 0ffffh
        mov    word ptr [ebp - 4], ax
        mov    word ptr [ebp - 6], ax
        mov    word ptr [ebp - 0ch], ax
        movzx  eax, word ptr [ebp - 0fch]
        cmp    eax, 1
        je     SHORT loc_5_09AD
        cmp    eax, 2
        je     SHORT loc_5_09D1
        cmp    eax, 3
        je     NEAR PTR loc_5_0A25
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0D28
loc_5_09AD:
        cmp    word ptr [ebp - 0d2h], 10h
        je     SHORT loc_5_09C1
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0D28
loc_5_09C1:
        mov    ax, word ptr [ebp - 0fah]
        mov    word ptr [ebp - 2], ax
        jmp    NEAR PTR loc_5_0A70
loc_5_09D1:
        cmp    word ptr [ebp - 0d2h], 10h
        je     SHORT loc_5_09E5
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0D28
loc_5_09E5:
        mov    ax, word ptr [ebp - 0fah]
        mov    word ptr [ebp - 2], ax
        movzx  eax, word ptr [ebp - 0d0h]
        cmp    eax, 2
        je     SHORT loc_5_0A0B
        cmp    eax, 4
        je     SHORT loc_5_0A18
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0D28
loc_5_0A0B:
        mov    ax, word ptr [ebp - 0f8h]
        mov    word ptr [ebp - 6], ax
        jmp    SHORT loc_5_0A70
loc_5_0A18:
        mov    ax, word ptr [ebp - 0f8h]
        mov    word ptr [ebp - 4], ax
        jmp    SHORT loc_5_0A70
loc_5_0A25:
        cmp    word ptr [ebp - 0d2h], 10h
        jne    NEAR PTR loc_5_0C79
        cmp    word ptr [ebp - 0d0h], 4
        jne    NEAR PTR loc_5_0C79
        cmp    word ptr [ebp - 0ceh], 2
        jne    NEAR PTR loc_5_0C79
        mov    ax, word ptr [ebp - 0fah]
        mov    cx, word ptr [ebp - 0f8h]
        mov    dx, word ptr [ebp - 0f6h]
        mov    word ptr [ebp - 2], ax
        mov    word ptr [ebp - 4], cx
        mov    word ptr [ebp - 6], dx
loc_5_0A70:
        cmp    word ptr [ebp - 0aah], 1
        jb     NEAR PTR loc_5_0C6F
        cmp    word ptr [ebp - 9ah], 1
        jb     NEAR PTR loc_5_0C6F
        movzx  ax, byte ptr [ebp - 0a8h]
        cmp    word ptr [ebp - 6], 0ffffh
        mov    word ptr [ebp - 8], ax
        je     SHORT loc_5_0AC0
        cmp    word ptr [ebp - 0aah], 2
        jae    SHORT loc_5_0AB4
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0D28
loc_5_0AB4:
        movzx  ax, byte ptr [ebp - 0a7h]
        mov    word ptr [ebp - 0ch], ax
loc_5_0AC0:
        movzx  ax, byte ptr [ebp - 98h]
        cmp    word ptr [ebp - 2], 0ffffh
        mov    word ptr [ebp - 0ah], ax
        jne    SHORT loc_5_0ADE
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0D28
loc_5_0ADE:
        cmp    word ptr [ebp - 4], 0ffffh
        je     SHORT loc_5_0AEE
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [ebp - 4], ax
loc_5_0AEE:
        xor    edi, edi
        lea    eax, [ebp - 34h]
        push   edi
        lea    ecx, [ebp - 10h]
        push   edi
        mov    dword ptr [ebp - 34h], edi
        push   eax
        push   ecx
        push   esi
        call   sub_5_2678
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_5_0B0D
        mov    dword ptr [ebp - 10h], edi
loc_5_0B0D:
        mov    eax, dword ptr [ebp - 10h]
        cmp    eax, 8
        je     SHORT loc_5_0B21
        cmp    eax, 10h
        je     SHORT loc_5_0B28
        cmp    eax, 40h
        je     SHORT loc_5_0B2F
        jmp    SHORT loc_5_0B34
loc_5_0B21:
        mov    edi, 20h
        jmp    SHORT loc_5_0B34
loc_5_0B28:
        mov    edi, 10h
        jmp    SHORT loc_5_0B34
loc_5_0B2F:
        mov    edi, 40h
loc_5_0B34:
        mov    ax, word ptr [ebp - 6]
        sub    ax, 0ffffh
        cmp    ax, 1
        sbb    ebx, ebx
        inc    ebx
        push   ebx
        push   esi
        call   sub_5_016A
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_5_0D28
        push   edi
        lea    eax, [ebp - 3ch]
        push   eax
        lea    ecx, [ebp - 48h]
        push   ecx
        lea    eax, [ebp - 60h]
        push   ebx
        lea    ecx, [ebp - 54h]
        push   eax
        push   ecx
        push   esi
        call   sub_5_052B
        add    esp, 1ch
        test   eax, eax
        jne    NEAR PTR loc_5_0D28
        lea    eax, [ebp - 60h]
        lea    ecx, [ebp - 54h]
        push   eax
        push   ecx
        lea    eax, [ebp - 3ch]
        lea    ecx, [ebp - 48h]
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 6]
        push   edi
        push   20h
        push   eax
        push   ecx
        push   dword ptr [ebp - 0ah]
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 2]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0BA3:
        add    esp, 34h
        mov    edi, eax
        test   edi, edi
        jne    NEAR PTR loc_5_0C5B
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [ebp - 4]
        mov    dword ptr [ebp - 28h], esi
        sub    ax, 0ffffh
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [ebp - 30h], eax
        push   dword ptr [ebp - 30h]
        push   dword ptr [ebp - 2ch]
        push   dword ptr [ebp - 28h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 001h, 000h, 04Ah, 004h
        add    esp, 0ch
        mov    dword ptr [ebp - 24h], ebx
        mov    dword ptr [ebp - 1ch], esi
        db  0C7h, 045h, 0E0h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp - 1ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 001h, 000h, 04Ah, 004h
        add    esp, 0ch
        test   edi, edi
        jne    SHORT loc_5_0C68
        cmp    word ptr [ebp - 8], 9
        mov    eax, 2
        je     SHORT loc_5_0C1D
        movzx  eax, word ptr [ebp - 8]
loc_5_0C1D:
        movzx  ecx, word ptr [ebp - 0ah]
        movzx  edx, word ptr [ebp - 2]
        push   ecx
        push   eax
        push   edx
        lea    ecx, [ebp - 80h]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_5_2670
        add    esp, 14h
        lea    ecx, [ebp - 80h]
        db  0C7h, 045h, 0ECh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 18h], ecx
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 14h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 002h, 000h, 04Ah, 004h
        add    esp, 8
loc_5_0C5B:
        test   edi, edi
        jne    SHORT loc_5_0C68
        push   esi
        call   sub_5_1E0D
        add    esp, 4
loc_5_0C68:
        mov    eax, edi
        jmp    NEAR PTR loc_5_0D28
loc_5_0C6F:
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0D28
loc_5_0C79:
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0D28
loc_5_0C83:
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        je     SHORT loc_5_0C99
        cmp    eax, 1
        je     SHORT loc_5_0CB4
        mov    eax, 1
        jmp    NEAR PTR loc_5_0D28
loc_5_0C99:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0CA1:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 13h
        jmp    SHORT loc_5_0D28
loc_5_0CB4:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0CBC:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 13h
        jmp    SHORT loc_5_0D28
loc_5_0CCF:
        cmp    dword ptr [ebp + 0ch], 1
        jne    SHORT loc_5_0D11
        jmp    SHORT loc_5_0D26
loc_5_0CD7:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 7fffffffh
        cmp    eax, 4
        jb     SHORT loc_5_0D0A
        cmp    eax, 5
        jbe    SHORT loc_5_0CF5
        cmp    eax, 6
        jb     SHORT loc_5_0D0A
        cmp    eax, 8
        jbe    SHORT loc_5_0CFF
        jmp    SHORT loc_5_0D0A
loc_5_0CF5:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0CFD:
        jmp    SHORT loc_5_0D07
loc_5_0CFF:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0D07:
        add    esp, 4
loc_5_0D0A:
        mov    eax, 1
        jmp    SHORT loc_5_0D28
loc_5_0D11:
        mov    esi, dword ptr [ebp + 10h]
        push   esi
        call   sub_5_1EBC
        add    esp, 4
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0D23:
        add    esp, 4
loc_5_0D26:
        xor    eax, eax
loc_5_0D28:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  0CCh
sub_5_0D30:
        push   eax
        push   edx
        push   ebx
        push   ecx
        movzx  edx, word ptr [edi + 8]
        mov    ebx, 105h
        mov    al, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0D44:
        mov    ebx, 4
        mov    al, 60h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0D50:
        mov    ebx, 104h
        mov    al, 3fh
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0D5C:
        mov    ebx, 8
        xor    al, al
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0D68:
        mov    ebx, 0bdh
        mov    al, 0c0h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0D74:
        mov    ecx, 15h
        mov    ebx, 40h
        mov    al, 3fh
loc_5_0D80:
        xor    bh, bh
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0D87:
        mov    bh, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0D8E:
        inc    bl
        loop   loc_5_0D80
        mov    ecx, 8
        mov    ebx, 0b0h
        xor    eax, eax
loc_5_0D9E:
        xor    bh, bh
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0DA5:
        mov    bh, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0DAC:
        inc    bl
        loop   loc_5_0D9E
        mov    ebx, 104h
        xor    eax, eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0DBC:
        mov    ebx, 105h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0DC6:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0DCB:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0DD0:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0DD5:
        pop    ecx
        pop    ebx
        pop    edx
        pop    eax
        ret
        db  08Bh, 0C0h
loc_5_0DDC:
        push   eax
        mov    eax, dword ptr [esi + 33h]
        cmp    eax, ebx
        je     SHORT loc_5_0DFC
        or     eax, eax
        jne    SHORT loc_5_0E0F
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 1
        call   sub_5_1251
        pop    edx
        pop    ecx
        pop    ebx
        jb     SHORT loc_5_0E0F
loc_5_0DFC:
        pop    eax
        cmp    ecx, 4
        jbe    SHORT loc_5_0E08
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_5_0E08:
        je     SHORT loc_5_0E0D
        in     al, dx
        jmp    SHORT loc_5_0E0E
loc_5_0E0D:
        out    dx, al
loc_5_0E0E:
        ret
loc_5_0E0F:
        mov    eax, 1
        mov    edi, esi
        call   sub_5_1036
        pop    eax
        xor    eax, eax
        dec    eax
        ret
loc_5_0E20:
        push   eax
        mov    eax, dword ptr [esi + 43h]
        cmp    eax, ebx
        je     SHORT loc_5_0E40
        or     eax, eax
        jne    SHORT loc_5_0E53
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 4
        call   sub_5_1251
        pop    edx
        pop    ecx
        pop    ebx
        jb     SHORT loc_5_0E53
loc_5_0E40:
        pop    eax
        cmp    ecx, 4
        jbe    SHORT loc_5_0E4C
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_5_0E4C:
        je     SHORT loc_5_0E51
        in     al, dx
        jmp    SHORT loc_5_0E52
loc_5_0E51:
        out    dx, al
loc_5_0E52:
        ret
loc_5_0E53:
        mov    eax, 4
        mov    edi, esi
        call   sub_5_1036
        pop    eax
        xor    eax, eax
        dec    eax
        ret
loc_5_0E64:
        push   eax
        mov    eax, dword ptr [esi + 3bh]
        cmp    eax, ebx
        je     SHORT loc_5_0E89
        or     eax, eax
        jne    SHORT loc_5_0E9C
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 2
        call   sub_5_1251
        pop    edx
        pop    ecx
        pop    ebx
        jb     SHORT loc_5_0E9C
        call   sub_5_0D30
loc_5_0E89:
        pop    eax
        cmp    ecx, 4
        jbe    SHORT loc_5_0E95
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_5_0E95:
        je     SHORT loc_5_0E9A
        in     al, dx
        jmp    SHORT loc_5_0E9B
loc_5_0E9A:
        out    dx, al
loc_5_0E9B:
        ret
loc_5_0E9C:
        mov    eax, 2
        mov    edi, esi
        call   sub_5_1036
        pop    eax
        xor    eax, eax
        dec    eax
        ret
sub_5_0EAD:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 0fh
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_0EBB:
        cmp    ecx, 3
        jbe    SHORT loc_5_0ED0
        cmp    ecx, 8
        je     SHORT loc_5_0ED0
        cmp    ecx, 9
        je     SHORT loc_5_0ED0
        db  0CDh, 020h, 098h, 000h, 001h, 000h
loc_5_0ED0:
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_0EDB
        inc    cx
        jmp    SHORT loc_5_0EBB
loc_5_0EDB:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_0EE1:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 0fh
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_0EEF:
        cmp    ecx, 3
        jbe    SHORT loc_5_0F04
        cmp    ecx, 8
        je     SHORT loc_5_0F04
        cmp    ecx, 9
        je     SHORT loc_5_0F04
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
loc_5_0F04:
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_0F0F
        inc    cx
        jmp    SHORT loc_5_0EEF
loc_5_0F0F:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_0F15:
        push   eax
        push   ebx
        push   edx
        movzx  edx, word ptr [edi + 8]
        mov    ebx, 4
        mov    eax, 60h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0F2B:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0F30:
        pop    edx
        pop    ebx
        pop    eax
        ret
sub_5_0F34:
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
loc_5_0F7A:
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_0F8B
        inc    cx
        jmp    SHORT loc_5_0F7A
loc_5_0F8B:
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_0F90:
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
loc_5_0FD7:
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_0FE8
        inc    cx
        jmp    SHORT loc_5_0FD7
loc_5_0FE8:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_0FEE:
        push   esi
        push   eax
        push   ecx
        mov    si, 1
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 0ah]
loc_5_0FFB:
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_100C
        inc    cx
        jmp    SHORT loc_5_0FFB
loc_5_100C:
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_1011:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 1
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 0ah]
loc_5_101F:
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_1030
        inc    cx
        jmp    SHORT loc_5_101F
loc_5_1030:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_1036:
        test   word ptr [edi + 4], 1
        jne    SHORT loc_5_10AA
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1043:
        jb     SHORT loc_5_10AA
        cmp    eax, 2
        je     SHORT loc_5_1069
        cmp    eax, 4
        je     SHORT loc_5_1083
        test   dword ptr [esi], 1
        jne    SHORT loc_5_10AA
        or     dword ptr [esi], 1
        db  00Fh, 0B7h, 00Dh, 00Ch, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        jmp    SHORT loc_5_109B
loc_5_1069:
        test   dword ptr [esi], 2
        jne    SHORT loc_5_10AA
        or     dword ptr [esi], 2
        db  00Fh, 0B7h, 00Dh, 00Eh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        jmp    SHORT loc_5_109B
loc_5_1083:
        test   dword ptr [esi], 4
        jne    SHORT loc_5_10AA
        or     dword ptr [esi], 4
        db  00Fh, 0B7h, 00Dh, 010h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
loc_5_109B:
        mov    eax, 30h
        xor    esi, esi
        xor    edi, edi
        db  0CDh, 020h, 004h, 000h, 017h, 000h
loc_5_10AA:
        ret
loc_5_10AB:
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_1107
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        sti
        je     SHORT loc_5_1107
loc_5_10BF:
        push   eax
        mov    edi, dword ptr [eax]
        xor    eax, eax
        cmp    dword ptr [edi + 33h], ebx
        jne    SHORT loc_5_10D3
        or     eax, 1
        mov    dword ptr [edi + 37h], 0ffffffffh
loc_5_10D3:
        cmp    dword ptr [edi + 3bh], ebx
        jne    SHORT loc_5_10E2
        or     eax, 2
        mov    dword ptr [edi + 3fh], 0ffffffffh
loc_5_10E2:
        cmp    dword ptr [edi + 43h], ebx
        jne    SHORT loc_5_10F1
        or     eax, 4
        mov    dword ptr [edi + 47h], 0ffffffffh
loc_5_10F1:
        or     eax, eax
        je     SHORT loc_5_10FA
        call   sub_5_117F
loc_5_10FA:
        pop    eax
        cli
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        sti
        je     SHORT loc_5_1107
        jmp    SHORT loc_5_10BF
loc_5_1107:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        xor    esi, esi
        xchg   dword ptr [edi + ebx], esi
        or     esi, esi
        je     SHORT loc_5_111C
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_5_111C:
        clc
        ret
loc_5_111E:
        pushad
        pushfd
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    eax, 5
        mov    ecx, 24h
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_5_1177
        mov    dword ptr [edi + ebx], esi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_117B
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        sti
        je     SHORT loc_5_117B
loc_5_114F:
        mov    edi, dword ptr [eax]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1156:
        jb     SHORT loc_5_1164
        cli
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        sti
        je     SHORT loc_5_117B
        jmp    SHORT loc_5_114F
loc_5_1164:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     SHORT loc_5_1177
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_5_1177:
        popfd
        stc
        jmp    SHORT loc_5_117D
loc_5_117B:
        popfd
        clc
loc_5_117D:
        popad
        ret
sub_5_117F:
        push   eax
        push   edx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1187:
        jb     NEAR PTR loc_5_124C
        test   eax, 4
        je     SHORT loc_5_11B1
        cmp    dword ptr [edi + 43h], ebx
        jne    SHORT loc_5_11B1
        movzx  edx, word ptr [edi + 6]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_11A2:
        call   sub_5_0FEE
        mov    dword ptr [edi + 43h], 0
        and    dword ptr [esi], 0fffffffbh
loc_5_11B1:
        test   eax, 2
        je     SHORT loc_5_11D1
        cmp    dword ptr [edi + 3bh], ebx
        jne    SHORT loc_5_11D1
        call   sub_5_0F15
        call   sub_5_0F34
        mov    dword ptr [edi + 3bh], 0
        and    dword ptr [esi], 0fffffffdh
loc_5_11D1:
        test   eax, 1
        je     SHORT loc_5_120D
        cmp    dword ptr [edi + 33h], ebx
        jne    SHORT loc_5_120D
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_5_11EE
        mov    eax, dword ptr [edi + 4bh]
        db  0CDh, 020h, 014h, 000h, 004h, 000h
loc_5_11EE:
        call   sub_5_0EAD
        movzx  edx, word ptr [edi + 6]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_11FC:
        jb     SHORT loc_5_1203
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1203:
        mov    dword ptr [edi + 33h], 0
        and    dword ptr [esi], 0fffffffeh
loc_5_120D:
        cmp    word ptr [edi + 0ah], -1
        je     SHORT loc_5_1230
        mov    eax, dword ptr [edi + 43h]
        or     eax, eax
        jne    SHORT loc_5_1230
        mov    eax, dword ptr [edi + 2bh]
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        db  0CDh, 020h, 003h, 000h, 003h, 000h
loc_5_1230:
        mov    eax, dword ptr [edi + 33h]
        or     eax, eax
        jne    SHORT loc_5_124C
        mov    eax, dword ptr [edi + 2fh]
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        db  0CDh, 020h, 003h, 000h, 003h, 000h
loc_5_124C:
        pop    esi
        pop    edx
        pop    eax
        clc
        ret
sub_5_1251:
        push   edx
        push   ecx
        push   eax
        test   eax, 4
        je     SHORT loc_5_127A
        cmp    word ptr [edi + 0ah], -1
        je     SHORT loc_5_12B6
        cmp    dword ptr [edi + 43h], 0
        je     SHORT loc_5_126F
        cmp    dword ptr [edi + 43h], ebx
        je     SHORT loc_5_127A
        jmp    SHORT loc_5_12B6
loc_5_126F:
        mov    dword ptr [edi + 43h], ebx
        call   sub_5_1011
        mov    dword ptr [edi + 47h], ebx
loc_5_127A:
        test   eax, 2
        je     SHORT loc_5_12A0
        cmp    word ptr [edi + 8], -1
        je     SHORT loc_5_12B6
        cmp    dword ptr [edi + 3bh], 0
        je     SHORT loc_5_1295
        cmp    dword ptr [edi + 3bh], ebx
        je     SHORT loc_5_12A0
        jmp    SHORT loc_5_12B6
loc_5_1295:
        mov    dword ptr [edi + 3bh], ebx
        call   sub_5_0F90
        mov    dword ptr [edi + 3fh], ebx
loc_5_12A0:
        test   eax, 1
        je     SHORT loc_5_12EB
        cmp    dword ptr [edi + 33h], 0
        je     SHORT loc_5_12BC
        cmp    dword ptr [edi + 33h], ebx
        je     NEAR PTR loc_5_1343
loc_5_12B6:
        stc
        jmp    NEAR PTR loc_5_1344
loc_5_12BC:
        mov    dword ptr [edi + 33h], ebx
        cmp    dword ptr [edi + 37h], ebx
        je     SHORT loc_5_12CD
        movzx  edx, word ptr [edi + 6]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_12CD:
        mov    dword ptr [edi + 37h], ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_12D5:
        call   sub_5_0EE1
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_5_12EB
        mov    eax, dword ptr [edi + 4bh]
        db  0CDh, 020h, 013h, 000h, 004h, 000h
loc_5_12EB:
        mov    eax, dword ptr [esp]
        test   eax, 5
        je     SHORT loc_5_1343
        push   eax
        test   eax, 4
        je     SHORT loc_5_131C
        mov    eax, dword ptr [edi + 2bh]
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   ecx, 10h
        je     SHORT loc_5_1316
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        jmp    SHORT loc_5_131C
loc_5_1316:
        db  0CDh, 020h, 009h, 000h, 003h, 000h
loc_5_131C:
        pop    eax
        test   eax, 1
        je     SHORT loc_5_1343
        mov    eax, dword ptr [edi + 2fh]
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   ecx, 10h
        je     SHORT loc_5_133D
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        jmp    SHORT loc_5_1343
loc_5_133D:
        db  0CDh, 020h, 009h, 000h, 003h, 000h
loc_5_1343:
        clc
loc_5_1344:
        pop    eax
        pop    ecx
        pop    edx
        ret
sub_5_1348:
        mov    eax, 400h
        clc
        ret
sub_5_134F:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_135D:
        add    esp, 8
        xor    eax, eax
        or     edi, edi
        je     SHORT loc_5_136C
        mov    eax, dword ptr [edi + 80h]
loc_5_136C:
        pop    edi
        pop    ebp
        ret
sub_5_136F:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_137D:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_138D
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [edi + 80h], eax
loc_5_138D:
        pop    edi
        pop    ebp
        ret
AUDDRV_PM_API:
        movzx  eax, word ptr [ebp + 14h]
        cmp    ah, 4
        jae    SHORT loc_5_13BA
        movzx  ecx, ah
        shl    ecx, 3
        db  08Dh, 089h, 000h, 000h, 000h, 000h
        movzx  eax, al
        cmp    eax, dword ptr [ecx]
        jae    SHORT loc_5_13BA
        mov    ecx, dword ptr [ecx + 4]
        call   dword ptr [ecx + eax*4]
        jb     SHORT loc_5_13BA
        and    word ptr [ebp + 2ch], 0fffeh
        ret
loc_5_13BA:
        or     word ptr [ebp + 2ch], 1
        ret
loc_5_13C0:
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_13CC:
        add    esp, 8
        jb     SHORT loc_5_13F2
        movzx  eax, word ptr [ebp + 10h]
        test   eax, 0ffffffffh
        jne    SHORT loc_5_13F2
        mov    eax, dword ptr [edi + 4bh]
        or     eax, eax
        je     SHORT loc_5_13F2
        push   ecx
        db  0CDh, 020h, 01Ch, 000h, 004h, 000h
        mov    word ptr [ebp + 1ch], cx
        pop    ecx
        clc
        jmp    SHORT loc_5_13F9
loc_5_13F2:
        mov    word ptr [ebp + 1ch], 0ffffh
        stc
loc_5_13F9:
        pop    edi
        ret
loc_5_13FB:
        call   sub_5_1348
        mov    word ptr [ebp + 1ch], ax
        clc
        ret
loc_5_1406:
        movzx  eax, word ptr [ebp + 10h]
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_1448
        test   eax, 1
        je     SHORT loc_5_142D
        push   2
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1421:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_1448
        mov    ecx, dword ptr [edi + 37h]
        jmp    SHORT loc_5_1450
loc_5_142D:
        test   eax, 2
        je     SHORT loc_5_1448
        push   3
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_143C:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_1448
        mov    ecx, dword ptr [edi + 3fh]
        jmp    SHORT loc_5_1450
loc_5_1448:
        mov    word ptr [ebp + 1ch], 1
loc_5_144E:
        stc
        ret
loc_5_1450:
        mov    word ptr [ebp + 1ch], 0
        call   sub_5_1251
        jb     SHORT loc_5_145E
        ret
loc_5_145E:
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_5_144E
loc_5_1466:
        movzx  eax, word ptr [ebp + 10h]
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_14C2
        test   eax, 1
        je     SHORT loc_5_14A7
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_5_1493
        push   2
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1489:
        add    esp, 8
        or     edi, edi
        jne    SHORT loc_5_14A2
        add    edx, 4
loc_5_1493:
        push   2
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_149B:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_14C2
loc_5_14A2:
        mov    ecx, dword ptr [edi + 33h]
        jmp    SHORT loc_5_14CA
loc_5_14A7:
        test   eax, 2
        je     SHORT loc_5_14C2
        push   3
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_14B6:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_14C2
        mov    ecx, dword ptr [edi + 3bh]
        jmp    SHORT loc_5_14CA
loc_5_14C2:
        mov    word ptr [ebp + 1ch], 1
loc_5_14C8:
        stc
        ret
loc_5_14CA:
        cmp    ecx, ebx
        je     SHORT loc_5_14D6
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_5_14C8
loc_5_14D6:
        call   sub_5_117F
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
loc_5_14E3:
        pushfd
        mov    ax, word ptr [ebp + 1ch]
        and    ax, 0fffeh
        jne    SHORT loc_5_1554
        xor    ecx, ecx
        mov    ax, word ptr [ebp + 1ch]
        and    ax, 1
        je     SHORT loc_5_14FD
        mov    ecx, dword ptr [ebp + 18h]
loc_5_14FD:
        cli
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_1554
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_5_1554
        mov    edi, dword ptr [eax]
        jecxz  loc_5_1525
loc_5_1514:
        cmp    dword ptr [edi + 53h], ecx
        je     SHORT loc_5_1525
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_5_1554
        mov    edi, dword ptr [eax]
        jmp    SHORT loc_5_1514
loc_5_1525:
        popfd
        pushfd
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cmp    edi, -1
        je     SHORT loc_5_1554
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     SHORT loc_5_1554
        cmp    ecx, 86h
        ja     SHORT loc_5_1554
        cld
        rep movsb
        mov    word ptr [ebp + 1ch], 1
        popfd
        clc
        ret
loc_5_1554:
        mov    word ptr [ebp + 1ch], 0
        popfd
        stc
        ret
loc_5_155D:
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_1592
        push   3
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_156D:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_1592
        mov    ecx, dword ptr [edi + 3fh]
        mov    word ptr [ebp + 1ch], 0
        mov    eax, 2
        call   sub_5_1251
        jb     SHORT loc_5_158A
        ret
loc_5_158A:
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_5_1598
loc_5_1592:
        mov    word ptr [ebp + 1ch], 1
loc_5_1598:
        stc
        ret
loc_5_159A:
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_15D2
        push   3
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_15AA:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_15D2
        mov    ecx, dword ptr [edi + 3bh]
        cmp    ecx, ebx
        je     SHORT loc_5_15C0
        mov    word ptr [ebp + 1ch], 3
        jmp    SHORT loc_5_15D8
loc_5_15C0:
        mov    eax, 2
        call   sub_5_117F
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
loc_5_15D2:
        mov    word ptr [ebp + 1ch], 1
loc_5_15D8:
        stc
        ret
loc_5_15DA:
        pushfd
        mov    ax, word ptr [ebp + 1ch]
        cmp    ax, 1
        jne    SHORT loc_5_1650
        mov    ecx, dword ptr [ebp + 18h]
        cli
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_1650
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_5_1650
loc_5_15FB:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 53h], ecx
        je     SHORT loc_5_160C
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_5_1650
        jmp    SHORT loc_5_15FB
loc_5_160C:
        popfd
        pushfd
        cmp    word ptr [edi + 8], -1
        je     SHORT loc_5_1650
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cmp    edi, -1
        je     SHORT loc_5_1650
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     SHORT loc_5_1650
        cmp    ecx, 1ch
        ja     SHORT loc_5_1650
        mov    word ptr [edi + 4], 1
        mov    ax, word ptr [esi + 8]
        mov    word ptr [edi + 6], ax
        mov    eax, dword ptr [esi + 53h]
        mov    dword ptr [edi + 0ch], eax
        mov    word ptr [ebp + 1ch], 1
        popfd
        clc
        ret
loc_5_1650:
        mov    word ptr [ebp + 1ch], 0
        popfd
        stc
        ret
loc_5_1659:
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
loc_5_16B1:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_16DD
        lea    eax, [edi + 4fh]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   eax
        push   dword ptr [esi]
        call   sub_5_1912
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_5_16DD
        mov    word ptr [ebp + 1ch], ax
        shr    eax, 10h
        mov    word ptr [ebp + 14h], ax
        clc
        ret
loc_5_16DD:
        mov    word ptr [ebp + 1ch], 0
        mov    word ptr [ebp + 14h], 0
        stc
        ret
loc_5_16EB:
        xchg   esi, eax
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        mov    eax, dword ptr [esi]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1701:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_1717
        lea    eax, [edi + 4fh]
        push   dword ptr [esi + 4]
        push   eax
        call   sub_5_1AA2
        add    esp, 8
loc_5_1717:
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
loc_5_171F:
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_1754
        push   4
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_172F:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_1754
        mov    ecx, dword ptr [edi + 47h]
        mov    word ptr [ebp + 1ch], 0
        mov    eax, 4
        call   sub_5_1251
        jb     SHORT loc_5_174C
        ret
loc_5_174C:
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_5_175A
loc_5_1754:
        mov    word ptr [ebp + 1ch], 1
loc_5_175A:
        stc
        ret
loc_5_175C:
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_1794
        push   4
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_176C:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_1794
        mov    ecx, dword ptr [edi + 43h]
        cmp    ecx, ebx
        je     SHORT loc_5_1782
        mov    word ptr [ebp + 1ch], 3
        jmp    SHORT loc_5_179A
loc_5_1782:
        mov    eax, 4
        call   sub_5_117F
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
loc_5_1794:
        mov    word ptr [ebp + 1ch], 1
loc_5_179A:
        stc
        ret
loc_5_179C:
        pushfd
        mov    ax, word ptr [ebp + 1ch]
        cmp    ax, 1
        jne    SHORT loc_5_1818
        mov    ecx, dword ptr [ebp + 18h]
        cli
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_1818
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_5_1818
loc_5_17BD:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 53h], ecx
        je     SHORT loc_5_17CE
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_5_1818
        jmp    SHORT loc_5_17BD
loc_5_17CE:
        popfd
        pushfd
        cmp    word ptr [edi + 0ah], -1
        je     SHORT loc_5_1818
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cmp    edi, -1
        je     SHORT loc_5_1818
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     SHORT loc_5_1818
        cmp    ecx, 24h
        ja     SHORT loc_5_1818
        mov    word ptr [edi + 4], 0
        mov    ax, word ptr [esi + 0ah]
        mov    word ptr [edi + 6], ax
        mov    al, byte ptr [esi + 0ch]
        mov    byte ptr [edi + 8], al
        mov    eax, dword ptr [esi + 53h]
        mov    dword ptr [edi + 10h], eax
        mov    word ptr [ebp + 1ch], 1
        popfd
        clc
        ret
loc_5_1818:
        mov    word ptr [ebp + 1ch], 0
        popfd
        stc
        ret
sub_5_1821:
        push   ebp
        mov    ebp, esp
        sub    esp, 6ch
        mov    eax, 0ffffffffh
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   esi, esi
        je     NEAR PTR loc_5_190B
        lea    eax, [ebp - 6ch]
        push   eax
        call   sub_5_1B36
        add    esp, 4
        call   sub_5_1B24
        call   sub_5_1B73
        mov    edi, dword ptr [eax + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_1B65
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_1B65
        add    esp, 4
        mov    ebx, dword ptr [ebp + 10h]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_1B65
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_1B65
        add    esp, 4
        mov    ebx, dword ptr [ebp + 14h]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_1B65
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_1B65
        add    esp, 4
        mov    ebx, dword ptr [ebp + 18h]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_1B65
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_1B65
        movzx  ecx, si
        shr    esi, 10h
        add    esp, 4
        movzx  eax, si
        push   ecx
        push   eax
        call   sub_5_1B56
        add    esp, 8
        call   sub_5_1B30
        movzx  esi, word ptr [edi + 14h]
        shl    esi, 10h
        movzx  eax, word ptr [edi + 1ch]
        or     esi, eax
        call   sub_5_1B2A
        lea    ecx, [ebp - 6ch]
        push   ecx
        call   sub_5_1B46
        add    esp, 4
        mov    eax, esi
loc_5_190B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_1912:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        push   ebp
        mov    ebp, dword ptr [esi]
        test   ebp, ebp
        jne    SHORT loc_5_1938
        push   15h
        push   4
        call   sub_5_26CC
        mov    ebp, eax
        test   ebp, ebp
        jne    SHORT loc_5_1936
        xor    eax, eax
        jmp    NEAR PTR loc_5_1A9D
loc_5_1936:
        mov    dword ptr [esi], ebp
loc_5_1938:
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 8
        jbe    SHORT loc_5_1955
        mov    eax, dword ptr [esp + 1ch]
        mov    byte ptr [eax + 8], 0
loc_5_1955:
        push   ebp
        call   sub_5_26BC
        mov    ebx, eax
        test   ebx, ebx
        je     NEAR PTR loc_5_1A49
loc_5_1965:
        mov    ecx, dword ptr [esp + 1ch]
        lea    edx, [ebx + 0ch]
loc_5_196C:
        mov    al, byte ptr [ecx]
        cmp    al, byte ptr [edx]
        jne    SHORT loc_5_198C
        or     al, al
        je     SHORT loc_5_1988
        mov    al, byte ptr [ecx + 1]
        cmp    al, byte ptr [edx + 1]
        jne    SHORT loc_5_198C
        add    ecx, 2
        add    edx, 2
        or     al, al
        jne    SHORT loc_5_196C
loc_5_1988:
        xor    eax, eax
        jmp    SHORT loc_5_1991
loc_5_198C:
        sbb    eax, eax
        sbb    eax, -1
loc_5_1991:
        test   eax, eax
        je     SHORT loc_5_19A2
        push   ebx
        push   ebp
        call   sub_5_26AC
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_5_1965
loc_5_19A2:
        test   ebx, ebx
        je     NEAR PTR loc_5_1A49
        cmp    dword ptr [ebx + 4], 0
        je     SHORT loc_5_19B7
        xor    eax, eax
        jmp    NEAR PTR loc_5_1A9D
loc_5_19B7:
        push   0
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_1821
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_5_19E4
        push   ebx
        mov    dword ptr [ebx], 0
        push   esi
        call   sub_5_1AA2
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_5_1A9D
loc_5_19E4:
        mov    eax, dword ptr [esp + 20h]
        push   0
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebx + 4], ecx
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   ecx
        call   sub_5_1821
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_5_1A1B
        mov    dword ptr [ebx + 4], 0
        push   ebx
        push   esi
        call   sub_5_1AA2
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_5_1A9D
loc_5_1A1B:
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx + 8]
        push   10000h
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_1821
        add    esp, 14h
        push   dword ptr [ebx]
        push   dword ptr [ebx + 8]
        push   10000h
        push   ebx
        push   dword ptr [ebx + 4]
        call   sub_5_1821
        add    esp, 14h
        jmp    SHORT loc_5_1A9B
loc_5_1A49:
        push   ebp
        call   sub_5_2698
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_5_1A59
        xor    eax, eax
        jmp    SHORT loc_5_1A9D
loc_5_1A59:
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
        call   sub_5_2688
loc_5_1A9B:
        mov    eax, ebx
loc_5_1A9D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_1AA2:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        test   edi, edi
        je     SHORT loc_5_1AEE
        mov    eax, dword ptr [edi]
        test   eax, eax
        je     SHORT loc_5_1ACC
        push   0
        push   dword ptr [edi + 8]
        push   40000h
        push   edi
        push   eax
        call   sub_5_1821
        add    esp, 14h
        mov    dword ptr [edi], 0
loc_5_1ACC:
        mov    eax, dword ptr [edi + 4]
        test   eax, eax
        je     SHORT loc_5_1AEE
        push   0
        push   dword ptr [edi + 8]
        push   40000h
        push   edi
        push   eax
        call   sub_5_1821
        mov    dword ptr [edi + 4], 0
        add    esp, 14h
loc_5_1AEE:
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     SHORT loc_5_1B1F
        push   edi
        push   eax
        call   sub_5_2700
        push   edi
        push   dword ptr [esi]
        call   sub_5_26F0
        push   dword ptr [esi]
        call   sub_5_26BC
        test   eax, eax
        jne    SHORT loc_5_1B19
        push   dword ptr [esi]
        call   sub_5_26E0
loc_5_1B19:
        mov    dword ptr [esi], 0
loc_5_1B1F:
        pop    edi
        pop    esi
        ret
        db  0CCh, 0CCh
sub_5_1B24:
        db  0CDh, 020h, 083h, 080h, 001h, 000h
sub_5_1B2A:
        db  0CDh, 020h, 086h, 080h, 001h, 000h
sub_5_1B30:
        db  0CDh, 020h, 085h, 080h, 001h, 000h
sub_5_1B36:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        pop    ebp
        ret
sub_5_1B46:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        pop    ebp
        ret
sub_5_1B56:
        movzx  ecx, word ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        db  0CDh, 020h, 048h, 080h, 001h, 000h
sub_5_1B65:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        pop    ebp
        ret
sub_5_1B73:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
loc_5_1B7E:
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        push   edi
        mov    ecx, 10h
        db  0BFh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb
        je     SHORT loc_5_1BB3
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        mov    esi, eax
        repe cmpsb
        mov    eax, 80004002h
        jne    SHORT loc_5_1BC1
loc_5_1BB3:
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_1BC1:
        pop    edi
        pop    esi
        ret    0ch
loc_5_1BC6:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0ch]
        inc    eax
        mov    dword ptr [ecx + 0ch], eax
        ret    4
loc_5_1BD4:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0ch]
        dec    eax
        mov    dword ptr [ecx + 0ch], eax
        ret    4
loc_5_1BE2:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        push   0ffh
        db  068h, 000h, 000h, 000h, 000h
        lea    esi, [edi + 104h]
        push   esi
        mov    dword ptr [edi], 9
        call   sub_5_2714
        add    esp, 0ch
        call   sub_5_2180
        push   0ffh
        lea    ecx, [edi + 4]
        push   eax
        push   ecx
        call   sub_5_2714
        mov    edx, dword ptr [esp + 18h]
        add    esp, 0ch
        xor    ecx, ecx
        mov    dword ptr [edi + 210h], ecx
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [edi + 204h], eax
        mov    dword ptr [edi + 20ch], ecx
        mov    word ptr [edi + 208h], 33f6h
        mov    eax, 0b3h
        cmp    byte ptr [esi], cl
        je     SHORT loc_5_1C56
loc_5_1C4B:
        movsx  ecx, byte ptr [esi]
        add    eax, ecx
        inc    esi
        cmp    byte ptr [esi], 0
        jne    SHORT loc_5_1C4B
loc_5_1C56:
        mov    ecx, 0f7h
        sub    edx, edx
        div    ecx
        mov    word ptr [edi + 20ah], dx
        xor    eax, eax
        pop    edi
        pop    esi
        ret    8
loc_5_1C6D:
        push   esi
        push   edi
        mov    edi, 8878000ah
        mov    esi, dword ptr [esp + 0ch]
        cmp    dword ptr [esi + 0ch], 0
        jne    SHORT loc_5_1C92
        push   dword ptr [esi + 4]
        call   sub_5_2264
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_5_1C92
        xor    edi, edi
        inc    dword ptr [esi + 0ch]
loc_5_1C92:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    4
loc_5_1C99:
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
loc_5_1CE4:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   ebp
        mov    edi, 8007000eh
        mov    esi, dword ptr [esp + 2ch]
        test   byte ptr [esp + 1ch], 0f0h
        mov    dword ptr [esi], ebx
        je     SHORT loc_5_1D06
        mov    eax, 8878001eh
        jmp    NEAR PTR loc_5_1DCC
loc_5_1D06:
        mov    ebp, dword ptr [esp + 14h]
        cmp    dword ptr [ebp + 10h], 0
        jne    NEAR PTR loc_5_1DAB
        push   201h
        push   31h
        call   sub_5_2724
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_5_1DA4
        db  0C7h, 003h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 4], ebp
        push   dword ptr [ebp + 4]
        push   ebx
        call   sub_5_22A9
        add    esp, 8
        mov    eax, dword ptr [ebx + 14h]
        cmp    eax, 1000h
        jbe    SHORT loc_5_1DB0
        cmp    eax, 0ffffh
        jae    SHORT loc_5_1DB0
        cmp    dword ptr [ebx + 0ch], 0
        je     SHORT loc_5_1DB0
        cmp    dword ptr [ebx + 10h], 0
        je     SHORT loc_5_1DB0
        cmp    dword ptr [ebx + 18h], 0
        je     SHORT loc_5_1DB0
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 2ch]
        push   ebx
        xor    edi, edi
        call   sub_5_1F24
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
        call   sub_5_22F0
        add    esp, 14h
        jmp    SHORT loc_5_1DB0
loc_5_1DA4:
        mov    edi, 8007000eh
        jmp    SHORT loc_5_1DB0
loc_5_1DAB:
        mov    edi, 8878000ah
loc_5_1DB0:
        test   edi, edi
        je     SHORT loc_5_1DC5
        test   ebx, ebx
        je     SHORT loc_5_1DCA
        push   0
        push   ebx
        call   sub_5_271C
        add    esp, 8
        jmp    SHORT loc_5_1DCA
loc_5_1DC5:
        mov    dword ptr [esi], ebx
        inc    dword ptr [ebp + 10h]
loc_5_1DCA:
        mov    eax, edi
loc_5_1DCC:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    1ch
loc_5_1DD3:
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], 0
        mov    eax, 80004001h
        ret    0ch
loc_5_1DE5:
        mov    eax, 80004005h
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 0ch]
        dec    edx
        mov    dword ptr [ecx + 0ch], edx
        jne    SHORT loc_5_1E0A
        cmp    dword ptr [ecx + 10h], 0
        jne    SHORT loc_5_1E0A
        push   dword ptr [ecx + 4]
        call   sub_5_2244
        add    esp, 4
        xor    eax, eax
loc_5_1E0A:
        ret    4
sub_5_1E0D:
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
        call   sub_5_2724
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_5_1E8C
        mov    edi, dword ptr [ebp + 8]
        push   1
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 14h]
        push   ecx
        mov    dword ptr [esi + 4], edi
        db  0C7h, 006h, 000h, 000h, 000h, 000h
        call   sub_5_2740
        test   eax, eax
        jne    SHORT loc_5_1E8C
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
        jne    SHORT loc_5_1E8C
        push   esi
        mov    ebx, 1
        push   edi
        call   sub_5_136F
        add    esp, 8
loc_5_1E8C:
        test   ebx, ebx
        jne    SHORT loc_5_1EB5
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_5_1EA6
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 0bh]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_5_272C
loc_5_1EA6:
        test   esi, esi
        je     SHORT loc_5_1EB5
        push   0
        push   esi
        call   sub_5_271C
        add    esp, 8
loc_5_1EB5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_1EBC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_5_134F
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_5_1F1E
        push   0
        push   edi
        call   sub_5_136F
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
        call   sub_5_272C
        push   0
        push   esi
        call   sub_5_271C
        add    esp, 8
loc_5_1F1E:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_5_1F24:
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 14h]
        cmp    dword ptr [edx], eax
        jbe    SHORT loc_5_1F36
        mov    dword ptr [edx], eax
loc_5_1F36:
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
loc_5_1F56:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 20h]
        dec    esi
        mov    dword ptr [eax + 20h], esi
        jne    SHORT loc_5_1F7B
        cmp    dword ptr [eax + 8], 0
        jne    SHORT loc_5_1F7B
        mov    ecx, dword ptr [eax + 4]
        push   0
        push   eax
        dec    dword ptr [ecx + 10h]
        call   sub_5_271C
        add    esp, 8
loc_5_1F7B:
        mov    eax, esi
        pop    esi
        ret    4
loc_5_1F81:
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        push   edi
        mov    ecx, 10h
        db  0BFh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb
        je     SHORT loc_5_1FB6
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        mov    esi, eax
        repe cmpsb
        mov    eax, 80004002h
        jne    SHORT loc_5_1FC4
loc_5_1FB6:
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_1FC4:
        pop    edi
        pop    esi
        ret    0ch
loc_5_1FC9:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 20h]
        inc    eax
        mov    dword ptr [ecx + 20h], eax
        ret    4
loc_5_1FD7:
        mov    eax, 80004001h
        ret    20h
loc_5_1FDF:
        mov    eax, 80004001h
        ret    14h
loc_5_1FE7:
        mov    eax, dword ptr [esp + 8]
        test   eax, eax
        je     SHORT loc_5_2040
        cmp    word ptr [eax], 1
        jne    SHORT loc_5_2040
        mov    edx, dword ptr [eax + 4]
        cmp    edx, 0afc8h
        jae    SHORT loc_5_2040
        mov    cx, word ptr [eax + 0eh]
        cmp    cx, 8
        je     SHORT loc_5_2010
        cmp    cx, 10h
        jne    SHORT loc_5_2040
loc_5_2010:
        mov    ax, word ptr [eax + 2]
        cmp    ax, 1
        je     SHORT loc_5_2020
        cmp    ax, 2
        jne    SHORT loc_5_2040
loc_5_2020:
        movzx  eax, ax
        movzx  ecx, cx
        push   eax
        push   ecx
        mov    eax, dword ptr [esp + 0ch]
        push   edx
        movzx  ecx, word ptr [eax + 2eh]
        push   1
        push   ecx
        call   sub_5_22F0
        add    esp, 14h
        xor    eax, eax
        jmp    SHORT loc_5_2045
loc_5_2040:
        mov    eax, 88780064h
loc_5_2045:
        ret    8
loc_5_2048:
        mov    eax, dword ptr [esp + 8]
        test   eax, eax
        je     SHORT loc_5_206D
        cmp    eax, 0afc8h
        jae    SHORT loc_5_206D
        mov    ecx, dword ptr [esp + 4]
        push   eax
        movzx  edx, word ptr [ecx + 2eh]
        push   edx
        call   sub_5_2364
        add    esp, 8
        xor    eax, eax
        jmp    SHORT loc_5_2072
loc_5_206D:
        mov    eax, 88780064h
loc_5_2072:
        ret    8
loc_5_2075:
        mov    eax, 80004001h
        ret    8
loc_5_207D:
        mov    eax, 80004001h
        ret    8
loc_5_2085:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 8], 1
        jne    SHORT loc_5_20B7
        push   dword ptr [esi + 18h]
        call   sub_5_218E
        add    esp, 4
        inc    eax
        cmp    byte ptr [esi + 30h], 3
        jbe    SHORT loc_5_20A4
        add    eax, eax
loc_5_20A4:
        mov    ecx, dword ptr [esi + 1ch]
        mov    edx, dword ptr [esp + 10h]
        sub    ecx, eax
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx], ecx
        jmp    SHORT loc_5_20CB
loc_5_20B7:
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx], 0
loc_5_20CB:
        xor    eax, eax
        pop    esi
        ret    0ch
loc_5_20D1:
        push   esi
        mov    eax, 80004005h
        mov    esi, dword ptr [esp + 8]
        push   edi
        cmp    dword ptr [esi + 8], 0
        jne    SHORT loc_5_2146
        push   dword ptr [esi + 18h]
        movzx  edi, word ptr [esi + 2eh]
        call   sub_5_21A9
        add    esp, 4
        push   dword ptr [esi + 1ch]
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi + 18h]
        call   sub_5_21CD
        add    esp, 0ch
        push   dword ptr [esi + 18h]
        call   sub_5_21EE
        add    esp, 4
        push   dword ptr [esi + 18h]
        call   sub_5_21B7
        mov    ax, word ptr [esi + 1ch]
        add    esp, 4
        cmp    dword ptr [esi + 24h], 10h
        jne    SHORT loc_5_2125
        shr    ax, 1
loc_5_2125:
        cmp    dword ptr [esi + 28h], 2
        jne    SHORT loc_5_212E
        shr    ax, 1
loc_5_212E:
        dec    ax
        movzx  ecx, ax
        push   ecx
        push   edi
        call   sub_5_23C8
        add    esp, 8
        xor    eax, eax
        mov    dword ptr [esi + 8], 1
loc_5_2146:
        pop    edi
        pop    esi
        ret    10h
loc_5_214B:
        push   esi
        mov    eax, 80004005h
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 8], 0
        je     SHORT loc_5_217C
        movzx  eax, word ptr [esi + 2eh]
        push   eax
        call   sub_5_25A6
        add    esp, 4
        push   dword ptr [esi + 18h]
        call   sub_5_21A9
        add    esp, 4
        xor    eax, eax
        mov    dword ptr [esi + 8], 0
loc_5_217C:
        pop    esi
        ret    4
sub_5_2180:
        db  00Fh, 0B7h, 005h, 012h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        ret
sub_5_218E:
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
sub_5_21A9:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 018h, 000h, 004h, 000h
        pop    ebp
        ret
sub_5_21B7:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 019h, 000h, 004h, 000h
        pop    ebx
        pop    ebp
        ret
sub_5_21CD:
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
sub_5_21EE:
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
loc_5_2224:
        xor    eax, eax
        jae    SHORT loc_5_222D
        mov    eax, 1
loc_5_222D:
        pop    edx
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    ah, byte ptr [ebp + 0ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_223F:
        xor    ah, ah
        pop    edx
        pop    ebp
        ret
sub_5_2244:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2254:
        add    esp, 8
        mov    word ptr [edi + 84h], 0
        pop    ebx
        pop    edi
        pop    ebp
        ret
sub_5_2264:
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
loc_5_227A:
        add    esp, 8
        mov    eax, dword ptr [edi + 33h]
        or     eax, eax
        je     SHORT loc_5_2290
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        cmp    eax, ebx
        je     SHORT loc_5_2290
        jmp    SHORT loc_5_22A2
loc_5_2290:
        mov    word ptr [edi + 84h], 1
        movzx  edx, word ptr [edi + 6]
        mov    edx, 0
loc_5_22A2:
        mov    eax, edx
        pop    ebx
        pop    edx
        pop    edi
        pop    ebp
        ret
sub_5_22A9:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_22BC:
        add    esp, 8
        mov    ax, word ptr [edi + 6]
        mov    word ptr [esi + 2eh], ax
        mov    ah, byte ptr [edi + 0eh]
        mov    byte ptr [esi + 30h], ah
        mov    eax, dword ptr [edi + 1bh]
        mov    dword ptr [esi + 0ch], eax
        mov    eax, dword ptr [edi + 1fh]
        mov    dword ptr [esi + 10h], eax
        mov    eax, dword ptr [edi + 23h]
        mov    dword ptr [esi + 14h], eax
        mov    eax, dword ptr [edi + 4bh]
        mov    dword ptr [esi + 18h], eax
        xor    ax, ax
        mov    word ptr [esi + 2ch], ax
        pop    esi
        pop    edi
        pop    ebp
        ret
sub_5_22F0:
        push   ebp
        mov    ebp, esp
        push   eax
        push   edi
        cmp    word ptr [ebp + 0ch], 1
        jne    SHORT loc_5_235F
        cmp    word ptr [ebp + 10h], 0fa0h
        jb     SHORT loc_5_235F
        cmp    word ptr [ebp + 10h], 0afc8h
        ja     SHORT loc_5_235F
        cmp    word ptr [ebp + 14h], 8
        je     SHORT loc_5_231A
        cmp    word ptr [ebp + 14h], 10h
        jne    SHORT loc_5_235F
loc_5_231A:
        cmp    word ptr [ebp + 18h], 1
        jb     SHORT loc_5_235F
        cmp    word ptr [ebp + 18h], 2
        ja     SHORT loc_5_235F
        db  066h, 083h, 025h, 000h, 000h, 000h, 000h, 0F9h
        cmp    word ptr [ebp + 14h], 10h
        jne    SHORT loc_5_233F
        db  066h, 083h, 00Dh, 000h, 000h, 000h, 000h, 002h
loc_5_233F:
        cmp    word ptr [ebp + 18h], 2
        jne    SHORT loc_5_234E
        db  066h, 083h, 00Dh, 000h, 000h, 000h, 000h, 004h
loc_5_234E:
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 8]
        call   sub_5_2364
        add    esp, 8
        clc
        jmp    SHORT loc_5_2360
loc_5_235F:
        stc
loc_5_2360:
        pop    edi
        pop    eax
        pop    ebp
        ret
sub_5_2364:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ebx
        push   edx
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    al, 0c6h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2375:
        jb     SHORT loc_5_23C1
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
loc_5_2396:
        pop    eax
        jb     SHORT loc_5_23C1
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_239E:
        jb     SHORT loc_5_23C1
        push   edx
        mov    dx, 3
        mov    ax, 0b9eh
        div    bx
        neg    al
        pop    edx
        push   eax
        mov    al, 0a2h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_23B7:
        pop    eax
        jb     SHORT loc_5_23C1
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_23BF:
        jmp    SHORT loc_5_23C2
loc_5_23C1:
        stc
loc_5_23C2:
        pop    edi
        pop    edx
        pop    ebx
        pop    eax
        pop    ebp
        ret
sub_5_23C8:
        push   ebp
        mov    ebp, esp
        push   eax
        push   edx
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    al, 0d1h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_23D8:
        jb     NEAR PTR loc_5_25A0
        add    edx, 6
        mov    al, 2
        out    dx, al
        in     al, dx
        in     al, dx
        in     al, dx
        sub    edx, 6
        jmp    SHORT loc_5_23EC
loc_5_23EC:
        jmp    SHORT loc_5_23EE
loc_5_23EE:
        add    edx, 6
        xor    al, al
        out    dx, al
        sub    edx, 6
        mov    al, 0c6h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_23FE:
        jb     NEAR PTR loc_5_25A0
        mov    al, 0a4h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_240B:
        jb     NEAR PTR loc_5_25A0
        mov    eax, dword ptr [ebp + 0ch]
        neg    ax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_241C:
        jb     NEAR PTR loc_5_25A0
        push   eax
        mov    al, 0a5h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_242A:
        pop    eax
        jb     NEAR PTR loc_5_25A0
        mov    al, ah
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2438:
        jb     NEAR PTR loc_5_25A0
        mov    al, 0a8h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2445:
        jb     NEAR PTR loc_5_25A0
        mov    al, 0f5h
        db  066h, 0F7h, 005h, 000h, 000h, 000h, 000h, 004h, 000h
        jne    SHORT loc_5_245A
        mov    al, 0f6h
loc_5_245A:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_245F:
        jb     NEAR PTR loc_5_25A0
        mov    al, 0c0h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_246C:
        jb     NEAR PTR loc_5_25A0
        mov    al, 0b1h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2479:
        jb     NEAR PTR loc_5_25A0
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2484:
        jb     NEAR PTR loc_5_25A0
        and    al, 0fh
        or     al, 50h
        push   eax
        mov    al, 0b1h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2496:
        pop    eax
        jb     NEAR PTR loc_5_25A0
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_24A2:
        jb     NEAR PTR loc_5_25A0
        mov    al, 0c0h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_24AF:
        jb     NEAR PTR loc_5_25A0
        mov    al, 0b2h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_24BC:
        jb     NEAR PTR loc_5_25A0
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_24C7:
        jb     NEAR PTR loc_5_25A0
        and    al, 0fh
        or     al, 50h
        push   eax
        mov    al, 0b2h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_24D9:
        pop    eax
        jb     NEAR PTR loc_5_25A0
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_24E5:
        jb     NEAR PTR loc_5_25A0
        db  066h, 0F7h, 005h, 000h, 000h, 000h, 000h, 002h, 000h
        je     SHORT loc_5_2545
        mov    al, 0b6h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_24FD:
        jb     NEAR PTR loc_5_25A0
        xor    al, al
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_250A:
        jb     NEAR PTR loc_5_25A0
        mov    al, 0b7h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2517:
        jb     NEAR PTR loc_5_25A0
        mov    al, 71h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2524:
        jb     SHORT loc_5_25A0
        mov    al, 0b7h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_252D:
        jb     SHORT loc_5_25A0
        mov    al, 0bch
        db  066h, 0F7h, 005h, 000h, 000h, 000h, 000h, 004h, 000h
        jne    SHORT loc_5_253E
        mov    al, 0f4h
loc_5_253E:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2543:
        jb     SHORT loc_5_25A0
loc_5_2545:
        mov    al, 0b7h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_254C:
        jb     SHORT loc_5_25A0
        mov    al, 0d0h
        db  066h, 0F7h, 005h, 000h, 000h, 000h, 000h, 004h, 000h
        je     SHORT loc_5_255D
        mov    al, 98h
loc_5_255D:
        db  066h, 0F7h, 005h, 000h, 000h, 000h, 000h, 002h, 000h
        je     SHORT loc_5_256A
        or     al, 24h
loc_5_256A:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_256F:
        jb     SHORT loc_5_25A0
        mov    al, 0c0h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2578:
        jb     SHORT loc_5_25A0
        mov    al, 0b8h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2581:
        jb     SHORT loc_5_25A0
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2588:
        jb     SHORT loc_5_25A0
        and    al, 30h
        or     al, 5
        push   eax
        mov    al, 0b8h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2596:
        pop    eax
        jb     SHORT loc_5_25A0
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_259E:
        jmp    SHORT loc_5_25A1
loc_5_25A0:
        stc
loc_5_25A1:
        pop    edi
        pop    edx
        pop    eax
        pop    ebp
        ret
sub_5_25A6:
        push   ebp
        mov    ebp, esp
        push   eax
        push   edx
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    al, 0d3h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_25B6:
        jb     SHORT loc_5_25EE
        mov    al, 0c0h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_25BF:
        jb     SHORT loc_5_25EE
        mov    al, 0b8h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_25C8:
        jb     SHORT loc_5_25EE
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_25CF:
        jb     SHORT loc_5_25EE
        and    al, 30h
        push   eax
        mov    al, 0b8h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_25DB:
        pop    eax
        jb     SHORT loc_5_25EE
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_25E3:
        jmp    SHORT loc_5_25EF
        db  0B0h, 0D0h, 0E8h, 000h, 000h, 000h, 000h, 0EBh, 001h
loc_5_25EE:
        stc
loc_5_25EF:
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
        call   sub_5_23C8
        add    esp, 0ch
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_5_25A6
        add    esp, 8
        pop    ebp
        ret
        db  050h, 052h, 0C1h, 0E8h, 008h, 024h, 0F0h, 0C1h, 0EBh, 00Ch, 00Ah, 0C3h, 083h, 0C2h, 004h, 050h
        db  0B0h, 014h, 0EEh, 058h, 066h, 042h, 0EEh, 0F8h, 05Ah, 058h, 0C3h
sub_5_2638:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        int3
        int3
sub_5_2640:
        db  0CDh, 020h, 04Bh, 081h, 001h, 000h
        int3
        int3
sub_5_2648:
        db  0CDh, 020h, 052h, 081h, 001h, 000h
        int3
        int3
sub_5_2650:
        db  0CDh, 020h, 04Ah, 081h, 001h, 000h
        int3
        int3
sub_5_2658:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        int3
        int3
sub_5_2660:
        db  0CDh, 020h, 051h, 081h, 001h, 000h
        int3
        int3
sub_5_2668:
        db  0CDh, 020h, 03Dh, 080h, 033h, 000h
        int3
        int3
sub_5_2670:
        db  0CDh, 020h, 074h, 081h, 001h, 000h
        int3
        int3
sub_5_2678:
        db  0CDh, 020h, 044h, 080h, 033h, 000h
        int3
        int3
sub_5_2680:
        db  0CDh, 020h, 03Bh, 080h, 033h, 000h
        int3
        int3
sub_5_2688:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_2698:
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
sub_5_26AC:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_26BC:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_26CC:
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
sub_5_26E0:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_26F0:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_2700:
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
sub_5_2714:
        db  0CDh, 020h, 07Bh, 081h, 001h, 000h
        int3
        int3
sub_5_271C:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        int3
        int3
sub_5_2724:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        int3
        int3
sub_5_272C:
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
sub_5_2740:
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 0ch]
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        jb     SHORT loc_5_275E
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_5_275E:
        ret    10h
_PTEXT ends
        end
