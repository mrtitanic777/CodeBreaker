; sb16_lcode_01_02.asm -- SB16 object 1 segment _LTEXT  (part 2/2)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_61DB:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [esp + 8]
        push   1
        push   dword ptr [eax + 0e8h]
        call   sub_1_3846
        add    esp, 0ch
        xor    eax, eax
        ret
sub_1_61F6:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    edx, dword ptr [esp + 0ch]
        push   edi
        lea    esi, [eax + 4ch]
        mov    edi, edx
        mov    ecx, 1ah
        rep movsd
        mov    ecx, dword ptr [eax + 4]
        pop    edi
        xor    eax, eax
        pop    esi
        mov    dword ptr [edx + 4], ecx
        ret
sub_1_6217:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ecx + 4]
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        lea    esi, [ecx + 0e8h]
        test   al, 1
        mov    edx, dword ptr [esi]
        jne    SHORT loc_1_623F
        mov    eax, 3
        jmp    NEAR PTR loc_1_62F4
loc_1_623F:
        test   al, 2
        je     SHORT loc_1_624D
        mov    eax, 4
        jmp    NEAR PTR loc_1_62F4
loc_1_624D:
        mov    eax, dword ptr [edx + 0f0h]
        and    eax, 4
        jne    SHORT loc_1_6261
        test   byte ptr [edx + 1dch], 4
        je     SHORT loc_1_6266
loc_1_6261:
        mov    ebx, 1
loc_1_6266:
        mov    edi, dword ptr [ebp + 10h]
        cmp    word ptr [edi + 12h], 2
        jne    SHORT loc_1_62A3
        test   ebx, ebx
        je     SHORT loc_1_62A3
        test   eax, eax
        je     SHORT loc_1_628A
        mov    eax, dword ptr [edi + 0eh]
        cmp    dword ptr [edx + 11ch], eax
        je     SHORT loc_1_628A
        mov    eax, 4
        jmp    SHORT loc_1_62F4
loc_1_628A:
        test   byte ptr [edx + 1dch], 4
        je     SHORT loc_1_62A3
        mov    eax, dword ptr [edi + 0eh]
        cmp    dword ptr [edx + 208h], eax
        mov    eax, 4
        jne    SHORT loc_1_62F4
loc_1_62A3:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 0b4h], eax
        mov    edx, dword ptr [edi + 6]
        mov    dword ptr [ecx + 0b8h], edx
        mov    ebx, dword ptr [edi + 0ah]
        mov    dword ptr [ecx + 0bch], ebx
        mov    eax, dword ptr [edi + 0eh]
        mov    dword ptr [ecx + 30h], eax
        mov    dx, word ptr [edi + 12h]
        mov    word ptr [ecx + 34h], dx
        or     byte ptr [ecx + 4], 2
        lea    ecx, [ebp - 2]
        push   ecx
        push   dword ptr [esi]
        call   sub_1_3984
        mov    cx, word ptr [ebp - 2]
        add    esp, 8
        mov    word ptr [edi + 4], cx
        push   2
        push   dword ptr [esi]
        call   sub_1_394B
        add    esp, 8
        xor    eax, eax
loc_1_62F4:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_62FB:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        lea    edx, [ecx + 0b4h]
        cmp    dword ptr [edx], eax
        je     SHORT loc_1_6314
        mov    eax, 1
        jmp    SHORT loc_1_634E
loc_1_6314:
        cmp    dword ptr [ecx + 8], 0
        je     SHORT loc_1_6321
        mov    eax, 8
        jmp    SHORT loc_1_634E
loc_1_6321:
        test   byte ptr [ecx + 4], 1
        mov    eax, 3
        je     SHORT loc_1_634E
        xor    eax, eax
        push   3
        mov    dword ptr [edx], eax
        mov    dword ptr [ecx + 0b8h], eax
        mov    dword ptr [edx], eax
        and    byte ptr [ecx + 4], 0fdh
        push   dword ptr [ecx + 0e8h]
        call   sub_1_394B
        add    esp, 8
        xor    eax, eax
loc_1_634E:
        ret
sub_1_634F:
        mov    eax, dword ptr [esp + 4]
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0e0h]
        mov    edi, dword ptr [esp + 10h]
        add    esi, 4
        movsd
        movsd
        movsd
        xor    eax, eax
        pop    edi
        pop    esi
        ret
sub_1_636A:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_6389
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_6387
        mov    ecx, dword ptr [esi + 8]
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], ecx
        xor    eax, eax
loc_1_6387:
        pop    esi
        ret
sub_1_6389:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 4]
        and    eax, 1
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 3
        ret
sub_1_639C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        xor    eax, eax
        jmp    NEAR PTR loc_1_63B2
loc_1_63B2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_63B7:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        or     byte ptr [eax], 1
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 2ch], eax
        xor    eax, eax
        jmp    NEAR PTR loc_1_63DC
loc_1_63DC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_63E1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax], 1
        je     NEAR PTR loc_1_63FB
        jmp    NEAR PTR loc_1_6405
loc_1_63FB:
        mov    eax, 3
        jmp    NEAR PTR loc_1_652C
loc_1_6405:
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax], 2
        je     NEAR PTR loc_1_6439
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 4], ecx
        jne    NEAR PTR loc_1_642F
        mov    eax, 4
        jmp    NEAR PTR loc_1_652C
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_1_642F:
        mov    eax, 4
        jmp    NEAR PTR loc_1_652C
loc_1_6439:
        mov    eax, dword ptr [ebp + 8]
        or     byte ptr [eax], 2
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 30h], 2
        push   dword ptr [ebp + 8]
        call   sub_1_672C
        add    esp, 4
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_647D
loc_1_6479:
        inc    word ptr [ebp - 4]
loc_1_647D:
        movzx  eax, word ptr [ebp - 4]
        cmp    eax, 12h
        jge    NEAR PTR loc_1_64A4
        movzx  eax, word ptr [ebp - 4]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [eax + ecx + 87h], 0
        jmp    NEAR PTR loc_1_6479
loc_1_64A4:
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_64B3
loc_1_64AF:
        inc    word ptr [ebp - 4]
loc_1_64B3:
        movzx  eax, word ptr [ebp - 4]
        cmp    eax, 10h
        jge    NEAR PTR loc_1_64E3
        movzx  eax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 1e9h], 4
        movzx  eax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 1f9h], 0ffh
        jmp    NEAR PTR loc_1_64AF
loc_1_64E3:
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_650A
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 2]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0ch], eax
        jmp    NEAR PTR loc_1_6515
loc_1_650A:
        push   dword ptr [ebp + 8]
        call   sub_1_6667
        add    esp, 4
loc_1_6515:
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax + 2ch]
        call   sub_1_394B
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_1_652C
loc_1_652C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_6531:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax], 1
        je     NEAR PTR loc_1_654B
        jmp    NEAR PTR loc_1_6555
loc_1_654B:
        mov    eax, 3
        jmp    NEAR PTR loc_1_6605
loc_1_6555:
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax], 2
        je     NEAR PTR loc_1_65FE
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 4], ecx
        jne    NEAR PTR loc_1_65F4
        mov    eax, dword ptr [ebp + 8]
        and    byte ptr [eax], 0fdh
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_6585
loc_1_6581:
        inc    word ptr [ebp - 4]
loc_1_6585:
        movzx  eax, word ptr [ebp - 4]
        cmp    eax, 12h
        jge    NEAR PTR loc_1_65D8
        movzx  eax, word ptr [ebp - 4]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 82h]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 81h]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 83h]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_759B
        add    esp, 10h
        jmp    NEAR PTR loc_1_6581
loc_1_65D8:
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax + 2ch]
        call   sub_1_394B
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_1_6605
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_1_65F4:
        mov    eax, 4
        jmp    NEAR PTR loc_1_6605
loc_1_65FE:
        xor    eax, eax
        jmp    NEAR PTR loc_1_6605
loc_1_6605:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_660A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_1_68A3
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        and    byte ptr [eax], 0feh
        xor    eax, eax
        jmp    NEAR PTR loc_1_6628
loc_1_6628:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_662D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   eax
        in     al, dx
        jmp    SHORT loc_1_6637
loc_1_6637:
        in     al, dx
        jmp    SHORT loc_1_663A
loc_1_663A:
        pop    eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_6640:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   ebx
        push   edx
        push   eax
        mov    eax, ebx
        shl    ah, 1
        add    dl, ah
        out    dx, al
        sub    dl, ah
        call   sub_1_662D
        pop    eax
        inc    dl
        out    dx, al
        call   sub_1_662D
        pop    edx
        pop    ebx
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_6667:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 4], ax
        push   eax
        push   edx
        push   ebx
        push   ecx
        movzx  edx, word ptr [ebp - 4]
        mov    ebx, 105h
        mov    al, 1
        call   sub_1_6640
        mov    ebx, 4
        mov    al, 60h
        call   sub_1_6640
        mov    ebx, 104h
        mov    al, 3fh
        call   sub_1_6640
        mov    ebx, 8
        xor    al, al
        call   sub_1_6640
        mov    ebx, 0bdh
        mov    al, 0c0h
        call   sub_1_6640
        mov    ecx, 15h
        mov    ebx, 40h
        mov    al, 3fh
loc_1_66CE:
        xor    bh, bh
        call   sub_1_6640
        mov    bh, 1
        call   sub_1_6640
        inc    bl
        loop   loc_1_66CE
        mov    ecx, 8
        mov    ebx, 0b0h
        xor    eax, eax
loc_1_66EC:
        xor    bh, bh
        call   sub_1_6640
        mov    bh, 1
        call   sub_1_6640
        inc    bl
        loop   loc_1_66EC
        mov    ebx, 104h
        xor    eax, eax
        call   sub_1_6640
        mov    ebx, 105h
        call   sub_1_6640
        call   sub_1_662D
        call   sub_1_662D
        call   sub_1_662D
        pop    ecx
        pop    ebx
        pop    edx
        pop    eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_672C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0ch
        push   60h
        push   4
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0ch
        push   0
        push   8
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + 30h]
        cmp    eax, 2
        jne    NEAR PTR loc_1_6796
        push   1
        push   105h
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0ch
        push   0
        push   104h
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0ch
loc_1_6796:
        push   0c0h
        push   0bdh
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0ch
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_67BA
loc_1_67B6:
        inc    word ptr [ebp - 4]
loc_1_67BA:
        movzx  eax, word ptr [ebp - 4]
        cmp    eax, 15h
        jge    NEAR PTR loc_1_6808
        push   3fh
        movzx  eax, word ptr [ebp - 4]
        add    eax, 40h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + 30h]
        cmp    eax, 2
        jne    NEAR PTR loc_1_6803
        push   3fh
        movzx  eax, word ptr [ebp - 4]
        add    eax, 140h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0ch
loc_1_6803:
        jmp    NEAR PTR loc_1_67B6
loc_1_6808:
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_6817
loc_1_6813:
        inc    word ptr [ebp - 4]
loc_1_6817:
        movzx  eax, word ptr [ebp - 4]
        cmp    eax, 8
        jge    NEAR PTR loc_1_6867
        push   0
        movzx  eax, word ptr [ebp - 4]
        add    eax, 0b0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + 30h]
        cmp    eax, 2
        jne    NEAR PTR loc_1_6862
        push   0
        movzx  eax, word ptr [ebp - 4]
        add    eax, 1b0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0ch
loc_1_6862:
        jmp    NEAR PTR loc_1_6813
loc_1_6867:
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_6876
loc_1_6872:
        inc    word ptr [ebp - 4]
loc_1_6876:
        movzx  eax, word ptr [ebp - 4]
        cmp    eax, 10h
        jge    NEAR PTR loc_1_6897
        movzx  eax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + eax*4 + 31h], 0
        jmp    NEAR PTR loc_1_6872
loc_1_6897:
        xor    eax, eax
        jmp    NEAR PTR loc_1_689E
loc_1_689E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_68A3:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_68BB
loc_1_68B7:
        inc    word ptr [ebp - 4]
loc_1_68BB:
        movzx  eax, word ptr [ebp - 4]
        cmp    eax, 12h
        jge    NEAR PTR loc_1_690E
        movzx  eax, word ptr [ebp - 4]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 82h]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 81h]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 83h]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_759B
        add    esp, 10h
        jmp    NEAR PTR loc_1_68B7
loc_1_690E:
        push   dword ptr [ebp + 8]
        call   sub_1_672C
        add    esp, 4
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_6928
loc_1_6924:
        inc    word ptr [ebp - 4]
loc_1_6928:
        movzx  eax, word ptr [ebp - 4]
        cmp    eax, 12h
        jge    NEAR PTR loc_1_694F
        movzx  eax, word ptr [ebp - 4]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [eax + ecx + 87h], 0
        jmp    NEAR PTR loc_1_6924
loc_1_694F:
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_695E
loc_1_695A:
        inc    word ptr [ebp - 4]
loc_1_695E:
        movzx  eax, word ptr [ebp - 4]
        cmp    eax, 10h
        jge    NEAR PTR loc_1_698E
        movzx  eax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 1e9h], 4
        movzx  eax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 1f9h], 0ffh
        jmp    NEAR PTR loc_1_695A
loc_1_698E:
        xor    eax, eax
        jmp    NEAR PTR loc_1_6995
loc_1_6995:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_699A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 4], 1
        jmp    NEAR PTR loc_1_69B2
loc_1_69AC:
        shr    dword ptr [ebp + 8], 1
        inc    byte ptr [ebp - 4]
loc_1_69B2:
        cmp    dword ptr [ebp + 8], 400h
        jb     NEAR PTR loc_1_69C4
        jmp    NEAR PTR loc_1_69AC
loc_1_69C4:
        movzx  eax, byte ptr [ebp - 4]
        cmp    eax, 7
        jle    NEAR PTR loc_1_69D5
        mov    byte ptr [ebp - 4], 7
loc_1_69D5:
        movzx  eax, byte ptr [ebp - 4]
        shl    eax, 0ah
        movzx  ecx, word ptr [ebp + 8]
        or     eax, ecx
        jmp    NEAR PTR loc_1_69E7
loc_1_69E7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_69EC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        movsx  eax, word ptr [ebp + 0ch]
        test   eax, eax
        jle    NEAR PTR loc_1_6A20
        movsx  eax, word ptr [ebp + 0ch]
        imul   eax, eax, 1fh
        sar    eax, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        imul   eax, dword ptr [ebp + 8]
        shr    eax, 0fh
        add    dword ptr [ebp + 8], eax
        jmp    NEAR PTR loc_1_6A4E
loc_1_6A20:
        movsx  eax, word ptr [ebp + 0ch]
        test   eax, eax
        jge    NEAR PTR loc_1_6A4E
        movsx  eax, word ptr [ebp + 0ch]
        imul   eax, eax, 1bh
        neg    eax
        sar    eax, 8
        mov    dword ptr [ebp - 4], eax
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 4]
        imul   ecx, dword ptr [ebp + 8]
        shr    ecx, 0fh
        sub    eax, ecx
        neg    eax
        sub    dword ptr [ebp + 8], eax
loc_1_6A4E:
        mov    eax, dword ptr [ebp + 8]
        jmp    NEAR PTR loc_1_6A56
loc_1_6A56:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_6A5B:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        movzx  eax, byte ptr [ebp + 1ch]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_6B7D
loc_1_6A70:
        movzx  eax, byte ptr [ebp + 18h]
        cmp    eax, 3
        jne    NEAR PTR loc_1_6A86
        mov    byte ptr [ebp - 0ch], 1
        jmp    NEAR PTR loc_1_6A8A
loc_1_6A86:
        mov    byte ptr [ebp - 0ch], 0
loc_1_6A8A:
        jmp    NEAR PTR loc_1_6BB1
loc_1_6A8F:
        movzx  eax, byte ptr [ebp + 18h]
        cmp    eax, 1
        je     NEAR PTR loc_1_6AA9
        movzx  eax, byte ptr [ebp + 18h]
        cmp    eax, 3
        jne    NEAR PTR loc_1_6AB2
loc_1_6AA9:
        mov    byte ptr [ebp - 0ch], 1
        jmp    NEAR PTR loc_1_6AB6
loc_1_6AB2:
        mov    byte ptr [ebp - 0ch], 0
loc_1_6AB6:
        jmp    NEAR PTR loc_1_6BB1
loc_1_6ABB:
        movzx  eax, byte ptr [ebp + 18h]
        test   eax, eax
        je     NEAR PTR loc_1_6AD4
        movzx  eax, byte ptr [ebp + 18h]
        cmp    eax, 3
        jne    NEAR PTR loc_1_6ADD
loc_1_6AD4:
        mov    byte ptr [ebp - 0ch], 1
        jmp    NEAR PTR loc_1_6AE1
loc_1_6ADD:
        mov    byte ptr [ebp - 0ch], 0
loc_1_6AE1:
        jmp    NEAR PTR loc_1_6BB1
loc_1_6AE6:
        movzx  eax, byte ptr [ebp + 18h]
        cmp    eax, 1
        je     NEAR PTR loc_1_6AFC
        mov    byte ptr [ebp - 0ch], 1
        jmp    NEAR PTR loc_1_6B00
loc_1_6AFC:
        mov    byte ptr [ebp - 0ch], 0
loc_1_6B00:
        jmp    NEAR PTR loc_1_6BB1
loc_1_6B05:
        movzx  eax, byte ptr [ebp + 18h]
        cmp    eax, 1
        je     NEAR PTR loc_1_6B1F
        movzx  eax, byte ptr [ebp + 18h]
        cmp    eax, 3
        jne    NEAR PTR loc_1_6B28
loc_1_6B1F:
        mov    byte ptr [ebp - 0ch], 1
        jmp    NEAR PTR loc_1_6B2C
loc_1_6B28:
        mov    byte ptr [ebp - 0ch], 0
loc_1_6B2C:
        jmp    NEAR PTR loc_1_6BB1
loc_1_6B31:
        movzx  eax, byte ptr [ebp + 18h]
        cmp    eax, 1
        jl     NEAR PTR loc_1_6B47
        mov    byte ptr [ebp - 0ch], 1
        jmp    NEAR PTR loc_1_6B4B
loc_1_6B47:
        mov    byte ptr [ebp - 0ch], 0
loc_1_6B4B:
        jmp    NEAR PTR loc_1_6BB1
loc_1_6B50:
        movzx  eax, byte ptr [ebp + 18h]
        cmp    eax, 2
        jg     NEAR PTR loc_1_6B66
        mov    byte ptr [ebp - 0ch], 1
        jmp    NEAR PTR loc_1_6B6A
loc_1_6B66:
        mov    byte ptr [ebp - 0ch], 0
loc_1_6B6A:
        jmp    NEAR PTR loc_1_6BB1
loc_1_6B6F:
        mov    byte ptr [ebp - 0ch], 1
        jmp    NEAR PTR loc_1_6BB1
        db  0E9h, 034h, 000h, 000h, 000h
loc_1_6B7D:
        cmp    dword ptr [ebp - 10h], 7
        ja     NEAR PTR loc_1_6BB1
        mov    eax, dword ptr [ebp - 10h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_6BB1:
        movzx  eax, byte ptr [ebp - 0ch]
        test   eax, eax
        jne    NEAR PTR loc_1_6BC5
        mov    al, byte ptr [ebp + 0ch]
        jmp    NEAR PTR loc_1_6C33
loc_1_6BC5:
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, word ptr [eax + 227h]
        mov    ecx, dword ptr [ebp + 8]
        movzx  ecx, word ptr [ecx + 225h]
        cmp    eax, ecx
        jl     NEAR PTR loc_1_6BE3
        mov    eax, ecx
loc_1_6BE3:
        mov    word ptr [ebp - 4], ax
        movzx  eax, byte ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 1e9h]
        movzx  ecx, byte ptr [ebp + 14h]
        sar    ecx, 2
        db  00Fh, 0B6h, 089h, 000h, 000h, 000h, 000h
        add    eax, ecx
        movzx  ecx, byte ptr [ebp + 0ch]
        add    eax, ecx
        movzx  ecx, word ptr [ebp - 4]
        lea    eax, [eax + ecx*2]
        mov    word ptr [ebp - 8], ax
        movzx  eax, word ptr [ebp - 8]
        cmp    eax, 3fh
        jle    NEAR PTR loc_1_6C2B
        mov    al, 3fh
        jmp    NEAR PTR loc_1_6C2E
loc_1_6C2B:
        mov    al, byte ptr [ebp - 8]
loc_1_6C2E:
        jmp    NEAR PTR loc_1_6C33
loc_1_6C33:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_6C38:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        movzx  eax, byte ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 1e9h]
        mov    ecx, dword ptr [ebp + 8]
        movzx  ecx, word ptr [ecx + 225h]
        lea    eax, [eax + ecx*2]
        mov    word ptr [ebp - 8], ax
        movzx  eax, byte ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 1e9h]
        mov    ecx, dword ptr [ebp + 8]
        movzx  ecx, word ptr [ecx + 227h]
        lea    eax, [eax + ecx*2]
        mov    word ptr [ebp - 4], ax
        movzx  eax, word ptr [ebp - 8]
        cmp    eax, 3fh
        jle    NEAR PTR loc_1_6CA2
        movzx  eax, word ptr [ebp - 4]
        cmp    eax, 3fh
        jle    NEAR PTR loc_1_6CA2
        mov    al, 0cfh
        jmp    NEAR PTR loc_1_6D11
loc_1_6CA2:
        movzx  eax, word ptr [ebp - 8]
        add    eax, 8
        movzx  ecx, word ptr [ebp - 4]
        cmp    eax, ecx
        jge    NEAR PTR loc_1_6CD0
        movzx  eax, byte ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 1f9h]
        and    al, 0efh
        jmp    NEAR PTR loc_1_6D11
        db  0E9h, 041h, 000h, 000h, 000h
loc_1_6CD0:
        movzx  eax, word ptr [ebp - 4]
        add    eax, 8
        movzx  ecx, word ptr [ebp - 8]
        cmp    eax, ecx
        jge    NEAR PTR loc_1_6CFE
        movzx  eax, byte ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 1f9h]
        and    al, 0dfh
        jmp    NEAR PTR loc_1_6D11
        db  0E9h, 013h, 000h, 000h, 000h
loc_1_6CFE:
        movzx  eax, byte ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 1f9h]
        jmp    NEAR PTR loc_1_6D11
loc_1_6D11:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_6D16:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + 30h]
        cmp    eax, 2
        jne    NEAR PTR loc_1_6D3A
        mov    word ptr [ebp - 0ch], 12h
        jmp    NEAR PTR loc_1_6D40
loc_1_6D3A:
        mov    word ptr [ebp - 0ch], 9
loc_1_6D40:
        mov    word ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_6D4F
loc_1_6D4B:
        inc    word ptr [ebp - 10h]
loc_1_6D4F:
        movzx  eax, word ptr [ebp - 10h]
        movzx  ecx, word ptr [ebp - 0ch]
        cmp    eax, ecx
        jge    NEAR PTR loc_1_6D85
        movzx  eax, word ptr [ebp - 10h]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + ecx + 87h], 0
        jne    NEAR PTR loc_1_6D80
        mov    ax, word ptr [ebp - 10h]
        jmp    NEAR PTR loc_1_6F3B
loc_1_6D80:
        jmp    NEAR PTR loc_1_6D4B
loc_1_6D85:
        mov    dword ptr [ebp - 4], 0ffffffffh
        mov    word ptr [ebp - 8], 0ffffh
        mov    word ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_6DA1
loc_1_6D9D:
        inc    word ptr [ebp - 10h]
loc_1_6DA1:
        movzx  eax, word ptr [ebp - 10h]
        movzx  ecx, word ptr [ebp - 0ch]
        cmp    eax, ecx
        jge    NEAR PTR loc_1_6E06
        movzx  eax, word ptr [ebp - 10h]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 84h]
        test   eax, eax
        jne    NEAR PTR loc_1_6E01
        movzx  eax, word ptr [ebp - 10h]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx + 87h], edx
        jae    NEAR PTR loc_1_6E01
        movzx  eax, word ptr [ebp - 10h]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + ecx + 87h]
        mov    dword ptr [ebp - 4], eax
        mov    ax, word ptr [ebp - 10h]
        mov    word ptr [ebp - 8], ax
loc_1_6E01:
        jmp    NEAR PTR loc_1_6D9D
loc_1_6E06:
        movzx  eax, word ptr [ebp - 8]
        cmp    eax, 0ffffh
        je     NEAR PTR loc_1_6E1E
        mov    ax, word ptr [ebp - 8]
        jmp    NEAR PTR loc_1_6F3B
loc_1_6E1E:
        mov    dword ptr [ebp - 4], 0ffffffffh
        mov    word ptr [ebp - 8], 0ffffh
        mov    word ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_6E3A
loc_1_6E36:
        inc    word ptr [ebp - 10h]
loc_1_6E3A:
        movzx  eax, word ptr [ebp - 10h]
        movzx  ecx, word ptr [ebp - 0ch]
        cmp    eax, ecx
        jge    NEAR PTR loc_1_6EA3
        movzx  eax, word ptr [ebp - 10h]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 83h]
        movzx  ecx, byte ptr [ebp + 0ch]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_6E9E
        movzx  eax, word ptr [ebp - 10h]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx + 87h], edx
        jae    NEAR PTR loc_1_6E9E
        movzx  eax, word ptr [ebp - 10h]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + ecx + 87h]
        mov    dword ptr [ebp - 4], eax
        mov    ax, word ptr [ebp - 10h]
        mov    word ptr [ebp - 8], ax
loc_1_6E9E:
        jmp    NEAR PTR loc_1_6E36
loc_1_6EA3:
        movzx  eax, word ptr [ebp - 8]
        cmp    eax, 0ffffh
        je     NEAR PTR loc_1_6EBB
        mov    ax, word ptr [ebp - 8]
        jmp    NEAR PTR loc_1_6F3B
loc_1_6EBB:
        mov    word ptr [ebp - 8], 0
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + ecx + 87h]
        mov    dword ptr [ebp - 4], eax
        movzx  eax, word ptr [ebp - 8]
        inc    eax
        mov    word ptr [ebp - 10h], ax
        jmp    NEAR PTR loc_1_6EE7
loc_1_6EE3:
        inc    word ptr [ebp - 10h]
loc_1_6EE7:
        movzx  eax, word ptr [ebp - 10h]
        movzx  ecx, word ptr [ebp - 0ch]
        cmp    eax, ecx
        jge    NEAR PTR loc_1_6F32
        movzx  eax, word ptr [ebp - 10h]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx + 87h], edx
        jae    NEAR PTR loc_1_6F2D
        movzx  eax, word ptr [ebp - 10h]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + ecx + 87h]
        mov    dword ptr [ebp - 4], eax
        mov    ax, word ptr [ebp - 10h]
        mov    word ptr [ebp - 8], ax
loc_1_6F2D:
        jmp    NEAR PTR loc_1_6EE3
loc_1_6F32:
        mov    ax, word ptr [ebp - 8]
        jmp    NEAR PTR loc_1_6F3B
loc_1_6F3B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_6F40:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_6F58
loc_1_6F54:
        inc    word ptr [ebp - 4]
loc_1_6F58:
        movzx  eax, word ptr [ebp - 4]
        cmp    eax, 4
        jge    NEAR PTR loc_1_6FF4
        movzx  eax, word ptr [ebp - 4]
        imul   eax, eax, 5
        mov    ecx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + ecx]
        movzx  ecx, word ptr [ebp - 4]
        imul   ecx, ecx, 5
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx], al
        movzx  eax, word ptr [ebp - 4]
        imul   eax, eax, 5
        mov    ecx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + ecx + 1]
        movzx  ecx, word ptr [ebp - 4]
        imul   ecx, ecx, 5
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 1], al
        movzx  eax, word ptr [ebp - 4]
        imul   eax, eax, 5
        mov    ecx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + ecx + 2]
        movzx  ecx, word ptr [ebp - 4]
        imul   ecx, ecx, 5
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 2], al
        movzx  eax, word ptr [ebp - 4]
        imul   eax, eax, 5
        mov    ecx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + ecx + 3]
        movzx  ecx, word ptr [ebp - 4]
        imul   ecx, ecx, 5
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 3], al
        movzx  eax, word ptr [ebp - 4]
        imul   eax, eax, 5
        mov    ecx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + ecx + 4]
        movzx  ecx, word ptr [ebp - 4]
        imul   ecx, ecx, 5
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 4], al
        jmp    NEAR PTR loc_1_6F54
loc_1_6FF4:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 14h]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 14h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 15h]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 15h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 16h]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 16h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 17h]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 17h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 18h]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 18h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 19h]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 19h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 1ah]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 1ah], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 1bh]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 1bh], al
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_7059:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        movzx  eax, word ptr [ebp + 0ch]
        cmp    eax, 9
        jl     NEAR PTR loc_1_7081
        movzx  eax, word ptr [ebp + 0ch]
        add    eax, 0f7h
        mov    word ptr [ebp - 4], ax
        jmp    NEAR PTR loc_1_7089
loc_1_7081:
        mov    ax, word ptr [ebp + 0ch]
        mov    word ptr [ebp - 4], ax
loc_1_7089:
        push   0
        movzx  eax, word ptr [ebp - 4]
        add    eax, 0b0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0ch
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_70AF
loc_1_70AB:
        inc    word ptr [ebp - 8]
loc_1_70AF:
        movzx  eax, word ptr [ebp - 8]
        cmp    eax, 2
        jge    NEAR PTR loc_1_716A
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 5
        add    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 0ch], eax
        movzx  eax, word ptr [ebp - 8]
        movzx  ecx, word ptr [ebp + 0ch]
        shl    ecx, 2
        db  066h, 08Bh, 084h, 041h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        add    eax, 20h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 1]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        add    eax, 40h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 2]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        add    eax, 60h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 3]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        add    eax, 80h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 4]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        add    eax, 0e0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0ch
        jmp    NEAR PTR loc_1_70AB
loc_1_716A:
        movzx  eax, word ptr [ebp + 0ch]
        cmp    eax, 9
        jl     NEAR PTR loc_1_7189
        movzx  eax, word ptr [ebp + 0ch]
        add    eax, 0f7h
        mov    word ptr [ebp - 4], ax
        jmp    NEAR PTR loc_1_7191
loc_1_7189:
        mov    ax, word ptr [ebp + 0ch]
        mov    word ptr [ebp - 4], ax
loc_1_7191:
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 14h]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        add    eax, 0a0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 18h]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        add    eax, 0c0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 10h]
        movzx  eax, byte ptr [eax + 16h]
        or     al, 20h
        push   eax
        movzx  eax, word ptr [ebp - 4]
        add    eax, 0b0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_71ED:
        push   ebp
        mov    ebp, esp
        sub    esp, 44h
        push   ebx
        push   esi
        push   edi
        db  0C7h, 045h, 0F4h, 000h, 000h, 000h, 000h
        movzx  eax, byte ptr [ebp + 0ch]
        imul   eax, eax, 1ch
        add    dword ptr [ebp - 0ch], eax
        mov    ecx, 0ch
        movzx  eax, byte ptr [ebp + 10h]
        cdq
        idiv   ecx
        db  08Bh, 004h, 095h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        mov    ecx, 0ch
        movzx  eax, byte ptr [ebp + 10h]
        cdq
        idiv   ecx
        mov    byte ptr [ebp - 44h], al
        movzx  eax, byte ptr [ebp - 44h]
        cmp    eax, 5
        jle    NEAR PTR loc_1_724A
        movzx  eax, byte ptr [ebp - 44h]
        sub    eax, 5
        mov    cl, al
        shl    dword ptr [ebp - 4], cl
        jmp    NEAR PTR loc_1_7267
loc_1_724A:
        movzx  eax, byte ptr [ebp - 44h]
        cmp    eax, 5
        jge    NEAR PTR loc_1_7267
        mov    eax, 5
        movzx  ecx, byte ptr [ebp - 44h]
        sub    eax, ecx
        mov    cl, al
        shr    dword ptr [ebp - 4], cl
loc_1_7267:
        push   dword ptr [ebp - 0ch]
        lea    eax, [ebp - 34h]
        push   eax
        call   sub_1_6F40
        add    esp, 8
        mov    word ptr [ebp - 18h], 0
        jmp    NEAR PTR loc_1_7285
loc_1_7281:
        inc    word ptr [ebp - 18h]
loc_1_7285:
        movzx  eax, word ptr [ebp - 18h]
        cmp    eax, 2
        jge    NEAR PTR loc_1_7334
        mov    eax, dword ptr [ebp - 4]
        movzx  ecx, word ptr [ebp - 18h]
        mov    dword ptr [ebp + ecx*4 - 3ch], eax
        movzx  eax, word ptr [ebp - 18h]
        movzx  eax, byte ptr [ebp + eax - 1eh]
        shr    eax, 2
        and    al, 7
        mov    byte ptr [ebp - 44h], al
        movzx  eax, byte ptr [ebp - 44h]
        cmp    eax, 4
        jle    NEAR PTR loc_1_72D1
        movzx  eax, byte ptr [ebp - 44h]
        sub    eax, 4
        movzx  edx, word ptr [ebp - 18h]
        mov    cl, al
        shl    dword ptr [ebp + edx*4 - 3ch], cl
        jmp    NEAR PTR loc_1_72F3
loc_1_72D1:
        movzx  eax, byte ptr [ebp - 44h]
        cmp    eax, 4
        jge    NEAR PTR loc_1_72F3
        mov    eax, 4
        movzx  ecx, byte ptr [ebp - 44h]
        sub    eax, ecx
        movzx  edx, word ptr [ebp - 18h]
        mov    cl, al
        shr    dword ptr [ebp + edx*4 - 3ch], cl
loc_1_72F3:
        push   dword ptr [ebp + 1ch]
        movzx  eax, word ptr [ebp - 18h]
        push   dword ptr [ebp + eax*4 - 3ch]
        call   sub_1_69EC
        add    esp, 8
        push   eax
        call   sub_1_699A
        add    esp, 4
        mov    word ptr [ebp - 10h], ax
        mov    al, byte ptr [ebp - 10h]
        movzx  ecx, word ptr [ebp - 18h]
        mov    byte ptr [ebp + ecx - 20h], al
        movzx  eax, word ptr [ebp - 10h]
        movzx  eax, ah
        or     al, 20h
        movzx  ecx, word ptr [ebp - 18h]
        mov    byte ptr [ebp + ecx - 1eh], al
        jmp    NEAR PTR loc_1_7281
loc_1_7334:
        movzx  eax, byte ptr [ebp - 1ch]
        and    eax, 1
        add    eax, eax
        add    eax, 4
        mov    byte ptr [ebp - 8], al
        mov    word ptr [ebp - 14h], 0
        jmp    NEAR PTR loc_1_7352
loc_1_734E:
        inc    word ptr [ebp - 14h]
loc_1_7352:
        movzx  eax, word ptr [ebp - 14h]
        cmp    eax, 2
        jge    NEAR PTR loc_1_73B2
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        movzx  eax, word ptr [ebp - 14h]
        imul   eax, eax, 5
        movzx  eax, byte ptr [ebp + eax - 33h]
        and    eax, 3fh
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_6A5B
        add    esp, 18h
        movzx  ax, al
        mov    word ptr [ebp - 10h], ax
        movzx  eax, word ptr [ebp - 14h]
        imul   eax, eax, 5
        movzx  eax, byte ptr [ebp + eax - 33h]
        and    eax, 0c0h
        or     al, byte ptr [ebp - 10h]
        movzx  ecx, word ptr [ebp - 14h]
        imul   ecx, ecx, 5
        mov    byte ptr [ebp + ecx - 33h], al
        jmp    NEAR PTR loc_1_734E
loc_1_73B2:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 8]
        call   sub_1_6C38
        add    esp, 8
        mov    byte ptr [ebp - 40h], al
        movzx  eax, byte ptr [ebp - 1ch]
        movzx  ecx, byte ptr [ebp - 40h]
        and    eax, ecx
        mov    byte ptr [ebp - 1ch], al
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_1_6D16
        add    esp, 8
        mov    word ptr [ebp - 10h], ax
        lea    eax, [ebp - 34h]
        push   eax
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp + 8]
        call   sub_1_7059
        add    esp, 0ch
        mov    al, byte ptr [ebp + 10h]
        movzx  ecx, word ptr [ebp - 10h]
        imul   ecx, ecx, 14h
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 81h], al
        mov    al, byte ptr [ebp + 14h]
        movzx  ecx, word ptr [ebp - 10h]
        imul   ecx, ecx, 14h
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 82h], al
        mov    al, byte ptr [ebp + 0ch]
        movzx  ecx, word ptr [ebp - 10h]
        imul   ecx, ecx, 14h
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 83h], al
        mov    al, byte ptr [ebp + 18h]
        movzx  ecx, word ptr [ebp - 10h]
        imul   ecx, ecx, 14h
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 85h], al
        movzx  eax, word ptr [ebp - 10h]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 84h], 1
        movzx  eax, word ptr [ebp - 10h]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 86h], 0
        db  0A1h, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [ebp - 10h]
        imul   ecx, ecx, 14h
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + edx + 87h], eax
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 3ch]
        movzx  ecx, word ptr [ebp - 10h]
        imul   ecx, ecx, 14h
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + edx + 8bh], eax
        mov    eax, dword ptr [ebp - 38h]
        movzx  ecx, word ptr [ebp - 10h]
        imul   ecx, ecx, 14h
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + edx + 8fh], eax
        mov    al, byte ptr [ebp - 1eh]
        movzx  ecx, word ptr [ebp - 10h]
        imul   ecx, ecx, 14h
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 93h], al
        mov    al, byte ptr [ebp - 1dh]
        movzx  ecx, word ptr [ebp - 10h]
        imul   ecx, ecx, 14h
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 94h], al
        xor    eax, eax
        jmp    NEAR PTR loc_1_74DB
loc_1_74DB:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_74E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + 30h]
        cmp    eax, 2
        jne    NEAR PTR loc_1_7504
        mov    word ptr [ebp - 4], 12h
        jmp    NEAR PTR loc_1_750A
loc_1_7504:
        mov    word ptr [ebp - 4], 9
loc_1_750A:
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_7519
loc_1_7515:
        inc    word ptr [ebp - 8]
loc_1_7519:
        movzx  eax, word ptr [ebp - 4]
        movzx  ecx, word ptr [ebp - 8]
        cmp    eax, ecx
        jle    NEAR PTR loc_1_758D
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 82h]
        movzx  ecx, byte ptr [ebp + 10h]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_7588
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 81h]
        movzx  ecx, byte ptr [ebp + 0ch]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_7588
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 84h]
        test   eax, eax
        je     NEAR PTR loc_1_7588
        mov    ax, word ptr [ebp - 8]
        jmp    NEAR PTR loc_1_7596
loc_1_7588:
        jmp    NEAR PTR loc_1_7515
loc_1_758D:
        mov    ax, 0ffffh
        jmp    NEAR PTR loc_1_7596
loc_1_7596:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_759B:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        db  0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 8]
        call   sub_1_74E0
        add    esp, 0ch
        mov    word ptr [ebp - 8], ax
        movzx  eax, word ptr [ebp - 8]
        cmp    eax, 0ffffh
        je     NEAR PTR loc_1_76E0
        movzx  eax, byte ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 209h]
        test   eax, eax
        je     NEAR PTR loc_1_75FF
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 86h], 1
        xor    eax, eax
        jmp    NEAR PTR loc_1_76E7
loc_1_75FF:
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 83h]
        imul   eax, eax, 1ch
        add    dword ptr [ebp - 4], eax
        movzx  eax, word ptr [ebp - 8]
        cmp    eax, 9
        jl     NEAR PTR loc_1_7636
        movzx  eax, word ptr [ebp - 8]
        add    eax, 0f7h
        mov    word ptr [ebp - 0ch], ax
        jmp    NEAR PTR loc_1_763E
loc_1_7636:
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [ebp - 0ch], ax
loc_1_763E:
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 93h]
        and    eax, 1fh
        push   eax
        movzx  eax, word ptr [ebp - 0ch]
        add    eax, 0b0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0ch
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 84h], 0
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 93h]
        and    al, 1fh
        movzx  ecx, word ptr [ebp - 8]
        imul   ecx, ecx, 14h
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 93h], al
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 94h]
        and    al, 1fh
        movzx  ecx, word ptr [ebp - 8]
        imul   ecx, ecx, 14h
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 94h], al
        db  0A1h, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [ebp - 8]
        imul   ecx, ecx, 14h
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + edx + 87h], eax
        jmp    NEAR PTR loc_1_76E0
loc_1_76E0:
        xor    eax, eax
        jmp    NEAR PTR loc_1_76E7
loc_1_76E7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_76EC:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + 30h]
        cmp    eax, 2
        jne    NEAR PTR loc_1_77CB
        movzx  eax, word ptr [ebp + 0ch]
        cmp    eax, 100h
        jge    NEAR PTR loc_1_7727
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 8], ax
        jmp    NEAR PTR loc_1_7738
loc_1_7727:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        movzx  eax, word ptr [eax + 4]
        add    eax, 2
        mov    word ptr [ebp - 8], ax
loc_1_7738:
        mov    word ptr [ebp - 4], 1
        mov    al, byte ptr [ebp + 0ch]
        mov    byte ptr [ebp - 10h], al
        mov    dx, word ptr [ebp - 8]
        mov    al, byte ptr [ebp - 10h]
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 8], ax
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_7769
loc_1_7766:
        inc    dword ptr [ebp - 0ch]
loc_1_7769:
        cmp    dword ptr [ebp - 0ch], 19h
        jae    NEAR PTR loc_1_777E
        mov    dx, word ptr [ebp - 8]
        in     ax, dx
        jmp    NEAR PTR loc_1_7766
loc_1_777E:
        movzx  eax, word ptr [ebp - 4]
        movzx  ecx, word ptr [ebp - 8]
        add    eax, ecx
        mov    word ptr [ebp - 8], ax
        mov    dx, word ptr [ebp - 8]
        mov    al, byte ptr [ebp + 10h]
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 8], ax
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_77B1
loc_1_77AE:
        inc    dword ptr [ebp - 0ch]
loc_1_77B1:
        cmp    dword ptr [ebp - 0ch], 19h
        jae    NEAR PTR loc_1_77C6
        mov    dx, word ptr [ebp - 8]
        in     ax, dx
        jmp    NEAR PTR loc_1_77AE
loc_1_77C6:
        jmp    NEAR PTR loc_1_77CB
loc_1_77CB:
        xor    eax, eax
        jmp    NEAR PTR loc_1_77D2
loc_1_77D2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_77D7:
        push   ebp
        mov    ebp, esp
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 28h], 0
        mov    word ptr [ebp - 26h], 0
        mov    word ptr [ebp - 24h], 0
        mov    word ptr [ebp - 22h], 1
        mov    word ptr [ebp - 20h], 1
        mov    word ptr [ebp - 1eh], 1
        mov    word ptr [ebp - 1ch], 2
        mov    word ptr [ebp - 1ah], 2
        mov    word ptr [ebp - 18h], 2
        mov    word ptr [ebp - 16h], 2
        mov    word ptr [ebp - 14h], 3
        mov    word ptr [ebp - 12h], 3
        mov    word ptr [ebp - 10h], 3
        mov    word ptr [ebp - 0eh], 3
        mov    word ptr [ebp - 0ch], 3
        mov    word ptr [ebp - 0ah], 3
        movzx  eax, word ptr [ebp + 8]
        cmp    eax, 0ffffh
        jne    NEAR PTR loc_1_7856
        xor    al, al
        jmp    NEAR PTR loc_1_794C
loc_1_7856:
        movzx  eax, word ptr [ebp + 8]
        test   eax, eax
        jne    NEAR PTR loc_1_7869
        mov    al, 0c0h
        jmp    NEAR PTR loc_1_794C
loc_1_7869:
        mov    ax, word ptr [ebp + 8]
        mov    word ptr [ebp - 4], ax
        mov    word ptr [ebp - 30h], 0
        mov    word ptr [ebp - 2ch], 0
        jmp    NEAR PTR loc_1_7895
loc_1_7882:
        movzx  eax, word ptr [ebp - 30h]
        add    eax, 4
        mov    word ptr [ebp - 30h], ax
        shr    word ptr [ebp - 4], 1
        inc    word ptr [ebp - 2ch]
loc_1_7895:
        movzx  eax, word ptr [ebp - 4]
        test   eax, eax
        je     NEAR PTR loc_1_78A6
        jmp    NEAR PTR loc_1_7882
loc_1_78A6:
        movzx  eax, word ptr [ebp - 2ch]
        cmp    eax, 5
        jle    NEAR PTR loc_1_78C9
        movzx  eax, word ptr [ebp + 8]
        movzx  ecx, word ptr [ebp - 2ch]
        sub    ecx, 5
        sar    eax, cl
        mov    word ptr [ebp - 4], ax
        jmp    NEAR PTR loc_1_78F8
loc_1_78C9:
        movzx  eax, word ptr [ebp - 2ch]
        cmp    eax, 5
        jge    NEAR PTR loc_1_78F0
        movzx  eax, word ptr [ebp + 8]
        mov    ecx, 5
        movzx  edx, word ptr [ebp - 2ch]
        sub    ecx, edx
        shl    eax, cl
        mov    word ptr [ebp - 4], ax
        jmp    NEAR PTR loc_1_78F8
loc_1_78F0:
        mov    ax, word ptr [ebp + 8]
        mov    word ptr [ebp - 4], ax
loc_1_78F8:
        movzx  eax, word ptr [ebp - 4]
        and    eax, 0fh
        mov    word ptr [ebp - 4], ax
        movzx  eax, word ptr [ebp - 4]
        movzx  eax, word ptr [ebp + eax*2 - 28h]
        movzx  ecx, word ptr [ebp - 30h]
        add    eax, ecx
        mov    word ptr [ebp - 30h], ax
        mov    eax, 43h
        movzx  ecx, byte ptr [ebp - 30h]
        sub    eax, ecx
        mov    byte ptr [ebp - 8], al
        movzx  eax, byte ptr [ebp - 8]
        xor    ecx, ecx
        cmp    eax, 80h
        setl   cl
        movzx  eax, cl
        test   eax, eax
        je     NEAR PTR loc_1_7945
        mov    al, byte ptr [ebp - 8]
        jmp    NEAR PTR loc_1_7947
loc_1_7945:
        mov    al, 7fh
loc_1_7947:
        jmp    NEAR PTR loc_1_794C
loc_1_794C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_7951:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        db  0C7h, 045h, 0E4h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 221h], 0
        je     NEAR PTR loc_1_79F0
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax + 219h]
        call   sub_1_77D7
        add    esp, 4
        movzx  ebx, al
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax + 21dh]
        call   sub_1_77D7
        add    esp, 4
        movzx  eax, al
        add    ebx, eax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 225h], bx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 21dh]
        shr    eax, 10h
        and    eax, 0ffffh
        push   eax
        call   sub_1_77D7
        add    esp, 4
        movzx  ebx, al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 219h]
        shr    eax, 10h
        and    eax, 0ffffh
        push   eax
        call   sub_1_77D7
        add    esp, 4
        movzx  eax, al
        add    ebx, eax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 227h], bx
        jmp    NEAR PTR loc_1_7A08
loc_1_79F0:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 225h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 227h], 0
loc_1_7A08:
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, word ptr [eax + 225h]
        add    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 225h], ax
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, word ptr [eax + 225h]
        cmp    eax, 0c0h
        jle    NEAR PTR loc_1_7A3F
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 225h], 0c0h
loc_1_7A3F:
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, word ptr [eax + 227h]
        add    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 227h], ax
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, word ptr [eax + 227h]
        cmp    eax, 0c0h
        jle    NEAR PTR loc_1_7A76
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 227h], 0c0h
loc_1_7A76:
        mov    word ptr [ebp - 14h], 0
        jmp    NEAR PTR loc_1_7A85
loc_1_7A81:
        inc    word ptr [ebp - 14h]
loc_1_7A85:
        movzx  eax, word ptr [ebp - 14h]
        cmp    eax, 12h
        jge    NEAR PTR loc_1_7C10
        movzx  eax, word ptr [ebp - 14h]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 82h]
        movzx  ecx, byte ptr [ebp + 0ch]
        cmp    eax, ecx
        je     NEAR PTR loc_1_7ABF
        movzx  eax, byte ptr [ebp + 0ch]
        cmp    eax, 0ffh
        jne    NEAR PTR loc_1_7C0B
loc_1_7ABF:
        movzx  eax, word ptr [ebp - 14h]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 83h]
        imul   eax, eax, 1ch
        add    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        movzx  eax, byte ptr [eax + 18h]
        and    eax, 1
        add    eax, eax
        add    eax, 4
        mov    byte ptr [ebp - 4], al
        mov    word ptr [ebp - 18h], 0
        jmp    NEAR PTR loc_1_7AFB
loc_1_7AF7:
        inc    word ptr [ebp - 18h]
loc_1_7AFB:
        movzx  eax, word ptr [ebp - 18h]
        cmp    eax, 2
        jge    NEAR PTR loc_1_7BA1
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 18h]
        movzx  eax, word ptr [ebp - 14h]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 85h]
        push   eax
        movzx  eax, word ptr [ebp - 14h]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 82h]
        push   eax
        movzx  eax, word ptr [ebp - 18h]
        imul   eax, eax, 5
        mov    ecx, dword ptr [ebp - 8]
        movzx  eax, byte ptr [eax + ecx + 1]
        and    eax, 3fh
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_6A5B
        add    esp, 18h
        movzx  ax, al
        mov    word ptr [ebp - 10h], ax
        movzx  eax, word ptr [ebp - 14h]
        movzx  ecx, word ptr [ebp - 18h]
        add    ecx, ecx
        db  066h, 08Bh, 084h, 081h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 0ch], ax
        movzx  eax, word ptr [ebp - 18h]
        imul   eax, eax, 5
        mov    ecx, dword ptr [ebp - 8]
        movzx  eax, byte ptr [eax + ecx + 1]
        and    eax, 0c0h
        movzx  ecx, byte ptr [ebp - 10h]
        or     eax, ecx
        push   eax
        movzx  eax, word ptr [ebp - 0ch]
        add    eax, 40h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0ch
        jmp    NEAR PTR loc_1_7AF7
loc_1_7BA1:
        movzx  eax, word ptr [ebp - 14h]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 82h]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_6C38
        add    esp, 8
        mov    byte ptr [ebp - 20h], al
        movzx  eax, word ptr [ebp - 14h]
        cmp    eax, 9
        jl     NEAR PTR loc_1_7BE0
        movzx  eax, word ptr [ebp - 14h]
        add    eax, 0f7h
        mov    word ptr [ebp - 0ch], ax
        jmp    NEAR PTR loc_1_7BE8
loc_1_7BE0:
        mov    ax, word ptr [ebp - 14h]
        mov    word ptr [ebp - 0ch], ax
loc_1_7BE8:
        mov    eax, dword ptr [ebp - 8]
        movzx  eax, byte ptr [eax + 18h]
        movzx  ecx, byte ptr [ebp - 20h]
        and    eax, ecx
        push   eax
        movzx  eax, word ptr [ebp - 0ch]
        add    eax, 0c0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0ch
loc_1_7C0B:
        jmp    NEAR PTR loc_1_7A81
loc_1_7C10:
        xor    eax, eax
        jmp    NEAR PTR loc_1_7C17
loc_1_7C17:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_7C1C:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        movzx  ecx, byte ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 31h], eax
        mov    word ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_7C42
loc_1_7C3E:
        inc    word ptr [ebp - 0ch]
loc_1_7C42:
        movzx  eax, word ptr [ebp - 0ch]
        cmp    eax, 12h
        jge    NEAR PTR loc_1_7D57
        movzx  eax, word ptr [ebp - 0ch]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 82h]
        movzx  ecx, byte ptr [ebp + 0ch]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_7D52
        mov    word ptr [ebp - 10h], 0
        push   dword ptr [ebp + 10h]
        movzx  eax, word ptr [ebp - 10h]
        movzx  ecx, word ptr [ebp - 0ch]
        imul   ecx, ecx, 14h
        lea    eax, [ecx + eax*4]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [eax + ecx + 8bh]
        call   sub_1_69EC
        add    esp, 8
        mov    dword ptr [ebp - 14h], eax
        push   dword ptr [ebp - 14h]
        call   sub_1_699A
        add    esp, 4
        movzx  ecx, word ptr [ebp - 10h]
        mov    word ptr [ebp + ecx*2 - 8], ax
        movzx  eax, word ptr [ebp - 0ch]
        imul   eax, eax, 14h
        movzx  ecx, word ptr [ebp - 10h]
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 93h]
        and    al, 0e0h
        movzx  eax, al
        movzx  ecx, word ptr [ebp - 10h]
        movzx  ecx, word ptr [ebp + ecx*2 - 8]
        or     al, ch
        movzx  ecx, word ptr [ebp - 0ch]
        imul   ecx, ecx, 14h
        movzx  edx, word ptr [ebp - 10h]
        add    ecx, edx
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 93h], al
        movzx  eax, word ptr [ebp - 0ch]
        cmp    eax, 9
        jl     NEAR PTR loc_1_7D0B
        movzx  eax, word ptr [ebp - 0ch]
        add    eax, 0f7h
        mov    word ptr [ebp - 4], ax
        jmp    NEAR PTR loc_1_7D13
loc_1_7D0B:
        mov    ax, word ptr [ebp - 0ch]
        mov    word ptr [ebp - 4], ax
loc_1_7D13:
        movzx  eax, word ptr [ebp - 0ch]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 93h]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        add    eax, 0b0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0ch
        push   dword ptr [ebp - 8]
        movzx  eax, word ptr [ebp - 4]
        add    eax, 0a0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0ch
loc_1_7D52:
        jmp    NEAR PTR loc_1_7C3E
loc_1_7D57:
        xor    eax, eax
        jmp    NEAR PTR loc_1_7D5E
loc_1_7D5E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_7D63:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        movzx  eax, byte ptr [ebp + 10h]
        cmp    eax, 40h
        jge    NEAR PTR loc_1_7F19
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + 30h]
        cmp    eax, 2
        jne    NEAR PTR loc_1_7D94
        mov    word ptr [ebp - 4], 12h
        jmp    NEAR PTR loc_1_7D9A
loc_1_7D94:
        mov    word ptr [ebp - 4], 9
loc_1_7D9A:
        movzx  eax, byte ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 209h], 0
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_7DB8
loc_1_7DB4:
        inc    word ptr [ebp - 8]
loc_1_7DB8:
        movzx  eax, word ptr [ebp - 4]
        movzx  ecx, word ptr [ebp - 8]
        cmp    eax, ecx
        jle    NEAR PTR loc_1_7F14
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 82h]
        movzx  ecx, byte ptr [ebp + 0ch]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_7F0F
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 84h]
        test   eax, eax
        je     NEAR PTR loc_1_7F0F
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 86h]
        test   eax, eax
        je     NEAR PTR loc_1_7F0F
        db  0C7h, 045h, 0F4h, 000h, 000h, 000h, 000h
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 83h]
        imul   eax, eax, 1ch
        add    dword ptr [ebp - 0ch], eax
        movzx  eax, word ptr [ebp - 8]
        cmp    eax, 9
        jl     NEAR PTR loc_1_7E58
        movzx  eax, word ptr [ebp - 8]
        add    eax, 0f7h
        mov    word ptr [ebp - 10h], ax
        jmp    NEAR PTR loc_1_7E60
loc_1_7E58:
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [ebp - 10h], ax
loc_1_7E60:
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 93h]
        and    eax, 1fh
        push   eax
        movzx  eax, word ptr [ebp - 10h]
        add    eax, 0b0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0ch
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 84h], 0
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 86h], 0
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 93h]
        and    al, 1fh
        movzx  ecx, word ptr [ebp - 8]
        imul   ecx, ecx, 14h
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 93h], al
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 94h]
        and    al, 1fh
        movzx  ecx, word ptr [ebp - 8]
        imul   ecx, ecx, 14h
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 94h], al
        db  0A1h, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [ebp - 8]
        imul   ecx, ecx, 14h
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + edx + 87h], eax
loc_1_7F0F:
        jmp    NEAR PTR loc_1_7DB4
loc_1_7F14:
        jmp    NEAR PTR loc_1_7F28
loc_1_7F19:
        movzx  eax, byte ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 209h], 1
loc_1_7F28:
        xor    eax, eax
        jmp    NEAR PTR loc_1_7F2F
loc_1_7F2F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_7F34:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + 30h]
        cmp    eax, 2
        jne    NEAR PTR loc_1_7F58
        mov    word ptr [ebp - 4], 12h
        jmp    NEAR PTR loc_1_7F5E
loc_1_7F58:
        mov    word ptr [ebp - 4], 9
loc_1_7F5E:
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_7F6D
loc_1_7F69:
        inc    word ptr [ebp - 8]
loc_1_7F6D:
        movzx  eax, word ptr [ebp - 4]
        movzx  ecx, word ptr [ebp - 8]
        cmp    eax, ecx
        jle    NEAR PTR loc_1_80DE
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 82h]
        movzx  ecx, byte ptr [ebp + 0ch]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_80D9
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 84h]
        test   eax, eax
        je     NEAR PTR loc_1_80D9
        movzx  eax, byte ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 209h]
        cmp    eax, 1
        jne    NEAR PTR loc_1_7FE4
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 86h], 1
        jmp    NEAR PTR loc_1_7F69
loc_1_7FE4:
        db  0C7h, 045h, 0F4h, 000h, 000h, 000h, 000h
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 83h]
        imul   eax, eax, 1ch
        add    dword ptr [ebp - 0ch], eax
        movzx  eax, word ptr [ebp - 8]
        cmp    eax, 9
        jl     NEAR PTR loc_1_8022
        movzx  eax, word ptr [ebp - 8]
        add    eax, 0f7h
        mov    word ptr [ebp - 10h], ax
        jmp    NEAR PTR loc_1_802A
loc_1_8022:
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [ebp - 10h], ax
loc_1_802A:
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 93h]
        and    eax, 1fh
        push   eax
        movzx  eax, word ptr [ebp - 10h]
        add    eax, 0b0h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_76EC
        add    esp, 0ch
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 84h], 0
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 86h], 0
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 93h]
        and    al, 1fh
        movzx  ecx, word ptr [ebp - 8]
        imul   ecx, ecx, 14h
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 93h], al
        movzx  eax, word ptr [ebp - 8]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 94h]
        and    al, 1fh
        movzx  ecx, word ptr [ebp - 8]
        imul   ecx, ecx, 14h
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 94h], al
        db  0A1h, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [ebp - 8]
        imul   ecx, ecx, 14h
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + edx + 87h], eax
loc_1_80D9:
        jmp    NEAR PTR loc_1_7F69
loc_1_80DE:
        xor    eax, eax
        jmp    NEAR PTR loc_1_80E5
loc_1_80E5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_80EA:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        and    al, 0f0h
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp - 18h]
        and    al, 0fh
        mov    byte ptr [ebp - 10h], al
        mov    eax, dword ptr [ebp - 18h]
        shr    eax, 8
        and    al, 7fh
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 18h]
        shr    eax, 10h
        and    al, 7fh
        mov    byte ptr [ebp - 0ch], al
        movzx  eax, byte ptr [ebp - 8]
        mov    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_1_84AA
loc_1_812D:
        movzx  eax, byte ptr [ebp - 0ch]
        test   eax, eax
        je     NEAR PTR loc_1_81D4
        movzx  eax, byte ptr [ebp - 10h]
        cmp    eax, 9
        jne    NEAR PTR loc_1_81A4
        movzx  eax, byte ptr [ebp - 4]
        cmp    eax, 23h
        jl     NEAR PTR loc_1_819F
        movzx  eax, byte ptr [ebp - 4]
        cmp    eax, 57h
        jg     NEAR PTR loc_1_819F
        movzx  eax, byte ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ecx + eax*4 + 31h]
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 10h]
        movzx  eax, byte ptr [ebp - 4]
        db  08Ah, 004h, 045h, 0BBh, 0FFh, 0FFh, 0FFh
        push   eax
        movzx  eax, byte ptr [ebp - 4]
        db  00Fh, 0B6h, 004h, 045h, 0BAh, 0FFh, 0FFh, 0FFh
        add    eax, 80h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_71ED
        add    esp, 18h
        jmp    NEAR PTR loc_1_84F0
loc_1_819F:
        jmp    NEAR PTR loc_1_81D4
loc_1_81A4:
        movzx  eax, byte ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ecx + eax*4 + 31h]
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 4]
        movzx  eax, byte ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 71h]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_71ED
        add    esp, 18h
        jmp    NEAR PTR loc_1_84F0
loc_1_81D4:
        movzx  eax, byte ptr [ebp - 10h]
        cmp    eax, 9
        jne    NEAR PTR loc_1_8231
        movzx  eax, byte ptr [ebp - 4]
        cmp    eax, 23h
        jl     NEAR PTR loc_1_822C
        movzx  eax, byte ptr [ebp - 4]
        cmp    eax, 57h
        jg     NEAR PTR loc_1_822C
        push   dword ptr [ebp - 10h]
        movzx  eax, byte ptr [ebp - 4]
        db  08Ah, 004h, 045h, 0BBh, 0FFh, 0FFh, 0FFh
        push   eax
        movzx  eax, byte ptr [ebp - 4]
        db  00Fh, 0B6h, 004h, 045h, 0BAh, 0FFh, 0FFh, 0FFh
        add    eax, 80h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_759B
        add    esp, 10h
        jmp    NEAR PTR loc_1_84F0
loc_1_822C:
        jmp    NEAR PTR loc_1_8253
loc_1_8231:
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 4]
        movzx  eax, byte ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 71h]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_759B
        add    esp, 10h
        jmp    NEAR PTR loc_1_84F0
loc_1_8253:
        xor    eax, eax
        jmp    NEAR PTR loc_1_84F0
loc_1_825A:
        movzx  eax, byte ptr [ebp - 4]
        mov    dword ptr [ebp - 24h], eax
        jmp    NEAR PTR loc_1_83D6
loc_1_8266:
        movzx  eax, byte ptr [ebp - 0ch]
        and    eax, 7ch
        shr    eax, 2
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        movzx  ecx, byte ptr [ebp - 10h]
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 1e9h], al
        jmp    NEAR PTR loc_1_8436
loc_1_8289:
        movzx  eax, byte ptr [ebp - 0ch]
        cmp    eax, 50h
        jle    NEAR PTR loc_1_82AA
        movzx  eax, byte ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 1f9h], 0efh
        jmp    NEAR PTR loc_1_82DA
loc_1_82AA:
        movzx  eax, byte ptr [ebp - 0ch]
        cmp    eax, 30h
        jge    NEAR PTR loc_1_82CB
        movzx  eax, byte ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 1f9h], 0dfh
        jmp    NEAR PTR loc_1_82DA
loc_1_82CB:
        movzx  eax, byte ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 1f9h], 0ffh
loc_1_82DA:
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp + 8]
        call   sub_1_7951
        add    esp, 8
        jmp    NEAR PTR loc_1_8436
loc_1_82ED:
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp + 8]
        call   sub_1_7D63
        add    esp, 0ch
        jmp    NEAR PTR loc_1_84F0
loc_1_8303:
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp + 8]
        call   sub_1_7F34
        add    esp, 8
        jmp    NEAR PTR loc_1_84F0
loc_1_8316:
        push   0
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp + 8]
        call   sub_1_7D63
        add    esp, 0ch
        movzx  eax, byte ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + eax*4 + 31h], 0ffffh
        movzx  eax, byte ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ecx + eax*4 + 31h]
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp + 8]
        call   sub_1_7C1C
        add    esp, 0ch
        jmp    NEAR PTR loc_1_8436
loc_1_8353:
        mov    dword ptr [ebp - 1ch], 0
        jmp    NEAR PTR loc_1_8362
loc_1_835F:
        inc    dword ptr [ebp - 1ch]
loc_1_8362:
        cmp    dword ptr [ebp - 1ch], 12h
        jae    NEAR PTR loc_1_83CC
        mov    eax, dword ptr [ebp - 1ch]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + ecx + 82h]
        movzx  ecx, byte ptr [ebp - 10h]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_83C7
        mov    eax, dword ptr [ebp - 1ch]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 82h]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 81h]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        imul   eax, eax, 14h
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 83h]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_759B
        add    esp, 10h
loc_1_83C7:
        jmp    NEAR PTR loc_1_835F
loc_1_83CC:
        jmp    NEAR PTR loc_1_8436
        db  0E9h, 060h, 000h, 000h, 000h
loc_1_83D6:
        cmp    dword ptr [ebp - 24h], 40h
        jg     NEAR PTR loc_1_8409
        je     NEAR PTR loc_1_82ED
        cmp    dword ptr [ebp - 24h], 7
        je     NEAR PTR loc_1_8266
        cmp    dword ptr [ebp - 24h], 8
        je     NEAR PTR loc_1_8289
        cmp    dword ptr [ebp - 24h], 0ah
        je     NEAR PTR loc_1_8289
        jmp    NEAR PTR loc_1_8436
loc_1_8409:
        cmp    dword ptr [ebp - 24h], 78h
        je     NEAR PTR loc_1_8303
        cmp    dword ptr [ebp - 24h], 79h
        je     NEAR PTR loc_1_8316
        cmp    dword ptr [ebp - 24h], 7bh
        jl     NEAR PTR loc_1_8436
        cmp    dword ptr [ebp - 24h], 7fh
        jle    NEAR PTR loc_1_8353
        jmp    NEAR PTR loc_1_8436
loc_1_8436:
        xor    eax, eax
        jmp    NEAR PTR loc_1_84F0
loc_1_843D:
        mov    al, byte ptr [ebp - 4]
        movzx  ecx, byte ptr [ebp - 10h]
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 71h], al
        xor    eax, eax
        jmp    NEAR PTR loc_1_84F0
loc_1_8452:
        movzx  eax, byte ptr [ebp - 4]
        shl    eax, 2
        movzx  ecx, byte ptr [ebp - 0ch]
        shl    ecx, 9
        or     eax, ecx
        mov    word ptr [ebp - 14h], ax
        movzx  eax, word ptr [ebp - 14h]
        add    eax, 7fffh
        movsx  eax, ax
        movzx  ecx, byte ptr [ebp - 10h]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 31h], eax
        movzx  eax, byte ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ecx + eax*4 + 31h]
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp + 8]
        call   sub_1_7C1C
        add    esp, 0ch
        jmp    NEAR PTR loc_1_84F0
loc_1_849B:
        mov    eax, 1
        jmp    NEAR PTR loc_1_84F0
        db  0E9h, 046h, 000h, 000h, 000h
loc_1_84AA:
        cmp    dword ptr [ebp - 20h], 80h
        je     NEAR PTR loc_1_81D4
        cmp    dword ptr [ebp - 20h], 90h
        je     NEAR PTR loc_1_812D
        cmp    dword ptr [ebp - 20h], 0b0h
        je     NEAR PTR loc_1_825A
        cmp    dword ptr [ebp - 20h], 0c0h
        je     NEAR PTR loc_1_843D
        cmp    dword ptr [ebp - 20h], 0e0h
        je     NEAR PTR loc_1_8452
        jmp    NEAR PTR loc_1_849B
loc_1_84F0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_84F5:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], 2
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 4], 41eh
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 26h], 4
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 28h], 12h
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 2ch], 0ffffh
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 2eh], 1
        mov    dword ptr [ebp - 4], 2
        cmp    dword ptr [ebp - 4], 1
        jne    NEAR PTR loc_1_8586
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 2], 12dh
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 2ah], 9
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        movzx  eax, word ptr [eax + 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        add    eax, 6
        push   eax
        call   sub_1_2403
        add    esp, 10h
        jmp    NEAR PTR loc_1_85BC
loc_1_8586:
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 2], 12eh
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 2ah], 12h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        movzx  eax, word ptr [eax + 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        add    eax, 6
        push   eax
        call   sub_1_2403
        add    esp, 10h
loc_1_85BC:
        xor    eax, eax
        jmp    NEAR PTR loc_1_85C3
loc_1_85C3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_85C8:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 24h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 1eh], eax
        mov    dword ptr [ebp - 22h], 30h
        push   dword ptr [ebp + 8]
        call   sub_1_305D
        add    esp, 4
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_1_8606
        mov    eax, 1
        jmp    NEAR PTR loc_1_8651
loc_1_8606:
        lea    eax, [ebp - 10h]
        push   eax
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp - 14h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_8615:
        add    esp, 0ch
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_1_862D
        mov    eax, dword ptr [ebp - 18h]
        jmp    NEAR PTR loc_1_8651
loc_1_862D:
        lea    eax, [ebp - 24h]
        push   eax
        push   dword ptr [ebp - 14h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_8639:
        add    esp, 8
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp - 14h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_8647:
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_1_8651
loc_1_8651:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_8656:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 4], eax
        movzx  eax, word ptr [ebp + 0ch]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_8777
loc_1_8671:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp - 4]
        call   sub_1_63E1
        add    esp, 0ch
        jmp    NEAR PTR loc_1_87C1
loc_1_8687:
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp - 4]
        call   sub_1_6531
        add    esp, 8
        jmp    NEAR PTR loc_1_87C1
loc_1_869A:
        push   dword ptr [ebp + 14h]
        push   10h
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 2ch]
        call   sub_1_3846
        add    esp, 0ch
        xor    eax, eax
        jmp    NEAR PTR loc_1_87C1
loc_1_86B4:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp - 4]
        call   sub_1_84F5
        add    esp, 8
        jmp    NEAR PTR loc_1_87C1
loc_1_86C7:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp - 4]
        call   sub_1_80EA
        add    esp, 8
        jmp    NEAR PTR loc_1_87C1
loc_1_86DA:
        xor    eax, eax
        jmp    NEAR PTR loc_1_87C1
loc_1_86E1:
        xor    eax, eax
        jmp    NEAR PTR loc_1_87C1
loc_1_86E8:
        mov    eax, dword ptr [ebp + 14h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 221h], 0
        je     NEAR PTR loc_1_8726
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 21dh], eax
        push   0ffh
        push   dword ptr [ebp - 4]
        call   sub_1_7951
        add    esp, 8
        jmp    NEAR PTR loc_1_87C1
        db  0E9h, 016h, 000h, 000h, 000h
loc_1_8726:
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp - 4]
        call   sub_1_85C8
        add    esp, 0ch
        jmp    NEAR PTR loc_1_87C1
loc_1_873C:
        xor    eax, eax
        jmp    NEAR PTR loc_1_87C1
loc_1_8743:
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [eax], 41eh
        xor    eax, eax
        jmp    NEAR PTR loc_1_87C1
loc_1_8753:
        push   dword ptr [ebp - 4]
        call   sub_1_68A3
        add    esp, 4
        jmp    NEAR PTR loc_1_87C1
loc_1_8763:
        mov    eax, 1
        jmp    NEAR PTR loc_1_87C1
        db  0E9h, 048h, 000h, 000h, 000h, 0E9h, 043h, 000h, 000h, 000h
loc_1_8777:
        dec    dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 0ch], 0ah
        ja     NEAR PTR loc_1_8763
        mov    eax, dword ptr [ebp - 0ch]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 033h, 0C0h, 0E9h, 000h
        db  000h, 000h, 000h
loc_1_87C1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_87C6:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 14h]
        mov    edi, dword ptr [ebp + 10h]
        xor    esi, esi
        mov    ecx, dword ptr [edi]
        test   cl, 1
        jne    SHORT loc_1_87E8
        mov    eax, 3
        jmp    NEAR PTR loc_1_8A77
loc_1_87E8:
        movzx  eax, word ptr [ebp + 0ch]
        cmp    eax, 1
        je     NEAR PTR loc_1_8A0E
        cmp    eax, 2
        je     NEAR PTR loc_1_8A48
        test   cl, 2
        jne    SHORT loc_1_880D
        mov    eax, 5
        jmp    NEAR PTR loc_1_8A77
loc_1_880D:
        sub    eax, 3
        cmp    eax, 0ah
        ja     SHORT loc_1_881C
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_881C:
        mov    eax, 0ch
        jmp    NEAR PTR loc_1_8A77
loc_1_8826:
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_9B4D
        add    esp, 4
        push   edi
        call   sub_1_5B2A
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_8849
        mov    esi, 1
        jmp    NEAR PTR loc_1_8A0A
loc_1_8849:
        cmp    word ptr [ebp + 0ch], 3
        jne    SHORT loc_1_8857
        push   0
        or     byte ptr [edi], 4
        jmp    SHORT loc_1_8859
loc_1_8857:
        push   1
loc_1_8859:
        mov    ecx, dword ptr [edi + 24h]
        push   dword ptr [ebx + 4]
        push   dword ptr [ecx + 14h]
        push   edi
        call   sub_1_8B9B
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_1_8874
        mov    esi, 1
loc_1_8874:
        mov    dword ptr [edi + 0ch], 0
        push   dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_8A02
loc_1_8883:
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_9B4D
        add    esp, 4
        push   edi
        call   sub_1_8DA5
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_88A1
        mov    esi, 1
loc_1_88A1:
        push   dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_8A02
loc_1_88A9:
        push   edi
        call   sub_1_8E06
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_88BB
        mov    esi, 1
loc_1_88BB:
        and    byte ptr [edi], 0fbh
        jmp    NEAR PTR loc_1_8A0A
loc_1_88C3:
        mov    eax, dword ptr [ebx]
        cmp    eax, 4
        je     SHORT loc_1_88DE
        cmp    eax, 5
        je     SHORT loc_1_88E6
        cmp    eax, 6
        je     SHORT loc_1_88F0
        cmp    eax, 7
        je     SHORT loc_1_88FB
        jmp    NEAR PTR loc_1_8A0A
loc_1_88DE:
        mov    dword ptr [edi + 28h], eax
        jmp    NEAR PTR loc_1_8A0A
loc_1_88E6:
        mov    esi, 0ch
        jmp    NEAR PTR loc_1_8A0A
loc_1_88F0:
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [edi + 0ch], eax
        jmp    NEAR PTR loc_1_8A0A
loc_1_88FB:
        mov    esi, 0ch
        jmp    NEAR PTR loc_1_8A0A
loc_1_8905:
        mov    eax, dword ptr [ebx]
        cmp    eax, 1
        je     SHORT loc_1_891B
        cmp    eax, 3
        je     SHORT loc_1_8926
        cmp    eax, 6
        je     SHORT loc_1_8931
        jmp    NEAR PTR loc_1_8A0A
loc_1_891B:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [ebx + 4], eax
        jmp    NEAR PTR loc_1_8A0A
loc_1_8926:
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 4], eax
        jmp    NEAR PTR loc_1_8A0A
loc_1_8931:
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebx + 4], eax
        jmp    NEAR PTR loc_1_8A0A
loc_1_893C:
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_9B4D
        add    esp, 4
        push   dword ptr [ebx]
        push   dword ptr [ebx + 4]
        push   edi
        call   sub_1_8D8A
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_895F
        mov    esi, 1
loc_1_895F:
        push   dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_8A02
loc_1_8967:
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_9B4D
        add    esp, 4
        lea    eax, [ebp - 1]
        push   eax
        push   dword ptr [ebx]
        push   edi
        call   sub_1_8D1B
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_898E
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebx + 4], al
        jmp    SHORT loc_1_8993
loc_1_898E:
        mov    esi, 1
loc_1_8993:
        push   dword ptr [ebp - 8]
        jmp    SHORT loc_1_8A02
loc_1_8998:
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_9B4D
        add    esp, 4
        push   edi
        call   sub_1_8E1E
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_89B6
        mov    esi, 1
loc_1_89B6:
        push   dword ptr [ebp - 8]
        jmp    SHORT loc_1_8A02
loc_1_89BB:
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_9B4D
        add    esp, 4
        push   edi
        call   sub_1_8E3B
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_89D9
        mov    esi, 1
loc_1_89D9:
        push   dword ptr [ebp - 8]
        jmp    SHORT loc_1_8A02
loc_1_89DE:
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_9B4D
        add    esp, 4
        push   edi
        call   sub_1_8E54
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_89FC
        mov    esi, 1
loc_1_89FC:
        or     byte ptr [edi], 4
        push   dword ptr [ebp - 8]
loc_1_8A02:
        call   sub_1_9B5B
        add    esp, 4
loc_1_8A0A:
        mov    eax, esi
        jmp    SHORT loc_1_8A77
loc_1_8A0E:
        test   cl, 2
        jne    SHORT loc_1_8A72
        or     cl, 2
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi], ecx
        mov    dword ptr [edi + 4], eax
        mov    edx, dword ptr [edi + 24h]
        movzx  eax, word ptr [edx + 4]
        shl    eax, 10h
        push   2
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [edi + 18h]
        inc    eax
        mov    dword ptr [edi + 18h], eax
        mov    dword ptr [edi + 1ch], eax
        mov    dword ptr [ebx + 4], eax
        push   dword ptr [edi + 8]
        call   sub_1_394B
        add    esp, 8
        xor    eax, eax
        jmp    SHORT loc_1_8A77
loc_1_8A48:
        test   cl, 2
        jne    SHORT loc_1_8A54
        mov    eax, 5
        jmp    SHORT loc_1_8A77
loc_1_8A54:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [edi + 4], eax
        jne    SHORT loc_1_8A72
        and    cl, 0fdh
        push   3
        mov    dword ptr [edi], ecx
        push   dword ptr [edi + 8]
        call   sub_1_394B
        add    esp, 8
        xor    eax, eax
        jmp    SHORT loc_1_8A77
loc_1_8A72:
        mov    eax, 4
loc_1_8A77:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_8AAA:
        push   esi
        push   0ffffh
        mov    esi, dword ptr [esp + 0ch]
        push   4
        push   dword ptr [esi + 20h]
        call   sub_1_436F
        add    esp, 0ch
        test   eax, eax
        mov    eax, 1
        jne    SHORT loc_1_8AE4
        push   0ffffh
        push   dword ptr [esp + 10h]
        push   dword ptr [esi + 20h]
        call   sub_1_436F
        add    esp, 0ch
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_1_8AE4:
        pop    esi
        ret
sub_1_8AE6:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   0ffffh
        push   5
        lea    esi, [edi + 20h]
        push   dword ptr [esi]
        call   sub_1_436F
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_8B13
        mov    eax, 1
        jmp    NEAR PTR loc_1_8B95
loc_1_8B13:
        push   0ffffh
        push   dword ptr [ebp + 0ch]
        push   dword ptr [esi]
        call   sub_1_436F
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_8B30
        mov    eax, 1
        jmp    SHORT loc_1_8B95
loc_1_8B30:
        push   0ffffh
        push   dword ptr [ebp + 10h]
        push   dword ptr [esi]
        call   sub_1_436F
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_8B4D
        mov    eax, 1
        jmp    SHORT loc_1_8B95
loc_1_8B4D:
        push   0ffffh
        push   3
        push   dword ptr [esi]
        call   sub_1_436F
        add    esp, 0ch
        test   eax, eax
        mov    eax, 1
        jne    SHORT loc_1_8B95
        push   edi
        call   sub_1_913C
        add    esp, 4
        mov    eax, dword ptr [esi]
        mov    ax, word ptr [eax + 10h]
        lea    ecx, [ebp - 1]
        mov    word ptr [ebp - 4], ax
        mov    dword ptr [ebp - 8], ecx
        mov    dx, word ptr [ebp - 4]
        in     al, dx
        mov    esi, dword ptr [ebp - 8]
        mov    byte ptr [esi], al
        mov    al, byte ptr [ebp - 1]
        sub    al, byte ptr [ebp + 10h]
        cmp    al, 1
        sbb    eax, eax
        inc    eax
loc_1_8B95:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_8B9B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        add    esi, 20h
        push   0ffffh
        push   1
        push   dword ptr [esi]
        call   sub_1_436F
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_8BC9
        mov    eax, 1
        jmp    NEAR PTR loc_1_8D14
loc_1_8BC9:
        push   0ffffh
        lea    eax, [ebp - 1]
        push   eax
        push   dword ptr [esi]
        call   sub_1_43EF
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_8BEA
        mov    eax, 1
        jmp    NEAR PTR loc_1_8D14
loc_1_8BEA:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_8BFA
        mov    eax, 1
        jmp    NEAR PTR loc_1_8D14
loc_1_8BFA:
        mov    di, word ptr [ebp + 10h]
        push   0ffffh
        dec    di
        push   edi
        push   dword ptr [esi]
        call   sub_1_436F
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_8C1E
        mov    eax, 1
        jmp    NEAR PTR loc_1_8D14
loc_1_8C1E:
        mov    ax, di
        push   0ffffh
        shr    ax, 8
        push   eax
        push   dword ptr [esi]
        call   sub_1_436F
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_8C43
        mov    eax, 1
        jmp    NEAR PTR loc_1_8D14
loc_1_8C43:
        movzx  edi, di
        xor    bx, bx
        inc    edi
        test   edi, edi
        jle    SHORT loc_1_8C74
loc_1_8C4E:
        movzx  ecx, bx
        push   0ffffh
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [ecx + eax]
        push   eax
        push   dword ptr [esi]
        call   sub_1_436F
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_8C9D
        inc    bx
        movzx  eax, bx
        cmp    eax, edi
        jl     SHORT loc_1_8C4E
loc_1_8C74:
        push   dword ptr [ebp + 8]
        call   sub_1_913C
        add    esp, 4
        lea    eax, [ebp - 1]
        push   0ffffh
        push   eax
        push   dword ptr [esi]
        call   sub_1_43EF
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_8CA4
        mov    eax, 1
        jmp    SHORT loc_1_8D14
loc_1_8C9D:
        mov    eax, 1
        jmp    SHORT loc_1_8D14
loc_1_8CA4:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_8CB1
        mov    eax, 1
        jmp    SHORT loc_1_8D14
loc_1_8CB1:
        cmp    dword ptr [ebp + 14h], 1
        jne    SHORT loc_1_8D12
        push   dword ptr [ebp + 8]
        call   sub_1_913C
        add    esp, 4
        push   0ffffh
        push   3
        push   dword ptr [esi]
        call   sub_1_436F
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_8CDE
        mov    eax, 1
        jmp    SHORT loc_1_8D14
loc_1_8CDE:
        push   dword ptr [ebp + 8]
        call   sub_1_913C
        add    esp, 4
        lea    eax, [ebp - 1]
        push   0ffffh
        push   eax
        push   dword ptr [esi]
        call   sub_1_43EF
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_8D07
        mov    eax, 1
        jmp    SHORT loc_1_8D14
loc_1_8D07:
        cmp    byte ptr [ebp - 1], 55h
        mov    eax, 1
        jne    SHORT loc_1_8D14
loc_1_8D12:
        xor    eax, eax
loc_1_8D14:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_8D1B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   0d0h
        push   dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_8AE6
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_8D41
        mov    eax, 1
        jmp    SHORT loc_1_8D85
loc_1_8D41:
        push   esi
        call   sub_1_913C
        add    esp, 4
        push   0ffffh
        push   3
        push   dword ptr [esi + 20h]
        call   sub_1_436F
        add    esp, 0ch
        test   eax, eax
        mov    eax, 1
        jne    SHORT loc_1_8D85
        push   esi
        call   sub_1_913C
        add    esp, 4
        mov    eax, dword ptr [esi + 20h]
        mov    ax, word ptr [eax + 10h]
        mov    word ptr [ebp - 2], ax
        mov    dx, word ptr [ebp - 2]
        in     al, dx
        mov    esi, dword ptr [ebp + 10h]
        mov    byte ptr [esi], al
        xor    eax, eax
loc_1_8D85:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_8D8A:
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        call   sub_1_8AE6
        add    esp, 0ch
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        ret
sub_1_8DA5:
        push   esi
        push   80h
        mov    esi, dword ptr [esp + 0ch]
        push   esi
        call   sub_1_8AAA
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_8DC3
        mov    eax, 1
        jmp    SHORT loc_1_8E04
loc_1_8DC3:
        push   70h
        push   esi
        call   sub_1_8AAA
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_8DD9
        mov    eax, 1
        jmp    SHORT loc_1_8E04
loc_1_8DD9:
        push   dword ptr [esi + 28h]
        push   esi
        call   sub_1_8E83
        add    esp, 8
        test   eax, eax
        mov    eax, 1
        jne    SHORT loc_1_8E04
        push   80h
        push   0
        push   esi
        call   sub_1_8AE6
        add    esp, 0ch
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_1_8E04:
        pop    esi
        ret
sub_1_8E06:
        push   80h
        push   dword ptr [esp + 8]
        call   sub_1_8AAA
        add    esp, 8
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        ret
sub_1_8E1E:
        push   80h
        push   0ffh
        push   dword ptr [esp + 0ch]
        call   sub_1_8AE6
        add    esp, 0ch
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        ret
sub_1_8E3B:
        push   80h
        push   0
        push   dword ptr [esp + 0ch]
        call   sub_1_8AE6
        add    esp, 0ch
        mov    eax, 1
        ret
sub_1_8E54:
        push   80h
        push   dword ptr [esp + 8]
        call   sub_1_8AAA
        add    esp, 8
        test   eax, eax
        mov    eax, 1
        jne    SHORT loc_1_8E82
        push   70h
        push   dword ptr [esp + 8]
        call   sub_1_8AAA
        add    esp, 8
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_1_8E82:
        ret
sub_1_8E83:
        xor    al, al
        cmp    word ptr [esp + 8], 1
        jne    SHORT loc_1_8E8F
        mov    al, 11h
loc_1_8E8F:
        push   81h
        push   eax
        push   dword ptr [esp + 0ch]
        call   sub_1_8AE6
        add    esp, 0ch
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        ret
sub_1_8EA8:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0
        xor    eax, eax
        ret
sub_1_8EB5:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        xor    ebx, ebx
        mov    dword ptr [esi + 8], eax
        or     byte ptr [esi], 1
        mov    dword ptr [esi + 4], ebx
        mov    dword ptr [esi + 0ch], ebx
        mov    dword ptr [esi + 14h], ebx
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 18h], ebx
        push   ebx
        mov    dword ptr [esi + 1ch], ebx
        push   ebx
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 10h], 1200000h
        mov    dword ptr [esi + 28h], 2
        push   esi
        call   sub_1_8AE6
        add    esp, 0ch
        push   0fch
        push   esi
        mov    edi, 0ffffh
        call   sub_1_8AAA
        add    esp, 8
        push   0fah
        push   esi
        call   sub_1_8AAA
        add    esp, 8
loc_1_8F1C:
        push   edi
        push   0fh
        push   dword ptr [ebp + 10h]
        call   sub_1_436F
        add    esp, 0ch
        push   edi
        push   83h
        push   dword ptr [ebp + 10h]
        call   sub_1_436F
        movzx  eax, bl
        add    esp, 0ch
        inc    bl
        lea    ecx, [ebp + eax - 8]
        push   edi
        push   ecx
        mov    dword ptr [ebp - 0ch], ecx
        push   dword ptr [ebp + 10h]
        call   sub_1_43EF
        add    esp, 0ch
        push   edi
        push   0eh
        push   dword ptr [ebp + 10h]
        call   sub_1_436F
        add    esp, 0ch
        push   edi
        push   83h
        push   dword ptr [ebp + 10h]
        call   sub_1_436F
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 0ch]
        push   edi
        mov    al, byte ptr [ecx]
        not    al
        push   eax
        push   dword ptr [ebp + 10h]
        call   sub_1_436F
        add    esp, 0ch
        cmp    bl, 4
        jb     SHORT loc_1_8F1C
        push   0fch
        push   esi
        xor    bl, bl
        call   sub_1_8AAA
        add    esp, 8
        mov    edi, 0ffffh
        push   0f9h
        push   esi
        call   sub_1_8AAA
        add    esp, 8
loc_1_8FAF:
        push   edi
        push   0fh
        push   dword ptr [ebp + 10h]
        call   sub_1_436F
        add    esp, 0ch
        push   edi
        push   83h
        push   dword ptr [ebp + 10h]
        call   sub_1_436F
        add    esp, 0ch
        lea    eax, [ebp - 1]
        push   edi
        push   eax
        push   dword ptr [ebp + 10h]
        call   sub_1_43EF
        movzx  ecx, bl
        add    esp, 0ch
        mov    al, byte ptr [ebp + ecx - 8]
        not    al
        cmp    al, byte ptr [ebp - 1]
        jne    NEAR PTR loc_1_90EF
        inc    bl
        cmp    bl, 4
        jb     SHORT loc_1_8FAF
        push   0fch
        push   esi
        xor    bl, bl
        call   sub_1_8AAA
        add    esp, 8
        mov    edi, 0ffffh
        push   0fah
        push   esi
        call   sub_1_8AAA
        add    esp, 8
loc_1_901A:
        push   edi
        push   0eh
        push   dword ptr [ebp + 10h]
        call   sub_1_436F
        add    esp, 0ch
        push   edi
        push   83h
        push   dword ptr [ebp + 10h]
        call   sub_1_436F
        movzx  eax, bl
        add    esp, 0ch
        inc    bl
        push   edi
        push   dword ptr [ebp + eax - 8]
        push   dword ptr [ebp + 10h]
        call   sub_1_436F
        add    esp, 0ch
        cmp    bl, 4
        jb     SHORT loc_1_901A
        push   0fch
        push   esi
        xor    bl, bl
        call   sub_1_8AAA
        add    esp, 8
        mov    edi, 0ffffh
        push   0f9h
        push   esi
        call   sub_1_8AAA
        add    esp, 8
loc_1_9076:
        push   edi
        push   0fh
        push   dword ptr [ebp + 10h]
        call   sub_1_436F
        add    esp, 0ch
        push   edi
        push   83h
        push   dword ptr [ebp + 10h]
        call   sub_1_436F
        add    esp, 0ch
        lea    eax, [ebp - 1]
        push   edi
        push   eax
        push   dword ptr [ebp + 10h]
        call   sub_1_43EF
        movzx  ecx, bl
        add    esp, 0ch
        mov    dl, byte ptr [ebp - 1]
        cmp    byte ptr [ebp + ecx - 8], dl
        jne    SHORT loc_1_9109
        inc    bl
        cmp    bl, 4
        jb     SHORT loc_1_9076
        push   0
        push   esi
        call   sub_1_8AAA
        add    esp, 8
        push   0ffffh
        push   dword ptr [ebp + 10h]
        call   sub_1_4288
        add    esp, 8
        mov    edi, dword ptr [ebp + 14h]
        push   4
        push   edi
        call   sub_1_5272
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_9123
        mov    eax, 1
        and    byte ptr [esi], 0feh
        jmp    SHORT loc_1_9128
loc_1_90EF:
        push   0ffffh
        and    byte ptr [esi], 0feh
        push   dword ptr [ebp + 10h]
        call   sub_1_4288
        add    esp, 8
        mov    eax, 1
        jmp    SHORT loc_1_9128
loc_1_9109:
        push   0ffffh
        and    byte ptr [esi], 0feh
        push   dword ptr [ebp + 10h]
        call   sub_1_4288
        add    esp, 8
        mov    eax, 1
        jmp    SHORT loc_1_9128
loc_1_9123:
        xor    eax, eax
        mov    dword ptr [esi + 24h], edi
loc_1_9128:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_912F:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0
        xor    eax, eax
        ret
sub_1_913C:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 4
        mov    ecx, dword ptr [eax + 20h]
        mov    dx, word ptr [ecx + 4]
        mov    word ptr [esp + 2], dx
        mov    cx, 6a4h
        mov    dx, word ptr [esp + 2]
        add    dx, 5
loc_1_915C:
        in     al, dx
        dec    cx
        cmp    cx, 0
        jne    SHORT loc_1_915C
        add    esp, 4
        ret
        db  0CCh, 0CCh, 0CCh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0BFh, 000h, 000h, 000h
sub_1_9178:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 0A5h, 032h, 004h, 01Eh, 000h, 000h, 056h, 053h, 042h, 031h
        db  036h, 020h, 020h, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_91FC:
        and    eax, 255b2073h
        db  078h, 05Dh
        add    byte ptr [eax], al
        int3
        int3
        int3
loc_1_9208:
        cmp    dword ptr [esp + 4], 1
        je     SHORT loc_1_9219
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_92B4
loc_1_9219:
        mov    eax, dword ptr [esp + 8]
        test   eax, eax
        je     NEAR PTR loc_1_92B2
        cmp    eax, 1
        je     SHORT loc_1_9240
        cmp    eax, 2
        je     SHORT loc_1_925C
        cmp    eax, 3
        je     SHORT loc_1_929B
        cmp    eax, 4
        je     SHORT loc_1_92A9
        mov    eax, 0ffffffffh
        jmp    SHORT loc_1_92B4
loc_1_9240:
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 0BFh
        xor    eax, eax
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_92B2
loc_1_925C:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_1_301E
        test   eax, eax
        je     SHORT loc_1_9292
loc_1_926E:
        test   byte ptr [eax + 0d4h], 2
        je     SHORT loc_1_9285
        mov    cx, word ptr [eax + 683h]
        db  066h, 089h, 00Dh, 000h, 000h, 000h, 000h
loc_1_9285:
        push   eax
        call   sub_1_303E
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_926E
loc_1_9292:
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_92B4
loc_1_929B:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_92B2
loc_1_92A9:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_92B2:
        xor    eax, eax
loc_1_92B4:
        ret
sub_1_92B5:
        push   ebp
        mov    ax, word ptr [esp + 0ch]
        mov    ebp, esp
        db  066h, 039h, 005h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_92F2
        cmp    dword ptr [ebp + 14h], 0
        je     SHORT loc_1_92DB
        mov    eax, dword ptr [ebp + 10h]
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 0BFh
        mov    byte ptr [eax], 0feh
        jmp    SHORT loc_1_9319
loc_1_92DB:
        mov    ecx, dword ptr [ebp + 10h]
        push   dword ptr [ebp + 8]
        movzx  edx, byte ptr [ecx]
        push   edx
        push   5
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 0ch
        jmp    SHORT loc_1_9319
loc_1_92F2:
        cmp    dword ptr [ebp + 14h], 0
        je     SHORT loc_1_9305
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax], cl
        jmp    SHORT loc_1_9319
loc_1_9305:
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax]
        cmp    al, 3fh
        je     SHORT loc_1_9312
        cmp    al, 0ffh
        jne    SHORT loc_1_9319
loc_1_9312:
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 03Fh
loc_1_9319:
        pop    ebp
        ret
        db  0CCh
sub_1_931C:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        mov    eax, 0
        clc
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [esi], ecx
        mov    ax, 0
        clc
        pop    ebx
        pop    esi
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 00Ch, 000h, 003h, 000h
        mov    eax, esi
        mov    esi, dword ptr [ebp + 0ch]
        mov    byte ptr [esi], al
        mov    eax, 0
        clc
        pop    esi
        leave
        ret
sub_1_936C:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [ebp + 8]
        db  0CDh, 020h, 002h, 000h, 003h, 000h
        mov    eax, 0
        clc
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [ebp + 8]
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        mov    eax, 0
        clc
        pop    ebx
        leave
        ret
sub_1_939E:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        mov    eax, 0
        clc
        leave
        ret
sub_1_93B2:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        mov    eax, 0
        clc
        leave
        ret
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 00Ah, 000h, 003h, 000h
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   0
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 015h, 001h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_93F1
        mov    eax, 0
        clc
        jmp    SHORT loc_1_93F7
loc_1_93F1:
        stc
        mov    eax, 1
loc_1_93F7:
        leave
        ret
sub_1_93F9:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        push   0
        push   eax
        push   edx
        db  0CDh, 020h, 076h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_9422
        mov    esi, dword ptr [ebp + 10h]
        mov    word ptr [esi], ax
        mov    eax, 0
        clc
        jmp    SHORT loc_1_9428
loc_1_9422:
        mov    eax, 1
        stc
loc_1_9428:
        pop    esi
        leave
        ret
sub_1_942B:
        push   ebp
        mov    ebp, esp
        movzx  eax, word ptr [ebp + 8]
        push   0
        push   eax
        db  0CDh, 020h, 077h, 000h, 001h, 000h
        add    esp, 8
        leave
        ret
sub_1_9440:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [ebp + 14h]
        push   ecx
        push   eax
        db  0CDh, 020h, 01Eh, 000h, 004h, 000h
        mov    edx, 0fffh
        cmp    eax, edx
        jae    SHORT loc_1_9463
        mov    edx, eax
loc_1_9463:
        pop    eax
        push   0eh
        push   ebx
        push   edx
        push   0
        push   eax
        push   0
        push   1
        push   ecx
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        pop    ecx
        or     eax, eax
        je     SHORT loc_1_9498
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [esi], eax
        mov    esi, dword ptr [ebp + 18h]
        shl    ecx, 0ch
        mov    dword ptr [esi], ecx
        mov    esi, dword ptr [ebp + 1ch]
        mov    dword ptr [esi], edx
        mov    eax, 0
        clc
        jmp    SHORT loc_1_949E
loc_1_9498:
        mov    eax, 1
        stc
loc_1_949E:
        pop    edi
        pop    esi
        pop    ecx
        pop    ebx
        leave
        ret
sub_1_94A4:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
        leave
        ret
sub_1_94B8:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        push   0
        push   0
        push   92h
        push   ecx
        push   edx
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_94EF
        mov    esi, dword ptr [ebp + 14h]
        mov    dword ptr [esi], eax
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [esi], edx
        mov    eax, 0
        clc
        jmp    SHORT loc_1_94F5
loc_1_94EF:
        mov    eax, 1
        stc
loc_1_94F5:
        pop    edi
        pop    esi
        pop    ecx
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        jb     SHORT loc_1_9517
        mov    eax, 0
        clc
        jmp    SHORT loc_1_951D
loc_1_9517:
        mov    eax, 1
        stc
loc_1_951D:
        pop    esi
        leave
        ret
        push   ebp
        mov    ebp, esp
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        db  0CDh, 020h, 018h, 001h, 001h, 000h
        jb     SHORT loc_1_9538
        mov    eax, 0
        clc
        jmp    SHORT loc_1_953E
loc_1_9538:
        mov    eax, 1
        stc
loc_1_953E:
        leave
        ret
sub_1_9540:
        push   ebp
        mov    ebp, esp
        push   esi
        movzx  edx, word ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_1_955B
        mov    eax, 0
        clc
        jmp    SHORT loc_1_9561
loc_1_955B:
        mov    eax, 1
        stc
loc_1_9561:
        pop    esi
        leave
        ret
sub_1_9564:
        push   ebp
        mov    ebp, esp
        movzx  edx, word ptr [ebp + 8]
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        mov    eax, 0
        clc
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 095h, 000h, 001h, 000h
        jb     SHORT loc_1_9590
        mov    eax, 0
        clc
        jmp    SHORT loc_1_9596
loc_1_9590:
        mov    eax, 1
        stc
loc_1_9596:
        pop    esi
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi], ebx
        pop    ebx
        pop    esi
        leave
        ret
sub_1_95AD:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        jb     SHORT loc_1_95CA
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [esi], eax
        mov    eax, 0
        clc
        jmp    SHORT loc_1_95D0
loc_1_95CA:
        mov    eax, 1
        stc
loc_1_95D0:
        pop    edi
        pop    esi
        leave
        ret
sub_1_95D4:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 011h, 000h, 003h, 000h
        mov    eax, 0
        clc
        leave
        ret
loc_1_95E8:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dx, word ptr [ebp + 0ch]
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        pop    ebx
        leave
        ret
sub_1_95FE:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dx, word ptr [ebp + 0ch]
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        pop    ebx
        leave
        ret
sub_1_9614:
        push   ebp
        mov    ebp, esp
        push   esi
        db  0CDh, 020h, 000h, 000h, 001h, 000h
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi], ax
        clc
        pop    esi
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 012h, 000h, 004h, 000h
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 013h, 000h, 004h, 000h
        jb     SHORT loc_1_9655
        mov    eax, 0
        clc
        jmp    SHORT loc_1_965B
loc_1_9655:
        mov    eax, 1
        stc
loc_1_965B:
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 014h, 000h, 004h, 000h
        jb     SHORT loc_1_9678
        mov    eax, 0
        clc
        jmp    SHORT loc_1_967E
loc_1_9678:
        mov    eax, 1
        stc
loc_1_967E:
        pop    ebx
        leave
        ret
sub_1_9681:
        push   ebp
        mov    ebp, esp
        push   esi
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 001h, 000h, 004h, 000h
        jb     SHORT loc_1_96A2
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [esi], eax
        mov    eax, 0
        clc
        jmp    SHORT loc_1_96A8
loc_1_96A2:
        mov    eax, 1
        stc
loc_1_96A8:
        pop    esi
        leave
        ret
sub_1_96AB:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 01Ah, 000h, 004h, 000h
        mov    eax, 0
        clc
        leave
        ret
sub_1_96BF:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   esi
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    ebx, 1
        sbb    ebx, ebx
        inc    bh
        mov    bl, 0
        mov    edx, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [ebp + 14h]
        mov    esi, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 003h, 000h, 004h, 000h
        mov    eax, 0
        clc
        pop    esi
        pop    ecx
        pop    ebx
        leave
        ret
sub_1_96EE:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        mov    dl, byte ptr [ebp + 10h]
        mov    dh, 0
        db  0CDh, 020h, 006h, 000h, 004h, 000h
        mov    eax, 0
        clc
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 008h, 000h, 004h, 000h
        mov    eax, 0
        clc
        pop    ebx
        leave
        ret
sub_1_9725:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        db  0CDh, 020h, 019h, 000h, 004h, 000h
        mov    eax, 0
        clc
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 007h, 000h, 004h, 000h
        mov    eax, 0
        clc
        pop    ebx
        leave
        ret
sub_1_9757:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 018h, 000h, 004h, 000h
        mov    eax, 0
        clc
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 004h, 000h, 004h, 000h
        mov    ebx, dword ptr [ebp + 10h]
        mov    dword ptr [ebx], esi
        mov    ebx, dword ptr [ebp + 14h]
        mov    dword ptr [ebx], ecx
        mov    ebx, dword ptr [ebp + 18h]
        mov    byte ptr [ebx], dl
        mov    eax, 0
        clc
        pop    edi
        pop    esi
        pop    ecx
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 14h]
        movzx  dx, byte ptr [ebp + 18h]
        db  0CDh, 020h, 005h, 000h, 004h, 000h
        mov    eax, 0
        clc
        pop    esi
        pop    ecx
        pop    ebx
        leave
        ret
sub_1_97C6:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   esi
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 01Ch, 000h, 004h, 000h
        mov    esi, dword ptr [ebp + 10h]
        mov    word ptr [esi], cx
        mov    eax, 0
        clc
        pop    esi
        pop    ecx
        pop    ebx
        leave
        ret
sub_1_97E9:
        push   ebp
        mov    ebp, esp
        push   esi
        pushfd
        cli
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     SHORT loc_1_9808
        popfd
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [esi], eax
        mov    eax, 0
        clc
        jmp    SHORT loc_1_980F
loc_1_9808:
        popfd
        mov    eax, 1
        stc
loc_1_980F:
        pop    esi
        leave
        ret
sub_1_9812:
        push   ebp
        mov    ebp, esp
        push   esi
        pushfd
        cli
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        popfd
        pop    esi
        leave
        ret
sub_1_9828:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, 5
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_1_984A
        mov    ebx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebx], esi
        mov    eax, 0
        clc
        jmp    SHORT loc_1_9850
loc_1_984A:
        mov    eax, 1
        stc
loc_1_9850:
        pop    ecx
        pop    ebx
        leave
        ret
sub_1_9854:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
        pop    esi
        leave
        ret
sub_1_9864:
        push   ebp
        mov    ebp, esp
        push   esi
        pushfd
        cli
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        popfd
        pop    esi
        leave
        ret
sub_1_987A:
        push   ebp
        mov    ebp, esp
        push   esi
        pushfd
        cli
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        popfd
        pop    esi
        leave
        ret
sub_1_9890:
        push   ebp
        mov    ebp, esp
        push   esi
        movzx  eax, word ptr [ebp + 8]
        push   1
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_98B5
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [esi], eax
        mov    eax, 0
        clc
        jmp    SHORT loc_1_98BB
loc_1_98B5:
        mov    eax, 1
        stc
loc_1_98BB:
        pop    esi
        leave
        ret
sub_1_98BE:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        leave
        ret
sub_1_98D2:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, 15000eh
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        jb     SHORT loc_1_98ED
        mov    eax, 0
        jmp    SHORT loc_1_98F2
loc_1_98ED:
        mov    eax, 1
loc_1_98F2:
        pop    esi
        leave
        ret
sub_1_98F5:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, 15000eh
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 01Ch, 001h, 001h, 000h
        jb     SHORT loc_1_9910
        mov    eax, 0
        jmp    SHORT loc_1_9915
loc_1_9910:
        mov    eax, 1
loc_1_9915:
        pop    esi
        leave
        ret
        db  050h, 053h, 0CDh, 020h, 03Fh, 000h, 001h, 000h, 08Bh, 0D8h, 0CDh, 020h, 03Fh, 000h, 001h, 000h
        db  03Bh, 0C3h, 074h, 0F6h, 05Bh, 058h, 0C3h, 0CCh
VSB16_Service_00:
        mov    eax, 101h
        cmp    ecx, 4354454dh
        jne    SHORT loc_1_994F
        mov    ecx, 454d4354h
        db  089h, 015h, 000h, 000h, 000h, 000h
        db  0BAh, 000h, 000h, 000h, 000h
        clc
        ret
loc_1_994F:
        mov    ah, 4
        mov    al, 1eh
        clc
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_9958:
        push   edi
        push   ecx
        push   eax
        push   ebx
        call   sub_1_2C34
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_996B
        clc
        pop    edi
        ret
loc_1_996B:
        stc
        pop    edi
        ret
        db  08Bh, 0C0h
loc_1_9970:
        push   edi
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        pop    edi
        ret
        db  090h
loc_1_9980:
        db  0CDh, 020h, 012h, 000h, 004h, 000h
        clc
        ret
        db  0EBh, 006h, 0FFh, 025h, 000h, 000h, 000h, 000h
loc_1_9990:
        pushad
        pushfd
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_1_99A5
        xor    edx, edx
        push   edx
        push   ebx
        call   sub_1_2C8E
        pop    ecx
        pop    ecx
loc_1_99A5:
        popfd
        popad
        db  0FFh, 025h, 000h, 000h, 000h, 000h
loc_1_99AD:
        pushad
        or     edx, 0f000000h
        push   edx
        push   ebx
        call   sub_1_2C8E
        pop    ecx
        pop    ecx
        popad
        clc
        ret
VSB16_Control:
        cmp    eax, 0bh
        je     SHORT loc_1_99AD
        cmp    eax, 1bh
        je     NEAR PTR loc_1_9B64
        cmp    eax, 1ch
        je     NEAR PTR loc_1_9B71
        cmp    eax, 22h
        je     NEAR PTR loc_1_9C6C
        cmp    eax, 23h
        je     NEAR PTR loc_1_9C31
        clc
        ret
        db  090h
loc_1_99EC:
        push   edi
        push   edx
        push   eax
        push   ebx
        call   sub_1_288E
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_99FF
        pop    edi
        clc
        ret
loc_1_99FF:
        pop    edi
        stc
        ret
VSB16_PM_API:
        push   ebp
        push   eax
        movzx  eax, word ptr [ebp + 38h]
        or     ax, ax
        pop    eax
        je     SHORT loc_1_9A18
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
loc_1_9A18:
        push   eax
        movzx  eax, word ptr [ebp + 4]
        push   eax
        mov    ax, word ptr [ebp + 18h]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 1ch]
        push   eax
        movzx  eax, word ptr [ebp + 14h]
        push   eax
        movzx  eax, word ptr [ebp + 10h]
        push   eax
        mov    eax, ebx
        push   eax
        call   sub_1_25C4
        add    esp, 18h
        pop    ebp
        mov    ebx, eax
        mov    word ptr [ebp + 1ch], ax
        mov    cl, 10h
        shr    eax, cl
        mov    word ptr [ebp + 14h], ax
        or     ebx, ebx
        jne    SHORT loc_1_9A5A
        and    dword ptr [ebp + 2ch], 0fffeh
        ret
loc_1_9A5A:
        or     dword ptr [ebp + 2ch], 1
        ret
        db  090h
sub_1_9A60:
        push   ebp
        mov    ebp, esp
        pushad
        mov    esi, dword ptr [ebp + 8]
        mov    eax, 1
        pushfd
        cli
        xchg   dword ptr [esi + 8], eax
        or     eax, eax
        jne    SHORT loc_1_9A92
        popfd
        mov    ebx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ecx, 1
        mov    eax, 400000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        pushfd
loc_1_9A92:
        popfd
        popad
        leave
        ret
        db  08Bh, 0C0h
loc_1_9A98:
        pushad
        pushfd
        mov    edi, edx
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
loc_1_9AB1:
        pushfd
        cli
        mov    ebx, dword ptr [edi + 4]
        cmp    ebx, dword ptr [edi]
        je     SHORT loc_1_9B2F
        mov    esi, dword ptr [edi + ebx*4 + 0ch]
        xor    edx, edx
        mov    eax, ebx
        inc    eax
        mov    ebx, 20h
        div    bx
        mov    dword ptr [edi + 4], edx
        popfd
        mov    eax, dword ptr [esi + 14h]
        mov    word ptr [ebp + 1ch], ax
        shr    eax, 10h
        mov    word ptr [ebp + 18h], ax
        movzx  eax, word ptr [esi + 12h]
        mov    word ptr [ebp + 10h], ax
        movzx  eax, word ptr [esi + 10h]
        mov    word ptr [ebp + 14h], ax
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [ebp + 4], ax
        shr    eax, 10h
        mov    word ptr [ebp + 3ch], ax
        mov    eax, dword ptr [esi + 4]
        mov    word ptr [ebp], ax
        shr    eax, 10h
        mov    word ptr [ebp + 38h], ax
        movzx  edx, word ptr [esi + 0eh]
        mov    cx, word ptr [esi + 0ch]
        push   esi
        push   edi
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        pop    edi
        pop    esi
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_1_9AB1
        push   dword ptr [esi + 14h]
        call   eax
        pop    eax
        jmp    SHORT loc_1_9AB1
loc_1_9B2F:
        xor    eax, eax
        xchg   dword ptr [edi + 8], eax
        popfd
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        popfd
        popad
        ret
sub_1_9B4D:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        pushfd
        pop    dword ptr [esi]
        cli
        pop    esi
        leave
        ret
sub_1_9B5B:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        popfd
        leave
        ret
loc_1_9B64:
        call   sub_1_308F
        or     eax, eax
        jne    SHORT loc_1_9B6F
        clc
        ret
loc_1_9B6F:
        stc
        ret
loc_1_9B71:
        call   sub_1_30D0
        or     eax, eax
        jne    SHORT loc_1_9B7C
        clc
        ret
loc_1_9B7C:
        stc
        ret
        db  08Bh, 0C0h
loc_1_9B80:
        cmp    ecx, 4
        jbe    SHORT loc_1_9B8B
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_9B8B:
        je     SHORT loc_1_9B94
        push   eax
        mov    eax, esp
        push   1
        jmp    SHORT loc_1_9B99
loc_1_9B94:
        push   eax
        mov    eax, esp
        push   0
loc_1_9B99:
        push   eax
        push   edx
        push   ebx
        call   sub_1_3109
        add    esp, 10h
        pop    eax
        ret
        db  08Bh, 0C0h
loc_1_9BA8:
        cmp    ecx, 4
        jbe    SHORT loc_1_9BB3
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_9BB3:
        je     SHORT loc_1_9BBC
        push   eax
        mov    eax, esp
        push   1
        jmp    SHORT loc_1_9BC1
loc_1_9BBC:
        push   eax
        mov    eax, esp
        push   0
loc_1_9BC1:
        push   eax
        push   edx
        push   ebx
        call   sub_1_338D
        add    esp, 10h
        pop    eax
        ret
        db  066h, 081h, 07Dh, 01Ch, 0F0h, 091h, 075h, 032h, 0CDh, 020h, 004h, 000h, 001h, 000h, 074h, 02Ah
        db  052h, 051h, 056h, 0CDh, 020h, 083h, 000h, 001h, 000h, 0BEh, 054h, 000h, 000h, 000h, 08Bh, 00Eh
        db  08Bh, 0D1h, 0C1h, 0C1h, 010h, 0CDh, 020h, 04Ch, 000h, 001h, 000h, 0CDh, 020h, 085h, 000h, 001h
        db  000h, 0CDh, 020h, 086h, 000h, 001h, 000h, 05Eh, 059h, 05Ah, 0F9h, 0C3h, 08Bh, 0C0h
sub_1_9C0C:
        push   ebp
        mov    ebp, esp
        pushad
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        pushfd
        cli
        mov    ebx, dword ptr [edi]
        mov    dword ptr [edi + ebx*4 + 0ch], eax
        xor    edx, edx
        mov    eax, ebx
        inc    eax
        mov    ebx, 20h
        div    bx
        mov    dword ptr [edi], edx
        popfd
        popad
        leave
        ret
loc_1_9C31:
        push   esi
        push   edx
        push   ebx
        push   ecx
        call   sub_1_3A60
        add    esp, 10h
        ret
sub_1_9C3E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   0
        push   word ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_1_9C66
        movzx  ebx, word ptr [ebp + 10h]
        add    eax, ebx
        mov    ebx, dword ptr [ebp + 14h]
        mov    dword ptr [ebx], eax
        xor    eax, eax
loc_1_9C66:
        pop    ebx
        leave
        ret
        db  0CCh, 0CCh, 0CCh
loc_1_9C6C:
        pushad
        mov    eax, ebx
        db  0BBh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 04Ah, 004h
        popad
        mov    eax, 0
        stc
        ret
sub_1_9C82:
        push   ecx
        db  00Fh, 0B7h, 00Dh, 00Ch, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 00Eh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 010h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 012h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 014h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        pop    ecx
        ret
sub_1_9CE4:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    dx, word ptr [ebp + 8]
        in     al, dx
        mov    esi, dword ptr [ebp + 0ch]
        mov    byte ptr [esi], al
        pop    esi
        leave
        ret
sub_1_9CF5:
        push   ebp
        mov    ebp, esp
        mov    dx, word ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0ch]
        out    dx, al
        leave
        ret
sub_1_9D02:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    dx, word ptr [ebp + 8]
        in     ax, dx
        mov    esi, dword ptr [ebp + 0ch]
        mov    word ptr [esi], ax
        pop    esi
        leave
        ret
sub_1_9D15:
        push   ebp
        mov    ebp, esp
        mov    dx, word ptr [ebp + 8]
        mov    ax, word ptr [ebp + 0ch]
        out    dx, ax
        leave
        ret
        push   ebp
        mov    ebp, esp
        pushad
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    dx, word ptr [ebp + 8]
        db  066h, 089h, 015h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp + 0ch]
        db  089h, 015h, 000h, 000h, 000h, 000h
        mov    dx, word ptr [ebp + 10h]
        db  066h, 089h, 015h, 000h, 000h, 000h, 000h
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ecx, 3
        mov    eax, 10h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        popad
        leave
        ret
loc_1_9D65:
        pushad
        sub    esp, 6ch
        mov    edi, esp
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 1607h
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp + 10h], dx
        db  00Fh, 0B7h, 00Dh, 000h, 000h, 000h, 000h
        sub    word ptr [ebp + 30h], cx
        xchg   edi, eax
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        cld
        rep movsw
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  00Fh, 0B7h, 00Dh, 000h, 000h, 000h, 000h
        add    word ptr [ebp + 30h], cx
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        popad
        ret
sub_1_9DDD:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 4]
        and    eax, 4
        shr    eax, 2
        ret
sub_1_9DEB:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 4]
        test   al, 1
        jne    SHORT loc_1_9DFE
        mov    eax, 1
        jmp    SHORT loc_1_9E41
loc_1_9DFE:
        cmp    dword ptr [esi + 8], 0
        jne    SHORT loc_1_9E14
        push   esi
        call   sub_1_A0F0
        add    esp, 4
        mov    eax, 1
        jmp    SHORT loc_1_9E41
loc_1_9E14:
        test   al, 4
        je     SHORT loc_1_9E31
        push   esi
        call   sub_1_A0F0
        add    esp, 4
        push   esi
        call   sub_1_A122
        add    esp, 4
        mov    eax, 1
        jmp    SHORT loc_1_9E41
loc_1_9E31:
        mov    ax, word ptr [esi + 10h]
        sub    ax, word ptr [esp + 0ch]
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
loc_1_9E41:
        pop    esi
        ret
sub_1_9E43:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 0c0h]
        lea    ecx, [esi + 0b8h]
        cmp    dword ptr [ecx], 0
        je     SHORT loc_1_9EC1
        mov    dword ptr [edi + 14h], esi
        mov    word ptr [edi + 12h], 1
        mov    word ptr [edi + 10h], 1
        mov    eax, dword ptr [esi + 0bch]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [ecx]
        shr    eax, 10h
        lea    edx, [ebp - 4]
        mov    word ptr [edi + 0ch], ax
        push   edx
        mov    eax, dword ptr [ecx]
        mov    word ptr [edi + 0eh], ax
        mov    ecx, dword ptr [esi + 1ah]
        mov    dword ptr [edi + 4], ecx
        db  0C7h, 007h, 000h, 000h, 000h, 000h
        push   dword ptr [esi + 0e8h]
        call   sub_1_39B0
        add    esp, 8
        push   edi
        push   dword ptr [ebp - 4]
        call   sub_1_9C0C
        add    esp, 8
        push   dword ptr [esi + 0b4h]
        push   dword ptr [ebp - 4]
        call   sub_1_9A60
        add    esp, 8
loc_1_9EC1:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_9EC9:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 4], 0
        xor    eax, eax
        ret
sub_1_9ED7:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        xor    edi, edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [esi + 8], edi
        cmp    dword ptr [ebp + 10h], edi
        mov    dword ptr [esi], edi
        mov    dword ptr [esi + 0e8h], eax
        mov    dword ptr [esi + 0e4h], edi
        je     NEAR PTR loc_1_A02A
        cmp    dword ptr [ebp + 18h], edi
        je     NEAR PTR loc_1_A02A
        mov    dword ptr [esi + 54h], 300000ch
        mov    word ptr [esi + 58h], 2
        mov    word ptr [esi + 5ah], 68h
        lea    eax, [ebp - 2]
        push   eax
        db  0BBh, 000h, 000h, 000h, 000h
        push   dword ptr [ebp + 10h]
        call   sub_1_4807
        add    esp, 8
        lea    eax, [esi + 5ch]
        movzx  ecx, word ptr [ebp - 2]
        push   ecx
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   ebx
        push   eax
        call   sub_1_2403
        mov    word ptr [esi + 86h], 2
        add    esp, 10h
        lea    edx, [esi + 8ah]
        mov    word ptr [esi + 88h], 4
        movzx  eax, word ptr [ebp - 2]
        push   eax
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   ebx
        push   edx
        call   sub_1_2403
        add    esp, 10h
        mov    ecx, dword ptr [ebp + 14h]
        cmp    word ptr [ecx + 6], 4
        jae    SHORT loc_1_9FBB
        mov    eax, 333h
        lea    edx, [esi + 84h]
        lea    ecx, [esi + 0aah]
        mov    dword ptr [esi + 7ch], eax
        mov    dword ptr [esi + 80h], edi
        mov    word ptr [edx], 2
        mov    dword ptr [ecx], eax
        lea    eax, [esi + 0b2h]
        mov    dword ptr [esi + 0aeh], edi
        mov    word ptr [eax], 2
        mov    dword ptr [esi], 1
        jmp    SHORT loc_1_9FF2
loc_1_9FBB:
        mov    eax, 0ccch
        xor    ebx, ebx
        lea    edx, [esi + 84h]
        mov    edi, 2
        lea    ecx, [esi + 0aah]
        mov    dword ptr [esi + 7ch], eax
        mov    dword ptr [esi + 80h], ebx
        mov    word ptr [edx], di
        mov    dword ptr [ecx], eax
        lea    eax, [esi + 0b2h]
        mov    dword ptr [esi + 0aeh], ebx
        mov    word ptr [eax], di
        mov    dword ptr [esi], edi
loc_1_9FF2:
        cmp    dword ptr [ebp + 1ch], 0
        je     SHORT loc_1_A00B
        mov    edi, 0ccch
        or     dword ptr [esi + 7ch], edi
        or     byte ptr [edx], 2
        or     dword ptr [ecx], edi
        or     byte ptr [eax], 2
        or     byte ptr [esi], 2
loc_1_A00B:
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebp + 18h]
        mov    dword ptr [esi + 0d8h], eax
        mov    dword ptr [esi + 0dch], ecx
        mov    dword ptr [esi + 0e0h], edx
        or     byte ptr [esi + 4], 1
loc_1_A02A:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_A033:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax + 4], 1
        jne    SHORT loc_1_A044
        mov    eax, 1
        jmp    SHORT loc_1_A04D
loc_1_A044:
        mov    dword ptr [eax + 4], 0
        xor    eax, eax
loc_1_A04D:
        ret
sub_1_A04E:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 4]
        test   esi, 1
        jne    SHORT loc_1_A065
        mov    eax, 1
        jmp    SHORT loc_1_A09C
loc_1_A065:
        lea    edx, [eax + 0b4h]
        mov    ecx, dword ptr [esp + 0ch]
        cmp    dword ptr [edx], ecx
        je     SHORT loc_1_A09A
        test   esi, 2
        je     SHORT loc_1_A082
        mov    eax, 4
        jmp    SHORT loc_1_A09C
loc_1_A082:
        or     esi, 2
        push   2
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [edx], ecx
        push   dword ptr [eax + 0e8h]
        call   sub_1_394B
        add    esp, 8
loc_1_A09A:
        xor    eax, eax
loc_1_A09C:
        pop    esi
        ret
sub_1_A09E:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 4]
        test   al, 1
        jne    SHORT loc_1_A0B1
        mov    eax, 1
        jmp    SHORT loc_1_A0EE
loc_1_A0B1:
        lea    edx, [esi + 0b4h]
        mov    ecx, dword ptr [esp + 0ch]
        cmp    dword ptr [edx], ecx
        je     SHORT loc_1_A0C6
        mov    eax, 1
        jmp    SHORT loc_1_A0EE
loc_1_A0C6:
        test   al, 2
        jne    SHORT loc_1_A0D1
        mov    eax, 1
        jmp    SHORT loc_1_A0EE
loc_1_A0D1:
        and    al, 0fdh
        push   3
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [edx], 0
        push   dword ptr [esi + 0e8h]
        call   sub_1_394B
        add    esp, 8
        xor    eax, eax
loc_1_A0EE:
        pop    esi
        ret
sub_1_A0F0:
        xor    eax, eax
        mov    ecx, dword ptr [esp + 4]
        movzx  edx, word ptr [ecx + 10h]
        cmp    edx, 8
        je     SHORT loc_1_A106
        cmp    edx, 10h
        je     SHORT loc_1_A113
        jmp    SHORT loc_1_A121
loc_1_A106:
        push   dword ptr [ecx + 0d8h]
        call   sub_1_41A1
        jmp    SHORT loc_1_A11E
loc_1_A113:
        push   dword ptr [ecx + 0d8h]
        call   sub_1_41BD
loc_1_A11E:
        add    esp, 4
loc_1_A121:
        ret
sub_1_A122:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0e8h]
        push   dword ptr [ecx + 6afh]
        call   sub_1_931C
        add    esp, 4
        ret
sub_1_A13B:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    edx, dword ptr [ebp + 14h]
        cmp    word ptr [edx + 4], 10h
        push   edi
        jne    SHORT loc_1_A15E
        mov    byte ptr [ebp - 4], 0b2h
        mov    byte ptr [ebp - 3], 10h
        mov    ecx, dword ptr [ebp + 10h]
        shr    ecx, 1
        jmp    SHORT loc_1_A169
loc_1_A15E:
        mov    byte ptr [ebp - 4], 0c2h
        mov    byte ptr [ebp - 3], 0
        mov    ecx, dword ptr [ebp + 10h]
loc_1_A169:
        dec    cx
        mov    byte ptr [ebp - 2], cl
        mov    byte ptr [ebp - 1], ch
        cmp    word ptr [edx + 0ch], 0
        je     SHORT loc_1_A17C
        or     byte ptr [ebp - 4], 8
loc_1_A17C:
        cmp    word ptr [ebp + 0ch], 0
        jne    SHORT loc_1_A187
        or     byte ptr [ebp - 4], 4
loc_1_A187:
        cmp    word ptr [edx + 6], 2
        jne    SHORT loc_1_A192
        or     byte ptr [ebp - 3], 20h
loc_1_A192:
        xor    edi, edi
        mov    esi, dword ptr [ebp + 8]
loc_1_A197:
        test   eax, eax
        jne    SHORT loc_1_A1B8
        push   0ffffh
        inc    edi
        push   dword ptr [ebp + edi - 5]
        push   dword ptr [esi + 0d8h]
        call   sub_1_436F
        add    esp, 0ch
        cmp    edi, 4
        jl     SHORT loc_1_A197
loc_1_A1B8:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_A1BE:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0e8h]
        cmp    dword ptr [edi + 0f4h], 0
        je     SHORT loc_1_A1E2
        cmp    dword ptr [edi + 1e0h], 0
        jne    SHORT loc_1_A221
loc_1_A1E2:
        mov    ax, word ptr [esi + 10h]
        mov    ebx, 0ffffh
        sub    ax, 10h
        push   ebx
        cmp    ax, 1
        sbb    al, al
        and    al, 5
        add    al, 0d0h
        push   eax
        push   dword ptr [esi + 0d8h]
        call   sub_1_436F
        mov    dword ptr [ebp - 8], 0
        add    esp, 0ch
        cmp    dword ptr [ebp - 8], ebx
        jae    SHORT loc_1_A221
loc_1_A215:
        inc    dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], 0ffffh
        jb     SHORT loc_1_A215
loc_1_A221:
        mov    ax, word ptr [esi + 10h]
        push   0ffffh
        sub    ax, 10h
        cmp    ax, 1
        sbb    al, al
        xor    ebx, ebx
        add    al, 0dah
        push   eax
        push   dword ptr [esi + 0d8h]
        call   sub_1_436F
        mov    word ptr [ebp - 2], bx
        add    esp, 0ch
        lea    eax, [ebp - 2]
        mov    edx, dword ptr [esi + 0dch]
        push   eax
        push   dword ptr [esi + 0b4h]
        push   dword ptr [edx + 8]
        call   sub_1_97C6
        add    esp, 0ch
        cmp    word ptr [ebp - 2], bx
        je     SHORT loc_1_A291
loc_1_A26C:
        mov    bx, word ptr [ebp - 2]
        lea    eax, [ebp - 2]
        push   eax
        mov    edx, dword ptr [esi + 0dch]
        push   dword ptr [esi + 0b4h]
        push   dword ptr [edx + 8]
        call   sub_1_97C6
        add    esp, 0ch
        cmp    word ptr [ebp - 2], bx
        jne    SHORT loc_1_A26C
loc_1_A291:
        cmp    dword ptr [edi + 0f4h], 0
        je     SHORT loc_1_A2A3
        cmp    dword ptr [edi + 1e0h], 0
        jne    SHORT loc_1_A2B6
loc_1_A2A3:
        push   0ffffh
        push   dword ptr [esi + 0d8h]
        call   sub_1_4288
        add    esp, 8
loc_1_A2B6:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_A2BD:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 4
        cmp    dword ptr [ebp + 0ch], 1
        sbb    cl, cl
        push   esi
        push   edi
        add    cl, 42h
        xor    edi, edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 4], cl
        mov    cx, word ptr [ebp + 10h]
        mov    byte ptr [ebp - 3], ch
        mov    byte ptr [ebp - 2], cl
loc_1_A2E2:
        test   eax, eax
        jne    SHORT loc_1_A303
        push   0ffffh
        inc    edi
        push   dword ptr [ebp + edi - 5]
        push   dword ptr [esi + 0d8h]
        call   sub_1_436F
        add    esp, 0ch
        cmp    edi, 3
        jl     SHORT loc_1_A2E2
loc_1_A303:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_A309:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        push   0
        push   esi
        call   sub_1_A4AA
        add    esp, 8
        lea    edi, [esi + 0d8h]
        or     byte ptr [esi], 8
        cmp    word ptr [edi], 40h
        jae    SHORT loc_1_A35A
loc_1_A329:
        movzx  eax, word ptr [edi]
        movzx  ecx, word ptr [esi + 0dah]
        push   1
        add    eax, ecx
        add    eax, esi
        add    eax, 58h
        push   eax
        push   dword ptr [esi + 8]
        call   sub_1_414B
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_A35A
        mov    ax, word ptr [edi]
        inc    ax
        mov    word ptr [edi], ax
        cmp    ax, 40h
        jb     SHORT loc_1_A329
loc_1_A35A:
        lea    eax, [esi + 0dch]
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_A373
        push   esi
        mov    dword ptr [eax], 1
        call   sub_1_A381
        jmp    SHORT loc_1_A379
loc_1_A373:
        push   esi
        call   sub_1_A460
loc_1_A379:
        add    esp, 4
        xor    eax, eax
        pop    edi
        pop    esi
        ret
sub_1_A381:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 50h], esi
        lea    edi, [esi + 3ch]
        mov    word ptr [edi + 12h], 2
        mov    word ptr [edi + 10h], 1
        mov    eax, dword ptr [esi + 0ch]
        lea    edx, [esi + 0dah]
        lea    ecx, [esi + 0d8h]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 10h]
        shr    eax, 10h
        mov    word ptr [edi + 0ch], ax
        mov    eax, dword ptr [esi + 10h]
        mov    word ptr [edi + 0eh], ax
        mov    ax, word ptr [edx]
        shl    ax, 8
        or     ax, word ptr [ecx]
        mov    word ptr [edi + 4], ax
        xor    eax, eax
        mov    word ptr [ecx], ax
        cmp    word ptr [edx], ax
        mov    word ptr [edx], 40h
        je     SHORT loc_1_A3E3
        mov    word ptr [edx], 0
loc_1_A3E3:
        lea    eax, [ebp - 4]
        db  0C7h, 007h, 000h, 000h, 000h, 000h
        push   eax
        push   dword ptr [esi + 54h]
        call   sub_1_39B0
        add    esp, 8
        push   edi
        push   dword ptr [ebp - 4]
        call   sub_1_9C0C
        add    esp, 8
        push   dword ptr [esi + 4]
        push   dword ptr [ebp - 4]
        call   sub_1_9A60
        add    esp, 8
        push   esi
        call   sub_1_A460
        add    esp, 4
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_1_A423:
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax + 0d8h], 0
        jbe    SHORT loc_1_A43C
        push   eax
        call   sub_1_A381
        add    esp, 4
        jmp    SHORT loc_1_A446
loc_1_A43C:
        mov    dword ptr [eax + 0dch], 0
loc_1_A446:
        ret
sub_1_A447:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax]
        test   al, 1
        je     SHORT loc_1_A45A
        test   al, 4
        mov    eax, 0
        jne    SHORT loc_1_A45F
loc_1_A45A:
        mov    eax, 1
loc_1_A45F:
        ret
sub_1_A460:
        mov    ecx, dword ptr [esp + 4]
        lea    eax, [ecx + 0dch]
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_A48A
        cmp    word ptr [ecx + 0d8h], 0
        jbe    SHORT loc_1_A48A
        push   ecx
        mov    dword ptr [eax], 1
        call   sub_1_A381
        add    esp, 4
        jmp    SHORT loc_1_A4A9
loc_1_A48A:
        mov    eax, dword ptr [ecx]
        test   al, 10h
        je     SHORT loc_1_A49A
        and    al, 0efh
        mov    dword ptr [ecx], eax
        and    al, 0f7h
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_A4A9
loc_1_A49A:
        and    al, 0f7h
        push   1
        push   ecx
        mov    dword ptr [ecx], eax
        call   sub_1_A4AA
        add    esp, 8
loc_1_A4A9:
        ret
sub_1_A4AA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi], 1
        je     SHORT loc_1_A500
        lea    edi, [esi + 8]
        lea    eax, [ebp - 1]
        push   eax
        mov    ebx, 83h
        push   ebx
        push   dword ptr [edi]
        call   sub_1_40A6
        add    esp, 0ch
        cmp    dword ptr [ebp + 0ch], 1
        jne    SHORT loc_1_A4E4
        mov    al, byte ptr [ebp - 1]
        or     byte ptr [esi], 4
        or     al, 4
        push   eax
        push   ebx
        jmp    SHORT loc_1_A4F2
loc_1_A4E4:
        mov    al, byte ptr [ebp - 1]
        and    byte ptr [esi], 0fbh
        and    al, 0fbh
        push   eax
        push   83h
loc_1_A4F2:
        push   dword ptr [edi]
        call   sub_1_4076
        add    esp, 0ch
        xor    eax, eax
        jmp    SHORT loc_1_A505
loc_1_A500:
        mov    eax, 1
loc_1_A505:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  060h, 09Ch, 0CDh, 020h, 007h, 000h, 005h, 000h, 08Bh, 0DAh, 08Bh, 0F0h, 0B9h, 032h, 000h, 000h
        db  000h, 0E2h, 0FEh, 0CDh, 020h, 007h, 000h, 005h, 000h, 02Bh, 0C6h, 08Bh, 0D8h, 0B8h, 0C8h, 000h
        db  000h, 000h, 0F6h, 0F3h, 0FEh, 0C4h, 088h, 025h, 000h, 000h, 000h, 000h, 09Dh, 061h, 0C3h, 060h
        db  09Ch, 0CDh, 020h, 007h, 000h, 005h, 000h, 0BAh, 000h, 000h, 000h, 000h, 0B8h, 000h, 000h, 000h
        db  000h, 08Ah, 01Dh, 000h, 000h, 000h, 000h, 0B9h, 032h, 000h, 000h, 000h, 0E2h, 0FEh, 0FEh, 0CBh
        db  075h, 0F5h, 09Dh, 061h, 0C3h, 060h, 09Ch, 0CDh, 020h, 007h, 000h, 005h, 000h, 08Bh, 0DAh, 08Bh
        db  0C8h, 081h, 0C1h, 0FAh, 000h, 000h, 000h, 083h, 0D3h, 000h, 0CDh, 020h, 007h, 000h, 005h, 000h
        db  03Bh, 0D3h, 072h, 0F6h, 077h, 004h, 03Bh, 0C1h, 072h, 0F0h, 09Dh, 061h, 0C3h, 0CCh, 0CCh, 0CCh
sub_1_A58C:
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
        db  0CCh
sub_1_A598:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_1_A5A8:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_1_A5B8:
        db  0CDh, 020h, 03Fh, 080h, 033h, 000h
        int3
        int3
sub_1_A5C0:
        db  0CDh, 020h, 03Eh, 080h, 033h, 000h
        int3
        int3
_LTEXT ends
        end
