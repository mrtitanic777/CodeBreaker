; vredir_pcode_06_04.asm -- VREDIR object 6 segment _PTEXT  (part 4/4)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_6_CDC0:
        push   ebp
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 10h]
loc_6_CDDC:
        inc    esi
        mov    eax, ecx
        sub    edx, edx
        dec    dword ptr [ebp + 14h]
        div    edi
        db  08Ah, 082h, 000h, 000h, 000h, 000h
        sub    edx, edx
        mov    byte ptr [esi], al
        mov    eax, ecx
        div    edi
        mov    ecx, eax
        test   ecx, ecx
        jne    SHORT loc_6_CDDC
        cmp    dword ptr [ebp + 14h], 0
        jle    SHORT loc_6_CE1A
        mov    edx, dword ptr [ebp + 14h]
        mov    edi, ebx
        mov    eax, 30303030h
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        add    ebx, edx
        and    ecx, 3
        rep stosb
loc_6_CE1A:
        mov    al, byte ptr [esi]
        inc    ebx
        dec    esi
        mov    byte ptr [ebx - 1], al
        cmp    byte ptr [esi], 0
        jne    SHORT loc_6_CE1A
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 049h, 000h
sub_6_CE30:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        mov    eax, dword ptr [ebp + 0ch]
        push   esi
        lea    ecx, [ebp + 10h]
        push   edi
        mov    dword ptr [ebp - 8], eax
        movsx  eax, byte ptr [eax]
        mov    dword ptr [ebp - 0ch], ecx
        test   eax, eax
        je     NEAR PTR loc_6_CFC3
loc_6_CE54:
        cmp    eax, 0ah
        jne    SHORT loc_6_CE66
        mov    byte ptr [edx], 0dh
        inc    edx
        mov    byte ptr [edx], 0ah
        inc    edx
        jmp    NEAR PTR loc_6_CFB2
loc_6_CE66:
        cmp    eax, 25h
        je     SHORT loc_6_CE73
        mov    byte ptr [edx], al
        inc    edx
        jmp    NEAR PTR loc_6_CFB2
loc_6_CE73:
        xor    ecx, ecx
        xor    ebx, ebx
        mov    esi, 0ah
        mov    eax, dword ptr [ebp - 8]
        inc    eax
        mov    byte ptr [ebp - 1], cl
        cmp    byte ptr [eax], 2dh
        jne    SHORT loc_6_CE8F
        mov    byte ptr [ebp - 1], 1
        mov    dword ptr [ebp - 8], eax
loc_6_CE8F:
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        movsx  edi, byte ptr [eax]
        cmp    edi, 30h
        jl     SHORT loc_6_CEB7
loc_6_CE9D:
        cmp    edi, 39h
        jg     SHORT loc_6_CEB7
        lea    eax, [ebx + ebx*4]
        inc    dword ptr [ebp - 8]
        lea    ebx, [edi + eax*2 - 30h]
        mov    eax, dword ptr [ebp - 8]
        movsx  edi, byte ptr [eax]
        cmp    edi, 30h
        jge    SHORT loc_6_CE9D
loc_6_CEB7:
        cmp    edi, 6ch
        jne    SHORT loc_6_CEC7
        mov    cl, 1
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        movsx  edi, byte ptr [eax]
loc_6_CEC7:
        sub    edi, 25h
        cmp    edi, 53h
        ja     SHORT loc_6_CEE9
        mov    dword ptr [ebp - 10h], 0
        db  08Ah, 087h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 10h], al
        mov    eax, dword ptr [ebp - 10h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_6_CEE9:
        mov    byte ptr [edx], 3fh
        inc    edx
        inc    edx
        mov    eax, dword ptr [ebp - 8]
        inc    edx
        mov    al, byte ptr [eax - 1]
        mov    byte ptr [edx - 2], 27h
        mov    byte ptr [edx - 1], al
        mov    byte ptr [edx], 27h
        inc    edx
        jmp    NEAR PTR loc_6_CFB2
loc_6_CF05:
        mov    byte ptr [edx], 25h
        inc    edx
        jmp    NEAR PTR loc_6_CFB2
loc_6_CF0E:
        add    dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 0ch]
        inc    edx
        mov    al, byte ptr [eax - 4]
        mov    byte ptr [edx - 1], al
        jmp    NEAR PTR loc_6_CFB2
loc_6_CF21:
        add    dword ptr [ebp - 0ch], 4
        push   4
        push   10h
        jmp    SHORT loc_6_CFA0
loc_6_CF2B:
        add    dword ptr [ebp - 0ch], 4
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, 0ffffffffh
        mov    esi, dword ptr [eax - 4]
        sub    eax, eax
        mov    edi, esi
        repne scasb
        not    ecx
        dec    ecx
        test   ebx, ebx
        je     SHORT loc_6_CF67
        cmp    ecx, ebx
        jl     SHORT loc_6_CF4F
        mov    ecx, ebx
        jmp    SHORT loc_6_CF67
loc_6_CF4F:
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_6_CF67
        mov    eax, ebx
        dec    ebx
        cmp    ecx, eax
        jge    SHORT loc_6_CF67
loc_6_CF5C:
        mov    byte ptr [edx], 20h
        inc    edx
        mov    eax, ebx
        dec    ebx
        cmp    ecx, eax
        jl     SHORT loc_6_CF5C
loc_6_CF67:
        xor    eax, eax
        test   ecx, ecx
        jle    SHORT loc_6_CF7F
        mov    eax, ecx
        mov    edi, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        add    edx, eax
        and    ecx, 3
        rep movsb
loc_6_CF7F:
        mov    ecx, eax
        inc    eax
        cmp    ebx, ecx
        jle    SHORT loc_6_CFB2
loc_6_CF86:
        mov    byte ptr [edx], 20h
        inc    edx
        mov    ecx, eax
        inc    eax
        cmp    ebx, ecx
        jg     SHORT loc_6_CF86
        jmp    SHORT loc_6_CFB2
loc_6_CF93:
        mov    esi, 10h
loc_6_CF98:
        test   cl, cl
        add    dword ptr [ebp - 0ch], 4
        push   ebx
        push   esi
loc_6_CFA0:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [eax - 4]
        push   ecx
        push   edx
        call   sub_6_CDC0
        add    esp, 10h
        mov    edx, eax
loc_6_CFB2:
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        movsx  eax, byte ptr [eax]
        test   eax, eax
        jne    NEAR PTR loc_6_CE54
loc_6_CFC3:
        mov    eax, edx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 006h, 006h, 006h
        db  006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h
        db  006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h
        db  006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h
        db  006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 001h, 002h, 006h, 006h, 006h, 006h
        db  006h, 006h, 006h, 006h, 006h, 006h, 006h, 003h, 006h, 006h, 004h, 006h, 006h, 006h, 006h, 005h
        db  08Dh, 064h, 024h, 000h
loc_6_D040:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 10h]
        mov    edi, dword ptr [esi + 72h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_D057:
        mov    byte ptr [esi + 18h], 1
        sub    eax, 1eh
        mov    dword ptr [ebp - 4], eax
        mov    ax, word ptr [esi + 1ah]
        and    ax, 0ff7fh
        test   byte ptr [edi + 0dh], 1
        mov    word ptr [esi + 1ah], ax
        je     NEAR PTR loc_6_D1C6
        test   al, 20h
        je     NEAR PTR loc_6_D1AE
        cmp    dword ptr [ebp - 4], 1000h
        jle    SHORT loc_6_D08F
        mov    dword ptr [ebp - 4], 0fe2h
loc_6_D08F:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_D098:
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        je     NEAR PTR loc_6_D196
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_D0AF:
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        test   eax, eax
        je     NEAR PTR loc_6_D189
        mov    cx, word ptr [ebx + 2]
        mov    eax, dword ptr [ebp - 4]
        sub    ax, 25h
        or     ch, 0c0h
        mov    word ptr [ebx + 2], cx
        mov    byte ptr [edi], 1
        mov    word ptr [edi + 3], ax
        mov    edx, dword ptr [ebp - 0ch]
        or     cl, 1
        mov    dword ptr [ebx + 28h], edx
        mov    byte ptr [ebx + 40h], 2bh
        mov    word ptr [edi + 1], 1
        mov    word ptr [ebx + 2], cx
        mov    ecx, dword ptr [ebx + 10h]
        mov    word ptr [ebx + 50h], 0
        mov    eax, dword ptr [ecx + 76h]
        test   eax, eax
        je     SHORT loc_6_D11A
        mov    edx, dword ptr [ebp - 4]
        sub    edx, 20h
        push   edx
        mov    edx, dword ptr [ebx + 38h]
        sub    edx, ebx
        sub    edx, 3ch
        push   edx
        push   ecx
        push   ebx
        call   dword ptr [eax + 30h]
        add    esp, 10h
        mov    dword ptr [ebp - 8], eax
        jmp    SHORT loc_6_D121
loc_6_D11A:
        mov    dword ptr [ebp - 8], 37h
loc_6_D121:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_6_D133
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_D12D:
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
loc_6_D133:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_D13C:
        add    esp, 4
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_D145:
        add    esp, 4
        test   byte ptr [esi + 1dh], 80h
        je     SHORT loc_6_D157
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_D157:
        mov    dword ptr [ebx + 20h], 0
        mov    dword ptr [ebx + 28h], 0
        and    word ptr [ebx + 2], 3fffh
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_6_D1C6
        mov    eax, 222h
        pop    edi
        mov    word ptr [esi + 1eh], 222h
        mov    word ptr [esi + 20h], 222h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_D189:
        test   edi, edi
        je     SHORT loc_6_D196
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_D193:
        add    esp, 4
loc_6_D196:
        mov    eax, 222h
        pop    edi
        mov    word ptr [esi + 1eh], 222h
        mov    word ptr [esi + 20h], 222h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_D1AE:
        mov    eax, 222h
        pop    edi
        mov    word ptr [esi + 1eh], 222h
        mov    word ptr [esi + 20h], 222h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_D1C6:
        xor    eax, eax
        mov    ax, word ptr [esi + 20h]
        cmp    eax, dword ptr [ebp - 4]
        jle    SHORT loc_6_D1D9
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [esi + 20h], ax
loc_6_D1D9:
        xor    eax, eax
        mov    ax, word ptr [esi + 1eh]
        cmp    eax, dword ptr [ebp - 4]
        jle    SHORT loc_6_D1EC
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [esi + 1eh], ax
loc_6_D1EC:
        xor    eax, eax
        pop    edi
        mov    ax, word ptr [esi + 1eh]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_6_D200:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   0
        push   eax
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_6_C930
        add    esp, 0ch
        xor    eax, eax
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_6_D220:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [edi + 10h]
        cmp    dword ptr [ebx + 22h], 0
        je     NEAR PTR loc_6_D318
        mov    esi, dword ptr [ebx + 1ah]
        mov    al, byte ptr [ebx + 28h]
        inc    al
        mov    byte ptr [ebx + 28h], al
        cmp    al, 4
        ja     SHORT loc_6_D2AA
        cmp    dword ptr [esi + 76h], 0
        jne    SHORT loc_6_D2AA
        mov    eax, dword ptr [ebx + 1eh]
        cmp    byte ptr [eax + 8], 0
        jne    SHORT loc_6_D297
        cmp    dword ptr [ebx + 31h], 0
        je     SHORT loc_6_D281
        lea    eax, [ebx + 35h]
        mov    edx, 0ffffffffh
        mov    dword ptr [ebx + 31h], 0
        mov    dword ptr [eax], edx
        mov    word ptr [eax + 4], dx
        mov    eax, dword ptr [ebx + 1eh]
        lea    ecx, [eax + 1ch]
        mov    dword ptr [ecx], edx
        mov    word ptr [ecx + 4], dx
        mov    byte ptr [eax + 62h], 5
        jmp    SHORT loc_6_D289
loc_6_D281:
        mov    byte ptr [ebx + 30h], 14h
        mov    byte ptr [eax + 62h], 4
loc_6_D289:
        mov    eax, dword ptr [ebx + 1eh]
        push   eax
        push   3
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_D294:
        add    esp, 8
loc_6_D297:
        push   1f4h
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_D2A2:
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_D2AA:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        db  0B9h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_D2CB
loc_6_D2B8:
        mov    eax, dword ptr [ecx]
        cmp    eax, ebx
        je     SHORT loc_6_D2C7
        cmp    dword ptr [eax], 0
        mov    ecx, eax
        jne    SHORT loc_6_D2B8
        jmp    SHORT loc_6_D2CB
loc_6_D2C7:
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
loc_6_D2CB:
        cli
        mov    eax, dword ptr [ebx + 1eh]
        cmp    byte ptr [eax + 8], 0
        je     SHORT loc_6_D2DE
        mov    dword ptr [eax + 40h], 0
        jmp    SHORT loc_6_D2E7
loc_6_D2DE:
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_D2E4:
        add    esp, 4
loc_6_D2E7:
        sti
        mov    eax, dword ptr [ebx + 22h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_D2F1:
        add    esp, 4
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_D2FA:
        add    esp, 4
        mov    al, byte ptr [ebx + 29h]
        not    al
        and    al, byte ptr [esi + 7bh]
        dec    byte ptr [esi + 2]
        mov    byte ptr [esi + 7bh], al
        test   al, al
        jne    SHORT loc_6_D318
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_D315:
        add    esp, 4
loc_6_D318:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 049h, 000h
loc_6_D320:
        xor    eax, eax
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_6_D330:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        mov    ecx, 0ffffffffh
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        add    edi, 2ch
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    dword ptr [ebp - 8], ecx
        cmp    ecx, 0fh
        jle    SHORT loc_6_D35F
        mov    eax, 35h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_D35F:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 58h]
        test   ax, ax
        je     SHORT loc_6_D375
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        movzx  eax, ax
        pop    ebp
        ret
loc_6_D375:
        push   20h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_D37C:
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     NEAR PTR loc_6_D545
        push   8dh
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_D393:
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        je     NEAR PTR loc_6_D538
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 14h], ebx
        db  0C7h, 043h, 010h, 000h, 000h, 000h, 000h
loc_6_D3B0:
        inc    ax
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        mov    word ptr [ebx + 26h], ax
        je     SHORT loc_6_D3B0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 22h], esi
        push   0
        mov    edx, dword ptr [ebp + 0ch]
        push   edx
        mov    dword ptr [ebx + 1ah], eax
        or     byte ptr [ebx + 27h], 80h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx], ecx
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_D3E4:
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        lea    ecx, [ebx + 2bh]
        mov    dword ptr [ebx + 1eh], eax
        mov    dword ptr [eax + 40h], ebx
        mov    dword ptr [eax + 24h], ecx
        mov    byte ptr [ebx + 30h], 4
        mov    dx, word ptr [ebx + 26h]
        mov    ecx, 1
        mov    word ptr [eax + 28h], 62h
        mov    byte ptr [ebx + 69h], 0f3h
        mov    word ptr [eax + 22h], cx
        mov    byte ptr [ebx + 6ah], cl
        mov    word ptr [ebx + 6bh], dx
        lea    ecx, [ebx + 6dh]
        mov    word ptr [eax + 0ah], 5205h
        mov    word ptr [ebx + 3bh], 5105h
        mov    eax, 20202020h
        mov    dword ptr [ecx], eax
        mov    edi, ecx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, eax
        shr    ecx, 2
        add    esi, 2ch
        rep movsd
        mov    ecx, eax
        db  0BAh, 000h, 000h, 000h, 000h
        and    ecx, 3
        rep movsb
        lea    ecx, [ebx + 7dh]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    esi, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], esi
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [ecx + 8], eax
        mov    edx, dword ptr [edx + 0ch]
        mov    dword ptr [ecx + 0ch], edx
        lea    ecx, [ebp - 1ch]
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        add    eax, 2ch
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        call   sub_6_D930
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_6_D4C4
        cmp    byte ptr [ebp - 1ch], 2
        jne    SHORT loc_6_D4C4
        cmp    dword ptr [ebp - 1bh], 0
        je     SHORT loc_6_D4C4
        lea    ecx, [ebp - 1bh]
        lea    edx, [ebx + 31h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        lea    eax, [ebp - 11h]
        mov    esi, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], esi
        mov    cx, word ptr [ecx + 8]
        mov    word ptr [edx + 8], cx
        mov    edx, dword ptr [ebp - 4]
        add    edx, 1ch
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
        jmp    SHORT loc_6_D4E6
loc_6_D4C4:
        lea    eax, [ebx + 35h]
        mov    ecx, 0ffffffffh
        mov    dword ptr [ebx + 31h], 0
        mov    dword ptr [eax], ecx
        mov    word ptr [eax + 4], cx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 1ch], ecx
        add    eax, 1ch
        mov    word ptr [eax + 4], cx
loc_6_D4E6:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [edx + 5eh]
        inc    byte ptr [eax + 2]
        push   1f4h
        or     byte ptr [eax + 7bh], cl
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [ebx + 29h], cl
        mov    cl, byte ptr [edx + 5fh]
        lea    edx, [ebx + 4]
        push   edx
        mov    byte ptr [ebx + 2ah], cl
        db  0C7h, 040h, 004h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_D516:
        add    esp, 8
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        push   3
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_D524:
        add    esp, 8
        cmp    al, 4
        jne    SHORT loc_6_D52F
        mov    byte ptr [ebx + 28h], 5
loc_6_D52F:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_D538:
        test   esi, esi
        je     SHORT loc_6_D545
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_D542:
        add    esp, 4
loc_6_D545:
        mov    eax, 45h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_D560:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 72h]
        movsx  edx, word ptr [ecx]
        mov    eax, dword ptr [ebp + 0ch]
        cmp    edx, eax
        jae    SHORT loc_6_D577
        mov    word ptr [ecx], ax
loc_6_D577:
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_D580:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 72h]
        add    eax, 14h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_D59A:
        add    esp, 8
        cmp    eax, -1
        je     SHORT loc_6_D5D9
        lea    ecx, [eax + eax*4]
        cmp    eax, -1
        lea    edx, [eax + ecx*2]
        lea    ecx, [edx + edx*4]
        mov    dword ptr [esi + 85h], ecx
        je     SHORT loc_6_D5D9
        cmp    eax, 5
        jle    SHORT loc_6_D5D9
        mov    eax, dword ptr [esi + 85h]
        mov    ecx, 28h
        add    eax, eax
        cdq
        idiv   ecx
        push   eax
        push   esi
        call   sub_6_D560
        or     byte ptr [esi + 1bh], 8
        add    esp, 8
loc_6_D5D9:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_6_D5E0:
        mov    eax, 37h
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_6_D5F0:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        cmp    ecx, 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 63h], cl
        jle    SHORT loc_6_D60F
        push   0
        push   ecx
        push   eax
        call   sub_6_C840
        add    esp, 0ch
loc_6_D60F:
        xor    eax, eax
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_D620:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    dword ptr [eax], 1000h
        mov    byte ptr [eax + 18h], 81h
        mov    byte ptr [eax + 19h], 0
        mov    ecx, dword ptr [ebp + 0ch]
        pop    ebp
        mov    dword ptr [eax + 4], ecx
        ret
        db  08Dh, 049h, 000h
sub_6_D640:
        push   ebp
        mov    edx, dword ptr [esp + 18h]
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 4]
        add    esi, dword ptr [ebp + 14h]
        mov    edi, dword ptr [ecx + 4]
        add    edi, dword ptr [ebp + 0ch]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  090h
sub_6_D670:
        push   ebp
        mov    edx, dword ptr [esp + 14h]
        mov    ebp, esp
        test   edx, edx
        push   esi
        push   edi
        je     SHORT loc_6_D697
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 10h]
        mov    ecx, edx
        mov    edi, dword ptr [eax + 4]
        shr    ecx, 2
        add    edi, dword ptr [ebp + 0ch]
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
loc_6_D697:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_6_D6A0:
        push   ebp
        mov    edx, dword ptr [esp + 14h]
        mov    ebp, esp
        test   edx, edx
        push   esi
        push   edi
        je     SHORT loc_6_D6C7
        mov    eax, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, edx
        mov    esi, dword ptr [eax + 4]
        shr    ecx, 2
        add    esi, dword ptr [ebp + 10h]
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
loc_6_D6C7:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 049h, 000h
sub_6_D6D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        cmp    byte ptr [esi + 2], 2
        mov    dword ptr [ebp - 8], edi
        je     NEAR PTR loc_6_D7C9
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_D6F0:
        cmp    word ptr [esi + 6], di
        lea    ebx, [eax + 1eh]
        jbe    SHORT loc_6_D729
loc_6_D6F9:
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_D6FE:
        mov    dword ptr [ebp - 4], eax
        cmp    eax, ebx
        jae    SHORT loc_6_D729
        mov    eax, dword ptr [esi + 8]
        lea    ecx, [edi + edi*4]
        shl    ecx, 4
        cmp    byte ptr [eax + ecx + 31h], 0ffh
        jne    SHORT loc_6_D71E
        xor    edi, edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_D71C:
        jmp    SHORT loc_6_D71F
loc_6_D71E:
        inc    edi
loc_6_D71F:
        xor    eax, eax
        mov    ax, word ptr [esi + 6]
        cmp    eax, edi
        jg     SHORT loc_6_D6F9
loc_6_D729:
        cmp    dword ptr [ebp - 4], ebx
        jb     SHORT loc_6_D788
        push   esi
        xor    edi, edi
        mov    dword ptr [ebp - 8], 1
        call   sub_6_4170
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_6_D788
        cmp    word ptr [esi + 6], di
        mov    dword ptr [ebp - 4], edi
        jbe    SHORT loc_6_D77F
loc_6_D74F:
        mov    eax, dword ptr [esi + 8]
        add    eax, edi
        cmp    dword ptr [eax + 44h], esi
        jne    SHORT loc_6_D76E
        cmp    byte ptr [eax + 31h], 0ffh
        jne    SHORT loc_6_D76E
        push   ebx
        mov    byte ptr [ebx], 35h
        mov    dword ptr [ebx + 4], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_D76B:
        add    esp, 4
loc_6_D76E:
        add    edi, 50h
        xor    eax, eax
        mov    ax, word ptr [esi + 6]
        inc    dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebp - 4]
        jg     SHORT loc_6_D74F
loc_6_D77F:
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_D785:
        add    esp, 4
loc_6_D788:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_6_D7C9
        xor    edi, edi
        db  089h, 035h, 000h, 000h, 000h, 000h
        cmp    word ptr [esi + 6], di
        jbe    SHORT loc_6_D7BF
loc_6_D79C:
        lea    eax, [edi + edi*4]
        mov    ecx, dword ptr [esi + 8]
        shl    eax, 4
        cmp    dword ptr [eax + ecx + 44h], esi
        jne    SHORT loc_6_D7B4
        xor    edi, edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_D7B2:
        jmp    SHORT loc_6_D7B5
loc_6_D7B4:
        inc    edi
loc_6_D7B5:
        xor    eax, eax
        mov    ax, word ptr [esi + 6]
        cmp    eax, edi
        jg     SHORT loc_6_D79C
loc_6_D7BF:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_6_D7C9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_6_D7D0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        mov    eax, dword ptr [eax + 1ch]
        push   edi
        test   eax, eax
        je     SHORT loc_6_D850
        mov    eax, dword ptr [eax + 10h]
        test   byte ptr [eax + 1bh], 2
        je     SHORT loc_6_D850
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 18h]
        mov    ax, word ptr [edx]
        mov    ebx, dword ptr [ebp + 10h]
        sub    ax, 2
        sub    ecx, ebx
        movzx  eax, ax
        add    ecx, 4bch
        cmp    ecx, eax
        jb     SHORT loc_6_D844
        lea    esi, [edx + 4]
        mov    edi, ebx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        cmp    word ptr [ebx], 0
        je     SHORT loc_6_D835
loc_6_D822:
        mov    ax, word ptr [ebx]
        movzx  ecx, ax
        mov    word ptr [ebx], 5ch
        add    ebx, ecx
        cmp    word ptr [ebx], 0
        jne    SHORT loc_6_D822
loc_6_D835:
        mov    dx, word ptr [edx]
        pop    edi
        sub    dx, 2
        pop    esi
        movzx  eax, dx
        pop    ebx
        pop    ebp
        ret
loc_6_D844:
        mov    dx, 0ffffh
        pop    edi
        movzx  eax, dx
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_D850:
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 4
        mov    ebx, dword ptr [ebp + 10h]
        push   edx
        db  0BEh, 000h, 000h, 000h, 000h
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_D86B:
        add    esp, 10h
        lea    edx, [eax + 1]
        movzx  eax, dx
        mov    ecx, eax
        mov    edi, ebx
        shr    ecx, 2
        repe cmpsd
        jne    SHORT loc_6_D8A5
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb
        jne    SHORT loc_6_D8A5
        cmp    dword ptr [ebp + 1ch], 0
        je     SHORT loc_6_D8A5
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        inc    dx
        mov    word ptr [edi - 1], ax
loc_6_D8A5:
        movzx  eax, dx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 049h, 000h
sub_6_D8B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        db  0C7h, 045h, 0ECh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 10h], 0
        mov    dword ptr [ebp - 0ch], 1eh
        db  0C7h, 045h, 0F8h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 014h, 000h, 08Bh, 004h
        add    esp, 10h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_6_D8FA
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_6_D8FA:
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
sub_6_D900:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_D927
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0CDh, 020h, 015h, 000h, 08Bh, 004h
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
loc_6_D927:
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_6_D930:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_D989
        mov    eax, dword ptr [ebp + 10h]
        lea    ecx, [ebp - 1ch]
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 14h], ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 10h], edx
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 1ch], 11h
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 017h, 000h, 08Bh, 004h
        add    esp, 14h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret
loc_6_D989:
        mov    eax, 1
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_D9A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_D9F2
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 18h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 10h], ecx
        mov    dword ptr [ebp - 0ch], edx
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 14h], 11h
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 018h, 000h, 08Bh, 004h
        add    esp, 14h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret
loc_6_D9F2:
        mov    eax, 1
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
_PTEXT ends
        end
