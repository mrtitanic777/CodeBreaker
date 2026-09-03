; vredir_pcode_06_03.asm -- VREDIR object 6 segment _PTEXT  (part 3/4)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_6_88D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   13eh
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_88E3:
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    SHORT loc_6_88F9
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_88F9:
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 14h
        add    eax, 104h
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [eax + 36h], edx
        db  0C7h, 040h, 026h, 000h, 000h, 000h, 000h
        mov    edi, dword ptr [ebp - 4]
        db  0C7h, 040h, 02Ah, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 02Eh, 000h, 000h, 000h, 000h
        mov    ecx, 9
        add    eax, 26h
        xor    eax, eax
        rep stosd
        stosw
        mov    ebx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 24h], 3bh
        mov    dword ptr [ecx], ebx
        add    ecx, 26h
        mov    eax, ecx
        mov    dword ptr [ebp - 0ch], ecx
        db  0C7h, 041h, 0DEh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        db  0BFh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, 0ffffffffh
        mov    dword ptr [eax + 0ch], 0
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [ebp - 8]
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [edx]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        mov    esi, edi
        mov    ecx, 0ffffffffh
        mov    edi, dword ptr [ebp - 8]
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
        mov    edi, dword ptr [ebp - 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    eax, dword ptr [edx]
        mov    dword ptr [edi], eax
        mov    ecx, dword ptr [edx + 4]
        mov    dword ptr [edi + 4], ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [edx + 8]
        push   ecx
        mov    dword ptr [edi + 8], eax
        mov    dx, word ptr [edx + 0ch]
        mov    word ptr [edi + 0ch], dx
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_89EA:
        mov    byte ptr [ebx + 5], 0
        mov    dword ptr [ebx + 28h], 0
        mov    byte ptr [ebx + 4], 4
        mov    word ptr [ebx + 1ah], 0
        mov    word ptr [ebx + 18h], 1
        add    esp, 8
        mov    word ptr [ebx + 6], 0ffh
        push   ebx
        call   sub_6_1A70
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_6_8A88
        mov    ecx, dword ptr [ebp - 4]
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [ebx + 10h], ecx
        mov    byte ptr [ebx + 4], 7
        push   eax
        push   1388h
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   ecx
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8A41:
        add    esp, 1ch
        mov    ecx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ecx + 0ch]
        test   eax, eax
        je     SHORT loc_6_8A57
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8A54:
        add    esp, 4
loc_6_8A57:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 0ch]
        test   eax, eax
        je     SHORT loc_6_8A6A
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8A67:
        add    esp, 4
loc_6_8A6A:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 10h]
        test   eax, eax
        je     SHORT loc_6_8A88
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8A7A:
        add    esp, 4
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [ecx + 1eh]
        mov    dword ptr [ebx], eax
loc_6_8A88:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8A91:
        mov    ax, word ptr [ebx + 1ah]
        add    esp, 4
        test   ax, ax
        je     SHORT loc_6_8AB5
        cmp    ax, 0eah
        je     SHORT loc_6_8AB5
        cmp    ax, 84bh
        je     SHORT loc_6_8AB5
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_8AB5:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
VREDIR_Service_15:
        push   ebp
        mov    ebp, esp
        sub    esp, 74h
        push   ebx
        push   esi
        push   edi
        push   208h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8AD3:
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_6_8AE8
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_8AE8:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8AF4:
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_6_8B23
        lea    edi, [ebp - 74h]
        xor    eax, eax
        mov    ecx, 1dh
        push   ebx
        rep stosd
        lea    eax, [ebp - 74h]
        mov    dword ptr [ebp - 68h], esi
        push   eax
        call   sub_6_88D0
        add    esp, 8
        mov    eax, dword ptr [ebx + 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8B20:
        add    esp, 4
loc_6_8B23:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8B29:
        add    esp, 4
        movsx  eax, word ptr [ebp - 5ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_8B40:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        mov    ecx, dword ptr [ebp + 0ch]
        push   esi
        mov    dword ptr [ebp - 4], eax
        push   edi
        mov    dword ptr [ebp - 8], eax
        mov    ax, word ptr [ecx + 18h]
        mov    dword ptr [ebp - 0ch], eax
        mov    al, byte ptr [ecx + 10h]
        cmp    al, 2
        jbe    SHORT loc_6_8B72
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    word ptr [eax + 1ah], 7ch
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_8B72:
        xor    ecx, ecx
        mov    cl, al
        cmp    dword ptr [ebp + 10h], 0
        db  08Bh, 01Ch, 08Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_6_8B8A
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx]
        jmp    SHORT loc_6_8B97
loc_6_8B8A:
        push   3800h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8B94:
        add    esp, 4
loc_6_8B97:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        test   ecx, ecx
        jle    SHORT loc_6_8BDA
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 10h], ecx
loc_6_8BAA:
        mov    esi, dword ptr [edx]
        test   byte ptr [esi + 10h], 3
        je     SHORT loc_6_8BD2
        cmp    ebx, dword ptr [ebp - 0ch]
        jg     SHORT loc_6_8BCF
        mov    edi, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        add    eax, ebx
        sub    dword ptr [ebp - 0ch], ebx
        inc    dword ptr [ebp - 4]
loc_6_8BCF:
        inc    dword ptr [ebp - 8]
loc_6_8BD2:
        add    edx, 4
        dec    dword ptr [ebp - 10h]
        jne    SHORT loc_6_8BAA
loc_6_8BDA:
        mov    ax, word ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dx, word ptr [ebp - 4]
        pop    edi
        mov    word ptr [ecx + 18h], ax
        pop    esi
        mov    word ptr [ecx + 14h], dx
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_8C00:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   ebx
        cmp    byte ptr [eax + 14h], 0
        push   esi
        push   edi
        je     SHORT loc_6_8C1E
        mov    ecx, dword ptr [ebp + 8]
        pop    edi
        mov    word ptr [ecx + 1ah], 88fh
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_8C1E:
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_6_8C29
        mov    ebx, dword ptr [eax + 10h]
        jmp    SHORT loc_6_8C38
loc_6_8C29:
        push   3810h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8C33:
        add    esp, 4
        mov    ebx, eax
loc_6_8C38:
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 10h], 0
        mov    ax, word ptr [ebx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx]
        jne    SHORT loc_6_8C54
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8C51:
        add    esp, 4
loc_6_8C54:
        mov    ecx, dword ptr [ebp + 8]
        mov    edi, eax
        sub    eax, eax
        mov    edx, dword ptr [ecx + 2ch]
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, edx
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx], 58h
        jae    SHORT loc_6_8C8E
        mov    word ptr [ecx + 1ah], 84bh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_8C8E:
        push   edx
        call   sub_6_8DF0
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_6_8CAB
        mov    ecx, dword ptr [ebp + 8]
        pop    edi
        mov    word ptr [ecx + 1ah], 888h
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_8CAB:
        test   byte ptr [esi + 10h], 3
        jne    SHORT loc_6_8CBF
        mov    ecx, dword ptr [ebp + 8]
        pop    edi
        mov    word ptr [ecx + 1ah], 888h
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_8CBF:
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_6_8CCA
        mov    edi, dword ptr [ebx + 6]
        jmp    SHORT loc_6_8CD8
loc_6_8CCA:
        mov    eax, dword ptr [ebx + 6]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8CD3:
        add    esp, 4
        mov    edi, eax
loc_6_8CD8:
        mov    ecx, 16h
        rep movsd
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_8CF0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_6_8D04
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 10h]
        jmp    SHORT loc_6_8D11
loc_6_8D04:
        push   3810h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8D0E:
        add    esp, 4
loc_6_8D11:
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, eax
        mov    ecx, 16h
        mov    edx, dword ptr [ebx + 2ch]
        mov    edi, edx
        rep movsd
        mov    ecx, dword ptr [ebp + 0ch]
        mov    esi, edx
        xor    eax, eax
        mov    al, byte ptr [ecx + 14h]
        test   eax, eax
        je     SHORT loc_6_8D44
        cmp    eax, 1
        je     NEAR PTR loc_6_8DC7
        mov    word ptr [ebx + 1ah], 88fh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_8D44:
        xor    edi, edi
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, edi
        jle    SHORT loc_6_8D65
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_6_8D55:
        mov    edx, dword ptr [ecx]
        test   byte ptr [edx + 10h], 3
        je     SHORT loc_6_8D90
        add    ecx, 4
        inc    edi
        cmp    edi, eax
        jl     SHORT loc_6_8D55
loc_6_8D65:
        xor    eax, eax
        db  066h, 0A1h, 028h, 000h, 000h, 000h
        cmp    eax, edi
        jle    SHORT loc_6_8DBC
        push   5ch
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8D78:
        add    esp, 4
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        test   eax, eax
        mov    dword ptr [ecx + edi*4], eax
        je     SHORT loc_6_8DBC
        lea    eax, [edi + 1]
        db  0A3h, 000h, 000h, 000h, 000h
loc_6_8D90:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        xor    edx, edx
        xor    eax, eax
        mov    edi, dword ptr [ecx + edi*4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dx, word ptr [ecx + 3ch]
        shl    edx, 10h
        mov    ax, word ptr [ecx + 4]
        add    edx, eax
        mov    dword ptr [edi + 58h], edx
loc_6_8DB0:
        mov    ecx, 16h
        rep movsd
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_8DBC:
        mov    word ptr [ebx + 1ah], 885h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_8DC7:
        push   esi
        call   sub_6_8DF0
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_6_8DB0
        mov    word ptr [ebx + 1ah], 888h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_8DF0:
        push   ebp
        mov    ecx, 0ffffffffh
        mov    ebp, esp
        sub    eax, eax
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        repne scasb
        not    ecx
        mov    eax, ecx
        xor    edx, edx
        db  039h, 015h, 000h, 000h, 000h, 000h
        jle    SHORT loc_6_8E3F
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
loc_6_8E16:
        mov    edi, dword ptr [ebx]
        test   byte ptr [edi + 10h], 3
        je     SHORT loc_6_8E33
        mov    ecx, eax
        mov    esi, dword ptr [ebp + 8]
        shr    ecx, 2
        repe cmpsd
        jne    SHORT loc_6_8E31
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb
loc_6_8E31:
        je     SHORT loc_6_8E46
loc_6_8E33:
        add    ebx, 4
        inc    edx
        db  039h, 015h, 000h, 000h, 000h, 000h
        jg     SHORT loc_6_8E16
loc_6_8E3F:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_8E46:
        db  0A1h, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        mov    eax, dword ptr [eax + edx*4]
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_8E60:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 10h]
        test   ebx, ebx
        je     SHORT loc_6_8E77
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [edi]
        jmp    SHORT loc_6_8E89
loc_6_8E77:
        push   3800h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8E81:
        add    esp, 4
        mov    esi, eax
        mov    edi, dword ptr [ebp + 0ch]
loc_6_8E89:
        xor    eax, eax
        mov    al, byte ptr [edi + 10h]
        cmp    eax, 0eh
        ja     SHORT loc_6_8EA2
        xor    ecx, ecx
        db  08Ah, 088h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_6_8EA2:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    word ptr [eax + 1ah], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_8EB2:
        push   0ch
        lea    eax, [ebp - 0ch]
        push   esi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8EBE:
        add    esp, 0ch
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_6_8EFC
        test   ebx, ebx
        je     SHORT loc_6_8ED0
        mov    esi, dword ptr [ebp - 0ch]
        jmp    SHORT loc_6_8EDE
loc_6_8ED0:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8ED9:
        add    esp, 4
        mov    esi, eax
loc_6_8EDE:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        mov    edi, edx
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        push   edx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8EF9:
        add    esp, 0ch
loc_6_8EFC:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_6_8F37
        test   ebx, ebx
        je     SHORT loc_6_8F0B
        mov    edx, dword ptr [ebp - 8]
        jmp    SHORT loc_6_8F19
loc_6_8F0B:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8F14:
        add    esp, 4
        mov    edx, eax
loc_6_8F19:
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        mov    edi, esi
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        push   esi
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8F34:
        add    esp, 0ch
loc_6_8F37:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_6_906C
        test   ebx, ebx
        je     SHORT loc_6_8F4A
        mov    edx, dword ptr [ebp - 4]
        jmp    SHORT loc_6_8F58
loc_6_8F4A:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8F53:
        add    esp, 4
        mov    edx, eax
loc_6_8F58:
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8F74:
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_8F7E:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_6_8F9B
loc_6_8F86:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_6_8F9B
loc_6_8F8E:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_6_8F9B
loc_6_8F96:
        db  0BAh, 000h, 000h, 000h, 000h
loc_6_8F9B:
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        push   edx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8FB0:
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_8FBA:
        mov    word ptr [edi + 1ch], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_8FC7:
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_6_8FD9
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8FD6:
        add    esp, 4
loc_6_8FD9:
        xor    eax, eax
        mov    ax, word ptr [edi + 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_8FE5:
        add    esp, 4
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_6_900B
        mov    ecx, dword ptr [ebp + 8]
        pop    edi
        mov    word ptr [ecx + 1ah], 8
        pop    esi
        pop    ebx
        mov    esp, ebp
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        pop    ebp
        ret
loc_6_900B:
        xor    ecx, ecx
        db  0A3h, 000h, 000h, 000h, 000h
        mov    cx, word ptr [edi + 18h]
        push   ecx
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        push   esi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_9024:
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_902E:
        xor    eax, eax
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ax, word ptr [edi + 18h]
        cmp    eax, ecx
        jae    SHORT loc_6_9052
        mov    word ptr [edi + 18h], cx
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1ah], 0eah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_9052:
        push   ecx
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_905F:
        add    esp, 0ch
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    word ptr [edi + 18h], cx
loc_6_906C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 008h, 001h, 002h, 008h, 003h, 004h, 005h, 008h, 008h, 008h
        db  008h, 008h, 006h, 007h, 08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_90B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 10h]
        test   ebx, ebx
        je     SHORT loc_6_90CA
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi + 14h], eax
        jmp    SHORT loc_6_90E0
loc_6_90CA:
        push   3800h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_90D4:
        add    esp, 4
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [edi + 14h], eax
loc_6_90E0:
        test   ebx, ebx
        je     SHORT loc_6_90F0
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [esi + 18h]
        mov    dword ptr [edi + 10h], eax
        mov    dword ptr [edi], ecx
        jmp    SHORT loc_6_9103
loc_6_90F0:
        xor    eax, eax
        xor    ecx, ecx
        mov    ax, word ptr [esi + 38h]
        shl    eax, 10h
        mov    cx, word ptr [esi]
        add    eax, ecx
        mov    dword ptr [edi + 10h], eax
loc_6_9103:
        xor    eax, eax
        mov    ax, word ptr [esi + 10h]
        push   eax
        push   edi
        call   sub_6_9130
        add    esp, 8
        cmp    word ptr [edi + 1ah], 0
        je     SHORT loc_6_9120
        mov    eax, dword ptr [edi]
        mov    word ptr [esi + 14h], ax
loc_6_9120:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_9130:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + 5]
        db  08Bh, 034h, 085h, 000h, 000h, 000h, 000h
        test   esi, esi
        je     SHORT loc_6_914C
        add    esi, 13h
        jmp    SHORT loc_6_9151
loc_6_914C:
        db  0BEh, 000h, 000h, 000h, 000h
loc_6_9151:
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        jl     SHORT loc_6_9162
        cmp    eax, 1
        jle    SHORT loc_6_916B
        cmp    eax, 0ah
        je     SHORT loc_6_91BA
loc_6_9162:
        mov    word ptr [ecx + 1ah], 7ch
        pop    esi
        pop    ebp
        ret
loc_6_916B:
        lea    edx, [eax*8]
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   0
        db  068h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        push   1
        push   0
        push   0
        push   32h
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        push   esi
        push   eax
        db  08Bh, 0B2h, 004h, 000h, 000h, 000h
        db  08Bh, 092h, 000h, 000h, 000h, 000h
        push   esi
        push   edx
        push   ecx
        call   sub_6_9270
        add    esp, 3ch
        pop    esi
        pop    ebp
        ret
loc_6_91BA:
        push   0
        xor    eax, eax
        db  068h, 000h, 000h, 000h, 000h
        db  0A0h, 001h, 000h, 000h, 000h
        push   eax
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        xor    eax, eax
        db  0A0h, 000h, 000h, 000h, 000h
        push   eax
        push   edx
        push   esi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   16h
        push   ecx
        call   sub_6_9270
        add    esp, 28h
        pop    esi
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_6_9200:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, 1dh
        sub    esp, 74h
        push   esi
        push   edi
        lea    edi, [ebp - 74h]
        rep stosd
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 6]
        mov    dword ptr [ebp - 60h], eax
        mov    dword ptr [ebp - 64h], eax
        xor    eax, eax
        mov    ax, word ptr [esi + 4]
        mov    dword ptr [ebp - 74h], eax
        lea    eax, [ebp - 74h]
        movsx  ecx, word ptr [esi + 0ah]
        push   ecx
        push   eax
        call   sub_6_9130
        add    esp, 8
        cmp    word ptr [ebp - 5ah], 0
        je     SHORT loc_6_9253
        mov    ecx, dword ptr [ebp - 74h]
        mov    eax, dword ptr [esi]
        mov    word ptr [eax], cx
        pop    edi
        movsx  eax, word ptr [ebp - 5ah]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_6_9253:
        mov    cx, word ptr [esi + 4]
        mov    eax, dword ptr [esi]
        sub    cx, word ptr [ebp - 74h]
        pop    edi
        mov    word ptr [eax], cx
        pop    esi
        movsx  eax, word ptr [ebp - 5ah]
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_6_9270:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        mov    dword ptr [ebp - 0ch], eax
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edi]
        mov    ebx, dword ptr [edi + 14h]
        mov    dword ptr [ebp - 18h], ecx
        mov    dword ptr [ebp - 28h], ecx
        lea    eax, [ebx + ecx]
        cmp    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 24h], eax
        jae    SHORT loc_6_929F
        mov    word ptr [edi + 1ah], 84bh
loc_6_929F:
        lea    edx, [ebp + 14h]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 8], eax
        cmp    byte ptr [eax], 0
        je     NEAR PTR loc_6_94D8
loc_6_92B1:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 1ch], 0
        cmp    byte ptr [eax], 39h
        jg     SHORT loc_6_92E4
loc_6_92C0:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax]
        cmp    al, 30h
        jl     SHORT loc_6_92E4
        movsx  ecx, al
        mov    eax, dword ptr [ebp - 1ch]
        inc    dword ptr [ebp - 8]
        lea    esi, [eax + eax*4]
        mov    eax, dword ptr [ebp - 8]
        lea    ecx, [ecx + esi*2 - 30h]
        cmp    byte ptr [eax], 39h
        mov    dword ptr [ebp - 1ch], ecx
        jle    SHORT loc_6_92C0
loc_6_92E4:
        mov    edi, dword ptr [ebp + 8]
        mov    ax, word ptr [edi + 1ah]
        mov    word ptr [ebp - 20h], ax
        test   ax, ax
        je     SHORT loc_6_92F7
        lea    ebx, [ebp - 14h]
loc_6_92F7:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 4], al
        movsx  eax, al
        sub    eax, 42h
        cmp    eax, 32h
        ja     SHORT loc_6_9319
        xor    ecx, ecx
        db  08Ah, 088h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_6_9319:
        mov    edi, dword ptr [ebp + 8]
        cmp    byte ptr [ebp - 4], 0
        mov    word ptr [edi + 1ah], 1
        jne    NEAR PTR loc_6_94BC
        dec    dword ptr [ebp - 8]
        jmp    NEAR PTR loc_6_94BC
loc_6_9334:
        mov    al, byte ptr [edx]
        add    edx, 4
        mov    byte ptr [ebx], al
        inc    ebx
        jmp    NEAR PTR loc_6_94BC
loc_6_9341:
        mov    eax, dword ptr [edx]
        add    edx, 4
        mov    dword ptr [ebx], eax
        add    ebx, 4
        jmp    NEAR PTR loc_6_94BC
loc_6_9350:
        add    ebx, dword ptr [edx]
        add    edx, 4
        jmp    NEAR PTR loc_6_94BC
loc_6_935A:
        mov    esi, dword ptr [edx]
        add    edx, 4
        test   esi, esi
        jne    SHORT loc_6_9368
        db  0BEh, 000h, 000h, 000h, 000h
loc_6_9368:
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, ecx
        cmp    dword ptr [ebp - 18h], eax
        jle    SHORT loc_6_93AF
        mov    edi, dword ptr [ebp + 8]
        add    ebx, 4
        sub    dword ptr [ebp - 24h], ecx
        sub    dword ptr [ebp - 28h], ecx
        mov    edi, dword ptr [edi + 10h]
        mov    dword ptr [ebp - 0ch], ecx
        shr    ecx, 2
        add    edi, dword ptr [ebp - 28h]
        mov    dword ptr [ebx - 4], edi
        mov    edi, dword ptr [ebp - 24h]
        rep movsd
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 3
        rep movsb
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_6_94BC
loc_6_93AF:
        cmp    word ptr [ebp - 20h], 0
        jne    SHORT loc_6_93BC
        mov    dword ptr [ebx], 0
loc_6_93BC:
        add    ebx, 4
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_6_94BC
loc_6_93C7:
        mov    eax, dword ptr [edx]
        add    edx, 4
        mov    word ptr [ebx], ax
        add    ebx, 2
        jmp    NEAR PTR loc_6_94BC
loc_6_93D7:
        mov    byte ptr [ebx], 0
        inc    ebx
        jmp    NEAR PTR loc_6_94BC
loc_6_93E0:
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        add    edx, 4
        mov    dword ptr [ebp - 4], eax
        add    edx, 4
        cmp    word ptr [ebp - 20h], 0
        jne    SHORT loc_6_9426
        test   esi, esi
        je     SHORT loc_6_9411
        mov    edi, ebx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        add    ebx, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_6_94BC
loc_6_9411:
        mov    esi, dword ptr [ebp - 4]
        mov    edi, ebx
        xor    eax, eax
        mov    ecx, esi
        shr    ecx, 2
        rep stosd
        mov    ecx, esi
        and    ecx, 3
        rep stosb
loc_6_9426:
        add    ebx, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_6_94BC
loc_6_942E:
        mov    esi, dword ptr [edx]
        add    edx, 4
        jmp    SHORT loc_6_947A
loc_6_9435:
        mov    ecx, dword ptr [edx]
        add    edx, 4
        test   ecx, ecx
        jne    SHORT loc_6_9444
        mov    byte ptr [ebp - 14h], 0
        jmp    SHORT loc_6_9477
loc_6_9444:
        cmp    ecx, 1ah
        jg     SHORT loc_6_945B
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        add    cl, 40h
        mov    byte ptr [ebp - 14h], cl
        mov    word ptr [ebp - 13h], ax
        jmp    SHORT loc_6_9477
loc_6_945B:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        db  0B8h, 000h, 000h, 000h, 000h
        lea    esi, [ebp - 14h]
        sub    cl, 1bh
        mov    al, byte ptr [eax + 4]
        mov    dword ptr [esi], edi
        mov    byte ptr [esi + 4], al
        add    byte ptr [ebp - 11h], cl
loc_6_9477:
        lea    esi, [ebp - 14h]
loc_6_947A:
        mov    eax, dword ptr [edx]
        add    edx, 4
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        cmp    word ptr [edi + 1ah], 0
        jne    SHORT loc_6_94BC
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx - 1]
        cmp    dword ptr [ebp - 4], eax
        jg     SHORT loc_6_94A5
        mov    eax, dword ptr [ebp - 4]
        dec    eax
loc_6_94A5:
        mov    edi, ebx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    byte ptr [eax + ebx], 0
        add    ebx, dword ptr [ebp - 4]
loc_6_94BC:
        dec    dword ptr [ebp - 1ch]
        cmp    dword ptr [ebp - 1ch], 0
        jg     NEAR PTR loc_6_92E4
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        cmp    byte ptr [eax], 0
        jne    NEAR PTR loc_6_92B1
loc_6_94D8:
        mov    edi, dword ptr [ebp + 8]
        cmp    word ptr [edi + 1ah], 0
        jne    SHORT loc_6_94F0
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 18h], eax
        jge    SHORT loc_6_94F0
        mov    word ptr [edi + 1ah], 0eah
loc_6_94F0:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 0ch]
        mov    cx, word ptr [edi + 1ah]
        test   cx, cx
        je     SHORT loc_6_950B
        mov    dword ptr [edi], eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        movsx  eax, cx
        pop    ebp
        ret
loc_6_950B:
        mov    edi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        sub    dword ptr [edi], eax
        add    dword ptr [edi + 14h], edx
        add    dword ptr [edi + 10h], edx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        movsx  eax, cx
        pop    ebp
        ret
        db  090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 009h, 001h, 009h, 009h, 009h, 009h
        db  002h, 009h, 009h, 009h, 009h, 009h, 009h, 009h, 009h, 009h, 003h, 009h, 009h, 009h, 004h, 009h
        db  009h, 005h, 009h, 009h, 009h, 009h, 009h, 009h, 009h, 009h, 006h, 009h, 009h, 009h, 009h, 009h
        db  009h, 009h, 009h, 009h, 009h, 009h, 009h, 009h, 009h, 009h, 007h, 008h, 090h
sub_6_9580:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        sub    esp, 54h
        db  0A1h, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        mov    dword ptr [ebp - 24h], edx
        push   edi
        mov    dword ptr [ebp - 50h], edx
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 1ch], edx
        lea    eax, [ebp - 38h]
        mov    dword ptr [ebp - 3ch], edx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [eax], edx
        mov    dword ptr [ebp - 18h], ecx
        mov    dword ptr [eax + 4], edx
        db  039h, 015h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 8], edx
        mov    dword ptr [eax + 0ch], edx
        mov    dword ptr [eax + 10h], edx
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [ebp - 54h], eax
        je     SHORT loc_6_95E2
        mov    ecx, dword ptr [ebp + 0ch]
        pop    edi
        mov    word ptr [ecx + 18h], dx
        pop    esi
        mov    word ptr [ebx + 1ah], 5
        mov    word ptr [ecx + 10h], dx
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_95E2:
        xor    esi, esi
        db  066h, 039h, 035h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_6_9E4B
        db  039h, 035h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_6_9E4B
        push   56h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_9604:
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    eax, esi
        jne    SHORT loc_6_9629
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 18h], si
        pop    edi
        mov    word ptr [eax + 1ah], 8
        mov    word ptr [ecx + 10h], si
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_9629:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        cmp    byte ptr [eax + 1ch], 4ch
        mov    dword ptr [ecx], 0
        jne    SHORT loc_6_9697
        mov    ax, word ptr [eax + 10h]
        add    ecx, 16h
        and    ax, 0ffh
        xor    edx, edx
        mov    word ptr [ecx], ax
        mov    dword ptr [ebp - 14h], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dx, word ptr [eax + 38h]
        add    ecx, 12h
        shl    edx, 10h
        xor    eax, eax
        mov    dword ptr [ebp - 8], ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [ecx]
        mov    ecx, dword ptr [ebp - 8]
        add    edx, eax
        mov    dword ptr [ecx], edx
        mov    ecx, dword ptr [ebp - 4]
        add    ecx, 10h
        mov    edx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [edx + 18h]
        mov    dword ptr [ebp - 0ch], ecx
        mov    word ptr [ecx], ax
        mov    ecx, dword ptr [ebp - 4]
        add    ecx, 4
        mov    dword ptr [ebp - 20h], ecx
        mov    dword ptr [ecx], 2
        jmp    NEAR PTR loc_6_979D
loc_6_9697:
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_6_96A5
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 4]
        jmp    SHORT loc_6_96B2
loc_6_96A5:
        push   3c04h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_96AF:
        add    esp, 4
loc_6_96B2:
        push   10h
        push   eax
        lea    eax, [ebp - 4ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_96BE:
        mov    ax, word ptr [ebp - 4ch]
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 16h], ax
        add    ecx, 16h
        mov    eax, dword ptr [ebp - 4ah]
        mov    dword ptr [ebp - 14h], ecx
        mov    ecx, dword ptr [ebp - 4]
        add    ecx, 12h
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ecx], eax
        mov    ax, word ptr [ebp - 46h]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 10h], ax
        add    ecx, 10h
        mov    eax, dword ptr [ebp - 44h]
        mov    dword ptr [ebp - 0ch], ecx
        mov    ecx, dword ptr [ebp - 4]
        add    ecx, 4
        mov    dword ptr [ebp - 20h], ecx
        mov    dword ptr [ecx], eax
        cmp    dword ptr [ebp - 40h], 0
        je     NEAR PTR loc_6_979D
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_6_9714
        mov    esi, dword ptr [ebp - 40h]
        jmp    SHORT loc_6_9722
loc_6_9714:
        mov    eax, dword ptr [ebp - 40h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_971D:
        add    esp, 4
        mov    esi, eax
loc_6_9722:
        cmp    byte ptr [esi], 0
        je     SHORT loc_6_979D
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edi, [ecx - 1]
        cmp    edi, 0fh
        jle    SHORT loc_6_9747
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1ah], 35h
        jmp    SHORT loc_6_9763
loc_6_9747:
        lea    eax, [edi + 1]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_9750:
        add    esp, 4
        mov    dword ptr [ebp - 10h], eax
        test   eax, eax
        jne    SHORT loc_6_9763
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1ah], 8
loc_6_9763:
        mov    eax, dword ptr [ebp + 8]
        cmp    word ptr [eax + 1ah], 0
        je     SHORT loc_6_978F
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [ecx + 18h], 0
        mov    word ptr [ecx + 10h], 0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_9785:
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_978F:
        push   edi
        mov    eax, dword ptr [ebp - 10h]
        push   esi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_979A:
        add    esp, 0ch
loc_6_979D:
        mov    ecx, dword ptr [ebp - 14h]
        cmp    word ptr [ecx], 1
        jle    SHORT loc_6_97ED
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_6_97C2
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp - 10h], eax
        je     SHORT loc_6_97C2
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_97BF:
        add    esp, 4
loc_6_97C2:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 1ah], 7ch
        mov    word ptr [ecx + 18h], 0
        mov    word ptr [ecx + 10h], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_97E3:
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_97ED:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ecx]
        test   eax, eax
        jne    SHORT loc_6_97FE
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [ecx], 0
loc_6_97FE:
        cmp    dword ptr [ebp + 10h], 0
        jne    SHORT loc_6_980D
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_980A:
        add    esp, 4
loc_6_980D:
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        mov    dword ptr [ebp - 3ch], eax
        jbe    NEAR PTR loc_6_98AE
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 0ch]
        mov    ax, word ptr [ecx]
        mov    edx, dword ptr [ebp - 8]
        add    eax, dword ptr [edx]
        mov    dword ptr [ebp - 34h], eax
        cmp    dword ptr [edx], 0
        je     SHORT loc_6_9895
        mov    ax, word ptr [ecx]
        test   ax, ax
        je     SHORT loc_6_9895
        movzx  eax, ax
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_9844:
        add    esp, 4
        mov    dword ptr [ebp - 1ch], eax
        test   eax, eax
        jne    SHORT loc_6_9895
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_6_986A
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp - 10h], eax
        je     SHORT loc_6_986A
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_9867:
        add    esp, 4
loc_6_986A:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 1ah], 8
        mov    word ptr [ecx + 18h], 0
        mov    word ptr [ecx + 10h], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_988B:
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_9895:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 1ch]
        lea    edx, [ebp - 28h]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [ebp - 26h]
        mov    dword ptr [ecx + 0ch], edx
        mov    dword ptr [ecx + 8], eax
        jmp    SHORT loc_6_98BD
loc_6_98AE:
        lea    eax, [ebp - 2ch]
        mov    ecx, dword ptr [ebp - 4]
        lea    edx, [ebp - 2ah]
        mov    dword ptr [ecx + 0ch], eax
        mov    dword ptr [ecx + 8], edx
loc_6_98BD:
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        xor    edi, edi
        xor    eax, eax
        mov    dword ptr [ebp - 8], edi
loc_6_98CA:
        mov    ecx, dword ptr [ebp - 10h]
        lea    esi, [ebx + 23h]
loc_6_98D0:
        mov    dl, byte ptr [ecx]
        cmp    dl, byte ptr [esi]
        jne    SHORT loc_6_98F0
        or     dl, dl
        je     SHORT loc_6_98EC
        mov    dl, byte ptr [ecx + 1]
        cmp    dl, byte ptr [esi + 1]
        jne    SHORT loc_6_98F0
        add    ecx, 2
        add    esi, 2
        or     dl, dl
        jne    SHORT loc_6_98D0
loc_6_98EC:
        xor    ecx, ecx
        jmp    SHORT loc_6_98F5
loc_6_98F0:
        sbb    ecx, ecx
        sbb    ecx, -1
loc_6_98F5:
        test   ecx, ecx
        je     SHORT loc_6_9907
        mov    ecx, dword ptr [ebx]
        test   ecx, ecx
        je     SHORT loc_6_990C
        inc    eax
        mov    dword ptr [ebp - 8], ebx
        mov    ebx, ecx
        jmp    SHORT loc_6_98CA
loc_6_9907:
        mov    edi, 1
loc_6_990C:
        test   edi, edi
        jne    NEAR PTR loc_6_99D9
        cmp    eax, 4
        jge    SHORT loc_6_9968
        push   195h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_9923:
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_6_9968
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_6_993D
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], ecx
        jmp    SHORT loc_6_9945
loc_6_993D:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebp - 8], eax
loc_6_9945:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, 8
        mov    edx, 5ch
        mov    dword ptr [ecx], ebx
        lea    ecx, [ebx + 50h]
loc_6_9957:
        mov    byte ptr [ecx], dl
        add    ecx, 2ch
        dec    eax
        mov    byte ptr [ecx - 2dh], dl
        mov    byte ptr [ecx - 3eh], dl
        mov    byte ptr [ecx - 3fh], dl
        jne    SHORT loc_6_9957
loc_6_9968:
        xor    edx, edx
        cmp    dword ptr [ebp - 8], edx
        je     NEAR PTR loc_6_9B3D
        cmp    byte ptr [ebx + 0ch], dl
        jne    NEAR PTR loc_6_9B3D
        mov    ecx, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 10h]
        sub    eax, eax
        mov    ebx, dword ptr [ecx]
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        lea    edi, [ebx + 23h]
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 8], edx
        mov    dword ptr [ebx + 4], edx
        mov    dword ptr [ebx + 0fh], eax
        inc    eax
        mov    ecx, 8
        mov    byte ptr [ebx + 0dh], 9
        mov    byte ptr [ebx + 0eh], 2
        db  0A3h, 000h, 000h, 000h, 000h
        lea    eax, [ebx + 51h]
loc_6_99C8:
        mov    byte ptr [eax], dl
        add    eax, 2ch
        dec    ecx
        mov    byte ptr [eax - 3eh], dl
        mov    dword ptr [eax - 48h], edx
        mov    dword ptr [eax - 44h], edx
        jne    SHORT loc_6_99C8
loc_6_99D9:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_6_99FA
        mov    ecx, dword ptr [ebp - 8]
        db  039h, 00Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_6_99FA
        mov    eax, dword ptr [ebx]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx]
        mov    dword ptr [ebx], eax
        mov    dword ptr [edx], ebx
loc_6_99FA:
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 18h], ax
        inc    byte ptr [ebx + 0ch]
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        mov    word ptr [ecx + 10h], ax
        jbe    SHORT loc_6_9A2E
        mov    word ptr [ebp - 2ah], ax
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [ebp - 2ch], ax
        mov    ax, word ptr [ecx]
        mov    ecx, dword ptr [ebp - 34h]
        add    eax, dword ptr [ebp - 3ch]
        mov    dword ptr [ebp - 30h], ecx
        mov    dword ptr [ebp - 38h], eax
loc_6_9A2E:
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        test   esi, esi
        je     SHORT loc_6_9A6D
loc_6_9A38:
        xor    eax, eax
        mov    al, byte ptr [esi + 5eh]
        test   dword ptr [ebx + 8], eax
        jne    SHORT loc_6_9A66
        mov    eax, dword ptr [esi + 14h]
        mov    ecx, eax
        lea    edx, [eax + eax*4]
        lea    eax, [edx + edx*8]
        sub    eax, ecx
        mov    edi, dword ptr [eax + ebx + 35h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_9A58:
        cmp    edi, eax
        jbe    SHORT loc_6_9A66
        xor    eax, eax
        mov    al, byte ptr [esi + 5eh]
        not    eax
        and    dword ptr [ebp - 18h], eax
loc_6_9A66:
        mov    esi, dword ptr [esi + 18h]
        test   esi, esi
        jne    SHORT loc_6_9A38
loc_6_9A6D:
        mov    dword ptr [ebp - 14h], 1
loc_6_9A74:
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_6_9D0B
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebx + 8], eax
        je     SHORT loc_6_9ADF
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        test   esi, esi
        je     SHORT loc_6_9ADF
loc_6_9A92:
        xor    eax, eax
        mov    al, byte ptr [esi + 5eh]
        test   dword ptr [ebp - 18h], eax
        je     SHORT loc_6_9AD8
        test   dword ptr [ebx + 8], eax
        jne    SHORT loc_6_9AD8
        cmp    dword ptr [ebp - 14h], 2
        mov    eax, 1
        jg     SHORT loc_6_9AAE
        xor    eax, eax
loc_6_9AAE:
        push   eax
        push   0
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   ebx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_9ABC:
        add    esp, 14h
        cmp    dword ptr [ebp - 14h], 6
        jne    SHORT loc_6_9AD8
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 5ah]
        push   eax
        push   ebx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_9AD5:
        add    esp, 0ch
loc_6_9AD8:
        mov    esi, dword ptr [esi + 18h]
        test   esi, esi
        jne    SHORT loc_6_9A92
loc_6_9ADF:
        mov    eax, dword ptr [ebp - 18h]
        test   dword ptr [ebx + 8], eax
        jne    SHORT loc_6_9AF4
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_9AF1:
        add    esp, 8
loc_6_9AF4:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        test   edi, edi
        je     NEAR PTR loc_6_9CFC
loc_6_9B02:
        mov    esi, dword ptr [edi + 14h]
        xor    eax, eax
        mov    al, byte ptr [edi + 5eh]
        test   dword ptr [ebp - 18h], eax
        je     NEAR PTR loc_6_9CF1
        mov    eax, dword ptr [ebp + 8]
        db  039h, 01Dh, 000h, 000h, 000h, 000h
        movzx  cx, byte ptr [edi + 5ah]
        mov    word ptr [eax + 6], cx
        jne    SHORT loc_6_9B84
        lea    eax, [ebp - 3ch]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   edx
        call   sub_6_9E70
        add    esp, 0ch
        jmp    SHORT loc_6_9B89
loc_6_9B3D:
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_6_9B59
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp - 10h], eax
        je     SHORT loc_6_9B59
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_9B56:
        add    esp, 4
loc_6_9B59:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 1ah], 8
        mov    word ptr [ecx + 18h], 0
        mov    word ptr [ecx + 10h], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_9B7A:
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_9B84:
        mov    eax, 1
loc_6_9B89:
        test   eax, eax
        je     NEAR PTR loc_6_9C3D
        xor    eax, eax
        mov    al, byte ptr [edi + 5eh]
        test   dword ptr [ebx + 8], eax
        je     NEAR PTR loc_6_9CF1
        lea    eax, [esi + esi*4]
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [eax + eax*8]
        sub    eax, esi
        mov    dword ptr [ebp - 0ch], eax
        add    eax, ebx
        cmp    byte ptr [eax + 51h], 0
        jne    SHORT loc_6_9BC0
        add    ecx, 18h
        add    eax, 3dh
        mov    dword ptr [ebp - 40h], ecx
        jmp    SHORT loc_6_9BC9
loc_6_9BC0:
        add    ecx, 18h
        add    eax, 4fh
        mov    dword ptr [ebp - 40h], ecx
loc_6_9BC9:
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 54h]
        mov    eax, dword ptr [ebp + 8]
        push   edx
        mov    dword ptr [eax + 0ch], ecx
        mov    cx, 1
        mov    eax, 1
        db  066h, 03Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        adc    eax, -1
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_6_9F10
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_6_9C3D
        mov    edx, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 40h]
        sub    edx, dword ptr [ecx]
        add    edx, ebx
        cmp    edx, -3dh
        je     SHORT loc_6_9C35
        push   1
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, ebx
        mov    ecx, dword ptr [ebp + 8]
        add    eax, 51h
        push   eax
        push   edi
        push   ebx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_9C25:
        add    esp, 14h
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ebx + ecx + 51h], 0
        jmp    NEAR PTR loc_6_9CF1
loc_6_9C35:
        test   eax, eax
        jne    NEAR PTR loc_6_9CCE
loc_6_9C3D:
        xor    eax, eax
        inc    dword ptr [ebp - 50h]
        mov    al, byte ptr [edi + 5eh]
        mov    ecx, eax
        or     dword ptr [ebx + 8], eax
        not    ecx
        mov    eax, dword ptr [ebp - 20h]
        and    dword ptr [ebp - 18h], ecx
        cmp    dword ptr [eax], 80000000h
        je     SHORT loc_6_9C66
        lea    eax, [esi + esi*4]
        lea    eax, [eax + eax*8]
        sub    eax, esi
        inc    dword ptr [ebx + eax + 39h]
loc_6_9C66:
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        jbe    SHORT loc_6_9CE8
        lea    eax, [esi + esi*4]
        lea    eax, [eax + eax*8]
        sub    eax, esi
        mov    dword ptr [ebp - 0ch], eax
        lea    ecx, [ebx + eax]
        mov    eax, 2
        cmp    byte ptr [ecx + 51h], 0
        je     SHORT loc_6_9C9B
        xor    eax, eax
        db  0A0h, 000h, 000h, 000h, 000h
        cmp    eax, dword ptr [ecx + 39h]
        mov    eax, 1
        adc    eax, -1
loc_6_9C9B:
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [ebp - 3ch]
        mov    edx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   edx
        call   sub_6_A110
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_6_9CE8
        push   eax
        push   0
        push   esi
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_9CBE:
        add    esp, 10h
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebx + eax + 39h], 0
        jmp    SHORT loc_6_9CE8
loc_6_9CCE:
        cmp    dword ptr [ebp - 24h], 0
        jne    SHORT loc_6_9CDE
        mov    eax, dword ptr [ebp + 8]
        movsx  ecx, word ptr [eax + 1ah]
        mov    dword ptr [ebp - 24h], ecx
loc_6_9CDE:
        xor    eax, eax
        mov    al, byte ptr [edi + 5eh]
        not    eax
        and    dword ptr [ebx + 8], eax
loc_6_9CE8:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1ah], 0
loc_6_9CF1:
        mov    edi, dword ptr [edi + 18h]
        test   edi, edi
        jne    NEAR PTR loc_6_9B02
loc_6_9CFC:
        mov    eax, dword ptr [ebp - 14h]
        inc    dword ptr [ebp - 14h]
        cmp    eax, 6
        jl     NEAR PTR loc_6_9A74
loc_6_9D0B:
        db  0A1h, 000h, 000h, 000h, 000h
        dec    byte ptr [ebx + 0ch]
        cmp    dword ptr [ebx + 8], eax
        je     SHORT loc_6_9D6C
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        test   esi, esi
        je     SHORT loc_6_9D6C
loc_6_9D22:
        mov    edi, dword ptr [esi + 14h]
        xor    eax, eax
        mov    al, byte ptr [esi + 5eh]
        test   dword ptr [ebx + 8], eax
        jne    SHORT loc_6_9D65
        lea    eax, [edi + edi*4]
        lea    eax, [eax + eax*8]
        sub    eax, edi
        mov    dword ptr [ebp - 0ch], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_9D3F:
        mov    ecx, dword ptr [ebp - 0ch]
        cmp    eax, dword ptr [ebx + ecx + 35h]
        jb     SHORT loc_6_9D65
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   ebx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_9D53:
        add    esp, 0ch
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_9D5B:
        add    eax, 1eh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebx + ecx + 35h], eax
loc_6_9D65:
        mov    esi, dword ptr [esi + 18h]
        test   esi, esi
        jne    SHORT loc_6_9D22
loc_6_9D6C:
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        jne    SHORT loc_6_9DA9
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [edx + 14h]
        mov    ecx, eax
        lea    esi, [eax + eax*4]
        lea    eax, [esi + esi*8]
        sub    eax, ecx
        xor    ecx, ecx
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax + ebx + 39h], ecx
        jbe    SHORT loc_6_9DA9
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   edx
        push   ebx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_9DA6:
        add    esp, 14h
loc_6_9DA9:
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_6_9DC5
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp - 10h], eax
        je     SHORT loc_6_9DC5
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_9DC2:
        add    esp, 4
loc_6_9DC5:
        mov    ax, word ptr [ebp - 2ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dx, word ptr [ebp - 2ah]
        mov    word ptr [ecx + 10h], ax
        mov    word ptr [ecx + 18h], dx
        cmp    ax, dx
        jae    SHORT loc_6_9DF5
        test   dx, dx
        mov    eax, dword ptr [ebp + 8]
        je     SHORT loc_6_9DED
        mov    word ptr [eax + 1ah], 0eah
        jmp    SHORT loc_6_9E1D
loc_6_9DED:
        mov    word ptr [eax + 1ah], 84bh
        jmp    SHORT loc_6_9E1D
loc_6_9DF5:
        cmp    dword ptr [ebp - 50h], 0
        jne    SHORT loc_6_9E1D
        mov    eax, dword ptr [ebp + 8]
        cmp    word ptr [eax + 1ah], 0
        jne    SHORT loc_6_9E0D
        mov    cx, word ptr [ebp - 24h]
        mov    word ptr [eax + 1ah], cx
loc_6_9E0D:
        mov    eax, dword ptr [ebp + 8]
        cmp    word ptr [eax + 1ah], 0
        jne    SHORT loc_6_9E1D
        mov    word ptr [eax + 1ah], 17e6h
loc_6_9E1D:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 1ch], 0
        mov    word ptr [ecx + 1ch], 0
        je     SHORT loc_6_9E38
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_9E35:
        add    esp, 4
loc_6_9E38:
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_9E41:
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_9E4B:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 1ah], 836h
        xor    eax, eax
        pop    edi
        mov    word ptr [ecx + 18h], ax
        pop    esi
        mov    word ptr [ecx + 10h], ax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_6_9E70:
        push   ebp
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ebp, esp
        test   ecx, ecx
        push   ebx
        push   esi
        push   edi
        jne    SHORT loc_6_9E8A
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_9E8A:
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [ebp + 10h]
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        mov    esi, dword ptr [edi + 12h]
        jne    SHORT loc_6_9EA2
        mov    eax, dword ptr [ebx]
        mov    dword ptr [edi + 12h], eax
loc_6_9EA2:
        xor    edx, edx
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 6]
        push   edx
        push   edi
        call   ecx
        add    esp, 8
        mov    dword ptr [edi + 12h], esi
        test   eax, eax
        je     SHORT loc_6_9ED1
        cmp    eax, 0eah
        je     SHORT loc_6_9ED1
        cmp    eax, 84bh
        je     SHORT loc_6_9ED1
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_9ED1:
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        jne    SHORT loc_6_9F00
        cmp    word ptr [edi + 16h], 0
        je     SHORT loc_6_9F00
        mov    edx, dword ptr [ebx]
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 10h]
        sub    esi, edx
        mov    eax, ecx
        dec    ecx
        test   eax, eax
        je     SHORT loc_6_9F00
loc_6_9EF3:
        add    dword ptr [edx + 16h], esi
        add    edx, 1ah
        mov    eax, ecx
        dec    ecx
        test   eax, eax
        jne    SHORT loc_6_9EF3
loc_6_9F00:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_9F10:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        push   104h
        lea    edi, [esi + 42h]
        lea    ecx, [esi + 1ch]
        mov    dword ptr [ebp - 4], ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_9F2F:
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    SHORT loc_6_9F4E
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, 8
        mov    word ptr [ebx + 1ah], 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_9F4E:
        lea    eax, [esi + 18h]
        db  0C7h, 007h, 000h, 000h, 000h, 000h
        cmp    word ptr [esi + 16h], 0
        mov    dword ptr [edi + 10h], eax
        db  0B8h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_9F6B
        db  0B8h, 000h, 000h, 000h, 000h
loc_6_9F6B:
        mov    ecx, 9
        mov    dword ptr [edi + 4], eax
        db  0C7h, 047h, 008h, 000h, 000h, 000h, 000h
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        rep stosd
        stosw
        mov    ebx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 4]
        add    ecx, 26h
        mov    word ptr [ecx - 2], 68h
        mov    dword ptr [ecx - 26h], ebx
        mov    dword ptr [ebp - 0ch], ecx
        mov    edi, dword ptr [esi + 18h]
        sub    eax, eax
        db  0C7h, 041h, 0DEh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 0ch], 0
        mov    dword ptr [edx + 8], ecx
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [ebp - 8]
        rep movsd
        mov    ecx, edx
        db  0BAh, 000h, 000h, 000h, 000h
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [ebp - 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    eax, dword ptr [edx]
        mov    dword ptr [edi], eax
        mov    ecx, dword ptr [edx + 4]
        mov    dword ptr [edi + 4], ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    esi, dword ptr [edx + 8]
        push   ecx
        mov    dword ptr [edi + 8], esi
        mov    ax, word ptr [edx + 0ch]
        mov    edx, dword ptr [ebx + 0ch]
        mov    word ptr [edi + 0ch], ax
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A006:
        mov    byte ptr [ebx + 5], 0
        mov    dword ptr [ebx + 28h], 0
        add    esp, 8
        mov    word ptr [ebx + 1ah], 0
        push   4
        push   1
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A024:
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_6_A043
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A034:
        add    esp, 4
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_A043:
        cmp    dword ptr [ebp + 10h], 1
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 10h], ecx
        sbb    cl, cl
        and    cl, 0fah
        add    cl, 7
        cmp    dword ptr [ebp + 14h], 0
        mov    byte ptr [ebx + 4], cl
        je     SHORT loc_6_A064
        or     cl, 4
        mov    byte ptr [ebx + 4], cl
loc_6_A064:
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        push   1388h
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   ecx
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A083:
        add    esp, 1ch
        mov    ecx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ecx + 0ch]
        test   eax, eax
        je     SHORT loc_6_A099
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A096:
        add    esp, 4
loc_6_A099:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 0ch]
        test   eax, eax
        je     SHORT loc_6_A0AC
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A0A9:
        add    esp, 4
loc_6_A0AC:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 10h]
        test   eax, eax
        je     SHORT loc_6_A0CA
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A0BC:
        add    esp, 4
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [ecx + 1eh]
        mov    dword ptr [ebx], eax
loc_6_A0CA:
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A0D0:
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebx + 1ch], 0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A0E3:
        mov    ax, word ptr [ebx + 1ah]
        add    esp, 4
        test   ax, ax
        je     SHORT loc_6_A107
        cmp    ax, 0eah
        je     SHORT loc_6_A107
        cmp    ax, 84bh
        je     SHORT loc_6_A107
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_A107:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_6_A110:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        xor    eax, eax
        sub    esp, 1ch
        mov    ecx, dword ptr [ebp + 10h]
        mov    ax, word ptr [ecx + 14h]
        mov    dword ptr [ebp - 14h], edx
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0ch]
        movsx  ecx, word ptr [ebx + 16h]
        mov    dword ptr [ebp - 4], eax
        db  08Bh, 004h, 08Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 10h], eax
        mov    ax, word ptr [ecx + 16h]
        sub    ax, word ptr [ebp - 4]
        add    word ptr [ecx + 12h], ax
        mov    eax, dword ptr [ebx + 12h]
        cmp    dword ptr [ebp + 14h], 1
        mov    dword ptr [ebp - 0ch], eax
        jle    SHORT loc_6_A15B
        mov    edx, dword ptr [ebp - 4]
        jmp    SHORT loc_6_A179
loc_6_A15B:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_6_A179
        db  0A1h, 000h, 000h, 000h, 000h
        mov    edx, eax
        inc    eax
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp - 4], edx
        jge    SHORT loc_6_A179
        mov    eax, edx
        cdq
        idiv   dword ptr [ebp - 4]
loc_6_A179:
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [ebp - 4]
        test   eax, eax
        je     NEAR PTR loc_6_A2BB
        mov    ecx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebp - 1ch], eax
loc_6_A18F:
        xor    edi, edi
        mov    ebx, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp + 10h]
        mov    ax, word ptr [ecx + 10h]
        mov    word ptr [ebp - 8], ax
        cmp    ax, di
        je     SHORT loc_6_A1E6
loc_6_A1A4:
        mov    eax, ebx
        mov    esi, dword ptr [ebp - 0ch]
loc_6_A1A9:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [esi]
        jne    SHORT loc_6_A1C9
        or     cl, cl
        je     SHORT loc_6_A1C5
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [esi + 1]
        jne    SHORT loc_6_A1C9
        add    eax, 2
        add    esi, 2
        or     cl, cl
        jne    SHORT loc_6_A1A9
loc_6_A1C5:
        xor    eax, eax
        jmp    SHORT loc_6_A1CE
loc_6_A1C9:
        sbb    eax, eax
        sbb    eax, -1
loc_6_A1CE:
        test   eax, eax
        je     NEAR PTR loc_6_A2A7
        add    ebx, dword ptr [ebp - 10h]
        inc    edi
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, edi
        jg     SHORT loc_6_A1A4
loc_6_A1E6:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    word ptr [ecx + 16h], 0
        jle    SHORT loc_6_A223
        cmp    dword ptr [ebp + 14h], 0
        je     SHORT loc_6_A20E
        cmp    dword ptr [ebp - 4], edx
        jg     SHORT loc_6_A20E
        mov    ecx, dword ptr [ebp - 0ch]
        test   byte ptr [ecx + 14h], 6
        je     SHORT loc_6_A20E
        mov    dword ptr [ebp + 14h], 0
        mov    dword ptr [ebp - 14h], ecx
loc_6_A20E:
        mov    ecx, dword ptr [ebp - 0ch]
        sub    eax, eax
        mov    edi, dword ptr [ecx + 16h]
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        mov    eax, ecx
        jmp    SHORT loc_6_A225
loc_6_A223:
        xor    eax, eax
loc_6_A225:
        mov    ecx, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp - 10h]
        add    ecx, ebx
        mov    dword ptr [ebp - 18h], esi
        add    ecx, eax
        cmp    ecx, esi
        ja     SHORT loc_6_A2A0
        mov    si, word ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 10h]
        inc    si
        mov    edi, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 10h], si
        mov    dword ptr [ebp - 8], edi
        mov    edi, ebx
        mov    esi, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        shr    ecx, 2
        rep movsd
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 3
        rep movsb
        mov    esi, dword ptr [ebp + 0ch]
        cmp    word ptr [esi + 16h], 0
        jle    SHORT loc_6_A2A0
        mov    edi, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp + 10h]
        sub    edi, eax
        mov    dword ptr [ecx + 4], edi
        mov    ecx, dword ptr [ecx + 0ch]
        sub    ecx, eax
        mov    esi, ecx
        mov    dword ptr [ebp - 8], ecx
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 0ch], esi
        mov    ecx, dword ptr [ebp - 0ch]
        mov    esi, dword ptr [ecx + 16h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    esi, dword ptr [ebp - 8]
        mov    dword ptr [ebx + 16h], esi
loc_6_A2A0:
        mov    ecx, dword ptr [ebp + 10h]
        inc    word ptr [ecx + 12h]
loc_6_A2A7:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 4]
        add    dword ptr [ebp - 0ch], eax
        dec    dword ptr [ebp - 4]
        test   ecx, ecx
        jne    NEAR PTR loc_6_A18F
loc_6_A2BB:
        mov    eax, dword ptr [ebp - 14h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_A2D0:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        movzx  ax, byte ptr [ecx + 10h]
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 10h]
        mov    word ptr [ecx + 6], ax
        mov    ebp, esp
        push   edx
        mov    eax, dword ptr [ecx + 14h]
        push   1
        push   eax
        push   ecx
        call   sub_6_9F10
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_A300:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        xor    edx, edx
        sub    esp, 44h
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 30h]
        mov    dword ptr [ebp - 34h], edx
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [eax], edx
        mov    ecx, dword ptr [ebx + 0ch]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [ebp - 1ch], ecx
        mov    dword ptr [eax + 8], edx
        db  039h, 015h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 0ch], edx
        mov    dword ptr [eax + 10h], edx
        je     SHORT loc_6_A350
        mov    ecx, dword ptr [ebp + 0ch]
        pop    edi
        mov    word ptr [ecx + 18h], dx
        pop    esi
        mov    word ptr [ebx + 1ah], 5
        mov    word ptr [ecx + 10h], dx
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_A350:
        xor    edi, edi
        db  066h, 039h, 03Dh, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_6_A595
        db  039h, 03Dh, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_6_A595
        push   56h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A372:
        add    esp, 4
        mov    esi, eax
        cmp    esi, edi
        jne    SHORT loc_6_A393
        mov    word ptr [ebx + 1ah], 8
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 18h], di
        mov    word ptr [ecx + 10h], di
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_A393:
        cmp    dword ptr [ebp + 10h], 0
        mov    dword ptr [esi], 0
        je     SHORT loc_6_A3A7
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 4]
        jmp    SHORT loc_6_A3B4
loc_6_A3A7:
        push   3c04h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A3B1:
        add    esp, 4
loc_6_A3B4:
        push   10h
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A3C0:
        add    esp, 0ch
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_6_A3D0
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx]
        jmp    SHORT loc_6_A3DD
loc_6_A3D0:
        push   3c00h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A3DA:
        add    esp, 4
loc_6_A3DD:
        push   10h
        push   eax
        lea    eax, [ebp - 44h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A3E9:
        mov    ax, word ptr [ebp - 18h]
        add    esp, 0ch
        mov    word ptr [esi + 16h], ax
        mov    ecx, dword ptr [ebp - 16h]
        mov    dword ptr [esi + 12h], ecx
        mov    dx, word ptr [ebp - 12h]
        mov    word ptr [esi + 10h], dx
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [esi + 4], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_6_A499
        cmp    dword ptr [ebp + 10h], 0
        mov    eax, dword ptr [ebp - 0ch]
        jne    SHORT loc_6_A424
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A421:
        add    esp, 4
loc_6_A424:
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        cmp    byte ptr [eax], 0
        je     SHORT loc_6_A499
        mov    edi, eax
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edi, [ecx - 1]
        cmp    edi, 0fh
        jle    SHORT loc_6_A44C
        mov    word ptr [ebx + 1ah], 35h
        jmp    SHORT loc_6_A465
loc_6_A44C:
        lea    eax, [edi + 1]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A455:
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_6_A465
        mov    word ptr [ebx + 1ah], 8
loc_6_A465:
        xor    eax, eax
        cmp    word ptr [ebx + 1ah], ax
        je     SHORT loc_6_A488
        mov    ecx, dword ptr [ebp + 0ch]
        push   esi
        mov    word ptr [ecx + 18h], ax
        mov    word ptr [ecx + 10h], ax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A47E:
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_A488:
        push   edi
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A496:
        add    esp, 0ch
loc_6_A499:
        cmp    word ptr [esi + 16h], 1
        jle    SHORT loc_6_A4DF
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_6_A4BC
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, dword ptr [ebp - 4]
        je     SHORT loc_6_A4BC
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A4B9:
        add    esp, 4
loc_6_A4BC:
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 18h], ax
        push   esi
        mov    word ptr [ebx + 1ah], 7ch
        mov    word ptr [ecx + 10h], ax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A4D5:
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_A4DF:
        mov    eax, dword ptr [esi + 12h]
        test   eax, eax
        jne    SHORT loc_6_A4EC
        mov    word ptr [esi + 10h], 0
loc_6_A4EC:
        cmp    dword ptr [ebp + 10h], 0
        jne    SHORT loc_6_A4FB
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A4F8:
        add    esp, 4
loc_6_A4FB:
        lea    ecx, [ebp - 22h]
        mov    edx, dword ptr [ebp + 0ch]
        mov    word ptr [edx + 18h], 0
        mov    word ptr [edx + 10h], 0
        mov    dword ptr [ebp - 34h], eax
        lea    eax, [ebp - 24h]
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 0ch], eax
        lea    eax, [ebp - 44h]
        push   edx
        push   0
        mov    dword ptr [esi + 8], ecx
        mov    ecx, dword ptr [ebp - 1ch]
        push   esi
        push   ebx
        mov    dword ptr [esi + 18h], eax
        mov    dword ptr [ebx + 0ch], ecx
        call   sub_6_9F10
        mov    dx, word ptr [ebp - 24h]
        add    esp, 10h
        mov    ax, word ptr [ebp - 22h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 10h], dx
        mov    word ptr [ecx + 18h], ax
        cmp    dx, ax
        jae    SHORT loc_6_A562
        test   ax, ax
        je     SHORT loc_6_A55A
        mov    word ptr [ebx + 1ah], 0eah
        jmp    SHORT loc_6_A57C
loc_6_A55A:
        mov    word ptr [ebx + 1ah], 84bh
        jmp    SHORT loc_6_A57C
loc_6_A562:
        cmp    word ptr [ebx + 1ah], 0
        jne    SHORT loc_6_A57C
        mov    word ptr [ebx + 1ah], 0
        cmp    word ptr [ebx + 1ah], 0
        jne    SHORT loc_6_A57C
        mov    word ptr [ebx + 1ah], 17e6h
loc_6_A57C:
        mov    ecx, dword ptr [ebp + 0ch]
        push   esi
        mov    word ptr [ecx + 1ch], 0
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A58B:
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_A595:
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 18h], ax
        pop    edi
        mov    word ptr [ecx + 10h], ax
        pop    esi
        mov    word ptr [ebx + 1ah], 836h
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
sub_6_A5B0:
        push   ebp
        db  0B9h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 10h
        mov    eax, dword ptr [ecx]
        push   ebx
        push   esi
        lea    edx, [ebp - 10h]
        push   edi
        mov    ebx, dword ptr [ecx + 4]
        mov    esi, dword ptr [ecx + 8]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [edx + 4], ebx
        mov    ecx, 0ffffffffh
        mov    dword ptr [edx + 8], esi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [edx + 0ch], eax
        mov    edi, esi
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        jne    SHORT loc_6_A5F3
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_6_A63A
loc_6_A5F3:
        db  0BAh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx], eax
        cmp    ecx, 0fh
        mov    ebx, dword ptr [esi + 4]
        mov    dword ptr [edx + 4], ebx
        mov    edi, dword ptr [esi + 8]
        mov    dword ptr [edx + 8], edi
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [edx + 0ch], eax
        jge    SHORT loc_6_A633
        mov    edx, 0fh
        db  08Dh, 0B9h, 000h, 000h, 000h, 000h
        sub    edx, ecx
        mov    eax, 20202020h
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
loc_6_A633:
        db  0C6h, 005h, 00Fh, 000h, 000h, 000h, 003h
loc_6_A63A:
        call   sub_6_4220
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        test   ebx, ebx
        je     NEAR PTR loc_6_A6D8
loc_6_A64D:
        cmp    byte ptr [ebx + 2], 0
        jne    SHORT loc_6_A6CD
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A659:
        add    esp, 4
        mov    edi, eax
        cmp    byte ptr [esi], 0
        db  0C7h, 047h, 02Ch, 000h, 000h, 000h, 000h
        mov    dword ptr [edi + 4ch], 0
        db  0C7h, 047h, 040h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_A680
        push   edi
        call   sub_6_B400
        jmp    SHORT loc_6_A6CA
loc_6_A680:
        cmp    byte ptr [ebp - 10h], 0
        je     SHORT loc_6_A6C4
        lea    eax, [ebp - 10h]
        lea    ecx, [edi + 1ah]
        push   edi
        db  0FEh, 005h, 000h, 000h, 000h, 000h
        mov    byte ptr [edi], 0b1h
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], edx
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [ecx + 8], edx
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ecx + 0ch], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A6B1:
        add    esp, 4
        test   eax, eax
        db  0A0h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_A6CD
        dec    al
        db  0A2h, 000h, 000h, 000h, 000h
loc_6_A6C4:
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A6CA:
        add    esp, 4
loc_6_A6CD:
        mov    ebx, dword ptr [ebx + 18h]
        test   ebx, ebx
        jne    NEAR PTR loc_6_A64D
loc_6_A6D8:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 01Dh, 004h, 000h, 000h, 000h
        db  08Bh, 035h, 008h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  0A1h, 00Ch, 000h, 000h, 000h
        db  089h, 01Dh, 004h, 000h, 000h, 000h
        db  089h, 035h, 008h, 000h, 000h, 000h
        db  0B9h, 000h, 000h, 000h, 000h
        db  0A3h, 00Ch, 000h, 000h, 000h
        db  0BAh, 000h, 000h, 000h, 000h
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_A720:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        xor    eax, eax
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 14h]
        mov    ax, word ptr [ecx + 8]
        mov    dword ptr [ebp - 8], eax
        xor    eax, eax
        mov    dword ptr [ebp - 18h], eax
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp + 10h], eax
        mov    eax, dword ptr [ecx]
        jne    SHORT loc_6_A753
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A750:
        add    esp, 4
loc_6_A753:
        cmp    dword ptr [ebp - 8], 640h
        mov    dword ptr [ebp - 4], eax
        jg     NEAR PTR loc_6_A9FD
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    ebx, [ecx - 1]
        test   ebx, ebx
        je     NEAR PTR loc_6_A9FD
        cmp    ebx, 1
        jne    SHORT loc_6_A78D
        mov    eax, dword ptr [ebp - 4]
        cmp    byte ptr [eax], 2ah
        je     NEAR PTR loc_6_A9FD
loc_6_A78D:
        cmp    ebx, 2
        jne    SHORT loc_6_A7BC
        mov    eax, dword ptr [ebp - 4]
        cmp    word ptr [eax], 2a2ah
        jne    SHORT loc_6_A7BC
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        mov    dword ptr [ebp - 4], edi
        repne scasb
        not    ecx
        mov    dword ptr [ebp - 18h], 1
        lea    ebx, [ecx - 1]
        jmp    SHORT loc_6_A7D5
loc_6_A7BC:
        push   ebx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_6_AA10
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_6_A7D5
        mov    dword ptr [ebp - 18h], 1
        dec    ebx
loc_6_A7D5:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_A7F5
        xor    edi, edi
loc_6_A7E0:
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A7EA:
        add    esp, 4
        db  039h, 03Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_6_A7E0
loc_6_A7F5:
        push   0
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A802:
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_6_A817
        mov    dword ptr [ebp - 0ch], 8
        jmp    NEAR PTR loc_6_A9CA
loc_6_A817:
        db  0C7h, 047h, 028h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp + 10h], 0
        mov    dword ptr [edi + 2ch], esi
        je     SHORT loc_6_A832
        mov    eax, dword ptr [esi + 14h]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [edi + 8], ecx
        jmp    SHORT loc_6_A844
loc_6_A832:
        mov    ecx, dword ptr [esi + 14h]
        mov    edx, dword ptr [ecx + 4]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A83E:
        add    esp, 4
        mov    dword ptr [edi + 8], eax
loc_6_A844:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [esi]
        mov    edx, dword ptr [esi + 14h]
        mov    dword ptr [edi + 20h], ecx
        push   61h
        mov    dword ptr [ebp - 28h], eax
        push   esi
        mov    dword ptr [ebp - 14h], edx
        mov    byte ptr [edi + 1eh], 1
        mov    dword ptr [esi], ecx
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [esi + 14h], ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A869:
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_6_A9C3
        cmp    dword ptr [ebp - 18h], 0
        mov    eax, dword ptr [esi + 50h]
        mov    dword ptr [edi + 0ch], eax
        je     SHORT loc_6_A8AF
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 10h]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   ebx
        push   edx
        push   edi
        push   esi
        call   sub_6_AE00
        add    esp, 18h
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A8A7:
        add    esp, 4
        jmp    NEAR PTR loc_6_A9CA
loc_6_A8AF:
        lea    eax, [ebp - 24h]
        mov    ecx, dword ptr [ebp - 4]
        push   1
        mov    dword ptr [esi + 10h], eax
        push   3
        mov    dword ptr [esi + 2ch], ecx
        push   esi
        mov    dword ptr [esi], ebx
        call   sub_6_4670
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_6_A9C3
        push   ebx
        lea    eax, [ebp - 24h]
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        push   edi
        push   esi
        call   sub_6_ACD0
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_6_A9BA
        db  0A0h, 000h, 000h, 000h, 000h
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [edi + 1fh], al
        mov    dword ptr [esi + 14h], edi
        mov    dword ptr [ebp - 8], 0
        test   ebx, ebx
        je     NEAR PTR loc_6_A99D
loc_6_A90D:
        cmp    byte ptr [ebx + 2], 2
        jne    SHORT loc_6_A938
        push   0
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A91B:
        add    esp, 8
        mov    dword ptr [eax + 40h], edi
        test   byte ptr [edi + 1eh], 0ah
        jne    SHORT loc_6_A977
        lea    ecx, [ebp - 24h]
        push   ecx
        push   ebx
        push   eax
        push   edi
        call   sub_6_BDE0
        add    esp, 10h
        jmp    SHORT loc_6_A968
loc_6_A938:
        test   byte ptr [ebx + 60h], 1
        jne    SHORT loc_6_A94B
        push   1
        push   edi
        call   sub_6_AA60
        add    esp, 8
        jmp    SHORT loc_6_A96B
loc_6_A94B:
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A951:
        add    esp, 4
        test   byte ptr [edi + 1eh], 0ah
        jne    SHORT loc_6_A97F
        lea    ecx, [ebp - 24h]
        push   ecx
        push   edi
        push   eax
        call   sub_6_AD90
        add    esp, 0ch
loc_6_A968:
        inc    dword ptr [ebp - 10h]
loc_6_A96B:
        mov    ebx, dword ptr [ebx + 18h]
        inc    dword ptr [ebp - 8]
        test   ebx, ebx
        jne    SHORT loc_6_A90D
        jmp    SHORT loc_6_A99D
loc_6_A977:
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A97D:
        jmp    SHORT loc_6_A985
loc_6_A97F:
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A985:
        add    esp, 4
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        sub    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        call   sub_6_AA60
        add    esp, 8
loc_6_A99D:
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_6_A9CA
        cmp    dword ptr [esi + 14h], 0
        je     SHORT loc_6_A9CA
loc_6_A9A9:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A9AF:
        add    esp, 4
        cmp    dword ptr [esi + 14h], 0
        jne    SHORT loc_6_A9A9
        jmp    SHORT loc_6_A9CA
loc_6_A9BA:
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A9C0:
        add    esp, 4
loc_6_A9C3:
        movsx  eax, word ptr [esi + 1ah]
        mov    dword ptr [ebp - 0ch], eax
loc_6_A9CA:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_6_A9EB
        mov    ax, word ptr [ebp - 0ch]
        db  068h, 000h, 000h, 000h, 000h
        mov    word ptr [esi + 1ah], ax
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_A9E8:
        add    esp, 4
loc_6_A9EB:
        mov    eax, dword ptr [ebp - 28h]
        mov    ecx, dword ptr [ebp - 14h]
        pop    edi
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 14h], ecx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_A9FD:
        mov    word ptr [esi + 1ah], 57h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_6_AA10:
        push   ebp
        mov    ebp, esp
        sub    esp, 40h
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    esi, 20h
        jg     SHORT loc_6_AA4B
        test   esi, esi
        je     SHORT loc_6_AA4B
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   esi
        lea    ecx, [ebp - 40h]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_AA33:
        mov    ax, word ptr [ebp + esi*2 - 42h]
        add    esp, 10h
        sub    ax, 2ah
        pop    esi
        cmp    ax, 1
        mov    esp, ebp
        sbb    eax, eax
        pop    ebp
        inc    eax
        ret
loc_6_AA4B:
        mov    eax, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_AA60:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 0ch]
        sub    byte ptr [eax + 1fh], cl
        mov    cl, byte ptr [eax + 1eh]
        test   cl, 0ah
        je     SHORT loc_6_AA83
        test   cl, 4
        jne    SHORT loc_6_AA83
        push   0
        push   eax
        call   sub_6_B760
loc_6_AA83:
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_6_AA90:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 1ah], cx
        push   eax
        mov    dword ptr [eax + 14h], 0
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_AAAC:
        mov    esp, ebp
        pop    ebp
        ret
VREDIR_Service_07:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        xor    ecx, ecx
        sub    esp, 18h
        mov    al, byte ptr [ebp + 1ch]
        mov    dword ptr [ebp - 4], 0
        push   ebx
        db  08Ah, 088h, 000h, 000h, 000h, 000h
        push   esi
        push   edi
        db  08Bh, 004h, 08Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        test   byte ptr [eax + 60h], 1
        jne    SHORT loc_6_AAE9
        mov    eax, 5
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_AAE9:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_AAFE
        mov    eax, 5
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_AAFE:
        mov    ecx, dword ptr [ebp - 8]
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_AB0D:
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        je     NEAR PTR loc_6_AC21
        push   74h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_AB21:
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     NEAR PTR loc_6_AC21
        mov    eax, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ebx + 10h], esi
        mov    dword ptr [ebx + 2ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 28h], ecx
        mov    ecx, dword ptr [ebp + 0ch]
        push   61h
        mov    dword ptr [ebx + 8], eax
        mov    dword ptr [ebx + 20h], ecx
        push   esi
        mov    dword ptr [esi], ecx
        mov    byte ptr [ebx + 1eh], 1
        mov    dword ptr [esi + 14h], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_AB5A:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_6_AB6D
        movsx  eax, word ptr [esi + 1ah]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_6_AC28
loc_6_AB6D:
        mov    eax, dword ptr [esi + 50h]
        lea    ecx, [ebp - 18h]
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [ebx + 0ch], eax
        mov    eax, 20202020h
        mov    edi, edx
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx + 0ch], eax
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edi, [ebp - 18h]
        mov    esi, edx
        lea    eax, [ecx - 1]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        push   eax
        and    ecx, 3
        rep movsb
        mov    byte ptr [ebp - 9], 3
        lea    ecx, [ebp - 18h]
        push   ecx
        push   edx
        push   ebx
        push   0
        call   sub_6_ACD0
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_6_AC18
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ebx + 1fh], 1
        cmp    byte ptr [ecx + 2], 2
        jne    SHORT loc_6_ABF8
        push   1
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_ABDA:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_6_AC21
        lea    ecx, [ebp - 18h]
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        mov    dword ptr [eax + 40h], ebx
        push   edx
        push   eax
        push   ebx
        call   sub_6_BDE0
        add    esp, 10h
        jmp    SHORT loc_6_AC28
loc_6_ABF8:
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        call   sub_6_4170
        add    esp, 4
        test   eax, eax
        je     SHORT loc_6_AC21
        lea    ecx, [ebp - 18h]
        push   ecx
        push   ebx
        push   eax
        call   sub_6_AD90
        add    esp, 0ch
        jmp    SHORT loc_6_AC28
loc_6_AC18:
        mov    dword ptr [ebp - 4], 5
        jmp    SHORT loc_6_AC28
loc_6_AC21:
        mov    dword ptr [ebp - 4], 8
loc_6_AC28:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_6_AC4E
        test   ebx, ebx
        je     SHORT loc_6_AC3B
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_AC38:
        add    esp, 4
loc_6_AC3B:
        db  068h, 000h, 000h, 000h, 000h
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_AC4B:
        add    esp, 4
loc_6_AC4E:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_6_AC60:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, ebx
        je     SHORT loc_6_AC9A
        mov    eax, dword ptr [ebp + 0ch]
        push   esi
        mov    dword ptr [esi], ebx
        mov    dword ptr [esi + 2ch], eax
        call   sub_6_5980
        add    esp, 4
        test   eax, eax
        je     SHORT loc_6_AC8A
        mov    word ptr [esi + 1ah], bx
        jmp    SHORT loc_6_AC9A
loc_6_AC8A:
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    ebx, ecx
loc_6_AC9A:
        test   ebx, ebx
        jne    SHORT loc_6_ACC2
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        mov    edi, esi
        sub    eax, eax
        repne scasb
        not    ecx
        mov    ebx, ecx
        mov    edi, dword ptr [ebp + 0ch]
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
loc_6_ACC2:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_ACD0:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 4
        lea    eax, [eax + 78h]
        mov    ecx, 10h
        push   ebx
        mov    dword ptr [ebp - 4], eax
        push   esi
        mov    eax, dword ptr [ebp + 14h]
        push   edi
        db  0BFh, 000h, 000h, 000h, 000h
        mov    esi, eax
        repe cmpsb
        je     SHORT loc_6_AD6D
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        mov    esi, eax
        repe cmpsb
        je     SHORT loc_6_AD6D
        mov    eax, dword ptr [ebp - 4]
        lea    ebx, [eax + 3]
        mov    byte ptr [eax], 0
        inc    ebx
        mov    eax, dword ptr [ebp + 8]
        push   ebx
        push   eax
        mov    byte ptr [ebx - 1], 4
        call   sub_6_AC60
        add    esp, 8
        add    ebx, eax
        inc    ebx
        mov    eax, dword ptr [ebp + 18h]
        mov    edi, ebx
        mov    esi, dword ptr [ebp + 10h]
        mov    byte ptr [ebx - 1], 4
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        add    ebx, eax
        and    ecx, 3
        inc    ebx
        rep movsb
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ebx - 1], 0
        sub    ebx, dword ptr [ebp - 4]
        pop    edi
        sub    bx, 3
        pop    esi
        mov    dword ptr [ecx + 4], 0
        mov    dword ptr [ecx + 24h], 0
        mov    word ptr [eax + 1], bx
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_AD6D:
        mov    eax, dword ptr [ebp + 8]
        test   eax, eax
        je     SHORT loc_6_AD7A
        mov    word ptr [eax + 1ah], 5
loc_6_AD7A:
        mov    eax, 5
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_AD90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 10h]
        lea    ecx, [edi + 0ah]
        mov    dword ptr [edi + 4ch], esi
        mov    eax, dword ptr [edx]
        mov    byte ptr [edi], 90h
        db  0C7h, 047h, 02Ch, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [edx + 8]
        mov    edx, dword ptr [edx + 0ch]
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx + 0ch], edx
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [edi + 1ah]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 4]
        push   edi
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [ecx + 8], eax
        mov    edx, dword ptr [edx + 0ch]
        mov    dword ptr [ecx + 0ch], edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_ADE8:
        add    esp, 4
        test   eax, eax
        je     SHORT loc_6_ADFA
        push   eax
        push   esi
        push   edi
        call   sub_6_B320
        add    esp, 0ch
loc_6_ADFA:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Bh, 0FFh
sub_6_AE00:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        add    eax, 78h
        sub    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp + 1ch], 80h
        push   ebx
        push   esi
        push   edi
        jle    SHORT loc_6_AE2C
        mov    ebx, dword ptr [ebp + 8]
        pop    edi
        mov    word ptr [ebx + 1ah], 57h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_AE2C:
        mov    ecx, dword ptr [ebp - 8]
        mov    ebx, dword ptr [ebp + 8]
        push   ecx
        push   ebx
        call   sub_6_AC60
        add    esp, 8
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, eax
        mov    esi, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 14h]
        mov    edi, ecx
        mov    dword ptr [ebp - 4], ecx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebp + 1ch]
        push   edx
        add    dword ptr [ebp - 4], eax
        mov    ecx, dword ptr [ebp - 4]
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        inc    dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [ecx], 0
        mov    ecx, dword ptr [eax + 0ch]
        push   ecx
        push   edx
        call   sub_6_D6A0
        add    esp, 10h
        mov    eax, dword ptr [ebp + 1ch]
        add    eax, dword ptr [ebp - 4]
        cmp    byte ptr [eax - 1], 0
        je     SHORT loc_6_AE94
        mov    byte ptr [eax], 0
        inc    dword ptr [ebp + 1ch]
loc_6_AE94:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        add    eax, 58h
        mov    edx, dword ptr [ebp + 14h]
        push   1
        mov    dword ptr [ebx + 10h], eax
        push   0
        mov    dword ptr [ebx + 2ch], ecx
        push   ebx
        mov    dword ptr [ebx], edx
        call   sub_6_4670
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_6_AEF0
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 4]
        sub    eax, ecx
        mov    dword ptr [ebx + 14h], ecx
        db  0C7h, 043h, 00Ch, 000h, 000h, 000h, 000h
        mov    byte ptr [ebx + 4], 2
        mov    word ptr [ebx + 18h], 1
        mov    dword ptr [ebx + 28h], 0
        add    eax, dword ptr [ebp + 1ch]
        push   ebx
        mov    word ptr [ebx + 6], 0ffh
        mov    dword ptr [ebx], eax
        call   sub_6_44E0
        add    esp, 4
loc_6_AEF0:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_AF00:
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 001h
        push   esi
        push   edi
        xor    esi, esi
        db  0BFh, 004h, 000h, 000h, 000h
loc_6_AF10:
        db  039h, 035h, 000h, 000h, 000h, 000h
        jle    SHORT loc_6_AF38
        movsx  eax, word ptr [edi]
        movsx  ecx, word ptr [edi - 2]
        movsx  edx, word ptr [edi - 4]
        push   eax
        push   ecx
        push   edx
        call   sub_6_AF50
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_6_AF42
        add    edi, 6
        inc    esi
        jmp    SHORT loc_6_AF10
loc_6_AF38:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_6_AF42:
        pop    edi
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_AF50:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 8
        mov    word ptr [ebp - 6], ax
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ebp - 4], cx
        mov    edx, dword ptr [ebp + 10h]
        mov    word ptr [ebp - 2], dx
        push   0
        push   0
        lea    eax, [ebp - 8]
        mov    word ptr [ebp - 8], 0
        push   0
        push   8
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_AF88:
        add    esp, 18h
        cmp    eax, 2
        jne    SHORT loc_6_AFA0
        mov    eax, 1
        mov    esp, ebp
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        pop    ebp
        ret
loc_6_AFA0:
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_AFB0:
        push   ebp
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ebp, esp
        test   ecx, ecx
        push   ebx
        push   esi
        push   edi
        jne    SHORT loc_6_AFE0
        mov    ebx, dword ptr [ebp + 10h]
        mov    edi, dword ptr [ebp + 0ch]
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        push   edi
        push   esi
        call   sub_6_AF50
        add    esp, 0ch
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_6_B03C
        jmp    SHORT loc_6_AFE9
loc_6_AFE0:
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [ebp + 10h]
loc_6_AFE9:
        cmp    ecx, 8
        jl     SHORT loc_6_B01B
        db  066h, 083h, 03Dh, 02Ah, 000h, 000h, 000h, 003h
        jne    SHORT loc_6_B004
        db  066h, 0FFh, 005h, 02Ch, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_B004:
        db  066h, 0C7h, 005h, 02Ah, 000h, 000h, 000h, 003h, 000h
        db  066h, 0C7h, 005h, 02Ch, 000h, 000h, 000h, 002h, 000h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_B01B:
        lea    eax, [ecx + ecx*2]
        inc    ecx
        add    eax, eax
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        db  066h, 089h, 0B0h, 000h, 000h, 000h, 000h
        db  066h, 089h, 0B8h, 002h, 000h, 000h, 000h
        db  066h, 089h, 098h, 004h, 000h, 000h, 000h
loc_6_B03C:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_6_B050:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 44h]
        mov    cl, byte ptr [esi + 1]
        test   cl, cl
        je     SHORT loc_6_B0DC
        mov    dl, byte ptr [esi]
        cmp    dl, 92h
        je     SHORT loc_6_B0DC
        cmp    dl, 0b0h
        je     SHORT loc_6_B073
        cmp    dl, 0b1h
        jne    SHORT loc_6_B079
loc_6_B073:
        db  0FEh, 00Dh, 000h, 000h, 000h, 000h
loc_6_B079:
        xor    eax, eax
        mov    al, cl
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_6_0EE0
        add    esp, 8
        mov    edi, eax
        cmp    byte ptr [esi], 0b0h
        jne    SHORT loc_6_B0B0
        db  081h, 07Eh, 040h, 000h, 000h, 000h, 000h
        push   edi
        jne    SHORT loc_6_B0A6
        xor    eax, eax
        mov    al, byte ptr [esi + 30h]
        push   eax
        push   1
        jmp    SHORT loc_6_B0C6
loc_6_B0A6:
        xor    eax, eax
        mov    al, byte ptr [esi + 30h]
        push   eax
        push   2
        jmp    SHORT loc_6_B0C6
loc_6_B0B0:
        mov    eax, dword ptr [esi + 4ch]
        test   eax, eax
        je     SHORT loc_6_B0CE
        test   byte ptr [eax + 1eh], 20h
        je     SHORT loc_6_B0CE
        push   edi
        xor    eax, eax
        mov    al, byte ptr [esi + 30h]
        push   eax
        push   4
loc_6_B0C6:
        call   sub_6_AFB0
        add    esp, 0ch
loc_6_B0CE:
        push   edi
        push   esi
        call   sub_6_B8C0
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_6_B0DC:
        xor    ecx, ecx
        mov    cl, byte ptr [esi]
        sub    ecx, 91h
        cmp    ecx, 20h
        ja     NEAR PTR loc_6_B1C7
        xor    edx, edx
        db  08Ah, 091h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 095h, 000h, 000h, 000h, 000h
loc_6_B0FE:
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_B104:
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_6_B118
        push   esi
        call   sub_6_B3A0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_6_B118:
        xor    ecx, ecx
        mov    dl, byte ptr [esi + 2]
        mov    dword ptr [eax + 18h], esi
        mov    dword ptr [eax + 28h], ecx
        push   esi
        mov    byte ptr [eax + 1fh], cl
        mov    byte ptr [eax + 1ch], dl
        mov    dword ptr [esi + 4ch], eax
        or     byte ptr [eax + 1eh], 4
        call   sub_6_B500
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_6_B13D:
        push   esi
        call   sub_6_B480
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_6_B14A:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4]
        cmp    al, 0d6h
        je     SHORT loc_6_B165
        cmp    al, 0d0h
        je     SHORT loc_6_B165
        push   esi
        call   sub_6_B500
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_6_B165:
        push   0
        push   esi
        call   sub_6_B8C0
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_6_B174:
        xor    eax, eax
        mov    ecx, dword ptr [esi + 4ch]
        mov    ax, word ptr [esi + 8]
        push   eax
        push   ecx
        call   sub_6_B550
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_6_B18B:
        db  0FEh, 00Dh, 000h, 000h, 000h, 000h
        db  081h, 07Eh, 040h, 000h, 000h, 000h, 000h
        jne    SHORT loc_6_B19E
        or     byte ptr [eax + 60h], 1
loc_6_B19E:
        push   esi
        call   sub_6_B480
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_6_B1AB:
        db  081h, 07Eh, 040h, 000h, 000h, 000h, 000h
        jne    SHORT loc_6_B1B8
        and    byte ptr [eax + 60h], 0feh
loc_6_B1B8:
        push   esi
        db  0FEh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_6_B400
        add    esp, 4
loc_6_B1C7:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 006h
        db  002h, 003h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h
        db  006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 004h, 005h, 08Dh, 0A4h
        db  024h, 000h, 000h, 000h, 000h
loc_6_B210:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 44h]
        mov    edi, dword ptr [esi + 4ch]
        mov    al, byte ptr [esi + 1]
        test   al, al
        je     SHORT loc_6_B273
        cmp    byte ptr [esi], 92h
        je     SHORT loc_6_B273
        xor    ecx, ecx
        mov    cl, al
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        call   sub_6_0EE0
        add    esp, 8
        mov    ebx, eax
        cmp    byte ptr [esi], 90h
        push   ebx
        jne    SHORT loc_6_B254
        push   edi
        push   esi
        call   sub_6_B320
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_B254:
        xor    eax, eax
        mov    al, byte ptr [esi + 30h]
        push   eax
        push   4
        call   sub_6_AFB0
        add    esp, 0ch
        push   ebx
        push   esi
        call   sub_6_B8C0
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_B273:
        xor    eax, eax
        mov    al, byte ptr [esi]
        sub    eax, 90h
        cmp    eax, 5
        ja     SHORT loc_6_B2FD
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_6_B288:
        mov    al, byte ptr [edi + 1eh]
        dec    byte ptr [edi + 1fh]
        test   al, 0ah
        je     SHORT loc_6_B2B1
        push   esi
        call   sub_6_B3A0
        add    esp, 4
        test   byte ptr [edi + 1eh], 4
        jne    SHORT loc_6_B2FD
        push   0
        push   edi
        call   sub_6_B760
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_B2B1:
        mov    cl, byte ptr [esi + 2]
        or     al, 0ch
        push   edx
        mov    byte ptr [edi + 1ch], cl
        push   edi
        mov    byte ptr [edi + 1eh], al
        mov    dword ptr [edi + 18h], esi
        mov    dword ptr [edi + 14h], edx
        call   sub_6_B8F0
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_B2D1:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_B2D7:
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_B2DF:
        push   esi
        call   sub_6_B500
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_B2ED:
        xor    eax, eax
        mov    ax, word ptr [esi + 8]
        push   eax
        push   edi
        call   sub_6_BB10
        add    esp, 8
loc_6_B2FD:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 08Dh, 064h, 024h, 000h
sub_6_B320:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    cl, byte ptr [eax + 1fh]
        dec    cl
        mov    byte ptr [eax + 1fh], cl
        jne    SHORT loc_6_B350
        mov    dl, byte ptr [eax + 1eh]
        mov    ecx, 0
        test   dl, 8
        jne    SHORT loc_6_B341
        mov    ecx, dword ptr [ebp + 10h]
loc_6_B341:
        test   dl, 4
        jne    SHORT loc_6_B350
        push   ecx
        push   eax
        call   sub_6_B760
        add    esp, 8
loc_6_B350:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_B359:
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
sub_6_B360:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    cl, byte ptr [eax + 1fh]
        dec    cl
        mov    byte ptr [eax + 1fh], cl
        jne    SHORT loc_6_B38D
        mov    dl, byte ptr [eax + 1eh]
        mov    ecx, 0
        test   dl, 8
        jne    SHORT loc_6_B381
        mov    ecx, dword ptr [ebp + 0ch]
loc_6_B381:
        test   dl, 4
        jne    SHORT loc_6_B38D
        push   ecx
        push   eax
        call   sub_6_B760
loc_6_B38D:
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_B3A0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    byte ptr [esi], 92h
        mov    dword ptr [esi + 4ch], 0
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_B3B7:
        add    esp, 4
        test   eax, eax
        je     SHORT loc_6_B3F3
        db  081h, 07Eh, 02Ch, 000h, 000h, 000h, 000h
        jne    SHORT loc_6_B3D9
        cmp    byte ptr [esi + 1], 8
        jne    SHORT loc_6_B3D9
        push   esi
        call   sub_6_B480
        add    esp, 4
        pop    esi
        pop    ebp
        ret
loc_6_B3D9:
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 30h]
        push   eax
        push   4
        call   sub_6_AFB0
        add    esp, 0ch
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_B3F0:
        add    esp, 4
loc_6_B3F3:
        pop    esi
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_B400:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 40h]
        cmp    byte ptr [edx], 0
        je     SHORT loc_6_B468
        lea    ecx, [esi + 1ah]
        mov    eax, dword ptr [edx]
        db  0FEh, 005h, 000h, 000h, 000h, 000h
        mov    byte ptr [esi], 0b0h
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 4]
        push   esi
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [ecx + 8], eax
        mov    edx, dword ptr [edx + 0ch]
        mov    dword ptr [ecx + 0ch], edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_B437:
        add    esp, 4
        test   eax, eax
        je     SHORT loc_6_B471
        db  081h, 07Eh, 040h, 000h, 000h, 000h, 000h
        push   eax
        jne    SHORT loc_6_B452
        xor    eax, eax
        mov    al, byte ptr [esi + 30h]
        push   eax
        push   1
        jmp    SHORT loc_6_B45A
loc_6_B452:
        xor    eax, eax
        mov    al, byte ptr [esi + 30h]
        push   eax
        push   2
loc_6_B45A:
        call   sub_6_AFB0
        add    esp, 0ch
        db  0FEh, 00Dh, 000h, 000h, 000h, 000h
loc_6_B468:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_B46E:
        add    esp, 4
loc_6_B471:
        pop    esi
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_B480:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 44h]
        mov    edi, dword ptr [esi + 40h]
        cmp    byte ptr [edi], 0
        jne    SHORT loc_6_B4A0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_B499:
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_6_B4A0:
        lea    edx, [esi + 1ah]
        mov    eax, dword ptr [edi]
        mov    byte ptr [esi + 0ah], 2ah
        mov    byte ptr [esi], 91h
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [edi + 4]
        mov    cl, byte ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [edi + 8]
        push   esi
        mov    dword ptr [edx + 8], eax
        mov    edi, dword ptr [edi + 0ch]
        mov    dword ptr [edx + 0ch], edi
        mov    byte ptr [esi + 2ah], 0
        mov    byte ptr [esi + 2bh], cl
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_B4D0:
        add    esp, 4
        test   eax, eax
        je     SHORT loc_6_B4F1
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 30h]
        push   eax
        push   4
        call   sub_6_AFB0
        add    esp, 0ch
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_B4EE:
        add    esp, 4
loc_6_B4F1:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_B500:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [eax + 4ch]
        test   byte ptr [esi + 1eh], 2
        je     SHORT loc_6_B51F
        push   37h
        push   esi
        call   sub_6_B760
        add    esp, 8
        pop    esi
        pop    ebp
        ret
loc_6_B51F:
        lea    ecx, [esi + 58h]
        push   eax
        mov    byte ptr [eax], 95h
        mov    dword ptr [eax + 4], ecx
        mov    word ptr [eax + 8], 0c8h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_B534:
        add    esp, 4
        test   eax, eax
        je     SHORT loc_6_B545
        push   eax
        push   esi
        call   sub_6_B760
        add    esp, 8
loc_6_B545:
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_6_B550:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 10h], 0
        lea    eax, [ebx + 1eh]
        mov    cl, byte ptr [eax]
        test   cl, 2
        je     SHORT loc_6_B57F
        push   37h
        push   ebx
        call   sub_6_B760
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_B57F:
        or     cl, 20h
        cmp    dword ptr [ebp + 0ch], 23h
        mov    byte ptr [eax], cl
        jb     NEAR PTR loc_6_B72C
        cmp    dword ptr [ebx + 58h], 424d53ffh
        jne    NEAR PTR loc_6_B72C
        lea    ecx, [ebx + 78h]
        lea    eax, [ebx + 61h]
        lea    esi, [ebx + 5fh]
        mov    dword ptr [ebp - 4], ecx
        mov    word ptr [esi], 0
        lea    ecx, [ebx + 5dh]
        mov    dword ptr [ebp - 0ch], eax
        or     byte ptr [eax], 80h
        xor    eax, eax
        mov    dword ptr [ebp - 8], ecx
        mov    al, byte ptr [ebx + 5ch]
        mov    byte ptr [ecx], 0
        sub    eax, 0d0h
        cmp    eax, 7
        ja     SHORT loc_6_B5D9
        xor    ecx, ecx
        db  08Ah, 088h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_6_B5D9:
        push   3ah
        push   ebx
        call   sub_6_B760
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_B5EB:
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [ecx + 1]
        add    ecx, 3
        mov    dword ptr [ebx + 20h], eax
        mov    dword ptr [ebx + 4], ecx
loc_6_B5FD:
        mov    word ptr [esi], 54h
loc_6_B602:
        cmp    dword ptr [ebp - 10h], 0
        mov    ecx, dword ptr [ebp - 4]
        jne    NEAR PTR loc_6_B6F1
        lea    eax, [ecx + 1]
        mov    byte ptr [ecx], 0
        jmp    NEAR PTR loc_6_B700
loc_6_B61A:
        push   663h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_B624:
        add    esp, 4
        mov    edi, eax
        mov    dword ptr [ebx + 8], eax
        test   edi, edi
        jne    SHORT loc_6_B63D
        mov    word ptr [esi], 53h
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 2
        jmp    SHORT loc_6_B602
loc_6_B63D:
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [ecx + 1]
        mov    dword ptr [ebx + 4], edi
        lea    esi, [ecx + 3]
        mov    dword ptr [ebx + 20h], eax
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebx + 20h]
        mov    dword ptr [ebp - 10h], 1
        mov    byte ptr [edx + eax], 1
        db  0A0h, 000h, 000h, 000h, 000h
        add    dword ptr [ebx + 20h], 3
        mov    byte ptr [ebx + 1dh], al
        inc    al
        db  0A2h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_6_B602
loc_6_B686:
        xor    eax, eax
        xor    edx, edx
        mov    al, byte ptr [ebx + 1dh]
        mov    ecx, dword ptr [ebp - 4]
        mov    dx, word ptr [ecx + 1]
        cmp    eax, edx
        je     SHORT loc_6_B6A8
        mov    word ptr [esi], 5
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 2
        jmp    NEAR PTR loc_6_B602
loc_6_B6A8:
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [ecx + 3]
        mov    edi, dword ptr [ebx + 20h]
        sub    eax, 3
        mov    ecx, 663h
        sub    ecx, edi
        cmp    ecx, eax
        jae    SHORT loc_6_B6D2
        mov    word ptr [esi], 53h
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 2
        jmp    NEAR PTR loc_6_B602
loc_6_B6D2:
        mov    esi, dword ptr [ebp - 4]
        add    edi, dword ptr [ebx + 8]
        add    esi, 8
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        add    dword ptr [ebx + 20h], eax
        jmp    NEAR PTR loc_6_B602
loc_6_B6F1:
        movzx  ax, byte ptr [ebx + 1dh]
        mov    word ptr [ecx + 1], ax
        mov    byte ptr [ecx], 1
        lea    eax, [ecx + 3]
loc_6_B700:
        mov    word ptr [eax], 0
        mov    ecx, 0ffffffa8h
        sub    ecx, ebx
        add    cx, ax
        movzx  eax, cx
        add    eax, 2
        mov    ecx, dword ptr [ebp - 0ch]
        push   eax
        push   ebx
        or     byte ptr [ecx], 80h
        call   sub_6_B9D0
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_B72C:
        push   3ah
        push   ebx
        call   sub_6_B760
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 004h, 004h, 004h, 001h, 002h, 003h, 08Dh, 064h
        db  024h, 000h
sub_6_B760:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        test   byte ptr [esi + 1eh], 1
        jne    SHORT loc_6_B7D1
        test   edi, edi
        jne    SHORT loc_6_B7C1
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_B7C1
        mov    eax, dword ptr [esi + 20h]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_B78C:
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_6_B7C1
        push   0
        mov    eax, dword ptr [esi + 20h]
        sub    ax, 5
        push   0
        push   0
        push   eax
        mov    eax, dword ptr [esi + 4]
        add    eax, 5
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_B7B2:
        add    esp, 18h
        cmp    eax, 2
        jne    SHORT loc_6_B7C1
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
loc_6_B7C1:
        mov    eax, dword ptr [esi + 8]
        test   eax, eax
        je     SHORT loc_6_B7D1
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_B7CE:
        add    esp, 4
loc_6_B7D1:
        mov    eax, dword ptr [esi + 14h]
        test   eax, eax
        je     NEAR PTR loc_6_B875
        cmp    byte ptr [eax + 2], 2
        je     SHORT loc_6_B818
        mov    eax, dword ptr [esi + 18h]
        test   eax, eax
        je     NEAR PTR loc_6_B875
        mov    dword ptr [esi + 18h], 0
        mov    cl, byte ptr [esi + 1eh]
        test   cl, 4
        je     SHORT loc_6_B810
        and    cl, 0fbh
        mov    dl, byte ptr [esi + 1ch]
        push   eax
        mov    byte ptr [esi + 1eh], cl
        mov    byte ptr [eax + 2], dl
        call   sub_6_B3A0
        jmp    SHORT loc_6_B872
loc_6_B810:
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_B816:
        jmp    SHORT loc_6_B872
loc_6_B818:
        mov    al, byte ptr [esi + 1eh]
        test   al, 1
        je     SHORT loc_6_B865
        test   al, 4
        je     SHORT loc_6_B865
        and    al, 0fbh
        push   esi
        mov    byte ptr [esi + 1eh], al
        call   sub_6_C350
        add    esp, 4
        test   eax, eax
        je     SHORT loc_6_B865
        mov    byte ptr [esi + 78h], 0f5h
        and    byte ptr [esi + 61h], 7fh
        test   edi, edi
        je     SHORT loc_6_B84B
        mov    byte ptr [esi + 5dh], 1
        mov    word ptr [esi + 5fh], di
        jmp    SHORT loc_6_B852
loc_6_B84B:
        mov    dword ptr [esi + 5dh], 0
loc_6_B852:
        xor    ebx, ebx
        push   21h
        push   esi
        mov    dword ptr [eax + 40h], ebx
        call   sub_6_B9D0
        add    esp, 8
        mov    dword ptr [esi + 18h], ebx
loc_6_B865:
        mov    eax, dword ptr [esi + 18h]
        test   eax, eax
        je     SHORT loc_6_B875
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_B872:
        add    esp, 4
loc_6_B875:
        cmp    byte ptr [esi + 1fh], 0
        jne    SHORT loc_6_B8B5
        mov    ecx, dword ptr [esi + 28h]
        test   ecx, ecx
        je     SHORT loc_6_B88C
        push   edi
        mov    eax, dword ptr [esi + 2ch]
        push   eax
        call   ecx
        add    esp, 8
loc_6_B88C:
        mov    al, byte ptr [esi + 1eh]
        xor    ebx, ebx
        and    al, 1
        push   esi
        mov    bl, al
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_B89B:
        add    esp, 4
        test   ebx, ebx
        je     SHORT loc_6_B8B5
        db  068h, 000h, 000h, 000h, 000h
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_B8B2:
        add    esp, 4
loc_6_B8B5:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_6_B8C0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 4ch]
        test   ecx, ecx
        je     SHORT loc_6_B8DC
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   ecx
        call   sub_6_B760
        mov    esp, ebp
        pop    ebp
        ret
loc_6_B8DC:
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_B8E2:
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_B8F0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    edi, edi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 1eh], 2
        lea    ecx, [esi + 78h]
        je     SHORT loc_6_B914
        push   37h
        push   esi
        call   sub_6_B760
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_B914:
        mov    dword ptr [esi + 5dh], 0
        cmp    dword ptr [esi + 4], 0
        jne    SHORT loc_6_B951
        mov    dx, word ptr [ecx + 1]
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [esi + 4], eax
        movzx  eax, dx
        lea    ebx, [eax + ecx + 3]
        mov    eax, dword ptr [esi + 20h]
        cmp    eax, 80h
        jbe    SHORT loc_6_B942
        mov    byte ptr [esi + 5ch], 0d5h
        jmp    SHORT loc_6_B98C
loc_6_B942:
        mov    byte ptr [esi + 5ch], 0d0h
        mov    edi, eax
        lea    eax, [edx + eax]
        add    ax, 3
        jmp    SHORT loc_6_B988
loc_6_B951:
        lea    ebx, [ecx + 5]
        mov    edi, dword ptr [esi + 20h]
        sub    edi, dword ptr [esi + 24h]
        je     SHORT loc_6_B976
        mov    byte ptr [esi + 5ch], 0d7h
        cmp    edi, 80h
        jbe    SHORT loc_6_B96D
        mov    edi, 80h
loc_6_B96D:
        lea    eax, [edi + 3]
        mov    word ptr [ecx + 3], ax
        jmp    SHORT loc_6_B980
loc_6_B976:
        mov    byte ptr [esi + 5ch], 0d6h
        mov    word ptr [ecx + 3], 0
loc_6_B980:
        movzx  ax, byte ptr [esi + 1dh]
        mov    byte ptr [ecx], 1
loc_6_B988:
        mov    word ptr [ecx + 1], ax
loc_6_B98C:
        test   edi, edi
        je     SHORT loc_6_B9B1
        mov    byte ptr [ebx], 1
        inc    ebx
        mov    word ptr [ebx], di
        add    ebx, 2
        push   edi
        mov    eax, dword ptr [esi + 24h]
        push   eax
        mov    ecx, dword ptr [esi + 0ch]
        push   ecx
        push   ebx
        add    ebx, edi
        call   sub_6_D6A0
        add    esp, 10h
        add    dword ptr [esi + 24h], edi
loc_6_B9B1:
        and    byte ptr [esi + 61h], 7fh
        sub    ebx, esi
        sub    ebx, 58h
        push   ebx
        push   esi
        call   sub_6_B9D0
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_B9D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 14h]
        cmp    byte ptr [eax + 2], 2
        jne    NEAR PTR loc_6_BAAD
        mov    edi, dword ptr [esi + 18h]
        push   esi
        call   sub_6_BAE0
        add    esp, 4
        lea    eax, [edi + 44h]
        lea    ecx, [esi + 58h]
        mov    edx, dword ptr [ebp + 0ch]
        mov    word ptr [edi + 2eh], dx
        mov    dword ptr [edi + 24h], eax
        lea    edx, [edi + 1ch]
        mov    dword ptr [edi + 2ah], ecx
        mov    byte ptr [edi + 49h], 4
        mov    word ptr [edi + 28h], 1eh
        lea    ecx, [esi + 46h]
        mov    word ptr [edi + 22h], 2
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    cx, word ptr [ecx + 4]
        mov    word ptr [edx + 4], cx
        lea    edx, [esi + 4ch]
        lea    ecx, [edi + 4ah]
        mov    eax, dword ptr [edx]
        mov    byte ptr [edi + 62h], 0
        mov    ebx, dword ptr [edx + 4]
        mov    dword ptr [ecx], eax
        cmp    dword ptr [edi + 40h], 1
        mov    word ptr [edi + 0ah], 5405h
        mov    edx, dword ptr [edx + 8]
        push   edi
        push   3
        mov    dword ptr [ecx + 4], ebx
        sbb    ebx, ebx
        mov    dword ptr [ecx + 8], edx
        db  0C7h, 047h, 004h, 000h, 000h, 000h, 000h
        inc    ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_BA5D:
        add    esp, 8
        test   ebx, ebx
        je     SHORT loc_6_BAD7
        mov    al, byte ptr [esi + 1eh]
        mov    dword ptr [esi + 40h], edi
        or     al, 10h
        mov    byte ptr [esi + 1eh], al
        test   al, 1
        je     SHORT loc_6_BA90
        push   2710h
        add    esi, 30h
        push   esi
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_BA88:
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_BA90:
        push   9c40h
        add    esi, 30h
        push   esi
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_BAA5:
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_BAAD:
        mov    ecx, dword ptr [esi + 18h]
        lea    eax, [esi + 58h]
        mov    edx, dword ptr [ebp + 0ch]
        push   ecx
        mov    word ptr [ecx + 8], dx
        mov    dword ptr [ecx + 4], eax
        mov    byte ptr [ecx], 94h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_BAC6:
        add    esp, 4
        test   eax, eax
        je     SHORT loc_6_BAD7
        push   eax
        push   esi
        call   sub_6_B760
        add    esp, 8
loc_6_BAD7:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
sub_6_BAE0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    cl, byte ptr [eax + 1eh]
        test   cl, 10h
        je     SHORT loc_6_BAFE
        and    cl, 0efh
        add    eax, 30h
        push   eax
        mov    byte ptr [eax - 12h], cl
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_BAFE:
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_BB10:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        test   byte ptr [ecx + 1eh], 2
        push   esi
        mov    esi, dword ptr [ecx + 14h]
        je     SHORT loc_6_BB31
        push   37h
        push   ecx
        call   sub_6_B760
        add    esp, 8
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_BB31:
        cmp    dword ptr [ebp + 0ch], 23h
        jb     SHORT loc_6_BB94
        cmp    dword ptr [ecx + 58h], 424d53ffh
        jne    SHORT loc_6_BB94
        lea    eax, [ecx + 78h]
        xor    edx, edx
        mov    dl, byte ptr [ecx + 5ch]
        sub    edx, 0d0h
        cmp    edx, 7
        ja     SHORT loc_6_BB62
        xor    ebx, ebx
        db  08Ah, 09Ah, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 09Dh, 000h, 000h, 000h, 000h
loc_6_BB62:
        push   3ah
        push   ecx
        call   sub_6_B760
        add    esp, 8
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_BB71:
        push   0
        push   ecx
        call   sub_6_B760
        add    esp, 8
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_BB80:
        mov    al, byte ptr [eax + 1]
        mov    byte ptr [ecx + 1dh], al
loc_6_BB86:
        push   esi
        push   ecx
        call   sub_6_B8F0
        add    esp, 8
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_BB94:
        push   3ah
        push   ecx
        call   sub_6_B760
        add    esp, 8
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 004h, 004h, 004h, 004h, 001h, 002h, 003h
sub_6_BBC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        db  0BAh, 000h, 000h, 000h, 000h
        push   ebx
        mov    dword ptr [ebp - 4], 1
        push   esi
        mov    eax, dword ptr [edx]
        push   edi
        lea    esi, [ebp - 2ch]
        mov    ecx, esi
        mov    ebx, dword ptr [edx + 4]
        db  0C7h, 045h, 0F8h, 000h, 000h, 000h, 000h
        mov    edi, dword ptr [edx + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 0ch]
        mov    dword ptr [ecx + 4], ebx
        mov    dword ptr [ecx + 8], edi
        mov    dword ptr [ecx + 0ch], eax
        xor    ecx, ecx
        db  088h, 00Dh, 000h, 000h, 000h, 000h
        db  038h, 00Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_6_BC38
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 01Dh, 004h, 000h, 000h, 000h
        db  0BFh, 000h, 000h, 000h, 000h
        lea    edx, [ebp - 1ch]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [edi + 8]
        mov    edi, dword ptr [edi + 0ch]
        mov    dword ptr [edx + 4], ebx
        mov    dword ptr [edx + 8], eax
        db  088h, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], 2
        mov    dword ptr [edx + 0ch], edi
loc_6_BC38:
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_BC3D:
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 003h
        lea    edi, [eax + 1eh]
        jne    SHORT loc_6_BC69
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_BC69
loc_6_BC52:
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_BC57:
        cmp    eax, edi
        jae    SHORT loc_6_BC69
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_BC60:
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_6_BC52
loc_6_BC69:
        db  0A1h, 000h, 000h, 000h, 000h
        xor    ebx, ebx
        mov    edi, dword ptr [ebp + 8]
        test   eax, eax
        je     SHORT loc_6_BC87
loc_6_BC77:
        cmp    dword ptr [eax + 14h], edi
        jne    SHORT loc_6_BC87
        or     byte ptr [eax + 1eh], 2
        mov    eax, dword ptr [eax]
        inc    ebx
        test   eax, eax
        jne    SHORT loc_6_BC77
loc_6_BC87:
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_BC8C:
        add    eax, 1eh
        test   ebx, ebx
        mov    dword ptr [ebp - 0ch], eax
        jle    SHORT loc_6_BCC0
loc_6_BC96:
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_BC9B:
        cmp    eax, dword ptr [ebp - 0ch]
        jae    SHORT loc_6_BCC0
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_BCA5:
        xor    ecx, ecx
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, ecx
        je     SHORT loc_6_BCBC
loc_6_BCB0:
        cmp    dword ptr [eax + 14h], edi
        jne    SHORT loc_6_BCB6
        inc    ecx
loc_6_BCB6:
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    SHORT loc_6_BCB0
loc_6_BCBC:
        test   ecx, ecx
        jg     SHORT loc_6_BC96
loc_6_BCC0:
        cmp    byte ptr [edi + 2], 0
        jne    SHORT loc_6_BD38
        cmp    dword ptr [ebp - 4], 0
        jle    SHORT loc_6_BD38
loc_6_BCCC:
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_BCD2:
        lea    ecx, [eax + 1ah]
        mov    byte ptr [eax], 0b1h
        add    esp, 4
        mov    edx, dword ptr [esi + 4]
        db  0FEh, 005h, 000h, 000h, 000h, 000h
        mov    ebx, eax
        mov    eax, dword ptr [ebp - 8]
        push   ebx
        db  0C7h, 043h, 02Ch, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 4ch], 0
        mov    dword ptr [ebx + 40h], eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi + 8]
        mov    esi, dword ptr [esi + 0ch]
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx + 0ch], esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_BD13:
        add    esp, 4
        test   eax, eax
        je     SHORT loc_6_BD29
        push   ebx
        db  0FEh, 00Dh, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_BD26:
        add    esp, 4
loc_6_BD29:
        lea    esi, [ebp - 1ch]
        dec    dword ptr [ebp - 4]
        db  0C7h, 045h, 0F8h, 000h, 000h, 000h, 000h
        jne    SHORT loc_6_BCCC
loc_6_BD38:
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_BD3D:
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 003h
        lea    esi, [eax + 1eh]
        jne    SHORT loc_6_BD69
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_BD69
loc_6_BD52:
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_BD57:
        cmp    eax, esi
        jae    SHORT loc_6_BD69
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_BD60:
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_6_BD52
loc_6_BD69:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 01Dh, 004h, 000h, 000h, 000h
        db  08Bh, 035h, 008h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  0A1h, 00Ch, 000h, 000h, 000h
        db  089h, 01Dh, 004h, 000h, 000h, 000h
        db  089h, 035h, 008h, 000h, 000h, 000h
        db  0B9h, 000h, 000h, 000h, 000h
        db  0A3h, 00Ch, 000h, 000h, 000h
        db  0BAh, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 01Dh, 004h, 000h, 000h, 000h
        db  08Bh, 035h, 008h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  0A1h, 00Ch, 000h, 000h, 000h
        db  089h, 01Dh, 004h, 000h, 000h, 000h
        db  089h, 035h, 008h, 000h, 000h, 000h
        db  0B9h, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        db  0A3h, 00Ch, 000h, 000h, 000h
        db  0BAh, 000h, 000h, 000h, 000h
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
sub_6_BDE0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   88h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_BDF0:
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_6_BE0C
        push   8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_6_B360
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_BE0C:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 14h], esi
        db  0C7h, 046h, 010h, 000h, 000h, 000h, 000h
loc_6_BE1C:
        inc    ax
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        mov    word ptr [esi + 22h], ax
        je     SHORT loc_6_BE1C
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [esi], eax
        lea    eax, [esi + 26h]
        db  089h, 035h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 1ah], ecx
        mov    dword ptr [esi + 1eh], edx
        mov    edi, 0ffffffffh
        mov    dword ptr [ecx + 40h], esi
        mov    edx, 1
        mov    dword ptr [ecx + 24h], eax
        lea    eax, [ecx + 1ch]
        mov    word ptr [ecx + 28h], 62h
        mov    dword ptr [eax], edi
        lea    ebx, [esi + 30h]
        mov    word ptr [eax + 4], di
        mov    eax, 5405h
        mov    byte ptr [esi + 2bh], 4
        mov    dword ptr [esi + 2ch], 0
        mov    word ptr [ecx + 22h], dx
        mov    dword ptr [ebx], edi
        mov    word ptr [ecx + 0ah], ax
        mov    word ptr [ebx + 4], di
        mov    di, word ptr [esi + 22h]
        mov    byte ptr [esi + 65h], dl
        mov    word ptr [esi + 36h], ax
        lea    edx, [esi + 68h]
        mov    word ptr [esi + 66h], di
        mov    edi, dword ptr [ebp + 14h]
        mov    byte ptr [esi + 64h], 0f3h
        mov    eax, dword ptr [edi]
        mov    ebx, dword ptr [edi + 4]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [edi + 8]
        mov    edi, dword ptr [edi + 0ch]
        mov    dword ptr [edx + 4], ebx
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx + 0ch], edi
        db  0BFh, 000h, 000h, 000h, 000h
        lea    edx, [esi + 78h]
        mov    eax, dword ptr [edi]
        mov    dword ptr [edx], eax
        mov    ebx, dword ptr [edi + 4]
        push   ecx
        push   3
        mov    dword ptr [edx + 4], ebx
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [edx + 8], eax
        add    esi, 4
        mov    edi, dword ptr [edi + 0ch]
        mov    dword ptr [edx + 0ch], edi
        db  0C7h, 041h, 004h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_BEE5:
        add    esp, 8
        push   1f4h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_BEF3:
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_6_BF00:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        test   eax, eax
        push   esi
        push   edi
        je     SHORT loc_6_BF30
        mov    edx, dword ptr [ebp + 8]
loc_6_BF11:
        mov    cx, word ptr [edx + 30h]
        cmp    word ptr [eax + 6ah], cx
        jne    SHORT loc_6_BF2A
        lea    esi, [edx + 12h]
        lea    edi, [eax + 4ch]
        mov    ecx, 0ch
        repe cmpsb
        je     SHORT loc_6_BF30
loc_6_BF2A:
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    SHORT loc_6_BF11
loc_6_BF30:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_BF40:
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    edx, dword ptr [ebp + 18h]
        push   esi
        mov    dword ptr [ebp - 4], eax
        push   edi
        mov    ecx, dword ptr [edx]
        cmp    ecx, 1eh
        jae    SHORT loc_6_BF63
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_BF63:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 3eh]
        cmp    al, 0f1h
        jb     NEAR PTR loc_6_C168
        cmp    al, 0f3h
        jne    NEAR PTR loc_6_C006
        cmp    ecx, 62h
        jae    SHORT loc_6_BF87
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_BF87:
        mov    eax, dword ptr [ebp + 0ch]
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        lea    eax, [eax + 42h]
        mov    esi, eax
        repe cmpsb
        je     SHORT loc_6_BFB6
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        mov    esi, eax
        repe cmpsb
        je     SHORT loc_6_BFB6
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_BFB6:
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_BFC1:
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_6_BFD3
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_BFD3:
        push   62h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_BFDA:
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_6_BFF5
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_BFE9:
        add    esp, 4
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_BFF5:
        mov    dword ptr [esi + 40h], 0
        db  0BAh, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_6_C314
loc_6_C006:
        cmp    al, 0f4h
        jne    NEAR PTR loc_6_C0FF
        cmp    ecx, 62h
        jae    SHORT loc_6_C01C
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_C01C:
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        test   ebx, ebx
        je     SHORT loc_6_C039
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 40h]
loc_6_C02D:
        cmp    word ptr [ebx + 22h], ax
        je     SHORT loc_6_C042
        mov    ebx, dword ptr [ebx]
        test   ebx, ebx
        jne    SHORT loc_6_C02D
loc_6_C039:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_C042:
        mov    esi, dword ptr [ebp + 0ch]
        lea    edi, [ebx + 68h]
        mov    ecx, 10h
        lea    esi, [esi + 42h]
        repe cmpsb
        je     SHORT loc_6_C05D
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_C05D:
        mov    edi, dword ptr [ebx + 1eh]
        test   edi, edi
        jne    SHORT loc_6_C06D
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_C06D:
        test   byte ptr [edi + 1eh], 8
        je     SHORT loc_6_C080
        xor    eax, eax
        pop    edi
        mov    byte ptr [ebx + 24h], 4
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_C080:
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_C08B:
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_6_C09D
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_C09D:
        dec    byte ptr [edi + 1fh]
        mov    dword ptr [edi + 18h], esi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [esi + 40h], edi
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [edi + 14h], eax
        add    edx, 12h
        lea    ecx, [edi + 4ch]
        mov    byte ptr [ebx + 24h], 4
        mov    dword ptr [ebx + 1eh], 0
        mov    eax, dword ptr [edx]
        mov    ebx, dword ptr [edx + 4]
        mov    edx, dword ptr [edx + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 10h]
        add    edi, 46h
        mov    dword ptr [ecx + 4], ebx
        mov    dword ptr [ecx + 8], edx
        mov    ecx, dword ptr [eax]
        or     byte ptr [edi - 28h], 0ch
        mov    ax, word ptr [eax + 4]
        db  0BAh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], 0
        mov    word ptr [edi + 10h], 5405h
        mov    dword ptr [edi], ecx
        mov    word ptr [edi + 4], ax
        xor    edi, edi
        jmp    NEAR PTR loc_6_C314
loc_6_C0FF:
        cmp    al, 0f5h
        jne    SHORT loc_6_C15F
        cmp    ecx, 3eh
        jae    SHORT loc_6_C111
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_C111:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_6_BF00
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_6_C12C
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_C12C:
        push   ebx
        call   sub_6_C350
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_6_C144
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_C144:
        push   ebx
        xor    edi, edi
        call   sub_6_BAE0
        add    esp, 4
        db  0BAh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 5dh], edi
        mov    dword ptr [ebp - 4], edi
        jmp    NEAR PTR loc_6_C314
loc_6_C15F:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_C168:
        mov    esi, dword ptr [ebp + 0ch]
        add    esi, 1eh
        cmp    dword ptr [esi], 424d53ffh
        je     SHORT loc_6_C17F
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_C17F:
        mov    eax, dword ptr [ebp + 14h]
        sub    ecx, 1eh
        push   eax
        push   ecx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_C18D:
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_6_C19D
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_C19D:
        mov    eax, dword ptr [ebp + 0ch]
        test   byte ptr [eax + 27h], 80h
        jne    NEAR PTR loc_6_C267
        mov    al, byte ptr [eax + 22h]
        cmp    al, 0d0h
        je     SHORT loc_6_C1DB
        cmp    al, 0d5h
        je     SHORT loc_6_C1DB
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_6_BF00
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_6_C1D0
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_C1D0:
        mov    di, word ptr [ebx + 6ch]
        inc    di
        jmp    NEAR PTR loc_6_C286
loc_6_C1DB:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_6_BF00
        add    esp, 4
        test   eax, eax
        je     SHORT loc_6_C1F4
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_C1F4:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_C1FD:
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_6_C20F
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_C20F:
        mov    edx, dword ptr [ebp + 0ch]
        lea    ecx, [ebx + 4ch]
        mov    eax, dword ptr [edx + 12h]
        mov    edi, dword ptr [edx + 16h]
        add    edx, 12h
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 4], edi
        mov    edx, dword ptr [edx + 8]
        mov    dword ptr [ecx + 8], edx
        lea    edx, [ebx + 46h]
        mov    word ptr [ebx + 56h], 5405h
        mov    ecx, dword ptr [eax]
        mov    ax, word ptr [eax + 4]
        mov    dword ptr [edx], ecx
        mov    word ptr [edx + 4], ax
        lea    edi, [ebx + 58h]
        xor    eax, eax
        mov    ecx, 8
        mov    dword ptr [ebx + 18h], eax
        mov    dword ptr [ebx + 28h], eax
        or     byte ptr [ebx + 1eh], 4
        mov    byte ptr [ebx + 1fh], al
        rep movsd
        mov    di, word ptr [ebx + 6ch]
        lea    eax, [edi - 1]
        mov    word ptr [ebx + 6ch], ax
        jmp    SHORT loc_6_C286
loc_6_C267:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_6_BF00
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_6_C282
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_C282:
        mov    di, word ptr [ebx + 6ch]
loc_6_C286:
        test   ebx, ebx
        jne    SHORT loc_6_C293
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_C293:
        push   ebx
        call   sub_6_C350
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_6_C2AB
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_C2AB:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 32h]
        cmp    di, ax
        je     SHORT loc_6_C2DC
        test   byte ptr [ebx + 1eh], 1
        je     SHORT loc_6_C2C6
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_C2C6:
        cmp    word ptr [ebx + 6ch], ax
        je     SHORT loc_6_C2D5
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_C2D5:
        xor    edi, edi
        mov    dword ptr [ebp - 4], edi
        jmp    SHORT loc_6_C306
loc_6_C2DC:
        inc    word ptr [ebx + 6ch]
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 22h]
        mov    dx, word ptr [ecx + 25h]
        lea    edi, [ebx + 78h]
        mov    word ptr [ebx + 5fh], dx
        mov    byte ptr [ebx + 5ch], al
        mov    al, byte ptr [ecx + 23h]
        mov    byte ptr [ebx + 5dh], al
        mov    eax, dword ptr [ebp + 14h]
        sub    eax, 3eh
        mov    dword ptr [ebp - 4], eax
loc_6_C306:
        push   ebx
        call   sub_6_BAE0
        add    esp, 4
        db  0BAh, 000h, 000h, 000h, 000h
loc_6_C314:
        mov    bx, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 14h]
        mov    word ptr [esi + 0ah], 5405h
        mov    word ptr [esi + 28h], bx
        sub    ecx, dword ptr [ebp - 4]
        mov    word ptr [esi + 22h], 0
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [esi + 24h], edi
        mov    byte ptr [esi + 62h], 0
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi + 4], edx
        mov    word ptr [esi + 22h], 1
        sti
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
sub_6_C350:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 18h]
        test   eax, eax
        je     SHORT loc_6_C371
        cmp    byte ptr [eax + 8], cl
        je     SHORT loc_6_C36D
        mov    dword ptr [eax + 40h], ecx
        mov    dword ptr [esi + 18h], ecx
        xor    eax, eax
loc_6_C36D:
        test   eax, eax
        jne    SHORT loc_6_C38E
loc_6_C371:
        push   1
        mov    eax, dword ptr [esi + 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_C37C:
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_6_C388
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
loc_6_C388:
        mov    dword ptr [esi + 18h], eax
        mov    dword ptr [eax + 40h], esi
loc_6_C38E:
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
VRedir_Register:
        push   ebp
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        mov    dx, word ptr [esp + 8]
        mov    ebp, esp
        db  066h, 089h, 015h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_C3C1
        db  0A1h, 000h, 000h, 000h, 000h
        mov    word ptr [eax + 6], dx
loc_6_C3C1:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    word ptr [ecx + 16h], dx
        mov    word ptr [eax + 16h], dx
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        pop    ebp
        sub    eax, 55ab0000h
        mov    dword ptr [ecx + 1ch], eax
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
VRedir_MakeMailSlot:
        push   ebp
        mov    ecx, dword ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_C405:
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_6_C416
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_6_C416:
        mov    ecx, dword ptr [esi + 11h]
        mov    edx, dword ptr [ebp + 8]
        not    edx
        and    edx, ecx
        or     edx, dword ptr [ebp + 0ch]
        cmp    edx, ecx
        je     SHORT loc_6_C439
        mov    al, byte ptr [esi + 4]
        inc    al
        mov    byte ptr [esi + 4], al
        jne    SHORT loc_6_C436
        inc    al
        mov    byte ptr [esi + 4], al
loc_6_C436:
        mov    dword ptr [esi + 11h], edx
loc_6_C439:
        cmp    dword ptr [esi + 0ddh], 0
        jne    SHORT loc_6_C479
        push   esi
        lea    eax, [esi + 0d9h]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        push   esi
        call   sub_6_C660
        add    esp, 10h
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    byte ptr [eax + 46h], 0
        je     SHORT loc_6_C479
        push   esi
        lea    eax, [esi + 0f9h]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        push   esi
        call   sub_6_C660
        add    esp, 10h
loc_6_C479:
        cmp    dword ptr [esi + 5], 2
        jb     SHORT loc_6_C499
        mov    al, byte ptr [esi + 2]
        test   al, 40h
        jne    SHORT loc_6_C499
        or     al, 40h
        push   0ea60h
        push   esi
        mov    byte ptr [esi + 2], al
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_C496:
        add    esp, 8
loc_6_C499:
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
VREDIR_Service_06:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        cmp    edi, 0c2h
        jne    SHORT loc_6_C4BE
        xor    eax, eax
        pop    edi
        pop    esi
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        pop    ebx
        pop    ebp
        ret
loc_6_C4BE:
        push   0
        mov    bl, byte ptr [ebp + 8]
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_C4C9:
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_6_C4DC
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_C4DC:
        test   edi, edi
        je     SHORT loc_6_C4F3
        cmp    edi, 1
        je     SHORT loc_6_C503
        cmp    edi, 2
        je     NEAR PTR loc_6_C582
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_C4F3:
        push   esi
        call   sub_6_C9A0
        add    esp, 4
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_C503:
        mov    al, byte ptr [esi + 2]
        test   al, 18h
        je     SHORT loc_6_C514
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_C514:
        test   al, 4
        je     SHORT loc_6_C51F
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_C51F:
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 003h
        or     al, 4
        cmp    dword ptr [esi + 5], 4
        mov    byte ptr [esi + 2], al
        jbe    SHORT loc_6_C538
        mov    dword ptr [esi + 5], 0
loc_6_C538:
        mov    eax, dword ptr [esi + 19h]
        cmp    byte ptr [eax + 65h], 0ffh
        jne    SHORT loc_6_C565
        push   2
        push   esi
        call   sub_6_C7F0
        add    esp, 8
        test   eax, eax
        je     SHORT loc_6_C565
        mov    eax, 1
        pop    edi
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        and    byte ptr [esi + 2], 0fbh
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_C565:
        mov    eax, dword ptr [esi + 19h]
        cmp    byte ptr [eax + 2], 2
        je     SHORT loc_6_C57B
        xor    eax, eax
        mov    al, bl
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_C578:
        add    esp, 4
loc_6_C57B:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_C582:
        mov    al, byte ptr [esi + 2]
        pop    edi
        and    al, 18h
        pop    esi
        pop    ebx
        cmp    al, 1
        sbb    eax, eax
        pop    ebp
        neg    eax
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
VRedir_ServerEnum:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_C5B4:
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_6_C5C2
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret
loc_6_C5C2:
        mov    dword ptr [eax + 5], 0
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_C5D3:
        add    esp, 8
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        push   10h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_C5EF:
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_6_C603
        mov    eax, 8
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_6_C603:
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp + 10h]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_C610:
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_6_C622
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_6_C622:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   edi
        push   esi
        call   sub_6_C660
        add    esp, 10h
        mov    ecx, dword ptr [esi + 5]
        mov    dword ptr [ebp - 4], ecx
        mov    dword ptr [esi + 5], 0
        cmp    dword ptr [ebp - 4], 2
        jl     SHORT loc_6_C655
        push   0ea60h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_C652:
        add    esp, 8
loc_6_C655:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
sub_6_C660:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        cmp    dword ptr [edx + 9], 0
        mov    esi, dword ptr [ebp + 0ch]
        jne    SHORT loc_6_C677
        mov    dword ptr [edx + 9], esi
        mov    dword ptr [edx + 0dh], esi
loc_6_C677:
        mov    word ptr [esi], 6950h
        mov    eax, dword ptr [edx + 9]
        mov    ecx, dword ptr [eax + 4]
        add    eax, 4
        inc    byte ptr [edx + 3]
        test   byte ptr [edx + 2], 20h
        mov    dword ptr [esi + 4], ecx
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [eax], esi
        mov    dword ptr [esi + 0ch], ecx
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 8], eax
        jne    SHORT loc_6_C6B3
        mov    eax, dword ptr [edx + 19h]
        cmp    byte ptr [eax + 66h], 0ffh
        jne    SHORT loc_6_C6B3
        push   3
        push   edx
        call   sub_6_C7F0
        add    esp, 8
loc_6_C6B3:
        pop    esi
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_C6C0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 9]
        mov    edi, edx
        test   edi, edi
        je     SHORT loc_6_C704
        mov    ecx, dword ptr [ebp + 0ch]
loc_6_C6D4:
        mov    eax, dword ptr [edi + 4]
        cmp    ecx, eax
        je     SHORT loc_6_C6E5
        cmp    eax, edx
        mov    edi, eax
        jne    SHORT loc_6_C6D4
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_6_C6E5:
        cmp    edi, ecx
        jne    SHORT loc_6_C6F2
        mov    dword ptr [esi + 9], 0
        jmp    SHORT loc_6_C6FB
loc_6_C6F2:
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [esi + 9], eax
loc_6_C6FB:
        mov    eax, dword ptr [esi + 9]
        dec    byte ptr [esi + 3]
        mov    dword ptr [esi + 0dh], eax
loc_6_C704:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_6_C710:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_C721:
        add    esp, 4
        test   eax, eax
        je     SHORT loc_6_C739
        movsx  edx, byte ptr [edi + 3]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        imul   edx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 2], edx
loc_6_C739:
        push   0
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   0
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        push   ecx
        push   edx
        push   4
        push   edi
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_C755:
        add    esp, 20h
        mov    eax, dword ptr [edi + 19h]
        cmp    byte ptr [eax + 2], 2
        je     SHORT loc_6_C76F
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 5ah]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_C76C:
        add    esp, 4
loc_6_C76F:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_6_C780:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 19h]
        cmp    byte ptr [eax + 2], 2
        je     SHORT loc_6_C7E8
        mov    edi, dword ptr [ebp + 8]
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_C79A:
        add    esp, 4
        test   eax, eax
        je     SHORT loc_6_C7C9
        movsx  dx, byte ptr [esi + 3]
        imul   dx, word ptr [ebp + 10h]
        test   dx, dx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    word ptr [ecx + 8], dx
        jne    SHORT loc_6_C7C3
        mov    dword ptr [ecx + 2], 0
        jmp    SHORT loc_6_C7C9
loc_6_C7C3:
        mov    edx, dword ptr [esi + 11h]
        mov    dword ptr [ecx + 2], edx
loc_6_C7C9:
        push   0
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   0
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        push   ecx
        push   edx
        push   1
        push   esi
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_C7E5:
        add    esp, 20h
loc_6_C7E8:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
sub_6_C7F0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 2]
        test   al, 20h
        je     SHORT loc_6_C805
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_6_C805:
        or     al, 20h
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        mov    byte ptr [esi + 2], al
        mov    eax, dword ptr [esi + 19h]
        push   eax
        call   dword ptr [eax + 1ch]
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_6_C82B
        and    byte ptr [esi + 2], 0dfh
        push   esi
        call   sub_6_C9A0
        add    esp, 4
loc_6_C82B:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_C840:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    byte ptr [ebp - 1], 1
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 54h]
        test   esi, esi
        je     NEAR PTR loc_6_C921
        mov    al, byte ptr [esi + 2]
        mov    ebx, dword ptr [ebp + 0ch]
        and    al, 0dfh
        cmp    ebx, 3
        mov    byte ptr [esi + 2], al
        je     SHORT loc_6_C8B2
        cmp    dword ptr [ebp + 10h], 0
        jne    NEAR PTR loc_6_C918
        test   al, 18h
        jne    NEAR PTR loc_6_C918
        cmp    ebx, 2
        jne    SHORT loc_6_C896
        push   4
        push   esi
        call   sub_6_C7F0
        add    esp, 8
        mov    dword ptr [ebp + 10h], eax
        mov    byte ptr [ebp - 1], 5
        jmp    SHORT loc_6_C8B2
loc_6_C896:
        cmp    byte ptr [edi + 2], 2
        je     SHORT loc_6_C8B2
        push   esi
        lea    eax, [esi + 0e9h]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        push   esi
        call   sub_6_C660
        add    esp, 10h
loc_6_C8B2:
        cmp    dword ptr [ebp + 10h], 0
        jne    SHORT loc_6_C8E0
        lea    ecx, [edi + ebx]
        mov    al, byte ptr [ebp - 1]
        dec    byte ptr [ebp - 1]
        test   al, al
        mov    byte ptr [ecx + 68h], 0
        mov    byte ptr [ecx + 6dh], al
        je     SHORT loc_6_C8E0
loc_6_C8CC:
        push   ebx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_C8D3:
        add    esp, 8
        mov    al, byte ptr [ebp - 1]
        dec    byte ptr [ebp - 1]
        test   al, al
        jne    SHORT loc_6_C8CC
loc_6_C8E0:
        cmp    ebx, 3
        jne    SHORT loc_6_C8FC
        mov    eax, dword ptr [ebp + 10h]
        mov    cl, byte ptr [edi + 5ah]
        push   eax
        push   0
        push   ecx
        call   dword ptr [esi + 1dh]
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_C8FC:
        cmp    ebx, 4
        jne    SHORT loc_6_C921
        mov    eax, dword ptr [ebp + 10h]
        mov    cl, byte ptr [edi + 5ah]
        push   eax
        push   1
        push   ecx
        call   dword ptr [esi + 1dh]
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_C918:
        push   esi
        call   sub_6_C9A0
        add    esp, 4
loc_6_C921:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_6_C930:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        cmp    ecx, 2
        push   esi
        jl     SHORT loc_6_C96F
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 54h]
        test   esi, esi
        je     SHORT loc_6_C96F
        cmp    ecx, 2
        jne    SHORT loc_6_C962
        cmp    byte ptr [eax + 67h], 0ffh
        je     SHORT loc_6_C962
        push   84h
        push   eax
        call   dword ptr [eax + 20h]
        add    esp, 8
        test   eax, eax
        je     SHORT loc_6_C96F
loc_6_C962:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   esi
        call   sub_6_C980
        add    esp, 8
loc_6_C96F:
        pop    esi
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_C980:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    edx, dword ptr [ecx + 19h]
        and    byte ptr [ecx + 2], 0e7h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   2
        mov    al, byte ptr [edx + 5ah]
        push   eax
        call   dword ptr [ecx + 1dh]
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
sub_6_C9A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    byte ptr [ebp - 2], 0eh
        mov    byte ptr [ebp - 1], 1
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 19h]
        mov    al, byte ptr [esi + 2]
        or     al, 8
        mov    byte ptr [esi + 2], al
        test   al, 30h
        jne    SHORT loc_6_CA38
        or     al, 10h
        cmp    byte ptr [edi + 2], 2
        mov    byte ptr [esi + 2], al
        je     SHORT loc_6_C9DD
        lea    eax, [esi + 0e9h]
        push   eax
        push   esi
        call   sub_6_C6C0
        add    esp, 8
loc_6_C9DD:
        mov    al, byte ptr [esi + 2]
        test   al, 4
        je     SHORT loc_6_CA07
        and    al, 0fbh
        push   0
        push   0
        mov    cl, byte ptr [edi + 5ah]
        push   ecx
        lea    edx, [ebp - 2]
        push   2
        mov    byte ptr [esi + 2], al
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        push   edx
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 14h
loc_6_CA07:
        cmp    byte ptr [edi + 65h], 0ffh
        je     SHORT loc_6_CA2D
        push   82h
        push   edi
        call   dword ptr [edi + 20h]
        add    esp, 8
        test   eax, eax
        je     SHORT loc_6_CA38
        push   eax
        push   esi
        call   sub_6_C980
        add    esp, 8
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_6_CA2D:
        push   0
        push   esi
        call   sub_6_C980
        add    esp, 8
loc_6_CA38:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
sub_6_CA40:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        test   byte ptr [eax + 61h], 4
        push   esi
        push   edi
        mov    esi, 1
        jne    NEAR PTR loc_6_CADD
        mov    edi, dword ptr [eax + 54h]
        test   edi, edi
        je     SHORT loc_6_CAA0
        test   byte ptr [edi + 2], 4
        je     SHORT loc_6_CA6F
        push   edi
        call   sub_6_C9A0
        add    esp, 4
loc_6_CA6F:
        mov    dword ptr [edi + 15h], 0
        mov    al, byte ptr [edi + 2]
        test   al, 1
        je     SHORT loc_6_CA8E
        and    al, 0feh
        lea    ecx, [edi + 21h]
        push   ecx
        mov    byte ptr [edi + 2], al
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_CA8B:
        add    esp, 4
loc_6_CA8E:
        test   byte ptr [edi + 2], 2
        jne    SHORT loc_6_CAA0
        push   0
        lea    eax, [edi + 21h]
        push   eax
        call   dword ptr [edi + 2dh]
        add    esp, 8
loc_6_CAA0:
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_CAA5:
        lea    edi, [eax + 0fh]
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [eax + 54h]
        test   ebx, ebx
        je     SHORT loc_6_CAD6
        cmp    byte ptr [ebx + 3], 0
        jle    SHORT loc_6_CAD6
loc_6_CAB8:
        test   esi, esi
        je     SHORT loc_6_CAD6
        mov    esi, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_CAC6:
        cmp    eax, edi
        adc    esi, -1
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_CAD0:
        cmp    byte ptr [ebx + 3], 0
        jg     SHORT loc_6_CAB8
loc_6_CAD6:
        mov    eax, dword ptr [ebp + 8]
        or     byte ptr [eax + 61h], 4
loc_6_CADD:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
VREDIR_Service_08:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_CAFF:
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_6_CB6B
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    ebx, ecx
        mov    edi, dword ptr [ebp + 0ch]
        test   edi, edi
        jne    SHORT loc_6_CB26
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_CB26:
        lea    eax, [ebx + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_CB2F:
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_6_CB40
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_CB40:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 4], edi
        lea    edi, [eax + 0ch]
        mov    dword ptr [eax], ecx
        mov    ecx, ebx
        db  0A3h, 000h, 000h, 000h, 000h
        shr    ecx, 2
        mov    dword ptr [eax + 8], ebx
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_CB6B:
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_6_CB7B
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_CB7B:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        cmp    eax, edx
        jne    SHORT loc_6_CB93
        mov    eax, dword ptr [eax]
        pop    edi
        pop    esi
        db  0A3h, 000h, 000h, 000h, 000h
        xor    eax, eax
        pop    ebx
        pop    ebp
        ret
loc_6_CB93:
        test   edx, edx
        je     SHORT loc_6_CBA9
loc_6_CB97:
        mov    ecx, dword ptr [edx]
        cmp    eax, ecx
        je     SHORT loc_6_CBA5
        mov    edx, ecx
        test   edx, edx
        jne    SHORT loc_6_CB97
        jmp    SHORT loc_6_CBA9
loc_6_CBA5:
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
loc_6_CBA9:
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_CBAF:
        add    esp, 4
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_6_CBC0:
        push   ebp
        db  0A0h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        test   al, al
        push   esi
        push   edi
        jne    NEAR PTR loc_6_CC65
        inc    al
        xor    ecx, ecx
        mov    esi, dword ptr [ebp + 8]
        db  0A2h, 000h, 000h, 000h, 000h
        mov    cl, byte ptr [esi + 1]
        test   ecx, ecx
        jne    SHORT loc_6_CC5E
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        push   1
        push   0
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_6_CE30
        add    esp, 10h
        test   edi, edi
        je     SHORT loc_6_CC1D
loc_6_CC05:
        lea    ecx, [edi + 0ch]
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_6_CE30
        add    esp, 0ch
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    SHORT loc_6_CC05
loc_6_CC1D:
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_6_CE30
        add    esp, 8
        mov    dl, byte ptr [ebp + 10h]
        add    esi, 12h
        db  02Dh, 000h, 000h, 000h, 000h
        push   edx
        db  0FEh, 005h, 000h, 000h, 000h, 000h
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   VREDIR_Service_07
        add    esp, 18h
        test   eax, eax
        db  0A0h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_CC5E
        dec    al
loc_6_CC5E:
        dec    al
        db  0A2h, 000h, 000h, 000h, 000h
loc_6_CC65:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_6_CC70:
        push   ebp
        db  0A0h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        test   al, al
        push   ebx
        push   esi
        push   edi
        jne    NEAR PTR loc_6_CDA8
        inc    al
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0A2h, 000h, 000h, 000h, 000h
        call   sub_6_CE30
        mov    ecx, dword ptr [ebp + 8]
        add    esp, 8
        xor    edx, edx
        mov    dl, byte ptr [ecx + 1]
        test   edx, edx
        je     SHORT loc_6_CCC3
        cmp    edx, 1
        je     SHORT loc_6_CCE2
        push   edx
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_6_CE30
        add    esp, 0ch
        mov    edi, eax
        jmp    NEAR PTR loc_6_CD78
loc_6_CCC3:
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_6_CE30
        add    esp, 0ch
        mov    edi, eax
        jmp    NEAR PTR loc_6_CD78
loc_6_CCE2:
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_6_CE30
        add    esp, 0ch
        mov    edi, eax
        test   esi, esi
        je     SHORT loc_6_CD78
loc_6_CD06:
        push   0
        mov    al, byte ptr [esi + 5ah]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_CD11:
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_6_CD30
        xor    eax, eax
        mov    al, byte ptr [esi + 5ah]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        call   sub_6_CE30
        add    esp, 0ch
        jmp    SHORT loc_6_CD6F
loc_6_CD30:
        movsx  ecx, byte ptr [ebx + 3]
        mov    eax, dword ptr [ebx + 5]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebx + 4]
        push   eax
        push   ecx
        xor    eax, eax
        mov    al, byte ptr [ebx + 2]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 5ah]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        call   sub_6_CE30
        add    esp, 1ch
        mov    ecx, dword ptr [ebx + 15h]
        mov    edx, dword ptr [ebx + 11h]
        push   ecx
        push   edx
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_6_CE30
        add    esp, 10h
loc_6_CD6F:
        mov    edi, eax
        mov    esi, dword ptr [esi + 18h]
        test   esi, esi
        jne    SHORT loc_6_CD06
loc_6_CD78:
        mov    eax, dword ptr [ebp + 10h]
        db  081h, 0EFh, 000h, 000h, 000h, 000h
        push   eax
        push   0
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        add    eax, 12h
        push   eax
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        call   VREDIR_Service_07
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_6_CDA8
        db  0FEh, 00Dh, 000h, 000h, 000h, 000h
loc_6_CDA8:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 049h, 000h
loc_6_CDB0:
        db  0FEh, 00Dh, 000h, 000h, 000h, 000h
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
_PTEXT ends
        end
