; vredir_pcode_05.asm -- VREDIR object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_5_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
loc_5_000C:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0011:
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 003h
        mov    dword ptr [ebp - 4], eax
        jne    NEAR PTR loc_5_012A
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        test   edi, edi
        je     NEAR PTR loc_5_012A
        and    eax, 0fh
        mov    dword ptr [ebp - 8], eax
loc_5_0035:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_5_0048
        and    byte ptr [edi + 1dh], 0f7h
        mov    word ptr [edi + 8dh], 0
loc_5_0048:
        mov    dword ptr [esi + 1ch], 0
        mov    ebx, dword ptr [edi + 0ch]
        test   ebx, ebx
        jne    SHORT loc_5_0077
        cmp    byte ptr [edi + 2], 0
        jne    NEAR PTR loc_5_011B
        cmp    byte ptr [edi + 2ch], 0
        je     NEAR PTR loc_5_011B
        push   edi
        push   0
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0072:
        add    esp, 8
        jmp    SHORT loc_5_000C
loc_5_0077:
        mov    ax, word ptr [ebx + 1ch]
        test   al, 80h
        jne    SHORT loc_5_0082
        mov    dword ptr [esi + 1ch], ebx
loc_5_0082:
        test   al, 8
        je     SHORT loc_5_00B6
        cmp    word ptr [ebx + 7bh], 0
        je     SHORT loc_5_00B6
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 28h], eax
        ja     SHORT loc_5_00B6
        push   esi
        mov    dword ptr [esi + 1ch], ebx
        mov    word ptr [esi + 1ah], 0
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_00A4:
        add    esp, 4
        add    eax, 3
        cmp    eax, 3
        ja     SHORT loc_5_00B6
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_5_00B6:
        mov    ebx, dword ptr [ebx + 4]
        cmp    dword ptr [edi + 0ch], ebx
        jne    SHORT loc_5_0077
        mov    al, byte ptr [edi + 1dh]
        and    al, 10h
        shr    al, 4
        mov    byte ptr [esi + 4], al
        test   al, al
        jne    SHORT loc_5_00D3
        cmp    byte ptr [edi + 3], 0
        jne    SHORT loc_5_011B
loc_5_00D3:
        mov    edx, dword ptr [edi + 76h]
        test   edx, edx
        je     SHORT loc_5_011B
        cmp    dword ptr [esi + 1ch], 0
        je     SHORT loc_5_011B
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, 0ffffh
        mov    byte ptr [esi + 5], 0
        mov    dword ptr [esi + 28h], eax
loc_5_00EF:
        mov    al, byte ptr [esi + 5]
        xor    ecx, ecx
        mov    cl, al
        cmp    word ptr [edi + ecx*2 + 7fh], bx
        jne    SHORT loc_5_0106
        inc    al
        mov    byte ptr [esi + 5], al
        cmp    al, 3
        jb     SHORT loc_5_00EF
loc_5_0106:
        push   esi
        call   dword ptr [edx + 40h]
        add    esp, 4
        cmp    eax, -3
        je     SHORT loc_5_0173
        cmp    eax, -1
        je     NEAR PTR loc_5_000C
loc_5_011B:
        mov    edi, dword ptr [edi + 4]
        db  03Bh, 03Dh, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_5_0035
loc_5_012A:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0130:
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   3e8h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0141:
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_014B:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0151:
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_015B:
        push   3e8h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0169:
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_0173:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0179:
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_01A0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    edx, dword ptr [esp + 8]
        mov    ecx, edx
        add    edx, 2
        cmp    word ptr [ecx], ax
        je     SHORT loc_5_01BF
loc_5_01B3:
        inc    eax
        mov    ecx, edx
        add    edx, 2
        cmp    word ptr [ecx], 0
        jne    SHORT loc_5_01B3
loc_5_01BF:
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_01D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        mov    dword ptr [ebp - 4], 1
loc_5_01DD:
        push   1
        lea    eax, [ebp - 4]
        push   1
        lea    ecx, [ebp - 0ch]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_01EE:
        add    esp, 10h
        mov    dword ptr [ebp - 8], edx
        test   byte ptr [ebp - 8], 2
        je     SHORT loc_5_0204
        mov    eax, dword ptr [ebp - 4]
        db  0C6h, 080h, 000h, 000h, 000h, 000h, 001h
loc_5_0204:
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 100h
        jl     SHORT loc_5_01DD
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_0220:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    cx, word ptr [edx + 2]
        mov    ebp, esp
        sub    esp, 8
        movzx  eax, cx
        push   ebx
        push   esi
        push   edi
        lea    ebx, [eax + edx]
        mov    ax, word ptr [ebx]
        mov    esi, dword ptr [ebp + 0ch]
        add    cx, ax
        movzx  eax, ax
        mov    word ptr [edx + 2], cx
        add    ebx, eax
        test   byte ptr [ebp + 10h], 2
        je     SHORT loc_5_0272
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        push   1
        repne scasb
        not    ecx
        dec    ecx
        lea    eax, [ebx + 2]
        push   ecx
        push   esi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_026A:
        add    esp, 10h
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_5_0293
loc_5_0272:
        push   esi
        lea    edi, [ebx + 2]
        call   sub_5_01A0
        add    esp, 4
        add    eax, eax
        mov    ecx, eax
        mov    edx, eax
        shr    ecx, 2
        mov    dword ptr [ebp - 4], eax
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
loc_5_0293:
        test   byte ptr [ebp + 10h], 1
        je     SHORT loc_5_02AA
        mov    eax, dword ptr [ebp - 4]
        lea    ecx, [ebx + 2]
        push   eax
        push   ecx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_02A7:
        add    esp, 0ch
loc_5_02AA:
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        add    ax, 2
        pop    edi
        mov    word ptr [ebx], ax
        pop    esi
        add    word ptr [edx], ax
        xor    eax, eax
        mov    ax, word ptr [ebx]
        mov    word ptr [ebx + eax], 0
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
sub_5_02D0:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi], 4
        mov    al, byte ptr [edx]
        mov    word ptr [esi + 2], 4
        mov    word ptr [esi + 4], 0
        cmp    al, 5ch
        jne    SHORT loc_5_0301
loc_5_02F6:
        inc    edi
        cmp    byte ptr [edi], 5ch
        je     SHORT loc_5_02F6
        cmp    al, 5ch
        jne    SHORT loc_5_0301
        inc    edx
loc_5_0301:
        cmp    byte ptr [edi], 0
        je     SHORT loc_5_0336
        xor    ebx, ebx
loc_5_0308:
        mov    al, byte ptr [edi]
        xor    ecx, ecx
        mov    cl, al
        db  038h, 099h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_0319
        inc    edi
        jmp    SHORT loc_5_0331
loc_5_0319:
        cmp    al, 5ch
        jne    SHORT loc_5_0331
        push   3
        mov    byte ptr [edi], bl
        push   edx
        push   esi
        call   sub_5_0220
        add    esp, 0ch
        lea    edx, [edi + 1]
        mov    byte ptr [edi], 5ch
loc_5_0331:
        inc    edi
        cmp    byte ptr [edi], bl
        jne    SHORT loc_5_0308
loc_5_0336:
        push   3
        push   edx
        push   esi
        call   sub_5_0220
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_0350:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        mov    ax, word ptr [edx + 2]
        mov    word ptr [edx + 2], 4
        add    eax, edx
        mov    cx, word ptr [eax]
        sub    word ptr [edx], cx
        lea    ecx, [edx + 4]
        mov    word ptr [eax], 0
        cmp    word ptr [ecx], 0
        je     SHORT loc_5_038D
loc_5_0379:
        mov    ax, word ptr [ecx]
        movzx  esi, ax
        add    ecx, esi
        je     SHORT loc_5_0387
        add    word ptr [edx + 2], ax
loc_5_0387:
        cmp    word ptr [ecx], 0
        jne    SHORT loc_5_0379
loc_5_038D:
        pop    esi
        pop    ebp
        ret
loc_5_0390:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 48h]
        test   edi, edi
        jne    SHORT loc_5_03C7
        mov    al, byte ptr [esi + 1]
        test   al, al
        je     NEAR PTR loc_5_0442
        sub    al, 6
        cmp    al, 1
        mov    ecx, dword ptr [esi + 44h]
        sbb    eax, eax
        neg    eax
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 2]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_03C2:
        add    esp, 0ch
        jmp    SHORT loc_5_0442
loc_5_03C7:
        cmp    byte ptr [esi], 95h
        jne    SHORT loc_5_0417
        mov    ax, word ptr [esi + 8]
        mov    edx, dword ptr [edi + 1ch]
        movzx  ecx, ax
        cmp    ax, 33h
        mov    dword ptr [edx + 34h], ecx
        jne    SHORT loc_5_040E
        xor    eax, eax
        mov    ecx, dword ptr [esi + 44h]
        mov    al, byte ptr [esi + 2]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_03EE:
        add    esp, 8
        mov    ecx, dword ptr [esi + 4]
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_03FB:
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_5_040E
        mov    eax, dword ptr [edi + 1ch]
        mov    dword ptr [eax + 34h], 0
        jmp    SHORT loc_5_0442
loc_5_040E:
        mov    al, byte ptr [esi + 1]
        test   al, al
        je     SHORT loc_5_0439
        jmp    SHORT loc_5_0424
loc_5_0417:
        test   byte ptr [edi + 2], 2
        je     SHORT loc_5_0442
        mov    al, byte ptr [esi + 1]
        test   al, al
        je     SHORT loc_5_0439
loc_5_0424:
        xor    ecx, ecx
        mov    cl, al
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0433:
        add    esp, 8
        mov    dword ptr [edi + 20h], eax
loc_5_0439:
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_043F:
        add    esp, 4
loc_5_0442:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0448:
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  090h
sub_5_0450:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + edi + 63h], 0ffh
        jne    SHORT loc_5_046F
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_046F:
        mov    al, byte ptr [esi + edi + 68h]
        cmp    byte ptr [esi + edi + 6dh], al
        ja     SHORT loc_5_0482
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_0482:
        mov    eax, 2
        cmp    edi, 1
        jg     SHORT loc_5_0491
        mov    eax, 1
loc_5_0491:
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0498:
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_5_04AE
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_04AE:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_04B4:
        mov    byte ptr [eax], 0a1h
        add    esp, 4
        mov    ebx, eax
        mov    dl, byte ptr [esi + edi + 63h]
        mov    word ptr [ebx + 8], 200h
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [ebx + 3], dl
        mov    dword ptr [ebx + 4ch], eax
        mov    ecx, dword ptr [eax + 4]
        db  08Bh, 004h, 0BDh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 4], ecx
        lea    ecx, [ebx + 1ah]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], edx
        mov    edx, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 0ch]
        push   ebx
        mov    dword ptr [ecx + 8], edx
        inc    byte ptr [esi + edi + 68h]
        mov    dword ptr [ecx + 0ch], eax
        db  0C7h, 043h, 02Ch, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 40h], edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0509:
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     SHORT loc_5_052C
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_051C:
        add    esp, 4
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0525:
        add    esp, 4
        dec    byte ptr [esi + edi + 68h]
loc_5_052C:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_0540:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        push   ebx
        or     byte ptr [edi + 2], 2
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0556:
        mov    byte ptr [eax], 0a0h
        add    esp, 4
        lea    ecx, [eax + 0ah]
        mov    edx, dword ptr [ebp + 10h]
        db  0C7h, 040h, 02Ch, 000h, 000h, 000h, 000h
        mov    esi, eax
        mov    al, byte ptr [ebx + 63h]
        mov    ebx, dword ptr [edx + 4]
        mov    byte ptr [esi + 3], al
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 8]
        mov    edx, dword ptr [edx + 0ch]
        mov    dword ptr [ecx + 4], ebx
        mov    dword ptr [ecx + 8], eax
        push   esi
        mov    dword ptr [ecx + 0ch], edx
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [esi + 8], cx
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [esi + 48h], edi
        mov    dword ptr [esi + 4], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_059D:
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_5_05AF
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_05AC:
        add    esp, 4
loc_5_05AF:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_05C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebx + 44h]
        mov    ecx, dword ptr [ebx + 40h]
        add    edx, 20h
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 14h], ecx
        dec    byte ptr [eax + ecx + 68h]
        lea    esi, [eax + ecx]
        push   ecx
        mov    dword ptr [ebp - 4], edx
        add    edx, 1bh
        push   eax
        mov    dword ptr [ebp - 0ch], edx
        call   sub_5_0450
        add    esp, 8
        mov    al, byte ptr [ebx + 1]
        test   al, al
        je     SHORT loc_5_0612
        cmp    al, 17h
        jne    NEAR PTR loc_5_0744
        mov    al, byte ptr [esi + 6dh]
        add    byte ptr [esi + 68h], al
        jmp    NEAR PTR loc_5_0744
loc_5_0612:
        cmp    word ptr [ebx + 8], 45h
        jb     NEAR PTR loc_5_0744
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax], 424d53ffh
        jne    NEAR PTR loc_5_0744
        cmp    byte ptr [eax + 4], 25h
        jne    NEAR PTR loc_5_0744
        mov    ecx, dword ptr [ebp - 4]
        cmp    byte ptr [ecx], 11h
        jne    NEAR PTR loc_5_0744
        mov    eax, dword ptr [ebp - 0ch]
        cmp    word ptr [eax + 2], 1
        jne    NEAR PTR loc_5_0744
        db  0BEh, 000h, 000h, 000h, 000h
        lea    edi, [eax + 0ah]
        mov    ecx, 0ah
        repe cmpsb
        jne    NEAR PTR loc_5_0744
        push   208h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_066F:
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     NEAR PTR loc_5_0744
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        add    eax, 13h
        lea    esi, [ebx + 0ah]
        push   eax
        push   ecx
        call   sub_5_02D0
        add    esp, 8
        lea    ecx, [ebp - 34h]
        db  0BFh, 000h, 000h, 000h, 000h
        push   1
        push   10h
        push   esi
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_06A6:
        add    esp, 10h
        mov    ecx, 10h
        repe cmpsb
        jne    SHORT loc_5_0710
        mov    esi, dword ptr [ebp - 0ch]
        db  0BFh, 000h, 000h, 000h, 000h
        add    esi, 14h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, ecx
        db  0BFh, 000h, 000h, 000h, 000h
        shr    ecx, 2
        repe cmpsd
        jne    SHORT loc_5_0710
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb
        jne    SHORT loc_5_0710
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_0710
        push   0
        lea    eax, [ebp - 34h]
        push   eax
        mov    cl, byte ptr [ebx + 30h]
        push   ecx
        xor    eax, eax
        mov    edx, dword ptr [ebp - 4]
        mov    ax, word ptr [edx + 17h]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [edx + 19h]
        add    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_5_07D0
        add    esp, 14h
        jmp    SHORT loc_5_0738
loc_5_0710:
        push   0
        lea    eax, [ebp - 34h]
        push   eax
        mov    cl, byte ptr [ebx + 30h]
        push   ecx
        mov    edx, dword ptr [ebp - 4]
        mov    ax, word ptr [edx + 17h]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [edx + 19h]
        add    eax, dword ptr [ebp + 0ch]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0735:
        add    esp, 18h
loc_5_0738:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0741:
        add    esp, 4
loc_5_0744:
        mov    eax, dword ptr [ebx + 4ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_074D:
        add    esp, 4
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0756:
        add    esp, 4
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [ebp - 10h]
        push   ecx
        push   eax
        call   sub_5_0450
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_0770:
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        test   edi, edi
        je     SHORT loc_5_0796
        xor    ebx, ebx
loc_5_0781:
        cmp    byte ptr [edi + 0ch], bl
        je     SHORT loc_5_0790
        push   edi
        inc    esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_078D:
        add    esp, 4
loc_5_0790:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    SHORT loc_5_0781
loc_5_0796:
        xor    eax, eax
        test   esi, esi
        je     SHORT loc_5_07BC
        push   1f4h
        db  0A3h, 004h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0A2h, 014h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_07B5:
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_07BC:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_5_07D0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 8
        mov    al, byte ptr [eax]
        push   ebx
        push   esi
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        push   edi
        cmp    al, 1
        je     SHORT loc_5_07F1
        cmp    al, 0ch
        je     SHORT loc_5_07F1
        cmp    al, 0fh
        jne    SHORT loc_5_080F
loc_5_07F1:
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, 0ffffffffh
        add    edi, 20h
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 30h
        jbe    SHORT loc_5_080F
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 50h], 0
loc_5_080F:
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 10h]
        db  039h, 00Dh, 000h, 000h, 000h, 000h
        jle    NEAR PTR loc_5_0A45
        xor    eax, eax
        db  08Ah, 081h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        cmp    eax, 0ffh
        je     NEAR PTR loc_5_0A45
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        cmp    al, 0fh
        jne    NEAR PTR loc_5_08D1
        mov    ecx, dword ptr [ebp - 4]
        db  08Dh, 004h, 08Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        mov    ecx, dword ptr [eax]
        cmp    byte ptr [ecx + 2], 2
        jne    SHORT loc_5_0872
        cmp    dword ptr [ebp + 14h], 0
        je     SHORT loc_5_0872
        mov    esi, dword ptr [ebp + 14h]
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        lea    esi, [esi + 10h]
        repe cmpsb
        jne    SHORT loc_5_08AE
loc_5_0872:
        push   0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 6
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        push   ebx
        call   sub_5_0A70
        add    esp, 10h
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [ebp + 14h], 0
        jne    SHORT loc_5_0896
        db  0B8h, 000h, 000h, 000h, 000h
loc_5_0896:
        mov    esi, dword ptr [ebp + 18h]
        push   esi
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [ecx + 54h]
        push   edx
        call   sub_5_1420
        add    esp, 0ch
        jmp    SHORT loc_5_08B1
loc_5_08AE:
        mov    esi, dword ptr [ebp + 18h]
loc_5_08B1:
        push   esi
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    ecx, dword ptr [ebp + 10h]
        push   ecx
        mov    edx, dword ptr [ebp + 0ch]
        push   edx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_1490
        add    esp, 14h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_08D1:
        cmp    al, 8
        jne    SHORT loc_5_093A
        mov    ecx, dword ptr [ebp - 4]
        db  08Bh, 004h, 08Dh, 000h, 000h, 000h, 000h
        cmp    byte ptr [eax + 2], 2
        jne    SHORT loc_5_08FF
        cmp    dword ptr [ebp + 14h], 0
        je     SHORT loc_5_08FF
        mov    esi, dword ptr [ebp + 14h]
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        lea    esi, [esi + 10h]
        repe cmpsb
        jne    SHORT loc_5_0917
loc_5_08FF:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0904:
        add    eax, 3ch
        mov    ecx, dword ptr [ebp - 4]
        lea    edx, [ecx + ecx*4]
        lea    ecx, [edx + edx*8]
        sub    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + ebx + 35h], eax
loc_5_0917:
        mov    esi, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp + 14h]
        push   esi
        mov    ecx, dword ptr [ebp + 10h]
        push   eax
        mov    edx, dword ptr [ebp + 0ch]
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   edx
        push   eax
        call   sub_5_1490
        add    esp, 14h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_093A:
        cmp    al, 0ah
        jne    NEAR PTR loc_5_0A22
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 1], 0
        je     NEAR PTR loc_5_0A22
        cmp    dword ptr [eax + 2], 2
        jne    SHORT loc_5_0990
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_5_0F80
        add    esp, 8
        test   eax, eax
        je     SHORT loc_5_0990
        test   byte ptr [eax + 2], 4
        je     SHORT loc_5_0990
        mov    esi, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp + 14h]
        push   esi
        mov    ecx, dword ptr [ebp + 10h]
        push   eax
        mov    edx, dword ptr [ebp + 0ch]
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   edx
        push   eax
        call   sub_5_1490
        add    esp, 14h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_0990:
        test   ebx, ebx
        je     SHORT loc_5_09A9
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2]
loc_5_099A:
        cmp    dword ptr [ebx + 0fh], eax
        je     SHORT loc_5_09A5
        mov    ebx, dword ptr [ebx]
        test   ebx, ebx
        jne    SHORT loc_5_099A
loc_5_09A5:
        test   ebx, ebx
        jne    SHORT loc_5_09B2
loc_5_09A9:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_09B2:
        mov    eax, 1
        mov    cl, byte ptr [ebp - 4]
        shl    eax, cl
        test   dword ptr [ebx + 8], eax
        je     SHORT loc_5_09CF
        test   dword ptr [ebx + 4], eax
        jne    SHORT loc_5_09CF
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_09CF:
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 1], 1
        jbe    SHORT loc_5_09EF
        lea    edi, [eax + 6]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        mov    eax, dword ptr [ebp + 8]
        not    ecx
        lea    eax, [ecx + eax + 6]
        jmp    SHORT loc_5_09F1
loc_5_09EF:
        xor    eax, eax
loc_5_09F1:
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 8]
        add    eax, 6
        push   eax
        push   ecx
        push   ebx
        call   sub_5_0A70
        add    esp, 10h
        mov    ecx, dword ptr [ebp - 4]
        lea    edx, [ecx + ecx*4]
        pop    edi
        pop    esi
        lea    eax, [edx + edx*8]
        sub    eax, ecx
        mov    dword ptr [eax + ebx + 39h], 0
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_0A22:
        mov    esi, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp + 14h]
        push   esi
        mov    ecx, dword ptr [ebp + 10h]
        push   eax
        mov    edx, dword ptr [ebp + 0ch]
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   edx
        push   eax
        call   sub_5_1490
        add    esp, 14h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_0A45:
        mov    esi, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp + 14h]
        push   esi
        mov    ecx, dword ptr [ebp + 10h]
        push   eax
        mov    edx, dword ptr [ebp + 0ch]
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   edx
        push   eax
        call   sub_5_1490
        add    esp, 14h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_5_0A70:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    cl, byte ptr [ebp + 0ch]
        shl    edx, cl
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 10h]
        test   edi, edi
        je     SHORT loc_5_0ABE
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, dword ptr [ebp + 0ch]
        mov    esi, edi
        mov    dword ptr [ebp - 4], ecx
        shr    ecx, 2
        lea    ebx, [eax + eax*4]
        lea    eax, [ebx + ebx*8]
        mov    ebx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ebp + 0ch]
        lea    edi, [eax + ebx + 3fh]
        rep movsd
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 3
        rep movsb
        jmp    SHORT loc_5_0AC1
loc_5_0ABE:
        mov    ebx, dword ptr [ebp + 8]
loc_5_0AC1:
        mov    edi, dword ptr [ebp + 14h]
        test   edi, edi
        je     SHORT loc_5_0AF7
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, dword ptr [ebp + 0ch]
        mov    esi, edi
        mov    dword ptr [ebp - 4], ecx
        shr    ecx, 2
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*8]
        sub    eax, dword ptr [ebp + 0ch]
        lea    edi, [eax + ebx + 51h]
        rep movsd
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 3
        rep movsb
loc_5_0AF7:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, ecx
        or     ecx, edx
        and    eax, edx
        mov    dword ptr [ebx + 8], ecx
        not    edx
        and    dword ptr [ebx + 4], edx
        cmp    byte ptr [ebx + 0ch], 0
        je     SHORT loc_5_0B1B
        test   eax, eax
        jne    SHORT loc_5_0B1B
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0B18:
        add    esp, 4
loc_5_0B1B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_0B30:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [ebx + 18h], 1
        add    eax, 0dh
        mov    dword ptr [ebx], 7
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ebx + 14h], eax
        mov    byte ptr [ebx + 4], 2
        db  0C7h, 043h, 00Ch, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 28h], 0
        movzx  dx, byte ptr [ecx + 5ah]
        mov    word ptr [ebx + 6], dx
        mov    esi, dword ptr [ebp + 14h]
        test   esi, esi
        je     SHORT loc_5_0B9E
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    edi, dword ptr [ebp + 0ch]
        lea    edx, [ecx]
        lea    eax, [ecx - 1]
        add    edi, 13h
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        add    eax, 7
        mov    dword ptr [ebx], eax
loc_5_0B9E:
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0BA4:
        add    esp, 4
        test   eax, eax
        je     SHORT loc_5_0BB4
        movsx  eax, word ptr [ebx + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_5_0BB4:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_5_0BC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 10h]
        xor    edi, edi
        mov    eax, 1
        mov    word ptr [ebp - 4], di
        mov    cl, bl
        mov    word ptr [ebp - 2], di
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        shl    eax, cl
        test   dword ptr [edx + 8], eax
        mov    dword ptr [ebp - 0ch], eax
        je     NEAR PTR loc_5_0D08
        push   270h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0BFB:
        add    esp, 4
        mov    esi, eax
        cmp    esi, edi
        je     NEAR PTR loc_5_0D08
        lea    eax, [esi + 5ch]
        lea    ecx, [ebx + ebx*4]
        mov    word ptr [esi + 10h], 11ah
        lea    ecx, [ecx + ecx*8]
        mov    dword ptr [ebp - 8], eax
        sub    ecx, ebx
        db  0A1h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 8]
        lea    ecx, [ecx + eax + 3dh]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 18h], ecx
        add    eax, 23h
        mov    dword ptr [esi + 12h], edx
        lea    edx, [ebp - 4]
        lea    edi, [ebp - 2]
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 8], edx
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        add    ecx, 2
        mov    dword ptr [esi + 4], 40000h
        mov    word ptr [esi + 16h], 1
        mov    dword ptr [esi + 0ch], edi
loc_5_0C58:
        mov    al, byte ptr [edx]
        cmp    al, byte ptr [ecx]
        jne    SHORT loc_5_0C78
        or     al, al
        je     SHORT loc_5_0C74
        mov    al, byte ptr [edx + 1]
        cmp    al, byte ptr [ecx + 1]
        jne    SHORT loc_5_0C78
        add    edx, 2
        add    ecx, 2
        or     al, al
        jne    SHORT loc_5_0C58
loc_5_0C74:
        xor    eax, eax
        jmp    SHORT loc_5_0C7D
loc_5_0C78:
        sbb    eax, eax
        sbb    eax, -1
loc_5_0C7D:
        test   eax, eax
        jne    SHORT loc_5_0C93
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_5_0C93
        push   ebx
        push   esi
        call   eax
        add    esp, 8
        jmp    SHORT loc_5_0CC3
loc_5_0C93:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [edi + 6], bx
        add    eax, 11ah
        mov    ecx, dword ptr [edi + 0ch]
        push   1
        push   1
        mov    dword ptr [ebp - 10h], ecx
        push   esi
        mov    dword ptr [edi + 0ch], eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0CB6:
        add    esp, 10h
        mov    ecx, dword ptr [ebp - 10h]
        movsx  eax, word ptr [edi + 1ah]
        mov    dword ptr [edi + 0ch], ecx
loc_5_0CC3:
        test   eax, eax
        je     SHORT loc_5_0CD5
        cmp    eax, 0eah
        je     SHORT loc_5_0CD5
        cmp    eax, 84bh
        jne    SHORT loc_5_0CFF
loc_5_0CD5:
        cmp    word ptr [ebp - 2], 1
        jne    SHORT loc_5_0CFF
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        test   dword ptr [eax + 8], ecx
        je     SHORT loc_5_0CEC
        test   dword ptr [eax + 4], ecx
        je     SHORT loc_5_0CFF
loc_5_0CEC:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp + 0ch]
        push   ebx
        push   ecx
        call   sub_5_0A70
        add    esp, 10h
loc_5_0CFF:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0D05:
        add    esp, 4
loc_5_0D08:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
sub_5_0D10:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        mov    eax, dword ptr [ebp + 10h]
        push   esi
        mov    ebx, dword ptr [ebp + 0ch]
        push   edi
        mov    cl, byte ptr [eax + 5eh]
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [ebx + 23h]
        or     dword ptr [ebx + 4], ecx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        mov    dword ptr [esi + 2ch], edi
        repne scasb
        lea    eax, [ebp - 10h]
        push   1
        push   1dh
        mov    dword ptr [esi + 10h], eax
        not    ecx
        dec    ecx
        push   esi
        mov    dword ptr [esi], ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0D4E:
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_0D60
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_0D60:
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 10h]
        push   eax
        push   ecx
        push   ebx
        push   esi
        call   sub_5_0B30
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_5_0D81
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_0D81:
        cmp    dword ptr [ebp + 18h], 0
        je     SHORT loc_5_0DE7
        mov    eax, dword ptr [ebp + 10h]
        cmp    byte ptr [eax + 2], 0
        jne    SHORT loc_5_0DE7
        lea    edi, [ebx + 23h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        mov    dword ptr [esi + 2ch], edi
        repne scasb
        lea    eax, [ebp - 10h]
        push   1
        push   1bh
        mov    dword ptr [esi + 10h], eax
        not    ecx
        dec    ecx
        push   esi
        mov    dword ptr [esi], ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0DB4:
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_0DC6
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_0DC6:
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 10h]
        push   eax
        push   ecx
        push   ebx
        push   esi
        call   sub_5_0B30
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_5_0DE7
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_0DE7:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_0DF0:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 10h
        db  08Bh, 00Ch, 085h, 000h, 000h, 000h, 000h
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        movzx  dx, byte ptr [ecx + 5ah]
        mov    word ptr [esi + 6], dx
        mov    edi, dword ptr [ebp + 0ch]
        add    edi, 23h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        mov    dword ptr [esi + 2ch], edi
        repne scasb
        lea    eax, [ebp - 10h]
        push   1
        push   1eh
        mov    dword ptr [esi + 10h], eax
        not    ecx
        dec    ecx
        push   esi
        mov    dword ptr [esi], ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E36:
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_5_0E71
        db  0C7h, 046h, 00Ch, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 014h, 000h, 000h, 000h, 000h
        push   esi
        mov    dword ptr [esi], 1eh
        mov    dword ptr [esi + 28h], 0
        mov    byte ptr [esi + 4], 2
        mov    word ptr [esi + 18h], 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E68:
        mov    word ptr [esi + 1ah], 0
        add    esp, 4
loc_5_0E71:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_0E80:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, 1dh
        sub    esp, 84h
        push   edi
        lea    edi, [ebp - 74h]
        rep stosd
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        add    edi, 23h
        db  08Bh, 00Ch, 085h, 000h, 000h, 000h, 000h
        sub    eax, eax
        mov    dword ptr [ebp - 48h], edi
        movzx  dx, byte ptr [ecx + 5ah]
        mov    word ptr [ebp - 6eh], dx
        mov    ecx, 0ffffffffh
        repne scasb
        lea    eax, [ebp - 84h]
        push   1
        push   1bh
        mov    dword ptr [ebp - 64h], eax
        not    ecx
        dec    ecx
        mov    dword ptr [ebp - 74h], ecx
        lea    ecx, [ebp - 74h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0EDA:
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_5_0F13
        db  0C7h, 045h, 098h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0A0h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 74h], 7
        mov    dword ptr [ebp - 4ch], 0
        mov    byte ptr [ebp - 70h], 2
        mov    word ptr [ebp - 5ch], 1
        lea    eax, [ebp - 74h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0F10:
        add    esp, 4
loc_5_0F13:
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
VRedir_DeleteMailSlot:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        call   sub_5_0F80
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_5_0F42
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret
loc_5_0F42:
        mov    cl, byte ptr [eax + 4]
        inc    cl
        mov    byte ptr [eax + 4], cl
        jne    SHORT loc_5_0F51
        inc    cl
        mov    byte ptr [eax + 4], cl
loc_5_0F51:
        cmp    dword ptr [eax + 5], 2
        jb     SHORT loc_5_0F73
        mov    cl, byte ptr [eax + 2]
        test   cl, 40h
        jne    SHORT loc_5_0F73
        or     cl, 40h
        push   0ea60h
        push   eax
        mov    byte ptr [eax + 2], cl
        call   sub_5_1080
        add    esp, 8
loc_5_0F73:
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_0F80:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    cl, byte ptr [ebp + 8]
        db  039h, 00Dh, 000h, 000h, 000h, 000h
        jle    NEAR PTR loc_5_1067
        db  08Ah, 081h, 000h, 000h, 000h, 000h
        cmp    al, 0ffh
        je     NEAR PTR loc_5_1067
        xor    ecx, ecx
        mov    cl, al
        db  08Bh, 01Ch, 08Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebx + 54h]
        test   eax, eax
        jne    NEAR PTR loc_5_1076
        mov    esi, dword ptr [ebp + 0ch]
        test   esi, esi
        jne    SHORT loc_5_0FC9
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_5_0FC9:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    byte ptr [eax + 46h], 1
        sbb    edi, edi
        db  0A1h, 000h, 000h, 000h, 000h
        and    edi, 0fffffff0h
        push   eax
        add    edi, 119h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0FE8:
        add    esp, 4
        mov    eax, dword ptr [ebx + 54h]
        test   eax, eax
        jne    NEAR PTR loc_5_1076
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0FFC:
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_5_1020
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi], 8
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1016:
        add    esp, 4
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_5_1020:
        mov    dword ptr [edi + 31h], edi
        mov    dword ptr [ebx + 54h], edi
        mov    word ptr [edi], 6942h
        mov    dword ptr [edi + 19h], ebx
        db  0C7h, 047h, 02Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [edi + 25h], 0
        mov    byte ptr [edi + 35h], 18h
        mov    dword ptr [edi + 15h], 0ah
        db  0C7h, 047h, 01Dh, 000h, 000h, 000h, 000h
        mov    byte ptr [edi + 4], 1
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_105D:
        add    esp, 4
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_5_1067:
        mov    esi, dword ptr [ebp + 0ch]
        test   esi, esi
        je     SHORT loc_5_1074
        mov    dword ptr [esi], 57h
loc_5_1074:
        xor    eax, eax
loc_5_1076:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_5_1080:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 3]
        test   al, al
        je     SHORT loc_5_10E0
        mov    edi, dword ptr [ebp + 0ch]
        cmp    edi, 32h
        jbe    SHORT loc_5_10AC
        movsx  ecx, al
        mov    eax, edi
        sub    edx, edx
        div    ecx
        cmp    eax, 32h
        mov    edi, eax
        jae    SHORT loc_5_10AC
        mov    edi, 32h
loc_5_10AC:
        mov    al, byte ptr [esi + 2]
        test   al, 1
        je     SHORT loc_5_10C4
        and    al, 0feh
        lea    ecx, [esi + 21h]
        push   ecx
        mov    byte ptr [esi + 2], al
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_10C1:
        add    esp, 4
loc_5_10C4:
        mov    al, byte ptr [esi + 2]
        mov    dword ptr [esi + 15h], edi
        test   al, 2
        jne    SHORT loc_5_10E0
        or     al, 1
        push   edi
        mov    byte ptr [esi + 2], al
        add    esi, 21h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_10DD:
        add    esp, 8
loc_5_10E0:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_10F0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    dword ptr [eax + 1ch], 0
        push   edi
        push   2900h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_110B:
        add    esp, 8
        mov    edx, eax
        test   edx, edx
        je     SHORT loc_5_112C
        lea    esi, [edx + 40h]
        xor    eax, eax
        mov    edi, esi
        mov    ecx, 7
        rep stosd
        mov    dword ptr [edx + 3ch], 424d53ffh
        mov    byte ptr [esi], 25h
loc_5_112C:
        mov    eax, edx
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1140:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0ch]
        test   ebx, ebx
        je     NEAR PTR loc_5_1201
        mov    edx, dword ptr [ebp + 1ch]
        lea    edi, [ebx + 5ch]
        mov    eax, dword ptr [ebp + 18h]
        mov    ecx, edx
        shr    ecx, 2
        mov    esi, eax
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    esi, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [esi + 9]
        sub    ecx, esi
        cmp    ecx, 0d9h
        jne    SHORT loc_5_11A7
        test   byte ptr [esi + 2], 4
        je     SHORT loc_5_11A7
        mov    ecx, dword ptr [esi + 19h]
        cmp    byte ptr [ecx + 2], 2
        je     SHORT loc_5_11A7
        push   0
        mov    edx, dword ptr [ebp + 20h]
        push   edx
        mov    cl, byte ptr [ecx + 5ah]
        push   ecx
        add    eax, 36h
        mov    ecx, dword ptr [ebp + 1ch]
        sub    ecx, 36h
        push   ecx
        push   eax
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 14h
loc_5_11A7:
        db  0C7h, 043h, 024h, 000h, 000h, 000h, 000h
        or     byte ptr [ebx + 2], 1
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 38h], esi
        je     SHORT loc_5_11C6
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_11C4:
        jmp    SHORT loc_5_120A
loc_5_11C6:
        mov    ecx, dword ptr [esi + 19h]
        test   byte ptr [ecx + 61h], 2
        jne    SHORT loc_5_11F9
        mov    eax, dword ptr [ebp + 24h]
        mov    edx, dword ptr [ebp + 20h]
        push   eax
        push   edx
        mov    eax, dword ptr [ebp + 1ch]
        lea    edx, [ebx + 3ch]
        add    eax, 20h
        mov    esi, dword ptr [ebp + 14h]
        push   eax
        push   edx
        db  08Bh, 004h, 0B5h, 000h, 000h, 000h, 000h
        push   eax
        push   ecx
        push   ebx
        call   dword ptr [ecx + 38h]
        add    esp, 1ch
        test   eax, eax
        je     SHORT loc_5_120D
loc_5_11F9:
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_11FF:
        jmp    SHORT loc_5_120A
loc_5_1201:
        mov    esi, dword ptr [ebp + 10h]
        push   esi
        call   sub_5_1380
loc_5_120A:
        add    esp, 4
loc_5_120D:
        mov    eax, dword ptr [ebp + 8]
        test   eax, eax
        je     SHORT loc_5_121A
        mov    word ptr [eax + 1ah], 0
loc_5_121A:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  090h
loc_5_1220:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        push   eax
        call   sub_5_10F0
        add    esp, 4
        test   eax, eax
        je     SHORT loc_5_1294
        test   byte ptr [esi + 13h], 4
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_5_1258
        mov    edx, 3
        mov    edi, 1
        mov    byte ptr [ebx], 0fh
        jmp    SHORT loc_5_126E
loc_5_1258:
        mov    edx, 2
        mov    edi, 0
        cmp    byte ptr [esi + 37h], 1
        adc    edi, -1
        mov    byte ptr [ebx], 1
        and    edi, edx
loc_5_126E:
        mov    cl, byte ptr [esi + 4]
        mov    byte ptr [ebx + 1], cl
        movsx  ecx, byte ptr [esi + 3]
        imul   ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ebx + 2], ecx
        test   ecx, ecx
        jne    SHORT loc_5_128C
        mov    dword ptr [ebx + 18h], 0
        jmp    SHORT loc_5_129A
loc_5_128C:
        mov    ecx, dword ptr [esi + 11h]
        mov    dword ptr [ebx + 18h], ecx
        jmp    SHORT loc_5_129A
loc_5_1294:
        mov    edx, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp - 4]
loc_5_129A:
        push   edi
        lea    ecx, [esi + 47h]
        push   ecx
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        push   ebx
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        push   edi
        push   edx
        push   esi
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_1140
        add    esp, 20h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_12D0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 74h
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 10h]
        mov    edx, dword ptr [esi + 9]
        mov    edi, dword ptr [esi + 15h]
        cmp    dword ptr [esi + 19h], 0
        je     SHORT loc_5_136A
        mov    cl, byte ptr [esi + 2]
        and    cl, 0beh
        test   cl, 2
        mov    byte ptr [esi + 2], cl
        jne    SHORT loc_5_136A
        test   edx, edx
        je     SHORT loc_5_136A
        test   edi, edi
        je     SHORT loc_5_133D
        mov    eax, dword ptr [esi + 0dh]
        test   eax, eax
        je     SHORT loc_5_130C
        cmp    edx, eax
        jne    SHORT loc_5_130F
loc_5_130C:
        inc    dword ptr [esi + 5]
loc_5_130F:
        mov    eax, dword ptr [esi + 5]
        test   eax, eax
        je     SHORT loc_5_1330
        cmp    eax, 0ch
        jbe    SHORT loc_5_1323
        db  08Bh, 03Dh, 00Ch, 000h, 000h, 000h
        jmp    SHORT loc_5_132D
loc_5_1323:
        dec    eax
        and    eax, 0fffffffch
        db  08Bh, 0B8h, 000h, 000h, 000h, 000h
loc_5_132D:
        mov    dword ptr [esi + 15h], edi
loc_5_1330:
        cmp    edi, 0ea60h
        jae    SHORT loc_5_133D
        mov    edi, 0ea60h
loc_5_133D:
        or     cl, 2
        xor    eax, eax
        mov    byte ptr [esi + 2], cl
        mov    esi, dword ptr [ebp + 0ch]
        test   esi, esi
        jne    SHORT loc_5_1355
        mov    word ptr [ebp - 5ah], ax
        mov    dword ptr [ebp - 6ch], eax
        test   esi, esi
loc_5_1355:
        mov    ecx, esi
        jne    SHORT loc_5_135C
        lea    ecx, [ebp - 74h]
loc_5_135C:
        push   edi
        mov    eax, dword ptr [edx + 0ch]
        push   eax
        push   ecx
        call   dword ptr [edx + 8]
        add    esp, 0ch
        jmp    SHORT loc_5_136D
loc_5_136A:
        mov    esi, dword ptr [ebp + 0ch]
loc_5_136D:
        test   esi, esi
        je     SHORT loc_5_137A
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1377:
        add    esp, 4
loc_5_137A:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_5_1380:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    cl, byte ptr [edx + 2]
        and    cl, 0fdh
        mov    eax, dword ptr [edx + 9]
        mov    byte ptr [edx + 2], cl
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [edx + 9], eax
        mov    eax, dword ptr [edx + 15h]
        test   eax, eax
        jne    SHORT loc_5_13C0
        mov    al, byte ptr [edx + 3]
        dec    al
        mov    byte ptr [edx + 3], al
        test   al, al
        jle    SHORT loc_5_13B8
        push   0
        push   edx
        call   sub_5_1080
        mov    esp, ebp
        pop    ebp
        ret
loc_5_13B8:
        or     cl, 2
        pop    ebp
        mov    byte ptr [edx + 2], cl
        ret
loc_5_13C0:
        push   eax
        push   edx
        call   sub_5_1080
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_5_13D0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [eax + 38h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_13E1:
        add    esp, 4
        push   esi
        call   sub_5_1380
        add    esp, 4
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_1400:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_1410:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1420:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        test   ecx, ecx
        je     SHORT loc_5_147E
        test   edx, edx
        jne    SHORT loc_5_1438
        db  0BAh, 000h, 000h, 000h, 000h
loc_5_1438:
        mov    eax, dword ptr [ecx + 19h]
        cmp    byte ptr [eax + 2], 2
        jne    SHORT loc_5_147E
        lea    esi, [ecx + 37h]
        mov    eax, dword ptr [edx]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edx + 4]
        add    ecx, 47h
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edx + 8]
        mov    edx, dword ptr [edx + 0ch]
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi + 0ch], edx
        mov    edx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [edx]
        mov    esi, dword ptr [edx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [ecx + 4], esi
        mov    esi, dword ptr [edx + 0ch]
        mov    dx, word ptr [edx + 10h]
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx + 0ch], esi
        mov    word ptr [ecx + 10h], dx
loc_5_147E:
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1490:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], 0
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_5_14B2
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_14B2:
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx], 2
        je     SHORT loc_5_14DD
        mov    eax, dword ptr [ebp + 18h]
        mov    edx, dword ptr [ebp + 14h]
        push   eax
        mov    ebx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [ebp + 0ch]
        push   ebx
        push   eax
        push   ecx
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 14h
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_14DD:
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_5_0F80
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_5_14FA
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_14FA:
        mov    eax, dword ptr [ebx + 19h]
        mov    edx, dword ptr [ebp + 14h]
        cmp    byte ptr [eax + 2], 2
        jne    SHORT loc_5_1524
        test   edx, edx
        je     SHORT loc_5_1524
        lea    esi, [edx + 10h]
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        repe cmpsb
        je     SHORT loc_5_1524
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_1524:
        test   byte ptr [ebx + 2], 2
        je     SHORT loc_5_153F
        mov    eax, ebx
        sub    eax, dword ptr [ebx + 9]
        cmp    eax, 0ffffff27h
        jne    SHORT loc_5_153F
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_153F:
        test   byte ptr [ebx + 13h], 4
        je     SHORT loc_5_155F
        test   edx, edx
        je     SHORT loc_5_155F
        lea    esi, [edx + 10h]
        mov    ecx, 10h
        db  08Bh, 03Dh, 010h, 000h, 000h, 000h
        repe cmpsb
        je     SHORT loc_5_155F
        push   64h
        jmp    SHORT loc_5_156A
loc_5_155F:
        cmp    dword ptr [ebx + 5], 5
        jbe    SHORT loc_5_1575
        push   1d4c0h
loc_5_156A:
        call   sub_5_15D0
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
loc_5_1575:
        cmp    dword ptr [ebp - 4], 0
        lea    eax, [ebx + 0d9h]
        mov    dword ptr [ebx + 9], eax
        je     SHORT loc_5_1591
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   ebx
        call   sub_5_1080
        add    esp, 8
loc_5_1591:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_5_15A0:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        imul   eax, eax, 41c64e6dh
        mov    ebp, esp
        add    eax, 3039h
        db  0A3h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_15BF
        mov    eax, 1
loc_5_15BF:
        sub    edx, edx
        db  0A3h, 000h, 000h, 000h, 000h
        div    dword ptr [ebp + 8]
        mov    eax, edx
        pop    ebp
        ret
        db  08Dh, 049h, 000h
sub_5_15D0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, esi
        shr    eax, 2
        push   eax
        call   sub_5_15A0
        add    esp, 4
        mov    ecx, esi
        shr    ecx, 3
        sub    eax, ecx
        add    eax, esi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_1600:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        test   eax, eax
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_5_1632
        mov    edx, dword ptr [ebp + 8]
loc_5_1612:
        lea    esi, [eax + 0ch]
        mov    ebx, dword ptr [eax + 8]
        mov    ecx, ebx
        mov    edi, edx
        shr    ecx, 2
        repe cmpsd
        jne    SHORT loc_5_162A
        mov    ecx, ebx
        and    ecx, 3
        repe cmpsb
loc_5_162A:
        je     SHORT loc_5_1632
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    SHORT loc_5_1612
loc_5_1632:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_1640:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi], 10h
        je     SHORT loc_5_1669
        mov    ecx, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp + 14h]
        push   ecx
        mov    edx, dword ptr [ebp + 10h]
        push   eax
        mov    ecx, dword ptr [ebp + 0ch]
        push   edx
        push   ecx
        push   esi
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 14h
        pop    esi
        pop    ebp
        ret
loc_5_1669:
        lea    eax, [esi + 2]
        push   eax
        call   sub_5_1600
        add    esp, 4
        test   eax, eax
        je     SHORT loc_5_1690
        mov    ecx, dword ptr [ebp + 18h]
        mov    edx, dword ptr [ebp + 14h]
        push   ecx
        push   edx
        mov    cl, byte ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 0ch]
        push   ecx
        push   edx
        push   esi
        call   dword ptr [eax + 4]
        add    esp, 14h
loc_5_1690:
        pop    esi
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_16A0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [eax + 10h]
        mov    al, byte ptr [esi + 4]
        test   al, al
        mov    edi, dword ptr [ecx + 72h]
        jne    SHORT loc_5_16E1
        cmp    dword ptr [edi + 6], 0
        jne    SHORT loc_5_16CA
        mov    eax, dword ptr [esi + 28h]
        mov    dword ptr [edi + 6], eax
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_5_16CA:
        test   al, al
        jne    SHORT loc_5_16E1
        mov    eax, dword ptr [esi + 28h]
        sub    eax, dword ptr [edi + 6]
        cmp    eax, 0f0h
        jae    SHORT loc_5_16E1
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_5_16E1:
        push   1
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   82bh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_16F8:
        add    esp, 14h
        cmp    eax, -3
        je     SHORT loc_5_1718
        test   eax, eax
        je     SHORT loc_5_170F
        mov    eax, dword ptr [esi + 28h]
        sub    eax, 0eeh
        mov    dword ptr [edi + 6], eax
loc_5_170F:
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_5_1718:
        mov    eax, 0fffffffdh
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_1730:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   62h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1740:
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_5_1755
        mov    eax, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_1755:
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1760:
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp - 4]
        mov    word ptr [esi + 22h], 1
        mov    dword ptr [esi + 40h], eax
        mov    dword ptr [esi + 24h], ebx
        mov    eax, dword ptr [ebp + 18h]
        mov    edx, dword ptr [ebp + 14h]
        mov    word ptr [esi + 28h], 62h
        mov    word ptr [esi + 2eh], ax
        xor    eax, eax
        mov    ax, word ptr [esi + 22h]
        lea    ecx, [eax + eax*2]
        mov    dword ptr [esi + ecx*2 + 24h], edx
        mov    ecx, dword ptr [ebp + 20h]
        inc    word ptr [esi + 22h]
        test   cl, 2
        je     SHORT loc_5_17CF
        mov    eax, dword ptr [ebp + 1ch]
        test   eax, eax
        je     SHORT loc_5_17CF
        mov    edi, dword ptr [eax]
        add    esi, 1ch
        add    eax, 6
        mov    dword ptr [esi], edi
        mov    di, word ptr [eax - 2]
        mov    word ptr [esi + 4], di
        lea    esi, [ebx + 6]
        mov    edi, dword ptr [eax]
        mov    dword ptr [esi], edi
        mov    edi, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [esi + 8], eax
        jmp    SHORT loc_5_17F1
loc_5_17CF:
        mov    eax, dword ptr [ebp - 4]
        mov    esi, 0ffffffffh
        mov    dword ptr [eax + 1ch], esi
        add    eax, 1ch
        mov    word ptr [eax + 4], si
        lea    eax, [ebx + 0ah]
        mov    dword ptr [ebx + 6], 0
        mov    dword ptr [eax], esi
        mov    word ptr [eax + 4], si
loc_5_17F1:
        mov    eax, 5305h
        mov    esi, dword ptr [ebp - 4]
        mov    byte ptr [ebx + 5], 14h
        mov    byte ptr [ebx + 3eh], 0fch
        test   cl, 1
        mov    word ptr [ebx + 40h], 0
        mov    word ptr [esi + 0ah], ax
        mov    word ptr [ebx + 10h], ax
        je     SHORT loc_5_181D
        mov    byte ptr [ebx + 3fh], 3
        mov    ecx, dword ptr [ebp + 10h]
        jmp    SHORT loc_5_1835
loc_5_181D:
        mov    ecx, dword ptr [ebp + 10h]
        mov    al, byte ptr [ecx + 0fh]
        test   al, al
        je     SHORT loc_5_1831
        cmp    al, 20h
        je     SHORT loc_5_1831
        mov    byte ptr [ebx + 3fh], 2
        jmp    SHORT loc_5_1835
loc_5_1831:
        mov    byte ptr [ebx + 3fh], 1
loc_5_1835:
        lea    esi, [ebx + 42h]
        mov    eax, 20202020h
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi + 0ch], eax
        lea    eax, [ebp - 18h]
        mov    esi, eax
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0ch], 0
        mov    byte ptr [eax + 10h], 0
        mov    eax, dword ptr [ecx]
        mov    edi, dword ptr [ecx + 4]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ecx + 0ch]
        mov    dword ptr [esi + 4], edi
        lea    edi, [ebp - 18h]
        mov    dword ptr [esi + 8], eax
        sub    eax, eax
        mov    dword ptr [esi + 0ch], ecx
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        lea    esi, [ebp - 18h]
        lea    edi, [ebx + 42h]
        lea    eax, [ecx - 1]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  0A1h, 000h, 000h, 000h, 000h
        db  0BEh, 000h, 000h, 000h, 000h
        lea    ecx, [ebx + 52h]
        mov    dword ptr [ecx], eax
        mov    edi, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], edi
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ecx + 8], eax
        mov    esi, dword ptr [esi + 0ch]
        cmp    dword ptr [edx], 424d53ffh
        mov    dword ptr [ecx + 0ch], esi
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ebx + 51h], 0
        lea    edi, [edx + 3bh]
        lea    ebx, [edx + 20h]
        db  0C7h, 041h, 004h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_192A
        cmp    byte ptr [edx + 4], 25h
        jne    SHORT loc_5_192A
        cmp    byte ptr [ebx], 11h
        jne    SHORT loc_5_192A
        cmp    word ptr [edi + 2], 1
        jne    SHORT loc_5_192A
        db  0BEh, 000h, 000h, 000h, 000h
        lea    edi, [edi + 0ah]
        mov    ecx, 11h
        repe cmpsb
        jne    SHORT loc_5_192A
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   0
        mov    al, byte ptr [eax + 5ah]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [ebx + 17h]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [ebx + 19h]
        add    eax, edx
        push   eax
        call   sub_5_07D0
        add    esp, 14h
loc_5_192A:
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp - 4]
        push   esi
        push   3
        or     byte ptr [eax + 2], 2
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_193C:
        add    esp, 8
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_5_1950:
        push   ebp
        mov    ecx, dword ptr [esp + 18h]
        mov    ebp, esp
        sub    esp, 0ch
        mov    eax, dword ptr [ecx]
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], eax
        cmp    byte ptr [edx + 3eh], 0f1h
        jb     SHORT loc_5_19B1
        lea    eax, [edx + 42h]
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        mov    edi, eax
        repe cmpsb
        je     SHORT loc_5_19A5
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        mov    edi, eax
        repe cmpsb
        je     SHORT loc_5_19A5
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        cmp    byte ptr [edx + 3fh], cl
        je     SHORT loc_5_19A5
        sti
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_19A5:
        sub    dword ptr [ebp - 4], 62h
        mov    esi, dword ptr [ebp + 0ch]
        add    esi, 62h
        jmp    SHORT loc_5_19BB
loc_5_19B1:
        sub    dword ptr [ebp - 4], 1eh
        mov    esi, dword ptr [ebp + 0ch]
        add    esi, 1eh
loc_5_19BB:
        lea    edx, [esi + 20h]
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ebp - 8], edx
        add    edx, 1bh
        sub    eax, dword ptr [ecx]
        add    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        mov    dword ptr [ebp - 0ch], edx
        push   ecx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_19DD:
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_5_1B4B
        cmp    dword ptr [ebp - 4], 45h
        jb     NEAR PTR loc_5_1B4B
        cmp    dword ptr [esi], 424d53ffh
        jne    NEAR PTR loc_5_1B4B
        cmp    byte ptr [esi + 4], 25h
        jne    NEAR PTR loc_5_1B4B
        mov    edx, dword ptr [ebp - 8]
        cmp    byte ptr [edx], 11h
        jne    NEAR PTR loc_5_1B4B
        mov    eax, dword ptr [ebp - 0ch]
        cmp    word ptr [eax + 2], 1
        jne    NEAR PTR loc_5_1B4B
        db  0BEh, 000h, 000h, 000h, 000h
        lea    edi, [eax + 0ah]
        mov    ecx, 0ah
        repe cmpsb
        jne    NEAR PTR loc_5_1B4B
        push   1
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1A42:
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        je     NEAR PTR loc_5_1B4B
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1A55:
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_5_1B40
        mov    ecx, dword ptr [ebp + 0ch]
        lea    edx, [eax + 4]
        mov    edi, dword ptr [ecx + 52h]
        add    ecx, 52h
        mov    dword ptr [edx], edi
        mov    edi, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], edi
        mov    edi, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ecx + 0ch]
        mov    dword ptr [edx + 8], edi
        mov    dword ptr [edx + 0ch], ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ecx + 42h]
        add    ecx, 42h
        lea    edx, [eax + 14h]
        mov    dword ptr [edx], edi
        mov    edi, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], edi
        mov    edi, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ecx + 0ch]
        mov    dword ptr [edx + 8], edi
        mov    dword ptr [edx + 0ch], ecx
        mov    ecx, dword ptr [ebp - 0ch]
        mov    edi, dword ptr [ecx + 14h]
        add    ecx, 14h
        lea    edx, [eax + 24h]
        mov    dword ptr [edx], edi
        mov    edi, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], edi
        mov    edi, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ecx + 0ch]
        mov    dword ptr [edx + 8], edi
        mov    dword ptr [edx + 0ch], ecx
        mov    ecx, dword ptr [ebp + 10h]
        lea    edx, [eax + 34h]
        mov    edi, dword ptr [ecx]
        mov    cx, word ptr [ecx + 4]
        mov    dword ptr [edx], edi
        mov    word ptr [edx + 4], cx
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [edi + 12h]
        add    edi, 12h
        lea    edx, [eax + 3ah]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [edi + 4]
        mov    edi, dword ptr [edi + 8]
        mov    dword ptr [edx + 4], ecx
        xor    ecx, ecx
        mov    dword ptr [edx + 8], edi
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [eax + 46h], esi
        mov    cx, word ptr [edx + 17h]
        xor    esi, esi
        mov    si, word ptr [edx + 19h]
        mov    dword ptr [eax], ecx
        sub    esi, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 18h]
        mov    word ptr [ebx + 0ah], 5305h
        add    esi, dword ptr [ecx]
        mov    dword ptr [ecx], esi
        mov    word ptr [ebx + 22h], 0
        mov    ecx, dword ptr [ebp + 14h]
        lea    edx, [eax + 4ah]
        sub    cx, si
        mov    word ptr [ebx + 28h], cx
        mov    dword ptr [ebx + 24h], edx
        db  0C7h, 043h, 004h, 000h, 000h, 000h, 000h
        mov    word ptr [ebx + 22h], 1
        mov    dword ptr [ebx + 40h], eax
        sti
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_1B40:
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1B46:
        add    esp, 4
        xor    ebx, ebx
loc_5_1B4B:
        sti
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
_PTEXT ends
        end
