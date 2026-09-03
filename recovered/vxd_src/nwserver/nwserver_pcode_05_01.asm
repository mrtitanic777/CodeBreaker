; nwserver_pcode_05_01.asm -- NWSERVER object 5 segment _PTEXT  (part 1/3)
.386p
_PTEXT segment dword public use32 'PCODE'

loc_5_0000:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 18h]
        db  0BEh, 098h, 09Ah, 000h, 000h
        mov    edx, dword ptr [esp + 14h]
        mov    word ptr [eax], 80h
        lea    ebx, [edx + 8]
        mov    edi, ebx
        mov    ecx, 20h
        rep movsd
        db  0BFh, 0ACh, 0B1h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  0A1h, 020h, 0B4h, 000h, 000h
        shr    eax, 8
        and    ax, 0ffh
        db  08Bh, 00Dh, 020h, 0B4h, 000h, 000h
        shl    cx, 8
        add    ax, cx
        mov    word ptr [edx + 3ah], ax
        db  0A1h, 01Ch, 0B4h, 000h, 000h
        shr    eax, 8
        and    ax, 0ffh
        db  08Bh, 00Dh, 01Ch, 0B4h, 000h, 000h
        shl    cx, 8
        add    ax, cx
        mov    word ptr [edx + 3ch], ax
        db  0A1h, 02Ch, 0B4h, 000h, 000h
        shr    eax, 8
        and    ax, 0ffh
        db  08Bh, 00Dh, 02Ch, 0B4h, 000h, 000h
        shl    cx, 8
        add    ax, cx
        mov    word ptr [edx + 43h], ax
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_00A2:
        mov    eax, dword ptr [esp + 10h]
        cmp    byte ptr [eax + 6ah], 3
        je     SHORT loc_5_00B6
        test   byte ptr [eax + 1ah], 10h
        mov    ax, 0c6h
        je     SHORT loc_5_00B9
loc_5_00B6:
        sub    ax, ax
loc_5_00B9:
        ret
loc_5_00BA:
        mov    eax, dword ptr [esp + 10h]
        cmp    byte ptr [eax + 6ah], 3
        je     SHORT loc_5_00D0
        test   byte ptr [eax + 1ah], 10h
        jne    SHORT loc_5_00D0
        mov    ax, 0c6h
        jmp    SHORT loc_5_00DD
loc_5_00D0:
        db  0C7h, 005h, 058h, 09Fh, 000h, 000h, 001h, 000h, 000h, 000h
        sub    ax, ax
loc_5_00DD:
        ret
loc_5_00DE:
        mov    eax, dword ptr [esp + 10h]
        cmp    byte ptr [eax + 6ah], 3
        je     SHORT loc_5_00F4
        test   byte ptr [eax + 1ah], 10h
        jne    SHORT loc_5_00F4
        mov    ax, 0c6h
        jmp    SHORT loc_5_0101
loc_5_00F4:
        db  0C7h, 005h, 058h, 09Fh, 000h, 000h, 000h, 000h, 000h, 000h
        sub    ax, ax
loc_5_0101:
        ret
loc_5_0102:
        mov    eax, dword ptr [esp + 10h]
        cmp    byte ptr [eax + 6ah], 3
        je     SHORT loc_5_0116
        test   byte ptr [eax + 1ah], 10h
        mov    ax, 0c6h
        je     SHORT loc_5_0134
loc_5_0116:
        sub    ax, ax
        db  083h, 03Dh, 058h, 09Fh, 000h, 000h, 001h
        sbb    dl, dl
        inc    dl
        mov    ecx, dword ptr [esp + 8]
        mov    byte ptr [ecx + 8], dl
        mov    ecx, dword ptr [esp + 0ch]
        mov    word ptr [ecx], 1
loc_5_0134:
        ret
loc_5_0135:
        mov    eax, dword ptr [esp + 10h]
        cmp    byte ptr [eax + 6ah], 3
        je     SHORT loc_5_0149
        test   byte ptr [eax + 1ah], 10h
        mov    ax, 0ffh
        je     SHORT loc_5_014C
loc_5_0149:
        sub    ax, ax
loc_5_014C:
        ret
loc_5_014D:
        mov    eax, dword ptr [esp + 10h]
        cmp    byte ptr [eax + 6ah], 3
        je     SHORT loc_5_0163
        test   byte ptr [eax + 1ah], 10h
        jne    SHORT loc_5_0163
        mov    ax, 0c6h
        jmp    SHORT loc_5_0176
loc_5_0163:
        mov    eax, dword ptr [esp + 4]
        add    eax, 0ah
        push   eax
        db  0E8h, 0A8h, 039h, 0FBh, 0FFh
        add    esp, 4
        sub    ax, ax
loc_5_0176:
        ret
loc_5_0177:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 14h]
        mov    dword ptr [ebp - 0ch], 0ch
        test   byte ptr [edi + 1ah], 10h
        jne    SHORT loc_5_01C2
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 8], eax
        mov    esi, dword ptr [ebp + 0ch]
        add    esi, 8
        mov    dword ptr [ebp - 4], esi
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 002h, 000h, 093h, 004h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_5_01BD
        mov    ax, 0ffh
        jmp    SHORT loc_5_01CD
loc_5_01BD:
        add    edi, 62h
        movsd
        movsd
loc_5_01C2:
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [eax], 8
        sub    ax, ax
loc_5_01CD:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_5_01D3:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    dword ptr [esp + 10h], 0
        mov    eax, dword ptr [eax]
        push   dword ptr [eax + 10h]
        db  0E8h, 000h, 091h, 0FBh, 0FFh
        mov    esi, dword ptr [esp + 1ch]
        add    esp, 4
        inc    esi
        db  039h, 035h, 024h, 0B4h, 000h, 000h
        jb     SHORT loc_5_0271
        lea    ebx, [esi*4]
        mov    di, word ptr [esp + 1ch]
loc_5_020D:
        db  0A1h, 04Ch, 0B4h, 000h, 000h
        mov    ebp, dword ptr [ebx + eax]
        db  039h, 02Dh, 024h, 0B4h, 000h, 000h
        jae    SHORT loc_5_0254
        push   dword ptr [ebp + 10h]
        db  0E8h, 0D6h, 090h, 0FBh, 0FFh
        add    esp, 4
        cmp    word ptr [ebp + 18h], di
        je     SHORT loc_5_0235
        cmp    di, 0ffffh
        jne    SHORT loc_5_0249
loc_5_0235:
        lea    eax, [ebp + 72h]
        push   eax
        push   dword ptr [esp + 24h]
        db  0E8h, 0DEh, 03Bh, 0FBh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_5_0262
loc_5_0249:
        push   dword ptr [ebp + 10h]
        db  0E8h, 0A0h, 090h, 0FBh, 0FFh
        add    esp, 4
loc_5_0254:
        add    ebx, 4
        inc    esi
        db  039h, 035h, 024h, 0B4h, 000h, 000h
        jae    SHORT loc_5_020D
        jmp    SHORT loc_5_0271
loc_5_0262:
        mov    dword ptr [esp + 10h], esi
        push   dword ptr [ebp + 10h]
        db  0E8h, 083h, 090h, 0FBh, 0FFh
        add    esp, 4
loc_5_0271:
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    eax, dword ptr [eax]
        push   dword ptr [eax + 10h]
        db  0E8h, 07Bh, 090h, 0FBh, 0FFh
        mov    eax, dword ptr [esp + 14h]
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_5_028F:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        mov    esi, dword ptr [esp + 18h]
        sub    eax, eax
        mov    cx, word ptr [esi + 0ah]
        mov    word ptr [esp + 12h], cx
        mov    ebp, dword ptr [esp + 1ch]
        movzx  edx, byte ptr [esi + 0ch]
        add    esi, 0ch
        inc    edx
        lea    edi, [ebp + 120h]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
loc_5_02C7:
        lea    ecx, [ebp + 120h]
        push   ecx
        push   dword ptr [esp + 16h]
        push   eax
        call   sub_5_01D3
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_02ED
        cmp    eax, 100h
        jae    SHORT loc_5_02ED
        mov    byte ptr [ebp + ebx + 9], al
        inc    ebx
        jmp    SHORT loc_5_02C7
loc_5_02ED:
        mov    byte ptr [ebp + 8], bl
        mov    eax, dword ptr [esp + 20h]
        inc    bx
        mov    word ptr [eax], bx
        sub    ax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_5_0304:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        sub    ebx, ebx
        mov    eax, dword ptr [esi + 0ah]
        mov    bp, word ptr [esi + 0eh]
        mov    ecx, dword ptr [esp + 1ch]
        add    ecx, 1a4h
        mov    dword ptr [esp + 10h], ecx
        movzx  edx, byte ptr [esi + 10h]
        add    esi, 10h
        inc    edx
        mov    edi, ecx
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    esi, dword ptr [esp + 1ch]
        add    esi, 9
loc_5_0345:
        push   dword ptr [esp + 10h]
        push   ebp
        push   eax
        call   sub_5_01D3
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_0364
        cmp    ebx, 64h
        jae    SHORT loc_5_0364
        mov    dword ptr [esi], eax
        add    esi, 4
        inc    ebx
        jmp    SHORT loc_5_0345
loc_5_0364:
        mov    ecx, dword ptr [esp + 1ch]
        mov    eax, dword ptr [esp + 20h]
        mov    byte ptr [ecx + 8], bl
        lea    ecx, [ebx*4]
        inc    cx
        mov    word ptr [eax], cx
        sub    ax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_5_0386:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 20h]
        cmp    byte ptr [ebx + 6ah], 0
        jne    SHORT loc_5_039D
        mov    ax, 0fbh
        jmp    NEAR PTR loc_5_045D
loc_5_039D:
        mov    eax, dword ptr [esp + 14h]
        mov    cl, byte ptr [eax + 0ah]
        movzx  eax, cl
        db  039h, 005h, 024h, 0B4h, 000h, 000h
        jb     NEAR PTR loc_5_0459
        or     cl, cl
        je     NEAR PTR loc_5_0459
        db  08Bh, 00Dh, 04Ch, 0B4h, 000h, 000h
        mov    ebp, dword ptr [ecx + eax*4]
        db  03Bh, 02Dh, 024h, 0B4h, 000h, 000h
        ja     SHORT loc_5_03DF
        mov    edi, dword ptr [esp + 18h]
        add    edi, 8
        sub    eax, eax
        mov    ecx, 0fh
        rep stosd
        stosb
        jmp    SHORT loc_5_044B
loc_5_03DF:
        push   dword ptr [ebx + 10h]
        db  0E8h, 00Ah, 08Fh, 0FBh, 0FFh
        add    esp, 4
        push   dword ptr [ebp + 10h]
        db  0E8h, 009h, 08Fh, 0FBh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [ebp + 14h]
        mov    edx, dword ptr [esp + 18h]
        mov    ecx, 0ch
        mov    dword ptr [edx + 8], eax
        lea    edi, [edx + 0eh]
        sub    eax, eax
        rep stosd
        lea    esi, [ebp + 6bh]
        lea    edi, [edx + 3eh]
        movsd
        movsw
        movsb
        mov    ax, word ptr [ebp + 18h]
        lea    esi, [ebp + 73h]
        mov    word ptr [edx + 0ch], ax
        movzx  eax, byte ptr [ebp + 72h]
        lea    edi, [edx + 0eh]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        push   dword ptr [ebp + 10h]
        db  0E8h, 0B4h, 08Eh, 0FBh, 0FFh
        add    esp, 4
        push   dword ptr [ebx + 10h]
        db  0E8h, 0B3h, 08Eh, 0FBh, 0FFh
        add    esp, 4
loc_5_044B:
        mov    eax, dword ptr [esp + 1ch]
        mov    word ptr [eax], 3dh
        sub    ax, ax
        jmp    SHORT loc_5_045D
loc_5_0459:
        mov    ax, 0fdh
loc_5_045D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_0462:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 20h]
        mov    al, byte ptr [eax + 6ah]
        or     al, al
        jne    SHORT loc_5_0488
        cmp    al, 3
        je     SHORT loc_5_0488
        mov    eax, dword ptr [esp + 20h]
        test   byte ptr [eax + 1ah], 10h
        jne    SHORT loc_5_0488
        mov    ax, 0fbh
        jmp    NEAR PTR loc_5_0543
loc_5_0488:
        mov    eax, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax + 0ah]
        db  039h, 005h, 024h, 0B4h, 000h, 000h
        jb     NEAR PTR loc_5_053F
        or     eax, eax
        je     NEAR PTR loc_5_053F
        db  08Bh, 00Dh, 04Ch, 0B4h, 000h, 000h
        mov    ebx, dword ptr [ecx + eax*4]
        db  039h, 01Dh, 024h, 0B4h, 000h, 000h
        jae    NEAR PTR loc_5_053F
        mov    eax, dword ptr [esp + 20h]
        push   dword ptr [eax + 10h]
        db  0E8h, 02Dh, 08Eh, 0FBh, 0FFh
        add    esp, 4
        push   dword ptr [ebx + 10h]
        db  0E8h, 02Ch, 08Eh, 0FBh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [ebx + 14h]
        mov    ebp, dword ptr [esp + 18h]
        mov    ecx, 0ch
        mov    dword ptr [ebp + 8], eax
        lea    edx, [ebp + 0eh]
        mov    edi, edx
        sub    eax, eax
        rep stosd
        lea    esi, [ebx + 6bh]
        lea    edi, [ebp + 3eh]
        movsd
        movsw
        movsb
        mov    ax, word ptr [ebx + 18h]
        lea    esi, [ebx + 73h]
        mov    word ptr [ebp + 0ch], ax
        movzx  eax, byte ptr [ebx + 72h]
        mov    edi, edx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        push   dword ptr [ebx + 10h]
        db  0E8h, 0D6h, 08Dh, 0FBh, 0FFh
        mov    eax, dword ptr [esp + 24h]
        add    esp, 4
        push   dword ptr [eax + 10h]
        db  0E8h, 0D1h, 08Dh, 0FBh, 0FFh
        add    esp, 4
        mov    byte ptr [ebp + 45h], 0
        mov    ecx, dword ptr [esp + 1ch]
        sub    ax, ax
        mov    word ptr [ecx], 3eh
        jmp    SHORT loc_5_0543
loc_5_053F:
        mov    ax, 0fdh
loc_5_0543:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_0548:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 1ch]
        mov    ax, word ptr [esi + 0ah]
        mov    cx, ax
        shl    cx, 8
        shr    ax, 8
        add    ax, cx
        mov    ecx, dword ptr [esp + 28h]
        cmp    byte ptr [ecx + 6ah], 3
        je     SHORT loc_5_057E
        test   byte ptr [ecx + 1ah], 10h
        jne    SHORT loc_5_057E
        mov    ax, 0c6h
        jmp    NEAR PTR loc_5_06FE
loc_5_057E:
        movzx  ecx, ax
        db  039h, 00Dh, 024h, 0B4h, 000h, 000h
        jb     NEAR PTR loc_5_06FA
        or     ax, ax
        je     NEAR PTR loc_5_06FA
        db  0A1h, 04Ch, 0B4h, 000h, 000h
        mov    ebp, dword ptr [eax + ecx*4]
        db  039h, 02Dh, 024h, 0B4h, 000h, 000h
        jae    NEAR PTR loc_5_06FA
        mov    ecx, dword ptr [esp + 28h]
        push   dword ptr [ecx + 10h]
        db  0E8h, 03Bh, 08Dh, 0FBh, 0FFh
        add    esp, 4
        push   dword ptr [ebp + 10h]
        db  0E8h, 03Ah, 08Dh, 0FBh, 0FFh
        mov    eax, dword ptr [esp + 28h]
        add    esp, 4
        mov    word ptr [eax], 3
        mov    eax, dword ptr [esp + 20h]
        mov    ecx, eax
        mov    byte ptr [eax + 0ah], 0
        add    ecx, 0bh
        mov    eax, dword ptr [ebp + 44h]
        mov    dword ptr [esp + 10h], ecx
        movzx  eax, word ptr [eax - 4]
        mov    dword ptr [esp + 14h], eax
        movzx  ebx, word ptr [esi + 0ch]
        inc    ebx
        cmp    eax, ebx
        jb     NEAR PTR loc_5_06C5
loc_5_05F6:
        mov    ecx, dword ptr [ebp + 44h]
        movzx  eax, word ptr [ecx - 4]
        cmp    ebx, eax
        jbe    SHORT loc_5_0603
        jmp    SHORT loc_5_060D
loc_5_0603:
        cmp    dword ptr [ecx + ebx*4], eax
        mov    eax, 1
        ja     SHORT loc_5_060F
loc_5_060D:
        sub    eax, eax
loc_5_060F:
        or     eax, eax
        jne    SHORT loc_5_061F
        inc    ebx
        cmp    dword ptr [esp + 14h], ebx
        jae    SHORT loc_5_05F6
        jmp    NEAR PTR loc_5_06C5
loc_5_061F:
        mov    eax, dword ptr [ebp + 44h]
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + ebx*4]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp + 44h]
        mov    esi, dword ptr [eax + ebx*4]
        movzx  eax, byte ptr [esi + 8]
        add    esi, 8
loc_5_063B:
        or     eax, eax
        jle    SHORT loc_5_0654
        push   eax
        lea    eax, [esi + 1]
        push   eax
        db  0E8h, 0F7h, 010h, 0FBh, 0FFh
        add    esp, 8
        cmp    byte ptr [eax + esi + 1], 5ch
        jne    SHORT loc_5_063B
        inc    eax
loc_5_0654:
        movzx  edx, byte ptr [esi]
        sub    edx, eax
        cmp    edx, 0dh
        jl     SHORT loc_5_0663
        mov    edx, 0dh
loc_5_0663:
        mov    edi, dword ptr [esp + 10h]
        add    edi, 7
        lea    esi, [eax + esi + 1]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [esp + 10h]
        mov    byte ptr [edx + ecx + 7], 0
        mov    byte ptr [ecx + 1], 0
        mov    byte ptr [ecx + 3], 0
        mov    eax, dword ptr [ebp + 44h]
        mov    eax, dword ptr [eax + ebx*4]
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        mov    byte ptr [ecx + 4], 0
        je     SHORT loc_5_06AC
        mov    al, byte ptr [eax + 1eh]
        mov    byte ptr [ecx + 4], al
        cmp    al, 0ffh
        jne    SHORT loc_5_06AC
        mov    byte ptr [ecx + 4], 0
loc_5_06AC:
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 20h]
        mov    word ptr [ecx + 5], 0
        inc    byte ptr [eax + 0ah]
        mov    eax, dword ptr [esp + 24h]
        add    word ptr [eax], 15h
loc_5_06C5:
        cmp    dword ptr [esp + 14h], ebx
        mov    eax, dword ptr [esp + 20h]
        jb     SHORT loc_5_06D5
        mov    word ptr [eax + 8], bx
        jmp    SHORT loc_5_06DB
loc_5_06D5:
        mov    word ptr [eax + 8], 0
loc_5_06DB:
        push   dword ptr [ebp + 10h]
        db  0E8h, 00Eh, 08Ch, 0FBh, 0FFh
        mov    ecx, dword ptr [esp + 2ch]
        add    esp, 4
        push   dword ptr [ecx + 10h]
        db  0E8h, 009h, 08Ch, 0FBh, 0FFh
        add    esp, 4
        sub    ax, ax
        jmp    SHORT loc_5_06FE
loc_5_06FA:
        mov    ax, 0fdh
loc_5_06FE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_0706:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 18h]
        cmp    byte ptr [esi + 6ah], 3
        je     SHORT loc_5_071E
        test   byte ptr [esi + 1ah], 10h
        jne    SHORT loc_5_071E
        mov    ax, 0c6h
        jmp    SHORT loc_5_0788
loc_5_071E:
        mov    eax, dword ptr [esp + 0ch]
        mov    cl, byte ptr [eax + 0ah]
        movzx  eax, cl
        db  039h, 005h, 024h, 0B4h, 000h, 000h
        jb     SHORT loc_5_0784
        or     cl, cl
        je     SHORT loc_5_0784
        db  08Bh, 00Dh, 04Ch, 0B4h, 000h, 000h
        mov    edi, dword ptr [ecx + eax*4]
        db  03Bh, 03Dh, 024h, 0B4h, 000h, 000h
        jbe    SHORT loc_5_0784
        push   dword ptr [esi + 10h]
        db  0E8h, 0A4h, 08Bh, 0FBh, 0FFh
        add    esp, 4
        push   dword ptr [edi + 10h]
        db  0E8h, 0A3h, 08Bh, 0FBh, 0FFh
        add    esp, 4
        mov    byte ptr [edi + 22h], 80h
        push   dword ptr [edi + 10h]
        db  0E8h, 08Ah, 08Bh, 0FBh, 0FFh
        add    esp, 4
        push   dword ptr [esi + 10h]
        db  0E8h, 089h, 08Bh, 0FBh, 0FFh
        add    esp, 4
        push   2
        db  0E8h, 03Dh, 08Fh, 0FBh, 0FFh
        add    esp, 4
        sub    ax, ax
        jmp    SHORT loc_5_0788
loc_5_0784:
        mov    ax, 0fdh
loc_5_0788:
        pop    edi
        pop    esi
        ret
loc_5_078B:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 18h]
        cmp    byte ptr [esi + 6ah], 3
        je     SHORT loc_5_07A3
        test   byte ptr [esi + 1ah], 10h
        jne    SHORT loc_5_07A3
        mov    ax, 0c6h
        jmp    SHORT loc_5_080A
loc_5_07A3:
        mov    eax, dword ptr [esp + 0ch]
        mov    eax, dword ptr [eax + 0ah]
        db  039h, 005h, 024h, 0B4h, 000h, 000h
        jb     SHORT loc_5_0806
        or     eax, eax
        je     SHORT loc_5_0806
        db  08Bh, 00Dh, 04Ch, 0B4h, 000h, 000h
        mov    edi, dword ptr [ecx + eax*4]
        db  039h, 03Dh, 024h, 0B4h, 000h, 000h
        jae    SHORT loc_5_0806
        push   dword ptr [esi + 10h]
        db  0E8h, 022h, 08Bh, 0FBh, 0FFh
        add    esp, 4
        push   dword ptr [edi + 10h]
        db  0E8h, 021h, 08Bh, 0FBh, 0FFh
        add    esp, 4
        mov    byte ptr [edi + 22h], 80h
        push   dword ptr [edi + 10h]
        db  0E8h, 008h, 08Bh, 0FBh, 0FFh
        add    esp, 4
        push   dword ptr [esi + 10h]
        db  0E8h, 007h, 08Bh, 0FBh, 0FFh
        add    esp, 4
        push   2
        db  0E8h, 0BBh, 08Eh, 0FBh, 0FFh
        add    esp, 4
        sub    ax, ax
        jmp    SHORT loc_5_080A
loc_5_0806:
        mov    ax, 0fdh
loc_5_080A:
        pop    edi
        pop    esi
        ret
sub_5_080D:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        mov    cl, byte ptr [eax + 0ah]
        movzx  eax, cl
        db  039h, 005h, 024h, 0B4h, 000h, 000h
        jb     SHORT loc_5_0885
        or     cl, cl
        je     SHORT loc_5_0885
        db  08Bh, 00Dh, 04Ch, 0B4h, 000h, 000h
        mov    ebp, dword ptr [ecx + eax*4]
        db  039h, 02Dh, 024h, 0B4h, 000h, 000h
        jb     SHORT loc_5_083A
        jmp    SHORT loc_5_0885
loc_5_083A:
        mov    ebx, dword ptr [esp + 20h]
        push   dword ptr [ebx + 10h]
        db  0E8h, 0ABh, 08Ah, 0FBh, 0FFh
        add    esp, 4
        push   dword ptr [ebp + 10h]
        db  0E8h, 0AAh, 08Ah, 0FBh, 0FFh
        add    esp, 4
        lea    esi, [ebp + 28h]
        mov    edi, dword ptr [esp + 18h]
        add    edi, 8
        movsd
        movsd
        movsd
        push   dword ptr [ebp + 10h]
        db  0E8h, 088h, 08Ah, 0FBh, 0FFh
        add    esp, 4
        push   dword ptr [ebx + 10h]
        db  0E8h, 087h, 08Ah, 0FBh, 0FFh
        mov    ecx, dword ptr [esp + 20h]
        add    esp, 4
        mov    word ptr [ecx], 0ch
        sub    ax, ax
        jmp    SHORT loc_5_0889
loc_5_0885:
        mov    ax, 0ffh
loc_5_0889:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_088E:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 14h]
        push   dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 14h]
        push   edi
        push   esi
        push   dword ptr [esp + 18h]
        call   sub_5_080D
        add    esp, 10h
        mov    byte ptr [esi + 14h], 2
        mov    word ptr [edi], 0dh
        sub    ax, ax
        pop    edi
        pop    esi
        ret
loc_5_08B9:
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    word ptr [esp + 12h], 64h
        sub    esi, esi
        push   0feh
        push   esi
        push   esi
        push   40h
        sub    di, di
        mov    eax, dword ptr [esp + 4ch]
        add    eax, 0ch
        push   eax
        mov    eax, dword ptr [esp + 50h]
        push   1
        movzx  eax, byte ptr [eax + 0ah]
        sub    edx, edx
        lea    ecx, [esp + 44h]
        push   eax
        call   sub_5_62A6
        cmp    ax, si
        je     SHORT loc_5_0901
        mov    ax, 9ch
        jmp    NEAR PTR loc_5_0B36
loc_5_0901:
        movzx  ebp, word ptr [esp + 12h]
        push   200h
        push   ebp
        db  0E8h, 053h, 03Ah, 0FBh, 0FFh
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_5_0960
        lea    eax, [esp + 12h]
        push   eax
        push   ebp
        push   ebx
        mov    eax, dword ptr [esp + 38h]
        push   0ch
        inc    eax
        push   eax
        db  0E8h, 090h, 034h, 0FBh, 0FFh
        add    esp, 14h
        or     ax, ax
        je     SHORT loc_5_098A
        cmp    ax, 8aeh
        je     NEAR PTR loc_5_0B09
        cmp    ax, 84bh
        je     SHORT loc_5_094C
        cmp    ax, 0eah
        jne    SHORT loc_5_0969
loc_5_094C:
        or     di, di
        jne    SHORT loc_5_0981
        mov    di, 1
        push   ebx
        db  0E8h, 035h, 03Ah, 0FBh, 0FFh
        add    esp, 4
        jmp    SHORT loc_5_0901
loc_5_0960:
        mov    di, 96h
        jmp    NEAR PTR loc_5_0B0D
loc_5_0969:
        cmp    ax, 8
        jne    SHORT loc_5_0978
        mov    di, 96h
        jmp    NEAR PTR loc_5_0B0D
loc_5_0978:
        mov    di, 0ffh
        jmp    NEAR PTR loc_5_0B0D
loc_5_0981:
        mov    di, 0ffh
        jmp    NEAR PTR loc_5_0B0D
loc_5_098A:
        lea    ebp, [ebx + 8]
        push   200h
        push   18h
        db  0E8h, 0CBh, 039h, 0FBh, 0FFh
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_5_09AB
        mov    di, 96h
        jmp    NEAR PTR loc_5_0B0D
loc_5_09AB:
        mov    eax, dword ptr [esp + 3ch]
        mov    dword ptr [esp + 24h], 0
        movzx  eax, byte ptr [eax + 0bh]
        imul   eax, eax, 14h
        mov    dword ptr [esp + 30h], eax
        movsx  eax, word ptr [ebx + 6]
        mov    dword ptr [esp + 34h], eax
        mov    dword ptr [esp + 14h], 0
        mov    dword ptr [esp + 18h], 0
        mov    eax, dword ptr [esp + 30h]
        cmp    eax, dword ptr [esp + 34h]
        jae    NEAR PTR loc_5_0B09
        mov    ecx, dword ptr [esp + 40h]
        add    ecx, 59h
        mov    dword ptr [esp + 20h], ecx
        mov    eax, dword ptr [esp + 40h]
        add    eax, 9
        mov    dword ptr [esp + 1ch], eax
        lea    eax, [ebp + 4]
        mov    dword ptr [esp + 28h], eax
loc_5_0A05:
        mov    eax, dword ptr [esp + 18h]
        cmp    dword ptr [esp + 34h], eax
        jbe    NEAR PTR loc_5_0AE9
        mov    eax, dword ptr [ebp]
        mov    al, byte ptr [eax]
        cmp    al, 2ah
        jne    SHORT loc_5_0A35
        mov    byte ptr [esi + 2], 11h
        push   11h
        db  068h, 01Ch, 09Ah, 000h, 000h
        lea    eax, [esi + 3]
        push   eax
        db  0E8h, 058h, 038h, 0FBh, 0FFh
        add    esp, 0ch
        jmp    SHORT loc_5_0A87
loc_5_0A35:
        cmp    al, 5ch
        je     SHORT loc_5_0A4F
loc_5_0A39:
        mov    eax, dword ptr [ebp]
        cmp    byte ptr [eax], 0
        je     SHORT loc_5_0A4C
        inc    eax
        mov    dword ptr [ebp], eax
        cmp    byte ptr [eax], 5ch
        jne    SHORT loc_5_0A39
        jmp    SHORT loc_5_0A4F
loc_5_0A4C:
        dec    dword ptr [ebp]
loc_5_0A4F:
        mov    edi, dword ptr [ebp]
        inc    edi
        mov    dword ptr [ebp], edi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    byte ptr [esi + 2], cl
        movzx  eax, cl
        push   eax
        push   dword ptr [ebp]
        lea    eax, [esi + 3]
        push   eax
        db  0E8h, 013h, 038h, 0FBh, 0FFh
        mov    eax, dword ptr [esp + 34h]
        add    esp, 0ch
        test   byte ptr [eax + 1], 80h
        mov    word ptr [esi], 200h
        jne    SHORT loc_5_0A8C
loc_5_0A87:
        mov    word ptr [esi], 100h
loc_5_0A8C:
        push   dword ptr [esp + 1ch]
        push   esi
        call   sub_5_1CF9
        add    esp, 8
        or     ax, ax
        jne    SHORT loc_5_0AD7
        add    dword ptr [esp + 28h], 6
        inc    dword ptr [esp + 24h]
        mov    eax, dword ptr [esp + 24h]
        cmp    eax, dword ptr [esp + 30h]
        jbe    SHORT loc_5_0AD7
        mov    ax, word ptr [ebp + 4]
        and    ax, 0ffh
        push   eax
        call   sub_5_4303
        mov    ecx, dword ptr [esp + 24h]
        add    esp, 4
        mov    word ptr [ecx], ax
        add    dword ptr [esp + 20h], 2
        add    dword ptr [esp + 1ch], 4
        inc    dword ptr [esp + 14h]
loc_5_0AD7:
        add    ebp, 6
        inc    dword ptr [esp + 18h]
        cmp    dword ptr [esp + 14h], 14h
        jb     NEAR PTR loc_5_0A05
loc_5_0AE9:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_5_0B09
        mov    ecx, dword ptr [esp + 40h]
        mov    al, byte ptr [esp + 14h]
        mov    byte ptr [ecx + 8], al
        mov    eax, dword ptr [esp + 44h]
        sub    di, di
        mov    word ptr [eax], 79h
        jmp    SHORT loc_5_0B0D
loc_5_0B09:
        mov    di, 9ch
loc_5_0B0D:
        or     esi, esi
        je     SHORT loc_5_0B1A
        push   esi
        db  0E8h, 079h, 038h, 0FBh, 0FFh
        add    esp, 4
loc_5_0B1A:
        or     ebx, ebx
        je     SHORT loc_5_0B27
        push   ebx
        db  0E8h, 06Ch, 038h, 0FBh, 0FFh
        add    esp, 4
loc_5_0B27:
        push   dword ptr [esp + 2ch]
        db  0E8h, 029h, 028h, 0FBh, 0FFh
        add    esp, 4
        mov    ax, di
loc_5_0B36:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
loc_5_0B3E:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        mov    word ptr [esp + 12h], 64h
        mov    dword ptr [esp + 1ch], ebx
        mov    ebp, dword ptr [esp + 2ch]
        push   0feh
        lea    eax, [ebp + 11h]
        push   ebx
        push   ebx
        push   40h
        push   eax
        sub    edx, edx
        push   1
        movzx  eax, byte ptr [ebp + 0ah]
        lea    ecx, [esp + 30h]
        push   eax
        call   sub_5_62A6
        cmp    ax, bx
        jne    NEAR PTR loc_5_0DC1
        mov    eax, dword ptr [esp + 18h]
        cmp    byte ptr [eax], 3
        je     SHORT loc_5_0BA6
        push   eax
        db  0E8h, 0E7h, 039h, 0FBh, 0FFh
        add    esp, 4
        cmp    ax, 0ffffh
        je     SHORT loc_5_0B9A
        test   al, 10h
        jne    SHORT loc_5_0BA6
loc_5_0B9A:
        mov    word ptr [esp + 14h], 9ch
        jmp    NEAR PTR loc_5_0DA3
loc_5_0BA6:
        mov    ax, word ptr [ebp + 0fh]
        push   eax
        call   sub_5_42E4
        add    esp, 4
        movzx  ax, al
        mov    word ptr [esp + 16h], ax
        mov    eax, dword ptr [ebp + 0bh]
        mov    dword ptr [ebp + 0ah], eax
        push   dword ptr [esp + 38h]
        movzx  eax, byte ptr [ebp + 6]
        push   dword ptr [esp + 38h]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esp + 2ch], ecx
        push   dword ptr [esp + 38h]
        mov    word ptr [ebp + 7], 500h
        mov    byte ptr [ebp + 6], 17h
        mov    byte ptr [ebp + 9], 36h
        push   ebp
        call   sub_5_1AC2
        mov    word ptr [esp + 24h], ax
        mov    eax, dword ptr [esp + 40h]
        add    esp, 10h
        add    eax, 0eh
        mov    dword ptr [esp + 20h], eax
        db  0BAh, 01Ch, 09Ah, 000h, 000h
loc_5_0C04:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    SHORT loc_5_0C24
        or     cl, cl
        je     SHORT loc_5_0C20
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    SHORT loc_5_0C24
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    SHORT loc_5_0C04
loc_5_0C20:
        sub    eax, eax
        jmp    SHORT loc_5_0C29
loc_5_0C24:
        sbb    eax, eax
        sbb    eax, -1
loc_5_0C29:
        or     eax, eax
        jne    SHORT loc_5_0C42
        db  0BEh, 018h, 09Bh, 000h, 000h
        mov    edi, dword ptr [esp + 20h]
        movsw
        mov    eax, dword ptr [esp + 30h]
        mov    word ptr [eax + 0ch], 200h
loc_5_0C42:
        movzx  eax, byte ptr [ebp + 6]
        mov    ecx, dword ptr [esp + 24h]
        mov    dword ptr [eax], ecx
        cmp    word ptr [esp + 14h], 0
        jne    NEAR PTR loc_5_0DA3
        mov    eax, dword ptr [esp + 34h]
        mov    esi, dword ptr [esp + 30h]
        mov    word ptr [eax], 0
        lea    ecx, [esi + 46h]
        add    esi, 3eh
        mov    eax, dword ptr [esp + 18h]
        inc    eax
        mov    dword ptr [esi], eax
        mov    word ptr [esi + 4], 0
        mov    word ptr [esi + 6], 1
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 30h]
        cmp    word ptr [eax + 0ch], 200h
        jne    SHORT loc_5_0C9F
        cmp    word ptr [esp + 16h], 0
        je     SHORT loc_5_0C9F
        or     word ptr [esp + 16h], 8000h
loc_5_0C9F:
        mov    ax, word ptr [esp + 16h]
        mov    word ptr [ecx + 4], ax
        push   0
        push   0eh
        push   esi
        mov    eax, dword ptr [esp + 24h]
        push   0ch
        inc    eax
        push   eax
        db  0E8h, 0FEh, 030h, 0FBh, 0FFh
        mov    word ptr [esp + 28h], ax
        add    esp, 14h
        cmp    ax, 8aeh
        jne    NEAR PTR loc_5_0D94
loc_5_0CCC:
        mov    di, word ptr [esp + 12h]
        push   200h
        movzx  ebp, di
        push   ebp
        db  0E8h, 085h, 036h, 0FBh, 0FFh
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_5_0D2E
        lea    eax, [esp + 12h]
        push   eax
        push   ebp
        push   ebx
        mov    eax, dword ptr [esp + 24h]
        push   0ch
        inc    eax
        push   eax
        db  0E8h, 0C2h, 030h, 0FBh, 0FFh
        add    esp, 14h
        or     ax, ax
        je     SHORT loc_5_0D53
        cmp    ax, 84bh
        je     SHORT loc_5_0D10
        cmp    ax, 0eah
        jne    SHORT loc_5_0D37
loc_5_0D10:
        cmp    dword ptr [esp + 1ch], 0
        jne    NEAR PTR loc_5_0D9C
        mov    dword ptr [esp + 1ch], 1
        push   ebx
        db  0E8h, 067h, 036h, 0FBh, 0FFh
        add    esp, 4
        jmp    SHORT loc_5_0CCC
loc_5_0D2E:
        mov    word ptr [esp + 14h], 96h
        jmp    SHORT loc_5_0DA3
loc_5_0D37:
        cmp    ax, 8
        jne    SHORT loc_5_0D46
        mov    word ptr [esp + 14h], 96h
        jmp    SHORT loc_5_0DA3
loc_5_0D46:
        cmp    ax, 8aeh
        je     SHORT loc_5_0D4E
        jmp    SHORT loc_5_0D9C
loc_5_0D4E:
        or     ax, ax
        jne    SHORT loc_5_0D82
loc_5_0D53:
        mov    eax, dword ptr [esp + 18h]
        inc    eax
        mov    dword ptr [ebx], eax
        push   edi
        push   ebx
        push   2
        db  0E8h, 043h, 030h, 0FBh, 0FFh
        add    esp, 0ch
        or     ax, ax
        jne    SHORT loc_5_0D82
        push   0
        push   0eh
        push   esi
        mov    eax, dword ptr [esp + 24h]
        push   0ch
        inc    eax
        push   eax
        db  0E8h, 03Bh, 030h, 0FBh, 0FFh
        add    esp, 14h
        jmp    SHORT loc_5_0D8F
loc_5_0D82:
        push   0eh
        push   esi
        push   2
        db  0E8h, 01Ah, 030h, 0FBh, 0FFh
        add    esp, 0ch
loc_5_0D8F:
        mov    word ptr [esp + 14h], ax
loc_5_0D94:
        cmp    word ptr [esp + 14h], 0
        je     SHORT loc_5_0DA3
loc_5_0D9C:
        mov    word ptr [esp + 14h], 0ffh
loc_5_0DA3:
        or     ebx, ebx
        je     SHORT loc_5_0DB0
        push   ebx
        db  0E8h, 0E3h, 035h, 0FBh, 0FFh
        add    esp, 4
loc_5_0DB0:
        push   dword ptr [esp + 18h]
        db  0E8h, 0A0h, 025h, 0FBh, 0FFh
        mov    ax, word ptr [esp + 18h]
        add    esp, 4
loc_5_0DC1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
loc_5_0DC9:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 20h]
        push   0feh
        lea    eax, [ebp + 10h]
        push   0
        push   0
        push   40h
        push   eax
        sub    edx, edx
        push   1
        movzx  eax, byte ptr [ebp + 0ah]
        lea    ecx, [esp + 2ch]
        push   eax
        call   sub_5_62A6
        or     ax, ax
        jne    NEAR PTR loc_5_0EE8
        mov    eax, dword ptr [ebp + 0bh]
        mov    dword ptr [ebp + 0ah], eax
        push   dword ptr [esp + 2ch]
        movzx  eax, byte ptr [ebp + 6]
        push   dword ptr [esp + 2ch]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esp + 20h], ecx
        mov    word ptr [ebp + 7], 500h
        mov    byte ptr [ebp + 6], 17h
        mov    byte ptr [ebp + 9], 36h
        mov    ebx, dword ptr [esp + 2ch]
        push   ebx
        push   ebp
        call   sub_5_1AC2
        mov    word ptr [esp + 22h], ax
        add    esp, 10h
        lea    eax, [ebx + 0eh]
        db  0BAh, 01Ch, 09Ah, 000h, 000h
loc_5_0E3F:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    SHORT loc_5_0E5F
        or     cl, cl
        je     SHORT loc_5_0E5B
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    SHORT loc_5_0E5F
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    SHORT loc_5_0E3F
loc_5_0E5B:
        sub    eax, eax
        jmp    SHORT loc_5_0E64
loc_5_0E5F:
        sbb    eax, eax
        sbb    eax, -1
loc_5_0E64:
        or     eax, eax
        jne    SHORT loc_5_0E72
        db  0BEh, 01Ch, 09Bh, 000h, 000h
        lea    edi, [ebx + 0eh]
        movsw
loc_5_0E72:
        movzx  eax, byte ptr [ebp + 6]
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [eax], ecx
        cmp    word ptr [esp + 12h], 0
        jne    SHORT loc_5_0ED7
        mov    eax, dword ptr [esp + 28h]
        mov    word ptr [eax], 0
        lea    ecx, [ebx + 46h]
        lea    eax, [ebx + 3eh]
        mov    edx, dword ptr [esp + 14h]
        inc    edx
        mov    dword ptr [eax], edx
        mov    word ptr [eax + 4], 0
        mov    word ptr [eax + 6], 1
        lea    edx, [ebx + 0eh]
        mov    dword ptr [ecx], edx
        mov    word ptr [ecx + 4], 0
        push   0
        push   0eh
        push   eax
        mov    eax, dword ptr [esp + 20h]
        push   0ch
        inc    eax
        push   eax
        db  0E8h, 0F5h, 02Eh, 0FBh, 0FFh
        mov    word ptr [esp + 26h], ax
        add    esp, 14h
        or     ax, ax
        je     SHORT loc_5_0ED7
        mov    word ptr [esp + 12h], 0ffh
loc_5_0ED7:
        push   dword ptr [esp + 14h]
        db  0E8h, 079h, 024h, 0FBh, 0FFh
        mov    ax, word ptr [esp + 16h]
        add    esp, 4
loc_5_0EE8:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_5_0EF0:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    bx, 64h
        mov    esi, dword ptr [esp + 34h]
        mov    di, 10h
        cmp    word ptr [esi + 6], 0
        jne    NEAR PTR loc_5_1124
        cmp    word ptr [esi + 8], 0
        jne    NEAR PTR loc_5_1124
        push   0ffh
        push   0
        push   0
        push   40h
        lea    eax, [esi + 14h]
        push   eax
        push   1
        lea    ecx, [esp + 3ch]
        push   0
        mov    edx, 0ffh
        call   sub_5_62A6
        or     ax, ax
        je     SHORT loc_5_0F47
        movzx  eax, ax
        jmp    NEAR PTR loc_5_1129
loc_5_0F47:
        movzx  ebp, bx
        push   200h
        push   ebp
        db  0E8h, 00Fh, 034h, 0FBh, 0FFh
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_5_0FC2
        lea    eax, [esp + 12h]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        push   ebp
        push   ebx
        push   0
        mov    eax, dword ptr [esp + 38h]
        push   1
        inc    eax
        push   eax
        db  0E8h, 039h, 02Eh, 0FBh, 0FFh
        add    esp, 1ch
        or     ax, ax
        je     SHORT loc_5_0FEC
        cmp    ax, 8aeh
        je     SHORT loc_5_0FCB
        cmp    ax, 84bh
        je     SHORT loc_5_0F93
        cmp    ax, 0eah
        jne    SHORT loc_5_0FD4
loc_5_0F93:
        movzx  eax, word ptr [esi + 0eh]
        add    eax, dword ptr [esi + 0ah]
        dec    eax
        movzx  ecx, word ptr [esp + 10h]
        cmp    eax, ecx
        jbe    SHORT loc_5_0FEC
        add    di, di
        cmp    di, 100h
        ja     SHORT loc_5_1007
        push   ebx
        db  0E8h, 0DCh, 033h, 0FBh, 0FFh
        mov    bx, word ptr [esp + 16h]
        add    esp, 4
        imul   bx, di
        jmp    SHORT loc_5_0F47
loc_5_0FC2:
        mov    si, 96h
        jmp    NEAR PTR loc_5_10FF
loc_5_0FCB:
        mov    si, 9ch
        jmp    NEAR PTR loc_5_10FF
loc_5_0FD4:
        cmp    ax, 8
        jne    SHORT loc_5_0FE3
        mov    si, 96h
        jmp    NEAR PTR loc_5_10FF
loc_5_0FE3:
        mov    si, 0ffh
        jmp    NEAR PTR loc_5_10FF
loc_5_0FEC:
        mov    eax, dword ptr [esi + 0ah]
        mov    dword ptr [esp + 28h], eax
        movzx  eax, word ptr [esp + 10h]
        cmp    eax, dword ptr [esp + 28h]
        ja     SHORT loc_5_1012
        mov    si, 0ffh
        jmp    NEAR PTR loc_5_10FF
loc_5_1007:
        sub    ebx, ebx
        mov    si, 0ffh
        jmp    NEAR PTR loc_5_10FF
loc_5_1012:
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    eax, dword ptr [eax]
        movzx  eax, word ptr [eax + 26h]
        sub    eax, 3bh
        mov    dword ptr [esp + 20h], eax
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_5_1035
        cmp    eax, dword ptr [esp + 20h]
        jae    SHORT loc_5_1035
        mov    dword ptr [esp + 20h], eax
loc_5_1035:
        cmp    word ptr [esi + 0eh], 0
        jne    SHORT loc_5_1044
        mov    dword ptr [esp + 2ch], 0ffffh
loc_5_1044:
        mov    ebp, dword ptr [esp + 38h]
        add    ebp, 0ch
        mov    dword ptr [esp + 18h], 0
        mov    dword ptr [esp + 1ch], 0
        sub    edx, edx
        cmp    word ptr [esp + 10h], dx
        jbe    SHORT loc_5_10CF
loc_5_1064:
        cmp    dword ptr [esp + 28h], edx
        ja     SHORT loc_5_10C2
        mov    edi, dword ptr [ebx + edx*4]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx + 2]
        dec    ecx
        mov    esi, dword ptr [esp + 18h]
        add    esi, eax
        cmp    esi, dword ptr [esp + 20h]
        ja     SHORT loc_5_10CE
        mov    byte ptr [ebp + 2], cl
        movzx  ecx, cl
        mov    dword ptr [esp + 14h], ecx
        lea    edi, [ebp + 3]
        mov    esi, dword ptr [ebx + edx*4]
        shr    ecx, 2
        rep movsd
        mov    ecx, dword ptr [esp + 14h]
        and    ecx, 3
        rep movsb
        inc    eax
        and    eax, 0fffffffeh
        add    dword ptr [esp + 18h], eax
        mov    word ptr [ebp], ax
        add    ebp, eax
        inc    dword ptr [esp + 1ch]
        mov    eax, dword ptr [esp + 1ch]
        cmp    eax, dword ptr [esp + 2ch]
        je     SHORT loc_5_10CF
loc_5_10C2:
        inc    edx
        movzx  eax, word ptr [esp + 10h]
        cmp    eax, edx
        ja     SHORT loc_5_1064
        jmp    SHORT loc_5_10CF
loc_5_10CE:
        dec    edx
loc_5_10CF:
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_5_111D
        mov    ecx, dword ptr [esp + 38h]
        sub    si, si
        mov    word ptr [ecx], si
        mov    ax, word ptr [esp + 1ch]
        mov    word ptr [ecx + 6], ax
        inc    edx
        mov    dword ptr [ecx + 2], edx
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ecx + 8], eax
        mov    ecx, eax
        mov    eax, dword ptr [esp + 3ch]
        add    ecx, 0ch
        mov    dword ptr [eax], ecx
loc_5_10FF:
        or     ebx, ebx
        je     SHORT loc_5_110C
        push   ebx
        db  0E8h, 087h, 032h, 0FBh, 0FFh
        add    esp, 4
loc_5_110C:
        push   dword ptr [esp + 24h]
        db  0E8h, 044h, 022h, 0FBh, 0FFh
        add    esp, 4
        movzx  eax, si
        jmp    SHORT loc_5_1129
loc_5_111D:
        mov    eax, 0ffh
        jmp    SHORT loc_5_1129
loc_5_1124:
        mov    eax, 100h
loc_5_1129:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_5_1140:
        push   esi
        db  083h, 03Dh, 04Ch, 09Fh, 000h, 000h, 000h
        jne    SHORT loc_5_1191
        push   200h
        push   0c0h
        db  0E8h, 00Bh, 032h, 0FBh, 0FFh
        add    esp, 8
        db  0A3h, 04Ch, 09Fh, 000h, 000h
        or     eax, eax
        jne    SHORT loc_5_1169
        sub    eax, eax
        jmp    SHORT loc_5_11C6
loc_5_1169:
        sub    edx, edx
loc_5_116B:
        db  0A1h, 04Ch, 09Fh, 000h, 000h
        add    eax, edx
        add    edx, 0ch
        lea    ecx, [eax + 0ch]
        mov    dword ptr [eax], ecx
        cmp    edx, 0b4h
        jb     SHORT loc_5_116B
        db  0A1h, 04Ch, 09Fh, 000h, 000h
        mov    dword ptr [eax + 0b4h], 0
loc_5_1191:
        db  08Bh, 015h, 04Ch, 09Fh, 000h, 000h
        mov    eax, edx
        mov    eax, dword ptr [eax]
        db  0A3h, 04Ch, 09Fh, 000h, 000h
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [esp + 8]
        mov    ecx, eax
        and    ecx, 7f0000h
        shr    ecx, 10h
        db  08Dh, 00Ch, 08Dh, 048h, 09Dh, 000h, 000h
        mov    dword ptr [edx + 8], eax
        mov    esi, dword ptr [ecx]
        mov    dword ptr [edx], esi
        mov    dword ptr [ecx], edx
loc_5_11C6:
        pop    esi
        ret
sub_5_11C8:
        push   esi
        db  08Bh, 015h, 050h, 09Fh, 000h, 000h
        mov    eax, edx
        shl    eax, 10h
        mov    ecx, edx
        shr    ecx, 10h
        or     eax, ecx
        cmp    eax, 0e8ffffffh
        jbe    SHORT loc_5_11E9
        mov    eax, 1
        jmp    SHORT loc_5_11F2
loc_5_11E9:
        mov    eax, 0
        setae  al
        dec    eax
loc_5_11F2:
        or     eax, eax
        jle    SHORT loc_5_11FB
        mov    edx, 0e800h
loc_5_11FB:
        db  089h, 015h, 050h, 09Fh, 000h, 000h
        push   dword ptr [esp + 8]
        push   edx
        call   sub_5_1140
        add    esp, 8
        db  08Bh, 00Dh, 050h, 09Fh, 000h, 000h
        mov    edx, ecx
        shl    edx, 10h
        mov    esi, ecx
        shr    esi, 10h
        or     edx, esi
        inc    edx
        shr    edx, 10h
        add    ecx, 10000h
        and    ecx, 0ffff0000h
        or     edx, ecx
        db  089h, 015h, 050h, 09Fh, 000h, 000h
        pop    esi
        ret
sub_5_123A:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, ecx
        and    eax, 7f0000h
        shr    eax, 10h
        db  08Bh, 004h, 085h, 048h, 09Dh, 000h, 000h
        or     eax, eax
        je     SHORT loc_5_125E
loc_5_1253:
        cmp    dword ptr [eax + 8], ecx
        je     SHORT loc_5_125E
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_5_1253
loc_5_125E:
        ret
sub_5_125F:
        push   dword ptr [esp + 4]
        call   sub_5_123A
        add    esp, 4
        or     eax, eax
        je     SHORT loc_5_1274
        mov    eax, dword ptr [eax + 4]
        jmp    SHORT loc_5_1276
loc_5_1274:
        sub    eax, eax
loc_5_1276:
        ret
sub_5_1277:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, eax
        and    ecx, 7f0000h
        shr    ecx, 10h
        db  08Dh, 00Ch, 08Dh, 048h, 09Dh, 000h, 000h
        mov    edx, dword ptr [ecx]
        or     edx, edx
        je     SHORT loc_5_12B3
loc_5_1293:
        cmp    dword ptr [edx + 8], eax
        je     SHORT loc_5_12A2
        mov    ecx, edx
        mov    edx, dword ptr [edx]
        or     edx, edx
        jne    SHORT loc_5_1293
        jmp    SHORT loc_5_12B3
loc_5_12A2:
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        db  0A1h, 04Ch, 09Fh, 000h, 000h
        mov    dword ptr [edx], eax
        db  089h, 015h, 04Ch, 09Fh, 000h, 000h
loc_5_12B3:
        ret
sub_5_12B4:
        push   esi
        push   edi
        sub    eax, eax
        mov    esi, dword ptr [esp + 0ch]
        sub    edi, edi
        mov    dl, byte ptr [esi]
        cmp    dl, al
        je     SHORT loc_5_12E3
loc_5_12C4:
        mov    cl, byte ptr [edi + esi + 1]
        or     cl, cl
        je     SHORT loc_5_12E3
        movzx  ecx, cl
        imul   eax, eax, 5
        db  00Fh, 0B6h, 089h, 038h, 0A0h, 000h, 000h
        add    eax, ecx
        inc    edi
        movzx  ecx, dl
        cmp    ecx, edi
        ja     SHORT loc_5_12C4
loc_5_12E3:
        and    eax, 7fh
        pop    edi
        pop    esi
        ret
sub_5_12E9:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        mov    ebx, dword ptr [esp + 18h]
        db  08Bh, 03Ch, 085h, 044h, 09Bh, 000h, 000h
        lea    esi, [ebx + 3]
        movzx  eax, byte ptr [ebx + 2]
        push   eax
        push   esi
        db  0E8h, 012h, 030h, 0FBh, 0FFh
        add    esp, 8
        mov    ebp, eax
        or     edi, edi
        je     SHORT loc_5_133D
loc_5_1313:
        mov    ax, word ptr [ebx]
        cmp    word ptr [edi + 16h], ax
        jne    SHORT loc_5_1336
        movzx  eax, byte ptr [edi + 18h]
        cmp    eax, ebp
        jne    SHORT loc_5_1336
        push   ebp
        push   esi
        lea    eax, [edi + 19h]
        push   eax
        db  0E8h, 0D9h, 004h, 0FBh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_1344
loc_5_1336:
        mov    edi, dword ptr [edi + 4]
        or     edi, edi
        jne    SHORT loc_5_1313
loc_5_133D:
        sub    eax, eax
loc_5_133F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_1344:
        db  0A1h, 0D8h, 09Fh, 000h, 000h
        cmp    dword ptr [edi + 10h], eax
        jae    SHORT loc_5_1359
        push   edi
        call   sub_5_321B
        add    esp, 4
        jmp    SHORT loc_5_133D
loc_5_1359:
        mov    eax, edi
        jmp    SHORT loc_5_133F
sub_5_135D:
        mov    eax, dword ptr [esp + 4]
        db  08Dh, 014h, 085h, 044h, 09Bh, 000h, 000h
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [edx]
        cmp    ecx, eax
        je     SHORT loc_5_137F
loc_5_1372:
        or     ecx, ecx
        je     SHORT loc_5_1384
        lea    edx, [ecx + 4]
        mov    ecx, dword ptr [edx]
        cmp    ecx, eax
        jne    SHORT loc_5_1372
loc_5_137F:
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx], eax
loc_5_1384:
        ret
sub_5_1385:
        push   esi
        push   edi
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_5_1396
        mov    ax, 0fh
        sub    ecx, ecx
        jmp    SHORT loc_5_139F
loc_5_1396:
        mov    ax, 0f0h
        mov    ecx, 4
loc_5_139F:
        mov    edi, dword ptr [esp + 10h]
        or     edi, edi
        je     SHORT loc_5_13F3
        mov    edx, dword ptr [esp + 0ch]
        movzx  eax, ax
        movzx  esi, byte ptr [edx + 15h]
        and    esi, eax
        sar    esi, cl
        or     esi, esi
        je     SHORT loc_5_13F3
        cmp    esi, 1
        je     SHORT loc_5_13CD
        cmp    esi, 2
        je     SHORT loc_5_13D7
        cmp    esi, 3
        je     SHORT loc_5_13E2
        sub    eax, eax
        jmp    SHORT loc_5_13F8
loc_5_13CD:
        cmp    byte ptr [edi + 6ah], 0
        jne    SHORT loc_5_13F3
        sub    eax, eax
        jmp    SHORT loc_5_13F8
loc_5_13D7:
        mov    eax, dword ptr [edx]
        cmp    dword ptr [edi + 14h], eax
        je     SHORT loc_5_13F3
        sub    eax, eax
        jmp    SHORT loc_5_13F8
loc_5_13E2:
        cmp    byte ptr [edi + 6ah], 3
        je     SHORT loc_5_13F3
        test   byte ptr [edi + 1ah], 10h
        mov    eax, 0
        je     SHORT loc_5_13F8
loc_5_13F3:
        mov    eax, 1
loc_5_13F8:
        pop    edi
        pop    esi
        ret
sub_5_13FB:
        push   esi
        push   edi
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_5_140C
        mov    dx, 0fh
        sub    ecx, ecx
        jmp    SHORT loc_5_1415
loc_5_140C:
        mov    dx, 0f0h
        mov    ecx, 4
loc_5_1415:
        mov    esi, dword ptr [esp + 14h]
        or     esi, esi
        je     SHORT loc_5_146D
        mov    eax, dword ptr [esp + 10h]
        movzx  edi, byte ptr [eax + 13h]
        movzx  eax, dx
        and    edi, eax
        sar    edi, cl
        or     edi, edi
        je     SHORT loc_5_146D
        cmp    edi, 1
        je     SHORT loc_5_1443
        cmp    edi, 2
        je     SHORT loc_5_144D
        cmp    edi, 3
        je     SHORT loc_5_145C
        sub    eax, eax
        jmp    SHORT loc_5_1472
loc_5_1443:
        cmp    byte ptr [esi + 6ah], 0
        jne    SHORT loc_5_146D
        sub    eax, eax
        jmp    SHORT loc_5_1472
loc_5_144D:
        mov    eax, dword ptr [esp + 0ch]
        mov    eax, dword ptr [eax]
        cmp    dword ptr [esi + 14h], eax
        je     SHORT loc_5_146D
        sub    eax, eax
        jmp    SHORT loc_5_1472
loc_5_145C:
        cmp    byte ptr [esi + 6ah], 3
        je     SHORT loc_5_146D
        test   byte ptr [esi + 1ah], 10h
        mov    eax, 0
        je     SHORT loc_5_1472
loc_5_146D:
        mov    eax, 1
loc_5_1472:
        pop    edi
        pop    esi
        ret
sub_5_1475:
        push   esi
        mov    esi, dword ptr [esp + 8]
        movzx  eax, byte ptr [esi]
        push   eax
        lea    eax, [esi + 1]
        push   eax
        db  0E8h, 095h, 02Eh, 0FBh, 0FFh
        add    esp, 8
        cmp    eax, dword ptr [esp + 0ch]
        ja     SHORT loc_5_14E5
        or     eax, eax
        je     SHORT loc_5_14E5
        mov    ecx, 0
        jbe    SHORT loc_5_14DE
loc_5_149B:
        movzx  edx, byte ptr [esi + ecx + 1]
        db  08Ah, 092h, 038h, 0A0h, 000h, 000h
        or     dl, dl
        jne    SHORT loc_5_14B1
        inc    ecx
        cmp    eax, ecx
        ja     SHORT loc_5_14D9
        jmp    SHORT loc_5_14E5
loc_5_14B1:
        cmp    dl, 20h
        jbe    SHORT loc_5_14E5
        cmp    dl, 2ah
        je     SHORT loc_5_14E5
        cmp    dl, 3fh
        je     SHORT loc_5_14E5
        cmp    dl, 2fh
        je     SHORT loc_5_14E5
        cmp    dl, 5ch
        je     SHORT loc_5_14E5
        cmp    dl, 3ah
        je     SHORT loc_5_14E5
        cmp    dl, 3bh
        je     SHORT loc_5_14E5
        cmp    dl, 2ch
        je     SHORT loc_5_14E5
loc_5_14D9:
        inc    ecx
        cmp    eax, ecx
        ja     SHORT loc_5_149B
loc_5_14DE:
        mov    eax, 1
        jmp    SHORT loc_5_14E7
loc_5_14E5:
        sub    eax, eax
loc_5_14E7:
        pop    esi
        ret
sub_5_14E9:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   esi
        db  0BEh, 068h, 09Fh, 000h, 000h
        mov    dword ptr [ebp - 18h], 0
        db  083h, 03Dh, 0E0h, 0B3h, 000h, 000h, 000h
        je     SHORT loc_5_1526
        lea    eax, [ebp - 20h]
        mov    dword ptr [ebp - 4], eax
        push   dword ptr [ebp - 4]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 005h, 000h, 093h, 004h
        add    esp, 4
        cmp    dword ptr [ebp - 18h], 0
        je     SHORT loc_5_1526
        mov    esi, dword ptr [ebp - 18h]
loc_5_1526:
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_5_152D:
        db  083h, 03Dh, 054h, 09Fh, 000h, 000h, 000h
        jne    SHORT loc_5_1556
        call   sub_5_14E9
        push   eax
        db  0FFh, 035h, 088h, 0B1h, 000h, 000h
        push   30h
        db  0E8h, 0A2h, 07Ch, 0FBh, 0FFh
        add    esp, 0ch
        db  0C7h, 005h, 054h, 09Fh, 000h, 000h, 001h, 000h, 000h, 000h
loc_5_1556:
        ret
sub_5_1557:
        db  0E8h, 028h, 02Ch, 0FBh, 0FFh
        or     al, al
        jne    SHORT loc_5_156C
        call   sub_5_152D
        mov    ax, word ptr [esp + 18h]
        jmp    SHORT loc_5_15AA
loc_5_156C:
        mov    ecx, dword ptr [esp + 10h]
        push   dword ptr [esp + 14h]
        push   ecx
        mov    cx, word ptr [esp + 14h]
        push   ecx
        mov    ecx, dword ptr [esp + 14h]
        push   ecx
        mov    cl, byte ptr [esp + 14h]
        push   ecx
        push   eax
        db  0E8h, 0B7h, 027h, 0FBh, 0FFh
        add    esp, 18h
        cmp    ax, 7
        jne    SHORT loc_5_15A0
        call   sub_5_152D
        mov    ax, word ptr [esp + 18h]
        jmp    SHORT loc_5_15AA
loc_5_15A0:
        db  0C7h, 005h, 054h, 09Fh, 000h, 000h, 000h, 000h, 000h, 000h
loc_5_15AA:
        ret
sub_5_15AB:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    bx, word ptr [esp + 28h]
        or     bx, bx
        mov    eax, 1
        je     SHORT loc_5_15C4
        movzx  eax, bx
loc_5_15C4:
        push   200h
        push   eax
        db  0E8h, 095h, 02Dh, 0FBh, 0FFh
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_5_15E1
        mov    ax, 96h
        jmp    NEAR PTR loc_5_1664
loc_5_15E1:
        mov    ax, word ptr [esp + 1ch]
        cmp    ax, 0ffffh
        jne    SHORT loc_5_1603
        mov    esi, dword ptr [esp + 18h]
        mov    ax, word ptr [esi + 7]
        push   eax
        db  0E8h, 066h, 024h, 0FBh, 0FFh
        add    esp, 4
        add    ax, 2
        jmp    SHORT loc_5_160B
loc_5_1603:
        add    ax, 3
        mov    esi, dword ptr [esp + 18h]
loc_5_160B:
        push   dword ptr [esp + 2ch]
        push   ebx
        push   ebp
        push   eax
        lea    eax, [esi + 7]
        push   eax
        mov    al, byte ptr [esi + 6]
        push   eax
        call   sub_5_1557
        mov    word ptr [esp + 2ah], ax
        add    esp, 18h
        or     ax, ax
        jne    SHORT loc_5_1656
        or     bx, bx
        je     SHORT loc_5_164B
        movzx  eax, bx
        mov    edi, dword ptr [esp + 20h]
        add    edi, 8
        mov    esi, ebp
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_5_164B:
        mov    eax, dword ptr [esp + 24h]
        or     eax, eax
        je     SHORT loc_5_1656
        mov    word ptr [eax], bx
loc_5_1656:
        push   ebp
        db  0E8h, 034h, 02Dh, 0FBh, 0FFh
        mov    ax, word ptr [esp + 16h]
        add    esp, 4
loc_5_1664:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_5_166C:
        push   ebp
        mov    ebp, esp
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 14h]
        test   byte ptr [ecx + 1ah], 10h
        je     SHORT loc_5_1683
        jmp    NEAR PTR loc_5_1835
loc_5_1683:
        db  083h, 03Dh, 058h, 09Fh, 000h, 000h, 000h
        jne    SHORT loc_5_1695
        mov    ax, 0feh
        jmp    NEAR PTR loc_5_1838
loc_5_1695:
        mov    esi, dword ptr [ebp + 8]
        add    esi, 14h
        mov    eax, dword ptr [ebp + 8]
        add    eax, 15h
        mov    dword ptr [ebp - 0ch], eax
        movzx  eax, byte ptr [esi]
        push   eax
        push   dword ptr [ebp - 0ch]
        db  0E8h, 06Ch, 02Ch, 0FBh, 0FFh
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        mov    byte ptr [ebp - 10h], al
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, eax
        mov    byte ptr [esi], al
        mov    byte ptr [ebx + 15h], 0
        push   30h
        push   esi
        call   sub_5_1475
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_5_16DC
        mov    ax, 0efh
        jmp    NEAR PTR loc_5_1838
loc_5_16DC:
        mov    eax, dword ptr [ebp - 8]
        inc    eax
        push   eax
        push   dword ptr [ebp - 0ch]
        lea    eax, [ebx + 16h]
        push   eax
        db  0E8h, 087h, 001h, 0FBh, 0FFh
        add    esp, 0ch
        lea    eax, [ebp - 2]
        mov    dword ptr [ebp - 30h], eax
        mov    dword ptr [ebp - 2ch], 8
        mov    eax, dword ptr [ebp + 14h]
        add    eax, 62h
        mov    dword ptr [ebp - 28h], eax
        mov    dword ptr [ebp - 24h], 8
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0ah
        mov    dword ptr [ebp - 20h], eax
        db  0C7h, 045h, 0E4h, 0ACh, 0B1h, 000h, 000h
        lea    eax, [ebx + 16h]
        mov    dword ptr [ebp - 18h], eax
        mov    dword ptr [ebp - 14h], 0
        push   dword ptr [ebp - 30h]
        push   dword ptr [ebp - 2ch]
        push   dword ptr [ebp - 28h]
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 14h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 003h, 000h, 093h, 004h
        add    esp, 20h
        cmp    word ptr [ebp - 2], 0
        je     SHORT loc_5_1761
        mov    ax, 0ffh
        jmp    NEAR PTR loc_5_1838
loc_5_1761:
        mov    ax, word ptr [ebp - 8]
        add    ax, 3
        push   eax
        db  0E8h, 0F1h, 022h, 0FBh, 0FFh
        add    esp, 4
        mov    word ptr [ebx + 16h], ax
        mov    byte ptr [ebx + 18h], 35h
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0bh
        mov    esi, dword ptr [ebp + 8]
        add    esi, 12h
        lea    edi, [ebx + 19h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    esi, [ebx + 7ah]
        push   0ffh
        push   64h
        mov    ecx, dword ptr [ebp + 8]
        push   esi
        mov    ax, word ptr [ecx + 7]
        push   eax
        db  0E8h, 0B1h, 022h, 0FBh, 0FFh
        add    esp, 4
        add    ax, 2
        push   eax
        lea    eax, [ebx + 16h]
        push   eax
        push   17h
        call   sub_5_1557
        add    esp, 18h
        mov    word ptr [ebp - 2], ax
        or     ax, ax
        je     SHORT loc_5_17D0
        jmp    SHORT loc_5_1838
loc_5_17D0:
        mov    ecx, dword ptr [ebp + 14h]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx + 14h], eax
        push   dword ptr [ebp - 0ch]
        mov    ax, word ptr [ebx + 7eh]
        mov    word ptr [ecx + 18h], ax
        push   0
        db  0E8h, 0B6h, 025h, 0FBh, 0FFh
        add    esp, 8
        or     eax, eax
        mov    ecx, dword ptr [ebp + 14h]
        je     SHORT loc_5_1812
        mov    byte ptr [ecx + 6ah], 3
        db  083h, 03Dh, 0DCh, 0B3h, 000h, 000h, 000h
        jne    SHORT loc_5_1816
        db  0E8h, 0F3h, 00Bh, 0FBh, 0FFh
        db  0C7h, 005h, 0DCh, 0B3h, 000h, 000h, 001h, 000h, 000h, 000h
        jmp    SHORT loc_5_1816
loc_5_1812:
        mov    byte ptr [ecx + 6ah], 1
loc_5_1816:
        mov    ecx, dword ptr [ebp + 14h]
        mov    al, byte ptr [ebp - 10h]
        mov    byte ptr [ecx + 72h], al
        mov    eax, dword ptr [ebp - 8]
        inc    eax
        push   eax
        push   dword ptr [ebp - 0ch]
        mov    eax, ecx
        add    eax, 73h
        push   eax
        db  0E8h, 042h, 000h, 0FBh, 0FFh
        add    esp, 0ch
loc_5_1835:
        sub    ax, ax
loc_5_1838:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_183F:
        push   0ffh
        push   0
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        push   0ffffh
        push   dword ptr [esp + 18h]
        call   sub_5_15AB
        add    esp, 18h
        ret
loc_5_1860:
        mov    ecx, dword ptr [esp + 4]
        push   0ffh
        movzx  dx, byte ptr [ecx + 14h]
        push   0
        add    dx, 0bh
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        push   edx
        push   ecx
        call   sub_5_15AB
        add    esp, 18h
        ret
loc_5_1887:
        mov    edx, dword ptr [esp + 4]
        push   0ffh
        movzx  cx, byte ptr [edx + 14h]
        push   0
        movzx  eax, cx
        movzx  ax, byte ptr [edx + eax + 15h]
        push   dword ptr [esp + 14h]
        lea    ecx, [ecx + eax + 0ch]
        push   dword ptr [esp + 14h]
        push   ecx
        push   edx
        call   sub_5_15AB
        add    esp, 18h
        ret
loc_5_18B7:
        mov    ax, 0d6h
        ret
sub_5_18BC:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        push   30h
        lea    edi, [esi + 4]
        push   edi
        call   sub_5_1475
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_5_18DF
        mov    ax, 0efh
        jmp    NEAR PTR loc_5_19C6
loc_5_18DF:
        movzx  eax, byte ptr [edi]
        push   eax
        lea    eax, [esi + 5]
        push   eax
        db  0E8h, 030h, 02Ah, 0FBh, 0FFh
        add    esp, 8
        mov    ebx, eax
        mov    byte ptr [edi], bl
        push   edi
        call   sub_5_12B4
        add    esp, 4
        mov    ebp, eax
        lea    eax, [esi + 2]
        push   eax
        push   ebp
        call   sub_5_12E9
        add    esp, 8
        or     eax, eax
        je     SHORT loc_5_1918
        mov    ax, 0eeh
        jmp    NEAR PTR loc_5_19C6
loc_5_1918:
        push   200h
        lea    eax, [ebx + 19h]
        push   eax
        db  0E8h, 03Eh, 02Ah, 0FBh, 0FFh
        mov    edi, dword ptr [esp + 20h]
        add    esp, 8
        mov    dword ptr [edi], eax
        or     eax, eax
        jne    SHORT loc_5_193C
        mov    ax, 96h
        jmp    NEAR PTR loc_5_19C6
loc_5_193C:
        push   dword ptr [edi]
        call   sub_5_11C8
        add    esp, 4
        mov    ecx, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edi]
        cmp    dword ptr [eax], 0
        jne    SHORT loc_5_1960
        push   eax
        db  0E8h, 039h, 02Ah, 0FBh, 0FFh
        add    esp, 4
        mov    ax, 96h
        jmp    SHORT loc_5_19C6
loc_5_1960:
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [edi]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [edi]
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [edi]
        mov    dword ptr [eax + 10h], 0ffffffffh
        mov    ecx, dword ptr [edi]
        mov    al, byte ptr [esi]
        mov    byte ptr [ecx + 14h], al
        mov    ecx, dword ptr [edi]
        mov    al, byte ptr [esi + 1]
        mov    byte ptr [ecx + 15h], al
        mov    ecx, dword ptr [edi]
        mov    ax, word ptr [esi + 2]
        mov    word ptr [ecx + 16h], ax
        mov    eax, dword ptr [edi]
        push   ebx
        mov    byte ptr [eax + 18h], bl
        lea    eax, [esi + 5]
        push   eax
        mov    eax, dword ptr [edi]
        add    eax, 19h
        push   eax
        db  0E8h, 0C4h, 0FEh, 0FAh, 0FFh
        add    esp, 0ch
        db  08Dh, 004h, 0ADh, 044h, 09Bh, 000h, 000h
        mov    edx, dword ptr [edi]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx + 4], ecx
        mov    dword ptr [eax], edx
        sub    ax, ax
loc_5_19C6:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_19CB:
        mov    ax, 0f5h
        ret
loc_5_19D0:
        mov    ax, 0f1h
        ret
sub_5_19D5:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 1ch]
        push   30h
        push   ebx
        call   sub_5_1475
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_5_19F8
        mov    ax, 0efh
        jmp    NEAR PTR loc_5_1AB5
loc_5_19F8:
        mov    eax, dword ptr [esp + 18h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 10h], eax
        push   dword ptr [eax]
        call   sub_5_123A
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_5_1A1B
        mov    ax, 0ffh
        jmp    NEAR PTR loc_5_1AB5
loc_5_1A1B:
        push   200h
        movzx  eax, byte ptr [ebx]
        add    eax, 19h
        push   eax
        db  0E8h, 038h, 029h, 0FBh, 0FFh
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_5_1A3B
        mov    ax, 96h
        jmp    SHORT loc_5_1AB5
loc_5_1A3B:
        mov    dword ptr [esi + 4], ebp
        mov    edi, ebp
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, 6
        rep movsd
        movsb
        mov    dword ptr [ebp + 4], 0
        push   ebx
        call   sub_5_12B4
        add    esp, 4
        mov    esi, eax
        mov    eax, dword ptr [esp + 10h]
        add    eax, 18h
        push   eax
        call   sub_5_12B4
        add    esp, 4
        push   dword ptr [esp + 10h]
        push   eax
        call   sub_5_135D
        add    esp, 8
        db  08Dh, 004h, 0B5h, 044h, 09Bh, 000h, 000h
        push   dword ptr [esp + 10h]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebp + 4], ecx
        mov    dword ptr [eax], ebp
        db  0E8h, 0FEh, 028h, 0FBh, 0FFh
        add    esp, 4
        mov    al, byte ptr [ebx]
        mov    byte ptr [ebp + 18h], al
        movzx  eax, byte ptr [ebx]
        push   eax
        inc    ebx
        push   ebx
        lea    eax, [ebp + 19h]
        push   eax
        db  0E8h, 0CBh, 0FDh, 0FAh, 0FFh
        mov    eax, dword ptr [esp + 24h]
        add    esp, 0ch
        mov    dword ptr [eax], ebp
        sub    ax, ax
loc_5_1AB5:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_5_1ABD:
        mov    ax, 0f3h
        ret
sub_5_1AC2:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 0F5h, 079h, 0FBh, 0FFh
        mov    edi, dword ptr [esp + 1ch]
        add    esp, 4
        push   dword ptr [edi + 0ah]
        call   sub_5_125F
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    NEAR PTR loc_5_1B8F
        mov    esi, dword ptr [edi + 0ah]
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 010h, 07Ah, 0FBh, 0FFh
        mov    ebp, dword ptr [esp + 28h]
        add    esp, 4
        mov    ax, word ptr [ebp + 1ah]
        test   al, 10h
        je     SHORT loc_5_1B19
        test   al, 40h
        jne    SHORT loc_5_1B19
        mov    byte ptr [esp + 13h], 0fch
        jmp    NEAR PTR loc_5_1BFC
loc_5_1B19:
        push   0fch
        push   36h
        mov    ebx, dword ptr [esp + 24h]
        push   0
        push   ebx
        push   0ffffh
        push   edi
        call   sub_5_15AB
        mov    byte ptr [esp + 2bh], al
        add    esp, 18h
        cmp    al, 0fch
        jne    SHORT loc_5_1B7D
        cmp    byte ptr [ebp + 6ah], 0
        je     SHORT loc_5_1B7D
        cmp    dword ptr [ebp + 14h], esi
        jne    SHORT loc_5_1B7D
        mov    dword ptr [ebx + 8], esi
        mov    ax, word ptr [ebp + 18h]
        mov    word ptr [ebx + 0ch], ax
        add    ebx, 0eh
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
        movzx  eax, byte ptr [ebp + 72h]
        lea    esi, [ebp + 73h]
        mov    edi, ebx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    byte ptr [esp + 13h], 0
loc_5_1B7D:
        cmp    byte ptr [esp + 13h], 0
        jne    SHORT loc_5_1BFC
        mov    eax, dword ptr [esp + 20h]
        mov    word ptr [eax], 36h
        jmp    SHORT loc_5_1BFC
loc_5_1B8F:
        mov    ebp, dword ptr [esp + 24h]
        push   1
        push   ebp
        push   esi
        call   sub_5_1385
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_5_1BAA
        mov    byte ptr [esp + 13h], 0f2h
        jmp    SHORT loc_5_1BEF
loc_5_1BAA:
        mov    ebx, dword ptr [esp + 1ch]
        mov    eax, dword ptr [edi + 0ah]
        mov    dword ptr [ebx + 8], eax
        add    ebx, 0eh
        mov    ax, word ptr [esi + 16h]
        mov    word ptr [ebx - 2], ax
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
        movzx  eax, byte ptr [esi + 18h]
        add    esi, 19h
        mov    edi, ebx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 20h]
        mov    byte ptr [esp + 13h], 0
        mov    word ptr [eax], 36h
loc_5_1BEF:
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 013h, 079h, 0FBh, 0FFh
        add    esp, 4
loc_5_1BFC:
        movzx  ax, byte ptr [esp + 13h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_5_1C0A:
        push   esi
        mov    esi, dword ptr [esp + 8]
        lea    eax, [esi + 2]
        push   eax
        call   sub_5_12B4
        add    esp, 4
        push   esi
        push   eax
        call   sub_5_12E9
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_5_1C2F
        mov    ax, 0fch
        jmp    SHORT loc_5_1C3A
loc_5_1C2F:
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], ecx
        sub    ax, ax
loc_5_1C3A:
        pop    esi
        ret
sub_5_1C3C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    ax, word ptr [eax + 1ah]
        test   al, 10h
        je     SHORT loc_5_1C53
        test   al, 40h
        je     SHORT loc_5_1C66
loc_5_1C53:
        mov    ebx, dword ptr [esp + 14h]
        mov    ax, word ptr [ebx]
        cmp    ax, 100h
        je     SHORT loc_5_1C6F
        cmp    ax, 200h
        je     SHORT loc_5_1C6F
loc_5_1C66:
        mov    si, 0fch
        jmp    NEAR PTR loc_5_1CF1
loc_5_1C6F:
        push   200h
        push   78h
        db  0E8h, 0E9h, 026h, 0FBh, 0FFh
        add    esp, 8
        mov    ebp, eax
        movzx  eax, byte ptr [ebx + 2]
        add    eax, 3
        lea    edi, [ebp + 3]
        mov    esi, ebx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        movzx  ax, byte ptr [ebx + 2]
        add    ax, 4
        push   eax
        db  0E8h, 0B7h, 01Dh, 0FBh, 0FFh
        add    esp, 4
        mov    word ptr [ebp], ax
        mov    byte ptr [ebp + 2], 35h
        lea    edi, [ebp + 3ch]
        push   0fch
        push   3ch
        push   edi
        push   eax
        db  0E8h, 09Bh, 01Dh, 0FBh, 0FFh
        add    esp, 4
        add    ax, 2
        push   eax
        push   ebp
        push   17h
        call   sub_5_1557
        add    esp, 18h
        mov    si, ax
        or     si, si
        jne    SHORT loc_5_1CE8
        mov    eax, dword ptr [esp + 18h]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [eax], ecx
loc_5_1CE8:
        push   ebp
        db  0E8h, 0A2h, 026h, 0FBh, 0FFh
        add    esp, 4
loc_5_1CF1:
        mov    ax, si
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_1CF9:
        push   ebx
        push   esi
        push   edi
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 0C2h, 077h, 0FBh, 0FFh
        mov    edi, dword ptr [esp + 18h]
        add    esp, 4
        mov    esi, dword ptr [esp + 10h]
        push   edi
        push   esi
        call   sub_5_1C0A
        add    esp, 8
        mov    bx, ax
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 0E4h, 077h, 0FBh, 0FFh
        add    esp, 4
        cmp    bx, 0fch
        jne    SHORT loc_5_1D3F
        push   edi
        push   esi
        call   sub_5_1C3C
        add    esp, 8
        mov    bx, ax
loc_5_1D3F:
        mov    ax, bx
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_1D46:
        push   ebx
        push   esi
        push   edi
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 075h, 077h, 0FBh, 0FFh
        mov    edi, dword ptr [esp + 14h]
        add    esp, 4
        lea    eax, [edi + 0ch]
        push   eax
        call   sub_5_12B4
        add    esp, 4
        lea    ebx, [edi + 0ah]
        push   ebx
        push   eax
        call   sub_5_12E9
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_5_1DD0
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 089h, 077h, 0FBh, 0FFh
        mov    eax, dword ptr [esp + 20h]
        add    esp, 4
        mov    ax, word ptr [eax + 1ah]
        test   al, 10h
        je     SHORT loc_5_1D96
        test   al, 40h
        je     SHORT loc_5_1DA5
loc_5_1D96:
        mov    ax, word ptr [ebx]
        cmp    ax, 100h
        je     SHORT loc_5_1DAE
        cmp    ax, 200h
        je     SHORT loc_5_1DAE
loc_5_1DA5:
        mov    si, 0fch
        jmp    NEAR PTR loc_5_1E38
loc_5_1DAE:
        push   0fch
        push   36h
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 20h]
        push   0ffffh
        push   edi
        call   sub_5_15AB
        add    esp, 18h
        mov    si, ax
        jmp    SHORT loc_5_1E38
loc_5_1DD0:
        push   1
        push   dword ptr [esp + 20h]
        push   esi
        call   sub_5_1385
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_5_1DE9
        mov    si, 0f2h
        jmp    SHORT loc_5_1E2B
loc_5_1DE9:
        mov    edx, dword ptr [esp + 14h]
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx + 8], eax
        add    edx, 0eh
        mov    ax, word ptr [esi + 16h]
        mov    word ptr [edx - 2], ax
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
        movzx  eax, byte ptr [esi + 18h]
        add    esi, 19h
        mov    edi, edx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 18h]
        sub    si, si
        mov    word ptr [eax], 36h
loc_5_1E2B:
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 0D7h, 076h, 0FBh, 0FFh
        add    esp, 4
loc_5_1E38:
        mov    ax, si
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_1E3F:
        push   esi
        mov    esi, dword ptr [esp + 8]
        lea    eax, [esi + 2]
        push   eax
        call   sub_5_12B4
        add    esp, 4
        push   esi
        push   eax
        call   sub_5_12E9
        mov    ecx, dword ptr [esp + 14h]
        add    esp, 8
        mov    dword ptr [ecx], eax
        or     eax, eax
        mov    ax, 0
        jne    SHORT loc_5_1E6C
        mov    ax, 0ffh
loc_5_1E6C:
        pop    esi
        ret
sub_5_1E6E:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 18h]
        mov    cx, word ptr [ebx + 0eh]
        lea    eax, [ecx - 0ffffh]
        cmp    ax, 1
        sbb    ebp, ebp
        neg    ebp
        mov    eax, dword ptr [esp + 24h]
        mov    ax, word ptr [eax + 1ah]
        test   al, 10h
        je     SHORT loc_5_1EA4
        test   al, 40h
        jne    SHORT loc_5_1EA4
        mov    ax, 0fch
        jmp    NEAR PTR loc_5_1F8F
loc_5_1EA4:
        cmp    cx, 100h
        je     SHORT loc_5_1EBF
        cmp    cx, 200h
        je     SHORT loc_5_1EBF
        or     ebp, ebp
        jne    SHORT loc_5_1EC7
        mov    ax, 0fch
        jmp    NEAR PTR loc_5_1F8F
loc_5_1EBF:
        or     ebp, ebp
        je     NEAR PTR loc_5_1F66
loc_5_1EC7:
        lea    edi, [ebx + 67h]
        mov    esi, ebx
        mov    ecx, 0fh
        rep movsd
        cmp    dword ptr [ebx + 0ah], -1
        jne    SHORT loc_5_1EE5
        mov    word ptr [ebx + 75h], 100h
        mov    esi, dword ptr [esp + 1ch]
        jmp    SHORT loc_5_1F29
loc_5_1EE5:
        mov    eax, dword ptr [ebx + 71h]
        mov    dword ptr [ebx + 0ah], eax
        mov    byte ptr [ebx + 9], 36h
        push   5
        db  0E8h, 06Ah, 01Bh, 0FBh, 0FFh
        add    esp, 4
        mov    word ptr [ebx + 7], ax
        push   0fch
        push   64h
        mov    esi, dword ptr [esp + 24h]
        push   0
        push   esi
        push   0ffffh
        push   ebx
        call   sub_5_15AB
        add    esp, 18h
        or     ax, ax
        jne    SHORT loc_5_1F8F
        lea    eax, [ebx + 75h]
        mov    ax, word ptr [esi + 0ch]
        mov    word ptr [ebx + 75h], ax
loc_5_1F29:
        mov    edi, 0ffffffffh
loc_5_1F2E:
        push   0fch
        push   39h
        push   0
        push   esi
        lea    eax, [ebx + 67h]
        push   0ffffh
        push   eax
        call   sub_5_15AB
        add    esp, 18h
        cmp    ax, 0fch
        jne    SHORT loc_5_1F81
        or     ebp, ebp
        je     SHORT loc_5_1F81
        cmp    word ptr [ebx + 75h], 100h
        jne    SHORT loc_5_1F81
        mov    word ptr [ebx + 75h], 200h
        mov    dword ptr [ebx + 71h], edi
        jmp    SHORT loc_5_1F2E
loc_5_1F66:
        push   0fch
        push   39h
        push   0
        push   dword ptr [esp + 28h]
        push   0ffffh
        push   ebx
        call   sub_5_15AB
        add    esp, 18h
loc_5_1F81:
        or     ax, ax
        jne    SHORT loc_5_1F8F
        mov    ecx, dword ptr [esp + 20h]
        mov    word ptr [ecx], 39h
loc_5_1F8F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_5_1F97:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        mov    esi, dword ptr [ebx]
        db  08Bh, 03Dh, 024h, 09Bh, 000h, 000h
        mov    ecx, dword ptr [edi + 8]
        mov    edx, ecx
        sub    ecx, esi
        jbe    SHORT loc_5_1FC7
        mov    edi, dword ptr [edi]
        lea    edi, [edi + esi*2]
        mov    ax, word ptr [esp + 14h]
        cmp    ax, 0ffffh
        je     SHORT loc_5_1FCB
        repne scasw
        jne    SHORT loc_5_1FC7
        inc    ecx
        jmp    SHORT loc_5_1FCB
loc_5_1FC7:
        sub    eax, eax
        jmp    SHORT loc_5_1FD4
loc_5_1FCB:
        sub    edx, ecx
        mov    dword ptr [ebx], edx
        mov    eax, 1
loc_5_1FD4:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_1FD8:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 20h]
        lea    esi, [eax + 1]
        movzx  eax, byte ptr [eax]
        push   eax
        push   esi
        db  0E8h, 02Ch, 023h, 0FBh, 0FFh
        add    esp, 8
        mov    ebp, eax
        mov    dword ptr [esp + 10h], 0
        mov    di, word ptr [esp + 1ch]
        lea    eax, [esp + 10h]
        push   edi
        push   eax
        call   sub_5_1F97
        add    esp, 8
        or     eax, eax
        je     SHORT loc_5_2062
        sub    ebx, ebx
loc_5_2016:
        mov    ecx, dword ptr [esp + 10h]
        push   ebp
        db  0A1h, 024h, 09Bh, 000h, 000h
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + ecx*4]
        add    eax, 16h
        push   eax
        push   esi
        db  0E8h, 0D8h, 0F7h, 0FAh, 0FFh
        add    esp, 0ch
        cmp    eax, ebx
        jne    SHORT loc_5_204C
        db  0A1h, 024h, 09Bh, 000h, 000h
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + ecx*4]
        cmp    byte ptr [ebp + eax + 16h], bl
        je     SHORT loc_5_206C
loc_5_204C:
        inc    dword ptr [esp + 10h]
        lea    eax, [esp + 10h]
        push   edi
        push   eax
        call   sub_5_1F97
        add    esp, 8
        cmp    eax, ebx
        jne    SHORT loc_5_2016
loc_5_2062:
        sub    eax, eax
loc_5_2064:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_5_206C:
        mov    eax, dword ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax], ecx
        mov    eax, 1
        jmp    SHORT loc_5_2064
sub_5_207D:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 20h]
        mov    ax, word ptr [ebp + 0eh]
        cmp    ax, 100h
        je     NEAR PTR loc_5_218A
        cmp    ax, 200h
        je     NEAR PTR loc_5_218A
        mov    ecx, dword ptr [ebp + 0ah]
        cmp    ecx, -1
        je     SHORT loc_5_20B1
        cmp    ch, 0e9h
        jne    NEAR PTR loc_5_218A
loc_5_20B1:
        movzx  eax, byte ptr [ebp + 10h]
        lea    eax, [ebp + eax + 11h]
        mov    dword ptr [esp + 14h], eax
        cmp    ecx, -1
        jne    SHORT loc_5_20CC
        mov    dword ptr [esp + 10h], 0
        jmp    SHORT loc_5_20D4
loc_5_20CC:
        shr    ecx, 10h
        inc    ecx
        mov    dword ptr [esp + 10h], ecx
loc_5_20D4:
        db  0A1h, 024h, 09Bh, 000h, 000h
        inc    dword ptr [eax + 0ch]
        mov    ax, word ptr [ebp + 0eh]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        call   sub_5_1F97
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_5_217B
        mov    eax, dword ptr [esp + 14h]
        inc    eax
        mov    dword ptr [esp + 18h], eax
loc_5_20FF:
        db  0A1h, 024h, 09Bh, 000h, 000h
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + ecx*4]
        mov    ecx, dword ptr [eax + 54h]
        sub    ecx, 3
        push   ecx
        add    eax, 16h
        push   eax
        db  0E8h, 0FEh, 021h, 0FBh, 0FFh
        mov    ebx, eax
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 8
        mov    byte ptr [eax], bl
        db  0A1h, 024h, 09Bh, 000h, 000h
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 4]
        mov    edi, dword ptr [esp + 18h]
        mov    esi, dword ptr [eax + ecx*4]
        add    esi, 16h
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        push   dword ptr [esp + 14h]
        lea    eax, [ebp + 10h]
        push   eax
        db  0E8h, 0C6h, 01Ch, 0FBh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_5_21A7
        inc    dword ptr [esp + 10h]
        mov    ax, word ptr [ebp + 0eh]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        call   sub_5_1F97
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_5_20FF
loc_5_217B:
        db  0A1h, 024h, 09Bh, 000h, 000h
        dec    dword ptr [eax + 0ch]
        mov    dword ptr [ebp + 0ah], 0ffffffffh
loc_5_218A:
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 2ch]
        push   ebp
        call   sub_5_1E6E
        add    esp, 10h
loc_5_219F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_5_21A7:
        mov    esi, dword ptr [esp + 24h]
        sub    eax, eax
        lea    edx, [esi + 0eh]
        mov    edi, edx
        mov    ecx, 0ch
        rep stosd
        mov    ecx, dword ptr [esp + 10h]
        push   ebx
        db  0A1h, 024h, 09Bh, 000h, 000h
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + ecx*4]
        add    eax, 16h
        push   eax
        push   edx
        db  0E8h, 0A1h, 0F6h, 0FAh, 0FFh
        mov    edx, dword ptr [esp + 1ch]
        add    esp, 0ch
        db  08Bh, 00Dh, 024h, 09Bh, 000h, 000h
        mov    ecx, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ecx + edx*4]
        mov    cx, word ptr [ecx + 14h]
        mov    word ptr [esi + 0ch], cx
        db  08Bh, 00Dh, 024h, 09Bh, 000h, 000h
        dec    dword ptr [ecx + 0ch]
        mov    ecx, dword ptr [esp + 10h]
        shl    ecx, 10h
        add    ecx, 0e900h
        mov    dword ptr [esi + 8], ecx
        mov    byte ptr [esi + 3eh], 1
        mov    byte ptr [esi + 3fh], 40h
        sub    ax, ax
        mov    ecx, dword ptr [esp + 28h]
        mov    byte ptr [esi + 40h], 0ffh
        mov    word ptr [ecx], 39h
        jmp    NEAR PTR loc_5_219F
loc_5_2224:
        sub    esp, 20h
        push   esi
        push   edi
        mov    dword ptr [esp + 18h], 0ffffffffh
        mov    dword ptr [esp + 8], 0
        mov    dword ptr [esp + 0ch], 0
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 07Dh, 072h, 0FBh, 0FFh
        mov    esi, dword ptr [esp + 30h]
        add    esp, 4
        mov    eax, dword ptr [esi + 0ah]
        add    esi, 0ah
        cmp    eax, -1
        je     SHORT loc_5_2292
        cmp    ah, 0e8h
        jne    NEAR PTR loc_5_23B6
        push   eax
        call   sub_5_123A
        mov    dword ptr [esp + 0ch], eax
        add    esp, 4
        mov    eax, dword ptr [esi]
        and    eax, 7f0000h
        shr    eax, 10h
        mov    dword ptr [esp + 18h], eax
        cmp    dword ptr [esp + 8], 0
        je     SHORT loc_5_229D
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 8], eax
loc_5_2292:
        cmp    dword ptr [esp + 8], 0
        jne    NEAR PTR loc_5_23EC
loc_5_229D:
        cmp    dword ptr [esp + 8], 0
        je     NEAR PTR loc_5_2372
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 30h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx + 8], eax
        mov    edx, ecx
        mov    eax, dword ptr [esp + 0ch]
        add    edx, 0eh
        mov    ax, word ptr [eax + 16h]
        mov    word ptr [ecx + 0ch], ax
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 0ch]
        movzx  eax, byte ptr [eax + 18h]
        mov    edi, edx
        add    esi, 19h
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 0ch]
        mov    cl, byte ptr [eax + 14h]
        mov    eax, dword ptr [esp + 30h]
        mov    byte ptr [eax + 3eh], cl
        mov    eax, dword ptr [esp + 0ch]
        mov    cl, byte ptr [eax + 15h]
        mov    eax, dword ptr [esp + 30h]
        mov    byte ptr [eax + 3fh], cl
        mov    eax, dword ptr [esp + 0ch]
        cmp    dword ptr [eax + 8], 0
        je     SHORT loc_5_2320
        mov    eax, dword ptr [esp + 30h]
        mov    byte ptr [eax + 40h], 0ffh
        jmp    SHORT loc_5_2353
loc_5_2320:
        mov    eax, dword ptr [esp + 0ch]
        add    eax, 0ch
        mov    dword ptr [esp + 20h], eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_5_234B
        mov    byte ptr [eax + 4], 2
        mov    eax, dword ptr [esp + 20h]
        mov    eax, dword ptr [eax]
        cmp    byte ptr [eax + 4], 0
        je     SHORT loc_5_2353
        mov    eax, dword ptr [esp + 30h]
        mov    byte ptr [eax + 40h], 0ffh
        jmp    SHORT loc_5_2353
loc_5_234B:
        mov    eax, dword ptr [esp + 30h]
        mov    byte ptr [eax + 40h], 0
loc_5_2353:
        mov    eax, dword ptr [esp + 34h]
        db  068h, 028h, 09Bh, 000h, 000h
        mov    word ptr [eax], 39h
        db  0E8h, 0A6h, 071h, 0FBh, 0FFh
        add    esp, 4
        sub    ax, ax
loc_5_236C:
        pop    edi
        pop    esi
        add    esp, 20h
        ret
loc_5_2372:
        mov    eax, dword ptr [esp + 18h]
        db  08Dh, 004h, 085h, 048h, 09Dh, 000h, 000h
        mov    dword ptr [esp + 14h], eax
        mov    eax, 4
loc_5_2386:
        add    dword ptr [esp + 14h], eax
        mov    ecx, dword ptr [esp + 14h]
        inc    dword ptr [esp + 18h]
        cmp    dword ptr [ecx], 0
        je     SHORT loc_5_2386
        mov    eax, dword ptr [esp + 18h]
        db  08Bh, 004h, 085h, 048h, 09Dh, 000h, 000h
        mov    dword ptr [esp + 1ch], eax
        cmp    eax, -1
        jne    SHORT loc_5_23DD
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [eax + 0ah], 0ffffffffh
loc_5_23B6:
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 04Ch, 071h, 0FBh, 0FFh
        add    esp, 4
        push   dword ptr [esp + 38h]
        push   dword ptr [esp + 38h]
        push   dword ptr [esp + 38h]
        push   dword ptr [esp + 38h]
        call   sub_5_207D
        add    esp, 10h
        jmp    SHORT loc_5_236C
loc_5_23DD:
        mov    eax, dword ptr [esp + 1ch]
loc_5_23E1:
        mov    dword ptr [esp + 8], eax
loc_5_23E5:
        cmp    dword ptr [esp + 8], 0
        je     SHORT loc_5_2372
loc_5_23EC:
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esp + 0ch], eax
        mov    eax, dword ptr [esp + 2ch]
        mov    ax, word ptr [eax + 0eh]
        mov    word ptr [esp + 10h], ax
        mov    eax, dword ptr [esp + 0ch]
        mov    cx, word ptr [esp + 10h]
        cmp    word ptr [eax + 16h], cx
        je     SHORT loc_5_241A
        cmp    cx, 0ffffh
        jne    SHORT loc_5_2482
loc_5_241A:
        mov    eax, dword ptr [esp + 0ch]
        add    eax, 18h
        push   eax
        mov    eax, dword ptr [esp + 30h]
        add    eax, 10h
        push   eax
        db  0E8h, 0F1h, 019h, 0FBh, 0FFh
        add    esp, 8
        or     eax, eax
        je     SHORT loc_5_2482
        mov    eax, dword ptr [esp + 0ch]
        db  08Bh, 00Dh, 0D8h, 09Fh, 000h, 000h
        cmp    dword ptr [eax + 10h], ecx
        jae    SHORT loc_5_245D
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [esp + 0ch]
        mov    esi, dword ptr [eax]
        call   sub_5_321B
        mov    dword ptr [esp + 0ch], esi
        add    esp, 4
        jmp    SHORT loc_5_23E5
loc_5_245D:
        push   1
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 14h]
        call   sub_5_1385
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_5_229D
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax]
        jmp    NEAR PTR loc_5_23E1
loc_5_2482:
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax]
        jmp    NEAR PTR loc_5_23E1
loc_5_248D:
        push   esi
        mov    eax, dword ptr [esp + 10h]
        db  068h, 028h, 09Bh, 000h, 000h
        mov    word ptr [eax], 5
        db  0E8h, 027h, 070h, 0FBh, 0FFh
        mov    ecx, dword ptr [esp + 18h]
        add    esp, 4
        mov    al, byte ptr [ecx + 6ah]
        or     al, al
        je     SHORT loc_5_24C8
        cmp    dword ptr [ecx + 14h], 0
        je     SHORT loc_5_24C8
        mov    dl, 11h
        imul   dl
        mov    edx, dword ptr [esp + 0ch]
        mov    byte ptr [edx + 8], al
        mov    eax, dword ptr [ecx + 14h]
        mov    dword ptr [edx + 9], eax
        jmp    SHORT loc_5_24D7
loc_5_24C8:
        mov    edx, dword ptr [esp + 0ch]
        mov    byte ptr [edx + 8], 0
        mov    dword ptr [edx + 9], 0ffffffffh
loc_5_24D7:
        sub    si, si
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 028h, 070h, 0FBh, 0FFh
        add    esp, 4
        mov    ax, si
        pop    esi
        ret
sub_5_24EC:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 14h]
        lea    esi, [eax + 1]
        movzx  eax, byte ptr [eax]
        push   eax
        push   esi
        db  0E8h, 01Ch, 01Eh, 0FBh, 0FFh
        mov    edi, eax
        mov    eax, dword ptr [esp + 18h]
        add    esp, 8
        mov    ebx, dword ptr [eax + 8]
        or     ebx, ebx
        je     SHORT loc_5_2530
loc_5_2510:
        movzx  eax, byte ptr [ebx + 14h]
        cmp    eax, edi
        jne    SHORT loc_5_252A
        push   edi
        lea    eax, [ebx + 15h]
        push   eax
        push   esi
        db  0E8h, 0E5h, 0F2h, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_2530
loc_5_252A:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    SHORT loc_5_2510
loc_5_2530:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_2536:
        push   esi
        mov    eax, dword ptr [esp + 8]
        or     eax, eax
        je     SHORT loc_5_2550
loc_5_253F:
        mov    esi, dword ptr [eax]
        push   eax
        db  0E8h, 049h, 01Eh, 0FBh, 0FFh
        add    esp, 4
        mov    eax, esi
        or     esi, esi
        jne    SHORT loc_5_253F
loc_5_2550:
        pop    esi
        ret
sub_5_2552:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [eax + 8]
        or     esi, esi
        je     SHORT loc_5_2580
loc_5_255F:
        db  0E8h, 04Dh, 06Eh, 0FBh, 0FFh
        mov    edi, esi
        mov    esi, dword ptr [esi]
        push   dword ptr [edi + 4]
        call   sub_5_2536
        add    esp, 4
        push   edi
        db  0E8h, 017h, 01Eh, 0FBh, 0FFh
        add    esp, 4
        or     esi, esi
        jne    SHORT loc_5_255F
loc_5_2580:
        pop    edi
        pop    esi
        ret
sub_5_2583:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 18h]
        movzx  eax, byte ptr [edi + 2]
        lea    esi, [edi + 2]
        push   eax
        lea    eax, [edi + 3]
        push   eax
        db  0E8h, 080h, 01Dh, 0FBh, 0FFh
        add    esp, 8
        mov    ebx, eax
        mov    byte ptr [esi], bl
        push   0fh
        push   esi
        call   sub_5_1475
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_5_25B8
        mov    ax, 0efh
        jmp    SHORT loc_5_2625
loc_5_25B8:
        mov    ebp, dword ptr [esp + 14h]
        push   esi
        push   ebp
        call   sub_5_24EC
        add    esp, 8
        or     eax, eax
        je     SHORT loc_5_25D0
        mov    ax, 0edh
        jmp    SHORT loc_5_2625
loc_5_25D0:
        push   201h
        lea    eax, [ebx + 15h]
        push   eax
        db  0E8h, 086h, 01Dh, 0FBh, 0FFh
        mov    esi, dword ptr [esp + 24h]
        add    esp, 8
        mov    dword ptr [esi], eax
        mov    ecx, eax
        or     ecx, ecx
        jne    SHORT loc_5_25F3
        mov    ax, 96h
        jmp    SHORT loc_5_2625
loc_5_25F3:
        mov    al, byte ptr [edi]
        mov    byte ptr [ecx + 12h], al
        mov    ecx, dword ptr [esi]
        mov    al, byte ptr [edi + 1]
        mov    byte ptr [ecx + 13h], al
        mov    eax, dword ptr [esi]
        push   ebx
        mov    byte ptr [eax + 14h], bl
        lea    eax, [edi + 3]
        push   eax
        mov    eax, dword ptr [esi]
        add    eax, 15h
        push   eax
        db  0E8h, 05Fh, 0F2h, 0FAh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp + 8], eax
        sub    ax, ax
loc_5_2625:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_262A:
        mov    ax, 0f7h
        ret
loc_5_262F:
        mov    ax, 0f1h
        ret
loc_5_2634:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, 1
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 07Eh, 06Eh, 0FBh, 0FFh
        mov    esi, dword ptr [esp + 20h]
        add    esp, 4
        lea    ebp, [esi + 0ch]
        push   ebp
        call   sub_5_12B4
        add    esp, 4
        lea    ecx, [esi + 0ah]
        push   ecx
        push   eax
        call   sub_5_12E9
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        jne    NEAR PTR loc_5_2739
        db  0A1h, 024h, 09Bh, 000h, 000h
        push   ebp
        add    dword ptr [eax + 0ch], ebx
        mov    ax, word ptr [esi + 0ah]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_5_1FD8
        add    esp, 0ch
        or     eax, eax
        db  0A1h, 024h, 09Bh, 000h, 000h
        je     SHORT loc_5_26EB
        dec    dword ptr [eax + 0ch]
        movzx  eax, byte ptr [ebp]
        cmp    dword ptr [esi + eax + 0dh], -1
        je     SHORT loc_5_26AB
        jmp    NEAR PTR loc_5_287B
loc_5_26AB:
        mov    edx, dword ptr [esp + 20h]
        sub    eax, eax
        lea    ecx, [edx + 8]
        mov    edi, ecx
        stosd
        stosd
        stosd
        stosd
        db  0BEh, 06Ch, 09Fh, 000h, 000h
        mov    edi, ecx
        movsd
        movsd
        movsd
        mov    byte ptr [edx + 18h], 1
        sub    si, si
        mov    byte ptr [edx + 19h], 40h
        mov    dword ptr [edx + 1ah], 1
        mov    byte ptr [edx + 1fh], al
        mov    eax, dword ptr [esp + 24h]
        mov    byte ptr [edx + 1eh], 0ffh
        mov    word ptr [eax], 18h
        jmp    NEAR PTR loc_5_287F
loc_5_26EB:
        dec    dword ptr [eax + 0ch]
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 014h, 06Eh, 0FBh, 0FFh
        mov    eax, dword ptr [esp + 2ch]
        add    esp, 4
        mov    ax, word ptr [eax + 1ah]
        test   al, 10h
        je     SHORT loc_5_2714
        test   al, 40h
        jne    SHORT loc_5_2714
        mov    si, 0fbh
        jmp    NEAR PTR loc_5_288C
loc_5_2714:
        push   0fch
        push   18h
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 2ch]
        push   0ffffh
        push   esi
        call   sub_5_15AB
        add    esp, 18h
        mov    si, ax
        jmp    NEAR PTR loc_5_288C
loc_5_2739:
        push   1
        push   dword ptr [esp + 2ch]
        push   edi
        call   sub_5_1385
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_5_2755
        mov    si, 0f2h
        jmp    NEAR PTR loc_5_287F
loc_5_2755:
        movzx  eax, byte ptr [ebp]
        lea    eax, [esi + eax + 0dh]
        mov    dword ptr [esp + 10h], eax
        mov    ebp, dword ptr [edi + 8]
        mov    ecx, dword ptr [eax]
        cmp    ecx, -1
        jne    SHORT loc_5_27BE
        mov    ebx, 1
        mov    eax, dword ptr [edi + 0ch]
        or     eax, eax
        je     SHORT loc_5_27E4
        cmp    byte ptr [eax + 4], 2
        jne    SHORT loc_5_27E4
        mov    edx, dword ptr [esp + 20h]
        sub    eax, eax
        lea    ecx, [edx + 8]
        mov    edi, ecx
        stosd
        stosd
        stosd
        stosd
        db  0BEh, 078h, 09Fh, 000h, 000h
        mov    edi, ecx
        movsd
        movsd
        movsd
        mov    byte ptr [edx + 18h], bl
        sub    si, si
        mov    byte ptr [edx + 19h], 40h
        mov    dword ptr [edx + 1ah], ebx
        mov    byte ptr [edx + 1fh], al
        mov    eax, dword ptr [esp + 24h]
        mov    byte ptr [edx + 1eh], 0ffh
        mov    word ptr [eax], 18h
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax], ebx
        jmp    NEAR PTR loc_5_287F
loc_5_27BE:
        mov    eax, dword ptr [edi + 0ch]
        or     eax, eax
        je     SHORT loc_5_27D0
        cmp    byte ptr [eax + 4], 2
        jne    SHORT loc_5_27D0
        mov    ebx, 2
loc_5_27D0:
        cmp    ebx, ecx
        ja     SHORT loc_5_27E4
loc_5_27D4:
        or     ebp, ebp
        je     NEAR PTR loc_5_287B
        mov    ebp, dword ptr [ebp]
        inc    ebx
        cmp    ebx, ecx
        jbe    SHORT loc_5_27D4
loc_5_27E4:
        or     ebp, ebp
        je     NEAR PTR loc_5_287B
        mov    esi, dword ptr [esp + 10h]
        add    esi, 4
loc_5_27F3:
        lea    eax, [ebp + 14h]
        push   eax
        push   esi
        db  0E8h, 023h, 016h, 0FBh, 0FFh
        add    esp, 8
        or     eax, eax
        je     SHORT loc_5_2818
        push   1
        push   dword ptr [esp + 2ch]
        push   ebp
        push   edi
        call   sub_5_13FB
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_5_2820
loc_5_2818:
        mov    ebp, dword ptr [ebp]
        inc    ebx
        or     ebp, ebp
        jne    SHORT loc_5_27F3
loc_5_2820:
        or     ebp, ebp
        je     SHORT loc_5_287B
        mov    edx, dword ptr [esp + 20h]
        sub    eax, eax
        lea    ecx, [edx + 8]
        mov    edi, ecx
        stosd
        stosd
        stosd
        stosd
        lea    esi, [ebp + 15h]
        movzx  eax, byte ptr [ebp + 14h]
        mov    edi, ecx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    al, byte ptr [ebp + 12h]
        mov    byte ptr [edx + 18h], al
        mov    al, byte ptr [ebp + 13h]
        mov    byte ptr [edx + 19h], al
        cmp    dword ptr [ebp + 4], 1
        cmc
        sbb    al, al
        mov    byte ptr [edx + 1eh], al
        cmp    dword ptr [ebp], 1
        cmc
        sbb    al, al
        mov    byte ptr [edx + 1fh], al
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [edx + 1ah], ebx
        sub    si, si
        mov    word ptr [eax], 18h
        jmp    SHORT loc_5_287F
loc_5_287B:
        mov    si, 0fbh
loc_5_287F:
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 083h, 06Ch, 0FBh, 0FFh
        add    esp, 4
loc_5_288C:
        mov    ax, si
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_5_2897:
        sub    esp, 8
        push   edi
        mov    eax, dword ptr [esp + 10h]
        add    eax, 4
        mov    dword ptr [esp + 8], eax
        mov    edi, dword ptr [esp + 8]
        mov    ecx, 20h
        xor    edx, edx
        mov    eax, dword ptr [esp + 14h]
loc_5_28B5:
        cmp    dword ptr [edi], eax
        je     SHORT loc_5_28C5
        cmp    dword ptr [edi], edx
        je     SHORT loc_5_28C5
        add    edi, 4
        loop   loc_5_28B5
        sub    edi, 4
loc_5_28C5:
        mov    dword ptr [esp + 4], edi
        sub    ecx, 20h
        neg    ecx
        mov    edi, dword ptr [esp + 18h]
        mov    dword ptr [edi], ecx
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_5_28E4
        mov    ax, 0dh
        jmp    SHORT loc_5_28F1
loc_5_28E4:
        cmp    dword ptr [esp + 14h], eax
        mov    ax, 1
        je     SHORT loc_5_28F1
        sub    ax, ax
loc_5_28F1:
        pop    edi
        add    esp, 8
        ret
loc_5_28F6:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 1ch]
        lea    ecx, [esp + 10h]
        movzx  eax, byte ptr [esi + 0ch]
        lea    ebx, [esi + 0ch]
        lea    edi, [eax + esi + 0dh]
        push   ecx
        movzx  eax, byte ptr [edi]
        lea    eax, [edi + eax + 1]
        push   eax
        call   sub_5_1CF9
        add    esp, 8
        mov    bp, ax
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 09Ah, 06Bh, 0FBh, 0FFh
        add    esp, 4
        push   ebx
        call   sub_5_12B4
        add    esp, 4
        lea    ecx, [esi + 0ah]
        push   ecx
        push   eax
        call   sub_5_12E9
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_5_2998
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 0B5h, 06Bh, 0FBh, 0FFh
        add    esp, 4
        mov    ax, word ptr [esi + 0ah]
        cmp    ax, 100h
        je     SHORT loc_5_2973
        cmp    ax, 200h
        je     SHORT loc_5_2973
        mov    si, 0fch
        jmp    NEAR PTR loc_5_2A49
loc_5_2973:
        push   0fch
        push   0
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 2ch]
        push   0ffffh
        push   esi
        call   sub_5_15AB
        add    esp, 18h
        mov    si, ax
        jmp    NEAR PTR loc_5_2A49
loc_5_2998:
        mov    esi, dword ptr [esp + 28h]
        push   1
        push   esi
        push   ebx
        call   sub_5_1385
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_5_29B5
        mov    si, 0f2h
        jmp    NEAR PTR loc_5_2A3C
loc_5_29B5:
        push   edi
        push   ebx
        call   sub_5_24EC
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_5_29CB
        mov    si, 0fbh
        jmp    SHORT loc_5_2A3C
loc_5_29CB:
        push   1
        push   esi
        push   edi
        push   ebx
        call   sub_5_13FB
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_5_29E2
        mov    si, 0f9h
        jmp    SHORT loc_5_2A3C
loc_5_29E2:
        mov    eax, dword ptr [ebx + 0ch]
        or     eax, eax
        je     SHORT loc_5_2A00
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_5_2A00
        push   2
        push   edi
        push   ebx
        call   eax
        add    esp, 0ch
        mov    si, ax
        or     si, si
        jne    SHORT loc_5_2A3C
loc_5_2A00:
        mov    si, bp
        or     bp, bp
        jne    SHORT loc_5_2A3C
        mov    esi, dword ptr [edi + 4]
        or     esi, esi
        je     SHORT loc_5_2A38
loc_5_2A0F:
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 14h]
        push   esi
        call   sub_5_2897
        add    esp, 0ch
        cmp    ax, 1
        je     SHORT loc_5_2A54
        cmp    ax, 0dh
        je     SHORT loc_5_2A38
        db  0E8h, 07Fh, 069h, 0FBh, 0FFh
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_5_2A0F
loc_5_2A38:
        mov    si, 0eah
loc_5_2A3C:
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 0C6h, 06Ah, 0FBh, 0FFh
        add    esp, 4
loc_5_2A49:
        mov    ax, si
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_2A54:
        sub    si, si
        jmp    SHORT loc_5_2A3C
loc_5_2A59:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 18h]
        sub    bx, bx
        movzx  eax, byte ptr [edi + 10h]
        lea    esi, [edi + 10h]
        lea    ebp, [eax + edi + 11h]
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 04Ch, 06Ah, 0FBh, 0FFh
        add    esp, 4
        push   esi
        call   sub_5_12B4
        add    esp, 4
        lea    ecx, [edi + 0eh]
        push   ecx
        push   eax
        call   sub_5_12E9
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_5_2B10
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 067h, 06Ah, 0FBh, 0FFh
        add    esp, 4
        mov    ax, word ptr [edi + 0eh]
        cmp    ax, 100h
        je     SHORT loc_5_2AC1
        cmp    ax, 200h
        je     SHORT loc_5_2AC1
        mov    si, 0fch
        jmp    NEAR PTR loc_5_2C1E
loc_5_2AC1:
        push   0fch
        push   258h
        push   0
        push   dword ptr [esp + 28h]
        push   0ffffh
        push   edi
        call   sub_5_15AB
        add    esp, 18h
        mov    si, ax
        or     si, si
        jne    NEAR PTR loc_5_2C1E
        mov    ecx, dword ptr [esp + 1ch]
        mov    ax, word ptr [ecx + 8]
        push   eax
        db  0E8h, 067h, 00Fh, 0FBh, 0FFh
        add    esp, 4
        shl    ax, 2
        mov    ecx, dword ptr [esp + 20h]
        add    ax, 2
        mov    word ptr [ecx], ax
        jmp    NEAR PTR loc_5_2C1E
loc_5_2B10:
        push   1
        push   dword ptr [esp + 28h]
        push   esi
        call   sub_5_1385
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_5_2B2C
        mov    si, 0f2h
        jmp    NEAR PTR loc_5_2C11
loc_5_2B2C:
        push   ebp
        push   esi
        call   sub_5_24EC
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_5_2B45
        mov    si, 0fbh
        jmp    NEAR PTR loc_5_2C11
loc_5_2B45:
        push   1
        push   dword ptr [esp + 28h]
        push   ebp
        push   esi
        call   sub_5_13FB
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_5_2B62
        mov    si, 0f9h
        jmp    NEAR PTR loc_5_2C11
loc_5_2B62:
        mov    eax, dword ptr [esi + 0ch]
        or     eax, eax
        je     SHORT loc_5_2B84
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_5_2B84
        push   2
        push   ebp
        push   esi
        call   eax
        add    esp, 0ch
        mov    si, ax
        or     si, si
        jne    NEAR PTR loc_5_2C11
loc_5_2B84:
        mov    esi, dword ptr [ebp + 4]
        or     esi, esi
        je     SHORT loc_5_2BBE
        cmp    dword ptr [edi + 0ah], -1
        jne    SHORT loc_5_2B95
        sub    ebp, ebp
        jmp    SHORT loc_5_2BC2
loc_5_2B95:
        or     esi, esi
        je     SHORT loc_5_2BBE
loc_5_2B99:
        sub    ebp, ebp
        lea    ecx, [esi + 4]
        mov    eax, dword ptr [edi + 0ah]
loc_5_2BA1:
        cmp    dword ptr [ecx], eax
        je     SHORT loc_5_2BBB
        add    ecx, 4
        inc    ebp
        cmp    ebp, 20h
        jb     SHORT loc_5_2BA1
        db  0E8h, 0FEh, 067h, 0FBh, 0FFh
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_5_2B99
        jmp    SHORT loc_5_2BC2
loc_5_2BBB:
        inc    ebp
        jmp    SHORT loc_5_2BC2
loc_5_2BBE:
        mov    ebp, dword ptr [esp + 10h]
loc_5_2BC2:
        or     esi, esi
        je     SHORT loc_5_2BEB
loc_5_2BC6:
        cmp    ebp, 1fh
        jbe    SHORT loc_5_2BD1
        sub    ebp, ebp
        mov    esi, dword ptr [esi]
        jmp    SHORT loc_5_2BE7
loc_5_2BD1:
        mov    eax, dword ptr [esi + ebp*4 + 4]
        inc    ebp
        or     eax, eax
        je     SHORT loc_5_2BEB
        mov    ecx, dword ptr [esp + 1ch]
        movzx  edx, bx
        mov    dword ptr [ecx + edx*4 + 0ah], eax
        inc    bx
loc_5_2BE7:
        or     esi, esi
        jne    SHORT loc_5_2BC6
loc_5_2BEB:
        push   ebx
        db  0E8h, 06Fh, 00Eh, 0FBh, 0FFh
        mov    ecx, dword ptr [esp + 20h]
        add    esp, 4
        mov    word ptr [ecx + 8], ax
        lea    eax, [ebx*4]
        mov    ecx, dword ptr [esp + 20h]
        add    ax, 2
        mov    word ptr [ecx], ax
        sub    si, si
loc_5_2C11:
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 0F1h, 068h, 0FBh, 0FFh
        add    esp, 4
loc_5_2C1E:
        mov    ax, si
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  083h, 0ECh, 004h, 053h, 056h, 057h, 055h, 08Bh, 074h, 024h, 018h, 02Bh, 0EDh, 08Bh, 05Eh, 004h
        db  03Bh, 0DDh, 08Bh, 07Ch, 024h, 01Ch, 074h, 030h, 08Bh, 0EBh, 08Dh, 044h, 024h, 010h, 050h, 057h
        db  053h, 0E8h, 048h, 0FCh, 0FFh, 0FFh, 083h, 0C4h, 00Ch, 066h, 03Dh, 001h, 000h, 074h, 013h, 066h
        db  03Dh, 00Dh, 000h, 074h, 04Ch, 0E8h, 04Eh, 067h, 0FBh, 0FFh, 08Bh, 01Bh, 00Bh, 0DBh, 075h, 0D8h
        db  0EBh, 006h, 066h, 0B8h, 0E9h, 000h, 0EBh, 047h, 068h, 001h, 002h, 000h, 000h, 068h, 084h, 000h
        db  000h, 000h, 0E8h, 0E4h, 016h, 0FBh, 0FFh, 083h, 0C4h, 008h, 08Bh, 0D8h, 00Bh, 0DBh, 075h, 006h
        db  066h, 0B8h, 096h, 000h, 0EBh, 029h, 00Bh, 0EDh, 074h, 005h, 089h, 05Dh, 000h, 0EBh, 003h, 089h
        db  05Eh, 004h, 089h, 05Eh, 008h, 066h, 0FFh, 046h, 00Ch, 0C7h, 044h, 024h, 010h, 000h, 000h, 000h
        db  000h, 08Bh, 044h, 024h, 010h, 089h, 07Ch, 083h, 004h, 0FFh, 046h, 00Eh, 066h, 02Bh, 0C0h, 05Dh
        db  05Fh, 05Eh, 05Bh, 083h, 0C4h, 004h, 0C3h
loc_5_2CC0:
        mov    ax, 0f8h
        ret
loc_5_2CC5:
        mov    ax, 0f8h
        ret
sub_5_2CCA:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 18h]
        mov    cl, byte ptr [esp + 1ch]
        movzx  eax, word ptr [ebx + 0ch]
        movzx  esi, cl
        lea    edx, [eax + 1]
        cmp    edx, esi
        jge    SHORT loc_5_2CF0
        mov    ax, 0ech
        jmp    NEAR PTR loc_5_2DD0
loc_5_2CF0:
        sub    eax, esi
        cmp    eax, -1
        jne    SHORT loc_5_2D49
        push   200h
        push   84h
        db  0E8h, 05Eh, 016h, 0FBh, 0FFh
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_5_2D18
        mov    ax, 96h
        jmp    NEAR PTR loc_5_2DD0
loc_5_2D18:
        mov    dword ptr [ebp], 0
        cmp    word ptr [ebx + 0ch], 0
        jne    SHORT loc_5_2D2E
        mov    dword ptr [ebx + 8], ebp
        mov    dword ptr [ebx + 4], ebp
        jmp    SHORT loc_5_2D36
loc_5_2D2E:
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [eax], ebp
        mov    dword ptr [ebx + 8], ebp
loc_5_2D36:
        mov    ax, word ptr [ebx + 0ch]
        inc    ax
        mov    word ptr [ebx + 0ch], ax
        movzx  eax, ax
        mov    dword ptr [esp + 10h], eax
        jmp    SHORT loc_5_2D66
loc_5_2D49:
        mov    ebp, dword ptr [ebx + 4]
        mov    dword ptr [esp + 10h], 1
        cmp    cl, 1
        jbe    SHORT loc_5_2D66
        lea    eax, [esi]
        dec    esi
        mov    dword ptr [esp + 10h], eax
loc_5_2D60:
        mov    ebp, dword ptr [ebp]
        dec    esi
        jne    SHORT loc_5_2D60
loc_5_2D66:
        mov    eax, dword ptr [esp + 28h]
        lea    edi, [ebp + 4]
        mov    esi, dword ptr [esp + 24h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        cmp    dword ptr [esp + 28h], 80h
        jae    SHORT loc_5_2DA8
        mov    edx, 80h
        sub    edx, eax
        mov    eax, dword ptr [esp + 28h]
        lea    edi, [ebp + eax + 4]
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
loc_5_2DA8:
        cmp    byte ptr [esp + 20h], 0
        je     SHORT loc_5_2DCD
        push   dword ptr [ebp]
        call   sub_5_2536
        add    esp, 4
        mov    dword ptr [ebp], 0
        mov    dword ptr [ebx + 8], ebp
        mov    ax, word ptr [esp + 10h]
        mov    word ptr [ebx + 0ch], ax
loc_5_2DCD:
        sub    ax, ax
loc_5_2DD0:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_5_2DD8:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 18h]
        mov    bl, 1
        mov    esi, dword ptr [esp + 1ch]
loc_5_2DE6:
        cmp    esi, 80h
        mov    ebp, 80h
        jae    SHORT loc_5_2DF5
        mov    ebp, esi
loc_5_2DF5:
        push   ebp
        push   edi
        push   0ffh
        push   ebx
        push   dword ptr [esp + 24h]
        call   sub_5_2CCA
        add    esp, 14h
        or     ax, ax
        jne    SHORT loc_5_2E18
        add    edi, ebp
        sub    esi, ebp
        inc    bl
        or     esi, esi
        jne    SHORT loc_5_2DE6
loc_5_2E18:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_2E1D:
        mov    ax, 0f8h
        ret
sub_5_2E22:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 18h]
        cmp    byte ptr [eax], 1
        jne    NEAR PTR loc_5_2EEF
        mov    cl, byte ptr [eax + 1]
        cmp    cl, 0bh
        je     SHORT loc_5_2E4D
        jbe    NEAR PTR loc_5_2EE9
        cmp    byte ptr [eax + 0dh], 0
        jne    NEAR PTR loc_5_2EE9
loc_5_2E4D:
        push   0bh
        db  068h, 084h, 09Fh, 000h, 000h
        add    eax, 2
        push   eax
        db  0E8h, 0ABh, 0E9h, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_5_2EE9
        db  0A1h, 024h, 09Bh, 000h, 000h
        inc    dword ptr [eax + 0ch]
        mov    eax, dword ptr [esp + 14h]
        lea    ecx, [eax + 0ch]
        push   ecx
        mov    ax, word ptr [eax + 0ah]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        call   sub_5_1FD8
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_2EDB
        mov    edx, dword ptr [esp + 1ch]
        sub    eax, eax
        lea    ebx, [edx + 8]
        mov    edi, ebx
        mov    ecx, 20h
        rep stosd
        db  0A1h, 024h, 09Bh, 000h, 000h
        mov    ecx, dword ptr [esp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    edi, ebx
        mov    esi, dword ptr [eax + ecx*4]
        add    esi, 46h
        movsd
        movsd
        movsd
        db  0A1h, 024h, 09Bh, 000h, 000h
        dec    dword ptr [eax + 0ch]
        mov    byte ptr [edx + 88h], 0
        mov    eax, dword ptr [esp + 20h]
        mov    byte ptr [edx + 89h], 1
        mov    word ptr [eax], 82h
        sub    ax, ax
        jmp    SHORT loc_5_2EF3
loc_5_2EDB:
        db  0A1h, 024h, 09Bh, 000h, 000h
        dec    dword ptr [eax + 0ch]
        mov    ax, 0fch
        jmp    SHORT loc_5_2EF3
loc_5_2EE9:
        mov    ax, 0fbh
        jmp    SHORT loc_5_2EF3
loc_5_2EEF:
        mov    ax, 0ech
loc_5_2EF3:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  053h, 056h, 057h, 055h, 08Bh, 074h, 024h, 014h, 08Dh, 046h, 00Ch, 050h, 0E8h, 0A9h, 0E3h, 0FFh
        db  0FFh, 083h, 0C4h, 004h, 08Dh, 04Eh, 00Ah, 051h, 050h, 0E8h, 0D1h, 0E3h, 0FFh, 0FFh, 083h, 0C4h
        db  008h, 08Bh, 0E8h, 00Bh, 0EDh, 075h, 017h, 0FFh, 074h, 024h, 020h, 0FFh, 074h, 024h, 020h, 0FFh
        db  074h, 024h, 020h, 056h, 0E8h, 0EFh, 0FEh, 0FFh, 0FFh, 083h, 0C4h, 010h, 0EBh, 042h, 083h, 07Dh
        db  00Ch, 000h, 075h, 006h, 066h, 0B8h, 0FBh, 000h, 0EBh, 036h, 08Bh, 05Ch, 024h, 01Ch, 02Bh, 0C0h
        db  08Dh, 053h, 008h, 08Bh, 0FAh, 0B9h, 020h, 000h, 000h, 000h, 0F3h, 0ABh, 08Bh, 075h, 00Ch, 08Bh
        db  0FAh, 083h, 0C6h, 010h, 0A5h, 0A5h, 0A5h, 088h, 083h, 088h, 000h, 000h, 000h, 08Bh, 044h, 024h
        db  020h, 0C6h, 083h, 089h, 000h, 000h, 000h, 001h, 066h, 0C7h, 000h, 082h, 000h, 066h, 02Bh, 0C0h
        db  05Dh, 05Fh, 05Eh, 05Bh, 0C3h
loc_5_2F7F:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 03Bh, 065h, 0FBh, 0FFh
        mov    ebx, dword ptr [esp + 18h]
        add    esp, 4
        movzx  eax, byte ptr [ebx + 0ch]
        lea    esi, [ebx + eax + 0dh]
        mov    ebp, dword ptr [esp + 18h]
        push   dword ptr [esp + 1ch]
        push   ebp
        push   esi
        push   ebx
        call   sub_5_2E22
        add    esp, 10h
        mov    di, ax
        or     di, di
        je     NEAR PTR loc_5_3188
        lea    eax, [ebx + 0ch]
        push   eax
        call   sub_5_12B4
        add    esp, 4
        lea    ecx, [ebx + 0ah]
        push   ecx
        push   eax
        call   sub_5_12E9
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        jne    NEAR PTR loc_5_306A
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 024h, 065h, 0FBh, 0FFh
        mov    eax, dword ptr [esp + 24h]
        add    esp, 4
        mov    ax, word ptr [eax + 1ah]
        test   al, 10h
        je     SHORT loc_5_2FFB
        test   al, 40h
        je     SHORT loc_5_300B
loc_5_2FFB:
        mov    ax, word ptr [ebx + 0ah]
        cmp    ax, 100h
        je     SHORT loc_5_301A
        cmp    ax, 200h
        je     SHORT loc_5_3014
loc_5_300B:
        mov    di, 0fch
        jmp    NEAR PTR loc_5_3195
loc_5_3014:
        cmp    ax, 100h
        jne    SHORT loc_5_3045
loc_5_301A:
        cmp    byte ptr [esi + 1], 0fh
        jne    SHORT loc_5_3045
        lea    edi, [esi + 2]
        push   0fh
        db  068h, 090h, 09Fh, 000h, 000h
        push   edi
        db  0E8h, 03Ch, 012h, 0FBh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_5_3045
        mov    byte ptr [esi + 1], 0dh
        db  0BEh, 0A0h, 09Fh, 000h, 000h
        movsd
        movsd
        movsd
        movsw
loc_5_3045:
        push   0fch
        push   82h
        push   dword ptr [esp + 24h]
        push   ebp
        push   0ffffh
        push   ebx
        call   sub_5_15AB
        add    esp, 18h
        mov    di, ax
        jmp    NEAR PTR loc_5_3195
loc_5_306A:
        mov    eax, dword ptr [edi + 0ch]
        or     eax, eax
        je     SHORT loc_5_30BE
        cmp    byte ptr [eax + 4], 2
        jne    SHORT loc_5_30BE
        cmp    byte ptr [esi + 1], 0bh
        jne    SHORT loc_5_30BE
        push   0bh
        db  068h, 0B0h, 09Fh, 000h, 000h
        lea    eax, [esi + 2]
        push   eax
        db  0E8h, 0DFh, 011h, 0FBh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_5_30BE
        mov    esi, dword ptr [edi + 0ch]
        lea    edi, [ebp + 8]
        add    esi, 10h
        movsd
        movsd
        movsd
        lea    edi, [ebp + 14h]
        sub    eax, eax
        mov    ecx, 1dh
        rep stosd
        mov    byte ptr [ebp + 88h], al
        mov    byte ptr [ebp + 89h], 1
        jmp    NEAR PTR loc_5_317C
loc_5_30BE:
        push   1
        push   dword ptr [esp + 24h]
        push   edi
        call   sub_5_1385
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_5_30DA
        mov    di, 0f2h
        jmp    NEAR PTR loc_5_3188
loc_5_30DA:
        lea    eax, [esi + 1]
        push   eax
        push   edi
        call   sub_5_24EC
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_5_30F6
        mov    di, 0fbh
        jmp    NEAR PTR loc_5_3188
loc_5_30F6:
        push   1
        push   dword ptr [esp + 24h]
        push   ebx
        push   edi
        call   sub_5_13FB
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_5_3110
        mov    di, 0f9h
        jmp    SHORT loc_5_3188
loc_5_3110:
        mov    eax, dword ptr [edi + 0ch]
        or     eax, eax
        je     SHORT loc_5_312E
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_5_312E
        push   2
        push   ebx
        push   edi
        call   eax
        add    esp, 0ch
        mov    di, ax
        or     di, di
        jne    SHORT loc_5_3188
loc_5_312E:
        mov    al, byte ptr [esi]
        movzx  si, al
        cmp    word ptr [ebx + 0ch], si
        jae    SHORT loc_5_3140
        mov    di, 0ech
        jmp    SHORT loc_5_3188
loc_5_3140:
        mov    edx, dword ptr [ebx + 4]
        mov    ecx, 1
        cmp    al, cl
        jbe    SHORT loc_5_315A
loc_5_314C:
        mov    edx, dword ptr [edx]
        or     edx, edx
        je     SHORT loc_5_319D
        inc    ecx
        movzx  eax, si
        cmp    eax, ecx
        ja     SHORT loc_5_314C
loc_5_315A:
        lea    esi, [edx + 4]
        lea    edi, [ebp + 8]
        mov    ecx, 20h
        rep movsd
        cmp    dword ptr [edx], 1
        cmc
        sbb    al, al
        mov    byte ptr [ebp + 88h], al
        mov    al, byte ptr [ebx + 12h]
        mov    byte ptr [ebp + 89h], al
loc_5_317C:
        mov    eax, dword ptr [esp + 1ch]
        sub    di, di
        mov    word ptr [eax], 82h
loc_5_3188:
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 07Ah, 063h, 0FBh, 0FFh
        add    esp, 4
loc_5_3195:
        mov    ax, di
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_319D:
        mov    di, 0ech
        jmp    SHORT loc_5_3188
loc_5_31A3:
        mov    ax, 0f6h
        ret
sub_5_31A8:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        lea    eax, [esi + 18h]
        push   eax
        call   sub_5_12B4
        add    esp, 4
        mov    edi, eax
        mov    eax, dword ptr [esi + 0ch]
        or     eax, eax
        je     SHORT loc_5_31EA
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_5_31D8
        push   1
        push   0
        push   esi
        call   eax
        add    esp, 0ch
        or     ax, ax
        jne    SHORT loc_5_3213
loc_5_31D8:
        push   dword ptr [esi + 0ch]
        db  0E8h, 0B0h, 011h, 0FBh, 0FFh
        add    esp, 4
        mov    dword ptr [esi + 0ch], 0
loc_5_31EA:
        push   esi
        call   sub_5_2552
        add    esp, 4
        push   dword ptr [esi]
        call   sub_5_1277
        add    esp, 4
        push   esi
        push   edi
        call   sub_5_135D
        add    esp, 8
        push   esi
        db  0E8h, 083h, 011h, 0FBh, 0FFh
        add    esp, 4
        sub    ax, ax
loc_5_3213:
        pop    edi
        pop    esi
        ret
loc_5_3216:
        mov    ax, 0f4h
        ret
sub_5_321B:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        lea    eax, [edi + 18h]
        push   eax
        call   sub_5_12B4
        add    esp, 4
        mov    esi, eax
        push   edi
        call   sub_5_2552
        add    esp, 4
        push   dword ptr [edi]
        call   sub_5_1277
        add    esp, 4
        push   edi
        push   esi
        call   sub_5_135D
        add    esp, 8
        push   edi
        call   sub_5_3258
        add    esp, 4
        pop    edi
        pop    esi
        ret
sub_5_3258:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 0ch], 0
        je     SHORT loc_5_328A
        db  0FFh, 00Dh, 0E8h, 09Fh, 000h, 000h
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 8
        push   eax
        db  0E8h, 0EBh, 0CDh, 0FAh, 0FFh
        add    esp, 4
        push   dword ptr [esi + 0ch]
        db  0E8h, 010h, 011h, 0FBh, 0FFh
        add    esp, 4
        mov    dword ptr [esi + 0ch], 0
loc_5_328A:
        push   esi
        db  0E8h, 000h, 011h, 0FBh, 0FFh
        add    esp, 4
        pop    esi
        ret
        db  000h, 000h, 000h
sub_5_3298:
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        clc
        test   al, al
        je     SHORT loc_5_32FA
        db  0A2h, 004h, 000h, 000h, 000h
        push   1
        push   1
        db  068h, 004h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 042h, 000h, 040h, 000h
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_5_32CB
        test   edx, 2
        je     SHORT loc_5_32D2
        stc
loc_5_32CB:
        db  0A0h, 004h, 000h, 000h, 000h
        jmp    SHORT loc_5_32FA
loc_5_32D2:
        push   2
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 043h, 000h, 040h, 000h
        add    esp, 0ch
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 044h, 000h, 040h, 000h
        add    esp, 4
        test   ah, ah
        jne    SHORT loc_5_32CB
loc_5_32FA:
        movzx  eax, al
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret
sub_5_3303:
        push   ebx
        push   esi
        push   edi
        cld
        db  08Dh, 03Dh, 038h, 0A0h, 000h, 000h
        mov    ecx, 100h
loc_5_3312:
        mov    al, byte ptr [edi]
        call   sub_5_3298
        jae    SHORT loc_5_3327
        db  0C7h, 005h, 034h, 0A0h, 000h, 000h, 001h, 000h, 000h, 000h
        mov    al, 0
loc_5_3327:
        stosb
        loop   loc_5_3312
        xor    eax, eax
        movzx  eax, al
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_5_3340:
        push   esi
        mov    esi, dword ptr [esp + 8]
        lea    eax, [esi + 4]
        push   eax
        db  0E8h, 006h, 007h, 0FBh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_5_336F
        cmp    byte ptr [esi + 1dh], 0
        je     SHORT loc_5_3366
        push   dword ptr [esi + 20h]
        db  0E8h, 02Dh, 010h, 0FBh, 0FFh
        add    esp, 4
loc_5_3366:
        push   esi
        db  0E8h, 024h, 010h, 0FBh, 0FFh
        add    esp, 4
loc_5_336F:
        pop    esi
        ret
sub_5_3371:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        db  068h, 054h, 0A1h, 000h, 000h
        lea    ebx, [ecx - 1]
        db  0E8h, 0AAh, 060h, 0FBh, 0FFh
        add    esp, 4
        db  08Bh, 03Dh, 050h, 0A1h, 000h, 000h
        or     edi, edi
        je     SHORT loc_5_33D0
loc_5_339F:
        cmp    byte ptr [edi + 0ch], bl
        jne    SHORT loc_5_33B9
        movzx  eax, bl
        push   eax
        push   esi
        lea    eax, [edi + 0dh]
        push   eax
        db  0E8h, 056h, 0E4h, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_33BF
loc_5_33B9:
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    SHORT loc_5_339F
loc_5_33BF:
        or     edi, edi
        je     SHORT loc_5_33D0
        mov    eax, dword ptr [esp + 14h]
        or     eax, eax
        je     SHORT loc_5_33D0
        mov    dword ptr [eax], edi
        inc    dword ptr [edi + 4]
loc_5_33D0:
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 0B6h, 060h, 0FBh, 0FFh
        add    esp, 4
        cmp    edi, 1
        sbb    eax, eax
        inc    eax
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_33E7:
        push   edi
        mov    edx, dword ptr [esp + 8]
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 3
        jl     SHORT loc_5_3435
        cmp    ecx, 80h
        jg     SHORT loc_5_3435
        mov    al, byte ptr [edx]
        cmp    al, 41h
        jb     SHORT loc_5_3435
        cmp    al, 5ah
        ja     SHORT loc_5_3435
        cmp    byte ptr [edx + 1], 3ah
        jne    SHORT loc_5_3435
        cmp    byte ptr [edx + 2], 5ch
        jne    SHORT loc_5_3435
        movzx  eax, al
        sub    eax, 40h
        push   eax
        db  0E8h, 067h, 007h, 0FBh, 0FFh
        add    esp, 4
        test   al, 14h
        mov    eax, 1
        je     SHORT loc_5_3437
loc_5_3435:
        sub    eax, eax
loc_5_3437:
        pop    edi
        ret
sub_5_3439:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        sub    edi, edi
        mov    al, byte ptr [esi]
        or     al, al
        je     SHORT loc_5_347F
loc_5_3447:
        movzx  eax, al
        db  08Ah, 080h, 038h, 0A0h, 000h, 000h
        or     al, al
        jne    SHORT loc_5_345D
        inc    edi
        cmp    byte ptr [esi + edi], 0
        jne    SHORT loc_5_3477
        jmp    SHORT loc_5_3490
loc_5_345D:
        cmp    al, 20h
        jbe    SHORT loc_5_3490
        sub    ecx, ecx
        movzx  eax, al
loc_5_3466:
        db  00Fh, 0BEh, 091h, 078h, 0A1h, 000h, 000h
        cmp    edx, eax
        je     SHORT loc_5_3490
        inc    ecx
        cmp    ecx, 13h
        jb     SHORT loc_5_3466
loc_5_3477:
        inc    edi
        mov    al, byte ptr [esi + edi]
        or     al, al
        jne    SHORT loc_5_3447
loc_5_347F:
        cmp    edi, 2
        jl     SHORT loc_5_3490
        cmp    edi, 10h
        jg     SHORT loc_5_3490
        mov    eax, 1
        jmp    SHORT loc_5_3492
loc_5_3490:
        sub    eax, eax
loc_5_3492:
        pop    edi
        pop    esi
        ret
sub_5_3495:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    eax, [esp + 10h]
        mov    esi, dword ptr [esp + 24h]
        push   eax
        push   esi
        call   sub_5_3371
        add    esp, 8
        or     eax, eax
        je     SHORT loc_5_34D1
        mov    eax, dword ptr [esp + 10h]
        test   byte ptr [eax + 8], 8
        je     SHORT loc_5_3511
        push   eax
        call   sub_5_3824
        add    esp, 4
        push   dword ptr [esp + 10h]
        call   sub_5_3340
        add    esp, 4
loc_5_34D1:
        mov    edi, dword ptr [esp + 28h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    edi, esi
        lea    ebx, [ecx - 1]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        push   dword ptr [esp + 28h]
        repne scasb
        not    ecx
        lea    eax, [ecx - 1]
        mov    dword ptr [esp + 18h], eax
        call   sub_5_33E7
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_5_3527
        mov    eax, 9ch
        jmp    NEAR PTR loc_5_37F7
loc_5_3511:
        push   dword ptr [esp + 10h]
        call   sub_5_3340
        add    esp, 4
        mov    eax, 0eeh
        jmp    NEAR PTR loc_5_37F7
loc_5_3527:
        push   esi
        call   sub_5_3439
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_5_353E
        mov    eax, 0efh
        jmp    NEAR PTR loc_5_37F7
loc_5_353E:
        cmp    dword ptr [esp + 14h], 3
        jne    SHORT loc_5_355E
        push   3
        db  068h, 0ACh, 0A1h, 000h, 000h
        push   esi
        db  0E8h, 0B6h, 0E2h, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        mov    edi, 1
        je     SHORT loc_5_3560
loc_5_355E:
        sub    edi, edi
loc_5_3560:
        push   1
        lea    eax, [ebx + 26h]
        push   eax
        db  0E8h, 0F9h, 00Dh, 0FBh, 0FFh
        mov    dword ptr [esp + 18h], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_5_3580
        mov    eax, 96h
        jmp    NEAR PTR loc_5_37F7
loc_5_3580:
        mov    eax, dword ptr [esp + 10h]
        push   dword ptr [esp + 14h]
        mov    dword ptr [eax + 4], 1
        mov    ecx, dword ptr [esp + 14h]
        mov    al, byte ptr [esp + 18h]
        mov    byte ptr [ecx + 0ch], al
        mov    eax, dword ptr [esp + 14h]
        push   esi
        add    eax, 0dh
        push   eax
        db  0E8h, 0CCh, 0E2h, 0FAh, 0FFh
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 0ch
        push   ebx
        mov    byte ptr [eax + 24h], bl
        mov    eax, dword ptr [esp + 14h]
        push   dword ptr [esp + 2ch]
        add    eax, 25h
        push   eax
        db  0E8h, 0C4h, 00Ch, 0FBh, 0FFh
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 0ch
        movzx  eax, byte ptr [eax + 24h]
        add    eax, dword ptr [esp + 10h]
        add    eax, 25h
        push   eax
        mov    eax, dword ptr [esp + 14h]
        add    eax, 25h
        push   eax
        db  0E8h, 02Ch, 0E1h, 0FAh, 0FFh
        add    esp, 8
        cmp    byte ptr [eax], 5ch
        je     SHORT loc_5_3605
        mov    eax, dword ptr [esp + 10h]
        inc    ebx
        movzx  eax, byte ptr [eax + 24h]
        mov    ecx, dword ptr [esp + 10h]
        mov    byte ptr [eax + ecx + 25h], 5ch
        mov    eax, dword ptr [esp + 10h]
        inc    byte ptr [eax + 24h]
loc_5_3605:
        mov    eax, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 2ch]
        mov    dword ptr [eax + 8], esi
        cmp    dword ptr [esp + 34h], 0
        je     SHORT loc_5_365D
        cmp    dword ptr [esp + 34h], 100h
        jae    SHORT loc_5_365D
        push   0
        push   dword ptr [esp + 38h]
        db  0E8h, 038h, 00Dh, 0FBh, 0FFh
        mov    ecx, dword ptr [esp + 18h]
        add    esp, 8
        mov    dword ptr [ecx + 20h], eax
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [eax + 20h]
        or     ecx, ecx
        je     SHORT loc_5_365D
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 34h]
        push   ecx
        db  0E8h, 039h, 00Ch, 0FBh, 0FFh
        mov    ecx, dword ptr [esp + 1ch]
        mov    al, byte ptr [esp + 40h]
        add    esp, 0ch
        mov    byte ptr [ecx + 1dh], al
loc_5_365D:
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 061h, 05Eh, 0FBh, 0FFh
        add    esp, 4
        db  0BAh, 050h, 0A1h, 000h, 000h
        db  08Bh, 00Dh, 050h, 0A1h, 000h, 000h
        or     edi, edi
        je     SHORT loc_5_368C
        mov    eax, ecx
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 10h]
        db  0A3h, 050h, 0A1h, 000h, 000h
        jmp    SHORT loc_5_36CB
loc_5_368C:
        test   esi, 1
        je     SHORT loc_5_36A8
        or     ecx, ecx
        je     SHORT loc_5_36A0
loc_5_3698:
        mov    edx, ecx
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    SHORT loc_5_3698
loc_5_36A0:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [edx], eax
        jmp    SHORT loc_5_36CB
loc_5_36A8:
        or     ecx, ecx
        je     SHORT loc_5_36BF
loc_5_36AC:
        mov    eax, dword ptr [ecx + 8]
        test   al, 1
        je     SHORT loc_5_36B7
        test   al, 8
        je     SHORT loc_5_36BF
loc_5_36B7:
        mov    edx, ecx
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    SHORT loc_5_36AC
loc_5_36BF:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax], ecx
loc_5_36CB:
        sub    dl, dl
        db  08Bh, 00Dh, 050h, 0A1h, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_5_36F4
loc_5_36D7:
        mov    eax, dword ptr [ecx + 8]
        test   al, 1
        je     SHORT loc_5_36E9
        test   al, 8
        jne    SHORT loc_5_36E9
        cmp    dl, 80h
        jae    SHORT loc_5_36E9
        mov    dl, 80h
loc_5_36E9:
        mov    byte ptr [ecx + 1eh], dl
        inc    dl
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    SHORT loc_5_36D7
loc_5_36F4:
        db  0BFh, 0ACh, 0B1h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   200h
        lea    eax, [ecx - 1]
        mov    dword ptr [esp + 20h], eax
        lea    eax, [ebx + 1]
        push   eax
        db  0E8h, 04Bh, 00Ch, 0FBh, 0FFh
        mov    dword ptr [esp + 20h], eax
        add    esp, 8
        mov    eax, dword ptr [esp + 1ch]
        add    eax, dword ptr [esp + 14h]
        push   200h
        add    eax, 4
        push   eax
        db  0E8h, 02Eh, 00Ch, 0FBh, 0FFh
        add    esp, 8
        mov    ebp, eax
        cmp    dword ptr [esp + 18h], 0
        je     NEAR PTR loc_5_37E8
        or     ebp, ebp
        je     NEAR PTR loc_5_37E8
        cmp    ebx, 3
        jbe    SHORT loc_5_3754
        dec    ebx
loc_5_3754:
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 18h]
        add    esi, 25h
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 18h]
        mov    edx, dword ptr [esp + 1ch]
        add    edx, ebp
        mov    byte ptr [ebx + eax], 0
        mov    byte ptr [edx + 2], 5ch
        mov    byte ptr [ebp + 1], 5ch
        mov    byte ptr [ebp], 5ch
        db  0BEh, 0ACh, 0B1h, 000h, 000h
        mov    eax, dword ptr [esp + 1ch]
        lea    edi, [ebp + 2]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 14h]
        mov    esi, dword ptr [esp + 10h]
        lea    edi, [edx + 3]
        add    esi, 0dh
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 14h]
        mov    byte ptr [edx + eax + 3], 0
        push   dword ptr [esp + 18h]
        push   ebp
        db  0E8h, 040h, 00Ch, 0FBh, 0FFh
        add    esp, 8
        push   ebp
        db  0E8h, 0B7h, 00Bh, 0FBh, 0FFh
        add    esp, 4
        push   dword ptr [esp + 18h]
        db  0E8h, 0ABh, 00Bh, 0FBh, 0FFh
        add    esp, 4
loc_5_37E8:
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 01Ah, 05Dh, 0FBh, 0FFh
        add    esp, 4
        sub    eax, eax
loc_5_37F7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_5_37FF:
        db  083h, 03Dh, 0D4h, 0B3h, 000h, 000h, 000h
        mov    eax, 0ffh
        je     SHORT loc_5_3823
        push   0
        push   0
        push   0
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        call   sub_5_3495
        add    esp, 14h
loc_5_3823:
        ret
sub_5_3824:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    esi, esi
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 091h, 05Ch, 0FBh, 0FFh
        db  0C7h, 044h, 024h, 014h, 050h, 0A1h, 000h, 000h
        add    esp, 4
        db  0A1h, 050h, 0A1h, 000h, 000h
        cmp    eax, esi
        je     SHORT loc_5_385D
        mov    ebx, dword ptr [esp + 1ch]
loc_5_384F:
        cmp    ebx, eax
        je     SHORT loc_5_3886
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_5_384F
loc_5_385D:
        mov    edi, 0ffh
loc_5_3862:
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 0A0h, 05Ch, 0FBh, 0FFh
        add    esp, 4
        or     edi, edi
        jne    SHORT loc_5_387C
        or     esi, esi
        je     SHORT loc_5_387C
        db  0E8h, 08Ah, 0ECh, 0FAh, 0FFh
loc_5_387C:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_3886:
        db  0BFh, 0ACh, 0B1h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   200h
        lea    eax, [ecx - 1]
        mov    dword ptr [esp + 18h], eax
        movzx  eax, byte ptr [ebx + 0ch]
        add    eax, dword ptr [esp + 18h]
        add    eax, 4
        push   eax
        db  0E8h, 0B1h, 00Ah, 0FBh, 0FFh
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_5_391F
        mov    eax, dword ptr [esp + 14h]
        add    eax, ebp
        mov    edx, dword ptr [esp + 14h]
        mov    byte ptr [eax + 2], 5ch
        mov    byte ptr [ebp + 1], 5ch
        mov    byte ptr [ebp], 5ch
        db  0BEh, 0ACh, 0B1h, 000h, 000h
        lea    edi, [ebp + 2]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        movzx  edx, byte ptr [ebx + 0ch]
        lea    esi, [ebx + 0dh]
        lea    edi, [eax + 3]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        movzx  eax, byte ptr [ebx + 0ch]
        add    eax, dword ptr [esp + 14h]
        mov    byte ptr [ebp + eax + 3], 0
        push   ebp
        db  0E8h, 003h, 00Bh, 0FBh, 0FFh
        add    esp, 4
        push   ebp
        db  0E8h, 074h, 00Ah, 0FBh, 0FFh
        add    esp, 4
loc_5_391F:
        test   byte ptr [ebx + 8], 8
        jne    SHORT loc_5_3947
        cmp    byte ptr [ebx + 0ch], 3
        jne    SHORT loc_5_3947
        push   3
        db  068h, 0B0h, 0A1h, 000h, 000h
        lea    eax, [ebx + 0dh]
        push   eax
        db  0E8h, 0CDh, 0DEh, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        mov    esi, 1
        je     SHORT loc_5_3949
loc_5_3947:
        sub    esi, esi
loc_5_3949:
        or     dword ptr [ebx + 8], 10000h
        mov    byte ptr [ebx + 1eh], 0ffh
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [eax], ecx
        push   ebx
        call   sub_5_3340
        add    esp, 4
        sub    dl, dl
        db  08Bh, 00Dh, 050h, 0A1h, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_5_398E
loc_5_3971:
        mov    eax, dword ptr [ecx + 8]
        test   al, 1
        je     SHORT loc_5_3983
        test   al, 8
        jne    SHORT loc_5_3983
        cmp    dl, 80h
        jae    SHORT loc_5_3983
        mov    dl, 80h
loc_5_3983:
        mov    byte ptr [ecx + 1eh], dl
        inc    dl
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    SHORT loc_5_3971
loc_5_398E:
        sub    edi, edi
        jmp    NEAR PTR loc_5_3862
loc_5_3995:
        sub    esp, 4
        push   esi
        lea    eax, [esp + 4]
        push   eax
        mov    esi, 0ffh
        push   dword ptr [esp + 10h]
        call   sub_5_3371
        add    esp, 8
        or     eax, eax
        je     SHORT loc_5_39D4
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax + 8], 1
        jne    SHORT loc_5_39D4
        push   eax
        call   sub_5_3824
        add    esp, 4
        mov    esi, eax
        push   dword ptr [esp + 4]
        call   sub_5_3340
        add    esp, 4
loc_5_39D4:
        mov    eax, esi
        pop    esi
        add    esp, 4
        ret
loc_5_39DB:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 14h]
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        db  068h, 054h, 0A1h, 000h, 000h
        lea    edi, [ecx - 1]
        db  0E8h, 03Fh, 05Ah, 0FBh, 0FFh
        add    esp, 4
        db  08Bh, 035h, 050h, 0A1h, 000h, 000h
        or     edi, edi
        je     SHORT loc_5_3A1D
        lea    eax, [ebx + edi]
        push   eax
        push   ebx
        db  0E8h, 0FCh, 0DCh, 0FAh, 0FFh
        add    esp, 8
        cmp    byte ptr [eax], 5ch
        jne    SHORT loc_5_3A1D
        dec    edi
loc_5_3A1D:
        or     esi, esi
        je     SHORT loc_5_3A4D
        mov    ebp, 1
loc_5_3A26:
        test   dword ptr [esi + 8], ebp
        jne    SHORT loc_5_3A47
        movzx  eax, byte ptr [esi + 24h]
        sub    eax, edi
        cmp    eax, ebp
        jne    SHORT loc_5_3A47
        push   edi
        push   ebx
        lea    eax, [esi + 25h]
        push   eax
        db  0E8h, 0C8h, 0DDh, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_3A6A
loc_5_3A47:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_5_3A26
loc_5_3A4D:
        mov    edi, dword ptr [esp + 18h]
        sub    esi, esi
        mov    byte ptr [edi], 0
loc_5_3A56:
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 030h, 05Ah, 0FBh, 0FFh
        add    esp, 4
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_3A6A:
        movzx  eax, byte ptr [esi + 0ch]
        push   eax
        lea    eax, [esi + 0dh]
        mov    edi, dword ptr [esp + 1ch]
        push   eax
        push   edi
        db  0E8h, 00Bh, 008h, 0FBh, 0FFh
        add    esp, 0ch
        movzx  eax, byte ptr [esi + 0ch]
        mov    byte ptr [edi + eax], 0
        mov    esi, 1
        jmp    SHORT loc_5_3A56
loc_5_3A8F:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 09Ch, 059h, 0FBh, 0FFh
        add    esp, 4
        db  08Bh, 015h, 050h, 0A1h, 000h, 000h
        or     edx, edx
        je     SHORT loc_5_3B20
        mov    eax, 1
loc_5_3AB2:
        test   dword ptr [edx + 8], eax
        jne    SHORT loc_5_3ABC
        or     esi, esi
        je     SHORT loc_5_3AC2
        dec    esi
loc_5_3ABC:
        mov    edx, dword ptr [edx]
        or     edx, edx
        jne    SHORT loc_5_3AB2
loc_5_3AC2:
        or     edx, edx
        je     SHORT loc_5_3B20
        movzx  eax, byte ptr [edx + 24h]
        lea    esi, [edx + 25h]
        mov    ebx, dword ptr [esp + 14h]
        mov    edi, ebx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    al, byte ptr [edx + 24h]
        cmp    al, 3
        movzx  eax, al
        jbe    SHORT loc_5_3AF2
        mov    byte ptr [eax + ebx - 1], 0
        jmp    SHORT loc_5_3AF6
loc_5_3AF2:
        mov    byte ptr [eax + ebx], 0
loc_5_3AF6:
        movzx  eax, byte ptr [edx + 0ch]
        lea    esi, [edx + 0dh]
        mov    ebx, dword ptr [esp + 18h]
        mov    edi, ebx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        movzx  eax, byte ptr [edx + 0ch]
        mov    byte ptr [ebx + eax], 0
        mov    esi, 1
        jmp    SHORT loc_5_3B30
loc_5_3B20:
        mov    ebx, dword ptr [esp + 14h]
        sub    esi, esi
        mov    byte ptr [ebx], 0
        mov    ebx, dword ptr [esp + 18h]
        mov    byte ptr [ebx], 0
loc_5_3B30:
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 056h, 059h, 0FBh, 0FFh
        add    esp, 4
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_3B43:
        sub    esp, 4
        push   esi
        lea    eax, [esp + 4]
        push   eax
        push   dword ptr [esp + 10h]
        call   sub_5_3371
        add    esp, 8
        mov    esi, eax
        cmp    dword ptr [esp + 4], 0
        je     SHORT loc_5_3B79
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax + 8], 8
        je     SHORT loc_5_3B6D
        sub    esi, esi
loc_5_3B6D:
        push   dword ptr [esp + 4]
        call   sub_5_3340
        add    esp, 4
loc_5_3B79:
        mov    eax, esi
        pop    esi
        add    esp, 4
        ret
sub_5_3B80:
        mov    ax, word ptr [esp + 4]
        and    eax, 300h
        ret
loc_5_3B8B:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 28h]
        cmp    byte ptr [eax + 6ah], 3
        je     SHORT loc_5_3BAC
        test   byte ptr [eax + 1ah], 10h
        jne    SHORT loc_5_3BAC
        mov    eax, 0c6h
        jmp    NEAR PTR loc_5_3CE7
loc_5_3BAC:
        push   0
        push   300h
        db  0E8h, 0ACh, 007h, 0FBh, 0FFh
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_5_3BCB
        mov    eax, 0ffh
        jmp    NEAR PTR loc_5_3CE7
loc_5_3BCB:
        lea    eax, [ebx + 100h]
        mov    ebp, dword ptr [esp + 1ch]
        mov    dword ptr [esp + 10h], eax
        movzx  eax, byte ptr [ebp + 0ah]
        lea    esi, [ebp + 0bh]
        mov    edi, ebx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        movzx  eax, byte ptr [ebp + 0ah]
        mov    byte ptr [ebx + eax], 0
        movzx  eax, byte ptr [ebp + 0ah]
        lea    edx, [ebp + eax + 0bh]
        mov    edi, dword ptr [esp + 10h]
        movzx  eax, byte ptr [edx]
        lea    esi, [edx + 1]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        movzx  ecx, byte ptr [edx]
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [ecx + eax], 0
        lea    ecx, [ebx + 200h]
        movzx  eax, byte ptr [edx]
        lea    edx, [edx + eax + 1]
        mov    dword ptr [esp + 14h], ecx
        movzx  eax, byte ptr [edx]
        mov    edi, ecx
        lea    esi, [edx + 1]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        movzx  eax, byte ptr [edx]
        mov    ecx, dword ptr [esp + 14h]
        mov    byte ptr [eax + ecx], 0
        mov    ax, word ptr [ebp + 8]
        or     ax, ax
        jne    SHORT loc_5_3C83
        sub    eax, eax
        test   byte ptr [ebp + 7], 2
        je     SHORT loc_5_3C6B
        mov    eax, 1
loc_5_3C6B:
        movzx  ecx, byte ptr [edx]
        push   ecx
        lea    ecx, [edx + 1]
        push   ecx
        push   eax
        push   dword ptr [esp + 1ch]
        push   ebx
        call   sub_5_3495
        add    esp, 14h
        jmp    SHORT loc_5_3C9F
loc_5_3C83:
        cmp    ax, 1
        mov    esi, 0ffh
        jne    SHORT loc_5_3CA1
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        push   ebx
        call   sub_5_9C14
        add    esp, 0ch
loc_5_3C9F:
        mov    esi, eax
loc_5_3CA1:
        or     esi, esi
        jne    SHORT loc_5_3CD2
        mov    ax, word ptr [ebp + 6]
        test   ah, 1
        je     SHORT loc_5_3CD2
        push   eax
        call   sub_5_3B80
        add    esp, 4
        push   eax
        cmp    word ptr [ebp + 8], 1
        sbb    eax, eax
        inc    eax
        push   eax
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 1ch]
        push   ebx
        db  0E8h, 0EBh, 049h, 0FBh, 0FFh
        add    esp, 14h
loc_5_3CD2:
        push   ebx
        db  0E8h, 0B8h, 006h, 0FBh, 0FFh
        mov    eax, dword ptr [esp + 28h]
        add    esp, 4
        mov    dword ptr [eax], 0
        mov    eax, esi
loc_5_3CE7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_3CEF:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 24h]
        mov    ebx, 0ffh
        cmp    byte ptr [eax + 6ah], 3
        je     SHORT loc_5_3D15
        test   byte ptr [eax + 1ah], 10h
        jne    SHORT loc_5_3D15
        mov    eax, 0c6h
        jmp    NEAR PTR loc_5_3DBB
loc_5_3D15:
        push   0
        push   100h
        db  0E8h, 043h, 006h, 0FBh, 0FFh
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_5_3D34
        mov    eax, 0ffh
        jmp    NEAR PTR loc_5_3DBB
loc_5_3D34:
        mov    edx, dword ptr [esp + 18h]
        mov    edi, ebp
        movzx  eax, byte ptr [edx + 8]
        lea    esi, [edx + 9]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        movzx  eax, byte ptr [edx + 8]
        mov    byte ptr [ebp + eax], 0
        mov    ax, word ptr [edx + 6]
        or     ax, ax
        jne    SHORT loc_5_3D96
        lea    eax, [esp + 10h]
        push   eax
        push   ebp
        call   sub_5_3371
        add    esp, 8
        or     eax, eax
        je     SHORT loc_5_3DA7
        mov    eax, dword ptr [esp + 10h]
        test   byte ptr [eax + 8], 1
        jne    SHORT loc_5_3D88
        push   eax
        call   sub_5_3824
        add    esp, 4
        mov    ebx, eax
loc_5_3D88:
        push   dword ptr [esp + 10h]
        call   sub_5_3340
        add    esp, 4
        jmp    SHORT loc_5_3DA7
loc_5_3D96:
        cmp    ax, 1
        jne    SHORT loc_5_3DA7
        push   ebp
        call   sub_5_9C85
        add    esp, 4
        mov    ebx, eax
loc_5_3DA7:
        push   ebp
        db  0E8h, 0A1h, 04Dh, 0FBh, 0FFh
        add    esp, 4
        push   ebp
        db  0E8h, 0DAh, 005h, 0FBh, 0FFh
        add    esp, 4
        mov    eax, ebx
loc_5_3DBB:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_5_3DC3:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 24h]
        mov    di, word ptr [eax + 0ah]
        movzx  esi, word ptr [eax + 8]
        cmp    word ptr [eax + 6], 0
        je     SHORT loc_5_3DE7
        mov    eax, 0ffh
        jmp    NEAR PTR loc_5_3FE7
loc_5_3DE7:
        push   0
        push   200h
        db  0E8h, 071h, 005h, 0FBh, 0FFh
        mov    dword ptr [esp + 1ch], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_5_3E08
        mov    eax, 0ffh
        jmp    NEAR PTR loc_5_3FE7
loc_5_3E08:
        mov    eax, dword ptr [esp + 14h]
        add    eax, 100h
        mov    ebx, dword ptr [esp + 28h]
        mov    dword ptr [esp + 18h], eax
        mov    word ptr [ebx + 2], 0
        or     di, di
        jne    NEAR PTR loc_5_3F05
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 00Ah, 056h, 0FBh, 0FFh
        add    esp, 4
        db  0A1h, 050h, 0A1h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_5_3EE7
loc_5_3E42:
        mov    ecx, esi
        dec    esi
        or     ecx, ecx
        je     SHORT loc_5_3E4F
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_5_3E42
loc_5_3E4F:
        or     eax, eax
        je     NEAR PTR loc_5_3EE7
        mov    ecx, dword ptr [eax + 8]
        and    ecx, 1
        cmp    ecx, 1
        cmc
        sbb    ecx, ecx
        and    ecx, 200h
        or     word ptr [ebx + 2], cx
        mov    cl, byte ptr [eax + 0ch]
        mov    byte ptr [ebx + 6], cl
        lea    edi, [ebx + 7]
        movzx  edx, byte ptr [eax + 0ch]
        lea    esi, [eax + 0dh]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        movzx  ecx, byte ptr [ebx + 6]
        lea    edx, [ebx + ecx + 7]
        mov    cl, byte ptr [eax + 24h]
        mov    byte ptr [edx], cl
        movzx  ebp, byte ptr [eax + 24h]
        lea    esi, [eax + 25h]
        lea    edi, [edx + 1]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        cmp    byte ptr [eax + 24h], 3
        jbe    SHORT loc_5_3EB8
        dec    byte ptr [edx]
loc_5_3EB8:
        movzx  ecx, byte ptr [edx]
        lea    ebp, [edx + ecx + 1]
        mov    cl, byte ptr [eax + 1dh]
        mov    byte ptr [ebp], cl
        movzx  edx, byte ptr [eax + 1dh]
        lea    edi, [ebp + 1]
        mov    esi, dword ptr [eax + 20h]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    dword ptr [esp + 10h], 0
        jmp    SHORT loc_5_3EF3
loc_5_3EE7:
        mov    dword ptr [esp + 10h], 0ffh
        mov    ebp, dword ptr [esp + 1ch]
loc_5_3EF3:
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 093h, 055h, 0FBh, 0FFh
        add    esp, 4
        jmp    NEAR PTR loc_5_3FC1
loc_5_3F05:
        cmp    di, 1
        jne    NEAR PTR loc_5_3FB5
        lea    edi, [ebx + 7]
        push   edi
        push   dword ptr [esp + 18h]
        push   esi
        call   sub_5_9E49
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_5_3FB5
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esp + 10h], 0
        mov    byte ptr [eax], 0
        push   eax
        push   edi
        call   sub_5_9D2A
        add    esp, 8
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    byte ptr [ebx + 6], cl
        movzx  eax, cl
        lea    edx, [ebx + eax + 7]
        mov    edi, dword ptr [esp + 14h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    byte ptr [edx], cl
        movzx  eax, cl
        lea    edi, [edx + 1]
        mov    esi, dword ptr [esp + 14h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        movzx  eax, byte ptr [edx]
        lea    ebp, [edx + eax + 1]
        mov    edi, dword ptr [esp + 18h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    byte ptr [ebp], cl
        movzx  eax, cl
        lea    edi, [ebp + 1]
        mov    esi, dword ptr [esp + 18h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        jmp    SHORT loc_5_3FC1
loc_5_3FB5:
        mov    dword ptr [esp + 10h], 0ffh
        mov    ebp, dword ptr [esp + 1ch]
loc_5_3FC1:
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_5_3FD7
        movzx  ecx, byte ptr [ebp]
        sub    ecx, ebx
        add    ecx, ebp
        mov    eax, dword ptr [esp + 2ch]
        inc    ecx
        mov    dword ptr [eax], ecx
loc_5_3FD7:
        push   dword ptr [esp + 14h]
        db  0E8h, 0B0h, 003h, 0FBh, 0FFh
        mov    eax, dword ptr [esp + 14h]
        add    esp, 4
loc_5_3FE7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_5_3FEF:
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 38h]
        mov    ebx, dword ptr [ecx + 8]
        movzx  eax, byte ptr [ecx + 0eh]
        lea    ebp, [ecx + eax + 0fh]
        movzx  eax, byte ptr [ebp]
        lea    eax, [ebp + eax + 1]
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [esp + 44h]
        cmp    byte ptr [eax + 6ah], 3
        je     SHORT loc_5_402B
        test   byte ptr [eax + 1ah], 10h
        jne    SHORT loc_5_402B
        mov    eax, 0c6h
        jmp    NEAR PTR loc_5_42DC
loc_5_402B:
        push   0
        push   400h
        db  0E8h, 02Dh, 003h, 0FBh, 0FFh
        mov    dword ptr [esp + 18h], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_5_404C
        mov    eax, 96h
        jmp    NEAR PTR loc_5_42DC
loc_5_404C:
        mov    edx, dword ptr [esp + 38h]
        mov    ecx, dword ptr [esp + 38h]
        add    edx, 0fh
        movzx  eax, byte ptr [ecx + 0eh]
        mov    edi, dword ptr [esp + 10h]
        mov    esi, edx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [esp + 38h]
        movzx  ecx, byte ptr [ecx + 0eh]
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [ecx + eax], 0
        add    eax, 100h
        mov    dword ptr [esp + 20h], eax
        add    eax, 100h
        mov    dword ptr [esp + 1ch], eax
        add    eax, 100h
        mov    dword ptr [esp + 24h], eax
        movzx  eax, byte ptr [ebp]
        mov    edi, dword ptr [esp + 20h]
        lea    esi, [ebp + 1]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        movzx  ecx, byte ptr [ebp]
        mov    eax, dword ptr [esp + 20h]
        mov    byte ptr [ecx + eax], 0
        test   ebx, 20000h
        je     SHORT loc_5_40F4
        mov    eax, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 18h]
        movzx  eax, byte ptr [eax]
        inc    esi
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 18h]
        movzx  ecx, byte ptr [eax]
        mov    eax, dword ptr [esp + 1ch]
        mov    byte ptr [ecx + eax], 0
loc_5_40F4:
        test   ebx, 10000h
        je     SHORT loc_5_4118
        movzx  eax, byte ptr [ebp]
        push   eax
        lea    ecx, [ebp + 1]
        push   ecx
        push   edx
        db  0E8h, 0FDh, 0D6h, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_5_4118
        xor    ebx, 10000h
loc_5_4118:
        mov    ecx, dword ptr [esp + 38h]
        mov    ax, word ptr [ecx + 6]
        or     ax, ax
        jne    NEAR PTR loc_5_4210
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 14h]
        call   sub_5_3371
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_5_425B
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 07Ch, 053h, 0FBh, 0FFh
        add    esp, 4
        test   ebx, 10000h
        je     SHORT loc_5_417D
        movzx  eax, byte ptr [ebp]
        mov    edi, dword ptr [esp + 14h]
        add    edi, 0dh
        lea    esi, [ebp + 1]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 14h]
        mov    cl, byte ptr [ebp]
        mov    byte ptr [eax + 0ch], cl
loc_5_417D:
        test   ebx, 20000h
        je     SHORT loc_5_41F3
        mov    eax, dword ptr [esp + 14h]
        cmp    byte ptr [eax + 1dh], 0
        je     SHORT loc_5_41A2
        push   dword ptr [eax + 20h]
        db  0E8h, 0F9h, 001h, 0FBh, 0FFh
        mov    eax, dword ptr [esp + 18h]
        add    esp, 4
        mov    byte ptr [eax + 1dh], 0
loc_5_41A2:
        mov    eax, dword ptr [esp + 18h]
        mov    al, byte ptr [eax]
        or     al, al
        je     SHORT loc_5_41F3
        push   0
        movzx  eax, al
        push   eax
        db  0E8h, 0ADh, 001h, 0FBh, 0FFh
        mov    ecx, dword ptr [esp + 1ch]
        add    esp, 8
        mov    dword ptr [ecx + 20h], eax
        mov    eax, dword ptr [esp + 14h]
        mov    edi, dword ptr [eax + 20h]
        or     edi, edi
        je     SHORT loc_5_41F3
        mov    eax, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 18h]
        movzx  eax, byte ptr [eax]
        inc    esi
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 18h]
        mov    cl, byte ptr [eax]
        mov    eax, dword ptr [esp + 14h]
        mov    byte ptr [eax + 1dh], cl
loc_5_41F3:
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 00Fh, 053h, 0FBh, 0FFh
        add    esp, 4
        sub    esi, esi
        push   dword ptr [esp + 14h]
        call   sub_5_3340
        add    esp, 4
        jmp    SHORT loc_5_425F
loc_5_4210:
        cmp    ax, 1
        jne    SHORT loc_5_4254
        sub    esi, esi
        test   ebx, 10000h
        je     SHORT loc_5_4232
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 14h]
        call   sub_5_9EE7
        add    esp, 8
        mov    esi, eax
loc_5_4232:
        or     esi, esi
        jne    NEAR PTR loc_5_42CE
        test   ebx, 20000h
        je     SHORT loc_5_425F
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 24h]
        call   sub_5_9DCA
        add    esp, 8
        jmp    SHORT loc_5_425F
loc_5_4254:
        mov    esi, 0ffh
        jmp    SHORT loc_5_425F
loc_5_425B:
        mov    esi, dword ptr [esp + 30h]
loc_5_425F:
        or     esi, esi
        jne    SHORT loc_5_42CE
        test   ebx, 20000h
        mov    ecx, 0
        jne    SHORT loc_5_4274
        mov    ecx, dword ptr [esp + 1ch]
loc_5_4274:
        lea    eax, [esp + 2ch]
        push   eax
        lea    eax, [esp + 2ch]
        push   eax
        push   ecx
        push   dword ptr [esp + 30h]
        push   dword ptr [esp + 20h]
        db  0E8h, 085h, 046h, 0FBh, 0FFh
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_5_42CE
        and    ebx, 10000h
        je     SHORT loc_5_42A1
        mov    eax, dword ptr [esp + 20h]
        jmp    SHORT loc_5_42A5
loc_5_42A1:
        mov    eax, dword ptr [esp + 10h]
loc_5_42A5:
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 30h]
        push   eax
        db  0E8h, 0FFh, 043h, 0FBh, 0FFh
        add    esp, 14h
        or     ebx, ebx
        je     SHORT loc_5_42CE
        push   dword ptr [esp + 10h]
        db  0E8h, 083h, 048h, 0FBh, 0FFh
        add    esp, 4
loc_5_42CE:
        push   dword ptr [esp + 10h]
        db  0E8h, 0B9h, 000h, 0FBh, 0FFh
        add    esp, 4
        mov    eax, esi
loc_5_42DC:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
sub_5_42E4:
        sub    al, al
        sub    ecx, ecx
        mov    dl, byte ptr [esp + 4]
        cmp    dl, al
        je     SHORT loc_5_4302
loc_5_42F0:
        test   dl, 1
        je     SHORT loc_5_42FB
        db  00Ah, 081h, 080h, 0B0h, 000h, 000h
loc_5_42FB:
        shr    dl, 1
        inc    ecx
        or     dl, dl
        jne    SHORT loc_5_42F0
loc_5_4302:
        ret
sub_5_4303:
        sub    ax, ax
        mov    cx, word ptr [esp + 4]
        cmp    cx, ax
        je     SHORT loc_5_4328
        db  0BAh, 06Ch, 0B0h, 000h, 000h
loc_5_4315:
        test   cl, 1
        je     SHORT loc_5_431D
        or     ax, word ptr [edx]
loc_5_431D:
        shr    cx, 1
        add    edx, 2
        or     cx, cx
        jne    SHORT loc_5_4315
loc_5_4328:
        ret
sub_5_4329:
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        call   sub_5_6192
        add    esp, 0ch
        and    ax, 1ffh
        push   eax
        call   sub_5_4303
        add    esp, 4
        ret
sub_5_434B:
        push   esi
        push   1
        push   0ah
        db  0E8h, 00Fh, 000h, 0FBh, 0FFh
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_5_4360
        jmp    SHORT loc_5_437B
loc_5_4360:
        mov    ecx, dword ptr [esp + 8]
        add    ecx, 40h
        mov    edx, esi
        db  0E8h, 09Dh, 0ECh, 0FAh, 0FFh
        or     al, al
        jne    SHORT loc_5_437D
        push   esi
        db  0E8h, 018h, 000h, 0FBh, 0FFh
        add    esp, 4
loc_5_437B:
        sub    al, al
loc_5_437D:
        pop    esi
        ret
sub_5_437F:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        movzx  esi, byte ptr [esp + 14h]
        mov    eax, dword ptr [edi + 40h]
        mov    ebx, dword ptr [eax + esi*4]
        cmp    ebx, 0ffh
        jbe    SHORT loc_5_43C1
        mov    eax, dword ptr [ebx + 4]
        or     eax, eax
        je     SHORT loc_5_43A9
        push   eax
        db  0E8h, 0EAh, 0FFh, 0FAh, 0FFh
        add    esp, 4
loc_5_43A9:
        mov    eax, dword ptr [ebx]
        or     eax, eax
        je     SHORT loc_5_43B8
        push   eax
        call   sub_5_3340
        add    esp, 4
loc_5_43B8:
        push   ebx
        db  0E8h, 0D2h, 0FFh, 0FAh, 0FFh
        add    esp, 4
loc_5_43C1:
        mov    eax, dword ptr [edi + 40h]
        pop    edi
        movzx  ecx, word ptr [eax + 2]
        mov    dword ptr [eax + esi*4], ecx
        mov    word ptr [eax + 2], si
        pop    esi
        pop    ebx
        ret
sub_5_43D3:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 1ch]
        add    esi, 40h
        push   50h
        push   10h
        mov    edx, 10h
        mov    ecx, esi
        db  0E8h, 09Fh, 0EBh, 0FAh, 0FFh
        or     eax, eax
        jne    SHORT loc_5_43FA
        jmp    NEAR PTR loc_5_4583
loc_5_43FA:
        push   dword ptr [esp + 1ch]
        call   sub_5_434B
        mov    byte ptr [esp + 17h], al
        add    esp, 4
        or     al, al
        jne    SHORT loc_5_4413
        jmp    NEAR PTR loc_5_4583
loc_5_4413:
        mov    ecx, dword ptr [esi]
        movzx  eax, byte ptr [esp + 13h]
        mov    ebx, dword ptr [ecx + eax*4]
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 015h, 050h, 0FBh, 0FFh
        add    esp, 4
        db  08Bh, 02Dh, 050h, 0A1h, 000h, 000h
        mov    dword ptr [ebx], ebp
        or     ebp, ebp
        jne    SHORT loc_5_445D
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 050h, 050h, 0FBh, 0FFh
        add    esp, 4
        push   dword ptr [esp + 13h]
        push   dword ptr [esp + 20h]
        call   sub_5_437F
        add    esp, 8
        mov    eax, 1
        jmp    NEAR PTR loc_5_4585
loc_5_445D:
        inc    dword ptr [ebp + 4]
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 026h, 050h, 0FBh, 0FFh
        add    esp, 4
        push   0
        db  00Fh, 0B6h, 005h, 070h, 0A1h, 000h, 000h
        add    eax, 2
        push   eax
        db  0E8h, 0E5h, 0FEh, 0FAh, 0FFh
        add    esp, 8
        mov    dword ptr [ebx + 4], eax
        or     eax, eax
        je     NEAR PTR loc_5_4573
        cmp    byte ptr [ebp + 0ch], 3
        jne    NEAR PTR loc_5_455F
        push   3
        db  068h, 0B4h, 0A1h, 000h, 000h
        lea    eax, [ebp + 0dh]
        push   eax
        db  0E8h, 061h, 0D3h, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_5_455F
        db  00Fh, 0B6h, 005h, 070h, 0A1h, 000h, 000h
        inc    eax
        db  0BEh, 070h, 0A1h, 000h, 000h
        mov    edi, dword ptr [ebx + 4]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        push   200h
        push   106h
        db  0E8h, 085h, 0FEh, 0FAh, 0FFh
        mov    dword ptr [esp + 1ch], eax
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_5_4573
        movzx  eax, byte ptr [ebp + 24h]
        add    ebp, 24h
        inc    eax
        mov    edi, dword ptr [esp + 14h]
        mov    esi, ebp
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        push   0ffh
        push   0ffh
        mov    al, byte ptr [ebp]
        push   eax
        push   dword ptr [ebx + 4]
        push   dword ptr [esp + 24h]
        db  0E8h, 06Ch, 0D7h, 0FAh, 0FFh
        mov    eax, dword ptr [esp + 28h]
        add    esp, 14h
        inc    eax
        push   eax
        db  0E8h, 01Dh, 000h, 0FBh, 0FFh
        add    esp, 4
        cmp    ax, 0ffffh
        je     SHORT loc_5_453F
        test   al, 10h
        jne    SHORT loc_5_454C
loc_5_453F:
        mov    eax, dword ptr [ebx + 4]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebx + 4]
        mov    byte ptr [eax + 1], 0
loc_5_454C:
        push   dword ptr [esp + 14h]
        db  0E8h, 03Bh, 0FEh, 0FAh, 0FFh
        add    esp, 4
        mov    eax, 1
        jmp    SHORT loc_5_4585
loc_5_455F:
        mov    eax, dword ptr [ebx + 4]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebx + 4]
        mov    byte ptr [eax + 1], 0
        mov    eax, 1
        jmp    SHORT loc_5_4585
loc_5_4573:
        push   dword ptr [esp + 13h]
        push   dword ptr [esp + 20h]
        call   sub_5_437F
        add    esp, 8
loc_5_4583:
        sub    eax, eax
loc_5_4585:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_5_458D:
        push   esi
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    edx, dword ptr [eax]
        mov    al, byte ptr [esp + 8]
        or     al, al
        jne    SHORT loc_5_45A3
        mov    ax, 0ceh
        jmp    SHORT loc_5_45DB
loc_5_45A3:
        movzx  esi, al
        mov    eax, dword ptr [edx + 40h]
        movzx  ecx, word ptr [eax - 4]
        cmp    ecx, esi
        jae    SHORT loc_5_45B3
        jmp    SHORT loc_5_45BD
loc_5_45B3:
        cmp    dword ptr [eax + esi*4], ecx
        mov    eax, 1
        ja     SHORT loc_5_45BF
loc_5_45BD:
        sub    eax, eax
loc_5_45BF:
        or     eax, eax
        jne    SHORT loc_5_45C9
        mov    ax, 0ceh
        jmp    SHORT loc_5_45DB
loc_5_45C9:
        mov    eax, dword ptr [edx + 40h]
        mov    eax, dword ptr [eax + esi*4]
        cmp    dword ptr [eax], 0
        mov    ax, 0ceh
        je     SHORT loc_5_45DB
        sub    ax, ax
loc_5_45DB:
        pop    esi
        ret
loc_5_45DD:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        mov    bl, byte ptr [eax + 0ah]
        push   ebx
        call   sub_5_458D
        add    esp, 4
        or     ax, ax
        jne    NEAR PTR loc_5_4692
        mov    eax, dword ptr [esp + 20h]
        movzx  ecx, bl
        mov    eax, dword ptr [eax + 40h]
        db  068h, 054h, 0A1h, 000h, 000h
        mov    ebp, dword ptr [eax + ecx*4]
        db  0E8h, 02Bh, 04Eh, 0FBh, 0FFh
        add    esp, 4
        mov    esi, dword ptr [ebp]
        mov    edi, dword ptr [esp + 18h]
        movzx  ebx, byte ptr [esi + 0ch]
        add    esi, 0dh
        add    edi, 9
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 053h, 04Eh, 0FBh, 0FFh
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 4
        mov    byte ptr [ebx + eax + 9], 3ah
        inc    ebx
        mov    edx, eax
        mov    eax, dword ptr [ebp + 4]
        add    edx, 8
        mov    al, byte ptr [eax]
        add    al, bl
        mov    byte ptr [edx], al
        movzx  cx, al
        mov    eax, dword ptr [esp + 1ch]
        inc    cx
        mov    word ptr [eax], cx
        mov    esi, dword ptr [ebp + 4]
        mov    eax, dword ptr [esp + 18h]
        movzx  ebp, byte ptr [esi]
        inc    esi
        lea    edi, [ebx + eax + 9]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        push   2fh
        push   5ch
        push   edx
        db  0E8h, 03Ch, 0D1h, 0FAh, 0FFh
        add    esp, 0ch
        sub    ax, ax
loc_5_4692:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_4697:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        push   ebx
        db  0E8h, 058h, 0D2h, 0FAh, 0FFh
        add    esp, 4
        mov    edi, eax
        cmp    edi, 2
        jb     SHORT loc_5_4725
        cmp    edi, 10h
        ja     SHORT loc_5_4725
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 07Fh, 04Dh, 0FBh, 0FFh
        add    esp, 4
        db  08Bh, 035h, 050h, 0A1h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_4714
loc_5_46CA:
        movzx  eax, byte ptr [esi + 0ch]
        cmp    eax, edi
        jne    SHORT loc_5_46E7
        push   edi
        lea    eax, [esi + 0dh]
        push   eax
        lea    eax, [ebx + 1]
        push   eax
        db  0E8h, 028h, 0D1h, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_46EF
loc_5_46E7:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_5_46CA
        jmp    SHORT loc_5_4714
loc_5_46EF:
        movzx  ecx, byte ptr [ebx]
        cmp    edi, ecx
        jae    SHORT loc_5_470B
loc_5_46F6:
        mov    al, byte ptr [ebx + edi + 1]
        cmp    al, 3ah
        je     SHORT loc_5_4706
        cmp    al, 5ch
        je     SHORT loc_5_4706
        cmp    al, 2fh
        jne    SHORT loc_5_470B
loc_5_4706:
        inc    edi
        cmp    edi, ecx
        jb     SHORT loc_5_46F6
loc_5_470B:
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax], edi
        inc    dword ptr [esi + 4]
loc_5_4714:
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 072h, 04Dh, 0FBh, 0FFh
        add    esp, 4
        mov    eax, esi
        jmp    SHORT loc_5_4727
loc_5_4725:
        sub    eax, eax
loc_5_4727:
        pop    edi
        pop    esi
        pop    ebx
        ret
_PTEXT ends
        end
