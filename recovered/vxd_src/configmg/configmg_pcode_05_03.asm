; configmg_pcode_05_03.asm -- CONFIGMG object 5 segment _PTEXT  (part 3/4)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_5_886D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        push   1
        push   dword ptr [ebp + 18h]
        call   sub_5_86BE
        mov    edi, eax
        mov    eax, 1dh
        test   edi, edi
        je     SHORT loc_5_88BA
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   0
        push   dword ptr [ebp + 8]
        push   edi
        call   sub_5_D0EC
        add    esp, 18h
        mov    ebx, eax
        mov    edx, edi
        mov    ecx, esi
        call   sub_5_8766
        mov    eax, 0
        cmp    ebx, 1
        adc    eax, -1
        and    eax, 1dh
loc_5_88BA:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
sub_5_88C1:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        lea    eax, [ebp - 8]
        push   esi
        mov    dword ptr [ebp - 10h], edx
        push   edi
        mov    esi, ecx
        push   eax
        lea    ecx, [ebp - 0ch]
        push   0
        push   ecx
        push   0
        push   dword ptr [ebp + 8]
        push   edx
        call   sub_5_D11C
        add    esp, 18h
        mov    ebx, eax
        cmp    ebx, 3f4h
        je     NEAR PTR loc_5_8ACB
        cmp    ebx, 2
        je     NEAR PTR loc_5_8ACB
        test   ebx, ebx
        je     SHORT loc_5_8918
        mov    edx, 13h
        mov    ecx, esi
        call   sub_5_731F
        mov    eax, 1dh
        jmp    NEAR PTR loc_5_8ACD
loc_5_8918:
        push   201h
        push   dword ptr [ebp - 8]
        call   sub_5_D134
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_5_8944
        mov    edx, 3
        mov    ecx, esi
        call   sub_5_731F
        mov    eax, 2
        jmp    NEAR PTR loc_5_8ACD
loc_5_8944:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   eax
        push   edi
        push   ecx
        push   0
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp - 10h]
        call   sub_5_D11C
        add    esp, 18h
        test   ebx, ebx
        je     SHORT loc_5_898B
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        jne    SHORT loc_5_8975
        push   0
        push   edi
        call   sub_5_D0C8
        add    esp, 8
loc_5_8975:
        mov    edx, 3
        mov    ecx, esi
        call   sub_5_731F
        mov    eax, 1dh
        jmp    NEAR PTR loc_5_8ACD
loc_5_898B:
        cmp    dword ptr [ebp - 0ch], 3
        je     SHORT loc_5_89BB
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        jne    SHORT loc_5_89A5
        push   0
        push   edi
        call   sub_5_D0C8
        add    esp, 8
loc_5_89A5:
        mov    edx, 4
        mov    ecx, esi
        call   sub_5_731F
        mov    eax, 1dh
        jmp    NEAR PTR loc_5_8ACD
loc_5_89BB:
        mov    eax, 400h
        cmp    dword ptr [edi], eax
        jbe    SHORT loc_5_89ED
        db  085h, 005h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_89D7
        push   0
        push   edi
        call   sub_5_D0C8
        add    esp, 8
loc_5_89D7:
        mov    edx, 9
        mov    ecx, esi
        call   sub_5_731F
        mov    eax, 1dh
        jmp    NEAR PTR loc_5_8ACD
loc_5_89ED:
        xor    ebx, ebx
        mov    edx, dword ptr [edi + 4]
        push   ebx
        mov    ecx, esi
        push   dword ptr [ebp + 0ch]
        call   sub_5_16B4
        mov    dword ptr [ebp - 4], eax
        cmp    eax, ebx
        jne    SHORT loc_5_8A21
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        jne    SHORT loc_5_8A17
        push   ebx
        push   edi
        call   sub_5_D0C8
        add    esp, 8
loc_5_8A17:
        mov    eax, 2
        jmp    NEAR PTR loc_5_8ACD
loc_5_8A21:
        mov    ecx, 8
        mov    eax, dword ptr [edi + 8]
        test   eax, eax
        je     SHORT loc_5_8A57
loc_5_8A2D:
        lea    ebx, [ecx + eax]
        cmp    dword ptr [ebp - 8], ebx
        jbe    SHORT loc_5_8A57
        sub    eax, 8
        lea    edx, [ecx + edi + 8]
        push   eax
        push   edx
        mov    edx, dword ptr [ecx + edi + 4]
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_1761
        test   eax, eax
        je     SHORT loc_5_8A79
        mov    ecx, ebx
        mov    eax, dword ptr [ebx + edi]
        test   eax, eax
        jne    SHORT loc_5_8A2D
loc_5_8A57:
        sub    ecx, dword ptr [ebp - 8]
        cmp    ecx, -4
        jne    SHORT loc_5_8A9C
        test   eax, eax
        jne    SHORT loc_5_8A9C
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        jne    SHORT loc_5_8ACB
        push   0
        push   edi
        call   sub_5_D0C8
        add    esp, 8
        jmp    SHORT loc_5_8ACB
loc_5_8A79:
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        jne    SHORT loc_5_8A8D
        push   0
        push   edi
        call   sub_5_D0C8
        add    esp, 8
loc_5_8A8D:
        lea    ecx, [ebp - 4]
        call   sub_5_16EE
        mov    eax, 2
        jmp    SHORT loc_5_8ACD
loc_5_8A9C:
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        jne    SHORT loc_5_8AB0
        push   0
        push   edi
        call   sub_5_D0C8
        add    esp, 8
loc_5_8AB0:
        lea    ecx, [ebp - 4]
        call   sub_5_16EE
        mov    edx, 9
        mov    ecx, esi
        call   sub_5_731F
        mov    eax, 1fh
        jmp    SHORT loc_5_8ACD
loc_5_8ACB:
        xor    eax, eax
loc_5_8ACD:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_5_8AD6:
        push   ebx
        push   esi
        push   edi
        mov    ebx, edx
        mov    edi, ecx
        push   0
        push   0
        xor    edx, edx
        call   sub_5_86BE
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_5_8B05
        mov    edx, esi
        mov    ecx, edi
        push   dword ptr [esp + 10h]
        push   ebx
        call   sub_5_88C1
        mov    edx, esi
        mov    ecx, edi
        call   sub_5_8766
loc_5_8B05:
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_5_8B0B:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        push   0
        push   dword ptr [ebp + 0ch]
        call   sub_5_86BE
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_5_8B2E
        xor    eax, eax
        jmp    NEAR PTR loc_5_8BC8
loc_5_8B2E:
        xor    ebx, ebx
loc_5_8B30:
        push   0
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 0ch], 100h
        push   0
        push   0
        push   0
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   ebx
        push   dword ptr [ebp - 4]
        call   sub_5_D234
        add    esp, 20h
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_5_8B73
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, esi
        push   dword ptr [ebp + 8]
        db  068h, 000h, 000h, 000h, 000h
        call   sub_5_88C1
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    SHORT loc_5_8B8F
loc_5_8B73:
        inc    ebx
        test   edi, edi
        je     SHORT loc_5_8B30
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        cmp    edi, 103h
        jne    SHORT loc_5_8BAB
        xor    eax, eax
        jmp    SHORT loc_5_8BC8
loc_5_8B8F:
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        lea    ecx, [esi + eax*4 + 30h]
        call   sub_5_15ED
        mov    eax, dword ptr [ebp - 8]
        jmp    SHORT loc_5_8BC8
loc_5_8BAB:
        mov    eax, dword ptr [ebp + 8]
        lea    ecx, [esi + eax*4 + 30h]
        call   sub_5_15ED
        mov    edx, 13h
        mov    ecx, esi
        call   sub_5_731F
        mov    eax, 1dh
loc_5_8BC8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_5_8BD1:
        sub    esp, 4
        mov    dword ptr [esp], edx
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, ecx
        mov    esi, 0ch
        test   edi, edi
        je     SHORT loc_5_8BFD
        mov    eax, dword ptr [edi + 10h]
        test   eax, eax
        je     SHORT loc_5_8BFD
loc_5_8BEE:
        mov    ecx, dword ptr [eax + 14h]
        mov    eax, dword ptr [eax + 10h]
        add    ecx, 8
        add    esi, ecx
        test   eax, eax
        jne    SHORT loc_5_8BEE
loc_5_8BFD:
        push   201h
        push   esi
        call   sub_5_D134
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_5_8C15
        xor    eax, eax
        jmp    SHORT loc_5_8C75
loc_5_8C15:
        mov    dword ptr [ebx], 400h
        test   edi, edi
        je     SHORT loc_5_8C25
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebx + 4], eax
loc_5_8C25:
        mov    ebp, 8
        test   edi, edi
        je     SHORT loc_5_8C6D
        mov    edi, dword ptr [edi + 10h]
        test   edi, edi
        je     SHORT loc_5_8C6D
loc_5_8C35:
        lea    ecx, [ebx + ebp]
        mov    eax, dword ptr [edi + 14h]
        lea    edx, [eax + 8]
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [edi + 8]
        test   edx, edx
        je     SHORT loc_5_8C4C
        mov    edx, dword ptr [edx + 4]
        jmp    SHORT loc_5_8C4F
loc_5_8C4C:
        mov    edx, dword ptr [edi + 4]
loc_5_8C4F:
        mov    dword ptr [ecx + 4], edx
        push   eax
        lea    edx, [edi + 1ch]
        add    ecx, 8
        call   sub_5_5B5D
        mov    eax, dword ptr [edi + 14h]
        mov    edi, dword ptr [edi + 10h]
        add    eax, 8
        add    ebp, eax
        test   edi, edi
        jne    SHORT loc_5_8C35
loc_5_8C6D:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax], esi
        mov    eax, ebx
loc_5_8C75:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_5_8C7D:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    edi, edx
        mov    ecx, dword ptr [esp + 14h]
        lea    edx, [esp + 0ch]
        call   sub_5_8BD1
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_5_8CC4
        push   0
        db  0BAh, 000h, 000h, 000h, 000h
        push   dword ptr [esp + 10h]
        push   ebx
        push   3
        mov    ecx, esi
        push   edi
        call   sub_5_886D
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        jne    SHORT loc_5_8CC4
        push   0
        push   ebx
        call   sub_5_D0C8
        add    esp, 8
loc_5_8CC4:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    4
sub_5_8CCD:
        sub    esp, 8
        push   ebx
        push   esi
        mov    eax, dword ptr [ecx + 4]
        push   edi
        push   ebp
        mov    esi, ecx
        xor    ebx, ebx
        cmp    byte ptr [eax], bl
        je     NEAR PTR loc_5_8D66
        push   ebx
        mov    edi, 1
        push   edi
        push   ebx
        push   edi
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_8D66
        push   ebx
        xor    edx, edx
        push   ebx
        mov    ecx, esi
        call   sub_5_86BE
        cmp    eax, ebx
        mov    ebp, eax
        je     SHORT loc_5_8D66
        db  068h, 000h, 000h, 000h, 000h
        push   ebp
        call   sub_5_D14C
        add    esp, 8
        mov    edx, ebp
        mov    ecx, esi
        call   sub_5_8766
        cmp    dword ptr [esi + 34h], ebx
        je     SHORT loc_5_8D36
        push   ebx
        mov    eax, 800h
        push   eax
        mov    ecx, esi
        push   ebx
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_8D38
loc_5_8D36:
        xor    edi, edi
loc_5_8D38:
        mov    edi, dword ptr [esi + edi*4 + 30h]
        test   edi, edi
        je     SHORT loc_5_8D66
loc_5_8D40:
        lea    eax, [esp + 10h]
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        inc    ebx
        push   eax
        call   sub_5_D0FC
        lea    edx, [esp + 1ch]
        add    esp, 0ch
        mov    ecx, esi
        push   edi
        call   sub_5_8C7D
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    SHORT loc_5_8D40
loc_5_8D66:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_5_8D6E:
        sub    esp, 4
        mov    eax, 0
        cmp    edx, 1
        push   esi
        adc    eax, -1
        lea    esi, [esp + 4]
        mov    dword ptr [esp + 4], 4
        and    eax, 200h
        test   edx, edx
        push   eax
        push   esi
        mov    esi, dword ptr [esp + 14h]
        db  0B8h, 000h, 000h, 000h, 000h
        push   esi
        push   3
        jne    SHORT loc_5_8DA5
        db  0B8h, 000h, 000h, 000h, 000h
loc_5_8DA5:
        push   eax
        xor    edx, edx
        call   sub_5_87D6
        test   eax, eax
        mov    eax, 1
        je     SHORT loc_5_8DBE
        xor    eax, eax
        mov    dword ptr [esi], 0
loc_5_8DBE:
        pop    esi
        add    esp, 4
        ret    4
loc_5_8DC5:
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        jne    SHORT loc_5_8DDC
        push   0
        push   dword ptr [esp + 8]
        call   sub_5_D0C8
        add    esp, 8
loc_5_8DDC:
        ret
sub_5_8DDD:
        push   esi
        mov    edx, dword ptr [ecx + 20h]
        mov    esi, dword ptr [edx + 24h]
        cmp    esi, ecx
        jne    SHORT loc_5_8DF0
        mov    eax, dword ptr [ecx + 28h]
        mov    dword ptr [edx + 24h], eax
        jmp    SHORT loc_5_8E0B
loc_5_8DF0:
        test   esi, esi
        je     SHORT loc_5_8E0B
loc_5_8DF4:
        mov    eax, dword ptr [esi + 28h]
        cmp    eax, ecx
        je     SHORT loc_5_8E01
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_5_8DF4
loc_5_8E01:
        test   esi, esi
        je     SHORT loc_5_8E0B
        mov    eax, dword ptr [ecx + 28h]
        mov    dword ptr [esi + 28h], eax
loc_5_8E0B:
        pop    esi
        ret
sub_5_8E0D:
        push   ebx
        push   esi
        lea    ebx, [ecx + 4ch]
        push   edi
        mov    esi, ecx
        mov    edi, 3
        call   sub_5_595F
        mov    ecx, esi
        call   sub_5_26D4
loc_5_8E26:
        mov    eax, dword ptr [ebx]
        test   eax, eax
        je     SHORT loc_5_8E40
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        jne    SHORT loc_5_8E40
        push   0
        push   eax
        call   sub_5_D0C8
        add    esp, 8
loc_5_8E40:
        add    ebx, 4
        dec    edi
        jne    SHORT loc_5_8E26
        mov    ecx, esi
        call   sub_5_8DDD
        push   0
        db  0B9h, 000h, 000h, 000h, 000h
        push   0
        mov    edx, esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_8E5D:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_8E61:
        push   ebx
        mov    ecx, 40000h
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        push   0
        push   ecx
        push   0
        push   ecx
        mov    ecx, esi
        call   sub_5_18E3
        test   eax, eax
        jne    NEAR PTR loc_5_8FD5
        push   0
        mov    ecx, esi
        push   40000h
        mov    edi, 0
        call   sub_5_1906
        db  0A1h, 000h, 000h, 000h, 000h
        test   al, 8
        je     SHORT loc_5_8EAB
        and    eax, 400h
        cmp    eax, 1
        sbb    edi, edi
        add    edi, 2
loc_5_8EAB:
        xor    ebx, ebx
        mov    edx, 8004h
        push   ebx
        mov    ecx, esi
        call   sub_5_6048
        push   ebx
        mov    edx, esi
        push   ebx
        xor    ecx, ecx
        push   -1
        push   edi
        push   4
        call   sub_5_CEA8
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 008h
        je     SHORT loc_5_8EE1
        mov    ecx, esi
        mov    dword ptr [esi], ebx
        call   sub_5_8DDD
        jmp    NEAR PTR loc_5_8FD5
loc_5_8EE1:
        mov    ecx, esi
        xor    edi, edi
        call   sub_5_1CB6
        mov    ecx, esi
        call   sub_5_0858
        push   edi
        mov    ecx, esi
        push   8
        call   sub_5_1923
        mov    edx, 15h
        mov    ecx, esi
        call   sub_5_731F
        mov    eax, 20000000h
        mov    dword ptr [esi], edi
        push   eax
        mov    ecx, esi
        push   edi
        mov    dword ptr [esi + 18h], edi
        push   eax
        mov    dword ptr [esi + 1ch], edi
        push   edi
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_8F29
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
loc_5_8F29:
        mov    eax, 1000000h
        mov    ecx, esi
        push   eax
        push   0
        push   eax
        push   0
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_8F45
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
loc_5_8F45:
        push   0
        mov    ecx, 8000h
        push   ecx
        push   0
        push   ecx
        mov    ecx, esi
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_8F61
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
loc_5_8F61:
        xor    edi, edi
        mov    eax, 1000h
        push   edi
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_8F8C
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        jne    SHORT loc_5_8F8C
        push   edi
        push   dword ptr [esi + 4]
        call   sub_5_D0C8
        add    esp, 8
loc_5_8F8C:
        lea    ebx, [esi + 30h]
        mov    edi, 7
        mov    dword ptr [esi + 4], 0
loc_5_8F9B:
        mov    ecx, ebx
        add    ebx, 4
        call   sub_5_15ED
        dec    edi
        jne    SHORT loc_5_8F9B
        mov    eax, dword ptr [esi + 68h]
        test   eax, eax
        je     SHORT loc_5_8FC3
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        jne    SHORT loc_5_8FC3
        push   0
        push   eax
        call   sub_5_D0C8
        add    esp, 8
loc_5_8FC3:
        mov    ecx, esi
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        call   sub_5_8E0D
        call   sub_5_73CA
loc_5_8FD5:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_8FDE:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    edx, 0
        test   al, 8
        je     SHORT loc_5_8FF9
        and    eax, 400h
        cmp    eax, 1
        sbb    edx, edx
        add    edx, 2
loc_5_8FF9:
        push   1
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   dword ptr [esp + 8]
        push   dword ptr [esp + 0ch]
        call   sub_5_1E4B
        mov    eax, 1
        ret
sub_5_9014:
        push   ebx
        xor    edx, edx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [esp + 14h]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        mov    eax, 1
        je     SHORT loc_5_9097
        push   0
        mov    edx, esi
        db  068h, 000h, 000h, 000h, 000h
        mov    ebx, 2
        mov    ecx, ebx
        db  0BDh, 000h, 000h, 000h, 000h
        call   sub_5_1A48
        push   ebp
        mov    edx, esi
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        mov    ecx, ebx
        call   sub_5_CBE2
        push   ebp
        mov    edx, esi
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 00Eh, 000h, 000h, 000h
        mov    ecx, 3
        call   sub_5_CBE2
        db  068h, 000h, 000h, 000h, 000h
        mov    edx, esi
        mov    ecx, ebx
        call   sub_5_CBE2
        call   sub_5_7B5F
        mov    eax, 1
        db  089h, 03Dh, 000h, 000h, 000h, 000h
loc_5_9097:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_909C:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [eax + 24h]
        test   esi, esi
        je     SHORT loc_5_9105
        xor    ebx, ebx
        mov    ebp, 200h
loc_5_90B2:
        mov    ecx, esi
        mov    edi, esi
        mov    esi, dword ptr [esi + 28h]
        push   ebx
        push   ebp
        push   ebx
        push   ebp
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_9101
        push   ebx
        mov    ecx, edi
        push   1000h
        push   ebx
        push   1000h
        call   sub_5_18E3
        test   eax, eax
        db  0A1h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_9101
        cmp    dword ptr [edi + 20h], eax
        jne    SHORT loc_5_90F8
        push   ebx
        mov    ecx, edi
        push   1
        push   ebx
        push   1
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_9101
loc_5_90F8:
        push   edi
        call   sub_5_9014
        add    esp, 4
loc_5_9101:
        test   esi, esi
        jne    SHORT loc_5_90B2
loc_5_9105:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_910A:
        push   esi
        push   0
        mov    esi, ecx
        push   1
        call   sub_5_1906
        push   3
        db  0BAh, 000h, 000h, 000h, 000h
        mov    ecx, esi
        call   sub_5_8AD6
        push   0
        db  0BAh, 000h, 000h, 000h, 000h
        push   0
        mov    ecx, esi
        call   sub_5_8B0B
        pop    esi
        ret
sub_5_9136:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        push   0
        lea    edx, [ebp - 4]
        push   eax
        push   ecx
        push   edx
        call   _CONFIGMG_Create_DevNode
        add    esp, 10h
        cmp    eax, 10h
        jne    SHORT loc_5_917E
        push   0
        mov    ecx, dword ptr [ebp - 4]
        push   8
        push   0
        push   8
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_917E
        push   1
        xor    edx, edx
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 4]
        mov    ecx, 0bh
        call   sub_5_1E4B
loc_5_917E:
        mov    esp, ebp
        pop    ebp
        ret
sub_5_9182:
        push   ebp
        mov    ebp, esp
        sub    esp, 12ch
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 010h
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_5_919E
        xor    eax, eax
        jmp    NEAR PTR loc_5_932A
loc_5_919E:
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 12ch]
        call   sub_5_C486
        lea    ecx, [ebp - 12ch]
        call   sub_5_C3C4
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 12ch]
        mov    dword ptr [ebp - 1ch], eax
        call   sub_5_C44E
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 12ch]
        push   eax
        push   ecx
        push   80000002h
        call   sub_5_D154
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_91F2
        mov    eax, 1dh
        jmp    NEAR PTR loc_5_932A
loc_5_91F2:
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 12ch]
        xor    esi, esi
        call   sub_5_C44E
        lea    ecx, [ebp - 12ch]
        call   sub_5_C3C4
        mov    edi, eax
        mov    dword ptr [ebp - 0ch], esi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 28h], eax
        mov    eax, 100h
        sub    eax, edi
        mov    dword ptr [ebp - 2ch], eax
loc_5_9224:
        push   dword ptr [ebp - 2ch]
        lea    eax, [ebp + edi - 12ch]
        push   eax
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 28h]
        call   sub_5_D16C
        add    esp, 10h
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    NEAR PTR loc_5_92FD
        lea    eax, [ebp - 10h]
        lea    ecx, [ebp - 12ch]
        push   eax
        push   ecx
        push   80000002h
        call   sub_5_D154
        add    esp, 0ch
        mov    ebx, eax
        test   ebx, ebx
        jne    NEAR PTR loc_5_9318
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 12ch]
        call   sub_5_C44E
        lea    ecx, [ebp - 12ch]
        call   sub_5_C3C4
        mov    dword ptr [ebp - 18h], eax
        test   ebx, ebx
        mov    dword ptr [ebp - 14h], 0
        jne    SHORT loc_5_92F0
        mov    eax, 100h
        mov    ecx, dword ptr [ebp - 10h]
        sub    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 20h], ecx
        mov    dword ptr [ebp - 24h], eax
loc_5_92A4:
        mov    ecx, dword ptr [ebp - 18h]
        push   dword ptr [ebp - 24h]
        lea    eax, [ebp + ecx - 12ch]
        push   eax
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 20h]
        call   sub_5_D16C
        add    esp, 10h
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_5_92D7
        mov    eax, dword ptr [ebp - 1ch]
        lea    ecx, [ebp + eax - 12ch]
        call   sub_5_9136
        jmp    SHORT loc_5_92DF
loc_5_92D7:
        cmp    ebx, 103h
        jne    SHORT loc_5_92E8
loc_5_92DF:
        inc    dword ptr [ebp - 14h]
        test   ebx, ebx
        je     SHORT loc_5_92A4
        jmp    SHORT loc_5_92F0
loc_5_92E8:
        mov    esi, 1dh
        mov    dword ptr [ebp - 8], ebx
loc_5_92F0:
        push   dword ptr [ebp - 10h]
        call   sub_5_D0E4
        add    esp, 4
        jmp    SHORT loc_5_9309
loc_5_92FD:
        cmp    eax, 103h
        je     SHORT loc_5_9309
        mov    esi, 1dh
loc_5_9309:
        inc    dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_5_9224
        jmp    SHORT loc_5_931D
loc_5_9318:
        mov    esi, 1dh
loc_5_931D:
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        mov    eax, esi
loc_5_932A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_9331:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0c8h
        cmp    eax, 5
        push   esi
        push   edi
        je     SHORT loc_5_9354
        cmp    eax, 6
        je     SHORT loc_5_937D
        mov    eax, 1
        jmp    NEAR PTR loc_5_93DB
loc_5_9354:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   _CONFIGMG_Reset_Children_Marks
        add    esp, 8
        call   sub_5_9182
        test   eax, eax
        jne    SHORT loc_5_93DB
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_5_909C
        add    esp, 4
        jmp    SHORT loc_5_93D9
loc_5_937D:
        lea    ecx, [ebp - 0c8h]
        mov    eax, dword ptr [ebp + 14h]
        mov    edx, dword ptr [eax + 4]
        call   sub_5_C486
        lea    ecx, [ebp - 0c8h]
        mov    edx, 5ch
        call   sub_5_C3E0
        lea    esi, [ebp + eax - 0c7h]
        mov    edi, eax
        mov    edx, 5ch
        mov    ecx, esi
        call   sub_5_C3E0
        add    eax, edi
        mov    ecx, esi
        mov    byte ptr [ebp + eax - 0c7h], 0
        call   sub_5_C3C4
        push   0
        xor    edx, edx
        push   eax
        mov    ecx, dword ptr [ebp + 14h]
        push   esi
        push   1
        db  068h, 000h, 000h, 000h, 000h
        call   sub_5_886D
loc_5_93D9:
        xor    eax, eax
loc_5_93DB:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_5_93E1:
        mov    ecx, dword ptr [esp + 4]
        db  0A1h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ecx + 50h]
        cmp    dword ptr [edx], eax
        jne    SHORT loc_5_93FC
        cmp    dword ptr [ecx + 0ch], 17h
        jne    SHORT loc_5_93FC
        call   sub_5_7024
loc_5_93FC:
        mov    eax, 1
        ret
loc_5_9402:
        mov    ecx, dword ptr [esp + 4]
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ecx + 18h], eax
        jne    SHORT loc_5_9420
        push   1
        xor    edx, edx
        push   ecx
        push   ecx
        mov    ecx, 7
        call   sub_5_1E4B
loc_5_9420:
        mov    eax, 1
        ret
loc_5_9426:
        mov    ecx, dword ptr [esp + 4]
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ecx + 1ch], eax
        jne    SHORT loc_5_9444
        push   0
        xor    edx, edx
        push   ecx
        push   ecx
        mov    ecx, 7
        call   sub_5_1E4B
loc_5_9444:
        mov    eax, 1
        ret
sub_5_944A:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, ecx
        inc    eax
        push   eax
        call   sub_5_5B5D
        mov    ecx, esi
        call   sub_5_21A9
        pop    esi
        ret    4
sub_5_9463:
        push   esi
        mov    edx, 1
        push   edi
        mov    esi, ecx
        call   sub_5_1AC6
        test   eax, eax
        mov    eax, 5
        je     SHORT loc_5_949B
        xor    edi, edi
        mov    ecx, esi
        push   edi
        push   20h
        call   sub_5_1906
        push   edi
        db  0B9h, 000h, 000h, 000h, 000h
        push   edi
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        push   edi
        call   sub_5_02D3
        xor    eax, eax
loc_5_949B:
        pop    edi
        pop    esi
        ret
_CONFIGMG_Get_Version:
        clc
        mov    eax, 400h
        ret
_CONFIGMG_Initialize:
        push   ebp
        mov    ebp, esp
        test   dword ptr [esp + 8], 0fffffffeh
        je     SHORT loc_5_94B9
        mov    eax, 4
        jmp    SHORT loc_5_9513
loc_5_94B9:
        cmp    dword ptr [ebp + 8], 1
        jne    SHORT loc_5_94EE
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 002h
        je     SHORT loc_5_94CC
        xor    eax, eax
        jmp    SHORT loc_5_9513
loc_5_94CC:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_94D1:
        test   eax, eax
        jne    SHORT loc_5_94DB
        db  0CDh, 020h, 0BFh, 080h, 001h, 000h
loc_5_94DB:
        xor    eax, eax
        db  0B9h, 000h, 000h, 000h, 000h
        push   eax
        xor    edx, edx
        push   eax
        push   eax
        call   sub_5_02D3
        jmp    SHORT loc_5_9511
loc_5_94EE:
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 001h
        je     SHORT loc_5_94FB
        xor    eax, eax
        jmp    SHORT loc_5_9513
loc_5_94FB:
        db  080h, 00Dh, 000h, 000h, 000h, 000h, 001h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_9507:
        test   eax, eax
        jne    SHORT loc_5_9511
        db  0CDh, 020h, 0BFh, 080h, 001h, 000h
loc_5_9511:
        xor    eax, eax
loc_5_9513:
        pop    ebp
        ret
_CONFIGMG_Locate_DevNode:
        push   ebp
        mov    ebp, esp
        sub    esp, 0cch
        push   esi
        push   edi
        push   1
        mov    esi, dword ptr [ebp + 8]
        push   0
        push   0
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_9542
        mov    eax, 3
        jmp    NEAR PTR loc_5_95C2
loc_5_9542:
        push   1
        mov    edi, dword ptr [ebp + 0ch]
        push   0
        push   0
        push   1
        push   edi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_9561
        mov    eax, 3
        jmp    SHORT loc_5_95C2
loc_5_9561:
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_5_956E
        mov    eax, 4
        jmp    SHORT loc_5_95C2
loc_5_956E:
        mov    ecx, edi
        call   sub_5_C3C4
        test   eax, eax
        jne    SHORT loc_5_9588
        test   esi, esi
        je     SHORT loc_5_9584
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi], eax
loc_5_9584:
        xor    eax, eax
        jmp    SHORT loc_5_95C2
loc_5_9588:
        cmp    eax, 0c8h
        jbe    SHORT loc_5_9596
        mov    eax, 1eh
        jmp    SHORT loc_5_95C2
loc_5_9596:
        push   eax
        lea    ecx, [ebp - 0cch]
        mov    edx, edi
        call   sub_5_944A
        lea    ecx, [ebp - 0cch]
        call   sub_5_1BDC
        test   eax, eax
        je     SHORT loc_5_95BD
        test   esi, esi
        je     SHORT loc_5_95B9
        mov    dword ptr [esi], eax
loc_5_95B9:
        xor    eax, eax
        jmp    SHORT loc_5_95C2
loc_5_95BD:
        mov    eax, 0dh
loc_5_95C2:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
_CONFIGMG_Get_Parent:
        push   esi
        xor    eax, eax
        push   edi
        push   eax
        mov    edi, dword ptr [esp + 10h]
        push   eax
        push   eax
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_95E9
        mov    eax, 3
        jmp    SHORT loc_5_9624
loc_5_95E9:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_5_95F7
        mov    eax, 4
        jmp    SHORT loc_5_9624
loc_5_95F7:
        mov    edx, 1
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        jne    SHORT loc_5_9612
        mov    eax, 5
        jmp    SHORT loc_5_9624
loc_5_9612:
        mov    eax, dword ptr [esi + 20h]
        test   eax, eax
        je     SHORT loc_5_961F
        mov    dword ptr [edi], eax
        xor    eax, eax
        jmp    SHORT loc_5_9624
loc_5_961F:
        mov    eax, 0dh
loc_5_9624:
        pop    edi
        pop    esi
        ret
_CONFIGMG_Get_Child:
        push   esi
        xor    eax, eax
        push   edi
        push   eax
        mov    edi, dword ptr [esp + 10h]
        push   eax
        push   eax
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_9648
        mov    eax, 3
        jmp    SHORT loc_5_9683
loc_5_9648:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_5_9656
        mov    eax, 4
        jmp    SHORT loc_5_9683
loc_5_9656:
        mov    edx, 1
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        jne    SHORT loc_5_9671
        mov    eax, 5
        jmp    SHORT loc_5_9683
loc_5_9671:
        mov    eax, dword ptr [esi + 24h]
        test   eax, eax
        je     SHORT loc_5_967E
        mov    dword ptr [edi], eax
        xor    eax, eax
        jmp    SHORT loc_5_9683
loc_5_967E:
        mov    eax, 0dh
loc_5_9683:
        pop    edi
        pop    esi
        ret
_CONFIGMG_Get_Sibling:
        push   esi
        xor    eax, eax
        push   edi
        push   eax
        mov    edi, dword ptr [esp + 10h]
        push   eax
        push   eax
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_96A7
        mov    eax, 3
        jmp    SHORT loc_5_96E2
loc_5_96A7:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_5_96B5
        mov    eax, 4
        jmp    SHORT loc_5_96E2
loc_5_96B5:
        mov    edx, 1
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        jne    SHORT loc_5_96D0
        mov    eax, 5
        jmp    SHORT loc_5_96E2
loc_5_96D0:
        mov    eax, dword ptr [esi + 28h]
        test   eax, eax
        je     SHORT loc_5_96DD
        mov    dword ptr [edi], eax
        xor    eax, eax
        jmp    SHORT loc_5_96E2
loc_5_96DD:
        mov    eax, 0dh
loc_5_96E2:
        pop    edi
        pop    esi
        ret
_CONFIGMG_Get_Device_ID_Size:
        push   esi
        xor    eax, eax
        mov    esi, dword ptr [esp + 8]
        push   edi
        push   eax
        push   eax
        push   eax
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_9706
        mov    eax, 3
        jmp    SHORT loc_5_9739
loc_5_9706:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_5_9714
        mov    eax, 4
        jmp    SHORT loc_5_9739
loc_5_9714:
        mov    edx, 1
        mov    edi, dword ptr [esp + 10h]
        mov    ecx, edi
        call   sub_5_1AC6
        test   eax, eax
        mov    eax, 5
        je     SHORT loc_5_9739
        mov    ecx, dword ptr [edi + 4]
        call   sub_5_C3C4
        mov    dword ptr [esi], eax
        xor    eax, eax
loc_5_9739:
        pop    edi
        pop    esi
        ret
_CONFIGMG_Get_Device_ID:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   eax
        mov    edi, dword ptr [ebp + 10h]
        push   eax
        push   eax
        push   edi
        push   dword ptr [ebp + 0ch]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_9761
        mov    eax, 3
        jmp    SHORT loc_5_97AF
loc_5_9761:
        cmp    dword ptr [ebp + 14h], 0
        je     SHORT loc_5_976E
        mov    eax, 4
        jmp    SHORT loc_5_97AF
loc_5_976E:
        mov    edx, 1
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        jne    SHORT loc_5_9788
        mov    eax, 5
        jmp    SHORT loc_5_97AF
loc_5_9788:
        mov    esi, dword ptr [esi + 4]
        mov    ecx, esi
        call   sub_5_C3C4
        mov    ebx, eax
        lea    eax, [eax + 1]
        cmp    eax, edi
        jb     SHORT loc_5_979D
        mov    eax, edi
loc_5_979D:
        push   eax
        mov    edx, esi
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_5_5B5D
        cmp    edi, ebx
        sbb    eax, eax
        and    eax, 1ah
loc_5_97AF:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
_CONFIGMG_Get_Depth:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   eax
        push   eax
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_97D6
        mov    eax, 3
        jmp    SHORT loc_5_9805
loc_5_97D6:
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_5_97E3
        mov    eax, 4
        jmp    SHORT loc_5_9805
loc_5_97E3:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_5_1AC6
        test   eax, eax
        mov    eax, 5
        je     SHORT loc_5_9805
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_5_1D31
        mov    dword ptr [esi], eax
        xor    eax, eax
loc_5_9805:
        pop    esi
        pop    ebp
        ret
_CONFIGMG_Get_Private_DWord:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   eax
        push   eax
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_982A
        mov    eax, 3
        jmp    SHORT loc_5_985C
loc_5_982A:
        cmp    dword ptr [ebp + 14h], 0
        je     SHORT loc_5_9837
        mov    eax, 4
        jmp    SHORT loc_5_985C
loc_5_9837:
        mov    edx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_5_1D72
        test   eax, eax
        mov    eax, 5
        je     SHORT loc_5_985C
        mov    edx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_5_1D46
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi], eax
        xor    eax, eax
loc_5_985C:
        pop    esi
        pop    ebp
        ret
_CONFIGMG_Set_Private_DWord:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_5_9870
        mov    eax, 4
        jmp    SHORT loc_5_9896
loc_5_9870:
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_1D72
        test   eax, eax
        mov    eax, 5
        je     SHORT loc_5_9896
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_1D46
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [eax], ecx
        xor    eax, eax
loc_5_9896:
        pop    ebp
        ret
_CONFIGMG_Create_DevNode:
        push   ebp
        mov    ebp, esp
        sub    esp, 0dch
        push   ebx
        push   esi
        push   edi
        push   1
        push   0
        push   0
        push   4
        push   dword ptr [ebp + 8]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_98C5
        mov    eax, 3
        jmp    NEAR PTR loc_5_9C40
loc_5_98C5:
        push   1
        mov    edi, dword ptr [ebp + 0ch]
        push   0
        push   0
        push   1
        push   edi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_98E7
        mov    eax, 3
        jmp    NEAR PTR loc_5_9C40
loc_5_98E7:
        test   dword ptr [ebp + 14h], 0fffffffch
        je     SHORT loc_5_98FA
        mov    eax, 4
        jmp    NEAR PTR loc_5_9C40
loc_5_98FA:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 10h]
        call   sub_5_1AC6
        test   eax, eax
        jne    SHORT loc_5_9915
        mov    eax, 5
        jmp    NEAR PTR loc_5_9C40
loc_5_9915:
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 010h
        je     SHORT loc_5_9933
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 24h], 0
        je     SHORT loc_5_9933
        mov    eax, 15h
        jmp    NEAR PTR loc_5_9C40
loc_5_9933:
        mov    ecx, edi
        call   sub_5_C3C4
        cmp    eax, 0c8h
        mov    esi, eax
        jbe    SHORT loc_5_994D
        mov    eax, 1eh
        jmp    NEAR PTR loc_5_9C40
loc_5_994D:
        test   esi, esi
        je     NEAR PTR loc_5_9A90
        push   esi
        lea    ecx, [ebp - 0dch]
        mov    edx, edi
        lea    ebx, [esi - 1]
        call   sub_5_944A
        lea    ecx, [ebp - 0dch]
        mov    edx, 5ch
        call   sub_5_C3E0
        cmp    eax, ebx
        mov    edi, eax
        sbb    ecx, ecx
        lea    eax, [edi + 1]
        inc    ecx
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        or     ecx, eax
        cmp    edi, 1
        sbb    eax, eax
        neg    eax
        or     ecx, eax
        je     SHORT loc_5_999E
        mov    eax, 1eh
        jmp    NEAR PTR loc_5_9C40
loc_5_999E:
        lea    ecx, [ebp + edi - 0dbh]
        mov    edx, 5ch
        call   sub_5_C3E0
        cmp    eax, -1
        jne    SHORT loc_5_99E8
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 0dch]
        inc    esi
        call   sub_5_C44E
        or     byte ptr [ebp + 14h], 2
loc_5_99C9:
        test   byte ptr [ebp + 14h], 2
        je     NEAR PTR loc_5_9A97
        mov    eax, dword ptr [ebp + 10h]
        mov    edi, dword ptr [eax + 4]
        cmp    byte ptr [edi], 0
        jne    SHORT loc_5_9A2B
        mov    eax, 1eh
        jmp    NEAR PTR loc_5_9C40
loc_5_99E8:
        lea    eax, [edi + eax + 1]
        mov    ecx, eax
        sub    ecx, edi
        dec    ecx
        cmp    ecx, 1
        sbb    edx, edx
        neg    edx
        cmp    eax, ebx
        sbb    ecx, ecx
        inc    ecx
        or     edx, ecx
        je     SHORT loc_5_9A0B
        mov    eax, 1eh
        jmp    NEAR PTR loc_5_9C40
loc_5_9A0B:
        lea    ecx, [ebp + eax - 0dbh]
        mov    edx, 5ch
        call   sub_5_C3E0
        cmp    eax, -1
        je     SHORT loc_5_99C9
        mov    eax, 1eh
        jmp    NEAR PTR loc_5_9C40
loc_5_9A2B:
        mov    ecx, edi
        call   sub_5_C3C4
        add    eax, esi
        inc    eax
        cmp    eax, 0c8h
        jbe    SHORT loc_5_9A46
        mov    eax, 1eh
        jmp    NEAR PTR loc_5_9C40
loc_5_9A46:
        lea    ecx, [ebp - 0dch]
        mov    edx, edi
        call   sub_5_C44E
        lea    ecx, [ebp + esi - 0dch]
        mov    edx, 5ch
        call   sub_5_C3E0
        lea    edi, [esi + eax]
        mov    ebx, eax
        mov    edx, 5ch
        lea    ecx, [ebp + edi - 0dbh]
        call   sub_5_C3E0
        add    eax, ebx
        add    eax, esi
        mov    byte ptr [ebp + edi - 0dch], 26h
        mov    byte ptr [ebp + eax - 0dbh], 26h
        jmp    SHORT loc_5_9A97
loc_5_9A90:
        mov    byte ptr [ebp - 0dch], 0
loc_5_9A97:
        lea    ecx, [ebp - 0dch]
        call   sub_5_1BDC
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_5_9ACB
        push   0
        mov    ecx, esi
        push   220h
        call   sub_5_1906
        cmp    dword ptr [ebp + 8], 0
        je     SHORT loc_5_9AC1
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], esi
loc_5_9AC1:
        mov    eax, 10h
        jmp    NEAR PTR loc_5_9C40
loc_5_9ACB:
        lea    ecx, [ebp - 0dch]
        mov    edx, dword ptr [ebp + 10h]
        call   sub_5_1C73
        mov    esi, eax
        test   esi, esi
        je     NEAR PTR loc_5_9C3B
        mov    eax, dword ptr [esi + 4]
        cmp    byte ptr [eax], 0
        je     NEAR PTR loc_5_9BEA
        push   1
        xor    edx, edx
        push   0
        mov    ecx, esi
        call   sub_5_86BE
        lea    eax, [ebp - 4]
        xor    edx, edx
        push   eax
        mov    ecx, esi
        call   sub_5_8D6E
        test   byte ptr [ebp + 14h], 1
        je     SHORT loc_5_9B2D
        test   eax, eax
        je     SHORT loc_5_9B19
        test   byte ptr [ebp - 4], 20h
        je     SHORT loc_5_9B2D
loc_5_9B19:
        push   1000000h
        mov    ecx, esi
        push   0
        call   sub_5_1906
        db  0FFh, 005h, 000h, 000h, 000h, 000h
loc_5_9B2D:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 24h], esi
        je     NEAR PTR loc_5_9BEA
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 008h
        jne    SHORT loc_5_9B48
        or     byte ptr [ebp - 4], 10h
loc_5_9B48:
        mov    edi, 4
        cmp    dword ptr [ebp + 10h], eax
        mov    dword ptr [ebp - 0ch], edi
        jne    SHORT loc_5_9BBA
        push   5
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 0dch]
        call   sub_5_C3FC
        test   eax, eax
        je     SHORT loc_5_9BBA
        xor    ebx, ebx
        lea    eax, [ebp - 0ch]
        push   ebx
        lea    ecx, [ebp - 8]
        push   eax
        xor    edx, edx
        push   ecx
        push   3
        db  068h, 000h, 000h, 000h, 000h
        mov    ecx, esi
        call   sub_5_87D6
        test   eax, eax
        jne    SHORT loc_5_9BBA
        test   byte ptr [ebp - 5], 80h
        je     SHORT loc_5_9BBA
        push   2000000h
        mov    ecx, esi
        push   ebx
        call   sub_5_1906
        push   ebx
        lea    eax, [ebp - 8]
        push   edi
        xor    edx, edx
        push   eax
        mov    ecx, esi
        push   3
        mov    dword ptr [ebp - 0ch], edi
        and    byte ptr [ebp - 5], 7fh
        db  068h, 000h, 000h, 000h, 000h
        call   sub_5_886D
loc_5_9BBA:
        lea    eax, [ebp - 10h]
        mov    edx, 1
        push   eax
        mov    ecx, esi
        call   sub_5_8D6E
        test   byte ptr [ebp - 10h], 2
        jne    SHORT loc_5_9BDC
        test   byte ptr [ebp - 4], 2
        jne    SHORT loc_5_9BDC
        test   byte ptr [ebp - 4], 10h
        jne    SHORT loc_5_9BEA
loc_5_9BDC:
        mov    ecx, esi
        call   sub_5_8E0D
        mov    eax, 15h
        jmp    SHORT loc_5_9C40
loc_5_9BEA:
        mov    ecx, esi
        call   sub_5_2654
        push   0
        mov    ecx, esi
        push   220h
        call   sub_5_1906
        mov    eax, dword ptr [ebp + 10h]
        db  03Bh, 005h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_9C27
        push   5
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 0dch]
        call   sub_5_C3FC
        test   eax, eax
        je     SHORT loc_5_9C27
        mov    ecx, esi
        call   sub_5_910A
loc_5_9C27:
        call   sub_5_7B5F
        cmp    dword ptr [ebp + 8], 0
        je     SHORT loc_5_9C37
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], esi
loc_5_9C37:
        xor    eax, eax
        jmp    SHORT loc_5_9C40
loc_5_9C3B:
        mov    eax, 2
loc_5_9C40:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
_CONFIGMG_Query_Remove_SubTree:
        test   dword ptr [esp + 8], 0fffffffeh
        push   esi
        push   edi
        je     SHORT loc_5_9C5A
        mov    eax, 4
        jmp    SHORT loc_5_9CB0
loc_5_9C5A:
        mov    edx, 1
        mov    esi, dword ptr [esp + 0ch]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     SHORT loc_5_9CAB
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_9CAB
        mov    edi, dword ptr [esp + 10h]
        and    edi, 1
        je     SHORT loc_5_9C85
        db  0FFh, 005h, 000h, 000h, 000h, 000h
loc_5_9C85:
        push   13h
        db  0B9h, 000h, 000h, 000h, 000h
        push   0
        mov    edx, esi
        push   0
        call   sub_5_02D3
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 17h
        test   edi, edi
        je     SHORT loc_5_9CB0
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_5_9CB0
loc_5_9CAB:
        mov    eax, 5
loc_5_9CB0:
        pop    edi
        pop    esi
        ret
_CONFIGMG_Remove_SubTree:
        test   dword ptr [esp + 8], 0fffffffeh
        push   esi
        push   edi
        je     SHORT loc_5_9CC6
        mov    eax, 4
        jmp    SHORT loc_5_9D16
loc_5_9CC6:
        mov    edx, 1
        mov    esi, dword ptr [esp + 0ch]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     SHORT loc_5_9D11
        db  039h, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_9D11
        mov    edi, dword ptr [esp + 10h]
        and    edi, 1
        je     SHORT loc_5_9CF1
        db  0FFh, 005h, 000h, 000h, 000h, 000h
loc_5_9CF1:
        push   0
        db  0B9h, 000h, 000h, 000h, 000h
        push   0
        mov    edx, esi
        push   0
        call   sub_5_02D3
        test   edi, edi
        je     SHORT loc_5_9D0D
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
loc_5_9D0D:
        xor    eax, eax
        jmp    SHORT loc_5_9D16
loc_5_9D11:
        mov    eax, 5
loc_5_9D16:
        pop    edi
        pop    esi
        ret
_CONFIGMG_Register_Device_Driver:
        push   ebp
        mov    ebp, esp
        test   dword ptr [esp + 14h], 0fffffff8h
        je     SHORT loc_5_9D2D
        mov    eax, 4
        jmp    SHORT loc_5_9D59
loc_5_9D2D:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_1AC6
        test   eax, eax
        mov    eax, 5
        je     SHORT loc_5_9D59
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_5_1F16
        add    esp, 10h
        xor    eax, eax
loc_5_9D59:
        pop    ebp
        ret
_CONFIGMG_Register_Enumerator:
        push   ebp
        mov    ebp, esp
        push   esi
        test   dword ptr [ebp + 10h], 0fffffffeh
        je     SHORT loc_5_9D6F
        mov    eax, 4
        jmp    SHORT loc_5_9DBF
loc_5_9D6F:
        mov    edx, 1
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        jne    SHORT loc_5_9D89
        mov    eax, 5
        jmp    SHORT loc_5_9DBF
loc_5_9D89:
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, esi
        and    eax, 1
        shl    eax, 7
        cdq
        push   edx
        push   eax
        call   sub_5_1906
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        mov    dword ptr [esi + 1ch], eax
        jne    SHORT loc_5_9DAD
        db  0C7h, 046h, 01Ch, 000h, 000h, 000h, 000h
loc_5_9DAD:
        push   0
        mov    ecx, esi
        push   24h
        call   sub_5_1906
        call   sub_5_7B5F
        xor    eax, eax
loc_5_9DBF:
        pop    esi
        pop    ebp
        ret
_CONFIGMG_Register_Arbitrator:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 1ch]
        test   ebx, 0ffffffe0h
        je     SHORT loc_5_9DDD
        mov    eax, 4
        jmp    NEAR PTR loc_5_9E7D
loc_5_9DDD:
        mov    edi, dword ptr [ebp + 18h]
        test   edi, edi
        je     SHORT loc_5_9E02
        mov    edx, 1
        mov    ecx, edi
        call   sub_5_1AC6
        test   eax, eax
        je     NEAR PTR loc_5_9E78
        db  03Bh, 03Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_9E0C
        jmp    SHORT loc_5_9E78
loc_5_9E02:
        test   bl, 1
        je     SHORT loc_5_9E78
        test   bl, 8
        jne    SHORT loc_5_9E78
loc_5_9E0C:
        mov    esi, dword ptr [ebp + 0ch]
        test   esi, esi
        jne    SHORT loc_5_9E1A
        mov    eax, 0bh
        jmp    SHORT loc_5_9E7D
loc_5_9E1A:
        test   esi, 8000h
        je     SHORT loc_5_9E29
        mov    eax, 0bh
        jmp    SHORT loc_5_9E7D
loc_5_9E29:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   1
        push   dword ptr [ebp + 10h]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_9E46
        mov    eax, 3
        jmp    SHORT loc_5_9E7D
loc_5_9E46:
        push   1
        push   0
        push   0
        push   4
        push   dword ptr [ebp + 8]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_9E64
        mov    eax, 3
        jmp    SHORT loc_5_9E7D
loc_5_9E64:
        push   ebx
        mov    edx, esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        call   sub_5_0656
        jmp    SHORT loc_5_9E7D
loc_5_9E78:
        mov    eax, 5
loc_5_9E7D:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
_CONFIGMG_Deregister_Arbitrator:
        cmp    dword ptr [esp + 8], 0
        mov    eax, 4
        jne    SHORT loc_5_9E9F
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_083E
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 8
loc_5_9E9F:
        ret
_CONFIGMG_Query_Arbitrator_Free_Size:
        push   ebp
        mov    ebp, esp
        sub    esp, 38h
        cmp    dword ptr [ebp + 14h], 0
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_5_9EB9
        mov    eax, 4
        jmp    NEAR PTR loc_5_9F41
loc_5_9EB9:
        mov    esi, dword ptr [ebp + 0ch]
        test   esi, esi
        jne    SHORT loc_5_9ED4
        db  08Bh, 035h, 000h, 000h, 000h, 000h
loc_5_9EC6:
        mov    ebx, dword ptr [ebp + 10h]
        test   ebx, ebx
        jne    SHORT loc_5_9EEB
        mov    eax, 0bh
        jmp    SHORT loc_5_9F41
loc_5_9ED4:
        mov    edx, 1
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        jne    SHORT loc_5_9EC6
        mov    eax, 5
        jmp    SHORT loc_5_9F41
loc_5_9EEB:
        xor    eax, eax
        mov    edi, dword ptr [ebp + 8]
        push   eax
        push   eax
        push   eax
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_9F09
        mov    eax, 3
        jmp    SHORT loc_5_9F41
loc_5_9F09:
        push   esi
        lea    ecx, [ebp - 38h]
        mov    edx, ebx
        call   sub_5_08C9
        mov    ecx, eax
        call   sub_5_0516
        mov    ecx, eax
        test   ecx, ecx
        jne    SHORT loc_5_9F28
        mov    eax, 1bh
        jmp    SHORT loc_5_9F41
loc_5_9F28:
        lea    eax, [ebp - 8]
        mov    edx, 4
        push   eax
        call   sub_5_0B16
        test   eax, eax
        jne    SHORT loc_5_9F41
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edi], eax
        xor    eax, eax
loc_5_9F41:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
_CONFIGMG_Query_Arbitrator_Free_Data:
        push   ebp
        mov    ebp, esp
        sub    esp, 38h
        cmp    dword ptr [ebp + 18h], 0
        push   esi
        push   edi
        je     SHORT loc_5_9F60
        mov    eax, 4
        jmp    NEAR PTR loc_5_A004
loc_5_9F60:
        mov    esi, dword ptr [ebp + 10h]
        test   esi, esi
        jne    SHORT loc_5_9F7E
        db  08Bh, 035h, 000h, 000h, 000h, 000h
loc_5_9F6D:
        mov    edi, dword ptr [ebp + 14h]
        test   edi, edi
        jne    SHORT loc_5_9F95
        mov    eax, 0bh
        jmp    NEAR PTR loc_5_A004
loc_5_9F7E:
        mov    edx, 1
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        jne    SHORT loc_5_9F6D
        mov    eax, 5
        jmp    SHORT loc_5_A004
loc_5_9F95:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_9FB3
        mov    eax, 3
        jmp    SHORT loc_5_A004
loc_5_9FB3:
        push   esi
        lea    ecx, [ebp - 38h]
        mov    edx, edi
        call   sub_5_08C9
        mov    ecx, eax
        call   sub_5_0516
        test   eax, eax
        jne    SHORT loc_5_9FD0
        mov    eax, 1bh
        jmp    SHORT loc_5_A004
loc_5_9FD0:
        lea    ecx, [ebp - 8]
        mov    edx, 4
        push   ecx
        mov    ecx, eax
        call   sub_5_0B16
        test   eax, eax
        jne    SHORT loc_5_A004
        mov    esi, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        cmp    esi, eax
        jb     SHORT loc_5_9FF0
        mov    esi, eax
loc_5_9FF0:
        push   esi
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_5B5D
        cmp    esi, dword ptr [ebp - 4]
        sbb    eax, eax
        and    eax, 1ah
loc_5_A004:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
_CONFIGMG_Sort_NodeList:
        cmp    dword ptr [esp + 8], 0
        je     SHORT loc_5_A018
        mov    eax, 4
        jmp    SHORT loc_5_A03A
loc_5_A018:
        mov    edx, 1
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_6723
        test   eax, eax
        mov    eax, 9
        je     SHORT loc_5_A03A
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_692A
        xor    eax, eax
loc_5_A03A:
        ret
_CONFIGMG_Add_Empty_Log_Conf:
        push   ebx
        xor    eax, eax
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        push   ebp
        push   eax
        push   eax
        push   eax
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_A05E
        mov    eax, 3
        jmp    SHORT loc_5_A0D8
loc_5_A05E:
        mov    ebp, dword ptr [esp + 20h]
        test   ebp, 0fffffff0h
        jne    SHORT loc_5_A0D3
        mov    esi, ebp
        and    esi, 7
        cmp    esi, 7
        jae    SHORT loc_5_A0D3
        mov    edx, 1
        mov    ebx, dword ptr [esp + 18h]
        mov    ecx, ebx
        call   sub_5_1AC6
        test   eax, eax
        je     SHORT loc_5_A0CC
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_5_A0CC
        cmp    dword ptr [esp + 1ch], 0ffffh
        jbe    SHORT loc_5_A0A1
        mov    eax, 27h
        jmp    SHORT loc_5_A0D8
loc_5_A0A1:
        and    ebp, 8
        mov    edx, dword ptr [esp + 1ch]
        sub    ebp, 8
        mov    ecx, ebx
        cmp    ebp, 1
        sbb    eax, eax
        neg    eax
        push   eax
        push   esi
        call   sub_5_16B4
        test   eax, eax
        je     SHORT loc_5_A0C5
        mov    dword ptr [edi], eax
        xor    eax, eax
        jmp    SHORT loc_5_A0D8
loc_5_A0C5:
        mov    eax, 2
        jmp    SHORT loc_5_A0D8
loc_5_A0CC:
        mov    eax, 5
        jmp    SHORT loc_5_A0D8
loc_5_A0D3:
        mov    eax, 4
loc_5_A0D8:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
_CONFIGMG_Free_Log_Conf:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_5_A0EE
        mov    eax, 4
        jmp    SHORT loc_5_A138
loc_5_A0EE:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_1450
        test   eax, eax
        jne    SHORT loc_5_A106
        mov    eax, 7
        jmp    SHORT loc_5_A138
loc_5_A106:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, 1
        mov    ecx, dword ptr [eax + 4]
        call   sub_5_1AC6
        test   eax, eax
        je     SHORT loc_5_A133
        mov    ecx, dword ptr [ebp + 8]
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ecx + 4], eax
        je     SHORT loc_5_A133
        lea    ecx, [ebp + 8]
        call   sub_5_16EE
        xor    eax, eax
        jmp    SHORT loc_5_A138
loc_5_A133:
        mov    eax, 5
loc_5_A138:
        pop    ebp
        ret
_CONFIGMG_Get_First_Log_Conf:
        push   ebx
        push   esi
        push   edi
        push   1
        mov    edi, dword ptr [esp + 14h]
        push   0
        push   0
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_A15D
        mov    eax, 3
        jmp    SHORT loc_5_A1AA
loc_5_A15D:
        mov    ebx, dword ptr [esp + 18h]
        test   ebx, 0fffffff8h
        jne    SHORT loc_5_A1A5
        mov    eax, ebx
        and    al, 7
        cmp    al, 7
        jae    SHORT loc_5_A1A5
        mov    edx, 1
        mov    esi, dword ptr [esp + 14h]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        jne    SHORT loc_5_A18C
        mov    eax, 5
        jmp    SHORT loc_5_A1AA
loc_5_A18C:
        mov    eax, dword ptr [esi + ebx*4 + 30h]
        test   eax, eax
        je     SHORT loc_5_A19E
        test   edi, edi
        je     SHORT loc_5_A19A
        mov    dword ptr [edi], eax
loc_5_A19A:
        xor    eax, eax
        jmp    SHORT loc_5_A1AA
loc_5_A19E:
        mov    eax, 0eh
        jmp    SHORT loc_5_A1AA
loc_5_A1A5:
        mov    eax, 4
loc_5_A1AA:
        pop    edi
        pop    esi
        pop    ebx
        ret
_CONFIGMG_Get_Next_Log_Conf:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        push   1
        push   0
        push   0
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_A1D0
        mov    eax, 3
        jmp    SHORT loc_5_A20E
loc_5_A1D0:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_5_A1DE
        mov    eax, 4
        jmp    SHORT loc_5_A20E
loc_5_A1DE:
        mov    edx, 1
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, esi
        call   sub_5_1450
        test   eax, eax
        jne    SHORT loc_5_A1F9
        mov    eax, 7
        jmp    SHORT loc_5_A20E
loc_5_A1F9:
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     SHORT loc_5_A209
        test   edi, edi
        je     SHORT loc_5_A205
        mov    dword ptr [edi], eax
loc_5_A205:
        xor    eax, eax
        jmp    SHORT loc_5_A20E
loc_5_A209:
        mov    eax, 0eh
loc_5_A20E:
        pop    edi
        pop    esi
        ret
_CONFIGMG_Add_Res_Des:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   1
        mov    esi, dword ptr [ebp + 8]
        push   0
        push   0
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_A239
        mov    eax, 3
        jmp    NEAR PTR loc_5_A2D1
loc_5_A239:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_A257
        mov    eax, 3
        jmp    SHORT loc_5_A2D1
loc_5_A257:
        cmp    dword ptr [ebp + 1ch], 0
        je     SHORT loc_5_A264
        mov    eax, 4
        jmp    SHORT loc_5_A2D1
loc_5_A264:
        mov    edx, 1
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, edi
        call   sub_5_1450
        test   eax, eax
        jne    SHORT loc_5_A27E
        mov    eax, 7
        jmp    SHORT loc_5_A2D1
loc_5_A27E:
        mov    ebx, dword ptr [edi + 4]
        mov    edx, 1
        mov    ecx, ebx
        call   sub_5_1AC6
        test   eax, eax
        je     SHORT loc_5_A2CC
        db  039h, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_5_A2CC
        mov    edx, dword ptr [ebp + 10h]
        test   edx, edx
        jne    SHORT loc_5_A2A7
        mov    eax, 0bh
        jmp    SHORT loc_5_A2D1
loc_5_A2A7:
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        mov    ecx, edi
        call   sub_5_1761
        test   eax, eax
        je     SHORT loc_5_A2C5
        test   esi, esi
        je     SHORT loc_5_A2C1
        add    eax, 1ch
        mov    dword ptr [esi], eax
loc_5_A2C1:
        xor    eax, eax
        jmp    SHORT loc_5_A2D1
loc_5_A2C5:
        mov    eax, 2
        jmp    SHORT loc_5_A2D1
loc_5_A2CC:
        mov    eax, 5
loc_5_A2D1:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
_CONFIGMG_Modify_Res_Des:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   eax
        push   eax
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_A2FB
        mov    eax, 3
        jmp    NEAR PTR loc_5_A3A5
loc_5_A2FB:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_A31C
        mov    eax, 3
        jmp    NEAR PTR loc_5_A3A5
loc_5_A31C:
        cmp    dword ptr [ebp + 1ch], 0
        je     SHORT loc_5_A329
        mov    eax, 4
        jmp    SHORT loc_5_A3A5
loc_5_A329:
        lea    ecx, [ebp + 0ch]
        mov    edx, 1
        call   sub_5_1489
        test   eax, eax
        jne    SHORT loc_5_A341
        mov    eax, 6
        jmp    SHORT loc_5_A3A5
loc_5_A341:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, 1
        mov    eax, dword ptr [eax + 0ch]
        mov    ecx, dword ptr [eax + 4]
        call   sub_5_1AC6
        test   eax, eax
        je     SHORT loc_5_A3A0
        mov    eax, dword ptr [ebp + 0ch]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [eax + 0ch]
        cmp    dword ptr [ecx + 4], edx
        je     SHORT loc_5_A3A0
        mov    edx, dword ptr [ebp + 10h]
        mov    ecx, eax
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        call   sub_5_178A
        test   eax, eax
        je     SHORT loc_5_A386
        add    eax, 1ch
        mov    dword ptr [esi], eax
        xor    eax, eax
        jmp    SHORT loc_5_A3A5
loc_5_A386:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, 3
        mov    esi, dword ptr [eax + 0ch]
        mov    ecx, dword ptr [esi + 4]
        call   sub_5_731F
        mov    eax, 2
        jmp    SHORT loc_5_A3A5
loc_5_A3A0:
        mov    eax, 5
loc_5_A3A5:
        pop    esi
        pop    ebp
        ret
_CONFIGMG_Free_Res_Des:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   eax
        push   eax
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_A3CA
        mov    eax, 3
        jmp    SHORT loc_5_A434
loc_5_A3CA:
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_5_A3D7
        mov    eax, 4
        jmp    SHORT loc_5_A434
loc_5_A3D7:
        lea    ecx, [ebp + 0ch]
        mov    edx, 1
        call   sub_5_1489
        test   eax, eax
        jne    SHORT loc_5_A3EF
        mov    eax, 6
        jmp    SHORT loc_5_A434
loc_5_A3EF:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, 1
        mov    eax, dword ptr [eax + 0ch]
        mov    ecx, dword ptr [eax + 4]
        call   sub_5_1AC6
        test   eax, eax
        je     SHORT loc_5_A42F
        mov    eax, dword ptr [ebp + 0ch]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [eax + 0ch]
        cmp    dword ptr [ecx + 4], edx
        je     SHORT loc_5_A42F
        mov    ecx, eax
        call   sub_5_17FB
        test   eax, eax
        je     SHORT loc_5_A428
        mov    dword ptr [esi], eax
        xor    eax, eax
        jmp    SHORT loc_5_A434
loc_5_A428:
        mov    eax, 0fh
        jmp    SHORT loc_5_A434
loc_5_A42F:
        mov    eax, 5
loc_5_A434:
        pop    esi
        pop    ebp
        ret
_CONFIGMG_Get_Next_Res_Des:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   1
        push   0
        mov    esi, dword ptr [ebp + 8]
        push   0
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_A45E
        mov    eax, 3
        jmp    NEAR PTR loc_5_A542
loc_5_A45E:
        push   1
        mov    edi, dword ptr [ebp + 14h]
        push   0
        push   0
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_A480
        mov    eax, 3
        jmp    NEAR PTR loc_5_A542
loc_5_A480:
        cmp    dword ptr [ebp + 18h], 0
        je     SHORT loc_5_A490
        mov    eax, 4
        jmp    NEAR PTR loc_5_A542
loc_5_A490:
        mov    eax, dword ptr [ebp + 0ch]
        xor    ecx, ecx
        sub    eax, 4
        push   ecx
        push   ecx
        push   ecx
        push   4
        push   eax
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_A4B4
        mov    eax, 3
        jmp    NEAR PTR loc_5_A542
loc_5_A4B4:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax - 4]
        cmp    eax, 4c52574dh
        je     SHORT loc_5_A4CF
        cmp    eax, 574d5950h
        je     SHORT loc_5_A4F4
        mov    eax, 6
        jmp    SHORT loc_5_A542
loc_5_A4CF:
        lea    ecx, [ebp + 0ch]
        mov    edx, 1
        call   sub_5_1489
        test   eax, eax
        jne    SHORT loc_5_A4E7
        mov    eax, 6
        jmp    SHORT loc_5_A542
loc_5_A4E7:
        mov    edx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_5_1877
        jmp    SHORT loc_5_A517
loc_5_A4F4:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_5_1450
        test   eax, eax
        jne    SHORT loc_5_A50C
        mov    eax, 7
        jmp    SHORT loc_5_A542
loc_5_A50C:
        mov    edx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_5_184D
loc_5_A517:
        test   eax, eax
        je     SHORT loc_5_A53D
        test   edi, edi
        je     SHORT loc_5_A530
        mov    ecx, dword ptr [eax + 8]
        test   ecx, ecx
        je     SHORT loc_5_A52B
        mov    ecx, dword ptr [ecx + 4]
        jmp    SHORT loc_5_A52E
loc_5_A52B:
        mov    ecx, dword ptr [eax + 4]
loc_5_A52E:
        mov    dword ptr [edi], ecx
loc_5_A530:
        test   esi, esi
        je     SHORT loc_5_A539
        add    eax, 1ch
        mov    dword ptr [esi], eax
loc_5_A539:
        xor    eax, eax
        jmp    SHORT loc_5_A542
loc_5_A53D:
        mov    eax, 0fh
loc_5_A542:
        pop    edi
        pop    esi
        pop    ebp
        ret
_CONFIGMG_Get_Performance_Info:
        mov    eax, 13h
        ret
_CONFIGMG_Get_Res_Des_Data_Size:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   eax
        push   eax
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_A56E
        mov    eax, 3
        jmp    SHORT loc_5_A59B
loc_5_A56E:
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_5_A57B
        mov    eax, 4
        jmp    SHORT loc_5_A59B
loc_5_A57B:
        lea    ecx, [ebp + 0ch]
        mov    edx, 1
        call   sub_5_1489
        test   eax, eax
        mov    eax, 6
        je     SHORT loc_5_A59B
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 14h]
        xor    eax, eax
        mov    dword ptr [esi], ecx
loc_5_A59B:
        pop    esi
        pop    ebp
        ret
_CONFIGMG_Get_Res_Des_Data:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   edi
        push   eax
        push   eax
        mov    esi, dword ptr [ebp + 10h]
        push   eax
        push   esi
        push   dword ptr [ebp + 0ch]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_A5C2
        mov    eax, 3
        jmp    SHORT loc_5_A60B
loc_5_A5C2:
        cmp    dword ptr [ebp + 14h], 0
        je     SHORT loc_5_A5CF
        mov    eax, 4
        jmp    SHORT loc_5_A60B
loc_5_A5CF:
        lea    ecx, [ebp + 8]
        mov    edx, 1
        call   sub_5_1489
        test   eax, eax
        jne    SHORT loc_5_A5E7
        mov    eax, 6
        jmp    SHORT loc_5_A60B
loc_5_A5E7:
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [eax + 14h]
        cmp    edi, esi
        mov    eax, edi
        jb     SHORT loc_5_A5F5
        mov    eax, esi
loc_5_A5F5:
        push   eax
        mov    edx, dword ptr [ebp + 8]
        add    edx, 1ch
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_5_5B5D
        cmp    esi, edi
        sbb    eax, eax
        and    eax, 1ah
loc_5_A60B:
        pop    edi
        pop    esi
        pop    ebp
        ret
_CONFIGMG_Process_Events_Now:
        cmp    dword ptr [esp + 4], 0
        mov    eax, 4
        jne    SHORT loc_5_A632
        xor    eax, eax
        db  0B9h, 000h, 000h, 000h, 000h
        push   eax
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        push   eax
        push   eax
        call   sub_5_02D3
        xor    eax, eax
loc_5_A632:
        ret
_CONFIGMG_Create_Range_List:
        cmp    dword ptr [esp + 8], 0
        push   esi
        je     SHORT loc_5_A642
        mov    eax, 4
        jmp    SHORT loc_5_A675
loc_5_A642:
        xor    eax, eax
        mov    esi, dword ptr [esp + 8]
        push   eax
        push   eax
        push   eax
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_A661
        mov    eax, 3
        jmp    SHORT loc_5_A675
loc_5_A661:
        call   sub_5_838F
        test   eax, eax
        jne    SHORT loc_5_A671
        mov    eax, 2
        jmp    SHORT loc_5_A675
loc_5_A671:
        mov    dword ptr [esi], eax
        xor    eax, eax
loc_5_A675:
        pop    esi
        ret
_CONFIGMG_Add_Range:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 10h]
        call   sub_5_8317
        test   eax, eax
        jne    SHORT loc_5_A693
        mov    eax, 11h
        jmp    SHORT loc_5_A6C7
loc_5_A693:
        test   dword ptr [ebp + 14h], 0fffffffeh
        je     SHORT loc_5_A6A3
        mov    eax, 4
        jmp    SHORT loc_5_A6C7
loc_5_A6A3:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0ch], eax
        mov    eax, 12h
        jb     SHORT loc_5_A6C7
        mov    eax, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebp + 0ch]
        not    eax
        and    eax, 1
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   dword ptr [ebp + 10h]
        call   sub_5_7BC2
loc_5_A6C7:
        pop    ebp
        ret
_CONFIGMG_Delete_Range:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 10h]
        call   sub_5_8317
        test   eax, eax
        jne    SHORT loc_5_A6E5
        mov    eax, 11h
        jmp    SHORT loc_5_A715
loc_5_A6E5:
        cmp    dword ptr [ebp + 14h], 0
        je     SHORT loc_5_A6F2
        mov    eax, 4
        jmp    SHORT loc_5_A715
loc_5_A6F2:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, dword ptr [ebp + 8]
        mov    eax, 12h
        jb     SHORT loc_5_A715
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 10h]
        call   sub_5_7C9B
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 2
loc_5_A715:
        pop    ebp
        ret
_CONFIGMG_Test_Range_Available:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        sub    esp, 0ch
        mov    ecx, dword ptr [ebp + 10h]
        call   sub_5_8317
        test   eax, eax
        jne    SHORT loc_5_A735
        mov    eax, 11h
        jmp    SHORT loc_5_A771
loc_5_A735:
        cmp    dword ptr [ebp + 14h], 0
        je     SHORT loc_5_A742
        mov    eax, 4
        jmp    SHORT loc_5_A771
loc_5_A742:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0ch], eax
        mov    eax, 12h
        jb     SHORT loc_5_A771
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 8]
        push   eax
        lea    edx, [ebp - 0ch]
        push   ecx
        push   edx
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 10h]
        call   sub_5_8473
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 13h
loc_5_A771:
        mov    esp, ebp
        pop    ebp
        ret
_CONFIGMG_Dup_Range_List:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        call   sub_5_8317
        test   eax, eax
        jne    SHORT loc_5_A791
        mov    eax, 11h
        jmp    SHORT loc_5_A7C7
loc_5_A791:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_5_8317
        test   eax, eax
        jne    SHORT loc_5_A7A9
        mov    eax, 11h
        jmp    SHORT loc_5_A7C7
loc_5_A7A9:
        cmp    dword ptr [ebp + 10h], 0
        mov    eax, 4
        jne    SHORT loc_5_A7C7
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_7D9A
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 2
loc_5_A7C7:
        pop    ebp
        ret
_CONFIGMG_Free_Range_List:
        push   esi
        mov    edx, 1
        mov    esi, dword ptr [esp + 8]
        mov    ecx, esi
        call   sub_5_8317
        test   eax, eax
        jne    SHORT loc_5_A7E5
        mov    eax, 11h
        jmp    SHORT loc_5_A801
loc_5_A7E5:
        cmp    dword ptr [esp + 0ch], 0
        mov    eax, 4
        jne    SHORT loc_5_A801
        mov    ecx, esi
        call   sub_5_85F7
        mov    ecx, esi
        call   sub_5_83A9
        xor    eax, eax
loc_5_A801:
        pop    esi
        ret
_CONFIGMG_Invert_Range_List:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        call   sub_5_8317
        test   eax, eax
        jne    SHORT loc_5_A81F
        mov    eax, 11h
        jmp    SHORT loc_5_A858
loc_5_A81F:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_5_8317
        test   eax, eax
        jne    SHORT loc_5_A837
        mov    eax, 11h
        jmp    SHORT loc_5_A858
loc_5_A837:
        cmp    dword ptr [ebp + 14h], 0
        mov    eax, 4
        jne    SHORT loc_5_A858
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 10h]
        call   sub_5_7DBA
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 2
loc_5_A858:
        pop    ebp
        ret
_CONFIGMG_Intersect_Range_List:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        call   sub_5_8317
        test   eax, eax
        jne    SHORT loc_5_A876
        mov    eax, 11h
        jmp    SHORT loc_5_A8C7
loc_5_A876:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_5_8317
        test   eax, eax
        jne    SHORT loc_5_A88E
        mov    eax, 11h
        jmp    SHORT loc_5_A8C7
loc_5_A88E:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 10h]
        call   sub_5_8317
        test   eax, eax
        jne    SHORT loc_5_A8A6
        mov    eax, 11h
        jmp    SHORT loc_5_A8C7
loc_5_A8A6:
        cmp    dword ptr [ebp + 14h], 0
        mov    eax, 4
        jne    SHORT loc_5_A8C7
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 10h]
        call   sub_5_7E40
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 2
loc_5_A8C7:
        pop    ebp
        ret
_CONFIGMG_First_Range:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_8317
        test   eax, eax
        jne    SHORT loc_5_A8EA
        mov    eax, 11h
        jmp    NEAR PTR loc_5_A97F
loc_5_A8EA:
        push   1
        mov    esi, dword ptr [ebp + 0ch]
        push   0
        push   0
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_A909
        mov    eax, 3
        jmp    SHORT loc_5_A97F
loc_5_A909:
        push   1
        mov    edi, dword ptr [ebp + 10h]
        push   0
        push   0
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_A928
        mov    eax, 3
        jmp    SHORT loc_5_A97F
loc_5_A928:
        push   1
        mov    ebx, dword ptr [ebp + 14h]
        push   0
        push   0
        push   4
        push   ebx
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_A947
        mov    eax, 3
        jmp    SHORT loc_5_A97F
loc_5_A947:
        cmp    dword ptr [ebp + 18h], 0
        je     SHORT loc_5_A954
        mov    eax, 4
        jmp    SHORT loc_5_A97F
loc_5_A954:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 4]
        test   ecx, ecx
        jne    SHORT loc_5_A965
        mov    eax, 13h
        jmp    SHORT loc_5_A97F
loc_5_A965:
        test   ebx, ebx
        je     SHORT loc_5_A96B
        mov    dword ptr [ebx], ecx
loc_5_A96B:
        test   esi, esi
        je     SHORT loc_5_A974
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [esi], eax
loc_5_A974:
        test   edi, edi
        je     SHORT loc_5_A97D
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [edi], eax
loc_5_A97D:
        xor    eax, eax
loc_5_A97F:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
_CONFIGMG_Next_Range:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        push   ebp
        push   1
        push   0
        push   0
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_A9AB
        mov    eax, 3
        jmp    NEAR PTR loc_5_AA35
loc_5_A9AB:
        mov    edi, dword ptr [esp + 1ch]
        push   1
        push   0
        push   0
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_A9CB
        mov    eax, 3
        jmp    SHORT loc_5_AA35
loc_5_A9CB:
        xor    eax, eax
        mov    ebx, dword ptr [esp + 14h]
        push   eax
        push   eax
        push   eax
        push   4
        push   ebx
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_A9EA
        mov    eax, 3
        jmp    SHORT loc_5_AA35
loc_5_A9EA:
        mov    ebp, dword ptr [ebx]
        mov    edx, 1
        mov    ecx, ebp
        call   sub_5_8335
        test   eax, eax
        jne    SHORT loc_5_AA03
        mov    eax, 12h
        jmp    SHORT loc_5_AA35
loc_5_AA03:
        cmp    dword ptr [esp + 20h], 0
        je     SHORT loc_5_AA11
        mov    eax, 4
        jmp    SHORT loc_5_AA35
loc_5_AA11:
        mov    eax, dword ptr [ebp + 4]
        test   eax, eax
        jne    SHORT loc_5_AA1F
        mov    eax, 13h
        jmp    SHORT loc_5_AA35
loc_5_AA1F:
        mov    dword ptr [ebx], eax
        test   esi, esi
        je     SHORT loc_5_AA2A
        mov    ecx, dword ptr [eax + 8]
        mov    dword ptr [esi], ecx
loc_5_AA2A:
        test   edi, edi
        je     SHORT loc_5_AA33
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [edi], eax
loc_5_AA33:
        xor    eax, eax
loc_5_AA35:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
_CONFIGMG_Dump_Range_List:
        mov    edx, 1
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_8317
        test   eax, eax
        mov    eax, 11h
        je     SHORT loc_5_AA62
        mov    eax, dword ptr [esp + 8]
        and    al, 0feh
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0fh
        add    eax, 4
loc_5_AA62:
        ret
_CONFIGMG_Load_DLVxDs:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_5_AA74
        mov    eax, 4
        jmp    SHORT loc_5_AAC9
loc_5_AA74:
        cmp    dword ptr [ebp + 10h], 3
        jb     SHORT loc_5_AA81
        mov    eax, 19h
        jmp    SHORT loc_5_AAC9
loc_5_AA81:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_1AC6
        test   eax, eax
        jne    SHORT loc_5_AA99
        mov    eax, 5
        jmp    SHORT loc_5_AAC9
loc_5_AA99:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   1
        push   dword ptr [ebp + 0ch]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        mov    eax, 3
        je     SHORT loc_5_AAC9
        push   13h
        db  0B9h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        call   sub_5_02D3
loc_5_AAC9:
        pop    ebp
        ret
_CONFIGMG_Get_DDBs:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 18h], 0
        je     SHORT loc_5_AADE
        mov    eax, 4
        jmp    SHORT loc_5_AB51
loc_5_AADE:
        mov    esi, dword ptr [ebp + 10h]
        cmp    esi, 3
        jb     SHORT loc_5_AAED
        mov    eax, 19h
        jmp    SHORT loc_5_AB51
loc_5_AAED:
        mov    edx, 1
        mov    edi, dword ptr [ebp + 14h]
        mov    ecx, edi
        call   sub_5_1AC6
        test   eax, eax
        jne    SHORT loc_5_AB07
        mov    eax, 5
        jmp    SHORT loc_5_AB51
loc_5_AB07:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   4
        push   dword ptr [ebp + 8]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_AB24
        mov    eax, 3
        jmp    SHORT loc_5_AB51
loc_5_AB24:
        xor    eax, eax
        mov    ebx, dword ptr [ebp + 0ch]
        push   eax
        push   eax
        push   eax
        push   4
        push   ebx
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        mov    eax, 3
        je     SHORT loc_5_AB51
        mov    eax, dword ptr [edi + esi*4 + 58h]
        mov    ecx, dword ptr [edi + esi*4 + 4ch]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ebx], eax
        xor    eax, eax
        mov    dword ptr [edx], ecx
loc_5_AB51:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
_CONFIGMG_Get_CRC_CheckSum:
        cmp    dword ptr [esp + 10h], 0
        push   ebx
        push   esi
        push   edi
        push   ebp
        je     SHORT loc_5_AB6B
        mov    eax, 4
        jmp    NEAR PTR loc_5_ABEC
loc_5_AB6B:
        xor    eax, eax
        mov    edi, dword ptr [esp + 1ch]
        push   eax
        push   eax
        push   eax
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_AB8A
        mov    eax, 3
        jmp    SHORT loc_5_ABEC
loc_5_AB8A:
        xor    eax, eax
        mov    esi, dword ptr [esp + 18h]
        push   eax
        push   eax
        push   eax
        push   esi
        push   dword ptr [esp + 24h]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_ABAB
        mov    eax, 3
        jmp    SHORT loc_5_ABEC
loc_5_ABAB:
        mov    ebx, dword ptr [esp + 14h]
        test   esi, esi
        je     SHORT loc_5_ABEA
loc_5_ABB3:
        mov    edx, dword ptr [edi]
        mov    al, byte ptr [ebx]
        xor    al, dl
        dec    esi
        shr    edx, 8
        mov    cl, al
        shr    cl, 3
        inc    ebx
        movzx  ecx, cl
        and    cl, 0feh
        db  066h, 08Bh, 0A9h, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        mov    cl, al
        and    ecx, 0fh
        db  066h, 033h, 02Ch, 04Dh, 000h, 000h, 000h, 000h
        movzx  eax, bp
        xor    eax, edx
        test   esi, esi
        mov    dword ptr [edi], eax
        jne    SHORT loc_5_ABB3
loc_5_ABEA:
        xor    eax, eax
loc_5_ABEC:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
_CONFIGMG_Register_DevLoader:
        cmp    dword ptr [esp + 8], 0
        push   esi
        je     SHORT loc_5_AC00
        mov    eax, 4
        jmp    SHORT loc_5_AC27
loc_5_AC00:
        mov    eax, dword ptr [esp + 8]
        test   eax, eax
        je     SHORT loc_5_AC25
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        db  0BAh, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        db  0A3h, 000h, 000h, 000h, 000h
        call   sub_5_CC30
        db  089h, 035h, 000h, 000h, 000h, 000h
loc_5_AC25:
        xor    eax, eax
loc_5_AC27:
        pop    esi
        ret
_CONFIGMG_Setup_DevNode:
        test   dword ptr [esp + 8], 0fffffffch
        push   esi
        je     SHORT loc_5_AC3E
        mov    eax, 4
        jmp    NEAR PTR loc_5_ACBF
loc_5_AC3E:
        mov    edx, 1
        mov    esi, dword ptr [esp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        jne    SHORT loc_5_AC59
        mov    eax, 5
        jmp    SHORT loc_5_ACBF
loc_5_AC59:
        mov    eax, dword ptr [esp + 0ch]
        test   eax, eax
        je     SHORT loc_5_AC72
        cmp    eax, 1
        je     SHORT loc_5_AC87
        cmp    eax, 2
        je     SHORT loc_5_ACA3
        cmp    eax, 3
        je     SHORT loc_5_ACAC
        jmp    SHORT loc_5_ACBD
loc_5_AC72:
        push   esi
        call   sub_5_8780
        add    esp, 4
        xor    eax, eax
        db  0B9h, 000h, 000h, 000h, 000h
        push   eax
        push   eax
        push   eax
        jmp    SHORT loc_5_ACB6
loc_5_AC87:
        db  039h, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_ACBD
        mov    eax, dword ptr [esi + 4]
        cmp    byte ptr [eax], 0
        je     SHORT loc_5_ACBD
        xor    eax, eax
        db  0B9h, 000h, 000h, 000h, 000h
        push   eax
        push   eax
        push   eax
        jmp    SHORT loc_5_ACB6
loc_5_ACA3:
        mov    ecx, esi
        call   sub_5_8CCD
        jmp    SHORT loc_5_ACBD
loc_5_ACAC:
        xor    eax, eax
        db  0B9h, 000h, 000h, 000h, 000h
        push   eax
        push   eax
        push   eax
loc_5_ACB6:
        mov    edx, esi
        call   sub_5_02D3
loc_5_ACBD:
        xor    eax, eax
loc_5_ACBF:
        pop    esi
        ret
_CONFIGMG_Reset_Children_Marks:
        cmp    dword ptr [esp + 8], 0
        push   esi
        je     SHORT loc_5_ACD0
        mov    eax, 4
        jmp    SHORT loc_5_AD09
loc_5_ACD0:
        mov    edx, 1
        mov    esi, dword ptr [esp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        jne    SHORT loc_5_ACEB
        mov    eax, 5
        jmp    SHORT loc_5_AD09
loc_5_ACEB:
        mov    esi, dword ptr [esi + 24h]
        test   esi, esi
        je     SHORT loc_5_AD07
loc_5_ACF2:
        push   0
        mov    ecx, esi
        push   200h
        call   sub_5_1923
        mov    esi, dword ptr [esi + 28h]
        test   esi, esi
        jne    SHORT loc_5_ACF2
loc_5_AD07:
        xor    eax, eax
loc_5_AD09:
        pop    esi
        ret
_CONFIGMG_Get_DevNode_Status:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 14h], 0
        je     SHORT loc_5_AD1E
        mov    eax, 4
        jmp    SHORT loc_5_AD8A
loc_5_AD1E:
        mov    edx, 1
        mov    edi, dword ptr [ebp + 10h]
        mov    ecx, edi
        call   sub_5_1AC6
        test   eax, eax
        jne    SHORT loc_5_AD38
        mov    eax, 5
        jmp    SHORT loc_5_AD8A
loc_5_AD38:
        push   1
        mov    ebx, dword ptr [ebp + 8]
        push   0
        push   0
        push   4
        push   ebx
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_AD57
        mov    eax, 3
        jmp    SHORT loc_5_AD8A
loc_5_AD57:
        push   1
        mov    esi, dword ptr [ebp + 0ch]
        push   0
        push   0
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_AD76
        mov    eax, 3
        jmp    SHORT loc_5_AD8A
loc_5_AD76:
        test   ebx, ebx
        je     SHORT loc_5_AD7F
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ebx], eax
loc_5_AD7F:
        test   esi, esi
        je     SHORT loc_5_AD88
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [esi], eax
loc_5_AD88:
        xor    eax, eax
loc_5_AD8A:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
_CONFIGMG_Remove_Unmarked_Children:
        cmp    dword ptr [esp + 8], 0
        je     SHORT loc_5_AD9D
        mov    eax, 4
        jmp    SHORT loc_5_ADC9
loc_5_AD9D:
        mov    edx, 1
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_1AC6
        test   eax, eax
        mov    eax, 5
        je     SHORT loc_5_ADC9
        xor    eax, eax
        db  0B9h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [esp + 4]
        push   eax
        push   eax
        push   eax
        call   sub_5_02D3
        xor    eax, eax
loc_5_ADC9:
        ret
_CONFIGMG_ISAPNP_To_CM:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 14h]
        test   edi, 0fffffffch
        je     SHORT loc_5_ADE1
        mov    eax, 4
        jmp    SHORT loc_5_AE42
loc_5_ADE1:
        mov    edx, 1
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        jne    SHORT loc_5_ADFB
        mov    eax, 5
        jmp    SHORT loc_5_AE42
loc_5_ADFB:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   1
        push   dword ptr [ebp + 8]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_AE18
        mov    eax, 3
        jmp    SHORT loc_5_AE42
loc_5_AE18:
        cmp    dword ptr [esi + 30h], 0
        je     SHORT loc_5_AE22
        test   edi, edi
        je     SHORT loc_5_AE2D
loc_5_AE22:
        cmp    dword ptr [esi + 3ch], 0
        je     SHORT loc_5_AE34
        cmp    edi, 2
        jne    SHORT loc_5_AE34
loc_5_AE2D:
        mov    eax, 0ah
        jmp    SHORT loc_5_AE42
loc_5_AE34:
        push   edi
        mov    edx, esi
        push   dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_5391
loc_5_AE42:
        pop    edi
        pop    esi
        pop    ebp
        ret
_CONFIGMG_CallBack_Device_Driver:
        push   ebp
        mov    eax, 4
        mov    ebp, esp
        push   esi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_5_AE77
        mov    eax, dword ptr [ebp + 8]
        db  0BAh, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        db  0A3h, 000h, 000h, 000h, 000h
        call   sub_5_CC30
        xor    eax, eax
        db  089h, 035h, 000h, 000h, 000h, 000h
loc_5_AE77:
        pop    esi
        pop    ebp
        ret
_CONFIGMG_CallBack_Enumerator:
        push   ebp
        mov    eax, 4
        mov    ebp, esp
        push   esi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_5_AEAB
        mov    eax, dword ptr [ebp + 8]
        db  0BAh, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        db  0A3h, 000h, 000h, 000h, 000h
        call   sub_5_CC30
        xor    eax, eax
        db  089h, 035h, 000h, 000h, 000h, 000h
loc_5_AEAB:
        pop    esi
        pop    ebp
        ret
_CONFIGMG_Get_Alloc_Log_Conf:
        test   dword ptr [esp + 0ch], 0fffffffeh
        push   ebx
        push   esi
        push   edi
        push   ebp
        je     SHORT loc_5_AEC6
        mov    eax, 4
        jmp    NEAR PTR loc_5_B075
loc_5_AEC6:
        mov    edx, 1
        mov    esi, dword ptr [esp + 18h]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        jne    SHORT loc_5_AEE4
        mov    eax, 5
        jmp    NEAR PTR loc_5_B075
loc_5_AEE4:
        xor    eax, eax
        mov    ebx, dword ptr [esp + 14h]
        push   eax
        push   eax
        push   eax
        push   0d8h
        push   ebx
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_AF09
        mov    eax, 3
        jmp    NEAR PTR loc_5_B075
loc_5_AF09:
        mov    edi, ebx
        xor    eax, eax
        mov    ecx, 36h
        rep stosd
        mov    eax, dword ptr [esp + 1ch]
        and    eax, 1
        dec    eax
        cmp    eax, 1
        mov    eax, 3
        adc    eax, -1
        mov    esi, dword ptr [esi + eax*4 + 30h]
        test   esi, esi
        jne    SHORT loc_5_AF39
        mov    eax, 0eh
        jmp    NEAR PTR loc_5_B075
loc_5_AF39:
        xor    di, di
        mov    edx, 1
        mov    ecx, esi
        call   sub_5_184D
        test   eax, eax
        je     SHORT loc_5_AF85
loc_5_AF4C:
        cmp    di, 9
        jae    SHORT loc_5_AF85
        mov    ecx, dword ptr [eax + 20h]
        lea    ebp, [eax + 1ch]
        movzx  edx, di
        inc    di
        mov    dword ptr [ebx + edx*4 + 2], ecx
        mov    ecx, dword ptr [ebp + 8]
        sub    ecx, dword ptr [ebp + 4]
        mov    bp, word ptr [ebp + 0ch]
        inc    ecx
        mov    dword ptr [ebx + edx*4 + 26h], ecx
        mov    ecx, eax
        mov    word ptr [ebx + edx*2 + 4ah], bp
        mov    edx, 1
        call   sub_5_1877
        test   eax, eax
        jne    SHORT loc_5_AF4C
loc_5_AF85:
        mov    word ptr [ebx], di
        mov    edx, 2
        xor    di, di
        mov    ecx, esi
        call   sub_5_184D
        test   eax, eax
        je     SHORT loc_5_AFD4
loc_5_AF9B:
        cmp    di, 14h
        jae    SHORT loc_5_AFD4
        movzx  ecx, di
        inc    di
        lea    edx, [eax + 1ch]
        lea    ebp, [ebx + ecx*2]
        mov    cx, word ptr [edx + 4]
        mov    dx, word ptr [edx + 6]
        sub    dx, cx
        inc    dx
        mov    word ptr [ebp + 5eh], cx
        mov    ecx, eax
        mov    word ptr [ebp + 86h], dx
        mov    edx, 2
        call   sub_5_1877
        test   eax, eax
        jne    SHORT loc_5_AF9B
loc_5_AFD4:
        mov    word ptr [ebx + 5ch], di
        mov    edx, 4
        xor    di, di
        mov    ecx, esi
        call   sub_5_184D
        test   eax, eax
        je     SHORT loc_5_B01E
loc_5_AFEB:
        cmp    di, 7
        jae    SHORT loc_5_B01E
        movzx  ecx, di
        lea    edx, [ecx + ebx]
        mov    cl, byte ptr [eax + 1eh]
        inc    di
        lea    ebp, [eax + 1ch]
        mov    byte ptr [edx + 0b0h], cl
        mov    cl, byte ptr [ebp]
        mov    byte ptr [edx + 0b7h], cl
        mov    edx, 4
        mov    ecx, eax
        call   sub_5_1877
        test   eax, eax
        jne    SHORT loc_5_AFEB
loc_5_B01E:
        mov    word ptr [ebx + 0aeh], di
        mov    edx, 3
        mov    ecx, esi
        xor    si, si
        call   sub_5_184D
        test   eax, eax
        je     SHORT loc_5_B06C
loc_5_B038:
        cmp    si, 7
        jae    SHORT loc_5_B06C
        mov    cl, byte ptr [eax + 1dh]
        lea    edx, [eax + 1ch]
        movzx  edi, si
        inc    si
        mov    byte ptr [edi + ebx + 0c0h], cl
        movzx  dx, byte ptr [edx]
        mov    word ptr [ebx + edi*2 + 0c7h], dx
        mov    ecx, eax
        mov    edx, 3
        call   sub_5_1877
        test   eax, eax
        jne    SHORT loc_5_B038
loc_5_B06C:
        mov    word ptr [ebx + 0beh], si
        xor    eax, eax
loc_5_B075:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
_CONFIGMG_Get_DevNode_Key_Size:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   edi
        push   eax
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_B0A0
        mov    eax, 3
        jmp    NEAR PTR loc_5_B121
loc_5_B0A0:
        mov    edi, dword ptr [ebp + 14h]
        test   edi, 0fffffcfeh
        je     SHORT loc_5_B0B2
        mov    eax, 4
        jmp    SHORT loc_5_B121
loc_5_B0B2:
        mov    eax, edi
        and    ah, 3
        cmp    ah, 3
        jne    SHORT loc_5_B0C3
        mov    eax, 4
        jmp    SHORT loc_5_B121
loc_5_B0C3:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_5_1AC6
        test   eax, eax
        jne    SHORT loc_5_B0DB
        mov    eax, 5
        jmp    SHORT loc_5_B121
loc_5_B0DB:
        push   1
        push   0
        push   0
        push   1
        push   dword ptr [ebp + 10h]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_B0F9
        mov    eax, 3
        jmp    SHORT loc_5_B121
loc_5_B0F9:
        push   edi
        db  0BAh, 000h, 000h, 000h, 000h
        push   dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_5_8613
        test   eax, eax
        mov    eax, 1ch
        je     SHORT loc_5_B121
        db  0B9h, 000h, 000h, 000h, 000h
        call   sub_5_C3C4
        mov    dword ptr [esi], eax
        xor    eax, eax
loc_5_B121:
        pop    edi
        pop    esi
        pop    ebp
        ret
_CONFIGMG_Get_DevNode_Key:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   edi
        push   eax
        push   eax
        mov    esi, dword ptr [ebp + 14h]
        push   eax
        push   esi
        push   dword ptr [ebp + 10h]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_B14C
        mov    eax, 3
        jmp    NEAR PTR loc_5_B1EE
loc_5_B14C:
        mov    edi, dword ptr [ebp + 18h]
        test   edi, 0fffffcfeh
        je     SHORT loc_5_B161
        mov    eax, 4
        jmp    NEAR PTR loc_5_B1EE
loc_5_B161:
        mov    eax, edi
        and    ah, 3
        cmp    ah, 3
        jne    SHORT loc_5_B172
        mov    eax, 4
        jmp    SHORT loc_5_B1EE
loc_5_B172:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_1AC6
        test   eax, eax
        jne    SHORT loc_5_B18A
        mov    eax, 5
        jmp    SHORT loc_5_B1EE
loc_5_B18A:
        push   1
        push   0
        push   0
        push   1
        push   dword ptr [ebp + 0ch]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_B1A8
        mov    eax, 3
        jmp    SHORT loc_5_B1EE
loc_5_B1A8:
        push   edi
        db  0BAh, 000h, 000h, 000h, 000h
        push   dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_8613
        test   eax, eax
        jne    SHORT loc_5_B1C4
        mov    eax, 1ch
        jmp    SHORT loc_5_B1EE
loc_5_B1C4:
        db  0B9h, 000h, 000h, 000h, 000h
        call   sub_5_C3C4
        mov    edi, eax
        lea    eax, [eax + 1]
        cmp    eax, esi
        jb     SHORT loc_5_B1D9
        mov    eax, esi
loc_5_B1D9:
        push   eax
        db  0BAh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 10h]
        call   sub_5_5B5D
        cmp    esi, edi
        sbb    eax, eax
        and    eax, 1ah
loc_5_B1EE:
        pop    edi
        pop    esi
        pop    ebp
        ret
_CONFIGMG_Read_Registry_Value:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_1AC6
        test   eax, eax
        jne    SHORT loc_5_B211
        mov    eax, 5
        jmp    NEAR PTR loc_5_B2C4
loc_5_B211:
        push   1
        push   0
        push   0
        push   1
        push   dword ptr [ebp + 0ch]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_B232
        mov    eax, 3
        jmp    NEAR PTR loc_5_B2C4
loc_5_B232:
        push   1
        push   0
        push   0
        push   1
        push   dword ptr [ebp + 10h]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_B250
        mov    eax, 3
        jmp    SHORT loc_5_B2C4
loc_5_B250:
        xor    eax, eax
        mov    esi, dword ptr [ebp + 1ch]
        push   eax
        push   eax
        push   eax
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_B26E
        mov    eax, 3
        jmp    SHORT loc_5_B2C4
loc_5_B26E:
        push   1
        push   0
        push   0
        push   dword ptr [esi]
        push   dword ptr [ebp + 18h]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_B28C
        mov    eax, 3
        jmp    SHORT loc_5_B2C4
loc_5_B28C:
        mov    eax, dword ptr [ebp + 20h]
        test   eax, 0fffffcfeh
        je     SHORT loc_5_B29D
        mov    eax, 4
        jmp    SHORT loc_5_B2C4
loc_5_B29D:
        mov    ecx, eax
        and    ch, 3
        cmp    ch, 3
        jne    SHORT loc_5_B2AE
        mov    eax, 4
        jmp    SHORT loc_5_B2C4
loc_5_B2AE:
        push   eax
        mov    edx, dword ptr [ebp + 0ch]
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        call   sub_5_87D6
loc_5_B2C4:
        pop    esi
        pop    ebp
        ret
_CONFIGMG_Write_Registry_Value:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     NEAR PTR loc_5_B382
        mov    eax, dword ptr [esi + 4]
        xor    ecx, ecx
        cmp    byte ptr [eax], cl
        je     NEAR PTR loc_5_B382
        push   1
        push   ecx
        push   ecx
        push   1
        push   dword ptr [ebp + 0ch]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_B30B
        mov    eax, 3
        jmp    SHORT loc_5_B387
loc_5_B30B:
        push   1
        push   0
        push   0
        push   1
        push   dword ptr [ebp + 10h]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_B329
        mov    eax, 3
        jmp    SHORT loc_5_B387
loc_5_B329:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_B347
        mov    eax, 3
        jmp    SHORT loc_5_B387
loc_5_B347:
        mov    eax, dword ptr [ebp + 20h]
        test   eax, 0fffffcfeh
        je     SHORT loc_5_B358
        mov    eax, 4
        jmp    SHORT loc_5_B387
loc_5_B358:
        mov    ecx, eax
        and    ch, 3
        cmp    ch, 3
        jne    SHORT loc_5_B369
        mov    eax, 4
        jmp    SHORT loc_5_B387
loc_5_B369:
        push   eax
        mov    edx, dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        mov    ecx, esi
        call   sub_5_886D
        jmp    SHORT loc_5_B387
loc_5_B382:
        mov    eax, 5
loc_5_B387:
        pop    esi
        pop    ebp
        ret
_CONFIGMG_Disable_DevNode:
        push   ebx
        mov    edx, 1
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     NEAR PTR loc_5_B44C
        db  039h, 035h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_5_B44C
        mov    edi, dword ptr [esp + 14h]
        test   edi, 0fffffffch
        je     SHORT loc_5_B3C7
        mov    eax, 4
        jmp    NEAR PTR loc_5_B451
loc_5_B3C7:
        test   edi, 2
        je     SHORT loc_5_B3DD
        mov    edx, 1dh
        mov    ecx, esi
        call   sub_5_731F
        jmp    SHORT loc_5_B448
loc_5_B3DD:
        xor    ebx, ebx
        cmp    dword ptr [esi + 10h], ebx
        jne    SHORT loc_5_B445
        push   ebx
        mov    ecx, esi
        push   8
        push   ebx
        push   8
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_B434
        test   edi, 1
        jne    SHORT loc_5_B434
        push   ebx
        mov    eax, 2000h
        push   eax
        mov    ecx, esi
        push   ebx
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_B418
        mov    eax, 28h
        jmp    SHORT loc_5_B451
loc_5_B418:
        xor    eax, eax
        db  0B9h, 000h, 000h, 000h, 000h
        push   eax
        mov    edx, esi
        push   eax
        push   eax
        call   sub_5_02D3
        test   eax, eax
        jne    SHORT loc_5_B434
        mov    eax, 28h
        jmp    SHORT loc_5_B451
loc_5_B434:
        xor    eax, eax
        db  0B9h, 000h, 000h, 000h, 000h
        push   eax
        mov    edx, esi
        push   eax
        push   eax
        call   sub_5_02D3
loc_5_B445:
        inc    dword ptr [esi + 10h]
loc_5_B448:
        xor    eax, eax
        jmp    SHORT loc_5_B451
loc_5_B44C:
        mov    eax, 5
loc_5_B451:
        pop    edi
        pop    esi
        pop    ebx
        ret
_CONFIGMG_Enable_DevNode:
        push   esi
        mov    edx, 1
        mov    esi, dword ptr [esp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     SHORT loc_5_B49E
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_B49E
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_5_B480
        mov    eax, 4
        jmp    SHORT loc_5_B4A3
loc_5_B480:
        mov    eax, dword ptr [esi + 10h]
        test   eax, eax
        je     SHORT loc_5_B49A
        dec    eax
        mov    dword ptr [esi + 10h], eax
        jne    SHORT loc_5_B49A
        cmp    dword ptr [esi + 0ch], 16h
        jne    SHORT loc_5_B49A
        mov    ecx, esi
        call   sub_5_7024
loc_5_B49A:
        xor    eax, eax
        jmp    SHORT loc_5_B4A3
loc_5_B49E:
        mov    eax, 5
loc_5_B4A3:
        pop    esi
        ret
_CONFIGMG_Move_DevNode:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   ebp
        test   esi, esi
        je     SHORT loc_5_B4D1
        mov    edx, 1
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     NEAR PTR loc_5_B5BE
        mov    eax, dword ptr [esi + 4]
        cmp    byte ptr [eax], 0
        je     NEAR PTR loc_5_B5BE
loc_5_B4D1:
        mov    edx, 1
        mov    edi, dword ptr [esp + 18h]
        mov    ecx, edi
        call   sub_5_1AC6
        test   eax, eax
        je     NEAR PTR loc_5_B5BE
        mov    eax, dword ptr [edi + 4]
        cmp    byte ptr [eax], 0
        je     NEAR PTR loc_5_B5BE
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_5_B506
        mov    eax, 4
        jmp    NEAR PTR loc_5_B5C3
loc_5_B506:
        test   esi, esi
        je     SHORT loc_5_B57F
        mov    ecx, dword ptr [edi + 4]
        call   sub_5_C3C4
        mov    ebx, eax
        push   201h
        lea    eax, [ebx + 1]
        push   eax
        call   sub_5_D134
        add    esp, 8
        mov    ebp, eax
        test   eax, eax
        jne    SHORT loc_5_B541
        mov    edx, 3
        mov    ecx, esi
        call   sub_5_731F
        mov    eax, 2
        jmp    NEAR PTR loc_5_B5C3
loc_5_B541:
        push   esi
        call   sub_5_8780
        add    esp, 4
        mov    edx, dword ptr [edi + 4]
        mov    ecx, ebp
        push   ebx
        xor    ebx, ebx
        call   sub_5_5B5D
        push   ebx
        mov    ecx, esi
        push   1000h
        mov    dword ptr [esi + 4], ebp
        call   sub_5_1906
        mov    ecx, edi
        mov    dword ptr [edi + 10h], ebx
        call   sub_5_7024
        mov    edx, 19h
        mov    ecx, edi
        call   sub_5_731F
        jmp    SHORT loc_5_B5BA
loc_5_B57F:
        xor    ebx, ebx
        db  0B9h, 000h, 000h, 000h, 000h
        push   ebx
        mov    edx, edi
        push   ebx
        push   ebx
        call   sub_5_02D3
        mov    esi, dword ptr [edi + 24h]
        cmp    esi, ebx
        je     SHORT loc_5_B5BA
loc_5_B597:
        mov    ecx, esi
        call   sub_5_7024
        mov    edx, esi
        mov    ecx, edi
        call   sub_5_6600
        mov    edx, 1
        mov    ecx, esi
        call   sub_5_731F
        mov    esi, dword ptr [esi + 28h]
        test   esi, esi
        jne    SHORT loc_5_B597
loc_5_B5BA:
        xor    eax, eax
        jmp    SHORT loc_5_B5C3
loc_5_B5BE:
        mov    eax, 5
loc_5_B5C3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
_CONFIGMG_Set_Bus_Info:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 18h], 0
        je     SHORT loc_5_B5DE
        mov    eax, 4
        jmp    NEAR PTR loc_5_B68B
loc_5_B5DE:
        xor    eax, eax
        mov    edi, dword ptr [ebp + 10h]
        push   eax
        push   eax
        push   eax
        push   edi
        push   dword ptr [ebp + 14h]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_B600
        mov    eax, 3
        jmp    NEAR PTR loc_5_B68B
loc_5_B600:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_1AC6
        test   eax, eax
        jne    SHORT loc_5_B618
        mov    eax, 5
        jmp    SHORT loc_5_B68B
loc_5_B618:
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_1AE7
        mov    esi, eax
        mov    eax, dword ptr [eax + 68h]
        test   eax, eax
        je     SHORT loc_5_B644
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        jne    SHORT loc_5_B63D
        push   0
        push   eax
        call   sub_5_D0C8
        add    esp, 8
loc_5_B63D:
        mov    dword ptr [esi + 68h], 0
loc_5_B644:
        test   edi, edi
        je     SHORT loc_5_B683
        push   201h
        lea    eax, [edi + 4]
        push   eax
        call   sub_5_D134
        mov    dword ptr [esi + 68h], eax
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_5_B675
        mov    edx, 3
        mov    ecx, esi
        call   sub_5_731F
        mov    eax, 2
        jmp    SHORT loc_5_B68B
loc_5_B675:
        push   edi
        lea    ecx, [ebx + 4]
        mov    edx, dword ptr [ebp + 14h]
        call   sub_5_5B5D
        mov    dword ptr [ebx], edi
loc_5_B683:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 64h], eax
        xor    eax, eax
loc_5_B68B:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
_CONFIGMG_Get_Bus_Info:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 18h], 0
        je     SHORT loc_5_B6A6
        mov    eax, 4
        jmp    NEAR PTR loc_5_B78C
loc_5_B6A6:
        push   1
        mov    esi, dword ptr [ebp + 0ch]
        push   0
        push   0
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_B6C8
        mov    eax, 3
        jmp    NEAR PTR loc_5_B78C
loc_5_B6C8:
        mov    edi, dword ptr [ebp + 10h]
        xor    eax, eax
        test   edi, edi
        je     SHORT loc_5_B6F1
        push   eax
        push   eax
        push   eax
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_B6ED
        mov    eax, 3
        jmp    NEAR PTR loc_5_B78C
loc_5_B6ED:
        mov    ebx, dword ptr [edi]
        jmp    SHORT loc_5_B6F3
loc_5_B6F1:
        xor    ebx, ebx
loc_5_B6F3:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   ebx
        push   dword ptr [ebp + 14h]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_B70F
        mov    eax, 3
        jmp    SHORT loc_5_B78C
loc_5_B70F:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_1AC6
        test   eax, eax
        jne    SHORT loc_5_B727
        mov    eax, 5
        jmp    SHORT loc_5_B78C
loc_5_B727:
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_1AE7
        test   esi, esi
        je     SHORT loc_5_B738
        mov    ecx, dword ptr [eax + 64h]
        mov    dword ptr [esi], ecx
loc_5_B738:
        mov    edx, dword ptr [eax + 68h]
        mov    esi, 0
        test   edx, edx
        je     SHORT loc_5_B746
        mov    esi, dword ptr [edx]
loc_5_B746:
        test   ebx, ebx
        jne    SHORT loc_5_B754
        test   edi, edi
        je     SHORT loc_5_B750
        mov    dword ptr [edi], esi
loc_5_B750:
        xor    eax, eax
        jmp    SHORT loc_5_B78C
loc_5_B754:
        cmp    ebx, esi
        jb     SHORT loc_5_B75A
        mov    ebx, esi
loc_5_B75A:
        test   edi, edi
        je     SHORT loc_5_B760
        mov    dword ptr [edi], ebx
loc_5_B760:
        push   ebx
        add    edx, 4
        mov    ecx, dword ptr [ebp + 14h]
        call   sub_5_5B5D
        mov    edx, 1
        cmp    ebx, esi
        adc    edx, -1
        cmp    ebx, 1
        sbb    eax, eax
        inc    eax
        and    edx, eax
        mov    eax, 0
        cmp    edx, 1
        adc    eax, -1
        and    eax, 1ah
loc_5_B78C:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
_CONFIGMG_Set_HW_Prof:
        cmp    dword ptr [esp + 8], 0
        mov    eax, 4
        jne    SHORT loc_5_B7B1
        push   13h
        db  0B9h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [esp + 8]
        push   0
        push   0
        call   sub_5_02D3
loc_5_B7B1:
        ret
_CONFIGMG_Recompute_HW_Prof:
        push   ebp
        mov    eax, 4
        mov    ebp, esp
        test   dword ptr [esp + 10h], 0fffffffeh
        jne    SHORT loc_5_B7D9
        push   13h
        db  0B9h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        call   sub_5_02D3
loc_5_B7D9:
        pop    ebp
        ret
_CONFIGMG_Query_Change_HW_Prof:
        push   ebp
        mov    eax, 4
        mov    ebp, esp
        test   dword ptr [esp + 10h], 0fffffffch
        jne    SHORT loc_5_B802
        push   13h
        db  0B9h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        call   sub_5_02D3
loc_5_B802:
        pop    ebp
        ret
_CONFIGMG_Get_Device_Driver_Private_DWord:
        cmp    dword ptr [esp + 0ch], 0
        push   esi
        push   edi
        je     SHORT loc_5_B814
        mov    eax, 4
        jmp    SHORT loc_5_B853
loc_5_B814:
        mov    edx, 1
        mov    esi, dword ptr [esp + 0ch]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        jne    SHORT loc_5_B82F
        mov    eax, 5
        jmp    SHORT loc_5_B853
loc_5_B82F:
        xor    eax, eax
        mov    edi, dword ptr [esp + 10h]
        push   eax
        push   eax
        push   eax
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        mov    eax, 3
        je     SHORT loc_5_B853
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [edi], eax
        xor    eax, eax
loc_5_B853:
        pop    edi
        pop    esi
        ret
_CONFIGMG_Set_Device_Driver_Private_DWord:
        cmp    dword ptr [esp + 0ch], 0
        push   esi
        je     SHORT loc_5_B865
        mov    eax, 4
        jmp    SHORT loc_5_B887
loc_5_B865:
        mov    edx, 1
        mov    esi, dword ptr [esp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        mov    eax, 5
        je     SHORT loc_5_B887
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [esi + 14h], eax
        xor    eax, eax
loc_5_B887:
        pop    esi
        ret
_CONFIGMG_Get_HW_Prof_Flags:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   eax
        push   eax
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   1
        push   esi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_B8AB
        mov    eax, 3
        jmp    SHORT loc_5_B8F2
loc_5_B8AB:
        mov    ecx, esi
        call   sub_5_C3C4
        test   eax, eax
        jne    SHORT loc_5_B8BD
        mov    eax, 3
        jmp    SHORT loc_5_B8F2
loc_5_B8BD:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   4
        push   dword ptr [ebp + 10h]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_B8DA
        mov    eax, 3
        jmp    SHORT loc_5_B8F2
loc_5_B8DA:
        cmp    dword ptr [ebp + 14h], 0
        mov    eax, 4
        jne    SHORT loc_5_B8F2
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, esi
        push   dword ptr [ebp + 10h]
        call   sub_5_410E
loc_5_B8F2:
        pop    esi
        pop    ebp
        ret
_CONFIGMG_Set_HW_Prof_Flags:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   eax
        push   eax
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   1
        push   esi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_B917
        mov    eax, 3
        jmp    SHORT loc_5_B948
loc_5_B917:
        mov    ecx, esi
        call   sub_5_C3C4
        test   eax, eax
        jne    SHORT loc_5_B929
        mov    eax, 3
        jmp    SHORT loc_5_B948
loc_5_B929:
        cmp    dword ptr [ebp + 14h], 0
        mov    eax, 4
        jne    SHORT loc_5_B948
        push   13h
        db  0B9h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        mov    edx, esi
        call   sub_5_02D3
loc_5_B948:
        pop    esi
        pop    ebp
        ret
_CONFIGMG_Read_Registry_Log_Confs:
        cmp    dword ptr [esp + 8], 0
        je     SHORT loc_5_B959
        mov    eax, 4
        jmp    SHORT loc_5_B982
loc_5_B959:
        mov    edx, 1
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_1AC6
        test   eax, eax
        mov    eax, 5
        je     SHORT loc_5_B982
        push   1
        db  0BAh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [esp + 8]
        push   0
        call   sub_5_8B0B
loc_5_B982:
        ret
_CONFIGMG_Run_Detection:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        cmp    dword ptr [ebp + 8], 0
        je     SHORT loc_5_B996
        mov    eax, 4
        jmp    SHORT loc_5_B9BC
loc_5_B996:
        lea    edx, [ebp - 4]
        xor    ecx, ecx
        db  080h, 00Dh, 000h, 000h, 000h, 000h, 020h
        call   sub_5_44B6
        or     byte ptr [ebp - 1], 80h
        mov    edx, dword ptr [ebp - 4]
        xor    ecx, ecx
        call   sub_5_4572
        call   sub_5_7B5F
        xor    eax, eax
loc_5_B9BC:
        mov    esp, ebp
        pop    ebp
        ret
_CONFIGMG_Fail_Change_HW_Prof:
        cmp    dword ptr [esp + 8], 0
        push   esi
        je     SHORT loc_5_B9CF
        mov    eax, 4
        jmp    SHORT loc_5_BA03
loc_5_B9CF:
        mov    esi, dword ptr [esp + 8]
        test   esi, esi
        je     SHORT loc_5_B9F1
        cmp    esi, -1
        je     SHORT loc_5_B9F1
        mov    edx, 1
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        mov    eax, 5
        je     SHORT loc_5_BA03
loc_5_B9F1:
        push   13h
        db  0B9h, 000h, 000h, 000h, 000h
        push   0
        mov    edx, esi
        push   0
        call   sub_5_02D3
loc_5_BA03:
        pop    esi
        ret
_CONFIGMG_Set_Private_Problem:
        cmp    dword ptr [esp + 0ch], 0
        push   esi
        je     SHORT loc_5_BA14
        mov    eax, 4
        jmp    SHORT loc_5_BA44
loc_5_BA14:
        mov    edx, 1
        mov    esi, dword ptr [esp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     SHORT loc_5_BA3F
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_BA3F
        mov    edx, dword ptr [esp + 0ch]
        mov    ecx, esi
        call   sub_5_7393
        xor    eax, eax
        jmp    SHORT loc_5_BA44
loc_5_BA3F:
        mov    eax, 5
loc_5_BA44:
        pop    esi
        ret
_CONFIGMG_Debug_DevNode:
        cmp    dword ptr [esp + 8], 0
        mov    eax, 4
        jne    SHORT loc_5_BA6A
        mov    edx, 1
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_1AC6
        cmp    eax, 1
        sbb    eax, eax
        and    al, 0f2h
        add    eax, 13h
loc_5_BA6A:
        ret
_CONFIGMG_Get_Hardware_Profile_Info:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   eax
        push   eax
        push   eax
        push   58h
        push   dword ptr [ebp + 0ch]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_BA8B
        mov    eax, 3
        jmp    SHORT loc_5_BAA1
loc_5_BA8B:
        cmp    dword ptr [ebp + 10h], 0
        mov    eax, 4
        jne    SHORT loc_5_BAA1
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_4617
loc_5_BAA1:
        pop    ebp
        ret
_CONFIGMG_Register_Enumerator_Function:
        cmp    dword ptr [esp + 0ch], 0
        push   esi
        je     SHORT loc_5_BAB2
        mov    eax, 4
        jmp    SHORT loc_5_BADE
loc_5_BAB2:
        mov    edx, 1
        mov    esi, dword ptr [esp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     SHORT loc_5_BAD9
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_BAD9
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [esi + 2ch], eax
        xor    eax, eax
        jmp    SHORT loc_5_BADE
loc_5_BAD9:
        mov    eax, 5
loc_5_BADE:
        pop    esi
        ret
_CONFIGMG_Call_Enumerator_Function:
        push   ebp
        mov    ebp, esp
        push   esi
        cmp    dword ptr [ebp + 1ch], 0
        je     SHORT loc_5_BAF4
        mov    eax, 4
        jmp    NEAR PTR loc_5_BB78
loc_5_BAF4:
        mov    edx, 1
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     SHORT loc_5_BB73
        db  039h, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_BB73
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_BB2D
        mov    eax, 3
        jmp    SHORT loc_5_BB78
loc_5_BB2D:
        push   0
        mov    eax, 20000h
        push   eax
        mov    ecx, esi
        push   0
        push   eax
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_BB4E
        mov    eax, dword ptr [esi + 20h]
        cmp    dword ptr [eax + 2ch], 0
        jne    SHORT loc_5_BB4E
        mov    esi, eax
loc_5_BB4E:
        mov    eax, dword ptr [esi + 20h]
        mov    ecx, dword ptr [eax + 2ch]
        test   ecx, ecx
        jne    SHORT loc_5_BB5F
        mov    eax, 20h
        jmp    SHORT loc_5_BB78
loc_5_BB5F:
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   esi
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        call   ecx
        add    esp, 14h
        jmp    SHORT loc_5_BB78
loc_5_BB73:
        mov    eax, 5
loc_5_BB78:
        pop    esi
        pop    ebp
        ret
_CONFIGMG_Add_ID:
        push   ebp
        mov    ebp, esp
        push   esi
        test   dword ptr [ebp + 10h], 0fffffffeh
        je     SHORT loc_5_BB8F
        mov    eax, 4
        jmp    SHORT loc_5_BBE3
loc_5_BB8F:
        mov    edx, 1
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     SHORT loc_5_BBDE
        db  039h, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_BBDE
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   1
        push   dword ptr [ebp + 0ch]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_BBC7
        mov    eax, 3
        jmp    SHORT loc_5_BBE3
loc_5_BBC7:
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, esi
        cmp    dword ptr [ebp + 10h], 1
        sbb    eax, eax
        neg    eax
        push   eax
        call   sub_5_21CF
        xor    eax, eax
        jmp    SHORT loc_5_BBE3
loc_5_BBDE:
        mov    eax, 5
loc_5_BBE3:
        pop    esi
        pop    ebp
        ret
_CONFIGMG_Find_Range:
        push   ebp
        mov    ebp, esp
        push   1
        push   0
        push   0
        push   4
        push   dword ptr [ebp + 8]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_BC07
        mov    eax, 3
        jmp    SHORT loc_5_BC49
loc_5_BC07:
        cmp    dword ptr [ebp + 20h], 0
        je     SHORT loc_5_BC14
        mov    eax, 4
        jmp    SHORT loc_5_BC49
loc_5_BC14:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 1ch]
        call   sub_5_8317
        test   eax, eax
        mov    eax, 11h
        je     SHORT loc_5_BC49
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        call   sub_5_7EB3
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 13h
loc_5_BC49:
        pop    ebp
        ret
_CONFIGMG_Get_Global_State:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   eax
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_BC70
        mov    eax, 3
        jmp    SHORT loc_5_BCC4
loc_5_BC70:
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_5_BC7D
        mov    eax, 4
        jmp    SHORT loc_5_BCC4
loc_5_BC7D:
        xor    ecx, ecx
        db  039h, 00Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_BC8C
        mov    ecx, 1
loc_5_BC8C:
        mov    dword ptr [ebp - 4], esp
        mov    eax, dword ptr [ebp - 4]
        db  02Bh, 005h, 000h, 000h, 000h, 000h
        cmp    eax, 4000h
        jae    SHORT loc_5_BCA2
        or     cl, 2
loc_5_BCA2:
        db  0A1h, 000h, 000h, 000h, 000h
        test   al, 40h
        jne    SHORT loc_5_BCB2
        test   al, 1
        je     SHORT loc_5_BCB2
        or     cl, 4
loc_5_BCB2:
        test   al, 8
        je     SHORT loc_5_BCB9
        or     cl, 8
loc_5_BCB9:
        test   al, 20h
        je     SHORT loc_5_BCC0
        or     cl, 10h
loc_5_BCC0:
        xor    eax, eax
        mov    dword ptr [esi], ecx
loc_5_BCC4:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
_CONFIGMG_Broadcast_Device_Change_Message:
        push   ebp
        mov    ebp, esp
        test   dword ptr [esp + 10h], 0fffffffeh
        je     SHORT loc_5_BCDD
        mov    eax, 4
        jmp    SHORT loc_5_BD06
loc_5_BCDD:
        mov    eax, dword ptr [ebp + 8]
        shr    eax, 10h
        test   ax, ax
        mov    eax, 13h
        jne    SHORT loc_5_BD06
        push   13h
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 1
        db  0B9h, 000h, 000h, 000h, 000h
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        call   sub_5_02D3
loc_5_BD06:
        pop    ebp
        ret
_CONFIGMG_Call_DevNode_Handler:
        push   ebp
        mov    ebp, esp
        push   esi
        test   dword ptr [ebp + 10h], 0fffffffeh
        je     SHORT loc_5_BD1C
        mov    eax, 4
        jmp    SHORT loc_5_BD56
loc_5_BD1C:
        mov    edx, 1
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     SHORT loc_5_BD51
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_BD51
        push   13h
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 1
        db  0B9h, 000h, 000h, 000h, 000h
        push   eax
        mov    edx, esi
        push   dword ptr [ebp + 0ch]
        call   sub_5_02D3
        jmp    SHORT loc_5_BD56
loc_5_BD51:
        mov    eax, 5
loc_5_BD56:
        pop    esi
        pop    ebp
        ret
_CONFIGMG_Remove_Reinsert_All:
        test   dword ptr [esp + 4], 0fffffffeh
        mov    eax, 4
        jne    SHORT loc_5_BD80
        xor    eax, eax
        mov    edx, dword ptr [esp + 4]
        push   eax
        and    edx, 1
        push   eax
        db  0B9h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_5_02D3
        xor    eax, eax
loc_5_BD80:
        ret
CONFIGMG_Service_91:
        push   ebp
        mov    ebp, esp
        test   dword ptr [esp + 10h], 0fffffffeh
        je     SHORT loc_5_BD95
        mov    eax, 4
        jmp    SHORT loc_5_BDC7
loc_5_BD95:
        mov    ecx, dword ptr [ebp + 0ch]
        test   ecx, 0fe449d3fh
        je     SHORT loc_5_BDA7
        mov    eax, 1fh
        jmp    SHORT loc_5_BDC7
loc_5_BDA7:
        mov    eax, dword ptr [ebp + 10h]
        test   eax, eax
        je     SHORT loc_5_BDB5
        cmp    eax, 1
        je     SHORT loc_5_BDBF
        jmp    SHORT loc_5_BDC5
loc_5_BDB5:
        mov    eax, dword ptr [ebp + 8]
        not    ecx
        and    dword ptr [eax + 8], ecx
        jmp    SHORT loc_5_BDC5
loc_5_BDBF:
        mov    eax, dword ptr [ebp + 8]
        or     dword ptr [eax + 8], ecx
loc_5_BDC5:
        xor    eax, eax
loc_5_BDC7:
        pop    ebp
        ret
CONFIGMG_Service_92:
        test   dword ptr [esp + 8], 0fffffffeh
        mov    eax, 4
        jne    SHORT loc_5_BDF4
        mov    eax, dword ptr [esp + 8]
        push   0
        push   0
        and    eax, 1
        push   eax
        db  0B9h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [esp + 10h]
        call   sub_5_02D3
        xor    eax, eax
loc_5_BDF4:
        ret
CONFIGMG_Service_93:
        cmp    dword ptr [esp + 0ch], 0
        push   esi
        je     SHORT loc_5_BE04
        mov    eax, 4
        jmp    SHORT loc_5_BE2E
loc_5_BE04:
        xor    eax, eax
        mov    esi, dword ptr [esp + 8]
        push   eax
        push   eax
        push   eax
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        mov    eax, 3
        je     SHORT loc_5_BE2E
        mov    ecx, dword ptr [esp + 0ch]
        call   sub_5_194A
        mov    dword ptr [esi], eax
        xor    eax, eax
loc_5_BE2E:
        pop    esi
        ret
CONFIGMG_Service_94:
        cmp    dword ptr [esp + 8], 0
        push   esi
        je     SHORT loc_5_BE3F
        mov    eax, 4
        jmp    SHORT loc_5_BE88
loc_5_BE3F:
        mov    edx, 1
        mov    esi, dword ptr [esp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     SHORT loc_5_BE83
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_BE83
        xor    eax, eax
        cmp    dword ptr [esi + 44h], eax
        je     SHORT loc_5_BE67
        mov    dword ptr [esi + 3ch], eax
        jmp    SHORT loc_5_BE88
loc_5_BE67:
        mov    eax, dword ptr [esi + 3ch]
        push   800000h
        mov    ecx, esi
        mov    dword ptr [esi + 44h], eax
        xor    eax, eax
        push   eax
        mov    dword ptr [esi + 3ch], eax
        call   sub_5_1923
        xor    eax, eax
        jmp    SHORT loc_5_BE88
loc_5_BE83:
        mov    eax, 5
loc_5_BE88:
        pop    esi
        ret
CONFIGMG_Service_95:
        cmp    dword ptr [esp + 0ch], 0
        push   esi
        push   edi
        je     SHORT loc_5_BE9A
        mov    eax, 4
        jmp    SHORT loc_5_BF02
loc_5_BE9A:
        mov    edx, 1
        mov    edi, dword ptr [esp + 10h]
        mov    ecx, edi
        call   sub_5_1AC6
        test   eax, eax
        je     SHORT loc_5_BEFD
        mov    esi, dword ptr [esp + 0ch]
        test   esi, esi
        je     SHORT loc_5_BEC6
        mov    edx, 1
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     SHORT loc_5_BEFD
loc_5_BEC6:
        mov    edx, edi
        mov    ecx, esi
        call   sub_5_1D59
        test   eax, eax
        jne    SHORT loc_5_BEFD
        mov    edx, esi
        mov    ecx, edi
        call   sub_5_1D59
        test   eax, eax
        jne    SHORT loc_5_BEFD
        mov    dword ptr [edi + 74h], esi
        test   esi, esi
        jne    SHORT loc_5_BEF4
        cmp    dword ptr [edi + 0ch], 1fh
        jne    SHORT loc_5_BEF4
        mov    ecx, edi
        call   sub_5_7024
loc_5_BEF4:
        call   sub_5_7B5F
        xor    eax, eax
        jmp    SHORT loc_5_BF02
loc_5_BEFD:
        mov    eax, 5
loc_5_BF02:
        pop    edi
        pop    esi
        ret
CONFIGMG_Service_96:
        cmp    dword ptr [esp + 0ch], 0
        push   esi
        push   edi
        je     SHORT loc_5_BF15
        mov    eax, 4
        jmp    SHORT loc_5_BF6E
loc_5_BF15:
        mov    edi, dword ptr [esp + 0ch]
        push   1
        push   0
        push   0
        push   4
        push   edi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_BF35
        mov    eax, 3
        jmp    SHORT loc_5_BF6E
loc_5_BF35:
        mov    edx, 1
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     SHORT loc_5_BF69
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_BF69
        mov    eax, dword ptr [esi + 74h]
        test   eax, eax
        je     SHORT loc_5_BF62
        test   edi, edi
        je     SHORT loc_5_BF5E
        mov    dword ptr [edi], eax
loc_5_BF5E:
        xor    eax, eax
        jmp    SHORT loc_5_BF6E
loc_5_BF62:
        mov    eax, 2ch
        jmp    SHORT loc_5_BF6E
loc_5_BF69:
        mov    eax, 5
loc_5_BF6E:
        pop    edi
        pop    esi
        ret
CONFIGMG_Service_97:
        push   esi
        mov    edx, 1
        mov    esi, dword ptr [esp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     SHORT loc_5_BFB2
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_BFB2
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_5_BF9C
        mov    eax, 4
        jmp    SHORT loc_5_BFB7
loc_5_BF9C:
        push   13h
        db  0B9h, 000h, 000h, 000h, 000h
        push   0
        mov    edx, esi
        push   0
        call   sub_5_02D3
        xor    eax, eax
        jmp    SHORT loc_5_BFB7
loc_5_BFB2:
        mov    eax, 5
loc_5_BFB7:
        pop    esi
        ret
CONFIGMG_Service_99:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        call   sub_5_8317
        test   eax, eax
        jne    SHORT loc_5_BFD5
        mov    eax, 11h
        jmp    SHORT loc_5_C026
loc_5_BFD5:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_5_8317
        test   eax, eax
        jne    SHORT loc_5_BFED
        mov    eax, 11h
        jmp    SHORT loc_5_C026
loc_5_BFED:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 10h]
        call   sub_5_8317
        test   eax, eax
        jne    SHORT loc_5_C005
        mov    eax, 11h
        jmp    SHORT loc_5_C026
loc_5_C005:
        cmp    dword ptr [ebp + 14h], 0
        mov    eax, 4
        jne    SHORT loc_5_C026
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 10h]
        call   sub_5_7FAF
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 2
loc_5_C026:
        pop    ebp
        ret
CONFIGMG_Service_98:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        call   sub_5_8317
        test   eax, eax
        jne    SHORT loc_5_C044
        mov    eax, 11h
        jmp    SHORT loc_5_C095
loc_5_C044:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_5_8317
        test   eax, eax
        jne    SHORT loc_5_C05C
        mov    eax, 11h
        jmp    SHORT loc_5_C095
loc_5_C05C:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 10h]
        call   sub_5_8317
        test   eax, eax
        jne    SHORT loc_5_C074
        mov    eax, 11h
        jmp    SHORT loc_5_C095
loc_5_C074:
        cmp    dword ptr [ebp + 14h], 0
        mov    eax, 4
        jne    SHORT loc_5_C095
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 10h]
        call   sub_5_808F
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 2
loc_5_C095:
        pop    ebp
        ret
CONFIGMG_Service_100:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        call   sub_5_1AC6
        test   eax, eax
        jne    SHORT loc_5_C0B3
        mov    eax, 5
        jmp    SHORT loc_5_C0F4
loc_5_C0B3:
        mov    ecx, dword ptr [ebp + 0ch]
        test   ecx, 0fffffff0h
        je     SHORT loc_5_C0C5
        mov    eax, 4
        jmp    SHORT loc_5_C0F4
loc_5_C0C5:
        lea    eax, [ecx - 1]
        test   ecx, eax
        je     SHORT loc_5_C0D3
        mov    eax, 1fh
        jmp    SHORT loc_5_C0F4
loc_5_C0D3:
        test   dword ptr [ebp + 10h], 0fffffffeh
        mov    eax, 4
        jne    SHORT loc_5_C0F4
        push   13h
        mov    edx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 10h]
        push   ecx
        db  0B9h, 000h, 000h, 000h, 000h
        call   sub_5_02D3
loc_5_C0F4:
        pop    ebp
        ret
CONFIGMG_Service_101:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        call   sub_5_1AC6
        test   eax, eax
        jne    SHORT loc_5_C112
        mov    eax, 5
        jmp    SHORT loc_5_C145
loc_5_C112:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   4
        push   dword ptr [ebp + 0ch]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_C12F
        mov    eax, 3
        jmp    SHORT loc_5_C145
loc_5_C12F:
        cmp    dword ptr [ebp + 10h], 0
        mov    eax, 4
        jne    SHORT loc_5_C145
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_5_6FB9
loc_5_C145:
        mov    esp, ebp
        pop    ebp
        ret
CONFIGMG_Service_102:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        call   sub_5_1AC6
        test   eax, eax
        jne    SHORT loc_5_C165
        mov    eax, 5
        jmp    SHORT loc_5_C18B
loc_5_C165:
        test   dword ptr [ebp + 0ch], 0ffffff80h
        je     SHORT loc_5_C175
        mov    eax, 4
        jmp    SHORT loc_5_C18B
loc_5_C175:
        cmp    dword ptr [ebp + 10h], 0
        mov    eax, 4
        jne    SHORT loc_5_C18B
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_5_6FC9
loc_5_C18B:
        mov    esp, ebp
        pop    ebp
        ret
CONFIGMG_Service_103:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        call   sub_5_1AC6
        test   eax, eax
        jne    SHORT loc_5_C1AB
        mov    eax, 5
        jmp    SHORT loc_5_C1DE
loc_5_C1AB:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   4
        push   dword ptr [ebp + 0ch]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_C1C8
        mov    eax, 3
        jmp    SHORT loc_5_C1DE
loc_5_C1C8:
        cmp    dword ptr [ebp + 10h], 0
        mov    eax, 4
        jne    SHORT loc_5_C1DE
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_5_6FD7
loc_5_C1DE:
        mov    esp, ebp
        pop    ebp
        ret
CONFIGMG_Service_104:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   eax
        push   eax
        push   eax
        push   1
        push   dword ptr [ebp + 8]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_C202
        mov    eax, 3
        jmp    SHORT loc_5_C230
loc_5_C202:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_5_8317
        test   eax, eax
        jne    SHORT loc_5_C21A
        mov    eax, 11h
        jmp    SHORT loc_5_C230
loc_5_C21A:
        cmp    dword ptr [ebp + 10h], 0
        mov    eax, 4
        jne    SHORT loc_5_C230
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_812A
loc_5_C230:
        pop    ebp
        ret
CONFIGMG_Service_105:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   eax
        push   eax
        push   eax
        push   1
        push   dword ptr [ebp + 8]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_C252
        mov    eax, 3
        jmp    SHORT loc_5_C280
loc_5_C252:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_5_8317
        test   eax, eax
        jne    SHORT loc_5_C26A
        mov    eax, 11h
        jmp    SHORT loc_5_C280
loc_5_C26A:
        cmp    dword ptr [ebp + 10h], 0
        mov    eax, 4
        jne    SHORT loc_5_C280
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_8255
loc_5_C280:
        pop    ebp
        ret
CONFIGMG_Service_106:
        cmp    dword ptr [esp + 0ch], 0
        push   esi
        push   edi
        je     SHORT loc_5_C292
        mov    eax, 4
        jmp    SHORT loc_5_C2D1
loc_5_C292:
        mov    edx, 1
        mov    edi, dword ptr [esp + 10h]
        mov    ecx, edi
        call   sub_5_1450
        test   eax, eax
        jne    SHORT loc_5_C2AD
        mov    eax, 7
        jmp    SHORT loc_5_C2D1
loc_5_C2AD:
        xor    eax, eax
        mov    esi, dword ptr [esp + 0ch]
        push   eax
        push   eax
        push   eax
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        mov    eax, 3
        je     SHORT loc_5_C2D1
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [esi], eax
        xor    eax, eax
loc_5_C2D1:
        pop    edi
        pop    esi
        ret
CONFIGMG_Service_107:
        cmp    dword ptr [esp + 8], 0
        push   esi
        je     SHORT loc_5_C2E3
        mov    eax, 4
        jmp    SHORT loc_5_C30F
loc_5_C2E3:
        mov    edx, 1
        mov    esi, dword ptr [esp + 8]
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     SHORT loc_5_C30A
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_C30A
        mov    ecx, esi
        call   sub_5_2488
        xor    eax, eax
        jmp    SHORT loc_5_C30F
loc_5_C30A:
        mov    eax, 5
loc_5_C30F:
        pop    esi
        ret
CONFIGMG_Service_108:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   eax
        push   eax
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   4
        push   esi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_C336
        mov    eax, 3
        jmp    NEAR PTR loc_5_C3C1
loc_5_C336:
        mov    eax, dword ptr [ebp + 0ch]
        xor    ecx, ecx
        sub    eax, 4
        push   ecx
        push   ecx
        push   ecx
        push   4
        push   eax
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_5_C357
        mov    eax, 3
        jmp    SHORT loc_5_C3C1
loc_5_C357:
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_5_C364
        mov    eax, 4
        jmp    SHORT loc_5_C3C1
loc_5_C364:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax - 4]
        cmp    eax, 4c52574dh
        je     SHORT loc_5_C37F
        cmp    eax, 574d5950h
        je     SHORT loc_5_C39F
        mov    eax, 6
        jmp    SHORT loc_5_C3C1
loc_5_C37F:
        lea    ecx, [ebp + 0ch]
        mov    edx, 1
        call   sub_5_1489
        test   eax, eax
        jne    SHORT loc_5_C397
        mov    eax, 6
        jmp    SHORT loc_5_C3C1
loc_5_C397:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 0ch]
        jmp    SHORT loc_5_C3BD
loc_5_C39F:
        mov    edx, 1
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_5_1450
        test   eax, eax
        jne    SHORT loc_5_C3B7
        mov    eax, 7
        jmp    SHORT loc_5_C3C1
loc_5_C3B7:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 4]
loc_5_C3BD:
        xor    eax, eax
        mov    dword ptr [esi], ecx
loc_5_C3C1:
        pop    esi
        pop    ebp
        ret
sub_5_C3C4:
        push   edi
        mov    eax, 0
        test   ecx, ecx
        je     SHORT loc_5_C3DE
        mov    edi, ecx
        sub    eax, eax
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        lea    eax, [ecx - 1]
loc_5_C3DE:
        pop    edi
        ret
sub_5_C3E0:
        xor    eax, eax
        test   ecx, ecx
        je     SHORT loc_5_C3F6
        cmp    byte ptr [ecx], 0
        je     SHORT loc_5_C3F6
loc_5_C3EB:
        cmp    byte ptr [ecx], dl
        je     SHORT loc_5_C3FB
        inc    ecx
        inc    eax
        cmp    byte ptr [ecx], 0
        jne    SHORT loc_5_C3EB
loc_5_C3F6:
        mov    eax, 0ffffffffh
loc_5_C3FB:
        ret
sub_5_C3FC:
        push   esi
        push   edi
        mov    esi, ecx
        test   esi, esi
        je     SHORT loc_5_C429
        test   edx, edx
        je     SHORT loc_5_C429
        mov    eax, dword ptr [esp + 0ch]
        mov    edi, edx
        mov    ecx, eax
        shr    ecx, 2
        repe cmpsd
        jne    SHORT loc_5_C41E
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb
loc_5_C41E:
        mov    eax, 1
        je     SHORT loc_5_C42B
        xor    eax, eax
        jmp    SHORT loc_5_C42B
loc_5_C429:
        xor    eax, eax
loc_5_C42B:
        pop    edi
        pop    esi
        ret    4
sub_5_C430:
        test   ecx, ecx
        je     SHORT loc_5_C44D
        cmp    byte ptr [ecx], 0
        je     SHORT loc_5_C44D
loc_5_C439:
        mov    al, byte ptr [ecx]
        cmp    al, 61h
        jl     SHORT loc_5_C447
        cmp    al, 7ah
        jg     SHORT loc_5_C447
        sub    al, 20h
        mov    byte ptr [ecx], al
loc_5_C447:
        inc    ecx
        cmp    byte ptr [ecx], 0
        jne    SHORT loc_5_C439
loc_5_C44D:
        ret
sub_5_C44E:
        push   ebx
        push   esi
        push   edi
        mov    ebx, ecx
        push   ebp
        mov    edi, edx
        mov    edx, 0ffffffffh
        sub    eax, eax
        mov    ecx, edx
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    ebp, ecx
        mov    esi, edi
        mov    ecx, edx
        mov    edi, ebx
        sub    eax, eax
        repne scasb
        add    edi, edx
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        pop    ebp
        and    ecx, 3
        rep movsb
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_C486:
        push   ebx
        sub    eax, eax
        push   esi
        mov    ebx, ecx
        push   edi
        mov    ecx, 0ffffffffh
        mov    edi, edx
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, ebx
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_C4B0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        xor    edi, edi
        cmp    dword ptr [esi + 3ch], edi
        jne    NEAR PTR loc_5_C54C
        cmp    dword ptr [esi + 44h], edi
        jne    NEAR PTR loc_5_C54C
        push   edi
        mov    ecx, esi
        push   10h
        push   edi
        push   10h
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_C54C
        push   edi
        mov    eax, 1000h
        push   eax
        mov    ecx, esi
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_C54C
        push   edi
        mov    eax, 10000h
        push   eax
        mov    ecx, esi
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_C508
        cmp    dword ptr [esi + 24h], edi
        jne    SHORT loc_5_C54C
loc_5_C508:
        xor    edi, edi
        mov    eax, 20000h
        push   edi
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_C54C
        push   edi
        mov    ecx, esi
        push   8
        push   edi
        push   8
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_C54C
        mov    eax, 400000h
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        push   edi
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_C54C
        push   esi
        call   sub_5_9014
        add    esp, 4
loc_5_C54C:
        mov    eax, 1
        pop    edi
        pop    esi
        ret
loc_5_C554:
        mov    eax, dword ptr [esp + 4]
        db  03Bh, 005h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_C56F
        cmp    dword ptr [eax + 44h], 0
        je     SHORT loc_5_C56F
        push   eax
        call   sub_5_122C
        add    esp, 4
loc_5_C56F:
        mov    eax, 1
        ret
loc_5_C575:
        mov    eax, dword ptr [esp + 4]
        db  039h, 005h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_C599
        mov    ecx, dword ptr [eax + 44h]
        xor    edx, edx
        test   ecx, ecx
        je     SHORT loc_5_C599
        push   eax
        mov    dword ptr [eax + 38h], ecx
        mov    dword ptr [eax + 44h], edx
        call   sub_5_1033
        add    esp, 4
loc_5_C599:
        mov    eax, 1
        ret
sub_5_C59F:
        push   edi
        push   0
        mov    edi, ecx
        push   10000h
        call   sub_5_1906
        push   0
        mov    eax, 80h
        push   eax
        mov    ecx, dword ptr [edi + 20h]
        push   0
        push   eax
        call   sub_5_18E3
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        call   _CONFIGMG_Register_Enumerator
        add    esp, 0ch
        pop    edi
        ret
sub_5_C5D8:
        push   ebp
        mov    ebp, esp
        sub    esp, 104h
        mov    dword ptr [ebp - 4], 4
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        xor    ebx, ebx
        xor    edi, edi
        push   1
        lea    eax, [ebp - 4]
        push   eax
        lea    edx, [ebp - 8]
        push   edx
        push   3
        db  068h, 000h, 000h, 000h, 000h
        xor    edx, edx
        call   sub_5_87D6
        test   eax, eax
        jne    SHORT loc_5_C61F
        push   dword ptr [ebp - 8]
        push   ebx
        push   esi
        mov    edi, 1
        call   _CONFIGMG_Register_Enumerator
        add    esp, 0ch
loc_5_C61F:
        push   1
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        xor    edx, edx
        push   3
        mov    ecx, esi
        mov    dword ptr [ebp - 4], 4
        db  068h, 000h, 000h, 000h, 000h
        call   sub_5_87D6
        test   eax, eax
        jne    SHORT loc_5_C65C
        mov    eax, dword ptr [ebp - 8]
        or     al, 4
        mov    ebx, 1
        push   eax
        push   0
        push   0
        push   esi
        call   sub_5_1F16
        add    esp, 10h
loc_5_C65C:
        push   1
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 104h]
        push   ecx
        xor    edx, edx
        push   1
        mov    ecx, esi
        mov    dword ptr [ebp - 4], 100h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_5_87D6
        test   eax, eax
        jne    SHORT loc_5_C69F
        mov    ebx, 1
        push   2
        push   esi
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_5_5854
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_5_C708
loc_5_C69F:
        push   1
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 104h]
        push   ecx
        xor    edx, edx
        push   1
        mov    ecx, esi
        mov    dword ptr [ebp - 4], 100h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_5_87D6
        test   eax, eax
        jne    SHORT loc_5_C6E2
        mov    edi, 1
        push   0
        push   esi
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_5_5854
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_5_C708
loc_5_C6E2:
        test   edi, edi
        jne    SHORT loc_5_C6F3
        push   0
        push   0
        push   esi
        call   _CONFIGMG_Register_Enumerator
        add    esp, 0ch
loc_5_C6F3:
        test   ebx, ebx
        jne    SHORT loc_5_C706
        push   4
        push   0
        push   0
        push   esi
        call   sub_5_1F16
        add    esp, 10h
loc_5_C706:
        xor    eax, eax
loc_5_C708:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_C70F:
        mov    eax, dword ptr [esp + 8]
        test   eax, eax
        je     SHORT loc_5_C723
        cmp    eax, 1
        je     SHORT loc_5_C730
        mov    eax, 1
        jmp    SHORT loc_5_C739
loc_5_C723:
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_C59F
        xor    eax, eax
        jmp    SHORT loc_5_C739
loc_5_C730:
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_C5D8
loc_5_C739:
        ret    8
sub_5_C73C:
        xor    ax, ax
        push   esi
        xor    esi, esi
        db  066h, 039h, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_C77B
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_C77B
        db  068h, 000h, 000h, 000h, 000h
        db  0BAh, 000h, 000h, 000h, 000h
        push   2
        xor    ecx, ecx
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_5_1240
        db  089h, 035h, 000h, 000h, 000h, 000h
loc_5_C77B:
        pop    esi
        ret
sub_5_C77D:
        sub    esp, 4
        mov    word ptr [esp + 2], cx
        push   esi
        xor    esi, esi
        cmp    cx, si
        je     SHORT loc_5_C7BD
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_C7BD
        lea    eax, [esp + 6]
        db  0BAh, 000h, 000h, 000h, 000h
        push   eax
        xor    ecx, ecx
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        push   2
        db  068h, 000h, 000h, 000h, 000h
        call   sub_5_1240
        db  089h, 035h, 000h, 000h, 000h, 000h
loc_5_C7BD:
        pop    esi
        add    esp, 4
        ret
sub_5_C7C2:
        db  0B9h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR sub_5_1409
sub_5_C7CC:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [ebp - 8], edx
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, 122dd4h
        mul    ecx
        mov    ecx, 0f4240h
        div    ecx
        mov    ecx, dword ptr [ebp - 4]
        add    dword ptr [ecx], eax
        adc    dword ptr [ecx + 4], 0
        mov    esp, ebp
        pop    ebp
        ret
sub_5_C7F6:
        xor    edx, edx
        db  0CDh, 020h, 007h, 000h, 005h, 000h
        sub    eax, ecx
        test   eax, eax
        mov    eax, 1
        jg     SHORT loc_5_C80B
        xor    eax, eax
loc_5_C80B:
        ret
sub_5_C80C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        xor    edx, edx
        db  0CDh, 020h, 007h, 000h, 005h, 000h
        lea    ecx, [ebp - 4]
        mov    edx, esi
        mov    dword ptr [ebp - 4], eax
        call   sub_5_C7CC
        test   edi, edi
        je     SHORT loc_5_C837
        db  0CDh, 020h, 083h, 000h, 001h, 000h
loc_5_C837:
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_C7F6
        test   eax, eax
        jne    SHORT loc_5_C859
loc_5_C843:
        test   edi, edi
        je     SHORT loc_5_C84D
        db  0CDh, 020h, 085h, 000h, 001h, 000h
loc_5_C84D:
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_C7F6
        test   eax, eax
        je     SHORT loc_5_C843
loc_5_C859:
        test   edi, edi
        je     SHORT loc_5_C863
        db  0CDh, 020h, 086h, 000h, 001h, 000h
loc_5_C863:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_5_C869:
        mov    eax, dword ptr [ecx + 24h]
        test   eax, eax
        jne    SHORT loc_5_C874
        xor    eax, eax
        jmp    SHORT loc_5_C883
loc_5_C874:
        cmp    dword ptr [eax + 28h], 0
        je     SHORT loc_5_C883
loc_5_C87A:
        mov    eax, dword ptr [eax + 28h]
        cmp    dword ptr [eax + 28h], 0
        jne    SHORT loc_5_C87A
loc_5_C883:
        ret
sub_5_C884:
        mov    eax, dword ptr [ecx + 20h]
        test   eax, eax
        jne    SHORT loc_5_C88F
        xor    eax, eax
        jmp    SHORT loc_5_C8A7
loc_5_C88F:
        mov    eax, dword ptr [eax + 24h]
        cmp    eax, ecx
        jne    SHORT loc_5_C89A
        xor    eax, eax
        jmp    SHORT loc_5_C8A7
loc_5_C89A:
        cmp    dword ptr [eax + 28h], ecx
        je     SHORT loc_5_C8A7
loc_5_C89F:
        mov    eax, dword ptr [eax + 28h]
        cmp    dword ptr [eax + 28h], ecx
        jne    SHORT loc_5_C89F
loc_5_C8A7:
        ret
sub_5_C8A8:
        push   esi
        db  03Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    esi, ecx
        jne    SHORT loc_5_C8B7
        xor    eax, eax
        jmp    SHORT loc_5_C8CB
loc_5_C8B7:
        push   esi
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_5_C8CB
        db  089h, 035h, 000h, 000h, 000h, 000h
loc_5_C8CB:
        pop    esi
        ret
loc_5_C8CD:
        push   esi
        mov    esi, dword ptr [esp + 8]
loc_5_C8D2:
        test   esi, esi
        je     SHORT loc_5_C8E8
        mov    ecx, esi
        mov    esi, dword ptr [esi + 20h]
        call   sub_5_C8A8
        test   eax, eax
        jne    SHORT loc_5_C8D2
        xor    eax, eax
        jmp    SHORT loc_5_C8ED
loc_5_C8E8:
        mov    eax, 1
loc_5_C8ED:
        pop    esi
        ret
sub_5_C8EF:
        push   esi
        mov    esi, dword ptr [esp + 8]
        test   esi, esi
        jne    SHORT loc_5_C8FF
        mov    eax, 1
        jmp    SHORT loc_5_C91A
loc_5_C8FF:
        push   dword ptr [esi + 20h]
        call   sub_5_C8EF
        add    esp, 4
        test   eax, eax
        mov    eax, 0
        je     SHORT loc_5_C91A
        mov    ecx, esi
        call   sub_5_C8A8
loc_5_C91A:
        pop    esi
        ret
sub_5_C91C:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        test   esi, esi
        je     SHORT loc_5_C97A
        push   dword ptr [esi + 24h]
        call   sub_5_C91C
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_5_C939
        xor    eax, eax
        jmp    SHORT loc_5_C97F
loc_5_C939:
        mov    edi, dword ptr [esi + 28h]
        mov    ecx, esi
        call   sub_5_C8A8
        test   eax, eax
        jne    SHORT loc_5_C94B
        xor    eax, eax
        jmp    SHORT loc_5_C97F
loc_5_C94B:
        db  039h, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_C97A
        test   edi, edi
        je     SHORT loc_5_C97A
loc_5_C957:
        push   dword ptr [edi + 24h]
        mov    esi, dword ptr [edi + 28h]
        call   sub_5_C91C
        add    esp, 4
        test   eax, eax
        je     SHORT loc_5_C982
        mov    ecx, edi
        call   sub_5_C8A8
        test   eax, eax
        je     SHORT loc_5_C986
        mov    edi, esi
        test   esi, esi
        jne    SHORT loc_5_C957
loc_5_C97A:
        mov    eax, 1
loc_5_C97F:
        pop    edi
        pop    esi
        ret
loc_5_C982:
        xor    eax, eax
        jmp    SHORT loc_5_C97F
loc_5_C986:
        xor    eax, eax
        jmp    SHORT loc_5_C97F
sub_5_C98A:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        test   esi, esi
        je     SHORT loc_5_CA08
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_C9C0
        mov    ecx, esi
        call   sub_5_C8A8
        test   eax, eax
        jne    SHORT loc_5_C9AC
        xor    eax, eax
        jmp    SHORT loc_5_CA0D
loc_5_C9AC:
        xor    edx, edx
        mov    ecx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     SHORT loc_5_CA08
        mov    esi, dword ptr [esi + 24h]
        test   esi, esi
        je     SHORT loc_5_CA08
loc_5_C9C0:
        mov    ebx, dword ptr [esi + 20h]
        mov    edi, dword ptr [esi + 28h]
        mov    ecx, esi
        call   sub_5_C8A8
        test   eax, eax
        jne    SHORT loc_5_C9D5
        xor    eax, eax
        jmp    SHORT loc_5_CA0D
loc_5_C9D5:
        test   edi, edi
        je     SHORT loc_5_C9EB
        mov    ecx, edi
        mov    edi, dword ptr [edi + 28h]
        call   sub_5_C8A8
        test   eax, eax
        jne    SHORT loc_5_C9D5
        xor    eax, eax
        jmp    SHORT loc_5_CA0D
loc_5_C9EB:
        mov    esi, dword ptr [ebx + 24h]
        test   esi, esi
        je     SHORT loc_5_CA08
loc_5_C9F2:
        push   dword ptr [esi + 24h]
        call   sub_5_C98A
        add    esp, 4
        test   eax, eax
        je     SHORT loc_5_CA11
        mov    esi, dword ptr [esi + 28h]
        test   esi, esi
        jne    SHORT loc_5_C9F2
loc_5_CA08:
        mov    eax, 1
loc_5_CA0D:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_CA11:
        xor    eax, eax
        jmp    SHORT loc_5_CA0D
sub_5_CA15:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_CA29
        mov    eax, 1
        jmp    SHORT loc_5_CA44
loc_5_CA29:
        push   dword ptr [esi + 20h]
        call   sub_5_CA15
        add    esp, 4
        test   eax, eax
        mov    eax, 0
        je     SHORT loc_5_CA44
        mov    ecx, esi
        call   sub_5_C8A8
loc_5_CA44:
        pop    esi
        ret
loc_5_CA46:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [eax + 20h]
loc_5_CA4F:
        test   esi, esi
        je     SHORT loc_5_CA65
        mov    ecx, esi
        mov    esi, dword ptr [esi + 20h]
        call   sub_5_C8A8
        test   eax, eax
        jne    SHORT loc_5_CA4F
        xor    eax, eax
        jmp    SHORT loc_5_CA6A
loc_5_CA65:
        mov    eax, 1
loc_5_CA6A:
        pop    esi
        ret
_PTEXT ends
        end
