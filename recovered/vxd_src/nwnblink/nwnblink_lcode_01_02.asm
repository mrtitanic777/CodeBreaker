; nwnblink_lcode_01_02.asm -- NWNBLINK object 1 segment _LTEXT  (part 2/2)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_4D40:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 1ch]
        mov    ax, word ptr [esi + 1eh]
        cmp    word ptr [ecx + 0ah], ax
        jne    NEAR PTR loc_1_4EB3
        mov    ax, word ptr [ecx + 0ch]
        mov    edi, dword ptr [esp + 20h]
        mov    word ptr [esp + 12h], ax
        lea    ebp, [edi + 24h]
loc_1_4D70:
        add    ebp, 6
        mov    ax, word ptr [edi + 22h]
        inc    ax
        mov    word ptr [edi + 22h], ax
        cmp    ax, 4
        ja     NEAR PTR loc_1_4E68
        cmp    word ptr [esi + 78h], 0
        je     SHORT loc_1_4D96
        mov    ebx, dword ptr [esi + 50h]
        sub    ebx, 34h
        jmp    SHORT loc_1_4DCD
loc_1_4D96:
        push   esi
        call   sub_1_48C0
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_1_4EA5
        lea    edx, [ebx + 34h]
        push   edx
        call   sub_1_3B50
        add    esp, 4
        lea    eax, [esi + 4ch]
        mov    dword ptr [ebx + 34h], eax
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebx + 38h], ecx
        lea    edx, [ebx + 34h]
        mov    dword ptr [ecx], edx
        mov    dword ptr [eax + 4], edx
        mov    byte ptr [ebx + 32h], 0dh
loc_1_4DCD:
        xor    edx, edx
        mov    dx, word ptr [esi + 78h]
        xor    eax, eax
        mov    ax, word ptr [ebx + 8]
        xor    ecx, ecx
        mov    cx, word ptr [esp + 12h]
        sub    eax, edx
        cmp    eax, ecx
        jge    SHORT loc_1_4DF0
        mov    cx, word ptr [ebx + 8]
        sub    cx, word ptr [esi + 78h]
        jmp    SHORT loc_1_4DF5
loc_1_4DF0:
        mov    cx, word ptr [esp + 12h]
loc_1_4DF5:
        mov    eax, dword ptr [ebx + 4]
        add    eax, edx
        mov    dword ptr [ebp], eax
        mov    word ptr [ebp + 4], cx
        sub    word ptr [esp + 12h], cx
        mov    ax, word ptr [esi + 78h]
        add    ax, cx
        mov    word ptr [esi + 78h], ax
        mov    ax, word ptr [esi + 1eh]
        add    ax, cx
        mov    ecx, dword ptr [esp + 1ch]
        mov    word ptr [esi + 1eh], ax
        cmp    word ptr [ecx + 8], ax
        je     SHORT loc_1_4E70
        mov    ax, word ptr [ebx + 8]
        cmp    word ptr [esi + 78h], ax
        jne    SHORT loc_1_4E53
        mov    word ptr [esi + 78h], 0
        mov    byte ptr [ebx + 1], 86h
        mov    al, byte ptr [esi + 2dh]
        or     al, al
        je     SHORT loc_1_4E4E
        mov    ecx, dword ptr [esi + 54h]
        sub    ecx, esi
        cmp    ecx, 54h
        je     SHORT loc_1_4E4E
        mov    byte ptr [esi + 2fh], al
loc_1_4E4E:
        mov    dword ptr [ebx + 3ch], edi
        jmp    SHORT loc_1_4E5A
loc_1_4E53:
        mov    dword ptr [ebx + 3ch], 0
loc_1_4E5A:
        cmp    word ptr [esp + 12h], 0
        jne    NEAR PTR loc_1_4D70
        jmp    SHORT loc_1_4EA9
loc_1_4E68:
        dec    word ptr [edi + 22h]
        sub    ebx, ebx
        jmp    SHORT loc_1_4EA9
loc_1_4E70:
        mov    ax, word ptr [esi + 78h]
        inc    word ptr [esi + 1ch]
        mov    word ptr [ebx + 8], ax
        mov    word ptr [esi + 78h], 0
        mov    word ptr [esi + 1eh], 0
        mov    byte ptr [ebx + 1], 80h
        mov    al, byte ptr [esi + 2dh]
        or     al, al
        je     SHORT loc_1_4EA0
        mov    ecx, dword ptr [esi + 54h]
        sub    ecx, esi
        cmp    ecx, 54h
        je     SHORT loc_1_4EA0
        mov    byte ptr [esi + 2fh], al
loc_1_4EA0:
        mov    dword ptr [ebx + 3ch], edi
        jmp    SHORT loc_1_4EA9
loc_1_4EA5:
        dec    word ptr [edi + 22h]
loc_1_4EA9:
        or     ebx, ebx
        jne    SHORT loc_1_4EB3
        or     word ptr [esi + 4], 400h
loc_1_4EB3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_1_4EC0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 20h]
        mov    esi, dword ptr [esp + 1ch]
        mov    ax, word ptr [eax + 0ch]
        sub    ax, word ptr [esi + 1eh]
        mov    edi, dword ptr [esp + 24h]
        mov    word ptr [esp + 12h], ax
        lea    ebp, [edi + 24h]
loc_1_4EE3:
        add    ebp, 6
        mov    ax, word ptr [edi + 22h]
        inc    ax
        mov    word ptr [edi + 22h], ax
        cmp    ax, 4
        ja     NEAR PTR loc_1_5015
        cmp    word ptr [esi + 78h], 0
        je     SHORT loc_1_4F09
        mov    ebx, dword ptr [esi + 50h]
        sub    ebx, 34h
        jmp    SHORT loc_1_4F40
loc_1_4F09:
        push   esi
        call   sub_1_48C0
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_1_5020
        lea    ecx, [ebx + 34h]
        push   ecx
        call   sub_1_3B50
        add    esp, 4
        lea    edx, [esi + 4ch]
        mov    dword ptr [ebx + 34h], edx
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ebx + 38h], eax
        lea    ecx, [ebx + 34h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx + 4], ecx
        mov    byte ptr [ebx + 32h], 0dh
loc_1_4F40:
        xor    eax, eax
        mov    ax, word ptr [esi + 78h]
        mov    cx, word ptr [ebx + 8]
        mov    dword ptr [esp + 14h], eax
        mov    edx, ecx
        and    edx, 0ffffh
        sub    edx, eax
        xor    eax, eax
        mov    ax, word ptr [esp + 12h]
        cmp    edx, eax
        jge    SHORT loc_1_4F70
        mov    ax, cx
        sub    ax, word ptr [esi + 78h]
        jmp    SHORT loc_1_4F75
        db  08Dh, 064h, 024h, 000h
loc_1_4F70:
        mov    ax, word ptr [esp + 12h]
loc_1_4F75:
        mov    edx, dword ptr [ebx + 4]
        add    edx, dword ptr [esp + 14h]
        mov    dword ptr [ebp], edx
        mov    word ptr [ebp + 4], ax
        add    word ptr [esi + 1eh], ax
        mov    dx, word ptr [esi + 78h]
        add    dx, ax
        sub    word ptr [esp + 12h], ax
        mov    word ptr [esi + 78h], dx
        jne    SHORT loc_1_4FD2
        mov    eax, dword ptr [esp + 20h]
        test   byte ptr [eax], 10h
        je     SHORT loc_1_4FD2
        mov    ax, dx
        mov    word ptr [ebx + 8], ax
        mov    word ptr [esi + 78h], 0
        mov    word ptr [esi + 1eh], 0
        mov    al, byte ptr [esi + 2dh]
        or     al, al
        je     SHORT loc_1_4FC9
        mov    ecx, dword ptr [esi + 54h]
        sub    ecx, esi
        cmp    ecx, 54h
        je     SHORT loc_1_4FC9
        mov    byte ptr [esi + 2fh], al
loc_1_4FC9:
        mov    byte ptr [ebx + 1], 80h
        mov    dword ptr [ebx + 3ch], edi
        jmp    SHORT loc_1_5007
loc_1_4FD2:
        cmp    word ptr [esi + 78h], cx
        jne    SHORT loc_1_5000
        mov    word ptr [esi + 78h], 0
        mov    byte ptr [ebx + 1], 86h
        mov    al, byte ptr [esi + 2dh]
        or     al, al
        je     SHORT loc_1_4FF6
        mov    ecx, dword ptr [esi + 54h]
        sub    ecx, esi
        cmp    ecx, 54h
        je     SHORT loc_1_4FF6
        mov    byte ptr [esi + 2fh], al
loc_1_4FF6:
        mov    dword ptr [ebx + 3ch], edi
        jmp    SHORT loc_1_5007
        db  005h, 000h, 000h, 000h, 000h
loc_1_5000:
        mov    dword ptr [ebx + 3ch], 0
loc_1_5007:
        cmp    word ptr [esp + 12h], 0
        jne    NEAR PTR loc_1_4EE3
        jmp    SHORT loc_1_5024
loc_1_5015:
        dec    word ptr [edi + 22h]
        sub    ebx, ebx
        jmp    SHORT loc_1_5024
        db  08Dh, 049h, 000h
loc_1_5020:
        dec    word ptr [edi + 22h]
loc_1_5024:
        or     ebx, ebx
        jne    SHORT loc_1_5036
        or     word ptr [esi + 4], 480h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_5036:
        inc    word ptr [esi + 1ch]
        mov    eax, dword ptr [esp + 20h]
        mov    word ptr [esi + 1eh], 0
        mov    cl, byte ptr [eax]
        test   cl, 10h
        jne    SHORT loc_1_5064
        mov    ax, word ptr [esi + 2ah]
        inc    ax
        cmp    word ptr [esi + 34h], ax
        mov    word ptr [esi + 2ah], ax
        ja     SHORT loc_1_5064
        mov    eax, dword ptr [esp + 20h]
        or     cl, 40h
        mov    byte ptr [eax], cl
loc_1_5064:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_5070:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        xor    ecx, ecx
        db  0A1h, 07Ch, 079h, 000h, 000h
        mov    cx, word ptr [esi + 3eh]
        mov    edi, dword ptr [eax + ecx*4]
        or     edi, edi
        je     SHORT loc_1_50E2
        mov    ecx, dword ptr [edi + 4ch]
        mov    eax, edi
        sub    eax, ecx
        cmp    eax, -4ch
        je     SHORT loc_1_50DC
loc_1_5096:
        mov    ebx, dword ptr [ecx]
        cmp    dword ptr [ecx + 8], esi
        lea    ebp, [ecx - 34h]
        jne    SHORT loc_1_50D1
        push   ecx
        call   sub_1_3B50
        add    esp, 4
        mov    byte ptr [ebp + 32h], 0
        and    byte ptr [ebp + 1], 7fh
        test   byte ptr [ebp], 80h
        je     SHORT loc_1_50D1
        db  0C7h, 045h, 034h, 090h, 078h, 000h, 000h
        add    ebp, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [ebp + 4], eax
        mov    dword ptr [eax], ebp
        db  089h, 02Dh, 094h, 078h, 000h, 000h
loc_1_50D1:
        mov    ecx, ebx
        mov    eax, edi
        sub    eax, ebx
        cmp    eax, -4ch
        jne    SHORT loc_1_5096
loc_1_50DC:
        mov    word ptr [esi + 22h], 1
loc_1_50E2:
        push   esi
        call   sub_1_3900
        add    esp, 4
        call   sub_1_2E80
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5100:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        sub    bl, bl
        lea    esi, [ebp + 8ch]
        push   esi
        call   sub_1_3240
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_5125
        mov    bl, byte ptr [eax + 14h]
        and    bl, 0c7h
        jmp    SHORT loc_1_5138
loc_1_5125:
        cmp    byte ptr [esi], 2ah
        jne    SHORT loc_1_5138
        db  081h, 03Dh, 018h, 079h, 000h, 000h, 018h, 079h, 000h, 000h
        je     SHORT loc_1_5138
        mov    bl, 0c4h
loc_1_5138:
        or     bl, bl
        je     SHORT loc_1_51B9
        lea    edi, [ebp + 4ch]
        call   sub_1_37F0
        or     eax, eax
        je     SHORT loc_1_51B9
        db  0C7h, 040h, 004h, 020h, 03Bh, 000h, 000h
        lea    edx, [eax + 1ch]
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [edx], ecx
        mov    cx, word ptr [ebp + 20h]
        mov    word ptr [edx + 4], cx
        add    ebp, 1ch
        mov    word ptr [eax + 28h], 50h
        mov    byte ptr [eax + 51h], 4
        lea    edx, [eax + 4ch]
        add    edi, 12h
        lea    ebp, [edx + 6]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [ebp], ecx
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [ebp + 4], ecx
        add    edx, 1eh
        mov    cx, word ptr [edi + 8]
        mov    word ptr [ebp + 8], cx
        mov    byte ptr [edx + 21h], 2
        mov    byte ptr [edx + 1], 2
        mov    byte ptr [edx + 20h], bl
        mov    ecx, dword ptr [esi]
        mov    dword ptr [edx + 22h], ecx
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [edx + 26h], ecx
        add    edx, 22h
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [edx + 8], ecx
        mov    ecx, dword ptr [esi + 0ch]
        mov    dword ptr [edx + 0ch], ecx
        push   eax
        call   sub_1_0292
        add    esp, 4
loc_1_51B9:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_51C0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 18h]
        lea    eax, [ebp + 8ch]
        add    ebp, 4ch
        db  08Bh, 015h, 0ECh, 078h, 000h, 000h
        db  081h, 0FAh, 0ECh, 078h, 000h, 000h
        je     SHORT loc_1_5200
loc_1_51E2:
        lea    ebx, [edx - 20h]
        mov    ecx, 4
        mov    esi, eax
        mov    edi, ebx
        repe cmpsd
        je     SHORT loc_1_5204
        mov    edx, dword ptr [edx]
        db  081h, 0FAh, 0ECh, 078h, 000h, 000h
        jne    SHORT loc_1_51E2
        jmp    SHORT loc_1_5204
        db  08Bh, 0FFh
loc_1_5200:
        mov    ebx, dword ptr [esp + 10h]
loc_1_5204:
        db  081h, 0FAh, 0ECh, 078h, 000h, 000h
        je     NEAR PTR loc_1_52C2
        mov    cl, byte ptr [ebp + 3eh]
        mov    al, cl
        and    al, 0c7h
        cmp    al, 44h
        jne    SHORT loc_1_5297
        mov    cl, byte ptr [ebx + 28h]
        test   cl, 80h
        jne    SHORT loc_1_5274
        or     cl, al
        mov    byte ptr [ebx + 28h], cl
        lea    eax, [ebx + 20h]
        push   eax
        call   sub_1_3B50
        add    esp, 4
        lea    ecx, [ebx + 10h]
        mov    eax, dword ptr [ebp + 12h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 16h]
        mov    dword ptr [ecx + 4], eax
        mov    ax, word ptr [ebp + 1ah]
        add    ebp, 12h
        mov    word ptr [ecx + 8], ax
        mov    eax, dword ptr [esp + 18h]
        add    eax, 1ch
        lea    edx, [ebx + 1ah]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edx + 4], ax
        push   ebx
        call   sub_1_29B0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_5274:
        or     cl, 2
        mov    byte ptr [ebx + 28h], cl
        lea    eax, [ebx + 20h]
        push   eax
        call   sub_1_3B50
        add    esp, 4
        push   ebx
        call   sub_1_2AC0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_5297:
        and    cl, 84h
        cmp    cl, 84h
        jne    SHORT loc_1_52C2
        or     byte ptr [ebx + 28h], al
        test   byte ptr [ebp + 4], 0fh
        db  0B8h, 0ACh, 078h, 000h, 000h
        je     SHORT loc_1_52B4
        mov    eax, dword ptr [esp + 18h]
        add    eax, 1ch
loc_1_52B4:
        push   eax
        mov    eax, dword ptr [ebp + 12h]
        push   eax
        push   ebx
        call   sub_1_2C80
        add    esp, 0ch
loc_1_52C2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_52D0:
        push   esi
        mov    eax, dword ptr [esp + 8]
        add    eax, 8ch
        push   eax
        call   sub_1_3240
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_5396
        mov    cl, byte ptr [eax + 14h]
        test   cl, 4
        je     SHORT loc_1_5313
        or     cl, 2
        mov    byte ptr [eax + 14h], cl
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 15h]
        db  08Bh, 015h, 050h, 078h, 000h, 000h
        mov    byte ptr [ecx + edx], 0ffh
        push   eax
        call   sub_1_3760
        add    esp, 4
        pop    esi
        ret
loc_1_5313:
        mov    ecx, dword ptr [eax + 3ch]
        or     ecx, ecx
        je     SHORT loc_1_534D
        cmp    byte ptr [ecx + 1], 0ffh
        jne    SHORT loc_1_534D
        mov    byte ptr [ecx + 1], 16h
        mov    edx, dword ptr [eax + 3ch]
        test   byte ptr [edx], 80h
        je     SHORT loc_1_534D
        db  0C7h, 042h, 034h, 090h, 078h, 000h, 000h
        add    edx, 34h
        db  08Bh, 00Dh, 094h, 078h, 000h, 000h
        mov    dword ptr [edx + 4], ecx
        db  08Bh, 00Dh, 094h, 078h, 000h, 000h
        mov    dword ptr [ecx], edx
        db  089h, 015h, 094h, 078h, 000h, 000h
loc_1_534D:
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 15h]
        db  08Bh, 015h, 050h, 078h, 000h, 000h
        mov    byte ptr [ecx + edx], 0ffh
        db  08Bh, 00Dh, 080h, 078h, 000h, 000h
        mov    esi, ecx
        or     ecx, ecx
        je     SHORT loc_1_5375
loc_1_5368:
        mov    edx, dword ptr [esi + 10h]
        cmp    edx, eax
        je     SHORT loc_1_5375
        mov    esi, edx
        or     esi, esi
        jne    SHORT loc_1_5368
loc_1_5375:
        cmp    ecx, eax
        jne    SHORT loc_1_5388
        mov    ecx, dword ptr [ecx + 10h]
        db  089h, 00Dh, 080h, 078h, 000h, 000h
        mov    byte ptr [eax + 14h], 0
        pop    esi
        ret
loc_1_5388:
        or     esi, esi
        je     SHORT loc_1_5392
        mov    ecx, dword ptr [eax + 10h]
        mov    dword ptr [esi + 10h], ecx
loc_1_5392:
        mov    byte ptr [eax + 14h], 0
loc_1_5396:
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_53A0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        db  0A1h, 024h, 079h, 000h, 000h
        lea    edi, [esi + 6ah]
        cmp    dword ptr [esi + 62h], eax
        lea    ebx, [esi + 4ch]
        jne    SHORT loc_1_53D1
        db  066h, 0A1h, 028h, 079h, 000h, 000h
        cmp    word ptr [ebx + 1ah], ax
        jne    SHORT loc_1_53D1
        test   byte ptr [ebx + 4], 0fh
        je     NEAR PTR loc_1_54B9
loc_1_53D1:
        lea    eax, [edi + 22h]
        mov    dword ptr [esp + 10h], eax
        push   eax
        call   sub_1_3240
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     NEAR PTR loc_1_5480
        mov    al, byte ptr [edi + 20h]
        and    al, byte ptr [ebp + 14h]
        test   al, 80h
        jne    NEAR PTR loc_1_5480
        push   esi
        call   sub_1_37F0
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_54B9
        lea    edx, [eax + 1ch]
        mov    ecx, dword ptr [esi + 1ch]
        mov    dword ptr [edx], ecx
        mov    cx, word ptr [esi + 20h]
        mov    word ptr [edx + 4], cx
        add    esi, 1ch
        mov    word ptr [eax + 28h], 50h
        mov    byte ptr [eax + 51h], 4
        lea    edx, [eax + 4ch]
        add    ebx, 12h
        lea    esi, [edx + 6]
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [esi], ecx
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [esi + 4], ecx
        add    edx, 1eh
        mov    cx, word ptr [ebx + 8]
        mov    word ptr [esi + 8], cx
        mov    byte ptr [edx + 21h], 4
        mov    byte ptr [edx + 1], 4
        add    edx, 22h
        mov    cl, byte ptr [ebp + 14h]
        mov    byte ptr [edx - 2], cl
        mov    ecx, dword ptr [ebp]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [ebp + 4]
        mov    dword ptr [edx + 4], ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 8], ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 0ch], ecx
        push   eax
        call   sub_1_0292
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_5480:
        test   byte ptr [edi + 20h], 80h
        je     SHORT loc_1_54B9
        test   byte ptr [ebx + 4], 0fh
        je     SHORT loc_1_54B9
        mov    eax, dword ptr [esp + 10h]
        push   eax
        db  068h, 0ECh, 078h, 000h, 000h
        call   sub_1_2780
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_54B9
        test   byte ptr [eax + 28h], 80h
        je     SHORT loc_1_54B9
        add    esi, 1ch
        push   esi
        mov    ecx, dword ptr [ebx + 12h]
        push   ecx
        push   eax
        call   sub_1_2C80
        add    esp, 0ch
loc_1_54B9:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_54D0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        add    eax, 8ch
        db  08Bh, 02Dh, 0C0h, 078h, 000h, 000h
        db  081h, 0FDh, 0C0h, 078h, 000h, 000h
        je     SHORT loc_1_5510
        mov    edi, edi
loc_1_54F0:
        lea    ebx, [ebp - 20h]
        mov    ecx, 4
        mov    esi, eax
        mov    edi, ebx
        repe cmpsd
        je     SHORT loc_1_5514
        mov    ebp, dword ptr [ebp]
        db  081h, 0FDh, 0C0h, 078h, 000h, 000h
        jne    SHORT loc_1_54F0
        jmp    SHORT loc_1_5514
        db  08Dh, 049h, 000h
loc_1_5510:
        mov    ebx, dword ptr [esp + 10h]
loc_1_5514:
        db  081h, 0FDh, 0C0h, 078h, 000h, 000h
        je     SHORT loc_1_5559
        test   byte ptr [ebx + 28h], 80h
        jne    SHORT loc_1_5559
        push   ebp
        call   sub_1_3B50
        add    esp, 4
        cmp    word ptr [ebx + 2ah], 0
        je     SHORT loc_1_5550
        mov    dword ptr [ebx + 2ch], 0
        push   ebp
        db  068h, 0C0h, 078h, 000h, 000h
        call   sub_1_3B30
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_5550:
        push   ebx
        call   sub_1_2BC0
        add    esp, 4
loc_1_5559:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5570:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 14h]
        db  08Bh, 035h, 07Ch, 079h, 000h, 000h
        lea    ebp, [edx + 6ah]
        lea    eax, [edx + 4ch]
        mov    cx, word ptr [ebp + 4]
        mov    edi, ecx
        and    edi, 0ffh
        mov    ebx, dword ptr [esi + edi*4]
        or     ebx, ebx
        je     NEAR PTR loc_1_5630
        cmp    word ptr [ebx + 10h], cx
        jne    NEAR PTR loc_1_5630
        lea    edi, [ebx + 3ch]
        lea    esi, [edx + 5eh]
        mov    ecx, 2
        repe cmpsd
        jne    NEAR PTR loc_1_564B
        mov    cx, word ptr [esi]
        cmp    cx, word ptr [edi]
        jne    NEAR PTR loc_1_564B
        mov    cl, byte ptr [ebx + 6]
        cmp    cl, 1
        jne    SHORT loc_1_55E1
        test   byte ptr [ebp], 80h
        je     SHORT loc_1_55E1
        push   eax
        push   ebx
        call   sub_1_14C0
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_55E1:
        mov    ax, word ptr [ebx + 12h]
        cmp    word ptr [ebp + 2], ax
        jne    SHORT loc_1_564B
        cmp    cl, 3
        jne    SHORT loc_1_564B
        test   byte ptr [ebp], 80h
        je     SHORT loc_1_561D
        test   byte ptr [ebx + 5], 80h
        push   ebp
        push   ebx
        je     SHORT loc_1_5610
        call   sub_1_1650
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_5610:
        call   sub_1_1990
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_561D:
        push   ebp
        push   edx
        push   ebx
        call   sub_1_1C50
        add    esp, 0ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_5630:
        cmp    cx, 0ffffh
        jne    SHORT loc_1_564B
        cmp    word ptr [ebp + 6], 0
        jne    SHORT loc_1_564B
        add    edx, 1ch
        push   edx
        push   eax
        call   sub_1_11A0
        add    esp, 8
loc_1_564B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_5650:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        add    ecx, 6ah
        xor    edx, edx
        db  0A1h, 07Ch, 079h, 000h, 000h
        mov    dl, byte ptr [ecx + 4]
        mov    esi, dword ptr [eax + edx*4]
        or     esi, esi
        je     SHORT loc_1_56A5
        mov    byte ptr [esi + 6], 6
        test   byte ptr [esi + 5], 80h
        je     SHORT loc_1_567D
        push   ecx
        push   esi
        call   sub_1_1650
        add    esp, 8
loc_1_567D:
        mov    eax, dword ptr [esi + 68h]
        test   byte ptr [eax + 3ch], 8
        je     SHORT loc_1_56A0
        push   eax
        call   sub_1_3C30
        add    esp, 4
        push   1
        mov    eax, dword ptr [esi + 68h]
        push   eax
        push   esi
        call   sub_1_0B20
        add    esp, 0ch
        pop    esi
        ret
loc_1_56A0:
        or     word ptr [esi + 4], 8
loc_1_56A5:
        pop    esi
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_56B0:
        push   esi
        mov    eax, dword ptr [esp + 8]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 6eh]
        db  0A1h, 07Ch, 079h, 000h, 000h
        mov    esi, dword ptr [eax + ecx*4]
        or     esi, esi
        je     SHORT loc_1_56FE
        mov    eax, dword ptr [esi + 68h]
        test   byte ptr [eax + 3ch], 8
        je     SHORT loc_1_56E6
        push   eax
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [esi + 68h]
        push   eax
        call   sub_1_3820
        add    esp, 4
        jmp    SHORT loc_1_56EC
loc_1_56E6:
        mov    word ptr [eax + 3eh], 0ffh
loc_1_56EC:
        mov    dword ptr [esi + 68h], 0
        push   0ah
        push   esi
        call   sub_1_07A0
        add    esp, 8
loc_1_56FE:
        pop    esi
        ret
loc_1_5700:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   0
        mov    eax, dword ptr [esi + 2ah]
        push   eax
        call   sub_1_01FA
        add    esp, 8
        push   esi
        call   sub_1_3820
        add    esp, 4
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_5720:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        call   sub_1_37F0
        lea    edi, [ebx + 4ch]
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_57CD
        db  0C7h, 046h, 004h, 000h, 057h, 000h, 000h
        lea    ecx, [esi + 1ch]
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [ecx], eax
        mov    ax, word ptr [ebx + 20h]
        mov    word ptr [ecx + 4], ax
        add    ebx, 1ch
        mov    word ptr [esi + 28h], 20h
        mov    byte ptr [esi + 51h], 4
        lea    ecx, [esi + 4ch]
        add    edi, 12h
        lea    edx, [ecx + 6]
        mov    eax, dword ptr [edi]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [edx + 4], eax
        mov    ax, word ptr [edi + 8]
        mov    word ptr [edx + 8], ax
        mov    byte ptr [ecx + 1fh], 0ah
        db  0A1h, 084h, 079h, 000h, 000h
        imul   ax, ax, 12h
        add    ax, 3ch
        push   0
        mov    edi, eax
        and    edi, 0ffffh
        push   edi
        call   sub_1_01DD
        add    esp, 8
        mov    dword ptr [esi + 2ah], eax
        or     eax, eax
        je     SHORT loc_1_57C4
        push   edi
        push   eax
        call   sub_1_6230
        add    esp, 8
        mov    word ptr [esi + 2eh], ax
        mov    word ptr [esi + 22h], 2
        push   esi
        call   sub_1_0292
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_57C4:
        push   esi
        call   sub_1_3820
        add    esp, 4
loc_1_57CD:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_57E0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 02Dh, 080h, 079h, 000h, 000h
        or     ebp, ebp
        je     NEAR PTR loc_1_58B8
        mov    edx, dword ptr [ebp + 3ch]
        lea    ebx, [ebp + 3ch]
        or     edx, edx
        je     NEAR PTR loc_1_58B8
        mov    eax, dword ptr [esp + 18h]
        lea    esi, [edx + 52h]
        lea    eax, [eax + 4ch]
        mov    edi, eax
        mov    dword ptr [esp + 10h], eax
        lea    edi, [edi + 12h]
        mov    ecx, 2
        repe cmpsd
        jne    NEAR PTR loc_1_58B8
        mov    ax, word ptr [esi]
        cmp    ax, word ptr [edi]
        jne    NEAR PTR loc_1_58B8
        db  0C7h, 005h, 080h, 079h, 000h, 000h, 000h, 000h, 000h, 000h
        test   byte ptr [edx + 3ch], 8
        je     SHORT loc_1_5853
        push   edx
        call   sub_1_3C30
        add    esp, 4
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_3820
        add    esp, 4
loc_1_5853:
        mov    eax, dword ptr [esp + 10h]
        mov    ax, word ptr [eax + 2]
        push   eax
        call   sub_1_0187
        add    esp, 4
        sub    ax, 20h
        mov    cx, word ptr [ebp + 8]
        cmp    ax, cx
        jbe    SHORT loc_1_5878
        mov    ax, cx
        mov    bl, 6
        jmp    SHORT loc_1_587E
loc_1_5878:
        mov    word ptr [ebp + 8], ax
        sub    bl, bl
loc_1_587E:
        push   eax
        mov    eax, dword ptr [ebp + 4]
        push   eax
        mov    eax, dword ptr [esp + 20h]
        push   20h
        add    eax, 24h
        push   eax
        call   sub_1_3B60
        add    esp, 10h
        mov    byte ptr [ebp + 1], bl
        test   byte ptr [ebp], 80h
        je     SHORT loc_1_58B8
        db  0C7h, 045h, 034h, 090h, 078h, 000h, 000h
        add    ebp, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [ebp + 4], eax
        mov    dword ptr [eax], ebp
        db  089h, 02Dh, 094h, 078h, 000h, 000h
loc_1_58B8:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_58C0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 18h]
        lea    ebx, [edi + 6ah]
        mov    ax, word ptr [edi + 4eh]
        push   eax
        call   sub_1_0187
        add    esp, 4
        lea    ebp, [eax - 40h]
        lea    eax, [ebx + 12h]
        push   eax
        call   sub_1_3240
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_59C4
        test   byte ptr [eax + 14h], 4
        je     NEAR PTR loc_1_59C4
        mov    edx, dword ptr [eax + 20h]
        mov    ecx, edx
        sub    ecx, eax
        cmp    ecx, 20h
        mov    esi, 0
        je     SHORT loc_1_5919
        lea    esi, [edx - 34h]
        push   edx
        call   sub_1_3B50
        add    esp, 4
loc_1_5919:
        or     esi, esi
        jne    SHORT loc_1_5949
        db  081h, 03Dh, 06Ch, 079h, 000h, 000h, 06Ch, 079h, 000h, 000h
        mov    esi, 0
        je     SHORT loc_1_5945
        db  08Bh, 035h, 06Ch, 079h, 000h, 000h
        sub    esi, 34h
        db  0A1h, 06Ch, 079h, 000h, 000h
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_5945:
        or     esi, esi
        je     SHORT loc_1_59C4
loc_1_5949:
        mov    byte ptr [esi + 32h], 0
        mov    ax, word ptr [esi + 8]
        cmp    bp, ax
        jbe    SHORT loc_1_5960
        mov    bp, ax
        mov    byte ptr [esp + 13h], 6
        jmp    SHORT loc_1_5969
loc_1_5960:
        mov    byte ptr [esp + 13h], 0
        mov    word ptr [esi + 8], bp
loc_1_5969:
        lea    ecx, [esi + 0ah]
        mov    eax, dword ptr [ebx + 2]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx + 6]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebx + 0ah]
        add    ebx, 2
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [ecx + 0ch], eax
        push   ebp
        add    edi, 24h
        mov    eax, dword ptr [esi + 4]
        push   eax
        push   40h
        push   edi
        call   sub_1_3B60
        mov    al, byte ptr [esp + 23h]
        add    esp, 10h
        mov    byte ptr [esi + 1], al
        test   byte ptr [esi], 80h
        je     SHORT loc_1_59C4
        db  0C7h, 046h, 034h, 090h, 078h, 000h, 000h
        add    esi, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [esi + 4], eax
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [eax], esi
        db  089h, 035h, 094h, 078h, 000h, 000h
loc_1_59C4:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h
sub_1_59D0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 14h]
        mov    eax, dword ptr [esp + 14h]
        add    edi, 6ah
        mov    ax, word ptr [eax + 4eh]
        push   eax
        call   sub_1_0187
        add    esp, 4
        lea    ebp, [eax - 40h]
        db  081h, 03Dh, 018h, 079h, 000h, 000h, 018h, 079h, 000h, 000h
        mov    esi, 0
        je     SHORT loc_1_5A17
        db  08Bh, 035h, 018h, 079h, 000h, 000h
        sub    esi, 34h
        db  0A1h, 018h, 079h, 000h, 000h
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_5A17:
        or     esi, esi
        je     NEAR PTR loc_1_5ABC
        add    edi, 2
loc_1_5A22:
        cmp    word ptr [esi + 8], bp
        mov    bl, 6
        jb     SHORT loc_1_5A30
        sub    bl, bl
        mov    word ptr [esi + 8], bp
loc_1_5A30:
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 1ah], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esi + 1eh], eax
        lea    ecx, [esi + 1ah]
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ecx + 0ch], eax
        mov    ax, word ptr [esi + 8]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   40h
        add    eax, 24h
        push   eax
        call   sub_1_3B60
        add    esp, 10h
        mov    byte ptr [esi + 1], bl
        test   byte ptr [esi], 80h
        je     SHORT loc_1_5A8C
        db  0C7h, 046h, 034h, 090h, 078h, 000h, 000h
        add    esi, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [esi + 4], eax
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [eax], esi
        db  089h, 035h, 094h, 078h, 000h, 000h
loc_1_5A8C:
        db  081h, 03Dh, 018h, 079h, 000h, 000h, 018h, 079h, 000h, 000h
        mov    esi, 0
        je     SHORT loc_1_5AB4
        db  08Bh, 035h, 018h, 079h, 000h, 000h
        sub    esi, 34h
        db  0A1h, 018h, 079h, 000h, 000h
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_5AB4:
        or     esi, esi
        jne    NEAR PTR loc_1_5A22
loc_1_5ABC:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5AD0:
        push   esi
        sub    esi, esi
        mov    eax, dword ptr [esp + 8]
        add    eax, 40h
        push   eax
        call   sub_1_3240
        add    esp, 4
        cmp    eax, esi
        je     SHORT loc_1_5B00
        call   sub_1_3860
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_5B00
        db  0C7h, 046h, 004h, 010h, 03Ah, 000h, 000h
        db  0C7h, 046h, 040h, 000h, 051h, 000h, 000h
loc_1_5B00:
        mov    eax, esi
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5B10:
        push   esi
        sub    esi, esi
        mov    eax, dword ptr [esp + 8]
        add    eax, 40h
        push   eax
        db  068h, 0C0h, 078h, 000h, 000h
        call   sub_1_2780
        add    esp, 8
        cmp    eax, esi
        je     SHORT loc_1_5B45
        call   sub_1_3860
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_5B45
        db  0C7h, 046h, 004h, 010h, 03Ah, 000h, 000h
        db  0C7h, 046h, 040h, 0D0h, 054h, 000h, 000h
loc_1_5B45:
        mov    eax, esi
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5B50:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        sub    edi, edi
        lea    ecx, [ebx + 40h]
        mov    al, byte ptr [ebx + 3eh]
        lea    esi, [ebx + 1eh]
        and    al, 84h
        cmp    al, 84h
        jne    SHORT loc_1_5BA6
        push   ecx
        db  068h, 0ECh, 078h, 000h, 000h
        call   sub_1_2780
        add    esp, 8
        cmp    eax, edi
        je     SHORT loc_1_5BBF
        mov    cl, byte ptr [esi + 20h]
        and    cl, 0c7h
        or     byte ptr [eax + 28h], cl
        test   byte ptr [ebx + 4], 0fh
        db  0B9h, 0ACh, 078h, 000h, 000h
        je     SHORT loc_1_5B92
        mov    ecx, dword ptr [esp + 14h]
loc_1_5B92:
        push   ecx
        mov    ecx, dword ptr [ebx + 12h]
        push   ecx
        push   eax
        call   sub_1_2C80
        add    esp, 0ch
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_5BA6:
        call   sub_1_3860
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_5BBF
        db  0C7h, 047h, 004h, 010h, 03Ah, 000h, 000h
        db  0C7h, 047h, 040h, 0C0h, 051h, 000h, 000h
loc_1_5BBF:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5BD0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ecx, dword ptr [esi + 40h]
        cmp    byte ptr [ecx + 31h], 0ffh
        jne    SHORT loc_1_5C06
        and    byte ptr [ecx + 1], 7fh
        test   byte ptr [ecx], 80h
        je     SHORT loc_1_5C06
        db  0C7h, 041h, 034h, 090h, 078h, 000h, 000h
        add    ecx, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  089h, 00Dh, 094h, 078h, 000h, 000h
loc_1_5C06:
        push   esi
        call   sub_1_59D0
        add    esp, 4
        push   esi
        call   sub_1_3900
        add    esp, 4
        call   sub_1_2E80
        pop    esi
        ret
        db  090h
loc_1_5C20:
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 40h]
        cmp    byte ptr [edx + 31h], 0ffh
        jne    SHORT loc_1_5C55
        and    byte ptr [edx + 1], 7fh
        test   byte ptr [edx], 80h
        je     SHORT loc_1_5C55
        db  0C7h, 042h, 034h, 090h, 078h, 000h, 000h
        add    edx, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [edx + 4], eax
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [eax], edx
        db  089h, 015h, 094h, 078h, 000h, 000h
loc_1_5C55:
        push   ecx
        call   sub_1_3900
        add    esp, 4
        jmp    NEAR PTR sub_1_2E80
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5C70:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        sub    edi, edi
        lea    ebp, [eax + 1eh]
        mov    ax, word ptr [eax + 2]
        push   eax
        call   sub_1_0187
        add    esp, 4
        lea    ebx, [eax - 40h]
        lea    eax, [ebp + 12h]
        push   eax
        call   sub_1_3240
        add    esp, 4
        mov    esi, eax
        cmp    esi, edi
        je     NEAR PTR loc_1_5D7C
        test   byte ptr [esi + 14h], 4
        je     NEAR PTR loc_1_5D7C
        call   sub_1_3860
        mov    edi, eax
        or     edi, edi
        je     NEAR PTR loc_1_5D7C
        mov    ecx, dword ptr [esi + 20h]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 20h
        mov    esi, 0
        je     SHORT loc_1_5CD9
        lea    esi, [ecx - 34h]
        push   ecx
        call   sub_1_3B50
        add    esp, 4
loc_1_5CD9:
        or     esi, esi
        jne    SHORT loc_1_5D20
        db  081h, 03Dh, 06Ch, 079h, 000h, 000h, 06Ch, 079h, 000h, 000h
        mov    esi, 0
        je     SHORT loc_1_5D05
        db  08Bh, 035h, 06Ch, 079h, 000h, 000h
        sub    esi, 34h
        db  0A1h, 06Ch, 079h, 000h, 000h
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_5D05:
        or     esi, esi
        jne    SHORT loc_1_5D20
        push   edi
        call   sub_1_3900
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5D20:
        mov    byte ptr [esi + 32h], 0
        mov    ax, word ptr [esi + 8]
        cmp    bx, ax
        jbe    SHORT loc_1_5D34
        mov    bx, ax
        mov    al, 6
        jmp    SHORT loc_1_5D3A
loc_1_5D34:
        sub    al, al
        mov    word ptr [esi + 8], bx
loc_1_5D3A:
        lea    edx, [esi + 0ah]
        mov    ecx, dword ptr [ebp + 2]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [ebp + 6]
        mov    dword ptr [edx + 4], ecx
        mov    ecx, dword ptr [ebp + 0ah]
        add    ebp, 2
        mov    dword ptr [edx + 8], ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 0ch], ecx
        mov    word ptr [edi + 28h], 40h
        or     al, 80h
        mov    word ptr [edi + 2eh], bx
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [edi + 2ah], ecx
        mov    word ptr [edi + 22h], 2
        mov    dword ptr [edi + 40h], esi
        db  0C7h, 047h, 004h, 020h, 05Ch, 000h, 000h
        mov    byte ptr [esi + 1], al
loc_1_5D7C:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5D90:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        sub    edi, edi
        lea    ebp, [eax + 1eh]
        mov    ax, word ptr [eax + 2]
        push   eax
        call   sub_1_0187
        add    esp, 4
        lea    ebx, [eax - 40h]
        db  081h, 03Dh, 018h, 079h, 000h, 000h, 018h, 079h, 000h, 000h
        mov    esi, 0
        je     SHORT loc_1_5DD5
        db  08Bh, 035h, 018h, 079h, 000h, 000h
        sub    esi, 34h
        db  0A1h, 018h, 079h, 000h, 000h
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_5DD5:
        or     esi, esi
        je     NEAR PTR loc_1_5E64
        call   sub_1_3860
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_5E10
        db  0C7h, 046h, 034h, 018h, 079h, 000h, 000h
        add    esi, 34h
        db  0A1h, 01Ch, 079h, 000h, 000h
        mov    dword ptr [esi + 4], eax
        db  0A1h, 01Ch, 079h, 000h, 000h
        pop    ebp
        mov    dword ptr [eax], esi
        sub    eax, eax
        db  089h, 035h, 01Ch, 079h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_5E10:
        cmp    word ptr [esi + 8], bx
        mov    al, 6
        jb     SHORT loc_1_5E1E
        sub    al, al
        mov    word ptr [esi + 8], bx
loc_1_5E1E:
        lea    edx, [esi + 1ah]
        mov    ecx, dword ptr [ebp + 2]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [ebp + 6]
        mov    dword ptr [edx + 4], ecx
        mov    ecx, dword ptr [ebp + 0ah]
        add    ebp, 2
        mov    dword ptr [edx + 8], ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 0ch], ecx
        mov    word ptr [edi + 28h], 40h
        mov    ecx, dword ptr [esi + 4]
        or     al, 80h
        mov    dword ptr [edi + 2ah], ecx
        mov    cx, word ptr [esi + 8]
        mov    word ptr [edi + 2eh], cx
        mov    word ptr [edi + 22h], 2
        mov    dword ptr [edi + 40h], esi
        db  0C7h, 047h, 004h, 0D0h, 05Bh, 000h, 000h
        mov    byte ptr [esi + 1], al
loc_1_5E64:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_5E70:
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 40h]
        cmp    byte ptr [edx + 31h], 0ffh
        jne    SHORT loc_1_5EA5
        and    byte ptr [edx + 1], 7fh
        test   byte ptr [edx], 80h
        je     SHORT loc_1_5EA5
        db  0C7h, 042h, 034h, 090h, 078h, 000h, 000h
        add    edx, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [edx + 4], eax
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [eax], edx
        db  089h, 015h, 094h, 078h, 000h, 000h
loc_1_5EA5:
        push   ecx
        call   sub_1_3900
        add    esp, 4
        jmp    NEAR PTR sub_1_2E80
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_5EC0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 01Dh, 080h, 079h, 000h, 000h
        sub    ebp, ebp
        cmp    ebx, ebp
        je     NEAR PTR loc_1_5F95
        lea    eax, [ebx + 3ch]
        mov    dword ptr [esp + 10h], eax
        mov    esi, dword ptr [eax]
        cmp    esi, ebp
        je     NEAR PTR loc_1_5F95
        mov    edi, dword ptr [esp + 18h]
        lea    esi, [esi + 52h]
        lea    edi, [edi + 12h]
        mov    ecx, 2
        repe cmpsd
        jne    NEAR PTR loc_1_5F95
        mov    ax, word ptr [esi]
        cmp    ax, word ptr [edi]
        jne    NEAR PTR loc_1_5F95
        call   sub_1_3860
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_1_5F95
        mov    eax, dword ptr [esp + 10h]
        db  0C7h, 005h, 080h, 079h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax]
        test   byte ptr [eax + 3ch], 8
        je     SHORT loc_1_5F44
        push   eax
        call   sub_1_3C30
        mov    eax, dword ptr [esp + 14h]
        add    esp, 4
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_3820
        add    esp, 4
loc_1_5F44:
        mov    eax, dword ptr [esp + 18h]
        mov    ax, word ptr [eax + 2]
        push   eax
        call   sub_1_0187
        add    esp, 4
        sub    ax, 20h
        mov    cx, word ptr [ebx + 8]
        cmp    ax, cx
        jbe    SHORT loc_1_5F69
        mov    ax, cx
        mov    cl, 6
        jmp    SHORT loc_1_5F6F
loc_1_5F69:
        mov    word ptr [ebx + 8], ax
        sub    cl, cl
loc_1_5F6F:
        mov    word ptr [ebp + 22h], 2
        mov    word ptr [ebp + 28h], 20h
        mov    edx, dword ptr [ebx + 4]
        mov    dword ptr [ebp + 2ah], edx
        or     cl, 80h
        mov    word ptr [ebp + 2eh], ax
        db  0C7h, 045h, 004h, 070h, 05Eh, 000h, 000h
        mov    dword ptr [ebp + 40h], ebx
        mov    byte ptr [ebx + 1], cl
loc_1_5F95:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
sub_1_5FA0:
        push   esi
        mov    edx, dword ptr [esp + 0ch]
        sub    eax, eax
        cmp    word ptr [edx + 1ch], 5504h
        je     SHORT loc_1_5FC0
        db  08Bh, 00Dh, 044h, 078h, 000h, 000h
        inc    ecx
        db  089h, 00Dh, 044h, 078h, 000h, 000h
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_5FC0:
        cmp    byte ptr [edx + 5], 14h
        jne    SHORT loc_1_5FCB
        mov    cl, byte ptr [edx + 3fh]
        jmp    SHORT loc_1_5FCE
loc_1_5FCB:
        mov    cl, byte ptr [edx + 1fh]
loc_1_5FCE:
        dec    cl
        cmp    cl, 0ch
        jae    SHORT loc_1_6012
        xor    eax, eax
        mov    al, cl
        lea    esi, [eax*4]
        db  08Bh, 08Eh, 020h, 077h, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_1_5FF7
        mov    eax, dword ptr [esp + 10h]
        push   eax
        push   edx
        call   ecx
        add    esp, 8
        jmp    SHORT loc_1_601F
loc_1_5FF7:
        call   sub_1_3860
        or     eax, eax
        je     SHORT loc_1_601F
        db  0C7h, 040h, 004h, 010h, 03Ah, 000h, 000h
        db  08Bh, 08Eh, 0E4h, 076h, 000h, 000h
        mov    dword ptr [eax + 40h], ecx
        jmp    SHORT loc_1_601F
loc_1_6012:
        db  08Bh, 00Dh, 044h, 078h, 000h, 000h
        inc    ecx
        db  089h, 00Dh, 044h, 078h, 000h, 000h
loc_1_601F:
        mov    ecx, dword ptr [esp + 14h]
        pop    esi
        mov    dword ptr [ecx], 0
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_6030:
        db  08Bh, 015h, 080h, 079h, 000h, 000h
        or     edx, edx
        mov    ecx, dword ptr [esp + 4]
        je     SHORT loc_1_6096
        cmp    dword ptr [edx + 3ch], ecx
        jne    SHORT loc_1_6096
        mov    ax, word ptr [ecx + 3eh]
        or     ax, ax
        je     SHORT loc_1_6063
        dec    ax
        mov    word ptr [ecx + 3eh], ax
        db  0C7h, 041h, 004h, 0A0h, 060h, 000h, 000h
        push   ecx
        call   sub_1_0292
        add    esp, 4
        ret
loc_1_6063:
        mov    byte ptr [edx + 1], 5
        test   byte ptr [edx], 80h
        je     SHORT loc_1_608B
        db  0C7h, 042h, 034h, 090h, 078h, 000h, 000h
        add    edx, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [edx + 4], eax
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [eax], edx
        db  089h, 015h, 094h, 078h, 000h, 000h
loc_1_608B:
        db  0C7h, 005h, 080h, 079h, 000h, 000h, 000h, 000h, 000h, 000h
        ret
loc_1_6096:
        push   ecx
        call   sub_1_3820
        add    esp, 4
        ret
loc_1_60A0:
        db  083h, 03Dh, 080h, 079h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 4]
        je     SHORT loc_1_60C5
        db  0C7h, 040h, 004h, 030h, 060h, 000h, 000h
        db  08Bh, 00Dh, 048h, 079h, 000h, 000h
        push   ecx
        push   eax
        call   sub_1_3BE0
        add    esp, 8
        ret
loc_1_60C5:
        push   eax
        call   sub_1_3820
        add    esp, 4
        ret
        db  090h
loc_1_60D0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 4], 0
        je     SHORT loc_1_612D
        cmp    word ptr [esi + 8], 3ch
        jb     SHORT loc_1_612D
        lea    eax, [esi + 0ah]
        cmp    byte ptr [eax], 2ah
        je     SHORT loc_1_6114
        push   eax
        call   sub_1_3240
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_6114
        db  0C7h, 046h, 03Ch, 040h, 061h, 000h, 000h
        push   esi
        call   sub_1_27D0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_6110
        mov    al, 21h
        pop    esi
        ret
        db  090h
loc_1_6110:
        mov    al, 0ffh
        pop    esi
        ret
loc_1_6114:
        mov    ax, word ptr [esi + 8]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_6230
        add    esp, 8
        mov    word ptr [esi + 8], ax
        sub    al, al
        pop    esi
        ret
loc_1_612D:
        mov    al, 1
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_6140:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 14h]
        or     edi, edi
        jne    SHORT loc_1_6152
        mov    al, 5
        jmp    NEAR PTR loc_1_61F2
loc_1_6152:
        db  083h, 03Dh, 080h, 079h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_61F0
        call   sub_1_37F0
        or     eax, eax
        je     NEAR PTR loc_1_61F0
        test   byte ptr [edi + 28h], 80h
        jne    SHORT loc_1_61E0
        mov    esi, dword ptr [esp + 10h]
        db  089h, 035h, 080h, 079h, 000h, 000h
        mov    dword ptr [esi + 3ch], eax
        mov    word ptr [eax + 28h], 20h
        lea    ecx, [edi + 1ah]
        lea    ebx, [eax + 1ch]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [ebx], edx
        mov    cx, word ptr [ecx + 4]
        mov    word ptr [ebx + 4], cx
        lea    edx, [eax + 4ch]
        db  0C7h, 040h, 004h, 0A0h, 060h, 000h, 000h
        db  08Bh, 00Dh, 054h, 078h, 000h, 000h
        mov    word ptr [eax + 3eh], cx
        mov    byte ptr [edx + 5], 4
        add    edi, 10h
        lea    ebx, [edx + 6]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [ebx], ecx
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [ebx + 4], ecx
        add    edx, 1eh
        mov    cx, word ptr [edi + 8]
        mov    word ptr [ebx + 8], cx
        mov    byte ptr [edx], 0
        mov    byte ptr [edx + 1], 9
        push   eax
        call   sub_1_0292
        add    esp, 4
        mov    al, 0ffh
        jmp    SHORT loc_1_61F6
        db  08Bh, 0FFh
loc_1_61E0:
        push   eax
        call   sub_1_3820
        add    esp, 4
        mov    al, 5
        jmp    SHORT loc_1_61F2
        db  08Dh, 049h, 000h
loc_1_61F0:
        mov    al, 9
loc_1_61F2:
        mov    esi, dword ptr [esp + 10h]
loc_1_61F6:
        cmp    al, 0ffh
        je     SHORT loc_1_6221
        mov    byte ptr [esi + 1], al
        test   byte ptr [esi], 80h
        je     SHORT loc_1_6221
        db  0C7h, 046h, 034h, 090h, 078h, 000h, 000h
        add    esi, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [esi + 4], eax
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [eax], esi
        db  089h, 035h, 094h, 078h, 000h, 000h
loc_1_6221:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_6230:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0B8h, 024h, 079h, 000h, 000h
        mov    ecx, dword ptr [esp + 14h]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ecx + 4], ax
        mov    byte ptr [ecx + 6], 40h
        mov    byte ptr [ecx + 7], 0
        mov    byte ptr [ecx + 8], 1
        mov    byte ptr [ecx + 9], 0
        db  0A1h, 064h, 078h, 000h, 000h
        shr    eax, 9
        mov    word ptr [ecx + 0ah], ax
        db  0A1h, 044h, 078h, 000h, 000h
        mov    word ptr [ecx + 0ch], ax
        mov    word ptr [ecx + 10h], 0
        mov    word ptr [ecx + 0eh], 0
        mov    word ptr [ecx + 12h], 0
        db  0A1h, 040h, 078h, 000h, 000h
        mov    dword ptr [ecx + 14h], eax
        db  0A1h, 078h, 079h, 000h, 000h
        mov    dword ptr [ecx + 18h], eax
        db  0A1h, 05Ch, 079h, 000h, 000h
        mov    word ptr [ecx + 1ch], ax
        mov    word ptr [ecx + 1eh], 0
        mov    dword ptr [ecx + 24h], 0
        mov    dword ptr [ecx + 20h], 0
        db  066h, 0A1h, 098h, 078h, 000h, 000h
        mov    word ptr [ecx + 28h], ax
        mov    word ptr [ecx + 2ch], ax
        mov    word ptr [ecx + 2ah], ax
        mov    word ptr [ecx + 2eh], 0
        db  066h, 08Bh, 015h, 0E4h, 078h, 000h, 000h
        mov    ax, dx
        cmp    ax, 200h
        jb     SHORT loc_1_62D9
        mov    ax, 200h
loc_1_62D9:
        mov    word ptr [ecx + 30h], ax
        mov    word ptr [ecx + 32h], 0
        db  066h, 0A1h, 044h, 079h, 000h, 000h
        mov    word ptr [ecx + 36h], ax
        mov    word ptr [ecx + 34h], ax
        mov    word ptr [ecx + 38h], dx
        mov    word ptr [ecx + 3ah], 0
        db  083h, 03Dh, 084h, 079h, 000h, 000h, 001h
        db  08Bh, 035h, 0F4h, 078h, 000h, 000h
        jbe    SHORT loc_1_6334
        lea    edi, [esi + 58h]
        db  08Bh, 015h, 084h, 079h, 000h, 000h
        dec    edx
loc_1_6314:
        test   byte ptr [edi], 40h
        je     SHORT loc_1_632E
        inc    word ptr [ecx + 3ah]
        mov    eax, dword ptr [edi + 24h]
        or     eax, eax
        je     SHORT loc_1_632E
loc_1_6324:
        inc    word ptr [ecx + 32h]
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_6324
loc_1_632E:
        add    edi, 44h
        dec    edx
        jne    SHORT loc_1_6314
loc_1_6334:
        mov    eax, 4eh
        mov    edi, 1
        add    ecx, 3ch
        cmp    word ptr [esp + 18h], ax
        jb     NEAR PTR loc_1_63E5
        db  08Bh, 01Dh, 050h, 078h, 000h, 000h
loc_1_6352:
        inc    edi
        cmp    edi, 0ffh
        jge    NEAR PTR loc_1_63E5
        xor    edx, edx
        mov    dl, byte ptr [edi + ebx]
        db  039h, 015h, 084h, 079h, 000h, 000h
        jbe    SHORT loc_1_63D3
        mov    ebp, edx
        shl    ebp, 4
        add    ebp, edx
        lea    ebp, [esi + ebp*4]
        mov    edx, dword ptr [ebp]
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [ebp + 4]
        mov    dword ptr [ecx + 4], edx
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 8], edx
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 0ch], edx
        mov    dl, byte ptr [ebp + 15h]
        mov    byte ptr [ecx + 10h], dl
        mov    dl, byte ptr [ebp + 14h]
        test   dl, 4
        je     SHORT loc_1_63A4
        lea    ebp, [ecx + 11h]
        mov    byte ptr [ebp], 4
        jmp    SHORT loc_1_63C4
loc_1_63A4:
        test   dl, 1
        je     SHORT loc_1_63B2
        lea    ebp, [ecx + 11h]
        mov    byte ptr [ebp], 5
        jmp    SHORT loc_1_63C4
loc_1_63B2:
        test   dl, 2
        lea    ebp, [ecx + 11h]
        je     SHORT loc_1_63C0
        mov    byte ptr [ebp], 6
        jmp    SHORT loc_1_63C4
loc_1_63C0:
        mov    byte ptr [ebp], 0
loc_1_63C4:
        test   dl, 80h
        je     SHORT loc_1_63CD
        or     byte ptr [ebp], 80h
loc_1_63CD:
        add    eax, 12h
        add    ecx, 12h
loc_1_63D3:
        mov    edx, dword ptr [esp + 18h]
        and    edx, 0ffffh
        cmp    edx, eax
        jge    NEAR PTR loc_1_6352
loc_1_63E5:
        sub    eax, 12h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_1_63F0:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 10h]
        mov    al, byte ptr [ecx + 10h]
        mov    byte ptr [edx], al
        mov    al, byte ptr [ecx + 6]
        cmp    al, 1
        jne    SHORT loc_1_640C
        mov    byte ptr [edx + 1], 2
        jmp    SHORT loc_1_641E
loc_1_640C:
        cmp    al, 6
        je     SHORT loc_1_641A
        test   byte ptr [ecx + 4], 10h
        mov    byte ptr [edx + 1], 3
        je     SHORT loc_1_641E
loc_1_641A:
        mov    byte ptr [edx + 1], 4
loc_1_641E:
        lea    esi, [edx + 2]
        mov    eax, dword ptr [ecx + 7ah]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ecx + 7eh]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [ecx + 82h]
        lea    edi, [ecx + 7ah]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [esi + 0ch], eax
        lea    esi, [edx + 12h]
        mov    eax, dword ptr [ecx + 8ah]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ecx + 8eh]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [ecx + 92h]
        lea    edi, [ecx + 8ah]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [esi + 0ch], eax
        mov    byte ptr [edx + 22h], 0
        lea    esi, [ecx + 5ch]
        mov    byte ptr [edx + 23h], 0
        mov    eax, ecx
        sub    eax, dword ptr [esi]
        cmp    eax, -5ch
        je     SHORT loc_1_6489
loc_1_647B:
        inc    byte ptr [edx + 23h]
        mov    esi, dword ptr [esi]
        mov    eax, ecx
        sub    eax, dword ptr [esi]
        cmp    eax, -5ch
        jne    SHORT loc_1_647B
loc_1_6489:
        mov    eax, dword ptr [ecx + 54h]
        lea    esi, [ecx + 54h]
        sub    eax, ecx
        cmp    eax, 54h
        je     SHORT loc_1_64A4
loc_1_6496:
        inc    byte ptr [edx + 22h]
        mov    esi, dword ptr [esi]
        mov    eax, dword ptr [esi]
        sub    eax, ecx
        cmp    eax, 54h
        jne    SHORT loc_1_6496
loc_1_64A4:
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_64B0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 1ch]
        mov    esi, dword ptr [eax + 4]
        or     esi, esi
        je     NEAR PTR loc_1_6650
        cmp    word ptr [eax + 8], 4
        jb     NEAR PTR loc_1_6650
        mov    edi, 4
        lea    ebx, [esi + 4]
        mov    byte ptr [esi], 0
        mov    byte ptr [esi + 1], 0
        mov    byte ptr [esi + 3], 0
        add    eax, 1ah
        mov    byte ptr [esi + 2], 0
        cmp    byte ptr [eax], 2ah
        jne    NEAR PTR loc_1_65A6
        mov    dword ptr [esp + 10h], 0
        db  083h, 03Dh, 084h, 079h, 000h, 000h, 000h
        je     NEAR PTR loc_1_6620
        mov    dword ptr [esp + 14h], 0
loc_1_6511:
        mov    ecx, dword ptr [esp + 14h]
        db  003h, 00Dh, 0F4h, 078h, 000h, 000h
        test   byte ptr [ecx + 14h], 40h
        je     SHORT loc_1_658B
        mov    eax, dword ptr [ecx + 18h]
        lea    edx, [ecx + 18h]
        sub    eax, ecx
        cmp    eax, 18h
        je     SHORT loc_1_653E
        mov    edi, edi
loc_1_6530:
        inc    byte ptr [esi + 3]
        mov    edx, dword ptr [edx]
        mov    eax, dword ptr [edx]
        sub    eax, ecx
        cmp    eax, 18h
        jne    SHORT loc_1_6530
loc_1_653E:
        mov    eax, dword ptr [ecx + 20h]
        lea    edx, [ecx + 20h]
        sub    eax, ecx
        cmp    eax, 20h
        je     SHORT loc_1_6559
loc_1_654B:
        inc    byte ptr [esi + 2]
        mov    edx, dword ptr [edx]
        mov    eax, dword ptr [edx]
        sub    eax, ecx
        cmp    eax, 20h
        jne    SHORT loc_1_654B
loc_1_6559:
        mov    ebp, dword ptr [ecx + 38h]
        or     ebp, ebp
        je     SHORT loc_1_658B
loc_1_6560:
        mov    eax, dword ptr [esp + 1ch]
        xor    ecx, ecx
        inc    byte ptr [esi + 1]
        mov    cx, word ptr [eax + 8]
        lea    eax, [edi + 24h]
        cmp    ecx, eax
        jb     SHORT loc_1_6584
        push   ebx
        push   ebp
        call   sub_1_63F0
        add    esp, 8
        add    edi, 24h
        add    ebx, 24h
loc_1_6584:
        mov    ebp, dword ptr [ebp]
        or     ebp, ebp
        jne    SHORT loc_1_6560
loc_1_658B:
        add    dword ptr [esp + 14h], 44h
        inc    dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 10h]
        db  03Bh, 005h, 084h, 079h, 000h, 000h
        jb     NEAR PTR loc_1_6511
        jmp    SHORT loc_1_6620
loc_1_65A6:
        push   eax
        call   sub_1_3240
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_6620
        mov    cl, byte ptr [eax + 15h]
        mov    byte ptr [esi], cl
        mov    ecx, dword ptr [eax + 18h]
        lea    edx, [eax + 18h]
        sub    ecx, eax
        cmp    ecx, 18h
        je     SHORT loc_1_65D3
loc_1_65C5:
        inc    byte ptr [esi + 3]
        mov    edx, dword ptr [edx]
        mov    ecx, dword ptr [edx]
        sub    ecx, eax
        cmp    ecx, 18h
        jne    SHORT loc_1_65C5
loc_1_65D3:
        lea    edx, [eax + 20h]
        mov    ecx, eax
        sub    ecx, dword ptr [edx]
        cmp    ecx, -20h
        je     SHORT loc_1_65EE
loc_1_65E0:
        inc    byte ptr [esi + 2]
        mov    edx, dword ptr [edx]
        mov    ecx, eax
        sub    ecx, dword ptr [edx]
        cmp    ecx, -20h
        jne    SHORT loc_1_65E0
loc_1_65EE:
        mov    ebp, dword ptr [eax + 38h]
        or     ebp, ebp
        je     SHORT loc_1_6620
loc_1_65F5:
        mov    eax, dword ptr [esp + 1ch]
        xor    ecx, ecx
        inc    byte ptr [esi + 1]
        mov    cx, word ptr [eax + 8]
        lea    eax, [edi + 24h]
        cmp    ecx, eax
        jb     SHORT loc_1_6619
        push   ebx
        push   ebp
        call   sub_1_63F0
        add    esp, 8
        add    edi, 24h
        add    ebx, 24h
loc_1_6619:
        mov    ebp, dword ptr [ebp]
        or     ebp, ebp
        jne    SHORT loc_1_65F5
loc_1_6620:
        mov    eax, dword ptr [esp + 1ch]
        mov    word ptr [eax + 8], di
        xor    eax, eax
        mov    al, byte ptr [esi + 1]
        shl    eax, 2
        lea    eax, [eax + eax*8]
        sub    eax, edi
        cmp    eax, -4
        jne    SHORT loc_1_6644
        sub    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_6644:
        mov    al, 6
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_6650:
        mov    al, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_6660:
        mov    al, 24h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_6670:
        db  0C7h, 005h, 070h, 078h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0B4h, 078h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 050h, 079h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 080h, 079h, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_1_6AA0
        db  0A3h, 070h, 078h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_66D4
        db  083h, 00Dh, 0B4h, 078h, 000h, 000h, 002h
        db  080h, 03Dh, 060h, 079h, 000h, 000h, 000h
        je     SHORT loc_1_66CA
        call   sub_1_022E
        db  0A3h, 070h, 078h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_66D4
        db  0C6h, 005h, 060h, 079h, 000h, 000h, 000h
loc_1_66CA:
        push   0
        call   sub_1_0201
        add    esp, 4
loc_1_66D4:
        db  0A3h, 070h, 078h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_6744
        db  083h, 00Dh, 0B4h, 078h, 000h, 000h, 001h
        db  068h, 020h, 079h, 000h, 000h
        call   sub_1_02CA
        db  066h, 0C7h, 005h, 02Ah, 079h, 000h, 000h, 004h, 055h
        db  0A1h, 020h, 079h, 000h, 000h
        db  0A3h, 0A8h, 078h, 000h, 000h
        db  0C7h, 005h, 0ACh, 078h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        add    esp, 4
        db  066h, 0C7h, 005h, 0B0h, 078h, 000h, 000h, 0FFh, 0FFh
        call   sub_1_74F0
        mov    cx, 3
        db  066h, 0A1h, 0E4h, 078h, 000h, 000h
        sub    dx, dx
        div    cx
        add    ax, 0fh
        and    ax, 0fff0h
        db  066h, 0A3h, 08Ch, 078h, 000h, 000h
        call   sub_1_6C90
        db  0A3h, 070h, 078h, 000h, 000h
loc_1_6744:
        call   sub_1_0081
        db  0A1h, 070h, 078h, 000h, 000h
        or     eax, eax
        db  08Bh, 00Dh, 0B4h, 078h, 000h, 000h
        jne    NEAR PTR loc_1_6890
        or     ecx, 4
        db  089h, 00Dh, 0B4h, 078h, 000h, 000h
        db  0C7h, 005h, 058h, 078h, 000h, 000h, 058h, 078h, 000h, 000h
        db  0C7h, 005h, 05Ch, 078h, 000h, 000h, 058h, 078h, 000h, 000h
        db  0C7h, 005h, 02Ch, 079h, 000h, 000h, 02Ch, 079h, 000h, 000h
        db  0C7h, 005h, 030h, 079h, 000h, 000h, 02Ch, 079h, 000h, 000h
        db  0C7h, 005h, 084h, 078h, 000h, 000h, 084h, 078h, 000h, 000h
        db  0C7h, 005h, 088h, 078h, 000h, 000h, 084h, 078h, 000h, 000h
        db  0C7h, 005h, 090h, 078h, 000h, 000h, 090h, 078h, 000h, 000h
        db  0C7h, 005h, 094h, 078h, 000h, 000h, 090h, 078h, 000h, 000h
        db  0C7h, 005h, 0F8h, 078h, 000h, 000h, 0F8h, 078h, 000h, 000h
        db  0C7h, 005h, 0FCh, 078h, 000h, 000h, 0F8h, 078h, 000h, 000h
        db  0C7h, 005h, 06Ch, 079h, 000h, 000h, 06Ch, 079h, 000h, 000h
        db  0C7h, 005h, 070h, 079h, 000h, 000h, 06Ch, 079h, 000h, 000h
        db  0C7h, 005h, 074h, 078h, 000h, 000h, 074h, 078h, 000h, 000h
        db  0C7h, 005h, 078h, 078h, 000h, 000h, 074h, 078h, 000h, 000h
        db  0C7h, 005h, 018h, 079h, 000h, 000h, 018h, 079h, 000h, 000h
        db  0C7h, 005h, 01Ch, 079h, 000h, 000h, 018h, 079h, 000h, 000h
        db  0C7h, 005h, 0D4h, 078h, 000h, 000h, 0D4h, 078h, 000h, 000h
        db  0C7h, 005h, 0D8h, 078h, 000h, 000h, 0D4h, 078h, 000h, 000h
        db  0C7h, 005h, 048h, 078h, 000h, 000h, 048h, 078h, 000h, 000h
        db  0C7h, 005h, 04Ch, 078h, 000h, 000h, 048h, 078h, 000h, 000h
        db  0C7h, 005h, 0A0h, 078h, 000h, 000h, 0A0h, 078h, 000h, 000h
        db  0C7h, 005h, 0A4h, 078h, 000h, 000h, 0A0h, 078h, 000h, 000h
        db  0C7h, 005h, 044h, 078h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 078h, 079h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 040h, 078h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 06Ch, 078h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 05Ch, 079h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 064h, 078h, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_1_00E9
        db  08Bh, 00Dh, 0B4h, 078h, 000h, 000h
        or     ecx, 8
        db  0A1h, 070h, 078h, 000h, 000h
loc_1_6890:
        or     eax, eax
        jne    SHORT loc_1_68D3
        db  089h, 00Dh, 0B4h, 078h, 000h, 000h
        db  080h, 03Dh, 060h, 079h, 000h, 000h, 000h
        jne    SHORT loc_1_68D3
        db  0C7h, 005h, 04Ch, 079h, 000h, 000h, 000h, 000h, 000h, 000h
        lea    ecx, [ecx]
loc_1_68B0:
        call   sub_1_39A0
        or     eax, eax
        db  08Bh, 00Dh, 0B4h, 078h, 000h, 000h
        db  0A1h, 070h, 078h, 000h, 000h
        jne    SHORT loc_1_68B0
        db  083h, 03Dh, 04Ch, 079h, 000h, 000h, 000h
        je     SHORT loc_1_68D2
        or     ecx, 10h
        jmp    SHORT loc_1_68D3
loc_1_68D2:
        inc    eax
loc_1_68D3:
        db  089h, 00Dh, 0B4h, 078h, 000h, 000h
        db  0A3h, 070h, 078h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_68FB
        db  0A1h, 07Ch, 078h, 000h, 000h
        push   eax
        db  068h, 068h, 078h, 000h, 000h
        call   sub_1_0042
        add    esp, 8
        db  08Bh, 00Dh, 0B4h, 078h, 000h, 000h
loc_1_68FB:
        db  0A3h, 070h, 078h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_692A
        or     ecx, 20h
        db  089h, 00Dh, 0B4h, 078h, 000h, 000h
        db  0C6h, 005h, 074h, 079h, 000h, 000h, 001h
        call   sub_1_3560
        db  08Bh, 00Dh, 0B4h, 078h, 000h, 000h
        or     ecx, 8000h
        db  0A1h, 070h, 078h, 000h, 000h
loc_1_692A:
        db  089h, 00Dh, 0B4h, 078h, 000h, 000h
        db  0A3h, 070h, 078h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_69B3
        test   cl, 20h
        je     SHORT loc_1_6954
        xor    eax, eax
        db  0A0h, 068h, 078h, 000h, 000h
        push   eax
        call   sub_1_0072
        add    esp, 4
        db  08Bh, 00Dh, 0B4h, 078h, 000h, 000h
loc_1_6954:
        db  089h, 00Dh, 0B4h, 078h, 000h, 000h
        test   cl, 8
        je     SHORT loc_1_696A
        call   sub_1_010A
        db  08Bh, 00Dh, 0B4h, 078h, 000h, 000h
loc_1_696A:
        db  089h, 00Dh, 0B4h, 078h, 000h, 000h
        test   cl, 4
        je     SHORT loc_1_6995
        push   0
        db  0A1h, 0E0h, 078h, 000h, 000h
        push   eax
        call   sub_1_01FA
        add    esp, 8
        db  0C7h, 005h, 0E0h, 078h, 000h, 000h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 0B4h, 078h, 000h, 000h
loc_1_6995:
        db  089h, 00Dh, 0B4h, 078h, 000h, 000h
        test   cl, 1
        je     SHORT loc_1_69A5
        call   sub_1_0258
loc_1_69A5:
        db  0A1h, 014h, 077h, 000h, 000h
        dec    eax
        db  0A3h, 014h, 077h, 000h, 000h
        sub    eax, eax
        ret
loc_1_69B3:
        db  0A1h, 014h, 077h, 000h, 000h
        dec    eax
        db  0A3h, 014h, 077h, 000h, 000h
        mov    eax, 1
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_69D0:
        sub    esp, 8
        mov    dword ptr [esp], 0
        mov    dword ptr [esp + 4], 4
        lea    eax, [esp + 4]
        push   1
        push   eax
        lea    eax, [esp + 8]
        push   eax
        mov    eax, dword ptr [esp + 18h]
        push   3
        push   eax
        push   0
        db  0A1h, 07Ch, 078h, 000h, 000h
        push   eax
        db  0E8h, 005h, 096h, 0FEh, 0FFh
        add    esp, 1ch
        or     eax, eax
        jne    SHORT loc_1_6A3D
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [esp + 4], eax
        jb     SHORT loc_1_6A3D
        cmp    eax, 1
        jne    SHORT loc_1_6A27
        mov    ecx, dword ptr [esp + 10h]
        mov    al, byte ptr [esp]
        mov    byte ptr [ecx], al
        add    esp, 8
        ret
loc_1_6A27:
        cmp    eax, 2
        mov    eax, dword ptr [esp]
        mov    ecx, dword ptr [esp + 10h]
        jne    SHORT loc_1_6A3B
        mov    word ptr [ecx], ax
        add    esp, 8
        ret
loc_1_6A3B:
        mov    dword ptr [ecx], eax
loc_1_6A3D:
        add    esp, 8
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_6A50:
        sub    esp, 8
        mov    dword ptr [esp + 4], 4
        lea    eax, [esp + 4]
        push   1
        push   eax
        lea    eax, [esp + 8]
        push   eax
        mov    eax, dword ptr [esp + 18h]
        push   3
        push   eax
        push   0
        db  0A1h, 07Ch, 078h, 000h, 000h
        push   eax
        db  0E8h, 08Dh, 095h, 0FEh, 0FFh
        add    esp, 1ch
        or     eax, eax
        jne    SHORT loc_1_6A91
        cmp    byte ptr [esp], 1
        sbb    cl, cl
        inc    cl
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [eax], cl
loc_1_6A91:
        add    esp, 8
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_6AA0:
        db  0C6h, 005h, 068h, 078h, 000h, 000h, 0FFh
        push   1
        db  068h, 068h, 078h, 000h, 000h
        db  068h, 05Ch, 077h, 000h, 000h
        call   sub_1_69D0
        add    esp, 0ch
        db  0C7h, 005h, 004h, 079h, 000h, 000h, 008h, 000h, 000h, 000h
        push   4
        db  068h, 004h, 079h, 000h, 000h
        db  068h, 068h, 077h, 000h, 000h
        call   sub_1_69D0
        add    esp, 0ch
        db  083h, 03Dh, 004h, 079h, 000h, 000h, 004h
        db  0A1h, 004h, 079h, 000h, 000h
        jbe    SHORT loc_1_6AF0
        add    eax, 8
        jmp    SHORT loc_1_6AF3
        db  08Dh, 064h, 024h, 000h
loc_1_6AF0:
        lea    eax, [eax + eax*2]
loc_1_6AF3:
        db  0A3h, 060h, 078h, 000h, 000h
        db  0C7h, 005h, 084h, 079h, 000h, 000h, 011h, 000h, 000h, 000h
        push   4
        db  068h, 084h, 079h, 000h, 000h
        db  068h, 078h, 077h, 000h, 000h
        call   sub_1_69D0
        add    esp, 0ch
        db  0C7h, 005h, 09Ch, 078h, 000h, 000h, 010h, 000h, 000h, 000h
        db  0C7h, 005h, 044h, 079h, 000h, 000h, 0FAh, 000h, 000h, 000h
        push   4
        db  068h, 044h, 079h, 000h, 000h
        db  068h, 080h, 077h, 000h, 000h
        call   sub_1_69D0
        add    esp, 0ch
        db  0C7h, 005h, 010h, 079h, 000h, 000h, 00Ah, 000h, 000h, 000h
        push   4
        db  068h, 010h, 079h, 000h, 000h
        db  068h, 08Ch, 077h, 000h, 000h
        call   sub_1_69D0
        add    esp, 0ch
        db  0C6h, 005h, 014h, 078h, 000h, 000h, 001h
        db  068h, 014h, 078h, 000h, 000h
        db  068h, 098h, 077h, 000h, 000h
        call   sub_1_6A50
        add    esp, 8
        db  0C6h, 005h, 090h, 079h, 000h, 000h, 001h
        db  068h, 090h, 079h, 000h, 000h
        db  068h, 0A4h, 077h, 000h, 000h
        call   sub_1_6A50
        add    esp, 8
        db  0C6h, 005h, 08Eh, 078h, 000h, 000h, 001h
        db  068h, 08Eh, 078h, 000h, 000h
        db  068h, 0B0h, 077h, 000h, 000h
        call   sub_1_6A50
        add    esp, 8
        db  080h, 03Dh, 08Eh, 078h, 000h, 000h, 000h
        jne    SHORT loc_1_6BB7
        db  0C6h, 005h, 090h, 079h, 000h, 000h, 000h
loc_1_6BB7:
        db  0C6h, 005h, 060h, 079h, 000h, 000h, 001h
        db  068h, 060h, 079h, 000h, 000h
        db  068h, 0C0h, 077h, 000h, 000h
        call   sub_1_6A50
        add    esp, 8
        db  0C7h, 005h, 054h, 078h, 000h, 000h, 004h, 000h, 000h, 000h
        push   4
        db  068h, 054h, 078h, 000h, 000h
        db  068h, 0CCh, 077h, 000h, 000h
        call   sub_1_69D0
        add    esp, 0ch
        db  0C7h, 005h, 048h, 079h, 000h, 000h, 008h, 000h, 000h, 000h
        push   4
        db  068h, 048h, 079h, 000h, 000h
        db  068h, 0DCh, 077h, 000h, 000h
        call   sub_1_69D0
        add    esp, 0ch
        db  0C7h, 005h, 094h, 079h, 000h, 000h, 00Ah, 000h, 000h, 000h
        push   4
        db  068h, 094h, 079h, 000h, 000h
        db  068h, 0ECh, 077h, 000h, 000h
        call   sub_1_69D0
        add    esp, 0ch
        db  0C7h, 005h, 068h, 079h, 000h, 000h, 005h, 000h, 000h, 000h
        push   4
        db  068h, 068h, 079h, 000h, 000h
        db  068h, 0F8h, 077h, 000h, 000h
        call   sub_1_69D0
        add    esp, 0ch
        db  0C7h, 005h, 0E8h, 078h, 000h, 000h, 032h, 000h, 000h, 000h
        push   4
        db  068h, 0E8h, 078h, 000h, 000h
        db  068h, 004h, 078h, 000h, 000h
        call   sub_1_69D0
        add    esp, 0ch
        db  0C7h, 005h, 03Ch, 079h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0C7h, 005h, 098h, 078h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 091h, 079h, 000h, 000h, 064h, 000h
        sub    eax, eax
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_6C90:
        push   esi
        db  08Bh, 015h, 084h, 079h, 000h, 000h
        add    edx, 19h
        mov    eax, edx
        shl    edx, 4
        add    edx, eax
        db  0A1h, 09Ch, 078h, 000h, 000h
        shl    eax, 2
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        lea    edx, [eax + edx*4]
        db  0A1h, 010h, 079h, 000h, 000h
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        lea    edx, [edx + eax*4]
        db  0A1h, 004h, 079h, 000h, 000h
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        lea    edx, [edx + eax*4]
        db  080h, 03Dh, 060h, 079h, 000h, 000h, 000h
        jne    SHORT loc_1_6CED
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 08Ch, 078h, 000h, 000h
        db  00Fh, 0AFh, 00Dh, 060h, 078h, 000h, 000h
        add    edx, ecx
loc_1_6CED:
        db  089h, 015h, 01Ch, 078h, 000h, 000h
        push   0
        push   edx
        call   sub_1_01DD
        add    esp, 8
        db  0A3h, 0E0h, 078h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_1_6D10
        mov    eax, 0ffh
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_6D10:
        db  0A3h, 0E0h, 078h, 000h, 000h
        mov    esi, eax
        push   0ffh
        push   100h
        push   esi
        call   sub_1_0192
        add    esp, 0ch
        db  089h, 035h, 050h, 078h, 000h, 000h
        add    esi, 100h
        db  089h, 035h, 07Ch, 079h, 000h, 000h
        push   0
        push   400h
        push   esi
        call   sub_1_0192
        add    esp, 0ch
        add    esi, 400h
        db  080h, 03Dh, 060h, 079h, 000h, 000h, 000h
        jne    SHORT loc_1_6D73
        db  089h, 035h, 034h, 079h, 000h, 000h
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 08Ch, 078h, 000h, 000h
        db  00Fh, 0AFh, 00Dh, 060h, 078h, 000h, 000h
        add    esi, ecx
loc_1_6D73:
        db  089h, 035h, 0F4h, 078h, 000h, 000h
        db  0A1h, 084h, 079h, 000h, 000h
        mov    ecx, eax
        shl    eax, 4
        add    eax, ecx
        lea    esi, [esi + eax*4]
        db  0A1h, 09Ch, 078h, 000h, 000h
        shl    eax, 2
        db  089h, 035h, 008h, 079h, 000h, 000h
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        add    esi, eax
        db  089h, 035h, 054h, 079h, 000h, 000h
        db  0A1h, 010h, 079h, 000h, 000h
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        lea    esi, [esi + eax*4]
        db  089h, 035h, 040h, 079h, 000h, 000h
        db  0A1h, 004h, 079h, 000h, 000h
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*8]
        lea    eax, [esi + eax*4]
        db  0A3h, 064h, 079h, 000h, 000h
        call   sub_1_6E00
        call   sub_1_6E60
        db  080h, 03Dh, 060h, 079h, 000h, 000h, 000h
        jne    SHORT loc_1_6DE7
        call   sub_1_6F30
loc_1_6DE7:
        call   sub_1_6F00
        call   sub_1_6F90
        sub    eax, eax
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_6E00:
        push   esi
        push   0
        db  0A1h, 084h, 079h, 000h, 000h
        mov    ecx, eax
        shl    eax, 4
        add    eax, ecx
        shl    eax, 2
        push   eax
        db  0A1h, 0F4h, 078h, 000h, 000h
        push   eax
        call   sub_1_0192
        add    esp, 0ch
        sub    esi, esi
        db  039h, 035h, 084h, 079h, 000h, 000h
        db  08Bh, 015h, 0F4h, 078h, 000h, 000h
        jbe    SHORT loc_1_6E45
        lea    eax, [edx + 10h]
loc_1_6E34:
        lea    ecx, [eax + 34h]
        mov    dword ptr [eax], ecx
        add    eax, 44h
        inc    esi
        db  039h, 035h, 084h, 079h, 000h, 000h
        ja     SHORT loc_1_6E34
loc_1_6E45:
        mov    eax, esi
        shl    eax, 4
        add    eax, esi
        mov    dword ptr [edx + eax*4 - 34h], 0
        db  0C7h, 005h, 080h, 078h, 000h, 000h, 000h, 000h, 000h, 000h
        pop    esi
        ret
sub_1_6E60:
        db  0A1h, 09Ch, 078h, 000h, 000h
        shl    eax, 2
        push   0
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        push   eax
        db  0A1h, 008h, 079h, 000h, 000h
        push   eax
        call   sub_1_0192
        db  0C7h, 005h, 0C0h, 078h, 000h, 000h, 0C0h, 078h, 000h, 000h
        db  0C7h, 005h, 0C4h, 078h, 000h, 000h, 0C0h, 078h, 000h, 000h
        db  0C7h, 005h, 0ECh, 078h, 000h, 000h, 0ECh, 078h, 000h, 000h
        db  0C7h, 005h, 0F0h, 078h, 000h, 000h, 0ECh, 078h, 000h, 000h
        add    esp, 0ch
        db  0C7h, 005h, 0CCh, 078h, 000h, 000h, 0CCh, 078h, 000h, 000h
        db  0C7h, 005h, 0D0h, 078h, 000h, 000h, 0CCh, 078h, 000h, 000h
        db  08Bh, 00Dh, 09Ch, 078h, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_1_6EF1
        db  08Bh, 015h, 008h, 079h, 000h, 000h
        add    edx, 20h
        mov    edi, edi
loc_1_6ED0:
        db  0C7h, 002h, 0CCh, 078h, 000h, 000h
        db  0A1h, 0D0h, 078h, 000h, 000h
        mov    dword ptr [edx + 4], eax
        db  0A1h, 0D0h, 078h, 000h, 000h
        mov    dword ptr [eax], edx
        db  089h, 015h, 0D0h, 078h, 000h, 000h
        add    edx, 3ch
        dec    ecx
        jne    SHORT loc_1_6ED0
loc_1_6EF1:
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_6F00:
        push   esi
        db  0A1h, 064h, 079h, 000h, 000h
        mov    ecx, eax
loc_1_6F08:
        lea    edx, [ecx + 0eh]
        mov    dword ptr [ecx], edx
        mov    ecx, edx
        mov    esi, eax
        sub    esi, edx
        cmp    esi, 0fffffe5ch
        jne    SHORT loc_1_6F08
        mov    dword ptr [ecx - 0eh], 0
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_6F30:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 03Dh, 034h, 079h, 000h, 000h
        db  08Bh, 035h, 060h, 078h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_6F72
        xor    ebx, ebx
        db  066h, 08Bh, 01Dh, 08Ch, 078h, 000h, 000h
        db  066h, 08Bh, 015h, 08Ch, 078h, 000h, 000h
        sub    dx, 8
        mov    ecx, esi
        sub    ebp, ebp
        lea    esp, [esp]
loc_1_6F60:
        lea    eax, [edi + ebx]
        mov    dword ptr [edi], eax
        mov    word ptr [edi + 4], dx
        mov    word ptr [edi + 6], bp
        mov    edi, eax
        dec    ecx
        jne    SHORT loc_1_6F60
loc_1_6F72:
        xor    eax, eax
        db  066h, 0A1h, 08Ch, 078h, 000h, 000h
        pop    ebp
        sub    edi, eax
        mov    dword ptr [edi], 0
        db  089h, 035h, 0C8h, 078h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_1_6F90:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 035h, 054h, 079h, 000h, 000h
        db  089h, 035h, 08Ch, 079h, 000h, 000h
        db  0A1h, 040h, 079h, 000h, 000h
        db  0A3h, 088h, 079h, 000h, 000h
        db  08Bh, 015h, 010h, 079h, 000h, 000h
        db  003h, 015h, 004h, 079h, 000h, 000h
        or     edx, edx
        jle    SHORT loc_1_7001
        mov    edi, 1
        mov    ebp, 5504h
        sub    ebx, ebx
loc_1_6FC6:
        mov    word ptr [esi + 22h], di
        lea    ecx, [esi + 4ch]
        mov    dword ptr [esi + 24h], ecx
        mov    word ptr [esi + 5ch], bp
        mov    word ptr [esi + 28h], 58h
        mov    word ptr [esi + 0ah], bp
        lea    ecx, [esi + 0a4h]
        mov    dword ptr [esi + 4], ebx
        mov    byte ptr [esi + 8], bl
        mov    word ptr [esi + 3ch], bx
        mov    dword ptr [esi + 48h], ebx
        cmp    eax, ecx
        mov    dword ptr [esi + 44h], ebx
        mov    dword ptr [esi], ecx
        jne    SHORT loc_1_6FFC
        mov    dword ptr [esi], ebx
loc_1_6FFC:
        mov    esi, ecx
        dec    edx
        jne    SHORT loc_1_6FC6
loc_1_7001:
        mov    dword ptr [esi - 0a4h], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_7010:
        push   esi
        push   edi
        call   sub_1_70E0
        call   sub_1_0258
        db  081h, 03Dh, 090h, 078h, 000h, 000h, 090h, 078h, 000h, 000h
        je     SHORT loc_1_7067
        db  0BFh, 090h, 078h, 000h, 000h
        db  039h, 03Dh, 090h, 078h, 000h, 000h
        mov    esi, 0
        je     SHORT loc_1_7051
loc_1_703A:
        db  08Bh, 035h, 090h, 078h, 000h, 000h
        sub    esi, 34h
        db  0A1h, 090h, 078h, 000h, 000h
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_7051:
        test   byte ptr [esi], 80h
        je     SHORT loc_1_705F
        push   esi
        call   sub_1_0151
        add    esp, 4
loc_1_705F:
        db  039h, 03Dh, 090h, 078h, 000h, 000h
        jne    SHORT loc_1_703A
loc_1_7067:
        xor    eax, eax
        db  0A0h, 068h, 078h, 000h, 000h
        push   eax
        call   sub_1_0072
        add    esp, 4
        pop    edi
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_7080:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    bl, byte ptr [esp + 14h]
loc_1_708B:
        mov    eax, dword ptr [esi]
        cmp    esi, eax
        mov    edi, 0
        je     SHORT loc_1_70A2
        lea    edi, [eax - 34h]
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_70A2:
        or     edi, edi
        je     SHORT loc_1_70D3
        mov    byte ptr [edi + 1], bl
        mov    byte ptr [edi + 32h], 0
        test   byte ptr [edi], 80h
        je     SHORT loc_1_708B
        db  0C7h, 047h, 034h, 090h, 078h, 000h, 000h
        add    edi, 34h
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [edi + 4], eax
        db  0A1h, 094h, 078h, 000h, 000h
        mov    dword ptr [eax], edi
        db  089h, 03Dh, 094h, 078h, 000h, 000h
        jmp    SHORT loc_1_708B
loc_1_70D3:
        cmp    edi, 1
        sbb    eax, eax
        inc    eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_1_70E0:
        push   ebx
        push   esi
        push   edi
        db  0A1h, 0B4h, 078h, 000h, 000h
        and    eax, 0ffff7fffh
        db  0A3h, 0B4h, 078h, 000h, 000h
        call   sub_1_010A
        db  083h, 025h, 0B4h, 078h, 000h, 000h, 0F7h
        sub    edi, edi
        db  08Bh, 035h, 088h, 079h, 000h, 000h
        db  039h, 03Dh, 004h, 079h, 000h, 000h
        jbe    SHORT loc_1_715D
        sub    ebx, ebx
loc_1_7110:
        cmp    byte ptr [esi + 8], bl
        je     SHORT loc_1_714E
        push   esi
        call   sub_1_02F1
        add    esp, 4
        cmp    eax, ebx
        db  08Bh, 015h, 04Ch, 079h, 000h, 000h
        jne    SHORT loc_1_7147
        db  0C7h, 046h, 044h, 058h, 078h, 000h, 000h
        lea    ecx, [esi + 44h]
        db  0A1h, 05Ch, 078h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        db  0A1h, 05Ch, 078h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  089h, 00Dh, 05Ch, 078h, 000h, 000h
loc_1_7147:
        dec    edx
        db  089h, 015h, 04Ch, 079h, 000h, 000h
loc_1_714E:
        add    esi, 0a4h
        inc    edi
        db  03Bh, 03Dh, 004h, 079h, 000h, 000h
        jb     SHORT loc_1_7110
loc_1_715D:
        db  083h, 025h, 0B4h, 078h, 000h, 000h, 0EFh
        push   0bh
        db  068h, 06Ch, 079h, 000h, 000h
        call   sub_1_7080
        add    esp, 8
        push   0bh
        db  068h, 018h, 079h, 000h, 000h
        call   sub_1_7080
        add    esp, 8
        push   0bh
        db  068h, 074h, 078h, 000h, 000h
        call   sub_1_7080
        add    esp, 8
        push   0bh
        db  068h, 048h, 078h, 000h, 000h
        call   sub_1_7080
        add    esp, 8
        sub    ebx, ebx
        db  039h, 01Dh, 084h, 079h, 000h, 000h
        jbe    SHORT loc_1_7224
        sub    edi, edi
        lea    esp, [esp]
loc_1_71B0:
        db  08Bh, 035h, 0F4h, 078h, 000h, 000h
        add    esi, edi
        test   byte ptr [esi + 14h], 40h
        je     SHORT loc_1_7218
        cmp    dword ptr [esi + 38h], 0
        je     SHORT loc_1_71D8
loc_1_71C4:
        push   18h
        mov    eax, dword ptr [esi + 38h]
        push   eax
        call   sub_1_07A0
        add    esp, 8
        cmp    dword ptr [esi + 38h], 0
        jne    SHORT loc_1_71C4
loc_1_71D8:
        test   byte ptr [esi + 14h], 40h
        je     SHORT loc_1_7218
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 15h]
        db  003h, 00Dh, 050h, 078h, 000h, 000h
        xor    eax, eax
        mov    al, byte ptr [ecx]
        db  039h, 005h, 084h, 079h, 000h, 000h
        jbe    SHORT loc_1_720B
        mov    edx, eax
        shl    edx, 4
        add    edx, eax
        db  0A1h, 0F4h, 078h, 000h, 000h
        lea    edx, [eax + edx*4]
        cmp    edx, esi
        jne    SHORT loc_1_720B
        mov    byte ptr [ecx], 0ffh
loc_1_720B:
        push   esi
        call   sub_1_3760
        add    esp, 4
        mov    byte ptr [esi + 14h], 0
loc_1_7218:
        add    edi, 44h
        inc    ebx
        db  03Bh, 01Dh, 084h, 079h, 000h, 000h
        jb     SHORT loc_1_71B0
loc_1_7224:
        db  0BFh, 0C0h, 078h, 000h, 000h
        sub    ebx, ebx
        add    eax, 0
loc_1_7230:
        db  039h, 03Dh, 0C0h, 078h, 000h, 000h
        mov    esi, ebx
        je     SHORT loc_1_7251
        db  08Bh, 035h, 0C0h, 078h, 000h, 000h
        sub    esi, 20h
        db  0A1h, 0C0h, 078h, 000h, 000h
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_7251:
        cmp    esi, ebx
        je     SHORT loc_1_7287
        push   0bh
        lea    eax, [esi + 34h]
        push   eax
        call   sub_1_7080
        add    esp, 8
        mov    byte ptr [esi + 28h], bl
        db  0C7h, 046h, 020h, 0CCh, 078h, 000h, 000h
        add    esi, 20h
        db  0A1h, 0D0h, 078h, 000h, 000h
        mov    dword ptr [esi + 4], eax
        db  0A1h, 0D0h, 078h, 000h, 000h
        mov    dword ptr [eax], esi
        db  089h, 035h, 0D0h, 078h, 000h, 000h
        jmp    SHORT loc_1_7230
loc_1_7287:
        db  0BFh, 0ECh, 078h, 000h, 000h
        sub    ebx, ebx
        mov    edi, edi
loc_1_7290:
        db  039h, 03Dh, 0ECh, 078h, 000h, 000h
        mov    esi, ebx
        je     SHORT loc_1_72B1
        db  08Bh, 035h, 0ECh, 078h, 000h, 000h
        sub    esi, 20h
        db  0A1h, 0ECh, 078h, 000h, 000h
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_72B1:
        cmp    esi, ebx
        push   0bh
        je     SHORT loc_1_72E7
        lea    eax, [esi + 34h]
        push   eax
        call   sub_1_7080
        add    esp, 8
        mov    byte ptr [esi + 28h], bl
        db  0C7h, 046h, 020h, 0CCh, 078h, 000h, 000h
        add    esi, 20h
        db  0A1h, 0D0h, 078h, 000h, 000h
        mov    dword ptr [esi + 4], eax
        db  0A1h, 0D0h, 078h, 000h, 000h
        mov    dword ptr [eax], esi
        db  089h, 035h, 0D0h, 078h, 000h, 000h
        jmp    SHORT loc_1_7290
loc_1_72E7:
        db  068h, 084h, 078h, 000h, 000h
        call   sub_1_7080
        add    esp, 8
        db  0BFh, 02Ch, 079h, 000h, 000h
        lea    esp, [esp]
loc_1_7300:
        db  039h, 03Dh, 02Ch, 079h, 000h, 000h
        mov    esi, 0
        je     SHORT loc_1_7324
        db  08Bh, 035h, 02Ch, 079h, 000h, 000h
        sub    esi, 44h
        db  0A1h, 02Ch, 079h, 000h, 000h
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_7324:
        or     esi, esi
        je     SHORT loc_1_7333
        push   esi
        call   sub_1_39E0
        add    esp, 4
        jmp    SHORT loc_1_7300
loc_1_7333:
        db  0BEh, 058h, 078h, 000h, 000h
loc_1_7338:
        db  039h, 035h, 058h, 078h, 000h, 000h
        mov    edi, 0
        je     SHORT loc_1_735C
        db  08Bh, 03Dh, 058h, 078h, 000h, 000h
        sub    edi, 44h
        db  0A1h, 058h, 078h, 000h, 000h
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_735C:
        or     edi, edi
        je     SHORT loc_1_7370
        push   edi
        call   sub_1_39E0
        add    esp, 4
        jmp    SHORT loc_1_7338
        db  005h, 000h, 000h, 000h, 000h
loc_1_7370:
        db  0BEh, 0A0h, 078h, 000h, 000h
loc_1_7375:
        db  039h, 035h, 0A0h, 078h, 000h, 000h
        mov    edi, 0
        je     SHORT loc_1_7399
        db  08Bh, 03Dh, 0A0h, 078h, 000h, 000h
        sub    edi, 44h
        db  0A1h, 0A0h, 078h, 000h, 000h
        push   eax
        call   sub_1_3B50
        add    esp, 4
loc_1_7399:
        or     edi, edi
        je     SHORT loc_1_73A8
        push   edi
        call   sub_1_39E0
        add    esp, 4
        jmp    SHORT loc_1_7375
loc_1_73A8:
        sub    edi, edi
        db  08Bh, 035h, 08Ch, 079h, 000h, 000h
        db  039h, 03Dh, 010h, 079h, 000h, 000h
        jbe    SHORT loc_1_73E4
        sub    ebx, ebx
loc_1_73BA:
        cmp    byte ptr [esi + 8], bl
        je     SHORT loc_1_73D5
        push   esi
        call   sub_1_02F1
        add    esp, 4
        cmp    eax, ebx
        jne    SHORT loc_1_73D5
        push   esi
        call   sub_1_3820
        add    esp, 4
loc_1_73D5:
        add    esi, 0a4h
        inc    edi
        db  03Bh, 03Dh, 010h, 079h, 000h, 000h
        jb     SHORT loc_1_73BA
loc_1_73E4:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_73F0:
        db  0C7h, 005h, 070h, 078h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 0B4h, 078h, 000h, 000h
        test   al, 8
        jne    SHORT loc_1_7410
        call   sub_1_00E9
        db  0A1h, 0B4h, 078h, 000h, 000h
        or     eax, 8
loc_1_7410:
        db  080h, 03Dh, 060h, 079h, 000h, 000h, 000h
        jne    SHORT loc_1_7430
        db  0A3h, 0B4h, 078h, 000h, 000h
        test   al, 10h
        jne    SHORT loc_1_7430
loc_1_7422:
        call   sub_1_39A0
        or     eax, eax
        db  0A1h, 0B4h, 078h, 000h, 000h
        jne    SHORT loc_1_7422
loc_1_7430:
        db  0A3h, 0B4h, 078h, 000h, 000h
        db  083h, 03Dh, 070h, 078h, 000h, 000h, 000h
        jne    SHORT loc_1_7446
        or     eax, 10h
        db  0A3h, 0B4h, 078h, 000h, 000h
loc_1_7446:
        call   sub_1_3560
        db  083h, 03Dh, 070h, 078h, 000h, 000h, 000h
        jne    SHORT loc_1_7463
        db  0A1h, 0B4h, 078h, 000h, 000h
        or     eax, 8000h
        db  0A3h, 0B4h, 078h, 000h, 000h
loc_1_7463:
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_7470:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 3
        je     SHORT loc_1_74E1
        cmp    eax, 9
        je     SHORT loc_1_74E1
        cmp    eax, 0ah
        je     SHORT loc_1_74E1
        cmp    eax, 0dh
        je     SHORT loc_1_7490
        cmp    eax, 0eh
        je     SHORT loc_1_74A1
        sub    eax, eax
        ret
loc_1_7490:
        db  0F6h, 005h, 0B5h, 078h, 000h, 000h, 080h
        je     SHORT loc_1_74E1
        call   sub_1_7010
        sub    eax, eax
        ret
loc_1_74A1:
        db  08Bh, 00Dh, 058h, 077h, 000h, 000h
        cmp    ecx, 1
        jne    SHORT loc_1_74DA
        db  0F6h, 005h, 0B5h, 078h, 000h, 000h, 080h
        je     SHORT loc_1_74C0
        call   sub_1_7010
        db  08Bh, 00Dh, 058h, 077h, 000h, 000h
loc_1_74C0:
        db  0A1h, 0E0h, 078h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_74DA
        push   0
        push   eax
        call   sub_1_01FA
        add    esp, 8
        db  08Bh, 00Dh, 058h, 077h, 000h, 000h
loc_1_74DA:
        dec    ecx
        db  089h, 00Dh, 058h, 077h, 000h, 000h
loc_1_74E1:
        sub    eax, eax
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_74F0:
        call   sub_1_02E2
        or     ax, ax
        je     SHORT loc_1_751B
        sub    ax, 30h
        and    ax, 0fff0h
        db  080h, 03Dh, 060h, 079h, 000h, 000h, 000h
        jne    SHORT loc_1_7515
        cmp    ax, 0c00h
        jbe    SHORT loc_1_7515
        mov    ax, 0c00h
loc_1_7515:
        db  066h, 0A3h, 0E4h, 078h, 000h, 000h
loc_1_751B:
        and    eax, 0ffffh
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_7530:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        sub    esi, esi
        db  0CDh, 020h, 000h, 000h, 033h, 000h
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], si
        je     SHORT loc_1_756C
        push   esi
        push   esi
        db  068h, 070h, 074h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 0A7h, 08Ah, 0FEh, 0FFh
        add    esp, 10h
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_756C
        db  0C7h, 005h, 050h, 077h, 000h, 000h, 001h, 000h, 000h, 000h
loc_1_756C:
        db  0A1h, 058h, 077h, 000h, 000h
        mov    ecx, eax
        inc    eax
        db  0A3h, 058h, 077h, 000h, 000h
        or     ecx, ecx
        jne    SHORT loc_1_7593
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 07Ch, 078h, 000h, 000h
        db  083h, 03Dh, 054h, 077h, 000h, 000h, 000h
        je     SHORT loc_1_7593
        call   sub_1_6670
loc_1_7593:
        db  068h, 020h, 079h, 000h, 000h
        call   sub_1_02CA
        add    esp, 4
        call   sub_1_74F0
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_75B0:
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    eax, dword ptr [ebx]
        add    al, 4
        add    eax, 6050505h
        add    byte ptr [eax], al
        db  000h
loc_1_75C0:
        clc
loc_1_75C1:
        add    byte ptr [eax], al
        db  000h, 00Ch, 079h
        add    byte ptr [eax], al
        add    al, 0
loc_1_75CA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        shl    byte ptr [eax], 0
        add    byte ptr [eax], al
loc_1_75F1:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 080h, 003h, 000h, 000h, 0B0h
        add    eax, dword ptr [eax]
        db  000h, 080h, 004h, 000h, 000h, 000h
        add    byte ptr [eax], al
        db  000h, 050h, 03Dh
        add    byte ptr [eax], al
        add    byte ptr [esi], 0
        adc    byte ptr [edi], al
        db  000h, 020h
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        xor    byte ptr [ecx], ah
        add    byte ptr [eax], al
        pushad
        and    al, byte ptr [eax]
        db  000h, 0E0h
        and    al, byte ptr [eax]
        db  000h, 050h, 023h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        xor    byte ptr [edx], 0
        db  000h, 040h, 033h
        add    byte ptr [eax], al
        jo     SHORT loc_1_7702
        add    byte ptr [eax], al
        shl    byte ptr [eax], 1
        db  000h, 0B0h, 064h, 000h, 000h, 0C0h
        xor    byte ptr [eax], al
        db  000h, 080h, 032h, 000h, 000h, 000h
        push   ecx
        add    byte ptr [eax], al
        rcl    byte ptr [ecx], 0
        db  0A0h, 053h, 000h, 000h, 0D0h
        push   edx
        add    byte ptr [eax], al
        rcl    byte ptr [eax + eax]
        db  070h, 055h
        add    byte ptr [eax], al
        push   eax
        push   esi
        add    byte ptr [eax], al
        mov    al, 56h
loc_1_7702:
        add    byte ptr [eax], al
        and    byte ptr [edi], dl
        db  000h, 0E0h
        push   edi
        add    byte ptr [eax], al
        rcr    byte ptr [eax], 0
        rcr    byte ptr [ecx], 1
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
loc_1_7720:
        rcr    byte ptr [edx], 1
        db  000h, 050h, 05Bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    byte ptr [ebx], bl
        db  000h, 0B0h, 01Dh, 000h, 000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0C0h
        pop    esi
        add    byte ptr [eax], al
        jo     SHORT loc_1_77A6
        add    byte ptr [eax], al
        nop
        pop    ebp
        add    byte ptr [eax], al
loc_1_7750:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7754:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7758:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_775C:
        dec    esp
        inc    ecx
        dec    esi
        inc    ecx
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7768:
        push   edx
        inc    ebp
        inc    ebx
        inc    ebp
        dec    ecx
        push   esi
        inc    ebp
        inc    edx
        push   ebp
        inc    esi
        inc    esi
        inc    ebp
        push   edx
        push   ebx
        add    byte ptr [eax], al
loc_1_7778:
        dec    esi
        inc    ecx
        dec    ebp
        inc    ebp
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [ebx + 45h], dl
        push   ebx
        push   ebx
        dec    ecx
        dec    edi
        dec    esi
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_778C:
        push   ebx
        inc    ebp
        dec    esi
        inc    esp
        inc    edx
        push   ebp
        inc    esi
        inc    esi
        inc    ebp
        push   edx
        push   ebx
        add    byte ptr [ecx + 4eh], cl
        push   esp
        inc    ebp
        push   edx
        dec    esi
        inc    ebp
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_77A4:
        push   eax
        dec    ecx
loc_1_77A6:
        inc    edi
        inc    edi
        pop    ecx
        inc    edx
        inc    ecx
        inc    ebx
        dec    ebx
        add    byte ptr [eax], al
        add    byte ptr [ebp + 53h], cl
        inc    ebp
        pop    eax
        push   esp
        inc    ebp
        dec    esi
        push   ebx
        dec    ecx
        dec    edi
        dec    esi
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_77C0:
        dec    esp
        dec    edi
        dec    edi
        dec    ebx
        inc    ecx
        dec    eax
        inc    ebp
        inc    ecx
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [edx + 52h], al
        dec    edi
        inc    ecx
        inc    esp
        inc    ebx
        inc    ecx
        push   ebx
        push   esp
        inc    ebx
        dec    edi
        push   ebp
        dec    esi
        push   esp
        add    byte ptr [eax], al
loc_1_77DC:
        inc    edx
        push   edx
        dec    edi
        inc    ecx
        inc    esp
        inc    ebx
        inc    ecx
        push   ebx
        push   esp
        inc    esp
        inc    ebp
        dec    esp
        inc    ecx
        pop    ecx
        add    byte ptr [eax], al
loc_1_77EC:
        push   edx
        inc    ebp
        push   esp
        push   edx
        pop    ecx
        inc    ebx
        dec    edi
        push   ebp
        dec    esi
        push   esp
        add    byte ptr [eax], al
loc_1_77F8:
        push   edx
        inc    ebp
        push   esp
        push   edx
        pop    ecx
        inc    esp
        inc    ebp
        dec    esp
        inc    ecx
        pop    ecx
        add    byte ptr [eax], al
loc_1_7804:
        push   esi
        inc    ebp
        push   edx
        dec    ecx
        inc    esi
        pop    ecx
        push   esp
        dec    ecx
        dec    ebp
        inc    ebp
        dec    edi
        push   ebp
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_78B5:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7991:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
_LTEXT ends
        end
