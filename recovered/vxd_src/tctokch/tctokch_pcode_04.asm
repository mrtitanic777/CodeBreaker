; tctokch_pcode_04.asm -- TCTOKCH object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 14h]
        cmp    byte ptr [eax + 127h], 0
        je     SHORT loc_4_003A
        mov    dword ptr [edi], 0
        push   0ffffffh
        push   0
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        mov    esi, dword ptr [ebp + 10h]
        push   esi
        db  0E8h, 0D8h, 005h, 0FCh, 0FFh
        or     eax, eax
        je     SHORT loc_4_0070
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
loc_4_003A:
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        push   esi
        push   0
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        mov    eax, dword ptr [eax + 0f8h]
        push   eax
        db  0E8h, 071h, 005h, 0FCh, 0FFh
        cmp    dword ptr [esi], 0
        jne    SHORT loc_4_0060
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
loc_4_0060:
        cmp    dword ptr [edi], 0ffffffh
        jbe    SHORT loc_4_0070
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
loc_4_0070:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_0080:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 16bh]
        push   eax
        lea    eax, [esi + 167h]
        push   eax
        push   6
        push   esi
        call   0
        or     al, al
        jne    SHORT loc_4_00B1
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_00B1:
        lea    eax, [esi + 177h]
        push   eax
        lea    eax, [esi + 173h]
        push   eax
        push   8
        push   esi
        call   0
        or     al, al
        jne    SHORT loc_4_00D6
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_00D6:
        lea    eax, [esi + 187h]
        push   eax
        lea    eax, [esi + 183h]
        push   eax
        push   12h
        push   esi
        call   0
        or     al, al
        jne    SHORT loc_4_0100
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_4_0100:
        lea    eax, [esi + 18fh]
        push   eax
        lea    eax, [esi + 18bh]
        push   eax
        push   0eh
        push   esi
        call   0
        or     al, al
        jne    SHORT loc_4_0125
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_0125:
        mov    eax, dword ptr [esi + 13ch]
        mov    dword ptr [esi + 19fh], eax
        lea    edi, [esi + 197h]
        lea    ebx, [esi + 193h]
        push   edi
        push   ebx
        push   eax
        push   esi
        call   0
        or     al, al
        jne    SHORT loc_4_0155
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_0155:
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi + 17bh], eax
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 17fh], eax
        lea    eax, [esi + 7eh]
        push   eax
        lea    eax, [esi + 7ah]
        push   eax
        mov    eax, dword ptr [esi + 76h]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        add    eax, ecx
        push   eax
        push   esi
        call   0
        or     al, al
        jne    SHORT loc_4_0190
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_0190:
        push   0
        mov    eax, dword ptr [esi + 76h]
        shl    eax, 2
        push   0
        lea    eax, [eax + eax*8]
        push   eax
        lea    eax, [esi + 6ah]
        push   eax
        db  0E8h, 05Fh, 004h, 0FCh, 0FFh
        or     eax, eax
        je     SHORT loc_4_01B6
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_01B6:
        mov    ebx, dword ptr [esi + 82h]
        imul   ebx, dword ptr [esi + 86h]
        imul   ebx, dword ptr [esi + 76h]
        lea    eax, [esi + 8eh]
        push   eax
        lea    eax, [esi + 8ah]
        push   eax
        push   ebx
        push   esi
        call   0
        or     al, al
        jne    SHORT loc_4_01F0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_4_01F0:
        mov    edx, dword ptr [esi + 76h]
        shl    edx, 2
        mov    edi, dword ptr [esi + 6ah]
        lea    edx, [edx + edx*8]
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        mov    edx, dword ptr [esi + 76h]
        mov    eax, edx
        lea    edx, [edx + edx*4]
        mov    edi, dword ptr [esi + 7ah]
        lea    edx, [edx + edx*4]
        add    edx, eax
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        mov    edi, dword ptr [esi + 8ah]
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd
        mov    ecx, ebx
        and    ecx, 3
        rep stosb
        mov    edx, dword ptr [esi + 6ah]
        mov    dword ptr [ebp - 8], edx
        mov    eax, dword ptr [esi + 7ah]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [esi + 7eh]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [esi + 8ah]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [esi + 8eh]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 0ch], 0
        cmp    dword ptr [esi + 76h], 0
        jbe    NEAR PTR loc_4_04D2
loc_4_0275:
        mov    edx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [edx + 8], eax
        mov    eax, edx
        add    eax, 4
        mov    dword ptr [ebp - 4], eax
        mov    ecx, dword ptr [ebp - 20h]
        mov    dword ptr [eax], ecx
        sub    ecx, ecx
        cmp    dword ptr [esi + 86h], ecx
        jbe    NEAR PTR loc_4_038A
        add    edx, 0ch
        sub    eax, eax
        mov    edi, edi
loc_4_02A0:
        mov    edi, dword ptr [esi + 86h]
        sub    edi, ecx
        cmp    edi, 1
        jne    SHORT loc_4_02B6
        mov    bx, word ptr [esi + 82h]
        jmp    SHORT loc_4_02C1
loc_4_02B6:
        mov    ebx, dword ptr [esi + 82h]
        or     bx, 8000h
loc_4_02C1:
        mov    di, bx
        and    edi, 0ffff00ffh
        shl    di, 8
        mov    bl, bh
        and    ebx, 0ffff00ffh
        add    di, bx
        mov    ebx, dword ptr [ebp - 20h]
        mov    word ptr [eax + ebx + 8], di
        cmp    byte ptr [esi + 127h], 0
        je     SHORT loc_4_0310
        mov    ebx, dword ptr [ebp - 18h]
        shr    ebx, 10h
        movzx  di, bh
        and    bx, 0ffh
        shl    bx, 8
        add    di, bx
        mov    ebx, dword ptr [ebp - 20h]
        mov    word ptr [eax + ebx + 0ah], di
        mov    bx, word ptr [ebp - 18h]
        jmp    SHORT loc_4_033A
        db  08Bh, 0FFh
loc_4_0310:
        mov    ebx, dword ptr [ebp - 14h]
        shr    ebx, 10h
        mov    di, bx
        and    edi, 0ffff00ffh
        shl    di, 8
        mov    bl, bh
        and    ebx, 0ffff00ffh
        add    di, bx
        mov    ebx, dword ptr [ebp - 20h]
        mov    word ptr [eax + ebx + 0ah], di
        mov    bx, word ptr [ebp - 14h]
loc_4_033A:
        mov    di, bx
        and    edi, 0ffff00ffh
        shl    di, 8
        mov    bl, bh
        and    ebx, 0ffff00ffh
        add    di, bx
        mov    ebx, dword ptr [ebp - 20h]
        mov    word ptr [eax + ebx + 0ch], di
        mov    edi, dword ptr [ebp - 18h]
        mov    dword ptr [edx], edi
        add    edx, 8
        mov    edi, dword ptr [ebp - 14h]
        mov    dword ptr [edx - 4], edi
        mov    edi, dword ptr [esi + 82h]
        add    dword ptr [ebp - 18h], edi
        add    eax, 6
        mov    edi, dword ptr [esi + 82h]
        add    dword ptr [ebp - 14h], edi
        inc    ecx
        cmp    dword ptr [esi + 86h], ecx
        ja     NEAR PTR loc_4_02A0
loc_4_038A:
        mov    eax, dword ptr [esi + 76h]
        dec    eax
        cmp    eax, dword ptr [ebp - 0ch]
        jbe    NEAR PTR loc_4_0444
        mov    edx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [edx + 8], eax
        add    dword ptr [ebp - 1ch], 1ah
        add    dword ptr [ebp - 20h], 1ah
        cmp    byte ptr [esi + 127h], 0
        je     SHORT loc_4_03F0
        mov    eax, dword ptr [ebp - 20h]
        shr    eax, 10h
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    word ptr [eax], cx
        mov    ax, word ptr [ebp - 20h]
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        jmp    SHORT loc_4_0428
        db  08Bh, 0FFh
loc_4_03F0:
        mov    eax, dword ptr [ebp - 1ch]
        shr    eax, 10h
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    word ptr [eax], cx
        mov    ax, word ptr [ebp - 1ch]
        mov    cl, ah
        and    ecx, 0ffff00ffh
        and    ax, 0ffh
        shl    ax, 8
loc_4_0428:
        add    cx, ax
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    word ptr [eax + 2], cx
        mov    eax, dword ptr [ebp - 8]
        add    eax, 24h
        mov    dword ptr [edx], eax
        mov    dword ptr [ebp - 8], eax
        jmp    SHORT loc_4_04C3
loc_4_0444:
        cmp    byte ptr [esi + 127h], 0
        je     SHORT loc_4_0477
        mov    eax, dword ptr [esi + 7ah]
        shr    eax, 10h
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    word ptr [eax], cx
        mov    ax, word ptr [esi + 7ah]
        jmp    SHORT loc_4_049C
loc_4_0477:
        mov    eax, dword ptr [esi + 7eh]
        shr    eax, 10h
        mov    cl, ah
        and    ecx, 0ffff00ffh
        and    ax, 0ffh
        shl    ax, 8
        add    cx, ax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    word ptr [eax], cx
        mov    ax, word ptr [esi + 7eh]
loc_4_049C:
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    word ptr [eax + 2], cx
        mov    eax, dword ptr [esi + 6ah]
        mov    dword ptr [edx], eax
loc_4_04C3:
        inc    dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [esi + 76h], eax
        ja     NEAR PTR loc_4_0275
loc_4_04D2:
        lea    eax, [esi + 62h]
        push   eax
        lea    eax, [esi + 5eh]
        push   eax
        mov    eax, dword ptr [esi + 56h]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        add    eax, ecx
        push   eax
        push   esi
        call   0
        or     al, al
        jne    SHORT loc_4_0500
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_4_0500:
        push   0
        mov    eax, dword ptr [esi + 56h]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        push   0
        lea    eax, [eax + eax*8]
        sub    eax, ecx
        push   eax
        lea    eax, [esi + 42h]
        push   eax
        db  0E8h, 0EBh, 000h, 0FCh, 0FFh
        or     eax, eax
        je     SHORT loc_4_052A
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_052A:
        mov    edx, dword ptr [esi + 56h]
        mov    eax, edx
        lea    edx, [edx + edx*4]
        mov    edi, dword ptr [esi + 42h]
        lea    edx, [edx + edx*8]
        sub    edx, eax
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        mov    edx, dword ptr [esi + 56h]
        mov    eax, edx
        lea    edx, [edx + edx*4]
        mov    edi, dword ptr [esi + 5eh]
        lea    edx, [edx + edx*4]
        add    edx, eax
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        mov    edx, dword ptr [esi + 42h]
        mov    dword ptr [esi + 4eh], edx
        mov    dword ptr [esi + 46h], edx
        mov    eax, dword ptr [esi + 56h]
        mov    edi, 1ah
        mov    dword ptr [esi + 52h], eax
        mov    eax, dword ptr [esi + 5eh]
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [esi + 62h]
        mov    dword ptr [ebp - 24h], eax
        sub    eax, eax
loc_4_058C:
        mov    ecx, dword ptr [ebp - 28h]
        mov    dword ptr [edx + 0ch], ecx
        cmp    byte ptr [esi + 127h], 0
        jne    SHORT loc_4_05B9
        mov    ecx, dword ptr [ebp - 24h]
        mov    dword ptr [edx + 10h], ecx
        sub    ecx, ecx
        cmp    dword ptr [esi + 5ah], ecx
        jbe    SHORT loc_4_05BC
        lea    ebx, [edx + 14h]
loc_4_05AB:
        mov    dword ptr [ebx], eax
        inc    eax
        add    ebx, 8
        inc    ecx
        cmp    dword ptr [esi + 5ah], ecx
        ja     SHORT loc_4_05AB
        jmp    SHORT loc_4_05BC
loc_4_05B9:
        add    eax, dword ptr [esi + 5ah]
loc_4_05BC:
        mov    ecx, dword ptr [esi + 5ah]
        imul   ecx, dword ptr [esi + 56h]
        cmp    ecx, eax
        jbe    NEAR PTR loc_4_0655
        add    dword ptr [ebp - 28h], edi
        add    dword ptr [ebp - 24h], edi
        cmp    byte ptr [esi + 127h], 0
        je     SHORT loc_4_0603
        mov    ecx, dword ptr [ebp - 28h]
        shr    ecx, 10h
        mov    bl, cl
        and    ebx, 0ffff00ffh
        shl    bx, 8
        mov    cl, ch
        and    ecx, 0ffff00ffh
        add    bx, cx
        mov    ecx, dword ptr [edx + 0ch]
        mov    word ptr [ecx], bx
        mov    cx, word ptr [ebp - 28h]
        jmp    SHORT loc_4_062A
loc_4_0603:
        mov    ecx, dword ptr [ebp - 24h]
        shr    ecx, 10h
        mov    bl, cl
        and    ebx, 0ffff00ffh
        shl    bx, 8
        mov    cl, ch
        and    ecx, 0ffff00ffh
        add    bx, cx
        mov    ecx, dword ptr [edx + 0ch]
        mov    word ptr [ecx], bx
        mov    cx, word ptr [ebp - 24h]
loc_4_062A:
        mov    bl, cl
        and    ebx, 0ffff00ffh
        shl    bx, 8
        mov    cl, ch
        and    ecx, 0ffff00ffh
        add    bx, cx
        mov    ecx, dword ptr [edx + 0ch]
        mov    word ptr [ecx + 2], bx
        lea    ecx, [edx + 2ch]
        mov    dword ptr [edx + 8], ecx
        mov    edx, ecx
        jmp    NEAR PTR loc_4_058C
loc_4_0655:
        cmp    byte ptr [esi + 127h], 0
        je     SHORT loc_4_0686
        mov    eax, dword ptr [esi + 5eh]
        shr    eax, 10h
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [edx + 0ch]
        mov    word ptr [eax], cx
        mov    ax, word ptr [esi + 5eh]
        jmp    SHORT loc_4_06AC
loc_4_0686:
        mov    eax, dword ptr [esi + 62h]
        shr    eax, 10h
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [edx + 0ch]
        mov    word ptr [eax], cx
        mov    ax, word ptr [esi + 62h]
loc_4_06AC:
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [edx + 0ch]
        lea    ebx, [esi + 1ah]
        mov    word ptr [eax + 2], cx
        mov    eax, dword ptr [esi + 42h]
        mov    dword ptr [edx + 8], eax
        push   0
        push   0
        push   8ch
        push   ebx
        db  0E8h, 025h, 0FFh, 0FBh, 0FFh
        or     eax, eax
        je     SHORT loc_4_06F0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_06F0:
        mov    edi, dword ptr [ebx]
        sub    eax, eax
        mov    ecx, 23h
        rep stosd
        mov    edx, dword ptr [ebx]
        mov    eax, 9
loc_4_0702:
        lea    ecx, [edx + 0eh]
        mov    dword ptr [edx], ecx
        mov    edx, ecx
        dec    eax
        jne    SHORT loc_4_0702
        mov    eax, dword ptr [ebx]
        mov    dword ptr [edx - 0eh], eax
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi + 22h], eax
        mov    dword ptr [esi + 1eh], eax
        mov    eax, dword ptr [esi + 173h]
        mov    dword ptr [eax], 0
        mov    ecx, 8
        mov    word ptr [eax + 4], 0
        mov    eax, dword ptr [esi + 173h]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        sub    eax, eax
        mov    edi, dword ptr [esi + 17bh]
        rep stosd
        mov    eax, dword ptr [esi + 183h]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0ch], 0
        mov    word ptr [eax + 10h], 0
        mov    eax, dword ptr [esi + 18bh]
        pop    edi
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    word ptr [eax + 0ch], 0
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_4_07A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 44h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 091h, 0FDh, 0FBh, 0FFh
        push   0
        push   0
        push   8
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 03Eh, 0FEh, 0FBh, 0FFh
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    NEAR PTR loc_4_0890
        mov    ecx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    byte ptr [ebp - 44h], 3
        mov    byte ptr [ebp - 43h], 0ah
        db  0C7h, 045h, 0C2h, 0A0h, 008h, 000h, 000h
        db  0C7h, 045h, 0C6h, 0B0h, 012h, 000h, 000h
        db  0C7h, 045h, 0CAh, 0E0h, 03Ch, 000h, 000h
        db  0C7h, 045h, 0CEh, 080h, 027h, 000h, 000h
        db  0C7h, 045h, 0D2h, 040h, 014h, 000h, 000h
        db  0C7h, 045h, 0D6h, 010h, 02Ah, 000h, 000h
        db  0C7h, 045h, 0DAh, 0B0h, 02Ah, 000h, 000h
        db  0C7h, 045h, 0DEh, 0D0h, 018h, 000h, 000h
        db  0C7h, 045h, 0E2h, 070h, 00Ah, 000h, 000h
        db  0C7h, 045h, 0E6h, 050h, 012h, 000h, 000h
        db  0A1h, 040h, 045h, 000h, 000h
        db  08Bh, 00Dh, 044h, 045h, 000h, 000h
        mov    dword ptr [ebp - 16h], eax
        lea    eax, [ebp - 44h]
        mov    dword ptr [ebp - 12h], ecx
        push   36h
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 4
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 0FEh, 0FCh, 0FBh, 0FFh
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_0870
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_4_0870:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 0D5h, 0FCh, 0FBh, 0FFh
        mov    dword ptr [ebp - 4], 0c0000001h
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_4_0890:
        mov    dword ptr [ebp - 4], 0c000009ah
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_08A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    eax, dword ptr [ebp + 8]
        sub    edx, edx
        mov    dword ptr [eax], 0
        mov    ecx, dword ptr [ebp + 18h]
        cmp    ecx, edx
        je     SHORT loc_4_08D1
        mov    eax, dword ptr [ebp + 14h]
        lea    esp, [esp]
loc_4_08C0:
        cmp    dword ptr [eax], 1
        je     SHORT loc_4_08CD
        add    eax, 4
        inc    edx
        cmp    ecx, edx
        ja     SHORT loc_4_08C0
loc_4_08CD:
        cmp    ecx, edx
        jne    SHORT loc_4_08E0
loc_4_08D1:
        mov    eax, 0c0010019h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 049h, 000h
loc_4_08E0:
        mov    eax, dword ptr [ebp + 10h]
        push   0
        mov    dword ptr [eax], edx
        push   0
        lea    eax, [ebp - 4]
        push   92h
        push   eax
        db  0E8h, 00Fh, 0FDh, 0FBh, 0FFh
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        jne    NEAR PTR loc_4_0A40
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp + 20h]
        mov    dword ptr [eax + 8], esi
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 18h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [eax + 18h]
        mov    byte ptr [eax + 19h], cl
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0ch
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 23dh]
        push   eax
        db  0E8h, 086h, 0F7h, 0FBh, 0FFh
        or     al, al
        je     NEAR PTR loc_4_0A05
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [esi + 0bfh]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 0c3h]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [esi + 0c3h]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0c3h], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 14h]
        cmp    byte ptr [esi + 11dh], 0
        je     SHORT loc_4_09F4
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 18h], 1
        mov    dword ptr [ebp - 8], 103h
        cmp    byte ptr [esi + 11bh], 0
        jne    SHORT loc_4_09E3
        mov    byte ptr [esi + 11bh], 1
        mov    word ptr [esi + 0b1h], 3
        push   esi
        db  0E8h, 067h, 0FFh, 0FBh, 0FFh
        mov    eax, dword ptr [ebp - 8]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_4_09E3:
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp - 8]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_4_09F4:
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp - 8]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_4_0A05:
        push   0
        push   0aa000002h
        mov    eax, dword ptr [esi + 0f8h]
        push   eax
        db  0E8h, 0C0h, 0FCh, 0FBh, 0FFh
        add    esp, 0ch
        push   0
        push   92h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 0E1h, 0FBh, 0FBh, 0FFh
        mov    dword ptr [ebp - 8], 0c0000001h
        mov    eax, dword ptr [ebp - 8]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 064h, 024h, 000h
loc_4_0A40:
        push   0
        mov    esi, dword ptr [ebp + 20h]
        push   0aa000002h
        mov    eax, dword ptr [esi + 0f8h]
        push   eax
        db  0E8h, 082h, 0FCh, 0FBh, 0FFh
        add    esp, 0ch
        mov    dword ptr [ebp - 8], 0c000009ah
        mov    eax, dword ptr [ebp - 8]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_0A70:
        push   ebp
        mov    ebp, esp
        sub    esp, 68h
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    word ptr [ebp - 68h], 8
        mov    word ptr [ebp - 66h], 8
        db  0C7h, 045h, 09Ch, 03Ch, 043h, 000h, 000h
        mov    word ptr [ebp - 40h], 6
        mov    word ptr [ebp - 3eh], 6
        db  0C7h, 045h, 0C4h, 04Ch, 043h, 000h, 000h
        mov    word ptr [ebp - 60h], 0bh
        mov    word ptr [ebp - 5eh], 0bh
        db  0C7h, 045h, 0A4h, 060h, 043h, 000h, 000h
        mov    word ptr [ebp - 48h], 0bh
        mov    word ptr [ebp - 46h], 0bh
        db  0C7h, 045h, 0BCh, 078h, 043h, 000h, 000h
        mov    word ptr [ebp - 58h], 0ah
        mov    word ptr [ebp - 56h], 0ah
        db  0C7h, 045h, 0ACh, 090h, 043h, 000h, 000h
        mov    word ptr [ebp - 50h], 7
        mov    word ptr [ebp - 4eh], 7
        db  0C7h, 045h, 0B4h, 0A4h, 043h, 000h, 000h
        mov    word ptr [ebp - 38h], 0bh
        mov    word ptr [ebp - 36h], 0bh
        db  0C7h, 045h, 0CCh, 0B8h, 043h, 000h, 000h
        mov    word ptr [ebp - 30h], 0bh
        mov    word ptr [ebp - 2eh], 0bh
        db  0C7h, 045h, 0D4h, 0D0h, 043h, 000h, 000h
        mov    word ptr [ebp - 28h], 0ah
        mov    word ptr [ebp - 26h], 0ah
        db  0C7h, 045h, 0DCh, 0E8h, 043h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 049h, 0FBh, 0FBh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_4_10E9
        push   0
        push   0
        push   241h
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 0AFh, 0FAh, 0FBh, 0FFh
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        jne    NEAR PTR loc_4_10E0
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 90h
        rep stosd
        stosb
        push   eax
        lea    eax, [ebp - 68h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 005h, 0FBh, 0FBh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_4_10C4
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 134h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 134h]
        cmp    eax, 3
        jne    NEAR PTR loc_4_0CB0
        push   0
        lea    eax, [ebp - 60h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 0C3h, 0FAh, 0FBh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_4_10C4
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 138h], eax
        lea    eax, [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 138h
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 0B6h, 0F9h, 0FBh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_4_10C4
        cmp    word ptr [ebp - 18h], 51h
        jne    NEAR PTR loc_4_0CA0
        mov    byte ptr [ebp - 20h], 9
        mov    byte ptr [ebp - 1fh], 0ah
        mov    byte ptr [ebp - 1eh], 0bh
        mov    byte ptr [ebp - 1dh], 0ch
        mov    byte ptr [ebp - 1ch], 0eh
        mov    byte ptr [ebp - 1bh], 0fh
        mov    byte ptr [ebp - 1ah], 3
        mov    byte ptr [ebp - 19h], 4
        mov    al, byte ptr [ebp - 15h]
        and    eax, 7
        mov    al, byte ptr [ebp + eax - 20h]
        mov    ecx, dword ptr [ebp - 4]
        and    eax, 0ffh
        mov    dword ptr [ecx + 128h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 12ch], 2
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 123h], 0
        mov    al, byte ptr [ebp - 16h]
        and    al, 0eh
        shr    al, 1
        and    eax, 0ffh
        shl    eax, 0ch
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 1a20h
        mov    dword ptr [ecx + 11fh], eax
        mov    al, byte ptr [ebp - 15h]
        and    al, 0f0h
        cmp    al, 0f0h
        mov    eax, dword ptr [ebp - 4]
        jne    SHORT loc_4_0C90
        jmp    NEAR PTR loc_4_0D93
        db  090h
loc_4_0C90:
        mov    byte ptr [eax + 127h], 0
        jmp    NEAR PTR loc_4_0D9A
        db  08Dh, 064h, 024h, 000h
loc_4_0CA0:
        mov    dword ptr [ebp - 8], 0c0000001h
        jmp    NEAR PTR loc_4_0D9A
        db  08Dh, 064h, 024h, 000h
loc_4_0CB0:
        cmp    eax, 1
        je     SHORT loc_4_0CD8
        cmp    eax, 2
        je     SHORT loc_4_0CD8
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0aa00000ah
        mov    eax, dword ptr [eax + 0f8h]
        push   eax
        db  0E8h, 008h, 0FAh, 0FBh, 0FFh
        add    esp, 0ch
        jmp    NEAR PTR loc_4_0D9A
loc_4_0CD8:
        push   0
        lea    eax, [ebp - 58h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 09Bh, 0F9h, 0FBh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_4_10C4
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 128h], eax
        mov    eax, dword ptr [ebp - 4]
        push   1
        mov    dword ptr [eax + 12ch], 1
        lea    eax, [ebp - 50h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 05Eh, 0F9h, 0FBh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_4_10C4
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 11fh], eax
        lea    eax, [ebp - 48h]
        push   0
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 02Eh, 0F9h, 0FBh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_4_10C4
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 123h], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 123h], 0
        je     SHORT loc_4_0D90
        mov    byte ptr [eax + 127h], 0
        jmp    SHORT loc_4_0D9A
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_0D90:
        mov    eax, dword ptr [ebp - 4]
loc_4_0D93:
        mov    byte ptr [eax + 127h], 1
loc_4_0D9A:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_4_10CA
        push   0
        lea    eax, [ebp - 40h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 0CFh, 0F8h, 0FBh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_4_10CA
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [eax + 4], 4
        mov    eax, dword ptr [ebp - 4]
        jne    SHORT loc_4_0DE0
        mov    dword ptr [eax + 130h], 4
        jmp    SHORT loc_4_0DEA
        db  08Dh, 049h, 000h
loc_4_0DE0:
        mov    dword ptr [eax + 130h], 10h
loc_4_0DEA:
        push   2
        lea    eax, [ebp - 38h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 089h, 0F8h, 0FBh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_4_0F2B
        mov    eax, dword ptr [ebp - 0ch]
        sub    ecx, ecx
        mov    esi, dword ptr [eax + 8]
        mov    di, word ptr [eax + 4]
        cmp    di, cx
        je     NEAR PTR loc_4_0EF6
loc_4_0E20:
        cmp    ecx, 6
        jge    NEAR PTR loc_4_0EF9
        mov    dl, byte ptr [esi]
        cmp    dl, 30h
        jl     SHORT loc_4_0E35
        cmp    dl, 39h
        jle    SHORT loc_4_0E49
loc_4_0E35:
        cmp    dl, 61h
        jl     SHORT loc_4_0E3F
        cmp    dl, 66h
        jle    SHORT loc_4_0E49
loc_4_0E3F:
        cmp    dl, 41h
        jl     SHORT loc_4_0E50
        cmp    dl, 46h
        jg     SHORT loc_4_0E50
loc_4_0E49:
        mov    eax, 1
        jmp    SHORT loc_4_0E52
loc_4_0E50:
        sub    eax, eax
loc_4_0E52:
        or     eax, eax
        je     NEAR PTR loc_4_0EF6
        mov    al, byte ptr [esi + 1]
        inc    esi
        cmp    al, 30h
        jl     SHORT loc_4_0E66
        cmp    al, 39h
        jle    SHORT loc_4_0E76
loc_4_0E66:
        cmp    al, 61h
        jl     SHORT loc_4_0E6E
        cmp    al, 66h
        jle    SHORT loc_4_0E76
loc_4_0E6E:
        cmp    al, 41h
        jl     SHORT loc_4_0E80
        cmp    al, 46h
        jg     SHORT loc_4_0E80
loc_4_0E76:
        mov    eax, 1
        jmp    SHORT loc_4_0E82
        db  08Dh, 049h, 000h
loc_4_0E80:
        sub    eax, eax
loc_4_0E82:
        or     eax, eax
        je     SHORT loc_4_0EF6
        cmp    dl, 61h
        jl     SHORT loc_4_0E95
        cmp    dl, 66h
        jg     SHORT loc_4_0E95
        sub    dl, 61h
        jmp    SHORT loc_4_0EA7
loc_4_0E95:
        cmp    dl, 41h
        jl     SHORT loc_4_0EA4
        cmp    dl, 46h
        jg     SHORT loc_4_0EA4
        sub    dl, 41h
        jmp    SHORT loc_4_0EA7
loc_4_0EA4:
        sub    dl, 30h
loc_4_0EA7:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [ecx + eax + 140h], dl
        mov    al, byte ptr [esi]
        cmp    al, 61h
        jl     SHORT loc_4_0EC3
        cmp    al, 66h
        jg     SHORT loc_4_0EC3
        movsx  eax, al
        sub    eax, 61h
        jmp    SHORT loc_4_0ED9
loc_4_0EC3:
        cmp    al, 41h
        jl     SHORT loc_4_0ED3
        cmp    al, 46h
        jg     SHORT loc_4_0ED3
        movsx  eax, al
        sub    eax, 41h
        jmp    SHORT loc_4_0ED9
loc_4_0ED3:
        movsx  eax, al
        sub    eax, 30h
loc_4_0ED9:
        mov    edx, dword ptr [ebp - 4]
        shl    al, 4
        inc    esi
        add    byte ptr [edx + ecx + 140h], al
        cmp    byte ptr [esi], 2dh
        jne    SHORT loc_4_0EED
        inc    esi
loc_4_0EED:
        inc    ecx
        dec    di
        jne    NEAR PTR loc_4_0E20
loc_4_0EF6:
        cmp    ecx, 6
loc_4_0EF9:
        jne    SHORT loc_4_0F2B
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [eax + 142h]
        or     cl, byte ptr [eax + 141h]
        or     cl, byte ptr [eax + 143h]
        or     cl, byte ptr [eax + 144h]
        or     cl, byte ptr [eax + 145h]
        or     cl, byte ptr [eax + 140h]
        je     SHORT loc_4_0F2B
        mov    byte ptr [eax + 146h], 1
loc_4_0F2B:
        push   0
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 048h, 0F7h, 0FBh, 0FFh
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 130h]
        sub    eax, 10h
        cmp    eax, 1
        sbb    ecx, ecx
        and    ecx, 3390h
        add    ecx, 1130h
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_4_0F7B
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 0eh
        ja     SHORT loc_4_0F75
        mov    eax, 0eh
loc_4_0F75:
        cmp    ecx, eax
        jb     SHORT loc_4_0F7B
        mov    ecx, eax
loc_4_0F7B:
        mov    eax, dword ptr [ebp - 4]
        push   2
        mov    dword ptr [eax + 13ch], ecx
        lea    eax, [ebp - 28h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 0EFh, 0F6h, 0FBh, 0FFh
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_4_109E
        mov    eax, dword ptr [ebp - 0ch]
        sub    ecx, ecx
        mov    esi, dword ptr [eax + 8]
        mov    di, word ptr [eax + 4]
        cmp    di, cx
        je     NEAR PTR loc_4_1090
        lea    ebx, [ebx]
loc_4_0FC0:
        cmp    ecx, 12h
        jae    NEAR PTR loc_4_1090
        mov    dl, byte ptr [esi]
        cmp    dl, 30h
        jl     SHORT loc_4_0FD5
        cmp    dl, 39h
        jle    SHORT loc_4_0FE9
loc_4_0FD5:
        cmp    dl, 61h
        jl     SHORT loc_4_0FDF
        cmp    dl, 66h
        jle    SHORT loc_4_0FE9
loc_4_0FDF:
        cmp    dl, 41h
        jl     SHORT loc_4_0FF0
        cmp    dl, 46h
        jg     SHORT loc_4_0FF0
loc_4_0FE9:
        mov    eax, 1
        jmp    SHORT loc_4_0FF2
loc_4_0FF0:
        sub    eax, eax
loc_4_0FF2:
        or     eax, eax
        je     NEAR PTR loc_4_1090
        mov    al, byte ptr [esi + 1]
        inc    esi
        cmp    al, 30h
        jl     SHORT loc_4_1006
        cmp    al, 39h
        jle    SHORT loc_4_1016
loc_4_1006:
        cmp    al, 61h
        jl     SHORT loc_4_100E
        cmp    al, 66h
        jle    SHORT loc_4_1016
loc_4_100E:
        cmp    al, 41h
        jl     SHORT loc_4_1020
        cmp    al, 46h
        jg     SHORT loc_4_1020
loc_4_1016:
        mov    eax, 1
        jmp    SHORT loc_4_1022
        db  08Dh, 049h, 000h
loc_4_1020:
        sub    eax, eax
loc_4_1022:
        or     eax, eax
        je     SHORT loc_4_1090
        cmp    dl, 61h
        jl     SHORT loc_4_1035
        cmp    dl, 66h
        jg     SHORT loc_4_1035
        sub    dl, 61h
        jmp    SHORT loc_4_1047
loc_4_1035:
        cmp    dl, 41h
        jl     SHORT loc_4_1044
        cmp    dl, 46h
        jg     SHORT loc_4_1044
        sub    dl, 41h
        jmp    SHORT loc_4_1047
loc_4_1044:
        sub    dl, 30h
loc_4_1047:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [ecx + eax + 147h], dl
        mov    al, byte ptr [esi]
        cmp    al, 61h
        jl     SHORT loc_4_1063
        cmp    al, 66h
        jg     SHORT loc_4_1063
        movsx  eax, al
        sub    eax, 61h
        jmp    SHORT loc_4_1079
loc_4_1063:
        cmp    al, 41h
        jl     SHORT loc_4_1073
        cmp    al, 46h
        jg     SHORT loc_4_1073
        movsx  eax, al
        sub    eax, 41h
        jmp    SHORT loc_4_1079
loc_4_1073:
        movsx  eax, al
        sub    eax, 30h
loc_4_1079:
        mov    edx, dword ptr [ebp - 4]
        shl    al, 4
        add    byte ptr [edx + ecx + 147h], al
        inc    esi
        inc    ecx
        dec    di
        jne    NEAR PTR loc_4_0FC0
loc_4_1090:
        or     ecx, ecx
        je     SHORT loc_4_109E
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 159h], 1
loc_4_109E:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   sub_4_1100
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     SHORT loc_4_10C4
        mov    dword ptr [ebp - 4], 0
loc_4_10C4:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_4_10E0
loc_4_10CA:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_10E0
        push   0
        push   241h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 02Ch, 0F5h, 0FBh, 0FFh
loc_4_10E0:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 0A7h, 0F5h, 0FBh, 0FFh
loc_4_10E9:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1100:
        push   ebp
        mov    ebp, esp
        sub    esp, 38h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        lea    eax, [esi + 0b7h]
        mov    dword ptr [esi + 0bbh], eax
        mov    dword ptr [eax], eax
        lea    eax, [esi + 0bfh]
        mov    dword ptr [esi + 0c3h], eax
        mov    dword ptr [eax], eax
        lea    eax, [esi + 0c7h]
        mov    dword ptr [esi + 0cbh], eax
        mov    dword ptr [eax], eax
        lea    eax, [esi + 0cfh]
        mov    dword ptr [esi + 0d3h], eax
        mov    dword ptr [eax], eax
        mov    byte ptr [esi + 11bh], 0
        mov    byte ptr [esi + 11dh], 1
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0fch], edx
        lea    edi, [ebp - 38h]
        mov    dword ptr [esi + 56h], 0ah
        mov    dword ptr [esi + 5ah], 3
        mov    dword ptr [esi + 76h], 0fh
        mov    dword ptr [esi + 86h], 3
        sub    eax, eax
        mov    dword ptr [esi + 82h], 3f8h
        mov    word ptr [esi + 100h], 400h
        mov    ecx, 9
        rep stosd
        stosw
        mov    dword ptr [ebp - 22h], 1
        mov    eax, dword ptr [esi + 11fh]
        mov    dword ptr [ebp - 1eh], eax
        mov    dword ptr [ebp - 1ah], 0fh
        mov    eax, dword ptr [esi + 134h]
        cmp    eax, 1
        jl     SHORT loc_4_11F8
        cmp    eax, 2
        jle    SHORT loc_4_11D0
        cmp    eax, 3
        je     SHORT loc_4_11E6
        jmp    SHORT loc_4_11F8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_11D0:
        mov    byte ptr [ebp - 33h], 0
        mov    eax, dword ptr [esi + 123h]
        mov    dword ptr [ebp - 38h], eax
        mov    dword ptr [ebp - 2eh], 1
        jmp    SHORT loc_4_11F8
loc_4_11E6:
        mov    byte ptr [ebp - 33h], 1
        mov    dword ptr [ebp - 38h], 0
        mov    dword ptr [ebp - 2eh], 3
loc_4_11F8:
        cmp    byte ptr [esi + 127h], 0
        je     SHORT loc_4_1220
        mov    byte ptr [ebp - 34h], 0
        mov    dword ptr [ebp - 38h], 0
        mov    dword ptr [ebp - 2ah], 0
        mov    dword ptr [ebp - 26h], 0
        jmp    SHORT loc_4_1237
        db  08Dh, 064h, 024h, 000h
loc_4_1220:
        mov    byte ptr [ebp - 34h], 1
        mov    eax, dword ptr [esi + 13ch]
        mov    dword ptr [ebp - 26h], eax
        mov    eax, dword ptr [esi + 5ah]
        imul   eax, dword ptr [esi + 56h]
        mov    dword ptr [ebp - 2ah], eax
loc_4_1237:
        db  0C7h, 045h, 0CEh, 090h, 014h, 000h, 000h
        lea    eax, [ebp - 38h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        lea    edi, [esi + 0f8h]
        push   eax
        push   esi
        push   edx
        push   edi
        db  0E8h, 00Ah, 0F3h, 0FBh, 0FFh
        mov    dword ptr [ebp - 10h], eax
        or     eax, eax
        jne    NEAR PTR loc_4_147E
        mov    byte ptr [esi + 0e8h], 1
        push   esi
        call   sub_4_0080
        or     al, al
        je     NEAR PTR loc_4_1461
        mov    byte ptr [esi + 23ch], 1
        mov    eax, dword ptr [esi + 12ch]
        push   eax
        push   0
        mov    eax, dword ptr [esi + 128h]
        push   eax
        push   eax
        db  068h, 070h, 01Dh, 000h, 000h
        push   esi
        db  068h, 090h, 01Ch, 000h, 000h
        mov    eax, dword ptr [edi]
        push   eax
        lea    eax, [ebp - 10h]
        push   esi
        push   eax
        db  0E8h, 046h, 0F3h, 0FBh, 0FFh
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_4_1449
        mov    byte ptr [esi + 18h], 1
        push   esi
        call   sub_4_1830
        or     al, al
        je     NEAR PTR loc_4_1440
        cmp    byte ptr [esi + 0eah], 1
        je     SHORT loc_4_1337
        mov    word ptr [ebp - 0ah], 0a00h
        mov    cx, word ptr [esi + 11fh]
        mov    al, byte ptr [ebp - 0ah]
        lea    edx, [ecx + 0ch]
        out    dx, al
        lea    edx, [ecx + 0dh]
        mov    al, byte ptr [ebp - 9]
        mov    word ptr [ebp - 8], 1
        mov    cx, word ptr [esi + 11fh]
        out    dx, al
        lea    edx, [ecx + 4]
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        lea    edx, [ecx + 5]
        mov    al, byte ptr [ebp - 7]
        out    dx, al
        sub    ecx, ecx
loc_4_1305:
        mov    edx, dword ptr [esi + 11fh]
        add    dx, 2
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [esi + 11fh]
        add    dx, 3
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    ax, word ptr [ebp - 2]
        mov    byte ptr [esi + ecx + 15bh], ah
        inc    ecx
        cmp    ecx, 6
        jb     SHORT loc_4_1305
        jmp    NEAR PTR loc_4_13BB
loc_4_1337:
        mov    word ptr [ebp - 6], 0a00h
        mov    cx, word ptr [esi + 11fh]
        mov    al, byte ptr [ebp - 6]
        lea    edx, [ecx + 4]
        out    dx, al
        lea    edx, [ecx + 5]
        mov    al, byte ptr [ebp - 5]
        out    dx, al
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [esi + 11fh]
        inc    dx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [ebp - 4], ax
        mov    cx, word ptr [esi + 11fh]
        lea    edx, [ecx + 4]
        out    dx, al
        lea    edx, [ecx + 5]
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        sub    ecx, ecx
loc_4_1381:
        mov    edx, dword ptr [esi + 11fh]
        add    dx, 2
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [esi + 11fh]
        add    dx, 3
        in     al, dx
        mov    byte ptr [ebp - 1], al
        lea    edx, [esi + ecx]
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [edx + 15ch], al
        mov    ax, word ptr [ebp - 2]
        mov    byte ptr [edx + 15bh], ah
        add    ecx, 2
        cmp    ecx, 6
        jb     SHORT loc_4_1381
loc_4_13BB:
        cmp    byte ptr [esi + 146h], 0
        lea    ecx, [esi + 161h]
        jne    SHORT loc_4_13D2
        lea    eax, [esi + 15bh]
        jmp    SHORT loc_4_13D8
loc_4_13D2:
        lea    eax, [esi + 140h]
loc_4_13D8:
        mov    dl, byte ptr [eax]
        mov    byte ptr [ecx], dl
        mov    dl, byte ptr [eax + 1]
        mov    byte ptr [ecx + 1], dl
        mov    dl, byte ptr [eax + 2]
        mov    byte ptr [ecx + 2], dl
        lea    ebx, [esi + 23dh]
        mov    dl, byte ptr [eax + 3]
        mov    byte ptr [ecx + 3], dl
        mov    dl, byte ptr [eax + 4]
        mov    byte ptr [ecx + 4], dl
        mov    al, byte ptr [eax + 5]
        mov    byte ptr [ecx + 5], al
        lea    eax, [esi + 238h]
        push   ebx
        push   eax
        push   ecx
        db  068h, 020h, 031h, 000h, 000h
        db  068h, 090h, 038h, 000h, 000h
        db  068h, 000h, 037h, 000h, 000h
        db  068h, 0B0h, 037h, 000h, 000h
        db  0E8h, 0DEh, 0EBh, 0FBh, 0FFh
        or     al, al
        je     SHORT loc_4_1431
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_1431:
        mov    dword ptr [ebx], 0
        push   2
        jmp    SHORT loc_4_1463
        db  005h, 000h, 000h, 000h, 000h
loc_4_1440:
        mov    dword ptr [ebp - 10h], 0c0000001h
        jmp    SHORT loc_4_147E
loc_4_1449:
        push   1
        push   0
        push   2
        push   0aa000005h
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 07Ch, 0F2h, 0FBh, 0FFh
        add    esp, 14h
        jmp    SHORT loc_4_147E
loc_4_1461:
        push   3
loc_4_1463:
        push   0
        push   2
        push   0aa000002h
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 064h, 0F2h, 0FBh, 0FFh
        add    esp, 14h
        mov    dword ptr [ebp - 10h], 0c000009ah
loc_4_147E:
        push   esi
        db  0E8h, 0CCh, 0FDh, 0FBh, 0FFh
        mov    eax, dword ptr [ebp - 10h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_1490:
        push   ebp
        mov    ebp, esp
        sub    esp, 5ch
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 7], al
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 6], al
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 5], al
        mov    eax, dword ptr [ecx + 134h]
        cmp    eax, 1
        je     SHORT loc_4_14F2
        cmp    eax, 2
        je     SHORT loc_4_14F2
        lea    esi, [ecx + 0eah]
        mov    byte ptr [esi], 3
        jmp    NEAR PTR loc_4_17B9
loc_4_14F2:
        mov    si, 64h
loc_4_14F6:
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 3], al
        mov    ax, word ptr [ebp - 4]
        and    word ptr [ebp - 8], ax
        or     word ptr [ebp - 6], ax
        dec    si
        jne    SHORT loc_4_14F6
        cmp    word ptr [ebp - 6], 0ffffh
        je     NEAR PTR loc_4_1805
        mov    ax, word ptr [ebp - 8]
        cmp    word ptr [ebp - 6], ax
        jne    NEAR PTR loc_4_1805
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 0eh
        sub    al, al
        out    dx, al
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    ax, 0ffh
        shl    ax, 8
        mov    word ptr [ebp - 4], ax
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 0eh
        mov    al, 1
        out    dx, al
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    dl, al
        and    edx, 0ffff00ffh
        or     word ptr [ebp - 4], dx
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 0fh
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    al, 4
        mov    byte ptr [ecx + 0e9h], al
        cmp    word ptr [ebp - 4], 4045h
        mov    edx, dword ptr [ecx + 11fh]
        jne    NEAR PTR loc_4_170A
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 3], al
        mov    ax, word ptr [ebp - 4]
        or     ax, 80h
        mov    word ptr [ebp - 0ah], ax
        mov    di, word ptr [ecx + 11fh]
        lea    esi, [edi + 8]
        mov    dx, si
        out    dx, al
        lea    edx, [edi + 9]
        mov    al, byte ptr [ebp - 9]
        out    dx, al
        mov    dx, si
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 3], al
        test   byte ptr [ebp - 4], 4
        je     SHORT loc_4_1606
        test   byte ptr [ebp - 3], 0e0h
        lea    esi, [ecx + 0eah]
        je     SHORT loc_4_1601
        mov    byte ptr [esi], 6
        jmp    SHORT loc_4_161A
loc_4_1601:
        mov    byte ptr [esi], 5
        jmp    SHORT loc_4_161A
loc_4_1606:
        test   byte ptr [ebp - 3], 0e0h
        lea    esi, [ecx + 0eah]
        je     SHORT loc_4_1617
        mov    byte ptr [esi], 7
        jmp    SHORT loc_4_161A
loc_4_1617:
        mov    byte ptr [esi], 4
loc_4_161A:
        mov    dword ptr [ebp - 1ch], 1
        mov    dword ptr [ebp - 18h], 5
        mov    dword ptr [ebp - 14h], 6
        mov    dword ptr [ebp - 10h], 7
        mov    dword ptr [ebp - 3ch], 0ch
        mov    dword ptr [ebp - 38h], 0bh
        mov    dword ptr [ebp - 34h], 0ah
        mov    dword ptr [ebp - 30h], 5
        mov    al, 3
        mov    dword ptr [ebp - 2ch], 7
        mov    dword ptr [ebp - 28h], 6
        mov    dword ptr [ebp - 24h], 3
        mov    dword ptr [ebp - 20h], 2
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 0eh
        out    dx, al
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    dl, al
        and    dl, 6
        shr    dl, 1
        mov    eax, dword ptr [ecx + 123h]
        or     eax, eax
        je     SHORT loc_4_16C9
        and    edx, 0ffh
        cmp    dword ptr [ebp + edx*4 - 1ch], eax
        jne    SHORT loc_4_16A5
        cmp    byte ptr [ecx + 0e9h], 0
        je     SHORT loc_4_16C9
loc_4_16A5:
        push   0
        push   0aa00000ah
        mov    eax, dword ptr [ecx + 0f8h]
        push   eax
        db  0E8h, 020h, 0F0h, 0FBh, 0FFh
        add    esp, 0ch
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_16C9:
        mov    al, byte ptr [ebp - 2]
        and    al, 38h
        shr    al, 3
        and    eax, 0ffh
        mov    edx, dword ptr [ecx + 128h]
        cmp    dword ptr [ebp + eax*4 - 3ch], edx
        je     NEAR PTR loc_4_17B9
        push   0
        push   0aa00000ah
        mov    eax, dword ptr [ecx + 0f8h]
        push   eax
        db  0E8h, 0DFh, 0EFh, 0FBh, 0FFh
        add    esp, 0ch
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_170A:
        add    dx, 0fh
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    bl, al
        and    bl, 2eh
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 0fh
        or     al, 51h
        out    dx, al
        in     al, dx
        mov    byte ptr [ebp - 1], al
        xor    byte ptr [ebp - 1], 51h
        xor    byte ptr [ebp - 1], bl
        test   byte ptr [ebp - 1], 0f7h
        je     SHORT loc_4_1762
        mov    byte ptr [ecx + 0eah], 1
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 9
        or     al, 28h
        out    dx, al
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_1762:
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 0fh
        mov    al, bl
        and    al, 28h
        out    dx, al
        in     al, dx
        mov    byte ptr [ebp - 1], al
        xor    byte ptr [ebp - 1], bl
        test   byte ptr [ebp - 1], 0f7h
        je     SHORT loc_4_17B0
        mov    byte ptr [ecx + 0eah], 1
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 9
        or     al, 28h
        out    dx, al
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h
loc_4_17B0:
        lea    esi, [ecx + 0eah]
        mov    byte ptr [esi], 2
loc_4_17B9:
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 0fh
        in     al, dx
        mov    byte ptr [ebp - 1], al
        cmp    byte ptr [esi], 4
        je     SHORT loc_4_17E0
        cmp    dword ptr [ecx + 130h], 10h
        jne    SHORT loc_4_17E0
        and    byte ptr [ebp - 1], 0dfh
        jmp    SHORT loc_4_17E4
        db  005h, 000h, 000h, 000h, 000h
loc_4_17E0:
        or     byte ptr [ebp - 1], 20h
loc_4_17E4:
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 0fh
        mov    al, byte ptr [ebp - 1]
        or     al, 80h
        out    dx, al
        mov    al, byte ptr [ebp - 1]
        or     al, 8
        out    dx, al
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_1805:
        push   0
        push   0aa000004h
        mov    eax, dword ptr [ecx + 0f8h]
        push   eax
        db  0E8h, 0C0h, 0EEh, 0FBh, 0FFh
        add    esp, 0ch
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1830:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    di, di
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, 0ffbfh
loc_4_1844:
        push   esi
        db  0E8h, 046h, 0F4h, 0FBh, 0FFh
        push   esi
        db  0E8h, 0E0h, 0F4h, 0FBh, 0FFh
        cmp    byte ptr [esi + 0eah], 1
        je     SHORT loc_4_18A7
        push   esi
        call   sub_4_1900
        or     al, al
        je     SHORT loc_4_18E0
        mov    edx, dword ptr [esi + 11fh]
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [esi + 11fh]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    word ptr [ebp - 2], bx
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [ebp - 4], ax
        mov    cx, word ptr [esi + 11fh]
        push   3e8h
        lea    edx, [ecx + 8]
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        db  0E8h, 059h, 0EDh, 0FBh, 0FFh
loc_4_18A7:
        push   esi
        db  0E8h, 0B3h, 0F5h, 0FBh, 0FFh
        or     al, al
        jne    SHORT loc_4_18F1
        inc    di
        cmp    di, 3
        jb     SHORT loc_4_1844
        push   0bh
        push   0
        push   2
        push   0aa000003h
        mov    eax, dword ptr [esi + 0f8h]
        push   eax
        db  0E8h, 008h, 0EEh, 0FBh, 0FFh
        add    esp, 14h
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_4_18E0:
        push   esi
        db  0E8h, 0AAh, 0F3h, 0FBh, 0FFh
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_18F1:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_1900:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 2ch], 0dh
        lea    eax, [ebp - 2ch]
        mov    word ptr [ebp - 2ah], 0dh
        db  0C7h, 045h, 0D8h, 004h, 044h, 000h, 000h
        push   -1
        push   eax
        lea    eax, [ebp - 24h]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        db  0E8h, 069h, 0EDh, 0FBh, 0FFh
        cmp    dword ptr [ebp - 1ch], 0
        jne    NEAR PTR loc_4_1A80
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        db  0E8h, 054h, 0EDh, 0FBh, 0FFh
        cmp    dword ptr [ebp - 1ch], 0
        jne    NEAR PTR loc_4_1A80
        mov    eax, dword ptr [ebp - 10h]
        mov    cx, word ptr [eax + 80h]
        mov    ax, word ptr [eax + 82h]
        add    dword ptr [ebp - 10h], 100h
        mov    word ptr [ebp - 0ah], ax
        mov    eax, dword ptr [ebp - 10h]
        dec    cx
        mov    dword ptr [ebp - 14h], eax
        mov    si, 0ffffh
        or     cx, cx
        je     NEAR PTR loc_4_1A60
        mov    eax, ecx
        and    eax, 0ffffh
        mov    dword ptr [ebp - 18h], eax
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, 2
loc_4_199B:
        mov    eax, dword ptr [ebp - 10h]
        mov    dx, word ptr [eax]
        add    dword ptr [ebp - 10h], ecx
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax]
        add    dword ptr [ebp - 10h], ecx
        mov    word ptr [ebp - 6], ax
        mov    eax, dword ptr [ebp - 10h]
        xor    ebx, ebx
        mov    bx, word ptr [eax]
        mov    ax, word ptr [ebp - 6]
        add    dword ptr [ebp - 10h], ecx
        add    ax, dx
        add    ax, bx
        add    si, ax
        cmp    dx, 0ffffh
        je     SHORT loc_4_1A15
        mov    word ptr [ebp - 8], dx
        mov    dx, word ptr [edi + 11fh]
        add    dx, 0ch
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        mov    dx, word ptr [edi + 11fh]
        add    dx, 0dh
        mov    al, byte ptr [ebp - 7]
        out    dx, al
        mov    ax, word ptr [ebp - 6]
        mov    word ptr [ebp - 4], ax
        mov    dx, word ptr [edi + 11fh]
        add    dx, 4
        out    dx, al
        mov    dx, word ptr [edi + 11fh]
        add    dx, 5
        mov    al, byte ptr [ebp - 3]
        out    dx, al
loc_4_1A15:
        or     ebx, ebx
        je     SHORT loc_4_1A47
loc_4_1A19:
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax]
        add    si, ax
        add    dword ptr [ebp - 10h], ecx
        mov    word ptr [ebp - 2], ax
        mov    dx, word ptr [edi + 11fh]
        add    dx, 2
        out    dx, al
        mov    dx, word ptr [edi + 11fh]
        add    dx, 3
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        dec    ebx
        jne    SHORT loc_4_1A19
loc_4_1A47:
        add    dword ptr [ebp - 14h], 100h
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 10h], eax
        dec    dword ptr [ebp - 18h]
        jne    NEAR PTR loc_4_199B
        jmp    SHORT loc_4_1A63
        db  090h
loc_4_1A60:
        mov    edi, dword ptr [ebp + 8]
loc_4_1A63:
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        db  0E8h, 042h, 0ECh, 0FBh, 0FFh
        cmp    word ptr [ebp - 0ah], si
        jne    SHORT loc_4_1A83
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_4_1A80:
        mov    edi, dword ptr [ebp + 8]
loc_4_1A83:
        push   0dh
        push   0
        push   2
        push   0aa000006h
        mov    eax, dword ptr [edi + 0f8h]
        push   eax
        db  0E8h, 03Eh, 0ECh, 0FBh, 0FFh
        add    esp, 14h
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 031h
_PTEXT ends
        end
