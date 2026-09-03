; vserver_lcode_01_02.asm -- VSERVER object 1 segment _LTEXT  (part 2/5)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_4BBC:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   edi
        lea    edi, [ebp - 18h]
        sub    eax, eax
        mov    ecx, 6
        rep stosd
        mov    byte ptr [ebp - 7], 1
        push   dword ptr [ebp + 0ch]
        lea    eax, [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        call   dword ptr [eax]
        add    esp, 8
        pop    edi
        leave
        ret    0ch
        db  090h
sub_1_4BE8:
        push   ebp
        mov    ebp, esp
        sub    edx, edx
        db  0B9h, 048h, 022h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
loc_1_4BF5:
        cmp    dword ptr [ecx], eax
        je     SHORT loc_1_4C08
        add    ecx, 8
        inc    edx
        db  081h, 0F9h, 0A0h, 022h, 001h, 000h
        jb     SHORT loc_1_4BF5
        leave
        ret
        db  090h
loc_1_4C08:
        db  066h, 08Bh, 004h, 0D5h, 04Ch, 022h, 001h, 000h
        leave
        ret
        db  08Bh, 0FFh
sub_1_4C14:
        push   ebp
        mov    ebp, esp
        sub    esp, 100h
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebp - 100h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    edi, [ebp - 100h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        push   1
        push   0
        lea    eax, [ebp - 100h]
        push   eax
        push   eax
        call   sub_1_9288
        add    esp, 14h
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        lea    eax, [ebp - 100h]
        push   eax
        call   sub_1_9276
        add    esp, 0ch
        pop    edi
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_4C88:
        push   ebp
        mov    ebp, esp
        sub    esp, 38h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 0eh]
        mov    word ptr [ebp - 14h], ax
        cmp    ax, 1
        je     SHORT loc_1_4CB4
        cmp    ax, 32h
        je     SHORT loc_1_4CB4
        mov    eax, 7ch
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_4CB4:
        cmp    word ptr [ebp - 14h], 32h
        jne    SHORT loc_1_4CD0
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 11h], 1
        jne    SHORT loc_1_4CD0
        mov    eax, 41h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_4CD0:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 34h], eax
        db  066h, 08Bh, 00Dh, 0F1h, 024h, 001h, 000h
        mov    word ptr [eax], cx
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 0ah]
        mov    dword ptr [ebp - 8], ecx
        movzx  eax, word ptr [eax + 8]
        add    eax, ecx
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 2ch], 0
        mov    dword ptr [ebp - 30h], 0
        mov    dword ptr [ebp - 24h], 0
        sub    ecx, ecx
        cmp    dword ptr [ebp - 8], ecx
        jne    SHORT loc_1_4D18
        jmp    NEAR PTR loc_1_4F74
        db  08Dh, 049h, 000h
loc_1_4D18:
        db  08Bh, 01Dh, 000h, 01Ch, 001h, 000h
        db  083h, 03Dh, 0F1h, 024h, 001h, 000h, 000h
        je     NEAR PTR loc_1_4F7B
        mov    edx, dword ptr [ebp - 38h]
loc_1_4D2E:
        mov    edi, dword ptr [ebx + 34h]
        cmp    edi, ecx
        je     NEAR PTR loc_1_4F5B
        cmp    word ptr [ebp - 14h], 32h
        jne    SHORT loc_1_4D4C
        mov    edx, dword ptr [ebp - 8]
        add    dword ptr [ebp - 8], 2ah
        jmp    SHORT loc_1_4D56
        db  08Dh, 049h, 000h
loc_1_4D4C:
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 20h], ecx
        add    dword ptr [ebp - 8], 14h
loc_1_4D56:
        or     edi, edi
        je     SHORT loc_1_4D6C
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    dword ptr [ebp - 10h], ecx
        jmp    SHORT loc_1_4D73
        db  08Bh, 0FFh
loc_1_4D6C:
        mov    dword ptr [ebp - 10h], 1
loc_1_4D73:
        mov    eax, dword ptr [ebp - 10h]
        sub    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 18h], eax
        cmp    word ptr [ebp - 14h], 32h
        jne    SHORT loc_1_4DA4
        mov    edi, dword ptr [ebx + 2ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    dword ptr [ebp - 1ch], ecx
        mov    eax, ecx
        inc    eax
        sub    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
loc_1_4DA4:
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebp - 8]
        jb     NEAR PTR loc_1_4F74
        cmp    word ptr [ebp - 14h], 1
        jne    SHORT loc_1_4DD8
        mov    ecx, dword ptr [ebp - 20h]
        movsx  ax, byte ptr [ebx + 9]
        mov    word ptr [ecx + 0eh], ax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 10h], eax
        lea    esi, [ebx + 11h]
        mov    edi, ecx
        movsd
        movsd
        movsd
        movsb
        jmp    NEAR PTR loc_1_4F05
        db  090h
loc_1_4DD8:
        mov    al, byte ptr [ebx + 9]
        mov    byte ptr [edx + 0dh], al
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [edx + 10h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [edx + 14h], eax
        mov    edi, dword ptr [ebx + 2ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [ebp - 0ch]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        cmp    byte ptr [ebx + 9], 0
        jne    SHORT loc_1_4E24
        cmp    dword ptr [ebp - 1ch], 3
        jbe    SHORT loc_1_4E24
        mov    ecx, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + ecx - 1], 0
loc_1_4E24:
        mov    edi, edx
        lea    esi, [ebx + 11h]
        movsd
        movsd
        movsd
        movsb
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 34h]
        cmp    dword ptr [eax + 34h], ecx
        jne    SHORT loc_1_4E88
        mov    word ptr [edx + 0eh], 3
        lea    edi, [ebx + 1eh]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [edx + 18h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [ebx]
        add    edi, 1eh
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [edx + 21h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        jmp    SHORT loc_1_4EED
        db  090h
loc_1_4E88:
        lea    eax, [edx + 0eh]
        cmp    byte ptr [ebx + 10h], 89h
        jne    SHORT loc_1_4EC0
        mov    word ptr [edx + 0eh], 1
        lea    edi, [ebx + 1eh]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [edx + 21h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    byte ptr [edx + 18h], 0
        jmp    SHORT loc_1_4EED
loc_1_4EC0:
        lea    edi, [ebx + 1eh]
        mov    word ptr [edx + 0eh], 2
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [edx + 18h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    byte ptr [edx + 21h], 0
loc_1_4EED:
        mov    ax, word ptr [ebx + 40h]
        test   al, 1
        je     SHORT loc_1_4EFB
        or     word ptr [edx + 0eh], 100h
loc_1_4EFB:
        test   al, 4
        je     SHORT loc_1_4F05
        or     word ptr [edx + 0eh], 200h
loc_1_4F05:
        cmp    dword ptr [ebp - 10h], 1
        jne    SHORT loc_1_4F14
        mov    eax, dword ptr [ebp - 18h]
        mov    byte ptr [eax], 0
        jmp    SHORT loc_1_4F2B
        db  090h
loc_1_4F14:
        mov    eax, dword ptr [ebp - 10h]
        mov    edi, dword ptr [ebp - 18h]
        mov    esi, dword ptr [ebx + 34h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_1_4F2B:
        cmp    word ptr [ebp - 14h], 32h
        jne    SHORT loc_1_4F55
        mov    edi, dword ptr [ebp - 0ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [edx + 14h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_1_4F55:
        mov    ecx, dword ptr [ebx + 34h]
        inc    dword ptr [ebp - 2ch]
loc_1_4F5B:
        mov    ebx, dword ptr [ebx]
        inc    dword ptr [ebp - 24h]
        mov    eax, dword ptr [ebp - 24h]
        db  039h, 005h, 0F1h, 024h, 001h, 000h
        ja     NEAR PTR loc_1_4D2E
        jmp    SHORT loc_1_4F7B
        db  08Dh, 049h, 000h
loc_1_4F74:
        mov    dword ptr [ebp - 30h], 0eah
loc_1_4F7B:
        cmp    dword ptr [ebp - 30h], 0
        jne    SHORT loc_1_4F8B
        mov    eax, dword ptr [ebp - 34h]
        mov    cx, word ptr [ebp - 2ch]
        mov    word ptr [eax], cx
loc_1_4F8B:
        pop    edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    cx, word ptr [ebp - 2ch]
        mov    word ptr [eax], cx
        mov    eax, dword ptr [ebp - 30h]
        pop    esi
        pop    ebx
        leave
        ret
sub_1_4FA0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        mov    edi, dword ptr [eax]
        mov    edx, dword ptr [ebp + 10h]
        sub    edi, edx
        cmp    dword ptr [ebp + 14h], edi
        ja     SHORT loc_1_4FD0
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        pop    edi
        pop    esi
        leave
        ret
loc_1_4FD0:
        pop    edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_4FE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [ebx + 4]
        add    eax, dword ptr [ebx + 6]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0ch], 0
        mov    esi, 0
        je     SHORT loc_1_500F
        movsx  eax, byte ptr [eax + 12h]
        db  08Bh, 034h, 085h, 0B0h, 01Bh, 001h, 000h
loc_1_500F:
        db  083h, 03Dh, 020h, 031h, 001h, 000h, 000h
        jne    SHORT loc_1_5024
        mov    eax, 842h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_5024:
        mov    ax, word ptr [ebx + 0ah]
        cmp    ax, 1
        je     SHORT loc_1_5044
        cmp    ax, 0bh
        je     SHORT loc_1_5044
        cmp    ax, 32h
        je     SHORT loc_1_5044
        mov    eax, 7ch
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_5044:
        db  08Bh, 00Dh, 020h, 031h, 001h, 000h
        mov    ecx, dword ptr [ecx + 10h]
        mov    dword ptr [ebp - 4], ecx
        cmp    ax, 32h
        jne    SHORT loc_1_50C8
        mov    edi, 2ah
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        je     SHORT loc_1_5090
        lea    eax, [ebp - 28h]
        push   eax
        call   sub_1_1FB1
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_5080
        mov    eax, 85ch
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_5080:
        mov    eax, dword ptr [ebp - 24h]
        add    eax, dword ptr [ebp - 1ch]
        add    eax, dword ptr [ebp - 14h]
        add    dword ptr [ebp - 4], eax
        jmp    SHORT loc_1_50D8
        db  08Bh, 0FFh
loc_1_5090:
        mov    dword ptr [ebp - 28h], 0
        db  0C7h, 045h, 0F0h, 0A0h, 022h, 001h, 000h
        db  0C7h, 045h, 0E8h, 0A0h, 022h, 001h, 000h
        db  0C7h, 045h, 0E0h, 0A0h, 022h, 001h, 000h
        mov    dword ptr [ebp - 14h], 1
        mov    dword ptr [ebp - 1ch], 1
        mov    dword ptr [ebp - 24h], 1
        add    dword ptr [ebp - 4], 3
        jmp    SHORT loc_1_50D8
        db  090h
loc_1_50C8:
        mov    edi, 4
        cmp    ax, 0bh
        je     SHORT loc_1_50D8
        mov    edi, 1ah
loc_1_50D8:
        mov    eax, dword ptr [ebx]
        add    dword ptr [ebp - 4], edi
        mov    cx, word ptr [ebp - 4]
        mov    word ptr [eax], cx
        movzx  eax, word ptr [ebx + 4]
        cmp    eax, edi
        jae    SHORT loc_1_50F8
        mov    eax, 84bh
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_50F8:
        or     esi, esi
        mov    eax, dword ptr [ebx + 6]
        lea    ecx, [eax + edi]
        mov    dword ptr [ebp - 8], ecx
        jne    SHORT loc_1_512C
        mov    edx, 1
        inc    ecx
        db  0B9h, 0B0h, 01Bh, 001h, 000h
loc_1_5110:
        mov    esi, dword ptr [ecx]
        cmp    esi, 0fffff000h
        je     SHORT loc_1_511D
        or     edx, dword ptr [esi + 18h]
loc_1_511D:
        add    ecx, 4
        db  081h, 0F9h, 0F4h, 01Bh, 001h, 000h
        jb     SHORT loc_1_5110
        jmp    SHORT loc_1_512F
        db  08Bh, 0FFh
loc_1_512C:
        mov    edx, dword ptr [esi + 18h]
loc_1_512F:
        cmp    word ptr [ebx + 0ah], 0bh
        jne    SHORT loc_1_5140
        mov    dword ptr [eax], edx
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_5140:
        mov    edi, eax
        db  0BEh, 092h, 024h, 001h, 000h
        movsd
        movsd
        movsd
        movsd
        mov    byte ptr [eax + 10h], 4
        mov    byte ptr [eax + 11h], 0
        mov    dword ptr [eax + 12h], edx
        lea    ecx, [ebp - 0ch]
        push   ecx
        push   dword ptr [ebp - 8]
        db  08Bh, 00Dh, 020h, 031h, 001h, 000h
        push   dword ptr [ecx + 10h]
        push   dword ptr [ecx + 0ch]
        add    eax, 16h
        push   eax
        call   sub_1_4FA0
        add    esp, 14h
        cmp    word ptr [ebx + 0ah], 32h
        jne    SHORT loc_1_51E9
        mov    esi, dword ptr [ebx + 6]
        mov    eax, dword ptr [ebp - 28h]
        mov    word ptr [esi + 1ah], ax
        db  066h, 0A1h, 0A2h, 023h, 001h, 000h
        and    ax, 2
        or     ax, ax
        mov    ax, 0
        setne  al
        mov    word ptr [esi + 1ch], ax
        lea    eax, [ebp - 0ch]
        push   eax
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 20h]
        lea    eax, [esi + 1eh]
        push   eax
        call   sub_1_4FA0
        add    esp, 14h
        lea    eax, [ebp - 0ch]
        push   eax
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        lea    eax, [esi + 22h]
        push   eax
        call   sub_1_4FA0
        add    esp, 14h
        lea    eax, [ebp - 0ch]
        push   eax
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 10h]
        add    esi, 26h
        push   esi
        call   sub_1_4FA0
        add    esp, 14h
loc_1_51E9:
        mov    eax, 0eah
        movzx  ecx, word ptr [ebx + 4]
        cmp    ecx, dword ptr [ebp - 4]
        jb     SHORT loc_1_51F9
        sub    eax, eax
loc_1_51F9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
sub_1_5200:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    di, word ptr [esi]
        cmp    di, 7b2h
        jae    SHORT loc_1_5220
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_5220:
        movzx  ecx, di
        sub    ecx, 7b2h
        lea    ebx, [ecx + 1]
        shr    ebx, 2
        lea    eax, [ecx + 45h]
        mov    dword ptr [ebp - 8], 64h
        sub    edx, edx
        div    dword ptr [ebp - 8]
        sub    ebx, eax
        lea    eax, [ecx + 171h]
        mov    dword ptr [ebp - 4], 190h
        sub    edx, edx
        div    dword ptr [ebp - 4]
        add    eax, ebx
        lea    edx, [ecx + ecx*8]
        lea    edx, [ecx + edx*8]
        lea    edx, [edx + edx*4]
        add    edx, eax
        lea    edx, [edx + edx*4]
        lea    edx, [edx + edx*8]
        lea    edx, [edx + edx*2]
        lea    edx, [edx + edx*4]
        shl    edx, 7
        movzx  eax, word ptr [esi + 2]
        db  08Bh, 004h, 085h, 0A0h, 022h, 001h, 000h
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*8]
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        shl    eax, 7
        lea    ecx, [eax + edx]
        test   di, 3
        jne    SHORT loc_1_52C3
        mov    bx, 64h
        mov    ax, di
        sub    dx, dx
        div    bx
        or     dx, dx
        jne    SHORT loc_1_52B6
        mov    bx, 190h
        mov    ax, di
        sub    dx, dx
        div    bx
        or     dx, dx
        jne    SHORT loc_1_52C3
loc_1_52B6:
        cmp    word ptr [esi + 2], 2
        jbe    SHORT loc_1_52C3
        add    ecx, 15180h
loc_1_52C3:
        movzx  edx, word ptr [esi + 6]
        lea    edx, [edx + edx*2]
        movzx  eax, word ptr [esi + 8]
        lea    edx, [eax + edx*8]
        shl    edx, 2
        lea    edx, [edx + edx*2]
        lea    edx, [edx + edx*4]
        movzx  eax, word ptr [esi + 0ah]
        add    edx, eax
        shl    edx, 2
        lea    edx, [edx + edx*2]
        lea    edx, [edx + edx*4]
        movzx  eax, word ptr [esi + 0ch]
        add    edx, eax
        lea    eax, [ecx + edx - 15180h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
sub_1_52FC:
        push   ebp
        mov    ebp, esp
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_1_5200
        add    esp, 4
        mov    esi, eax
        call   sub_1_9D2F
        sub    esi, eax
        mov    eax, esi
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_531C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        or     esi, esi
        jne    SHORT loc_1_533C
        sub    eax, eax
        mov    edx, dword ptr [ebp + 1ch]
        cmp    dword ptr [edx], eax
        jne    SHORT loc_1_5393
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        pop    edi
        pop    esi
        leave
        ret
        db  08Bh, 0FFh
loc_1_533C:
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, ecx
        mov    ecx, dword ptr [ebp + 18h]
        add    dword ptr [ecx], eax
        mov    edx, dword ptr [ebp + 1ch]
        cmp    dword ptr [edx], 0
        mov    ecx, dword ptr [ebp + 14h]
        jne    SHORT loc_1_5368
        cmp    dword ptr [ecx], eax
        jb     SHORT loc_1_5368
        mov    dword ptr [edx], 0
        jmp    SHORT loc_1_536E
        db  090h
loc_1_5368:
        mov    dword ptr [edx], 1
loc_1_536E:
        cmp    dword ptr [edx], 0
        jne    SHORT loc_1_5393
        mov    edi, dword ptr [ecx]
        sub    edi, eax
        mov    dword ptr [ecx], edi
        mov    edx, dword ptr [ebp + 10h]
        add    edx, edi
        mov    edi, edx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], edx
loc_1_5393:
        pop    edi
        pop    esi
        leave
        ret
        db  090h
sub_1_5398:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 1ch]
        push   ebx
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_531C
        add    esp, 18h
        mov    edi, eax
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_1_53D6
        or     edi, edi
        je     SHORT loc_1_53D6
        mov    eax, dword ptr [esi]
        push   edi
        push   0
        push   1
        push   eax
        push   eax
        call   sub_1_9288
        add    esp, 14h
loc_1_53D6:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_53E0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        test   cl, 1
        je     SHORT loc_1_53F4
        mov    ax, 1
        jmp    SHORT loc_1_540C
        db  08Dh, 049h, 000h
loc_1_53F4:
        test   cl, 8
        je     SHORT loc_1_5400
        mov    ax, 2
        jmp    SHORT loc_1_540C
        db  090h
loc_1_5400:
        test   cl, 10h
        mov    ax, 3
        jne    SHORT loc_1_540C
        sub    ax, ax
loc_1_540C:
        test   cl, 80h
        je     SHORT loc_1_5415
        or     ax, 4
loc_1_5415:
        test   cl, 40h
        je     SHORT loc_1_541E
        or     ax, 100h
loc_1_541E:
        test   cl, 20h
        je     SHORT loc_1_5427
        or     ax, 20h
loc_1_5427:
        test   cl, 4
        je     SHORT loc_1_5430
        or     ax, 8000h
loc_1_5430:
        test   cl, 2
        je     SHORT loc_1_5439
        or     ax, 10h
loc_1_5439:
        leave
        ret
        db  090h
sub_1_543C:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        mov    ebx, dword ptr [eax]
        mov    eax, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebp - 18h], ecx
        mov    dword ptr [ebp - 1ch], 0
        mov    dword ptr [ebp - 20h], 0
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [eax], 0
        cmp    dword ptr [ebp + 0ch], 3
        jle    SHORT loc_1_547C
        mov    esi, 7ch
        jmp    NEAR PTR loc_1_5AAB
        db  08Dh, 049h, 000h
loc_1_547C:
        push   400h
        call   sub_1_8CB6
        add    esp, 4
        mov    dword ptr [ebp - 1ch], eax
        or     eax, eax
        jne    NEAR PTR loc_1_59A9
        mov    esi, 8
        jmp    NEAR PTR loc_1_5AAB
        db  08Bh, 0FFh
loc_1_54A0:
        mov    eax, dword ptr [ebp + 20h]
        add    dword ptr [eax], 2
        mov    ecx, dword ptr [ebp + 24h]
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_54BC
        cmp    dword ptr [ebp - 8], 2
        jl     SHORT loc_1_54BC
        mov    dword ptr [ecx], 0
        jmp    SHORT loc_1_54C5
loc_1_54BC:
        mov    ecx, dword ptr [ebp + 24h]
        mov    dword ptr [ecx], 1
loc_1_54C5:
        mov    ecx, dword ptr [ebp + 24h]
        cmp    dword ptr [ecx], 0
        jne    NEAR PTR loc_1_5977
        sub    dword ptr [ebp - 8], 2
        lea    eax, [ebx + 2]
        mov    dword ptr [ebp - 0ch], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx]
        mov    word ptr [ebx], ax
        jmp    NEAR PTR loc_1_5977
loc_1_54E8:
        mov    eax, dword ptr [ebp + 20h]
        add    dword ptr [eax], 4ah
        mov    ecx, dword ptr [ebp + 24h]
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_5504
        cmp    dword ptr [ebp - 8], 4ah
        jl     SHORT loc_1_5504
        mov    dword ptr [ecx], 0
        jmp    SHORT loc_1_550D
loc_1_5504:
        mov    ecx, dword ptr [ebp + 24h]
        mov    dword ptr [ecx], 1
loc_1_550D:
        mov    ecx, dword ptr [ebp + 24h]
        cmp    dword ptr [ecx], 0
        jne    NEAR PTR loc_1_565B
        sub    dword ptr [ebp - 8], 4ah
        lea    eax, [ebx + 4ah]
        mov    dword ptr [ebp - 0ch], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx]
        mov    word ptr [ebx], ax
        mov    edi, dword ptr [ecx + 0ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        cmp    ecx, 14h
        mov    ecx, 14h
        ja     SHORT loc_1_5554
        mov    ecx, dword ptr [ebp - 4]
        mov    edi, dword ptr [ecx + 0ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
loc_1_5554:
        mov    eax, ecx
        lea    edx, [ebx + 2]
        mov    edi, edx
        mov    ecx, dword ptr [ebp - 4]
        mov    esi, dword ptr [ecx + 0ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    byte ptr [ebx + 16h], 0
        push   14h
        push   0
        push   1
        push   edx
        push   edx
        call   sub_1_9288
        add    esp, 14h
        mov    ecx, dword ptr [ebp - 4]
        mov    edi, dword ptr [ecx + 14h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        cmp    ecx, 0fh
        mov    ecx, 0fh
        ja     SHORT loc_1_55AF
        mov    ecx, dword ptr [ebp - 4]
        mov    edi, dword ptr [ecx + 14h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
loc_1_55AF:
        mov    edx, ecx
        lea    eax, [ebx + 18h]
        mov    edi, eax
        mov    ecx, dword ptr [ebp - 4]
        mov    esi, dword ptr [ecx + 14h]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    byte ptr [ebx + 27h], 0
        push   0fh
        push   0
        push   1
        push   eax
        push   eax
        call   sub_1_9288
        add    esp, 14h
        mov    ecx, dword ptr [ebp - 4]
        mov    edi, dword ptr [ecx + 18h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        cmp    ecx, 9
        mov    eax, 9
        ja     SHORT loc_1_560C
        mov    ecx, dword ptr [ebp - 4]
        mov    edi, dword ptr [ecx + 18h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, ecx
loc_1_560C:
        lea    edi, [ebx + 28h]
        mov    ecx, dword ptr [ebp - 4]
        mov    esi, dword ptr [ecx + 18h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    byte ptr [ebx + 31h], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 3ch]
        mov    word ptr [ebx + 36h], ax
        push   dword ptr [ecx + 34h]
        call   sub_1_53E0
        add    esp, 4
        mov    word ptr [ebx + 38h], ax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 50h
        push   eax
        call   sub_1_52FC
        add    esp, 4
        mov    dword ptr [ebx + 3eh], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 4ch]
        mov    dword ptr [ebx + 42h], eax
loc_1_565B:
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        push   dword ptr [ecx + 20h]
        lea    eax, [ebx + 32h]
        push   eax
        call   sub_1_531C
        add    esp, 18h
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        push   dword ptr [ecx + 2ch]
        lea    eax, [ebx + 3ah]
        push   eax
        call   sub_1_5398
        add    esp, 18h
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        push   dword ptr [ecx + 10h]
        lea    eax, [ebx + 46h]
        jmp    NEAR PTR loc_1_596E
loc_1_56B4:
        mov    eax, dword ptr [ebp + 20h]
        add    dword ptr [eax], 1ch
        mov    ecx, dword ptr [ebp + 24h]
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_56D0
        cmp    dword ptr [ebp - 8], 1ch
        jl     SHORT loc_1_56D0
        mov    dword ptr [ecx], 0
        jmp    SHORT loc_1_56D9
loc_1_56D0:
        mov    ecx, dword ptr [ebp + 24h]
        mov    dword ptr [ecx], 1
loc_1_56D9:
        mov    ecx, dword ptr [ebp + 24h]
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_572B
        sub    dword ptr [ebp - 8], 1ch
        lea    eax, [ebx + 1ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx]
        mov    word ptr [ebx], ax
        mov    eax, dword ptr [ecx + 38h]
        mov    word ptr [ebx + 2], ax
        mov    eax, dword ptr [ecx + 3ch]
        mov    word ptr [ebx + 8], ax
        push   dword ptr [ecx + 34h]
        call   sub_1_53E0
        add    esp, 4
        mov    word ptr [ebx + 0ah], ax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 50h
        push   eax
        call   sub_1_52FC
        add    esp, 4
        mov    dword ptr [ebx + 0ch], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 4ch]
        mov    dword ptr [ebx + 10h], eax
loc_1_572B:
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        push   dword ptr [ecx + 0ch]
        lea    eax, [ebx + 4]
        push   eax
        call   sub_1_5398
        add    esp, 18h
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        push   dword ptr [ecx + 10h]
        lea    eax, [ebx + 14h]
        push   eax
        call   sub_1_5398
        add    esp, 18h
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        push   dword ptr [ecx + 10h]
        lea    eax, [ebx + 18h]
        jmp    NEAR PTR loc_1_596E
loc_1_5784:
        mov    eax, dword ptr [ebp + 20h]
        add    dword ptr [eax], 44h
        mov    ecx, dword ptr [ebp + 24h]
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_57A0
        cmp    dword ptr [ebp - 8], 44h
        jl     SHORT loc_1_57A0
        mov    dword ptr [ecx], 0
        jmp    SHORT loc_1_57A9
loc_1_57A0:
        mov    ecx, dword ptr [ebp + 24h]
        mov    dword ptr [ecx], 1
loc_1_57A9:
        mov    ecx, dword ptr [ebp + 24h]
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_5802
        sub    dword ptr [ebp - 8], 44h
        lea    eax, [ebx + 44h]
        mov    dword ptr [ebp - 0ch], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx]
        mov    word ptr [ebx], ax
        mov    eax, dword ptr [ecx + 38h]
        mov    word ptr [ebx + 2], ax
        mov    eax, dword ptr [ecx + 3ch]
        mov    word ptr [ebx + 8], ax
        push   dword ptr [ecx + 34h]
        call   sub_1_53E0
        add    esp, 4
        mov    word ptr [ebx + 0ah], ax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 50h
        push   eax
        call   sub_1_52FC
        add    esp, 4
        mov    dword ptr [ebx + 0ch], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 4ch]
        mov    dword ptr [ebx + 10h], eax
        mov    dword ptr [ebx + 3ch], 0
loc_1_5802:
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        push   dword ptr [ecx + 0ch]
        lea    eax, [ebx + 4]
        push   eax
        call   sub_1_5398
        add    esp, 18h
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        push   dword ptr [ecx + 10h]
        lea    eax, [ebx + 14h]
        push   eax
        call   sub_1_5398
        add    esp, 18h
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        push   dword ptr [ecx + 10h]
        lea    eax, [ebx + 18h]
        push   eax
        call   sub_1_5398
        add    esp, 18h
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        push   dword ptr [ecx + 14h]
        lea    eax, [ebx + 1ch]
        push   eax
        call   sub_1_5398
        add    esp, 18h
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        push   dword ptr [ecx + 18h]
        lea    eax, [ebx + 20h]
        push   eax
        call   sub_1_531C
        add    esp, 18h
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        push   dword ptr [ecx + 20h]
        lea    eax, [ebx + 24h]
        push   eax
        call   sub_1_531C
        add    esp, 18h
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        push   dword ptr [ecx + 2ch]
        lea    eax, [ebx + 28h]
        push   eax
        call   sub_1_5398
        add    esp, 18h
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 11h
        push   eax
        lea    eax, [ebx + 2ch]
        push   eax
        call   sub_1_531C
        add    esp, 18h
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        push   dword ptr [ecx + 1ch]
        lea    eax, [ebx + 30h]
        push   eax
        call   sub_1_531C
        add    esp, 18h
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        push   dword ptr [ecx + 20h]
        lea    eax, [ebx + 34h]
        push   eax
        call   sub_1_531C
        add    esp, 18h
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        push   dword ptr [ecx + 24h]
        lea    eax, [ebx + 38h]
        push   eax
        call   sub_1_531C
        add    esp, 18h
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        push   dword ptr [ecx + 4]
        lea    eax, [ebx + 40h]
loc_1_596E:
        push   eax
        call   sub_1_5398
        add    esp, 18h
loc_1_5977:
        mov    ecx, dword ptr [ebp + 24h]
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_598D
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 18h], ecx
        mov    ebx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp + 14h]
        inc    dword ptr [eax]
loc_1_598D:
        inc    dword ptr [ebp - 14h]
        add    dword ptr [ebp - 4], 68h
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [ebp - 10h], eax
        ja     NEAR PTR loc_1_5A3A
loc_1_59A0:
        mov    eax, dword ptr [ebp - 20h]
        add    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 20h], eax
loc_1_59A9:
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 10h], 0
        push   eax
        lea    eax, [ebp - 24h]
        push   eax
        push   400h
        push   dword ptr [ebp - 1ch]
        push   2
        push   2710h
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp + 8]
        call   sub_1_9270
        add    esp, 20h
        cmp    eax, 7ah
        jne    SHORT loc_1_5A21
        push   dword ptr [ebp - 1ch]
        call   sub_1_8CF5
        add    esp, 4
        push   dword ptr [ebp - 24h]
        call   sub_1_8CB6
        add    esp, 4
        mov    dword ptr [ebp - 1ch], eax
        or     eax, eax
        je     SHORT loc_1_5A70
        mov    dword ptr [ebp - 10h], 0
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 24h]
        push   eax
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 1ch]
        push   2
        push   2710h
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp + 8]
        call   sub_1_9270
        add    esp, 20h
loc_1_5A21:
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_5A78
        mov    dword ptr [ebp - 14h], 0
        jbe    NEAR PTR loc_1_59A0
        mov    ecx, dword ptr [ebp - 1ch]
        mov    dword ptr [ebp - 4], ecx
loc_1_5A3A:
        mov    dword ptr [ebp - 0ch], ebx
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebp + 0ch]
        or     eax, eax
        je     NEAR PTR loc_1_54A0
        cmp    eax, 1
        je     NEAR PTR loc_1_54E8
        cmp    eax, 2
        je     NEAR PTR loc_1_56B4
        cmp    eax, 3
        je     NEAR PTR loc_1_5784
        jmp    NEAR PTR loc_1_5977
        db  08Bh, 0FFh
loc_1_5A70:
        mov    esi, 8
        jmp    SHORT loc_1_5AAB
        db  090h
loc_1_5A78:
        or     eax, eax
        jne    SHORT loc_1_5A9C
        mov    ecx, dword ptr [ebp + 24h]
        cmp    dword ptr [ecx], 0
        je     SHORT loc_1_5A9C
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_5A94
        mov    esi, 0eah
        jmp    SHORT loc_1_5AAB
        db  090h
loc_1_5A94:
        mov    esi, 7ah
        jmp    SHORT loc_1_5AAB
        db  090h
loc_1_5A9C:
        mov    esi, eax
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], ebx
        mov    eax, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [eax], ecx
loc_1_5AAB:
        cmp    dword ptr [ebp - 1ch], 0
        je     SHORT loc_1_5ABC
        push   dword ptr [ebp - 1ch]
        call   sub_1_8CF5
        add    esp, 4
loc_1_5ABC:
        push   esi
        call   sub_1_4BE8
        add    esp, 4
        and    eax, 0ffffh
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
sub_1_5AD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        mov    dword ptr [ebp - 4], 400h
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [esi], 7ah
        sub    ebx, ebx
loc_1_5AED:
        cmp    ebx, 0ah
        jae    SHORT loc_1_5B3A
        or     edi, edi
        je     SHORT loc_1_5AFF
        push   edi
        call   sub_1_8CF5
        add    esp, 4
loc_1_5AFF:
        push   dword ptr [ebp - 4]
        call   sub_1_8CB6
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_5B34
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 4]
        push   edi
        push   3
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_925E
        add    esp, 18h
        mov    dword ptr [esi], eax
        inc    ebx
        cmp    eax, 7ah
        je     SHORT loc_1_5AED
        jmp    SHORT loc_1_5B3A
        db  08Dh, 049h, 000h
loc_1_5B34:
        mov    dword ptr [esi], 8
loc_1_5B3A:
        cmp    dword ptr [esi], 0
        je     SHORT loc_1_5B4E
        or     edi, edi
        je     SHORT loc_1_5B4E
        push   edi
        call   sub_1_8CF5
        add    esp, 4
        sub    edi, edi
loc_1_5B4E:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_5B58:
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 14h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 10h], 0
        mov    dword ptr [ebp - 18h], 0
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    ebx, 5
        jle    SHORT loc_1_5BA4
        cmp    ebx, 32h
        jl     SHORT loc_1_5B98
        cmp    ebx, 34h
        jle    SHORT loc_1_5BA4
loc_1_5B98:
        mov    esi, 7ch
        jmp    NEAR PTR loc_1_6472
        db  08Bh, 0FFh
loc_1_5BA4:
        mov    esi, dword ptr [ebp + 10h]
        add    esi, 11h
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    dword ptr [ebp - 34h], ecx
        or     ebx, ebx
        jle    SHORT loc_1_5BC3
        cmp    ebx, 5
        jl     SHORT loc_1_5BCC
loc_1_5BC3:
        cmp    ebx, 33h
        jne    NEAR PTR loc_1_5C68
loc_1_5BCC:
        push   400h
        call   sub_1_8CB6
        add    esp, 4
        mov    dword ptr [ebp - 0ch], eax
        or     eax, eax
        jne    SHORT loc_1_5BEC
        mov    esi, 8
        jmp    NEAR PTR loc_1_6472
        db  08Bh, 0FFh
loc_1_5BEC:
        lea    eax, [ebp - 30h]
        push   eax
        push   400h
        push   dword ptr [ebp - 0ch]
        push   2
        push   dword ptr [ebp + 8]
        call   sub_1_9258
        add    esp, 14h
        mov    dword ptr [ebp - 20h], eax
        cmp    eax, 7ah
        jne    SHORT loc_1_5C4E
        push   dword ptr [ebp - 0ch]
        call   sub_1_8CF5
        add    esp, 4
        push   dword ptr [ebp - 30h]
        call   sub_1_8CB6
        add    esp, 4
        mov    dword ptr [ebp - 0ch], eax
        or     eax, eax
        jne    SHORT loc_1_5C34
        mov    esi, 8
        jmp    NEAR PTR loc_1_6472
loc_1_5C34:
        lea    eax, [ebp - 30h]
        push   eax
        push   dword ptr [ebp - 30h]
        push   dword ptr [ebp - 0ch]
        push   2
        push   dword ptr [ebp + 8]
        call   sub_1_9258
        add    esp, 14h
        mov    dword ptr [ebp - 20h], eax
loc_1_5C4E:
        cmp    dword ptr [ebp - 20h], 0
        je     SHORT loc_1_5C68
        push   dword ptr [ebp - 20h]
        call   sub_1_4BE8
        add    esp, 4
        movzx  esi, ax
        jmp    NEAR PTR loc_1_6472
        db  090h
loc_1_5C68:
        cmp    ebx, 5
        jg     SHORT loc_1_5CAC
        je     NEAR PTR loc_1_5F74
        or     ebx, ebx
        je     SHORT loc_1_5CCC
        cmp    ebx, 1
        jl     SHORT loc_1_5C93
        cmp    ebx, 2
        jle    NEAR PTR loc_1_5D10
        cmp    ebx, 3
        jl     SHORT loc_1_5C93
        cmp    ebx, 4
        jle    NEAR PTR loc_1_5E3C
loc_1_5C93:
        mov    ebx, dword ptr [ebp + 20h]
loc_1_5C96:
        cmp    dword ptr [ebx], 0
        je     NEAR PTR loc_1_6460
loc_1_5C9F:
        mov    esi, 7ah
        jmp    NEAR PTR loc_1_6472
        db  08Dh, 049h, 000h
loc_1_5CAC:
        cmp    ebx, 32h
        je     NEAR PTR loc_1_5FC0
        cmp    ebx, 33h
        je     NEAR PTR loc_1_61B0
        cmp    ebx, 34h
        je     NEAR PTR loc_1_6210
        jmp    SHORT loc_1_5C93
        db  08Dh, 049h, 000h
loc_1_5CCC:
        mov    eax, dword ptr [ebp + 1ch]
        add    dword ptr [eax], 0dh
        mov    ebx, dword ptr [ebp + 20h]
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_1_5CE6
        cmp    dword ptr [ebp - 4], 0dh
        mov    dword ptr [ebx], 0
        jge    SHORT loc_1_5CEC
loc_1_5CE6:
        mov    dword ptr [ebx], 1
loc_1_5CEC:
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_1_5C9F
        mov    eax, dword ptr [ebp - 34h]
        mov    edi, dword ptr [ebp - 8]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        sub    dword ptr [ebp - 4], 0dh
        add    dword ptr [ebp - 8], 0dh
        jmp    SHORT loc_1_5C96
        db  090h
loc_1_5D10:
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 14h], ecx
        mov    eax, dword ptr [ebp + 1ch]
        add    dword ptr [eax], 2ch
        mov    ebx, dword ptr [ebp + 20h]
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_1_5D30
        cmp    dword ptr [ebp - 4], 2ch
        mov    dword ptr [ebx], 0
        jge    SHORT loc_1_5D36
loc_1_5D30:
        mov    dword ptr [ebx], 1
loc_1_5D36:
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_1_5DB7
        sub    dword ptr [ebp - 4], 2ch
        mov    eax, dword ptr [ebp - 34h]
        mov    edi, dword ptr [ebp - 14h]
        mov    ecx, eax
        shr    ecx, 2
        add    dword ptr [ebp - 8], 2ch
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 38h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    word ptr [ecx + 0eh], ax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 40h]
        mov    word ptr [ecx + 10h], ax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 44h]
        mov    word ptr [ecx + 12h], ax
        mov    dword ptr [ecx + 1ch], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 48h]
        cmp    eax, 1
        jne    SHORT loc_1_5D94
        mov    word ptr [ecx + 28h], 1
        jmp    SHORT loc_1_5DAA
        db  08Bh, 0FFh
loc_1_5D94:
        cmp    eax, 4
        mov    ecx, dword ptr [ebp - 14h]
        jne    SHORT loc_1_5DA4
        mov    word ptr [ecx + 28h], 3
        jmp    SHORT loc_1_5DAA
loc_1_5DA4:
        mov    word ptr [ecx + 28h], 0
loc_1_5DAA:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4ch]
        mov    ecx, dword ptr [ebp - 14h]
        mov    word ptr [ecx + 2ah], ax
loc_1_5DB7:
        push   ebx
        push   dword ptr [ebp + 1ch]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0ch]
        push   dword ptr [eax + 20h]
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 14h
        push   eax
        call   sub_1_531C
        add    esp, 18h
        push   ebx
        push   dword ptr [ebp + 1ch]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0ch]
        push   dword ptr [eax + 24h]
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 18h
        push   eax
        call   sub_1_531C
        add    esp, 18h
        push   ebx
        push   dword ptr [ebp + 1ch]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0ch]
        push   dword ptr [eax + 2ch]
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 20h
        push   eax
        call   sub_1_531C
        add    esp, 18h
        push   ebx
        push   dword ptr [ebp + 1ch]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0ch]
        push   dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 24h
        push   eax
        call   sub_1_5398
        add    esp, 18h
        jmp    NEAR PTR loc_1_5C96
loc_1_5E3C:
        mov    edi, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 1ch]
        add    dword ptr [eax], 2ch
        mov    ebx, dword ptr [ebp + 20h]
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_1_5E59
        cmp    dword ptr [ebp - 4], 2ch
        mov    dword ptr [ebx], 0
        jge    SHORT loc_1_5E5F
loc_1_5E59:
        mov    dword ptr [ebx], 1
loc_1_5E5F:
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_1_5EC9
        sub    dword ptr [ebp - 4], 2ch
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 38h]
        add    dword ptr [ebp - 8], 2ch
        mov    word ptr [edi + 4], ax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 40h]
        mov    word ptr [edi + 6], ax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 44h]
        mov    word ptr [edi + 8], ax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 48h]
        cmp    eax, 1
        jne    SHORT loc_1_5EA0
        mov    word ptr [edi + 1ch], 1
        jmp    SHORT loc_1_5EB1
        db  08Dh, 049h, 000h
loc_1_5EA0:
        cmp    eax, 4
        mov    word ptr [edi + 1ch], 3
        je     SHORT loc_1_5EB1
        mov    word ptr [edi + 1ch], 0
loc_1_5EB1:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4ch]
        mov    word ptr [edi + 1eh], ax
        mov    dword ptr [edi + 20h], 0
        mov    dword ptr [edi + 28h], 0
loc_1_5EC9:
        push   ebx
        push   dword ptr [ebp + 1ch]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 8]
        push   esi
        push   edi
        call   sub_1_531C
        add    esp, 18h
        push   ebx
        push   dword ptr [ebp + 1ch]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0ch]
        push   dword ptr [eax + 20h]
        lea    eax, [edi + 0ch]
        push   eax
        call   sub_1_531C
        add    esp, 18h
        push   ebx
        push   dword ptr [ebp + 1ch]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0ch]
        push   dword ptr [eax + 24h]
        lea    eax, [edi + 10h]
        push   eax
        call   sub_1_531C
        add    esp, 18h
        push   ebx
        push   dword ptr [ebp + 1ch]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0ch]
        push   dword ptr [eax + 2ch]
        lea    eax, [edi + 14h]
        push   eax
        call   sub_1_531C
        add    esp, 18h
        push   ebx
        push   dword ptr [ebp + 1ch]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0ch]
        push   dword ptr [eax + 14h]
        lea    eax, [edi + 18h]
        push   eax
        call   sub_1_5398
        add    esp, 18h
        push   ebx
        push   dword ptr [ebp + 1ch]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0ch]
        push   dword ptr [eax + 10h]
        add    edi, 24h
        push   edi
        call   sub_1_531C
        add    esp, 18h
        jmp    NEAR PTR loc_1_5C96
loc_1_5F74:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 1ch]
        add    dword ptr [eax], 4
        mov    ebx, dword ptr [ebp + 20h]
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_1_5F91
        cmp    dword ptr [ebp - 4], 4
        mov    dword ptr [ebx], 0
        jge    SHORT loc_1_5F97
loc_1_5F91:
        mov    dword ptr [ebx], 1
loc_1_5F97:
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_1_5FA4
        sub    dword ptr [ebp - 4], 4
        add    dword ptr [ebp - 8], 4
loc_1_5FA4:
        push   ebx
        push   dword ptr [ebp + 1ch]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 8]
        push   esi
        push   ecx
        call   sub_1_531C
        add    esp, 18h
        jmp    NEAR PTR loc_1_5C96
        db  08Bh, 0FFh
loc_1_5FC0:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 24h], eax
        mov    dword ptr [ebp - 2ch], 0
        push   64h
        call   sub_1_8CB6
        add    esp, 4
        mov    dword ptr [ebp - 18h], eax
        or     eax, eax
        jne    SHORT loc_1_5FE8
        mov    esi, 8
        jmp    NEAR PTR loc_1_6472
loc_1_5FE8:
        mov    eax, dword ptr [ebp - 18h]
        mov    byte ptr [eax], 5ch
        mov    edx, eax
        inc    edx
        mov    byte ptr [edx], 5ch
        inc    edx
        db  0BFh, 092h, 024h, 001h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edi, [ecx + edx - 1]
        db  0BEh, 0D4h, 022h, 001h, 000h
        movsd
        movsd
        movsw
        lea    eax, [ebp - 20h]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_5AD0
        add    esp, 8
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 20h], 0
        je     SHORT loc_1_6060
        push   dword ptr [ebp - 20h]
        call   sub_1_4BE8
        add    esp, 4
        movzx  esi, ax
        jmp    NEAR PTR loc_1_6472
        db  08Bh, 0FFh
loc_1_6060:
        mov    eax, dword ptr [ebp + 1ch]
        add    dword ptr [eax], 18h
        mov    ebx, dword ptr [ebp + 20h]
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_1_607A
        cmp    dword ptr [ebp - 4], 18h
        mov    dword ptr [ebx], 0
        jge    SHORT loc_1_6080
loc_1_607A:
        mov    dword ptr [ebx], 1
loc_1_6080:
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_1_6098
        sub    dword ptr [ebp - 4], 18h
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 24h]
        add    dword ptr [ebp - 8], 18h
        mov    word ptr [ecx], ax
loc_1_6098:
        push   ebx
        push   dword ptr [ebp + 1ch]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 10h]
        push   dword ptr [eax + 4]
        mov    eax, dword ptr [ebp - 24h]
        add    eax, 2
        push   eax
        call   sub_1_531C
        add    esp, 18h
        push   ebx
        push   dword ptr [ebp + 1ch]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 10h]
        push   dword ptr [eax + 0ch]
        mov    eax, dword ptr [ebp - 24h]
        add    eax, 6
        push   eax
        call   sub_1_531C
        add    esp, 18h
        push   ebx
        push   dword ptr [ebp + 1ch]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 10h]
        push   dword ptr [eax + 10h]
        mov    eax, dword ptr [ebp - 24h]
        add    eax, 0ah
        push   eax
        call   sub_1_531C
        add    esp, 18h
        push   ebx
        push   dword ptr [ebp + 1ch]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 10h]
        push   dword ptr [eax + 20h]
        mov    eax, dword ptr [ebp - 24h]
        add    eax, 0eh
        push   eax
        call   sub_1_531C
        add    esp, 18h
        push   ebx
        push   dword ptr [ebp + 1ch]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 18h]
        mov    eax, dword ptr [ebp - 24h]
        add    eax, 12h
        push   eax
        call   sub_1_531C
        add    esp, 18h
        mov    eax, dword ptr [ebp - 10h]
        mov    edi, dword ptr [eax + 1ch]
        or     edi, edi
        je     SHORT loc_1_6196
        cmp    byte ptr [edi], 0
        je     SHORT loc_1_6196
        mov    esi, dword ptr [ebp - 24h]
        add    esi, 18h
loc_1_614A:
        mov    eax, dword ptr [ebp + 1ch]
        add    dword ptr [eax], 4
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_1_6161
        cmp    dword ptr [ebp - 4], 4
        mov    dword ptr [ebx], 0
        jge    SHORT loc_1_6167
loc_1_6161:
        mov    dword ptr [ebx], 1
loc_1_6167:
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_1_6174
        sub    dword ptr [ebp - 4], 4
        add    dword ptr [ebp - 8], 4
loc_1_6174:
        push   ebx
        push   dword ptr [ebp + 1ch]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 8]
        push   edi
        push   esi
        call   sub_1_531C
        add    esp, 18h
        add    edi, eax
        add    esi, 4
        inc    dword ptr [ebp - 2ch]
        cmp    byte ptr [edi], 0
        jne    SHORT loc_1_614A
loc_1_6196:
        cmp    dword ptr [ebx], 0
        jne    NEAR PTR loc_1_5C9F
        mov    ax, word ptr [ebp - 2ch]
        mov    ecx, dword ptr [ebp - 24h]
        mov    word ptr [ecx + 16h], ax
        jmp    NEAR PTR loc_1_5C96
        db  090h
loc_1_61B0:
        mov    edi, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0ch]
        mov    esi, dword ptr [eax + 1ch]
        or     esi, esi
        mov    edx, 0
        je     SHORT loc_1_61CD
        movzx  eax, word ptr [esi + 24h]
        movzx  ecx, word ptr [esi + 26h]
        lea    edx, [eax + ecx]
loc_1_61CD:
        mov    eax, dword ptr [ebp + 1ch]
        add    dword ptr [eax], edx
        mov    ebx, dword ptr [ebp + 20h]
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_1_61E5
        cmp    dword ptr [ebp - 4], edx
        mov    dword ptr [ebx], 0
        jge    SHORT loc_1_61EB
loc_1_61E5:
        mov    dword ptr [ebx], 1
loc_1_61EB:
        cmp    dword ptr [ebx], 0
        jne    NEAR PTR loc_1_5C9F
        sub    dword ptr [ebp - 4], edx
        mov    ecx, edx
        shr    ecx, 2
        add    dword ptr [ebp - 8], edx
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        jmp    NEAR PTR loc_1_5C96
        db  08Dh, 049h, 000h
loc_1_6210:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 1ch], eax
        mov    dword ptr [ebp - 28h], 0
        push   64h
        call   sub_1_8CB6
        add    esp, 4
        mov    dword ptr [ebp - 18h], eax
        or     eax, eax
        jne    SHORT loc_1_6238
        mov    esi, 8
        jmp    NEAR PTR loc_1_6472
loc_1_6238:
        mov    eax, dword ptr [ebp - 18h]
        mov    byte ptr [eax], 5ch
        mov    edx, eax
        inc    edx
        mov    byte ptr [edx], 5ch
        inc    edx
        db  0BFh, 092h, 024h, 001h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edi, [ecx + edx - 1]
        db  0BEh, 0E0h, 022h, 001h, 000h
        movsd
        movsd
        movsw
        lea    eax, [ebp - 20h]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_5AD0
        add    esp, 8
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 20h], 0
        je     SHORT loc_1_62B0
        push   dword ptr [ebp - 20h]
        call   sub_1_4BE8
        add    esp, 4
        movzx  esi, ax
        jmp    NEAR PTR loc_1_6472
        db  08Bh, 0FFh
loc_1_62B0:
        mov    eax, dword ptr [ebp + 1ch]
        add    dword ptr [eax], 24h
        mov    ebx, dword ptr [ebp + 20h]
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_1_62CA
        cmp    dword ptr [ebp - 4], 24h
        mov    dword ptr [ebx], 0
        jge    SHORT loc_1_62D0
loc_1_62CA:
        mov    dword ptr [ebx], 1
loc_1_62D0:
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_1_62E8
        sub    dword ptr [ebp - 4], 24h
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 1ch]
        add    dword ptr [ebp - 8], 24h
        mov    word ptr [ecx], ax
loc_1_62E8:
        push   ebx
        push   dword ptr [ebp + 1ch]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 10h]
        push   dword ptr [eax + 4]
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, 2
        push   eax
        call   sub_1_531C
        add    esp, 18h
        push   ebx
        push   dword ptr [ebp + 1ch]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 10h]
        push   dword ptr [eax + 0ch]
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, 6
        push   eax
        call   sub_1_531C
        add    esp, 18h
        push   ebx
        push   dword ptr [ebp + 1ch]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 10h]
        push   dword ptr [eax + 10h]
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, 0ah
        push   eax
        call   sub_1_531C
        add    esp, 18h
        push   ebx
        push   dword ptr [ebp + 1ch]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 10h]
        push   dword ptr [eax + 20h]
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, 0eh
        push   eax
        call   sub_1_531C
        add    esp, 18h
        push   ebx
        push   dword ptr [ebp + 1ch]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 18h]
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, 12h
        push   eax
        call   sub_1_531C
        add    esp, 18h
        push   ebx
        push   dword ptr [ebp + 1ch]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 10h]
        push   dword ptr [eax + 24h]
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, 16h
        push   eax
        call   sub_1_531C
        add    esp, 18h
        push   ebx
        push   dword ptr [ebp + 1ch]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 10h]
        push   dword ptr [eax + 18h]
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, 1ah
        push   eax
        call   sub_1_531C
        add    esp, 18h
        push   ebx
        push   dword ptr [ebp + 1ch]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 10h]
        push   dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, 1eh
        push   eax
        call   sub_1_531C
        add    esp, 18h
        mov    eax, dword ptr [ebp - 10h]
        mov    esi, dword ptr [eax + 1ch]
        or     esi, esi
        je     SHORT loc_1_6446
        cmp    byte ptr [esi], 0
        je     SHORT loc_1_6446
        mov    edi, dword ptr [ebp - 1ch]
        add    edi, 24h
loc_1_63FA:
        mov    eax, dword ptr [ebp + 1ch]
        add    dword ptr [eax], 4
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_1_6411
        cmp    dword ptr [ebp - 4], 4
        mov    dword ptr [ebx], 0
        jge    SHORT loc_1_6417
loc_1_6411:
        mov    dword ptr [ebx], 1
loc_1_6417:
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_1_6424
        sub    dword ptr [ebp - 4], 4
        add    dword ptr [ebp - 8], 4
loc_1_6424:
        push   ebx
        push   dword ptr [ebp + 1ch]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 8]
        push   esi
        push   edi
        call   sub_1_531C
        add    esp, 18h
        add    esi, eax
        add    edi, 4
        inc    dword ptr [ebp - 28h]
        cmp    byte ptr [esi], 0
        jne    SHORT loc_1_63FA
loc_1_6446:
        cmp    dword ptr [ebx], 0
        jne    NEAR PTR loc_1_5C9F
        mov    ax, word ptr [ebp - 28h]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    word ptr [ecx + 22h], ax
        jmp    NEAR PTR loc_1_5C96
        db  090h
loc_1_6460:
        sub    esi, esi
        mov    ecx, dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], ecx
loc_1_6472:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_1_6483
        push   dword ptr [ebp - 0ch]
        call   sub_1_8CF5
        add    esp, 4
loc_1_6483:
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_6494
        push   dword ptr [ebp - 10h]
        call   sub_1_8CF5
        add    esp, 4
loc_1_6494:
        cmp    dword ptr [ebp - 18h], 0
        je     SHORT loc_1_64A5
        push   dword ptr [ebp - 18h]
        call   sub_1_8CF5
        add    esp, 4
loc_1_64A5:
        push   esi
        call   sub_1_4BE8
        add    esp, 4
        and    eax, 0ffffh
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_64B8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    esi, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], 0
        push   0
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   dword ptr [eax + 2ch]
        call   sub_1_4C14
        add    esp, 0ch
        push   eax
        call   sub_1_4BE8
        add    esp, 4
        or     ax, ax
        je     SHORT loc_1_64FC
        and    eax, 0ffffh
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_64FC:
        lea    eax, [ebp - 0ch]
        push   eax
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp - 8]
        call   sub_1_5B58
        add    esp, 1ch
        mov    bx, ax
        mov    edi, dword ptr [ebp - 0ch]
        or     bx, bx
        je     SHORT loc_1_6530
        cmp    bx, 84bh
        jne    NEAR PTR loc_1_65C3
loc_1_6530:
        cmp    dword ptr [ebp + 8], 2
        jne    SHORT loc_1_657C
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 0ch]
        push   eax
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp + 0ch]
        push   1
        push   dword ptr [ebp - 8]
        call   sub_1_543C
        add    esp, 20h
        mov    bx, ax
        cmp    bx, 84bh
        jne    SHORT loc_1_656C
        mov    bx, 0eah
loc_1_656C:
        or     edi, edi
        jne    SHORT loc_1_65C7
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [esi + 2ah], ax
        jmp    SHORT loc_1_65C3
        db  08Dh, 049h, 000h
loc_1_657C:
        cmp    dword ptr [ebp + 8], 4
        jne    SHORT loc_1_65C3
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 0ch]
        push   eax
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp + 0ch]
        push   2
        push   dword ptr [ebp - 8]
        call   sub_1_543C
        add    esp, 20h
        mov    bx, ax
        cmp    bx, 84bh
        jne    SHORT loc_1_65B8
        mov    bx, 0eah
loc_1_65B8:
        or     edi, edi
        jne    SHORT loc_1_65C7
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [esi + 1eh], ax
loc_1_65C3:
        or     edi, edi
        je     SHORT loc_1_65D7
loc_1_65C7:
        or     bx, bx
        je     SHORT loc_1_65D3
        cmp    bx, 0eah
        jne    SHORT loc_1_65D7
loc_1_65D3:
        mov    bx, 84bh
loc_1_65D7:
        push   dword ptr [ebp - 8]
        call   sub_1_926A
        add    esp, 4
        movzx  eax, bx
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_65EC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 6]
        mov    dword ptr [ebp - 8], eax
        movsx  edi, word ptr [esi + 0ah]
        movzx  eax, word ptr [esi + 4]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 4], 0
        cmp    edi, 5
        jle    SHORT loc_1_6628
        cmp    edi, 32h
        jl     SHORT loc_1_661E
        cmp    edi, 34h
        jle    SHORT loc_1_6628
loc_1_661E:
        pop    edi
        mov    eax, 7ch
        pop    esi
        leave
        ret
        db  090h
loc_1_6628:
        push   1
        push   dword ptr [esi + 0ch]
        call   sub_1_6674
        add    esp, 8
        push   dword ptr [esi + 0ch]
        call   sub_1_7E77
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_6650
        mov    eax, 866h
        pop    edi
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_6650:
        lea    ecx, [ebp - 4]
        push   ecx
        lea    ecx, [ebp - 0ch]
        push   ecx
        lea    ecx, [ebp - 8]
        push   ecx
        push   eax
        push   edi
        call   sub_1_64B8
        add    esp, 14h
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi]
        mov    word ptr [ecx], dx
        pop    edi
        pop    esi
        leave
        ret
        db  08Bh, 0FFh
sub_1_6674:
        push   ebp
        mov    ebp, esp
        sub    esp, 40h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edi, [ecx - 1]
        cmp    edi, 20h
        jae    SHORT loc_1_66D6
        cmp    dword ptr [ebp + 0ch], 1
        sbb    eax, eax
        inc    eax
        push   eax
        push   edi
        push   esi
        lea    eax, [ebp - 40h]
        push   eax
        call   sub_1_923C
        add    esp, 10h
        mov    ebx, eax
        push   ebx
        lea    eax, [ebp - 40h]
        push   eax
        push   eax
        call   sub_1_9244
        add    esp, 0ch
        cmp    dword ptr [ebp + 0ch], 1
        sbb    eax, eax
        inc    eax
        push   eax
        push   edi
        push   ebx
        lea    eax, [ebp - 40h]
        push   eax
        push   esi
        call   sub_1_924C
        add    esp, 14h
        mov    byte ptr [eax + esi], 0
loc_1_66D6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_66DC:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 0ah]
        mov    dword ptr [ebp - 8], eax
        movsx  esi, word ptr [ecx + 0eh]
        mov    dword ptr [ebp - 10h], 0
        movzx  eax, word ptr [ecx + 8]
        mov    dword ptr [ebp - 0ch], eax
        sub    eax, eax
        sub    edi, edi
        mov    dword ptr [ebp - 4], eax
        cmp    esi, 5
        jle    SHORT loc_1_6720
        cmp    esi, 32h
        jl     SHORT loc_1_6716
        cmp    esi, 34h
        jle    SHORT loc_1_6720
loc_1_6716:
        pop    edi
        mov    eax, 7ch
        pop    esi
        pop    ebx
        leave
        ret
loc_1_6720:
        db  08Bh, 01Dh, 000h, 01Ch, 001h, 000h
        db  081h, 0FBh, 000h, 01Ch, 001h, 000h
        je     SHORT loc_1_6762
loc_1_672E:
        cmp    byte ptr [ebx + 8], 2
        jne    SHORT loc_1_6758
        cmp    byte ptr [ebx + 9], 1
        jne    SHORT loc_1_6758
        or     eax, eax
        jne    SHORT loc_1_6757
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   ebx
        push   esi
        call   sub_1_64B8
        add    esp, 14h
        inc    dword ptr [ebp - 4]
loc_1_6757:
        inc    edi
loc_1_6758:
        mov    ebx, dword ptr [ebx]
        db  081h, 0FBh, 000h, 01Ch, 001h, 000h
        jne    SHORT loc_1_672E
loc_1_6762:
        cmp    eax, 84bh
        jne    SHORT loc_1_676E
        mov    eax, 0eah
loc_1_676E:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx]
        mov    word ptr [ecx], di
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ecx + 4]
        mov    cx, word ptr [ebp - 4]
        mov    word ptr [edx], cx
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_6788:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   400h
        call   sub_1_8CB6
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_67B0
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_67B0:
        lea    eax, [ebp - 0ch]
        push   eax
        push   400h
        push   esi
        push   2
        mov    eax, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [eax + 0ah]
        push   eax
        push   0
        call   sub_1_9264
        add    esp, 18h
        cmp    eax, 7ah
        jne    SHORT loc_1_6814
        push   esi
        call   sub_1_8CF5
        add    esp, 4
        push   dword ptr [ebp - 0ch]
        call   sub_1_8CB6
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_67F8
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_67F8:
        lea    eax, [ebp - 0ch]
        push   eax
        push   dword ptr [ebp - 0ch]
        push   esi
        push   2
        mov    eax, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [eax + 0ah]
        push   eax
        push   0
        call   sub_1_9264
        add    esp, 18h
loc_1_6814:
        or     eax, eax
        jne    NEAR PTR loc_1_6B23
        mov    eax, dword ptr [ebp + 8]
        mov    cl, byte ptr [eax + 11h]
        and    cl, 1
        or     cl, cl
        mov    ecx, 0
        setne  cl
        mov    dword ptr [ebp - 4], ecx
        or     ecx, ecx
        jne    SHORT loc_1_6870
        mov    ecx, dword ptr [esi + 0ch]
        mov    eax, dword ptr [eax + 4]
loc_1_683C:
        mov    dl, byte ptr [ecx]
        cmp    dl, byte ptr [eax]
        jne    SHORT loc_1_685C
        or     dl, dl
        je     SHORT loc_1_6858
        mov    dl, byte ptr [ecx + 1]
        cmp    dl, byte ptr [eax + 1]
        jne    SHORT loc_1_685C
        add    ecx, 2
        add    eax, 2
        or     dl, dl
        jne    SHORT loc_1_683C
loc_1_6858:
        sub    eax, eax
        jmp    SHORT loc_1_6861
loc_1_685C:
        sbb    eax, eax
        sbb    eax, -1
loc_1_6861:
        or     eax, eax
        je     SHORT loc_1_6870
        mov    edi, 5
        jmp    NEAR PTR loc_1_6B2F
        db  090h
loc_1_6870:
        mov    eax, dword ptr [esi + 3ch]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [esi + 3ch], 0
        mov    eax, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [eax]
        cmp    eax, 12h
        ja     SHORT loc_1_6890
        db  0FFh, 024h, 085h, 048h, 06Bh, 000h, 000h
        db  090h
loc_1_6890:
        mov    edi, 57h
        jmp    NEAR PTR loc_1_6B2F
        db  08Bh, 0FFh
loc_1_689C:
        mov    eax, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [eax + 8]
        cmp    eax, 1
        je     SHORT loc_1_68B8
        cmp    eax, 3
        je     SHORT loc_1_6920
        mov    edi, 7ch
        jmp    NEAR PTR loc_1_6B2F
        db  090h
loc_1_68B8:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [eax + 4]
        lea    eax, [ebx + 18h]
        mov    dword ptr [esi + 14h], eax
        lea    eax, [ebx + 28h]
        mov    dword ptr [esi + 18h], eax
        mov    eax, dword ptr [ebx + 32h]
        or     eax, eax
        je     SHORT loc_1_68D4
        mov    dword ptr [esi + 20h], eax
loc_1_68D4:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_68E3
        movzx  eax, word ptr [ebx + 36h]
        cmp    eax, dword ptr [ebp - 8]
        jbe    SHORT loc_1_68EA
loc_1_68E3:
        movzx  eax, word ptr [ebx + 36h]
        mov    dword ptr [esi + 3ch], eax
loc_1_68EA:
        mov    edi, dword ptr [ebx + 46h]
        or     edi, edi
        je     NEAR PTR loc_1_6B0C
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    eax, dword ptr [ebx + 46h]
        push   ecx
        push   1
        push   0
        push   eax
        push   eax
        call   sub_1_9288
        add    esp, 14h
        mov    eax, dword ptr [ebx + 46h]
        mov    dword ptr [esi + 10h], eax
        jmp    NEAR PTR loc_1_6B0C
        db  08Bh, 0FFh
loc_1_6920:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [eax + 4]
        mov    edi, dword ptr [ebx + 1ch]
        or     edi, edi
        je     SHORT loc_1_6951
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    eax, dword ptr [ebx + 1ch]
        push   ecx
        push   1
        push   0
        push   eax
        push   eax
        call   sub_1_9288
        add    esp, 14h
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [esi + 14h], eax
loc_1_6951:
        mov    eax, dword ptr [ebx + 20h]
        or     eax, eax
        je     SHORT loc_1_695B
        mov    dword ptr [esi + 18h], eax
loc_1_695B:
        mov    eax, dword ptr [ebx + 24h]
        or     eax, eax
        je     SHORT loc_1_6965
        mov    dword ptr [esi + 20h], eax
loc_1_6965:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_6974
        movzx  eax, word ptr [ebx + 8]
        cmp    eax, dword ptr [ebp - 8]
        jbe    SHORT loc_1_697B
loc_1_6974:
        movzx  eax, word ptr [ebx + 8]
        mov    dword ptr [esi + 3ch], eax
loc_1_697B:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_698A
        movzx  eax, word ptr [ebx + 2]
        cmp    eax, dword ptr [esi + 38h]
        jae    SHORT loc_1_6991
loc_1_698A:
        movzx  eax, word ptr [ebx + 2]
        mov    dword ptr [esi + 38h], eax
loc_1_6991:
        mov    edi, dword ptr [ebx + 18h]
        or     edi, edi
        je     SHORT loc_1_69C0
        mov    dword ptr [esi + 10h], edi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    eax, dword ptr [esi + 10h]
        push   ecx
        push   1
        push   0
        push   eax
        push   eax
        call   sub_1_9288
        add    esp, 14h
        jmp    NEAR PTR loc_1_6B0C
        db  08Bh, 0FFh
loc_1_69C0:
        mov    edi, dword ptr [ebx + 14h]
        or     edi, edi
        je     NEAR PTR loc_1_6B0C
        mov    dword ptr [esi + 10h], edi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    eax, dword ptr [esi + 10h]
        push   ecx
        push   1
        push   0
        push   eax
        push   eax
        call   sub_1_9288
        add    esp, 14h
        jmp    NEAR PTR loc_1_6B0C
        db  08Dh, 049h, 000h
loc_1_69F4:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [eax + 4]
        or     ebx, ebx
        je     NEAR PTR loc_1_6B0C
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        push   1
        push   0
        push   ebx
        push   ebx
        call   sub_1_9288
        add    esp, 14h
        mov    dword ptr [esi + 14h], ebx
        jmp    NEAR PTR loc_1_6B0C
        db  090h
loc_1_6A28:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        je     NEAR PTR loc_1_6B0C
        mov    dword ptr [esi + 18h], eax
        jmp    NEAR PTR loc_1_6B0C
        db  08Bh, 0FFh
loc_1_6A40:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        je     NEAR PTR loc_1_6B0C
        mov    dword ptr [esi + 20h], eax
        jmp    NEAR PTR loc_1_6B0C
        db  08Bh, 0FFh
loc_1_6A58:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [eax + 4]
        mov    ax, word ptr [eax]
        jne    SHORT loc_1_6A7C
        movzx  ecx, ax
        cmp    ecx, dword ptr [ebp - 8]
        ja     SHORT loc_1_6A7C
        mov    edi, 5
        jmp    NEAR PTR loc_1_6B2F
        db  08Dh, 049h, 000h
loc_1_6A7C:
        and    eax, 0ffffh
        mov    dword ptr [esi + 3ch], eax
        jmp    NEAR PTR loc_1_6B0C
        db  08Dh, 049h, 000h
loc_1_6A8C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [eax + 4]
        or     ebx, ebx
        je     SHORT loc_1_6AB3
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        push   1
        push   0
        push   ebx
        push   ebx
        call   sub_1_9288
        add    esp, 14h
loc_1_6AB3:
        mov    dword ptr [esi + 10h], ebx
        jmp    SHORT loc_1_6B0C
loc_1_6AB8:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [eax + 4]
        or     ebx, ebx
        je     SHORT loc_1_6B0C
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        push   1
        push   0
        push   ebx
        push   ebx
        call   sub_1_9288
        add    esp, 14h
        mov    dword ptr [esi + 10h], ebx
        jmp    SHORT loc_1_6B0C
loc_1_6AE4:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [eax + 4]
        mov    ax, word ptr [eax]
        jne    SHORT loc_1_6B04
        movzx  ecx, ax
        cmp    dword ptr [esi + 38h], ecx
        ja     SHORT loc_1_6B04
        mov    edi, 5
        jmp    SHORT loc_1_6B2F
        db  08Bh, 0FFh
loc_1_6B04:
        and    eax, 0ffffh
        mov    dword ptr [esi + 38h], eax
loc_1_6B0C:
        push   0
        push   esi
        push   2
        mov    eax, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [eax + 0ah]
        push   eax
        push   0
        call   sub_1_927C
        add    esp, 14h
loc_1_6B23:
        push   eax
        call   sub_1_4BE8
        add    esp, 4
        movzx  edi, ax
loc_1_6B2F:
        push   esi
        call   sub_1_8CF5
        add    esp, 4
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_6B40:
        mov    edi, 32h
        jmp    SHORT loc_1_6B2F
        db  090h, 09Ch, 068h, 000h, 000h, 090h, 068h, 000h, 000h, 090h, 068h, 000h, 000h, 0F4h, 069h, 000h
        db  000h, 028h, 06Ah, 000h, 000h, 040h, 06Ah, 000h, 000h, 058h, 06Ah, 000h, 000h, 090h, 068h, 000h
        db  000h, 090h, 068h, 000h, 000h, 090h, 068h, 000h, 000h, 090h, 068h, 000h, 000h, 08Ch, 06Ah, 000h
        db  000h, 0B8h, 06Ah, 000h, 000h, 090h, 068h, 000h, 000h, 0E4h, 06Ah, 000h, 000h, 090h, 068h, 000h
        db  000h, 040h, 06Bh, 000h, 000h, 090h, 068h, 000h, 000h, 040h, 06Bh, 000h, 000h
sub_1_6B94:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    esi, edi
        jne    SHORT loc_1_6BB0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_6BB0:
        mov    ebx, dword ptr [ebp + 10h]
        test   byte ptr [ebx + 11h], 1
        jne    NEAR PTR loc_1_6C84
        push   400h
        call   sub_1_8CB6
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_6BDC
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_6BDC:
        lea    eax, [ebp - 4]
        push   eax
        push   400h
        push   edi
        push   2
        push   dword ptr [ebp + 8]
        push   0
        call   sub_1_9264
        add    esp, 18h
        cmp    eax, 7ah
        jne    SHORT loc_1_6C37
        push   edi
        call   sub_1_8CF5
        add    esp, 4
        push   dword ptr [ebp - 4]
        call   sub_1_8CB6
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_6C20
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_6C20:
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 4]
        push   edi
        push   2
        push   dword ptr [ebp + 8]
        push   0
        call   sub_1_9264
        add    esp, 18h
loc_1_6C37:
        or     eax, eax
        jne    SHORT loc_1_6C96
        mov    eax, dword ptr [edi + 0ch]
        push   14h
        push   0
        push   1
        push   eax
        push   eax
        call   sub_1_9288
        add    esp, 14h
        mov    eax, dword ptr [edi + 0ch]
        mov    edx, dword ptr [ebx + 4]
loc_1_6C54:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    SHORT loc_1_6C74
        or     cl, cl
        je     SHORT loc_1_6C70
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    SHORT loc_1_6C74
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    SHORT loc_1_6C54
loc_1_6C70:
        sub    eax, eax
        jmp    SHORT loc_1_6C79
loc_1_6C74:
        sbb    eax, eax
        sbb    eax, -1
loc_1_6C79:
        or     eax, eax
        je     SHORT loc_1_6C84
        mov    esi, 5
        jmp    SHORT loc_1_6CA2
loc_1_6C84:
        push   esi
        push   0
        push   0
        push   dword ptr [ebp + 8]
        push   0
        call   sub_1_927C
        add    esp, 14h
loc_1_6C96:
        push   eax
        call   sub_1_4BE8
        add    esp, 4
        movzx  esi, ax
loc_1_6CA2:
        or     edi, edi
        je     SHORT loc_1_6CAF
        push   edi
        call   sub_1_8CF5
        add    esp, 4
loc_1_6CAF:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_6CB8:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        push   3
        mov    eax, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [eax]
        push   eax
        call   sub_1_6B94
        leave
        ret
        db  08Bh, 0FFh
loc_1_6CD0:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        push   1
        mov    eax, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [eax]
        push   eax
        call   sub_1_6B94
        leave
        ret
        db  08Bh, 0FFh
loc_1_6CE8:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        push   2
        mov    eax, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [eax]
        push   eax
        call   sub_1_6B94
        leave
        ret
        db  08Bh, 0FFh
loc_1_6D00:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 2]
        cmp    word ptr [eax], 15h
        jae    SHORT loc_1_6D18
        mov    eax, 84bh
        leave
        ret
        db  08Bh, 0FFh
loc_1_6D18:
        push   ecx
        call   sub_1_7F32
        add    esp, 4
        sub    eax, eax
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_6D28:
        push   ebp
        mov    ebp, esp
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 10h]
        push   ebx
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        call   sub_1_8C68
        add    esp, 0ch
        mov    ecx, 6
        mov    esi, dword ptr [ebp + 14h]
        mov    edi, ebx
        repe cmpsd
        jne    SHORT loc_1_6D58
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_6D58:
        push   1
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        push   dword ptr [ebp + 8]
        lea    eax, [ebp - 30h]
        push   eax
        call   sub_1_923C
        add    esp, 10h
        push   ebx
        push   eax
        lea    eax, [ebp - 30h]
        push   eax
        push   dword ptr [ebp + 0ch]
        call   sub_1_8C6E
        add    esp, 10h
        mov    ecx, 6
        mov    esi, dword ptr [ebp + 14h]
        mov    edi, ebx
        repe cmpsd
        jne    SHORT loc_1_6DA0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_6DA0:
        sub    eax, eax
        lea    edi, [ebp - 10h]
        stosd
        stosd
        stosd
        stosw
        stosb
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        push   dword ptr [ebp + 8]
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_1_E44D
        add    esp, 0ch
        push   1
        lea    edi, [ebp - 10h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 30h]
        push   eax
        call   sub_1_923C
        add    esp, 10h
        push   ebx
        push   eax
        lea    eax, [ebp - 30h]
        push   eax
        push   dword ptr [ebp + 0ch]
        call   sub_1_8C6E
        add    esp, 10h
        mov    ecx, 6
        mov    esi, dword ptr [ebp + 14h]
        mov    edi, ebx
        repe cmpsd
        jne    SHORT loc_1_6E14
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_6E14:
        pop    edi
        mov    eax, 1
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_6E20:
        ret
loc_1_6E21:
        db  080h, 03Dh, 0DCh, 01Ch, 001h, 000h, 000h
        jne    SHORT loc_1_6E20
        db  08Bh, 01Dh, 028h, 028h, 001h, 000h
        db  08Bh, 00Dh, 015h, 023h, 001h, 000h
        db  089h, 01Dh, 015h, 023h, 001h, 000h
        sub    ebx, ecx
        db  0A1h, 0C4h, 031h, 001h, 000h
        db  08Bh, 00Dh, 019h, 023h, 001h, 000h
        db  0A3h, 019h, 023h, 001h, 000h
        sub    eax, ecx
        mov    ecx, 3e8h
        xor    edx, edx
        add    eax, 1f4h
        div    ecx
        sub    eax, ebx
        cdq
        sub    eax, edx
        and    al, 0feh
        db  0BFh, 02Ch, 01Ch, 001h, 000h
loc_1_6E6A:
        mov    edi, dword ptr [edi]
        test   edi, edi
        je     SHORT loc_1_6ECA
        cmp    byte ptr [edi + 1eh], 2
        jne    SHORT loc_1_6E6A
        sub    dword ptr [edi + 34h], eax
        cmp    word ptr [edi + 44h], 0
        jne    SHORT loc_1_6E9D
        db  08Bh, 00Dh, 028h, 028h, 001h, 000h
        sub    ecx, dword ptr [edi + 34h]
        db  03Bh, 00Dh, 048h, 01Ah, 001h, 000h
        jae    SHORT loc_1_6EB5
        cmp    word ptr [edi + 42h], 0
        jne    SHORT loc_1_6E9D
        cmp    ecx, 1eh
        jae    SHORT loc_1_6EB5
loc_1_6E9D:
        mov    esi, dword ptr [edi + 14h]
        test   esi, esi
        je     SHORT loc_1_6E6A
        db  08Bh, 00Dh, 0C4h, 031h, 001h, 000h
        sub    ecx, dword ptr [esi + 0ch]
        db  03Bh, 00Dh, 0A0h, 01Bh, 001h, 000h
        jbe    SHORT loc_1_6E6A
loc_1_6EB5:
        mov    byte ptr [edi + 1eh], 5
        db  08Bh, 015h, 05Ch, 027h, 001h, 000h
        mov    dword ptr [edi + 18h], edx
        db  089h, 03Dh, 05Ch, 027h, 001h, 000h
        jmp    SHORT loc_1_6E6A
loc_1_6ECA:
        call   sub_1_8814
        db  0B8h, 021h, 023h, 001h, 000h
        mov    dword ptr [eax + 0ch], 0ea60h
        call   sub_1_911C
        db  0B8h, 030h, 01Ch, 001h, 000h
        jmp    NEAR PTR sub_1_8BC0
        db  08Bh, 0C0h
loc_1_6EEC:
        db  0A1h, 014h, 028h, 001h, 000h
        mov    eax, dword ptr [eax + 0ch]
        db  0A3h, 01Dh, 023h, 001h, 000h
        xor    eax, eax
        db  0A2h, 0DCh, 01Ch, 001h, 000h
        call   sub_1_91C8
        db  0B8h, 021h, 023h, 001h, 000h
        call   sub_1_911C
        db  0A1h, 028h, 028h, 001h, 000h
        db  0A3h, 015h, 023h, 001h, 000h
        db  0A1h, 0C4h, 031h, 001h, 000h
        db  0A3h, 019h, 023h, 001h, 000h
        call   sub_1_8F35
        call   sub_1_1192C
        call   sub_1_11A2C
        db  0B8h, 080h, 087h, 000h, 000h
        call   sub_1_89B4
        db  0B8h, 080h, 087h, 000h, 000h
        call   sub_1_89B4
        db  0B8h, 080h, 087h, 000h, 000h
        call   sub_1_89B4
        jmp    SHORT loc_1_6F63
loc_1_6F52:
        db  0C6h, 005h, 0FCh, 022h, 001h, 000h, 001h
loc_1_6F59:
        db  0B8h, 030h, 01Ch, 001h, 000h
        call   sub_1_875C
loc_1_6F63:
        db  0C6h, 005h, 0FCh, 022h, 001h, 000h, 000h
        db  0A1h, 020h, 01Ch, 001h, 000h
        cmp    byte ptr [eax], 2
        je     NEAR PTR loc_1_7054
        db  0F7h, 005h, 038h, 034h, 001h, 000h, 020h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_7074
        db  083h, 03Dh, 0ACh, 01Bh, 001h, 000h, 000h
        jne    NEAR PTR loc_1_707E
        db  083h, 03Dh, 008h, 01Ch, 001h, 000h, 000h
        jne    NEAR PTR loc_1_7088
        db  083h, 03Dh, 058h, 027h, 001h, 000h, 000h
        jne    NEAR PTR loc_1_70D0
        db  083h, 03Dh, 05Ch, 027h, 001h, 000h, 000h
        jne    NEAR PTR loc_1_70F1
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        sub    edx, edx
        mov    dword ptr [esi + 20h], edx
        db  08Bh, 03Dh, 0A6h, 024h, 001h, 000h
        or     edi, edi
        je     SHORT loc_1_6F59
loc_1_6FD1:
        cmp    byte ptr [edi + 9], 0ffh
        je     SHORT loc_1_6FE9
        cmp    byte ptr [edi + 1], 0
        jne    SHORT loc_1_6FE9
        movzx  edx, byte ptr [edi]
        push   esi
        call   sub_1_7464
        pop    esi
        jb     SHORT loc_1_6FF2
loc_1_6FE9:
        mov    edi, dword ptr [edi + 0ch]
        test   edi, edi
        jne    SHORT loc_1_6FD1
        jmp    SHORT loc_1_6FF8
loc_1_6FF2:
        db  0FEh, 005h, 0FCh, 022h, 001h, 000h
loc_1_6FF8:
        sub    edx, edx
        mov    dword ptr [esi + 20h], edx
        db  08Bh, 03Dh, 0A6h, 024h, 001h, 000h
        or     edi, edi
        je     NEAR PTR loc_1_6F59
loc_1_700B:
        cmp    byte ptr [edi + 9], 0ffh
        je     SHORT loc_1_702E
        mov    al, byte ptr [edi + 3]
        cmp    byte ptr [edi + 5], al
        jae    SHORT loc_1_702E
        movzx  edx, byte ptr [edi]
        push   esi
        call   sub_1_7388
        pop    esi
        jb     NEAR PTR loc_1_6F52
        jne    SHORT loc_1_702E
        inc    dword ptr [esi + 20h]
loc_1_702E:
        mov    edi, dword ptr [edi + 0ch]
        test   edi, edi
        jne    SHORT loc_1_700B
        cli
        cmp    dword ptr [esi + 20h], 0
        je     NEAR PTR loc_1_6F59
        sti
        jmp    SHORT loc_1_6FF8
loc_1_7043:
        call   sub_1_7253
        db  0C6h, 005h, 0DCh, 01Ch, 001h, 000h, 001h
        jmp    NEAR PTR loc_1_8780
loc_1_7054:
        call   sub_1_F680
        call   sub_1_7253
        mov    edx, 0ffffffffh
loc_1_7063:
        inc    edx
        cmp    edx, 11h
        jae    SHORT loc_1_7043
        push   edx
        call   sub_1_7115
        add    esp, 4
        jmp    SHORT loc_1_7063
loc_1_7074:
        call   sub_1_DA2C
        jmp    NEAR PTR loc_1_6F63
loc_1_707E:
        call   sub_1_770C
        jmp    NEAR PTR loc_1_6F63
loc_1_7088:
        db  08Bh, 03Dh, 008h, 01Ch, 001h, 000h
        mov    eax, dword ptr [edi]
        db  0A3h, 008h, 01Ch, 001h, 000h
        mov    bl, 3
        call   sub_1_37BC
        mov    eax, dword ptr [edi + 34h]
        test   eax, eax
        je     SHORT loc_1_70AE
        push   ecx
        push   edx
        push   eax
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
loc_1_70AE:
        mov    eax, dword ptr [edi + 30h]
        test   eax, eax
        je     SHORT loc_1_70C0
        push   ecx
        push   edx
        push   eax
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
loc_1_70C0:
        push   ecx
        push   edx
        push   edi
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        jmp    NEAR PTR loc_1_6F63
loc_1_70D0:
        db  08Bh, 00Dh, 058h, 027h, 001h, 000h
loc_1_70D6:
        mov    esi, dword ptr [ecx + 2eh]
        db  089h, 035h, 058h, 027h, 001h, 000h
        mov    al, 2
        call   sub_1_1E8C
        mov    ecx, esi
        test   ecx, ecx
        jne    SHORT loc_1_70D6
        jmp    NEAR PTR loc_1_6F63
loc_1_70F1:
        cli
        db  08Bh, 03Dh, 05Ch, 027h, 001h, 000h
        mov    esi, dword ptr [edi + 18h]
        db  089h, 035h, 05Ch, 027h, 001h, 000h
        sti
        call   sub_1_728F
        db  083h, 03Dh, 05Ch, 027h, 001h, 000h, 000h
        jne    SHORT loc_1_70F1
        jmp    NEAR PTR loc_1_6F63
sub_1_7115:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   ebx
        db  0A1h, 014h, 028h, 001h, 000h
        mov    dword ptr [eax + 20h], 0fffff000h
        mov    dword ptr [eax + 30h], 0fffff000h
        mov    edx, dword ptr [ebp + 8]
        db  08Bh, 034h, 095h, 0B0h, 01Bh, 001h, 000h
        cmp    esi, 0fffff000h
        je     NEAR PTR loc_1_7210
        db  080h, 08Ah, 0B8h, 01Ch, 001h, 000h, 040h
        mov    byte ptr [esi + 2], 0
        mov    byte ptr [esi + 3], 0
        mov    byte ptr [esi + 4], 0
        cmp    byte ptr [esi + 9], 0ffh
        je     SHORT loc_1_71B6
        db  0F6h, 082h, 0B8h, 01Ch, 001h, 000h, 002h
        jne    NEAR PTR loc_1_7218
        mov    al, 4
        call   sub_1_7738
        jae    SHORT loc_1_7183
        mov    al, 0
        call   sub_1_7738
        jae    SHORT loc_1_7183
loc_1_717C:
        xor    eax, eax
        pop    ebx
        pop    esi
        pop    edi
        leave
        ret
loc_1_7183:
        mov    dword ptr [ecx + 48h], 0
        mov    byte ptr [ecx + 5], 7eh
        mov    byte ptr [ecx + 8], 31h
        mov    al, byte ptr [esi + 9]
        mov    byte ptr [ecx + 0bh], al
        mov    byte ptr [esi + 9], 0ffh
        push   edx
        push   esi
        db  0BEh, 048h, 023h, 001h, 000h
        lea    edi, [ecx + 22h]
        movsd
        movsd
        movsd
        movsd
        call   sub_1_75B4
        pop    esi
        pop    edx
        call   sub_1_781C
loc_1_71B6:
        lea    edi, [esi + 0f2h]
        mov    ecx, 100h
loc_1_71C1:
        xor    eax, eax
        cld
        repe scasd
        je     SHORT loc_1_71D8
        pushad
        mov    edi, dword ptr [edi - 4]
        call   sub_1_728F
        popad
        jb     SHORT loc_1_717C
        test   ecx, ecx
        jne    SHORT loc_1_71C1
loc_1_71D8:
        db  0C7h, 004h, 095h, 0B0h, 01Bh, 001h, 000h, 000h, 0F0h, 0FFh, 0FFh
        db  00Fh, 0B3h, 015h, 004h, 023h, 001h, 000h
        db  0BBh, 09Ah, 024h, 001h, 000h
loc_1_71EF:
        cmp    esi, dword ptr [ebx + 0ch]
        je     SHORT loc_1_71F9
        mov    ebx, dword ptr [ebx + 0ch]
        jmp    SHORT loc_1_71EF
loc_1_71F9:
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [ebx + 0ch], eax
        db  0FFh, 00Dh, 0F4h, 01Bh, 001h, 000h
        push   ecx
        push   edx
        push   esi
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
loc_1_7210:
        xor    eax, eax
        inc    eax
        pop    ebx
        pop    esi
        pop    edi
        leave
        ret
loc_1_7218:
        mov    byte ptr [esi + 9], 0ffh
        push   esi
        push   edx
        mov    ebx, 1
        mov    edx, 5105h
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        mov    ebx, 1
        mov    edx, 5005h
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        pop    edx
        pop    esi
        push   ecx
        push   edx
        push   dword ptr [esi + 14h]
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        jmp    NEAR PTR loc_1_71B6
sub_1_7253:
        db  080h, 03Dh, 0CBh, 01Ch, 001h, 000h, 000h
        jne    SHORT loc_1_725D
        ret
loc_1_725D:
        pushad
        push   5
        mov    eax, esp
        call   sub_1_88E0
loc_1_7267:
        call   sub_1_890C
        jbe    SHORT loc_1_7277
        db  080h, 03Dh, 0CBh, 01Ch, 001h, 000h, 000h
        jne    SHORT loc_1_7267
loc_1_7277:
        call   sub_1_8900
        pop    eax
        popad
        ret
sub_1_727F:
        push   esi
        push   edi
        push   ebx
        mov    edi, dword ptr [esp + 10h]
        call   sub_1_728F
        pop    ebx
        pop    edi
        pop    esi
        ret
sub_1_728F:
        cmp    byte ptr [edi + 1eh], 5
        je     SHORT loc_1_729F
        cmp    byte ptr [edi + 1eh], 2
        jne    NEAR PTR loc_1_7337
loc_1_729F:
        mov    byte ptr [edi + 1eh], 3
        call   sub_1_74F4
        jb     NEAR PTR loc_1_7336
        mov    bl, 4
        call   sub_1_37BC
loc_1_72B5:
        mov    byte ptr [edi + 1eh], 0
        movzx  edx, byte ptr [edi + 3fh]
        db  0A1h, 07Ch, 023h, 001h, 000h
        mov    dword ptr [edi + 18h], eax
        db  089h, 03Dh, 07Ch, 023h, 001h, 000h
        mov    eax, dword ptr [edi + 38h]
loc_1_72CE:
        test   eax, eax
        je     SHORT loc_1_7304
        mov    ebx, dword ptr [eax]
        test   byte ptr [eax + 1eh], 2
        je     SHORT loc_1_72F5
        push   eax
        push   ebx
        push   edx
        lea    ecx, [edi + 4]
        push   ecx
        lea    ecx, [eax + 8]
        push   ecx
        lea    ecx, [eax + 24h]
        push   ecx
        db  0CDh, 020h, 004h, 000h, 093h, 004h
        add    esp, 0ch
        pop    edx
        pop    ebx
        pop    eax
loc_1_72F5:
        push   ecx
        push   edx
        push   eax
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        mov    eax, ebx
        jmp    SHORT loc_1_72CE
loc_1_7304:
        lea    ebx, [edi + 2ch]
        mov    eax, dword ptr [edi + 24h]
        cmp    eax, ebx
        je     SHORT loc_1_731D
        or     eax, eax
        je     SHORT loc_1_731D
        push   ecx
        push   edx
        push   eax
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
loc_1_731D:
        db  08Bh, 03Ch, 095h, 0B0h, 01Bh, 001h, 000h
        cmp    edi, 0fffff000h
        je     SHORT loc_1_7334
        dec    byte ptr [edi + 7]
        call   sub_1_7344
loc_1_7334:
        xor    eax, eax
loc_1_7336:
        ret
loc_1_7337:
        cmp    byte ptr [edi + 1eh], 1
        je     NEAR PTR loc_1_72B5
        ret
        db  08Bh, 0C0h
sub_1_7344:
        movzx  eax, byte ptr [edi + 7]
        lea    eax, [eax*4 + 2]
        movzx  ebx, byte ptr [edi + 2]
        sub    eax, ebx
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, ebx
        mov    byte ptr [edi + 3], al
        add    eax, 3
        shr    eax, 2
        mov    byte ptr [edi + 4], al
        ret
loc_1_7368:
        db  0A1h, 020h, 01Ch, 001h, 000h
        cmp    byte ptr [eax], 2
        je     SHORT loc_1_7387
        db  08Bh, 03Ch, 095h, 0B0h, 01Bh, 001h, 000h
        push   edx
        call   sub_1_7388
        pop    edx
        db  080h, 0A2h, 0B8h, 01Ch, 001h, 000h, 0EFh
loc_1_7387:
        ret
sub_1_7388:
        mov    al, 0
        call   sub_1_7738
        jb     SHORT loc_1_73B8
sub_1_7391:
        mov    byte ptr [ecx + 5], 0
        mov    byte ptr [ecx + 8], 96h
        mov    ax, word ptr [edi + 0ah]
        mov    word ptr [ecx + 10h], ax
        mov    al, byte ptr [edi + 9]
        mov    byte ptr [ecx + 0bh], al
        inc    byte ptr [edi + 5]
        inc    byte ptr [edi + 6]
        push   edi
        call   sub_1_75B4
        pop    edi
        test   al, al
        jne    SHORT loc_1_73B9
loc_1_73B8:
        ret
loc_1_73B9:
        dec    byte ptr [edi + 5]
        dec    byte ptr [edi + 6]
        call   sub_1_781C
        test   esp, esp
        ret
sub_1_73C7:
        push   esi
        push   edi
        push   ebx
        push   ecx
        db  08Bh, 035h, 07Ch, 023h, 001h, 000h
        test   esi, esi
        je     SHORT loc_1_73E3
        mov    eax, dword ptr [esi + 18h]
        db  0A3h, 07Ch, 023h, 001h, 000h
        mov    bx, word ptr [esi + 1ch]
        jmp    SHORT loc_1_7416
loc_1_73E3:
        push   edx
        push   ecx
        push   edx
        push   58h
        call   sub_1_8CB6
        pop    edx
        pop    edx
        pop    ecx
        pop    edx
        mov    esi, eax
        test   eax, eax
        je     SHORT loc_1_7434
        db  08Bh, 01Dh, 011h, 023h, 001h, 000h
        db  0FFh, 005h, 011h, 023h, 001h, 000h
        cli
        db  0A1h, 02Ch, 01Ch, 001h, 000h
        db  089h, 035h, 02Ch, 01Ch, 001h, 000h
        mov    dword ptr [esi], eax
        mov    byte ptr [esi + 1eh], 0
        sti
loc_1_7416:
        lea    edi, [esi + 4]
        mov    ecx, 13h
        xor    eax, eax
        rep stosd
        mov    eax, edi
        stosd
        stosd
        mov    word ptr [esi + 1ch], bx
        mov    byte ptr [esi + 1eh], 1
        mov    byte ptr [esi + 3eh], 1
        mov    eax, esi
loc_1_7434:
        pop    ecx
        pop    ebx
        pop    edi
        pop    esi
        ret
loc_1_7439:
        stc
        ret
loc_1_743B:
        push   edi
        call   sub_1_BB9C
        pop    edi
        cmp    eax, 1
        ret
loc_1_7446:
        push   edx
        mov    al, byte ptr [edi + 9]
        mov    ah, byte ptr [edi]
        push   eax
        push   32h
        push   1
        db  0CDh, 020h, 004h, 000h, 080h, 004h
        add    esp, 0ch
        push   edi
        call   sub_1_FC6C
        pop    ecx
        pop    edx
        mov    eax, eax
sub_1_7464:
        db  00Fh, 0ABh, 015h, 004h, 023h, 001h, 000h
        jae    SHORT loc_1_7446
        db  0F6h, 082h, 0B8h, 01Ch, 001h, 000h, 002h
        jne    SHORT loc_1_743B
        mov    al, 4
        call   sub_1_7738
        jb     SHORT loc_1_7439
        call   sub_1_73C7
        test   eax, eax
        je     SHORT loc_1_74CD
        mov    byte ptr [eax + 3fh], dl
        mov    dword ptr [ecx + 48h], eax
        mov    byte ptr [ecx + 12h], 2ah
        mov    byte ptr [ecx + 5], 7dh
        mov    byte ptr [ecx + 8], 91h
        mov    bl, byte ptr [edi + 9]
        mov    byte ptr [ecx + 0bh], bl
        mov    word ptr [ecx + 32h], 78f0h
        inc    byte ptr [edi + 1]
        inc    byte ptr [edi + 7]
        push   edi
        push   eax
        db  0BEh, 048h, 023h, 001h, 000h
        lea    edi, [ecx + 22h]
        movsd
        movsd
        movsd
        movsd
        call   sub_1_75B4
        pop    esi
        pop    edi
        test   al, al
        jne    SHORT loc_1_74D4
        call   sub_1_7344
        xor    eax, eax
        ret
loc_1_74CD:
        call   sub_1_781C
        stc
        ret
loc_1_74D4:
        mov    byte ptr [esi + 1eh], 0
        db  0A1h, 07Ch, 023h, 001h, 000h
        mov    dword ptr [esi + 18h], eax
        db  089h, 035h, 07Ch, 023h, 001h, 000h
        dec    byte ptr [edi + 1]
        dec    byte ptr [edi + 7]
        call   sub_1_781C
        test   esp, esp
        ret
sub_1_74F4:
        movzx  edx, byte ptr [edi + 3fh]
        movzx  eax, byte ptr [edi + 40h]
        db  08Bh, 034h, 095h, 0B0h, 01Bh, 001h, 000h
        cmp    dword ptr [esi + eax*4 + 0f2h], edi
        jne    SHORT loc_1_7545
        db  0F6h, 082h, 0B8h, 01Ch, 001h, 000h, 002h
        jne    SHORT loc_1_7548
        push   eax
        mov    al, 4
        call   sub_1_7738
        pop    eax
        jb     SHORT loc_1_7544
        mov    dword ptr [esi + eax*4 + 0f2h], 0
        mov    byte ptr [ecx + 0ah], al
        mov    dword ptr [ecx + 48h], edi
        mov    byte ptr [ecx + 5], 7eh
        mov    byte ptr [ecx + 8], 92h
        push   edi
        call   sub_1_75B4
        pop    edi
        test   al, al
        jne    SHORT loc_1_755F
loc_1_7544:
        ret
loc_1_7545:
        xor    eax, eax
        ret
loc_1_7548:
        mov    dword ptr [esi + eax*4 + 0f2h], 0
        mov    ecx, dword ptr [edi + 14h]
        xor    eax, eax
        mov    dword ptr [ecx], eax
        mov    word ptr [ecx + 2ah], ax
        ret
loc_1_755F:
        call   sub_1_781C
        test   esp, esp
        ret
        db  090h
sub_1_7568:
        db  08Bh, 03Dh, 014h, 028h, 001h, 000h
        mov    edx, dword ptr [edi + 20h]
        or     byte ptr [edx + 9], 80h
        mov    ebx, ecx
        mov    ecx, dword ptr [edi + 1ch]
        mov    word ptr [ecx + 10h], ax
        mov    al, 94h
        test   ebx, ebx
        je     SHORT loc_1_758D
        mov    al, 97h
        mov    word ptr [ecx + 12h], bx
        mov    dword ptr [ecx + 14h], esi
loc_1_758D:
        mov    byte ptr [ecx + 8], al
        mov    byte ptr [ecx + 5], 7fh
        mov    dword ptr [ecx + 48h], 0
        nop
sub_1_759C:
        db  0A1h, 014h, 028h, 001h, 000h
        mov    eax, dword ptr [eax + 30h]
        cmp    byte ptr [eax + 1eh], 2
        jne    SHORT loc_1_75B6
        db  0A1h, 020h, 01Ch, 001h, 000h
        cmp    byte ptr [eax], 2
        je     SHORT loc_1_75B6
sub_1_75B4:
        jmp    SHORT sub_1_75C0
loc_1_75B6:
        mov    al, 0ah
        mov    byte ptr [ecx + 9], al
        mov    byte ptr [ecx + 39h], al
        ret
        db  090h
sub_1_75C0:
        push   ecx
        or     byte ptr [ecx + 5], 80h
        movzx  ebx, byte ptr [ecx + 7]
        push   ebx
        db  0F6h, 083h, 0B8h, 01Ch, 001h, 000h, 002h
        jne    SHORT loc_1_7611
        db  0FFh, 005h, 008h, 023h, 001h, 000h
        lea    ebx, [ecx + 8]
        cmp    byte ptr [ecx + 8], 31h
        je     SHORT loc_1_75EB
        db  0CDh, 020h, 002h, 000h, 014h, 000h
        cld
        jmp    SHORT loc_1_761A
loc_1_75EB:
        db  0CDh, 020h, 002h, 000h, 014h, 000h
        cld
        pop    esi
        pop    ecx
        db  08Bh, 01Dh, 028h, 01Ch, 001h, 000h
        movzx  ebx, word ptr [ebx + 68h]
        db  08Bh, 03Ch, 0B5h, 0B0h, 01Bh, 001h, 000h
        test   al, al
        jne    SHORT loc_1_7637
        cmp    byte ptr [ecx + 8], 31h
        je     SHORT loc_1_7637
        jmp    SHORT loc_1_7631
loc_1_7611:
        push   ecx
        call   sub_1_C134
        pop    ecx
        xor    eax, eax
loc_1_761A:
        pop    esi
        pop    ecx
        db  08Bh, 01Dh, 028h, 01Ch, 001h, 000h
        movzx  ebx, word ptr [ebx + 68h]
        db  08Bh, 03Ch, 0B5h, 0B0h, 01Bh, 001h, 000h
        test   al, al
        jne    SHORT loc_1_7637
loc_1_7631:
        inc    word ptr [edi + ebx*2 + 10h]
        ret
loc_1_7637:
        db  0FFh, 005h, 000h, 023h, 001h, 000h
        db  0FFh, 00Dh, 008h, 023h, 001h, 000h
        je     SHORT loc_1_764E
loc_1_7645:
        mov    byte ptr [ecx + 39h], al
        inc    word ptr [edi + ebx*2 + 12h]
        ret
loc_1_764E:
        push   eax
        push   edx
        db  0A1h, 020h, 01Ch, 001h, 000h
        cmp    byte ptr [eax], 2
        jne    SHORT loc_1_7666
        db  068h, 008h, 023h, 001h, 000h
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        pop    edx
loc_1_7666:
        pop    edx
        pop    eax
        jmp    SHORT loc_1_7645
        db  08Bh, 0C0h
sub_1_766C:
        cli
        test   byte ptr [ecx + 5], 80h
        jne    SHORT loc_1_767A
        mov    al, byte ptr [ecx + 9]
        sti
        test   al, al
        ret
loc_1_767A:
        lea    eax, [ecx + 48h]
        call   sub_1_875C
        jmp    SHORT sub_1_766C
sub_1_7684:
        pushfd
        cli
        db  0A1h, 0F8h, 022h, 001h, 000h
        test   eax, eax
        je     SHORT loc_1_76AC
        mov    ecx, dword ptr [eax + 50h]
        db  089h, 00Dh, 0F8h, 022h, 001h, 000h
        popfd
        push   ecx
        push   edx
        push   eax
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        db  0FFh, 00Dh, 0F4h, 022h, 001h, 000h
        jmp    SHORT sub_1_7684
loc_1_76AC:
        popfd
        ret
sub_1_76AE:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ecx
        push   edx
        db  0BAh, 0A8h, 022h, 001h, 000h
        mov    ecx, dword ptr [ebp + 8]
        pushfd
        cli
loc_1_76BE:
        mov    eax, dword ptr [edx + 50h]
        test   eax, eax
        je     SHORT loc_1_76DC
        cmp    eax, ecx
        jne    SHORT loc_1_76D8
        mov    eax, dword ptr [eax + 50h]
        mov    dword ptr [edx + 50h], eax
        mov    dword ptr [ecx + 50h], 0ffffffffh
        jmp    SHORT loc_1_76DC
loc_1_76D8:
        mov    edx, eax
        jmp    SHORT loc_1_76BE
loc_1_76DC:
        popfd
        pop    edx
        pop    ecx
        pop    eax
        leave
        ret
sub_1_76E2:
        push   esi
        pushfd
        cli
        db  0BEh, 060h, 01Bh, 001h, 000h
loc_1_76EA:
        mov    eax, dword ptr [esi + 4ch]
        test   eax, eax
        je     SHORT loc_1_7709
        lea    ecx, [edi + 4]
        cmp    ecx, dword ptr [eax - 4]
        je     SHORT loc_1_76FD
        mov    esi, eax
        jmp    SHORT loc_1_76EA
loc_1_76FD:
        db  0FFh, 00Dh, 0F4h, 022h, 001h, 000h
        mov    ecx, dword ptr [eax + 4ch]
        mov    dword ptr [esi + 4ch], ecx
loc_1_7709:
        popfd
        pop    esi
        ret
sub_1_770C:
        pushfd
        cli
        db  0A1h, 0ACh, 01Bh, 001h, 000h
        test   eax, eax
        je     SHORT loc_1_7734
        db  0FFh, 00Dh, 0F4h, 022h, 001h, 000h
        mov    ecx, dword ptr [eax + 4ch]
        db  089h, 00Dh, 0ACh, 01Bh, 001h, 000h
        popfd
        push   ecx
        push   edx
        push   eax
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        jmp    SHORT sub_1_770C
loc_1_7734:
        popfd
        ret
        db  08Bh, 0C0h
sub_1_7738:
        and    eax, 7fh
        jne    SHORT loc_1_7745
        db  08Ah, 082h, 0B8h, 01Ch, 001h, 000h
        and    al, 3
loc_1_7745:
        test   al, al
        jne    SHORT loc_1_779D
        db  08Dh, 01Ch, 085h, 058h, 023h, 001h, 000h
        cli
        mov    ecx, dword ptr [ebx]
        jecxz  loc_1_779C
        mov    eax, dword ptr [ecx + 4ch]
        mov    dword ptr [ebx], eax
        sti
loc_1_775B:
        db  0FFh, 005h, 0F4h, 022h, 001h, 000h
        db  08Bh, 01Ch, 095h, 0B0h, 01Bh, 001h, 000h
        mov    al, byte ptr [ebx]
        mov    dword ptr [ecx], 624eh
        mov    byte ptr [ecx + 38h], al
        mov    byte ptr [ecx + 7], dl
        mov    byte ptr [ecx + 5], 0
        cmp    dl, 10h
        je     SHORT loc_1_778A
        db  0C7h, 041h, 034h, 06Ch, 07Ah, 000h, 000h
        clc
        ret
loc_1_7788:
        stc
        ret
loc_1_778A:
        pushfd
        cli
        db  0A1h, 0F8h, 022h, 001h, 000h
        mov    dword ptr [ecx + 50h], eax
        db  089h, 00Dh, 0F8h, 022h, 001h, 000h
        popfd
        ret
loc_1_779C:
        sti
loc_1_779D:
        test   eax, eax
        je     SHORT loc_1_7788
        push   eax
        push   edi
        mov    edi, 5ch
        cmp    al, 4
        je     SHORT loc_1_77B3
        db  003h, 03Ch, 085h, 034h, 023h, 001h, 000h
loc_1_77B3:
        call   sub_1_76E2
        test   eax, eax
        jne    SHORT loc_1_77C7
        push   ecx
        push   edx
        push   edi
        call   sub_1_8CB6
        pop    edx
        pop    edx
        pop    ecx
loc_1_77C7:
        xchg   ecx, eax
        pop    edi
        pop    eax
        test   ecx, ecx
        je     SHORT loc_1_7788
        mov    byte ptr [ecx + 4], 0ffh
        cmp    al, 4
        je     SHORT loc_1_775B
        mov    ebx, ecx
        add    ebx, 5ch
        cmp    al, 2
        jne    SHORT loc_1_77E5
        mov    dword ptr [ecx + 34h], ebx
        add    ebx, 1eh
loc_1_77E5:
        mov    dword ptr [ecx + 0ch], ebx
        jmp    NEAR PTR loc_1_775B
sub_1_77ED:
        cmp    byte ptr [ecx + 7], 10h
        jne    SHORT loc_1_7800
        push   ecx
        call   sub_1_76AE
        add    esp, 4
        mov    byte ptr [ecx + 7], 0
loc_1_7800:
        pushfd
        cmp    byte ptr [ecx + 4], 0ffh
        jne    SHORT loc_1_7836
        cli
        db  08Bh, 01Dh, 0ACh, 01Bh, 001h, 000h
        db  089h, 00Dh, 0ACh, 01Bh, 001h, 000h
        mov    dword ptr [ecx + 4ch], ebx
        popfd
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_781C:
        mov    dword ptr [ecx], 626eh
        cmp    byte ptr [ecx + 7], 10h
        jne    SHORT loc_1_7835
        push   ecx
        call   sub_1_76AE
        add    esp, 4
        mov    byte ptr [ecx + 7], 0
loc_1_7835:
        pushfd
loc_1_7836:
        db  0FFh, 00Dh, 0F4h, 022h, 001h, 000h
        movzx  eax, byte ptr [ecx + 4]
        cmp    al, 0ffh
        je     SHORT loc_1_786E
        db  08Dh, 01Ch, 085h, 058h, 023h, 001h, 000h
        cli
        mov    byte ptr [ecx + 5], 0
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx + 4ch], eax
        mov    dword ptr [ebx], ecx
        db  080h, 03Dh, 0FCh, 022h, 001h, 000h, 000h
        jne    SHORT loc_1_7862
        popfd
        ret
loc_1_7862:
        db  0B8h, 030h, 01Ch, 001h, 000h
        call   sub_1_8BC0
        popfd
        ret
loc_1_786E:
        push   ecx
        call   sub_1_799D
        add    esp, 4
        popfd
        ret
sub_1_7879:
        push   edx
        add    eax, 0fffh
        shr    eax, 0ch
        je     SHORT loc_1_78A2
        push   eax
        push   eax
        call   sub_1_8D2A
        add    esp, 4
        mov    ecx, eax
        xchg   edx, eax
        or     edx, eax
        je     SHORT loc_1_789E
        db  0FFh, 005h, 0ECh, 022h, 001h, 000h
        pop    edx
        pop    edx
        ret
loc_1_789E:
        pop    eax
        shl    eax, 0bh
loc_1_78A2:
        stc
        pop    edx
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_78A8:
        db  0FFh, 00Dh, 0ECh, 022h, 001h, 000h
        push   eax
        push   ecx
        push   edx
        push   eax
        call   sub_1_8DD1
        add    esp, 4
        pop    edx
        pop    ecx
        pop    eax
        ret
        db  08Bh, 0C0h
sub_1_78C0:
        cmp    ecx, 1
        push   edx
        movzx  edx, al
        mov    al, 4
        call   sub_1_7738
        pop    edx
        jb     SHORT loc_1_78E1
        mov    ebx, ecx
        mov    byte ptr [ebx + 6], 0
        db  0C7h, 043h, 034h, 004h, 07Ah, 000h, 000h
        mov    dword ptr [ebx + 48h], ebx
loc_1_78E1:
        ret
        db  08Bh, 0C0h
sub_1_78E4:
        cmp    ecx, 1
        mov    ecx, ebx
        call   sub_1_781C
        ret
        db  090h
sub_1_78F0:
        push   ebx
        push   esi
        push   edi
        push   ecx
        mov    ebx, dword ptr [esp + 14h]
        mov    esi, dword ptr [esp + 18h]
        xor    ecx, ecx
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        and    eax, 0ffh
        pop    ecx
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
sub_1_7910:
        push   dword ptr [esp + 4]
        db  0CDh, 020h, 010h, 000h, 081h, 004h
        add    esp, 4
        ret
sub_1_791E:
        push   ebx
        push   esi
        push   edi
        mov    ebx, 0ah
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        cld
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_7937:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 14h]
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_794F:
        push   ebx
        push   esi
        push   edi
        mov    ebx, 1
        mov    edx, dword ptr [esp + 10h]
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        cld
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_7966:
        db  08Bh, 015h, 00Ch, 023h, 001h, 000h
        test   edx, edx
        je     SHORT loc_1_7995
        mov    eax, dword ptr [edx + 4]
        db  0A3h, 00Ch, 023h, 001h, 000h
        mov    eax, edx
        mov    edx, dword ptr [eax]
        push   eax
        push   ecx
        push   edx
        push   edx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        pop    eax
        push   eax
        db  0CDh, 020h, 00Eh, 000h, 040h, 000h
        add    esp, 4
        jmp    SHORT loc_1_7966
loc_1_7995:
        db  080h, 025h, 010h, 023h, 001h, 000h, 0FEh
        ret
sub_1_799D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edx
        push   ecx
        push   eax
        mov    eax, 8
        push   eax
        db  0CDh, 020h, 00Dh, 000h, 040h, 000h
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_79F9
        mov    edx, dword ptr [ebp + 8]
        db  08Bh, 035h, 00Ch, 023h, 001h, 000h
        mov    dword ptr [eax], edx
        mov    dword ptr [eax + 4], esi
        db  0A3h, 00Ch, 023h, 001h, 000h
        db  0F6h, 005h, 010h, 023h, 001h, 000h, 001h
        jne    SHORT loc_1_79F2
        db  080h, 00Dh, 010h, 023h, 001h, 000h, 001h
        xor    eax, eax
        xor    ebx, ebx
        mov    ecx, 2
        db  0BEh, 066h, 079h, 000h, 000h
        mov    edx, dword ptr [ebp + 8]
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
loc_1_79F2:
        pop    eax
        pop    ecx
        pop    edx
        pop    esi
        pop    ebx
        leave
        ret
loc_1_79F9:
        int3
        pop    eax
        pop    ecx
        pop    edx
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h
loc_1_7A04:
        db  0FFh, 00Dh, 008h, 023h, 001h, 000h
        je     SHORT loc_1_7A2C
loc_1_7A0C:
        lea    ecx, [ebx - 8]
        and    byte ptr [ecx + 5], 7fh
        cmp    byte ptr [ecx + 6], 2
        je     SHORT loc_1_7A28
        cmp    dword ptr [ecx + 4ch], 0
        je     SHORT loc_1_7A27
        lea    eax, [ecx + 4ch]
        call   sub_1_8BC0
loc_1_7A27:
        ret
loc_1_7A28:
        call   dword ptr [ecx + 4ch]
        ret
loc_1_7A2C:
        push   eax
        push   edx
        db  0A1h, 020h, 01Ch, 001h, 000h
        cmp    byte ptr [eax], 2
        jne    SHORT loc_1_7A44
        db  068h, 008h, 023h, 001h, 000h
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        pop    edx
loc_1_7A44:
        pop    edx
        pop    eax
        jmp    SHORT loc_1_7A0C
sub_1_7A48:
        mov    ecx, dword ptr [esp + 4]
        jmp    SHORT loc_1_7A77
loc_1_7A4E:
        push   eax
        push   edx
        db  0A1h, 020h, 01Ch, 001h, 000h
        cmp    byte ptr [eax], 2
        jne    SHORT loc_1_7A66
        db  068h, 008h, 023h, 001h, 000h
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        pop    edx
loc_1_7A66:
        pop    edx
        pop    eax
        jmp    SHORT loc_1_7A74
        db  0C3h, 090h
loc_1_7A6C:
        db  0FFh, 00Dh, 008h, 023h, 001h, 000h
        je     SHORT loc_1_7A4E
loc_1_7A74:
        lea    ecx, [ebx - 8]
loc_1_7A77:
        and    byte ptr [ecx + 5], 7fh
        movzx  edx, byte ptr [ecx + 7]
        db  0F6h, 082h, 0B8h, 01Ch, 001h, 000h, 040h
        jne    NEAR PTR loc_1_7B8B
        db  08Bh, 034h, 095h, 0B0h, 01Bh, 001h, 000h
        cmp    esi, 0fffff000h
        je     NEAR PTR loc_1_7B8B
        mov    ax, word ptr [ecx + 8]
        cmp    al, 96h
        je     SHORT loc_1_7AB1
        cmp    al, 0c8h
        jne    NEAR PTR loc_1_7B78
        jmp    SHORT loc_1_7AB4
loc_1_7AB1:
        dec    byte ptr [esi + 6]
loc_1_7AB4:
        dec    byte ptr [esi + 5]
        cmp    byte ptr [esi + 6], 0
        je     NEAR PTR loc_1_7B41
loc_1_7AC1:
        mov    al, byte ptr [esi + 5]
        cmp    byte ptr [esi + 4], al
        jbe    SHORT loc_1_7AD6
        db  0B8h, 030h, 01Ch, 001h, 000h
        call   sub_1_8BC0
        mov    ah, byte ptr [ecx + 9]
loc_1_7AD6:
        movzx  edi, byte ptr [ecx + 0ah]
        mov    edi, dword ptr [esi + edi*4 + 0f2h]
        test   edi, edi
        je     NEAR PTR loc_1_7B8B
        mov    dword ptr [ecx + 48h], edi
        test   ah, ah
        jne    SHORT loc_1_7B0C
        mov    ebx, dword ptr [ecx + 0ch]
        cmp    dword ptr [ebx], 424d53ffh
        jne    NEAR PTR loc_1_7B8B
        mov    bx, word ptr [edi + 3ch]
        mov    word ptr [ecx + 3ch], bx
        inc    bh
        mov    byte ptr [edi + 3dh], bh
loc_1_7B0C:
        cli
        db  0A1h, 0C4h, 031h, 001h, 000h
        mov    dword ptr [ecx + 44h], eax
        xor    ebx, ebx
        mov    dword ptr [ecx + 4ch], ebx
loc_1_7B1A:
        db  087h, 01Dh, 018h, 01Ch, 001h, 000h
        test   ebx, ebx
        je     SHORT loc_1_7B1A
        mov    dword ptr [ebx + 4ch], ecx
        db  089h, 00Dh, 018h, 01Ch, 001h, 000h
        db  0F0h, 0FEh, 005h, 0CBh, 01Ch, 001h, 000h
        sti
        db  0B8h, 054h, 001h, 000h, 000h
        call   sub_1_8998
        clc
        ret
loc_1_7B41:
        db  00Fh, 0BAh, 0AAh, 0B8h, 01Ch, 001h, 000h, 004h
        jb     NEAR PTR loc_1_7AC1
        push   eax
        push   ecx
        push   esi
        mov    eax, 1000h
        db  08Bh, 01Dh, 014h, 031h, 001h, 000h
        mov    ecx, 9
        db  0BEh, 068h, 073h, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        pop    esi
        pop    ecx
        pop    eax
        jmp    NEAR PTR loc_1_7AC1
        db  02Eh, 08Bh, 0C0h
loc_1_7B78:
        cmp    al, 0a0h
        je     SHORT loc_1_7B8B
        mov    al, byte ptr [ecx + 5]
        cmp    al, 7fh
        je     SHORT loc_1_7B91
        cmp    al, 7dh
        je     SHORT loc_1_7B9A
        test   ah, ah
        jne    SHORT loc_1_7B0C
loc_1_7B8B:
        call   sub_1_77ED
        ret
loc_1_7B91:
        lea    eax, [ecx + 48h]
        call   sub_1_8BC0
        ret
loc_1_7B9A:
        dec    byte ptr [esi + 1]
        jne    SHORT loc_1_7BA9
        db  0B8h, 030h, 01Ch, 001h, 000h
        call   sub_1_8BC0
loc_1_7BA9:
        cmp    byte ptr [ecx + 9], 0
        jne    NEAR PTR loc_1_7B0C
        push   esi
        lea    esi, [ecx + 12h]
        mov    edx, dword ptr [ecx + 48h]
        lea    edi, [edx + 4]
        cld
        movsd
        movsd
        movsd
        movsd
        pop    esi
        db  0A1h, 028h, 028h, 001h, 000h
        mov    dword ptr [edx + 30h], eax
        mov    dword ptr [edx + 34h], eax
        mov    byte ptr [edx + 1eh], 2
        movzx  eax, byte ptr [ecx + 0ah]
        mov    byte ptr [edx + 40h], al
        mov    dword ptr [esi + eax*4 + 0f2h], edx
        call   sub_1_77ED
        ret
        db  000h, 000h
VSERVER_Service_01:
        push   ebx
        db  08Bh, 01Dh, 0E8h, 025h, 001h, 000h
        cmp    ebx, 28h
        jae    SHORT loc_1_7C0C
        db  0FFh, 005h, 0E8h, 025h, 001h, 000h
        db  066h, 089h, 004h, 05Dh, 0F5h, 024h, 001h, 000h
        db  089h, 034h, 09Dh, 048h, 025h, 001h, 000h
        pop    ebx
        clc
        ret
loc_1_7C0C:
        pop    ebx
        stc
        ret
sub_1_7C0F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cmp    ebx, 11h
        jb     SHORT loc_1_7C27
loc_1_7C1D:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_7C27:
        push   ecx
        push   edx
        push   4f2h
        call   sub_1_8CB6
        pop    edx
        pop    edx
        pop    ecx
        test   eax, eax
        je     SHORT loc_1_7C1D
        mov    esi, eax
        mov    ecx, 4f2h
        mov    edi, eax
        xor    eax, eax
        rep stosb
        mov    edx, ebx
        db  0FFh, 005h, 0F4h, 01Bh, 001h, 000h
        db  089h, 034h, 095h, 0B0h, 01Bh, 001h, 000h
        mov    byte ptr [esi], bl
        mov    byte ptr [esi + 9], 0ffh
        mov    byte ptr [esi + 3], 3
        mov    byte ptr [esi + 4], 2
        push   esi
        call   sub_1_119F8
        add    esp, 4
        cmp    bl, 10h
        je     NEAR PTR loc_1_7D09
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_1_7C8F
        mov    word ptr [esi + 0ah], 0b68h
        mov    byte ptr [esi + 2], 0bh
        db  0C6h, 082h, 0B8h, 01Ch, 001h, 000h, 009h
        jmp    SHORT loc_1_7CE4
loc_1_7C8F:
        db  0C6h, 082h, 0B8h, 01Ch, 001h, 000h, 000h
        mov    word ptr [esi + 0ah], 5b4h
        mov    byte ptr [esi + 2], 4
        push   edx
        mov    eax, 1c84h
        push   204h
        push   eax
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        add    esp, 8
        pop    edx
        cmp    eax, 1
        jb     SHORT loc_1_7D2B
        mov    edi, eax
        mov    esi, 5
loc_1_7CC2:
        mov    al, 4
        call   sub_1_7738
        jb     SHORT loc_1_7D2B
        mov    byte ptr [ecx + 4], 0
        mov    dword ptr [ecx + 0ch], edi
        mov    byte ptr [ecx + 39h], 0
        call   sub_1_781C
        add    edi, 5b4h
        dec    esi
        jne    SHORT loc_1_7CC2
loc_1_7CE4:
        push   edx
        call   sub_1_7D58
        pop    edx
        jb     SHORT loc_1_7D2B
loc_1_7CED:
        db  08Bh, 004h, 095h, 0B0h, 01Bh, 001h, 000h
        db  08Bh, 01Dh, 0A6h, 024h, 001h, 000h
        db  0A3h, 0A6h, 024h, 001h, 000h
        mov    dword ptr [eax + 0ch], ebx
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_7D09:
        mov    word ptr [esi + 0ah], 596h
        mov    byte ptr [esi + 2], 16h
        mov    byte ptr [esi + 9], 1
        db  0C6h, 082h, 0B8h, 01Ch, 001h, 000h, 002h
        push   edx
        push   esi
        call   sub_1_C3A4
        pop    esi
        pop    edx
        test   eax, eax
        je     SHORT loc_1_7CED
loc_1_7D2B:
        db  0FFh, 00Dh, 0F4h, 01Bh, 001h, 000h
        mov    eax, 0fffff000h
        db  087h, 004h, 095h, 0B0h, 01Bh, 001h, 000h
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_1_7D43
loc_1_7D43:
        push   ecx
        push   edx
        push   eax
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_7D58:
        mov    al, 4
        call   sub_1_7738
        jb     SHORT loc_1_7D87
        db  0C7h, 041h, 034h, 0AAh, 07Dh, 000h, 000h
        mov    byte ptr [ecx + 8], 0b0h
        db  0BEh, 048h, 023h, 001h, 000h
        lea    edi, [ecx + 22h]
        movsd
        movsd
        movsd
        movsd
        lea    ebx, [ecx + 8]
        push   ecx
        db  0CDh, 020h, 002h, 000h, 014h, 000h
        pop    ecx
        test   al, al
        jne    SHORT loc_1_7D88
loc_1_7D87:
        ret
loc_1_7D88:
        call   sub_1_781C
        stc
        ret
sub_1_7D8F:
        db  080h, 03Dh, 048h, 023h, 001h, 000h, 000h
        je     SHORT loc_1_7DA7
        call   sub_1_8C8D
        db  0B8h, 0ECh, 06Eh, 000h, 000h
        call   sub_1_89F1
loc_1_7DA7:
        xor    eax, eax
        ret
loc_1_7DAA:
        lea    ecx, [ebx - 8]
        cmp    byte ptr [ecx + 9], 0
        jne    SHORT loc_1_7DD6
        mov    al, byte ptr [ecx + 0bh]
        movzx  ebx, byte ptr [ecx + 7]
        db  08Bh, 01Ch, 09Dh, 0B0h, 01Bh, 001h, 000h
        cmp    ebx, 0fffff000h
        je     SHORT loc_1_7DCC
        mov    byte ptr [ebx + 9], al
loc_1_7DCC:
        db  0B8h, 030h, 01Ch, 001h, 000h
        call   sub_1_8BC0
loc_1_7DD6:
        call   sub_1_77ED
        ret
sub_1_7DDC:
        pushad
        db  0A1h, 020h, 01Ch, 001h, 000h
        mov    byte ptr [eax], 2
        db  0B8h, 030h, 01Ch, 001h, 000h
        call   sub_1_8BC0
        popad
        ret
sub_1_7DF1:
        cmp    eax, 1bh
        ja     SHORT loc_1_7DFE
        db  0FFh, 014h, 085h, 0ECh, 025h, 001h, 000h
        ret
loc_1_7DFE:
        mov    ax, 32h
        ret
sub_1_7E03:
        sub    al, al
        mov    ecx, 0ffffh
        mov    ebx, edi
        repne scasb
        mov    ecx, edi
        sub    ecx, ebx
        ret
sub_1_7E13:
        push   eax
        mov    ebx, esi
        xchg   edi, esi
        push   ecx
        sub    al, al
        repne scasb
        mov    eax, ecx
        pop    ecx
        jne    SHORT loc_1_7E23
        inc    eax
loc_1_7E23:
        sub    ecx, eax
        mov    edi, ebx
        xchg   edi, esi
        mov    ebx, ecx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        xchg   ecx, eax
        mov    ebx, ecx
        shr    ecx, 2
        rep stosd
        mov    ecx, ebx
        and    ecx, 3
        rep stosb
        pop    eax
        ret
sub_1_7E48:
        mov    edi, edx
        call   sub_1_7E03
        mov    eax, ecx
        db  08Bh, 01Dh, 000h, 01Ch, 001h, 000h
loc_1_7E57:
        db  081h, 0FBh, 000h, 01Ch, 001h, 000h
        je     SHORT loc_1_7E74
        cmp    byte ptr [ebx + 8], 2
        jne    SHORT loc_1_7E70
        lea    esi, [ebx + 11h]
        mov    edi, edx
        mov    ecx, eax
        repe cmpsb
        je     SHORT loc_1_7E76
loc_1_7E70:
        mov    ebx, dword ptr [ebx]
        jmp    SHORT loc_1_7E57
loc_1_7E74:
        sub    ebx, ebx
loc_1_7E76:
        ret
sub_1_7E77:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        call   sub_1_7E48
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_7E89:
        mov    eax, 32h
        ret
loc_1_7E8F:
        sub    eax, eax
loc_1_7E91:
        ret
loc_1_7E92:
        sub    eax, eax
        ret
loc_1_7E95:
        sub    byte ptr [eax], al
        add    byte ptr [eax], al
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [esi], ch
        add    byte ptr [eax], al
        scasd
        add    byte ptr [eax], al
        add    al, cl
        add    byte ptr [eax], al
        add    dl, bh
        add    byte ptr [eax], al
        add    byte ptr [ecx + eax], ch
        add    byte ptr [eax], al
        hlt
        add    dword ptr [eax], eax
        add    byte ptr [eax + 28000013h], cl
        and    eax, dword ptr [eax]
        db  000h
loc_1_7EBD:
        db  08Bh, 01Dh, 020h, 01Ch, 001h, 000h
        cmp    ebx, 0fffff000h
        je     SHORT loc_1_7EE1
        cmp    byte ptr [ebx], 2
        je     SHORT loc_1_7EE1
        db  0A1h, 09Ch, 023h, 001h, 000h
        db  08Bh, 004h, 085h, 091h, 07Eh, 000h, 000h
        mov    dword ptr [esi], eax
        sub    eax, eax
        ret
loc_1_7EE1:
        mov    eax, 842h
        ret
sub_1_7EE7:
        push   ebx
        test   eax, eax
        je     SHORT loc_1_7EF1
        cmp    eax, 0ah
        jbe    SHORT loc_1_7F09
loc_1_7EF1:
        mov    ebx, eax
        mov    eax, 0ah
loc_1_7EF8:
        sub    eax, 1
        jb     SHORT loc_1_7F06
        db  03Bh, 01Ch, 085h, 095h, 07Eh, 000h, 000h
        jb     SHORT loc_1_7EF8
loc_1_7F06:
        adc    eax, 1
loc_1_7F09:
        pop    ebx
        ret
loc_1_7F0B:
        db  08Bh, 01Dh, 020h, 01Ch, 001h, 000h
        cmp    ebx, 0fffff000h
        je     SHORT loc_1_7EE1
        cmp    byte ptr [ebx], 2
        je     SHORT loc_1_7EE1
        mov    eax, dword ptr [esi]
sub_1_7F20:
        call   sub_1_7EE7
        db  0A3h, 09Ch, 023h, 001h, 000h
        call   sub_1_8B80
        sub    eax, eax
        ret
sub_1_7F32:
        push   ebp
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  08Bh, 02Dh, 018h, 028h, 001h, 000h
        mov    word ptr [ebp + 1ch], 2a00h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        cld
        mov    ecx, dword ptr [esp + 8]
        mov    ax, word ptr [ebp + 1ch]
        mov    byte ptr [ecx + 14h], al
        mov    ax, word ptr [ebp + 18h]
        mov    word ptr [ecx + 12h], ax
        mov    ax, word ptr [ebp + 14h]
        mov    byte ptr [ecx + 11h], ah
        mov    byte ptr [ecx + 10h], al
        mov    word ptr [ebp + 1ch], 2c00h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        cld
        mov    ax, word ptr [ebp + 18h]
        mov    byte ptr [ecx + 8], ah
        mov    byte ptr [ecx + 9], al
        mov    ax, word ptr [ebp + 14h]
        mov    byte ptr [ecx + 0ah], ah
        mov    byte ptr [ecx + 0bh], al
        db  0CDh, 020h, 060h, 000h, 040h, 000h
        lea    edx, [eax + eax*2]
        mov    word ptr [ecx + 0ch], ax
        shl    edx, 2
        lea    edx, [edx + edx*4]
        db  0A1h, 028h, 028h, 001h, 000h
        sub    eax, edx
        mov    dword ptr [ecx], eax
        db  0A1h, 0C4h, 031h, 001h, 000h
        mov    dword ptr [ecx + 4], eax
        mov    word ptr [ecx + 0eh], 3e8h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        pop    ebp
        ret
loc_1_7FC6:
        mov    eax, dword ptr [esi + 0ch]
        cmp    eax, 0
        jne    SHORT loc_1_7FD5
        mov    ecx, 30ah
        jmp    SHORT loc_1_7FDA
loc_1_7FD5:
        cmp    eax, -1
        jne    SHORT loc_1_7FDE
loc_1_7FDA:
        xor    eax, eax
        jmp    SHORT loc_1_8016
loc_1_7FDE:
        sub    eax, 3e8h
        jl     SHORT loc_1_8011
        db  08Bh, 00Dh, 0E8h, 025h, 001h, 000h
        db  0BFh, 0F5h, 024h, 001h, 000h
        test   edi, edi
        repne scasw
        jne    SHORT loc_1_8011
        db  081h, 0EFh, 0F7h, 024h, 001h, 000h
        push   dword ptr [esi + 14h]
        push   dword ptr [esi + 10h]
        db  0FFh, 034h, 07Dh, 048h, 025h, 001h, 000h
        call   sub_1_4BBC
        jmp    SHORT loc_1_8016
loc_1_8011:
        mov    eax, 1
loc_1_8016:
        clc
        ret
sub_1_8018:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        db  0CDh, 020h, 006h, 000h, 014h, 000h
        jae    SHORT loc_1_802B
        mov    eax, 0ffffffffh
loc_1_802B:
        leave
        ret
        db  000h, 000h, 000h
loc_1_8030:
        db  0B8h, 030h, 01Ch, 001h, 000h
        call   sub_1_8BC0
        db  0BDh, 0F4h, 026h, 001h, 000h
        lea    edi, [ebx + 3]
        movzx  eax, word ptr [ebx + 1]
        mov    edx, 0ffffffffh
loc_1_804B:
        mov    ebx, ebp
        cmp    byte ptr [edi], 2
        jne    SHORT loc_1_8083
        inc    edi
        dec    eax
        inc    dl
loc_1_8056:
        movzx  ecx, byte ptr [ebx + 8]
        mov    esi, dword ptr [ebx + 4]
        push   edi
        repe cmpsb
        pop    edi
        je     SHORT loc_1_8088
        add    ebx, 0ah
        db  080h, 03Dh, 0C9h, 01Ch, 001h, 000h, 000h
        jne    SHORT loc_1_8079
        db  081h, 0FBh, 03Ah, 027h, 001h, 000h
        jb     SHORT loc_1_8056
        jmp    SHORT loc_1_8090
loc_1_8079:
        db  081h, 0FBh, 030h, 027h, 001h, 000h
        jb     SHORT loc_1_8056
        jmp    SHORT loc_1_8090
loc_1_8083:
        jmp    NEAR PTR loc_1_000A
loc_1_8088:
        cmp    ebp, ebx
        ja     SHORT loc_1_8090
        mov    ebp, ebx
        mov    dh, dl
loc_1_8090:
        xchg   ecx, eax
        mov    al, 0
        repne scasb
        jecxz  loc_1_809A
        xchg   ecx, eax
        jmp    SHORT loc_1_804B
loc_1_809A:
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    edi, dword ptr [esi + 30h]
        call   sub_1_2F30
        sete   al
        or     byte ptr [edi + 1fh], al
        mov    al, byte ptr ds:[ebp + 9]
        mov    byte ptr [edi + 3eh], al
        mov    ebx, dword ptr [esi + 28h]
        movsx  eax, dh
        mov    word ptr [ebx + 1], ax
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        jne    SHORT loc_1_80DE
loc_1_80C8:
        lea    eax, [edi + 2ch]
        mov    dword ptr [edi + 24h], eax
        mov    byte ptr [edi + 4eh], 8
loc_1_80D2:
        notrack call dword ptr ds:[ebp]
        mov    dword ptr [esi + 28h], ebx
        jmp    NEAR PTR loc_1_0134
loc_1_80DE:
        mov    edx, 40002h
        db  03Eh, 081h, 07Dh, 000h, 037h, 081h, 000h, 000h
        je     SHORT loc_1_8130
        test   byte ptr [edi + 1fh], 1
        jne    SHORT loc_1_80C8
        push   edi
        push   ecx
        push   edx
        push   20h
        call   sub_1_8CB6
        pop    edx
        pop    edx
        pop    ecx
        pop    edi
        mov    edx, 590002h
        mov    dword ptr [edi + 24h], eax
        test   eax, eax
        je     SHORT loc_1_8130
        push   eax
        push   20h
        mov    edx, esp
        push   edx
        push   eax
        db  0CDh, 020h, 002h, 000h, 093h, 004h
        add    esp, 8
        pop    ecx
        mov    edx, eax
        shl    edx, 10h
        mov    dx, 2
        or     eax, eax
        jne    SHORT loc_1_8130
        mov    byte ptr [edi + 4eh], cl
        jmp    SHORT loc_1_80D2
loc_1_8130:
        mov    eax, edx
        jmp    NEAR PTR loc_1_0033
loc_1_8137:
        mov    byte ptr [ebx], 1
        mov    word ptr [ebx + 3], 0
        add    ebx, 5
        ret
loc_1_8144:
        mov    byte ptr [ebx], 0dh
        mov    word ptr [ebx + 9], 1
        db  0CDh, 020h, 007h, 000h, 040h, 000h
        mov    dword ptr [ebx + 11h], eax
        db  0CDh, 020h, 060h, 000h, 040h, 000h
        mov    word ptr [ebx + 15h], ax
        xor    eax, eax
        mov    word ptr [ebx + 0bh], ax
        mov    word ptr [ebx + 3], ax
        mov    dword ptr [ebx + 17h], eax
        mov    word ptr [ebx + 1bh], ax
        movzx  edx, byte ptr [edi + 3fh]
        inc    eax
        db  0F6h, 082h, 0B8h, 01Ch, 001h, 000h, 002h
        jne    SHORT loc_1_8180
        inc    eax
loc_1_8180:
        mov    word ptr [ebx + 7], ax
        db  08Bh, 004h, 095h, 0B0h, 01Bh, 001h, 000h
        mov    ax, word ptr [eax + 0ah]
        mov    word ptr [ebx + 5], ax
        db  0F6h, 082h, 0B8h, 01Ch, 001h, 000h, 020h
        jne    SHORT loc_1_81AB
        db  0F6h, 082h, 0B8h, 01Ch, 001h, 000h, 001h
        je     SHORT loc_1_81AB
        mov    word ptr [ebx + 0bh], 3
loc_1_81AB:
        mov    ax, word ptr [edi + 1ch]
        shl    eax, 10h
        mov    edx, dword ptr [esi + 1ch]
        mov    ah, byte ptr [edx + 7]
        mov    al, byte ptr [edx + 0ah]
        mov    dword ptr [edi + 2ch], eax
        mov    dword ptr [ebx + 0dh], eax
        mov    word ptr [ebx + 3], 2
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        je     SHORT loc_1_81D5
        or     word ptr [ebx + 3], 1
loc_1_81D5:
        movzx  ecx, byte ptr [edi + 4eh]
        mov    edx, esi
        mov    word ptr [ebx + 1bh], cx
        mov    esi, dword ptr [edi + 24h]
        lea    edi, [ebx + 1dh]
        rep movsb
        mov    esi, edx
        mov    ebx, edi
        ret
loc_1_81EC:
        mov    byte ptr [ebx], 11h
        db  0CDh, 020h, 060h, 000h, 040h, 000h
        mov    word ptr [ebx + 20h], ax
        db  0CDh, 020h, 007h, 000h, 040h, 000h
        push   eax
        db  0CDh, 020h, 039h, 000h, 040h, 000h
        pop    ecx
        mov    dword ptr [ebx + 18h], eax
        mov    dword ptr [ebx + 1ch], edx
        sub    eax, eax
        mov    dword ptr [ebx + 0ch], eax
        mov    dword ptr [ebx + 14h], 200h
        inc    eax
        mov    word ptr [ebx + 4], ax
        mov    word ptr [ebx + 6], ax
        mov    word ptr [edi + 4ch], 0ffffh
        movzx  edx, byte ptr [edi + 3fh]
        db  08Bh, 004h, 095h, 0B0h, 01Bh, 001h, 000h
        movzx  eax, word ptr [eax + 0ah]
        mov    dword ptr [ebx + 8], eax
        db  0F6h, 082h, 0B8h, 01Ch, 001h, 000h, 020h
        jne    SHORT loc_1_825D
        db  0F6h, 082h, 0B8h, 01Ch, 001h, 000h, 001h
        je     SHORT loc_1_825D
        mov    word ptr [ebx + 4], 2
        mov    dword ptr [ebx + 0ch], 10000h
        or     dword ptr [ebx + 14h], 3
loc_1_825D:
        mov    ax, word ptr [edi + 1ch]
        shl    eax, 10h
        mov    edx, dword ptr [esi + 1ch]
        mov    ah, byte ptr [edx + 7]
        mov    al, byte ptr [edx + 0ah]
        mov    dword ptr [edi + 2ch], eax
        mov    dword ptr [ebx + 10h], eax
        mov    byte ptr [ebx + 3], 2
        db  0F6h, 005h, 0A2h, 023h, 001h, 000h, 001h
        je     SHORT loc_1_8284
        or     byte ptr [ebx + 3], 1
loc_1_8284:
        movzx  ecx, byte ptr [edi + 4eh]
        mov    edx, esi
        mov    word ptr [ebx + 23h], cx
        mov    byte ptr [ebx + 22h], cl
        mov    esi, dword ptr [edi + 24h]
        lea    edi, [ebx + 25h]
        rep movsb
        mov    esi, edx
        mov    ebx, edi
        ret
        db  000h, 000h
loc_1_82A0:
        db  0A1h, 0C4h, 031h, 001h, 000h
        db  02Bh, 005h, 00Ch, 01Ch, 001h, 000h
        db  0BAh, 028h, 027h, 001h, 000h
        mov    ebx, dword ptr [edx + 14h]
        cmp    ebx, edx
        je     SHORT loc_1_8308
        cmp    dword ptr [ebx + 44h], eax
        ja     SHORT loc_1_8301
        mov    ecx, dword ptr [ebx + 18h]
        call   sub_1_8394
        jb     SHORT loc_1_82A0
        mov    eax, dword ptr [ebx + 14h]
        mov    ecx, dword ptr [ebx + 18h]
        mov    dword ptr [eax + 18h], ecx
        mov    dword ptr [ecx + 14h], eax
        mov    al, byte ptr [ebx + 3ah]
        cmp    al, 29h
        je     SHORT loc_1_8327
        db  08Bh, 035h, 014h, 028h, 001h, 000h
        mov    dword ptr [esi + 1ch], ebx
        mov    ecx, dword ptr [ebx + 0ch]
        mov    dword ptr [esi + 20h], ecx
        mov    ecx, dword ptr [ebx + 24h]
        mov    dword ptr [esi + 24h], ecx
        mov    ecx, dword ptr [ebx + 28h]
        mov    dword ptr [esi + 28h], ecx
        mov    ecx, dword ptr [ebx + 48h]
        mov    dword ptr [esi + 30h], ecx
        mov    al, 20h
        jmp    NEAR PTR loc_1_002C
loc_1_8301:
        mov    ecx, dword ptr [ebx + 44h]
        sub    ecx, eax
        jmp    SHORT loc_1_830E
loc_1_8308:
        db  08Bh, 00Dh, 00Ch, 01Ch, 001h, 000h
loc_1_830E:
        db  0C6h, 005h, 054h, 027h, 001h, 000h, 000h
        db  0B8h, 044h, 027h, 001h, 000h
        mov    dword ptr [eax + 0ch], ecx
        call   sub_1_911C
        jmp    NEAR PTR loc_1_0154
loc_1_8327:
        mov    dword ptr [ebx + 18h], 0ffffffffh
        lea    eax, [ebx + 1ch]
        call   sub_1_8BC0
        jmp    NEAR PTR loc_1_82A0
        db  090h
loc_1_833C:
        push   ebx
        push   ecx
        push   edx
        db  08Bh, 00Dh, 0C4h, 031h, 001h, 000h
        db  02Bh, 00Dh, 00Ch, 01Ch, 001h, 000h
        db  0BAh, 028h, 027h, 001h, 000h
        mov    ebx, dword ptr [edx + 14h]
        cmp    ebx, edx
        je     SHORT loc_1_8363
        cmp    dword ptr [ebx + 44h], ecx
        jbe    SHORT loc_1_8375
        mov    edx, dword ptr [ebx + 44h]
        sub    edx, ecx
        jmp    SHORT loc_1_8369
loc_1_8363:
        db  08Bh, 015h, 00Ch, 01Ch, 001h, 000h
loc_1_8369:
        mov    dword ptr [eax + 0ch], edx
        call   sub_1_911C
        pop    ebx
        pop    ecx
        pop    edx
        ret
loc_1_8375:
        db  0C6h, 005h, 054h, 027h, 001h, 000h, 001h
        db  0B8h, 054h, 001h, 000h, 000h
        call   sub_1_89B4
        jb     SHORT loc_1_838F
        db  0F0h, 0FEh, 005h, 0CBh, 01Ch, 001h, 000h
loc_1_838F:
        pop    ebx
        pop    ecx
        pop    edx
        ret
        db  090h
sub_1_8394:
        db  08Bh, 00Ch, 08Dh, 01Ch, 03Ah, 001h, 000h
        jecxz  loc_1_83C4
        test   byte ptr [ecx + 18h], 2
        je     SHORT loc_1_83C4
        mov    esi, dword ptr [ecx + 0ch]
        db  0A1h, 028h, 028h, 001h, 000h
        sub    eax, dword ptr [esi + 34h]
        db  039h, 005h, 010h, 01Ch, 001h, 000h
        ja     SHORT loc_1_83BF
        mov    al, 2
        call   sub_1_1E8C
        stc
        ret
loc_1_83BF:
        and    byte ptr [ecx + 18h], 0f7h
        ret
loc_1_83C4:
        clc
        ret
        db  08Bh, 0C0h
loc_1_83C8:
        mov    ecx, ebx
        db  0A1h, 014h, 028h, 001h, 000h
        mov    esi, dword ptr [eax + 1ch]
        shr    ebx, 10h
        mov    dword ptr [esi + 18h], ebx
        mov    byte ptr [esi + 5], 1
        mov    edx, dword ptr [eax + 24h]
        mov    dword ptr [esi + 24h], edx
        mov    edx, dword ptr [eax + 28h]
        mov    dword ptr [esi + 28h], edx
        db  0BAh, 028h, 027h, 001h, 000h
        mov    ebx, dword ptr [edx + 18h]
        mov    dword ptr [ebx + 14h], esi
        mov    dword ptr [esi + 14h], edx
        mov    dword ptr [esi + 18h], ebx
        mov    dword ptr [edx + 18h], esi
        call   sub_1_841C
        jbe    SHORT loc_1_8408
        jmp    NEAR PTR loc_1_0154
loc_1_8408:
        jb     SHORT loc_1_8411
        mov    ecx, esi
        jmp    NEAR PTR loc_1_0285
loc_1_8411:
        mov    ebx, dword ptr [esi + 0ch]
        mov    al, 20h
        jmp    NEAR PTR loc_1_002C
        db  090h
sub_1_841C:
        mov    ebx, ecx
        shr    ecx, 10h
        db  038h, 099h, 01Ch, 04Ah, 001h, 000h
        jne    SHORT loc_1_8477
        db  08Bh, 00Ch, 08Dh, 01Ch, 03Ah, 001h, 000h
        jecxz  loc_1_8477
        test   byte ptr [ecx + 18h], 8
        jne    SHORT loc_1_846A
        test   byte ptr [ecx + 18h], 2
        je     SHORT loc_1_846B
        push   ecx
        mov    edx, dword ptr [ecx + 0ch]
        movzx  edx, byte ptr [edx + 3fh]
        mov    al, 83h
        call   sub_1_7738
        jae    SHORT loc_1_848A
        mov    al, 80h
        call   sub_1_7738
        jae    SHORT loc_1_848A
        pop    ecx
        test   esi, esi
        je     SHORT loc_1_8469
        mov    eax, dword ptr [esi + 14h]
        mov    ebx, dword ptr [esi + 18h]
        mov    dword ptr [eax + 18h], ebx
        mov    dword ptr [ebx + 14h], eax
loc_1_8469:
        stc
loc_1_846A:
        ret
loc_1_846B:
        or     byte ptr [ecx + 18h], 8
        mov    edi, dword ptr [ecx + 0ch]
        inc    word ptr [edi + 46h]
        ret
loc_1_8477:
        test   esi, esi
        je     SHORT loc_1_846A
        mov    eax, dword ptr [esi + 14h]
        mov    ebx, dword ptr [esi + 18h]
        mov    dword ptr [eax + 18h], ebx
        mov    dword ptr [ebx + 14h], eax
        xor    eax, eax
        ret
loc_1_848A:
        pop    edx
        or     byte ptr [edx + 18h], 8
        mov    esi, ecx
        mov    ebx, dword ptr [esi + 0ch]
        mov    edi, ebx
        mov    ecx, 0dh
        sub    eax, eax
        rep stosd
        mov    dword ptr [ebx], 424d53ffh
        mov    byte ptr [ebx + 4], 24h
        dec    eax
        mov    word ptr [ebx + 1eh], ax
        mov    word ptr [ebx + 1ah], ax
        mov    cx, word ptr [edx + 14h]
        mov    word ptr [ebx + 18h], cx
        add    ebx, 20h
        mov    byte ptr [ebx], 8
        mov    byte ptr [ebx + 1], al
        mov    word ptr [ebx + 7], 2
        mov    eax, dword ptr [edx + 8]
        mov    word ptr [ebx + 5], ax
        mov    word ptr [esi + 10h], 33h
        mov    al, byte ptr [edx + 34h]
        mov    byte ptr [esi + 0ah], al
        mov    edi, dword ptr [edx + 0ch]
        inc    word ptr [edi + 46h]
        mov    dword ptr [esi + 4ch], 0
        mov    dword ptr [esi + 48h], edi
        mov    al, byte ptr [edi + 3ch]
        mov    byte ptr [esi + 3ch], al
        mov    byte ptr [esi + 5], 7eh
        mov    byte ptr [esi + 8], 94h
        db  0A1h, 014h, 028h, 001h, 000h
        mov    dword ptr [eax + 30h], edi
        lea    eax, [edi - 24h]
        cli
loc_1_8507:
        mov    ecx, dword ptr [eax + 4ch]
        jecxz  loc_1_8510
        mov    eax, ecx
        jmp    SHORT loc_1_8507
loc_1_8510:
        mov    dword ptr [eax + 4ch], esi
        sti
        cmp    byte ptr [edi + 41h], 0
        je     SHORT sub_1_851D
        test   esp, esp
        ret
sub_1_851D:
        cli
        mov    ecx, dword ptr [edi + 28h]
        jecxz  loc_1_853C
        mov    ebx, dword ptr [ecx + 4ch]
        mov    dword ptr [edi + 28h], ebx
        sti
        push   edi
        call   sub_1_759C
        pop    edi
        test   al, al
        je     SHORT sub_1_851D
        call   sub_1_781C
        jmp    SHORT sub_1_851D
loc_1_853C:
        sti
        test   esp, esp
        ret
sub_1_8540:
        pushad
        mov    eax, dword ptr [ecx + 22h]
        call   sub_1_9978
        mov    ecx, dword ptr [esp + 18h]
        jmp    SHORT loc_1_8551
        db  090h
sub_1_8550:
        pushad
loc_1_8551:
        and    byte ptr [ecx + 18h], 0f7h
        mov    edi, dword ptr [ecx + 0ch]
        dec    word ptr [edi + 46h]
        mov    ebx, dword ptr [ecx + 8]
        mov    esi, dword ptr [ecx + 8]
        db  0BAh, 028h, 027h, 001h, 000h
        mov    ecx, edx
        mov    eax, eax
loc_1_856C:
        mov    ecx, dword ptr [ecx + 14h]
loc_1_856F:
        cmp    ecx, edx
        je     SHORT loc_1_85AE
        cmp    dword ptr [ecx + 18h], esi
        jne    SHORT loc_1_856C
        mov    edi, dword ptr [ecx + 14h]
        mov    ebx, dword ptr [ecx + 18h]
        mov    dword ptr [edi + 18h], ebx
        mov    dword ptr [ebx + 14h], edi
        cmp    byte ptr [ecx + 3ah], 29h
        je     SHORT loc_1_85B0
        db  0B8h, 085h, 002h, 000h, 000h
        call   sub_1_89B4
        jb     SHORT loc_1_85A1
        db  0F0h, 0FEh, 005h, 0CBh, 01Ch, 001h, 000h
        mov    ecx, edi
        jmp    SHORT loc_1_856F
loc_1_85A1:
        mov    ebx, ecx
        xchg   edx, eax
        call   sub_1_0338
        xchg   edx, eax
        mov    ecx, edi
        jmp    SHORT loc_1_856F
loc_1_85AE:
        popad
        ret
loc_1_85B0:
        mov    dword ptr [ecx + 18h], 0fffffffeh
        lea    eax, [ecx + 1ch]
        call   sub_1_8BC0
        mov    ecx, edi
        jmp    SHORT loc_1_856F
        db  000h, 056h, 057h, 053h, 08Bh, 0FEh, 0E8h, 035h, 0F8h, 0FFh, 0FFh, 08Bh, 01Ch, 024h, 08Dh, 07Bh
        db  01Eh, 083h, 0F9h, 00Eh, 076h, 005h, 0B9h, 00Eh, 000h, 000h, 000h, 0F3h, 0A6h, 05Bh, 05Fh, 05Eh
        db  0C3h, 057h, 00Fh, 0B6h, 04Fh, 00Ah, 08Dh, 041h, 046h, 056h, 050h, 057h, 051h, 052h, 050h, 0E8h
        db  0BFh, 006h, 000h, 000h, 05Ah, 05Ah, 059h, 05Bh, 059h, 085h, 0C0h, 074h, 046h, 08Bh, 0F3h, 08Bh
        db  0F8h, 051h, 0C1h, 0E9h, 002h, 0F3h, 0A5h, 059h, 083h, 0E1h, 003h, 0F3h, 0A4h, 08Dh, 078h, 01Eh
        db  0B9h, 007h, 000h, 000h, 000h, 050h, 02Bh, 0C0h, 0F3h, 066h, 0ABh, 058h, 05Eh, 08Bh, 0FEh, 050h
        db  0E8h, 0DBh, 0F7h, 0FFh, 0FFh, 058h, 083h, 0F9h, 00Eh, 076h, 005h, 0B9h, 00Eh, 000h, 000h, 000h
        db  08Dh, 078h, 01Eh, 0E8h, 0D8h, 0F7h, 0FFh, 0FFh, 08Dh, 078h, 045h, 089h, 078h, 02Ch, 08Bh, 0D8h
        db  05Fh, 0F8h, 0C3h, 05Eh, 05Fh, 02Bh, 0DBh, 0F9h, 0C3h, 050h, 051h, 052h, 083h, 0EEh, 001h, 01Bh
        db  0C0h, 0F7h, 0D0h, 023h, 0D0h, 08Dh, 074h, 016h, 001h, 085h, 0DBh, 074h, 04Ah, 0FFh, 00Dh, 0F1h
        db  024h, 001h, 000h, 08Bh, 00Bh, 08Bh, 053h, 004h, 089h, 051h, 004h, 089h, 00Ah, 02Bh, 0C9h, 085h
        db  0F6h, 074h, 012h, 0E8h, 049h, 0FFh, 0FFh, 0FFh, 074h, 038h, 053h, 0E8h, 061h, 0FFh, 0FFh, 0FFh
        db  072h, 035h, 059h, 087h, 0D9h, 0C7h, 043h, 034h, 000h, 000h, 000h, 000h, 0A1h, 008h, 01Ch, 001h
        db  000h, 089h, 003h, 089h, 01Dh, 008h, 01Ch, 001h, 000h, 0B8h, 030h, 01Ch, 001h, 000h, 0E8h, 01Ah
        db  005h, 000h, 000h, 08Bh, 0D9h, 0EBh, 00Bh, 085h, 0F6h, 074h, 007h, 0E8h, 031h, 0FFh, 0FFh, 0FFh
        db  0EBh, 001h, 0F8h, 05Ah, 059h, 058h, 0C3h, 05Bh, 0EBh, 0F9h, 000h, 000h, 000h
sub_1_86C0:
        push   ecx
        push   edx
        push   104h
        call   sub_1_8CB6
        pop    edx
        pop    edx
        pop    ecx
        test   eax, eax
        je     SHORT loc_1_871F
        movzx  bx, byte ptr [esi]
        or     bl, 20h
        sub    bl, 61h
        mov    word ptr [eax], bx
        push   eax
        add    esi, 3
        lea    edi, [eax + 5]
        lea    ecx, [eax + 4]
        mov    edx, esi
        db  08Bh, 01Dh, 054h, 037h, 001h, 000h
        jmp    SHORT loc_1_86F5
loc_1_86F4:
        stosb
loc_1_86F5:
        lodsb
        xlatb
        cmp    al, 5ch
        je     SHORT loc_1_870C
        cmp    al, 2
        ja     SHORT loc_1_86F4
        jne    SHORT loc_1_8728
        mov    ax, word ptr [esi - 1]
        stosw
        inc    esi
        jmp    SHORT loc_1_86F5
        db  08Bh, 0C0h
loc_1_870C:
        xchg   edi, ecx
        sub    ecx, edi
        test   ch, ch
        jne    SHORT loc_1_8754
        mov    byte ptr [edi], cl
        add    ecx, edi
        lea    edi, [ecx + 1]
        mov    edx, esi
        jmp    SHORT loc_1_86F5
loc_1_871F:
        mov    eax, 8
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_8728:
        mov    eax, edi
        sub    eax, ecx
        test   ah, ah
        jne    SHORT loc_1_8754
        cmp    al, 1
        je     SHORT loc_1_8741
        mov    byte ptr [ecx], al
        pop    eax
        sub    ecx, eax
        sub    ecx, 2
        mov    word ptr [eax + 2], cx
        ret
loc_1_8741:
        mov    word ptr [ecx], ax
        pop    eax
        sub    ecx, eax
        cmp    ecx, 2
        jne    SHORT loc_1_8750
        mov    word ptr [eax], cx
        ret
loc_1_8750:
        mov    al, 3
        stc
        ret
loc_1_8754:
        pop    eax
        mov    al, 0ceh
        stc
        ret
        db  000h, 000h, 000h
sub_1_875C:
        pushad
        db  08Bh, 01Dh, 014h, 028h, 001h, 000h
        cli
        inc    dword ptr [eax]
loc_1_8766:
        mov    dword ptr [ebx + 10h], esp
        push   15h
        push   eax
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebx + 10h], 0
        popad
        ret
        db  08Bh, 0C0h
loc_1_8780:
        cli
        db  08Bh, 01Dh, 0F4h, 027h, 001h, 000h
        db  03Bh, 01Dh, 0F8h, 027h, 001h, 000h
        ja     SHORT loc_1_87BC
        db  08Bh, 01Dh, 014h, 028h, 001h, 000h
        db  08Bh, 00Dh, 028h, 028h, 001h, 000h
        mov    dword ptr [ebx + 18h], ecx
        db  08Bh, 015h, 000h, 028h, 001h, 000h
        mov    dword ptr [ebx], edx
        db  089h, 01Dh, 000h, 028h, 001h, 000h
        mov    esp, dword ptr [ebx + 14h]
        mov    eax, ebx
        db  068h, 080h, 087h, 000h, 000h
        pushad
        jmp    SHORT loc_1_8766
        db  02Eh, 08Bh, 0C0h
loc_1_87BC:
        db  08Bh, 01Dh, 014h, 028h, 001h, 000h
        db  0B8h, 000h, 028h, 001h, 000h
loc_1_87C7:
        cmp    dword ptr [eax + 4], ebx
        je     SHORT loc_1_87D8
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 0fffff000h
        jne    SHORT loc_1_87C7
        jmp    SHORT loc_1_87DE
loc_1_87D8:
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [eax + 4], ecx
loc_1_87DE:
        db  0FFh, 00Dh, 0F4h, 027h, 001h, 000h
        jne    SHORT loc_1_8800
        push   eax
        push   edx
        db  0A1h, 020h, 01Ch, 001h, 000h
        cmp    byte ptr [eax], 2
        jne    SHORT loc_1_87FE
        db  068h, 004h, 028h, 001h, 000h
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        pop    edx
loc_1_87FE:
        pop    edx
        pop    eax
loc_1_8800:
        mov    esp, dword ptr [ebx + 14h]
        push   ecx
        push   edx
        push   ebx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
        jmp    NEAR PTR loc_1_8B6B
        db  090h
sub_1_8814:
        db  0A1h, 0F4h, 027h, 001h, 000h
        db  03Bh, 005h, 0FCh, 027h, 001h, 000h
        ja     SHORT loc_1_8822
        ret
loc_1_8822:
        db  081h, 03Dh, 000h, 028h, 001h, 000h, 000h, 0F0h, 0FFh, 0FFh
        jne    SHORT loc_1_882F
        ret
loc_1_882F:
        push   ebx
        push   ecx
        push   edx
        db  0B9h, 000h, 028h, 001h, 000h
        mov    ebx, dword ptr [ecx]
loc_1_8839:
        sub    dword ptr [ebx + 18h], eax
        db  08Bh, 015h, 028h, 028h, 001h, 000h
        sub    edx, dword ptr [ebx + 18h]
        db  03Bh, 015h, 008h, 028h, 001h, 000h
        jae    SHORT loc_1_885D
        mov    ecx, ebx
        mov    ebx, dword ptr [ebx]
        cmp    ebx, 0fffff000h
        jne    SHORT loc_1_8839
loc_1_8859:
        pop    edx
        pop    ecx
        pop    ebx
        ret
loc_1_885D:
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx + 14h]
        sub    eax, 24h
        db  0C7h, 040h, 020h, 0BCh, 087h, 000h, 000h
        push   ebx
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        pop    ebx
        jmp    SHORT loc_1_8859
sub_1_8878:
        pushad
        db  0C7h, 005h, 0F8h, 027h, 001h, 000h, 000h, 000h, 000h, 000h
        call   sub_1_E70C
        mov    ebx, 0fffff000h
        db  087h, 01Dh, 000h, 028h, 001h, 000h
        cmp    ebx, 0fffff000h
        je     SHORT loc_1_88BC
loc_1_889B:
        sub    ecx, ecx
        push   dword ptr [ebx]
        xchg   dword ptr [ebx + 10h], ecx
        jecxz  loc_1_88B3
        db  0C7h, 041h, 020h, 0BCh, 087h, 000h, 000h
        push   ebx
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        pop    ebx
loc_1_88B3:
        pop    ebx
        cmp    ebx, 0fffff000h
        jne    SHORT loc_1_889B
loc_1_88BC:
        cli
        db  081h, 03Dh, 004h, 028h, 001h, 000h, 000h, 0F0h, 0FFh, 0FFh
        je     SHORT loc_1_88DB
        push   5
        db  068h, 004h, 028h, 001h, 000h
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        add    esp, 8
        jmp    SHORT loc_1_88BC
loc_1_88DB:
        sti
        popad
        ret
        db  08Bh, 0C0h
sub_1_88E0:
        push   eax
        mov    edx, eax
        shl    dword ptr [edx], 3
        je     SHORT loc_1_88FE
        mov    eax, 7dh
        db  0BEh, 030h, 089h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        test   esi, esi
        jne    SHORT loc_1_88FE
        mov    dword ptr [edx], esi
loc_1_88FE:
        pop    eax
        ret
sub_1_8900:
        cmp    dword ptr [eax], 0
        jle    SHORT loc_1_8926
        mov    dword ptr [eax], 1
        nop
sub_1_890C:
        cmp    dword ptr [eax], 0
        jle    SHORT loc_1_8926
        push   eax
        push   ecx
        push   edx
        push   5
        push   eax
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        add    esp, 8
        pop    edx
        pop    ecx
        pop    eax
        cmp    dword ptr [eax], 0
loc_1_8926:
        ret
loc_1_8927:
        mov    dword ptr [edx], 1
        mov    eax, eax
loc_1_8930:
        cmp    dword ptr [edx], 0
        jle    SHORT loc_1_8937
        dec    dword ptr [edx]
loc_1_8937:
        push   edx
        push   ecx
        push   edx
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        pop    edx
        pop    ecx
        pop    edx
        cmp    dword ptr [edx], 0
        jle    SHORT loc_1_895C
        mov    eax, 7dh
        db  0BEh, 030h, 089h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        test   esi, esi
        je     SHORT loc_1_8927
loc_1_895C:
        ret
loc_1_895D:
        db  00Fh, 0BAh, 035h, 010h, 028h, 001h, 000h, 002h
        mov    ecx, edx
        db  0B8h, 054h, 001h, 000h, 000h
        call   sub_1_89B4
        ret
loc_1_8972:
        pushad
        db  00Fh, 0BAh, 02Dh, 010h, 028h, 001h, 000h, 002h
        jb     SHORT loc_1_8995
        xor    eax, eax
        db  08Bh, 01Dh, 014h, 031h, 001h, 000h
        mov    ecx, 2
        db  0BEh, 05Dh, 089h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
loc_1_8995:
        popad
        ret
        db  090h
sub_1_8998:
        db  0BBh, 054h, 001h, 000h, 000h
        pushfd
        cli
        push   eax
        push   ecx
        db  0A1h, 000h, 028h, 001h, 000h
        cmp    eax, 0fffff000h
        jne    SHORT loc_1_89C9
        pop    ecx
        pop    eax
        popfd
        jmp    SHORT loc_1_8972
        db  08Bh, 0C0h
sub_1_89B4:
        db  0BBh, 054h, 001h, 000h, 000h
loc_1_89B9:
        pushfd
        cli
        push   eax
        push   ecx
        db  0A1h, 000h, 028h, 001h, 000h
        cmp    eax, 0fffff000h
        je     SHORT loc_1_89F5
loc_1_89C9:
        mov    ebx, dword ptr [eax]
        db  089h, 01Dh, 000h, 028h, 001h, 000h
        mov    ebx, dword ptr [eax + 14h]
        sub    ebx, 24h
        pop    dword ptr [ebx + 18h]
        pop    dword ptr [ebx + 20h]
        popfd
        mov    dword ptr [ebx + 4], esi
        mov    dword ptr [ebx], edi
        push   ecx
        push   edx
        push   eax
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        pop    eax
        pop    edx
        pop    ecx
        clc
        ret
sub_1_89F1:
        mov    ebx, eax
        jmp    SHORT loc_1_89B9
loc_1_89F5:
        mov    dword ptr [esp + 4], ebx
        db  0A1h, 0F4h, 027h, 001h, 000h
        db  03Bh, 005h, 0F8h, 027h, 001h, 000h
        jae    NEAR PTR loc_1_8AD1
        db  0FFh, 005h, 0F4h, 027h, 001h, 000h
        push   dword ptr [esp + 8]
        popfd
        db  080h, 03Dh, 0CAh, 01Ch, 001h, 000h, 002h
        jne    SHORT loc_1_8A23
        call   sub_1_8C93
loc_1_8A23:
        push   edi
        push   ecx
        push   edx
        push   4cah
        call   sub_1_8CB6
        pop    edx
        pop    edx
        pop    ecx
        pop    edi
        test   eax, eax
        je     NEAR PTR loc_1_8AD6
        mov    dword ptr [eax + 10h], 0
        mov    ecx, dword ptr [esp]
        mov    dword ptr [eax + 18h], ecx
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [eax], ecx
        push   eax
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  03Bh, 01Dh, 014h, 031h, 001h, 000h
        pop    ebx
        jne    SHORT loc_1_8A85
loc_1_8A5F:
        push   esi
        push   edx
        push   ebx
        mov    ecx, 1000h
        mov    edx, ebx
        db  0BBh, 007h, 08Bh, 000h, 000h
        db  0BEh, 0FEh, 08Ah, 000h, 000h
        db  0CDh, 020h, 013h, 000h, 02Ah, 000h
        pop    ebx
        pop    edx
        pop    esi
        or     eax, eax
        je     SHORT loc_1_8ABD
        pop    ecx
        pop    eax
        popfd
        stc
        ret
loc_1_8A85:
        db  00Fh, 0BAh, 02Dh, 010h, 028h, 001h, 000h, 001h
        jb     SHORT loc_1_8ABD
        push   edx
        push   ebx
        push   esi
        mov    edx, ebx
        mov    eax, 1000h
        db  08Bh, 01Dh, 014h, 031h, 001h, 000h
        mov    ecx, 8
        db  0BEh, 0DEh, 08Ah, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        test   esi, esi
        pop    esi
        pop    ebx
        pop    edx
        jne    SHORT loc_1_8AD1
        db  083h, 025h, 010h, 028h, 001h, 000h, 0FEh
loc_1_8ABD:
        pushfd
        cli
        db  0FFh, 00Dh, 0F4h, 027h, 001h, 000h
        popfd
        push   ecx
        push   edx
        push   ebx
        call   sub_1_8CF5
        pop    edx
        pop    edx
        pop    ecx
loc_1_8AD1:
        pop    ecx
        pop    eax
        popfd
        stc
        ret
loc_1_8AD6:
        db  0FFh, 00Dh, 0F4h, 027h, 001h, 000h
        jmp    SHORT loc_1_8AD1
loc_1_8ADE:
        db  00Fh, 0BAh, 035h, 010h, 028h, 001h, 000h, 001h
        pushfd
        push   eax
        push   ecx
        mov    ebx, edx
        db  08Bh, 00Dh, 0F4h, 027h, 001h, 000h
        db  03Bh, 00Dh, 0F8h, 027h, 001h, 000h
        jae    SHORT loc_1_8ABD
        jmp    NEAR PTR loc_1_8A5F
loc_1_8AFE:
        pushfd
        push   eax
        push   ecx
        mov    ebx, dword ptr [esp + 10h]
        jmp    SHORT loc_1_8ABD
loc_1_8B07:
        push   ebx
        mov    ebx, dword ptr [esp + 8]
        push   esi
        push   edi
        push   ebp
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  068h, 080h, 087h, 000h, 000h
        db  0A1h, 004h, 028h, 001h, 000h
        mov    dword ptr [ebx + 4], eax
        db  089h, 01Dh, 004h, 028h, 001h, 000h
        mov    dword ptr [ebx + 14h], esp
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    dword ptr [ebx + 0ch], edi
        db  0A1h, 0EEh, 08Bh, 000h, 000h
        mov    dword ptr [eax + edi], ebx
        db  089h, 01Dh, 014h, 028h, 001h, 000h
        db  0CDh, 020h, 06Ch, 001h, 001h, 000h
        cli
        db  0A1h, 01Dh, 023h, 001h, 000h
        cmp    eax, 0fffff000h
        je     SHORT loc_1_8B64
        db  0CDh, 020h, 06Bh, 001h, 001h, 000h
loc_1_8B64:
        sti
        mov    ecx, dword ptr [ebx + 18h]
        push   dword ptr [ebx]
        ret
loc_1_8B6B:
        pop    ebx
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_8B80:
        push   edi
        push   ecx
        db  08Bh, 00Ch, 085h, 070h, 027h, 001h, 000h
        db  089h, 00Dh, 0FCh, 027h, 001h, 000h
        db  08Bh, 00Ch, 085h, 09Ch, 027h, 001h, 000h
        db  089h, 00Dh, 0F8h, 027h, 001h, 000h
        db  08Bh, 004h, 085h, 0C8h, 027h, 001h, 000h
        db  02Bh, 005h, 00Ch, 028h, 001h, 000h
        je     SHORT loc_1_8BB9
        db  08Bh, 03Dh, 01Dh, 023h, 001h, 000h
        cmp    edi, 0fffff000h
        je     SHORT loc_1_8BB9
loc_1_8BB9:
        pop    ecx
        pop    edi
        ret
sub_1_8BBC:
        mov    eax, dword ptr [esp + 4]
sub_1_8BC0:
        push   ecx
        pushfd
        cli
        xor    ecx, ecx
        xchg   dword ptr [eax], ecx
        jecxz  loc_1_8BD1
        push   eax
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        pop    ecx
loc_1_8BD1:
        popfd
        pop    ecx
        ret
        db  060h, 09Ch, 0A1h, 014h, 028h, 001h, 000h, 0CDh, 020h, 038h, 000h, 001h, 000h, 09Dh, 061h, 0C3h
loc_1_8BE4:
        mov    eax, dword ptr [edi + 10h]
        db  0A3h, 018h, 028h, 001h, 000h
        mov    eax, dword ptr [edi - 1000h]
        db  0A3h, 014h, 028h, 001h, 000h
        ret
loc_1_8BF8:
        push   edx
        call   sub_1_BB30
        pop    edx
        ret
loc_1_8C00:
        push   ebx
        push   eax
        push   esi
        push   ecx
        sub    ebx, ebx
        mov    dword ptr [edx + 54h], ebx
        mov    eax, 1000h
        db  08Bh, 01Dh, 014h, 031h, 001h, 000h
        mov    ecx, 0bh
        db  0BEh, 0F8h, 08Bh, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        pop    ecx
        pop    esi
        pop    eax
        pop    ebx
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_8C2C:
        pushfd
        mov    esi, dword ptr [ecx + 54h]
        test   esi, esi
        je     SHORT loc_1_8C41
        mov    dword ptr [ecx + 54h], 0
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
loc_1_8C41:
        popfd
        ret
        db  090h
sub_1_8C44:
        push   edx
        push   eax
        push   ecx
        push   esi
        cmp    byte ptr [ecx + 7], 10h
        jne    SHORT loc_1_8C63
        mov    edx, ecx
        mov    eax, 0ea60h
        db  0BEh, 000h, 08Ch, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    dword ptr [ecx + 54h], esi
loc_1_8C63:
        pop    esi
        pop    ecx
        pop    eax
        pop    edx
        ret
sub_1_8C68:
        db  0CDh, 020h, 005h, 080h, 080h, 004h
sub_1_8C6E:
        db  0CDh, 020h, 006h, 080h, 080h, 004h
sub_1_8C74:
        db  0CDh, 020h, 071h, 080h, 040h, 000h
sub_1_8C7A:
        db  0CDh, 020h, 072h, 080h, 040h, 000h
loc_1_8C80:
        db  0C6h, 005h, 020h, 028h, 001h, 000h, 000h
        db  0CDh, 020h, 011h, 080h, 040h, 000h
sub_1_8C8D:
        db  0CDh, 020h, 011h, 080h, 040h, 000h
sub_1_8C93:
        pushad
        xor    eax, eax
        inc    al
        db  086h, 005h, 020h, 028h, 001h, 000h
        test   al, al
        jne    SHORT loc_1_8CB4
        xor    ebx, ebx
        mov    ecx, 2
        db  0BEh, 080h, 08Ch, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
loc_1_8CB4:
        popad
        ret
sub_1_8CB6:
        mov    eax, dword ptr [esp + 4]
        add    eax, 4
        push   eax
        db  080h, 03Dh, 0CAh, 01Ch, 001h, 000h, 002h
        je     SHORT loc_1_8CD5
        push   1
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        jmp    SHORT loc_1_8CE4
loc_1_8CD5:
        call   sub_1_8C93
        push   eax
        db  0CDh, 020h, 00Dh, 000h, 040h, 000h
        add    esp, 4
loc_1_8CE4:
        pop    edx
        test   eax, eax
        je     SHORT loc_1_8CF4
        db  001h, 015h, 01Ch, 028h, 001h, 000h
        mov    dword ptr [eax], edx
        add    eax, 4
loc_1_8CF4:
        ret
sub_1_8CF5:
        mov    eax, dword ptr [esp + 4]
        test   eax, eax
        je     SHORT loc_1_8D29
        sub    eax, 4
        mov    edx, dword ptr [eax]
        db  029h, 015h, 01Ch, 028h, 001h, 000h
        db  080h, 03Dh, 0CAh, 01Ch, 001h, 000h, 002h
        je     SHORT loc_1_8D1F
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        jmp    SHORT loc_1_8D29
loc_1_8D1F:
        push   eax
        db  0CDh, 020h, 00Eh, 000h, 040h, 000h
        add    esp, 4
loc_1_8D29:
        ret
sub_1_8D2A:
        push   ebp
        mov    ebp, esp
        push   81h
        push   0
        push   0
        push   0
        push   0
        push   0
        push   1
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        test   eax, eax
        je     SHORT loc_1_8D5C
        push   eax
        mov    eax, dword ptr [ebp + 8]
        shl    eax, 0ch
        db  001h, 005h, 01Ch, 028h, 001h, 000h
        pop    eax
loc_1_8D5C:
        leave
        ret
loc_1_8D5E:
        db  080h, 025h, 025h, 028h, 001h, 000h, 0FEh
loc_1_8D65:
        db  08Bh, 015h, 021h, 028h, 001h, 000h
        test   edx, edx
        je     SHORT loc_1_8DAC
        mov    eax, dword ptr [edx + 4]
        db  0A3h, 021h, 028h, 001h, 000h
        mov    eax, edx
        mov    edx, dword ptr [eax]
        push   eax
        push   edx
        push   0
        push   edx
        db  0CDh, 020h, 058h, 000h, 001h, 000h
        add    esp, 8
        shl    eax, 0ch
        db  029h, 005h, 01Ch, 028h, 001h, 000h
        pop    edx
        push   0
        push   edx
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
        pop    eax
        push   eax
        db  0CDh, 020h, 00Eh, 000h, 040h, 000h
        add    esp, 4
        jmp    SHORT loc_1_8D65
loc_1_8DAC:
        ret
loc_1_8DAD:
        push   edx
        push   0
        push   edx
        db  0CDh, 020h, 058h, 000h, 001h, 000h
        add    esp, 8
        shl    eax, 0ch
        db  029h, 005h, 01Ch, 028h, 001h, 000h
        pop    edx
        push   0
        push   edx
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
        ret
sub_1_8DD1:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    eax, 8
        push   eax
        db  0CDh, 020h, 00Dh, 000h, 040h, 000h
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_8E27
        mov    edx, dword ptr [ebp + 8]
        db  08Bh, 035h, 021h, 028h, 001h, 000h
        mov    dword ptr [eax], edx
        mov    dword ptr [eax + 4], esi
        db  0A3h, 021h, 028h, 001h, 000h
        db  0F6h, 005h, 025h, 028h, 001h, 000h, 001h
        jne    SHORT loc_1_8E3E
        db  080h, 00Dh, 025h, 028h, 001h, 000h, 001h
        xor    eax, eax
        xor    ebx, ebx
        mov    ecx, 2
        db  0BEh, 05Eh, 08Dh, 000h, 000h
        mov    edx, 0
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        jmp    SHORT loc_1_8E3E
loc_1_8E27:
        xor    eax, eax
        xor    ebx, ebx
        mov    ecx, 2
        db  0BEh, 0ADh, 08Dh, 000h, 000h
        mov    edx, dword ptr [ebp + 8]
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
loc_1_8E3E:
        pop    esi
        pop    ebx
        leave
        ret
sub_1_8E42:
        push   ebp
        mov    ebp, esp
        push   0
        push   0
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 057h, 000h, 001h, 000h
        add    esp, 10h
        leave
        ret
        db  000h, 000h
sub_1_8E5C:
        db  068h, 0BCh, 030h, 001h, 000h
        push   30ah
        push   30ah
        db  0CDh, 020h, 003h, 000h, 080h, 004h
        add    esp, 0ch
        ret
sub_1_8E75:
        push   ebx
        db  068h, 008h, 031h, 001h, 000h
        db  068h, 0E8h, 030h, 001h, 000h
        push   80000002h
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_8EEC
        sub    esp, 0ch
        mov    eax, esp
        mov    edi, eax
        push   eax
        mov    dword ptr [eax], 4
        add    eax, 4
        push   eax
        add    eax, 4
        push   eax
        sub    eax, eax
        push   eax
        db  068h, 0FAh, 030h, 001h, 000h
        db  0FFh, 035h, 008h, 031h, 001h, 000h
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        or     eax, eax
        jne    SHORT loc_1_8EDD
        cmp    dword ptr [edi + 4], 0
        je     SHORT loc_1_8EDD
        db  0CDh, 020h, 000h, 000h, 091h, 004h
        jb     SHORT loc_1_8EEF
        db  0CDh, 020h, 000h, 000h, 093h, 004h
        jb     SHORT loc_1_8F0A
        db  080h, 00Dh, 0A2h, 023h, 001h, 000h, 001h
loc_1_8EDD:
        db  0FFh, 035h, 008h, 031h, 001h, 000h
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 28h
loc_1_8EEC:
        clc
        pop    ebx
        ret
loc_1_8EEF:
        db  0C7h, 005h, 0BCh, 030h, 001h, 000h, 06Fh, 018h, 000h, 000h
        db  066h, 083h, 005h, 0C4h, 030h, 001h, 000h, 000h
        db  0C6h, 005h, 0C6h, 030h, 001h, 000h, 000h
        jmp    SHORT loc_1_8F23
loc_1_8F0A:
        db  0C7h, 005h, 0BCh, 030h, 001h, 000h, 070h, 018h, 000h, 000h
        db  066h, 083h, 005h, 0C4h, 030h, 001h, 000h, 000h
        db  0C6h, 005h, 0C6h, 030h, 001h, 000h, 000h
loc_1_8F23:
        db  0FFh, 035h, 008h, 031h, 001h, 000h
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 28h
        stc
        pop    ebx
        ret
sub_1_8F35:
        push   eax
        push   ebx
        push   ecx
        push   edx
        push   esi
        db  0BEh, 0D7h, 08Fh, 000h, 000h
        db  0CDh, 020h, 03Ah, 000h, 001h, 000h
        jb     NEAR PTR loc_1_8FD5
        db  0A1h, 0B4h, 031h, 001h, 000h
        db  0A3h, 0B8h, 031h, 001h, 000h
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        db  0A3h, 0BCh, 031h, 001h, 000h
        mov    ecx, 3e8h
        mov    edx, ecx
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
loc_1_8F76:
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    word ptr [ebp + 10h], 0
        mov    word ptr [ebp + 1ch], 1689h
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        loop   loc_1_8F76
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        db  02Bh, 005h, 0BCh, 031h, 001h, 000h
        db  0A3h, 0BCh, 031h, 001h, 000h
        db  0A1h, 0B4h, 031h, 001h, 000h
        db  0A3h, 0B8h, 031h, 001h, 000h
        db  0C7h, 005h, 0B4h, 031h, 001h, 000h, 000h, 000h, 000h, 000h
loc_1_8FCF:
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        ret
loc_1_8FD5:
        jmp    SHORT loc_1_8FCF
loc_1_8FD7:
        db  0FFh, 005h, 0B4h, 031h, 001h, 000h
        stc
        ret
VSERVER_Service_00:
        mov    eax, 30ah
        xor    ebx, ebx
        clc
        ret
VSERVER_PM_API:
        push   ebp
        push   ebx
        xchg   esi, eax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        movzx  eax, word ptr [ebp + 1ch]
        call   sub_1_7DF1
        pop    ebx
        pop    ebp
        mov    word ptr [ebp + 1ch], ax
        ret
loc_1_9006:
        db  0CDh, 020h, 011h, 001h, 001h, 000h
        cmp    eax, 20000000h
        jb     SHORT loc_1_9036
        cmp    eax, 0a0000000h
        jb     SHORT loc_1_901D
        stc
        jmp    SHORT loc_1_9037
loc_1_901D:
        db  0E8h, 059h, 075h, 0FDh, 0FFh
        jb     SHORT loc_1_9037
        db  0CDh, 020h, 011h, 001h, 001h, 000h
        cmp    eax, 50000000h
        jb     SHORT loc_1_9036
        db  0E8h, 0CAh, 06Fh, 0FDh, 0FFh
loc_1_9036:
        clc
loc_1_9037:
        ret
loc_1_9038:
        stc
        ret
        db  08Bh, 0C0h
VSERVER_Service_02:
        sub    ebx, ebx
        ret
        db  090h
VSERVER_Service_03:
        cmp    esp, 0
        ret
VSERVER_Control:
        cmp    eax, 13h
        je     SHORT loc_1_90B0
        cmp    eax, 0bh
        je     SHORT loc_1_9095
        cmp    eax, 1bh
        je     SHORT loc_1_9006
        cmp    eax, 2
        db  00Fh, 084h, 01Fh, 075h, 0FDh, 0FFh
        cmp    eax, 17h
        je     SHORT loc_1_908D
        cmp    eax, 1ch
        je     SHORT loc_1_9038
        cmp    eax, 31h
        db  00Fh, 084h, 091h, 06Fh, 0FDh, 0FFh
        cmp    eax, 32h
        je     SHORT loc_1_90BC
        cmp    eax, 22h
        jne    SHORT loc_1_9082
        push   edx
        push   ebx
        db  0E8h, 0FCh, 074h, 0FDh, 0FFh
        stc
        ret
loc_1_9082:
        cmp    eax, 23h
        je     NEAR PTR loc_1_7FC6
        clc
        ret
loc_1_908D:
        db  0FEh, 005h, 024h, 031h, 001h, 000h
        clc
        ret
loc_1_9095:
        db  039h, 01Dh, 014h, 031h, 001h, 000h
        jne    SHORT loc_1_90A4
        db  0C6h, 005h, 0DCh, 01Ch, 001h, 000h, 001h
loc_1_90A4:
        clc
        ret
        db  0CDh, 020h, 004h, 000h, 001h, 000h, 075h, 000h, 0F8h, 0C3h
loc_1_90B0:
        db  039h, 01Dh, 014h, 031h, 001h, 000h
        je     SHORT loc_1_90BA
        clc
        ret
loc_1_90BA:
        int3
        ret
loc_1_90BC:
        db  0F7h, 005h, 038h, 034h, 001h, 000h, 010h, 000h, 000h, 000h
        je     SHORT loc_1_9113
        db  080h, 03Dh, 024h, 031h, 001h, 000h, 000h
        jne    SHORT loc_1_9113
        pushad
        mov    eax, 0
        call   sub_1_8B80
        push   ebx
        call   sub_1_7DDC
        push   32h
        push   2
        db  0CDh, 020h, 004h, 000h, 080h, 004h
        add    esp, 8
        pop    ebx
loc_1_90F0:
        cli
        db  083h, 03Dh, 008h, 023h, 001h, 000h, 000h
        je     SHORT loc_1_910C
        push   5
        db  068h, 008h, 023h, 001h, 000h
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        add    esp, 8
        jmp    SHORT loc_1_90F0
loc_1_910C:
        sti
        call   sub_1_8878
        popad
loc_1_9113:
        db  083h, 025h, 038h, 034h, 001h, 000h, 0EFh
        clc
        ret
sub_1_911C:
        pushfd
        mov    ecx, dword ptr [eax + 0ch]
        and    cl, 0feh
        mov    edx, eax
        cli
        cmp    dword ptr [edx], 0
        jne    SHORT loc_1_9155
        db  0B8h, 0C8h, 031h, 001h, 000h
        mov    dword ptr [eax + 0ch], 0ffffffffh
        nop
loc_1_9138:
        mov    eax, dword ptr [eax]
        sub    ecx, dword ptr [eax + 0ch]
        jae    SHORT loc_1_9138
        add    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [edx + 0ch], ecx
        sub    dword ptr [eax + 0ch], ecx
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], ecx
        mov    dword ptr [eax + 4], edx
loc_1_9155:
        popfd
        ret
        db  090h, 09Ch, 0FAh, 083h, 038h, 000h, 074h, 01Bh, 08Bh, 048h, 00Ch, 08Bh, 018h, 001h, 04Bh, 00Ch
        db  08Bh, 018h, 08Bh, 048h, 004h, 089h, 04Bh, 004h, 089h, 019h, 0C7h, 000h, 000h, 000h, 000h, 000h
        db  09Dh, 0F8h, 0C3h, 09Dh, 0F9h, 0C3h, 02Eh, 08Bh, 0C0h, 09Ch, 0FAh, 080h, 0E1h, 0FEh, 083h, 038h
        db  000h, 074h, 012h, 08Bh, 050h, 00Ch, 08Bh, 018h, 001h, 053h, 00Ch, 08Bh, 018h, 08Bh, 050h, 004h
        db  089h, 053h, 004h, 089h, 01Ah, 0BAh, 0C8h, 031h, 001h, 000h, 0C7h, 042h, 00Ch, 0FFh, 0FFh, 0FFh
        db  0FFh, 08Bh, 012h, 02Bh, 04Ah, 00Ch, 073h, 0F9h, 003h, 04Ah, 00Ch, 089h, 048h, 00Ch, 029h, 04Ah
        db  00Ch, 08Bh, 04Ah, 004h, 089h, 001h, 089h, 010h, 089h, 048h, 004h, 089h, 042h, 004h, 09Dh, 0C3h
        db  090h
sub_1_91C8:
        push   ebp
        db  0CDh, 020h, 006h, 000h, 040h, 000h
        db  0A3h, 028h, 028h, 001h, 000h
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        db  08Bh, 01Dh, 0C4h, 031h, 001h, 000h
        db  0A3h, 0C4h, 031h, 001h, 000h
        sub    eax, ebx
        xchg   ecx, eax
loc_1_91E8:
        cli
        db  0C7h, 005h, 0D4h, 031h, 001h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0A1h, 0C8h, 031h, 001h, 000h
        sub    dword ptr [eax + 0ch], ecx
        jbe    SHORT loc_1_9219
        sti
        mov    eax, 64h
        xor    edx, edx
        db  0BEh, 0C8h, 091h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        test   esi, esi
        je     SHORT loc_1_9216
        pop    ebp
        ret
loc_1_9216:
        int3
        pop    ebp
        ret
loc_1_9219:
        mov    ecx, dword ptr [eax + 0ch]
        neg    ecx
        mov    ebx, dword ptr [eax]
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ebx + 4], edx
        mov    dword ptr [edx], ebx
        mov    dword ptr [eax], 0
        sti
        push   eax
        push   ecx
        call   dword ptr [eax + 8]
        pop    ecx
        pop    eax
        jmp    SHORT loc_1_91E8
        db  0C3h
_LTEXT ends
        end
