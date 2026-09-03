; pccard_lcode_01_02.asm -- PCCARD object 1 segment _LTEXT  (part 2/3)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_4245:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   ebp
        test   esi, esi
        je     NEAR PTR loc_1_434B
        mov    eax, dword ptr [esi + 14h]
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [ecx + 0ch]
        mov    ebx, dword ptr [edx + 14h]
        test   ebx, ebx
        je     NEAR PTR loc_1_434B
        db  0BDh, 000h, 000h, 000h, 000h
loc_1_426D:
        test   byte ptr [ebx + 0ch], 2
        je     NEAR PTR loc_1_4341
        cmp    dword ptr [ebx + 1ah], esi
        jne    NEAR PTR loc_1_4341
        mov    eax, dword ptr [esi + 14h]
        db  081h, 0FEh, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_4321
        test   byte ptr [eax + 0bh], 18h
        jne    NEAR PTR loc_1_4321
        mov    edi, ebp
        xor    eax, eax
        mov    ecx, 9
        rep stosd
        cmp    byte ptr [esi + 1ah], al
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        jne    SHORT loc_1_42EB
        db  066h, 0C7h, 005h, 002h, 000h, 000h, 000h, 014h, 000h
        mov    eax, dword ptr [esi + 8]
        push   ebp
        db  0A3h, 004h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 0ch]
        db  003h, 005h, 004h, 000h, 000h, 000h
        dec    eax
        db  0A3h, 008h, 000h, 000h, 000h
        mov    ecx, dword ptr [esi + 14h]
        push   dword ptr [ecx]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_42DD:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_4321
        mov    eax, 19h
        jmp    SHORT loc_1_434D
loc_1_42EB:
        db  066h, 0C7h, 005h, 002h, 000h, 000h, 000h, 00Ch, 000h
        mov    eax, dword ptr [esi + 8]
        push   ebp
        db  066h, 0A3h, 004h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 0ch]
        db  066h, 003h, 005h, 004h, 000h, 000h, 000h
        dec    ax
        db  066h, 0A3h, 006h, 000h, 000h, 000h
        mov    ecx, dword ptr [esi + 14h]
        push   dword ptr [ecx]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_431A:
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_4352
loc_1_4321:
        and    word ptr [ebx + 14h], 0fffbh
        push   ebx
        call   sub_1_4053
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_434D
        and    word ptr [ebx + 0ch], 0fffdh
        mov    dword ptr [ebx + 1ah], 0
loc_1_4341:
        mov    ebx, dword ptr [ebx]
        test   ebx, ebx
        jne    NEAR PTR loc_1_426D
loc_1_434B:
        xor    eax, eax
loc_1_434D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_4352:
        mov    eax, 19h
        jmp    SHORT loc_1_434D
sub_1_4359:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   0
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 30h], eax
        mov    dword ptr [ecx + 34h], edx
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ecx + 38h], eax
        call   sub_1_3806
        add    esp, 4
        xor    eax, eax
        pop    ebp
        ret
sub_1_4380:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        mov    edx, dword ptr [ebp + 10h]
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        lea    edi, [ebp - 28h]
        xor    eax, eax
        mov    ecx, 8
        mov    dword ptr [esi], ebx
        mov    dword ptr [edx], ebx
        rep stosd
        stosw
        stosb
        mov    word ptr [ebp - 20h], 6
        mov    byte ptr [ebp - 6], 3ah
        db  0C7h, 045h, 0DCh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 28h], 40000000h
        mov    dword ptr [ebp - 12h], 0ffffffffh
        mov    eax, 1000h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 1ah], eax
        mov    dword ptr [ebp - 16h], eax
        mov    dword ptr [ebp - 0ah], eax
        db  0A3h, 008h, 000h, 000h, 000h
        db  0A3h, 00Ch, 000h, 000h, 000h
        lea    eax, [ebp - 4]
        mov    edx, dword ptr [ecx + 18h]
        push   eax
        lea    ecx, [ebp - 28h]
        db  089h, 015h, 014h, 000h, 000h, 000h
        push   ecx
        call   sub_1_3B7B
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_4408
        mov    ecx, dword ptr [ebp - 1ah]
        mov    ebx, dword ptr [ebp - 12h]
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [esi], ecx
        mov    dword ptr [edx], ebx
loc_1_4408:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], ecx
        test   ecx, ecx
        je     SHORT loc_1_442B
loc_1_4415:
        mov    ecx, dword ptr [ebp - 4]
        and    word ptr [ecx + 0ch], 0fffbh
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 4]
        mov    dword ptr [ebp - 4], edx
        test   edx, edx
        jne    SHORT loc_1_4415
loc_1_442B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_4432:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 24h
        lea    edx, [ecx + 0ch]
        push   esi
        push   edi
        mov    eax, dword ptr [edx]
        cmp    dword ptr [eax + 30h], 0
        jne    SHORT loc_1_4453
        mov    edi, 19h
        jmp    NEAR PTR loc_1_44F7
loc_1_4453:
        mov    eax, dword ptr [eax + 38h]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [edx]
        mov    edi, dword ptr [esi + 30h]
        db  089h, 03Dh, 008h, 000h, 000h, 000h
        lea    edi, [ebp - 24h]
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [esi + 34h]
        db  0A3h, 00Ch, 000h, 000h, 000h
        xor    eax, eax
        mov    ecx, dword ptr [ecx + 18h]
        db  089h, 00Dh, 014h, 000h, 000h, 000h
        mov    ecx, 8
        rep stosd
        stosw
        stosb
        mov    word ptr [ebp - 1ch], 6
        db  08Bh, 03Dh, 008h, 000h, 000h, 000h
        db  0A1h, 00Ch, 000h, 000h, 000h
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 1ah], edi
        push   esi
        mov    dword ptr [ebp - 16h], eax
        mov    byte ptr [ebp - 2], 3ah
        db  0C7h, 045h, 0E0h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 24h]
        push   eax
        call   sub_1_3B7B
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_44F7
        push   dword ptr [esi]
        call   sub_1_4053
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_44E4
        push   dword ptr [esi]
        call   sub_1_4154
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_44E4
        xor    eax, eax
        jmp    SHORT loc_1_44F9
loc_1_44E4:
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_4245
        add    esp, 4
        mov    dword ptr [esi], 0
loc_1_44F7:
        mov    eax, edi
loc_1_44F9:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_44FF:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 1ah]
        call   sub_1_4245
        add    esp, 4
        ret
sub_1_450F:
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 0ch]
        push   edi
        mov    esi, dword ptr [esp + 14h]
        mov    eax, dword ptr [ebx + 16h]
        cmp    eax, esi
        ja     SHORT loc_1_452C
        lea    edi, [ebx + 22h]
        mov    ecx, dword ptr [edi]
        add    ecx, eax
        cmp    ecx, esi
        ja     SHORT loc_1_454A
loc_1_452C:
        lea    edi, [ebx + 22h]
        mov    eax, esi
        sub    edx, edx
        push   ebx
        mov    ecx, dword ptr [edi]
        div    ecx
        imul   eax, ecx
        mov    dword ptr [ebx + 16h], eax
        call   sub_1_4154
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_4560
loc_1_454A:
        mov    eax, esi
        sub    edx, edx
        div    dword ptr [edi]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 18h]
        add    ecx, edx
        mov    dword ptr [eax], ecx
        xor    eax, eax
loc_1_4560:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_4564:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 8]
        mov    dl, byte ptr [eax + 0fh]
        db  0A1h, 00Ah, 000h, 000h, 000h
        mov    byte ptr [eax], dl
        db  0A1h, 00Ah, 000h, 000h, 000h
        mov    cl, byte ptr [ecx + 10h]
        mov    byte ptr [eax + 1], cl
        xor    eax, eax
        ret
loc_1_4583:
        sub    esp, 4
        db  08Bh, 00Dh, 00Ah, 000h, 000h, 000h
        lea    eax, [esp]
        mov    dl, byte ptr [ecx + 1]
        push   eax
        push   edx
        mov    al, byte ptr [ecx]
        push   eax
        call   sub_1_3B15
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_45C3
        mov    eax, dword ptr [esp]
        test   byte ptr [eax + 0ch], 2
        mov    eax, 21h
        je     SHORT loc_1_45C3
        mov    eax, dword ptr [esp]
        mov    ax, word ptr [eax + 0eh]
        db  066h, 0A3h, 002h, 000h, 000h, 000h
        xor    eax, eax
loc_1_45C3:
        add    esp, 4
        ret
loc_1_45C7:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 02Dh, 00Ah, 000h, 000h, 000h
        mov    eax, ebp
        mov    ax, word ptr [eax]
        test   ax, 0fff1h
        je     SHORT loc_1_45E9
        mov    eax, 2
        jmp    NEAR PTR loc_1_46DB
loc_1_45E9:
        and    ax, 2
        lea    ecx, [esp + 10h]
        cmp    ax, 1
        push   ecx
        sbb    eax, eax
        mov    edi, dword ptr [esp + 1ch]
        add    eax, 2
        push   eax
        push   edi
        call   sub_1_2AA9
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_46DB
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_4622
        mov    eax, 2
        jmp    NEAR PTR loc_1_46DB
loc_1_4622:
        mov    dx, word ptr [ebp]
        db  08Ah, 00Dh, 002h, 000h, 000h, 000h
        test   dl, 4
        jne    SHORT loc_1_4644
        mov    eax, dword ptr [esp + 10h]
        test   byte ptr [eax], 4
        jne    SHORT loc_1_4644
        mov    eax, 2
        jmp    NEAR PTR loc_1_46DB
loc_1_4644:
        mov    si, word ptr [edi + 14h]
        mov    bl, byte ptr [edi + 11h]
        test   dl, 8
        je     SHORT loc_1_4684
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 10h]
        and    cl, 80h
        mov    al, byte ptr [eax + 1fh]
        push   eax
        cmp    cl, 1
        mov    al, byte ptr [edx + 1eh]
        mov    dl, byte ptr [ebp + 2]
        push   eax
        push   edx
        sbb    eax, eax
        inc    eax
        push   eax
        call   sub_1_270C
        add    esp, 10h
        test   al, al
        jne    SHORT loc_1_4681
        mov    eax, 17h
        jmp    SHORT loc_1_46DB
loc_1_4681:
        mov    byte ptr [edi + 11h], al
loc_1_4684:
        mov    cx, word ptr [edi + 14h]
        push   edi
        and    cx, 0fff9h
        mov    word ptr [edi + 14h], cx
        mov    ax, word ptr [ebp]
        and    ax, 6
        or     ax, cx
        mov    word ptr [edi + 14h], ax
        call   sub_1_4053
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_46B6
        mov    word ptr [edi + 14h], si
        mov    byte ptr [edi + 11h], bl
        jmp    SHORT loc_1_46DB
loc_1_46B6:
        push   edi
        call   sub_1_4154
        add    esp, 4
        mov    ebp, eax
        test   ebp, ebp
        je     SHORT loc_1_46D9
        mov    word ptr [edi + 14h], si
        push   edi
        mov    byte ptr [edi + 11h], bl
        call   sub_1_4053
        add    esp, 4
        mov    eax, ebp
        jmp    SHORT loc_1_46DB
loc_1_46D9:
        xor    eax, eax
loc_1_46DB:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_46E3:
        push   ebp
        mov    ax, word ptr [esp + 14h]
        mov    ebp, esp
        sub    esp, 34h
        mov    word ptr [ebp - 2], ax
        push   ebx
        push   esi
        xor    eax, eax
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        lea    edi, [ebp - 30h]
        mov    ecx, 8
        lea    ebx, [esi + 10h]
        rep stosd
        stosw
        stosb
        test   word ptr [ebp - 2], 0fe81h
        je     SHORT loc_1_471C
        mov    eax, 2
        jmp    NEAR PTR loc_1_48F6
loc_1_471C:
        push   1bh
        call   sub_1_2540
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_4736
        mov    eax, 20h
        jmp    NEAR PTR loc_1_48F6
loc_1_4736:
        mov    eax, dword ptr [ebp + 14h]
        and    eax, 40000000h
        mov    dword ptr [ebp - 0ch], eax
        je     SHORT loc_1_478E
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [ebp - 1eh], eax
        mov    ecx, dword ptr [ebx + 0ch]
        mov    dword ptr [ebp - 1ah], ecx
        cmp    eax, ecx
        jbe    SHORT loc_1_475F
        mov    dword ptr [ebp - 4], 3
        jmp    NEAR PTR loc_1_48EA
loc_1_475F:
        mov    eax, dword ptr [ebx]
        mov    ecx, 0fffff000h
        and    eax, ecx
        mov    dword ptr [ebx], eax
        not    eax
        inc    eax
        mov    dword ptr [ebp - 12h], eax
        mov    eax, dword ptr [ebx + 4]
        add    eax, 0fffh
        and    eax, ecx
        mov    dword ptr [ebx + 4], eax
        mov    ecx, dword ptr [ebp - 1eh]
        mov    dword ptr [edi + 8], ecx
loc_1_4783:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_1_47AD
        mov    eax, dword ptr [ebx + 4]
        jmp    SHORT loc_1_47B4
loc_1_478E:
        push   dword ptr [esi + 4]
        call   sub_1_2650
        add    esp, 4
        mov    dword ptr [edi + 8], eax
        cmp    eax, -1
        jne    SHORT loc_1_4783
        mov    dword ptr [ebp - 4], 3
        jmp    NEAR PTR loc_1_48EA
loc_1_47AD:
        mov    eax, dword ptr [esi + 8]
        sub    eax, dword ptr [esi + 4]
        inc    eax
loc_1_47B4:
        push   eax
        push   dword ptr [edi + 8]
        call   sub_1_2678
        add    esp, 8
        mov    dword ptr [edi + 0ch], eax
        cmp    eax, -1
        je     NEAR PTR loc_1_48E3
        test   byte ptr [ebp - 2], 10h
        je     SHORT loc_1_47DD
        test   eax, 3fffh
        jne    NEAR PTR loc_1_48E3
loc_1_47DD:
        test   eax, eax
        jne    SHORT loc_1_47E9
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [edi + 0ch], eax
loc_1_47E9:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [edi + 1ah], 0
        mov    dword ptr [edi + 10h], eax
        mov    dword ptr [edi + 14h], ecx
        mov    cl, byte ptr [ebp + 18h]
        mov    edx, dword ptr [edi + 8]
        mov    dword ptr [ebp - 26h], edx
        mov    dx, word ptr [ebp - 2]
        mov    eax, dword ptr [edi + 0ch]
        mov    word ptr [ebp - 28h], dx
        mov    dword ptr [ebp - 22h], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [ebp - 0eh], cl
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 2ch], edi
        push   ecx
        mov    dword ptr [ebp - 30h], eax
        lea    eax, [ebp - 30h]
        push   eax
        call   sub_1_3B7B
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    NEAR PTR loc_1_48EA
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_1_485D
        mov    eax, dword ptr [ebp - 1eh]
        push   edi
        mov    dword ptr [ebx + 8], eax
        mov    ecx, dword ptr [ebp - 1ah]
        mov    dword ptr [ebx + 0ch], ecx
        mov    edx, dword ptr [ebp - 22h]
        mov    dword ptr [ebx + 4], edx
        call   sub_1_254F
        add    esp, 4
        jmp    NEAR PTR loc_1_48DF
loc_1_485D:
        test   byte ptr [ebp + 17h], 10h
        je     SHORT loc_1_4869
        mov    eax, dword ptr [ebp - 26h]
        mov    dword ptr [edi + 8], eax
loc_1_4869:
        db  081h, 07Dh, 008h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_48BB
        test   byte ptr [ebp + 17h], 80h
        jne    SHORT loc_1_48BB
        mov    eax, dword ptr [esi + 28h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 16h], eax
        push   dword ptr [ebp - 8]
        call   sub_1_4053
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_1_48A5
        push   dword ptr [ebp - 8]
        call   sub_1_4154
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     SHORT loc_1_48BB
loc_1_48A5:
        push   edi
        call   sub_1_4245
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_48EA
        mov    dword ptr [ebp - 4], 19h
        jmp    SHORT loc_1_48EA
loc_1_48BB:
        push   4
        mov    eax, dword ptr [ebp + 0ch]
        push   edi
        add    eax, 24h
        push   eax
        call   sub_1_2D57
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        add    eax, 1ch
        push   0
        push   edi
        push   eax
        call   sub_1_2D57
        add    esp, 0ch
loc_1_48DF:
        xor    eax, eax
        jmp    SHORT loc_1_48F6
loc_1_48E3:
        mov    dword ptr [ebp - 4], 0ah
loc_1_48EA:
        push   edi
        call   sub_1_254F
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
loc_1_48F6:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_48FD:
        mov    eax, dword ptr [esp + 8]
        sub    esp, 0ch
        mov    word ptr [esp + 2], 0
        push   ebx
        mov    ecx, dword ptr [eax + 18h]
        push   esi
        db  08Bh, 01Dh, 00Ah, 000h, 000h, 000h
        mov    dword ptr [esp + 10h], ecx
        push   edi
        push   ebp
        lea    esi, [ebx + 2]
        test   word ptr [esi], 0fe81h
        je     SHORT loc_1_4930
        mov    eax, 2
        jmp    NEAR PTR loc_1_4B47
loc_1_4930:
        push   dword ptr [ebx + 4]
        call   sub_1_2650
        add    esp, 4
        mov    edi, eax
        cmp    edi, -1
        jne    SHORT loc_1_494C
        mov    eax, 3
        jmp    NEAR PTR loc_1_4B47
loc_1_494C:
        push   dword ptr [ebx + 8]
        push   edi
        call   sub_1_2678
        add    esp, 8
        cmp    eax, -1
        je     NEAR PTR loc_1_4B42
        test   byte ptr [esi], 10h
        je     SHORT loc_1_4971
        test   eax, 3fffh
        jne    NEAR PTR loc_1_4B42
loc_1_4971:
        mov    ecx, dword ptr [esp + 18h]
        mov    ecx, dword ptr [ecx + 8]
        test   ecx, 10000000h
        je     SHORT loc_1_498A
        mov    eax, 1eh
        jmp    NEAR PTR loc_1_4B47
loc_1_498A:
        test   ecx, 8000000h
        je     NEAR PTR loc_1_4A7E
        mov    edx, 1
        mov    ecx, dword ptr [esp + 18h]
        mov    ebp, dword ptr [ecx + 24h]
        test   ebp, ebp
        je     SHORT loc_1_49D8
loc_1_49A6:
        cmp    byte ptr [ebp + 1ah], 0
        jne    SHORT loc_1_49D1
        db  081h, 07Dh, 010h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_49D1
        xor    edx, edx
        test   edi, edi
        je     SHORT loc_1_49C0
        cmp    dword ptr [ebp + 8], edi
        jne    SHORT loc_1_49D1
loc_1_49C0:
        test   eax, eax
        je     SHORT loc_1_49D8
        cmp    dword ptr [ebp + 0ch], eax
        je     SHORT loc_1_49D8
        test   edi, edi
        jne    NEAR PTR loc_1_4B42
loc_1_49D1:
        mov    ebp, dword ptr [ebp + 4]
        test   ebp, ebp
        jne    SHORT loc_1_49A6
loc_1_49D8:
        test   edx, edx
        je     SHORT loc_1_49E6
        mov    eax, 20h
        jmp    NEAR PTR loc_1_4B47
loc_1_49E6:
        test   ebp, ebp
        jne    SHORT loc_1_49FA
        cmp    edi, 1
        sbb    eax, eax
        and    eax, 1dh
        add    eax, 3
        jmp    NEAR PTR loc_1_4B47
loc_1_49FA:
        push   ebp
        push   dword ptr [esp + 24h]
        call   sub_1_6288
        add    esp, 8
        push   ebp
        call   sub_1_3B5C
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_4A20
        mov    eax, 20h
        jmp    NEAR PTR loc_1_4B47
loc_1_4A20:
        mov    ax, word ptr [edi + 14h]
        mov    cx, word ptr [esi]
        mov    word ptr [esp + 12h], ax
        mov    word ptr [edi + 14h], cx
loc_1_4A30:
        push   edi
        call   sub_1_4053
        mov    dword ptr [esp + 18h], eax
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_4AF7
        push   edi
        call   sub_1_4154
        mov    dword ptr [esp + 18h], eax
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_4AF7
        mov    ax, word ptr [edi + 0eh]
        db  066h, 0A3h, 002h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 4], ecx
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebx + 8], edx
        mov    ax, word ptr [edi + 14h]
        mov    word ptr [esi], ax
        xor    eax, eax
        jmp    NEAR PTR loc_1_4B47
loc_1_4A7E:
        db  0BAh, 000h, 000h, 000h, 000h
        xor    eax, eax
        mov    edi, edx
        mov    ecx, 9
        rep stosd
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  066h, 0C7h, 005h, 002h, 000h, 000h, 000h, 014h, 000h
        mov    eax, dword ptr [ebx + 4]
        db  0A3h, 004h, 000h, 000h, 000h
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebx + 8]
        dec    eax
        db  0A3h, 008h, 000h, 000h, 000h
        movzx  eax, word ptr [esi]
        mov    cl, byte ptr [ebx + 0ch]
        or     eax, 10000000h
        push   ecx
        push   eax
        push   edx
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 30h]
        call   sub_1_46E3
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_1_4B47
        mov    eax, dword ptr [esp + 20h]
        mov    ebp, dword ptr [eax + 1ch]
        push   ebp
        call   sub_1_3B5C
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    NEAR PTR loc_1_4A30
        mov    eax, 20h
        jmp    SHORT loc_1_4B47
loc_1_4AF7:
        mov    ecx, dword ptr [esp + 18h]
        push   ebp
        test   byte ptr [ecx + 0bh], 8
        je     SHORT loc_1_4B2C
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_6288
        mov    ax, word ptr [esp + 1ah]
        add    esp, 8
        mov    word ptr [edi + 14h], ax
        push   edi
        call   sub_1_4053
        add    esp, 4
        push   edi
        call   sub_1_4154
        add    esp, 4
        jmp    SHORT loc_1_4B3C
loc_1_4B2C:
        call   sub_1_62B7
        mov    dword ptr [esp + 18h], eax
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_4B47
loc_1_4B3C:
        mov    eax, dword ptr [esp + 14h]
        jmp    SHORT loc_1_4B47
loc_1_4B42:
        mov    eax, 0ah
loc_1_4B47:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_1_4B4F:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 1ah]
        mov    ecx, dword ptr [esi + 14h]
        test   byte ptr [ecx + 0bh], 8
        je     SHORT loc_1_4B83
        and    word ptr [eax + 14h], 0fffbh
        push   eax
        call   sub_1_4053
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_4B92
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_6288
        add    esp, 8
        jmp    SHORT loc_1_4B90
loc_1_4B83:
        push   esi
        call   sub_1_62B7
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_4B92
loc_1_4B90:
        xor    eax, eax
loc_1_4B92:
        pop    esi
        ret
loc_1_4B94:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 03Dh, 00Ah, 000h, 000h, 000h
        push   dword ptr [edi]
        call   sub_1_2664
        add    esp, 4
        mov    ecx, eax
        cmp    ecx, -1
        je     SHORT loc_1_4C0A
        mov    esi, dword ptr [esp + 14h]
        mov    bx, word ptr [esi + 14h]
        test   bh, 1
        je     SHORT loc_1_4BC5
        sub    edx, edx
        div    dword ptr [esi + 22h]
        test   edx, edx
        jne    SHORT loc_1_4C0A
loc_1_4BC5:
        mov    al, byte ptr [edi + 4]
        test   al, al
        je     SHORT loc_1_4BE2
        test   bl, 10h
        je     SHORT loc_1_4BDB
        mov    edx, dword ptr [esi + 22h]
        shr    edx, 0eh
        cmp    dl, al
        ja     SHORT loc_1_4BE2
loc_1_4BDB:
        mov    eax, 8
        jmp    SHORT loc_1_4C0F
loc_1_4BE2:
        mov    bl, byte ptr [esi + 12h]
        lea    ebp, [esi + 16h]
        push   esi
        mov    byte ptr [esi + 12h], al
        mov    edi, dword ptr [ebp]
        mov    dword ptr [ebp], ecx
        call   sub_1_4154
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_4C06
        mov    byte ptr [esi + 12h], bl
        mov    dword ptr [ebp], edi
        jmp    SHORT loc_1_4C0F
loc_1_4C06:
        xor    eax, eax
        jmp    SHORT loc_1_4C0F
loc_1_4C0A:
        mov    eax, 7
loc_1_4C0F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_4C14:
        push   dword ptr [esp + 4]
        call   sub_1_4F83
        add    esp, 4
        xor    edx, edx
        test   eax, eax
        je     SHORT loc_1_4C40
        push   dword ptr [eax + 4]
        movzx  ecx, word ptr [esp + 0ah]
        add    eax, 0ch
        push   eax
        push   edx
        push   ecx
        push   edx
        push   edx
        push   15h
        call   sub_1_24A2
        add    esp, 1ch
loc_1_4C40:
        ret
loc_1_4C41:
        push   ebp
        db  08Bh, 00Dh, 00Ah, 000h, 000h, 000h
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ebp + 8]
        movzx  ecx, word ptr [ecx]
        mov    ax, word ptr [eax + 16h]
        mov    word ptr [ebp - 4], ax
        mov    word ptr [ebp - 2], dx
        push   dword ptr [ebp - 4]
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2566
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_4C82
        mov    eax, 20h
        jmp    SHORT loc_1_4C97
loc_1_4C82:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        db  066h, 0A3h, 002h, 000h, 000h, 000h
        xor    eax, eax
        db  066h, 0FFh, 005h, 000h, 000h, 000h, 000h
loc_1_4C97:
        mov    esp, ebp
        pop    ebp
        ret
sub_1_4C9B:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 037h
        push   esi
        push   edi
        jae    NEAR PTR loc_1_4EB0
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        imul   eax, eax, 0ch
        db  08Dh, 0B0h, 000h, 000h, 000h, 000h
        cmp    dword ptr [esi + 4], 0
        je     NEAR PTR loc_1_4EB0
        test   byte ptr [esi], 20h
        mov    ax, word ptr [esi + 8]
        je     SHORT loc_1_4CE5
        db  066h, 039h, 005h, 008h, 000h, 000h, 000h
        mov    eax, 1
        adc    eax, -1
        jmp    SHORT loc_1_4CF3
loc_1_4CE5:
        db  066h, 02Bh, 005h, 008h, 000h, 000h, 000h
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
loc_1_4CF3:
        test   eax, eax
        je     SHORT loc_1_4D01
        mov    eax, 1bh
        jmp    NEAR PTR loc_1_4EB5
loc_1_4D01:
        xor    edi, edi
        mov    dword ptr [ebp - 8], edi
        mov    eax, dword ptr [esi]
        and    eax, 7
        je     SHORT loc_1_4D3B
        cmp    eax, 1
        je     NEAR PTR loc_1_4DA1
        cmp    eax, 2
        je     NEAR PTR loc_1_4DC8
        cmp    eax, 3
        je     NEAR PTR loc_1_4E0D
        cmp    eax, 4
        je     NEAR PTR loc_1_4E3B
        mov    eax, 19h
        jmp    NEAR PTR loc_1_4EB5
loc_1_4D3B:
        mov    dword ptr [ebp - 4], 0
loc_1_4D42:
        mov    eax, dword ptr [esi]
        test   al, 40h
        je     SHORT loc_1_4D5C
        db  08Bh, 00Dh, 00Ah, 000h, 000h, 000h
        movzx  edx, byte ptr [esi + 0ah]
        test   byte ptr [edx + ecx], 1
        je     NEAR PTR loc_1_4EA2
loc_1_4D5C:
        test   al, 8
        je     SHORT loc_1_4D84
        lea    eax, [ebp - 8]
        db  08Bh, 00Dh, 00Ah, 000h, 000h, 000h
        push   eax
        movzx  edx, byte ptr [esi + 0bh]
        movzx  ax, byte ptr [edx + ecx]
        push   eax
        call   sub_1_9645
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_4EB5
loc_1_4D84:
        test   byte ptr [esi], 10h
        je     NEAR PTR loc_1_4E7B
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_4E5C
        mov    eax, 19h
        jmp    NEAR PTR loc_1_4EB5
loc_1_4DA1:
        db  066h, 0A1h, 002h, 000h, 000h, 000h
        push   eax
        call   sub_1_4F83
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_4DC0
        mov    eax, 21h
        jmp    NEAR PTR loc_1_4EB5
loc_1_4DC0:
        mov    dword ptr [ebp - 4], edi
        jmp    NEAR PTR loc_1_4D42
loc_1_4DC8:
        db  066h, 08Bh, 00Dh, 002h, 000h, 000h, 000h
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_3AE5
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_4EB5
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax + 0ch], 2
        jne    SHORT loc_1_4DF7
        mov    eax, 21h
        jmp    NEAR PTR loc_1_4EB5
loc_1_4DF7:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 1ah]
        mov    edx, dword ptr [ecx + 14h]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ebp - 8], eax
        mov    edi, dword ptr [ecx + 10h]
        jmp    NEAR PTR loc_1_4D42
loc_1_4E0D:
        db  066h, 0A1h, 002h, 000h, 000h, 000h
        push   eax
        call   sub_1_0499
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_1_4E2D
        mov    eax, 21h
        jmp    NEAR PTR loc_1_4EB5
loc_1_4E2D:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_1_4D42
loc_1_4E3B:
        db  066h, 0A1h, 002h, 000h, 000h, 000h
        push   eax
        call   sub_1_086E
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    NEAR PTR loc_1_4D42
        mov    eax, 21h
        jmp    SHORT loc_1_4EB5
loc_1_4E5C:
        push   dword ptr [ebp - 8]
        call   sub_1_27C1
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_4EB5
        db  0F6h, 005h, 003h, 000h, 000h, 000h, 080h
        jne    SHORT loc_1_4E7B
        mov    eax, 14h
        jmp    SHORT loc_1_4EB5
loc_1_4E7B:
        test   byte ptr [esi], 80h
        je     SHORT loc_1_4EA2
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_4E8D
        mov    eax, 19h
        jmp    SHORT loc_1_4EB5
loc_1_4E8D:
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax + 17h], 40h
        je     SHORT loc_1_4EA2
        cmp    dword ptr [eax + 1ch], edi
        je     SHORT loc_1_4EA2
        mov    eax, 14h
        jmp    SHORT loc_1_4EB5
loc_1_4EA2:
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        call   dword ptr [esi + 4]
        add    esp, 8
        jmp    SHORT loc_1_4EB5
loc_1_4EB0:
        mov    eax, 15h
loc_1_4EB5:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_1_4EBB:
        push   esi
        db  08Bh, 015h, 00Ah, 000h, 000h, 000h
        db  00Fh, 0B7h, 005h, 008h, 000h, 000h, 000h
        push   edi
        sub    eax, 4
        cmp    eax, 9
        ja     SHORT loc_1_4EE0
        db  00Fh, 0B6h, 088h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_1_4EE0:
        db  066h, 083h, 03Dh, 008h, 000h, 000h, 000h, 04Bh
        mov    ax, 3dh
        jae    SHORT loc_1_4EF8
        db  066h, 0A1h, 008h, 000h, 000h, 000h
        sub    ax, 0eh
loc_1_4EF8:
        db  0BEh, 000h, 000h, 000h, 000h
        db  08Bh, 03Dh, 00Ah, 000h, 000h, 000h
        movzx  eax, ax
        add    edi, 0eh
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    word ptr [edx + 0ch], 3dh
loc_1_4F1D:
        mov    word ptr [edx + 0ah], 0eh
loc_1_4F23:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    word ptr [edx + 8], ax
loc_1_4F2D:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    word ptr [edx + 6], ax
loc_1_4F37:
        xor    ecx, ecx
        mov    word ptr [edx + 4], cx
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, ecx
        je     SHORT loc_1_4F51
loc_1_4F46:
        inc    word ptr [edx + 4]
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jne    SHORT loc_1_4F46
loc_1_4F51:
        mov    word ptr [edx], 4bh
        xor    eax, eax
        mov    word ptr [edx + 2], 5343h
        pop    edi
        pop    esi
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 001h, 002h, 002h, 003h, 003h
        db  004h, 004h
sub_1_4F83:
        mov    cx, word ptr [esp + 4]
        push   esi
        db  066h, 039h, 00Dh, 016h, 000h, 000h, 000h
        jne    SHORT loc_1_4F99
        db  0B8h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_4FF4
loc_1_4F99:
        xor    dx, dx
        xor    esi, esi
loc_1_4F9E:
        movzx  eax, dx
        test   eax, eax
        je     SHORT loc_1_4FB8
        cmp    eax, 1
        je     SHORT loc_1_4FBF
        cmp    eax, 2
        je     SHORT loc_1_4FC6
        xor    eax, eax
        mov    esi, 1
        jmp    SHORT loc_1_4FCB
loc_1_4FB8:
        db  0A1h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_4FCB
loc_1_4FBF:
        db  0A1h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_4FCB
loc_1_4FC6:
        db  0A1h, 000h, 000h, 000h, 000h
loc_1_4FCB:
        test   eax, eax
        je     SHORT loc_1_4FEE
loc_1_4FCF:
        test   esi, esi
        jne    SHORT loc_1_4FEE
        cmp    word ptr [eax + 16h], cx
        jne    SHORT loc_1_4FE8
        cmp    byte ptr [eax + 20h], 2
        jne    SHORT loc_1_4FE1
        xor    eax, eax
loc_1_4FE1:
        mov    esi, 1
        jmp    SHORT loc_1_4FEA
loc_1_4FE8:
        mov    eax, dword ptr [eax]
loc_1_4FEA:
        test   eax, eax
        jne    SHORT loc_1_4FCF
loc_1_4FEE:
        inc    dx
        test   esi, esi
        je     SHORT loc_1_4F9E
loc_1_4FF4:
        pop    esi
        ret
sub_1_4FF6:
        xor    dx, dx
        mov    eax, dword ptr [esp + 4]
        test   eax, eax
        je     SHORT loc_1_5031
        xor    ecx, ecx
        mov    cx, word ptr [eax + 8]
        and    ecx, 7
        cmp    ecx, 1
        je     SHORT loc_1_501B
        cmp    ecx, 2
        je     SHORT loc_1_5021
        cmp    ecx, 4
        je     SHORT loc_1_5027
        jmp    SHORT loc_1_502B
loc_1_501B:
        mov    dx, 3
        jmp    SHORT loc_1_502B
loc_1_5021:
        mov    dx, 2
        jmp    SHORT loc_1_502B
loc_1_5027:
        mov    dx, 1
loc_1_502B:
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    SHORT loc_1_5044
loc_1_5031:
        movzx  eax, dx
        test   eax, eax
        je     SHORT loc_1_5045
        cmp    eax, 1
        je     SHORT loc_1_504C
        cmp    eax, 2
        je     SHORT loc_1_5053
        xor    eax, eax
loc_1_5044:
        ret
loc_1_5045:
        db  0A1h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_5058
loc_1_504C:
        db  0A1h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_5058
loc_1_5053:
        db  0A1h, 000h, 000h, 000h, 000h
loc_1_5058:
        test   eax, eax
        jne    SHORT loc_1_5044
        inc    dx
        jmp    SHORT loc_1_5031
sub_1_5060:
        push   esi
        push   edi
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        test   edi, edi
        je     SHORT loc_1_50B8
        mov    esi, dword ptr [esp + 0ch]
loc_1_5070:
        mov    ecx, edi
        call   sub_1_2967
        test   al, 80h
        je     SHORT loc_1_50B1
        mov    eax, dword ptr [edi + 14h]
        and    eax, 40000000h
        je     SHORT loc_1_508B
        test   byte ptr [esi + 8], 10h
        jne    SHORT loc_1_5095
loc_1_508B:
        test   eax, eax
        jne    SHORT loc_1_50B1
        test   byte ptr [esi + 8], 8
        je     SHORT loc_1_50B1
loc_1_5095:
        push   dword ptr [esi + 4]
        mov    dx, word ptr [edi + 10h]
        lea    ecx, [esi + 0ch]
        push   ecx
        push   0
        push   0
        push   0
        push   edx
        push   40h
        call   sub_1_24A2
        add    esp, 1ch
loc_1_50B1:
        mov    edi, dword ptr [edi + 4]
        test   edi, edi
        jne    SHORT loc_1_5070
loc_1_50B8:
        pop    edi
        pop    esi
        ret
loc_1_50BB:
        push   esi
        mov    esi, dword ptr [esp + 8]
        movzx  eax, byte ptr [esi + 20h]
        cmp    eax, 1
        je     SHORT loc_1_50D3
        cmp    eax, 2
        je     SHORT loc_1_5124
        jmp    NEAR PTR loc_1_5168
loc_1_50D3:
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_1_50EA
loc_1_50DC:
        cmp    word ptr [eax + 32h], 0
        jne    SHORT loc_1_5112
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jne    SHORT loc_1_50DC
loc_1_50EA:
        push   esi
        call   sub_1_5060
        add    esp, 4
        xor    ecx, ecx
        push   dword ptr [esi + 4]
        mov    byte ptr [esi + 20h], cl
        add    esi, 0ch
        push   esi
        push   ecx
        push   ecx
        push   ecx
        push   ecx
        push   82h
        call   sub_1_24A2
        add    esp, 1ch
        jmp    SHORT loc_1_5168
loc_1_5112:
        push   esi
        push   32h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2566
        add    esp, 0ch
        jmp    SHORT loc_1_5168
loc_1_5124:
        xor    eax, eax
        mov    ax, word ptr [esi + 8]
        and    eax, 7
        cmp    eax, 1
        je     SHORT loc_1_5140
        cmp    eax, 2
        je     SHORT loc_1_5147
        cmp    eax, 4
        je     SHORT loc_1_514E
        xor    eax, eax
        jmp    SHORT loc_1_5153
loc_1_5140:
        db  0B8h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_5153
loc_1_5147:
        db  0B8h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_5153
loc_1_514E:
        db  0B8h, 000h, 000h, 000h, 000h
loc_1_5153:
        push   0
        push   esi
        push   eax
        call   sub_1_2E0B
        add    esp, 0ch
        push   esi
        call   sub_1_254F
        add    esp, 4
loc_1_5168:
        pop    esi
        ret
loc_1_516A:
        sub    esp, 4
        db  0A1h, 00Ah, 000h, 000h, 000h
        mov    dword ptr [esp], eax
        push   ebx
        push   esi
        xor    ebx, ebx
        test   word ptr [eax], 0ffe0h
        push   edi
        push   ebp
        je     SHORT loc_1_518D
        mov    eax, 2
        jmp    NEAR PTR loc_1_52D8
loc_1_518D:
        db  0A1h, 00Ah, 000h, 000h, 000h
        test   byte ptr [eax + 3], 0f8h
        jne    NEAR PTR loc_1_52D3
        db  0A1h, 00Ah, 000h, 000h, 000h
        mov    ax, word ptr [eax + 0ch]
        db  066h, 039h, 005h, 000h, 000h, 000h, 000h
        jb     NEAR PTR loc_1_52D3
        cmp    ax, 201h
        jb     NEAR PTR loc_1_52D3
        push   21h
        call   sub_1_2540
        add    esp, 4
        mov    ebp, eax
        test   ebp, ebp
        jne    SHORT loc_1_51D6
        mov    eax, 20h
        jmp    NEAR PTR loc_1_52D8
loc_1_51D6:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        lea    edi, [ebp + 8]
        mov    word ptr [ebp + 16h], ax
        mov    esi, dword ptr [esp + 10h]
        db  066h, 0FFh, 005h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp + 20h], 1
        db  08Bh, 00Dh, 004h, 000h, 000h, 000h
        mov    dword ptr [ebp + 4], ecx
        movsd
        movsd
        movsd
        movsw
        xor    esi, esi
        mov    dword ptr [ebp + 18h], esi
        mov    dword ptr [ebp + 1ch], esi
        db  00Fh, 0B6h, 00Dh, 000h, 000h, 000h, 000h
        not    ecx
        mov    al, byte ptr [ebp + 0ah]
        movzx  edx, al
        test   edx, ecx
        je     SHORT loc_1_522C
        db  008h, 005h, 000h, 000h, 000h, 000h
        call   sub_1_9619
        cmp    eax, esi
        mov    edi, eax
        jne    SHORT loc_1_5273
loc_1_522C:
        test   byte ptr [ebp + 8], 18h
        je     SHORT loc_1_524F
        push   ebp
        push   32h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2566
        add    esp, 0ch
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_5253
        mov    edi, 19h
        jmp    SHORT loc_1_5273
loc_1_524F:
        mov    byte ptr [ebp + 20h], 0
loc_1_5253:
        xor    ecx, ecx
        mov    eax, dword ptr [esp + 10h]
        mov    cx, word ptr [eax]
        and    ecx, 7
        cmp    ecx, 1
        je     SHORT loc_1_5297
        cmp    ecx, 2
        je     SHORT loc_1_52A6
        cmp    ecx, 4
        je     SHORT loc_1_52B5
        mov    edi, 2
loc_1_5273:
        call   sub_1_2A6F
        call   sub_1_9619
        test   ebx, ebx
        je     SHORT loc_1_528A
        push   ebx
        call   sub_1_25AF
        add    esp, 4
loc_1_528A:
        push   ebp
        call   sub_1_254F
        add    esp, 4
        mov    eax, edi
        jmp    SHORT loc_1_52D8
loc_1_5297:
        push   0
        push   ebp
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2D7C
        jmp    SHORT loc_1_52C2
loc_1_52A6:
        push   0
        push   ebp
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2D7C
        jmp    SHORT loc_1_52C2
loc_1_52B5:
        push   0
        push   ebp
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2D57
loc_1_52C2:
        mov    ax, word ptr [ebp + 16h]
        add    esp, 0ch
        db  066h, 0A3h, 002h, 000h, 000h, 000h
        xor    eax, eax
        jmp    SHORT loc_1_52D8
loc_1_52D3:
        mov    eax, 1ch
loc_1_52D8:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_52E0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    byte ptr [esi + 20h], 1
        jne    SHORT loc_1_52F5
        mov    eax, 21h
        jmp    NEAR PTR loc_1_537A
loc_1_52F5:
        xor    ecx, ecx
        cmp    dword ptr [esi + 1ch], ecx
        jne    SHORT loc_1_5375
        cmp    dword ptr [esi + 18h], ecx
        jne    SHORT loc_1_5375
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, ecx
        je     SHORT loc_1_531C
loc_1_530A:
        test   byte ptr [eax + 17h], 40h
        je     SHORT loc_1_5315
        cmp    dword ptr [eax + 1ch], esi
        je     SHORT loc_1_5375
loc_1_5315:
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jne    SHORT loc_1_530A
loc_1_531C:
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_1_5330
loc_1_5325:
        cmp    dword ptr [eax + 10h], esi
        je     SHORT loc_1_5375
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    SHORT loc_1_5325
loc_1_5330:
        test   byte ptr [esi + 8], 2
        je     SHORT loc_1_533F
        push   esi
        call   sub_1_091B
        add    esp, 4
loc_1_533F:
        mov    byte ptr [esi + 20h], 2
        push   esi
        push   32h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2566
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_535E
        mov    eax, 19h
        jmp    SHORT loc_1_537A
loc_1_535E:
        call   sub_1_2A6F
        call   sub_1_9619
        db  066h, 0C7h, 005h, 002h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        jmp    SHORT loc_1_537A
loc_1_5375:
        mov    eax, 1eh
loc_1_537A:
        pop    esi
        ret
loc_1_537C:
        push   ebx
        push   esi
        push   edi
        db  08Bh, 035h, 00Ah, 000h, 000h, 000h
        push   ebp
        add    esi, 2
        test   word ptr [esi], 0fffeh
        je     SHORT loc_1_5397
        mov    eax, 2
        jmp    SHORT loc_1_53DF
loc_1_5397:
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 14h]
        mov    ebp, 1
loc_1_53A2:
        push   edi
        call   sub_1_4FF6
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_1_53E4
        cmp    byte ptr [edi + 20h], 2
        je     SHORT loc_1_53CF
        test   word ptr [esi], bp
        je     SHORT loc_1_53CD
        push   dword ptr [esp + 18h]
        push   edi
        call   sub_1_2635
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_53CF
loc_1_53CD:
        mov    ebx, ebp
loc_1_53CF:
        test   ebx, ebx
        je     SHORT loc_1_53A2
        mov    ax, word ptr [edi + 16h]
        db  066h, 0A3h, 002h, 000h, 000h, 000h
        xor    eax, eax
loc_1_53DF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_53E4:
        mov    eax, 1fh
        jmp    SHORT loc_1_53DF
loc_1_53EB:
        push   ebx
        push   esi
        push   edi
        db  08Bh, 035h, 00Ah, 000h, 000h, 000h
        mov    edi, dword ptr [esp + 10h]
        xor    ebx, ebx
        push   dword ptr [edi + 4]
        lea    ecx, [edi + 0ch]
        push   ecx
        push   ebx
        push   esi
        push   ebx
        push   ebx
        push   14h
        call   sub_1_24A2
        add    esp, 1ch
        cmp    byte ptr [esi + 5], bl
        jne    SHORT loc_1_5430
        mov    ax, word ptr [edi + 8]
        mov    word ptr [esi + 4], ax
        db  066h, 083h, 03Dh, 008h, 000h, 000h, 000h, 00Ah
        jb     SHORT loc_1_5430
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    word ptr [esi + 8], ax
loc_1_5430:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_5436:
        push   ebx
        db  0A1h, 00Ah, 000h, 000h, 000h
        cmp    word ptr [eax + 2], 0
        push   esi
        push   edi
        push   ebp
        je     SHORT loc_1_5450
        mov    eax, 2
        jmp    NEAR PTR loc_1_54E5
loc_1_5450:
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 02Dh
        jne    SHORT loc_1_5474
        mov    edi, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 14h]
        cmp    dword ptr [edi + 1ch], esi
        je     SHORT loc_1_546E
        mov    eax, 21h
        jmp    SHORT loc_1_54E5
loc_1_546E:
        mov    bx, 0feh
        jmp    SHORT loc_1_5499
loc_1_5474:
        mov    bx, 0eh
        mov    edi, dword ptr [esp + 18h]
        mov    eax, dword ptr [edi + 14h]
        test   eax, 60000000h
        je     SHORT loc_1_548D
        mov    eax, 1eh
        jmp    SHORT loc_1_54E5
loc_1_548D:
        or     eax, 20000000h
        mov    esi, dword ptr [esp + 14h]
        mov    dword ptr [edi + 14h], eax
loc_1_5499:
        push   0ah
        call   sub_1_2540
        add    esp, 4
        mov    ebp, eax
        test   ebp, ebp
        jne    SHORT loc_1_54B4
        and    byte ptr [edi + 17h], 0dfh
        mov    eax, 20h
        jmp    SHORT loc_1_54E5
loc_1_54B4:
        mov    word ptr [ebp + 8], bx
        push   ebp
        push   32h
        mov    dword ptr [ebp], edi
        db  068h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp + 4], esi
        call   sub_1_2566
        add    esp, 0ch
        test   eax, eax
        mov    eax, 0
        jne    SHORT loc_1_54E5
        push   ebp
        call   sub_1_254F
        add    esp, 4
        mov    eax, 19h
loc_1_54E5:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_54EA:
        db  0A1h, 00Ah, 000h, 000h, 000h
        cmp    word ptr [eax], 3
        mov    eax, 16h
        jne    SHORT loc_1_5506
        db  0C7h, 005h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
loc_1_5506:
        ret
sub_1_5507:
        push   ebp
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_269E
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_552B
        db  0A2h, 001h, 000h, 000h, 000h
        jmp    SHORT loc_1_5559
loc_1_552B:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 18h]
        db  089h, 00Dh, 014h, 000h, 000h, 000h
        mov    edx, dword ptr [eax + 1ch]
        db  089h, 015h, 00Ah, 000h, 000h, 000h
        mov    al, byte ptr [eax + 0ch]
        db  0A2h, 000h, 000h, 000h, 000h
        call   sub_1_2438
        mov    ecx, dword ptr [ebp - 4]
        mov    dl, byte ptr [ecx + 0fh]
        db  088h, 015h, 000h, 000h, 000h, 000h
loc_1_5559:
        mov    esp, ebp
        pop    ebp
        ret
sub_1_555D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        lea    esi, [ecx + 0ch]
        mov    edi, ecx
        and    byte ptr [ecx + 14h], 0dfh
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 090h
        mov    eax, dword ptr [esi]
        mov    al, byte ptr [eax + 0ch]
        db  0A2h, 000h, 000h, 000h, 000h
        mov    cl, byte ptr [edi + 12h]
        db  088h, 00Dh, 002h, 000h, 000h, 000h
        mov    edx, dword ptr [esi]
        mov    eax, dword ptr [edx + 18h]
        db  0A3h, 014h, 000h, 000h, 000h
        mov    edx, dword ptr [esi]
        mov    ecx, dword ptr [edx + 1ch]
        db  089h, 00Dh, 00Ah, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        jne    SHORT loc_1_560E
        test   byte ptr [edi + 17h], 10h
        je     SHORT loc_1_560C
        push   0
        lea    ecx, [ebp - 2]
        push   2
        mov    eax, dword ptr [esi]
        push   ecx
        push   3eh
        push   0
        push   dword ptr [eax + 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_55C2:
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_1_55D0
        mov    eax, 19h
        jmp    SHORT loc_1_560E
loc_1_55D0:
        push   0
        lea    eax, [ebp - 2]
        push   2
        mov    ecx, dword ptr [esi]
        or     byte ptr [ebp - 2], 40h
        push   eax
        push   3eh
        push   1
        push   dword ptr [ecx + 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_55EA:
        and    word ptr [ebp - 2], 0ffbfh
        add    esp, 18h
        lea    ecx, [ebp - 2]
        mov    edx, dword ptr [esi]
        push   0
        push   2
        push   ecx
        push   3eh
        push   1
        push   dword ptr [edx + 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5609:
        add    esp, 18h
loc_1_560C:
        xor    eax, eax
loc_1_560E:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_5614:
        push   esi
        mov    eax, 0
        test   byte ptr [ecx + 14h], 20h
        mov    esi, ecx
        je     SHORT loc_1_5632
        mov    ecx, esi
        call   sub_1_555D
        push   esi
        call   sub_1_2CC4
        add    esp, 4
loc_1_5632:
        pop    esi
        ret
sub_1_5634:
        push   esi
        mov    esi, ecx
        lea    ecx, [ecx + 0ch]
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 18h]
        db  089h, 015h, 014h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 1ch]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Dh
        db  089h, 015h, 00Ah, 000h, 000h, 000h
        mov    eax, dword ptr [ecx]
        mov    al, byte ptr [eax + 0ch]
        db  0A2h, 000h, 000h, 000h, 000h
        mov    cl, byte ptr [esi + 12h]
        db  088h, 00Dh, 002h, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        jne    SHORT loc_1_569F
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Eh
        push   esi
        call   sub_1_954E
        add    esp, 4
        db  0A2h, 003h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 008h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 00Eh, 000h, 000h, 000h, 000h, 001h
        call   sub_1_2438
loc_1_569F:
        pop    esi
        ret
sub_1_56A1:
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [ecx + 18h]
        mov    esi, ecx
        xor    cl, cl
        cmp    byte ptr [esi + 30h], cl
        jbe    SHORT loc_1_56C6
        xor    edx, edx
loc_1_56B3:
        mov    byte ptr [eax + 14h], dl
        inc    cl
        mov    byte ptr [eax + 15h], dl
        mov    byte ptr [eax + 16h], dl
        add    eax, 28h
        cmp    byte ptr [esi + 30h], cl
        ja     SHORT loc_1_56B3
loc_1_56C6:
        lea    eax, [esp + 6]
        push   eax
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   dword ptr [esi + 18h]
        call   sub_1_6B27
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_1_56E7
        mov    eax, 19h
        jmp    SHORT loc_1_574A
loc_1_56E7:
        lea    ecx, [esi + 0ch]
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 18h]
        db  089h, 015h, 014h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 1ch]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Dh
        db  089h, 015h, 00Ah, 000h, 000h, 000h
        mov    eax, dword ptr [ecx]
        mov    al, byte ptr [eax + 0ch]
        db  0A2h, 000h, 000h, 000h, 000h
        mov    cl, byte ptr [esi + 12h]
        db  088h, 00Dh, 002h, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        jne    SHORT loc_1_574A
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Eh
        db  0C6h, 005h, 003h, 000h, 000h, 000h, 080h
        db  066h, 0C7h, 005h, 008h, 000h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [esp + 6]
        db  066h, 0A3h, 004h, 000h, 000h, 000h
        call   sub_1_2438
loc_1_574A:
        pop    esi
        add    esp, 4
        ret
sub_1_574F:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   ebp
        xor    edi, edi
        mov    bl, byte ptr [esp + 18h]
        cmp    byte ptr [esi + 30h], bl
        je     SHORT loc_1_5796
        movzx  eax, bl
        imul   eax, eax, 28h
        push   eax
        call   sub_1_2540
        add    esp, 4
        mov    ebp, eax
        cmp    ebp, edi
        je     SHORT loc_1_5787
        push   dword ptr [esi + 18h]
        call   sub_1_254F
        add    esp, 4
        mov    dword ptr [esi + 18h], ebp
        jmp    SHORT loc_1_5793
loc_1_5787:
        cmp    byte ptr [esi + 30h], bl
        jae    SHORT loc_1_5793
        mov    bl, 1
        mov    edi, 19h
loc_1_5793:
        mov    byte ptr [esi + 30h], bl
loc_1_5796:
        xor    cl, cl
        mov    eax, dword ptr [esi + 18h]
        cmp    byte ptr [esi + 30h], cl
        jbe    SHORT loc_1_57CE
loc_1_57A0:
        mov    dword ptr [eax], esi
        mov    byte ptr [eax + 1fh], cl
        add    eax, 28h
        inc    cl
        mov    dword ptr [eax - 1ch], 0ffffffffh
        mov    byte ptr [eax - 11h], 0ffh
        mov    byte ptr [eax - 10h], 0ffh
        mov    byte ptr [eax - 0fh], 0ffh
        mov    word ptr [eax - 0eh], 0ffffh
        mov    word ptr [eax - 0ch], 0ffffh
        cmp    byte ptr [esi + 30h], cl
        ja     SHORT loc_1_57A0
loc_1_57CE:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_57D5:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   34h
        call   sub_1_2540
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_57F6
        mov    eax, 19h
        jmp    NEAR PTR loc_1_5970
loc_1_57F6:
        xor    ebx, ebx
        mov    esi, dword ptr [esp + 1ch]
        mov    ax, word ptr [esi + 10h]
        push   edi
        mov    word ptr [edi], ax
        push   esi
        mov    word ptr [edi + 2], bx
        mov    dword ptr [esp + 1ch], ebx
        mov    byte ptr [edi + 5], bl
        mov    byte ptr [edi + 4], 6
        call   sub_1_384D
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_583A
        push   edi
        mov    byte ptr [edi + 5], bl
        mov    word ptr [edi + 10h], 20h
        push   esi
        call   sub_1_3967
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_583A
        mov    bl, byte ptr [edi + 14h]
loc_1_583A:
        cmp    bl, 1
        jae    SHORT loc_1_5843
        mov    bl, 1
        jmp    SHORT loc_1_584A
loc_1_5843:
        cmp    bl, 8
        jbe    SHORT loc_1_584A
        mov    bl, 8
loc_1_584A:
        push   ebx
        push   esi
        call   sub_1_574F
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_5865
        mov    dword ptr [esp + 14h], 19h
        jmp    NEAR PTR loc_1_5963
loc_1_5865:
        mov    byte ptr [esp + 11h], 0
        cmp    byte ptr [esi + 30h], 0
        mov    ebx, dword ptr [esi + 18h]
        jbe    NEAR PTR loc_1_5963
loc_1_5877:
        mov    word ptr [esp + 12h], 0
loc_1_587E:
        mov    ax, word ptr [esi + 10h]
        mov    cl, byte ptr [esp + 11h]
        movzx  ebp, word ptr [esp + 12h]
        mov    word ptr [edi], ax
        push   edi
        push   esi
        mov    byte ptr [edi + 1], cl
        mov    word ptr [edi + 2], 0
        mov    byte ptr [edi + 5], 0
        db  08Ah, 085h, 000h, 000h, 000h, 000h
        mov    byte ptr [edi + 4], al
        call   sub_1_384D
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_593E
        mov    byte ptr [edi + 5], 0
        mov    word ptr [edi + 10h], 20h
        push   edi
        push   esi
        call   sub_1_3967
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_593E
        test   ebp, ebp
        je     SHORT loc_1_58E3
        cmp    ebp, 1
        je     SHORT loc_1_58EE
        cmp    ebp, 2
        je     SHORT loc_1_58FC
        cmp    ebp, 3
        je     SHORT loc_1_590E
        jmp    SHORT loc_1_593E
loc_1_58E3:
        mov    al, byte ptr [edi + 14h]
        shr    al, 4
        mov    byte ptr [ebx + 17h], al
        jmp    SHORT loc_1_593E
loc_1_58EE:
        mov    al, byte ptr [edi + 14h]
        mov    byte ptr [ebx + 18h], al
        mov    cl, byte ptr [edi + 15h]
        mov    byte ptr [ebx + 19h], cl
        jmp    SHORT loc_1_593E
loc_1_58FC:
        mov    ax, word ptr [edi + 14h]
        mov    word ptr [ebx + 1ah], ax
        mov    cx, word ptr [edi + 16h]
        mov    word ptr [ebx + 1ch], cx
        jmp    SHORT loc_1_593E
loc_1_590E:
        mov    dword ptr [ebx + 0ch], 0
        xor    ax, ax
        mov    al, byte ptr [edi + 14h]
        and    ax, 3
        inc    ax
        je     SHORT loc_1_593E
loc_1_5923:
        dec    ax
        movzx  ecx, ax
        movzx  edx, byte ptr [ecx + edi + 16h]
        lea    ecx, [eax*8]
        shl    edx, cl
        or     dword ptr [ebx + 0ch], edx
        test   ax, ax
        jne    SHORT loc_1_5923
loc_1_593E:
        inc    word ptr [esp + 12h]
        cmp    word ptr [esp + 12h], 4
        jb     NEAR PTR loc_1_587E
        add    ebx, 28h
        inc    byte ptr [esp + 11h]
        mov    al, byte ptr [esp + 11h]
        cmp    byte ptr [esi + 30h], al
        ja     NEAR PTR loc_1_5877
loc_1_5963:
        push   edi
        call   sub_1_254F
        mov    eax, dword ptr [esp + 18h]
        add    esp, 4
loc_1_5970:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_5978:
        push   esi
        push   34h
        call   sub_1_2540
        add    esp, 4
        mov    esi, eax
        mov    eax, 19h
        test   esi, esi
        je     SHORT loc_1_59B3
        push   esi
        push   dword ptr [esp + 0ch]
        call   sub_1_124A
        add    esp, 8
        push   esi
        push   dword ptr [esp + 0ch]
        call   sub_1_13F4
        add    esp, 8
        push   esi
        call   sub_1_254F
        add    esp, 4
        xor    eax, eax
loc_1_59B3:
        pop    esi
        ret
sub_1_59B5:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    si, word ptr [eax + 0ah]
        mov    ecx, dword ptr [ebp + 0ch]
        test   ecx, ecx
        je     SHORT loc_1_59F0
        test   byte ptr [ecx + 17h], 40h
        je     SHORT loc_1_59DE
        cmp    dword ptr [ecx + 1ch], eax
        je     SHORT loc_1_59DE
        cmp    word ptr [ebp + 10h], 16h
        je     SHORT loc_1_59DE
        xor    eax, eax
        jmp    SHORT loc_1_5A58
loc_1_59DE:
        push   ecx
        push   eax
        call   sub_1_2635
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_59F0
        or     si, word ptr [eax + 10h]
loc_1_59F0:
        movzx  eax, word ptr [ebp + 10h]
        cmp    eax, 16h
        jg     SHORT loc_1_5A08
        je     SHORT loc_1_5A4A
        dec    eax
        cmp    eax, 10h
        ja     SHORT loc_1_5A0D
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_5A08:
        cmp    eax, 40h
        je     SHORT loc_1_5A2C
loc_1_5A0D:
        mov    eax, 1
        jmp    SHORT loc_1_5A58
loc_1_5A14:
        mov    ax, 10h
        jmp    SHORT loc_1_5A4E
loc_1_5A1A:
        mov    ax, 20h
        jmp    SHORT loc_1_5A4E
loc_1_5A20:
        mov    ax, 2
        jmp    SHORT loc_1_5A4E
loc_1_5A26:
        mov    ax, 40h
        jmp    SHORT loc_1_5A4E
loc_1_5A2C:
        mov    ax, 80h
        jmp    SHORT loc_1_5A4E
loc_1_5A32:
        mov    ax, 4
        jmp    SHORT loc_1_5A4E
loc_1_5A38:
        mov    ax, 8
        jmp    SHORT loc_1_5A4E
loc_1_5A3E:
        mov    ax, 100h
        jmp    SHORT loc_1_5A4E
loc_1_5A44:
        mov    ax, 200h
        jmp    SHORT loc_1_5A4E
loc_1_5A4A:
        mov    ax, 400h
loc_1_5A4E:
        and    ax, si
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
loc_1_5A58:
        pop    esi
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
sub_1_5A9F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        test   esi, esi
        je     NEAR PTR loc_1_5B98
loc_1_5AB6:
        and    byte ptr [esi + 8], 0fdh
        cmp    word ptr [esi + 24h], 0
        je     NEAR PTR loc_1_5B8E
        mov    edi, dword ptr [esi + 10h]
        test   edi, edi
        je     NEAR PTR loc_1_5B8E
loc_1_5AD0:
        mov    ax, 1
        mov    cl, byte ptr [edi + 12h]
        shl    ax, cl
        test   word ptr [esi + 24h], ax
        je     NEAR PTR loc_1_5B84
        pushfd
        pop    dword ptr [ebp - 8]
        cli
        not    ax
        and    word ptr [esi + 24h], ax
        push   dword ptr [ebp - 8]
        popfd
        mov    eax, dword ptr [edi + 38h]
        xor    ebx, ebx
        test   eax, eax
        je     SHORT loc_1_5B09
        push   eax
        call   sub_1_25AF
        add    esp, 4
        mov    dword ptr [edi + 38h], ebx
loc_1_5B09:
        push   edi
        call   sub_1_27C1
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_5B1E
        xor    ebx, ebx
        mov    word ptr [ebp - 2], bx
        jmp    SHORT loc_1_5B32
loc_1_5B1E:
        db  066h, 00Fh, 0B6h, 01Dh, 009h, 000h, 000h, 000h
        db  066h, 00Fh, 0B6h, 005h, 003h, 000h, 000h, 000h
        mov    word ptr [ebp - 2], ax
loc_1_5B32:
        mov    ax, word ptr [ebp - 2]
        and    ax, 80h
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
        push   eax
        push   edi
        call   sub_1_2981
        mov    ax, word ptr [edi + 36h]
        add    esp, 8
        xor    ax, word ptr [ebp - 2]
        test   al, 80h
        je     SHORT loc_1_5B5C
        or     bl, 80h
        jmp    SHORT loc_1_5B61
loc_1_5B5C:
        and    bx, 0ff7fh
loc_1_5B61:
        mov    ax, word ptr [ebp - 2]
        push   edi
        or     word ptr [edi + 34h], bx
        mov    word ptr [edi + 36h], ax
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2566
        add    esp, 0ch
        mov    dword ptr [edi + 38h], eax
loc_1_5B84:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    NEAR PTR loc_1_5AD0
loc_1_5B8E:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    NEAR PTR loc_1_5AB6
loc_1_5B98:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_5B9F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        xor    edi, edi
loc_1_5BAC:
        push   edi
        call   sub_1_4FF6
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_1_5C06
        cmp    byte ptr [edi + 20h], bl
        jne    SHORT loc_1_5BAC
        test   byte ptr [ebp + 10h], 2
        je     SHORT loc_1_5BD7
        push   dword ptr [ebp + 8]
        push   esi
        push   edi
        call   sub_1_59B5
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_5BAC
loc_1_5BD7:
        mov    ecx, 0
        test   esi, esi
        je     SHORT loc_1_5BE4
        movzx  ecx, word ptr [esi + 10h]
loc_1_5BE4:
        push   dword ptr [edi + 4]
        lea    edx, [edi + 0ch]
        push   edx
        push   ebx
        push   ebx
        push   ebx
        push   ecx
        push   dword ptr [ebp + 8]
        call   sub_1_24A2
        add    esp, 1ch
        test   byte ptr [ebp + 10h], 1
        je     SHORT loc_1_5BAC
        test   eax, eax
        je     SHORT loc_1_5BAC
        jmp    SHORT loc_1_5C08
loc_1_5C06:
        xor    eax, eax
loc_1_5C08:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_5C0D:
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_1_5C23
loc_1_5C16:
        cmp    dword ptr [esp + 4], eax
        je     SHORT loc_1_5C29
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jne    SHORT loc_1_5C16
loc_1_5C23:
        mov    eax, 21h
loc_1_5C28:
        ret
loc_1_5C29:
        xor    eax, eax
        jmp    SHORT loc_1_5C28
sub_1_5C2D:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 14h]
        push   edi
        call   sub_1_5C0D
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_5DB7
        mov    ax, word ptr [edi + 32h]
        xor    esi, esi
        cmp    ax, 0ffh
        mov    dword ptr [edi + 38h], esi
        jne    SHORT loc_1_5C70
        test   byte ptr [edi + 34h], 80h
        je     SHORT loc_1_5C70
        test   byte ptr [edi + 36h], 80h
        jne    SHORT loc_1_5C70
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5C67:
        mov    word ptr [edi + 32h], si
        add    esp, 4
        jmp    SHORT loc_1_5C79
loc_1_5C70:
        test   ax, ax
        jne    NEAR PTR loc_1_5DB7
loc_1_5C79:
        mov    si, 1
loc_1_5C7D:
        mov    bx, word ptr [edi + 36h]
        mov    ax, word ptr [edi + 34h]
        and    ax, si
        movzx  eax, ax
        cmp    eax, 1
        je     SHORT loc_1_5CB3
        cmp    eax, 2
        je     SHORT loc_1_5CCC
        cmp    eax, 10h
        je     SHORT loc_1_5CE2
        cmp    eax, 20h
        je     SHORT loc_1_5CF3
        cmp    eax, 40h
        je     SHORT loc_1_5D0C
        cmp    eax, 80h
        je     SHORT loc_1_5D22
        xor    bp, bp
        jmp    NEAR PTR loc_1_5D94
loc_1_5CB3:
        and    bx, 1
        mov    ebp, 0
        cmp    bx, 1
        adc    ebp, -1
        and    bp, 17h
        jmp    NEAR PTR loc_1_5D94
loc_1_5CCC:
        and    bx, 2
        cmp    bx, 1
        sbb    ebp, ebp
        and    ebp, 3
        add    bp, 3
        jmp    NEAR PTR loc_1_5D94
loc_1_5CE2:
        and    bx, 10h
        cmp    bx, 1
        sbb    ebp, ebp
        inc    bp
        jmp    NEAR PTR loc_1_5D94
loc_1_5CF3:
        and    bx, 20h
        mov    ebp, 0
        cmp    bx, 1
        adc    ebp, -1
        and    bp, 2
        jmp    NEAR PTR loc_1_5D94
loc_1_5D0C:
        and    bx, 40h
        mov    ebp, 0
        cmp    bx, 1
        adc    ebp, -1
        and    bp, 4
        jmp    SHORT loc_1_5D94
loc_1_5D22:
        xor    bp, bp
        test   byte ptr [edi + 17h], 80h
        je     SHORT loc_1_5D4A
        push   2
        push   edi
        push   5
        call   sub_1_5B9F
        and    byte ptr [edi + 17h], 7fh
        add    esp, 0ch
        test   bl, 80h
        jne    SHORT loc_1_5D4F
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5D47:
        add    esp, 4
loc_1_5D4A:
        test   bl, 80h
        je     SHORT loc_1_5D72
loc_1_5D4F:
        mov    ecx, edi
        call   sub_1_555D
        mov    word ptr [edi + 32h], 40h
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2566
        add    esp, 0ch
        jmp    SHORT loc_1_5D94
loc_1_5D72:
        and    byte ptr [edi + 14h], 0f7h
        push   edi
        call   sub_1_3806
        add    esp, 4
        mov    ecx, edi
        call   sub_1_56A1
        and    byte ptr [edi + 14h], 0efh
        push   4
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5D91:
        add    esp, 4
loc_1_5D94:
        test   bp, bp
        je     SHORT loc_1_5DA5
        push   2
        push   edi
        push   ebp
        call   sub_1_5B9F
        add    esp, 0ch
loc_1_5DA5:
        add    si, si
        test   si, si
        jne    NEAR PTR loc_1_5C7D
        mov    word ptr [edi + 34h], 0
loc_1_5DB7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_5DBC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_5C0D
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_5EC2
        mov    ecx, esi
        call   sub_1_2967
        test   al, 80h
        je     NEAR PTR loc_1_5EA8
        test   al, 40h
        je     NEAR PTR loc_1_5E92
        lea    eax, [esi + 24h]
        push   eax
        call   sub_1_0889
        add    esp, 4
        lea    eax, [esi + 28h]
        push   eax
        call   sub_1_0889
        add    esp, 4
        lea    eax, [esi + 2ch]
        push   eax
        call   sub_1_0889
        add    esp, 4
        mov    ecx, esi
        call   sub_1_5634
        test   byte ptr [esi + 17h], 10h
        jne    SHORT loc_1_5E80
        mov    eax, dword ptr [esi + 0ch]
        cmp    dword ptr [eax + 30h], 0
        jne    SHORT loc_1_5E31
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5E2E:
        add    esp, 4
loc_1_5E31:
        mov    word ptr [ebp - 2], 0
        mov    ax, word ptr [esi + 10h]
        mov    word ptr [ebp - 4], ax
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        call   sub_1_3A1C
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_5E6C
        cmp    word ptr [ebp - 2], 0
        je     SHORT loc_1_5E6C
        push   esi
        call   sub_1_57D5
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_5E6C
        push   esi
        call   sub_1_5978
        jmp    SHORT loc_1_5E7D
loc_1_5E6C:
        push   1
        push   esi
        call   sub_1_574F
        add    esp, 8
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5E7D:
        add    esp, 4
loc_1_5E80:
        or     byte ptr [esi + 14h], 8
        mov    word ptr [esi + 32h], 0ffh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5E90:
        jmp    SHORT loc_1_5EBF
loc_1_5E92:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2566
        add    esp, 0ch
        jmp    SHORT loc_1_5EC2
loc_1_5EA8:
        xor    eax, eax
        mov    word ptr [esi + 32h], ax
        cmp    word ptr [esi + 34h], ax
        je     SHORT loc_1_5EC2
        cmp    dword ptr [esi + 38h], eax
        jne    SHORT loc_1_5EC2
        push   esi
        call   sub_1_5C2D
loc_1_5EBF:
        add    esp, 4
loc_1_5EC2:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_5EC7:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    word ptr [esi + 32h], 0ffh
        jne    SHORT loc_1_5EFF
        push   2
        push   esi
        push   40h
        call   sub_1_5B9F
        add    esp, 0ch
        xor    eax, eax
        mov    word ptr [esi + 32h], ax
        or     byte ptr [esi + 17h], 80h
        cmp    word ptr [esi + 34h], ax
        je     SHORT loc_1_5EFF
        cmp    dword ptr [esi + 38h], eax
        jne    SHORT loc_1_5EFF
        push   esi
        call   sub_1_5C2D
        add    esp, 4
loc_1_5EFF:
        pop    esi
        ret
sub_1_5F01:
        push   esi
        mov    esi, dword ptr [esp + 8]
        test   byte ptr [esi + 17h], 80h
        je     SHORT loc_1_5F1D
        push   2
        push   esi
        push   5
        call   sub_1_5B9F
        and    byte ptr [esi + 17h], 7fh
        add    esp, 0ch
loc_1_5F1D:
        pop    esi
        ret
loc_1_5F1F:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [ebp - 8], 32h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax]
        mov    edi, dword ptr [eax + 4]
        push   esi
        call   sub_1_5C0D
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_5F82
        mov    ecx, esi
        call   sub_1_2967
        mov    word ptr [ebp - 2], ax
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, word ptr [eax + 8]
        cmp    eax, 0eh
        je     SHORT loc_1_5F94
        cmp    eax, 10h
        je     NEAR PTR loc_1_6008
        cmp    eax, 11h
        je     NEAR PTR loc_1_6088
        cmp    eax, 16h
        je     NEAR PTR loc_1_60EE
        cmp    eax, 0feh
        je     NEAR PTR loc_1_610B
loc_1_5F82:
        push   dword ptr [ebp + 8]
        call   sub_1_254F
        add    esp, 4
loc_1_5F8D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_5F94:
        xor    ebx, ebx
        cmp    word ptr [esi + 32h], bx
        jne    NEAR PTR loc_1_604C
        push   3
        push   esi
        push   0eh
        call   sub_1_5B9F
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        cmp    eax, ebx
        jne    SHORT loc_1_5FE1
        push   2
        push   esi
        push   5
        call   sub_1_5B9F
        add    esp, 0ch
        lea    ecx, [edi + 0ch]
        or     byte ptr [esi + 17h], 40h
        mov    dword ptr [esi + 1ch], edi
        push   dword ptr [edi + 4]
        push   ecx
        mov    dx, word ptr [esi + 10h]
        push   ebx
        push   ebx
        push   ebx
        push   edx
        push   40h
        call   sub_1_24A2
        add    esp, 1ch
loc_1_5FE1:
        and    byte ptr [esi + 17h], 0dfh
        mov    eax, dword ptr [edi + 4]
        add    edi, 0ch
        push   eax
        push   edi
        mov    ax, word ptr [esi + 10h]
        push   0
        push   0
        push   dword ptr [ebp - 4]
        push   eax
        push   0dh
        call   sub_1_24A2
        add    esp, 1ch
        jmp    NEAR PTR loc_1_5F82
loc_1_6008:
        cmp    word ptr [esi + 32h], 0
        jne    SHORT loc_1_604C
        push   3
        push   esi
        push   10h
        call   sub_1_5B9F
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_6064
        push   2
        push   esi
        push   0fh
        call   sub_1_5B9F
        add    esp, 0ch
        mov    ecx, esi
        call   sub_1_555D
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 8], 11h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    word ptr [esi + 32h], 11h
        mov    dword ptr [ebp - 8], ecx
loc_1_604C:
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp - 8]
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2566
        add    esp, 0ch
        jmp    NEAR PTR loc_1_5F8D
loc_1_6064:
        mov    ecx, dword ptr [edi + 4]
        add    edi, 0ch
        push   ecx
        push   edi
        mov    cx, word ptr [esi + 10h]
        push   0
        push   0
        push   eax
        push   ecx
        push   80h
        call   sub_1_24A2
        add    esp, 1ch
        jmp    NEAR PTR loc_1_5F82
loc_1_6088:
        cmp    word ptr [esi + 32h], 11h
        jne    SHORT loc_1_604C
        mov    ebx, 80h
        test   word ptr [ebp - 2], bx
        je     SHORT loc_1_60C7
        test   byte ptr [ebp - 2], 40h
        je     SHORT loc_1_604C
        push   2
        add    edi, 0ch
        push   esi
        push   11h
        call   sub_1_5B9F
        add    esp, 0ch
        xor    eax, eax
        push   dword ptr [edi - 8]
        push   edi
        mov    cx, word ptr [esi + 10h]
        push   eax
        push   eax
        push   eax
        push   ecx
        push   ebx
        call   sub_1_24A2
        add    esp, 1ch
loc_1_60C7:
        xor    eax, eax
        mov    word ptr [esi + 32h], ax
        cmp    word ptr [esi + 34h], ax
        je     NEAR PTR loc_1_5F82
        cmp    dword ptr [esi + 38h], eax
        jne    NEAR PTR loc_1_5F82
        push   esi
        call   sub_1_5C2D
        add    esp, 4
        jmp    NEAR PTR loc_1_5F82
loc_1_60EE:
        cmp    word ptr [esi + 32h], 0
        jne    NEAR PTR loc_1_604C
        push   2
        push   esi
        push   16h
        call   sub_1_5B9F
        add    esp, 0ch
        jmp    NEAR PTR loc_1_5F82
loc_1_610B:
        xor    ebx, ebx
        cmp    word ptr [esi + 32h], bx
        jne    NEAR PTR loc_1_604C
        push   dword ptr [edi + 4]
        mov    cx, word ptr [esi + 10h]
        add    edi, 0ch
        push   edi
        push   ebx
        push   ebx
        push   ebx
        push   ecx
        push   5
        call   sub_1_24A2
        and    byte ptr [esi + 17h], 0bfh
        add    esp, 1ch
        mov    dword ptr [esi + 1ch], ebx
        test   byte ptr [ebp - 2], 80h
        je     NEAR PTR loc_1_5F82
        push   2
        push   esi
        push   40h
        call   sub_1_5B9F
        add    esp, 0ch
        jmp    NEAR PTR loc_1_5F82
sub_1_6153:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    esi, dword ptr [edi + 10h]
        test   esi, esi
        je     SHORT loc_1_616F
loc_1_6160:
        push   esi
        call   sub_1_5F01
        add    esp, 4
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    SHORT loc_1_6160
loc_1_616F:
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_1_6186
loc_1_6178:
        cmp    eax, edi
        je     SHORT loc_1_6182
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    SHORT loc_1_6178
loc_1_6182:
        test   eax, eax
        jne    SHORT loc_1_6198
loc_1_6186:
        push   edi
        call   sub_1_8CBE
        add    esp, 4
        push   edi
        call   sub_1_8008
        add    esp, 4
loc_1_6198:
        push   0
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2E0B
        add    esp, 0ch
        push   edi
        call   sub_1_254F
        add    esp, 4
        pop    edi
        pop    esi
        ret
sub_1_61B4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 14h]
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, edi
        and    ebx, 80000000h
        je     SHORT loc_1_61D0
        test   byte ptr [esi + 0bh], 10h
        jne    SHORT loc_1_61DA
loc_1_61D0:
        test   ebx, ebx
        jne    SHORT loc_1_61E4
        test   byte ptr [esi + 0bh], 8
        je     SHORT loc_1_61E4
loc_1_61DA:
        mov    eax, 19h
        jmp    NEAR PTR loc_1_6283
loc_1_61E4:
        test   edi, 20000000h
        je     SHORT loc_1_6208
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     NEAR PTR loc_1_6281
loc_1_61F9:
        and    word ptr [eax + 0ch], 0fffbh
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jne    SHORT loc_1_61F9
        jmp    SHORT loc_1_6281
loc_1_6208:
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 1
        je     SHORT loc_1_6221
        cmp    eax, 2
        je     SHORT loc_1_623A
        cmp    eax, 4
        je     SHORT loc_1_6252
        mov    eax, 0dh
        jmp    SHORT loc_1_6283
loc_1_6221:
        push   80h
        push   edi
        push   dword ptr [ebp + 0ch]
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_46E3
        add    esp, 14h
        jmp    SHORT loc_1_6264
loc_1_623A:
        push   edi
        push   dword ptr [ebp + 0ch]
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_8E41
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_1_6268
        jmp    SHORT loc_1_6283
loc_1_6252:
        push   edi
        push   dword ptr [ebp + 0ch]
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_83E3
        add    esp, 10h
loc_1_6264:
        test   eax, eax
        jne    SHORT loc_1_6283
loc_1_6268:
        test   edi, 40000000h
        jne    SHORT loc_1_6281
        mov    eax, 8000000h
        cmp    ebx, 1
        sbb    ecx, ecx
        and    ecx, eax
        add    ecx, eax
        or     dword ptr [esi + 8], ecx
loc_1_6281:
        xor    eax, eax
loc_1_6283:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_6288:
        push   esi
        push   0
        mov    esi, dword ptr [esp + 10h]
        push   esi
        mov    eax, dword ptr [esi + 10h]
        add    eax, 1ch
        push   eax
        call   sub_1_2E0B
        mov    eax, dword ptr [esp + 14h]
        add    esp, 0ch
        mov    dword ptr [esi + 10h], eax
        push   0
        push   esi
        add    eax, 1ch
        push   eax
        call   sub_1_2D57
        add    esp, 0ch
        pop    esi
        ret
sub_1_62B7:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   ebp
        push   esi
        mov    ebx, dword ptr [esi + 14h]
        call   sub_1_4245
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_6412
        db  081h, 07Eh, 010h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_63AA
        movzx  eax, byte ptr [esi + 1ah]
        cmp    eax, 1
        je     SHORT loc_1_62F3
        cmp    eax, 2
        je     SHORT loc_1_6316
        jmp    NEAR PTR loc_1_63AA
loc_1_62F3:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, ecx
        and    eax, 40000000h
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 20200000h
        sub    eax, 40400001h
        and    eax, ecx
        mov    dword ptr [ebx + 8], eax
        jmp    NEAR PTR loc_1_63AA
loc_1_6316:
        mov    ebp, dword ptr [ebx]
        mov    edi, 1
        xor    al, al
        mov    edx, dword ptr [ebp + 18h]
        mov    cl, byte ptr [ebp + 30h]
        cmp    cl, al
        je     SHORT loc_1_6340
loc_1_6329:
        cmp    edx, ebx
        je     SHORT loc_1_6333
        test   byte ptr [edx + 0ah], 80h
        jne    SHORT loc_1_633E
loc_1_6333:
        inc    al
        add    edx, 28h
        cmp    al, cl
        jb     SHORT loc_1_6329
        jmp    SHORT loc_1_6340
loc_1_633E:
        xor    edi, edi
loc_1_6340:
        test   edi, edi
        je     SHORT loc_1_63A3
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 18h]
        db  089h, 00Dh, 014h, 000h, 000h, 000h
        mov    edx, dword ptr [ebx]
        mov    eax, dword ptr [edx + 0ch]
        mov    ecx, dword ptr [eax + 1ch]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Dh
        db  089h, 00Dh, 00Ah, 000h, 000h, 000h
        mov    edx, dword ptr [ebx]
        mov    eax, dword ptr [edx + 0ch]
        mov    al, byte ptr [eax + 0ch]
        db  0A2h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebx]
        mov    cl, byte ptr [edx + 12h]
        db  088h, 00Dh, 002h, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        jne    NEAR PTR loc_1_6412
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Eh
        db  066h, 081h, 025h, 00Eh, 000h, 000h, 000h, 07Fh, 0FFh
        call   sub_1_2438
        test   eax, eax
        jne    SHORT loc_1_6412
loc_1_63A3:
        and    dword ptr [ebx + 8], 7f7bffffh
loc_1_63AA:
        cmp    byte ptr [esi + 1ah], 2
        jne    SHORT loc_1_63E6
        test   byte ptr [ebx + 0bh], 18h
        jne    SHORT loc_1_63E6
        db  0BAh, 000h, 000h, 000h, 000h
        xor    eax, eax
        mov    edi, edx
        mov    ecx, 9
        rep stosd
        push   edx
        mov    eax, dword ptr [esi + 8]
        db  066h, 0A3h, 002h, 000h, 000h, 000h
        mov    ecx, dword ptr [esi + 14h]
        push   dword ptr [ecx]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_63DA:
        add    esp, 8
        test   eax, eax
        mov    eax, 19h
        jne    SHORT loc_1_6412
loc_1_63E6:
        push   4
        add    ebx, 24h
        push   esi
        push   ebx
        call   sub_1_2E0B
        add    esp, 0ch
        mov    eax, dword ptr [esi + 10h]
        add    eax, 1ch
        push   0
        push   esi
        push   eax
        call   sub_1_2E0B
        add    esp, 0ch
        push   esi
        call   sub_1_254F
        add    esp, 4
        xor    eax, eax
loc_1_6412:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_6417:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        test   byte ptr [esi + 0bh], 1
        je     SHORT loc_1_642E
        mov    eax, 1dh
        jmp    NEAR PTR loc_1_64FF
loc_1_642E:
        mov    eax, dword ptr [esp + 18h]
        cmp    eax, 1
        je     SHORT loc_1_644B
        cmp    eax, 2
        je     SHORT loc_1_648C
        cmp    eax, 4
        je     SHORT loc_1_649F
        mov    eax, 0dh
        jmp    NEAR PTR loc_1_64FF
loc_1_644B:
        mov    edi, dword ptr [esp + 14h]
        push   dword ptr [edi + 4]
        call   sub_1_2650
        add    esp, 4
        mov    ebx, eax
        cmp    ebx, -1
        jne    SHORT loc_1_646B
        mov    eax, 3
        jmp    NEAR PTR loc_1_64FF
loc_1_646B:
        mov    eax, dword ptr [edi + 8]
        sub    eax, dword ptr [edi + 4]
        inc    eax
        push   eax
        push   ebx
        call   sub_1_2678
        add    esp, 8
        cmp    eax, -1
        jne    SHORT loc_1_6488
        mov    eax, 0ah
        jmp    SHORT loc_1_64FF
loc_1_6488:
        xor    cl, cl
        jmp    SHORT loc_1_64AE
loc_1_648C:
        mov    edi, dword ptr [esp + 14h]
        mov    cl, 1
        movzx  ebx, word ptr [edi + 4]
        movzx  eax, word ptr [edi + 6]
        sub    eax, ebx
        inc    eax
        jmp    SHORT loc_1_64AE
loc_1_649F:
        mov    edi, dword ptr [esp + 14h]
        mov    eax, 1
        movzx  ebx, word ptr [edi + 2]
        mov    cl, 2
loc_1_64AE:
        mov    edx, dword ptr [esi + 24h]
        test   edx, edx
        je     SHORT loc_1_64CF
loc_1_64B5:
        cmp    byte ptr [edx + 1ah], cl
        jne    SHORT loc_1_64C4
        cmp    dword ptr [edx + 8], ebx
        jne    SHORT loc_1_64C4
        cmp    dword ptr [edx + 0ch], eax
        je     SHORT loc_1_64CB
loc_1_64C4:
        mov    edx, dword ptr [edx + 4]
        test   edx, edx
        jne    SHORT loc_1_64B5
loc_1_64CB:
        test   edx, edx
        jne    SHORT loc_1_64D6
loc_1_64CF:
        mov    eax, 1ch
        jmp    SHORT loc_1_64FF
loc_1_64D6:
        db  081h, 07Ah, 010h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_64E6
        mov    eax, 19h
        jmp    SHORT loc_1_64FF
loc_1_64E6:
        push   edx
        call   sub_1_62B7
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_64FF
        cmp    dword ptr [esi + 24h], 0
        jne    SHORT loc_1_64FD
        and    byte ptr [esi + 0bh], 0e7h
loc_1_64FD:
        xor    eax, eax
loc_1_64FF:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_6503:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        push   esi
        xor    bl, bl
        lea    ecx, [edx + 14h]
        mov    esi, dword ptr [edx + 18h]
        mov    dl, byte ptr [edx + 30h]
        mov    eax, dword ptr [ecx]
        and    eax, 0fdffffffh
        cmp    dl, bl
        mov    dword ptr [ecx], eax
        je     SHORT loc_1_6539
loc_1_6521:
        test   byte ptr [esi + 0bh], 2
        jne    SHORT loc_1_6532
        inc    bl
        add    esi, 28h
        cmp    bl, dl
        jb     SHORT loc_1_6521
        jmp    SHORT loc_1_6539
loc_1_6532:
        or     eax, 2000000h
        mov    dword ptr [ecx], eax
loc_1_6539:
        pop    esi
        pop    ebx
        ret
sub_1_653C:
        mov    ecx, dword ptr [esp + 4]
        push   ebx
        push   esi
        add    ecx, 0ch
        push   edi
        push   ebp
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 18h]
        db  089h, 015h, 014h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx]
        mov    ebx, dword ptr [eax + 1ch]
        db  089h, 01Dh, 00Ah, 000h, 000h, 000h
        mov    eax, dword ptr [ecx]
        mov    al, byte ptr [eax + 0ch]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 084h
        db  0C7h, 005h, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A2h, 000h, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        jne    NEAR PTR loc_1_6638
        xor    di, di
        db  0BAh, 010h, 000h, 000h, 000h
        xor    bp, bp
        xor    si, si
        db  066h, 039h, 02Dh, 00Eh, 000h, 000h, 000h
        jbe    SHORT loc_1_660C
        xor    cx, cx
        xor    ax, ax
loc_1_65A2:
        test   bp, 8
        mov    bx, word ptr [edx]
        jne    SHORT loc_1_65BE
        cmp    byte ptr [esp + 18h], bl
        jne    SHORT loc_1_65BE
        test   bh, 80h
        je     SHORT loc_1_65BE
        or     di, ax
        or     bp, 8
loc_1_65BE:
        test   bp, 10h
        jne    SHORT loc_1_65D7
        cmp    byte ptr [esp + 1ch], bl
        jne    SHORT loc_1_65D7
        test   bh, 40h
        je     SHORT loc_1_65D7
        or     di, cx
        or     bp, 10h
loc_1_65D7:
        test   bp, 20h
        jne    SHORT loc_1_65F0
        cmp    byte ptr [esp + 20h], bl
        jne    SHORT loc_1_65F0
        test   bh, 20h
        je     SHORT loc_1_65F0
        or     di, si
        or     bp, 20h
loc_1_65F0:
        inc    si
        add    edx, 2
        movzx  ecx, cx
        add    cx, 10h
        movzx  eax, ax
        add    ax, 100h
        db  066h, 039h, 035h, 00Eh, 000h, 000h, 000h
        ja     SHORT loc_1_65A2
loc_1_660C:
        test   bp, 8
        jne    SHORT loc_1_661A
        mov    eax, 0eh
        jmp    SHORT loc_1_6638
loc_1_661A:
        test   bp, 10h
        je     SHORT loc_1_6633
        test   bp, 20h
        je     SHORT loc_1_6633
        mov    eax, dword ptr [esp + 24h]
        mov    word ptr [eax], di
        xor    eax, eax
        jmp    SHORT loc_1_6638
loc_1_6633:
        mov    eax, 0fh
loc_1_6638:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_663D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 2]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   dword ptr [esi]
        call   sub_1_653C
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_1_6668
        mov    eax, 19h
        jmp    NEAR PTR loc_1_67D0
loc_1_6668:
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [eax + 0ch]
        mov    edx, dword ptr [ecx + 18h]
        db  089h, 015h, 014h, 000h, 000h, 000h
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [eax + 0ch]
        mov    edx, dword ptr [ecx + 1ch]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Dh
        db  089h, 015h, 00Ah, 000h, 000h, 000h
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [eax + 0ch]
        mov    dl, byte ptr [ecx + 0ch]
        db  088h, 015h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi]
        mov    al, byte ptr [eax + 12h]
        db  0A2h, 002h, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        jne    NEAR PTR loc_1_67D0
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [eax + 0ch]
        cmp    word ptr [ecx + 26h], 500h
        jb     SHORT loc_1_671C
        db  066h, 0A1h, 004h, 000h, 000h, 000h
        xor    ax, word ptr [ebp - 2]
        test   ah, 0fh
        je     SHORT loc_1_671C
        mov    ax, word ptr [ebp - 2]
        xor    ax, word ptr [ebp + 18h]
        test   ah, 0fh
        je     SHORT loc_1_671C
        mov    eax, 1000h
        db  085h, 005h, 004h, 000h, 000h, 000h
        je     SHORT loc_1_66EC
        test   word ptr [ebp + 18h], ax
        jne    SHORT loc_1_671C
loc_1_66EC:
        db  066h, 0A1h, 004h, 000h, 000h, 000h
        xor    ax, word ptr [ebp + 18h]
        test   ah, 0fh
        je     SHORT loc_1_671C
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Eh
        mov    ax, word ptr [ebp - 2]
        or     ah, 20h
        db  066h, 0A3h, 004h, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        jne    NEAR PTR loc_1_67D0
loc_1_671C:
        mov    al, byte ptr [ebp + 14h]
        db  0A2h, 003h, 000h, 000h, 000h
        db  066h, 081h, 025h, 00Eh, 000h, 000h, 000h, 0FFh, 0FCh
        movzx  eax, word ptr [ebp + 10h]
        test   eax, eax
        je     SHORT loc_1_673C
        cmp    eax, 1
        je     SHORT loc_1_6747
        jmp    SHORT loc_1_674E
loc_1_673C:
        db  066h, 081h, 025h, 00Eh, 000h, 000h, 000h, 07Fh, 0FFh
        jmp    SHORT loc_1_674E
loc_1_6747:
        db  080h, 00Dh, 00Eh, 000h, 000h, 000h, 080h
loc_1_674E:
        mov    ax, word ptr [ebp + 0ch]
        test   ax, ax
        je     SHORT loc_1_676B
        and    ax, 1fh
        db  066h, 081h, 025h, 00Eh, 000h, 000h, 000h, 0E0h, 0FFh
        db  066h, 009h, 005h, 00Eh, 000h, 000h, 000h
loc_1_676B:
        mov    eax, dword ptr [esi]
        mov    ecx, 100h
        test   byte ptr [eax + 17h], 2
        je     SHORT loc_1_6784
        movzx  edx, byte ptr [eax + 31h]
        db  00Fh, 0B7h, 00Ch, 055h, 000h, 000h, 000h, 000h
loc_1_6784:
        mov    eax, dword ptr [esi + 8]
        and    eax, 40000h
        cmp    eax, 1
        sbb    eax, eax
        and    ax, 40h
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Eh
        or     ax, cx
        mov    cx, word ptr [ebp + 18h]
        mov    dx, word ptr [ebp + 1ch]
        db  066h, 089h, 00Dh, 004h, 000h, 000h, 000h
        db  066h, 009h, 005h, 00Eh, 000h, 000h, 000h
        db  066h, 089h, 015h, 012h, 000h, 000h, 000h
        call   sub_1_2438
        cmp    eax, 1
        mov    ecx, eax
        mov    eax, 0
        adc    eax, -1
        and    eax, ecx
loc_1_67D0:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_67D5:
        push   ebx
        xor    eax, eax
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 18h]
        push   ebp
        test   esi, esi
        je     SHORT loc_1_67E5
        mov    dword ptr [esi], eax
loc_1_67E5:
        mov    edi, dword ptr [esp + 18h]
        test   edi, edi
        je     SHORT loc_1_6800
        xor    ax, ax
        xor    ecx, ecx
loc_1_67F2:
        movzx  edx, ax
        inc    ax
        mov    dword ptr [edi + edx*4], ecx
        cmp    ax, 2
        jb     SHORT loc_1_67F2
loc_1_6800:
        mov    ecx, dword ptr [esp + 14h]
        lea    edx, [ecx + 8]
        test   byte ptr [edx + 3], 2
        je     SHORT loc_1_685E
        xor    bp, bp
        mov    ebx, dword ptr [ecx + 24h]
        test   ebx, ebx
        je     SHORT loc_1_685E
loc_1_6817:
        cmp    dword ptr [ebx + 14h], ecx
        jne    SHORT loc_1_6857
        movzx  eax, byte ptr [ebx + 1ah]
        cmp    eax, 1
        je     SHORT loc_1_682C
        cmp    eax, 2
        je     SHORT loc_1_6846
        jmp    SHORT loc_1_6857
loc_1_682C:
        test   edi, edi
        je     SHORT loc_1_6857
        test   byte ptr [edx + 2], 20h
        je     SHORT loc_1_6865
        cmp    bp, 2
        jae    SHORT loc_1_6865
        movzx  eax, bp
        inc    bp
        mov    dword ptr [edi + eax*4], ebx
        jmp    SHORT loc_1_6857
loc_1_6846:
        test   esi, esi
        je     SHORT loc_1_6857
        test   byte ptr [edx + 2], 80h
        je     SHORT loc_1_686C
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_686C
        mov    dword ptr [esi], ebx
loc_1_6857:
        mov    ebx, dword ptr [ebx + 4]
        test   ebx, ebx
        jne    SHORT loc_1_6817
loc_1_685E:
        xor    eax, eax
loc_1_6860:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_6865:
        mov    eax, 19h
        jmp    SHORT loc_1_6860
loc_1_686C:
        mov    eax, 19h
        jmp    SHORT loc_1_6860
sub_1_6873:
        push   ebp
        mov    ecx, dword ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    word ptr [ecx], si
        cmp    dword ptr [ebp + 0ch], esi
        je     SHORT loc_1_68C5
        lea    eax, [ebp - 4]
        mov    edi, dword ptr [ebp + 8]
        push   eax
        push   esi
        push   edi
        call   sub_1_67D5
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_68BE
        cmp    dword ptr [ebp - 4], esi
        je     SHORT loc_1_68BE
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 8], esi
        je     SHORT loc_1_68BE
        or     byte ptr [edi + 0bh], 4
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [eax + 8]
        mov    word ptr [ecx], dx
        jmp    SHORT loc_1_690A
loc_1_68BE:
        mov    eax, 19h
        jmp    SHORT loc_1_690C
loc_1_68C5:
        mov    edi, dword ptr [ebp + 8]
        xor    bl, bl
        and    byte ptr [edi + 0bh], 0fbh
        mov    edi, dword ptr [edi]
        mov    esi, dword ptr [edi + 18h]
        cmp    byte ptr [edi + 30h], bl
        jbe    SHORT loc_1_690A
loc_1_68D8:
        test   byte ptr [esi + 0bh], 4
        je     SHORT loc_1_6900
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   esi
        call   sub_1_67D5
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_6900
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_6900
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 8], 0
        jne    SHORT loc_1_6913
loc_1_6900:
        inc    bl
        add    esi, 28h
        cmp    byte ptr [edi + 30h], bl
        ja     SHORT loc_1_68D8
loc_1_690A:
        xor    eax, eax
loc_1_690C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_6913:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [eax + 8]
        mov    word ptr [ecx], dx
        jmp    SHORT loc_1_690A
sub_1_6921:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    di, di
loc_1_692C:
        mov    ecx, dword ptr [ebp + 8]
        movzx  edx, di
        lea    ebx, [ecx + edx*4]
        mov    ecx, dword ptr [ebx]
        test   ecx, ecx
        je     SHORT loc_1_698A
        mov    ecx, dword ptr [ecx + 14h]
        mov    edx, dword ptr [ecx]
        mov    ecx, dword ptr [edx + 0ch]
        mov    esi, dword ptr [ecx + 14h]
        test   esi, esi
        je     SHORT loc_1_6982
loc_1_694A:
        mov    ecx, dword ptr [ebx]
        cmp    dword ptr [esi + 1ah], ecx
        jne    SHORT loc_1_697C
        lea    ecx, [esi + 14h]
        mov    ax, word ptr [ecx]
        and    ax, 0fffbh
        cmp    dword ptr [ebp + 0ch], 0
        mov    word ptr [ecx], ax
        je     SHORT loc_1_6969
        or     al, 4
        mov    word ptr [ecx], ax
loc_1_6969:
        push   esi
        call   sub_1_4053
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_697C
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_1_698A
loc_1_697C:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    SHORT loc_1_694A
loc_1_6982:
        inc    di
        cmp    di, 2
        jb     SHORT loc_1_692C
loc_1_698A:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_698F:
        push   ebx
        xor    edx, edx
        mov    ecx, dword ptr [esp + 0ch]
        push   esi
        mov    eax, dword ptr [esp + 0ch]
        push   edi
        mov    dword ptr [ecx], 0ffffffffh
        mov    edi, dword ptr [eax + 24h]
        cmp    edi, edx
        je     SHORT loc_1_69CF
loc_1_69A9:
        cmp    byte ptr [edi + 1ah], 1
        jne    SHORT loc_1_69C8
        mov    eax, dword ptr [edi + 8]
        cmp    dword ptr [ecx], eax
        jbe    SHORT loc_1_69B8
        mov    dword ptr [ecx], eax
loc_1_69B8:
        mov    eax, dword ptr [edi + 0ch]
        mov    esi, dword ptr [edi + 8]
        lea    eax, [eax + esi - 1]
        cmp    edx, eax
        jae    SHORT loc_1_69C8
        mov    edx, eax
loc_1_69C8:
        mov    edi, dword ptr [edi + 4]
        test   edi, edi
        jne    SHORT loc_1_69A9
loc_1_69CF:
        mov    eax, dword ptr [ecx]
        cmp    eax, -1
        jne    SHORT loc_1_69E2
        xor    edx, edx
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ecx], edx
        mov    byte ptr [eax], dl
        jmp    SHORT loc_1_6A02
loc_1_69E2:
        xor    edx, eax
        mov    ebx, 1
        cmp    edx, ebx
        jbe    SHORT loc_1_69F7
loc_1_69ED:
        test   ebx, ebx
        je     SHORT loc_1_69F7
        add    ebx, ebx
        cmp    ebx, edx
        jb     SHORT loc_1_69ED
loc_1_69F7:
        dec    ebx
        mov    eax, dword ptr [esp + 18h]
        mov    byte ptr [eax], bl
        not    ebx
        and    dword ptr [ecx], ebx
loc_1_6A02:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_6A06:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 8]
        test   byte ptr [edi], 0fh
        jne    SHORT loc_1_6A20
        xor    eax, eax
        jmp    NEAR PTR loc_1_6B21
loc_1_6A20:
        lea    eax, [ebp - 10h]
        push   eax
        push   dword ptr [esi]
        call   sub_1_4432
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_6A3C
        mov    eax, 19h
        jmp    NEAR PTR loc_1_6B21
loc_1_6A3C:
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [esi + 0ch]
        push   dword ptr [ebp - 10h]
        call   sub_1_450F
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_6A67
        push   dword ptr [ebp - 10h]
        call   sub_1_44FF
        add    esp, 4
        mov    eax, 19h
        jmp    NEAR PTR loc_1_6B21
loc_1_6A67:
        test   byte ptr [edi], 2
        je     SHORT loc_1_6A75
        mov    cl, byte ptr [esi + 10h]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 2], cl
loc_1_6A75:
        test   byte ptr [edi], 4
        je     SHORT loc_1_6A83
        mov    cl, byte ptr [esi + 11h]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 4], cl
loc_1_6A83:
        test   byte ptr [edi], 8
        je     SHORT loc_1_6A91
        mov    cl, byte ptr [esi + 12h]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 6], cl
loc_1_6A91:
        test   byte ptr [edi], 1
        je     SHORT loc_1_6B08
        mov    eax, dword ptr [esi]
        cmp    byte ptr [eax + 30h], 2
        jb     SHORT loc_1_6B00
        mov    al, byte ptr [esi + 13h]
        and    al, 3
        cmp    al, 3
        jne    SHORT loc_1_6B00
        lea    eax, [ebp - 1]
        lea    ecx, [ebp - 0ch]
        push   eax
        push   ecx
        push   esi
        call   sub_1_698F
        add    esp, 0ch
        test   byte ptr [edi], 20h
        je     SHORT loc_1_6AC6
        mov    cl, byte ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0ah], cl
loc_1_6AC6:
        test   byte ptr [edi], 40h
        je     SHORT loc_1_6AD4
        mov    cl, byte ptr [ebp - 0bh]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0ch], cl
loc_1_6AD4:
        test   byte ptr [edi], 80h
        je     SHORT loc_1_6AE2
        mov    cl, byte ptr [ebp - 0ah]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0eh], cl
loc_1_6AE2:
        test   byte ptr [edi + 1], 1
        je     SHORT loc_1_6AF1
        mov    cl, byte ptr [ebp - 9]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 10h], cl
loc_1_6AF1:
        test   byte ptr [edi + 1], 2
        je     SHORT loc_1_6B00
        mov    cl, byte ptr [ebp - 1]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 12h], cl
loc_1_6B00:
        mov    cl, byte ptr [esi + 13h]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], cl
loc_1_6B08:
        push   dword ptr [ebp - 10h]
        call   sub_1_44FF
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 19h
loc_1_6B21:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_6B27:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        lea    eax, [ebp - 2]
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   eax
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        mov    esi, dword ptr [edi]
        push   esi
        call   sub_1_653C
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_1_6D22
        mov    eax, dword ptr [esi + 0ch]
        mov    ecx, dword ptr [eax + 18h]
        db  089h, 00Dh, 014h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 0ch]
        mov    edx, dword ptr [eax + 1ch]
        db  089h, 015h, 00Ah, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 0ch]
        mov    al, byte ptr [eax + 0ch]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 084h
        db  0C7h, 005h, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A2h, 000h, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        jne    NEAR PTR loc_1_6D22
        test   byte ptr [ebp + 19h], 10h
        je     SHORT loc_1_6BA9
        xor    eax, eax
        lea    ebx, [edi + 16h]
        mov    byte ptr [edi + 14h], al
        mov    byte ptr [edi + 15h], al
        mov    byte ptr [ebx], al
        jmp    SHORT loc_1_6BC0
loc_1_6BA9:
        lea    eax, [edi + 14h]
        mov    cl, byte ptr [ebp + 0ch]
        mov    al, byte ptr [ebp + 10h]
        lea    ebx, [edi + 16h]
        mov    byte ptr [edi + 14h], cl
        mov    byte ptr [edi + 15h], al
        mov    cl, byte ptr [ebp + 14h]
        mov    byte ptr [ebx], cl
loc_1_6BC0:
        xor    dl, dl
        mov    eax, dword ptr [esi + 18h]
        cmp    byte ptr [esi + 30h], dl
        je     SHORT loc_1_6C32
loc_1_6BCA:
        cmp    byte ptr [ebp + 0ch], 0
        je     SHORT loc_1_6BF6
        test   byte ptr [ebp + 19h], 10h
        jne    SHORT loc_1_6BF6
        mov    cl, byte ptr [eax + 14h]
        test   cl, cl
        je     SHORT loc_1_6C12
        cmp    byte ptr [ebp + 0ch], cl
        je     SHORT loc_1_6C12
        xor    eax, eax
        mov    byte ptr [edi + 14h], al
        mov    byte ptr [edi + 15h], al
        mov    byte ptr [ebx], al
        mov    eax, 0eh
        jmp    NEAR PTR loc_1_6D22
loc_1_6BF6:
        mov    cl, byte ptr [eax + 14h]
        test   cl, cl
        je     SHORT loc_1_6C12
        and    word ptr [ebp + 18h], 0efffh
        mov    byte ptr [ebp + 0ch], cl
        mov    cl, byte ptr [eax + 15h]
        mov    byte ptr [ebp + 10h], cl
        mov    cl, byte ptr [eax + 16h]
        mov    byte ptr [ebp + 14h], cl
loc_1_6C12:
        mov    cl, byte ptr [eax + 15h]
        cmp    byte ptr [ebp + 10h], cl
        jae    SHORT loc_1_6C1D
        mov    byte ptr [ebp + 10h], cl
loc_1_6C1D:
        mov    cl, byte ptr [eax + 16h]
        cmp    byte ptr [ebp + 14h], cl
        jae    SHORT loc_1_6C28
        mov    byte ptr [ebp + 14h], cl
loc_1_6C28:
        inc    dl
        add    eax, 28h
        cmp    dl, byte ptr [esi + 30h]
        jb     SHORT loc_1_6BCA
loc_1_6C32:
        xor    ax, ax
        db  0BFh, 010h, 000h, 000h, 000h
        mov    word ptr [ebp - 2], 0
        xor    bx, bx
        db  066h, 039h, 005h, 00Eh, 000h, 000h, 000h
        jbe    NEAR PTR loc_1_6D16
        xor    cx, cx
        mov    word ptr [ebp - 0ch], ax
loc_1_6C57:
        mov    dl, byte ptr [edi]
        test   al, 8
        mov    byte ptr [ebp - 5], dl
        jne    SHORT loc_1_6CA7
        cmp    byte ptr [ebp + 0ch], dl
        jne    SHORT loc_1_6CA7
        test   byte ptr [edi + 1], 80h
        je     SHORT loc_1_6CA7
        mov    edx, dword ptr [esi + 0ch]
        cmp    word ptr [edx + 26h], 500h
        jb     SHORT loc_1_6C93
        mov    dx, word ptr [ebp + 18h]
        and    dx, 1000h
        cmp    dx, 1
        sbb    edx, edx
        and    edx, 1000h
        add    edx, 1000h
        jmp    SHORT loc_1_6C95
loc_1_6C93:
        xor    edx, edx
loc_1_6C95:
        mov    word ptr [ebp - 10h], cx
        or     al, 8
        or     word ptr [ebp - 10h], dx
        mov    dx, word ptr [ebp - 10h]
        or     word ptr [ebp - 2], dx
loc_1_6CA7:
        test   al, 10h
        jne    SHORT loc_1_6CCC
        mov    dl, byte ptr [ebp - 5]
        cmp    byte ptr [ebp + 10h], dl
        jne    SHORT loc_1_6CCC
        test   byte ptr [edi + 1], 40h
        je     SHORT loc_1_6CCC
        mov    dx, word ptr [ebp - 2]
        or     al, 10h
        and    dx, 0ff0fh
        or     dx, word ptr [ebp - 0ch]
        mov    word ptr [ebp - 2], dx
loc_1_6CCC:
        test   al, 20h
        jne    SHORT loc_1_6CF0
        mov    dl, byte ptr [ebp - 5]
        cmp    byte ptr [ebp + 14h], dl
        jne    SHORT loc_1_6CF0
        test   byte ptr [edi + 1], 20h
        je     SHORT loc_1_6CF0
        mov    dx, word ptr [ebp - 2]
        or     al, 20h
        and    dx, 0fff0h
        or     dx, bx
        mov    word ptr [ebp - 2], dx
loc_1_6CF0:
        inc    bx
        add    edi, 2
        movzx  edx, word ptr [ebp - 0ch]
        add    dx, 10h
        movzx  ecx, cx
        add    cx, 100h
        mov    word ptr [ebp - 0ch], dx
        db  066h, 03Bh, 01Dh, 00Eh, 000h, 000h, 000h
        jb     NEAR PTR loc_1_6C57
loc_1_6D16:
        mov    cx, word ptr [ebp - 2]
        mov    eax, dword ptr [ebp + 1ch]
        mov    word ptr [eax], cx
        xor    eax, eax
loc_1_6D22:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_6D29:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        mov    word ptr [esi], 0
        mov    al, byte ptr [edi + 7]
        and    al, 0fh
        cmp    al, 8
        je     SHORT loc_1_6D48
        xor    eax, eax
        jmp    NEAR PTR loc_1_6DD6
loc_1_6D48:
        mov    ecx, dword ptr [esp + 0ch]
        mov    eax, dword ptr [ecx]
        mov    eax, dword ptr [eax + 0ch]
        cmp    word ptr [eax + 26h], 501h
        jb     SHORT loc_1_6DD1
        mov    eax, dword ptr [eax + 18h]
        db  0A3h, 014h, 000h, 000h, 000h
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [edx + 0ch]
        mov    edx, dword ptr [eax + 1ch]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Ch
        db  089h, 015h, 00Ah, 000h, 000h, 000h
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 0ch]
        mov    al, byte ptr [edx + 0ch]
        db  0A2h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx]
        mov    dl, byte ptr [ecx + 12h]
        db  0C7h, 005h, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  088h, 015h, 002h, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        jne    SHORT loc_1_6DD6
        db  066h, 083h, 03Dh, 002h, 000h, 000h, 000h, 012h
        jb     SHORT loc_1_6DD1
        xor    dx, dx
        db  066h, 039h, 015h, 010h, 000h, 000h, 000h
        jbe    SHORT loc_1_6DD1
        mov    eax, dword ptr [edi + 12h]
loc_1_6DBA:
        movzx  ecx, dx
        db  039h, 004h, 08Dh, 012h, 000h, 000h, 000h
        je     SHORT loc_1_6DD9
        inc    dx
        db  066h, 03Bh, 015h, 010h, 000h, 000h, 000h
        jb     SHORT loc_1_6DBA
loc_1_6DD1:
        mov    eax, 0bh
loc_1_6DD6:
        pop    edi
        pop    esi
        ret
loc_1_6DD9:
        mov    word ptr [esi], dx
        xor    eax, eax
        jmp    SHORT loc_1_6DD6
sub_1_6DE0:
        xor    dl, dl
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 18h]
        mov    al, byte ptr [eax + 30h]
        cmp    al, dl
        je     SHORT loc_1_6DFF
loc_1_6DF0:
        cmp    byte ptr [ecx + 14h], 0
        jne    SHORT loc_1_6E02
        inc    dl
        add    ecx, 28h
        cmp    dl, al
        jb     SHORT loc_1_6DF0
loc_1_6DFF:
        xor    eax, eax
loc_1_6E01:
        ret
loc_1_6E02:
        mov    eax, 1
        jmp    SHORT loc_1_6E01
sub_1_6E09:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        lea    edi, [ecx + 0ch]
        mov    eax, dword ptr [edi]
        mov    edx, dword ptr [eax + 18h]
        db  089h, 015h, 014h, 000h, 000h, 000h
        mov    eax, dword ptr [edi]
        mov    esi, dword ptr [eax + 1ch]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Dh
        db  089h, 035h, 00Ah, 000h, 000h, 000h
        mov    eax, dword ptr [edi]
        mov    al, byte ptr [eax + 0ch]
        db  0A2h, 000h, 000h, 000h, 000h
        mov    cl, byte ptr [ecx + 12h]
        db  088h, 00Dh, 002h, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        jne    NEAR PTR loc_1_6F22
        db  066h, 08Bh, 035h, 004h, 000h, 000h, 000h
        mov    eax, dword ptr [edi]
        mov    ecx, dword ptr [eax + 18h]
        db  089h, 00Dh, 014h, 000h, 000h, 000h
        mov    eax, dword ptr [edi]
        mov    edx, dword ptr [eax + 1ch]
        db  089h, 015h, 00Ah, 000h, 000h, 000h
        mov    eax, dword ptr [edi]
        mov    al, byte ptr [eax + 0ch]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 084h
        db  0C7h, 005h, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A2h, 000h, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        jne    NEAR PTR loc_1_6F22
        mov    ax, si
        and    ax, 0f00h
        shr    ax, 8
        db  066h, 039h, 005h, 00Eh, 000h, 000h, 000h
        jbe    SHORT loc_1_6F1D
        movzx  eax, ax
        db  066h, 08Bh, 00Ch, 045h, 010h, 000h, 000h, 000h
        test   ch, 80h
        je     SHORT loc_1_6F1D
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], cl
        mov    ax, si
        and    ax, 0f0h
        shr    ax, 4
        db  066h, 039h, 005h, 00Eh, 000h, 000h, 000h
        jbe    SHORT loc_1_6F16
        movzx  eax, ax
        db  066h, 08Bh, 00Ch, 045h, 010h, 000h, 000h, 000h
        test   ch, 40h
        je     SHORT loc_1_6F16
        and    si, 0fh
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax], cl
        db  066h, 039h, 035h, 00Eh, 000h, 000h, 000h
        jbe    SHORT loc_1_6F0F
        movzx  eax, si
        db  066h, 08Bh, 00Ch, 045h, 010h, 000h, 000h, 000h
        test   ch, 20h
        je     SHORT loc_1_6F0F
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax], cl
        xor    eax, eax
        jmp    SHORT loc_1_6F22
loc_1_6F0F:
        mov    eax, 0fh
        jmp    SHORT loc_1_6F22
loc_1_6F16:
        mov    eax, 0fh
        jmp    SHORT loc_1_6F22
loc_1_6F1D:
        mov    eax, 0eh
loc_1_6F22:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_1_6F26:
        sub    esp, 4
        lea    eax, [esp + 2]
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   eax
        push   1000h
        push   32h
        push   32h
        push   32h
        push   esi
        call   sub_1_6B27
        add    esp, 18h
        test   eax, eax
        mov    eax, 19h
        jne    SHORT loc_1_6F70
        push   0
        push   dword ptr [esp + 0ah]
        push   dword ptr [esi]
        call   sub_1_954E
        add    esp, 4
        push   eax
        push   2
        push   0
        push   esi
        call   sub_1_663D
        add    esp, 18h
        xor    eax, eax
loc_1_6F70:
        pop    esi
        add    esp, 4
        ret
sub_1_6F75:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        test   word ptr [edi + 2], 0fff8h
        je     SHORT loc_1_6F93
        mov    eax, 2
        jmp    NEAR PTR loc_1_70B3
loc_1_6F93:
        mov    cl, byte ptr [edi + 7]
        mov    al, cl
        and    al, 0fh
        test   cl, 0f8h
        jne    NEAR PTR loc_1_70AE
        cmp    al, 1
        je     SHORT loc_1_6FB3
        cmp    al, 2
        je     SHORT loc_1_6FB3
        cmp    al, 8
        jne    NEAR PTR loc_1_70AE
loc_1_6FB3:
        lea    eax, [ebp - 8]
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   edi
        push   esi
        call   sub_1_6D29
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_70B3
        lea    eax, [ebp - 6]
        mov    cl, byte ptr [edi + 6]
        push   eax
        mov    dl, byte ptr [edi + 5]
        push   ecx
        mov    al, byte ptr [edi + 4]
        push   edx
        push   eax
        push   dword ptr [esi]
        call   sub_1_653C
        add    esp, 14h
        mov    ebx, eax
        test   ebx, ebx
        jne    NEAR PTR loc_1_70B3
        push   dword ptr [esi]
        call   sub_1_6DE0
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_70AA
        mov    al, byte ptr [edi + 4]
        test   al, al
        je     NEAR PTR loc_1_70AA
        cmp    al, 32h
        je     NEAR PTR loc_1_70AA
        mov    cx, word ptr [ebp - 6]
        mov    eax, 1
        shr    cx, 8
        and    cl, 0fh
        shl    eax, cl
        mov    ecx, dword ptr [esi]
        mov    dword ptr [ebp - 0ch], eax
        mov    ebx, dword ptr [ecx + 0ch]
        test   dword ptr [ebx + 3ch], eax
        je     SHORT loc_1_703B
        mov    eax, 19h
        jmp    SHORT loc_1_70B3
loc_1_703B:
        lea    eax, [ebp - 2]
        lea    edx, [ebp - 3]
        push   eax
        lea    ebx, [ebp - 1]
        push   edx
        push   ebx
        push   ecx
        call   sub_1_6E09
        add    esp, 10h
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_70B3
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [edi + 4], al
        je     SHORT loc_1_70AA
        mov    ax, word ptr [ebp - 6]
        push   0
        or     ah, 20h
        push   eax
        push   dword ptr [esi]
        call   sub_1_954E
        add    esp, 4
        push   eax
        push   2
        push   0
        push   esi
        call   sub_1_663D
        add    esp, 18h
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_70A1
        push   dword ptr [esi]
        call   sub_1_2CC4
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_1_70A1
        mov    eax, dword ptr [esi]
        mov    edx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 0ch]
        or     dword ptr [eax + 3ch], edx
loc_1_70A1:
        push   esi
        call   sub_1_6F26
        add    esp, 4
loc_1_70AA:
        mov    eax, ebx
        jmp    SHORT loc_1_70B3
loc_1_70AE:
        mov    eax, 0dh
loc_1_70B3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_70BA:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        test   word ptr [edi + 2], 0fff8h
        je     SHORT loc_1_70D8
        mov    eax, 2
        jmp    NEAR PTR loc_1_7288
loc_1_70D8:
        mov    al, byte ptr [edi + 7]
        mov    bl, al
        and    bl, 0fh
        test   al, 0f8h
        jne    NEAR PTR loc_1_7283
        test   bl, bl
        je     NEAR PTR loc_1_7283
        movzx  eax, bl
        lea    ecx, [eax - 1]
        test   eax, ecx
        jne    NEAR PTR loc_1_7283
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0bh], 1
        je     SHORT loc_1_7111
        mov    eax, 1dh
        jmp    NEAR PTR loc_1_7288
loc_1_7111:
        lea    eax, [ebp - 4]
        mov    cl, byte ptr [edi + 6]
        push   eax
        mov    dl, byte ptr [edi + 5]
        push   0
        mov    al, byte ptr [edi + 4]
        push   ecx
        push   edx
        push   eax
        push   esi
        call   sub_1_6B27
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_1_7288
        lea    eax, [ebp - 6]
        push   eax
        push   edi
        push   esi
        call   sub_1_6D29
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_7288
        mov    eax, dword ptr [esi]
        cmp    bl, 2
        mov    byte ptr [eax + 31h], bl
        je     SHORT loc_1_7159
        cmp    bl, 8
        jne    SHORT loc_1_7166
loc_1_7159:
        mov    ecx, 2000000h
        mov    eax, dword ptr [esi]
        or     dword ptr [esi + 8], ecx
        or     dword ptr [eax + 14h], ecx
loc_1_7166:
        lea    eax, [ebp - 2]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [edi + 2]
        and    eax, 2
        push   eax
        push   esi
        call   sub_1_6873
        add    esp, 0ch
        mov    ebx, eax
        test   ebx, ebx
        jne    NEAR PTR loc_1_725C
        push   dword ptr [ebp - 6]
        push   dword ptr [ebp - 4]
        push   dword ptr [esi]
        call   sub_1_954E
        add    esp, 4
        cmp    word ptr [ebp - 2], 1
        push   eax
        sbb    eax, eax
        inc    eax
        push   eax
        push   dword ptr [ebp - 2]
        push   esi
        call   sub_1_663D
        add    esp, 18h
        mov    ebx, eax
        test   ebx, ebx
        jne    NEAR PTR loc_1_725C
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esi + 0ch], eax
        mov    eax, dword ptr [esi + 8]
        movzx  ecx, byte ptr [edi + 10h]
        xor    ecx, eax
        and    ecx, 0fh
        xor    ecx, eax
        mov    dword ptr [esi + 8], ecx
        mov    al, byte ptr [edi + 0ch]
        mov    byte ptr [esi + 10h], al
        mov    dl, byte ptr [edi + 0dh]
        mov    byte ptr [esi + 11h], dl
        push   esi
        mov    cl, byte ptr [edi + 0eh]
        mov    byte ptr [esi + 12h], cl
        mov    al, byte ptr [edi + 0fh]
        and    al, 3fh
        or     al, 40h
        mov    byte ptr [esi + 13h], al
        call   sub_1_6A06
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_1_726E
        push   dword ptr [esi]
        and    byte ptr [esi + 0bh], 0fch
        call   sub_1_6503
        add    esp, 4
        lea    eax, [ebp - 4]
        push   eax
        push   1000h
        push   32h
        push   32h
        push   32h
        push   esi
        call   sub_1_6B27
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_1_7228
        mov    ebx, 19h
loc_1_7228:
        lea    eax, [ebp - 2]
        push   eax
        push   0
        push   esi
        call   sub_1_6873
        add    esp, 0ch
        push   0
        push   dword ptr [ebp - 4]
        push   dword ptr [esi]
        call   sub_1_954E
        add    esp, 4
        cmp    word ptr [ebp - 2], 1
        push   eax
        sbb    eax, eax
        inc    eax
        push   eax
        push   dword ptr [ebp - 2]
        push   esi
        call   sub_1_663D
        add    esp, 18h
loc_1_725C:
        push   dword ptr [esi]
        and    byte ptr [esi + 0bh], 0fch
        call   sub_1_6503
        add    esp, 4
        mov    eax, ebx
        jmp    SHORT loc_1_7288
loc_1_726E:
        mov    eax, dword ptr [esi]
        or     byte ptr [esi + 0bh], 1
        and    byte ptr [eax + 14h], 0dfh
        db  0C7h, 046h, 020h, 000h, 000h, 000h, 000h
        xor    eax, eax
        jmp    SHORT loc_1_7288
loc_1_7283:
        mov    eax, 0dh
loc_1_7288:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_728F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi]
        call   sub_1_2967
        and    ax, 80h
        push   0
        mov    bx, ax
        lea    ecx, [ebp - 8]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_72B4:
        xor    ax, ax
        add    esp, 8
        xor    cl, cl
        test   bx, bx
        je     SHORT loc_1_72CD
        test   byte ptr [ebp - 8], 40h
        je     SHORT loc_1_72CD
        mov    ax, 1000h
        mov    cl, 32h
loc_1_72CD:
        lea    edx, [ebp - 4]
        push   edx
        push   eax
        push   ecx
        push   ecx
        push   ecx
        push   esi
        call   sub_1_6B27
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_1_72EC
        mov    eax, 19h
        jmp    NEAR PTR loc_1_73D1
loc_1_72EC:
        lea    edi, [esi + 8]
        mov    eax, dword ptr [edi]
        test   eax, 2000000h
        je     NEAR PTR loc_1_737F
        test   bx, bx
        je     SHORT loc_1_731E
        test   al, 1
        je     SHORT loc_1_731E
        and    al, 0f1h
        push   esi
        mov    byte ptr [esi + 13h], 0
        mov    dword ptr [edi], eax
        call   sub_1_6A06
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_73D1
loc_1_731E:
        push   dword ptr [esi]
        and    byte ptr [edi + 3], 0fdh
        call   sub_1_6503
        add    esp, 4
        lea    eax, [ebp - 2]
        push   eax
        push   0
        push   esi
        call   sub_1_6873
        add    esp, 0ch
        mov    eax, 80h
        test   bx, bx
        je     SHORT loc_1_7352
        push   dword ptr [esi]
        call   sub_1_954E
        movzx  eax, al
        add    esp, 4
loc_1_7352:
        cmp    word ptr [ebp - 2], 1
        push   0
        push   dword ptr [ebp - 4]
        push   eax
        sbb    eax, eax
        inc    eax
        push   eax
        push   dword ptr [ebp - 2]
        push   esi
        call   sub_1_663D
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_1_73C4
        mov    edx, 2000000h
        mov    ecx, dword ptr [esi]
        or     dword ptr [edi], edx
        or     dword ptr [ecx + 14h], edx
        jmp    SHORT loc_1_73D1
loc_1_737F:
        lea    eax, [ebp - 2]
        push   eax
        push   0
        push   esi
        call   sub_1_6873
        add    esp, 0ch
        mov    eax, 80h
        test   bx, bx
        je     SHORT loc_1_73A5
        push   dword ptr [esi]
        call   sub_1_954E
        movzx  eax, al
        add    esp, 4
loc_1_73A5:
        cmp    word ptr [ebp - 2], 1
        push   0
        push   dword ptr [ebp - 4]
        push   eax
        sbb    eax, eax
        inc    eax
        push   eax
        push   dword ptr [ebp - 2]
        push   esi
        call   sub_1_663D
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_1_73D1
loc_1_73C4:
        and    byte ptr [edi + 3], 0feh
        mov    dword ptr [esi + 20h], 0
        xor    eax, eax
loc_1_73D1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_73D8:
        xor    eax, eax
        ret
loc_1_73DB:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ecx + 18h]
        db  08Bh, 03Dh, 00Ah, 000h, 000h, 000h
        lea    eax, [edi + 2]
        test   word ptr [eax], 0fff8h
        mov    dword ptr [ebp - 8], eax
        je     SHORT loc_1_7408
        mov    eax, 2
        jmp    NEAR PTR loc_1_75E8
loc_1_7408:
        db  0A1h, 00Ah, 000h, 000h, 000h
        mov    al, byte ptr [eax + 7]
        mov    bl, al
        and    bl, 0fh
        test   al, 0f8h
        jne    NEAR PTR loc_1_75E3
        cmp    bl, 1
        je     SHORT loc_1_742B
        cmp    bl, 2
        jne    NEAR PTR loc_1_75E3
loc_1_742B:
        test   byte ptr [esi + 0bh], 11h
        je     SHORT loc_1_743B
        mov    eax, 1dh
        jmp    NEAR PTR loc_1_75E8
loc_1_743B:
        lea    eax, [ebp - 4]
        mov    cl, byte ptr [edi + 6]
        push   eax
        mov    dl, byte ptr [edi + 5]
        push   0
        mov    al, byte ptr [edi + 4]
        push   ecx
        push   edx
        push   eax
        push   esi
        call   sub_1_6B27
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_1_75E8
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    bl, 2
        mov    byte ptr [ecx + 31h], bl
        jne    SHORT loc_1_7474
        mov    eax, 2000000h
        or     dword ptr [esi + 8], eax
        or     dword ptr [ecx + 14h], eax
loc_1_7474:
        push   0
        lea    eax, [ebp - 10h]
        push   eax
        push   esi
        call   sub_1_67D5
        add    esp, 0ch
        mov    ebx, eax
        test   ebx, ebx
        jne    NEAR PTR loc_1_75D0
        lea    eax, [ebp - 2]
        xor    ecx, ecx
        push   eax
        mov    edx, dword ptr [ebp - 8]
        mov    cx, word ptr [edx]
        and    ecx, 2
        push   ecx
        push   esi
        call   sub_1_6873
        add    esp, 0ch
        mov    ebx, eax
        test   ebx, ebx
        jne    NEAR PTR loc_1_75D0
        push   0
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 0ch]
        call   sub_1_954E
        add    esp, 4
        cmp    word ptr [ebp - 2], 1
        push   eax
        sbb    eax, eax
        inc    eax
        push   eax
        push   dword ptr [ebp - 2]
        push   esi
        call   sub_1_663D
        add    esp, 18h
        mov    ebx, eax
        test   ebx, ebx
        jne    NEAR PTR loc_1_75D0
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esi + 0ch], eax
        mov    eax, dword ptr [esi + 8]
        movzx  ecx, byte ptr [edi + 10h]
        xor    ecx, eax
        and    ecx, 0fh
        xor    ecx, eax
        mov    dword ptr [esi + 8], ecx
        mov    al, byte ptr [edi + 0ch]
        mov    byte ptr [esi + 10h], al
        mov    dl, byte ptr [edi + 0dh]
        mov    byte ptr [esi + 11h], dl
        push   esi
        mov    cl, byte ptr [edi + 0eh]
        mov    byte ptr [esi + 12h], cl
        mov    al, byte ptr [edi + 0fh]
        and    al, 3fh
        or     al, 40h
        mov    byte ptr [esi + 13h], al
        call   sub_1_6A06
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_754F
        test   byte ptr [esi + 0bh], 2
        je     SHORT loc_1_753E
        push   0
        lea    eax, [ebp - 10h]
        push   1
        push   eax
        call   sub_1_6921
        add    esp, 0ch
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_754F
loc_1_753E:
        or     byte ptr [esi + 0bh], 1
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [esi + 20h], eax
        xor    eax, eax
        jmp    NEAR PTR loc_1_75E8
loc_1_754F:
        test   byte ptr [esi + 0bh], 2
        je     SHORT loc_1_756B
        push   1
        lea    eax, [ebp - 10h]
        push   0
        push   eax
        call   sub_1_6921
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_756B
        mov    ebx, eax
loc_1_756B:
        push   dword ptr [ebp + 0ch]
        and    byte ptr [esi + 0bh], 0fch
        call   sub_1_6503
        add    esp, 4
        lea    eax, [ebp - 4]
        push   eax
        push   1000h
        push   32h
        push   32h
        push   32h
        push   esi
        call   sub_1_6B27
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_1_759B
        mov    ebx, 19h
loc_1_759B:
        lea    eax, [ebp - 2]
        push   eax
        push   0
        push   esi
        call   sub_1_6873
        add    esp, 0ch
        push   0
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 0ch]
        call   sub_1_954E
        add    esp, 4
        cmp    word ptr [ebp - 2], 1
        push   eax
        sbb    eax, eax
        inc    eax
        push   eax
        push   dword ptr [ebp - 2]
        push   esi
        call   sub_1_663D
        add    esp, 18h
loc_1_75D0:
        push   dword ptr [ebp + 0ch]
        and    byte ptr [esi + 0bh], 0fch
        call   sub_1_6503
        add    esp, 4
        mov    eax, ebx
        jmp    SHORT loc_1_75E8
loc_1_75E3:
        mov    eax, 0dh
loc_1_75E8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_75EF:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [edi + 18h]
        mov    ecx, dword ptr [esi + 8]
        test   ecx, 1000000h
        je     NEAR PTR loc_1_7784
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 20h], eax
        jne    NEAR PTR loc_1_7784
        test   ecx, 10000000h
        jne    NEAR PTR loc_1_7784
        push   0
        lea    eax, [ebp - 10h]
        push   eax
        push   esi
        call   sub_1_67D5
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_7789
        mov    ecx, edi
        call   sub_1_2967
        and    ax, 80h
        lea    ecx, [ebp - 4]
        mov    bx, ax
        push   ecx
        push   1000h
        push   32h
        push   32h
        push   32h
        push   esi
        call   sub_1_6B27
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_1_7670
        mov    eax, 19h
        jmp    NEAR PTR loc_1_7789
loc_1_7670:
        test   byte ptr [esi + 0bh], 2
        je     NEAR PTR loc_1_7731
        push   0
        lea    eax, [ebp - 10h]
        push   0
        push   eax
        call   sub_1_6921
        add    esp, 0ch
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    SHORT loc_1_770A
        test   bx, bx
        je     SHORT loc_1_76B6
        mov    eax, dword ptr [esi + 8]
        test   al, 1
        je     SHORT loc_1_76B6
        and    al, 0f1h
        push   esi
        mov    dword ptr [esi + 8], eax
        mov    byte ptr [esi + 13h], 0
        call   sub_1_6A06
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    SHORT loc_1_770A
loc_1_76B6:
        and    byte ptr [esi + 0bh], 0fdh
        push   edi
        call   sub_1_6503
        add    esp, 4
        lea    eax, [ebp - 2]
        push   eax
        push   0
        push   esi
        call   sub_1_6873
        add    esp, 0ch
        mov    eax, 80h
        test   bx, bx
        je     SHORT loc_1_76E8
        push   edi
        call   sub_1_954E
        movzx  eax, al
        add    esp, 4
loc_1_76E8:
        cmp    word ptr [ebp - 2], 1
        push   0
        push   dword ptr [ebp - 4]
        push   eax
        sbb    eax, eax
        inc    eax
        push   eax
        push   dword ptr [ebp - 2]
        push   esi
        call   sub_1_663D
        add    esp, 18h
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     SHORT loc_1_7775
loc_1_770A:
        mov    eax, 2000000h
        push   1
        push   1
        lea    ecx, [ebp - 10h]
        push   ecx
        or     dword ptr [esi + 8], eax
        or     dword ptr [edi + 14h], eax
        call   sub_1_6921
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_772C
        mov    dword ptr [ebp - 8], eax
loc_1_772C:
        mov    eax, dword ptr [ebp - 8]
        jmp    SHORT loc_1_7789
loc_1_7731:
        lea    eax, [ebp - 2]
        push   eax
        push   0
        push   esi
        call   sub_1_6873
        add    esp, 0ch
        mov    eax, 80h
        test   bx, bx
        je     SHORT loc_1_7756
        push   edi
        call   sub_1_954E
        movzx  eax, al
        add    esp, 4
loc_1_7756:
        cmp    word ptr [ebp - 2], 1
        push   0
        push   dword ptr [ebp - 4]
        push   eax
        sbb    eax, eax
        inc    eax
        push   eax
        push   dword ptr [ebp - 2]
        push   esi
        call   sub_1_663D
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_1_7789
loc_1_7775:
        and    byte ptr [esi + 0bh], 0feh
        mov    dword ptr [esi + 20h], 0
        xor    eax, eax
        jmp    SHORT loc_1_7789
loc_1_7784:
        mov    eax, 21h
loc_1_7789:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_7790:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [edi + 18h]
        mov    edx, dword ptr [ecx + 8]
        test   edx, 1000000h
        je     NEAR PTR loc_1_78B0
        mov    eax, dword ptr [esp + 18h]
        cmp    dword ptr [ecx + 20h], eax
        jne    NEAR PTR loc_1_78B0
        db  08Bh, 035h, 00Ah, 000h, 000h, 000h
        add    esi, 2
        mov    bx, word ptr [esi]
        test   bx, 0ffc1h
        jne    NEAR PTR loc_1_78A9
        not    edx
        test   edx, 2800000h
        je     SHORT loc_1_77E4
        test   bl, 6
        jne    NEAR PTR loc_1_78A9
loc_1_77E4:
        test   bl, 20h
        je     SHORT loc_1_77F4
        db  0A1h, 00Ah, 000h, 000h, 000h
        movzx  edx, byte ptr [eax + 6]
        jmp    SHORT loc_1_77F8
loc_1_77F4:
        movzx  edx, byte ptr [ecx + 16h]
loc_1_77F8:
        test   bl, 10h
        je     SHORT loc_1_7808
        db  0A1h, 00Ah, 000h, 000h, 000h
        movzx  ebp, byte ptr [eax + 5]
        jmp    SHORT loc_1_780C
loc_1_7808:
        movzx  ebp, byte ptr [ecx + 15h]
loc_1_780C:
        test   bl, 8
        je     SHORT loc_1_781D
        db  08Bh, 01Dh, 00Ah, 000h, 000h, 000h
        movzx  eax, byte ptr [ebx + 4]
        jmp    SHORT loc_1_7821
loc_1_781D:
        movzx  eax, byte ptr [ecx + 14h]
loc_1_7821:
        lea    ebx, [esp + 12h]
        push   ebx
        push   0
        push   edx
        push   ebp
        push   eax
        push   ecx
        call   sub_1_6B27
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_1_78B5
        lea    ecx, [edi + 0ch]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Dh
        mov    eax, dword ptr [ecx]
        mov    al, byte ptr [eax + 0ch]
        db  0A2h, 000h, 000h, 000h, 000h
        mov    dl, byte ptr [edi + 12h]
        db  088h, 015h, 002h, 000h, 000h, 000h
        mov    ebx, dword ptr [ecx]
        mov    eax, dword ptr [ebx + 18h]
        db  0A3h, 014h, 000h, 000h, 000h
        mov    ebx, dword ptr [ecx]
        mov    ecx, dword ptr [ebx + 1ch]
        db  089h, 00Dh, 00Ah, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        jne    SHORT loc_1_78B5
        mov    ax, word ptr [esi]
        test   al, 4
        je     SHORT loc_1_7890
        test   al, 2
        je     SHORT loc_1_7887
        db  080h, 00Dh, 00Eh, 000h, 000h, 000h, 080h
        jmp    SHORT loc_1_7890
loc_1_7887:
        db  066h, 081h, 025h, 00Eh, 000h, 000h, 000h, 07Fh, 0FFh
loc_1_7890:
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Eh
        mov    ax, word ptr [esp + 12h]
        db  066h, 0A3h, 004h, 000h, 000h, 000h
        call   sub_1_2438
        jmp    SHORT loc_1_78B5
loc_1_78A9:
        mov    eax, 2
        jmp    SHORT loc_1_78B5
loc_1_78B0:
        mov    eax, 21h
loc_1_78B5:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_78BD:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        db  08Bh, 035h, 00Ah, 000h, 000h, 000h
        push   ebp
        mov    edi, esi
        mov    ebp, dword ptr [esp + 24h]
        xor    eax, eax
        mov    ecx, 9
        mov    ebx, dword ptr [ebp + 18h]
        rep stosd
        stosb
        mov    ax, word ptr [ebp + 10h]
        mov    word ptr [esi], ax
        test   byte ptr [ebx + 0bh], 1
        je     SHORT loc_1_78FB
        or     byte ptr [esi + 3], 1
        mov    eax, dword ptr [ebx + 20h]
        mov    ax, word ptr [eax + 16h]
        db  066h, 0A3h, 002h, 000h, 000h, 000h
loc_1_78FB:
        cmp    dword ptr [ebx + 0ch], -1
        jne    SHORT loc_1_790B
        mov    eax, 19h
        jmp    NEAR PTR loc_1_7AC2
loc_1_790B:
        lea    ecx, [ebp + 0ch]
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 18h]
        db  089h, 015h, 014h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx]
        mov    edi, dword ptr [eax + 1ch]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 084h
        db  089h, 03Dh, 00Ah, 000h, 000h, 000h
        mov    eax, dword ptr [ecx]
        mov    al, byte ptr [eax + 0ch]
        db  0C7h, 005h, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A2h, 000h, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        jne    NEAR PTR loc_1_7AC2
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Dh
        mov    al, byte ptr [ebp + 12h]
        db  0A2h, 002h, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        jne    NEAR PTR loc_1_7AC2
        db  066h, 0A1h, 00Eh, 000h, 000h, 000h
        and    ah, 3
        cmp    ah, 1
        jne    SHORT loc_1_797C
        or     byte ptr [esi + 7], 1
        jmp    SHORT loc_1_7980
loc_1_797C:
        or     byte ptr [esi + 7], 2
loc_1_7980:
        test   byte ptr [ebx + 0ah], 80h
        lea    edi, [esi + 19h]
        je     SHORT loc_1_7995
        db  066h, 0A1h, 00Eh, 000h, 000h, 000h
        and    al, 1fh
        mov    byte ptr [edi], al
        jmp    SHORT loc_1_7998
loc_1_7995:
        mov    byte ptr [edi], 0ffh
loc_1_7998:
        db  0F6h, 005h, 00Eh, 000h, 000h, 000h, 080h
        je     SHORT loc_1_79A5
        or     byte ptr [esi + 2], 2
loc_1_79A5:
        test   byte ptr [ebp + 17h], 40h
        je     SHORT loc_1_79AF
        or     byte ptr [esi + 2], 1
loc_1_79AF:
        db  0A1h, 004h, 000h, 000h, 000h
        db  08Bh, 015h, 004h, 000h, 000h, 000h
        shr    eax, 4
        and    dl, 0fh
        and    al, 0fh
        db  08Bh, 00Dh, 004h, 000h, 000h, 000h
        shr    ecx, 8
        and    cl, 0fh
        movzx  eax, al
        movzx  ecx, cl
        db  08Ah, 00Ch, 04Dh, 010h, 000h, 000h, 000h
        mov    byte ptr [esi + 4], cl
        movzx  ecx, dl
        db  08Ah, 004h, 045h, 010h, 000h, 000h, 000h
        mov    byte ptr [esi + 5], al
        lea    eax, [esp + 10h]
        db  08Ah, 014h, 04Dh, 010h, 000h, 000h, 000h
        push   eax
        lea    ecx, [esp + 18h]
        mov    byte ptr [esi + 6], dl
        push   ecx
        push   ebx
        call   sub_1_67D5
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_7AC2
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_7A38
        mov    eax, dword ptr [esp + 10h]
        mov    cl, byte ptr [edi]
        cmp    byte ptr [eax + 8], cl
        je     SHORT loc_1_7A2C
        mov    eax, 19h
        jmp    NEAR PTR loc_1_7AC2
loc_1_7A2C:
        mov    eax, dword ptr [esp + 10h]
        mov    ax, word ptr [eax + 18h]
        mov    word ptr [esi + 1ah], ax
loc_1_7A38:
        xor    cx, cx
loc_1_7A3B:
        movzx  eax, cx
        shl    eax, 2
        lea    edx, [esp + eax + 14h]
        mov    ebp, dword ptr [edx]
        test   ebp, ebp
        je     SHORT loc_1_7A75
        inc    cx
        lea    edi, [esi + eax]
        cmp    cx, 2
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [edi + 1ch], ax
        mov    ebp, dword ptr [edx]
        mov    al, byte ptr [ebp + 0ch]
        mov    byte ptr [edi + 1eh], al
        mov    ebp, dword ptr [edx]
        mov    al, byte ptr [ebp + 19h]
        mov    byte ptr [edi + 1fh], al
        mov    ebp, dword ptr [edx]
        mov    dl, byte ptr [ebp + 18h]
        mov    byte ptr [esi + 24h], dl
        jb     SHORT loc_1_7A3B
loc_1_7A75:
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [esi + 8], eax
        mov    cl, byte ptr [ebx + 13h]
        mov    byte ptr [esi + 0fh], cl
        mov    dl, byte ptr [ebx + 10h]
        mov    byte ptr [esi + 0ch], dl
        mov    al, byte ptr [ebx + 11h]
        mov    byte ptr [esi + 0dh], al
        mov    cl, byte ptr [ebx + 12h]
        mov    byte ptr [esi + 0eh], cl
        mov    eax, dword ptr [ebx + 8]
        and    al, 0fh
        mov    byte ptr [esi + 18h], al
        mov    byte ptr [esi + 10h], al
        mov    cl, byte ptr [ebx + 17h]
        mov    byte ptr [esi + 11h], cl
        mov    dl, byte ptr [ebx + 18h]
        mov    byte ptr [esi + 12h], dl
        mov    al, byte ptr [ebx + 19h]
        mov    byte ptr [esi + 13h], al
        xor    eax, eax
        mov    cx, word ptr [ebx + 1ah]
        mov    word ptr [esi + 14h], cx
        mov    dx, word ptr [ebx + 1ch]
        mov    word ptr [esi + 16h], dx
loc_1_7AC2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_1_7ACA:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        db  08Bh, 035h, 00Ah, 000h, 000h, 000h
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [edi + 18h]
        cmp    dword ptr [eax + 0ch], -1
        jne    SHORT loc_1_7AEB
        mov    eax, 19h
        jmp    SHORT loc_1_7B64
loc_1_7AEB:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_1_4432
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_7B03
        mov    eax, 19h
        jmp    SHORT loc_1_7B64
loc_1_7B03:
        lea    eax, [ebp - 8]
        mov    edx, dword ptr [edi + 18h]
        movzx  ecx, byte ptr [esi + 3]
        push   eax
        add    ecx, dword ptr [edx + 0ch]
        push   ecx
        push   dword ptr [ebp - 4]
        call   sub_1_450F
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_7B33
        push   dword ptr [ebp - 4]
        call   sub_1_44FF
        add    esp, 4
        mov    eax, 19h
        jmp    SHORT loc_1_7B64
loc_1_7B33:
        cmp    byte ptr [esi + 2], 0
        jne    SHORT loc_1_7B43
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax]
        mov    byte ptr [esi + 4], al
        jmp    SHORT loc_1_7B4B
loc_1_7B43:
        mov    cl, byte ptr [esi + 4]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], cl
loc_1_7B4B:
        push   dword ptr [ebp - 4]
        call   sub_1_44FF
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 19h
loc_1_7B64:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_7B6A:
        push   esi
        push   40h
        call   sub_1_2540
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_1_7B92
        mov    eax, dword ptr [esp + 8]
        push   0
        push   esi
        mov    dword ptr [esi + 4], eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2D7C
        add    esp, 0ch
loc_1_7B92:
        mov    eax, esi
        pop    esi
        ret
        db  056h, 057h, 033h, 0FFh, 08Bh, 074h, 024h, 00Ch, 039h, 07Eh, 010h, 075h, 005h, 039h, 07Eh, 014h
        db  074h, 00Fh, 056h, 0E8h, 05Ah, 004h, 000h, 000h, 083h, 0C4h, 004h, 085h, 0C0h, 074h, 002h, 08Bh
        db  0F8h, 06Ah, 000h, 056h, 068h, 000h, 000h, 000h, 000h, 0E8h, 047h, 0B2h, 0FFh, 0FFh, 083h, 0C4h
        db  00Ch, 056h, 0E8h, 082h, 0A9h, 0FFh, 0FFh, 083h, 0C4h, 004h, 085h, 0C0h, 074h, 002h, 08Bh, 0F8h
        db  08Bh, 0C7h, 05Fh, 05Eh, 0C3h
sub_1_7BDB:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        db  089h, 01Dh, 00Ah, 000h, 000h, 000h
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 084h
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  089h, 03Dh, 014h, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        jne    NEAR PTR loc_1_7FCB
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        test   esi, esi
        je     SHORT loc_1_7C36
        mov    eax, dword ptr [ebx]
loc_1_7C27:
        cmp    dword ptr [esi + 4], eax
        je     SHORT loc_1_7C32
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    SHORT loc_1_7C27
loc_1_7C32:
        test   esi, esi
        jne    SHORT loc_1_7C40
loc_1_7C36:
        mov    eax, 19h
        jmp    NEAR PTR loc_1_7FCB
loc_1_7C40:
        db  0A0h, 002h, 000h, 000h, 000h
        mov    byte ptr [esi + 0dh], al
        db  08Ah, 00Dh, 003h, 000h, 000h, 000h
        mov    byte ptr [esi + 0eh], cl
        mov    dword ptr [esi + 18h], edi
        mov    dword ptr [esi + 1ch], ebx
        db  0F6h, 005h, 004h, 000h, 000h, 000h, 008h
        je     SHORT loc_1_7C64
        or     byte ptr [esi + 8], 20h
loc_1_7C64:
        xor    edi, edi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    ecx, edi
        je     NEAR PTR loc_1_7D04
loc_1_7C74:
        mov    edx, dword ptr [ecx + 10h]
        mov    ebx, dword ptr [ecx + 14h]
        mov    dword ptr [ebp - 10h], ebx
        test   edi, edi
        jne    SHORT loc_1_7C9F
        cmp    byte ptr [ecx + 0fh], 0
        je     SHORT loc_1_7CF8
        movzx  ax, byte ptr [esi + 0dh]
        cmp    word ptr [edx + 10h], ax
        jb     SHORT loc_1_7CF8
        movzx  ax, byte ptr [esi + 0eh]
        cmp    ax, word ptr [ebx + 0eh]
        jae    SHORT loc_1_7CF8
        jmp    SHORT loc_1_7D04
loc_1_7C9F:
        mov    ebx, dword ptr [edi + 10h]
        movzx  ax, byte ptr [edi + 0dh]
        add    ax, word ptr [ebx + 10h]
        mov    ebx, dword ptr [edi + 14h]
        mov    word ptr [ebp - 2], ax
        mov    ax, word ptr [ebx + 0eh]
        movzx  bx, byte ptr [edi + 0eh]
        add    ax, bx
        mov    word ptr [ebp - 4], ax
        movzx  ebx, byte ptr [ecx + 0fh]
        movzx  eax, byte ptr [edi + 0fh]
        sub    eax, ebx
        cmp    eax, -1
        je     SHORT loc_1_7CF8
        movzx  edx, word ptr [edx + 10h]
        movzx  eax, word ptr [ebp - 2]
        movzx  ebx, byte ptr [esi + 0dh]
        sub    edx, eax
        cmp    edx, ebx
        jl     SHORT loc_1_7CF8
        mov    ebx, dword ptr [ebp - 10h]
        movzx  eax, word ptr [ebp - 4]
        movzx  edx, word ptr [ebx + 0eh]
        movzx  ebx, byte ptr [esi + 0eh]
        sub    edx, eax
        cmp    edx, ebx
        jge    SHORT loc_1_7D04
loc_1_7CF8:
        mov    edi, ecx
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    NEAR PTR loc_1_7C74
loc_1_7D04:
        xor    ebx, ebx
        test   edi, edi
        jne    SHORT loc_1_7D1C
        mov    word ptr [ebp - 2], bx
        mov    byte ptr [esi + 0fh], bl
        mov    word ptr [ebp - 4], 1
        mov    dword ptr [ebp - 0ch], ebx
        jmp    SHORT loc_1_7D62
loc_1_7D1C:
        mov    al, byte ptr [edi + 0fh]
        inc    al
        mov    byte ptr [esi + 0fh], al
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [ebp - 0ch], ecx
        cmp    dword ptr [ecx], 0
        je     SHORT loc_1_7D3C
loc_1_7D2F:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], ecx
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_7D2F
loc_1_7D3C:
        mov    ebx, dword ptr [edi + 14h]
        cmp    dword ptr [ebx], 0
        je     SHORT loc_1_7D4B
loc_1_7D44:
        mov    ebx, dword ptr [ebx]
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_1_7D44
loc_1_7D4B:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 10h]
        inc    ax
        mov    word ptr [ebp - 2], ax
        mov    ax, word ptr [ebx + 0eh]
        inc    ax
        mov    word ptr [ebp - 4], ax
loc_1_7D62:
        push   0
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2E0B
        add    esp, 0ch
        push   0
        push   esi
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2DB5
        mov    byte ptr [ebp - 0dh], 0
        mov    dword ptr [ebp - 18h], 0
        mov    byte ptr [ebp - 11h], 0
        add    esp, 10h
        cmp    byte ptr [esi + 0eh], 0
        jbe    NEAR PTR loc_1_7EAC
loc_1_7D9C:
        push   26h
        call   sub_1_2540
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        je     NEAR PTR loc_1_7FE8
        mov    cx, word ptr [ebp - 4]
        mov    al, byte ptr [ebp - 11h]
        inc    word ptr [ebp - 4]
        push   0
        push   edi
        mov    dword ptr [edi + 8], esi
        mov    byte ptr [edi + 10h], al
        lea    eax, [esi + 14h]
        mov    word ptr [edi + 0eh], cx
        push   eax
        call   sub_1_2D7C
        add    esp, 0ch
        push   4
        push   edi
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        mov    ebx, edi
        call   sub_1_2DB5
        add    esp, 10h
        lea    eax, [ebp - 1ch]
        push   eax
        push   3
        push   edi
        call   sub_1_2AA9
        add    esp, 0ch
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    NEAR PTR loc_1_7FEF
        db  00Fh, 0B7h, 00Dh, 004h, 000h, 000h, 000h
        db  0A0h, 002h, 000h, 000h, 000h
        mov    dl, al
        mov    dword ptr [edi + 16h], ecx
        xor    ecx, ecx
        and    dl, 4
        mov    cl, al
        and    ecx, 1
        cmp    dl, 1
        sbb    edx, edx
        and    al, 2
        inc    edx
        cmp    al, 1
        sbb    eax, eax
        inc    eax
        cmp    eax, ecx
        jne    NEAR PTR loc_1_7EF0
        cmp    edx, ecx
        je     NEAR PTR loc_1_7EF0
        mov    byte ptr [edi + 11h], cl
        test   cl, cl
        je     SHORT loc_1_7E9D
        cmp    dword ptr [ebp - 1ch], 0
        je     SHORT loc_1_7E9D
        cmp    byte ptr [esi + 0eh], 1
        jbe    SHORT loc_1_7E5F
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 0ah]
        db  039h, 005h, 000h, 000h, 000h, 000h
        jae    SHORT loc_1_7E5F
        db  0A3h, 000h, 000h, 000h, 000h
loc_1_7E5F:
        mov    eax, dword ptr [edi + 16h]
        test   dword ptr [ebp - 18h], eax
        je     SHORT loc_1_7E91
        mov    ecx, dword ptr [esi + 14h]
        test   ecx, ecx
        je     SHORT loc_1_7E85
loc_1_7E6E:
        cmp    dword ptr [ecx + 16h], eax
        je     SHORT loc_1_7E7B
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    SHORT loc_1_7E6E
        jmp    SHORT loc_1_7E81
loc_1_7E7B:
        mov    al, byte ptr [ecx + 12h]
        mov    byte ptr [edi + 12h], al
loc_1_7E81:
        test   ecx, ecx
        jne    SHORT loc_1_7E9D
loc_1_7E85:
        mov    dword ptr [ebp - 8], 19h
        jmp    NEAR PTR loc_1_7FEF
loc_1_7E91:
        or     dword ptr [ebp - 18h], eax
        mov    al, byte ptr [ebp - 0dh]
        mov    byte ptr [edi + 12h], al
        inc    byte ptr [ebp - 0dh]
loc_1_7E9D:
        inc    byte ptr [ebp - 11h]
        mov    al, byte ptr [ebp - 11h]
        cmp    byte ptr [esi + 0eh], al
        ja     NEAR PTR loc_1_7D9C
loc_1_7EAC:
        xor    cl, cl
        mov    eax, 1
        mov    dl, byte ptr [esi + 0dh]
        cmp    dl, cl
        je     SHORT loc_1_7EC7
loc_1_7EBA:
        test   dword ptr [ebp - 18h], eax
        je     SHORT loc_1_7EFC
        inc    cl
        add    eax, eax
        cmp    cl, dl
        jb     SHORT loc_1_7EBA
loc_1_7EC7:
        xor    dl, dl
        cmp    byte ptr [ebp - 0dh], dl
        je     SHORT loc_1_7F1B
loc_1_7ECE:
        xor    ecx, ecx
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, ecx
        je     SHORT loc_1_7F0C
loc_1_7ED7:
        cmp    byte ptr [eax + 11h], 0
        je     SHORT loc_1_7EE8
        cmp    byte ptr [eax + 12h], dl
        jne    SHORT loc_1_7EE8
        inc    ecx
        cmp    ecx, 1
        ja     SHORT loc_1_7F08
loc_1_7EE8:
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    SHORT loc_1_7ED7
        jmp    SHORT loc_1_7F0C
loc_1_7EF0:
        mov    dword ptr [ebp - 8], 19h
        jmp    NEAR PTR loc_1_7FEF
loc_1_7EFC:
        mov    dword ptr [ebp - 8], 19h
        jmp    NEAR PTR loc_1_7FEF
loc_1_7F08:
        or     byte ptr [eax + 0ch], 1
loc_1_7F0C:
        test   ecx, ecx
        je     NEAR PTR loc_1_7FD2
        inc    dl
        cmp    byte ptr [ebp - 0dh], dl
        ja     SHORT loc_1_7ECE
loc_1_7F1B:
        xor    bl, bl
        cmp    byte ptr [esi + 0dh], bl
        jbe    NEAR PTR loc_1_7FC4
loc_1_7F26:
        push   0c8h
        call   sub_1_2540
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        je     NEAR PTR loc_1_7FE8
        push   28h
        call   sub_1_2540
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_1_7FDB
        push   0
        lea    ecx, [esi + 10h]
        push   edi
        mov    dword ptr [eax], edi
        mov    dword ptr [eax + 0ch], 0ffffffffh
        mov    byte ptr [eax + 17h], 0ffh
        mov    byte ptr [eax + 18h], 0ffh
        mov    byte ptr [eax + 19h], 0ffh
        mov    word ptr [eax + 1ah], 0ffffh
        mov    word ptr [eax + 1ch], 0ffffh
        push   ecx
        mov    dword ptr [edi + 18h], eax
        mov    byte ptr [edi + 30h], 1
        mov    ax, word ptr [ebp - 2]
        mov    dword ptr [edi + 0ch], esi
        inc    word ptr [ebp - 2]
        mov    byte ptr [edi + 12h], bl
        mov    byte ptr [edi + 13h], 80h
        mov    dword ptr [edi + 20h], 0
        mov    word ptr [edi + 10h], ax
        inc    bl
        call   sub_1_2D7C
        add    esp, 0ch
        push   4
        push   edi
        push   dword ptr [ebp - 0ch]
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2DB5
        add    esp, 10h
        mov    dword ptr [ebp - 0ch], edi
        cmp    byte ptr [esi + 0dh], bl
        ja     NEAR PTR loc_1_7F26
loc_1_7FC4:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], esi
        xor    eax, eax
loc_1_7FCB:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_7FD2:
        mov    dword ptr [ebp - 8], 19h
        jmp    SHORT loc_1_7FEF
loc_1_7FDB:
        test   edi, edi
        je     SHORT loc_1_7FE8
        push   edi
        call   sub_1_254F
        add    esp, 4
loc_1_7FE8:
        mov    dword ptr [ebp - 8], 20h
loc_1_7FEF:
        push   esi
        call   sub_1_8008
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_8003
        mov    dword ptr [ebp - 8], 19h
loc_1_8003:
        mov    eax, dword ptr [ebp - 8]
        jmp    SHORT loc_1_7FCB
sub_1_8008:
        push   ebx
        push   esi
        push   edi
        push   ebp
        xor    ebp, ebp
        mov    edi, dword ptr [esp + 14h]
        push   ebp
        lea    esi, [edi + 10h]
        push   esi
        call   sub_1_2DDA
        add    esp, 8
        mov    ebx, eax
        cmp    ebx, ebp
        je     SHORT loc_1_8063
loc_1_8025:
        push   4
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2E0B
        add    esp, 0ch
        push   dword ptr [ebx + 18h]
        call   sub_1_254F
        add    esp, 4
        push   ebx
        call   sub_1_254F
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_8052
        mov    ebp, 19h
loc_1_8052:
        push   0
        push   esi
        call   sub_1_2DDA
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_8025
loc_1_8063:
        lea    esi, [edi + 14h]
        push   0
        push   esi
        call   sub_1_2DDA
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_1_80AA
loc_1_8077:
        push   4
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2E0B
        add    esp, 0ch
        push   ebx
        call   sub_1_254F
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_8099
        mov    ebp, 19h
loc_1_8099:
        push   0
        push   esi
        call   sub_1_2DDA
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_8077
loc_1_80AA:
        push   0
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2E0B
        add    esp, 0ch
        push   0
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2D7C
        add    esp, 0ch
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_80D1:
        mov    eax, dword ptr [esp + 4]
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 10h]
        test   esi, esi
        je     SHORT loc_1_8124
        mov    edi, 80h
loc_1_80E3:
        mov    ecx, esi
        call   sub_1_56A1
        mov    ecx, esi
        call   sub_1_2967
        test   di, ax
        je     SHORT loc_1_811E
        cmp    dword ptr [esp + 10h], 1
        mov    word ptr [esi + 34h], di
        mov    eax, 0
        adc    eax, -1
        and    ax, di
        mov    word ptr [esi + 36h], ax
        cmp    word ptr [esi + 32h], 0
        jne    SHORT loc_1_811E
        push   esi
        call   sub_1_5C2D
        add    esp, 4
loc_1_811E:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    SHORT loc_1_80E3
loc_1_8124:
        xor    eax, eax
        pop    edi
        pop    esi
        ret
sub_1_8129:
        push   ebp
        db  0A1h, 014h, 000h, 000h, 000h
        mov    ebp, esp
        db  08Bh, 00Dh, 00Ah, 000h, 000h, 000h
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        sub    esp, 1ch
        mov    word ptr [ebp - 2], dx
        mov    dword ptr [ebp - 1ch], eax
        push   ebx
        mov    dword ptr [ebp - 18h], ecx
        push   esi
        push   edi
        db  0A1h, 004h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 8], ecx
        xor    eax, eax
        mov    dword ptr [ebp - 0ch], eax
        cmp    ecx, eax
        je     SHORT loc_1_81E7
loc_1_8168:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ecx + 20h], eax
        jne    SHORT loc_1_81DB
        add    ecx, 8
        mov    dword ptr [ebp - 10h], ecx
        test   byte ptr [ecx], 8
        jne    SHORT loc_1_8182
        or     byte ptr [ebp - 0ch], 2
loc_1_8182:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ecx + 18h]
        db  0A3h, 014h, 000h, 000h, 000h
        mov    edx, dword ptr [ecx + 1ch]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 09Eh
        db  089h, 015h, 00Ah, 000h, 000h, 000h
        mov    al, byte ptr [ecx + 0ch]
        db  0A2h, 000h, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        je     SHORT loc_1_81AF
        int3
loc_1_81AF:
        db  066h, 083h, 03Dh, 004h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_81DB
        mov    ecx, dword ptr [ebp - 10h]
        mov    eax, dword ptr [ecx]
        test   al, 2
        jne    SHORT loc_1_81CA
        or     byte ptr [ebp - 0ch], 1
        or     al, 2
        mov    dword ptr [ecx], eax
loc_1_81CA:
        db  066h, 0A1h, 004h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        or     byte ptr [ebp - 0ch], 2
        or     word ptr [ecx + 24h], ax
loc_1_81DB:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    SHORT loc_1_8168
loc_1_81E7:
        mov    dx, word ptr [ebp - 2]
        mov    eax, dword ptr [ebp - 1ch]
        test   byte ptr [ebp - 0ch], 1
        db  066h, 089h, 015h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 18h]
        db  0A3h, 014h, 000h, 000h, 000h
        db  089h, 00Dh, 00Ah, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 14h]
        db  0A3h, 004h, 000h, 000h, 000h
        je     SHORT loc_1_8222
        push   1
        push   0
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_9888
        add    esp, 0ch
loc_1_8222:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        and    eax, 2
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_822F:
        push   esi
        xor    eax, eax
        push   edi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    esi, eax
        je     SHORT loc_1_82AD
loc_1_8242:
        test   byte ptr [esi + 8], 1
        je     SHORT loc_1_82A7
        mov    edi, dword ptr [esi + 10h]
        test   edi, edi
        je     SHORT loc_1_82A7
loc_1_824F:
        mov    eax, dword ptr [esi + 18h]
        db  0A3h, 014h, 000h, 000h, 000h
        mov    ecx, dword ptr [esi + 1ch]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Fh
        db  089h, 00Dh, 00Ah, 000h, 000h, 000h
        mov    al, byte ptr [esi + 0ch]
        db  0A2h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [edi + 12h]
        db  0A2h, 002h, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        jne    SHORT loc_1_82A1
        mov    ax, word ptr [edi + 36h]
        db  066h, 00Fh, 0B6h, 00Dh, 003h, 000h, 000h, 000h
        xor    ax, cx
        test   al, 80h
        je     SHORT loc_1_82A1
        mov    ax, 1
        mov    cl, byte ptr [edi + 12h]
        shl    ax, cl
        or     word ptr [esi + 24h], ax
loc_1_82A1:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    SHORT loc_1_824F
loc_1_82A7:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    SHORT loc_1_8242
loc_1_82AD:
        call   sub_1_5A9F
        push   0
        push   7d0h
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_82C3:
        pop    edi
        db  0A3h, 000h, 000h, 000h, 000h
        pop    esi
        ret
sub_1_82CB:
        mov    eax, dword ptr [esp + 4]
        push   esi
        test   byte ptr [eax + 36h], 80h
        je     SHORT loc_1_82DC
        mov    word ptr [eax + 34h], 80h
loc_1_82DC:
        mov    si, 1
        mov    edx, dword ptr [eax + 0ch]
        mov    cl, byte ptr [eax + 12h]
        shl    si, cl
        or     word ptr [edx + 24h], si
        call   sub_1_5A9F
        pop    esi
        ret
loc_1_82F4:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        db  08Bh, 035h, 00Ah, 000h, 000h, 000h
        mov    eax, esi
        mov    ax, word ptr [eax]
        test   ah, 0f8h
        je     SHORT loc_1_8313
        mov    eax, 2
        jmp    NEAR PTR loc_1_839D
loc_1_8313:
        test   al, 1
        jne    SHORT loc_1_834F
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 02Eh
        jne    SHORT loc_1_8334
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 00Dh, 00Ah, 000h, 000h, 000h
        mov    ax, word ptr [eax + 0ah]
        mov    word ptr [ecx + 2], ax
        jmp    SHORT loc_1_839B
loc_1_8334:
        mov    cx, word ptr [esi + 2]
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0ah], cx
        call   sub_1_2A6F
        call   sub_1_9619
        test   eax, eax
        je     SHORT loc_1_839B
        jmp    SHORT loc_1_839D
loc_1_834F:
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_1_2635
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_8369
        mov    eax, 0bh
        jmp    SHORT loc_1_839D
loc_1_8369:
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 02Eh
        jne    SHORT loc_1_837D
        mov    ax, word ptr [eax + 10h]
        mov    word ptr [esi + 2], ax
        jmp    SHORT loc_1_839B
loc_1_837D:
        mov    cx, word ptr [esi + 2]
        push   edi
        mov    word ptr [eax + 10h], cx
        call   sub_1_9572
        add    esp, 4
        push   edi
        call   sub_1_95A0
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_839D
loc_1_839B:
        xor    eax, eax
loc_1_839D:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_1_83A1:
        push   dword ptr [esp + 4]
        call   sub_1_27C1
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_83CF
        mov    ecx, dword ptr [esp + 8]
        db  066h, 00Fh, 0B6h, 005h, 003h, 000h, 000h, 000h
        mov    word ptr [ecx + 2], ax
        xor    eax, eax
        db  066h, 00Fh, 0B6h, 015h, 009h, 000h, 000h, 000h
        mov    word ptr [ecx + 4], dx
loc_1_83CF:
        ret
loc_1_83D0:
        db  0A1h, 00Ah, 000h, 000h, 000h
        push   eax
        push   dword ptr [esp + 0ch]
        call   sub_1_83A1
        add    esp, 8
        ret
sub_1_83E3:
        push   ebp
        mov    al, byte ptr [esp + 14h]
        mov    ebp, esp
        sub    esp, 8
        test   al, 20h
        push   ebx
        push   esi
        push   edi
        je     NEAR PTR loc_1_86BB
        test   al, 0c0h
        jne    NEAR PTR loc_1_86BB
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 8]
        test   eax, 80000000h
        je     SHORT loc_1_8417
        mov    eax, 1eh
        jmp    NEAR PTR loc_1_86C0
loc_1_8417:
        test   eax, 1800000h
        je     SHORT loc_1_8428
        mov    eax, 19h
        jmp    NEAR PTR loc_1_86C0
loc_1_8428:
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [eax + 0ch]
        mov    edx, dword ptr [ecx + 18h]
        db  089h, 015h, 014h, 000h, 000h, 000h
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [eax + 0ch]
        mov    edx, dword ptr [ecx + 1ch]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Ch
        db  089h, 015h, 00Ah, 000h, 000h, 000h
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [eax + 0ch]
        mov    dl, byte ptr [ecx + 0ch]
        db  088h, 015h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi]
        mov    al, byte ptr [eax + 12h]
        db  0C7h, 005h, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A2h, 002h, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        jne    NEAR PTR loc_1_86C0
        db  0F6h, 005h, 004h, 000h, 000h, 000h, 002h
        jne    SHORT loc_1_848D
        mov    eax, 0bh
        jmp    NEAR PTR loc_1_86C0
loc_1_848D:
        mov    ecx, dword ptr [ebp + 10h]
        test   byte ptr [ecx], 2
        je     SHORT loc_1_849D
        db  066h, 0A1h, 00Ah, 000h, 000h, 000h
        jmp    SHORT loc_1_84A3
loc_1_849D:
        db  066h, 0A1h, 006h, 000h, 000h, 000h
loc_1_84A3:
        test   byte ptr [ebp + 17h], 40h
        mov    word ptr [ebp - 4], ax
        mov    ecx, dword ptr [ebp + 10h]
        je     SHORT loc_1_84CA
        mov    ax, word ptr [ecx + 4]
        and    ax, word ptr [ebp - 4]
        mov    word ptr [ecx + 4], ax
        cmp    ax, 1
        sbb    eax, eax
        and    eax, 19h
        jmp    NEAR PTR loc_1_86C0
loc_1_84CA:
        mov    ax, word ptr [ecx + 2]
        mov    dx, word ptr [ecx + 4]
        mov    word ptr [ebp - 2], ax
        mov    word ptr [ebp - 6], dx
        test   ax, ax
        je     SHORT loc_1_84F0
        mov    edx, 1
        mov    cl, al
        shl    edx, cl
        movzx  eax, word ptr [ebp - 4]
        test   eax, edx
        je     SHORT loc_1_84FE
loc_1_84F0:
        cmp    word ptr [ebp - 2], 0
        jne    SHORT loc_1_8508
        cmp    word ptr [ebp - 6], 0
        jne    SHORT loc_1_8508
loc_1_84FE:
        mov    eax, 6
        jmp    NEAR PTR loc_1_86C0
loc_1_8508:
        xor    cl, cl
        mov    eax, dword ptr [esi]
        mov    edi, dword ptr [eax + 18h]
        mov    dl, byte ptr [eax + 30h]
        cmp    dl, cl
        je     SHORT loc_1_8540
loc_1_8516:
        cmp    esi, edi
        je     SHORT loc_1_8537
        mov    eax, dword ptr [edi + 24h]
        test   eax, eax
        je     SHORT loc_1_8537
loc_1_8521:
        cmp    byte ptr [eax + 1ah], 2
        jne    SHORT loc_1_8530
        movzx  ebx, word ptr [ebp - 2]
        cmp    dword ptr [eax + 8], ebx
        jne    SHORT loc_1_855A
loc_1_8530:
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jne    SHORT loc_1_8521
loc_1_8537:
        inc    cl
        add    edi, 28h
        cmp    cl, dl
        jb     SHORT loc_1_8516
loc_1_8540:
        push   1bh
        call   sub_1_2540
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_8564
        mov    eax, 20h
        jmp    NEAR PTR loc_1_86C0
loc_1_855A:
        mov    eax, 6
        jmp    NEAR PTR loc_1_86C0
loc_1_8564:
        db  081h, 07Dh, 008h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_8612
        test   byte ptr [ebp + 17h], 80h
        jne    NEAR PTR loc_1_860C
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [eax + 0ch]
        mov    edx, dword ptr [ecx + 18h]
        db  089h, 015h, 014h, 000h, 000h, 000h
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [eax + 0ch]
        mov    edx, dword ptr [ecx + 1ch]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Dh
        db  089h, 015h, 00Ah, 000h, 000h, 000h
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [eax + 0ch]
        mov    dl, byte ptr [ecx + 0ch]
        db  088h, 015h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi]
        mov    al, byte ptr [eax + 12h]
        db  0A2h, 002h, 000h, 000h, 000h
        call   sub_1_2438
        mov    edi, eax
        test   edi, edi
        jne    NEAR PTR loc_1_8652
        mov    ecx, dword ptr [ebp + 10h]
        db  066h, 081h, 025h, 00Eh, 000h, 000h, 000h, 0E0h, 0FFh
        mov    ax, word ptr [ecx]
        and    ax, 2
        mov    cx, word ptr [ebp - 2]
        cmp    ax, 1
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Eh
        sbb    eax, eax
        and    ax, 40h
        and    cx, 1fh
        or     ax, cx
        or     al, 80h
        db  066h, 009h, 005h, 00Eh, 000h, 000h, 000h
        call   sub_1_2438
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_8652
        or     byte ptr [esi + 0ah], 80h
loc_1_860C:
        or     byte ptr [esi + 0bh], 80h
        jmp    SHORT loc_1_8669
loc_1_8612:
        test   byte ptr [ebp + 17h], 10h
        je     SHORT loc_1_8669
        db  0BAh, 000h, 000h, 000h, 000h
        xor    eax, eax
        mov    edi, edx
        mov    ecx, 9
        rep stosd
        mov    ax, word ptr [ebp - 4]
        push   edx
        and    ax, word ptr [ebp - 6]
        db  066h, 0A3h, 004h, 000h, 000h, 000h
        push   dword ptr [esi]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_863E:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_865F
        cmp    word ptr [ebp - 2], 1
        sbb    edi, edi
        and    edi, 1ah
        add    edi, 6
loc_1_8652:
        push   ebx
        call   sub_1_254F
        add    esp, 4
        mov    eax, edi
        jmp    SHORT loc_1_86C0
loc_1_865F:
        db  066h, 0A1h, 002h, 000h, 000h, 000h
        mov    word ptr [ebp - 2], ax
loc_1_8669:
        movzx  eax, word ptr [ebp - 2]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [ebx + 8], eax
        mov    dword ptr [ebx + 0ch], 1
        mov    byte ptr [ebx + 1ah], 2
        mov    dword ptr [ebx + 10h], ecx
        mov    dword ptr [ebx + 14h], esi
        test   byte ptr [edx], 2
        je     SHORT loc_1_8692
        or     byte ptr [esi + 0ah], 4
        jmp    SHORT loc_1_8696
loc_1_8692:
        and    byte ptr [esi + 0ah], 0fbh
loc_1_8696:
        push   4
        add    esi, 24h
        push   ebx
        push   esi
        call   sub_1_2D57
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        add    eax, 1ch
        push   0
        push   ebx
        push   eax
        call   sub_1_2D57
        add    esp, 0ch
        xor    eax, eax
        jmp    SHORT loc_1_86C0
loc_1_86BB:
        mov    eax, 1ch
loc_1_86C0:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_86C7:
        mov    ecx, dword ptr [esp + 8]
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [ecx + 18h]
        db  08Bh, 035h, 00Ah, 000h, 000h, 000h
        lea    ebp, [esi + 5]
        mov    al, byte ptr [ebp]
        test   al, 20h
        je     NEAR PTR loc_1_887C
        test   al, 0c0h
        jne    NEAR PTR loc_1_887C
        mov    eax, dword ptr [ebx + 8]
        test   eax, 11000000h
        je     SHORT loc_1_8702
        mov    eax, 1dh
        jmp    NEAR PTR loc_1_8881
loc_1_8702:
        test   eax, 800000h
        je     SHORT loc_1_8713
        mov    eax, 1eh
        jmp    NEAR PTR loc_1_8881
loc_1_8713:
        lea    edx, [ecx + 0ch]
        mov    eax, dword ptr [edx]
        mov    edi, dword ptr [eax + 18h]
        db  089h, 03Dh, 014h, 000h, 000h, 000h
        mov    eax, dword ptr [edx]
        mov    edi, dword ptr [eax + 1ch]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Ch
        db  089h, 03Dh, 00Ah, 000h, 000h, 000h
        mov    eax, dword ptr [edx]
        mov    al, byte ptr [eax + 0ch]
        db  0A2h, 000h, 000h, 000h, 000h
        mov    cl, byte ptr [ecx + 12h]
        db  0C7h, 005h, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  088h, 00Dh, 002h, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        jne    NEAR PTR loc_1_8881
        db  0F6h, 005h, 004h, 000h, 000h, 000h, 002h
        jne    SHORT loc_1_8770
        mov    eax, 0bh
        jmp    NEAR PTR loc_1_8881
loc_1_8770:
        test   byte ptr [esi + 2], 7
        je     SHORT loc_1_8780
        mov    eax, 2
        jmp    NEAR PTR loc_1_8881
loc_1_8780:
        mov    eax, dword ptr [ebx + 8]
        test   eax, 8000000h
        je     NEAR PTR loc_1_8815
        test   eax, 80000000h
        jne    SHORT loc_1_879F
        mov    eax, 20h
        jmp    NEAR PTR loc_1_8881
loc_1_879F:
        mov    edi, dword ptr [ebx + 24h]
        test   edi, edi
        je     SHORT loc_1_880E
loc_1_87A6:
        cmp    byte ptr [edi + 1ah], 2
        je     SHORT loc_1_87B3
        mov    edi, dword ptr [edi + 4]
        test   edi, edi
        jne    SHORT loc_1_87A6
loc_1_87B3:
        test   edi, edi
        je     SHORT loc_1_880E
        db  081h, 07Fh, 010h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_880E
        mov    al, byte ptr [ebp]
        test   al, 10h
        je     SHORT loc_1_87E3
        mov    edx, 1
        mov    cl, byte ptr [edi + 8]
        shl    edx, cl
        movzx  eax, word ptr [esi + 6]
        test   eax, edx
        jne    SHORT loc_1_87FF
        mov    eax, 6
        jmp    NEAR PTR loc_1_8881
loc_1_87E3:
        movzx  eax, al
        mov    ecx, dword ptr [edi + 8]
        sub    ecx, eax
        cmp    ecx, 1
        sbb    eax, eax
        inc    eax
        test   al, 0fh
        je     SHORT loc_1_87FF
        mov    eax, 6
        jmp    NEAR PTR loc_1_8881
loc_1_87FF:
        push   edi
        push   dword ptr [esp + 18h]
        call   sub_1_6288
        add    esp, 8
        jmp    SHORT loc_1_8868
loc_1_880E:
        mov    eax, 19h
        jmp    SHORT loc_1_8881
loc_1_8815:
        db  0BFh, 000h, 000h, 000h, 000h
        xor    eax, eax
        mov    ecx, 9
        rep stosd
        mov    cl, byte ptr [ebp]
        test   cl, 10h
        je     SHORT loc_1_8831
        mov    ax, word ptr [esi + 6]
        jmp    SHORT loc_1_883B
loc_1_8831:
        mov    ax, 1
        and    cl, 0fh
        shl    ax, cl
loc_1_883B:
        db  066h, 0A3h, 004h, 000h, 000h, 000h
        movzx  eax, byte ptr [ebp]
        or     eax, 10000000h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   ebx
        push   dword ptr [esp + 20h]
        call   sub_1_83E3
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_1_8881
        mov    eax, dword ptr [esp + 14h]
        mov    edi, dword ptr [eax + 1ch]
loc_1_8868:
        mov    al, byte ptr [edi + 8]
        mov    byte ptr [esi + 4], al
        xor    eax, eax
        and    word ptr [esi + 2], 0feffh
        or     byte ptr [ebx + 0ah], 80h
        jmp    SHORT loc_1_8881
loc_1_887C:
        mov    eax, 1ch
loc_1_8881:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_8886:
        mov    eax, dword ptr [esp + 8]
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 18h]
        mov    ecx, dword ptr [esi + 8]
        test   ecx, 11000000h
        je     SHORT loc_1_88A4
        mov    eax, 1dh
        jmp    NEAR PTR loc_1_8925
loc_1_88A4:
        mov    edi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [edi + 1ch]
        test   edx, edx
        je     SHORT loc_1_88D2
loc_1_88AF:
        cmp    byte ptr [edx + 1ah], 2
        jne    SHORT loc_1_88C8
        db  0A1h, 00Ah, 000h, 000h, 000h
        movzx  eax, byte ptr [eax + 4]
        cmp    eax, dword ptr [edx + 8]
        jne    SHORT loc_1_88C8
        cmp    dword ptr [edx + 14h], esi
        je     SHORT loc_1_88CE
loc_1_88C8:
        mov    edx, dword ptr [edx]
        test   edx, edx
        jne    SHORT loc_1_88AF
loc_1_88CE:
        test   edx, edx
        jne    SHORT loc_1_88D9
loc_1_88D2:
        mov    eax, 6
        jmp    SHORT loc_1_8925
loc_1_88D9:
        cmp    dword ptr [edx + 10h], edi
        je     SHORT loc_1_88E5
        mov    eax, 19h
        jmp    SHORT loc_1_8925
loc_1_88E5:
        db  0A1h, 00Ah, 000h, 000h, 000h
        mov    ax, word ptr [eax + 2]
        cmp    word ptr [edx + 18h], ax
        je     SHORT loc_1_88FB
        mov    eax, 2
        jmp    SHORT loc_1_8925
loc_1_88FB:
        test   ecx, 8000000h
        push   edx
        je     SHORT loc_1_8913
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_6288
        add    esp, 8
        jmp    SHORT loc_1_891F
loc_1_8913:
        call   sub_1_62B7
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_8925
loc_1_891F:
        and    byte ptr [esi + 0ah], 7fh
        xor    eax, eax
loc_1_8925:
        pop    edi
        pop    esi
        ret
sub_1_8928:
        push   esi
        push   1
        xor    esi, esi
        push   esi
        db  089h, 035h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_9888
        add    esp, 0ch
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    eax, esi
        jne    SHORT loc_1_895F
        push   esi
        push   7d0h
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_895A:
        db  0A3h, 000h, 000h, 000h, 000h
loc_1_895F:
        pop    esi
        ret
sub_1_8961:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 085h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 0ch]
        db  0A2h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [esi + 18h]
        db  089h, 00Dh, 014h, 000h, 000h, 000h
        mov    edx, dword ptr [esi + 1ch]
        db  089h, 015h, 00Ah, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        jne    NEAR PTR loc_1_8ADA
        db  0F6h, 005h, 00Eh, 000h, 000h, 000h, 01Fh
        jne    SHORT loc_1_89DF
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 086h
        db  066h, 081h, 025h, 00Eh, 000h, 000h, 000h, 07Fh, 0FFh
        db  080h, 00Dh, 00Eh, 000h, 000h, 000h, 040h
        call   sub_1_2438
        or     byte ptr [esi + 8], 1
        db  0A1h, 000h, 000h, 000h, 000h
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_1_89D8
        call   sub_1_8928
loc_1_89D8:
        xor    eax, eax
        jmp    NEAR PTR loc_1_8ADA
loc_1_89DF:
        push   2
        lea    ecx, [ebp - 8]
        push   dword ptr [esi + 4]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_89ED:
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_8AD5
        push   0
        lea    eax, [ebp - 0ch]
        push   eax
        lea    edx, [ebp - 4]
        push   4
        push   dword ptr [ebp - 8]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_8A0C:
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_1_8AD5
        db  066h, 08Bh, 00Dh, 00Eh, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        and    cx, 1fh
        cmp    word ptr [eax + 2], cx
        jne    NEAR PTR loc_1_8AD5
        push   20h
        call   sub_1_2540
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_8A49
        mov    eax, 20h
        jmp    NEAR PTR loc_1_8ADA
loc_1_8A49:
        db  066h, 0A1h, 00Eh, 000h, 000h, 000h
        lea    ebx, [esi + 8]
        and    ax, 1fh
        mov    word ptr [edi], ax
        mov    word ptr [edi + 2], 0
        mov    ecx, dword ptr [ebp - 4]
        and    byte ptr [ebx], 0f7h
        test   byte ptr [ecx], 1
        je     SHORT loc_1_8A73
        mov    word ptr [edi + 2], 2
        or     byte ptr [ebx], 8
loc_1_8A73:
        add    esi, 20h
        push   edi
        db  0C7h, 047h, 004h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_8A83:
        push   edi
        mov    dword ptr [esi], eax
        test   eax, eax
        jne    SHORT loc_1_8A94
        call   sub_1_254F
        add    esp, 4
        jmp    SHORT loc_1_8AD5
loc_1_8A94:
        call   sub_1_254F
        add    esp, 4
        push   dword ptr [esi]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_8AA3:
        and    byte ptr [ebx], 0feh
        xor    ebx, ebx
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 086h
        db  080h, 00Dh, 00Eh, 000h, 000h, 000h, 0C0h
        call   sub_1_2438
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_1_8AD1
        push   dword ptr [esi]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_8AC8:
        push   dword ptr [esi]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_8ACF:
        mov    dword ptr [esi], ebx
loc_1_8AD1:
        mov    eax, edi
        jmp    SHORT loc_1_8ADA
loc_1_8AD5:
        mov    eax, 19h
loc_1_8ADA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_8AE1:
        push   ebp
        db  0A1h, 00Ah, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        cmp    word ptr [eax], 3
        push   ebx
        push   esi
        push   edi
        db  08Bh, 035h, 004h, 000h, 000h, 000h
        je     SHORT loc_1_8B05
        mov    eax, 16h
        jmp    NEAR PTR loc_1_8C02
loc_1_8B05:
        db  0A1h, 004h, 000h, 000h, 000h
        db  08Bh, 01Dh, 00Ah, 000h, 000h, 000h
        add    ebx, 2
        db  0A3h, 014h, 000h, 000h, 000h
        mov    eax, dword ptr [ebx]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 083h
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        db  0A3h, 00Ah, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        jne    NEAR PTR loc_1_8C02
        db  066h, 08Bh, 03Dh, 002h, 000h, 000h, 000h
        db  066h, 03Bh, 03Dh, 000h, 000h, 000h, 000h
        ja     NEAR PTR loc_1_8BFD
        cmp    di, 201h
        jb     NEAR PTR loc_1_8BFD
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        push   dword ptr [ebx]
        call   sub_1_7BDB
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_8C02
        mov    eax, dword ptr [ebp - 4]
        push   1
        mov    word ptr [eax + 26h], di
        xor    esi, esi
        push   dword ptr [ebp - 4]
        call   sub_1_80D1
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_8BE5
        push   dword ptr [ebp - 4]
        call   sub_1_8961
        add    esp, 4
        mov    edi, eax
        cmp    edi, esi
        jne    SHORT loc_1_8BE5
        push   0ah
        call   sub_1_2540
        add    esp, 4
        mov    edi, eax
        cmp    edi, esi
        je     SHORT loc_1_8BE1
        mov    dword ptr [edi], esi
        mov    dword ptr [edi + 4], esi
        mov    word ptr [edi + 8], 16h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [edi], eax
        test   eax, eax
        je     SHORT loc_1_8BD8
        push   edi
        push   32h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2566
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_8BE1
loc_1_8BD8:
        push   edi
        call   sub_1_254F
        add    esp, 4
loc_1_8BE1:
        xor    eax, eax
        jmp    SHORT loc_1_8C02
loc_1_8BE5:
        push   dword ptr [ebp - 4]
        call   sub_1_8008
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_8BF9
        mov    edi, 19h
loc_1_8BF9:
        mov    eax, edi
        jmp    SHORT loc_1_8C02
loc_1_8BFD:
        mov    eax, 19h
loc_1_8C02:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_8C09:
        push   esi
        db  0A1h, 00Ah, 000h, 000h, 000h
        cmp    word ptr [eax + 4], 3
        je     SHORT loc_1_8C1D
        mov    eax, 16h
        jmp    SHORT loc_1_8C65
loc_1_8C1D:
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        test   esi, esi
        je     SHORT loc_1_8C3E
        db  0A1h, 00Ah, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 6]
loc_1_8C2F:
        cmp    dword ptr [esi + 1ch], eax
        je     SHORT loc_1_8C3A
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    SHORT loc_1_8C2F
loc_1_8C3A:
        test   esi, esi
        jne    SHORT loc_1_8C45
loc_1_8C3E:
        mov    eax, 19h
        jmp    SHORT loc_1_8C65
loc_1_8C45:
        push   esi
        call   sub_1_8CBE
        add    esp, 4
        push   esi
        call   sub_1_8961
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 19h
loc_1_8C65:
        pop    esi
        ret
_LTEXT ends
        end
