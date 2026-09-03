; vip_lcode_01_02.asm -- VIP object 1 segment _LTEXT  (part 2/3)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_4B70:
        push   esi
        push   edi
        db  0A1h, 0E8h, 0BFh, 000h, 000h
        mov    dl, byte ptr [esp + 0ch]
        cmp    byte ptr [eax + 10h], dl
        jne    SHORT loc_1_4B88
        mov    eax, dword ptr [eax + 4]
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_4B88:
        sub    eax, eax
        sub    esi, esi
        db  08Bh, 00Dh, 068h, 0C0h, 000h, 000h
        cmp    ecx, eax
        jle    SHORT loc_1_4BC7
        inc    edi
        db  0BFh, 00Ch, 0C0h, 000h, 000h
loc_1_4B9C:
        cmp    byte ptr [edi], dl
        je     SHORT loc_1_4BAC
        add    edi, 14h
        inc    esi
        cmp    ecx, esi
        jg     SHORT loc_1_4B9C
        pop    edi
        pop    esi
        ret
        db  090h
loc_1_4BAC:
        lea    eax, [esi*4]
        lea    ecx, [eax + eax*4]
        db  08Dh, 081h, 0FCh, 0BFh, 000h, 000h
        db  0A3h, 0E8h, 0BFh, 000h, 000h
        db  08Bh, 081h, 000h, 0C0h, 000h, 000h
loc_1_4BC7:
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
sub_1_4BCC:
        push   esi
        push   edi
        sub    edi, edi
        db  039h, 03Dh, 068h, 0C0h, 000h, 000h
        jle    SHORT loc_1_4BF1
        db  0BEh, 008h, 0C0h, 000h, 000h
loc_1_4BDD:
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_1_4BE5
        call   eax
loc_1_4BE5:
        inc    edi
        add    esi, 14h
        db  039h, 03Dh, 068h, 0C0h, 000h, 000h
        jg     SHORT loc_1_4BDD
loc_1_4BF1:
        pop    edi
        pop    esi
        ret
sub_1_4BF4:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 14h]
        lea    esi, [eax + 1ch]
        mov    eax, dword ptr [eax + 1ch]
        or     eax, eax
        je     SHORT loc_1_4C33
        mov    ecx, dword ptr [esp + 1ch]
        mov    dx, word ptr [esp + 20h]
        mov    bl, byte ptr [esp + 24h]
        mov    edi, dword ptr [esp + 18h]
loc_1_4C16:
        cmp    dword ptr [eax + 4], edi
        jne    SHORT loc_1_4C2B
        cmp    dword ptr [eax + 8], ecx
        jne    SHORT loc_1_4C2B
        cmp    word ptr [eax + 0ch], dx
        jne    SHORT loc_1_4C2B
        cmp    byte ptr [eax + 0eh], bl
        je     SHORT loc_1_4C33
loc_1_4C2B:
        mov    esi, eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_4C16
loc_1_4C33:
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx], esi
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_1_4C40:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    eax, eax
        mov    ecx, dword ptr [esp + 18h]
        mov    esi, dword ptr [ecx]
        movzx  ecx, byte ptr [ecx + 8]
        mov    dword ptr [esp + 10h], ecx
        cmp    ecx, eax
        je     SHORT loc_1_4CD3
        mov    ebp, dword ptr [esp + 1ch]
        nop
loc_1_4C60:
        mov    cl, byte ptr [esi]
        or     cl, cl
        je     SHORT loc_1_4CD3
        cmp    cl, 1
        jne    SHORT loc_1_4C70
        inc    eax
        inc    esi
        jmp    SHORT loc_1_4CCD
        db  090h
loc_1_4C70:
        mov    dl, byte ptr [esi + 1]
        movzx  edi, dl
        lea    ebx, [edi + eax]
        cmp    ebx, dword ptr [esp + 10h]
        ja     SHORT loc_1_4CE0
        mov    bl, byte ptr [esi + 2]
        dec    bl
        cmp    cl, 44h
        jne    SHORT loc_1_4C90
        cmp    dl, 4
        jb     SHORT loc_1_4CEC
        jmp    SHORT loc_1_4CC9
loc_1_4C90:
        cmp    dl, 3
        jb     SHORT loc_1_4CF8
        cmp    cl, 83h
        je     SHORT loc_1_4CB0
        cmp    cl, 89h
        je     SHORT loc_1_4CB0
        cmp    cl, 7
        jne    SHORT loc_1_4CC9
        cmp    bl, dl
        jae    SHORT loc_1_4CC9
        mov    byte ptr [ebp + 1], al
        jmp    SHORT loc_1_4CC9
        db  08Dh, 049h, 000h
loc_1_4CB0:
        cmp    bl, dl
        jae    SHORT loc_1_4CC9
        mov    edx, edi
        and    ebx, 0ffh
        sub    edx, ebx
        cmp    edx, 4
        jb     SHORT loc_1_4D04
        mov    byte ptr [ebp + 3], cl
        mov    byte ptr [ebp], al
loc_1_4CC9:
        add    eax, edi
        add    esi, edi
loc_1_4CCD:
        cmp    eax, dword ptr [esp + 10h]
        jb     SHORT loc_1_4C60
loc_1_4CD3:
        pop    ebp
        mov    al, 28h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_4CE0:
        inc    al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_4CEC:
        inc    al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_4CF8:
        inc    al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_4D04:
        inc    al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
sub_1_4D10:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        mov    ebp, dword ptr [esi + 0ch]
        db  08Bh, 01Dh, 090h, 0C0h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_1_4D6A
        mov    edi, dword ptr [esp + 1ch]
loc_1_4D29:
        test   byte ptr [ebx + 14h], 8
        je     SHORT loc_1_4D64
        cmp    dword ptr [ebx + 0ch], ebp
        jne    SHORT loc_1_4D64
        push   ebx
        push   edi
        call   sub_1_6FA4
        add    esp, 8
        test   al, 1
        je     SHORT loc_1_4D64
        push   dword ptr [esp + 30h]
        push   dword ptr [esp + 30h]
        push   1
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 34h]
        push   dword ptr [ebx + 4]
        push   dword ptr [esp + 38h]
        push   edi
        push   esi
        call   dword ptr [esp + 38h]
        add    esp, 24h
loc_1_4D64:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    SHORT loc_1_4D29
loc_1_4D6A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
sub_1_4D70:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 14h]
        mov    al, byte ptr [edi + 9]
        push   eax
        call   sub_1_4B70
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_4DFC
        db  0FFh, 005h, 0F8h, 0C1h, 000h, 000h
        cmp    byte ptr [esp + 24h], 0
        jne    SHORT loc_1_4DD4
        mov    esi, dword ptr [esp + 10h]
        push   dword ptr [esp + 20h]
        mov    cl, byte ptr [edi + 9]
        push   ecx
        push   0
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 28h]
        push   dword ptr [esi + 4]
        push   dword ptr [edi + 0ch]
        push   dword ptr [edi + 10h]
        push   dword ptr [esp + 2ch]
        call   eax
        add    esp, 24h
        or     eax, eax
        je     SHORT loc_1_4E18
        push   0
        push   3
        push   3
        push   edi
        push   dword ptr [esi + 4]
        call   sub_1_9610
        add    esp, 14h
        pop    edi
        pop    esi
        ret
loc_1_4DD4:
        mov    cl, byte ptr [edi + 9]
        push   dword ptr [esp + 20h]
        push   ecx
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 24h]
        push   dword ptr [edi + 0ch]
        push   dword ptr [edi + 10h]
        push   dword ptr [esp + 24h]
        push   eax
        call   sub_1_4D10
        add    esp, 20h
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_4DFC:
        db  0FFh, 005h, 0F0h, 0C1h, 000h, 000h
        push   0
        push   2
        push   3
        push   edi
        mov    esi, dword ptr [esp + 20h]
        push   dword ptr [esi + 4]
        call   sub_1_9610
        add    esp, 14h
loc_1_4E18:
        pop    edi
        pop    esi
        ret
        db  090h
sub_1_4E1C:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     SHORT loc_1_4E3A
loc_1_4E29:
        mov    eax, edi
        mov    edi, dword ptr [edi]
        push   eax
        call   sub_1_B944
        add    esp, 4
        or     edi, edi
        jne    SHORT loc_1_4E29
loc_1_4E3A:
        push   esi
        call   sub_1_B944
        add    esp, 4
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
sub_1_4E48:
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 40h]
        mov    eax, dword ptr [edx + 0ch]
        test   byte ptr [esp + 4ch], 1
        mov    word ptr [esp + 12h], ax
        je     SHORT loc_1_4E88
        push   dword ptr [esp + 4ch]
        push   0
        push   0
        push   eax
        push   dword ptr [edx + 8]
        push   dword ptr [esp + 5ch]
        mov    ebp, dword ptr [esp + 5ch]
        push   ebp
        push   dword ptr [esp + 58h]
        call   sub_1_89E0
        add    esp, 20h
        jmp    SHORT loc_1_4E8C
        db  08Dh, 049h, 000h
loc_1_4E88:
        mov    ebp, dword ptr [esp + 44h]
loc_1_4E8C:
        mov    al, byte ptr [ebp + 9]
        push   eax
        mov    ax, word ptr [ebp + 4]
        push   eax
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 10h]
        push   dword ptr [esp + 48h]
        lea    eax, [esp + 30h]
        push   eax
        call   sub_1_4BF4
        add    esp, 18h
        mov    ebx, eax
        or     ebx, ebx
        jne    NEAR PTR loc_1_4F66
        push   60h
        call   sub_1_B93E
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_4EE8
        call   sub_1_0045
        db  0FFh, 005h, 018h, 0C2h, 000h, 000h
        push   dword ptr [esp + 40h]
        call   sub_1_B944
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  08Dh, 049h, 000h
loc_1_4EE8:
        mov    al, byte ptr [ebp + 9]
        push   eax
        mov    ax, word ptr [ebp + 4]
        push   eax
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 10h]
        push   dword ptr [esp + 48h]
        lea    eax, [esp + 30h]
        push   eax
        call   sub_1_4BF4
        add    esp, 18h
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_4F1C
        push   ebx
        call   sub_1_B944
        add    esp, 4
        mov    ebx, esi
        jmp    SHORT loc_1_4F66
        db  090h
loc_1_4F1C:
        mov    eax, dword ptr [esp + 1ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [eax], ebx
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebx + 8], eax
        mov    ax, word ptr [ebp + 4]
        mov    word ptr [ebx + 0ch], ax
        mov    al, byte ptr [ebp + 9]
        mov    byte ptr [ebx + 0eh], al
        db  0A0h, 0ECh, 0BFh, 000h, 000h
        mov    byte ptr [ebx + 0fh], al
        mov    word ptr [ebx + 14h], 0ffffh
        mov    dword ptr [ebx + 10h], 0
        mov    word ptr [ebx + 16h], 0
        mov    dword ptr [ebx + 18h], 0
loc_1_4F66:
        mov    ax, word ptr [ebp + 6]
        and    ax, 0ff1fh
        mov    ch, al
        mov    cl, ah
        shl    cx, 3
        mov    word ptr [esp + 10h], cx
        or     cx, cx
        jne    SHORT loc_1_4F9D
        mov    eax, dword ptr [esp + 48h]
        mov    dword ptr [ebx + 18h], eax
        add    eax, 8
        lea    edi, [ebx + 1ch]
        mov    esi, ebp
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_1_4F9D:
        test   byte ptr [ebp + 6], 20h
        jne    SHORT loc_1_4FB1
        mov    ax, word ptr [esp + 10h]
        add    ax, word ptr [esp + 12h]
        mov    word ptr [ebx + 14h], ax
loc_1_4FB1:
        mov    al, byte ptr [ebp + 8]
        sub    al, 0feh
        sbb    cl, cl
        and    al, cl
        add    al, 0feh
        movzx  ecx, al
        inc    ecx
        movzx  eax, byte ptr [ebx + 0fh]
        cmp    ecx, eax
        jg     SHORT loc_1_4FCA
        mov    ecx, eax
loc_1_4FCA:
        mov    byte ptr [ebx + 0fh], cl
        mov    di, word ptr [esp + 10h]
        mov    ax, word ptr [esp + 12h]
        mov    cx, di
        lea    ebp, [eax + ecx - 1]
        lea    ecx, [ebx + 10h]
        mov    dword ptr [esp + 14h], ecx
        mov    esi, dword ptr [ebx + 10h]
        or     esi, esi
        je     SHORT loc_1_504B
loc_1_4FEC:
        cmp    word ptr [esi + 10h], di
        ja     SHORT loc_1_5010
        mov    ax, word ptr [esi + 12h]
        cmp    di, ax
        ja     SHORT loc_1_5003
        cmp    bp, ax
        jbe    SHORT loc_1_5048
        lea    edi, [eax + 1]
loc_1_5003:
        mov    dword ptr [esp + 14h], esi
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_4FEC
        jmp    SHORT loc_1_504B
        db  090h
loc_1_5010:
        cmp    word ptr [esi + 10h], bp
        ja     SHORT loc_1_504B
loc_1_5016:
        cmp    word ptr [esi + 12h], bp
        jae    SHORT loc_1_5034
        mov    eax, dword ptr [esi + 0ch]
        sub    word ptr [ebx + 16h], ax
        mov    eax, esi
        mov    esi, dword ptr [esi]
        push   eax
        call   sub_1_B944
        add    esp, 4
        jmp    SHORT loc_1_503A
        db  08Bh, 0FFh
loc_1_5034:
        mov    bp, word ptr [esi + 10h]
        dec    bp
loc_1_503A:
        or     esi, esi
        je     SHORT loc_1_504B
        cmp    word ptr [esi + 10h], bp
        jbe    SHORT loc_1_5016
        jmp    SHORT loc_1_504B
        db  08Bh, 0FFh
loc_1_5048:
        lea    ebp, [edi - 1]
loc_1_504B:
        mov    cx, bp
        sub    cx, di
        inc    cx
        mov    word ptr [esp + 20h], di
        mov    ax, di
        sub    ax, word ptr [esp + 10h]
        mov    word ptr [esp + 20h], ax
        movzx  eax, cx
        mov    dword ptr [esp + 24h], eax
        mov    edx, dword ptr [esp + 40h]
        mov    dword ptr [edx + 0ch], eax
        mov    word ptr [edx + 12h], bp
        mov    word ptr [edx + 10h], di
        mov    ax, word ptr [ebx + 16h]
        add    ax, cx
        movzx  ecx, word ptr [esp + 20h]
        mov    word ptr [ebx + 16h], ax
        add    dword ptr [edx + 8], ecx
        mov    dword ptr [edx], esi
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [ecx], edx
        cmp    word ptr [ebx + 14h], ax
        jne    NEAR PTR loc_1_5186
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [ecx], eax
        lea    eax, [ebx + 1ch]
        mov    dword ptr [esp + 18h], eax
        mov    al, byte ptr [eax + 8]
        mov    byte ptr [esp + 31h], al
        mov    eax, dword ptr [esp + 18h]
        mov    al, byte ptr [eax + 1]
        mov    byte ptr [esp + 32h], al
        mov    byte ptr [esp + 33h], 0
        cmp    dword ptr [ebx + 18h], 14h
        je     SHORT loc_1_50E4
        mov    eax, dword ptr [esp + 18h]
        add    eax, 14h
        mov    dword ptr [esp + 28h], eax
        mov    eax, dword ptr [ebx + 18h]
        sub    al, 14h
        mov    byte ptr [esp + 30h], al
        jmp    SHORT loc_1_50F1
        db  08Bh, 0FFh
loc_1_50E4:
        mov    dword ptr [esp + 28h], 0
        mov    byte ptr [esp + 30h], 0
loc_1_50F1:
        mov    ebp, dword ptr [ebx + 10h]
        cmp    dword ptr [ebp + 0ch], 0c8h
        jae    SHORT loc_1_5154
loc_1_50FD:
        mov    edx, dword ptr [ebp]
        or     edx, edx
        je     SHORT loc_1_5154
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, 0c8h
        sub    eax, edi
        mov    ecx, dword ptr [edx + 0ch]
        sub    eax, ecx
        sbb    esi, esi
        and    eax, esi
        add    eax, ecx
        mov    esi, dword ptr [edx + 8]
        mov    ecx, eax
        shr    ecx, 2
        add    edi, dword ptr [ebp + 8]
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        add    dword ptr [ebp + 0ch], eax
        add    dword ptr [edx + 8], eax
        mov    ecx, dword ptr [edx + 0ch]
        sub    ecx, eax
        mov    dword ptr [edx + 0ch], ecx
        jne    SHORT loc_1_514B
        mov    eax, dword ptr [edx]
        mov    dword ptr [ebp], eax
        push   edx
        call   sub_1_B944
        add    esp, 4
loc_1_514B:
        cmp    dword ptr [ebp + 0ch], 0c8h
        jb     SHORT loc_1_50FD
loc_1_5154:
        db  0FFh, 005h, 014h, 0C2h, 000h, 000h
        push   dword ptr [esp + 4ch]
        lea    eax, [esp + 2ch]
        push   eax
        movzx  eax, word ptr [ebx + 14h]
        push   eax
        push   ebp
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 4ch]
        push   dword ptr [esp + 54h]
        call   sub_1_4D70
        add    esp, 1ch
        push   ebx
        call   sub_1_4E1C
        add    esp, 4
loc_1_5186:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  08Bh, 0FFh
sub_1_5190:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        lea    edi, [esi + 38h]
        cmp    dword ptr [esp + 18h], 0
        jne    SHORT loc_1_51CA
        mov    ecx, dword ptr [edi + 10h]
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [ecx + 0ch], eax
        mov    al, byte ptr [edi + 14h]
        push   eax
        movzx  eax, byte ptr [edi + 15h]
        push   eax
        lea    eax, [edi + 18h]
        push   eax
        push   ecx
        push   dword ptr [esp + 20h]
        push   dword ptr [edi + 0ch]
        call   sub_1_4E48
        add    esp, 18h
loc_1_51CA:
        lea    eax, [esp + 8]
        push   eax
        push   esi
        call   sub_1_BA5C
        push   dword ptr [esp + 8]
        call   sub_1_BA3E
        and    byte ptr [edi + 5], 0fbh
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 0ch]
        mov    ecx, dword ptr [eax + 2ch]
        mov    dword ptr [edi], ecx
        mov    dword ptr [eax + 2ch], esi
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  090h
sub_1_51F8:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0FFh, 005h, 010h, 0C2h, 000h, 000h
        mov    eax, dword ptr [esp + 28h]
        movzx  eax, word ptr [eax + 6]
        test   eax, 0ffffff1fh
        mov    eax, dword ptr [esp + 38h]
        jne    SHORT loc_1_5228
        sub    eax, 0c8h
        sbb    ecx, ecx
        not    ecx
        and    eax, ecx
        add    eax, 0c8h
loc_1_5228:
        add    eax, 14h
        push   eax
        call   sub_1_B93E
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_1_53A4
        lea    edi, [ebx + 14h]
        mov    dword ptr [ebx + 8], edi
        mov    eax, dword ptr [esp + 38h]
        mov    dword ptr [ebx + 0ch], eax
        mov    dword ptr [ebx + 4], 0
        mov    eax, dword ptr [esp + 34h]
        cmp    dword ptr [esp + 38h], eax
        jbe    NEAR PTR loc_1_5368
        push   dword ptr [esp + 38h]
        push   edi
        db  0FFh, 035h, 0F4h, 0C2h, 000h, 000h
        lea    eax, [esp + 24h]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        call   sub_1_BA32
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_1_5298
        db  0FFh, 005h, 018h, 0C2h, 000h, 000h
        push   ebx
        call   sub_1_B944
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_1_5298:
        mov    eax, dword ptr [esp + 20h]
        mov    ebp, dword ptr [eax + 0ch]
        mov    eax, dword ptr [ebp + 2ch]
        mov    dword ptr [esp + 10h], eax
        or     eax, eax
        je     NEAR PTR loc_1_5350
        mov    edi, eax
        add    edi, 38h
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebp + 2ch], eax
        or     byte ptr [edi + 5], 4
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [edi + 0ch], eax
        mov    al, byte ptr [esp + 3ch]
        mov    byte ptr [edi + 14h], al
        mov    al, byte ptr [esp + 2ch]
        mov    byte ptr [edi + 15h], al
        mov    dword ptr [edi + 10h], ebx
        mov    eax, dword ptr [esp + 2ch]
        add    eax, 8
        add    edi, 18h
        mov    esi, dword ptr [esp + 28h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 14h]
        call   sub_1_BA50
        lea    eax, [esp + 38h]
        push   eax
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 40h]
        push   dword ptr [esp + 38h]
        push   dword ptr [esp + 54h]
        push   dword ptr [esp + 54h]
        push   dword ptr [ebp + 4]
        call   dword ptr [ebp + 0ch]
        add    esp, 1ch
        mov    dword ptr [esp + 14h], eax
        cmp    eax, 103h
        je     NEAR PTR loc_1_53AF
        push   dword ptr [esp + 38h]
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 2ch]
        call   sub_1_5190
        add    esp, 10h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Bh, 0FFh
loc_1_5350:
        push   ebx
        call   sub_1_B944
        add    esp, 4
        db  0FFh, 005h, 018h, 0C2h, 000h, 000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  090h
loc_1_5368:
        mov    eax, dword ptr [esp + 38h]
        mov    esi, dword ptr [esp + 30h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 30h]
        push   dword ptr [esp + 30h]
        push   ebx
        push   dword ptr [esp + 30h]
        push   dword ptr [esp + 38h]
        call   sub_1_4E48
        add    esp, 18h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  090h
loc_1_53A4:
        call   sub_1_0045
        db  0FFh, 005h, 018h, 0C2h, 000h, 000h
loc_1_53AF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  090h
sub_1_53B8:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 10h]
        mov    al, byte ptr [esi]
        and    eax, 0fh
        lea    edx, [eax*4]
        lea    eax, [esi + 14h]
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [ecx], eax
        sub    dl, 14h
        mov    byte ptr [ecx + 8], dl
        mov    byte ptr [esp + 4], 28h
        lea    eax, [esp + 4]
        push   eax
        push   ecx
        call   sub_1_4C40
        add    esp, 8
        cmp    al, 28h
        jae    SHORT loc_1_5418
        and    eax, 0ffh
        add    eax, 14h
        push   eax
        push   0
        push   0ch
        push   esi
        mov    eax, dword ptr [esp + 1ch]
        push   dword ptr [eax + 4]
        call   sub_1_9610
        add    esp, 14h
        mov    al, 0ffh
        pop    esi
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_5418:
        cmp    byte ptr [esp + 4], 28h
        je     SHORT loc_1_5430
        test   byte ptr [esp + 18h], 1
        jne    SHORT loc_1_5430
        mov    al, 6
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_5430:
        sub    al, al
        pop    esi
        add    esp, 4
        ret
        db  090h
sub_1_5438:
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 34h]
        add    esi, 38h
        cmp    dword ptr [esp + 38h], 0
        jne    NEAR PTR loc_1_54F8
        lea    edi, [esi + 18h]
        mov    al, byte ptr [edi + 8]
        mov    byte ptr [esp + 19h], al
        mov    al, byte ptr [edi + 1]
        mov    byte ptr [esp + 1ah], al
        movzx  eax, word ptr [edi + 6]
        shl    eax, 8
        shr    eax, 0dh
        and    al, 2
        mov    byte ptr [esp + 1bh], al
        cmp    byte ptr [esi + 15h], 14h
        je     SHORT loc_1_548C
        lea    eax, [edi + 14h]
        mov    dword ptr [esp + 10h], eax
        mov    al, byte ptr [esi + 15h]
        sub    al, 14h
        mov    byte ptr [esp + 18h], al
        jmp    SHORT loc_1_5499
        db  08Bh, 0FFh
loc_1_548C:
        mov    dword ptr [esp + 10h], 0
        mov    byte ptr [esp + 18h], 0
loc_1_5499:
        mov    bl, byte ptr [esi + 14h]
        mov    dword ptr [esp + 1ch], 0
        mov    dword ptr [esp + 20h], 0
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [esp + 24h], eax
        mov    ebp, dword ptr [esp + 3ch]
        mov    dword ptr [esp + 28h], ebp
        push   ebx
        lea    eax, [esp + 14h]
        push   eax
        push   ebp
        lea    eax, [esp + 28h]
        push   eax
        push   edi
        push   dword ptr [esi + 0ch]
        push   dword ptr [esp + 48h]
        call   sub_1_4D70
        add    esp, 1ch
        test   bl, 1
        je     SHORT loc_1_54F8
        push   ebx
        push   0
        push   0
        push   ebp
        push   dword ptr [esp + 34h]
        movzx  eax, byte ptr [esi + 15h]
        push   eax
        push   edi
        push   dword ptr [esp + 4ch]
        call   sub_1_89E0
        add    esp, 20h
loc_1_54F8:
        mov    eax, dword ptr [esp + 30h]
        mov    ecx, dword ptr [eax + 0ch]
        mov    eax, dword ptr [ecx + 2ch]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [ecx + 2ch], eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  090h
sub_1_5514:
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 4ch]
        mov    dword ptr [esp + 14h], esi
        sub    ebp, ebp
        db  0FFh, 005h, 0E0h, 0C1h, 000h, 000h
        mov    edi, dword ptr [esp + 50h]
        cmp    edi, ebp
        je     NEAR PTR loc_1_5830
        mov    al, byte ptr [esi]
        and    eax, 0fh
        lea    ebx, [eax*4]
        cmp    ebx, 14h
        jb     NEAR PTR loc_1_5830
        cmp    edi, ebx
        jb     NEAR PTR loc_1_5830
        push   ebx
        push   esi
        call   sub_1_5C30
        add    esp, 8
        cmp    ax, 0ffffh
        jne    NEAR PTR loc_1_5830
        mov    ax, word ptr [esi + 2]
        mov    cx, ax
        and    ecx, 0ffh
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0ffffh
        or     ecx, eax
        mov    dword ptr [esp + 18h], ecx
        mov    al, byte ptr [esi]
        and    al, 0f0h
        cmp    al, 40h
        jne    NEAR PTR loc_1_5830
        cmp    ecx, 14h
        jbe    NEAR PTR loc_1_5830
        mov    eax, dword ptr [esp + 54h]
        cmp    eax, ecx
        jb     NEAR PTR loc_1_5830
        sub    dword ptr [esp + 18h], ebx
        add    esi, ebx
        sub    edi, ebx
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [eax + 10h]
        mov    eax, dword ptr [esp + 48h]
        mov    dword ptr [esp + 24h], eax
        lea    eax, [esp + 24h]
        push   eax
        push   ecx
        call   sub_1_714C
        add    esp, 8
        mov    byte ptr [esp + 13h], al
        cmp    dword ptr [esp + 60h], ebp
        je     SHORT loc_1_55EC
        test   al, 1
        jne    SHORT loc_1_55EC
        db  0FFh, 005h, 0E4h, 0C1h, 000h, 000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  08Dh, 049h, 000h
loc_1_55EC:
        mov    eax, dword ptr [esp + 14h]
        mov    al, byte ptr [eax + 8]
        mov    byte ptr [esp + 31h], al
        mov    eax, dword ptr [esp + 14h]
        mov    al, byte ptr [eax + 1]
        mov    byte ptr [esp + 32h], al
        mov    eax, dword ptr [esp + 14h]
        movzx  eax, word ptr [eax + 6]
        shl    eax, 8
        shr    eax, 0dh
        and    al, 2
        mov    byte ptr [esp + 33h], al
        mov    dword ptr [esp + 28h], 0
        mov    byte ptr [esp + 30h], 0
        cmp    byte ptr [esp + 13h], 6
        jae    NEAR PTR loc_1_57F0
        cmp    ebx, 14h
        je     SHORT loc_1_5668
        push   dword ptr [esp + 13h]
        lea    eax, [esp + 2ch]
        push   eax
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 54h]
        call   sub_1_53B8
        add    esp, 10h
        or     al, al
        je     SHORT loc_1_5668
        cmp    al, 6
        je     NEAR PTR loc_1_57F0
        db  0FFh, 005h, 0E4h, 0C1h, 000h, 000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  08Bh, 0FFh
loc_1_5668:
        mov    eax, dword ptr [esp + 14h]
        movzx  eax, word ptr [eax + 6]
        test   eax, 0ffffff3fh
        jne    NEAR PTR loc_1_57C0
        cmp    dword ptr [esp + 18h], edi
        jbe    NEAR PTR loc_1_5738
        mov    eax, dword ptr [esp + 48h]
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [esp + 1ch], ecx
        mov    eax, dword ptr [ecx + 2ch]
        mov    dword ptr [esp + 20h], eax
        or     eax, eax
        je     NEAR PTR loc_1_5728
        mov    ebp, eax
        add    ebp, 38h
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ecx + 2ch], eax
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [ebp + 0ch], eax
        mov    al, byte ptr [esp + 13h]
        mov    byte ptr [ebp + 14h], al
        mov    byte ptr [ebp + 15h], bl
        lea    eax, [ebx + 8]
        lea    edi, [ebp + 18h]
        mov    esi, dword ptr [esp + 14h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    eax, [esp + 18h]
        push   eax
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 20h]
        push   ebx
        push   dword ptr [esp + 6ch]
        push   dword ptr [esp + 6ch]
        mov    ecx, dword ptr [esp + 34h]
        push   dword ptr [ecx + 4]
        call   dword ptr [ecx + 0ch]
        add    esp, 1ch
        mov    esi, dword ptr [ebp + 8]
        cmp    eax, 103h
        je     NEAR PTR loc_1_5836
        or     eax, eax
        je     SHORT loc_1_5738
        db  0FFh, 005h, 0F4h, 0C1h, 000h, 000h
        mov    ecx, dword ptr [esp + 1ch]
        mov    eax, dword ptr [ecx + 2ch]
        mov    dword ptr [ebp], eax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [ecx + 2ch], eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  090h
loc_1_5728:
        pop    ebp
        db  0FFh, 005h, 0F4h, 0C1h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  08Bh, 0FFh
loc_1_5738:
        mov    dword ptr [esp + 3ch], esi
        mov    dword ptr [esp + 34h], 0
        mov    dword ptr [esp + 38h], 0
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esp + 40h], eax
        push   dword ptr [esp + 13h]
        lea    eax, [esp + 2ch]
        push   eax
        push   dword ptr [esp + 20h]
        lea    eax, [esp + 40h]
        push   eax
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 38h]
        push   dword ptr [esp + 60h]
        call   sub_1_4D70
        add    esp, 1ch
        test   byte ptr [esp + 13h], 1
        je     SHORT loc_1_579F
        push   dword ptr [esp + 13h]
        push   0
        push   0
        push   dword ptr [esp + 24h]
        push   esi
        push   ebx
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 64h]
        call   sub_1_89E0
        add    esp, 20h
loc_1_579F:
        or     ebp, ebp
        je     NEAR PTR loc_1_5836
        mov    ecx, dword ptr [esp + 1ch]
        mov    eax, dword ptr [ecx + 2ch]
        mov    dword ptr [ebp], eax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [ecx + 2ch], eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
loc_1_57C0:
        push   dword ptr [esp + 5ch]
        push   dword ptr [esp + 5ch]
        push   dword ptr [esp + 1bh]
        push   dword ptr [esp + 24h]
        push   edi
        push   esi
        push   ebx
        push   dword ptr [esp + 30h]
        push   dword ptr [esp + 44h]
        push   dword ptr [esp + 6ch]
        call   sub_1_51F8
        add    esp, 28h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  090h
loc_1_57F0:
        cmp    byte ptr [esp + 13h], 0ffh
        je     SHORT loc_1_5820
        push   dword ptr [esp + 13h]
        push   dword ptr [esp + 60h]
        push   dword ptr [esp + 60h]
        push   edi
        push   esi
        push   ebx
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 64h]
        call   sub_1_89E0
        add    esp, 20h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  08Bh, 0FFh
loc_1_5820:
        pop    ebp
        db  0FFh, 005h, 0E8h, 0C1h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  08Bh, 0FFh
loc_1_5830:
        db  0FFh, 005h, 0E4h, 0C1h, 000h, 000h
loc_1_5836:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  08Bh, 0FFh
sub_1_5840:
        mov    ecx, dword ptr [esp + 8]
        lea    eax, [ecx + 38h]
        mov    al, byte ptr [eax + 5]
        test   al, 8
        jne    SHORT loc_1_5874
        test   al, 4
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   ecx
        push   dword ptr [esp + 10h]
        jne    SHORT loc_1_5868
        call   sub_1_5438
        add    esp, 10h
        ret
loc_1_5868:
        call   sub_1_5190
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_1_5874:
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   ecx
        call   sub_1_8710
        add    esp, 0ch
        ret
        db  08Bh, 0FFh
sub_1_5888:
        db  0A1h, 0E0h, 0BCh, 000h, 000h
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        db  089h, 00Dh, 0E0h, 0BCh, 000h, 000h
        ret
        db  08Bh, 0FFh
sub_1_589C:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        or     esi, esi
        je     SHORT loc_1_58C1
loc_1_58A8:
        lea    eax, [esp + 4]
        push   eax
        push   esi
        call   sub_1_BA68
        push   esi
        call   sub_1_BA3E
        mov    esi, dword ptr [esp + 4]
        or     esi, esi
        jne    SHORT loc_1_58A8
loc_1_58C1:
        pop    esi
        add    esp, 4
        ret
        db  08Bh, 0FFh
sub_1_58C8:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 20h]
        lea    edi, [esi + 38h]
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [esp + 10h], eax
        mov    al, byte ptr [edi + 5]
        test   al, 18h
        jne    SHORT loc_1_5904
        push   esi
        call   sub_1_BA14
        db  0A1h, 0ECh, 0C2h, 000h, 000h
        mov    dword ptr [edi], eax
        db  089h, 035h, 0ECh, 0C2h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  090h
loc_1_5904:
        and    al, 0efh
        mov    byte ptr [edi + 5], al
        mov    ebx, dword ptr [esp + 10h]
        mov    eax, ebx
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 10h], eax
        test   byte ptr [edi + 5], 1
        je     SHORT loc_1_5957
        mov    ebp, eax
        lea    eax, [esp + 10h]
        push   eax
        push   dword ptr [esp + 14h]
        call   sub_1_BA68
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   ebp
        call   sub_1_BA44
        test   byte ptr [edi + 5], 8
        jne    SHORT loc_1_594D
        push   dword ptr [esp + 14h]
        call   sub_1_B944
        add    esp, 4
loc_1_594D:
        push   ebp
        call   sub_1_BA3E
        and    byte ptr [edi + 5], 0feh
loc_1_5957:
        test   byte ptr [edi + 5], 2
        je     SHORT loc_1_5975
        push   dword ptr [esp + 10h]
        call   sub_1_589C
        add    esp, 4
        mov    dword ptr [esp + 10h], 0
        and    byte ptr [edi + 5], 0fdh
loc_1_5975:
        test   byte ptr [edi + 5], 8
        jne    SHORT loc_1_5997
        push   ebx
        call   sub_1_5888
        add    esp, 4
        push   esi
        call   sub_1_BA14
        db  0A1h, 0ECh, 0C2h, 000h, 000h
        mov    dword ptr [edi], eax
        db  089h, 035h, 0ECh, 0C2h, 000h, 000h
loc_1_5997:
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  090h
sub_1_59A4:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0A1h, 0ECh, 0BCh, 000h, 000h
        db  039h, 005h, 0F0h, 0BCh, 000h, 000h
        jbe    SHORT loc_1_59EA
        push   0ch
        call   sub_1_B93E
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_59EA
        lea    ebx, [esi + 4]
        push   14h
        push   10h
        push   ebx
        lea    eax, [esp + 20h]
        push   eax
        call   sub_1_BA02
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_1_59F4
        push   esi
        call   sub_1_B944
        add    esp, 4
loc_1_59EA:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_59F4:
        sub    ebp, ebp
        db  0A1h, 0E4h, 0BCh, 000h, 000h
        mov    dword ptr [esi], eax
        db  089h, 035h, 0E4h, 0BCh, 000h, 000h
        db  083h, 005h, 0ECh, 0BCh, 000h, 000h, 010h
        sub    esi, esi
loc_1_5A0C:
        push   dword ptr [ebx]
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_1_BA0E
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_1_5A56
        mov    edi, dword ptr [esp + 10h]
        add    edi, 38h
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        stosd
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [eax + 3ch], 1
        or     esi, esi
        je     SHORT loc_1_5A4C
        push   dword ptr [esp + 10h]
        call   sub_1_58C8
        add    esp, 4
        jmp    SHORT loc_1_5A50
loc_1_5A4C:
        mov    ebp, dword ptr [esp + 10h]
loc_1_5A50:
        inc    esi
        cmp    esi, 10h
        jb     SHORT loc_1_5A0C
loc_1_5A56:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_5A60:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0A1h, 0F8h, 0BCh, 000h, 000h
        db  03Bh, 005h, 0F4h, 0BCh, 000h, 000h
        jbe    SHORT loc_1_5AA4
        push   0ch
        call   sub_1_B93E
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_5AA4
        lea    edi, [esi + 4]
        push   10h
        push   edi
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_1_BA26
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_1_5AB0
        push   esi
        call   sub_1_B944
        add    esp, 4
loc_1_5AA4:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_5AB0:
        push   140h
        call   sub_1_B93E
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_5AE0
        push   dword ptr [edi]
        call   sub_1_BA2C
        push   esi
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_5AE0:
        mov    dword ptr [esi + 8], ebx
        db  0A1h, 0E8h, 0BCh, 000h, 000h
        mov    dword ptr [esi], eax
        db  089h, 035h, 0E8h, 0BCh, 000h, 000h
        sub    esi, esi
        db  083h, 005h, 0F4h, 0BCh, 000h, 000h, 010h
        sub    ebp, ebp
loc_1_5AFC:
        push   14h
        push   ebx
        push   dword ptr [edi]
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        call   sub_1_BA32
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_1_5B39
        or     ebp, ebp
        je     SHORT loc_1_5B2C
        push   dword ptr [esp + 10h]
        call   sub_1_5888
        add    esp, 4
        jmp    SHORT loc_1_5B30
        db  08Dh, 049h, 000h
loc_1_5B2C:
        mov    esi, dword ptr [esp + 10h]
loc_1_5B30:
        inc    ebp
        add    ebx, 14h
        cmp    ebp, 10h
        jb     SHORT loc_1_5AFC
loc_1_5B39:
        mov    eax, esi
        sub    ebp, 10h
        db  001h, 02Dh, 0F4h, 0BCh, 000h, 000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_5B4C:
        db  0A1h, 0ECh, 0C2h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_5B60
        mov    ecx, dword ptr [eax + 38h]
        db  089h, 00Dh, 0ECh, 0C2h, 000h, 000h
        ret
        db  090h
loc_1_5B60:
        jmp    NEAR PTR sub_1_59A4
        db  08Dh, 049h, 000h
sub_1_5B68:
        db  0A1h, 0E0h, 0BCh, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_5B80
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 0E0h, 0BCh, 000h, 000h
        mov    dword ptr [eax], 0
        ret
loc_1_5B80:
        jmp    NEAR PTR loc_1_5A60
        db  08Dh, 049h, 000h
sub_1_5B88:
        push   esi
        push   edi
        call   sub_1_5B4C
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_5BC1
        call   sub_1_5B68
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_5BB8
        push   esi
        push   edi
        call   sub_1_BA56
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], edi
        or     byte ptr [edi + 3dh], 10h
        mov    eax, dword ptr [esi + 4]
        pop    edi
        pop    esi
        ret
        db  090h
loc_1_5BB8:
        push   edi
        call   sub_1_58C8
        add    esp, 4
loc_1_5BC1:
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
sub_1_5BC8:
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        add    eax, dword ptr [edx + 4]
        mov    dword ptr [edx + 4], eax
        ret
        db  090h
sub_1_5BD8:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        lea    ecx, [eax + 38h]
        mov    edx, dword ptr [ecx + 0ch]
        mov    esi, dword ptr [edx]
        mov    edi, dword ptr [ecx + 10h]
        mov    ebx, dword ptr [ecx + 8]
        push   eax
        call   sub_1_58C8
        add    esp, 4
        or     ebx, ebx
        jne    SHORT loc_1_5C08
        push   eax
        push   edi
        call   esi
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_5C08:
        push   -1
        push   ebx
        call   sub_1_5BC8
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_5C29
        mov    ebp, dword ptr [ebx]
        push   ebx
        call   sub_1_B944
        add    esp, 4
        push   ebp
        push   edi
        call   esi
        add    esp, 8
loc_1_5C29:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
sub_1_5C30:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        sub    esi, esi
        mov    ecx, dword ptr [esp + 10h]
        cmp    ecx, 1
        jle    SHORT loc_1_5C59
        mov    edx, ecx
        shr    edx, 1
        lea    eax, [edx*2]
        sub    ecx, eax
loc_1_5C4E:
        movzx  eax, word ptr [edi]
        add    esi, eax
        add    edi, 2
        dec    edx
        jne    SHORT loc_1_5C4E
loc_1_5C59:
        or     ecx, ecx
        je     SHORT loc_1_5C62
        movzx  eax, byte ptr [edi]
        add    esi, eax
loc_1_5C62:
        mov    eax, esi
        shr    eax, 10h
        and    esi, 0ffffh
        add    eax, esi
        mov    ecx, eax
        shr    ecx, 10h
        add    eax, ecx
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_5C7C:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   14h
        push   dword ptr [esp + 30h]
        call   sub_1_5C30
        add    esp, 8
        movzx  edi, ax
        mov    ebx, dword ptr [esp + 30h]
        or     ebx, ebx
        mov    esi, dword ptr [esp + 24h]
        je     SHORT loc_1_5D1A
        lea    ebp, [esi + 38h]
        push   dword ptr [esp + 34h]
        push   ebx
        db  0FFh, 035h, 0F4h, 0C2h, 000h, 000h
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        call   sub_1_BA32
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_5CE4
        push   ebx
        call   sub_1_B944
        add    esp, 4
        push   esi
        call   sub_1_58C8
        add    esp, 4
        mov    eax, 2afeh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
loc_1_5CE4:
        or     byte ptr [ebp + 5], 1
        push   dword ptr [esp + 14h]
        push   esi
        call   sub_1_BA56
        push   dword ptr [esp + 34h]
        push   ebx
        call   sub_1_5C30
        add    esp, 8
        and    eax, 0ffffh
        add    edi, eax
        mov    eax, edi
        shr    eax, 10h
        and    edi, 0ffffh
        add    edi, eax
        mov    eax, edi
        shr    eax, 10h
        add    edi, eax
loc_1_5D1A:
        not    di
        push   dword ptr [esp + 28h]
        mov    eax, dword ptr [esp + 30h]
        mov    word ptr [eax + 0ah], di
        push   esi
        call   sub_1_BA56
        push   0
        push   dword ptr [esp + 24h]
        push   esi
        mov    eax, dword ptr [esp + 28h]
        push   dword ptr [eax + 4]
        call   dword ptr [eax + 8]
        add    esp, 10h
        mov    dword ptr [esp + 10h], eax
        cmp    eax, 103h
        jne    SHORT loc_1_5D5C
        mov    eax, 2bf7h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
loc_1_5D5C:
        push   esi
        call   sub_1_58C8
        add    esp, 4
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_5D78
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_5D78:
        pop    ebp
        mov    eax, 2b00h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
sub_1_5D88:
        db  0A1h, 004h, 0BCh, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_5DC0
        mov    cx, word ptr [eax + 14h]
        and    cl, 9
        cmp    cl, 8
        jne    SHORT loc_1_5DC0
        push   0
        push   0
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   dword ptr [eax + 0ch]
        call   sub_1_5C7C
        add    esp, 1ch
        ret
        db  08Dh, 049h, 000h
loc_1_5DC0:
        push   dword ptr [esp + 8]
        call   sub_1_58C8
        add    esp, 4
        db  0FFh, 005h, 004h, 0C2h, 000h, 000h
        mov    eax, 2afbh
        ret
sub_1_5DD8:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 1ch]
        inc    ecx
        mov    ecx, dword ptr [esp + 20h]
loc_1_5DE8:
        mov    eax, dword ptr [edi + 0ch]
        cmp    ecx, eax
        jb     SHORT loc_1_5E04
        sub    ecx, eax
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    SHORT loc_1_5DE8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_5E04:
        lea    ebx, [esp + 14h]
        mov    dword ptr [esp + 14h], 0
        mov    esi, dword ptr [esp + 24h]
loc_1_5E14:
        mov    eax, dword ptr [edi + 0ch]
        sub    eax, ecx
        sub    eax, esi
        sbb    edx, edx
        and    eax, edx
        lea    ebp, [eax + esi]
        push   ebp
        mov    eax, dword ptr [edi + 4]
        add    eax, ecx
        push   eax
        db  0FFh, 035h, 0F4h, 0C2h, 000h, 000h
        push   ebx
        lea    eax, [esp + 20h]
        push   eax
        call   sub_1_BA32
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_5E4F
        sub    ecx, ecx
        mov    ebx, dword ptr [ebx]
        mov    edi, dword ptr [edi]
        sub    esi, ebp
        je     SHORT loc_1_5E4F
        cmp    edi, ecx
        jne    SHORT loc_1_5E14
loc_1_5E4F:
        or     esi, esi
        jne    SHORT loc_1_5E60
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
loc_1_5E60:
        mov    esi, dword ptr [esp + 14h]
        or     esi, esi
        je     SHORT loc_1_5E76
loc_1_5E68:
        mov    eax, esi
        mov    esi, dword ptr [esi]
        push   eax
        call   sub_1_BA3E
        or     esi, esi
        jne    SHORT loc_1_5E68
loc_1_5E76:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_5E80:
        sub    esp, 48h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 68h]
        add    eax, 38h
        mov    dword ptr [esp + 28h], eax
        mov    dword ptr [esp + 24h], 0
        mov    word ptr [esp + 14h], 0
        sub    ebx, ebx
        mov    byte ptr [esp + 13h], bl
        mov    ebp, dword ptr [esp + 60h]
        sub    ebp, dword ptr [esp + 7ch]
        and    ebp, 0fffffff8h
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esp + 2ch], eax
        mov    ecx, dword ptr [esp + 6ch]
        mov    ax, word ptr [ecx + 6]
        test   al, 40h
        je     SHORT loc_1_5F0C
        push   dword ptr [esp + 68h]
        call   sub_1_58C8
        add    esp, 4
        cmp    dword ptr [esp + 78h], ebx
        je     SHORT loc_1_5EE3
        push   dword ptr [esp + 78h]
        call   sub_1_B944
        add    esp, 4
loc_1_5EE3:
        cmp    dword ptr [esp + 80h], 0
        jne    SHORT loc_1_5EF9
        push   dword ptr [esp + 2ch]
        call   sub_1_B944
        add    esp, 4
loc_1_5EF9:
        pop    ebp
        db  0FFh, 005h, 020h, 0C2h, 000h, 000h
        mov    eax, 2b01h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 48h
        ret
loc_1_5F0C:
        and    ax, 0ff1fh
        mov    ch, al
        mov    cl, ah
        lea    eax, [ecx*8]
        cmp    dword ptr [esp + 78h], 0
        mov    word ptr [esp + 16h], ax
        je     SHORT loc_1_5F82
        mov    edx, dword ptr [esp + 78h]
        lea    edi, [esp + 30h]
        sub    eax, eax
        mov    ecx, 0ah
        rep stosd
        mov    eax, dword ptr [esp + 78h]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_5F77
loc_1_5F41:
        mov    al, byte ptr [edx]
        test   al, 80h
        je     SHORT loc_1_5F64
        movzx  eax, byte ptr [edx + 1]
        lea    edi, [esp + ebx + 30h]
        mov    esi, edx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        add    ebx, eax
        jmp    SHORT loc_1_5F70
        db  090h
loc_1_5F64:
        cmp    al, 1
        jne    SHORT loc_1_5F6C
        inc    edx
        jmp    SHORT loc_1_5F72
        db  090h
loc_1_5F6C:
        movzx  eax, byte ptr [edx + 1]
loc_1_5F70:
        add    edx, eax
loc_1_5F72:
        cmp    byte ptr [edx], 0
        jne    SHORT loc_1_5F41
loc_1_5F77:
        test   bl, 3
        je     SHORT loc_1_5F82
        and    ebx, 0fffffffch
        add    ebx, 4
loc_1_5F82:
        mov    eax, dword ptr [esp + 28h]
        or     byte ptr [eax + 5], 2
loc_1_5F8A:
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_1_5B88
        add    esp, 4
        mov    dword ptr [esp + 18h], eax
        or     eax, eax
        je     NEAR PTR loc_1_60CC
        movzx  eax, word ptr [esp + 14h]
        mov    dword ptr [esp + 20h], eax
        push   ebp
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 78h]
        call   sub_1_5DD8
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_1_60C0
        mov    edx, dword ptr [esp + 1ch]
        add    edx, 38h
        mov    edi, edx
        mov    esi, dword ptr [esp + 28h]
        mov    ecx, 5
        rep movsd
        mov    edi, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 6ch]
        mov    ecx, 5
        rep movsd
        and    byte ptr [edx + 5], 0f7h
        mov    ecx, dword ptr [esp + 7ch]
        add    ecx, 14h
        shr    ecx, 2
        add    cl, 40h
        mov    esi, dword ptr [esp + 18h]
        mov    byte ptr [esi], cl
        mov    ecx, dword ptr [esp + 7ch]
        add    ecx, ebp
        add    ecx, 14h
        mov    cl, ch
        mov    dx, word ptr [esp + 7ch]
        add    dx, bp
        add    dx, 14h
        mov    ch, dl
        mov    word ptr [esi + 2], cx
        movzx  ecx, word ptr [esp + 16h]
        add    ecx, dword ptr [esp + 20h]
        sar    ecx, 3
        push   dword ptr [esp + 7ch]
        mov    dx, cx
        push   dword ptr [esp + 7ch]
        or     dx, 2000h
        mov    dl, dh
        mov    dh, cl
        push   dword ptr [esp + 20h]
        mov    word ptr [esi + 6], dx
        push   eax
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 78h]
        push   dword ptr [esp + 74h]
        call   sub_1_5C7C
        add    esp, 1ch
        cmp    eax, 2bf7h
        jne    SHORT loc_1_6064
        inc    dword ptr [esp + 24h]
loc_1_6064:
        db  0FFh, 005h, 024h, 0C2h, 000h, 000h
        add    word ptr [esp + 14h], bp
        sub    dword ptr [esp + 74h], ebp
        or     ebx, ebx
        je     SHORT loc_1_60A4
        mov    dword ptr [esp + 7ch], ebx
        push   ebx
        call   sub_1_B93E
        add    esp, 4
        mov    dword ptr [esp + 78h], eax
        or     eax, eax
        je     SHORT loc_1_60CC
        lea    esi, [esp + 30h]
        mov    edi, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        jmp    SHORT loc_1_60B4
        db  08Bh, 0FFh
loc_1_60A4:
        mov    dword ptr [esp + 78h], 0
        mov    dword ptr [esp + 7ch], 0
loc_1_60B4:
        cmp    dword ptr [esp + 74h], ebp
        ja     NEAR PTR loc_1_5F8A
        jmp    SHORT loc_1_60D1
loc_1_60C0:
        push   dword ptr [esp + 1ch]
        call   sub_1_58C8
        add    esp, 4
loc_1_60CC:
        mov    byte ptr [esp + 13h], 1
loc_1_60D1:
        cmp    byte ptr [esp + 13h], 0
        jne    NEAR PTR loc_1_61B0
        movzx  esi, word ptr [esp + 14h]
        push   dword ptr [esp + 74h]
        push   esi
        push   dword ptr [esp + 78h]
        call   sub_1_5DD8
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_6118
        cmp    dword ptr [esp + 78h], 0
        je     SHORT loc_1_6109
        push   dword ptr [esp + 78h]
        call   sub_1_B944
        add    esp, 4
loc_1_6109:
        push   dword ptr [esp + 68h]
        call   sub_1_58C8
        jmp    NEAR PTR loc_1_61CC
        db  090h
loc_1_6118:
        mov    edx, dword ptr [esp + 7ch]
        add    edx, 14h
        shr    edx, 2
        add    dl, 40h
        mov    ecx, dword ptr [esp + 6ch]
        mov    byte ptr [ecx], dl
        mov    edx, dword ptr [esp + 74h]
        add    edx, dword ptr [esp + 7ch]
        add    edx, 14h
        mov    dl, dh
        mov    ecx, dword ptr [esp + 74h]
        add    cx, word ptr [esp + 7ch]
        add    cx, 14h
        mov    dh, cl
        mov    ecx, dword ptr [esp + 6ch]
        mov    word ptr [ecx + 2], dx
        movzx  edx, word ptr [esp + 16h]
        add    edx, esi
        sar    edx, 3
        mov    bx, word ptr [ecx + 6]
        and    bx, 20h
        push   dword ptr [esp + 7ch]
        mov    cx, dx
        push   dword ptr [esp + 7ch]
        shr    cx, 8
        push   dword ptr [esp + 74h]
        or     bx, cx
        mov    bh, dl
        mov    ecx, dword ptr [esp + 78h]
        mov    word ptr [ecx + 6], bx
        push   eax
        push   dword ptr [esp + 78h]
        push   dword ptr [esp + 78h]
        push   dword ptr [esp + 74h]
        call   sub_1_5C7C
        add    esp, 1ch
        cmp    eax, 2bf7h
        jne    SHORT loc_1_61A1
        inc    dword ptr [esp + 24h]
loc_1_61A1:
        db  0FFh, 005h, 024h, 0C2h, 000h, 000h
        db  0FFh, 005h, 01Ch, 0C2h, 000h, 000h
        jmp    SHORT loc_1_61D5
        db  090h
loc_1_61B0:
        push   dword ptr [esp + 68h]
        call   sub_1_58C8
        add    esp, 4
        cmp    dword ptr [esp + 78h], 0
        je     SHORT loc_1_61CF
        push   dword ptr [esp + 78h]
        call   sub_1_B944
loc_1_61CC:
        add    esp, 4
loc_1_61CF:
        db  0FFh, 005h, 004h, 0C2h, 000h, 000h
loc_1_61D5:
        mov    ecx, dword ptr [esp + 80h]
        or     ecx, ecx
        jne    SHORT loc_1_621C
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 30h]
        call   sub_1_5BC8
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_620C
        push   dword ptr [esp + 2ch]
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 48h
        ret
        db  08Bh, 0FFh
loc_1_620C:
        pop    ebp
        mov    eax, 2bf7h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 48h
        ret
        db  08Dh, 049h, 000h
loc_1_621C:
        mov    eax, dword ptr [esp + 24h]
        add    dword ptr [ecx], eax
        mov    eax, 2bf7h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 48h
        ret
        db  090h
sub_1_6230:
        push   ebx
        mov    ecx, dword ptr [esp + 8]
        mov    al, byte ptr [ecx + 2]
        dec    al
        mov    bl, byte ptr [ecx + 1]
        cmp    al, bl
        jae    SHORT loc_1_625F
        movzx  edx, al
        movzx  eax, bl
        sub    eax, edx
        cmp    eax, 4
        jae    SHORT loc_1_6254
        sub    al, al
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_6254:
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [edx + ecx], eax
        add    byte ptr [ecx + 2], 4
loc_1_625F:
        pop    ebx
        mov    al, 1
        ret
        db  090h
sub_1_6264:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        or     esi, esi
        je     NEAR PTR loc_1_6367
        mov    edi, dword ptr [esp + 1ch]
        or     edi, edi
        je     NEAR PTR loc_1_6367
        mov    bl, byte ptr [edi]
        cmp    bl, 28h
        mov    ebp, dword ptr [esp + 20h]
        je     SHORT loc_1_62AC
        push   ebp
        movzx  eax, bl
        add    eax, esi
        push   eax
        call   sub_1_6230
        add    esp, 8
        or     al, al
        jne    SHORT loc_1_62AC
        mov    al, bl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_62AC:
        mov    bl, byte ptr [edi + 1]
        cmp    bl, 28h
        je     SHORT loc_1_62D4
        push   ebp
        movzx  eax, bl
        add    eax, esi
        push   eax
        call   sub_1_6230
        add    esp, 8
        or     al, al
        jne    SHORT loc_1_62D4
        mov    al, bl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_62D4:
        mov    bl, byte ptr [edi + 2]
        cmp    bl, 28h
        je     NEAR PTR loc_1_6367
        movzx  eax, bl
        add    esi, eax
        mov    cl, byte ptr [esi + 2]
        dec    cl
        mov    byte ptr [esp + 12h], cl
        mov    al, byte ptr [esi + 3]
        mov    cl, al
        and    cl, 0fh
        mov    byte ptr [esp + 13h], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [esp + 11h], cl
        mov    cl, byte ptr [esp + 12h]
        cmp    cl, byte ptr [esp + 11h]
        jae    NEAR PTR loc_1_63CC
        call   sub_1_B92C
        db  003h, 005h, 0D0h, 0C1h, 000h, 000h
        db  00Bh, 005h, 094h, 0C0h, 000h, 000h
        mov    ecx, 5265c00h
        sub    edx, edx
        div    ecx
        mov    edi, edx
        shr    edi, 10h
        mov    eax, edx
        and    eax, 0ff0000h
        or     edi, eax
        shr    edi, 8
        mov    eax, edx
        shl    eax, 10h
        and    edx, 0ff00h
        or     eax, edx
        shl    eax, 8
        or     edi, eax
        movzx  eax, byte ptr [esp + 13h]
        movzx  edx, byte ptr [esp + 12h]
        lea    ecx, [edx + esi]
        or     eax, eax
        je     SHORT loc_1_6374
        cmp    eax, 1
        je     SHORT loc_1_63A0
        cmp    eax, 3
        je     SHORT loc_1_639C
loc_1_6367:
        pop    ebp
        mov    al, 28h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_6374:
        movzx  eax, byte ptr [esp + 11h]
        sub    eax, edx
        cmp    eax, 4
        jae    SHORT loc_1_638C
        mov    al, bl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_638C:
        mov    dword ptr [ecx], edi
        add    byte ptr [esi + 2], 4
        mov    al, 28h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_639C:
        cmp    dword ptr [ecx], ebp
        jne    SHORT loc_1_6367
loc_1_63A0:
        movzx  eax, byte ptr [esp + 11h]
        sub    eax, edx
        cmp    eax, 8
        jae    SHORT loc_1_63B8
        mov    al, bl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_63B8:
        mov    dword ptr [ecx], ebp
        mov    dword ptr [ecx + 4], edi
        add    byte ptr [esi + 2], 8
        mov    al, 28h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_63CC:
        mov    cl, al
        and    cl, 0f0h
        cmp    cl, 0f0h
        je     SHORT loc_1_63E8
        add    al, 10h
        mov    byte ptr [esi + 3], al
        mov    al, 28h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_63E8:
        mov    al, bl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
sub_1_63F4:
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 50h]
        add    eax, 38h
        mov    dword ptr [esp + 3ch], eax
        mov    dword ptr [esp + 30h], 0
        db  0A1h, 054h, 0C1h, 000h, 000h
        lea    eax, [eax + eax*2]
        shl    eax, 2
        push   eax
        call   sub_1_B93E
        add    esp, 4
        mov    dword ptr [esp + 20h], eax
        or     eax, eax
        jne    SHORT loc_1_6438
        mov    eax, 2afeh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  090h
loc_1_6438:
        db  0A1h, 054h, 0C1h, 000h, 000h
        lea    edx, [eax + eax*2]
        shl    edx, 2
        mov    edi, dword ptr [esp + 20h]
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        cmp    byte ptr [esp + 68h], al
        je     SHORT loc_1_6468
        mov    dword ptr [esp + 40h], eax
        sub    edi, edi
        jmp    SHORT loc_1_6479
        db  08Dh, 049h, 000h
loc_1_6468:
        mov    eax, dword ptr [esp + 48h]
        mov    ecx, dword ptr [eax + 4]
        and    ecx, dword ptr [eax + 8]
        mov    dword ptr [esp + 40h], ecx
        mov    edi, dword ptr [eax + 0ch]
loc_1_6479:
        db  08Bh, 035h, 090h, 0C0h, 000h, 000h
        mov    dword ptr [esp + 34h], 0
        mov    dword ptr [esp + 10h], 0
        or     esi, esi
        je     NEAR PTR loc_1_657B
        nop
loc_1_6498:
        test   byte ptr [esi + 14h], 1
        je     NEAR PTR loc_1_6571
        db  03Bh, 035h, 050h, 0C0h, 000h, 000h
        je     NEAR PTR loc_1_6571
        mov    ebx, dword ptr [esi + 4]
        and    ebx, dword ptr [esi + 8]
        cmp    dword ptr [esp + 40h], ebx
        je     NEAR PTR loc_1_6571
        cmp    dword ptr [esi + 0ch], edi
        je     NEAR PTR loc_1_6571
        push   esi
        push   dword ptr [esp + 50h]
        call   sub_1_6FA4
        add    esp, 8
        test   al, 1
        je     NEAR PTR loc_1_6571
        sub    ecx, ecx
        cmp    ecx, dword ptr [esp + 10h]
        je     SHORT loc_1_653D
        mov    eax, dword ptr [esp + 20h]
        inc    edx
        mov    edx, dword ptr [esi + 8]
loc_1_64EC:
        mov    ebp, dword ptr [eax]
        and    ebp, edx
        cmp    ebp, ebx
        je     SHORT loc_1_6508
        mov    ebp, dword ptr [esi + 0ch]
        cmp    dword ptr [eax + 4], ebp
        je     SHORT loc_1_6508
        add    eax, 0ch
        inc    ecx
        cmp    ecx, dword ptr [esp + 10h]
        jb     SHORT loc_1_64EC
        jmp    SHORT loc_1_6537
loc_1_6508:
        lea    eax, [ecx + ecx*2]
        mov    edx, dword ptr [esp + 20h]
        lea    ebx, [edx + eax*4 + 8]
        movzx  eax, word ptr [esi + 3ch]
        mov    edx, dword ptr [ebx]
        sub    eax, edx
        sbb    ebp, ebp
        and    eax, ebp
        add    eax, edx
        mov    dword ptr [ebx], eax
        mov    edx, dword ptr [esp + 5ch]
        add    edx, dword ptr [esp + 64h]
        cmp    edx, eax
        jbe    SHORT loc_1_6537
        mov    dword ptr [esp + 34h], 1
loc_1_6537:
        cmp    ecx, dword ptr [esp + 10h]
        jne    SHORT loc_1_6571
loc_1_653D:
        lea    eax, [ecx + ecx*2]
        mov    edx, dword ptr [esp + 20h]
        lea    ecx, [edx + eax*4]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [ecx + 4], eax
        movzx  eax, word ptr [esi + 3ch]
        mov    dword ptr [ecx + 8], eax
        mov    ecx, dword ptr [esp + 5ch]
        add    ecx, dword ptr [esp + 64h]
        cmp    ecx, eax
        jbe    SHORT loc_1_656D
        mov    dword ptr [esp + 34h], 1
loc_1_656D:
        inc    dword ptr [esp + 10h]
loc_1_6571:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    NEAR PTR loc_1_6498
loc_1_657B:
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_6598
        push   dword ptr [esp + 20h]
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
loc_1_6598:
        cmp    dword ptr [esp + 10h], 1
        ja     SHORT loc_1_65BC
        cmp    dword ptr [esp + 34h], 0
        jne    SHORT loc_1_65BC
        mov    dword ptr [esp + 38h], 0
        mov    eax, dword ptr [esp + 3ch]
        mov    dword ptr [eax + 8], 0
        jmp    SHORT loc_1_6600
        db  090h
loc_1_65BC:
        push   8
        call   sub_1_B93E
        add    esp, 4
        mov    dword ptr [esp + 38h], eax
        or     eax, eax
        jne    SHORT loc_1_65E8
        push   dword ptr [esp + 20h]
        call   sub_1_B944
        add    esp, 4
        mov    eax, 2afeh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  090h
loc_1_65E8:
        mov    eax, dword ptr [esp + 58h]
        mov    ecx, dword ptr [esp + 38h]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 4], 0
        mov    eax, dword ptr [esp + 3ch]
        mov    dword ptr [eax + 8], ecx
loc_1_6600:
        mov    dword ptr [esp + 28h], 0
        cmp    dword ptr [esp + 10h], 0
        je     NEAR PTR loc_1_67D8
        mov    ebx, dword ptr [esp + 20h]
        add    ebx, 8
loc_1_661A:
        mov    eax, dword ptr [esp + 10h]
        sub    eax, dword ptr [esp + 28h]
        mov    dword ptr [esp + 2ch], eax
        cmp    eax, 1
        je     NEAR PTR loc_1_6714
        lea    eax, [esp + 14h]
        push   eax
        call   sub_1_5B88
        add    esp, 4
        mov    dword ptr [esp + 1ch], eax
        or     eax, eax
        jne    SHORT loc_1_6650
        db  0FFh, 005h, 004h, 0C2h, 000h, 000h
        jmp    NEAR PTR loc_1_67C3
        db  090h
loc_1_6650:
        push   dword ptr [esp + 5ch]
        push   0
        push   dword ptr [esp + 60h]
        call   sub_1_5DD8
        add    esp, 0ch
        mov    dword ptr [esp + 18h], eax
        or     eax, eax
        jne    SHORT loc_1_6684
        push   dword ptr [esp + 14h]
        call   sub_1_58C8
        add    esp, 4
        db  0FFh, 005h, 004h, 0C2h, 000h, 000h
        jmp    NEAR PTR loc_1_67C3
        db  08Dh, 049h, 000h
loc_1_6684:
        mov    ecx, 5
        mov    edi, dword ptr [esp + 14h]
        add    edi, 38h
        mov    esi, dword ptr [esp + 3ch]
        rep movsd
        mov    edi, dword ptr [esp + 1ch]
        mov    esi, dword ptr [esp + 54h]
        mov    ecx, 5
        rep movsd
        mov    eax, dword ptr [esp + 14h]
        or     byte ptr [eax + 3dh], 2
        mov    eax, dword ptr [esp + 14h]
        and    byte ptr [eax + 3dh], 0f7h
        cmp    dword ptr [esp + 60h], 0
        je     SHORT loc_1_6710
        push   dword ptr [esp + 64h]
        call   sub_1_B93E
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_1_66F4
        push   dword ptr [esp + 18h]
        call   sub_1_589C
        add    esp, 4
        push   dword ptr [esp + 14h]
        call   sub_1_58C8
        add    esp, 4
        db  0FFh, 005h, 004h, 0C2h, 000h, 000h
        jmp    NEAR PTR loc_1_67C3
        db  08Dh, 049h, 000h
loc_1_66F4:
        mov    edi, ebp
        mov    eax, dword ptr [esp + 64h]
        mov    esi, dword ptr [esp + 60h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        jmp    SHORT loc_1_6730
        db  08Bh, 0FFh
loc_1_6710:
        sub    ebp, ebp
        jmp    SHORT loc_1_6730
loc_1_6714:
        mov    eax, dword ptr [esp + 54h]
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [esp + 50h]
        mov    dword ptr [esp + 14h], eax
        mov    ebp, dword ptr [esp + 60h]
        mov    eax, dword ptr [esp + 58h]
        mov    dword ptr [esp + 18h], eax
loc_1_6730:
        push   dword ptr [ebx - 8]
        push   dword ptr [esp + 70h]
        push   ebp
        call   sub_1_6264
        add    esp, 0ch
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [esp + 5ch]
        add    eax, dword ptr [esp + 64h]
        cmp    eax, ecx
        jbe    SHORT loc_1_6794
        lea    eax, [esp + 30h]
        push   eax
        push   dword ptr [esp + 68h]
        push   ebp
        push   dword ptr [esp + 68h]
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 30h]
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 68h]
        push   ecx
        push   dword ptr [ebx - 4]
        call   sub_1_5E80
        add    esp, 28h
        mov    dword ptr [esp + 24h], eax
        cmp    dword ptr [esp + 2ch], 1
        je     SHORT loc_1_67C3
        push   dword ptr [esp + 18h]
        call   sub_1_589C
        add    esp, 4
        jmp    SHORT loc_1_67C3
        db  08Dh, 049h, 000h
loc_1_6794:
        push   dword ptr [esp + 64h]
        push   ebp
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 60h]
        push   dword ptr [ebx - 4]
        call   sub_1_5C7C
        add    esp, 1ch
        mov    dword ptr [esp + 24h], eax
        cmp    eax, 2bf7h
        jne    SHORT loc_1_67C3
        inc    dword ptr [esp + 30h]
loc_1_67C3:
        add    ebx, 0ch
        inc    dword ptr [esp + 28h]
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [esp + 28h], eax
        jb     NEAR PTR loc_1_661A
loc_1_67D8:
        cmp    dword ptr [esp + 38h], 0
        je     SHORT loc_1_6830
        push   dword ptr [esp + 30h]
        push   dword ptr [esp + 3ch]
        call   sub_1_5BC8
        add    esp, 8
        or     eax, eax
        push   dword ptr [esp + 20h]
        jne    SHORT loc_1_6818
        call   sub_1_B944
        add    esp, 4
        push   dword ptr [esp + 38h]
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  08Dh, 049h, 000h
loc_1_6818:
        call   sub_1_B944
        add    esp, 4
        mov    eax, 2bf7h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  08Dh, 049h, 000h
loc_1_6830:
        push   dword ptr [esp + 20h]
        call   sub_1_B944
        add    esp, 4
        mov    eax, dword ptr [esp + 24h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
sub_1_6848:
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0FFh, 005h, 0FCh, 0C1h, 000h, 000h
        call   sub_1_5B4C
        mov    ebp, eax
        or     ebp, ebp
        je     NEAR PTR loc_1_6CE8
        lea    ecx, [ebp + 40h]
        mov    dword ptr [esp + 28h], ecx
        mov    dword ptr [ecx], 0
        mov    eax, dword ptr [esp + 30h]
        mov    dword ptr [ebp + 44h], eax
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [ebp + 48h], eax
        mov    esi, dword ptr [esp + 4ch]
        or     esi, esi
        je     NEAR PTR loc_1_69F0
        cmp    byte ptr [esi + 10h], 0
        je     NEAR PTR loc_1_69C4
        inc    dword ptr [esi + 14h]
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [ecx + 0ch]
        cmp    eax, -1
        jne    SHORT loc_1_68A7
        mov    eax, dword ptr [esp + 40h]
loc_1_68A7:
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [esp + 20h], eax
        mov    eax, dword ptr [ecx + 18h]
        mov    dword ptr [esp + 24h], eax
        mov    eax, dword ptr [esp + 48h]
        cmp    dword ptr [eax], 0
        jne    NEAR PTR loc_1_69B8
        cmp    byte ptr [esi + 11h], 1
        je     NEAR PTR loc_1_69B8
        mov    ecx, dword ptr [esp + 3ch]
        cmp    dword ptr [esp + 24h], ecx
        jb     NEAR PTR loc_1_69B8
        mov    ecx, dword ptr [esp + 38h]
        add    dword ptr [ecx + 0ch], 14h
        push   ecx
        push   ebp
        call   sub_1_BA56
        mov    ecx, dword ptr [esp + 38h]
        mov    edi, dword ptr [ecx + 4]
        mov    eax, dword ptr [esp + 30h]
        mov    al, byte ptr [eax + 10h]
        mov    byte ptr [edi + 9], al
        mov    word ptr [edi + 0ah], 0
        mov    eax, dword ptr [esp + 40h]
        mov    dword ptr [edi + 10h], eax
        mov    eax, dword ptr [esp + 44h]
        mov    dword ptr [edi + 0ch], eax
        mov    eax, dword ptr [esp + 48h]
        mov    al, byte ptr [eax + 9]
        mov    byte ptr [edi + 8], al
        mov    eax, dword ptr [esp + 48h]
        mov    al, byte ptr [eax + 0ah]
        mov    byte ptr [edi + 1], al
        mov    eax, dword ptr [esp + 48h]
        movzx  eax, byte ptr [eax + 0bh]
        shl    eax, 0dh
        shr    eax, 8
        and    ax, 40h
        mov    word ptr [edi + 6], ax
        db  0A1h, 08Ch, 0C0h, 000h, 000h
        mov    word ptr [edi + 4], ax
        db  0FFh, 005h, 08Ch, 0C0h, 000h, 000h
        mov    byte ptr [edi], 45h
        mov    eax, dword ptr [esp + 3ch]
        add    eax, 14h
        mov    al, ah
        mov    ecx, dword ptr [esp + 3ch]
        add    cx, 14h
        mov    ah, cl
        mov    word ptr [edi + 2], ax
        push   14h
        push   edi
        call   sub_1_5C30
        add    esp, 8
        not    ax
        mov    word ptr [edi + 0ah], ax
        push   esi
        push   dword ptr [esp + 18h]
        push   ebp
        mov    eax, dword ptr [esp + 2ch]
        push   dword ptr [eax + 4]
        call   dword ptr [eax + 8]
        add    esp, 10h
        dec    dword ptr [esi + 14h]
        cmp    eax, 103h
        je     SHORT loc_1_69A8
        push   ebp
        call   sub_1_58C8
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  090h
loc_1_69A8:
        pop    ebp
        mov    eax, 2bf7h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Dh, 049h, 000h
loc_1_69B8:
        dec    dword ptr [esi + 14h]
        mov    al, byte ptr [esi + 11h]
        mov    byte ptr [esp + 13h], al
        jmp    SHORT loc_1_69F8
loc_1_69C4:
        push   esi
        call   sub_1_783C
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_69F0
        db  0FFh, 005h, 008h, 0C2h, 000h, 000h
        push   ebp
        call   sub_1_58C8
        add    esp, 4
        mov    eax, 2afbh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Dh, 049h, 000h
loc_1_69F0:
        mov    dword ptr [esp + 14h], 0
loc_1_69F8:
        call   sub_1_5B68
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_6A20
        push   ebp
        call   sub_1_58C8
        add    esp, 4
        db  0FFh, 005h, 004h, 0C2h, 000h, 000h
        mov    eax, 2afeh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  090h
loc_1_6A20:
        mov    ecx, dword ptr [esp + 38h]
        mov    edi, dword ptr [ecx + 4]
        lea    esi, [edi + 14h]
        mov    eax, dword ptr [ecx + 0ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        push   ebx
        push   ebp
        call   sub_1_BA56
        mov    ebx, dword ptr [ebx + 4]
        mov    eax, dword ptr [esp + 30h]
        mov    al, byte ptr [eax + 10h]
        mov    byte ptr [ebx + 9], al
        mov    word ptr [ebx + 0ah], 0
        mov    eax, dword ptr [esp + 40h]
        mov    dword ptr [ebx + 10h], eax
        mov    eax, dword ptr [esp + 44h]
        mov    dword ptr [ebx + 0ch], eax
        mov    eax, dword ptr [esp + 48h]
        mov    al, byte ptr [eax + 9]
        mov    byte ptr [ebx + 8], al
        mov    eax, dword ptr [esp + 48h]
        mov    al, byte ptr [eax + 0ah]
        mov    byte ptr [ebx + 1], al
        mov    eax, dword ptr [esp + 48h]
        movzx  eax, byte ptr [eax + 0bh]
        shl    eax, 0dh
        shr    eax, 8
        and    ax, 40h
        mov    word ptr [ebx + 6], ax
        db  0A1h, 08Ch, 0C0h, 000h, 000h
        mov    word ptr [ebx + 4], ax
        db  0FFh, 005h, 08Ch, 0C0h, 000h, 000h
        or     byte ptr [ebp + 3dh], 10h
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_1_6B08
        mov    eax, dword ptr [esp + 48h]
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        je     SHORT loc_1_6AB9
        mov    dword ptr [esp + 40h], eax
        mov    dword ptr [ebx + 10h], eax
loc_1_6AB9:
        lea    eax, [esp + 24h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   dword ptr [esp + 4ch]
        push   dword ptr [esp + 4ch]
        call   sub_1_7500
        add    esp, 10h
        mov    dword ptr [esp + 20h], eax
        or     eax, eax
        jne    SHORT loc_1_6AF8
        push   ebp
        call   sub_1_58C8
        add    esp, 4
        db  0FFh, 005h, 008h, 0C2h, 000h, 000h
        mov    eax, 2afbh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  090h
loc_1_6AF8:
        push   dword ptr [esp + 40h]
        call   sub_1_7044
        add    esp, 4
        mov    byte ptr [esp + 13h], al
loc_1_6B08:
        mov    eax, dword ptr [esp + 48h]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_6BA0
        test   byte ptr [eax + 0bh], 80h
        je     SHORT loc_1_6B44
        mov    eax, dword ptr [esp + 40h]
        cmp    dword ptr [esp + 14h], eax
        je     SHORT loc_1_6B44
        push   ebp
        call   sub_1_58C8
        add    esp, 4
        db  0FFh, 005h, 008h, 0C2h, 000h, 000h
        mov    eax, 2afbh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Dh, 049h, 000h
loc_1_6B44:
        mov    eax, dword ptr [esp + 48h]
        movzx  eax, byte ptr [eax + 8]
        mov    dword ptr [esp + 18h], eax
        push   eax
        call   sub_1_B93E
        add    esp, 4
        mov    dword ptr [esp + 1ch], eax
        or     eax, eax
        jne    SHORT loc_1_6B80
        push   ebp
        call   sub_1_58C8
        add    esp, 4
        db  0FFh, 005h, 004h, 0C2h, 000h, 000h
        mov    eax, 2afeh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Dh, 049h, 000h
loc_1_6B80:
        mov    edx, dword ptr [esp + 18h]
        mov    edi, dword ptr [esp + 1ch]
        mov    eax, dword ptr [esp + 48h]
        mov    esi, dword ptr [eax]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        jmp    SHORT loc_1_6BB0
        db  08Bh, 0FFh
loc_1_6BA0:
        mov    dword ptr [esp + 1ch], 0
        mov    dword ptr [esp + 18h], 0
loc_1_6BB0:
        mov    eax, dword ptr [esp + 18h]
        add    eax, 14h
        shr    eax, 2
        add    al, 40h
        mov    byte ptr [ebx], al
        mov    edx, dword ptr [esp + 3ch]
        mov    eax, dword ptr [esp + 3ch]
        add    edx, dword ptr [esp + 18h]
        add    ax, word ptr [esp + 18h]
        add    ax, 14h
        mov    ah, al
        lea    ecx, [edx + 14h]
        mov    al, ch
        cmp    byte ptr [esp + 13h], 1
        mov    word ptr [ebx + 2], ax
        jne    SHORT loc_1_6C34
        cmp    dword ptr [esp + 44h], 0
        jne    SHORT loc_1_6C08
        push   ebx
        push   dword ptr [esp + 3ch]
        push   ebp
        push   dword ptr [esp + 4ch]
        call   sub_1_5D88
        add    esp, 10h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Bh, 0FFh
loc_1_6C08:
        push   0
        push   1
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 4ch]
        push   dword ptr [esp + 4ch]
        push   ebx
        push   ebp
        push   dword ptr [esp + 60h]
        push   0
        call   sub_1_63F4
        add    esp, 28h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_1_6C34:
        cmp    dword ptr [esp + 24h], edx
        jae    NEAR PTR loc_1_6CC0
        push   8
        call   sub_1_B93E
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_6C7C
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_1_6C5F
        push   dword ptr [esp + 1ch]
        call   sub_1_B944
        add    esp, 4
loc_1_6C5F:
        push   ebp
        call   sub_1_58C8
        add    esp, 4
        db  0FFh, 005h, 004h, 0C2h, 000h, 000h
        mov    eax, 2afeh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  090h
loc_1_6C7C:
        push   0
        mov    ecx, dword ptr [esp + 3ch]
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], 0
        mov    ecx, dword ptr [esp + 2ch]
        mov    dword ptr [ecx], eax
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 48h]
        push   dword ptr [esp + 48h]
        push   ebx
        push   ebp
        push   dword ptr [esp + 30h]
        push   dword ptr [esp + 44h]
        push   dword ptr [esp + 44h]
        call   sub_1_5E80
        add    esp, 28h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  090h
loc_1_6CC0:
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 20h]
        push   ebx
        push   dword ptr [esp + 44h]
        push   ebp
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 38h]
        call   sub_1_5C7C
        add    esp, 1ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Bh, 0FFh
loc_1_6CE8:
        pop    ebp
        db  0FFh, 005h, 004h, 0C2h, 000h, 000h
        mov    eax, 2afeh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  090h
sub_1_6CFC:
        push   ebx
        push   esi
        sub    eax, eax
        sub    edx, edx
        db  08Bh, 00Dh, 068h, 0C0h, 000h, 000h
        cmp    ecx, eax
        jle    SHORT loc_1_6D32
        db  0BEh, 00Ch, 0C0h, 000h, 000h
        mov    bl, byte ptr [esp + 0ch]
loc_1_6D15:
        cmp    byte ptr [esi], bl
        je     SHORT loc_1_6D24
        add    esi, 14h
        inc    edx
        cmp    edx, ecx
        jl     SHORT loc_1_6D15
        pop    esi
        pop    ebx
        ret
loc_1_6D24:
        lea    eax, [edx*4]
        db  08Bh, 084h, 080h, 004h, 0C0h, 000h, 000h
loc_1_6D32:
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_1_6D38:
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   dword ptr [esp + 1ch]
        call   sub_1_6CFC
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_6D70
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 28h]
        push   0
        mov    edi, dword ptr [esp + 24h]
        push   edi
        mov    esi, dword ptr [esp + 24h]
        push   esi
        push   2b0ch
        push   0
        call   eax
        add    esp, 1ch
        jmp    SHORT loc_1_6D78
        db  08Bh, 0FFh
loc_1_6D70:
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 18h]
loc_1_6D78:
        sub    ebx, ebx
        db  039h, 01Dh, 068h, 0C0h, 000h, 000h
        jle    SHORT loc_1_6DB3
        db  0BDh, 004h, 0C0h, 000h, 000h
        nop
loc_1_6D88:
        mov    eax, dword ptr [ebp]
        or     eax, eax
        je     SHORT loc_1_6DA7
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 28h]
        push   0
        push   edi
        push   esi
        push   2b0dh
        push   1
        call   eax
        add    esp, 1ch
loc_1_6DA7:
        inc    ebx
        add    ebp, 14h
        db  03Bh, 01Dh, 068h, 0C0h, 000h, 000h
        jl     SHORT loc_1_6D88
loc_1_6DB3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_6DB8:
        push   esi
        push   edi
        sub    edi, edi
        db  039h, 03Dh, 068h, 0C0h, 000h, 000h
        jle    SHORT loc_1_6DF1
        db  0BEh, 004h, 0C0h, 000h, 000h
loc_1_6DC9:
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_1_6DE5
        push   0
        push   0
        push   0
        push   0
        push   0
        push   2b0eh
        push   1
        call   eax
        add    esp, 1ch
loc_1_6DE5:
        inc    edi
        add    esi, 14h
        db  03Bh, 03Dh, 068h, 0C0h, 000h, 000h
        jl     SHORT loc_1_6DC9
loc_1_6DF1:
        pop    edi
        pop    esi
        ret
sub_1_6DF4:
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [esp + 10h]
        cmp    eax, 1
        je     SHORT loc_1_6E10
        cmp    eax, 2
        je     SHORT loc_1_6E60
        cmp    eax, 3
        je     SHORT loc_1_6E7C
        pop    esi
        add    esp, 4
        ret
loc_1_6E10:
        cmp    dword ptr [esp + 18h], 4
        jb     NEAR PTR loc_1_6EC5
        mov    eax, dword ptr [esp + 0ch]
        mov    edx, dword ptr [eax + 0ch]
        mov    esi, dword ptr [esp + 14h]
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx + 40h], eax
        sub    eax, 14h
        mov    dword ptr [esp + 4], eax
        mov    eax, dword ptr [edx + 38h]
        or     eax, eax
        je     SHORT loc_1_6E48
loc_1_6E39:
        mov    ecx, dword ptr [esp + 4]
        mov    word ptr [eax + 3ch], cx
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_1_6E39
loc_1_6E48:
        lea    eax, [esp + 4]
        push   eax
        push   edx
        db  068h, 0B0h, 077h, 000h, 000h
        call   sub_1_77EC
        add    esp, 0ch
        pop    esi
        add    esp, 4
        ret
loc_1_6E60:
        cmp    dword ptr [esp + 18h], 4
        jb     SHORT loc_1_6EC5
        mov    esi, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [eax + 44h], ecx
        pop    esi
        add    esp, 4
        ret
loc_1_6E7C:
        cmp    dword ptr [esp + 18h], 8
        jb     SHORT loc_1_6EC5
        mov    esi, dword ptr [esp + 14h]
        mov    eax, dword ptr [esi]
        sub    eax, 14h
        mov    dword ptr [esp + 4], eax
        add    esi, 4
        lea    eax, [esp + 4]
        push   eax
        push   esi
        db  068h, 0CCh, 077h, 000h, 000h
        call   sub_1_77EC
        add    esp, 0ch
        push   1
        push   2
        push   1
        push   dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 1ch]
        push   dword ptr [eax + 0ch]
        push   -1
        push   -1
        push   dword ptr [esi]
        call   sub_1_7F60
        add    esp, 20h
loc_1_6EC5:
        pop    esi
        add    esp, 4
        ret
        db  08Bh, 0FFh
sub_1_6ECC:
        mov    edx, dword ptr [esp + 8]
        cmp    edx, -1
        je     SHORT loc_1_6EFC
        or     edx, edx
        je     SHORT loc_1_6EFC
        mov    al, byte ptr [esp + 4]
        shr    al, 2
        and    eax, 0ffh
        and    eax, 0fffffffch
        db  08Bh, 080h, 000h, 0BDh, 000h, 000h
        mov    ecx, eax
        and    ecx, edx
        cmp    ecx, eax
        je     SHORT loc_1_6EFE
        sub    eax, eax
        ret
        db  08Dh, 049h, 000h
loc_1_6EFC:
        mov    eax, edx
loc_1_6EFE:
        ret
        db  090h
sub_1_6F00:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 38h]
        or     ecx, ecx
        je     SHORT loc_1_6F22
        inc    eax
        mov    eax, dword ptr [esp + 8]
loc_1_6F10:
        test   byte ptr [ecx + 14h], 1
        je     SHORT loc_1_6F1B
        cmp    dword ptr [ecx + 4], eax
        je     SHORT loc_1_6F28
loc_1_6F1B:
        mov    ecx, dword ptr [ecx + 10h]
        or     ecx, ecx
        jne    SHORT loc_1_6F10
loc_1_6F22:
        sub    eax, eax
        ret
        db  08Dh, 049h, 000h
loc_1_6F28:
        mov    eax, 1
        ret
        db  08Bh, 0FFh
sub_1_6F30:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax + 38h]
        or     eax, eax
        je     SHORT loc_1_6F7C
        sub    ecx, ecx
        mov    esi, dword ptr [esp + 10h]
loc_1_6F44:
        test   byte ptr [eax + 14h], 1
        je     SHORT loc_1_6F60
        mov    edx, dword ptr [eax + 8]
        mov    edi, dword ptr [eax + 4]
        and    edi, edx
        mov    ebx, esi
        and    ebx, edx
        cmp    edi, ebx
        je     SHORT loc_1_6F9E
        or     ecx, ecx
        jne    SHORT loc_1_6F60
        mov    ecx, eax
loc_1_6F60:
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_1_6F44
        or     ecx, ecx
        jne    SHORT loc_1_6F74
        cmp    esi, -1
        je     SHORT loc_1_6F7C
        or     esi, esi
        je     SHORT loc_1_6F7C
loc_1_6F74:
        mov    eax, ecx
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_6F7C:
        db  0A1h, 090h, 0C0h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_6F9C
        inc    ecx
        db  08Bh, 00Dh, 050h, 0C0h, 000h, 000h
loc_1_6F8C:
        cmp    ecx, eax
        je     SHORT loc_1_6F96
        test   byte ptr [eax + 14h], 1
        jne    SHORT loc_1_6F9E
loc_1_6F96:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_6F8C
loc_1_6F9C:
        sub    eax, eax
loc_1_6F9E:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
sub_1_6FA4:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    eax, dword ptr [edi + 0ch]
        mov    edx, dword ptr [eax + 3ch]
        test   byte ptr [edi + 14h], 1
        je     SHORT loc_1_7030
        mov    eax, dword ptr [edi + 8]
        mov    esi, dword ptr [edi + 4]
        mov    ecx, eax
        not    ecx
        and    ecx, edx
        and    eax, esi
        or     ecx, eax
        cmp    ecx, dword ptr [esp + 0ch]
        jne    SHORT loc_1_6FD4
        mov    al, 3
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_6FD4:
        mov    al, byte ptr [esp + 0ch]
        and    al, 0f0h
        cmp    al, 0e0h
        je     SHORT loc_1_7008
        mov    al, byte ptr [esp + 0ch]
        shr    al, 2
        and    eax, 0ffh
        and    eax, 0fffffffch
        db  08Bh, 080h, 000h, 0BDh, 000h, 000h
        mov    ecx, eax
        not    ecx
        and    ecx, edx
        and    eax, esi
        or     ecx, eax
        cmp    ecx, dword ptr [esp + 0ch]
        jne    SHORT loc_1_7030
        mov    al, 1
        pop    edi
        pop    esi
        ret
loc_1_7008:
        db  083h, 03Dh, 0F8h, 0C2h, 000h, 000h, 002h
        jne    SHORT loc_1_7030
        mov    eax, dword ptr [edi + 40h]
        or     eax, eax
        je     SHORT loc_1_7030
loc_1_7018:
        mov    ecx, dword ptr [esp + 0ch]
        cmp    dword ptr [eax + 4], ecx
        je     SHORT loc_1_7027
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_7018
loc_1_7027:
        or     eax, eax
        je     SHORT loc_1_7030
        mov    al, 5
        pop    edi
        pop    esi
        ret
loc_1_7030:
        cmp    edx, dword ptr [esp + 0ch]
        jne    SHORT loc_1_703C
        mov    al, 1
        pop    edi
        pop    esi
        ret
        db  090h
loc_1_703C:
        sub    al, al
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_7044:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    al, byte ptr [ebp + 8]
        and    al, 0f0h
        cmp    al, 0f0h
        jne    SHORT loc_1_705C
        cmp    dword ptr [ebp + 8], -1
        je     SHORT loc_1_705C
        mov    al, 0ffh
        pop    esi
        leave
        ret
loc_1_705C:
        db  08Bh, 035h, 090h, 0C0h, 000h, 000h
loc_1_7062:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 4], eax
        jne    SHORT loc_1_7070
        test   byte ptr [esi + 14h], 1
        jne    SHORT loc_1_70B4
loc_1_7070:
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_1_6FA4
        add    esp, 8
        or     al, al
        jne    NEAR PTR loc_1_7126
        test   byte ptr [esi + 14h], 1
        je     SHORT loc_1_70A0
        mov    ecx, dword ptr [esi + 8]
        mov    eax, dword ptr [ebp + 8]
        and    eax, ecx
        mov    edx, dword ptr [esi + 4]
        and    edx, ecx
        cmp    edx, eax
        jne    SHORT loc_1_70A0
        cmp    dword ptr [ebp + 8], eax
        je     SHORT loc_1_70BC
loc_1_70A0:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_7062
        cmp    byte ptr [ebp + 8], 7fh
        jne    SHORT loc_1_70C4
        sub    al, al
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_70B4:
        sub    al, al
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_70BC:
        pop    esi
        mov    al, 0ffh
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_70C4:
        mov    al, byte ptr [ebp + 8]
        and    al, 0f0h
        cmp    al, 0e0h
        jne    SHORT loc_1_70E4
        db  083h, 03Dh, 0F8h, 0C2h, 000h, 000h, 000h
        je     SHORT loc_1_70DC
        mov    al, 9
        pop    esi
        leave
        ret
        db  090h
loc_1_70DC:
        pop    esi
        mov    al, 0ffh
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_70E4:
        mov    al, byte ptr [ebp + 8]
        shr    al, 2
        and    eax, 0ffh
        and    eax, 0fffffffch
        db  08Bh, 088h, 000h, 0BDh, 000h, 000h
        mov    eax, ecx
        not    eax
        or     eax, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ebp + 8]
        jne    SHORT loc_1_710C
        mov    al, 7
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_710C:
        mov    eax, dword ptr [ebp + 8]
        and    eax, ecx
        cmp    eax, dword ptr [ebp + 8]
        je     SHORT loc_1_7124
        cmp    dword ptr [ebp + 8], 0
        je     SHORT loc_1_7124
        mov    al, 6
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_7124:
        mov    al, 0ffh
loc_1_7126:
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_712C:
        movzx  eax, byte ptr [esp + 6]
        movzx  ecx, byte ptr [esp + 7]
        add    eax, ecx
        movzx  ecx, byte ptr [esp + 5]
        add    eax, ecx
        movzx  ecx, byte ptr [esp + 4]
        add    eax, ecx
        and    eax, 1fh
        ret
        db  08Bh, 0FFh
sub_1_714C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        mov    ebx, dword ptr [esi]
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [ebx + 4], eax
        jne    SHORT loc_1_716C
        test   byte ptr [ebx + 14h], 1
        je     SHORT loc_1_716C
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_716C:
        push   ebx
        push   dword ptr [esp + 18h]
        call   sub_1_6FA4
        add    esp, 8
        or     al, al
        jne    NEAR PTR loc_1_727A
        mov    ebp, dword ptr [ebx + 0ch]
        db  08Bh, 03Dh, 090h, 0C0h, 000h, 000h
loc_1_718A:
        cmp    ebx, edi
        je     SHORT loc_1_71B3
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [edi + 4], eax
        jne    SHORT loc_1_719D
        test   byte ptr [edi + 14h], 1
        jne    SHORT loc_1_71D0
loc_1_719D:
        cmp    dword ptr [edi + 0ch], ebp
        jne    SHORT loc_1_71B3
        push   edi
        push   dword ptr [esp + 18h]
        call   sub_1_6FA4
        add    esp, 8
        or     al, al
        jne    SHORT loc_1_71DC
loc_1_71B3:
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    SHORT loc_1_718A
        cmp    byte ptr [esp + 14h], 7fh
        jne    SHORT loc_1_71E4
        db  0A1h, 050h, 0C0h, 000h, 000h
        mov    dword ptr [esi], eax
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_71D0:
        mov    dword ptr [esi], edi
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_71DC:
        mov    dword ptr [esi], edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_71E4:
        mov    cl, byte ptr [esp + 14h]
        and    cl, 0f0h
        cmp    cl, 0e0h
        jne    SHORT loc_1_7208
        db  083h, 03Dh, 0F8h, 0C2h, 000h, 000h, 000h
        je     SHORT loc_1_7200
        mov    al, 9
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_7200:
        pop    ebp
        mov    al, 0ffh
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_7208:
        mov    esi, dword ptr [esi]
        mov    al, byte ptr [esp + 14h]
        shr    al, 2
        and    eax, 0ffh
        and    eax, 0fffffffch
        db  08Bh, 0B8h, 000h, 0BDh, 000h, 000h
        mov    edx, edi
        and    edx, dword ptr [esp + 14h]
        mov    eax, dword ptr [esi + 0ch]
        mov    eax, dword ptr [eax + 3ch]
        not    edi
        and    eax, edi
        or     eax, edx
        cmp    eax, dword ptr [esp + 14h]
        jne    SHORT loc_1_7240
        mov    al, 7
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_7240:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_1_7278
        cmp    dword ptr [esp + 14h], edx
        je     SHORT loc_1_7278
        cmp    cl, 0f0h
        jne    SHORT loc_1_7259
        cmp    dword ptr [esp + 14h], -1
        jne    SHORT loc_1_7278
loc_1_7259:
        db  0A1h, 004h, 0BCh, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_7270
        cmp    eax, esi
        jne    SHORT loc_1_7270
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_7270:
        pop    ebp
        mov    al, 6
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_7278:
        mov    al, 0ffh
loc_1_727A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
sub_1_7280:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 14h]
        push   edi
        mov    esi, dword ptr [esp + 14h]
        push   esi
        call   sub_1_6ECC
        add    esp, 8
        mov    ecx, esi
        and    ecx, eax
        push   ecx
        call   sub_1_712C
        add    esp, 4
        db  08Dh, 01Ch, 085h, 048h, 0C2h, 000h, 000h
        mov    eax, dword ptr [ebx]
        or     eax, eax
        je     SHORT loc_1_72D4
        mov    ecx, dword ptr [esp + 18h]
        inc    edx
        mov    edx, dword ptr [esp + 1ch]
loc_1_72B8:
        cmp    dword ptr [eax + 4], esi
        jne    SHORT loc_1_72CC
        cmp    dword ptr [eax + 8], edi
        jne    SHORT loc_1_72CC
        cmp    dword ptr [eax + 0ch], ecx
        jne    SHORT loc_1_72CC
        cmp    dword ptr [eax + 1ch], edx
        je     SHORT loc_1_72D4
loc_1_72CC:
        mov    ebx, eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_72B8
loc_1_72D4:
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [ecx], ebx
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
sub_1_72E0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 20h]
        db  08Bh, 034h, 085h, 048h, 0C2h, 000h, 000h
        mov    eax, dword ptr [esp + 24h]
loc_1_72F6:
        or     esi, esi
        je     SHORT loc_1_7304
        cmp    dword ptr [esi + 10h], eax
        jbe    SHORT loc_1_7310
        mov    esi, dword ptr [esi]
        jmp    SHORT loc_1_72F6
        db  090h
loc_1_7304:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_7310:
        mov    edi, dword ptr [esp + 18h]
        mov    dword ptr [esp + 10h], 0
        mov    eax, dword ptr [esp + 28h]
loc_1_7320:
        test   byte ptr [esi + 26h], 1
        je     SHORT loc_1_7335
        cmp    dword ptr [esi + 10h], eax
        jb     SHORT loc_1_7340
        mov    ecx, dword ptr [esi + 8]
        and    ecx, edi
        cmp    ecx, dword ptr [esi + 4]
        je     SHORT loc_1_734C
loc_1_7335:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_7320
        jmp    SHORT loc_1_73B0
        db  08Dh, 049h, 000h
loc_1_7340:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_734C:
        mov    dword ptr [esp + 10h], esi
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_1_73B0
        push   dword ptr [esp + 1ch]
        push   dword ptr [esi + 1ch]
        call   sub_1_6F00
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_73B0
        mov    ebx, dword ptr [esi + 10h]
        mov    ebp, dword ptr [esi + 14h]
        mov    esi, dword ptr [esi]
        or     esi, esi
        je     SHORT loc_1_73B0
loc_1_7376:
        cmp    dword ptr [esi + 10h], ebx
        jne    SHORT loc_1_73B0
        cmp    dword ptr [esi + 14h], ebp
        jne    SHORT loc_1_73B0
        test   byte ptr [esi + 26h], 1
        je     SHORT loc_1_73A3
        mov    eax, dword ptr [esi + 8]
        and    eax, edi
        cmp    eax, dword ptr [esi + 4]
        jne    SHORT loc_1_73A3
        push   dword ptr [esp + 1ch]
        push   dword ptr [esi + 1ch]
        call   sub_1_6F00
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_73AC
loc_1_73A3:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_7376
        jmp    SHORT loc_1_73B0
        db  090h
loc_1_73AC:
        mov    dword ptr [esp + 10h], esi
loc_1_73B0:
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_73BC:
        push   esi
        push   edi
        sub    esi, esi
        call   sub_1_B92C
        mov    ecx, 3e8h
        sub    edx, edx
        div    ecx
        mov    edi, eax
        push   esi
        call   sub_1_712C
        add    esp, 4
        db  08Bh, 00Ch, 085h, 048h, 0C2h, 000h, 000h
        cmp    ecx, esi
        je     SHORT loc_1_7412
        mov    eax, dword ptr [esp + 0ch]
loc_1_73E8:
        cmp    dword ptr [ecx + 8], 0
        jne    SHORT loc_1_740C
        mov    dx, word ptr [ecx + 26h]
        test   dl, 1
        jne    SHORT loc_1_740C
        or     eax, eax
        je     SHORT loc_1_7400
        cmp    dword ptr [ecx + 0ch], eax
        jne    SHORT loc_1_740C
loc_1_7400:
        inc    esi
        or     dx, 1
        mov    word ptr [ecx + 26h], dx
        mov    dword ptr [ecx + 30h], edi
loc_1_740C:
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    SHORT loc_1_73E8
loc_1_7412:
        mov    eax, esi
        db  001h, 035h, 0C8h, 0C2h, 000h, 000h
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_7420:
        push   esi
        mov    esi, dword ptr [esp + 10h]
        cmp    esi, 20h
        jne    SHORT loc_1_7450
        push   esi
        push   esi
        push   dword ptr [esp + 10h]
        call   sub_1_712C
        add    esp, 4
        push   eax
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        call   sub_1_72E0
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_7450
        pop    esi
        ret
        db  090h
loc_1_7450:
        or     esi, esi
        je     SHORT loc_1_748E
        push   1
        push   esi
        mov    al, byte ptr [esp + 10h]
        shr    al, 2
        and    eax, 0ffh
        and    eax, 0fffffffch
        db  08Bh, 080h, 000h, 0BDh, 000h, 000h
        and    eax, dword ptr [esp + 10h]
        push   eax
        call   sub_1_712C
        add    esp, 4
        push   eax
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        call   sub_1_72E0
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_1_74AC
loc_1_748E:
        push   0
        push   esi
        push   0
        call   sub_1_712C
        add    esp, 4
        push   eax
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        call   sub_1_72E0
        add    esp, 14h
loc_1_74AC:
        pop    esi
        ret
        db  08Bh, 0FFh
sub_1_74B0:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        mov    esi, dword ptr [ebx + 10h]
        mov    edi, dword ptr [ebx + 14h]
        push   dword ptr [ebx + 8]
        push   dword ptr [ebx + 4]
        call   sub_1_6ECC
        add    esp, 8
        mov    ecx, dword ptr [ebx + 4]
        and    ecx, eax
        push   ecx
        call   sub_1_712C
        add    esp, 4
        db  08Dh, 004h, 085h, 048h, 0C2h, 000h, 000h
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     SHORT loc_1_74FC
loc_1_74E6:
        mov    edx, dword ptr [ecx + 10h]
        cmp    esi, edx
        ja     SHORT loc_1_74FC
        jne    SHORT loc_1_74F4
        cmp    dword ptr [ecx + 14h], edi
        ja     SHORT loc_1_74FC
loc_1_74F4:
        mov    eax, ecx
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    SHORT loc_1_74E6
loc_1_74FC:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_7500:
        push   esi
        push   edi
        push   20h
        push   dword ptr [esp + 14h]
        mov    esi, dword ptr [esp + 14h]
        push   esi
        call   sub_1_7420
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_753C
        mov    edi, dword ptr [eax + 1ch]
        mov    edx, dword ptr [eax + 0ch]
        cmp    edx, -1
        mov    ecx, dword ptr [esp + 14h]
        jne    SHORT loc_1_752C
        mov    dword ptr [ecx], esi
        jmp    SHORT loc_1_752E
loc_1_752C:
        mov    dword ptr [ecx], edx
loc_1_752E:
        mov    ecx, dword ptr [eax + 18h]
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [eax], ecx
        mov    eax, edi
        pop    edi
        pop    esi
        ret
loc_1_753C:
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_7544:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_B92C
        mov    ecx, 3e8h
        sub    edx, edx
        div    ecx
        mov    dword ptr [esp + 10h], eax
        mov    esi, dword ptr [esp + 18h]
        mov    ebx, dword ptr [esi + 4]
        mov    ebp, dword ptr [ebx + 1ch]
        mov    ecx, dword ptr [ebx + 4]
        mov    edi, dword ptr [esp + 1ch]
        mov    dword ptr [edi], ecx
        mov    eax, dword ptr [ebp + 30h]
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [ebx + 14h]
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [edi + 0ch], 0ffffffffh
        mov    dword ptr [edi + 10h], 0ffffffffh
        mov    dword ptr [edi + 14h], 0ffffffffh
        mov    dword ptr [edi + 2ch], 0ffffffffh
        mov    eax, dword ptr [ebx + 0ch]
        cmp    eax, -1
        jne    SHORT loc_1_75BE
        push   ebp
        push   ecx
        call   sub_1_6F30
        add    esp, 8
        cmp    dword ptr [ebp + 38h], 0
        je     SHORT loc_1_75BC
        or     eax, eax
        je     SHORT loc_1_75BC
        mov    eax, dword ptr [eax + 4]
        jmp    SHORT loc_1_75BE
        db  08Dh, 049h, 000h
loc_1_75BC:
        mov    eax, dword ptr [edi]
loc_1_75BE:
        mov    dword ptr [edi + 18h], eax
        test   byte ptr [ebx + 26h], 1
        je     SHORT loc_1_75D0
        movzx  eax, word ptr [ebx + 24h]
        mov    dword ptr [edi + 1ch], eax
        jmp    SHORT loc_1_75D7
loc_1_75D0:
        mov    dword ptr [edi + 1ch], 2
loc_1_75D7:
        mov    eax, dword ptr [ebx + 2ch]
        mov    dword ptr [edi + 20h], eax
        mov    eax, dword ptr [esp + 10h]
        sub    eax, dword ptr [ebx + 30h]
        mov    dword ptr [edi + 24h], eax
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [edi + 28h], eax
        mov    eax, dword ptr [ebx]
        or     eax, eax
        je     SHORT loc_1_7604
        mov    dword ptr [esi + 4], eax
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_7604:
        mov    ecx, dword ptr [esi]
        inc    ecx
        cmp    ecx, 20h
        jae    SHORT loc_1_7624
        inc    eax
        db  08Dh, 004h, 08Dh, 048h, 0C2h, 000h, 000h
loc_1_7614:
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_763C
        add    eax, 4
        inc    ecx
        db  03Dh, 0C8h, 0C2h, 000h, 000h
        jb     SHORT loc_1_7614
loc_1_7624:
        sub    eax, eax
        mov    dword ptr [esi], 0
        mov    dword ptr [esi + 4], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_763C:
        pop    ebp
        db  08Bh, 004h, 08Dh, 048h, 0C2h, 000h, 000h
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi], ecx
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
sub_1_7658:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        or     esi, esi
        jne    SHORT loc_1_76A4
        or     eax, eax
        jne    SHORT loc_1_76A4
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], 1
        inc    eax
        db  08Dh, 004h, 0B5h, 048h, 0C2h, 000h, 000h
loc_1_767C:
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        jne    SHORT loc_1_7691
        add    eax, 4
        inc    esi
        db  03Dh, 0C8h, 0C2h, 000h, 000h
        jb     SHORT loc_1_767C
        or     ecx, ecx
        je     SHORT loc_1_76A0
loc_1_7691:
        mov    dword ptr [edx], esi
        mov    dword ptr [edx + 4], ecx
        mov    eax, 1
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_76A0:
        sub    eax, eax
        pop    esi
        ret
loc_1_76A4:
        cmp    esi, 20h
        jae    SHORT loc_1_76BE
        db  08Bh, 00Ch, 0B5h, 048h, 0C2h, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_1_76BE
loc_1_76B4:
        cmp    ecx, eax
        je     SHORT loc_1_76CC
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    SHORT loc_1_76B4
loc_1_76BE:
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], 0
        sub    eax, eax
        pop    esi
        ret
loc_1_76CC:
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], 1
        mov    eax, 1
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_76E0:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0ch]
        mov    edi, dword ptr [eax + 1ch]
        mov    esi, dword ptr [eax + 20h]
        mov    dword ptr [eax + 20h], 0
        or     esi, esi
        je     SHORT loc_1_771B
        nop
loc_1_76F8:
        cmp    byte ptr [esi + 10h], 0
        je     SHORT loc_1_7715
        mov    byte ptr [esi + 10h], 0
        mov    dword ptr [esi + 4], edi
        cmp    dword ptr [esi + 14h], 0
        jne    SHORT loc_1_7715
        push   esi
        push   dword ptr [edi + 4]
        call   dword ptr [edi + 14h]
        add    esp, 8
loc_1_7715:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_76F8
loc_1_771B:
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
sub_1_7720:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax], ecx
        db  0FFh, 00Dh, 030h, 0C2h, 000h, 000h
        cmp    dword ptr [esi + 8], 0
        jne    SHORT loc_1_775E
        db  0FFh, 00Dh, 0CCh, 0C1h, 000h, 000h
        test   byte ptr [esi + 26h], 1
        je     SHORT loc_1_774B
        db  0FFh, 00Dh, 0C8h, 0C2h, 000h, 000h
loc_1_774B:
        db  083h, 03Dh, 0C8h, 0C2h, 000h, 000h, 000h
        jne    SHORT loc_1_775E
        push   0
        call   sub_1_73BC
        add    esp, 4
loc_1_775E:
        push   esi
        call   sub_1_76E0
        add    esp, 4
        push   esi
        call   sub_1_B944
        add    esp, 4
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_7774:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [ecx + 1ch], eax
        jne    SHORT loc_1_778C
        mov    eax, dword ptr [eax + 3ch]
        cmp    dword ptr [ecx + 4], eax
        je     SHORT loc_1_778C
        sub    eax, eax
        ret
loc_1_778C:
        mov    eax, 1
        ret
        db  08Bh, 0FFh
loc_1_7794:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [ecx + 1ch], eax
        jne    SHORT loc_1_77AA
        push   ecx
        call   sub_1_76E0
        add    esp, 4
loc_1_77AA:
        mov    eax, 1
        ret
loc_1_77B0:
        mov    eax, dword ptr [esp + 0ch]
        mov    edx, dword ptr [eax]
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        cmp    dword ptr [eax + 1ch], ecx
        jne    SHORT loc_1_77C6
        mov    dword ptr [eax + 18h], edx
loc_1_77C6:
        mov    eax, 1
        ret
loc_1_77CC:
        mov    eax, dword ptr [esp + 0ch]
        mov    edx, dword ptr [eax]
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 0ch], ecx
        jne    SHORT loc_1_77E4
        mov    dword ptr [eax + 18h], edx
loc_1_77E4:
        mov    eax, 1
        ret
        db  08Bh, 0FFh
sub_1_77EC:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0BEh, 048h, 0C2h, 000h, 000h
        mov    edi, dword ptr [esp + 14h]
loc_1_77F9:
        mov    ebp, esi
        mov    ebx, dword ptr [esi]
        or     ebx, ebx
        je     SHORT loc_1_7829
loc_1_7801:
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 1ch]
        push   ebx
        call   edi
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_7820
        push   ebx
        push   ebp
        call   sub_1_7720
        add    esp, 8
        jmp    SHORT loc_1_7822
        db  090h
loc_1_7820:
        mov    ebp, ebx
loc_1_7822:
        mov    ebx, dword ptr [ebp]
        or     ebx, ebx
        jne    SHORT loc_1_7801
loc_1_7829:
        add    esi, 4
        db  081h, 0FEh, 0C8h, 0C2h, 000h, 000h
        jb     SHORT loc_1_77F9
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_1_783C:
        push   esi
        push   edi
        db  0A1h, 090h, 0C0h, 000h, 000h
        or     eax, eax
        mov    esi, dword ptr [esp + 0ch]
        je     SHORT loc_1_7860
        nop
loc_1_784C:
        test   byte ptr [eax + 14h], 1
        je     SHORT loc_1_785A
        mov    ecx, dword ptr [esi + 0ch]
        cmp    dword ptr [eax + 4], ecx
        je     SHORT loc_1_7860
loc_1_785A:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_784C
loc_1_7860:
        or     eax, eax
        jne    SHORT loc_1_786C
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_786C:
        push   20h
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi + 8]
        call   sub_1_7420
        add    esp, 0ch
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_78B4
        cmp    dword ptr [esi + 14h], 0
        jne    SHORT loc_1_78AA
        cmp    byte ptr [esi + 10h], 0
        jne    SHORT loc_1_78AA
        mov    eax, dword ptr [esi + 4]
        push   esi
        push   dword ptr [eax + 4]
        call   dword ptr [eax + 14h]
        add    esp, 8
        mov    dword ptr [esi + 4], edi
        mov    byte ptr [esi + 10h], 1
        mov    eax, dword ptr [edi + 20h]
        mov    dword ptr [esi], eax
        mov    dword ptr [edi + 20h], esi
loc_1_78AA:
        pop    edi
        mov    eax, 1
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_78B4:
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_78BC:
        push   20h
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        call   sub_1_7420
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_78F4
        mov    ecx, dword ptr [esp + 0ch]
        or     ecx, ecx
        je     SHORT loc_1_78DF
        mov    edx, dword ptr [eax + 18h]
        mov    dword ptr [ecx], edx
loc_1_78DF:
        mov    ecx, dword ptr [esp + 10h]
        or     ecx, ecx
        je     SHORT loc_1_78EF
        mov    eax, dword ptr [eax + 1ch]
        mov    eax, dword ptr [eax + 44h]
        mov    dword ptr [ecx], eax
loc_1_78EF:
        sub    eax, eax
        ret
        db  08Bh, 0FFh
loc_1_78F4:
        mov    eax, 2afbh
        ret
        db  08Bh, 0FFh
loc_1_78FC:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_B92C
        mov    ecx, 3e8h
        sub    edx, edx
        div    ecx
        mov    edi, eax
        db  083h, 03Dh, 05Ch, 0C1h, 000h, 000h, 000h
        je     NEAR PTR loc_1_7A27
        push   20h
        mov    esi, dword ptr [esp + 20h]
        push   esi
        mov    ebp, dword ptr [esp + 20h]
        push   ebp
        call   sub_1_7420
        add    esp, 0ch
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_1_7A27
        mov    eax, edi
        sub    eax, dword ptr [ebx + 30h]
        cmp    eax, 3ch
        jbe    NEAR PTR loc_1_7A27
        cmp    dword ptr [ebx + 0ch], -1
        je     NEAR PTR loc_1_7A27
        cmp    dword ptr [ebx + 2ch], 4
        jne    SHORT loc_1_79B4
        and    word ptr [ebx + 26h], 0fffeh
        push   20h
        push   esi
        push   ebp
        call   sub_1_7420
        add    esp, 0ch
        mov    dword ptr [esp + 10h], eax
        or     eax, eax
        jne    SHORT loc_1_7984
        or     word ptr [ebx + 26h], 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_7984:
        lea    eax, [esp + 10h]
        push   eax
        push   dword ptr [ebx + 1ch]
        push   dword ptr [ebx + 0ch]
        push   dword ptr [ebx + 8]
        push   dword ptr [ebx + 4]
        call   sub_1_7280
        add    esp, 14h
        push   eax
        push   dword ptr [esp + 14h]
        call   sub_1_7720
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_79B4:
        cmp    dword ptr [ebx + 8], 0
        jne    SHORT loc_1_7A27
        db  083h, 03Dh, 0CCh, 0C1h, 000h, 000h, 001h
        jbe    SHORT loc_1_7A27
        push   ebx
        call   sub_1_76E0
        add    esp, 4
        db  083h, 03Dh, 0C8h, 0C2h, 000h, 000h, 001h
        jne    SHORT loc_1_79E8
        push   0
        call   sub_1_73BC
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_79E8:
        db  0FFh, 00Dh, 0C8h, 0C2h, 000h, 000h
        and    word ptr [ebx + 26h], 0fffeh
        push   0
        push   0
        push   0
        call   sub_1_712C
        add    esp, 4
        push   eax
        push   esi
        push   ebp
        call   sub_1_72E0
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_1_7A24
        push   0
        call   sub_1_73BC
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_7A24:
        mov    dword ptr [eax + 30h], edi
loc_1_7A27:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
sub_1_7A30:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 20h]
        or     eax, eax
        je     SHORT loc_1_7A58
        mov    ecx, dword ptr [esp + 8]
        inc    edx
        mov    edx, dword ptr [esp + 0ch]
loc_1_7A44:
        cmp    dword ptr [eax + 8], ecx
        jne    SHORT loc_1_7A52
        or     edx, edx
        je     SHORT loc_1_7A58
        cmp    dword ptr [eax + 0ch], edx
        je     SHORT loc_1_7A58
loc_1_7A52:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_7A44
loc_1_7A58:
        ret
        db  08Dh, 049h, 000h
loc_1_7A5C:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 30h]
        mov    esi, dword ptr [eax + 4]
        or     esi, esi
        jne    SHORT loc_1_7A72
        mov    esi, dword ptr [esp + 1ch]
loc_1_7A72:
        db  083h, 03Dh, 0FCh, 0BBh, 000h, 000h, 000h
        je     SHORT loc_1_7A88
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_7A88:
        push   esi
        call   sub_1_7044
        add    esp, 4
        mov    byte ptr [esp + 13h], al
        mov    edi, dword ptr [esp + 28h]
        mov    byte ptr [edi], al
        cmp    al, 0ffh
        je     NEAR PTR loc_1_7BE0
        mov    ebx, dword ptr [esp + 20h]
        or     ebx, ebx
        je     SHORT loc_1_7AD4
        db  0A1h, 090h, 0C0h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_7AC9
loc_1_7AB4:
        test   byte ptr [eax + 14h], 1
        je     SHORT loc_1_7ABF
        cmp    dword ptr [eax + 4], ebx
        je     SHORT loc_1_7AC5
loc_1_7ABF:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_7AB4
loc_1_7AC5:
        or     eax, eax
        jne    SHORT loc_1_7AD4
loc_1_7AC9:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
loc_1_7AD4:
        push   20h
        push   ebx
        push   esi
        call   sub_1_7420
        add    esp, 0ch
        mov    ebp, eax
        or     ebp, ebp
        je     NEAR PTR loc_1_7BD4
        mov    ecx, dword ptr [ebp + 18h]
        mov    eax, dword ptr [esp + 2ch]
        mov    word ptr [eax], cx
        cmp    bl, 7fh
        jne    SHORT loc_1_7B0C
        db  081h, 07Dh, 01Ch, 0B8h, 0C0h, 000h, 000h
        je     SHORT loc_1_7B0C
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_7B0C:
        cmp    dword ptr [ebp + 0ch], -1
        je     SHORT loc_1_7B15
        or     byte ptr [edi], 10h
loc_1_7B15:
        push   ebx
        push   esi
        push   ebp
        call   sub_1_7A30
        add    esp, 0ch
        mov    ecx, eax
        or     ecx, ecx
        jne    NEAR PTR loc_1_7BBC
        cmp    byte ptr [esp + 13h], 0
        jne    SHORT loc_1_7B38
        mov    dword ptr [esp + 14h], esi
        jmp    SHORT loc_1_7B5B
        db  090h
loc_1_7B38:
        push   dword ptr [ebp + 1ch]
        push   esi
        call   sub_1_6F30
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_7B54
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_7B54:
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esp + 14h], eax
loc_1_7B5B:
        push   20h
        call   sub_1_B93E
        add    esp, 4
        mov    edx, eax
        mov    edi, dword ptr [esp + 24h]
        mov    dword ptr [edi], edx
        or     edx, edx
        je     SHORT loc_1_7BAE
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 8
        rep stosd
        cmp    ebx, eax
        jne    SHORT loc_1_7B8C
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [edx + 0ch], eax
        jmp    SHORT loc_1_7B8F
        db  08Dh, 049h, 000h
loc_1_7B8C:
        mov    dword ptr [edx + 0ch], ebx
loc_1_7B8F:
        mov    dword ptr [edx + 8], esi
        mov    al, byte ptr [esp + 13h]
        mov    byte ptr [edx + 11h], al
        mov    word ptr [edx + 12h], 1
        mov    dword ptr [edx + 4], ebp
        mov    byte ptr [edx + 10h], 1
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [edx], eax
        mov    dword ptr [ebp + 20h], edx
loc_1_7BAE:
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_7BBC:
        mov    eax, dword ptr [ecx + 0ch]
        inc    word ptr [ecx + 12h]
        mov    edi, dword ptr [esp + 24h]
        mov    dword ptr [edi], ecx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_7BD4:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_7BE0:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_7BEC:
        push   esi
        mov    esi, dword ptr [esp + 8]
        or     esi, esi
        je     SHORT loc_1_7C3A
        mov    ax, word ptr [esi + 12h]
        dec    ax
        mov    word ptr [esi + 12h], ax
        jne    SHORT loc_1_7C3A
        cmp    byte ptr [esi + 10h], 0
        je     SHORT loc_1_7C24
        mov    ecx, dword ptr [esi + 4]
        mov    edx, dword ptr [ecx + 1ch]
        add    ecx, 20h
        je     SHORT loc_1_7C1E
loc_1_7C12:
        mov    eax, dword ptr [ecx]
        cmp    esi, eax
        je     SHORT loc_1_7C1E
        mov    ecx, eax
        or     ecx, ecx
        jne    SHORT loc_1_7C12
loc_1_7C1E:
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_7C27
loc_1_7C24:
        mov    edx, dword ptr [esi + 4]
loc_1_7C27:
        push   esi
        push   dword ptr [edx + 4]
        call   dword ptr [edx + 14h]
        add    esp, 8
        push   esi
        call   sub_1_B944
        add    esp, 4
loc_1_7C3A:
        pop    esi
        ret
sub_1_7C3C:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_B92C
        mov    ecx, 3e8h
        sub    edx, edx
        div    ecx
        mov    ebx, eax
        mov    eax, dword ptr [esp + 30h]
        and    eax, dword ptr [esp + 2ch]
        cmp    eax, dword ptr [esp + 2ch]
        je     SHORT loc_1_7C70
        mov    eax, 2b0ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Bh, 0FFh
loc_1_7C70:
        cmp    dword ptr [esp + 48h], 0
        je     SHORT loc_1_7C94
        cmp    dword ptr [esp + 48h], 1
        je     SHORT loc_1_7C94
        cmp    dword ptr [esp + 48h], 2
        je     SHORT loc_1_7C94
        mov    eax, 2b03h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Bh, 0FFh
loc_1_7C94:
        mov    eax, 0
        mov    edi, dword ptr [esp + 34h]
        cmp    edi, -1
        setne  al
        add    eax, 3
        mov    dword ptr [esp + 24h], eax
        mov    ebp, dword ptr [esp + 3ch]
        sub    ebp, 44h
        sbb    eax, eax
        not    eax
        and    ebp, eax
        add    ebp, 44h
        mov    ecx, dword ptr [esp + 38h]
        cmp    dword ptr [ecx + 34h], 0
        jne    SHORT loc_1_7CE0
        cmp    dword ptr [ecx + 38h], 0
        je     SHORT loc_1_7CE0
        mov    eax, dword ptr [esp + 2ch]
        cmp    dword ptr [ecx + 3ch], eax
        je     SHORT loc_1_7CE0
        mov    eax, 2b03h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
loc_1_7CE0:
        lea    eax, [esp + 20h]
        push   eax
        push   dword ptr [esp + 3ch]
        push   edi
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 3ch]
        call   sub_1_7280
        add    esp, 14h
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_7D84
        mov    dword ptr [esi + 18h], ebp
        mov    eax, dword ptr [esp + 44h]
        mov    dword ptr [esi + 2ch], eax
        mov    dword ptr [esi + 30h], ebx
        mov    dword ptr [esi + 34h], ebx
        mov    bx, word ptr [esi + 26h]
        mov    ax, bx
        and    ax, 0fffdh
        or     ax, 1
        cmp    dword ptr [esi + 8], 0
        mov    word ptr [esi + 26h], ax
        jne    SHORT loc_1_7D38
        test   bl, 1
        jne    SHORT loc_1_7D38
        db  0FFh, 005h, 0C8h, 0C2h, 000h, 000h
loc_1_7D38:
        mov    eax, dword ptr [esi]
        mov    edx, dword ptr [esi + 14h]
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [ecx], eax
        mov    edi, dword ptr [esp + 40h]
        mov    dword ptr [esi + 14h], edi
        cmp    edi, edx
        jb     SHORT loc_1_7D78
        test   bl, 1
        je     SHORT loc_1_7D78
        push   esi
        call   sub_1_74B0
        add    esp, 4
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi], ecx
        mov    dword ptr [eax], esi
        push   esi
        call   sub_1_76E0
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Dh, 049h, 000h
loc_1_7D78:
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [esp + 18h], eax
        jmp    NEAR PTR loc_1_7E2C
loc_1_7D84:
        push   38h
        call   sub_1_B93E
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_7DA4
        mov    eax, 2afeh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Dh, 049h, 000h
loc_1_7DA4:
        db  0FFh, 005h, 030h, 0C2h, 000h, 000h
        mov    dword ptr [esp + 18h], 0
        mov    eax, dword ptr [esp + 30h]
        or     eax, eax
        je     SHORT loc_1_7DC9
loc_1_7DBA:
        mov    ecx, eax
        and    ecx, 1
        add    dword ptr [esp + 18h], ecx
        shr    eax, 1
        or     eax, eax
        jne    SHORT loc_1_7DBA
loc_1_7DC9:
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esi + 10h], eax
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [esp + 30h]
        mov    dword ptr [esi + 8], eax
        or     eax, eax
        jne    SHORT loc_1_7DEE
        db  0FFh, 005h, 0CCh, 0C1h, 000h, 000h
        db  0FFh, 005h, 0C8h, 0C2h, 000h, 000h
loc_1_7DEE:
        mov    dword ptr [esi + 0ch], edi
        mov    edi, dword ptr [esp + 40h]
        mov    dword ptr [esi + 14h], edi
        mov    dword ptr [esi + 18h], ebp
        mov    ecx, dword ptr [esp + 38h]
        mov    dword ptr [esi + 1ch], ecx
        mov    dword ptr [esi + 20h], 0
        mov    ax, word ptr [esp + 24h]
        mov    word ptr [esi + 24h], ax
        mov    word ptr [esi + 26h], 1
        mov    eax, dword ptr [esp + 44h]
        mov    dword ptr [esi + 2ch], eax
        mov    dword ptr [esi + 30h], ebx
        mov    dword ptr [esi + 34h], ebx
        mov    eax, dword ptr [esp + 48h]
        mov    dword ptr [esi + 28h], eax
loc_1_7E2C:
        push   esi
        call   sub_1_74B0
        add    esp, 4
        mov    dword ptr [esp + 20h], eax
        push   dword ptr [esp + 18h]
        push   0
        push   dword ptr [esp + 34h]
        call   sub_1_7420
        add    esp, 0ch
        mov    dword ptr [esp + 10h], eax
        or     eax, eax
        je     NEAR PTR loc_1_7F45
        mov    edx, eax
        mov    eax, dword ptr [edx + 14h]
        mov    dword ptr [esp + 1ch], eax
        mov    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [edx + 10h]
        cmp    dword ptr [esp + 18h], eax
        ja     SHORT loc_1_7E8A
        cmp    edi, dword ptr [esp + 1ch]
        jb     SHORT loc_1_7E86
        jne    NEAR PTR loc_1_7F45
        mov    ecx, dword ptr [esp + 38h]
        cmp    dword ptr [edx + 1ch], ecx
        je     NEAR PTR loc_1_7F45
loc_1_7E86:
        cmp    dword ptr [esp + 18h], eax
loc_1_7E8A:
        jne    SHORT loc_1_7E9A
        cmp    edi, dword ptr [esp + 1ch]
        mov    dword ptr [esp + 14h], 0
        je     SHORT loc_1_7EA2
loc_1_7E9A:
        mov    dword ptr [esp + 14h], 1
loc_1_7EA2:
        mov    edx, dword ptr [esp + 10h]
        mov    ebx, dword ptr [edx + 1ch]
        mov    ebp, edx
        add    ebp, 20h
        mov    edi, dword ptr [ebp]
        or     edi, edi
        je     SHORT loc_1_7F15
loc_1_7EB5:
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_1_7ECF
        push   dword ptr [edi + 0ch]
        push   dword ptr [esp + 3ch]
        call   sub_1_6F00
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_7F0C
loc_1_7ECF:
        mov    eax, dword ptr [edi + 8]
        and    eax, dword ptr [esp + 30h]
        cmp    eax, dword ptr [esp + 2ch]
        jne    SHORT loc_1_7F0C
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebp], eax
        cmp    dword ptr [edi + 14h], 0
        jne    SHORT loc_1_7F00
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [edi], eax
        mov    dword ptr [esi + 20h], edi
        mov    dword ptr [edi + 4], esi
        push   edi
        push   dword ptr [ebx + 4]
        call   dword ptr [ebx + 14h]
        add    esp, 8
        jmp    SHORT loc_1_7F0E
        db  08Bh, 0FFh
loc_1_7F00:
        mov    dword ptr [edi + 4], ebx
        mov    byte ptr [edi + 10h], 0
        jmp    SHORT loc_1_7F0E
        db  08Dh, 049h, 000h
loc_1_7F0C:
        mov    ebp, edi
loc_1_7F0E:
        mov    edi, dword ptr [ebp]
        or     edi, edi
        jne    SHORT loc_1_7EB5
loc_1_7F15:
        mov    edx, dword ptr [esp + 10h]
        mov    edx, dword ptr [edx]
        mov    dword ptr [esp + 10h], edx
        or     edx, edx
        je     SHORT loc_1_7F45
        mov    eax, dword ptr [esp + 18h]
        cmp    dword ptr [edx + 10h], eax
        jne    SHORT loc_1_7F45
        mov    eax, dword ptr [esp + 1ch]
        cmp    dword ptr [edx + 14h], eax
        jne    SHORT loc_1_7F45
        mov    eax, dword ptr [edx + 8]
        and    eax, dword ptr [esp + 2ch]
        cmp    eax, dword ptr [edx + 4]
        je     NEAR PTR loc_1_7EA2
loc_1_7F45:
        mov    eax, dword ptr [esp + 20h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [eax], esi
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Dh, 049h, 000h
sub_1_7F60:
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 20h]
        call   sub_1_7C3C
        add    esp, 20h
        ret
        db  08Dh, 049h, 000h
sub_1_7F8C:
        sub    esp, 4
        lea    eax, [esp]
        push   eax
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        call   sub_1_7280
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_1_7FBC
        mov    eax, 2b04h
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_7FBC:
        cmp    dword ptr [eax + 28h], 0
        jne    SHORT loc_1_7FCC
        mov    eax, 2b03h
        add    esp, 4
        ret
        db  090h
loc_1_7FCC:
        push   eax
        push   dword ptr [esp + 4]
        call   sub_1_7720
        add    esp, 8
        sub    eax, eax
        add    esp, 4
        ret
        db  090h
sub_1_7FE0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 2ch]
        or     ebx, ebx
        je     NEAR PTR loc_1_809E
        cmp    bl, 7fh
        je     NEAR PTR loc_1_809E
        push   20h
        mov    edi, dword ptr [esp + 2ch]
        push   edi
        mov    esi, dword ptr [esp + 2ch]
        push   esi
        call   sub_1_7420
        add    esp, 0ch
        mov    ebp, eax
        or     ebp, ebp
        je     NEAR PTR loc_1_809E
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, -1
        je     SHORT loc_1_809E
        cmp    eax, dword ptr [esp + 20h]
        jne    SHORT loc_1_809E
        cmp    dword ptr [ebp + 8], 0
        jne    SHORT loc_1_804C
        push   ebx
        call   sub_1_73BC
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_804C
        push   ebp
        call   sub_1_76E0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_804C:
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   edi
        push   ebx
        call   sub_1_7500
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_809E
        cmp    dword ptr [esp + 14h], ebx
        jne    SHORT loc_1_809E
        mov    ecx, dword ptr [esp + 1ch]
        mov    eax, dword ptr [ecx + 8]
        mov    edx, dword ptr [ecx + 4]
        and    edx, eax
        and    eax, ebx
        cmp    edx, eax
        jne    SHORT loc_1_809E
        cmp    ebx, esi
        jne    SHORT loc_1_8085
        mov    ebx, 0ffffffffh
loc_1_8085:
        push   1
        push   4
        push   1
        push   dword ptr [esp + 1ch]
        push   dword ptr [ecx + 0ch]
        push   ebx
        push   -1
        push   esi
        call   sub_1_7F60
        add    esp, 20h
loc_1_809E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
sub_1_80A8:
        mov    edx, 0ah
        db  0B9h, 068h, 0BDh, 000h, 000h
        mov    eax, dword ptr [esp + 4]
loc_1_80B6:
        cmp    dword ptr [ecx], eax
        ja     SHORT loc_1_80C6
        sub    ecx, 4
        dec    edx
        db  081h, 0F9h, 040h, 0BDh, 000h, 000h
        jne    SHORT loc_1_80B6
loc_1_80C6:
        db  08Bh, 004h, 095h, 040h, 0BDh, 000h, 000h
        ret
        db  08Bh, 0FFh
sub_1_80D0:
        sub    ecx, ecx
        db  0BAh, 040h, 0BDh, 000h, 000h
        inc    eax
        mov    eax, dword ptr [esp + 4]
loc_1_80DC:
        cmp    dword ptr [edx], eax
        jb     SHORT loc_1_80EC
        add    edx, 4
        inc    ecx
        db  081h, 0FAh, 068h, 0BDh, 000h, 000h
        jb     SHORT loc_1_80DC
loc_1_80EC:
        db  08Bh, 004h, 08Dh, 040h, 0BDh, 000h, 000h
        ret
sub_1_80F4:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  083h, 03Dh, 060h, 0C0h, 000h, 000h, 000h
        je     NEAR PTR loc_1_81D4
        mov    bp, word ptr [esp + 1ch]
        or     bp, bp
        je     SHORT loc_1_8123
        sub    bp, 44h
        sbb    ax, ax
        not    ax
        and    bp, ax
        sub    bp, -30h
loc_1_8123:
        push   20h
        mov    esi, dword ptr [esp + 1ch]
        mov    al, byte ptr [esi]
        push   dword ptr [esi + 0ch]
        and    ax, 0fh
        push   dword ptr [esi + 10h]
        shl    ax, 2
        mov    word ptr [esp + 1eh], ax
        call   sub_1_7420
        add    esp, 0ch
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_1_81D4
        mov    edi, dword ptr [ebx + 18h]
        movzx  eax, bp
        cmp    eax, edi
        ja     SHORT loc_1_81D4
        or     bp, bp
        jne    SHORT loc_1_8194
        mov    cx, word ptr [esi + 2]
        mov    ah, cl
        mov    al, ch
        sub    ax, 14h
        movzx  ecx, ax
        cmp    ecx, edi
        jb     SHORT loc_1_8177
        sub    ax, word ptr [esp + 12h]
loc_1_8177:
        and    eax, 0ffffh
        cmp    eax, edi
        jbe    SHORT loc_1_8188
        mov    bp, di
        jmp    SHORT loc_1_8194
        db  08Dh, 049h, 000h
loc_1_8188:
        push   eax
        call   sub_1_80D0
        add    esp, 4
        mov    bp, ax
loc_1_8194:
        and    ebp, 0ffffh
        cmp    ebp, edi
        je     SHORT loc_1_81B9
        push   1
        push   4
        push   dword ptr [ebx + 14h]
        push   ebp
        push   dword ptr [ebx + 1ch]
        push   dword ptr [ebx + 0ch]
        push   -1
        push   dword ptr [esi + 10h]
        call   sub_1_7C3C
        add    esp, 20h
loc_1_81B9:
        push   ebp
        movzx  eax, word ptr [esp + 16h]
        add    eax, esi
        push   eax
        mov    al, byte ptr [esi + 9]
        push   eax
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi + 10h]
        call   sub_1_6D38
        add    esp, 14h
loc_1_81D4:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_81DC:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_B92C
        mov    ecx, 3e8h
        sub    edx, edx
        div    ecx
        mov    dword ptr [esp + 10h], eax
        db  0BBh, 048h, 0C2h, 000h, 000h
loc_1_81FA:
        mov    edi, ebx
        mov    esi, dword ptr [ebx]
        or     esi, esi
        je     NEAR PTR loc_1_830C
loc_1_8206:
        cmp    dword ptr [esi + 8], -1
        jne    NEAR PTR loc_1_830C
        lea    edx, [esi + 26h]
        mov    cx, word ptr [edx]
        test   cl, 1
        je     NEAR PTR loc_1_8300
        db  083h, 03Dh, 060h, 0C0h, 000h, 000h, 000h
        je     NEAR PTR loc_1_82D8
        mov    eax, dword ptr [esp + 10h]
        sub    eax, dword ptr [esi + 34h]
        test   cl, 2
        je     SHORT loc_1_8240
        cmp    eax, 78h
        jmp    SHORT loc_1_8245
        db  08Dh, 049h, 000h
loc_1_8240:
        cmp    eax, 258h
loc_1_8245:
        mov    eax, 0
        setae  al
        or     eax, eax
        je     NEAR PTR loc_1_82D8
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi + 34h], eax
        mov    ebp, dword ptr [esi + 1ch]
        mov    eax, dword ptr [esi + 18h]
        cmp    eax, dword ptr [ebp + 40h]
        jae    SHORT loc_1_82D0
        or     cx, 2
        mov    word ptr [edx], cx
        push   eax
        call   sub_1_80A8
        add    esp, 4
        cmp    eax, dword ptr [ebp + 40h]
        jb     SHORT loc_1_8284
        mov    edi, dword ptr [ebp + 40h]
        jmp    SHORT loc_1_8291
        db  08Dh, 049h, 000h
loc_1_8284:
        push   dword ptr [esi + 18h]
        call   sub_1_80A8
        add    esp, 4
        mov    edi, eax
loc_1_8291:
        mov    dword ptr [esi + 18h], edi
        mov    esi, dword ptr [esi + 4]
        mov    ebp, dword ptr [ebp + 38h]
        or     ebp, ebp
        je     SHORT loc_1_82C8
loc_1_829E:
        test   byte ptr [ebp + 14h], 1
        je     SHORT loc_1_82C1
        movzx  eax, word ptr [ebp + 3ch]
        sub    eax, edi
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, edi
        push   eax
        push   0
        push   0
        push   dword ptr [ebp + 4]
        push   esi
        call   sub_1_6D38
        add    esp, 14h
loc_1_82C1:
        mov    ebp, dword ptr [ebp + 10h]
        or     ebp, ebp
        jne    SHORT loc_1_829E
loc_1_82C8:
        mov    edi, ebx
        mov    esi, dword ptr [ebx]
        jmp    SHORT loc_1_8304
        db  08Bh, 0FFh
loc_1_82D0:
        and    cx, 0fffdh
        mov    word ptr [edx], cx
loc_1_82D8:
        cmp    dword ptr [esi + 2ch], 4
        jne    SHORT loc_1_8300
        cmp    dword ptr [esi + 20h], 0
        jne    SHORT loc_1_8300
        mov    eax, dword ptr [esp + 10h]
        sub    eax, dword ptr [esi + 30h]
        cmp    eax, 258h
        jbe    SHORT loc_1_8300
        push   esi
        push   edi
        call   sub_1_7720
        add    esp, 8
        mov    esi, dword ptr [edi]
        jmp    SHORT loc_1_8304
loc_1_8300:
        mov    edi, esi
        mov    esi, dword ptr [esi]
loc_1_8304:
        or     esi, esi
        jne    NEAR PTR loc_1_8206
loc_1_830C:
        add    ebx, 4
        db  081h, 0FBh, 0C8h, 0C2h, 000h, 000h
        jb     NEAR PTR loc_1_81FA
        push   0
        db  068h, 0DCh, 081h, 000h, 000h
        push   0ea60h
        db  068h, 024h, 0C1h, 000h, 000h
        call   sub_1_B920
        add    esp, 10h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_833C:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    dword ptr [esi + 8], 0
        mov    dword ptr [esi + 14h], 0
        mov    dword ptr [esi], 0
        mov    dword ptr [esi + 4], 0
        lea    edi, [esi + 38h]
        mov    eax, dword ptr [edi + 2ch]
        or     eax, eax
        je     SHORT loc_1_8377
        push   eax
        call   sub_1_B944
        add    esp, 4
        mov    dword ptr [edi + 2ch], 0
loc_1_8377:
        mov    eax, dword ptr [edi + 24h]
        or     eax, eax
        je     SHORT loc_1_8395
        db  08Bh, 015h, 05Ch, 0C0h, 000h, 000h
        mov    ecx, dword ptr [edi + 28h]
        mov    dword ptr [ecx], edx
        db  0A3h, 05Ch, 0C0h, 000h, 000h
        mov    dword ptr [edi + 24h], 0
loc_1_8395:
        db  0A1h, 0ACh, 0BFh, 000h, 000h
        mov    dword ptr [edi], eax
        db  089h, 035h, 0ACh, 0BFh, 000h, 000h
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_83A8:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        db  08Bh, 035h, 0C0h, 0C1h, 000h, 000h
        db  02Bh, 035h, 020h, 0C1h, 000h, 000h
        sub    esi, 14h
        sbb    eax, eax
        and    esi, eax
        add    esi, 14h
        mov    ebx, 0
        je     SHORT loc_1_8419
        lea    eax, [esi*4]
        lea    eax, [eax + eax*4]
        push   eax
        call   sub_1_B93E
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_8426
        push   esi
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_BA26
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_8419
        push   50h
        push   esi
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_BA02
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_8430
        push   dword ptr [ebp - 10h]
        call   sub_1_BA2C
loc_1_8419:
        or     ebx, ebx
        je     SHORT loc_1_8426
        push   ebx
        call   sub_1_B944
        add    esp, 4
loc_1_8426:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_8430:
        db  001h, 035h, 020h, 0C1h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_84A2
loc_1_843A:
        push   14h
        push   ebx
        push   dword ptr [ebp - 10h]
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_BA32
        push   dword ptr [ebp - 14h]
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_BA0E
        mov    edi, dword ptr [ebp - 4]
        add    edi, 38h
        sub    eax, eax
        mov    ecx, 14h
        rep stosd
        mov    ecx, dword ptr [ebp - 4]
        add    ecx, 38h
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 1ch], eax
        mov    dword ptr [ecx + 20h], ebx
        mov    byte ptr [ecx + 5], 8
        mov    byte ptr [ecx + 4], 1
        db  0A1h, 0FCh, 0BCh, 000h, 000h
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 10h], eax
        push   dword ptr [ebp - 4]
        call   sub_1_833C
        add    esp, 4
        add    ebx, 14h
        dec    esi
        jne    SHORT loc_1_843A
loc_1_84A2:
        pop    edi
        mov    eax, 1
        pop    esi
        pop    ebx
        leave
        ret
sub_1_84AC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        db  08Bh, 035h, 0F8h, 0BFh, 000h, 000h
        db  02Bh, 035h, 0FCh, 0C2h, 000h, 000h
        sub    esi, 7800h
        sbb    eax, eax
        and    esi, eax
        add    esi, 7800h
        cmp    esi, 100h
        jb     SHORT loc_1_854C
        shr    esi, 8
        mov    ebx, esi
        shl    ebx, 8
        push   ebx
        call   sub_1_B93E
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_8559
        push   esi
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_BA26
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_854C
        or     esi, esi
        je     SHORT loc_1_853B
loc_1_850A:
        push   100h
        push   edi
        push   dword ptr [ebp - 0ch]
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_BA32
        db  0A1h, 05Ch, 0C0h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 05Ch, 0C0h, 000h, 000h
        add    edi, 100h
        dec    esi
        jne    SHORT loc_1_850A
loc_1_853B:
        pop    edi
        db  001h, 01Dh, 0FCh, 0C2h, 000h, 000h
        mov    eax, 1
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_854C:
        or     edi, edi
        je     SHORT loc_1_8559
        push   edi
        call   sub_1_B944
        add    esp, 4
loc_1_8559:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_8560:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0ch]
        lea    eax, [ecx + 38h]
        test   byte ptr [eax + 4eh], 1
        jne    SHORT loc_1_8578
        mov    esi, dword ptr [eax + 30h]
        add    esi, 54h
        jmp    SHORT loc_1_857E
        db  090h
loc_1_8578:
        db  08Bh, 035h, 048h, 0C1h, 000h, 000h
loc_1_857E:
        push   ecx
        call   sub_1_833C
        add    esp, 4
        mov    eax, dword ptr [esi + 8]
        dec    eax
        mov    dword ptr [esi + 8], eax
        cmp    dword ptr [esi + 0ch], 0
        je     SHORT loc_1_85E3
        cmp    dword ptr [esi + 10h], 0
        jne    SHORT loc_1_85E3
        mov    dword ptr [esi + 10h], 1
        db  03Bh, 005h, 040h, 0C2h, 000h, 000h
        jae    SHORT loc_1_85DC
loc_1_85A9:
        mov    ecx, dword ptr [esi + 0ch]
        or     ecx, ecx
        je     SHORT loc_1_85DC
        mov    edx, dword ptr [esi]
        mov    eax, dword ptr [edx]
        mov    dword ptr [esi], eax
        mov    edi, dword ptr [edx + 4]
        mov    dword ptr [eax + 4], edi
        dec    ecx
        mov    dword ptr [esi + 0ch], ecx
        inc    dword ptr [esi + 8]
        sub    edx, 4ch
        push   dword ptr [edx + 7ch]
        push   edx
        call   sub_1_85E8
        add    esp, 8
        db  0A1h, 040h, 0C2h, 000h, 000h
        cmp    dword ptr [esi + 8], eax
        jb     SHORT loc_1_85A9
loc_1_85DC:
        mov    dword ptr [esi + 10h], 0
loc_1_85E3:
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
sub_1_85E8:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 14h]
        lea    ebx, [edi + 38h]
        mov    esi, dword ptr [edi + 8]
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [edi + 0ch], eax
        mov    dword ptr [eax], 0
        mov    dword ptr [edi + 4], 14h
        mov    dword ptr [edi + 14h], 1
        mov    eax, dword ptr [eax + 4]
        lea    ecx, [eax + 13h]
        shr    ecx, 0ch
        shr    eax, 0ch
        sub    ecx, eax
        inc    ecx
        mov    dword ptr [edi], ecx
        cmp    byte ptr [ebx + 4eh], 1
        je     NEAR PTR loc_1_86CC
        movzx  eax, byte ptr [ebx + 4ch]
        mov    ebp, dword ptr [esp + 18h]
        lea    ecx, [eax + ebp]
        cmp    ecx, dword ptr [ebx + 38h]
        ja     SHORT loc_1_865C
        push   eax
        push   dword ptr [ebx + 2ch]
        push   dword ptr [ebx + 20h]
        push   esi
        push   edi
        push   dword ptr [ebx + 40h]
        push   dword ptr [ebx + 30h]
        call   sub_1_5C7C
        add    esp, 1ch
        jmp    NEAR PTR loc_1_86F4
        db  08Bh, 0FFh
loc_1_865C:
        push   8
        call   sub_1_B93E
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_867C
        push   esi
        push   edi
        call   sub_1_8560
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_867C:
        mov    dword ptr [eax], esi
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [ebx + 8], eax
        push   0
        movzx  eax, byte ptr [ebx + 4ch]
        push   eax
        push   dword ptr [ebx + 2ch]
        push   ebp
        push   esi
        push   dword ptr [ebx + 20h]
        push   edi
        push   dword ptr [ebx + 40h]
        push   dword ptr [ebx + 38h]
        push   dword ptr [ebx + 30h]
        call   sub_1_5E80
        add    esp, 28h
        mov    ebp, eax
        cmp    ebp, 2b01h
        jne    SHORT loc_1_86F6
        push   0
        push   4
        push   3
        push   dword ptr [ebx + 20h]
        mov    eax, dword ptr [ebx + 3ch]
        push   dword ptr [eax + 4]
        call   sub_1_9610
        add    esp, 14h
        jmp    SHORT loc_1_86F6
loc_1_86CC:
        lea    eax, [ebx + 48h]
        push   eax
        mov    al, byte ptr [ebx + 4dh]
        push   eax
        movzx  eax, byte ptr [ebx + 4ch]
        push   eax
        push   dword ptr [ebx + 2ch]
        mov    ebp, dword ptr [esp + 28h]
        push   ebp
        push   esi
        push   dword ptr [ebx + 20h]
        push   edi
        push   dword ptr [ebx + 40h]
        push   dword ptr [ebx + 3ch]
        call   sub_1_63F4
        add    esp, 28h
loc_1_86F4:
        mov    ebp, eax
loc_1_86F6:
        cmp    ebp, 2bf7h
        je     SHORT loc_1_8708
        push   esi
        push   edi
        call   sub_1_8560
        add    esp, 8
loc_1_8708:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_1_8710:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        lea    edx, [ecx + 38h]
        mov    esi, dword ptr [edx + 30h]
        test   byte ptr [edx + 4eh], 1
        je     SHORT loc_1_872C
        db  08Bh, 035h, 048h, 0C1h, 000h, 000h
        jmp    SHORT loc_1_872F
        db  08Dh, 049h, 000h
loc_1_872C:
        add    esi, 54h
loc_1_872F:
        mov    eax, dword ptr [esi + 8]
        db  03Bh, 005h, 040h, 0C2h, 000h, 000h
        jae    SHORT loc_1_8754
        cmp    dword ptr [esi + 0ch], 0
        jne    SHORT loc_1_8754
        inc    eax
        mov    dword ptr [esi + 8], eax
        push   dword ptr [esp + 10h]
        push   ecx
        call   sub_1_85E8
        add    esp, 8
        pop    esi
        ret
        db  090h
loc_1_8754:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [edx + 44h], eax
        lea    eax, [edx + 14h]
        mov    dword ptr [eax], esi
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [edx + 18h], ecx
        mov    dword ptr [ecx], eax
        mov    dword ptr [esi + 4], eax
        inc    dword ptr [esi + 0ch]
        pop    esi
        ret
sub_1_8770:
        push   esi
        push   edi
        call   sub_1_B92C
        mov    esi, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esi + 0ch]
        or     ecx, ecx
        je     SHORT loc_1_87CC
        lea    edi, [ecx + 1]
        sub    edx, edx
        div    edi
        cmp    edx, ecx
        jne    SHORT loc_1_8794
        sub    al, al
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_8794:
        mov    edi, dword ptr [esi]
        mov    eax, edx
        dec    edx
        or     eax, eax
        je     SHORT loc_1_87A6
loc_1_879D:
        mov    edi, dword ptr [edi]
        mov    eax, edx
        dec    edx
        or     eax, eax
        jne    SHORT loc_1_879D
loc_1_87A6:
        mov    edx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], edx
        dec    ecx
        mov    dword ptr [esi + 0ch], ecx
        sub    edi, 4ch
        push   edi
        call   sub_1_833C
        add    esp, 4
        db  0FFh, 005h, 004h, 0C2h, 000h, 000h
        mov    al, 1
        pop    edi
        pop    esi
        ret
        db  090h
loc_1_87CC:
        sub    al, al
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_87D4:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 24h]
        shr    esi, 8
        mov    eax, dword ptr [esp + 24h]
        and    eax, 0ffh
        mov    dword ptr [esp + 10h], eax
        or     eax, eax
        je     SHORT loc_1_87F4
        inc    esi
loc_1_87F4:
        inc    edi
        mov    edi, dword ptr [esp + 1ch]
        add    edi, 38h
loc_1_87FC:
        sub    eax, eax
        db  08Bh, 01Dh, 05Ch, 0C0h, 000h, 000h
        db  0B9h, 05Ch, 0C0h, 000h, 000h
loc_1_8809:
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        je     SHORT loc_1_881B
        mov    dword ptr [ecx + 0ch], 100h
        inc    eax
        cmp    eax, esi
        jb     SHORT loc_1_8809
loc_1_881B:
        cmp    eax, esi
        je     NEAR PTR loc_1_88C0
        db  0A1h, 0F8h, 0BFh, 000h, 000h
        db  02Bh, 005h, 0FCh, 0C2h, 000h, 000h
        cmp    eax, 100h
        ja     SHORT loc_1_883E
        call   sub_1_84AC
        or     eax, eax
        jne    SHORT loc_1_87FC
loc_1_883E:
        test   byte ptr [edi + 4eh], 1
        jne    SHORT loc_1_884C
        mov    ebp, dword ptr [edi + 30h]
        add    ebp, 54h
        jmp    SHORT loc_1_8852
loc_1_884C:
        db  08Bh, 02Dh, 048h, 0C1h, 000h, 000h
loc_1_8852:
        push   ebp
        call   sub_1_8770
        add    esp, 4
        or     al, al
        db  08Bh, 01Dh, 0B4h, 0C0h, 000h, 000h
        jne    SHORT loc_1_87FC
        test   byte ptr [edi + 4eh], 1
        jne    SHORT loc_1_88A0
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_1_88B0
        or     ebx, ebx
        je     SHORT loc_1_8893
loc_1_8875:
        lea    eax, [ebx + 54h]
        push   eax
        call   sub_1_8770
        add    esp, 4
        or     al, al
        jne    SHORT loc_1_888B
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    SHORT loc_1_8875
loc_1_888B:
        or     ebx, ebx
        jne    NEAR PTR loc_1_87FC
loc_1_8893:
        pop    ebp
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_88A0:
        pop    ebp
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_88B0:
        pop    ebp
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_88C0:
        mov    eax, dword ptr [ecx]
        db  0A3h, 05Ch, 0C0h, 000h, 000h
        mov    dword ptr [ecx], 0
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_88DB
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 0ch], eax
loc_1_88DB:
        push   ebx
        mov    dword ptr [edi + 24h], ebx
        mov    dword ptr [edi + 28h], ecx
        push   dword ptr [esp + 20h]
        call   sub_1_BA50
        mov    eax, dword ptr [esp + 28h]
        cmp    eax, dword ptr [esp + 24h]
        jae    SHORT loc_1_8928
        mov    eax, dword ptr [esp + 18h]
        mov    eax, dword ptr [eax + 0ch]
        lea    ecx, [esp + 24h]
        push   ecx
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 38h]
        push   dword ptr [esp + 44h]
        push   dword ptr [esp + 44h]
        push   dword ptr [eax + 4]
        call   dword ptr [eax + 0ch]
        add    esp, 1ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_8928:
        cmp    dword ptr [esp + 24h], 0
        je     SHORT loc_1_8965
        inc    edx
        mov    edx, dword ptr [esp + 20h]
loc_1_8934:
        mov    edi, dword ptr [ebx + 4]
        mov    ebp, dword ptr [esp + 24h]
        sub    ebp, 100h
        sbb    eax, eax
        and    ebp, eax
        add    ebp, 100h
        mov    esi, edx
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        add    edx, ebp
        sub    dword ptr [esp + 24h], ebp
        mov    ebx, dword ptr [ebx]
        jne    SHORT loc_1_8934
loc_1_8965:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
sub_1_8970:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        mov    bl, byte ptr [esp + 14h]
loc_1_897A:
        db  08Bh, 015h, 0ACh, 0BFh, 000h, 000h
        or     edx, edx
        jne    SHORT loc_1_89BC
        db  0A1h, 0C0h, 0C1h, 000h, 000h
        db  039h, 005h, 020h, 0C1h, 000h, 000h
        jbe    SHORT loc_1_899A
        call   sub_1_83A8
        or     eax, eax
        jne    SHORT loc_1_897A
loc_1_899A:
        test   bl, 1
        lea    eax, [esi + 54h]
        je     SHORT loc_1_89A7
        db  0A1h, 048h, 0C1h, 000h, 000h
loc_1_89A7:
        push   eax
        call   sub_1_8770
        add    esp, 4
        or     al, al
        jne    SHORT loc_1_897A
        sub    eax, eax
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_89BC:
        lea    ecx, [edx + 38h]
        mov    eax, dword ptr [ecx]
        db  0A3h, 0ACh, 0BFh, 000h, 000h
        or     byte ptr [ecx + 5], 10h
        mov    dword ptr [ecx + 8], 0
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [ecx + 20h]
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_1_89E0:
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    byte ptr [esp + 12h], 0
        db  080h, 03Dh, 0A4h, 0BFh, 000h, 000h, 000h
        je     NEAR PTR loc_1_8EB0
        mov    ebx, dword ptr [esp + 48h]
        mov    eax, dword ptr [ebx + 10h]
        mov    dword ptr [esp + 18h], eax
        test   byte ptr [esp + 60h], 1
        je     SHORT loc_1_8A64
        db  080h, 03Dh, 050h, 0C1h, 000h, 000h, 000h
        jne    SHORT loc_1_8A30
        cmp    byte ptr [esp + 60h], 6
        jbe    NEAR PTR loc_1_8EEB
        db  0FFh, 005h, 0E8h, 0C1h, 000h, 000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  08Dh, 049h, 000h
loc_1_8A30:
        cmp    dword ptr [esp + 18h], -1
        je     NEAR PTR loc_1_8EEB
        cmp    dword ptr [esp + 18h], 0
        je     NEAR PTR loc_1_8EEB
        cmp    byte ptr [esp + 60h], 3
        je     NEAR PTR loc_1_8EEB
        mov    al, byte ptr [esp + 18h]
        and    al, 0f0h
        cmp    al, 0e0h
        jne    SHORT loc_1_8A89
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  090h
loc_1_8A64:
        cmp    byte ptr [esp + 60h], 6
        jne    SHORT loc_1_8A89
        mov    ecx, dword ptr [esp + 44h]
        push   dword ptr [ecx + 0ch]
        push   dword ptr [ebx + 0ch]
        call   sub_1_6F30
        add    esp, 8
        mov    dword ptr [esp + 44h], eax
        or     eax, eax
        je     NEAR PTR loc_1_8EEB
loc_1_8A89:
        cmp    byte ptr [ebx + 8], 1
        ja     SHORT loc_1_8AB4
        db  0FFh, 005h, 0E4h, 0C1h, 000h, 000h
        push   0
        push   0
        push   0bh
        push   ebx
        mov    ecx, dword ptr [esp + 54h]
        push   dword ptr [ecx + 4]
        call   sub_1_9610
        add    esp, 14h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  090h
loc_1_8AB4:
        mov    ax, word ptr [ebx + 2]
        mov    cx, ax
        and    ecx, 0ffh
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0ffffh
        or     ecx, eax
        sub    ecx, dword ptr [esp + 4ch]
        mov    dword ptr [esp + 2ch], ecx
        mov    byte ptr [esp + 17h], 0
        mov    byte ptr [esp + 14h], 28h
        mov    byte ptr [esp + 15h], 28h
        mov    byte ptr [esp + 16h], 28h
        cmp    dword ptr [esp + 4ch], 14h
        je     NEAR PTR loc_1_8C28
        lea    eax, [ebx + 14h]
        mov    dword ptr [esp + 34h], eax
        mov    eax, dword ptr [esp + 4ch]
        sub    al, 14h
        mov    byte ptr [esp + 3ch], al
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 38h]
        push   eax
        call   sub_1_4C40
        add    esp, 8
        cmp    al, 28h
        jae    SHORT loc_1_8B48
        db  0FFh, 005h, 0E4h, 0C1h, 000h, 000h
        and    eax, 0ffh
        add    eax, 14h
        push   eax
        push   0
        push   0ch
        push   ebx
        mov    ecx, dword ptr [esp + 54h]
        push   dword ptr [ecx + 4]
        call   sub_1_9610
        add    esp, 14h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
loc_1_8B48:
        movzx  eax, byte ptr [esp + 3ch]
        push   eax
        call   sub_1_B93E
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_1_8B6C
        db  0FFh, 005h, 004h, 0C2h, 000h, 000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  08Bh, 0FFh
loc_1_8B6C:
        mov    edi, ebp
        mov    al, byte ptr [esp + 3ch]
        mov    byte ptr [esp + 13h], al
        and    eax, 0ffh
        mov    esi, dword ptr [esp + 34h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        cmp    byte ptr [esp + 14h], 28h
        je     NEAR PTR loc_1_8C2F
        cmp    byte ptr [esp + 60h], 6
        jb     SHORT loc_1_8BC8
        cmp    byte ptr [esp + 17h], 89h
        jne    SHORT loc_1_8BC0
        push   ebp
        call   sub_1_B944
        add    esp, 4
        db  0FFh, 005h, 0E8h, 0C1h, 000h, 000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  08Dh, 049h, 000h
loc_1_8BC0:
        mov    byte ptr [esp + 14h], 28h
        jmp    SHORT loc_1_8C2F
        db  090h
loc_1_8BC8:
        movzx  eax, byte ptr [esp + 14h]
        add    eax, ebp
        mov    cl, byte ptr [eax + 2]
        dec    cl
        and    ecx, 0ffh
        mov    eax, dword ptr [ecx + eax]
        mov    dword ptr [esp + 18h], eax
        push   eax
        call   sub_1_7044
        add    esp, 4
        mov    byte ptr [esp + 60h], al
        cmp    al, 0ffh
        jne    SHORT loc_1_8C20
        push   0
        push   5
        push   3
        push   ebx
        mov    ecx, dword ptr [esp + 54h]
        push   dword ptr [ecx + 4]
        call   sub_1_9610
        add    esp, 14h
        db  0FFh, 005h, 0E4h, 0C1h, 000h, 000h
        push   ebp
        call   sub_1_B944
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  090h
loc_1_8C20:
        mov    byte ptr [esp + 12h], 1
        jmp    SHORT loc_1_8C2F
        db  090h
loc_1_8C28:
        sub    ebp, ebp
        mov    byte ptr [esp + 13h], 0
loc_1_8C2F:
        db  0FFh, 005h, 0ECh, 0C1h, 000h, 000h
        lea    eax, [esp + 30h]
        push   eax
        lea    eax, [esp + 28h]
        push   eax
        mov    ecx, dword ptr [esp + 4ch]
        push   dword ptr [ecx + 4]
        push   dword ptr [esp + 24h]
        call   sub_1_7500
        add    esp, 10h
        mov    dword ptr [esp + 1ch], eax
        or     eax, eax
        jne    SHORT loc_1_8C90
        db  0FFh, 005h, 008h, 0C2h, 000h, 000h
        push   0
        push   1
        push   3
        push   ebx
        mov    ecx, dword ptr [esp + 54h]
        push   dword ptr [ecx + 4]
        call   sub_1_9610
        add    esp, 14h
        or     ebp, ebp
        je     NEAR PTR loc_1_8EEB
        push   ebp
        call   sub_1_B944
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  090h
loc_1_8C90:
        cmp    byte ptr [esp + 17h], 89h
        jne    SHORT loc_1_8CD0
        mov    eax, dword ptr [esp + 18h]
        cmp    dword ptr [esp + 24h], eax
        je     SHORT loc_1_8CD0
        db  0FFh, 005h, 008h, 0C2h, 000h, 000h
        push   0
        push   5
        push   3
        push   ebx
        mov    ecx, dword ptr [esp + 54h]
        push   dword ptr [ecx + 4]
        call   sub_1_9610
        add    esp, 14h
        push   ebp
        call   sub_1_B944
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  08Bh, 0FFh
loc_1_8CD0:
        cmp    byte ptr [esp + 60h], 1
        je     NEAR PTR loc_1_8D64
        or     ebp, ebp
        je     NEAR PTR loc_1_8D64
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 1ch]
        call   sub_1_6F30
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_8D08
        push   ebp
        call   sub_1_B944
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
loc_1_8D08:
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esp + 28h], eax
        cmp    al, 7fh
        jne    SHORT loc_1_8D1C
        mov    eax, dword ptr [esp + 18h]
        jmp    SHORT loc_1_8D20
        db  08Dh, 049h, 000h
loc_1_8D1C:
        mov    eax, dword ptr [esp + 28h]
loc_1_8D20:
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   ebp
        call   sub_1_6264
        add    esp, 0ch
        cmp    al, 28h
        je     SHORT loc_1_8D64
        db  0FFh, 005h, 0E4h, 0C1h, 000h, 000h
        and    eax, 0ffh
        add    eax, 14h
        push   eax
        push   0
        push   0ch
        push   ebx
        push   dword ptr [esp + 38h]
        call   sub_1_9610
        add    esp, 14h
        push   ebp
        call   sub_1_B944
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
loc_1_8D64:
        mov    eax, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [esp + 44h]
        cmp    dword ptr [ecx + 0ch], eax
        jne    SHORT loc_1_8DA7
        mov    edx, dword ptr [ecx + 4]
        mov    esi, dword ptr [ecx + 8]
        mov    ecx, edx
        and    ecx, esi
        mov    eax, esi
        and    eax, dword ptr [esp + 24h]
        cmp    eax, ecx
        jne    SHORT loc_1_8DA7
        mov    eax, dword ptr [ebx + 0ch]
        and    eax, esi
        cmp    eax, ecx
        jne    SHORT loc_1_8DA7
        cmp    byte ptr [esp + 14h], 28h
        jne    SHORT loc_1_8DA7
        push   dword ptr [esp + 24h]
        push   1
        push   5
        push   ebx
        push   edx
        call   sub_1_9610
        add    esp, 14h
loc_1_8DA7:
        push   dword ptr [esp + 60h]
        push   dword ptr [esp + 20h]
        lea    eax, [esp + 28h]
        push   eax
        call   sub_1_8970
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_1_8E94
        mov    edi, eax
        mov    esi, ebx
        mov    ecx, 5
        rep movsd
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [eax + 10h], ecx
        mov    cl, byte ptr [ebx + 8]
        dec    cl
        mov    byte ptr [eax + 8], cl
        mov    word ptr [eax + 0ah], 0
        mov    edx, dword ptr [esp + 20h]
        add    edx, 38h
        mov    dword ptr [edx + 2ch], ebp
        mov    al, byte ptr [esp + 13h]
        mov    byte ptr [edx + 4ch], al
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [edx + 30h], eax
        mov    eax, dword ptr [esp + 30h]
        mov    dword ptr [edx + 38h], eax
        mov    ecx, dword ptr [esp + 44h]
        mov    dword ptr [edx + 3ch], ecx
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [edx + 40h], eax
        mov    al, byte ptr [esp + 12h]
        mov    byte ptr [edx + 4dh], al
        mov    al, byte ptr [esp + 60h]
        mov    byte ptr [edx + 4eh], al
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [edx + 48h], eax
        push   dword ptr [esp + 5ch]
        push   dword ptr [esp + 5ch]
        push   dword ptr [esp + 54h]
        push   dword ptr [esp + 60h]
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 64h]
        push   dword ptr [esp + 38h]
        push   dword ptr [esp + 60h]
        call   sub_1_87D4
        add    esp, 20h
        cmp    eax, 103h
        je     NEAR PTR loc_1_8EEB
        or     eax, eax
        jne    SHORT loc_1_8E78
        push   dword ptr [esp + 2ch]
        push   eax
        push   dword ptr [esp + 28h]
        call   sub_1_8710
        add    esp, 0ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  08Bh, 0FFh
loc_1_8E78:
        db  0FFh, 005h, 004h, 0C2h, 000h, 000h
        push   dword ptr [esp + 20h]
        call   sub_1_833C
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  08Bh, 0FFh
loc_1_8E94:
        db  0FFh, 005h, 004h, 0C2h, 000h, 000h
        or     ebp, ebp
        je     SHORT loc_1_8EEB
        push   ebp
        call   sub_1_B944
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  090h
loc_1_8EB0:
        cmp    byte ptr [esp + 60h], 1
        je     SHORT loc_1_8EEB
        cmp    byte ptr [esp + 60h], 3
        je     SHORT loc_1_8EEB
        db  0FFh, 005h, 0E8h, 0C1h, 000h, 000h
        test   byte ptr [esp + 60h], 1
        jne    SHORT loc_1_8EEB
        cmp    byte ptr [esp + 60h], 0
        jne    SHORT loc_1_8EEB
        push   0
        push   5
        push   3
        push   dword ptr [esp + 54h]
        mov    ecx, dword ptr [esp + 54h]
        push   dword ptr [ecx + 4]
        call   sub_1_9610
        add    esp, 14h
loc_1_8EEB:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  090h
_LTEXT ends
        end
