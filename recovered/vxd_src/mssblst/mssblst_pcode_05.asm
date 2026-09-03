; mssblst_pcode_05.asm -- MSSBLST object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  00Fh, 0B7h, 005h, 010h, 000h, 000h, 000h, 003h, 005h, 004h, 000h, 000h, 000h, 0C3h
sub_5_000E:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        add    edx, dword ptr [ebp + 0ch]
        xor    eax, eax
        in     al, dx
        pop    ebp
        ret
sub_5_001C:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_002A:
        jb     SHORT loc_5_0030
        xor    eax, eax
        jmp    SHORT loc_5_0033
loc_5_0030:
        xor    eax, eax
        inc    eax
loc_5_0033:
        pop    ebp
        ret
sub_5_0035:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0040:
        pop    ebp
        ret
sub_5_0042:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        add    edx, 4
        mov    eax, dword ptr [ebp + 0ch]
        out    dx, al
        jmp    SHORT loc_5_0051
loc_5_0051:
        inc    edx
        in     al, dx
        pop    ebp
        ret
sub_5_0055:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        add    edx, 4
        mov    eax, dword ptr [ebp + 0ch]
        out    dx, al
        jmp    SHORT loc_5_0064
loc_5_0064:
        inc    edx
        mov    eax, dword ptr [ebp + 10h]
        out    dx, al
        pop    ebp
        ret
sub_5_006B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 01Ch, 000h, 004h, 000h
        movzx  eax, cx
        pop    ecx
        pop    ebx
        pop    ebp
        ret
sub_5_0086:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 4
        cmp    ebx, 7
        jbe    SHORT loc_5_0099
        add    ebx, 4
loc_5_0099:
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    ecx, eax
loc_5_00A1:
        db  0CDh, 020h, 01Dh, 000h, 004h, 000h
        bt     eax, ebx
        jae    SHORT loc_5_00B9
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        sub    eax, ecx
        cmp    eax, 64h
        jb     SHORT loc_5_00A1
loc_5_00B9:
        pop    ecx
        pop    ebx
        pop    ebp
        ret
sub_5_00BD:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    edx, eax
loc_5_00CB:
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 01Ch, 000h, 004h, 000h
        cmp    ecx, 0ffffh
        je     SHORT loc_5_00EF
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        sub    eax, edx
        cmp    eax, dword ptr [ebp + 0ch]
        jb     SHORT loc_5_00CB
loc_5_00EF:
        pop    edx
        pop    ecx
        pop    ebx
        pop    ebp
        ret
sub_5_00F4:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 018h, 000h, 004h, 000h
        pop    ebp
        ret
sub_5_0102:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 019h, 000h, 004h, 000h
        pop    ebx
        pop    ebp
        ret
sub_5_0118:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        xor    bx, bx
        mov    bh, 1
        xor    esi, esi
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 003h, 000h, 004h, 000h
        pop    ecx
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_5_0139:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edx
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        xor    dx, dx
        or     dl, 8
        or     dl, 10h
        or     dl, 40h
        db  0CDh, 020h, 006h, 000h, 004h, 000h
        pop    edx
        pop    ebx
        pop    ebp
        ret
sub_5_015D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edx
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        xor    dx, dx
        or     dl, 8
        or     dl, 40h
        db  0CDh, 020h, 006h, 000h, 004h, 000h
        pop    edx
        pop    ebx
        pop    ebp
        ret
sub_5_017E:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_018E:
        add    esp, 8
        mov    word ptr [edi + 50h], 0
        pop    ebx
        pop    edi
        pop    ebp
        ret
sub_5_019B:
        push   ebp
        mov    ebp, esp
        push   edi
        push   edx
        push   ebx
        mov    edx, 0ffffh
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_01B1:
        add    esp, 8
        mov    eax, dword ptr [edi + 28h]
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        cmp    eax, ebx
        je     SHORT loc_5_01C3
        jmp    SHORT loc_5_01CE
loc_5_01C3:
        mov    word ptr [edi + 50h], 1
        mov    edx, 0
loc_5_01CE:
        mov    eax, edx
        pop    ebx
        pop    edx
        pop    edi
        pop    ebp
        ret
sub_5_01D5:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_01E5:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_01F2
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [edi + 4ch], eax
loc_5_01F2:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_5_01F6:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0206:
        add    esp, 8
        xor    eax, eax
        or     edi, edi
        je     SHORT loc_5_0212
        mov    eax, dword ptr [edi + 4ch]
loc_5_0212:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_5_0216:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0229:
        add    esp, 8
        mov    ax, word ptr [edi + 0ch]
        mov    word ptr [esi + 18h], ax
        pop    esi
        pop    edi
        pop    ebp
        ret
sub_5_0238:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_024B:
        add    esp, 8
        mov    ax, word ptr [edi + 0ch]
        mov    word ptr [esi + 34h], ax
        mov    ax, word ptr [edi + 6]
        mov    word ptr [esi + 36h], ax
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [esi + 0ch], eax
        mov    eax, dword ptr [edi + 18h]
        mov    dword ptr [esi + 10h], eax
        mov    eax, dword ptr [edi + 1ch]
        mov    dword ptr [esi + 14h], eax
        mov    eax, dword ptr [edi + 38h]
        mov    dword ptr [esi + 18h], eax
        mov    al, byte ptr [edi + 0bh]
        mov    byte ptr [esi + 38h], al
        pop    esi
        pop    edi
        pop    ebp
        ret
loc_5_0280:
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        push   edi
        mov    ecx, 10h
        db  0BFh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb
        je     SHORT loc_5_02B5
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        mov    esi, eax
        repe cmpsb
        mov    eax, 80004002h
        jne    SHORT loc_5_02C3
loc_5_02B5:
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_02C3:
        pop    edi
        pop    esi
        ret    0ch
loc_5_02C8:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0ch]
        inc    eax
        mov    dword ptr [ecx + 0ch], eax
        ret    4
loc_5_02D6:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0ch]
        dec    eax
        mov    dword ptr [ecx + 0ch], eax
        jne    SHORT loc_5_02F0
        push   0
        push   ecx
        call   sub_5_1928
        add    esp, 8
        xor    eax, eax
loc_5_02F0:
        ret    4
loc_5_02F3:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        lea    edi, [esi + 104h]
        mov    dword ptr [esi], 0bh
        call   0
        mov    ebx, eax
        push   0ffh
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        call   sub_5_1930
        add    esp, 0ch
        lea    eax, [esi + 4]
        push   0ffh
        push   ebx
        push   eax
        call   sub_5_1930
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 0ch
        xor    ecx, ecx
        mov    dword ptr [esi + 210h], ecx
        mov    edx, dword ptr [eax + 4]
        mov    eax, 0b3h
        mov    dword ptr [esi + 204h], edx
        mov    dword ptr [esi + 20ch], ecx
        mov    word ptr [esi + 208h], 44eh
        cmp    byte ptr [edi], cl
        je     SHORT loc_5_036A
loc_5_035F:
        movsx  ecx, byte ptr [edi]
        add    eax, ecx
        inc    edi
        cmp    byte ptr [edi], 0
        jne    SHORT loc_5_035F
loc_5_036A:
        mov    ecx, 0f7h
        sub    edx, edx
        div    ecx
        mov    word ptr [esi + 20ah], dx
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_5_0382:
        push   esi
        push   edi
        mov    edi, 8878000ah
        mov    esi, dword ptr [esp + 0ch]
        cmp    dword ptr [esi + 14h], 0
        jne    SHORT loc_5_03A7
        push   dword ptr [esi + 4]
        call   sub_5_019B
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_5_03A7
        xor    edi, edi
        inc    dword ptr [esi + 14h]
loc_5_03A7:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    4
loc_5_03AE:
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax + 18h], 300h
        mov    eax, dword ptr [esp + 8]
        jb     SHORT loc_5_03C6
        mov    dword ptr [eax], 7
        jmp    SHORT loc_5_03CC
loc_5_03C6:
        mov    dword ptr [eax], 5
loc_5_03CC:
        mov    dword ptr [eax + 0ch], 1
        xor    ecx, ecx
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [eax + 10h], ecx
        mov    dword ptr [eax + 14h], ecx
        mov    dword ptr [eax + 18h], ecx
        mov    dword ptr [eax + 1ch], ecx
        mov    dword ptr [eax + 20h], ecx
        mov    dword ptr [eax + 24h], ecx
        mov    dword ptr [eax + 28h], ecx
        mov    dword ptr [eax + 2ch], ecx
        mov    dword ptr [eax + 30h], ecx
        mov    dword ptr [eax + 34h], ecx
        mov    dword ptr [eax + 38h], ecx
        mov    dword ptr [eax + 3ch], ecx
        mov    dword ptr [eax + 40h], ecx
        mov    dword ptr [eax + 44h], ecx
        mov    dword ptr [eax + 48h], ecx
        xor    eax, eax
        ret    8
loc_5_040D:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   ebp
        mov    edi, 8007000eh
        mov    esi, dword ptr [esp + 2ch]
        test   byte ptr [esp + 1ch], 0f0h
        mov    dword ptr [esi], ebx
        je     SHORT loc_5_042F
        mov    eax, 8878001eh
        jmp    NEAR PTR loc_5_04DF
loc_5_042F:
        mov    ebp, dword ptr [esp + 14h]
        cmp    dword ptr [ebp + 10h], 0
        jne    NEAR PTR loc_5_04BE
        push   201h
        push   39h
        call   sub_5_1938
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_5_04B7
        db  0C7h, 003h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 4], ebp
        push   dword ptr [ebp + 4]
        push   ebx
        call   sub_5_0238
        add    esp, 8
        mov    eax, dword ptr [ebx + 14h]
        cmp    eax, 1000h
        jbe    SHORT loc_5_04C3
        cmp    eax, 0ffffh
        jae    SHORT loc_5_04C3
        cmp    dword ptr [ebx + 0ch], 0
        je     SHORT loc_5_04C3
        cmp    dword ptr [ebx + 10h], 0
        je     SHORT loc_5_04C3
        cmp    dword ptr [ebx + 18h], 0
        je     SHORT loc_5_04C3
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 2ch]
        push   ebx
        xor    edi, edi
        call   sub_5_0642
        mov    ecx, dword ptr [esp + 24h]
        add    esp, 0ch
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [ebx + 24h], eax
        movzx  edx, word ptr [ecx + 0eh]
        mov    dword ptr [ebx + 28h], edx
        movzx  ecx, word ptr [ecx + 2]
        mov    dword ptr [ebx + 2ch], ecx
        jmp    SHORT loc_5_04C3
loc_5_04B7:
        mov    edi, 8007000eh
        jmp    SHORT loc_5_04C3
loc_5_04BE:
        mov    edi, 8878000ah
loc_5_04C3:
        test   edi, edi
        je     SHORT loc_5_04D8
        test   ebx, ebx
        je     SHORT loc_5_04DD
        push   0
        push   ebx
        call   sub_5_1928
        add    esp, 8
        jmp    SHORT loc_5_04DD
loc_5_04D8:
        mov    dword ptr [esi], ebx
        inc    dword ptr [ebp + 10h]
loc_5_04DD:
        mov    eax, edi
loc_5_04DF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    1ch
loc_5_04E6:
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], 0
        mov    eax, 80004001h
        ret    0ch
loc_5_04F8:
        mov    eax, 80004005h
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [edx + 14h]
        dec    ecx
        mov    dword ptr [edx + 14h], ecx
        jne    SHORT loc_5_051D
        cmp    dword ptr [edx + 10h], 0
        jne    SHORT loc_5_051D
        push   dword ptr [edx + 4]
        call   sub_5_017E
        add    esp, 4
        xor    eax, eax
loc_5_051D:
        ret    4
sub_5_0520:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   201h
        mov    dword ptr [ebp - 4], ebx
        push   1ah
        call   sub_5_1938
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_5_05AF
        mov    edi, dword ptr [ebp + 8]
        push   1
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 14h]
        push   ecx
        mov    dword ptr [esi + 4], edi
        db  0C7h, 006h, 000h, 000h, 000h, 000h
        call   sub_5_1954
        test   eax, eax
        jne    SHORT loc_5_05AF
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 10h], ebx
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [ebp - 0ch], esi
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        db  0CDh, 020h, 001h, 000h, 07Eh, 035h
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], ebx
        jne    SHORT loc_5_05AF
        push   esi
        push   edi
        call   sub_5_01D5
        add    esp, 8
        push   edi
        push   esi
        mov    ebx, 1
        call   sub_5_0216
        add    esp, 8
        mov    eax, dword ptr [esi]
        push   esi
        call   dword ptr [eax + 4]
loc_5_05AF:
        test   ebx, ebx
        jne    SHORT loc_5_05D8
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_5_05C9
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 0bh]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_5_1940
loc_5_05C9:
        test   esi, esi
        je     SHORT loc_5_05D8
        push   0
        push   esi
        call   sub_5_1928
        add    esp, 8
loc_5_05D8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_05DF:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_5_01F6
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_5_063C
        push   0
        push   edi
        call   sub_5_01D5
        add    esp, 8
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [ebp - 8], 0
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        db  0CDh, 020h, 002h, 000h, 07Eh, 035h
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [esi + 8]
        push   dword ptr [eax + 0bh]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_5_1940
        push   esi
        mov    edi, dword ptr [esi]
        call   dword ptr [edi + 8]
loc_5_063C:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_5_0642:
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 14h]
        cmp    dword ptr [edx], eax
        jbe    SHORT loc_5_0654
        mov    dword ptr [edx], eax
loc_5_0654:
        mov    eax, dword ptr [esp + 0ch]
        and    dword ptr [edx], 0fffch
        mov    ecx, dword ptr [esi + 10h]
        mov    dword ptr [eax], ecx
        mov    edx, dword ptr [edx]
        mov    dword ptr [esi + 1ch], edx
        inc    dword ptr [esi + 20h]
        mov    dword ptr [esi + 8], 0
        pop    esi
        ret
loc_5_0674:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 20h]
        dec    esi
        mov    dword ptr [eax + 20h], esi
        jne    SHORT loc_5_0699
        cmp    dword ptr [eax + 8], 0
        jne    SHORT loc_5_0699
        mov    ecx, dword ptr [eax + 4]
        push   0
        push   eax
        dec    dword ptr [ecx + 10h]
        call   sub_5_1928
        add    esp, 8
loc_5_0699:
        mov    eax, esi
        pop    esi
        ret    4
loc_5_069F:
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        push   edi
        mov    ecx, 10h
        db  0BFh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb
        je     SHORT loc_5_06D4
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        mov    esi, eax
        repe cmpsb
        mov    eax, 80004002h
        jne    SHORT loc_5_06E2
loc_5_06D4:
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_06E2:
        pop    edi
        pop    esi
        ret    0ch
loc_5_06E7:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 20h]
        inc    eax
        mov    dword ptr [ecx + 20h], eax
        ret    4
loc_5_06F5:
        xor    eax, eax
        ret    20h
loc_5_06FA:
        xor    eax, eax
        ret    14h
loc_5_06FF:
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 24h], eax
        movzx  eax, word ptr [ecx + 0eh]
        mov    dword ptr [edx + 28h], eax
        mov    eax, 80004001h
        movzx  ecx, word ptr [ecx + 2]
        mov    dword ptr [edx + 2ch], ecx
        ret    8
loc_5_0723:
        mov    eax, 80004001h
        ret    8
loc_5_072B:
        mov    eax, 80004001h
        ret    8
loc_5_0733:
        mov    eax, 80004001h
        ret    8
loc_5_073B:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 8], 1
        jne    SHORT loc_5_076D
        push   dword ptr [esi + 18h]
        call   sub_5_006B
        add    esp, 4
        inc    eax
        cmp    byte ptr [esi + 38h], 3
        jbe    SHORT loc_5_075A
        add    eax, eax
loc_5_075A:
        mov    ecx, dword ptr [esi + 1ch]
        mov    edx, dword ptr [esp + 10h]
        sub    ecx, eax
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx], ecx
        jmp    SHORT loc_5_0781
loc_5_076D:
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx], 0
loc_5_0781:
        xor    eax, eax
        pop    esi
        ret    0ch
loc_5_0787:
        push   ebp
        mov    eax, 80004005h
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 8], 0
        jne    NEAR PTR loc_5_099F
        mov    eax, dword ptr [esi + 24h]
        mov    ebx, dword ptr [esi + 2ch]
        push   0eh
        mov    dword ptr [ebp - 4], eax
        movzx  edi, word ptr [esi + 36h]
        push   edi
        call   sub_5_000E
        add    esp, 8
        push   dword ptr [esi + 18h]
        call   sub_5_00F4
        imul   ebx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 30h], 0
        add    esp, 4
        cmp    ebx, 5622h
        ja     SHORT loc_5_07EB
        mov    eax, ebx
        sub    edx, edx
        shr    eax, 1
        add    eax, 0f4240h
        div    ebx
        neg    al
        mov    bl, al
        jmp    SHORT loc_5_0804
loc_5_07EB:
        mov    eax, ebx
        sub    edx, edx
        mov    dword ptr [esi + 30h], 1
        shr    eax, 1
        add    eax, 0f424000h
        div    ebx
        neg    ax
        mov    bl, ah
loc_5_0804:
        push   edi
        call   sub_5_0035
        add    esp, 4
        push   40h
        push   edi
        call   sub_5_001C
        add    esp, 8
        test   eax, eax
        je     SHORT loc_5_0830
        push   edi
        call   sub_5_0035
        add    esp, 4
        push   40h
        push   edi
        call   sub_5_001C
        add    esp, 8
loc_5_0830:
        movzx  eax, bl
        push   eax
        push   edi
        call   sub_5_001C
        add    esp, 8
        push   0d1h
        push   edi
        call   sub_5_001C
        add    esp, 8
        cmp    word ptr [esi + 34h], 300h
        jb     SHORT loc_5_0888
        push   0eh
        push   edi
        call   sub_5_0042
        add    esp, 8
        cmp    dword ptr [esi + 2ch], 2
        jne    SHORT loc_5_0868
        or     al, 2
        jmp    SHORT loc_5_0877
loc_5_0868:
        and    al, 0fdh
        cmp    dword ptr [ebp - 4], 4650h
        jae    SHORT loc_5_0877
        and    al, 0dfh
        jmp    SHORT loc_5_0879
loc_5_0877:
        or     al, 20h
loc_5_0879:
        movzx  eax, al
        push   eax
        push   0eh
        push   edi
        call   sub_5_0055
        add    esp, 0ch
loc_5_0888:
        cmp    dword ptr [esi + 2ch], 2
        jne    SHORT loc_5_08FA
        mov    eax, dword ptr [esi + 10h]
        push   1
        mov    bl, byte ptr [eax]
        mov    byte ptr [eax], 80h
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi + 18h]
        call   sub_5_0118
        add    esp, 0ch
        push   dword ptr [esi + 18h]
        call   sub_5_015D
        add    esp, 4
        push   dword ptr [esi + 18h]
        call   sub_5_0102
        add    esp, 4
        push   14h
        push   edi
        call   sub_5_001C
        add    esp, 8
        push   0
        push   edi
        call   sub_5_001C
        add    esp, 8
        push   0
        push   edi
        call   sub_5_001C
        add    esp, 8
        push   64h
        push   dword ptr [esi + 18h]
        call   sub_5_00BD
        add    esp, 8
        push   dword ptr [esi + 18h]
        call   sub_5_00F4
        add    esp, 4
        mov    eax, dword ptr [esi + 10h]
        mov    byte ptr [eax], bl
loc_5_08FA:
        push   dword ptr [esi + 1ch]
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi + 18h]
        call   sub_5_0118
        add    esp, 0ch
        push   dword ptr [esi + 18h]
        call   sub_5_0139
        add    esp, 4
        push   dword ptr [esi + 18h]
        call   sub_5_0102
        mov    bx, word ptr [esi + 1ch]
        add    esp, 4
        cmp    word ptr [esi + 34h], 200h
        jb     SHORT loc_5_096A
        push   48h
        push   edi
        call   sub_5_001C
        mov    ax, bx
        add    esp, 8
        dec    bx
        dec    al
        movzx  ecx, al
        push   ecx
        push   edi
        call   sub_5_001C
        add    esp, 8
        movzx  eax, bh
        push   eax
        push   edi
        call   sub_5_001C
        add    esp, 8
        cmp    dword ptr [esi + 30h], 0
        je     SHORT loc_5_0966
        push   90h
        jmp    SHORT loc_5_098D
loc_5_0966:
        push   1ch
        jmp    SHORT loc_5_098D
loc_5_096A:
        push   14h
        push   edi
        call   sub_5_001C
        mov    ax, bx
        add    esp, 8
        dec    bx
        dec    al
        movzx  ecx, al
        push   ecx
        push   edi
        call   sub_5_001C
        add    esp, 8
        movzx  eax, bh
        push   eax
loc_5_098D:
        push   edi
        call   sub_5_001C
        add    esp, 8
        xor    eax, eax
        mov    dword ptr [esi + 8], 1
loc_5_099F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_5_09A8:
        push   ebx
        mov    eax, 80004005h
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        cmp    dword ptr [esi + 8], 0
        je     NEAR PTR loc_5_0A7E
        cmp    dword ptr [esi + 30h], 0
        movzx  edi, word ptr [esi + 36h]
        je     SHORT loc_5_09D3
        push   edi
        call   sub_5_0035
        add    esp, 4
        jmp    SHORT loc_5_09E1
loc_5_09D3:
        push   0d0h
        push   edi
        call   sub_5_001C
        add    esp, 8
loc_5_09E1:
        mov    ebx, 46h
loc_5_09E6:
        push   0ch
        push   edi
        call   sub_5_000E
        add    esp, 8
        dec    ebx
        jne    SHORT loc_5_09E6
        movzx  eax, byte ptr [esi + 38h]
        push   eax
        call   sub_5_0086
        add    esp, 4
        push   dword ptr [esi + 18h]
        call   sub_5_00F4
        add    esp, 4
        push   0eh
        push   edi
        call   sub_5_000E
        add    esp, 8
        cmp    word ptr [esi + 34h], 300h
        jb     SHORT loc_5_0A67
        push   0ch
        push   edi
        call   sub_5_0042
        add    esp, 8
        or     al, 20h
        movzx  ecx, al
        push   ecx
        push   0ch
        push   edi
        call   sub_5_0055
        add    esp, 0ch
        push   0eh
        push   edi
        call   sub_5_0042
        add    esp, 8
        or     al, 20h
        and    eax, 0fdh
        push   eax
        push   0eh
        push   edi
        call   sub_5_0055
        add    esp, 0ch
        push   0a0h
        push   edi
        call   sub_5_001C
        add    esp, 8
loc_5_0A67:
        push   0d3h
        push   edi
        call   sub_5_001C
        add    esp, 8
        xor    eax, eax
        mov    dword ptr [esi + 8], 0
loc_5_0A7E:
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_5_0A84:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        db  0CDh, 020h, 088h, 001h, 001h, 000h
        test   eax, eax
        jne    SHORT loc_5_0AD1
        mov    dword ptr [ebp - 0ch], 0
        db  0C7h, 045h, 0F0h, 000h, 000h, 000h, 000h
        push   ecx
        mov    edx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 0ch]
        db  0CDh, 020h, 089h, 001h, 001h, 000h
        pop    ecx
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        push   ecx
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        db  0CDh, 020h, 089h, 001h, 001h, 000h
        pop    ecx
        db  0CDh, 020h, 08Ah, 001h, 001h, 000h
loc_5_0AD1:
        mov    esp, ebp
        pop    ebp
        ret
sub_5_0AD5:
        push   ebp
        mov    ebp, esp
        sub    esp, 80h
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  068h, 000h, 000h, 000h, 000h
        lea    edx, [ebp - 80h]
        push   edx
        call   sub_5_1978
        add    esp, 10h
        lea    ecx, [ebp - 80h]
        push   ecx
        call   sub_5_0A84
        mov    esp, ebp
        pop    ebp
        ret
sub_5_0B02:
        push   ebp
        mov    ebp, esp
        sub    esp, 80h
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  068h, 000h, 000h, 000h, 000h
        lea    edx, [ebp - 80h]
        push   edx
        call   sub_5_1978
        add    esp, 10h
        lea    ecx, [ebp - 80h]
        push   ecx
        call   sub_5_0A84
        mov    esp, ebp
        pop    ebp
        ret
sub_5_0B2F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   1
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 8], 4
        push   ecx
        push   3
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp + 8]
        call   sub_5_1980
        add    esp, 1ch
        test   eax, eax
        je     SHORT loc_5_0B68
        mov    dword ptr [ebp - 4], 1
loc_5_0B68:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_5_0B75
        mov    eax, dword ptr [ebp + 0ch]
        or     byte ptr [eax + 1], 2
loc_5_0B75:
        mov    esp, ebp
        pop    ebp
        ret
sub_5_0B79:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 224h
        test   eax, eax
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_5_0BB7
        cmp    eax, 1
        je     SHORT loc_5_0BBE
        cmp    eax, 2
        je     NEAR PTR loc_5_0DD1
        cmp    eax, 3
        je     NEAR PTR loc_5_0E03
        cmp    eax, 4
        je     NEAR PTR loc_5_0DD1
        mov    eax, 1
        jmp    NEAR PTR loc_5_0E2F
loc_5_0BB7:
        xor    eax, eax
        jmp    NEAR PTR loc_5_0E2F
loc_5_0BBE:
        push   0
        lea    eax, [ebp - 224h]
        push   100h
        mov    esi, dword ptr [ebp + 10h]
        push   eax
        push   0
        push   esi
        call   sub_5_1998
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_5_0E2F
        push   0
        lea    eax, [ebp - 124h]
        push   esi
        push   eax
        call   sub_5_1990
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_5_0E2F
        mov    dword ptr [ebp - 8], 0
        movzx  eax, word ptr [ebp - 0c8h]
        mov    bx, 0ffffh
        cmp    eax, 1
        je     SHORT loc_5_0C22
        cmp    eax, 2
        je     SHORT loc_5_0C3F
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0E2F
loc_5_0C22:
        cmp    word ptr [ebp - 9eh], 10h
        je     SHORT loc_5_0C36
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0E2F
loc_5_0C36:
        mov    di, word ptr [ebp - 0c6h]
        jmp    SHORT loc_5_0C8E
loc_5_0C3F:
        cmp    word ptr [ebp - 9eh], 10h
        je     SHORT loc_5_0C53
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0E2F
loc_5_0C53:
        movzx  eax, word ptr [ebp - 9ch]
        mov    di, word ptr [ebp - 0c6h]
        cmp    eax, 2
        je     SHORT loc_5_0C7A
        cmp    eax, 4
        je     SHORT loc_5_0C7A
        cmp    eax, 10h
        je     SHORT loc_5_0C83
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0E2F
loc_5_0C7A:
        mov    bx, word ptr [ebp - 0c4h]
        jmp    SHORT loc_5_0C8E
loc_5_0C83:
        mov    bx, word ptr [ebp - 0c4h]
        add    bx, 8
loc_5_0C8E:
        cmp    word ptr [ebp - 76h], 1
        jb     NEAR PTR loc_5_0DCA
        cmp    word ptr [ebp - 66h], 1
        jb     NEAR PTR loc_5_0DCA
        movzx  ax, byte ptr [ebp - 74h]
        movzx  edx, di
        mov    word ptr [ebp - 2], ax
        mov    dword ptr [ebp - 0ch], edx
        movzx  cx, byte ptr [ebp - 64h]
        mov    word ptr [ebp - 4], cx
        cmp    edx, -1
        jne    SHORT loc_5_0CCB
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0E2F
loc_5_0CCB:
        push   0
        lea    eax, [ebp - 2ch]
        push   0
        lea    ecx, [ebp - 10h]
        mov    dword ptr [ebp - 2ch], 0
        push   eax
        push   ecx
        push   esi
        call   sub_5_1988
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_5_0CF2
        mov    dword ptr [ebp - 10h], 0
loc_5_0CF2:
        mov    eax, dword ptr [ebp - 10h]
        cmp    eax, 8
        je     SHORT loc_5_0D01
        cmp    eax, 10h
        je     SHORT loc_5_0D07
        jmp    SHORT loc_5_0D0B
loc_5_0D01:
        or     byte ptr [ebp - 8], 20h
        jmp    SHORT loc_5_0D0B
loc_5_0D07:
        or     byte ptr [ebp - 8], 10h
loc_5_0D0B:
        lea    eax, [ebp - 8]
        push   eax
        push   esi
        call   sub_5_0B2F
        add    esp, 8
        push   dword ptr [ebp - 8]
        push   10h
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 2]
        push   ebx
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0D2B:
        add    esp, 1ch
        mov    edi, eax
        test   edi, edi
        jne    NEAR PTR loc_5_0DB9
        sub    bx, 0ffffh
        mov    dword ptr [ebp - 20h], esi
        db  0C7h, 045h, 0DCh, 000h, 000h, 000h, 000h
        cmp    bx, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [ebp - 28h], eax
        push   dword ptr [ebp - 28h]
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 20h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 001h, 000h, 04Ah, 004h
        add    esp, 0ch
        test   edi, edi
        jne    SHORT loc_5_0DC6
        cmp    word ptr [ebp - 2], 9
        mov    eax, 2
        je     SHORT loc_5_0D7D
        movzx  eax, word ptr [ebp - 2]
loc_5_0D7D:
        movzx  ecx, word ptr [ebp - 4]
        push   ecx
        push   eax
        push   dword ptr [ebp - 0ch]
        db  068h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 4ch]
        push   ecx
        call   sub_5_1978
        add    esp, 14h
        lea    ecx, [ebp - 4ch]
        db  0C7h, 045h, 0E8h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 1ch], ecx
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 002h, 000h, 04Ah, 004h
        add    esp, 8
loc_5_0DB9:
        test   edi, edi
        jne    SHORT loc_5_0DC6
        push   esi
        call   sub_5_0520
        add    esp, 4
loc_5_0DC6:
        mov    eax, edi
        jmp    SHORT loc_5_0E2F
loc_5_0DCA:
        mov    eax, 13h
        jmp    SHORT loc_5_0E2F
loc_5_0DD1:
        mov    esi, dword ptr [ebp + 10h]
        push   esi
        call   sub_5_05DF
        add    esp, 4
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0DE3:
        db  0C7h, 045h, 0ECh, 000h, 000h, 000h, 000h
        add    esp, 4
        push   dword ptr [ebp - 14h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 004h, 000h, 04Ah, 004h
        add    esp, 4
        xor    eax, eax
        jmp    SHORT loc_5_0E2F
loc_5_0E03:
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        je     SHORT loc_5_0E16
        cmp    eax, 1
        je     SHORT loc_5_0E16
        mov    eax, 1
        jmp    SHORT loc_5_0E2F
loc_5_0E16:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E1E:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 13h
loc_5_0E2F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  0CCh, 0CCh
loc_5_0E38:
        push   eax
        mov    eax, dword ptr [esi + 28h]
        cmp    eax, ebx
        je     SHORT loc_5_0E63
        or     eax, eax
        jne    SHORT loc_5_0E76
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 1
        test   word ptr [esi + 4], 100h
        je     SHORT loc_5_0E59
        or     eax, 2
loc_5_0E59:
        call   sub_5_11B9
        pop    edx
        pop    ecx
        pop    ebx
        jb     SHORT loc_5_0E76
loc_5_0E63:
        pop    eax
        cmp    ecx, 4
        jbe    SHORT loc_5_0E6F
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_5_0E6F:
        je     SHORT loc_5_0E74
        in     al, dx
        jmp    SHORT loc_5_0E75
loc_5_0E74:
        out    dx, al
loc_5_0E75:
        ret
loc_5_0E76:
        mov    eax, 1
        mov    edi, esi
        call   sub_5_1036
        pop    eax
        xor    eax, eax
        dec    eax
        ret
sub_5_0E87:
        push   eax
        push   edx
        push   ebx
        push   ecx
        movzx  edx, word ptr [edi + 8]
        mov    ebx, 105h
        mov    al, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E9B:
        mov    ebx, 4
        mov    al, 60h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0EA7:
        mov    ebx, 104h
        mov    al, 3fh
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0EB3:
        mov    ebx, 8
        xor    al, al
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0EBF:
        mov    ebx, 0bdh
        mov    al, 0c0h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0ECB:
        mov    ecx, 15h
        mov    ebx, 40h
        mov    al, 3fh
loc_5_0ED7:
        xor    bh, bh
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0EDE:
        mov    bh, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0EE5:
        inc    bl
        loop   loc_5_0ED7
        mov    ecx, 8
        mov    ebx, 0b0h
        xor    eax, eax
loc_5_0EF5:
        xor    bh, bh
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0EFC:
        mov    bh, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0F03:
        inc    bl
        loop   loc_5_0EF5
        mov    ebx, 104h
        xor    eax, eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0F13:
        mov    ebx, 105h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0F1D:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0F22:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0F27:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0F2C:
        pop    ecx
        pop    ebx
        pop    edx
        pop    eax
        ret
        db  02Eh, 08Bh, 0C0h
loc_5_0F34:
        push   eax
        test   word ptr [esi + 4], 100h
        je     SHORT loc_5_0F61
        mov    eax, dword ptr [esi + 30h]
        cmp    eax, ebx
        je     SHORT loc_5_0F61
        or     eax, eax
        jne    SHORT loc_5_0F74
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 2
        call   sub_5_11B9
        pop    edx
        pop    ecx
        pop    ebx
        jb     SHORT loc_5_0F74
        call   sub_5_0E87
loc_5_0F61:
        pop    eax
        cmp    ecx, 4
        jbe    SHORT loc_5_0F6D
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_5_0F6D:
        je     SHORT loc_5_0F72
        in     al, dx
        jmp    SHORT loc_5_0F73
loc_5_0F72:
        out    dx, al
loc_5_0F73:
        ret
loc_5_0F74:
        mov    eax, 2
        mov    edi, esi
        call   sub_5_1036
        pop    eax
        xor    eax, eax
        dec    eax
        ret
sub_5_0F85:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 0fh
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_0F93:
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_0FA4
        inc    cx
        jmp    SHORT loc_5_0F93
loc_5_0FA4:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_0FAA:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 0fh
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_0FB8:
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_0FC9
        inc    cx
        jmp    SHORT loc_5_0FB8
loc_5_0FC9:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_0FCF:
        push   eax
        push   ebx
        push   edx
        movzx  edx, word ptr [edi + 8]
        mov    ebx, 4
        mov    eax, 60h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0FE5:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0FEA:
        pop    edx
        pop    ebx
        pop    eax
        ret
sub_5_0FEE:
        push   esi
        push   eax
        push   ecx
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 8]
loc_5_0FFB:
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_100C
        inc    cx
        jmp    SHORT loc_5_0FFB
loc_5_100C:
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_1011:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 8]
loc_5_101F:
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_5_1030
        inc    cx
        jmp    SHORT loc_5_101F
loc_5_1030:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
sub_5_1036:
        test   word ptr [edi + 4], 1
        jne    SHORT loc_5_108B
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1043:
        jb     SHORT loc_5_108B
        cmp    eax, 2
        je     SHORT loc_5_1064
        test   dword ptr [esi], 1
        jne    SHORT loc_5_108B
        or     dword ptr [esi], 1
        db  00Fh, 0B7h, 00Dh, 00Ch, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        jmp    SHORT loc_5_107C
loc_5_1064:
        test   dword ptr [esi], 2
        jne    SHORT loc_5_108B
        or     dword ptr [esi], 2
        db  00Fh, 0B7h, 00Dh, 00Eh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
loc_5_107C:
        mov    eax, 30h
        xor    esi, esi
        xor    edi, edi
        db  0CDh, 020h, 004h, 000h, 017h, 000h
loc_5_108B:
        ret
loc_5_108C:
        pushfd
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_10D3
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_5_10D3
loc_5_10A0:
        popfd
        pushfd
        push   eax
        mov    edi, dword ptr [eax]
        xor    eax, eax
        cmp    dword ptr [edi + 28h], ebx
        jne    SHORT loc_5_10B6
        or     eax, 1
        mov    dword ptr [edi + 2ch], 0ffffffffh
loc_5_10B6:
        cmp    dword ptr [edi + 30h], ebx
        jne    SHORT loc_5_10BE
        or     eax, 2
loc_5_10BE:
        or     eax, eax
        je     SHORT loc_5_10C7
        call   sub_5_1149
loc_5_10C7:
        pop    eax
        cli
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_5_10D3
        jmp    SHORT loc_5_10A0
loc_5_10D3:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        xor    esi, esi
        xchg   dword ptr [edi + ebx], esi
        or     esi, esi
        je     SHORT loc_5_10E8
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_5_10E8:
        popfd
        clc
        ret
loc_5_10EB:
        pushad
        pushfd
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    eax, 5
        mov    ecx, 8
        cli
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_5_1141
        mov    dword ptr [edi + ebx], esi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_1145
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_5_1145
loc_5_111B:
        mov    edi, dword ptr [eax]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1122:
        jb     SHORT loc_5_112E
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_5_1145
        jmp    SHORT loc_5_111B
loc_5_112E:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     SHORT loc_5_1141
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_5_1141:
        popfd
        stc
        jmp    SHORT loc_5_1147
loc_5_1145:
        popfd
        clc
loc_5_1147:
        popad
        ret
sub_5_1149:
        push   eax
        push   edx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1151:
        jb     SHORT loc_5_11B4
        test   eax, 2
        je     SHORT loc_5_1173
        cmp    dword ptr [edi + 30h], ebx
        jne    SHORT loc_5_1173
        call   sub_5_0FCF
        call   sub_5_0FEE
        mov    dword ptr [edi + 30h], 0
        and    dword ptr [esi], 0fffffffdh
loc_5_1173:
        test   eax, 1
        je     SHORT loc_5_11B4
        cmp    dword ptr [edi + 28h], ebx
        jne    SHORT loc_5_11B4
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_5_1190
        mov    eax, dword ptr [edi + 38h]
        db  0CDh, 020h, 014h, 000h, 004h, 000h
loc_5_1190:
        call   sub_5_0F85
        mov    dword ptr [edi + 28h], 0
        mov    eax, dword ptr [edi + 24h]
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        and    dword ptr [esi], 0fffffffeh
loc_5_11B4:
        pop    esi
        pop    edx
        pop    eax
        clc
        ret
sub_5_11B9:
        push   eax
        push   ecx
        push   edx
        test   eax, 2
        je     SHORT loc_5_11DB
        cmp    dword ptr [edi + 30h], 0
        je     SHORT loc_5_11D0
        cmp    dword ptr [edi + 30h], ebx
        je     SHORT loc_5_11DB
        jmp    SHORT loc_5_11ED
loc_5_11D0:
        mov    dword ptr [edi + 30h], ebx
        call   sub_5_1011
        mov    dword ptr [edi + 34h], ebx
loc_5_11DB:
        test   eax, 1
        je     SHORT loc_5_123A
        cmp    dword ptr [edi + 28h], 0
        je     SHORT loc_5_11F0
        cmp    dword ptr [edi + 28h], ebx
        je     SHORT loc_5_1239
loc_5_11ED:
        stc
        jb     SHORT loc_5_123A
loc_5_11F0:
        mov    dword ptr [edi + 28h], ebx
        cmp    dword ptr [edi + 2ch], ebx
        je     SHORT loc_5_1201
        movzx  edx, word ptr [edi + 6]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1201:
        mov    dword ptr [edi + 2ch], ebx
        call   sub_5_0FAA
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_5_121A
        mov    eax, dword ptr [edi + 38h]
        db  0CDh, 020h, 013h, 000h, 004h, 000h
loc_5_121A:
        mov    eax, dword ptr [edi + 24h]
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   ecx, 10h
        je     SHORT loc_5_1233
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        jmp    SHORT loc_5_1239
loc_5_1233:
        db  0CDh, 020h, 009h, 000h, 003h, 000h
loc_5_1239:
        clc
loc_5_123A:
        pop    edx
        pop    ecx
        pop    eax
        ret
sub_5_123E:
        mov    eax, 400h
        clc
        ret
        db  0CCh, 0CCh, 0CCh
mssblst_PM_API:
        movzx  eax, word ptr [ebp + 14h]
        cmp    ah, 3
        jae    SHORT loc_5_1272
        movzx  ecx, ah
        shl    ecx, 3
        db  08Dh, 089h, 000h, 000h, 000h, 000h
        movzx  eax, al
        cmp    eax, dword ptr [ecx]
        jae    SHORT loc_5_1272
        mov    ecx, dword ptr [ecx + 4]
        call   dword ptr [ecx + eax*4]
        jb     SHORT loc_5_1272
        and    word ptr [ebp + 2ch], 0fffeh
        ret
loc_5_1272:
        or     word ptr [ebp + 2ch], 1
        ret
loc_5_1278:
        call   sub_5_123E
        mov    word ptr [ebp + 1ch], ax
        clc
        ret
loc_5_1283:
        movzx  eax, word ptr [ebp + 10h]
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_12C5
        test   eax, 1
        je     SHORT loc_5_12AA
        push   2
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_129E:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_12C5
        mov    ecx, dword ptr [edi + 2ch]
        jmp    SHORT loc_5_12CD
loc_5_12AA:
        test   eax, 2
        je     SHORT loc_5_12C5
        push   3
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_12B9:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_12C5
        mov    ecx, dword ptr [edi + 34h]
        jmp    SHORT loc_5_12CD
loc_5_12C5:
        mov    word ptr [ebp + 1ch], 1
loc_5_12CB:
        stc
        ret
loc_5_12CD:
        mov    word ptr [ebp + 1ch], 0
        call   sub_5_11B9
        jb     SHORT loc_5_12DB
        ret
loc_5_12DB:
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_5_12CB
loc_5_12E3:
        movzx  eax, word ptr [ebp + 10h]
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_133F
        test   eax, 1
        je     SHORT loc_5_1324
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_5_1310
        push   2
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1306:
        add    esp, 8
        or     edi, edi
        jne    SHORT loc_5_131F
        add    edx, 4
loc_5_1310:
        push   2
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1318:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_133F
loc_5_131F:
        mov    ecx, dword ptr [edi + 28h]
        jmp    SHORT loc_5_1347
loc_5_1324:
        test   eax, 2
        je     SHORT loc_5_133F
        push   3
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1333:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_133F
        mov    ecx, dword ptr [edi + 30h]
        jmp    SHORT loc_5_1347
loc_5_133F:
        mov    word ptr [ebp + 1ch], 1
loc_5_1345:
        stc
        ret
loc_5_1347:
        cmp    ecx, ebx
        je     SHORT loc_5_1353
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_5_1345
loc_5_1353:
        call   sub_5_1149
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
loc_5_1360:
        pushfd
        mov    ax, word ptr [ebp + 1ch]
        and    ax, 0fffeh
        jne    SHORT loc_5_13CE
        xor    ecx, ecx
        mov    ax, word ptr [ebp + 1ch]
        and    ax, 1
        je     SHORT loc_5_137A
        mov    ecx, dword ptr [ebp + 18h]
loc_5_137A:
        cli
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_13CE
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_5_13CE
        mov    edi, dword ptr [eax]
        jecxz  loc_5_13A2
loc_5_1391:
        cmp    dword ptr [edi + 40h], ecx
        je     SHORT loc_5_13A2
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_5_13CE
        mov    edi, dword ptr [eax]
        jmp    SHORT loc_5_1391
loc_5_13A2:
        popfd
        pushfd
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cmp    edi, -1
        je     SHORT loc_5_13CE
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     SHORT loc_5_13CE
        cmp    ecx, 52h
        ja     SHORT loc_5_13CE
        cld
        rep movsb
        mov    word ptr [ebp + 1ch], 1
        popfd
        clc
        ret
loc_5_13CE:
        mov    word ptr [ebp + 1ch], 0
        popfd
        stc
        ret
loc_5_13D7:
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_13E3:
        add    esp, 8
        jb     SHORT loc_5_13FE
        mov    eax, dword ptr [edi + 38h]
        or     eax, eax
        je     SHORT loc_5_13FE
        push   ecx
        db  0CDh, 020h, 01Ch, 000h, 004h, 000h
        mov    word ptr [ebp + 1ch], cx
        pop    ecx
        clc
        jmp    SHORT loc_5_1405
loc_5_13FE:
        mov    word ptr [ebp + 1ch], 0ffffh
        stc
loc_5_1405:
        pop    edi
        ret
loc_5_1407:
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_143C
        push   3
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1417:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_143C
        mov    ecx, dword ptr [edi + 34h]
        mov    word ptr [ebp + 1ch], 0
        mov    eax, 2
        call   sub_5_11B9
        jb     SHORT loc_5_1434
        ret
loc_5_1434:
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_5_1442
loc_5_143C:
        mov    word ptr [ebp + 1ch], 1
loc_5_1442:
        stc
        ret
loc_5_1444:
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_147C
        push   3
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1454:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_147C
        mov    ecx, dword ptr [edi + 30h]
        cmp    ecx, ebx
        je     SHORT loc_5_146A
        mov    word ptr [ebp + 1ch], 3
        jmp    SHORT loc_5_1482
loc_5_146A:
        mov    eax, 2
        call   sub_5_1149
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
loc_5_147C:
        mov    word ptr [ebp + 1ch], 1
loc_5_1482:
        stc
        ret
loc_5_1484:
        pushfd
        mov    ax, word ptr [ebp + 1ch]
        cmp    ax, 1
        jne    SHORT loc_5_1507
        mov    ecx, dword ptr [ebp + 18h]
        cli
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_5_1507
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_5_1507
loc_5_14A5:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 40h], ecx
        je     SHORT loc_5_14B6
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_5_1507
        jmp    SHORT loc_5_14A5
loc_5_14B6:
        popfd
        pushfd
        cmp    word ptr [edi + 8], -1
        je     SHORT loc_5_1507
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cmp    edi, -1
        je     SHORT loc_5_1507
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     SHORT loc_5_1507
        cmp    ecx, 1ch
        ja     SHORT loc_5_1507
        xor    ax, ax
        cmp    word ptr [esi + 0ch], 302h
        jbe    SHORT loc_5_14EC
        mov    ax, 1
loc_5_14EC:
        mov    word ptr [edi + 4], ax
        mov    ax, word ptr [esi + 8]
        mov    word ptr [edi + 6], ax
        mov    eax, dword ptr [esi + 40h]
        mov    dword ptr [edi + 0ch], eax
        mov    word ptr [ebp + 1ch], 1
        popfd
        clc
        ret
loc_5_1507:
        mov    word ptr [ebp + 1ch], 0
        popfd
        stc
        ret
loc_5_1510:
        xchg   esi, eax
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        push   dword ptr [ebp + 3ch]
        push   dword ptr [ebp + 14h]
        mov    eax, dword ptr [esi + 4]
        mov    word ptr [ebp + 14h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 3ch], ax
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [ebp + 14h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 3ch], ax
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    dword ptr [esi + 8], eax
        pop    dword ptr [ebp + 14h]
        pop    dword ptr [ebp + 3ch]
        mov    eax, dword ptr [esi]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1568:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_1594
        lea    eax, [edi + 3ch]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   eax
        push   dword ptr [esi]
        call   sub_5_16C7
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_5_1594
        mov    word ptr [ebp + 1ch], ax
        shr    eax, 10h
        mov    word ptr [ebp + 14h], ax
        clc
        ret
loc_5_1594:
        mov    word ptr [ebp + 1ch], 0
        mov    word ptr [ebp + 14h], 0
        stc
        ret
loc_5_15A2:
        xchg   esi, eax
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        mov    eax, dword ptr [esi]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_15B8:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_5_15CE
        lea    eax, [edi + 3ch]
        push   dword ptr [esi + 4]
        push   eax
        call   sub_5_1857
        add    esp, 8
loc_5_15CE:
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
sub_5_15D6:
        push   ebp
        mov    ebp, esp
        sub    esp, 6ch
        mov    eax, 0ffffffffh
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   esi, esi
        je     NEAR PTR loc_5_16C0
        lea    eax, [ebp - 6ch]
        push   eax
        call   sub_5_18DE
        add    esp, 4
        call   sub_5_18CC
        call   sub_5_191B
        mov    edi, dword ptr [eax + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_190D
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_190D
        add    esp, 4
        mov    ebx, dword ptr [ebp + 10h]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_190D
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_190D
        add    esp, 4
        mov    ebx, dword ptr [ebp + 14h]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_190D
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_190D
        add    esp, 4
        mov    ebx, dword ptr [ebp + 18h]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_190D
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_190D
        movzx  ecx, si
        shr    esi, 10h
        add    esp, 4
        movzx  eax, si
        push   ecx
        push   eax
        call   sub_5_18FE
        add    esp, 8
        call   sub_5_18D8
        movzx  esi, word ptr [edi + 14h]
        shl    esi, 10h
        movzx  eax, word ptr [edi + 1ch]
        or     esi, eax
        call   sub_5_18D2
        lea    ecx, [ebp - 6ch]
        push   ecx
        call   sub_5_18EE
        add    esp, 4
        mov    eax, esi
loc_5_16C0:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_16C7:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        push   ebp
        mov    ebp, dword ptr [esi]
        test   ebp, ebp
        jne    SHORT loc_5_16ED
        push   15h
        push   4
        call   sub_5_19E4
        mov    ebp, eax
        test   ebp, ebp
        jne    SHORT loc_5_16EB
        xor    eax, eax
        jmp    NEAR PTR loc_5_1852
loc_5_16EB:
        mov    dword ptr [esi], ebp
loc_5_16ED:
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 8
        jbe    SHORT loc_5_170A
        mov    eax, dword ptr [esp + 1ch]
        mov    byte ptr [eax + 8], 0
loc_5_170A:
        push   ebp
        call   sub_5_19D4
        mov    ebx, eax
        test   ebx, ebx
        je     NEAR PTR loc_5_17FE
loc_5_171A:
        mov    ecx, dword ptr [esp + 1ch]
        lea    edx, [ebx + 0ch]
loc_5_1721:
        mov    al, byte ptr [ecx]
        cmp    al, byte ptr [edx]
        jne    SHORT loc_5_1741
        or     al, al
        je     SHORT loc_5_173D
        mov    al, byte ptr [ecx + 1]
        cmp    al, byte ptr [edx + 1]
        jne    SHORT loc_5_1741
        add    ecx, 2
        add    edx, 2
        or     al, al
        jne    SHORT loc_5_1721
loc_5_173D:
        xor    eax, eax
        jmp    SHORT loc_5_1746
loc_5_1741:
        sbb    eax, eax
        sbb    eax, -1
loc_5_1746:
        test   eax, eax
        je     SHORT loc_5_1757
        push   ebx
        push   ebp
        call   sub_5_19C4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_5_171A
loc_5_1757:
        test   ebx, ebx
        je     NEAR PTR loc_5_17FE
        cmp    dword ptr [ebx + 4], 0
        je     SHORT loc_5_176C
        xor    eax, eax
        jmp    NEAR PTR loc_5_1852
loc_5_176C:
        push   0
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_15D6
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_5_1799
        push   ebx
        mov    dword ptr [ebx], 0
        push   esi
        call   sub_5_1857
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_5_1852
loc_5_1799:
        mov    eax, dword ptr [esp + 20h]
        push   0
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebx + 4], ecx
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   ecx
        call   sub_5_15D6
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_5_17D0
        mov    dword ptr [ebx + 4], 0
        push   ebx
        push   esi
        call   sub_5_1857
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_5_1852
loc_5_17D0:
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx + 8]
        push   10000h
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_15D6
        add    esp, 14h
        push   dword ptr [ebx]
        push   dword ptr [ebx + 8]
        push   10000h
        push   ebx
        push   dword ptr [ebx + 4]
        call   sub_5_15D6
        add    esp, 14h
        jmp    SHORT loc_5_1850
loc_5_17FE:
        push   ebp
        call   sub_5_19B0
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_5_180E
        xor    eax, eax
        jmp    SHORT loc_5_1852
loc_5_180E:
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 20h]
        mov    edi, dword ptr [esp + 1ch]
        mov    dword ptr [ebx + 8], eax
        mov    edx, dword ptr [ecx + 4]
        sub    eax, eax
        mov    ecx, 0ffffffffh
        mov    dword ptr [ebx], edx
        mov    dword ptr [ebx + 4], 0
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        lea    edi, [ebx + 0ch]
        rep movsd
        mov    ecx, eax
        push   ebx
        and    ecx, 3
        push   ebp
        rep movsb
        call   sub_5_19A0
loc_5_1850:
        mov    eax, ebx
loc_5_1852:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_1857:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        test   edi, edi
        je     SHORT loc_5_1896
        mov    eax, dword ptr [edi]
        test   eax, eax
        je     SHORT loc_5_187B
        push   0
        push   dword ptr [edi + 8]
        push   40000h
        push   edi
        push   eax
        call   sub_5_15D6
        add    esp, 14h
loc_5_187B:
        mov    eax, dword ptr [edi + 4]
        test   eax, eax
        je     SHORT loc_5_1896
        push   0
        push   dword ptr [edi + 8]
        push   40000h
        push   edi
        push   eax
        call   sub_5_15D6
        add    esp, 14h
loc_5_1896:
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     SHORT loc_5_18C7
        push   edi
        push   eax
        call   sub_5_1A18
        push   edi
        push   dword ptr [esi]
        call   sub_5_1A08
        push   dword ptr [esi]
        call   sub_5_19D4
        test   eax, eax
        jne    SHORT loc_5_18C1
        push   dword ptr [esi]
        call   sub_5_19F8
loc_5_18C1:
        mov    dword ptr [esi], 0
loc_5_18C7:
        pop    edi
        pop    esi
        ret
        db  0CCh, 0CCh
sub_5_18CC:
        db  0CDh, 020h, 083h, 080h, 001h, 000h
sub_5_18D2:
        db  0CDh, 020h, 086h, 080h, 001h, 000h
sub_5_18D8:
        db  0CDh, 020h, 085h, 080h, 001h, 000h
sub_5_18DE:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        pop    ebp
        ret
sub_5_18EE:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        pop    ebp
        ret
sub_5_18FE:
        movzx  ecx, word ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        db  0CDh, 020h, 048h, 080h, 001h, 000h
sub_5_190D:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        pop    ebp
        ret
sub_5_191B:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
        db  0CCh, 0CCh
sub_5_1928:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        int3
        int3
sub_5_1930:
        db  0CDh, 020h, 07Bh, 081h, 001h, 000h
        int3
        int3
sub_5_1938:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        int3
        int3
sub_5_1940:
        pop    ecx
        pop    eax
        pop    edx
        push   ebx
        mov    ebx, eax
        db  0CDh, 020h, 002h, 000h, 027h, 000h
        sbb    edx, edx
        and    eax, edx
        pop    ebx
        jmp    ecx
        db  0CCh
sub_5_1954:
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 0ch]
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        jb     SHORT loc_5_1972
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_5_1972:
        ret    10h
        db  0CCh, 0CCh, 0CCh
sub_5_1978:
        db  0CDh, 020h, 074h, 081h, 001h, 000h
        int3
        int3
sub_5_1980:
        db  0CDh, 020h, 03Eh, 080h, 033h, 000h
        int3
        int3
sub_5_1988:
        db  0CDh, 020h, 044h, 080h, 033h, 000h
        int3
        int3
sub_5_1990:
        db  0CDh, 020h, 03Bh, 080h, 033h, 000h
        int3
        int3
sub_5_1998:
        db  0CDh, 020h, 03Dh, 080h, 033h, 000h
        int3
        int3
sub_5_19A0:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_19B0:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        mov    esi, ecx
        cmc
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
        db  0CCh
sub_5_19C4:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_19D4:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_19E4:
        pop    edx
        pop    eax
        pop    ecx
        push   esi
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        cmc
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
sub_5_19F8:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_1A08:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_1A18:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        mov    esi, ecx
        sbb    eax, eax
        inc    eax
        jmp    edx
_PTEXT ends
        end
