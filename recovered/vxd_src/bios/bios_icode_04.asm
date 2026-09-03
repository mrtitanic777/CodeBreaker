; bios_icode_04.asm -- BIOS object 4 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

sub_4_0000:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0004:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0008:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_000C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0014:
        inc    edx
        dec    ecx
        dec    edi
        push   ebx
        pop    esp
        sub    dl, byte ptr [eax + 4eh]
        push   eax
        xor    byte ptr [ebx + 30h], al
        xor    ebx, dword ptr [eax + esi]
loc_4_0024:
        inc    esp
        insb
        insb
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_002C:
        inc    ecx
        xor    byte ptr [eax], al
        add    byte ptr [ebx - 74aeeb14h], al
        inc    esp
        and    al, 1ch
        mov    ecx, dword ptr [eax + 8]
        db  03Bh, 00Dh, 000h, 000h, 000h, 000h
        ja     SHORT loc_4_00A0
        add    ecx, dword ptr [eax + 0ch]
        db  03Bh, 00Dh, 000h, 000h, 000h, 000h
        jbe    SHORT loc_4_00A0
        mov    ecx, dword ptr [eax + 10h]
        mov    dword ptr [esp + 14h], ecx
        mov    ecx, dword ptr [eax + 8]
        mov    dword ptr [esp + 0ch], ecx
        db  02Bh, 00Dh, 000h, 000h, 000h, 000h
        jecxz  loc_4_007F
        neg    ecx
        mov    dword ptr [esp + 10h], ecx
        lea    eax, [esp + 4]
        push   dword ptr [esp + 20h]
        push   eax
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     SHORT loc_4_00AD
loc_4_007F:
        mov    eax, dword ptr [esp + 1ch]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        inc    ecx
        mov    dword ptr [esp + 0ch], ecx
        sub    ecx, dword ptr [eax + 8]
        sub    ecx, dword ptr [eax + 0ch]
        jecxz  loc_4_00AD
        neg    ecx
        mov    dword ptr [esp + 10h], ecx
        lea    eax, [esp + 4]
loc_4_00A0:
        push   dword ptr [esp + 20h]
        push   eax
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        pop    ecx
        pop    ecx
loc_4_00AD:
        pop    ecx
        add    esp, 14h
        ret
sub_4_00B2:
        push   ebp
        mov    eax, 0
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 4], eax
        db  0CDh, 020h, 000h, 000h, 029h, 000h
        setae  byte ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], eax
        je     SHORT loc_4_00DB
        db  0CDh, 020h, 001h, 000h, 029h, 000h
        mov    dword ptr [ebp - 4], eax
loc_4_00DB:
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret
sub_4_00E2:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   0
        push   1
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_00F2:
        add    esp, 0ch
        db  0A3h, 000h, 000h, 000h, 000h
        db  081h, 03Dh, 000h, 000h, 000h, 000h, 000h, 000h, 011h, 000h
        jae    SHORT loc_4_011D
        mov    eax, 100a6h
        db  08Dh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        db  089h, 035h, 000h, 000h, 000h, 000h
loc_4_011D:
        push   0
        push   0fah
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_012E:
        pop    esi
        ret
sub_4_0130:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   esi
        lea    eax, [ebp - 0ch]
        push   edi
        xor    esi, esi
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0149:
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_4_01D9
        push   2
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 0ch]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0162:
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_4_01D9
        push   esi
        lea    ecx, [ebp - 4]
        push   esi
        push   4
        push   dword ptr [ebp - 8]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0179:
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_4_01D9
        mov    eax, dword ptr [ebp - 4]
        push   esi
        mov    di, word ptr [eax + 2]
        push   esi
        push   2
        lea    eax, [ebp - 4]
        push   dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0197:
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_4_01D9
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 4]
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 24h], esi
        mov    word ptr [ebp - 2ch], di
        mov    dword ptr [ebp - 20h], esi
        mov    dword ptr [ebp - 1ch], esi
        mov    dword ptr [ebp - 18h], esi
        lea    edi, [ebp - 2ch]
        mov    dword ptr [ebp - 14h], esi
        db  0C7h, 045h, 0D8h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 2ah], 1
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_01D4:
        db  0A3h, 000h, 000h, 000h, 000h
loc_4_01D9:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_4_01DF:
        sub    esp, 4
        push   esi
        xor    esi, esi
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_0229
        cmp    ecx, 0c0000000h
        jb     SHORT loc_4_0229
        cmp    ecx, 0ffbfffffh
        ja     SHORT loc_4_0229
        shr    edx, 0ch
        push   esi
        lea    eax, [esp + 8]
        push   eax
        push   1
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_020E:
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_4_0229
        mov    eax, dword ptr [esp + 4]
        xor    eax, dword ptr [esp + 0ch]
        test   eax, 0fffff000h
        je     SHORT loc_4_0229
        mov    esi, 1
loc_4_0229:
        mov    eax, esi
        pop    esi
        add    esp, 4
        ret    4
sub_4_0232:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        call   sub_4_00B2
        test   eax, eax
        je     SHORT loc_4_0254
        mov    ecx, 1
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], ecx
        jmp    SHORT loc_4_026F
loc_4_0254:
        push   0
        push   10000h
        push   0f0000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0265:
        mov    dword ptr [ebp - 8], 1000h
        add    esp, 0ch
loc_4_026F:
        mov    dword ptr [ebp - 0ch], eax
loc_4_0272:
        cmp    dword ptr [eax], 506e5024h
        jne    SHORT loc_4_0299
        mov    esi, eax
        mov    edi, eax
        mov    cl, byte ptr [eax + 5]
        xor    dl, dl
        test   cl, cl
        je     SHORT loc_4_0290
        mov    bl, cl
loc_4_0289:
        add    dl, byte ptr [edi]
        inc    edi
        dec    bl
        jne    SHORT loc_4_0289
loc_4_0290:
        test   dl, dl
        jne    SHORT loc_4_0299
        cmp    cl, 13h
        jae    SHORT loc_4_02AD
loc_4_0299:
        add    eax, 10h
        dec    dword ptr [ebp - 8]
        jne    SHORT loc_4_0272
loc_4_02A1:
        mov    eax, 24h
loc_4_02A6:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_02AD:
        cmp    cl, 21h
        jb     SHORT loc_4_02A1
        cmp    byte ptr [esi + 4], 10h
        jb     SHORT loc_4_02A1
        lea    edi, [esi + 13h]
        mov    eax, edi
        mov    edx, edi
        sub    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [edi]
        add    eax, 0f0000h
        push   eax
        call   sub_4_01DF
        test   eax, eax
        je     SHORT loc_4_02D7
        mov    edi, dword ptr [edi]
        jmp    SHORT loc_4_02EA
loc_4_02D7:
        push   0
        push   10000h
        push   dword ptr [edi]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_02E5:
        add    esp, 0ch
        mov    edi, eax
loc_4_02EA:
        mov    ax, word ptr [esi + 11h]
        lea    ebx, [esi + 1dh]
        mov    word ptr [ebp - 4], ax
        mov    edx, ebx
        mov    eax, ebx
        mov    ecx, dword ptr [ebx]
        sub    eax, dword ptr [ebp - 0ch]
        add    eax, 0f0000h
        push   eax
        call   sub_4_01DF
        test   eax, eax
        je     SHORT loc_4_0311
        mov    ebx, dword ptr [ebx]
        jmp    SHORT loc_4_0324
loc_4_0311:
        push   0
        push   10000h
        push   dword ptr [ebx]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_031F:
        add    esp, 0ch
        mov    ebx, eax
loc_4_0324:
        mov    cx, word ptr [esi + 6]
        mov    eax, dword ptr [esi + 17h]
        mov    word ptr [ebp - 2], cx
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 9]
        cmp    edi, -1
        db  0A3h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_4_056B
        cmp    ebx, -1
        je     NEAR PTR loc_4_056B
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_036B
        movzx  eax, byte ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 8]
        shl    ecx, 8
        add    eax, ecx
        shl    eax, 8
        db  0A3h, 000h, 000h, 000h, 000h
loc_4_036B:
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, 7ae110eh
        je     SHORT loc_4_0385
        cmp    eax, 0aae110eh
        je     SHORT loc_4_0385
        cmp    eax, 0bae110eh
        jne    SHORT loc_4_038F
loc_4_0385:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_038F:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_03C9
        test   edi, edi
        je     NEAR PTR loc_4_02A1
        push   1
        mov    edx, 10000h
        mov    ecx, edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_03AE:
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        test   ax, ax
        jne    SHORT loc_4_03BF
        db  0CDh, 020h, 0BFh, 080h, 001h, 000h
loc_4_03BF:
        mov    ax, word ptr [ebp - 4]
        db  066h, 0A3h, 000h, 000h, 000h, 000h
loc_4_03C9:
        cmp    ebx, -1
        je     SHORT loc_4_03ED
        push   0
        mov    edx, 10000h
        mov    ecx, ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_03DC:
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        test   ax, ax
        jne    SHORT loc_4_03ED
        db  0CDh, 020h, 0BFh, 080h, 001h, 000h
loc_4_03ED:
        push   201h
        push   4
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_03F9:
        add    esp, 8
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_4_040B
        db  0CDh, 020h, 0BFh, 080h, 001h, 000h
loc_4_040B:
        push   0
        mov    edx, 4
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_041D:
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        test   ax, ax
        jne    SHORT loc_4_042E
        db  0CDh, 020h, 0BFh, 080h, 001h, 000h
loc_4_042E:
        xor    esi, esi
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_047D
        push   1
        push   esi
        push   esi
        push   esi
        push   esi
        push   esi
        push   1
        push   4
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0448:
        add    esp, 20h
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    eax, esi
        jne    SHORT loc_4_045A
        db  0CDh, 020h, 0BFh, 080h, 001h, 000h
loc_4_045A:
        push   0
        mov    edx, 4000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_046C:
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        test   ax, ax
        jne    SHORT loc_4_047D
        db  0CDh, 020h, 0BFh, 080h, 001h, 000h
loc_4_047D:
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0482:
        test   ax, ax
        jne    NEAR PTR loc_4_02A1
        db  0A1h, 000h, 000h, 000h, 000h
        push   202h
        movzx  ecx, word ptr [eax + 2]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        lea    esi, [ecx*2 + 19h]
        push   esi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_04AD:
        add    esp, 0ch
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_4_04BF
        db  0CDh, 020h, 0BFh, 080h, 001h, 000h
loc_4_04BF:
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   esi
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_04D2:
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        and    eax, 3
        cmp    eax, 1
        je     SHORT loc_4_04E7
        cmp    eax, 2
        je     SHORT loc_4_04EE
        jmp    SHORT loc_4_04F3
loc_4_04E7:
        call   sub_4_00E2
        jmp    SHORT loc_4_04F3
loc_4_04EE:
        call   sub_4_0130
loc_4_04F3:
        push   0
        db  0BFh, 000h, 000h, 000h, 000h
        push   10000h
        push   0f0000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0509:
        add    esp, 0ch
        lea    esi, [eax + 0e076h]
        mov    ecx, 5
        repe cmpsb
        jne    SHORT loc_4_0550
        cmp    byte ptr [eax + 0e845h], 48h
        jne    SHORT loc_4_0550
        lea    esi, [eax + 0e842h]
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 2
        repe cmpsb
        jne    SHORT loc_4_0550
        mov    al, byte ptr [eax + 0e844h]
        cmp    al, 30h
        jb     SHORT loc_4_0550
        cmp    al, 36h
        ja     SHORT loc_4_0550
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
loc_4_0550:
        mov    ecx, 42h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_055A:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        xor    eax, eax
        jmp    NEAR PTR loc_4_02A6
loc_4_056B:
        mov    eax, 2
        jmp    NEAR PTR loc_4_02A6
sub_4_0575:
        push   esi
        push   0
        push   dword ptr [esp + 0ch]
        mov    esi, 1
        db  089h, 035h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_058C:
        add    esp, 8
        mov    eax, esi
        pop    esi
        ret    4
_ITEXT ends
        end
