; configmg_pcode_04.asm -- CONFIGMG object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  066h, 08Bh, 002h, 053h, 056h, 057h, 066h, 0BEh, 001h, 000h, 066h, 033h, 0FFh, 066h, 085h, 0C0h
        db  074h, 014h, 00Fh, 0B7h, 0DFh, 06Bh, 0DBh, 00Ch, 066h, 039h, 074h, 013h, 00Ch, 074h, 007h, 066h
        db  047h, 066h, 03Bh, 0F8h, 072h, 0ECh, 066h, 03Bh, 0F8h, 074h, 011h, 066h, 046h, 066h, 083h, 0FEh
        db  009h, 072h, 0D7h, 066h, 0C7h, 001h, 008h, 000h, 05Fh, 05Eh, 05Bh, 0C3h, 066h, 089h, 031h, 0EBh
        db  0F7h
sub_4_0041:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        xor    bx, bx
        push   edi
        cmp    word ptr [eax], bx
        je     SHORT loc_4_0089
loc_4_0056:
        movzx  esi, bx
        imul   esi, esi, 0ch
        cmp    dword ptr [esi + eax + 2], ecx
        lea    edi, [esi + eax]
        jne    SHORT loc_4_0082
        cmp    dword ptr [edi + 6], edx
        jne    SHORT loc_4_0082
        mov    si, word ptr [ebp + 8]
        cmp    word ptr [edi + 0ah], si
        jne    SHORT loc_4_0082
        mov    si, word ptr [ebp + 0ch]
        cmp    word ptr [edi + 0ch], si
        je     NEAR PTR loc_4_011A
loc_4_0082:
        inc    bx
        cmp    bx, word ptr [eax]
        jb     SHORT loc_4_0056
loc_4_0089:
        mov    word ptr [ebp - 2], 0
        xor    bx, bx
        cmp    word ptr [eax], bx
        je     SHORT loc_4_00D6
loc_4_0097:
        cmp    word ptr [ebp - 2], bx
        je     SHORT loc_4_00B5
        movzx  esi, word ptr [ebp - 2]
        movzx  edi, bx
        imul   esi, esi, 0ch
        imul   edi, edi, 0ch
        lea    esi, [esi + eax + 2]
        lea    edi, [edi + eax + 2]
        movsd
        movsd
        movsd
loc_4_00B5:
        movzx  edi, word ptr [ebp - 2]
        imul   edi, edi, 0ch
        mov    si, word ptr [ebp + 0ch]
        cmp    word ptr [edi + eax + 0ch], si
        je     SHORT loc_4_00C9
        inc    bx
loc_4_00C9:
        inc    word ptr [ebp - 2]
        mov    si, word ptr [ebp - 2]
        cmp    word ptr [eax], si
        ja     SHORT loc_4_0097
loc_4_00D6:
        cmp    bx, 9
        jne    SHORT loc_4_00DE
        dec    bx
loc_4_00DE:
        movzx  esi, bx
        imul   esi, esi, 0ch
        mov    dword ptr [esi + eax + 2], ecx
        lea    edi, [esi + eax]
        mov    cx, word ptr [ebp + 8]
        mov    dword ptr [edi + 6], edx
        mov    si, word ptr [ebp + 0ch]
        push   6eh
        inc    bx
        push   eax
        mov    word ptr [edi + 0ah], cx
        push   3
        mov    word ptr [edi + 0ch], si
        push   0
        mov    word ptr [eax], bx
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp + 14h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0117:
        add    esp, 18h
loc_4_011A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
sub_4_0123:
        push   ebp
        mov    ebp, esp
        sub    esp, 4ch
        push   ebx
        mov    dword ptr [ebp - 4], edx
        push   esi
        push   edi
        db  0BEh, 000h, 000h, 000h, 000h
        lea    edi, [ebp - 4ch]
        movsd
        movsd
        movsd
        movsb
        lea    edx, [ebp - 0ch]
        lea    edi, [ebp - 0ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0146:
        mov    edx, 0ffffffffh
        sub    eax, eax
        mov    ecx, edx
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    ebx, ecx
        mov    esi, edi
        mov    ecx, edx
        lea    edi, [ebp - 4ch]
        sub    eax, eax
        repne scasb
        add    edi, edx
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, edx
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        lea    edx, [ebp - 4ch]
        push   dword ptr [ebp - 4]
        push   1
        push   0
        push   edx
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0191:
        add    esp, 18h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_4_019D:
        push   ebp
        mov    ebp, esp
        sub    esp, 50h
        push   ebx
        push   esi
        xor    si, si
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    bx, word ptr [edi]
        cmp    bx, si
        je     SHORT loc_4_0222
loc_4_01B4:
        movzx  eax, si
        imul   eax, eax, 0ch
        mov    edx, dword ptr [ebp + 14h]
        lea    ecx, [eax + edi]
        cmp    dword ptr [eax + edi + 2], edx
        jne    SHORT loc_4_021B
        mov    eax, dword ptr [ebp + 18h]
        cmp    dword ptr [ecx + 6], eax
        jne    SHORT loc_4_021B
        mov    ax, word ptr [ebp + 1ch]
        cmp    word ptr [ecx + 0ah], ax
        jne    SHORT loc_4_021B
        push   dword ptr [ebp + 20h]
        mov    cx, word ptr [ecx + 0ch]
        lea    edx, [ebp - 50h]
        push   edx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_01E9:
        add    esp, 0ch
        lea    eax, [ebp - 50h]
        mov    edx, dword ptr [ebp + 10h]
loc_4_01F2:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    SHORT loc_4_0212
        or     cl, cl
        je     SHORT loc_4_020E
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    SHORT loc_4_0212
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    SHORT loc_4_01F2
loc_4_020E:
        xor    eax, eax
        jmp    SHORT loc_4_0217
loc_4_0212:
        sbb    eax, eax
        sbb    eax, -1
loc_4_0217:
        test   eax, eax
        je     SHORT loc_4_022B
loc_4_021B:
        inc    si
        cmp    si, bx
        jb     SHORT loc_4_01B4
loc_4_0222:
        xor    eax, eax
loc_4_0224:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_022B:
        movzx  eax, si
        imul   eax, eax, 0ch
        mov    cx, word ptr [eax + edi + 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx], cx
        mov    eax, 1
        jmp    SHORT loc_4_0224
sub_4_0243:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        mov    ebx, dword ptr [ebp + 18h]
        push   ebx
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        push   edi
        push   esi
        call   sub_4_019D
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_4_02A6
        mov    edx, edi
        mov    ecx, esi
        call   0
        mov    si, word ptr [esi]
        push   ebx
        push   edi
        mov    edx, dword ptr [ebp + 10h]
        push   esi
        mov    ecx, dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 14h]
        call   sub_4_0041
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, esi
        call   sub_4_0123
        cmp    word ptr [edi], 1
        jbe    SHORT loc_4_02A6
        movzx  ecx, si
        mov    edx, 5
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_02A6:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
sub_4_02AD:
        push   ebp
        mov    ebp, esp
        sub    esp, 10ch
        push   ebx
        push   esi
        push   edi
        mov    esi, edx
        lea    edx, [ebp - 0ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_02C3:
        push   5
        lea    eax, [ebp - 0ch]
        push   eax
        push   1
        push   0
        lea    edi, [ebp - 10ch]
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        db  0BEh, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_02E3:
        add    esp, 18h
        mov    edx, 0ffffffffh
        mov    ecx, edx
        sub    eax, eax
        movsd
        movsd
        lea    edi, [ebp - 0ch]
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    ebx, ecx
        mov    esi, edi
        mov    ecx, edx
        lea    edi, [ebp - 10ch]
        sub    eax, eax
        repne scasb
        add    edi, edx
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 10ch]
        push   eax
        push   ecx
        push   80000002h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_032F:
        add    esp, 0ch
        push   80000005h
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_033F:
        add    esp, 8
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_034A:
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_0354:
        push   ebp
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        mov    ecx, edx
        sub    esp, 160h
        and    ecx, 4
        mov    eax, 1
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        cmp    ecx, eax
        sbb    ecx, ecx
        and    edx, 1000h
        neg    ecx
        shr    edx, 0ch
        mov    dword ptr [ebp - 8], ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 0ch], edx
        cmp    ecx, eax
        sbb    ebx, ebx
        add    ebx, eax
        cmp    dword ptr [ebp - 8], esi
        je     SHORT loc_4_03A4
        test   ebx, ebx
        jne    SHORT loc_4_03A4
        cmp    edx, esi
        jne    NEAR PTR loc_4_06A0
loc_4_03A4:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_4_03BE
        test   ebx, ebx
        je     SHORT loc_4_03B8
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_4_069E
loc_4_03B8:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_4_0414
loc_4_03BE:
        test   ebx, ebx
        je     SHORT loc_4_0414
        mov    eax, dword ptr [ecx + 0ch]
        shr    eax, 0ch
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx + 8]
        add    eax, 0fffh
        shr    eax, 0ch
        cmp    dword ptr [ebp - 0ch], 0
        db  0A3h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_0414
        push   1
        push   22ch
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_03EF:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_4_0405
        mov    edi, eax
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    ecx, 8bh
        rep movsd
loc_4_0405:
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, 1
        jmp    NEAR PTR loc_4_06A0
loc_4_0414:
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   80000002h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0427:
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_4_069E
        lea    eax, [ebp - 14h]
        lea    ecx, [ebp - 160h]
        mov    dword ptr [ebp - 14h], 6eh
        push   eax
        push   ecx
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0455:
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_4_0462
        cmp    dword ptr [ebp - 14h], 6eh
        je     SHORT loc_4_0473
loc_4_0462:
        lea    edi, [ebp - 160h]
        xor    eax, eax
        mov    ecx, 1bh
        rep stosd
        stosw
loc_4_0473:
        test   ebx, ebx
        je     NEAR PTR loc_4_0575
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        lea    edi, [ebp - 0f0h]
        add    esi, 10h
        mov    ecx, 17h
        mov    dword ptr [ebp - 18h], 5
        rep movsd
        movzx  eax, word ptr [ebp - 0e6h]
        cmp    word ptr [ebp - 0e6h], 1
        sbb    esi, esi
        neg    esi
        test   eax, eax
        je     SHORT loc_4_04E2
        cmp    eax, 0ffffh
        je     SHORT loc_4_0514
        lea    ecx, [ebp - 160h]
        mov    edx, dword ptr [ebp - 0ech]
        push   dword ptr [ebp - 4]
        push   ecx
        push   dword ptr [ebp - 0e6h]
        push   dword ptr [ebp - 0e8h]
        mov    ecx, dword ptr [ebp - 0f0h]
        call   sub_4_0041
        jmp    NEAR PTR loc_4_0611
loc_4_04E2:
        lea    eax, [ebp - 0e4h]
        lea    edx, [ebp - 160h]
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 0e8h]
        push   dword ptr [ebp - 0ech]
        push   dword ptr [ebp - 0f0h]
        push   eax
        lea    ecx, [ebp - 0e6h]
        call   sub_4_0243
        jmp    NEAR PTR loc_4_0611
loc_4_0514:
        lea    eax, [ebp - 18h]
        lea    ecx, [ebp - 24h]
        push   eax
        xor    edi, edi
        push   ecx
        push   edi
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_052D:
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_4_0693
        mov    word ptr [ebp - 0e6h], di
        lea    edx, [ebp - 24h]
        mov    eax, 4
loc_4_0547:
        shl    word ptr [ebp - 0e6h], 4
        mov    cl, byte ptr [edx]
        cmp    cl, 39h
        movsx  di, cl
        mov    cx, 37h
        jg     SHORT loc_4_0562
        mov    cx, 30h
loc_4_0562:
        sub    di, cx
        inc    edx
        add    word ptr [ebp - 0e6h], di
        dec    eax
        jne    SHORT loc_4_0547
        jmp    NEAR PTR loc_4_0611
loc_4_0575:
        xor    eax, eax
        cmp    word ptr [ebp - 160h], ax
        jne    SHORT loc_4_05A4
        push   dword ptr [ebp - 4]
        push   eax
        push   eax
        push   eax
        push   5
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_058D:
        add    esp, 4
        lea    edx, [ebp - 160h]
        lea    ecx, [ebp - 0e6h]
        push   eax
        call   sub_4_0243
        jmp    SHORT loc_4_0611
loc_4_05A4:
        cmp    word ptr [ebp - 160h], 1
        jne    SHORT loc_4_05B7
        mov    ax, word ptr [ebp - 154h]
        jmp    SHORT loc_4_060A
loc_4_05B7:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_4_05D2
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_05C5:
        add    esp, 4
        mov    eax, 1
        jmp    NEAR PTR loc_4_06A0
loc_4_05D2:
        xor    eax, eax
        lea    ecx, [ebp - 1ch]
        push   ecx
        db  0BAh, 000h, 000h, 000h, 000h
        push   8
        db  0B9h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 18h], 0ffffffffh
        push   eax
        mov    dword ptr [ebp - 1ch], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_05F4:
        cmp    eax, 21h
        jb     NEAR PTR loc_4_0693
        cmp    eax, 29h
        ja     NEAR PTR loc_4_0693
        sub    ax, 20h
loc_4_060A:
        mov    word ptr [ebp - 0e6h], ax
loc_4_0611:
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0e6h]
        call   sub_4_02AD
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0627:
        add    esp, 4
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_4_066C
        test   ebx, ebx
        je     SHORT loc_4_066C
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_4_066C
        lea    edx, [ebp - 10h]
        xor    ecx, ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0644:
        test   byte ptr [ebp - 10h], 4
        je     SHORT loc_4_065C
        test   esi, esi
        jne    SHORT loc_4_065C
        and    byte ptr [ebp - 10h], 0fbh
        mov    edx, dword ptr [ebp - 10h]
        xor    ecx, ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_065C:
        test   dword ptr [ebp - 10h], 80000003h
        je     SHORT loc_4_066C
        db  080h, 00Dh, 000h, 000h, 000h, 000h, 020h
loc_4_066C:
        xor    esi, esi
        cmp    dword ptr [ebp - 8], esi
        jne    SHORT loc_4_069E
        test   ebx, ebx
        je     SHORT loc_4_069E
        cmp    dword ptr [ebp - 0ch], esi
        je     SHORT loc_4_069E
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0688:
        add    esp, 8
        db  089h, 035h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_069E
loc_4_0693:
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_069B:
        add    esp, 4
loc_4_069E:
        xor    eax, eax
loc_4_06A0:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
_PTEXT ends
        end
