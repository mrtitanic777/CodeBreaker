; vgateway_pcode_02_01.asm -- VGATEWAY object 2 segment _PTEXT  (part 1/2)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        lea    eax, [eax + eax*2]
        shl    eax, 2
        add    eax, dword ptr [ebp + 8]
        add    eax, 260h
        mov    dword ptr [ebp - 18h], eax
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_2_7A70
        add    esp, 4
        lea    eax, [ebp - 24h]
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 14h], eax
loc_2_003D:
        mov    eax, dword ptr [ebp - 18h]
        cmp    dword ptr [ebp - 14h], eax
        je     NEAR PTR loc_2_00E4
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 65h]
        and    eax, 0ffh
        cmp    eax, 0ffh
        je     NEAR PTR loc_2_00D7
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 34h]
        and    eax, 7fh
        mov    dword ptr [ebp - 28h], eax
        jmp    NEAR PTR loc_2_00B9
loc_2_008A:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        jmp    NEAR PTR loc_2_00D2
loc_2_009F:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    eax, [ebp - 24h]
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        jmp    NEAR PTR loc_2_00D2
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_00B9:
        cmp    dword ptr [ebp - 28h], 10h
        jl     NEAR PTR loc_2_009F
        cmp    dword ptr [ebp - 28h], 11h
        jle    NEAR PTR loc_2_008A
        jmp    NEAR PTR loc_2_009F
loc_2_00D2:
        jmp    NEAR PTR loc_2_00DF
loc_2_00D7:
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 14h], eax
loc_2_00DF:
        jmp    NEAR PTR loc_2_003D
loc_2_00E4:
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_0113
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        call   dword ptr [eax + 20h]
        add    esp, 8
        jmp    NEAR PTR loc_2_00E4
loc_2_0113:
        lea    eax, [ebp - 24h]
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_0142
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        call   dword ptr [eax + 20h]
        add    esp, 8
        jmp    NEAR PTR loc_2_0113
loc_2_0142:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_0147:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_2_01D8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3EAC
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 3
        je     NEAR PTR loc_2_0194
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_01AC
loc_2_0194:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4C41
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1AAA
        add    esp, 4
loc_2_01AC:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_2_01D8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4C78
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_39E6
        add    esp, 4
loc_2_01D8:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_01DD:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 4], 3
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_2_01FC
loc_2_01F9:
        inc    dword ptr [ebp - 8]
loc_2_01FC:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 300h], ecx
        jbe    NEAR PTR loc_2_021F
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7567
        add    esp, 4
        jmp    NEAR PTR loc_2_01F9
loc_2_021F:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_77DB
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7CD0
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_0255
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7C4D
        add    esp, 4
        jmp    NEAR PTR loc_2_027A
loc_2_0255:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7DD3
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_027A
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7C4D
        add    esp, 4
loc_2_027A:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 2f0h], 0
        je     NEAR PTR loc_2_0296
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0700
        add    esp, 4
loc_2_0296:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ech]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 6], ax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_02AB:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_02C4
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_02C4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_02E6
        jmp    NEAR PTR loc_2_031E
loc_2_02E6:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 4], 4
        mov    ax, word ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 2c0h], ax
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 3
        jne    NEAR PTR loc_2_0312
loc_2_0312:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0D00
        add    esp, 4
loc_2_031E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_0323:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2c0h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 24h], eax
        jmp    NEAR PTR loc_2_037C
loc_2_0343:
        mov    word ptr [ebp - 20h], 469h
        jmp    NEAR PTR loc_2_0395
loc_2_034E:
        mov    word ptr [ebp - 20h], 467h
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [ebp - 1ah]
        add    esi, 134h
        mov    ecx, 6
        rep movsd
        jmp    NEAR PTR loc_2_0395
loc_2_036C:
        mov    word ptr [ebp - 20h], 468h
        jmp    NEAR PTR loc_2_0395
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_037C:
        cmp    dword ptr [ebp - 24h], 0
        je     NEAR PTR loc_2_0343
        cmp    dword ptr [ebp - 24h], 1
        je     NEAR PTR loc_2_034E
        jmp    NEAR PTR loc_2_036C
loc_2_0395:
        mov    eax, dword ptr [ebp - 1ah]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        call   dword ptr [eax + 8]
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 04Dh, 001h, 0FEh, 0FFh
        add    esp, 4
        mov    eax, 1
        mov    edx, dword ptr [ebp + 8]
        mov    cl, byte ptr [edx + 2b4h]
        shl    eax, cl
        db  009h, 005h, 028h, 00Ah, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 4], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_03DD:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_2_0405
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 5], 1
        jmp    NEAR PTR loc_2_040A
loc_2_0405:
        jmp    NEAR PTR loc_2_0418
loc_2_040A:
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_02C4
        add    esp, 8
loc_2_0418:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h
sub_2_0420:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 168h
        push   eax
        call   sub_2_7A70
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_043C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_063F
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_2_052A
        call   sub_2_06A8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_2_0488
        mov    eax, dword ptr [ebp + 14h]
        mov    word ptr [eax], 0eh
        sub    eax, eax
        jmp    NEAR PTR loc_2_0570
        db  0E9h, 0A2h, 000h, 000h, 000h
loc_2_0488:
        push   10h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0ch
        push   eax
        db  0E8h, 0B0h, 000h, 0FEh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 1eh], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 1ch
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8138
        add    esp, 0ch
        mov    byte ptr [ebp - 4], al
        jmp    NEAR PTR loc_2_04CE
loc_2_04CE:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_2_051B
loc_2_04DB:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 168h
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        jmp    NEAR PTR loc_2_052A
loc_2_04F5:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_06E3
        add    esp, 4
        mov    al, byte ptr [ebp - 4]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 14h]
        mov    word ptr [ecx], ax
        sub    eax, eax
        jmp    NEAR PTR loc_2_0570
        db  0E9h, 00Fh, 000h, 000h, 000h
loc_2_051B:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_2_04DB
        jmp    NEAR PTR loc_2_04F5
loc_2_052A:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 1eh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 10h]
        and    ecx, 0ffffh
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 1eh], ax
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_2_0560
        mov    eax, dword ptr [ebp - 8]
        inc    word ptr [eax + 20h]
loc_2_0560:
        mov    eax, dword ptr [ebp + 14h]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_2_0570
loc_2_0570:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_0575:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_05DE
loc_2_058E:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 1eh]
        and    eax, 0fffdh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 1eh], ax
        jmp    NEAR PTR loc_2_05F7
loc_2_05A6:
        mov    eax, dword ptr [ebp + 0ch]
        dec    word ptr [eax + 20h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 20h]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_05D4
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 1eh]
        and    eax, 0fffeh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 1eh], ax
loc_2_05D4:
        jmp    NEAR PTR loc_2_05F7
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_05DE:
        cmp    dword ptr [ebp - 4], 1
        je     NEAR PTR loc_2_05A6
        cmp    dword ptr [ebp - 4], 2
        je     NEAR PTR loc_2_058E
        jmp    NEAR PTR loc_2_05F7
loc_2_05F7:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 1eh]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_063A
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 0ch
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_81A6
        add    esp, 10h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_2_06E3
        add    esp, 4
loc_2_063A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_063F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 168h]
        mov    dword ptr [ebp - 4], eax
loc_2_0654:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 168h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_2_069C
        push   10h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0ch
        push   eax
        db  0E8h, 0B4h, 0FEh, 0FDh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_2_068F
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_0697
loc_2_068F:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_06A3
loc_2_0697:
        jmp    NEAR PTR loc_2_0654
loc_2_069C:
        sub    eax, eax
        jmp    NEAR PTR loc_2_06A3
loc_2_06A3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_06A8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   24h
        push   0
        db  0E8h, 046h, 0F9h, 0FDh, 0FFh
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_06D6
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7A95
        add    esp, 4
loc_2_06D6:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_06DE
loc_2_06DE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_06E3:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 03Fh, 0F9h, 0FDh, 0FFh
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_0700:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   40h
        push   0
        db  068h, 090h, 007h, 000h, 000h
        db  0E8h, 04Ah, 0FEh, 0FDh, 0FFh
        add    esp, 0ch
        db  0C6h, 005h, 090h, 007h, 000h, 000h, 075h
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2b4h]
        db  0A2h, 0C0h, 007h, 000h, 000h
        push   10h
        db  068h, 040h, 008h, 000h, 000h
        db  0B8h, 090h, 007h, 000h, 000h
        add    eax, 1ah
        push   eax
        db  0E8h, 006h, 0FEh, 0FDh, 0FFh
        add    esp, 0ch
        db  068h, 090h, 007h, 000h, 000h
        db  0E8h, 0C3h, 0FEh, 0FDh, 0FFh
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_2_0761
        jmp    NEAR PTR loc_2_07CD
loc_2_0761:
        db  0A0h, 093h, 007h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 250h], al
        push   0
        push   5adh
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_17B6
        add    esp, 10h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_2_0796
        jmp    NEAR PTR loc_2_07CD
loc_2_0796:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 4]
        db  0C7h, 040h, 020h, 0D2h, 007h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 250h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8764
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0B5A
        add    esp, 4
loc_2_07CD:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_07D2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        jmp    NEAR PTR loc_2_07E0
loc_2_07E0:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_0809
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        jmp    NEAR PTR loc_2_0937
loc_2_0809:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_2_091E
loc_2_081C:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 3
        je     NEAR PTR loc_2_084F
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 250h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8764
        add    esp, 0ch
        jmp    NEAR PTR loc_2_0937
loc_2_084F:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 3eh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6127
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_2_08D7
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0ch]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_2_08D7
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_2_0892
loc_2_088E:
        inc    word ptr [ebp - 4]
loc_2_0892:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        db  03Bh, 005h, 054h, 00Ah, 000h, 000h
        jae    NEAR PTR loc_2_08D7
        mov    ax, word ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [ecx + eax*4 + 26h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8704
        add    esp, 10h
        jmp    NEAR PTR loc_2_088E
loc_2_08D7:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 250h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8764
        add    esp, 0ch
        jmp    NEAR PTR loc_2_0937
loc_2_08F6:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    NEAR PTR loc_2_0937
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_091E:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_2_081C
        cmp    dword ptr [ebp - 0ch], 6
        je     NEAR PTR loc_2_08D7
        jmp    NEAR PTR loc_2_08F6
loc_2_0937:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_093C:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 10h], 0
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 14h], 0
        jmp    NEAR PTR loc_2_095F
loc_2_095F:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_0988
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        jmp    NEAR PTR loc_2_0B55
loc_2_0988:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_2_09B3
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 248h], 0
        jne    NEAR PTR loc_2_09B3
        mov    dword ptr [ebp - 10h], 1
loc_2_09B3:
        db  083h, 03Dh, 024h, 00Ah, 000h, 000h, 000h
        je     NEAR PTR loc_2_09EA
        mov    eax, dword ptr [ebp + 8]
        add    eax, 26ch
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_09E5
        mov    dword ptr [ebp - 4], 1
loc_2_09E5:
        jmp    NEAR PTR loc_2_09F1
loc_2_09EA:
        mov    dword ptr [ebp - 4], 1
loc_2_09F1:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 3eh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6127
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_2_0A13
        mov    dword ptr [ebp - 14h], 1
loc_2_0A13:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 37h]
        mov    byte ptr [ebp - 18h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 11h]
        mov    byte ptr [ebp - 0ch], al
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_2_0AC5
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_0AC5
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_2_0AC5
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_2_0AC5
        push   0
        push   5adh
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_17B6
        add    esp, 10h
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_2_0A83
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_2_0AC0
loc_2_0A83:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 34h], 0a2h
        push   10h
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 3eh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 4eh
        push   eax
        db  0E8h, 0ABh, 0FAh, 0FDh, 0FFh
        add    esp, 0ch
        db  0A1h, 024h, 00Ah, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 248h], eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1420
        add    esp, 8
loc_2_0AC0:
        jmp    NEAR PTR loc_2_0B1D
loc_2_0AC5:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 1ch], eax
        jmp    NEAR PTR loc_2_0B04
loc_2_0AD8:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_2_0B1D
loc_2_0AE3:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_2_0B1D
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_0B04:
        cmp    dword ptr [ebp - 1ch], 0
        je     NEAR PTR loc_2_0AD8
        cmp    dword ptr [ebp - 1ch], 6
        je     NEAR PTR loc_2_0AD8
        jmp    NEAR PTR loc_2_0AE3
loc_2_0B1D:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_2_0B55
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 10h], 1
        mov    al, byte ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 11h], al
        mov    eax, dword ptr [ebp - 8]
        db  0C7h, 040h, 020h, 03Ch, 009h, 000h, 000h
        mov    al, byte ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8764
        add    esp, 0ch
loc_2_0B55:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_0B5A:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7C6B
        add    esp, 4
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_2_0B81
        jmp    NEAR PTR loc_2_0C11
loc_2_0B81:
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_2_0B90
loc_2_0B8C:
        inc    word ptr [ebp - 4]
loc_2_0B90:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        db  03Bh, 005h, 054h, 00Ah, 000h, 000h
        jae    NEAR PTR loc_2_0C11
        push   0
        push   5adh
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_17B6
        add    esp, 10h
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_2_0BCB
        jmp    NEAR PTR loc_2_0C11
loc_2_0BCB:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 10h], 1
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 11h], al
        mov    eax, dword ptr [ebp - 8]
        db  0C7h, 040h, 020h, 03Ch, 009h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [ecx + eax*4 + 26h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8764
        add    esp, 0ch
        jmp    NEAR PTR loc_2_0B8C
loc_2_0C11:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0C20:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2b5h], 0
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_2_0C42
loc_2_0C3F:
        inc    dword ptr [ebp - 4]
loc_2_0C42:
        cmp    dword ptr [ebp - 4], 9
        jge    NEAR PTR loc_2_0C5F
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 2b6h], 0
        jmp    NEAR PTR loc_2_0C3F
loc_2_0C5F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_0C64:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_2_0C7C
loc_2_0C79:
        inc    dword ptr [ebp - 4]
loc_2_0C7C:
        cmp    dword ptr [ebp - 4], 9
        jge    NEAR PTR loc_2_0CAE
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 2b6h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_2_0CA9
        mov    ax, 2
        jmp    NEAR PTR loc_2_0CB7
loc_2_0CA9:
        jmp    NEAR PTR loc_2_0C79
loc_2_0CAE:
        mov    ax, 1
        jmp    NEAR PTR loc_2_0CB7
loc_2_0CB7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_0CBC:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_0CEB
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2E8F
        add    esp, 8
        jmp    NEAR PTR loc_2_0CFB
loc_2_0CEB:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_257B
        add    esp, 8
loc_2_0CFB:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_0D00:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        jmp    NEAR PTR loc_2_0D0E
loc_2_0D0E:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 150h
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_0D45
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2b6h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2b5h], 1
        jmp    NEAR PTR loc_2_0D4F
loc_2_0D45:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2b6h], 1
loc_2_0D4F:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_2_0D5E
loc_2_0D5B:
        inc    dword ptr [ebp - 4]
loc_2_0D5E:
        cmp    dword ptr [ebp - 4], 2
        jg     NEAR PTR loc_2_0DBD
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 260h]
        mov    dword ptr [ebp - 0ch], eax
loc_2_0D7B:
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        shl    eax, 2
        add    eax, dword ptr [ebp + 8]
        add    eax, 260h
        cmp    eax, dword ptr [ebp - 0ch]
        je     NEAR PTR loc_2_0DB8
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_34D2
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_2_0D7B
loc_2_0DB8:
        jmp    NEAR PTR loc_2_0D5B
loc_2_0DBD:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1f4h], 1
        sbb    eax, eax
        neg    eax
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 2b7h], al
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1f0h], 1
        sbb    eax, eax
        neg    eax
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 2bdh], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7A3B
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 2b9h], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_778F
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 2bah], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1A5E
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 2b8h], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_21E8
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 2bbh], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_355A
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 2bch], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_678E
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 2beh], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2b6h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_2_0E90
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0F54
        add    esp, 4
        jmp    NEAR PTR loc_2_0EC5
loc_2_0E90:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0C64
        add    esp, 4
        and    eax, 0ffffh
        cmp    eax, 1
        je     NEAR PTR loc_2_0EB9
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2b5h], 2
        jmp    NEAR PTR loc_2_0EC5
loc_2_0EB9:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0323
        add    esp, 4
loc_2_0EC5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_0ECA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        jmp    NEAR PTR loc_2_0ED5
loc_2_0ED5:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 2b6h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2b5h]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_2_0F1A
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2b6h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_2_0F1A
        jmp    NEAR PTR loc_2_0F4F
loc_2_0F1A:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0C64
        add    esp, 4
        and    eax, 0ffffh
        cmp    eax, 1
        je     NEAR PTR loc_2_0F43
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2b5h], 2
        jmp    NEAR PTR loc_2_0F4F
loc_2_0F43:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0323
        add    esp, 4
loc_2_0F4F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_0F54:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 150h]
        mov    dword ptr [ebp - 8], eax
loc_2_0F69:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 150h
        cmp    eax, dword ptr [ebp - 8]
        je     NEAR PTR loc_2_1022
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_2_0F8D
loc_2_0F8D:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0ch]
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_2_0FFA
        db  0E9h, 078h, 000h, 000h, 000h
loc_2_0FA5:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0CBC
        add    esp, 8
        jmp    NEAR PTR loc_2_101D
loc_2_0FBA:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 1e4h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3E42
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2F5B
        add    esp, 8
        jmp    NEAR PTR loc_2_101D
        db  0E9h, 028h, 000h, 000h, 000h, 0E9h, 023h, 000h, 000h, 000h
loc_2_0FFA:
        cmp    dword ptr [ebp - 0ch], 1
        je     NEAR PTR loc_2_101D
        cmp    dword ptr [ebp - 0ch], 2
        je     NEAR PTR loc_2_0FA5
        cmp    dword ptr [ebp - 0ch], 3
        je     NEAR PTR loc_2_0FBA
        jmp    NEAR PTR loc_2_101D
loc_2_101D:
        jmp    NEAR PTR loc_2_0F69
loc_2_1022:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 150h
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_104E
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0ECA
        add    esp, 8
loc_2_104E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_1053:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 150h
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_1085
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0ECA
        add    esp, 8
loc_2_1085:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_2_1090:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 22ch
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 244h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 238h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 23ah], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 23ch], 0
        db  0A1h, 024h, 00Ah, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 248h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 24ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 240h], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_1104:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_2_111C
loc_2_1118:
        inc    word ptr [ebp - 4]
loc_2_111C:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        db  03Bh, 005h, 054h, 00Ah, 000h, 000h
        jae    NEAR PTR loc_2_119B
        push   0
        push   5adh
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_17B6
        add    esp, 10h
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_2_1157
        jmp    NEAR PTR loc_2_119B
loc_2_1157:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp - 8]
        db  0C7h, 040h, 020h, 0A0h, 011h, 000h, 000h
        mov    ax, word ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [ecx + eax*4 + 26h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_86A5
        add    esp, 10h
        jmp    NEAR PTR loc_2_1118
loc_2_119B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_11A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 14h], 0
        mov    dword ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_2_11BC
loc_2_11BC:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 37h]
        mov    byte ptr [ebp - 1ch], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 11h]
        mov    byte ptr [ebp - 0ch], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 3
        je     NEAR PTR loc_2_1228
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_120B
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        jmp    NEAR PTR loc_2_1223
loc_2_120B:
        mov    al, byte ptr [ebp - 0ch]
        push   eax
        mov    al, byte ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_86A5
        add    esp, 10h
loc_2_1223:
        jmp    NEAR PTR loc_2_141B
loc_2_1228:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        and    eax, 0ffh
        cmp    eax, 40h
        jl     NEAR PTR loc_2_125F
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    NEAR PTR loc_2_141B
loc_2_125F:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 18h], eax
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 150h]
        mov    dword ptr [ebp - 8], eax
loc_2_127B:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 150h
        cmp    eax, dword ptr [ebp - 8]
        je     NEAR PTR loc_2_12EF
        mov    eax, dword ptr [ebp - 18h]
        cmp    dword ptr [ebp - 8], eax
        je     NEAR PTR loc_2_12A5
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_2_12EA
loc_2_12A5:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 0ch]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_2_12EA
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 18h]
        mov    al, byte ptr [ecx + eax*4 + 26h]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 1ch]
        cmp    eax, ecx
        jne    NEAR PTR loc_2_12EA
        mov    dword ptr [ebp - 4], 1
        jmp    NEAR PTR loc_2_12EF
loc_2_12EA:
        jmp    NEAR PTR loc_2_127B
loc_2_12EF:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_2_130E
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        jmp    NEAR PTR loc_2_141B
loc_2_130E:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_2_1402
loc_2_1321:
        push   0
        push   5adh
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_17B6
        add    esp, 10h
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_2_1395
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 34h], 0a0h
        push   10h
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 3eh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 4eh
        push   eax
        db  0E8h, 0EBh, 0F1h, 0FDh, 0FFh
        add    esp, 0ch
        push   10h
        mov    eax, dword ptr [ebp - 18h]
        add    eax, 12h
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 3eh
        push   eax
        db  0E8h, 0D3h, 0F1h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1420
        add    esp, 8
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_2_139B
loc_2_1395:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 10h], eax
loc_2_139B:
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp - 10h]
        db  0C7h, 040h, 020h, 0A0h, 011h, 000h, 000h
        mov    al, byte ptr [ebp - 0ch]
        push   eax
        mov    al, byte ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_86A5
        add    esp, 10h
        jmp    NEAR PTR loc_2_141B
loc_2_13CB:
        mov    al, byte ptr [ebp - 0ch]
        push   eax
        mov    al, byte ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_86A5
        add    esp, 10h
        jmp    NEAR PTR loc_2_141B
loc_2_13E8:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        jmp    NEAR PTR loc_2_141B
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_1402:
        cmp    dword ptr [ebp - 20h], 0
        je     NEAR PTR loc_2_1321
        cmp    dword ptr [ebp - 20h], 6
        je     NEAR PTR loc_2_13CB
        jmp    NEAR PTR loc_2_13E8
loc_2_141B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_1420:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 18h]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_2_14B4
        mov    eax, dword ptr [ebp + 8]
        add    eax, 22ch
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_14B4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 22ch]
        mov    dword ptr [ebp - 4], eax
loc_2_146F:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 22ch
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_2_14B4
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 18h]
        and    eax, 0ffffh
        cmp    eax, 2
        jne    NEAR PTR loc_2_14AF
        mov    dword ptr [ebp - 0ch], 1
        jmp    NEAR PTR loc_2_14B4
loc_2_14AF:
        jmp    NEAR PTR loc_2_146F
loc_2_14B4:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_2_14D8
        jmp    NEAR PTR loc_2_14C3
loc_2_14C3:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_7B57
        add    esp, 8
        jmp    NEAR PTR loc_2_14ED
loc_2_14D8:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 22ch
        push   eax
        call   sub_2_7AAA
        add    esp, 8
loc_2_14ED:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 244h]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_1510
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1515
        add    esp, 4
loc_2_1510:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_1515:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
loc_2_151E:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 22ch
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_156C
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 22ch]
        mov    ax, word ptr [eax + 18h]
        and    eax, 0ffffh
        cmp    eax, 1
        je     NEAR PTR loc_2_156C
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 24ch], 0
        je     NEAR PTR loc_2_156C
        jmp    NEAR PTR loc_2_169C
loc_2_156C:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 22ch
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_2_158F
        jmp    NEAR PTR loc_2_169C
loc_2_158F:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 244h], 1
        lea    eax, [ebp - 4]
        push   eax
        push   2
        mov    eax, dword ptr [ebp - 8]
        add    eax, 4eh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_043C
        add    esp, 10h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 240h], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    eax, 40h
        jl     NEAR PTR loc_2_1609
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1775
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 244h], 0
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    NEAR PTR loc_2_169C
loc_2_1609:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_167B
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 244h], 1
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 8]
        db  0C7h, 040h, 020h, 0A1h, 016h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 240h]
        mov    al, byte ptr [eax + 1ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 37h], al
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 24h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2b4h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 64h], al
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
        jmp    NEAR PTR loc_2_169C
        db  0E9h, 01Ch, 000h, 000h, 000h
loc_2_167B:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 244h], 0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        jmp    NEAR PTR loc_2_151E
loc_2_169C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_16A1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        push   2
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 240h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0575
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 240h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 244h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_1719
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1775
        add    esp, 4
        jmp    NEAR PTR loc_2_1770
loc_2_1719:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 40h
        jl     NEAR PTR loc_2_1746
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1775
        add    esp, 4
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    NEAR PTR loc_2_1770
loc_2_1746:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 22ch
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_1770
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1515
        add    esp, 4
loc_2_1770:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_1775:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
loc_2_177E:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 22ch
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_17B1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_192D
        add    esp, 8
        jmp    NEAR PTR loc_2_177E
loc_2_17B1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_17B6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_2_1821
loc_2_17CF:
        mov    eax, dword ptr [ebp + 14h]
        db  08Bh, 00Dh, 020h, 00Ah, 000h, 000h
        cmp    dword ptr [eax + 5bh], ecx
        jb     NEAR PTR loc_2_17E8
        sub    eax, eax
        jmp    NEAR PTR loc_2_1928
loc_2_17E8:
        jmp    NEAR PTR loc_2_183A
loc_2_17ED:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 23ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 2fch]
        jb     NEAR PTR loc_2_1812
        sub    eax, eax
        jmp    NEAR PTR loc_2_1928
loc_2_1812:
        jmp    NEAR PTR loc_2_183A
        db  0E9h, 01Eh, 000h, 000h, 000h, 0E9h, 019h, 000h, 000h, 000h
loc_2_1821:
        cmp    dword ptr [ebp - 8], 2
        je     NEAR PTR loc_2_17CF
        cmp    dword ptr [ebp - 8], 4
        je     NEAR PTR loc_2_17ED
        jmp    NEAR PTR loc_2_183A
loc_2_183A:
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        add    eax, 75h
        push   eax
        push   0
        db  0E8h, 0B3h, 0E7h, 0FDh, 0FFh
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_2_1864
        sub    eax, eax
        jmp    NEAR PTR loc_2_1928
loc_2_1864:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7A95
        add    esp, 4
        mov    ax, word ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 18h], ax
        push   8
        db  068h, 054h, 008h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        add    eax, 2ch
        push   eax
        db  0E8h, 0BCh, 0ECh, 0FDh, 0FFh
        add    esp, 0ch
        push   40h
        push   0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 34h
        push   eax
        db  0E8h, 0C0h, 0ECh, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        add    eax, 74h
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 38h], eax
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_2_18FD
loc_2_18C0:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 238h]
        jmp    NEAR PTR loc_2_1920
loc_2_18CF:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 23ah]
        mov    eax, dword ptr [ebp + 14h]
        inc    dword ptr [eax + 5bh]
        jmp    NEAR PTR loc_2_1920
loc_2_18E4:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 23ch]
        jmp    NEAR PTR loc_2_1920
        db  0E9h, 028h, 000h, 000h, 000h, 0E9h, 023h, 000h, 000h, 000h
loc_2_18FD:
        cmp    dword ptr [ebp - 0ch], 1
        je     NEAR PTR loc_2_18C0
        cmp    dword ptr [ebp - 0ch], 2
        je     NEAR PTR loc_2_18CF
        cmp    dword ptr [ebp - 0ch], 4
        je     NEAR PTR loc_2_18E4
        jmp    NEAR PTR loc_2_1920
loc_2_1920:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_1928
loc_2_1928:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_192D:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 18h]
        mov    word ptr [ebp - 0ch], ax
        push   10h
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 3eh
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        db  0E8h, 0F7h, 0EBh, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0E8h, 0D5h, 0E6h, 0FDh, 0FFh
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_2_19CF
loc_2_1975:
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 238h]
        jmp    NEAR PTR loc_2_19F2
loc_2_1984:
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 23ah]
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6850
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_19B1
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 5bh]
loc_2_19B1:
        jmp    NEAR PTR loc_2_19F2
loc_2_19B6:
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 23ch]
        jmp    NEAR PTR loc_2_19F2
        db  0E9h, 028h, 000h, 000h, 000h, 0E9h, 023h, 000h, 000h, 000h
loc_2_19CF:
        cmp    dword ptr [ebp - 20h], 1
        je     NEAR PTR loc_2_1975
        cmp    dword ptr [ebp - 20h], 2
        je     NEAR PTR loc_2_1984
        cmp    dword ptr [ebp - 20h], 4
        je     NEAR PTR loc_2_19B6
        jmp    NEAR PTR loc_2_19F2
loc_2_19F2:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_1A59
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 238h]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_1A59
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 23ah]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_1A59
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 23ch]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_1A59
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0ECA
        add    esp, 8
loc_2_1A59:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_1A5E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 238h]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_1A92
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 23ah]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_1A9E
loc_2_1A92:
        sub    al, al
        jmp    NEAR PTR loc_2_1AA5
        db  0E9h, 007h, 000h, 000h, 000h
loc_2_1A9E:
        mov    al, 1
        jmp    NEAR PTR loc_2_1AA5
loc_2_1AA5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_1AAA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_1AD5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1775
        add    esp, 4
        jmp    NEAR PTR loc_2_1B3A
loc_2_1AD5:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 248h], 0
        je     NEAR PTR loc_2_1AEE
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 248h]
loc_2_1AEE:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 24ch], 0
        je     NEAR PTR loc_2_1B3A
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 24ch]
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 24ch], 0
        jne    NEAR PTR loc_2_1B3A
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 244h]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_1B3A
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1515
        add    esp, 4
loc_2_1B3A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_1B3F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 2f8h], 0
        je     NEAR PTR loc_2_1B62
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 24ch], 0ah
loc_2_1B62:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_1B70:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_2_1B88
loc_2_1B84:
        inc    word ptr [ebp - 4]
loc_2_1B88:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        db  03Bh, 005h, 054h, 00Ah, 000h, 000h
        jae    NEAR PTR loc_2_1BF5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2165
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_2_1BBA
        jmp    NEAR PTR loc_2_1BF5
loc_2_1BBA:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 10h], 1
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 11h], al
        mov    eax, dword ptr [ebp - 8]
        db  0C7h, 040h, 020h, 0FAh, 01Bh, 000h, 000h
        db  068h, 060h, 008h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 12h
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_82ED
        add    esp, 10h
        jmp    NEAR PTR loc_2_1B84
loc_2_1BF5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_1BFA:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        jmp    NEAR PTR loc_2_1C08
loc_2_1C08:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_1C65
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_2_1C50
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 36h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 11h]
        and    eax, 0ffff00ffh
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_83A8
        add    esp, 10h
loc_2_1C50:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2196
        add    esp, 8
        jmp    NEAR PTR loc_2_1FEF
loc_2_1C65:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        and    eax, 0ffh
        cmp    eax, 40h
        jl     NEAR PTR loc_2_1C9C
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2196
        add    esp, 8
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    NEAR PTR loc_2_1FEF
loc_2_1C9C:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 4eh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6127
        add    esp, 8
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_2_1CD1
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2196
        add    esp, 8
        jmp    NEAR PTR loc_2_1FEF
loc_2_1CD1:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_2_1FD6
loc_2_1CE4:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 3
        je     NEAR PTR loc_2_1D3A
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 36h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 11h]
        and    eax, 0ffff00ffh
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_83A8
        add    esp, 10h
        db  068h, 064h, 008h, 000h, 000h
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 12h
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_82ED
        add    esp, 10h
        jmp    NEAR PTR loc_2_1FEF
loc_2_1D3A:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5DE6
        add    esp, 4
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_2_1D95
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 36h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 11h]
        and    eax, 0ffff00ffh
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_83A8
        add    esp, 10h
        db  068h, 068h, 008h, 000h, 000h
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 12h
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_82ED
        add    esp, 10h
        jmp    NEAR PTR loc_2_1FEF
loc_2_1D95:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 190h
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 0ch], 2
        push   10h
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 4eh
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 1dh
        push   eax
        db  0E8h, 084h, 0E7h, 0FDh, 0FFh
        add    esp, 0ch
        push   10h
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 3eh
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 0dh
        push   eax
        db  0E8h, 06Ch, 0E7h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 40h], 0
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 44h], eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2E13
        add    esp, 8
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_2_1E13
loc_2_1E0F:
        inc    word ptr [ebp - 4]
loc_2_1E13:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        db  03Bh, 005h, 054h, 00Ah, 000h, 000h
        jae    NEAR PTR loc_2_1E3C
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + ecx + 48h], 0
        jmp    NEAR PTR loc_2_1E0F
loc_2_1E3C:
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 58h
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 0d9h], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 0d8h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 11h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 0dah], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 36h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 0dbh], al
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 0dch], 3
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 0f4h], 0
        db  068h, 06Ch, 008h, 000h, 000h
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 12h
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_82ED
        add    esp, 10h
        lea    eax, [ebp - 8]
        push   eax
        push   1
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 0dh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_043C
        add    esp, 10h
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 40h], eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 1ch], eax
        jmp    NEAR PTR loc_2_1F21
        db  0E9h, 056h, 000h, 000h, 000h
loc_2_1EE4:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        jmp    NEAR PTR loc_2_1FEF
loc_2_1EF9:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    NEAR PTR loc_2_1FEF
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_1F21:
        cmp    dword ptr [ebp - 1ch], 0
        je     NEAR PTR loc_2_1F3A
        cmp    dword ptr [ebp - 1ch], 0eh
        je     NEAR PTR loc_2_1EE4
        jmp    NEAR PTR loc_2_1EF9
loc_2_1F3A:
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 64h
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp - 10h]
        db  0C7h, 040h, 020h, 0F4h, 01Fh, 000h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 0d9h], 1
        jmp    NEAR PTR loc_2_1F6C
loc_2_1F6C:
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 1dh
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 0dh
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8232
        add    esp, 10h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1B3F
        add    esp, 4
        jmp    NEAR PTR loc_2_1FEF
loc_2_1F9B:
        db  068h, 070h, 008h, 000h, 000h
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 12h
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_82ED
        add    esp, 10h
        jmp    NEAR PTR loc_2_1FEF
loc_2_1FBC:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2196
        add    esp, 8
        jmp    NEAR PTR loc_2_1FEF
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_1FD6:
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_2_1CE4
        cmp    dword ptr [ebp - 18h], 18h
        je     NEAR PTR loc_2_1F9B
        jmp    NEAR PTR loc_2_1FBC
loc_2_1FEF:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_1FF4:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        jmp    NEAR PTR loc_2_2002
loc_2_2002:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        and    eax, 0ffh
        cmp    eax, 40h
        jl     NEAR PTR loc_2_2042
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    NEAR PTR loc_2_2160
loc_2_2042:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_2_208D
loc_2_2055:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 36h]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0d8h], al
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0d9h], 3
        jmp    NEAR PTR loc_2_209C
loc_2_2073:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        jmp    NEAR PTR loc_2_2160
        db  0E9h, 00Fh, 000h, 000h, 000h
loc_2_208D:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_2_2055
        jmp    NEAR PTR loc_2_2073
loc_2_209C:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 44h]
        mov    al, byte ptr [eax + 0ch]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_20C8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        jmp    NEAR PTR loc_2_2160
loc_2_20C8:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0dch]
        and    eax, 0ffh
        cmp    eax, 5
        jne    NEAR PTR loc_2_20F4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5B1C
        add    esp, 8
        jmp    NEAR PTR loc_2_2160
loc_2_20F4:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0ch], 3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3FE8
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0dah]
        and    eax, 0ffff00ffh
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 44h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4088
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0f4h], 0
        je     NEAR PTR loc_2_2160
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0f4h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4211
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0f4h], 0
loc_2_2160:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_2165:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_2_7BC5
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_2189
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 1e0h]
loc_2_2189:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_2191
loc_2_2191:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_2196:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_2_7C30
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 1e0h]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_21E3
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1e0h], 0
        jne    NEAR PTR loc_2_21E3
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0ECA
        add    esp, 8
loc_2_21E3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_21E8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1e0h], 0
        je     NEAR PTR loc_2_220A
        sub    al, al
        jmp    NEAR PTR loc_2_2211
        db  0E9h, 007h, 000h, 000h, 000h
loc_2_220A:
        mov    al, 1
        jmp    NEAR PTR loc_2_2211
loc_2_2211:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_2220:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 2]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 10h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 12h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 134h], 0
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_2_225C
loc_2_2258:
        inc    word ptr [ebp - 4]
loc_2_225C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_2_22C3
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        add    eax, eax
        lea    eax, [eax + eax*8]
        add    eax, dword ptr [ebp + 0ch]
        add    eax, 4
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    ecx, ecx
        lea    ecx, [ecx + ecx*8]
        add    ecx, dword ptr [ebp + 8]
        add    ecx, 14h
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], edx
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [ecx + 8], edx
        mov    edx, dword ptr [eax + 0ch]
        mov    dword ptr [ecx + 0ch], edx
        mov    ax, word ptr [eax + 10h]
        mov    word ptr [ecx + 10h], ax
        jmp    NEAR PTR loc_2_2258
loc_2_22C3:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 150h
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        add    eax, 15ch
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2d8h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 14ch], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_22FC:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 10h]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 10h]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_2367
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_2_2333
loc_2_232F:
        inc    word ptr [ebp - 4]
loc_2_2333:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_2_2362
        mov    ax, word ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_26FA
        add    esp, 8
        jmp    NEAR PTR loc_2_232F
loc_2_2362:
        jmp    NEAR PTR loc_2_237E
loc_2_2367:
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 10h]
        push   0
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_28F7
        add    esp, 0ch
loc_2_237E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_2383:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2F09
        add    esp, 4
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_2_23AE
        mov    ax, 1
        jmp    NEAR PTR loc_2_2576
loc_2_23AE:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2F81
        add    esp, 4
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_23E0
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2F5B
        add    esp, 8
        mov    ax, 1
        jmp    NEAR PTR loc_2_2576
loc_2_23E0:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_2_7A95
        add    esp, 4
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 0ch], 1
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 0dh], 0
        db  0A1h, 054h, 00Ah, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 10h], ax
        push   10h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 12h
        push   eax
        db  0E8h, 032h, 0E1h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [eax + 0eh], 0
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 1e4h
        push   eax
        call   sub_2_7A95
        add    esp, 4
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 1f2h], eax
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_2_2450
loc_2_244C:
        inc    word ptr [ebp - 4]
loc_2_2450:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        db  03Bh, 005h, 054h, 00Ah, 000h, 000h
        jae    NEAR PTR loc_2_24CB
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + eax*4 + 24h], 0
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        shl    eax, 2
        add    eax, dword ptr [ebp - 10h]
        add    eax, 28h
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 0ch], eax
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [ecx + 10h], ax
        jmp    NEAR PTR loc_2_244C
loc_2_24CB:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 150h
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_2_24EF
loc_2_24EB:
        inc    word ptr [ebp - 4]
loc_2_24EF:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        db  03Bh, 005h, 054h, 00Ah, 000h, 000h
        jae    NEAR PTR loc_2_256E
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3040
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 10h], 1
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 11h], al
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_2_254B
        mov    eax, dword ptr [ebp - 8]
        db  0C7h, 040h, 020h, 0D5h, 027h, 000h, 000h
        jmp    NEAR PTR loc_2_2555
loc_2_254B:
        mov    eax, dword ptr [ebp - 8]
        db  0C7h, 040h, 020h, 04Eh, 02Bh, 000h, 000h
loc_2_2555:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_80E0
        add    esp, 0ch
        jmp    NEAR PTR loc_2_24EB
loc_2_256E:
        sub    ax, ax
        jmp    NEAR PTR loc_2_2576
loc_2_2576:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_257B:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 12h
        mov    dword ptr [ebp - 8], eax
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_2_259C
loc_2_2598:
        inc    word ptr [ebp - 4]
loc_2_259C:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        db  03Bh, 005h, 054h, 00Ah, 000h, 000h
        jae    NEAR PTR loc_2_2646
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [ecx + eax*4 + 24h]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_2641
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        shl    eax, 2
        add    eax, dword ptr [ebp + 0ch]
        add    eax, 28h
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_2_2622
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 4], 0
        je     NEAR PTR loc_2_2622
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
loc_2_2622:
        mov    ax, word ptr [ebp - 4]
        push   eax
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_81A6
        add    esp, 10h
        mov    byte ptr [ebp - 0ch], al
        jmp    NEAR PTR loc_2_2641
loc_2_2641:
        jmp    NEAR PTR loc_2_2598
loc_2_2646:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2F5B
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_2667:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 0ch], 2
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1104
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2e8h]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_26DC
loc_2_2698:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1B70
        add    esp, 8
        jmp    NEAR PTR loc_2_26F5
loc_2_26AD:
        mov    eax, dword ptr [ebp + 0ch]
        movsx  eax, byte ptr [eax + 21h]
        cmp    eax, 3
        jne    NEAR PTR loc_2_26CD
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1B70
        add    esp, 8
loc_2_26CD:
        jmp    NEAR PTR loc_2_26F5
        db  0E9h, 01Eh, 000h, 000h, 000h, 0E9h, 019h, 000h, 000h, 000h
loc_2_26DC:
        cmp    dword ptr [ebp - 4], 1
        je     NEAR PTR loc_2_26AD
        cmp    dword ptr [ebp - 4], 2
        je     NEAR PTR loc_2_2698
        jmp    NEAR PTR loc_2_26F5
loc_2_26F5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_26FA:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        add    eax, eax
        lea    eax, [eax + eax*8]
        add    eax, dword ptr [ebp + 8]
        add    eax, 14h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        add    eax, eax
        lea    eax, [eax + eax*8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + ecx + 24h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_2_27C1
loc_2_273B:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_322C
        add    esp, 8
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_276F
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_28F7
        add    esp, 0ch
        jmp    NEAR PTR loc_2_27A0
loc_2_276F:
        push   1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2383
        add    esp, 0ch
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_27A0
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_28F7
        add    esp, 0ch
loc_2_27A0:
        jmp    NEAR PTR loc_2_27D0
loc_2_27A5:
        push   1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_61F2
        add    esp, 0ch
        jmp    NEAR PTR loc_2_27D0
        db  0E9h, 00Fh, 000h, 000h, 000h
loc_2_27C1:
        cmp    dword ptr [ebp - 8], 1
        je     NEAR PTR loc_2_273B
        jmp    NEAR PTR loc_2_27A5
loc_2_27D0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_27D5:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 12h
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_27F5
loc_2_27F5:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_306A
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_2_7C30
        add    esp, 4
        mov    eax, dword ptr [ebp - 14h]
        dec    word ptr [eax + 10h]
        mov    eax, dword ptr [ebp - 14h]
        mov    ax, word ptr [eax + 10h]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_2831
        jmp    NEAR PTR loc_2_28F2
loc_2_2831:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_2866
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_257B
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1053
        add    esp, 4
        jmp    NEAR PTR loc_2_28F2
loc_2_2866:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_2_3172
        add    esp, 4
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_28D0
        mov    ax, word ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_28F7
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_257B
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_28CB
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1053
        add    esp, 4
loc_2_28CB:
        jmp    NEAR PTR loc_2_28F2
loc_2_28D0:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2667
        add    esp, 8
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_28F7
        add    esp, 0ch
loc_2_28F2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_28F7:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 1
        je     NEAR PTR loc_2_2919
        jmp    NEAR PTR loc_2_2A6F
loc_2_2919:
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_29FB
        push   10h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 13ch
        push   eax
        db  0E8h, 00Dh, 0DCh, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 134h], 1
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 28h], eax
        jmp    NEAR PTR loc_2_29BF
loc_2_295D:
        mov    eax, dword ptr [ebp + 0ch]
        movsx  eax, byte ptr [eax + 0fh]
        cmp    eax, 3
        jne    NEAR PTR loc_2_2984
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 138h], 4
        jmp    NEAR PTR loc_2_297F
loc_2_297F:
        jmp    NEAR PTR loc_2_2991
loc_2_2984:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 138h], 80000001h
loc_2_2991:
        jmp    NEAR PTR loc_2_29D8
loc_2_2996:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 138h], 80000002h
        jmp    NEAR PTR loc_2_29D8
loc_2_29A8:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 138h], 80000006h
        jmp    NEAR PTR loc_2_29D8
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_29BF:
        cmp    dword ptr [ebp - 28h], 1
        je     NEAR PTR loc_2_295D
        cmp    dword ptr [ebp - 28h], 2
        je     NEAR PTR loc_2_2996
        jmp    NEAR PTR loc_2_29A8
loc_2_29D8:
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 13bh], 80h
        je     NEAR PTR loc_2_29FB
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_02C4
        add    esp, 8
        jmp    NEAR PTR loc_2_2A6F
loc_2_29FB:
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 10h]
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 10h]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_2A6F
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 2e8h], 1
        jne    NEAR PTR loc_2_2A54
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7D31
        add    esp, 4
        mov    dword ptr [ebp - 24h], eax
        cmp    dword ptr [ebp - 24h], 0
        je     NEAR PTR loc_2_2A54
        jmp    NEAR PTR loc_2_2A44
loc_2_2A44:
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1B70
        add    esp, 8
loc_2_2A54:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 4], 2
        push   0
        push   467h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        call   dword ptr [eax + 8]
        add    esp, 0ch
loc_2_2A6F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_2A74:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        jmp    NEAR PTR loc_2_2A82
loc_2_2A82:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_322C
        add    esp, 8
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_2AA4
        jmp    NEAR PTR loc_2_2B49
loc_2_2AA4:
        push   2
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2383
        add    esp, 0ch
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_2B49
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8138
        add    esp, 0ch
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     eax, eax
        je     NEAR PTR loc_2_2AEC
        jmp    NEAR PTR loc_2_2B49
loc_2_2AEC:
        push   26h
        push   0
        db  0E8h, 00Bh, 0D5h, 0FDh, 0FFh
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_2_2B49
        push   10h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 036h, 0DAh, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 1eh], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 10h
        push   eax
        call   sub_2_7A95
        add    esp, 4
        db  068h, 0E3h, 02Dh, 000h, 000h
        push   4
        mov    eax, dword ptr [ebp - 8]
        add    eax, 10h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3D50
        add    esp, 10h
loc_2_2B49:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_2B4E:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 12h
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_2_2B6E
loc_2_2B6E:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_306A
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_2_7C30
        add    esp, 4
        mov    eax, dword ptr [ebp - 14h]
        dec    word ptr [eax + 10h]
        mov    eax, dword ptr [ebp - 14h]
        mov    ax, word ptr [eax + 10h]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_2BAA
        jmp    NEAR PTR loc_2_2D9B
loc_2_2BAA:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_2BDF
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_257B
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1053
        add    esp, 4
        jmp    NEAR PTR loc_2_2D9B
loc_2_2BDF:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_2_3172
        add    esp, 4
        and    eax, 0ffffh
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_2_2CDB
        db  0E9h, 001h, 001h, 000h, 000h
loc_2_2BFD:
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_2_2C0C
loc_2_2C08:
        inc    word ptr [ebp - 4]
loc_2_2C0C:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        db  03Bh, 005h, 054h, 00Ah, 000h, 000h
        jae    NEAR PTR loc_2_2C3C
        mov    ax, word ptr [ebp - 4]
        push   eax
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_81A6
        add    esp, 10h
        jmp    NEAR PTR loc_2_2C08
loc_2_2C3C:
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8138
        add    esp, 0ch
        mov    byte ptr [ebp - 0ch], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        or     eax, eax
        je     NEAR PTR loc_2_2C86
        jmp    NEAR PTR loc_2_2C65
loc_2_2C65:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2F5B
        add    esp, 8
        jmp    NEAR PTR loc_2_2D9B
loc_2_2C86:
        mov    eax, dword ptr [ebp - 14h]
        mov    byte ptr [eax + 0ch], 3
        db  068h, 0A0h, 02Dh, 000h, 000h
        push   4
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 1e4h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3D50
        add    esp, 10h
        jmp    NEAR PTR loc_2_2CAE
loc_2_2CAE:
        jmp    NEAR PTR loc_2_2D9B
loc_2_2CB3:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_257B
        add    esp, 8
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    NEAR PTR loc_2_2D9B
        db  0E9h, 023h, 000h, 000h, 000h
loc_2_2CDB:
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_2_2CFE
        cmp    dword ptr [ebp - 18h], 1
        jl     NEAR PTR loc_2_2CB3
        cmp    dword ptr [ebp - 18h], 2
        jle    NEAR PTR loc_2_2BFD
        jmp    NEAR PTR loc_2_2CB3
loc_2_2CFE:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2667
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 2e8h], 1
        jne    NEAR PTR loc_2_2D4C
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7D31
        add    esp, 4
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_2_2D4C
        jmp    NEAR PTR loc_2_2D3C
loc_2_2D3C:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1B70
        add    esp, 8
loc_2_2D4C:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7C6B
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_2_2D8D
        jmp    NEAR PTR loc_2_2D65
loc_2_2D65:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_2_7C4D
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 2f0h], 0
        je     NEAR PTR loc_2_2D8D
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0B5A
        add    esp, 4
loc_2_2D8D:
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3A33
        add    esp, 8
loc_2_2D9B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_2DA0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        jmp    NEAR PTR loc_2_2DAB
loc_2_2DAB:
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 12h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_81A6
        add    esp, 10h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_2F5B
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_2DE3:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        jmp    NEAR PTR loc_2_2DEE
loc_2_2DEE:
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_81A6
        add    esp, 10h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0E8h, 029h, 0D2h, 0FDh, 0FFh
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_2E13:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        inc    word ptr [eax + 0eh]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_2E25:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        dec    word ptr [eax + 0eh]
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 0ch]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_2E8A
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_2E8A
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_257B
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_2E8A
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1053
        add    esp, 4
loc_2_2E8A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_2E8F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 0ch], 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 190h]
        mov    dword ptr [ebp - 4], eax
loc_2_2EAB:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 190h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_2_2F04
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0ch]
        and    eax, 0ffh
        cmp    eax, 3
        jne    NEAR PTR loc_2_2EF7
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 44h], ecx
        jne    NEAR PTR loc_2_2EF7
        push   2
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5A59
        add    esp, 0ch
loc_2_2EF7:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_2EAB
loc_2_2F04:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_2F09:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 14ch], 0
        je     NEAR PTR loc_2_2F4E
        push   1fch
        push   0
        db  0E8h, 0CBh, 0D0h, 0FDh, 0FFh
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_2F4E
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 14ch]
loc_2_2F4E:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_2F56
loc_2_2F56:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_2F5B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 14ch]
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0E8h, 0BEh, 0D0h, 0FDh, 0FFh
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_2F81:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_2_2F9D
loc_2_2F95:
        inc    word ptr [ebp - 4]
        add    dword ptr [ebp - 0ch], 74h
loc_2_2F9D:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        db  03Bh, 005h, 054h, 00Ah, 000h, 000h
        jae    NEAR PTR loc_2_3033
        call   sub_2_7BC5
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_2_3019
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_2_2FD2
loc_2_2FCE:
        inc    word ptr [ebp - 8]
loc_2_2FD2:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_2_3010
        mov    eax, dword ptr [ebp + 8]
        add    eax, 15ch
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_2_7C30
        add    esp, 4
        jmp    NEAR PTR loc_2_2FCE
loc_2_3010:
        mov    ax, 1
        jmp    NEAR PTR loc_2_303B
loc_2_3019:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 15ch
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        jmp    NEAR PTR loc_2_2F95
loc_2_3033:
        sub    ax, ax
        jmp    NEAR PTR loc_2_303B
loc_2_303B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_3040:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 15ch
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_3065
loc_2_3065:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_306A:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 11h]
        and    eax, 0ffff00ffh
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        shl    eax, 2
        add    eax, dword ptr [ebp - 0ch]
        add    eax, 24h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_2_3121
loc_2_30B9:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 37h]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        mov    edx, ecx
        shl    ecx, 3
        sub    ecx, edx
        mov    edx, dword ptr [ebp - 0ch]
        mov    byte ptr [edx + ecx*4 + 26h], al
        jmp    NEAR PTR loc_2_316D
loc_2_30DB:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        or     eax, 4
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
loc_2_30EF:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        or     eax, 1
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        jmp    NEAR PTR loc_2_316D
loc_2_3108:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        or     eax, 2
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        jmp    NEAR PTR loc_2_316D
loc_2_3121:
        cmp    dword ptr [ebp - 10h], 0eh
        jg     NEAR PTR loc_2_314A
        je     NEAR PTR loc_2_30DB
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_2_30B9
        cmp    dword ptr [ebp - 10h], 0dh
        je     NEAR PTR loc_2_30EF
        jmp    NEAR PTR loc_2_3108
loc_2_314A:
        cmp    dword ptr [ebp - 10h], 15h
        jl     NEAR PTR loc_2_3108
        cmp    dword ptr [ebp - 10h], 16h
        jle    NEAR PTR loc_2_30EF
        cmp    dword ptr [ebp - 10h], 19h
        je     NEAR PTR loc_2_30EF
        jmp    NEAR PTR loc_2_3108
loc_2_316D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_3172:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 8], 0
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_2_3190
loc_2_318C:
        inc    word ptr [ebp - 4]
loc_2_3190:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        db  03Bh, 005h, 054h, 00Ah, 000h, 000h
        jae    NEAR PTR loc_2_31D4
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*4 + 24h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        or     eax, ecx
        mov    word ptr [ebp - 8], ax
        jmp    NEAR PTR loc_2_318C
loc_2_31D4:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   al, 2
        je     NEAR PTR loc_2_31ED
        mov    ax, 3
        jmp    NEAR PTR loc_2_3227
loc_2_31ED:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   al, 4
        je     NEAR PTR loc_2_3206
        mov    ax, 2
        jmp    NEAR PTR loc_2_3227
loc_2_3206:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   al, 1
        je     NEAR PTR loc_2_321F
        mov    ax, 1
        jmp    NEAR PTR loc_2_3227
loc_2_321F:
        sub    ax, ax
        jmp    NEAR PTR loc_2_3227
loc_2_3227:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_322C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 150h]
        mov    dword ptr [ebp - 4], eax
loc_2_3241:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 150h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_2_3289
        push   10h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 12h
        push   eax
        db  0E8h, 0C7h, 0D2h, 0FDh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_2_327C
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_3284
loc_2_327C:
        sub    ax, ax
        jmp    NEAR PTR loc_2_3292
loc_2_3284:
        jmp    NEAR PTR loc_2_3241
loc_2_3289:
        mov    ax, 1
        jmp    NEAR PTR loc_2_3292
loc_2_3292:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_32A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_2_32B8
loc_2_32B4:
        inc    word ptr [ebp - 4]
loc_2_32B8:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    eax, 6
        jge    NEAR PTR loc_2_32ED
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        lea    eax, [eax + eax*2]
        shl    eax, 2
        add    eax, dword ptr [ebp + 8]
        add    eax, 260h
        push   eax
        call   sub_2_7A70
        add    esp, 4
        jmp    NEAR PTR loc_2_32B4
loc_2_32ED:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_32F2:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 34h]
        and    eax, 7fh
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_2_33AA
loc_2_330C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_2_332F
        mov    eax, dword ptr [ebp + 8]
        add    eax, 278h
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_333A
loc_2_332F:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 29ch
        mov    dword ptr [ebp - 4], eax
loc_2_333A:
        jmp    NEAR PTR loc_2_3415
loc_2_333F:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_2_3362
        mov    eax, dword ptr [ebp + 8]
        add    eax, 26ch
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_336D
loc_2_3362:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 290h
        mov    dword ptr [ebp - 4], eax
loc_2_336D:
        jmp    NEAR PTR loc_2_3415
loc_2_3372:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_2_3395
        mov    eax, dword ptr [ebp + 8]
        add    eax, 260h
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_33A0
loc_2_3395:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 284h
        mov    dword ptr [ebp - 4], eax
loc_2_33A0:
        jmp    NEAR PTR loc_2_3415
        db  0E9h, 06Bh, 000h, 000h, 000h
loc_2_33AA:
        sub    dword ptr [ebp - 8], 10h
        cmp    dword ptr [ebp - 8], 13h
        ja     NEAR PTR loc_2_3372
        shl    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        db  0FFh, 0A0h, 0C5h, 033h, 000h, 000h
loc_2_33C5:
        or     al, 33h
        add    byte ptr [eax], al
        or     al, 33h
        add    byte ptr [eax], al
        db  072h, 033h
        add    byte ptr [eax], al
        jb     SHORT loc_2_3406
        add    byte ptr [eax], al
        aas
        xor    eax, dword ptr [eax]
        db  000h, 00Ch, 033h
        add    byte ptr [eax], al
        or     al, 33h
        add    byte ptr [eax], al
        db  072h, 033h
        add    byte ptr [eax], al
        db  072h, 033h
        add    byte ptr [eax], al
        db  072h, 033h
        add    byte ptr [eax], al
        jb     SHORT loc_2_3422
        add    byte ptr [eax], al
        jb     SHORT loc_2_3426
        add    byte ptr [eax], al
        jb     SHORT sub_2_342A
        add    byte ptr [eax], al
        db  072h, 033h
        add    byte ptr [eax], al
        jb     SHORT loc_2_3432
        add    byte ptr [eax], al
        jb     SHORT loc_2_3436
        add    byte ptr [eax], al
        aas
loc_2_3406:
        xor    eax, dword ptr [eax]
        db  000h, 00Ch, 033h
        add    byte ptr [eax], al
        aas
        xor    eax, dword ptr [eax]
        db  000h, 00Ch, 033h
        add    byte ptr [eax], al
loc_2_3415:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_7AAA
loc_2_3422:
        add    esp, 8
        pop    edi
loc_2_3426:
        pop    esi
        pop    ebx
        leave
        ret
sub_2_342A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
loc_2_3432:
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
loc_2_3436:
        add    eax, 34h
        mov    dword ptr [ebp - 4], eax
        push   ebx
        mov    ebx, dword ptr [ebp - 4]
        db  0E8h, 0D4h, 0CFh, 0FDh, 0FFh
        pop    ebx
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_344B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        db  0C7h, 040h, 060h, 019h, 004h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 34h
        push   eax
        db  0E8h, 0A1h, 0D1h, 0FDh, 0FFh
        add    esp, 4
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     eax, eax
        je     NEAR PTR loc_2_34BD
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 35h], al
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 65h], al
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_32F2
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_342A
        add    esp, 8
        jmp    NEAR PTR loc_2_34CD
loc_2_34BD:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_32F2
        add    esp, 8
loc_2_34CD:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_34D2:
        push   ebp
        mov    ebp, esp
        sub    esp, 40h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 65h]
        and    eax, 0ffh
        cmp    eax, 0ffh
        jne    NEAR PTR loc_2_3523
        push   40h
        push   0
        lea    eax, [ebp - 40h]
        push   eax
        db  0E8h, 063h, 0D0h, 0FDh, 0FFh
        add    esp, 0ch
        mov    byte ptr [ebp - 40h], 35h
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 34h
        mov    dword ptr [ebp - 3ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 64h]
        mov    byte ptr [ebp - 10h], al
        lea    eax, [ebp - 40h]
        push   eax
        db  0E8h, 0F4h, 0D0h, 0FDh, 0FFh
        add    esp, 4
loc_2_3523:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_3530:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 252h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2f4h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 254h], ax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_355A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 252h]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_3583
        mov    al, 1
        jmp    NEAR PTR loc_2_358A
        db  0E9h, 007h, 000h, 000h, 000h
loc_2_3583:
        sub    al, al
        jmp    NEAR PTR loc_2_358A
loc_2_358A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_358F:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 252h]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_35B4
        jmp    NEAR PTR loc_2_3702
loc_2_35B4:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_35CD
        jmp    NEAR PTR loc_2_3702
loc_2_35CD:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 26ch]
        mov    dword ptr [ebp - 0ch], eax
loc_2_35D9:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 26ch
        cmp    eax, dword ptr [ebp - 0ch]
        je     NEAR PTR loc_2_361D
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 34h]
        and    eax, 0ffh
        cmp    eax, 94h
        jne    NEAR PTR loc_2_3618
        jmp    NEAR PTR loc_2_3613
loc_2_3613:
        jmp    NEAR PTR loc_2_3702
loc_2_3618:
        jmp    NEAR PTR loc_2_35D9
loc_2_361D:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_3655
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7C6B
        add    esp, 4
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_2_3650
        jmp    NEAR PTR loc_2_364B
loc_2_364B:
        jmp    NEAR PTR loc_2_3702
loc_2_3650:
        jmp    NEAR PTR loc_2_3678
loc_2_3655:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3C73
        add    esp, 4
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_2_3678
        jmp    NEAR PTR loc_2_3673
loc_2_3673:
        jmp    NEAR PTR loc_2_3702
loc_2_3678:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3B93
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_2_3696
        jmp    NEAR PTR loc_2_3702
loc_2_3696:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2f4h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 254h], ax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 8]
        db  0C7h, 040h, 020h, 007h, 037h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 34h], 0b3h
        push   10h
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 12h
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 3eh
        push   eax
        db  0E8h, 074h, 0CEh, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 2b4h]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 64h], al
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_344B
        add    esp, 8
loc_2_3702:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_3707:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 38h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_3742
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3C28
        add    esp, 8
        jmp    NEAR PTR loc_2_39E1
loc_2_3742:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 252h]
        and    eax, 0ffffh
        cmp    eax, 2
        jne    NEAR PTR loc_2_3774
        jmp    NEAR PTR loc_2_375F
loc_2_375F:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3C28
        add    esp, 8
        jmp    NEAR PTR loc_2_39E1
loc_2_3774:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 3ah]
        and    eax, 0ffffh
        cmp    eax, 0ffh
        jle    NEAR PTR loc_2_37A0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3C28
        add    esp, 8
        jmp    NEAR PTR loc_2_39E1
loc_2_37A0:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_2_3844
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        and    eax, 0ffh
        cmp    eax, 5
        jne    NEAR PTR loc_2_382F
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 3eh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_6127
        add    esp, 8
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_2_382F
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3C28
        add    esp, 8
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_2_3CD8
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3C73
        add    esp, 4
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_2_382A
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_358F
        add    esp, 8
loc_2_382A:
        jmp    NEAR PTR loc_2_383F
loc_2_382F:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3C28
        add    esp, 8
loc_2_383F:
        jmp    NEAR PTR loc_2_39E1
loc_2_3844:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 150h]
        mov    dword ptr [ebp - 10h], eax
loc_2_3850:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 150h
        cmp    eax, dword ptr [ebp - 10h]
        je     NEAR PTR loc_2_38B6
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 0ch]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_2_38B1
        mov    eax, dword ptr [ebp - 18h]
        add    eax, 12h
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 38h]
        push   eax
        call   sub_2_3ACC
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_2_38B1
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0CBC
        add    esp, 8
loc_2_38B1:
        jmp    NEAR PTR loc_2_3850
loc_2_38B6:
        jmp    NEAR PTR loc_2_38BB
loc_2_38BB:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3CF6
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7C6B
        add    esp, 4
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_2_394B
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7CD0
        add    esp, 4
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_2_390A
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_2_7C4D
        add    esp, 4
        jmp    NEAR PTR loc_2_392F
loc_2_390A:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_7DD3
        add    esp, 4
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_2_392F
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_2_7C4D
        add    esp, 4
loc_2_392F:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 2f0h], 0
        je     NEAR PTR loc_2_394B
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0B5A
        add    esp, 4
loc_2_394B:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 174h]
        mov    dword ptr [ebp - 10h], eax
loc_2_3957:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 174h
        cmp    eax, dword ptr [ebp - 10h]
        je     NEAR PTR loc_2_39D1
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 20h]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_2_39CC
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 21h]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_39CC
        mov    eax, dword ptr [ebp - 4]
        add    eax, 10h
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 38h]
        push   eax
        call   sub_2_3ACC
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_2_39CC
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_63A5
        add    esp, 8
loc_2_39CC:
        jmp    NEAR PTR loc_2_3957
loc_2_39D1:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3C28
        add    esp, 8
loc_2_39E1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_39E6:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 254h]
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 254h]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_3A2E
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_358F
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2f4h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 254h], ax
loc_2_3A2E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_3A33:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_3AAE
loc_2_3A4C:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 252h]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_3A7B
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2f4h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 254h], ax
        jmp    NEAR PTR loc_2_3A87
loc_2_3A7B:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 252h], 2
loc_2_3A87:
        jmp    NEAR PTR loc_2_3AC7
loc_2_3A8C:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2f4h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 254h], ax
        jmp    NEAR PTR loc_2_3AC7
        db  0E9h, 01Eh, 000h, 000h, 000h, 0E9h, 019h, 000h, 000h, 000h
loc_2_3AAE:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_3A4C
        cmp    dword ptr [ebp - 4], 1
        je     NEAR PTR loc_2_3A8C
        jmp    NEAR PTR loc_2_3AC7
loc_2_3AC7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_3ACC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_2_3AEA
loc_2_3AE6:
        inc    word ptr [ebp - 8]
loc_2_3AEA:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 3ah]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_2_3B84
        push   10h
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        add    eax, eax
        lea    eax, [eax + eax*8]
        add    eax, dword ptr [ebp - 4]
        add    eax, 3ch
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0E8h, 005h, 0CAh, 0FDh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_2_3B7F
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        add    eax, eax
        lea    eax, [eax + eax*8]
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + ecx + 4dh]
        and    eax, 7
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_2_3B66
loc_2_3B50:
        sub    eax, eax
        jmp    NEAR PTR loc_2_3B8E
loc_2_3B57:
        mov    eax, 1
        jmp    NEAR PTR loc_2_3B8E
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_3B66:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_2_3B50
        cmp    dword ptr [ebp - 0ch], 4
        je     NEAR PTR loc_2_3B50
        jmp    NEAR PTR loc_2_3B57
loc_2_3B7F:
        jmp    NEAR PTR loc_2_3AE6
loc_2_3B84:
        mov    eax, 1
        jmp    NEAR PTR loc_2_3B8E
loc_2_3B8E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_3B93:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   129eh
        push   0
        db  0E8h, 058h, 0C4h, 0FDh, 0FFh
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_2_3BBF
        sub    eax, eax
        jmp    NEAR PTR loc_2_3C23
loc_2_3BBF:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 252h], 1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   40h
        push   0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 34h
        push   eax
        db  0E8h, 080h, 0C9h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_7A95
        add    esp, 4
        push   8
        db  068h, 074h, 008h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        add    eax, 2ch
        push   eax
        db  0E8h, 047h, 0C9h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        add    eax, 74h
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 38h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 3ch], 122ah
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_2_3C23
loc_2_3C23:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_3C28:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0E8h, 0FAh, 0C3h, 0FDh, 0FFh
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 252h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_3C6E
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0ECA
        add    esp, 8
loc_2_3C6E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_3C73:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 150h]
        mov    dword ptr [ebp - 4], eax
loc_2_3C88:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 150h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_2_3CCC
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0dh]
        and    eax, 0ffh
        test   al, 2
        je     NEAR PTR loc_2_3CBF
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_3CC7
loc_2_3CBF:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_2_3CD3
loc_2_3CC7:
        jmp    NEAR PTR loc_2_3C88
loc_2_3CCC:
        sub    eax, eax
        jmp    NEAR PTR loc_2_3CD3
loc_2_3CD3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_3CD8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0dh]
        and    eax, 0ffh
        or     al, 2
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0dh], al
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_3CF6:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 150h]
        mov    dword ptr [ebp - 4], eax
loc_2_3D0B:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 150h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_2_3D40
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0dh]
        and    eax, 0fdh
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 0dh], al
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_3D0B
loc_2_3D40:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_3D50:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2a8h]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_3D72
loc_2_3D6A:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_2_3D72:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 2a8h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_2_3DC0
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 10h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_2_3DC0
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 0ch]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp + 10h], ax
        jmp    NEAR PTR loc_2_3D6A
loc_2_3DC0:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 2a8h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 8], eax
        mov    ax, word ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 0ch], ax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 2a8h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_2_3E34
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ecx + 0ch]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0ch], ax
loc_2_3E34:
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 12h], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_3E42:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 2a8h
        cmp    dword ptr [eax + 8], ecx
        je     NEAR PTR loc_2_3E62
        jmp    NEAR PTR loc_2_3EA7
loc_2_3E62:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 2a8h
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    eax, dword ptr [ecx]
        je     NEAR PTR loc_2_3E9B
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ecx + 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx]
        mov    word ptr [ecx + 0ch], ax
loc_2_3E9B:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_2_7AE6
        add    esp, 4
loc_2_3EA7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_3EAC:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 2a8h
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_3ED8
        jmp    NEAR PTR loc_2_3F62
loc_2_3ED8:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2a8h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        dec    word ptr [eax + 0ch]
loc_2_3EF1:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 2a8h
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_3F62
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_2_3F62
        mov    eax, dword ptr [ebp + 8]
        add    eax, 2a8h
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0eh]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        call   dword ptr [eax + 12h]
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2a8h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_3EF1
loc_2_3F62:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_3F70:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1f4h], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 1f8h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 20ch], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 210h
        push   eax
        call   sub_2_7A70
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        add    eax, 21ch
        push   eax
        call   sub_2_7A70
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_3FE8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1f8h]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_2_400C
        jmp    NEAR PTR loc_2_4083
loc_2_400C:
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4E46
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_2_403A
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_03DD
        add    esp, 8
        jmp    NEAR PTR loc_2_4083
loc_2_403A:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1f8h], 1
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 1f8h
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp - 4]
        db  0C7h, 040h, 020h, 08Eh, 043h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2e0h]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 3ch], ax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_84A8
        add    esp, 8
loc_2_4083:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_4088:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [ecx + eax*4 + 28h]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_2_40BC
        sub    ax, ax
        jmp    NEAR PTR loc_2_4174
loc_2_40BC:
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4E46
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_2_40E0
        mov    ax, 1
        jmp    NEAR PTR loc_2_4174
loc_2_40E0:
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + eax*4 + 28h], 1
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 10h], 1
        mov    al, byte ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 11h], al
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        shl    eax, 2
        add    eax, dword ptr [ebp + 0ch]
        add    eax, 28h
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp - 4]
        db  0C7h, 040h, 020h, 08Eh, 043h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2e0h]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 3ch], ax
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [ecx + eax*4 + 26h]
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_84EA
        add    esp, 0ch
        sub    ax, ax
        jmp    NEAR PTR loc_2_4174
loc_2_4174:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_4179:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 1
        mov    eax, dword ptr [ebp + 0ch]
        db  0C7h, 040h, 020h, 08Eh, 043h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2e0h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 3ch], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_2_41D3
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_84A8
        add    esp, 8
        jmp    NEAR PTR loc_2_420C
loc_2_41D3:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 11h]
        and    eax, 0ffh
        mov    ecx, eax
        shl    eax, 3
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [ecx + eax*4 + 26h]
        mov    byte ptr [ebp - 0ch], al
        mov    al, byte ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_84EA
        add    esp, 0ch
loc_2_420C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_4211:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        jmp    NEAR PTR loc_2_421F
loc_2_421F:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 0f4h], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_438E
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_4250:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        jmp    NEAR PTR loc_2_425B
loc_2_425B:
        mov    eax, dword ptr [ebp + 0ch]
        db  0C7h, 040h, 020h, 0B9h, 046h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_8536
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_427A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_2_42E0
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 10h], 1
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 0dah]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 11h], al
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 0dbh]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 36h], al
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 6], 0
        mov    eax, dword ptr [ebp + 0ch]
        db  0C7h, 040h, 020h, 0B8h, 048h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_85AB
        add    esp, 8
        jmp    NEAR PTR loc_2_4389
loc_2_42E0:
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 0d8h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 36h], al
        mov    eax, dword ptr [ebp + 8]
        add    eax, 210h
        push   eax
        call   sub_2_7B96
        add    esp, 4
        and    eax, 0ffffh
        cmp    eax, 1
        je     NEAR PTR loc_2_432D
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 20ch]
        and    eax, 0ffffh
        cmp    eax, 2
        jne    NEAR PTR loc_2_4342
loc_2_432D:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4AC2
        add    esp, 8
        jmp    NEAR PTR loc_2_4389
loc_2_4342:
        mov    eax, dword ptr [ebp + 0ch]
        db  0C7h, 040h, 020h, 0B8h, 048h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 20ch]
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_85AB
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 6], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1B3F
        add    esp, 4
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_3A33
        add    esp, 8
loc_2_4389:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_438E:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 10h]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_43C9
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        jmp    NEAR PTR loc_2_46B4
loc_2_43C9:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        mov    byte ptr [ebp - 14h], al
        jmp    NEAR PTR loc_2_43D7
loc_2_43D7:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5095
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_2_4409
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        jmp    NEAR PTR loc_2_46B4
loc_2_4409:
        mov    al, byte ptr [ebp - 4]
        push   eax
        mov    al, byte ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4FD6
        add    esp, 0ch
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_4448
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 1ch]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        jmp    NEAR PTR loc_2_46B4
loc_2_4448:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_2_467D
loc_2_445B:
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4CD7
        add    esp, 0ch
        and    eax, 0ffffh
        mov    dword ptr [ebp - 1ch], eax
        jmp    NEAR PTR loc_2_4635
loc_2_447C:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4179
        add    esp, 8
        jmp    NEAR PTR loc_2_4658
loc_2_4491:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 3
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 0f4h], eax
        jmp    NEAR PTR loc_2_4658
loc_2_44B1:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_2_4602
loc_2_44C4:
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4E46
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_2_4518
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_2_4F9B
        add    esp, 8
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_427A
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4179
        add    esp, 8
        jmp    NEAR PTR loc_2_452E
loc_2_4518:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 4
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4A1D
        add    esp, 8
loc_2_452E:
        jmp    NEAR PTR loc_2_461B
loc_2_4533:
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4E46
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_2_45B9
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_2_4F9B
        add    esp, 8
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 38h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2e0h]
        mov    ecx, dword ptr [ebp - 0ch]
        add    dword ptr [ecx + 38h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 3ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 2e0h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [ecx + 3ch], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 2
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4250
        add    esp, 8
        jmp    NEAR PTR loc_2_45CF
loc_2_45B9:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 4
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4A1D
        add    esp, 8
loc_2_45CF:
        jmp    NEAR PTR loc_2_461B
loc_2_45D4:
        mov    al, byte ptr [ebp - 4]
        and    eax, 0ffff00ffh
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5A59
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4179
        add    esp, 8
        jmp    NEAR PTR loc_2_461B
loc_2_4602:
        cmp    dword ptr [ebp - 20h], 0
        je     NEAR PTR loc_2_44C4
        cmp    dword ptr [ebp - 20h], 6
        je     NEAR PTR loc_2_4533
        jmp    NEAR PTR loc_2_45D4
loc_2_461B:
        jmp    NEAR PTR loc_2_4658
loc_2_4620:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4179
        add    esp, 8
        jmp    NEAR PTR loc_2_4658
loc_2_4635:
        cmp    dword ptr [ebp - 1ch], 0
        je     NEAR PTR loc_2_447C
        cmp    dword ptr [ebp - 1ch], 1
        je     NEAR PTR loc_2_4491
        cmp    dword ptr [ebp - 1ch], 2
        je     NEAR PTR loc_2_44B1
        jmp    NEAR PTR loc_2_4620
loc_2_4658:
        jmp    NEAR PTR loc_2_46B4
loc_2_465D:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 0
        jmp    NEAR PTR loc_2_46B4
        db  0E9h, 037h, 000h, 000h, 000h
loc_2_467D:
        cmp    dword ptr [ebp - 18h], 0
        jl     NEAR PTR loc_2_465D
        cmp    dword ptr [ebp - 18h], 1
        jle    NEAR PTR loc_2_445B
        cmp    dword ptr [ebp - 18h], 6
        je     NEAR PTR loc_2_445B
        cmp    dword ptr [ebp - 18h], 0ah
        je     NEAR PTR loc_2_445B
        cmp    dword ptr [ebp - 18h], 18h
        je     NEAR PTR loc_2_445B
        jmp    NEAR PTR loc_2_465D
loc_2_46B4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_46B9:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 10h]
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5095
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_472A
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_4725
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 0
loc_2_4725:
        jmp    NEAR PTR loc_2_48B3
loc_2_472A:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        mov    byte ptr [ebp - 10h], al
        jmp    NEAR PTR loc_2_4738
loc_2_4738:
        mov    al, byte ptr [ebp - 8]
        push   eax
        mov    al, byte ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4FD6
        add    esp, 0ch
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_4791
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_478C
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 0
loc_2_478C:
        jmp    NEAR PTR loc_2_48B3
loc_2_4791:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_2_47B0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        jmp    NEAR PTR loc_2_48B3
loc_2_47B0:
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4CD7
        add    esp, 0ch
        and    eax, 0ffffh
        cmp    eax, 2
        jne    NEAR PTR loc_2_4886
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_2_4858
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    ax, word ptr [eax + 3ch]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 38h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        push   eax
        db  0E8h, 03Fh, 0BDh, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 38h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    ax, word ptr [eax + 3ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ecx + 3ch]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 3ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_427A
        add    esp, 0ch
        jmp    NEAR PTR loc_2_4881
loc_2_4858:
        mov    al, byte ptr [ebp - 8]
        and    eax, 0ffff00ffh
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5A59
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
loc_2_4881:
        jmp    NEAR PTR loc_2_4896
loc_2_4886:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
loc_2_4896:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4179
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 4], 0
loc_2_48B3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_48B8:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 10h]
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     eax, eax
        jne    NEAR PTR loc_2_48E1
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 20ch]
loc_2_48E1:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_2_490A
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        jmp    NEAR PTR loc_2_4A18
loc_2_490A:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        mov    byte ptr [ebp - 0ch], al
        jmp    NEAR PTR loc_2_4918
loc_2_4918:
        mov    al, byte ptr [ebp - 4]
        push   eax
        mov    al, byte ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4FD6
        add    esp, 0ch
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_2_494E
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        jmp    NEAR PTR loc_2_4A18
loc_2_494E:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 35h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_2_49C4
        db  0E9h, 06Dh, 000h, 000h, 000h
loc_2_4966:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4CD7
        add    esp, 0ch
        and    eax, 0ffffh
        cmp    eax, 2
        jne    NEAR PTR loc_2_499A
        push   2
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_5A59
        add    esp, 0ch
loc_2_499A:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     eax, eax
        jne    NEAR PTR loc_2_49BA
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 36h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4B64
        add    esp, 8
loc_2_49BA:
        jmp    NEAR PTR loc_2_49D3
        db  0E9h, 00Fh, 000h, 000h, 000h
loc_2_49C4:
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_2_49D3
        jmp    NEAR PTR loc_2_4966
loc_2_49D3:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4F34
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ech]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 6], ax
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     eax, eax
        jne    NEAR PTR loc_2_4A18
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4AE7
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_4A42
        add    esp, 4
loc_2_4A18:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_4A1D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        jmp    NEAR PTR loc_2_4A28
loc_2_4A28:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 21ch
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_4A42:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_2_7A70
        add    esp, 4
loc_2_4A57:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 21ch
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_2_4A8A
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        jmp    NEAR PTR loc_2_4A57
loc_2_4A8A:
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_2_7B16
        add    esp, 4
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_2_4ABD
        jmp    NEAR PTR loc_2_4AA8
loc_2_4AA8:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_438E
        add    esp, 8
        jmp    NEAR PTR loc_2_4A8A
loc_2_4ABD:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_4AC2:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        jmp    NEAR PTR loc_2_4ACD
loc_2_4ACD:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 210h
        push   eax
        call   sub_2_7AAA
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
_PTEXT ends
        end
