; qic117_lcode_01_02.asm -- QIC117 object 1 segment _LTEXT  (part 2/3)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_4656:
        db  0A1h, 014h, 0D0h, 040h, 000h
        mov    al, byte ptr [eax + 8]
        or     al, 3
        push   eax
        push   1eh
        call   sub_1_1D05
        add    esp, 8
        push   1388h
        call   sub_1_0720
        add    esp, 4
        ret
sub_1_4679:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 16h]
        push   esi
        mov    word ptr [ebp - 6], ax
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        movzx  eax, ax
        add    ax, 2
        db  08Bh, 035h, 014h, 0D0h, 040h, 000h
        mov    word ptr [ebp - 4], ax
        push   dword ptr [ebp - 4]
        call   sub_1_3281
        add    esp, 4
        mov    bl, al
        mov    byte ptr [esi + 2], 4
        push   4
        push   dword ptr [ebp - 4]
        call   sub_1_3276
        add    esp, 8
        test   bl, 4
        jne    SHORT loc_1_46D5
        push   1388h
        call   sub_1_0720
        add    esp, 4
loc_1_46D5:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 0
        mov    al, byte ptr [eax + 10h]
        movzx  edi, al
        imul   edi, edi, 19h
loc_1_46E5:
        movzx  eax, byte ptr [ebp - 1]
        add    eax, dword ptr [ebp + 8]
        mov    bl, byte ptr [eax + edi + 3dh]
        mov    al, bl
        and    bl, 0eh
        or     al, 0fh
        push   eax
        mov    byte ptr [esi], al
        push   dword ptr [ebp - 6]
        call   sub_1_3276
        add    esp, 8
        mov    byte ptr [esi + 2], bl
        push   ebx
        push   dword ptr [ebp - 4]
        call   sub_1_3276
        add    esp, 8
        push   0ah
        call   sub_1_0720
        add    esp, 4
        inc    byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 1], 19h
        jb     SHORT loc_1_46E5
        or     byte ptr [esi + 4], 8
        push   18h
        call   sub_1_3B66
        add    esp, 4
        and    al, 0fh
        cmp    al, 4
        jne    SHORT loc_1_4799
        push   1eh
        call   sub_1_3B66
        mov    byte ptr [esi + 8], al
        add    esp, 4
        mov    bl, al
        mov    al, bl
        xor    al, 2
        push   eax
        push   1eh
        call   sub_1_1D05
        add    esp, 8
        push   1eh
        call   sub_1_3B66
        add    esp, 4
        cmp    byte ptr [esi + 8], al
        jne    SHORT loc_1_4795
        push   4
        or     bl, 2
        push   18h
        call   sub_1_1D05
        add    esp, 8
        push   ebx
        push   1eh
        call   sub_1_1D05
        add    esp, 8
        push   0
        push   19h
        call   sub_1_1D05
        add    esp, 8
        call   sub_1_590F
        jmp    SHORT loc_1_47B0
loc_1_4795:
        push   1
        jmp    SHORT loc_1_479B
loc_1_4799:
        push   2
loc_1_479B:
        push   15923h
        push   1598h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    dword ptr [ebp - 0ch], eax
loc_1_47B0:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_47BA:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  08Bh, 035h, 014h, 0D0h, 040h, 000h
        push   ebp
        xor    ebx, ebx
        xor    edi, edi
        mov    ebp, dword ptr [esp + 1ch]
        mov    byte ptr [esp + 13h], bl
        push   ebp
        mov    byte ptr [ebp + 0eh], bl
        mov    byte ptr [esi + 22h], bl
        call   sub_1_4B6B
        add    esp, 4
        test   al, al
        jne    SHORT loc_1_47F8
        push   ebp
        call   sub_1_4B68
        add    esp, 4
        test   al, al
        jne    SHORT loc_1_47F8
        call   sub_1_4B4E
loc_1_47F8:
        call   sub_1_3288
        mov    al, byte ptr [esi + 3]
        and    al, 0feh
        or     al, 2
        push   eax
        push   18h
        call   sub_1_1D05
        add    esp, 8
        xor    al, al
loc_1_4811:
        movzx  ecx, al
        mov    byte ptr [esp + ecx + 14h], al
        inc    al
        cmp    al, 4
        jb     SHORT loc_1_4811
        push   0
        push   2
        call   sub_1_558B
        lea    eax, [esp + 1ch]
        add    esp, 8
        mov    byte ptr [esi + 14h], 2
        push   0
        push   4
        push   eax
        call   sub_1_39F7
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_4857
        push   1
        push   15922h
        push   eax
        call   sub_1_1CB6
        add    esp, 0ch
        mov    edi, eax
        jmp    SHORT loc_1_48C4
loc_1_4857:
        push   0
        push   3
        call   sub_1_558B
        lea    eax, [esp + 1ch]
        add    esp, 8
        mov    byte ptr [esi + 15h], 1
        push   0
        push   4
        push   eax
        call   sub_1_398D
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_4890
        push   2
        push   15922h
        push   eax
        call   sub_1_1CB6
        add    esp, 0ch
        mov    edi, eax
        jmp    SHORT loc_1_48C4
loc_1_4890:
        xor    al, al
loc_1_4892:
        movzx  edx, al
        mov    cl, byte ptr [esp + 14h]
        add    cl, al
        and    cl, 3
        cmp    byte ptr [esp + edx + 14h], cl
        je     SHORT loc_1_48A6
        mov    bl, 1
loc_1_48A6:
        inc    al
        cmp    al, 4
        jb     SHORT loc_1_4892
        cmp    bl, 1
        jne    SHORT loc_1_48BF
        cmp    byte ptr [esi + 22h], 0
        jne    SHORT loc_1_48BF
        mov    byte ptr [esi + 22h], 4
        xor    bl, bl
        jmp    SHORT loc_1_48C4
loc_1_48BF:
        mov    byte ptr [esp + 13h], 1
loc_1_48C4:
        cmp    byte ptr [esp + 13h], 0
        jne    SHORT loc_1_48D3
        test   edi, edi
        je     NEAR PTR loc_1_47F8
loc_1_48D3:
        test   edi, edi
        jne    SHORT loc_1_48F0
        test   bl, bl
        jne    SHORT loc_1_48DF
        mov    byte ptr [ebp + 0eh], 2
loc_1_48DF:
        mov    byte ptr [esi + 15h], 2
        push   1
        push   ebp
        call   sub_1_2F18
        add    esp, 8
        mov    edi, eax
loc_1_48F0:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_48FA:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        lea    eax, [ebp - 0ch]
        push   esi
        db  08Bh, 01Dh, 014h, 0D0h, 040h, 000h
        push   edi
        push   eax
        xor    edi, edi
        call   sub_1_5ADD
        add    esp, 4
        mov    esi, dword ptr [ebp + 8]
        add    esi, 24h
        lea    eax, [ebp - 1]
        push   eax
        lea    ecx, [ebp - 0ch]
        push   esi
        push   ecx
        call   sub_1_5A69
        add    esp, 0ch
        cmp    byte ptr [esi], 0ffh
        jne    SHORT loc_1_4963
        lea    eax, [ebp - 1]
        push   eax
        push   7
        db  0FFh, 035h, 090h, 0CFh, 040h, 000h
        call   sub_1_5953
        add    esp, 0ch
        mov    byte ptr [esi], al
        cmp    al, 0ffh
        jne    SHORT loc_1_4963
        lea    eax, [ebp - 1]
        push   eax
        push   5
        db  0FFh, 035h, 090h, 0CFh, 040h, 000h
        call   sub_1_5953
        add    esp, 0ch
        mov    byte ptr [esi], al
loc_1_4963:
        mov    al, byte ptr [esi]
        cmp    al, 0ffh
        je     SHORT loc_1_49A5
        mov    byte ptr [ebx + 16h], al
        mov    al, byte ptr [ebx + 8]
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_1_4979
        and    al, 0efh
        jmp    SHORT loc_1_497B
loc_1_4979:
        or     al, 10h
loc_1_497B:
        push   eax
        push   1eh
        call   sub_1_1D05
        add    esp, 8
        push   21h
        call   sub_1_3281
        add    esp, 4
        mov    dl, 1
        mov    cl, byte ptr [esi]
        shl    dl, cl
        or     al, dl
        push   eax
        push   21h
        call   sub_1_3276
        add    esp, 8
        jmp    SHORT loc_1_49BB
loc_1_49A5:
        push   1
        push   15926h
        push   159bh
        call   sub_1_1CB6
        add    esp, 0ch
        mov    edi, eax
loc_1_49BB:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_49C4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], bl
        mov    byte ptr [ebp - 2], bl
        cmp    word ptr [esi + 16h], 0cch
        jne    SHORT loc_1_49E3
        push   2
        jmp    SHORT loc_1_4A55
loc_1_49E3:
        mov    byte ptr [esi + 10h], 0
loc_1_49E7:
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_1_4A1D
        cmp    byte ptr [ebp - 2], 0
        je     SHORT loc_1_49FE
        push   0
        push   esi
        call   sub_1_2F18
        add    esp, 8
loc_1_49FE:
        push   esi
        call   sub_1_4679
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_4A11
        mov    byte ptr [ebp - 1], 1
loc_1_4A11:
        mov    al, byte ptr [esi + 10h]
        inc    al
        mov    byte ptr [esi + 10h], al
        cmp    al, 8
        jb     SHORT loc_1_49E7
loc_1_4A1D:
        push   ebx
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1598h
        jne    SHORT loc_1_4A34
        cmp    byte ptr [ebp - 2], 0
        jne    SHORT loc_1_4A34
        xor    ebx, ebx
loc_1_4A34:
        inc    byte ptr [ebp - 2]
        test   ebx, ebx
        jne    SHORT loc_1_4A47
        cmp    byte ptr [ebp - 2], 2
        jae    SHORT loc_1_4A47
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_49E3
loc_1_4A47:
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_1_4A69
        cmp    byte ptr [ebp - 2], 2
        jne    SHORT loc_1_4A69
        push   1
loc_1_4A55:
        push   15921h
        push   1599h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebx, eax
loc_1_4A69:
        mov    eax, ebx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_4A71:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 2], al
        mov    byte ptr [ebp - 1], al
        cmp    byte ptr [ecx + 1fh], 1
        jne    SHORT loc_1_4A8F
        mov    byte ptr [ebp - 2], 2
        jmp    SHORT loc_1_4AF4
loc_1_4A8F:
        mov    bl, 1
        call   sub_1_3405
        push   81h
        call   sub_1_3281
        add    esp, 4
        mov    byte ptr [ebp - 3], al
loc_1_4AA6:
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_1_4AD5
        push   ebx
        push   81h
        call   sub_1_3276
        add    esp, 8
        push   81h
        call   sub_1_3281
        add    esp, 4
        cmp    al, bl
        je     SHORT loc_1_4ACF
        mov    byte ptr [ebp - 1], 1
loc_1_4ACF:
        add    bl, bl
        test   bl, bl
        jne    SHORT loc_1_4AA6
loc_1_4AD5:
        push   dword ptr [ebp - 3]
        push   81h
        call   sub_1_3276
        add    esp, 8
        call   sub_1_33EF
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_1_4AF4
        mov    byte ptr [ebp - 2], 1
loc_1_4AF4:
        mov    al, byte ptr [ebp - 2]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_4AFC:
        db  08Bh, 00Dh, 014h, 0D0h, 040h, 000h
        movzx  eax, byte ptr [ecx + 22h]
        test   eax, eax
        je     SHORT loc_1_4B20
        cmp    eax, 1
        je     SHORT loc_1_4B27
        cmp    eax, 2
        jl     SHORT loc_1_4B4D
        cmp    eax, 3
        jle    SHORT loc_1_4B20
        cmp    eax, 4
        je     SHORT loc_1_4B34
        jmp    SHORT loc_1_4B4D
loc_1_4B20:
        mov    al, byte ptr [ecx + 2]
        or     al, 20h
        jmp    SHORT loc_1_4B39
loc_1_4B27:
        mov    al, byte ptr [ecx + 23h]
        and    al, 7fh
        push   eax
        mov    byte ptr [ecx + 23h], al
        push   65h
        jmp    SHORT loc_1_4B45
loc_1_4B34:
        mov    al, byte ptr [ecx + 2]
        or     al, 80h
loc_1_4B39:
        push   eax
        mov    byte ptr [ecx + 2], al
        movzx  eax, word ptr [ecx + 18h]
        add    eax, 2
        push   eax
loc_1_4B45:
        call   sub_1_3276
        add    esp, 8
loc_1_4B4D:
        ret
sub_1_4B4E:
        sub    esp, 8
        push   esi
        push   edi
        db  0BEh, 058h, 0D0h, 040h, 000h
        lea    edi, [esp + 8]
        movsd
        movsw
        movsb
        xor    al, al
        pop    edi
        pop    esi
        add    esp, 8
        ret
sub_1_4B68:
        xor    al, al
        ret
sub_1_4B6B:
        push   ebx
        push   esi
        db  08Bh, 035h, 014h, 0D0h, 040h, 000h
        xor    bl, bl
        call   sub_1_3405
        push   22h
        call   sub_1_3281
        movzx  ax, al
        mov    word ptr [esi + 26h], ax
        add    esp, 4
        test   al, 1
        jne    NEAR PTR loc_1_4CB5
        push   23h
        call   sub_1_3281
        add    esp, 4
        mov    byte ptr [esi + 28h], al
        test   al, 80h
        je     NEAR PTR loc_1_4CB5
        push   0fc23h
        call   sub_1_3281
        add    esp, 4
        push   0f023h
        call   sub_1_3281
        add    esp, 4
        push   0c023h
        call   sub_1_3281
        add    esp, 4
        push   23h
        call   sub_1_3281
        add    esp, 4
        push   23h
        call   sub_1_3281
        add    esp, 4
        test   al, 80h
        jne    NEAR PTR loc_1_4CB5
        push   61h
        push   24h
        call   sub_1_3276
        add    esp, 8
        push   25h
        call   sub_1_3281
        add    esp, 4
        mov    byte ptr [esi + 2ah], al
        test   al, 1
        jne    SHORT loc_1_4C25
        push   61h
        push   24h
        call   sub_1_3276
        add    esp, 8
        mov    al, byte ptr [esi + 2ah]
        or     al, 1
        push   eax
        push   24h
        call   sub_1_3276
        add    esp, 8
loc_1_4C25:
        push   0fah
        push   24h
        call   sub_1_3276
        add    esp, 8
        push   1
        push   25h
        call   sub_1_3276
        add    esp, 8
        push   102h
        call   sub_1_3281
        add    esp, 4
        mov    byte ptr [esi + 29h], al
        xor    eax, eax
        mov    al, byte ptr [esi + 29h]
        and    eax, 60h
        je     SHORT loc_1_4C66
        cmp    eax, 20h
        je     SHORT loc_1_4C72
        cmp    eax, 40h
        je     SHORT loc_1_4C7E
        jmp    SHORT loc_1_4C8C
loc_1_4C66:
        mov    eax, dword ptr [esp + 0ch]
        cmp    word ptr [eax + 16h], 378h
        jmp    SHORT loc_1_4C88
loc_1_4C72:
        mov    eax, dword ptr [esp + 0ch]
        cmp    word ptr [eax + 16h], 278h
        jmp    SHORT loc_1_4C88
loc_1_4C7E:
        mov    eax, dword ptr [esp + 0ch]
        cmp    word ptr [eax + 16h], 3bch
loc_1_4C88:
        jne    SHORT loc_1_4C8C
        mov    bl, 1
loc_1_4C8C:
        cmp    bl, 1
        jne    SHORT loc_1_4CAE
        mov    byte ptr [esi + 22h], 3
        mov    al, byte ptr [esi + 29h]
        test   al, 80h
        jne    SHORT loc_1_4CB5
        or     al, 80h
        push   eax
        push   102h
        call   sub_1_3276
        add    esp, 8
        jmp    SHORT loc_1_4CB5
loc_1_4CAE:
        xor    bl, bl
        call   sub_1_2A36
loc_1_4CB5:
        call   sub_1_33EF
        mov    al, bl
        pop    esi
        pop    ebx
        ret
sub_1_4CBF:
        push   esi
        push   edi
        db  08Bh, 03Dh, 014h, 0D0h, 040h, 000h
        push   10h
        call   sub_1_3B66
        add    esp, 4
        movzx  esi, al
        push   11h
        call   sub_1_3B66
        movzx  eax, al
        shl    eax, 8
        add    esp, 4
        or     esi, eax
        mov    al, byte ptr [edi + 8]
        and    al, 2
        cmp    al, 1
        mov    al, byte ptr [edi + 3]
        sbb    ecx, ecx
        and    al, 0dfh
        push   eax
        push   18h
        lea    edx, [ecx*4 + 8]
        sub    esi, edx
        call   sub_1_1D05
        add    esp, 8
        push   esi
        push   10h
        shr    esi, 8
        call   sub_1_1D05
        add    esp, 8
        push   esi
        push   11h
        call   sub_1_1D05
        add    esp, 8
        mov    al, byte ptr [edi + 3]
        and    al, 0efh
        or     al, 20h
        push   eax
        push   18h
        call   sub_1_1D05
        add    esp, 8
        push   2
        call   sub_1_0720
        add    esp, 4
        push   1ch
        call   sub_1_2C87
        add    esp, 4
        pop    edi
        pop    esi
        ret
sub_1_4D4A:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esi + 8]
        mov    byte ptr [eax + 4a0h], 1
        mov    eax, dword ptr [esi + 8]
        cmp    byte ptr [eax + 4a0h], 0
        je     SHORT loc_1_4D92
        xor    ebx, ebx
loc_1_4D68:
        push   esi
        call   sub_1_5CAA
        add    esp, 4
        mov    eax, dword ptr [esi + 8]
        cmp    byte ptr [eax + 4a0h], bl
        je     SHORT loc_1_4D87
        push   ebx
        push   ebx
        push   eax
        call   sub_1_5BDA
        add    esp, 0ch
loc_1_4D87:
        mov    eax, dword ptr [esi + 8]
        cmp    byte ptr [eax + 4a0h], bl
        jne    SHORT loc_1_4D68
loc_1_4D92:
        mov    eax, dword ptr [esi + 8]
        push   dword ptr [eax + 4b8h]
        call   sub_1_5BC8
        add    esp, 4
        pop    esi
        pop    ebx
        ret
sub_1_4DA6:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        xor    bl, bl
        mov    esi, dword ptr [esp + 20h]
        push   ebp
        push   dword ptr [esi + 110h]
        call   sub_1_60D4
        add    esp, 4
        cmp    al, 0ffh
        je     SHORT loc_1_4DE6
        push   1
        push   1101fh
        push   1150h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    dword ptr [esi + 0d4h], eax
        mov    al, 1
        jmp    NEAR PTR loc_1_504A
loc_1_4DE6:
        cmp    dword ptr [esi + 34h], 0
        jne    NEAR PTR loc_1_4E86
        lea    edi, [esi + 0d4h]
        push   2
        lea    eax, [esp + 16h]
        push   eax
        push   esi
        call   sub_1_5FD8
        add    esp, 0ch
        mov    dword ptr [edi], eax
        test   eax, eax
        jne    NEAR PTR loc_1_4EE1
        test   byte ptr [esp + 12h], 0c0h
        jne    SHORT loc_1_4E78
        push   dword ptr [esi + 110h]
        call   sub_1_60CE
        add    esp, 4
        cmp    al, 1
        je     SHORT loc_1_4E4A
        mov    al, byte ptr [esi + 5]
        or     al, 20h
        cmp    al, byte ptr [esp + 12h]
        je     SHORT loc_1_4E4A
        push   1
        push   1101fh
        push   115dh
        call   sub_1_1CB6
        add    esp, 0ch
        mov    dword ptr [edi], eax
loc_1_4E4A:
        mov    al, byte ptr [esp + 13h]
        cmp    byte ptr [esi + 0bbh], al
        je     SHORT loc_1_4E6C
        push   1
        push   1101fh
        push   1157h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    dword ptr [edi], eax
loc_1_4E6C:
        mov    al, byte ptr [esp + 13h]
        mov    byte ptr [esi + 10bh], al
        jmp    SHORT loc_1_4EE1
loc_1_4E78:
        push   2
        push   1101fh
        push   115dh
        jmp    SHORT loc_1_4ED7
loc_1_4E86:
        push   dword ptr [esi + 94h]
        push   dword ptr [esi + 90h]
        push   dword ptr [esi + 0cch]
        push   1
        push   dword ptr [esi + 110h]
        call   sub_1_5F42
        lea    ecx, [esp + 2ch]
        add    esp, 14h
        lea    edi, [esi + 0d4h]
        push   7
        push   ecx
        push   esi
        call   sub_1_5FD8
        add    esp, 0ch
        mov    dword ptr [edi], eax
        test   eax, eax
        jne    SHORT loc_1_4EE1
        test   byte ptr [esp + 18h], 0c0h
        je     SHORT loc_1_4EE1
        push   1
        push   1101fh
        push   115fh
loc_1_4ED7:
        call   sub_1_1CB6
        add    esp, 0ch
        mov    dword ptr [edi], eax
loc_1_4EE1:
        cmp    dword ptr [edi], 0
        jne    NEAR PTR loc_1_5046
        movzx  eax, word ptr [esi + 7ch]
        cmp    eax, dword ptr [esi + 34h]
        je     NEAR PTR loc_1_4FE6
        movzx  eax, word ptr [esi + 0d0h]
        lea    ebp, [esi + 94h]
        mov    ecx, dword ptr [esi + eax*4 + 0c4h]
        mov    dword ptr [esi + 90h], ecx
        movzx  eax, word ptr [esi + 48h]
        shl    eax, 2
        mov    dword ptr [ebp], eax
        push   dword ptr [esi + 110h]
        call   sub_1_384C
        add    esp, 4
        test   al, al
        je     SHORT loc_1_4F48
        mov    ax, word ptr [esi + 48h]
        push   ebp
        shl    ax, 2
        push   0
        movzx  ecx, word ptr [esi + 0d0h]
        movzx  eax, ax
        imul   ecx, eax
        push   ecx
        jmp    SHORT loc_1_4F55
loc_1_4F48:
        push   ebp
        push   dword ptr [esi + 90h]
        push   dword ptr [esi + 0cch]
loc_1_4F55:
        push   1
        push   dword ptr [esi + 110h]
        call   sub_1_5F59
        add    esp, 14h
        push   dword ptr [esi + 110h]
        call   sub_1_5E63
        add    esp, 4
        lea    eax, [esi + 0f8h]
        push   1
        push   6
        push   eax
        push   esi
        call   sub_1_5DDF
        add    esp, 10h
        mov    dword ptr [edi], eax
        test   eax, eax
        je     SHORT loc_1_4FAE
        push   dword ptr [ebp]
        push   dword ptr [esi + 90h]
        push   dword ptr [esi + 0cch]
        push   1
        push   dword ptr [esi + 110h]
        call   sub_1_5F42
        add    esp, 14h
        mov    bl, 1
loc_1_4FAE:
        mov    ax, word ptr [esi + 0d0h]
        mov    cx, word ptr [esi + 0d2h]
        mov    word ptr [esi + 0d0h], cx
        mov    word ptr [esi + 0d2h], ax
        mov    ax, word ptr [esi + 0d0h]
        push   eax
        push   esi
        call   sub_1_5E6F
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_4FE1
        mov    bl, 1
loc_1_4FE1:
        inc    dword ptr [esi + 34h]
        jmp    SHORT loc_1_5048
loc_1_4FE6:
        mov    byte ptr [esi + 10eh], 0
        mov    al, byte ptr [esi + 10bh]
        cmp    al, 80h
        jae    SHORT loc_1_4FFB
        add    al, 6
        jmp    SHORT loc_1_4FFD
loc_1_4FFB:
        sub    al, 6
loc_1_4FFD:
        mov    byte ptr [esp + 14h], 0fh
        mov    byte ptr [esp + 16h], al
        mov    al, byte ptr [esi + 5]
        mov    cl, byte ptr [esp + 16h]
        mov    byte ptr [esp + 15h], al
        mov    byte ptr [esi + 0bbh], cl
        push   dword ptr [esi + 110h]
        call   sub_1_5E63
        lea    ecx, [esp + 18h]
        add    esp, 4
        push   0
        push   3
        push   ecx
        push   esi
        call   sub_1_5DDF
        add    esp, 10h
        mov    dword ptr [edi], eax
        test   eax, eax
        jne    SHORT loc_1_5046
        mov    byte ptr [esi + 10fh], 1
        jmp    SHORT loc_1_5048
loc_1_5046:
        mov    bl, 1
loc_1_5048:
        mov    al, bl
loc_1_504A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
sub_1_5052:
        mov    eax, dword ptr [esp + 4]
        mov    al, byte ptr [eax + 10eh]
        ret
sub_1_505D:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    byte ptr [esi + 4e6h], 22h
        jne    SHORT loc_1_508E
        push   1
        push   1ah
        call   sub_1_1D05
        add    esp, 8
        call   sub_1_553D
        mov    ax, word ptr [esi + 4dah]
        inc    ax
        push   eax
        call   sub_1_3281
        add    esp, 4
loc_1_508E:
        cmp    byte ptr [esi + 4e1h], 0
        je     SHORT loc_1_50DE
        cmp    byte ptr [esi + 4e2h], 0
        je     SHORT loc_1_50BF
        cmp    byte ptr [esi + 4e3h], 0
        jne    SHORT loc_1_50C8
        mov    ax, word ptr [esi + 4dah]
        push   1fh
        push   0
        push   eax
        call   sub_1_53D1
        add    esp, 0ch
        jmp    SHORT loc_1_50DE
loc_1_50BF:
        cmp    byte ptr [esi + 4e3h], 0
        je     SHORT loc_1_50DE
loc_1_50C8:
        mov    ax, word ptr [esi + 4dah]
        push   4
        add    ax, 13h
        push   eax
        call   sub_1_3276
        add    esp, 8
loc_1_50DE:
        pop    esi
        ret
sub_1_50E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [ebp + 8]
        cmp    byte ptr [edi + 108h], 0
        je     SHORT loc_1_5130
        push   dword ptr [edi + 0e0h]
        push   dword ptr [edi + 110h]
        call   sub_1_60DF
        add    esp, 8
        and    al, 0c0h
        cmp    al, 0c0h
        je     NEAR PTR loc_1_51CA
        push   1
        push   1103ah
        push   1171h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    esi, eax
        jmp    NEAR PTR loc_1_51E4
loc_1_5130:
        mov    ebx, 20000h
loc_1_5135:
        push   dword ptr [edi + 0e0h]
        push   dword ptr [edi + 110h]
        call   sub_1_60DF
        add    esp, 8
        and    al, 0c0h
        cmp    al, 80h
        je     SHORT loc_1_5152
        dec    ebx
        jne    SHORT loc_1_5135
loc_1_5152:
        test   ebx, ebx
        je     SHORT loc_1_5190
        push   8
        mov    ebx, 20000h
        push   dword ptr [edi + 0dch]
        push   dword ptr [edi + 110h]
        call   sub_1_6100
        add    esp, 0ch
loc_1_5171:
        push   dword ptr [edi + 0e0h]
        push   dword ptr [edi + 110h]
        call   sub_1_60DF
        add    esp, 8
        test   al, 10h
        jne    SHORT loc_1_518C
        dec    ebx
        jne    SHORT loc_1_5171
loc_1_518C:
        test   ebx, ebx
        jne    SHORT loc_1_51A6
loc_1_5190:
        push   1
        push   1103ah
        push   1171h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    esi, eax
loc_1_51A6:
        cmp    byte ptr [ebp + 0ch], 0
        jne    SHORT loc_1_51E4
        test   esi, esi
        jne    SHORT loc_1_51F6
        push   dword ptr [edi + 0dch]
        push   dword ptr [edi + 110h]
        call   sub_1_60DF
        add    esp, 8
        mov    byte ptr [edi + 10ch], al
loc_1_51CA:
        push   dword ptr [edi + 0dch]
        push   dword ptr [edi + 110h]
        call   sub_1_60DF
        add    esp, 8
        mov    byte ptr [edi + 10ch], al
loc_1_51E4:
        test   esi, esi
        jne    SHORT loc_1_51F6
        mov    word ptr [edi + 106h], 0
        jmp    NEAR PTR loc_1_535F
loc_1_51F6:
        mov    ax, word ptr [edi + 106h]
        cmp    ax, 14h
        jbe    NEAR PTR loc_1_5356
        mov    word ptr [edi + 106h], 0
        cmp    byte ptr [edi + 11bh], 0
        jne    SHORT loc_1_5244
        mov    byte ptr [edi + 11ch], 0
        push   8
        push   dword ptr [edi + 0ech]
        push   dword ptr [edi + 110h]
        call   sub_1_6100
        add    esp, 0ch
        push   0ah
        call   sub_1_0720
        add    esp, 4
        push   0ch
        jmp    SHORT loc_1_527B
loc_1_5244:
        cmp    byte ptr [edi + 11ch], 1
        jne    SHORT loc_1_5252
        mov    bl, byte ptr [edi + 3]
        jmp    SHORT loc_1_5255
loc_1_5252:
        mov    bl, byte ptr [edi + 4]
loc_1_5255:
        and    bl, 0fbh
        push   ebx
        push   dword ptr [edi + 0ech]
        push   dword ptr [edi + 110h]
        call   sub_1_6100
        add    esp, 0ch
        or     bl, 0ch
        push   0ah
        call   sub_1_0720
        add    esp, 4
        push   ebx
loc_1_527B:
        push   dword ptr [edi + 0ech]
        push   dword ptr [edi + 110h]
        call   sub_1_6100
        add    esp, 0ch
        mov    ebx, 20000h
        push   1f4h
        call   sub_1_0720
        add    esp, 4
loc_1_52A1:
        push   dword ptr [edi + 0e0h]
        push   dword ptr [edi + 110h]
        call   sub_1_60DF
        add    esp, 8
        and    al, 0c0h
        cmp    al, 80h
        je     SHORT loc_1_52C8
        push   0ch
        call   sub_1_0720
        add    esp, 4
        dec    ebx
        jne    SHORT loc_1_52A1
loc_1_52C8:
        test   ebx, ebx
        je     SHORT loc_1_5316
        push   8
        push   dword ptr [edi + 0dch]
        push   dword ptr [edi + 110h]
        call   sub_1_6100
        add    esp, 0ch
        mov    ebx, 20000h
        push   0ch
        call   sub_1_0720
        add    esp, 4
loc_1_52F1:
        push   dword ptr [edi + 0e0h]
        push   dword ptr [edi + 110h]
        call   sub_1_60DF
        add    esp, 8
        test   al, 10h
        jne    SHORT loc_1_5316
        push   0ch
        call   sub_1_0720
        add    esp, 4
        dec    ebx
        jne    SHORT loc_1_52F1
loc_1_5316:
        push   dword ptr [edi + 0dch]
        push   dword ptr [edi + 110h]
        call   sub_1_60DF
        add    esp, 8
        mov    byte ptr [edi + 10ch], al
        push   dword ptr [edi + 0dch]
        push   dword ptr [edi + 110h]
        call   sub_1_60DF
        add    esp, 8
        mov    byte ptr [edi + 10ch], al
        push   0ch
        call   sub_1_0720
        add    esp, 4
        jmp    SHORT loc_1_535F
loc_1_5356:
        inc    ax
        mov    word ptr [edi + 106h], ax
loc_1_535F:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_5368:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        cmp    byte ptr [eax + 4e6h], 22h
        jne    SHORT loc_1_539C
        mov    ax, word ptr [eax + 4dah]
        mov    bl, 1
        inc    ax
        push   eax
        call   sub_1_3281
        add    esp, 4
        test   al, 8
        jne    SHORT loc_1_5390
        xor    bl, bl
loc_1_5390:
        cmp    bl, 1
        jne    SHORT loc_1_53CD
        call   sub_1_55CB
        jmp    SHORT loc_1_53CD
loc_1_539C:
        cmp    byte ptr [eax + 4e1h], 0
        je     SHORT loc_1_53CB
        cmp    byte ptr [eax + 4e0h], 0
        je     SHORT loc_1_53CB
        mov    ax, word ptr [eax + 4dah]
        add    ax, 6
        push   eax
        call   sub_1_3281
        add    esp, 4
        and    al, 40h
        shr    al, 6
        mov    bl, al
        jmp    SHORT loc_1_53CD
loc_1_53CB:
        mov    bl, 1
loc_1_53CD:
        mov    al, bl
        pop    ebx
        ret
sub_1_53D1:
        mov    cx, word ptr [esp + 4]
        push   ebx
        movzx  eax, cx
        cmp    eax, 210h
        jg     SHORT loc_1_53EC
        je     SHORT loc_1_5452
        cmp    eax, 180h
        je     SHORT loc_1_544E
        jmp    SHORT loc_1_544A
loc_1_53EC:
        cmp    eax, 300h
        jg     SHORT loc_1_53FE
        je     SHORT loc_1_5460
        cmp    eax, 2a0h
        je     SHORT loc_1_5459
        jmp    SHORT loc_1_544A
loc_1_53FE:
        cmp    eax, 340h
        jg     SHORT loc_1_5410
        je     SHORT loc_1_546E
        cmp    eax, 330h
        je     SHORT loc_1_5467
        jmp    SHORT loc_1_544A
loc_1_5410:
        cmp    eax, 1fe0h
        jg     SHORT loc_1_5420
        je     SHORT loc_1_544E
        cmp    eax, 370h
        jmp    SHORT loc_1_5448
loc_1_5420:
        cmp    eax, 3fe0h
        je     SHORT loc_1_5452
        cmp    eax, 5fe0h
        je     SHORT loc_1_5459
        cmp    eax, 7fe0h
        je     SHORT loc_1_5460
        cmp    eax, 9fe0h
        je     SHORT loc_1_5467
        cmp    eax, 0bfe0h
        je     SHORT loc_1_546E
        cmp    eax, 0dfe0h
loc_1_5448:
        je     SHORT loc_1_5475
loc_1_544A:
        xor    eax, eax
        jmp    SHORT loc_1_547A
loc_1_544E:
        xor    eax, eax
        jmp    SHORT loc_1_547A
loc_1_5452:
        mov    eax, 1
        jmp    SHORT loc_1_547A
loc_1_5459:
        mov    eax, 2
        jmp    SHORT loc_1_547A
loc_1_5460:
        mov    eax, 3
        jmp    SHORT loc_1_547A
loc_1_5467:
        mov    eax, 4
        jmp    SHORT loc_1_547A
loc_1_546E:
        mov    eax, 5
        jmp    SHORT loc_1_547A
loc_1_5475:
        mov    eax, 6
loc_1_547A:
        db  08Ah, 098h, 095h, 0CFh, 040h, 000h
        mov    dl, byte ptr [esp + 10h]
        add    cx, 6
        or     bl, byte ptr [esp + 0ch]
        and    dl, bl
        db  088h, 098h, 095h, 0CFh, 040h, 000h
        push   edx
        db  088h, 090h, 095h, 0CFh, 040h, 000h
        push   ecx
        call   sub_1_3276
        add    esp, 8
        pop    ebx
        ret
sub_1_54A6:
        push   esi
        mov    si, word ptr [esp + 8]
        push   esi
        call   sub_1_3281
        or     al, byte ptr [esp + 10h]
        add    esp, 4
        push   eax
        push   esi
        call   sub_1_3276
        add    esp, 8
        pop    esi
        ret
sub_1_54C5:
        push   esi
        push   edi
        db  08Bh, 03Dh, 014h, 0D0h, 040h, 000h
        mov    esi, dword ptr [esp + 0ch]
        mov    ax, word ptr [esi]
        cmp    ax, 3f2h
        je     SHORT loc_1_54EA
        cmp    byte ptr [esi + 22h], 22h
        push   0
        push   eax
        jne    SHORT loc_1_54FC
        call   sub_1_1D05
        jmp    SHORT loc_1_5501
loc_1_54EA:
        push   0ch
        push   eax
        call   sub_1_3276
        mov    ax, word ptr [esi + 2]
        add    esp, 8
        push   0ch
        push   eax
loc_1_54FC:
        call   sub_1_3276
loc_1_5501:
        add    esp, 8
        cmp    byte ptr [esi + 22h], 22h
        jne    SHORT loc_1_553A
        mov    al, byte ptr [edi + 4]
        and    al, 0f0h
        push   eax
        push   19h
        call   sub_1_1D05
        add    esp, 8
        cmp    byte ptr [edi + 20h], 0
        jne    SHORT loc_1_553A
        mov    al, byte ptr [edi + 2]
        and    al, 0efh
        push   eax
        mov    byte ptr [edi + 2], al
        mov    ax, word ptr [esi + 16h]
        add    ax, 2
        push   eax
        call   sub_1_3276
        add    esp, 8
loc_1_553A:
        pop    edi
        pop    esi
        ret
sub_1_553D:
        push   esi
        db  08Bh, 035h, 014h, 0D0h, 040h, 000h
        mov    ax, word ptr [esi + 1ah]
        test   ax, ax
        je     SHORT loc_1_5589
        dec    ax
        mov    word ptr [esi + 1ah], ax
        jne    SHORT loc_1_5589
        mov    ax, word ptr [esi + 1ch]
        test   ax, ax
        je     SHORT loc_1_5569
        push   eax
        push   19h
        call   sub_1_1D05
        add    esp, 8
loc_1_5569:
        cmp    word ptr [esi + 1eh], 0
        jne    SHORT loc_1_5589
        mov    ax, word ptr [esi + 18h]
        inc    ax
        push   eax
        call   sub_1_3281
        add    esp, 4
        push   1
        call   sub_1_0C8C
        add    esp, 4
loc_1_5589:
        pop    esi
        ret
sub_1_558B:
        db  0A1h, 014h, 0D0h, 040h, 000h
        cmp    word ptr [esp + 4], 2
        mov    dl, byte ptr [eax + 3]
        mov    cl, dl
        jne    SHORT loc_1_55A9
        mov    ax, word ptr [esp + 8]
        and    cl, 0efh
        or     al, 8
        jmp    SHORT loc_1_55B9
loc_1_55A9:
        test   cl, 3
        mov    ax, word ptr [esp + 8]
        jne    SHORT loc_1_55B7
        or     al, 10h
        jmp    SHORT loc_1_55B9
loc_1_55B7:
        or     al, 18h
loc_1_55B9:
        or     cl, al
        cmp    cl, dl
        je     SHORT loc_1_55CA
        push   ecx
        push   18h
        call   sub_1_1D05
        add    esp, 8
loc_1_55CA:
        ret
sub_1_55CB:
        push   ebx
        push   esi
        db  08Bh, 035h, 014h, 0D0h, 040h, 000h
        call   sub_1_3405
        mov    ax, word ptr [esi + 1ah]
        mov    bl, byte ptr [esi + 4]
        test   ax, ax
        lea    ecx, [eax + 1]
        mov    word ptr [esi + 1ah], cx
        jne    SHORT loc_1_5640
        db  083h, 03Dh, 0EDh, 01Fh, 040h, 000h, 000h
        jne    SHORT loc_1_55FC
        mov    word ptr [esi + 1eh], 0ffffh
        jmp    SHORT loc_1_561E
loc_1_55FC:
        call   sub_1_05A8
        test   ax, ax
        jne    SHORT loc_1_5618
        mov    word ptr [esi + 1eh], 0
        push   0
        call   sub_1_0C8C
        add    esp, 4
        jmp    SHORT loc_1_561E
loc_1_5618:
        mov    word ptr [esi + 1eh], 1
loc_1_561E:
        movzx  ax, byte ptr [esi + 4]
        mov    word ptr [esi + 1ch], ax
        movzx  ax, bl
        mov    word ptr [esi + 1ch], ax
        test   ax, ax
        je     SHORT loc_1_5640
        push   0
        push   19h
        call   sub_1_1D05
        add    esp, 8
loc_1_5640:
        call   sub_1_33EF
        mov    al, bl
        pop    esi
        pop    ebx
        ret
sub_1_564A:
        push   ebp
        mov    ebp, esp
        sub    esp, 84h
        mov    cx, 40h
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        lea    eax, [ebp - 84h]
        mov    dword ptr [ebp - 4], ebx
loc_1_5665:
        mov    byte ptr [eax], 0a5h
        inc    eax
        mov    byte ptr [eax], 0a5h
        inc    eax
        dec    cx
        jne    SHORT loc_1_5665
        xor    esi, esi
loc_1_5673:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 4], esi
        jbe    NEAR PTR loc_1_575E
        push   esi
        call   sub_1_3934
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    NEAR PTR loc_1_573B
        push   20h
        lea    eax, [ebp - 84h]
        push   80h
        push   eax
        call   sub_1_39F7
        add    esp, 0ch
        mov    edi, eax
        test   edi, edi
        jne    NEAR PTR loc_1_573B
        push   esi
        call   sub_1_3934
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_573B
        push   80h
        lea    eax, [ebp - 84h]
        push   0
        push   eax
        call   sub_1_33CF
        add    esp, 0ch
        lea    eax, [ebp - 84h]
        push   20h
        push   80h
        push   eax
        call   sub_1_398D
        add    esp, 0ch
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_573B
        mov    cx, 80h
        lea    eax, [ebp - 84h]
loc_1_56FD:
        test   bl, bl
        jne    SHORT loc_1_571B
        mov    edx, eax
        inc    eax
        cmp    byte ptr [edx], 0a5h
        je     SHORT loc_1_570B
        mov    bl, 1
loc_1_570B:
        mov    edx, eax
        inc    eax
        cmp    byte ptr [edx], 0a5h
        je     SHORT loc_1_5715
        mov    bl, 1
loc_1_5715:
        sub    cx, 2
        jne    SHORT loc_1_56FD
loc_1_571B:
        cmp    bl, 1
        jne    SHORT loc_1_5737
        push   1
        push   15943h
        push   1591h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
loc_1_5737:
        test   edi, edi
        je     SHORT loc_1_574E
loc_1_573B:
        push   1
        push   15943h
        push   edi
        call   sub_1_1CB6
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
loc_1_574E:
        add    esi, 1000h
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_5673
loc_1_575E:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_5768:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        mov    eax, 9090909h
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        db  08Bh, 035h, 014h, 0D0h, 040h, 000h
        lea    edi, [ebp - 20h]
        mov    ecx, 8
        rep stosd
        push   1bh
        call   sub_1_3B66
        add    esp, 4
        push   1ch
        call   sub_1_3B66
        add    esp, 4
        mov    al, byte ptr [esi + 8]
        or     al, 2
        push   eax
        push   1eh
        call   sub_1_1D05
        add    esp, 8
        mov    edi, dword ptr [ebp + 8]
        push   ebx
        mov    dword ptr [edi + 4], ebx
        call   sub_1_3934
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_57D7
        push   20h
        lea    eax, [ebp - 20h]
        mov    byte ptr [ebp - 20h], 0a5h
        push   1
        push   eax
        call   sub_1_39F7
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_57E9
loc_1_57D7:
        push   2
        push   15942h
        push   eax
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebx, eax
loc_1_57E9:
        test   ebx, ebx
        jne    NEAR PTR loc_1_58E1
        push   40000h
        call   sub_1_3934
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_581A
        push   20h
        lea    eax, [ebp - 20h]
        mov    byte ptr [ebp - 20h], 5ah
        push   1
        push   eax
        call   sub_1_39F7
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_582C
loc_1_581A:
        push   3
        push   15942h
        push   eax
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebx, eax
loc_1_582C:
        test   ebx, ebx
        jne    NEAR PTR loc_1_58E1
        push   0
        call   sub_1_3934
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_5856
        push   20h
        lea    eax, [ebp - 20h]
        push   1
        push   eax
        call   sub_1_398D
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_5868
loc_1_5856:
        push   4
        push   15942h
        push   eax
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebx, eax
loc_1_5868:
        test   ebx, ebx
        jne    SHORT loc_1_58E1
        cmp    byte ptr [ebp - 20h], 5ah
        jne    SHORT loc_1_587B
        mov    dword ptr [edi + 4], 40000h
        jmp    SHORT loc_1_58E1
loc_1_587B:
        cmp    byte ptr [ebp - 20h], 0a5h
        jne    SHORT loc_1_58E1
        push   40000h
        call   sub_1_3934
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_58A6
        push   20h
        lea    eax, [ebp - 20h]
        push   1
        push   eax
        call   sub_1_398D
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_58B8
loc_1_58A6:
        push   5
        push   15942h
        push   eax
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebx, eax
loc_1_58B8:
        test   ebx, ebx
        jne    SHORT loc_1_58CB
        cmp    byte ptr [ebp - 20h], 0a5h
        jne    SHORT loc_1_58CB
        mov    dword ptr [edi + 4], 100000h
        jmp    SHORT loc_1_58E1
loc_1_58CB:
        push   6
        push   15942h
        push   1590h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebx, eax
loc_1_58E1:
        test   byte ptr [esi + 8], 2
        je     SHORT loc_1_58EA
        shr    dword ptr [edi + 4], 1
loc_1_58EA:
        cmp    dword ptr [edi + 4], 0
        jne    SHORT loc_1_5906
        push   7
        push   15942h
        push   1590h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebx, eax
loc_1_5906:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_590F:
        push   esi
        push   18h
        db  08Bh, 035h, 014h, 0D0h, 040h, 000h
        call   sub_1_3B66
        add    esp, 4
        mov    byte ptr [esi + 3], al
        push   1eh
        call   sub_1_3B66
        add    esp, 4
        mov    byte ptr [esi + 8], al
        push   1bh
        call   sub_1_3B66
        add    esp, 4
        push   1ch
        call   sub_1_3B66
        add    esp, 4
        push   19h
        call   sub_1_3B66
        add    esp, 4
        mov    byte ptr [esi + 4], al
        pop    esi
        ret
sub_1_5953:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   ebp
        db  08Bh, 03Dh, 014h, 0D0h, 040h, 000h
        mov    bp, 0ffh
        push   ebx
        movzx  esi, word ptr [esp + 1ch]
        db  068h, 025h, 071h, 040h, 000h
        push   esi
        push   dword ptr [esp + 20h]
        call   sub_1_0618
        add    esp, 10h
        call   sub_1_2BE8
        mov    al, byte ptr [edi + 4]
        or     al, 8
        push   eax
        push   19h
        call   sub_1_1D05
        add    esp, 8
        mov    al, byte ptr [edi + 8]
        and    al, 0efh
        push   eax
        push   1eh
        call   sub_1_1D05
        add    esp, 8
        mov    al, byte ptr [edi + 2]
        or     al, 10h
        push   eax
        mov    byte ptr [edi + 2], al
        mov    ax, word ptr [edi + 18h]
        add    ax, 2
        push   eax
        call   sub_1_3276
        mov    cl, byte ptr [esp + 20h]
        add    esp, 8
        mov    al, 1
        shl    al, cl
        not    al
        push   eax
        push   21h
        call   sub_1_3276
        add    esp, 8
        call   sub_1_2C1D
        db  089h, 01Dh, 018h, 0D0h, 040h, 000h
        db  089h, 01Dh, 01Ch, 0D0h, 040h, 000h
        mov    bl, 0ah
loc_1_59E2:
        mov    al, byte ptr [edi + 8]
        xor    al, 10h
        push   eax
        mov    byte ptr [edi + 8], al
        push   1eh
        call   sub_1_1D05
        mov    ax, word ptr [edi + 18h]
        add    esp, 8
        inc    ax
        push   eax
        call   sub_1_3281
        add    esp, 4
        dec    bl
        jne    SHORT loc_1_59E2
        db  083h, 03Dh, 018h, 0D0h, 040h, 000h, 00Ah
        jne    SHORT loc_1_5A1F
        mov    bp, word ptr [esp + 18h]
        mov    eax, dword ptr [esp + 1ch]
        mov    byte ptr [eax], 0
        jmp    SHORT loc_1_5A34
loc_1_5A1F:
        db  083h, 03Dh, 01Ch, 0D0h, 040h, 000h, 00Ah
        jne    SHORT loc_1_5A34
        mov    bp, word ptr [esp + 18h]
        mov    eax, dword ptr [esp + 1ch]
        mov    byte ptr [eax], 10h
loc_1_5A34:
        call   sub_1_2BE8
        db  068h, 025h, 071h, 040h, 000h
        push   esi
        call   sub_1_07FC
        add    esp, 8
        mov    al, byte ptr [edi + 2]
        and    al, 0efh
        push   eax
        mov    byte ptr [edi + 2], al
        mov    ax, word ptr [edi + 18h]
        add    ax, 2
        push   eax
        call   sub_1_3276
        mov    ax, bp
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_5A69:
        push   ebx
        xor    eax, eax
        push   esi
        mov    cl, 80h
        mov    ebx, dword ptr [esp + 14h]
        push   edi
        mov    edi, dword ptr [esp + 14h]
        push   ebp
        xor    dl, dl
        mov    esi, dword ptr [esp + 14h]
        mov    byte ptr [edi], 0ffh
        mov    byte ptr [ebx], al
loc_1_5A84:
        cmp    byte ptr [edi], 0ffh
        jne    SHORT loc_1_5AD8
        xor    bp, bp
loc_1_5A8C:
        movzx  ebx, bp
        mov    bl, byte ptr [ebx + esi]
        and    bl, cl
        cmp    bl, 1
        sbb    ebx, ebx
        add    ax, ax
        inc    ebx
        or     bx, ax
        mov    ax, bx
        inc    bp
        cmp    bp, 0ah
        jb     SHORT loc_1_5A8C
        cmp    ax, 2aah
        je     SHORT loc_1_5ABE
        cmp    ax, 155h
        je     SHORT loc_1_5ABE
        shr    cl, 1
        xor    ax, ax
        jmp    SHORT loc_1_5AD1
loc_1_5ABE:
        mov    bl, 7
        sub    bl, dl
        cmp    ax, 155h
        mov    byte ptr [edi], bl
        jne    SHORT loc_1_5AD1
        mov    ebx, dword ptr [esp + 1ch]
        mov    byte ptr [ebx], 10h
loc_1_5AD1:
        inc    dl
        cmp    dl, 8
        jb     SHORT loc_1_5A84
loc_1_5AD8:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_5ADD:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  08Bh, 035h, 014h, 0D0h, 040h, 000h
        call   sub_1_3405
        push   21h
        call   sub_1_3281
        mov    byte ptr [esp + 13h], al
        add    esp, 4
        push   0ffh
        push   21h
        call   sub_1_3276
        add    esp, 8
        push   0ah
        push   20h
        call   sub_1_3276
        add    esp, 8
        mov    al, byte ptr [esi + 4]
        or     al, 8
        push   eax
        push   19h
        call   sub_1_1D05
        add    esp, 8
        mov    al, byte ptr [esi + 2]
        or     al, 10h
        push   eax
        mov    byte ptr [esi + 2], al
        mov    ax, word ptr [esi + 18h]
        xor    bl, bl
        add    ax, 2
        push   eax
        call   sub_1_3276
        mov    edi, dword ptr [esp + 1ch]
        add    esp, 8
loc_1_5B47:
        mov    ax, word ptr [esi + 18h]
        inc    ax
        push   eax
        call   sub_1_3281
        add    esp, 4
        mov    al, byte ptr [esi + 8]
        xor    al, 10h
        push   eax
        mov    byte ptr [esi + 8], al
        push   1eh
        call   sub_1_1D05
        add    esp, 8
        push   20h
        call   sub_1_3281
        add    esp, 4
        push   20h
        call   sub_1_3281
        add    esp, 4
        push   20h
        call   sub_1_3281
        movzx  ecx, bl
        add    esp, 4
        inc    bl
        mov    byte ptr [ecx + edi], al
        cmp    bl, 0ah
        jb     SHORT loc_1_5B47
        push   dword ptr [esp + 0fh]
        push   21h
        call   sub_1_3276
        add    esp, 8
        mov    al, byte ptr [esi + 2]
        and    al, 0efh
        push   eax
        mov    byte ptr [esi + 2], al
        mov    ax, word ptr [esi + 18h]
        add    ax, 2
        push   eax
        call   sub_1_3276
        add    esp, 8
        call   sub_1_33EF
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_5BC8:
        mov    eax, dword ptr [esp + 4]
        test   eax, eax
        je     SHORT loc_1_5BD9
        push   eax
        call   sub_1_0CB8
        add    esp, 4
loc_1_5BD9:
        ret
sub_1_5BDA:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        xor    edi, edi
        cmp    dword ptr [esi], edi
        je     SHORT loc_1_5C00
        push   esi
        db  0A1h, 090h, 0CFh, 040h, 000h
        db  068h, 0C8h, 071h, 040h, 000h
        push   400000h
        push   eax
        call   sub_1_079C
        add    esp, 10h
loc_1_5C00:
        mov    al, byte ptr [esp + 18h]
        test   al, al
        je     SHORT loc_1_5C40
        mov    cl, byte ptr [esi + 4afh]
        test   cl, cl
        je     SHORT loc_1_5C40
        test   al, al
        je     NEAR PTR loc_1_5CA4
        test   cl, cl
        je     NEAR PTR loc_1_5CA4
        cmp    dword ptr [esi + 4c0h], 0
        je     SHORT loc_1_5CA4
        mov    byte ptr [esi + 4b0h], 0
        mov    dword ptr [esi + 4c0h], 0
        push   2
        jmp    SHORT loc_1_5C90
loc_1_5C40:
        mov    ebx, dword ptr [esp + 14h]
        test   ebx, ebx
        je     SHORT loc_1_5C5D
        push   esi
        db  068h, 0A3h, 071h, 040h, 000h
        push   ebx
        call   sub_1_0CCC
        add    esp, 0ch
        mov    dword ptr [esi + 4bch], eax
loc_1_5C5D:
        lea    eax, [esi + 4a4h]
        mov    dword ptr [esi + 4a8h], ebx
        push   eax
        call   sub_1_6169
        add    esp, 4
        cmp    eax, 1
        je     SHORT loc_1_5C80
        cmp    dword ptr [esi + 4c0h], 0
        je     SHORT loc_1_5CA4
loc_1_5C80:
        xor    eax, eax
        push   1
        mov    byte ptr [esi + 4b0h], al
        mov    dword ptr [esi + 4c0h], eax
loc_1_5C90:
        push   1583bh
        push   15a0h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    edi, eax
loc_1_5CA4:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_5CAA:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   ebp
        mov    eax, dword ptr [esi + 8]
        cmp    dword ptr [eax + 128h], 0
        je     NEAR PTR loc_1_5DDA
loc_1_5CC2:
        mov    eax, dword ptr [esi + 8]
        mov    edi, dword ptr [eax + 128h]
        mov    eax, dword ptr [edi]
        cmp    dword ptr [eax], 150bh
        jne    SHORT loc_1_5D2A
        mov    ax, word ptr [eax + 18h]
        cmp    ax, 1112h
        je     SHORT loc_1_5D2A
        cmp    ax, 1101h
        je     SHORT loc_1_5CF1
        cmp    ax, 1100h
        je     SHORT loc_1_5CF1
        cmp    ax, 1104h
        jne    SHORT loc_1_5CFB
loc_1_5CF1:
        push   3
        call   sub_1_0D00
        add    esp, 4
loc_1_5CFB:
        push   dword ptr [edi]
        push   dword ptr [esi + 4]
        call   sub_1_62C5
        add    esp, 8
        mov    ecx, dword ptr [edi]
        mov    ax, word ptr [ecx + 18h]
        cmp    ax, 1101h
        je     SHORT loc_1_5D20
        cmp    ax, 1100h
        je     SHORT loc_1_5D20
        cmp    ax, 1104h
        jne    SHORT loc_1_5D2A
loc_1_5D20:
        push   3
        call   sub_1_0CF0
        add    esp, 4
loc_1_5D2A:
        mov    eax, dword ptr [edi]
        mov    bx, word ptr [eax + 18h]
        mov    ebp, dword ptr [eax]
        cmp    bx, 1103h
        je     SHORT loc_1_5D9A
        cmp    bx, 1100h
        jne    SHORT loc_1_5D44
        test   ebp, ebp
        jne    SHORT loc_1_5D9A
loc_1_5D44:
        cmp    bx, 1101h
        jne    SHORT loc_1_5D4F
        test   ebp, ebp
        jne    SHORT loc_1_5D9A
loc_1_5D4F:
        cmp    bx, 1102h
        jne    SHORT loc_1_5D87
        test   ebp, ebp
        je     SHORT loc_1_5D87
        push   ebp
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1167h
        je     SHORT loc_1_5D87
        push   ebp
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1164h
        je     SHORT loc_1_5D87
        push   ebp
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 115ch
        jne    SHORT loc_1_5D9A
loc_1_5D87:
        cmp    bx, 1112h
        jne    SHORT loc_1_5DA9
        mov    eax, dword ptr [esi + 8]
        cmp    byte ptr [eax + 4ach], 0
        jne    SHORT loc_1_5DA9
loc_1_5D9A:
        push   esi
        db  0A1h, 090h, 0CFh, 040h, 000h
        push   eax
        call   sub_1_388C
        add    esp, 8
loc_1_5DA9:
        push   dword ptr [edi + 4]
        push   dword ptr [edi]
        push   dword ptr [esi + 8]
        call   sub_1_1F88
        add    esp, 0ch
        mov    eax, dword ptr [esi + 8]
        add    eax, 128h
        push   eax
        call   sub_1_1F57
        add    esp, 4
        mov    ecx, dword ptr [esi + 8]
        cmp    dword ptr [ecx + 128h], 0
        jne    NEAR PTR loc_1_5CC2
loc_1_5DDA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_5DDF:
        mov    al, byte ptr [esp + 10h]
        push   ebx
        mov    ebx, dword ptr [esp + 8]
        push   esi
        push   edi
        push   ebp
        xor    edi, edi
        mov    byte ptr [ebx + 108h], al
        cmp    word ptr [esp + 1ch], di
        je     SHORT loc_1_5E46
        mov    esi, dword ptr [esp + 18h]
loc_1_5DFE:
        mov    ebp, 20000h
loc_1_5E03:
        push   dword ptr [ebx + 0e0h]
        push   dword ptr [ebx + 110h]
        call   sub_1_60DF
        add    esp, 8
        and    al, 0c0h
        cmp    al, 80h
        je     SHORT loc_1_5E20
        dec    ebp
        jne    SHORT loc_1_5E03
loc_1_5E20:
        test   ebp, ebp
        je     SHORT loc_1_5E4D
        mov    al, byte ptr [esi + edi]
        inc    edi
        push   eax
        push   dword ptr [ebx + 0dch]
        push   dword ptr [ebx + 110h]
        call   sub_1_6100
        movzx  ecx, word ptr [esp + 28h]
        add    esp, 0ch
        cmp    ecx, edi
        ja     SHORT loc_1_5DFE
loc_1_5E46:
        xor    eax, eax
loc_1_5E48:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_5E4D:
        push   1
        push   11033h
        push   115dh
        call   sub_1_1CB6
        add    esp, 0ch
        jmp    SHORT loc_1_5E48
sub_1_5E63:
        mov    eax, dword ptr [esp + 4]
        mov    byte ptr [eax + 4b0h], 1
        ret
sub_1_5E6F:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    ebx, dword ptr [esp + 14h]
        push   ebp
        movzx  ax, byte ptr [ebx + 0bah]
        cmp    word ptr [ebx + 4ch], ax
        jae    SHORT loc_1_5EB1
        mov    byte ptr [ebx + 0bah], 1
        mov    al, byte ptr [ebx + 0b8h]
        inc    al
        mov    byte ptr [ebx + 0b8h], al
        cmp    byte ptr [ebx + 50h], al
        jne    SHORT loc_1_5EB1
        mov    byte ptr [ebx + 0b8h], 0
        inc    byte ptr [ebx + 0b9h]
loc_1_5EB1:
        mov    al, byte ptr [ebx + 0b8h]
        mov    dl, byte ptr [ebx + 0b9h]
        mov    byte ptr [esp + 10h], al
        mov    byte ptr [esp + 11h], dl
        mov    byte ptr [esp + 13h], 3
        movzx  edi, word ptr [esp + 1ch]
        xor    dx, dx
        lea    ebp, [ebx + edi*4 + 0bch]
        cmp    word ptr [ebx + 48h], dx
        mov    ecx, dword ptr [ebp]
        jbe    SHORT loc_1_5F05
loc_1_5EE2:
        inc    dx
        mov    al, byte ptr [ebx + 0bah]
        mov    byte ptr [esp + 12h], al
        inc    al
        mov    byte ptr [ebx + 0bah], al
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ecx], eax
        add    ecx, 4
        cmp    word ptr [ebx + 48h], dx
        ja     SHORT loc_1_5EE2
loc_1_5F05:
        push   dword ptr [ebx + 110h]
        call   sub_1_384C
        add    esp, 4
        test   al, al
        je     SHORT loc_1_5F38
        mov    ax, word ptr [ebx + 48h]
        push   1
        shl    ax, 2
        push   2
        push   eax
        movzx  eax, ax
        imul   eax, edi
        push   eax
        push   dword ptr [ebp]
        call   sub_1_3434
        add    esp, 14h
        mov    esi, eax
loc_1_5F38:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_5F42:
        mov    eax, dword ptr [esp + 4]
        push   0
        movzx  ecx, byte ptr [eax + 4e9h]
        push   ecx
        call   sub_1_0D10
        add    esp, 8
        ret
sub_1_5F59:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 18h
        push   esi
        add    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 18h]
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 18h], eax
        mov    edx, dword ptr [ecx]
        movzx  eax, byte ptr [ebp + 0ch]
        mov    dword ptr [ebp - 14h], edx
        mov    dword ptr [ebp - 10h], eax
        movzx  ecx, byte ptr [esi + 4e9h]
        mov    dword ptr [ebp - 0ch], ecx
        movzx  eax, word ptr [esi + 4dah]
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 8], eax
        cmp    byte ptr [esi + 4e3h], 0
        jne    SHORT loc_1_5FAF
        lea    eax, [esi + 4c4h]
        push   eax
        call   sub_1_64DE
        add    esp, 4
loc_1_5FAF:
        lea    eax, [ebp - 18h]
        push   eax
        call   sub_1_0D84
        add    esp, 4
        cmp    byte ptr [esi + 4e3h], 0
        je     SHORT loc_1_5FD3
        add    esi, 4c4h
        push   esi
        call   sub_1_64DE
        add    esp, 4
loc_1_5FD3:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_5FD8:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 14h]
        push   ebp
        mov    dword ptr [esp + 10h], ebx
        cmp    byte ptr [edi + 108h], bl
        je     SHORT loc_1_6006
        mov    al, byte ptr [edi + 10ch]
        mov    ecx, dword ptr [esp + 1ch]
        mov    ebx, 1
        inc    dword ptr [esp + 1ch]
        mov    byte ptr [ecx], al
loc_1_6006:
        mov    ebp, 20000h
loc_1_600B:
        push   dword ptr [edi + 0e0h]
        push   dword ptr [edi + 110h]
        call   sub_1_60DF
        movzx  esi, al
        add    esp, 8
        dec    ebp
        je     SHORT loc_1_602D
        test   esi, 80h
        je     SHORT loc_1_600B
loc_1_602D:
        test   ebp, ebp
        jne    SHORT loc_1_603F
        push   1
        push   11036h
        push   115dh
        jmp    SHORT loc_1_6082
loc_1_603F:
        test   esi, 40h
        je     SHORT loc_1_60A1
        push   dword ptr [edi + 0dch]
        push   dword ptr [edi + 110h]
        call   sub_1_60DF
        mov    ecx, dword ptr [esp + 24h]
        add    esp, 8
        inc    ebx
        mov    byte ptr [ecx], al
        movzx  eax, word ptr [esp + 20h]
        cmp    eax, ebx
        jb     SHORT loc_1_6076
        cmp    ebx, 10h
        ja     SHORT loc_1_6076
        inc    dword ptr [esp + 1ch]
        jmp    SHORT loc_1_608E
loc_1_6076:
        push   1
        push   11036h
        push   1163h
loc_1_6082:
        call   sub_1_1CB6
        mov    dword ptr [esp + 1ch], eax
        add    esp, 0ch
loc_1_608E:
        test   esi, 40h
        je     SHORT loc_1_60A1
        cmp    dword ptr [esp + 10h], 0
        je     NEAR PTR loc_1_6006
loc_1_60A1:
        movzx  eax, word ptr [esp + 20h]
        cmp    eax, ebx
        je     SHORT loc_1_60C2
        push   2
        push   11036h
        push   1163h
        call   sub_1_1CB6
        mov    dword ptr [esp + 1ch], eax
        add    esp, 0ch
loc_1_60C2:
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_60CE:
        db  0A0h, 081h, 0CFh, 040h, 000h
        ret
sub_1_60D4:
        mov    eax, dword ptr [esp + 4]
        mov    al, byte ptr [eax + 4ach]
        ret
sub_1_60DF:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [esp + 8]
        cmp    byte ptr [eax + 4e6h], 22h
        jne    SHORT loc_1_60F7
        call   sub_1_3B66
        jmp    SHORT loc_1_60FC
loc_1_60F7:
        call   sub_1_3281
loc_1_60FC:
        add    esp, 4
        ret
sub_1_6100:
        push   dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [esp + 0ch]
        cmp    byte ptr [eax + 4e6h], 22h
        jne    SHORT loc_1_611C
        call   sub_1_1D05
        jmp    SHORT loc_1_6121
loc_1_611C:
        call   sub_1_3276
loc_1_6121:
        add    esp, 8
        ret
loc_1_6125:
        push   esi
        db  08Bh, 035h, 014h, 0D0h, 040h, 000h
        test   byte ptr [esi + 8], 10h
        je     SHORT loc_1_613A
        db  0FFh, 005h, 018h, 0D0h, 040h, 000h
        jmp    SHORT loc_1_6140
loc_1_613A:
        db  0FFh, 005h, 01Ch, 0D0h, 040h, 000h
loc_1_6140:
        mov    al, byte ptr [esi + 8]
        xor    al, 10h
        push   eax
        mov    byte ptr [esi + 8], al
        push   1eh
        call   sub_1_1D05
        mov    ax, word ptr [esi + 18h]
        add    esp, 8
        inc    ax
        push   eax
        call   sub_1_3281
        add    esp, 4
        call   sub_1_0790
        pop    esi
        ret
sub_1_6169:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   ebx
        push   ecx
        push   edx
        push   edi
        push   esi
        push   ebp
        push   ds
        push   es
        push   fs
        push   gs
        push   ss
        call   sub_1_3405
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx], esp
        db  08Bh, 025h, 08Ch, 0CFh, 040h, 000h
        call   sub_1_33EF
        pop    ss
        pop    gs
        pop    fs
        pop    es
        pop    ds
        pop    ebp
        pop    esi
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_61A3:
        sub    esp, 4
        cld
        mov    dword ptr [esp], edx
        push   dword ptr [esp]
        db  068h, 057h, 04Ah, 040h, 000h
        push   400000h
        db  0FFh, 035h, 090h, 0CFh, 040h, 000h
        call   sub_1_079C
        add    esp, 14h
        ret
loc_1_61C8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        mov    dword ptr [ebp - 8], edx
        call   sub_1_3405
        mov    eax, dword ptr [ebp - 8]
        add    eax, 4adh
        cmp    byte ptr [eax], 0
        jne    NEAR PTR loc_1_62AB
        mov    ecx, dword ptr [ebp - 8]
        cmp    byte ptr [ecx + 4a0h], 0
        je     NEAR PTR loc_1_62AB
        mov    byte ptr [eax], 1
        call   sub_1_33EF
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_629F
loc_1_620C:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax]
        lea    eax, [ebp - 2]
        push   eax
        mov    esi, dword ptr [ecx]
        mov    dx, word ptr [esi + 18h]
        lea    ecx, [ebp - 0ch]
        push   ecx
        push   edx
        call   sub_1_3532
        movzx  ecx, word ptr [ebp - 2]
        add    esp, 0ch
        add    ecx, esi
        mov    esi, dword ptr [ecx]
        add    ecx, 4
        test   esi, esi
        je     SHORT loc_1_626D
        cmp    dword ptr [ecx], esi
        jne    SHORT loc_1_6262
        movzx  eax, ax
        mov    dword ptr [esi + 4], eax
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi + 8], eax
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [edx]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 0ch], ecx
        push   dword ptr [esi]
        call   sub_1_62B5
        add    esp, 4
loc_1_6262:
        push   14h
        push   esi
        call   sub_1_041C
        add    esp, 8
loc_1_626D:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        mov    eax, dword ptr [ebp - 8]
        add    eax, 378h
        push   eax
        call   sub_1_1F88
        add    esp, 0ch
        push   dword ptr [ebp - 8]
        call   sub_1_1F57
        add    esp, 4
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ecx], 0
        jne    NEAR PTR loc_1_620C
loc_1_629F:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 4adh], 0
        jmp    SHORT loc_1_62B0
loc_1_62AB:
        call   sub_1_33EF
loc_1_62B0:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_62B5:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        db  0CDh, 020h, 001h, 000h, 02Ah, 000h
        pop    ebx
        pop    ebp
        ret
sub_1_62C5:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 10h]
        push   dword ptr [edi + 110h]
        mov    byte ptr [edi + 11eh], bl
        call   sub_1_60D4
        add    esp, 4
        cmp    al, 0ffh
        je     SHORT loc_1_62FC
        push   1
        push   11032h
        push   1150h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebx, eax
loc_1_62FC:
        test   ebx, ebx
        jne    SHORT loc_1_636C
        cmp    byte ptr [edi + 11bh], 0
        mov    esi, dword ptr [esp + 14h]
        jne    SHORT loc_1_6335
        mov    ax, word ptr [esi + 18h]
        cmp    ax, 1101h
        je     SHORT loc_1_6335
        cmp    ax, 1100h
        je     SHORT loc_1_6335
        push   1
        push   11032h
        push   1159h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebx, eax
        jmp    SHORT loc_1_6370
loc_1_6335:
        cmp    byte ptr [edi + 11dh], 0
        jne    SHORT loc_1_6370
        mov    ax, word ptr [esi + 18h]
        cmp    ax, 1102h
        je     SHORT loc_1_6370
        cmp    ax, 1100h
        je     SHORT loc_1_6370
        cmp    ax, 1101h
        je     SHORT loc_1_6370
        push   1
        push   11032h
        push   115ah
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebx, eax
        jmp    SHORT loc_1_6370
loc_1_636C:
        mov    esi, dword ptr [esp + 14h]
loc_1_6370:
        test   ebx, ebx
        jne    NEAR PTR loc_1_64D6
        cmp    byte ptr [edi + 120h], 0
        je     SHORT loc_1_63B3
        mov    ax, word ptr [esi + 18h]
        cmp    ax, 1101h
        je     SHORT loc_1_63B3
        cmp    ax, 1102h
        je     SHORT loc_1_63B3
        cmp    ax, 1104h
        je     SHORT loc_1_63B3
        cmp    ax, 1107h
        je     SHORT loc_1_63B3
        push   1
        push   11032h
        push   1164h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebx, eax
loc_1_63B3:
        test   ebx, ebx
        jne    NEAR PTR loc_1_64D6
        push   dword ptr [edi + 110h]
        call   sub_1_70A6
        add    esp, 4
        mov    ebx, eax
        push   ebx
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 15a2h
        jne    SHORT loc_1_63E5
        push   edi
        xor    ebx, ebx
        call   sub_1_6E6A
        add    esp, 4
loc_1_63E5:
        test   ebx, ebx
        jne    NEAR PTR loc_1_64D6
        movzx  eax, word ptr [esi + 18h]
        cmp    eax, 1100h
        jl     SHORT loc_1_6406
        cmp    eax, 1102h
        jle    SHORT loc_1_6417
        cmp    eax, 110ch
        je     SHORT loc_1_640A
loc_1_6406:
        push   0
        jmp    SHORT loc_1_640C
loc_1_640A:
        push   2
loc_1_640C:
        push   edi
        call   sub_1_6CF2
        add    esp, 8
        mov    ebx, eax
loc_1_6417:
        test   ebx, ebx
        jne    SHORT loc_1_6427
        push   esi
        push   edi
        call   sub_1_6956
        add    esp, 8
        mov    ebx, eax
loc_1_6427:
        push   dword ptr [edi + 110h]
        call   sub_1_60D4
        add    esp, 4
        cmp    al, 1
        je     SHORT loc_1_6480
        push   dword ptr [edi + 110h]
        call   sub_1_6943
        add    esp, 4
        test   al, al
        je     SHORT loc_1_6489
        cmp    byte ptr [edi + 11eh], 0
        jne    SHORT loc_1_6489
        push   edi
        call   sub_1_66A9
        add    esp, 4
        push   eax
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 115ch
        jne    SHORT loc_1_6489
        cmp    byte ptr [edi + 19h], 7
        jne    SHORT loc_1_6480
        cmp    byte ptr [edi + 1ah], 4
        jne    SHORT loc_1_6480
        push   edi
        call   sub_1_653B
        jmp    SHORT loc_1_6486
loc_1_6480:
        push   edi
        call   sub_1_6660
loc_1_6486:
        add    esp, 4
loc_1_6489:
        push   dword ptr [edi + 110h]
        call   sub_1_60D4
        add    esp, 4
        test   al, al
        jne    SHORT loc_1_64D6
        cmp    byte ptr [edi + 11ch], 0
        je     SHORT loc_1_64D6
        push   edi
        call   sub_1_65A2
        add    esp, 4
        push   edi
        call   sub_1_653B
        add    esp, 4
        push   1
        push   edi
        call   sub_1_6500
        add    esp, 8
        push   dword ptr [edi + 110h]
        call   sub_1_64FF
        mov    byte ptr [edi + 11dh], 0
        add    esp, 4
loc_1_64D6:
        mov    eax, ebx
        pop    edi
        mov    dword ptr [esi], ebx
        pop    esi
        pop    ebx
        ret
sub_1_64DE:
        mov    eax, dword ptr [esp + 4]
        cmp    byte ptr [eax + 1fh], 0
        je     SHORT loc_1_64FE
        cmp    byte ptr [eax + 1dh], 0
        je     SHORT loc_1_64FE
        push   1
        push   0c1h
        push   eax
        call   sub_1_70A9
        add    esp, 0ch
loc_1_64FE:
        ret
sub_1_64FF:
        ret
sub_1_6500:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_6E6A
        add    esp, 4
        cmp    byte ptr [esp + 0ch], 0
        je     SHORT loc_1_651E
        push   esi
        call   sub_1_7106
        add    esp, 4
loc_1_651E:
        mov    byte ptr [esi + 11eh], 1
        mov    byte ptr [esi + 3ah], 0
        push   0
        push   dword ptr [esi + 110h]
        call   sub_1_7105
        add    esp, 8
        pop    esi
        ret
sub_1_653B:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        push   esi
        call   sub_1_66A9
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_1_655F
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 115ch
        jne    SHORT loc_1_658D
loc_1_655F:
        push   12h
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_658D
        push   1
        push   1388h
        push   esi
        call   sub_1_7191
        add    esp, 0ch
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_658D
        mov    byte ptr [esi + 0b4h], 0
loc_1_658D:
        push   0
        push   dword ptr [esi + 110h]
        call   sub_1_7105
        add    esp, 8
        mov    eax, edi
        pop    edi
        pop    esi
        ret
sub_1_65A2:
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [esp + 0ch]
        cmp    byte ptr [edi + 11ch], 0
        jne    NEAR PTR loc_1_665B
        mov    al, byte ptr [edi + 3]
        push   eax
        push   dword ptr [edi + 0ech]
        push   dword ptr [edi + 110h]
        call   sub_1_6100
        add    esp, 0ch
        mov    al, byte ptr [edi + 3]
        cmp    al, 8fh
        je     SHORT loc_1_65DE
        cmp    al, 0dh
        je     SHORT loc_1_65DE
        cmp    al, 2dh
        jne    SHORT loc_1_663E
loc_1_65DE:
        cmp    byte ptr [edi + 5], 1
        je     SHORT loc_1_663E
        cmp    byte ptr [edi + 19h], 1
        jne    SHORT loc_1_661E
        push   2eh
        push   edi
        call   sub_1_7237
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_1_663E
        push   0
        push   3
        push   dword ptr [edi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        mov    al, byte ptr [edi + 8]
        add    al, 2
        push   eax
        push   edi
        call   sub_1_7237
        add    esp, 8
        mov    esi, eax
loc_1_661E:
        test   esi, esi
        jne    SHORT loc_1_6649
        mov    al, byte ptr [edi + 19h]
        cmp    al, 2
        je     SHORT loc_1_6631
        cmp    al, 5
        je     SHORT loc_1_6631
        cmp    al, 4
        jne    SHORT loc_1_663E
loc_1_6631:
        push   1
        push   edi
        call   sub_1_73B0
        add    esp, 8
        mov    esi, eax
loc_1_663E:
        test   esi, esi
        jne    SHORT loc_1_6649
        mov    byte ptr [edi + 11ch], 1
loc_1_6649:
        push   0
        push   3
        push   dword ptr [edi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
loc_1_665B:
        mov    eax, esi
        pop    edi
        pop    esi
        ret
sub_1_6660:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        push   3
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_6694
        push   1
        push   3e80h
        push   esi
        call   sub_1_7191
        add    esp, 0ch
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_6694
        mov    byte ptr [esi + 0b4h], 0
loc_1_6694:
        push   0
        push   dword ptr [esi + 110h]
        call   sub_1_7105
        add    esp, 8
        mov    eax, edi
        pop    edi
        pop    esi
        ret
sub_1_66A9:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 2], bl
        mov    byte ptr [ebp - 4], bl
        mov    byte ptr [edi + 116h], bl
        mov    byte ptr [edi + 117h], bl
loc_1_66C9:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 1]
        push   eax
        mov    byte ptr [ebp - 3], bl
        push   8
        push   ecx
        push   6
        push   edi
        call   sub_1_740C
        add    esp, 14h
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_1_670B
        push   esi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 115bh
        jne    SHORT loc_1_6703
        cmp    byte ptr [ebp - 2], bl
        jne    SHORT loc_1_6703
        mov    byte ptr [ebp - 3], 1
        mov    byte ptr [ebp - 2], 1
loc_1_6703:
        test   esi, esi
        jne    NEAR PTR loc_1_68E3
loc_1_670B:
        test   byte ptr [ebp - 1], 1
        jne    SHORT loc_1_6722
        push   1
        push   11022h
        push   115ch
        jmp    NEAR PTR loc_1_68D9
loc_1_6722:
        test   byte ptr [ebp - 1], 4
        je     SHORT loc_1_67A0
        test   byte ptr [ebp - 1], 10h
        jne    SHORT loc_1_6739
        cmp    byte ptr [edi + 19h], 7
        jne    SHORT loc_1_6740
        cmp    byte ptr [edi + 3bh], bl
        je     SHORT loc_1_6740
loc_1_6739:
        mov    byte ptr [edi + 120h], 1
loc_1_6740:
        test   byte ptr [ebp - 1], 40h
        mov    byte ptr [edi + 0b5h], 1
        jne    SHORT loc_1_6753
        mov    byte ptr [edi + 0b5h], bl
loc_1_6753:
        test   byte ptr [ebp - 1], 80h
        mov    byte ptr [edi + 0b6h], 1
        jne    SHORT loc_1_6766
        mov    byte ptr [edi + 0b6h], bl
loc_1_6766:
        test   byte ptr [ebp - 1], 20h
        je     SHORT loc_1_6772
        mov    byte ptr [edi + 3ch], 1
        jmp    SHORT loc_1_6788
loc_1_6772:
        mov    byte ptr [edi + 3ch], bl
        cmp    byte ptr [ebp - 2], bl
        jne    SHORT loc_1_6788
        test   byte ptr [ebp - 1], 2
        jne    SHORT loc_1_6788
        mov    byte ptr [ebp - 3], 1
        mov    byte ptr [ebp - 2], 1
loc_1_6788:
        test   byte ptr [ebp - 1], 8
        mov    byte ptr [edi + 82h], 1
        jne    SHORT loc_1_679B
        mov    byte ptr [edi + 82h], bl
loc_1_679B:
        mov    byte ptr [edi + 3bh], bl
        jmp    SHORT loc_1_67BF
loc_1_67A0:
        mov    byte ptr [edi + 120h], bl
        mov    byte ptr [edi + 3ch], bl
        mov    byte ptr [edi + 82h], bl
        mov    byte ptr [edi + 0b5h], bl
        mov    byte ptr [edi + 3bh], 1
        mov    byte ptr [edi + 0b6h], bl
loc_1_67BF:
        test   byte ptr [ebp - 1], 12h
        je     NEAR PTR loc_1_68E3
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 6]
        push   eax
        push   10h
        push   ecx
        push   7
        push   edi
        call   sub_1_740C
        add    esp, 14h
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_1_6808
        push   esi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 115bh
        jne    SHORT loc_1_6800
        cmp    byte ptr [ebp - 2], bl
        jne    SHORT loc_1_6800
        mov    byte ptr [ebp - 3], 1
        mov    byte ptr [ebp - 2], 1
loc_1_6800:
        test   esi, esi
        jne    NEAR PTR loc_1_68E3
loc_1_6808:
        test   byte ptr [ebp - 1], 2
        je     SHORT loc_1_683B
        mov    ax, word ptr [ebp - 6]
        mov    cl, byte ptr [ebp - 6]
        mov    byte ptr [edi + 117h], cl
        mov    byte ptr [edi + 116h], ah
        cmp    cl, 0dh
        jne    SHORT loc_1_6847
        mov    byte ptr [edi + 116h], bl
        mov    byte ptr [edi + 117h], bl
        mov    byte ptr [edi + 120h], 1
        jmp    SHORT loc_1_6847
loc_1_683B:
        mov    byte ptr [edi + 116h], bl
        mov    byte ptr [edi + 117h], bl
loc_1_6847:
        mov    al, byte ptr [edi + 117h]
        test   al, al
        je     NEAR PTR loc_1_68E3
        movzx  cx, al
        movzx  eax, cx
        cmp    eax, 1
        je     SHORT loc_1_6884
        cmp    eax, 2
        je     SHORT loc_1_68E3
        cmp    eax, 8
        je     SHORT loc_1_6892
        cmp    eax, 0ffh
        je     SHORT loc_1_68A0
        add    cx, 1100h
        push   1
        movzx  eax, byte ptr [edi + 116h]
        push   eax
        push   ecx
        jmp    SHORT loc_1_68D9
loc_1_6884:
        push   2
        push   11022h
        push   115ch
        jmp    SHORT loc_1_68D9
loc_1_6892:
        cmp    byte ptr [ebp - 4], bl
        je     SHORT loc_1_68E3
        mov    byte ptr [ebp - 3], 1
        mov    byte ptr [ebp - 4], bl
        jmp    SHORT loc_1_68E3
loc_1_68A0:
        push   edi
        call   sub_1_6E6A
        add    esp, 4
        mov    byte ptr [edi + 11ch], bl
        push   edi
        call   sub_1_65A2
        add    esp, 4
        mov    esi, eax
        cmp    byte ptr [ebp - 2], bl
        jne    SHORT loc_1_68CD
        test   esi, esi
        jne    SHORT loc_1_68CD
        mov    byte ptr [ebp - 3], 1
        mov    byte ptr [ebp - 2], 1
        jmp    SHORT loc_1_68E3
loc_1_68CD:
        push   1
        push   11022h
        push   1165h
loc_1_68D9:
        call   sub_1_1CB6
        add    esp, 0ch
        mov    esi, eax
loc_1_68E3:
        cmp    byte ptr [ebp - 3], bl
        jne    NEAR PTR loc_1_66C9
        test   esi, esi
        jne    SHORT loc_1_693A
        cmp    byte ptr [edi + 11dh], 0
        mov    al, byte ptr [edi + 120h]
        mov    byte ptr [edi + 3ah], al
        je     SHORT loc_1_693A
        cmp    byte ptr [edi + 3bh], 0
        je     SHORT loc_1_691E
        push   1
        push   11022h
        push   1167h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    esi, eax
loc_1_691E:
        cmp    byte ptr [edi + 3ah], 0
        je     SHORT loc_1_693A
        push   1
        push   11022h
        push   1164h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    esi, eax
loc_1_693A:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_6943:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 128h]
        cmp    dword ptr [ecx + 8], 1
        sbb    al, al
        neg    al
        ret
sub_1_6956:
        sub    esp, 4
        mov    dword ptr [esp], 0
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 14h]
        push   edi
        movzx  eax, word ptr [ebx + 18h]
        push   ebp
        sub    eax, 1100h
        cmp    eax, 14h
        ja     SHORT loc_1_697E
        db  0FFh, 024h, 085h, 09Eh, 07Ch, 040h, 000h
loc_1_697E:
        push   1
        push   11047h
        push   1162h
        call   sub_1_1CB6
        mov    dword ptr [esp + 1ch], eax
        add    esp, 0ch
        jmp    NEAR PTR loc_1_6C92
loc_1_699B:
        mov    ebp, dword ptr [esp + 18h]
        push   ebp
        call   sub_1_6E6A
        add    esp, 4
        push   ebp
        call   sub_1_8A04
        add    esp, 4
        push   ebp
        call   sub_1_8851
        mov    dword ptr [esp + 14h], eax
        add    esp, 4
        push   dword ptr [ebp + 110h]
        call   sub_1_64FF
        add    esp, 4
        jmp    NEAR PTR loc_1_6C86
loc_1_69D1:
        mov    ebp, dword ptr [esp + 18h]
        push   ebx
        push   ebp
        call   sub_1_8664
        mov    dword ptr [esp + 18h], eax
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_6A0A
        push   0
        push   3e8h
        push   dword ptr [ebp + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        push   ebx
        push   ebp
        call   sub_1_8664
        mov    dword ptr [esp + 18h], eax
        add    esp, 8
loc_1_6A0A:
        push   dword ptr [ebp + 110h]
        call   sub_1_64FF
        add    esp, 4
        jmp    NEAR PTR loc_1_6C86
loc_1_6A1D:
        mov    ebp, dword ptr [esp + 18h]
        push   ebp
        call   sub_1_6E6A
        add    esp, 4
        push   ebp
        call   sub_1_65A2
        mov    dword ptr [esp + 14h], eax
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_6AEE
        push   ebp
        call   sub_1_66A9
        mov    dword ptr [esp + 14h], eax
        add    esp, 4
        push   eax
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 115ch
        jne    SHORT loc_1_6A6F
        push   0
        push   493e0h
        push   ebp
        call   sub_1_7191
        mov    dword ptr [esp + 1ch], eax
        add    esp, 0ch
loc_1_6A6F:
        push   dword ptr [esp + 10h]
        call   sub_1_247A
        movzx  eax, ax
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_6AAB
        cmp    eax, 111eh
        je     SHORT loc_1_6AB4
        cmp    eax, 1164h
        je     SHORT loc_1_6AEE
        cmp    eax, 1167h
        je     SHORT loc_1_6AEE
        push   1
        cmp    eax, 15a0h
        push   ebp
        je     SHORT loc_1_6ACE
        call   sub_1_6500
        add    esp, 8
        jmp    SHORT loc_1_6AEE
loc_1_6AAB:
        mov    byte ptr [ebp + 11dh], 1
        jmp    SHORT loc_1_6AEE
loc_1_6AB4:
        push   ebp
        call   sub_1_66A9
        mov    dword ptr [esp + 14h], eax
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_6AEE
        mov    byte ptr [ebp + 11dh], 1
        jmp    SHORT loc_1_6AEE
loc_1_6ACE:
        call   sub_1_6500
        add    esp, 8
        push   1
        push   11047h
        push   1172h
        call   sub_1_1CB6
        mov    dword ptr [esp + 1ch], eax
        add    esp, 0ch
loc_1_6AEE:
        lea    esi, [ebp + 34h]
        lea    edi, [ebx + 20h]
        movsd
        movsd
        movsd
        jmp    NEAR PTR loc_1_6C92
loc_1_6AFC:
        mov    ebp, dword ptr [esp + 18h]
        push   1
        push   ebp
        call   sub_1_6500
        add    esp, 8
        lea    esi, [ebp + 34h]
        lea    edi, [ebx + 20h]
        push   dword ptr [ebp + 110h]
        call   sub_1_64FF
        movsd
        movsd
        movsd
        mov    byte ptr [ebp + 11dh], 0
        add    esp, 4
        jmp    NEAR PTR loc_1_6C92
loc_1_6B2E:
        mov    ebp, dword ptr [esp + 18h]
        push   ebx
        push   ebp
        lea    edi, [ebx + 40h]
        lea    esi, [ebp + 34h]
        call   sub_1_8454
        mov    dword ptr [esp + 18h], eax
        add    esp, 8
        movsd
        movsd
        movsd
        jmp    NEAR PTR loc_1_6C92
loc_1_6B4E:
        mov    ebp, dword ptr [esp + 18h]
        lea    edi, [ebx + 20h]
        push   ebp
        call   sub_1_8374
        mov    dword ptr [esp + 14h], eax
        add    esp, 4
        lea    esi, [ebp + 34h]
        movsd
        movsd
        movsd
        jmp    NEAR PTR loc_1_6C92
loc_1_6B6D:
        push   dword ptr [ebx + 2ch]
        mov    ebp, dword ptr [esp + 1ch]
        lea    edi, [ebx + 20h]
        push   ebp
        call   sub_1_82BE
        mov    dword ptr [esp + 18h], eax
        add    esp, 8
        lea    esi, [ebp + 34h]
        movsd
        movsd
        movsd
        jmp    NEAR PTR loc_1_6C92
loc_1_6B8F:
        mov    ebp, dword ptr [esp + 18h]
        push   ebx
        push   ebp
        call   sub_1_829F
        mov    dword ptr [esp + 18h], eax
        add    esp, 8
        mov    byte ptr [ebp + 11eh], 1
        jmp    NEAR PTR loc_1_6C92
loc_1_6BAD:
        mov    ebp, dword ptr [esp + 18h]
        push   ebx
        push   dword ptr [ebx + 20h]
        push   ebp
        call   sub_1_7E26
        mov    dword ptr [esp + 1ch], eax
        add    esp, 0ch
        jmp    NEAR PTR loc_1_6C92
loc_1_6BC7:
        mov    ebp, dword ptr [esp + 18h]
        push   ebx
        push   ebp
        lea    edi, [ebx + 3ch]
        lea    esi, [ebp + 34h]
        call   sub_1_7A3B
        mov    dword ptr [esp + 18h], eax
        add    esp, 8
        mov    byte ptr [ebp + 3dh], 0
        movsd
        movsd
        movsd
        jmp    NEAR PTR loc_1_6C92
loc_1_6BEB:
        mov    ebp, dword ptr [esp + 18h]
        push   ebx
        push   ebp
        lea    edi, [ebx + 20h]
        lea    esi, [ebp + 70h]
        call   sub_1_7855
        mov    dword ptr [esp + 18h], eax
        add    esp, 8
        mov    ecx, 8
        rep movsd
        jmp    NEAR PTR loc_1_6C92
loc_1_6C0F:
        mov    ebp, dword ptr [esp + 18h]
        push   0
        push   ebp
        lea    edi, [ebx + 20h]
        lea    esi, [ebp + 34h]
        call   sub_1_7766
        mov    dword ptr [esp + 18h], eax
        add    esp, 8
        movsd
        movsd
        movsd
        jmp    SHORT loc_1_6C92
loc_1_6C2D:
        add    ebx, 20h
        mov    ebp, dword ptr [esp + 18h]
        push   ebx
        push   ebp
        call   sub_1_7665
        mov    dword ptr [esp + 18h], eax
        add    esp, 8
        jmp    SHORT loc_1_6C92
loc_1_6C44:
        mov    ebp, dword ptr [esp + 18h]
        push   ebp
        call   sub_1_7496
        mov    dword ptr [esp + 14h], eax
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_6C79
        push   eax
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1167h
        je     SHORT loc_1_6C79
        mov    dword ptr [ebp + 0ch], 0
        mov    byte ptr [ebp + 1dh], 0
        mov    word ptr [ebp + 14h], 0
loc_1_6C79:
        lea    esi, [ebp + 0ch]
        lea    edi, [ebx + 68h]
        mov    ecx, 0ah
        rep movsd
loc_1_6C86:
        lea    esi, [ebp + 34h]
        lea    edi, [ebx + 90h]
        movsd
        movsd
        movsd
loc_1_6C92:
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  09Bh, 079h, 040h, 000h, 0D1h, 079h, 040h, 000h, 01Dh, 07Ah, 040h, 000h, 0FCh, 07Ah, 040h, 000h
        db  02Eh, 07Bh, 040h, 000h, 04Eh, 07Bh, 040h, 000h, 06Dh, 07Bh, 040h, 000h, 08Fh, 07Bh, 040h, 000h
        db  0ADh, 07Bh, 040h, 000h, 0C7h, 07Bh, 040h, 000h, 0C7h, 07Bh, 040h, 000h, 0C7h, 07Bh, 040h, 000h
        db  0C7h, 07Bh, 040h, 000h, 0C7h, 07Bh, 040h, 000h, 0C7h, 07Bh, 040h, 000h, 0EBh, 07Bh, 040h, 000h
        db  00Fh, 07Ch, 040h, 000h, 02Dh, 07Ch, 040h, 000h, 07Eh, 079h, 040h, 000h, 07Eh, 079h, 040h, 000h
        db  044h, 07Ch, 040h, 000h
sub_1_6CF2:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        mov    bl, byte ptr [esp + 18h]
        push   ebp
        mov    al, byte ptr [esi + 41h]
        movzx  edi, bl
        movsx  ecx, al
        cmp    ecx, edi
        jne    SHORT loc_1_6D15
        xor    eax, eax
        jmp    NEAR PTR loc_1_6E62
loc_1_6D15:
        test   al, al
        je     SHORT loc_1_6D21
        cmp    al, 2
        je     SHORT loc_1_6D21
        cmp    al, 1
        jne    SHORT loc_1_6D46
loc_1_6D21:
        push   esi
        call   sub_1_653B
        add    esp, 4
        mov    ebp, eax
        test   ebp, ebp
        je     SHORT loc_1_6D46
        push   ebp
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1167h
        je     SHORT loc_1_6D46
        mov    eax, ebp
        jmp    NEAR PTR loc_1_6E62
loc_1_6D46:
        test   edi, edi
        je     SHORT loc_1_6D77
        cmp    edi, 1
        je     SHORT loc_1_6D7E
        cmp    edi, 2
        je     SHORT loc_1_6D85
        cmp    edi, 3
        je     SHORT loc_1_6D8C
        cmp    edi, 4
        je     SHORT loc_1_6D93
        push   1
        push   11044h
        push   1162h
        call   sub_1_1CB6
        add    esp, 0ch
        jmp    NEAR PTR loc_1_6E62
loc_1_6D77:
        mov    byte ptr [esp + 13h], 1eh
        jmp    SHORT loc_1_6D98
loc_1_6D7E:
        mov    byte ptr [esp + 13h], 0fh
        jmp    SHORT loc_1_6D98
loc_1_6D85:
        mov    byte ptr [esp + 13h], 11h
        jmp    SHORT loc_1_6D98
loc_1_6D8C:
        mov    byte ptr [esp + 13h], 1ch
        jmp    SHORT loc_1_6D98
loc_1_6D93:
        mov    byte ptr [esp + 13h], 1dh
loc_1_6D98:
        push   dword ptr [esp + 13h]
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_6E62
        push   0
        push   3
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        cmp    bl, 3
        je     SHORT loc_1_6DFB
        cmp    bl, 4
        je     SHORT loc_1_6DFB
        push   esi
        call   sub_1_66A9
        add    esp, 4
        mov    edi, eax
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 115ch
        jne    SHORT loc_1_6E20
        push   1
        push   11044h
        push   1170h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    edi, eax
        jmp    SHORT loc_1_6E20
loc_1_6DFB:
        push   dword ptr [esp + 13h]
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_6E60
        push   0
        push   3
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
loc_1_6E20:
        mov    byte ptr [esi + 41h], bl
        test   edi, edi
        jne    SHORT loc_1_6E60
        cmp    bl, 2
        jne    SHORT loc_1_6E60
        cmp    byte ptr [esi + 19h], 1
        jne    SHORT loc_1_6E60
        mov    al, byte ptr [esi + 1ah]
        cmp    al, 4
        je     SHORT loc_1_6E3D
        cmp    al, 5
        jne    SHORT loc_1_6E60
loc_1_6E3D:
        push   0eh
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_6E60
        push   1
        push   0a3930h
        push   esi
        call   sub_1_7191
        add    esp, 0ch
        mov    edi, eax
loc_1_6E60:
        mov    eax, edi
loc_1_6E62:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_6E6A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    byte ptr [ebp - 1], 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    di, 5
        xor    ebx, ebx
        push   dword ptr [esi + 110h]
        mov    byte ptr [esi + 108h], bl
        call   sub_1_8CF8
        add    esp, 4
        cmp    byte ptr [esi + 11bh], bl
        jne    NEAR PTR loc_1_6F4A
        push   dword ptr [esi + 110h]
        mov    byte ptr [esi + 11ch], bl
        call   sub_1_384C
        add    esp, 4
        test   al, al
        jne    SHORT loc_1_6ED0
        push   8
        push   dword ptr [esi + 0f0h]
        push   dword ptr [esi + 110h]
        call   sub_1_6100
        add    esp, 0ch
        jmp    SHORT loc_1_6F16
loc_1_6ED0:
        call   sub_1_55CB
        push   8
        push   dword ptr [esi + 0ech]
        push   dword ptr [esi + 110h]
        call   sub_1_6100
        add    esp, 0ch
        push   0ah
        call   sub_1_0720
        add    esp, 4
        push   0ah
        call   sub_1_0720
        add    esp, 4
        push   1
        push   1ah
        push   dword ptr [esi + 110h]
        call   sub_1_6100
        add    esp, 0ch
        call   sub_1_553D
loc_1_6F16:
        push   0ah
        call   sub_1_0720
        add    esp, 4
        push   0ch
        push   dword ptr [esi + 0ech]
        push   dword ptr [esi + 110h]
        call   sub_1_6100
        add    esp, 0ch
        cmp    byte ptr [esi + 0f4h], 0
        je     NEAR PTR loc_1_700D
        push   0ch
        jmp    NEAR PTR loc_1_6FF9
loc_1_6F4A:
        cmp    byte ptr [esi + 11ch], 1
        jne    SHORT loc_1_6F58
        mov    bl, byte ptr [esi + 3]
        jmp    SHORT loc_1_6F5B
loc_1_6F58:
        mov    bl, byte ptr [esi + 4]
loc_1_6F5B:
        push   dword ptr [esi + 110h]
        call   sub_1_384C
        and    bl, 0fbh
        add    esp, 4
        test   al, al
        jne    SHORT loc_1_6F87
        push   ebx
        push   dword ptr [esi + 0f0h]
        push   dword ptr [esi + 110h]
        call   sub_1_6100
        add    esp, 0ch
        jmp    SHORT loc_1_6FCC
loc_1_6F87:
        call   sub_1_55CB
        push   ebx
        push   dword ptr [esi + 0ech]
        push   dword ptr [esi + 110h]
        call   sub_1_6100
        add    esp, 0ch
        push   0ah
        call   sub_1_0720
        add    esp, 4
        push   0ah
        call   sub_1_0720
        add    esp, 4
        push   1
        push   1ah
        push   dword ptr [esi + 110h]
        call   sub_1_6100
        add    esp, 0ch
        call   sub_1_553D
loc_1_6FCC:
        push   0ah
        or     bl, 0ch
        call   sub_1_0720
        add    esp, 4
        push   ebx
        push   dword ptr [esi + 0f0h]
        push   dword ptr [esi + 110h]
        call   sub_1_6100
        add    esp, 0ch
        cmp    byte ptr [esi + 0f4h], 0
        je     SHORT loc_1_700D
        push   0dh
loc_1_6FF9:
        push   dword ptr [esi + 0f0h]
        push   dword ptr [esi + 110h]
        call   sub_1_6100
        add    esp, 0ch
loc_1_700D:
        push   1
        push   1f4h
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_7039
        mov    di, 4
        push   2
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        call   sub_1_5FD8
        add    esp, 0ch
loc_1_7039:
        push   0
        lea    eax, [ebp - 1]
        push   1
        push   eax
        push   esi
        call   sub_1_5DDF
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_1_709D
        dec    di
        push   2
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        call   sub_1_5FD8
        add    esp, 0ch
        push   0
        push   4
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        mov    al, byte ptr [ebp - 4]
        and    al, 3
        cmp    al, 3
        jae    SHORT loc_1_707F
        test   di, di
        jne    SHORT loc_1_7039
loc_1_707F:
        push   esi
        call   sub_1_8BC4
        add    esp, 4
        xor    eax, eax
        mov    byte ptr [esi + 10bh], al
        mov    byte ptr [esi + 10dh], al
loc_1_7096:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_709D:
        mov    byte ptr [esi + 10bh], 0
        jmp    SHORT loc_1_7096
sub_1_70A6:
        xor    eax, eax
        ret
sub_1_70A9:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    ax, word ptr [esi + 16h]
        add    ax, 11h
        push   eax
        call   sub_1_3281
        add    esp, 4
        mov    bl, al
        and    al, 0fch
        push   eax
        mov    ax, word ptr [esi + 16h]
        add    ax, 11h
        push   eax
        call   sub_1_3276
        mov    ax, word ptr [esi + 16h]
        mov    cl, byte ptr [esp + 18h]
        add    ax, 15h
        add    esp, 8
        push   ecx
        push   eax
        call   sub_1_54A6
        mov    ax, word ptr [esi + 16h]
        or     bl, byte ptr [esp + 1ch]
        add    ax, 11h
        add    esp, 8
        push   ebx
        push   eax
        call   sub_1_3276
        add    esp, 8
        pop    esi
        pop    ebx
        ret
sub_1_7105:
        ret
sub_1_7106:
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [esp + 0ch]
        cmp    byte ptr [edi + 11ch], 1
        jne    SHORT loc_1_718C
        mov    al, byte ptr [edi + 3]
        cmp    al, 8fh
        je     SHORT loc_1_7126
        cmp    al, 0dh
        je     SHORT loc_1_7126
        cmp    al, 2dh
        jne    SHORT loc_1_715B
loc_1_7126:
        cmp    byte ptr [edi + 5], 1
        je     SHORT loc_1_715B
        cmp    byte ptr [edi + 19h], 1
        jne    SHORT loc_1_713F
        push   2fh
        push   edi
        call   sub_1_7237
        add    esp, 8
        mov    esi, eax
loc_1_713F:
        mov    al, byte ptr [edi + 19h]
        cmp    al, 2
        je     SHORT loc_1_714E
        cmp    al, 5
        je     SHORT loc_1_714E
        cmp    al, 4
        jne    SHORT loc_1_715B
loc_1_714E:
        push   0
        push   edi
        call   sub_1_73B0
        add    esp, 8
        mov    esi, eax
loc_1_715B:
        mov    al, byte ptr [edi + 4]
        push   eax
        push   dword ptr [edi + 0ech]
        push   dword ptr [edi + 110h]
        call   sub_1_6100
        add    esp, 0ch
        mov    byte ptr [edi + 11ch], 0
        push   0
        push   3
        push   dword ptr [edi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
loc_1_718C:
        mov    eax, esi
        pop    edi
        pop    esi
        ret
sub_1_7191:
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_0F80
        mov    esi, eax
        mov    edi, dword ptr [esp + 14h]
        mov    bl, byte ptr [esp + 1ch]
loc_1_71A4:
        push   0
        push   64h
        push   dword ptr [edi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        push   edi
        call   sub_1_66A9
        add    esp, 4
        mov    ebp, eax
        push   ebp
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 115ch
        jne    SHORT loc_1_7213
        test   bl, bl
        jne    SHORT loc_1_71E6
        push   dword ptr [edi + 110h]
        call   sub_1_60D4
        add    esp, 4
        cmp    al, 0ffh
        jne    SHORT loc_1_7217
loc_1_71E6:
        call   sub_1_0F80
        sub    eax, esi
        cmp    eax, dword ptr [esp + 18h]
        jb     SHORT loc_1_71A4
        push   dword ptr [edi + 110h]
        call   sub_1_60D4
        add    esp, 4
        cmp    al, 0ffh
        push   1
        push   1104ah
        je     SHORT loc_1_7225
        push   1150h
        jmp    SHORT loc_1_722A
loc_1_7213:
        mov    eax, ebp
        jmp    SHORT loc_1_7232
loc_1_7217:
        push   1
        push   1104ah
        push   1150h
        jmp    SHORT loc_1_722A
loc_1_7225:
        push   15a0h
loc_1_722A:
        call   sub_1_1CB6
        add    esp, 0ch
loc_1_7232:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_7237:
        push   ebp
        mov    al, byte ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 8
        cmp    al, 80h
        push   esi
        push   edi
        jb     SHORT loc_1_7260
        push   1
        push   11041h
        push   1162h
        call   sub_1_1CB6
        add    esp, 0ch
        jmp    NEAR PTR loc_1_73AA
loc_1_7260:
        mov    esi, dword ptr [ebp + 8]
        mov    cl, byte ptr [esi + 10bh]
        cmp    cl, 80h
        jae    SHORT loc_1_7275
        add    al, cl
        mov    byte ptr [ebp - 6], al
        jmp    SHORT loc_1_727A
loc_1_7275:
        sub    cl, al
        mov    byte ptr [ebp - 6], cl
loc_1_727A:
        push   dword ptr [esi + 110h]
        mov    byte ptr [ebp - 8], 0fh
        mov    al, byte ptr [esi + 5]
        mov    byte ptr [ebp - 7], al
        call   sub_1_8CF8
        add    esp, 4
        lea    ecx, [ebp - 8]
        push   0
        push   3
        push   ecx
        push   esi
        call   sub_1_5DDF
        add    esp, 10h
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_1_72BC
        push   dword ptr [esi + 110h]
        call   sub_1_8D0B
        add    esp, 4
        jmp    NEAR PTR loc_1_739F
loc_1_72BC:
        push   1
        push   3e8h
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        mov    edi, eax
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 15a0h
        jne    SHORT loc_1_7307
        push   0
        lea    eax, [ebp - 1]
        mov    byte ptr [ebp - 1], 8
        push   1
        push   eax
        push   esi
        call   sub_1_5DDF
        add    esp, 10h
        mov    edi, eax
        push   dword ptr [esi + 110h]
        call   sub_1_505D
        add    esp, 4
loc_1_7307:
        test   edi, edi
        jne    NEAR PTR loc_1_739F
        push   2
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        call   sub_1_5FD8
        add    esp, 0ch
        mov    edi, eax
        test   edi, edi
        jne    NEAR PTR loc_1_73A8
        test   byte ptr [ebp - 4], 0c0h
        jne    SHORT loc_1_7389
        push   dword ptr [esi + 110h]
        call   sub_1_60CE
        add    esp, 4
        cmp    al, 1
        je     SHORT loc_1_7360
        mov    al, byte ptr [esi + 5]
        or     al, 20h
        cmp    al, byte ptr [ebp - 4]
        je     SHORT loc_1_7360
        push   1
        push   11041h
        push   115dh
        call   sub_1_1CB6
        add    esp, 0ch
        jmp    SHORT loc_1_73AA
loc_1_7360:
        mov    al, byte ptr [ebp - 6]
        cmp    byte ptr [ebp - 3], al
        je     SHORT loc_1_737E
        push   1
        push   11041h
        push   1157h
        call   sub_1_1CB6
        add    esp, 0ch
        jmp    SHORT loc_1_73AA
loc_1_737E:
        mov    al, byte ptr [ebp - 3]
        mov    byte ptr [esi + 10bh], al
        jmp    SHORT loc_1_73A8
loc_1_7389:
        push   2
        push   11041h
        push   115dh
        call   sub_1_1CB6
        add    esp, 0ch
        jmp    SHORT loc_1_73AA
loc_1_739F:
        push   esi
        call   sub_1_6E6A
        add    esp, 4
loc_1_73A8:
        mov    eax, edi
loc_1_73AA:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_73B0:
        cmp    byte ptr [esp + 8], 0
        push   esi
        push   edi
        je     SHORT loc_1_73E4
        mov    esi, dword ptr [esp + 0ch]
        push   17h
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_7407
        push   0
        push   3
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        push   14h
        jmp    SHORT loc_1_73EA
loc_1_73E4:
        mov    esi, dword ptr [esp + 0ch]
        push   18h
loc_1_73EA:
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    edi, eax
        push   0
        push   3
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
loc_1_7407:
        mov    eax, edi
        pop    edi
        pop    esi
        ret
sub_1_740C:
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 0ch]
        push   edi
        xor    di, di
        push   ebp
        mov    esi, dword ptr [esp + 1ch]
loc_1_741B:
        cmp    byte ptr [ebx + 10fh], 0
        je     SHORT loc_1_742F
        mov    byte ptr [ebx + 10fh], 0
        xor    ebp, ebp
        jmp    SHORT loc_1_743E
loc_1_742F:
        push   dword ptr [esp + 18h]
        push   ebx
        call   sub_1_7237
        add    esp, 8
        mov    ebp, eax
loc_1_743E:
        test   ebp, ebp
        jne    SHORT loc_1_7483
        push   esi
        push   dword ptr [esp + 24h]
        push   ebx
        call   sub_1_8D17
        add    esp, 0ch
        mov    ebp, eax
        push   ebp
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1158h
        jne    SHORT loc_1_7483
        cmp    dword ptr [esp + 24h], 0
        je     SHORT loc_1_7483
        mov    eax, dword ptr [esp + 24h]
        push   ebx
        mov    byte ptr [eax], 1
        call   sub_1_6E6A
        add    esp, 4
        push   ebx
        call   sub_1_65A2
        add    esp, 4
        mov    ebp, eax
loc_1_7483:
        inc    di
        cmp    di, 6
        jae    SHORT loc_1_748F
        test   ebp, ebp
        jne    SHORT loc_1_741B
loc_1_748F:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_7496:
        sub    esp, 8
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 14h]
        push   edi
        push   ebp
        xor    edi, edi
        cmp    dword ptr [esi + 0ch], edi
        jne    NEAR PTR loc_1_7649
        cmp    byte ptr [esi + 19h], 1
        jne    NEAR PTR loc_1_764F
        cmp    word ptr [esi + 16h], 50h
        jb     NEAR PTR loc_1_7649
        push   3
        push   esi
        call   sub_1_6CF2
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_74DF
        push   35h
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    edi, eax
loc_1_74DF:
        push   0
        push   3
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        test   edi, edi
        jne    SHORT loc_1_7502
        push   5
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    edi, eax
loc_1_7502:
        push   0
        push   3
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        test   edi, edi
        jne    SHORT loc_1_752E
        lea    eax, [esp + 11h]
        push   0
        push   8
        push   eax
        push   2ch
        push   esi
        call   sub_1_740C
        add    esp, 14h
        mov    edi, eax
loc_1_752E:
        cmp    word ptr [esi + 16h], 6eh
        jb     SHORT loc_1_7569
        test   edi, edi
        jne    SHORT loc_1_7569
        lea    eax, [esp + 11h]
        push   0
        push   8
        push   eax
        push   2ch
        push   esi
        call   sub_1_740C
        add    esp, 14h
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_7569
        lea    eax, [esp + 11h]
        push   0
        push   8
        push   eax
        push   2ch
        push   esi
        call   sub_1_740C
        add    esp, 14h
        mov    edi, eax
loc_1_7569:
        mov    bx, 4
loc_1_756D:
        test   edi, edi
        jne    SHORT loc_1_758E
        movzx  eax, bx
        push   0
        push   8
        lea    ecx, [esp + eax + 1bh]
        push   ecx
        push   2ch
        push   esi
        call   sub_1_740C
        add    esp, 14h
        mov    edi, eax
        dec    bx
        jne    SHORT loc_1_756D
loc_1_758E:
        xor    bx, bx
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esi + 0ch], eax
loc_1_7598:
        test   edi, edi
        jne    SHORT loc_1_75BD
        movzx  eax, bx
        inc    bx
        push   0
        lea    ecx, [esp + eax + 16h]
        push   8
        push   ecx
        push   2ch
        push   esi
        call   sub_1_740C
        add    esp, 14h
        mov    edi, eax
        cmp    bx, 2
        jb     SHORT loc_1_7598
loc_1_75BD:
        movzx  ax, byte ptr [esp + 12h]
        xor    bx, bx
        movzx  cx, byte ptr [esp + 13h]
        mov    word ptr [esi + 14h], ax
        shl    ax, 8
        or     cx, ax
        mov    word ptr [esi + 14h], ax
        mov    word ptr [esi + 14h], cx
loc_1_75DF:
        test   edi, edi
        jne    SHORT loc_1_760A
        movzx  eax, bx
        push   0
        push   8
        lea    ebp, [eax + esi + 1dh]
        push   ebp
        push   2ch
        push   esi
        call   sub_1_740C
        add    esp, 14h
        mov    edi, eax
        cmp    byte ptr [ebp], 0
        je     SHORT loc_1_760A
        inc    bx
        cmp    bx, 14h
        jb     SHORT loc_1_75DF
loc_1_760A:
        cmp    word ptr [esi + 16h], 6eh
        jb     SHORT loc_1_7636
        xor    bx, bx
loc_1_7614:
        test   edi, edi
        jne    SHORT loc_1_765B
        inc    bx
        push   0
        lea    eax, [esp + 15h]
        push   8
        push   eax
        push   2ch
        push   esi
        call   sub_1_740C
        add    esp, 14h
        mov    edi, eax
        cmp    bx, 2
        jb     SHORT loc_1_7614
loc_1_7636:
        test   edi, edi
        jne    SHORT loc_1_765B
        push   0
        push   esi
        call   sub_1_6CF2
        add    esp, 8
        mov    edi, eax
        jmp    SHORT loc_1_765B
loc_1_7649:
        cmp    byte ptr [esi + 19h], 1
        je     SHORT loc_1_765B
loc_1_764F:
        xor    eax, eax
        mov    word ptr [esi + 14h], ax
        mov    dword ptr [esi + 0ch], eax
        mov    byte ptr [esi + 1dh], al
loc_1_765B:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_7665:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 2], bl
        mov    al, byte ptr [esi]
        inc    esi
        mov    byte ptr [ebp - 1], al
        cmp    al, bl
        je     SHORT loc_1_76B1
loc_1_7683:
        push   0
        push   3
        push   dword ptr [edi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        mov    al, byte ptr [esi]
        inc    esi
        push   eax
        push   edi
        call   sub_1_7237
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    NEAR PTR loc_1_775D
        dec    byte ptr [ebp - 1]
        jne    SHORT loc_1_7683
loc_1_76B1:
        mov    al, byte ptr [esi]
        inc    esi
        movzx  ecx, al
        cmp    ecx, 0fdh
        je     SHORT loc_1_76D4
        cmp    ecx, 0feh
        je     SHORT loc_1_76E3
        cmp    ecx, 0ffh
        je     SHORT loc_1_76FC
        mov    byte ptr [ebp - 2], al
        jmp    SHORT loc_1_770E
loc_1_76D4:
        mov    byte ptr [edi + 11eh], 1
        mov    al, byte ptr [esi]
        inc    esi
        mov    byte ptr [ebp - 2], al
        jmp    SHORT loc_1_770E
loc_1_76E3:
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     SHORT loc_1_770E
        push   0
        push   eax
        push   dword ptr [edi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        jmp    SHORT loc_1_770E
loc_1_76FC:
        push   1
        push   13d620h
        push   edi
        call   sub_1_7191
        add    esp, 0ch
        mov    ebx, eax
loc_1_770E:
        cmp    byte ptr [ebp - 2], 0
        je     SHORT loc_1_775D
        push   0
        push   3
        push   dword ptr [edi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        mov    al, byte ptr [ebp - 2]
        dec    al
        lea    ecx, [ebp - 4]
        push   ecx
        cmp    al, 1
        sbb    eax, eax
        and    al, 0f8h
        add    ax, 10h
        push   eax
        push   edi
        call   sub_1_8D17
        add    esp, 0ch
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_775D
        cmp    byte ptr [ebp - 2], 1
        jne    SHORT loc_1_7756
        mov    al, byte ptr [ebp - 4]
        mov    byte ptr [esi], al
        jmp    SHORT loc_1_775D
loc_1_7756:
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [esi], ax
loc_1_775D:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_7766:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        push   0ch
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_7852
        cmp    byte ptr [esi + 63h], 1
        je     SHORT loc_1_778A
        mov    eax, dword ptr [esi + 6ch]
        jmp    SHORT loc_1_779D
loc_1_778A:
        mov    al, byte ptr [esi + 1ah]
        sub    al, 2
        cmp    al, 1
        sbb    eax, eax
        and    eax, 88b8h
        add    eax, 15f90h
loc_1_779D:
        push   0
        push   eax
        push   esi
        call   sub_1_7191
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_7852
        push   0bh
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_7852
        cmp    byte ptr [esi + 63h], 1
        push   0
        je     SHORT loc_1_77DE
        push   dword ptr [esi + 6ch]
        push   esi
        call   sub_1_7191
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_7849
        jmp    SHORT loc_1_7852
loc_1_77DE:
        push   0d6d8h
        push   esi
        call   sub_1_7191
        add    esp, 0ch
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_1_782F
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 15a0h
        jne    SHORT loc_1_782B
        mov    al, byte ptr [esi + 1ah]
        push   0
        sub    al, 2
        cmp    al, 1
        sbb    eax, eax
        and    eax, 88b8h
        add    eax, 15f90h
        sub    eax, 0d6d8h
        push   eax
        push   esi
        call   sub_1_7191
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_7849
        jmp    SHORT loc_1_7852
loc_1_782B:
        mov    eax, edi
        jmp    SHORT loc_1_7852
loc_1_782F:
        mov    eax, dword ptr [esp + 10h]
        test   eax, eax
        je     SHORT loc_1_7849
        cmp    byte ptr [esi + 1ah], 2
        mov    dword ptr [eax], 44h
        je     SHORT loc_1_7849
        mov    dword ptr [eax], 64h
loc_1_7849:
        mov    dword ptr [esi + 34h], 0
        xor    eax, eax
loc_1_7852:
        pop    edi
        pop    esi
        ret
sub_1_7855:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [esp + 1ch]
        push   ebp
        mov    ebp, dword ptr [esp + 1ch]
        push   esi
        mov    dword ptr [esp + 18h], ebx
        push   ebp
        call   sub_1_921A
        add    esp, 8
        mov    edi, eax
        cmp    edi, ebx
        jne    NEAR PTR loc_1_7A15
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [ebp + 0cch], eax
        mov    dword ptr [ebp + 0c4h], ebx
        mov    ecx, dword ptr [esi + 14h]
        mov    dword ptr [ebp + 0c8h], 800h
        mov    dword ptr [ebp + 0bch], ecx
        mov    eax, dword ptr [esi + 14h]
        add    eax, 800h
        mov    dword ptr [ebp + 0c0h], eax
        mov    bx, word ptr [esi + 40h]
        mov    ax, word ptr [esi + 42h]
        mov    byte ptr [ebp + 0f8h], 4dh
        add    ax, bx
        mov    cl, byte ptr [ebp + 5]
        mov    word ptr [esp + 12h], ax
        mov    byte ptr [ebp + 0f9h], cl
        mov    byte ptr [ebp + 0fah], 3
        mov    byte ptr [ebp + 0fbh], 20h
        mov    byte ptr [ebp + 0fdh], 6bh
        movzx  eax, byte ptr [ebp + 1ah]
        mov    word ptr [ebp + 38h], 0ffffh
        cmp    eax, 4
        je     SHORT loc_1_7900
        cmp    eax, 5
        je     SHORT loc_1_7909
        mov    byte ptr [ebp + 0fch], 0e9h
        jmp    SHORT loc_1_7910
loc_1_7900:
        mov    byte ptr [ebp + 0fch], 0f1h
        jmp    SHORT loc_1_7910
loc_1_7909:
        mov    byte ptr [ebp + 0fch], 0f8h
loc_1_7910:
        push   1
        push   dword ptr [ebp + 110h]
        call   sub_1_9217
        add    esp, 8
        test   al, al
        je     SHORT loc_1_792D
        push   ebp
        call   sub_1_6E6A
        add    esp, 4
loc_1_792D:
        test   edi, edi
        jne    NEAR PTR loc_1_79CE
loc_1_7935:
        push   1
        push   ebp
        call   sub_1_8E15
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_7952
        push   ebx
        push   ebp
        call   sub_1_8ED0
        add    esp, 8
        mov    edi, eax
loc_1_7952:
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1154h
        je     SHORT loc_1_7970
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 115fh
        jne    SHORT loc_1_79B9
loc_1_7970:
        push   0
        push   ebp
        call   sub_1_6CF2
        add    esp, 8
        push   ebx
        push   ebp
        call   sub_1_9069
        add    esp, 8
        push   1
        push   ebp
        call   sub_1_6CF2
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_79B9
        push   1
        push   ebp
        call   sub_1_8E15
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_79B9
        mov    eax, dword ptr [esi + 14h]
        push   ebx
        mov    word ptr [eax], bx
        push   ebp
        call   sub_1_8ED0
        add    esp, 8
        mov    edi, eax
loc_1_79B9:
        inc    bx
        cmp    word ptr [esp + 12h], bx
        jbe    SHORT loc_1_79CA
        test   edi, edi
        je     NEAR PTR loc_1_7935
loc_1_79CA:
        test   edi, edi
        je     SHORT loc_1_79DD
loc_1_79CE:
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1150h
        jne    SHORT loc_1_7A15
loc_1_79DD:
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1150h
        jne    SHORT loc_1_79F0
        mov    dword ptr [esp + 14h], edi
loc_1_79F0:
        push   ebp
        call   sub_1_6E6A
        add    esp, 4
        push   0
        push   ebp
        call   sub_1_6CF2
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_7A26
        push   ebp
        call   sub_1_8374
        add    esp, 4
        mov    edi, eax
loc_1_7A15:
        test   edi, edi
        jne    SHORT loc_1_7A26
        push   0
        push   ebp
        call   sub_1_8E15
        add    esp, 8
        mov    edi, eax
loc_1_7A26:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_1_7A31
        mov    edi, dword ptr [esp + 14h]
loc_1_7A31:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_7A3B:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 18h]
        mov    dword ptr [ebp - 8], ebx
        push   eax
        mov    dword ptr [ebp - 0ch], ebx
        push   edi
        call   sub_1_9AAF
        mov    word ptr [esi + 34h], bx
        add    esp, 8
        mov    word ptr [esi + 36h], bx
        mov    dword ptr [esi + 2ch], ebx
        mov    word ptr [esi + 38h], bx
        mov    dword ptr [esi + 30h], ebx
        mov    ax, word ptr [edi + 0a6h]
        mov    dword ptr [edi + 90h], ebx
        mov    word ptr [edi + 0a4h], ax
        mov    dword ptr [edi + 94h], ebx
        mov    byte ptr [edi + 0b2h], 1
        mov    byte ptr [edi + 0b1h], bl
        push   dword ptr [esi + 24h]
        push   dword ptr [esi + 20h]
        push   edi
        call   sub_1_99DF
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_7D7D
        cmp    word ptr [esi + 18h], 110ch
        jne    SHORT loc_1_7ACA
        push   esi
        push   edi
        call   sub_1_991C
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_7D7D
loc_1_7ACA:
        mov    eax, dword ptr [esi + 28h]
        xor    cl, cl
        mov    word ptr [edi + 9eh], 0
        mov    dword ptr [edi + 98h], eax
        cmp    dword ptr [esi + 24h], 0
        jbe    SHORT loc_1_7B07
loc_1_7AE4:
        mov    eax, dword ptr [edi + 98h]
        test   al, 1
        jne    SHORT loc_1_7AF5
        inc    word ptr [edi + 9eh]
loc_1_7AF5:
        shr    eax, 1
        inc    cl
        movzx  edx, cl
        mov    dword ptr [edi + 98h], eax
        cmp    edx, dword ptr [esi + 24h]
        jb     SHORT loc_1_7AE4
loc_1_7B07:
        mov    eax, dword ptr [esi + 28h]
        mov    word ptr [edi + 9ch], 0
        mov    dword ptr [edi + 98h], eax
        mov    ax, word ptr [esi + 18h]
        cmp    ax, 110dh
        je     SHORT loc_1_7B3A
        cmp    ax, 110eh
        je     SHORT loc_1_7B3A
        mov    byte ptr [ebp - 1], 0
        cmp    byte ptr [edi + 8ch], 4
        jne    SHORT loc_1_7B52
        push   0
        jmp    SHORT loc_1_7B49
loc_1_7B3A:
        mov    byte ptr [ebp - 1], 1
        cmp    byte ptr [edi + 8ch], 4
        jne    SHORT loc_1_7B52
        push   1
loc_1_7B49:
        push   edi
        call   sub_1_8E15
        add    esp, 8
loc_1_7B52:
        push   dword ptr [ebp - 1]
        push   dword ptr [edi + 110h]
        call   sub_1_9217
        add    esp, 8
        test   al, al
        je     SHORT loc_1_7B70
        push   edi
        call   sub_1_6E6A
        add    esp, 4
loc_1_7B70:
        cmp    word ptr [edi + 9eh], 0
        jne    SHORT loc_1_7B88
        cmp    word ptr [edi + 9ch], 0
        je     NEAR PTR loc_1_7D84
loc_1_7B88:
        cmp    word ptr [edi + 9ch], 0
        jne    SHORT loc_1_7B9B
        push   edi
        call   sub_1_98BF
        add    esp, 4
loc_1_7B9B:
        mov    ax, word ptr [edi + 38h]
        cmp    word ptr [edi + 0ach], ax
        jne    SHORT loc_1_7BC7
        mov    eax, dword ptr [edi + 0a8h]
        mov    ecx, dword ptr [edi + 34h]
        cmp    eax, ecx
        jb     SHORT loc_1_7BC7
        je     SHORT loc_1_7BD8
        cmp    byte ptr [edi + 0b4h], 1
        jne    SHORT loc_1_7BC7
        sub    eax, ecx
        cmp    eax, 1
        je     SHORT loc_1_7BD8
loc_1_7BC7:
        push   edi
        call   sub_1_96A9
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_7D7D
loc_1_7BD8:
        movzx  eax, word ptr [edi + 9ch]
        shl    eax, 0ah
        mov    dword ptr [edi + 94h], eax
        lea    eax, [edi + 94h]
        push   eax
        push   dword ptr [edi + 90h]
        push   dword ptr [esi + 10h]
        push   dword ptr [ebp - 1]
        push   dword ptr [edi + 110h]
        call   sub_1_5F59
        movzx  eax, word ptr [esi + 18h]
        add    esp, 14h
        cmp    eax, 110dh
        je     SHORT loc_1_7C21
        cmp    eax, 110eh
        je     SHORT loc_1_7C27
        mov    byte ptr [ebp - 18h], 46h
        jmp    SHORT loc_1_7C2B
loc_1_7C21:
        mov    byte ptr [ebp - 18h], 45h
        jmp    SHORT loc_1_7C2B
loc_1_7C27:
        mov    byte ptr [ebp - 18h], 49h
loc_1_7C2B:
        mov    al, byte ptr [edi + 5]
        mov    cl, byte ptr [edi + 0aeh]
        mov    dl, byte ptr [edi + 0b0h]
        push   edi
        mov    byte ptr [ebp - 17h], al
        mov    byte ptr [ebp - 16h], cl
        mov    al, byte ptr [edi + 0afh]
        mov    cl, byte ptr [edi + 4ch]
        mov    byte ptr [ebp - 15h], dl
        mov    byte ptr [ebp - 14h], al
        mov    dl, byte ptr [edi + 4eh]
        mov    byte ptr [ebp - 12h], cl
        mov    byte ptr [ebp - 13h], 3
        mov    byte ptr [ebp - 10h], 0ffh
        mov    byte ptr [ebp - 11h], dl
        call   sub_1_9682
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    NEAR PTR loc_1_7D9B
        push   dword ptr [edi + 110h]
        call   sub_1_8CF8
        add    esp, 4
        lea    eax, [ebp - 18h]
        push   1
        push   9
        push   eax
        push   edi
        call   sub_1_5DDF
        add    esp, 10h
        mov    ebx, eax
        test   ebx, ebx
        jne    NEAR PTR loc_1_7DBF
        push   1
        push   1388h
        push   dword ptr [edi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        mov    ebx, eax
        push   dword ptr [edi + 94h]
        push   dword ptr [edi + 90h]
        push   dword ptr [esi + 10h]
        push   dword ptr [ebp - 1]
        push   dword ptr [edi + 110h]
        call   sub_1_5F42
        add    esp, 14h
        push   ebx
        call   sub_1_247A
        movzx  eax, ax
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_7CEC
        cmp    eax, 15a0h
        je     SHORT loc_1_7D27
        jmp    SHORT loc_1_7D47
loc_1_7CEC:
        lea    eax, [ebp - 8]
        push   eax
        push   esi
        push   edi
        call   sub_1_94BB
        add    esp, 0ch
        mov    ebx, eax
        test   ebx, ebx
        jne    NEAR PTR loc_1_7E0F
        push   dword ptr [ebp - 8]
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1155h
        je     NEAR PTR loc_1_7E16
        cmp    word ptr [edi + 0a0h], 0
        jne    SHORT loc_1_7D47
        mov    eax, ebx
        jmp    SHORT loc_1_7D7D
loc_1_7D27:
        lea    eax, [ebp - 8]
        push   eax
        push   esi
        push   edi
        call   sub_1_9408
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_7D7D
        cmp    word ptr [edi + 0a0h], 0
        je     NEAR PTR loc_1_7E1E
loc_1_7D47:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_7D53
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
loc_1_7D53:
        push   dword ptr [edi + 110h]
        call   sub_1_60D4
        add    esp, 4
        cmp    al, 0ffh
        je     NEAR PTR loc_1_7B70
        push   1
        push   11038h
        push   1150h
        call   sub_1_1CB6
        add    esp, 0ch
loc_1_7D7D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_7D84:
        mov    ax, word ptr [esi + 18h]
        push   eax
        push   edi
        call   sub_1_9373
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_7D7D
        mov    eax, dword ptr [ebp - 0ch]
        jmp    SHORT loc_1_7D7D
loc_1_7D9B:
        push   dword ptr [edi + 94h]
        push   dword ptr [edi + 90h]
        push   dword ptr [esi + 10h]
        push   dword ptr [ebp - 1]
        push   dword ptr [edi + 110h]
        call   sub_1_5F42
        add    esp, 14h
        mov    eax, ebx
        jmp    SHORT loc_1_7D7D
loc_1_7DBF:
        push   dword ptr [edi + 110h]
        call   sub_1_8D0B
        add    esp, 4
        push   edi
        call   sub_1_6E6A
        add    esp, 4
        push   dword ptr [edi + 94h]
        push   dword ptr [edi + 90h]
        push   dword ptr [esi + 10h]
        push   dword ptr [ebp - 1]
        push   dword ptr [edi + 110h]
        call   sub_1_5F42
        add    esp, 14h
        push   edi
        call   sub_1_6E6A
        add    esp, 4
        push   edi
        call   sub_1_6660
        add    esp, 4
        mov    eax, ebx
        jmp    NEAR PTR loc_1_7D7D
loc_1_7E0F:
        mov    eax, ebx
        jmp    NEAR PTR loc_1_7D7D
loc_1_7E16:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_7D7D
loc_1_7E1E:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_7D7D
sub_1_7E26:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    ecx, dword ptr [ebp + 0ch]
        push   esi
        xor    ebx, ebx
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [ebp + 8]
        movzx  eax, byte ptr [edi + 62h]
        mov    dword ptr [edi + 7ch], ecx
        cmp    eax, 1
        jl     NEAR PTR loc_1_81C1
        cmp    eax, 2
        jle    SHORT loc_1_7E65
        cmp    eax, 3
        jl     NEAR PTR loc_1_81C1
        cmp    eax, 4
        jle    NEAR PTR loc_1_80CF
        jmp    NEAR PTR loc_1_81C1
loc_1_7E65:
        cmp    ecx, 64h
        ja     SHORT loc_1_7E77
        je     NEAR PTR loc_1_7F16
        cmp    ecx, 44h
        je     SHORT loc_1_7ED7
        jmp    SHORT loc_1_7EBC
loc_1_7E77:
        cmp    ecx, 66h
        je     NEAR PTR loc_1_7F55
        cmp    ecx, 8dh
        je     NEAR PTR loc_1_7F94
        cmp    ecx, 96h
        je     NEAR PTR loc_1_7FD3
        cmp    ecx, 0cfh
        je     NEAR PTR loc_1_8012
        cmp    ecx, 16dh
        je     NEAR PTR loc_1_8051
        cmp    ecx, 219h
        je     NEAR PTR loc_1_8090
loc_1_7EBC:
        push   4
        push   11013h
        push   116ch
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebx, eax
        jmp    NEAR PTR loc_1_81C1
loc_1_7ED7:
        mov    byte ptr [edi + 61h], 1
        mov    dword ptr [edi + 54h], 5500h
        mov    word ptr [edi + 80h], 14h
        mov    dword ptr [edi + 64h], 1fbd0h
        mov    word ptr [edi + 50h], 0aah
        mov    eax, ecx
        imul   eax, eax, 280h
        shl    ecx, 5
        mov    dword ptr [edi + 58h], eax
        mov    eax, 0fde8h
        mov    dword ptr [edi + 5ch], ecx
        mov    dword ptr [edi + 68h], eax
        jmp    NEAR PTR loc_1_81BE
loc_1_7F16:
        mov    byte ptr [edi + 61h], 4
        mov    dword ptr [edi + 54h], 4b00h
        mov    word ptr [edi + 80h], 1ch
        mov    dword ptr [edi + 64h], 186a0h
        mov    word ptr [edi + 50h], 96h
        mov    eax, ecx
        imul   eax, eax, 380h
        shl    ecx, 5
        mov    dword ptr [edi + 58h], eax
        mov    eax, 0c350h
        mov    dword ptr [edi + 5ch], ecx
        mov    dword ptr [edi + 68h], eax
        jmp    NEAR PTR loc_1_81BE
loc_1_7F55:
        mov    byte ptr [edi + 61h], 2
        mov    dword ptr [edi + 54h], 7f80h
        mov    word ptr [edi + 80h], 14h
        mov    dword ptr [edi + 64h], 30d40h
        mov    word ptr [edi + 50h], 0ffh
        mov    eax, ecx
        imul   eax, eax, 280h
        shl    ecx, 5
        mov    dword ptr [edi + 58h], eax
        mov    eax, 186a0h
        mov    dword ptr [edi + 5ch], ecx
        mov    dword ptr [edi + 68h], eax
        jmp    NEAR PTR loc_1_81BE
loc_1_7F94:
        mov    byte ptr [edi + 61h], 0ch
        mov    dword ptr [edi + 54h], 5500h
        mov    word ptr [edi + 80h], 14h
        mov    dword ptr [edi + 64h], 3d090h
        mov    word ptr [edi + 50h], 0aah
        mov    eax, ecx
        imul   eax, eax, 280h
        shl    ecx, 5
        mov    dword ptr [edi + 58h], eax
        mov    eax, 1e848h
        mov    dword ptr [edi + 5ch], ecx
        mov    dword ptr [edi + 68h], eax
        jmp    NEAR PTR loc_1_81BE
loc_1_7FD3:
        mov    byte ptr [edi + 61h], 5
        mov    dword ptr [edi + 54h], 4b00h
        mov    word ptr [edi + 80h], 1ch
        mov    dword ptr [edi + 64h], 1fbd0h
        mov    word ptr [edi + 50h], 96h
        mov    eax, ecx
        imul   eax, eax, 380h
        shl    ecx, 5
        mov    dword ptr [edi + 58h], eax
        mov    eax, 0fde8h
        mov    dword ptr [edi + 5ch], ecx
        mov    dword ptr [edi + 68h], eax
        jmp    NEAR PTR loc_1_81BE
loc_1_8012:
        mov    byte ptr [edi + 61h], 0dh
        mov    dword ptr [edi + 54h], 4b00h
        mov    word ptr [edi + 80h], 1ch
        mov    dword ptr [edi + 64h], 2bf20h
        mov    word ptr [edi + 50h], 96h
        mov    eax, ecx
        imul   eax, eax, 380h
        shl    ecx, 5
        mov    dword ptr [edi + 58h], eax
        mov    eax, 15f90h
        mov    dword ptr [edi + 5ch], ecx
        mov    dword ptr [edi + 68h], eax
        jmp    NEAR PTR loc_1_81BE
loc_1_8051:
        mov    byte ptr [edi + 61h], 3
        mov    dword ptr [edi + 54h], 7f00h
        mov    word ptr [edi + 80h], 14h
        mov    dword ptr [edi + 64h], 0aae60h
        mov    word ptr [edi + 50h], 0feh
        mov    eax, ecx
        imul   eax, eax, 280h
        shl    ecx, 5
        mov    dword ptr [edi + 58h], eax
        mov    eax, 55730h
        mov    dword ptr [edi + 5ch], ecx
        mov    dword ptr [edi + 68h], eax
        jmp    NEAR PTR loc_1_81BE
loc_1_8090:
        mov    byte ptr [edi + 61h], 6
        mov    dword ptr [edi + 54h], 7f00h
        mov    word ptr [edi + 80h], 1ch
        mov    dword ptr [edi + 64h], 73f78h
        mov    word ptr [edi + 50h], 0feh
        mov    eax, ecx
        imul   eax, eax, 380h
        shl    ecx, 5
        mov    dword ptr [edi + 58h], eax
        mov    eax, 3d090h
        mov    dword ptr [edi + 5ch], ecx
        mov    dword ptr [edi + 68h], eax
        jmp    NEAR PTR loc_1_81BE
loc_1_80CF:
        movzx  eax, byte ptr [edi + 61h]
        cmp    eax, 9
        je     SHORT loc_1_80DF
        cmp    eax, 0bh
        je     SHORT loc_1_8112
        jmp    SHORT loc_1_8144
loc_1_80DF:
        mov    dword ptr [edi + 54h], 7f80h
        mov    word ptr [edi + 80h], 28h
        mov    word ptr [edi + 50h], 0ffh
        mov    eax, ecx
        mov    esi, ecx
        imul   eax, eax, 500h
        imul   esi, esi, 0a5h
        mov    dword ptr [edi + 58h], eax
        mov    eax, ecx
        shl    eax, 5
        mov    dword ptr [edi + 5ch], eax
        jmp    SHORT loc_1_8144
loc_1_8112:
        mov    byte ptr [edi + 61h], 0bh
        mov    dword ptr [edi + 54h], 7f80h
        mov    word ptr [edi + 80h], 28h
        mov    word ptr [edi + 50h], 0ffh
        mov    eax, ecx
        mov    esi, ecx
        imul   eax, eax, 500h
        imul   esi, esi, 54h
        mov    dword ptr [edi + 58h], eax
        mov    eax, ecx
        shl    eax, 5
        mov    dword ptr [edi + 5ch], eax
loc_1_8144:
        mov    al, byte ptr [edi + 61h]
        cmp    al, 9
        je     SHORT loc_1_814F
        cmp    al, 0bh
        jne    SHORT loc_1_81C1
loc_1_814F:
        mov    ecx, 0e2h
        mov    eax, esi
        sub    edx, edx
        div    ecx
        mov    ecx, 64h
        sub    edx, edx
        imul   eax, eax, 8ah
        add    eax, 32h
        div    ecx
        mov    ecx, 1c4h
        sub    edx, edx
        imul   eax, eax, 3e8h
        mov    dword ptr [edi + 64h], eax
        mov    eax, esi
        div    ecx
        mov    ecx, 64h
        sub    edx, edx
        imul   eax, eax, 8ah
        add    eax, 32h
        div    ecx
        mov    ecx, 384h
        sub    edx, edx
        imul   eax, eax, 3e8h
        mov    dword ptr [edi + 68h], eax
        mov    eax, esi
        div    ecx
        mov    ecx, 64h
        sub    edx, edx
        imul   eax, eax, 8ah
        add    eax, 32h
        div    ecx
        imul   eax, eax, 3e8h
loc_1_81BE:
        mov    dword ptr [edi + 6ch], eax
loc_1_81C1:
        push   edi
        lea    esi, [edi + 70h]
        call   sub_1_9B44
        movzx  ecx, word ptr [edi + 80h]
        imul   ecx, dword ptr [edi + 7ch]
        add    esp, 4
        mov    dword ptr [esi], ecx
        cmp    word ptr [edi + 50h], 0
        je     SHORT loc_1_821F
        movzx  eax, word ptr [edi + 50h]
        shl    eax, 2
        sub    edx, edx
        mov    dword ptr [ebp - 8], eax
        mov    eax, ecx
        div    dword ptr [ebp - 8]
        sub    edx, edx
        mov    dword ptr [ebp - 4], eax
        mov    eax, ecx
        div    dword ptr [ebp - 8]
        test   edx, edx
        jne    SHORT loc_1_8203
        dec    dword ptr [ebp - 4]
loc_1_8203:
        mov    byte ptr [edi + 89h], 80h
        mov    al, byte ptr [ebp - 4]
        mov    byte ptr [edi + 87h], al
        mov    ax, word ptr [edi + 50h]
        dec    al
        mov    byte ptr [edi + 88h], al
loc_1_821F:
        movzx  eax, byte ptr [edi + 63h]
        cmp    eax, 4
        je     SHORT loc_1_824A
        cmp    eax, 6
        je     SHORT loc_1_827E
        mov    al, byte ptr [edi + 61h]
        cmp    al, 0ch
        je     SHORT loc_1_8241
        cmp    al, 0dh
        je     SHORT loc_1_8241
        mov    byte ptr [edi + 8ch], 2
        jmp    SHORT loc_1_8285
loc_1_8241:
        mov    byte ptr [edi + 8ch], 5
        jmp    SHORT loc_1_8285
loc_1_824A:
        mov    eax, 3
        mov    byte ptr [edi + 8ch], al
        cmp    byte ptr [edi + 19h], al
        jne    SHORT loc_1_825D
        mov    byte ptr [edi + 40h], al
loc_1_825D:
        cmp    byte ptr [edi + 121h], 0
        jne    SHORT loc_1_8285
        push   1
        push   11013h
        push   1122h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebx, eax
        jmp    SHORT loc_1_8285
loc_1_827E:
        mov    byte ptr [edi + 8ch], 4
loc_1_8285:
        mov    edi, dword ptr [ebp + 10h]
        test   edi, edi
        je     SHORT loc_1_8296
        add    edi, 24h
        mov    ecx, 8
        rep movsd
loc_1_8296:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_829F:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        push   esi
        call   sub_1_66A9
        mov    edi, dword ptr [esp + 14h]
        add    esp, 4
        add    esi, 34h
        add    edi, 20h
        movsd
        movsd
        movsd
        pop    edi
        pop    esi
        ret
sub_1_82BE:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_1_9C5A
        add    esp, 8
        push   esi
        call   sub_1_653B
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_836F
        push   1bh
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_836F
        push   0
        push   3
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        mov    al, byte ptr [esi + 44h]
        add    al, 2
        push   eax
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_836F
        push   0
        push   3
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        push   esi
        call   sub_1_8BC4
        add    esp, 4
        lea    eax, [ebp - 1]
        push   0
        push   8
        push   eax
        push   8
        push   esi
        call   sub_1_740C
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_1_836F
        and    byte ptr [ebp - 1], 18h
        shr    byte ptr [ebp - 1], 3
        mov    cl, byte ptr [ebp - 1]
        cmp    byte ptr [esi + 44h], cl
        je     SHORT loc_1_836F
        push   1
        push   1100eh
        push   1169h
        call   sub_1_1CB6
        add    esp, 0ch
loc_1_836F:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_8374:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_653B
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_8452
        mov    dword ptr [esi + 34h], 0
        cmp    byte ptr [esi + 19h], 1
        jne    SHORT loc_1_83F6
        cmp    byte ptr [esi + 3ch], 0
        jne    SHORT loc_1_83F6
        cmp    word ptr [esi + 16h], 3ch
        jae    NEAR PTR loc_1_843C
        push   3
        push   esi
        call   sub_1_6CF2
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_8452
        push   11h
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_8452
        push   0
        push   2710h
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        push   0
        push   esi
        call   sub_1_6CF2
        mov    word ptr [esi + 38h], 0ffffh
        add    esp, 8
        jmp    SHORT loc_1_843C
loc_1_83F6:
        push   0dh
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_8452
        push   0
        push   3
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        push   2
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_8452
        push   0
        push   2710h
        push   esi
        call   sub_1_7191
        mov    word ptr [esi + 38h], 0
        add    esp, 0ch
loc_1_843C:
        push   0bh
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_8452
        mov    byte ptr [esi + 11eh], 1
loc_1_8452:
        pop    esi
        ret
sub_1_8454:
        sub    esp, 4
        xor    eax, eax
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        mov    byte ptr [esi + 120h], al
        mov    byte ptr [esi + 3ah], al
        mov    ax, word ptr [esi + 16h]
        cmp    ax, 3fh
        ja     SHORT loc_1_84D3
        cmp    ax, 3ch
        jb     SHORT loc_1_84D3
        cmp    byte ptr [esi + 19h], 1
        jne    SHORT loc_1_84D3
        push   esi
        call   sub_1_66A9
        add    esp, 4
        mov    edi, eax
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1122h
        je     SHORT loc_1_84BD
        cmp    byte ptr [esi + 82h], 0
        je     SHORT loc_1_84C8
        lea    eax, [esp + 0fh]
        push   eax
        push   esi
        call   sub_1_A152
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_84CC
        cmp    byte ptr [esp + 0fh], 0
        jne    SHORT loc_1_84C8
loc_1_84BD:
        push   esi
        call   sub_1_A104
        add    esp, 4
        mov    edi, eax
loc_1_84C8:
        test   edi, edi
        je     SHORT loc_1_84D3
loc_1_84CC:
        mov    eax, edi
        jmp    NEAR PTR loc_1_865D
loc_1_84D3:
        push   esi
        call   sub_1_66A9
        add    esp, 4
        cmp    byte ptr [esi + 3ch], 0
        jne    SHORT loc_1_852D
        cmp    byte ptr [esi + 3bh], 0
        jne    NEAR PTR loc_1_85A3
        cmp    byte ptr [esi + 19h], 1
        jne    SHORT loc_1_852D
        mov    ax, word ptr [esi + 16h]
        cmp    ax, 3ch
        jb     SHORT loc_1_852D
        cmp    ax, 3fh
        ja     SHORT loc_1_852D
        push   0eh
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_865D
        push   1
        push   0a3930h
        push   esi
        call   sub_1_7191
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_865D
loc_1_852D:
        cmp    byte ptr [esi + 3bh], 0
        jne    SHORT loc_1_85A3
        mov    al, byte ptr [esi + 19h]
        cmp    al, 7
        je     SHORT loc_1_853E
        cmp    al, 5
        jne    SHORT loc_1_85A3
loc_1_853E:
        cmp    byte ptr [esi + 0b5h], 0
        jne    SHORT loc_1_8572
        push   0bh
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_865D
        push   0
        push   9eb10h
        push   esi
        call   sub_1_7191
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_865D
loc_1_8572:
        cmp    byte ptr [esi + 3ch], 0
        jne    SHORT loc_1_85A3
        push   0eh
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_865D
        push   1
        push   0a3930h
        push   esi
        call   sub_1_7191
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_865D
loc_1_85A3:
        push   0
        push   esi
        call   sub_1_9E52
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_865D
        push   esi
        call   sub_1_9DFA
        add    esp, 4
        push   esi
        call   sub_1_9CBB
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_8644
        cmp    byte ptr [esi + 3bh], 0
        jne    SHORT loc_1_8632
        cmp    byte ptr [esi + 3ch], 0
        jne    SHORT loc_1_8632
        cmp    byte ptr [esi + 1ah], 4
        jne    SHORT loc_1_8632
        cmp    byte ptr [esi + 1bh], 5
        jne    SHORT loc_1_8632
        cmp    byte ptr [esi + 19h], 1
        jne    SHORT loc_1_8632
        push   1bh
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_865D
        push   0
        push   3
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        push   5
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_865B
        push   0
        push   3
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        jmp    SHORT loc_1_8644
loc_1_8632:
        mov    al, byte ptr [esi + 8bh]
        push   eax
        push   esi
        call   sub_1_82BE
        add    esp, 8
        mov    ebx, eax
loc_1_8644:
        add    esi, 70h
        mov    edi, dword ptr [esp + 18h]
        add    edi, 20h
        mov    ecx, 8
        mov    word ptr [esi - 38h], 0ffffh
        rep movsd
loc_1_865B:
        mov    eax, ebx
loc_1_865D:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_8664:
        sub    esp, 0ch
        mov    byte ptr [esp + 3], 0
        mov    eax, dword ptr [esp + 14h]
        push   ebx
        mov    ebx, dword ptr [esp + 14h]
        push   esi
        push   edi
        add    eax, 20h
        push   ebp
        mov    edi, ebx
        mov    esi, eax
        push   ebx
        mov    byte ptr [ebx + 11bh], 0
        mov    dword ptr [esp + 18h], eax
        movsd
        movsd
        movsb
        lea    esi, [ebx + 0ch]
        call   sub_1_A5B1
        mov    eax, dword ptr [esp + 28h]
        add    esp, 4
        add    eax, 68h
        mov    ecx, 0ah
        mov    edi, eax
        push   ebx
        mov    dword ptr [esp + 1ch], eax
        rep movsd
        call   sub_1_6E6A
        add    esp, 4
        push   ebx
        call   sub_1_8A04
        add    esp, 4
        push   ebx
        call   sub_1_A595
        add    esp, 4
        push   ebx
        call   sub_1_8851
        add    esp, 4
        mov    ebp, eax
        test   ebp, ebp
        jne    NEAR PTR loc_1_8827
        push   ebx
        call   sub_1_65A2
        add    esp, 4
        mov    ebp, eax
        test   ebp, ebp
        jne    NEAR PTR loc_1_8827
        mov    al, 1
        mov    cl, byte ptr [ebx + 3]
        and    cl, 3
        push   ebx
        shl    al, cl
        mov    byte ptr [esp + 17h], 1
        mov    byte ptr [ebx + 6], al
        call   sub_1_66A9
        add    esp, 4
        mov    ebp, eax
        push   ebp
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 115ch
        jne    SHORT loc_1_872C
        push   0
        push   493e0h
        push   ebx
        call   sub_1_7191
        add    esp, 0ch
        mov    ebp, eax
loc_1_872C:
        push   ebp
        call   sub_1_247A
        movzx  eax, ax
        add    esp, 4
        cmp    eax, 111eh
        je     SHORT loc_1_874F
        cmp    eax, 1122h
        je     SHORT loc_1_8760
        cmp    eax, 15a0h
        je     SHORT loc_1_876B
        jmp    SHORT loc_1_8781
loc_1_874F:
        cmp    byte ptr [ebx + 19h], 2
        jne    SHORT loc_1_8781
        push   ebx
        call   sub_1_66A9
        add    esp, 4
        jmp    SHORT loc_1_877F
loc_1_8760:
        push   ebx
        call   sub_1_A104
        add    esp, 4
        jmp    SHORT loc_1_877F
loc_1_876B:
        push   1
        push   11010h
        push   1172h
        call   sub_1_1CB6
        add    esp, 0ch
loc_1_877F:
        mov    ebp, eax
loc_1_8781:
        test   ebp, ebp
        jne    NEAR PTR loc_1_8827
        mov    byte ptr [ebx + 41h], 3
        push   0
        push   ebx
        call   sub_1_6CF2
        add    esp, 8
        mov    ebp, eax
        test   ebp, ebp
        jne    NEAR PTR loc_1_8827
        mov    byte ptr [ebx + 19h], 0ah
        push   ebx
        call   sub_1_A39C
        add    esp, 4
        mov    ebp, eax
        test   ebp, ebp
        jne    SHORT loc_1_87FA
        cmp    byte ptr [ebx + 2], 0
        je     SHORT loc_1_87CE
        mov    eax, dword ptr [esp + 24h]
        mov    al, byte ptr [eax + 51h]
        push   eax
        push   ebx
        call   sub_1_A1A5
        add    esp, 8
        mov    ebp, eax
loc_1_87CE:
        test   ebp, ebp
        jne    SHORT loc_1_87FA
        mov    edi, dword ptr [esp + 18h]
        lea    esi, [ebx + 0ch]
        mov    byte ptr [ebx + 11bh], 1
        mov    ecx, 0ah
        rep movsd
        cmp    byte ptr [ebx + 2], 0
        je     SHORT loc_1_87FA
        mov    edi, dword ptr [esp + 14h]
        mov    esi, ebx
        mov    byte ptr [ebx + 2], 0
        movsd
        movsd
        movsb
loc_1_87FA:
        mov    al, byte ptr [ebx + 19h]
        cmp    al, 2
        je     SHORT loc_1_8811
        cmp    al, 4
        je     SHORT loc_1_8811
        cmp    al, 6
        je     SHORT loc_1_8811
        cmp    al, 8
        je     SHORT loc_1_8811
        cmp    al, 9
        jne    SHORT loc_1_8827
loc_1_8811:
        push   2
        push   11010h
        push   1165h
        call   sub_1_1CB6
        add    esp, 0ch
        mov    ebp, eax
loc_1_8827:
        push   dword ptr [esp + 13h]
        push   ebx
        call   sub_1_6500
        add    esp, 8
        lea    eax, [ebx + 0ch]
        push   ebx
        push   eax
        push   dword ptr [ebx + 110h]
        call   sub_1_A1A4
        add    esp, 0ch
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_1_8851:
        push   ebx
        push   esi
        xor    si, si
        push   edi
        mov    ebx, dword ptr [esp + 10h]
loc_1_885B:
        mov    byte ptr [ebx + 19h], 0ah
        push   ebx
        call   sub_1_6E6A
        add    esp, 4
        push   4
        push   ebx
        call   sub_1_A607
        add    esp, 8
        mov    edi, eax
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 115bh
        je     SHORT loc_1_8897
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1157h
        jne    NEAR PTR loc_1_89A0
loc_1_8897:
        mov    byte ptr [ebx + 19h], 1
        push   ebx
        call   sub_1_6E6A
        add    esp, 4
        push   4
        push   ebx
        call   sub_1_A607
        add    esp, 8
        mov    edi, eax
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 115bh
        je     SHORT loc_1_88D3
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1157h
        jne    NEAR PTR loc_1_89A0
loc_1_88D3:
        mov    byte ptr [ebx + 19h], 2
        push   ebx
        call   sub_1_6E6A
        add    esp, 4
        push   4
        push   ebx
        call   sub_1_A607
        add    esp, 8
        mov    edi, eax
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 115bh
        je     SHORT loc_1_890F
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1157h
        jne    NEAR PTR loc_1_89A0
loc_1_890F:
        mov    byte ptr [ebx + 19h], 0ah
        push   ebx
        call   sub_1_6E6A
        add    esp, 4
        push   3
        push   ebx
        call   sub_1_A607
        add    esp, 8
        mov    edi, eax
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 115bh
        je     SHORT loc_1_8947
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1157h
        jne    SHORT loc_1_89A0
loc_1_8947:
        mov    byte ptr [ebx + 19h], 1
        push   ebx
        call   sub_1_6E6A
        add    esp, 4
        push   3
        push   ebx
        call   sub_1_A607
        add    esp, 8
        mov    edi, eax
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 115bh
        je     SHORT loc_1_897F
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 1157h
        jne    SHORT loc_1_89A0
loc_1_897F:
        inc    si
        push   0
        push   3e8h
        push   dword ptr [ebx + 110h]
        call   sub_1_5BDA
        add    esp, 0ch
        cmp    si, 2
        jb     NEAR PTR loc_1_885B
loc_1_89A0:
        push   edi
        call   sub_1_247A
        movzx  eax, ax
        add    esp, 4
        cmp    eax, 1157h
        jl     SHORT loc_1_89D6
        cmp    eax, 1158h
        jle    SHORT loc_1_89DA
        cmp    eax, 115bh
        je     SHORT loc_1_89DA
        cmp    eax, 115dh
        je     SHORT loc_1_89E8
        cmp    eax, 1162h
        je     SHORT loc_1_89FE
        cmp    eax, 1163h
        je     SHORT loc_1_89E8
loc_1_89D6:
        xor    edi, edi
        jmp    SHORT loc_1_89FE
loc_1_89DA:
        push   1
        push   11017h
        push   1165h
        jmp    SHORT loc_1_89F4
loc_1_89E8:
        push   1
        push   11017h
        push   1166h
loc_1_89F4:
        call   sub_1_1CB6
        add    esp, 0ch
        mov    edi, eax
loc_1_89FE:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_8A04:
        sub    esp, 8
        mov    byte ptr [esp + 2], 0
        mov    byte ptr [esp + 6], 12h
        push   ebx
        mov    eax, 18h
        mov    byte ptr [esp + 8], al
        push   esi
        mov    ebx, dword ptr [esp + 14h]
        push   edi
        mov    byte ptr [esp + 0fh], al
        push   ebp
        lea    ecx, [esp + 15h]
        push   0
        mov    byte ptr [esp + 1bh], 80h
        mov    byte ptr [esp + 19h], 10h
        mov    byte ptr [ebx + 1ch], 1
        push   1
        push   ecx
        push   ebx
        call   sub_1_5DDF
        add    esp, 10h
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_1_8A71
        lea    eax, [esp + 12h]
        push   1
        push   eax
        push   ebx
        call   sub_1_5FD8
        add    esp, 0ch
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_1_8A71
        cmp    byte ptr [esp + 12h], 90h
        mov    byte ptr [ebx + 1ch], 3
        je     SHORT loc_1_8A71
        mov    byte ptr [ebx + 1ch], 2
loc_1_8A71:
        cmp    byte ptr [ebx + 1ch], 3
        jne    NEAR PTR loc_1_8B00
        test   esi, esi
        jne    SHORT loc_1_8ABB
        lea    eax, [esp + 14h]
        push   0
        push   1
        push   eax
        push   ebx
        call   sub_1_5DDF
        add    esp, 10h
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_1_8ABB
        lea    eax, [esp + 12h]
        push   1
        push   eax
        push   ebx
        call   sub_1_5FD8
        add    esp, 0ch
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_1_8ABB
        mov    al, byte ptr [esp + 12h]
        and    al, 0f0h
        cmp    al, 70h
        jne    SHORT loc_1_8ABB
        mov    byte ptr [ebx + 1ch], 8
loc_1_8ABB:
        cmp    byte ptr [ebx + 1ch], 3
        jne    SHORT loc_1_8B00
        test   esi, esi
        jne    SHORT loc_1_8B00
        lea    eax, [esp + 16h]
        push   0
        push   2
        push   eax
        push   ebx
        call   sub_1_5DDF
        add    esp, 10h
        mov    esi, eax
        push   esi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 115dh
        jne    SHORT loc_1_8AFC
        lea    eax, [esp + 12h]
        push   1
        push   eax
        push   ebx
        call   sub_1_5FD8
        add    esp, 0ch
        mov    esi, eax
        jmp    SHORT loc_1_8B00
loc_1_8AFC:
        mov    byte ptr [ebx + 1ch], 4
loc_1_8B00:
        push   dword ptr [ebx + 110h]
        call   sub_1_384C
        add    esp, 4
        test   al, al
        jne    SHORT loc_1_8B6C
        cmp    byte ptr [ebx + 1ch], 4
        jne    SHORT loc_1_8B6C
        xor    ebp, ebp
        test   esi, esi
        jne    SHORT loc_1_8B6C
        xor    di, di
loc_1_8B21:
        push   edi
        push   dword ptr [ebx + 0e8h]
        push   dword ptr [ebx + 110h]
        call   sub_1_6100
        add    esp, 0ch
        push   dword ptr [ebx + 0e8h]
        push   dword ptr [ebx + 110h]
        call   sub_1_60DF
        add    esp, 8
        xor    ecx, ecx
        mov    cl, al
        movzx  eax, di
        and    ecx, 3
        cmp    ecx, eax
        jne    SHORT loc_1_8B5A
        inc    bp
loc_1_8B5A:
        inc    di
        cmp    di, 4
        jb     SHORT loc_1_8B21
        cmp    bp, 4
        jne    SHORT loc_1_8B6C
        mov    byte ptr [ebx + 1ch], 5
loc_1_8B6C:
        cmp    byte ptr [ebx + 1ch], 5
        jne    SHORT loc_1_8BBA
        test   esi, esi
        jne    SHORT loc_1_8BBA
        lea    eax, [esp + 13h]
        push   0
        push   1
        push   eax
        push   ebx
        call   sub_1_5DDF
        add    esp, 10h
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_1_8BBA
        lea    eax, [esp + 12h]
        push   1
        push   eax
        push   ebx
        call   sub_1_5FD8
        add    esp, 0ch
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_1_8BBA
        mov    al, byte ptr [esp + 12h]
        and    al, 0e0h
        jne    SHORT loc_1_8BB2
        mov    byte ptr [ebx + 1ch], 7
        jmp    SHORT loc_1_8BBA
loc_1_8BB2:
        cmp    al, 40h
        jne    SHORT loc_1_8BBA
        mov    byte ptr [ebx + 1ch], 6
loc_1_8BBA:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_8BC4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        xor    bl, bl
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 45h]
        push   eax
        push   esi
        call   sub_1_A6A9
        add    esp, 8
        cmp    byte ptr [esi + 1ch], 7
        jne    SHORT loc_1_8C0A
        mov    byte ptr [ebp - 8], 8eh
        mov    al, byte ptr [esi + 3]
        push   0
        and    al, 3
        push   3
        shl    al, 5
        lea    ecx, [ebp - 8]
        or     al, 18h
        push   ecx
        push   esi
        mov    byte ptr [ebp - 7], al
        mov    byte ptr [ebp - 6], 0c0h
        call   sub_1_5DDF
        add    esp, 10h
loc_1_8C0A:
        mov    al, byte ptr [esi + 1ch]
        cmp    al, 4
        je     SHORT loc_1_8C25
        cmp    al, 5
        je     SHORT loc_1_8C25
        cmp    al, 6
        je     SHORT loc_1_8C25
        cmp    al, 7
        je     SHORT loc_1_8C25
        cmp    al, 8
        jne    NEAR PTR loc_1_8CD3
loc_1_8C25:
        cmp    byte ptr [esi + 19h], 1
        jne    SHORT loc_1_8C3F
        movzx  eax, byte ptr [esi + 1ah]
        cmp    eax, 4
        jl     SHORT loc_1_8C39
        cmp    eax, 5
        jle    SHORT loc_1_8C3D
loc_1_8C39:
        xor    bl, bl
        jmp    SHORT loc_1_8C3F
loc_1_8C3D:
        mov    bl, 1ch
loc_1_8C3F:
        mov    al, byte ptr [esi + 45h]
        or     al, bl
        push   eax
        push   dword ptr [esi + 0e4h]
        push   dword ptr [esi + 110h]
        call   sub_1_6100
        add    esp, 0ch
        push   0
        push   dword ptr [esi + 0e8h]
        push   dword ptr [esi + 110h]
        call   sub_1_6100
        movzx  eax, byte ptr [esi + 45h]
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_8C7C
        cmp    eax, 2
        jne    SHORT loc_1_8C92
loc_1_8C7C:
        push   1
        push   dword ptr [esi + 0e8h]
        push   dword ptr [esi + 110h]
        call   sub_1_6100
        add    esp, 0ch
loc_1_8C92:
        push   dword ptr [esi + 110h]
        mov    byte ptr [ebp - 0ch], 13h
        mov    byte ptr [ebp - 0ah], 0fh
        xor    eax, eax
        mov    byte ptr [ebp - 0bh], al
        mov    byte ptr [ebp - 9], al
        call   sub_1_A696
        add    esp, 4
        test   al, al
        je     SHORT loc_1_8CBE
        cmp    byte ptr [esi + 1ch], 7
        jne    SHORT loc_1_8CBE
        or     byte ptr [ebp - 0ch], 80h
loc_1_8CBE:
        push   0
        lea    eax, [ebp - 0ch]
        push   4
        push   eax
        push   esi
        call   sub_1_5DDF
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_1_8CF2
loc_1_8CD3:
        mov    al, byte ptr [esi + 46h]
        push   0
        push   3
        lea    ecx, [ebp - 4]
        push   ecx
        mov    byte ptr [ebp - 3], al
        mov    byte ptr [ebp - 4], 3
        mov    byte ptr [ebp - 2], 2
        push   esi
        call   sub_1_5DDF
        add    esp, 10h
loc_1_8CF2:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_8CF8:
        mov    eax, dword ptr [esp + 4]
        mov    byte ptr [eax + 4afh], 0
        mov    byte ptr [eax + 4b0h], 1
        ret
sub_1_8D0B:
        mov    eax, dword ptr [esp + 4]
        mov    byte ptr [eax + 4b0h], 0
        ret
sub_1_8D17:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    word ptr [esp + 0eh], bx
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        push   esi
        call   sub_1_A705
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_8E0D
        mov    di, word ptr [esp + 1ch]
        mov    ebp, 8000h
loc_1_8D44:
        push   2
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_8E0D
        push   0
        push   5
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        lea    eax, [esp + 1dh]
        add    esp, 0ch
        push   eax
        push   esi
        call   sub_1_A6C9
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_8E0D
        shr    word ptr [esp + 12h], 1
        test   byte ptr [esp + 11h], 10h
        je     SHORT loc_1_8D90
        or     word ptr [esp + 12h], bp
loc_1_8D90:
        inc    bl
        movzx  eax, di
        movzx  ecx, bl
        cmp    ecx, eax
        jb     SHORT loc_1_8D44
        cmp    di, 8
        jne    SHORT loc_1_8DA8
        shr    word ptr [esp + 12h], 8
loc_1_8DA8:
        mov    eax, dword ptr [esp + 20h]
        cmp    di, 10h
        mov    cl, byte ptr [esp + 12h]
        mov    byte ptr [eax], cl
        jne    SHORT loc_1_8DBF
        mov    cl, byte ptr [esp + 13h]
        mov    byte ptr [eax + 1], cl
loc_1_8DBF:
        push   2
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_8E0D
        push   0
        push   5
        push   dword ptr [esi + 110h]
        call   sub_1_5BDA
        lea    eax, [esp + 1dh]
        add    esp, 0ch
        push   eax
        push   esi
        call   sub_1_A6C9
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_8E0D
        test   byte ptr [esp + 11h], 10h
        jne    SHORT loc_1_8E0D
        push   1
        push   1103bh
        push   1158h
        call   sub_1_1CB6
        add    esp, 0ch
loc_1_8E0D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_8E15:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        xor    edi, edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 1ah], 5
        jne    NEAR PTR loc_1_8EC8
        mov    byte ptr [ebp - 2], 12h
        mov    al, byte ptr [esi + 6]
        or     al, 0e0h
        shl    al, 2
        cmp    byte ptr [ebp + 0ch], 0
        mov    byte ptr [ebp - 1], al
        je     SHORT loc_1_8E85
        cmp    byte ptr [esi + 10dh], 0
        jne    SHORT loc_1_8EC8
        push   edi
        lea    eax, [ebp - 2]
        or     byte ptr [ebp - 1], 1
        or     byte ptr [ebp - 1], 2
        push   2
        push   eax
        push   esi
        call   sub_1_5DDF
        add    esp, 10h
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_1_8E7C
        push   esi
        call   sub_1_6E6A
        add    esp, 4
        push   esi
        call   sub_1_6660
        add    esp, 4
        jmp    SHORT loc_1_8EC8
loc_1_8E7C:
        mov    byte ptr [esi + 10dh], 1
        jmp    SHORT loc_1_8EC8
loc_1_8E85:
        cmp    byte ptr [esi + 10dh], 0
        je     SHORT loc_1_8EC8
        push   0
        lea    eax, [ebp - 2]
        and    byte ptr [ebp - 1], 0feh
        and    byte ptr [ebp - 1], 0fdh
        push   2
        push   eax
        push   esi
        call   sub_1_5DDF
        add    esp, 10h
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_1_8EC1
        push   esi
        call   sub_1_6E6A
        add    esp, 4
        push   esi
        call   sub_1_6660
        add    esp, 4
        jmp    SHORT loc_1_8EC8
loc_1_8EC1:
        mov    byte ptr [esi + 10dh], 0
loc_1_8EC8:
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_8ED0:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        mov    di, word ptr [esp + 14h]
        push   ebp
        mov    ecx, dword ptr [esi + 5ch]
        mov    ebx, dword ptr [esi + 54h]
        mov    byte ptr [esi + 0b9h], 0
        movzx  eax, di
        imul   ecx, eax
        movsx  eax, bx
        cmp    eax, ecx
        jg     SHORT loc_1_8F13
        test   ebx, ebx
        je     NEAR PTR loc_1_9011
        mov    eax, ecx
        sub    edx, edx
        div    ebx
        sub    edx, edx
        mov    byte ptr [esi + 0b9h], al
        mov    eax, ecx
        div    ebx
        mov    ecx, edx
loc_1_8F13:
        mov    ax, word ptr [esi + 4ch]
        xor    ebx, ebx
        test   ax, ax
        je     NEAR PTR loc_1_9050
        movzx  ecx, cx
        movsx  ebp, ax
        mov    eax, ecx
        sub    edx, edx
        div    ebp
        sub    edx, edx
        push   ebx
        push   esi
        mov    byte ptr [esi + 0b8h], al
        mov    eax, ecx
        div    ebp
        inc    dl
        mov    byte ptr [esi + 0bah], dl
        call   sub_1_5E6F
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_9040
        mov    word ptr [esi + 0d0h], bx
        push   edi
        mov    word ptr [esi + 0d2h], 1
        push   esi
        call   sub_1_A934
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_9040
        push   1
        mov    dword ptr [esi + 0d4h], ebx
        push   dword ptr [esi + 110h]
        mov    byte ptr [esi + 0b4h], 1
        mov    byte ptr [esi + 10eh], 1
        mov    byte ptr [esi + 10fh], bl
        call   sub_1_7105
        add    esp, 8
        push   esi
        call   sub_1_A751
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_9040
        push   esi
        call   sub_1_66A9
        add    esp, 4
        push   eax
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 115ch
        jne    SHORT loc_1_902C
        mov    al, byte ptr [esi + 8ah]
        push   ebx
        sub    al, byte ptr [esi + 3eh]
        cmp    al, 1
        sbb    eax, eax
        inc    ax
        movzx  eax, ax
        push   dword ptr [esi + eax*4 + 64h]
        push   esi
        call   sub_1_7191
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_9015
        mov    byte ptr [esi + 0b4h], bl
        cmp    byte ptr [esi + 0b5h], bl
        jne    SHORT loc_1_9040
        cmp    byte ptr [esi + 0b6h], bl
        jne    SHORT loc_1_9040
        push   1
        push   11020h
        push   116ah
        jmp    SHORT loc_1_9038
loc_1_9011:
        push   1
        jmp    SHORT loc_1_9052
loc_1_9015:
        push   esi
        call   sub_1_653B
        add    esp, 4
        push   1
        push   11020h
        push   115eh
        jmp    SHORT loc_1_9038
loc_1_902C:
        push   1
        push   11020h
        push   1154h
loc_1_9038:
        call   sub_1_1CB6
        add    esp, 0ch
loc_1_9040:
        xor    ecx, ecx
        mov    byte ptr [esi + 10eh], cl
        mov    byte ptr [esi + 10fh], cl
        jmp    SHORT loc_1_9064
loc_1_9050:
        push   2
loc_1_9052:
        push   11020h
        push   116ch
        call   sub_1_1CB6
        add    esp, 0ch
loc_1_9064:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
_LTEXT ends
        end
