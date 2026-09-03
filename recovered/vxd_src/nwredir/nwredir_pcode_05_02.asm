; nwredir_pcode_05_02.asm -- NWREDIR object 5 segment _PTEXT  (part 2/3)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_5_4B30:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, ecx
        mov    dword ptr [esp + 10h], ecx
        mov    ebx, dword ptr [eax + 1ch]
        mov    esi, edx
        mov    edi, dword ptr [ebx + 10h]
        mov    edx, dword ptr [esp + 2ch]
        mov    dword ptr [esi], 0ffffh
        sub    cl, cl
        mov    ebp, dword ptr [esp + 28h]
        mov    word ptr [edx], 0
        mov    dword ptr [ebp], 0
        test   byte ptr [ebx + 24h], 2
        jne    SHORT loc_5_4B84
        push   ebp
        push   esi
        push   edx
        push   ebx
        push   15h
        push   16h
        mov    eax, dword ptr [esp + 28h]
        db  068h, 004h, 017h, 000h, 000h
        push   eax
        push   edi
        db  0E8h, 000h, 0B7h, 0FAh, 0FFh
        add    esp, 24h
        mov    cl, al
loc_5_4B84:
        or     cl, cl
        jne    NEAR PTR loc_5_4C81
        cmp    dword ptr [esi], 0ffffh
        jne    NEAR PTR loc_5_4C81
        or     word ptr [ebx + 24h], 2
        mov    edx, dword ptr [esp + 2ch]
        push   edx
        lea    eax, [esp + 24h]
        push   10h
        push   eax
        xor    eax, eax
        push   ebp
        push   esi
        mov    al, byte ptr [ebx + 20h]
        push   eax
        push   2ch
        push   16h
        mov    eax, dword ptr [esp + 30h]
        db  068h, 00Ch, 017h, 000h, 000h
        push   eax
        push   edi
        db  0E8h, 0BAh, 0B6h, 0FAh, 0FFh
        add    esp, 2ch
        mov    cl, al
        or     cl, cl
        jne    SHORT loc_5_4C35
        mov    eax, dword ptr [esp + 20h]
        add    dword ptr [ebp], eax
        lea    eax, [esp + 1ch]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [edi + 74h]
        xor    ecx, ecx
        mov    cl, byte ptr [edi + 73h]
        shl    eax, 8
        or     eax, ecx
        push   eax
        push   1ch
        push   17h
        mov    eax, dword ptr [esp + 20h]
        db  068h, 018h, 017h, 000h, 000h
        push   eax
        push   edi
        db  0E8h, 081h, 0B6h, 0FAh, 0FFh
        add    esp, 1ch
        mov    cl, al
        or     cl, cl
        jne    SHORT loc_5_4C81
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        mov    eax, dword ptr [esp + 24h]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebx + 20h]
        push   eax
        push   29h
        push   16h
        mov    eax, dword ptr [esp + 28h]
        db  068h, 020h, 017h, 000h, 000h
        push   eax
        push   edi
        db  0E8h, 04Fh, 0B6h, 0FAh, 0FFh
        add    esp, 24h
        mov    cl, al
loc_5_4C35:
        or     cl, cl
        jne    SHORT loc_5_4C81
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [esp + 18h], eax
        jae    SHORT loc_5_4C59
        mov    edx, dword ptr [esp + 2ch]
        xor    edi, edi
        sub    eax, dword ptr [esp + 18h]
        mov    di, word ptr [edx]
        sub    edx, edx
        div    edi
        shl    eax, 3
        jmp    SHORT loc_5_4C5B
loc_5_4C59:
        sub    eax, eax
loc_5_4C5B:
        cmp    dword ptr [ebp], eax
        jbe    SHORT loc_5_4C63
        mov    dword ptr [ebp], eax
loc_5_4C63:
        mov    edx, dword ptr [esp + 2ch]
        xor    edi, edi
        mov    di, word ptr [edx]
        mov    eax, dword ptr [esp + 14h]
        sub    edx, edx
        div    edi
        shl    eax, 3
        mov    edx, dword ptr [esi]
        cmp    eax, edx
        jb     SHORT loc_5_4C7F
        mov    eax, edx
loc_5_4C7F:
        mov    dword ptr [esi], eax
loc_5_4C81:
        xor    eax, eax
        mov    al, cl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret    8
        db  090h
loc_5_4C90:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 1ch]
        lea    eax, [esp + 0eh]
        mov    dword ptr [esi + 24h], 0
        mov    dword ptr [esi + 28h], 0
        mov    dword ptr [esi + 14h], 0
        mov    ecx, esi
        push   eax
        mov    dword ptr [esi], 200h
        lea    eax, [esp + 18h]
        lea    edx, [esp + 14h]
        push   eax
        call   sub_5_4B30
        mov    bl, al
        or     bl, bl
        jne    NEAR PTR loc_5_4DF1
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [esp + 14h], eax
        jbe    SHORT loc_5_4CE6
        mov    dword ptr [esp + 14h], 0
loc_5_4CE6:
        db  080h, 03Dh, 06Fh, 004h, 000h, 000h, 000h
        je     SHORT loc_5_4D3B
        xor    ecx, ecx
        mov    cx, word ptr [esp + 0eh]
        mov    eax, 77359400h
        cdq
        idiv   ecx
        sub    edx, edx
        div    dword ptr [esi]
        mov    edi, eax
        cmp    edi, dword ptr [esp + 10h]
        jb     SHORT loc_5_4D10
        cmp    edi, dword ptr [esp + 14h]
        jae    SHORT loc_5_4D3B
loc_5_4D10:
        db  0E8h, 066h, 0BCh, 0FAh, 0FFh
        test   dword ptr [eax], 180h
        jne    SHORT loc_5_4D3B
        mov    eax, edi
        mov    ecx, dword ptr [esp + 10h]
        cmp    eax, ecx
        jb     SHORT loc_5_4D29
        mov    eax, ecx
loc_5_4D29:
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [esp + 14h]
        cmp    edi, eax
        jb     SHORT loc_5_4D37
        mov    edi, eax
loc_5_4D37:
        mov    dword ptr [esp + 14h], edi
loc_5_4D3B:
        cmp    dword ptr [esp + 10h], 10000h
        jb     SHORT loc_5_4D8E
        mov    eax, 8000h
        mov    ecx, 10000h
        nop
loc_5_4D50:
        cmp    word ptr [esp + 0eh], ax
        jae    SHORT loc_5_4D6A
        shr    dword ptr [esp + 10h], 1
        shr    dword ptr [esp + 14h], 1
        shl    word ptr [esp + 0eh], 1
        cmp    dword ptr [esp + 10h], ecx
        jae    SHORT loc_5_4D50
loc_5_4D6A:
        cmp    dword ptr [esp + 10h], 10000h
        jb     SHORT loc_5_4D8E
        mov    dword ptr [esp + 10h], 0ffffh
        cmp    dword ptr [esp + 14h], 10000h
        jb     SHORT loc_5_4D8E
        mov    dword ptr [esp + 14h], 0ffffh
loc_5_4D8E:
        xor    ecx, ecx
        mov    cx, word ptr [esp + 0eh]
        mov    eax, 77359400h
        cdq
        idiv   ecx
        sub    edx, edx
        div    dword ptr [esi]
        mov    edi, eax
        cmp    edi, dword ptr [esp + 10h]
        jb     SHORT loc_5_4DAF
        cmp    edi, dword ptr [esp + 14h]
        jae    SHORT loc_5_4DDA
loc_5_4DAF:
        db  0E8h, 0C7h, 0BBh, 0FAh, 0FFh
        test   dword ptr [eax], 180h
        jne    SHORT loc_5_4DDA
        mov    eax, edi
        mov    ecx, dword ptr [esp + 10h]
        cmp    eax, ecx
        jb     SHORT loc_5_4DC8
        mov    eax, ecx
loc_5_4DC8:
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [esp + 14h]
        cmp    edi, eax
        jb     SHORT loc_5_4DD6
        mov    edi, eax
loc_5_4DD6:
        mov    dword ptr [esp + 14h], edi
loc_5_4DDA:
        mov    ax, word ptr [esp + 0eh]
        mov    word ptr [esi + 18h], ax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi + 24h], eax
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esi + 28h], eax
loc_5_4DF1:
        mov    ecx, ebx
        db  0E8h, 058h, 0EDh, 0FAh, 0FFh
        mov    word ptr [esi + 1ah], ax
        movsx  eax, ax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_4E10:
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 48h]
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [esp + 14h], eax
        mov    esi, dword ptr [eax + 10h]
        mov    ebp, dword ptr [ebx + 0ch]
        mov    edi, dword ptr [ebx + 10h]
        mov    ecx, dword ptr [ebx + 14h]
        mov    eax, dword ptr [ebx + 24h]
        and    eax, 24000000h
        or     dword ptr [ebx], eax
        db  080h, 03Dh, 068h, 004h, 000h, 000h, 000h
        je     SHORT loc_5_4E7A
        or     ecx, ecx
        je     SHORT loc_5_4E7A
        test   byte ptr [ecx], 1
        je     SHORT loc_5_4E70
        mov    eax, dword ptr [ecx + 4]
        push   1
        movsx  ecx, word ptr [eax]
        sub    ecx, 2
        push   ecx
        push   ecx
        add    eax, 2
        push   eax
        lea    eax, [esp + 44h]
        push   eax
        db  0E8h, 055h, 0BCh, 0FAh, 0FFh
        add    esp, 14h
        mov    byte ptr [esp + eax + 34h], 0
        jmp    SHORT loc_5_4E7A
        db  090h
loc_5_4E70:
        mov    byte ptr [esp + 34h], 2dh
        mov    byte ptr [esp + 35h], 0
loc_5_4E7A:
        mov    ecx, ebx
        db  0E8h, 06Fh, 0EDh, 0FAh, 0FFh
        mov    byte ptr [esp + 13h], al
        cmp    al, 80h
        jne    SHORT loc_5_4EA0
        mov    word ptr [ebx + 1ah], 5
        mov    eax, 5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  08Dh, 064h, 024h, 000h
loc_5_4EA0:
        test   byte ptr [ebx], 8
        je     SHORT loc_5_4EB8
        mov    word ptr [ebx + 1ah], 5
        mov    eax, 5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
loc_5_4EB8:
        lea    eax, [ebp + 4]
        mov    dword ptr [esp + 20h], eax
        xor    eax, eax
        mov    ax, word ptr [ebp]
        add    eax, ebp
        mov    dword ptr [esp + 24h], eax
        lea    eax, [edi + 4]
        mov    dword ptr [esp + 28h], eax
        sub    edx, edx
        mov    ecx, ebx
        call   sub_5_A4A0
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_5_4F20
        inc    dword ptr [ebp + 2ch]
loc_5_4EE4:
        mov    eax, dword ptr [ebp + 48h]
        or     eax, eax
        jne    SHORT loc_5_4EF5
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_5_4F20
        or     eax, eax
        je     SHORT loc_5_4F04
loc_5_4EF5:
        inc    dword ptr [ebp + 58h]
        push   ebp
        db  0E8h, 0E6h, 0BBh, 0FAh, 0FFh
        add    esp, 4
        dec    dword ptr [ebp + 58h]
loc_5_4F04:
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_5_4EE4
        inc    byte ptr [esi + 37h]
        push   esi
        db  0E8h, 0D1h, 0BBh, 0FAh, 0FFh
        add    esp, 4
        dec    byte ptr [esi + 37h]
        jmp    SHORT loc_5_4EE4
        db  005h, 000h, 000h, 000h, 000h
loc_5_4F20:
        cmp    byte ptr [esp + 13h], 0ffh
        je     NEAR PTR loc_5_5034
        mov    byte ptr [esp + 12h], 0
        cmp    byte ptr [esp + 13h], 4
        jne    SHORT loc_5_4F5E
        test   byte ptr [ebx + 27h], 24h
        jne    SHORT loc_5_4F5E
        xor    eax, eax
        mov    ax, word ptr [edi]
        add    eax, edi
        push   eax
        mov    eax, dword ptr [esp + 2ch]
        push   eax
        mov    edx, ebx
        mov    ecx, dword ptr [esp + 1ch]
        db  0E8h, 0ABh, 0ECh, 0FAh, 0FFh
        or     eax, eax
        je     SHORT loc_5_4F5E
        mov    byte ptr [esp + 12h], 5
loc_5_4F5E:
        cmp    byte ptr [esp + 12h], 0
        jne    NEAR PTR loc_5_50B2
        xor    eax, eax
        mov    ax, word ptr [edi + 2]
        add    eax, edi
        mov    dword ptr [esp + 18h], eax
        db  0F6h, 005h, 065h, 004h, 000h, 000h, 001h
        je     SHORT loc_5_4F97
        cmp    byte ptr [esp + 13h], 4
        jne    SHORT loc_5_4F97
        mov    ecx, dword ptr [ebx + 28h]
        sub    ecx, 2
        mov    dword ptr [esp + 1ch], ecx
        mov    ax, word ptr [eax]
        mov    word ptr [ecx], ax
        jmp    SHORT loc_5_4F9F
loc_5_4F97:
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [esp + 1ch], ecx
loc_5_4F9F:
        mov    ecx, dword ptr [esp + 1ch]
        xor    eax, eax
        mov    ax, word ptr [ecx]
        add    eax, ecx
        mov    dword ptr [esp + 30h], eax
        xor    eax, eax
        mov    al, byte ptr [esp + 13h]
        mov    di, 6
        mov    dword ptr [esp + 2ch], eax
        lea    esp, [esp]
loc_5_4FC0:
        mov    eax, dword ptr [esp + 14h]
        xor    edx, edx
        mov    dl, byte ptr [eax + 20h]
        mov    eax, dword ptr [esp + 30h]
        push   11h
        mov    ecx, dword ptr [esp + 20h]
        push   eax
        push   ecx
        mov    eax, dword ptr [esp + 24h]
        push   11h
        push   eax
        mov    eax, dword ptr [esp + 3ch]
        push   eax
        mov    eax, dword ptr [esp + 3ch]
        push   0ah
        push   eax
        mov    eax, dword ptr [esp + 40h]
        push   eax
        mov    eax, dword ptr [esp + 38h]
        push   4
        push   eax
        push   edx
        push   4
        push   eax
        mov    eax, edi
        push   edx
        and    eax, 0ffffh
        and    eax, dword ptr [ebx]
        push   eax
        mov    eax, dword ptr [esp + 6ch]
        push   0
        push   eax
        push   4
        db  068h, 028h, 017h, 000h, 000h
        push   ebx
        push   esi
        db  0E8h, 067h, 0B2h, 0FAh, 0FFh
        mov    byte ptr [esp + 6ah], al
        add    esp, 58h
        test   di, 10h
        jne    NEAR PTR loc_5_50B2
        or     di, 10h
        or     al, al
        jne    SHORT loc_5_4FC0
        jmp    SHORT loc_5_50B2
loc_5_5034:
        xor    eax, eax
        mov    ax, word ptr [edi]
        lea    ecx, [edi + eax]
        mov    dword ptr [esp + 18h], ecx
        mov    eax, dword ptr [esp + 28h]
        push   ecx
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 30h]
        push   eax
        mov    eax, dword ptr [esp + 30h]
        push   eax
        mov    eax, dword ptr [ebx]
        and    eax, 6
        push   eax
        mov    eax, dword ptr [esp + 2ch]
        push   eax
        push   45h
        db  068h, 038h, 017h, 000h, 000h
        push   ebx
        push   esi
        db  0E8h, 011h, 0B2h, 0FAh, 0FFh
        mov    byte ptr [esp + 3eh], al
        add    esp, 2ch
        cmp    al, 0ffh
        jne    SHORT loc_5_50B2
        test   byte ptr [ebx], 10h
        je     SHORT loc_5_50B2
        xor    eax, eax
        mov    ax, word ptr [edi + 2]
        add    eax, edi
        mov    ecx, dword ptr [esp + 18h]
        push   ecx
        push   eax
        mov    eax, dword ptr [esp + 2ch]
        push   eax
        mov    eax, dword ptr [esp + 2ch]
        push   eax
        mov    eax, dword ptr [esp + 24h]
        push   eax
        push   0fh
        push   16h
        db  068h, 040h, 017h, 000h, 000h
        push   ebx
        push   esi
        db  0E8h, 0D4h, 0B1h, 0FAh, 0FFh
        mov    byte ptr [esp + 3ah], al
        add    esp, 28h
loc_5_50B2:
        or     ebp, ebp
        je     SHORT loc_5_511A
        dec    dword ptr [ebp + 2ch]
        cmp    byte ptr [esp + 12h], 0
        jne    SHORT loc_5_511A
        mov    eax, dword ptr [ebp + 78h]
        push   eax
        db  0E8h, 015h, 0BAh, 0FAh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [ebx + 10h]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        add    eax, 2
        push   eax
        db  0E8h, 0F8h, 0B9h, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [ebp + 78h], eax
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_5_50F5
        mov    ecx, ebp
        call   sub_5_A7F0
        jmp    SHORT loc_5_511A
loc_5_50F5:
        mov    esi, dword ptr [ebx + 10h]
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    eax, 2
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [ebx + 10h]
        call   sub_5_AA10
        mov    word ptr [ebp + 6ah], ax
loc_5_511A:
        mov    ecx, dword ptr [esp + 12h]
        db  0E8h, 02Dh, 0EAh, 0FAh, 0FFh
        mov    word ptr [ebx + 1ah], ax
        movsx  eax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_5140:
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 40h]
        mov    ecx, edi
        mov    eax, dword ptr [edi + 1ch]
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [esp + 18h], eax
        db  0E8h, 090h, 0EAh, 0FAh, 0FFh
        mov    byte ptr [esp + 13h], al
        cmp    al, 80h
        jne    SHORT loc_5_5180
        mov    word ptr [edi + 1ah], 5
        mov    eax, 5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 2ch
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_5_5180:
        mov    edx, dword ptr [esp + 1ch]
        db  068h, 0B0h, 0A7h, 000h, 000h
        mov    ecx, 3
        call   sub_5_2DC0
        push   2bh
        mov    eax, dword ptr [edi + 14h]
        push   eax
        db  0E8h, 0B2h, 0B8h, 0FAh, 0FFh
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_5_51C0
        mov    word ptr [edi + 1ah], 8
        mov    eax, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 2ch
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_51C0:
        cmp    byte ptr [edi + 4], 0
        jne    NEAR PTR loc_5_5470
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [esp + 20h], ecx
        mov    eax, ecx
        add    eax, 4
        mov    dword ptr [esp + 24h], eax
        cmp    word ptr [eax], 0
        jne    SHORT loc_5_5200
        push   2bh
        push   esi
        db  0E8h, 0A5h, 0B8h, 0FAh, 0FFh
        add    esp, 8
        mov    word ptr [edi + 1ah], 12h
        mov    eax, 12h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 2ch
        ret
        db  08Bh, 0FFh
loc_5_5200:
        mov    ecx, dword ptr [esp + 20h]
        xor    eax, eax
        mov    ax, word ptr [ecx + 2]
        mov    byte ptr [esp + 11h], 1
        add    ecx, eax
        mov    dword ptr [esp + 28h], ecx
        mov    ebx, ecx
        add    ebx, 2
        sub    ebp, ebp
        lea    esp, [esp]
loc_5_5220:
        cmp    word ptr [ebx], 2eh
        jne    SHORT loc_5_522E
        cmp    ebp, 8
        jne    SHORT loc_5_522E
        add    ebx, 2
loc_5_522E:
        mov    ax, word ptr [ebx]
        cmp    ax, 2eh
        je     SHORT loc_5_5241
        or     ax, ax
        je     SHORT loc_5_5241
        add    ebx, 2
        jmp    SHORT loc_5_5245
loc_5_5241:
        mov    ax, 20h
loc_5_5245:
        cmp    ax, 3fh
        je     SHORT loc_5_525C
        cmp    ax, 2ah
        je     SHORT loc_5_525C
        cmp    ax, 20h
        je     SHORT loc_5_525C
        mov    byte ptr [esp + 11h], 0
loc_5_525C:
        push   eax
        db  0E8h, 064h, 0B8h, 0FAh, 0FFh
        add    esp, 4
        cmp    ax, 100h
        jb     SHORT loc_5_5275
        cmp    ebp, 0ah
        jae    SHORT loc_5_5275
        mov    byte ptr [esi + ebp + 1], ah
        inc    ebp
loc_5_5275:
        mov    byte ptr [esi + ebp + 1], al
        inc    ebp
        cmp    ebp, 0bh
        jb     SHORT loc_5_5220
        test   byte ptr [edi + 3], 24h
        je     NEAR PTR loc_5_5320
        lea    eax, [esp + 12h]
        push   eax
        mov    ecx, dword ptr [esp + 2ch]
        push   0
        mov    eax, dword ptr [esp + 2ch]
        push   ecx
        push   eax
        mov    eax, dword ptr [esp + 2ch]
        push   4
        push   eax
        push   1
        push   1
        mov    al, byte ptr [eax + 20h]
        and    eax, 0ffh
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esp + 37h]
        push   eax
        push   0ch
        db  068h, 048h, 017h, 000h, 000h
        mov    eax, dword ptr [esp + 48h]
        push   edi
        push   eax
        db  0E8h, 0B8h, 0AFh, 0FAh, 0FFh
        add    esp, 38h
        mov    bl, al
        or     bl, bl
        jne    NEAR PTR loc_5_5356
        lea    eax, [esi + 0fh]
        push   eax
        lea    eax, [esi + 0dh]
        push   eax
        lea    eax, [esi + 11h]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esp + 1eh]
        push   eax
        push   3eh
        db  068h, 054h, 017h, 000h, 000h
        mov    eax, dword ptr [esp + 30h]
        push   edi
        push   eax
        db  0E8h, 086h, 0AFh, 0FAh, 0FFh
        add    esp, 20h
        mov    bl, al
        xor    eax, eax
        mov    al, byte ptr [esp + 12h]
        push   eax
        push   14h
        push   16h
        db  068h, 060h, 017h, 000h, 000h
        mov    eax, dword ptr [esp + 28h]
        push   edi
        push   eax
        db  0E8h, 066h, 0AFh, 0FAh, 0FFh
        add    esp, 18h
        jmp    SHORT loc_5_5352
        db  08Bh, 0FFh
loc_5_5320:
        lea    eax, [esi + 0fh]
        push   eax
        lea    eax, [esi + 0dh]
        push   eax
        lea    eax, [esi + 11h]
        mov    ecx, dword ptr [esp + 30h]
        push   eax
        mov    eax, dword ptr [esp + 30h]
        push   ecx
        push   eax
        mov    eax, dword ptr [esp + 30h]
        push   eax
        push   3eh
        db  068h, 064h, 017h, 000h, 000h
        mov    eax, dword ptr [esp + 38h]
        push   edi
        push   eax
        db  0E8h, 032h, 0AFh, 0FAh, 0FFh
        add    esp, 28h
        mov    bl, al
loc_5_5352:
        or     bl, bl
        je     SHORT loc_5_5377
loc_5_5356:
        push   2bh
        push   esi
        db  0E8h, 02Fh, 0B7h, 0FAh, 0FFh
        add    esp, 8
        mov    ecx, ebx
        db  0E8h, 0E8h, 0E7h, 0FAh, 0FFh
        mov    word ptr [edi + 1ah], ax
        movsx  eax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 2ch
        ret
loc_5_5377:
        mov    eax, dword ptr [edi]
        mov    byte ptr [esi + 0ch], al
        mov    byte ptr [esi + 12h], ah
        db  08Ah, 015h, 06Ch, 004h, 000h, 000h
        or     dl, dl
        je     SHORT loc_5_53AE
        mov    ecx, dword ptr [esp + 28h]
        cmp    ecx, dword ptr [esp + 24h]
        je     SHORT loc_5_53AE
        cmp    byte ptr [esi + 0fh], 0ffh
        jne    SHORT loc_5_53AE
        cmp    byte ptr [esi + 10h], 0ffh
        jne    SHORT loc_5_53AE
        cmp    byte ptr [esp + 11h], 0
        je     SHORT loc_5_53AE
        test   al, 10h
        je     SHORT loc_5_53AE
        mov    byte ptr [esi + 0fh], 0fdh
loc_5_53AE:
        mov    cl, al
        and    cl, 1eh
        cmp    cl, 8
        je     SHORT loc_5_53CF
        test   al, 8
        je     NEAR PTR loc_5_5476
        mov    ecx, dword ptr [esp + 20h]
        cmp    word ptr [ecx + 2], 4
        jne    NEAR PTR loc_5_5476
loc_5_53CF:
        push   8
        lea    eax, [esi + 1eh]
        push   eax
        mov    eax, dword ptr [esp + 24h]
        push   eax
        push   15h
        push   16h
        db  068h, 070h, 017h, 000h, 000h
        sub    ebx, ebx
        mov    eax, dword ptr [esp + 30h]
        push   edi
        push   eax
        db  0E8h, 08Fh, 0AEh, 0FAh, 0FFh
        add    esp, 20h
        mov    byte ptr [esi + 26h], 0
        db  08Ah, 015h, 06Ch, 004h, 000h, 000h
loc_5_5400:
        lea    ecx, [esi + ebx]
        mov    al, byte ptr [ecx + 1eh]
        or     al, al
        je     SHORT loc_5_5423
        mov    cl, byte ptr [ecx + 1]
        cmp    cl, 3fh
        je     SHORT loc_5_541D
        movsx  ebp, al
        xor    eax, eax
        mov    al, cl
        cmp    ebp, eax
        jne    SHORT loc_5_5423
loc_5_541D:
        inc    ebx
        cmp    ebx, 8
        jb     SHORT loc_5_5400
loc_5_5423:
        cmp    byte ptr [esi + ebx + 1eh], 0
        jne    SHORT loc_5_5476
        cmp    ebx, 0bh
        jae    SHORT loc_5_5445
        nop
loc_5_5430:
        mov    al, byte ptr [esi + ebx + 1]
        cmp    al, 20h
        je     SHORT loc_5_543C
        cmp    al, 3fh
        jne    SHORT loc_5_5442
loc_5_543C:
        inc    ebx
        cmp    ebx, 0bh
        jb     SHORT loc_5_5430
loc_5_5442:
        cmp    ebx, 0bh
loc_5_5445:
        jne    SHORT loc_5_5476
        mov    byte ptr [esi + 15h], 8
        mov    dword ptr [esi + 1ah], 0
        push   2bh
        push   esi
        db  0E8h, 033h, 0B6h, 0FAh, 0FFh
        add    esp, 8
        mov    word ptr [edi + 1ah], 0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 2ch
        ret
        db  08Dh, 049h, 000h
loc_5_5470:
        db  08Ah, 015h, 06Ch, 004h, 000h, 000h
loc_5_5476:
        cmp    byte ptr [edi + 4], 0
        je     SHORT loc_5_54A3
        mov    al, byte ptr [esi + 0ch]
        and    al, 1eh
        cmp    al, 8
        jne    SHORT loc_5_54A3
        push   2bh
        push   esi
        db  0E8h, 000h, 0B6h, 0FAh, 0FFh
        add    esp, 8
        mov    word ptr [edi + 1ah], 12h
        mov    eax, 12h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 2ch
        ret
loc_5_54A3:
        or     dl, dl
        je     SHORT loc_5_5510
        test   byte ptr [esi + 0ch], 10h
        je     SHORT loc_5_5510
        cmp    byte ptr [esi + 10h], 0ffh
        jne    SHORT loc_5_5510
        mov    al, byte ptr [esi + 0fh]
        cmp    al, 0fdh
        je     SHORT loc_5_54BE
        cmp    al, 0feh
        jne    SHORT loc_5_5510
loc_5_54BE:
        mov    byte ptr [esi + 15h], 10h
        mov    word ptr [esi + 16h], 0
        mov    word ptr [esi + 18h], 0
        mov    dword ptr [esi + 1ah], 0
        cmp    al, 0feh
        mov    byte ptr [esi + 1eh], 2eh
        mov    byte ptr [esi + 20h], 0
        mov    byte ptr [esi + 1fh], 0
        jne    SHORT loc_5_54E9
        mov    byte ptr [esi + 1fh], 2eh
loc_5_54E9:
        inc    al
        mov    byte ptr [esi + 0fh], al
        push   2bh
        push   esi
        db  0E8h, 097h, 0B5h, 0FAh, 0FFh
        add    esp, 8
        mov    word ptr [edi + 1ah], 0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 2ch
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_5510:
        lea    ecx, [esp + 2ch]
        mov    dword ptr [esp + 14h], ecx
        sub    bl, bl
        sub    ebp, ebp
        lea    esp, [esp]
loc_5_5520:
        mov    cl, byte ptr [esi + ebp + 1]
        db  0E8h, 0B7h, 0B8h, 0FAh, 0FFh
        or     bl, bl
        jne    SHORT loc_5_5538
        cmp    al, 20h
        jne    SHORT loc_5_5538
        cmp    ebp, 7
        ja     SHORT loc_5_557B
        jmp    SHORT loc_5_555C
loc_5_5538:
        cmp    ebp, 8
        jne    SHORT loc_5_5548
        mov    ecx, dword ptr [esp + 14h]
        inc    dword ptr [esp + 14h]
        mov    byte ptr [ecx], 0aeh
loc_5_5548:
        or     bl, bl
        jne    SHORT loc_5_5552
        cmp    al, 3fh
        jne    SHORT loc_5_5552
        or     al, 80h
loc_5_5552:
        mov    ecx, dword ptr [esp + 14h]
        inc    dword ptr [esp + 14h]
        mov    byte ptr [ecx], al
loc_5_555C:
        or     bl, bl
        je     SHORT loc_5_5564
        sub    bl, bl
        jmp    SHORT loc_5_5575
loc_5_5564:
        xor    eax, eax
        mov    al, byte ptr [esi + ebp + 1]
        db  080h, 0B8h, 0A0h, 004h, 000h, 000h, 001h
        sbb    bl, bl
        neg    bl
loc_5_5575:
        inc    ebp
        cmp    ebp, 0bh
        jb     SHORT loc_5_5520
loc_5_557B:
        mov    ecx, dword ptr [esp + 14h]
        mov    bl, 0ffh
        mov    byte ptr [ecx], 0
        mov    al, byte ptr [esi + 12h]
        mov    byte ptr [esp + 10h], al
        test   byte ptr [esi + 0ch], 10h
        je     SHORT loc_5_560B
        lea    ebp, [esi + 18h]
loc_5_5594:
        lea    eax, [esi + 16h]
        push   eax
        push   ebp
        lea    eax, [esi + 15h]
        push   eax
        push   0dh
        lea    eax, [esi + 1eh]
        push   eax
        lea    eax, [esi + 0fh]
        push   eax
        lea    eax, [esp + 44h]
        push   eax
        mov    al, byte ptr [esi + 0ch]
        and    eax, 16h
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi + 0fh]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi + 0dh]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 11h]
        push   eax
        push   3fh
        db  068h, 07Ch, 017h, 000h, 000h
        mov    eax, dword ptr [esp + 4ch]
        push   edi
        push   eax
        db  0E8h, 0A6h, 0ACh, 0FAh, 0FFh
        add    esp, 3ch
        mov    bl, al
        or     bl, bl
        jne    SHORT loc_5_55F3
        xor    eax, eax
        mov    al, byte ptr [esi + 15h]
        xor    ecx, ecx
        mov    cl, byte ptr [esp + 10h]
        not    eax
        test   ecx, eax
        jne    SHORT loc_5_5594
loc_5_55F3:
        cmp    bl, 0ffh
        jne    SHORT loc_5_5604
        and    byte ptr [esi + 0ch], 0efh
        mov    byte ptr [esi + 10h], 0ffh
        mov    byte ptr [esi + 0fh], 0ffh
loc_5_5604:
        mov    dword ptr [esi + 1ah], 0
loc_5_560B:
        mov    al, byte ptr [esi + 0ch]
        or     al, byte ptr [esp + 10h]
        test   al, 10h
        jne    SHORT loc_5_567C
        lea    ebp, [esi + 18h]
loc_5_5619:
        lea    eax, [esi + 16h]
        push   eax
        push   ebp
        lea    eax, [esi + 1ah]
        push   eax
        lea    eax, [esi + 15h]
        push   eax
        push   0dh
        lea    eax, [esi + 1eh]
        push   eax
        lea    eax, [esi + 0fh]
        push   eax
        lea    eax, [esp + 48h]
        push   eax
        mov    al, byte ptr [esi + 0ch]
        and    eax, 16h
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi + 0fh]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi + 0dh]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 11h]
        push   eax
        push   3fh
        db  068h, 08Ch, 017h, 000h, 000h
        mov    eax, dword ptr [esp + 50h]
        push   edi
        push   eax
        db  0E8h, 01Dh, 0ACh, 0FAh, 0FFh
        add    esp, 40h
        mov    bl, al
        or     bl, bl
        jne    SHORT loc_5_567C
        xor    eax, eax
        mov    al, byte ptr [esi + 15h]
        xor    ecx, ecx
        mov    cl, byte ptr [esp + 10h]
        not    eax
        test   ecx, eax
        jne    SHORT loc_5_5619
loc_5_567C:
        push   2bh
        push   esi
        db  0E8h, 009h, 0B4h, 0FAh, 0FFh
        add    esp, 8
        cmp    bl, 0ffh
        jne    SHORT loc_5_56A0
        mov    word ptr [edi + 1ah], 12h
        mov    eax, 12h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 2ch
        ret
        db  090h
loc_5_56A0:
        mov    ecx, ebx
        db  0E8h, 0A9h, 0E4h, 0FAh, 0FFh
        mov    word ptr [edi + 1ah], ax
        movsx  eax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 2ch
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_56C0:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 2ch]
        mov    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [esp + 14h]
        mov    bl, byte ptr [eax + 21h]
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [esp + 20h], eax
        mov    eax, dword ptr [ecx]
        test   eax, 24000000h
        je     SHORT loc_5_5708
        cmp    bl, 4
        je     SHORT loc_5_570D
        mov    word ptr [ecx + 1ah], 5
        mov    eax, 5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
loc_5_5708:
        cmp    bl, 4
        jne    SHORT loc_5_5716
loc_5_570D:
        test   eax, 84000000h
        jne    SHORT loc_5_5716
        sub    bl, bl
loc_5_5716:
        push   1
        push   118h
        db  0E8h, 09Ch, 0B2h, 0FAh, 0FFh
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_5_5742
        mov    ecx, dword ptr [esp + 2ch]
        mov    eax, 5
        mov    word ptr [ecx + 1ah], 5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
loc_5_5742:
        mov    ecx, dword ptr [esp + 2ch]
        mov    dword ptr [esi], 53524348h
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0ch], eax
        mov    byte ptr [esi + 11h], bl
        mov    eax, dword ptr [ecx + 10h]
        mov    dword ptr [ecx + 20h], esi
        db  0C7h, 000h, 020h, 05Bh, 000h, 000h
        mov    eax, dword ptr [ecx + 10h]
        db  0C7h, 040h, 004h, 060h, 06Dh, 000h, 000h
        mov    eax, dword ptr [ecx + 10h]
        db  0C7h, 040h, 008h, 0E0h, 014h, 000h, 000h
        mov    eax, dword ptr [esp + 20h]
        add    eax, 4
        mov    dword ptr [esp + 24h], eax
        mov    eax, dword ptr [esp + 20h]
        xor    ecx, ecx
        mov    cx, word ptr [eax + 2]
        add    eax, ecx
        mov    dword ptr [esp + 1ch], eax
        cmp    bl, 0ffh
        jne    SHORT loc_5_57D0
        lea    eax, [esi + 8]
        push   eax
        lea    eax, [esi + 4]
        push   eax
        lea    eax, [esi + 10h]
        push   eax
        mov    eax, dword ptr [esp + 28h]
        push   eax
        mov    eax, dword ptr [esp + 34h]
        push   eax
        mov    eax, dword ptr [esp + 28h]
        push   eax
        push   3eh
        mov    ecx, dword ptr [esp + 48h]
        db  068h, 0A0h, 017h, 000h, 000h
        mov    eax, dword ptr [esp + 38h]
        push   ecx
        push   eax
        db  0E8h, 0BAh, 0AAh, 0FAh, 0FFh
        add    esp, 28h
        mov    bl, al
        jmp    NEAR PTR loc_5_5911
        db  090h
loc_5_57D0:
        mov    eax, dword ptr [esp + 14h]
        cmp    byte ptr [eax + 21h], bl
        jne    SHORT loc_5_5826
        lea    eax, [esi + 8]
        push   eax
        lea    eax, [esi + 4]
        push   eax
        lea    eax, [esi + 10h]
        push   eax
        mov    eax, dword ptr [esp + 28h]
        push   0
        push   eax
        mov    eax, dword ptr [esp + 38h]
        push   eax
        mov    eax, dword ptr [esp + 2ch]
        push   4
        push   eax
        mov    al, byte ptr [eax + 20h]
        and    eax, 0ffh
        push   eax
        xor    eax, eax
        mov    al, bl
        push   eax
        push   2
        mov    ecx, dword ptr [esp + 58h]
        db  068h, 0A8h, 017h, 000h, 000h
        mov    eax, dword ptr [esp + 48h]
        push   ecx
        push   eax
        db  0E8h, 063h, 0AAh, 0FAh, 0FFh
        add    esp, 38h
        mov    bl, al
        jmp    NEAR PTR loc_5_5911
loc_5_5826:
        mov    byte ptr [esp + 13h], 0
        lea    eax, [esp + 13h]
        push   eax
        mov    eax, dword ptr [esp + 20h]
        push   0
        push   eax
        mov    eax, dword ptr [esp + 30h]
        push   eax
        mov    eax, dword ptr [esp + 24h]
        push   4
        push   eax
        push   1
        push   1
        mov    al, byte ptr [eax + 20h]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [esp + 38h]
        mov    al, byte ptr [eax + 21h]
        and    eax, 0ffh
        push   eax
        push   0ch
        mov    ecx, dword ptr [esp + 58h]
        db  068h, 0B4h, 017h, 000h, 000h
        mov    eax, dword ptr [esp + 48h]
        push   ecx
        push   eax
        db  0E8h, 00Ch, 0AAh, 0FAh, 0FFh
        add    esp, 38h
        mov    bl, al
        or     bl, bl
        jne    SHORT loc_5_58E8
        xor    eax, eax
        mov    al, byte ptr [esp + 13h]
        push   eax
        push   eax
        push   0
        push   16h
        mov    ecx, dword ptr [esp + 3ch]
        db  068h, 0C0h, 017h, 000h, 000h
        mov    eax, dword ptr [esp + 2ch]
        push   ecx
        push   eax
        db  0E8h, 0E3h, 0A9h, 0FAh, 0FFh
        add    esp, 1ch
        mov    bl, al
        or     bl, bl
        jne    SHORT loc_5_58E8
        lea    eax, [esi + 8]
        push   eax
        lea    eax, [esi + 4]
        push   eax
        lea    eax, [esi + 10h]
        push   eax
        push   4
        xor    eax, eax
        mov    al, byte ptr [esp + 23h]
        push   eax
        mov    eax, dword ptr [esp + 28h]
        mov    al, byte ptr [eax + 20h]
        and    eax, 0ffh
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 11h]
        push   eax
        push   2
        mov    ecx, dword ptr [esp + 4ch]
        db  068h, 0C8h, 017h, 000h, 000h
        mov    eax, dword ptr [esp + 3ch]
        push   ecx
        push   eax
        db  0E8h, 09Ch, 0A9h, 0FAh, 0FFh
        add    esp, 2ch
        mov    bl, al
loc_5_58E8:
        cmp    byte ptr [esp + 13h], 0
        je     SHORT loc_5_5911
        xor    eax, eax
        mov    al, byte ptr [esp + 13h]
        push   eax
        push   14h
        push   16h
        mov    ecx, dword ptr [esp + 38h]
        db  068h, 0D4h, 017h, 000h, 000h
        mov    eax, dword ptr [esp + 28h]
        push   ecx
        push   eax
        db  0E8h, 071h, 0A9h, 0FAh, 0FFh
        add    esp, 18h
loc_5_5911:
        or     bl, bl
        je     SHORT loc_5_5960
        mov    dword ptr [esi], 73726368h
        push   0
        push   esi
        db  0E8h, 0AEh, 0B0h, 0FAh, 0FFh
        add    esp, 8
        cmp    bl, 0ffh
        jne    SHORT loc_5_5942
        mov    ecx, dword ptr [esp + 2ch]
        mov    eax, 12h
        mov    word ptr [ecx + 1ah], 12h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
loc_5_5942:
        mov    ecx, ebx
        db  0E8h, 007h, 0E2h, 0FAh, 0FFh
        mov    ecx, dword ptr [esp + 2ch]
        pop    ebp
        mov    word ptr [ecx + 1ah], ax
        movsx  eax, ax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Dh, 064h, 024h, 000h
loc_5_5960:
        mov    edi, dword ptr [esp + 1ch]
        add    edi, 2
        cmp    byte ptr [esi + 11h], 0ffh
        jne    SHORT loc_5_597F
        cmp    word ptr [edi], 2ah
        jne    SHORT loc_5_597F
        cmp    word ptr [edi + 2], 0
        jne    SHORT loc_5_597F
        db  0BFh, 0D8h, 017h, 000h, 000h
loc_5_597F:
        mov    byte ptr [esp + 12h], 1
        lea    ebp, [esi + 12h]
loc_5_5987:
        mov    eax, edi
        add    edi, 2
        mov    ax, word ptr [eax]
        push   eax
        db  0E8h, 031h, 0B1h, 0FAh, 0FFh
        add    esp, 4
        mov    bx, ax
        or     bx, bx
        je     SHORT loc_5_59B7
        cmp    bx, 3fh
        je     SHORT loc_5_59B7
        cmp    bx, 2ah
        je     SHORT loc_5_59B7
        cmp    bx, 2eh
        je     SHORT loc_5_59B7
        mov    byte ptr [esp + 12h], 0
loc_5_59B7:
        cmp    byte ptr [esi + 11h], 0ffh
        jne    SHORT loc_5_5A07
        cmp    bx, 100h
        jb     SHORT loc_5_59E1
        mov    cx, bx
        shr    cx, 8
        db  0E8h, 010h, 0B4h, 0FAh, 0FFh
        mov    byte ptr [ebp], al
        inc    ebp
        mov    ecx, ebx
        db  0E8h, 005h, 0B4h, 0FAh, 0FFh
        movsx  bx, al
        jmp    SHORT loc_5_5A3A
loc_5_59E1:
        cmp    bx, 3fh
        je     SHORT loc_5_5A00
        cmp    bx, 2ah
        je     SHORT loc_5_5A00
        cmp    bx, 2eh
        je     SHORT loc_5_5A00
        mov    ecx, ebx
        db  0E8h, 0E6h, 0B3h, 0FAh, 0FFh
        movsx  bx, al
        jmp    SHORT loc_5_5A3A
loc_5_5A00:
        or     bx, 80h
        jmp    SHORT loc_5_5A3A
loc_5_5A07:
        cmp    bx, 3fh
        jne    SHORT loc_5_5A14
        mov    byte ptr [ebp], 0ffh
        inc    ebp
        jmp    SHORT loc_5_5A2F
loc_5_5A14:
        cmp    bx, 2ah
        jne    SHORT loc_5_5A2F
        mov    byte ptr [ebp], 0ffh
        inc    ebp
        cmp    word ptr [edi], 2eh
        jne    SHORT loc_5_5A2F
        cmp    word ptr [edi + 2], 2ah
        jne    SHORT loc_5_5A2F
        add    edi, 4
loc_5_5A2F:
        cmp    bx, 100h
        jb     SHORT loc_5_5A3A
        mov    byte ptr [ebp], bh
        inc    ebp
loc_5_5A3A:
        mov    byte ptr [ebp], bl
        inc    ebp
        or     bx, bx
        jne    NEAR PTR loc_5_5987
        db  080h, 03Dh, 06Ch, 004h, 000h, 000h, 000h
        je     SHORT loc_5_5A74
        mov    eax, dword ptr [esp + 1ch]
        cmp    eax, dword ptr [esp + 24h]
        je     SHORT loc_5_5A74
        cmp    dword ptr [esi + 8], -1
        jne    SHORT loc_5_5A74
        cmp    byte ptr [esp + 12h], 0
        je     SHORT loc_5_5A74
        test   byte ptr [esi + 0ch], 10h
        je     SHORT loc_5_5A74
        mov    dword ptr [esi + 8], 0fffffffdh
loc_5_5A74:
        mov    eax, dword ptr [esi + 0ch]
        mov    ecx, eax
        and    cl, 1eh
        cmp    cl, 8
        je     SHORT loc_5_5AC3
        test   al, 8
        je     SHORT loc_5_5A90
        mov    eax, dword ptr [esp + 20h]
        cmp    word ptr [eax + 2], 4
        je     SHORT loc_5_5AC3
loc_5_5A90:
        mov    ecx, dword ptr [esp + 2ch]
        push   ecx
        call   sub_5_5B20
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_5_5AB0
        mov    ecx, dword ptr [esp + 2ch]
        push   ecx
        call   sub_5_5F70
        add    esp, 4
loc_5_5AB0:
        mov    ecx, dword ptr [esp + 2ch]
        movsx  eax, si
        mov    word ptr [ecx + 1ah], si
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
loc_5_5AC3:
        mov    ecx, dword ptr [esp + 2ch]
        mov    esi, dword ptr [ecx + 14h]
        lea    eax, [esi + 2ch]
        push   eax
        mov    eax, dword ptr [esp + 18h]
        push   eax
        push   15h
        push   16h
        db  068h, 0E0h, 017h, 000h, 000h
        mov    eax, dword ptr [esp + 2ch]
        push   ecx
        push   eax
        db  0E8h, 098h, 0A7h, 0FAh, 0FFh
        add    esp, 1ch
        mov    word ptr [esi + 4ch], 0
        mov    dword ptr [esi], 8
        mov    word ptr [esi + 234h], 0
        mov    ecx, eax
        db  0E8h, 04Ah, 0E0h, 0FAh, 0FFh
        mov    ecx, dword ptr [esp + 2ch]
        pop    ebp
        mov    word ptr [ecx + 1ah], ax
        movsx  eax, ax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_5B20:
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 44h]
        mov    edi, dword ptr [ebx + 1ch]
        mov    ebp, dword ptr [ebx + 20h]
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esp + 14h], eax
        mov    edx, dword ptr [ebx + 14h]
        mov    dword ptr [esp + 18h], edx
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [esp + 13h], ah
        test   byte ptr [ebx + 18h], 40h
        je     SHORT loc_5_5B52
        mov    ecx, dword ptr [ebx + 24h]
        mov    dword ptr [ebp + 8], ecx
loc_5_5B52:
        db  080h, 03Dh, 06Ch, 004h, 000h, 000h, 000h
        je     SHORT loc_5_5BB8
        test   al, 10h
        je     SHORT loc_5_5BB8
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, -3
        je     SHORT loc_5_5B6C
        cmp    esi, -2
        jne    SHORT loc_5_5BB8
loc_5_5B6C:
        mov    edi, dword ptr [esp + 18h]
        sub    eax, eax
        mov    ecx, 94h
        rep stosd
        mov    edx, dword ptr [esp + 18h]
        cmp    esi, -2
        mov    dword ptr [edx], 10h
        mov    word ptr [edx + 2ch], 2eh
        jne    SHORT loc_5_5B94
        mov    word ptr [edx + 2eh], 2eh
loc_5_5B94:
        mov    edx, dword ptr [esp + 18h]
        inc    esi
        mov    word ptr [edx + 234h], 0
        mov    dword ptr [ebp + 8], esi
        mov    dword ptr [ebx + 24h], esi
        mov    word ptr [ebx + 1ah], 0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
loc_5_5BB8:
        mov    byte ptr [esp + 12h], 0ffh
        movzx  esi, byte ptr [esp + 13h]
        not    eax
        and    eax, esi
        test   al, 10h
        jne    NEAR PTR loc_5_5F34
        mov    edi, edi
loc_5_5BD0:
        cmp    byte ptr [ebp + 11h], 0ffh
        je     NEAR PTR loc_5_5C60
        mov    word ptr [esp + 2ah], 0
        mov    eax, dword ptr [esp + 18h]
        add    eax, 2ch
        lea    edx, [ebp + 8]
        push   eax
        lea    eax, [esp + 32h]
        push   14h
        push   eax
        lea    ecx, [ebp + 4]
        lea    eax, [esp + 38h]
        push   0eh
        push   eax
        lea    eax, [esp + 3ah]
        push   4
        push   eax
        lea    eax, [esp + 3eh]
        push   4
        push   eax
        lea    eax, [esp + 42h]
        push   6
        push   eax
        lea    eax, [esp + 48h]
        push   4
        push   eax
        lea    eax, [ebp + 12h]
        push   4
        push   edx
        push   ecx
        push   eax
        mov    eax, dword ptr [edx]
        push   eax
        mov    eax, dword ptr [ecx]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        push   eax
        push   58dh
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 16h
        push   eax
        push   0
        xor    eax, eax
        mov    al, byte ptr [ebp + 11h]
        push   eax
        push   3
        db  068h, 0ECh, 017h, 000h, 000h
        mov    eax, dword ptr [esp + 7ch]
        push   ebx
        push   eax
        db  0E8h, 02Ch, 0A6h, 0FAh, 0FFh
        add    esp, 70h
        jmp    NEAR PTR loc_5_5D50
        db  005h, 000h, 000h, 000h, 000h
loc_5_5C60:
        test   byte ptr [ebp + 0ch], 10h
        je     SHORT loc_5_5CE0
        mov    dword ptr [esp + 1eh], 0
        mov    word ptr [esp + 1ch], 0
        mov    word ptr [esp + 22h], 0
        mov    word ptr [esp + 24h], 0
        lea    ecx, [ebp + 8]
        mov    word ptr [esp + 2ah], 0
        mov    word ptr [esp + 2ch], 0
        mov    byte ptr [esp + 3fh], 0
        lea    eax, [esp + 26h]
        push   eax
        lea    eax, [esp + 2ch]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        lea    eax, [esp + 3eh]
        push   0dh
        push   eax
        lea    eax, [ebp + 12h]
        push   ecx
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 16h
        push   eax
        mov    eax, dword ptr [ecx]
        push   eax
        mov    eax, dword ptr [ebp + 4]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        push   eax
        push   3fh
        db  068h, 00Ch, 018h, 000h, 000h
        mov    eax, dword ptr [esp + 48h]
        push   ebx
        push   eax
        db  0E8h, 0A5h, 0A5h, 0FAh, 0FFh
        add    esp, 3ch
        jmp    SHORT loc_5_5D50
        db  090h
loc_5_5CE0:
        mov    word ptr [esp + 1ch], 0
        mov    word ptr [esp + 22h], 0
        mov    word ptr [esp + 2ah], 0
        mov    byte ptr [esp + 3fh], 0
        lea    eax, [esp + 26h]
        push   eax
        lea    ecx, [ebp + 8]
        lea    eax, [esp + 2ch]
        push   eax
        lea    eax, [esp + 34h]
        push   eax
        lea    eax, [esp + 30h]
        push   eax
        lea    eax, [esp + 2eh]
        push   eax
        lea    eax, [esp + 30h]
        push   eax
        lea    eax, [esp + 4ah]
        push   0dh
        push   eax
        lea    eax, [ebp + 12h]
        push   ecx
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 16h
        push   eax
        mov    eax, dword ptr [ecx]
        push   eax
        mov    eax, dword ptr [ebp + 4]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        push   eax
        push   3fh
        db  068h, 01Ch, 018h, 000h, 000h
        mov    eax, dword ptr [esp + 54h]
        push   ebx
        push   eax
        db  0E8h, 032h, 0A5h, 0FAh, 0FFh
        add    esp, 48h
loc_5_5D50:
        mov    byte ptr [esp + 12h], al
        cmp    byte ptr [esp + 12h], 0
        jne    SHORT loc_5_5D75
        mov    eax, dword ptr [esp + 1ch]
        and    eax, 0ffffh
        not    eax
        test   esi, eax
        jne    NEAR PTR loc_5_5BD0
        cmp    byte ptr [esp + 12h], 0
        je     SHORT loc_5_5DA0
loc_5_5D75:
        mov    eax, dword ptr [ebp + 0ch]
        test   al, 10h
        je     NEAR PTR loc_5_5F34
        and    eax, 0ffffffefh
        mov    dword ptr [ebp + 0ch], eax
        mov    dword ptr [ebp + 8], 0ffffffffh
        not    eax
        and    eax, esi
        test   al, 10h
        je     NEAR PTR loc_5_5BD0
        jmp    NEAR PTR loc_5_5F34
        db  08Bh, 0FFh
loc_5_5DA0:
        mov    eax, dword ptr [esp + 1ch]
        mov    edx, dword ptr [esp + 18h]
        and    eax, 0ffffh
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [esp + 22h]
        push   eax
        db  0E8h, 013h, 0ADh, 0FAh, 0FFh
        mov    ecx, dword ptr [esp + 1ch]
        add    esp, 4
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx + 8], edx
        add    ecx, 4
        mov    eax, dword ptr [esp + 2ah]
        push   eax
        db  0E8h, 0F9h, 0ACh, 0FAh, 0FFh
        mov    ecx, dword ptr [esp + 1ch]
        add    esp, 4
        mov    dword ptr [ecx + 0ch], eax
        mov    dword ptr [ecx + 10h], edx
        add    ecx, 0ch
        mov    eax, dword ptr [esp + 26h]
        push   eax
        db  0E8h, 0DFh, 0ACh, 0FAh, 0FFh
        mov    ecx, dword ptr [esp + 1ch]
        add    esp, 4
        mov    dword ptr [ecx + 14h], eax
        mov    dword ptr [ecx + 18h], edx
        add    ecx, 14h
        mov    edx, dword ptr [esp + 18h]
        mov    dword ptr [edx + 1ch], 0
        mov    eax, dword ptr [esp + 1eh]
        mov    dword ptr [edx + 20h], eax
        mov    al, byte ptr [ebp + 11h]
        cmp    al, 0ffh
        jne    SHORT loc_5_5E56
        lea    edi, [esp + 32h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        push   1
        lea    eax, [esp + 36h]
        push   ecx
        push   eax
        add    edx, 2ch
        push   edx
        db  0E8h, 089h, 0ACh, 0FAh, 0FFh
        add    esp, 10h
        and    eax, 0fffffffeh
        mov    edx, dword ptr [esp + 18h]
        mov    word ptr [eax + edx + 2ch], 0
        mov    word ptr [edx + 234h], 0
        jmp    NEAR PTR loc_5_5F34
loc_5_5E56:
        cmp    byte ptr [edi + 21h], 4
        je     SHORT loc_5_5E70
        mov    edx, dword ptr [esp + 18h]
        mov    word ptr [edx + 234h], 0
        jmp    NEAR PTR loc_5_5F34
        db  08Bh, 0FFh
loc_5_5E70:
        cmp    al, 4
        jne    SHORT loc_5_5EE4
        db  080h, 03Dh, 067h, 004h, 000h, 000h, 000h
        je     SHORT loc_5_5EC0
        mov    esi, dword ptr [esp + 18h]
        add    esi, 2ch
        push   esi
        db  0E8h, 06Ch, 0ACh, 0FAh, 0FFh
        add    esp, 4
        test   eax, 4000000h
        jne    SHORT loc_5_5EC0
        mov    eax, dword ptr [esp + 18h]
        add    eax, 234h
        mov    edi, eax
        mov    ecx, 6
        rep movsd
        movsw
        mov    edx, dword ptr [esp + 18h]
        push   eax
        mov    word ptr [edx + 24eh], 0
        db  0E8h, 030h, 0A9h, 0FAh, 0FFh
        add    esp, 4
        jmp    SHORT loc_5_5F34
loc_5_5EC0:
        mov    eax, dword ptr [esp + 18h]
        add    eax, 234h
        push   eax
        push   0fh
        mov    eax, dword ptr [esp + 36h]
        push   0
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        push   eax
        push   30h
        push   16h
        db  068h, 030h, 018h, 000h, 000h
        jmp    SHORT loc_5_5F26
loc_5_5EE4:
        mov    eax, dword ptr [esp + 18h]
        add    eax, 2ch
        mov    edi, dword ptr [esp + 18h]
        add    edi, 234h
        mov    esi, eax
        mov    ecx, 6
        rep movsd
        movsw
        mov    edx, dword ptr [esp + 18h]
        push   eax
        mov    word ptr [edx + 24eh], 0
        push   0fh
        mov    eax, dword ptr [esp + 36h]
        push   4
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        push   eax
        push   30h
        push   16h
        db  068h, 038h, 018h, 000h, 000h
loc_5_5F26:
        mov    eax, dword ptr [esp + 34h]
        push   ebx
        push   eax
        db  0E8h, 04Eh, 0A3h, 0FAh, 0FFh
        add    esp, 28h
loc_5_5F34:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 24h], eax
        cmp    byte ptr [esp + 12h], 0ffh
        jne    SHORT loc_5_5F54
        mov    word ptr [ebx + 1ah], 12h
        mov    eax, 12h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
loc_5_5F54:
        mov    ecx, dword ptr [esp + 12h]
        db  0E8h, 0F3h, 0DBh, 0FAh, 0FFh
        mov    word ptr [ebx + 1ah], ax
        movsx  eax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  08Dh, 064h, 024h, 000h
sub_5_5F70:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   0
        mov    eax, dword ptr [esi + 20h]
        push   eax
        mov    dword ptr [eax], 73726368h
        db  0E8h, 04Bh, 0AAh, 0FAh, 0FFh
        add    esp, 8
        mov    word ptr [esi + 1ah], 0
        sub    eax, eax
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_5FA0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ecx, esi
        call   sub_5_B480
        or     eax, eax
        je     SHORT loc_5_5FC2
        cmp    word ptr [esi + 1ah], 0
        jne    SHORT loc_5_5FC2
        push   esi
        call   sub_5_6060
        add    esp, 4
        pop    esi
        ret
loc_5_5FC2:
        mov    cx, word ptr [esi + 1ah]
        cmp    cx, 0a2h
        jne    SHORT loc_5_5FE9
        mov    eax, dword ptr [esi + 20h]
        mov    word ptr [esi + 1ah], 0
        mov    ecx, dword ptr [eax + 18h]
        call   sub_5_A9F0
        push   esi
        call   sub_5_6060
        add    esp, 4
        pop    esi
        ret
loc_5_5FE9:
        or     cx, cx
        jne    SHORT loc_5_5FFA
        db  0A1h, 050h, 010h, 000h, 000h
        add    eax, dword ptr [esi]
        db  0A3h, 050h, 010h, 000h, 000h
loc_5_5FFA:
        movsx  eax, cx
        pop    esi
        ret
        db  090h
loc_5_6000:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi], 0
        jne    SHORT loc_5_6023
        mov    eax, dword ptr [esi + 20h]
        mov    edx, dword ptr [esi + 24h]
        mov    ecx, dword ptr [eax + 18h]
        call   sub_5_B050
        push   esi
        call   sub_5_6390
        add    esp, 4
        pop    esi
        ret
loc_5_6023:
        mov    ecx, esi
        call   sub_5_B1C0
        or     eax, eax
        je     SHORT loc_5_6040
        cmp    word ptr [esi + 1ah], 0
        jne    SHORT loc_5_6040
        push   esi
        call   sub_5_6390
        add    esp, 4
        pop    esi
        ret
loc_5_6040:
        mov    cx, word ptr [esi + 1ah]
        or     cx, cx
        jne    SHORT loc_5_6055
        db  0A1h, 054h, 010h, 000h, 000h
        add    eax, dword ptr [esi]
        db  0A3h, 054h, 010h, 000h, 000h
loc_5_6055:
        movsx  eax, cx
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_5_6060:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ecx, esi
        call   sub_5_B3C0
        push   esi
        call   sub_5_6090
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_5_6087
        db  08Bh, 00Dh, 050h, 010h, 000h, 000h
        add    ecx, dword ptr [esi]
        db  089h, 00Dh, 050h, 010h, 000h, 000h
loc_5_6087:
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_6090:
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 40h]
        mov    eax, dword ptr [ebx + 20h]
        mov    dword ptr [esp + 2ch], eax
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [esp + 18h], 0
        mov    ecx, dword ptr [eax + 10h]
        mov    dword ptr [esp + 24h], ecx
        mov    eax, dword ptr [ecx + 48h]
        or     eax, eax
        mov    dword ptr [esp + 38h], eax
        je     SHORT loc_5_60D0
        cmp    byte ptr [ecx + 59h], 1
        jbe    SHORT loc_5_60D0
        mov    ax, word ptr [eax + 18h]
        jmp    SHORT loc_5_60D8
        db  005h, 000h, 000h, 000h, 000h
loc_5_60D0:
        mov    ecx, dword ptr [esp + 24h]
        mov    ax, word ptr [ecx + 4eh]
loc_5_60D8:
        mov    word ptr [esp + 16h], ax
        mov    eax, dword ptr [ebx + 24h]
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [ebx]
        sub    bp, bp
        mov    dword ptr [esp + 28h], eax
        mov    eax, dword ptr [ebx + 14h]
        mov    dword ptr [esp + 20h], eax
        mov    dword ptr [esp + 34h], 0
        mov    eax, dword ptr [esp + 2ch]
        mov    word ptr [esp + 14h], 0
        mov    ecx, dword ptr [eax + 18h]
        call   sub_5_A7B0
        mov    eax, dword ptr [esp + 2ch]
        mov    ecx, dword ptr [eax + 18h]
        call   sub_5_A710
        test   byte ptr [ebx + 19h], 10h
        jne    SHORT loc_5_6172
        mov    eax, dword ptr [ebx]
        push   eax
        mov    eax, dword ptr [ebx + 14h]
        push   eax
        db  0E8h, 023h, 0A9h, 0FAh, 0FFh
        mov    dword ptr [esp + 28h], eax
        mov    dword ptr [esp + 3ch], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_5_6172
        push   400h
        db  0E8h, 092h, 0A9h, 0FAh, 0FFh
        mov    dword ptr [esp + 1ch], eax
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_5_6164
        mov    word ptr [ebx + 1ah], 5
        mov    eax, 5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 2ch
        ret
loc_5_6164:
        mov    eax, dword ptr [ebx + 14h]
        mov    dword ptr [esp + 20h], eax
        mov    word ptr [esp + 16h], 400h
loc_5_6172:
        cmp    dword ptr [esp + 28h], 0
        je     NEAR PTR loc_5_62E9
loc_5_6180:
        cmp    word ptr [esp + 14h], bp
        jne    NEAR PTR loc_5_62E9
        xor    ebp, ebp
        mov    bp, word ptr [esp + 16h]
        mov    eax, dword ptr [esp + 28h]
        cmp    ebp, eax
        jb     SHORT loc_5_619C
        mov    ebp, eax
loc_5_619C:
        cmp    dword ptr [esp + 38h], 0
        je     NEAR PTR loc_5_6230
        mov    ecx, dword ptr [esp + 24h]
        cmp    byte ptr [ecx + 59h], 1
        je     SHORT loc_5_6230
        cmp    word ptr [ecx + 4eh], bp
        jae    SHORT loc_5_6230
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_5_61C4
        mov    eax, dword ptr [esp + 18h]
        jmp    SHORT loc_5_61C8
loc_5_61C4:
        mov    eax, dword ptr [esp + 20h]
loc_5_61C8:
        mov    ecx, ebp
        and    ecx, 0ffffh
        push   ecx
        push   eax
        mov    eax, dword ptr [esp + 24h]
        and    eax, 3
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 40h]
        push   2
        push   eax
        mov    eax, dword ptr [esp + 34h]
        push   ecx
        push   eax
        mov    eax, dword ptr [ebx]
        push   eax
        mov    eax, dword ptr [ebx + 24h]
        push   eax
        mov    eax, dword ptr [esp + 54h]
        push   4
        add    eax, 6
        push   eax
        push   1
        push   0
        db  068h, 04Ch, 018h, 000h, 000h
        mov    ecx, dword ptr [esp + 60h]
        push   ebx
        push   ecx
        db  0E8h, 06Eh, 0A0h, 0FAh, 0FFh
        mov    byte ptr [esp + 57h], al
        add    esp, 44h
        or     al, al
        jne    NEAR PTR loc_5_629C
        cmp    dword ptr [esp + 30h], 0
        je     SHORT loc_5_629C
        mov    byte ptr [esp + 13h], 0ffh
        jmp    SHORT loc_5_629C
        db  08Bh, 0FFh
loc_5_6230:
        mov    ax, 1000h
        mov    ecx, dword ptr [esp + 1ch]
        and    cx, 0fffh
        sub    ax, cx
        cmp    ax, bp
        jb     SHORT loc_5_6248
        mov    ax, bp
loc_5_6248:
        mov    bp, ax
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_5_6258
        mov    eax, dword ptr [esp + 18h]
        jmp    SHORT loc_5_625C
loc_5_6258:
        mov    eax, dword ptr [esp + 20h]
loc_5_625C:
        mov    ecx, ebp
        and    ecx, 0ffffh
        push   ecx
        push   eax
        mov    eax, dword ptr [esp + 24h]
        and    eax, 1
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        mov    eax, dword ptr [esp + 2ch]
        push   ecx
        push   eax
        mov    eax, dword ptr [esp + 44h]
        push   6
        add    eax, 4
        push   eax
        push   48h
        db  068h, 040h, 018h, 000h, 000h
        mov    ecx, dword ptr [esp + 4ch]
        push   ebx
        push   ecx
        db  0E8h, 0EAh, 09Fh, 0FAh, 0FFh
        mov    byte ptr [esp + 43h], al
        add    esp, 30h
loc_5_629C:
        cmp    byte ptr [esp + 13h], 0
        jne    SHORT loc_5_6305
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_5_62C9
        mov    eax, dword ptr [esp + 14h]
        and    eax, 0ffffh
        mov    edi, dword ptr [esp + 20h]
        mov    esi, dword ptr [esp + 18h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_5_62C9:
        mov    eax, dword ptr [esp + 14h]
        and    eax, 0ffffh
        add    dword ptr [esp + 1ch], eax
        sub    dword ptr [esp + 28h], eax
        add    dword ptr [esp + 20h], eax
        cmp    dword ptr [esp + 28h], 0
        jne    NEAR PTR loc_5_6180
loc_5_62E9:
        test   byte ptr [ebx + 19h], 10h
        jne    SHORT loc_5_6367
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_5_6350
        mov    eax, dword ptr [esp + 18h]
        push   eax
        db  0E8h, 0DEh, 0A7h, 0FAh, 0FFh
        add    esp, 4
        jmp    SHORT loc_5_6367
loc_5_6305:
        cmp    dword ptr [esp + 34h], 0
        je     SHORT loc_5_631C
        mov    eax, dword ptr [ebx]
        push   eax
        mov    eax, dword ptr [esp + 38h]
        push   eax
        db  0E8h, 074h, 0A7h, 0FAh, 0FFh
        add    esp, 8
loc_5_631C:
        cmp    byte ptr [esp + 13h], 0ffh
        jne    SHORT loc_5_6336
        mov    word ptr [ebx + 1ah], 5
        mov    eax, 5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 2ch
        ret
loc_5_6336:
        mov    ecx, dword ptr [esp + 13h]
        db  0E8h, 011h, 0D8h, 0FAh, 0FFh
        mov    word ptr [ebx + 1ah], ax
        movsx  eax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 2ch
        ret
        db  08Bh, 0FFh
loc_5_6350:
        cmp    dword ptr [esp + 34h], 0
        je     SHORT loc_5_6367
        mov    eax, dword ptr [ebx]
        push   eax
        mov    eax, dword ptr [esp + 38h]
        push   eax
        db  0E8h, 029h, 0A7h, 0FAh, 0FFh
        add    esp, 8
loc_5_6367:
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [ebx + 24h], eax
        mov    eax, dword ptr [esp + 28h]
        sub    dword ptr [ebx], eax
        sub    eax, eax
        mov    word ptr [ebx + 1ah], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 2ch
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_6390:
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 3ch]
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [esp + 34h], eax
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [esp + 34h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [eax + 48h]
        mov    dword ptr [esp + 20h], eax
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_5_63D0
        push   eax
        mov    eax, dword ptr [esi + 14h]
        push   eax
        db  0E8h, 085h, 0A6h, 0FAh, 0FFh
        add    esp, 8
        jmp    SHORT loc_5_63D3
loc_5_63D0:
        mov    eax, dword ptr [esi + 14h]
loc_5_63D3:
        mov    dword ptr [esp + 2ch], eax
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [esp + 18h], eax
        or     eax, eax
        jne    SHORT loc_5_6400
        cmp    dword ptr [esi], 0
        je     SHORT loc_5_6400
        mov    word ptr [esi + 1ah], 5
        mov    eax, 5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_5_6400:
        mov    eax, dword ptr [esp + 1ch]
        mov    edx, dword ptr [eax + 18h]
        mov    dword ptr [esp + 28h], edx
        mov    ecx, edx
        call   sub_5_A7B0
        mov    ecx, dword ptr [esp + 28h]
        call   sub_5_A710
        mov    eax, dword ptr [esp + 14h]
        mov    edi, dword ptr [esi + 24h]
        mov    ax, word ptr [eax + 4eh]
        mov    word ptr [esp + 12h], ax
        and    eax, 0ffffh
        mov    ebx, dword ptr [esi]
        mov    dword ptr [esp + 30h], eax
loc_5_6436:
        cmp    ebx, dword ptr [esp + 30h]
        mov    bp, bx
        jbe    SHORT loc_5_6444
        mov    bp, word ptr [esp + 12h]
loc_5_6444:
        mov    eax, edi
        and    eax, 0fffh
        mov    ecx, ebp
        and    ecx, 0ffffh
        add    eax, ecx
        cmp    eax, 1000h
        jbe    SHORT loc_5_6466
        mov    ebp, edi
        neg    bp
        and    bp, 0fffh
loc_5_6466:
        cmp    dword ptr [esp + 20h], 0
        je     SHORT loc_5_64E2
        mov    eax, dword ptr [esp + 14h]
        cmp    byte ptr [eax + 59h], 1
        je     SHORT loc_5_64E2
        mov    eax, ebp
        and    eax, 0ffffh
        cmp    eax, ebx
        je     SHORT loc_5_64E2
        mov    eax, dword ptr [esp + 20h]
        mov    ebp, dword ptr [eax + 14h]
        cmp    ebp, ebx
        jb     SHORT loc_5_648F
        mov    ebp, ebx
loc_5_648F:
        mov    ecx, ebp
        and    ecx, 0ffffh
        lea    eax, [esp + 24h]
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   ecx
        push   eax
        push   ecx
        push   edi
        mov    eax, dword ptr [esi]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        mov    eax, dword ptr [esp + 38h]
        push   4
        add    eax, 6
        push   eax
        push   2
        push   0
        db  068h, 064h, 018h, 000h, 000h
        mov    eax, dword ptr [esp + 44h]
        push   esi
        push   eax
        db  0E8h, 0B6h, 09Dh, 0FAh, 0FFh
        mov    byte ptr [esp + 49h], al
        add    esp, 38h
        or     al, al
        jne    SHORT loc_5_6515
        cmp    dword ptr [esp + 24h], 0
        je     SHORT loc_5_6515
        mov    byte ptr [esp + 11h], 0ffh
        jmp    SHORT loc_5_6515
loc_5_64E2:
        mov    ecx, ebp
        and    ecx, 0ffffh
        mov    eax, dword ptr [esp + 18h]
        push   ecx
        push   eax
        push   ecx
        push   edi
        mov    eax, dword ptr [esp + 2ch]
        push   6
        add    eax, 4
        push   eax
        push   49h
        db  068h, 05Ch, 018h, 000h, 000h
        mov    eax, dword ptr [esp + 34h]
        push   esi
        push   eax
        db  0E8h, 071h, 09Dh, 0FAh, 0FFh
        mov    byte ptr [esp + 39h], al
        add    esp, 28h
loc_5_6515:
        cmp    byte ptr [esp + 11h], 0
        jne    SHORT loc_5_6560
        mov    eax, ebp
        and    eax, 0ffffh
        add    dword ptr [esp + 18h], eax
        add    edi, eax
        sub    ebx, eax
        jne    NEAR PTR loc_5_6436
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_5_65A8
        push   eax
        mov    eax, dword ptr [esp + 30h]
        push   eax
        db  0E8h, 04Bh, 0A5h, 0FAh, 0FFh
        add    esp, 8
        mov    ecx, dword ptr [esi]
        mov    edx, dword ptr [esp + 28h]
        add    ecx, dword ptr [esi + 24h]
        mov    eax, dword ptr [edx + 5ch]
        cmp    ecx, eax
        ja     SHORT loc_5_6557
        mov    ecx, eax
loc_5_6557:
        mov    edx, dword ptr [esp + 28h]
        mov    dword ptr [edx + 5ch], ecx
        jmp    SHORT loc_5_65B2
loc_5_6560:
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_5_6574
        push   eax
        mov    eax, dword ptr [esp + 30h]
        push   eax
        db  0E8h, 01Ch, 0A5h, 0FAh, 0FFh
        add    esp, 8
loc_5_6574:
        cmp    byte ptr [esp + 11h], 0ffh
        jne    SHORT loc_5_6590
        mov    word ptr [esi + 1ah], 5
        mov    eax, 5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
        db  08Bh, 0FFh
loc_5_6590:
        mov    ecx, dword ptr [esp + 11h]
        db  0E8h, 0B7h, 0D5h, 0FAh, 0FFh
        mov    word ptr [esi + 1ah], ax
        movsx  eax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
loc_5_65A8:
        mov    edx, dword ptr [esp + 28h]
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [edx + 5ch], eax
loc_5_65B2:
        mov    edx, dword ptr [esi]
        db  0A1h, 054h, 010h, 000h, 000h
        add    eax, edx
        db  0A3h, 054h, 010h, 000h, 000h
        mov    eax, dword ptr [esp + 34h]
        mov    ecx, dword ptr [eax + 2ch]
        mov    eax, ecx
        cmp    eax, edx
        jb     SHORT loc_5_65CF
        mov    eax, edx
loc_5_65CF:
        sub    ecx, eax
        mov    eax, dword ptr [esp + 34h]
        mov    edx, dword ptr [esp + 28h]
        mov    dword ptr [eax + 2ch], ecx
        mov    dword ptr [esi + 24h], edi
        mov    eax, dword ptr [edx + 54h]
        test   ah, 2
        jne    SHORT loc_5_65F6
        or     eax, 200h
        mov    dword ptr [edx + 54h], eax
        mov    ecx, edx
        call   sub_5_A8D0
loc_5_65F6:
        mov    word ptr [esi + 1ah], 0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_6610:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        mov    edi, dword ptr [esi + 20h]
        mov    eax, dword ptr [esi + 1ch]
        mov    ebx, dword ptr [edi + 18h]
        mov    ebp, dword ptr [eax + 10h]
        xor    eax, eax
        mov    al, byte ptr [esi + 4]
        cmp    eax, 2
        jne    NEAR PTR loc_5_66C7
        test   byte ptr [ebx + 55h], 1
        jne    SHORT loc_5_667A
        mov    al, byte ptr [ebx + 77h]
        inc    al
        mov    byte ptr [ebx + 77h], al
        cmp    al, 4
        ja     SHORT loc_5_6651
        db  0E8h, 0A1h, 0A3h, 0FAh, 0FFh
        cmp    eax, dword ptr [ebx + 72h]
        jbe    SHORT loc_5_666D
loc_5_6651:
        or     dword ptr [ebx + 54h], 100h
        mov    ecx, ebx
        call   sub_5_A8D0
        mov    ecx, ebx
        call   sub_5_A7B0
        mov    ecx, ebx
        call   sub_5_A710
loc_5_666D:
        db  0E8h, 07Bh, 0A3h, 0FAh, 0FFh
        add    eax, 7d0h
        mov    dword ptr [ebx + 72h], eax
loc_5_667A:
        mov    eax, dword ptr [edi + 18h]
        test   byte ptr [eax + 55h], 4
        jne    SHORT loc_5_66B9
        lea    eax, [esp + 10h]
        push   eax
        push   6
        add    edi, 4
        push   edi
        push   47h
        db  068h, 070h, 018h, 000h, 000h
        push   esi
        push   ebp
        db  0E8h, 0E3h, 09Bh, 0FAh, 0FFh
        add    esp, 1ch
        or     al, al
        je     SHORT loc_5_66C0
        mov    ecx, eax
        db  0E8h, 0A6h, 0D4h, 0FAh, 0FFh
        mov    word ptr [esi + 1ah], ax
        movsx  eax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_5_66B9:
        mov    eax, dword ptr [eax + 5ch]
        mov    dword ptr [esp + 10h], eax
loc_5_66C0:
        mov    eax, dword ptr [esp + 10h]
        add    dword ptr [esi + 24h], eax
loc_5_66C7:
        mov    word ptr [esi + 1ah], 0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_66E0:
        push   ebx
        push   esi
        push   edi
        mov    ebx, ecx
        mov    esi, edx
        mov    edi, dword ptr [ebx + 18h]
        mov    eax, dword ptr [edi + 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 07Ah, 0DFh, 0FAh, 0FFh
        add    esp, 4
        inc    dword ptr [edi + 48h]
        inc    dword ptr [edi + 4ch]
        or     dword ptr [ebx + 2ch], 10h
        cmp    dword ptr [edi + 14h], ebx
        je     SHORT loc_5_671A
        cmp    dword ptr [edi + 18h], ebx
        je     SHORT loc_5_671A
        mov    edx, esi
        mov    ecx, ebx
        call   sub_5_6730
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_671A:
        mov    edx, esi
        mov    ecx, edi
        call   sub_5_B770
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_6730:
        push   ebx
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        call   sub_5_AF00
        mov    ebx, eax
        mov    eax, dword ptr [esi + 18h]
        xor    ecx, ecx
        dec    dword ptr [eax + 4ch]
        mov    dword ptr [ebx + 2ch], eax
        mov    byte ptr [ebx + 14h], 2
        mov    cl, byte ptr [esi + 10h]
        sub    edx, edx
        mov    dword ptr [ebx + 8], ecx
        mov    eax, dword ptr [eax + 8]
        mov    ecx, ebx
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebx + 18h], eax
        mov    dword ptr [ebx + 28h], edi
        mov    dword ptr [ebx + 48h], esi
        call   sub_5_BCF0
        or     edi, edi
        jne    SHORT loc_5_6775
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_6775:
        test   byte ptr [esi + 2ch], 20h
        jne    SHORT loc_5_678F
        add    eax, 0
loc_5_6780:
        push   esi
        db  0E8h, 05Eh, 0A3h, 0FAh, 0FFh
        add    esp, 4
        test   byte ptr [esi + 2ch], 20h
        je     SHORT loc_5_6780
loc_5_678F:
        mov    dword ptr [esi], 656c6966h
        push   0
        push   esi
        db  0E8h, 034h, 0A2h, 0FAh, 0FFh
        add    esp, 8
        movzx  esi, byte ptr [ebx + 17h]
        push   ebx
        db  0E8h, 048h, 09Ah, 0FAh, 0FFh
        add    esp, 4
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_67C0:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 24h]
        mov    ebp, dword ptr [esi + 1ch]
        mov    edi, dword ptr [esi + 20h]
        mov    ebx, dword ptr [ebp + 10h]
        cmp    byte ptr [edi + 0fh], 1
        jne    SHORT loc_5_67F5
        mov    al, byte ptr [ebp + 1fh]
        cmp    al, 3
        jae    SHORT loc_5_67F5
        xor    ecx, ecx
        mov    cl, al
        db  0A1h, 0E4h, 003h, 000h, 000h
        cmp    byte ptr [eax + ecx*8 + 4], 2
        jne    SHORT loc_5_67F5
        mov    byte ptr [edi + 0fh], 2
loc_5_67F5:
        mov    al, byte ptr [edi + 0eh]
        mov    byte ptr [esp + 13h], al
        mov    edx, dword ptr [edi + 14h]
        mov    dword ptr [esp + 14h], edx
        mov    ecx, dword ptr [edi + 18h]
        mov    dword ptr [esp + 1ch], ecx
        mov    al, byte ptr [edi + 0fh]
        cmp    al, 1
        jne    SHORT loc_5_6838
        xor    eax, eax
        mov    ax, word ptr [edi + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 28h]
        push   eax
        push   69h
        push   17h
        db  068h, 078h, 018h, 000h, 000h
        push   esi
        push   ebx
        db  0E8h, 053h, 09Ah, 0FAh, 0FFh
        add    esp, 1ch
        dec    word ptr [ebx + 50h]
        jmp    NEAR PTR loc_5_6905
loc_5_6838:
        cmp    al, 2
        jne    SHORT loc_5_6863
        xor    eax, eax
        mov    ax, word ptr [edi + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 28h]
        push   eax
        push   6ah
        push   17h
        db  068h, 07Ch, 018h, 000h, 000h
        push   esi
        push   ebx
        db  0E8h, 028h, 09Ah, 0FAh, 0FFh
        add    esp, 1ch
        dec    word ptr [ebx + 50h]
        jmp    NEAR PTR loc_5_6905
loc_5_6863:
        or     al, al
        jne    NEAR PTR loc_5_6905
        cmp    byte ptr [esp + 13h], 2
        jne    SHORT loc_5_6890
        push   6
        add    edi, 4
        push   edi
        push   42h
        db  068h, 080h, 018h, 000h, 000h
        push   esi
        push   ebx
        db  0E8h, 0F9h, 099h, 0FAh, 0FFh
        add    esp, 18h
        dec    word ptr [ebx + 50h]
        jmp    SHORT loc_5_6905
        db  090h
loc_5_6890:
        mov    edx, dword ptr [esi + 8]
        mov    dword ptr [esp + 18h], edx
        mov    al, byte ptr [esi + 4]
        or     al, al
        jne    SHORT loc_5_68B0
        mov    word ptr [esi + 1ah], 0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Bh, 0FFh
loc_5_68B0:
        cmp    al, 2
        jne    SHORT loc_5_68D9
        cmp    dword ptr [ebx + 24h], 0ee6bh
        jb     SHORT loc_5_68C2
        call   sub_5_A7B0
loc_5_68C2:
        mov    eax, dword ptr [esp + 1ch]
        mov    edx, esi
        cmp    dword ptr [eax + 4ch], 0
        je     SHORT loc_5_68D0
        sub    edx, edx
loc_5_68D0:
        mov    ecx, edi
        call   sub_5_66E0
        jmp    SHORT loc_5_68FE
loc_5_68D9:
        push   edx
        db  0E8h, 0BCh, 09Fh, 0FAh, 0FFh
        add    esp, 4
        cmp    byte ptr [edi + 10h], al
        je     SHORT loc_5_68FE
        push   6
        add    edi, 4
        push   edi
        push   42h
        db  068h, 084h, 018h, 000h, 000h
        push   esi
        push   ebx
        db  0E8h, 084h, 099h, 0FAh, 0FFh
        add    esp, 18h
loc_5_68FE:
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esi + 8], eax
loc_5_6905:
        cmp    byte ptr [esi + 4], 2
        je     SHORT loc_5_6920
        mov    word ptr [esi + 1ah], 0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_5_6920:
        cmp    byte ptr [esp + 13h], 2
        jne    SHORT loc_5_6965
        mov    cl, byte ptr [ebp + 1fh]
        cmp    cl, 3
        jae    SHORT loc_5_6965
        db  0A1h, 004h, 004h, 000h, 000h
        xor    edx, edx
        mov    dl, cl
        add    eax, dword ptr [esp + 14h]
        lea    ecx, [edx + edx*2]
        lea    ecx, [edx + ecx*4]
        lea    eax, [eax + ecx + 4ch]
        shl    edx, 6
        mov    byte ptr [eax + 1], 0
        mov    byte ptr [eax], 0
        mov    byte ptr [eax + 2], 0
        db  08Bh, 00Dh, 0E0h, 003h, 000h, 000h
        cmp    byte ptr [edx + ecx + 2bh], 0
        jne    SHORT loc_5_6965
        mov    byte ptr [eax + 0bh], 0
loc_5_6965:
        mov    word ptr [esi + 1ah], 0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_6980:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    ebx, dword ptr [edi + 20h]
        add    ebx, 4
        mov    eax, dword ptr [edi + 1ch]
        mov    ecx, dword ptr [ebx + 14h]
        mov    esi, dword ptr [eax + 10h]
        call   sub_5_A7B0
        push   6
        push   ebx
        push   3dh
        db  068h, 088h, 018h, 000h, 000h
        push   edi
        push   esi
        db  0E8h, 0D3h, 098h, 0FAh, 0FFh
        add    esp, 18h
        mov    word ptr [edi + 1ah], 0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_5_69C0:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 24h]
        mov    ebx, dword ptr [esi + 1ch]
        mov    ebp, dword ptr [esi + 20h]
        mov    eax, dword ptr [ebx + 10h]
        mov    edi, dword ptr [ebp + 18h]
        mov    dword ptr [esp + 18h], eax
        mov    edx, dword ptr [edi + 54h]
        test   dl, 2
        je     SHORT loc_5_69F6
        mov    word ptr [esi + 1ah], 1
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_5_69F6:
        and    edx, 20h
        mov    ecx, esi
        db  0E8h, 0D0h, 0D1h, 0FAh, 0FFh
        mov    edx, dword ptr [edi + 78h]
        lea    ecx, [edx + 4]
        mov    dword ptr [esp + 1ch], ecx
        xor    ecx, ecx
        mov    cx, word ptr [edx]
        add    ecx, edx
        xor    edx, edx
        mov    dl, byte ptr [esi + 4]
        cmp    edx, 7
        ja     SHORT loc_5_6A22
        db  0FFh, 024h, 095h, 0A8h, 06Bh, 000h, 000h
loc_5_6A22:
        mov    word ptr [esi + 1ah], 1
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_5_6A35:
        mov    ax, word ptr [edi + 6eh]
        mov    word ptr [esi + 28h], ax
        mov    ax, word ptr [edi + 6ch]
        mov    word ptr [esi + 2ah], ax
        sub    eax, eax
        mov    word ptr [esi + 18h], 0
        mov    word ptr [esi + 1ah], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_5_6A5B:
        mov    ecx, dword ptr [edi + 14h]
        cmp    ebp, ecx
        je     SHORT loc_5_6A7B
        mov    eax, dword ptr [ebp + 2ch]
        or     eax, 1
        mov    dword ptr [ebp + 2ch], eax
        or     ecx, ecx
        jne    SHORT loc_5_6A7B
        or     eax, 4
        mov    dword ptr [ebp + 2ch], eax
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [edi + 14h], eax
loc_5_6A7B:
        call   sub_5_AF00
        mov    ecx, eax
        mov    ax, word ptr [esi + 28h]
        mov    word ptr [edi + 6eh], ax
        mov    word ptr [ecx + 4fh], ax
        mov    ax, word ptr [esi + 2ah]
        mov    word ptr [edi + 6ch], ax
        mov    word ptr [ecx + 4dh], ax
        mov    edx, 1
        mov    dword ptr [ecx + 2ch], edi
        mov    byte ptr [ecx + 14h], 9
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ecx + 8], eax
        call   sub_5_BCF0
        mov    word ptr [esi + 1ah], 0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_5_6AC8:
        cmp    al, 0ffh
        je     SHORT loc_5_6B14
        xor    edx, edx
        mov    dl, al
        lea    eax, [esp + 16h]
        push   eax
        lea    eax, [esp + 18h]
        push   0ch
        push   eax
        lea    eax, [esp + 1eh]
        push   eax
        push   14h
        push   0
        push   ecx
        xor    eax, eax
        mov    ecx, dword ptr [esp + 38h]
        push   ecx
        push   4
        push   ebx
        mov    al, byte ptr [ebx + 20h]
        push   eax
        push   180h
        push   6
        push   edx
        push   edx
        push   6
        db  068h, 08Ch, 018h, 000h, 000h
        mov    eax, dword ptr [esp + 5ch]
        push   esi
        push   eax
        db  0E8h, 070h, 097h, 0FAh, 0FFh
        add    esp, 4ch
        jmp    SHORT loc_5_6B47
loc_5_6B14:
        mov    word ptr [esp + 12h], 0
        lea    eax, [esp + 16h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   18h
        push   ecx
        mov    ecx, dword ptr [esp + 2ch]
        push   ecx
        push   6
        push   ebx
        push   -1
        push   40h
        db  068h, 09Ch, 018h, 000h, 000h
        mov    eax, dword ptr [esp + 40h]
        push   esi
        push   eax
        db  0E8h, 03Bh, 097h, 0FAh, 0FFh
        add    esp, 30h
loc_5_6B47:
        or     al, al
        jne    SHORT loc_5_6B74
        cmp    byte ptr [esi + 4], 4
        jne    SHORT loc_5_6B62
        mov    word ptr [esi + 28h], 0
        mov    cx, word ptr [esp + 16h]
        mov    word ptr [esi + 2ah], cx
        jmp    SHORT loc_5_6B74
loc_5_6B62:
        mov    cx, word ptr [esp + 14h]
        mov    word ptr [esi + 2ah], cx
        mov    cx, word ptr [esp + 12h]
        mov    word ptr [esi + 28h], cx
loc_5_6B74:
        mov    ecx, eax
        db  0E8h, 0D5h, 0CFh, 0FAh, 0FFh
        mov    word ptr [esi + 1ah], ax
        movsx  eax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_6B90:
        sub    ecx, ecx
        db  0E8h, 0B9h, 0CFh, 0FAh, 0FFh
        mov    word ptr [esi + 1ah], ax
        movsx  eax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Bh, 0FFh
loc_5_6BA8:
        db  035h, 06Ah, 000h, 000h, 05Bh
        push   0
        db  000h, 022h
        push   0
        db  000h, 022h
        push   0
        db  000h, 0C8h
        push   0
        db  000h, 090h, 06Bh, 000h, 000h, 0C8h
        push   0
        db  000h, 090h, 06Bh, 000h, 000h, 08Dh
        and    al, 0
        lea    esp, [esp]
loc_5_6BD0:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    esi, dword ptr [edi + 20h]
        mov    eax, dword ptr [edi + 1ch]
        mov    ebx, dword ptr [eax + 10h]
        xor    eax, eax
        mov    al, byte ptr [edi + 4]
        or     eax, eax
        je     SHORT loc_5_6BF0
        cmp    eax, 1
        je     SHORT loc_5_6C00
        jmp    SHORT loc_5_6C08
loc_5_6BF0:
        mov    ecx, dword ptr [esi + 18h]
        call   sub_5_A9F0
        jmp    SHORT loc_5_6C08
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_6C00:
        mov    ecx, dword ptr [esi + 18h]
        call   sub_5_AA00
loc_5_6C08:
        mov    al, byte ptr [edi + 4]
        push   6
        mov    ecx, dword ptr [edi + 28h]
        push   ecx
        add    esi, 4
        mov    ecx, dword ptr [edi + 24h]
        push   ecx
        push   6
        push   esi
        cmp    al, 1
        sbb    ecx, ecx
        neg    ecx
        cmp    al, 1
        push   ecx
        sbb    eax, eax
        and    eax, 0fffffffch
        add    eax, 1eh
        push   eax
        db  068h, 0A8h, 018h, 000h, 000h
        push   edi
        push   ebx
        db  0E8h, 046h, 096h, 0FAh, 0FFh
        add    esp, 28h
        cmp    al, 0ffh
        jne    SHORT loc_5_6C50
        mov    word ptr [edi + 1ah], 21h
        mov    eax, 21h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_5_6C50:
        mov    ecx, eax
        db  0E8h, 0F9h, 0CEh, 0FAh, 0FFh
        mov    word ptr [edi + 1ah], ax
        movsx  eax, ax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_6C70:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 1ch]
        mov    eax, dword ptr [edi + 20h]
        mov    ebx, dword ptr [edi + 14h]
        mov    esi, dword ptr [eax + 18h]
        xor    eax, eax
        mov    al, byte ptr [edi + 4]
        or     eax, eax
        je     SHORT loc_5_6CA0
        mov    word ptr [edi + 1ah], 1
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Bh, 0FFh
loc_5_6CA0:
        mov    word ptr [esp + 14h], 0
        mov    word ptr [esp + 16h], 0
        mov    word ptr [esp + 10h], 0
        mov    ax, word ptr [esi + 70h]
        mov    word ptr [esp + 12h], ax
        mov    ax, word ptr [esi + 6eh]
        mov    word ptr [esp + 0ch], ax
        mov    ax, word ptr [esi + 6ch]
        mov    word ptr [esp + 0eh], ax
        xor    eax, eax
        mov    ax, word ptr [esi + 68h]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [esp + 14h]
        push   eax
        db  0E8h, 0EAh, 09Dh, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [ebx + 8], edx
        lea    ecx, [ebx + 4]
        mov    eax, dword ptr [esp + 10h]
        push   eax
        db  0E8h, 0D4h, 09Dh, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [ebx + 0ch], eax
        mov    dword ptr [ebx + 10h], edx
        lea    ecx, [ebx + 0ch]
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        db  0E8h, 0BEh, 09Dh, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [ebx + 14h], eax
        mov    dword ptr [ebx + 18h], edx
        lea    ecx, [ebx + 14h]
        mov    dword ptr [ebx + 1ch], 0
        mov    dword ptr [ebx + 20h], 0
        mov    eax, dword ptr [esi + 5ch]
        mov    dword ptr [ebx + 24h], eax
        sub    ecx, ecx
        mov    dword ptr [ebx + 28h], 0
        mov    dword ptr [ebx + 2ch], 0
        mov    dword ptr [ebx + 30h], esi
        db  0E8h, 00Ah, 0CEh, 0FAh, 0FFh
        mov    word ptr [edi + 1ah], ax
        movsx  eax, ax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_6D60:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 1ah], 1
        mov    eax, 1
        ret
loc_5_6D70:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 1ah], 2
        mov    eax, 2
        ret
sub_5_6D80:
        test   byte ptr [ecx + 42h], 4
        je     SHORT loc_5_6D93
        and    edx, 0ffh
        mov    eax, dword ptr [ecx + 44h]
        mov    eax, dword ptr [edx + eax]
        ret
loc_5_6D93:
        push   edx
        db  0E8h, 04Ah, 09Ch, 0FAh, 0FFh
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
sub_5_6DA0:
        sub    esp, 8
        push   esi
        push   edi
        mov    si, dx
        mov    edi, ecx
        db  0E8h, 0CCh, 09Bh, 0FAh, 0FFh
        mov    eax, dword ptr [eax + 4]
        mov    ecx, edi
        shr    ecx, 10h
        shl    ecx, 4
        add    eax, ecx
        and    edi, 0ffffh
        lea    ecx, [eax + edi]
        lea    eax, [esp + 0ch]
        push   eax
        lea    eax, [esp + 0eh]
        push   eax
        mov    eax, esi
        and    eax, 0ffffh
        push   eax
        push   ecx
        db  0E8h, 0C3h, 09Bh, 0FAh, 0FFh
        add    esp, 10h
        xor    eax, eax
        mov    ax, word ptr [esp + 0ah]
        shl    eax, 10h
        add    eax, dword ptr [esp + 0ch]
        pop    edi
        pop    esi
        add    esp, 8
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_6E00:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0E8h, 06Fh, 09Bh, 0FAh, 0FFh
        db  003h, 005h, 004h, 004h, 000h, 000h
        mov    dword ptr [esp + 20h], eax
        db  0E8h, 06Bh, 09Bh, 0FAh, 0FFh
        mov    eax, dword ptr [esp + 30h]
        mov    ax, word ptr [eax + 1ch]
        and    eax, 0ffffh
        cmp    eax, 111dh
        je     SHORT loc_5_6E40
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Dh, 064h, 024h, 000h
loc_5_6E40:
        mov    eax, dword ptr [esp + 2ch]
        mov    eax, dword ptr [eax + 8]
        push   eax
        db  0E8h, 067h, 09Ah, 0FAh, 0FFh
        add    esp, 4
        or     al, al
        je     NEAR PTR loc_5_6FBD
        mov    eax, dword ptr [esp + 2ch]
        sub    esi, esi
        mov    eax, dword ptr [eax + 8]
        push   eax
        db  0E8h, 034h, 09Ah, 0FAh, 0FFh
        mov    byte ptr [esp + 17h], al
        add    esp, 4
        sub    edi, edi
        db  0A1h, 0D8h, 003h, 000h, 000h
        db  08Bh, 015h, 0DCh, 003h, 000h, 000h
        add    eax, 0
loc_5_6E80:
        cmp    dword ptr [edx + edi*4], 0
        je     SHORT loc_5_6EB6
        mov    cl, byte ptr [esp + 13h]
        cmp    byte ptr [edi + eax], cl
        jne    SHORT loc_5_6EB6
        lea    eax, [edi + 41h]
        mov    byte ptr [esp + 14h], al
        mov    byte ptr [esp + 15h], 3ah
        mov    byte ptr [esp + 16h], 0
        lea    ecx, [esp + 14h]
        sub    edx, edx
        call   sub_5_71F0
        db  0A1h, 0D8h, 003h, 000h, 000h
        db  08Bh, 015h, 0DCh, 003h, 000h, 000h
loc_5_6EB6:
        inc    edi
        cmp    edi, 20h
        jb     SHORT loc_5_6E80
        mov    eax, dword ptr [esp + 20h]
        cmp    byte ptr [eax + 49h], 0
        je     NEAR PTR loc_5_6F73
        mov    byte ptr [eax + 48h], 0
        xor    eax, eax
        mov    al, byte ptr [esp + 13h]
        sub    edi, edi
        mov    dword ptr [esp + 18h], eax
        db  08Dh, 004h, 085h, 0D0h, 009h, 000h, 000h
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [eax]
        inc    dword ptr [eax + 8]
        db  066h, 039h, 03Dh, 014h, 004h, 000h, 000h
        je     SHORT loc_5_6F55
        sub    ebx, ebx
        db  08Bh, 00Dh, 0A0h, 003h, 000h, 000h
        add    eax, 0
loc_5_6F00:
        lea    ebp, [ebx + ecx]
        cmp    byte ptr [ebp + 34h], 0
        je     SHORT loc_5_6F42
        test   byte ptr [ebp + 58h], 10h
        jne    SHORT loc_5_6F42
        call   sub_5_AF00
        mov    esi, eax
        mov    eax, dword ptr [esp + 18h]
        mov    byte ptr [esi + 14h], 3
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi + 18h], ebp
        mov    edx, 1
        mov    eax, dword ptr [esp + 1ch]
        inc    dword ptr [ebp + 2ch]
        mov    eax, dword ptr [eax]
        mov    ecx, esi
        inc    dword ptr [eax + 8]
        call   sub_5_BCF0
        db  08Bh, 00Dh, 0A0h, 003h, 000h, 000h
loc_5_6F42:
        add    ebx, 0a0h
        inc    edi
        xor    eax, eax
        db  066h, 0A1h, 014h, 004h, 000h, 000h
        cmp    eax, edi
        ja     SHORT loc_5_6F00
loc_5_6F55:
        mov    eax, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [ecx + 8]
        add    ecx, 8
        dec    eax
        mov    dword ptr [ecx], eax
        jne    SHORT loc_5_6F73
        mov    eax, dword ptr [esp + 18h]
        push   eax
        db  0E8h, 054h, 099h, 0FAh, 0FFh
        add    esp, 4
loc_5_6F73:
        db  0BFh, 0B8h, 014h, 000h, 000h
        db  08Bh, 00Dh, 0B8h, 014h, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_5_6FA9
loc_5_6F82:
        mov    eax, dword ptr [esp + 2ch]
        mov    eax, dword ptr [eax + 8]
        cmp    word ptr [ecx + 40h], ax
        jne    SHORT loc_5_6FA0
        cmp    dword ptr [ecx], 534552h
        je     SHORT loc_5_6FA0
        mov    edx, edi
        call   sub_5_2FB0
        jmp    SHORT loc_5_6FA3
loc_5_6FA0:
        lea    edi, [ecx + 4]
loc_5_6FA3:
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        jne    SHORT loc_5_6F82
loc_5_6FA9:
        or     esi, esi
        jne    SHORT loc_5_6FBD
        mov    eax, dword ptr [esp + 2ch]
        mov    eax, dword ptr [eax + 8]
        push   eax
        db  0E8h, 00Ah, 099h, 0FAh, 0FFh
        add    esp, 4
loc_5_6FBD:
        mov    eax, dword ptr [esp + 20h]
        pop    ebp
        mov    byte ptr [eax + 4ah], 0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  090h
sub_5_6FD0:
        cmp    byte ptr [ecx], 0
        je     SHORT loc_5_700E
loc_5_6FD5:
        cmp    byte ptr [ecx], 3ah
        je     SHORT loc_5_6FE1
        inc    ecx
        cmp    byte ptr [ecx], 0
        jne    SHORT loc_5_6FD5
        ret
loc_5_6FE1:
        mov    al, byte ptr [ecx + 1]
        cmp    al, 5ch
        jne    SHORT loc_5_6FF6
        cmp    byte ptr [ecx + 2], 0
        jne    SHORT loc_5_6FF2
        mov    byte ptr [ecx], 0
        ret
loc_5_6FF2:
        cmp    al, 5ch
        je     SHORT loc_5_7000
loc_5_6FF6:
        or     al, al
        je     SHORT loc_5_7000
        mov    byte ptr [ecx], 5ch
        ret
        db  08Bh, 0FFh
loc_5_7000:
        mov    dl, byte ptr [ecx + 1]
        lea    eax, [ecx + 1]
        mov    byte ptr [ecx], dl
        mov    ecx, eax
        or     dl, dl
        jne    SHORT loc_5_7000
loc_5_700E:
        ret
        db  090h
sub_5_7010:
        sub    esp, 48h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 10h], edx
        mov    ebx, ecx
        lea    edi, [esp + 24h]
        sub    eax, eax
        mov    ecx, 0dh
        rep stosd
        mov    edx, dword ptr [esp + 60h]
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [esp + 24h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        cmp    byte ptr [edx + 1], 3ah
        jne    SHORT loc_5_70C1
        db  0A1h, 0DCh, 003h, 000h, 000h
        movsx  ecx, byte ptr [edx]
        cmp    dword ptr [eax + ecx*4 - 104h], 0
        je     SHORT loc_5_70C1
        lea    eax, [esp + 24h]
        mov    dword ptr [esp + 14h], eax
        mov    dword ptr [esp + 1ch], 1
        mov    dword ptr [esp + 18h], 3
        lea    eax, [esp + 14h]
        push   eax
        db  0A1h, 0A0h, 005h, 000h, 000h
        push   eax
        push   0
        db  0E8h, 084h, 09Ah, 0FAh, 0FFh
        add    esp, 0ch
        or     al, al
        jne    NEAR PTR loc_5_71DC
        cmp    word ptr [ebx + 92h], 0
        je     SHORT loc_5_70C1
loc_5_70A8:
        inc    byte ptr [ebx + 37h]
        push   ebx
        db  0E8h, 033h, 09Ah, 0FAh, 0FFh
        add    esp, 4
        dec    byte ptr [ebx + 37h]
        cmp    word ptr [ebx + 92h], 0
        jne    SHORT loc_5_70A8
loc_5_70C1:
        push   1
        push   144h
        db  0E8h, 0F1h, 098h, 0FAh, 0FFh
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_5_70E2
        mov    al, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 48h
        ret    0ch
loc_5_70E2:
        db  0B8h, 0D0h, 018h, 000h, 000h
        mov    cx, word ptr [eax]
        mov    word ptr [ebp], cx
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ebp + 2], al
        mov    ecx, 0ffffffffh
        mov    edi, dword ptr [ebx + 3ch]
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        db  066h, 0A1h, 0D4h, 018h, 000h, 000h
        mov    word ptr [edi - 1], ax
        mov    edi, dword ptr [esp + 64h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ebp + ecx - 2]
        cmp    byte ptr [eax], 5ch
        jne    SHORT loc_5_717F
        mov    byte ptr [eax], 0
loc_5_717F:
        mov    dword ptr [esp + 2eh], ebp
        db  0C7h, 044h, 024h, 032h, 0D8h, 018h, 000h, 000h
        mov    al, byte ptr [esp + 5ch]
        and    eax, 0ffff00ffh
        mov    word ptr [esp + 38h], ax
        mov    word ptr [esp + 3eh], 1
        lea    eax, [esp + 24h]
        mov    dword ptr [esp + 14h], eax
        mov    dword ptr [esp + 1ch], 5
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esp + 18h], eax
        lea    eax, [esp + 14h]
        push   eax
        db  0A1h, 0A0h, 005h, 000h, 000h
        push   eax
        push   0
        db  0E8h, 041h, 099h, 0FAh, 0FFh
        add    esp, 0ch
        mov    bl, al
        push   0
        push   ebp
        db  0E8h, 0FAh, 097h, 0FAh, 0FFh
        add    esp, 8
        mov    al, bl
loc_5_71DC:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 48h
        ret    0ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_71F0:
        sub    esp, 10h
        mov    dword ptr [esp], ecx
        mov    dword ptr [esp + 8], 1
        or     edx, edx
        mov    dword ptr [esp + 4], 1
        jne    SHORT loc_5_7213
        mov    dword ptr [esp + 4], 3
loc_5_7213:
        lea    eax, [esp]
        push   eax
        db  0A1h, 0A0h, 005h, 000h, 000h
        push   eax
        push   0
        db  0E8h, 0F2h, 098h, 0FAh, 0FFh
        add    esp, 1ch
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_7230:
        push   esi
        push   edi
        mov    esi, ecx
        db  08Bh, 03Dh, 0A8h, 005h, 000h, 000h
        or     edi, edi
        je     SHORT loc_5_7270
        mov    edi, edi
loc_5_7240:
        cmp    dword ptr [edi + 10h], esi
        jne    SHORT loc_5_7269
        push   0ffh
        push   3
        mov    eax, dword ptr [edi + 8]
        push   eax
        db  0E8h, 0ECh, 098h, 0FAh, 0FFh
        add    esp, 0ch
        or     al, al
        jne    SHORT loc_5_7269
        db  08Bh, 03Dh, 0A8h, 005h, 000h, 000h
        or     edi, edi
        jne    SHORT loc_5_7240
        pop    edi
        pop    esi
        ret
loc_5_7269:
        mov    edi, dword ptr [edi + 4]
        or     edi, edi
        jne    SHORT loc_5_7240
loc_5_7270:
        pop    edi
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_7280:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, edx
        mov    esi, ecx
        mov    ecx, ebx
        shl    ecx, 6
        db  0A1h, 0E0h, 003h, 000h, 000h
        add    eax, ecx
        mov    dword ptr [esp + 14h], eax
        db  0A1h, 0E4h, 003h, 000h, 000h
        db  068h, 0CEh, 005h, 000h, 000h
        lea    eax, [eax + ebx*8]
        mov    dword ptr [esp + 20h], eax
        mov    ebp, dword ptr [eax]
        db  0E8h, 0DFh, 08Eh, 0FAh, 0FFh
        add    esp, 4
        db  0E8h, 0CBh, 096h, 0FAh, 0FFh
        mov    dword ptr [esp + 18h], eax
        mov    edi, eax
        lea    eax, [ebx + ebx*2]
        lea    ecx, [ebx + eax*4]
        mov    dword ptr [esp + 20h], ecx
        db  0A1h, 004h, 004h, 000h, 000h
loc_5_72D0:
        add    eax, edi
        mov    ecx, dword ptr [esp + 20h]
        lea    ebx, [eax + ecx + 4ch]
        test   byte ptr [edi], 10h
        jne    NEAR PTR loc_5_73AD
        or     ebp, ebp
        je     NEAR PTR loc_5_739E
        mov    eax, dword ptr [esp + 14h]
        cmp    byte ptr [eax + 2ah], 0
        je     NEAR PTR loc_5_739E
        mov    ecx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [esp + 10h], eax
        cmp    byte ptr [ebx + 2], 0
        je     SHORT loc_5_7372
        cmp    byte ptr [ebx + 0bh], 0
        je     NEAR PTR loc_5_7397
        mov    eax, dword ptr [esp + 1ch]
        mov    al, byte ptr [eax + 4]
        cmp    al, 2
        jne    SHORT loc_5_7346
        xor    eax, eax
        mov    al, byte ptr [ebx + 0ch]
        mov    dword ptr [esi + 8], eax
        xor    eax, eax
        mov    ax, word ptr [ebx + 3]
        push   eax
        mov    eax, dword ptr [ebp + 28h]
        push   eax
        push   6ah
        push   17h
        db  068h, 0DCh, 018h, 000h, 000h
        push   esi
        push   ecx
        db  0E8h, 03Eh, 08Fh, 0FAh, 0FFh
        add    esp, 1ch
        jmp    SHORT loc_5_7397
loc_5_7346:
        cmp    al, 1
        jne    SHORT loc_5_7397
        xor    eax, eax
        mov    al, byte ptr [ebx + 0ch]
        mov    dword ptr [esi + 8], eax
        xor    eax, eax
        mov    ax, word ptr [ebx + 3]
        push   eax
        mov    eax, dword ptr [ebp + 28h]
        push   eax
        push   69h
        push   17h
        db  068h, 0E0h, 018h, 000h, 000h
        push   esi
        push   ecx
        db  0E8h, 012h, 08Fh, 0FAh, 0FFh
        add    esp, 1ch
        jmp    SHORT loc_5_7397
loc_5_7372:
        cmp    byte ptr [ebx + 0bh], 0
        je     SHORT loc_5_7397
        xor    eax, eax
        mov    al, byte ptr [ebx + 0ch]
        push   6
        mov    dword ptr [esi + 8], eax
        lea    eax, [ebx + 5]
        push   eax
        push   42h
        db  068h, 0E4h, 018h, 000h, 000h
        push   esi
        push   ecx
        db  0E8h, 0EBh, 08Eh, 0FAh, 0FFh
        add    esp, 18h
loc_5_7397:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi + 8], eax
loc_5_739E:
        mov    byte ptr [ebx], 0
        mov    byte ptr [ebx + 1], 0
        mov    byte ptr [ebx + 2], 0
        mov    byte ptr [ebx + 0bh], 0
loc_5_73AD:
        push   edi
        db  0E8h, 0DEh, 095h, 0FAh, 0FFh
        add    esp, 4
        mov    edi, eax
        cmp    edi, dword ptr [esp + 18h]
        db  0A1h, 004h, 004h, 000h, 000h
        jne    NEAR PTR loc_5_72D0
        db  068h, 0CEh, 005h, 000h, 000h
        db  0E8h, 0D8h, 08Dh, 0FAh, 0FFh
        mov    eax, dword ptr [esp + 18h]
        add    esp, 4
        mov    byte ptr [eax + 2ah], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_73F0:
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 3ch]
        mov    eax, dword ptr [esp + 40h]
        mov    edi, dword ptr [ebp + 1ch]
        mov    ax, word ptr [eax + 1ch]
        and    eax, 0ffffh
        cmp    eax, 1
        je     SHORT loc_5_7441
        cmp    eax, 2
        je     NEAR PTR loc_5_75E0
        cmp    eax, 3
        je     NEAR PTR loc_5_785C
        cmp    eax, 4
        je     NEAR PTR loc_5_7700
        cmp    eax, 5
        je     NEAR PTR loc_5_7820
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
loc_5_7441:
        mov    al, byte ptr [ebp + 4]
        dec    al
        cmp    al, 20h
        jb     SHORT loc_5_7457
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
loc_5_7457:
        db  08Bh, 015h, 0A0h, 005h, 000h, 000h
        cmp    dword ptr [ebp + 10h], edx
        je     SHORT loc_5_7480
        and    eax, 0ffh
        db  08Bh, 015h, 0C8h, 003h, 000h, 000h
        mov    byte ptr [eax + edx], 80h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_5_7480:
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [esp + 20h], ecx
        mov    cl, byte ptr [ecx + 56h]
        cmp    cl, 0ffh
        mov    esi, eax
        jne    SHORT loc_5_74A3
        and    esi, 0ffh
        db  08Bh, 015h, 0C8h, 003h, 000h, 000h
        mov    byte ptr [esi + edx], 80h
        jmp    SHORT loc_5_74BE
loc_5_74A3:
        and    esi, 0ffh
        db  08Bh, 015h, 0C8h, 003h, 000h, 000h
        or     byte ptr [esi + edx], 1
        inc    cl
        db  08Bh, 015h, 0CCh, 003h, 000h, 000h
        mov    byte ptr [esi + edx], cl
loc_5_74BE:
        mov    byte ptr [edi + 1fh], al
        db  08Bh, 00Dh, 0DCh, 003h, 000h, 000h
        mov    dword ptr [ecx + esi*4], edi
        db  080h, 03Dh, 0C7h, 003h, 000h, 000h, 000h
        je     SHORT loc_5_7500
        mov    ecx, dword ptr [ebp + 24h]
        or     ecx, ecx
        db  0A1h, 0D4h, 003h, 000h, 000h
        jne    SHORT loc_5_74F0
        mov    byte ptr [esi + eax], 0ffh
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  08Dh, 049h, 000h
loc_5_74F0:
        mov    byte ptr [esi + eax], cl
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  08Dh, 049h, 000h
loc_5_7500:
        mov    eax, dword ptr [ebp + 24h]
        and    eax, 0f00h
        sub    eax, 200h
        cmp    eax, 1
        sbb    al, al
        and    al, 1
        add    al, 12h
        mov    byte ptr [esp + 13h], al
        cmp    al, 13h
        jne    SHORT loc_5_7527
        db  0E8h, 058h, 094h, 0FAh, 0FFh
        mov    dword ptr [esp + 28h], eax
loc_5_7527:
        db  068h, 0CEh, 005h, 000h, 000h
        db  0E8h, 061h, 08Ch, 0FAh, 0FFh
        add    esp, 4
        db  0E8h, 04Dh, 094h, 0FAh, 0FFh
        mov    dword ptr [esp + 1ch], eax
        mov    ebx, eax
        db  0A1h, 004h, 004h, 000h, 000h
loc_5_7544:
        lea    ecx, [ebx + eax]
        test   byte ptr [ebx], 10h
        jne    SHORT loc_5_75A1
        mov    eax, dword ptr [ecx]
        mov    byte ptr [esi + eax], 0
        cmp    byte ptr [esp + 13h], 13h
        jne    SHORT loc_5_755F
        cmp    dword ptr [esp + 28h], ebx
        jne    SHORT loc_5_75A1
loc_5_755F:
        mov    eax, dword ptr [ebp + 24h]
        or     eax, eax
        jne    SHORT loc_5_7570
        mov    eax, dword ptr [ecx + 4]
        mov    byte ptr [esi + eax], 0ffh
        jmp    SHORT loc_5_7576
        db  090h
loc_5_7570:
        mov    edx, dword ptr [ecx + 4]
        mov    byte ptr [esi + edx], al
loc_5_7576:
        mov    eax, dword ptr [ecx]
        add    eax, esi
        push   eax
        db  068h, 0E8h, 018h, 000h, 000h
        lea    eax, [esi + 41h]
        push   eax
        push   edi
        xor    eax, eax
        mov    al, byte ptr [esp + 23h]
        push   eax
        push   16h
        db  068h, 0ECh, 018h, 000h, 000h
        mov    ecx, dword ptr [esp + 3ch]
        push   ebp
        push   ecx
        db  0E8h, 0E1h, 08Ch, 0FAh, 0FFh
        add    esp, 24h
loc_5_75A1:
        push   ebx
        db  0E8h, 0EAh, 093h, 0FAh, 0FFh
        add    esp, 4
        mov    ebx, eax
        cmp    dword ptr [esp + 1ch], ebx
        db  0A1h, 004h, 004h, 000h, 000h
        jne    SHORT loc_5_7544
        db  068h, 0CEh, 005h, 000h, 000h
        db  0E8h, 0E8h, 08Bh, 0FAh, 0FFh
        add    esp, 4
        cmp    byte ptr [esp + 13h], 13h
        jne    SHORT loc_5_75D6
        mov    cl, byte ptr [ebp + 40h]
        db  0A1h, 0D8h, 003h, 000h, 000h
        mov    byte ptr [esi + eax], cl
loc_5_75D6:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
loc_5_75E0:
        mov    al, byte ptr [ebp + 4]
        dec    al
        cmp    al, 20h
        jb     SHORT loc_5_75F6
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
loc_5_75F6:
        mov    esi, eax
        db  08Bh, 00Dh, 0DCh, 003h, 000h, 000h
        and    esi, 0ffh
        cmp    dword ptr [ecx + esi*4], 0
        je     SHORT loc_5_7626
        call   sub_5_CCD0
        or     eax, eax
        db  08Bh, 00Dh, 0DCh, 003h, 000h, 000h
        jne    SHORT loc_5_7626
        mov    ecx, esi
        call   sub_5_CDE0
        db  08Bh, 00Dh, 0DCh, 003h, 000h, 000h
loc_5_7626:
        sub    al, al
        xor    edx, edx
        db  08Ah, 015h, 06Dh, 004h, 000h, 000h
        dec    edx
        cmp    edx, esi
        db  08Bh, 015h, 0C8h, 003h, 000h, 000h
        jle    SHORT loc_5_763D
        mov    al, 80h
loc_5_763D:
        mov    byte ptr [esi + edx], al
        db  08Bh, 015h, 0A0h, 005h, 000h, 000h
        cmp    dword ptr [ebp + 10h], edx
        jne    NEAR PTR loc_5_76E6
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esp + 24h], eax
        db  08Bh, 015h, 0CCh, 003h, 000h, 000h
        mov    byte ptr [esi + edx], 0
        mov    byte ptr [edi + 1fh], 0ffh
        mov    dword ptr [ecx + esi*4], 0
        db  068h, 0CEh, 005h, 000h, 000h
        db  0E8h, 01Dh, 08Bh, 0FAh, 0FFh
        add    esp, 4
        db  0E8h, 009h, 093h, 0FAh, 0FFh
        mov    dword ptr [esp + 1ch], eax
        mov    edi, eax
        db  0A1h, 004h, 004h, 000h, 000h
loc_5_7688:
        lea    ebx, [edi + eax]
        test   byte ptr [edi], 10h
        jne    SHORT loc_5_76C3
        mov    eax, dword ptr [ebx]
        mov    al, byte ptr [esi + eax]
        or     al, al
        je     SHORT loc_5_76B6
        and    eax, 0ffh
        push   eax
        push   14h
        push   16h
        db  068h, 0F4h, 018h, 000h, 000h
        mov    eax, dword ptr [esp + 34h]
        push   ebp
        push   eax
        db  0E8h, 0CCh, 08Bh, 0FAh, 0FFh
        add    esp, 18h
loc_5_76B6:
        mov    eax, dword ptr [ebx]
        mov    byte ptr [esi + eax], 0
        mov    eax, dword ptr [ebx + 4]
        mov    byte ptr [esi + eax], 0
loc_5_76C3:
        push   edi
        db  0E8h, 0C8h, 092h, 0FAh, 0FFh
        add    esp, 4
        mov    edi, eax
        cmp    dword ptr [esp + 1ch], edi
        db  0A1h, 004h, 004h, 000h, 000h
        jne    SHORT loc_5_7688
        db  068h, 0CEh, 005h, 000h, 000h
        db  0E8h, 0C6h, 08Ah, 0FAh, 0FFh
        add    esp, 4
loc_5_76E6:
        db  0A1h, 0D8h, 003h, 000h, 000h
        mov    byte ptr [esi + eax], 0
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  08Dh, 064h, 024h, 000h
loc_5_7700:
        mov    bl, byte ptr [ebp + 4]
        db  0E8h, 073h, 092h, 0FAh, 0FFh
        mov    ebp, ebx
        and    ebp, 0ffh
        db  003h, 005h, 004h, 004h, 000h, 000h
        lea    ecx, [ebp + ebp*2]
        lea    ecx, [ebp + ecx*4]
        lea    ecx, [eax + ecx + 4ch]
        mov    edx, ebp
        shl    edx, 6
        db  003h, 015h, 0E0h, 003h, 000h, 000h
        mov    esi, dword ptr [edi + 10h]
        cmp    bl, 3
        jae    NEAR PTR loc_5_785C
        mov    byte ptr [edi + 1fh], bl
        db  0A1h, 0E4h, 003h, 000h, 000h
        lea    eax, [eax + ebp*8]
        mov    dword ptr [eax], edi
        cmp    byte ptr [edi + 22h], 0
        jne    NEAR PTR loc_5_785C
        mov    byte ptr [eax + 4], 1
        mov    byte ptr [edi + 22h], 0ffh
        mov    byte ptr [edx + 1], 80h
        mov    byte ptr [edx + 2], 8
        mov    byte ptr [edx + 3], 0
        mov    byte ptr [edx + 4], 1
        mov    byte ptr [edx + 5], 0
        db  0B8h, 0F8h, 018h, 000h, 000h
        lea    ebx, [edx + 7]
        mov    ebp, dword ptr [eax]
        mov    dword ptr [ebx], ebp
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [ebx + 4], al
        mov    word ptr [edx + 16h], 32ah
        mov    byte ptr [edx + 18h], 0
        mov    word ptr [esp + 1ah], 42h
        mov    ax, word ptr [esp + 1ah]
        xchg   al, ah
        mov    word ptr [esp + 18h], ax
        mov    ax, word ptr [esp + 18h]
        mov    word ptr [edx + 19h], ax
        mov    word ptr [esp + 16h], 84h
        mov    ax, word ptr [esp + 16h]
        xchg   al, ah
        mov    word ptr [esp + 14h], ax
        mov    ax, word ptr [esp + 14h]
        mov    word ptr [edx + 1bh], ax
        db  0B8h, 000h, 019h, 000h, 000h
        lea    ebx, [edx + 1dh]
        mov    ebp, dword ptr [eax]
        mov    dword ptr [ebx], ebp
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebx + 4], eax
        mov    byte ptr [edx + 2bh], 0
        mov    al, byte ptr [esi + 56h]
        inc    al
        mov    byte ptr [edx + 35h], al
        mov    byte ptr [edx + 37h], 0ffh
        mov    eax, dword ptr [edi + 28h]
        mov    dword ptr [esp + 30h], eax
        mov    eax, dword ptr [esp + 30h]
        xchg   ah, al
        rol    eax, 10h
        xchg   ah, al
        mov    dword ptr [esp + 2ch], eax
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [edx + 39h], eax
        mov    byte ptr [edx + 2ah], 0ffh
        mov    byte ptr [ecx], 0
        mov    byte ptr [ecx + 1], 0
        mov    byte ptr [ecx + 2], 0
        mov    byte ptr [ecx + 0bh], 0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  08Dh, 064h, 024h, 000h
loc_5_7820:
        mov    al, byte ptr [ebp + 4]
        cmp    al, 3
        jae    SHORT loc_5_785C
        mov    esi, eax
        and    esi, 0ffh
        mov    edx, esi
        mov    ecx, ebp
        call   sub_5_7280
        mov    ecx, esi
        shl    ecx, 6
        db  0A1h, 0E0h, 003h, 000h, 000h
        add    eax, ecx
        mov    byte ptr [eax + 2bh], 0
        mov    byte ptr [eax + 37h], 0
        db  0A1h, 0E4h, 003h, 000h, 000h
        mov    dword ptr [eax + esi*8], 0
        mov    byte ptr [edi + 1fh], 0ffh
loc_5_785C:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_7870:
        mov    eax, dword ptr [esp + 0ch]
        mov    byte ptr [eax + 1ch], 0
        sub    eax, eax
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_5_7880:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   esi
        push   edi
        db  0B8h, 008h, 019h, 000h, 000h
        lea    ecx, [ebp - 14h]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [ecx + 4], al
        lea    eax, [ebp - 0fh]
        mov    edx, 3c04h
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    word ptr [eax + 8], 0
        mov    byte ptr [eax + 0ah], 0
        mov    word ptr [ebp - 2], 0
        mov    word ptr [ebp - 4], 0
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, esi
        call   sub_5_6D80
        sub    dx, dx
loc_5_78D2:
        mov    cl, 7
        sub    cl, dl
        shl    cl, 2
        mov    edi, eax
        shr    edi, cl
        and    edi, 0fh
        db  08Ah, 08Fh, 0B0h, 018h, 000h, 000h
        mov    edi, edx
        and    edi, 0ffffh
        mov    byte ptr [ebp + edi - 10h], cl
        inc    dx
        cmp    dx, 8
        jb     SHORT loc_5_78D2
        mov    byte ptr [ebp - 8], 0
        xor    eax, eax
        mov    al, 21h
        push   eax
        mov    ah, 3dh
        mov    al, 12h
        lea    edx, [ebp - 14h]
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_5_7918
        mov    word ptr [ebp - 4], ax
        jmp    SHORT loc_5_791C
loc_5_7918:
        mov    word ptr [ebp - 2], ax
loc_5_791C:
        cmp    word ptr [ebp - 2], 0
        je     SHORT loc_5_7940
        mov    ecx, dword ptr [ebp + 10h]
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [ecx + 1ch], ax
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [esi + 1ah], ax
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_5_7940:
        mov    ecx, dword ptr [ebp + 10h]
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [ecx + 1ch], ax
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_7960:
        mov    ecx, dword ptr [esp + 0ch]
        xor    eax, eax
        mov    al, byte ptr [ecx + 1ch]
        cmp    eax, 3
        je     SHORT loc_5_7980
        cmp    eax, 4
        je     SHORT loc_5_7990
        cmp    eax, 6
        je     SHORT loc_5_79B0
        mov    eax, 1
        ret
        db  08Bh, 0FFh
loc_5_7980:
        db  0A1h, 0E8h, 003h, 000h, 000h
        mov    al, byte ptr [eax]
        mov    byte ptr [ecx + 1ch], al
        sub    eax, eax
        ret
        db  08Dh, 049h, 000h
loc_5_7990:
        lea    eax, [ecx + 10h]
        push   eax
        add    ecx, 38h
        push   ecx
        push   1
        db  0A1h, 0E8h, 003h, 000h, 000h
        push   eax
        db  0E8h, 0FBh, 08Fh, 0FAh, 0FFh
        add    esp, 10h
        sub    eax, eax
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_5_79B0:
        mov    word ptr [ecx + 1ch], 2
        sub    eax, eax
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_79C0:
        sub    esp, 2ch
        push   esi
        push   edi
        lea    ecx, [esp + 8]
        mov    edi, dword ptr [esp + 3ch]
        mov    edx, edi
        call   sub_5_9820
        or     al, al
        jne    NEAR PTR loc_5_7B33
        cmp    dword ptr [esp + 0ch], 0
        je     NEAR PTR loc_5_7B33
        mov    esi, dword ptr [esp + 40h]
        cmp    byte ptr [esi + 1ch], 0
        jne    NEAR PTR loc_5_7AC6
        cmp    byte ptr [esp + 11h], 0
        je     SHORT loc_5_7A59
        mov    edx, dword ptr [edi + 0ch]
        xor    ecx, ecx
        mov    cx, word ptr [edx + 2]
        lea    eax, [esp + 14h]
        push   20h
        push   eax
        mov    eax, dword ptr [esp + 10h]
        push   0deh
        mov    ax, word ptr [eax]
        shr    ax, 1
        and    eax, 0ffffh
        add    eax, 5
        push   eax
        xor    eax, eax
        mov    ax, word ptr [edx + ecx]
        add    eax, ecx
        add    eax, edx
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        push   6
        xor    eax, eax
        mov    al, byte ptr [esp + 2ch]
        push   eax
        push   0ffffh
        push   40h
        db  068h, 010h, 019h, 000h, 000h
        mov    eax, dword ptr [esp + 38h]
        push   edi
        push   eax
        db  0E8h, 02Bh, 088h, 0FAh, 0FFh
        add    esp, 34h
        jmp    SHORT loc_5_7A9C
loc_5_7A59:
        mov    ecx, dword ptr [edi + 0ch]
        xor    edx, edx
        mov    dx, word ptr [ecx + 2]
        lea    eax, [esp + 14h]
        push   20h
        push   eax
        xor    eax, eax
        mov    ax, word ptr [ecx + edx]
        add    eax, edx
        add    eax, ecx
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   eax
        push   6
        xor    eax, eax
        mov    al, byte ptr [esp + 24h]
        push   eax
        push   0ffffh
        push   40h
        db  068h, 01Ch, 019h, 000h, 000h
        mov    eax, dword ptr [esp + 30h]
        push   edi
        push   eax
        db  0E8h, 0E6h, 087h, 0FAh, 0FFh
        add    esp, 2ch
loc_5_7A9C:
        or     al, al
        je     SHORT loc_5_7AB0
        mov    byte ptr [esi + 1ch], al
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 2ch
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_5_7AB0:
        mov    al, byte ptr [esp + 27h]
        mov    byte ptr [esi + 1ch], al
        mov    al, byte ptr [esp + 27h]
        mov    byte ptr [esi + 18h], al
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 2ch
        ret
loc_5_7AC6:
        mov    edx, dword ptr [edi + 0ch]
        xor    ecx, ecx
        mov    cx, word ptr [edx + 2]
        mov    eax, dword ptr [esp + 8]
        push   0deh
        mov    ax, word ptr [eax]
        shr    ax, 1
        and    eax, 0ffffh
        add    eax, 4
        push   eax
        xor    eax, eax
        mov    ax, word ptr [edx + ecx]
        add    eax, ecx
        add    eax, edx
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   eax
        push   6
        xor    eax, eax
        mov    al, byte ptr [esp + 24h]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 18h]
        push   eax
        push   4fh
        db  0B8h, 024h, 019h, 000h, 000h
        cmp    byte ptr [esp + 31h], 0
        jne    SHORT loc_5_7B19
        db  0B8h, 02Ch, 019h, 000h, 000h
loc_5_7B19:
        push   eax
        mov    eax, dword ptr [esp + 30h]
        push   edi
        push   eax
        db  0E8h, 05Ah, 087h, 0FAh, 0FFh
        add    esp, 2ch
        mov    byte ptr [esi + 1ch], al
        sub    eax, eax
        pop    edi
        pop    esi
        add    esp, 2ch
        ret
loc_5_7B33:
        mov    esi, dword ptr [esp + 40h]
        sub    eax, eax
        mov    byte ptr [esi + 1ch], 0ffh
        pop    edi
        pop    esi
        add    esp, 2ch
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_7B50:
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0E8h, 01Fh, 08Eh, 0FAh, 0FFh
        mov    dword ptr [esp + 28h], eax
        db  0A1h, 004h, 004h, 000h, 000h
        mov    ecx, dword ptr [esp + 28h]
        mov    esi, dword ptr [esp + 38h]
        add    ecx, eax
        mov    dl, byte ptr [esi + 1ch]
        mov    word ptr [esi + 1ch], 0
        db  0A0h, 0EEh, 003h, 000h, 000h
        mov    di, dx
        and    edi, 0ffff00ffh
        mov    ebx, edi
        and    ebx, 0ffffh
        cmp    ebx, 9
        ja     SHORT loc_5_7BA0
        db  0FFh, 024h, 09Dh, 060h, 07Fh, 000h, 000h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_7BA0:
        mov    byte ptr [esi + 1dh], 0b8h
        mov    byte ptr [esi + 1ch], dl
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_7BB4:
        mov    al, byte ptr [esi + 15h]
loc_5_7BB7:
        cmp    al, 3
        jb     NEAR PTR loc_5_7D98
        sub    di, 4800h
        mov    word ptr [esi + 1ch], di
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_7BD5:
        mov    al, byte ptr [esi + 15h]
loc_5_7BD8:
        cmp    al, 3
        jb     NEAR PTR loc_5_7E65
        sub    di, 4800h
        mov    word ptr [esi + 1ch], di
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_7BF6:
        mov    byte ptr [esi + 15h], al
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_7C03:
        mov    al, byte ptr [esi + 15h]
        cmp    al, 3
        jae    SHORT loc_5_7C19
        db  0A2h, 0EEh, 003h, 000h, 000h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_7C19:
        mov    byte ptr [esi + 1ch], 0ffh
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_7C27:
        mov    al, byte ptr [esi + 15h]
        cmp    al, 3
        jae    SHORT loc_5_7C90
        xor    edx, edx
        mov    dl, al
        db  08Bh, 03Dh, 0E0h, 003h, 000h, 000h
        mov    eax, edx
        shl    eax, 6
        add    edi, eax
        lea    eax, [edx + edx*2]
        lea    eax, [edx + eax*4]
        lea    eax, [ecx + eax + 4ch]
        cmp    byte ptr [edi + 37h], 0
        jne    SHORT loc_5_7C82
        mov    byte ptr [edi + 37h], 0ffh
        mov    byte ptr [eax + 2], 0
        mov    byte ptr [edi + 2bh], 0
        xor    ecx, ecx
        mov    cx, word ptr [esi + 18h]
        xor    eax, eax
        mov    ax, word ptr [esi + 10h]
        shl    ecx, 10h
        or     ecx, eax
        mov    dword ptr [edi + 39h], ecx
        mov    ecx, dword ptr [esp + 34h]
        call   sub_5_3050
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_7C82:
        mov    byte ptr [esi + 1ch], 0ffh
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_7C90:
        mov    byte ptr [esi + 1ch], 0ffh
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Bh, 0FFh
loc_5_7CA0:
        mov    al, byte ptr [esi + 15h]
        cmp    al, 3
        jae    NEAR PTR loc_5_7D38
        xor    edx, edx
        mov    dl, al
        db  08Bh, 03Dh, 0E0h, 003h, 000h, 000h
        mov    eax, edx
        shl    eax, 6
        add    edi, eax
        lea    eax, [edx + edx*2]
        lea    eax, [edx + eax*4]
        lea    ebx, [ecx + eax + 4ch]
        cmp    byte ptr [edi + 37h], 0
        je     SHORT loc_5_7D2A
        mov    ax, word ptr [esi + 10h]
        mov    word ptr [esp + 1eh], ax
        mov    ax, word ptr [esp + 1eh]
        xchg   al, ah
        mov    word ptr [esp + 1ch], ax
        mov    ax, word ptr [esp + 1ch]
        mov    word ptr [ebx + 3], ax
        mov    byte ptr [ebx + 2], 0ffh
        mov    ax, word ptr [esi + 18h]
        mov    word ptr [ebx + 5], ax
        mov    ax, word ptr [esi + 4]
        mov    word ptr [ebx + 7], ax
        mov    ax, word ptr [esi]
        mov    word ptr [ebx + 9], ax
        mov    byte ptr [ebx + 0bh], 0ffh
        mov    eax, dword ptr [esp + 34h]
        mov    eax, dword ptr [eax + 8]
        push   eax
        db  0E8h, 085h, 08Bh, 0FAh, 0FFh
        add    esp, 4
        mov    byte ptr [ebx + 0ch], al
        mov    byte ptr [edi + 2bh], 0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_7D2A:
        mov    byte ptr [esi + 1ch], 0ffh
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_7D38:
        mov    byte ptr [esi + 1ch], 0ffh
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_7D46:
        mov    edx, 3810h
        mov    ecx, dword ptr [esp + 34h]
        call   sub_5_6D80
        push   0ch
        db  068h, 0F7h, 003h, 000h, 000h
        push   eax
        db  0E8h, 029h, 08Ah, 0FAh, 0FFh
        add    esp, 0ch
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Bh, 0FFh
loc_5_7D70:
        mov    edx, 3810h
        mov    ecx, dword ptr [esp + 34h]
        call   sub_5_6D80
        push   0ch
        push   eax
        db  068h, 0F7h, 003h, 000h, 000h
        db  0E8h, 0FFh, 089h, 0FAh, 0FFh
        add    esp, 0ch
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_7D98:
        xor    edx, edx
        mov    dl, al
        mov    eax, edx
        shl    eax, 6
        db  08Bh, 01Dh, 0E0h, 003h, 000h, 000h
        add    ebx, eax
        lea    eax, [edx + edx*2]
        lea    eax, [edx + eax*4]
        lea    eax, [eax + ecx + 4ch]
        mov    cl, byte ptr [eax]
        mov    byte ptr [ebx + 2ch], cl
        mov    cl, byte ptr [eax + 1]
        mov    byte ptr [ebx + 36h], cl
        mov    cl, byte ptr [eax + 2]
        mov    byte ptr [ebx + 38h], cl
        mov    ax, word ptr [eax + 3]
        mov    word ptr [esp + 1ah], ax
        mov    ax, word ptr [esp + 1ah]
        xchg   al, ah
        mov    word ptr [esp + 18h], ax
        mov    ax, word ptr [esp + 18h]
        mov    word ptr [ebx + 3dh], ax
        mov    edx, 3810h
        mov    ecx, dword ptr [esp + 34h]
        call   sub_5_6D80
        mov    dword ptr [esp + 20h], eax
        xor    ebp, ebp
        mov    bp, word ptr [esi + 18h]
        cmp    ebp, 40h
        jbe    SHORT loc_5_7E04
        mov    ebp, 40h
loc_5_7E04:
        mov    edi, dword ptr [esp + 20h]
        mov    esi, ebx
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [esp + 28h]
        test   byte ptr [ecx], 20h
        je     NEAR PTR loc_5_7F54
        cmp    ebp, 31h
        jb     SHORT loc_5_7E3E
        mov    edx, 44h
        mov    ecx, dword ptr [ebx + 2dh]
        call   sub_5_6DA0
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [ecx + 2dh], eax
loc_5_7E3E:
        cmp    ebp, 35h
        jb     NEAR PTR loc_5_7F54
        mov    edx, 18h
        mov    ecx, dword ptr [ebx + 31h]
        call   sub_5_6DA0
        mov    ecx, dword ptr [esp + 20h]
        pop    ebp
        mov    dword ptr [ecx + 31h], eax
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_7E65:
        and    eax, 0ffh
        mov    dword ptr [esp + 24h], eax
        shl    eax, 6
        db  08Bh, 02Dh, 0E0h, 003h, 000h, 000h
        add    ebp, eax
        mov    eax, dword ptr [esp + 24h]
        lea    edx, [eax + eax*2]
        lea    edx, [eax + edx*4]
        lea    ebx, [edx + ecx + 4ch]
        mov    edx, 3810h
        mov    ecx, dword ptr [esp + 34h]
        call   sub_5_6D80
        mov    dword ptr [esp + 20h], eax
        mov    al, byte ptr [ebx]
        mov    byte ptr [ebp + 2ch], al
        mov    al, byte ptr [ebx + 1]
        mov    byte ptr [ebp + 36h], al
        mov    al, byte ptr [ebx + 2]
        mov    byte ptr [ebp + 38h], al
        mov    ax, word ptr [ebx + 3]
        mov    word ptr [esp + 16h], ax
        mov    ax, word ptr [esp + 16h]
        xchg   al, ah
        mov    word ptr [esp + 14h], ax
        mov    ax, word ptr [esp + 14h]
        mov    word ptr [ebp + 3dh], ax
        xor    eax, eax
        mov    ax, word ptr [esi + 18h]
        cmp    eax, 40h
        jbe    SHORT loc_5_7ED8
        mov    eax, 40h
loc_5_7ED8:
        mov    edx, eax
        cmp    edx, 2dh
        jb     SHORT loc_5_7EE4
        mov    edx, 2dh
loc_5_7EE4:
        mov    edi, ebp
        mov    esi, dword ptr [esp + 20h]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        cmp    eax, 35h
        jbe    SHORT loc_5_7F18
        sub    eax, 35h
        mov    esi, dword ptr [esp + 20h]
        add    esi, 35h
        lea    edi, [ebp + 35h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_5_7F18:
        mov    al, byte ptr [ebp + 2ch]
        mov    byte ptr [ebx], al
        mov    al, byte ptr [ebp + 36h]
        mov    byte ptr [ebx + 1], al
        mov    al, byte ptr [ebp + 38h]
        mov    byte ptr [ebx + 2], al
        mov    ax, word ptr [ebp + 3dh]
        mov    word ptr [esp + 12h], ax
        mov    ax, word ptr [esp + 12h]
        xchg   al, ah
        mov    word ptr [esp + 10h], ax
        mov    ax, word ptr [esp + 10h]
        mov    word ptr [ebx + 3], ax
        mov    edx, dword ptr [esp + 24h]
        mov    ecx, dword ptr [esp + 34h]
        call   sub_5_3050
loc_5_7F54:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Bh, 0FFh
loc_5_7F60:
        mov    bh, 7bh
        add    byte ptr [eax], al
        fdivr  dword ptr [ebx]
        db  000h, 0B4h, 07Bh, 000h, 000h, 0D5h, 07Bh
        add    byte ptr [eax], al
        idiv   byte ptr [ebx]
        db  000h, 003h
        jl     SHORT loc_5_7F77
loc_5_7F77:
        db  000h, 027h
        jl     SHORT loc_5_7F7B
loc_5_7F7B:
        db  000h, 0A0h, 07Ch, 000h, 000h, 046h
        jge    SHORT loc_5_7F83
loc_5_7F83:
        db  000h, 070h, 07Dh
        add    byte ptr [eax], al
        lea    esp, [esp]
        lea    esp, [esp]
loc_5_7F90:
        db  0E8h, 05Eh, 089h, 0FAh, 0FFh
        mov    ecx, dword ptr [esp + 0ch]
        mov    word ptr [ecx + 14h], ax
        mov    word ptr [ecx + 38h], 0
        mov    word ptr [ecx], 0
        sub    eax, eax
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_5_7FB0:
        push   ebx
        db  0E8h, 0C5h, 089h, 0FAh, 0FFh
        db  08Bh, 00Dh, 004h, 004h, 000h, 000h
        add    ecx, eax
        mov    eax, dword ptr [esp + 10h]
        mov    dl, byte ptr [ecx + 49h]
        mov    bl, byte ptr [eax + 1ch]
        mov    byte ptr [ecx + 49h], bl
        mov    byte ptr [eax + 1ch], dl
        sub    eax, eax
        pop    ebx
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_7FE0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        mov    ax, word ptr [esi + 10h]
        mov    edi, dword ptr [esp + 10h]
        push   eax
        push   edi
        db  0E8h, 034h, 08Bh, 0FAh, 0FFh
        add    esp, 8
        or     ax, ax
        je     SHORT loc_5_8007
        sub    al, al
        mov    byte ptr [esi + 1ch], al
        sub    eax, eax
        pop    edi
        pop    esi
        ret
loc_5_8007:
        mov    eax, dword ptr [edi + 20h]
        mov    ecx, dword ptr [eax + 18h]
        call   sub_5_A9F0
        xor    eax, eax
        mov    ax, word ptr [esi + 8]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi + 4]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi + 14h]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi + 18h]
        push   eax
        push   6
        mov    eax, dword ptr [edi + 20h]
        add    eax, 4
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 1ch]
        push   eax
        push   1ah
        db  068h, 034h, 019h, 000h, 000h
        mov    eax, dword ptr [edi + 1ch]
        push   edi
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 028h, 082h, 0FAh, 0FFh
        add    esp, 30h
        mov    byte ptr [esi + 1ch], al
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_8070:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        mov    ax, word ptr [esi + 10h]
        mov    edi, dword ptr [esp + 10h]
        push   eax
        push   edi
        db  0E8h, 0A4h, 08Ah, 0FAh, 0FFh
        add    esp, 8
        or     ax, ax
        je     SHORT loc_5_8097
        sub    al, al
        mov    byte ptr [esi + 1ch], al
        sub    eax, eax
        pop    edi
        pop    esi
        ret
loc_5_8097:
        xor    eax, eax
        mov    ax, word ptr [esi]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi + 4]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi + 14h]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi + 18h]
        push   eax
        push   6
        mov    eax, dword ptr [edi + 20h]
        add    eax, 4
        push   eax
        mov    al, byte ptr [esi + 1dh]
        sub    al, 0bdh
        cmp    al, 1
        sbb    al, al
        and    al, 0feh
        add    al, 1eh
        and    eax, 0ffh
        push   eax
        db  068h, 040h, 019h, 000h, 000h
        mov    eax, dword ptr [edi + 1ch]
        push   edi
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 09Fh, 081h, 0FAh, 0FFh
        add    esp, 28h
        mov    byte ptr [esi + 1ch], al
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_5_80F0:
        push   ebx
        push   esi
        push   edi
        sub    al, al
        sub    bl, bl
        db  08Bh, 015h, 0ACh, 003h, 000h, 000h
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 18h]
loc_5_8105:
        xor    ecx, ecx
        mov    cl, bl
        mov    ecx, dword ptr [edx + ecx*4]
        or     ecx, ecx
        je     SHORT loc_5_813E
        cmp    byte ptr [ecx + 57h], 0
        je     SHORT loc_5_813A
        xor    eax, eax
        mov    ax, word ptr [edi + 8]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [edi + 1ch]
        push   eax
        push   1bh
        db  068h, 048h, 019h, 000h, 000h
        push   esi
        push   ecx
        db  0E8h, 04Eh, 081h, 0FAh, 0FFh
        add    esp, 18h
        db  08Bh, 015h, 0ACh, 003h, 000h, 000h
loc_5_813A:
        or     al, al
        jne    SHORT loc_5_8145
loc_5_813E:
        inc    bl
        cmp    bl, 8
        jb     SHORT loc_5_8105
loc_5_8145:
        mov    byte ptr [edi + 1ch], al
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_5_8150:
        push   ebx
        push   esi
        push   edi
        sub    bl, bl
        mov    esi, dword ptr [esp + 18h]
        db  08Bh, 015h, 0ACh, 003h, 000h, 000h
        mov    edi, dword ptr [esp + 14h]
loc_5_8163:
        xor    eax, eax
        mov    al, bl
        mov    ecx, dword ptr [edx + eax*4]
        or     ecx, ecx
        je     SHORT loc_5_819C
        cmp    byte ptr [ecx + 57h], 0
        je     SHORT loc_5_819C
        mov    al, byte ptr [esi + 1dh]
        sub    al, 0c3h
        cmp    al, 1
        sbb    al, al
        and    al, 0feh
        add    al, 1fh
        and    eax, 0ffh
        push   eax
        db  068h, 04Ch, 019h, 000h, 000h
        push   edi
        push   ecx
        db  0E8h, 0ECh, 080h, 0FAh, 0FFh
        add    esp, 10h
        db  08Bh, 015h, 0ACh, 003h, 000h, 000h
loc_5_819C:
        inc    bl
        cmp    bl, 8
        jb     SHORT loc_5_8163
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_81B0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    bl, 0ffh
        mov    esi, dword ptr [esp + 20h]
        mov    ecx, esi
        call   sub_5_2040
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_5_81D4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_81D4:
        mov    edi, dword ptr [esp + 24h]
        xor    eax, eax
        mov    ax, word ptr [edi + 14h]
        xor    ecx, ecx
        mov    cx, word ptr [edi + 18h]
        shl    eax, 10h
        add    eax, ecx
        mov    dword ptr [esp + 14h], eax
        mov    al, byte ptr [edi + 1ch]
        xor    edx, edx
        mov    dl, al
        or     edx, edx
        je     SHORT loc_5_8214
        cmp    edx, 1
        je     SHORT loc_5_8268
        cmp    edx, 2
        jl     NEAR PTR loc_5_82CC
        cmp    edx, 4
        jle    NEAR PTR loc_5_82A2
        jmp    NEAR PTR loc_5_82CC
loc_5_8214:
        mov    edx, 3c14h
        mov    ecx, esi
        call   sub_5_6D80
        lea    ecx, [esp + 12h]
        push   ecx
        lea    ecx, [esp + 18h]
        push   ecx
        xor    ecx, ecx
        mov    cl, byte ptr [eax]
        inc    ecx
        push   ecx
        push   eax
        xor    eax, eax
        mov    al, byte ptr [edi + 18h]
        push   eax
        push   0
        push   20h
        db  068h, 050h, 019h, 000h, 000h
        push   esi
        push   ebp
        db  0E8h, 038h, 080h, 0FAh, 0FFh
        add    esp, 28h
        mov    bl, al
        mov    eax, dword ptr [esp + 14h]
        shr    eax, 10h
        mov    word ptr [edi + 14h], ax
        mov    eax, dword ptr [esp + 14h]
        mov    word ptr [edi + 18h], ax
        mov    al, byte ptr [esp + 12h]
        mov    byte ptr [edi + 10h], al
        jmp    SHORT loc_5_82CC
loc_5_8268:
        lea    eax, [esp + 12h]
        push   eax
        lea    eax, [esp + 17h]
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        push   1
        push   20h
        db  068h, 058h, 019h, 000h, 000h
        push   esi
        push   ebp
        db  0E8h, 0F8h, 07Fh, 0FAh, 0FFh
        add    esp, 20h
        mov    bl, al
        mov    al, byte ptr [esp + 13h]
        and    eax, 0ffff00ffh
        mov    word ptr [edi + 18h], ax
        mov    al, byte ptr [esp + 12h]
        mov    byte ptr [edi + 14h], al
        jmp    SHORT loc_5_82CC
loc_5_82A2:
        xor    ecx, ecx
        mov    cx, word ptr [edi + 8]
        push   ecx
        cmp    al, 2
        mov    ecx, dword ptr [esp + 18h]
        push   ecx
        push   edx
        push   20h
        db  0B9h, 060h, 019h, 000h, 000h
        je     SHORT loc_5_82BF
        db  0B9h, 068h, 019h, 000h, 000h
loc_5_82BF:
        push   ecx
        push   esi
        push   ebp
        db  0E8h, 0B8h, 07Fh, 0FAh, 0FFh
        add    esp, 1ch
        mov    bl, al
loc_5_82CC:
        mov    byte ptr [edi + 1ch], bl
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_82E0:
        db  0E8h, 096h, 086h, 0FAh, 0FFh
        db  08Bh, 00Dh, 004h, 004h, 000h, 000h
        add    ecx, eax
        mov    eax, dword ptr [esp + 0ch]
        mov    dl, byte ptr [eax + 1ch]
        cmp    dl, 2
        je     SHORT loc_5_82FC
        mov    byte ptr [ecx + 48h], dl
loc_5_82FC:
        mov    cl, byte ptr [ecx + 48h]
        mov    byte ptr [eax + 1ch], cl
        sub    eax, eax
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_8310:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 1ch]
        mov    edi, dword ptr [esp + 18h]
        mov    ecx, edi
        mov    bl, byte ptr [esi + 1ch]
        call   sub_5_2040
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_5_8333
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_8333:
        xor    ecx, ecx
        mov    cl, bl
        cmp    ecx, 8
        ja     SHORT loc_5_8343
        db  0FFh, 024h, 08Dh, 030h, 084h, 000h, 000h
loc_5_8343:
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_834D:
        mov    bl, 1
loc_5_834F:
        push   ebp
        call   sub_5_A6B0
        add    esp, 4
        xor    eax, eax
        mov    al, bl
        push   eax
        push   22h
        db  068h, 070h, 019h, 000h, 000h
        push   edi
        push   ebp
        db  0E8h, 014h, 07Fh, 0FAh, 0FFh
        add    esp, 14h
loc_5_836E:
        mov    byte ptr [esi + 1ch], al
        and    ax, 0ffh
        mov    word ptr [edi + 1ah], ax
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_8380:
        push   ebp
        call   sub_5_A6B0
        add    esp, 4
        lea    eax, [esi + 14h]
        push   eax
        lea    eax, [esi + 18h]
        push   eax
        push   2
        push   22h
        db  068h, 074h, 019h, 000h, 000h
        push   edi
        push   ebp
        db  0E8h, 0DEh, 07Eh, 0FAh, 0FFh
        add    esp, 1ch
        jmp    SHORT loc_5_836E
loc_5_83A6:
        push   22h
        db  068h, 06Ch, 019h, 000h, 000h
        push   edi
        push   ebp
        db  0E8h, 0CBh, 07Eh, 0FAh, 0FFh
        add    esp, 10h
        cmp    al, 0ffh
        jne    SHORT loc_5_83C6
        mov    byte ptr [esi + 1ch], 1
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_83C6:
        mov    byte ptr [esi + 1ch], al
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_83D0:
        xor    eax, eax
        mov    ax, word ptr [esi + 14h]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [esi + 18h]
        push   eax
        push   ecx
        push   22h
        db  068h, 07Ch, 019h, 000h, 000h
        push   edi
        push   ebp
        db  0E8h, 092h, 07Eh, 0FAh, 0FFh
        add    esp, 1ch
        jmp    NEAR PTR loc_5_836E
loc_5_83F5:
        lea    eax, [esi + 18h]
        push   eax
        push   ecx
        push   22h
        db  068h, 084h, 019h, 000h, 000h
        push   edi
        push   ebp
        db  0E8h, 077h, 07Eh, 0FAh, 0FFh
        add    esp, 18h
        jmp    NEAR PTR loc_5_836E
loc_5_8410:
        xor    eax, eax
        mov    ax, word ptr [esi + 18h]
        push   eax
        push   ecx
        push   22h
        db  068h, 08Ch, 019h, 000h, 000h
        push   edi
        push   ebp
        db  0E8h, 059h, 07Eh, 0FAh, 0FFh
        add    esp, 18h
        jmp    NEAR PTR loc_5_836E
        db  08Bh, 0FFh
loc_5_8430:
        dec    ebp
        add    dword ptr [eax], 0
        db  080h, 083h, 000h, 000h, 0A6h, 083h, 000h
        db  000h, 04Fh, 083h
        add    byte ptr [eax], al
        db  0D0h, 083h, 000h, 000h, 0F5h, 083h
        add    byte ptr [eax], al
        db  010h, 084h, 000h, 000h, 0F5h, 083h, 000h
        db  000h, 010h
        test   byte ptr [eax], al
        add    byte ptr [0], al
        lea    esp, [esp]
loc_5_8460:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0E8h, 012h, 085h, 0FAh, 0FFh
        sub    cl, cl
        db  08Bh, 015h, 004h, 004h, 000h, 000h
        cmp    byte ptr [eax + edx + 48h], cl
        mov    ebp, dword ptr [esp + 1ch]
        jne    SHORT loc_5_848A
        cmp    byte ptr [ebp + 14h], 1
        cmc
        sbb    edi, edi
        and    edi, 0ffffh
        jmp    SHORT loc_5_848E
loc_5_848A:
        mov    di, word ptr [ebp + 8]
loc_5_848E:
        sub    bl, bl
        mov    esi, dword ptr [esp + 18h]
        db  0A1h, 0ACh, 003h, 000h, 000h
loc_5_8499:
        xor    edx, edx
        mov    dl, bl
        mov    edx, dword ptr [eax + edx*4]
        or     edx, edx
        je     SHORT loc_5_84CE
        cmp    byte ptr [edx + 57h], 0
        je     SHORT loc_5_84CA
        mov    eax, edi
        and    eax, 0ffffh
        push   eax
        push   4
        db  068h, 090h, 019h, 000h, 000h
        push   esi
        push   edx
        db  0E8h, 0BFh, 07Dh, 0FAh, 0FFh
        add    esp, 14h
        mov    cl, al
        db  0A1h, 0ACh, 003h, 000h, 000h
loc_5_84CA:
        or     cl, cl
        jne    SHORT loc_5_84D5
loc_5_84CE:
        inc    bl
        cmp    bl, 8
        jb     SHORT loc_5_8499
loc_5_84D5:
        mov    byte ptr [ebp + 1ch], cl
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_5_84E0:
        push   ebx
        push   esi
        push   edi
        sub    bl, bl
        db  0A1h, 0ACh, 003h, 000h, 000h
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 18h]
loc_5_84F2:
        xor    ecx, ecx
        mov    cl, bl
        mov    ecx, dword ptr [eax + ecx*4]
        or     ecx, ecx
        je     SHORT loc_5_8522
        cmp    byte ptr [ecx + 57h], 0
        je     SHORT loc_5_8522
        mov    al, byte ptr [edi + 1dh]
        add    al, 39h
        and    eax, 0ffh
        push   eax
        db  068h, 094h, 019h, 000h, 000h
        push   esi
        push   ecx
        db  0E8h, 065h, 07Dh, 0FAh, 0FFh
        add    esp, 10h
        db  0A1h, 0ACh, 003h, 000h, 000h
loc_5_8522:
        inc    bl
        cmp    bl, 8
        jb     SHORT loc_5_84F2
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_5_8530:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    byte ptr [esp + 13h], 0
        sub    bx, bx
        db  0E8h, 037h, 084h, 0FAh, 0FFh
        db  08Bh, 035h, 004h, 004h, 000h, 000h
        add    esi, eax
        mov    ecx, dword ptr [esp + 1ch]
        call   sub_5_2040
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_5_8565
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_5_8565:
        cmp    byte ptr [esi + 48h], 0
        mov    esi, dword ptr [esp + 20h]
        jbe    SHORT loc_5_857A
        mov    al, byte ptr [esi + 1ch]
        mov    byte ptr [esp + 13h], al
        mov    bx, word ptr [esi + 8]
loc_5_857A:
        mov    edx, 3c14h
        mov    ecx, dword ptr [esp + 1ch]
        call   sub_5_6D80
        mov    ebp, eax
        mov    ecx, edi
        call   sub_5_A9D0
        xor    eax, eax
        mov    al, byte ptr [ebp]
        inc    eax
        push   eax
        push   ebp
        mov    eax, ebx
        and    eax, 0ffffh
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esp + 1fh]
        push   eax
        push   9
        mov    eax, dword ptr [esp + 30h]
        db  068h, 098h, 019h, 000h, 000h
        push   eax
        push   edi
        db  0E8h, 0C5h, 07Ch, 0FAh, 0FFh
        add    esp, 20h
        mov    byte ptr [esi + 1ch], al
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_85D0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    byte ptr [esp + 12h], 0
        mov    byte ptr [esp + 13h], 0
        db  0E8h, 095h, 083h, 0FAh, 0FFh
        db  08Bh, 00Dh, 004h, 004h, 000h, 000h
        cmp    byte ptr [eax + ecx + 48h], 0
        mov    esi, dword ptr [esp + 20h]
        jbe    SHORT loc_5_8604
        mov    al, byte ptr [esi + 1ch]
        mov    byte ptr [esp + 13h], al
        mov    di, word ptr [esi + 8]
        jmp    SHORT loc_5_8611
loc_5_8604:
        cmp    byte ptr [esi + 14h], 1
        cmc
        sbb    edi, edi
        and    edi, 0ffffh
loc_5_8611:
        sub    bl, bl
        db  08Bh, 015h, 0ACh, 003h, 000h, 000h
        mov    ebp, dword ptr [esp + 1ch]
loc_5_8620:
        xor    eax, eax
        mov    al, bl
        mov    ecx, dword ptr [edx + eax*4]
        or     ecx, ecx
        je     SHORT loc_5_8662
        cmp    byte ptr [ecx + 57h], 0
        je     SHORT loc_5_865B
        mov    eax, edi
        and    eax, 0ffffh
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esp + 17h]
        push   eax
        push   0ah
        db  068h, 0A0h, 019h, 000h, 000h
        push   ebp
        push   ecx
        db  0E8h, 031h, 07Ch, 0FAh, 0FFh
        mov    byte ptr [esp + 2ah], al
        add    esp, 18h
        db  08Bh, 015h, 0ACh, 003h, 000h, 000h
loc_5_865B:
        cmp    byte ptr [esp + 12h], 0
        jne    SHORT loc_5_8669
loc_5_8662:
        inc    bl
        cmp    bl, 8
        jb     SHORT loc_5_8620
loc_5_8669:
        mov    al, byte ptr [esp + 12h]
        mov    byte ptr [esi + 1ch], al
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_8680:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        mov    ecx, esi
        call   sub_5_2040
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_5_86A0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_86A0:
        mov    edx, 3c14h
        mov    ecx, esi
        call   sub_5_6D80
        xor    ecx, ecx
        mov    cl, byte ptr [eax]
        inc    ecx
        push   ecx
        mov    ebx, dword ptr [esp + 1ch]
        push   eax
        mov    al, byte ptr [ebx + 1dh]
        sub    al, 0d2h
        cmp    al, 1
        sbb    al, al
        and    al, 1
        add    al, 0bh
        and    eax, 0ffh
        push   eax
        db  068h, 0A4h, 019h, 000h, 000h
        push   esi
        push   edi
        db  0E8h, 0A9h, 07Bh, 0FAh, 0FFh
        add    esp, 18h
        mov    byte ptr [ebx + 1ch], al
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_86F0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    bl, bl
        db  08Bh, 015h, 0ACh, 003h, 000h, 000h
        mov    esi, dword ptr [esp + 18h]
        mov    edi, dword ptr [esp + 1ch]
        mov    al, byte ptr [esp + 0fh]
loc_5_870A:
        xor    ecx, ecx
        mov    cl, bl
        mov    ecx, dword ptr [edx + ecx*4]
        or     ecx, ecx
        je     SHORT loc_5_8745
        cmp    byte ptr [ecx + 57h], 0
        je     SHORT loc_5_8741
        mov    al, byte ptr [edi + 1dh]
        sub    al, 0d3h
        cmp    al, 1
        sbb    al, al
        add    al, 0eh
        and    eax, 0ffh
        push   eax
        db  068h, 0A8h, 019h, 000h, 000h
        push   esi
        push   ecx
        db  0E8h, 047h, 07Bh, 0FAh, 0FFh
        add    esp, 10h
        db  08Bh, 015h, 0ACh, 003h, 000h, 000h
loc_5_8741:
        or     al, al
        jne    SHORT loc_5_874C
loc_5_8745:
        inc    bl
        cmp    bl, 8
        jb     SHORT loc_5_870A
loc_5_874C:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_8760:
        mov    eax, 1
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_8770:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  0A0h, 05Eh, 004h, 000h, 000h
        sub    ebx, ebx
        mov    byte ptr [esp + 0fh], al
        db  0A0h, 05Fh, 004h, 000h, 000h
        db  0A2h, 05Eh, 004h, 000h, 000h
        db  066h, 039h, 01Dh, 014h, 004h, 000h, 000h
        je     SHORT loc_5_87E0
        sub    esi, esi
        db  08Bh, 015h, 0A0h, 003h, 000h, 000h
        mov    edi, dword ptr [esp + 18h]
loc_5_87A0:
        lea    ecx, [esi + edx]
        cmp    byte ptr [ecx + 34h], 0
        je     SHORT loc_5_87C7
        cmp    byte ptr [ecx + 5bh], 0
        jne    SHORT loc_5_87C7
        mov    byte ptr [ecx + 54h], al
        push   edi
        push   ecx
        call   sub_5_16D0
        add    esp, 8
        db  0A0h, 05Fh, 004h, 000h, 000h
        db  08Bh, 015h, 0A0h, 003h, 000h, 000h
loc_5_87C7:
        add    esi, 0a0h
        inc    ebx
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 014h, 004h, 000h, 000h
        cmp    ecx, ebx
        ja     SHORT loc_5_87A0
        jmp    SHORT loc_5_87E4
        db  08Dh, 049h, 000h
loc_5_87E0:
        mov    edi, dword ptr [esp + 18h]
loc_5_87E4:
        mov    al, byte ptr [esp + 0fh]
        db  0A2h, 05Eh, 004h, 000h, 000h
        mov    word ptr [edi + 1ah], 0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h
loc_5_8800:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 14h]
        mov    ecx, edi
        call   sub_5_2040
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_5_8820
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_8820:
        mov    edx, 3800h
        mov    ecx, edi
        call   sub_5_6D80
        push   1ch
        mov    ebx, dword ptr [esp + 1ch]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebx + 14h]
        push   eax
        push   12h
        db  068h, 0ACh, 019h, 000h, 000h
        push   edi
        push   esi
        db  0E8h, 038h, 07Ah, 0FAh, 0FFh
        add    esp, 1ch
        mov    byte ptr [ebx + 1ch], al
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_8860:
        db  08Ah, 00Dh, 06Dh, 004h, 000h, 000h
        or     cl, cl
        je     SHORT loc_5_8876
        mov    eax, dword ptr [esp + 0ch]
        dec    cl
        mov    byte ptr [eax + 1ch], cl
        sub    eax, eax
        ret
loc_5_8876:
        sub    dl, dl
        db  0A1h, 0C8h, 003h, 000h, 000h
        lea    ecx, [ecx]
loc_5_8880:
        xor    ecx, ecx
        mov    cl, dl
        test   byte ptr [eax + ecx], 80h
        je     SHORT loc_5_8891
        inc    dl
        cmp    dl, 20h
        jb     SHORT loc_5_8880
loc_5_8891:
        mov    eax, dword ptr [esp + 0ch]
        mov    byte ptr [eax + 1ch], dl
        sub    eax, eax
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_5_88A0:
        push   ebx
        mov    ecx, dword ptr [esp + 0ch]
        call   sub_5_2040
        or     eax, eax
        jne    SHORT loc_5_88BA
        mov    eax, dword ptr [esp + 10h]
        pop    ebx
        mov    byte ptr [eax + 1ch], 0ffh
        sub    eax, eax
        ret
loc_5_88BA:
        mov    dl, byte ptr [eax + 73h]
        mov    cl, byte ptr [eax + 74h]
        mov    eax, dword ptr [esp + 10h]
        mov    bl, dl
        mov    byte ptr [eax + 1ch], dl
        and    ebx, 0fh
        mov    byte ptr [eax + 1dh], cl
        db  08Ah, 09Bh, 0B0h, 018h, 000h, 000h
        mov    byte ptr [eax + 19h], bl
        or     cl, cl
        jne    SHORT loc_5_88F0
        shr    dl, 4
        xor    ecx, ecx
        mov    cl, dl
        pop    ebx
        db  08Ah, 089h, 0B0h, 018h, 000h, 000h
        mov    byte ptr [eax + 18h], cl
        sub    eax, eax
        ret
loc_5_88F0:
        mov    byte ptr [eax + 18h], 58h
        sub    eax, eax
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_5_8900:
        sub    eax, eax
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_8910:
        mov    ecx, dword ptr [esp + 8]
        call   sub_5_2040
        mov    eax, dword ptr [esp + 0ch]
        mov    cl, byte ptr [eax + 14h]
        cmp    cl, 4
        jne    SHORT loc_5_892B
        db  08Ah, 00Dh, 0EDh, 003h, 000h, 000h
loc_5_892B:
        db  088h, 00Dh, 0EDh, 003h, 000h, 000h
        mov    byte ptr [eax + 1ch], cl
        sub    eax, eax
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_8940:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0E8h, 02Fh, 080h, 0FAh, 0FFh
        db  08Bh, 03Dh, 004h, 004h, 000h, 000h
        add    edi, eax
        mov    ecx, dword ptr [esp + 28h]
        call   sub_5_2040
        mov    esi, dword ptr [esp + 2ch]
        mov    dword ptr [esp + 14h], eax
        cmp    byte ptr [esi + 14h], 4
        jb     SHORT loc_5_8970
        mov    bl, byte ptr [esi + 15h]
        jmp    SHORT loc_5_8976
loc_5_8970:
        db  08Ah, 01Dh, 0EEh, 003h, 000h, 000h
loc_5_8976:
        cmp    bl, 3
        jb     SHORT loc_5_8990
        mov    byte ptr [esi + 1ch], 0ffh
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_8990:
        mov    ebp, ebx
        and    ebp, 0ffh
        mov    ecx, ebp
        shl    ecx, 6
        db  0A1h, 0E0h, 003h, 000h, 000h
        add    eax, ecx
        mov    dword ptr [esp + 10h], eax
        lea    eax, [ebp + ebp*2]
        lea    eax, [ebp + eax*4]
        lea    edi, [edi + eax + 4ch]
        mov    byte ptr [esp + 18h], 4ch
        mov    byte ptr [esp + 19h], 50h
        mov    byte ptr [esp + 1ah], 54h
        lea    eax, [ebx + 31h]
        mov    byte ptr [esp + 1bh], al
        mov    byte ptr [esp + 1ch], 0
        xor    eax, eax
        mov    al, byte ptr [esi + 14h]
        cmp    eax, 7
        ja     SHORT loc_5_89E0
        db  0FFh, 024h, 085h, 010h, 08Bh, 000h, 000h
loc_5_89E0:
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_5_89F0:
        db  0A1h, 0E4h, 003h, 000h, 000h
        cmp    dword ptr [eax + ebp*8], 0
        jne    NEAR PTR loc_5_8A96
        cmp    dword ptr [esp + 14h], 0
        je     NEAR PTR loc_5_8A96
        lea    eax, [esp + 18h]
        db  068h, 0B4h, 019h, 000h, 000h
        push   eax
        push   1
        sub    edx, edx
        mov    ecx, dword ptr [esp + 20h]
        call   sub_5_7010
        or     al, al
        mov    eax, dword ptr [esp + 28h]
        jne    SHORT loc_5_8A82
        mov    eax, dword ptr [eax + 1ch]
        mov    byte ptr [eax + 1fh], bl
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [eax + 2ah], 0ffh
        cmp    byte ptr [eax + 2bh], 0
        db  0A1h, 0E4h, 003h, 000h, 000h
        je     SHORT loc_5_8A50
        mov    byte ptr [eax + ebp*8 + 4], 0
        jmp    SHORT loc_5_8A55
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_8A50:
        mov    byte ptr [eax + ebp*8 + 4], 1
loc_5_8A55:
        mov    eax, dword ptr [esp + 14h]
        mov    byte ptr [edi + 1], 0
        mov    cl, byte ptr [eax + 56h]
        mov    eax, dword ptr [esp + 10h]
        inc    cl
        mov    byte ptr [eax + 35h], cl
        mov    edx, ebp
        mov    ecx, dword ptr [esp + 28h]
        call   sub_5_3050
        mov    byte ptr [esi + 1ch], 0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_5_8A82:
        mov    word ptr [eax + 1ah], 0
        mov    byte ptr [esi + 1ch], 0ffh
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_5_8A96:
        mov    byte ptr [esi + 1ch], 0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_5_8AA4:
        db  0A1h, 0E4h, 003h, 000h, 000h
        mov    byte ptr [eax + ebp*8 + 4], 2
        jmp    SHORT loc_5_8AC7
loc_5_8AB0:
        mov    byte ptr [edi + 1], 0
        mov    byte ptr [edi + 2], 0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_5_8AC2:
        db  0A1h, 0E4h, 003h, 000h, 000h
loc_5_8AC7:
        cmp    dword ptr [eax + ebp*8], 0
        je     SHORT loc_5_8B00
        lea    ecx, [esp + 18h]
        sub    edx, edx
        call   sub_5_71F0
        mov    eax, dword ptr [esp + 28h]
        cmp    word ptr [eax + 1ah], 1
        cmc
        sbb    al, al
        mov    byte ptr [esi + 1ch], al
        mov    eax, dword ptr [esp + 28h]
        pop    ebp
        mov    word ptr [eax + 1ah], 0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_5_8B00:
        mov    byte ptr [esi + 1ch], 0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Bh, 0FFh, 0F0h, 089h, 000h, 000h, 0C2h, 08Ah, 000h, 000h, 0A4h, 08Ah, 000h, 000h, 0B0h, 08Ah
        db  000h, 000h, 0F0h, 089h, 000h, 000h, 0C2h, 08Ah, 000h, 000h, 0A4h, 08Ah, 000h, 000h, 0B0h, 08Ah
        db  000h, 000h
loc_5_8B30:
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 4ch]
        mov    byte ptr [ecx + 1ch], 0ffh
        mov    ecx, dword ptr [esp + 48h]
        call   sub_5_2040
        mov    dword ptr [esp + 28h], eax
        or     eax, eax
        jne    SHORT loc_5_8B60
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_5_8B60:
        mov    edx, 3c04h
        mov    ecx, dword ptr [esp + 48h]
        call   sub_5_6D80
        mov    ebx, eax
        mov    edx, 3800h
        mov    ecx, dword ptr [esp + 48h]
        call   sub_5_6D80
        mov    ecx, dword ptr [esp + 4ch]
        mov    dword ptr [esp + 2ch], eax
        mov    al, byte ptr [ecx + 1dh]
        add    al, 34h
        mov    byte ptr [esp + 14h], al
        mov    al, byte ptr [ebx + 2]
        mov    byte ptr [esp + 13h], al
        cmp    byte ptr [esp + 14h], 17h
        mov    ax, word ptr [ebx]
        mov    word ptr [esp + 1ah], ax
        mov    eax, dword ptr [esp + 2ch]
        mov    ax, word ptr [eax]
        mov    word ptr [esp + 18h], ax
        jne    NEAR PTR loc_5_8C86
        cmp    byte ptr [esp + 13h], 0
        jne    NEAR PTR loc_5_8C86
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    eax, eax
        lea    esi, [ecx - 1]
        lea    ebp, [ebx + esi + 1]
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        mov    edx, dword ptr [esp + 28h]
        lea    edi, [ecx - 1]
        mov    eax, dword ptr [edx + 88h]
        or     eax, eax
        je     SHORT loc_5_8BFD
        push   0
        push   eax
        db  0E8h, 0D7h, 07Dh, 0FAh, 0FFh
        add    esp, 8
loc_5_8BFD:
        push   1
        lea    eax, [esi + edi]
        add    eax, 2
        push   eax
        db  0E8h, 0B3h, 07Dh, 0FAh, 0FFh
        mov    edx, dword ptr [esp + 30h]
        add    esp, 8
        mov    dword ptr [edx + 88h], eax
        mov    edx, eax
        or     edx, edx
        je     SHORT loc_5_8C86
        mov    edi, ebx
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
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    edx, dword ptr [esp + 28h]
        mov    esi, edi
        mov    edi, dword ptr [edx + 8ch]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edx, dword ptr [esp + 28h]
        mov    ecx, dword ptr [edx + 88h]
        call   sub_5_2150
        mov    edx, dword ptr [esp + 28h]
        mov    ecx, dword ptr [edx + 8ch]
        call   sub_5_2150
loc_5_8C86:
        cmp    byte ptr [esp + 14h], 16h
        jne    NEAR PTR loc_5_8EE4
        cmp    byte ptr [esp + 13h], 12h
        je     SHORT loc_5_8CA3
        cmp    byte ptr [esp + 13h], 13h
        jne    NEAR PTR loc_5_8EE4
loc_5_8CA3:
        mov    al, byte ptr [ebx + 4]
        sub    al, 41h
        mov    byte ptr [esp + 16h], al
        db  0A0h, 011h, 004h, 000h, 000h
        cmp    al, byte ptr [esp + 16h]
        jb     NEAR PTR loc_5_8EE4
        push   1
        push   102h
        db  0E8h, 0F7h, 07Ch, 0FAh, 0FFh
        add    esp, 8
        mov    ebp, eax
        sub    edx, edx
        cmp    ebp, edx
        jne    SHORT loc_5_8CE4
        mov    ecx, dword ptr [esp + 4ch]
        sub    eax, eax
        mov    byte ptr [ecx + 1ch], 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
loc_5_8CE4:
        sub    eax, eax
        mov    cl, byte ptr [ebx + 5]
        cmp    cl, al
        je     SHORT loc_5_8D0B
loc_5_8CF0:
        cmp    byte ptr [ebx + eax + 6], 3ah
        je     SHORT loc_5_8D06
        inc    eax
        mov    esi, ecx
        and    esi, 0ffh
        cmp    esi, eax
        jg     SHORT loc_5_8CF0
        jmp    SHORT loc_5_8D0B
loc_5_8D06:
        mov    edx, 1
loc_5_8D0B:
        lea    esi, [ebp + 2]
        mov    byte ptr [esi], 0
        mov    al, byte ptr [ebx + 3]
        or     al, al
        je     SHORT loc_5_8D7B
        or     edx, edx
        jne    SHORT loc_5_8D7B
        push   esi
        and    eax, 0ffh
        push   eax
        push   1
        push   16h
        mov    eax, dword ptr [esp + 58h]
        db  068h, 0B8h, 019h, 000h, 000h
        mov    edx, dword ptr [esp + 3ch]
        push   eax
        push   edx
        db  0E8h, 044h, 075h, 0FAh, 0FFh
        add    esp, 1ch
        mov    al, byte ptr [ebx + 6]
        cmp    al, 2fh
        je     SHORT loc_5_8D49
        cmp    al, 5ch
        jne    SHORT loc_5_8D60
loc_5_8D49:
        push   3ah
        push   esi
        db  0E8h, 0BAh, 07Ah, 0FAh, 0FFh
        add    esp, 8
        or     eax, eax
        je     SHORT loc_5_8D60
        mov    byte ptr [eax + 1], 0
        jmp    SHORT loc_5_8D7B
        db  08Bh, 0FFh
loc_5_8D60:
        cmp    byte ptr [ebx + 5], 0
        je     SHORT loc_5_8D7B
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        db  066h, 0A1h, 0C0h, 019h, 000h, 000h
        mov    word ptr [edi - 1], ax
loc_5_8D7B:
        xor    eax, eax
        mov    al, byte ptr [ebx + 5]
        push   eax
        lea    eax, [ebx + 6]
        push   eax
        push   esi
        db  0E8h, 01Fh, 07Ah, 0FAh, 0FFh
        add    esp, 0ch
        mov    ecx, esi
        call   sub_5_6FD0
        mov    edi, esi
        cmp    byte ptr [esi], 0
        je     SHORT loc_5_8DAB
loc_5_8DA0:
        cmp    byte ptr [edi], 5ch
        je     SHORT loc_5_8DAB
        inc    edi
        cmp    byte ptr [edi], 0
        jne    SHORT loc_5_8DA0
loc_5_8DAB:
        mov    al, byte ptr [edi]
        mov    byte ptr [esp + 17h], al
        mov    byte ptr [edi], 0
        mov    al, byte ptr [ebx + 4]
        mov    byte ptr [esp + 24h], al
        mov    byte ptr [esp + 25h], 3ah
        mov    byte ptr [esp + 26h], 0
        lea    eax, [esp + 24h]
        push   esi
        push   eax
        mov    al, byte ptr [esp + 1bh]
        push   0
        sub    al, 12h
        cmp    al, 1
        sbb    edx, edx
        and    edx, 0ffffff00h
        add    edx, 2ffh
        mov    ecx, dword ptr [esp + 34h]
        call   sub_5_7010
        mov    bl, al
        or     bl, bl
        je     SHORT loc_5_8E03
        mov    eax, dword ptr [esp + 48h]
        mov    bl, 98h
        mov    word ptr [eax + 1ah], 0
        jmp    NEAR PTR loc_5_8EB5
loc_5_8E03:
        cmp    byte ptr [esp + 17h], 0
        je     SHORT loc_5_8E88
        mov    al, byte ptr [esp + 17h]
        mov    byte ptr [edi], al
        dec    edi
        mov    al, byte ptr [esp + 16h]
        mov    byte ptr [edi], 3ah
        add    al, 41h
        mov    byte ptr [edi - 1], al
        mov    eax, dword ptr [esp + 48h]
        dec    edi
        mov    dword ptr [eax + 14h], edi
        db  068h, 0CEh, 005h, 000h, 000h
        db  0E8h, 063h, 073h, 0FAh, 0FFh
        add    esp, 4
        cmp    byte ptr [esp + 13h], 13h
        jne    SHORT loc_5_8E50
        db  0E8h, 03Dh, 07Bh, 0FAh, 0FFh
        push   eax
        mov    eax, dword ptr [esp + 4ch]
        push   eax
        db  0E8h, 0D4h, 07Ch, 0FAh, 0FFh
        add    esp, 8
        jmp    SHORT loc_5_8E7B
        db  08Bh, 0FFh
loc_5_8E50:
        db  0E8h, 031h, 07Bh, 0FAh, 0FFh
        mov    edi, eax
        mov    esi, edi
loc_5_8E59:
        test   byte ptr [esi], 10h
        jne    SHORT loc_5_8E6C
        mov    eax, dword ptr [esp + 48h]
        push   esi
        push   eax
        db  0E8h, 0B4h, 07Ch, 0FAh, 0FFh
        add    esp, 8
loc_5_8E6C:
        push   esi
        db  0E8h, 01Fh, 07Bh, 0FAh, 0FFh
        add    esp, 4
        mov    esi, eax
        cmp    edi, esi
        jne    SHORT loc_5_8E59
loc_5_8E7B:
        db  068h, 0CEh, 005h, 000h, 000h
        db  0E8h, 024h, 073h, 0FAh, 0FFh
        add    esp, 4
loc_5_8E88:
        cmp    word ptr [esp + 18h], 1
        jb     SHORT loc_5_8EA5
        xor    ecx, ecx
        mov    cl, byte ptr [esp + 16h]
        db  0A1h, 0D0h, 003h, 000h, 000h
        mov    cl, byte ptr [ecx + eax]
        mov    eax, dword ptr [esp + 2ch]
        mov    byte ptr [eax + 2], cl
loc_5_8EA5:
        cmp    word ptr [esp + 18h], 2
        jb     SHORT loc_5_8EB5
        mov    eax, dword ptr [esp + 2ch]
        mov    byte ptr [eax + 3], 0ffh
loc_5_8EB5:
        push   0
        push   ebp
        db  0E8h, 014h, 07Bh, 0FAh, 0FFh
        mov    eax, dword ptr [esp + 50h]
        add    esp, 8
        cmp    word ptr [eax + 1ah], 0
        je     SHORT loc_5_8ED3
        mov    bl, 9ch
        mov    word ptr [eax + 1ah], 0
loc_5_8ED3:
        mov    ecx, dword ptr [esp + 4ch]
        sub    eax, eax
        mov    byte ptr [ecx + 1ch], bl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
loc_5_8EE4:
        cmp    byte ptr [esp + 14h], 14h
        jne    NEAR PTR loc_5_8FC0
        cmp    byte ptr [esp + 13h], 9
        jne    NEAR PTR loc_5_8FC0
        db  0E8h, 07Ch, 07Ah, 0FAh, 0FFh
        xor    edx, edx
        db  08Ah, 015h, 0EEh, 003h, 000h, 000h
        db  003h, 005h, 004h, 004h, 000h, 000h
        lea    ecx, [edx + edx*2]
        lea    ecx, [edx + ecx*4]
        lea    esi, [eax + ecx + 4ch]
        mov    ecx, edx
        shl    ecx, 6
        db  08Bh, 03Dh, 0E0h, 003h, 000h, 000h
        db  0A1h, 0E4h, 003h, 000h, 000h
        add    edi, ecx
        lea    ebp, [eax + edx*8]
        mov    eax, dword ptr [esp + 48h]
        push   1eh
        mov    ecx, dword ptr [eax + 8]
        mov    dword ptr [esp + 40h], ecx
        lea    ecx, [ebx + 4]
        mov    dword ptr [eax + 8], 0feh
        push   6
        lea    eax, [esi + 5]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ecx]
        inc    eax
        push   eax
        push   ecx
        push   0
        xor    eax, eax
        mov    al, byte ptr [ebx + 3]
        push   eax
        push   43h
        mov    eax, dword ptr [esp + 68h]
        db  068h, 0C4h, 019h, 000h, 000h
        mov    edx, dword ptr [esp + 4ch]
        push   eax
        push   edx
        db  0E8h, 011h, 073h, 0FAh, 0FFh
        add    esp, 2ch
        mov    bl, al
        mov    eax, dword ptr [esp + 48h]
        mov    ecx, dword ptr [esp + 3ch]
        mov    dword ptr [eax + 8], ecx
        or     bl, bl
        jne    SHORT loc_5_8FAD
        mov    byte ptr [esi + 0bh], 0ffh
        mov    byte ptr [esi + 0ch], 0feh
        mov    byte ptr [esi + 1], 0
        mov    byte ptr [esi + 2], 0
        xor    edx, edx
        mov    byte ptr [ebp + 4], 0
        mov    byte ptr [edi + 2bh], 0ffh
        mov    byte ptr [edi + 37h], 0
        db  08Ah, 015h, 0EEh, 003h, 000h, 000h
        mov    ecx, eax
        call   sub_5_3050
loc_5_8FAD:
        mov    ecx, dword ptr [esp + 4ch]
        sub    eax, eax
        mov    byte ptr [ecx + 1ch], bl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  08Bh, 0FFh
loc_5_8FC0:
        cmp    byte ptr [esp + 14h], 16h
        jne    SHORT loc_5_900F
        cmp    byte ptr [esp + 13h], 14h
        jne    SHORT loc_5_900F
        mov    cl, byte ptr [ebx + 3]
        sub    al, al
        db  08Bh, 03Dh, 0DCh, 003h, 000h, 000h
        lea    esp, [esp]
loc_5_8FE0:
        mov    esi, eax
        and    esi, 0ffh
        cmp    dword ptr [edi + esi*4], 0
        je     SHORT loc_5_9009
        mov    ebp, dword ptr [edi + esi*4]
        mov    edx, dword ptr [esp + 28h]
        cmp    dword ptr [ebp + 10h], edx
        jne    SHORT loc_5_9009
        db  08Bh, 015h, 0D0h, 003h, 000h, 000h
        cmp    byte ptr [esi + edx], cl
        je     NEAR PTR loc_5_90A5
loc_5_9009:
        inc    al
        cmp    al, 20h
        jb     SHORT loc_5_8FE0
loc_5_900F:
        add    ebx, 3
        mov    eax, dword ptr [esp + 2ch]
        add    eax, 2
        mov    dword ptr [esp + 30h], eax
        mov    byte ptr [esp + 15h], 0ffh
        xor    esi, esi
        dec    word ptr [esp + 1ah]
        mov    si, word ptr [esp + 1ah]
        push   esi
        push   ebx
        db  0E8h, 01Bh, 07Ah, 0FAh, 0FFh
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        je     NEAR PTR loc_5_90F0
        mov    ebp, dword ptr [esp + 18h]
        and    ebp, 0ffffh
        mov    eax, dword ptr [esp + 30h]
        push   ebp
        push   eax
        db  0E8h, 0F9h, 079h, 0FAh, 0FFh
        mov    dword ptr [esp + 38h], eax
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_5_90E6
        push   ebp
        push   eax
        push   esi
        push   edi
        xor    eax, eax
        mov    al, byte ptr [esp + 23h]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esp + 28h]
        push   eax
        mov    eax, dword ptr [esp + 60h]
        db  068h, 0CCh, 019h, 000h, 000h
        mov    edx, dword ptr [esp + 44h]
        push   eax
        push   edx
        db  0E8h, 0F3h, 071h, 0FAh, 0FFh
        mov    byte ptr [esp + 39h], al
        add    esp, 24h
        push   esi
        push   edi
        db  0E8h, 0F3h, 079h, 0FAh, 0FFh
        add    esp, 8
        mov    eax, dword ptr [esp + 30h]
        push   ebp
        push   eax
        jmp    SHORT loc_5_90E8
loc_5_90A5:
        lea    ecx, [eax + 41h]
        mov    byte ptr [esp + 20h], cl
        mov    byte ptr [esp + 21h], 3ah
        mov    byte ptr [esp + 22h], 0
        xor    ecx, ecx
        mov    cl, al
        db  0A1h, 0D8h, 003h, 000h, 000h
        cmp    byte ptr [ecx + eax], 1
        sbb    edx, edx
        lea    ecx, [esp + 20h]
        inc    edx
        call   sub_5_71F0
        or     al, al
        je     SHORT loc_5_90D5
        mov    al, 9bh
loc_5_90D5:
        mov    ecx, dword ptr [esp + 4ch]
        pop    ebp
        mov    byte ptr [ecx + 1ch], al
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
loc_5_90E6:
        push   esi
        push   edi
loc_5_90E8:
        db  0E8h, 0A0h, 079h, 0FAh, 0FFh
        add    esp, 8
loc_5_90F0:
        mov    ecx, dword ptr [esp + 4ch]
        mov    al, byte ptr [esp + 15h]
        mov    byte ptr [ecx + 1ch], al
        cmp    byte ptr [esp + 14h], 17h
        jne    SHORT loc_5_916F
        cmp    byte ptr [esp + 13h], 68h
        jne    SHORT loc_5_916F
        or     al, al
        jne    SHORT loc_5_916F
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esp + 38h], eax
        mov    eax, dword ptr [esp + 38h]
        xchg   ah, al
        rol    eax, 10h
        xchg   ah, al
        mov    dword ptr [esp + 34h], eax
        mov    ecx, dword ptr [esp + 34h]
        mov    eax, dword ptr [esp + 2ch]
        mov    ax, word ptr [eax + 18h]
        mov    word ptr [esp + 1eh], ax
        mov    ax, word ptr [esp + 1eh]
        xchg   al, ah
        mov    word ptr [esp + 1ch], ax
        push   ecx
        mov    ax, word ptr [esp + 20h]
        push   eax
        mov    eax, dword ptr [esp + 34h]
        add    eax, 2ch
        mov    cx, word ptr [eax + 4]
        push   ecx
        mov    eax, dword ptr [eax]
        push   eax
        mov    edx, dword ptr [esp + 38h]
        push   edx
        mov    eax, dword ptr [esp + 5ch]
        mov    esi, dword ptr [eax + 8]
        db  0E8h, 015h, 078h, 0FAh, 0FFh
        mov    edx, esi
        mov    ecx, eax
        call   sub_5_2F20
loc_5_916F:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_9180:
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [esp + 14h]
        mov    ecx, esi
        call   sub_5_2040
        or     eax, eax
        jne    SHORT loc_5_919A
        sub    eax, eax
        pop    esi
        add    esp, 8
        ret
loc_5_919A:
        lea    ecx, [esp + 4]
        push   7
        push   ecx
        push   14h
        db  068h, 0D4h, 019h, 000h, 000h
        push   esi
        push   eax
        db  0E8h, 0D0h, 070h, 0FAh, 0FFh
        add    esp, 18h
        or     al, al
        je     SHORT loc_5_91C5
        mov    ecx, dword ptr [esp + 18h]
        sub    eax, eax
        mov    byte ptr [ecx + 1ch], 0ffh
        pop    esi
        add    esp, 8
        ret
loc_5_91C5:
        mov    edx, 3c14h
        mov    ecx, esi
        call   sub_5_6D80
        mov    ecx, dword ptr [esp + 18h]
        lea    edx, [esp + 4]
        mov    byte ptr [ecx + 1ch], 0
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax], ecx
        mov    cx, word ptr [edx + 4]
        mov    word ptr [eax + 4], cx
        mov    cl, byte ptr [edx + 6]
        mov    byte ptr [eax + 6], cl
        sub    eax, eax
        pop    esi
        add    esp, 8
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_9200:
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0E8h, 06Fh, 077h, 0FAh, 0FFh
        db  08Bh, 035h, 004h, 004h, 000h, 000h
        add    esi, eax
        mov    ebp, dword ptr [esp + 38h]
        xor    eax, eax
        mov    al, byte ptr [ebp + 1ch]
        cmp    eax, 0fdh
        jg     SHORT loc_5_9238
        je     NEAR PTR loc_5_95FB
        cmp    eax, 9
        ja     SHORT loc_5_924A
        shl    eax, 2
        db  0FFh, 0A0h, 070h, 097h, 000h, 000h
loc_5_9238:
        cmp    eax, 0feh
        jl     SHORT loc_5_924A
        cmp    eax, 0ffh
        jle    NEAR PTR loc_5_9672
loc_5_924A:
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_9257:
        mov    ecx, dword ptr [esp + 34h]
        call   sub_5_2040
        mov    al, byte ptr [ebp + 14h]
        cmp    al, 20h
        jb     SHORT loc_5_9277
        mov    word ptr [ebp + 1ch], 0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_9277:
        xor    ecx, ecx
        mov    cl, al
        db  0A1h, 0C8h, 003h, 000h, 000h
        mov    al, byte ptr [ecx + eax]
        mov    byte ptr [ebp + 1dh], al
        db  0A1h, 0D0h, 003h, 000h, 000h
        mov    al, byte ptr [ecx + eax]
        mov    byte ptr [ebp + 1ch], al
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_5_92A0:
        mov    edx, 3c14h
        mov    ecx, dword ptr [esp + 34h]
        call   sub_5_6D80
        mov    ecx, dword ptr [esi + 73h]
        add    esi, 73h
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esi + 0ch]
        mov    dword ptr [eax + 0ch], ecx
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_92D2:
        mov    edx, 3c14h
        mov    ecx, dword ptr [esp + 34h]
        call   sub_5_6D80
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 73h], ecx
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [esi + 77h], ecx
        add    esi, 73h
        mov    ecx, dword ptr [eax + 8]
        mov    dword ptr [esi + 8], ecx
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 0ch], eax
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_9304:
        mov    al, byte ptr [ebp + 14h]
        cmp    al, 20h
        jae    NEAR PTR loc_5_9765
        and    eax, 0ffh
        db  08Bh, 00Dh, 0C8h, 003h, 000h, 000h
        or     byte ptr [eax + ecx], 80h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_9328:
        mov    al, byte ptr [ebp + 14h]
        cmp    al, 20h
        jae    NEAR PTR loc_5_9765
        and    eax, 0ffh
        db  08Bh, 00Dh, 0C8h, 003h, 000h, 000h
        and    byte ptr [eax + ecx], 7fh
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Dh, 064h, 024h, 000h
loc_5_9350:
        mov    ebx, dword ptr [esp + 34h]
        mov    ecx, ebx
        call   sub_5_2040
        mov    dword ptr [esp + 1ch], eax
        or     eax, eax
        je     NEAR PTR loc_5_9765
        mov    al, byte ptr [ebp + 10h]
        mov    byte ptr [esp + 13h], al
        cmp    al, 20h
        ja     NEAR PTR loc_5_9765
        mov    edx, 3c14h
        mov    ecx, ebx
        call   sub_5_6D80
        mov    edi, eax
        cmp    byte ptr [esp + 13h], 0
        je     NEAR PTR loc_5_9765
        dec    byte ptr [esp + 13h]
        xor    ecx, ecx
        db  0A1h, 0DCh, 003h, 000h, 000h
        mov    cl, byte ptr [esp + 13h]
        cmp    dword ptr [eax + ecx*4], 0
        je     NEAR PTR loc_5_94D2
        lea    ecx, [esp + 20h]
        mov    edx, ebx
        call   sub_5_9820
        or     al, al
        jne    NEAR PTR loc_5_9500
        cmp    dword ptr [esp + 24h], 0
        je     NEAR PTR loc_5_9500
        cmp    byte ptr [esp + 29h], 0
        je     NEAR PTR loc_5_9464
        cmp    byte ptr [esp + 2ah], 0
        jne    SHORT loc_5_9427
        mov    edx, dword ptr [ebx + 0ch]
        xor    ecx, ecx
        mov    cx, word ptr [edx + 2]
        lea    eax, [esp + 28h]
        push   eax
        mov    eax, dword ptr [esp + 24h]
        push   0deh
        mov    ax, word ptr [eax]
        shr    ax, 1
        and    eax, 0ffffh
        add    eax, 6
        push   eax
        xor    eax, eax
        mov    ax, word ptr [edx + ecx]
        add    eax, ecx
        add    eax, edx
        push   eax
        mov    eax, dword ptr [esp + 30h]
        push   eax
        push   13h
        push   16h
        db  068h, 0D8h, 019h, 000h, 000h
        mov    eax, dword ptr [esp + 44h]
        push   ebx
        push   eax
        db  0E8h, 05Dh, 06Eh, 0FAh, 0FFh
        add    esp, 28h
        jmp    SHORT loc_5_94A0
loc_5_9427:
        mov    ecx, dword ptr [ebx + 0ch]
        xor    edx, edx
        mov    dx, word ptr [ecx + 2]
        lea    eax, [esp + 28h]
        push   eax
        push   1
        push   6
        push   3ah
        xor    eax, eax
        mov    ax, word ptr [ecx + edx]
        add    eax, edx
        add    eax, ecx
        push   eax
        mov    eax, dword ptr [esp + 34h]
        push   eax
        push   13h
        push   16h
        db  068h, 0E4h, 019h, 000h, 000h
        mov    eax, dword ptr [esp + 48h]
        push   ebx
        push   eax
        db  0E8h, 020h, 06Eh, 0FAh, 0FFh
        add    esp, 2ch
        jmp    SHORT loc_5_94A0
loc_5_9464:
        mov    edx, dword ptr [ebx + 0ch]
        xor    ecx, ecx
        mov    cx, word ptr [edx + 2]
        lea    eax, [esp + 28h]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [edx + ecx]
        add    eax, ecx
        add    eax, edx
        push   eax
        mov    eax, dword ptr [esp + 28h]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esp + 34h]
        push   eax
        push   13h
        push   16h
        db  068h, 0F0h, 019h, 000h, 000h
        mov    eax, dword ptr [esp + 40h]
        push   ebx
        push   eax
        db  0E8h, 0E2h, 06Dh, 0FAh, 0FFh
        add    esp, 24h
loc_5_94A0:
        or     al, al
        je     SHORT loc_5_94B4
        mov    word ptr [ebx + 1ah], 3
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_94B4:
        xor    eax, eax
        mov    al, byte ptr [esp + 28h]
        push   eax
        push   14h
        push   16h
        db  068h, 0F8h, 019h, 000h, 000h
        mov    eax, dword ptr [esp + 34h]
        push   ebx
        push   eax
        db  0E8h, 0B0h, 06Dh, 0FAh, 0FFh
        add    esp, 18h
loc_5_94D2:
        push   1
        push   100h
        db  0E8h, 0E0h, 074h, 0FAh, 0FFh
        mov    dword ptr [esp + 20h], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_5_9510
        mov    word ptr [ebx + 1ah], 3
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_5_9500:
        mov    word ptr [ebx + 1ah], 3
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_9510:
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [esp + 18h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [esp + 18h]
        call   sub_5_6FD0
        mov    al, byte ptr [esp + 13h]
        add    al, 41h
        mov    byte ptr [esp + 14h], al
        mov    byte ptr [esp + 15h], 3ah
        mov    byte ptr [esp + 16h], 0
        mov    eax, dword ptr [esp + 18h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   0
        mov    edx, 100h
        mov    ecx, dword ptr [esp + 28h]
        call   sub_5_7010
        or     al, al
        je     SHORT loc_5_9574
        mov    word ptr [ebx + 1ah], 3
        jmp    SHORT loc_5_9578
loc_5_9574:
        mov    byte ptr [ebp + 1ch], 0
loc_5_9578:
        mov    eax, dword ptr [esp + 18h]
        push   0
        push   eax
        db  0E8h, 04Dh, 074h, 0FAh, 0FFh
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_9591:
        mov    al, byte ptr [ebp + 10h]
        cmp    al, 20h
        jae    NEAR PTR loc_5_9765
        and    eax, 0ffh
        db  08Bh, 00Dh, 0D4h, 003h, 000h, 000h
        mov    byte ptr [eax + ecx], 0ffh
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_95B5:
        mov    al, byte ptr [ebp + 10h]
        cmp    al, 20h
        ja     NEAR PTR loc_5_9765
        dec    al
        and    eax, 0ffh
        db  08Bh, 00Dh, 0D4h, 003h, 000h, 000h
        mov    al, byte ptr [eax + ecx]
        mov    byte ptr [ebp + 1ch], al
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Dh, 049h, 000h
loc_5_95E0:
        mov    cl, byte ptr [ebp + 10h]
        db  0A0h, 06Ch, 004h, 000h, 000h
        mov    byte ptr [ebp + 10h], al
        db  088h, 00Dh, 06Ch, 004h, 000h, 000h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_95FB:
        mov    al, byte ptr [ebp + 14h]
        cmp    al, 20h
        jae    NEAR PTR loc_5_9765
        mov    esi, eax
        and    esi, 0ffh
        db  08Bh, 00Dh, 0C8h, 003h, 000h, 000h
        mov    cl, byte ptr [esi + ecx]
        mov    byte ptr [ebp + 1dh], cl
        db  0A1h, 0D0h, 003h, 000h, 000h
        mov    al, byte ptr [esi + eax]
        mov    byte ptr [ebp + 1ch], al
        db  0A1h, 0CCh, 003h, 000h, 000h
        db  08Bh, 015h, 0DCh, 003h, 000h, 000h
        mov    al, byte ptr [esi + eax]
        mov    edx, dword ptr [edx + esi*4]
        or     edx, edx
        je     SHORT loc_5_9656
        cmp    cl, 80h
        jne    SHORT loc_5_9656
        mov    byte ptr [ebp + 1dh], 1
        mov    eax, dword ptr [edx + 10h]
        db  02Bh, 005h, 0A0h, 003h, 000h, 000h
        mov    ecx, 0a0h
        cdq
        idiv   ecx
        sub    al, 7fh
loc_5_9656:
        xor    ecx, ecx
        mov    cl, al
        xor    eax, eax
        mov    ax, word ptr [ebp + 1ch]
        shl    ecx, 10h
        add    ecx, eax
        mov    dword ptr [ebp + 1ch], ecx
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_9672:
        mov    ebx, dword ptr [esp + 34h]
        mov    ecx, ebx
        call   sub_5_2040
        mov    esi, eax
        mov    edx, 3800h
        mov    ecx, ebx
        call   sub_5_6D80
        mov    edx, eax
        or     esi, esi
        jne    SHORT loc_5_96A1
        mov    word ptr [ebx + 1ah], 0ffh
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_96A1:
        or     edx, edx
        je     NEAR PTR loc_5_9765
        cmp    byte ptr [ebp + 1ch], 0ffh
        jne    NEAR PTR loc_5_9730
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 8
        rep stosd
        mov    al, byte ptr [esi + 34h]
        or     al, al
        je     NEAR PTR loc_5_9765
        mov    byte ptr [edx], al
        lea    ecx, [edx + 2]
        mov    eax, dword ptr [esi + 78h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi + 7ch]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi + 80h]
        lea    edi, [esi + 78h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [esi + 40h]
        add    eax, 1ch
        lea    edi, [edx + 10h]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edi], ecx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [edi + 4], ax
        mov    al, byte ptr [esi + 72h]
        mov    byte ptr [edx + 16h], al
        mov    byte ptr [edx + 18h], 2
        mov    al, byte ptr [esi + 73h]
        mov    byte ptr [edx + 17h], al
        mov    byte ptr [edx + 1bh], al
        mov    al, byte ptr [esi + 74h]
        mov    byte ptr [edx + 1ch], al
        mov    al, byte ptr [esi + 75h]
        mov    byte ptr [edx + 1dh], al
        mov    al, byte ptr [esi + 76h]
        mov    byte ptr [edx + 1fh], al
        mov    al, byte ptr [esi + 5bh]
        mov    byte ptr [ebp + 15h], al
        sub    eax, eax
loc_5_9727:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  090h
loc_5_9730:
        cmp    byte ptr [esi + 34h], 0
        je     SHORT loc_5_9762
        mov    edi, dword ptr [esi + 3ch]
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
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_9762:
        mov    byte ptr [edx], 0
loc_5_9765:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  090h
loc_5_9770:
        push   edi
        xchg   edx, eax
        add    byte ptr [eax], al
        db  0A0h, 092h, 000h, 000h, 0D2h
        xchg   edx, eax
        add    byte ptr [eax], al
        add    al, 93h
        add    byte ptr [eax], al
        db  028h, 093h, 000h, 000h, 050h, 093h
        add    byte ptr [eax], al
        xchg   ecx, eax
        xchg   ebp, eax
        add    byte ptr [eax], al
        mov    ch, 95h
        add    byte ptr [eax], al
        loopne loc_5_9727
        add    byte ptr [eax], al
        xchg   edi, eax
        add    byte ptr [eax], al
        lea    esp, [esp]
        lea    esp, [esp]
loc_5_97A0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 1ch]
        cmp    byte ptr [ebx + 1ch], 0
        je     SHORT loc_5_97F0
        mov    edx, 3800h
        mov    ecx, dword ptr [esp + 18h]
        call   sub_5_6D80
        mov    edx, eax
        sub    ebp, ebp
        mov    esi, 4
loc_5_97C5:
        db  08Dh, 0BDh, 070h, 004h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    ebp, ecx
        dec    esi
        jne    SHORT loc_5_97C5
        db  0BEh, 070h, 004h, 000h, 000h
        mov    edi, edx
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
loc_5_97F0:
        mov    byte ptr [ebx + 1dh], 0
        mov    byte ptr [ebx + 1ch], 0
        db  0A0h, 0C0h, 018h, 000h, 000h
        mov    byte ptr [ebx + 11h], al
        db  0A0h, 0C4h, 018h, 000h, 000h
        mov    byte ptr [ebx + 10h], al
        sub    eax, eax
        mov    byte ptr [ebx + 19h], 0
        mov    byte ptr [ebx + 18h], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
_PTEXT ends
        end
