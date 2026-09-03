; mssndsys_pcode_05.asm -- MSSNDSYS object 5 segment _PTEXT
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
        mov    ebp, esp
        sub    esp, 80h
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  068h, 000h, 000h, 000h, 000h
        lea    edx, [ebp - 80h]
        push   edx
        call   sub_5_226C
        add    esp, 10h
        lea    ecx, [ebp - 80h]
        push   ecx
        call   0
        mov    esp, ebp
        pop    ebp
        ret
sub_5_007E:
        push   ebp
        mov    ebp, esp
        sub    esp, 80h
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  068h, 000h, 000h, 000h, 000h
        lea    edx, [ebp - 80h]
        push   edx
        call   sub_5_226C
        add    esp, 10h
        lea    ecx, [ebp - 80h]
        push   ecx
        call   0
        mov    esp, ebp
        pop    ebp
        ret
sub_5_00AB:
        push   ebp
        mov    ebp, esp
        sub    esp, 214h
        push   ebx
        lea    eax, [ebp - 114h]
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        push   edi
        push   0
        push   100h
        push   eax
        push   0
        push   ebx
        call   sub_5_22A4
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_5_027A
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        mov    esi, edi
        mov    ecx, 0ffffffffh
        lea    edi, [ebp - 114h]
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 114h]
        push   eax
        push   ecx
        push   80000002h
        call   sub_5_2294
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_0132
        xor    eax, eax
        jmp    NEAR PTR loc_5_027A
loc_5_0132:
        push   1
        lea    eax, [ebp - 214h]
        push   100h
        push   eax
        push   0
        push   ebx
        call   sub_5_22A4
        add    esp, 14h
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_5_0159
        push   dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_5_0270
loc_5_0159:
        lea    eax, [ebp - 10h]
        lea    ecx, [ebp - 214h]
        push   eax
        push   ecx
        push   80000002h
        call   sub_5_2294
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_017F
        mov    esi, 1dh
        jmp    NEAR PTR loc_5_0245
loc_5_017F:
        lea    eax, [ebp - 14h]
        lea    ecx, [ebp - 1]
        mov    dword ptr [ebp - 14h], 1
        push   eax
        push   ecx
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 0ch]
        call   sub_5_229C
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_5_01B0
        mov    esi, 1dh
        jmp    NEAR PTR loc_5_0245
loc_5_01B0:
        cmp    dword ptr [ebp - 14h], 0
        je     SHORT loc_5_021F
        lea    eax, [ebp - 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 10h]
        call   sub_5_2294
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_01E2
        lea    eax, [ebp - 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 10h]
        call   sub_5_228C
        add    esp, 0ch
loc_5_01E2:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_5_0281
        push   1
        lea    eax, [ebp - 1]
        mov    dword ptr [ebp - 14h], 1
        push   eax
        push   3
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        call   sub_5_2284
        add    esp, 18h
        push   dword ptr [ebp - 8]
        call   sub_5_2274
        mov    dword ptr [ebp - 8], 0
        add    esp, 4
loc_5_021F:
        push   dword ptr [ebp - 0ch]
        call   sub_5_2274
        add    esp, 4
        lea    eax, [ebp - 114h]
        mov    dword ptr [ebp - 0ch], 0
        push   eax
        push   80000002h
        call   sub_5_227C
        add    esp, 8
loc_5_0245:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_5_0256
        push   dword ptr [ebp - 0ch]
        call   sub_5_2274
        add    esp, 4
loc_5_0256:
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_5_0267
        push   dword ptr [ebp - 10h]
        call   sub_5_2274
        add    esp, 4
loc_5_0267:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_5_0278
        push   dword ptr [ebp - 8]
loc_5_0270:
        call   sub_5_2274
        add    esp, 4
loc_5_0278:
        mov    eax, esi
loc_5_027A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_0281:
        mov    esi, 1dh
        jmp    SHORT loc_5_0245
sub_5_0288:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   1
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 1]
        mov    dword ptr [ebp - 8], 1
        push   ecx
        push   3
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp + 8]
        call   sub_5_22AC
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_5_02C3
        cmp    byte ptr [ebp - 1], 1
        sbb    eax, eax
        inc    eax
        jmp    SHORT loc_5_02C5
loc_5_02C3:
        xor    eax, eax
loc_5_02C5:
        test   eax, eax
        je     SHORT loc_5_02CF
        mov    eax, dword ptr [ebp + 0ch]
        or     byte ptr [eax], 4
loc_5_02CF:
        mov    esp, ebp
        pop    ebp
        ret
sub_5_02D3:
        push   ebp
        mov    ebp, esp
        sub    esp, 0d8h
        push   esi
        lea    ecx, [ebp - 0d8h]
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        push   ecx
        call   sub_5_22B4
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_5_043B
        movzx  eax, word ptr [ebp - 7ch]
        cmp    eax, 1
        je     SHORT loc_5_0316
        cmp    eax, 2
        je     SHORT loc_5_0351
        cmp    eax, 3
        je     NEAR PTR loc_5_03BC
        jmp    NEAR PTR loc_5_0436
loc_5_0316:
        movzx  eax, word ptr [ebp - 52h]
        cmp    eax, 4
        je     SHORT loc_5_0329
        cmp    eax, 8
        je     SHORT loc_5_0338
        jmp    NEAR PTR loc_5_0436
loc_5_0329:
        mov    ax, word ptr [ebp - 7ah]
        mov    esi, dword ptr [ebp + 14h]
        mov    word ptr [esi], ax
        jmp    NEAR PTR loc_5_03EF
loc_5_0338:
        mov    ax, word ptr [ebp - 7ah]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx], ax
        mov    esi, dword ptr [ebp + 14h]
        add    ax, 4
        mov    word ptr [esi], ax
        jmp    NEAR PTR loc_5_03EF
loc_5_0351:
        movzx  eax, word ptr [ebp - 52h]
        cmp    eax, 4
        je     SHORT loc_5_0364
        cmp    eax, 8
        je     SHORT loc_5_039C
        jmp    NEAR PTR loc_5_0436
loc_5_0364:
        mov    ax, word ptr [ebp - 7ah]
        mov    esi, dword ptr [ebp + 14h]
        mov    word ptr [esi], ax
        mov    edi, dword ptr [ebp + 10h]
        mov    ax, word ptr [ebp - 78h]
        push   0
        mov    word ptr [edi], ax
        push   eax
        mov    cx, word ptr [esi]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0384:
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_5_03EF
        mov    ax, word ptr [ebp - 78h]
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [edi], 0ffffh
        mov    word ptr [ecx], ax
        jmp    SHORT loc_5_03EF
loc_5_039C:
        mov    ax, word ptr [ebp - 7ah]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dx, word ptr [ebp - 78h]
        mov    esi, dword ptr [ebp + 14h]
        mov    word ptr [ecx], ax
        mov    ecx, dword ptr [ebp + 18h]
        add    ax, 4
        mov    word ptr [esi], ax
        mov    word ptr [ecx], dx
        jmp    SHORT loc_5_03EF
loc_5_03BC:
        cmp    word ptr [ebp - 52h], 4
        jne    SHORT loc_5_0436
        cmp    word ptr [ebp - 50h], 4
        jne    SHORT loc_5_0436
        cmp    word ptr [ebp - 4eh], 10h
        jne    SHORT loc_5_0436
        mov    ax, word ptr [ebp - 7ah]
        mov    esi, dword ptr [ebp + 14h]
        mov    dx, word ptr [ebp - 78h]
        mov    ecx, dword ptr [ebp + 18h]
        mov    di, word ptr [ebp - 76h]
        mov    word ptr [esi], ax
        mov    eax, dword ptr [ebp + 1ch]
        mov    word ptr [ecx], dx
        mov    word ptr [eax], di
loc_5_03EF:
        cmp    word ptr [ebp - 2ah], 1
        jb     SHORT loc_5_0436
        cmp    word ptr [ebp - 1ah], 1
        jb     SHORT loc_5_0436
        mov    eax, dword ptr [ebp + 20h]
        movzx  cx, byte ptr [ebp - 28h]
        mov    word ptr [eax], cx
        mov    eax, dword ptr [ebp + 24h]
        movzx  cx, byte ptr [ebp - 18h]
        mov    word ptr [eax], cx
        jbe    SHORT loc_5_0422
        mov    ecx, dword ptr [ebp + 28h]
        movzx  dx, byte ptr [ebp - 17h]
        mov    word ptr [ecx], dx
        jmp    SHORT loc_5_0428
loc_5_0422:
        mov    edx, dword ptr [ebp + 28h]
        mov    word ptr [edx], cx
loc_5_0428:
        movzx  eax, word ptr [esi]
        inc    eax
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 13h
        jmp    SHORT loc_5_043B
loc_5_0436:
        mov    eax, 13h
loc_5_043B:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_5_0441:
        push   ebp
        mov    ebp, esp
        sub    esp, 38h
        push   ebx
        lea    eax, [ebp - 8]
        push   esi
        lea    ecx, [ebp - 0ch]
        push   0
        lea    edx, [ebp - 10h]
        mov    dword ptr [ebp - 0ch], 4
        push   eax
        push   ecx
        push   edx
        push   dword ptr [ebp + 8]
        call   sub_5_22BC
        mov    cx, word ptr [ebp - 8]
        add    esp, 14h
        mov    word ptr [ebp - 38h], cx
        lea    ecx, [ebp - 38h]
        mov    dword ptr [ebp - 4], ecx
        push   ebx
        push   ecx
        push   esi
        mov    eax, 4
        mov    ebx, dword ptr [ebp - 4]
        mov    ecx, 25h
        xor    edx, edx
        xor    esi, esi
        db  0CDh, 020h, 001h, 000h, 07Ch, 009h
        pop    esi
        pop    ecx
        pop    ebx
        test   ax, ax
        je     SHORT loc_5_04A0
        mov    eax, 13h
        jmp    SHORT loc_5_0517
loc_5_04A0:
        mov    ax, word ptr [ebp - 22h]
        and    eax, 0fh
        cmp    eax, 5
        je     SHORT loc_5_04B3
        cmp    eax, 0bh
        je     SHORT loc_5_04FD
        jmp    SHORT loc_5_0515
loc_5_04B3:
        mov    ax, word ptr [ebp - 22h]
        and    eax, 0f000h
        cmp    eax, 0e000h
        je     SHORT loc_5_04D8
        cmp    eax, 0f000h
        mov    eax, dword ptr [ebp + 10h]
        je     SHORT loc_5_04F2
        test   eax, eax
        je     SHORT loc_5_0509
        mov    word ptr [eax], 3
        jmp    SHORT loc_5_0509
loc_5_04D8:
        mov    eax, dword ptr [ebp + 10h]
        test   eax, eax
        je     SHORT loc_5_04E4
        mov    word ptr [eax], 1
loc_5_04E4:
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        je     SHORT loc_5_0515
        mov    word ptr [eax], 4
        jmp    SHORT loc_5_0515
loc_5_04F2:
        test   eax, eax
        je     SHORT loc_5_0509
        mov    word ptr [eax], 2
        jmp    SHORT loc_5_0509
loc_5_04FD:
        mov    eax, dword ptr [ebp + 10h]
        test   eax, eax
        je     SHORT loc_5_0509
        mov    word ptr [eax], 100h
loc_5_0509:
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        je     SHORT loc_5_0515
        mov    word ptr [eax], 0fffch
loc_5_0515:
        xor    eax, eax
loc_5_0517:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_051D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0dch
        push   0
        lea    ecx, [ebp - 4]
        push   dword ptr [ebp + 8]
        push   ecx
        call   sub_5_22C4
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_5_0560
        push   0
        lea    ecx, [ebp - 0dch]
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_5_22B4
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_5_0560
        cmp    word ptr [ebp - 80h], 2
        jne    SHORT loc_5_0560
        movzx  eax, word ptr [ebp - 7ch]
        jmp    SHORT loc_5_0565
loc_5_0560:
        mov    eax, 0ffffffffh
loc_5_0565:
        mov    esp, ebp
        pop    ebp
        ret
sub_5_0569:
        push   ebp
        mov    ebp, esp
        sub    esp, 0dch
        push   esi
        lea    eax, [ebp - 0dch]
        push   0
        mov    esi, dword ptr [ebp + 8]
        push   esi
        push   eax
        call   sub_5_22B4
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_5_0642
        lea    eax, [ebp - 2]
        push   eax
        push   0
        push   esi
        call   sub_5_0441
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_5_0642
        movzx  eax, word ptr [ebp - 80h]
        cmp    eax, 1
        je     SHORT loc_5_05BF
        cmp    eax, 2
        je     SHORT loc_5_05F5
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0642
loc_5_05BF:
        movzx  eax, word ptr [ebp - 56h]
        cmp    eax, 10h
        je     SHORT loc_5_05CF
        mov    eax, 13h
        jmp    SHORT loc_5_0642
loc_5_05CF:
        mov    ax, word ptr [ebp - 7eh]
        mov    ecx, dword ptr [ebp + 0ch]
        add    ax, 4
        cmp    word ptr [ebp - 2], 100h
        mov    word ptr [ecx], ax
        jne    SHORT loc_5_061D
        mov    ax, word ptr [ebp - 7eh]
        mov    edx, dword ptr [ebp + 10h]
        add    ax, 8
        mov    word ptr [edx], ax
        jmp    SHORT loc_5_061D
loc_5_05F5:
        movzx  eax, word ptr [ebp - 56h]
        cmp    eax, 4
        je     SHORT loc_5_0605
        mov    eax, 13h
        jmp    SHORT loc_5_0642
loc_5_0605:
        mov    ax, word ptr [ebp - 7ch]
        mov    ecx, dword ptr [ebp + 0ch]
        add    ax, 4
        mov    edx, dword ptr [ebp + 10h]
        mov    si, word ptr [ebp - 7eh]
        mov    word ptr [ecx], ax
        mov    word ptr [edx], si
loc_5_061D:
        cmp    word ptr [ebp - 2eh], 1
        jae    SHORT loc_5_062B
        mov    eax, 13h
        jmp    SHORT loc_5_0642
loc_5_062B:
        mov    eax, dword ptr [ebp + 14h]
        movzx  dx, byte ptr [ebp - 2ch]
        mov    word ptr [eax], dx
        movzx  eax, word ptr [ecx]
        inc    eax
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 13h
loc_5_0642:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_5_0647:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 174h
        test   eax, eax
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_5_0696
        cmp    eax, 1
        je     NEAR PTR loc_5_07E6
        cmp    eax, 2
        je     NEAR PTR loc_5_09C8
        cmp    eax, 3
        je     NEAR PTR loc_5_0A15
        cmp    eax, 4
        je     NEAR PTR loc_5_09C8
        cmp    eax, 8
        je     NEAR PTR loc_5_0A59
        cmp    eax, 0dh
        je     NEAR PTR loc_5_09C8
        jmp    NEAR PTR loc_5_0A8C
loc_5_0696:
        mov    di, 0ffffh
        xor    ebx, ebx
        push   ebx
        lea    eax, [ebp - 54h]
        push   ebx
        lea    ecx, [ebp - 30h]
        push   eax
        mov    esi, dword ptr [ebp + 10h]
        push   ecx
        mov    dword ptr [ebp - 1ch], ebx
        push   esi
        mov    dword ptr [ebp - 54h], ebx
        call   sub_5_22BC
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_5_06BF
        mov    dword ptr [ebp - 30h], ebx
loc_5_06BF:
        cmp    dword ptr [ebp - 30h], 1
        je     SHORT loc_5_06D2
        cmp    dword ptr [ebp - 30h], 0
        je     SHORT loc_5_06D2
        xor    eax, eax
        jmp    NEAR PTR loc_5_0A91
loc_5_06D2:
        push   1
        lea    eax, [ebp - 24h]
        push   esi
        push   eax
        call   sub_5_22E4
        add    esp, 0ch
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_5_06EE
        mov    eax, ebx
        jmp    NEAR PTR loc_5_0A91
loc_5_06EE:
        push   0
        lea    eax, [ebp - 18h]
        push   eax
        lea    edx, [ebp - 14h]
        push   0
        push   dword ptr [ebp - 24h]
        push   edx
        call   sub_5_22DC
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_5_07CE
        xor    esi, esi
        test   ebx, ebx
        jne    SHORT loc_5_0759
loc_5_0713:
        mov    eax, dword ptr [ebp - 18h]
        cmp    eax, 2
        je     SHORT loc_5_0722
        cmp    eax, 4
        je     SHORT loc_5_0739
        jmp    SHORT loc_5_073C
loc_5_0722:
        mov    eax, dword ptr [ebp - 14h]
        cmp    word ptr [eax + 0eh], 8
        jne    SHORT loc_5_073C
        mov    dword ptr [ebp - 1ch], 1
        mov    di, word ptr [eax + 10h]
        jmp    SHORT loc_5_073C
loc_5_0739:
        mov    esi, dword ptr [ebp - 14h]
loc_5_073C:
        push   0
        lea    eax, [ebp - 18h]
        push   eax
        lea    edx, [ebp - 14h]
        push   0
        push   dword ptr [ebp - 14h]
        push   edx
        call   sub_5_22DC
        add    esp, 14h
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_5_0713
loc_5_0759:
        cmp    ebx, 0fh
        jne    SHORT loc_5_07D8
        cmp    dword ptr [ebp - 1ch], 0
        je     SHORT loc_5_07DF
        test   esi, esi
        je     SHORT loc_5_07DF
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_076E:
        add    esp, 4
        test   eax, eax
        je     SHORT loc_5_07DF
        xor    ebx, ebx
loc_5_0777:
        mov    edx, 1
        mov    cl, bl
        shl    edx, cl
        movzx  eax, word ptr [esi + 4]
        test   edx, eax
        je     SHORT loc_5_07A6
        push   ebx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_078F:
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_5_07A6
        mov    ax, 1
        mov    cl, bl
        shl    ax, cl
        not    ax
        and    word ptr [esi + 4], ax
loc_5_07A6:
        inc    ebx
        cmp    ebx, 10h
        jl     SHORT loc_5_0777
        push   0
        lea    ecx, [ebp - 24h]
        push   dword ptr [ebp - 24h]
        push   ecx
        call   sub_5_22D4
        add    esp, 0ch
        mov    ebx, eax
        test   ebx, ebx
        je     NEAR PTR loc_5_06EE
        xor    eax, eax
        jmp    NEAR PTR loc_5_0A91
loc_5_07CE:
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0A91
loc_5_07D8:
        mov    eax, ebx
        jmp    NEAR PTR loc_5_0A91
loc_5_07DF:
        xor    eax, eax
        jmp    NEAR PTR loc_5_0A91
loc_5_07E6:
        push   0
        lea    eax, [ebp - 174h]
        push   100h
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 28h], 0
        push   eax
        push   0
        push   esi
        call   sub_5_22A4
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_5_0A91
        xor    edi, edi
        lea    eax, [ebp - 50h]
        push   edi
        lea    ecx, [ebp - 2ch]
        push   edi
        mov    dword ptr [ebp - 50h], edi
        push   eax
        push   ecx
        push   esi
        call   sub_5_22BC
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_5_0830
        mov    dword ptr [ebp - 2ch], edi
loc_5_0830:
        mov    eax, 0ffffh
        mov    word ptr [ebp - 0ch], ax
        mov    word ptr [ebp - 0eh], ax
        mov    word ptr [ebp - 8], ax
        mov    word ptr [ebp - 2], ax
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 2ch]
        cmp    eax, 8
        je     SHORT loc_5_085D
        cmp    eax, 10h
        je     SHORT loc_5_087F
        cmp    eax, 40h
        je     SHORT loc_5_087F
        jmp    SHORT loc_5_0897
loc_5_085D:
        lea    eax, [ebp - 6]
        lea    ecx, [ebp - 2]
        push   eax
        lea    edx, [ebp - 8]
        or     byte ptr [ebp - 27h], 2
        push   ecx
        push   edx
        push   esi
        call   sub_5_0569
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_5_08C8
        jmp    NEAR PTR loc_5_0A91
loc_5_087F:
        mov    eax, dword ptr [ebp - 2ch]
        sub    eax, 10h
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0fffffd00h
        add    eax, 400h
        or     dword ptr [ebp - 28h], eax
loc_5_0897:
        lea    eax, [ebp - 10h]
        lea    ecx, [ebp - 0ah]
        push   eax
        lea    edx, [ebp - 6]
        push   ecx
        lea    eax, [ebp - 4]
        push   edx
        lea    ecx, [ebp - 2]
        push   eax
        lea    edx, [ebp - 8]
        push   ecx
        lea    eax, [ebp - 0eh]
        push   edx
        lea    ecx, [ebp - 0ch]
        push   eax
        push   ecx
        push   esi
        call   sub_5_02D3
        add    esp, 24h
        test   eax, eax
        jne    NEAR PTR loc_5_0A91
loc_5_08C8:
        push   esi
        call   sub_5_00AB
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_5_0A91
        lea    eax, [ebp - 28h]
        push   eax
        push   esi
        call   sub_5_0288
        add    esp, 8
        push   dword ptr [ebp - 28h]
        push   20h
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ah]
        push   dword ptr [ebp - 6]
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 2]
        push   dword ptr [ebp - 0eh]
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0909:
        add    esp, 2ch
        mov    edi, eax
        test   edi, edi
        jne    NEAR PTR loc_5_09C1
        cmp    word ptr [ebp - 4], 0ffffh
        jne    SHORT loc_5_0932
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_5_22CC
        add    esp, 0ch
        mov    edi, eax
        jmp    SHORT loc_5_0980
loc_5_0932:
        cmp    word ptr [ebp - 6], 9
        mov    eax, 2
        je     SHORT loc_5_0942
        movzx  eax, word ptr [ebp - 6]
loc_5_0942:
        movzx  ecx, word ptr [ebp - 0ah]
        movzx  edx, word ptr [ebp - 4]
        push   ecx
        push   eax
        push   edx
        lea    ecx, [ebp - 74h]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_5_226C
        add    esp, 14h
        lea    ecx, [ebp - 74h]
        db  0C7h, 045h, 0B8h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4ch], ecx
        push   dword ptr [ebp - 4ch]
        push   dword ptr [ebp - 48h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 002h, 000h, 04Ah, 004h
        add    esp, 8
loc_5_0980:
        db  0C7h, 045h, 0C0h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [ebp - 2]
        mov    dword ptr [ebp - 3ch], esi
        sub    ax, 0ffffh
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [ebp - 44h], eax
        push   dword ptr [ebp - 44h]
        push   dword ptr [ebp - 40h]
        push   dword ptr [ebp - 3ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 001h, 000h, 04Ah, 004h
        add    esp, 0ch
        test   edi, edi
        jne    SHORT loc_5_09C1
        push   esi
        call   sub_5_1ED8
        add    esp, 4
loc_5_09C1:
        mov    eax, edi
        jmp    NEAR PTR loc_5_0A91
loc_5_09C8:
        lea    eax, [ebp - 38h]
        mov    esi, dword ptr [ebp + 10h]
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_09D5:
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_5_0A8C
        test   byte ptr [ebp - 37h], 10h
        je     SHORT loc_5_09FF
        db  0C7h, 045h, 0CCh, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 34h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 004h, 000h, 04Ah, 004h
        add    esp, 4
loc_5_09FF:
        push   esi
        call   sub_5_1FB3
        add    esp, 4
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0A0E:
        add    esp, 4
        xor    eax, eax
        jmp    SHORT loc_5_0A91
loc_5_0A15:
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        je     SHORT loc_5_0A23
        cmp    eax, 1
        je     SHORT loc_5_0A3E
        jmp    SHORT loc_5_0A8C
loc_5_0A23:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0A2B:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 13h
        jmp    SHORT loc_5_0A91
loc_5_0A3E:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0A46:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 13h
        jmp    SHORT loc_5_0A91
loc_5_0A59:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 7fffffffh
        cmp    eax, 4
        jb     SHORT loc_5_0A8C
        cmp    eax, 5
        jbe    SHORT loc_5_0A77
        cmp    eax, 6
        jb     SHORT loc_5_0A8C
        cmp    eax, 8
        jbe    SHORT loc_5_0A81
        jmp    SHORT loc_5_0A8C
loc_5_0A77:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0A7F:
        jmp    SHORT loc_5_0A89
loc_5_0A81:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0A89:
        add    esp, 4
loc_5_0A8C:
        mov    eax, 1
loc_5_0A91:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_0A98:
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        cmp    dword ptr [ebp + 8], 5
        push   ebx
        push   esi
        jne    NEAR PTR loc_5_0BE3
        push   0
        mov    esi, dword ptr [ebp + 10h]
        push   esi
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_5_2304
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_5_0BE3
        mov    ebx, 1
        lea    eax, [ebp - 1ch]
        push   ebx
        lea    ecx, [ebp - 1]
        push   eax
        mov    dword ptr [ebp - 1ch], ebx
        push   ecx
        push   3
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_5_22AC
        add    esp, 1ch
        test   eax, eax
        je     SHORT loc_5_0AF1
        mov    byte ptr [ebp - 1], bl
loc_5_0AF1:
        cmp    byte ptr [ebp - 1], 0
        je     NEAR PTR loc_5_0BE3
        lea    eax, [ebp - 10h]
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0B05:
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_5_0BE3
        push   0
        lea    eax, [ebp - 14h]
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_5_22FC
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_5_0BE8
        push   0
        lea    ecx, [ebp - 0ch]
        push   3000h
        push   dword ptr [ebp - 14h]
        push   ecx
        call   sub_5_22F4
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_5_0BE8
        mov    word ptr [ebp - 34h], 1
        mov    word ptr [ebp - 32h], 0ch
        mov    word ptr [ebp - 28h], 0fff0h
        mov    word ptr [ebp - 26h], 10h
        mov    word ptr [ebp - 24h], 220h
        mov    word ptr [ebp - 22h], 24fh
        xor    ebx, ebx
        mov    word ptr [ebp - 30h], bx
        mov    byte ptr [ebp - 2ah], bl
        mov    word ptr [ebp - 2eh], bx
        mov    byte ptr [ebp - 29h], bl
        mov    word ptr [ebp - 2ch], bx
        push   ebx
        mov    word ptr [ebp - 20h], bx
        push   18h
        lea    eax, [ebp - 34h]
        lea    edx, [ebp - 18h]
        push   eax
        mov    byte ptr [ebp - 1eh], bl
        push   2
        mov    byte ptr [ebp - 1dh], bl
        push   dword ptr [ebp - 0ch]
        push   edx
        call   sub_5_22EC
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_5_0BE8
        test   byte ptr [ebp - 10h], 1
        je     SHORT loc_5_0BDF
        test   byte ptr [ebp - 10h], 10h
        je     SHORT loc_5_0BDF
        mov    byte ptr [ebp - 6], 2
        xor    ebx, ebx
        lea    eax, [ebp - 8]
        push   ebx
        mov    byte ptr [ebp - 8], bl
        push   4
        lea    edx, [ebp - 18h]
        push   eax
        mov    byte ptr [ebp - 7], bl
        push   3
        mov    byte ptr [ebp - 5], bl
        push   dword ptr [ebp - 0ch]
        push   edx
        call   sub_5_22EC
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_5_0BE8
loc_5_0BDF:
        xor    eax, eax
        jmp    SHORT loc_5_0BE8
loc_5_0BE3:
        mov    eax, 1
loc_5_0BE8:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_0BEE:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 108h
        test   eax, eax
        push   esi
        push   edi
        je     SHORT loc_5_0C2B
        cmp    eax, 1
        je     SHORT loc_5_0C32
        cmp    eax, 2
        je     NEAR PTR loc_5_0CF1
        cmp    eax, 3
        je     NEAR PTR loc_5_0D19
        cmp    eax, 4
        je     NEAR PTR loc_5_0CF1
        mov    eax, 1
        jmp    NEAR PTR loc_5_0D45
loc_5_0C2B:
        xor    eax, eax
        jmp    NEAR PTR loc_5_0D45
loc_5_0C32:
        push   0
        lea    ecx, [ebp - 108h]
        push   dword ptr [ebp + 10h]
        push   ecx
        call   sub_5_22B4
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_5_0D45
        mov    eax, 0ffffh
        cmp    word ptr [ebp - 0ach], 0
        mov    word ptr [ebp - 2], ax
        mov    word ptr [ebp - 4], ax
        jne    SHORT loc_5_0C6F
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0D45
loc_5_0C6F:
        cmp    word ptr [ebp - 4ah], 0
        mov    di, word ptr [ebp - 0aah]
        je     SHORT loc_5_0C86
        movzx  ax, byte ptr [ebp - 48h]
        mov    word ptr [ebp - 2], ax
loc_5_0C86:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 2]
        push   eax
        push   ecx
        push   edi
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0C97:
        add    esp, 10h
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_5_0CED
        cmp    word ptr [ebp - 4], 9
        mov    eax, 2
        je     SHORT loc_5_0CB0
        movzx  eax, word ptr [ebp - 4]
loc_5_0CB0:
        movzx  ecx, word ptr [ebp - 2]
        movzx  edx, di
        push   ecx
        push   eax
        push   edx
        lea    ecx, [ebp - 30h]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_5_226C
        add    esp, 14h
        lea    ecx, [ebp - 30h]
        db  0C7h, 045h, 0F4h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 10h], ecx
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 002h, 000h, 04Ah, 004h
        add    esp, 8
loc_5_0CED:
        mov    eax, esi
        jmp    SHORT loc_5_0D45
loc_5_0CF1:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0CF9:
        db  0C7h, 045h, 0F8h, 000h, 000h, 000h, 000h
        add    esp, 4
        push   dword ptr [ebp - 8]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 004h, 000h, 04Ah, 004h
        add    esp, 4
        xor    eax, eax
        jmp    SHORT loc_5_0D45
loc_5_0D19:
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        je     SHORT loc_5_0D2C
        cmp    eax, 1
        je     SHORT loc_5_0D2C
        mov    eax, 1
        jmp    SHORT loc_5_0D45
loc_5_0D2C:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0D34:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 13h
loc_5_0D45:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  0CCh
loc_5_0D4C:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_5_0D56
        jmp    SHORT loc_5_0D68
loc_5_0D56:
        cmp    ecx, 4
        jbe    SHORT loc_5_0D61
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_5_0D61:
        je     SHORT loc_5_0D64
        in     al, dx
loc_5_0D64:
        ret
        db  02Eh, 08Bh, 0C0h
loc_5_0D68:
        push   eax
        mov    eax, dword ptr [esi + 24h]
        cmp    eax, ebx
        je     SHORT loc_5_0D88
        or     eax, eax
        jne    SHORT loc_5_0D9B
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 1
        call   sub_5_123C
        pop    edx
        pop    ecx
        pop    ebx
        jb     SHORT loc_5_0D9B
loc_5_0D88:
        pop    eax
        cmp    ecx, 4
        jbe    SHORT loc_5_0D94
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_5_0D94:
        je     SHORT loc_5_0D99
        in     al, dx
        jmp    SHORT loc_5_0D9A
loc_5_0D99:
        out    dx, al
loc_5_0D9A:
        ret
loc_5_0D9B:
        mov    eax, 1
        mov    edi, esi
        call   sub_5_1069
        pop    eax
        xor    eax, eax
        dec    eax
        ret
loc_5_0DAC:
        push   eax
        mov    eax, dword ptr [esi + 24h]
        cmp    eax, ebx
        je     SHORT loc_5_0DCC
        or     eax, eax
        jne    SHORT loc_5_0DDF
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 1
        call   sub_5_123C
        pop    edx
        pop    ecx
        pop    ebx
        jb     SHORT loc_5_0DDF
loc_5_0DCC:
        pop    eax
        cmp    ecx, 4
        jbe    SHORT loc_5_0DD8
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_5_0DD8:
        je     SHORT loc_5_0DDD
        in     al, dx
        jmp    SHORT loc_5_0DDE
loc_5_0DDD:
        out    dx, al
loc_5_0DDE:
        ret
loc_5_0DDF:
        mov    eax, 1
        mov    edi, esi
        call   sub_5_1069
        pop    eax
        xor    eax, eax
        dec    eax
        ret
sub_5_0DF0:
        push   eax
        push   edx
        push   ebx
        push   ecx
        movzx  edx, word ptr [edi + 22h]
        mov    ebx, 105h
        mov    al, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E04:
        mov    ebx, 4
        mov    al, 60h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E10:
        mov    ebx, 104h
        mov    al, 3fh
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E1C:
        mov    ebx, 8
        xor    al, al
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E28:
        mov    ebx, 0bdh
        mov    al, 0c0h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E34:
        mov    ecx, 15h
        mov    ebx, 40h
        mov    al, 3fh
loc_5_0E40:
        xor    bh, bh
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E47:
        mov    bh, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E4E:
        inc    bl
        loop   loc_5_0E40
        mov    ecx, 8
        mov    ebx, 0b0h
        xor    eax, eax
loc_5_0E5E:
        xor    bh, bh
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E65:
        mov    bh, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E6C:
        inc    bl
        loop   loc_5_0E5E
        mov    ebx, 104h
        xor    eax, eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E7C:
        mov    ebx, 105h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E86:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E8B:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E90:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E95:
        pop    ecx
        pop    ebx
        pop    edx
        pop    eax
        ret
loc_5_0E9A:
        mov    edi, edx
        mov    ebx, dword ptr [edi + 30h]
        or     ebx, ebx
        je     SHORT loc_5_0EAE
        movzx  edx, word ptr [edi + 22h]
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        ret
loc_5_0EAE:
        ret
        db  090h
loc_5_0EB0:
        test   word ptr [esi + 4], 2000h
        je     SHORT loc_5_0ED8
        cmp    ecx, 4
        jbe    SHORT loc_5_0EC3
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_5_0EC3:
        je     SHORT loc_5_0F0E
        and    word ptr [esi + 4], 0dfffh
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        mov    al, byte ptr [esi + 0ach]
        ret
loc_5_0ED8:
        push   eax
        mov    eax, dword ptr [esi + 30h]
        cmp    eax, ebx
        je     SHORT loc_5_0EFD
        or     eax, eax
        jne    SHORT loc_5_0F10
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 2
        call   sub_5_123C
        pop    edx
        pop    ecx
        pop    ebx
        jb     SHORT loc_5_0F10
        call   sub_5_0DF0
loc_5_0EFD:
        pop    eax
        cmp    ecx, 4
        jbe    SHORT loc_5_0F09
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_5_0F09:
        je     SHORT loc_5_0F0E
        in     al, dx
        jmp    SHORT loc_5_0F0F
loc_5_0F0E:
        out    dx, al
loc_5_0F0F:
        ret
loc_5_0F10:
        mov    eax, 2
        mov    edi, esi
        call   sub_5_1069
        pop    eax
        xor    eax, eax
        dec    eax
        ret
sub_5_0F21:
        push   esi
        push   eax
        push   ecx
        push   edx
        test   word ptr [edi + 52h], 4
        je     SHORT loc_5_0F48
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_0F37:
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_0F48
        inc    cx
        jmp    SHORT loc_5_0F37
loc_5_0F48:
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 42h]
loc_5_0F52:
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_0F63
        inc    cx
        jmp    SHORT loc_5_0F52
loc_5_0F63:
        test   word ptr [edi + 4], 1000h
        je     SHORT loc_5_0F86
        mov    si, 0fh
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 40h]
loc_5_0F75:
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_0F86
        inc    cx
        jmp    SHORT loc_5_0F75
loc_5_0F86:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_0F8C:
        push   esi
        push   eax
        push   ecx
        push   edx
        test   word ptr [edi + 52h], 4
        je     SHORT loc_5_0FBB
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_5_0FBB
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_0FAA:
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_0FBB
        inc    cx
        jmp    SHORT loc_5_0FAA
loc_5_0FBB:
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 42h]
        add    edx, 0
loc_5_0FC8:
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_0FD9
        inc    cx
        jmp    SHORT loc_5_0FC8
loc_5_0FD9:
        test   word ptr [edi + 4], 1000h
        je     SHORT loc_5_0FFC
        mov    si, 0fh
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 40h]
loc_5_0FEB:
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_0FFC
        inc    cx
        jmp    SHORT loc_5_0FEB
loc_5_0FFC:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_1002:
        push   eax
        push   ebx
        push   edx
        movzx  edx, word ptr [edi + 22h]
        mov    ebx, 4
        mov    eax, 60h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1018:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_101D:
        pop    edx
        pop    ebx
        pop    eax
        ret
sub_5_1021:
        push   esi
        push   eax
        push   ecx
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 22h]
loc_5_102E:
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_103F
        inc    cx
        jmp    SHORT loc_5_102E
loc_5_103F:
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_1044:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 22h]
loc_5_1052:
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_1063
        inc    cx
        jmp    SHORT loc_5_1052
loc_5_1063:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_1069:
        test   word ptr [edi + 4], 1
        jne    SHORT loc_5_10BE
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1076:
        jb     SHORT loc_5_10BE
        cmp    eax, 2
        je     SHORT loc_5_1097
        test   dword ptr [esi], 1
        jne    SHORT loc_5_10BE
        or     dword ptr [esi], 1
        db  00Fh, 0B7h, 00Dh, 00Ch, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        jmp    SHORT loc_5_10AF
loc_5_1097:
        test   dword ptr [esi], 2
        jne    SHORT loc_5_10BE
        or     dword ptr [esi], 2
        db  00Fh, 0B7h, 00Dh, 00Eh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
loc_5_10AF:
        mov    eax, 30h
        xor    esi, esi
        xor    edi, edi
        db  0CDh, 020h, 004h, 000h, 017h, 000h
loc_5_10BE:
        ret
loc_5_10BF:
        pushfd
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_110D
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_5_110D
loc_5_10D3:
        popfd
        pushfd
        push   eax
        mov    edi, dword ptr [eax]
        xor    eax, eax
        cmp    dword ptr [edi + 24h], ebx
        jne    SHORT loc_5_10E9
        or     eax, 1
        mov    dword ptr [edi + 28h], 0ffffffffh
loc_5_10E9:
        cmp    dword ptr [edi + 30h], ebx
        jne    SHORT loc_5_10F8
        or     eax, 2
        mov    dword ptr [edi + 34h], 0ffffffffh
loc_5_10F8:
        or     eax, eax
        je     SHORT loc_5_1101
        call   sub_5_119E
loc_5_1101:
        pop    eax
        cli
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_5_110D
        jmp    SHORT loc_5_10D3
loc_5_110D:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        xor    esi, esi
        xchg   dword ptr [edi + ebx], esi
        or     esi, esi
        je     SHORT loc_5_1122
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_5_1122:
        popfd
        clc
        ret
loc_5_1125:
        pushad
        pushfd
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    eax, 5
        mov    ecx, 34h
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_5_1196
        mov    dword ptr [edi + ebx], esi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_116B
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        sti
        je     SHORT loc_5_116B
loc_5_1156:
        mov    edi, dword ptr [eax]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_115D:
        jb     SHORT loc_5_1183
        cli
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        sti
        je     SHORT loc_5_116B
        jmp    SHORT loc_5_1156
loc_5_116B:
        db  0A1h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 004h, 000h, 00Dh, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 004h, 000h, 00Dh, 000h
        jmp    SHORT loc_5_119A
loc_5_1183:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     SHORT loc_5_1196
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_5_1196:
        popfd
        stc
        jmp    SHORT loc_5_119C
loc_5_119A:
        popfd
        clc
loc_5_119C:
        popad
        ret
sub_5_119E:
        push   eax
        push   edx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_11A6:
        jb     NEAR PTR loc_5_1237
        test   eax, 2
        je     SHORT loc_5_11CC
        cmp    dword ptr [edi + 30h], ebx
        jne    SHORT loc_5_11CC
        call   sub_5_1002
        call   sub_5_1021
        mov    dword ptr [edi + 30h], 0
        and    dword ptr [esi], 0fffffffdh
loc_5_11CC:
        test   eax, 1
        je     SHORT loc_5_1237
        cmp    dword ptr [edi + 24h], ebx
        jne    SHORT loc_5_1237
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_5_11FE
        test   word ptr [edi + 4], 200h
        jne    SHORT loc_5_11FE
        mov    eax, dword ptr [edi + 38h]
        db  0CDh, 020h, 014h, 000h, 004h, 000h
        mov    eax, dword ptr [edi + 3ch]
        or     eax, eax
        je     SHORT loc_5_11FE
        db  0CDh, 020h, 014h, 000h, 004h, 000h
loc_5_11FE:
        test   word ptr [edi + 4], 4000h
        je     SHORT loc_5_120B
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_120B:
        call   sub_5_0F21
        mov    dword ptr [edi + 24h], 0
        mov    eax, dword ptr [edi + 2ch]
        test   word ptr [edi + 4], 8
        jne    SHORT loc_5_122E
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 000h, 003h, 000h
loc_5_122E:
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        and    dword ptr [esi], 0fffffffeh
loc_5_1237:
        pop    esi
        pop    edx
        pop    eax
        clc
        ret
sub_5_123C:
        push   eax
        push   ecx
        push   edx
        push   esi
        test   eax, 2
        je     SHORT loc_5_125F
        cmp    dword ptr [edi + 30h], 0
        je     SHORT loc_5_1254
        cmp    dword ptr [edi + 30h], ebx
        je     SHORT loc_5_125F
        jmp    SHORT loc_5_1279
loc_5_1254:
        mov    dword ptr [edi + 30h], ebx
        call   sub_5_1044
        mov    dword ptr [edi + 34h], ebx
loc_5_125F:
        test   eax, 1
        je     NEAR PTR loc_5_1304
        cmp    dword ptr [edi + 24h], 0
        je     SHORT loc_5_1280
        cmp    dword ptr [edi + 24h], ebx
        je     NEAR PTR loc_5_1303
loc_5_1279:
        stc
        jb     NEAR PTR loc_5_1304
loc_5_1280:
        mov    dword ptr [edi + 24h], ebx
        cmp    dword ptr [edi + 28h], ebx
        je     SHORT loc_5_12AE
        mov    ebx, dword ptr [edi + 28h]
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_5_129F
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1298:
        jb     SHORT loc_5_129F
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_129F:
        mov    ebx, dword ptr [edi + 24h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_12A7:
        jb     SHORT loc_5_12AE
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_12AE:
        mov    dword ptr [edi + 28h], ebx
        call   sub_5_0F8C
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_5_12DC
        test   word ptr [edi + 4], 200h
        jne    SHORT loc_5_12DC
        mov    eax, dword ptr [edi + 38h]
        db  0CDh, 020h, 013h, 000h, 004h, 000h
        mov    eax, dword ptr [edi + 3ch]
        or     eax, eax
        je     SHORT loc_5_12DC
        db  0CDh, 020h, 013h, 000h, 004h, 000h
loc_5_12DC:
        test   word ptr [edi + 4], 8
        jne    SHORT loc_5_1303
        mov    eax, dword ptr [edi + 2ch]
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   ecx, 10h
        je     SHORT loc_5_12FD
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        jmp    SHORT loc_5_1303
loc_5_12FD:
        db  0CDh, 020h, 009h, 000h, 003h, 000h
loc_5_1303:
        clc
loc_5_1304:
        pop    esi
        pop    edx
        pop    ecx
        pop    eax
        ret
sub_5_1309:
        mov    eax, 400h
        clc
        ret
mssndsys_PM_API:
        movzx  eax, word ptr [ebp + 14h]
        cmp    ah, 3
        jae    SHORT loc_5_133A
        movzx  ecx, ah
        shl    ecx, 3
        db  08Dh, 089h, 000h, 000h, 000h, 000h
        movzx  eax, al
        cmp    eax, dword ptr [ecx]
        jae    SHORT loc_5_133A
        mov    ecx, dword ptr [ecx + 4]
        call   dword ptr [ecx + eax*4]
        jb     SHORT loc_5_133A
        and    word ptr [ebp + 2ch], 0fffeh
        ret
loc_5_133A:
        or     word ptr [ebp + 2ch], 1
        ret
loc_5_1340:
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_13A1
        pushfd
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        popfd
        or     eax, eax
        je     SHORT loc_5_13A1
        mov    edi, dword ptr [eax]
        cmp    ebx, dword ptr [edi + 24h]
        jne    SHORT loc_5_13A1
        xchg   esi, eax
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        cmp    esi, -1
        je     SHORT loc_5_13A1
        movzx  ecx, word ptr [ebp + 10h]
        movzx  eax, word ptr [ebp + 18h]
        add    ecx, eax
        cmp    ecx, dword ptr [edi + 1ch]
        ja     SHORT loc_5_13A1
        movzx  ecx, word ptr [ebp + 18h]
        mov    edi, dword ptr [edi + 18h]
        movzx  eax, word ptr [ebp + 10h]
        add    edi, eax
        or     ecx, ecx
        je     SHORT loc_5_13A1
        cld
        shr    ecx, 1
        rep movsw
        adc    cl, cl
        rep movsb
        mov    word ptr [ebp + 1ch], 1
        clc
        ret
loc_5_13A1:
        mov    word ptr [ebp + 1ch], 0
        stc
        ret
loc_5_13A9:
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_140A
        pushfd
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        popfd
        or     eax, eax
        je     SHORT loc_5_140A
        mov    esi, dword ptr [eax]
        cmp    ebx, dword ptr [esi + 24h]
        jne    SHORT loc_5_140A
        xchg   edi, eax
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cmp    edi, -1
        je     SHORT loc_5_140A
        movzx  ecx, word ptr [ebp + 10h]
        movzx  eax, word ptr [ebp + 18h]
        add    ecx, eax
        cmp    ecx, dword ptr [esi + 1ch]
        ja     SHORT loc_5_140A
        movzx  ecx, word ptr [ebp + 18h]
        mov    esi, dword ptr [esi + 18h]
        movzx  eax, word ptr [ebp + 10h]
        add    esi, eax
        or     ecx, ecx
        je     SHORT loc_5_140A
        cld
        shr    ecx, 1
        rep movsw
        adc    cl, cl
        rep movsb
        mov    word ptr [ebp + 1ch], 1
        clc
        ret
loc_5_140A:
        mov    word ptr [ebp + 1ch], 0
        stc
        ret
loc_5_1412:
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_141E:
        add    esp, 8
        jb     SHORT loc_5_1452
        movzx  eax, word ptr [ebp + 10h]
        or     eax, eax
        je     SHORT loc_5_1452
        cmp    eax, 1
        jne    SHORT loc_5_1435
        mov    eax, dword ptr [edi + 38h]
        jmp    SHORT loc_5_143F
loc_5_1435:
        mov    eax, dword ptr [edi + 3ch]
        or     eax, eax
        jne    SHORT loc_5_143F
        mov    eax, dword ptr [edi + 38h]
loc_5_143F:
        or     eax, eax
        je     SHORT loc_5_1452
        push   ecx
        db  0CDh, 020h, 01Ch, 000h, 004h, 000h
        mov    word ptr [ebp + 1ch], cx
        pop    ecx
        clc
        jmp    SHORT loc_5_1459
loc_5_1452:
        mov    word ptr [ebp + 1ch], 0ffffh
        stc
loc_5_1459:
        pop    edi
        ret
loc_5_145B:
        call   sub_5_1309
        mov    word ptr [ebp + 1ch], ax
        clc
        ret
loc_5_1466:
        movzx  eax, word ptr [ebp + 10h]
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_14BA
        test   eax, 1
        je     SHORT loc_5_149F
        push   4
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1481:
        add    esp, 8
        or     edi, edi
        jne    SHORT loc_5_149A
        add    edx, 4
        push   4
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1493:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_14BA
loc_5_149A:
        mov    ecx, dword ptr [edi + 28h]
        jmp    SHORT loc_5_14C2
loc_5_149F:
        test   eax, 2
        je     SHORT loc_5_14BA
        push   5
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_14AE:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_14BA
        mov    ecx, dword ptr [edi + 34h]
        jmp    SHORT loc_5_14C2
loc_5_14BA:
        mov    word ptr [ebp + 1ch], 1
loc_5_14C0:
        stc
        ret
loc_5_14C2:
        mov    word ptr [ebp + 1ch], 0
        call   sub_5_123C
        jb     SHORT loc_5_14D0
        ret
loc_5_14D0:
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_5_14C0
loc_5_14D8:
        movzx  eax, word ptr [ebp + 10h]
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_1534
        test   eax, 1
        je     SHORT loc_5_1519
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_5_1505
        push   4
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_14FB:
        add    esp, 8
        or     edi, edi
        jne    SHORT loc_5_1514
        add    edx, 4
loc_5_1505:
        push   4
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_150D:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_1534
loc_5_1514:
        mov    ecx, dword ptr [edi + 24h]
        jmp    SHORT loc_5_153C
loc_5_1519:
        test   eax, 2
        je     SHORT loc_5_1534
        push   5
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1528:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_1534
        mov    ecx, dword ptr [edi + 30h]
        jmp    SHORT loc_5_153C
loc_5_1534:
        mov    word ptr [ebp + 1ch], 1
loc_5_153A:
        stc
        ret
loc_5_153C:
        cmp    ecx, ebx
        je     SHORT loc_5_1548
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_5_153A
loc_5_1548:
        call   sub_5_119E
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
loc_5_1555:
        pushfd
        mov    ax, word ptr [ebp + 1ch]
        and    ax, 0fffeh
        jne    SHORT loc_5_15C6
        xor    ecx, ecx
        mov    ax, word ptr [ebp + 1ch]
        and    ax, 1
        je     SHORT loc_5_156F
        mov    ecx, dword ptr [ebp + 18h]
loc_5_156F:
        cli
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_15C6
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_5_15C6
        mov    edi, dword ptr [eax]
        jecxz  loc_5_1597
loc_5_1586:
        cmp    dword ptr [edi + 54h], ecx
        je     SHORT loc_5_1597
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_5_15C6
        mov    edi, dword ptr [eax]
        jmp    SHORT loc_5_1586
loc_5_1597:
        popfd
        pushfd
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cmp    edi, -1
        je     SHORT loc_5_15C6
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     SHORT loc_5_15C6
        cmp    ecx, 0bah
        ja     SHORT loc_5_15C6
        cld
        rep movsb
        mov    word ptr [ebp + 1ch], 1
        popfd
        clc
        ret
loc_5_15C6:
        mov    word ptr [ebp + 1ch], 0
        popfd
        stc
        ret
loc_5_15CF:
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_1604
        push   5
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_15DF:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_1604
        mov    ecx, dword ptr [edi + 34h]
        mov    word ptr [ebp + 1ch], 0
        mov    eax, 2
        call   sub_5_123C
        jb     SHORT loc_5_15FC
        ret
loc_5_15FC:
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_5_160A
loc_5_1604:
        mov    word ptr [ebp + 1ch], 1
loc_5_160A:
        stc
        ret
loc_5_160C:
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_1644
        push   5
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_161C:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_1644
        mov    ecx, dword ptr [edi + 30h]
        cmp    ecx, ebx
        je     SHORT loc_5_1632
        mov    word ptr [ebp + 1ch], 3
        jmp    SHORT loc_5_164A
loc_5_1632:
        mov    eax, 2
        call   sub_5_119E
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
loc_5_1644:
        mov    word ptr [ebp + 1ch], 1
loc_5_164A:
        stc
        ret
loc_5_164C:
        pushfd
        mov    ax, word ptr [ebp + 1ch]
        cmp    ax, 1
        jne    SHORT loc_5_16C5
        mov    ecx, dword ptr [ebp + 18h]
        cli
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_16C5
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_5_16C5
        mov    edi, dword ptr [eax]
loc_5_166F:
        cmp    dword ptr [edi + 54h], ecx
        je     SHORT loc_5_1680
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_5_16C5
        mov    edi, dword ptr [eax]
        jmp    SHORT loc_5_166F
loc_5_1680:
        popfd
        pushfd
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cmp    edi, -1
        je     SHORT loc_5_16C5
        test   word ptr [esi + 52h], 8
        je     SHORT loc_5_16C5
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     SHORT loc_5_16C5
        cmp    ecx, 1ch
        ja     SHORT loc_5_16C5
        mov    word ptr [edi + 4], 1
        mov    ax, word ptr [esi + 22h]
        mov    word ptr [edi + 6], ax
        mov    eax, dword ptr [esi + 54h]
        mov    dword ptr [edi + 0ch], eax
        mov    word ptr [ebp + 1ch], 1
        popfd
        clc
        ret
loc_5_16C5:
        mov    word ptr [ebp + 1ch], 0
        popfd
        stc
        ret
loc_5_16CE:
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
loc_5_1726:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_1752
        lea    eax, [edi + 44h]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   eax
        push   dword ptr [esi]
        call   sub_5_1885
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_5_1752
        mov    word ptr [ebp + 1ch], ax
        shr    eax, 10h
        mov    word ptr [ebp + 14h], ax
        clc
        ret
loc_5_1752:
        mov    word ptr [ebp + 1ch], 0
        mov    word ptr [ebp + 14h], 0
        stc
        ret
loc_5_1760:
        xchg   esi, eax
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        mov    eax, dword ptr [esi]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1776:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_178C
        lea    eax, [edi + 44h]
        push   dword ptr [esi + 4]
        push   eax
        call   sub_5_1A15
        add    esp, 8
loc_5_178C:
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
sub_5_1794:
        push   ebp
        mov    ebp, esp
        sub    esp, 6ch
        mov    eax, 0ffffffffh
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   esi, esi
        je     NEAR PTR loc_5_187E
        lea    eax, [ebp - 6ch]
        push   eax
        call   sub_5_1AAA
        add    esp, 4
        call   sub_5_1A98
        call   sub_5_1AE7
        mov    edi, dword ptr [eax + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_1AD9
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_1AD9
        add    esp, 4
        mov    ebx, dword ptr [ebp + 10h]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_1AD9
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_1AD9
        add    esp, 4
        mov    ebx, dword ptr [ebp + 14h]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_1AD9
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_1AD9
        add    esp, 4
        mov    ebx, dword ptr [ebp + 18h]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_1AD9
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_1AD9
        movzx  ecx, si
        shr    esi, 10h
        add    esp, 4
        movzx  eax, si
        push   ecx
        push   eax
        call   sub_5_1ACA
        add    esp, 8
        call   sub_5_1AA4
        movzx  esi, word ptr [edi + 14h]
        shl    esi, 10h
        movzx  eax, word ptr [edi + 1ch]
        or     esi, eax
        call   sub_5_1A9E
        lea    ecx, [ebp - 6ch]
        push   ecx
        call   sub_5_1ABA
        add    esp, 4
        mov    eax, esi
loc_5_187E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_1885:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        push   ebp
        mov    ebp, dword ptr [esi]
        test   ebp, ebp
        jne    SHORT loc_5_18AB
        push   15h
        push   4
        call   sub_5_2350
        mov    ebp, eax
        test   ebp, ebp
        jne    SHORT loc_5_18A9
        xor    eax, eax
        jmp    NEAR PTR loc_5_1A10
loc_5_18A9:
        mov    dword ptr [esi], ebp
loc_5_18AB:
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 8
        jbe    SHORT loc_5_18C8
        mov    eax, dword ptr [esp + 1ch]
        mov    byte ptr [eax + 8], 0
loc_5_18C8:
        push   ebp
        call   sub_5_2340
        mov    ebx, eax
        test   ebx, ebx
        je     NEAR PTR loc_5_19BC
loc_5_18D8:
        mov    ecx, dword ptr [esp + 1ch]
        lea    edx, [ebx + 0ch]
loc_5_18DF:
        mov    al, byte ptr [ecx]
        cmp    al, byte ptr [edx]
        jne    SHORT loc_5_18FF
        or     al, al
        je     SHORT loc_5_18FB
        mov    al, byte ptr [ecx + 1]
        cmp    al, byte ptr [edx + 1]
        jne    SHORT loc_5_18FF
        add    ecx, 2
        add    edx, 2
        or     al, al
        jne    SHORT loc_5_18DF
loc_5_18FB:
        xor    eax, eax
        jmp    SHORT loc_5_1904
loc_5_18FF:
        sbb    eax, eax
        sbb    eax, -1
loc_5_1904:
        test   eax, eax
        je     SHORT loc_5_1915
        push   ebx
        push   ebp
        call   sub_5_2330
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_5_18D8
loc_5_1915:
        test   ebx, ebx
        je     NEAR PTR loc_5_19BC
        cmp    dword ptr [ebx + 4], 0
        je     SHORT loc_5_192A
        xor    eax, eax
        jmp    NEAR PTR loc_5_1A10
loc_5_192A:
        push   0
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_1794
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_5_1957
        push   ebx
        mov    dword ptr [ebx], 0
        push   esi
        call   sub_5_1A15
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_5_1A10
loc_5_1957:
        mov    eax, dword ptr [esp + 20h]
        push   0
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebx + 4], ecx
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   ecx
        call   sub_5_1794
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_5_198E
        mov    dword ptr [ebx + 4], 0
        push   ebx
        push   esi
        call   sub_5_1A15
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_5_1A10
loc_5_198E:
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx + 8]
        push   10000h
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_1794
        add    esp, 14h
        push   dword ptr [ebx]
        push   dword ptr [ebx + 8]
        push   10000h
        push   ebx
        push   dword ptr [ebx + 4]
        call   sub_5_1794
        add    esp, 14h
        jmp    SHORT loc_5_1A0E
loc_5_19BC:
        push   ebp
        call   sub_5_231C
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_5_19CC
        xor    eax, eax
        jmp    SHORT loc_5_1A10
loc_5_19CC:
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
        call   sub_5_230C
loc_5_1A0E:
        mov    eax, ebx
loc_5_1A10:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_1A15:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        test   edi, edi
        je     SHORT loc_5_1A61
        mov    eax, dword ptr [edi]
        test   eax, eax
        je     SHORT loc_5_1A3F
        push   0
        push   dword ptr [edi + 8]
        push   40000h
        push   edi
        push   eax
        call   sub_5_1794
        add    esp, 14h
        mov    dword ptr [edi], 0
loc_5_1A3F:
        mov    eax, dword ptr [edi + 4]
        test   eax, eax
        je     SHORT loc_5_1A61
        push   0
        push   dword ptr [edi + 8]
        push   40000h
        push   edi
        push   eax
        call   sub_5_1794
        mov    dword ptr [edi + 4], 0
        add    esp, 14h
loc_5_1A61:
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     SHORT loc_5_1A92
        push   edi
        push   eax
        call   sub_5_2384
        push   edi
        push   dword ptr [esi]
        call   sub_5_2374
        push   dword ptr [esi]
        call   sub_5_2340
        test   eax, eax
        jne    SHORT loc_5_1A8C
        push   dword ptr [esi]
        call   sub_5_2364
loc_5_1A8C:
        mov    dword ptr [esi], 0
loc_5_1A92:
        pop    edi
        pop    esi
        ret
        db  0CCh, 0CCh, 0CCh
sub_5_1A98:
        db  0CDh, 020h, 083h, 080h, 001h, 000h
sub_5_1A9E:
        db  0CDh, 020h, 086h, 080h, 001h, 000h
sub_5_1AA4:
        db  0CDh, 020h, 085h, 080h, 001h, 000h
sub_5_1AAA:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        pop    ebp
        ret
sub_5_1ABA:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        pop    ebp
        ret
sub_5_1ACA:
        movzx  ecx, word ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        db  0CDh, 020h, 048h, 080h, 001h, 000h
sub_5_1AD9:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        pop    ebp
        ret
sub_5_1AE7:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
        db  0CCh, 0CCh
sub_5_1AF4:
        db  00Fh, 0B7h, 005h, 010h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        ret
sub_5_1B02:
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
sub_5_1B1D:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 018h, 000h, 004h, 000h
        pop    ebp
        ret
sub_5_1B2B:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 019h, 000h, 004h, 000h
        pop    ebx
        pop    ebp
        ret
sub_5_1B41:
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
sub_5_1B62:
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
sub_5_1B86:
        push   ebp
        mov    ebp, esp
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    ah, byte ptr [ebp + 0ch]
        mov    al, byte ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1B98:
        xor    eax, eax
        jae    SHORT loc_5_1BA1
        mov    eax, 1
loc_5_1BA1:
        pop    edx
        pop    ebp
        ret
sub_5_1BA4:
        push   ebp
        mov    ebp, esp
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    ah, byte ptr [ebp + 0ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1BB3:
        xor    ah, ah
        pop    edx
        pop    ebp
        ret
sub_5_1BB8:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1BC8:
        add    esp, 8
        mov    word ptr [edi + 0b8h], 0
        pop    ebx
        pop    edi
        pop    ebp
        ret
sub_5_1BD8:
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
loc_5_1BEE:
        add    esp, 8
        mov    eax, dword ptr [edi + 24h]
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        cmp    eax, ebx
        je     SHORT loc_5_1C00
        jmp    SHORT loc_5_1C0E
loc_5_1C00:
        mov    word ptr [edi + 0b8h], 1
        mov    edx, 0
loc_5_1C0E:
        mov    eax, edx
        pop    ebx
        pop    edx
        pop    edi
        pop    ebp
        ret
sub_5_1C15:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1C28:
        add    esp, 8
        mov    ax, word ptr [edi + 42h]
        mov    word ptr [esi + 30h], ax
        mov    ah, byte ptr [edi + 9]
        mov    byte ptr [esi + 32h], ah
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [esi + 0ch], eax
        mov    eax, dword ptr [edi + 18h]
        mov    dword ptr [esi + 10h], eax
        mov    eax, dword ptr [edi + 1ch]
        mov    dword ptr [esi + 14h], eax
        mov    eax, dword ptr [edi + 38h]
        mov    dword ptr [esi + 18h], eax
        mov    ax, word ptr [edi + 52h]
        mov    word ptr [esi + 2eh], ax
        mov    ax, word ptr [edi + 4ch]
        mov    word ptr [esi + 2ch], ax
        pop    esi
        pop    edi
        pop    ebp
        ret
loc_5_1C65:
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        push   edi
        mov    ecx, 10h
        db  0BFh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb
        je     SHORT loc_5_1C9A
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        mov    esi, eax
        repe cmpsb
        mov    eax, 80004002h
        jne    SHORT loc_5_1CA8
loc_5_1C9A:
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_1CA8:
        pop    edi
        pop    esi
        ret    0ch
loc_5_1CAD:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0ch]
        inc    eax
        mov    dword ptr [ecx + 0ch], eax
        ret    4
loc_5_1CBB:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0ch]
        dec    eax
        mov    dword ptr [ecx + 0ch], eax
        ret    4
loc_5_1CC9:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    dword ptr [esi], 0bh
        call   sub_5_1AF4
        push   0ffh
        lea    ecx, [esi + 4]
        push   eax
        lea    edi, [esi + 104h]
        push   ecx
        call   sub_5_2398
        add    esp, 0ch
        push   0ffh
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        call   sub_5_2398
        mov    eax, dword ptr [esp + 18h]
        add    esp, 0ch
        xor    ecx, ecx
        mov    dword ptr [esi + 210h], ecx
        mov    edx, dword ptr [eax + 4]
        mov    eax, 0b3h
        mov    dword ptr [esi + 204h], edx
        mov    dword ptr [esi + 20ch], ecx
        mov    word ptr [esi + 208h], 45fh
        cmp    byte ptr [edi], cl
        je     SHORT loc_5_1D3D
loc_5_1D32:
        movsx  ecx, byte ptr [edi]
        add    eax, ecx
        inc    edi
        cmp    byte ptr [edi], 0
        jne    SHORT loc_5_1D32
loc_5_1D3D:
        mov    ecx, 0f7h
        sub    edx, edx
        div    ecx
        mov    word ptr [esi + 20ah], dx
        xor    eax, eax
        pop    edi
        pop    esi
        ret    8
loc_5_1D54:
        push   esi
        push   edi
        mov    edi, 8878000ah
        mov    esi, dword ptr [esp + 0ch]
        cmp    dword ptr [esi + 0ch], 0
        jne    SHORT loc_5_1D79
        push   dword ptr [esi + 4]
        call   sub_5_1BD8
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_5_1D79
        xor    edi, edi
        inc    dword ptr [esi + 0ch]
loc_5_1D79:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    4
loc_5_1D80:
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
loc_5_1DCB:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   ebp
        mov    edi, 8007000eh
        mov    esi, dword ptr [esp + 2ch]
        test   byte ptr [esp + 1ch], 0e0h
        mov    dword ptr [esi], ebx
        jne    NEAR PTR loc_5_1E72
        mov    ebp, dword ptr [esp + 14h]
        cmp    dword ptr [ebp + 10h], ebx
        jne    SHORT loc_5_1E6B
        push   201h
        push   33h
        call   sub_5_23A8
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_5_1E64
        db  0C7h, 003h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 4], ebp
        push   dword ptr [ebp + 4]
        push   ebx
        call   sub_5_1C15
        add    esp, 8
        mov    eax, dword ptr [ebx + 14h]
        cmp    eax, 1000h
        jbe    SHORT loc_5_1E7B
        cmp    eax, 0ffffh
        jae    SHORT loc_5_1E7B
        cmp    dword ptr [ebx + 0ch], 0
        je     SHORT loc_5_1E7B
        cmp    dword ptr [ebx + 10h], 0
        je     SHORT loc_5_1E7B
        cmp    dword ptr [ebx + 18h], 0
        je     SHORT loc_5_1E7B
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 2ch]
        push   ebx
        xor    edi, edi
        call   sub_5_201B
        mov    eax, dword ptr [esp + 24h]
        add    esp, 0ch
        movzx  ecx, word ptr [eax + 0eh]
        mov    dword ptr [ebx + 24h], ecx
        movzx  edx, word ptr [eax + 2]
        mov    dword ptr [ebx + 28h], edx
        jmp    SHORT loc_5_1E7B
loc_5_1E64:
        mov    edi, 8007000eh
        jmp    SHORT loc_5_1E7B
loc_5_1E6B:
        mov    edi, 8878000ah
        jmp    SHORT loc_5_1E7B
loc_5_1E72:
        mov    edi, 8878001eh
        mov    ebp, dword ptr [esp + 14h]
loc_5_1E7B:
        test   edi, edi
        je     SHORT loc_5_1E90
        test   ebx, ebx
        je     SHORT loc_5_1E95
        push   0
        push   ebx
        call   sub_5_23A0
        add    esp, 8
        jmp    SHORT loc_5_1E95
loc_5_1E90:
        mov    dword ptr [esi], ebx
        inc    dword ptr [ebp + 10h]
loc_5_1E95:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    1ch
loc_5_1E9E:
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], 0
        mov    eax, 80004001h
        ret    0ch
loc_5_1EB0:
        mov    eax, 80004005h
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 0ch]
        dec    edx
        mov    dword ptr [ecx + 0ch], edx
        jne    SHORT loc_5_1ED5
        cmp    dword ptr [ecx + 10h], 0
        jne    SHORT loc_5_1ED5
        push   dword ptr [ecx + 4]
        call   sub_5_1BB8
        add    esp, 4
        xor    eax, eax
loc_5_1ED5:
        ret    4
sub_5_1ED8:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        push   esi
        lea    eax, [ebp - 18h]
        push   esi
        lea    ecx, [ebp - 8]
        push   eax
        mov    ebx, dword ptr [ebp + 8]
        push   ecx
        mov    dword ptr [ebp - 4], esi
        push   ebx
        mov    dword ptr [ebp - 18h], esi
        call   sub_5_22BC
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_5_1F06
        mov    dword ptr [ebp - 8], esi
loc_5_1F06:
        cmp    dword ptr [ebp - 8], 8
        je     NEAR PTR loc_5_1FAC
        push   201h
        push   14h
        call   sub_5_23A8
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_5_1F83
        push   1
        lea    eax, [ebp - 4]
        db  068h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 1ch]
        push   eax
        mov    dword ptr [edi + 4], ebx
        push   ecx
        db  0C7h, 007h, 000h, 000h, 000h, 000h
        call   sub_5_23C4
        test   eax, eax
        jne    SHORT loc_5_1F83
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 10h], edi
        mov    dword ptr [ebp - 14h], 0
        mov    dword ptr [edi + 8], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 10h]
        db  0CDh, 020h, 001h, 000h, 07Eh, 035h
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_5_1F83
        push   edi
        mov    esi, 1
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1F80:
        add    esp, 8
loc_5_1F83:
        test   esi, esi
        jne    SHORT loc_5_1FAC
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_5_1F9D
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 0bh]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_5_23B0
loc_5_1F9D:
        test   edi, edi
        je     SHORT loc_5_1FAC
        push   0
        push   edi
        call   sub_5_23A0
        add    esp, 8
loc_5_1FAC:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_1FB3:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1FC4:
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_5_2015
        push   0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1FD5:
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
        call   sub_5_23B0
        push   0
        push   esi
        call   sub_5_23A0
        add    esp, 8
loc_5_2015:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_5_201B:
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 14h]
        cmp    dword ptr [edx], eax
        jbe    SHORT loc_5_202D
        mov    dword ptr [edx], eax
loc_5_202D:
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
loc_5_204D:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 20h]
        dec    esi
        mov    dword ptr [eax + 20h], esi
        jne    SHORT loc_5_2072
        cmp    dword ptr [eax + 8], 0
        jne    SHORT loc_5_2072
        mov    ecx, dword ptr [eax + 4]
        push   0
        push   eax
        dec    dword ptr [ecx + 10h]
        call   sub_5_23A0
        add    esp, 8
loc_5_2072:
        mov    eax, esi
        pop    esi
        ret    4
loc_5_2078:
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        push   edi
        mov    ecx, 10h
        db  0BFh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb
        je     SHORT loc_5_20AD
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        mov    esi, eax
        repe cmpsb
        mov    eax, 80004002h
        jne    SHORT loc_5_20BB
loc_5_20AD:
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_20BB:
        pop    edi
        pop    esi
        ret    0ch
loc_5_20C0:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 20h]
        inc    eax
        mov    dword ptr [ecx + 20h], eax
        ret    4
loc_5_20CE:
        mov    eax, 80004001h
        ret    20h
loc_5_20D6:
        mov    eax, 80004001h
        ret    14h
loc_5_20DE:
        mov    eax, 80004001h
        ret    8
loc_5_20E6:
        mov    eax, 80004001h
        ret    8
loc_5_20EE:
        mov    eax, 80004001h
        ret    8
loc_5_20F6:
        mov    eax, 80004001h
        ret    8
loc_5_20FE:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 8], 1
        jne    SHORT loc_5_2130
        push   dword ptr [esi + 18h]
        call   sub_5_1B02
        add    esp, 4
        inc    eax
        cmp    byte ptr [esi + 32h], 3
        jbe    SHORT loc_5_211D
        add    eax, eax
loc_5_211D:
        mov    ecx, dword ptr [esi + 1ch]
        mov    edx, dword ptr [esp + 10h]
        sub    ecx, eax
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx], ecx
        jmp    SHORT loc_5_2144
loc_5_2130:
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx], 0
loc_5_2144:
        xor    eax, eax
        pop    esi
        ret    0ch
loc_5_214A:
        push   ebx
        mov    eax, 80004005h
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        cmp    dword ptr [esi + 8], 0
        jne    NEAR PTR loc_5_2209
        movzx  edi, word ptr [esi + 30h]
        push   0
        push   0ah
        push   edi
        call   sub_5_1B86
        add    esp, 0ch
        lea    edx, [edi + 2]
        xor    al, al
        push   dword ptr [esi + 18h]
        out    dx, al
        call   sub_5_1B1D
        add    esp, 4
        push   dword ptr [esi + 1ch]
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi + 18h]
        call   sub_5_1B41
        add    esp, 0ch
        push   dword ptr [esi + 18h]
        call   sub_5_1B62
        add    esp, 4
        push   dword ptr [esi + 18h]
        call   sub_5_1B2B
        mov    bx, word ptr [esi + 1ch]
        add    esp, 4
        cmp    dword ptr [esi + 24h], 10h
        jne    SHORT loc_5_21B6
        shr    bx, 1
loc_5_21B6:
        cmp    dword ptr [esi + 28h], 2
        jne    SHORT loc_5_21BF
        shr    bx, 1
loc_5_21BF:
        dec    bx
        push   ebx
        push   0fh
        shr    bx, 8
        push   edi
        call   sub_5_1B86
        add    esp, 0ch
        push   ebx
        push   0eh
        push   edi
        call   sub_5_1B86
        add    esp, 0ch
        mov    al, 1
        test   byte ptr [esi + 2eh], 40h
        jne    SHORT loc_5_21E7
        mov    al, 5
loc_5_21E7:
        push   eax
        push   9
        push   edi
        call   sub_5_1B86
        add    esp, 0ch
        push   2
        push   0ah
        push   edi
        call   sub_5_1B86
        add    esp, 0ch
        xor    eax, eax
        mov    dword ptr [esi + 8], 1
loc_5_2209:
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
loc_5_220F:
        push   esi
        mov    eax, 80004005h
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        cmp    dword ptr [edi + 8], 0
        je     SHORT loc_5_2264
        movzx  esi, word ptr [edi + 30h]
        push   0ah
        push   esi
        call   sub_5_1BA4
        add    esp, 8
        and    al, 0c0h
        push   eax
        push   0ah
        push   esi
        call   sub_5_1B86
        add    esp, 0ch
        lea    edx, [esi + 2]
        xor    al, al
        push   0
        out    dx, al
        push   9
        push   esi
        call   sub_5_1B86
        add    esp, 0ch
        push   dword ptr [edi + 18h]
        call   sub_5_1B1D
        add    esp, 4
        xor    eax, eax
        mov    dword ptr [edi + 8], 0
loc_5_2264:
        pop    edi
        pop    esi
        ret    4
        db  0CCh, 0CCh, 0CCh
sub_5_226C:
        db  0CDh, 020h, 074h, 081h, 001h, 000h
        int3
        int3
sub_5_2274:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        int3
        int3
sub_5_227C:
        db  0CDh, 020h, 04Bh, 081h, 001h, 000h
        int3
        int3
sub_5_2284:
        db  0CDh, 020h, 052h, 081h, 001h, 000h
        int3
        int3
sub_5_228C:
        db  0CDh, 020h, 04Ah, 081h, 001h, 000h
        int3
        int3
sub_5_2294:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        int3
        int3
sub_5_229C:
        db  0CDh, 020h, 051h, 081h, 001h, 000h
        int3
        int3
sub_5_22A4:
        db  0CDh, 020h, 03Dh, 080h, 033h, 000h
        int3
        int3
sub_5_22AC:
        db  0CDh, 020h, 03Eh, 080h, 033h, 000h
        int3
        int3
sub_5_22B4:
        db  0CDh, 020h, 03Bh, 080h, 033h, 000h
        int3
        int3
sub_5_22BC:
        db  0CDh, 020h, 044h, 080h, 033h, 000h
        int3
        int3
sub_5_22C4:
        db  0CDh, 020h, 003h, 080h, 033h, 000h
        int3
        int3
sub_5_22CC:
        db  0CDh, 020h, 00Fh, 080h, 033h, 000h
        int3
        int3
sub_5_22D4:
        db  0CDh, 020h, 01Bh, 080h, 033h, 000h
        int3
        int3
sub_5_22DC:
        db  0CDh, 020h, 01Fh, 080h, 033h, 000h
        int3
        int3
sub_5_22E4:
        db  0CDh, 020h, 01Ah, 080h, 033h, 000h
        int3
        int3
sub_5_22EC:
        db  0CDh, 020h, 01Ch, 080h, 033h, 000h
        int3
        int3
sub_5_22F4:
        db  0CDh, 020h, 018h, 080h, 033h, 000h
        int3
        int3
sub_5_22FC:
        db  0CDh, 020h, 00Bh, 080h, 033h, 000h
        int3
        int3
sub_5_2304:
        db  0CDh, 020h, 004h, 080h, 033h, 000h
        int3
        int3
sub_5_230C:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_231C:
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
sub_5_2330:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_2340:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_2350:
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
sub_5_2364:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_2374:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_2384:
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
sub_5_2398:
        db  0CDh, 020h, 07Bh, 081h, 001h, 000h
        int3
        int3
sub_5_23A0:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        int3
        int3
sub_5_23A8:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        int3
        int3
sub_5_23B0:
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
sub_5_23C4:
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 0ch]
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        jb     SHORT loc_5_23E2
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_5_23E2:
        ret    10h
_PTEXT ends
        end
