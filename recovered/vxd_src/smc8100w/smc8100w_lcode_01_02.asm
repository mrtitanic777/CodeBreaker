; smc8100w_lcode_01_02.asm -- SMC8100W object 1 segment _LTEXT  (part 2/3)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_4FE3:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        test   eax, eax
        je     NEAR PTR loc_1_5378
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 0ch], 1
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 10h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_502C
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 10h]
        and    eax, 0ffffh
        dec    eax
        or     al, 8
        out    96h, al
        jmp    NEAR PTR loc_1_503C
loc_1_502C:
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 10h]
        and    eax, 0ffffh
        or     al, 8
        out    96h, al
loc_1_503C:
        mov    edx, 101h
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, 100h
        in     al, dx
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        shl    eax, 8
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        or     eax, ecx
        mov    word ptr [ebp - 0ch], ax
        mov    ax, word ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx + 1ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        cmp    eax, 6ec6h
        je     NEAR PTR loc_1_508D
        xor    al, al
        out    96h, al
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_5E8C
loc_1_508D:
        mov    eax, dword ptr [ebp + 18h]
        mov    byte ptr [eax + 20h], 5
        mov    edx, 102h
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    cl, byte ptr [ebp - 4]
        and    ecx, 0f0h
        mov    byte ptr [ebp - 4], cl
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        shl    ecx, 8
        add    ecx, 800h
        mov    edx, dword ptr [ebp + 18h]
        mov    word ptr [edx + 0eh], cx
        mov    edx, 105h
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 0ch
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        jne    NEAR PTR loc_1_50EB
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 16h], 3
        jmp    NEAR PTR loc_1_512C
loc_1_50EB:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 4
        jne    NEAR PTR loc_1_5107
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 16h], 4
        jmp    NEAR PTR loc_1_512C
loc_1_5107:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 8
        jne    NEAR PTR loc_1_5123
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 16h], 0ah
        jmp    NEAR PTR loc_1_512C
loc_1_5123:
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 16h], 0fh
loc_1_512C:
        mov    edx, 103h
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 8
        je     NEAR PTR loc_1_5181
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 80h
        je     NEAR PTR loc_1_5168
        mov    al, byte ptr [ebp - 4]
        and    eax, 7
        shl    eax, 0dh
        add    eax, 0fd0000h
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx + 4], eax
        jmp    NEAR PTR loc_1_517C
loc_1_5168:
        mov    al, byte ptr [ebp - 4]
        and    eax, 7
        shl    eax, 0dh
        add    eax, 0d0000h
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx + 4], eax
loc_1_517C:
        jmp    NEAR PTR loc_1_51BB
loc_1_5181:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 80h
        je     NEAR PTR loc_1_51A7
        mov    al, byte ptr [ebp - 4]
        and    eax, 7
        shl    eax, 0dh
        add    eax, 0fc0000h
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx + 4], eax
        jmp    NEAR PTR loc_1_51BB
loc_1_51A7:
        mov    al, byte ptr [ebp - 4]
        and    eax, 7
        shl    eax, 0dh
        add    eax, 0c0000h
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx + 4], eax
loc_1_51BB:
        mov    al, byte ptr [ebp - 4]
        and    eax, 30h
        mov    byte ptr [ebp - 4], al
        shr    byte ptr [ebp - 4], 4
        mov    eax, 8
        mov    cl, byte ptr [ebp - 4]
        shl    eax, cl
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx + 14h], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        cmp    eax, 6ec6h
        jne    NEAR PTR loc_1_51F5
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 12h], 40h
loc_1_51F5:
        mov    edx, 104h
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 8
        je     NEAR PTR loc_1_5224
        mov    al, byte ptr [ebp - 4]
        and    eax, 7
        shl    eax, 0dh
        add    eax, 0d0000h
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx + 8], eax
        jmp    NEAR PTR loc_1_5238
loc_1_5224:
        mov    al, byte ptr [ebp - 4]
        and    eax, 7
        shl    eax, 0dh
        add    eax, 0c0000h
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx + 8], eax
loc_1_5238:
        shr    byte ptr [ebp - 4], 4
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        jne    NEAR PTR loc_1_5257
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 18h], 8
        jmp    NEAR PTR loc_1_5298
loc_1_5257:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 1
        jne    NEAR PTR loc_1_5273
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 18h], 10h
        jmp    NEAR PTR loc_1_5298
loc_1_5273:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 2
        jne    NEAR PTR loc_1_528F
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 18h], 20h
        jmp    NEAR PTR loc_1_5298
loc_1_528F:
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 18h], 0
loc_1_5298:
        mov    edx, 105h
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 3
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        jne    NEAR PTR loc_1_52C5
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 1eh], 5
        jmp    NEAR PTR loc_1_5306
loc_1_52C5:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 1
        jne    NEAR PTR loc_1_52E1
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 1eh], 4
        jmp    NEAR PTR loc_1_5306
loc_1_52E1:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 3
        jne    NEAR PTR loc_1_52FD
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 1eh], 6
        jmp    NEAR PTR loc_1_5306
loc_1_52FD:
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 1eh], 7
loc_1_5306:
        mov    edx, 102h
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 2
        jne    NEAR PTR loc_1_5332
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 1ah]
        and    eax, 0ffffh
        or     eax, 20h
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx + 1ah], ax
loc_1_5332:
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    al, byte ptr [ebp + 14h]
        push   eax
        mov    al, byte ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_44BC
        mov    eax, dword ptr [ebp + 18h]
        test   byte ptr [eax + 3], 4
        je     NEAR PTR loc_1_5368
        mov    eax, dword ptr [ebp + 18h]
        mov    byte ptr [eax + 21h], 3
loc_1_5368:
        xor    al, al
        out    96h, al
        xor    eax, eax
        jmp    NEAR PTR loc_1_5E8C
        db  0E9h, 014h, 00Bh, 000h, 000h
loc_1_5378:
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 0ch], 0
        mov    byte ptr [ebp - 8], 0
        mov    word ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_5394
loc_1_5390:
        inc    word ptr [ebp - 0ch]
loc_1_5394:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        cmp    eax, 8
        jge    NEAR PTR loc_1_53D6
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        lea    edx, [eax + ecx + 8]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        add    eax, ecx
        mov    byte ptr [ebp - 8], al
        jmp    NEAR PTR loc_1_5390
loc_1_53D6:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 0eeh
        je     NEAR PTR loc_1_53F0
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_5E8C
loc_1_53F0:
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    al, byte ptr [ebp + 14h]
        push   eax
        mov    al, byte ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_44BC
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax]
        and    eax, 70000h
        cmp    eax, 20000h
        jb     NEAR PTR loc_1_548A
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax]
        and    eax, 70000h
        cmp    eax, 50000h
        ja     NEAR PTR loc_1_548A
        mov    eax, 8
        mov    edx, dword ptr [ebp + 18h]
        mov    edx, dword ptr [edx]
        and    edx, 70000h
        shr    edx, 10h
        sub    edx, 2
        mov    cl, dl
        shl    eax, cl
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx + 12h], ax
        mov    eax, dword ptr [ebp + 18h]
        test   byte ptr [eax], 80h
        je     NEAR PTR loc_1_5477
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 14h], 10h
        jmp    NEAR PTR loc_1_5485
loc_1_5477:
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 12h]
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx + 14h], ax
loc_1_5485:
        jmp    NEAR PTR loc_1_5498
loc_1_548A:
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 12h]
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx + 14h], ax
loc_1_5498:
        mov    eax, dword ptr [ebp + 18h]
        test   byte ptr [eax + 3], 4
        je     NEAR PTR loc_1_54AC
        mov    eax, dword ptr [ebp + 18h]
        mov    byte ptr [eax + 21h], 3
loc_1_54AC:
        mov    eax, dword ptr [ebp + 18h]
        test   byte ptr [eax], 10h
        jne    NEAR PTR loc_1_5551
        mov    eax, dword ptr [ebp + 18h]
        mov    byte ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 1eh], 0ffffh
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        lea    edx, [eax + 8]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        je     NEAR PTR loc_1_54F2
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_5E8C
loc_1_54F2:
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        lea    edx, [eax + 9]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        je     NEAR PTR loc_1_551C
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_5E8C
loc_1_551C:
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        lea    edx, [eax + 0ah]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 3
        je     NEAR PTR loc_1_5547
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_5E8C
loc_1_5547:
        mov    eax, 1
        jmp    NEAR PTR loc_1_5E8C
loc_1_5551:
        mov    eax, dword ptr [ebp + 18h]
        test   dword ptr [eax], 3c00000h
        jne    NEAR PTR loc_1_556C
        mov    eax, dword ptr [ebp + 18h]
        mov    byte ptr [eax + 20h], 1
        jmp    NEAR PTR loc_1_5594
loc_1_556C:
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax]
        and    eax, 3c00000h
        cmp    eax, 1000000h
        jne    NEAR PTR loc_1_558D
        mov    eax, dword ptr [ebp + 18h]
        mov    byte ptr [eax + 20h], 3
        jmp    NEAR PTR loc_1_5594
loc_1_558D:
        mov    eax, dword ptr [ebp + 18h]
        mov    byte ptr [eax + 20h], 2
loc_1_5594:
        mov    byte ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax]
        and    eax, 3c00000h
        cmp    eax, 1000000h
        jne    NEAR PTR loc_1_567F
        mov    byte ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        lea    edx, [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 10h], al
        mov    al, byte ptr [ebp - 10h]
        and    eax, 0c3h
        mov    byte ptr [ebp - 10h], al
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 10h]
        or     cl, 80h
        lea    edx, [eax + 4]
        mov    al, cl
        out    dx, al
        mov    ecx, dword ptr [ebp + 18h]
        mov    cx, word ptr [ecx + 0eh]
        and    ecx, 0ffffh
        lea    edx, [ecx + 0dh]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 0ch
        mov    byte ptr [ebp - 4], al
        shr    byte ptr [ebp - 4], 2
        mov    al, byte ptr [ebp - 8]
        and    eax, 40h
        mov    byte ptr [ebp - 8], al
        shr    byte ptr [ebp - 8], 4
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        or     al, byte ptr [ebp - 4]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        lea    edx, [eax + 4]
        mov    al, byte ptr [ebp - 10h]
        out    dx, al
        mov    eax, dword ptr [ebp + 18h]
        test   byte ptr [eax], 40h
        je     NEAR PTR loc_1_5663
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx + 16h], ax
        jmp    NEAR PTR loc_1_567A
loc_1_5663:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx + 16h], ax
loc_1_567A:
        jmp    NEAR PTR loc_1_57C9
loc_1_567F:
        mov    eax, dword ptr [ebp + 18h]
        test   dword ptr [eax], 3c00000h
        je     NEAR PTR loc_1_56AA
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        lea    edx, [eax + 1]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 4
        mov    byte ptr [ebp - 4], al
loc_1_56AA:
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        lea    edx, [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 60h
        mov    byte ptr [ebp - 8], al
        shr    byte ptr [ebp - 8], 5
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   eax, eax
        jne    NEAR PTR loc_1_5700
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        jne    NEAR PTR loc_1_56F2
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 16h], 2
        jmp    NEAR PTR loc_1_56FB
loc_1_56F2:
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 16h], 0ah
loc_1_56FB:
        jmp    NEAR PTR loc_1_57C9
loc_1_5700:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 1
        jne    NEAR PTR loc_1_5737
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        jne    NEAR PTR loc_1_5729
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 16h], 3
        jmp    NEAR PTR loc_1_5732
loc_1_5729:
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 16h], 0bh
loc_1_5732:
        jmp    NEAR PTR loc_1_57C9
loc_1_5737:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 2
        jne    NEAR PTR loc_1_5789
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        jne    NEAR PTR loc_1_577B
        mov    eax, dword ptr [ebp + 18h]
        test   byte ptr [eax + 2], 20h
        je     NEAR PTR loc_1_576D
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 16h], 5
        jmp    NEAR PTR loc_1_5776
loc_1_576D:
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 16h], 4
loc_1_5776:
        jmp    NEAR PTR loc_1_5784
loc_1_577B:
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 16h], 0fh
loc_1_5784:
        jmp    NEAR PTR loc_1_57C9
loc_1_5789:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 3
        jne    NEAR PTR loc_1_57C0
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        jne    NEAR PTR loc_1_57B2
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 16h], 7
        jmp    NEAR PTR loc_1_57BB
loc_1_57B2:
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 16h], 4
loc_1_57BB:
        jmp    NEAR PTR loc_1_57C9
loc_1_57C0:
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 16h], 3
loc_1_57C9:
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 1ah]
        and    eax, 7fffh
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx + 1ah], ax
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax]
        and    eax, 3c00000h
        cmp    eax, 1000000h
        jne    NEAR PTR loc_1_5822
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 16h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_581D
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 1ah]
        and    eax, 0ffffh
        or     eax, 8000h
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx + 1ah], ax
loc_1_581D:
        jmp    NEAR PTR loc_1_585A
loc_1_5822:
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        lea    edx, [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 80h
        je     NEAR PTR loc_1_585A
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 1ah]
        and    eax, 0ffffh
        or     eax, 8000h
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx + 1ah], ax
loc_1_585A:
        mov    eax, dword ptr [ebp + 18h]
        mov    dx, word ptr [eax + 0eh]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 3fh
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 18h]
        test   dword ptr [eax], 3c00000h
        jne    NEAR PTR loc_1_589A
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     al, 40h
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        shl    eax, 0dh
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx + 4], eax
        jmp    NEAR PTR loc_1_59DB
loc_1_589A:
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax]
        and    eax, 3c00000h
        cmp    eax, 1000000h
        jne    NEAR PTR loc_1_5993
        mov    byte ptr [ebp - 14h], 0
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        lea    edx, [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 14h], al
        mov    al, byte ptr [ebp - 14h]
        and    eax, 0c3h
        mov    byte ptr [ebp - 14h], al
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 14h]
        or     cl, 80h
        lea    edx, [eax + 4]
        mov    al, cl
        out    dx, al
        mov    ecx, dword ptr [ebp + 18h]
        mov    cx, word ptr [ecx + 0eh]
        and    ecx, 0ffffh
        lea    edx, [ecx + 0bh]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    cl, byte ptr [ebp - 4]
        and    ecx, 30h
        mov    byte ptr [ebp - 4], cl
        shr    byte ptr [ebp - 4], 4
        mov    word ptr [ebp - 0ch], 8
        mov    cl, byte ptr [ebp - 4]
        shl    word ptr [ebp - 0ch], cl
        mov    cx, word ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp + 18h]
        mov    word ptr [edx + 14h], cx
        mov    ecx, dword ptr [ebp + 18h]
        mov    cx, word ptr [ecx + 0eh]
        and    ecx, 0ffffh
        lea    edx, [ecx + 0bh]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 0fh
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 40h
        shr    eax, 2
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        or     eax, ecx
        or     al, 60h
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        shl    eax, 0dh
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx + 4], eax
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 80h
        je     NEAR PTR loc_1_597B
        mov    eax, dword ptr [ebp + 18h]
        or     dword ptr [eax + 4], 0f00000h
loc_1_597B:
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        lea    edx, [eax + 4]
        mov    al, byte ptr [ebp - 14h]
        out    dx, al
        jmp    NEAR PTR loc_1_59DB
loc_1_5993:
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        lea    edx, [eax + 5]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 1fh
        mov    byte ptr [ebp - 8], al
        shl    byte ptr [ebp - 8], 3
        mov    al, byte ptr [ebp - 4]
        and    eax, 38h
        shr    eax, 3
        or     al, byte ptr [ebp - 8]
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 7
        shl    eax, 0dh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        shl    ecx, 10h
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx + 4], eax
loc_1_59DB:
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax]
        and    eax, 3c00000h
        cmp    eax, 1000000h
        jne    NEAR PTR loc_1_5A76
        mov    byte ptr [ebp - 18h], 0
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        lea    edx, [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 18h], al
        mov    al, byte ptr [ebp - 18h]
        and    eax, 0c3h
        mov    byte ptr [ebp - 18h], al
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 18h]
        or     cl, 80h
        lea    edx, [eax + 4]
        mov    al, cl
        out    dx, al
        mov    ecx, dword ptr [ebp + 18h]
        mov    cx, word ptr [ecx + 0eh]
        and    ecx, 0ffffh
        lea    edx, [ecx + 0ch]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 0fh
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 40h
        shr    eax, 2
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        or     eax, ecx
        or     al, 60h
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        lea    edx, [eax + 4]
        mov    al, byte ptr [ebp - 18h]
        out    dx, al
        jmp    NEAR PTR loc_1_5A9C
loc_1_5A76:
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        lea    edx, [eax + 3]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 3eh
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     al, 40h
        mov    byte ptr [ebp - 4], al
loc_1_5A9C:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        shl    eax, 0dh
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax]
        and    eax, 3c00000h
        cmp    eax, 1000000h
        jne    NEAR PTR loc_1_5B61
        mov    byte ptr [ebp - 1ch], 0
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        lea    edx, [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 1ch], al
        mov    al, byte ptr [ebp - 1ch]
        and    eax, 0c3h
        mov    byte ptr [ebp - 1ch], al
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 1ch]
        or     cl, 80h
        lea    edx, [eax + 4]
        mov    al, cl
        out    dx, al
        mov    ecx, dword ptr [ebp + 18h]
        mov    cx, word ptr [ecx + 0eh]
        and    ecx, 0ffffh
        lea    edx, [ecx + 0ch]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 30h
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 30h
        jne    NEAR PTR loc_1_5B34
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 18h], 0
        jmp    NEAR PTR loc_1_5B49
loc_1_5B34:
        shr    byte ptr [ebp - 4], 4
        mov    eax, 8
        mov    cl, byte ptr [ebp - 4]
        shl    eax, cl
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx + 18h], ax
loc_1_5B49:
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        lea    edx, [eax + 4]
        mov    al, byte ptr [ebp - 1ch]
        out    dx, al
        jmp    NEAR PTR loc_1_5BAF
loc_1_5B61:
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        lea    edx, [eax + 3]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 0c0h
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        jne    NEAR PTR loc_1_5B9A
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [eax + 18h], 0
        jmp    NEAR PTR loc_1_5BAF
loc_1_5B9A:
        shr    byte ptr [ebp - 4], 6
        mov    eax, 8
        mov    cl, byte ptr [ebp - 4]
        shl    eax, cl
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx + 18h], ax
loc_1_5BAF:
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        lea    edx, [eax + 7]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 1ah]
        and    eax, 9fffh
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx + 1ah], ax
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 0fh
        je     NEAR PTR loc_1_5BFA
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 1ah]
        and    eax, 0ffffh
        or     eax, 2000h
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx + 1ah], ax
loc_1_5BFA:
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 1ah]
        and    eax, 0e7ffh
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx + 1ah], ax
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax]
        and    eax, 3c00000h
        cmp    eax, 1000000h
        jne    NEAR PTR loc_1_5CAF
        mov    byte ptr [ebp - 20h], 0
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        lea    edx, [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 20h], al
        mov    al, byte ptr [ebp - 20h]
        and    eax, 0c3h
        mov    byte ptr [ebp - 20h], al
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 20h]
        or     cl, 80h
        lea    edx, [eax + 4]
        mov    al, cl
        out    dx, al
        mov    ecx, dword ptr [ebp + 18h]
        mov    cx, word ptr [ecx + 0eh]
        and    ecx, 0ffffh
        lea    edx, [ecx + 0dh]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 20h
        je     NEAR PTR loc_1_5C97
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 1ah]
        and    eax, 0ffffh
        or     eax, 1800h
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx + 1ah], ax
loc_1_5C97:
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        lea    edx, [eax + 4]
        mov    al, byte ptr [ebp - 20h]
        out    dx, al
        jmp    NEAR PTR loc_1_5D2B
loc_1_5CAF:
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        lea    edx, [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 1
        je     NEAR PTR loc_1_5CE7
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 1ah]
        and    eax, 0ffffh
        or     eax, 1000h
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx + 1ah], ax
loc_1_5CE7:
        mov    eax, dword ptr [ebp + 18h]
        test   byte ptr [eax], 40h
        je     NEAR PTR loc_1_5D2B
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        lea    edx, [eax + 5]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 20h
        je     NEAR PTR loc_1_5D2B
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 1ah]
        and    eax, 0ffffh
        or     eax, 800h
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx + 1ah], ax
loc_1_5D2B:
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        lea    edx, [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 1ah]
        and    eax, 0f8ffh
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx + 1ah], ax
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax]
        and    eax, 3c00000h
        cmp    eax, 400000h
        je     NEAR PTR loc_1_5D83
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 1eh]
        and    eax, 0ffffh
        or     eax, 0ffffh
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx + 1eh], ax
        jmp    NEAR PTR loc_1_5E85
loc_1_5D83:
        mov    al, byte ptr [ebp - 4]
        and    eax, 6
        mov    ecx, 6
        and    ecx, 0fffffffeh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_5DB4
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 1eh]
        and    eax, 0ffffh
        or     eax, 6
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx + 1eh], ax
        jmp    NEAR PTR loc_1_5E2C
loc_1_5DB4:
        mov    al, byte ptr [ebp - 4]
        and    eax, 6
        mov    ecx, 4
        and    ecx, 0fffffffeh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_5DE5
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 1eh]
        and    eax, 0ffffh
        or     eax, 4
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx + 1eh], ax
        jmp    NEAR PTR loc_1_5E2C
loc_1_5DE5:
        mov    al, byte ptr [ebp - 4]
        and    eax, 6
        mov    ecx, 2
        and    ecx, 0fffffffeh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_5E16
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 1eh]
        and    eax, 0ffffh
        or     eax, 7
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx + 1eh], ax
        jmp    NEAR PTR loc_1_5E2C
loc_1_5E16:
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 1eh]
        and    eax, 0ffffh
        or     eax, 5
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx + 1eh], ax
loc_1_5E2C:
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        lea    edx, [eax + 7]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 2
        jne    NEAR PTR loc_1_5E62
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 1ah]
        and    eax, 0ffffh
        or     eax, 20h
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ecx + 1ah], ax
loc_1_5E62:
        mov    eax, dword ptr [ebp + 18h]
        mov    ax, word ptr [eax + 0eh]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6647
        test   eax, eax
        je     NEAR PTR loc_1_5E85
        mov    eax, 8ah
        jmp    NEAR PTR loc_1_5E8C
loc_1_5E85:
        xor    eax, eax
        jmp    NEAR PTR loc_1_5E8C
loc_1_5E8C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
sub_1_5E93:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        call   sub_1_D760
        cmp    dword ptr [ebp - 1ch], 0
        je     NEAR PTR loc_1_5ECC
        mov    ecx, dword ptr [ebp + 10h]
        mov    word ptr [ecx], 0
        mov    ax, 0ffffh
        jmp    NEAR PTR loc_1_5FE1
loc_1_5ECC:
        mov    al, byte ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 1], al
        mov    al, byte ptr [ebp - 5]
        mov    byte ptr [ebp - 0ch], al
        mov    al, byte ptr [ebp - 6]
        mov    byte ptr [ebp - 10h], al
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [ebp - 14h], ax
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_1_5F5C
loc_1_5EF9:
        mov    al, byte ptr [ebp - 6]
        mov    byte ptr [ebp - 0ch], al
        mov    al, byte ptr [ebp - 0ch]
        and    eax, 0feh
        mov    byte ptr [ebp - 0ch], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        shl    eax, 4
        mov    ecx, dword ptr [ebp + 10h]
        mov    word ptr [ecx], ax
        jmp    NEAR PTR loc_1_5FD9
loc_1_5F1D:
        mov    al, byte ptr [ebp - 6]
        mov    byte ptr [ebp - 0ch], al
        mov    al, byte ptr [ebp - 0ch]
        and    eax, 0f0h
        mov    byte ptr [ebp - 0ch], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        shl    eax, 8
        or     eax, 800h
        mov    ecx, dword ptr [ebp + 10h]
        mov    word ptr [ecx], ax
        jmp    NEAR PTR loc_1_5FD9
loc_1_5F46:
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [eax], 0
        mov    ax, 0ffffh
        jmp    NEAR PTR loc_1_5FE1
        db  0E9h, 07Dh, 000h, 000h, 000h
loc_1_5F5C:
        cmp    dword ptr [ebp - 20h], 6ec6h
        jg     NEAR PTR loc_1_5F8E
        je     NEAR PTR loc_1_5F1D
        cmp    dword ptr [ebp - 20h], 61c8h
        jl     NEAR PTR loc_1_5F46
        cmp    dword ptr [ebp - 20h], 61c9h
        jle    NEAR PTR loc_1_5F1D
        jmp    NEAR PTR loc_1_5F46
loc_1_5F8E:
        cmp    dword ptr [ebp - 20h], 0efd5h
        jg     NEAR PTR loc_1_5FC7
        cmp    dword ptr [ebp - 20h], 0efd4h
        jge    NEAR PTR loc_1_5F1D
        cmp    dword ptr [ebp - 20h], 6fc0h
        jl     NEAR PTR loc_1_5F46
        cmp    dword ptr [ebp - 20h], 6fc2h
        jle    NEAR PTR loc_1_5EF9
        jmp    NEAR PTR loc_1_5F46
loc_1_5FC7:
        cmp    dword ptr [ebp - 20h], 0efe5h
        je     NEAR PTR loc_1_5EF9
        jmp    NEAR PTR loc_1_5F46
loc_1_5FD9:
        xor    ax, ax
        jmp    NEAR PTR loc_1_5FE1
loc_1_5FE1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_5FE8:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 18h], 0
        mov    word ptr [ebp - 14h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0abh]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 95h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_602A
        mov    eax, 13h
        jmp    NEAR PTR loc_1_62E2
loc_1_602A:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_604B
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 1ffdh]
        and    eax, 0ffh
        cmp    eax, 54h
        jge    NEAR PTR loc_1_6055
loc_1_604B:
        mov    eax, 12h
        jmp    NEAR PTR loc_1_62E2
loc_1_6055:
        mov    word ptr [ebp - 18h], 0
        mov    word ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_6075
loc_1_6066:
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        add    eax, 2
        mov    word ptr [ebp - 10h], ax
loc_1_6075:
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        cmp    eax, 2000h
        jge    NEAR PTR loc_1_60B0
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + ecx]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 18h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 18h], ax
        jmp    NEAR PTR loc_1_6066
loc_1_60B0:
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_60CA
        mov    eax, 12h
        jmp    NEAR PTR loc_1_62E2
loc_1_60CA:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6A9B
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_92CB
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_67FC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_984F
        and    eax, 0ffffh
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_6136
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 1ffdh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 95h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_62C6
loc_1_6136:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 46h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 1ch], eax
        jmp    NEAR PTR loc_1_61BE
loc_1_614A:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 198h]
        and    eax, 0fdh
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 198h], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        lea    edx, [eax + 10h]
        mov    al, byte ptr [ecx + 198h]
        out    dx, al
        jmp    NEAR PTR loc_1_61E1
loc_1_617F:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 198h]
        and    eax, 0fdh
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 198h], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        lea    edx, [eax + 4]
        mov    al, byte ptr [ecx + 198h]
        out    dx, al
        jmp    NEAR PTR loc_1_61E1
        db  0E9h, 028h, 000h, 000h, 000h, 0E9h, 023h, 000h, 000h, 000h
loc_1_61BE:
        cmp    dword ptr [ebp - 1ch], 2
        je     NEAR PTR loc_1_614A
        cmp    dword ptr [ebp - 1ch], 3
        je     NEAR PTR loc_1_617F
        cmp    dword ptr [ebp - 1ch], 5
        je     NEAR PTR loc_1_614A
        jmp    NEAR PTR loc_1_61E1
loc_1_61E1:
        push   2000h
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
        push   eax
        call   sub_1_D8CC
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 46h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_1_6282
loc_1_620A:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 198h]
        and    eax, 0ffh
        or     al, 2
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 198h], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        lea    edx, [eax + 10h]
        mov    al, byte ptr [ecx + 198h]
        out    dx, al
        jmp    NEAR PTR loc_1_62A5
loc_1_6241:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 198h]
        and    eax, 0ffh
        or     al, 2
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 198h], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        lea    edx, [eax + 4]
        mov    al, byte ptr [ecx + 198h]
        out    dx, al
        jmp    NEAR PTR loc_1_62A5
        db  0E9h, 028h, 000h, 000h, 000h, 0E9h, 023h, 000h, 000h, 000h
loc_1_6282:
        cmp    dword ptr [ebp - 20h], 2
        je     NEAR PTR loc_1_620A
        cmp    dword ptr [ebp - 20h], 3
        je     NEAR PTR loc_1_6241
        cmp    dword ptr [ebp - 20h], 5
        je     NEAR PTR loc_1_620A
        jmp    NEAR PTR loc_1_62A5
loc_1_62A5:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_984F
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_62C1
        mov    word ptr [ebp - 14h], 8
loc_1_62C1:
        jmp    NEAR PTR loc_1_62CC
loc_1_62C6:
        mov    word ptr [ebp - 14h], 13h
loc_1_62CC:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_62E2
loc_1_62E2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_62E9:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0eh], 0
        mov    byte ptr [ebp - 8], 0
        mov    byte ptr [ebp - 7], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        and    eax, 0ffff00ffh
        mov    word ptr [ebp - 1ch], ax
        mov    dword ptr [ebp - 24h], 0
        mov    word ptr [ebp - 12h], 3
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 1ah], ax
        mov    word ptr [ebp - 16h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffff00ffh
        mov    word ptr [ebp - 18h], ax
        mov    word ptr [ebp - 0ah], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2A60
        lea    eax, [ebp - 28h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 50h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        dec    eax
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4ch]
        push   eax
        call   sub_1_4FE3
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 28h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1fch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 6], 0
        mov    ax, word ptr [ebp - 1ah]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 4], ax
        mov    ax, word ptr [ebp - 12h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0ah], ax
        mov    eax, dword ptr [ebp - 24h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 18h], eax
        mov    ax, word ptr [ebp - 16h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 16h], ax
        mov    ax, word ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 20h], ax
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0eh], eax
        mov    ax, word ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0ch], ax
        mov    ax, word ptr [ebp - 0ah]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 44h], ax
        mov    al, byte ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 1], al
        mov    ax, word ptr [ebp - 0eh]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 200h], ax
        mov    al, byte ptr [ebp - 8]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 46h], ax
        mov    al, byte ptr [ebp - 7]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 48h], ax
        mov    ax, word ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_6450
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0afh], 3
        jmp    NEAR PTR loc_1_6479
loc_1_6450:
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 1feh], 8
        je     NEAR PTR loc_1_646F
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0afh], 1
        jmp    NEAR PTR loc_1_6479
loc_1_646F:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0afh], 2
loc_1_6479:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2AB0
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_649A
        mov    ax, 1
        jmp    NEAR PTR loc_1_64B6
        db  0E9h, 013h, 000h, 000h, 000h
loc_1_649A:
        cmp    dword ptr [ebp - 4], 1
        jne    NEAR PTR loc_1_64AD
        mov    ax, 2
        jmp    NEAR PTR loc_1_64B6
loc_1_64AD:
        mov    ax, 0ffffh
        jmp    NEAR PTR loc_1_64B6
loc_1_64B6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_64BD:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 208h]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_1_64E2
        xor    ax, ax
        jmp    NEAR PTR loc_1_6506
loc_1_64E2:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 208h]
        and    eax, 0ffh
        cmp    eax, 4
        jne    NEAR PTR loc_1_64FE
        jmp    NEAR PTR loc_1_64FE
loc_1_64FE:
        xor    ax, ax
        jmp    NEAR PTR loc_1_6506
loc_1_6506:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 50h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_652E
        xor    ax, ax
        jmp    NEAR PTR loc_1_652E
loc_1_652E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 50h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_6556
        xor    ax, ax
        jmp    NEAR PTR loc_1_6575
loc_1_6556:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 20ah]
        and    eax, 0cah
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 20ah], al
        xor    ax, ax
        jmp    NEAR PTR loc_1_6575
loc_1_6575:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 50h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_659D
        xor    ax, ax
        jmp    NEAR PTR loc_1_659D
loc_1_659D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 50h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_65C5
        xor    ax, ax
        jmp    NEAR PTR loc_1_65E6
loc_1_65C5:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 20ah]
        and    eax, 0ffh
        or     al, 35h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 20ah], al
        xor    ax, ax
        jmp    NEAR PTR loc_1_65E6
loc_1_65E6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 4], 1
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 50h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_6615
        xor    ax, ax
        jmp    NEAR PTR loc_1_6615
loc_1_6615:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 50h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_6640
        xor    ax, ax
        jmp    NEAR PTR loc_1_6640
loc_1_6640:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_6647:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0ch], 0
        mov    byte ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_6662
loc_1_665F:
        inc    byte ptr [ebp - 8]
loc_1_6662:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 10h
        jge    NEAR PTR loc_1_678E
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        lea    edx, [eax + 1]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 4
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     al, 2
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        lea    edx, [eax + 1]
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        lea    edx, [ecx + 3]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 0fh
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        shl    eax, 4
        or     al, byte ptr [ebp - 4]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        lea    edx, [eax + 3]
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        lea    edx, [ecx + 1]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 4
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     al, 12h
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        lea    edx, [eax + 1]
        mov    al, byte ptr [ebp - 4]
        out    dx, al
loc_1_6709:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        lea    edx, [eax + 1]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 10h
        jne    NEAR PTR loc_1_6709
        mov    byte ptr [ebp - 10h], 0
        mov    word ptr [ebp - 14h], 8
        jmp    NEAR PTR loc_1_6738
loc_1_6734:
        inc    word ptr [ebp - 14h]
loc_1_6738:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 10h
        jge    NEAR PTR loc_1_6775
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        lea    edx, [eax + ecx]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        add    eax, ecx
        mov    byte ptr [ebp - 10h], al
        jmp    NEAR PTR loc_1_6734
loc_1_6775:
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 0ch], ax
        jmp    NEAR PTR loc_1_665F
loc_1_678E:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        lea    edx, [eax + 1]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 4
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     al, 10h
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        lea    edx, [eax + 1]
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffh
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        cmp    eax, 0ffh
        jne    NEAR PTR loc_1_67EA
        xor    eax, eax
        jmp    NEAR PTR loc_1_67F4
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_1_67EA:
        mov    eax, 8ah
        jmp    NEAR PTR loc_1_67F4
loc_1_67F4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
        db  000h
sub_1_67FC:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    byte ptr [eax + 0afh], 1
        jne    SHORT loc_1_6819
        xor    edx, edx
        mov    dx, word ptr [eax + 4]
        add    edx, 5
        in     al, dx
        or     al, 80h
        out    dx, al
loc_1_6819:
        xor    ax, ax
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_682C:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    byte ptr [eax + 0afh], 1
        jne    SHORT loc_1_6849
        xor    edx, edx
        mov    dx, word ptr [eax + 4]
        add    edx, 5
        in     al, dx
        and    al, 7fh
        out    dx, al
loc_1_6849:
        xor    ax, ax
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_685C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0fffffh
loc_1_686C:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_68A0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d2h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   ah, 80h
        je     NEAR PTR loc_1_6898
        xor    ax, ax
        jmp    NEAR PTR loc_1_68F6
loc_1_6898:
        dec    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_686C
loc_1_68A0:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_68AF
loc_1_68AC:
        inc    dword ptr [ebp - 4]
loc_1_68AF:
        cmp    dword ptr [ebp - 4], 3e8h
        jae    NEAR PTR loc_1_68ED
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d2h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        test   ah, 80h
        je     NEAR PTR loc_1_68DE
        xor    ax, ax
        jmp    NEAR PTR loc_1_68F6
loc_1_68DE:
        push   3e8h
        call   sub_1_D7E0
        jmp    NEAR PTR loc_1_68AC
loc_1_68ED:
        mov    ax, 8
        jmp    NEAR PTR loc_1_68F6
loc_1_68F6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_68FD:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0fffffh
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 46h]
        and    eax, 0ffffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_69B2
loc_1_6922:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        lea    edx, [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        dec    dword ptr [ebp - 4]
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 2
        je     NEAR PTR loc_1_694F
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_6922
loc_1_694F:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 2
        je     NEAR PTR loc_1_69AD
        mov    dword ptr [ebp - 4], 3e8h
        jmp    NEAR PTR loc_1_696B
loc_1_6968:
        dec    dword ptr [ebp - 4]
loc_1_696B:
        cmp    dword ptr [ebp - 4], 0
        jbe    NEAR PTR loc_1_69AD
        push   3e8h
        call   sub_1_D7E0
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 4]
        and    ecx, 0ffffh
        lea    edx, [ecx + 4]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 2
        jne    NEAR PTR loc_1_69A8
        xor    ax, ax
        jmp    NEAR PTR loc_1_6A5D
loc_1_69A8:
        jmp    NEAR PTR loc_1_6968
loc_1_69AD:
        jmp    NEAR PTR loc_1_6A3D
loc_1_69B2:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        lea    edx, [eax + 10h]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        dec    dword ptr [ebp - 4]
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 1
        je     NEAR PTR loc_1_69DF
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_69B2
loc_1_69DF:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 1
        je     NEAR PTR loc_1_6A3D
        mov    dword ptr [ebp - 4], 3e8h
        jmp    NEAR PTR loc_1_69FB
loc_1_69F8:
        dec    dword ptr [ebp - 4]
loc_1_69FB:
        cmp    dword ptr [ebp - 4], 0
        jbe    NEAR PTR loc_1_6A3D
        push   3e8h
        call   sub_1_D7E0
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 4]
        and    ecx, 0ffffh
        lea    edx, [ecx + 10h]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 1
        jne    NEAR PTR loc_1_6A38
        xor    ax, ax
        jmp    NEAR PTR loc_1_6A5D
loc_1_6A38:
        jmp    NEAR PTR loc_1_69F8
loc_1_6A3D:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_6A54
        xor    ax, ax
        jmp    NEAR PTR loc_1_6A5D
        db  0E9h, 009h, 000h, 000h, 000h
loc_1_6A54:
        mov    ax, 8
        jmp    NEAR PTR loc_1_6A5D
loc_1_6A5D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_6A7C
loc_1_6A79:
        inc    dword ptr [ebp - 4]
loc_1_6A7C:
        cmp    dword ptr [ebp - 4], 0f000h
        jae    NEAR PTR loc_1_6A8E
        jmp    NEAR PTR loc_1_6A79
loc_1_6A8E:
        xor    ax, ax
        jmp    NEAR PTR loc_1_6A96
loc_1_6A96:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_6A9B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        or     cl, 80h
        mov    dx, word ptr [eax + 4]
        mov    al, cl
        out    dx, al
        xor    ax, ax
        jmp    NEAR PTR loc_1_6AC9
loc_1_6AC9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_6AD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 8]
        mov    cl, byte ptr [ebp - 4]
        and    ecx, 7fh
        mov    dx, word ptr [eax + 4]
        mov    al, cl
        out    dx, al
        xor    ax, ax
        jmp    NEAR PTR loc_1_6AFC
loc_1_6AFC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_6B03:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 8]
        mov    cl, byte ptr [ebp - 4]
        and    ecx, 0bfh
        mov    dx, word ptr [eax + 4]
        mov    al, cl
        out    dx, al
        xor    ax, ax
        jmp    NEAR PTR loc_1_6B32
loc_1_6B32:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_6B39:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_6B68
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_6BCC
loc_1_6B68:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_6B8E
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_6BCC
loc_1_6B8E:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d2h]
        mov    word ptr [eax], 0
        mov    ax, word ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0d2h]
        mov    word ptr [ecx + 8], ax
        mov    ax, word ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0d2h]
        mov    word ptr [ecx + 0ah], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9093
        jmp    NEAR PTR loc_1_6BCC
loc_1_6BCC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_6BD3:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 46h]
        and    eax, 0ffffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_6C17
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ecx + 198h]
        and    ecx, 0ffh
        or     cl, 1
        lea    edx, [eax + 4]
        mov    al, cl
        out    dx, al
        jmp    NEAR PTR loc_1_6C54
loc_1_6C17:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ecx + 198h]
        and    ecx, 0ffh
        or     cl, 1
        lea    edx, [eax + 10h]
        mov    al, cl
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        lea    edx, [eax + 10h]
        mov    al, byte ptr [ecx + 198h]
        out    dx, al
loc_1_6C54:
        xor    ax, ax
        jmp    NEAR PTR loc_1_6C5C
loc_1_6C5C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_6C63:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        movsx  eax, word ptr [ecx + eax*2 + 176h]
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    cx, word ptr [edx + ecx*2 + 16ah]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_6CAB
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_6F44
loc_1_6CAB:
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        inc    eax
        and    eax, 0fffeh
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 17ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    cx, word ptr [edx + ecx*2 + 164h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_6D0C
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_6F44
loc_1_6D0C:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 158h]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [edx + ecx*4 + 14ch]
        jb     NEAR PTR loc_1_6DEF
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 14ch]
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        sub    eax, dword ptr [edx + ecx*4 + 158h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 17ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    cx, word ptr [edx + ecx*2 + 164h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_6DCA
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_6F44
loc_1_6DCA:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 140h]
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 158h], eax
loc_1_6DEF:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 17ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + ecx*2 + 17ch], ax
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        inc    word ptr [ecx + eax*2 + 176h]
        mov    ax, word ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + ecx*4 + 0f4h]
        mov    word ptr [ecx + 0ch], ax
        mov    ax, word ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + ecx*4 + 0f4h]
        mov    word ptr [ecx + 1ah], ax
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0f4h]
        mov    eax, dword ptr [eax + 0eh]
        mov    dword ptr [ebp - 4], eax
        mov    ax, word ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0ah], ax
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 158h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 158h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 6], eax
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 158h]
        mov    ecx, dword ptr [ebp + 10h]
        and    ecx, 0ffffh
        inc    ecx
        and    ecx, 0fffeh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 158h], eax
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0f4h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0f4h]
        mov    eax, dword ptr [eax + 12h]
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 0f4h], eax
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_6F44
loc_1_6F44:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_6F4B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 200h]
        and    eax, 0ffffh
        test   al, 40h
        je     NEAR PTR loc_1_6F7F
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 2], 8002h
        jmp    NEAR PTR loc_1_6F88
loc_1_6F7F:
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 2], 8004h
loc_1_6F88:
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 16ah]
        and    eax, 0ffffh
        cmp    eax, 1
        je     NEAR PTR loc_1_6FDE
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 16h]
        mov    ax, word ptr [eax + 2]
        and    eax, 0fh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 16h]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 16h]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        or     eax, 2000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 16h]
        mov    word ptr [ecx + 2], ax
loc_1_6FDE:
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 182h]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_7024
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + eax*2 + 182h], 1
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7034
        mov    word ptr [ebp - 4], ax
loc_1_7024:
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_702D
loc_1_702D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_7034:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_7059
        mov    ax, 8
        jmp    NEAR PTR loc_1_708F
loc_1_7059:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c8h]
        mov    word ptr [eax], 0c000h
        mov    eax, 100h
        mov    cl, byte ptr [ebp + 0ch]
        shl    eax, cl
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0c8h]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6BD3
        xor    ax, ax
        jmp    NEAR PTR loc_1_708F
loc_1_708F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_7096:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 20h]
        and    eax, 0ffffh
        shl    eax, 0ah
        mov    word ptr [ebp - 18h], ax
        push   0
        lea    eax, [ebp - 24h]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_D810
loc_1_70CB:
        cmp    dword ptr [ebp - 24h], 0
        je     NEAR PTR loc_1_71EB
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        call   sub_1_D834
loc_1_70E6:
        cmp    dword ptr [ebp - 4], 0
        jbe    NEAR PTR loc_1_71D9
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0b0h]
        and    ecx, 0ffffh
        and    eax, ecx
        mov    word ptr [ebp - 2ch], ax
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 2ch]
        and    ecx, 0ffffh
        sub    eax, ecx
        cmp    eax, dword ptr [ebp - 4]
        jae    NEAR PTR loc_1_7154
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 2ch]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp - 14h], ax
        jmp    NEAR PTR loc_1_715B
loc_1_7154:
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [ebp - 14h], ax
loc_1_715B:
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0b0h]
        and    ecx, 0ffffh
        and    eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        call   sub_1_D8CC
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 14h]
        and    ecx, 0ffffh
        sub    eax, ecx
        neg    eax
        sub    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        add    dword ptr [ebp + 10h], eax
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        add    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 14h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 0ch], ax
        jmp    NEAR PTR loc_1_70E6
loc_1_71D9:
        lea    eax, [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        call   sub_1_D858
        jmp    NEAR PTR loc_1_70CB
loc_1_71EB:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        mov    ax, word ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_7204
loc_1_7204:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_720B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0bch]
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 0bch], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_723F
loc_1_723F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_7246:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d2h]
        mov    word ptr [eax], 0
        mov    ax, word ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0d2h]
        mov    word ptr [ecx + 8], ax
        mov    ax, word ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0d2h]
        mov    word ptr [ecx + 0ah], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0d2h]
        mov    word ptr [ecx + 0ch], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9093
        jmp    NEAR PTR loc_1_72A6
loc_1_72A6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_72AD:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_72C5
loc_1_72C1:
        inc    word ptr [ebp - 4]
loc_1_72C5:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    eax, 3
        jge    NEAR PTR loc_1_7362
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0e8h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 11ch]
        mov    dword ptr [ebp - 10h], eax
        mov    word ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_730F
loc_1_730B:
        inc    word ptr [ebp - 0ch]
loc_1_730F:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 16ah]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_735D
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0eh], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 8], eax
        add    dword ptr [ebp - 8], 20h
        add    dword ptr [ebp - 10h], 20h
        jmp    NEAR PTR loc_1_730B
loc_1_735D:
        jmp    NEAR PTR loc_1_72C1
loc_1_7362:
        xor    eax, eax
        jmp    NEAR PTR loc_1_7369
loc_1_7369:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_7370:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d2h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 8001h
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 2], 8000h
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0ah], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0eh], 0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 14h
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    word ptr [ebp - 4], 1
        jmp    NEAR PTR loc_1_73E7
loc_1_73E3:
        inc    word ptr [ebp - 4]
loc_1_73E7:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0deh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_7468
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 8001h
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 2], 8000h
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0ah], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0eh], 0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 14h
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        jmp    NEAR PTR loc_1_73E3
loc_1_7468:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d2h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d2h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d2h]
        mov    eax, dword ptr [eax + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0dah], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d2h]
        mov    eax, dword ptr [eax + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0d6h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0e2h], 0
        xor    eax, eax
        jmp    NEAR PTR loc_1_74C9
loc_1_74C9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_74D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_74E8
loc_1_74E4:
        inc    word ptr [ebp - 4]
loc_1_74E8:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    eax, 3
        jge    NEAR PTR loc_1_7600
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 11ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 20h
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 2], eax
        mov    word ptr [ebp - 8], 1
        jmp    NEAR PTR loc_1_7543
loc_1_753F:
        inc    word ptr [ebp - 8]
loc_1_7543:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 170h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_75AC
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ecx + 10h]
        mov    dword ptr [ecx + 14h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 20h
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 2], eax
        jmp    NEAR PTR loc_1_753F
loc_1_75AC:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 11ch]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 11ch]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 2], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + ecx*4 + 11ch]
        mov    dword ptr [ecx + 14h], eax
        jmp    NEAR PTR loc_1_74E4
loc_1_7600:
        xor    eax, eax
        jmp    NEAR PTR loc_1_7607
loc_1_7607:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_760E:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_7626
loc_1_7622:
        inc    word ptr [ebp - 4]
loc_1_7626:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    eax, 3
        jge    NEAR PTR loc_1_77C7
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0e8h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 2], 8000h
        mov    eax, dword ptr [ebp - 8]
        add    eax, 20h
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 12h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 12h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    word ptr [ebp - 0ch], 1
        jmp    NEAR PTR loc_1_7693
loc_1_768F:
        inc    word ptr [ebp - 0ch]
loc_1_7693:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 16ah]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_770E
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 12h]
        mov    dword ptr [ecx + 16h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 12h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 2], 8000h
        mov    eax, dword ptr [ebp - 8]
        add    eax, 20h
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 12h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 12h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        jmp    NEAR PTR loc_1_768F
loc_1_770E:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0e8h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 12h], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0e8h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + ecx*4 + 0e8h]
        mov    dword ptr [ecx + 16h], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0e8h]
        mov    eax, dword ptr [eax + 12h]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 100h], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0e8h]
        mov    eax, dword ptr [eax + 12h]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 0f4h], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + eax*2 + 176h], 0
        jmp    NEAR PTR loc_1_7622
loc_1_77C7:
        xor    eax, eax
        jmp    NEAR PTR loc_1_77CE
loc_1_77CE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_77D5:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_77ED
loc_1_77E9:
        inc    word ptr [ebp - 8]
loc_1_77ED:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 2
        jge    NEAR PTR loc_1_798B
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 128h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 188h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [eax], 8000h
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [eax + 0ah], 100h
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 20h
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 10h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 2], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 6], eax
        mov    word ptr [ebp - 0ch], 1
        jmp    NEAR PTR loc_1_787E
loc_1_787A:
        inc    word ptr [ebp - 0ch]
loc_1_787E:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 0b6h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_790F
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ecx + 10h]
        mov    dword ptr [ecx + 14h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 10h], eax
        add    dword ptr [ebp - 4], 100h
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [eax + 0ah], 100h
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 20h
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 10h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 2], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 6], eax
        jmp    NEAR PTR loc_1_787A
loc_1_790F:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 128h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 128h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 2], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + ecx*4 + 128h]
        mov    dword ptr [ecx + 14h], eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 128h]
        mov    eax, dword ptr [eax + 10h]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 138h], eax
        jmp    NEAR PTR loc_1_77E9
loc_1_798B:
        xor    eax, eax
        jmp    NEAR PTR loc_1_7992
loc_1_7992:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_7999:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_79B1
loc_1_79AD:
        inc    word ptr [ebp - 4]
loc_1_79B1:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    eax, 2
        jge    NEAR PTR loc_1_7B15
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 10ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 2], 8000h
        mov    eax, dword ptr [ebp - 8]
        add    eax, 20h
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 12h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 12h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    word ptr [ebp - 0ch], 1
        jmp    NEAR PTR loc_1_7A1E
loc_1_7A1A:
        inc    word ptr [ebp - 0ch]
loc_1_7A1E:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 0b2h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_7A99
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 12h]
        mov    dword ptr [ecx + 16h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 12h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 2], 4000h
        mov    eax, dword ptr [ebp - 8]
        add    eax, 20h
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 12h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 12h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        jmp    NEAR PTR loc_1_7A1A
loc_1_7A99:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 10ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 12h], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 10ch]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + ecx*4 + 10ch]
        mov    dword ptr [ecx + 16h], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 10ch]
        mov    eax, dword ptr [eax + 12h]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 114h], eax
        jmp    NEAR PTR loc_1_79AD
loc_1_7B15:
        xor    eax, eax
        jmp    NEAR PTR loc_1_7B1C
loc_1_7B1C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_7B23:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_7B52
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_7BDE
loc_1_7B52:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_7B78
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_7BDE
loc_1_7B78:
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_7B87
loc_1_7B83:
        inc    word ptr [ebp - 8]
loc_1_7B87:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_7BCA
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 10h]
        mov    ax, word ptr [ecx + eax*2]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 0e4h]
        mov    word ptr [edx + ecx*2], ax
        jmp    NEAR PTR loc_1_7B83
loc_1_7BCA:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7246
        jmp    NEAR PTR loc_1_7BDE
loc_1_7BDE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_7BE5:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_7C14
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_7CD4
loc_1_7C14:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_7C3A
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_7CD4
loc_1_7C3A:
        mov    word ptr [ebp - 0ch], 0
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_7C5E
loc_1_7C4B:
        inc    word ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        add    eax, 2
        mov    word ptr [ebp - 8], ax
loc_1_7C5E:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_7CC0
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + ecx]
        and    eax, 0ffh
        shl    eax, 8
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 10h]
        mov    cl, byte ptr [ecx + edx + 1]
        and    ecx, 0ffh
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 0e4h]
        mov    word ptr [edx + ecx*2], ax
        jmp    NEAR PTR loc_1_7C4B
loc_1_7CC0:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7246
        jmp    NEAR PTR loc_1_7CD4
loc_1_7CD4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_7CDB:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_7D0A
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_7D44
loc_1_7D0A:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_7D30
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_7D44
loc_1_7D30:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7246
        jmp    NEAR PTR loc_1_7D44
loc_1_7D44:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_7D7A
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_7DB5
loc_1_7D7A:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_7DA0
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_7DB5
loc_1_7DA0:
        push   5412h
        push   7
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7246
        jmp    NEAR PTR loc_1_7DB5
loc_1_7DB5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_7DBC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_7DF7
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_7F44
loc_1_7DF7:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_7E1D
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_7F44
loc_1_7E1D:
        mov    word ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_7E2C
loc_1_7E28:
        inc    word ptr [ebp - 0ch]
loc_1_7E2C:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        cmp    eax, 3
        jge    NEAR PTR loc_1_7E63
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 0e8h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        add    dword ptr [ebp - 8], 4
        jmp    NEAR PTR loc_1_7E28
loc_1_7E63:
        jmp    NEAR PTR loc_1_7E6C
loc_1_7E68:
        inc    word ptr [ebp - 0ch]
loc_1_7E6C:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        cmp    eax, 8
        jge    NEAR PTR loc_1_7E8F
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax], 0
        add    dword ptr [ebp - 8], 4
        jmp    NEAR PTR loc_1_7E68
loc_1_7E8F:
        mov    word ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_7E9E
loc_1_7E9A:
        inc    word ptr [ebp - 0ch]
loc_1_7E9E:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        cmp    eax, 2
        jge    NEAR PTR loc_1_7EF6
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 10ch]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        add    dword ptr [ebp - 8], 4
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 128h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        add    dword ptr [ebp - 8], 4
        jmp    NEAR PTR loc_1_7E9A
loc_1_7EF6:
        jmp    NEAR PTR loc_1_7EFF
loc_1_7EFB:
        inc    word ptr [ebp - 0ch]
loc_1_7EFF:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        cmp    eax, 2
        jge    NEAR PTR loc_1_7F2F
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax], 0
        add    dword ptr [ebp - 8], 4
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax], 0
        add    dword ptr [ebp - 8], 4
        jmp    NEAR PTR loc_1_7EFB
loc_1_7F2F:
        push   0
        push   86h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7246
        jmp    NEAR PTR loc_1_7F44
loc_1_7F44:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_7F4B:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_7F86
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_84E0
loc_1_7F86:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_7FAC
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_84E0
loc_1_7FAC:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 19ah], 1024h
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 19ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 44h]
        and    eax, 0ffffh
        cmp    eax, 4
        je     NEAR PTR loc_1_7FEE
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 44h]
        and    eax, 0ffffh
        cmp    eax, 6
        jne    NEAR PTR loc_1_800A
loc_1_7FEE:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 19ah]
        and    eax, 0ffffh
        or     eax, 10h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 19ah], ax
loc_1_800A:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 200h]
        and    eax, 0ffffh
        test   al, 20h
        je     NEAR PTR loc_1_803F
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 19ah]
        and    eax, 0ffffh
        or     eax, 100h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 19ah], ax
loc_1_803F:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 200h]
        and    eax, 0ffffh
        test   al, 7
        je     NEAR PTR loc_1_8074
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 19ah]
        and    eax, 0ffffh
        or     eax, 200h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 19ah], ax
loc_1_8074:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 44h]
        and    eax, 0ffffh
        cmp    eax, 4
        je     NEAR PTR loc_1_809E
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 44h]
        and    eax, 0ffffh
        cmp    eax, 6
        jne    NEAR PTR loc_1_80BF
loc_1_809E:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 19ch]
        and    eax, 0ffffh
        or     eax, 3
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 19ch], ax
        jmp    NEAR PTR loc_1_80DB
loc_1_80BF:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 19ch]
        and    eax, 0ffffh
        or     eax, 1
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 19ch], ax
loc_1_80DB:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 19ah]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 19ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 44h]
        and    eax, 0ffffh
        cmp    eax, 5
        je     NEAR PTR loc_1_812D
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 44h]
        and    eax, 0ffffh
        cmp    eax, 7
        jne    NEAR PTR loc_1_81DA
loc_1_812D:
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0fah
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 2710h
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 2710h
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0a28h
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 6590h
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 3a98h
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 1b58h
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0a0h
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 7d0h
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0ah
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 1162h
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 7d0h
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 1388h
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0
        add    dword ptr [ebp - 8], 2
        jmp    NEAR PTR loc_1_8282
loc_1_81DA:
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 3e8h
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 9c40h
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 9c40h
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0a28h
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 6590h
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 3a98h
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 1b58h
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0a0h
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 7d0h
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0ah
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 4588h
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 1f40h
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 4e20h
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0
        add    dword ptr [ebp - 8], 2
loc_1_8282:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 24h]
        and    eax, 0ffh
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + 25h]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 26h]
        and    eax, 0ffh
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + 27h]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 28h]
        and    eax, 0ffh
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + 29h]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 97h]
        and    eax, 0ffffh
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 97h]
        and    ecx, 0ffffh
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 99h]
        and    eax, 0ffffh
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 99h]
        and    ecx, 0ffffh
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 9bh]
        and    eax, 0ffffh
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 9bh]
        and    ecx, 0ffffh
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 9dh]
        and    eax, 0ffffh
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 9dh]
        and    ecx, 0ffffh
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 9fh]
        and    eax, 0ffffh
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 9fh]
        and    ecx, 0ffffh
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0a1h]
        and    eax, 0ffffh
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0a1h]
        and    ecx, 0ffffh
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0a3h]
        and    eax, 0ffffh
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0a3h]
        and    ecx, 0ffffh
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0a5h]
        and    eax, 0ffffh
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0a5h]
        and    ecx, 0ffffh
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0a7h]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0a9h]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx], ax
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0
        add    dword ptr [ebp - 8], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0
        add    dword ptr [ebp - 8], 2
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_84B3
loc_1_84B0:
        inc    dword ptr [ebp - 0ch]
loc_1_84B3:
        cmp    dword ptr [ebp - 0ch], 9
        jae    NEAR PTR loc_1_84CE
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 0
        add    dword ptr [ebp - 8], 2
        jmp    NEAR PTR loc_1_84B0
loc_1_84CE:
        push   0
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7246
        jmp    NEAR PTR loc_1_84E0
loc_1_84E0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_84E7:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c0h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c0h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0b0h]
        and    ecx, 0ffffh
        and    eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c4h]
        mov    word ptr [eax], 0c00eh
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c8h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0c4h]
        mov    dword ptr [ecx + 2], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d2h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0c4h]
        mov    dword ptr [ecx + 6], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0cch]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0c4h]
        mov    dword ptr [ecx + 0ah], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c4h]
        mov    word ptr [eax + 0eh], 3eh
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c8h]
        mov    word ptr [eax], 8000h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c8h]
        mov    word ptr [eax + 2], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c8h]
        mov    word ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c8h]
        mov    word ptr [eax + 6], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c8h]
        mov    word ptr [eax + 8], 0
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_860D
loc_1_860A:
        inc    dword ptr [ebp - 8]
loc_1_860D:
        cmp    dword ptr [ebp - 8], 20h
        jae    NEAR PTR loc_1_863D
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0cch]
        mov    byte ptr [ecx + eax*2], 0f0h
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0cch]
        mov    byte ptr [ecx + eax*2 + 1], 0
        jmp    NEAR PTR loc_1_860A
loc_1_863D:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0d0h], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7370
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_72AD
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_74D0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_760E
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_77D5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7999
        xor    eax, eax
        jmp    NEAR PTR loc_1_8686
loc_1_8686:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_868D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
        add    eax, 0fff6h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0c0h], eax
        push   1ch
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0c4h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0bch]
        add    eax, 0fh
        and    eax, 0fff0h
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0bch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        push   0ah
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0c8h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0bch]
        add    eax, 0fh
        and    eax, 0fff0h
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0bch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0deh]
        and    eax, 0ffffh
        shl    eax, 2
        lea    eax, [eax + eax*4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0d2h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0bch]
        add    eax, 0fh
        and    eax, 0fff0h
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0bch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        push   40h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0cch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0bch]
        add    eax, 0fh
        and    eax, 0fff0h
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0bch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        push   80h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0e4h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0bch]
        add    eax, 0fh
        and    eax, 0fff0h
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0bch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 16ah]
        and    eax, 0ffffh
        shl    eax, 5
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0e8h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 16ch]
        and    eax, 0ffffh
        shl    eax, 5
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0ech], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 16eh]
        and    eax, 0ffffh
        shl    eax, 5
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0f0h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 170h]
        and    eax, 0ffffh
        shl    eax, 5
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 11ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 172h]
        and    eax, 0ffffh
        shl    eax, 5
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 120h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 174h]
        and    eax, 0ffffh
        shl    eax, 5
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 124h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0b2h]
        and    eax, 0ffffh
        shl    eax, 5
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 10ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0b4h]
        and    eax, 0ffffh
        shl    eax, 5
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 110h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0b6h]
        and    eax, 0ffffh
        shl    eax, 5
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 128h], eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 130h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0b8h]
        and    eax, 0ffffh
        shl    eax, 5
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 12ch], eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 134h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 164h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 140h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 140h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 158h], eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 14ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 168h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 148h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 148h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 160h], eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 154h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0b6h]
        and    eax, 0ffffh
        shl    eax, 8
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 188h], eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 190h], eax
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 166h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 144h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 144h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 15ch], eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 150h], eax
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0bch]
        add    eax, 0ffh
        and    eax, 0ff00h
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 0bch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0b8h]
        and    eax, 0ffffh
        shl    eax, 8
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 18ch], eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_720B
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 194h], eax
        xor    eax, eax
        jmp    NEAR PTR loc_1_8AEE
loc_1_8AEE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_8AF5:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        movsx  eax, word ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 114h]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        movsx  ecx, word ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + ecx*4 + 114h]
        mov    dword ptr [ecx + 0eh], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0ch]
        jmp    NEAR PTR loc_1_8B37
loc_1_8B37:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_8B3E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        add    eax, 1ch
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        add    eax, 0fh
        and    eax, 0fff0h
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        add    eax, 0ah
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        add    eax, 0fh
        and    eax, 0fff0h
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0deh]
        and    eax, 0ffffh
        shl    eax, 2
        lea    eax, [eax + eax*4]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        add    eax, 0fh
        and    eax, 0fff0h
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        add    eax, 40h
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        add    eax, 0fh
        and    eax, 0fff0h
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        add    eax, 80h
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        add    eax, 0fh
        and    eax, 0fff0h
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 16ah]
        and    eax, 0ffffh
        shl    eax, 5
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 16ch]
        and    eax, 0ffffh
        shl    eax, 5
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 16eh]
        and    eax, 0ffffh
        shl    eax, 5
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 170h]
        and    eax, 0ffffh
        shl    eax, 5
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 172h]
        and    eax, 0ffffh
        shl    eax, 5
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 174h]
        and    eax, 0ffffh
        shl    eax, 5
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0b2h]
        and    eax, 0ffffh
        shl    eax, 5
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0b4h]
        and    eax, 0ffffh
        shl    eax, 5
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0b6h]
        and    eax, 0ffffh
        shl    eax, 5
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 164h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 168h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0b6h]
        and    eax, 0ffffh
        shl    eax, 8
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        inc    eax
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 166h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        inc    eax
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        add    eax, 100h
        mov    word ptr [ebp - 4], ax
        mov    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    ecx, 120h
        sub    edx, edx
        div    ecx
        jmp    NEAR PTR loc_1_8E6E
loc_1_8E6E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_8E75:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0bch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0deh], 1
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 36h]
        and    eax, 0ffffh
        cmp    eax, 100h
        jge    NEAR PTR loc_1_8EC2
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 36h], 100h
        jmp    NEAR PTR loc_1_8EE2
loc_1_8EC2:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 36h]
        and    eax, 0ffffh
        cmp    eax, 5000h
        jle    NEAR PTR loc_1_8EE2
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 36h], 5000h
loc_1_8EE2:
        mov    eax, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [eax + 36h]
        mov    eax, 5000h
        cdq
        idiv   ecx
        dec    eax
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 3ah], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 3ah]
        and    eax, 0ffffh
        cmp    eax, 6
        jle    NEAR PTR loc_1_8F1E
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 3ah], 6
        jmp    NEAR PTR loc_1_8F3B
loc_1_8F1E:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 3ah]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_8F3B
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 3ah], 1
loc_1_8F3B:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 16eh], 8
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 174h], 8
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 168h], 100h
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 180h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 186h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 16ah], 8
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 170h], 8
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 164h], 400h
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 17ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 182h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 16ch], 6
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 172h], 6
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 166h], 5000h
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 17eh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 184h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0b2h], 8
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0b6h], 8
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0b4h], 20h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_8B3E
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0b8h], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_868D
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_84E7
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7F4B
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_905E
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_908C
loc_1_905E:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7DBC
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_9084
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_908C
loc_1_9084:
        xor    ax, ax
        jmp    NEAR PTR loc_1_908C
loc_1_908C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_9093:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_90C2
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_90FC
loc_1_90C2:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c8h]
        mov    word ptr [eax + 4], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c8h]
        mov    word ptr [eax], 0c000h
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0e0h], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6BD3
        xor    ax, ax
        jmp    NEAR PTR loc_1_90FC
loc_1_90FC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_9103:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_9128
        mov    ax, 8
        jmp    NEAR PTR loc_1_9176
loc_1_9128:
        movsx  eax, word ptr [ebp + 0ch]
        test   eax, eax
        jne    NEAR PTR loc_1_9148
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c8h]
        mov    word ptr [eax + 4], 10h
        jmp    NEAR PTR loc_1_9157
loc_1_9148:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c8h]
        mov    word ptr [eax + 4], 40h
loc_1_9157:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c8h]
        mov    word ptr [eax], 0c000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6BD3
        xor    ax, ax
        jmp    NEAR PTR loc_1_9176
loc_1_9176:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_917D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_91AC
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_91FA
loc_1_91AC:
        movsx  eax, word ptr [ebp + 0ch]
        test   eax, eax
        jne    NEAR PTR loc_1_91CC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c8h]
        mov    word ptr [eax + 4], 20h
        jmp    NEAR PTR loc_1_91DB
loc_1_91CC:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c8h]
        mov    word ptr [eax + 4], 80h
loc_1_91DB:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c8h]
        mov    word ptr [eax], 0c000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6BD3
        xor    ax, ax
        jmp    NEAR PTR loc_1_91FA
loc_1_91FA:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_9201:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_9230
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_925E
loc_1_9230:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c8h]
        mov    word ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c8h]
        mov    word ptr [eax], 8001h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6BD3
        xor    ax, ax
        jmp    NEAR PTR loc_1_925E
loc_1_925E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_9265:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_9294
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_92C4
loc_1_9294:
        mov    ax, word ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0c8h]
        mov    word ptr [ecx + 6], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c8h]
        mov    word ptr [eax], 8004h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6BD3
        xor    ax, ax
        jmp    NEAR PTR loc_1_92C4
loc_1_92C4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_92CB:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        or     cl, 40h
        mov    dx, word ptr [eax + 4]
        mov    al, cl
        out    dx, al
        xor    ax, ax
        jmp    NEAR PTR loc_1_92F9
loc_1_92F9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_9300:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 46h]
        and    eax, 0ffffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_9366
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 4]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 0e0h
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0b0h]
        and    eax, 0ffffh
        not    eax
        and    eax, dword ptr [ebp + 0ch]
        shr    eax, 0dh
        and    eax, 0ffh
        or     al, byte ptr [ebp - 8]
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 4]
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        jmp    NEAR PTR loc_1_938E
loc_1_9366:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        and    ah, 0c0h
        mov    byte ptr [ebp - 8], ah
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        lea    edx, [eax + 11h]
        mov    al, byte ptr [ebp - 8]
        out    dx, al
loc_1_938E:
        xor    ax, ax
        jmp    NEAR PTR loc_1_9396
loc_1_9396:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_939D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6A9B
        push   1388h
        call   sub_1_D7E0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6AD0
        push   1388h
        call   sub_1_D7E0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 46h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_943C
loc_1_93E0:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ecx + 198h]
        and    ecx, 0ffh
        or     cl, 0ch
        lea    edx, [eax + 10h]
        mov    al, cl
        out    dx, al
        jmp    NEAR PTR loc_1_945F
loc_1_9409:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ecx + 198h]
        and    ecx, 0ffh
        or     cl, 4
        lea    edx, [eax + 4]
        mov    al, cl
        out    dx, al
        jmp    NEAR PTR loc_1_945F
        db  0E9h, 028h, 000h, 000h, 000h, 0E9h, 023h, 000h, 000h, 000h
loc_1_943C:
        cmp    dword ptr [ebp - 8], 2
        je     NEAR PTR loc_1_93E0
        cmp    dword ptr [ebp - 8], 3
        je     NEAR PTR loc_1_9409
        cmp    dword ptr [ebp - 8], 5
        je     NEAR PTR loc_1_93E0
        jmp    NEAR PTR loc_1_945F
loc_1_945F:
        xor    ax, ax
        jmp    NEAR PTR loc_1_9467
loc_1_9467:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_946E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_948A
loc_1_9483:
        add    dword ptr [ebp - 4], 1000h
loc_1_948A:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 20h]
        and    eax, 0ffffh
        shl    eax, 0ah
        cmp    eax, dword ptr [ebp - 4]
        jbe    NEAR PTR loc_1_94CD
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + ecx]
        and    eax, 0ffffh
        cmp    eax, 0aa55h
        jne    NEAR PTR loc_1_94C8
        mov    ax, 85h
        jmp    NEAR PTR loc_1_94D5
loc_1_94C8:
        jmp    NEAR PTR loc_1_9483
loc_1_94CD:
        xor    ax, ax
        jmp    NEAR PTR loc_1_94D5
loc_1_94D5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_94DC:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_94F8
loc_1_94F1:
        add    dword ptr [ebp - 0ch], 400h
loc_1_94F8:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 20h]
        and    eax, 0ffffh
        shl    eax, 0ah
        cmp    eax, dword ptr [ebp - 0ch]
        jbe    NEAR PTR loc_1_9563
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
        add    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax], 1234h
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 10h], ax
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        cmp    eax, 1234h
        jne    NEAR PTR loc_1_955E
        mov    ax, word ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx], ax
        mov    ax, 84h
        jmp    NEAR PTR loc_1_956B
loc_1_955E:
        jmp    NEAR PTR loc_1_94F1
loc_1_9563:
        xor    ax, ax
        jmp    NEAR PTR loc_1_956B
loc_1_956B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_9572:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 18h], 0
        mov    word ptr [ebp - 2ch], 1
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 20h]
        and    ecx, 0ffffh
        mov    ax, word ptr [eax + 16h]
        and    eax, 0ffffh
        cdq
        idiv   ecx
        mov    word ptr [ebp - 14h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 1ch], eax
        mov    word ptr [ebp - 24h], 0
        jmp    NEAR PTR loc_1_95CE
loc_1_95B9:
        inc    word ptr [ebp - 24h]
        mov    eax, dword ptr [ebp - 2ch]
        and    eax, 0ffffh
        add    eax, 8000h
        mov    word ptr [ebp - 2ch], ax
loc_1_95CE:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 24h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_9702
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        not    eax
        test   eax, eax
        je     NEAR PTR loc_1_9702
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 20h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 24h]
        and    ecx, 0ffffh
        imul   eax, ecx
        shl    eax, 0ah
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 1ch]
        inc    eax
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        mov    ax, word ptr [ebp - 2ch]
        mov    word ptr [ebp - 28h], ax
        mov    dword ptr [ebp - 20h], 1
        jmp    NEAR PTR loc_1_963A
loc_1_9636:
        add    dword ptr [ebp - 20h], 2
loc_1_963A:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 20h]
        and    eax, 0ffffh
        shl    eax, 0ah
        dec    eax
        cmp    eax, dword ptr [ebp - 20h]
        jbe    NEAR PTR loc_1_966A
        mov    ax, word ptr [ebp - 28h]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    edx, dword ptr [ebp - 20h]
        mov    word ptr [ecx + edx], ax
        inc    word ptr [ebp - 28h]
        jmp    NEAR PTR loc_1_9636
loc_1_966A:
        mov    ax, word ptr [ebp - 2ch]
        mov    word ptr [ebp - 28h], ax
        mov    dword ptr [ebp - 20h], 1
        jmp    NEAR PTR loc_1_9686
loc_1_967E:
        add    dword ptr [ebp - 20h], 2
        inc    word ptr [ebp - 28h]
loc_1_9686:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 20h]
        and    eax, 0ffffh
        shl    eax, 0ah
        dec    eax
        cmp    eax, dword ptr [ebp - 20h]
        jbe    NEAR PTR loc_1_96FD
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        not    eax
        test   eax, eax
        je     NEAR PTR loc_1_96FD
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp - 20h]
        mov    ax, word ptr [eax + ecx]
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 28h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_96F8
        mov    byte ptr [ebp - 18h], 1
        mov    eax, dword ptr [ebp - 20h]
        mov    dword ptr [ebp - 10h], eax
        mov    ax, word ptr [ebp - 0ch]
        mov    word ptr [ebp - 8], ax
        mov    ax, word ptr [ebp - 28h]
        mov    word ptr [ebp - 4], ax
        mov    ax, 83h
        jmp    NEAR PTR loc_1_9848
loc_1_96F8:
        jmp    NEAR PTR loc_1_967E
loc_1_96FD:
        jmp    NEAR PTR loc_1_95B9
loc_1_9702:
        mov    word ptr [ebp - 24h], 0
        jmp    NEAR PTR loc_1_9711
loc_1_970D:
        inc    word ptr [ebp - 24h]
loc_1_9711:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 24h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_9830
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        not    eax
        test   eax, eax
        je     NEAR PTR loc_1_9830
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 20h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 24h]
        and    ecx, 0ffffh
        imul   eax, ecx
        shl    eax, 0ah
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        mov    word ptr [ebp - 28h], 0ffffh
        mov    dword ptr [ebp - 20h], 0
        jmp    NEAR PTR loc_1_977A
loc_1_9776:
        add    dword ptr [ebp - 20h], 2
loc_1_977A:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 20h]
        and    eax, 0ffffh
        shl    eax, 0ah
        cmp    eax, dword ptr [ebp - 20h]
        jbe    NEAR PTR loc_1_97A5
        mov    ax, word ptr [ebp - 28h]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    edx, dword ptr [ebp - 20h]
        mov    word ptr [ecx + edx], ax
        jmp    NEAR PTR loc_1_9776
loc_1_97A5:
        mov    dword ptr [ebp - 20h], 0
        jmp    NEAR PTR loc_1_97B5
loc_1_97B1:
        add    dword ptr [ebp - 20h], 2
loc_1_97B5:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 20h]
        and    eax, 0ffffh
        shl    eax, 0ah
        cmp    eax, dword ptr [ebp - 20h]
        jbe    NEAR PTR loc_1_982B
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        not    eax
        test   eax, eax
        je     NEAR PTR loc_1_982B
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp - 20h]
        mov    ax, word ptr [eax + ecx]
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 28h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_9826
        mov    byte ptr [ebp - 18h], 1
        mov    eax, dword ptr [ebp - 20h]
        mov    dword ptr [ebp - 10h], eax
        mov    ax, word ptr [ebp - 0ch]
        mov    word ptr [ebp - 8], ax
        mov    ax, word ptr [ebp - 28h]
        mov    word ptr [ebp - 4], ax
        mov    ax, 83h
        jmp    NEAR PTR loc_1_9848
loc_1_9826:
        jmp    NEAR PTR loc_1_97B1
loc_1_982B:
        jmp    NEAR PTR loc_1_970D
loc_1_9830:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        xor    ax, ax
        jmp    NEAR PTR loc_1_9848
loc_1_9848:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_984F:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6A9B
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_92CB
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 46h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_98F8
loc_1_9884:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 198h]
        and    eax, 0fdh
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 198h], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        lea    edx, [eax + 10h]
        mov    al, byte ptr [ecx + 198h]
        out    dx, al
        jmp    NEAR PTR loc_1_991B
loc_1_98B9:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 198h]
        and    eax, 0fdh
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 198h], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        lea    edx, [eax + 4]
        mov    al, byte ptr [ecx + 198h]
        out    dx, al
        jmp    NEAR PTR loc_1_991B
        db  0E9h, 028h, 000h, 000h, 000h, 0E9h, 023h, 000h, 000h, 000h
loc_1_98F8:
        cmp    dword ptr [ebp - 0ch], 2
        je     NEAR PTR loc_1_9884
        cmp    dword ptr [ebp - 0ch], 3
        je     NEAR PTR loc_1_98B9
        cmp    dword ptr [ebp - 0ch], 5
        je     NEAR PTR loc_1_9884
        jmp    NEAR PTR loc_1_991B
loc_1_991B:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
        mov    al, byte ptr [eax + 1ffdh]
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 95h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 95h]
        and    eax, 0ffffh
        cmp    eax, 54h
        jge    NEAR PTR loc_1_9957
        mov    ax, 12h
        jmp    NEAR PTR loc_1_9A68
loc_1_9957:
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_9971
loc_1_9962:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        add    eax, 2
        mov    word ptr [ebp - 4], ax
loc_1_9971:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    eax, 2000h
        jge    NEAR PTR loc_1_99B0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        mov    ax, word ptr [eax + ecx]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 8], ax
        jmp    NEAR PTR loc_1_9962
loc_1_99B0:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 46h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_9A3C
loc_1_99C4:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 198h]
        and    eax, 0ffh
        or     al, 2
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 198h], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        lea    edx, [eax + 10h]
        mov    al, byte ptr [ecx + 198h]
        out    dx, al
        jmp    NEAR PTR loc_1_9A5F
loc_1_99FB:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 198h]
        and    eax, 0ffh
        or     al, 2
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 198h], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        lea    edx, [eax + 4]
        mov    al, byte ptr [ecx + 198h]
        out    dx, al
        jmp    NEAR PTR loc_1_9A5F
        db  0E9h, 028h, 000h, 000h, 000h, 0E9h, 023h, 000h, 000h, 000h
loc_1_9A3C:
        cmp    dword ptr [ebp - 10h], 2
        je     NEAR PTR loc_1_99C4
        cmp    dword ptr [ebp - 10h], 3
        je     NEAR PTR loc_1_99FB
        cmp    dword ptr [ebp - 10h], 5
        je     NEAR PTR loc_1_99C4
        jmp    NEAR PTR loc_1_9A5F
loc_1_9A5F:
        mov    ax, word ptr [ebp - 8]
        jmp    NEAR PTR loc_1_9A68
loc_1_9A68:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_9A6F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6B39
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_9AA2
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_9B59
loc_1_9AA2:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_9AC8
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_9B59
loc_1_9AC8:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d2h]
        cmp    byte ptr [eax], 0
        je     NEAR PTR loc_1_9AE3
        mov    ax, 82h
        jmp    NEAR PTR loc_1_9B59
loc_1_9AE3:
        push   3
        push   81h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6B39
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_9B10
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_9B59
loc_1_9B10:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_9B36
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_9B59
loc_1_9B36:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d2h]
        cmp    byte ptr [eax], 0
        je     NEAR PTR loc_1_9B51
        mov    ax, 82h
        jmp    NEAR PTR loc_1_9B59
loc_1_9B51:
        xor    ax, ax
        jmp    NEAR PTR loc_1_9B59
loc_1_9B59:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_9B60:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 61h], 0
        je     NEAR PTR loc_1_9B8C
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ax, word ptr [eax]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 61h]
        add    dword ptr [ecx], eax
loc_1_9B8C:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 65h], 0
        je     NEAR PTR loc_1_9BB5
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        sar    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 65h]
        add    dword ptr [ecx], eax
loc_1_9BB5:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 69h], 0
        je     NEAR PTR loc_1_9BDC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 69h]
        add    dword ptr [ecx], eax
loc_1_9BDC:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 6dh], 0
        je     NEAR PTR loc_1_9C06
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        sar    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 6dh]
        add    dword ptr [ecx], eax
loc_1_9C06:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 71h], 0
        je     NEAR PTR loc_1_9C30
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        sar    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 71h]
        add    dword ptr [ecx], eax
loc_1_9C30:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 75h], 0
        je     NEAR PTR loc_1_9C57
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 75h]
        add    dword ptr [ecx], eax
loc_1_9C57:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 79h], 0
        je     NEAR PTR loc_1_9C81
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        sar    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 79h]
        add    dword ptr [ecx], eax
loc_1_9C81:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 7dh], 0
        je     NEAR PTR loc_1_9CA8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 7dh]
        add    dword ptr [ecx], eax
loc_1_9CA8:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 81h], 0
        je     NEAR PTR loc_1_9CD8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        sar    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 81h]
        add    dword ptr [ecx], eax
loc_1_9CD8:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 85h], 0
        je     NEAR PTR loc_1_9D08
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        sar    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 85h]
        add    dword ptr [ecx], eax
loc_1_9D08:
        xor    ax, ax
        jmp    NEAR PTR loc_1_9D10
loc_1_9D10:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_9D17:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 46h]
        and    eax, 0ffffh
        cmp    eax, 3
        jne    NEAR PTR loc_1_9D5B
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ecx + 198h]
        and    ecx, 0ffh
        or     cl, 4
        lea    edx, [eax + 4]
        mov    al, cl
        out    dx, al
        jmp    NEAR PTR loc_1_9D7F
loc_1_9D5B:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ecx + 198h]
        and    ecx, 0ffh
        or     cl, 8
        lea    edx, [eax + 10h]
        mov    al, cl
        out    dx, al
loc_1_9D7F:
        xor    ax, ax
        jmp    NEAR PTR loc_1_9D87
loc_1_9D87:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_9D8E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_68FD
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_9DB3
        mov    ax, 8
        jmp    NEAR PTR loc_1_9E09
loc_1_9DB3:
        mov    ax, word ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0c8h]
        mov    word ptr [ecx + 6], ax
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        add    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0c8h]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c8h]
        mov    word ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c8h]
        mov    word ptr [eax], 0a004h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6BD3
        xor    ax, ax
        jmp    NEAR PTR loc_1_9E09
loc_1_9E09:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_9E10:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 114h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 0ch]
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [eax + 2], 8000h
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 16h]
        mov    word ptr [eax + 2], 4000h
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 114h]
        mov    eax, dword ptr [eax + 12h]
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 114h], eax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        sar    eax, 3
        mov    word ptr [ebp - 8], ax
        cmp    byte ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_9EA6
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        add    eax, 20h
        mov    word ptr [ebp - 8], ax
loc_1_9EA6:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffe0h
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 138h]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 4]
        cmp    dword ptr [ecx + eax*4 + 130h], edx
        ja     NEAR PTR loc_1_9F18
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 128h]
        add    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        sub    eax, dword ptr [edx + ecx*4 + 130h]
        mov    dword ptr [ebp - 4], eax
loc_1_9F18:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 14h]
        mov    word ptr [eax], 8000h
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 138h]
        mov    eax, dword ptr [eax + 14h]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 138h], eax
        xor    ax, ax
        jmp    NEAR PTR loc_1_9F5B
loc_1_9F5B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_9F62:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        movsx  eax, word ptr [ecx + eax*2 + 176h]
        test   eax, eax
        jg     NEAR PTR loc_1_9F91
        mov    ax, 8
        jmp    NEAR PTR loc_1_A076
        db  0E9h, 0E5h, 000h, 000h, 000h
loc_1_9F91:
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 17ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cx, word ptr [ecx + 1ah]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    ecx, dword ptr [ebp + 10h]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + ecx*2 + 17ch], ax
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 17ch]
        and    eax, 0ffffh
        test   eax, eax
        jge    NEAR PTR loc_1_9FF5
        mov    ax, 8
        jmp    NEAR PTR loc_1_A076
loc_1_9FF5:
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 17ch]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_A03A
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 140h]
        mov    ecx, dword ptr [ebp + 10h]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 158h], eax
loc_1_A03A:
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        dec    word ptr [ecx + eax*2 + 176h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 12h]
        mov    ecx, dword ptr [ebp + 10h]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 100h], eax
        xor    ax, ax
        jmp    NEAR PTR loc_1_A076
loc_1_A076:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_A07D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
loc_1_A08C:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 100h]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_A1C0
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    eax, 5
        je     NEAR PTR loc_1_A1C0
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   ah, 7eh
        je     NEAR PTR loc_1_A0E0
        mov    ax, 8
        jmp    NEAR PTR loc_1_A1C9
loc_1_A0E0:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 100h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9F62
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_A116
        mov    ax, 8
        jmp    NEAR PTR loc_1_A1C9
loc_1_A116:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_A1BB
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 200h]
        and    eax, 0ffffh
        test   al, 40h
        je     NEAR PTR loc_1_A1A3
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   al, 88h
        jne    NEAR PTR loc_1_A16B
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   10h
        call   sub_1_CBF8
        mov    word ptr [ebp - 4], ax
        jmp    NEAR PTR loc_1_A19E
loc_1_A16B:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   al, 44h
        jne    NEAR PTR loc_1_A18F
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   11h
        call   sub_1_CBF8
        mov    word ptr [ebp - 4], ax
        jmp    NEAR PTR loc_1_A19E
loc_1_A18F:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   0
        call   sub_1_CBF8
        mov    word ptr [ebp - 4], ax
loc_1_A19E:
        jmp    NEAR PTR loc_1_A1B2
loc_1_A1A3:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   0
        call   sub_1_CBF8
        mov    word ptr [ebp - 4], ax
loc_1_A1B2:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_67FC
loc_1_A1BB:
        jmp    NEAR PTR loc_1_A08C
loc_1_A1C0:
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_A1C9
loc_1_A1C9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_A1D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        movsx  eax, word ptr [ecx + eax*2 + 176h]
        test   eax, eax
        jle    NEAR PTR loc_1_A240
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*2 + 182h]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_A240
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + eax*2 + 182h], 1
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7034
        mov    word ptr [ebp - 4], ax
loc_1_A240:
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_A249
loc_1_A249:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_A250:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0bah]
        mov    word ptr [ebp - 14h], ax
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 114h]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 0ch], ax
loc_1_A286:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 114h]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_A3EF
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    eax, 5
        je     NEAR PTR loc_1_A3EF
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        test   al, 7fh
        je     NEAR PTR loc_1_A313
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 3eh]
        and    eax, 0ffffh
        test   al, 10h
        jne    NEAR PTR loc_1_A313
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_67FC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9E10
        mov    ax, 8
        jmp    NEAR PTR loc_1_A3F8
loc_1_A313:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 114h]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        sub    eax, 5
        mov    word ptr [ebp - 10h], ax
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_A361
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        push   eax
        call   sub_1_CDF8
        mov    word ptr [ebp - 4], ax
        jmp    NEAR PTR loc_1_A3C4
loc_1_A361:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_8AF5
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 1ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 0b0h]
        and    ecx, 0ffffh
        and    eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_C647
        mov    word ptr [ebp - 4], ax
loc_1_A3C4:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_67FC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9E10
        jmp    NEAR PTR loc_1_A286
loc_1_A3EF:
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_A3F8
loc_1_A3F8:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_A3FF:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 8], 80h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_67FC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        lea    eax, [ebp - 8]
        push   eax
        push   0b601h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7BE5
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_A45F
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_A55D
loc_1_A45F:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_A485
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_A55D
loc_1_A485:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 9fh
        push   eax
        push   0b702h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7BE5
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_A4C2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_A55D
loc_1_A4C2:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_A4E8
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_A55D
loc_1_A4E8:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 9dh
        push   eax
        push   0b601h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_7BE5
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_A525
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_A55D
loc_1_A525:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_685C
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_A54B
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_A55D
loc_1_A54B:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_A55D
loc_1_A55D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
_LTEXT ends
        end
