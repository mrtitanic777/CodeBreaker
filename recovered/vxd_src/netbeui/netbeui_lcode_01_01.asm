; netbeui_lcode_01_01.asm -- NETBEUI object 1 segment _LTEXT  (part 1/2)
.386p
_LTEXT segment dword public use32 'LCODE'

NETBEUI_Control:
        cmp    eax, 1
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        cmp    eax, 1bh
        db  00Fh, 084h, 0EEh, 0FFh, 0FEh, 0FFh
        clc
        ret
loc_1_0014:
        mov    ax, 100h
        clc
        ret
        db  000h, 000h
sub_1_001C:
        enter  10h, 0
        mov    dword ptr [ebp - 8], ebx
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_1_6CDC
        mov    eax, dword ptr [ebp - 4]
        test   eax, eax
        je     SHORT loc_1_0071
        lea    ecx, [ebp - 0ch]
        push   ecx
        lea    ecx, [ebp - 10h]
        push   ecx
        push   eax
        call   sub_1_6CC4
        mov    ecx, dword ptr [ebp - 10h]
        cmp    dword ptr [edi + 3ch], 0
        je     SHORT loc_1_0059
        mov    eax, dword ptr [ebx + 124h]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ebx + 124h], ecx
loc_1_0059:
        push   dword ptr [ebp - 4]
        call   sub_1_6CBE
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_1_6CDC
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_0059
loc_1_0071:
        push   edi
        call   sub_1_6C9A
        mov    ebx, dword ptr [ebp - 8]
        leave
        ret
sub_1_007C:
        push   edi
        push   ebx
        push   ecx
        cmp    dword ptr [esi + 28h], 2
        jne    SHORT loc_1_0097
        or     dword ptr [esi + 4], 2
        mov    dword ptr [esi + 28h], 0
        mov    ebx, 4010102h
        jmp    SHORT loc_1_00A6
loc_1_0097:
        and    dword ptr [esi + 4], 0fffffffdh
        cmp    dword ptr [esi + 28h], 0
        jne    SHORT loc_1_00DD
        mov    ebx, 1010102h
loc_1_00A6:
        db  0C7h, 046h, 044h, 02Ch, 007h, 000h, 000h
        mov    ecx, 6
        lea    edi, [esi + 31h]
        call   sub_1_02F2
        jb     NEAR PTR loc_1_020D
        db  08Dh, 03Dh, 069h, 06Eh, 000h, 000h
        mov    ecx, 6
        mov    ebx, 1010103h
        call   sub_1_037F
        jb     NEAR PTR loc_1_020D
        jmp    SHORT loc_1_014B
loc_1_00DD:
        cmp    dword ptr [esi + 28h], 1
        je     SHORT loc_1_00E9
        stc
        jmp    NEAR PTR loc_1_020D
loc_1_00E9:
        db  0C7h, 046h, 044h, 002h, 007h, 000h, 000h
        mov    ebx, 2010101h
        db  080h, 03Dh, 035h, 06Fh, 000h, 000h, 000h
        je     SHORT loc_1_0103
        mov    ebx, 2010102h
loc_1_0103:
        mov    ecx, 6
        lea    edi, [esi + 31h]
        call   sub_1_02F2
        jb     NEAR PTR loc_1_020D
        xor    ecx, ecx
        mov    ebx, 2010103h
        call   sub_1_02F2
        jb     NEAR PTR loc_1_020D
        db  089h, 00Dh, 04Ch, 06Eh, 000h, 000h
        db  0A1h, 06Fh, 06Eh, 000h, 000h
        db  00Bh, 005h, 04Ch, 06Eh, 000h, 000h
        sub    ecx, ecx
        mov    ebx, 2010103h
        call   sub_1_037F
        jb     NEAR PTR loc_1_020D
loc_1_014B:
        test   dword ptr [esi + 4], 2
        je     SHORT loc_1_0193
        and    dword ptr [esi + 4], 0fffffffeh
        mov    dword ptr [esi + 8], 1
        db  08Dh, 03Dh, 073h, 06Eh, 000h, 000h
        mov    ecx, 6
        mov    ebx, 4010104h
        call   sub_1_037F
        jb     NEAR PTR loc_1_020D
        db  08Dh, 03Dh, 079h, 06Eh, 000h, 000h
        mov    ecx, 4
        mov    ebx, 4010106h
        call   sub_1_037F
        jb     SHORT loc_1_020D
        jmp    SHORT loc_1_01B5
loc_1_0193:
        db  00Fh, 0B6h, 00Dh, 02Ch, 06Fh, 000h, 000h
        mov    dword ptr [esi + 8], ecx
        mov    ebx, 10107h
        sub    ecx, ecx
        call   sub_1_02F2
        jb     SHORT loc_1_020D
        imul   ecx, ecx, 64h
        mov    dword ptr [esi + 40h], ecx
        or     dword ptr [esi + 4], 1
loc_1_01B5:
        sub    ecx, ecx
        mov    ebx, 10106h
        call   sub_1_02F2
        jb     SHORT loc_1_020D
        mov    word ptr [esi + 92h], cx
        mov    ebx, 10105h
        sub    ecx, ecx
        call   sub_1_02F2
        mov    eax, 80h
        cmp    ecx, eax
        jb     SHORT loc_1_020D
        sub    ecx, ecx
        mov    ebx, 1010fh
        call   sub_1_037F
        sub    ecx, ecx
        jb     SHORT loc_1_020D
        sub    ecx, ecx
        mov    ebx, 1010eh
        mov    eax, 3
        cmp    dword ptr [esi + 28h], 0
        je     SHORT loc_1_0206
        mov    eax, 4001h
loc_1_0206:
        call   sub_1_037F
        jb     SHORT loc_1_020D
loc_1_020D:
        pop    ecx
        pop    ebx
        pop    edi
        ret
sub_1_0211:
        enter  1ch, 0
        push   edi
        and    dword ptr [esi], 0fffffffdh
        lea    edi, [esi + 14h]
        mov    dword ptr [ebp - 18h], edi
        push   esi
        mov    esi, edi
        cld
loc_1_0223:
        lodsb
        or     al, al
        jne    SHORT loc_1_0223
        sub    esi, edi
        mov    word ptr [ebp - 1ch], si
        mov    word ptr [ebp - 1ah], si
        mov    dword ptr [ebp - 14h], 0
        mov    dword ptr [ebp - 10h], 1
        mov    dword ptr [ebp - 0ch], 3
        pop    esi
        xor    eax, eax
        push   eax
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        push   esi
        db  0FFh, 035h, 0B4h, 06Dh, 000h, 000h
        push   3
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [esi + 28h]
        push   eax
        lea    eax, [esi + 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_6C10
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, 103h
        je     SHORT loc_1_028D
        cmp    eax, 0
        jne    SHORT loc_1_028A
        and    dword ptr [esi], 0fffffdffh
loc_1_0287:
        pop    edi
        leave
        ret
loc_1_028A:
        stc
        jmp    SHORT loc_1_0287
loc_1_028D:
        mov    eax, dword ptr [esi + 130h]
        push   ecx
        mov    ecx, 5
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        pop    ecx
        test   dword ptr [esi], 200h
        je     SHORT loc_1_0287
        stc
        mov    eax, 0c0000001h
        jmp    SHORT loc_1_0287
sub_1_02B0:
        test   dword ptr [esi], 200h
        jne    SHORT loc_1_02F1
        push   dword ptr [esi + 10h]
        lea    eax, [esi + 2ch]
        push   eax
        call   sub_1_6C16
        cmp    dword ptr [esi + 2ch], 103h
        je     SHORT loc_1_02D5
        or     dword ptr [esi], 200h
        jmp    SHORT loc_1_02F1
loc_1_02D5:
        test   dword ptr [esi], 200h
        jne    SHORT loc_1_02F1
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        jmp    SHORT loc_1_02D5
loc_1_02F1:
        ret
sub_1_02F2:
        enter  30h, 0
        test   ecx, ecx
        mov    dword ptr [ebp - 0ch], 4
        lea    eax, [ebp - 2ch]
        je     SHORT loc_1_0309
        mov    dword ptr [ebp - 0ch], ecx
        mov    eax, edi
loc_1_0309:
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 18h], 0
        mov    dword ptr [ebp - 28h], 0
        mov    dword ptr [ebp - 14h], ebx
        mov    dword ptr [esi + 2ch], 0ffffffffh
        push   ebx
        push   ecx
        lea    eax, [ebp - 28h]
        push   eax
        mov    eax, dword ptr [esi + 10h]
        push   eax
        lea    eax, [ebp - 30h]
        push   eax
        call   sub_1_6C2E
        pop    ecx
        pop    ebx
        cmp    dword ptr [ebp - 30h], 0
        jne    SHORT loc_1_034E
loc_1_033F:
        test   ecx, ecx
        jne    SHORT loc_1_0348
        mov    ecx, dword ptr [ebp - 2ch]
        jmp    SHORT loc_1_034B
loc_1_0348:
        mov    ecx, dword ptr [ebp - 8]
loc_1_034B:
        clc
loc_1_034C:
        leave
        ret
loc_1_034E:
        cmp    dword ptr [ebp - 30h], 103h
        jne    SHORT loc_1_0379
loc_1_0357:
        cmp    dword ptr [esi + 2ch], -1
        jne    SHORT loc_1_0373
        push   ecx
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        pop    ecx
        jmp    SHORT loc_1_0357
loc_1_0373:
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_033F
loc_1_0379:
        mov    ecx, dword ptr [ebp - 4]
        stc
        jmp    SHORT loc_1_034C
sub_1_037F:
        enter  30h, 0
        push   eax
        push   ebx
        mov    dword ptr [ebp - 0ch], 4
        mov    dword ptr [ebp - 2ch], eax
        lea    eax, [ebp - 2ch]
        test   ecx, ecx
        je     SHORT loc_1_039B
        mov    dword ptr [ebp - 0ch], ecx
        mov    eax, edi
loc_1_039B:
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 18h], 1
        mov    dword ptr [ebp - 28h], 0
        mov    dword ptr [ebp - 14h], ebx
        mov    dword ptr [esi + 2ch], 0ffffffffh
        lea    eax, [ebp - 28h]
        push   eax
        mov    eax, dword ptr [esi + 10h]
        push   eax
        lea    eax, [ebp - 30h]
        push   eax
        call   sub_1_6C2E
        cmp    dword ptr [ebp - 30h], 0
        jne    SHORT loc_1_03D5
loc_1_03CD:
        mov    ecx, dword ptr [ebp - 8]
        clc
loc_1_03D1:
        pop    ebx
        pop    eax
        leave
        ret
loc_1_03D5:
        cmp    dword ptr [ebp - 30h], 103h
        jne    SHORT loc_1_0443
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        mov    ax, 0fdffh
        or     ax, word ptr [ebx + 2ch]
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
loc_1_040A:
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 038h, 000h, 001h, 000h
        cmp    dword ptr [esi + 2ch], -1
        je     SHORT loc_1_040A
        cmp    ax, 0ffffh
        jne    SHORT loc_1_0428
        db  0CDh, 020h, 01Bh, 000h, 001h, 000h
loc_1_0428:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_1_03CD
loc_1_0443:
        mov    ecx, dword ptr [ebp - 4]
        stc
        jmp    SHORT loc_1_03D1
sub_1_0449:
        test   word ptr [esi + 10h], 2
        jne    SHORT loc_1_04A8
        sub    eax, eax
        clc
        pushfd
        push   edi
        cli
        db  08Dh, 03Dh, 078h, 06Fh, 000h, 000h
        call   sub_1_2903
        sti
        je     SHORT loc_1_04BD
        push   edi
        push   esi
        mov    ecx, 30h
        cld
        rep movsb
        pop    esi
        push   esi
        movzx  ecx, word ptr [esi + 1ch]
        mov    esi, dword ptr [esi + 18h]
        push   ecx
        shr    ecx, 2
        rep movsd
        pop    ecx
        and    ecx, 3
        rep movsb
        pop    esi
        pop    edi
        lea    ecx, [edi + 30h]
        mov    dword ptr [edi + 18h], ecx
loc_1_048C:
        cli
        push   esi
        db  08Dh, 035h, 0B0h, 06Fh, 000h, 000h
        call   sub_1_287D
        pop    esi
        sti
        db  081h, 00Dh, 0ECh, 06Eh, 000h, 000h, 000h, 002h, 000h, 000h
        pop    edi
        popfd
        ret
loc_1_04A8:
        mov    eax, 2
        test   dword ptr [ebx], 40h
        je     SHORT loc_1_04CC
        and    word ptr [esi + 10h], 0fbffh
        stc
        ret
loc_1_04BD:
        or     dword ptr [ebx], 80h
        pop    edi
        pop    ecx
        mov    eax, 6
        clc
        ret
loc_1_04CC:
        stc
        pushfd
        push   edi
        mov    edi, esi
        jmp    SHORT loc_1_048C
loc_1_04D3:
        call   sub_1_0449
        mov    eax, 0
        jb     SHORT loc_1_0527
        inc    byte ptr [ebx + 30h]
        jmp    NEAR PTR loc_1_062D
loc_1_04E7:
        mov    eax, 8
        jmp    SHORT loc_1_04F3
loc_1_04EE:
        mov    eax, 6
loc_1_04F3:
        push   edi
        call   sub_1_6C9A
        jmp    NEAR PTR loc_1_062D
loc_1_04FE:
        call   sub_1_001C
        mov    eax, 6
        jmp    NEAR PTR loc_1_062D
loc_1_050D:
        mov    eax, 6
        jmp    NEAR PTR loc_1_062D
sub_1_0517:
        enter  4, 0
        push   edi
        mov    ebx, dword ptr [esi + 14h]
        test   word ptr [esi + 10h], 400h
        jne    SHORT loc_1_04D3
loc_1_0527:
        push   ebx
        sub    esp, 8
        mov    eax, esp
        push   dword ptr [ebx + 128h]
        push   eax
        add    eax, 4
        push   eax
        call   sub_1_6C8E
        pop    eax
        pop    edx
        cmp    edx, 0
        mov    edi, eax
        pop    eax
        jne    SHORT loc_1_050D
        xor    eax, eax
        test   word ptr [esi + 10h], 2
        je     SHORT loc_1_0553
        mov    eax, esi
loc_1_0553:
        mov    dword ptr [edi + 38h], eax
        movzx  ecx, word ptr [esi + 1ch]
        mov    dword ptr [edi + 3ch], ecx
        test   ecx, ecx
        je     SHORT loc_1_05BE
        cmp    ecx, 40h
        ja     SHORT loc_1_04E7
        mov    edx, dword ptr [ebx + 124h]
        test   edx, edx
        je     NEAR PTR loc_1_04EE
        mov    eax, dword ptr [edx]
        mov    dword ptr [ebx + 124h], eax
        push   edx
        push   ecx
        sub    esp, 8
        push   ecx
        push   edx
        push   dword ptr [ebx + 12ch]
        mov    eax, esp
        add    eax, 0ch
        push   eax
        add    eax, 4
        push   eax
        call   sub_1_6CB2
        pop    eax
        pop    edx
        cmp    edx, 0
        pop    ecx
        pop    edx
        jne    NEAR PTR loc_1_04FE
        xchg   edi, edx
        add    ecx, 3
        shr    ecx, 2
        push   esi
        mov    esi, dword ptr [esi + 18h]
        cld
        rep movsd
        pop    esi
        mov    edi, edx
        push   eax
        push   edi
        call   sub_1_6CD6
loc_1_05BE:
        movzx  ecx, word ptr [esi + 1eh]
        mov    edx, esi
        add    edx, 20h
loc_1_05C7:
        test   ecx, ecx
        je     SHORT loc_1_0608
        push   edx
        push   ecx
        mov    eax, dword ptr [edx + 4]
        movzx  ecx, word ptr [edx + 2]
        sub    esp, 8
        push   ecx
        push   eax
        push   dword ptr [ebx + 12ch]
        mov    eax, esp
        add    eax, 0ch
        push   eax
        add    eax, 4
        push   eax
        call   sub_1_6CB2
        pop    eax
        pop    edx
        cmp    edx, 0
        jne    NEAR PTR loc_1_04FE
        push   eax
        push   edi
        call   sub_1_6CD6
        pop    ecx
        pop    edx
        add    edx, 8
        dec    ecx
        jmp    SHORT loc_1_05C7
loc_1_0608:
        mov    ebx, dword ptr [esi + 14h]
        push   edi
        push   dword ptr [ebx + 10h]
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_6C1C
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, 103h
        jne    SHORT loc_1_0632
        mov    eax, 2
        inc    dword ptr [ebx + 0e8h]
loc_1_062D:
        mov    edx, esi
        pop    edi
        leave
        ret
loc_1_0632:
        mov    ebx, dword ptr [esi + 14h]
        inc    byte ptr [ebx + 30h]
        call   sub_1_001C
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_0676
        push   edi
        test   word ptr [esi + 10h], 402h
        jne    SHORT loc_1_0668
        mov    edi, dword ptr [esi + 0ch]
        or     edi, edi
        je     SHORT loc_1_0668
        mov    edx, 0
        xchg   dword ptr [edi + 0c4h], edx
        or     edx, edx
        je     SHORT loc_1_0668
        mov    edx, dword ptr [edi + 70h]
        mov    dword ptr [edi + 5ch], edx
loc_1_0668:
        pop    edi
        inc    dword ptr [ebx + 0e8h]
        mov    eax, 0
        jmp    SHORT loc_1_062D
loc_1_0676:
        or     dword ptr [ebx], 10h
        test   word ptr [esi + 10h], 400h
        je     SHORT loc_1_0693
        push   esi
        push   edi
        lea    esi, [esi]
        cli
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        sti
        pop    edi
        pop    esi
loc_1_0693:
        mov    eax, 6
        jmp    SHORT loc_1_062D
        db  000h, 000h, 001h
loc_1_069D:
        or     dword ptr [7f6f1303h], eax
        inc    ebx
        push   ebx
        arpl   word ptr [ebx + 0fh], si
        pop    ds
        xchg   dword ptr [edi - 0c1c4051h], edx
        nop
sub_1_06B0:
        push   ss
        xor    byte ptr [eax], al
        db  000h, 093h, 030h, 000h, 000h, 0E4h
        xor    byte ptr [eax], al
        db  000h, 014h, 031h
        add    byte ptr [eax], al
        adc    al, 31h
        add    byte ptr [eax], al
        pop    edx
        xor    dword ptr [eax], eax
        db  000h, 05Ah, 031h
        add    byte ptr [eax], al
        pushad
        xor    al, byte ptr [eax]
        db  000h, 060h, 032h
        add    byte ptr [eax], al
        mov    bl, 32h
        add    byte ptr [eax], al
        mov    bl, 32h
        add    byte ptr [eax], al
        and    esi, dword ptr [ebx]
        add    byte ptr [eax], al
        and    esi, dword ptr [ebx]
        add    byte ptr [eax], al
        inc    ecx
        xor    eax, dword ptr [eax]
        db  000h, 041h, 033h
        add    byte ptr [eax], al
        db  08Ch, 033h, 000h, 000h, 08Ch, 033h, 000h, 000h, 028h, 034h, 000h, 000h, 028h, 034h, 000h, 000h
        db  0B8h, 001h, 000h, 001h, 000h, 0C3h
loc_1_0702:
        mov    edx, esi
        mov    al, byte ptr [ebx + 1]
        and    al, 0c0h
        cmp    al, 40h
        jne    SHORT loc_1_0726
        inc    dword ptr [esi + 0f0h]
        lea    esi, [ebx + 0eh]
        test   byte ptr [ebx + 8], 80h
        je     SHORT loc_1_0737
        mov    al, byte ptr [ebx + 0eh]
        and    eax, 1fh
        add    esi, eax
        jmp    SHORT loc_1_0737
loc_1_0726:
        mov    eax, 10001h
        ret
loc_1_072C:
        inc    dword ptr [esi + 0f0h]
        mov    edx, esi
        lea    esi, [ebx + 0eh]
loc_1_0737:
        mov    ax, word ptr [esi]
        and    ax, 1feh
        cmp    al, 0f0h
        jne    SHORT loc_1_077F
        mov    al, byte ptr [esi + 2]
        movzx  ecx, ah
        test   al, 1
        jne    SHORT loc_1_0753
        mov    eax, edx
        jmp    NEAR PTR loc_1_2E80
loc_1_0753:
        db  0BFh, 09Ch, 006h, 000h, 000h
        push   ecx
        mov    ecx, 13h
        cld
        repne scasb
        pop    ecx
        mov    eax, edx
        jne    SHORT loc_1_0773
        db  081h, 0EFh, 09Dh, 006h, 000h, 000h
        db  0FFh, 014h, 0BDh, 0B0h, 006h, 000h, 000h
loc_1_0773:
        mov    eax, 0
        ret
loc_1_0779:
        mov    eax, 10001h
        ret
loc_1_077F:
        test   al, al
        je     SHORT loc_1_0787
        cmp    al, 0feh
        jne    SHORT loc_1_0779
loc_1_0787:
        mov    al, byte ptr [esi + 2]
        mov    cl, al
        and    al, 3
        cmp    al, 3
        jne    SHORT loc_1_0779
        mov    al, cl
        movzx  ecx, byte ptr [esi + 1]
        and    ecx, 1
        and    al, 0efh
        cmp    al, 0afh
        jne    SHORT loc_1_07AA
        mov    eax, edx
        call   sub_1_338C
        jmp    SHORT loc_1_0773
loc_1_07AA:
        cmp    al, 0e3h
        jne    SHORT loc_1_0779
        mov    eax, edx
        call   sub_1_3428
        jmp    SHORT loc_1_0773
loc_1_07B7:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_1_07C9
        and    dword ptr [ecx], 0fffffdffh
loc_1_07C9:
        mov    eax, dword ptr [ecx + 130h]
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        leave
        ret    0ch
loc_1_07D9:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        or     dword ptr [ebx], 200h
        pop    ebx
        leave
        ret    8
loc_1_07EB:
        ret    8
loc_1_07EE:
        push   ebp
        mov    ebp, esp
        pushfd
        push   esi
        push   edi
        push   ebx
        db  0F7h, 005h, 0ECh, 06Eh, 000h, 000h, 040h, 000h, 000h, 000h
        je     SHORT loc_1_0806
        call   sub_1_646D
loc_1_0806:
        mov    ebx, dword ptr [ebp + 8]
        cli
        inc    byte ptr [ebx + 30h]
        test   dword ptr [ebx], 10h
        je     SHORT loc_1_0831
        db  0A0h, 031h, 06Fh, 000h, 000h
        cmp    al, byte ptr [ebx + 30h]
        ja     SHORT loc_1_0831
        and    dword ptr [ebx], 0ffffffefh
        db  083h, 00Dh, 0ECh, 06Eh, 000h, 000h, 008h
        sti
        call   sub_1_67F7
        jmp    SHORT loc_1_0832
loc_1_0831:
        sti
loc_1_0832:
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [edi + 38h]
        test   esi, esi
        je     SHORT loc_1_0893
        test   word ptr [esi + 10h], 2
        je     SHORT loc_1_0893
        test   word ptr [esi + 10h], 8
        je     SHORT loc_1_0863
        mov    eax, dword ptr [ebp + 10h]
        xor    eax, 0
        je     SHORT loc_1_0856
        mov    al, 21h
loc_1_0856:
        push   esi
        mov    esi, dword ptr [esi + 30h]
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
        pop    esi
loc_1_0863:
        test   word ptr [esi + 10h], 1000h
        je     SHORT loc_1_0875
        db  081h, 025h, 0ECh, 06Eh, 000h, 000h, 0FFh, 0EFh, 0FFh, 0FFh
loc_1_0875:
        cli
        or     word ptr [esi + 10h], 40h
        test   word ptr [esi + 10h], 400h
        sti
        jne    SHORT loc_1_0893
        cli
        push   edi
        db  08Dh, 03Dh, 070h, 06Fh, 000h, 000h
        call   sub_1_2868
        pop    edi
        sti
loc_1_0893:
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        call   sub_1_001C
        pop    ebx
        pop    edi
        pop    esi
        popfd
        leave
        ret    0ch
loc_1_08A6:
        push   ebp
        mov    ebp, esp
        pushfd
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [edi + 2ch], eax
        cmp    eax, 0
        jne    SHORT loc_1_08C2
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ecx + 3ch], eax
loc_1_08C2:
        mov    eax, dword ptr [edi + 130h]
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        pop    edi
        popfd
        leave
        ret    10h
loc_1_08D4:
        pushfd
        push   edi
        push   esi
        push   ebx
        db  0F7h, 005h, 0ECh, 06Eh, 000h, 000h, 040h, 000h, 000h, 000h
        je     SHORT loc_1_08E9
        call   sub_1_646D
loc_1_08E9:
        db  0F7h, 005h, 0ECh, 06Eh, 000h, 000h, 0B8h, 006h, 000h, 000h
        je     SHORT loc_1_08FA
        call   sub_1_67F7
loc_1_08FA:
        pop    ebx
        pop    esi
        pop    edi
        popfd
        ret    4
loc_1_0901:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        pushfd
        db  0FEh, 005h, 0F6h, 06Eh, 000h, 000h
        je     SHORT loc_1_0912
        jmp    SHORT loc_1_0975
loc_1_0912:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, esi
        db  0BFh, 040h, 06Fh, 000h, 000h
        mov    ecx, 8
        repne scasd
        jne    SHORT loc_1_0975
        mov    ebx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 14h]
        db  0A3h, 080h, 06Eh, 000h, 000h
        mov    eax, dword ptr [ebp + 20h]
        add    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [esi + 28h], 1
        je     SHORT loc_1_0950
        movzx  eax, word ptr [ebx + 0ch]
        xchg   al, ah
        cmp    ax, 600h
        jae    SHORT loc_1_0983
        cmp    eax, dword ptr [ebp + 20h]
        ja     SHORT loc_1_0983
        add    eax, 0eh
loc_1_0950:
        mov    word ptr [esi + 76h], ax
        mov    eax, dword ptr [ebp + 1ch]
        add    eax, dword ptr [ebp + 14h]
        mov    word ptr [esi + 74h], ax
        mov    dword ptr [esi + 70h], ebx
        or     dword ptr [esi], 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 80h], eax
        push   esi
        call   dword ptr [esi + 44h]
        pop    esi
        and    dword ptr [esi], 0fffffffbh
loc_1_0975:
        db  0FEh, 00Dh, 0F6h, 06Eh, 000h, 000h
        popfd
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret    1ch
loc_1_0983:
        mov    eax, 10001h
        jmp    SHORT loc_1_0975
loc_1_098A:
        push   ebp
        mov    ebp, esp
        pushfd
        push   ebx
        mov    eax, dword ptr [ebp + 10h]
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 2ch], eax
        pop    ebx
        popfd
        leave
        ret    0ch
loc_1_099E:
        push   ebp
        mov    ebp, esp
        pushfd
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    eax, esi
        db  0BFh, 040h, 06Fh, 000h, 000h
        mov    ecx, 8
        repne scasd
        jne    NEAR PTR loc_1_0ABA
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, 40010007h
        jne    SHORT loc_1_09D7
        push   0
        push   esi
        lea    eax, [ebp + 0ch]
        push   eax
        db  0E8h, 049h, 0F7h, 0FEh, 0FFh
        jmp    NEAR PTR loc_1_0ABA
loc_1_09D7:
        cmp    eax, 40010004h
        je     SHORT loc_1_09F1
        cmp    eax, 40010005h
        je     SHORT loc_1_0A00
        cmp    eax, 40010003h
        jne    SHORT loc_1_0A0F
        jmp    NEAR PTR loc_1_0ABA
loc_1_09F1:
        db  081h, 00Dh, 0ECh, 06Eh, 000h, 000h, 000h, 001h, 000h, 000h
        jmp    NEAR PTR loc_1_0ABA
loc_1_0A00:
        db  081h, 025h, 0ECh, 06Eh, 000h, 000h, 0FFh, 0FEh, 0FFh, 0FFh
        jmp    NEAR PTR loc_1_0ABA
loc_1_0A0F:
        cmp    eax, 40010008h
        jne    SHORT loc_1_0A53
        or     dword ptr [esi + 4], 1
        mov    edi, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [edi]
        test   ecx, ecx
        jne    SHORT loc_1_0A28
        mov    ecx, 60h
loc_1_0A28:
        imul   ecx, ecx, 64h
        mov    dword ptr [esi + 40h], ecx
        mov    ecx, dword ptr [edi + 4]
        mov    word ptr [esi + 92h], cx
        movzx  ecx, word ptr [edi + 0ch]
        or     ecx, ecx
        jne    SHORT loc_1_0A45
        mov    ecx, 2
loc_1_0A45:
        mov    dword ptr [esi + 8], ecx
        mov    byte ptr [esi + 30h], cl
        mov    byte ptr [esi + 134h], cl
        jmp    SHORT loc_1_0ABA
loc_1_0A53:
        cmp    eax, 40010009h
        jne    SHORT loc_1_0A92
        and    dword ptr [esi + 4], 0fffffffeh
        mov    dword ptr [esi + 8], 1
        mov    edi, dword ptr [ebp + 10h]
        lea    edi, [edi]
        sub    edi, 6
        mov    eax, esi
        call   sub_1_236B
        jb     SHORT loc_1_0ABA
        cli
        mov    byte ptr [ebx + 38h], 2
        mov    dword ptr [ebx + 3ch], 2
        sti
        mov    byte ptr [ebx + 0b9h], 1
        push   ebx
        call   sub_1_6435
        jmp    SHORT loc_1_0ABA
loc_1_0A92:
        cmp    eax, 4001000ah
        jne    SHORT loc_1_0ABA
        mov    edi, dword ptr [ebp + 10h]
        lea    edi, [edi]
        sub    edi, 6
        mov    eax, esi
        call   sub_1_236B
        jb     SHORT loc_1_0ABA
        cli
        or     dword ptr [ebx + 34h], 14h
        or     dword ptr [ebx + 3ch], 20h
        sti
        push   ebx
        call   sub_1_4E6C
loc_1_0ABA:
        pop    ebx
        pop    edi
        pop    esi
        popfd
        leave
        ret    10h
loc_1_0AC2:
        ret    4
        db  000h, 000h, 000h, 000h, 002h, 001h, 002h, 002h, 003h, 000h, 000h, 001h, 000h, 005h
loc_1_0AD3:
        cmp    byte ptr [ebx + 25h], 1
        jne    SHORT loc_1_0B14
        dec    byte ptr [ebx + 90h]
        js     SHORT loc_1_0AFB
        mov    esi, dword ptr [ebx + 94h]
        mov    eax, dword ptr [esi + 4]
loc_1_0AEA:
        call   sub_1_39D9
        mov    ecx, eax
        db  0B8h, 0D3h, 00Ah, 000h, 000h
        jmp    NEAR PTR sub_1_34D0
loc_1_0AFB:
        test   word ptr [ebx + 42h], 40h
        jne    SHORT loc_1_0B0D
        mov    al, 12h
        test   word ptr [ebx + 40h], 8
        jne    SHORT loc_1_0B0F
loc_1_0B0D:
        mov    al, 14h
loc_1_0B0F:
        call   sub_1_0BAA
loc_1_0B14:
        ret
loc_1_0B15:
        cmp    byte ptr [ebx + 25h], 1
        jne    SHORT loc_1_0B3D
        mov    esi, dword ptr [ebx + 94h]
        mov    eax, dword ptr [esi + 4]
        dec    byte ptr [ebx + 90h]
        js     SHORT loc_1_0B3E
        call   sub_1_3927
        mov    ecx, eax
        db  0B8h, 015h, 00Bh, 000h, 000h
        call   sub_1_34D0
loc_1_0B3D:
        ret
loc_1_0B3E:
        test   dword ptr [eax + 4], 1
        jne    SHORT loc_1_0B4B
        mov    cl, 1
        jmp    SHORT loc_1_0B62
loc_1_0B4B:
        test   dword ptr [eax + 4], 2
        je     SHORT loc_1_0B5C
        db  08Ah, 00Dh, 021h, 06Fh, 000h, 000h
        jmp    SHORT loc_1_0B62
loc_1_0B5C:
        db  08Ah, 00Dh, 01Fh, 06Fh, 000h, 000h
loc_1_0B62:
        mov    byte ptr [ebx + 90h], cl
        jmp    SHORT loc_1_0AEA
loc_1_0B6A:
        mov    al, 5
        jmp    SHORT sub_1_0BAA
loc_1_0B6E:
        cli
        test   word ptr [ebx + 40h], 80h
        je     SHORT loc_1_0BA1
        test   word ptr [ebx + 42h], 4000h
        je     SHORT loc_1_0BA1
        or     word ptr [ebx + 40h], 40h
        mov    esi, dword ptr [ebx + 88h]
        and    word ptr [ebx + 40h], 0ff7fh
        sti
        mov    byte ptr [esi + 1], 5
        call   sub_1_6BE9
        call   sub_1_3CD2
        jb     SHORT loc_1_0BA3
loc_1_0BA1:
        sti
        ret
loc_1_0BA3:
        push   ebx
        call   sub_1_5BF3
        ret
sub_1_0BAA:
        cli
        cmp    byte ptr [ebx + 25h], 1
        je     SHORT loc_1_0BBF
        cmp    byte ptr [ebx + 25h], 3
        je     SHORT loc_1_0BBF
        cmp    byte ptr [ebx + 25h], 4
        je     SHORT loc_1_0BBF
        sti
        ret
loc_1_0BBF:
        mov    byte ptr [ebx + 25h], 0
        test   word ptr [ebx + 40h], 80h
        pushfd
        jne    SHORT loc_1_0BCE
        jmp    SHORT loc_1_0BE5
loc_1_0BCE:
        and    word ptr [ebx + 40h], 0ff7fh
        sti
        mov    esi, dword ptr [ebx + 88h]
        test   word ptr [ebx + 42h], 8
        je     SHORT loc_1_0BE5
        mov    al, 0bh
loc_1_0BE5:
        or     word ptr [ebx + 42h], 4000h
        or     word ptr [ebx + 6ah], 2
        push   esi
        mov    esi, dword ptr [ebx + 94h]
        dec    byte ptr [esi + 3]
        cmp    byte ptr [esi + 1], 3
        jne    SHORT loc_1_0C05
        call   sub_1_250A
loc_1_0C05:
        pop    esi
        test   word ptr [ebx + 42h], 2
        jne    SHORT loc_1_0C14
        push   ebx
        call   sub_1_5BF3
loc_1_0C14:
        popfd
        je     SHORT loc_1_0C1F
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
loc_1_0C1F:
        ret
sub_1_0C20:
        cli
        cmp    byte ptr [ebx + 25h], 2
        je     SHORT loc_1_0C2F
        cmp    byte ptr [ebx + 25h], 8
        je     SHORT loc_1_0BBF
        sti
        ret
loc_1_0C2F:
        test   byte ptr [ebx + 24h], 2
        je     SHORT loc_1_0C49
        and    byte ptr [ebx + 24h], 0fdh
        push   esi
        push   edi
        lea    esi, [ebx]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
loc_1_0C49:
        jmp    NEAR PTR loc_1_0BBF
sub_1_0C4E:
        mov    al, byte ptr [esi + 18h]
        add    byte ptr [edx + 2], al
        mov    al, byte ptr [esi + 19h]
        add    byte ptr [edx + 3], al
        sub    edi, edi
        push   edi
        cld
loc_1_0C5E:
        mov    edi, dword ptr [esp]
        db  08Bh, 03Ch, 0BDh, 0C0h, 077h, 000h, 000h
        test   edi, edi
        je     SHORT loc_1_0C82
        cmp    dword ptr [edi + 94h], esi
        jne    SHORT loc_1_0C82
        movzx  eax, byte ptr [edi + 25h]
        db  08Ah, 080h, 0C8h, 00Ah, 000h, 000h
        or     al, al
        jne    SHORT loc_1_0C99
loc_1_0C82:
        inc    dword ptr [esp]
        cmp    dword ptr [esp], 100h
        jb     SHORT loc_1_0C5E
        pop    edi
        sub    eax, eax
        ret
loc_1_0C92:
        pop    edi
        mov    eax, 6
        ret
loc_1_0C99:
        cmp    ecx, 24h
        jb     SHORT loc_1_0C92
        inc    byte ptr [edx + 1]
        mov    byte ptr [ebx + 1], al
        mov    al, byte ptr [edi + 68h]
        mov    byte ptr [ebx], al
        mov    al, byte ptr [edi + 8ch]
        mov    byte ptr [ebx + 22h], al
        mov    al, byte ptr [edi + 8dh]
        mov    byte ptr [ebx + 23h], al
        push   ecx
        push   esi
        lea    esi, [edi + 44h]
        lea    edi, [ebx + 2]
        mov    ecx, 8
        rep movsd
        pop    esi
        pop    ecx
        sub    ecx, 24h
        add    ebx, 24h
        jmp    SHORT loc_1_0C82
loc_1_0CD4:
        mov    edi, ebx
        mov    eax, esi
        lea    esi, [ebx + 1ah]
        call   sub_1_2480
        mov    esi, ebx
        mov    al, 15h
        jb     SHORT loc_1_0CFE
        cmp    byte ptr [ebx + 1], 2
        jne    SHORT loc_1_0CFE
        mov    al, 19h
        test   byte ptr [ebx], 2
        jne    SHORT loc_1_0CFE
        call   sub_1_229D
        mov    al, 11h
        jb     SHORT loc_1_0CFE
        jmp    SHORT loc_1_0D0B
loc_1_0CFE:
        mov    byte ptr [edi + 1], al
        mov    esi, edi
        call   sub_1_6BE9
        mov    al, 0ffh
        ret
loc_1_0D0B:
        mov    byte ptr [ebx + 25h], 1
        mov    al, byte ptr [ebx + 68h]
        mov    byte ptr [edi + 2], al
        cli
        mov    dword ptr [ebx + 88h], edi
        or     word ptr [ebx + 40h], 80h
        db  0B8h, 015h, 00Bh, 000h, 000h
        mov    ecx, dword ptr [esi + 4]
        call   sub_1_34D0
        sti
        mov    dword ptr [ebx + 94h], esi
        inc    byte ptr [esi + 3]
        push   dword ptr [esi + 4]
        mov    edx, edi
        mov    ecx, 4
        lea    esi, [edi + 1ah]
        lea    edi, [ebx + 44h]
        cld
        rep movsd
        mov    edi, edx
        mov    ecx, 4
        lea    esi, [edi + 0ah]
        lea    edi, [ebx + 54h]
        rep movsd
        mov    edi, edx
        mov    al, byte ptr [edi + 2ah]
        mov    byte ptr [ebx + 8eh], al
        mov    al, byte ptr [edi + 2bh]
        mov    byte ptr [ebx + 8fh], al
        mov    eax, dword ptr [esp]
        test   dword ptr [eax + 4], 1
        jne    SHORT loc_1_0D7F
        mov    al, 1
        jmp    SHORT loc_1_0D94
loc_1_0D7F:
        test   dword ptr [eax + 4], 2
        je     SHORT loc_1_0D8F
        db  0A0h, 021h, 06Fh, 000h, 000h
        jmp    SHORT loc_1_0D94
loc_1_0D8F:
        db  0A0h, 01Fh, 06Fh, 000h, 000h
loc_1_0D94:
        mov    byte ptr [ebx + 90h], al
        pop    eax
        call   sub_1_3927
        mov    al, 0ffh
        ret
loc_1_0DA3:
        mov    edi, ebx
        mov    eax, esi
        lea    esi, [edi + 1ah]
        call   sub_1_2480
        mov    al, 15h
        jb     SHORT loc_1_0DC0
        cmp    byte ptr [ebx + 1], 2
        jne    SHORT loc_1_0DC0
        mov    al, 19h
        test   byte ptr [ebx], 2
        je     SHORT loc_1_0DCD
loc_1_0DC0:
        mov    byte ptr [edi + 1], al
        mov    esi, edi
        call   sub_1_6BE9
        mov    al, 0ffh
        ret
loc_1_0DCD:
        mov    esi, ebx
        call   sub_1_229D
        mov    al, 11h
        jb     SHORT loc_1_0DC0
        mov    dword ptr [ebx + 94h], esi
        inc    byte ptr [esi + 3]
        mov    byte ptr [ebx + 25h], 2
        mov    al, byte ptr [ebx + 68h]
        mov    byte ptr [edi + 2], al
        mov    dword ptr [ebx + 88h], edi
        or     word ptr [ebx + 40h], 80h
        mov    eax, esi
        mov    edx, edi
        mov    ecx, 4
        lea    esi, [edi + 1ah]
        lea    edi, [ebx + 44h]
        cld
        rep movsd
        mov    edi, edx
        mov    ecx, 4
        lea    esi, [edi + 0ah]
        lea    edi, [ebx + 54h]
        rep movsd
        mov    edi, edx
        mov    esi, eax
        mov    al, byte ptr [edi + 2ah]
        mov    byte ptr [ebx + 8eh], al
        mov    al, byte ptr [edi + 2bh]
        mov    byte ptr [ebx + 8fh], al
        cli
        or     byte ptr [ebx + 24h], 2
        cmp    byte ptr [ebx + 54h], 2ah
        je     SHORT loc_1_0E45
        push   esi
        lea    esi, [esi + 1ch]
        call   sub_1_2892
        pop    esi
        jmp    SHORT loc_1_0E4F
loc_1_0E45:
        push   esi
        lea    esi, [esi + 24h]
        call   sub_1_2892
        pop    esi
loc_1_0E4F:
        sti
        mov    al, 0ffh
        ret
loc_1_0E53:
        call   sub_1_5D8D
        jb     SHORT loc_1_0E91
        mov    dword ptr [ebx + 88h], edi
        or     word ptr [ebx + 40h], 0a0h
        and    word ptr [ebx + 40h], 0ffbfh
        or     word ptr [ebx + 42h], 4000h
        db  0A1h, 017h, 06Fh, 000h, 000h
        mov    dword ptr [edi + 32h], eax
        db  0C7h, 047h, 03Ah, 06Eh, 00Bh, 000h, 000h
        lea    eax, [ebx + 30h]
        cmp    eax, dword ptr [ebx + 30h]
        sti
        jne    SHORT loc_1_0E8E
        call   sub_1_3CD2
loc_1_0E8E:
        mov    al, 0ffh
        ret
loc_1_0E91:
        mov    esi, edi
        call   sub_1_6BE9
        mov    al, 0ffh
        ret
loc_1_0E9B:
        mov    edi, ebx
        movzx  ecx, word ptr [edi + 8]
        cmp    ecx, 4
        jae    SHORT loc_1_0EB5
        mov    al, 1
loc_1_0EA8:
        mov    byte ptr [edi + 1], al
        mov    esi, edi
        call   sub_1_6BE9
        mov    al, 0ffh
        ret
loc_1_0EB5:
        mov    eax, esi
        mov    edx, eax
        sub    ebx, ebx
        cmp    byte ptr [edi + 1ah], 2ah
        je     SHORT loc_1_0EDC
        lea    esi, [edi + 1ah]
        push   ecx
        call   sub_1_2480
        pop    ecx
        mov    al, 15h
        jb     SHORT loc_1_0EA8
        cmp    byte ptr [ebx + 1], 2
        jne    SHORT loc_1_0EA8
        mov    al, 19h
        test   byte ptr [ebx], 2
        jne    SHORT loc_1_0EA8
loc_1_0EDC:
        mov    esi, ebx
        mov    ebx, dword ptr [edi + 4]
        xor    al, al
        mov    byte ptr [ebx + 1], al
        mov    byte ptr [ebx + 2], al
        mov    byte ptr [ebx + 3], al
        mov    eax, edx
        mov    edx, ebx
        push   edi
        test   esi, esi
        jne    SHORT loc_1_0F25
        mov    byte ptr [ebx], 0ffh
        add    ebx, 4
        mov    esi, eax
        mov    edi, esi
        mov    esi, dword ptr [edi + 84h]
loc_1_0F05:
        push   esi
        push   edi
        cmp    byte ptr [esi + 2], 0
        je     SHORT loc_1_0F12
        call   sub_1_0C4E
loc_1_0F12:
        pop    edi
        pop    esi
        cmp    al, 6
        je     SHORT loc_1_0F32
        add    esi, 44h
        cmp    dword ptr [edi + 88h], esi
        ja     SHORT loc_1_0F05
        jmp    SHORT loc_1_0F32
loc_1_0F25:
        mov    al, byte ptr [esi + 2]
        mov    byte ptr [ebx], al
        add    ebx, 4
        call   sub_1_0C4E
loc_1_0F32:
        pop    edi
        movzx  edx, word ptr [edi + 8]
        sub    edx, ecx
        mov    word ptr [edi + 8], dx
        ret
        db  000h, 000h
loc_1_0F40:
        cli
        cmp    byte ptr [ebx + 25h], 6
        jne    SHORT loc_1_0F94
        mov    esi, dword ptr [ebx + 94h]
        dec    byte ptr [ebx + 90h]
        js     SHORT loc_1_0F6B
        db  0B8h, 040h, 00Fh, 000h, 000h
        mov    ecx, dword ptr [esi + 4]
        call   sub_1_34D0
        mov    eax, dword ptr [esi + 4]
        sti
        jmp    NEAR PTR sub_1_39EC
loc_1_0F6B:
        and    word ptr [ebx + 40h], 0ff7fh
        sti
        mov    byte ptr [esi + 3], 0
        mov    byte ptr [esi + 1], 2
        mov    cl, byte ptr [esi + 2]
        mov    esi, dword ptr [ebx + 88h]
        call   sub_1_235E
        mov    byte ptr [esi + 3], cl
        mov    byte ptr [esi + 1], 0
        call   sub_1_6BE9
loc_1_0F94:
        sti
        ret
sub_1_0F96:
        cli
        mov    esi, dword ptr [ebx + 84h]
        add    esi, 44h
loc_1_0FA0:
        cmp    dword ptr [ebx + 88h], esi
        jb     SHORT loc_1_0FB3
        cmp    byte ptr [esi + 2], 0
        je     SHORT loc_1_0FB6
        add    esi, 44h
        jmp    SHORT loc_1_0FA0
loc_1_0FB3:
        sti
        stc
        ret
loc_1_0FB6:
        mov    byte ptr [esi + 2], 0ffh
        sti
        mov    edx, esi
        mov    ecx, ebx
        mov    esi, ebx
        mov    bl, byte ptr [esi + 90h]
        inc    bl
loc_1_0FC9:
        cmp    bl, 0feh
        jb     SHORT loc_1_0FD0
        mov    bl, 2
loc_1_0FD0:
        push   ebx
        call   sub_1_24CC
        pop    ebx
        jb     SHORT loc_1_0FDD
        inc    bl
        jmp    SHORT loc_1_0FC9
loc_1_0FDD:
        mov    esi, edx
        xchg   ebx, ecx
        mov    byte ptr [esi + 2], cl
        mov    byte ptr [ebx + 90h], cl
        mov    byte ptr [esi + 1], 0
        mov    dword ptr [esi + 4], ebx
        sub    eax, eax
        mov    byte ptr [esi], al
        mov    byte ptr [esi + 3], al
        mov    byte ptr [esi + 18h], al
        mov    byte ptr [esi + 19h], al
        lea    eax, [esi + 1ch]
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 20h], eax
        lea    eax, [esi + 24h]
        mov    dword ptr [esi + 24h], eax
        mov    dword ptr [esi + 28h], eax
        lea    eax, [esi + 2ch]
        mov    dword ptr [esi + 2ch], eax
        mov    dword ptr [esi + 30h], eax
        lea    eax, [esi + 34h]
        mov    dword ptr [esi + 34h], eax
        mov    dword ptr [esi + 38h], eax
        lea    eax, [esi + 3ch]
        mov    dword ptr [esi + 3ch], eax
        mov    dword ptr [esi + 40h], eax
        clc
        ret
loc_1_102D:
        mov    edi, ebx
        mov    ebx, esi
        call   sub_1_0F96
        jb     SHORT loc_1_1067
        mov    al, byte ptr [esi + 2]
        mov    byte ptr [edi + 3], al
        mov    al, byte ptr [edi]
        and    al, 7fh
        cmp    al, 36h
        jne    SHORT loc_1_1049
        or     byte ptr [esi], 1
loc_1_1049:
        mov    byte ptr [esi + 3], 0
        mov    byte ptr [esi + 1], 2
        inc    byte ptr [ebx + 91h]
        xchg   edi, esi
        add    esi, 1ah
        add    edi, 8
        cld
        movsd
        movsd
        movsd
        movsd
        sub    eax, eax
        ret
loc_1_1067:
        mov    al, 0eh
        ret
loc_1_106A:
        mov    al, byte ptr [ebx + 1ah]
        or     al, al
        je     SHORT loc_1_108E
        cmp    al, 2ah
        je     SHORT loc_1_108E
        mov    edi, ebx
        push   esi
        mov    eax, esi
        lea    esi, [edi + 1ah]
        call   sub_1_2480
        jb     SHORT loc_1_1091
        mov    al, byte ptr [ebx + 2]
        mov    byte ptr [edi + 3], al
        pop    esi
        mov    al, 0dh
        ret
loc_1_108E:
        mov    al, 15h
        ret
loc_1_1091:
        pop    ebx
        call   sub_1_0F96
        jb     SHORT loc_1_10E6
        test   dword ptr [ebx], 2
        jne    SHORT loc_1_10D0
        mov    al, byte ptr [esi + 2]
        mov    byte ptr [edi + 3], al
        mov    al, byte ptr [edi]
        and    al, 7fh
        cmp    al, 36h
        jne    SHORT loc_1_10B2
        or     byte ptr [esi], 1
loc_1_10B2:
        mov    byte ptr [esi + 3], 0
        mov    byte ptr [esi + 1], 2
        inc    byte ptr [ebx + 91h]
        xchg   edi, esi
        add    esi, 1ah
        add    edi, 8
        cld
        movsd
        movsd
        movsd
        movsd
        sub    eax, eax
        ret
loc_1_10D0:
        call   sub_1_229D
        jae    SHORT loc_1_10E9
        mov    word ptr [esi + 2], 0
        mov    word ptr [esi + 18h], 0
        mov    al, 21h
        ret
loc_1_10E6:
        mov    al, 0eh
        ret
loc_1_10E9:
        mov    dword ptr [ebx + 94h], esi
        mov    byte ptr [esi + 1], 1
        mov    byte ptr [esi + 3], 1
        mov    byte ptr [ebx + 25h], 6
        mov    al, byte ptr [edi]
        and    al, 7fh
        cmp    al, 36h
        jne    SHORT loc_1_110B
        or     word ptr [ebx + 40h], 10h
        or     byte ptr [esi], 1
loc_1_110B:
        cli
        mov    dword ptr [ebx + 88h], edi
        or     word ptr [ebx + 40h], 80h
        db  0B8h, 040h, 00Fh, 000h, 000h
        mov    ecx, dword ptr [esi + 4]
        call   sub_1_34D0
        sti
        mov    eax, esi
        mov    edx, edi
        lea    esi, [edi + 1ah]
        lea    edi, [ebx + 44h]
        cld
        movsd
        movsd
        movsd
        movsd
        mov    esi, eax
        mov    edi, edx
        mov    ecx, esi
        lea    esi, [edi + 1ah]
        lea    edi, [ecx + 8]
        movsd
        movsd
        movsd
        movsd
        mov    esi, eax
        mov    edi, edx
        mov    eax, dword ptr [esi + 4]
        inc    byte ptr [eax + 91h]
        test   dword ptr [eax + 4], 1
        jne    SHORT loc_1_115F
        mov    al, 1
        jmp    SHORT loc_1_1174
loc_1_115F:
        test   dword ptr [eax + 4], 2
        je     SHORT loc_1_116F
        db  0A0h, 021h, 06Fh, 000h, 000h
        jmp    SHORT loc_1_1174
loc_1_116F:
        db  0A0h, 01Fh, 06Fh, 000h, 000h
loc_1_1174:
        mov    byte ptr [ebx + 90h], al
        mov    eax, dword ptr [esi + 4]
        call   sub_1_39EC
        mov    al, 0ffh
        ret
loc_1_1185:
        mov    eax, esi
        lea    edi, [ebx + 1ah]
        cmp    byte ptr [edi], 0
        je     SHORT loc_1_119C
        cmp    byte ptr [edi], 2ah
        je     SHORT loc_1_119C
        call   sub_1_2527
        jb     SHORT loc_1_119C
        ret
loc_1_119C:
        mov    al, 15h
        ret
        db  000h
loc_1_11A0:
        test   byte ptr [ebx + 3fh], 8
        jne    NEAR PTR loc_1_1372
        jmp    NEAR PTR loc_1_12ED
loc_1_11AF:
        cli
        mov    al, 24h
        movzx  edi, byte ptr [ebx + 2]
        dec    edi
        db  08Bh, 03Ch, 0BDh, 0C0h, 077h, 000h, 000h
        test   edi, edi
        je     SHORT loc_1_11CF
        cmp    byte ptr [edi + 25h], 5
        je     SHORT loc_1_11CF
        or     word ptr [edi + 42h], 8
        sub    al, al
loc_1_11CF:
        sti
        ret
loc_1_11D1:
        cli
        mov    al, 24h
        movzx  edi, byte ptr [ebx + 2]
        dec    edi
        db  08Bh, 03Ch, 0BDh, 0C0h, 077h, 000h, 000h
        test   edi, edi
        je     SHORT loc_1_1201
        cmp    byte ptr [edi + 25h], 5
        je     SHORT loc_1_1201
        or     word ptr [edi + 42h], 8
        cmp    byte ptr [edi + 25h], 2
        jne    SHORT loc_1_11FF
        push   ebx
        mov    ebx, edi
        cli
        call   sub_1_0C20
        pop    ebx
loc_1_11FF:
        sub    al, al
loc_1_1201:
        sti
        ret
loc_1_1203:
        cli
        mov    edi, ebx
        test   edi, edi
        je     SHORT loc_1_1230
        push   esi
        push   edi
        lea    esi, [edi + 32h]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        sti
        db  0FEh, 005h, 005h, 06Fh, 000h, 000h
        mov    esi, edi
        mov    byte ptr [esi + 1], 0bh
        call   sub_1_6BE9
        sub    al, al
        ret
loc_1_1230:
        sti
        mov    al, 24h
        ret
loc_1_1234:
        sti
        stc
        ret
sub_1_1237:
        cli
        cmp    byte ptr [ebx + 31h], 0ffh
        jne    SHORT loc_1_1234
        test   byte ptr [ebx + 3fh], 4
        jne    SHORT loc_1_12AB
        push   esi
        push   edi
        lea    esi, [ebx + 32h]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        mov    dl, byte ptr [ebx]
        mov    edi, ebx
        call   sub_1_2913
        sti
        and    dl, 7fh
        cmp    dl, 15h
        je     SHORT loc_1_1290
        cmp    byte ptr [ebx + 3], 0ffh
        je     SHORT loc_1_128A
        mov    al, byte ptr [ebx + 30h]
        call   sub_1_2550
        mov    bl, byte ptr [ebx + 3]
        call   sub_1_24CC
        cmp    dl, 48h
        je     SHORT loc_1_1287
        cmp    dl, 16h
        jne    SHORT loc_1_128C
loc_1_1287:
        dec    byte ptr [ebx + 19h]
loc_1_128A:
        clc
        ret
loc_1_128C:
        dec    byte ptr [ebx + 18h]
        ret
loc_1_1290:
        movzx  ebx, byte ptr [ebx + 2]
        dec    ebx
        db  08Bh, 01Ch, 09Dh, 0C0h, 077h, 000h, 000h
        dec    byte ptr [ebx + 8ch]
        ret
loc_1_12A3:
        call   sub_1_5744
        pop    ebx
        clc
        ret
loc_1_12AB:
        push   ebx
        movzx  ebx, byte ptr [ebx + 2]
        dec    ebx
        db  08Bh, 01Ch, 09Dh, 0C0h, 077h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_1_12E9
        cmp    word ptr [ebx + 84h], 0
        je     SHORT loc_1_12A3
        or     word ptr [ebx + 40h], 0c000h
        call   sub_1_5744
        call   sub_1_5CEC
        jb     SHORT loc_1_12DB
loc_1_12D8:
        pop    ebx
        clc
        ret
loc_1_12DB:
        xor    eax, eax
        call   sub_1_3DA6
        call   sub_1_5C40
        jmp    SHORT loc_1_12D8
loc_1_12E9:
        sti
        pop    ebx
        stc
        ret
loc_1_12ED:
        mov    al, 0bh
        call   sub_1_1237
        jb     SHORT loc_1_12FE
        call   sub_1_646D
        sub    al, al
        ret
loc_1_12FE:
        mov    al, 24h
        ret
loc_1_1301:
        jmp    SHORT loc_1_12ED
loc_1_1303:
        jmp    SHORT loc_1_12ED
loc_1_1305:
        jmp    SHORT loc_1_12ED
loc_1_1307:
        cli
        mov    al, 24h
        movzx  edi, byte ptr [ebx + 2]
        dec    edi
        db  08Bh, 03Ch, 0BDh, 0C0h, 077h, 000h, 000h
        test   edi, edi
        je     SHORT loc_1_1327
        cmp    byte ptr [edi + 25h], 9
        jne    SHORT loc_1_1327
        or     word ptr [edi + 42h], 8
        sub    al, al
loc_1_1327:
        sti
        ret
sub_1_1329:
        cli
        cmp    byte ptr [ebx + 31h], 0ffh
        jne    SHORT loc_1_136F
        mov    esi, ebx
        movzx  ebx, byte ptr [esi + 2]
        dec    ebx
        db  08Bh, 01Ch, 09Dh, 0C0h, 077h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_1_136F
        push   esi
        push   edi
        lea    esi, [esi + 32h]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        or     word ptr [ebx + 40h], 60h
        sti
        db  0FEh, 005h, 005h, 06Fh, 000h, 000h
        mov    edi, esi
        cli
        call   sub_1_2913
        sti
        call   sub_1_3CD2
        clc
        ret
loc_1_136F:
        sti
        stc
        ret
loc_1_1372:
        mov    al, 0bh
        call   sub_1_1329
        jb     SHORT loc_1_137E
        sub    al, al
        ret
loc_1_137E:
        mov    al, 24h
        ret
sub_1_1381:
        push   eax
        push   ecx
        push   edi
        mov    eax, dword ptr [esi + 31h]
        mov    dword ptr [ebx], eax
        mov    ax, word ptr [esi + 35h]
        mov    word ptr [ebx + 4], ax
        mov    byte ptr [ebx + 6], 2
        mov    byte ptr [ebx + 8], 0ffh
        mov    byte ptr [ebx + 9], 22h
        mov    eax, dword ptr [esi + 0cch]
        shr    eax, 6
        mov    word ptr [ebx + 0ah], ax
        mov    eax, dword ptr [esi + 0d0h]
        mov    dword ptr [ebx + 0ch], eax
        mov    eax, dword ptr [esi + 0e0h]
        mov    dword ptr [ebx + 10h], eax
        mov    eax, dword ptr [esi + 0e8h]
        mov    dword ptr [ebx + 14h], eax
        mov    eax, dword ptr [esi + 0f0h]
        mov    dword ptr [ebx + 18h], eax
        mov    eax, dword ptr [esi + 0f8h]
        mov    dword ptr [ebx + 1ch], eax
        mov    eax, dword ptr [esi + 108h]
        mov    dword ptr [ebx + 20h], eax
        sub    ah, ah
        db  0A0h, 005h, 06Fh, 000h, 000h
        dec    al
        mov    word ptr [ebx + 28h], ax
        mov    word ptr [ebx + 2ah], 0ffh
        mov    word ptr [ebx + 2ch], 0ffh
        db  0A0h, 0F0h, 06Eh, 000h, 000h
        mov    word ptr [ebx + 34h], ax
        mov    word ptr [ebx + 36h], ax
        mov    al, byte ptr [esi + 91h]
        dec    al
        mov    word ptr [ebx + 3ah], ax
        mov    ax, 1000h
        mov    word ptr [ebx + 38h], ax
        mov    word ptr [ebx + 30h], 400h
        db  066h, 0A1h, 003h, 06Fh, 000h, 000h
        mov    word ptr [ebx + 2eh], ax
        mov    edi, dword ptr [esi + 84h]
        xor    ecx, ecx
        xor    eax, eax
loc_1_1434:
        cmp    byte ptr [edi + 2], 0
        je     SHORT loc_1_143F
        mov    al, byte ptr [edi + 3]
        add    ecx, eax
loc_1_143F:
        add    edi, 44h
        cmp    edi, dword ptr [esi + 88h]
        jbe    SHORT loc_1_1434
        mov    word ptr [ebx + 32h], cx
        pop    edi
        pop    ecx
        pop    eax
        ret
sub_1_1452:
        push   ecx
        mov    ecx, eax
        jecxz  loc_1_146A
loc_1_1457:
        add    ebx, 44h
        cmp    ebx, dword ptr [esi + 88h]
        ja     SHORT loc_1_146A
        cmp    byte ptr [ebx + 2], 0
        je     SHORT loc_1_1457
        loop   loc_1_1457
loc_1_146A:
        pop    ecx
        ret
sub_1_146C:
        push   ebx
        push   esi
        push   edi
        push   ecx
        mov    edx, ecx
        sub    edx, 3ch
        lea    edi, [ebx + 3ch]
        cli
        mov    ebx, dword ptr [esi + 84h]
        add    ebx, 44h
        or     eax, eax
        jne    SHORT loc_1_14D5
loc_1_1486:
        cmp    dword ptr [esi + 88h], ebx
        jb     SHORT loc_1_14EE
        cmp    byte ptr [ebx + 2], 0
        je     SHORT loc_1_14D0
        sub    edx, 12h
        js     SHORT loc_1_14E4
        push   esi
        lea    esi, [ebx + 8]
        cld
        movsd
        movsd
        movsd
        movsd
        pop    esi
        inc    eax
        movzx  ecx, byte ptr [ebx + 2]
        test   byte ptr [ebx], 1
        je     SHORT loc_1_14B0
        or     ch, 80h
loc_1_14B0:
        test   byte ptr [ebx], 2
        je     SHORT loc_1_14B8
        or     ch, 2
loc_1_14B8:
        cmp    byte ptr [ebx + 1], 2
        jne    SHORT loc_1_14C3
        or     ch, 4
        jmp    SHORT loc_1_14CC
loc_1_14C3:
        cmp    byte ptr [ebx + 1], 3
        jne    SHORT loc_1_14CC
        or     ch, 5
loc_1_14CC:
        xchg   ecx, eax
        stosw
        xchg   ecx, eax
loc_1_14D0:
        add    ebx, 44h
        jmp    SHORT loc_1_1486
loc_1_14D5:
        call   sub_1_1452
        xor    eax, eax
        add    edx, 3ch
        sub    edi, 3ch
        jmp    SHORT loc_1_1486
loc_1_14E4:
        sti
        add    edx, 12h
        mov    ebx, eax
        mov    al, 6
        jmp    SHORT loc_1_14F3
loc_1_14EE:
        sti
        mov    ebx, eax
        sub    al, al
loc_1_14F3:
        pop    ecx
        sub    ecx, edx
        mov    edx, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_14FC:
        pushad
        cli
        mov    esi, dword ptr [ebx + 4]
        mov    bl, byte ptr [ebx + 2]
        lea    edx, [esi + 0a4h]
        push   edx
        mov    edi, dword ptr [edx]
loc_1_150D:
        cmp    edi, dword ptr [esp]
        je     SHORT loc_1_153A
        lea    edx, [edi - 32h]
        cmp    byte ptr [edx + 3], bl
        je     SHORT loc_1_151E
loc_1_151A:
        mov    edi, dword ptr [edi]
        jmp    SHORT loc_1_150D
loc_1_151E:
        push   esi
        push   edi
        lea    esi, [edx + 32h]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        jecxz  loc_1_151A
        mov    edi, edx
        call   sub_1_2913
        jmp    SHORT loc_1_151A
loc_1_153A:
        pop    edx
        sti
        call   sub_1_646D
        popad
        ret
sub_1_1543:
        push   edi
        push   esi
loc_1_1545:
        mov    esi, dword ptr [esp]
        cmp    dword ptr [esi + 88h], ebx
        jb     SHORT loc_1_155B
        cmp    byte ptr [ebx + 2], 0
        jne    SHORT loc_1_155E
        jmp    NEAR PTR loc_1_161B
loc_1_155B:
        pop    esi
        pop    edi
        ret
loc_1_155E:
        call   sub_1_14FC
loc_1_1563:
        cli
        lea    edi, [ebx + 1ch]
        call   sub_1_2903
        sti
        je     SHORT loc_1_1598
        and    word ptr [edi + 40h], 0ff7fh
        jecxz  loc_1_1585
        mov    esi, dword ptr [edi + 88h]
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
loc_1_1585:
        db  0FEh, 005h, 005h, 06Fh, 000h, 000h
        mov    word ptr [edi + 40h], 2000h
        call   sub_1_5999
        jmp    SHORT loc_1_1563
loc_1_1598:
        cli
        lea    edi, [ebx + 24h]
        call   sub_1_2903
        sti
        je     SHORT loc_1_15CD
        and    word ptr [edi + 40h], 0ff7fh
        jecxz  loc_1_15BA
        mov    esi, dword ptr [edi + 88h]
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
loc_1_15BA:
        db  0FEh, 005h, 005h, 06Fh, 000h, 000h
        mov    word ptr [edi + 40h], 2000h
        call   sub_1_5999
        jmp    SHORT loc_1_1598
loc_1_15CD:
        cli
        lea    esi, [ebx + 2ch]
        call   sub_1_28E3
        lea    esi, [esi - 32h]
        sti
        je     SHORT loc_1_15EE
        jecxz  loc_1_15E6
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
loc_1_15E6:
        db  0FEh, 005h, 005h, 06Fh, 000h, 000h
        jmp    SHORT loc_1_15CD
loc_1_15EE:
        cli
        lea    esi, [ebx + 34h]
        call   sub_1_28E3
        lea    esi, [esi - 32h]
        sti
        je     SHORT loc_1_160F
        jecxz  loc_1_1607
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
loc_1_1607:
        db  0FEh, 005h, 005h, 06Fh, 000h, 000h
        jmp    SHORT loc_1_15EE
loc_1_160F:
        mov    word ptr [ebx + 2], 0
        mov    word ptr [ebx + 18h], 0
loc_1_161B:
        push   eax
        lea    eax, [ebx + 3ch]
        mov    dword ptr [ebx + 3ch], eax
        mov    dword ptr [ebx + 40h], eax
        pop    eax
        add    ebx, 44h
        jmp    NEAR PTR loc_1_1545
loc_1_162E:
        db  0FEh, 00Dh, 005h, 06Fh, 000h, 000h
        cli
        push   esi
        lea    esi, [esi + 0b4h]
        push   ebx
        lea    ebx, [ebx + 32h]
        call   sub_1_2892
        pop    ebx
        pop    esi
        sti
        mov    al, 0ffh
        ret
loc_1_164B:
        cmp    dword ptr [ebx + 4], 0
        je     SHORT loc_1_166E
        db  0FEh, 00Dh, 005h, 06Fh, 000h, 000h
        cli
        push   esi
        lea    esi, [esi + 0ach]
        push   ebx
        lea    ebx, [ebx + 32h]
        call   sub_1_2892
        pop    ebx
        pop    esi
        sti
        mov    al, 0ffh
        ret
loc_1_166E:
        mov    al, 1
        ret
sub_1_1671:
        db  083h, 025h, 0ECh, 06Eh, 000h, 000h, 0FAh
        cli
        mov    byte ptr [esi + 90h], 1
        mov    ebx, dword ptr [esi + 84h]
        mov    al, 17h
        call   sub_1_1543
        mov    byte ptr [esi + 91h], 1
        mov    word ptr [esi + 118h], 0
        mov    ebx, dword ptr [esi + 84h]
        mov    dword ptr [esi + 8ch], ebx
        sti
        mov    byte ptr [ebx], 4
        mov    byte ptr [ebx + 2], 1
        mov    dword ptr [ebx + 4], esi
        mov    byte ptr [ebx + 1], 2
        lea    eax, [ebx + 1ch]
        mov    dword ptr [ebx + 1ch], eax
        mov    dword ptr [ebx + 20h], eax
        lea    eax, [ebx + 24h]
        mov    dword ptr [ebx + 24h], eax
        mov    dword ptr [ebx + 28h], eax
        lea    eax, [ebx + 2ch]
        mov    dword ptr [ebx + 2ch], eax
        mov    dword ptr [ebx + 30h], eax
        lea    eax, [ebx + 34h]
        mov    dword ptr [ebx + 34h], eax
        mov    dword ptr [ebx + 38h], eax
        lea    eax, [ebx + 3ch]
        mov    dword ptr [ebx + 3ch], eax
        mov    dword ptr [ebx + 40h], eax
        sub    eax, eax
        mov    dword ptr [esi + 0cch], eax
        mov    dword ptr [esi + 0e8h], eax
        mov    dword ptr [esi + 0f0h], eax
        mov    byte ptr [ebx + 18h], al
        mov    byte ptr [ebx + 19h], al
        mov    byte ptr [ebx + 3], al
        mov    dword ptr [ebx + 8], eax
        mov    dword ptr [ebx + 0ch], eax
        mov    word ptr [ebx + 10h], ax
        mov    eax, dword ptr [esi + 31h]
        mov    dword ptr [ebx + 12h], eax
        mov    dx, word ptr [esi + 35h]
        mov    word ptr [ebx + 16h], ax
        mov    edi, esi
loc_1_171C:
        cli
        lea    esi, [edi + 94h]
        call   sub_1_28E3
        lea    esi, [esi - 32h]
        sti
        je     SHORT loc_1_1741
        jecxz  loc_1_1739
        mov    byte ptr [esi + 1], 0bh
        call   sub_1_6BE9
loc_1_1739:
        db  0FEh, 005h, 005h, 06Fh, 000h, 000h
        jmp    SHORT loc_1_171C
loc_1_1741:
        cli
        lea    esi, [edi + 0a4h]
        call   sub_1_28E3
        lea    esi, [esi - 32h]
        sti
        je     SHORT loc_1_1766
        jecxz  loc_1_175E
        mov    byte ptr [esi + 1], 0bh
        call   sub_1_6BE9
loc_1_175E:
        db  0FEh, 005h, 005h, 06Fh, 000h, 000h
        jmp    SHORT loc_1_1741
loc_1_1766:
        cli
        lea    esi, [edi + 9ch]
        call   sub_1_28E3
        lea    esi, [esi - 32h]
        sti
        je     SHORT loc_1_178B
        jecxz  loc_1_1783
        mov    byte ptr [esi + 1], 0bh
        call   sub_1_6BE9
loc_1_1783:
        db  0FEh, 005h, 005h, 06Fh, 000h, 000h
        jmp    SHORT loc_1_1766
loc_1_178B:
        cli
        lea    esi, [edi + 0ach]
        call   sub_1_28E3
        lea    esi, [esi - 32h]
        sti
        je     SHORT loc_1_17B0
        jecxz  loc_1_17A8
        mov    byte ptr [esi + 1], 0bh
        call   sub_1_6BE9
loc_1_17A8:
        db  0FEh, 005h, 005h, 06Fh, 000h, 000h
        jmp    SHORT loc_1_178B
loc_1_17B0:
        cli
        lea    esi, [edi + 0b4h]
        call   sub_1_28E3
        lea    esi, [esi - 32h]
        sti
        je     SHORT loc_1_17D5
        jecxz  loc_1_17CD
        mov    byte ptr [esi + 1], 0bh
        call   sub_1_6BE9
loc_1_17CD:
        db  0FEh, 005h, 005h, 06Fh, 000h, 000h
        jmp    SHORT loc_1_17B0
loc_1_17D5:
        mov    esi, edi
        test   dword ptr [esi], 2
        jne    SHORT loc_1_17F0
        db  083h, 00Dh, 0ECh, 06Eh, 000h, 000h, 004h
        call   sub_1_007C
        jae    SHORT loc_1_17F0
        mov    al, 0f7h
        ret
loc_1_17F0:
        push   ecx
        mov    ecx, 100h
        mov    eax, esi
        db  0BEh, 0C0h, 077h, 000h, 000h
loc_1_17FD:
        mov    edi, dword ptr [esi]
        test   edi, edi
        je     SHORT loc_1_1837
        cmp    byte ptr [edi + 25h], 0ah
        jne    SHORT loc_1_180E
        mov    ebx, dword ptr [edi + 3ch]
        jmp    SHORT loc_1_1818
loc_1_180E:
        mov    ebx, dword ptr [edi + 38h]
        test   ebx, ebx
        je     SHORT loc_1_1837
        mov    ebx, dword ptr [ebx + 40h]
loc_1_1818:
        cmp    ebx, eax
        jne    SHORT loc_1_1837
        or     word ptr [edi + 40h], 40h
        cmp    dword ptr [esp], 0
        jne    SHORT loc_1_182C
        or     word ptr [edi + 6ah], 2
loc_1_182C:
        or     word ptr [edi + 42h], 4000h
        call   sub_1_5999
loc_1_1837:
        add    esi, 4
        loop   loc_1_17FD
        pop    ecx
        mov    ecx, 100h
loc_1_1842:
        mov    ebx, ecx
        dec    ebx
        shl    ebx, 3
        db  08Dh, 09Bh, 0C0h, 06Fh, 000h, 000h
        mov    esi, dword ptr [ebx]
loc_1_1850:
        cmp    esi, ebx
        je     SHORT loc_1_1890
        sub    esi, 1ch
        cmp    dword ptr [esi + 40h], eax
        jne    SHORT loc_1_187D
        mov    byte ptr [esi + 38h], 2
        mov    dword ptr [esi + 3ch], 2
        push   eax
        push   ecx
        push   ebx
        mov    edi, esi
        mov    byte ptr [edi + 0b9h], 0
        call   sub_1_5292
        jb     SHORT loc_1_1882
        pop    ebx
        pop    ecx
        pop    eax
loc_1_187D:
        mov    esi, dword ptr [esi + 1ch]
        jmp    SHORT loc_1_1850
loc_1_1882:
        push   dword ptr [esi + 1ch]
        call   sub_1_6338
        pop    esi
        pop    ebx
        pop    ecx
        pop    eax
        jmp    SHORT loc_1_1850
loc_1_1890:
        loop   loc_1_1842
        mov    esi, eax
        db  083h, 00Dh, 0ECh, 06Eh, 000h, 000h, 005h
        sub    al, al
        ret
loc_1_189E:
        sub    ecx, ecx
        test   byte ptr [ebx], 80h
        je     NEAR PTR sub_1_1671
        mov    al, 3
        ret
loc_1_18AC:
        test   byte ptr [ebx], 80h
        je     SHORT loc_1_18B4
        mov    al, 3
        ret
loc_1_18B4:
        mov    ebx, dword ptr [ebx + 4]
        mov    al, byte ptr [ebx]
        and    al, 7fh
        db  0BEh, 084h, 06Eh, 000h, 000h
loc_1_18C0:
        cmp    byte ptr [esi], al
        je     SHORT loc_1_18D3
        add    esi, 5
        db  081h, 0FEh, 0C0h, 06Eh, 000h, 000h
        jb     SHORT loc_1_18C0
        mov    al, 26h
        jmp    SHORT loc_1_18D8
loc_1_18D3:
        mov    esi, dword ptr [esi + 1]
        call   esi
loc_1_18D8:
        ret
sub_1_18D9:
        cli
        cmp    byte ptr [ebx + 25h], 0
        jne    SHORT loc_1_18E1
        ret
loc_1_18E1:
        cmp    byte ptr [ebx + 25h], 9
        je     SHORT loc_1_18E8
        ret
loc_1_18E8:
        test   byte ptr [ebx + 24h], 2
        je     SHORT loc_1_1902
        and    byte ptr [ebx + 24h], 0fdh
        push   esi
        push   edi
        lea    esi, [ebx]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
loc_1_1902:
        push   esi
        test   word ptr [ebx + 40h], 80h
        jne    SHORT loc_1_1912
        call   sub_1_235E
        jmp    SHORT loc_1_1936
loc_1_1912:
        and    word ptr [ebx + 40h], 0ff7fh
        sti
        mov    esi, dword ptr [ebx + 88h]
        test   word ptr [ebx + 42h], 8
        je     SHORT loc_1_1929
        mov    al, 0bh
loc_1_1929:
        mov    byte ptr [esi + 1], al
        call   sub_1_235E
        call   sub_1_6BE9
loc_1_1936:
        pop    esi
        ret
loc_1_1938:
        cli
        cmp    byte ptr [ebx + 25h], 9
        jne    SHORT loc_1_195B
        dec    byte ptr [ebx + 90h]
        je     SHORT loc_1_195D
        db  0B8h, 038h, 019h, 000h, 000h
        mov    ecx, dword ptr [ebx + 3ch]
        call   sub_1_34D0
        sti
        call   sub_1_3ED6
        ret
loc_1_195B:
        sti
        ret
loc_1_195D:
        sti
        mov    al, 5
        call   sub_1_18D9
        ret
loc_1_1966:
        mov    edi, ebx
        movzx  ecx, word ptr [edi + 8]
        cmp    ecx, 3ch
        jae    SHORT loc_1_1974
        mov    al, 1
        ret
loc_1_1974:
        mov    ebx, edi
        cmp    byte ptr [ebx + 0ah], 2ah
        jne    SHORT loc_1_19A5
        cmp    byte ptr [ebx + 1ah], 2ah
        jne    SHORT loc_1_198E
        mov    eax, dword ptr [esi + 40h]
        mov    dword ptr [ebx + 1ah], eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ebx + 1eh], eax
loc_1_198E:
        mov    ebx, dword ptr [edi + 4]
        sub    eax, eax
        call   sub_1_1381
        call   sub_1_146C
        mov    word ptr [edi + 8], cx
        ret
loc_1_19A2:
        mov    al, 21h
        ret
loc_1_19A5:
        call   sub_1_229D
        jb     SHORT loc_1_19A2
        mov    edx, dword ptr [edi + 4]
        mov    dword ptr [ebx + 80h], edx
        mov    edx, esi
        mov    eax, edi
        lea    esi, [edi + 0ah]
        lea    edi, [ebx + 54h]
        mov    ecx, 4
        cld
        rep movsd
        mov    edi, eax
        mov    al, byte ptr [ebx + 68h]
        mov    byte ptr [edi + 2], al
        mov    byte ptr [ebx + 25h], 9
        mov    cx, word ptr [edi + 8]
        mov    word ptr [ebx + 86h], cx
        mov    word ptr [ebx + 84h], 0
        mov    word ptr [ebx + 66h], 1
        mov    dword ptr [ebx + 3ch], edx
        test   dword ptr [edx + 4], 1
        jne    SHORT loc_1_19FD
        mov    al, 1
        jmp    SHORT loc_1_1A12
loc_1_19FD:
        test   dword ptr [edx + 4], 2
        je     SHORT loc_1_1A0D
        db  0A0h, 021h, 06Fh, 000h, 000h
        jmp    SHORT loc_1_1A12
loc_1_1A0D:
        db  0A0h, 01Fh, 06Fh, 000h, 000h
loc_1_1A12:
        mov    byte ptr [ebx + 90h], al
        cli
        mov    dword ptr [ebx + 88h], edi
        or     word ptr [ebx + 40h], 80h
        db  0B8h, 038h, 019h, 000h, 000h
        mov    ecx, edx
        call   sub_1_34D0
        sti
        call   sub_1_3ED6
        mov    al, 0ffh
        ret
loc_1_1A3A:
        sub    al, al
        mov    byte ptr [ebx + 1], al
        ret
sub_1_1A40:
        mov    al, 5
        jmp    NEAR PTR sub_1_1329
loc_1_1A47:
        mov    cl, 1
        jmp    SHORT loc_1_1A8B
loc_1_1A4B:
        mov    cx, 3
        mov    dx, word ptr [ebx + 0ah]
        mov    ax, word ptr [ebx + 8]
        add    ax, dx
        jb     SHORT loc_1_1A5E
        jmp    SHORT loc_1_1A8D
loc_1_1A5E:
        mov    al, 1
        ret
loc_1_1A61:
        mov    cl, 8
        jmp    SHORT loc_1_1A8B
loc_1_1A65:
        mov    cx, 2
        mov    dx, word ptr [ebx + 0ah]
        mov    ax, word ptr [ebx + 8]
        add    ax, dx
        jae    SHORT loc_1_1A8D
        mov    al, 1
        ret
loc_1_1A79:
        ret
loc_1_1A7A:
        and    cl, 0feh
        jmp    SHORT loc_1_1AA2
loc_1_1A7F:
        mov    edi, eax
loc_1_1A81:
        call   sub_1_5E2C
        mov    al, 0ffh
        ret
loc_1_1A89:
        sub    ecx, ecx
loc_1_1A8B:
        xor    edx, edx
loc_1_1A8D:
        call   sub_1_5D8D
        jb     SHORT loc_1_1A79
        db  0FEh, 00Dh, 005h, 06Fh, 000h, 000h
        test   word ptr [ebx + 40h], 1
        je     SHORT loc_1_1A7A
loc_1_1AA2:
        mov    byte ptr [edi + 3fh], cl
        cli
        lea    eax, [ebx + 30h]
        lea    esi, [edi + 32h]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [eax], esi
        mov    dword ptr [ebx + 34h], esi
        sti
        inc    byte ptr [ebx + 8dh]
        mov    al, byte ptr [ebx + 8fh]
        mov    byte ptr [edi + 3eh], al
        test   word ptr [ebx + 40h], 402h
loc_1_1ACF:
        jne    SHORT loc_1_1A81
        add    dx, word ptr [edi + 8]
        cmp    dx, word ptr [ebx + 6eh]
        ja     SHORT loc_1_1ACF
        mov    eax, edi
        mov    edi, dword ptr [ebx + 38h]
        call   sub_1_3821
        jae    SHORT loc_1_1A7F
        or     word ptr [ebx + 40h], 400h
        add    word ptr [esi + 3ch], dx
        mov    word ptr [esi + 1eh], 1
        mov    dx, word ptr [eax + 8]
        mov    word ptr [esi + 22h], dx
        mov    dword ptr [ebx + 74h], edx
        mov    dword ptr [esi + 24h], edx
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ebx + 74h], edx
        mov    dword ptr [esi + 24h], edx
        test   cl, 2
        je     SHORT loc_1_1B25
        inc    word ptr [esi + 1eh]
        mov    dx, word ptr [eax + 0ah]
        mov    word ptr [esi + 2ah], dx
        mov    edx, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 2ch], edx
loc_1_1B25:
        xor    edx, edx
        mov    word ptr [esi + 36h], dx
        mov    word ptr [esi + 3ah], cs
        mov    word ptr [ebx + 72h], dx
        shl    ecx, 9
        and    ch, 2
        or     byte ptr [esi + 11h], ch
        or     byte ptr [ebx + 43h], ch
        or     ch, 4
        cli
        test   word ptr [ebx + 6ah], 4
        je     SHORT loc_1_1B5D
        or     ch, 8
        db  0FFh, 00Dh, 0FFh, 06Eh, 000h, 000h
        and    word ptr [ebx + 6ah], 0fff3h
        mov    dx, word ptr [ebx + 64h]
loc_1_1B5D:
        sti
        mov    word ptr [esi + 38h], dx
        mov    byte ptr [esi + 35h], ch
        test   word ptr [ebx + 42h], 1
        jne    SHORT loc_1_1BC2
loc_1_1B6D:
        mov    byte ptr [esi + 34h], 16h
        or     word ptr [ebx + 40h], 200h
        cli
        lea    eax, [edi + 8]
        cmp    eax, dword ptr [edi + 8]
        jne    SHORT loc_1_1BCD
        call   sub_1_4F96
        sti
        jb     SHORT loc_1_1BB8
        mov    dword ptr [esi + 0ch], edi
        call   sub_1_0517
        db  08Ah, 00Dh, 020h, 06Fh, 000h, 000h
        mov    byte ptr [edi + 0b9h], cl
        push   edi
        mov    edi, dword ptr [edi + 40h]
        test   dword ptr [edi + 4], 2
        pop    edi
        je     SHORT loc_1_1BAA
loc_1_1BAA:
        push   eax
        mov    eax, dword ptr [edi + 70h]
        mov    dword ptr [edi + 5ch], eax
        pop    eax
        cmp    ax, 2
        ja     SHORT loc_1_1BBB
loc_1_1BB8:
        mov    al, 0ffh
        ret
loc_1_1BBB:
        and    dword ptr [edi + 34h], 0fffffff7h
        mov    al, 0ffh
        ret
loc_1_1BC2:
        mov    byte ptr [esi + 36h], 1
        and    word ptr [ebx + 42h], 0fffeh
        jmp    SHORT loc_1_1B6D
loc_1_1BCD:
        push   esi
        push   eax
        lea    eax, [edi + 8]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [esi + 4], eax
        xchg   esi, eax
        mov    dword ptr [esi], eax
        mov    dword ptr [edi + 0ch], eax
        pop    eax
        pop    esi
        sti
        mov    al, 0ffh
        ret
        db  000h, 000h
sub_1_1BE8:
        mov    al, 5
        call   sub_1_1237
        jb     SHORT loc_1_1BF7
        call   sub_1_646D
        ret
loc_1_1BF7:
        ret
loc_1_1BF8:
        mov    esi, edi
        call   sub_1_6BE9
        mov    al, 0ffh
        ret
loc_1_1C02:
        mov    edi, ebx
        call   sub_1_5D8D
        jb     SHORT loc_1_1BF8
        db  0FEh, 00Dh, 005h, 06Fh, 000h, 000h
        mov    byte ptr [edi + 3fh], 0
        mov    dl, byte ptr [ebx + 8eh]
        mov    byte ptr [edi + 3eh], dl
        cli
        push   esi
        lea    esi, [ebx + 28h]
        push   edi
        lea    edi, [edi + 32h]
        call   sub_1_287D
        pop    edi
        pop    esi
        inc    byte ptr [ebx + 8ch]
        or     word ptr [ebx + 42h], 8000h
        test   byte ptr [ebx + 24h], 20h
        je     SHORT loc_1_1C5A
        push   esi
        push   edi
        lea    esi, [ebx + 18h]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        and    byte ptr [ebx + 24h], 0dfh
        call   sub_1_5CEC
loc_1_1C5A:
        sti
        mov    al, 0ffh
        ret
        db  0C3h
loc_1_1C5F:
        mov    bl, al
        xor    bh, bh
        call   sub_1_24CC
        jae    SHORT loc_1_1C90
loc_1_1C6A:
        mov    al, 13h
        jmp    NEAR PTR loc_1_1D2A
loc_1_1C71:
        mov    al, 19h
        jmp    NEAR PTR loc_1_1D2A
loc_1_1C78:
        mov    edi, ebx
        mov    al, byte ptr [edi + 3]
        cmp    al, 0ffh
        je     NEAR PTR loc_1_1D2B
        mov    ebx, dword ptr [esi + 8ch]
        cmp    byte ptr [ebx + 2], al
        jne    SHORT loc_1_1C5F
loc_1_1C90:
        cmp    byte ptr [ebx + 1], 2
        jne    SHORT loc_1_1C6A
        test   byte ptr [ebx], 2
        jne    SHORT loc_1_1C71
        db  0FEh, 00Dh, 005h, 06Fh, 000h, 000h
        mov    byte ptr [edi + 3fh], 0
        mov    edx, edi
        mov    edi, ebx
        cli
        add    edi, 3ch
        cmp    edi, dword ptr [edi]
        jne    SHORT loc_1_1CCB
        lea    edx, [edx + 32h]
        sub    edi, 8
        mov    dword ptr [edx], edi
        mov    ebx, dword ptr [edi + 4]
        mov    dword ptr [edx + 4], ebx
        mov    dword ptr [ebx], edx
        mov    dword ptr [edi + 4], edx
        sti
        inc    byte ptr [edi - 1bh]
        mov    al, 0ffh
        ret
loc_1_1CCB:
        mov    ebx, dword ptr [edi]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [edi], eax
        mov    dword ptr [eax + 4], edi
        sub    ebx, 18h
        and    byte ptr [ebx + 24h], 0dfh
        mov    edi, edx
loc_1_1CDD:
        mov    al, byte ptr [ebx + 68h]
        mov    byte ptr [edi + 2], al
        cmp    byte ptr [ebx + 25h], 5
        jne    SHORT loc_1_1D0C
        push   esi
        lea    esi, [ebx + 28h]
        push   edi
        lea    edi, [edi + 32h]
        call   sub_1_287D
        pop    edi
        pop    esi
        or     word ptr [ebx + 42h], 8000h
        inc    byte ptr [ebx + 8ch]
        sti
        call   sub_1_5CEC
        jmp    SHORT loc_1_1D26
loc_1_1D0C:
        db  0FEh, 005h, 005h, 06Fh, 000h, 000h
        sti
        mov    ebx, edi
        call   sub_1_5D8D
        sti
        mov    esi, edi
        call   sub_1_6BE9
        mov    al, 0ffh
        jmp    SHORT loc_1_1D29
loc_1_1D26:
        sti
        mov    al, 0ffh
loc_1_1D29:
        ret
loc_1_1D2A:
        ret
loc_1_1D2B:
        mov    ebx, esi
        db  0FEh, 00Dh, 005h, 06Fh, 000h, 000h
        mov    byte ptr [edi + 3fh], 0
        cli
        test   dword ptr [ebx], 400h
        jne    SHORT loc_1_1D56
loc_1_1D40:
        push   esi
        lea    esi, [ebx + 9ch]
        push   edi
        lea    edi, [edi + 32h]
        call   sub_1_287D
        pop    edi
        pop    esi
        sti
        mov    al, 0ffh
        ret
loc_1_1D56:
        mov    edx, edi
        mov    esi, dword ptr [ebx + 84h]
loc_1_1D5E:
        cmp    esi, dword ptr [ebx + 88h]
        jae    SHORT loc_1_1D91
        cmp    byte ptr [esi + 2], 0
        je     SHORT loc_1_1D79
        lea    edi, [esi + 3ch]
        call   sub_1_2903
        lea    edi, [edi - 18h]
        jne    SHORT loc_1_1D7E
loc_1_1D79:
        add    esi, 44h
        jmp    SHORT loc_1_1D5E
loc_1_1D7E:
        mov    ebx, edi
        mov    edi, edx
        mov    cl, byte ptr [esi + 2]
        mov    byte ptr [edi + 3], cl
        and    byte ptr [ebx + 24h], 0dfh
        jmp    NEAR PTR loc_1_1CDD
loc_1_1D91:
        and    dword ptr [ebx], 0fffffbffh
        mov    edi, edx
        jmp    SHORT loc_1_1D40
        db  000h
sub_1_1D9C:
        inc    word ptr [ebx + 4ah]
        or     dword ptr [ebx + 3ch], 80h
        push   ebx
        call   sub_1_4E6C
        ret
sub_1_1DAE:
        db  083h, 00Dh, 0ECh, 06Eh, 000h, 000h, 078h
        sub    eax, eax
loc_1_1DB7:
        cmp    eax, 8
        je     SHORT loc_1_1DD0
        db  08Bh, 034h, 085h, 040h, 06Fh, 000h, 000h
        test   esi, esi
        je     SHORT loc_1_1DCD
        inc    dword ptr [esi + 0cch]
loc_1_1DCD:
        inc    eax
        jmp    SHORT loc_1_1DB7
loc_1_1DD0:
        ret
sub_1_1DD1:
        test   dword ptr [ebx + 34h], 40h
        je     SHORT loc_1_1DED
        cmp    byte ptr [ebx + 0bah], 0ah
        jb     SHORT loc_1_1DED
        cli
        call   sub_1_1EE8
        sti
        inc    dword ptr [ebx + 50h]
loc_1_1DED:
        test   dword ptr [ebx + 34h], 80h
        je     SHORT loc_1_1E02
        call   sub_1_1FA8
        and    dword ptr [ebx + 34h], 0ffff7fffh
loc_1_1E02:
        mov    eax, dword ptr [ebx + 4ch]
        mov    dword ptr [ebx + 64h], eax
        ret
sub_1_1E09:
        cmp    byte ptr [ebx + 38h], 4
        jne    SHORT loc_1_1E1F
        mov    edi, dword ptr [ebx + 40h]
        inc    dword ptr [edi + 110h]
        mov    edi, ebx
        call   sub_1_3F66
loc_1_1E1F:
        mov    eax, dword ptr [ebx + 6ch]
        mov    dword ptr [ebx + 58h], eax
        ret
sub_1_1E26:
        cmp    byte ptr [ebx + 38h], 1
        jne    SHORT loc_1_1E66
        mov    edi, dword ptr [ebx + 40h]
        inc    dword ptr [edi + 108h]
        dec    byte ptr [ebx + 0b9h]
        js     SHORT loc_1_1E48
        or     dword ptr [ebx + 3ch], 8
        push   ebx
        call   sub_1_4E6C
        ret
loc_1_1E48:
        db  0A0h, 020h, 06Fh, 000h, 000h
        mov    byte ptr [ebx + 0b9h], al
        mov    eax, dword ptr [ebx + 68h]
        mov    dword ptr [ebx + 70h], eax
        mov    dword ptr [ebx + 3ch], 2
        push   ebx
        call   sub_1_4E6C
loc_1_1E66:
        ret
sub_1_1E67:
        cmp    byte ptr [ebx + 38h], 2
        jne    SHORT loc_1_1E96
        mov    edi, dword ptr [ebx + 40h]
        or     edi, edi
        je     SHORT loc_1_1E90
        inc    dword ptr [edi + 108h]
        dec    byte ptr [ebx + 0b9h]
        js     SHORT loc_1_1E90
        mov    dword ptr [ebx + 3ch], 2
        push   ebx
        call   sub_1_4E6C
        ret
loc_1_1E90:
        push   ebx
        call   sub_1_6435
loc_1_1E96:
        ret
sub_1_1E97:
        cmp    byte ptr [ebx + 38h], 4
        jne    SHORT loc_1_1EE0
        mov    edi, dword ptr [ebx + 40h]
        inc    dword ptr [edi + 108h]
        dec    byte ptr [ebx + 0b9h]
        js     SHORT loc_1_1EC2
        call   sub_1_2038
        or     dword ptr [ebx + 3ch], 1
        and    dword ptr [ebx + 34h], 0fffffff7h
        push   ebx
        call   sub_1_4E6C
        ret
loc_1_1EC2:
        db  0A0h, 020h, 06Fh, 000h, 000h
        mov    byte ptr [ebx + 0b9h], al
        mov    eax, dword ptr [ebx + 68h]
        mov    dword ptr [ebx + 70h], eax
        mov    dword ptr [ebx + 3ch], 2
        push   ebx
        call   sub_1_4E6C
loc_1_1EE0:
        ret
loc_1_1EE1:
        mov    al, 18h
        jmp    NEAR PTR sub_1_0C20
sub_1_1EE8:
        push   eax
        push   ecx
        push   edx
        xor    eax, eax
        db  08Ah, 015h, 02Dh, 06Fh, 000h, 000h
        mov    al, byte ptr [ebx + 48h]
        movzx  ecx, byte ptr [ebx + 0bbh]
        cmp    cl, dl
        ja     SHORT loc_1_1F37
        cmp    dword ptr [ebx + 68h], 3ch
        jb     SHORT loc_1_1F14
        push   ecx
        mov    ecx, dword ptr [ebx + 40h]
        cmp    al, byte ptr [ecx + 134h]
        pop    ecx
        jae    SHORT loc_1_1F68
loc_1_1F14:
        test   dword ptr [ebx + 34h], 2000h
        je     SHORT loc_1_1F68
        mov    byte ptr [ebx + 0bch], al
        cmp    al, 7fh
        je     SHORT loc_1_1F68
        cmp    byte ptr [ebx + 0bdh], 0ffh
        je     SHORT loc_1_1F32
        jmp    SHORT loc_1_1F68
loc_1_1F32:
        inc    byte ptr [ebx + 48h]
        jmp    SHORT loc_1_1F68
loc_1_1F37:
        and    dword ptr [ebx + 34h], 0ffffbfffh
        cmp    byte ptr [ebx + 48h], 1
        je     SHORT loc_1_1F68
        mov    byte ptr [ebx + 0bdh], al
        cmp    al, byte ptr [ebx + 0bch]
        jne    SHORT loc_1_1F5F
        dec    al
        mov    byte ptr [ebx + 48h], al
        mov    byte ptr [ebx + 0bch], al
        jmp    SHORT loc_1_1F68
loc_1_1F5F:
        mov    al, byte ptr [ebx + 0bch]
        mov    byte ptr [ebx + 48h], al
loc_1_1F68:
        and    dword ptr [ebx + 34h], 0ffffdfffh
        sub    ecx, ecx
        mov    byte ptr [ebx + 0bah], cl
        mov    byte ptr [ebx + 0bbh], cl
        test   dword ptr [ebx + 34h], 4000h
        jne    SHORT loc_1_1FA4
        inc    byte ptr [ebx + 0c1h]
        cmp    word ptr [ebx + 0c1h], 78h
        jne    SHORT loc_1_1FA4
        mov    byte ptr [ebx + 0c1h], cl
        not    cl
        mov    byte ptr [ebx + 0bdh], cl
loc_1_1FA4:
        pop    edx
        pop    ecx
        pop    eax
        ret
sub_1_1FA8:
        sub    eax, eax
        mov    al, byte ptr [ebx + 49h]
        cli
        movzx  ecx, byte ptr [ebx + 56h]
        cmp    word ptr [ebx + 4ah], 0
        jne    SHORT loc_1_1FFD
        jecxz  loc_1_1FBE
        dec    byte ptr [ebx + 56h]
loc_1_1FBE:
        cmp    byte ptr [ebx + 54h], 0
        jne    SHORT loc_1_1FF8
        mov    byte ptr [ebx + 0c0h], al
        cmp    dword ptr [ebx + 68h], 3ch
        jb     SHORT loc_1_1FDD
        push   ecx
        mov    ecx, dword ptr [ebx + 40h]
        cmp    al, byte ptr [ecx + 134h]
        pop    ecx
        ja     SHORT loc_1_1FF0
loc_1_1FDD:
        test   dword ptr [ebx + 34h], 8000h
        je     SHORT loc_1_1FF0
        mov    al, 7fh
        cmp    byte ptr [ebx + 49h], al
        jae    SHORT loc_1_1FF0
        inc    byte ptr [ebx + 49h]
loc_1_1FF0:
        mov    word ptr [ebx + 4ah], 0
        sti
        ret
loc_1_1FF8:
        dec    byte ptr [ebx + 54h]
        sti
        ret
loc_1_1FFD:
        add    ecx, 2
        cmp    ecx, 0ah
        jl     SHORT loc_1_2017
        mov    cl, byte ptr [ebx + 55h]
        mov    byte ptr [ebx + 54h], cl
        add    byte ptr [ebx + 55h], 3ch
        jae    SHORT loc_1_2015
        mov    byte ptr [ebx + 55h], 0ffh
loc_1_2015:
        xor    ecx, ecx
loc_1_2017:
        mov    byte ptr [ebx + 56h], cl
        cmp    al, 1
        je     SHORT loc_1_1FF0
        cmp    al, byte ptr [ebx + 0c0h]
        jne    SHORT loc_1_202D
        dec    word ptr [ebx + 0c0h]
loc_1_202D:
        mov    al, byte ptr [ebx + 0c0h]
        mov    byte ptr [ebx + 49h], al
        jmp    SHORT loc_1_1FF0
sub_1_2038:
        mov    ecx, dword ptr [ebx + 40h]
        test   dword ptr [ecx + 4], 2
        jne    SHORT loc_1_205F
        mov    ecx, dword ptr [ebx + 68h]
        shl    ecx, 4
        cmp    ecx, 3c0h
        jb     SHORT loc_1_2055
        shr    ecx, 3
loc_1_2055:
        mov    eax, dword ptr [ebx + 70h]
        cmp    eax, ecx
        jae    SHORT loc_1_205F
        add    dword ptr [ebx + 70h], eax
loc_1_205F:
        ret
loc_1_2060:
        mov    cl, 1
        jmp    SHORT loc_1_2066
loc_1_2064:
        sub    ecx, ecx
loc_1_2066:
        mov    edi, ebx
        movzx  edx, word ptr [edi + 8]
        cmp    edx, 400h
        ja     SHORT loc_1_209B
        add    edx, 2ch
        mov    bl, byte ptr [edi + 3]
        call   sub_1_24CC
        jb     SHORT loc_1_2098
        cmp    byte ptr [ebx + 1], 2
        jne    SHORT loc_1_2098
        test   byte ptr [ebx], 2
        jne    SHORT loc_1_209E
        mov    eax, esi
        call   sub_1_37A5
        jae    SHORT loc_1_20A1
        mov    al, 9
        ret
loc_1_2098:
        mov    al, 13h
        ret
loc_1_209B:
        mov    al, 1
        ret
loc_1_209E:
        mov    al, 19h
        ret
loc_1_20A1:
        mov    dword ptr [esi + 14h], eax
        movzx  eax, word ptr [esi + 22h]
        add    word ptr [esi + 22h], 2ch
        push   esi
        push   edi
        mov    edi, dword ptr [esi + 24h]
        add    edi, eax
        lea    edi, [edi + 1ch]
        lea    esi, [ebx + 8]
        movsd
        movsd
        movsd
        movsd
        pop    edi
        pop    esi
        mov    ebx, esi
        mov    edx, edi
        test   ecx, ecx
        jne    SHORT loc_1_20D7
        lea    esi, [edi + 0ah]
        mov    edi, dword ptr [ebx + 24h]
        add    edi, eax
        lea    edi, [edi + 0ch]
        movsd
        movsd
        movsd
        movsd
loc_1_20D7:
        mov    esi, ebx
        mov    edi, edx
        mov    bx, word ptr [edi + 8]
        mov    word ptr [esi + 2ah], bx
        mov    ebx, dword ptr [edi + 4]
        mov    dword ptr [esi + 2ch], ebx
        mov    dword ptr [esi + 30h], edi
        or     word ptr [esi + 10h], 8
        inc    word ptr [esi + 1eh]
        mov    ebx, dword ptr [esi + 24h]
        add    ebx, eax
        mov    word ptr [ebx], 2ch
        mov    word ptr [ebx + 2], 0efffh
        mov    byte ptr [ebx + 4], 8
        jecxz  loc_1_210F
        mov    byte ptr [ebx + 4], 9
loc_1_210F:
        sub    eax, eax
        mov    byte ptr [ebx + 5], al
        mov    word ptr [ebx + 6], ax
        mov    word ptr [ebx + 8], ax
        mov    word ptr [ebx + 0ah], ax
        mov    ebx, dword ptr [esi + 14h]
        cli
        push   edi
        lea    edi, [ebx + 0bch]
        call   sub_1_2868
        pop    edi
        sti
        or     dword ptr [ebx], 100h
        db  083h, 00Dh, 0ECh, 06Eh, 000h, 000h, 008h
        mov    al, 0ffh
        ret
loc_1_2142:
        mov    edi, ebx
        mov    bl, byte ptr [edi + 3]
        cmp    bl, 0ffh
        mov    al, 1
        jne    SHORT loc_1_2159
        mov    al, 2
        jmp    SHORT loc_1_2177
loc_1_2152:
        mov    edi, ebx
        mov    bl, byte ptr [edi + 3]
        sub    al, al
loc_1_2159:
        call   sub_1_24CC
        jb     SHORT loc_1_2176
        mov    esi, ebx
        cli
        cmp    byte ptr [esi + 1], 2
        jne    SHORT loc_1_2173
        test   byte ptr [esi], 2
        je     SHORT loc_1_2177
        sti
        sti
        mov    al, 19h
        ret
loc_1_2173:
        sti
        mov    al, 13h
loc_1_2176:
        ret
loc_1_2177:
        db  0FEh, 00Dh, 005h, 06Fh, 000h, 000h
        mov    byte ptr [edi + 3fh], 0
        or     al, al
        je     SHORT loc_1_21B6
        cmp    al, 1
        jne    SHORT loc_1_219F
        cli
        push   esi
        lea    esi, [esi + 2ch]
        push   edi
        lea    edi, [edi + 32h]
        call   sub_1_287D
        pop    edi
        pop    esi
        sti
        inc    byte ptr [esi + 18h]
        jmp    SHORT loc_1_21B3
loc_1_219F:
        cli
        push   esi
        lea    esi, [esi + 94h]
        push   edi
        lea    edi, [edi + 32h]
        call   sub_1_287D
        pop    edi
        pop    esi
        sti
loc_1_21B3:
        mov    al, 0ffh
        ret
loc_1_21B6:
        cli
        mov    ax, word ptr [edi + 8]
        inc    byte ptr [esi + 18h]
        mov    ebx, dword ptr [esi + 4]
        lea    ecx, [ebx + 0a4h]
        cmp    ecx, dword ptr [ebx + 0a4h]
        je     SHORT loc_1_21E1
        mov    esi, dword ptr [ebx + 0a4h]
loc_1_21D5:
        cmp    ax, word ptr [esi - 2ah]
        ja     SHORT loc_1_21F7
        mov    esi, dword ptr [esi]
        cmp    esi, ecx
        jne    SHORT loc_1_21D5
loc_1_21E1:
        push   esi
        lea    esi, [ebx + 0a4h]
        push   edi
        lea    edi, [edi + 32h]
        call   sub_1_287D
        pop    edi
        pop    esi
loc_1_21F3:
        sti
        mov    al, 0ffh
        ret
loc_1_21F7:
        lea    edi, [edi + 32h]
        mov    dword ptr [edi], esi
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [eax], edi
        jmp    SHORT loc_1_21F3
        db  000h, 000h, 000h
sub_1_220C:
        push   eax
        push   ecx
        db  0A1h, 060h, 06Fh, 000h, 000h
        db  00Fh, 0B6h, 00Dh, 023h, 06Fh, 000h, 000h
loc_1_221A:
        cmp    dword ptr [eax + 30h], edi
        je     SHORT loc_1_2228
loc_1_221F:
        add    eax, 3eh
        loop   loc_1_221A
        clc
loc_1_2225:
        pop    ecx
        pop    eax
        ret
loc_1_2228:
        test   word ptr [eax + 10h], 8000h
        jne    SHORT loc_1_221F
        stc
        jmp    SHORT loc_1_2225
sub_1_2233:
        db  080h, 03Dh, 0C0h, 06Eh, 000h, 000h, 000h
        je     SHORT loc_1_226D
        cld
        push   ecx
        push   edi
        push   eax
        push   0
        push   eax
        db  0CDh, 020h, 00Dh, 000h, 040h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_2267
        cld
        pop    ecx
        push   eax
        mov    edi, eax
        xor    eax, eax
        push   ecx
        shr    ecx, 2
        rep stosd
        pop    ecx
        and    ecx, 3
        rep stosb
        pop    eax
        pop    edi
        pop    ecx
        ret
loc_1_2267:
        add    esp, 4
        pop    edi
        pop    ecx
        ret
loc_1_226D:
        push   0
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        ret
sub_1_227A:
        db  080h, 03Dh, 0C0h, 06Eh, 000h, 000h, 000h
        je     SHORT loc_1_2290
        push   0
        push   eax
        db  0CDh, 020h, 00Eh, 000h, 040h, 000h
        add    esp, 8
        ret
loc_1_2290:
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        ret
sub_1_229D:
        push   edx
        mov    ecx, 0feh
        db  00Fh, 0B6h, 015h, 034h, 06Fh, 000h, 000h
        sub    eax, eax
        cli
loc_1_22AD:
        cmp    edx, 0feh
        jae    SHORT loc_1_22C5
loc_1_22B5:
        db  03Bh, 004h, 095h, 0C0h, 077h, 000h, 000h
        je     SHORT loc_1_22C9
        inc    edx
        loop   loc_1_22AD
loc_1_22C1:
        sti
        pop    edx
        stc
        ret
loc_1_22C5:
        sub    edx, edx
        jmp    SHORT loc_1_22B5
loc_1_22C9:
        push   edx
        mov    eax, 0d0h
        call   sub_1_2233
        pop    edx
        test   eax, eax
        je     SHORT loc_1_22C1
        mov    ebx, eax
        sub    eax, eax
        db  089h, 01Ch, 095h, 0C0h, 077h, 000h, 000h
        mov    byte ptr [ebx + 25h], 0
        sti
        mov    word ptr [ebx + 40h], ax
        mov    word ptr [ebx + 42h], ax
        mov    word ptr [ebx + 6ah], ax
        mov    dword ptr [ebx + 3ch], eax
        mov    dword ptr [ebx + 38h], eax
        mov    byte ptr [ebx + 24h], al
        mov    word ptr [ebx + 64h], ax
        mov    word ptr [ebx + 66h], ax
        mov    byte ptr [ebx + 69h], al
        mov    dword ptr [ebx + 88h], eax
        mov    byte ptr [ebx + 8dh], al
        mov    byte ptr [ebx + 8ch], al
        mov    byte ptr [ebx + 6dh], al
        mov    ax, 1000h
        mov    word ptr [ebx + 6eh], ax
        db  0A0h, 02Eh, 06Fh, 000h, 000h
        mov    byte ptr [ebx + 6ch], al
        cli
        db  0A1h, 0BCh, 06Fh, 000h, 000h
        mov    dword ptr [ebx + 20h], eax
        db  089h, 01Dh, 0BCh, 06Fh, 000h, 000h
        sti
        inc    dl
        db  088h, 015h, 034h, 06Fh, 000h, 000h
        mov    byte ptr [ebx + 68h], dl
        lea    eax, [ebx + 28h]
        mov    dword ptr [ebx + 28h], eax
        mov    dword ptr [ebx + 2ch], eax
        lea    eax, [ebx + 30h]
        mov    dword ptr [ebx + 30h], eax
        mov    dword ptr [ebx + 34h], eax
        clc
        pop    edx
        ret
sub_1_235E:
        mov    word ptr [ebx + 40h], 2000h
        push   ebx
        call   sub_1_5BF3
        ret
sub_1_236B:
        push   ecx
        push   edx
        cli
        pushfd
        cmp    dword ptr [eax + 28h], 1
        je     SHORT loc_1_23AC
        movzx  ebx, word ptr [edi + 0ah]
        xor    bl, bh
        sub    bh, bh
        db  08Dh, 014h, 0DDh, 0C0h, 06Fh, 000h, 000h
        mov    ebx, dword ptr [edx]
loc_1_2386:
        cmp    edx, ebx
        je     SHORT loc_1_23DF
        cmp    eax, dword ptr [ebx + 24h]
        jne    SHORT loc_1_23A8
        mov    ecx, dword ptr [edi + 8]
        cmp    ecx, dword ptr [ebx + 72h]
        jne    SHORT loc_1_23A8
        mov    cx, word ptr [edi + 6]
        cmp    cx, word ptr [ebx + 70h]
        jne    SHORT loc_1_23A8
loc_1_23A1:
        popfd
        pop    edx
        pop    ecx
        sub    ebx, 1ch
        ret
loc_1_23A8:
        mov    ebx, dword ptr [ebx]
        jmp    SHORT loc_1_2386
loc_1_23AC:
        movzx  ebx, word ptr [edi + 0ch]
        xor    bl, bh
        sub    bh, bh
        db  08Dh, 014h, 0DDh, 0C0h, 06Fh, 000h, 000h
        mov    ebx, dword ptr [edx]
loc_1_23BD:
        cmp    edx, ebx
        je     SHORT loc_1_23DF
        cmp    eax, dword ptr [ebx + 24h]
        jne    SHORT loc_1_23DB
        mov    ecx, dword ptr [edi + 0ah]
        cmp    ecx, dword ptr [ebx + 74h]
        jne    SHORT loc_1_23DB
        mov    cx, word ptr [edi + 8]
        and    cl, 7fh
        cmp    cx, word ptr [ebx + 72h]
        je     SHORT loc_1_23A1
loc_1_23DB:
        mov    ebx, dword ptr [ebx]
        jmp    SHORT loc_1_23BD
loc_1_23DF:
        mov    ebx, edx
        popfd
        pop    edx
        pop    ecx
        stc
        ret
sub_1_23E6:
        push   ebx
        push   esi
        push   edi
        mov    edi, esi
        mov    ebx, edi
        push   ecx
        mov    ecx, esp
        call   sub_1_14FC
        pop    ecx
loc_1_23F6:
        cli
        lea    ebx, [edi + 1ch]
        call   sub_1_28F3
        sti
        je     SHORT loc_1_241A
        dec    byte ptr [edi + 3]
        mov    esi, dword ptr [ebx + 88h]
        call   sub_1_235E
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
        jmp    SHORT loc_1_23F6
loc_1_241A:
        cli
        lea    ebx, [edi + 24h]
        call   sub_1_28F3
        sti
        je     SHORT loc_1_243E
        dec    byte ptr [edi + 3]
        mov    esi, dword ptr [ebx + 88h]
        call   sub_1_235E
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
        jmp    SHORT loc_1_241A
loc_1_243E:
        cli
        lea    esi, [edi + 2ch]
        call   sub_1_28E3
        lea    esi, [esi - 32h]
        sti
        je     SHORT loc_1_245D
        mov    byte ptr [esi + 1], al
        db  0FEh, 005h, 005h, 06Fh, 000h, 000h
        call   sub_1_6BE9
        jmp    SHORT loc_1_243E
loc_1_245D:
        cli
        lea    esi, [edi + 34h]
        call   sub_1_28E3
        lea    esi, [esi - 32h]
        sti
        je     SHORT loc_1_247C
        mov    byte ptr [esi + 1], al
        db  0FEh, 005h, 005h, 06Fh, 000h, 000h
        call   sub_1_6BE9
        jmp    SHORT loc_1_245D
loc_1_247C:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_2480:
        push   edi
        cli
        cld
        mov    ebx, dword ptr [eax + 8ch]
        cmp    byte ptr [ebx + 2], 0
        je     SHORT loc_1_249D
        mov    ecx, 4
        push   esi
        lea    edi, [ebx + 8]
        repe cmpsd
        pop    esi
        je     SHORT loc_1_24C4
loc_1_249D:
        mov    ebx, dword ptr [eax + 84h]
loc_1_24A3:
        cmp    ebx, dword ptr [eax + 88h]
        ja     SHORT loc_1_24C8
        cmp    byte ptr [ebx + 2], 0
        je     SHORT loc_1_24BF
        mov    ecx, 4
        push   esi
        lea    edi, [ebx + 8]
        repe cmpsd
        pop    esi
        je     SHORT loc_1_24C4
loc_1_24BF:
        add    ebx, 44h
        jmp    SHORT loc_1_24A3
loc_1_24C4:
        sti
        pop    edi
        clc
        ret
loc_1_24C8:
        sti
        pop    edi
        stc
        ret
sub_1_24CC:
        test   bl, bl
        je     SHORT loc_1_2506
        cmp    bl, 0ffh
        je     SHORT loc_1_2506
        push   edi
        cli
        mov    edi, dword ptr [esi + 8ch]
        cmp    byte ptr [edi + 2], bl
        jne    SHORT loc_1_24EE
loc_1_24E2:
        sti
        mov    ebx, edi
        mov    dword ptr [esi + 8ch], ebx
        pop    edi
        clc
        ret
loc_1_24EE:
        mov    edi, dword ptr [esi + 84h]
loc_1_24F4:
        cmp    byte ptr [edi + 2], bl
        je     SHORT loc_1_24E2
        add    edi, 44h
        cmp    edi, dword ptr [esi + 88h]
        jbe    SHORT loc_1_24F4
        sti
        pop    edi
loc_1_2506:
        mov    al, 13h
        stc
        ret
sub_1_250A:
        cmp    byte ptr [esi + 3], 0
        jne    SHORT loc_1_2521
        push   ebx
        mov    ebx, dword ptr [esi + 4]
        dec    byte ptr [ebx + 91h]
        mov    byte ptr [esi + 2], 0
        pop    ebx
        clc
        ret
loc_1_2521:
        mov    byte ptr [esi + 1], 3
        stc
        ret
sub_1_2527:
        push   esi
        push   ebx
        mov    esi, edi
        call   sub_1_2480
        jb     SHORT loc_1_254D
        mov    esi, ebx
        cmp    byte ptr [esi + 2], 1
        je     SHORT loc_1_254C
        mov    al, 17h
        call   sub_1_23E6
        call   sub_1_250A
        mov    al, 0fh
        jb     SHORT loc_1_254C
        sub    al, al
loc_1_254C:
        clc
loc_1_254D:
        pop    ebx
        pop    esi
        ret
sub_1_2550:
        cmp    al, 8
        jae    SHORT loc_1_2563
        movzx  eax, al
        db  08Bh, 034h, 085h, 040h, 06Fh, 000h, 000h
        test   esi, esi
        je     SHORT loc_1_2563
        ret
loc_1_2563:
        mov    al, 23h
        stc
        ret
loc_1_2567:
        adc    al, byte ptr [edx]
        ljmp   0d11h:86081205h
        db  020h
sub_1_2571:
        cmp    dword ptr [eax + 28h], 1
        je     SHORT loc_1_25A2
        mov    edx, dword ptr [edi + 6]
        mov    dword ptr [esi], edx
        mov    dx, word ptr [edi + 0ah]
        mov    word ptr [esi + 4], dx
        mov    edx, dword ptr [eax + 31h]
        mov    dword ptr [esi + 6], edx
        mov    dx, word ptr [eax + 35h]
        mov    word ptr [esi + 0ah], dx
        mov    dx, word ptr [edi + 0ch]
        mov    word ptr [esi + 0ch], dx
        sub    edx, edx
        mov    ecx, 0eh
        ret
loc_1_25A2:
        mov    dh, byte ptr [edi + 1]
        mov    dl, 10h
        mov    word ptr [esi], dx
        mov    edx, dword ptr [edi + 8]
        mov    ecx, edx
        and    dl, 7fh
        and    cl, 80h
        mov    dword ptr [esi + 2], edx
        mov    dx, word ptr [edi + 0ch]
        mov    word ptr [esi + 6], dx
        mov    edx, dword ptr [eax + 31h]
        or     dl, cl
        mov    dword ptr [esi + 8], edx
        mov    dx, word ptr [eax + 35h]
        mov    word ptr [esi + 0ch], dx
        xor    ecx, ecx
        xor    edx, edx
        test   byte ptr [edi + 8], 80h
        je     SHORT loc_1_261F
        mov    cl, byte ptr [edi + 0eh]
        and    ecx, 1fh
        cmp    ecx, 2
        jbe    SHORT loc_1_2623
        push   ebx
        mov    bl, byte ptr [edi + 0fh]
        and    ebx, 70h
        cmp    bl, 50h
        jae    SHORT loc_1_25FD
        shr    ebx, 3
        db  00Fh, 0B7h, 093h, 067h, 025h, 000h, 000h
        add    edx, ecx
loc_1_25FD:
        push   eax
        push   ecx
        push   esi
        push   edi
        xchg   edi, esi
        shr    ecx, 1
        lea    esi, [esi + 0eh]
        lea    edi, [edi + 0eh]
        cld
        lodsw
        and    al, 1fh
        stosw
        dec    ecx
        rep movsw
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        xor    byte ptr [esi + 0fh], 80h
        pop    ebx
loc_1_261F:
        add    ecx, 0eh
        ret
loc_1_2623:
        sub    ecx, ecx
        and    byte ptr [esi + 8], 7fh
        jmp    SHORT loc_1_261F
sub_1_262B:
        push   esi
        xor    eax, eax
        lea    edi, [edi + 31h]
        mov    ecx, 3
        push   ecx
        cld
        repe cmpsw
        pop    ecx
        je     SHORT loc_1_2645
        pop    edi
        repe scasw
        je     SHORT loc_1_2652
        ret
loc_1_2645:
        mov    edi, esi
        rep stosw
        mov    byte ptr [ebx + 0b6h], 0eh
        pop    eax
loc_1_2652:
        or     dword ptr [ebx + 34h], 400h
        ret
sub_1_265A:
        db  03Bh, 005h, 0E8h, 06Dh, 000h, 000h
        jae    SHORT loc_1_2666
        sub    eax, eax
        jmp    SHORT loc_1_2670
loc_1_2666:
        push   edx
        sub    edx, edx
        db  0F7h, 03Dh, 0E8h, 06Dh, 000h, 000h
        pop    edx
loc_1_2670:
        ret
sub_1_2671:
        test   dword ptr [ebx + 4], 1
        jne    SHORT loc_1_267F
        xor    eax, eax
        mov    ecx, eax
        ret
loc_1_267F:
        push   edx
        mov    ecx, dword ptr [ebx + 40h]
        movzx  edx, word ptr [ebx + 92h]
        imul   eax, edx, 1f40h
        sub    edx, edx
        idiv   ecx
        test   edx, edx
        je     SHORT loc_1_2699
        inc    eax
loc_1_2699:
        movzx  ecx, byte ptr [ebx + 134h]
        imul   ecx, eax
        pop    edx
        ret
sub_1_26A5:
        push   eax
        push   edi
        sub    eax, eax
        mov    dword ptr [esi + 50h], eax
        mov    dword ptr [esi + 3ch], eax
        mov    byte ptr [esi + 44h], al
        mov    byte ptr [esi + 81h], al
        mov    byte ptr [esi + 45h], al
        mov    byte ptr [esi + 46h], al
        mov    byte ptr [esi + 47h], al
        mov    word ptr [esi + 78h], 0f0f0h
        mov    word ptr [esi + 4ah], ax
        mov    byte ptr [esi + 39h], al
        push   ebx
        mov    ebx, dword ptr [esi + 40h]
        call   sub_1_2671
        test   dword ptr [ebx + 4], 2
        jne    SHORT loc_1_26E2
        shl    eax, 1
loc_1_26E2:
        call   sub_1_265A
        xchg   ecx, eax
        call   sub_1_265A
        xchg   ecx, eax
        db  003h, 005h, 00Bh, 06Fh, 000h, 000h
        add    eax, ecx
        pop    ebx
        mov    dword ptr [esi + 68h], eax
        mov    dword ptr [esi + 70h], eax
        db  0A1h, 007h, 06Fh, 000h, 000h
        mov    dword ptr [esi + 6ch], eax
        db  0A1h, 00Fh, 06Fh, 000h, 000h
        mov    dword ptr [esi + 74h], eax
        db  0A0h, 020h, 06Fh, 000h, 000h
        mov    byte ptr [esi + 0b9h], al
        mov    dword ptr [esi + 58h], 0ffffffffh
        mov    dword ptr [esi + 5ch], 0ffffffffh
        mov    dword ptr [esi + 60h], 0ffffffffh
        mov    dword ptr [esi + 64h], 0ffffffffh
        db  0A0h, 02Bh, 06Fh, 000h, 000h
        mov    byte ptr [esi + 49h], al
        push   ebx
        mov    ebx, dword ptr [esi + 40h]
        mov    eax, dword ptr [ebx + 8]
        mov    byte ptr [esi + 48h], al
        pop    ebx
        push   ecx
        mov    al, 0ffh
        xor    ecx, ecx
        mov    byte ptr [esi + 0bch], al
        mov    byte ptr [esi + 0bdh], al
        mov    byte ptr [esi + 0bah], cl
        mov    byte ptr [esi + 0bbh], cl
        mov    byte ptr [esi + 0c1h], cl
        mov    byte ptr [esi + 0c0h], cl
        mov    word ptr [esi + 4ah], cx
        mov    byte ptr [esi + 56h], cl
        mov    byte ptr [esi + 54h], cl
        mov    byte ptr [esi + 55h], 3ch
        pop    ecx
        db  0A1h, 013h, 06Fh, 000h, 000h
        mov    dword ptr [esi + 4ch], eax
        or     eax, eax
        je     SHORT loc_1_27AD
        push   ebx
        mov    ebx, dword ptr [esi + 40h]
        test   dword ptr [ebx + 4], 2
        jne    SHORT loc_1_27A1
        or     dword ptr [esi + 34h], 40c0h
        jmp    SHORT loc_1_27AC
loc_1_27A1:
        or     dword ptr [esi + 34h], 4000h
        mov    byte ptr [esi + 49h], 1
loc_1_27AC:
        pop    ebx
loc_1_27AD:
        pop    edi
        pop    eax
        ret
sub_1_27B0:
        push   edi
        push   eax
        push   ebx
        mov    dword ptr [esi + 40h], eax
        mov    ebx, eax
        cli
        lea    eax, [esi]
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], eax
        lea    eax, [esi + 10h]
        mov    dword ptr [esi + 10h], eax
        mov    dword ptr [esi + 14h], eax
        lea    eax, [esi + 8]
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi + 0ch], eax
        xor    eax, eax
        mov    dword ptr [esi + 34h], eax
        mov    byte ptr [esi + 0b8h], al
        mov    byte ptr [esi + 38h], 0
        call   sub_1_26A5
        pop    ebx
        push   edi
        lea    edi, [ebx]
        push   esi
        lea    esi, [esi + 1ch]
        call   sub_1_2868
        pop    esi
        pop    edi
        or     byte ptr [esi + 0b8h], 2
        db  0A1h, 0B8h, 06Fh, 000h, 000h
        mov    dword ptr [esi + 18h], eax
        db  089h, 035h, 0B8h, 06Fh, 000h, 000h
        mov    ebx, esi
        pop    eax
        push   esi
        push   ecx
        lea    esi, [ebx + 8ch]
        call   sub_1_2571
        mov    edi, eax
        push   eax
        movzx  eax, word ptr [edi + 92h]
        or     edx, edx
        je     SHORT loc_1_2830
        sub    eax, edx
        sbb    esi, esi
        and    eax, esi
        add    eax, edx
loc_1_2830:
        mov    word ptr [ebx + 7ah], ax
        mov    byte ptr [ebx + 0b6h], cl
        mov    eax, 0ch
        lea    esi, [ebx + 8ch]
        cmp    dword ptr [edi + 28h], 0
        je     SHORT loc_1_2856
        mov    eax, 32h
        lea    esi, [ebx + 8eh]
loc_1_2856:
        mov    byte ptr [ebx + 0b7h], al
        call   sub_1_262B
        pop    eax
        pop    ecx
        pop    esi
        sti
        pop    edi
        clc
        ret
sub_1_2868:
        push   esi
        push   eax
        lea    eax, [edi]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esi + 4], eax
        xchg   esi, eax
        mov    dword ptr [esi], eax
        mov    dword ptr [edi + 4], eax
        pop    eax
        pop    esi
        ret
sub_1_287D:
        push   edi
        push   eax
        lea    eax, [esi]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edi + 4], eax
        xchg   edi, eax
        mov    dword ptr [edi], eax
        mov    dword ptr [esi + 4], eax
        pop    eax
        pop    edi
        ret
sub_1_2892:
        push   ebx
        push   eax
        lea    eax, [esi]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ebx + 4], eax
        xchg   ebx, eax
        mov    dword ptr [ebx], eax
        mov    dword ptr [esi + 4], eax
        pop    eax
        pop    ebx
        ret
sub_1_28A7:
        push   eax
        push   esi
        lea    eax, [edi]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi], eax
        xchg   esi, eax
        mov    dword ptr [edi], eax
        mov    dword ptr [esi + 4], eax
        pop    esi
        pop    eax
        ret
sub_1_28BB:
        push   eax
        push   edi
        lea    eax, [esi]
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi], eax
        xchg   edi, eax
        mov    dword ptr [esi], eax
        mov    dword ptr [edi + 4], eax
        pop    edi
        pop    eax
        ret
sub_1_28CF:
        push   eax
        push   ebx
        lea    eax, [esi]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebx], eax
        xchg   ebx, eax
        mov    dword ptr [esi], eax
        mov    dword ptr [ebx + 4], eax
        pop    ebx
        pop    eax
        ret
sub_1_28E3:
        push   edi
        mov    edi, dword ptr [esi]
        push   edi
        mov    edi, dword ptr [edi]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    esi
        cmp    esi, edi
        pop    edi
        ret
sub_1_28F3:
        push   edi
        mov    edi, dword ptr [ebx]
        push   edi
        mov    edi, dword ptr [edi]
        mov    dword ptr [ebx], edi
        mov    dword ptr [edi + 4], ebx
        pop    ebx
        cmp    ebx, edi
        pop    edi
        ret
sub_1_2903:
        push   esi
        mov    esi, dword ptr [edi]
        push   esi
        mov    esi, dword ptr [esi]
        mov    dword ptr [edi], esi
        mov    dword ptr [esi + 4], edi
        pop    edi
        cmp    edi, esi
        pop    esi
        ret
sub_1_2913:
        mov    byte ptr [edi + 1], al
        push   edi
        push   eax
        db  08Dh, 005h, 0A8h, 06Fh, 000h, 000h
        lea    edi, [edi + 32h]
        mov    dword ptr [edi], eax
        db  0A1h, 0ACh, 06Fh, 000h, 000h
        mov    dword ptr [edi + 4], eax
        xchg   edi, eax
        mov    dword ptr [edi], eax
        db  0A3h, 0ACh, 06Fh, 000h, 000h
        pop    eax
        pop    edi
        db  083h, 00Dh, 0ECh, 06Eh, 000h, 000h, 040h
        ret
        db  0E8h, 02Fh, 0FDh, 0FFh, 0FFh, 0D1h, 0E0h, 0E8h, 011h, 0FDh, 0FFh, 0FFh, 091h, 0E8h, 00Bh, 0FDh
        db  0FFh, 0FFh, 091h, 003h, 005h, 00Bh, 06Fh, 000h, 000h, 003h, 0C1h, 0C3h
sub_1_2959:
        push   eax
        push   ebx
        push   ecx
        push   edx
        push   edi
        mov    edi, eax
        movzx  ecx, word ptr [esi + 0cah]
        test   ecx, ecx
        je     SHORT loc_1_29AB
        mov    eax, 1
        shl    eax, cl
        mov    ecx, eax
        test   ecx, ecx
        jne    SHORT loc_1_2979
        dec    ecx
loc_1_2979:
        movzx  edx, word ptr [edi + 92h]
        imul   eax, edx, 1f40h
        sub    edx, edx
        idiv   ecx
        test   edx, edx
        je     SHORT loc_1_298F
        inc    eax
loc_1_298F:
        shl    eax, 1
        call   sub_1_265A
        mov    ecx, eax
        movzx  eax, byte ptr [edi + 134h]
        imul   ecx, eax
        add    ecx, dword ptr [ebx + 68h]
        mov    dword ptr [ebx + 68h], ecx
        mov    dword ptr [ebx + 70h], ecx
loc_1_29AB:
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        ret
sub_1_29B1:
        push   ebx
        mov    bl, 1fh
loc_1_29B4:
        clc
        shl    eax, 1
        jb     SHORT loc_1_29BD
        dec    bl
        jne    SHORT loc_1_29B4
loc_1_29BD:
        xor    eax, eax
        mov    al, bl
        pop    ebx
        ret
        db  000h
sub_1_29C4:
        cmp    al, byte ptr [eax + eax]
        pop    ebp
        inc    esp
        add    byte ptr [eax], al
        int    44h
        add    byte ptr [eax], al
        sub    dword ptr [edx], eax
        db  000h, 059h, 031h
        add    byte ptr [eax], al
        pop    ecx
        xor    dword ptr [eax], eax
        db  000h, 059h, 031h
        add    byte ptr [eax], al
        pop    ecx
        xor    dword ptr [eax], eax
        db  000h, 0CBh
        dec    esp
        add    byte ptr [eax], al
        db  081h, 04Dh, 000h, 000h, 054h, 045h, 000h
        db  000h, 059h, 031h
        add    byte ptr [eax], al
        pop    ecx
        xor    dword ptr [eax], eax
        db  000h, 07Bh, 044h
        add    byte ptr [eax], al
        db  0C7h, 047h, 000h, 000h, 049h, 043h, 000h
        db  000h, 059h, 031h
        add    byte ptr [eax], al
        pop    ecx
        xor    dword ptr [eax], eax
        db  000h, 059h, 031h
        add    byte ptr [eax], al
        pop    ecx
        xor    dword ptr [eax], eax
        db  000h, 04Dh, 049h
        add    byte ptr [eax], al
        pop    ebx
        dec    ecx
        add    byte ptr [eax], al
        push   ebx
        dec    edx
        add    byte ptr [eax], al
        lodsb
        inc    eax
        add    byte ptr [eax], al
        push   ebp
        inc    ecx
        add    byte ptr [eax], al
        mov    word ptr [ecx], es
        db  000h, 030h
        dec    esp
        add    byte ptr [eax], al
        db  080h, 04Ch, 000h, 000h, 064h
        dec    esp
        add    byte ptr [eax], al
        pop    ecx
        xor    dword ptr [eax], eax
        db  000h, 059h, 031h
        add    byte ptr [eax], al
        cdq
        inc    eax
        add    byte ptr [eax], al
sub_1_2A44:
        add    edi, esi
        sub    edi, ebx
        cmp    word ptr [eax + 76h], di
        ret
loc_1_2A4D:
        ret
sub_1_2A4E:
        mov    dl, 0fh
        test   dword ptr [eax], 2
        je     SHORT loc_1_2A4D
        test   byte ptr [esi + 2], 3
        jne    SHORT loc_1_2A6F
        test   byte ptr [esi + 1], 1
        jne    SHORT loc_1_2A4D
        test   byte ptr [esi + 2], 1
        je     SHORT loc_1_2A4D
        or     dl, 10h
        jmp    SHORT loc_1_2A78
loc_1_2A6F:
        test   byte ptr [esi + 2], 10h
        je     SHORT loc_1_2A78
        or     dl, 10h
loc_1_2A78:
        cli
        db  08Dh, 01Dh, 070h, 06Fh, 000h, 000h
        call   sub_1_28F3
        sti
        je     SHORT loc_1_2ADF
        mov    dword ptr [ebx + 14h], eax
        lea    esi, [ebx + 34h]
        mov    dword ptr [ebx + 24h], esi
        mov    word ptr [ebx + 1eh], 1
        mov    word ptr [ebx + 10h], 2
        push   edx
        call   sub_1_2571
        pop    edx
        add    esi, ecx
        mov    byte ptr [esi], 0f0h
        mov    byte ptr [esi + 1], 0f1h
        mov    byte ptr [esi + 2], dl
        add    ecx, 3
        mov    word ptr [ebx + 22h], cx
        cmp    dword ptr [eax + 28h], 1
        je     SHORT loc_1_2AC2
        mov    word ptr [esi - 2], 300h
loc_1_2AC2:
        cli
        push   esi
        lea    esi, [eax + 0bch]
        call   sub_1_2892
        pop    esi
        sti
        or     dword ptr [eax], 100h
        db  083h, 00Dh, 0ECh, 06Eh, 000h, 000h, 008h
        ret
loc_1_2ADF:
        ret
sub_1_2AE0:
        push   eax
        movzx  eax, word ptr [ebx + 44h]
        shl    eax, 1
        test   byte ptr [ebx + 80h], 40h
        jne    SHORT loc_1_2B10
        test   byte ptr [esi + 1], 1
        je     SHORT loc_1_2B01
        cmp    byte ptr [ebx + 38h], 3
        je     SHORT loc_1_2B42
loc_1_2AFC:
        or     ah, 1
        jmp    SHORT loc_1_2B23
loc_1_2B01:
        test   byte ptr [esi + 3], 1
        je     SHORT loc_1_2B23
        or     byte ptr [ebx + 80h], 80h
        jmp    SHORT loc_1_2B23
loc_1_2B10:
        test   byte ptr [esi + 1], 1
        jne    SHORT loc_1_2AFC
        test   byte ptr [esi + 2], 10h
        je     SHORT loc_1_2B23
        or     byte ptr [ebx + 80h], 80h
loc_1_2B23:
        cli
        mov    dword ptr [ebx + 3ch], 40h
        mov    word ptr [ebx + 7eh], ax
        mov    ax, word ptr [esi + 2]
        mov    word ptr [ebx + 7ch], ax
        mov    byte ptr [ebx + 38h], 3
        sti
        push   ebx
        call   sub_1_4E6C
loc_1_2B42:
        pop    eax
        ret
sub_1_2B44:
        mov    edi, edx
        call   sub_1_236B
        jb     SHORT loc_1_2B83
        mov    cl, byte ptr [esi + 2]
        shr    cl, 1
        cmp    cl, byte ptr [ebx + 45h]
        jne    SHORT loc_1_2B8A
        and    dword ptr [ebx + 34h], 0fffffffbh
        mov    al, byte ptr [esi + 3]
        call   sub_1_2CBF
        jb     SHORT loc_1_2B88
        cli
        inc    byte ptr [ebx + 45h]
        and    byte ptr [ebx + 45h], 7fh
        mov    cl, byte ptr [ebx + 47h]
        add    cl, byte ptr [ebx + 49h]
        and    cl, 7fh
        cmp    cl, byte ptr [ebx + 45h]
        je     SHORT loc_1_2BAD
        sti
loc_1_2B7C:
        mov    eax, dword ptr [ebx + 74h]
        mov    dword ptr [ebx + 60h], eax
        ret
loc_1_2B83:
        call   sub_1_2A4E
loc_1_2B88:
        sti
        ret
loc_1_2B8A:
        test   dword ptr [ebx + 34h], 4
        jne    SHORT loc_1_2B9B
        or     dword ptr [ebx + 3ch], 20h
        or     dword ptr [ebx + 34h], 4
loc_1_2B9B:
        test   byte ptr [esi + 3], 1
        je     SHORT loc_1_2BA5
        or     dword ptr [ebx + 3ch], 10h
loc_1_2BA5:
        sti
        push   ebx
        call   sub_1_4E6C
        ret
loc_1_2BAD:
        or     dword ptr [ebx + 3ch], 80h
        sti
        push   ebx
        call   sub_1_4E6C
        jmp    SHORT loc_1_2B7C
sub_1_2BBD:
        push   ecx
        cmp    bl, 3
        je     SHORT loc_1_2BE0
        cmp    dword ptr [eax + 28h], 1
        je     SHORT loc_1_2BE3
        test   byte ptr [edx], 1
        je     SHORT loc_1_2BE0
        mov    ecx, dword ptr [eax + 31h]
        cmp    ecx, dword ptr [edx + 6]
        jne    SHORT loc_1_2BE0
        mov    cx, word ptr [eax + 35h]
        cmp    cx, word ptr [edx + 0ah]
        je     SHORT loc_1_2BFE
loc_1_2BE0:
        clc
        pop    ecx
        ret
loc_1_2BE3:
        test   byte ptr [edx + 2], 80h
        je     SHORT loc_1_2BE0
        mov    ecx, dword ptr [edx + 8]
        and    cl, 7fh
        cmp    ecx, dword ptr [eax + 31h]
        jne    SHORT loc_1_2BE0
        mov    cx, word ptr [eax + 35h]
        cmp    cx, word ptr [edx + 0ch]
        jne    SHORT loc_1_2BE0
loc_1_2BFE:
        stc
        pop    ecx
        ret
sub_1_2C01:
        mov    word ptr [esi + 22h], cx
        push   ebx
        mov    ebx, dword ptr [esi + 14h]
        cli
        push   edi
        lea    edi, [ebx + 0bch]
        call   sub_1_2868
        pop    edi
        sti
        or     dword ptr [ebx], 100h
        db  083h, 00Dh, 0ECh, 06Eh, 000h, 000h, 008h
        pop    ebx
        ret
sub_1_2C27:
        push   edx
        push   edi
        push   esi
        lea    eax, [ebx + 8]
        mov    esi, dword ptr [ebx + 10h]
        mov    dword ptr [esi + 4], eax
        mov    edx, esi
        mov    esi, dword ptr [ebx + 8]
        mov    edi, dword ptr [ebx + 14h]
        mov    dword ptr [edi], esi
        cmp    esi, eax
        je     SHORT loc_1_2C46
        mov    dword ptr [esi + 4], edi
        jmp    SHORT loc_1_2C49
loc_1_2C46:
        mov    dword ptr [ebx + 0ch], edi
loc_1_2C49:
        mov    dword ptr [ebx + 8], edx
        lea    eax, [ebx + 10h]
        mov    dword ptr [ebx + 10h], eax
        mov    dword ptr [ebx + 14h], eax
        movzx  eax, byte ptr [ebx + 44h]
        sub    al, byte ptr [ebx + 46h]
        and    al, 7fh
        mov    edi, dword ptr [ebx + 40h]
        add    dword ptr [edi + 0f8h], eax
        or     al, al
        je     SHORT loc_1_2C84
        add    byte ptr [ebx + 0bbh], al
        inc    byte ptr [ebx + 39h]
        db  0A0h, 020h, 06Fh, 000h, 000h
        cmp    al, byte ptr [ebx + 39h]
        jb     SHORT loc_1_2C88
        mov    al, byte ptr [ebx + 46h]
        mov    byte ptr [ebx + 44h], al
loc_1_2C84:
        pop    esi
        pop    edi
        pop    edx
        ret
loc_1_2C88:
        or     dword ptr [ebx + 3ch], 2
        mov    byte ptr [ebx + 38h], 2
        jmp    SHORT loc_1_2C84
loc_1_2C92:
        cmp    byte ptr [ebx + 38h], 3
        je     NEAR PTR loc_1_2D7A
        test   dword ptr [ebx + 34h], 1000h
        je     SHORT loc_1_2CB7
        and    dword ptr [ebx + 34h], 0ffffefffh
        mov    byte ptr [ebx + 38h], 4
        call   sub_1_6104
        jmp    SHORT loc_1_2CC5
loc_1_2CB7:
        push   ebx
        call   sub_1_4E6C
        stc
        ret
sub_1_2CBF:
        cmp    byte ptr [ebx + 38h], 4
        jne    SHORT loc_1_2C92
loc_1_2CC5:
        shr    al, 1
        pushfd
        cmp    al, byte ptr [ebx + 46h]
        je     SHORT loc_1_2CFD
        push   ecx
        cli
        mov    cl, byte ptr [ebx + 81h]
        mov    ah, byte ptr [ebx + 46h]
        sub    cl, ah
        mov    ch, al
        sub    ch, ah
        and    cx, 7f7fh
        cmp    ch, cl
        jbe    SHORT loc_1_2CF2
        cmp    ch, 7fh
        jne    NEAR PTR loc_1_2D88
        mov    al, ah
loc_1_2CF2:
        sti
        pop    ecx
        mov    byte ptr [ebx + 39h], 0
        call   sub_1_617A
loc_1_2CFD:
        popfd
        jb     NEAR PTR loc_1_2D9A
loc_1_2D04:
        test   dword ptr [ebx + 34h], 100h
        jne    NEAR PTR loc_1_2E13
loc_1_2D11:
        mov    al, byte ptr [ebx + 46h]
        cmp    al, byte ptr [ebx + 44h]
        jne    SHORT loc_1_2D3C
        test   dword ptr [ebx + 34h], 8
        jne    SHORT loc_1_2D3C
        mov    dword ptr [ebx + 5ch], 0ffffffffh
        db  0A0h, 020h, 06Fh, 000h, 000h
        mov    byte ptr [ebx + 0b9h], al
        mov    eax, dword ptr [ebx + 68h]
        mov    dword ptr [ebx + 70h], eax
        clc
        ret
loc_1_2D3C:
        cli
        test   byte ptr [ebx + 0b8h], 1
        jne    SHORT loc_1_2D78
        or     byte ptr [ebx + 0b8h], 1
        mov    edx, dword ptr [ebx + 40h]
        push   ebx
        push   eax
        lea    eax, [edx + 0c4h]
        lea    ebx, [ebx + 24h]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [edx + 0c8h]
        mov    dword ptr [ebx + 4], eax
        xchg   ebx, eax
        mov    dword ptr [ebx], eax
        mov    dword ptr [edx + 0c8h], eax
        pop    eax
        pop    ebx
        db  083h, 00Dh, 0ECh, 06Eh, 000h, 000h, 008h
loc_1_2D78:
        clc
        ret
loc_1_2D7A:
        mov    byte ptr [ebx + 80h], 1
        call   sub_1_2AE0
        stc
        ret
loc_1_2D88:
        pop    ecx
        add    esp, 4
        mov    byte ptr [ebx + 80h], 8
        call   sub_1_2AE0
        stc
        ret
loc_1_2D9A:
        test   byte ptr [esi + 1], 1
        jne    SHORT loc_1_2DEE
        cli
        or     dword ptr [ebx + 3ch], 10h
        test   byte ptr [ebx + 0b8h], 1
        jne    NEAR PTR loc_1_2D11
        or     byte ptr [ebx + 0b8h], 1
        mov    edx, dword ptr [ebx + 40h]
        push   ebx
        push   eax
        lea    eax, [edx + 0c4h]
        lea    ebx, [ebx + 24h]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [edx + 0c8h]
        mov    dword ptr [ebx + 4], eax
        xchg   ebx, eax
        mov    dword ptr [ebx], eax
        mov    dword ptr [edx + 0c8h], eax
        pop    eax
        pop    ebx
        db  083h, 00Dh, 0ECh, 06Eh, 000h, 000h, 008h
        jmp    NEAR PTR loc_1_2D11
loc_1_2DE9:
        jmp    NEAR PTR loc_1_2D04
loc_1_2DEE:
        test   dword ptr [ebx + 34h], 8
        je     SHORT loc_1_2DE9
        and    dword ptr [ebx + 34h], 0ffffffd7h
        mov    dword ptr [ebx + 5ch], 0ffffffffh
        db  0A0h, 020h, 06Fh, 000h, 000h
        mov    byte ptr [ebx + 0b9h], al
        mov    eax, dword ptr [ebx + 68h]
        mov    dword ptr [ebx + 70h], eax
loc_1_2E13:
        cli
        lea    eax, [ebx + 10h]
        cmp    dword ptr [ebx + 10h], eax
        je     SHORT loc_1_2E21
        call   sub_1_2C27
loc_1_2E21:
        test   byte ptr [ebx + 0b8h], 1
        jne    SHORT loc_1_2E5C
        or     byte ptr [ebx + 0b8h], 1
        mov    edx, dword ptr [ebx + 40h]
        push   ebx
        push   eax
        lea    eax, [edx + 0c4h]
        lea    ebx, [ebx + 24h]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [edx + 0c8h]
        mov    dword ptr [ebx + 4], eax
        xchg   ebx, eax
        mov    dword ptr [ebx], eax
        mov    dword ptr [edx + 0c8h], eax
        pop    eax
        pop    ebx
        db  083h, 00Dh, 0ECh, 06Eh, 000h, 000h, 008h
loc_1_2E5C:
        clc
        ret
        db  08Bh, 0FBh, 0E8h, 006h, 0F5h, 0FFh, 0FFh, 072h, 012h, 080h, 07Bh, 038h, 004h, 075h, 00Ch, 0C6h
        db  083h, 080h, 000h, 000h, 000h, 001h, 0E8h, 067h, 0FCh, 0FFh, 0FFh, 0C3h
loc_1_2E7A:
        mov    eax, 10001h
        ret
loc_1_2E80:
        mov    edi, esi
        sub    edi, ebx
        add    edi, 12h
        cmp    word ptr [eax + 76h], di
        jb     SHORT loc_1_2E7A
        mov    edx, ebx
        lea    edi, [esi + 4]
        movzx  ecx, byte ptr [edi + 0ch]
        dec    ecx
        js     NEAR PTR loc_1_2F79
        db  08Bh, 00Ch, 08Dh, 0C0h, 077h, 000h, 000h
        test   ecx, ecx
        je     NEAR PTR loc_1_2F79
        mov    ebx, dword ptr [ecx + 38h]
        test   ebx, ebx
        je     NEAR PTR loc_1_2FDE
loc_1_2EB7:
        mov    al, byte ptr [esi + 3]
        call   sub_1_2CBF
        mov    eax, ecx
        jb     SHORT loc_1_2F25
        cli
        mov    cl, byte ptr [esi + 2]
        shr    cl, 1
        cmp    cl, byte ptr [ebx + 45h]
        jne    NEAR PTR loc_1_2F81
        and    dword ptr [ebx + 34h], 0fffffffbh
        inc    byte ptr [ebx + 45h]
        and    byte ptr [ebx + 45h], 7fh
        test   dword ptr [ebx + 3ch], 10h
        jne    SHORT loc_1_2F29
        sti
        mov    cl, byte ptr [ebx + 47h]
        add    cl, byte ptr [ebx + 49h]
        and    cl, 7fh
        cmp    cl, byte ptr [ebx + 45h]
        je     NEAR PTR loc_1_2FFB
loc_1_2EF9:
        mov    ecx, dword ptr [ebx + 74h]
        mov    dword ptr [ebx + 60h], ecx
loc_1_2EFF:
        test   word ptr [eax + 40h], 40h
        jne    SHORT loc_1_2F25
        cmp    word ptr [edi + 2], 0efffh
        jne    SHORT loc_1_2F25
        movzx  ebx, byte ptr [edi + 4]
        cmp    bl, 20h
        jae    SHORT loc_1_2F25
        db  0FFh, 014h, 09Dh, 0C4h, 029h, 000h, 000h
loc_1_2F1F:
        mov    eax, 0
        ret
loc_1_2F25:
        sti
        xor    eax, eax
        ret
loc_1_2F29:
        push   eax
        cli
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, 100000h
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
        db  0FEh, 005h, 0F6h, 06Eh, 000h, 000h
        pop    ebx
        jne    SHORT loc_1_2F57
        push   edi
        push   edx
        push   esi
        mov    edi, ebx
        call   sub_1_5121
        sbb    eax, eax
        mov    ebx, edi
        pop    esi
        pop    edx
        pop    edi
loc_1_2F57:
        db  0FEh, 00Dh, 0F6h, 06Eh, 000h, 000h
        push   eax
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, 0fff00000h
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
        pop    ebx
        pop    eax
        test   eax, eax
        pop    eax
        jne    SHORT loc_1_2EF9
        jmp    SHORT loc_1_2EFF
loc_1_2F79:
        call   sub_1_2B44
        xor    eax, eax
        ret
loc_1_2F81:
        test   dword ptr [ebx + 34h], 4
        jne    SHORT loc_1_2F1F
        cli
        or     dword ptr [ebx + 34h], 4
        or     dword ptr [ebx + 3ch], 20h
        mov    edi, ebx
        cli
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, 100000h
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
        db  0FEh, 005h, 0F6h, 06Eh, 000h, 000h
        pop    ebx
        jne    SHORT loc_1_2FB8
        call   sub_1_535E
        jae    SHORT loc_1_2FBE
loc_1_2FB8:
        push   edi
        call   sub_1_4E6C
loc_1_2FBE:
        sti
        db  0FEh, 00Dh, 0F6h, 06Eh, 000h, 000h
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, 0fff00000h
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
        pop    ebx
        mov    eax, 0
        ret
loc_1_2FDE:
        mov    ebx, ecx
        call   sub_1_62FA
        jb     SHORT loc_1_2FEE
        mov    ebx, eax
        jmp    NEAR PTR loc_1_2EB7
loc_1_2FEE:
        mov    edi, edx
        call   sub_1_2A4E
        mov    eax, 0
        ret
loc_1_2FFB:
        or     dword ptr [ebx + 3ch], 80h
        or     dword ptr [ebx + 34h], 8000h
        push   eax
        push   ebx
        call   sub_1_4E6C
        pop    eax
        jmp    NEAR PTR loc_1_2EFF
loc_1_3016:
        mov    edi, 4
        call   sub_1_2A44
        jb     SHORT loc_1_304B
        push   eax
        mov    edi, ebx
        call   sub_1_236B
        jb     SHORT loc_1_304C
        pop    edi
        mov    al, byte ptr [esi + 3]
        call   sub_1_2CBF
        jb     SHORT loc_1_3045
        and    dword ptr [ebx + 34h], 0fffffffdh
        cli
        test   dword ptr [ebx + 3ch], 10h
        jne    SHORT loc_1_3053
loc_1_3045:
        push   ebx
        call   sub_1_4E6C
loc_1_304B:
        ret
loc_1_304C:
        pop    eax
        call   sub_1_2A4E
        ret
loc_1_3053:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, 100000h
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
        db  0FEh, 005h, 0F6h, 06Eh, 000h, 000h
        pop    ebx
        jne    SHORT loc_1_3077
        mov    edi, ebx
        call   sub_1_5121
        mov    ebx, edi
loc_1_3077:
        db  0FEh, 00Dh, 0F6h, 06Eh, 000h, 000h
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, 0fff00000h
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
        pop    ebx
        sti
        jmp    SHORT loc_1_3045
loc_1_3093:
        mov    edi, 4
        call   sub_1_2A44
        jb     SHORT loc_1_30DC
        push   eax
        mov    edi, ebx
        call   sub_1_236B
        jb     SHORT loc_1_30DD
        pop    edi
        mov    al, byte ptr [esi + 3]
        cli
        mov    cl, al
        shr    cl, 1
        add    cl, 81h
        sub    cl, byte ptr [ebx + 46h]
        and    cl, 7fh
        cmp    byte ptr [ebx + 48h], cl
        jbe    SHORT loc_1_30D6
        sti
        or     dword ptr [ebx + 34h], 100h
        call   sub_1_2CBF
        jb     SHORT loc_1_30D6
        and    dword ptr [ebx + 34h], 0fffffefdh
loc_1_30D6:
        push   ebx
        call   sub_1_4E6C
loc_1_30DC:
        ret
loc_1_30DD:
        pop    eax
        call   sub_1_2A4E
        ret
loc_1_30E4:
        mov    edi, 4
        call   sub_1_2A44
        jb     SHORT loc_1_3107
        mov    edi, ebx
        call   sub_1_236B
        jb     SHORT loc_1_310E
        mov    al, byte ptr [esi + 3]
        call   sub_1_2CBF
        jb     SHORT loc_1_3107
        or     dword ptr [ebx + 34h], 2
loc_1_3107:
        push   ebx
        call   sub_1_4E6C
        ret
loc_1_310E:
        call   sub_1_2A4E
        ret
loc_1_3114:
        mov    edi, 2fh
        call   sub_1_2A44
        jb     SHORT loc_1_3158
        test   ecx, ecx
        jne    SHORT loc_1_3158
        test   byte ptr [esi + 2], 10h
        jne    SHORT loc_1_3158
        mov    edx, ebx
        lea    edi, [esi + 3]
        cmp    word ptr [edi + 2], 0efffh
        jne    SHORT loc_1_3158
        movzx  ebx, byte ptr [edi + 4]
        cmp    bl, 20h
        jae    SHORT loc_1_3158
        cmp    bl, 7
        ja     SHORT loc_1_314C
        call   sub_1_2BBD
        jb     SHORT loc_1_3158
loc_1_314C:
        cmp    bl, 14h
        jae    SHORT loc_1_3158
        db  0FFh, 014h, 09Dh, 0C4h, 029h, 000h, 000h
loc_1_3158:
        ret
loc_1_3159:
        ret
loc_1_315A:
        mov    edx, eax
        mov    edi, ebx
        call   sub_1_236B
        jae    NEAR PTR loc_1_3225
        cli
        push   eax
        mov    eax, 0c8h
        call   sub_1_2233
        mov    esi, eax
        pop    eax
        test   esi, esi
        je     SHORT loc_1_31B9
        mov    edx, eax
        mov    dword ptr [esi + 40h], eax
        push   edx
        call   sub_1_27B0
        pop    edx
        jb     SHORT loc_1_31B9
        mov    dword ptr [ebx + 40h], edx
        call   sub_1_236B
        jb     SHORT loc_1_31BF
        mov    dword ptr [ebx + 40h], edx
loc_1_3197:
        mov    byte ptr [ebx + 38h], 1
loc_1_319B:
        or     dword ptr [ebx + 3ch], 4
        or     dword ptr [ebx + 34h], 1000h
        push   ebx
        call   sub_1_4E6C
        mov    eax, dword ptr [ebx + 6ch]
        mov    dword ptr [ebx + 58h], eax
        mov    eax, dword ptr [ebx + 4ch]
        mov    dword ptr [ebx + 64h], eax
        ret
loc_1_31B9:
        call   sub_1_2A4E
        ret
loc_1_31BF:
        call   sub_1_2A4E
        ret
loc_1_31C5:
        cmp    byte ptr [ebx + 38h], 1
        je     SHORT loc_1_319B
        cli
        test   byte ptr [ebx + 0b8h], 4
        je     SHORT loc_1_31ED
        push   esi
        push   edi
        lea    esi, [ebx + 2ch]
        mov    edi, dword ptr [esi]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        and    byte ptr [ebx + 0b8h], 0fbh
loc_1_31ED:
        sti
        call   sub_1_6255
loc_1_31F3:
        cli
        lea    esi, [ebx]
        call   sub_1_28E3
        je     SHORT loc_1_320F
        and    byte ptr [esi + 24h], 0fdh
        sti
        or     word ptr [esi + 40h], 40h
        push   esi
        call   sub_1_5BF3
        jmp    SHORT loc_1_31F3
loc_1_320F:
        sti
        mov    dword ptr [ebx + 34h], 0
        xchg   ebx, esi
        call   sub_1_26A5
        xchg   ebx, esi
        jmp    NEAR PTR loc_1_3197
loc_1_3225:
        cmp    byte ptr [ebx + 38h], 4
        jne    SHORT loc_1_31C5
        call   sub_1_6255
loc_1_3230:
        cli
        lea    esi, [ebx]
        call   sub_1_28E3
        je     SHORT loc_1_324C
        and    byte ptr [esi + 24h], 0fdh
        sti
        or     word ptr [esi + 40h], 40h
        push   esi
        call   sub_1_5BF3
        jmp    SHORT loc_1_3230
loc_1_324C:
        sti
        mov    dword ptr [ebx + 34h], 0
        mov    esi, ebx
        call   sub_1_26A5
        jmp    NEAR PTR loc_1_3197
loc_1_3260:
        mov    edi, ebx
        call   sub_1_236B
        jb     SHORT loc_1_32AD
        mov    dword ptr [ebx + 58h], 0ffffffffh
        mov    dword ptr [ebx + 5ch], 0ffffffffh
        mov    dword ptr [ebx + 60h], 0ffffffffh
        mov    dword ptr [ebx + 64h], 0ffffffffh
        db  0A0h, 020h, 06Fh, 000h, 000h
        mov    byte ptr [ebx + 0b9h], al
        mov    eax, dword ptr [ebx + 68h]
        mov    dword ptr [ebx + 70h], eax
        mov    byte ptr [ebx + 38h], 2
        call   sub_1_6255
        mov    dword ptr [ebx + 3ch], 4
        push   ebx
        call   sub_1_4E6C
        ret
loc_1_32AD:
        call   sub_1_2A4E
        ret
loc_1_32B3:
        mov    edi, ebx
        call   sub_1_236B
        jb     SHORT loc_1_32E8
        jecxz  loc_1_32E9
        mov    dword ptr [ebx + 5ch], 0ffffffffh
        db  0A0h, 020h, 06Fh, 000h, 000h
        mov    byte ptr [ebx + 0b9h], al
        mov    eax, dword ptr [ebx + 68h]
        mov    dword ptr [ebx + 70h], eax
        cmp    byte ptr [ebx + 38h], 1
        je     SHORT loc_1_32FC
        cmp    byte ptr [ebx + 38h], 2
        jne    SHORT loc_1_32E9
        push   ebx
        call   sub_1_6435
loc_1_32E8:
        ret
loc_1_32E9:
        cmp    byte ptr [ebx + 38h], 4
        jne    SHORT loc_1_32E8
        mov    byte ptr [ebx + 80h], 41h
        call   sub_1_2AE0
        ret
loc_1_32FC:
        mov    dword ptr [ebx + 3ch], 1
        test   dword ptr [ebx + 34h], 1000h
        jne    SHORT loc_1_3310
        or     dword ptr [ebx + 34h], 20h
loc_1_3310:
        push   ebx
        call   sub_1_4E6C
        mov    eax, dword ptr [ebx + 6ch]
        mov    dword ptr [ebx + 58h], eax
        mov    eax, dword ptr [ebx + 4ch]
        mov    dword ptr [ebx + 64h], eax
        ret
loc_1_3323:
        mov    edi, ebx
        call   sub_1_236B
        jb     SHORT loc_1_3340
        cmp    byte ptr [ebx + 38h], 0
        je     SHORT loc_1_333F
        mov    dword ptr [ebx + 5ch], 0ffffffffh
        push   ebx
        call   sub_1_6435
loc_1_333F:
        ret
loc_1_3340:
        ret
loc_1_3341:
        mov    edi, 8
        call   sub_1_2A44
        jne    SHORT loc_1_338A
        inc    dword ptr [eax + 0d0h]
        mov    edi, ebx
        call   sub_1_236B
        jb     SHORT loc_1_338A
        jecxz  loc_1_3378
        cmp    byte ptr [ebx + 38h], 4
        je     SHORT loc_1_336A
        cmp    byte ptr [ebx + 38h], 3
        jne    SHORT loc_1_338A
loc_1_336A:
        mov    dword ptr [ebx + 3ch], 2
        push   ebx
        call   sub_1_4E6C
        ret
loc_1_3378:
        cmp    byte ptr [ebx + 38h], 4
        jne    SHORT loc_1_338A
        mov    byte ptr [ebx + 80h], 41h
        call   sub_1_2AE0
loc_1_338A:
        ret
loc_1_338B:
        ret
sub_1_338C:
        mov    edi, 6
        call   sub_1_2A44
        jne    SHORT loc_1_338B
        or     ecx, ecx
        jne    SHORT loc_1_338B
        mov    edi, ebx
        db  08Ah, 035h, 02Bh, 06Fh, 000h, 000h
        mov    cx, word ptr [esi]
        xchg   cl, ch
        or     ch, ch
        jne    SHORT loc_1_3400
loc_1_33AD:
        mov    dl, byte ptr [esi + 2]
        cli
        db  08Dh, 01Dh, 070h, 06Fh, 000h, 000h
        call   sub_1_28F3
        sti
        je     SHORT loc_1_33FF
        mov    dword ptr [ebx + 14h], eax
        lea    esi, [ebx + 34h]
        mov    dword ptr [ebx + 24h], esi
        mov    word ptr [ebx + 10h], 2
        mov    word ptr [ebx + 1eh], 1
        push   edx
        push   ecx
        call   sub_1_2571
        add    esi, ecx
        pop    edx
        mov    word ptr [esi], dx
        pop    edx
        mov    byte ptr [esi + 2], dl
        lea    esi, [esi + 3]
        mov    byte ptr [esi], 81h
        mov    byte ptr [esi + 1], 3
        and    dh, 7fh
        mov    byte ptr [esi + 2], dh
        add    ecx, 6
        mov    esi, ebx
        call   sub_1_2C01
loc_1_33FF:
        ret
loc_1_3400:
        cmp    ch, 0f0h
        jne    SHORT loc_1_341F
        call   sub_1_236B
        jb     SHORT loc_1_33AD
        cmp    byte ptr [ebx + 38h], 3
        je     SHORT loc_1_33FF
        mov    dh, byte ptr [esi + 5]
        shr    dh, 1
        mov    byte ptr [ebx + 48h], dh
        mov    dh, byte ptr [ebx + 49h]
loc_1_341D:
        jmp    SHORT loc_1_33AD
loc_1_341F:
        cmp    ch, 0ffh
        jne    SHORT loc_1_33FF
        mov    ch, 0f0h
        jmp    SHORT loc_1_341D
sub_1_3428:
        jecxz  loc_1_342C
loc_1_342A:
        sti
        ret
loc_1_342C:
        mov    edi, ebx
        mov    bl, 0e3h
        test   byte ptr [esi + 2], 10h
        je     SHORT loc_1_3439
        or     bl, 10h
loc_1_3439:
        cli
        db  0F7h, 005h, 0ECh, 06Eh, 000h, 000h, 000h, 010h, 000h, 000h
        jne    SHORT loc_1_342A
        db  08Dh, 035h, 070h, 06Fh, 000h, 000h
        call   sub_1_28E3
        je     SHORT loc_1_342A
        db  081h, 00Dh, 0ECh, 06Eh, 000h, 000h, 000h, 010h, 000h, 000h
        sti
        mov    dword ptr [esi + 14h], eax
        mov    word ptr [esi + 10h], 1002h
        mov    word ptr [esi + 1eh], 1
        db  0C7h, 046h, 024h, 0D4h, 07Bh, 000h, 000h
        push   esi
        mov    esi, dword ptr [esi + 24h]
        call   sub_1_2571
        add    esi, ecx
        mov    byte ptr [esi], 0f0h
        mov    byte ptr [esi + 1], 0f1h
        mov    byte ptr [esi + 2], bl
        add    ecx, 3
        pop    edi
        push   esi
        push   edi
        lea    edx, [esi + 3]
        mov    ebx, ecx
        mov    edi, dword ptr [edi + 14h]
        push   ecx
        neg    ecx
        add    ecx, 258h
        call   sub_1_5F9D
        jae    SHORT loc_1_34A9
        sub    ecx, ecx
loc_1_34A9:
        mov    ebx, ecx
        pop    ecx
        add    ecx, ebx
        pop    esi
        or     ebx, ebx
        jne    SHORT loc_1_34C8
        mov    edi, dword ptr [esi + 14h]
        cmp    dword ptr [edi + 28h], 1
        je     SHORT loc_1_34C8
        pop    edi
        add    ebx, 3
        xchg   bl, bh
        mov    word ptr [edi - 2], bx
        jmp    SHORT loc_1_34C9
loc_1_34C8:
        pop    edi
loc_1_34C9:
        call   sub_1_2C01
        ret
        db  000h
sub_1_34D0:
        pushfd
        cli
        test   word ptr [ebx + 40h], 80h
        je     SHORT loc_1_3529
        push   eax
        test   dword ptr [ecx + 4], 2
        je     SHORT loc_1_34FE
        mov    eax, 7d0h
        test   dword ptr [ecx + 4], 1
        jne    SHORT loc_1_34F7
        mov    eax, 64h
loc_1_34F7:
        call   sub_1_265A
        jmp    SHORT loc_1_351C
loc_1_34FE:
        push   ebx
        mov    ebx, ecx
        call   sub_1_2671
        shl    eax, 1
        pop    ebx
        call   sub_1_265A
        xchg   ecx, eax
        call   sub_1_265A
        add    eax, ecx
        db  003h, 005h, 01Bh, 06Fh, 000h, 000h
loc_1_351C:
        mov    ecx, dword ptr [ebx + 88h]
        mov    dword ptr [ecx + 32h], eax
        pop    eax
        mov    dword ptr [ecx + 3ah], eax
loc_1_3529:
        popfd
        ret
sub_1_352B:
        db  08Bh, 01Dh, 0B8h, 06Fh, 000h, 000h
loc_1_3531:
        test   ebx, ebx
        je     SHORT loc_1_3556
        cli
        cmp    byte ptr [ebx + 38h], 0
        je     SHORT loc_1_3550
        dec    dword ptr [ebx + 5ch]
        je     SHORT loc_1_3557
loc_1_3541:
        dec    dword ptr [ebx + 60h]
        je     SHORT loc_1_357E
loc_1_3546:
        dec    dword ptr [ebx + 58h]
        je     SHORT loc_1_3585
loc_1_354B:
        dec    dword ptr [ebx + 64h]
        je     SHORT loc_1_358C
loc_1_3550:
        sti
        mov    ebx, dword ptr [ebx + 18h]
        jmp    SHORT loc_1_3531
loc_1_3556:
        ret
loc_1_3557:
        cmp    byte ptr [ebx + 38h], 4
        jne    SHORT loc_1_3564
        call   sub_1_1E97
        jmp    SHORT loc_1_3541
loc_1_3564:
        cmp    byte ptr [ebx + 38h], 1
        jne    SHORT loc_1_3571
        call   sub_1_1E26
        jmp    SHORT loc_1_3541
loc_1_3571:
        cmp    byte ptr [ebx + 38h], 2
        jne    SHORT loc_1_3541
        call   sub_1_1E67
        jmp    SHORT loc_1_3541
loc_1_357E:
        call   sub_1_1D9C
        jmp    SHORT loc_1_3546
loc_1_3585:
        call   sub_1_1E09
        jmp    SHORT loc_1_354B
loc_1_358C:
        call   sub_1_1DD1
        jmp    SHORT loc_1_3550
sub_1_3593:
        db  0FFh, 00Dh, 0C4h, 06Eh, 000h, 000h
        db  0A1h, 0C4h, 06Eh, 000h, 000h
        push   eax
        test   eax, eax
        jne    SHORT loc_1_35AD
        db  0A1h, 0C8h, 06Eh, 000h, 000h
        db  0A3h, 0C4h, 06Eh, 000h, 000h
loc_1_35AD:
        db  08Bh, 01Dh, 0BCh, 06Fh, 000h, 000h
loc_1_35B3:
        test   ebx, ebx
        je     NEAR PTR loc_1_3652
        cli
        test   byte ptr [ebx + 40h], 80h
        je     SHORT loc_1_35D8
        cmp    byte ptr [ebx + 25h], 2
        je     SHORT loc_1_35D8
        mov    edi, dword ptr [ebx + 88h]
        dec    dword ptr [edi + 32h]
        jne    SHORT loc_1_35D8
        push   ebx
        call   dword ptr [edi + 3ah]
        pop    ebx
loc_1_35D8:
        sti
        cmp    dword ptr [esp], 0
        jne    SHORT loc_1_3649
        cli
        cmp    byte ptr [ebx + 25h], 5
        jne    SHORT loc_1_3649
        cmp    byte ptr [ebx + 8fh], 0
        je     SHORT loc_1_3612
        lea    eax, [ebx + 30h]
        mov    edx, dword ptr [eax]
loc_1_35F4:
        cmp    eax, edx
        je     SHORT loc_1_3612
        dec    byte ptr [edx + 0ch]
        jne    SHORT loc_1_360E
        mov    ecx, dword ptr [edx]
        pushad
        lea    ebx, [edx - 32h]
        call   sub_1_1A40
        popad
        cli
        mov    edx, ecx
        jmp    SHORT loc_1_35F4
loc_1_360E:
        mov    edx, dword ptr [edx]
        jmp    SHORT loc_1_35F4
loc_1_3612:
        cmp    byte ptr [ebx + 8eh], 0
        je     SHORT loc_1_3649
        lea    eax, [ebx + 28h]
        mov    edx, dword ptr [eax]
loc_1_3620:
        cmp    eax, edx
        je     SHORT loc_1_3649
        mov    cl, byte ptr [edx - 32h]
        and    cl, 7fh
        cmp    cl, 15h
        jne    SHORT loc_1_3645
        dec    byte ptr [edx + 0ch]
        jne    SHORT loc_1_3645
        mov    ecx, dword ptr [edx]
        pushad
        lea    ebx, [edx - 32h]
        call   sub_1_1BE8
        popad
        cli
        mov    edx, ecx
        jmp    SHORT loc_1_3620
loc_1_3645:
        mov    edx, dword ptr [edx]
        jmp    SHORT loc_1_3620
loc_1_3649:
        sti
        mov    ebx, dword ptr [ebx + 20h]
        jmp    NEAR PTR loc_1_35B3
loc_1_3652:
        pop    eax
        ret
loc_1_3654:
        db  0FFh, 005h, 0D4h, 06Eh, 000h, 000h
        db  00Fh, 0BAh, 02Dh, 0ECh, 06Eh, 000h, 000h, 001h
        jb     SHORT loc_1_3698
        db  0CDh, 020h, 0F1h, 000h, 001h, 000h
        jb     SHORT loc_1_367E
        mov    eax, dword ptr [ebx + 8]
        test   byte ptr [eax + 2dh], 2
        je     SHORT loc_1_367E
        pushfd
        call   sub_1_36AF
        popfd
        jmp    SHORT loc_1_3698
loc_1_367E:
        sub    eax, eax
        sub    ebx, ebx
        db  0BEh, 0AFh, 036h, 000h, 000h
        mov    ecx, 801h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        db  089h, 035h, 03Ch, 06Fh, 000h, 000h
loc_1_3698:
        db  0A1h, 0E8h, 06Dh, 000h, 000h
        db  0BEh, 054h, 036h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 038h, 06Fh, 000h, 000h
        ret
sub_1_36AF:
        sub    eax, eax
        db  080h, 025h, 0ECh, 06Eh, 000h, 000h, 0FDh
        db  0A3h, 03Ch, 06Fh, 000h, 000h
        db  039h, 005h, 0D4h, 06Eh, 000h, 000h
        je     SHORT loc_1_372E
        db  0FEh, 005h, 0F6h, 06Eh, 000h, 000h
loc_1_36CB:
        db  0FFh, 00Dh, 0D4h, 06Eh, 000h, 000h
        sub    eax, eax
loc_1_36D3:
        cmp    eax, 8
        je     SHORT loc_1_36F3
        db  08Bh, 034h, 085h, 040h, 06Fh, 000h, 000h
        test   esi, esi
        je     SHORT loc_1_36F0
        btr    dword ptr [esi], 4
        jae    SHORT loc_1_36F0
        db  083h, 00Dh, 0ECh, 06Eh, 000h, 000h, 008h
loc_1_36F0:
        inc    eax
        jmp    SHORT loc_1_36D3
loc_1_36F3:
        call   sub_1_352B
        call   sub_1_3593
        call   sub_1_5638
        sti
        db  0FFh, 00Dh, 0CCh, 06Eh, 000h, 000h
        jne    SHORT loc_1_371A
        db  0A1h, 0D0h, 06Eh, 000h, 000h
        db  0A3h, 0CCh, 06Eh, 000h, 000h
        call   sub_1_1DAE
loc_1_371A:
        db  083h, 03Dh, 0D4h, 06Eh, 000h, 000h, 000h
        jne    SHORT loc_1_36CB
        db  0FEh, 00Dh, 0F6h, 06Eh, 000h, 000h
        call   sub_1_67F7
loc_1_372E:
        ret
        db  000h
loc_1_3730:
        adc    byte ptr [eax - 40h], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  080h
loc_1_3738:
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
sub_1_373E:
        push   ebx
        cli
        db  08Dh, 01Dh, 070h, 06Fh, 000h, 000h
        call   sub_1_28F3
        sti
        stc
        je     SHORT loc_1_37A3
        mov    word ptr [ebx + 10h], 2
        mov    word ptr [ebx + 1eh], 1
        push   edi
        mov    edi, edx
        lea    esi, [ebx + 34h]
        mov    dword ptr [ebx + 24h], esi
        call   sub_1_2571
        cmp    dword ptr [eax + 28h], 0
        je     SHORT loc_1_3772
        add    esi, 2
loc_1_3772:
        mov    edx, dword ptr [eax + 31h]
        cmp    edx, dword ptr [esi]
        jne    SHORT loc_1_3787
        mov    edx, dword ptr [eax + 33h]
        cmp    edx, dword ptr [esi + 2]
        jne    SHORT loc_1_3787
        or     word ptr [ebx + 10h], 400h
loc_1_3787:
        pop    edi
        mov    esi, dword ptr [ebx + 24h]
        mov    byte ptr [ecx + esi + 1], 0f0h
        mov    byte ptr [ecx + esi], 0f0h
        mov    byte ptr [ecx + esi + 2], 3
        add    ecx, 3
        mov    esi, ebx
        mov    word ptr [esi + 22h], cx
        clc
loc_1_37A3:
        pop    ebx
        ret
sub_1_37A5:
        push   ebx
        cli
        db  08Dh, 035h, 070h, 06Fh, 000h, 000h
        call   sub_1_28E3
        sti
        stc
        je     SHORT loc_1_381F
        mov    word ptr [esi + 10h], 402h
        mov    word ptr [esi + 1eh], 1
        push   edi
        push   ecx
        lea    edi, [esi + 34h]
        mov    dword ptr [esi + 24h], edi
        push   esi
        db  0BEh, 030h, 037h, 000h, 000h
        mov    ecx, 8
        cmp    dword ptr [eax + 28h], 1
        je     SHORT loc_1_37E5
        db  0BEh, 038h, 037h, 000h, 000h
        mov    ecx, 6
loc_1_37E5:
        cld
        rep movsb
        lea    esi, [eax + 31h]
        movsd
        movsw
        je     SHORT loc_1_37FA
        add    edx, 3
        xchg   dl, dh
        mov    word ptr [edi], dx
        jmp    SHORT loc_1_3803
loc_1_37FA:
        or     byte ptr [edi - 6], 80h
        mov    word ptr [edi], 70c2h
loc_1_3803:
        add    edi, 2
        pop    esi
        mov    byte ptr [edi], 0f0h
        mov    byte ptr [edi + 1], 0f0h
        mov    byte ptr [edi + 2], 3
        sub    edi, dword ptr [esi + 24h]
        add    edi, 3
        mov    word ptr [esi + 22h], di
        clc
        pop    ecx
        pop    edi
loc_1_381F:
        pop    ebx
        ret
sub_1_3821:
        cli
        push   eax
        db  08Bh, 035h, 068h, 06Fh, 000h, 000h
        mov    esi, dword ptr [esi]
        db  08Dh, 005h, 068h, 06Fh, 000h, 000h
        mov    dword ptr [esi + 4], eax
        db  087h, 035h, 068h, 06Fh, 000h, 000h
        cmp    esi, eax
        pop    eax
        je     SHORT loc_1_3888
        sti
        mov    word ptr [esi + 10h], 1
        push   eax
        mov    eax, dword ptr [edi + 34h]
        and    eax, 400h
        or     word ptr [esi + 10h], ax
        mov    eax, dword ptr [edi + 40h]
        mov    dword ptr [esi + 14h], eax
        lea    eax, [ebx + 98h]
        mov    dword ptr [esi + 18h], eax
        sub    eax, eax
        mov    byte ptr [esi + 20h], al
        mov    byte ptr [esi + 28h], al
        mov    word ptr [esi + 1eh], ax
        movzx  eax, byte ptr [ebx + 91h]
        mov    word ptr [esi + 1ch], ax
        mov    dword ptr [esi + 8], edi
        mov    dword ptr [esi + 30h], ebx
        mov    word ptr [esi + 3ch], 12h
        pop    eax
        stc
        ret
loc_1_3888:
        test   byte ptr [ebx + 24h], 8
        jne    SHORT loc_1_38A4
        push   esi
        db  08Dh, 035h, 0A0h, 06Fh, 000h, 000h
        push   ebx
        lea    ebx, [ebx + 10h]
        call   sub_1_2892
        pop    ebx
        pop    esi
        or     byte ptr [ebx + 24h], 8
loc_1_38A4:
        db  081h, 00Dh, 0ECh, 06Eh, 000h, 000h, 000h, 008h, 000h, 000h
        sti
        ret
sub_1_38B0:
        mov    edx, 2ch
        call   sub_1_37A5
        jae    SHORT loc_1_38BD
        ret
loc_1_38BD:
        mov    edx, esi
        movzx  ecx, word ptr [esi + 22h]
        add    word ptr [esi + 22h], 2ch
        mov    esi, dword ptr [esi + 24h]
        add    esi, ecx
        sub    ecx, ecx
        mov    byte ptr [esi + 5], cl
        mov    word ptr [esi + 6], cx
        mov    word ptr [esi + 8], cx
        mov    word ptr [esi + 0ah], cx
        mov    word ptr [esi], 2ch
        mov    word ptr [esi + 2], 0efffh
        mov    byte ptr [esi + 4], 2
        lea    edi, [esi + 0ch]
        lea    esi, [ebx + 54h]
        cld
        movsd
        movsd
        movsd
        movsd
        mov    esi, dword ptr [eax + 84h]
        lea    esi, [esi + 8]
        movsd
        movsd
        movsd
        movsd
        mov    esi, edx
        mov    dword ptr [esi + 14h], eax
        cli
        push   edi
        lea    edi, [eax + 0bch]
        call   sub_1_2868
        pop    edi
        sti
        or     dword ptr [eax], 100h
        db  083h, 00Dh, 0ECh, 06Eh, 000h, 000h, 008h
        ret
sub_1_3927:
        mov    edx, 2ch
        call   sub_1_37A5
        jae    SHORT loc_1_3934
        ret
loc_1_3934:
        cmp    dword ptr [eax + 28h], 1
        jne    SHORT loc_1_3955
        db  080h, 03Dh, 036h, 06Fh, 000h, 000h, 001h
        je     SHORT loc_1_3955
        sub    word ptr [esi + 22h], 2
        mov    edx, dword ptr [esi + 24h]
        and    byte ptr [edx + 8], 7fh
        mov    ecx, dword ptr [edx + 10h]
        mov    dword ptr [edx + 0eh], ecx
loc_1_3955:
        mov    dword ptr [esi + 14h], eax
        movzx  ecx, word ptr [esi + 22h]
        add    word ptr [esi + 22h], 2ch
        add    ecx, dword ptr [esi + 24h]
        mov    word ptr [ecx], 2ch
        mov    word ptr [ecx + 2], 0efffh
        mov    byte ptr [ecx + 4], 0ah
        sub    edx, edx
        mov    byte ptr [ecx + 5], dl
        mov    word ptr [ecx + 8], dx
        push   eax
        mov    eax, dword ptr [eax + 40h]
        call   sub_1_29B1
        mov    byte ptr [ecx + 5], al
        pop    eax
        mov    dl, byte ptr [ebx + 68h]
        mov    word ptr [ecx + 0ah], dx
        cmp    byte ptr [ebx + 25h], 7
        jne    SHORT loc_1_3998
        xor    dl, dl
loc_1_3998:
        mov    word ptr [ecx + 6], dx
        push   esi
        push   ecx
        lea    edi, [ecx + 0ch]
        lea    esi, [ebx + 54h]
        mov    ecx, 4
        cld
        rep movsd
        pop    esi
        mov    ecx, 4
        lea    edi, [esi + 1ch]
        lea    esi, [ebx + 44h]
        rep movsd
        pop    esi
        cli
        push   edi
        lea    edi, [eax + 0bch]
        call   sub_1_2868
        pop    edi
        sti
        or     dword ptr [eax], 100h
        db  083h, 00Dh, 0ECh, 06Eh, 000h, 000h, 008h
        clc
        ret
sub_1_39D9:
        mov    edx, 2ch
        call   sub_1_37A5
        jb     SHORT loc_1_39EA
        jmp    NEAR PTR loc_1_3955
loc_1_39EA:
        stc
        ret
sub_1_39EC:
        mov    edx, 2ch
        call   sub_1_37A5
        jae    SHORT loc_1_39F9
        ret
loc_1_39F9:
        mov    dword ptr [esi + 14h], eax
        movzx  ecx, word ptr [esi + 22h]
        add    word ptr [esi + 22h], 2ch
        add    ecx, dword ptr [esi + 24h]
        mov    word ptr [ecx], 2ch
        mov    word ptr [ecx + 2], 0efffh
        mov    dl, 1
        test   word ptr [ebx + 40h], 10h
        je     SHORT loc_1_3A1F
        mov    dl, 0
loc_1_3A1F:
        mov    byte ptr [ecx + 4], dl
        xor    edx, edx
        mov    byte ptr [ecx + 5], dl
        mov    word ptr [ecx + 6], dx
        mov    word ptr [ecx + 8], dx
        mov    dl, byte ptr [ebx + 68h]
        mov    word ptr [ecx + 0ah], dx
        push   esi
        lea    edi, [ecx + 1ch]
        lea    esi, [ebx + 44h]
        mov    ecx, 4
        cld
        rep movsd
        pop    esi
        cli
        push   edi
        lea    edi, [eax + 0bch]
        call   sub_1_2868
        pop    edi
        sti
        or     dword ptr [eax], 100h
        db  083h, 00Dh, 0ECh, 06Eh, 000h, 000h, 008h
        clc
        ret
sub_1_3A64:
        call   sub_1_373E
        jae    SHORT loc_1_3A6C
        ret
loc_1_3A6C:
        mov    dword ptr [esi + 14h], eax
        movzx  ecx, word ptr [esi + 22h]
        add    word ptr [esi + 22h], 2ch
        add    ecx, dword ptr [esi + 24h]
        mov    word ptr [ecx], 2ch
        mov    word ptr [ecx + 2], 0efffh
        mov    byte ptr [ecx + 4], 0dh
        mov    byte ptr [ecx + 5], 0
        sub    edx, edx
        test   byte ptr [ebx], 1
        je     SHORT loc_1_3A96
        inc    edx
loc_1_3A96:
        mov    word ptr [ecx + 6], dx
        mov    dx, word ptr [edi + 0ah]
        mov    word ptr [ecx + 8], dx
        mov    word ptr [ecx + 0ah], 0
        mov    ebx, ecx
        mov    ecx, 4
        push   edi
loc_1_3AB0:
        mov    edx, dword ptr [edi + 1ch]
        mov    dword ptr [ebx + 1ch], edx
        mov    dword ptr [ebx + 0ch], edx
        add    edi, 4
        add    ebx, 4
        loop   loc_1_3AB0
        pop    edi
        cmp    dword ptr [eax + 28h], 1
        je     SHORT loc_1_3AD6
        mov    ecx, dword ptr [esi + 24h]
        mov    edx, 2fh
        xchg   dl, dh
        mov    word ptr [ecx + 0ch], dx
loc_1_3AD6:
        cli
        push   edi
        lea    edi, [eax + 0bch]
        call   sub_1_2868
        pop    edi
        sti
        or     dword ptr [eax], 100h
        db  083h, 00Dh, 0ECh, 06Eh, 000h, 000h, 008h
        clc
        ret
sub_1_3AF4:
        xchg   ebx, eax
        call   sub_1_373E
        xchg   ebx, eax
        jae    SHORT loc_1_3AFE
        ret
loc_1_3AFE:
        cmp    dword ptr [ebx + 28h], 1
        mov    edx, dword ptr [esi + 24h]
        jne    SHORT loc_1_3B2A
        test   byte ptr [edx + 8], 80h
        je     SHORT loc_1_3B38
        mov    cx, 70c2h
        and    cl, 0bfh
        mov    word ptr [edx + 0eh], cx
        mov    word ptr [edx + 10h], 0f0f0h
        mov    byte ptr [edx + 12h], 3
        mov    word ptr [esi + 22h], 13h
        jmp    SHORT loc_1_3B38
loc_1_3B2A:
        mov    cx, word ptr [esi + 22h]
        add    cx, 1eh
        xchg   cl, ch
        mov    word ptr [edx + 0ch], cx
loc_1_3B38:
        mov    dword ptr [esi + 14h], ebx
        movzx  ecx, word ptr [esi + 22h]
        add    word ptr [esi + 22h], 2ch
        push   esi
        mov    esi, dword ptr [esi + 24h]
        add    esi, ecx
        mov    word ptr [esi], 2ch
        mov    word ptr [esi + 2], 0efffh
        mov    byte ptr [esi + 4], 0eh
        push   eax
        mov    eax, dword ptr [ebx + 40h]
        call   sub_1_29B1
        mov    byte ptr [esi + 5], al
        pop    eax
        mov    word ptr [esi + 6], ax
        xor    ah, ah
        mov    word ptr [esi + 0ah], ax
        mov    ax, word ptr [edi + 0ah]
        mov    word ptr [esi + 8], ax
        xchg   edi, esi
        push   esi
        push   edi
        mov    ecx, 4
        lea    esi, [esi + 1ch]
        lea    edi, [edi + 0ch]
        cld
        rep movsd
        pop    edi
        pop    esi
        mov    ecx, 4
        lea    esi, [esi + 0ch]
        lea    edi, [edi + 1ch]
        rep movsd
        pop    esi
        cli
        push   edi
        lea    edi, [ebx + 0bch]
        call   sub_1_2868
        pop    edi
        sti
        or     dword ptr [ebx], 100h
        db  083h, 00Dh, 0ECh, 06Eh, 000h, 000h, 008h
        clc
        ret
sub_1_3BB8:
        mov    edi, dword ptr [ebx + 38h]
        call   sub_1_3821
        jae    SHORT loc_1_3C39
        push   ecx
        push   edx
        mov    byte ptr [esi + 34h], 19h
        mov    al, 0eh
        mov    ecx, dword ptr [edi + 40h]
        cmp    dword ptr [ecx + 28h], 1
        jne    SHORT loc_1_3BE7
        test   byte ptr [edi + 94h], 80h
        je     SHORT loc_1_3BE7
        mov    al, byte ptr [edi + 9bh]
        and    al, 70h
        shr    al, 3
loc_1_3BE7:
        or     al, 81h
        mov    byte ptr [esi + 35h], al
        mov    eax, 1000h
        movzx  ecx, byte ptr [edi + 0b6h]
        movzx  edx, word ptr [edi + 7ah]
        sub    edx, ecx
        sub    edx, 12h
        sub    eax, edx
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, edx
        mov    word ptr [esi + 36h], ax
        mov    al, byte ptr [ebx + 68h]
        sub    ah, ah
        mov    word ptr [esi + 3ah], ax
        mov    ax, word ptr [ebx + 64h]
        mov    word ptr [esi + 38h], ax
        push   ebx
        mov    ebx, dword ptr [ebx + 38h]
        cli
        push   edi
        lea    edi, [ebx + 8]
        call   sub_1_2868
        pop    edi
        sti
        push   ebx
        call   sub_1_4E6C
        pop    ebx
        clc
        pop    edx
        pop    ecx
        ret
loc_1_3C39:
        or     word ptr [ebx + 6ah], 400h
        sti
        stc
        ret
sub_1_3C42:
        mov    edi, dword ptr [ebx + 38h]
        call   sub_1_3821
        jae    SHORT loc_1_3CC9
        mov    byte ptr [esi + 34h], 17h
        mov    byte ptr [esi + 35h], 81h
        mov    eax, 1000h
        movzx  ecx, byte ptr [edi + 0b6h]
        movzx  edx, word ptr [edi + 7ah]
        sub    edx, ecx
        sub    edx, 12h
        sub    eax, edx
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, edx
        mov    word ptr [esi + 36h], ax
        db  0A1h, 007h, 06Fh, 000h, 000h
        add    eax, 32h
        mov    dword ptr [edi + 6ch], eax
        mov    eax, dword ptr [edi + 6ch]
        mov    dword ptr [edi + 58h], eax
        mov    al, byte ptr [ebx + 68h]
        xor    ah, ah
        mov    word ptr [esi + 3ah], ax
        mov    ax, word ptr [ebx + 64h]
        mov    word ptr [esi + 38h], ax
        push   ebx
        mov    ebx, dword ptr [ebx + 38h]
        cli
        push   edi
        lea    edi, [ebx + 8]
        call   sub_1_2868
        pop    edi
        sti
        push   ebx
        call   sub_1_4E6C
        pop    ebx
        push   eax
        mov    eax, dword ptr [ebx + 38h]
        cmp    dword ptr [eax + 68h], 3ch
        jbe    SHORT loc_1_3CC6
        mov    byte ptr [ebx + 8fh], 0
        mov    byte ptr [ebx + 8eh], 0
loc_1_3CC6:
        pop    eax
        clc
        ret
loc_1_3CC9:
        or     word ptr [ebx + 6ah], 800h
        sti
        stc
        ret
sub_1_3CD2:
        push   esi
        push   edi
        test   word ptr [ebx + 42h], 2
        jne    SHORT loc_1_3D4D
        cli
        test   word ptr [ebx + 6ah], 4
        je     SHORT loc_1_3CFB
        and    word ptr [ebx + 6ah], 0fff3h
        db  0FFh, 00Dh, 0FFh, 06Eh, 000h, 000h
        push   eax
        mov    ax, word ptr [ebx + 64h]
        call   sub_1_3E4E
        pop    eax
loc_1_3CFB:
        sti
        mov    edi, dword ptr [ebx + 38h]
        call   sub_1_3821
        jae    SHORT loc_1_3D4D
        or     word ptr [ebx + 42h], 2
        or     word ptr [esi + 10h], 100h
        mov    byte ptr [esi + 34h], 18h
        sub    eax, eax
        mov    byte ptr [esi + 35h], al
        mov    word ptr [esi + 38h], ax
        mov    word ptr [esi + 3ah], ax
        and    word ptr [ebx + 40h], 0f0ffh
        test   word ptr [ebx + 40h], 40h
        je     SHORT loc_1_3D31
        inc    eax
loc_1_3D31:
        mov    word ptr [esi + 36h], ax
        push   ebx
        mov    ebx, dword ptr [ebx + 38h]
        cli
        push   edi
        lea    edi, [ebx + 8]
        call   sub_1_2868
        pop    edi
        sti
        push   ebx
        call   sub_1_4E6C
        pop    ebx
        stc
loc_1_3D4D:
        cmc
        pop    edi
        pop    esi
        ret
sub_1_3D51:
        push   esi
        push   edi
        cli
        test   word ptr [ebx + 6ah], 80h
        jne    SHORT loc_1_3D97
        sti
        mov    edi, dword ptr [ebx + 38h]
        call   sub_1_3821
        jae    SHORT loc_1_3D97
        mov    byte ptr [esi + 34h], 1bh
        mov    word ptr [esi + 36h], ax
        sub    eax, eax
        mov    byte ptr [esi + 35h], al
        mov    word ptr [esi + 38h], ax
        mov    word ptr [esi + 3ah], ax
        push   ebx
        mov    ebx, dword ptr [ebx + 38h]
        cli
        push   edi
        lea    edi, [ebx + 8]
        call   sub_1_2868
        pop    edi
        sti
        push   ebx
        call   sub_1_4E6C
        pop    ebx
        clc
        pop    edi
        pop    esi
        ret
loc_1_3D97:
        or     word ptr [ebx + 6ah], 100h
        mov    word ptr [ebx + 7eh], ax
        stc
        sti
        pop    edi
        pop    esi
        ret
sub_1_3DA6:
        push   esi
        push   edi
        push   ecx
        sbb    ecx, ecx
        mov    edi, dword ptr [ebx + 38h]
        call   sub_1_3821
        jae    SHORT loc_1_3DE7
        mov    byte ptr [esi + 34h], 1ah
        mov    word ptr [esi + 36h], ax
        sub    eax, eax
        and    cl, 2
        mov    byte ptr [esi + 35h], cl
        mov    word ptr [esi + 38h], ax
        mov    word ptr [esi + 3ah], ax
        push   ebx
        mov    ebx, dword ptr [ebx + 38h]
        cli
        push   edi
        lea    edi, [ebx + 8]
        call   sub_1_2868
        pop    edi
        sti
        push   ebx
        call   sub_1_4E6C
        pop    ebx
        clc
        jmp    SHORT loc_1_3DFF
loc_1_3DE7:
        mov    word ptr [ebx + 7ch], ax
        test   ecx, ecx
        jne    SHORT loc_1_3DF7
        or     word ptr [ebx + 6ah], 80h
        jmp    SHORT loc_1_3DFD
loc_1_3DF7:
        or     word ptr [ebx + 6ah], 1000h
loc_1_3DFD:
        sti
        stc
loc_1_3DFF:
        pop    ecx
        pop    edi
        pop    esi
        ret
sub_1_3E03:
        push   esi
        push   edi
        mov    edi, dword ptr [ebx + 38h]
        call   sub_1_3821
        jae    SHORT loc_1_3E3F
        mov    byte ptr [esi + 34h], 1ch
        mov    word ptr [esi + 38h], ax
        sub    eax, eax
        mov    byte ptr [esi + 35h], al
        mov    word ptr [esi + 36h], ax
        mov    word ptr [esi + 3ah], ax
        push   ebx
        mov    ebx, dword ptr [ebx + 38h]
        cli
        push   edi
        lea    edi, [ebx + 8]
        call   sub_1_2868
        pop    edi
        sti
        push   ebx
        call   sub_1_4E6C
        pop    ebx
        clc
        pop    edi
        pop    esi
        ret
loc_1_3E3F:
        or     word ptr [ebx + 6ah], 200h
        mov    word ptr [ebx + 66h], ax
        sti
        stc
        pop    edi
        pop    esi
        ret
sub_1_3E4E:
        push   esi
        push   edi
        mov    edi, dword ptr [ebx + 38h]
        cli
        db  08Dh, 035h, 068h, 06Fh, 000h, 000h
        call   sub_1_28E3
        stc
        je     SHORT loc_1_3ED1
        mov    word ptr [esi + 10h], 21h
        mov    word ptr [esi + 38h], ax
        mov    eax, dword ptr [edi + 34h]
        and    eax, 400h
        or     word ptr [esi + 10h], ax
        mov    eax, dword ptr [edi + 40h]
        mov    dword ptr [esi + 14h], eax
        lea    eax, [ebx + 98h]
        mov    dword ptr [esi + 18h], eax
        xor    eax, eax
        mov    byte ptr [esi + 20h], al
        mov    byte ptr [esi + 28h], al
        mov    byte ptr [esi + 35h], al
        mov    word ptr [esi + 36h], ax
        mov    word ptr [esi + 3ah], ax
        mov    word ptr [esi + 1eh], ax
        movzx  eax, byte ptr [ebx + 91h]
        mov    word ptr [esi + 1ch], ax
        mov    dword ptr [esi + 8], edi
        mov    dword ptr [esi + 30h], ebx
        mov    word ptr [esi + 3ch], 12h
        mov    byte ptr [esi + 34h], 14h
        push   ebx
        mov    ebx, dword ptr [ebx + 38h]
        cli
        push   edi
        lea    edi, [ebx + 8]
        call   sub_1_2868
        pop    edi
        sti
        push   ebx
        call   sub_1_4E6C
        pop    ebx
        clc
loc_1_3ED1:
        pop    edi
        pop    esi
        ret
loc_1_3ED4:
        stc
        ret
sub_1_3ED6:
        mov    eax, dword ptr [ebx + 3ch]
        mov    edx, 2ch
        call   sub_1_37A5
        jb     SHORT loc_1_3ED4
        mov    dword ptr [esi + 14h], eax
        movzx  edx, word ptr [esi + 22h]
        add    word ptr [esi + 22h], 2ch
        add    edx, dword ptr [esi + 24h]
        mov    word ptr [edx], 2ch
        mov    word ptr [edx + 2], 0efffh
        mov    byte ptr [edx + 4], 3
        sub    ecx, ecx
        mov    word ptr [edx + 8], cx
        mov    cl, byte ptr [ebx + 68h]
        mov    word ptr [edx + 0ah], cx
        mov    cx, word ptr [ebx + 86h]
        sub    cx, word ptr [ebx + 84h]
        mov    word ptr [edx + 6], cx
        mov    cl, byte ptr [ebx + 66h]
        mov    byte ptr [edx + 5], cl
        push   edi
        push   esi
        mov    ecx, 4
        lea    edi, [edx + 0ch]
        lea    esi, [ebx + 54h]
        cld
        rep movsd
        xchg   ecx, eax
        lea    edi, [edx + 1ch]
        stosd
        stosd
        stosw
        lea    esi, [ecx + 31h]
        movsd
        movsw
        pop    esi
        pop    edi
        cli
        push   edi
        lea    edi, [ecx + 0bch]
        call   sub_1_2868
        pop    edi
        sti
        or     dword ptr [ecx], 100h
        db  083h, 00Dh, 0ECh, 06Eh, 000h, 000h, 008h
        clc
        ret
sub_1_3F66:
        push   ebx
        cli
        lea    ebx, [edi]
        cmp    ebx, dword ptr [ebx]
        clc
        je     SHORT loc_1_3FA1
        mov    ebx, dword ptr [ebx]
        sti
        call   sub_1_3821
        jae    SHORT loc_1_3FA1
        mov    byte ptr [esi + 34h], 1fh
        sub    eax, eax
        mov    byte ptr [esi + 35h], al
        mov    word ptr [esi + 36h], ax
        mov    word ptr [esi + 38h], ax
        mov    word ptr [esi + 3ah], ax
        cli
        push   edi
        lea    edi, [edi + 8]
        call   sub_1_2868
        pop    edi
        sti
        push   edi
        call   sub_1_4E6C
        stc
loc_1_3FA1:
        cmc
        sti
        pop    ebx
        ret
        db  000h, 000h, 000h
sub_1_3FA8:
        push   eax
        push   edx
        cli
        mov    edx, dword ptr [esi + 1ch]
        mov    eax, edi
loc_1_3FB0:
        lea    ebx, [esi + 1ch]
        call   sub_1_28F3
        je     SHORT loc_1_3FD9
        mov    ecx, 4
        push   esi
        lea    esi, [ebx + 54h]
        mov    edi, eax
        cld
        repe cmpsd
        pop    esi
        je     SHORT loc_1_3FDC
        push   esi
        lea    esi, [esi + 1ch]
        call   sub_1_2892
        pop    esi
        cmp    edx, ebx
        jne    SHORT loc_1_3FB0
loc_1_3FD9:
        stc
        jmp    SHORT loc_1_3FE1
loc_1_3FDC:
        and    byte ptr [ebx + 24h], 0fdh
        clc
loc_1_3FE1:
        sti
        mov    edi, eax
        pop    edx
        pop    eax
        ret
sub_1_3FE7:
        push   eax
        push   ecx
        push   esi
        mov    al, byte ptr [edi + 6]
        sub    esi, esi
        mov    ecx, 0feh
loc_1_3FF4:
        db  08Bh, 09Eh, 0C0h, 077h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_1_4022
        cmp    byte ptr [ebx + 69h], al
        jne    SHORT loc_1_4022
        cmp    byte ptr [ebx + 25h], 8
        jne    SHORT loc_1_4022
        push   esi
        push   edi
        push   ecx
        lea    esi, [ebx + 54h]
        add    edi, 1ch
        mov    ecx, 4
        cld
        repe cmpsd
        pop    ecx
        pop    edi
        pop    esi
        jne    SHORT loc_1_4022
        stc
        jmp    SHORT loc_1_4028
loc_1_4022:
        add    esi, 4
        loop   loc_1_3FF4
        clc
loc_1_4028:
        pop    esi
        pop    ecx
        pop    eax
        ret
sub_1_402C:
        push   esi
        push   edi
        push   ecx
        mov    edi, dword ptr [esp + 14h]
        mov    esi, dword ptr [esp + 10h]
        push   ecx
        shr    ecx, 2
        cld
        rep movsd
        pop    ecx
        and    ecx, 3
        rep movsb
        pop    ecx
        pop    edi
        pop    esi
        ret    8
sub_1_404A:
        cli
        lea    ebx, [eax + 0b4h]
        call   sub_1_28F3
        lea    ebx, [ebx - 32h]
        sti
        je     SHORT loc_1_4098
        push   esi
        push   edi
        mov    esi, edi
        mov    edi, ebx
        db  0FEh, 005h, 005h, 06Fh, 000h, 000h
        mov    al, byte ptr [esi + 4]
        mov    byte ptr [edi + 3], al
        mov    ax, word ptr [esi + 6]
        mov    word ptr [edi + 8], ax
        push   edi
        add    edi, 1ah
        add    esi, 0ch
        cld
        movsd
        movsd
        movsd
        movsd
        sub    edi, 20h
        cld
        movsd
        movsd
        movsd
        movsd
        pop    edi
        mov    byte ptr [edi + 1], 0
        mov    esi, edi
        call   sub_1_6BE9
        pop    edi
        pop    esi
loc_1_4098:
        ret
loc_1_4099:
        ret
loc_1_409A:
        cmp    byte ptr [ebx + 25h], 0
        je     SHORT loc_1_40AB
        or     word ptr [ebx + 40h], 40h
        push   ebx
        call   sub_1_5BF3
loc_1_40AB:
        ret
loc_1_40AC:
        mov    ebx, eax
        cmp    byte ptr [ebx + 25h], 4
        jne    SHORT loc_1_409A
        test   word ptr [ebx + 42h], 40h
        jne    SHORT loc_1_40D3
        test   word ptr [ebx + 42h], 8
        je     SHORT loc_1_40D4
        or     word ptr [ebx + 40h], 40h
        call   sub_1_3CD2
        call   sub_1_0BAA
loc_1_40D3:
        ret
loc_1_40D4:
        mov    edx, dword ptr [ebx + 38h]
        movzx  ecx, byte ptr [edx + 0b6h]
        movzx  edx, word ptr [edx + 7ah]
        sub    edx, ecx
        sub    edx, 12h
        movzx  eax, word ptr [edi + 6]
        sub    eax, edx
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, edx
        mov    word ptr [ebx + 6eh], ax
        and    word ptr [ebx + 40h], 0fffah
        mov    al, byte ptr [edi + 5]
        test   al, 80h
        je     SHORT loc_1_4108
        or     word ptr [ebx + 40h], 1
loc_1_4108:
        test   al, 1
        je     SHORT loc_1_4111
        or     word ptr [ebx + 40h], 4
loc_1_4111:
        mov    byte ptr [ebx + 25h], 5
        cli
        test   word ptr [ebx + 40h], 80h
        je     SHORT loc_1_4154
        and    word ptr [ebx + 40h], 0ff7fh
        sti
        mov    dl, byte ptr [ebx + 68h]
        mov    esi, dword ptr [ebx + 88h]
        mov    byte ptr [esi + 2], dl
        sub    al, al
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
        push   eax
        mov    eax, dword ptr [ebx + 38h]
        cmp    dword ptr [eax + 68h], 3ch
        jbe    SHORT loc_1_4153
        mov    byte ptr [ebx + 8fh], 0
        mov    byte ptr [ebx + 8eh], 0
loc_1_4153:
        pop    eax
loc_1_4154:
        ret
loc_1_4155:
        test   word ptr [eax + 42h], 4000h
        jne    SHORT loc_1_418A
        cmp    byte ptr [eax + 25h], 5
        jne    SHORT loc_1_418A
        or     word ptr [eax + 40h], 20h
        cmp    word ptr [edi + 6], 0
        je     SHORT loc_1_4174
        or     word ptr [eax + 40h], 40h
loc_1_4174:
        mov    edi, dword ptr [eax + 38h]
        or     dword ptr [edi + 3ch], 80h
        push   edi
        call   sub_1_4E6C
        push   eax
        call   sub_1_5BF3
loc_1_418A:
        ret
loc_1_418B:
        ret
loc_1_418C:
        mov    ebx, eax
        cmp    byte ptr [ebx + 25h], 8
        jne    SHORT loc_1_418B
        test   word ptr [ebx + 42h], 8
        je     SHORT loc_1_41A2
        call   sub_1_0C20
        ret
loc_1_41A2:
        cli
        test   word ptr [ebx + 40h], 80h
        je     SHORT loc_1_4211
        and    word ptr [ebx + 40h], 0ff7fh
        sti
        mov    al, byte ptr [edi + 5]
        test   al, 80h
        je     SHORT loc_1_41BE
        or     word ptr [ebx + 40h], 1
loc_1_41BE:
        test   al, 1
        je     SHORT loc_1_41C7
        or     word ptr [ebx + 40h], 4
loc_1_41C7:
        mov    ax, word ptr [edi + 0ah]
        mov    word ptr [ebx + 64h], ax
        movzx  eax, word ptr [edi + 6]
        mov    edx, dword ptr [ebx + 38h]
        movzx  ecx, byte ptr [edx + 0b6h]
        movzx  edx, word ptr [edx + 7ah]
        sub    edx, ecx
        sub    edx, 12h
        sub    eax, edx
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, edx
        mov    word ptr [ebx + 6eh], ax
        mov    byte ptr [ebx + 25h], 5
        call   sub_1_3C42
        mov    esi, dword ptr [ebx + 88h]
        mov    dl, byte ptr [ebx + 68h]
        mov    byte ptr [esi + 2], dl
        sub    al, al
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
loc_1_4211:
        push   dword ptr [ebx + 38h]
        call   sub_1_4E6C
        ret
loc_1_421A:
        push   edi
        db  08Dh, 03Dh, 070h, 06Fh, 000h, 000h
        call   sub_1_2868
        pop    edi
        sti
loc_1_4228:
        ret
loc_1_4229:
        push   eax
        lea    esi, [edi + 0ch]
        call   sub_1_2480
        pop    eax
        jb     SHORT loc_1_4228
        call   sub_1_373E
        jb     SHORT loc_1_4228
        cli
        db  0F7h, 005h, 0ECh, 06Eh, 000h, 000h, 000h, 010h, 000h, 000h
        jne    SHORT loc_1_421A
        db  081h, 00Dh, 0ECh, 06Eh, 000h, 000h, 000h, 010h, 000h, 000h
        sti
        mov    dword ptr [esi + 14h], eax
        or     word ptr [esi + 10h], 1000h
        movzx  edx, word ptr [edi + 6]
        and    dh, 3fh
        db  0BBh, 0D4h, 07Bh, 000h, 000h
        mov    ecx, 258h
        mov    dword ptr [esi + 2ch], ebx
        mov    byte ptr [esi + 28h], 0
        inc    word ptr [esi + 1eh]
        push   edx
        push   ecx
        sub    ecx, edx
        sbb    eax, eax
        and    ecx, eax
        add    ecx, edx
        push   esi
        mov    esi, dword ptr [esi + 14h]
        sub    ah, ah
        mov    al, byte ptr [edi + 5]
        cmp    al, 1
        ja     SHORT loc_1_4297
        call   sub_1_1381
        sub    eax, eax
loc_1_4297:
        push   eax
        call   sub_1_146C
        pop    ebx
        add    edx, ebx
        pop    esi
        test   al, al
        jne    SHORT loc_1_431C
        add    esp, 8
loc_1_42A8:
        movzx  ebx, word ptr [esi + 22h]
        add    ebx, dword ptr [esi + 24h]
        mov    word ptr [ebx + 6], cx
        and    ch, 3fh
        mov    word ptr [esi + 2ah], cx
        add    word ptr [esi + 22h], 2ch
        mov    word ptr [ebx], 2ch
        mov    word ptr [ebx + 2], 0efffh
        mov    byte ptr [ebx + 4], 0fh
        mov    byte ptr [ebx + 5], dl
        mov    ax, word ptr [edi + 0ah]
        mov    word ptr [ebx + 8], ax
        xor    eax, eax
        mov    word ptr [ebx + 0ah], ax
        push   esi
        lea    esi, [edi + 0ch]
        lea    edi, [ebx + 1ch]
        mov    ecx, 4
        push   ecx
        cld
        rep movsd
        lea    edi, [ebx + 0ch]
        pop    ecx
        rep movsd
        pop    esi
        mov    ebx, dword ptr [esi + 14h]
        cmp    dword ptr [ebx + 28h], 1
        jne    SHORT loc_1_432C
loc_1_42FF:
        cli
        push   edi
        lea    edi, [ebx + 0bch]
        call   sub_1_2868
        pop    edi
        sti
        or     dword ptr [ebx], 100h
        db  083h, 00Dh, 0ECh, 06Eh, 000h, 000h, 008h
        ret
loc_1_431C:
        pop    ebx
        pop    eax
        cmp    eax, ebx
        jbe    SHORT loc_1_4327
        or     ch, 80h
loc_1_4325:
        jmp    SHORT loc_1_42A8
loc_1_4327:
        or     ch, 40h
        jmp    SHORT loc_1_4325
loc_1_432C:
        mov    edi, dword ptr [esi + 24h]
        mov    ax, word ptr [esi + 22h]
        add    ax, word ptr [esi + 2ah]
        sub    ax, 0eh
        xchg   al, ah
        mov    word ptr [edi + 0ch], ax
        jmp    SHORT loc_1_42FF
loc_1_4343:
        mov    ecx, eax
        jmp    SHORT loc_1_4391
loc_1_4347:
        sti
        ret
loc_1_4349:
        movzx  ebx, word ptr [edi + 8]
        dec    ebx
        cmp    ebx, 0fdh
        ja     SHORT loc_1_4347
        db  08Bh, 01Ch, 09Dh, 0C0h, 077h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_1_4347
        cmp    byte ptr [ebx + 25h], 9
        jne    SHORT loc_1_4347
        cli
        test   word ptr [ebx + 40h], 80h
        je     SHORT loc_1_4347
        mov    ecx, dword ptr [ebx + 88h]
        mov    dword ptr [ecx + 3ah], 0ffffffffh
        sti
        push   eax
        movzx  ecx, word ptr [edi + 6]
        and    ch, 3fh
        movzx  eax, word ptr [ebx + 86h]
        cmp    ecx, eax
        ja     SHORT loc_1_4343
loc_1_4391:
        pop    eax
        push   edi
        push   ebx
        mov    esi, ebx
        mov    ebx, edi
        add    ebx, 2ch
        sub    ebx, edx
        mov    edx, dword ptr [esi + 80h]
        mov    edi, eax
        call   sub_1_5F9D
        jb     SHORT loc_1_43D2
loc_1_43AC:
        pop    ebx
        mov    eax, edi
        pop    edi
        mov    dh, byte ptr [edi + 7]
        test   word ptr [ebx + 42h], 8
        jne    SHORT loc_1_43C2
        test   dh, 0c0h
        jne    SHORT loc_1_43D4
        sub    al, al
loc_1_43C2:
        mov    esi, dword ptr [ebx + 88h]
        mov    word ptr [esi + 8], cx
        call   sub_1_18D9
        ret
loc_1_43D2:
        jmp    SHORT loc_1_43AC
loc_1_43D4:
        mov    esi, eax
        mov    al, 6
        test   dh, 40h
        jne    SHORT loc_1_43C2
        cmp    byte ptr [edi + 5], 0
        je     SHORT loc_1_43C2
        add    word ptr [ebx + 84h], cx
        add    dword ptr [ebx + 80h], ecx
        sub    word ptr [ebx + 86h], cx
        mov    al, byte ptr [edi + 5]
        xor    ah, ah
        mov    word ptr [ebx + 66h], ax
        mov    eax, esi
        test   dword ptr [eax + 4], 1
        jne    SHORT loc_1_440F
        mov    al, 1
        jmp    SHORT loc_1_4424
loc_1_440F:
        test   dword ptr [eax + 4], 2
        je     SHORT loc_1_441F
        db  0A0h, 021h, 06Fh, 000h, 000h
        jmp    SHORT loc_1_4424
loc_1_441F:
        db  0A0h, 01Fh, 06Fh, 000h, 000h
loc_1_4424:
        mov    byte ptr [ebx + 90h], al
        call   sub_1_3ED6
        db  0B8h, 038h, 019h, 000h, 000h
        call   sub_1_34D0
        ret
loc_1_443A:
        push   eax
        lea    esi, [edi + 1ch]
        call   sub_1_2480
        jb     SHORT loc_1_4456
        test   byte ptr [ebx], 1
        jne    SHORT loc_1_4456
        mov    eax, dword ptr [ebx + 4]
        call   sub_1_3A64
        add    esp, 4
        ret
loc_1_4456:
        pop    eax
        call   sub_1_404A
        ret
loc_1_445D:
        push   eax
        lea    esi, [edi + 1ch]
        call   sub_1_2480
        jb     SHORT loc_1_4474
        mov    eax, dword ptr [ebx + 4]
        call   sub_1_3A64
        add    esp, 4
        ret
loc_1_4474:
        pop    eax
        call   sub_1_404A
        ret
loc_1_447B:
        movzx  ebx, word ptr [edi + 8]
        dec    ebx
        cmp    ebx, 0fdh
        ja     SHORT loc_1_44CA
        db  08Bh, 01Ch, 09Dh, 0C0h, 077h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_1_44CA
        cli
        cmp    byte ptr [ebx + 25h], 6
        jne    SHORT loc_1_44CA
        btr    word ptr [ebx + 40h], 7
        jae    SHORT loc_1_44CA
        sti
        mov    edi, dword ptr [ebx + 94h]
        mov    byte ptr [edi + 3], 0
        lea    edi, [edi + 8]
        call   sub_1_2527
        mov    esi, dword ptr [ebx + 88h]
        call   sub_1_235E
        mov    al, 16h
        mov    byte ptr [esi + 1], al
        call   sub_1_6BE9
loc_1_44CA:
        sti
        ret
loc_1_44CC:
        ret
loc_1_44CD:
        lea    esi, [edi + 0ch]
        call   sub_1_2480
        jb     SHORT loc_1_44CC
        or     byte ptr [ebx], 2
        mov    esi, ebx
        mov    al, 19h
        call   sub_1_23E6
        db  0BFh, 0C0h, 077h, 000h, 000h
        mov    ecx, 100h
loc_1_44ED:
        mov    ebx, dword ptr [edi]
        test   ebx, ebx
        je     SHORT loc_1_454D
        cmp    dword ptr [ebx + 94h], esi
        jne    SHORT loc_1_454D
        cli
        cmp    byte ptr [ebx + 25h], 5
        je     SHORT loc_1_454D
        test   word ptr [ebx + 40h], 80h
        je     SHORT loc_1_454D
        test   word ptr [ebx + 42h], 40h
        jne    SHORT loc_1_454D
        and    word ptr [ebx + 40h], 0ff7fh
        sti
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebx + 88h]
        mov    byte ptr [esi + 1], 19h
        call   sub_1_6BE9
        pop    ebx
        pop    edi
        pop    esi
        cmp    byte ptr [ebx + 25h], 1
        je     SHORT loc_1_4548
        cmp    byte ptr [ebx + 25h], 7
        je     SHORT loc_1_4548
        or     word ptr [ebx + 42h], 4000h
        push   ebx
        call   sub_1_5BF3
        jmp    SHORT loc_1_454D
loc_1_4548:
        call   sub_1_235E
loc_1_454D:
        sti
        add    edi, 4
        loop   loc_1_44ED
        ret
loc_1_4554:
        lea    esi, [edi + 0ch]
        push   eax
        call   sub_1_2480
        jb     SHORT loc_1_457B
        add    esp, 4
        mov    al, byte ptr [edi + 6]
        or     al, al
        jne    SHORT loc_1_45A5
loc_1_4569:
        xor    ah, ah
        test   byte ptr [ebx], 1
        je     SHORT loc_1_4572
        inc    ah
loc_1_4572:
        mov    ebx, dword ptr [ebx + 4]
        call   sub_1_3AF4
        ret
loc_1_457B:
        pop    eax
        call   sub_1_404A
        ret
loc_1_4582:
        cli
        lea    ebx, [esi + 24h]
        call   sub_1_28F3
        je     SHORT loc_1_4594
        and    byte ptr [ebx + 24h], 0fdh
        sti
        jmp    SHORT loc_1_45BE
loc_1_4594:
        sti
        push   eax
        mov    eax, dword ptr [esi + 4]
        call   sub_1_404A
        pop    eax
        mov    ebx, esi
        xor    al, al
        jmp    SHORT loc_1_4569
loc_1_45A5:
        mov    esi, ebx
        call   sub_1_3FE7
        jb     NEAR PTR loc_1_46A9
        push   edi
        lea    edi, [edi + 1ch]
        call   sub_1_3FA8
        pop    edi
        jb     SHORT loc_1_4582
loc_1_45BE:
        cli
        mov    ecx, dword ptr [ebx + 88h]
        db  0C7h, 041h, 03Ah, 0E1h, 01Eh, 000h, 000h
        push   ecx
        push   ebx
        mov    ebx, dword ptr [esi + 4]
        call   sub_1_2671
        shl    eax, 1
        call   sub_1_265A
        xchg   ecx, eax
        call   sub_1_265A
        add    eax, ecx
        db  003h, 005h, 01Bh, 06Fh, 000h, 000h
        test   dword ptr [ebx + 4], 2
        je     SHORT loc_1_4616
        mov    eax, 7d0h
        test   dword ptr [ebx + 4], 1
        jne    SHORT loc_1_4607
        mov    eax, 64h
loc_1_4607:
        call   sub_1_265A
        db  00Fh, 0B6h, 00Dh, 021h, 06Fh, 000h, 000h
        imul   eax, ecx
loc_1_4616:
        xor    ecx, ecx
        test   dword ptr [ebx + 4], 1
        jne    SHORT loc_1_4625
        mov    cl, 1
        jmp    SHORT loc_1_463C
loc_1_4625:
        test   dword ptr [ebx + 4], 2
        je     SHORT loc_1_4636
        db  08Ah, 00Dh, 021h, 06Fh, 000h, 000h
        jmp    SHORT loc_1_463C
loc_1_4636:
        db  08Ah, 00Dh, 01Fh, 06Fh, 000h, 000h
loc_1_463C:
        pop    ebx
        pop    ecx
        mov    dword ptr [ecx + 32h], eax
        mov    byte ptr [ebx + 25h], 8
        sti
        mov    dword ptr [ebx + 38h], 0
        movzx  eax, byte ptr [edi + 5]
        mov    word ptr [ebx + 0cah], ax
        mov    ecx, 4
        push   esi
        push   edi
        lea    esi, [edi + 1ch]
        lea    edi, [ebx + 54h]
        cld
        rep movsd
        pop    edi
        mov    ecx, 4
        push   edi
        lea    esi, [edi + 0ch]
        lea    edi, [ebx + 44h]
        rep movsd
        mov    esi, dword ptr [ebx + 88h]
        push   esi
        mov    ecx, 4
        lea    edi, [esi + 0ah]
        lea    esi, [ebx + 54h]
        rep movsd
        pop    esi
        mov    ecx, 4
        lea    edi, [esi + 1ah]
        lea    esi, [ebx + 44h]
        rep movsd
        pop    edi
        pop    esi
        mov    ax, word ptr [edi + 0ah]
        mov    word ptr [ebx + 64h], ax
        mov    al, byte ptr [edi + 6]
        mov    byte ptr [ebx + 69h], al
loc_1_46A9:
        mov    al, byte ptr [ebx + 68h]
        xor    ah, ah
        test   byte ptr [esi], 1
        je     SHORT loc_1_46B5
        inc    ah
loc_1_46B5:
        push   ebx
        mov    ebx, dword ptr [esi + 4]
        call   sub_1_3AF4
        cmp    dword ptr [ebx + 28h], 1
        pop    ebx
        je     SHORT loc_1_46CB
        mov    word ptr [ebx + 6eh], 5cah
loc_1_46CB:
        ret
loc_1_46CC:
        dec    byte ptr [ebx + 90h]
        js     SHORT loc_1_46EE
        mov    esi, dword ptr [ebx + 94h]
        mov    eax, dword ptr [esi + 4]
        call   sub_1_38B0
        mov    ecx, eax
        db  0B8h, 0CCh, 046h, 000h, 000h
        jmp    NEAR PTR sub_1_34D0
loc_1_46EE:
        cmp    byte ptr [ebx + 25h], 5
        jne    SHORT loc_1_46F5
        ret
loc_1_46F5:
        cmp    byte ptr [ebx + 25h], 7
        jne    SHORT loc_1_4701
        call   sub_1_235E
        ret
loc_1_4701:
        or     word ptr [ebx + 42h], 4000h
        push   ebx
        call   sub_1_5BF3
        ret
_LTEXT ends
        end
