; ibmtok_pcode_02.asm -- IBMTOK object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   edi
        db  0A1h, 0CCh, 056h, 000h, 000h
        push   eax
        push   0
        lea    eax, [ebp - 4]
        push   32h
        push   eax
        db  0E8h, 01Ch, 006h, 0FEh, 0FFh
        mov    ecx, dword ptr [ebp + 20h]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     SHORT loc_2_0030
        sub    al, al
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  090h
loc_2_0030:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
        stosw
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 18h]
        mov    dword ptr [eax + 2eh], 0ffffffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [edx]
        mov    byte ptr [ecx + 18h], al
        mov    al, byte ptr [edx + 1]
        mov    byte ptr [ecx + 19h], al
        add    ecx, 18h
        mov    al, byte ptr [edx + 2]
        mov    byte ptr [ecx + 2], al
        mov    al, byte ptr [edx + 3]
        mov    byte ptr [ecx + 3], al
        mov    al, byte ptr [edx + 4]
        mov    byte ptr [ecx + 4], al
        mov    al, byte ptr [edx + 5]
        mov    byte ptr [ecx + 5], al
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1eh], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 22h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 26h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ecx + 2ah], eax
        mov    al, 1
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_00D0:
        push   ebp
        mov    ebp, esp
        push   0
        push   32h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 05Ch, 005h, 0FEh, 0FFh
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_00F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 2eh], 0
        jne    SHORT loc_2_0109
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_0109:
        db  0A1h, 0CCh, 056h, 000h, 000h
        push   eax
        push   0
        push   1fh
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 01Ah, 005h, 0FEh, 0FFh
        or     eax, eax
        je     SHORT loc_2_0129
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_0129:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 2eh]
        mov    edi, edi
loc_2_0130:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    SHORT loc_2_0143
        inc    ecx
        cmp    ecx, 20h
        jb     SHORT loc_2_0130
        jmp    SHORT loc_2_0152
loc_2_0143:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 2eh], eax
loc_2_0152:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 14h]
        mov    dword ptr [eax + 10h], ecx
        mov    eax, dword ptr [esi + 14h]
        or     eax, eax
        je     SHORT loc_2_0168
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], ecx
loc_2_0168:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 14h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], 1
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [ebp - 8]
        mov    byte ptr [eax + 1ch], cl
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1dh], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1eh], 0
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    al, 1
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  090h
sub_2_01D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_04F0
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_2_01F9
        cmp    ebx, 103h
        jne    SHORT loc_2_0211
loc_2_01F9:
        push   0
        db  068h, 0C4h, 056h, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   edi
        push   esi
        call   sub_2_02C0
        or     eax, eax
        je     SHORT loc_2_0211
        mov    ebx, eax
loc_2_0211:
        or     ebx, ebx
        je     SHORT loc_2_021D
        cmp    ebx, 103h
        jne    SHORT loc_2_0248
loc_2_021D:
        cmp    byte ptr [edi + 1dh], 0
        je     SHORT loc_2_0248
        dec    dword ptr [esi + 0ch]
        mov    byte ptr [edi + 1dh], 0
        cmp    dword ptr [esi + 0ch], 0
        jne    SHORT loc_2_0248
        push   0
        db  068h, 0C4h, 056h, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   edi
        push   esi
        call   sub_2_0360
        or     eax, eax
        je     SHORT loc_2_0248
        mov    ebx, eax
loc_2_0248:
        or     ebx, ebx
        je     SHORT loc_2_0254
        cmp    ebx, 103h
        jne    SHORT loc_2_02B5
loc_2_0254:
        mov    eax, dword ptr [edi + 18h]
        dec    eax
        mov    dword ptr [edi + 18h], eax
        jne    SHORT loc_2_02B0
        mov    eax, dword ptr [edi + 10h]
        or     eax, eax
        je     SHORT loc_2_026A
        mov    ecx, dword ptr [edi + 14h]
        mov    dword ptr [eax + 14h], ecx
loc_2_026A:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_2_0279
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_2_027F
loc_2_0279:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 14h], eax
loc_2_027F:
        cmp    byte ptr [edi + 1eh], 0
        je     SHORT loc_2_028E
        mov    eax, dword ptr [edi + 4]
        push   eax
        db  0E8h, 00Ah, 003h, 0FEh, 0FFh
loc_2_028E:
        mov    eax, 1
        mov    cl, byte ptr [edi + 1ch]
        shl    eax, cl
        or     dword ptr [esi + 2eh], eax
        push   0
        push   1fh
        push   edi
        db  0E8h, 097h, 003h, 0FEh, 0FFh
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_2_02B0:
        mov    ebx, 0c001000eh
loc_2_02B5:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_2_02C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        xor    edx, edx
        mov    esi, dword ptr [edi + 4]
        mov    ecx, dword ptr [ebx + 0ch]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 14h]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 2]
        mov    dl, byte ptr [eax]
        shl    edx, 10h
        or     ecx, edx
        shl    ecx, 8
        xor    edx, edx
        mov    dl, byte ptr [eax + 1]
        shl    edx, 10h
        or     ecx, edx
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        or     ecx, eax
        mov    eax, ecx
        mov    dword ptr [ebx + 0ch], eax
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [edi + 4], 0
        or     eax, eax
        je     SHORT loc_2_0320
loc_2_0313:
        mov    ecx, dword ptr [eax + 0ch]
        or     dword ptr [edi + 4], ecx
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_2_0313
loc_2_0320:
        mov    ecx, dword ptr [edi + 4]
        cmp    esi, ecx
        je     SHORT loc_2_0354
        mov    al, byte ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebx]
        push   eax
        push   ecx
        push   esi
        call   dword ptr [edi + 1eh]
        or     eax, eax
        je     SHORT loc_2_0356
        cmp    eax, 103h
        je     SHORT loc_2_0356
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 0ch], ecx
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_0354:
        sub    eax, eax
loc_2_0356:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  090h
sub_2_0360:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        xor    eax, eax
        mov    ecx, dword ptr [esi + 0ch]
        mov    dword ptr [ebp - 8], ecx
        mov    ecx, dword ptr [ebp + 14h]
        xor    edx, edx
        mov    dl, byte ptr [ecx]
        mov    al, byte ptr [ecx + 2]
        shl    edx, 10h
        or     edx, eax
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [ecx + 1]
        shl    eax, 10h
        or     edx, eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 3]
        or     edx, eax
        mov    dword ptr [ebp - 4], edx
        db  0A0h, 0C4h, 056h, 000h, 000h
        cmp    byte ptr [ecx], al
        jne    SHORT loc_2_0408
        db  0A0h, 0C5h, 056h, 000h, 000h
        cmp    byte ptr [ecx + 1], al
        jne    SHORT loc_2_0408
        db  0A0h, 0C6h, 056h, 000h, 000h
        cmp    byte ptr [ecx + 2], al
        jne    SHORT loc_2_0408
        db  0A0h, 0C7h, 056h, 000h, 000h
        cmp    byte ptr [ecx + 3], al
        jne    SHORT loc_2_0408
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    byte ptr [ebx + 1dh], 0
        je     SHORT loc_2_03E9
        dec    dword ptr [esi + 0ch]
        mov    byte ptr [ebx + 1dh], 0
        cmp    dword ptr [esi + 0ch], 0
        je     NEAR PTR loc_2_0483
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_03E9:
        cmp    dword ptr [esi + 0ch], 0
        je     SHORT loc_2_03FD
        mov    eax, 0c001001ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_03FD:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_0408:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 8], eax
        jne    SHORT loc_2_0440
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    byte ptr [ebx + 1dh], 0
        je     SHORT loc_2_0424
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_0424:
        cmp    dword ptr [esi + 0ch], 0
        je     SHORT loc_2_0483
        mov    byte ptr [ebx + 1dh], 1
        inc    dword ptr [esi + 0ch]
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h
loc_2_0440:
        mov    eax, dword ptr [esi + 0ch]
        cmp    eax, 1
        jbe    SHORT loc_2_0456
        mov    eax, 0c001001ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_0456:
        mov    ebx, dword ptr [ebp + 0ch]
        jne    SHORT loc_2_0470
        cmp    byte ptr [ebx + 1dh], 0
        jne    SHORT loc_2_0470
        mov    eax, 0c001001ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  090h
loc_2_0470:
        cmp    eax, 1
        jne    SHORT loc_2_0483
        cmp    byte ptr [ebx + 1dh], 0
        je     SHORT loc_2_0483
        dec    eax
        mov    dword ptr [esi + 0ch], eax
        mov    byte ptr [ebx + 1dh], 0
loc_2_0483:
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 8], ecx
        mov    al, byte ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebx]
        push   eax
        push   ecx
        push   edi
        call   dword ptr [esi + 22h]
        or     eax, eax
        je     SHORT loc_2_04B6
        cmp    eax, 103h
        je     SHORT loc_2_04B6
        mov    dword ptr [esi + 8], edi
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0ch], ecx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_04B6:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_2_04D0
        mov    byte ptr [ebx + 1dh], 0
        mov    dword ptr [esi + 0ch], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_04D0:
        mov    byte ptr [ebx + 1dh], 1
        mov    dword ptr [esi + 0ch], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_04F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [esi + 10h]
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 14h]
        or     eax, eax
        mov    dword ptr [esi + 10h], 0
        je     SHORT loc_2_052D
        lea    esp, [esp]
loc_2_0520:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 10h], ecx
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_2_0520
loc_2_052D:
        mov    ecx, dword ptr [esi + 10h]
        cmp    ebx, ecx
        je     SHORT loc_2_0561
        mov    al, byte ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        push   ecx
        push   ebx
        call   dword ptr [esi + 26h]
        or     eax, eax
        je     SHORT loc_2_0563
        cmp    eax, 103h
        je     SHORT loc_2_0563
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 10h], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_0561:
        sub    eax, eax
loc_2_0563:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h
_PTEXT ends
        end
