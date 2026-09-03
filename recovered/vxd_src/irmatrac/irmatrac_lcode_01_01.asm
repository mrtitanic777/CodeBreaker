; irmatrac_lcode_01_01.asm -- IRMATRAC object 1 segment _LTEXT  (part 1/2)
.386p
_LTEXT segment dword public use32 'LCODE'

        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 23fh]
        mov    eax, 1
        out    dx, ax
        mov    word ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0afh]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 225h]
        and    eax, 0ffffh
        add    eax, 0eh
        mov    word ptr [ebp - 18h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0bfh]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0bbh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_0084
loc_1_005F:
        inc    word ptr [ebp - 10h]
        add    dword ptr [ebp - 0ch], 1bh
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        add    eax, 12h
        mov    word ptr [ebp - 18h], ax
        add    dword ptr [ebp - 4], 100h
        add    dword ptr [ebp - 8], 100h
loc_1_0084:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0a7h]
        and    eax, 0ffffh
        dec    eax
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_01AC
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 23dh]
        mov    ax, word ptr [ebp - 18h]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 243h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_0101
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        mov    dx, word ptr [eax + 233h]
        mov    eax, ecx
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp - 4]
        shr    ebx, 10h
        mov    dx, word ptr [ecx + 233h]
        mov    ax, bx
        out    dx, ax
        jmp    NEAR PTR loc_1_0126
loc_1_0101:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 233h]
        mov    ax, word ptr [ebp - 8]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        shr    ecx, 10h
        mov    dx, word ptr [eax + 233h]
        mov    ax, cx
        out    dx, ax
loc_1_0126:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 14h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0a7h]
        and    eax, 0ffffh
        dec    eax
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_0165
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 1bh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_1_0173
loc_1_0165:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0afh]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx], eax
loc_1_0173:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 225h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        add    ecx, ecx
        lea    ecx, [ecx + ecx*8]
        add    eax, ecx
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 8], eax
        jmp    NEAR PTR loc_1_005F
loc_1_01AC:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0afh]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1efh], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0afh]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1f3h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1ebh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 212h]
        and    ecx, 0ffffh
        add    ecx, 4
        mov    dx, word ptr [eax + 23dh]
        mov    ax, cx
        out    dx, ax
        mov    word ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_020D
loc_1_0209:
        inc    word ptr [ebp - 10h]
loc_1_020D:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0abh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_0336
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 233h]
        mov    ax, word ptr [ecx + 247h]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 243h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_02AF
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 20eh]
        mov    ecx, dword ptr [edx + ecx*4]
        shr    ecx, 10h
        mov    dx, word ptr [eax + 233h]
        mov    ax, cx
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 20eh]
        mov    cx, word ptr [edx + ecx*4]
        and    ecx, 0ffffh
        mov    dx, word ptr [eax + 233h]
        mov    eax, ecx
        out    dx, ax
        jmp    NEAR PTR loc_1_0300
loc_1_02AF:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        shl    ecx, 2
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [edx + 206h]
        mov    dx, word ptr [eax + 233h]
        mov    ax, word ptr [ecx + ebx]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        shl    ecx, 2
        lea    ecx, [ecx + ecx*4]
        mov    edx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [edx + 206h]
        mov    dx, word ptr [eax + 233h]
        mov    ax, word ptr [ecx + ebx + 2]
        out    dx, ax
loc_1_0300:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 233h]
        mov    eax, 200h
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 233h]
        in     ax, dx
        mov    word ptr [ebp - 14h], ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 233h]
        in     ax, dx
        mov    word ptr [ebp - 14h], ax
        jmp    NEAR PTR loc_1_0209
loc_1_0336:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_033D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   cx
        push   ax
        call   sub_1_04C8
        and    al, 4
        pop    cx
        and    cl, 1
        or     al, cl
        call   sub_1_0536
        or     al, 2
        call   sub_1_0536
        xchg   ah, al
        in     al, dx
        nop
        nop
        nop
        nop
        xchg   ah, al
        call   sub_1_0536
        and    al, 5
        call   sub_1_0536
        xchg   ah, al
        and    al, 1
        pop    cx
        jmp    NEAR PTR loc_1_0381
loc_1_0381:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0386:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  08Dh, 035h, 000h, 000h, 000h, 000h
        mov    byte ptr [esi + 1], 40h
        mov    byte ptr [esi], 40h
        mov    word ptr [esi + 2], 50h
        mov    word ptr [esi + 4], 28h
        jmp    NEAR PTR loc_1_03AA
loc_1_03AA:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_03AF:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   ebx
        call   sub_1_04C8
        and    al, 5
        call   sub_1_0536
        or     al, 4
        call   sub_1_0536
        xor    ebx, ebx
        mov    cx, 8
loc_1_03CF:
        shl    bl, 1
        call   sub_1_033D
        or     bl, al
        loop   loc_1_03CF
        mov    al, bl
        pop    ebx
        jmp    NEAR PTR loc_1_03E3
loc_1_03E3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_03E8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   ax
        call   sub_1_04C8
        and    al, 5
        call   sub_1_0536
        or     al, 0
        call   sub_1_0536
        pop    ax
        mov    ecx, 8
loc_1_040A:
        rol    al, 1
        push   ax
        call   sub_1_033D
        pop    ax
        loop   loc_1_040A
        jmp    NEAR PTR loc_1_041D
loc_1_041D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0422:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        call   sub_1_04C8
        and    al, 5
        call   sub_1_0536
        mov    al, 1
        call   sub_1_0536
        or     al, 2
        call   sub_1_0536
        mov    al, 2
        call   sub_1_0536
        mov    al, 0
        call   sub_1_0536
        jmp    NEAR PTR loc_1_0455
loc_1_0455:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_045A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        call   sub_1_04C8
        and    al, 5
        call   sub_1_0536
        mov    al, 0
        call   sub_1_0536
        or     al, 2
        call   sub_1_0536
        or     al, 1
        call   sub_1_0536
        mov    al, 1
        call   sub_1_0536
        jmp    NEAR PTR loc_1_048D
loc_1_048D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0492:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        call   sub_1_04C8
        and    al, 5
        call   sub_1_0536
        mov    al, 1
        call   sub_1_0536
        or     al, 2
        call   sub_1_0536
        call   sub_1_0536
        mov    al, 1
        call   sub_1_0536
        jmp    NEAR PTR loc_1_04C3
loc_1_04C3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_04C8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        in     al, dx
        nop
        nop
        nop
        nop
        mov    ah, al
        and    ah, 6
        mov    ecx, 3
        sar    al, cl
        and    al, 1
        or     al, ah
        jmp    NEAR PTR loc_1_04E8
loc_1_04E8:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_04ED:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        call   sub_1_04C8
        and    al, 5
        call   sub_1_0536
        or     al, 4
        call   sub_1_0536
        call   sub_1_033D
        and    al, 1
        cmp    al, 0
        je     NEAR PTR loc_1_0519
        or     byte ptr [esi + 6], 1
loc_1_0519:
        call   sub_1_04C8
        and    al, 5
        call   sub_1_0536
        and    al, 1
        call   sub_1_0536
        jmp    NEAR PTR loc_1_0531
loc_1_0531:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0536:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        out    dx, al
        xor    ecx, ecx
        mov    cx, word ptr [esi + 4]
        call   sub_1_0552
        jmp    NEAR PTR loc_1_054D
loc_1_054D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0552:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
loc_1_0558:
        loop   loc_1_0558
        jmp    NEAR PTR loc_1_055F
loc_1_055F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0564:
        push   ebp
        mov    ebp, esp
        sub    esp, 74h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 214h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 19h]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_0655
        mov    dword ptr [ebp - 20h], 0eh
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 244h]
        and    eax, 0ffh
        test   al, 8
        jne    NEAR PTR loc_1_05B6
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 19h], 0eh
        jmp    NEAR PTR loc_1_0655
loc_1_05B6:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 22fh]
        mov    word ptr [ebp - 64h], ax
        call   sub_1_0386
        db  08Dh, 035h, 000h, 000h, 000h, 000h
        lea    edi, [ebp - 60h]
        movzx  edx, word ptr [ebp - 64h]
        add    edx, 1ch
loc_1_05D9:
        call   sub_1_0422
        movzx  eax, byte ptr [esi + 1]
        shl    ax, 1
        sub    al, byte ptr [esi]
        rol    al, 1
        or     al, 1
        call   sub_1_03E8
        call   sub_1_04ED
        call   sub_1_03AF
        stosb
        call   sub_1_0492
        call   sub_1_045A
        dec    byte ptr [esi]
        jne    NEAR PTR loc_1_05D9
        lea    esi, [ebp - 60h]
        cmp    word ptr [esi], 55aah
        jne    NEAR PTR loc_1_064B
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 2]
        sub    cl, 2
        lea    esi, [esi + 3]
loc_1_0628:
        movzx  eax, word ptr [esi]
        cmp    al, 1
        je     NEAR PTR loc_1_0643
        and    al, 0fh
        add    al, 1
        sub    cl, al
        jbe    NEAR PTR loc_1_064B
        add    esi, eax
        jmp    SHORT loc_1_0628
loc_1_0643:
        xor    eax, eax
        inc    esi
        mov    al, byte ptr [esi]
        mov    dword ptr [ebp - 20h], eax
loc_1_064B:
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 19h], ax
loc_1_0655:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 237h]
        mov    eax, 0ffffh
        out    dx, ax
        push   4e20h
        call   sub_1_58D0
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 239h]
        in     al, dx
        mov    byte ptr [ebp - 1ch], al
        mov    eax, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 1ch]
        or     cl, 8
        mov    dx, word ptr [eax + 239h]
        mov    al, cl
        out    dx, al
        mov    word ptr [ebp - 4], 2710h
loc_1_0699:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        jle    NEAR PTR loc_1_06FC
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 237h]
        in     al, dx
        mov    byte ptr [ebp - 1ch], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 1ch]
        cmp    eax, 40h
        jne    NEAR PTR loc_1_06CA
        jmp    NEAR PTR loc_1_06FC
loc_1_06CA:
        xor    eax, eax
        mov    al, byte ptr [ebp - 1ch]
        test   al, 10h
        jne    NEAR PTR loc_1_06E4
        xor    eax, eax
        mov    al, byte ptr [ebp - 1ch]
        test   al, 20h
        jne    NEAR PTR loc_1_06E9
loc_1_06E4:
        jmp    NEAR PTR loc_1_0CF5
loc_1_06E9:
        push   2710h
        call   sub_1_58D0
        dec    word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_0699
loc_1_06FC:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_0711
        jmp    NEAR PTR loc_1_0CF5
loc_1_0711:
        push   3e8h
        call   sub_1_58D0
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 23fh]
        mov    eax, 1
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 23dh]
        mov    eax, 0a00h
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 233h]
        mov    eax, 8000h
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 233h]
        xor    eax, eax
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 233h]
        xor    eax, eax
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 233h]
        xor    eax, eax
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 233h]
        xor    eax, eax
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 8]
        mov    dx, word ptr [edx + 19h]
        and    edx, 0ffffh
        shl    edx, 8
        mov    ebx, edx
        and    ebx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    dx, word ptr [edx + 19h]
        and    edx, 0ffffh
        or     ebx, edx
        mov    dx, word ptr [ecx + 233h]
        mov    ax, bx
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 233h]
        mov    eax, 505h
        out    dx, ax
        mov    byte ptr [ebp - 18h], 0
        jmp    NEAR PTR loc_1_07D9
loc_1_07D6:
        inc    byte ptr [ebp - 18h]
loc_1_07D9:
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        cmp    eax, 0ah
        jge    NEAR PTR loc_1_07FA
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 233h]
        xor    eax, eax
        out    dx, ax
        jmp    NEAR PTR loc_1_07D6
loc_1_07FA:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 233h]
        mov    eax, 400h
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 233h]
        mov    eax, 20h
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 233h]
        mov    ax, word ptr [ebx + 0a7h]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 233h]
        mov    ax, word ptr [ecx + 0abh]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 237h]
        mov    eax, 9080h
        out    dx, ax
        push   4e20h
        call   sub_1_58D0
        mov    word ptr [ebp - 4], 4e20h
loc_1_0869:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        jle    NEAR PTR loc_1_08BF
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 237h]
        in     al, dx
        mov    byte ptr [ebp - 1ch], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 1ch]
        test   al, 70h
        jne    NEAR PTR loc_1_0899
        jmp    NEAR PTR loc_1_08BF
loc_1_0899:
        mov    al, byte ptr [ebp - 1ch]
        and    eax, 70h
        cmp    al, 10h
        jne    NEAR PTR loc_1_08AC
        jmp    NEAR PTR loc_1_0CF5
loc_1_08AC:
        push   2710h
        call   sub_1_58D0
        dec    word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_0869
loc_1_08BF:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_08D4
        jmp    NEAR PTR loc_1_0CF5
loc_1_08D4:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 23fh]
        mov    eax, 1
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 23dh]
        mov    eax, 0a30h
        out    dx, ax
        push   4e20h
        call   sub_1_58D0
        mov    word ptr [ebp - 4], 0fa0h
loc_1_0906:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        jle    NEAR PTR loc_1_0965
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 231h]
        in     ax, dx
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        test   ah, 40h
        je     NEAR PTR loc_1_093C
        jmp    NEAR PTR loc_1_0965
loc_1_093C:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        test   ah, 20h
        je     NEAR PTR loc_1_0952
        jmp    NEAR PTR loc_1_0CF5
loc_1_0952:
        push   3e8h
        call   sub_1_58D0
        dec    word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_0906
loc_1_0965:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_097A
        jmp    NEAR PTR loc_1_0CF5
loc_1_097A:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffdfh
        mov    dx, word ptr [eax + 231h]
        mov    ax, cx
        out    dx, ax
        mov    word ptr [ebp - 4], 1770h
loc_1_0998:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        jle    NEAR PTR loc_1_09E1
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 231h]
        in     ax, dx
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        test   ah, 20h
        jne    NEAR PTR loc_1_09CE
        jmp    NEAR PTR loc_1_09E1
loc_1_09CE:
        push   3e8h
        call   sub_1_58D0
        dec    word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_0998
loc_1_09E1:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_09F6
        jmp    NEAR PTR loc_1_0CF5
loc_1_09F6:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 23fh]
        mov    eax, 1
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 23dh]
        mov    eax, 0a02h
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 231h]
        in     ax, dx
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 23dh]
        mov    ax, word ptr [ebp - 0ch]
        out    dx, ax
        mov    byte ptr [ebp - 18h], 0
        jmp    NEAR PTR loc_1_0A44
loc_1_0A41:
        inc    byte ptr [ebp - 18h]
loc_1_0A44:
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        cmp    eax, 3
        jge    NEAR PTR loc_1_0A6D
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 233h]
        in     ax, dx
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 18h]
        mov    word ptr [ebp + ecx*2 - 14h], ax
        jmp    NEAR PTR loc_1_0A41
loc_1_0A6D:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_0A8D
        xor    eax, eax
        mov    ax, word ptr [ebp - 12h]
        cmp    eax, 3
        je     NEAR PTR loc_1_0A92
loc_1_0A8D:
        jmp    NEAR PTR loc_1_0CF5
loc_1_0A92:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 243h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_0AE6
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 21dh]
        shr    eax, 10h
        and    eax, 0ffffh
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 217h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 21dh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 219h], ax
        jmp    NEAR PTR loc_1_0B1D
loc_1_0AE6:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 221h]
        and    eax, 0ff0000h
        shr    eax, 10h
        and    ax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 217h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 221h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 219h], ax
loc_1_0B1D:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 215h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 21bh], 8000h
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2C60
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_0B52
        jmp    NEAR PTR loc_1_0CF5
loc_1_0B52:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3fh
        mov    dword ptr [ebp - 68h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 21dh]
        mov    dword ptr [ebp - 6ch], eax
        mov    eax, dword ptr [ebp - 6ch]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 68h]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 6ch]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 68h]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 6ch]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 68h]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 6ch]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 68h]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 6ch]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 68h]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp - 6ch]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp - 68h]
        mov    byte ptr [ecx + 5], al
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 3fh]
        test   eax, eax
        je     NEAR PTR loc_1_0BD1
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 3fh]
        cmp    eax, 10h
        je     NEAR PTR loc_1_0BD1
        jmp    NEAR PTR loc_1_0CF5
loc_1_0BD1:
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 45h]
        test   eax, eax
        jne    NEAR PTR loc_1_0C83
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 46h]
        test   eax, eax
        jne    NEAR PTR loc_1_0C83
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 47h]
        test   eax, eax
        jne    NEAR PTR loc_1_0C83
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 48h]
        test   eax, eax
        jne    NEAR PTR loc_1_0C83
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 49h]
        test   eax, eax
        jne    NEAR PTR loc_1_0C83
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, byte ptr [eax + 4ah]
        test   eax, eax
        jne    NEAR PTR loc_1_0C83
        mov    eax, dword ptr [ebp + 8]
        add    eax, 45h
        mov    dword ptr [ebp - 70h], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3fh
        mov    dword ptr [ebp - 74h], eax
        mov    eax, dword ptr [ebp - 74h]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 70h]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 74h]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 70h]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 74h]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 70h]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 74h]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 70h]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 74h]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 70h]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp - 74h]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp - 70h]
        mov    byte ptr [ecx + 5], al
loc_1_0C83:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 23fh]
        mov    eax, 1
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 23dh]
        mov    eax, 0a32h
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 231h]
        in     ax, dx
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 225h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 23dh]
        mov    eax, 0a34h
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 231h]
        in     ax, dx
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 212h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2fh], 0
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_0D19
loc_1_0CF5:
        push   7
        push   0
        push   2
        push   0aa000008h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 81h]
        push   eax
        call   sub_1_59A8
        add    esp, 14h
        mov    dword ptr [ebp - 8], 0c0000001h
loc_1_0D19:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_0D21
loc_1_0D21:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_1_0D28:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 14h], 0
        mov    dword ptr [ebp - 0ch], 3bh
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 3eh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_0D6B
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_0F87
loc_1_0D6B:
        mov    dword ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_0D7A
loc_1_0D77:
        inc    dword ptr [ebp - 10h]
loc_1_0D7A:
        mov    eax, dword ptr [ebp + 18h]
        cmp    dword ptr [ebp - 10h], eax
        jae    NEAR PTR loc_1_0DA0
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 14h]
        cmp    dword ptr [ecx + eax*4], 1
        jne    NEAR PTR loc_1_0D9B
        jmp    NEAR PTR loc_1_0DA0
loc_1_0D9B:
        jmp    NEAR PTR loc_1_0D77
loc_1_0DA0:
        mov    eax, dword ptr [ebp + 18h]
        cmp    dword ptr [ebp - 10h], eax
        jne    NEAR PTR loc_1_0DB6
        mov    eax, 0c0010019h
        jmp    NEAR PTR loc_1_0F87
loc_1_0DB6:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 5dh]
        mov    dword ptr [ebp - 18h], 0ffffffffh
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        push   0
        push   96h
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_58D6
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_1_0E13
        push   5
        push   1
        push   2
        push   0aa000002h
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 81h]
        push   eax
        call   sub_1_59A8
        add    esp, 14h
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_1_0F87
loc_1_0E13:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1dh], 0
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0ch
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 85h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0E7A:
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_0EBE
        push   8
        push   1
        push   2
        push   0aa000002h
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 81h]
        push   eax
        call   sub_1_59A8
        add    esp, 14h
        push   0
        push   96h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_58DC
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_0F87
loc_1_0EBE:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], 100h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 264h], 100h
        mov    eax, dword ptr [ebp - 8]
        add    eax, 61h
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 65h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 65h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 65h], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 30h]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_0F7F
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1ch], 1
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 31h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_0F7F
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 39h], 3
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 22dh], 0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_2A07
        test   eax, eax
        je     NEAR PTR loc_1_0F70
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 5dh]
        mov    dword ptr [ebp - 14h], 0c0000001h
        mov    eax, dword ptr [ebp - 14h]
        jmp    NEAR PTR loc_1_0F87
loc_1_0F70:
        mov    dword ptr [ebp - 14h], 103h
        mov    eax, dword ptr [ebp - 14h]
        jmp    NEAR PTR loc_1_0F87
loc_1_0F7F:
        mov    eax, dword ptr [ebp - 14h]
        jmp    NEAR PTR loc_1_0F87
loc_1_0F87:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    24h
sub_1_0F8E:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 61h]
        mov    dword ptr [ebp - 4], eax
loc_1_0FA7:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 61h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_1_0FE1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 18h], ecx
        jbe    NEAR PTR loc_1_0FD4
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 8], eax
loc_1_0FD4:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_0FA7
loc_1_0FE1:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_0FF2
        mov    dword ptr [ebp - 8], 100h
loc_1_0FF2:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 264h], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_1_1005:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 5dh]
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 1dh]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_1048
        mov    eax, 0c0010002h
        jmp    NEAR PTR loc_1_12D0
loc_1_1048:
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 14h]
        push   0
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 85h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_1066:
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_115B
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 14h], 2
        jne    NEAR PTR loc_1_10F5
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 264h]
        cmp    dword ptr [eax + 18h], ecx
        jne    NEAR PTR loc_1_10E0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0F8E
loc_1_10E0:
        push   0
        push   96h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_58DC
        jmp    NEAR PTR loc_1_1156
loc_1_10F5:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 1dh], 1
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 69h
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 6dh]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 6dh]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 6dh], eax
        mov    eax, dword ptr [ebp - 8]
        sub    dword ptr [eax + 14h], 2
        mov    dword ptr [ebp - 0ch], 103h
loc_1_1156:
        jmp    NEAR PTR loc_1_12C8
loc_1_115B:
        cmp    dword ptr [ebp - 0ch], 103h
        jne    NEAR PTR loc_1_11C7
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 1dh], 1
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        sub    dword ptr [eax + 14h], 2
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 264h]
        cmp    dword ptr [eax + 18h], ecx
        jne    NEAR PTR loc_1_11B5
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0F8E
loc_1_11B5:
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 5dh]
        xor    eax, eax
        jmp    NEAR PTR loc_1_12D0
        db  0E9h, 001h, 001h, 000h, 000h
loc_1_11C7:
        cmp    dword ptr [ebp - 0ch], 0c001000eh
        jne    NEAR PTR loc_1_1239
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 1dh], 1
        mov    dword ptr [ebp - 0ch], 103h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp - 20h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 20h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 69h
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 6dh]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 6dh]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 6dh], eax
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_1_12C8
loc_1_1239:
        cmp    dword ptr [ebp - 0ch], 0c001000dh
        jne    NEAR PTR loc_1_12AB
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 1dh], 1
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 24h]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 24h]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 71h
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 75h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 75h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 75h], eax
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 14h]
        mov    dword ptr [ebp - 0ch], 103h
        jmp    NEAR PTR loc_1_12C8
loc_1_12AB:
        push   1
        push   0ah
        push   2
        push   0aa000006h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 81h]
        push   eax
        call   sub_1_59A8
        add    esp, 14h
loc_1_12C8:
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_12D0
loc_1_12D0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_1_12D7:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_57FC
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_57F0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_1_1304:
        push   ebp
        mov    ebp, esp
        sub    esp, 1e8h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 48h], 0
        mov    word ptr [ebp - 19ch], 6
        mov    word ptr [ebp - 19ah], 6
        db  0C7h, 085h, 068h, 0FEh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 20h], 8
        mov    word ptr [ebp - 1eh], 8
        db  0C7h, 045h, 0E4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 44h], 0ah
        mov    word ptr [ebp - 42h], 0ah
        db  0C7h, 045h, 0C0h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 1d0h], 0ah
        mov    word ptr [ebp - 1ceh], 0ah
        db  0C7h, 085h, 034h, 0FEh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 0ch], 0bh
        mov    word ptr [ebp - 0ah], 0bh
        db  0C7h, 045h, 0F8h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 1bch], 9
        mov    word ptr [ebp - 1bah], 9
        db  0C7h, 085h, 048h, 0FEh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 38h], 0dh
        mov    word ptr [ebp - 36h], 0dh
        db  0C7h, 045h, 0CCh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 1ach], 0ah
        mov    word ptr [ebp - 1aah], 0ah
        db  0C7h, 085h, 058h, 0FEh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 30h], 0dh
        mov    word ptr [ebp - 2eh], 0dh
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 1b4h], 12h
        mov    word ptr [ebp - 1b2h], 12h
        db  0C7h, 085h, 050h, 0FEh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 1a4h], 12h
        mov    word ptr [ebp - 1a2h], 12h
        db  0C7h, 085h, 060h, 0FEh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 194h], 11h
        mov    word ptr [ebp - 192h], 11h
        db  0C7h, 085h, 070h, 0FEh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 18h], 0bh
        mov    word ptr [ebp - 16h], 0bh
        db  0C7h, 045h, 0ECh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 3ch], 0
        mov    byte ptr [ebp - 18ch], 0
        mov    byte ptr [ebp - 1c8h], 2
        mov    byte ptr [ebp - 1c7h], 3
        mov    byte ptr [ebp - 1c6h], 0ah
        mov    byte ptr [ebp - 1c5h], 0bh
        mov    byte ptr [ebp - 1c4h], 4
        mov    byte ptr [ebp - 1c3h], 6
        mov    byte ptr [ebp - 1c2h], 5
        mov    byte ptr [ebp - 1c1h], 7
        jmp    NEAR PTR loc_1_1498
loc_1_1498:
        jmp    NEAR PTR loc_1_149D
loc_1_149D:
        mov    dword ptr [ebp - 1dch], 0ffffffffh
        mov    eax, dword ptr [ebp - 1dch]
        push   eax
        push   0
        push   268h
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_1_58D6
        mov    dword ptr [ebp - 48h], eax
        cmp    dword ptr [ebp - 48h], 0
        je     NEAR PTR loc_1_14D5
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_1_1E5D
loc_1_14D5:
        mov    edi, dword ptr [ebp - 10h]
        xor    eax, eax
        mov    ecx, 9ah
        rep stosd
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 7dh], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [eax + 0a7h], 40h
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [eax + 0abh], 20h
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [eax + 245h], 1194h
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [eax + 247h], 1194h
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [eax + 39h], 2
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 1c0h]
        push   eax
        lea    eax, [ebp - 48h]
        push   eax
        call   sub_1_5954
        cmp    dword ptr [ebp - 48h], 0
        je     NEAR PTR loc_1_1560
        push   0
        push   268h
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_58DC
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_1E5D
loc_1_1560:
        push   1
        lea    eax, [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp - 1c0h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        lea    eax, [ebp - 48h]
        push   eax
        call   sub_1_595A
        cmp    dword ptr [ebp - 48h], 0
        jne    NEAR PTR loc_1_15D7
        mov    eax, dword ptr [ebp - 28h]
        cmp    dword ptr [eax + 4], 3
        jne    NEAR PTR loc_1_15B1
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 244h]
        and    eax, 0ffh
        or     al, 10h
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 244h], al
        mov    byte ptr [ebp - 18ch], 1
loc_1_15B1:
        mov    eax, dword ptr [ebp - 28h]
        cmp    dword ptr [eax + 4], 2
        jne    NEAR PTR loc_1_15D7
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 244h]
        and    eax, 0ffh
        or     al, 30h
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 244h], al
loc_1_15D7:
        mov    eax, dword ptr [ebp - 1c0h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 24h]
        push   eax
        lea    eax, [ebp - 48h]
        push   eax
        call   sub_1_585C
        cmp    dword ptr [ebp - 48h], 0
        jne    NEAR PTR loc_1_16FE
        cmp    dword ptr [ebp - 4], 6
        jne    NEAR PTR loc_1_16FE
        mov    eax, dword ptr [ebp - 24h]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 24h]
        mov    cl, byte ptr [ecx + 3]
        and    ecx, 0ffh
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 24h]
        mov    cl, byte ptr [ecx + 4]
        and    ecx, 0ffh
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 24h]
        mov    cl, byte ptr [ecx + 5]
        and    ecx, 0ffh
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 24h]
        mov    cl, byte ptr [ecx]
        and    ecx, 0ffh
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 24h]
        mov    cl, byte ptr [ecx + 1]
        and    ecx, 0ffh
        or     eax, ecx
        je     NEAR PTR loc_1_16FE
        mov    eax, dword ptr [ebp - 24h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        cmp    eax, 4fh
        jg     NEAR PTR loc_1_16FE
        mov    eax, dword ptr [ebp - 24h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        cmp    eax, 40h
        jl     NEAR PTR loc_1_16FE
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 45h
        mov    dword ptr [ebp - 1e0h], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [ebp - 1e4h], eax
        mov    eax, dword ptr [ebp - 1e4h]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 1e0h]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 1e4h]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 1e0h]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 1e4h]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 1e0h]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 1e4h]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 1e0h]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 1e4h]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 1e0h]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp - 1e4h]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp - 1e0h]
        mov    byte ptr [ecx + 5], al
loc_1_16FE:
        xor    eax, eax
        mov    al, byte ptr [ebp - 18ch]
        cmp    eax, 1
        jne    NEAR PTR loc_1_1A0A
        push   0
        lea    eax, [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 1c0h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        lea    eax, [ebp - 48h]
        push   eax
        call   sub_1_595A
        cmp    dword ptr [ebp - 48h], 0
        jne    NEAR PTR loc_1_1741
        mov    eax, dword ptr [ebp - 28h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        mov    dword ptr [ebp - 3ch], eax
loc_1_1741:
        lea    eax, [ebp - 1d8h]
        push   eax
        lea    eax, [ebp - 3ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 48h]
        push   eax
        call   sub_1_5850
        cmp    dword ptr [ebp - 48h], 0
        je     NEAR PTR loc_1_177B
        push   0
        push   268h
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_58DC
        mov    eax, dword ptr [ebp - 48h]
        jmp    NEAR PTR loc_1_1E5D
loc_1_177B:
        mov    eax, dword ptr [ebp - 1d8h]
        and    eax, 0ffffh
        cmp    eax, 5c1ch
        je     NEAR PTR loc_1_17C1
        mov    eax, dword ptr [ebp - 1d8h]
        and    eax, 0ffffh
        cmp    eax, 5c1dh
        je     NEAR PTR loc_1_17C1
        push   0
        push   268h
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_58DC
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_1E5D
loc_1_17C1:
        xor    eax, eax
        mov    al, byte ptr [ebp - 1d6h]
        test   al, 2
        jne    NEAR PTR loc_1_17EA
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 244h]
        and    eax, 0ffh
        or     al, 1
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 244h], al
loc_1_17EA:
        mov    al, byte ptr [ebp - 1d3h]
        and    eax, 7
        mov    dword ptr [ebp - 1e8h], eax
        jmp    NEAR PTR loc_1_188B
loc_1_17FE:
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [eax + 22fh], 0a00h
        jmp    NEAR PTR loc_1_18C5
loc_1_180F:
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [eax + 22fh], 0a20h
        jmp    NEAR PTR loc_1_18C5
loc_1_1820:
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [eax + 22fh], 0a40h
        jmp    NEAR PTR loc_1_18C5
loc_1_1831:
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [eax + 22fh], 0a60h
        jmp    NEAR PTR loc_1_18C5
loc_1_1842:
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [eax + 22fh], 1a00h
        jmp    NEAR PTR loc_1_18C5
loc_1_1853:
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [eax + 22fh], 1a20h
        jmp    NEAR PTR loc_1_18C5
loc_1_1864:
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [eax + 22fh], 1a40h
        jmp    NEAR PTR loc_1_18C5
loc_1_1875:
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [eax + 22fh], 1a60h
        jmp    NEAR PTR loc_1_18C5
        db  0E9h, 03Ah, 000h, 000h, 000h
loc_1_188B:
        cmp    dword ptr [ebp - 1e8h], 7
        ja     NEAR PTR loc_1_18C5
        mov    eax, dword ptr [ebp - 1e8h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_18C5:
        xor    eax, eax
        mov    al, byte ptr [ebp - 1d5h]
        test   al, 10h
        jne    NEAR PTR loc_1_196C
        mov    al, byte ptr [ebp - 1d5h]
        and    eax, 7
        mov    al, byte ptr [ebp + eax - 1c8h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 18h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 1d6h]
        test   al, 40h
        je     NEAR PTR loc_1_1934
        xor    eax, eax
        mov    al, byte ptr [ebp - 1d4h]
        sar    eax, 4
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 243h], al
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 243h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_192F
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 243h], 10h
loc_1_192F:
        jmp    NEAR PTR loc_1_193E
loc_1_1934:
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 243h], 0
loc_1_193E:
        xor    eax, eax
        mov    al, byte ptr [ebp - 1d6h]
        test   al, 4
        je     NEAR PTR loc_1_1967
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 244h]
        and    eax, 0ffh
        or     al, 4
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 244h], al
loc_1_1967:
        jmp    NEAR PTR loc_1_1A05
loc_1_196C:
        xor    eax, eax
        mov    al, byte ptr [ebp - 1d3h]
        test   al, 1
        je     NEAR PTR loc_1_198D
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [eax + 22fh], 1a20h
        jmp    NEAR PTR loc_1_1999
loc_1_198D:
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [eax + 22fh], 0a20h
loc_1_1999:
        mov    al, byte ptr [ebp - 1d5h]
        and    eax, 3
        mov    al, byte ptr [ebp + eax - 1c8h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 18h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 1d6h]
        test   al, 40h
        je     NEAR PTR loc_1_19FB
        xor    eax, eax
        mov    al, byte ptr [ebp - 1d4h]
        sar    eax, 6
        add    eax, 4
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 243h], al
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 243h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_19F6
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 243h], 10h
loc_1_19F6:
        jmp    NEAR PTR loc_1_1A05
loc_1_19FB:
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 243h], 0
loc_1_1A05:
        jmp    NEAR PTR loc_1_1C73
loc_1_1A0A:
        lea    eax, [ebp - 188h]
        push   eax
        lea    eax, [ebp - 3ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 48h]
        push   eax
        call   sub_1_5856
        cmp    dword ptr [ebp - 48h], 0
        jne    NEAR PTR loc_1_1A9E
        cmp    dword ptr [ebp - 188h], 10026110h
        je     NEAR PTR loc_1_1A46
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_1E5D
loc_1_1A46:
        mov    al, byte ptr [ebp - 0d6h]
        and    al, 0fh
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 18h], al
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 243h], 10h
        mov    ax, word ptr [ebp - 0bfh]
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 22fh], ax
        xor    eax, eax
        mov    al, byte ptr [ebp - 15dh]
        cmp    eax, 31h
        jne    NEAR PTR loc_1_1A99
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 244h]
        and    eax, 0ffh
        or     al, 1
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 244h], al
loc_1_1A99:
        jmp    NEAR PTR loc_1_1C73
loc_1_1A9E:
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 244h]
        and    eax, 0cfh
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 244h], al
        push   1
        lea    eax, [ebp - 44h]
        push   eax
        mov    eax, dword ptr [ebp - 1c0h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        lea    eax, [ebp - 48h]
        push   eax
        call   sub_1_595A
        cmp    dword ptr [ebp - 48h], 0
        jne    NEAR PTR loc_1_1AEE
        mov    eax, dword ptr [ebp - 28h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 22fh], ax
        jmp    NEAR PTR loc_1_1AF8
loc_1_1AEE:
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_1E5D
loc_1_1AF8:
        push   0
        lea    eax, [ebp - 1d0h]
        push   eax
        mov    eax, dword ptr [ebp - 1c0h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        lea    eax, [ebp - 48h]
        push   eax
        call   sub_1_595A
        cmp    dword ptr [ebp - 48h], 0
        jne    NEAR PTR loc_1_1B2B
        mov    eax, dword ptr [ebp - 28h]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 18h], al
loc_1_1B2B:
        push   0
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 1c0h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        lea    eax, [ebp - 48h]
        push   eax
        call   sub_1_595A
        cmp    dword ptr [ebp - 48h], 0
        jne    NEAR PTR loc_1_1B63
        mov    eax, dword ptr [ebp - 28h]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 243h], al
        jmp    NEAR PTR loc_1_1B6D
loc_1_1B63:
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 243h], 0
loc_1_1B6D:
        push   0
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp - 1c0h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        lea    eax, [ebp - 48h]
        push   eax
        call   sub_1_595A
        cmp    dword ptr [ebp - 48h], 0
        jne    NEAR PTR loc_1_1BAF
        mov    eax, dword ptr [ebp - 28h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_1BAF
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 243h], 0
loc_1_1BAF:
        push   0
        lea    eax, [ebp - 1bch]
        push   eax
        mov    eax, dword ptr [ebp - 1c0h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        lea    eax, [ebp - 48h]
        push   eax
        call   sub_1_595A
        cmp    dword ptr [ebp - 48h], 0
        jne    NEAR PTR loc_1_1C03
        mov    eax, dword ptr [ebp - 28h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 10h
        jne    NEAR PTR loc_1_1C03
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 244h]
        and    eax, 0ffh
        or     al, 1
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 244h], al
loc_1_1C03:
        push   0
        lea    eax, [ebp - 1ach]
        push   eax
        mov    eax, dword ptr [ebp - 1c0h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        lea    eax, [ebp - 48h]
        push   eax
        call   sub_1_595A
        cmp    dword ptr [ebp - 48h], 0
        jne    NEAR PTR loc_1_1C73
        mov    eax, dword ptr [ebp - 28h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_1C5C
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 244h]
        and    eax, 0ffh
        or     al, 4
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 244h], al
        jmp    NEAR PTR loc_1_1C73
loc_1_1C5C:
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 244h]
        and    eax, 0fbh
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 244h], al
loc_1_1C73:
        push   0
        lea    eax, [ebp - 38h]
        push   eax
        mov    eax, dword ptr [ebp - 1c0h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        lea    eax, [ebp - 48h]
        push   eax
        call   sub_1_595A
        cmp    dword ptr [ebp - 48h], 0
        jne    NEAR PTR loc_1_1CE0
        mov    eax, dword ptr [ebp - 28h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_1CC9
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 244h]
        and    eax, 0ffh
        or     al, 2
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 244h], al
        jmp    NEAR PTR loc_1_1CE0
loc_1_1CC9:
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 244h]
        and    eax, 0fdh
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 244h], al
loc_1_1CE0:
        push   0
        lea    eax, [ebp - 19ch]
        push   eax
        mov    eax, dword ptr [ebp - 1c0h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        lea    eax, [ebp - 48h]
        push   eax
        call   sub_1_595A
        cmp    dword ptr [ebp - 48h], 0
        jne    NEAR PTR loc_1_1D14
        mov    eax, dword ptr [ebp - 28h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 19h], ax
loc_1_1D14:
        push   0
        lea    eax, [ebp - 1b4h]
        push   eax
        mov    eax, dword ptr [ebp - 1c0h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        lea    eax, [ebp - 48h]
        push   eax
        call   sub_1_595A
        cmp    dword ptr [ebp - 48h], 0
        jne    NEAR PTR loc_1_1D50
        mov    eax, dword ptr [ebp - 28h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 0abh], ax
loc_1_1D50:
        push   0
        lea    eax, [ebp - 1a4h]
        push   eax
        mov    eax, dword ptr [ebp - 1c0h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        lea    eax, [ebp - 48h]
        push   eax
        call   sub_1_595A
        cmp    dword ptr [ebp - 48h], 0
        jne    NEAR PTR loc_1_1D8C
        mov    eax, dword ptr [ebp - 28h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 0a7h], ax
loc_1_1D8C:
        push   0
        lea    eax, [ebp - 194h]
        push   eax
        mov    eax, dword ptr [ebp - 1c0h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        lea    eax, [ebp - 48h]
        push   eax
        call   sub_1_595A
        cmp    dword ptr [ebp - 48h], 0
        jne    NEAR PTR loc_1_1E1B
        mov    eax, dword ptr [ebp - 28h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 247h], eax
        mov    eax, dword ptr [ebp - 28h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 245h], ax
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 244h]
        and    eax, 0ffh
        test   al, 1
        jne    NEAR PTR loc_1_1E1B
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 245h]
        and    eax, 0ffffh
        cmp    eax, 1194h
        jle    NEAR PTR loc_1_1E1B
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [eax + 245h], 1194h
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [eax + 247h], 1194h
loc_1_1E1B:
        mov    eax, dword ptr [ebp - 1c0h]
        push   eax
        call   sub_1_5960
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_1E38:
        mov    dword ptr [ebp - 48h], eax
        cmp    dword ptr [ebp - 48h], 0
        je     NEAR PTR loc_1_1E55
        push   0
        push   268h
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_58DC
loc_1_1E55:
        mov    eax, dword ptr [ebp - 48h]
        jmp    NEAR PTR loc_1_1E5D
loc_1_1E5D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
loc_1_1E64:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 1fh
        push   eax
        call   sub_1_58B8
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 3eh], 1
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 21bh], 200h
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 221h]
        and    eax, 0ff0000h
        shr    eax, 10h
        and    ax, 0ffh
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 217h], ax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 221h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 219h], ax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 215h], 4
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_2C60
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_1FA5
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 61h]
        mov    dword ptr [ebp - 0ch], eax
loc_1_1EFA:
        mov    eax, dword ptr [ebp - 8]
        add    eax, 61h
        cmp    eax, dword ptr [ebp - 0ch]
        je     NEAR PTR loc_1_1F49
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        inc    dword ptr [eax + 14h]
        push   0
        push   0
        push   40010007h
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_583E
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_5844
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 10h]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_1_1EFA
loc_1_1F49:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 1f7h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 1fbh], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 31h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 30h], 1
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 3dh], 1
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 22bh], 0
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 227h]
        dec    dword ptr [eax + 14h]
        push   0c0000001h
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 227h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_5826
loc_1_1FA5:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_58C4
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 85h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_1FBD:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_1FC6:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 81h]
        push   eax
        call   sub_1_5808
        push   0
        push   268h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_58DC
        jmp    NEAR PTR loc_1_1FEA
loc_1_1FEA:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_1_1FF1:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 5dh]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 3dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_208D
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 1dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_2081
        mov    eax, dword ptr [ebp - 0ch]
        inc    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2315
        mov    eax, dword ptr [ebp - 0ch]
        dec    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_20FA
        test   eax, eax
        je     NEAR PTR loc_1_206F
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 5dh]
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_20F3
loc_1_206F:
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 5dh]
        mov    dword ptr [ebp - 8], 103h
        jmp    NEAR PTR loc_1_2088
loc_1_2081:
        mov    dword ptr [ebp - 8], 0c0010002h
loc_1_2088:
        jmp    NEAR PTR loc_1_20EB
loc_1_208D:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 22bh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_20AF
        mov    dword ptr [ebp - 8], 0c001000dh
        jmp    NEAR PTR loc_1_20EB
loc_1_20AF:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 30h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_20CE
        mov    dword ptr [ebp - 8], 0c0000001h
        jmp    NEAR PTR loc_1_20EB
loc_1_20CE:
        push   4
        push   0bh
        push   2
        push   0aa000006h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 81h]
        push   eax
        call   sub_1_59A8
        add    esp, 14h
loc_1_20EB:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_20F3
loc_1_20F3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_20FA:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 30h], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0564
        test   eax, eax
        je     NEAR PTR loc_1_21BF
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 61h]
        mov    dword ptr [ebp - 10h], eax
loc_1_2124:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 61h
        cmp    eax, dword ptr [ebp - 10h]
        je     NEAR PTR loc_1_2173
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        inc    dword ptr [eax + 14h]
        push   0
        push   0
        push   40010007h
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_583E
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_5844
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 14h]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_1_2124
loc_1_2173:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 31h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 30h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 3dh], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 22bh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 227h]
        dec    dword ptr [eax + 14h]
        push   0c0000001h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 227h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_5826
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_230E
loc_1_21BF:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1f3h]
        cmp    dword ptr [eax + 10h], 0
        je     NEAR PTR loc_1_225D
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1f3h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1f3h]
        mov    dword ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1f3h]
        mov    al, byte ptr [eax + 1ah]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_2217
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1f3h]
        mov    byte ptr [eax + 1ah], 0
loc_1_2217:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1f3h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1f3h], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 24h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0ch], eax
        push   0c001000ch
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_581A
        mov    eax, dword ptr [ebp - 0ch]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_1_21BF
loc_1_225D:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0afh]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1f3h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0afh]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1efh], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0a7h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 200h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1ebh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1edh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1ffh], 1
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1f7h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1fbh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 5bh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0afh]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 202h], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2A07
        test   eax, eax
        je     NEAR PTR loc_1_2307
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_230E
loc_1_2307:
        xor    eax, eax
        jmp    NEAR PTR loc_1_230E
loc_1_230E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_2315:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 61h]
        mov    dword ptr [ebp - 4], eax
loc_1_2327:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 61h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_1_235E
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   0
        push   0
        push   40010004h
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_583E
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_2327
loc_1_235E:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 22bh], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 3dh], 1
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 227h], eax
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_238B
        mov    eax, dword ptr [ebp + 0ch]
        inc    dword ptr [eax + 14h]
loc_1_238B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_2392:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 61h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 65h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 65h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 61h], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 69h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 6dh], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 6dh]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 69h], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 71h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 75h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 75h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 71h], eax
        jmp    NEAR PTR loc_1_23E5
loc_1_23E5:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 31h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 30h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 3dh], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 2fh], 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_1_2408:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp + 14h], 0
        jne    NEAR PTR loc_1_243D
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 1eh
        mov    dword ptr [ebp + 14h], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [eax + 10h], 4
loc_1_243D:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 22bh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_245F
        mov    dword ptr [ebp - 4], 0c001000dh
        jmp    NEAR PTR loc_1_250B
loc_1_245F:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0ch], eax
        jne    NEAR PTR loc_1_246B
loc_1_246B:
        test   byte ptr [ebp + 0ch], 10h
        je     NEAR PTR loc_1_2481
        mov    dword ptr [ebp - 4], 0c0000001h
        jmp    NEAR PTR loc_1_250B
loc_1_2481:
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 10h], 4
        jne    NEAR PTR loc_1_24AB
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 8], eax
        jmp    NEAR PTR loc_1_24B4
loc_1_24AB:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 8], eax
loc_1_24B4:
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 89h], 0
        jne    NEAR PTR loc_1_24F0
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 8dh], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8dh]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 89h], eax
        jmp    NEAR PTR loc_1_24FE
loc_1_24F0:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 8dh]
        mov    dword ptr [ecx], eax
loc_1_24FE:
        mov    eax, dword ptr [ebp - 0ch]
        inc    dword ptr [eax + 14h]
        mov    dword ptr [ebp - 4], 103h
loc_1_250B:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_2513
loc_1_2513:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
loc_1_251A:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp + 14h], 0
        jne    NEAR PTR loc_1_254F
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 46h
        mov    dword ptr [ebp + 14h], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [eax + 10h], 9
loc_1_254F:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 22bh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_2571
        mov    dword ptr [ebp - 4], 0c001000dh
        jmp    NEAR PTR loc_1_25E0
loc_1_2571:
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 89h], 0
        jne    NEAR PTR loc_1_25C5
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 8dh], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8dh]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 89h], eax
        jmp    NEAR PTR loc_1_25D3
loc_1_25C5:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 8dh]
        mov    dword ptr [ecx], eax
loc_1_25D3:
        mov    eax, dword ptr [ebp - 0ch]
        inc    dword ptr [eax + 14h]
        mov    dword ptr [ebp - 4], 103h
loc_1_25E0:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_25E8
loc_1_25E8:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
loc_1_25EF:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp + 14h], 0
        jne    NEAR PTR loc_1_2624
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 6eh
        mov    dword ptr [ebp + 14h], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [eax + 10h], 0ah
loc_1_2624:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 22bh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_2646
        mov    dword ptr [ebp - 4], 0c001000dh
        jmp    NEAR PTR loc_1_26B5
loc_1_2646:
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 89h], 0
        jne    NEAR PTR loc_1_269A
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 8dh], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8dh]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 89h], eax
        jmp    NEAR PTR loc_1_26A8
loc_1_269A:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 8dh]
        mov    dword ptr [ecx], eax
loc_1_26A8:
        mov    eax, dword ptr [ebp - 0ch]
        inc    dword ptr [eax + 14h]
        mov    dword ptr [ebp - 4], 103h
loc_1_26B5:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_26BD
loc_1_26BD:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
sub_1_26C4:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
loc_1_26CD:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 89h], 0
        je     NEAR PTR loc_1_28BF
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 89h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 89h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 89h], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 89h], 0
        jne    NEAR PTR loc_1_271A
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8dh], 0
loc_1_271A:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 10h], 8
        jne    NEAR PTR loc_1_274E
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 4], 0
        je     NEAR PTR loc_1_2742
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2F48
        jmp    NEAR PTR loc_1_2742
loc_1_2742:
        mov    dword ptr [ebp - 8], 103h
        jmp    NEAR PTR loc_1_28BA
loc_1_274E:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_2876
loc_1_275C:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 14h], 1010eh
        jne    NEAR PTR loc_1_2784
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3404
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_27CF
loc_1_2784:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 14h], 2010103h
        jne    NEAR PTR loc_1_27B4
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 99h], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_30AF
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_27CF
loc_1_27B4:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0a1h], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_31A1
        mov    dword ptr [ebp - 8], eax
loc_1_27CF:
        jmp    NEAR PTR loc_1_28BA
loc_1_27D4:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3404
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_28BA
loc_1_27EC:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 99h], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_30AF
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_28BA
loc_1_280C:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0a1h], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_31A1
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_28BA
loc_1_282C:
        mov    dword ptr [ebp - 8], 103h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2F48
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 4], 1
        jmp    NEAR PTR loc_1_28BA
loc_1_2848:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        push   1
        push   9
        push   3
        push   0aa000006h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 81h]
        push   eax
        call   sub_1_59A8
        add    esp, 18h
        jmp    NEAR PTR loc_1_28BA
        db  0E9h, 044h, 000h, 000h, 000h
loc_1_2876:
        dec    dword ptr [ebp - 10h]
        cmp    dword ptr [ebp - 10h], 9
        ja     NEAR PTR loc_1_2848
        mov    eax, dword ptr [ebp - 10h]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 005h, 002h, 005h, 005h, 005h, 005h
        db  003h, 004h
loc_1_28BA:
        jmp    NEAR PTR loc_1_26CD
loc_1_28BF:
        cmp    dword ptr [ebp - 8], 103h
        jne    NEAR PTR loc_1_28E7
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 91h], eax
        mov    eax, 103h
        jmp    NEAR PTR loc_1_29ED
        db  0E9h, 0FFh, 000h, 000h, 000h
loc_1_28E7:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_29C2
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_297D
loc_1_28FF:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_1_29BD
loc_1_2913:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_1_29BD
loc_1_2921:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_1_29BD
loc_1_292F:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 7dh]
        push   eax
        call   sub_1_584A
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 5dh]
        jmp    NEAR PTR loc_1_29BD
loc_1_294C:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        push   9
        db  068h, 000h, 000h, 000h, 000h
        push   3
        push   0aa000006h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 81h]
        push   eax
        call   sub_1_59A8
        add    esp, 18h
        jmp    NEAR PTR loc_1_29BD
        db  0E9h, 040h, 000h, 000h, 000h
loc_1_297D:
        dec    dword ptr [ebp - 14h]
        cmp    dword ptr [ebp - 14h], 9
        ja     NEAR PTR loc_1_294C
        mov    eax, dword ptr [ebp - 14h]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 001h, 004h, 002h, 004h, 004h, 004h, 004h, 003h, 003h
loc_1_29BD:
        jmp    NEAR PTR loc_1_29E6
loc_1_29C2:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   0ah
        db  068h, 000h, 000h, 000h, 000h
        push   3
        push   0aa000006h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 81h]
        push   eax
        call   sub_1_59A8
        add    esp, 18h
loc_1_29E6:
        xor    eax, eax
        jmp    NEAR PTR loc_1_29ED
loc_1_29ED:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_1_29F4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 14h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_2A07:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        db  0B8h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 14h]
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
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 244h]
        and    eax, 0ffh
        test   al, 2
        jne    NEAR PTR loc_1_2A73
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 22dh]
        and    eax, 0ffffh
        or     eax, 1000h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 22dh], ax
loc_1_2A73:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 21bh], 100h
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 243h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_2AD1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0b7h]
        and    eax, 0ff0000h
        shr    eax, 10h
        and    ax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 217h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0b7h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 219h], ax
        jmp    NEAR PTR loc_1_2B00
loc_1_2AD1:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0b3h]
        shr    eax, 10h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 217h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0b3h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 219h], ax
loc_1_2B00:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 215h], 1ah
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [eax + 0b3h]
        xor    eax, eax
        mov    ecx, 6
        rep stosd
        stosw
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 22dh]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0b3h]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0b3h]
        movsx  eax, word ptr [eax]
        and    eax, 97fdh
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0b3h]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0b3h]
        add    eax, 2
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 45h
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp - 18h]
        mov    byte ptr [ecx + 5], al
        push   12h
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0b3h]
        add    eax, 8
        push   eax
        call   sub_1_59BC
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 31h], 1
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2C60
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_2BF2
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_2C17
loc_1_2BF2:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 33h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_2C10
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_2C17
loc_1_2C10:
        xor    eax, eax
        jmp    NEAR PTR loc_1_2C17
loc_1_2C17:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_2C1E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 89h], 0
        je     NEAR PTR loc_1_2C52
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_26C4
        cmp    eax, 103h
        jne    NEAR PTR loc_1_2C52
        mov    eax, 103h
        jmp    NEAR PTR loc_1_2C59
loc_1_2C52:
        xor    eax, eax
        jmp    NEAR PTR loc_1_2C59
loc_1_2C59:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_2C60:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 214h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_2C95
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1ffh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_2C9C
loc_1_2C95:
        xor    al, al
        jmp    NEAR PTR loc_1_2F41
loc_1_2C9C:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 214h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 23dh]
        mov    eax, 0a30h
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 231h]
        in     ax, dx
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        or     ecx, 20h
        mov    dx, word ptr [eax + 231h]
        mov    ax, cx
        out    dx, ax
        mov    word ptr [ebp - 8], 0ea60h
loc_1_2CE8:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   eax, eax
        jle    NEAR PTR loc_1_2D31
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 231h]
        in     ax, dx
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        test   ah, 20h
        je     NEAR PTR loc_1_2D1E
        jmp    NEAR PTR loc_1_2D31
loc_1_2D1E:
        push   0bb8h
        call   sub_1_58D0
        dec    word ptr [ebp - 8]
        jmp    NEAR PTR loc_1_2CE8
loc_1_2D31:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_2D52
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 214h], 1
        xor    al, al
        jmp    NEAR PTR loc_1_2F41
loc_1_2D52:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 23dh]
        mov    eax, 0a3ch
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 233h]
        mov    ax, word ptr [ebx + 215h]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 233h]
        mov    ax, word ptr [ecx + 217h]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 233h]
        mov    ax, word ptr [ecx + 219h]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 233h]
        mov    ax, word ptr [ecx + 21bh]
        out    dx, ax
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        test   eax, eax
        jne    NEAR PTR loc_1_2E87
        push   186a0h
        call   sub_1_58D0
        mov    word ptr [ebp - 8], 7530h
loc_1_2DD8:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   eax, eax
        jle    NEAR PTR loc_1_2E31
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 23dh]
        mov    eax, 0a42h
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 231h]
        in     ax, dx
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        test   al, 8
        je     NEAR PTR loc_1_2E1E
        jmp    NEAR PTR loc_1_2E31
loc_1_2E1E:
        push   3e8h
        call   sub_1_58D0
        dec    word ptr [ebp - 8]
        jmp    NEAR PTR loc_1_2DD8
loc_1_2E31:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_2E52
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 214h], 1
        xor    al, al
        jmp    NEAR PTR loc_1_2F41
loc_1_2E52:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 231h]
        xor    eax, eax
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 23dh]
        mov    eax, 0a3ch
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 231h]
        in     ax, dx
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 215h], ax
loc_1_2E87:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 23dh]
        mov    eax, 0a30h
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 231h]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffdfh
        mov    dx, word ptr [eax + 231h]
        mov    ax, cx
        out    dx, ax
        mov    word ptr [ebp - 8], 0ea60h
loc_1_2EC6:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   eax, eax
        jle    NEAR PTR loc_1_2F0F
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 231h]
        in     ax, dx
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        test   ah, 20h
        jne    NEAR PTR loc_1_2EFC
        jmp    NEAR PTR loc_1_2F0F
loc_1_2EFC:
        push   3e8h
        call   sub_1_58D0
        dec    word ptr [ebp - 8]
        jmp    NEAR PTR loc_1_2EC6
loc_1_2F0F:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_2F30
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 214h], 1
        xor    al, al
        jmp    NEAR PTR loc_1_2F41
loc_1_2F30:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 214h], 1
        mov    al, 1
        jmp    NEAR PTR loc_1_2F41
loc_1_2F41:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_2F48:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 21bh], 800h
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 243h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_2FA4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 25bh]
        shr    eax, 10h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 217h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 25bh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 219h], ax
        jmp    NEAR PTR loc_1_2FDB
loc_1_2FA4:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 25fh]
        and    eax, 0ff0000h
        shr    eax, 10h
        and    ax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 217h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 25fh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 219h], ax
loc_1_2FDB:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 215h], 0
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2C60
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_2FF9:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 21bh], 400h
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 243h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_3055
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 21dh]
        shr    eax, 10h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 217h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 21dh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 219h], ax
        jmp    NEAR PTR loc_1_308C
loc_1_3055:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 221h]
        and    eax, 0ff0000h
        shr    eax, 10h
        and    ax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 217h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 221h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 219h], ax
loc_1_308C:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 215h], 6
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2C60
        jmp    NEAR PTR loc_1_30A8
loc_1_30A8:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_30AF:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 7ffffffeh
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 95h]
        and    eax, 0ffffh
        test   ah, 20h
        je     NEAR PTR loc_1_30E2
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_3112
loc_1_30E2:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 95h]
        and    eax, 0ffffh
        test   ah, 40h
        je     NEAR PTR loc_1_310B
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 99h]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_3112
loc_1_310B:
        mov    dword ptr [ebp - 4], 0
loc_1_3112:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 9dh], ecx
        je     NEAR PTR loc_1_3193
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 21dh]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 0c0h
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 1], 0
        mov    eax, dword ptr [ebp - 4]
        shr    eax, 18h
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 4]
        shr    eax, 10h
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 4], ah
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 5], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2FF9
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_3187
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_319A
loc_1_3187:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 9dh], eax
loc_1_3193:
        xor    eax, eax
        jmp    NEAR PTR loc_1_319A
loc_1_319A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_31A1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 21dh]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 0c0h
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0a1h]
        shr    eax, 18h
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0a1h]
        shr    eax, 10h
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0a1h]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 4], ah
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0a1h]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 5], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2FF9
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_322A
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_3231
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_322A:
        xor    eax, eax
        jmp    NEAR PTR loc_1_3231
loc_1_3231:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_3238:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 8], 8618h
        mov    word ptr [ebp - 4], 8418h
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 95h]
        and    eax, 0ffffh
        test   al, 20h
        je     NEAR PTR loc_1_3287
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 22dh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        or     eax, ecx
        mov    word ptr [ebp - 0ch], ax
        jmp    NEAR PTR loc_1_32E2
loc_1_3287:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 95h]
        and    eax, 0ffffh
        test   ah, 80h
        je     NEAR PTR loc_1_32C2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 22dh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        or     eax, ecx
        mov    word ptr [ebp - 0ch], ax
        jmp    NEAR PTR loc_1_32E2
loc_1_32C2:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 22dh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        not    ecx
        and    eax, ecx
        mov    word ptr [ebp - 0ch], ax
loc_1_32E2:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 22dh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_3309
        xor    eax, eax
        jmp    NEAR PTR loc_1_33FD
loc_1_3309:
        mov    ax, word ptr [ebp - 0ch]
        mov    word ptr [ebp - 10h], ax
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 97fdh
        mov    word ptr [ebp - 10h], ax
        mov    ax, word ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 21dh]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 21bh], 2000h
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 243h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_3383
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 21dh]
        shr    eax, 10h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 217h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 21dh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 219h], ax
        jmp    NEAR PTR loc_1_33BA
loc_1_3383:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 221h]
        and    eax, 0ff0000h
        shr    eax, 10h
        and    ax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 217h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 221h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 219h], ax
loc_1_33BA:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 215h], 2
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2C60
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_33E8
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_33FD
loc_1_33E8:
        mov    ax, word ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 22dh], ax
        xor    eax, eax
        jmp    NEAR PTR loc_1_33FD
loc_1_33FD:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_3404:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 97h]
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 95h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 97h], ax
        mov    ax, word ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 95h], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_30AF
        test   eax, eax
        jne    NEAR PTR loc_1_3466
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3238
        test   eax, eax
        jne    NEAR PTR loc_1_3466
        xor    eax, eax
        jmp    NEAR PTR loc_1_34A4
loc_1_3466:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 97h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 95h], ax
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 97h], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_30AF
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3238
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_34A4
loc_1_34A4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
        db  000h
loc_1_34AC:
        db  0A0h, 068h, 05Ah, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_34D1
        db  0FEh, 005h, 068h, 05Ah, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_34D3
        push   0
        push   0
        db  0E8h, 034h, 0CBh, 0FEh, 0FFh
        cmp    eax, 0
        jne    SHORT loc_1_34D3
loc_1_34D1:
        clc
        ret
loc_1_34D3:
        stc
        ret
IRMATRAC_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_34E4
        db  0FFh, 024h, 085h, 0E6h, 034h, 000h, 000h
loc_1_34E4:
        clc
        ret
        db  0ACh, 034h, 000h, 000h, 0E4h, 034h, 000h, 000h, 0E4h, 034h, 000h, 000h, 0E4h, 034h, 000h, 000h
        db  0E4h, 034h, 000h, 000h, 0E4h, 034h, 000h, 000h, 0E4h, 034h, 000h, 000h, 0E4h, 034h, 000h, 000h
        db  0E4h, 034h, 000h, 000h, 0E4h, 034h, 000h, 000h, 0E4h, 034h, 000h, 000h, 0E4h, 034h, 000h, 000h
        db  0E4h, 034h, 000h, 000h, 0E4h, 034h, 000h, 000h, 0E4h, 034h, 000h, 000h, 0E4h, 034h, 000h, 000h
        db  0E4h, 034h, 000h, 000h, 0E4h, 034h, 000h, 000h, 0E4h, 034h, 000h, 000h, 0E4h, 034h, 000h, 000h
        db  0E4h, 034h, 000h, 000h, 0E4h, 034h, 000h, 000h, 0E4h, 034h, 000h, 000h, 0E4h, 034h, 000h, 000h
        db  0E4h, 034h, 000h, 000h, 0E4h, 034h, 000h, 000h, 0ACh, 034h, 000h, 000h, 000h, 000h
loc_1_3554:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 103h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 200h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_35A2
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 1ebh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ecx + 187h]
        jb     NEAR PTR loc_1_360E
loc_1_35A2:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 214h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_360E
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1ffh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_360E
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 5ah]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_360E
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 5ah], 1
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1ffh], 0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_4F2C
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1ffh], 1
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 5ah], 0
loc_1_360E:
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_5900
        cmp    dword ptr [ebp - 8], 0eh
        jb     NEAR PTR loc_1_3643
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 245h]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 8]
        jae    NEAR PTR loc_1_364D
loc_1_3643:
        mov    eax, 0c001000fh
        jmp    NEAR PTR loc_1_37C8
loc_1_364D:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 3dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_3731
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 1dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_3725
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 24h
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 10h]
        inc    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 1f7h], 0
        jne    NEAR PTR loc_1_36BB
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 1f7h], eax
        jmp    NEAR PTR loc_1_36CA
loc_1_36BB:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 1fbh]
        mov    dword ptr [ecx + 24h], eax
loc_1_36CA:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 1fbh], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 214h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_3720
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1ffh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_3720
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1ffh], 0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_37CF
loc_1_3720:
        jmp    NEAR PTR loc_1_372C
loc_1_3725:
        mov    dword ptr [ebp - 0ch], 0c0010002h
loc_1_372C:
        jmp    NEAR PTR loc_1_378F
loc_1_3731:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 22bh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_3753
        mov    dword ptr [ebp - 0ch], 0c001000dh
        jmp    NEAR PTR loc_1_378F
loc_1_3753:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 30h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_3772
        mov    dword ptr [ebp - 0ch], 0c0000001h
        jmp    NEAR PTR loc_1_378F
loc_1_3772:
        push   2
        push   0bh
        push   2
        push   0aa000006h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 81h]
        push   eax
        call   sub_1_59A8
        add    esp, 14h
loc_1_378F:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 22bh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_37C0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    ecx, 69h
        cmp    dword ptr [eax + 69h], ecx
        je     NEAR PTR loc_1_37C0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_502C
loc_1_37C0:
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_37C8
loc_1_37C8:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_37CF:
        push   ebp
        mov    ebp, esp
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 10h], 0
        mov    dword ptr [ebp - 1ch], 0
        mov    word ptr [ebp - 18h], 0
loc_1_37EC:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1f7h], 0
        je     NEAR PTR loc_1_3B66
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1f7h]
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [ebp - 24h]
        add    eax, 24h
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 200h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_3846
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1ebh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 187h]
        jb     NEAR PTR loc_1_384B
loc_1_3846:
        jmp    NEAR PTR loc_1_3B66
loc_1_384B:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        test   eax, eax
        je     NEAR PTR loc_1_385D
        jmp    NEAR PTR loc_1_385D
loc_1_385D:
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        push   0
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        call   sub_1_5900
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        test   eax, eax
        je     NEAR PTR loc_1_3886
        jmp    NEAR PTR loc_1_3886
loc_1_3886:
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 200h]
        cmp    dword ptr [ebp - 10h], 100h
        jae    NEAR PTR loc_1_3977
        mov    word ptr [ebp - 18h], 0
        mov    eax, dword ptr [ebp - 28h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1f7h], eax
loc_1_38B1:
        cmp    dword ptr [ebp - 20h], 0
        je     NEAR PTR loc_1_3912
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        call   sub_1_5924
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1efh]
        mov    eax, dword ptr [eax + 14h]
        mov    ecx, dword ptr [ebp - 18h]
        and    ecx, 0ffffh
        add    eax, ecx
        push   eax
        call   sub_1_59BC
        lea    eax, [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        call   sub_1_5948
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        add    eax, dword ptr [ebp - 8]
        mov    word ptr [ebp - 18h], ax
        jmp    NEAR PTR loc_1_38B1
loc_1_3912:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 1efh]
        mov    cx, word ptr [ecx + 4]
        and    ecx, 0ffffh
        add    ecx, 0ch
        mov    dx, word ptr [eax + 23dh]
        mov    ax, cx
        out    dx, ax
        mov    eax, dword ptr [ebp - 24h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 1efh]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1efh]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1efh], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 10h]
        or     ecx, 2100h
        mov    dx, word ptr [eax + 231h]
        mov    ax, cx
        out    dx, ax
        jmp    NEAR PTR loc_1_3B61
loc_1_3977:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1efh]
        mov    byte ptr [eax + 1ah], 1
        mov    eax, dword ptr [ebp - 28h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1f7h], eax
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 1ebh]
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        test   eax, eax
        jne    NEAR PTR loc_1_39AE
        jmp    NEAR PTR loc_1_39AE
loc_1_39AE:
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1edh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 18bh]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        call   sub_1_3BFC
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        test   eax, eax
        jne    NEAR PTR loc_1_39ED
        jmp    NEAR PTR loc_1_39ED
loc_1_39ED:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 1efh]
        mov    cx, word ptr [ecx + 4]
        and    ecx, 0ffffh
        add    ecx, 6
        mov    dx, word ptr [eax + 23dh]
        mov    ax, cx
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 233h]
        mov    ax, word ptr [ebp - 10h]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 243h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_3A9E
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1edh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0c7h]
        mov    dword ptr [ebp - 30h], eax
        lea    eax, [ebp - 14h]
        mov    dword ptr [ebp - 2ch], eax
        mov    eax, dword ptr [ebp - 30h]
        mov    ecx, dword ptr [ebp - 2ch]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [ebp - 30h]
        and    eax, 0ff0000h
        shr    eax, 10h
        and    ax, 0ffh
        mov    ecx, dword ptr [ebp - 2ch]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 233h]
        mov    ax, word ptr [ebp - 14h]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 233h]
        mov    ax, word ptr [ebp - 12h]
        out    dx, ax
        jmp    NEAR PTR loc_1_3AF9
loc_1_3A9E:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 1edh]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + ecx*4 + 18bh]
        shr    ecx, 10h
        mov    dx, word ptr [eax + 233h]
        mov    ax, cx
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 1edh]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    cx, word ptr [edx + ecx*4 + 18bh]
        and    ecx, 0ffffh
        mov    dx, word ptr [eax + 233h]
        mov    eax, ecx
        out    dx, ax
loc_1_3AF9:
        mov    eax, dword ptr [ebp - 24h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 1efh]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 1edh]
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1edh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 187h]
        jne    NEAR PTR loc_1_3B3C
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1edh], 0
loc_1_3B3C:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1efh]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1efh], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 231h]
        mov    eax, 100h
        out    dx, ax
loc_1_3B61:
        jmp    NEAR PTR loc_1_37EC
loc_1_3B66:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1ffh], 1
        jmp    NEAR PTR loc_1_3B75
loc_1_3B75:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
loc_1_3B7C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 5dh]
        cmp    byte ptr [esi + 3dh], 0
        jne    SHORT loc_1_3BC1
        cmp    byte ptr [edi + 1dh], 0
        jne    SHORT loc_1_3BBA
        mov    eax, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [ebp + 18h]
        push   eax
        mov    edx, dword ptr [ebp + 14h]
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   ecx
        add    eax, dword ptr [ebp + 10h]
        push   edx
        inc    dword ptr [edi + 14h]
        push   eax
        call   sub_1_3CDC
        dec    dword ptr [edi + 14h]
        xor    edi, edi
        jmp    SHORT loc_1_3BC6
loc_1_3BBA:
        mov    edi, 0c001000ch
        jmp    SHORT loc_1_3BC6
loc_1_3BC1:
        mov    edi, 0c001000dh
loc_1_3BC6:
        mov    eax, dword ptr [esi + 5dh]
        dec    eax
        mov    dword ptr [esi + 5dh], eax
        jne    SHORT loc_1_3BE8
        cmp    byte ptr [esi + 22bh], 0
        jne    SHORT loc_1_3BE2
        mov    eax, dword ptr [esi + 69h]
        sub    eax, esi
        cmp    eax, 69h
        je     SHORT loc_1_3BE8
loc_1_3BE2:
        push   esi
        call   sub_1_502C
loc_1_3BE8:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    18h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3BFC:
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 10h], esi
        je     NEAR PTR loc_1_3CC5
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 10h]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   edx
        call   sub_1_5900
        cmp    dword ptr [ebp - 10h], esi
        je     NEAR PTR loc_1_3CC5
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_5924
        cmp    dword ptr [ebp + 10h], esi
        je     SHORT loc_1_3CC0
        mov    edi, dword ptr [ebp + 0ch]
loc_1_3C4B:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_3C77
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_5948
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_3CC0
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_5924
        jmp    SHORT loc_1_3CBB
loc_1_3C77:
        test   edi, edi
        je     SHORT loc_1_3C94
        cmp    dword ptr [ebp - 4], edi
        jae    SHORT loc_1_3C8C
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_3CBB
loc_1_3C8C:
        add    dword ptr [ebp - 0ch], edi
        sub    dword ptr [ebp - 4], edi
        xor    edi, edi
loc_1_3C94:
        mov    ebx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, esi
        cmp    ebx, eax
        jb     SHORT loc_1_3CA2
        mov    ebx, eax
loc_1_3CA2:
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    ecx, dword ptr [ebp + 14h]
        push   ecx
        add    esi, ebx
        call   sub_1_59BC
        add    dword ptr [ebp + 14h], ebx
        add    dword ptr [ebp - 0ch], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_3CBB:
        cmp    esi, dword ptr [ebp + 10h]
        jb     SHORT loc_1_3C4B
loc_1_3CC0:
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], esi
loc_1_3CC5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3CDC:
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 0ch], esi
        je     NEAR PTR loc_1_3DA8
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 14h]
        push   ecx
        mov    edx, dword ptr [ebp + 10h]
        push   esi
        push   edx
        call   sub_1_5900
        cmp    dword ptr [ebp - 14h], esi
        je     NEAR PTR loc_1_3DA8
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_5924
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0ch], esi
        mov    dword ptr [ebp - 10h], ecx
        je     SHORT loc_1_3DA3
        mov    edi, dword ptr [ebp + 14h]
loc_1_3D31:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_3D5D
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_5948
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_3DA3
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_5924
        jmp    SHORT loc_1_3D9E
loc_1_3D5D:
        test   edi, edi
        je     SHORT loc_1_3D7A
        cmp    dword ptr [ebp - 4], edi
        jae    SHORT loc_1_3D72
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_3D9E
loc_1_3D72:
        add    dword ptr [ebp - 0ch], edi
        sub    dword ptr [ebp - 4], edi
        xor    edi, edi
loc_1_3D7A:
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, esi
        cmp    ebx, eax
        jb     SHORT loc_1_3D88
        mov    ebx, eax
loc_1_3D88:
        push   ebx
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        add    esi, ebx
        call   sub_1_59BC
        add    dword ptr [ebp - 10h], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_3D9E:
        cmp    esi, dword ptr [ebp + 0ch]
        jb     SHORT loc_1_3D31
loc_1_3DA3:
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], esi
loc_1_3DA8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 01Ch
        db  08Bh, 0ECh, 083h, 0ECh, 024h, 0C7h, 000h, 000h, 000h, 000h, 000h, 053h, 0C7h, 045h, 0E4h, 000h
        db  000h, 000h, 000h, 083h, 07Dh, 010h, 000h, 056h, 057h, 00Fh, 084h, 056h, 001h, 000h, 000h, 06Ah
        db  000h, 08Dh, 045h, 0F0h, 050h, 08Dh, 04Dh, 0DCh, 051h, 08Bh, 055h, 008h, 06Ah, 000h, 052h, 0E8h
        db  00Bh, 01Bh, 000h, 000h, 083h, 07Dh, 0DCh, 000h, 00Fh, 084h, 037h, 001h, 000h, 000h, 08Dh, 045h
        db  0FCh, 08Dh, 04Dh, 0E8h, 050h, 08Bh, 055h, 0F0h, 051h, 052h, 0E8h, 014h, 01Bh, 000h, 000h, 06Ah
        db  000h, 08Dh, 04Dh, 0F4h, 051h, 08Dh, 055h, 0E0h, 052h, 08Bh, 045h, 014h, 06Ah, 000h, 050h, 0E8h
        db  0DBh, 01Ah, 000h, 000h, 083h, 07Dh, 0E0h, 000h, 00Fh, 084h, 007h, 001h, 000h, 000h, 08Dh, 045h
        db  0F8h, 08Dh, 04Dh, 0ECh, 050h, 08Bh, 055h, 0F4h, 051h, 052h, 0E8h, 0E4h, 01Ah, 000h, 000h, 083h
        db  07Dh, 010h, 000h, 00Fh, 084h, 0E4h, 000h, 000h, 000h, 08Bh, 075h, 00Ch, 08Bh, 07Dh, 018h, 083h
        db  07Dh, 0FCh, 000h, 075h, 02Dh, 08Dh, 045h, 0F0h, 08Bh, 04Dh, 0F0h, 050h, 051h, 0E8h, 0E5h, 01Ah
        db  000h, 000h, 083h, 07Dh, 0F0h, 000h, 00Fh, 084h, 0C1h, 000h, 000h, 000h, 08Dh, 045h, 0FCh, 08Dh
        db  04Dh, 0E8h, 050h, 08Bh, 055h, 0F0h, 051h, 052h, 0E8h, 0A6h, 01Ah, 000h, 000h, 0E9h, 09Fh, 000h
        db  000h, 000h, 083h, 07Dh, 0F8h, 000h, 075h, 02Ah, 08Dh, 045h, 0F4h, 08Bh, 04Dh, 0F4h, 050h, 051h
        db  0E8h, 0B2h, 01Ah, 000h, 000h, 083h, 07Dh, 0F4h, 000h, 00Fh, 084h, 08Eh, 000h, 000h, 000h, 08Dh
        db  045h, 0F8h, 08Dh, 04Dh, 0ECh, 050h, 08Bh, 055h, 0F4h, 051h, 052h, 0E8h, 073h, 01Ah, 000h, 000h
        db  0EBh, 06Fh, 085h, 0F6h, 074h, 019h, 03Bh, 075h, 0FCh, 076h, 00Ch, 02Bh, 075h, 0FCh, 0C7h, 045h
        db  0FCh, 000h, 000h, 000h, 000h, 0EBh, 05Ah, 001h, 075h, 0E8h, 029h, 075h, 0FCh, 033h, 0F6h, 085h
        db  0FFh, 074h, 019h, 03Bh, 07Dh, 0F8h, 076h, 00Ch, 02Bh, 07Dh, 0F8h, 0C7h, 045h, 0F8h, 000h, 000h
        db  000h, 000h, 0EBh, 03Dh, 001h, 07Dh, 0ECh, 029h, 07Dh, 0F8h, 033h, 0FFh, 08Bh, 05Dh, 0F8h, 08Bh
        db  045h, 0FCh, 03Bh, 0D8h, 072h, 002h, 08Bh, 0D8h, 08Bh, 045h, 010h, 02Bh, 045h, 0E4h, 03Bh, 0D8h
        db  072h, 002h, 08Bh, 0D8h, 053h, 08Bh, 045h, 0ECh, 050h, 08Bh, 04Dh, 0E8h, 051h, 0E8h, 0A9h, 01Ah
        db  000h, 000h, 001h, 05Dh, 0E8h, 001h, 05Dh, 0ECh, 001h, 05Dh, 0E4h, 029h, 05Dh, 0F8h, 029h, 05Dh
        db  0FCh, 08Bh, 04Dh, 0E4h, 039h, 04Dh, 010h, 00Fh, 087h, 022h, 0FFh, 0FFh, 0FFh, 08Bh, 045h, 01Ch
        db  08Bh, 04Dh, 0E4h, 089h, 008h, 05Fh, 05Eh, 05Bh, 08Bh, 0E5h, 05Dh, 0C2h, 018h, 000h, 08Dh, 09Bh
        db  000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3F4C:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        inc    dword ptr [edi + 5dh]
        mov    edx, dword ptr [ecx + 10h]
        test   edx, edx
        jne    SHORT loc_1_3F73
        push   ecx
        push   eax
        push   edi
        call   sub_1_443C
        mov    esi, eax
        jmp    SHORT loc_1_3FD5
loc_1_3F73:
        cmp    edx, 1
        jne    SHORT loc_1_3FD0
        cmp    byte ptr [edi + 3dh], 0
        jne    SHORT loc_1_3F97
        cmp    byte ptr [eax + 1dh], 0
        jne    SHORT loc_1_3F90
        push   ecx
        push   eax
        push   edi
        call   sub_1_448C
        mov    esi, eax
        jmp    SHORT loc_1_3FD5
loc_1_3F90:
        mov    esi, 0c0010002h
        jmp    SHORT loc_1_3FD5
loc_1_3F97:
        cmp    byte ptr [edi + 22bh], 1
        jne    SHORT loc_1_3FA7
        mov    esi, 0c001000dh
        jmp    SHORT loc_1_3FD5
loc_1_3FA7:
        cmp    byte ptr [edi + 30h], 1
        jne    SHORT loc_1_3FB4
        mov    esi, 0c0000001h
        jmp    SHORT loc_1_3FD5
loc_1_3FB4:
        push   3
        mov    eax, dword ptr [edi + 81h]
        push   0bh
        push   2
        push   0aa000006h
        push   eax
        call   sub_1_59A8
        add    esp, 14h
        jmp    SHORT loc_1_3FD5
loc_1_3FD0:
        mov    esi, 10001h
loc_1_3FD5:
        mov    eax, dword ptr [edi + 5dh]
        dec    eax
        mov    dword ptr [edi + 5dh], eax
        jne    SHORT loc_1_3FF7
        cmp    byte ptr [edi + 22bh], 0
        jne    SHORT loc_1_3FF1
        mov    eax, dword ptr [edi + 69h]
        sub    eax, edi
        cmp    eax, 69h
        je     SHORT loc_1_3FF7
loc_1_3FF1:
        push   edi
        call   sub_1_502C
loc_1_3FF7:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_400C:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        mov    dword ptr [ebp - 10h], eax
        push   esi
        mov    ecx, dword ptr [ebp + 10h]
        push   edi
        mov    esi, 4
        lea    edi, [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
        cmp    ecx, 2010101h
        mov    eax, ecx
        mov    dword ptr [ebp - 14h], 1
        ja     SHORT loc_1_4050
        je     NEAR PTR loc_1_4286
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_1_4061
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_4050:
        sub    eax, 2010102h
        cmp    eax, 5
        ja     SHORT loc_1_4061
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_4061:
        mov    dword ptr [ebp - 10h], 0c00000bbh
        jmp    NEAR PTR loc_1_4399
loc_1_406D:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_1_4082
        db  0BFh, 000h, 000h, 000h, 000h
        mov    esi, 68h
        jmp    NEAR PTR loc_1_4399
loc_1_4082:
        db  0BFh, 000h, 000h, 000h, 000h
        mov    esi, 0a0h
        jmp    NEAR PTR loc_1_4399
loc_1_4091:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 22bh], 0
        je     SHORT loc_1_40A6
        mov    dword ptr [ebp - 0ch], 2
        jmp    SHORT loc_1_40D1
loc_1_40A6:
        cmp    byte ptr [ebx + 2fh], 0
        jne    SHORT loc_1_40CA
        cmp    byte ptr [ebx + 31h], 0
        jne    SHORT loc_1_40CA
        cmp    byte ptr [ebx + 3dh], 0
        je     SHORT loc_1_40C1
        mov    dword ptr [ebp - 0ch], 4
        jmp    SHORT loc_1_40D1
loc_1_40C1:
        mov    dword ptr [ebp - 0ch], 0
        jmp    SHORT loc_1_40D1
loc_1_40CA:
        mov    dword ptr [ebp - 0ch], 1
loc_1_40D1:
        lea    edi, [ebp - 0ch]
        mov    esi, 4
        jmp    NEAR PTR loc_1_4399
loc_1_40DE:
        lea    edi, [ebp - 14h]
        mov    esi, 4
        jmp    NEAR PTR loc_1_4399
loc_1_40EB:
        mov    dword ptr [ebp - 8], 100h
        jmp    NEAR PTR loc_1_4399
loc_1_40F7:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 245h]
        cmp    ecx, 10106h
        mov    dword ptr [ebp - 8], eax
        jne    NEAR PTR loc_1_4399
        sub    dword ptr [ebp - 8], 0eh
        jmp    NEAR PTR loc_1_4399
loc_1_411B:
        mov    ebx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebx + 244h]
        and    al, 1
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0fffe2b40h
        add    eax, 27100h
        jmp    NEAR PTR loc_1_4396
loc_1_4139:
        mov    ebx, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 0a7h]
        xor    eax, eax
        mov    ax, word ptr [ebx + 245h]
        imul   ecx, eax
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_1_4399
loc_1_4159:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0abh]
        imul   eax, dword ptr [ebx + 247h]
        jmp    NEAR PTR loc_1_4396
loc_1_4171:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 245h]
        jmp    NEAR PTR loc_1_4396
loc_1_4182:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 247h]
        jmp    NEAR PTR loc_1_4396
loc_1_4190:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        lea    ecx, [ebp - 8]
        lea    eax, [ebx + 3fh]
        push   eax
        push   ecx
        call   sub_1_59BC
        and    dword ptr [ebp - 8], 0ffffff00h
        test   byte ptr [ebx + 244h], 10h
        jne    NEAR PTR loc_1_4399
        or     dword ptr [ebp - 8], 1
        jmp    NEAR PTR loc_1_4399
loc_1_41BF:
        mov    ebx, dword ptr [ebp + 8]
        test   byte ptr [ebx + 244h], 10h
        jne    SHORT loc_1_41DA
        db  0BFh, 000h, 000h, 000h, 000h
        mov    esi, 31h
        jmp    NEAR PTR loc_1_4399
loc_1_41DA:
        db  0BFh, 000h, 000h, 000h, 000h
        mov    esi, 2eh
        jmp    NEAR PTR loc_1_4399
loc_1_41E9:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_4200
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 95h]
        jmp    NEAR PTR loc_1_4396
loc_1_4200:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 0ch]
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 8], edx
        jmp    NEAR PTR loc_1_4399
loc_1_4211:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_1_4225
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_1_4399
loc_1_4225:
        mov    dword ptr [ebp - 8], 0
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, ebx
        mov    ecx, dword ptr [ebx + 61h]
        sub    eax, ecx
        cmp    eax, -61h
        je     NEAR PTR loc_1_4399
        mov    edx, 100h
loc_1_4244:
        mov    eax, dword ptr [ecx + 18h]
        cmp    dword ptr [ebp - 8], eax
        jae    SHORT loc_1_4257
        mov    dword ptr [ebp - 8], eax
        cmp    eax, edx
        je     NEAR PTR loc_1_4399
loc_1_4257:
        mov    ecx, dword ptr [ecx]
        mov    eax, ebx
        sub    eax, ecx
        cmp    eax, -61h
        jne    SHORT loc_1_4244
        jmp    NEAR PTR loc_1_4399
loc_1_4267:
        lea    edi, [ebp - 2]
        mov    esi, 2
        mov    word ptr [ebp - 2], 300h
        jmp    NEAR PTR loc_1_4399
loc_1_427A:
        mov    dword ptr [ebp - 8], 6
        jmp    NEAR PTR loc_1_4399
loc_1_4286:
        mov    ebx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebx + 3fh]
        add    ebx, 3fh
        mov    byte ptr [ebp - 1ch], al
        mov    cl, byte ptr [ebx + 1]
        mov    byte ptr [ebp - 1bh], cl
        mov    dl, byte ptr [ebx + 2]
        mov    byte ptr [ebp - 1ah], dl
        lea    edi, [ebp - 1ch]
        mov    al, byte ptr [ebx + 3]
        mov    esi, 6
        mov    byte ptr [ebp - 19h], al
        mov    cl, byte ptr [ebx + 4]
        mov    byte ptr [ebp - 18h], cl
        mov    dl, byte ptr [ebx + 5]
        mov    byte ptr [ebp - 17h], dl
        jmp    NEAR PTR loc_1_4399
loc_1_42BD:
        mov    ebx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebx + 45h]
        add    ebx, 45h
        mov    byte ptr [ebp - 1ch], al
        mov    cl, byte ptr [ebx + 1]
        mov    byte ptr [ebp - 1bh], cl
        mov    dl, byte ptr [ebx + 2]
        mov    byte ptr [ebp - 1ah], dl
        lea    edi, [ebp - 1ch]
        mov    al, byte ptr [ebx + 3]
        mov    esi, 6
        mov    byte ptr [ebp - 19h], al
        mov    cl, byte ptr [ebx + 4]
        mov    byte ptr [ebp - 18h], cl
        mov    dl, byte ptr [ebx + 5]
        mov    byte ptr [ebp - 17h], dl
        jmp    NEAR PTR loc_1_4399
loc_1_42F4:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_1_4308
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 0ch]
        mov    edx, dword ptr [ecx + 0ch]
        mov    dword ptr [ebp - 8], edx
        jmp    SHORT loc_1_4314
loc_1_4308:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 9dh]
        mov    dword ptr [ebp - 8], eax
loc_1_4314:
        mov    edx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 10h
        and    edx, 0ff00h
        or     edx, eax
        mov    ecx, dword ptr [ebp - 8]
        shl    edx, 8
        mov    eax, dword ptr [ebp - 8]
        shr    ecx, 10h
        and    eax, 0ff0000h
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [ebp - 8], edx
        jmp    SHORT loc_1_4399
loc_1_4342:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0a1h]
        mov    edx, eax
        mov    dword ptr [ebp - 8], eax
        shl    eax, 10h
        and    edx, 0ff00h
        or     edx, eax
        mov    ecx, dword ptr [ebp - 8]
        shl    edx, 8
        mov    eax, dword ptr [ebp - 8]
        shr    ecx, 10h
        and    eax, 0ff0000h
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [ebp - 8], edx
        jmp    SHORT loc_1_4399
loc_1_4378:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 33h]
        or     eax, 0c0011000h
        jmp    SHORT loc_1_4396
loc_1_4388:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 35h]
        jmp    SHORT loc_1_4396
loc_1_4390:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 39h]
loc_1_4396:
        mov    dword ptr [ebp - 8], eax
loc_1_4399:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_43CC
        cmp    esi, dword ptr [ebp + 1ch]
        jbe    SHORT loc_1_43BC
        mov    eax, dword ptr [ebp + 20h]
        pop    edi
        mov    dword ptr [ebp - 10h], 0c0010014h
        mov    dword ptr [eax], esi
        mov    eax, dword ptr [ebp - 10h]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_1_43BC:
        push   esi
        mov    eax, dword ptr [ebp + 18h]
        push   edi
        push   eax
        call   sub_1_59BC
        mov    eax, dword ptr [ebp + 24h]
        add    dword ptr [eax], esi
loc_1_43CC:
        mov    eax, dword ptr [ebp - 10h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
sub_1_443C:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    edx, [ebp - 4]
        push   edx
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [esi + 1ch]
        mov    edx, dword ptr [esi + 18h]
        push   eax
        push   edx
        push   ecx
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 14h]
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   eax
        push   edx
        call   sub_1_400C
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 20h], ecx
        mov    dword ptr [esi + 24h], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_448C:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        xor    ebx, ebx
        mov    ecx, dword ptr [esi + 18h]
        mov    eax, dword ptr [esi + 14h]
        mov    edi, dword ptr [esi + 1ch]
        cmp    edi, 4
        je     SHORT loc_1_44B2
        mov    ebx, 0c0010014h
        jmp    NEAR PTR loc_1_45A6
loc_1_44B2:
        cmp    eax, 2010103h
        ja     SHORT loc_1_44E2
        je     NEAR PTR loc_1_4584
        cmp    eax, 1010eh
        je     SHORT loc_1_44F7
        cmp    eax, 1010fh
        je     SHORT loc_1_451A
        cmp    eax, 10112h
        je     NEAR PTR loc_1_4580
        mov    ebx, 0c0010017h
        jmp    NEAR PTR loc_1_45A6
loc_1_44E2:
        cmp    eax, 2010104h
        je     NEAR PTR loc_1_4595
        mov    ebx, 0c0010017h
        jmp    NEAR PTR loc_1_45A6
loc_1_44F7:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_59BC
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edx
        call   sub_1_45DC
        jmp    NEAR PTR loc_1_45A4
loc_1_451A:
        push   4
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        call   sub_1_59BC
        cmp    dword ptr [ebp - 4], 100h
        jbe    SHORT loc_1_4536
        mov    ebx, 0c0010014h
        jmp    SHORT loc_1_45A6
loc_1_4536:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 264h]
        cmp    dword ptr [ebp - 4], edx
        jbe    SHORT loc_1_4555
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 18h], edx
        mov    dword ptr [eax + 264h], edx
        jmp    SHORT loc_1_45A6
loc_1_4555:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 18h]
        cmp    ecx, edx
        jne    SHORT loc_1_4575
        cmp    dword ptr [ebp - 4], ecx
        jae    SHORT loc_1_4575
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        mov    dword ptr [ecx + 18h], edx
        call   sub_1_0F8E
        jmp    SHORT loc_1_45A6
loc_1_4575:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 18h], eax
        jmp    SHORT loc_1_45A6
loc_1_4580:
        xor    ebx, ebx
        jmp    SHORT loc_1_45A6
loc_1_4584:
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   esi
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        push   eax
        call   sub_1_48BC
        jmp    SHORT loc_1_45A4
loc_1_4595:
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   esi
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        push   eax
        call   sub_1_48FC
loc_1_45A4:
        mov    ebx, eax
loc_1_45A6:
        cmp    ebx, 103h
        je     SHORT loc_1_45CB
        push   ebx
        mov    ecx, dword ptr [ebp + 0ch]
        push   esi
        mov    ebx, 103h
        mov    dword ptr [esi + 24h], 0
        mov    dword ptr [esi + 20h], edi
        mov    eax, dword ptr [ecx + 10h]
        push   eax
        call   sub_1_582C
loc_1_45CB:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_45DC:
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        mov    ebp, esp
        test   al, 56h
        push   esi
        push   edi
        je     SHORT loc_1_45F4
        mov    eax, 0c00000bbh
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
loc_1_45F4:
        mov    esi, dword ptr [ebp + 0ch]
        push   1
        push   eax
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [esi + 0ch]
        push   eax
        inc    dword ptr [esi + 14h]
        push   ecx
        mov    eax, dword ptr [edi + 85h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4614:
        dec    dword ptr [esi + 14h]
        cmp    eax, 103h
        jne    SHORT loc_1_4624
        push   edi
        call   sub_1_2C1E
loc_1_4624:
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
sub_1_462C:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 8], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        lea    edx, [ebp - 0ch]
        mov    eax, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 18h]
        push   edx
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 14h], ecx
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 10h]
        push   eax
        mov    edx, dword ptr [ebp - 14h]
        push   ecx
        mov    eax, dword ptr [esi + 14h]
        push   edx
        mov    ebx, dword ptr [ebp + 8]
        push   1
        push   eax
        push   0
        push   ebx
        call   sub_1_400C
        cmp    eax, 0c00000bbh
        mov    edi, eax
        jne    NEAR PTR loc_1_477C
        xor    edi, edi
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 2020101h
        ja     SHORT loc_1_469F
        je     SHORT loc_1_4703
        sub    eax, 20101h
        cmp    eax, 4
        ja     SHORT loc_1_46AF
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_469F:
        cmp    eax, 2020201h
        ja     SHORT loc_1_46B9
        je     SHORT loc_1_471C
        cmp    eax, 2020102h
        je     SHORT loc_1_470F
loc_1_46AF:
        mov    edi, 0c0010017h
        jmp    NEAR PTR loc_1_474D
loc_1_46B9:
        sub    eax, 2020202h
        cmp    eax, 5
        ja     SHORT loc_1_46AF
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_46CA:
        mov    eax, dword ptr [ebx + 24bh]
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_1_474D
loc_1_46D5:
        mov    eax, dword ptr [ebx + 24fh]
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_1_474D
loc_1_46E0:
        mov    eax, dword ptr [ebx + 253h]
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_1_474D
loc_1_46EB:
        mov    eax, dword ptr [ebx + 257h]
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_1_474D
loc_1_46F6:
        mov    eax, dword ptr [ebx + 25bh]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 6]
        jmp    SHORT loc_1_474A
loc_1_4703:
        mov    eax, dword ptr [ebx + 25bh]
        xor    ecx, ecx
        mov    cl, byte ptr [eax]
        jmp    SHORT loc_1_474A
loc_1_470F:
        mov    eax, dword ptr [ebx + 25bh]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 5]
        jmp    SHORT loc_1_474A
loc_1_471C:
        mov    eax, dword ptr [ebx + 25bh]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 2]
        jmp    SHORT loc_1_474A
loc_1_4729:
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_474D
loc_1_4732:
        mov    eax, dword ptr [ebx + 25bh]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 7]
        jmp    SHORT loc_1_474A
loc_1_473F:
        mov    eax, dword ptr [ebx + 25bh]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 9]
loc_1_474A:
        mov    dword ptr [ebp - 4], ecx
loc_1_474D:
        test   edi, edi
        jne    SHORT loc_1_477C
        cmp    dword ptr [ebp - 10h], 4
        jb     SHORT loc_1_476C
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 14h]
        push   ecx
        call   sub_1_59BC
        add    dword ptr [ebp - 0ch], 4
        jmp    SHORT loc_1_477C
loc_1_476C:
        mov    eax, 4
        mov    edi, 0c0010014h
        sub    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 8], eax
loc_1_477C:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], ecx
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h, 000h, 000h
        db  000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_47CC:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    edi, edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        inc    dword ptr [esi + 5dh]
        mov    eax, dword ptr [ebx + 14h]
        cmp    eax, 10111h
        ja     SHORT loc_1_4803
        cmp    eax, 1010eh
        jae    SHORT loc_1_4859
        cmp    eax, 10101h
        jb     SHORT loc_1_47FC
        cmp    eax, 1010ch
        jbe    SHORT loc_1_4859
loc_1_47FC:
        mov    edi, 0c0010017h
        jmp    SHORT loc_1_4859
loc_1_4803:
        cmp    eax, 20105h
        ja     SHORT loc_1_481F
        cmp    eax, 20101h
        jae    SHORT loc_1_4853
        cmp    eax, 10113h
        je     SHORT loc_1_4859
        mov    edi, 0c0010017h
        jmp    SHORT loc_1_4859
loc_1_481F:
        cmp    eax, 2020102h
        ja     SHORT loc_1_4842
        cmp    eax, 2020101h
        jae    SHORT loc_1_4853
        cmp    eax, 2010101h
        jb     SHORT loc_1_47FC
        cmp    eax, 2010107h
        jbe    SHORT loc_1_4859
        mov    edi, 0c0010017h
        jmp    SHORT loc_1_4859
loc_1_4842:
        sub    eax, 2020201h
        cmp    eax, 6
        ja     SHORT loc_1_47FC
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_4853:
        push   esi
        call   sub_1_2F48
loc_1_4859:
        test   edi, edi
        jne    SHORT loc_1_4866
        push   ebx
        push   esi
        call   sub_1_462C
        mov    edi, eax
loc_1_4866:
        mov    eax, dword ptr [esi + 5dh]
        dec    eax
        mov    dword ptr [esi + 5dh], eax
        jne    SHORT loc_1_4888
        cmp    byte ptr [esi + 22bh], 0
        jne    SHORT loc_1_4882
        mov    eax, dword ptr [esi + 69h]
        sub    eax, esi
        cmp    eax, 69h
        je     SHORT loc_1_4888
loc_1_4882:
        push   esi
        call   sub_1_502C
loc_1_4888:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h
        db  000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_48BC:
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        mov    ebp, esp
        push   esi
        push   1
        push   eax
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        push   ecx
        mov    edx, dword ptr [esi + 0ch]
        push   edx
        mov    eax, dword ptr [esi + 8]
        inc    dword ptr [esi + 14h]
        mov    ecx, dword ptr [eax + 85h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_48E4:
        dec    dword ptr [esi + 14h]
        cmp    eax, 103h
        jne    SHORT loc_1_48F7
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2C1E
loc_1_48F7:
        pop    esi
        pop    ebp
        ret    10h
sub_1_48FC:
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        mov    ebp, esp
        push   esi
        push   1
        push   eax
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        push   ecx
        mov    edx, dword ptr [esi + 0ch]
        push   edx
        mov    eax, dword ptr [esi + 8]
        inc    dword ptr [esi + 14h]
        mov    ecx, dword ptr [eax + 85h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4924:
        dec    dword ptr [esi + 14h]
        cmp    eax, 103h
        jne    SHORT loc_1_4937
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2C1E
loc_1_4937:
        pop    esi
        pop    ebp
        ret    10h
loc_1_493C:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    si, word ptr [ecx + 8]
        mov    edx, dword ptr [ecx + 4]
        mov    eax, dword ptr [ecx]
        mov    ax, word ptr [eax + 5bh]
        and    ax, si
        not    si
        or     word ptr [edx], ax
        mov    al, 1
        mov    ecx, dword ptr [ecx]
        and    word ptr [ecx + 5bh], si
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_496C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dx, word ptr [edi + 23bh]
        in     al, dx
        and    al, 3
        cmp    al, 3
        jne    SHORT loc_1_49DB
        mov    dx, word ptr [edi + 23fh]
        push   edi
        in     ax, dx
        mov    bx, ax
        mov    dx, word ptr [edi + 23dh]
        in     ax, dx
        mov    si, ax
        call   sub_1_52AC
        mov    dx, word ptr [edi + 23fh]
        mov    ax, bx
        out    dx, ax
        mov    dx, word ptr [edi + 23dh]
        mov    ax, si
        out    dx, ax
loc_1_49B9:
        test   byte ptr [edi + 244h], 10h
        jne    SHORT loc_1_49CC
        mov    dx, word ptr [edi + 241h]
        mov    al, 0ffh
        out    dx, al
loc_1_49CC:
        cmp    byte ptr [edi + 30h], 1
        sbb    al, al
        pop    edi
        neg    al
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_49DB:
        mov    dx, word ptr [edi + 237h]
        in     al, dx
        and    al, 7fh
        out    dx, al
        cmp    byte ptr [edi + 31h], 0
        je     SHORT loc_1_49B9
        mov    bx, word ptr [edi + 23dh]
        mov    dx, bx
        in     ax, dx
        mov    cx, ax
        mov    eax, 0a30h
        out    dx, ax
        mov    si, word ptr [edi + 231h]
        mov    dx, si
        in     ax, dx
        test   ah, 10h
        je     SHORT loc_1_4A21
        mov    word ptr [edi + 5bh], 4
        mov    word ptr [edi + 33h], 0
        jmp    SHORT loc_1_4A5B
loc_1_4A21:
        mov    dx, bx
        mov    eax, 0a42h
        out    dx, ax
        mov    dx, si
        in     ax, dx
        test   al, 8
        je     SHORT loc_1_4A5B
        test   ah, 1
        je     SHORT loc_1_4A5B
        xor    eax, eax
        out    dx, ax
        mov    dx, bx
        mov    eax, 0a3ch
        out    dx, ax
        mov    dx, si
        in     ax, dx
        mov    word ptr [edi + 33h], ax
        test   ax, ax
        je     SHORT loc_1_4A5B
        mov    word ptr [edi + 5bh], 4
loc_1_4A5B:
        mov    dx, word ptr [edi + 23dh]
        test   byte ptr [edi + 244h], 10h
        mov    ax, cx
        out    dx, ax
        jne    SHORT loc_1_4A7A
        mov    dx, word ptr [edi + 241h]
        mov    al, 0ffh
        out    dx, al
loc_1_4A7A:
        mov    al, byte ptr [edi + 5bh]
        pop    edi
        and    al, 4
        pop    esi
        shr    al, 2
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_4A8C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        mov    word ptr [ebp - 2], 0
        push   ebx
        push   esi
        push   edi
        mov    bl, 1
        mov    esi, dword ptr [ebp + 0ch]
        mov    dx, word ptr [esi + 23dh]
        in     ax, dx
        mov    di, ax
loc_1_4AAC:
        cmp    byte ptr [esi + 59h], 1
        je     SHORT loc_1_4AE3
        mov    byte ptr [esi + 59h], 1
        push   esi
        call   sub_1_4BAC
        mov    byte ptr [esi + 59h], 0
        test   al, al
        jne    SHORT loc_1_4AC8
        test   bl, bl
        je     SHORT loc_1_4AE3
loc_1_4AC8:
        xor    bl, bl
        cmp    byte ptr [esi + 5ah], 1
        je     SHORT loc_1_4AE3
        mov    byte ptr [esi + 5ah], 1
        push   1
        push   esi
        call   sub_1_4F2C
        mov    byte ptr [esi + 5ah], bl
        test   al, al
        jne    SHORT loc_1_4AAC
loc_1_4AE3:
        cmp    byte ptr [esi + 31h], 1
        jne    SHORT loc_1_4B23
        lea    eax, [ebp - 2]
        lea    ecx, [ebp - 0ch]
        push   ecx
        mov    dword ptr [ebp - 0ch], esi
        db  068h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        mov    word ptr [ebp - 4], 4
        push   esi
        call   sub_1_58CA
        test   byte ptr [ebp - 2], 4
        je     SHORT loc_1_4B23
        push   esi
        inc    dword ptr [esi + 5dh]
        and    word ptr [ebp - 2], 0fffbh
        mov    byte ptr [esi + 31h], 0
        call   sub_1_4CDC
        dec    dword ptr [esi + 5dh]
loc_1_4B23:
        cmp    byte ptr [esi + 30h], 0
        jne    SHORT loc_1_4B75
        cmp    byte ptr [esi + 214h], 1
        jne    SHORT loc_1_4B75
        mov    bx, word ptr [esi + 23dh]
        mov    eax, 0a30h
        mov    dx, bx
        out    dx, ax
        mov    cx, word ptr [esi + 231h]
        mov    dx, cx
        in     ax, dx
        test   al, 1
        je     SHORT loc_1_4B75
        and    eax, 0fffeh
        out    dx, ax
        mov    dx, bx
        mov    eax, 0a3ah
        out    dx, ax
        mov    dx, cx
        in     ax, dx
        test   ax, ax
        je     SHORT loc_1_4B75
        push   eax
        push   esi
        call   sub_1_4E2C
loc_1_4B75:
        cmp    byte ptr [esi + 22bh], 0
        jne    SHORT loc_1_4B8E
        mov    eax, dword ptr [esi + 69h]
        sub    eax, esi
        cmp    eax, 69h
        je     SHORT loc_1_4B8E
        push   esi
        call   sub_1_502C
loc_1_4B8E:
        mov    dx, word ptr [esi + 23dh]
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, ax
        pop    ebp
        ret    10h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4BAC:
        push   ebp
        xor    cl, cl
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 22bh], 1
        jne    SHORT loc_1_4C29
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_4BC9:
        mov    eax, 0eh
loc_1_4BCE:
        cmp    eax, ecx
        jbe    SHORT loc_1_4BEB
        cmp    ecx, 6
        jb     SHORT loc_1_4C02
        add    eax, edx
        push   0
        push   0
        push   0
        push   ecx
        push   edx
        push   eax
        mov    ecx, dword ptr [esi + 85h]
        push   ecx
        jmp    SHORT loc_1_4BFD
loc_1_4BEB:
        sub    ecx, eax
        lea    ebx, [eax + edx]
        push   ecx
        push   ecx
        push   ebx
        push   eax
        push   edx
        mov    eax, dword ptr [esi + 85h]
        push   ebx
        push   eax
loc_1_4BFD:
        call   sub_1_5350
loc_1_4C02:
        mov    eax, dword ptr [esi + 85h]
        push   eax
        call   sub_1_55A0
        mov    dx, word ptr [esi + 23dh]
        mov    eax, edi
        out    dx, ax
        mov    dx, word ptr [esi + 231h]
        mov    eax, 200h
        out    dx, ax
        mov    cl, 1
loc_1_4C29:
        mov    bx, word ptr [esi + 212h]
        mov    ax, bx
        add    ax, 0ah
        mov    dx, word ptr [esi + 23dh]
        mov    edi, eax
        and    edi, 0ffffh
        mov    eax, edi
        out    dx, ax
        mov    dx, word ptr [esi + 231h]
        in     ax, dx
        cmp    ax, 8
        jne    SHORT loc_1_4CCB
        mov    dx, word ptr [esi + 23dh]
        mov    eax, ebx
        and    eax, 0ffffh
        inc    dword ptr [esi + 24fh]
        out    dx, ax
        mov    dx, word ptr [esi + 233h]
        in     ax, dx
        mov    word ptr [esi + 212h], ax
        in     ax, dx
        mov    bx, ax
        shr    bx, 8
        mov    dx, word ptr [esi + 231h]
        in     ax, dx
        mov    ecx, eax
        mov    ax, word ptr [esi + 247h]
        and    ecx, 0ffffh
        out    dx, ax
        mov    edx, ebx
        mov    eax, dword ptr [esi + 20eh]
        and    edx, 0ffffh
        mov    edx, dword ptr [eax + edx*4]
        test   byte ptr [edx + 8], 80h
        je     NEAR PTR loc_1_4BC9
        mov    al, byte ptr [edx + 0eh]
        and    eax, 1fh
        add    eax, 0eh
        jmp    NEAR PTR loc_1_4BCE
loc_1_4CCB:
        mov    al, cl
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_4CDC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    word ptr [esi + 33h], 0
        je     NEAR PTR loc_1_4DC5
        mov    edi, dword ptr [esi + 61h]
        mov    eax, esi
        mov    byte ptr [esi + 3dh], 1
        mov    byte ptr [esi + 30h], 1
        mov    dword ptr [esi + 39h], 5
        sub    eax, edi
        cmp    eax, -61h
        je     NEAR PTR loc_1_4E19
loc_1_4D12:
        cmp    byte ptr [edi + 1ch], 0
        mov    ebx, edi
        je     SHORT loc_1_4D45
        mov    byte ptr [edi + 1ch], 0
        mov    word ptr [esi + 33h], 0
        mov    edi, dword ptr [edi]
        mov    eax, dword ptr [ebx + 4]
        push   0
        push   96h
        push   ebx
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebx + 4]
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [ecx + 4], eax
        call   sub_1_58DC
        dec    dword ptr [esi + 5dh]
        jmp    SHORT loc_1_4DAD
loc_1_4D45:
        xor    eax, eax
        push   4
        mov    ax, word ptr [esi + 33h]
        or     eax, 0c0011000h
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 4]
        push   eax
        inc    dword ptr [ebx + 14h]
        mov    word ptr [esi + 33h], 0
        push   40010007h
        mov    ecx, dword ptr [ebx + 10h]
        push   ecx
        call   sub_1_583E
        mov    ecx, dword ptr [ebx + 10h]
        push   ecx
        call   sub_1_5844
        dec    dword ptr [ebx + 14h]
        cmp    byte ptr [esi + 22bh], 0
        je     SHORT loc_1_4DAD
        mov    eax, dword ptr [esi + 227h]
        push   0c0000001h
        mov    byte ptr [esi + 22bh], 0
        mov    byte ptr [esi + 3dh], 0
        dec    dword ptr [eax + 14h]
        mov    eax, dword ptr [esi + 227h]
        mov    ecx, dword ptr [eax + 10h]
        push   ecx
        call   sub_1_5826
loc_1_4DAD:
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -61h
        jne    NEAR PTR loc_1_4D12
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_4DC5:
        xor    eax, eax
        mov    byte ptr [esi + 3dh], al
        mov    byte ptr [esi + 30h], al
        cmp    byte ptr [esi + 22bh], al
        jne    SHORT loc_1_4E13
        mov    edi, dword ptr [esi + 61h]
        mov    eax, esi
        mov    dword ptr [esi + 39h], 1
        sub    eax, edi
        cmp    eax, -61h
        je     SHORT loc_1_4E19
loc_1_4DE8:
        cmp    byte ptr [edi + 1ch], 0
        je     SHORT loc_1_4DFF
        mov    byte ptr [edi + 1ch], 0
        push   0
        mov    eax, dword ptr [edi + 10h]
        push   0
        push   eax
        call   sub_1_580E
loc_1_4DFF:
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -61h
        jne    SHORT loc_1_4DE8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_4E13:
        push   esi
        call   sub_1_517C
loc_1_4E19:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4E2C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        xor    eax, eax
        push   ebx
        mov    cx, word ptr [ebp + 0ch]
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 0
        test   cl, 20h
        mov    dx, word ptr [esi + 231h]
        out    dx, ax
        je     SHORT loc_1_4E5A
        mov    dword ptr [ebp - 4], 40h
loc_1_4E5A:
        test   cl, 40h
        je     SHORT loc_1_4E66
        or     dword ptr [ebp - 4], 80h
loc_1_4E66:
        test   ch, 8
        je     SHORT loc_1_4E72
        or     dword ptr [ebp - 4], 800h
loc_1_4E72:
        test   cl, 80h
        je     SHORT loc_1_4E7E
        or     dword ptr [ebp - 4], 100h
loc_1_4E7E:
        test   ch, 1
        je     SHORT loc_1_4E8A
        or     dword ptr [ebp - 4], 200h
loc_1_4E8A:
        test   ch, 4
        je     SHORT loc_1_4E96
        or     dword ptr [ebp - 4], 400h
loc_1_4E96:
        test   ch, 10h
        je     SHORT loc_1_4EA2
        or     dword ptr [ebp - 4], 1000h
loc_1_4EA2:
        test   ch, 20h
        je     SHORT loc_1_4EAE
        or     dword ptr [ebp - 4], 2000h
loc_1_4EAE:
        test   ch, 40h
        je     SHORT loc_1_4EBA
        or     dword ptr [ebp - 4], 4000h
loc_1_4EBA:
        test   ch, 80h
        je     SHORT loc_1_4EC6
        or     dword ptr [ebp - 4], 8000h
loc_1_4EC6:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_4F0D
        mov    edi, dword ptr [esi + 61h]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -61h
        je     SHORT loc_1_4F07
loc_1_4ED8:
        lea    ebx, [edi + 14h]
        push   4
        lea    eax, [ebp - 4]
        inc    dword ptr [ebx]
        push   eax
        mov    ecx, dword ptr [edi + 10h]
        push   40010006h
        push   ecx
        call   sub_1_583E
        mov    ecx, dword ptr [edi + 10h]
        push   ecx
        call   sub_1_5844
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        dec    dword ptr [ebx]
        cmp    eax, -61h
        jne    SHORT loc_1_4ED8
loc_1_4F07:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 35h], eax
loc_1_4F0D:
        test   byte ptr [ebp - 3], 1
        je     SHORT loc_1_4F19
        push   esi
        call   sub_1_2F48
loc_1_4F19:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4F2C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    bl, bl
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 30h], 1
        jne    SHORT loc_1_4F44
        xor    al, al
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_4F44:
        mov    eax, dword ptr [esi + 1f3h]
        cmp    dword ptr [esi + 1efh], eax
        jne    SHORT loc_1_4F60
        cmp    word ptr [esi + 200h], 0
        jne    NEAR PTR loc_1_4FF5
loc_1_4F60:
        mov    ecx, dword ptr [eax + 10h]
        test   ecx, ecx
        je     NEAR PTR loc_1_4FF5
        mov    ax, word ptr [eax + 4]
        add    ax, 0ch
        mov    dx, word ptr [esi + 23dh]
        and    eax, 0ffffh
        out    dx, ax
        mov    dx, word ptr [esi + 231h]
        in     ax, dx
        cmp    ax, 8
        jne    SHORT loc_1_4FED
        xor    eax, eax
        out    dx, ax
        mov    edx, dword ptr [ecx + 28h]
        dec    dword ptr [edx + 14h]
        inc    word ptr [esi + 200h]
        mov    eax, dword ptr [esi + 1f3h]
        add    eax, 1ah
        inc    dword ptr [esi + 24bh]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_4FBF
        mov    byte ptr [eax], 0
        dec    word ptr [esi + 1ebh]
loc_1_4FBF:
        mov    eax, dword ptr [esi + 1f3h]
        push   0
        push   ecx
        mov    bl, 1
        mov    dword ptr [eax + 10h], 0
        mov    ecx, dword ptr [edx + 10h]
        push   ecx
        call   sub_1_581A
        mov    ecx, dword ptr [esi + 1f3h]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [esi + 1f3h], edx
        jmp    NEAR PTR loc_1_4F44
loc_1_4FED:
        mov    al, bl
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_4FF5:
        cmp    byte ptr [esi + 214h], 0
        je     SHORT loc_1_5016
        cmp    byte ptr [esi + 1ffh], 0
        je     SHORT loc_1_5016
        mov    byte ptr [esi + 1ffh], 0
        push   1
        push   esi
        call   sub_1_37CF
loc_1_5016:
        mov    al, bl
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_502C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 5dh]
        cmp    byte ptr [esi + 22bh], 0
        jne    NEAR PTR loc_1_5123
        mov    edi, dword ptr [esi + 71h]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 71h
        je     NEAR PTR loc_1_5123
        push   0
        inc    dword ptr [edi + 14h]
        mov    eax, dword ptr [edi + 0ch]
        mov    ecx, dword ptr [esi + 85h]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5069:
        test   eax, eax
        jne    SHORT loc_1_50D5
        cmp    dword ptr [edi + 14h], 2
        jne    SHORT loc_1_50AD
        mov    eax, dword ptr [edi]
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edi]
        mov    edx, dword ptr [edi + 4]
        mov    dword ptr [eax + 4], edx
        mov    ecx, dword ptr [edi + 18h]
        cmp    dword ptr [esi + 264h], ecx
        jne    SHORT loc_1_5093
        push   esi
        call   sub_1_0F8E
loc_1_5093:
        push   0
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_5814
        push   0
        push   96h
        push   edi
        call   sub_1_58DC
        jmp    SHORT loc_1_5123
loc_1_50AD:
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        lea    edx, [esi + 69h]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [edi], edx
        mov    eax, dword ptr [esi + 6dh]
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [esi + 6dh]
        mov    dword ptr [eax], edi
        mov    dword ptr [esi + 6dh], edi
        sub    dword ptr [edi + 14h], 2
        jmp    SHORT loc_1_5123
loc_1_50D5:
        cmp    eax, 103h
        jne    SHORT loc_1_5108
        mov    byte ptr [edi + 1dh], 1
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        lea    edx, [esi + 69h]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [edi], edx
        mov    eax, dword ptr [esi + 6dh]
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [esi + 6dh]
        mov    dword ptr [eax], edi
        mov    dword ptr [esi + 6dh], edi
        sub    dword ptr [edi + 14h], 2
        jmp    SHORT loc_1_5123
loc_1_5108:
        push   eax
        push   0ah
        push   3
        mov    eax, dword ptr [esi + 81h]
        push   3
        push   0aa000006h
        push   eax
        call   sub_1_59A8
        add    esp, 18h
loc_1_5123:
        mov    edi, dword ptr [esi + 69h]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 69h
        je     SHORT loc_1_516D
        cmp    dword ptr [edi + 14h], 0
        jne    SHORT loc_1_516D
        mov    eax, dword ptr [edi + 18h]
        cmp    dword ptr [esi + 264h], eax
        jne    SHORT loc_1_5146
        push   esi
        call   sub_1_0F8E
loc_1_5146:
        push   0
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_5814
        mov    eax, dword ptr [edi]
        mov    ecx, dword ptr [edi + 4]
        push   0
        push   96h
        push   edi
        mov    dword ptr [ecx], eax
        mov    edx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [eax + 4], edx
        call   sub_1_58DC
loc_1_516D:
        pop    edi
        dec    dword ptr [esi + 5dh]
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_517C:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    dword ptr [esi + 9dh], 0
        call   sub_1_30AF
        test   eax, eax
        jne    SHORT loc_1_51A7
        push   esi
        call   sub_1_31A1
        test   eax, eax
        jne    SHORT loc_1_51A7
        push   esi
        call   sub_1_51AC
loc_1_51A7:
        pop    esi
        pop    ebp
        ret    4
sub_1_51AC:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 61h]
        mov    byte ptr [esi + 22bh], al
        mov    byte ptr [esi + 3dh], al
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -61h
        je     SHORT loc_1_51F1
loc_1_51CB:
        push   0
        mov    eax, dword ptr [edi + 10h]
        push   0
        push   40010005h
        push   eax
        call   sub_1_583E
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_5844
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -61h
        jne    SHORT loc_1_51CB
loc_1_51F1:
        mov    eax, dword ptr [esi + 227h]
        push   0
        dec    dword ptr [eax + 14h]
        mov    eax, dword ptr [esi + 227h]
        mov    ecx, dword ptr [eax + 10h]
        push   ecx
        call   sub_1_5826
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 214h], 0
        je     SHORT loc_1_528B
        cmp    byte ptr [esi + 1ffh], 0
        je     SHORT loc_1_528B
        mov    ax, word ptr [esi + 200h]
        cmp    word ptr [esi + 0a7h], ax
        je     SHORT loc_1_5269
        cmp    byte ptr [esi + 5ah], 0
        jne    SHORT loc_1_5269
        mov    byte ptr [esi + 5ah], 1
        mov    byte ptr [esi + 1ffh], 0
        push   1
        push   esi
        call   sub_1_4F2C
        mov    byte ptr [esi + 1ffh], 1
        mov    byte ptr [esi + 5ah], 0
loc_1_5269:
        cmp    byte ptr [esi + 59h], 0
        jne    SHORT loc_1_528B
        mov    byte ptr [esi + 59h], 1
        mov    byte ptr [esi + 1ffh], 0
        push   esi
        call   sub_1_4BAC
        mov    byte ptr [esi + 1ffh], 1
        mov    byte ptr [esi + 59h], 0
loc_1_528B:
        push   1388h
        add    esi, 1fh
        push   esi
        call   sub_1_58B2
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_52AC:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi + 23bh]
        in     al, dx
        mov    bl, al
        or     al, 8
        out    dx, al
        mov    dx, word ptr [esi + 233h]
        in     ax, dx
        mov    cx, ax
        test   cx, cx
        je     SHORT loc_1_5336
        mov    dx, word ptr [esi + 235h]
        in     ax, dx
        mov    di, ax
        mov    dx, word ptr [esi + 237h]
        in     ax, dx
        mov    edx, eax
        mov    eax, edi
        and    edx, 0ffffh
        and    eax, 0ffffh
        shl    edx, 10h
        or     edx, eax
        mov    eax, ecx
        test   bl, 4
        je     SHORT loc_1_5320
        and    eax, 0ffffh
        xor    ecx, ecx
        mov    cx, word ptr [esi + 231h]
        push   eax
        push   edx
        push   ecx
        call   sub_1_57AD
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_5320:
        and    eax, 0ffffh
        xor    ecx, ecx
        mov    cx, word ptr [esi + 231h]
        push   eax
        push   edx
        push   ecx
        call   sub_1_5770
loc_1_5336:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h
sub_1_5350:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 20h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_5380
        pop    ebp
        ret    1ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_5380:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 10h]
        add    ecx, 2
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 8
        cmp    dword ptr [ebp + 14h], 0eh
        jb     NEAR PTR loc_1_54A0
        mov    al, byte ptr [eax]
        shr    al, 7
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ecx]
        and    al, 80h
        mov    byte ptr [ebp - 2], al
        or     al, al
        je     NEAR PTR loc_1_5444
        mov    al, byte ptr [ecx]
        cmp    al, 0ffh
        jne    SHORT loc_1_53C2
        cmp    byte ptr [ecx + 1], 0ffh
        je     SHORT loc_1_53CC
loc_1_53C2:
        cmp    al, 0c0h
        jne    SHORT loc_1_53E8
        cmp    byte ptr [ecx + 1], 0
        jne    SHORT loc_1_53E8
loc_1_53CC:
        cmp    byte ptr [ecx + 2], 0ffh
        jne    SHORT loc_1_53E8
        cmp    byte ptr [ecx + 3], 0ffh
        jne    SHORT loc_1_53E8
        cmp    byte ptr [ecx + 4], 0ffh
        jne    SHORT loc_1_53E8
        cmp    byte ptr [ecx + 5], 0ffh
        mov    byte ptr [ebp - 2], 1
        je     SHORT loc_1_53EC
loc_1_53E8:
        mov    byte ptr [ebp - 2], 0
loc_1_53EC:
        cmp    byte ptr [ebp - 2], 0
        je     SHORT loc_1_5400
        mov    dword ptr [ebp - 8], 8
        jmp    NEAR PTR loc_1_54AB
        db  08Bh, 0FFh
loc_1_5400:
        mov    al, byte ptr [ecx]
        and    al, byte ptr [ecx + 2]
        and    al, 80h
        mov    byte ptr [ebp - 2], al
        or     al, al
        mov    dword ptr [ebp - 8], 1000h
        jne    SHORT loc_1_541C
        mov    dword ptr [ebp - 8], 4000h
loc_1_541C:
        xor    edx, edx
        mov    dl, byte ptr [ecx + 4]
        xor    eax, eax
        mov    al, byte ptr [ecx + 2]
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [ecx + 3]
        shl    eax, 10h
        or     edx, eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 5]
        or     edx, eax
        mov    dword ptr [ebp - 0ch], edx
        jmp    SHORT loc_1_54AB
loc_1_5444:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 10h], 30h
        je     SHORT loc_1_5490
        mov    edx, dword ptr [esi + 1ah]
        lea    eax, [esi + 18h]
        cmp    dword ptr [ecx + 2], edx
        jne    SHORT loc_1_5467
        mov    ax, word ptr [eax]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_546E
loc_1_5467:
        mov    dword ptr [ebp - 10h], 1
loc_1_546E:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_5480
        mov    dword ptr [ebp - 8], 1
        jmp    SHORT loc_1_54AE
        db  08Dh, 049h, 000h
loc_1_5480:
        mov    dword ptr [ebp - 8], 0
        jmp    SHORT loc_1_54AE
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5490:
        mov    dword ptr [ebp - 8], 1
        jmp    SHORT loc_1_54AE
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_54A0:
        mov    dword ptr [ebp - 8], 20h
        mov    byte ptr [ebp - 1], 0
loc_1_54AB:
        mov    esi, dword ptr [ebp + 8]
loc_1_54AE:
        mov    edi, dword ptr [esi + 14h]
        or     edi, edi
        je     NEAR PTR loc_1_5591
loc_1_54B9:
        mov    ecx, dword ptr [edi + 8]
        mov    eax, ecx
        and    eax, dword ptr [ebp - 8]
        test   al, 9
        jne    SHORT loc_1_54FD
        test   ah, 40h
        je     SHORT loc_1_54D2
        mov    eax, dword ptr [edi + 0ch]
        test   dword ptr [ebp - 0ch], eax
        jne    SHORT loc_1_54FD
loc_1_54D2:
        test   byte ptr [ebp - 7], 40h
        je     SHORT loc_1_54DD
        test   ch, 20h
        jne    SHORT loc_1_54FD
loc_1_54DD:
        test   byte ptr [ebp - 7], 10h
        je     SHORT loc_1_54E9
        cmp    byte ptr [edi + 1dh], 0
        jne    SHORT loc_1_54FD
loc_1_54E9:
        test   cl, 10h
        je     SHORT loc_1_54F4
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_1_54FD
loc_1_54F4:
        test   cl, 20h
        je     NEAR PTR loc_1_5586
loc_1_54FD:
        inc    dword ptr [edi + 18h]
        mov    eax, dword ptr [ebp + 20h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_5832
        mov    byte ptr [edi + 1eh], 1
        mov    eax, dword ptr [edi + 18h]
        dec    eax
        mov    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_5586
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_553F
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_553F:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_5550
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_5556
        db  08Bh, 0FFh
loc_1_5550:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 14h], eax
loc_1_5556:
        cmp    byte ptr [edi + 1eh], 0
        je     SHORT loc_1_5565
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_5838
loc_1_5565:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 2ah]
        mov    eax, 1
        mov    cl, byte ptr [edi + 1ch]
        shl    eax, cl
        or     dword ptr [esi + 2eh], eax
        push   0
        push   1fh
        push   edi
        call   sub_1_58DC
        mov    edi, ebx
        jmp    SHORT loc_1_5589
loc_1_5586:
        mov    edi, dword ptr [edi + 10h]
loc_1_5589:
        or     edi, edi
        jne    NEAR PTR loc_1_54B9
loc_1_5591:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
_LTEXT ends
        end
