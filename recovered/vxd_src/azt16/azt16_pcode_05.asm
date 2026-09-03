; azt16_pcode_05.asm -- AZT16 object 5 segment _PTEXT
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
        call   sub_5_1CC4
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
        call   sub_5_1CC4
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
        call   sub_5_1CFC
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
        call   sub_5_1CEC
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
        call   sub_5_1CFC
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
        call   sub_5_1CEC
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
        call   sub_5_1CF4
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
        call   sub_5_1CEC
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_01E2
        lea    eax, [ebp - 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 10h]
        call   sub_5_1CE4
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
        call   sub_5_1CDC
        add    esp, 18h
        push   dword ptr [ebp - 8]
        call   sub_5_1CCC
        mov    dword ptr [ebp - 8], 0
        add    esp, 4
loc_5_021F:
        push   dword ptr [ebp - 0ch]
        call   sub_5_1CCC
        add    esp, 4
        lea    eax, [ebp - 114h]
        mov    dword ptr [ebp - 0ch], 0
        push   eax
        push   80000002h
        call   sub_5_1CD4
        add    esp, 8
loc_5_0245:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_5_0256
        push   dword ptr [ebp - 0ch]
        call   sub_5_1CCC
        add    esp, 4
loc_5_0256:
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_5_0267
        push   dword ptr [ebp - 10h]
        call   sub_5_1CCC
        add    esp, 4
loc_5_0267:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_5_0278
        push   dword ptr [ebp - 8]
loc_5_0270:
        call   sub_5_1CCC
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
        call   sub_5_1D04
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
        push   0
        lea    ecx, [ebp - 0d8h]
        push   dword ptr [ebp + 8]
        push   ecx
        call   sub_5_1D0C
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_5_039E
        cmp    word ptr [ebp - 7ch], 3
        jne    NEAR PTR loc_5_0399
        cmp    word ptr [ebp - 52h], 10h
        jne    NEAR PTR loc_5_0399
        cmp    word ptr [ebp - 50h], 8
        jne    NEAR PTR loc_5_0399
        cmp    word ptr [ebp - 4eh], 4
        jne    SHORT loc_5_0399
        mov    cx, word ptr [ebp - 7ah]
        mov    eax, dword ptr [ebp + 1ch]
        mov    dx, word ptr [ebp - 78h]
        mov    word ptr [eax], cx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ax, dx
        add    ax, 4
        mov    word ptr [ecx], dx
        mov    ecx, dword ptr [ebp + 14h]
        cmp    word ptr [ebp - 2ah], 2
        mov    dx, word ptr [ebp - 76h]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax], dx
        jb     SHORT loc_5_0399
        cmp    word ptr [ebp - 1ah], 2
        jb     SHORT loc_5_0399
        mov    eax, dword ptr [ebp + 24h]
        movzx  dx, byte ptr [ebp - 28h]
        mov    word ptr [eax], dx
        mov    eax, dword ptr [ebp + 20h]
        movzx  dx, byte ptr [ebp - 27h]
        mov    word ptr [eax], dx
        mov    eax, dword ptr [ebp + 30h]
        movzx  dx, byte ptr [ebp - 18h]
        mov    word ptr [eax], dx
        mov    eax, dword ptr [ebp + 28h]
        movzx  dx, byte ptr [ebp - 17h]
        mov    word ptr [eax], dx
        mov    eax, dword ptr [ebp + 2ch]
        mov    word ptr [eax], dx
        movzx  eax, word ptr [ecx]
        inc    eax
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 13h
        jmp    SHORT loc_5_039E
loc_5_0399:
        mov    eax, 13h
loc_5_039E:
        mov    esp, ebp
        pop    ebp
        ret
sub_5_03A2:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 16ch
        test   eax, eax
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_5_03F1
        cmp    eax, 1
        je     NEAR PTR loc_5_0541
        cmp    eax, 2
        je     NEAR PTR loc_5_06CA
        cmp    eax, 3
        je     NEAR PTR loc_5_0717
        cmp    eax, 4
        je     NEAR PTR loc_5_06CA
        cmp    eax, 8
        je     NEAR PTR loc_5_075B
        cmp    eax, 0dh
        je     NEAR PTR loc_5_06CA
        jmp    NEAR PTR loc_5_078E
loc_5_03F1:
        mov    di, 0ffffh
        xor    ebx, ebx
        push   ebx
        lea    eax, [ebp - 4ch]
        push   ebx
        lea    ecx, [ebp - 34h]
        push   eax
        mov    esi, dword ptr [ebp + 10h]
        push   ecx
        mov    dword ptr [ebp - 20h], ebx
        push   esi
        mov    dword ptr [ebp - 4ch], ebx
        call   sub_5_1D14
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_5_041A
        mov    dword ptr [ebp - 34h], ebx
loc_5_041A:
        cmp    dword ptr [ebp - 34h], 1
        je     SHORT loc_5_042D
        cmp    dword ptr [ebp - 34h], 0
        je     SHORT loc_5_042D
        xor    eax, eax
        jmp    NEAR PTR loc_5_0793
loc_5_042D:
        push   1
        lea    eax, [ebp - 28h]
        push   esi
        push   eax
        call   sub_5_1D2C
        add    esp, 0ch
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_5_0449
        mov    eax, ebx
        jmp    NEAR PTR loc_5_0793
loc_5_0449:
        push   0
        lea    eax, [ebp - 1ch]
        push   eax
        lea    edx, [ebp - 18h]
        push   0
        push   dword ptr [ebp - 28h]
        push   edx
        call   sub_5_1D24
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_5_0529
        xor    esi, esi
        test   ebx, ebx
        jne    SHORT loc_5_04B4
loc_5_046E:
        mov    eax, dword ptr [ebp - 1ch]
        cmp    eax, 2
        je     SHORT loc_5_047D
        cmp    eax, 4
        je     SHORT loc_5_0494
        jmp    SHORT loc_5_0497
loc_5_047D:
        mov    eax, dword ptr [ebp - 18h]
        cmp    word ptr [eax + 0eh], 8
        jne    SHORT loc_5_0497
        mov    dword ptr [ebp - 20h], 1
        mov    di, word ptr [eax + 10h]
        jmp    SHORT loc_5_0497
loc_5_0494:
        mov    esi, dword ptr [ebp - 18h]
loc_5_0497:
        push   0
        lea    eax, [ebp - 1ch]
        push   eax
        lea    edx, [ebp - 18h]
        push   0
        push   dword ptr [ebp - 18h]
        push   edx
        call   sub_5_1D24
        add    esp, 14h
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_5_046E
loc_5_04B4:
        cmp    ebx, 0fh
        jne    SHORT loc_5_0533
        cmp    dword ptr [ebp - 20h], 0
        je     SHORT loc_5_053A
        test   esi, esi
        je     SHORT loc_5_053A
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_04C9:
        add    esp, 4
        test   eax, eax
        je     SHORT loc_5_053A
        xor    ebx, ebx
loc_5_04D2:
        mov    edx, 1
        mov    cl, bl
        shl    edx, cl
        movzx  eax, word ptr [esi + 4]
        test   edx, eax
        je     SHORT loc_5_0501
        push   ebx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_04EA:
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_5_0501
        mov    ax, 1
        mov    cl, bl
        shl    ax, cl
        not    ax
        and    word ptr [esi + 4], ax
loc_5_0501:
        inc    ebx
        cmp    ebx, 10h
        jl     SHORT loc_5_04D2
        push   0
        lea    ecx, [ebp - 28h]
        push   dword ptr [ebp - 28h]
        push   ecx
        call   sub_5_1D1C
        add    esp, 0ch
        mov    ebx, eax
        test   ebx, ebx
        je     NEAR PTR loc_5_0449
        xor    eax, eax
        jmp    NEAR PTR loc_5_0793
loc_5_0529:
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0793
loc_5_0533:
        mov    eax, ebx
        jmp    NEAR PTR loc_5_0793
loc_5_053A:
        xor    eax, eax
        jmp    NEAR PTR loc_5_0793
loc_5_0541:
        push   0
        lea    eax, [ebp - 16ch]
        push   100h
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 2ch], 0
        push   eax
        push   0
        push   esi
        call   sub_5_1CFC
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_5_0793
        xor    edi, edi
        lea    eax, [ebp - 48h]
        push   edi
        lea    ecx, [ebp - 30h]
        push   edi
        mov    dword ptr [ebp - 48h], edi
        push   eax
        push   ecx
        push   esi
        call   sub_5_1D14
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_5_058B
        mov    dword ptr [ebp - 30h], edi
loc_5_058B:
        mov    eax, 0ffffh
        mov    word ptr [ebp - 6], ax
        mov    word ptr [ebp - 0ch], ax
        mov    word ptr [ebp - 0ah], ax
        mov    word ptr [ebp - 0eh], ax
        mov    word ptr [ebp - 2], ax
        mov    eax, dword ptr [ebp - 30h]
        cmp    eax, 10h
        je     SHORT loc_5_05B1
        cmp    eax, 40h
        jne    SHORT loc_5_05C9
loc_5_05B1:
        mov    eax, dword ptr [ebp - 30h]
        sub    eax, 10h
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0fffffd00h
        add    eax, 400h
        or     dword ptr [ebp - 2ch], eax
loc_5_05C9:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 14h]
        push   eax
        lea    edx, [ebp - 12h]
        push   ecx
        lea    eax, [ebp - 4]
        push   edx
        lea    ecx, [ebp - 10h]
        push   eax
        lea    edx, [ebp - 2]
        push   ecx
        lea    eax, [ebp - 0eh]
        push   edx
        lea    ecx, [ebp - 0ah]
        push   eax
        lea    edx, [ebp - 0ch]
        push   ecx
        lea    eax, [ebp - 6]
        push   edx
        push   eax
        push   esi
        call   sub_5_02D3
        add    esp, 2ch
        test   eax, eax
        jne    NEAR PTR loc_5_0793
        push   esi
        call   sub_5_00AB
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_5_0793
        lea    eax, [ebp - 2ch]
        push   eax
        push   esi
        call   sub_5_0288
        add    esp, 8
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 6]
        push   dword ptr [ebp - 2]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0631:
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_5_0793
        push   dword ptr [ebp - 2ch]
        push   20h
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 12h]
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 2]
        push   dword ptr [ebp - 0eh]
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 6]
        push   dword ptr [ebp - 0ah]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_065F:
        add    esp, 2ch
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_5_06C3
        cmp    word ptr [ebp - 4], 9
        mov    eax, 2
        je     SHORT loc_5_0678
        movzx  eax, word ptr [ebp - 4]
loc_5_0678:
        movzx  ecx, word ptr [ebp - 8]
        movzx  edx, word ptr [ebp - 2]
        push   ecx
        push   eax
        push   edx
        lea    ecx, [ebp - 6ch]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_5_1CC4
        add    esp, 14h
        lea    ecx, [ebp - 6ch]
        db  0C7h, 045h, 0C0h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 44h], ecx
        push   dword ptr [ebp - 44h]
        push   dword ptr [ebp - 40h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 002h, 000h, 04Ah, 004h
        add    esp, 8
        test   edi, edi
        jne    SHORT loc_5_06C3
        push   esi
        call   sub_5_1914
        add    esp, 4
loc_5_06C3:
        mov    eax, edi
        jmp    NEAR PTR loc_5_0793
loc_5_06CA:
        lea    eax, [ebp - 3ch]
        mov    esi, dword ptr [ebp + 10h]
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_06D7:
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_5_078E
        test   byte ptr [ebp - 3bh], 10h
        je     SHORT loc_5_0701
        db  0C7h, 045h, 0C8h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 38h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 004h, 000h, 04Ah, 004h
        add    esp, 4
loc_5_0701:
        push   esi
        call   sub_5_19EF
        add    esp, 4
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0710:
        add    esp, 4
        xor    eax, eax
        jmp    SHORT loc_5_0793
loc_5_0717:
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        je     SHORT loc_5_0725
        cmp    eax, 1
        je     SHORT loc_5_0740
        jmp    SHORT loc_5_078E
loc_5_0725:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_072D:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 13h
        jmp    SHORT loc_5_0793
loc_5_0740:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0748:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 13h
        jmp    SHORT loc_5_0793
loc_5_075B:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 7fffffffh
        cmp    eax, 4
        jb     SHORT loc_5_078E
        cmp    eax, 5
        jbe    SHORT loc_5_0779
        cmp    eax, 6
        jb     SHORT loc_5_078E
        cmp    eax, 8
        jbe    SHORT loc_5_0783
        jmp    SHORT loc_5_078E
loc_5_0779:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0781:
        jmp    SHORT loc_5_078B
loc_5_0783:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_078B:
        add    esp, 4
loc_5_078E:
        mov    eax, 1
loc_5_0793:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  0CCh, 0CCh
loc_5_079C:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_5_07A6
        jmp    SHORT loc_5_07B8
loc_5_07A6:
        cmp    ecx, 4
        jbe    SHORT loc_5_07B1
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_5_07B1:
        je     SHORT loc_5_07B4
        in     al, dx
loc_5_07B4:
        ret
        db  02Eh, 08Bh, 0C0h
loc_5_07B8:
        push   eax
        mov    eax, dword ptr [esi + 24h]
        cmp    eax, ebx
        je     SHORT loc_5_07D8
        or     eax, eax
        jne    SHORT loc_5_07EB
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 1
        call   sub_5_0C76
        pop    edx
        pop    ecx
        pop    ebx
        jb     SHORT loc_5_07EB
loc_5_07D8:
        pop    eax
        cmp    ecx, 4
        jbe    SHORT loc_5_07E4
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_5_07E4:
        je     SHORT loc_5_07E9
        in     al, dx
        jmp    SHORT loc_5_07EA
loc_5_07E9:
        out    dx, al
loc_5_07EA:
        ret
loc_5_07EB:
        mov    eax, 1
        mov    edi, esi
        call   sub_5_0AC1
        pop    eax
        xor    eax, eax
        dec    eax
        ret
loc_5_07FC:
        push   eax
        mov    eax, dword ptr [esi + 24h]
        cmp    eax, ebx
        je     SHORT loc_5_081C
        or     eax, eax
        jne    SHORT loc_5_0836
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 1
        call   sub_5_0C76
        pop    edx
        pop    ecx
        pop    ebx
        jb     SHORT loc_5_0836
loc_5_081C:
        pop    eax
        mov    edi, esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0824:
        cmp    ecx, 4
        jbe    SHORT loc_5_082F
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_5_082F:
        je     SHORT loc_5_0834
        in     al, dx
        jmp    SHORT loc_5_0835
loc_5_0834:
        out    dx, al
loc_5_0835:
        ret
loc_5_0836:
        mov    eax, 1
        mov    edi, esi
        call   sub_5_0AC1
        pop    eax
        xor    eax, eax
        dec    eax
        ret
sub_5_0847:
        push   eax
        push   edx
        push   ebx
        push   ecx
        movzx  edx, word ptr [edi + 22h]
        mov    ebx, 105h
        mov    al, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_085B:
        mov    ebx, 4
        mov    al, 60h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0867:
        mov    ebx, 104h
        mov    al, 3fh
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0873:
        mov    ebx, 8
        xor    al, al
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_087F:
        mov    ebx, 0bdh
        mov    al, 0c0h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_088B:
        mov    ecx, 15h
        mov    ebx, 40h
        mov    al, 3fh
loc_5_0897:
        xor    bh, bh
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_089E:
        mov    bh, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_08A5:
        inc    bl
        loop   loc_5_0897
        mov    ecx, 8
        mov    ebx, 0b0h
        xor    eax, eax
loc_5_08B5:
        xor    bh, bh
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_08BC:
        mov    bh, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_08C3:
        inc    bl
        loop   loc_5_08B5
        mov    ebx, 104h
        xor    eax, eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_08D3:
        mov    ebx, 105h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_08DD:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_08E2:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_08E7:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_08EC:
        pop    ecx
        pop    ebx
        pop    edx
        pop    eax
        ret
loc_5_08F1:
        mov    edi, edx
        mov    ebx, dword ptr [edi + 30h]
        or     ebx, ebx
        je     SHORT loc_5_0905
        movzx  edx, word ptr [edi + 22h]
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        ret
loc_5_0905:
        ret
        db  08Bh, 0C0h
loc_5_0908:
        test   word ptr [esi + 4], 2000h
        je     SHORT loc_5_0930
        cmp    ecx, 4
        jbe    SHORT loc_5_091B
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_5_091B:
        je     SHORT loc_5_0966
        and    word ptr [esi + 4], 0dfffh
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        mov    al, byte ptr [esi + 0ach]
        ret
loc_5_0930:
        push   eax
        mov    eax, dword ptr [esi + 30h]
        cmp    eax, ebx
        je     SHORT loc_5_0955
        or     eax, eax
        jne    SHORT loc_5_0968
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 2
        call   sub_5_0C76
        pop    edx
        pop    ecx
        pop    ebx
        jb     SHORT loc_5_0968
        call   sub_5_0847
loc_5_0955:
        pop    eax
        cmp    ecx, 4
        jbe    SHORT loc_5_0961
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_5_0961:
        je     SHORT loc_5_0966
        in     al, dx
        jmp    SHORT loc_5_0967
loc_5_0966:
        out    dx, al
loc_5_0967:
        ret
loc_5_0968:
        mov    eax, 2
        mov    edi, esi
        call   sub_5_0AC1
        pop    eax
        xor    eax, eax
        dec    eax
        ret
sub_5_0979:
        push   esi
        push   eax
        push   ecx
        push   edx
        test   word ptr [edi + 52h], 4
        je     SHORT loc_5_09A0
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_098F:
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_09A0
        inc    cx
        jmp    SHORT loc_5_098F
loc_5_09A0:
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 42h]
loc_5_09AA:
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_09BB
        inc    cx
        jmp    SHORT loc_5_09AA
loc_5_09BB:
        test   word ptr [edi + 4], 1000h
        je     SHORT loc_5_09DE
        mov    si, 0fh
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 40h]
loc_5_09CD:
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_09DE
        inc    cx
        jmp    SHORT loc_5_09CD
loc_5_09DE:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_09E4:
        push   esi
        push   eax
        push   ecx
        push   edx
        test   word ptr [edi + 52h], 4
        je     SHORT loc_5_0A13
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_5_0A13
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_0A02:
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_0A13
        inc    cx
        jmp    SHORT loc_5_0A02
loc_5_0A13:
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 42h]
        add    edx, 0
loc_5_0A20:
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_0A31
        inc    cx
        jmp    SHORT loc_5_0A20
loc_5_0A31:
        test   word ptr [edi + 4], 1000h
        je     SHORT loc_5_0A54
        mov    si, 0fh
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 40h]
loc_5_0A43:
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_0A54
        inc    cx
        jmp    SHORT loc_5_0A43
loc_5_0A54:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_0A5A:
        push   eax
        push   ebx
        push   edx
        movzx  edx, word ptr [edi + 22h]
        mov    ebx, 4
        mov    eax, 60h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0A70:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0A75:
        pop    edx
        pop    ebx
        pop    eax
        ret
sub_5_0A79:
        push   esi
        push   eax
        push   ecx
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 22h]
loc_5_0A86:
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_0A97
        inc    cx
        jmp    SHORT loc_5_0A86
loc_5_0A97:
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_0A9C:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 22h]
loc_5_0AAA:
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_0ABB
        inc    cx
        jmp    SHORT loc_5_0AAA
loc_5_0ABB:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_0AC1:
        test   word ptr [edi + 4], 1
        jne    SHORT loc_5_0B16
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0ACE:
        jb     SHORT loc_5_0B16
        cmp    eax, 2
        je     SHORT loc_5_0AEF
        test   dword ptr [esi], 1
        jne    SHORT loc_5_0B16
        or     dword ptr [esi], 1
        db  00Fh, 0B7h, 00Dh, 00Ch, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        jmp    SHORT loc_5_0B07
loc_5_0AEF:
        test   dword ptr [esi], 2
        jne    SHORT loc_5_0B16
        or     dword ptr [esi], 2
        db  00Fh, 0B7h, 00Dh, 00Eh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
loc_5_0B07:
        mov    eax, 30h
        xor    esi, esi
        xor    edi, edi
        db  0CDh, 020h, 004h, 000h, 017h, 000h
loc_5_0B16:
        ret
loc_5_0B17:
        pushfd
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_0B65
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_5_0B65
loc_5_0B2B:
        popfd
        pushfd
        push   eax
        mov    edi, dword ptr [eax]
        xor    eax, eax
        cmp    dword ptr [edi + 24h], ebx
        jne    SHORT loc_5_0B41
        or     eax, 1
        mov    dword ptr [edi + 28h], 0ffffffffh
loc_5_0B41:
        cmp    dword ptr [edi + 30h], ebx
        jne    SHORT loc_5_0B50
        or     eax, 2
        mov    dword ptr [edi + 34h], 0ffffffffh
loc_5_0B50:
        or     eax, eax
        je     SHORT loc_5_0B59
        call   sub_5_0BE0
loc_5_0B59:
        pop    eax
        cli
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_5_0B65
        jmp    SHORT loc_5_0B2B
loc_5_0B65:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        xor    esi, esi
        xchg   dword ptr [edi + ebx], esi
        or     esi, esi
        je     SHORT loc_5_0B7A
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_5_0B7A:
        popfd
        clc
        ret
loc_5_0B7D:
        pushad
        pushfd
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    eax, 5
        mov    ecx, 34h
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_5_0BD8
        mov    dword ptr [edi + ebx], esi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_0BC3
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        sti
        je     SHORT loc_5_0BC3
loc_5_0BAE:
        mov    edi, dword ptr [eax]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0BB5:
        jb     SHORT loc_5_0BC5
        cli
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        sti
        je     SHORT loc_5_0BC3
        jmp    SHORT loc_5_0BAE
loc_5_0BC3:
        jmp    SHORT loc_5_0BDC
loc_5_0BC5:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     SHORT loc_5_0BD8
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_5_0BD8:
        popfd
        stc
        jmp    SHORT loc_5_0BDE
loc_5_0BDC:
        popfd
        clc
loc_5_0BDE:
        popad
        ret
sub_5_0BE0:
        push   eax
        push   edx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0BE8:
        jb     NEAR PTR loc_5_0C71
        test   eax, 2
        je     SHORT loc_5_0C0E
        cmp    dword ptr [edi + 30h], ebx
        jne    SHORT loc_5_0C0E
        call   sub_5_0A5A
        call   sub_5_0A79
        mov    dword ptr [edi + 30h], 0
        and    dword ptr [esi], 0fffffffdh
loc_5_0C0E:
        test   eax, 1
        je     SHORT loc_5_0C71
        cmp    dword ptr [edi + 24h], ebx
        jne    SHORT loc_5_0C71
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_5_0C40
        test   word ptr [edi + 4], 200h
        jne    SHORT loc_5_0C40
        mov    eax, dword ptr [edi + 38h]
        db  0CDh, 020h, 014h, 000h, 004h, 000h
        mov    eax, dword ptr [edi + 3ch]
        or     eax, eax
        je     SHORT loc_5_0C40
        db  0CDh, 020h, 014h, 000h, 004h, 000h
loc_5_0C40:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0C45:
        call   sub_5_0979
        mov    dword ptr [edi + 24h], 0
        mov    eax, dword ptr [edi + 2ch]
        test   word ptr [edi + 4], 8
        jne    SHORT loc_5_0C68
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 000h, 003h, 000h
loc_5_0C68:
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        and    dword ptr [esi], 0fffffffeh
loc_5_0C71:
        pop    esi
        pop    edx
        pop    eax
        clc
        ret
sub_5_0C76:
        push   eax
        push   ecx
        push   edx
        push   esi
        test   eax, 2
        je     SHORT loc_5_0C99
        cmp    dword ptr [edi + 30h], 0
        je     SHORT loc_5_0C8E
        cmp    dword ptr [edi + 30h], ebx
        je     SHORT loc_5_0C99
        jmp    SHORT loc_5_0CB3
loc_5_0C8E:
        mov    dword ptr [edi + 30h], ebx
        call   sub_5_0A9C
        mov    dword ptr [edi + 34h], ebx
loc_5_0C99:
        test   eax, 1
        je     NEAR PTR loc_5_0D3E
        cmp    dword ptr [edi + 24h], 0
        je     SHORT loc_5_0CBA
        cmp    dword ptr [edi + 24h], ebx
        je     NEAR PTR loc_5_0D3D
loc_5_0CB3:
        stc
        jb     NEAR PTR loc_5_0D3E
loc_5_0CBA:
        mov    dword ptr [edi + 24h], ebx
        cmp    dword ptr [edi + 28h], ebx
        je     SHORT loc_5_0CE8
        mov    ebx, dword ptr [edi + 28h]
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_5_0CD9
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0CD2:
        jb     SHORT loc_5_0CD9
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0CD9:
        mov    ebx, dword ptr [edi + 24h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0CE1:
        jb     SHORT loc_5_0CE8
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0CE8:
        mov    dword ptr [edi + 28h], ebx
        call   sub_5_09E4
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_5_0D16
        test   word ptr [edi + 4], 200h
        jne    SHORT loc_5_0D16
        mov    eax, dword ptr [edi + 38h]
        db  0CDh, 020h, 013h, 000h, 004h, 000h
        mov    eax, dword ptr [edi + 3ch]
        or     eax, eax
        je     SHORT loc_5_0D16
        db  0CDh, 020h, 013h, 000h, 004h, 000h
loc_5_0D16:
        test   word ptr [edi + 4], 8
        jne    SHORT loc_5_0D3D
        mov    eax, dword ptr [edi + 2ch]
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   ecx, 10h
        je     SHORT loc_5_0D37
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        jmp    SHORT loc_5_0D3D
loc_5_0D37:
        db  0CDh, 020h, 009h, 000h, 003h, 000h
loc_5_0D3D:
        clc
loc_5_0D3E:
        pop    esi
        pop    edx
        pop    ecx
        pop    eax
        ret
sub_5_0D43:
        mov    eax, 400h
        clc
        ret
        db  0CCh, 0CCh
azt16_PM_API:
        movzx  eax, word ptr [ebp + 14h]
        cmp    ah, 3
        jae    SHORT loc_5_0D76
        movzx  ecx, ah
        shl    ecx, 3
        db  08Dh, 089h, 000h, 000h, 000h, 000h
        movzx  eax, al
        cmp    eax, dword ptr [ecx]
        jae    SHORT loc_5_0D76
        mov    ecx, dword ptr [ecx + 4]
        call   dword ptr [ecx + eax*4]
        jb     SHORT loc_5_0D76
        and    word ptr [ebp + 2ch], 0fffeh
        ret
loc_5_0D76:
        or     word ptr [ebp + 2ch], 1
        ret
loc_5_0D7C:
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_0DDD
        pushfd
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        popfd
        or     eax, eax
        je     SHORT loc_5_0DDD
        mov    edi, dword ptr [eax]
        cmp    ebx, dword ptr [edi + 24h]
        jne    SHORT loc_5_0DDD
        xchg   esi, eax
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        cmp    esi, -1
        je     SHORT loc_5_0DDD
        movzx  ecx, word ptr [ebp + 10h]
        movzx  eax, word ptr [ebp + 18h]
        add    ecx, eax
        cmp    ecx, dword ptr [edi + 1ch]
        ja     SHORT loc_5_0DDD
        movzx  ecx, word ptr [ebp + 18h]
        mov    edi, dword ptr [edi + 18h]
        movzx  eax, word ptr [ebp + 10h]
        add    edi, eax
        or     ecx, ecx
        je     SHORT loc_5_0DDD
        cld
        shr    ecx, 1
        rep movsw
        adc    cl, cl
        rep movsb
        mov    word ptr [ebp + 1ch], 1
        clc
        ret
loc_5_0DDD:
        mov    word ptr [ebp + 1ch], 0
        stc
        ret
loc_5_0DE5:
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_0E46
        pushfd
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        popfd
        or     eax, eax
        je     SHORT loc_5_0E46
        mov    esi, dword ptr [eax]
        cmp    ebx, dword ptr [esi + 24h]
        jne    SHORT loc_5_0E46
        xchg   edi, eax
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cmp    edi, -1
        je     SHORT loc_5_0E46
        movzx  ecx, word ptr [ebp + 10h]
        movzx  eax, word ptr [ebp + 18h]
        add    ecx, eax
        cmp    ecx, dword ptr [esi + 1ch]
        ja     SHORT loc_5_0E46
        movzx  ecx, word ptr [ebp + 18h]
        mov    esi, dword ptr [esi + 18h]
        movzx  eax, word ptr [ebp + 10h]
        add    esi, eax
        or     ecx, ecx
        je     SHORT loc_5_0E46
        cld
        shr    ecx, 1
        rep movsw
        adc    cl, cl
        rep movsb
        mov    word ptr [ebp + 1ch], 1
        clc
        ret
loc_5_0E46:
        mov    word ptr [ebp + 1ch], 0
        stc
        ret
loc_5_0E4E:
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E5A:
        add    esp, 8
        jb     SHORT loc_5_0E8E
        movzx  eax, word ptr [ebp + 10h]
        or     eax, eax
        je     SHORT loc_5_0E8E
        cmp    eax, 1
        jne    SHORT loc_5_0E71
        mov    eax, dword ptr [edi + 38h]
        jmp    SHORT loc_5_0E7B
loc_5_0E71:
        mov    eax, dword ptr [edi + 3ch]
        or     eax, eax
        jne    SHORT loc_5_0E7B
        mov    eax, dword ptr [edi + 38h]
loc_5_0E7B:
        or     eax, eax
        je     SHORT loc_5_0E8E
        push   ecx
        db  0CDh, 020h, 01Ch, 000h, 004h, 000h
        mov    word ptr [ebp + 1ch], cx
        pop    ecx
        clc
        jmp    SHORT loc_5_0E95
loc_5_0E8E:
        mov    word ptr [ebp + 1ch], 0ffffh
        stc
loc_5_0E95:
        pop    edi
        ret
loc_5_0E97:
        call   sub_5_0D43
        mov    word ptr [ebp + 1ch], ax
        clc
        ret
loc_5_0EA2:
        movzx  eax, word ptr [ebp + 10h]
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_0EF6
        test   eax, 1
        je     SHORT loc_5_0EDB
        push   4
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0EBD:
        add    esp, 8
        or     edi, edi
        jne    SHORT loc_5_0ED6
        add    edx, 4
        push   4
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0ECF:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_0EF6
loc_5_0ED6:
        mov    ecx, dword ptr [edi + 28h]
        jmp    SHORT loc_5_0EFE
loc_5_0EDB:
        test   eax, 2
        je     SHORT loc_5_0EF6
        push   5
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0EEA:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_0EF6
        mov    ecx, dword ptr [edi + 34h]
        jmp    SHORT loc_5_0EFE
loc_5_0EF6:
        mov    word ptr [ebp + 1ch], 1
loc_5_0EFC:
        stc
        ret
loc_5_0EFE:
        mov    word ptr [ebp + 1ch], 0
        call   sub_5_0C76
        jb     SHORT loc_5_0F0C
        ret
loc_5_0F0C:
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_5_0EFC
loc_5_0F14:
        movzx  eax, word ptr [ebp + 10h]
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_0F70
        test   eax, 1
        je     SHORT loc_5_0F55
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_5_0F41
        push   4
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0F37:
        add    esp, 8
        or     edi, edi
        jne    SHORT loc_5_0F50
        add    edx, 4
loc_5_0F41:
        push   4
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0F49:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_0F70
loc_5_0F50:
        mov    ecx, dword ptr [edi + 24h]
        jmp    SHORT loc_5_0F78
loc_5_0F55:
        test   eax, 2
        je     SHORT loc_5_0F70
        push   5
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0F64:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_0F70
        mov    ecx, dword ptr [edi + 30h]
        jmp    SHORT loc_5_0F78
loc_5_0F70:
        mov    word ptr [ebp + 1ch], 1
loc_5_0F76:
        stc
        ret
loc_5_0F78:
        cmp    ecx, ebx
        je     SHORT loc_5_0F84
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_5_0F76
loc_5_0F84:
        call   sub_5_0BE0
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
loc_5_0F91:
        pushfd
        mov    ax, word ptr [ebp + 1ch]
        and    ax, 0fffeh
        jne    SHORT loc_5_1002
        xor    ecx, ecx
        mov    ax, word ptr [ebp + 1ch]
        and    ax, 1
        je     SHORT loc_5_0FAB
        mov    ecx, dword ptr [ebp + 18h]
loc_5_0FAB:
        cli
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_1002
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_5_1002
        mov    edi, dword ptr [eax]
        jecxz  loc_5_0FD3
loc_5_0FC2:
        cmp    dword ptr [edi + 54h], ecx
        je     SHORT loc_5_0FD3
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_5_1002
        mov    edi, dword ptr [eax]
        jmp    SHORT loc_5_0FC2
loc_5_0FD3:
        popfd
        pushfd
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cmp    edi, -1
        je     SHORT loc_5_1002
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     SHORT loc_5_1002
        cmp    ecx, 0beh
        ja     SHORT loc_5_1002
        cld
        rep movsb
        mov    word ptr [ebp + 1ch], 1
        popfd
        clc
        ret
loc_5_1002:
        mov    word ptr [ebp + 1ch], 0
        popfd
        stc
        ret
loc_5_100B:
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_1040
        push   5
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_101B:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_1040
        mov    ecx, dword ptr [edi + 34h]
        mov    word ptr [ebp + 1ch], 0
        mov    eax, 2
        call   sub_5_0C76
        jb     SHORT loc_5_1038
        ret
loc_5_1038:
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_5_1046
loc_5_1040:
        mov    word ptr [ebp + 1ch], 1
loc_5_1046:
        stc
        ret
loc_5_1048:
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_1080
        push   5
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1058:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_1080
        mov    ecx, dword ptr [edi + 30h]
        cmp    ecx, ebx
        je     SHORT loc_5_106E
        mov    word ptr [ebp + 1ch], 3
        jmp    SHORT loc_5_1086
loc_5_106E:
        mov    eax, 2
        call   sub_5_0BE0
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
loc_5_1080:
        mov    word ptr [ebp + 1ch], 1
loc_5_1086:
        stc
        ret
loc_5_1088:
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
        mov    edi, dword ptr [eax]
loc_5_10AB:
        cmp    dword ptr [edi + 54h], ecx
        je     SHORT loc_5_10BC
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_5_1101
        mov    edi, dword ptr [eax]
        jmp    SHORT loc_5_10AB
loc_5_10BC:
        popfd
        pushfd
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cmp    edi, -1
        je     SHORT loc_5_1101
        test   word ptr [esi + 52h], 8
        je     SHORT loc_5_1101
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     SHORT loc_5_1101
        cmp    ecx, 1ch
        ja     SHORT loc_5_1101
        mov    word ptr [edi + 4], 1
        mov    ax, word ptr [esi + 22h]
        mov    word ptr [edi + 6], ax
        mov    eax, dword ptr [esi + 54h]
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
        lea    eax, [edi + 44h]
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
        lea    eax, [edi + 44h]
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
        call   sub_5_1D78
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
        call   sub_5_1D68
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
        call   sub_5_1D58
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
        call   sub_5_1D44
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
        call   sub_5_1D34
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
        call   sub_5_1DAC
        push   edi
        push   dword ptr [esi]
        call   sub_5_1D9C
        push   dword ptr [esi]
        call   sub_5_1D68
        test   eax, eax
        jne    SHORT loc_5_14C8
        push   dword ptr [esi]
        call   sub_5_1D8C
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
        db  0CCh, 0CCh
sub_5_1530:
        db  00Fh, 0B7h, 005h, 010h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        ret
sub_5_153E:
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
sub_5_1559:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 018h, 000h, 004h, 000h
        pop    ebp
        ret
sub_5_1567:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 019h, 000h, 004h, 000h
        pop    ebx
        pop    ebp
        ret
sub_5_157D:
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
sub_5_159E:
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
sub_5_15C2:
        push   ebp
        mov    ebp, esp
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    ah, byte ptr [ebp + 0ch]
        mov    al, byte ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_15D4:
        xor    eax, eax
        jae    SHORT loc_5_15DD
        mov    eax, 1
loc_5_15DD:
        pop    edx
        pop    ebp
        ret
sub_5_15E0:
        push   ebp
        mov    ebp, esp
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    ah, byte ptr [ebp + 0ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_15EF:
        xor    ah, ah
        pop    edx
        pop    ebp
        ret
sub_5_15F4:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1604:
        add    esp, 8
        mov    word ptr [edi + 0bch], 0
        pop    ebx
        pop    edi
        pop    ebp
        ret
sub_5_1614:
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
loc_5_162A:
        add    esp, 8
        mov    eax, dword ptr [edi + 24h]
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        cmp    eax, ebx
        je     SHORT loc_5_163C
        jmp    SHORT loc_5_164A
loc_5_163C:
        mov    word ptr [edi + 0bch], 1
        mov    edx, 0
loc_5_164A:
        mov    eax, edx
        pop    ebx
        pop    edx
        pop    edi
        pop    ebp
        ret
sub_5_1651:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1664:
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
loc_5_16A1:
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        push   edi
        mov    ecx, 10h
        db  0BFh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb
        je     SHORT loc_5_16D6
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        mov    esi, eax
        repe cmpsb
        mov    eax, 80004002h
        jne    SHORT loc_5_16E4
loc_5_16D6:
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_16E4:
        pop    edi
        pop    esi
        ret    0ch
loc_5_16E9:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0ch]
        inc    eax
        mov    dword ptr [ecx + 0ch], eax
        ret    4
loc_5_16F7:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0ch]
        dec    eax
        mov    dword ptr [ecx + 0ch], eax
        ret    4
loc_5_1705:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    dword ptr [esi], 0bh
        call   sub_5_1530
        push   0ffh
        lea    ecx, [esi + 4]
        push   eax
        lea    edi, [esi + 104h]
        push   ecx
        call   sub_5_1DC0
        add    esp, 0ch
        push   0ffh
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        call   sub_5_1DC0
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
        je     SHORT loc_5_1779
loc_5_176E:
        movsx  ecx, byte ptr [edi]
        add    eax, ecx
        inc    edi
        cmp    byte ptr [edi], 0
        jne    SHORT loc_5_176E
loc_5_1779:
        mov    ecx, 0f7h
        sub    edx, edx
        div    ecx
        mov    word ptr [esi + 20ah], dx
        xor    eax, eax
        pop    edi
        pop    esi
        ret    8
loc_5_1790:
        push   esi
        push   edi
        mov    edi, 8878000ah
        mov    esi, dword ptr [esp + 0ch]
        cmp    dword ptr [esi + 0ch], 0
        jne    SHORT loc_5_17B5
        push   dword ptr [esi + 4]
        call   sub_5_1614
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_5_17B5
        xor    edi, edi
        inc    dword ptr [esi + 0ch]
loc_5_17B5:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    4
loc_5_17BC:
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
loc_5_1807:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   ebp
        mov    edi, 8007000eh
        mov    esi, dword ptr [esp + 2ch]
        test   byte ptr [esp + 1ch], 0e0h
        mov    dword ptr [esi], ebx
        jne    NEAR PTR loc_5_18AE
        mov    ebp, dword ptr [esp + 14h]
        cmp    dword ptr [ebp + 10h], ebx
        jne    SHORT loc_5_18A7
        push   201h
        push   33h
        call   sub_5_1DD0
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_5_18A0
        db  0C7h, 003h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 4], ebp
        push   dword ptr [ebp + 4]
        push   ebx
        call   sub_5_1651
        add    esp, 8
        mov    eax, dword ptr [ebx + 14h]
        cmp    eax, 1000h
        jbe    SHORT loc_5_18B7
        cmp    eax, 0ffffh
        jae    SHORT loc_5_18B7
        cmp    dword ptr [ebx + 0ch], 0
        je     SHORT loc_5_18B7
        cmp    dword ptr [ebx + 10h], 0
        je     SHORT loc_5_18B7
        cmp    dword ptr [ebx + 18h], 0
        je     SHORT loc_5_18B7
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 2ch]
        push   ebx
        xor    edi, edi
        call   sub_5_1A57
        mov    eax, dword ptr [esp + 24h]
        add    esp, 0ch
        movzx  ecx, word ptr [eax + 0eh]
        mov    dword ptr [ebx + 24h], ecx
        movzx  edx, word ptr [eax + 2]
        mov    dword ptr [ebx + 28h], edx
        jmp    SHORT loc_5_18B7
loc_5_18A0:
        mov    edi, 8007000eh
        jmp    SHORT loc_5_18B7
loc_5_18A7:
        mov    edi, 8878000ah
        jmp    SHORT loc_5_18B7
loc_5_18AE:
        mov    edi, 8878001eh
        mov    ebp, dword ptr [esp + 14h]
loc_5_18B7:
        test   edi, edi
        je     SHORT loc_5_18CC
        test   ebx, ebx
        je     SHORT loc_5_18D1
        push   0
        push   ebx
        call   sub_5_1DC8
        add    esp, 8
        jmp    SHORT loc_5_18D1
loc_5_18CC:
        mov    dword ptr [esi], ebx
        inc    dword ptr [ebp + 10h]
loc_5_18D1:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    1ch
loc_5_18DA:
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], 0
        mov    eax, 80004001h
        ret    0ch
loc_5_18EC:
        mov    eax, 80004005h
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 0ch]
        dec    edx
        mov    dword ptr [ecx + 0ch], edx
        jne    SHORT loc_5_1911
        cmp    dword ptr [ecx + 10h], 0
        jne    SHORT loc_5_1911
        push   dword ptr [ecx + 4]
        call   sub_5_15F4
        add    esp, 4
        xor    eax, eax
loc_5_1911:
        ret    4
sub_5_1914:
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
        call   sub_5_1D14
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_5_1942
        mov    dword ptr [ebp - 8], esi
loc_5_1942:
        cmp    dword ptr [ebp - 8], 8
        je     NEAR PTR loc_5_19E8
        push   201h
        push   14h
        call   sub_5_1DD0
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_5_19BF
        push   1
        lea    eax, [ebp - 4]
        db  068h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 1ch]
        push   eax
        mov    dword ptr [edi + 4], ebx
        push   ecx
        db  0C7h, 007h, 000h, 000h, 000h, 000h
        call   sub_5_1DEC
        test   eax, eax
        jne    SHORT loc_5_19BF
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
        jne    SHORT loc_5_19BF
        push   edi
        mov    esi, 1
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_19BC:
        add    esp, 8
loc_5_19BF:
        test   esi, esi
        jne    SHORT loc_5_19E8
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_5_19D9
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 0bh]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_5_1DD8
loc_5_19D9:
        test   edi, edi
        je     SHORT loc_5_19E8
        push   0
        push   edi
        call   sub_5_1DC8
        add    esp, 8
loc_5_19E8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_19EF:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1A00:
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_5_1A51
        push   0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1A11:
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
        call   sub_5_1DD8
        push   0
        push   esi
        call   sub_5_1DC8
        add    esp, 8
loc_5_1A51:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_5_1A57:
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 14h]
        cmp    dword ptr [edx], eax
        jbe    SHORT loc_5_1A69
        mov    dword ptr [edx], eax
loc_5_1A69:
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
loc_5_1A89:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 20h]
        dec    esi
        mov    dword ptr [eax + 20h], esi
        jne    SHORT loc_5_1AAE
        cmp    dword ptr [eax + 8], 0
        jne    SHORT loc_5_1AAE
        mov    ecx, dword ptr [eax + 4]
        push   0
        push   eax
        dec    dword ptr [ecx + 10h]
        call   sub_5_1DC8
        add    esp, 8
loc_5_1AAE:
        mov    eax, esi
        pop    esi
        ret    4
loc_5_1AB4:
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        push   edi
        mov    ecx, 10h
        db  0BFh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb
        je     SHORT loc_5_1AE9
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        mov    esi, eax
        repe cmpsb
        mov    eax, 80004002h
        jne    SHORT loc_5_1AF7
loc_5_1AE9:
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_1AF7:
        pop    edi
        pop    esi
        ret    0ch
loc_5_1AFC:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 20h]
        inc    eax
        mov    dword ptr [ecx + 20h], eax
        ret    4
loc_5_1B0A:
        mov    eax, 80004001h
        ret    20h
loc_5_1B12:
        mov    eax, 80004001h
        ret    14h
loc_5_1B1A:
        mov    eax, 80004001h
        ret    8
loc_5_1B22:
        mov    eax, 80004001h
        ret    8
loc_5_1B2A:
        mov    eax, 80004001h
        ret    8
loc_5_1B32:
        mov    eax, 80004001h
        ret    8
loc_5_1B3A:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 8], 1
        jne    SHORT loc_5_1B6C
        push   dword ptr [esi + 18h]
        call   sub_5_153E
        add    esp, 4
        inc    eax
        cmp    byte ptr [esi + 32h], 3
        jbe    SHORT loc_5_1B59
        add    eax, eax
loc_5_1B59:
        mov    ecx, dword ptr [esi + 1ch]
        mov    edx, dword ptr [esp + 10h]
        sub    ecx, eax
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx], ecx
        jmp    SHORT loc_5_1B80
loc_5_1B6C:
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx], 0
loc_5_1B80:
        xor    eax, eax
        pop    esi
        ret    0ch
loc_5_1B86:
        push   ebx
        mov    eax, 80004005h
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        cmp    dword ptr [esi + 8], 0
        jne    NEAR PTR loc_5_1C64
        movzx  edi, word ptr [esi + 30h]
        push   0
        push   0ah
        push   edi
        call   sub_5_15C2
        add    esp, 0ch
        lea    edx, [edi + 2]
        xor    al, al
        push   dword ptr [esi + 18h]
        out    dx, al
        call   sub_5_1559
        add    esp, 4
        push   dword ptr [esi + 1ch]
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi + 18h]
        call   sub_5_157D
        add    esp, 0ch
        push   dword ptr [esi + 18h]
        call   sub_5_159E
        add    esp, 4
        push   dword ptr [esi + 18h]
        call   sub_5_1567
        mov    bx, word ptr [esi + 1ch]
        add    esp, 4
        cmp    dword ptr [esi + 24h], 10h
        jne    SHORT loc_5_1BF2
        shr    bx, 1
loc_5_1BF2:
        cmp    dword ptr [esi + 28h], 2
        jne    SHORT loc_5_1BFB
        shr    bx, 1
loc_5_1BFB:
        dec    bx
        push   ebx
        push   0fh
        push   edi
        call   sub_5_15C2
        add    esp, 0ch
        mov    al, bh
        push   eax
        push   0eh
        push   edi
        call   sub_5_15C2
        add    esp, 0ch
        cmp    word ptr [esi + 2ch], 2
        jne    SHORT loc_5_1C38
        push   ebx
        push   1fh
        push   edi
        mov    bl, bh
        call   sub_5_15C2
        add    esp, 0ch
        push   ebx
        push   1eh
        push   edi
        call   sub_5_15C2
        add    esp, 0ch
loc_5_1C38:
        test   byte ptr [esi + 2eh], 40h
        mov    al, 1
        jne    SHORT loc_5_1C42
        mov    al, 5
loc_5_1C42:
        push   eax
        push   9
        push   edi
        call   sub_5_15C2
        add    esp, 0ch
        push   2
        push   0ah
        push   edi
        call   sub_5_15C2
        add    esp, 0ch
        xor    eax, eax
        mov    dword ptr [esi + 8], 1
loc_5_1C64:
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
loc_5_1C6A:
        push   esi
        mov    eax, 80004005h
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        cmp    dword ptr [edi + 8], 0
        je     SHORT loc_5_1CBF
        movzx  esi, word ptr [edi + 30h]
        push   0ah
        push   esi
        call   sub_5_15E0
        add    esp, 8
        and    al, 0c0h
        push   eax
        push   0ah
        push   esi
        call   sub_5_15C2
        add    esp, 0ch
        lea    edx, [esi + 2]
        xor    al, al
        push   0
        out    dx, al
        push   9
        push   esi
        call   sub_5_15C2
        add    esp, 0ch
        push   dword ptr [edi + 18h]
        call   sub_5_1559
        add    esp, 4
        xor    eax, eax
        mov    dword ptr [edi + 8], 0
loc_5_1CBF:
        pop    edi
        pop    esi
        ret    4
sub_5_1CC4:
        db  0CDh, 020h, 074h, 081h, 001h, 000h
        int3
        int3
sub_5_1CCC:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        int3
        int3
sub_5_1CD4:
        db  0CDh, 020h, 04Bh, 081h, 001h, 000h
        int3
        int3
sub_5_1CDC:
        db  0CDh, 020h, 052h, 081h, 001h, 000h
        int3
        int3
sub_5_1CE4:
        db  0CDh, 020h, 04Ah, 081h, 001h, 000h
        int3
        int3
sub_5_1CEC:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        int3
        int3
sub_5_1CF4:
        db  0CDh, 020h, 051h, 081h, 001h, 000h
        int3
        int3
sub_5_1CFC:
        db  0CDh, 020h, 03Dh, 080h, 033h, 000h
        int3
        int3
sub_5_1D04:
        db  0CDh, 020h, 03Eh, 080h, 033h, 000h
        int3
        int3
sub_5_1D0C:
        db  0CDh, 020h, 03Bh, 080h, 033h, 000h
        int3
        int3
sub_5_1D14:
        db  0CDh, 020h, 044h, 080h, 033h, 000h
        int3
        int3
sub_5_1D1C:
        db  0CDh, 020h, 01Bh, 080h, 033h, 000h
        int3
        int3
sub_5_1D24:
        db  0CDh, 020h, 01Fh, 080h, 033h, 000h
        int3
        int3
sub_5_1D2C:
        db  0CDh, 020h, 01Ah, 080h, 033h, 000h
        int3
        int3
sub_5_1D34:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_1D44:
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
sub_5_1D58:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_1D68:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_1D78:
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
sub_5_1D8C:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_1D9C:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_1DAC:
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
sub_5_1DC0:
        db  0CDh, 020h, 07Bh, 081h, 001h, 000h
        int3
        int3
sub_5_1DC8:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        int3
        int3
sub_5_1DD0:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        int3
        int3
sub_5_1DD8:
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
sub_5_1DEC:
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 0ch]
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        jb     SHORT loc_5_1E0A
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_5_1E0A:
        ret    10h
_PTEXT ends
        end
