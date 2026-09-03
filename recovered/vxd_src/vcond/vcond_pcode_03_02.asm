; vcond_pcode_03_02.asm -- VCOND object 3 segment _PTEXT  (part 2/2)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_3_4064:
        push   ebp
        mov    ebp, esp
        sub    esp, 30h
        mov    dword ptr [ebp - 4], 1
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [edi]
        cmp    edx, 8
        jne    SHORT loc_3_409E
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        or     byte ptr [esi + 28h], 4
        jmp    NEAR PTR loc_3_41C5
loc_3_409E:
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 18h], 1
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp - 18h]
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        cmp    edx, 1
        jne    NEAR PTR loc_3_4198
        lea    eax, [ebp - 30h]
        push   eax
        push   esi
        call   sub_3_149E
        test   eax, eax
        je     NEAR PTR loc_3_4198
        cmp    dword ptr [ebp - 30h], 1
        jne    NEAR PTR loc_3_4198
        movsx  edx, word ptr [ebp - 26h]
        movsx  ebx, word ptr [edi + 0ah]
        lea    ecx, [edx - 1]
        cmp    ebx, ecx
        jl     SHORT loc_3_410B
        movsx  eax, word ptr [ebp - 22h]
        inc    eax
        cmp    eax, ebx
        jge    SHORT loc_3_4135
loc_3_410B:
        movsx  eax, word ptr [edi + 0eh]
        cmp    eax, ecx
        jl     SHORT loc_3_411C
        movsx  ecx, word ptr [ebp - 22h]
        inc    ecx
        cmp    ecx, eax
        jge    SHORT loc_3_4135
loc_3_411C:
        dec    ebx
        cmp    ebx, edx
        jg     SHORT loc_3_4128
        lea    ecx, [eax + 1]
        cmp    ecx, edx
        jge    SHORT loc_3_4135
loc_3_4128:
        movsx  ecx, word ptr [ebp - 22h]
        cmp    ecx, ebx
        jl     SHORT loc_3_4198
        inc    eax
        cmp    eax, edx
        jl     SHORT loc_3_4198
loc_3_4135:
        mov    ax, word ptr [edi + 8]
        mov    cx, word ptr [ebp - 28h]
        cmp    ax, cx
        jl     SHORT loc_3_4145
        mov    ax, cx
loc_3_4145:
        mov    cx, word ptr [edi + 0ah]
        mov    word ptr [ebp - 28h], ax
        mov    ax, word ptr [ebp - 26h]
        cmp    ax, cx
        jl     SHORT loc_3_4159
        mov    ax, cx
loc_3_4159:
        mov    cx, word ptr [ebp - 24h]
        mov    word ptr [ebp - 26h], ax
        mov    ax, word ptr [edi + 0ch]
        cmp    ax, cx
        jg     SHORT loc_3_416D
        mov    ax, cx
loc_3_416D:
        mov    cx, word ptr [ebp - 22h]
        mov    word ptr [ebp - 24h], ax
        mov    ax, word ptr [edi + 0eh]
        cmp    ax, cx
        jg     SHORT loc_3_4181
        mov    ax, cx
loc_3_4181:
        mov    word ptr [ebp - 22h], ax
        lea    eax, [ebp - 30h]
        push   eax
        push   esi
        call   sub_3_14F8
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_3_41B0
loc_3_4198:
        test   byte ptr [esi + 28h], 40h
        je     SHORT loc_3_41A7
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_3_41B0
loc_3_41A7:
        push   edi
        push   1
        push   esi
        call   sub_3_130B
loc_3_41B0:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_3_41C5
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
loc_3_41C5:
        mov    eax, dword ptr [esi + 28h]
        test   al, 10h
        je     SHORT loc_3_41E9
        and    al, 0efh
        mov    dword ptr [esi + 28h], eax
        test   al, 20h
        je     SHORT loc_3_41E9
        and    al, 0dfh
        mov    ecx, dword ptr [esi + 24h]
        mov    dword ptr [esi + 28h], eax
        mov    dword ptr [ebp - 0ch], ecx
        mov    eax, dword ptr [ebp - 0ch]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
loc_3_41E9:
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_3_4201:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_3_3FA3
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        push   1
        push   esi
        call   sub_3_13BA
        test   eax, eax
        je     SHORT loc_3_424C
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        mov    ecx, 1
        jmp    SHORT loc_3_426C
loc_3_424C:
        mov    eax, dword ptr [esi + 28h]
        test   al, 4
        je     SHORT loc_3_4265
        mov    ecx, 1
        and    al, 0fbh
        mov    dword ptr [esi + 28h], eax
        mov    dword ptr [edi], 8
        jmp    SHORT loc_3_426C
loc_3_4265:
        and    al, 0feh
        xor    ecx, ecx
        mov    dword ptr [esi + 28h], eax
loc_3_426C:
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        mov    eax, ecx
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
sub_3_4285:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        xor    si, si
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_3_3FA3
        mov    eax, dword ptr [edi + 18h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
loc_3_42B0:
        push   ebx
        push   edi
        call   sub_3_153B
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_3_42C2
        or     si, word ptr [ebx]
        jmp    SHORT loc_3_42B0
loc_3_42C2:
        test   si, si
        jne    SHORT loc_3_42CB
        and    byte ptr [edi + 28h], 0feh
loc_3_42CB:
        mov    eax, dword ptr [edi + 28h]
        test   al, 4
        je     SHORT loc_3_42DB
        or     si, 8
        and    al, 0fbh
        mov    dword ptr [edi + 28h], eax
loc_3_42DB:
        mov    eax, dword ptr [edi + 28h]
        test   al, 8
        je     SHORT loc_3_42EB
        or     si, 20h
        and    al, 0f7h
        mov    dword ptr [edi + 28h], eax
loc_3_42EB:
        mov    eax, dword ptr [edi + 18h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        mov    ax, si
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_4306:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 14h
        push   esi
        mov    esi, dword ptr [edx + 0ch]
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        mov    eax, dword ptr [esi + 28h]
        test   al, 1
        jne    SHORT loc_3_4353
        or     al, 1
        mov    ecx, dword ptr [esi + 18h]
        mov    dword ptr [esi + 28h], eax
        mov    dword ptr [ebp - 10h], ecx
        mov    eax, dword ptr [ebp - 10h]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        push   0
        push   41ah
        push   dword ptr [edx]
        call   sub_3_0B98
        jmp    SHORT loc_3_4362
loc_3_4353:
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
loc_3_4362:
        mov    eax, dword ptr [esi + 28h]
        test   al, 80h
        je     SHORT loc_3_4388
        mov    dword ptr [ebp - 4], 1
        or     ah, 1
        mov    ecx, dword ptr [esi + 20h]
        mov    dword ptr [esi + 28h], eax
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        db  0CDh, 020h, 027h, 000h, 001h, 000h
loc_3_4388:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_438F:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 14h
        mov    edx, dword ptr [eax + 0ch]
        mov    ecx, dword ptr [edx + 18h]
        mov    dword ptr [ebp - 14h], ecx
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        mov    eax, dword ptr [edx + 10h]
        cmp    dword ptr [edx + 0ch], eax
        je     SHORT loc_3_43E6
        or     byte ptr [edx + 28h], 2
        mov    eax, dword ptr [edx + 18h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        mov    dword ptr [ebp - 8], 1
        mov    eax, dword ptr [edx + 20h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        jmp    SHORT loc_3_4402
loc_3_43E6:
        mov    eax, dword ptr [edx + 28h]
        mov    ecx, dword ptr [edx + 18h]
        and    al, 0fdh
        mov    dword ptr [ebp - 4], ecx
        mov    dword ptr [edx + 28h], eax
        or     al, 10h
        mov    dword ptr [edx + 28h], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
loc_3_4402:
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_4408:
        push   esi
        db  0BAh, 000h, 000h, 000h, 000h
        xor    esi, esi
        mov    ecx, dword ptr [esp + 8]
loc_3_4414:
        mov    eax, dword ptr [edx]
        test   eax, eax
        je     SHORT loc_3_4425
        cmp    dword ptr [eax], ecx
        jne    SHORT loc_3_4425
        mov    eax, dword ptr [eax + 0ch]
        test   eax, eax
        jne    SHORT loc_3_4430
loc_3_4425:
        inc    esi
        add    edx, 4
        cmp    esi, 40h
        jl     SHORT loc_3_4414
        jmp    SHORT loc_3_4434
loc_3_4430:
        or     byte ptr [eax + 28h], 8
loc_3_4434:
        pop    esi
        ret    4
sub_3_4438:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_3_193C
        test   eax, eax
        je     NEAR PTR loc_3_455B
        movzx  edx, word ptr [ebp + 0ch]
        cmp    edx, 3
        je     NEAR PTR loc_3_454B
        cmp    edx, 4
        je     NEAR PTR loc_3_4551
        cmp    edx, 7
        je     NEAR PTR loc_3_455B
        cmp    edx, 8
        je     NEAR PTR loc_3_4557
        mov    esi, dword ptr [eax + 0ch]
        test   esi, esi
        je     NEAR PTR loc_3_455B
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        cmp    edx, 1
        je     SHORT loc_3_44AF
        cmp    edx, 2
        je     SHORT loc_3_4503
        cmp    edx, 5
        je     SHORT loc_3_4512
        cmp    edx, 6
        je     SHORT loc_3_4518
        jmp    NEAR PTR loc_3_453A
loc_3_44AF:
        test   byte ptr [esi + 28h], 40h
        jne    NEAR PTR loc_3_455B
        lea    eax, [ebp - 24h]
        push   eax
        push   1
        push   esi
        call   sub_3_13BA
        test   eax, eax
        je     SHORT loc_3_44E8
loc_3_44C9:
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        lea    eax, [ebp - 24h]
        push   eax
        push   1
        push   esi
        call   sub_3_13BA
        test   eax, eax
        jne    SHORT loc_3_44C9
loc_3_44E8:
        or     byte ptr [esi + 28h], 40h
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        push   esi
        call   sub_3_3FA3
        jmp    SHORT loc_3_455B
loc_3_4503:
        mov    eax, dword ptr [esi + 28h]
        and    al, 0bfh
        mov    dword ptr [esi + 28h], eax
        and    al, 0feh
        mov    dword ptr [esi + 28h], eax
        jmp    SHORT loc_3_453A
loc_3_4512:
        or     byte ptr [esi + 28h], 80h
        jmp    SHORT loc_3_453A
loc_3_4518:
        mov    eax, dword ptr [esi + 28h]
        and    al, 7fh
        test   ah, 1
        mov    dword ptr [esi + 28h], eax
        je     SHORT loc_3_453A
        and    ah, 0feh
        mov    ecx, dword ptr [esi + 20h]
        mov    dword ptr [esi + 28h], eax
        mov    dword ptr [ebp - 0ch], ecx
        mov    eax, dword ptr [ebp - 0ch]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
loc_3_453A:
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        jmp    SHORT loc_3_455B
loc_3_454B:
        and    byte ptr [eax + 1ch], 0efh
        jmp    SHORT loc_3_455B
loc_3_4551:
        or     byte ptr [eax + 1ch], 10h
        jmp    SHORT loc_3_455B
loc_3_4557:
        and    byte ptr [eax + 1ch], 0dfh
loc_3_455B:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_3_4562:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_3_193C
        mov    esi, dword ptr [eax + 0ch]
        mov    edx, eax
        and    byte ptr [esi + 28h], 0feh
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        mov    eax, dword ptr [esi + 10h]
        cmp    dword ptr [esi + 0ch], eax
        mov    eax, dword ptr [esi + 18h]
        je     SHORT loc_3_45AD
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        push   edx
        call   sub_3_4306
        jmp    SHORT loc_3_45B9
loc_3_45AD:
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
loc_3_45B9:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_45C0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 20h
        push   esi
        mov    esi, dword ptr [eax + 0ch]
        mov    ecx, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 10h], ecx
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        lea    eax, [ebp - 20h]
        push   eax
        push   1
        push   esi
        call   sub_3_13BA
        test   eax, eax
        je     SHORT loc_3_4611
loc_3_45F2:
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        lea    eax, [ebp - 20h]
        push   eax
        push   1
        push   esi
        call   sub_3_13BA
        test   eax, eax
        jne    SHORT loc_3_45F2
loc_3_4611:
        test   byte ptr [esi + 28h], 20h
        je     SHORT loc_3_462A
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        and    byte ptr [esi + 28h], 0dfh
loc_3_462A:
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_4640:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        test   esi, esi
        jne    SHORT loc_3_4650
        db  0BEh, 000h, 000h, 000h, 000h
loc_3_4650:
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        push   0
        repne scasb
        not    ecx
        push   ecx
        mov    ebx, ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_4667:
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_4672
        xor    eax, eax
        jmp    SHORT loc_3_468D
loc_3_4672:
        mov    edi, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, 1
loc_3_468D:
        pop    edi
        pop    esi
        pop    ebx
        ret    8
sub_3_4693:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   1
        push   4ch
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_46A5:
        add    esp, 8
        mov    ebx, eax
        xor    ecx, ecx
        test   ebx, ebx
        je     NEAR PTR loc_3_4787
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [ebx], 52444952h
        mov    dword ptr [ebx + 4], ecx
        mov    dword ptr [ebx + 48h], eax
        mov    dword ptr [ebp - 18h], ecx
        mov    ecx, dword ptr [ebp - 18h]
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        jae    SHORT loc_3_46D3
        xor    eax, eax
loc_3_46D3:
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebx + 8], eax
        test   eax, eax
        je     NEAR PTR loc_3_477C
        mov    dword ptr [ebp - 10h], 0
        mov    ecx, dword ptr [ebp - 10h]
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        jae    SHORT loc_3_46F8
        xor    eax, eax
loc_3_46F8:
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebx + 0ch], eax
        test   eax, eax
        je     SHORT loc_3_476D
        push   dword ptr [ebp + 8]
        lea    esi, [ebx + 20h]
        push   esi
        call   sub_3_4640
        test   eax, eax
        je     SHORT loc_3_475E
        push   dword ptr [ebp + 0ch]
        lea    edi, [ebx + 24h]
        push   edi
        call   sub_3_4640
        test   eax, eax
        je     SHORT loc_3_4752
        push   dword ptr [ebp + 10h]
        lea    ecx, [ebx + 28h]
        push   ecx
        call   sub_3_4640
        test   eax, eax
        je     SHORT loc_3_4746
        lea    edi, [ebx + 2ch]
        mov    esi, dword ptr [ebp + 14h]
        mov    ecx, 6
        rep movsd
        mov    eax, ebx
        jmp    SHORT loc_3_4789
loc_3_4746:
        push   0
        push   dword ptr [edi]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_474F:
        add    esp, 8
loc_3_4752:
        push   0
        push   dword ptr [esi]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_475B:
        add    esp, 8
loc_3_475E:
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
loc_3_476D:
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
loc_3_477C:
        push   0
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_4784:
        add    esp, 8
loc_3_4787:
        xor    eax, eax
loc_3_4789:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
sub_3_4792:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   0
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [esi + 28h]
        mov    dword ptr [esi], 0
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_47AC:
        add    esp, 8
        push   0
        push   dword ptr [esi + 24h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_47B9:
        add    esp, 8
        push   0
        push   dword ptr [esi + 20h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_47C6:
        add    esp, 8
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_47EF:
        add    esp, 8
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_47F9:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0ch
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        mov    dword ptr [ebp - 4], 1
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_4831:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 8
        mov    ecx, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], 1
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_485A:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ecx + 4]
        test   al, 2
        je     SHORT loc_3_4873
        push   ecx
        call   sub_3_4792
        jmp    SHORT loc_3_4887
loc_3_4873:
        or     al, 1
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [ecx + 0ch]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
loc_3_4887:
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_488D:
        push   ebp
        mov    eax, 80000002h
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        test   byte ptr [edx + 4], 1
        jne    NEAR PTR loc_3_4924
        mov    ecx, dword ptr [edx + 48h]
        mov    eax, dword ptr [ebp + 1ch]
        mov    edi, dword ptr [edx + 20h]
        mov    dword ptr [eax], ecx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [ebp + 0ch]
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [edx + 24h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [ebp + 10h]
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [edx + 28h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [ebp + 14h]
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    esi, [edx + 2ch]
        mov    edi, dword ptr [ebp + 18h]
        mov    ecx, 6
        rep movsd
        xor    eax, eax
loc_3_4924:
        pop    edi
        pop    esi
        pop    ebp
        ret    18h
sub_3_492A:
        mov    edx, dword ptr [esp + 4]
        push   edi
        test   byte ptr [edx + 4], 1
        je     SHORT loc_3_493C
        mov    eax, 80000002h
        jmp    SHORT loc_3_4984
loc_3_493C:
        mov    edi, dword ptr [edx + 20h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        cmp    ecx, 0ffffh
        jbe    SHORT loc_3_4959
        mov    eax, 80000001h
        jmp    SHORT loc_3_4984
loc_3_4959:
        mov    eax, dword ptr [esp + 0ch]
        mov    edi, dword ptr [edx + 24h]
        mov    word ptr [eax], cx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, 80000001h
        cmp    ecx, 0ffffh
        ja     SHORT loc_3_4984
        mov    eax, dword ptr [esp + 10h]
        mov    word ptr [eax], cx
        xor    eax, eax
loc_3_4984:
        pop    edi
        ret    0ch
sub_3_4988:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 4], 1
        je     SHORT loc_3_499F
        mov    eax, 80000002h
        jmp    SHORT loc_3_49FC
loc_3_499F:
        mov    al, byte ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [esi + 12h], cx
        mov    edx, dword ptr [ebp + 10h]
        mov    byte ptr [esi + 10h], al
        mov    dword ptr [esi + 14h], edx
        mov    byte ptr [esi + 11h], 1
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        test   edx, edx
        je     SHORT loc_3_49D5
        db  0CDh, 020h, 020h, 000h, 001h, 000h
loc_3_49D5:
        mov    dword ptr [ebp - 4], 1
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        test   edx, edx
        je     SHORT loc_3_49F9
        push   1
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_49F9:
        mov    eax, dword ptr [esi + 18h]
loc_3_49FC:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
sub_3_4A03:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 4], 1
        je     SHORT loc_3_4A1A
        mov    eax, 80000002h
        jmp    SHORT loc_3_4A77
loc_3_4A1A:
        mov    al, byte ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [esi + 12h], cx
        mov    edx, dword ptr [ebp + 10h]
        mov    byte ptr [esi + 10h], al
        mov    dword ptr [esi + 14h], edx
        mov    byte ptr [esi + 11h], 5
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        test   edx, edx
        je     SHORT loc_3_4A50
        db  0CDh, 020h, 020h, 000h, 001h, 000h
loc_3_4A50:
        mov    dword ptr [ebp - 4], 1
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        test   edx, edx
        je     SHORT loc_3_4A74
        push   1
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_4A74:
        mov    eax, dword ptr [esi + 18h]
loc_3_4A77:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
sub_3_4A7E:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 4], 1
        je     SHORT loc_3_4A95
        mov    eax, 80000002h
        jmp    SHORT loc_3_4AF2
loc_3_4A95:
        mov    al, byte ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [esi + 12h], cx
        mov    edx, dword ptr [ebp + 10h]
        mov    byte ptr [esi + 10h], al
        mov    dword ptr [esi + 14h], edx
        mov    byte ptr [esi + 11h], 2
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        test   edx, edx
        je     SHORT loc_3_4ACB
        db  0CDh, 020h, 020h, 000h, 001h, 000h
loc_3_4ACB:
        mov    dword ptr [ebp - 4], 1
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        test   edx, edx
        je     SHORT loc_3_4AEF
        push   1
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_4AEF:
        mov    eax, dword ptr [esi + 18h]
loc_3_4AF2:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
sub_3_4AF9:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax + 4], 1
        je     SHORT loc_3_4B0A
        mov    eax, 80000002h
        jmp    SHORT loc_3_4B30
loc_3_4B0A:
        mov    ecx, dword ptr [esp + 8]
        test   ecx, ecx
        je     SHORT loc_3_4B1E
        cmp    ecx, 1
        je     SHORT loc_3_4B24
        cmp    ecx, 2
        je     SHORT loc_3_4B2A
        jmp    SHORT loc_3_4B2E
loc_3_4B1E:
        inc    word ptr [eax + 2eh]
        jmp    SHORT loc_3_4B2E
loc_3_4B24:
        inc    word ptr [eax + 36h]
        jmp    SHORT loc_3_4B2E
loc_3_4B2A:
        inc    word ptr [eax + 3eh]
loc_3_4B2E:
        xor    eax, eax
loc_3_4B30:
        ret    8
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 4], 1
        je     SHORT loc_3_4B4A
        mov    eax, 80000002h
        jmp    SHORT loc_3_4BAF
loc_3_4B4A:
        mov    ecx, dword ptr [ebp + 0ch]
        test   ecx, ecx
        je     SHORT loc_3_4BB6
        cmp    ecx, 1
        je     SHORT loc_3_4BC5
        cmp    ecx, 2
        je     SHORT loc_3_4BD4
        mov    eax, dword ptr [ebp - 10h]
loc_3_4B5E:
        test   eax, eax
        jne    SHORT loc_3_4BAC
        mov    eax, dword ptr [esi + 8]
        mov    byte ptr [esi + 10h], cl
        mov    byte ptr [esi + 11h], 4
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        test   edx, edx
        je     SHORT loc_3_4B88
        db  0CDh, 020h, 020h, 000h, 001h, 000h
loc_3_4B88:
        mov    dword ptr [ebp - 4], 1
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        test   edx, edx
        je     SHORT loc_3_4BAC
        push   1
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_4BAC:
        mov    eax, dword ptr [esi + 18h]
loc_3_4BAF:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_3_4BB6:
        mov    ax, word ptr [esi + 2eh]
        dec    ax
        mov    word ptr [esi + 2eh], ax
        movzx  eax, ax
        jmp    SHORT loc_3_4B5E
loc_3_4BC5:
        mov    ax, word ptr [esi + 36h]
        dec    ax
        mov    word ptr [esi + 36h], ax
        movzx  eax, ax
        jmp    SHORT loc_3_4B5E
loc_3_4BD4:
        mov    ax, word ptr [esi + 3eh]
        dec    ax
        mov    word ptr [esi + 3eh], ax
        movzx  eax, ax
        jmp    NEAR PTR loc_3_4B5E
sub_3_4BE6:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ecx + 4]
        test   al, 1
        je     SHORT loc_3_4BFF
        push   ecx
        call   sub_3_4792
        jmp    SHORT loc_3_4C17
loc_3_4BFF:
        mov    byte ptr [ecx + 11h], 3
        or     al, 2
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp - 4]
        db  0CDh, 020h, 031h, 001h, 001h, 000h
loc_3_4C17:
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_4C1C:
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
sub_3_4C24:
        push   ebx
        db  080h, 03Dh, 0C0h, 003h, 000h, 000h, 000h
        jne    SHORT loc_3_4C57
        db  0CDh, 020h, 000h, 000h, 040h, 000h
        jb     SHORT loc_3_4C5E
        db  068h, 058h, 003h, 000h, 000h
        push   22h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 062h, 000h, 040h, 000h
        add    esp, 0ch
        cmp    eax, 0
        jl     SHORT loc_3_4C5E
        db  0C6h, 005h, 0C0h, 003h, 000h, 000h, 001h
loc_3_4C57:
        mov    eax, 1
loc_3_4C5C:
        pop    ebx
        ret
loc_3_4C5E:
        xor    eax, eax
        jmp    SHORT loc_3_4C5C
sub_3_4C62:
        db  0CDh, 020h, 050h, 080h, 040h, 000h
sub_3_4C68:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0A1h, 0E6h, 001h, 000h, 000h
        add    eax, ebx
        pop    ebx
        ret
sub_3_4C78:
        push   ebx
        db  0CDh, 020h, 000h, 000h, 021h, 000h
        cmp    bl, 2
        je     SHORT loc_3_4C88
        xor    eax, eax
loc_3_4C86:
        pop    ebx
        ret
loc_3_4C88:
        mov    eax, 1
        jmp    SHORT loc_3_4C86
loc_3_4C8F:
        xchg   dword ptr [esp + 4], esi
        mov    word ptr [esi + 1ah], 1fh
        mov    eax, 1fh
        xchg   dword ptr [esp + 4], esi
        ret
        db  000h
_PTEXT ends
        end
