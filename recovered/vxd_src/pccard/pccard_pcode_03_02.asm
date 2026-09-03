; pccard_pcode_03_02.asm -- PCCARD object 3 segment _PTEXT  (part 2/2)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_3_42EC:
        mov    edx, dword ptr [esp + 8]
        push   edi
        mov    edi, edx
        xor    eax, eax
        mov    ecx, 2bh
        rep stosd
        stosw
        stosb
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax]
        test   byte ptr [ecx + 17h], 10h
        jne    SHORT loc_3_431E
        mov    byte ptr [edx + 2], 32h
        mov    byte ptr [edx + 3], 32h
        mov    byte ptr [edx + 4], 32h
        mov    byte ptr [edx + 0a9h], 70h
loc_3_431E:
        pop    edi
        ret
sub_3_4320:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    word ptr [ebp - 2], bx
        lea    edi, [ebp - 18h]
        xor    eax, eax
        db  068h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], ebx
        stosd
        stosd
        stosd
        stosd
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_3_42EC
        add    esp, 8
        mov    eax, dword ptr [esi]
        mov    edi, dword ptr [eax + 14h]
        and    edi, 10000000h
        jne    NEAR PTR loc_3_43F7
        cmp    byte ptr [eax + 30h], 1
        jne    SHORT loc_3_437C
        push   1ah
        push   esi
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        je     SHORT loc_3_43B3
        mov    eax, 13h
        jmp    NEAR PTR loc_3_44ED
loc_3_437C:
        push   6
        push   esi
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        je     SHORT loc_3_4395
        mov    eax, 13h
        jmp    NEAR PTR loc_3_44ED
loc_3_4395:
        db  0C6h, 005h, 004h, 000h, 000h, 000h, 01Ah
        push   esi
        call   sub_3_4CDD
        add    esp, 4
        test   eax, eax
        je     SHORT loc_3_43B3
        mov    eax, 13h
        jmp    NEAR PTR loc_3_44ED
loc_3_43B3:
        push   esi
        call   sub_3_4CF1
        add    esp, 4
        test   eax, eax
        je     SHORT loc_3_43CA
        mov    eax, 13h
        jmp    NEAR PTR loc_3_44ED
loc_3_43CA:
        lea    eax, [ebp - 18h]
        lea    ecx, [ebp - 2]
        push   eax
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        db  068h, 000h, 000h, 000h, 000h
        call   sub_3_366B
        add    esp, 10h
        mov    eax, dword ptr [esi + 8]
        movzx  ecx, word ptr [ebp - 2]
        xor    ecx, eax
        and    ecx, 3ffh
        xor    ecx, eax
        mov    dword ptr [esi + 8], ecx
loc_3_43F7:
        test   edi, edi
        je     SHORT loc_3_4414
        push   5
        push   esi
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        je     SHORT loc_3_446C
        mov    eax, 13h
        jmp    NEAR PTR loc_3_44ED
loc_3_4414:
        mov    eax, dword ptr [esi]
        cmp    byte ptr [eax + 30h], 1
        jne    SHORT loc_3_4435
        push   1bh
        push   esi
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        je     SHORT loc_3_446C
        mov    eax, 13h
        jmp    NEAR PTR loc_3_44ED
loc_3_4435:
        push   6
        push   esi
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        je     SHORT loc_3_444E
        mov    eax, 13h
        jmp    NEAR PTR loc_3_44ED
loc_3_444E:
        db  0C6h, 005h, 004h, 000h, 000h, 000h, 01Bh
        push   esi
        call   sub_3_4CDD
        add    esp, 4
        test   eax, eax
        je     SHORT loc_3_446C
        mov    eax, 13h
        jmp    NEAR PTR loc_3_44ED
loc_3_446C:
        push   esi
        call   sub_3_4CF1
        add    esp, 4
        test   eax, eax
        je     SHORT loc_3_4480
        mov    eax, 13h
        jmp    SHORT loc_3_44ED
loc_3_4480:
        db  0BBh, 000h, 000h, 000h, 000h
loc_3_4485:
        push   ebx
        push   esi
        call   sub_3_42EC
        add    esp, 8
        push   edi
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        call   sub_3_3603
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 8]
        mov    cl, byte ptr [ebp - 2]
        db  0A3h, 006h, 000h, 000h, 000h
        xor    eax, eax
        db  088h, 00Dh, 00Eh, 000h, 000h, 000h
        db  0A0h, 0A9h, 000h, 000h, 000h
        and    eax, 3
        push   dword ptr [ebp + 10h]
        push   esi
        mov    ecx, dword ptr [ebp + eax*4 - 18h]
        db  089h, 00Dh, 0ABh, 000h, 000h, 000h
        call   dword ptr [ebp + 0ch]
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_44ED
        push   esi
        call   sub_3_4CDD
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_3_44EB
        push   esi
        call   sub_3_4CF1
        add    esp, 4
        test   eax, eax
        je     SHORT loc_3_4485
loc_3_44EB:
        xor    eax, eax
loc_3_44ED:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_44F4:
        push   ebp
        mov    ebp, esp
        sub    esp, 78h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        cmp    byte ptr [esi + 2], 21h
        jbe    SHORT loc_3_450F
        add    dword ptr [ebp + 14h], 200h
        jmp    SHORT loc_3_4518
loc_3_450F:
        jne    SHORT loc_3_4518
        add    dword ptr [ebp + 14h], 100h
loc_3_4518:
        push   0
        lea    edx, [ebp - 8]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 0ch]
        push   edx
        call   sub_3_5350
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_3_4775
        lea    edi, [esi + 0fh]
        xor    ebx, ebx
        mov    dword ptr [ebp - 4], 0
loc_3_4540:
        mov    ax, word ptr [edi]
        test   ax, ax
        je     NEAR PTR loc_3_45E0
        mov    word ptr [ebp - 54h], ax
        mov    ecx, dword ptr [esi + 1fh]
        mov    ax, word ptr [edi + 2]
        mov    dword ptr [ebp - 4ah], ecx
        mov    word ptr [ebp - 62h], 1
        mov    word ptr [ebp - 60h], 0ch
        mov    word ptr [ebp - 52h], ax
        mov    byte ptr [ebp - 58h], bl
        mov    word ptr [ebp - 5eh], bx
        mov    ecx, 1
        mov    word ptr [ebp - 5ch], bx
        mov    eax, 0ffffh
        mov    word ptr [ebp - 5ah], bx
        mov    byte ptr [ebp - 57h], bl
        cmp    word ptr [ebp - 54h], cx
        jbe    SHORT loc_3_4598
        movzx  edx, word ptr [ebp - 54h]
loc_3_4590:
        add    ecx, ecx
        add    eax, eax
        cmp    ecx, edx
        jb     SHORT loc_3_4590
loc_3_4598:
        mov    word ptr [ebp - 50h], 0ffffh
        mov    word ptr [ebp - 56h], ax
        cmp    word ptr [ebp - 52h], bx
        je     SHORT loc_3_45B6
        mov    ax, word ptr [ebp - 54h]
        add    ax, word ptr [ebp - 52h]
        dec    ax
        mov    word ptr [ebp - 50h], ax
loc_3_45B6:
        mov    word ptr [ebp - 4eh], bx
        push   ebx
        push   1ch
        lea    eax, [ebp - 62h]
        push   eax
        lea    edx, [ebp - 0ch]
        push   2
        mov    byte ptr [ebp - 4ch], bl
        push   dword ptr [ebp - 8]
        push   edx
        mov    byte ptr [ebp - 4bh], bl
        call   sub_3_5348
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_3_4775
loc_3_45E0:
        add    edi, 4
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 4
        jb     NEAR PTR loc_3_4540
        mov    ax, word ptr [esi + 23h]
        test   ax, ax
        je     SHORT loc_3_465D
        mov    word ptr [ebp - 42h], ax
        mov    edx, dword ptr [esi + 25h]
        mov    eax, edx
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ebp - 46h], 0
        and    al, 7fh
        mov    edi, dword ptr [ecx]
        mov    dword ptr [ebp - 3eh], eax
        mov    eax, dword ptr [edi + 8]
        test   eax, eax
        je     SHORT loc_3_462A
        cmp    dword ptr [ebp + 0ch], eax
        je     SHORT loc_3_462A
        test   byte ptr [edi + 14h], 40h
        je     SHORT loc_3_462A
        mov    word ptr [ebp - 46h], 1
loc_3_462A:
        test   dl, 80h
        je     SHORT loc_3_4633
        or     byte ptr [ebp - 46h], 2
loc_3_4633:
        xor    edi, edi
        lea    eax, [ebp - 46h]
        mov    word ptr [ebp - 44h], di
        push   edi
        mov    word ptr [ebp - 40h], di
        push   0ch
        push   eax
        lea    edx, [ebp - 0ch]
        push   4
        push   dword ptr [ebp - 8]
        push   edx
        call   sub_3_5348
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_3_4775
loc_3_465D:
        lea    edi, [esi + 29h]
        xor    ebx, ebx
        mov    dword ptr [ebp - 4], 0
loc_3_4669:
        mov    eax, dword ptr [edi]
        test   eax, eax
        je     SHORT loc_3_46EC
        mov    ecx, dword ptr [edi + 8]
        mov    edx, dword ptr [edi + 0ch]
        mov    dword ptr [ebp - 26h], eax
        mov    dword ptr [ebp - 1eh], ecx
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebp - 12h], edx
        mov    dword ptr [ebp - 22h], eax
        mov    dword ptr [ebp - 36h], ebx
        mov    word ptr [ebp - 2eh], bx
        mov    dword ptr [ebp - 32h], ebx
        mov    word ptr [ebp - 2ch], bx
        mov    ecx, 1
        mov    word ptr [ebp - 3ah], 1
        mov    dword ptr [ebp - 16h], 8
        mov    eax, 0ffffffffh
        cmp    dword ptr [ebp - 26h], ecx
        mov    word ptr [ebp - 38h], 14h
        jbe    SHORT loc_3_46C1
loc_3_46B4:
        test   ecx, ecx
        je     SHORT loc_3_46C1
        add    ecx, ecx
        add    eax, eax
        cmp    ecx, dword ptr [ebp - 26h]
        jb     SHORT loc_3_46B4
loc_3_46C1:
        mov    word ptr [ebp - 1ah], bx
        push   ebx
        mov    word ptr [ebp - 18h], bx
        push   2ch
        lea    edx, [ebp - 0ch]
        mov    dword ptr [ebp - 2ah], eax
        lea    eax, [ebp - 3ah]
        push   eax
        push   1
        push   dword ptr [ebp - 8]
        push   edx
        call   sub_3_5348
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_3_4775
loc_3_46EC:
        add    edi, 10h
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 8
        jb     NEAR PTR loc_3_4669
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax]
        mov    dx, word ptr [ecx + 10h]
        mov    cl, byte ptr [esi + 2]
        mov    ax, word ptr [esi]
        mov    byte ptr [ebp - 74h], cl
        mov    word ptr [ebp - 78h], dx
        mov    cl, byte ptr [esi + 5]
        mov    word ptr [ebp - 76h], ax
        mov    dl, byte ptr [esi + 3]
        mov    al, byte ptr [esi + 4]
        mov    byte ptr [ebp - 73h], dl
        mov    edx, dword ptr [esi + 6]
        mov    byte ptr [ebp - 72h], al
        mov    al, byte ptr [esi + 0ah]
        mov    byte ptr [ebp - 71h], cl
        mov    cl, byte ptr [esi + 0bh]
        mov    dword ptr [ebp - 70h], edx
        mov    dl, byte ptr [esi + 0ch]
        mov    byte ptr [ebp - 6ch], al
        mov    al, byte ptr [esi + 0dh]
        mov    byte ptr [ebp - 6bh], cl
        mov    cl, byte ptr [esi + 0eh]
        mov    byte ptr [ebp - 6ah], dl
        mov    edx, dword ptr [esi + 0abh]
        push   0
        push   16h
        mov    byte ptr [ebp - 69h], al
        lea    eax, [ebp - 78h]
        mov    byte ptr [ebp - 68h], cl
        push   eax
        mov    dword ptr [ebp - 66h], edx
        push   897ch
        lea    edx, [ebp - 0ch]
        push   dword ptr [ebp - 8]
        push   edx
        mov    byte ptr [ebp - 67h], 0
        call   sub_3_5348
        add    esp, 18h
loc_3_4775:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_477C:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        mov    cl, byte ptr [esi + 0a9h]
        and    cl, 0fh
        je     SHORT loc_3_47A9
        cmp    dword ptr [esi + 6], 0
        je     SHORT loc_3_47A2
        test   byte ptr [esi + 0eh], 1
        je     SHORT loc_3_47A2
        cmp    byte ptr [esi + 0dh], 0
        jne    SHORT loc_3_47A9
loc_3_47A2:
        mov    eax, 13h
        jmp    SHORT loc_3_480F
loc_3_47A9:
        xor    eax, eax
        mov    word ptr [esi], ax
        cmp    word ptr [esi + 23h], ax
        je     SHORT loc_3_47BF
        mov    word ptr [esi + 23h], 0ffffh
        mov    word ptr [esi], 2
loc_3_47BF:
        mov    byte ptr [esi + 0ah], 0
        movzx  eax, cl
        mov    bl, byte ptr [esi + 0eh]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        test   bl, 2
        mov    byte ptr [esi + 5], al
        je     SHORT loc_3_47FA
        lea    eax, [esp + 8]
        push   0
        push   eax
        call   sub_3_1180
        add    esp, 8
        test   byte ptr [esp + 8], 10h
        jne    SHORT loc_3_47FA
        test   byte ptr [esi + 0aah], 8
        je     SHORT loc_3_47FA
        mov    byte ptr [esi + 0ah], 8
loc_3_47FA:
        mov    byte ptr [esi + 0bh], 0
        and    bl, 0fbh
        test   bl, 8
        mov    byte ptr [esi + 0eh], bl
        je     SHORT loc_3_480D
        mov    byte ptr [esi + 0ch], 0
loc_3_480D:
        xor    eax, eax
loc_3_480F:
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_3_4815:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  068h, 000h, 000h, 000h, 000h
        call   sub_3_477C
        add    esp, 4
        test   eax, eax
        je     SHORT loc_3_4834
        xor    eax, eax
        jmp    NEAR PTR loc_3_48E5
loc_3_4834:
        xor    ecx, ecx
        xor    edx, edx
        db  0B8h, 00Fh, 000h, 000h, 000h
loc_3_483D:
        cmp    word ptr [eax], 0
        je     SHORT loc_3_4852
        mov    ecx, 1
        cmp    word ptr [eax + 2], cx
        sbb    esi, esi
        add    esi, ecx
        or     edx, esi
loc_3_4852:
        add    eax, 4
        db  03Dh, 01Fh, 000h, 000h, 000h
        jb     SHORT loc_3_483D
        mov    ebx, 2000h
        test   edx, edx
        jne    SHORT loc_3_4876
        cmp    ecx, 1
        sbb    ebx, ebx
        and    ebx, 2000h
        add    ebx, 3000h
loc_3_4876:
        mov    edi, dword ptr [esp + 1ch]
        push   ebx
        mov    esi, dword ptr [esp + 1ch]
        db  068h, 000h, 000h, 000h, 000h
        db  080h, 025h, 025h, 000h, 000h, 000h, 07Fh
        push   dword ptr [edi]
        push   esi
        call   sub_3_44F4
        add    esp, 10h
        mov    ebp, eax
        test   ebp, ebp
        jne    SHORT loc_3_48E5
        lea    eax, [esp + 10h]
        push   eax
        push   dword ptr [esi]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_48A8:
        add    esp, 8
        db  00Fh, 0B7h, 00Dh, 023h, 000h, 000h, 000h
        test   dword ptr [esp + 10h], ecx
        je     SHORT loc_3_48E3
        mov    eax, dword ptr [esp + 10h]
        add    ebx, 800h
        db  080h, 00Dh, 025h, 000h, 000h, 000h, 080h
        db  066h, 021h, 005h, 023h, 000h, 000h, 000h
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [edi]
        push   esi
        call   sub_3_44F4
        add    esp, 10h
        mov    ebp, eax
loc_3_48E3:
        mov    eax, ebp
loc_3_48E5:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_3_48ED:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  066h, 083h, 03Dh, 023h, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        je     SHORT loc_3_492E
        mov    esi, dword ptr [ebp + 0ch]
        lea    eax, [ebp - 1]
        mov    byte ptr [ebp - 1], 0
        mov    bl, byte ptr [esi]
        push   eax
        push   dword ptr [ebp + 8]
        or     bl, 1
        mov    byte ptr [esi], bl
        call   sub_3_2D49
        add    esp, 8
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_3_492E
        or     bl, 2
        mov    eax, 13h
        mov    byte ptr [esi], bl
        jmp    SHORT loc_3_4930
loc_3_492E:
        xor    eax, eax
loc_3_4930:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_4936:
        mov    ecx, dword ptr [esp + 4]
        movzx  eax, byte ptr [ecx + 1eh]
        cmp    eax, 2
        je     SHORT loc_3_494D
        cmp    eax, 9
        mov    eax, 13h
        jne    SHORT loc_3_495A
loc_3_494D:
        push   dword ptr [esp + 8]
        push   ecx
        call   sub_3_2FB8
        add    esp, 8
loc_3_495A:
        ret
sub_3_495B:
        lea    eax, [esp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [esp + 0ch]
        call   sub_3_4320
        add    esp, 0ch
        ret
sub_3_4972:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax]
        mov    al, byte ptr [esi + 30h]
        cmp    al, 1
        ja     SHORT loc_3_4992
        mov    eax, 13h
        jmp    NEAR PTR loc_3_4AF2
loc_3_4992:
        xor    edi, edi
        mov    ebx, dword ptr [esi + 18h]
        mov    byte ptr [ebp - 2], 0
        mov    byte ptr [ebp - 3], 0
        test   al, al
        je     SHORT loc_3_49D6
loc_3_49A3:
        mov    byte ptr [ebp - 1], 0
        lea    eax, [ebp - 1]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   ebx
        call   sub_3_4320
        add    esp, 0ch
        test   byte ptr [ebp - 1], 1
        je     SHORT loc_3_49C0
        inc    edi
loc_3_49C0:
        mov    al, byte ptr [ebp - 1]
        add    ebx, 28h
        and    al, 2
        inc    byte ptr [ebp - 3]
        mov    cl, byte ptr [ebp - 3]
        or     byte ptr [ebp - 2], al
        cmp    byte ptr [esi + 30h], cl
        ja     SHORT loc_3_49A3
loc_3_49D6:
        cmp    edi, 2
        jb     NEAR PTR loc_3_4ACF
        or     byte ptr [esi + 14h], 40h
        db  0BFh, 000h, 000h, 000h, 000h
        xor    ebx, ebx
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_3_42EC
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 002h, 000h
        add    esp, 8
        db  088h, 01Dh, 002h, 000h, 000h, 000h
        db  088h, 01Dh, 003h, 000h, 000h, 000h
        db  088h, 01Dh, 004h, 000h, 000h, 000h
        db  0C6h, 005h, 005h, 000h, 000h, 000h, 002h
        test   byte ptr [ebp - 2], 2
        db  089h, 01Dh, 025h, 000h, 000h, 000h
        je     SHORT loc_3_4A49
        db  066h, 0C7h, 005h, 023h, 000h, 000h, 000h, 018h, 000h
        push   2000h
        push   edi
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_3_44F4
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_3_4AF2
loc_3_4A49:
        db  066h, 0C7h, 005h, 023h, 000h, 000h, 000h, 0FFh, 0FFh
        push   3000h
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_3_44F4
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_3_4AF2
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   dword ptr [ecx]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_4A80:
        db  00Fh, 0B7h, 00Dh, 023h, 000h, 000h, 000h
        add    esp, 8
        test   dword ptr [ebp - 8], ecx
        je     SHORT loc_3_4AB8
        mov    eax, dword ptr [ebp - 8]
        push   3800h
        db  080h, 00Dh, 025h, 000h, 000h, 000h, 080h
        db  066h, 021h, 005h, 023h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_3_44F4
        add    esp, 10h
loc_3_4AB8:
        push   0
        push   dword ptr [ebp + 0ch]
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   4
        push   0
        call   sub_3_5440
        add    esp, 18h
loc_3_4ACF:
        push   0
        push   esi
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 0ch]
        call   sub_3_5430
        add    esp, 10h
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp + 0ch]
        call   sub_3_5288
        add    esp, 0ch
loc_3_4AF2:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_4AF9:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 3ch]
        movzx  eax, byte ptr [edx + 2]
        cmp    eax, 1
        je     SHORT loc_3_4B1F
        cmp    eax, 2
        je     SHORT loc_3_4B2A
        cmp    eax, 3
        je     SHORT loc_3_4B35
        mov    eax, 13h
        jmp    SHORT loc_3_4B3E
loc_3_4B1F:
        push   dword ptr [ebp + 0ch]
        push   ecx
        call   sub_3_4936
        jmp    SHORT loc_3_4B3E
loc_3_4B2A:
        push   dword ptr [ebp + 0ch]
        push   ecx
        call   sub_3_495B
        jmp    SHORT loc_3_4B3E
loc_3_4B35:
        push   dword ptr [ebp + 0ch]
        push   ecx
        call   sub_3_4972
loc_3_4B3E:
        mov    esp, ebp
        pop    ebp
        ret
loc_3_4B42:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 3], 0
        jne    SHORT loc_3_4BA9
        mov    al, byte ptr [esi]
        test   al, al
        je     SHORT loc_3_4B61
        db  038h, 005h, 002h, 000h, 000h, 000h
        jae    SHORT loc_3_4BA9
loc_3_4B61:
        lea    eax, [ebp - 2]
        db  08Ah, 00Dh, 004h, 000h, 000h, 000h
        push   eax
        db  08Ah, 015h, 003h, 000h, 000h, 000h
        push   ecx
        db  0A0h, 002h, 000h, 000h, 000h
        push   edx
        mov    edi, dword ptr [ebp + 8]
        push   eax
        push   dword ptr [edi]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_4B83:
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_3_4C18
        db  0A0h, 002h, 000h, 000h, 000h
        mov    byte ptr [esi], al
        db  08Ah, 00Dh, 003h, 000h, 000h, 000h
        mov    byte ptr [esi + 1], cl
        db  08Ah, 015h, 004h, 000h, 000h, 000h
        mov    byte ptr [esi + 2], dl
        jmp    SHORT loc_3_4C18
loc_3_4BA9:
        db  0A0h, 002h, 000h, 000h, 000h
        cmp    byte ptr [esi], al
        jne    SHORT loc_3_4C18
        db  0A0h, 003h, 000h, 000h, 000h
        cmp    byte ptr [esi + 1], al
        jae    SHORT loc_3_4BE6
        lea    eax, [ebp - 2]
        db  08Ah, 00Dh, 003h, 000h, 000h, 000h
        push   eax
        mov    edi, dword ptr [ebp + 8]
        push   0
        push   ecx
        push   0
        push   dword ptr [edi]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_4BD5:
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_3_4BE9
        db  0A0h, 003h, 000h, 000h, 000h
        mov    byte ptr [esi + 1], al
        jmp    SHORT loc_3_4BE9
loc_3_4BE6:
        mov    edi, dword ptr [ebp + 8]
loc_3_4BE9:
        db  0A0h, 004h, 000h, 000h, 000h
        cmp    byte ptr [esi + 2], al
        jae    SHORT loc_3_4C18
        lea    eax, [ebp - 2]
        db  08Ah, 00Dh, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        push   0
        push   0
        push   dword ptr [edi]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_4C09:
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_3_4C18
        db  0A0h, 004h, 000h, 000h, 000h
        mov    byte ptr [esi + 2], al
loc_3_4C18:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_3_4C20:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [edx]
        push   ebx
        mov    byte ptr [ebp - 1], 0
        xor    bl, bl
        mov    eax, dword ptr [ecx + 18h]
        mov    cl, byte ptr [ecx + 30h]
        cmp    cl, bl
        je     SHORT loc_3_4C54
loc_3_4C3D:
        cmp    byte ptr [eax + 14h], 0
        jne    SHORT loc_3_4C4E
        inc    bl
        add    eax, 28h
        cmp    cl, bl
        ja     SHORT loc_3_4C3D
        jmp    SHORT loc_3_4C54
loc_3_4C4E:
        mov    al, byte ptr [eax + 14h]
        mov    byte ptr [ebp - 1], al
loc_3_4C54:
        mov    al, byte ptr [ebp - 1]
        lea    ecx, [ebp - 4]
        push   ecx
        mov    byte ptr [ebp - 4], al
        db  068h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 3], al
        push   edx
        mov    byte ptr [ebp - 2], al
        call   sub_3_4320
        add    esp, 0ch
        mov    cl, byte ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0ch]
        mov    bl, byte ptr [ebp - 3]
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [edx], cl
        mov    cl, byte ptr [ebp - 2]
        mov    edx, dword ptr [ebp + 14h]
        mov    byte ptr [eax], bl
        xor    eax, eax
        pop    ebx
        mov    byte ptr [edx], cl
        mov    esp, ebp
        pop    ebp
        ret
sub_3_4C91:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx]
        mov    ax, word ptr [eax + 10h]
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        mov    dl, byte ptr [ecx + 1fh]
        db  088h, 015h, 001h, 000h, 000h, 000h
        mov    dl, byte ptr [esp + 8]
        db  0C6h, 005h, 005h, 000h, 000h, 000h, 000h
        lea    eax, [edx - 0ffh]
        db  068h, 000h, 000h, 000h, 000h
        cmp    al, 1
        db  088h, 015h, 004h, 000h, 000h, 000h
        sbb    eax, eax
        neg    eax
        db  066h, 0A3h, 002h, 000h, 000h, 000h
        push   dword ptr [ecx]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_4CD9:
        add    esp, 8
        ret
sub_3_4CDD:
        mov    eax, dword ptr [esp + 4]
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [eax]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_4CED:
        add    esp, 8
        ret
sub_3_4CF1:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        db  08Ah, 015h, 004h, 000h, 000h, 000h
        db  066h, 08Bh, 00Dh, 002h, 000h, 000h, 000h
        db  088h, 015h, 004h, 000h, 000h, 000h
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 00Ch, 000h, 000h, 000h
        db  066h, 0A1h, 006h, 000h, 000h, 000h
        db  089h, 015h, 00Ch, 000h, 000h, 000h
        db  066h, 089h, 00Dh, 002h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  066h, 0A3h, 006h, 000h, 000h, 000h
        db  08Bh, 00Dh, 008h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 8]
        db  089h, 00Dh, 008h, 000h, 000h, 000h
        db  0C6h, 005h, 005h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 010h, 000h, 000h, 000h, 080h, 000h
        push   dword ptr [eax]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_4D5B:
        db  066h, 08Bh, 00Dh, 012h, 000h, 000h, 000h
        add    esp, 8
        db  066h, 039h, 00Dh, 010h, 000h, 000h, 000h
        jbe    SHORT loc_3_4D75
        db  066h, 089h, 00Dh, 010h, 000h, 000h, 000h
loc_3_4D75:
        ret
sub_3_4D76:
        mov    edx, 4
        mov    eax, dword ptr [esp + 4]
loc_3_4D7F:
        mov    cx, word ptr [esp + 8]
        shr    cx, 0ch
        shl    word ptr [esp + 8], 4
        movzx  ecx, cx
        db  08Ah, 089h, 000h, 000h, 000h, 000h
        mov    byte ptr [eax], cl
        inc    eax
        dec    edx
        jne    SHORT loc_3_4D7F
        ret
sub_3_4D9E:
        push   ebx
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        push   esi
        test   edx, edx
        push   edi
        je     SHORT loc_3_4DF8
        mov    ecx, dword ptr [esp + 10h]
loc_3_4DAF:
        mov    edi, dword ptr [edx + 10h]
        test   edi, edi
        je     SHORT loc_3_4DF2
loc_3_4DB6:
        mov    eax, dword ptr [edi + 2ch]
        test   eax, eax
        jne    SHORT loc_3_4DC4
        mov    eax, dword ptr [edi + 24h]
        test   eax, eax
        je     SHORT loc_3_4DEC
loc_3_4DC4:
        test   byte ptr [eax + 0bh], 3
        jne    SHORT loc_3_4DE6
        cmp    dword ptr [eax + 2ch], 0
        je     SHORT loc_3_4DE6
        cmp    ecx, eax
        je     SHORT loc_3_4DE6
        mov    si, word ptr [eax + 1ah]
        cmp    word ptr [ecx + 1ah], si
        jne    SHORT loc_3_4DE6
        mov    bl, byte ptr [eax + 30h]
        cmp    byte ptr [ecx + 30h], bl
        je     SHORT loc_3_4DFA
loc_3_4DE6:
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    SHORT loc_3_4DC4
loc_3_4DEC:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    SHORT loc_3_4DB6
loc_3_4DF2:
        mov    edx, dword ptr [edx]
        test   edx, edx
        jne    SHORT loc_3_4DAF
loc_3_4DF8:
        xor    eax, eax
loc_3_4DFA:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_4DFE:
        mov    ecx, dword ptr [esp + 8]
        push   ebx
        push   esi
        db  0B8h, 000h, 000h, 000h, 000h
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   edi
        push   ebp
        je     SHORT loc_3_4E1F
loc_3_4E14:
        mov    dl, byte ptr [eax]
        inc    eax
        mov    byte ptr [ecx], dl
        inc    ecx
        cmp    byte ptr [eax], 0
        jne    SHORT loc_3_4E14
loc_3_4E1F:
        mov    esi, dword ptr [esp + 14h]
        mov    ax, word ptr [esi + 1ah]
        push   eax
        push   ecx
        xor    ebx, ebx
        call   sub_3_4D76
        mov    edi, dword ptr [esp + 24h]
        mov    byte ptr [eax], 5ch
        mov    byte ptr [eax + 2], bl
        add    esp, 8
        mov    ebp, eax
        mov    dword ptr [edi], ebx
        inc    ebp
loc_3_4E42:
        lea    eax, [ebx + 30h]
        push   esi
        mov    byte ptr [ebp], al
        mov    byte ptr [esi + 30h], bl
        call   sub_3_4D9E
        add    esp, 4
        test   eax, eax
        je     SHORT loc_3_4E6F
        test   bl, bl
        jne    SHORT loc_3_4E5E
        mov    dword ptr [edi], eax
loc_3_4E5E:
        inc    bl
        cmp    bl, 0ah
        jl     SHORT loc_3_4E42
        mov    eax, 13h
loc_3_4E6A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_4E6F:
        xor    eax, eax
        jmp    SHORT loc_3_4E6A
sub_3_4E73:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [ebp + 8]
        cmp    dword ptr [edi + 2ch], esi
        je     NEAR PTR loc_3_4F7E
        push   esi
        mov    ebx, 80h
        push   ebx
        call   sub_3_53A0
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    eax, esi
        je     NEAR PTR loc_3_4F7E
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        mov    dword ptr [ebp - 8], ebx
        push   dword ptr [ebp - 4]
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        push   dword ptr [edi + 2ch]
        call   sub_3_5298
        add    esp, 1ch
        test   eax, eax
        jne    NEAR PTR loc_3_4F71
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        push   dword ptr [edx + 2ch]
        call   sub_3_5330
        add    esp, 1ch
        test   eax, eax
        jne    NEAR PTR loc_3_4F71
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        mov    dword ptr [ebp - 8], ebx
        push   dword ptr [ebp - 4]
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        push   dword ptr [edi + 2ch]
        call   sub_3_5298
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_3_4F71
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        push   dword ptr [edx + 2ch]
        call   sub_3_5330
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_3_4F71
        push   esi
        mov    dword ptr [ebp - 8], ebx
        lea    ebx, [ebp - 8]
        push   ebx
        push   dword ptr [ebp - 4]
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        push   dword ptr [edi + 2ch]
        call   sub_3_5298
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_3_4F71
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        push   dword ptr [edx + 2ch]
        call   sub_3_5330
        add    esp, 1ch
loc_3_4F71:
        push   0
        push   dword ptr [ebp - 4]
        call   sub_3_53A8
        add    esp, 8
loc_3_4F7E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_4F85:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 2ch]
        test   edi, edi
        jne    SHORT loc_3_4FA3
        mov    edi, dword ptr [esi + 24h]
        test   edi, edi
        je     NEAR PTR loc_3_50E9
loc_3_4FA3:
        mov    dword ptr [ebp - 10h], 10000h
        lea    eax, [ebp - 14h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   80000002h
        call   sub_3_5328
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_4FF2
        lea    eax, [ebp - 18h]
        lea    ecx, [ebp - 10h]
        mov    dword ptr [ebp - 18h], 4
        push   eax
        push   ecx
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 14h]
        call   sub_3_5320
        add    esp, 18h
        push   dword ptr [ebp - 14h]
        call   sub_3_5318
        add    esp, 4
loc_3_4FF2:
        test   edi, edi
        je     NEAR PTR loc_3_50EE
        xor    ebx, ebx
loc_3_4FFC:
        test   byte ptr [edi + 0bh], 3
        jne    NEAR PTR loc_3_50DD
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [edi + 10h], eax
        jb     NEAR PTR loc_3_50DD
        cmp    word ptr [edi + 1ah], bx
        jne    SHORT loc_3_5022
        cmp    byte ptr [edi + 0ah], 6
        jne    NEAR PTR loc_3_50DD
loc_3_5022:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 2ch]
        push   eax
        push   ecx
        push   edi
        call   sub_3_4DFE
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_3_50E9
        push   ebx
        lea    ecx, [ebp - 2ch]
        push   dword ptr [ebp + 0ch]
        push   ecx
        or     byte ptr [esi + 14h], 4
        lea    edx, [ebp - 4]
        push   edx
        call   sub_3_52E0
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_3_506A
        cmp    eax, 10h
        jne    NEAR PTR loc_3_50EE
        cmp    dword ptr [edi + 2ch], ebx
        jne    SHORT loc_3_50DD
        jmp    NEAR PTR loc_3_50EE
loc_3_506A:
        mov    eax, dword ptr [ebp - 4]
        push   ebx
        push   edi
        mov    dword ptr [edi + 2ch], eax
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp - 4]
        call   sub_3_5430
        mov    dx, word ptr [edi + 4]
        add    esp, 10h
        lea    ecx, [edi + 31h]
        mov    eax, dword ptr [edi + 28h]
        mov    word ptr [ecx], dx
        test   eax, eax
        je     SHORT loc_3_5096
        mov    eax, dword ptr [eax + 0ch]
        jmp    SHORT loc_3_5099
loc_3_5096:
        mov    eax, dword ptr [edi + 0ch]
loc_3_5099:
        push   ebx
        mov    dword ptr [edi + 33h], eax
        push   ecx
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 4]
        call   sub_3_5430
        mov    cx, word ptr [esi + 10h]
        add    esp, 10h
        mov    word ptr [ebp - 0ch], cx
        push   ebx
        mov    word ptr [ebp - 0ah], bx
        lea    ecx, [ebp - 0ch]
        push   ecx
        push   4
        push   8
        push   dword ptr [ebp - 4]
        call   sub_3_5428
        add    esp, 14h
        cmp    dword ptr [ebp - 8], ebx
        je     SHORT loc_3_50DD
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_3_4E73
        add    esp, 8
loc_3_50DD:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    NEAR PTR loc_3_4FFC
        jmp    SHORT loc_3_50EE
loc_3_50E9:
        mov    eax, 13h
loc_3_50EE:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_50F5:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 2ch]
        test   esi, esi
        jne    SHORT loc_3_5108
        mov    esi, dword ptr [eax + 24h]
        test   esi, esi
        je     SHORT loc_3_5120
loc_3_5108:
        mov    eax, dword ptr [esi + 2ch]
        test   eax, eax
        je     SHORT loc_3_511A
        push   0
        push   eax
        call   sub_3_52B8
        add    esp, 8
loc_3_511A:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    SHORT loc_3_5108
loc_3_5120:
        xor    eax, eax
        pop    esi
        ret
sub_3_5124:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        push   0
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        lea    eax, [ebp - 4]
        push   dword ptr [esi + 4]
        push   edi
        push   eax
        call   sub_3_5370
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_3_5179
        mov    eax, dword ptr [esi + 10h]
        test   eax, eax
        je     SHORT loc_3_5179
loc_3_514F:
        mov    ecx, dword ptr [eax + 2ch]
        test   ecx, ecx
        jne    SHORT loc_3_515D
        mov    ecx, dword ptr [eax + 24h]
        test   ecx, ecx
        je     SHORT loc_3_5173
loc_3_515D:
        test   byte ptr [ecx + 0bh], 3
        jne    SHORT loc_3_516D
        cmp    dword ptr [ecx + 2ch], edi
        jne    SHORT loc_3_516D
        cmp    dword ptr [ebp - 4], ecx
        je     SHORT loc_3_5181
loc_3_516D:
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    SHORT loc_3_515D
loc_3_5173:
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    SHORT loc_3_514F
loc_3_5179:
        xor    eax, eax
loc_3_517B:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_3_5181:
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_3_517B
        mov    eax, ecx
        jmp    SHORT loc_3_517B
sub_3_518B:
        push   1
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        call   sub_3_5124
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_51A8
        mov    eax, 13h
        jmp    SHORT loc_3_51B1
loc_3_51A8:
        mov    dword ptr [eax + 2ch], 0
        xor    eax, eax
loc_3_51B1:
        ret
sub_3_51B2:
        push   ebp
        mov    ebp, esp
        sub    esp, 80h
        push   ebx
        push   esi
        push   edi
        push   1
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_3_5124
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_3_526B
        db  0B9h, 000h, 000h, 000h, 000h
        lea    edx, [ebp - 80h]
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_3_51F9
loc_3_51E7:
        lea    esi, [ebp - 1]
        cmp    edx, esi
        jae    SHORT loc_3_51F9
        mov    bl, byte ptr [ecx]
        inc    ecx
        mov    byte ptr [edx], bl
        inc    edx
        cmp    byte ptr [ecx], 0
        jne    SHORT loc_3_51E7
loc_3_51F9:
        lea    ecx, [ebp - 80h]
        sub    ecx, edx
        add    ecx, 7fh
        cmp    ecx, 0dh
        jae    SHORT loc_3_520B
        mov    byte ptr [edx], 0
        jmp    SHORT loc_3_5241
loc_3_520B:
        db  0B9h, 000h, 000h, 000h, 000h
        mov    byte ptr [edx], 20h
        inc    edx
        mov    byte ptr [edx], 28h
        inc    edx
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_3_522C
loc_3_5221:
        mov    bl, byte ptr [ecx]
        inc    ecx
        mov    byte ptr [edx], bl
        inc    edx
        cmp    byte ptr [ecx], 0
        jne    SHORT loc_3_5221
loc_3_522C:
        mov    ax, word ptr [eax + 1ah]
        push   eax
        push   edx
        call   sub_3_4D76
        add    esp, 8
        mov    byte ptr [eax], 29h
        mov    byte ptr [eax + 1], 0
loc_3_5241:
        push   0
        lea    edi, [ebp - 80h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        lea    eax, [ebp - 80h]
        push   ecx
        push   eax
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   dword ptr [ebp + 0ch]
        call   sub_3_5330
        add    esp, 1ch
loc_3_526B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_5272:
        xor    ax, ax
        ret
        db  0CCh, 0CCh
sub_3_5278:
        db  0CDh, 020h, 032h, 080h, 033h, 000h
        int3
        int3
sub_3_5280:
        db  0CDh, 020h, 00Eh, 080h, 033h, 000h
        int3
        int3
sub_3_5288:
        db  0CDh, 020h, 00Fh, 080h, 033h, 000h
        int3
        int3
sub_3_5290:
        db  0CDh, 020h, 02Fh, 080h, 033h, 000h
        int3
        int3
sub_3_5298:
        db  0CDh, 020h, 03Eh, 080h, 033h, 000h
        int3
        int3
sub_3_52A0:
        db  0CDh, 020h, 054h, 080h, 033h, 000h
        int3
        int3
sub_3_52A8:
        db  0CDh, 020h, 044h, 080h, 033h, 000h
        int3
        int3
sub_3_52B0:
        db  0CDh, 020h, 053h, 080h, 033h, 000h
        int3
        int3
sub_3_52B8:
        db  0CDh, 020h, 00Dh, 080h, 033h, 000h
        int3
        int3
sub_3_52C0:
        db  0CDh, 020h, 065h, 080h, 033h, 000h
        int3
        int3
sub_3_52C8:
        db  0CDh, 020h, 033h, 080h, 033h, 000h
        int3
        int3
sub_3_52D0:
        db  0CDh, 020h, 00Ch, 080h, 033h, 000h
        int3
        int3
sub_3_52D8:
        db  0CDh, 020h, 003h, 080h, 033h, 000h
        int3
        int3
sub_3_52E0:
        db  0CDh, 020h, 00Bh, 080h, 033h, 000h
        int3
        int3
sub_3_52E8:
        db  0CDh, 020h, 019h, 080h, 033h, 000h
        int3
        int3
sub_3_52F0:
        db  0CDh, 020h, 01Bh, 080h, 033h, 000h
        int3
        int3
sub_3_52F8:
        db  0CDh, 020h, 01Fh, 080h, 033h, 000h
        int3
        int3
sub_3_5300:
        db  0CDh, 020h, 01Ah, 080h, 033h, 000h
        int3
        int3
sub_3_5308:
        db  0CDh, 020h, 015h, 080h, 033h, 000h
        int3
        int3
sub_3_5310:
        db  0CDh, 020h, 05Bh, 080h, 033h, 000h
        int3
        int3
sub_3_5318:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        int3
        int3
sub_3_5320:
        db  0CDh, 020h, 051h, 081h, 001h, 000h
        int3
        int3
sub_3_5328:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        int3
        int3
sub_3_5330:
        db  0CDh, 020h, 03Fh, 080h, 033h, 000h
        int3
        int3
sub_3_5338:
        db  0CDh, 020h, 018h, 080h, 017h, 000h
        int3
        int3
sub_3_5340:
        db  0CDh, 020h, 06Ch, 080h, 001h, 000h
        int3
        int3
sub_3_5348:
        db  0CDh, 020h, 01Ch, 080h, 033h, 000h
        int3
        int3
sub_3_5350:
        db  0CDh, 020h, 018h, 080h, 033h, 000h
        int3
        int3
sub_3_5358:
        db  0CDh, 020h, 002h, 080h, 033h, 000h
        int3
        int3
sub_3_5360:
        db  0CDh, 020h, 005h, 080h, 033h, 000h
        int3
        int3
sub_3_5368:
        db  0CDh, 020h, 004h, 080h, 033h, 000h
        int3
        int3
sub_3_5370:
        db  0CDh, 020h, 009h, 080h, 033h, 000h
        int3
        int3
sub_3_5378:
        db  0CDh, 020h, 02Dh, 080h, 033h, 000h
        int3
        int3
sub_3_5380:
        db  0CDh, 020h, 02Ch, 080h, 033h, 000h
        int3
        int3
sub_3_5388:
        db  0CDh, 020h, 013h, 080h, 033h, 000h
        int3
        int3
sub_3_5390:
        db  0CDh, 020h, 01Eh, 080h, 033h, 000h
        int3
        int3
sub_3_5398:
        db  0CDh, 020h, 036h, 080h, 033h, 000h
        int3
        int3
sub_3_53A0:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        int3
        int3
sub_3_53A8:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        int3
        int3
sub_3_53B0:
        push   esi
        mov    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        ret    0ch
        db  0CCh, 0CCh, 0CCh
sub_3_53CC:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        ret    4
        db  0CCh
sub_3_53DC:
        mov    eax, dword ptr [esp + 4]
        db  0CDh, 020h, 011h, 000h, 003h, 000h
        ret    4
        db  0CCh, 0CCh, 0CCh
sub_3_53EC:
        mov    eax, dword ptr [esp + 4]
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        ret    4
        db  0CCh, 0CCh, 0CCh
sub_3_53FC:
        mov    eax, dword ptr [esp + 4]
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        ret    4
        db  0CCh, 0CCh, 0CCh
sub_3_540C:
        push   edi
        mov    edi, dword ptr [esp + 8]
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        cmc
        sbb    ecx, ecx
        pop    edi
        and    eax, ecx
        ret    4
sub_3_5420:
        db  0CDh, 020h, 031h, 080h, 033h, 000h
        int3
        int3
sub_3_5428:
        db  0CDh, 020h, 043h, 080h, 033h, 000h
        int3
        int3
sub_3_5430:
        db  0CDh, 020h, 00Ah, 080h, 033h, 000h
        int3
        int3
sub_3_5438:
        db  0CDh, 020h, 055h, 080h, 033h, 000h
        int3
        int3
sub_3_5440:
        db  0CDh, 020h, 010h, 080h, 033h, 000h
_PTEXT ends
        end
