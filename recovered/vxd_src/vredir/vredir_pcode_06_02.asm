; vredir_pcode_06_02.asm -- VREDIR object 6 segment _PTEXT  (part 2/4)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_6_4350:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_6_D6D0
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_4370:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        push   4
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4388:
        add    esp, 8
        call   sub_6_34A0
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        test   esi, esi
        je     SHORT loc_6_43B3
loc_6_439A:
        push   esi
        call   sub_6_42C0
        add    esp, 4
        push   esi
        call   sub_6_4350
        add    esp, 4
        mov    esi, dword ptr [esi + 18h]
        test   esi, esi
        jne    SHORT loc_6_439A
loc_6_43B3:
        xor    eax, eax
        pop    esi
        db  0A2h, 000h, 000h, 000h, 000h
        db  0A2h, 000h, 000h, 000h, 000h
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_43D0:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   esi
        je     SHORT loc_6_43FE
        mov    esi, dword ptr [ebp + 8]
        push   0
        push   esi
        mov    byte ptr [esi + 6eh], 1
        mov    byte ptr [esi + 6dh], 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_43F0:
        add    esp, 8
        push   1
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_43FB:
        add    esp, 8
loc_6_43FE:
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_4410:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        pop    ebp
        mov    byte ptr [eax + 6eh], cl
        mov    byte ptr [eax + 6dh], cl
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_6_4430:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 4]
        test   eax, eax
        je     SHORT loc_6_4452
        cmp    eax, 1
        je     SHORT loc_6_4487
        cmp    eax, 2
        je     SHORT loc_6_44B7
        mov    eax, 1
        pop    esi
        pop    ebp
        ret
loc_6_4452:
        call   sub_6_AF00
        cmp    word ptr [esi + 18h], 1
        jne    SHORT loc_6_4482
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        test   esi, esi
        je     SHORT loc_6_4482
loc_6_4472:
        push   esi
        call   sub_6_43D0
        add    esp, 4
        mov    esi, dword ptr [esi + 18h]
        test   esi, esi
        jne    SHORT loc_6_4472
loc_6_4482:
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
loc_6_4487:
        cmp    word ptr [esi + 18h], 0
        jne    SHORT loc_6_44B2
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        test   esi, esi
        je     SHORT loc_6_44B2
loc_6_44A2:
        push   esi
        call   sub_6_4410
        add    esp, 4
        mov    esi, dword ptr [esi + 18h]
        test   esi, esi
        jne    SHORT loc_6_44A2
loc_6_44B2:
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
loc_6_44B7:
        push   0
        push   0
        push   esi
        call   sub_6_4670
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_6_44CF
        movsx  eax, word ptr [esi + 1ah]
        pop    esi
        pop    ebp
        ret
loc_6_44CF:
        mov    eax, dword ptr [esi + 24h]
        push   esi
        mov    dword ptr [esi], eax
        call   sub_6_44E0
        add    esp, 4
        pop    esi
        pop    ebp
        ret
sub_6_44E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    word ptr [ebp - 2], bx
        push   26h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_44F6:
        add    esp, 4
        mov    esi, eax
        cmp    esi, ebx
        jne    SHORT loc_6_4514
        mov    edi, dword ptr [ebp + 8]
        mov    eax, 8
        mov    word ptr [edi + 1ah], 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_4514:
        mov    edi, esi
        xor    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [esi], edi
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 8], eax
        mov    ax, word ptr [edi]
        mov    word ptr [esi + 18h], ax
        test   ax, ax
        je     SHORT loc_6_4566
        push   40h
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4540:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_6_455B
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_454D:
        add    esp, 4
        movsx  eax, word ptr [edi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_455B:
        mov    ebx, 1
        mov    eax, dword ptr [edi + 50h]
        mov    dword ptr [esi + 0ch], eax
loc_6_4566:
        lea    eax, [ebp - 2]
        mov    ecx, dword ptr [edi + 0ch]
        push   eax
        push   ecx
        push   2
        xor    eax, eax
        mov    ax, word ptr [edi + 18h]
        mov    ecx, dword ptr [edi + 28h]
        push   eax
        push   1
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        push   2
        push   esi
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_458B:
        add    esp, 28h
        cmp    word ptr [edi + 1ah], 0
        jne    SHORT loc_6_459D
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [edi + 1ah], ax
loc_6_459D:
        test   ebx, ebx
        je     SHORT loc_6_45B1
        mov    eax, dword ptr [esi + 0ch]
        test   eax, eax
        je     SHORT loc_6_45B1
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_45AE:
        add    esp, 4
loc_6_45B1:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_45B7:
        add    esp, 4
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
VREDIR_Service_12:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   27ch
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_45E3:
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_6_45F8
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_45F8:
        lea    edi, [esi + 74h]
        mov    eax, dword ptr [ebp + 8]
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4605:
        add    esp, 8
        lea    eax, [ebp - 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        xor    ebx, ebx
        push   ebx
        mov    dword ptr [esi + 10h], eax
        mov    al, byte ptr [ebp + 18h]
        mov    dword ptr [esi + 0ch], edi
        push   eax
        mov    dword ptr [esi], ecx
        mov    byte ptr [esi + 4], 2
        push   esi
        call   sub_6_4670
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_6_4652
        mov    word ptr [esi + 18h], bx
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [esi + 5], bl
        push   esi
        mov    dword ptr [esi + 28h], ebx
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 14h], ecx
        mov    word ptr [esi + 6], 0ffh
        call   sub_6_44E0
        add    esp, 4
loc_6_4652:
        movsx  edi, word ptr [esi + 1ah]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_465C:
        add    esp, 4
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_6_4670:
        push   ebp
        mov    ebp, esp
        sub    esp, 38h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 2ch]
        mov    eax, dword ptr [ebx + 10h]
        mov    ecx, eax
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp + 10h], 0
        mov    eax, 20202020h
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx + 0ch], eax
        jne    NEAR PTR loc_6_473C
        mov    esi, dword ptr [ebx + 0ch]
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        sub    edx, 4
        cmp    edx, 22h
        jbe    SHORT loc_6_46C4
        mov    eax, 35h
        pop    edi
        mov    word ptr [ebx + 1ah], 35h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_46C4:
        add    esi, 8
        lea    edi, [ebp - 28h]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        shr    edx, 1
        and    ecx, 3
        rep movsb
        cmp    edx, 2
        jbe    SHORT loc_6_4707
        lea    ecx, [ebp + edx*2 - 2ah]
        cmp    word ptr [ecx], 2ah
        jne    SHORT loc_6_4707
        lea    eax, [ebp + edx*2 - 2ch]
        cmp    word ptr [eax], 2ah
        jne    SHORT loc_6_4707
        mov    word ptr [ecx], 30h
        sub    edx, 2
        mov    byte ptr [ebp + 0ch], 1bh
        mov    word ptr [eax], 30h
        jmp    SHORT loc_6_4720
loc_6_4707:
        cmp    edx, 1
        jbe    SHORT loc_6_4720
        lea    eax, [ebp + edx*2 - 2ah]
        cmp    word ptr [eax], 2ah
        jne    SHORT loc_6_4720
        mov    word ptr [eax], 30h
        dec    edx
        mov    byte ptr [ebp + 0ch], 1ch
loc_6_4720:
        push   1
        add    edx, edx
        push   0fh
        lea    eax, [ebp - 28h]
        push   edx
        lea    ecx, [ebp - 38h]
        push   eax
        lea    esi, [ebp - 38h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4737:
        add    esp, 14h
        mov    dword ptr [ebx], eax
loc_6_473C:
        mov    edx, dword ptr [ebx]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, 20202020h
        cmp    edx, 1
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx + 0ch], eax
        jne    SHORT loc_6_477F
        cmp    byte ptr [esi], 2ah
        jne    SHORT loc_6_477F
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        mov    edi, esi
        sub    eax, eax
        repne scasb
        not    ecx
        mov    edi, dword ptr [ebp - 4]
        lea    eax, [ecx - 1]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        jmp    SHORT loc_6_478B
loc_6_477F:
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
loc_6_478B:
        and    ecx, 3
        rep movsb
        mov    cl, byte ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp + 10h], 0
        mov    byte ptr [eax + 0fh], cl
        jne    SHORT loc_6_47C3
        mov    eax, dword ptr [ebx + 0ch]
        xor    ecx, ecx
        mov    cx, word ptr [eax + 4]
        xor    edx, edx
        mov    dx, word ptr [eax]
        xor    esi, esi
        mov    si, word ptr [eax + 2]
        sub    edx, ecx
        sub    esi, ecx
        add    eax, ecx
        mov    word ptr [eax], dx
        mov    dword ptr [ebx + 0ch], eax
        mov    word ptr [eax + 2], si
loc_6_47C3:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_6_47D0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 10h]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jl     SHORT loc_6_47F0
        mov    word ptr [ecx + 1ah], ax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_47ED:
        add    esp, 4
loc_6_47F0:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_47F6:
        add    esp, 4
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_6_4800:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 10h]
        mov    edx, dword ptr [ecx + 4]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4813:
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_6_4820:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 10h]
        mov    ecx, dword ptr [eax]
        cmp    dword ptr [ecx + 30h], 0
        je     SHORT loc_6_484C
        mov    eax, dword ptr [eax + 8]
        inc    eax
        push   eax
        push   ecx
        call   sub_6_48A0
        add    esp, 8
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4846:
        add    esp, 4
        pop    esi
        pop    ebp
        ret
loc_6_484C:
        mov    eax, dword ptr [eax + 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4855:
        add    esp, 4
        pop    esi
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_6_4860:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [eax + 10h]
        test   byte ptr [ecx + 1ah], 80h
        jne    SHORT loc_6_488F
        cmp    dword ptr [esi + 30h], 0
        je     SHORT loc_6_488F
        push   61h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4881:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_6_488F
        movsx  eax, word ptr [esi + 1ah]
        pop    esi
        pop    ebp
        ret
loc_6_488F:
        push   0
        push   esi
        call   sub_6_48A0
        add    esp, 8
        pop    esi
        pop    ebp
        ret
        db  08Dh, 049h, 000h
sub_6_48A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi]
        mov    ecx, dword ptr [esi + 1ch]
        mov    dword ptr [esi + 3ch], ebx
        mov    eax, dword ptr [ecx + 10h]
        or     byte ptr [ecx + 1ch], 10h
        test   byte ptr [eax + 1ah], 80h
        jne    NEAR PTR loc_6_4A47
        xor    edi, edi
        mov    dword ptr [ebp - 8], edi
loc_6_48CA:
        mov    byte ptr [esi + 4], 23h
        cmp    dword ptr [esi + 30h], 0
        mov    dword ptr [esi], ebx
        je     SHORT loc_6_48DA
        push   0
        jmp    SHORT loc_6_48DC
loc_6_48DA:
        push   1
loc_6_48DC:
        push   esi
        call   sub_6_5380
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [esi + 24h], 0
        jne    NEAR PTR loc_6_49A0
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_6_49A0
        test   edi, edi
        jne    SHORT loc_6_4933
        push   22h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4907:
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_6_4968
        mov    dword ptr [edi + 1ah], edi
        lea    eax, [edi + 16h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [edi + 10h], eax
        mov    dword ptr [eax], esi
        mov    dword ptr [eax + 8], ecx
        db  0C7h, 047h, 00Ch, 000h, 000h, 000h, 000h
        mov    byte ptr [edi + 14h], 0
        mov    dword ptr [edi + 4], 0
loc_6_4933:
        cmp    dword ptr [esi + 30h], 0
        jne    SHORT loc_6_4974
        mov    eax, 3ch
        cmp    eax, dword ptr [ebp - 8]
        sbb    eax, eax
        and    eax, 7148h
        add    eax, 3e8h
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4954:
        add    esp, 8
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_495D:
        add    esp, 4
        inc    dword ptr [ebp - 8]
        jmp    NEAR PTR loc_6_48CA
loc_6_4968:
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_4974:
        mov    eax, 3ch
        mov    dword ptr [esi], ebx
        cmp    eax, dword ptr [ebp + 0ch]
        sbb    eax, eax
        and    eax, 7148h
        add    eax, 3e8h
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4991:
        add    esp, 8
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_49A0:
        test   edi, edi
        je     SHORT loc_6_49AD
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_49AA:
        add    esp, 4
loc_6_49AD:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_6_49C5
        mov    ax, word ptr [ebp - 4]
        pop    edi
        mov    word ptr [esi + 1ah], ax
        pop    esi
        pop    ebx
        mov    esp, ebp
        movsx  eax, ax
        pop    ebp
        ret
loc_6_49C5:
        mov    byte ptr [esi + 4], 36h
        cmp    dword ptr [esi + 30h], 0
        mov    dword ptr [esi], ebx
        je     SHORT loc_6_49D5
        push   0
        jmp    SHORT loc_6_49D7
loc_6_49D5:
        push   1
loc_6_49D7:
        push   esi
        call   sub_6_5380
        add    esp, 8
        mov    edi, eax
        cmp    dword ptr [esi + 30h], 0
        je     SHORT loc_6_4A3E
        push   1eh
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_49EF:
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_6_4A02
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_4A02:
        mov    dword ptr [eax + 1ah], edi
        lea    ecx, [eax + 16h]
        db  0C7h, 040h, 00Ch, 000h, 000h, 000h, 000h
        push   0ah
        mov    dword ptr [eax + 10h], ecx
        push   eax
        mov    dword ptr [ecx], esi
        mov    word ptr [esi + 1ah], 0ffffh
        xor    ecx, ecx
        mov    byte ptr [eax + 14h], cl
        mov    dword ptr [eax + 4], ecx
        mov    edi, 0ffffffffh
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4A2F:
        add    esp, 8
        mov    ecx, dword ptr [esi + 50h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4A3B:
        add    esp, 4
loc_6_4A3E:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_4A47:
        cmp    dword ptr [esi + 30h], 0
        je     SHORT loc_6_4A6C
        xor    eax, eax
        mov    ax, word ptr [ecx + 20h]
        sub    eax, 3eh
        cmp    eax, ebx
        jae    SHORT loc_6_4A6C
        mov    eax, 7ah
        pop    edi
        mov    word ptr [esi + 1ah], 7ah
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_4A6C:
        push   0a1h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4A77:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_6_4A89
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_4A89:
        mov    byte ptr [esi + 4], 0
        mov    word ptr [esi + 1ah], 0ffffh
        push   esi
        call   sub_6_4AD0
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jl     SHORT loc_6_4AC1
        mov    eax, dword ptr [esi + 50h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4AAB:
        mov    word ptr [esi + 1ah], di
        add    esp, 4
        mov    eax, dword ptr [esi + 3ch]
        sub    dword ptr [esi], eax
        movsx  eax, di
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_4AC1:
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
sub_6_4AD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ecx, dword ptr [esi + 1ch]
        mov    ax, word ptr [ecx + 20h]
        mov    edi, dword ptr [esi + 20h]
        sub    eax, 3ch
        push   esi
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 4], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4AF7:
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_6_4B07
        mov    byte ptr [esi + 4], 0
        mov    ebx, dword ptr [ebp - 4]
loc_6_4B07:
        mov    eax, dword ptr [esi + 3ch]
        cmp    ebx, eax
        jbe    SHORT loc_6_4B10
        mov    ebx, eax
loc_6_4B10:
        cmp    byte ptr [esi + 4], 0
        mov    dword ptr [esi + 38h], ebx
        je     SHORT loc_6_4B56
        cmp    ebx, 33h
        jb     SHORT loc_6_4B56
        test   byte ptr [edi + 25h], 1
        je     SHORT loc_6_4B29
        cmp    dword ptr [edi + 20h], eax
        ja     SHORT loc_6_4B56
loc_6_4B29:
        mov    eax, dword ptr [esi + 50h]
        push   eax
        push   ebx
        push   ebx
        xor    eax, eax
        mov    ax, word ptr [edi + 35h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   0a1ah
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4B4B:
        add    esp, 20h
        test   eax, eax
        je     NEAR PTR loc_6_4BDC
loc_6_4B56:
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [esi + 1ah], 0ffffh
        mov    eax, dword ptr [ecx + 10h]
        mov    al, byte ptr [eax + 1ah]
        and    al, 1
        cmp    ebx, dword ptr [ebp - 4]
        mov    byte ptr [esi + 4], al
        jbe    SHORT loc_6_4B72
        mov    ebx, dword ptr [ebp - 4]
loc_6_4B72:
        mov    ecx, 82eh
        mov    eax, dword ptr [esi + 3ch]
        mov    dword ptr [esi + 38h], ebx
        cmp    dword ptr [esi], eax
        je     SHORT loc_6_4BA1
        test   byte ptr [edi + 25h], 1
        jne    SHORT loc_6_4BA1
        mov    eax, dword ptr [edi + 1ch]
        test   eax, eax
        jne    SHORT loc_6_4B96
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_6_4B98
loc_6_4B96:
        mov    edx, dword ptr [eax]
loc_6_4B98:
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_6_4BAE
loc_6_4BA1:
        mov    edx, 0ffffffffh
        mov    ecx, 282eh
        mov    dword ptr [ebp - 4], ebx
loc_6_4BAE:
        mov    eax, dword ptr [esi + 50h]
        push   eax
        push   ebx
        mov    eax, dword ptr [esi + 3ch]
        push   eax
        push   edx
        mov    edx, dword ptr [ebp - 4]
        xor    eax, eax
        mov    ax, word ptr [edi + 35h]
        push   edx
        push   ebx
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4BD5:
        add    esp, 2ch
        test   eax, eax
        jne    SHORT loc_6_4BE1
loc_6_4BDC:
        mov    eax, 0ffffffffh
loc_6_4BE1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_6_4BF0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        mov    cl, byte ptr [eax + 40h]
        push   edi
        mov    esi, dword ptr [eax + 1ch]
        mov    edi, dword ptr [eax + 20h]
        sub    cl, 1ah
        push   eax
        cmp    cl, 1
        sbb    bl, bl
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4C11:
        add    esp, 4
        mov    edx, dword ptr [esi + 34h]
        neg    bl
        mov    eax, dword ptr [esi + 3ch]
        sub    eax, edx
        mov    dword ptr [esi + 3ch], eax
        cmp    eax, 1
        sbb    al, al
        neg    al
        test   al, al
        jne    NEAR PTR loc_6_4CB4
        test   bl, bl
        je     SHORT loc_6_4C4F
        test   edx, edx
        jne    SHORT loc_6_4C3E
        mov    byte ptr [esi + 4], 0
        jmp    SHORT loc_6_4C71
loc_6_4C3E:
        mov    ecx, dword ptr [esi + 20h]
        cmp    dword ptr [esi + 38h], edx
        mov    dword ptr [ecx + 20h], 0
        je     SHORT loc_6_4C71
        jmp    SHORT loc_6_4C6F
loc_6_4C4F:
        mov    ecx, dword ptr [esi + 20h]
        test   byte ptr [ecx + 25h], 1
        je     SHORT loc_6_4C64
        cmp    edi, 0eah
        jne    SHORT loc_6_4C6F
        xor    edi, edi
        jmp    SHORT loc_6_4C71
loc_6_4C64:
        cmp    dword ptr [esi + 38h], edx
        jne    SHORT loc_6_4C6F
        cmp    dword ptr [ecx + 20h], 0
        jne    SHORT loc_6_4C71
loc_6_4C6F:
        inc    al
loc_6_4C71:
        test   al, al
        jne    SHORT loc_6_4CB4
        test   edi, edi
        jne    SHORT loc_6_4CB4
        push   edx
        mov    eax, dword ptr [esi + 50h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4C83:
        add    esp, 8
        push   esi
        call   sub_6_4AD0
        add    esp, 4
        test   eax, eax
        jl     SHORT loc_6_4CAD
        mov    eax, dword ptr [esi + 50h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4C9C:
        add    esp, 4
        mov    eax, dword ptr [esi + 3ch]
        sub    dword ptr [esi], eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4CAA:
        add    esp, 4
loc_6_4CAD:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_4CB4:
        mov    word ptr [esi + 1ah], di
        mov    eax, dword ptr [esi + 50h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4CC1:
        add    esp, 4
        mov    eax, dword ptr [esi + 3ch]
        sub    dword ptr [esi], eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4CCF:
        add    esp, 4
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_6_4CE0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 1ch]
        mov    eax, dword ptr [ecx + 10h]
        or     byte ptr [ecx + 1ch], 10h
        mov    ax, word ptr [eax + 1ah]
        test   al, 80h
        jne    SHORT loc_6_4D5C
        mov    byte ptr [esi + 4], 37h
        push   esi
        call   sub_6_5360
        add    esp, 4
        mov    edi, eax
        cmp    dword ptr [esi + 30h], 0
        je     SHORT loc_6_4D56
        push   1eh
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4D16:
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_6_4D26
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_6_4D26:
        mov    dword ptr [eax + 1ah], edi
        lea    ecx, [eax + 16h]
        push   0ah
        mov    dword ptr [eax + 10h], ecx
        push   eax
        mov    dword ptr [ecx], esi
        db  0C7h, 040h, 00Ch, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        mov    edi, 0ffffffffh
        mov    byte ptr [eax + 14h], cl
        mov    dword ptr [eax + 4], ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4D4D:
        mov    word ptr [esi + 1ah], 0ffffh
        add    esp, 8
loc_6_4D56:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_6_4D5C:
        and    al, 2
        shr    al, 1
        cmp    dword ptr [esi + 30h], 0
        mov    byte ptr [esi + 4], al
        je     SHORT loc_6_4D86
        xor    eax, eax
        mov    ax, word ptr [ecx + 22h]
        sub    eax, 3eh
        cmp    eax, dword ptr [esi]
        jae    SHORT loc_6_4D86
        mov    eax, 7ah
        pop    edi
        mov    word ptr [esi + 1ah], 7ah
        pop    esi
        pop    ebp
        ret
loc_6_4D86:
        mov    eax, dword ptr [esi]
        push   0a0h
        push   esi
        mov    dword ptr [esi + 3ch], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4D96:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_6_4DA5
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_6_4DA5:
        mov    word ptr [esi + 1ah], 0ffffh
        push   esi
        call   sub_6_4DE0
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jl     SHORT loc_6_4DD6
        mov    eax, dword ptr [esi + 50h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4DC3:
        mov    word ptr [esi + 1ah], di
        add    esp, 4
        mov    eax, dword ptr [esi + 3ch]
        sub    dword ptr [esi], eax
        movsx  eax, di
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_6_4DD6:
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  090h
sub_6_4DE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [ebp - 8], 0
        push   ebx
        xor    ecx, ecx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    edi, dword ptr [esi + 20h]
        mov    eax, dword ptr [esi + 1ch]
        mov    cx, word ptr [eax + 22h]
        sub    ecx, 3ch
        mov    dword ptr [ebp - 4], ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4E0B:
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_6_4E19
        mov    ebx, 100h
loc_6_4E19:
        mov    eax, dword ptr [esi + 3ch]
        cmp    eax, ebx
        jbe    SHORT loc_6_4E2C
        test   byte ptr [edi + 25h], 4
        je     SHORT loc_6_4E2E
        mov    byte ptr [esi + 4], 0
        jmp    SHORT loc_6_4E2E
loc_6_4E2C:
        mov    ebx, eax
loc_6_4E2E:
        mov    dword ptr [esi + 34h], ebx
        cmp    dword ptr [ebp - 4], ebx
        jae    SHORT loc_6_4E68
        cmp    byte ptr [esi + 4], 0
        je     SHORT loc_6_4E68
        mov    eax, dword ptr [esi + 50h]
        push   eax
        push   ebx
        push   0
        xor    eax, eax
        mov    ax, word ptr [edi + 35h]
        push   ebx
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   2a1dh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4E60:
        add    esp, 24h
        cmp    eax, -3
        jne    SHORT loc_6_4ED8
loc_6_4E68:
        mov    byte ptr [esi + 4], 0
        cmp    dword ptr [ebp - 4], ebx
        jae    SHORT loc_6_4E74
        mov    ebx, dword ptr [ebp - 4]
loc_6_4E74:
        xor    eax, eax
        test   byte ptr [edi + 25h], 4
        je     SHORT loc_6_4EA8
        mov    edx, dword ptr [esi]
        cmp    edx, ebx
        je     SHORT loc_6_4EA8
        mov    dword ptr [ebp - 8], 4
        mov    ecx, dword ptr [esi + 3ch]
        cmp    ecx, edx
        jne    SHORT loc_6_4EA8
        mov    eax, ecx
        add    ebx, 2
        mov    dword ptr [ebp - 8], 0ch
        cmp    dword ptr [ebp - 4], ebx
        jae    SHORT loc_6_4EA4
        mov    ebx, dword ptr [ebp - 4]
loc_6_4EA4:
        mov    byte ptr [esi + 4], 2
loc_6_4EA8:
        push   eax
        mov    ecx, dword ptr [esi + 3ch]
        mov    eax, dword ptr [esi + 50h]
        mov    edx, dword ptr [ebp - 8]
        push   eax
        mov    dword ptr [esi + 38h], ebx
        push   ebx
        xor    eax, eax
        mov    ax, word ptr [edi + 35h]
        push   ecx
        push   edx
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   282fh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4ED5:
        add    esp, 28h
loc_6_4ED8:
        test   eax, eax
        jne    SHORT loc_6_4EE1
        mov    eax, 0ffffffffh
loc_6_4EE1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_6_4EF0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 1ch]
        mov    edi, dword ptr [eax + 20h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4F05:
        add    esp, 4
        mov    cl, byte ptr [esi + 4]
        cmp    cl, 1
        jne    SHORT loc_6_4F59
        cmp    edi, 47h
        jg     SHORT loc_6_4F21
        je     SHORT loc_6_4F48
        cmp    edi, -3
        je     SHORT loc_6_4F36
        mov    edx, dword ptr [esi + 34h]
        jmp    SHORT loc_6_4F6A
loc_6_4F21:
        cmp    edi, 0fah
        jl     SHORT loc_6_4F43
        cmp    edi, 0fch
        jle    SHORT loc_6_4F3D
        mov    edx, dword ptr [esi + 34h]
        jmp    SHORT loc_6_4F6A
loc_6_4F36:
        mov    dword ptr [esi + 34h], 0
loc_6_4F3D:
        mov    byte ptr [esi + 4], 0
        xor    edi, edi
loc_6_4F43:
        mov    edx, dword ptr [esi + 34h]
        jmp    SHORT loc_6_4F6A
loc_6_4F48:
        cmp    dword ptr [esi + 34h], 0
        jne    SHORT loc_6_4F52
        mov    byte ptr [esi + 4], 0
loc_6_4F52:
        xor    edi, edi
        mov    edx, dword ptr [esi + 34h]
        jmp    SHORT loc_6_4F6A
loc_6_4F59:
        mov    edx, dword ptr [esi + 38h]
        xor    eax, eax
        mov    byte ptr [esi + 4], 0
        mov    al, cl
        mov    dword ptr [esi + 38h], eax
        add    dword ptr [esi + 3ch], eax
loc_6_4F6A:
        test   edi, edi
        jne    SHORT loc_6_4FBC
        mov    ecx, dword ptr [esi + 34h]
        mov    eax, dword ptr [esi + 3ch]
        sub    eax, ecx
        mov    dword ptr [esi + 3ch], eax
        je     SHORT loc_6_4FBC
        cmp    edx, ecx
        jne    SHORT loc_6_4FBC
        sub    ecx, dword ptr [esi + 38h]
        mov    eax, dword ptr [esi + 50h]
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4F8C:
        add    esp, 8
        push   esi
        call   sub_6_4DE0
        add    esp, 4
        test   eax, eax
        jl     SHORT loc_6_4FB6
        mov    eax, dword ptr [esi + 50h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4FA5:
        add    esp, 4
        mov    eax, dword ptr [esi + 3ch]
        sub    dword ptr [esi], eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4FB3:
        add    esp, 4
loc_6_4FB6:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_6_4FBC:
        mov    eax, dword ptr [esi + 50h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4FC5:
        mov    word ptr [esi + 1ah], di
        add    esp, 4
        mov    eax, dword ptr [esi + 3ch]
        push   esi
        sub    dword ptr [esi], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4FD7:
        add    esp, 4
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_6_4FE0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    word ptr [esi + 18h], 1
        mov    edi, dword ptr [esi + 20h]
        je     SHORT loc_6_5003
        mov    eax, 7ch
        pop    edi
        mov    word ptr [esi + 1ah], 7ch
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_5003:
        xor    eax, eax
        mov    al, byte ptr [esi + 4]
        cmp    eax, 3
        ja     SHORT loc_6_5014
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_6_5014:
        mov    eax, 57h
        pop    edi
        mov    word ptr [esi + 1ah], 57h
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_5024:
        mov    eax, dword ptr [esi]
        mov    dword ptr [esi], 6
        cmp    eax, 6
        jae    SHORT loc_6_5041
        mov    eax, 84bh
        pop    edi
        mov    word ptr [esi + 1ah], 84bh
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_5041:
        mov    eax, dword ptr [edi + 1ch]
        test   eax, eax
        jne    SHORT loc_6_504D
        db  0B8h, 000h, 000h, 000h, 000h
loc_6_504D:
        push   6
        push   eax
        mov    eax, dword ptr [esi + 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5059:
        add    esp, 0ch
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_5063:
        mov    ebx, 6
        jmp    SHORT loc_6_5076
loc_6_506A:
        mov    ebx, 4
        jmp    SHORT loc_6_5076
loc_6_5071:
        mov    ebx, 2
loc_6_5076:
        cmp    dword ptr [esi], ebx
        jae    SHORT loc_6_508A
        mov    eax, 84bh
        pop    edi
        mov    word ptr [esi + 1ah], 84bh
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_508A:
        mov    eax, dword ptr [edi + 1ch]
        test   eax, eax
        jne    SHORT loc_6_50C7
        push   6
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5098:
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_6_50AF
        mov    eax, 8
        pop    edi
        mov    word ptr [esi + 1ah], 8
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_50AF:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  0B9h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax], edx
        mov    cx, word ptr [ecx + 4]
        mov    word ptr [eax + 4], cx
        mov    dword ptr [edi + 1ch], eax
loc_6_50C7:
        push   ebx
        mov    ecx, dword ptr [esi + 14h]
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_50D2:
        add    esp, 0ch
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  08Dh, 064h, 024h, 000h
sub_6_50F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 24h]
        mov    ecx, dword ptr [esi + 10h]
        mov    edi, dword ptr [esi]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [ebp - 0ch], ecx
        mov    ecx, dword ptr [esi + 1ch]
        push   esi
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [esi + 3ch], edi
        or     byte ptr [ecx + 1ch], 10h
        mov    byte ptr [esi + 4], 21h
        call   sub_6_5360
        add    esp, 4
        test   byte ptr [esi + 19h], 1
        jne    SHORT loc_6_513F
        mov    eax, 0e6h
        pop    edi
        mov    word ptr [esi + 1ah], 0e6h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_513F:
        mov    byte ptr [esi + 4], 23h
        push   esi
        call   sub_6_5360
        add    esp, 4
        test   eax, eax
        je     SHORT loc_6_515E
        mov    word ptr [esi + 1ah], ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        movsx  eax, ax
        pop    ebp
        ret
loc_6_515E:
        cmp    dword ptr [esi + 24h], 0
        je     SHORT loc_6_5176
        mov    eax, 0e7h
        pop    edi
        mov    word ptr [esi + 1ah], 0e7h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_5176:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 0ch]
        push   esi
        mov    dword ptr [esi], eax
        mov    byte ptr [esi + 4], 37h
        mov    dword ptr [esi + 14h], ecx
        call   sub_6_5360
        add    esp, 4
        test   eax, eax
        je     SHORT loc_6_51A0
        mov    word ptr [esi + 1ah], ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        movsx  eax, ax
        pop    ebp
        ret
loc_6_51A0:
        xor    ebx, ebx
        mov    dword ptr [ebp - 14h], ebx
loc_6_51A5:
        mov    eax, dword ptr [ebp - 4]
        push   esi
        mov    dword ptr [esi], edi
        mov    dword ptr [esi + 14h], eax
        mov    byte ptr [esi + 4], 23h
        call   sub_6_5360
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [esi + 24h], 0
        jne    SHORT loc_6_5230
        test   eax, eax
        jne    SHORT loc_6_5230
        test   ebx, ebx
        jne    SHORT loc_6_51F8
        push   1eh
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_51D2:
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_6_5224
        mov    dword ptr [ebx + 1ah], ebx
        lea    eax, [ebx + 16h]
        db  0C7h, 043h, 00Ch, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 10h], eax
        mov    dword ptr [eax], esi
        mov    byte ptr [ebx + 14h], 0
        mov    dword ptr [ebx + 4], 0
loc_6_51F8:
        mov    eax, 3ch
        cmp    eax, dword ptr [ebp - 14h]
        sbb    eax, eax
        and    eax, 7148h
        add    eax, 3e8h
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5213:
        add    esp, 8
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_521C:
        add    esp, 4
        inc    dword ptr [ebp - 14h]
        jmp    SHORT loc_6_51A5
loc_6_5224:
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_5230:
        test   ebx, ebx
        je     SHORT loc_6_523D
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_523A:
        add    esp, 4
loc_6_523D:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_6_5255
        mov    ax, word ptr [ebp - 8]
        pop    edi
        mov    word ptr [esi + 1ah], ax
        pop    esi
        pop    ebx
        mov    esp, ebp
        movsx  eax, ax
        pop    ebp
        ret
loc_6_5255:
        mov    eax, dword ptr [ebp - 4]
        push   esi
        mov    dword ptr [esi], edi
        mov    dword ptr [esi + 14h], eax
        mov    byte ptr [esi + 4], 36h
        call   sub_6_5360
        mov    word ptr [esi + 1ah], ax
        add    esp, 4
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp - 4]
        pop    edi
        mov    dword ptr [esi + 24h], eax
        mov    dword ptr [esi + 10h], ecx
        mov    byte ptr [esi + 4], 26h
        movsx  eax, word ptr [esi + 1ah]
        mov    dword ptr [esi + 14h], edx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
sub_6_5290:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 24h]
        mov    ecx, dword ptr [edi + 10h]
        mov    edx, dword ptr [edi]
        mov    esi, dword ptr [edi + 14h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 10h], ecx
        mov    ax, word ptr [edi + 18h]
        mov    ecx, dword ptr [edi + 28h]
        mov    word ptr [ebp - 2], ax
        push   edi
        mov    dword ptr [ebp - 0ch], edx
        mov    dword ptr [ebp - 8], ecx
        mov    byte ptr [edi + 4], 53h
        call   sub_6_5360
        add    esp, 4
        test   eax, eax
        je     SHORT loc_6_52DD
        mov    word ptr [edi + 1ah], ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        movsx  eax, ax
        pop    ebp
        ret
loc_6_52DD:
        mov    dword ptr [edi + 24h], 0
        mov    dword ptr [edi + 14h], 0
        mov    dword ptr [edi + 28h], 0
        push   edi
        mov    dword ptr [edi], 0
        mov    byte ptr [edi + 4], 42h
        mov    dword ptr [edi + 10h], 0
        mov    word ptr [edi + 18h], 0
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_530F:
        add    esp, 4
        mov    ebx, eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp - 0ch]
        mov    dword ptr [edi + 24h], eax
        mov    ax, word ptr [ebp - 2]
        mov    dword ptr [edi + 10h], ecx
        mov    word ptr [edi + 18h], ax
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edi], edx
        mov    dword ptr [edi + 14h], esi
        mov    dword ptr [edi + 28h], ecx
        test   ebx, ebx
        jne    SHORT loc_6_5352
        push   edi
        call   sub_6_50F0
        add    esp, 4
        mov    ebx, eax
        mov    byte ptr [edi + 4], 2
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_534F:
        add    esp, 4
loc_6_5352:
        movsx  eax, bx
        mov    word ptr [edi + 1ah], bx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_6_5360:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 1ch]
        push   1
        push   eax
        inc    dword ptr [ecx + 55h]
        call   sub_6_5380
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_5380:
        push   ebp
        db  0B9h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 20h
        mov    eax, dword ptr [ecx]
        push   ebx
        push   esi
        lea    edx, [ebp - 20h]
        push   edi
        mov    ebx, dword ptr [ecx + 4]
        mov    esi, dword ptr [ecx + 8]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [edx + 4], ebx
        mov    dword ptr [edx + 8], esi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [edx + 0ch], eax
        mov    ecx, dword ptr [esi + 1ch]
        mov    eax, dword ptr [esi + 20h]
        or     byte ptr [ecx + 1ch], 10h
        mov    dword ptr [ebp - 0ch], eax
        test   eax, eax
        je     SHORT loc_6_53C3
        mov    ax, word ptr [eax + 35h]
        mov    word ptr [ebp - 4], ax
loc_6_53C3:
        cmp    byte ptr [esi + 4], 21h
        jne    SHORT loc_6_53DD
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        mov    ax, word ptr [eax + 24h]
        mov    word ptr [esi + 18h], ax
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_53DD:
        push   26h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_53E4:
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_6_53FF
        mov    eax, 36h
        pop    edi
        mov    word ptr [esi + 1ah], 36h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_53FF:
        mov    edi, ebx
        xor    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        mov    eax, dword ptr [ebp - 0ch]
        xor    ecx, ecx
        mov    dword ptr [ebx], esi
        mov    dword ptr [ebx + 8], eax
        lea    edi, [ebp - 20h]
        mov    dword ptr [ebp - 10h], ecx
        db  0C7h, 043h, 004h, 000h, 000h, 000h, 000h
        movzx  ax, byte ptr [esi + 4]
        mov    word ptr [ebx + 24h], ax
        mov    byte ptr [ebp - 1], cl
        movzx  ecx, ax
        dec    ecx
        cmp    ecx, 53h
        ja     SHORT loc_6_5447
        xor    eax, eax
        db  08Ah, 081h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_6_5447:
        mov    word ptr [esi + 1ah], 1
        jmp    NEAR PTR loc_6_55CC
loc_6_5452:
        mov    byte ptr [ebp - 1], 1
        mov    ax, word ptr [esi + 18h]
        mov    word ptr [ebp - 6], ax
        jmp    NEAR PTR loc_6_557B
loc_6_5463:
        mov    byte ptr [ebp - 1], 1
        mov    ax, word ptr [esi + 18h]
        mov    word ptr [ebp - 6], ax
loc_6_546F:
        mov    word ptr [ebx + 1ch], 6
loc_6_5475:
        mov    eax, dword ptr [esi]
        cmp    byte ptr [ebp + 0ch], 0
        mov    word ptr [ebx + 1eh], ax
        je     SHORT loc_6_5494
        push   61h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5489:
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_6_55CC
loc_6_5494:
        mov    eax, dword ptr [esi + 50h]
        mov    dword ptr [ebx + 10h], eax
        jmp    NEAR PTR loc_6_557B
loc_6_549F:
        mov    word ptr [ebx + 1ch], 2
        mov    eax, dword ptr [esi]
        cmp    byte ptr [ebp + 0ch], 0
        mov    word ptr [ebx + 18h], ax
        je     NEAR PTR loc_6_5575
        push   60h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_54BD:
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_6_55CC
        jmp    NEAR PTR loc_6_5575
loc_6_54CD:
        mov    cx, word ptr [esi + 18h]
        mov    edi, dword ptr [esi + 0ch]
        mov    word ptr [ebp - 4], cx
        mov    eax, dword ptr [esi + 28h]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_6_557B
loc_6_54E3:
        mov    eax, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [eax + 10h]
        test   byte ptr [ecx + 1ah], 80h
        jne    SHORT loc_6_54FF
        push   esi
        call   sub_6_5290
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_54FF:
        mov    cx, word ptr [esi + 18h]
        mov    edi, dword ptr [esi + 0ch]
        mov    word ptr [ebp - 4], cx
        mov    eax, dword ptr [esi + 28h]
        mov    dword ptr [ebp - 10h], eax
loc_6_5510:
        mov    eax, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [eax + 10h]
        test   byte ptr [ecx + 1ah], 80h
        jne    SHORT loc_6_552C
        push   esi
        call   sub_6_50F0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_552C:
        mov    eax, dword ptr [esi]
        cmp    byte ptr [ebp + 0ch], 0
        mov    word ptr [ebx + 1eh], ax
        je     SHORT loc_6_554B
        push   61h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5540:
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_6_55CC
loc_6_554B:
        mov    eax, dword ptr [esi + 50h]
        mov    ecx, dword ptr [esi + 10h]
        mov    dword ptr [ebx + 10h], eax
        mov    dword ptr [esi + 14h], ecx
        mov    eax, dword ptr [esi + 24h]
        cmp    byte ptr [ebp + 0ch], 0
        mov    word ptr [ebx + 18h], ax
        mov    dword ptr [esi], eax
        je     SHORT loc_6_5575
        push   40h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_556E:
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_6_55CC
loc_6_5575:
        mov    eax, dword ptr [esi + 50h]
        mov    dword ptr [ebx + 0ch], eax
loc_6_557B:
        mov    al, byte ptr [esi + 4]
        cmp    al, 54h
        je     SHORT loc_6_558B
        mov    edx, 0
        cmp    al, 23h
        jne    SHORT loc_6_5590
loc_6_558B:
        mov    edx, 1
loc_6_5590:
        mov    ecx, dword ptr [ebp - 6]
        and    ecx, 0ffffh
        push   ecx
        push   edi
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        push   ecx
        xor    ecx, ecx
        cmp    byte ptr [ebp - 1], 0
        mov    cl, al
        mov    eax, dword ptr [ebp - 10h]
        push   ecx
        push   eax
        push   edx
        db  0B8h, 000h, 000h, 000h, 000h
        jne    SHORT loc_6_55BF
        db  0B8h, 000h, 000h, 000h, 000h
loc_6_55BF:
        push   eax
        push   1
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_55C9:
        add    esp, 28h
loc_6_55CC:
        cmp    byte ptr [ebp + 0ch], 0
        je     SHORT loc_6_55E2
        mov    eax, dword ptr [ebx + 0ch]
        test   eax, eax
        je     SHORT loc_6_55E2
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_55DF:
        add    esp, 4
loc_6_55E2:
        mov    eax, dword ptr [ebx + 10h]
        test   eax, eax
        je     SHORT loc_6_5600
        cmp    byte ptr [ebp + 0ch], 0
        je     SHORT loc_6_55F8
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_55F5:
        add    esp, 4
loc_6_55F8:
        xor    eax, eax
        mov    ax, word ptr [ebx + 1eh]
        mov    dword ptr [esi], eax
loc_6_5600:
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5606:
        add    esp, 4
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Ah, 00Ah, 00Ah
        db  00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 001h, 00Ah, 00Ah, 00Ah
        db  00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 002h, 003h, 00Ah
        db  00Ah, 004h, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 005h, 00Ah, 00Ah, 00Ah
        db  00Ah, 006h, 007h, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah
        db  00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 00Ah, 008h, 009h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_6_56A0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    dx, word ptr [eax + 24h]
        mov    ecx, dword ptr [eax]
        dec    edx
        cmp    edx, 36h
        ja     SHORT loc_6_570E
        xor    eax, eax
        db  08Ah, 082h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_6_56C4:
        mov    ax, word ptr [ecx + 18h]
        mov    edx, dword ptr [ecx + 20h]
        mov    cx, word ptr [edx + 24h]
        add    edx, 24h
        xor    cx, ax
        pop    ebp
        and    cx, 7effh
        xor    cx, ax
        xor    eax, eax
        mov    word ptr [edx], cx
        ret
loc_6_56E4:
        xor    eax, eax
        mov    edx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [edx]
        mov    dword ptr [ecx + 24h], eax
        xor    eax, eax
        mov    ax, word ptr [edx + 2]
        pop    ebp
        mov    dx, word ptr [edx + 4]
        mov    dword ptr [ecx + 28h], eax
        mov    word ptr [ecx + 18h], dx
        xor    eax, eax
        ret
loc_6_5704:
        xor    eax, eax
        mov    edx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [edx]
        mov    dword ptr [ecx], eax
loc_6_570E:
        xor    eax, eax
        pop    ebp
        ret
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 001h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 002h, 004h, 004h, 004h, 004h, 004h, 003h, 090h
sub_6_5760:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 002h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        je     SHORT loc_6_5787
        mov    eax, 836h
        pop    edi
        mov    word ptr [ebx + 1ah], 836h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_5787:
        mov    dl, byte ptr [ebx + 5]
        cmp    dl, 3
        jb     SHORT loc_6_57A1
        mov    eax, 57h
        pop    edi
        mov    word ptr [ebx + 1ah], 57h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_57A1:
        mov    esi, dword ptr [ebx + 2ch]
        mov    ecx, 0ffffffffh
        mov    edi, esi
        sub    eax, eax
        repne scasb
        not    ecx
        mov    dword ptr [ebp - 4], ecx
        cmp    ecx, 15h
        jbe    SHORT loc_6_57CB
        mov    eax, 57h
        pop    edi
        mov    word ptr [ebx + 1ah], 57h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_57CB:
        xor    eax, eax
        mov    al, dl
        db  08Bh, 004h, 085h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_6_5838
        mov    dword ptr [eax + 28h], 0
        lea    edi, [eax + 13h]
        mov    edx, edi
loc_6_57E6:
        mov    cl, byte ptr [edx]
        cmp    cl, byte ptr [esi]
        jne    SHORT loc_6_5806
        or     cl, cl
        je     SHORT loc_6_5802
        mov    cl, byte ptr [edx + 1]
        cmp    cl, byte ptr [esi + 1]
        jne    SHORT loc_6_5806
        add    edx, 2
        add    esi, 2
        or     cl, cl
        jne    SHORT loc_6_57E6
loc_6_5802:
        xor    ecx, ecx
        jmp    SHORT loc_6_580B
loc_6_5806:
        sbb    ecx, ecx
        sbb    ecx, -1
loc_6_580B:
        test   ecx, ecx
        je     NEAR PTR loc_6_58B5
        mov    cl, byte ptr [edi]
        db  03Ah, 00Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_6_5826
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_6_587B
loc_6_5826:
        mov    eax, 55h
        pop    edi
        mov    word ptr [ebx + 1ah], 55h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_5838:
        push   ebx
        call   sub_6_5A50
        add    esp, 4
        xor    eax, eax
        mov    al, byte ptr [ebx + 5]
        push   40h
        db  08Dh, 034h, 085h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5854:
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_6_586D
        mov    eax, 54h
        pop    edi
        mov    word ptr [ebx + 1ah], 54h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_586D:
        mov    cl, byte ptr [ebx + 5]
        mov    dword ptr [esi], eax
        mov    dword ptr [eax + 1], 0
        mov    byte ptr [eax], cl
loc_6_587B:
        lea    edi, [eax + 13h]
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi], edx
        mov    esi, dword ptr [ebx + 2ch]
        mov    dword ptr [edi + 4], edx
        mov    dword ptr [edi + 8], edx
        mov    dword ptr [ebp - 4], ecx
        shr    ecx, 2
        mov    dword ptr [edi + 0ch], edx
        mov    dword ptr [edi + 10h], edx
        mov    byte ptr [edi + 14h], dl
        rep movsd
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 3
        rep movsb
        lea    ecx, [eax + 30h]
        mov    dword ptr [ecx], edx
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [ecx + 8], edx
        mov    dword ptr [ecx + 0ch], edx
loc_6_58B5:
        lea    edx, [eax + 5]
        mov    esi, dword ptr [ebx + 28h]
        xor    eax, eax
        mov    edi, esi
        mov    ecx, 0ffffffffh
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [edx + 8], eax
        mov    word ptr [edx + 0ch], ax
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx - 1]
        cmp    eax, 0eh
        jbe    SHORT loc_6_58E3
        mov    eax, 0eh
loc_6_58E3:
        mov    edi, edx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
sub_6_5900:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 5]
        cmp    al, 3
        jb     SHORT loc_6_591E
        mov    eax, 57h
        pop    edi
        mov    word ptr [esi + 1ah], 57h
        pop    esi
        pop    ebp
        ret
loc_6_591E:
        xor    ecx, ecx
        mov    cl, al
        db  08Bh, 03Ch, 08Dh, 000h, 000h, 000h, 000h
        test   edi, edi
        jne    SHORT loc_6_593C
        mov    eax, 6
        pop    edi
        mov    word ptr [esi + 1ah], 6
        pop    esi
        pop    ebp
        ret
loc_6_593C:
        push   esi
        call   sub_6_5A50
        add    esp, 4
        test   eax, eax
        je     SHORT loc_6_5951
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_6_5951:
        inc    edi
        cmp    dword ptr [edi], 0
        je     SHORT loc_6_5965
loc_6_5957:
        push   edi
        call   sub_6_5A20
        add    esp, 4
        cmp    dword ptr [edi], 0
        jne    SHORT loc_6_5957
loc_6_5965:
        xor    eax, eax
        mov    al, byte ptr [esi + 5]
        db  08Dh, 00Ch, 085h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_6_5A20
        add    esp, 4
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_6_5980:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        mov    al, byte ptr [edx + 5]
        push   edi
        cmp    al, 3
        jb     SHORT loc_6_59A1
        mov    eax, 57h
        pop    edi
        mov    word ptr [edx + 1ah], 57h
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_59A1:
        xor    ecx, ecx
        mov    cl, al
        db  08Bh, 01Ch, 08Dh, 000h, 000h, 000h, 000h
        test   ebx, ebx
        jne    SHORT loc_6_59C0
        mov    eax, 6
        pop    edi
        mov    word ptr [edx + 1ah], 6
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_59C0:
        lea    edi, [ebx + 13h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [edx + 2ch]
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        cmp    dword ptr [edx], 0
        je     SHORT loc_6_5A0B
        lea    edi, [ebx + 5]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [edx + 14h]
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_6_5A0B:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_5A20:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   edi
        mov    edx, dword ptr [eax]
        mov    edi, edx
        mov    ecx, dword ptr [edx + 1]
        push   edx
        mov    dword ptr [eax], ecx
        xor    eax, eax
        mov    ecx, 10h
        rep stosd
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5A40:
        add    esp, 4
        pop    edi
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_5A50:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
loc_6_5A5C:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        test   edi, edi
        je     SHORT loc_6_5AC4
loc_6_5A66:
        mov    dword ptr [ebp - 4], 0
        mov    ebx, dword ptr [edi + 0ch]
        test   ebx, ebx
        je     SHORT loc_6_5A89
loc_6_5A74:
        cmp    byte ptr [ebx + 34h], 0
        je     SHORT loc_6_5ACD
        mov    dword ptr [ebp - 4], 1
loc_6_5A81:
        mov    ebx, dword ptr [ebx + 4]
        cmp    dword ptr [edi + 0ch], ebx
        jne    SHORT loc_6_5A74
loc_6_5A89:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_6_5AB9
        mov    al, byte ptr [esi + 5]
        push   eax
        push   edi
        push   esi
        call   sub_6_30C0
        add    esp, 0ch
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_6_5AB5
loc_6_5AA6:
        cmp    eax, edi
        je     SHORT loc_6_5AB9
        mov    eax, dword ptr [eax + 4]
        db  03Bh, 005h, 000h, 000h, 000h, 000h
        jne    SHORT loc_6_5AA6
loc_6_5AB5:
        cmp    eax, edi
        jne    SHORT loc_6_5A5C
loc_6_5AB9:
        mov    edi, dword ptr [edi + 4]
        db  03Bh, 03Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_6_5A66
loc_6_5AC4:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_5ACD:
        mov    edx, dword ptr [ebx + 30h]
        test   edx, edx
        je     SHORT loc_6_5A81
        mov    ecx, edx
        mov    dword ptr [esi + 1ch], ebx
loc_6_5AD9:
        mov    al, byte ptr [esi + 5]
        cmp    byte ptr [ecx + 30h], al
        je     SHORT loc_6_5AEA
        mov    ecx, dword ptr [ecx + 8]
        cmp    edx, ecx
        jne    SHORT loc_6_5AD9
        jmp    SHORT loc_6_5A81
loc_6_5AEA:
        push   ecx
        push   esi
        call   0
        add    esp, 8
        test   eax, eax
        je     SHORT loc_6_5ACD
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_5B10:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        cmp    word ptr [edx], 4
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        jne    SHORT loc_6_5B27
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_5B27:
        xor    esi, esi
        mov    edx, dword ptr [ebp + 8]
        mov    si, word ptr [edx + 2]
        db  0BFh, 000h, 000h, 000h, 000h
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5B3B:
        add    esp, 4
        lea    eax, [eax*2 + 2]
        mov    ecx, eax
        mov    edx, dword ptr [ebp + 8]
        shr    ecx, 2
        lea    esi, [esi + edx + 2]
        repe cmpsd
        jne    SHORT loc_6_5B81
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb
        jne    SHORT loc_6_5B81
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5B64:
        add    esp, 4
        mov    edx, dword ptr [ebp + 8]
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5B77:
        add    esp, 0ch
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_5B81:
        xor    eax, eax
        mov    edx, dword ptr [ebp + 8]
        mov    ax, word ptr [edx + 2]
        lea    ebx, [eax + edx + 2]
        cmp    word ptr [ebx], 0
        mov    esi, ebx
        je     NEAR PTR loc_6_5CA4
        xor    ecx, ecx
        mov    edx, 2ah
        mov    edi, 2eh
loc_6_5BA6:
        mov    ax, word ptr [ebx]
        cmp    ax, di
        jne    SHORT loc_6_5BC5
        cmp    word ptr [ebx + 2], cx
        je     SHORT loc_6_5BE5
        cmp    ax, di
        jne    SHORT loc_6_5BC5
        cmp    word ptr [ebx + 2], dx
        jne    SHORT loc_6_5BC5
        cmp    word ptr [ebx + 4], cx
        je     SHORT loc_6_5C3C
loc_6_5BC5:
        cmp    ax, dx
        jne    SHORT loc_6_5BD6
        cmp    word ptr [ebx + 2], di
        jne    SHORT loc_6_5BD6
        cmp    word ptr [ebx + 4], cx
        je     SHORT loc_6_5C3C
loc_6_5BD6:
        add    ebx, 2
        cmp    word ptr [ebx], cx
        jne    SHORT loc_6_5BA6
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_5BE5:
        mov    edi, dword ptr [ebp + 0ch]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5BEE:
        add    esp, 4
        lea    eax, [eax*2 + 2]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5BFE:
        add    esp, 4
        mov    dword ptr [edi + 234h], eax
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_6_5C17
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_5C17:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5C1D:
        add    esp, 4
        add    eax, eax
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    word ptr [ebx], 2ah
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_5C3C:
        mov    edi, dword ptr [ebp + 0ch]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5C45:
        add    esp, 4
        lea    eax, [eax*2 + 2]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5C55:
        add    esp, 4
        mov    dword ptr [edi + 234h], eax
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_6_5C6E
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_5C6E:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5C74:
        add    esp, 4
        add    eax, eax
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    word ptr [ebx + 2], 0
        mov    word ptr [ebx], 2ah
        xor    eax, eax
        mov    edx, dword ptr [ebp + 8]
        mov    ax, word ptr [edx + 2]
        sub    word ptr [eax + edx], 2
        sub    word ptr [edx], 2
loc_6_5CA4:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_6_5CB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 20h]
        mov    dword ptr [ebp - 8], ecx
        mov    edi, dword ptr [eax + 10h]
        test   byte ptr [eax + 1ch], 80h
        je     SHORT loc_6_5CF0
        push   4ffh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5CD9:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_6_5CE7
        mov    ecx, dword ptr [eax + 20h]
        mov    word ptr [esi + 1ah], cx
loc_6_5CE7:
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5CED:
        add    esp, 4
loc_6_5CF0:
        test   byte ptr [edi + 1bh], 1
        jne    SHORT loc_6_5D08
        mov    eax, 1
        pop    edi
        mov    word ptr [esi + 1ah], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_5D08:
        push   26h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5D0F:
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_6_5D2A
        mov    eax, 8
        pop    edi
        mov    word ptr [esi + 1ah], 8
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_5D2A:
        push   28h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5D35:
        add    esp, 8
        mov    ebx, eax
        cmp    dword ptr [ebp + 10h], 0
        mov    word ptr [edi + 1ch], 2
        mov    dword ptr [edi], esi
        je     SHORT loc_6_5D71
        lea    eax, [ebp - 2]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        mov    dword ptr [edi + 10h], ebx
        push   101h
        xor    eax, eax
        mov    word ptr [edi + 1eh], 28h
        mov    ax, word ptr [ecx + 35h]
        push   eax
        push   7
        push   0
        push   1
        db  068h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_6_5D98
loc_6_5D71:
        lea    eax, [ebp - 2]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        mov    dword ptr [edi + 0ch], ebx
        push   101h
        xor    eax, eax
        mov    word ptr [edi + 18h], 28h
        mov    ax, word ptr [ecx + 35h]
        push   eax
        push   8
        push   0
        push   1
        db  068h, 000h, 000h, 000h, 000h
loc_6_5D98:
        push   0
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5DA1:
        add    esp, 28h
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5DAA:
        add    esp, 4
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5DB3:
        add    esp, 4
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_5DD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   252h
        mov    eax, dword ptr [esi + 20h]
        mov    edi, dword ptr [esi + 14h]
        mov    dword ptr [ebp - 10h], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5DEF:
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_6_5E0A
        mov    eax, 8
        pop    edi
        mov    word ptr [esi + 1ah], 8
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_5E0A:
        push   26h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5E11:
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    SHORT loc_6_5E36
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5E21:
        add    esp, 4
        mov    eax, 8
        mov    word ptr [esi + 1ah], 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_5E36:
        push   252h
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5E41:
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [eax + 1eh], 252h
        mov    dword ptr [eax], esi
        mov    dword ptr [eax + 10h], ecx
        mov    word ptr [eax + 1ch], 2
        lea    ecx, [ebp - 2]
        mov    eax, dword ptr [ebp - 10h]
        push   ecx
        push   107h
        xor    ecx, ecx
        mov    cx, word ptr [eax + 35h]
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   7
        push   0
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   edx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5E88:
        add    esp, 28h
        cmp    word ptr [esi + 1ah], 0
        jne    SHORT loc_6_5EE7
        mov    eax, dword ptr [ebx + 20h]
        mov    ecx, dword ptr [ebx + 4]
        lea    edx, [edi + 4]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [ebx]
        mov    dword ptr [edx], eax
        lea    eax, [ebx + 8]
        mov    dword ptr [edx + 4], ecx
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        lea    edx, [edi + 0ch]
        mov    dword ptr [edx], ecx
        mov    dword ptr [edx + 4], eax
        lea    eax, [ebx + 10h]
        lea    edx, [edi + 14h]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [ebx + 34h]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [edi + 20h], ecx
        mov    eax, dword ptr [ebx + 30h]
        mov    dword ptr [edi + 24h], eax
        mov    edx, dword ptr [ebx + 38h]
        mov    dword ptr [edi + 28h], edx
        push   1
        add    edi, 1ch
        push   edi
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5EE4:
        add    esp, 10h
loc_6_5EE7:
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5EF0:
        add    esp, 4
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5EF9:
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_5F05:
        add    esp, 4
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_5F20:
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        mov    ebp, esp
        cmp    eax, -1
        push   esi
        je     SHORT loc_6_5F3D
        db  039h, 005h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_5F3D
        mov    eax, 1
        pop    esi
        pop    ebp
        ret
loc_6_5F3D:
        mov    edx, dword ptr [ebp + 10h]
        xor    eax, eax
        mov    ax, word ptr [edx + 1ch]
        cmp    eax, 1180h
        jg     SHORT loc_6_5F60
        je     NEAR PTR loc_6_5FDA
        cmp    eax, 3
        je     SHORT loc_6_5FC9
loc_6_5F58:
        mov    eax, 1
        pop    esi
        pop    ebp
        ret
loc_6_5F60:
        cmp    eax, 118ah
        jg     SHORT loc_6_5F92
        je     NEAR PTR loc_6_6040
        cmp    eax, 1181h
        je     SHORT loc_6_5FF0
        cmp    eax, 1182h
        je     NEAR PTR loc_6_6014
        cmp    eax, 1184h
        je     NEAR PTR loc_6_602A
        mov    eax, 1
        pop    esi
        pop    ebp
        ret
loc_6_5F92:
        cmp    eax, 5f30h
        jg     SHORT loc_6_5FB0
        je     NEAR PTR loc_6_6064
        sub    eax, 118bh
        cmp    eax, 3
        ja     SHORT loc_6_5F58
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_6_5FB0:
        sub    eax, 5f3dh
        cmp    eax, 4ch
        ja     SHORT loc_6_5F58
        xor    ecx, ecx
        db  08Ah, 088h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_6_5FC9:
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        call   sub_6_11D0
        add    esp, 4
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
loc_6_5FDA:
        mov    eax, dword ptr [ebp + 18h]
        mov    esi, dword ptr [ebp + 0ch]
        push   eax
        push   edx
        push   esi
        call   sub_6_83E0
        add    esp, 0ch
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
loc_6_5FF0:
        mov    eax, dword ptr [ebp + 18h]
        mov    esi, dword ptr [ebp + 0ch]
        push   eax
        push   edx
        push   esi
        call   sub_6_8510
        add    esp, 0ch
        cmp    word ptr [esi + 1ah], 0
        jne    NEAR PTR loc_6_616C
        mov    eax, 2
        pop    esi
        pop    ebp
        ret
loc_6_6014:
        mov    eax, dword ptr [ebp + 18h]
        mov    esi, dword ptr [ebp + 0ch]
        push   eax
        push   edx
        push   esi
        call   sub_6_8CF0
        add    esp, 0ch
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
loc_6_602A:
        mov    eax, dword ptr [ebp + 18h]
        mov    esi, dword ptr [ebp + 0ch]
        push   eax
        push   edx
        push   esi
        call   sub_6_8E60
        add    esp, 0ch
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
loc_6_6040:
        mov    esi, dword ptr [ebp + 0ch]
        xor    eax, eax
        mov    word ptr [esi + 1ah], 32h
        pop    esi
        pop    ebp
        ret
loc_6_604E:
        mov    eax, dword ptr [ebp + 18h]
        mov    esi, dword ptr [ebp + 0ch]
        push   eax
        push   edx
        push   esi
        call   sub_6_84A0
        add    esp, 0ch
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
loc_6_6064:
        db  066h, 00Fh, 0B6h, 005h, 000h, 000h, 000h, 000h
        imul   ax, ax, 64h
        db  066h, 00Fh, 0B6h, 00Dh, 001h, 000h, 000h, 000h
        add    ax, cx
        pop    esi
        mov    word ptr [edx + 1ch], ax
        pop    ebp
        xor    eax, eax
        ret
loc_6_6084:
        mov    eax, dword ptr [ebp + 18h]
        mov    esi, dword ptr [ebp + 0ch]
        push   eax
        push   edx
        push   esi
        call   sub_6_6FA0
        add    esp, 0ch
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
loc_6_609A:
        mov    eax, dword ptr [ebp + 18h]
        mov    esi, dword ptr [ebp + 0ch]
        push   eax
        push   edx
        push   esi
        call   sub_6_7300
        add    esp, 0ch
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
loc_6_60B0:
        mov    eax, dword ptr [ebp + 18h]
        mov    esi, dword ptr [ebp + 0ch]
        push   eax
        push   edx
        push   esi
        call   sub_6_A720
        add    esp, 0ch
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
loc_6_60C6:
        mov    eax, dword ptr [ebp + 18h]
        mov    esi, dword ptr [ebp + 0ch]
        push   eax
        push   edx
        push   esi
        call   sub_6_8B40
        add    esp, 0ch
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
loc_6_60DC:
        mov    eax, dword ptr [ebp + 18h]
        mov    esi, dword ptr [ebp + 0ch]
        push   eax
        push   edx
        push   esi
        call   sub_6_8C00
        add    esp, 0ch
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
loc_6_60F2:
        mov    eax, dword ptr [ebp + 18h]
        mov    esi, dword ptr [ebp + 0ch]
        push   eax
        push   edx
        push   esi
        call   sub_6_90B0
        add    esp, 0ch
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
loc_6_6108:
        mov    eax, dword ptr [ebp + 18h]
        mov    esi, dword ptr [ebp + 0ch]
        push   eax
        push   edx
        push   esi
        call   sub_6_9580
        add    esp, 0ch
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
loc_6_611E:
        push   edx
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        call   sub_6_7260
        add    esp, 8
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
loc_6_6130:
        mov    ax, word ptr [edx + 18h]
        mov    esi, dword ptr [ebp + 0ch]
        mov    word ptr [esi + 6], ax
        mov    ecx, dword ptr [ebp + 18h]
        push   ecx
        push   edx
        push   esi
        call   sub_6_A300
        add    esp, 0ch
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
loc_6_614E:
        mov    eax, dword ptr [ebp + 18h]
        mov    esi, dword ptr [ebp + 0ch]
        push   eax
        push   edx
        push   esi
        call   sub_6_A2D0
        add    esp, 0ch
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
loc_6_6164:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [edx + 1ch], eax
loc_6_616C:
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Dh, 001h, 002h, 003h
        db  004h, 00Dh, 005h, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 006h, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh
        db  00Dh, 007h, 008h, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh
        db  00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh
        db  00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 00Dh, 009h
        db  00Dh, 00Dh, 00Dh, 00Dh, 00Ah, 00Dh, 00Bh, 00Ch, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_6210:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [edx + 0ch]
        cmp    eax, -1
        ja     SHORT loc_6_6230
        je     SHORT loc_6_625E
        sub    eax, 64h
        cmp    eax, 0bh
        ja     SHORT loc_6_6230
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_6_6230:
        mov    eax, 57h
        pop    ebp
        ret
loc_6_6237:
        cmp    dword ptr [edx + 1ch], 2
        jae    SHORT loc_6_6244
        mov    eax, 7ah
        pop    ebp
        ret
loc_6_6244:
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [edx + 18h]
        mov    word ptr [eax], cx
        mov    eax, dword ptr [edx + 20h]
        test   eax, eax
        je     SHORT loc_6_625E
        mov    dword ptr [eax], 2
loc_6_625E:
        xor    eax, eax
        pop    ebp
        ret
loc_6_6262:
        cmp    dword ptr [edx + 1ch], 4
        jae    SHORT loc_6_626F
        mov    eax, 7ah
        pop    ebp
        ret
loc_6_626F:
        mov    eax, dword ptr [edx + 18h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [edx + 20h]
        test   eax, eax
        je     SHORT loc_6_625E
        pop    ebp
        mov    dword ptr [eax], 4
        xor    eax, eax
        ret
loc_6_628B:
        push   edx
        call   sub_6_6DB0
        mov    esp, ebp
        pop    ebp
        ret
loc_6_6295:
        push   edx
        call   sub_6_6E30
        mov    esp, ebp
        pop    ebp
        ret
loc_6_629F:
        push   edx
        call   sub_6_6A40
        mov    esp, ebp
        pop    ebp
        ret
loc_6_62A9:
        push   edx
        call   sub_6_6460
        mov    esp, ebp
        pop    ebp
        ret
loc_6_62B3:
        push   edx
        call   sub_6_6380
        mov    esp, ebp
        pop    ebp
        ret
loc_6_62BD:
        push   edx
        call   sub_6_6310
        mov    esp, ebp
        pop    ebp
        ret
loc_6_62C7:
        push   edx
        call   sub_6_6F20
        mov    esp, ebp
        pop    ebp
        ret
loc_6_62D1:
        push   edx
        call   sub_6_6E80
        mov    esp, ebp
        pop    ebp
        ret
        db  090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 08Dh, 064h, 024h, 000h
sub_6_6310:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        cmp    dword ptr [esi + 1ch], 4
        jae    SHORT loc_6_6335
        mov    eax, 7ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_6335:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        test   edi, edi
        je     SHORT loc_6_636E
loc_6_633F:
        mov    ebx, dword ptr [edi + 10h]
        test   ebx, ebx
        je     SHORT loc_6_6367
loc_6_6346:
        mov    eax, dword ptr [ebx]
        test   eax, eax
        je     SHORT loc_6_635D
        lea    ecx, [ebp - 1]
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_6356:
        add    esp, 8
        test   al, al
        jne    SHORT loc_6_6360
loc_6_635D:
        inc    dword ptr [ebp - 8]
loc_6_6360:
        mov    ebx, dword ptr [ebx + 4]
        test   ebx, ebx
        jne    SHORT loc_6_6346
loc_6_6367:
        mov    edi, dword ptr [edi + 18h]
        test   edi, edi
        jne    SHORT loc_6_633F
loc_6_636E:
        mov    eax, dword ptr [esi + 18h]
        mov    ecx, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], ecx
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
sub_6_6380:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        cmp    dword ptr [ecx + 14h], 4
        push   esi
        push   edi
        jae    SHORT loc_6_639A
        mov    eax, 57h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_639A:
        cmp    dword ptr [ecx + 1ch], 0ch
        jae    SHORT loc_6_63AA
        mov    eax, 57h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_63AA:
        mov    eax, dword ptr [ecx + 10h]
        mov    edi, dword ptr [ecx + 18h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        test   ecx, ecx
        je     SHORT loc_6_63F3
loc_6_63BA:
        mov    edx, dword ptr [eax]
        lea    esi, [ecx + 2ch]
loc_6_63BF:
        mov    bl, byte ptr [edx]
        cmp    bl, byte ptr [esi]
        jne    SHORT loc_6_63DF
        or     bl, bl
        je     SHORT loc_6_63DB
        mov    bl, byte ptr [edx + 1]
        cmp    bl, byte ptr [esi + 1]
        jne    SHORT loc_6_63DF
        add    edx, 2
        add    esi, 2
        or     bl, bl
        jne    SHORT loc_6_63BF
loc_6_63DB:
        xor    edx, edx
        jmp    SHORT loc_6_63E4
loc_6_63DF:
        sbb    edx, edx
        sbb    edx, -1
loc_6_63E4:
        test   edx, edx
        je     SHORT loc_6_63FD
        mov    ecx, dword ptr [ecx + 4]
        db  039h, 00Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_6_63BA
loc_6_63F3:
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_63FD:
        mov    ecx, dword ptr [ecx + 85h]
        cmp    ecx, -1
        je     SHORT loc_6_6437
        mov    esi, 37h
        mov    eax, ecx
        cdq
        idiv   esi
        mov    dword ptr [edi], eax
        mov    eax, ecx
        shl    eax, 6
        add    eax, ecx
        lea    edx, [eax + eax*2]
        lea    ebx, [ecx + edx*4]
        lea    eax, [ecx + ebx*4]
        mov    ecx, 240h
        lea    eax, [eax + eax*4]
        shl    eax, 6
        cdq
        idiv   ecx
        mov    dword ptr [edi + 4], eax
        jmp    SHORT loc_6_6444
loc_6_6437:
        mov    dword ptr [edi + 4], 0ffffffffh
        mov    dword ptr [edi], 0ffffffffh
loc_6_6444:
        mov    dword ptr [edi + 8], 1
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_6460:
        push   ebp
        mov    ebp, esp
        sub    esp, 64h
        mov    dword ptr [ebp - 14h], 0
        push   ebx
        push   esi
        push   edi
        push   1a8h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_647A:
        add    esp, 4
        mov    dword ptr [ebp - 18h], eax
        test   eax, eax
        jne    SHORT loc_6_6490
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_6490:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 14h], 20h
        jae    SHORT loc_6_64A5
        mov    dword ptr [ebp - 14h], 57h
        jmp    NEAR PTR loc_6_68E1
loc_6_64A5:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 1ch], 32h
        jae    SHORT loc_6_64BA
        mov    dword ptr [ebp - 14h], 57h
        jmp    NEAR PTR loc_6_68E1
loc_6_64BA:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 10h]
        mov    edx, dword ptr [ecx + 18h]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 4], edx
        test   dword ptr [eax], 0fffffffeh
        je     SHORT loc_6_64DD
        mov    dword ptr [ebp - 14h], 57h
        jmp    NEAR PTR loc_6_68E1
loc_6_64DD:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [eax + 8], 8
        je     SHORT loc_6_64F2
        mov    dword ptr [ebp - 14h], 57h
        jmp    NEAR PTR loc_6_68E1
loc_6_64F2:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 2ch], 0
        mov    edx, dword ptr [eax + 18h]
        mov    ecx, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 10h], edx
        mov    dword ptr [ebp - 1ch], ecx
        mov    edx, dword ptr [eax + 10h]
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 24h], edx
        mov    dword ptr [ebp - 8], ecx
        test   ecx, ecx
        jne    NEAR PTR loc_6_661C
        mov    ecx, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 20h], ecx
        test   ecx, ecx
        je     NEAR PTR loc_6_65D5
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    eax, edi
        test   edi, edi
        je     NEAR PTR loc_6_65D5
        mov    cx, word ptr [ebp - 2]
loc_6_653E:
        mov    edx, dword ptr [ebp - 20h]
        lea    esi, [eax + 2ch]
loc_6_6544:
        mov    bl, byte ptr [edx]
        cmp    bl, byte ptr [esi]
        jne    SHORT loc_6_6564
        or     bl, bl
        je     SHORT loc_6_6560
        mov    bl, byte ptr [edx + 1]
        cmp    bl, byte ptr [esi + 1]
        jne    SHORT loc_6_6564
        add    edx, 2
        add    esi, 2
        or     bl, bl
        jne    SHORT loc_6_6544
loc_6_6560:
        xor    edx, edx
        jmp    SHORT loc_6_6569
loc_6_6564:
        sbb    edx, edx
        sbb    edx, -1
loc_6_6569:
        test   edx, edx
        jne    SHORT loc_6_65C4
        mov    edx, dword ptr [eax + 0ch]
        test   byte ptr [edx + 1ch], 20h
        je     SHORT loc_6_65C4
        add    edx, 35h
        cmp    dword ptr [ebp - 10h], 0
        mov    dword ptr [ebp - 8], edx
        je     SHORT loc_6_6588
        cmp    dword ptr [ebp - 1ch], 0
        jne    SHORT loc_6_65C4
loc_6_6588:
        cmp    byte ptr [eax + 7eh], 1
        jbe    SHORT loc_6_6595
        mov    cx, word ptr [eax + 81h]
loc_6_6595:
        test   cx, cx
        jne    SHORT loc_6_65A3
        mov    cx, word ptr [eax + 7fh]
        test   cx, cx
        je     SHORT loc_6_65C4
loc_6_65A3:
        movzx  edx, cx
        db  08Bh, 014h, 095h, 000h, 000h, 000h, 000h
        test   edx, edx
        je     SHORT loc_6_65C4
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_6_65BD
        add    edx, 13h
        mov    dword ptr [ebp - 10h], edx
loc_6_65BD:
        db  0C7h, 045h, 0E4h, 000h, 000h, 000h, 000h
loc_6_65C4:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_6_6637
        mov    eax, dword ptr [eax + 4]
        cmp    edi, eax
        jne    NEAR PTR loc_6_653E
loc_6_65D5:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_6_6637
        db  083h, 03Dh, 004h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_6610
        db  0A1h, 004h, 000h, 000h, 000h
        add    eax, 5
        cmp    dword ptr [ebp - 10h], 0
        mov    dword ptr [ebp - 8], eax
        jne    SHORT loc_6_6600
        db  0A1h, 004h, 000h, 000h, 000h
        add    eax, 13h
        mov    dword ptr [ebp - 10h], eax
loc_6_6600:
        db  0C7h, 045h, 0E4h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 2ch], 1
        jmp    SHORT loc_6_6637
loc_6_6610:
        mov    dword ptr [ebp - 14h], 186eh
        jmp    NEAR PTR loc_6_68E1
loc_6_661C:
        db  083h, 03Dh, 004h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_6630
        db  0A1h, 004h, 000h, 000h, 000h
        add    eax, 13h
        mov    dword ptr [ebp - 10h], eax
loc_6_6630:
        db  0C7h, 045h, 0E4h, 000h, 000h, 000h, 000h
loc_6_6637:
        lea    eax, [ebp - 3ch]
        xor    ecx, ecx
        mov    edi, dword ptr [ebp - 8]
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax + 8], ecx
        mov    word ptr [eax + 0ch], cx
        mov    byte ptr [eax + 0eh], cl
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    eax, 0eh
        cmp    ecx, 0eh
        jae    SHORT loc_6_6675
        mov    edi, dword ptr [ebp - 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx - 1]
loc_6_6675:
        lea    edi, [ebp - 3ch]
        mov    esi, dword ptr [ebp - 8]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [ebp - 1ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    edi, dword ptr [ebp - 10h]
        sub    eax, eax
        lea    edx, [ecx - 1]
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        mov    eax, dword ptr [ebp - 18h]
        lea    ecx, [ecx + edx + 31h]
        push   eax
        lea    edx, [ebp - 3ch]
        push   edx
        mov    dword ptr [ebp - 20h], ecx
        mov    ecx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ecx + 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_66C3:
        add    esp, 0ch
        mov    edx, dword ptr [ebp - 4]
        add    edx, 18h
        mov    esi, dword ptr [ebp - 18h]
        mov    edi, edx
        mov    ecx, 6
        mov    dword ptr [ebp - 28h], edx
        rep movsd
        mov    eax, dword ptr [ebp - 2ch]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 24h], 0
        lea    ebx, [ecx + 30h]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 4], 18h
        je     NEAR PTR loc_6_67B9
        add    dword ptr [ebp - 20h], 18h
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 20h]
        cmp    dword ptr [ecx + 1ch], eax
        jae    SHORT loc_6_6712
        mov    dword ptr [ebp - 14h], 57h
        jmp    NEAR PTR loc_6_68E1
loc_6_6712:
        lea    eax, [ebp - 3ch]
        xor    ecx, ecx
        mov    edi, dword ptr [ebp - 24h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax + 8], ecx
        mov    word ptr [eax + 0ch], cx
        mov    byte ptr [eax + 0eh], cl
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    eax, 0eh
        cmp    ecx, 0eh
        jae    SHORT loc_6_6750
        mov    edi, dword ptr [ebp - 24h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx - 1]
loc_6_6750:
        lea    edi, [ebp - 3ch]
        mov    esi, dword ptr [ebp - 24h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        push   1
        and    ecx, 3
        rep movsb
        lea    edi, [ebp - 3ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        lea    edx, [ebp - 3ch]
        repne scasb
        not    ecx
        dec    ecx
        lea    eax, [ebp - 64h]
        push   ecx
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_6783:
        add    esp, 10h
        mov    ecx, dword ptr [ebp - 18h]
        lea    edx, [ebp - 64h]
        push   ecx
        push   eax
        push   edx
        mov    eax, dword ptr [ebp - 0ch]
        mov    edi, ebx
        mov    ecx, dword ptr [eax + 4]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_679D:
        add    esp, 10h
        mov    esi, dword ptr [ebp - 18h]
        mov    ecx, 6
        add    ebx, 18h
        rep movsd
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edx + 8], 18h
        jmp    SHORT loc_6_67C3
loc_6_67B9:
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edx + 8], 0
loc_6_67C3:
        mov    edi, dword ptr [ebp - 10h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        mov    edx, dword ptr [ebp - 4]
        repne scasb
        mov    edi, ebx
        mov    esi, dword ptr [ebp - 10h]
        not    ecx
        mov    eax, ecx
        mov    dword ptr [edx + 10h], ecx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edx, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp - 1ch]
        mov    ecx, 0ffffffffh
        add    ebx, dword ptr [edx + 10h]
        sub    eax, eax
        repne scasb
        mov    edi, ebx
        mov    esi, dword ptr [ebp - 1ch]
        not    ecx
        mov    eax, ecx
        mov    dword ptr [edx + 14h], ecx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [edx + 0ch], 0
        add    ebx, dword ptr [edx + 14h]
        test   byte ptr [eax], 1
        je     NEAR PTR loc_6_68D8
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp + 8]
        add    eax, 8
        cmp    dword ptr [ecx + 1ch], eax
        jae    SHORT loc_6_6847
        mov    dword ptr [ebp - 14h], 57h
        jmp    NEAR PTR loc_6_68E1
loc_6_6847:
        lea    eax, [ebp - 3ch]
        xor    ecx, ecx
        mov    edi, dword ptr [ebp - 8]
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax + 8], ecx
        mov    word ptr [eax + 0ch], cx
        mov    byte ptr [eax + 0eh], cl
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    eax, 0eh
        cmp    ecx, 0eh
        jae    SHORT loc_6_6885
        mov    edi, dword ptr [ebp - 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx - 1]
loc_6_6885:
        lea    edi, [ebp - 3ch]
        mov    esi, dword ptr [ebp - 8]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        lea    edx, [ebp - 44h]
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebp - 28h]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        lea    ecx, [ebp - 3ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [edx + 4], eax
        mov    edx, dword ptr [ebp - 18h]
        push   edx
        lea    eax, [ebp - 44h]
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_68BA:
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 18h]
        mov    edx, dword ptr [ebp - 4]
        add    ebx, 8
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebx - 8], ecx
        mov    dword ptr [ebx - 4], eax
        mov    dword ptr [edx + 0ch], 8
loc_6_68D8:
        sub    ebx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 20h], ebx
loc_6_68E1:
        mov    ecx, dword ptr [ebp - 18h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_68EA:
        add    esp, 4
        mov    eax, dword ptr [ebp - 14h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_6900:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        push   208h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_6912:
        add    esp, 4
        mov    esi, eax
        cmp    esi, ebx
        jne    SHORT loc_6_6925
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_6925:
        push   1
        mov    edx, dword ptr [ebp + 8]
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        push   edx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_6940:
        add    esp, 10h
        lea    eax, [esi + 4]
        cmp    word ptr [eax], 0
        mov    edi, eax
        je     SHORT loc_6_695D
loc_6_694E:
        cmp    word ptr [eax], 5ch
        je     SHORT loc_6_6963
        add    eax, 2
        cmp    word ptr [eax], 0
        jne    SHORT loc_6_694E
loc_6_695D:
        cmp    word ptr [eax], 5ch
        jne    SHORT loc_6_696B
loc_6_6963:
        mov    word ptr [eax], 0
        lea    ebx, [eax + 2]
loc_6_696B:
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_6971:
        add    esp, 4
        mov    ecx, dword ptr [ebp + 10h]
        lea    eax, [eax*2 + 2]
        mov    edx, dword ptr [ebp + 0ch]
        push   1
        push   ecx
        push   eax
        push   edi
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_698C:
        add    esp, 14h
        test   ebx, ebx
        je     SHORT loc_6_69B7
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_6999:
        add    esp, 4
        mov    ecx, dword ptr [ebp + 18h]
        lea    eax, [eax*2 + 2]
        mov    edx, dword ptr [ebp + 14h]
        push   1
        push   ecx
        push   eax
        push   ebx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_69B4:
        add    esp, 14h
loc_6_69B7:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_69BD:
        add    esp, 4
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_69D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        add    edi, edi
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_69E1:
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_6_69F4
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_69F4:
        push   1
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   esi
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_6A04:
        add    esp, 10h
        push   edi
        push   ebx
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_6A0F:
        add    esp, 0ch
        mov    ecx, dword ptr [ebp + 0ch]
        push   1
        push   ecx
        push   edi
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_6A20:
        add    esp, 14h
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_6A29:
        add    esp, 4
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_6A40:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        db  0A0h, 000h, 000h, 000h, 000h
        sub    esp, 108h
        mov    dword ptr [ebp - 10h], ecx
        mov    byte ptr [ebp - 30h], al
        push   ebx
        push   esi
        lea    eax, [ebp - 2fh]
        push   edi
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax + 8], ecx
        mov    word ptr [eax + 0ch], cx
        mov    byte ptr [eax + 0eh], cl
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 1ch], 14h
        jae    SHORT loc_6_6A80
        mov    eax, 7ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_6A80:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, 0ffffffffh
        mov    ebx, dword ptr [eax + 10h]
        sub    eax, eax
        mov    edx, dword ptr [ebx]
        mov    edi, edx
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        push   edx
        call   sub_6_69D0
        add    esp, 8
        test   eax, eax
        je     SHORT loc_6_6AB9
        mov    eax, 8
        pop    edi
        mov    word ptr [ebp - 8ah], 8
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_6AB9:
        lea    edi, [ebp - 0a4h]
        xor    eax, eax
        mov    ecx, 1dh
        rep stosd
        lea    ecx, [ebp - 30h]
        lea    edi, [ebp - 108h]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 4], eax
        push   2eh
        lea    edx, [ebp - 108h]
        mov    dword ptr [ecx + 8], eax
        push   edx
        mov    dword ptr [ecx + 0ch], eax
        mov    ecx, 0bh
        push   10h
        rep stosd
        stosw
        lea    ecx, [ebp - 30h]
        mov    eax, dword ptr [ebx]
        push   ecx
        push   eax
        call   sub_6_6900
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_6_6B18
        mov    eax, 8
        pop    edi
        mov    word ptr [ebp - 8ah], 8
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_6B18:
        push   0
        lea    eax, [ebp - 30h]
        push   0ffh
        push   eax
        call   sub_6_21F0
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     SHORT loc_6_6B5A
        lea    eax, [ebp - 108h]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        lea    edx, [ebp - 0a4h]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_6B49:
        add    esp, 0ch
        mov    dword ptr [ebp - 88h], eax
        test   eax, eax
        jne    NEAR PTR loc_6_6C32
loc_6_6B5A:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_6_6B66
        mov    eax, dword ptr [ebp - 4]
        dec    byte ptr [eax + 2]
loc_6_6B66:
        lea    edi, [ebp - 0d8h]
        xor    eax, eax
        mov    ecx, 0dh
        rep stosd
        db  0A0h, 000h, 000h, 000h, 000h
        mov    esi, 1
        mov    byte ptr [ebp - 0d8h], al
        lea    eax, [ebp - 0d8h]
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [ebp - 0ceh], ecx
        mov    edx, dword ptr [ebx + 4]
        mov    word ptr [ebp - 0beh], si
        mov    dword ptr [ebp - 0cah], edx
        mov    dword ptr [ebp - 20h], eax
        mov    dword ptr [ebp - 18h], esi
        lea    eax, [ebp - 20h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   eax
        mov    dword ptr [ebp - 1ch], esi
        mov    dword ptr [ebp - 14h], 0ffh
        mov    word ptr [ebp - 0c4h], 0ffffh
        push   ecx
        push   0
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_6BCE:
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_6_6BD8
        mov    dword ptr [ebp - 10h], esi
loc_6_6BD8:
        push   0
        lea    eax, [ebp - 30h]
        push   0ffh
        push   eax
        call   sub_6_21F0
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_6_6C00
        mov    word ptr [ebp - 8ah], 0fdh
        jmp    NEAR PTR loc_6_6D09
loc_6_6C00:
        lea    eax, [ebp - 108h]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        lea    edx, [ebp - 0a4h]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_6C17:
        add    esp, 0ch
        mov    dword ptr [ebp - 88h], eax
        test   eax, eax
        jne    SHORT loc_6_6C32
        mov    word ptr [ebp - 8ah], 0fdh
        jmp    NEAR PTR loc_6_6D09
loc_6_6C32:
        mov    ax, word ptr [ebx + 10h]
        test   ax, ax
        movsx  eax, ax
        jge    SHORT loc_6_6C40
        neg    eax
loc_6_6C40:
        lea    ecx, [eax*8]
        sub    ecx, eax
        lea    esi, [ecx*4 + 2]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_6C56:
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    SHORT loc_6_6C6E
        mov    word ptr [ebp - 8ah], 8
        jmp    NEAR PTR loc_6_6D09
loc_6_6C6E:
        push   esi
        mov    edx, dword ptr [ebp - 8]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_6C78:
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        test   eax, eax
        je     SHORT loc_6_6CFD
        push   eax
        push   esi
        xor    eax, eax
        lea    edx, [ebp - 0a4h]
        movsx  ecx, word ptr [ebx + 10h]
        mov    ax, word ptr [ebx + 12h]
        push   eax
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        push   40c3h
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_6CA6:
        add    esp, 1ch
        mov    ecx, dword ptr [ebp - 0a4h]
        mov    word ptr [ebx + 10h], cx
        mov    edx, dword ptr [ebp - 80h]
        mov    word ptr [ebx + 12h], dx
        mov    eax, dword ptr [ebx + 0ch]
        xor    ecx, ecx
        mov    edx, dword ptr [ebp - 8]
        mov    cx, word ptr [edx]
        cmp    ecx, eax
        jbe    SHORT loc_6_6CD5
        mov    word ptr [ebp - 8ah], 0eah
        mov    word ptr [edx], ax
loc_6_6CD5:
        xor    eax, eax
        mov    edx, dword ptr [ebp - 8]
        mov    ax, word ptr [edx]
        mov    edi, dword ptr [ebx + 8]
        lea    esi, [edx + 2]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_6CFA:
        add    esp, 4
loc_6_6CFD:
        mov    edx, dword ptr [ebp - 8]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_6D06:
        add    esp, 4
loc_6_6D09:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_6_6D15
        mov    eax, dword ptr [ebp - 4]
        dec    byte ptr [eax + 2]
loc_6_6D15:
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_6_6D4A
        mov    eax, dword ptr [ebx]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 18h], 1
        mov    dword ptr [ebp - 14h], 0ffh
        mov    dword ptr [ebp - 1ch], 3
        mov    dword ptr [ebp - 20h], eax
        lea    eax, [ebp - 20h]
        push   eax
        push   ecx
        push   0
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_6D47:
        add    esp, 0ch
loc_6_6D4A:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    ecx, dword ptr [eax + 20h]
        movsx  eax, word ptr [ebp - 8ah]
        mov    dword ptr [ecx], 14h
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_6D70:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dl, byte ptr [eax + 5ah]
        add    ecx, 10h
        mov    dword ptr [ecx - 8], edx
        mov    byte ptr [ecx], 0
        xor    edx, edx
        push   0ch
        mov    dl, byte ptr [eax + 5fh]
        push   ecx
        mov    dword ptr [ecx - 0ch], edx
        mov    edx, dword ptr [eax + 14h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ecx - 10h], edx
        mov    edx, dword ptr [eax]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_6DA3:
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_6DB0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        mov    ecx, dword ptr [eax + 10h]
        push   edi
        xor    esi, esi
        cmp    dword ptr [ecx], esi
        je     SHORT loc_6_6DCD
        mov    eax, 7ch
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_6DCD:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        test   edi, edi
        mov    ebx, dword ptr [eax + 18h]
        je     SHORT loc_6_6E06
loc_6_6DDD:
        add    esi, 1ch
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1ch], esi
        jb     SHORT loc_6_6E19
        push   edi
        push   ebx
        call   sub_6_6D70
        add    esp, 8
        test   byte ptr [edi + 3], 2
        je     SHORT loc_6_6DFC
        or     dword ptr [ebx + 0ch], 1
loc_6_6DFC:
        mov    edi, dword ptr [edi + 18h]
        add    ebx, 1ch
        test   edi, edi
        jne    SHORT loc_6_6DDD
loc_6_6E06:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 20h]
        test   eax, eax
        je     SHORT loc_6_6E12
        mov    dword ptr [eax], esi
loc_6_6E12:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_6E19:
        mov    eax, 7ah
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_6E30:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   esi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        test   esi, esi
        mov    ecx, dword ptr [eax + 10h]
        je     SHORT loc_6_6E6F
        mov    eax, dword ptr [ecx]
loc_6_6E48:
        cmp    dword ptr [esi + 14h], eax
        je     SHORT loc_6_6E5F
        mov    esi, dword ptr [esi + 18h]
        test   esi, esi
        jne    SHORT loc_6_6E48
        pop    esi
        cmp    edx, 1
        sbb    eax, eax
        pop    ebp
        and    eax, 57h
        ret
loc_6_6E5F:
        mov    eax, dword ptr [ecx + 4]
        cmp    eax, -1
        je     SHORT loc_6_6E6A
        mov    byte ptr [esi + 5fh], al
loc_6_6E6A:
        mov    edx, 1
loc_6_6E6F:
        pop    esi
        cmp    edx, 1
        sbb    eax, eax
        pop    ebp
        and    eax, 57h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_6_6E80:
        push   ebp
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 8
        mov    eax, 2
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   ecx, ecx
        mov    edi, dword ptr [esi + 10h]
        mov    ebx, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 8], ebx
        mov    edx, dword ptr [edi]
        mov    dword ptr [ebp - 4], edx
        je     SHORT loc_6_6F15
        add    edi, 4
loc_6_6EAC:
        lea    edx, [ecx + 23h]
        mov    esi, edi
loc_6_6EB1:
        mov    bl, byte ptr [edx]
        cmp    bl, byte ptr [esi]
        jne    SHORT loc_6_6ED1
        or     bl, bl
        je     SHORT loc_6_6ECD
        mov    bl, byte ptr [edx + 1]
        cmp    bl, byte ptr [esi + 1]
        jne    SHORT loc_6_6ED1
        add    edx, 2
        add    esi, 2
        or     bl, bl
        jne    SHORT loc_6_6EB1
loc_6_6ECD:
        xor    edx, edx
        jmp    SHORT loc_6_6ED6
loc_6_6ED1:
        sbb    edx, edx
        sbb    edx, -1
loc_6_6ED6:
        test   edx, edx
        je     SHORT loc_6_6EE0
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    SHORT loc_6_6EAC
loc_6_6EE0:
        test   ecx, ecx
        je     SHORT loc_6_6F15
        mov    edx, dword ptr [ebp - 4]
        lea    eax, [edx + edx*4]
        lea    eax, [eax + eax*8]
        sub    eax, edx
        lea    edi, [eax + ecx + 3dh]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [ebp - 8]
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        xor    eax, eax
loc_6_6F15:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
sub_6_6F20:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        lea    ecx, [ebp - 8]
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   ecx
        mov    eax, dword ptr [esi + 10h]
        mov    edi, dword ptr [eax]
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_6F3B:
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_6_6F4E
        mov    eax, 57h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_6F4E:
        cmp    dword ptr [esi + 1ch], 1ch
        mov    ebx, dword ptr [esi + 18h]
        jae    SHORT loc_6_6F63
        mov    eax, 7ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_6F63:
        mov    ecx, dword ptr [esi + 20h]
        push   eax
        mov    dword ptr [ebx + 0ch], 0
        push   ebx
        mov    dword ptr [ecx], 1ch
        call   sub_6_6D70
        add    esp, 8
        lea    eax, [ebp - 1]
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_6F87:
        add    esp, 8
        test   al, al
        je     SHORT loc_6_6F92
        or     dword ptr [ebx + 0ch], 1
loc_6_6F92:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_6_6FA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 3ch
        cmp    dword ptr [ebp + 10h], 0
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_6_6FB7
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [edi + 4]
        jmp    SHORT loc_6_6FC9
loc_6_6FB7:
        push   3c04h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_6FC1:
        add    esp, 4
        mov    ebx, eax
        mov    edi, dword ptr [ebp + 0ch]
loc_6_6FC9:
        cmp    dword ptr [ebp + 10h], 0
        mov    eax, dword ptr [ebx]
        jne    SHORT loc_6_6FDA
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_6FD7:
        add    esp, 4
loc_6_6FDA:
        push   eax
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_6FE7:
        add    esp, 8
        lea    eax, [ebp - 3ch]
        mov    dword ptr [esi + 28h], eax
        mov    ax, word ptr [ebx + 2ch]
        cmp    ah, 4ch
        jne    SHORT loc_6_7001
        movzx  ax, al
        mov    word ptr [esi + 6], ax
loc_6_7001:
        mov    dword ptr [ebp - 4], 1
        cmp    byte ptr [edi + 1ch], 54h
        jne    SHORT loc_6_7023
        cmp    word ptr [edi + 14h], 0ffffh
        jne    SHORT loc_6_701F
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_6_7023
loc_6_701F:
        mov    byte ptr [esi + 5], 0
loc_6_7023:
        xor    ecx, ecx
        cmp    word ptr [ebx + 26h], cx
        jne    SHORT loc_6_70A1
        lea    eax, [ebp - 3ch]
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax + 8], ecx
        mov    word ptr [eax + 0ch], cx
        mov    byte ptr [eax + 0eh], cl
        mov    eax, dword ptr [ebx + 4]
        test   eax, eax
        je     SHORT loc_6_706D
        cmp    dword ptr [ebp + 10h], ecx
        jne    SHORT loc_6_7052
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_704F:
        add    esp, 4
loc_6_7052:
        lea    edx, [ebp - 3ch]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    edi, dword ptr [eax + 4]
        mov    dword ptr [edx + 4], edi
        mov    ecx, dword ptr [eax + 8]
        mov    dword ptr [edx + 8], ecx
        mov    ax, word ptr [eax + 0ch]
        mov    word ptr [edx + 0ch], ax
loc_6_706D:
        cmp    dword ptr [ebp - 4], 1
        sbb    eax, eax
        and    eax, 0fffffffdh
        add    eax, 4
        push   eax
        push   1
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_7082:
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_6_70A1
        cmp    word ptr [esi + 1ah], 43h
        jne    NEAR PTR loc_6_724A
        mov    word ptr [esi + 1ah], 85dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_70A1:
        lea    edi, [ebp - 2ch]
        xor    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        mov    dword ptr [ebp - 2ch], esi
        mov    ax, word ptr [ebx + 20h]
        mov    dword ptr [ebp - 24h], 0
        mov    word ptr [ebp - 10h], ax
        mov    cx, word ptr [ebx + 1eh]
        mov    word ptr [ebp - 14h], cx
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        mov    dword ptr [esi], eax
        je     SHORT loc_6_7103
        cmp    dword ptr [ebp + 10h], 0
        mov    eax, dword ptr [ebx + 0ch]
        jne    SHORT loc_6_70E7
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_70E4:
        add    esp, 4
loc_6_70E7:
        push   40h
        mov    dword ptr [esi + 14h], eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_70F2:
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_6_7203
        mov    eax, dword ptr [esi + 50h]
        mov    dword ptr [ebp - 20h], eax
loc_6_7103:
        mov    ax, word ptr [ebx + 22h]
        mov    word ptr [ebp - 0eh], ax
        xor    eax, eax
        mov    ax, word ptr [ebp - 0eh]
        mov    dword ptr [esi], eax
        test   eax, eax
        je     SHORT loc_6_7145
        cmp    dword ptr [ebp + 10h], 0
        mov    eax, dword ptr [ebx + 18h]
        jne    SHORT loc_6_7129
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_7126:
        add    esp, 4
loc_6_7129:
        push   41h
        mov    dword ptr [esi + 14h], eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_7134:
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_6_7203
        mov    eax, dword ptr [esi + 50h]
        mov    dword ptr [ebp - 1ch], eax
loc_6_7145:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_6_71A6
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_6_7156
        mov    edi, dword ptr [ebx + 14h]
        jmp    SHORT loc_6_7164
loc_6_7156:
        mov    eax, dword ptr [ebx + 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_715F:
        add    esp, 4
        mov    edi, eax
loc_6_7164:
        cmp    dword ptr [ebp + 10h], 0
        mov    eax, dword ptr [ebx + 8]
        jne    SHORT loc_6_7176
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_7173:
        add    esp, 4
loc_6_7176:
        push   edi
        lea    ecx, [ebx + 30h]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [ebx + 1ch]
        push   eax
        push   ecx
        xor    eax, eax
        mov    ecx, dword ptr [ebx + 28h]
        mov    ax, word ptr [ebx + 2eh]
        mov    edx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 2ch]
        push   edx
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_71A1:
        add    esp, 28h
        jmp    SHORT loc_6_7203
loc_6_71A6:
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_6_71B1
        mov    edi, dword ptr [ebx + 14h]
        jmp    SHORT loc_6_71BF
loc_6_71B1:
        mov    eax, dword ptr [ebx + 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_71BA:
        add    esp, 4
        mov    edi, eax
loc_6_71BF:
        cmp    dword ptr [ebp + 10h], 0
        mov    eax, dword ptr [ebx + 8]
        jne    SHORT loc_6_71D1
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_71CE:
        add    esp, 4
loc_6_71D1:
        push   edi
        mov    ecx, dword ptr [esi + 0ch]
        push   eax
        lea    edx, [ebx + 30h]
        xor    eax, eax
        mov    ax, word ptr [ebx + 1ch]
        push   eax
        push   ecx
        push   edx
        xor    eax, eax
        mov    ax, word ptr [ebx + 2eh]
        mov    ecx, dword ptr [ebx + 28h]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        lea    eax, [ebp - 2ch]
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_7200:
        add    esp, 2ch
loc_6_7203:
        cmp    dword ptr [ebp - 20h], 0
        je     SHORT loc_6_7215
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_7212:
        add    esp, 4
loc_6_7215:
        cmp    dword ptr [ebp - 1ch], 0
        je     SHORT loc_6_7227
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_7224:
        add    esp, 4
loc_6_7227:
        cmp    word ptr [esi + 1ah], 43h
        jne    SHORT loc_6_7234
        mov    word ptr [esi + 1ah], 85dh
loc_6_7234:
        cmp    dword ptr [ebp - 4], 2
        je     SHORT loc_6_724A
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_7240:
        mov    dword ptr [esi + 1ch], 0
        add    esp, 4
loc_6_724A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_7260:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        xor    ebx, ebx
        call   sub_6_4220
        xor    ecx, ecx
        mov    eax, dword ptr [ebp + 0ch]
        mov    cx, word ptr [eax + 10h]
        cmp    ecx, ebx
        mov    ecx, dword ptr [ebp + 8]
        je     SHORT loc_6_728F
        mov    word ptr [ecx + 1ah], 7ch
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_728F:
        mov    eax, dword ptr [ecx + 14h]
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        test   edi, edi
        je     SHORT loc_6_72E1
        mov    edx, dword ptr [ecx]
        mov    dword ptr [ebp - 4], edx
loc_6_72A1:
        lea    ecx, [esi + 6]
        cmp    ecx, dword ptr [ebp - 4]
        jae    SHORT loc_6_72D7
        mov    dx, word ptr [edi + 58h]
        add    esi, 6
        mov    word ptr [eax + 2], dx
        add    eax, 6
        movzx  cx, byte ptr [edi + 5ah]
        mov    word ptr [eax - 6], cx
        mov    dl, byte ptr [edi + 2]
        mov    cl, byte ptr [edi + 3]
        and    dl, 0fh
        and    cl, 80h
        or     cl, dl
        mov    dl, byte ptr [edi + 63h]
        mov    byte ptr [eax - 2], cl
        mov    byte ptr [eax - 1], dl
loc_6_72D7:
        add    ebx, 6
        mov    edi, dword ptr [edi + 18h]
        test   edi, edi
        jne    SHORT loc_6_72A1
loc_6_72E1:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    ebx, esi
        mov    word ptr [eax + 18h], si
        je     SHORT loc_6_72F9
        mov    word ptr [eax + 14h], bx
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1ah], 6fh
loc_6_72F9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_6_7300:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 10h]
        test   ebx, ebx
        je     SHORT loc_6_7318
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 10h]
        jmp    SHORT loc_6_7325
loc_6_7318:
        push   3810h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_7322:
        add    esp, 4
loc_6_7325:
        push   3cffh
        mov    dword ptr [ebp - 10h], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_7332:
        add    esp, 4
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, 0ffffffffh
        mov    dword ptr [ebp - 0ch], eax
        xor    eax, eax
        mov    edi, dword ptr [ebp - 0ch]
        mov    ax, word ptr [esi + 4]
        add    edi, eax
        sub    eax, eax
        repne scasb
        xor    eax, eax
        mov    edi, dword ptr [ebp - 0ch]
        not    ecx
        mov    ax, word ptr [esi]
        mov    dword ptr [ebp - 18h], ecx
        add    edi, eax
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        xor    eax, eax
        mov    edi, dword ptr [ebp - 0ch]
        not    ecx
        mov    ax, word ptr [esi + 14h]
        mov    dword ptr [ebp - 14h], ecx
        add    edi, eax
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 1ch], ecx
        add    eax, ecx
        add    eax, dword ptr [ebp - 18h]
        add    eax, 3ah
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_7396:
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_6_73B0
        mov    ebx, dword ptr [ebp + 8]
        pop    edi
        mov    word ptr [ebx + 1ah], 8
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_73B0:
        mov    eax, dword ptr [ebp - 10h]
        test   ebx, ebx
        je     SHORT loc_6_73BB
        mov    eax, dword ptr [eax]
        jmp    SHORT loc_6_73C6
loc_6_73BB:
        mov    ecx, dword ptr [eax]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_73C3:
        add    esp, 4
loc_6_73C6:
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, eax
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [ebx + 2ch]
        rep movsd
        mov    ecx, eax
        db  0BAh, 000h, 000h, 000h, 000h
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [ebx + 2ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    eax, dword ptr [edx]
        mov    dword ptr [edi], eax
        mov    ecx, dword ptr [edx + 4]
        mov    dword ptr [edi + 4], ecx
        mov    esi, dword ptr [edx + 8]
        mov    dword ptr [edi + 8], esi
        mov    ax, word ptr [edx + 0ch]
        mov    word ptr [edi + 0ch], ax
        mov    ecx, dword ptr [ebx + 2ch]
        mov    edx, dword ptr [ebx + 0ch]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_7422:
        add    esp, 8
        mov    dword ptr [ebx + 28h], 0
        push   4
        push   1
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_7436:
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_6_7455
        cmp    word ptr [ebx + 1ah], 43h
        jne    NEAR PTR loc_6_7580
        mov    word ptr [ebx + 1ah], 85dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_7455:
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    ecx, 0eh
        rep stosd
        stosw
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 4]
        add    eax, 26h
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [edx], ebx
        mov    cx, word ptr [esi + 18h]
        mov    word ptr [edx + 24h], cx
        mov    ecx, dword ptr [ebp - 8]
        db  0C7h, 042h, 004h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 18h]
        mov    dword ptr [ecx + 10h], eax
        lea    eax, [ecx + 14h]
        mov    edi, eax
        mov    dword ptr [ecx], eax
        xor    ecx, ecx
        mov    cx, word ptr [esi + 4]
        mov    esi, dword ptr [ebp - 0ch]
        add    esi, ecx
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        add    eax, dword ptr [ebp - 18h]
        mov    edx, dword ptr [ebp - 8]
        xor    ecx, ecx
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, eax
        mov    dword ptr [edx + 4], eax
        mov    cx, word ptr [esi]
        mov    edx, dword ptr [ebp - 14h]
        mov    esi, dword ptr [ebp - 0ch]
        add    esi, ecx
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        add    eax, dword ptr [ebp - 14h]
        mov    edx, dword ptr [ebp - 8]
        xor    ecx, ecx
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 8], eax
        mov    edx, dword ptr [ebp - 1ch]
        mov    cx, word ptr [esi + 14h]
        mov    edi, eax
        mov    esi, dword ptr [ebp - 0ch]
        add    esi, ecx
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        db  068h, 000h, 000h, 000h, 000h
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebx + 0ch]
        push   edx
        mov    dword ptr [ebx + 10h], eax
        mov    byte ptr [ebx + 4], 0
        push   1388h
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_752B:
        add    esp, 1ch
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ecx + 0ch]
        test   eax, eax
        je     SHORT loc_6_7541
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_753E:
        add    esp, 4
loc_6_7541:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0ch]
        test   eax, eax
        je     SHORT loc_6_7554
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_7551:
        add    esp, 4
loc_6_7554:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        test   eax, eax
        je     SHORT loc_6_7567
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_7564:
        add    esp, 4
loc_6_7567:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_7570:
        add    esp, 4
        cmp    word ptr [ebx + 1ah], 43h
        jne    SHORT loc_6_7580
        mov    word ptr [ebx + 1ah], 85dh
loc_6_7580:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_7590:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [edx]
        movsx  eax, byte ptr [esi]
        sub    eax, 42h
        cmp    eax, 38h
        ja     SHORT loc_6_75B6
        xor    ecx, ecx
        db  08Ah, 088h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_6_75B6:
        xor    eax, eax
        pop    edi
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 05Ch, 008h
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_75C6:
        mov    eax, 1
        jmp    SHORT loc_6_75D9
loc_6_75CD:
        mov    eax, 4
        jmp    SHORT loc_6_75D9
loc_6_75D4:
        mov    eax, 2
loc_6_75D9:
        xor    edi, edi
        inc    esi
        cmp    byte ptr [esi], 39h
        jg     SHORT loc_6_75F8
loc_6_75E1:
        mov    cl, byte ptr [esi]
        cmp    cl, 30h
        jl     SHORT loc_6_75F8
        movsx  ebx, cl
        lea    ecx, [edi + edi*4]
        inc    esi
        lea    edi, [ebx + ecx*2 - 30h]
        cmp    byte ptr [esi], 39h
        jle    SHORT loc_6_75E1
loc_6_75F8:
        dec    esi
        test   edi, edi
        mov    dword ptr [edx], esi
        je     SHORT loc_6_7609
        imul   eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_7607:
        xor    eax, eax
loc_6_7609:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Ch, 001h, 00Ch, 002h, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch
        db  00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 003h, 00Ch, 00Ch, 00Ch, 00Ch
        db  00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 004h, 00Ch, 005h, 00Ch, 00Ch, 006h, 007h, 008h, 00Ch, 00Ch
        db  009h, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ah, 00Ch, 00Ch, 00Bh, 08Dh
        db  049h, 000h
sub_6_7680:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [edx]
        mov    cl, byte ptr [eax]
        cmp    cl, 5ah
        jg     SHORT loc_6_76D8
        cmp    cl, 4fh
        je     SHORT loc_6_76D8
        movsx  eax, cl
        sub    eax, 42h
        cmp    eax, 15h
        ja     SHORT loc_6_76AF
        xor    ecx, ecx
        db  08Ah, 088h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_6_76AF:
        xor    eax, eax
        pop    ebp
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 05Ch, 008h
        ret
loc_6_76BC:
        push   edx
        call   sub_6_7590
        mov    esp, ebp
        pop    ebp
        ret
loc_6_76C6:
        mov    eax, 4
        pop    ebp
        ret
loc_6_76CD:
        xor    eax, eax
        pop    ebp
        ret
loc_6_76D1:
        mov    eax, 2
        pop    ebp
        ret
loc_6_76D8:
        mov    cl, byte ptr [eax + 1]
        inc    eax
        cmp    cl, 39h
        jg     SHORT loc_6_76E6
        cmp    cl, 30h
        jge    SHORT loc_6_76D8
loc_6_76E6:
        dec    eax
        pop    ebp
        mov    dword ptr [edx], eax
        mov    eax, 4
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 002h, 009h, 009h, 009h, 009h, 009h
        db  009h, 003h, 004h, 009h, 005h, 009h, 006h, 009h, 009h, 009h, 007h, 009h, 009h, 008h, 08Bh, 0FFh
sub_6_7730:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        xor    edi, edi
        mov    dword ptr [esi], 0ffffffffh
        mov    al, byte ptr [eax]
        test   al, al
        je     SHORT loc_6_776F
loc_6_774A:
        cmp    al, 4eh
        jne    SHORT loc_6_7755
        mov    dword ptr [esi], edi
        add    edi, 2
        jmp    SHORT loc_6_7763
loc_6_7755:
        lea    eax, [ebp + 8]
        push   eax
        call   sub_6_7680
        add    esp, 4
        add    edi, eax
loc_6_7763:
        inc    dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        test   al, al
        jne    SHORT loc_6_774A
loc_6_776F:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_7780:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        mov    ebp, esp
        cmp    dword ptr [edx], 0
        jne    SHORT loc_6_7790
        xor    eax, eax
        pop    ebp
        ret
loc_6_7790:
        mov    cx, word ptr [edx]
        mov    eax, 1
        sub    cx, word ptr [ebp + 10h]
        movzx  ecx, cx
        add    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [edx], ecx
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_77B0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 1ch
        mov    eax, dword ptr [ecx + 8]
        mov    edx, dword ptr [ecx + 10h]
        push   ebx
        mov    dword ptr [ebp - 1ch], eax
        push   esi
        xor    eax, eax
        push   edi
        mov    esi, dword ptr [edx + 4]
        mov    ax, word ptr [ecx + 1eh]
        mov    edi, dword ptr [ecx + 14h]
        mov    dword ptr [ebp - 10h], 0
        add    eax, edi
        cmp    dword ptr [ebp + 10h], 0
        mov    dword ptr [ebp - 18h], eax
        je     NEAR PTR loc_6_78BC
        mov    bx, word ptr [ebp + 0ch]
loc_6_77EC:
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [ebp - 0ch], 0
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 8], ecx
        mov    al, byte ptr [ecx]
        test   al, al
        je     SHORT loc_6_784E
loc_6_7802:
        cmp    al, 4eh
        jne    SHORT loc_6_780E
        xor    ecx, ecx
        mov    cx, word ptr [esi]
        mov    dword ptr [ebp - 0ch], ecx
loc_6_780E:
        cmp    al, 5ah
        jle    SHORT loc_6_7834
        push   ebx
        push   esi
        push   edi
        call   sub_6_7780
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_6_7834
        mov    eax, dword ptr [esi]
        cmp    edi, eax
        ja     NEAR PTR loc_6_78C6
        cmp    dword ptr [ebp - 18h], eax
        jb     NEAR PTR loc_6_78C6
loc_6_7834:
        lea    eax, [ebp - 8]
        push   eax
        call   sub_6_7680
        add    esp, 4
        add    esi, eax
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax]
        test   al, al
        jne    SHORT loc_6_7802
loc_6_784E:
        mov    dword ptr [ebp - 14h], 0
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_6_78AD
loc_6_785B:
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    al, byte ptr [ecx]
        test   al, al
        je     SHORT loc_6_78A2
loc_6_786A:
        cmp    al, 5ah
        jle    SHORT loc_6_7888
        push   ebx
        push   esi
        push   edi
        call   sub_6_7780
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_6_7888
        mov    eax, dword ptr [esi]
        cmp    edi, eax
        ja     SHORT loc_6_78D1
        cmp    dword ptr [ebp - 18h], eax
        jb     SHORT loc_6_78D1
loc_6_7888:
        lea    eax, [ebp - 4]
        push   eax
        call   sub_6_7680
        add    esp, 4
        add    esi, eax
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        test   al, al
        jne    SHORT loc_6_786A
loc_6_78A2:
        mov    eax, dword ptr [ebp - 0ch]
        inc    dword ptr [ebp - 14h]
        cmp    eax, dword ptr [ebp - 14h]
        ja     SHORT loc_6_785B
loc_6_78AD:
        inc    dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 10h]
        cmp    eax, dword ptr [ebp + 10h]
        jb     NEAR PTR loc_6_77EC
loc_6_78BC:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_78C6:
        mov    ax, 0dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_78D1:
        mov    ax, 0dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
sub_6_78E0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 2ch
        mov    ecx, dword ptr [eax + 8]
        push   ebx
        mov    ax, word ptr [eax + 18h]
        push   esi
        mov    word ptr [ebp - 6], ax
        push   edi
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 6]
        and    eax, 0ffffh
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 1ch], ecx
        mov    dword ptr [ebp - 2ch], eax
        cmp    eax, edx
        jb     NEAR PTR loc_6_7B01
        mov    eax, dword ptr [ebp + 14h]
        cmp    eax, edx
        je     NEAR PTR loc_6_7B01
        cmp    eax, -1
        je     SHORT loc_6_7958
        mov    ax, word ptr [ebx + eax]
        mov    ecx, ebx
        mov    cx, ax
        imul   cx, word ptr [ebp + 18h]
        add    cx, dx
        mov    word ptr [ebp - 4], ax
        mov    word ptr [ebp - 2], cx
        cmp    cx, word ptr [ebp - 6]
        jbe    SHORT loc_6_794F
        mov    eax, 84bh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_794F:
        mov    dword ptr [ebp - 10h], 2
        jmp    SHORT loc_6_7969
loc_6_7958:
        mov    dword ptr [ebp - 10h], 1
        mov    word ptr [ebp - 4], 0
        mov    word ptr [ebp - 2], dx
loc_6_7969:
        mov    eax, dword ptr [ebp - 2]
        mov    edx, dword ptr [ebp - 1ch]
        mov    dword ptr [ebp - 14h], 1
        and    eax, 0ffffh
        mov    ecx, dword ptr [edx + 4]
        add    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 18h], ecx
        mov    dword ptr [ebp - 20h], 0
        cmp    dword ptr [ebp - 10h], 0
        mov    dword ptr [ebp - 28h], eax
        jle    NEAR PTR loc_6_7AD5
loc_6_7998:
        mov    dword ptr [ebp - 24h], 0
        cmp    dword ptr [ebp - 14h], 0
        jle    NEAR PTR loc_6_7AB2
loc_6_79A9:
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 0ch], eax
        mov    cl, byte ptr [eax]
        test   cl, cl
        je     NEAR PTR loc_6_7AA3
loc_6_79B9:
        cmp    cl, 5ah
        jle    NEAR PTR loc_6_7A85
        mov    edi, dword ptr [ebx]
        test   edi, edi
        jne    SHORT loc_6_79DC
        add    ebx, 4
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_6_7590
        add    esp, 4
        jmp    NEAR PTR loc_6_7A93
loc_6_79DC:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    edi, eax
        jb     SHORT loc_6_7A0E
        add    eax, dword ptr [ebp - 2ch]
        cmp    eax, edi
        jbe    SHORT loc_6_7A0E
        cmp    dword ptr [ebp + 1ch], 0
        je     NEAR PTR loc_6_7AE9
        mov    dword ptr [ebx], 0
        lea    eax, [ebp - 0ch]
        push   eax
        add    ebx, 4
        call   sub_6_7590
        add    esp, 4
        jmp    NEAR PTR loc_6_7A93
loc_6_7A0E:
        movsx  eax, cl
        cmp    eax, 6ch
        je     SHORT loc_6_7A29
        cmp    eax, 7ah
        je     SHORT loc_6_7A30
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_6_7590
        add    esp, 4
        jmp    SHORT loc_6_7A3D
loc_6_7A29:
        xor    eax, eax
        mov    ax, word ptr [edi]
        jmp    SHORT loc_6_7A3D
loc_6_7A30:
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, ecx
loc_6_7A3D:
        add    word ptr [ebp - 2], ax
        mov    cx, word ptr [ebp - 2]
        cmp    cx, word ptr [ebp - 6]
        jbe    SHORT loc_6_7A6A
        mov    ecx, dword ptr [ebp - 1ch]
        cmp    dword ptr [ecx + 0ch], 0
        jne    NEAR PTR loc_6_7AF5
        mov    ecx, dword ptr [ebp - 2]
        mov    edi, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        mov    esi, edi
        sub    ecx, eax
        rep movsb
loc_6_7A6A:
        mov    esi, dword ptr [ebx]
        mov    edi, dword ptr [ebp - 28h]
        mov    ecx, eax
        add    ebx, 4
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        add    dword ptr [ebp - 28h], eax
        jmp    SHORT loc_6_7A93
loc_6_7A85:
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_6_7680
        add    esp, 4
        add    ebx, eax
loc_6_7A93:
        inc    dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 0ch]
        mov    cl, byte ptr [eax]
        test   cl, cl
        jne    NEAR PTR loc_6_79B9
loc_6_7AA3:
        inc    dword ptr [ebp - 24h]
        mov    eax, dword ptr [ebp - 24h]
        cmp    eax, dword ptr [ebp - 14h]
        jl     NEAR PTR loc_6_79A9
loc_6_7AB2:
        mov    ecx, dword ptr [ebp - 1ch]
        inc    dword ptr [ebp - 20h]
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp - 20h]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        cmp    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        jl     NEAR PTR loc_6_7998
loc_6_7AD5:
        mov    cx, word ptr [ebp - 2]
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 18h], cx
        pop    edi
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_7AE9:
        mov    eax, 57h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_7AF5:
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_7B01:
        mov    eax, 84bh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
loc_6_7B10:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 3ch
        mov    eax, dword ptr [ecx + 1ch]
        push   ebx
        push   esi
        mov    dword ptr [ebp - 14h], eax
        push   edi
        xor    eax, eax
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 14h]
        mov    word ptr [ebp - 2], ax
        mov    byte ptr [ebp - 19h], al
        mov    edx, dword ptr [ecx + 10h]
        mov    dword ptr [ebp - 30h], eax
        mov    dword ptr [ebp - 18h], edx
        mov    dword ptr [ebp - 2ch], eax
        mov    ecx, dword ptr [edx + 8]
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 2
        mov    dword ptr [ebp - 24h], ecx
        mov    ebx, dword ptr [ecx + 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 28h], 4
        mov    cx, word ptr [edx + 24h]
        mov    word ptr [eax - 2], cx
        mov    ecx, dword ptr [ebp - 24h]
        mov    dword ptr [ebp - 20h], eax
        sub    eax, eax
        mov    edi, dword ptr [ecx]
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        mov    dword ptr [ebp - 8], ecx
        add    ecx, 2
        mov    dword ptr [ebp - 10h], ecx
        cmp    ecx, 168h
        jbe    SHORT loc_6_7B9A
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        mov    dword ptr [ecx + 20h], 85ch
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_7B9A:
        mov    ecx, dword ptr [ebp - 24h]
        mov    eax, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 0ch]
        mov    esi, dword ptr [ecx]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 24h]
        add    dword ptr [ebp - 0ch], eax
        mov    edi, dword ptr [ecx + 4]
        mov    edx, dword ptr [ebp - 0ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        mov    dword ptr [ebp - 3ch], edx
        repne scasb
        not    ecx
        mov    dword ptr [ebp - 8], ecx
        add    dword ptr [ebp - 10h], ecx
        cmp    dword ptr [ebp - 10h], 168h
        jbe    SHORT loc_6_7BF3
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        mov    dword ptr [ecx + 20h], 85ch
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_7BF3:
        mov    ecx, dword ptr [ebp - 24h]
        mov    eax, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 0ch]
        mov    esi, dword ptr [ecx + 4]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 20h]
        add    dword ptr [ebp - 0ch], eax
        cmp    byte ptr [edx], 0
        je     NEAR PTR loc_6_7EBE
loc_6_7C1F:
        mov    eax, dword ptr [ebp - 20h]
        movsx  eax, byte ptr [eax]
        sub    eax, 44h
        cmp    eax, 36h
        ja     NEAR PTR loc_6_7EAF
        xor    ecx, ecx
        db  08Ah, 088h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_6_7C40:
        add    dword ptr [ebp - 10h], 4
        cmp    dword ptr [ebp - 10h], 168h
        ja     NEAR PTR loc_6_7F1C
        mov    eax, dword ptr [ebx - 4]
        sub    ebx, 4
        mov    edx, dword ptr [ebp - 0ch]
        add    dword ptr [ebp - 0ch], 4
        mov    dword ptr [edx], eax
        jmp    NEAR PTR loc_6_7EAF
loc_6_7C65:
        lea    eax, [ebp - 20h]
        push   eax
        call   sub_6_7590
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        add    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 168h
        ja     NEAR PTR loc_6_7F30
        jmp    NEAR PTR loc_6_7EAC
loc_6_7C89:
        add    dword ptr [ebp - 10h], 2
        mov    ax, word ptr [ebx - 2]
        sub    ebx, 2
        mov    ecx, dword ptr [ebp - 18h]
        cmp    dword ptr [ebp - 10h], 168h
        mov    word ptr [ecx + 1eh], ax
        ja     NEAR PTR loc_6_7F44
        mov    ax, word ptr [ebx]
        mov    edx, dword ptr [ebp - 0ch]
        add    dword ptr [ebp - 0ch], 2
        mov    word ptr [edx], ax
        jmp    NEAR PTR loc_6_7EAF
loc_6_7CBA:
        add    dword ptr [ebp - 10h], 2
        cmp    dword ptr [ebp - 10h], 168h
        ja     NEAR PTR loc_6_7F58
        mov    ax, word ptr [ebx - 2]
        sub    ebx, 2
        or     byte ptr [ebp - 19h], 4
        mov    word ptr [ebp - 2], ax
        mov    edx, dword ptr [ebp - 0ch]
        add    dword ptr [ebp - 0ch], 2
        mov    word ptr [edx], ax
        jmp    NEAR PTR loc_6_7EAF
loc_6_7CE9:
        mov    ax, word ptr [ebx - 2]
        sub    ebx, 2
        mov    ecx, dword ptr [ebp - 18h]
        mov    word ptr [ecx + 18h], ax
        jmp    NEAR PTR loc_6_7EAF
loc_6_7CFC:
        add    dword ptr [ebp - 10h], 2
        cmp    dword ptr [ebp - 10h], 168h
        ja     NEAR PTR loc_6_7F6C
        mov    ax, word ptr [ebx - 2]
        sub    ebx, 2
        mov    edx, dword ptr [ebp - 0ch]
        add    dword ptr [ebp - 0ch], 2
        mov    word ptr [edx], ax
        jmp    NEAR PTR loc_6_7EAF
loc_6_7D23:
        sub    ebx, 4
        mov    ecx, dword ptr [ebp - 14h]
        test   byte ptr [ecx + 4], 1
        je     SHORT loc_6_7D33
        mov    esi, dword ptr [ebx]
        jmp    SHORT loc_6_7D40
loc_6_7D33:
        mov    eax, dword ptr [ebx]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_7D3B:
        add    esp, 4
        mov    esi, eax
loc_6_7D40:
        test   esi, esi
        jne    SHORT loc_6_7D4F
        mov    eax, dword ptr [ebp - 20h]
        mov    byte ptr [eax], 4fh
        jmp    NEAR PTR loc_6_7EAF
loc_6_7D4F:
        lea    eax, [ebp - 20h]
        push   eax
        call   sub_6_7590
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        add    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 168h
        ja     NEAR PTR loc_6_7F80
        mov    edi, dword ptr [ebp - 0ch]
        mov    ecx, eax
        jmp    NEAR PTR loc_6_7E9D
loc_6_7D78:
        add    dword ptr [ebp - 28h], 2
        sub    ebx, 4
        cmp    dword ptr [ebp - 28h], 168h
        jbe    NEAR PTR loc_6_7EAF
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        mov    dword ptr [ecx + 20h], 85ch
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_7DA0:
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [ebx - 4]
        sub    ebx, 4
        test   byte ptr [ecx + 4], 1
        jne    SHORT loc_6_7DB8
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_7DB5:
        add    esp, 4
loc_6_7DB8:
        test   eax, eax
        jne    SHORT loc_6_7DC7
        mov    eax, dword ptr [ebp - 20h]
        mov    byte ptr [eax], 4fh
        jmp    NEAR PTR loc_6_7EAF
loc_6_7DC7:
        lea    eax, [ebp - 20h]
        push   eax
        call   sub_6_7590
        add    esp, 4
        add    dword ptr [ebp - 28h], eax
        cmp    dword ptr [ebp - 28h], 168h
        jbe    NEAR PTR loc_6_7EAF
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        mov    dword ptr [ecx + 20h], 85ch
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_7DF7:
        mov    eax, dword ptr [ebx - 4]
        sub    ebx, 4
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 34h], eax
        or     byte ptr [ebp - 19h], 1
        test   byte ptr [ecx + 4], 4
        je     SHORT loc_6_7E15
        mov    dword ptr [ebp - 2ch], eax
        jmp    NEAR PTR loc_6_7EAF
loc_6_7E15:
        mov    eax, dword ptr [ebp - 34h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_7E1E:
        add    esp, 4
        mov    dword ptr [ebp - 2ch], eax
        jmp    NEAR PTR loc_6_7EAF
loc_6_7E29:
        mov    eax, dword ptr [ebx - 4]
        sub    ebx, 4
        mov    ecx, dword ptr [ebp - 14h]
        or     byte ptr [ebp - 19h], 2
        test   byte ptr [ecx + 4], 4
        je     SHORT loc_6_7E41
        mov    dword ptr [ebp - 30h], eax
        jmp    SHORT loc_6_7EAF
loc_6_7E41:
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_7E47:
        add    esp, 4
        mov    dword ptr [ebp - 30h], eax
        jmp    SHORT loc_6_7EAF
loc_6_7E4F:
        sub    ebx, 4
        mov    ecx, dword ptr [ebp - 14h]
        test   byte ptr [ecx + 4], 1
        je     SHORT loc_6_7E5F
        mov    esi, dword ptr [ebx]
        jmp    SHORT loc_6_7E6C
loc_6_7E5F:
        mov    eax, dword ptr [ebx]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_7E67:
        add    esp, 4
        mov    esi, eax
loc_6_7E6C:
        test   esi, esi
        jne    SHORT loc_6_7E78
        mov    eax, dword ptr [ebp - 20h]
        mov    byte ptr [eax], 4fh
        jmp    SHORT loc_6_7EAF
loc_6_7E78:
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    dword ptr [ebp - 8], ecx
        add    dword ptr [ebp - 10h], ecx
        cmp    dword ptr [ebp - 10h], 168h
        ja     NEAR PTR loc_6_7F94
        mov    eax, ecx
        mov    edi, dword ptr [ebp - 0ch]
loc_6_7E9D:
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebp - 8]
loc_6_7EAC:
        add    dword ptr [ebp - 0ch], eax
loc_6_7EAF:
        inc    dword ptr [ebp - 20h]
        mov    eax, dword ptr [ebp - 20h]
        cmp    byte ptr [eax], 0
        jne    NEAR PTR loc_6_7C1F
loc_6_7EBE:
        test   byte ptr [ebp - 19h], 3
        je     NEAR PTR loc_6_7FF1
        lea    eax, [ebp - 38h]
        mov    ecx, dword ptr [ebp - 3ch]
        push   eax
        push   ecx
        call   sub_6_7730
        add    esp, 8
        mov    ebx, eax
        cmp    dword ptr [ebp - 38h], -1
        je     NEAR PTR loc_6_7FF4
        mov    ecx, dword ptr [ebp - 24h]
        sub    eax, eax
        mov    edi, dword ptr [ecx + 8]
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        mov    dword ptr [ebp - 8], ecx
        add    dword ptr [ebp - 10h], ecx
        cmp    dword ptr [ebp - 10h], 168h
        jbe    NEAR PTR loc_6_7FA8
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        mov    dword ptr [ecx + 20h], 57h
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_7F1C:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        mov    dword ptr [ecx + 20h], 57h
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_7F30:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        mov    dword ptr [ecx + 20h], 57h
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_7F44:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        mov    dword ptr [ecx + 20h], 57h
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_7F58:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        mov    dword ptr [ecx + 20h], 57h
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_7F6C:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        mov    dword ptr [ecx + 20h], 57h
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_7F80:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        mov    dword ptr [ecx + 20h], 57h
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_7F94:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        mov    dword ptr [ecx + 20h], 57h
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_7FA8:
        mov    eax, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp - 8]
        mov    edi, eax
        mov    ecx, dword ptr [ebp - 24h]
        mov    esi, dword ptr [ecx + 8]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    edx, dword ptr [ebp - 8]
        lea    ecx, [ebp - 8]
        push   ecx
        add    dword ptr [ebp - 0ch], edx
        push   eax
        call   sub_6_7730
        add    esp, 8
        cmp    dword ptr [ebp - 8], -1
        je     SHORT loc_6_7FF7
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        mov    dword ptr [ecx + 20h], 57h
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_7FF1:
        mov    ebx, dword ptr [ebp - 1ch]
loc_6_7FF4:
        mov    eax, dword ptr [ebp - 1ch]
loc_6_7FF7:
        test   byte ptr [ebp - 19h], 2
        je     NEAR PTR loc_6_8099
        cmp    word ptr [ebp - 2], 0
        jne    SHORT loc_6_8045
        mov    ecx, dword ptr [ebp - 3ch]
        cmp    byte ptr [ecx], 4bh
        je     SHORT loc_6_8045
        mov    dl, byte ptr [ebp - 19h]
        xor    ecx, ecx
        and    dl, 4
        mov    cl, dl
        mov    edx, dword ptr [ebp - 30h]
        push   ecx
        push   eax
        mov    eax, dword ptr [ebp - 38h]
        mov    ecx, dword ptr [ebp - 18h]
        push   eax
        push   ebx
        push   edx
        push   ecx
        call   sub_6_78E0
        add    esp, 18h
        mov    ecx, dword ptr [ebp + 0ch]
        test   eax, eax
        mov    dword ptr [ecx + 20h], eax
        je     SHORT loc_6_8045
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_8045:
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 18h]
        mov    ax, word ptr [ecx + 18h]
        mov    ecx, dword ptr [ebp - 14h]
        test   eax, eax
        mov    dword ptr [ecx], eax
        je     SHORT loc_6_8099
        mov    ecx, dword ptr [ebp - 24h]
        mov    eax, dword ptr [ecx + 0ch]
        test   eax, eax
        jne    SHORT loc_6_80B7
        mov    eax, dword ptr [ebp - 30h]
        mov    ecx, dword ptr [ebp - 14h]
        test   eax, eax
        mov    dword ptr [ecx + 14h], eax
        je     SHORT loc_6_8099
        push   40h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8076:
        add    esp, 8
        mov    ecx, dword ptr [ebp + 0ch]
        test   eax, eax
        mov    dword ptr [ecx + 20h], eax
        je     SHORT loc_6_808D
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_808D:
        mov    ecx, dword ptr [ebp - 14h]
        mov    edx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [ecx + 50h]
        mov    dword ptr [edx + 0ch], eax
loc_6_8099:
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_80E8
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        mov    dword ptr [ecx + 20h], 85ch
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_80B7:
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 18h]
        mov    dx, word ptr [ecx + 18h]
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_80C7:
        add    esp, 8
        mov    ecx, dword ptr [ebp - 18h]
        test   eax, eax
        mov    dword ptr [ecx + 0ch], eax
        jne    SHORT loc_6_8099
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        mov    dword ptr [ecx + 20h], 8
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_80E8:
        test   byte ptr [ebp - 19h], 1
        je     SHORT loc_6_813E
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 18h]
        mov    ax, word ptr [ecx + 1eh]
        mov    ecx, dword ptr [ebp - 14h]
        test   eax, eax
        mov    dword ptr [ecx], eax
        je     SHORT loc_6_813E
        mov    eax, dword ptr [ebp - 2ch]
        mov    dword ptr [ecx + 14h], eax
        test   eax, eax
        je     SHORT loc_6_813E
        test   byte ptr [ecx + 4], 2
        je     SHORT loc_6_815C
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 18h]
        mov    ax, word ptr [ecx + 1eh]
        mov    edx, dword ptr [ebp - 2ch]
        push   eax
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8123:
        add    esp, 8
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 50h], eax
loc_6_812C:
        mov    ecx, dword ptr [ebp - 14h]
        mov    edx, dword ptr [ebp - 18h]
        mov    ebx, dword ptr [ebp - 34h]
        mov    eax, dword ptr [ecx + 50h]
        mov    dword ptr [edx + 10h], eax
        mov    dword ptr [edx + 14h], ebx
loc_6_813E:
        mov    ax, word ptr [ebp - 28h]
        mov    ecx, dword ptr [ebp - 18h]
        mov    word ptr [ecx + 1ch], ax
        mov    ebx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    dword ptr [ebx + 38h], edx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_815C:
        push   41h
        mov    ecx, dword ptr [ebp - 14h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8167:
        add    esp, 8
        mov    ecx, dword ptr [ebp + 0ch]
        test   eax, eax
        mov    dword ptr [ecx + 20h], eax
        je     SHORT loc_6_812C
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Eh
        db  001h, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 002h, 00Eh, 00Eh, 00Eh, 003h, 00Eh, 00Eh, 00Eh, 004h, 00Eh
        db  00Eh, 005h, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 006h, 00Eh, 007h, 008h
        db  00Eh, 009h, 009h, 009h, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Eh, 00Ah, 00Bh, 00Eh, 00Eh
        db  00Eh, 00Ch, 00Eh, 00Eh, 00Dh, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h
        db  000h, 000h
loc_6_8200:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 18h
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [ecx + 8]
        push   ebx
        mov    dword ptr [ebp - 18h], eax
        push   esi
        mov    eax, dword ptr [edx]
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 8], eax
        xor    eax, eax
        mov    ebx, dword ptr [edx + 10h]
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [esi]
        mov    dword ptr [ebp - 10h], eax
        test   eax, eax
        je     SHORT loc_6_8249
        cmp    eax, 0eah
        je     SHORT loc_6_8249
        cmp    eax, 90dh
        je     SHORT loc_6_8249
        mov    word ptr [ecx + 1eh], 0
loc_6_8249:
        mov    ax, word ptr [esi + 2]
        add    esi, 2
        mov    word ptr [ebp - 2], ax
        mov    ecx, dword ptr [edx + 4]
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], esi
        add    dword ptr [ebp - 0ch], 2
        cmp    byte ptr [ecx], 1
        sbb    edx, edx
        inc    edx
        cmp    byte ptr [eax], 0
        mov    dword ptr [ebp - 14h], edx
        je     NEAR PTR loc_6_8357
loc_6_8273:
        mov    eax, dword ptr [ebp - 8]
        movsx  eax, byte ptr [eax]
        sub    eax, 46h
        cmp    eax, 23h
        ja     SHORT loc_6_8290
        xor    ecx, ecx
        db  08Ah, 088h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_6_8290:
        lea    eax, [ebp - 8]
        push   eax
        call   sub_6_7680
        add    esp, 4
        sub    ebx, eax
        jmp    NEAR PTR loc_6_8348
loc_6_82A3:
        lea    eax, [ebp - 8]
        push   eax
        call   sub_6_7590
        add    esp, 4
        jmp    NEAR PTR loc_6_8348
loc_6_82B4:
        sub    ebx, 4
        mov    eax, dword ptr [ebp - 18h]
        test   byte ptr [eax + 4], 1
        mov    eax, dword ptr [ebx]
        jne    SHORT loc_6_82CB
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_82C8:
        add    esp, 4
loc_6_82CB:
        xor    ecx, ecx
        mov    esi, dword ptr [ebp - 0ch]
        mov    cx, word ptr [esi]
        add    dword ptr [ebp - 0ch], 2
        mov    word ptr [eax], cx
        mov    dword ptr [ebp - 14h], ecx
        jmp    SHORT loc_6_8348
loc_6_82DF:
        sub    ebx, 4
        mov    eax, dword ptr [ebp - 18h]
        test   byte ptr [eax + 4], 1
        je     SHORT loc_6_82EF
        mov    edi, dword ptr [ebx]
        jmp    SHORT loc_6_82FC
loc_6_82EF:
        mov    eax, dword ptr [ebx]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_82F7:
        add    esp, 4
        mov    edi, eax
loc_6_82FC:
        lea    eax, [ebp - 8]
        push   eax
        call   sub_6_7590
        add    esp, 4
        test   edi, edi
        je     SHORT loc_6_8348
        mov    esi, dword ptr [ebp - 0ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [ebp - 8]
        cmp    byte ptr [ecx], 68h
        jne    SHORT loc_6_8345
        mov    esi, dword ptr [ebp - 0ch]
        cmp    word ptr [esi], 0
        jne    SHORT loc_6_8345
        mov    ecx, dword ptr [ebp + 8]
        cmp    word ptr [ecx + 24h], 46h
        jne    SHORT loc_6_8345
        mov    dword ptr [ebp - 10h], 7ch
        mov    word ptr [ecx + 1eh], 0
loc_6_8345:
        add    dword ptr [ebp - 0ch], eax
loc_6_8348:
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        cmp    byte ptr [eax], 0
        jne    NEAR PTR loc_6_8273
loc_6_8357:
        mov    ecx, dword ptr [ebp + 8]
        cmp    word ptr [ecx + 1eh], 0
        je     SHORT loc_6_8380
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [ebp - 2]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   eax
        push   edx
        call   sub_6_77B0
        add    esp, 0ch
        test   ax, ax
        je     SHORT loc_6_8380
        movzx  eax, ax
        mov    dword ptr [ebp - 10h], eax
loc_6_8380:
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_8396
        mov    eax, 85ch
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_8396:
        mov    eax, dword ptr [ebp - 10h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h
        db  003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 001h
        db  003h, 002h, 002h, 002h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_83E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        xor    eax, eax
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    bx, word ptr [esi + 18h]
        mov    ax, word ptr [esi + 1ch]
        cmp    eax, 1180h
        je     SHORT loc_6_840F
        cmp    eax, 118bh
        je     SHORT loc_6_8445
        cmp    eax, 118dh
        je     SHORT loc_6_844E
        jmp    SHORT loc_6_8455
loc_6_840F:
        mov    edi, dword ptr [ebp + 8]
        push   edi
        mov    dword ptr [edi], 0
        call   sub_6_5980
        add    esp, 4
        test   eax, eax
        je     SHORT loc_6_8431
        mov    eax, dword ptr [edi + 2ch]
        mov    word ptr [edi + 1ah], 0
        mov    byte ptr [eax], 0
loc_6_8431:
        mov    word ptr [esi + 18h], 0
        mov    word ptr [esi + 10h], 0
        mov    ecx, dword ptr [edi + 2ch]
        mov    dword ptr [ebp - 4], ecx
        jmp    SHORT loc_6_8455
loc_6_8445:
        db  0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_6_8455
loc_6_844E:
        db  0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h
loc_6_8455:
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        cmp    ecx, ebx
        jae    SHORT loc_6_8469
        mov    ebx, ecx
loc_6_8469:
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_6_8473
        mov    eax, dword ptr [esi]
        jmp    SHORT loc_6_8480
loc_6_8473:
        push   3800h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_847D:
        add    esp, 4
loc_6_8480:
        push   ebx
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_848B:
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_84A0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [ecx + 1ch]
        cmp    eax, 118ch
        je     SHORT loc_6_84C0
        cmp    eax, 118eh
        je     SHORT loc_6_84CC
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_6_84C0:
        mov    edi, 11h
        db  0BEh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_6_84D6
loc_6_84CC:
        mov    edi, 0fh
        db  0BEh, 000h, 000h, 000h, 000h
loc_6_84D6:
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_6_84E1
        mov    eax, dword ptr [ecx + 4]
        jmp    SHORT loc_6_84EE
loc_6_84E1:
        push   3c04h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_84EB:
        add    esp, 4
loc_6_84EE:
        push   edi
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_84F6:
        add    esp, 0ch
        mov    byte ptr [esi + edi], 0
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_8510:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_6_8524
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebx + 4]
        jmp    SHORT loc_6_8534
loc_6_8524:
        push   3c04h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_852E:
        add    esp, 4
        mov    ebx, dword ptr [ebp + 0ch]
loc_6_8534:
        mov    esi, dword ptr [ebp + 8]
        push   24h
        push   eax
        mov    eax, dword ptr [esi + 2ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8543:
        add    esp, 0ch
        mov    edi, dword ptr [esi + 2ch]
        xor    edx, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        mov    byte ptr [edi + 23h], dl
        mov    byte ptr [edi + 24h], dl
        repne scasb
        mov    ax, word ptr [ebx + 14h]
        not    ecx
        test   ax, ax
        lea    edi, [ecx - 1]
        jne    SHORT loc_6_8577
        test   edi, edi
        jne    SHORT loc_6_858B
        test   ax, ax
        jne    SHORT loc_6_8577
        mov    byte ptr [esi + 4], 0
        jmp    SHORT loc_6_857B
loc_6_8577:
        mov    byte ptr [esi + 4], 3
loc_6_857B:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8581:
        add    esp, 4
        cmp    word ptr [esi + 1ah], 0
        jne    SHORT loc_6_85C0
loc_6_858B:
        mov    byte ptr [esi + 4], 0
        test   edi, edi
        je     SHORT loc_6_85A4
        mov    eax, dword ptr [esi + 2ch]
        push   esi
        add    eax, edi
        inc    eax
        mov    dword ptr [esi + 28h], eax
        call   sub_6_5760
        jmp    SHORT loc_6_85AA
loc_6_85A4:
        push   esi
        call   sub_6_5900
loc_6_85AA:
        add    esp, 4
        cmp    word ptr [esi + 1ah], 0
        jne    SHORT loc_6_85C0
        mov    eax, dword ptr [esi + 2ch]
        push   eax
        call   sub_6_A5B0
        add    esp, 4
loc_6_85C0:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
VREDIR_Service_09:
        push   ebp
        mov    ebp, esp
        sub    esp, 190h
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   esi
        lea    ebx, [ebp - 190h]
        push   edi
        add    eax, 4
        push   0
        push   0ffh
        push   eax
        call   sub_6_21F0
        add    esp, 0ch
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     NEAR PTR loc_6_88AD
        test   byte ptr [eax + 1dh], 80h
        jne    NEAR PTR loc_6_88AD
        mov    edi, ebx
        mov    ecx, 1dh
        push   242h
        dec    byte ptr [eax + 2]
        xor    eax, eax
        rep stosd
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8625:
        add    esp, 4
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 18h], eax
        test   eax, eax
        jne    SHORT loc_6_863E
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_863E:
        mov    byte ptr [ebx + 4], 0
        mov    eax, dword ptr [ebp + 14h]
        push   ebx
        mov    byte ptr [ebx + 5], 2
        mov    dword ptr [ebx + 28h], 0
        mov    word ptr [ebx + 1ah], 0
        mov    ecx, dword ptr [eax + 10h]
        mov    dword ptr [ebx + 2ch], ecx
        call   sub_6_5760
        add    esp, 4
        cmp    word ptr [ebx + 1ah], 0
        jne    NEAR PTR loc_6_8896
        mov    eax, dword ptr [ebp - 0ch]
        db  08Bh, 00Dh, 008h, 000h, 000h, 000h
        add    eax, 208h
        mov    dword ptr [ebp - 4], eax
        lea    edx, [eax + 26h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 2ch], eax
        mov    ecx, dword ptr [ebp + 14h]
        sub    eax, eax
        mov    edi, dword ptr [ecx + 18h]
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        db  08Bh, 03Dh, 008h, 000h, 000h, 000h
        add    edi, 30h
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [ebp - 18h]
        mov    eax, dword ptr [ebp + 14h]
        add    eax, 14h
        mov    dword ptr [ebx + 0ch], ecx
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [edx + 10h], eax
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    ecx, 9
        db  0C7h, 002h, 000h, 000h, 000h, 000h
        db  0C7h, 042h, 004h, 000h, 000h, 000h, 000h
        db  0C7h, 042h, 008h, 000h, 000h, 000h, 000h
        rep stosd
        stosw
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 24h], 3bh
        mov    dword ptr [ecx], ebx
        add    ecx, 26h
        db  0BFh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 10h], ecx
        sub    eax, eax
        mov    dword ptr [edx + 8], ecx
        db  0C7h, 041h, 0DEh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 0ch], 0
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [ebx + 0ch], ecx
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        mov    esi, edi
        shr    ecx, 2
        lea    edi, [ebp - 11ch]
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, 0ffffffffh
        mov    edi, dword ptr [eax]
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        mov    esi, edi
        mov    ecx, 0ffffffffh
        lea    edi, [ebp - 11ch]
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        db  0BAh, 000h, 000h, 000h, 000h
        and    ecx, 3
        rep movsb
        lea    edi, [ebp - 11ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    eax, dword ptr [edx]
        mov    ecx, dword ptr [edx + 4]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [edi + 4], ecx
        mov    dx, word ptr [edx + 0ch]
        lea    ecx, [ebp - 11ch]
        push   ecx
        mov    dword ptr [edi + 8], eax
        mov    word ptr [edi + 0ch], dx
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_87B4:
        mov    byte ptr [ebx + 4], 4
        mov    word ptr [ebx + 18h], 1
        add    esp, 8
        mov    word ptr [ebx + 6], 0ffh
        push   ebx
        call   sub_6_1A70
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_6_8885
        mov    ecx, dword ptr [ebp - 4]
        db  0BAh, 000h, 000h, 000h, 000h
        mov    byte ptr [ebx + 4], 7
        mov    dword ptr [ebx + 10h], ecx
        lea    ecx, [ebp - 11ch]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 4]
        push   ecx
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [edx + 8]
        mov    dl, byte ptr [edx + 0ch]
        mov    dword ptr [ecx + 8], eax
        mov    byte ptr [ecx + 0ch], dl
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_880D:
        add    esp, 8
        mov    ecx, dword ptr [ebx + 0ch]
        mov    edx, dword ptr [ebp - 4]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        push   1388h
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   edx
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_882F:
        add    esp, 1ch
        mov    ecx, dword ptr [ebp - 10h]
        mov    eax, dword ptr [ecx + 0ch]
        test   eax, eax
        je     SHORT loc_6_8845
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8842:
        add    esp, 4
loc_6_8845:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 0ch]
        test   eax, eax
        je     SHORT loc_6_8858
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8855:
        add    esp, 4
loc_6_8858:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 10h]
        test   eax, eax
        je     SHORT loc_6_8876
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8868:
        add    esp, 4
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [ecx + 1eh]
        mov    dword ptr [ebx], eax
loc_6_8876:
        push   2
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   ebx
        call   sub_6_30C0
        add    esp, 0ch
loc_6_8885:
        mov    byte ptr [ebx + 5], 2
        mov    byte ptr [ebx + 4], 0
        push   ebx
        call   sub_6_5900
        add    esp, 4
loc_6_8896:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_889F:
        add    esp, 4
        movsx  eax, word ptr [ebx + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_88AD:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_6_88B9
        mov    eax, dword ptr [ebp - 8]
        dec    byte ptr [eax + 2]
loc_6_88B9:
        mov    eax, 5
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
_PTEXT ends
        end
