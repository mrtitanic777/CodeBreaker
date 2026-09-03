; irenum_pcode_02.asm -- IRENUM object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], ecx
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_2_001D
        xor    eax, eax
        jmp    NEAR PTR loc_2_0033
loc_2_001D:
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx - 1]
        jmp    NEAR PTR loc_2_0033
loc_2_0033:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 0ch], dl
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_2_0062
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_2_009F
loc_2_0062:
        mov    eax, dword ptr [ebp - 8]
        movsx  eax, byte ptr [eax]
        test   eax, eax
        je     NEAR PTR loc_2_0095
        mov    eax, dword ptr [ebp - 8]
        movsx  eax, byte ptr [eax]
        movsx  ecx, byte ptr [ebp - 0ch]
        cmp    eax, ecx
        jne    NEAR PTR loc_2_008A
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_009F
loc_2_008A:
        inc    dword ptr [ebp - 8]
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_0062
loc_2_0095:
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_2_009F
loc_2_009F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 0ch], dl
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_2_00CB
        xor    eax, eax
        jmp    NEAR PTR loc_2_0105
loc_2_00CB:
        mov    eax, dword ptr [ebp - 8]
        movsx  eax, byte ptr [eax]
        test   eax, eax
        je     NEAR PTR loc_2_00FE
        mov    eax, dword ptr [ebp - 8]
        movsx  eax, byte ptr [eax]
        movsx  ecx, byte ptr [ebp - 0ch]
        cmp    eax, ecx
        jne    NEAR PTR loc_2_00F3
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_2_0105
loc_2_00F3:
        inc    dword ptr [ebp - 8]
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_00CB
loc_2_00FE:
        xor    eax, eax
        jmp    NEAR PTR loc_2_0105
loc_2_0105:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_010A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], edx
        mov    dword ptr [ebp - 4], ecx
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_012D
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_2_0134
loc_2_012D:
        xor    eax, eax
        jmp    NEAR PTR loc_2_0168
loc_2_0134:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, eax
        shr    ecx, 2
        mov    esi, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp - 8]
        repe cmpsd
        jne    NEAR PTR loc_2_0161
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb
        jne    NEAR PTR loc_2_0161
        mov    eax, 1
        jmp    NEAR PTR loc_2_0163
loc_2_0161:
        xor    eax, eax
loc_2_0163:
        jmp    NEAR PTR loc_2_0168
loc_2_0168:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_2_016F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], ecx
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_2_018A
        jmp    NEAR PTR loc_2_01D1
loc_2_018A:
        mov    eax, dword ptr [ebp - 4]
        movsx  eax, byte ptr [eax]
        test   eax, eax
        je     NEAR PTR loc_2_01CC
        mov    eax, dword ptr [ebp - 4]
        movsx  eax, byte ptr [eax]
        cmp    eax, 61h
        jl     NEAR PTR loc_2_01C4
        mov    eax, dword ptr [ebp - 4]
        movsx  eax, byte ptr [eax]
        cmp    eax, 7ah
        jg     NEAR PTR loc_2_01C4
        mov    eax, dword ptr [ebp - 4]
        movsx  eax, byte ptr [eax]
        sub    eax, 20h
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx], al
loc_2_01C4:
        inc    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_018A
loc_2_01CC:
        jmp    NEAR PTR loc_2_01D1
loc_2_01D1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_01D6:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], edx
        mov    dword ptr [ebp - 4], ecx
        mov    edi, dword ptr [ebp - 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    edx, edi
        mov    ebx, ecx
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    esi, edx
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_021B:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], edx
        mov    dword ptr [ebp - 4], ecx
        mov    edi, dword ptr [ebp - 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    edx, edi
        mov    edi, dword ptr [ebp - 4]
        mov    esi, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_0254:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 10h], dl
        mov    dword ptr [ebp - 0ch], ecx
        mov    dword ptr [ebp - 4], 0
loc_2_026A:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_2_02B3
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp + 0ch], eax
        jle    NEAR PTR loc_2_02B3
        mov    eax, dword ptr [ebp - 0ch]
        movsx  eax, byte ptr [eax]
        movsx  ecx, byte ptr [ebp - 10h]
        cmp    eax, ecx
        jne    NEAR PTR loc_2_02AB
        mov    al, byte ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_2_02AE
loc_2_02AB:
        inc    dword ptr [ebp - 0ch]
loc_2_02AE:
        jmp    NEAR PTR loc_2_026A
loc_2_02B3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_2_02BA:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 0ch], dl
        mov    dword ptr [ebp - 8], ecx
loc_2_02C9:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        dec    dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_02EC
        mov    al, byte ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 8]
        jmp    NEAR PTR loc_2_02C9
loc_2_02EC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_2_02F3:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], edx
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_0310
loc_2_030D:
        dec    dword ptr [ebp - 4]
loc_2_0310:
        cmp    dword ptr [ebp - 4], 0
        jbe    NEAR PTR loc_2_032F
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp - 0ch]
        inc    dword ptr [ebp - 8]
        jmp    NEAR PTR loc_2_030D
loc_2_032F:
        mov    eax, dword ptr [ebp + 8]
        jmp    NEAR PTR loc_2_0337
loc_2_0337:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        db  0CCh, 0CCh
        push   ebp
        mov    ebp, esp
        sub    esp, 114h
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 54h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   80000002h
        call   sub_2_1C9C
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_2_0521
        mov    dword ptr [ebp - 100h], 0
loc_2_0374:
        mov    eax, dword ptr [ebp - 100h]
        mov    dword ptr [ebp - 114h], eax
        inc    dword ptr [ebp - 100h]
        push   50h
        lea    eax, [ebp - 50h]
        push   eax
        mov    eax, dword ptr [ebp - 114h]
        push   eax
        mov    eax, dword ptr [ebp - 54h]
        push   eax
        call   sub_2_1CBC
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_2_0515
        lea    eax, [ebp - 0ach]
        push   eax
        lea    eax, [ebp - 50h]
        push   eax
        mov    eax, dword ptr [ebp - 54h]
        push   eax
        call   sub_2_1C9C
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_2_0510
        mov    dword ptr [ebp - 104h], 50h
        lea    eax, [ebp - 104h]
        push   eax
        lea    eax, [ebp - 0fch]
        push   eax
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 0ach]
        push   eax
        call   sub_2_1C8C
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_2_0501
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 0a4h]
        call   sub_2_021B
        lea    edx, [ebp - 0fch]
        lea    ecx, [ebp - 0a4h]
        call   sub_2_01D6
        lea    eax, [ebp - 0a8h]
        push   eax
        lea    eax, [ebp - 0a4h]
        push   eax
        push   80000002h
        call   sub_2_1C9C
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_2_0501
        mov    dword ptr [ebp - 10ch], 4
        mov    dword ptr [ebp - 108h], 0
        lea    eax, [ebp - 10ch]
        push   eax
        lea    eax, [ebp - 108h]
        push   eax
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 0a8h]
        push   eax
        call   sub_2_1C8C
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_2_04F2
        cmp    dword ptr [ebp - 108h], 0
        je     NEAR PTR loc_2_04C4
        mov    dword ptr [ebp - 110h], 1ah
        mov    eax, dword ptr [ebp - 110h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   0
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 0ach]
        push   eax
        call   sub_2_1CB4
        add    esp, 18h
        jmp    NEAR PTR loc_2_04F2
loc_2_04C4:
        mov    dword ptr [ebp - 110h], 1ah
        mov    eax, dword ptr [ebp - 110h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   0
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 0ach]
        push   eax
        call   sub_2_1CB4
        add    esp, 18h
loc_2_04F2:
        mov    eax, dword ptr [ebp - 0a8h]
        push   eax
        call   sub_2_1C94
        add    esp, 4
loc_2_0501:
        mov    eax, dword ptr [ebp - 0ach]
        push   eax
        call   sub_2_1C94
        add    esp, 4
loc_2_0510:
        jmp    NEAR PTR loc_2_0374
loc_2_0515:
        mov    eax, dword ptr [ebp - 54h]
        push   eax
        call   sub_2_1C94
        add    esp, 4
loc_2_0521:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
loc_2_0530:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0ch], 0
        jne    NEAR PTR loc_2_0552
        mov    ecx, dword ptr [ebp + 8]
        call   sub_2_0C68
        jmp    NEAR PTR loc_2_055C
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_2_0552:
        mov    eax, 1
        jmp    NEAR PTR loc_2_055C
loc_2_055C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
        db  0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
sub_2_0570:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], 0
        push   1
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   3
        db  068h, 000h, 000h, 000h, 000h
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1CC4
        add    esp, 1ch
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_2_05D5
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_2_05C6
        jmp    NEAR PTR loc_2_05C6
loc_2_05C6:
        mov    eax, 1
        jmp    NEAR PTR loc_2_05DC
        db  0E9h, 007h, 000h, 000h, 000h
loc_2_05D5:
        xor    eax, eax
        jmp    NEAR PTR loc_2_05DC
loc_2_05DC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_05E1:
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 28h
        push   0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 30h]
        push   eax
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_2_1CC4
        add    esp, 1ch
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_0622
        jmp    NEAR PTR loc_2_06B8
loc_2_0622:
        cmp    dword ptr [ebp - 8], 6
        jne    NEAR PTR loc_2_0650
        lea    ecx, [ebp - 30h]
        call   sub_2_016F
        push   6
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 30h]
        call   sub_2_010A
        mov    dword ptr [ebp - 34h], eax
        cmp    dword ptr [ebp - 34h], 0
        jne    NEAR PTR loc_2_0655
loc_2_0650:
        jmp    NEAR PTR loc_2_06B8
loc_2_0655:
        mov    dword ptr [ebp - 8], 28h
        push   0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 30h]
        push   eax
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_1CC4
        add    esp, 1ch
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_068D
        jmp    NEAR PTR loc_2_06B8
loc_2_068D:
        push   1
        lea    eax, [ebp - 30h]
        push   eax
        call   sub_2_1CD4
        add    esp, 4
        push   eax
        lea    eax, [ebp - 30h]
        push   eax
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_2_1CCC
        add    esp, 1ch
        mov    dword ptr [ebp - 4], eax
loc_2_06B8:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_2_06D1
        mov    eax, 1
        jmp    NEAR PTR loc_2_06D8
        db  0E9h, 007h, 000h, 000h, 000h
loc_2_06D1:
        xor    eax, eax
        jmp    NEAR PTR loc_2_06D8
loc_2_06D8:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_06DD:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_2_0570
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_2_084A
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [ebp + 14h], eax
        jne    NEAR PTR loc_2_07C9
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_2_07AB
loc_2_071A:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_2_0731
        mov    eax, dword ptr [ebp + 10h]
        db  0A3h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_2_0739
loc_2_0731:
        mov    eax, dword ptr [ebp + 10h]
        db  0A3h, 000h, 000h, 000h, 000h
loc_2_0739:
        xor    eax, eax
        jmp    NEAR PTR loc_2_0969
loc_2_0740:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_2_075A
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_0764
loc_2_075A:
        mov    eax, 1
        jmp    NEAR PTR loc_2_0969
loc_2_0764:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp + 10h], eax
        jne    NEAR PTR loc_2_0784
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 10h]
        call   sub_2_096E
        jmp    NEAR PTR loc_2_079F
loc_2_0784:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp + 10h], eax
        jne    NEAR PTR loc_2_079F
        mov    edx, 2
        mov    ecx, dword ptr [ebp + 10h]
        call   sub_2_096E
loc_2_079F:
        xor    eax, eax
        jmp    NEAR PTR loc_2_0969
        db  0E9h, 019h, 000h, 000h, 000h
loc_2_07AB:
        cmp    dword ptr [ebp - 18h], 1
        je     NEAR PTR loc_2_071A
        cmp    dword ptr [ebp - 18h], 5
        je     NEAR PTR loc_2_0740
        jmp    NEAR PTR loc_2_07C4
loc_2_07C4:
        jmp    NEAR PTR loc_2_0845
loc_2_07C9:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp + 10h], eax
        jne    NEAR PTR loc_2_0845
        push   0
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_2_1CDC
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_0800
        mov    eax, 13h
        jmp    NEAR PTR loc_2_0969
loc_2_0800:
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [ebp - 10h], eax
        je     NEAR PTR loc_2_0816
        mov    eax, 1
        jmp    NEAR PTR loc_2_0969
loc_2_0816:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 1ch], eax
        jmp    NEAR PTR loc_2_0836
loc_2_0821:
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_2_05E1
        add    esp, 8
        jmp    NEAR PTR loc_2_0845
loc_2_0836:
        cmp    dword ptr [ebp - 1ch], 1
        je     NEAR PTR loc_2_0821
        jmp    NEAR PTR loc_2_0845
loc_2_0845:
        jmp    NEAR PTR loc_2_095F
loc_2_084A:
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [ebp + 14h], eax
        jne    NEAR PTR loc_2_095F
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_2_0937
loc_2_0861:
        mov    eax, dword ptr [ebp + 10h]
        db  0A3h, 000h, 000h, 000h, 000h
        xor    eax, eax
        jmp    NEAR PTR loc_2_0969
loc_2_0870:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0fffffffh
        cmp    eax, 7
        jne    NEAR PTR loc_2_089B
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0886:
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    word ptr [eax], 0
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_089B:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_08C3
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_2_0AD6
        add    esp, 4
        jmp    NEAR PTR loc_2_08FD
loc_2_08C3:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_2_08E0
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_1CAC
        add    esp, 8
loc_2_08E0:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_2_08FD
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_1CAC
        add    esp, 8
loc_2_08FD:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_2_0917
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_0921
loc_2_0917:
        mov    eax, 1
        jmp    NEAR PTR loc_2_0969
loc_2_0921:
        xor    edx, edx
        mov    ecx, dword ptr [ebp + 10h]
        call   sub_2_096E
        xor    eax, eax
        jmp    NEAR PTR loc_2_0969
        db  0E9h, 023h, 000h, 000h, 000h
loc_2_0937:
        cmp    dword ptr [ebp - 20h], 1
        je     NEAR PTR loc_2_0861
        cmp    dword ptr [ebp - 20h], 5
        je     NEAR PTR loc_2_089B
        cmp    dword ptr [ebp - 20h], 8
        je     NEAR PTR loc_2_0870
        jmp    NEAR PTR loc_2_095A
loc_2_095A:
        jmp    NEAR PTR loc_2_095F
loc_2_095F:
        mov    eax, 1
        jmp    NEAR PTR loc_2_0969
loc_2_0969:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_096E:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 20h], edx
        mov    dword ptr [ebp - 1ch], ecx
        db  0C7h, 045h, 0E8h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 10h], 0
        call   sub_2_19CB
        mov    dword ptr [ebp - 8], eax
        push   0
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_2_1CEC
        add    esp, 8
        cmp    dword ptr [ebp - 20h], 0
        jne    NEAR PTR loc_2_09B7
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_2_0AD6
        add    esp, 4
loc_2_09B7:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_2_09C6
        jmp    NEAR PTR loc_2_0AA8
loc_2_09C6:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_2_09DC
loc_2_09D2:
        inc    dword ptr [ebp - 4]
        add    dword ptr [ebp - 18h], 0c0h
loc_2_09DC:
        cmp    dword ptr [ebp - 4], 8
        jae    NEAR PTR loc_2_0AA8
        mov    eax, dword ptr [ebp - 18h]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_2_0AA8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_1AFB
        add    esp, 4
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 18h]
        cmp    dword ptr [eax + 6ch], 0
        je     NEAR PTR loc_2_0A9B
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffff66h
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 20h], 0
        jne    NEAR PTR loc_2_0A39
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_2_0A75
loc_2_0A39:
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_2_0A57
        test   byte ptr [ebp - 0ch], 10h
        je     NEAR PTR loc_2_0A57
        test   byte ptr [ebp - 20h], 1
        jne    NEAR PTR loc_2_0A75
loc_2_0A57:
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_2_0A96
        test   byte ptr [ebp - 0ch], 8
        je     NEAR PTR loc_2_0A96
        test   byte ptr [ebp - 20h], 2
        je     NEAR PTR loc_2_0A96
loc_2_0A75:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_2_0CC5
        add    esp, 8
loc_2_0A96:
        jmp    NEAR PTR loc_2_0AA0
loc_2_0A9B:
        jmp    NEAR PTR loc_2_09D2
loc_2_0AA0:
        inc    dword ptr [ebp - 10h]
        jmp    NEAR PTR loc_2_09D2
loc_2_0AA8:
        push   0
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_2_1CE4
        add    esp, 8
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_2_0ACA
        mov    eax, 1
        jmp    NEAR PTR loc_2_0ACC
loc_2_0ACA:
        xor    eax, eax
loc_2_0ACC:
        jmp    NEAR PTR loc_2_0AD1
loc_2_0AD1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_0AD6:
        push   ebp
        mov    ebp, esp
        sub    esp, 110h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_2_0AF1
loc_2_0AEE:
        inc    dword ptr [ebp - 8]
loc_2_0AF1:
        cmp    dword ptr [ebp - 8], 1
        jg     NEAR PTR loc_2_0C59
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 10ch]
        call   sub_2_021B
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_2_0B2A
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 10ch]
        call   sub_2_01D6
        jmp    NEAR PTR loc_2_0B3A
loc_2_0B2A:
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 10ch]
        call   sub_2_01D6
loc_2_0B3A:
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 10ch]
        push   eax
        lea    eax, [ebp - 110h]
        push   eax
        call   sub_2_1CF4
        add    esp, 10h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_0B68
        jmp    NEAR PTR loc_2_0AEE
loc_2_0B68:
        push   0
        push   5
        db  068h, 000h, 000h, 000h, 000h
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        mov    eax, dword ptr [ebp - 110h]
        push   eax
        call   sub_2_1CCC
        add    esp, 1ch
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_2_0BF6
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_1CD4
        add    esp, 4
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        mov    eax, dword ptr [ebp - 110h]
        push   eax
        call   sub_2_1CCC
        add    esp, 1ch
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_1CD4
        add    esp, 4
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        mov    eax, dword ptr [ebp - 110h]
        push   eax
        call   sub_2_1CCC
        add    esp, 1ch
        jmp    NEAR PTR loc_2_0C54
loc_2_0BF6:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_1CD4
        add    esp, 4
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        mov    eax, dword ptr [ebp - 110h]
        push   eax
        call   sub_2_1CCC
        add    esp, 1ch
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_2_1CD4
        add    esp, 4
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        mov    eax, dword ptr [ebp - 110h]
        push   eax
        call   sub_2_1CCC
        add    esp, 1ch
loc_2_0C54:
        jmp    NEAR PTR loc_2_0AEE
loc_2_0C59:
        mov    eax, 1
        jmp    NEAR PTR loc_2_0C63
loc_2_0C63:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_0C68:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], ecx
        push   1
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_1D04
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_0C99
        jmp    NEAR PTR loc_2_0CB8
loc_2_0C99:
        push   2
        push   0
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_2_1CFC
        add    esp, 10h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_0CB8
loc_2_0CB8:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_0CC0
loc_2_0CC0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_0CC5:
        push   ebp
        mov    ebp, esp
        sub    esp, 120h
        push   ebx
        push   esi
        push   edi
        lea    ecx, [ebp - 108h]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        call   sub_2_021B
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 70h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_2_0D0B
        mov    edx, dword ptr [ebp + 8]
        lea    ecx, [ebp - 108h]
        add    edx, 70h
        call   sub_2_01D6
        jmp    NEAR PTR loc_2_0D1B
loc_2_0D0B:
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 108h]
        call   sub_2_01D6
loc_2_0D1B:
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        lea    eax, [ebp - 108h]
        push   eax
        lea    eax, [ebp - 110h]
        push   eax
        call   sub_2_1CF4
        add    esp, 10h
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 2
        jne    NEAR PTR loc_2_0D4C
        jmp    NEAR PTR loc_2_0FF7
loc_2_0D4C:
        cmp    dword ptr [ebp - 8], 10h
        jne    NEAR PTR loc_2_0D5B
        jmp    NEAR PTR loc_2_0FF7
loc_2_0D5B:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_2_0D6A
        jmp    NEAR PTR loc_2_0FF7
loc_2_0D6A:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_2_0D9F
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 10h
        push   eax
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        mov    eax, dword ptr [ebp - 110h]
        push   eax
        call   sub_2_1CCC
        add    esp, 1ch
loc_2_0D9F:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 6ch], 0
        je     NEAR PTR loc_2_0DD4
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 6ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 70h
        push   eax
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        mov    eax, dword ptr [ebp - 110h]
        push   eax
        call   sub_2_1CCC
        add    esp, 1ch
loc_2_0DD4:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 88h], 0
        je     NEAR PTR loc_2_0E50
        push   8
        db  0BAh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 8ch
        call   sub_2_010A
        test   eax, eax
        je     NEAR PTR loc_2_0E23
        push   9
        db  0BAh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 8ch
        call   sub_2_02F3
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 88h], 8
loc_2_0E23:
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 88h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 8ch
        push   eax
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        mov    eax, dword ptr [ebp - 110h]
        push   eax
        call   sub_2_1CCC
        add    esp, 1ch
loc_2_0E50:
        test   byte ptr [ebp + 0ch], 2
        je     NEAR PTR loc_2_0FA8
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 4], 8
        je     NEAR PTR loc_2_0FA8
        mov    edx, dword ptr [ebp + 8]
        lea    ecx, [ebp - 108h]
        add    edx, 0a0h
        call   sub_2_021B
        push   1ch
        db  0BAh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 10h
        call   sub_2_010A
        test   eax, eax
        je     NEAR PTR loc_2_0EB1
        push   0fh
        db  0BAh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 10h
        call   sub_2_02F3
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0ch], 0eh
loc_2_0EB1:
        mov    edx, dword ptr [ebp + 8]
        lea    ecx, [ebp - 108h]
        add    edx, 10h
        call   sub_2_01D6
        mov    dword ptr [ebp - 11ch], 0
        push   0
        lea    eax, [ebp - 11ch]
        push   eax
        lea    ecx, [ebp - 108h]
        call   0
        push   eax
        lea    eax, [ebp - 108h]
        push   eax
        call   sub_2_1D14
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_2_0F02
        mov    dword ptr [ebp - 11ch], 0deadh
loc_2_0F02:
        mov    eax, dword ptr [ebp - 11ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 118h]
        push   eax
        call   sub_2_1D0C
        add    esp, 0ch
        lea    ecx, [ebp - 108h]
        call   0
        mov    dword ptr [ebp - 120h], eax
        cmp    dword ptr [ebp - 120h], 14h
        jbe    NEAR PTR loc_2_0F45
        mov    dword ptr [ebp - 120h], 14h
loc_2_0F45:
        mov    eax, dword ptr [ebp - 120h]
        mov    byte ptr [ebp + eax - 108h], 0
        mov    eax, dword ptr [ebp - 120h]
        push   eax
        push   5fh
        lea    ecx, [ebp - 108h]
        mov    edx, 20h
        call   sub_2_0254
        lea    edx, [ebp - 118h]
        lea    ecx, [ebp - 108h]
        call   sub_2_01D6
        push   0
        mov    eax, dword ptr [ebp - 120h]
        add    eax, 4
        push   eax
        lea    eax, [ebp - 108h]
        push   eax
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        mov    eax, dword ptr [ebp - 110h]
        push   eax
        call   sub_2_1CCC
        add    esp, 1ch
loc_2_0FA8:
        mov    dword ptr [ebp - 4], 100h
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 108h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_2_1B47
        add    esp, 0ch
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_0FF7
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    eax, [ebp - 108h]
        push   eax
        push   3
        db  068h, 000h, 000h, 000h, 000h
        push   0
        mov    eax, dword ptr [ebp - 110h]
        push   eax
        call   sub_2_1CCC
        add    esp, 1ch
loc_2_0FF7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0fffffff8h
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        dec    eax
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        test   ecx, eax
        je     NEAR PTR loc_2_1037
        jmp    NEAR PTR loc_2_10CA
loc_2_1037:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   al, 8
        je     NEAR PTR loc_2_1068
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], 8
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        push   eax
        db  0BAh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_2_02F3
        jmp    NEAR PTR loc_2_10CA
loc_2_1068:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   al, 10h
        je     NEAR PTR loc_2_1099
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], 6
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        push   eax
        db  0BAh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_2_02F3
        jmp    NEAR PTR loc_2_10CA
loc_2_1099:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   al, 40h
        je     NEAR PTR loc_2_10CA
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], 4
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        push   eax
        db  0BAh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_2_02F3
        jmp    NEAR PTR loc_2_10CA
loc_2_10CA:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  0CCh
loc_2_10D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        db  0B8h, 000h, 000h, 000h, 000h
        add    eax, 4
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*8]
        add    eax, 4
        push   eax
        db  0B9h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp + 0ch]
        call   sub_2_02F3
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_1115:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        db  0B8h, 000h, 000h, 000h, 000h
        add    eax, 4
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 10h], 0
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    NEAR PTR loc_2_1148
        mov    eax, dword ptr [ebp - 10h]
        jmp    NEAR PTR loc_2_1707
loc_2_1148:
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_2_115B
loc_2_1154:
        inc    dword ptr [ebp - 8]
        add    dword ptr [ebp - 0ch], 29h
loc_2_115B:
        xor    eax, eax
        db  0A0h, 000h, 000h, 000h, 000h
        cmp    eax, dword ptr [ebp - 8]
        jle    NEAR PTR loc_2_16FF
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        cmp    eax, dword ptr [ebp - 8]
        jle    NEAR PTR loc_2_16FF
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        test   al, 4
        je     NEAR PTR loc_2_11A5
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 6
        db  0C7h, 084h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_2_1154
loc_2_11A5:
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        test   al, 1
        je     NEAR PTR loc_2_16E9
        mov    dword ptr [ebp - 14h], 0
loc_2_11BF:
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 6
        db  0C7h, 084h, 040h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        shl    eax, 8
        mov    al, 1
        and    eax, 0ffffh
        db  0A3h, 018h, 000h, 000h, 000h
        db  0C7h, 005h, 01Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        db  0A3h, 020h, 000h, 000h, 000h
        db  0C7h, 005h, 024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 028h, 000h, 000h, 000h, 0C8h, 0AFh, 000h, 000h
        db  0C6h, 005h, 02Ch, 000h, 000h, 000h, 004h
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 003h
        db  0C7h, 005h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 005h, 000h, 000h, 000h, 008h
        db  0C7h, 005h, 006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 02Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 033h, 000h, 000h, 000h, 00Ah, 001h
        db  0C7h, 005h, 035h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0FFh, 00Fh, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1264:
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 2
        jne    NEAR PTR loc_2_12FB
loc_2_1274:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_2_12A8
        db  081h, 03Dh, 000h, 000h, 000h, 000h, 0FFh, 00Fh, 000h, 000h
        je     NEAR PTR loc_2_1296
        jmp    NEAR PTR loc_2_12A8
loc_2_1296:
        push   0afc8h
        call   sub_2_1C2F
        add    esp, 4
        jmp    NEAR PTR loc_2_1274
loc_2_12A8:
        db  081h, 03Dh, 000h, 000h, 000h, 000h, 0FEh, 00Fh, 000h, 000h
        jne    NEAR PTR loc_2_12D3
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_2_12D3
        mov    dword ptr [ebp - 14h], 1
        jmp    NEAR PTR loc_2_11BF
        db  0E9h, 023h, 000h, 000h, 000h
loc_2_12D3:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_2_12F6
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 6
        db  0C7h, 084h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_2_1154
loc_2_12F6:
        jmp    NEAR PTR loc_2_1311
loc_2_12FB:
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 6
        db  0C7h, 084h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_2_1154
loc_2_1311:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        shl    eax, 8
        mov    al, 2
        and    eax, 0ffffh
        db  0A3h, 018h, 000h, 000h, 000h
        db  0C7h, 005h, 01Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        db  0A3h, 020h, 000h, 000h, 000h
        db  0C7h, 005h, 024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 028h, 000h, 000h, 000h, 0C8h, 0AFh, 000h, 000h
        db  0C6h, 005h, 02Ch, 000h, 000h, 000h, 004h
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 003h
        db  0C7h, 005h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 005h, 000h, 000h, 000h, 004h
        db  0C7h, 005h, 006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 02Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 033h, 000h, 000h, 000h, 00Ah, 001h
        db  0C7h, 005h, 035h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0FFh, 00Fh, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_13A5:
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 2
        jne    NEAR PTR loc_2_13EE
loc_2_13B5:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_2_13E9
        db  081h, 03Dh, 000h, 000h, 000h, 000h, 0FFh, 00Fh, 000h, 000h
        je     NEAR PTR loc_2_13D7
        jmp    NEAR PTR loc_2_13E9
loc_2_13D7:
        push   0afc8h
        call   sub_2_1C2F
        add    esp, 4
        jmp    NEAR PTR loc_2_13B5
loc_2_13E9:
        jmp    NEAR PTR loc_2_1404
loc_2_13EE:
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 6
        db  0C7h, 084h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_2_1154
loc_2_1404:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        shl    eax, 8
        mov    al, 4
        and    eax, 0ffffh
        db  0A3h, 018h, 000h, 000h, 000h
        db  0C7h, 005h, 01Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        db  0A3h, 020h, 000h, 000h, 000h
        db  0C7h, 005h, 024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 028h, 000h, 000h, 000h, 0C8h, 0AFh, 000h, 000h
        db  0C6h, 005h, 02Ch, 000h, 000h, 000h, 004h
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 003h
        db  0C7h, 005h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 005h, 000h, 000h, 000h, 007h
        db  0C7h, 005h, 006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 02Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 033h, 000h, 000h, 000h, 00Ah, 001h
        db  0C7h, 005h, 035h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0FFh, 00Fh, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1498:
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 2
        jne    NEAR PTR loc_2_14E1
loc_2_14A8:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_2_14DC
        db  081h, 03Dh, 000h, 000h, 000h, 000h, 0FFh, 00Fh, 000h, 000h
        je     NEAR PTR loc_2_14CA
        jmp    NEAR PTR loc_2_14DC
loc_2_14CA:
        push   0afc8h
        call   sub_2_1C2F
        add    esp, 4
        jmp    NEAR PTR loc_2_14A8
loc_2_14DC:
        jmp    NEAR PTR loc_2_14F7
loc_2_14E1:
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 6
        db  0C7h, 084h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_2_1154
loc_2_14F7:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        shl    eax, 8
        mov    al, 5
        and    eax, 0ffffh
        db  0A3h, 018h, 000h, 000h, 000h
        db  0C7h, 005h, 01Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        db  0A3h, 020h, 000h, 000h, 000h
        db  0C7h, 005h, 024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 028h, 000h, 000h, 000h, 0C8h, 0AFh, 000h, 000h
        db  0C6h, 005h, 02Ch, 000h, 000h, 000h, 004h
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 003h
        db  0C7h, 005h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 005h, 000h, 000h, 000h, 007h
        db  0C7h, 005h, 006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 02Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 033h, 000h, 000h, 000h, 00Ah, 001h
        db  0C7h, 005h, 035h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0FFh, 00Fh, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_158B:
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 2
        jne    NEAR PTR loc_2_15D4
loc_2_159B:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_2_15CF
        db  081h, 03Dh, 000h, 000h, 000h, 000h, 0FFh, 00Fh, 000h, 000h
        je     NEAR PTR loc_2_15BD
        jmp    NEAR PTR loc_2_15CF
loc_2_15BD:
        push   0afc8h
        call   sub_2_1C2F
        add    esp, 4
        jmp    NEAR PTR loc_2_159B
loc_2_15CF:
        jmp    NEAR PTR loc_2_15EA
loc_2_15D4:
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 6
        db  0C7h, 084h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_2_1154
loc_2_15EA:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        shl    eax, 8
        mov    al, 6
        and    eax, 0ffffh
        db  0A3h, 018h, 000h, 000h, 000h
        db  0C7h, 005h, 01Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        db  0A3h, 020h, 000h, 000h, 000h
        db  0C7h, 005h, 024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 028h, 000h, 000h, 000h, 0C8h, 0AFh, 000h, 000h
        db  0C6h, 005h, 02Ch, 000h, 000h, 000h, 004h
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 003h
        db  0C7h, 005h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 005h, 000h, 000h, 000h, 00Ch
        db  0C7h, 005h, 006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 02Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 033h, 000h, 000h, 000h, 00Ah, 001h
        db  0C7h, 005h, 035h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0FFh, 00Fh, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_167E:
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 2
        jne    NEAR PTR loc_2_16C7
loc_2_168E:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_2_16C2
        db  081h, 03Dh, 000h, 000h, 000h, 000h, 0FFh, 00Fh, 000h, 000h
        je     NEAR PTR loc_2_16B0
        jmp    NEAR PTR loc_2_16C2
loc_2_16B0:
        push   0afc8h
        call   sub_2_1C2F
        add    esp, 4
        jmp    NEAR PTR loc_2_168E
loc_2_16C2:
        jmp    NEAR PTR loc_2_16DD
loc_2_16C7:
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 6
        db  0C7h, 084h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_2_1154
loc_2_16DD:
        mov    dword ptr [ebp - 10h], 1
        jmp    NEAR PTR loc_2_16FA
loc_2_16E9:
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 6
        db  0C7h, 084h, 040h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_16FA:
        jmp    NEAR PTR loc_2_1154
loc_2_16FF:
        mov    eax, dword ptr [ebp - 10h]
        jmp    NEAR PTR loc_2_1707
loc_2_1707:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_170C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_1721:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_1742
loc_2_1735:
        mov    eax, dword ptr [ebp + 0ch]
        db  0A3h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_2_17AB
loc_2_1742:
        dec    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 2eh
        ja     NEAR PTR loc_2_17AB
        mov    eax, dword ptr [ebp - 4]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h, 005h, 005h, 005h, 005h, 005h, 005h
        db  005h, 005h, 005h, 005h, 005h, 005h, 001h, 005h, 002h, 005h, 005h, 005h, 005h, 005h, 005h, 005h
        db  005h, 005h, 003h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h
        db  005h, 005h, 005h, 005h, 005h, 004h, 004h
loc_2_17AB:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_17B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_2_17BB:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 18h]
        and    eax, 0ffffh
        shr    eax, 8
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 18h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp + 8], 0
        je     NEAR PTR loc_2_1814
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp - 4], 1
        jne    NEAR PTR loc_2_180F
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0FEh, 00Fh, 000h, 000h
loc_2_180F:
        jmp    NEAR PTR loc_2_19C6
loc_2_1814:
        xor    eax, eax
        db  0A0h, 008h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        db  0B8h, 000h, 000h, 000h, 000h
        add    eax, 9
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_2_198D
loc_2_1834:
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, 10h
        jl     NEAR PTR loc_2_1845
        mov    eax, 10h
loc_2_1845:
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        shl    eax, 6
        mov    edx, dword ptr [ebp - 10h]
        db  08Dh, 08Ch, 040h, 070h, 000h, 000h, 000h
        call   sub_2_02F3
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0ch]
        shl    ecx, 6
        db  089h, 084h, 049h, 06Ch, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0ch]
        shl    ecx, 6
        lea    ecx, [ecx + ecx*2]
        db  0C6h, 084h, 008h, 071h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_2_19BC
loc_2_1884:
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, 5ch
        jl     NEAR PTR loc_2_1895
        mov    eax, 5ch
loc_2_1895:
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        shl    eax, 6
        mov    edx, dword ptr [ebp - 10h]
        db  08Dh, 08Ch, 040h, 010h, 000h, 000h, 000h
        call   sub_2_02F3
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0ch]
        shl    ecx, 6
        db  089h, 084h, 049h, 00Ch, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0ch]
        shl    ecx, 6
        lea    ecx, [ecx + ecx*2]
        db  0C6h, 084h, 008h, 011h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_2_19BC
loc_2_18D4:
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, 4
        jl     NEAR PTR loc_2_18E5
        mov    eax, 4
loc_2_18E5:
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        shl    eax, 6
        mov    edx, dword ptr [ebp - 10h]
        db  08Dh, 08Ch, 040h, 084h, 000h, 000h, 000h
        call   sub_2_02F3
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0ch]
        shl    ecx, 6
        db  089h, 084h, 049h, 080h, 000h, 000h, 000h
        jmp    NEAR PTR loc_2_19BC
loc_2_1910:
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, 10h
        jl     NEAR PTR loc_2_1921
        mov    eax, 10h
loc_2_1921:
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        shl    eax, 6
        mov    edx, dword ptr [ebp - 10h]
        db  08Dh, 08Ch, 040h, 08Ch, 000h, 000h, 000h
        call   sub_2_02F3
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0ch]
        shl    ecx, 6
        db  089h, 084h, 049h, 088h, 000h, 000h, 000h
        jmp    NEAR PTR loc_2_19BC
loc_2_194C:
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, 20h
        jl     NEAR PTR loc_2_195D
        mov    eax, 20h
loc_2_195D:
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        shl    eax, 6
        mov    edx, dword ptr [ebp - 10h]
        db  08Dh, 08Ch, 040h, 0A0h, 000h, 000h, 000h
        call   sub_2_02F3
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0ch]
        shl    ecx, 6
        db  089h, 084h, 049h, 09Ch, 000h, 000h, 000h
        jmp    NEAR PTR loc_2_19BC
        db  0E9h, 02Fh, 000h, 000h, 000h
loc_2_198D:
        dec    dword ptr [ebp - 14h]
        cmp    dword ptr [ebp - 14h], 5
        ja     NEAR PTR loc_2_19BC
        mov    eax, dword ptr [ebp - 14h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_19BC:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_19C6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_19CB:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 8], 0
        call   sub_2_1BB5
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_19F2:
        add    esp, 4
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_2_1A0A
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_2_1AF6
loc_2_1A0A:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1A1B:
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_2_1A36
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_1AD7
loc_2_1A36:
        db  068h, 000h, 000h, 000h, 000h
        push   0
        db  068h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   0ffffh
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1A52:
        add    esp, 14h
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_2_1A6D
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_1AC6
loc_2_1A6D:
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1A72:
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_2_1A8B
        mov    dword ptr [ebp - 4], 1
        jmp    NEAR PTR loc_2_1AC6
loc_2_1A8B:
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*8]
        add    eax, 4
        push   eax
        db  0B9h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 10h]
        call   sub_2_02F3
        call   sub_2_1115
        test   eax, eax
        jne    NEAR PTR loc_2_1AC6
        mov    dword ptr [ebp - 4], 1
        jmp    NEAR PTR loc_2_1AC6
loc_2_1AC6:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1AD1:
        add    esp, 4
        mov    dword ptr [ebp - 0ch], eax
loc_2_1AD7:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1AE8:
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_1AF6
loc_2_1AF6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_1AFB:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        cmp    eax, dword ptr [ebp + 8]
        jae    NEAR PTR loc_2_1B1C
        xor    eax, eax
        jmp    NEAR PTR loc_2_1B42
loc_2_1B1C:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        db  005h, 000h, 000h, 000h, 000h
        add    eax, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        jmp    NEAR PTR loc_2_1B42
loc_2_1B42:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_1B47:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        cmp    eax, dword ptr [ebp + 8]
        jae    NEAR PTR loc_2_1B66
        jmp    NEAR PTR loc_2_1BB0
loc_2_1B66:
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [eax], 29h
        jae    NEAR PTR loc_2_1B80
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], 0
        jmp    NEAR PTR loc_2_1BB0
loc_2_1B80:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        db  005h, 000h, 000h, 000h, 000h
        add    eax, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], 29h
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_2_02F3
loc_2_1BB0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_1BB5:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 020h, 000h, 000h, 000h
        db  0C7h, 005h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 014h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 01Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        push   71h
        db  0B9h, 000h, 000h, 000h, 000h
        xor    edx, edx
        call   sub_2_02BA
        push   600h
        db  0B9h, 000h, 000h, 000h, 000h
        xor    edx, edx
        call   sub_2_02BA
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_1C2F:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1C3D:
        add    eax, dword ptr [ebp + 8]
        adc    edx, 0
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 8], edx
loc_2_1C49:
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1C4E:
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 4], edx
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 4], eax
        ja     NEAR PTR loc_2_1C78
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 4], eax
        jne    NEAR PTR loc_2_1C7D
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp - 0ch], eax
        jb     NEAR PTR loc_2_1C7D
loc_2_1C78:
        jmp    NEAR PTR loc_2_1C87
loc_2_1C7D:
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1C82:
        jmp    NEAR PTR loc_2_1C49
loc_2_1C87:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_1C8C:
        db  0CDh, 020h, 051h, 081h, 001h, 000h
        int3
        int3
sub_2_1C94:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        int3
        int3
sub_2_1C9C:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        int3
        int3
sub_2_1CA4:
        db  0CDh, 020h, 0CFh, 080h, 001h, 000h
        int3
        int3
sub_2_1CAC:
        db  0CDh, 020h, 033h, 080h, 033h, 000h
        int3
        int3
sub_2_1CB4:
        db  0CDh, 020h, 052h, 081h, 001h, 000h
        int3
        int3
sub_2_1CBC:
        db  0CDh, 020h, 04Ch, 081h, 001h, 000h
        int3
        int3
sub_2_1CC4:
        db  0CDh, 020h, 03Eh, 080h, 033h, 000h
        int3
        int3
sub_2_1CCC:
        db  0CDh, 020h, 03Fh, 080h, 033h, 000h
        int3
        int3
sub_2_1CD4:
        db  0CDh, 020h, 07Ch, 081h, 001h, 000h
        int3
        int3
sub_2_1CDC:
        db  0CDh, 020h, 003h, 080h, 033h, 000h
        int3
        int3
sub_2_1CE4:
        db  0CDh, 020h, 037h, 080h, 033h, 000h
        int3
        int3
sub_2_1CEC:
        db  0CDh, 020h, 035h, 080h, 033h, 000h
        int3
        int3
sub_2_1CF4:
        db  0CDh, 020h, 00Bh, 080h, 033h, 000h
        int3
        int3
sub_2_1CFC:
        db  0CDh, 020h, 00Eh, 080h, 033h, 000h
        int3
        int3
sub_2_1D04:
        db  0CDh, 020h, 00Fh, 080h, 033h, 000h
        int3
        int3
sub_2_1D0C:
        db  0CDh, 020h, 074h, 081h, 001h, 000h
        int3
        int3
sub_2_1D14:
        db  0CDh, 020h, 031h, 080h, 033h, 000h
_PTEXT ends
        end
