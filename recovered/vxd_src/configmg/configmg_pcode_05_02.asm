; configmg_pcode_05_02.asm -- CONFIGMG object 5 segment _PTEXT  (part 2/4)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_5_44B6:
        push   ebp
        mov    ebp, esp
        sub    esp, 10ch
        push   ebx
        push   esi
        push   edi
        mov    ebx, edx
        mov    esi, ecx
        test   esi, esi
        je     SHORT loc_5_4515
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 10ch]
        call   sub_5_C486
        lea    edx, [ebp - 0ch]
        mov    ecx, esi
        call   sub_5_30A3
        lea    edx, [ebp - 0ch]
        lea    ecx, [ebp - 10ch]
        call   sub_5_C44E
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 10ch]
        push   eax
        push   ecx
        push   80000002h
        call   sub_5_D164
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_451C
        mov    eax, 1dh
        jmp    SHORT loc_5_456B
loc_5_4515:
        mov    dword ptr [ebp - 4], 80000005h
loc_5_451C:
        mov    dword ptr [ebp - 0bh], 4
        lea    eax, [ebp - 0bh]
        push   eax
        push   ebx
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        call   sub_5_D11C
        add    esp, 18h
        mov    edi, eax
        cmp    eax, 3f4h
        je     SHORT loc_5_454A
        cmp    eax, 2
        jne    SHORT loc_5_454E
loc_5_454A:
        xor    edi, edi
        mov    dword ptr [ebx], edi
loc_5_454E:
        test   esi, esi
        je     SHORT loc_5_455D
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
loc_5_455D:
        mov    eax, 0
        cmp    edi, 1
        adc    eax, -1
        and    eax, 1dh
loc_5_456B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_4572:
        push   ebp
        mov    ebp, esp
        sub    esp, 110h
        push   esi
        mov    dword ptr [ebp - 8], edx
        push   edi
        mov    esi, ecx
        test   esi, esi
        je     SHORT loc_5_45D1
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 110h]
        call   sub_5_C486
        lea    edx, [ebp - 10h]
        mov    ecx, esi
        call   sub_5_30A3
        lea    edx, [ebp - 10h]
        lea    ecx, [ebp - 110h]
        call   sub_5_C44E
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 110h]
        push   eax
        push   ecx
        push   80000002h
        call   sub_5_D164
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_45D8
        mov    eax, 1dh
        jmp    SHORT loc_5_4611
loc_5_45D1:
        mov    dword ptr [ebp - 4], 80000005h
loc_5_45D8:
        push   4
        lea    eax, [ebp - 8]
        push   eax
        push   3
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        call   sub_5_D0EC
        add    esp, 18h
        mov    edi, eax
        test   esi, esi
        je     SHORT loc_5_4603
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
loc_5_4603:
        mov    eax, 0
        cmp    edi, 1
        adc    eax, -1
        and    eax, 1dh
loc_5_4611:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_5_4617:
        push   ebp
        mov    ebp, esp
        sub    esp, 84h
        push   ebx
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 8], 6eh
        push   esi
        push   edi
        mov    esi, edx
        mov    edi, ecx
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   80000002h
        call   sub_5_D154
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_5_4782
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 84h]
        push   eax
        xor    ebx, ebx
        push   ecx
        push   ebx
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        call   sub_5_D11C
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_5_4777
        cmp    dword ptr [ebp - 8], 6eh
        jne    NEAR PTR loc_5_4777
        cmp    edi, -1
        jne    SHORT loc_5_4700
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 14h]
        mov    dword ptr [ebp - 8], 5
        push   eax
        push   ecx
        push   ebx
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        call   sub_5_D11C
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_5_4777
        xor    dx, dx
        lea    ecx, [ebp - 14h]
        mov    eax, 4
loc_5_46B6:
        mov    word ptr [ebp - 0ch], 37h
        movsx  bx, byte ptr [ecx]
        shl    dx, 4
        cmp    byte ptr [ecx], 39h
        jg     SHORT loc_5_46CF
        mov    word ptr [ebp - 0ch], 30h
loc_5_46CF:
        sub    dx, word ptr [ebp - 0ch]
        inc    ecx
        add    dx, bx
        dec    eax
        jne    SHORT loc_5_46B6
        xor    ecx, ecx
        cmp    word ptr [ebp - 84h], cx
        jbe    SHORT loc_5_4700
        lea    eax, [ebp - 78h]
loc_5_46E8:
        cmp    word ptr [eax], dx
        je     SHORT loc_5_46FE
        add    eax, 0ch
        inc    ecx
        movzx  ebx, word ptr [ebp - 84h]
        cmp    ebx, ecx
        ja     SHORT loc_5_46E8
        jmp    SHORT loc_5_4700
loc_5_46FE:
        mov    edi, ecx
loc_5_4700:
        movzx  eax, word ptr [ebp - 84h]
        cmp    eax, edi
        ja     SHORT loc_5_471D
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        mov    eax, 23h
        jmp    SHORT loc_5_4787
loc_5_471D:
        imul   edi, edi, 0ch
        mov    cx, word ptr [ebp + edi - 78h]
        lea    edi, [ebp + edi - 82h]
        push   dword ptr [ebp - 4]
        movzx  eax, cx
        lea    ebx, [esi + 4]
        mov    dword ptr [esi], eax
        push   ebx
        push   ecx
        call   sub_5_30B6
        add    esp, 0ch
        xor    ecx, ecx
        cmp    dword ptr [edi], ecx
        jne    SHORT loc_5_4761
        mov    eax, dword ptr [edi + 4]
        test   eax, eax
        jne    SHORT loc_5_4753
        mov    dword ptr [esi + 54h], ecx
        jmp    SHORT loc_5_4768
loc_5_4753:
        cmp    eax, 1
        jne    SHORT loc_5_4761
        mov    dword ptr [esi + 54h], 1
        jmp    SHORT loc_5_4768
loc_5_4761:
        mov    dword ptr [esi + 54h], 2
loc_5_4768:
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        xor    eax, eax
        jmp    SHORT loc_5_4787
loc_5_4777:
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
loc_5_4782:
        mov    eax, 1dh
loc_5_4787:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_478E:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    cl, byte ptr [eax]
        test   cl, 80h
        je     SHORT loc_5_47A3
        movzx  eax, word ptr [eax + 1]
        add    eax, 3
        jmp    SHORT loc_5_47AB
loc_5_47A3:
        xor    eax, eax
        mov    al, cl
        and    eax, 7
        inc    eax
loc_5_47AB:
        ret
sub_5_47AC:
        and    ecx, 1fh
        mov    eax, 1ah
        cmp    eax, ecx
        sbb    eax, eax
        inc    eax
        cmp    ecx, 1
        sbb    ecx, ecx
        inc    ecx
        and    eax, ecx
        ret
sub_5_47C2:
        push   esi
        push   edi
        mov    eax, ecx
        mov    esi, ecx
        shr    ax, 0dh
        and    ecx, 3
        movzx  edx, ax
        lea    ecx, [edx + ecx*8]
        call   sub_5_47AC
        mov    edi, eax
        mov    ecx, esi
        shr    ecx, 8
        call   sub_5_47AC
        and    edi, eax
        mov    ecx, esi
        shr    ecx, 2
        and    esi, 80h
        call   sub_5_47AC
        and    edi, eax
        cmp    esi, 1
        sbb    eax, eax
        neg    eax
        and    edi, eax
        mov    eax, edi
        pop    edi
        pop    esi
        ret
sub_5_4808:
        push   ebx
        mov    byte ptr [ecx], 2ah
        mov    al, dl
        inc    ecx
        shr    al, 2
        mov    bl, dl
        add    al, 40h
        and    bl, 3
        mov    byte ptr [ecx], al
        inc    ecx
        mov    ax, dx
        shr    ax, 0dh
        lea    ebx, [eax + ebx*8]
        mov    ax, dx
        add    bl, 40h
        shr    ax, 8
        mov    byte ptr [ecx], bl
        shr    edx, 10h
        inc    ecx
        and    al, 1fh
        add    al, 40h
        mov    byte ptr [ecx], al
        inc    ecx
        movzx  ebx, dl
        movzx  eax, dh
        push   eax
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_5_D0FC
        add    esp, 10h
        pop    ebx
        ret
sub_5_4855:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    edi, edx
        jne    SHORT loc_5_48CA
        test   esi, 8000h
        jne    SHORT loc_5_4873
        db  0FFh, 005h, 000h, 000h, 000h, 000h
loc_5_4873:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        test   ecx, ecx
        je     SHORT loc_5_4893
        push   dword ptr [esp + 10h]
        push   edi
        mov    edx, esi
        call   sub_5_1761
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 2
        jmp    SHORT loc_5_48CC
loc_5_4893:
        mov    ecx, 0
        db  0A1h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        adc    ecx, -1
        and    ecx, 3
        mov    ebx, dword ptr [eax + ecx*4 + 30h]
        test   ebx, ebx
        je     SHORT loc_5_48CA
loc_5_48B2:
        mov    edx, esi
        mov    ecx, ebx
        push   dword ptr [esp + 10h]
        push   edi
        call   sub_5_1761
        test   eax, eax
        je     SHORT loc_5_48D2
        mov    ebx, dword ptr [ebx]
        test   ebx, ebx
        jne    SHORT loc_5_48B2
loc_5_48CA:
        xor    eax, eax
loc_5_48CC:
        pop    edi
        pop    esi
        pop    ebx
        ret    4
loc_5_48D2:
        mov    eax, 2
        jmp    SHORT loc_5_48CC
loc_5_48D9:
        sub    esp, 8
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   esi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_4932
        movzx  eax, byte ptr [esi + 2]
        mov    cl, byte ptr [esi + 1]
        push   eax
        xor    eax, eax
        mov    al, cl
        lea    edx, [esp + 8]
        shr    cl, 4
        and    eax, 0fh
        push   eax
        movzx  eax, cl
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        call   sub_5_D0FC
        lea    ecx, [esp + 18h]
        add    esp, 14h
        xor    edx, edx
        push   0
        push   0
        push   ecx
        push   1
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_5_886D
loc_5_4932:
        cmp    byte ptr [esi + 1], 10h
        sbb    eax, eax
        pop    esi
        and    eax, 1fh
        add    esp, 8
        ret
loc_5_4940:
        sub    esp, 0ch
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   esi
        je     SHORT loc_5_4981
        db  0A1h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [eax + 1]
        mov    ecx, esi
        call   sub_5_47C2
        test   eax, eax
        mov    eax, 0
        je     SHORT loc_5_4983
        lea    ecx, [esp + 4]
        mov    edx, esi
        call   sub_5_4808
        lea    edx, [esp + 4]
        push   1
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_21CF
loc_5_4981:
        xor    eax, eax
loc_5_4983:
        pop    esi
        add    esp, 0ch
        ret
loc_5_4988:
        sub    esp, 0ch
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   esi
        je     SHORT loc_5_49C9
        db  0A1h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [eax + 1]
        mov    ecx, esi
        call   sub_5_47C2
        test   eax, eax
        mov    eax, 0
        je     SHORT loc_5_49CB
        lea    ecx, [esp + 4]
        mov    edx, esi
        call   sub_5_4808
        lea    edx, [esp + 4]
        push   0
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_21CF
loc_5_49C9:
        xor    eax, eax
loc_5_49CB:
        pop    esi
        add    esp, 0ch
        ret
loc_5_49D0:
        sub    esp, 104h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        push   edi
        je     NEAR PTR loc_5_4A90
        lea    eax, [esp + 0ch]
        push   0
        lea    ecx, [esp + 14h]
        push   eax
        push   ecx
        db  0B8h, 000h, 000h, 000h, 000h
        mov    dword ptr [esp + 18h], 100h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 0FFh
        push   1
        je     SHORT loc_5_4A0F
        db  0B8h, 000h, 000h, 000h, 000h
loc_5_4A0F:
        push   eax
        xor    edx, edx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_87D6
        test   eax, eax
        je     SHORT loc_5_4A90
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        push   201h
        movzx  esi, word ptr [ebx + 1]
        lea    eax, [esi + 1]
        push   eax
        call   sub_5_D134
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_5_4A49
        mov    eax, 2
        jmp    SHORT loc_5_4A92
loc_5_4A49:
        push   esi
        lea    edx, [ebx + 3]
        mov    ecx, edi
        call   sub_5_5B5D
        push   0
        db  0B8h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 0FFh
        push   0
        push   edi
        push   1
        je     SHORT loc_5_4A6E
        db  0B8h, 000h, 000h, 000h, 000h
loc_5_4A6E:
        push   eax
        xor    edx, edx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_886D
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        jne    SHORT loc_5_4A90
        push   0
        push   edi
        call   sub_5_D0C8
        add    esp, 8
loc_5_4A90:
        xor    eax, eax
loc_5_4A92:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 104h
        ret
loc_5_4A9C:
        push   ebp
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ebp, esp
        inc    ecx
        sub    esp, 8
        cmp    word ptr [ecx], 0
        push   edi
        jne    SHORT loc_5_4ABB
        push   0
        xor    edx, edx
        mov    ecx, 8004h
        jmp    SHORT loc_5_4AF0
loc_5_4ABB:
        lea    edi, [ebp - 8]
        xor    eax, eax
        stosd
        stosd
        db  039h, 005h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [ecx]
        je     SHORT loc_5_4AE2
        cmp    ax, 1
        je     SHORT loc_5_4AE6
loc_5_4AD3:
        shr    ax, 1
        inc    word ptr [ebp - 6]
        cmp    ax, 1
        jne    SHORT loc_5_4AD3
        jmp    SHORT loc_5_4AE6
loc_5_4AE2:
        mov    word ptr [ebp - 4], ax
loc_5_4AE6:
        push   8
        lea    edx, [ebp - 8]
        mov    ecx, 4
loc_5_4AF0:
        call   sub_5_4855
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret
loc_5_4AFA:
        push   ebp
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ebp, esp
        inc    ecx
        sub    esp, 4
        cmp    byte ptr [ecx], 0
        push   edi
        jne    SHORT loc_5_4B18
        push   0
        xor    edx, edx
        mov    ecx, 8003h
        jmp    SHORT loc_5_4B44
loc_5_4B18:
        lea    edi, [ebp - 4]
        xor    eax, eax
        stosd
        db  039h, 005h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ecx]
        je     SHORT loc_5_4B37
        cmp    al, 1
        je     SHORT loc_5_4B3A
loc_5_4B2C:
        shr    al, 1
        inc    byte ptr [ebp - 3]
        cmp    al, 1
        jne    SHORT loc_5_4B2C
        jmp    SHORT loc_5_4B3A
loc_5_4B37:
        mov    byte ptr [ebp - 2], al
loc_5_4B3A:
        push   4
        lea    edx, [ebp - 4]
        mov    ecx, 3
loc_5_4B44:
        call   sub_5_4855
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret
sub_5_4B4E:
        push   esi
        cmp    edx, ecx
        je     SHORT loc_5_4B93
        mov    esi, dword ptr [esp + 8]
        test   esi, esi
        je     SHORT loc_5_4B93
        lea    eax, [esi - 1]
        test   eax, esi
        jne    SHORT loc_5_4B6A
        test   eax, ecx
        jne    SHORT loc_5_4B6A
        test   eax, edx
        je     SHORT loc_5_4B93
loc_5_4B6A:
        cmp    edx, ecx
        jb     SHORT loc_5_4B83
        mov    eax, edx
        sub    edx, edx
        sub    eax, ecx
        div    esi
        inc    eax
        cmp    eax, 100h
        mov    eax, 0
        jb     SHORT loc_5_4B98
loc_5_4B83:
        mov    edx, 9
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_731F
loc_5_4B93:
        mov    eax, 1
loc_5_4B98:
        pop    esi
        ret    4
sub_5_4B9C:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        mov    word ptr [ebp - 2], dx
        push   ebx
        mov    word ptr [ebp - 4], cx
        push   esi
        and    word ptr [ebp - 2], 3ffh
        mov    dx, word ptr [ebp - 2]
        push   edi
        and    word ptr [ebp - 4], 3ffh
        cmp    word ptr [ebp - 4], dx
        ja     NEAR PTR loc_5_4CEF
        cmp    dx, 100h
        jb     NEAR PTR loc_5_4CEF
        mov    ecx, 0
        db  0A1h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        adc    ecx, -1
        and    ecx, 3
        mov    esi, dword ptr [eax + ecx*4 + 30h]
        test   esi, esi
        je     NEAR PTR loc_5_4CEF
loc_5_4BF5:
        mov    edx, 2
        mov    ecx, esi
        call   sub_5_184D
        test   eax, eax
        je     NEAR PTR loc_5_4CE5
loc_5_4C09:
        mov    di, word ptr [eax + 22h]
        lea    edx, [eax + 1ch]
        mov    cx, word ptr [ebp - 2]
        cmp    di, cx
        jb     SHORT loc_5_4C1C
        mov    di, cx
loc_5_4C1C:
        mov    cx, word ptr [edx + 4]
        mov    bx, word ptr [ebp - 4]
        cmp    cx, bx
        ja     SHORT loc_5_4C2C
        mov    cx, bx
loc_5_4C2C:
        mov    word ptr [ebp - 6], 0
        cmp    di, cx
        sbb    ecx, ecx
        inc    ecx
        cmp    word ptr [edx], 0
        mov    dword ptr [ebp - 10h], ecx
        je     SHORT loc_5_4CA4
        mov    cx, word ptr [edx]
        mov    word ptr [ebp - 14h], cx
loc_5_4C48:
        movzx  ecx, word ptr [ebp - 6]
        mov    bx, word ptr [ebp - 4]
        imul   ecx, ecx, 0ch
        add    ecx, edx
        mov    di, word ptr [ecx + 10h]
        cmp    bx, di
        ja     SHORT loc_5_4C61
        mov    bx, di
loc_5_4C61:
        mov    di, word ptr [ecx + 12h]
        mov    word ptr [ebp - 0ch], di
        mov    di, word ptr [ebp - 2]
        cmp    word ptr [ebp - 0ch], di
        jb     SHORT loc_5_4C77
        mov    word ptr [ebp - 0ch], di
loc_5_4C77:
        cmp    bx, word ptr [ebp - 0ch]
        ja     SHORT loc_5_4C96
        movzx  edi, word ptr [ecx + 12h]
        movzx  ebx, word ptr [ecx + 0eh]
        movzx  ecx, word ptr [ecx + 10h]
        sub    edi, ebx
        sub    edi, ecx
        cmp    edi, -1
        jne    SHORT loc_5_4C96
        or     byte ptr [ebp - 10h], 1
loc_5_4C96:
        mov    cx, word ptr [ebp - 14h]
        inc    word ptr [ebp - 6]
        cmp    word ptr [ebp - 6], cx
        jb     SHORT loc_5_4C48
loc_5_4CA4:
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_5_4CD1
        mov    byte ptr [edx + 0ah], 0
        mov    byte ptr [edx + 0bh], 0
        xor    di, di
        cmp    word ptr [edx], di
        je     SHORT loc_5_4CD1
loc_5_4CBA:
        movzx  ebx, di
        imul   ebx, ebx, 0ch
        add    ebx, edx
        inc    di
        mov    byte ptr [ebx + 16h], 0
        mov    byte ptr [ebx + 17h], 0
        cmp    word ptr [edx], di
        ja     SHORT loc_5_4CBA
loc_5_4CD1:
        mov    edx, 2
        mov    ecx, eax
        call   sub_5_1877
        test   eax, eax
        jne    NEAR PTR loc_5_4C09
loc_5_4CE5:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    NEAR PTR loc_5_4BF5
loc_5_4CEF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_4CF6:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        lea    esi, [ebx + 7]
        cmp    byte ptr [esi], 0
        jne    SHORT loc_5_4D20
        push   0
        xor    edx, edx
        mov    ecx, 8002h
        call   sub_5_4855
        jmp    NEAR PTR loc_5_4EE2
loc_5_4D20:
        lea    edi, [ebp - 24h]
        xor    eax, eax
        mov    ecx, 6
        rep stosd
        mov    word ptr [ebp - 22h], 0ch
        mov    ax, 1
        db  066h, 02Bh, 005h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 24h], ax
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_4D74
        lea    ecx, [ebx + 2]
        mov    ax, word ptr [ecx]
        mov    word ptr [ebp - 20h], ax
        movzx  ax, byte ptr [esi]
        add    ax, word ptr [ecx]
        dec    ax
        mov    word ptr [ebp - 1eh], ax
        test   byte ptr [ebx + 1], 1
        jne    SHORT loc_5_4D6F
        mov    byte ptr [ebp - 1ah], 4
        mov    byte ptr [ebp - 19h], 3
loc_5_4D6F:
        mov    cx, word ptr [ecx]
        jmp    SHORT loc_5_4DF1
loc_5_4D74:
        movzx  eax, byte ptr [ebx + 6]
        mov    dx, word ptr [ebx + 2]
        push   eax
        movzx  ecx, word ptr [ebx + 4]
        mov    word ptr [ebp - 0ch], dx
        mov    dword ptr [ebp - 4], eax
        movzx  edi, dx
        mov    edx, ecx
        mov    dword ptr [ebp - 8], ecx
        mov    ecx, edi
        call   sub_5_4B4E
        test   eax, eax
        je     NEAR PTR loc_5_4E24
        cmp    byte ptr [ebx + 6], 0
        je     SHORT loc_5_4DB5
        movzx  ax, byte ptr [ebx + 6]
        dec    ax
        not    ax
        mov    word ptr [ebp - 18h], ax
        jmp    SHORT loc_5_4DBB
loc_5_4DB5:
        mov    word ptr [ebp - 18h], 0ffffh
loc_5_4DBB:
        movzx  ax, byte ptr [esi]
        mov    word ptr [ebp - 16h], ax
        mov    cx, word ptr [ebx + 2]
        mov    word ptr [ebp - 14h], cx
        movzx  ax, byte ptr [esi]
        add    ax, word ptr [ebx + 4]
        dec    ax
        mov    word ptr [ebp - 12h], ax
        test   byte ptr [ebx + 1], 1
        jne    SHORT loc_5_4DE7
        mov    byte ptr [ebp - 0eh], 4
        mov    byte ptr [ebp - 0dh], 3
loc_5_4DE7:
        mov    cx, word ptr [ebx + 2]
        cmp    word ptr [ebx + 4], cx
        jne    SHORT loc_5_4E06
loc_5_4DF1:
        cmp    cx, 400h
        jbe    SHORT loc_5_4E06
        movzx  dx, byte ptr [esi]
        add    dx, cx
        dec    dx
        call   sub_5_4B9C
loc_5_4E06:
        mov    ecx, 2
        lea    edx, [ebp - 24h]
        mov    eax, ecx
        db  02Bh, 005h, 000h, 000h, 000h, 000h
        imul   eax, eax, 0ch
        push   eax
        call   sub_5_4855
        jmp    NEAR PTR loc_5_4EE2
loc_5_4E24:
        mov    eax, dword ptr [ebp - 8]
        sub    eax, edi
        cdq
        idiv   dword ptr [ebp - 4]
        inc    eax
        push   201h
        mov    dword ptr [ebp - 4], eax
        inc    eax
        imul   eax, eax, 0ch
        push   eax
        mov    dword ptr [ebp - 8], eax
        call   sub_5_D134
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_5_4E63
        mov    edx, 3
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_731F
        mov    eax, 2
        jmp    SHORT loc_5_4EE2
loc_5_4E63:
        mov    word ptr [edi + 2], 0ch
        cmp    dword ptr [ebp - 4], 0
        mov    ax, word ptr [ebp - 4]
        mov    dx, word ptr [ebp - 0ch]
        mov    word ptr [edi], ax
        je     SHORT loc_5_4EBB
        lea    ecx, [edi + 0ch]
        mov    eax, dword ptr [ebp - 4]
        movzx  si, byte ptr [esi]
        mov    dword ptr [ebp - 4], eax
loc_5_4E87:
        mov    word ptr [ecx], 0ffffh
        lea    eax, [esi + edx]
        dec    ax
        mov    word ptr [ecx + 2], si
        test   byte ptr [ebx + 1], 1
        mov    word ptr [ecx + 4], dx
        mov    word ptr [ecx + 6], ax
        jne    SHORT loc_5_4EAB
        mov    byte ptr [ecx + 0ah], 4
        mov    byte ptr [ecx + 0bh], 3
loc_5_4EAB:
        movzx  ax, byte ptr [ebx + 6]
        add    dx, ax
        add    ecx, 0ch
        dec    dword ptr [ebp - 4]
        jne    SHORT loc_5_4E87
loc_5_4EBB:
        mov    edx, edi
        mov    ecx, 2
        push   dword ptr [ebp - 8]
        call   sub_5_4855
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        mov    esi, eax
        jne    SHORT loc_5_4EE0
        push   0
        push   edi
        call   sub_5_D0C8
        add    esp, 8
loc_5_4EE0:
        mov    eax, esi
loc_5_4EE2:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_4EE9:
        push   ebp
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        lea    esi, [edx + 3]
        cmp    byte ptr [esi], 0
        jne    SHORT loc_5_4F0E
        push   0
        xor    edx, edx
        mov    ecx, 8002h
        jmp    NEAR PTR loc_5_4FBF
loc_5_4F0E:
        lea    edi, [ebp - 1ch]
        xor    eax, eax
        mov    ecx, 6
        rep stosd
        mov    di, word ptr [edx + 1]
        cmp    di, 100h
        jae    SHORT loc_5_4F2C
        xor    bl, bl
        mov    byte ptr [ebp - 1], al
        jmp    SHORT loc_5_4F50
loc_5_4F2C:
        cmp    di, 400h
        jae    SHORT loc_5_4F3B
        mov    byte ptr [ebp - 1], 3
        mov    bl, 4
        jmp    SHORT loc_5_4F50
loc_5_4F3B:
        xor    bl, bl
        mov    ecx, edi
        movzx  dx, byte ptr [esi]
        add    dx, di
        mov    byte ptr [ebp - 1], bl
        dec    dx
        call   sub_5_4B9C
loc_5_4F50:
        mov    ax, 1
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 1ah], 0ch
        sub    ax, dx
        test   edx, edx
        mov    word ptr [ebp - 1ch], ax
        je     SHORT loc_5_4F87
        mov    word ptr [ebp - 18h], di
        mov    cl, byte ptr [ebp - 1]
        movzx  ax, byte ptr [esi]
        add    ax, di
        mov    byte ptr [ebp - 12h], bl
        dec    ax
        mov    byte ptr [ebp - 11h], cl
        mov    word ptr [ebp - 16h], ax
        jmp    SHORT loc_5_4FAF
loc_5_4F87:
        mov    word ptr [ebp - 10h], 0ffffh
        movzx  ax, byte ptr [esi]
        mov    word ptr [ebp - 0eh], ax
        mov    cl, byte ptr [ebp - 1]
        mov    word ptr [ebp - 0ch], di
        movzx  ax, byte ptr [esi]
        add    ax, di
        mov    byte ptr [ebp - 6], bl
        dec    ax
        mov    byte ptr [ebp - 5], cl
        mov    word ptr [ebp - 0ah], ax
loc_5_4FAF:
        mov    ecx, 2
        mov    eax, ecx
        sub    eax, edx
        lea    edx, [ebp - 1ch]
        imul   eax, eax, 0ch
        push   eax
loc_5_4FBF:
        call   sub_5_4855
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_4FCB:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   esi
        push   edi
        jne    NEAR PTR loc_5_50B0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_4FED
        mov    eax, 1fh
        jmp    NEAR PTR loc_5_50B2
loc_5_4FED:
        call   sub_5_478E
        cmp    eax, 1
        jbe    SHORT loc_5_500D
        db  0A1h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 1]
        cmp    al, 3
        jb     SHORT loc_5_500F
        mov    eax, 1fh
        jmp    NEAR PTR loc_5_50B2
loc_5_500D:
        mov    al, 1
loc_5_500F:
        movzx  eax, al
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        imul   ecx, ecx, 3
        add    eax, ecx
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        db  08Bh, 034h, 085h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_5064
        mov    edx, 0beefdeadh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_14E6
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_5_504A
        mov    eax, 2
        jmp    SHORT loc_5_50B2
loc_5_504A:
        push   0
        mov    edx, edi
        push   0
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   esi
        call   sub_5_1674
        db  089h, 03Dh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_5_50B0
loc_5_5064:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_5074
        mov    eax, 1fh
        jmp    SHORT loc_5_50B2
loc_5_5074:
        mov    edx, 0beefdeadh
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        mov    ecx, dword ptr [eax + 30h]
        call   sub_5_14E6
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_5_50A0
        mov    eax, 2
        jmp    SHORT loc_5_50B2
loc_5_50A0:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 30h]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 0ch], esi
loc_5_50B0:
        xor    eax, eax
loc_5_50B2:
        pop    edi
        pop    esi
        ret
loc_5_50B5:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_50E6
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_50CE
        mov    eax, 1fh
        jmp    SHORT loc_5_50E8
loc_5_50CE:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        mov    eax, 1fh
        je     SHORT loc_5_50E8
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_5_50E6:
        xor    eax, eax
loc_5_50E8:
        ret
sub_5_50E9:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        cmp    dword ptr [ebp + 0ch], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, edx
        mov    ebx, ecx
        jne    SHORT loc_5_510F
        push   0
        xor    edx, edx
        mov    ecx, 8001h
        call   sub_5_4855
        jmp    NEAR PTR loc_5_524F
loc_5_510F:
        lea    edi, [ebp - 24h]
        xor    eax, eax
        mov    ecx, 9
        rep stosd
        mov    word ptr [ebp - 22h], 14h
        mov    ax, 1
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        sub    ax, dx
        test   edx, edx
        mov    word ptr [ebp - 24h], ax
        je     SHORT loc_5_514C
        mov    cx, word ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [ebp - 18h], cx
        add    eax, esi
        dec    eax
        mov    dword ptr [ebp - 20h], ebx
        mov    dword ptr [ebp - 1ch], eax
        jmp    SHORT loc_5_5192
loc_5_514C:
        mov    edx, esi
        mov    ecx, ebx
        push   dword ptr [ebp + 8]
        call   sub_5_4B4E
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_5_51AB
        cmp    dword ptr [ebp + 8], 0
        je     SHORT loc_5_5173
        mov    eax, dword ptr [ebp + 8]
        dec    eax
        not    eax
        mov    dword ptr [ebp - 14h], eax
        jmp    SHORT loc_5_517A
loc_5_5173:
        mov    dword ptr [ebp - 14h], 0ffffffffh
loc_5_517A:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 0ch], ebx
        mov    dword ptr [ebp - 10h], ecx
        lea    eax, [esi + ecx]
        mov    cx, word ptr [ebp + 10h]
        dec    eax
        mov    word ptr [ebp - 4], cx
        mov    dword ptr [ebp - 8], eax
loc_5_5192:
        imul   edx, edx, -14h
        add    edx, 24h
        mov    ecx, 1
        push   edx
        lea    edx, [ebp - 24h]
        call   sub_5_4855
        jmp    NEAR PTR loc_5_524F
loc_5_51AB:
        sub    esi, ebx
        sub    edx, edx
        mov    eax, esi
        push   201h
        div    dword ptr [ebp + 8]
        lea    edi, [eax + 1]
        mov    eax, edi
        imul   eax, eax, 14h
        add    eax, 10h
        push   eax
        call   sub_5_D134
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_5_51EA
        mov    edx, 3
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_731F
        mov    eax, 2
        jmp    SHORT loc_5_524F
loc_5_51EA:
        mov    word ptr [esi], di
        test   edi, edi
        mov    word ptr [esi + 2], 0ch
        je     SHORT loc_5_5222
        lea    eax, [esi + 10h]
        mov    edx, edi
loc_5_51FC:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], 0ffffffffh
        mov    dword ptr [eax + 4], ecx
        add    eax, 14h
        add    ecx, ebx
        mov    dword ptr [eax - 0ch], ebx
        dec    ecx
        add    ebx, dword ptr [ebp + 8]
        dec    edx
        mov    dword ptr [eax - 8], ecx
        mov    cx, word ptr [ebp + 10h]
        mov    word ptr [eax - 4], cx
        jne    SHORT loc_5_51FC
loc_5_5222:
        mov    eax, edi
        mov    edx, esi
        imul   eax, eax, 14h
        add    eax, 10h
        mov    ecx, 1
        push   eax
        call   sub_5_4855
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        mov    edi, eax
        jne    SHORT loc_5_524D
        push   0
        push   esi
        call   sub_5_D0C8
        add    esp, 8
loc_5_524D:
        mov    eax, edi
loc_5_524F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_5_5258:
        push   esi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [esi + 8]
        test   ax, ax
        je     SHORT loc_5_526D
        movzx  eax, ax
        jmp    SHORT loc_5_5272
loc_5_526D:
        mov    eax, 10000h
loc_5_5272:
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 3]
        movzx  edx, word ptr [esi + 6]
        shl    edx, 8
        and    ecx, 1
        push   ecx
        movzx  ecx, word ptr [esi + 0ah]
        shl    ecx, 8
        push   ecx
        push   eax
        movzx  ecx, word ptr [esi + 4]
        shl    ecx, 8
        call   sub_5_50E9
        pop    esi
        ret
loc_5_5299:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        xor    eax, eax
        mov    al, byte ptr [ecx + 3]
        mov    edx, dword ptr [ecx + 8]
        and    eax, 1
        or     al, 2
        push   eax
        push   dword ptr [ecx + 10h]
        push   dword ptr [ecx + 0ch]
        mov    ecx, dword ptr [ecx + 4]
        call   sub_5_50E9
        ret
loc_5_52BC:
        db  0A1h, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        mov    edx, dword ptr [eax + 4]
        mov    cl, byte ptr [eax + 3]
        and    ecx, 1
        or     cl, 2
        push   ecx
        mov    ecx, edx
        push   dword ptr [eax + 8]
        push   0
        call   sub_5_50E9
        ret
loc_5_52DD:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        call   sub_5_478E
        cmp    eax, 1
        ja     SHORT loc_5_52F5
        xor    eax, eax
        jmp    SHORT loc_5_5317
loc_5_52F5:
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0
        je     SHORT loc_5_5317
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    byte ptr [eax + 1], 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 1fh
loc_5_5317:
        ret
sub_5_5318:
        push   ebx
        db  0A1h, 000h, 000h, 000h, 000h
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [eax + 30h]
        test   eax, eax
        je     SHORT loc_5_537C
        mov    edi, dword ptr [eax]
        test   edi, edi
        je     SHORT loc_5_537C
        xor    ebx, ebx
        mov    ebp, dword ptr [eax + 10h]
        cmp    ebp, ebx
        je     SHORT loc_5_5365
loc_5_5337:
        mov    esi, edi
        test   edi, edi
        je     SHORT loc_5_535D
loc_5_533D:
        mov    edx, ebx
        mov    ecx, esi
        call   sub_5_189A
        test   eax, eax
        je     SHORT loc_5_5383
        mov    eax, dword ptr [eax + 4]
        xor    eax, dword ptr [ebp + 4]
        test   eax, 0ffff7fffh
        jne    SHORT loc_5_5383
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    SHORT loc_5_533D
loc_5_535D:
        inc    ebx
        mov    ebp, dword ptr [ebp + 10h]
        test   ebp, ebp
        jne    SHORT loc_5_5337
loc_5_5365:
        test   edi, edi
        je     SHORT loc_5_537C
loc_5_5369:
        mov    edx, ebx
        mov    ecx, edi
        call   sub_5_189A
        test   eax, eax
        jne    SHORT loc_5_538A
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    SHORT loc_5_5369
loc_5_537C:
        xor    eax, eax
loc_5_537E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_5383:
        mov    eax, 1
        jmp    SHORT loc_5_537E
loc_5_538A:
        mov    eax, 1
        jmp    SHORT loc_5_537E
sub_5_5391:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    edi, edx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, 1
        lea    eax, [ecx - 3]
        lea    ebx, [ecx - 2]
        cmp    eax, edx
        sbb    eax, eax
        neg    eax
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    ebx, edx
        sbb    eax, eax
        dec    ecx
        neg    eax
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    ecx, edx
        sbb    eax, eax
        neg    eax
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_5_5413
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_53E9
        db  0A1h, 000h, 000h, 000h, 000h
        imul   eax, eax, 0ch
        db  08Bh, 090h, 000h, 000h, 000h, 000h
loc_5_53E9:
        push   0
        mov    eax, 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        adc    eax, -1
        mov    ecx, edi
        and    eax, 3
        push   eax
        call   sub_5_16B4
        test   eax, eax
        jne    SHORT loc_5_5413
        mov    eax, 2
        jmp    NEAR PTR loc_5_56F8
loc_5_5413:
        xor    ebx, ebx
        db  089h, 035h, 000h, 000h, 000h, 000h
        xor    esi, esi
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        db  089h, 03Dh, 000h, 000h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], ebx
        db  088h, 01Dh, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_5_5454:
        inc    dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], 100h
        ja     NEAR PTR loc_5_556E
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   0
        push   0
        push   1
        push   eax
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        db  0A1h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_5_556E
        test   byte ptr [eax], 80h
        je     SHORT loc_5_54A5
        push   0
        push   0
        push   0
        push   3
        push   eax
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_5_556E
loc_5_54A5:
        call   sub_5_478E
        push   0
        mov    dword ptr [ebp - 4], eax
        push   0
        push   0
        push   eax
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        db  0A0h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_5_556E
        xor    edx, edx
        cmp    dword ptr [ebp - 4], edx
        je     SHORT loc_5_54EA
loc_5_54D6:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    al, byte ptr [ecx + edx]
        inc    edx
        db  0A2h, 000h, 000h, 000h, 000h
        cmp    edx, dword ptr [ebp - 4]
        jb     SHORT loc_5_54D6
loc_5_54EA:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    al, byte ptr [ecx]
        test   al, 80h
        jne    SHORT loc_5_54F8
        and    al, 0f8h
loc_5_54F8:
        test   al, al
        je     SHORT loc_5_556E
        cmp    al, 0ffh
        je     SHORT loc_5_556E
        cmp    al, 10h
        jne    SHORT loc_5_550A
        db  0FFh, 005h, 000h, 000h, 000h, 000h
loc_5_550A:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 0FFh
        je     SHORT loc_5_5522
        mov    ecx, dword ptr [ebp + 8]
        db  03Bh, 00Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_5_5522
        cmp    al, 78h
        jne    SHORT loc_5_5552
loc_5_5522:
        db  080h, 03Dh, 004h, 000h, 000h, 000h, 000h
        db  0B9h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_5552
loc_5_5530:
        cmp    byte ptr [ecx + 4], al
        je     SHORT loc_5_5540
        add    ecx, 6
        cmp    byte ptr [ecx + 4], 0
        jne    SHORT loc_5_5530
        jmp    SHORT loc_5_5552
loc_5_5540:
        movzx  eax, byte ptr [ecx + 5]
        mov    esi, 1fh
        cmp    eax, dword ptr [ebp - 4]
        ja     SHORT loc_5_5552
        call   dword ptr [ecx]
        mov    esi, eax
loc_5_5552:
        test   esi, esi
        jne    SHORT loc_5_5573
        mov    eax, dword ptr [ebp - 4]
        db  001h, 005h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_5_5454
        jmp    SHORT loc_5_5573
loc_5_556E:
        mov    esi, 1fh
loc_5_5573:
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, 1
        db  0A1h, 000h, 000h, 000h, 000h
        db  039h, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, edx
        adc    ecx, -1
        add    eax, edx
        cmp    eax, edx
        sbb    eax, eax
        add    eax, edx
        and    ecx, eax
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp + 8], edx
        sbb    ebx, ebx
        add    eax, edx
        add    ebx, edx
        cmp    eax, edx
        sbb    eax, eax
        neg    eax
        and    ebx, eax
        or     ecx, ebx
        je     SHORT loc_5_55B4
        mov    esi, 14h
loc_5_55B4:
        cmp    esi, 2
        je     SHORT loc_5_55C0
        cmp    esi, 1fh
        je     SHORT loc_5_55C7
        jmp    SHORT loc_5_55D3
loc_5_55C0:
        mov    edx, 3
        jmp    SHORT loc_5_55CC
loc_5_55C7:
        mov    edx, 9
loc_5_55CC:
        mov    ecx, edi
        call   sub_5_731F
loc_5_55D3:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_5_56F6
        db  0B9h, 000h, 000h, 000h, 000h
        call   sub_5_1596
        test   esi, esi
        jne    SHORT loc_5_55F7
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_5612
loc_5_55F7:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        mov    eax, 0
        adc    eax, -1
        and    eax, 3
        lea    ecx, [edi + eax*4 + 30h]
        call   sub_5_15ED
loc_5_5612:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_5643
        call   sub_5_5318
        test   eax, eax
        je     SHORT loc_5_5636
        push   0
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   400000h
        call   sub_5_1906
loc_5_5636:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_5_56F6
loc_5_5643:
        mov    edi, dword ptr [edi + 3ch]
        test   edi, edi
        je     NEAR PTR loc_5_56F6
        mov    edx, 3
        mov    ecx, edi
        call   sub_5_184D
        test   eax, eax
        jne    NEAR PTR loc_5_56F6
        mov    edx, 1
        mov    ecx, edi
        call   sub_5_184D
        test   eax, eax
        jne    NEAR PTR loc_5_56F6
        mov    edx, 2
        mov    ecx, edi
        call   sub_5_184D
        mov    ecx, eax
        test   ecx, ecx
        je     SHORT loc_5_56F6
        mov    ax, word ptr [ecx + 20h]
        cmp    ax, 3f8h
        je     SHORT loc_5_56A4
        cmp    ax, 2f8h
        je     SHORT loc_5_56A4
        cmp    ax, 3e8h
        je     SHORT loc_5_56A4
        cmp    ax, 2e8h
        jne    SHORT loc_5_56F6
loc_5_56A4:
        movzx  edx, word ptr [ecx + 22h]
        movzx  eax, ax
        sub    edx, eax
        cmp    edx, 7
        jne    SHORT loc_5_56F6
        mov    edx, 2
        call   sub_5_1877
        test   eax, eax
        jne    SHORT loc_5_56F6
        mov    edx, 4
        mov    ecx, edi
        call   sub_5_184D
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_5_56F6
        mov    ax, word ptr [edi + 1eh]
        cmp    ax, 4
        je     SHORT loc_5_56E2
        cmp    ax, 3
        jne    SHORT loc_5_56F6
loc_5_56E2:
        mov    edx, 4
        mov    ecx, edi
        call   sub_5_1877
        test   eax, eax
        jne    SHORT loc_5_56F6
        or     byte ptr [edi + 1ch], 1
loc_5_56F6:
        mov    eax, esi
loc_5_56F8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_5_5701:
        push   esi
        mov    esi, ecx
        test   byte ptr [ecx + 0bh], 80h
        jne    SHORT loc_5_570E
        xor    eax, eax
        jmp    SHORT loc_5_5722
loc_5_570E:
        call   sub_5_D17C
        test   eax, eax
        je     SHORT loc_5_5722
loc_5_5717:
        cmp    dword ptr [eax + 5], esi
        je     SHORT loc_5_5722
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    SHORT loc_5_5717
loc_5_5722:
        pop    esi
        ret
sub_5_5724:
        push   esi
        push   0
        mov    esi, ecx
        push   0
        push   dword ptr [esp + 10h]
        push   esi
        push   22h
        push   edx
        call   sub_5_D184
        cmp    dword ptr [esp + 8], 1
        mov    ecx, eax
        jne    SHORT loc_5_5767
        cmp    ecx, 21h
        jne    SHORT loc_5_5767
        push   4000000h
        mov    ecx, esi
        push   0
        call   sub_5_1906
        mov    edx, 17h
        mov    ecx, esi
        call   sub_5_731F
        mov    eax, 13h
        jmp    SHORT loc_5_5772
loc_5_5767:
        push   esi
        mov    edx, 13h
        call   sub_5_1DBE
loc_5_5772:
        pop    esi
        ret    4
sub_5_5776:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, ecx
        push   ebp
        db  0BEh, 000h, 000h, 000h, 000h
        cmp    edx, 8
        mov    ebp, edx
        lea    edi, [esp + 10h]
        mov    eax, 0
        movsd
        movsd
        ja     SHORT loc_5_57B5
        mov    ecx, ebx
        call   sub_5_C430
        lea    ecx, [esp + 10h]
        push   ebp
        mov    edx, ebx
        call   sub_5_5B5D
        lea    eax, [esp + 10h]
        push   eax
        push   0
        call   sub_5_D1B0
loc_5_57B5:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_5_57BD:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        call   sub_5_C3C4
        test   eax, eax
        jne    SHORT loc_5_57D6
        xor    eax, eax
        jmp    SHORT loc_5_584E
loc_5_57D6:
        cmp    byte ptr [esi], 2ah
        jne    SHORT loc_5_57E8
        lea    edx, [eax - 1]
        lea    ecx, [esi + 1]
        call   sub_5_5776
        jmp    SHORT loc_5_584E
loc_5_57E8:
        push   1
        lea    eax, [ebp - 8]
        push   esi
        lea    ecx, [ebp - 4]
        push   eax
        push   ecx
        call   sub_5_D1C8
        test   eax, eax
        je     SHORT loc_5_584B
        cmp    eax, 3
        je     SHORT loc_5_581B
        cmp    eax, 6
        je     SHORT loc_5_581F
        cmp    eax, 7
        je     SHORT loc_5_581B
        mov    edx, 14h
        mov    ecx, edi
        call   sub_5_731F
        xor    eax, eax
        jmp    SHORT loc_5_584E
loc_5_581B:
        xor    eax, eax
        jmp    SHORT loc_5_584E
loc_5_581F:
        mov    edx, 2eh
        mov    ecx, esi
        call   sub_5_C3E0
        cmp    eax, -1
        mov    edx, eax
        jne    SHORT loc_5_583D
        mov    ecx, esi
        call   sub_5_C3C4
        mov    edx, eax
        jmp    SHORT loc_5_5841
loc_5_583D:
        mov    byte ptr [esi + edx], 0
loc_5_5841:
        mov    ecx, esi
        call   sub_5_5776
        mov    dword ptr [ebp - 4], eax
loc_5_584B:
        mov    eax, dword ptr [ebp - 4]
loc_5_584E:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_5_5854:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, esi
        mov    edi, esi
        call   sub_5_C3C4
        add    eax, esi
        mov    dword ptr [ebp - 8], eax
loc_5_586E:
        mov    edx, 20h
        mov    ecx, edi
        call   sub_5_C3E0
        cmp    eax, -1
        je     SHORT loc_5_5889
        mov    byte ptr [edi + eax], 0
        lea    edi, [edi + eax + 1]
        jmp    SHORT loc_5_586E
loc_5_5889:
        mov    edi, esi
loc_5_588B:
        mov    edx, 2ch
        mov    ecx, edi
        call   sub_5_C3E0
        cmp    eax, -1
        je     SHORT loc_5_58E0
        mov    byte ptr [edi + eax], 0
        lea    edi, [edi + eax + 1]
        jmp    SHORT loc_5_588B
loc_5_58A6:
        push   4
        call   sub_5_D134
        add    esp, 8
loc_5_58B0:
        test   eax, eax
        je     NEAR PTR loc_5_5946
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, edi
        push   dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 4ch], eax
        mov    dword ptr [eax + ebx*4 - 4], edi
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_5_5724
        test   eax, eax
        jne    NEAR PTR loc_5_5958
        mov    ecx, esi
        call   sub_5_C3C4
        add    esi, eax
loc_5_58E0:
        cmp    byte ptr [esi], 0
        jne    SHORT loc_5_58F0
loc_5_58E5:
        cmp    dword ptr [ebp - 8], esi
        jbe    SHORT loc_5_593B
        inc    esi
        cmp    byte ptr [esi], 0
        je     SHORT loc_5_58E5
loc_5_58F0:
        cmp    dword ptr [ebp - 8], esi
        jbe    SHORT loc_5_593B
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, esi
        call   sub_5_57BD
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_5_593F
        mov    ecx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 0ch]
        push   201h
        mov    ebx, dword ptr [eax + ecx*4 + 58h]
        lea    edx, [eax + ecx*4]
        inc    ebx
        mov    dword ptr [ebp - 4], edx
        cmp    ebx, 1
        mov    dword ptr [edx + 58h], ebx
        je     SHORT loc_5_58A6
        lea    eax, [ebx*4]
        push   eax
        push   dword ptr [edx + 4ch]
        call   sub_5_D0C0
        add    esp, 0ch
        jmp    NEAR PTR loc_5_58B0
loc_5_593B:
        xor    eax, eax
        jmp    SHORT loc_5_5958
loc_5_593F:
        mov    eax, 0ch
        jmp    SHORT loc_5_5958
loc_5_5946:
        mov    edx, 3
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_5_731F
        mov    eax, 2
loc_5_5958:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_595F:
        push   ebx
        push   esi
        push   edi
        mov    esi, 3
        push   ebp
        lea    edi, [ecx + 58h]
loc_5_596B:
        mov    ebx, dword ptr [edi]
        test   ebx, ebx
        je     SHORT loc_5_59AE
        mov    eax, dword ptr [edi - 0ch]
        lea    ebp, [eax + ebx*4 - 4]
loc_5_5978:
        push   80000000h
        push   0
        push   0
        push   50h
        push   dword ptr [ebp]
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_5_59A8
        mov    ecx, dword ptr [ebp]
        call   sub_5_5701
        test   eax, eax
        je     SHORT loc_5_59A8
        push   dword ptr [eax + 0bh]
        push   0
        call   sub_5_D1EC
loc_5_59A8:
        sub    ebp, 4
        dec    ebx
        jne    SHORT loc_5_5978
loc_5_59AE:
        add    edi, 4
        dec    esi
        jne    SHORT loc_5_596B
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_59B9:
        db  0B8h, 000h, 000h, 000h, 000h
        push   0
        push   4006h
        mov    dword ptr [ecx + 18h], eax
        mov    dword ptr [ecx + 1ch], eax
        call   sub_5_1906
        ret
sub_5_59D1:
        push   ebp
        mov    eax, 4000000h
        mov    ebp, esp
        sub    esp, 108h
        push   esi
        push   edi
        mov    esi, ecx
        push   eax
        push   0
        push   eax
        push   0
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_5A05
        push   1
        mov    eax, dword ptr [esi + 50h]
        mov    ecx, esi
        mov    edx, dword ptr [eax]
        call   sub_5_5724
        jmp    NEAR PTR loc_5_5AF5
loc_5_5A05:
        mov    eax, dword ptr [esi + 4]
        cmp    byte ptr [eax], 0
        je     NEAR PTR loc_5_5B50
        lea    eax, [ebp - 8]
        xor    edx, edx
        push   eax
        mov    ecx, esi
        call   sub_5_8D6E
        test   eax, eax
        jne    SHORT loc_5_5A33
        mov    edx, 1
        mov    ecx, esi
        call   sub_5_731F
        jmp    NEAR PTR loc_5_5B57
loc_5_5A33:
        test   byte ptr [ebp - 8], 4
        je     SHORT loc_5_5A44
        push   0
        mov    ecx, esi
        push   10h
        call   sub_5_1906
loc_5_5A44:
        xor    edi, edi
        lea    eax, [ebp - 4]
        push   edi
        xor    edx, edx
        push   eax
        mov    ecx, esi
        push   edi
        mov    dword ptr [ebp - 4], edi
        push   1
        db  068h, 000h, 000h, 000h, 000h
        call   sub_5_87D6
        test   eax, eax
        je     SHORT loc_5_5A71
        cmp    eax, 25h
        je     NEAR PTR loc_5_5B50
        jmp    NEAR PTR loc_5_5B57
loc_5_5A71:
        push   1
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 108h]
        push   ecx
        xor    edx, edx
        push   1
        mov    ecx, esi
        mov    dword ptr [ebp - 4], 100h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_5_87D6
        test   eax, eax
        je     SHORT loc_5_5AB9
        cmp    eax, 1dh
        je     NEAR PTR loc_5_5B3B
        cmp    eax, 25h
        je     NEAR PTR loc_5_5B50
        cmp    eax, 26h
        je     NEAR PTR loc_5_5B42
        jmp    NEAR PTR loc_5_5B57
loc_5_5AB9:
        lea    ecx, [ebp - 108h]
        mov    edx, 20h
        call   sub_5_C3E0
        cmp    eax, -1
        jne    SHORT loc_5_5B2D
        lea    ecx, [ebp - 108h]
        mov    edx, 2ch
        call   sub_5_C3E0
        cmp    eax, -1
        jne    SHORT loc_5_5B2D
        push   1
        lea    eax, [ebp - 108h]
        push   esi
        push   eax
        call   sub_5_5854
        add    esp, 0ch
loc_5_5AF5:
        cmp    eax, 0ch
        jne    SHORT loc_5_5B08
        mov    edx, 8
        mov    ecx, esi
        call   sub_5_731F
        jmp    SHORT loc_5_5B57
loc_5_5B08:
        test   eax, eax
        jne    SHORT loc_5_5B1F
        push   0
        mov    ecx, esi
        push   2
        push   0
        push   2
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_5B57
loc_5_5B1F:
        mov    edx, 2
        mov    ecx, esi
        call   sub_5_731F
        jmp    SHORT loc_5_5B57
loc_5_5B2D:
        mov    edx, 8
        mov    ecx, esi
        call   sub_5_731F
        jmp    SHORT loc_5_5B57
loc_5_5B3B:
        mov    edx, 13h
        jmp    SHORT loc_5_5B47
loc_5_5B42:
        mov    edx, 4
loc_5_5B47:
        mov    ecx, esi
        call   sub_5_731F
        jmp    SHORT loc_5_5B57
loc_5_5B50:
        mov    ecx, esi
        call   sub_5_59B9
loc_5_5B57:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_5_5B5D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    dword ptr [ebp - 8], edx
        push   edi
        mov    dword ptr [ebp - 4], ecx
        mov    esi, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        cmp    edi, esi
        jbe    SHORT loc_5_5B8C
        mov    eax, esi
        add    eax, ecx
        cmp    edi, eax
        jae    SHORT loc_5_5B8C
        add    esi, ecx
        add    edi, ecx
        dec    edi
        dec    esi
        std
        rep movsb
        cld
        jmp    SHORT loc_5_5B9B
loc_5_5B8C:
        mov    eax, ecx
        shr    ecx, 2
        cld
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_5_5B9B:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_5_5BA3:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [ebp - 4], 4
        lea    eax, [ebp - 4]
        push   eax
        push   edx
        lea    eax, [ebp - 8]
        push   eax
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_5_D11C
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_5_5BDE
        cmp    dword ptr [ebp - 8], 3
        jne    SHORT loc_5_5BDE
        cmp    dword ptr [ebp - 4], 4
        mov    eax, 1
        je     SHORT loc_5_5BE0
loc_5_5BDE:
        xor    eax, eax
loc_5_5BE0:
        mov    esp, ebp
        pop    ebp
        ret
sub_5_5BE4:
        sub    esp, 4
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   80000002h
        call   sub_5_D154
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_5C0E
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_5_5C2F
loc_5_5C0E:
        lea    edx, [esp]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_5BA3
        test   eax, eax
        je     SHORT loc_5_5C2F
        test   byte ptr [esp], 1
        je     SHORT loc_5_5C2F
        db  080h, 00Dh, 000h, 000h, 000h, 000h, 080h
loc_5_5C2F:
        add    esp, 4
        ret
loc_5_5C33:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   esi
        lea    eax, [ebp - 20h]
        push   edi
        xor    esi, esi
        mov    word ptr [ebp - 1ch], si
        push   eax
        push   6
        db  0BAh, 000h, 000h, 000h, 000h
        push   esi
        xor    ecx, ecx
        mov    dword ptr [ebp - 20h], 7f02h
        mov    edi, 10040h
        call   sub_5_1240
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        push   edi
        call   sub_5_D208
        add    esp, 0ch
        mov    dword ptr [ebp - 10h], eax
        cmp    eax, esi
        je     NEAR PTR loc_5_5D07
        lea    eax, [ebp - 18h]
        mov    dword ptr [ebp - 14h], esi
        push   eax
        mov    dword ptr [ebp - 18h], esi
        push   0ch
        push   201h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_5_D104
        add    esp, 10h
        mov    dword ptr [ebp - 4], eax
        cmp    eax, 20h
        jb     SHORT loc_5_5CFC
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        push   edi
        call   sub_5_D208
        add    esp, 0ch
        mov    dword ptr [ebp - 8], eax
        cmp    eax, esi
        je     SHORT loc_5_5CE4
        lea    eax, [ebp - 8]
        push   eax
        push   8
        push   203h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_5_D104
        add    esp, 10h
        db  0A3h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        call   sub_5_D200
        add    esp, 4
loc_5_5CE4:
        lea    eax, [ebp - 4]
        push   eax
        push   4
        push   202h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_5_D104
        add    esp, 10h
loc_5_5CFC:
        push   dword ptr [ebp - 10h]
        call   sub_5_D200
        add    esp, 4
loc_5_5D07:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_5D15
        call   sub_5_5BE4
loc_5_5D15:
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 080h
        je     SHORT loc_5_5D5B
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 018h
        je     SHORT loc_5_5D51
        db  080h, 025h, 000h, 000h, 000h, 000h, 07Fh
        push   0
        push   8
        call   sub_5_6710
        add    esp, 4
        mov    edi, eax
        push   9
        call   sub_5_6710
        add    esp, 4
        mov    edx, edi
        mov    ecx, eax
        call   sub_5_5EA8
        jmp    SHORT loc_5_5D5B
loc_5_5D51:
        push   1
        call   sub_5_7092
        add    esp, 4
loc_5_5D5B:
        db  080h, 00Dh, 000h, 000h, 000h, 000h, 004h
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 020h
        je     SHORT loc_5_5D77
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_5D70:
        db  080h, 025h, 001h, 000h, 000h, 000h, 0DFh
loc_5_5D77:
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 010h
        je     SHORT loc_5_5D85
        call   sub_5_22AF
loc_5_5D85:
        db  0A1h, 000h, 000h, 000h, 000h
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_5_79CB
        add    esp, 4
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 080h
        je     SHORT loc_5_5E11
        db  080h, 025h, 000h, 000h, 000h, 000h, 07Fh
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_5_79CB
        add    esp, 4
        mov    ecx, 20h
        call   sub_5_231C
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_5_79CB
        add    esp, 4
        push   0
        call   sub_5_7092
        add    esp, 4
        lea    edx, [ebp - 0ch]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_5BA3
        test   eax, eax
        je     SHORT loc_5_5E0C
        push   4
        lea    eax, [ebp - 0ch]
        and    byte ptr [ebp - 0ch], 0feh
        push   eax
        push   3
        push   0
        db  068h, 000h, 000h, 000h, 000h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_5_D0EC
        add    esp, 18h
loc_5_5E0C:
        mov    esi, 1
loc_5_5E11:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_5E28
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_5_D0E4
        add    esp, 4
loc_5_5E28:
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_73CA
        test   esi, esi
        je     SHORT loc_5_5E44
        push   80000002h
        call   sub_5_D0DC
        add    esp, 4
loc_5_5E44:
        db  080h, 00Dh, 001h, 000h, 000h, 000h, 080h
        call   sub_5_73F9
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_5_5E56:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [esp + 8], 219h
        jne    SHORT loc_5_5EA1
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, 20h
        je     SHORT loc_5_5E79
        cmp    eax, 22h
        je     SHORT loc_5_5E8C
        cmp    eax, 7fffh
        je     SHORT loc_5_5E99
        jmp    SHORT loc_5_5EA1
loc_5_5E79:
        xor    eax, eax
        db  0B9h, 000h, 000h, 000h, 000h
        push   eax
        xor    edx, edx
        push   eax
        push   eax
        call   sub_5_02D3
        jmp    SHORT loc_5_5EA1
loc_5_5E8C:
        push   dword ptr [ebp + 10h]
        call   sub_5_0094
        add    esp, 4
        jmp    SHORT loc_5_5EA1
loc_5_5E99:
        mov    ecx, dword ptr [ebp + 10h]
        call   sub_5_020F
loc_5_5EA1:
        mov    eax, 1
        pop    ebp
        ret
sub_5_5EA8:
        cmp    dword ptr [esp + 4], 1
        sbb    eax, eax
        push   ecx
        push   edx
        and    eax, 4ffff030h
        sub    eax, 3ffff000h
        push   eax
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_5_D210
        ret    4
sub_5_5ECA:
        cmp    dword ptr [esp + 4], 1
        sbb    eax, eax
        push   ecx
        push   edx
        and    eax, 4ffff030h
        sub    eax, 3fffeffch
        push   eax
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_5_D210
        sub    eax, 6
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        ret    4
sub_5_5EF6:
        push   ebp
        mov    ebp, esp
        sub    esp, 304h
        push   esi
        push   edi
        mov    edi, ecx
        push   0eh
        call   sub_5_6710
        add    esp, 4
        lea    ecx, [ebp - 104h]
        push   eax
        push   ecx
        call   sub_5_D0FC
        add    esp, 8
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 4], 100h
        push   0
        lea    edx, [ebp - 104h]
        push   ecx
        push   edx
        push   1
        xor    edx, edx
        db  068h, 000h, 000h, 000h, 000h
        mov    ecx, edi
        call   sub_5_87D6
        lea    ecx, [ebp - 104h]
        push   ecx
        push   0ch
        call   sub_5_6710
        add    esp, 4
        lea    ecx, [ebp - 304h]
        lea    esi, [ebp - 304h]
        push   eax
        push   ecx
        call   sub_5_D0FC
        add    esp, 0ch
        push   0
        push   0dh
        call   sub_5_6710
        add    esp, 4
        mov    edx, esi
        mov    ecx, eax
        call   sub_5_5ECA
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_5_5F83:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        test   al, 2
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_5_5FF5
        test   al, 8
        jne    SHORT loc_5_5FF5
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        sbb    eax, eax
        inc    eax
        mov    dword ptr [ebp - 4], eax
        call   sub_5_C73C
        push   dword ptr [ebp + 0ch]
        mov    si, ax
        lea    edx, [ebp - 4]
        push   dword ptr [ebp + 8]
        push   219h
        push   edx
        mov    edi, dword ptr [ebp + 10h]
        cmp    edi, 1
        sbb    eax, eax
        add    eax, 69h
        push   eax
        call   sub_5_D07C
        add    esp, 14h
        mov    ebx, eax
        mov    ecx, esi
        call   sub_5_C77D
        test   edi, edi
        je     SHORT loc_5_5FF5
        test   ebx, ebx
        je     SHORT loc_5_5FEE
        cmp    ebx, 1
        je     SHORT loc_5_5FF5
        mov    eax, 13h
        jmp    SHORT loc_5_5FF7
loc_5_5FEE:
        mov    eax, 2ah
        jmp    SHORT loc_5_5FF7
loc_5_5FF5:
        xor    eax, eax
loc_5_5FF7:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_5FFE:
        push   esi
        push   dword ptr [esp + 0ch]
        push   0
        push   dword ptr [esp + 10h]
        call   sub_5_5F83
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_603D
        cmp    eax, 2ah
        je     SHORT loc_5_6044
        push   0
        push   0ah
        call   sub_5_6710
        add    esp, 4
        mov    esi, eax
        push   0bh
        call   sub_5_6710
        add    esp, 4
        mov    edx, esi
        mov    ecx, eax
        call   sub_5_5ECA
        jmp    SHORT loc_5_6046
loc_5_603D:
        mov    eax, 1
        jmp    SHORT loc_5_6046
loc_5_6044:
        xor    eax, eax
loc_5_6046:
        pop    esi
        ret
sub_5_6048:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        mov    dword ptr [ebp - 10h], 10h
        mov    dword ptr [ebp - 0ch], 1
        mov    dword ptr [ebp - 8], 0
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], ecx
        mov    esi, ecx
        push   dword ptr [ebp + 8]
        lea    ecx, [ebp - 10h]
        push   ecx
        push   edx
        call   sub_5_5F83
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_608C
        cmp    eax, 2ah
        je     SHORT loc_5_6093
        mov    ecx, esi
        call   sub_5_5EF6
        jmp    SHORT loc_5_6095
loc_5_608C:
        mov    eax, 1
        jmp    SHORT loc_5_6095
loc_5_6093:
        xor    eax, eax
loc_5_6095:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_5_609D:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    ebx, ecx
        db  0BEh, 000h, 000h, 000h, 000h
        lea    edi, [esp + 0ch]
        lea    eax, [esp + 0ch]
        push   edx
        movsd
        movsd
        movsw
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_5_D0FC
        lea    edx, [esp + 1ch]
        add    esp, 10h
        mov    ecx, ebx
        push   0
        push   0
        call   sub_5_86BE
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_5_60E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        mov    eax, 1
        push   ebx
        push   esi
        xor    ebx, ebx
        mov    esi, dword ptr [ecx + 38h]
        mov    byte ptr [ebp - 10h], bl
        mov    dword ptr [ebp - 0fh], ebx
        mov    dword ptr [ebp - 0bh], ebx
        mov    dword ptr [ebp - 7], eax
        test   esi, esi
        je     NEAR PTR loc_5_61C7
        lea    eax, [ebp - 14h]
        lea    ecx, [ebp - 10h]
        mov    dword ptr [ebp - 14h], 0dh
        push   eax
        push   ecx
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        call   sub_5_D11C
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_5_61C2
        movzx  edx, byte ptr [ebp - 10h]
        mov    ecx, esi
        call   sub_5_189A
        test   eax, eax
        jne    SHORT loc_5_6145
        xor    eax, eax
        jmp    NEAR PTR loc_5_61C7
loc_5_6145:
        mov    ecx, dword ptr [eax + 4]
        cmp    ecx, 1
        je     SHORT loc_5_6156
        cmp    ecx, 2
        je     SHORT loc_5_618B
        xor    eax, eax
        jmp    SHORT loc_5_61C7
loc_5_6156:
        mov    ecx, dword ptr [eax + 20h]
        add    eax, 1ch
        mov    edx, dword ptr [ebp - 0fh]
        mov    eax, dword ptr [eax + 8]
        add    edx, ecx
        lea    esi, [eax + 1]
        cmp    edx, esi
        jae    SHORT loc_5_6187
        cmp    dword ptr [ebp - 0bh], 0
        je     SHORT loc_5_6187
        sub    eax, ecx
        sub    edx, edx
        sub    eax, dword ptr [ebp - 0fh]
        mov    ecx, dword ptr [ebp - 7]
        inc    eax
        div    dword ptr [ebp - 0bh]
        cmp    eax, ecx
        jb     SHORT loc_5_61C7
        mov    eax, ecx
        jmp    SHORT loc_5_61C7
loc_5_6187:
        xor    eax, eax
        jmp    SHORT loc_5_61C7
loc_5_618B:
        add    eax, 1ch
        mov    edx, dword ptr [ebp - 0fh]
        movzx  ecx, word ptr [eax + 4]
        movzx  eax, word ptr [eax + 6]
        add    edx, ecx
        lea    esi, [eax + 1]
        cmp    edx, esi
        jae    SHORT loc_5_61BE
        cmp    dword ptr [ebp - 0bh], 0
        je     SHORT loc_5_61BE
        sub    eax, ecx
        sub    edx, edx
        sub    eax, dword ptr [ebp - 0fh]
        mov    ecx, dword ptr [ebp - 7]
        inc    eax
        div    dword ptr [ebp - 0bh]
        cmp    eax, ecx
        jb     SHORT loc_5_61C7
        mov    eax, ecx
        jmp    SHORT loc_5_61C7
loc_5_61BE:
        xor    eax, eax
        jmp    SHORT loc_5_61C7
loc_5_61C2:
        mov    eax, 1
loc_5_61C7:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_61CD:
        push   esi
        push   edi
        mov    edi, edx
        lea    esi, [ecx + 38h]
        mov    eax, dword ptr [esi]
        test   eax, eax
        jne    SHORT loc_5_61F4
        mov    eax, dword ptr [ecx + 20h]
        push   0
        push   2
        mov    eax, dword ptr [eax + 38h]
        mov    edx, dword ptr [eax + 0ch]
        call   sub_5_16B4
        test   eax, eax
        jne    SHORT loc_5_61F4
        xor    eax, eax
        jmp    SHORT loc_5_6233
loc_5_61F4:
        mov    edx, edi
        mov    ecx, eax
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        call   sub_5_1761
        test   eax, eax
        jne    SHORT loc_5_6214
        mov    ecx, esi
        call   sub_5_1596
        xor    eax, eax
        jmp    SHORT loc_5_6233
loc_5_6214:
        cmp    dword ptr [eax + 4], 4
        jne    SHORT loc_5_621E
        or     byte ptr [eax + 1ch], 1
loc_5_621E:
        cmp    edi, 1
        jb     SHORT loc_5_622E
        cmp    edi, 4
        ja     SHORT loc_5_622E
        db  0FFh, 005h, 000h, 000h, 000h, 000h
loc_5_622E:
        mov    eax, 1
loc_5_6233:
        pop    edi
        pop    esi
        ret    8
sub_5_6238:
        push   ebp
        mov    ebp, esp
        sub    esp, 148h
        push   ebx
        push   esi
        mov    edx, dword ptr [ecx + 20h]
        push   edi
        mov    esi, ecx
        call   sub_5_1D46
        mov    ecx, dword ptr [eax]
        movzx  edi, cx
        mov    eax, ecx
        lea    ecx, [esi + 38h]
        shr    eax, 10h
        xor    ebx, ebx
        movzx  edx, ax
        mov    dword ptr [ebp - 10h], edx
        call   sub_5_15ED
        mov    ecx, dword ptr [esi + 20h]
        mov    eax, dword ptr [ecx + 38h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    eax, ebx
        je     NEAR PTR loc_5_6505
        mov    edx, edi
        call   sub_5_609D
        cmp    eax, ebx
        mov    edi, eax
        je     NEAR PTR loc_5_6505
        lea    eax, [ebp - 38h]
        lea    ecx, [ebp - 4]
        push   eax
        mov    dword ptr [ebp - 4], ebx
        mov    dword ptr [ebp - 38h], 4
        push   ecx
        push   ebx
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        call   sub_5_D11C
        add    esp, 18h
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], 100h
        push   ecx
        lea    eax, [ebp - 148h]
        push   eax
        push   ebx
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        call   sub_5_D11C
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_5_62D5
        mov    dword ptr [ebp - 8], ebx
loc_5_62D5:
        lea    eax, [ebp - 34h]
        lea    ecx, [ebp - 20h]
        push   eax
        xor    ebx, ebx
        mov    dword ptr [ebp - 34h], 0dh
        push   ecx
        push   ebx
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        call   sub_5_D11C
        add    esp, 18h
        cmp    eax, 1
        sbb    eax, eax
        push   edi
        neg    eax
        mov    dword ptr [ebp - 30h], eax
        call   sub_5_D0E4
        add    esp, 4
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp - 8], ebx
        jbe    SHORT loc_5_6366
loc_5_6314:
        movzx  edx, byte ptr [ebp + ebx - 148h]
        mov    ecx, dword ptr [ebp - 0ch]
        call   sub_5_189A
        test   eax, eax
        jne    SHORT loc_5_6347
        test   byte ptr [ebp - 4], 4
        je     SHORT loc_5_6394
        push   0
        mov    edx, 8000h
        push   0
        mov    ecx, esi
        call   sub_5_61CD
        test   eax, eax
        jne    SHORT loc_5_6360
        jmp    NEAR PTR loc_5_6505
loc_5_6347:
        lea    edx, [eax + 1ch]
        mov    ecx, esi
        push   dword ptr [eax + 14h]
        push   edx
        mov    edx, dword ptr [eax + 4]
        call   sub_5_61CD
        test   eax, eax
        je     NEAR PTR loc_5_6505
loc_5_6360:
        inc    ebx
        cmp    dword ptr [ebp - 8], ebx
        ja     SHORT loc_5_6314
loc_5_6366:
        cmp    dword ptr [ebp - 30h], 0
        je     NEAR PTR loc_5_64BF
        mov    ecx, dword ptr [ebp - 0ch]
        inc    dword ptr [ebp - 8]
        movzx  edx, byte ptr [ebp - 20h]
        call   sub_5_189A
        test   eax, eax
        jne    SHORT loc_5_63AD
        mov    edx, 9
        mov    ecx, esi
        call   sub_5_731F
        jmp    NEAR PTR loc_5_6505
loc_5_6394:
        lea    ecx, [esi + 38h]
        call   sub_5_1596
        mov    edx, 11h
        mov    ecx, esi
        call   sub_5_731F
        jmp    NEAR PTR loc_5_6505
loc_5_63AD:
        mov    ecx, dword ptr [eax + 4]
        cmp    ecx, 1
        je     SHORT loc_5_63CF
        cmp    ecx, 2
        je     NEAR PTR loc_5_6443
        mov    edx, 9
        mov    ecx, esi
        call   sub_5_731F
        jmp    NEAR PTR loc_5_6505
loc_5_63CF:
        mov    ecx, dword ptr [eax + 20h]
        add    eax, 1ch
        mov    edx, dword ptr [ebp - 1bh]
        imul   edx, dword ptr [ebp - 10h]
        add    edx, dword ptr [ebp - 1fh]
        add    edx, ecx
        cmp    edx, ecx
        jb     SHORT loc_5_6432
        mov    ecx, dword ptr [ebp - 1bh]
        lea    ecx, [ecx + edx - 1]
        cmp    dword ptr [eax + 8], ecx
        jb     SHORT loc_5_6432
        mov    dword ptr [ebp - 44h], edx
        mov    dword ptr [ebp - 40h], ecx
        mov    word ptr [ebp - 48h], 0
        push   10h
        lea    edx, [ebp - 48h]
        mov    word ptr [ebp - 46h], 14h
        mov    cx, word ptr [eax + 0ch]
        push   edx
        mov    word ptr [ebp - 3ch], cx
        mov    edx, 1
        mov    ax, word ptr [eax + 0eh]
        mov    ecx, esi
        mov    word ptr [ebp - 3ah], ax
        call   sub_5_61CD
        test   eax, eax
        jne    NEAR PTR loc_5_64BF
        jmp    NEAR PTR loc_5_6505
loc_5_6432:
        mov    edx, 9
        mov    ecx, esi
        call   sub_5_731F
        jmp    NEAR PTR loc_5_6505
loc_5_6443:
        add    eax, 1ch
        mov    ecx, dword ptr [ebp - 1bh]
        imul   ecx, dword ptr [ebp - 10h]
        add    ecx, dword ptr [ebp - 1fh]
        movzx  edi, word ptr [eax + 4]
        lea    edx, [ecx + edi]
        cmp    edx, edi
        jb     SHORT loc_5_64B1
        movzx  edi, word ptr [eax + 6]
        mov    ecx, dword ptr [ebp - 1bh]
        add    ecx, edx
        dec    ecx
        cmp    ecx, edi
        ja     SHORT loc_5_64B1
        mov    word ptr [ebp - 28h], dx
        mov    ecx, dword ptr [ebp - 1bh]
        mov    word ptr [ebp - 2ch], 0
        mov    word ptr [ebp - 2ah], 0ch
        add    cx, dx
        dec    cx
        mov    word ptr [ebp - 26h], cx
        push   0ch
        mov    dx, word ptr [eax + 8]
        lea    ecx, [ebp - 2ch]
        mov    word ptr [ebp - 24h], dx
        push   ecx
        mov    bl, byte ptr [eax + 0ah]
        mov    edx, 2
        mov    ecx, esi
        mov    byte ptr [ebp - 22h], bl
        mov    al, byte ptr [eax + 0bh]
        mov    byte ptr [ebp - 21h], al
        call   sub_5_61CD
        test   eax, eax
        jne    SHORT loc_5_64BF
        jmp    SHORT loc_5_6505
loc_5_64B1:
        mov    edx, 9
        mov    ecx, esi
        call   sub_5_731F
        jmp    SHORT loc_5_6505
loc_5_64BF:
        test   byte ptr [ebp - 4], 1
        jne    SHORT loc_5_64E2
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_64F7
        test   byte ptr [ebp - 4], 2
        jne    SHORT loc_5_64EE
        mov    edx, 1dh
        mov    ecx, esi
        call   sub_5_731F
        jmp    SHORT loc_5_64F7
loc_5_64E2:
        cmp    dword ptr [esi + 38h], 0
        jne    SHORT loc_5_64F7
        test   byte ptr [ebp - 4], 2
        je     SHORT loc_5_64F7
loc_5_64EE:
        push   esi
        call   sub_5_9014
        add    esp, 4
loc_5_64F7:
        push   80000000h
        mov    ecx, esi
        push   0
        call   sub_5_1906
loc_5_6505:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_650C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0d8h
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
loc_5_651A:
        mov    edx, esi
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_609D
        mov    edi, eax
        test   edi, edi
        je     NEAR PTR loc_5_65F9
        mov    edx, edi
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_60E0
        push   edi
        mov    dword ptr [ebp - 8], eax
        xor    edi, edi
        call   sub_5_D0E4
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, 1
        ja     SHORT loc_5_6553
        mov    eax, 1
loc_5_6553:
        mov    dword ptr [ebp - 0ch], eax
        test   eax, eax
        je     NEAR PTR loc_5_65F3
        mov    ebx, esi
loc_5_6560:
        cmp    dword ptr [ebp - 8], 1
        ja     SHORT loc_5_657D
        push   esi
        lea    eax, [ebp - 0d8h]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_5_D0FC
        add    esp, 0ch
        jmp    SHORT loc_5_6593
loc_5_657D:
        push   edi
        lea    eax, [ebp - 0d8h]
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_5_D0FC
        add    esp, 10h
loc_5_6593:
        push   0
        lea    ecx, [ebp - 0d8h]
        push   dword ptr [ebp + 8]
        push   ecx
        lea    edx, [ebp - 4]
        push   edx
        call   _CONFIGMG_Create_DevNode
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_5_65B4
        cmp    eax, 10h
        jne    SHORT loc_5_65E3
loc_5_65B4:
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_1D46
        push   0
        mov    dword ptr [eax], ebx
        push   20000h
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_1906
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_5_65E3
        mov    edx, 9
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_731F
loc_5_65E3:
        add    ebx, 10000h
        inc    edi
        cmp    edi, dword ptr [ebp - 0ch]
        jb     NEAR PTR loc_5_6560
loc_5_65F3:
        inc    esi
        jmp    NEAR PTR loc_5_651A
loc_5_65F9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_6600:
        push   ebp
        mov    ebp, esp
        sub    esp, 404h
        push   ebx
        push   esi
        push   edi
        mov    esi, edx
        mov    edx, ecx
        mov    edi, ecx
        mov    ecx, esi
        xor    ebx, ebx
        call   sub_5_1D46
        movzx  edx, word ptr [eax]
        mov    ecx, edi
        call   sub_5_609D
        mov    edi, eax
        test   edi, edi
        je     NEAR PTR loc_5_66BC
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 404h]
        mov    dword ptr [ebp - 4], 400h
        push   eax
        push   ecx
        push   ebx
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        call   sub_5_D11C
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_5_6670
        push   ebx
        lea    ecx, [ebp - 404h]
        push   dword ptr [ebp - 4]
        push   ecx
        push   1
        xor    edx, edx
        db  068h, 000h, 000h, 000h, 000h
        mov    ecx, esi
        call   sub_5_886D
loc_5_6670:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 404h]
        push   eax
        xor    ebx, ebx
        mov    dword ptr [ebp - 4], 400h
        push   ecx
        push   ebx
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        call   sub_5_D11C
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_5_66B3
        push   ebx
        lea    ecx, [ebp - 404h]
        push   dword ptr [ebp - 4]
        push   ecx
        push   1
        xor    edx, edx
        db  068h, 000h, 000h, 000h, 000h
        mov    ecx, esi
        call   sub_5_886D
loc_5_66B3:
        push   edi
        call   sub_5_D0E4
        add    esp, 4
loc_5_66BC:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_66C3:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    eax, 5
        je     SHORT loc_5_66DB
        cmp    eax, 6
        je     SHORT loc_5_6700
        mov    eax, 1
        jmp    SHORT loc_5_670D
loc_5_66DB:
        push   0
        push   dword ptr [ebp + 10h]
        call   _CONFIGMG_Reset_Children_Marks
        add    esp, 8
        push   dword ptr [ebp + 10h]
        call   sub_5_650C
        add    esp, 4
        push   dword ptr [ebp + 10h]
        call   sub_5_909C
        add    esp, 4
        jmp    SHORT loc_5_670B
loc_5_6700:
        mov    edx, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 10h]
        call   sub_5_6600
loc_5_670B:
        xor    eax, eax
loc_5_670D:
        pop    ebp
        ret
        db  0CCh
sub_5_6710:
        mov    ecx, dword ptr [esp + 4]
        db  00Fh, 0B7h, 004h, 04Dh, 00Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        ret
sub_5_6723:
        cmp    edx, 1
        sbb    eax, eax
        and    eax, 80000000h
        push   eax
        push   8
        push   484e5950h
        push   0ch
        push   ecx
        call   sub_5_D12C
        add    esp, 14h
        ret
sub_5_6741:
        cmp    edx, 1
        sbb    eax, eax
        and    eax, 80000000h
        push   eax
        push   14h
        push   454e5950h
        push   18h
        push   ecx
        call   sub_5_D12C
        add    esp, 14h
        ret
sub_5_675F:
        push   201h
        push   0ch
        call   sub_5_D134
        add    esp, 8
        test   eax, eax
        je     SHORT loc_5_6779
        mov    dword ptr [eax + 8], 484e5950h
loc_5_6779:
        ret
sub_5_677A:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        test   eax, eax
        je     SHORT loc_5_6785
        mov    dword ptr [eax + 4], edx
loc_5_6785:
        cmp    dword ptr [ecx + 4], 0
        jne    SHORT loc_5_678E
        mov    dword ptr [ecx + 4], edx
loc_5_678E:
        mov    dword ptr [ecx], edx
        ret
sub_5_6791:
        push   ebx
        push   esi
        push   edi
        mov    ebx, edx
        mov    esi, ecx
        push   201h
        push   18h
        call   sub_5_D134
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_5_67C0
        mov    edx, edi
        mov    ecx, esi
        mov    dword ptr [edi + 14h], 454e5950h
        mov    dword ptr [edi + 8], ebx
        call   sub_5_677A
loc_5_67C0:
        cmp    edi, 1
        pop    edi
        sbb    eax, eax
        pop    esi
        inc    eax
        pop    ebx
        ret
loc_5_67CA:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 4
        test   eax, eax
        je     SHORT loc_5_6807
        push   201h
        push   18h
        call   sub_5_D134
        add    esp, 8
        mov    edx, eax
        test   edx, edx
        je     SHORT loc_5_680E
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [edx + 8], esi
        mov    dword ptr [edx + 14h], 454e5950h
        call   sub_5_677A
loc_5_6807:
        mov    eax, 1
loc_5_680C:
        pop    esi
        ret
loc_5_680E:
        xor    eax, eax
        jmp    SHORT loc_5_680C
sub_5_6812:
        db  068h, 000h, 000h, 000h, 000h
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, 3
        db  089h, 015h, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        call   sub_5_CBE2
        test   eax, eax
        mov    eax, 1
        jne    SHORT loc_5_6849
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_684A
        xor    eax, eax
loc_5_6849:
        ret
sub_5_684A:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ecx]
        mov    edi, ecx
        test   eax, eax
        je     SHORT loc_5_6875
        xor    ebx, ebx
loc_5_6857:
        mov    esi, dword ptr [eax]
        mov    dword ptr [eax + 14h], ebx
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        jne    SHORT loc_5_686F
        push   ebx
        push   eax
        call   sub_5_D0C8
        add    esp, 8
loc_5_686F:
        mov    eax, esi
        test   esi, esi
        jne    SHORT loc_5_6857
loc_5_6875:
        xor    eax, eax
        mov    dword ptr [edi], eax
        mov    dword ptr [edi + 4], eax
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_6880:
        sub    esp, 18h
        push   esi
        push   edi
        mov    esi, edx
        lea    edi, [ecx + 8]
        lea    ecx, [esp + 10h]
        push   10h
        mov    edx, edi
        add    esi, 8
        call   sub_5_5B5D
        push   10h
        mov    edx, esi
        mov    ecx, edi
        call   sub_5_5B5D
        lea    edx, [esp + 10h]
        push   10h
        mov    ecx, esi
        call   sub_5_5B5D
        pop    edi
        pop    esi
        add    esp, 18h
        ret
sub_5_68B8:
        push   ebx
        push   esi
        mov    ebx, ecx
        push   edi
        cmp    edx, ecx
        push   ebp
        mov    ebp, edx
        mov    edi, edx
        mov    esi, ecx
        je     SHORT loc_5_6925
loc_5_68C8:
        mov    eax, dword ptr [ebx + 10h]
        cmp    dword ptr [ebp + 10h], eax
        jbe    SHORT loc_5_68D8
loc_5_68D0:
        mov    ebp, dword ptr [ebp + 4]
        cmp    dword ptr [ebp + 10h], eax
        ja     SHORT loc_5_68D0
loc_5_68D8:
        cmp    ebx, ebp
        je     SHORT loc_5_6908
        mov    edx, ebp
        mov    ecx, ebx
        call   sub_5_6880
        mov    ebx, dword ptr [ebx]
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [ebx + 10h], eax
        jae    SHORT loc_5_68F6
loc_5_68EF:
        mov    ebx, dword ptr [ebx]
        cmp    dword ptr [ebx + 10h], eax
        jb     SHORT loc_5_68EF
loc_5_68F6:
        cmp    ebx, ebp
        je     SHORT loc_5_6908
        mov    edx, ebp
        mov    ecx, ebx
        call   sub_5_6880
        mov    ebp, dword ptr [ebp + 4]
        jmp    SHORT loc_5_68C8
loc_5_6908:
        cmp    ebx, esi
        je     SHORT loc_5_6916
        mov    edx, dword ptr [ebx + 4]
        mov    ecx, esi
        call   sub_5_68B8
loc_5_6916:
        cmp    edi, ebp
        je     SHORT loc_5_6925
        mov    esi, dword ptr [ebp]
        mov    ebp, edi
        mov    ebx, esi
        cmp    edi, esi
        jne    SHORT loc_5_68C8
loc_5_6925:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_692A:
        mov    edx, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ecx]
        jmp    NEAR PTR sub_5_68B8
sub_5_6934:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        push   ebp
        push   0
        push   8
        lea    edi, [esi + 1ch]
        call   sub_5_1906
        mov    ebx, dword ptr [esi + 18h]
        mov    ebp, dword ptr [edi]
        db  0B8h, 000h, 000h, 000h, 000h
        push   esi
        mov    dword ptr [esi + 18h], eax
        mov    dword ptr [edi], eax
        call   sub_5_1FB0
        add    esp, 4
        mov    ecx, esi
        inc    dword ptr [esi + 10h]
        push   0
        push   8
        call   sub_5_1923
        mov    dword ptr [esi + 18h], ebx
        mov    dword ptr [edi], ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_6976:
        push   esi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        add    esi, 24h
        db  039h, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_69B0
loc_5_6988:
        test   byte ptr [esi + 0bh], 80h
        je     SHORT loc_5_69A5
        mov    edx, 8
        mov    ecx, esi
        push   dword ptr [esi + 20h]
        call   sub_5_0B16
        test   eax, eax
        jne    SHORT loc_5_69A5
        or     byte ptr [esi + 0bh], 20h
loc_5_69A5:
        add    esi, 24h
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_6988
loc_5_69B0:
        call   sub_5_0A32
        pop    esi
        ret
sub_5_69B7:
        push   esi
        mov    esi, ecx
        mov    ecx, edx
        call   sub_5_099C
        test   eax, eax
        je     SHORT loc_5_69D1
        cmp    eax, 2
        je     SHORT loc_5_69DD
        cmp    eax, 13h
        je     SHORT loc_5_69F5
        jmp    SHORT loc_5_69FC
loc_5_69D1:
        call   sub_5_6976
        mov    eax, 1
        jmp    SHORT loc_5_69FC
loc_5_69DD:
        call   sub_5_09DB
        mov    edx, 3
        mov    ecx, esi
        call   sub_5_731F
        mov    eax, 1
        jmp    SHORT loc_5_69FC
loc_5_69F5:
        call   sub_5_09DB
        xor    eax, eax
loc_5_69FC:
        pop    esi
        ret
sub_5_69FE:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        mov    dword ptr [ebp - 0ch], edx
        mov    dword ptr [ebp - 8], 0
        push   esi
        push   edi
        cmp    dword ptr [ecx + 40h], 1
        sbb    ebx, ebx
        mov    edi, ecx
        inc    ebx
        mov    edx, 2
        cmp    ebx, 1
        sbb    eax, eax
        mov    esi, dword ptr [edi + eax*4 + 40h]
        mov    ecx, esi
        call   sub_5_14E6
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_5_6A49
        mov    edx, 3
        mov    ecx, edi
        call   sub_5_731F
        xor    eax, eax
        jmp    NEAR PTR loc_5_6AE2
loc_5_6A49:
        test   byte ptr [ebp - 0ch], 20h
        je     SHORT loc_5_6A5B
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + 24h], edi
        je     SHORT loc_5_6A5B
        xor    ebx, ebx
loc_5_6A5B:
        mov    edx, esi
        mov    ecx, edi
        call   sub_5_058A
        test   eax, eax
        jne    SHORT loc_5_6A74
        lea    ecx, [ebp - 4]
        call   sub_5_1596
        xor    eax, eax
        jmp    SHORT loc_5_6AE2
loc_5_6A74:
        mov    edx, esi
        mov    ecx, edi
        call   sub_5_0969
        test   eax, eax
        jne    SHORT loc_5_6A91
        mov    edx, 3
        mov    ecx, edi
        call   sub_5_731F
        xor    eax, eax
        jmp    SHORT loc_5_6AE2
loc_5_6A91:
        mov    edx, 1
        mov    ecx, edi
        call   sub_5_69B7
        test   eax, eax
        jne    SHORT loc_5_6ABE
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 004h
        je     SHORT loc_5_6AE9
        mov    edx, esi
        mov    ecx, edi
        call   sub_5_2F3A
        test   eax, eax
        je     SHORT loc_5_6AE9
        mov    dword ptr [ebp - 8], 1
loc_5_6ABE:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 10h]
        mov    dword ptr [edi + 38h], eax
        mov    eax, dword ptr [eax + 10h]
        test   eax, eax
        je     SHORT loc_5_6ADF
loc_5_6ACE:
        test   ecx, ecx
        je     SHORT loc_5_6ADF
        mov    dword ptr [eax + 8], ecx
        mov    eax, dword ptr [eax + 10h]
        mov    ecx, dword ptr [ecx + 10h]
        test   eax, eax
        jne    SHORT loc_5_6ACE
loc_5_6ADF:
        mov    eax, dword ptr [ebp - 8]
loc_5_6AE2:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_6AE9:
        test   ebx, ebx
        je     SHORT loc_5_6AFD
        mov    edx, 6
        mov    ecx, edi
        call   sub_5_69B7
        test   eax, eax
        jne    SHORT loc_5_6ABE
loc_5_6AFD:
        mov    eax, dword ptr [edi + 4]
        cmp    byte ptr [eax], 0
        je     SHORT loc_5_6B16
        mov    ecx, edi
        call   sub_5_1D31
        mov    edx, 0fh
        cmp    eax, 2
        jae    SHORT loc_5_6B1B
loc_5_6B16:
        mov    edx, 6
loc_5_6B1B:
        mov    ecx, edi
        call   sub_5_731F
        lea    ecx, [ebp - 4]
        call   sub_5_1596
        xor    eax, eax
        jmp    SHORT loc_5_6AE2
sub_5_6B2E:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        xor    edi, edi
        mov    eax, 20000h
        push   edi
        mov    dword ptr [ebp - 8], edi
        push   eax
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_6B5D
        push   10000000h
        mov    ecx, esi
        push   edi
        call   sub_5_1906
loc_5_6B5D:
        lea    ecx, [esi + 40h]
        lea    ebx, [esi + 34h]
        call   sub_5_15ED
        push   4
        db  0BAh, 000h, 000h, 000h, 000h
        mov    ecx, esi
        call   sub_5_8AD6
        mov    ecx, ebx
        call   sub_5_15ED
        push   1
        db  0BAh, 000h, 000h, 000h, 000h
        push   1
        mov    ecx, esi
        call   sub_5_8B0B
        test   eax, eax
        je     SHORT loc_5_6B98
        xor    eax, eax
        jmp    NEAR PTR loc_5_6DAA
loc_5_6B98:
        cmp    dword ptr [ebx], 0
        je     SHORT loc_5_6BB0
        lea    ecx, [esi + 30h]
        call   sub_5_15ED
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi + 30h], eax
        mov    dword ptr [ebx], 0
loc_5_6BB0:
        xor    ebx, ebx
        mov    ecx, esi
        push   ebx
        push   1
        push   ebx
        push   1
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_6C0E
        push   ebx
        lea    eax, [ebp - 10h]
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        xor    edx, edx
        push   3
        mov    ecx, esi
        mov    dword ptr [ebp - 10h], 4
        db  068h, 000h, 000h, 000h, 000h
        call   sub_5_87D6
        test   eax, eax
        jne    SHORT loc_5_6C0E
        mov    eax, 2000000h
        mov    ecx, esi
        push   eax
        push   ebx
        push   eax
        push   ebx
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_6C0E
        mov    edx, 0dh
        mov    ecx, esi
        call   sub_5_731F
        xor    eax, eax
        jmp    NEAR PTR loc_5_6DAA
loc_5_6C0E:
        lea    eax, [ebp - 4]
        xor    edx, edx
        push   eax
        mov    ecx, esi
        call   sub_5_8D6E
        test   byte ptr [ebp - 4], 8
        je     SHORT loc_5_6C29
        lea    ecx, [esi + 3ch]
        call   sub_5_15ED
loc_5_6C29:
        lea    eax, [ebp - 0ch]
        mov    edx, 1
        push   eax
        mov    ecx, esi
        call   sub_5_8D6E
        test   byte ptr [ebp - 0ch], 4
        je     SHORT loc_5_6C52
        mov    edx, 16h
        mov    ecx, esi
        call   sub_5_731F
        xor    eax, eax
        jmp    NEAR PTR loc_5_6DAA
loc_5_6C52:
        cmp    dword ptr [esi + 40h], 0
        jne    SHORT loc_5_6C5E
        cmp    dword ptr [esi + 3ch], 0
        je     SHORT loc_5_6CAC
loc_5_6C5E:
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, esi
        call   sub_5_69FE
        mov    edi, eax
        push   0
        mov    ecx, 400h
        push   ecx
        push   0
        push   ecx
        mov    ecx, esi
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_6C87
        xor    eax, eax
        jmp    NEAR PTR loc_5_6DAA
loc_5_6C87:
        test   byte ptr [ebp - 8], 10h
        je     SHORT loc_5_6CAC
        test   byte ptr [ebp - 8], 0fh
        je     SHORT loc_5_6CAC
        cmp    dword ptr [esi + 40h], 0
        jne    SHORT loc_5_6CAC
        mov    edx, 10h
        mov    ecx, esi
        call   sub_5_731F
        xor    eax, eax
        jmp    NEAR PTR loc_5_6DAA
loc_5_6CAC:
        push   10000000h
        mov    ecx, esi
        push   0
        call   sub_5_1906
        test   byte ptr [ebp - 4], 1
        je     SHORT loc_5_6CC7
        mov    ecx, esi
        call   sub_5_6934
loc_5_6CC7:
        test   byte ptr [ebp - 0ch], 1
        je     SHORT loc_5_6CE2
        mov    ecx, esi
        call   sub_5_6934
        push   40000000h
        mov    ecx, esi
        push   0
        call   sub_5_1906
loc_5_6CE2:
        test   byte ptr [ebp - 4], 20h
        je     SHORT loc_5_6D18
        mov    eax, 1000000h
        mov    ecx, esi
        push   eax
        push   0
        push   eax
        push   0
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_6D18
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    esi, eax
        je     SHORT loc_5_6D18
        cmp    dword ptr [eax + 24h], esi
        je     SHORT loc_5_6D18
        mov    edx, 12h
        mov    ecx, esi
        call   sub_5_731F
loc_5_6D18:
        test   byte ptr [ebp - 4], 40h
        je     SHORT loc_5_6D40
        mov    eax, 1000000h
        mov    ecx, esi
        push   eax
        push   0
        push   eax
        push   0
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_6D40
        mov    edx, 1ch
        mov    ecx, esi
        call   sub_5_731F
loc_5_6D40:
        push   0
        mov    ecx, esi
        push   0
        push   200000h
        push   8
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_6DA8
        cmp    dword ptr [esi + 40h], 0
        jne    SHORT loc_5_6DA8
        mov    eax, dword ptr [esi + 30h]
        test   eax, eax
        je     SHORT loc_5_6DA8
        cmp    dword ptr [eax + 0ch], 7fffh
        ja     SHORT loc_5_6DA8
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 002h
        jne    SHORT loc_5_6DA8
        mov    ebx, dword ptr [eax + 10h]
        test   ebx, ebx
        je     SHORT loc_5_6DA8
loc_5_6D7C:
        test   byte ptr [ebx + 5], 80h
        jne    SHORT loc_5_6D93
        mov    ecx, ebx
        call   sub_5_0516
        test   eax, eax
        je     SHORT loc_5_6D9C
        test   byte ptr [eax + 8], 4
        je     SHORT loc_5_6D9C
loc_5_6D93:
        mov    ebx, dword ptr [ebx + 10h]
        test   ebx, ebx
        jne    SHORT loc_5_6D7C
        jmp    SHORT loc_5_6DA8
loc_5_6D9C:
        mov    edx, 1ah
        mov    ecx, esi
        call   sub_5_731F
loc_5_6DA8:
        mov    eax, edi
loc_5_6DAA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_6DB1:
        push   1
        db  0A1h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [esp + 8]
        push   eax
        push   8
        mov    ecx, 1
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   8
        call   sub_5_CEA8
        ret
loc_5_6DD2:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        sbb    ecx, ecx
        push   0
        mov    edx, dword ptr [esp + 0ch]
        push   -1
        neg    ecx
        push   eax
        push   8
        call   sub_5_CEA8
        mov    eax, 1
        ret
loc_5_6DFA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        push   edi
        xor    edi, edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], edi
        cmp    esi, edi
        jne    NEAR PTR loc_5_6E9F
        mov    ebx, dword ptr [ebp + 0ch]
        and    ebx, 10h
        cmp    ebx, 1
        sbb    eax, eax
        neg    eax
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    ebx, 1
        mov    eax, 3
        adc    eax, -1
        test   byte ptr [ebp + 0ch], 1
        db  0A3h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_6E52
        mov    eax, 80000000h
        db  009h, 005h, 000h, 000h, 000h, 000h
        db  009h, 005h, 000h, 000h, 000h, 000h
loc_5_6E52:
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 4]
        push   eax
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        push   0
        mov    ecx, 2
        call   sub_5_CB5A
        test   eax, eax
        je     SHORT loc_5_6E8D
        mov    edx, 5
        mov    ecx, 2
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        cmp    ebx, 1
        adc    edx, -1
        jmp    SHORT loc_5_6ECA
loc_5_6E8D:
        mov    edi, 80000002h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    ecx, 2
        jmp    SHORT loc_5_6EC0
loc_5_6E9F:
        test   byte ptr [ebp + 0ch], 20h
        mov    edx, 8
        jne    SHORT loc_5_6EBB
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, 7
        and    eax, 10h
        cmp    eax, 1
        adc    edx, -1
loc_5_6EBB:
        mov    ecx, 3
loc_5_6EC0:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_5_6ECA:
        test   byte ptr [ebp + 0ch], 1
        db  089h, 015h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_6EE2
        or     edx, 80000000h
        db  089h, 015h, 000h, 000h, 000h, 000h
loc_5_6EE2:
        db  068h, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        push   0
        push   dword ptr [ebp - 4]
        call   sub_5_CB5A
        test   esi, esi
        je     SHORT loc_5_6F14
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_6F14
        push   1
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_6F11:
        add    esp, 8
loc_5_6F14:
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
        test   esi, esi
        je     SHORT loc_5_6F2C
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_6F2C
        call   sub_5_73CA
loc_5_6F2C:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_6F35:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi + 78h], edi
        je     SHORT loc_5_6FB5
        cmp    edi, 1
        je     SHORT loc_5_6F5D
        cmp    edi, 2
        je     SHORT loc_5_6F6D
        cmp    edi, 4
        je     SHORT loc_5_6F7D
        cmp    edi, 8
        je     SHORT loc_5_6F8D
        jmp    SHORT loc_5_6F9B
loc_5_6F5D:
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 1
        cmp    eax, 1
        sbb    eax, eax
        add    eax, 0ah
        jmp    SHORT loc_5_6F9B
loc_5_6F6D:
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 1
        cmp    eax, 1
        sbb    eax, eax
        add    eax, 0ch
        jmp    SHORT loc_5_6F9B
loc_5_6F7D:
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 1
        cmp    eax, 1
        sbb    eax, eax
        add    eax, 0eh
        jmp    SHORT loc_5_6F9B
loc_5_6F8D:
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 1
        cmp    eax, 1
        sbb    eax, eax
        add    eax, 10h
loc_5_6F9B:
        push   eax
        mov    edx, esi
        push   8
        xor    ecx, ecx
        call   sub_5_CFA3
        test   eax, eax
        mov    eax, 13h
        je     SHORT loc_5_6FB5
        xor    eax, eax
        mov    dword ptr [esi + 78h], edi
loc_5_6FB5:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_5_6FB9:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax + 78h]
        xor    eax, eax
        mov    dword ptr [edx], ecx
        ret
sub_5_6FC9:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        or     dword ptr [eax + 7ch], ecx
        xor    eax, eax
        ret
sub_5_6FD7:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax + 7ch]
        xor    eax, eax
        mov    dword ptr [edx], ecx
        ret
sub_5_6FE7:
        sub    esp, 4
        mov    eax, dword ptr [ecx + 0ch]
        cmp    eax, 1
        je     SHORT loc_5_701B
        cmp    eax, 6
        je     SHORT loc_5_701B
        cmp    eax, 0ch
        jb     SHORT loc_5_7006
        cmp    eax, 0fh
        jbe    SHORT loc_5_701B
        cmp    eax, 12h
        je     SHORT loc_5_701B
loc_5_7006:
        lea    eax, [esp]
        xor    edx, edx
        push   eax
        call   sub_5_8D6E
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        jmp    SHORT loc_5_7020
loc_5_701B:
        mov    eax, 1
loc_5_7020:
        add    esp, 4
        ret
sub_5_7024:
        push   esi
        mov    eax, 400h
        push   edi
        mov    esi, ecx
        xor    edi, edi
        push   edi
        push   eax
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_7056
        mov    eax, 20000000h
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        push   edi
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_7056
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
loc_5_7056:
        push   20000000h
        mov    ecx, esi
        push   400h
        call   sub_5_1923
        xor    eax, eax
        mov    dword ptr [esi + 0ch], eax
        cmp    dword ptr [esi + 10h], eax
        je     SHORT loc_5_707F
        mov    edx, 16h
        mov    ecx, esi
        call   sub_5_731F
        jmp    SHORT loc_5_708F
loc_5_707F:
        push   0
        mov    ecx, esi
        push   20h
        call   sub_5_1906
        call   sub_5_7B5F
loc_5_708F:
        pop    edi
        pop    esi
        ret
sub_5_7092:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], ecx
        db  0A3h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], ecx
        db  0BAh, 000h, 000h, 000h, 000h
        db  0B9h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 0ch]
        push   eax
        push   0ch
        db  068h, 000h, 000h, 000h, 000h
        call   sub_5_1240
        mov    esp, ebp
        pop    ebp
        ret
loc_5_70C9:
        push   ebp
        mov    ecx, 40000h
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   0
        push   ecx
        push   0
        push   ecx
        mov    ecx, esi
        call   sub_5_18E3
        test   eax, eax
        jne    NEAR PTR loc_5_7271
        xor    edi, edi
        db  039h, 03Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_5_7126
        cmp    dword ptr [esi + 40h], edi
        jne    SHORT loc_5_7126
        mov    eax, 1000000h
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        push   edi
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_7126
        mov    eax, dword ptr [esi + 30h]
        test   eax, eax
        je     SHORT loc_5_7126
        cmp    dword ptr [eax + 0ch], 7fffh
        jbe    NEAR PTR loc_5_7271
loc_5_7126:
        mov    edi, 1000000h
        xor    ebx, ebx
        push   edi
        mov    ecx, esi
        push   ebx
        push   edi
        push   ebx
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_7193
        push   ebx
        mov    ecx, esi
        push   400h
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_1906
        mov    eax, 1
        push   esi
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 0ch], eax
        call   sub_5_8780
        add    esp, 4
        lea    ecx, [ebp - 0ch]
        db  0BAh, 000h, 000h, 000h, 000h
        push   ecx
        push   0ch
        db  0B9h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_5_1240
        push   edi
        mov    ecx, esi
        push   400h
        call   sub_5_1923
        mov    dword ptr [esi + 0ch], ebx
loc_5_7193:
        xor    edi, edi
        mov    eax, 400h
        push   edi
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_7220
        mov    eax, 20000000h
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        push   edi
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_7220
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 080h
        je     SHORT loc_5_71CC
        cmp    dword ptr [esi + 0ch], 0ch
        je     SHORT loc_5_7220
loc_5_71CC:
        push   20000000h
        mov    ecx, esi
        push   0
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_1906
        push   esi
        call   sub_5_8780
        add    esp, 4
        mov    ecx, esi
        call   sub_5_6FE7
        test   eax, eax
        je     SHORT loc_5_7220
        mov    eax, dword ptr [esi + 0ch]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [ebp - 8], eax
        db  0BAh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 0ch], ecx
        lea    ecx, [ebp - 0ch]
        push   ecx
        push   0ch
        db  068h, 000h, 000h, 000h, 000h
        db  0B9h, 000h, 000h, 000h, 000h
        call   sub_5_1240
loc_5_7220:
        xor    edi, edi
        mov    ebx, 8000h
        push   edi
        mov    ecx, esi
        push   ebx
        push   edi
        push   ebx
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_7271
        push   edi
        mov    ecx, esi
        push   ebx
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_1923
        mov    eax, dword ptr [esi + 6ch]
        push   esi
        mov    dword ptr [ebp - 10h], esi
        mov    dword ptr [ebp - 14h], eax
        call   sub_5_8780
        add    esp, 4
        lea    ecx, [ebp - 14h]
        db  0BAh, 000h, 000h, 000h, 000h
        push   ecx
        push   8
        db  0B9h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_5_1240
loc_5_7271:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_727D:
        push   esi
        db  0BAh, 000h, 000h, 000h, 000h
        mov    esi, 1
        mov    ecx, esi
        db  089h, 035h, 000h, 000h, 000h, 000h
        call   sub_5_CC30
        db  0BAh, 000h, 000h, 000h, 000h
        mov    ecx, esi
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_5_CC30
        pop    esi
        ret
sub_5_72AD:
        db  0B9h, 000h, 000h, 000h, 000h
        call   sub_5_1409
        db  0B9h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR sub_5_1409
sub_5_72C1:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 8
        db  080h, 025h, 000h, 000h, 000h, 000h, 0DFh
        lea    edx, [ebp - 4]
        call   sub_5_44B6
        mov    edx, dword ptr [ebp - 4]
        xor    ecx, ecx
        or     dl, 2
        call   sub_5_4572
        mov    eax, dword ptr [ebp - 4]
        db  0BAh, 000h, 000h, 000h, 000h
        and    eax, 3
        db  0B9h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        push   eax
        push   4
        push   0
        call   sub_5_1240
        and    dword ptr [ebp - 4], 7ffffffch
        mov    edx, dword ptr [ebp - 4]
        xor    ecx, ecx
        call   sub_5_4572
        call   sub_5_35C1
        mov    esp, ebp
        pop    ebp
        ret
sub_5_731F:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        push   ebp
        mov    edi, edx
        xor    ebx, ebx
        mov    ebp, 400h
        push   ebx
        push   ebp
        push   ebx
        push   ebp
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_738E
        push   ebx
        mov    ecx, esi
        push   ebp
        call   sub_5_1906
        push   ebx
        mov    ecx, esi
        push   8
        mov    dword ptr [esi + 0ch], edi
        push   ebx
        push   8
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_737C
        call   sub_5_D0B8
        test   al, 2
        je     SHORT loc_5_736C
        push   esi
        call   sub_5_122C
        add    esp, 4
        jmp    SHORT loc_5_737C
loc_5_736C:
        push   0
        db  0B9h, 000h, 000h, 000h, 000h
        push   0
        mov    edx, esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_737C:
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        call   sub_5_7B5F
        mov    ecx, esi
        call   sub_5_0BAC
loc_5_738E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_7393:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    edi, edx
        push   0
        mov    ebx, 8000h
        push   ebx
        push   0
        push   ebx
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_73C6
        push   0
        mov    ecx, esi
        push   ebx
        call   sub_5_1906
        mov    dword ptr [esi + 6ch], edi
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        call   sub_5_7B5F
loc_5_73C6:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_73CA:
        xor    eax, eax
        db  039h, 005h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_73EE
        db  039h, 005h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_73EE
        push   eax
        db  0A3h, 000h, 000h, 000h, 000h
        push   7
        call   sub_5_5FFE
        add    esp, 8
        jmp    SHORT loc_5_73F8
loc_5_73EE:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
loc_5_73F8:
        ret
sub_5_73F9:
        call   sub_5_8438
        call   sub_5_088A
        call   sub_5_87CA
        call   sub_5_72AD
        call   sub_5_C7C2
        jmp    NEAR PTR loc_5_142F
sub_5_7417:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, ecx
        xor    esi, esi
        lea    edi, [ebx + 34h]
        mov    ecx, edi
        mov    dword ptr [ebp - 4], esi
        call   sub_5_15ED
        mov    ebx, dword ptr [ebx + 30h]
        cmp    ebx, esi
        jne    SHORT loc_5_743F
        mov    eax, 1
        jmp    SHORT loc_5_747B
loc_5_743F:
        test   ebx, ebx
        je     SHORT loc_5_7470
loc_5_7443:
        mov    edx, 1
        mov    ecx, ebx
        call   sub_5_14E6
        test   eax, eax
        je     SHORT loc_5_7468
        test   esi, esi
        je     SHORT loc_5_745B
        mov    dword ptr [esi], eax
        jmp    SHORT loc_5_745E
loc_5_745B:
        mov    dword ptr [ebp - 4], eax
loc_5_745E:
        mov    esi, eax
        mov    ebx, dword ptr [ebx]
        test   ebx, ebx
        jne    SHORT loc_5_7443
        jmp    SHORT loc_5_7470
loc_5_7468:
        lea    ecx, [ebp - 4]
        call   sub_5_15ED
loc_5_7470:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edi], eax
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_5_747B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_7482:
        mov    eax, dword ptr [ecx + 10h]
        test   eax, eax
        je     SHORT loc_5_7495
        xor    ecx, ecx
loc_5_748B:
        mov    dword ptr [eax + 8], ecx
        mov    eax, dword ptr [eax + 10h]
        test   eax, eax
        jne    SHORT loc_5_748B
loc_5_7495:
        ret
sub_5_7496:
        mov    eax, dword ptr [edx + 10h]
        test   eax, eax
        je     SHORT loc_5_74BD
loc_5_749D:
        cmp    dword ptr [eax + 8], 0
        jne    SHORT loc_5_74B1
        mov    edx, dword ptr [eax + 4]
        xor    edx, dword ptr [ecx + 4]
        test   edx, 0ffff7fffh
        je     SHORT loc_5_74BA
loc_5_74B1:
        mov    eax, dword ptr [eax + 10h]
        test   eax, eax
        jne    SHORT loc_5_749D
        jmp    SHORT loc_5_74BD
loc_5_74BA:
        mov    dword ptr [eax + 8], ecx
loc_5_74BD:
        ret
sub_5_74BE:
        mov    edx, dword ptr [ecx]
        push   0
        push   1
        push   0f000h
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [edx + 4]
        call   sub_5_1674
        ret
sub_5_74D6:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        mov    dword ptr [ebp - 4], ecx
        push   esi
        push   edi
        push   800000h
        push   0
        call   sub_5_1923
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 3ch]
        mov    dword ptr [ebp - 8], ecx
        test   ecx, ecx
        je     NEAR PTR loc_5_7607
        mov    eax, dword ptr [ebp - 4]
        add    eax, 34h
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 10h], eax
        mov    esi, dword ptr [eax]
        test   esi, esi
        je     NEAR PTR loc_5_75C5
loc_5_7515:
        cmp    dword ptr [esi + 0ch], 7fffh
        ja     NEAR PTR loc_5_75C5
        mov    ecx, esi
        call   sub_5_7482
        mov    ecx, dword ptr [ebp - 8]
        call   sub_5_7482
        mov    eax, dword ptr [esi]
        mov    edi, dword ptr [esi + 10h]
        mov    dword ptr [ebp - 14h], eax
        test   edi, edi
        je     SHORT loc_5_7584
        mov    ebx, 8000h
loc_5_7542:
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, edi
        call   sub_5_7496
        test   eax, eax
        je     SHORT loc_5_756E
        test   dword ptr [eax + 4], ebx
        je     SHORT loc_5_7562
        test   dword ptr [edi + 4], ebx
        je     SHORT loc_5_756E
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [edi + 4], ebx
        jmp    SHORT loc_5_757D
loc_5_7562:
        lea    ecx, [edi + 4]
        mov    dword ptr [edi + 8], eax
        test   dword ptr [ecx], ebx
        mov    dword ptr [ecx], ebx
        jmp    SHORT loc_5_757D
loc_5_756E:
        push   800000h
        mov    ecx, dword ptr [ebp - 4]
        push   0
        call   sub_5_1906
loc_5_757D:
        mov    edi, dword ptr [edi + 10h]
        test   edi, edi
        jne    SHORT loc_5_7542
loc_5_7584:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        test   eax, eax
        je     SHORT loc_5_75B7
loc_5_758E:
        cmp    dword ptr [eax + 8], 0
        je     SHORT loc_5_759D
        mov    eax, dword ptr [eax + 10h]
        test   eax, eax
        jne    SHORT loc_5_758E
        jmp    SHORT loc_5_75B7
loc_5_759D:
        push   800000h
        mov    ecx, dword ptr [ebp - 4]
        push   0
        call   sub_5_1906
        mov    ecx, dword ptr [ebp - 10h]
        call   sub_5_74BE
        mov    esi, dword ptr [ebp - 10h]
loc_5_75B7:
        mov    dword ptr [ebp - 10h], esi
        mov    esi, dword ptr [ebp - 14h]
        test   esi, esi
        jne    NEAR PTR loc_5_7515
loc_5_75C5:
        mov    ecx, dword ptr [ebp - 8]
        call   sub_5_7482
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    esi, dword ptr [eax]
        mov    eax, 800000h
        push   eax
        push   0
        push   eax
        push   0
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_7607
        test   esi, esi
        je     SHORT loc_5_75F6
        cmp    dword ptr [esi + 0ch], 7fffh
        jbe    SHORT loc_5_7607
loc_5_75F6:
        mov    edx, 1bh
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_731F
        xor    eax, eax
        jmp    SHORT loc_5_760C
loc_5_7607:
        mov    eax, 1
loc_5_760C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_7613:
        push   esi
        mov    eax, 20000h
        mov    esi, dword ptr [esp + 8]
        push   edi
        xor    edi, edi
        mov    ecx, esi
        push   edi
        push   eax
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    NEAR PTR loc_5_76F8
        cmp    dword ptr [esi + 40h], edi
        jne    NEAR PTR loc_5_76F8
        mov    ecx, esi
        call   sub_5_7417
        test   eax, eax
        jne    SHORT loc_5_7658
        mov    edx, 3
        mov    ecx, esi
        call   sub_5_731F
        jmp    NEAR PTR loc_5_76F8
loc_5_7658:
        push   1
        xor    eax, eax
        push   eax
        mov    edx, esi
        push   -1
        xor    ecx, ecx
        push   eax
        push   eax
        call   sub_5_CEA8
        test   eax, eax
        je     SHORT loc_5_76A3
        mov    ecx, esi
        call   sub_5_74D6
        test   eax, eax
        je     SHORT loc_5_76F8
        cmp    dword ptr [esi + 34h], 0
        jne    SHORT loc_5_7693
        cmp    dword ptr [esi + 30h], 0
        je     SHORT loc_5_7693
        mov    edx, 7
        mov    ecx, esi
        call   sub_5_731F
        jmp    SHORT loc_5_76F8
loc_5_7693:
        push   0
        mov    ecx, esi
        push   800h
        call   sub_5_1906
        jmp    SHORT loc_5_76AF
loc_5_76A3:
        mov    edx, 7
        mov    ecx, esi
        call   sub_5_731F
loc_5_76AF:
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 080h
        je     SHORT loc_5_76F8
        mov    eax, 1000000h
        xor    edi, edi
        push   eax
        mov    ecx, esi
        push   edi
        push   eax
        push   edi
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_76F8
        mov    eax, 800000h
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        push   edi
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_76EC
        cmp    dword ptr [esi + 34h], edi
        je     SHORT loc_5_76F8
        cmp    dword ptr [esi + 3ch], edi
        jne    SHORT loc_5_76F8
loc_5_76EC:
        mov    edx, 20h
        mov    ecx, esi
        call   sub_5_731F
loc_5_76F8:
        mov    eax, 1
        pop    edi
        pop    esi
        ret
sub_5_7700:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        push   ebp
        xor    edi, edi
        push   edi
        push   4
        push   edi
        push   4
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_7764
        db  0BBh, 000h, 000h, 000h, 000h
        mov    edx, 1
        push   ebx
        mov    ecx, esi
        call   sub_5_1B18
        test   eax, eax
        je     SHORT loc_5_7764
        push   edi
        mov    ebp, 80000h
        push   esi
        mov    ecx, esi
        push   esi
        push   edi
        push   ebp
        push   edi
        push   ebp
        call   sub_5_18E3
        mov    ecx, 5
        cmp    eax, 1
        sbb    edx, edx
        neg    edx
        call   sub_5_1E4B
        push   edi
        mov    ecx, esi
        push   ebp
        call   sub_5_1906
        push   ebx
        xor    edx, edx
        mov    ecx, esi
        call   sub_5_1B18
loc_5_7764:
        push   0
        mov    ecx, esi
        push   20h
        call   sub_5_1923
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_7774:
        push   esi
        mov    eax, 400h
        mov    esi, dword ptr [esp + 8]
        push   edi
        xor    edi, edi
        mov    ecx, esi
        push   edi
        push   eax
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    NEAR PTR loc_5_79C3
        push   edi
        mov    ecx, esi
        push   8
        push   edi
        push   8
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_77CB
        push   edi
        mov    ecx, esi
        push   2
        push   edi
        push   2
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_77CB
        push   edi
        mov    ecx, esi
        push   20h
        push   edi
        push   20h
        call   sub_5_18E3
        test   eax, eax
        je     NEAR PTR loc_5_79C3
loc_5_77CB:
        mov    ecx, dword ptr [esi + 20h]
        test   ecx, ecx
        je     SHORT loc_5_77E7
        push   0
        push   8
        push   0
        push   8
        call   sub_5_18E3
        test   eax, eax
        je     NEAR PTR loc_5_79C3
loc_5_77E7:
        mov    ecx, dword ptr [esi + 74h]
        test   ecx, ecx
        je     SHORT loc_5_780B
        push   0
        push   8
        push   0
        push   8
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_780B
        mov    edx, 1fh
        mov    ecx, esi
        call   sub_5_731F
loc_5_780B:
        xor    edi, edi
        mov    ecx, esi
        push   edi
        push   8
        push   edi
        push   8
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_7862
        mov    eax, 10000000h
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        push   edi
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_7862
        push   edi
        mov    eax, 400h
        push   eax
        mov    ecx, esi
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_7862
        mov    ecx, esi
        call   sub_5_6B2E
        test   eax, eax
        je     SHORT loc_5_7858
        xor    eax, eax
        jmp    NEAR PTR loc_5_79C8
loc_5_7858:
        call   sub_5_7B5F
        jmp    NEAR PTR loc_5_79C3
loc_5_7862:
        xor    edi, edi
        mov    ecx, esi
        push   edi
        push   2
        push   edi
        push   2
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_7890
        push   edi
        mov    eax, 400h
        push   eax
        mov    ecx, esi
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_7890
        mov    ecx, esi
        call   sub_5_59D1
loc_5_7890:
        xor    edi, edi
        mov    eax, 800h
        push   edi
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_78D4
        push   edi
        mov    ecx, esi
        push   8
        push   edi
        push   8
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_78D4
        push   edi
        mov    eax, 400h
        push   eax
        mov    ecx, esi
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_78D4
        push   esi
        call   sub_5_7613
        add    esp, 4
loc_5_78D4:
        xor    edi, edi
        mov    ecx, esi
        push   edi
        push   8
        push   edi
        push   8
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_792F
        push   edi
        mov    eax, 400h
        push   eax
        mov    ecx, esi
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_792F
        mov    eax, 80000000h
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        push   edi
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_792F
        mov    edx, dword ptr [esi + 34h]
        mov    ecx, esi
        call   sub_5_058A
        test   eax, eax
        je     SHORT loc_5_792F
        mov    ecx, esi
        call   sub_5_2D7F
        test   eax, eax
        je     SHORT loc_5_792F
        xor    eax, eax
        jmp    NEAR PTR loc_5_79C8
loc_5_792F:
        xor    edi, edi
        mov    ecx, esi
        push   edi
        push   8
        push   edi
        push   8
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_7973
        push   edi
        mov    eax, 400h
        push   eax
        mov    ecx, esi
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_7973
        mov    eax, 80000000h
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        push   edi
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_7973
        push   esi
        call   sub_5_1033
        add    esp, 4
loc_5_7973:
        xor    edi, edi
        mov    ecx, esi
        push   edi
        push   20h
        push   edi
        push   20h
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_79C3
        push   edi
        mov    ecx, esi
        push   2
        push   edi
        push   2
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_79C3
        push   edi
        mov    ecx, esi
        push   8
        push   edi
        push   8
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_79C3
        push   edi
        mov    eax, 400h
        push   eax
        mov    ecx, esi
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_79C3
        mov    ecx, esi
        call   sub_5_7700
loc_5_79C3:
        mov    eax, 1
loc_5_79C8:
        pop    edi
        pop    esi
        ret
sub_5_79CB:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    word ptr [ebp - 2], 0
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_79F3
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        call   sub_5_C73C
        mov    word ptr [ebp - 2], ax
loc_5_79F3:
        xor    edi, edi
        mov    ebx, 1
        mov    dword ptr [ebp - 8], 0bh
        db  0FFh, 005h, 000h, 000h, 000h, 000h
loc_5_7A07:
        db  068h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, 3
        db  089h, 03Dh, 000h, 000h, 000h, 000h
        call   sub_5_CBE2
        test   eax, eax
        jne    SHORT loc_5_7A31
        db  0A1h, 000h, 000h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp + 8], eax
loc_5_7A31:
        db  00Bh, 035h, 000h, 000h, 000h, 000h
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 004h
        je     SHORT loc_5_7A96
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        db  039h, 03Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_5_7A67
        db  039h, 03Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_7A67
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        call   sub_5_727D
        db  089h, 03Dh, 000h, 000h, 000h, 000h
loc_5_7A67:
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 020h
        je     SHORT loc_5_7A90
        db  039h, 03Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_7A8B
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        call   sub_5_72C1
        db  089h, 03Dh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_5_7A90
loc_5_7A8B:
        call   sub_5_7B5F
loc_5_7A90:
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
loc_5_7A96:
        db  039h, 03Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_5_7AA7
        dec    dword ptr [ebp - 8]
        jne    NEAR PTR loc_5_7A07
loc_5_7AA7:
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_7ADE
        test   esi, esi
        je     SHORT loc_5_7AB8
        call   sub_5_73CA
loc_5_7AB8:
        mov    ecx, dword ptr [ebp - 2]
        call   sub_5_C77D
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_7AD0
        xor    ecx, ecx
        call   sub_5_3874
loc_5_7AD0:
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 080h
        je     SHORT loc_5_7ADE
        call   sub_5_73F9
loc_5_7ADE:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_7AE5:
        xor    eax, eax
        db  0A3h, 000h, 000h, 000h, 000h
        db  039h, 005h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_7B16
        db  039h, 005h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_7B08
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        jmp    SHORT loc_5_7B16
loc_5_7B08:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_5_79CB
        add    esp, 4
loc_5_7B16:
        ret
loc_5_7B17:
        push   esi
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [esp + 8]
        test   esi, esi
        je     SHORT loc_5_7B4F
        push   80000000h
        push   0
        push   59504c52h
        push   8ch
        push   esi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_5_7B4F
        push   0
        mov    ecx, esi
        push   20h
        call   sub_5_1906
loc_5_7B4F:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_7B5D
        call   sub_5_7B5F
loc_5_7B5D:
        pop    esi
        ret
sub_5_7B5F:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_7B74
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        jmp    SHORT loc_5_7BC1
loc_5_7B74:
        xor    ecx, ecx
        db  039h, 00Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_7BC1
        db  039h, 00Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_7BC1
        db  0A1h, 000h, 000h, 000h, 000h
        test   al, 2
        je     SHORT loc_5_7BC1
        test   al, 8
        jne    SHORT loc_5_7BC1
        db  039h, 00Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_5_7BA7
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        jmp    SHORT loc_5_7BC1
loc_5_7BA7:
        push   0
        db  0B9h, 000h, 000h, 000h, 000h
        push   0
        xor    edx, edx
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_7BC1:
        ret
sub_5_7BC2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        mov    ebx, ecx
        push   edi
        lea    esi, [ebp - 8]
        mov    edi, edx
        push   eax
        push   esi
        lea    eax, [ebp - 0ch]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_5_8473
        test   eax, eax
        je     SHORT loc_5_7C17
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_5_7C24
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, esi
        call   sub_5_84FB
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_5_7C09
        mov    eax, 2
        jmp    NEAR PTR loc_5_7C92
loc_5_7C09:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], edi
        jmp    SHORT loc_5_7C90
loc_5_7C17:
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_5_7C24
        mov    eax, 13h
        jmp    SHORT loc_5_7C92
loc_5_7C24:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        mov    ecx, dword ptr [eax]
        cmp    ecx, ebx
        jb     SHORT loc_5_7C32
        mov    ecx, ebx
loc_5_7C32:
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0ch
        mov    ecx, dword ptr [eax]
        cmp    ecx, edi
        ja     SHORT loc_5_7C42
        mov    ecx, edi
loc_5_7C42:
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], ecx
        test   ecx, ecx
        je     SHORT loc_5_7C90
loc_5_7C54:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0ch], edi
        jae    SHORT loc_5_7C6F
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, esi
        call   sub_5_8530
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_5_7C54
        jmp    SHORT loc_5_7C90
loc_5_7C6F:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        dec    eax
        cmp    eax, edi
        ja     SHORT loc_5_7C90
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [eax + 0ch]
        mov    dword ptr [ecx + 0ch], edx
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, esi
        call   sub_5_8530
loc_5_7C90:
        xor    eax, eax
loc_5_7C92:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_5_7C9B:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    edi, edx
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 4]
        test   eax, eax
        je     NEAR PTR loc_5_7D85
        cmp    dword ptr [eax + 8], esi
        jb     SHORT loc_5_7CC7
        mov    eax, dword ptr [ebx + 8]
        cmp    dword ptr [eax + 0ch], edi
        jbe    NEAR PTR loc_5_7D85
loc_5_7CC7:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 8]
        push   eax
        lea    edx, [ebp - 0ch]
        push   ecx
        push   edx
        push   ebx
        mov    edx, edi
        mov    ecx, esi
        call   sub_5_8473
        test   eax, eax
        jne    NEAR PTR loc_5_7D8C
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        add    ecx, 8
        cmp    dword ptr [ecx], esi
        jb     SHORT loc_5_7D0E
        cmp    dword ptr [eax + 0ch], edi
        jbe    SHORT loc_5_7CFF
        inc    edi
        mov    dword ptr [ecx], edi
        jmp    NEAR PTR loc_5_7D8C
loc_5_7CFF:
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, ebx
        call   sub_5_8530
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_5_7D55
loc_5_7D0E:
        add    eax, 0ch
        cmp    dword ptr [eax], edi
        jbe    SHORT loc_5_7D46
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, ebx
        call   sub_5_84FB
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    SHORT loc_5_7D2A
        xor    eax, eax
        jmp    SHORT loc_5_7D91
loc_5_7D2A:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [eax + 0ch]
        mov    dword ptr [ecx + 0ch], edx
        mov    ecx, dword ptr [ebp - 8]
        inc    edi
        dec    esi
        mov    dword ptr [ecx + 8], edi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], esi
        jmp    SHORT loc_5_7D8C
loc_5_7D46:
        dec    esi
        mov    dword ptr [eax], esi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], ecx
loc_5_7D55:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_5_7D8C
loc_5_7D5B:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0ch], edi
        ja     SHORT loc_5_7D76
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, ebx
        call   sub_5_8530
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_5_7D5B
        jmp    SHORT loc_5_7D8C
loc_5_7D76:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        cmp    dword ptr [eax], edi
        ja     SHORT loc_5_7D8C
        inc    edi
        mov    dword ptr [eax], edi
        jmp    SHORT loc_5_7D8C
loc_5_7D85:
        mov    ecx, ebx
        call   sub_5_85F7
loc_5_7D8C:
        mov    eax, 1
loc_5_7D91:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_5_7D9A:
        sub    esp, 8
        push   esi
        mov    esi, ecx
        lea    ecx, [esp + 4]
        call   sub_5_856D
        lea    ecx, [esp + 4]
        mov    edx, dword ptr [esi + 4]
        call   sub_5_85B7
        pop    esi
        add    esp, 8
        ret
sub_5_7DBA:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, ecx
        lea    ecx, [ebp - 8]
        call   sub_5_856D
        mov    esi, dword ptr [esi + 4]
        test   esi, esi
        jne    SHORT loc_5_7DE1
        lea    ecx, [ebp - 8]
        xor    edx, edx
        push   dword ptr [ebp + 8]
        call   sub_5_857D
        jmp    SHORT loc_5_7E39
loc_5_7DE1:
        cmp    dword ptr [esi + 8], 0
        mov    edx, 0
        jne    SHORT loc_5_7DF3
        mov    edx, dword ptr [esi + 0ch]
        mov    esi, dword ptr [esi + 4]
        inc    edx
loc_5_7DF3:
        test   esi, esi
        je     SHORT loc_5_7E18
loc_5_7DF7:
        mov    eax, dword ptr [esi + 8]
        cmp    dword ptr [ebp + 8], eax
        jb     SHORT loc_5_7E18
        dec    eax
        lea    ecx, [ebp - 8]
        push   eax
        call   sub_5_857D
        test   eax, eax
        je     SHORT loc_5_7E30
        mov    edx, dword ptr [esi + 0ch]
        mov    esi, dword ptr [esi + 4]
        inc    edx
        test   esi, esi
        jne    SHORT loc_5_7DF7
loc_5_7E18:
        cmp    edx, dword ptr [ebp + 8]
        ja     SHORT loc_5_7E34
        push   dword ptr [ebp + 8]
        lea    ecx, [ebp - 8]
        call   sub_5_857D
        test   eax, eax
        jne    SHORT loc_5_7E34
        xor    eax, eax
        jmp    SHORT loc_5_7E39
loc_5_7E30:
        xor    eax, eax
        jmp    SHORT loc_5_7E39
loc_5_7E34:
        mov    eax, 1
loc_5_7E39:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_5_7E40:
        sub    esp, 8
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        lea    ecx, [esp + 8]
        mov    edx, dword ptr [esp + 14h]
        call   sub_5_856D
        mov    esi, dword ptr [esi + 4]
        mov    edi, dword ptr [edi + 4]
        test   esi, esi
        je     SHORT loc_5_7EA2
loc_5_7E60:
        test   edi, edi
        je     SHORT loc_5_7EA2
        mov    edx, dword ptr [edi + 8]
        mov    eax, dword ptr [esi + 8]
        cmp    edx, eax
        ja     SHORT loc_5_7E70
        mov    edx, eax
loc_5_7E70:
        mov    eax, dword ptr [edi + 0ch]
        mov    ecx, dword ptr [esi + 0ch]
        cmp    eax, ecx
        jb     SHORT loc_5_7E7C
        mov    eax, ecx
loc_5_7E7C:
        cmp    eax, edx
        jb     SHORT loc_5_7E8E
        lea    ecx, [esp + 8]
        push   eax
        call   sub_5_857D
        test   eax, eax
        je     SHORT loc_5_7EAF
loc_5_7E8E:
        mov    eax, dword ptr [esi + 0ch]
        cmp    dword ptr [edi + 0ch], eax
        jbe    SHORT loc_5_7E9B
        mov    esi, dword ptr [esi + 4]
        jmp    SHORT loc_5_7E9E
loc_5_7E9B:
        mov    edi, dword ptr [edi + 4]
loc_5_7E9E:
        test   esi, esi
        jne    SHORT loc_5_7E60
loc_5_7EA2:
        mov    eax, 1
loc_5_7EA7:
        pop    edi
        pop    esi
        add    esp, 8
        ret    4
loc_5_7EAF:
        xor    eax, eax
        jmp    SHORT loc_5_7EA7
sub_5_7EB3:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        not    esi
        mov    edi, ecx
        mov    eax, 1
        lea    ecx, [edx + esi]
        lea    ebx, [esi + 1]
        and    ebx, esi
        cmp    ebx, eax
        sbb    ebx, ebx
        add    ebx, eax
        cmp    ecx, edx
        mov    edx, eax
        adc    edx, -1
        or     ebx, edx
        lea    edx, [esi + 1]
        cmp    edx, eax
        sbb    edx, edx
        neg    edx
        or     ebx, edx
        cmp    dword ptr [ebp + 8], eax
        sbb    eax, eax
        neg    eax
        or     ebx, eax
        je     SHORT loc_5_7EFC
        xor    eax, eax
        jmp    NEAR PTR loc_5_7F9A
loc_5_7EFC:
        mov    ebx, esi
        mov    eax, dword ptr [ebp + 8]
        not    ebx
        and    ebx, ecx
        lea    edx, [ebx + eax - 1]
        mov    eax, 1
        mov    ecx, eax
        cmp    dword ptr [ebp + 10h], edx
        adc    ecx, -1
        cmp    edx, ebx
        adc    eax, -1
        or     ecx, eax
        je     SHORT loc_5_7F23
        xor    eax, eax
        jmp    SHORT loc_5_7F9A
loc_5_7F23:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 8]
        push   eax
        push   ecx
        lea    eax, [ebp - 0ch]
        mov    ecx, ebx
        push   eax
        push   dword ptr [ebp + 14h]
        call   sub_5_8473
        test   eax, eax
        jne    SHORT loc_5_7F93
loc_5_7F3D:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0ch]
        inc    eax
        je     SHORT loc_5_7FA3
        lea    ecx, [eax + esi]
        cmp    eax, ecx
        ja     SHORT loc_5_7FA7
        mov    ebx, esi
        mov    eax, dword ptr [ebp + 8]
        not    ebx
        and    ebx, ecx
        mov    edx, 1
        lea    ecx, [ebx + eax - 1]
        mov    eax, 1
        cmp    dword ptr [ebp + 10h], ecx
        adc    edx, -1
        cmp    ecx, ebx
        adc    eax, -1
        or     edx, eax
        jne    SHORT loc_5_7FAB
loc_5_7F73:
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], edx
        test   edx, edx
        je     SHORT loc_5_7F85
        cmp    dword ptr [edx + 0ch], ebx
        jb     SHORT loc_5_7F73
loc_5_7F85:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_5_7F93
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 8], ecx
        jbe    SHORT loc_5_7F3D
loc_5_7F93:
        mov    eax, 1
        mov    dword ptr [edi], ebx
loc_5_7F9A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_5_7FA3:
        xor    eax, eax
        jmp    SHORT loc_5_7F9A
loc_5_7FA7:
        xor    eax, eax
        jmp    SHORT loc_5_7F9A
loc_5_7FAB:
        xor    eax, eax
        jmp    SHORT loc_5_7F9A
sub_5_7FAF:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [ecx + 4]
        test   esi, esi
        jne    SHORT loc_5_7FD0
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_85F7
        mov    eax, 1
        jmp    NEAR PTR loc_5_8087
loc_5_7FD0:
        mov    edi, dword ptr [edx + 4]
        test   edi, edi
        jne    SHORT loc_5_7FE4
        mov    edx, dword ptr [ebp + 8]
        call   sub_5_7D9A
        jmp    NEAR PTR loc_5_8087
loc_5_7FE4:
        lea    ecx, [ebp - 8]
        mov    edx, dword ptr [ebp + 8]
        call   sub_5_856D
loc_5_7FEF:
        mov    edx, dword ptr [esi + 8]
loc_5_7FF2:
        mov    eax, dword ptr [esi + 0ch]
        mov    ecx, dword ptr [edi + 8]
        cmp    eax, ecx
        jae    SHORT loc_5_8012
        push   eax
        lea    ecx, [ebp - 8]
        call   sub_5_857D
        test   eax, eax
        je     SHORT loc_5_8060
        mov    esi, dword ptr [esi + 4]
        test   esi, esi
        je     SHORT loc_5_8064
        jmp    SHORT loc_5_7FEF
loc_5_8012:
        cmp    ecx, edx
        jbe    SHORT loc_5_8024
        dec    ecx
        push   ecx
        lea    ecx, [ebp - 8]
        call   sub_5_857D
        test   eax, eax
        je     SHORT loc_5_806B
loc_5_8024:
        mov    eax, dword ptr [edi + 0ch]
        inc    eax
        je     SHORT loc_5_806F
loc_5_802A:
        cmp    dword ptr [esi + 0ch], eax
        jae    SHORT loc_5_803D
        mov    esi, dword ptr [esi + 4]
        test   esi, esi
        jne    SHORT loc_5_802A
        mov    eax, 1
        jmp    SHORT loc_5_8087
loc_5_803D:
        mov    edx, dword ptr [esi + 8]
        cmp    edx, eax
        ja     SHORT loc_5_8046
        mov    edx, eax
loc_5_8046:
        mov    edi, dword ptr [edi + 4]
        test   edi, edi
        jne    SHORT loc_5_7FF2
        push   dword ptr [esi + 0ch]
        lea    ecx, [ebp - 8]
        call   sub_5_857D
        test   eax, eax
        jne    SHORT loc_5_8076
        xor    eax, eax
        jmp    SHORT loc_5_8087
loc_5_8060:
        xor    eax, eax
        jmp    SHORT loc_5_8087
loc_5_8064:
        mov    eax, 1
        jmp    SHORT loc_5_8087
loc_5_806B:
        xor    eax, eax
        jmp    SHORT loc_5_8087
loc_5_806F:
        mov    eax, 1
        jmp    SHORT loc_5_8087
loc_5_8076:
        lea    ecx, [ebp - 8]
        mov    edx, dword ptr [esi + 4]
        call   sub_5_85B7
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_5_8087:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_5_808F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        lea    ecx, [ebp - 8]
        mov    edx, dword ptr [ebp + 8]
        call   sub_5_856D
        mov    esi, dword ptr [esi + 4]
        mov    edi, dword ptr [edi + 4]
loc_5_80AC:
        test   esi, esi
        je     SHORT loc_5_80FF
        test   edi, edi
        je     SHORT loc_5_80FF
        mov    edx, dword ptr [esi + 8]
        mov    ecx, dword ptr [edi + 8]
        cmp    edx, ecx
        jbe    SHORT loc_5_80C6
        mov    eax, esi
        mov    edx, ecx
        mov    esi, edi
        mov    edi, eax
loc_5_80C6:
        mov    eax, dword ptr [esi + 0ch]
        test   edi, edi
        mov    esi, dword ptr [esi + 4]
        je     SHORT loc_5_80F0
loc_5_80D0:
        cmp    dword ptr [edi + 0ch], eax
        ja     SHORT loc_5_80DC
        mov    edi, dword ptr [edi + 4]
        test   edi, edi
        jne    SHORT loc_5_80D0
loc_5_80DC:
        test   edi, edi
        je     SHORT loc_5_80F0
        lea    ecx, [eax + 1]
        cmp    dword ptr [edi + 8], ecx
        ja     SHORT loc_5_80F0
        mov    eax, esi
        mov    esi, edi
        mov    edi, eax
        jmp    SHORT loc_5_80C6
loc_5_80F0:
        push   eax
        lea    ecx, [ebp - 8]
        call   sub_5_857D
        test   eax, eax
        jne    SHORT loc_5_80AC
        jmp    SHORT loc_5_8120
loc_5_80FF:
        lea    ecx, [ebp - 8]
        mov    edx, esi
        call   sub_5_85B7
        test   eax, eax
        je     SHORT loc_5_8120
        lea    ecx, [ebp - 8]
        mov    edx, edi
        call   sub_5_85B7
        test   eax, eax
        mov    eax, 1
        jne    SHORT loc_5_8122
loc_5_8120:
        xor    eax, eax
loc_5_8122:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_5_812A:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        lea    eax, [ebp - 8]
        push   esi
        push   edi
        mov    esi, edx
        push   eax
        push   ecx
        push   80000002h
        call   sub_5_D154
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_8155
        mov    eax, 25h
        jmp    NEAR PTR loc_5_824E
loc_5_8155:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0ch]
        push   eax
        push   0
        push   ecx
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        call   sub_5_D11C
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_5_817F
        mov    esi, 25h
        jmp    NEAR PTR loc_5_8241
loc_5_817F:
        cmp    dword ptr [ebp - 0ch], 3
        jne    NEAR PTR loc_5_823C
        test   byte ptr [ebp - 4], 7
        jne    NEAR PTR loc_5_823C
        xor    edi, edi
        cmp    dword ptr [ebp - 4], edi
        je     SHORT loc_5_81BA
        push   201h
        push   dword ptr [ebp - 4]
        call   sub_5_D134
        add    esp, 8
        mov    edi, eax
        test   eax, eax
        jne    SHORT loc_5_81BA
        mov    esi, 2
        jmp    NEAR PTR loc_5_8241
loc_5_81BA:
        lea    ecx, [ebp - 14h]
        mov    edx, esi
        call   sub_5_856D
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_5_81CE
        xor    esi, esi
        jmp    SHORT loc_5_8241
loc_5_81CE:
        mov    ebx, edi
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 0ch]
        push   edi
        push   ecx
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        call   sub_5_D11C
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_5_81F6
        mov    esi, 25h
        jmp    SHORT loc_5_8226
loc_5_81F6:
        xor    esi, esi
        cmp    dword ptr [ebp - 4], 8
        jb     SHORT loc_5_8226
loc_5_81FE:
        mov    edx, dword ptr [edi]
        lea    ecx, [ebp - 14h]
        add    edi, 4
        mov    eax, dword ptr [edi]
        add    edi, 4
        push   eax
        call   sub_5_857D
        test   eax, eax
        je     SHORT loc_5_8221
        sub    dword ptr [ebp - 4], 8
        cmp    dword ptr [ebp - 4], 8
        jae    SHORT loc_5_81FE
        jmp    SHORT loc_5_8226
loc_5_8221:
        mov    esi, 2
loc_5_8226:
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        jne    SHORT loc_5_8241
        push   0
        push   ebx
        call   sub_5_D0C8
        add    esp, 8
        jmp    SHORT loc_5_8241
loc_5_823C:
        mov    esi, 1fh
loc_5_8241:
        push   dword ptr [ebp - 8]
        call   sub_5_D0E4
        add    esp, 4
        mov    eax, esi
loc_5_824E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_8255:
        sub    esp, 4
        xor    eax, eax
        push   ebx
        push   esi
        push   edi
        mov    ebx, ecx
        push   ebp
        mov    edi, dword ptr [edx + 0ch]
        shl    edi, 3
        mov    ebp, edx
        test   edi, edi
        je     SHORT loc_5_8288
        push   201h
        push   edi
        call   sub_5_D134
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_5_8288
        mov    eax, 2
        jmp    NEAR PTR loc_5_830F
loc_5_8288:
        mov    esi, eax
        mov    edx, dword ptr [ebp + 4]
        test   edx, edx
        je     SHORT loc_5_82A8
loc_5_8291:
        mov    ecx, dword ptr [edx + 8]
        mov    ebp, dword ptr [edx + 0ch]
        mov    edx, dword ptr [edx + 4]
        mov    dword ptr [eax], ecx
        add    eax, 4
        mov    dword ptr [eax], ebp
        add    eax, 4
        test   edx, edx
        jne    SHORT loc_5_8291
loc_5_82A8:
        lea    eax, [esp + 10h]
        push   eax
        push   ebx
        push   80000002h
        mov    ebx, 1dh
        call   sub_5_D164
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_5_82F5
        push   edi
        push   esi
        push   3
        push   0
        db  068h, 000h, 000h, 000h, 000h
        mov    ebx, 0
        push   dword ptr [esp + 24h]
        call   sub_5_D0EC
        add    esp, 18h
        cmp    eax, 1
        adc    ebx, -1
        push   dword ptr [esp + 10h]
        call   sub_5_D0E4
        and    ebx, 1dh
        add    esp, 4
loc_5_82F5:
        test   edi, edi
        je     SHORT loc_5_830D
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        jne    SHORT loc_5_830D
        push   0
        push   esi
        call   sub_5_D0C8
        add    esp, 8
loc_5_830D:
        mov    eax, ebx
loc_5_830F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_5_8317:
        cmp    edx, 1
        sbb    eax, eax
        and    eax, 80000000h
        push   eax
        push   0
        push   48525950h
        push   10h
        push   ecx
        call   sub_5_D12C
        add    esp, 14h
        ret
sub_5_8335:
        push   esi
        cmp    edx, 1
        sbb    eax, eax
        push   edi
        mov    edi, ecx
        and    eax, 80000000h
        push   eax
        xor    esi, esi
        push   esi
        push   45525950h
        push   10h
        push   edi
        call   sub_5_D12C
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_5_838A
        db  039h, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_8383
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, esi
        je     SHORT loc_5_8379
loc_5_836C:
        cmp    eax, edi
        je     SHORT loc_5_838A
        mov    dword ptr [eax], esi
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jne    SHORT loc_5_836C
loc_5_8379:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_5_8383:
        mov    eax, 1
        jmp    SHORT loc_5_838C
loc_5_838A:
        xor    eax, eax
loc_5_838C:
        pop    edi
        pop    esi
        ret
sub_5_838F:
        push   201h
        push   10h
        call   sub_5_D134
        add    esp, 8
        test   eax, eax
        je     SHORT loc_5_83A8
        mov    dword ptr [eax], 48525950h
loc_5_83A8:
        ret
sub_5_83A9:
        xor    eax, eax
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        mov    dword ptr [ecx], eax
        jne    SHORT loc_5_83C0
        push   eax
        push   ecx
        call   sub_5_D0C8
        add    esp, 8
loc_5_83C0:
        ret
sub_5_83C1:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        xor    esi, esi
        test   eax, eax
        je     SHORT loc_5_83E2
        lea    edx, [eax + 4]
        mov    dword ptr [eax], 45525950h
        mov    ecx, dword ptr [edx]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [edx], esi
        jmp    SHORT loc_5_8405
loc_5_83E2:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        push   201h
        push   10h
        call   sub_5_D134
        add    esp, 8
        test   eax, eax
        je     SHORT loc_5_8405
        mov    dword ptr [eax], 45525950h
loc_5_8405:
        pop    esi
        ret
sub_5_8407:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx], 0
        mov    dword ptr [ecx + 4], edx
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        ret
sub_5_841D:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ecx + 4]
        db  0A3h, 000h, 000h, 000h, 000h
        ret
sub_5_8438:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        test   ecx, ecx
        je     SHORT loc_5_8468
loc_5_8442:
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        mov    eax, dword ptr [ecx + 4]
        db  0A3h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_845E
        push   0
        push   ecx
        call   sub_5_D0C8
        add    esp, 8
loc_5_845E:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        test   ecx, ecx
        jne    SHORT loc_5_8442
loc_5_8468:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        ret
sub_5_8473:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        xor    edi, edi
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 14h]
        mov    dword ptr [esi], edi
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebx], eax
        test   eax, eax
        je     SHORT loc_5_84A8
loc_5_848F:
        mov    edi, dword ptr [eax + 0ch]
        cmp    ecx, edi
        jbe    SHORT loc_5_84D6
        sub    edi, ecx
        cmp    edi, -1
        je     SHORT loc_5_84BD
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebx], eax
        test   eax, eax
        jne    SHORT loc_5_848F
loc_5_84A8:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], 0
loc_5_84B1:
        mov    eax, 1
loc_5_84B6:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_5_84BD:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        mov    dword ptr [ecx], 1
        je     SHORT loc_5_84B1
        cmp    dword ptr [eax + 8], edx
        ja     SHORT loc_5_84B1
        xor    eax, eax
        jmp    SHORT loc_5_84B6
loc_5_84D6:
        mov    eax, dword ptr [eax + 8]
        cmp    edx, eax
        jae    SHORT loc_5_84EE
        sub    eax, edx
        mov    ecx, dword ptr [ebp + 0ch]
        dec    eax
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_5_84B1
loc_5_84EE:
        mov    ecx, dword ptr [ebp + 0ch]
        xor    eax, eax
        mov    dword ptr [ecx], 1
        jmp    SHORT loc_5_84B6
sub_5_84FB:
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        call   sub_5_83C1
        test   eax, eax
        jne    SHORT loc_5_850E
        xor    eax, eax
        jmp    SHORT loc_5_852D
loc_5_850E:
        inc    dword ptr [esi + 0ch]
        test   edi, edi
        jne    SHORT loc_5_851D
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [esi + 4], eax
        jmp    SHORT loc_5_8523
loc_5_851D:
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [edi + 4], eax
loc_5_8523:
        mov    dword ptr [eax + 4], ecx
        test   ecx, ecx
        jne    SHORT loc_5_852D
        mov    dword ptr [esi + 8], eax
loc_5_852D:
        pop    edi
        pop    esi
        ret
sub_5_8530:
        push   esi
        push   edi
        lea    esi, [ecx + 0ch]
        mov    edi, ecx
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     SHORT loc_5_8540
        dec    eax
        mov    dword ptr [esi], eax
loc_5_8540:
        test   edx, edx
        jne    SHORT loc_5_854F
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edi + 4], eax
        jmp    SHORT loc_5_8558
loc_5_854F:
        mov    ecx, dword ptr [edx + 4]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
loc_5_8558:
        cmp    dword ptr [edi + 8], ecx
        jne    SHORT loc_5_8560
        mov    dword ptr [edi + 8], edx
loc_5_8560:
        mov    esi, dword ptr [ecx + 4]
        call   sub_5_8407
        mov    eax, esi
        pop    edi
        pop    esi
        ret
sub_5_856D:
        mov    dword ptr [ecx + 4], 0
        mov    dword ptr [ecx], edx
        mov    ecx, edx
        jmp    NEAR PTR sub_5_85F7
sub_5_857D:
        push   ebx
        push   esi
        mov    esi, dword ptr [ecx]
        push   edi
        mov    edi, edx
        mov    ebx, ecx
        mov    ecx, esi
        mov    edx, dword ptr [ebx + 4]
        call   sub_5_84FB
        test   eax, eax
        jne    SHORT loc_5_859F
        mov    ecx, esi
        call   sub_5_85F7
        xor    eax, eax
        jmp    SHORT loc_5_85B1
loc_5_859F:
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 8], edi
        mov    dword ptr [eax + 0ch], ecx
        mov    dword ptr [ebx + 4], eax
        mov    eax, 1
loc_5_85B1:
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_5_85B7:
        push   esi
        push   edi
        mov    esi, edx
        test   esi, esi
        je     SHORT loc_5_85E4
        mov    edi, dword ptr [ecx]
        mov    eax, dword ptr [ecx + 4]
loc_5_85C4:
        mov    edx, eax
        mov    ecx, edi
        call   sub_5_84FB
        test   eax, eax
        je     SHORT loc_5_85EC
        mov    ecx, dword ptr [esi + 8]
        mov    edx, dword ptr [esi + 0ch]
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [eax + 0ch], edx
        test   esi, esi
        jne    SHORT loc_5_85C4
loc_5_85E4:
        mov    eax, 1
loc_5_85E9:
        pop    edi
        pop    esi
        ret
loc_5_85EC:
        mov    ecx, edi
        call   sub_5_85F7
        xor    eax, eax
        jmp    SHORT loc_5_85E9
sub_5_85F7:
        push   esi
        push   edi
        mov    esi, ecx
        xor    edi, edi
        cmp    dword ptr [esi + 0ch], edi
        je     SHORT loc_5_8610
        call   sub_5_841D
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [esi + 8], edi
        mov    dword ptr [esi + 0ch], edi
loc_5_8610:
        pop    edi
        pop    esi
        ret
sub_5_8613:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    eax, dword ptr [ecx + 4]
        mov    edi, edx
        mov    esi, ecx
        cmp    byte ptr [eax], 0
        jne    SHORT loc_5_862E
        xor    eax, eax
        jmp    NEAR PTR loc_5_86B6
loc_5_862E:
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        je     SHORT loc_5_863B
        xor    eax, eax
        jmp    SHORT loc_5_86B6
loc_5_863B:
        test   byte ptr [ebp + 0ch], 1
        je     SHORT loc_5_867A
        push   0
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [edi + 28h]
        push   ecx
        xor    edx, edx
        push   1
        mov    ecx, esi
        mov    dword ptr [ebp - 4], 0d8h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_5_87D6
        test   eax, eax
        je     SHORT loc_5_866A
        xor    eax, eax
        jmp    SHORT loc_5_86B6
loc_5_866A:
        push   28h
        db  0BAh, 000h, 000h, 000h, 000h
        mov    ecx, edi
        call   sub_5_5B5D
        jmp    SHORT loc_5_8690
loc_5_867A:
        db  0BAh, 000h, 000h, 000h, 000h
        mov    ecx, edi
        call   sub_5_C486
        mov    edx, dword ptr [esi + 4]
        mov    ecx, edi
        call   sub_5_C44E
loc_5_8690:
        mov    esi, dword ptr [ebp + 8]
        test   esi, esi
        je     SHORT loc_5_86B1
        cmp    byte ptr [esi], 0
        je     SHORT loc_5_86B1
        db  0BAh, 000h, 000h, 000h, 000h
        mov    ecx, edi
        call   sub_5_C44E
        mov    edx, esi
        mov    ecx, edi
        call   sub_5_C44E
loc_5_86B1:
        mov    eax, 1
loc_5_86B6:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
sub_5_86BE:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        xor    ebx, ebx
        test   byte ptr [ebp + 9], 1
        je     SHORT loc_5_86D8
        mov    edi, 80000001h
        jmp    SHORT loc_5_86EF
loc_5_86D8:
        test   byte ptr [ebp + 9], 2
        mov    edi, 80000005h
        jne    SHORT loc_5_86EF
        mov    edi, 80000002h
        cmp    edx, 1
        sbb    ebx, ebx
        neg    ebx
loc_5_86EF:
        test   ebx, ebx
        je     SHORT loc_5_870B
        mov    eax, dword ptr [ebp + 8]
        and    eax, 1
        mov    ecx, dword ptr [esi + eax*4 + 80h]
        mov    dword ptr [ebp - 4], ecx
        test   ecx, ecx
        je     SHORT loc_5_870B
        mov    eax, ecx
        jmp    SHORT loc_5_875D
loc_5_870B:
        push   dword ptr [ebp + 8]
        push   edx
        db  0BAh, 000h, 000h, 000h, 000h
        mov    ecx, esi
        call   sub_5_8613
        test   eax, eax
        je     SHORT loc_5_875B
        cmp    dword ptr [ebp + 0ch], 0
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        je     SHORT loc_5_8736
        call   sub_5_D164
        jmp    SHORT loc_5_873B
loc_5_8736:
        call   sub_5_D154
loc_5_873B:
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_5_875B
        test   ebx, ebx
        je     SHORT loc_5_8756
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        and    ecx, 1
        mov    dword ptr [esi + ecx*4 + 80h], eax
loc_5_8756:
        mov    eax, dword ptr [ebp - 4]
        jmp    SHORT loc_5_875D
loc_5_875B:
        xor    eax, eax
loc_5_875D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_5_8766:
        cmp    dword ptr [ecx + 80h], edx
        je     SHORT loc_5_877F
        cmp    dword ptr [ecx + 84h], edx
        je     SHORT loc_5_877F
        push   edx
        call   sub_5_D0E4
        add    esp, 4
loc_5_877F:
        ret
sub_5_8780:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        mov    eax, dword ptr [esi + 4]
        cmp    byte ptr [eax], 0
        je     SHORT loc_5_87C1
        lea    edi, [esi + 80h]
        xor    ebx, ebx
        mov    eax, dword ptr [edi]
        test   eax, eax
        je     SHORT loc_5_87A8
        push   eax
        call   sub_5_D0E4
        add    esp, 4
        mov    dword ptr [edi], ebx
loc_5_87A8:
        add    esi, 84h
        xor    edi, edi
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     SHORT loc_5_87C1
        push   eax
        call   sub_5_D0E4
        add    esp, 4
        mov    dword ptr [esi], edi
loc_5_87C1:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_87CA:
        db  0BAh, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        jmp    NEAR PTR sub_5_CC30
sub_5_87D6:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        push   0
        push   dword ptr [ebp + 18h]
        call   sub_5_86BE
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_5_87F8
        mov    eax, 25h
        jmp    SHORT loc_5_8864
loc_5_87F8:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        lea    edx, [ebp - 4]
        push   edx
        push   0
        push   dword ptr [ebp + 8]
        push   edi
        call   sub_5_D11C
        add    esp, 18h
        mov    ebx, eax
        mov    edx, edi
        mov    ecx, esi
        call   sub_5_8766
        test   ebx, ebx
        je     SHORT loc_5_883B
        cmp    ebx, 2
        je     SHORT loc_5_8858
        cmp    ebx, 0eah
        je     SHORT loc_5_885F
        cmp    ebx, 3f4h
        je     SHORT loc_5_8858
        mov    eax, 1dh
        jmp    SHORT loc_5_8864
loc_5_883B:
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        je     SHORT loc_5_8854
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_5_8854
        cmp    eax, dword ptr [ebp - 4]
        je     SHORT loc_5_8854
        mov    eax, 26h
        jmp    SHORT loc_5_8864
loc_5_8854:
        xor    eax, eax
        jmp    SHORT loc_5_8864
loc_5_8858:
        mov    eax, 25h
        jmp    SHORT loc_5_8864
loc_5_885F:
        mov    eax, 1ah
loc_5_8864:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
_PTEXT ends
        end
