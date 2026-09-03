; ndis_pcode_04_01.asm -- NDIS object 4 segment _PTEXT  (part 1/3)
.386p
_PTEXT segment dword public use32 'PCODE'

loc_4_0000:
        sub    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ecx, [esp]
        push   edi
        push   eax
        push   0
        push   34h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0018:
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 24h]
        mov    dword ptr [esp + 8], eax
        test   eax, eax
        mov    dword ptr [edx], ecx
        je     SHORT loc_4_0033
        xor    al, al
        pop    edi
        add    esp, 8
        ret    18h
loc_4_0033:
        mov    edi, dword ptr [esp + 4]
        xor    eax, eax
        mov    ecx, 0dh
        rep stosd
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 10h]
        mov    dl, byte ptr [esp + 1ch]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx + 18h], 0ffffffffh
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx + 0ch], 0
        mov    eax, dword ptr [esp + 4]
        mov    byte ptr [eax + 2ch], dl
        mov    eax, dword ptr [esp + 20h]
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 14h]
        push   40h
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [ecx + 10h], edx
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx + 14h], eax
        mov    eax, dword ptr [esp + 8]
        add    eax, 1ch
        lea    ecx, [esp + 0ch]
        push   eax
        push   ecx
        call   sub_4_35F0
        cmp    dword ptr [esp + 8], 0
        je     SHORT loc_4_00BE
        mov    eax, dword ptr [esp + 4]
        push   0
        push   34h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_00B5:
        xor    al, al
        pop    edi
        add    esp, 8
        ret    18h
loc_4_00BE:
        mov    al, 1
        pop    edi
        add    esp, 8
        ret    18h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_00D0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        call   sub_4_3660
        cmp    dword ptr [esi + 28h], 0
        je     SHORT loc_4_00FB
loc_4_00E5:
        mov    eax, dword ptr [esi + 28h]
        push   eax
        push   esi
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 28h], ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_00F5:
        cmp    dword ptr [esi + 28h], 0
        jne    SHORT loc_4_00E5
loc_4_00FB:
        cmp    dword ptr [esi + 24h], 0
        je     SHORT loc_4_011A
loc_4_0101:
        mov    eax, dword ptr [esi + 24h]
        push   0
        push   5ch
        push   eax
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 24h], ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0114:
        cmp    dword ptr [esi + 24h], 0
        jne    SHORT loc_4_0101
loc_4_011A:
        cmp    dword ptr [esi + 20h], 0
        je     SHORT loc_4_0148
loc_4_0120:
        mov    edi, dword ptr [esi + 20h]
        push   0
        push   400h
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [esi + 20h], eax
        mov    ecx, dword ptr [edi]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0138:
        push   0
        push   10h
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0142:
        cmp    dword ptr [esi + 20h], 0
        jne    SHORT loc_4_0120
loc_4_0148:
        push   0
        push   34h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0152:
        pop    edi
        pop    esi
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0160:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        cmp    dword ptr [esi + 18h], 0
        jne    SHORT loc_4_0179
        xor    al, al
        pop    edi
        pop    esi
        add    esp, 4
        ret    10h
loc_4_0179:
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ecx, [esp + 8]
        push   eax
        push   0
        push   1ch
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_018D:
        test   eax, eax
        je     SHORT loc_4_019B
        xor    al, al
        pop    edi
        pop    esi
        add    esp, 4
        ret    10h
loc_4_019B:
        xor    ecx, ecx
        mov    edi, dword ptr [esi + 18h]
loc_4_01A0:
        mov    eax, 1
        shl    eax, cl
        test   edi, eax
        jne    SHORT loc_4_01B7
        inc    ecx
        cmp    ecx, 20h
        jb     SHORT loc_4_01A0
        mov    edx, dword ptr [esp + 8]
        jmp    SHORT loc_4_01C7
loc_4_01B7:
        mov    edx, ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    eax, edi
        mov    dword ptr [esi + 18h], eax
loc_4_01C7:
        mov    eax, dword ptr [esi + 0ch]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx + 10h], eax
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esi + 0ch], ecx
        mov    byte ptr [ecx + 15h], dl
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx + 0ch], 1
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 18h]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx + 4], edx
        xor    ecx, ecx
        mov    eax, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 1ch]
        mov    dword ptr [eax + 8], ecx
        mov    eax, dword ptr [esp + 8]
        mov    byte ptr [eax + 14h], cl
        mov    ecx, dword ptr [esp + 8]
        mov    al, 1
        pop    edi
        pop    esi
        mov    dword ptr [edx], ecx
        add    esp, 4
        ret    10h
        db  090h
sub_4_0220:
        mov    eax, dword ptr [esp + 0ch]
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 14h]
        push   0
        mov    esi, dword ptr [esp + 14h]
        push   0
        push   eax
        push   edi
        push   esi
        call   sub_4_02B0
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_4_0249
        cmp    ebx, 103h
        jne    SHORT loc_4_02A2
loc_4_0249:
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_4_029D
        lea    ecx, [esi + 0ch]
        cmp    dword ptr [ecx], 0
        je     SHORT loc_4_026F
loc_4_025A:
        mov    eax, dword ptr [ecx]
        cmp    eax, edi
        je     SHORT loc_4_026A
        lea    ecx, [eax + 10h]
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_4_025A
        jmp    SHORT loc_4_026F
loc_4_026A:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [ecx], eax
loc_4_026F:
        cmp    byte ptr [edi + 14h], 0
        je     SHORT loc_4_027E
        mov    eax, dword ptr [edi + 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_027E:
        mov    eax, 1
        mov    cl, byte ptr [edi + 15h]
        shl    eax, cl
        push   0
        or     dword ptr [esi + 18h], eax
        push   1ch
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0295:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
loc_4_029D:
        mov    ebx, 0c001000eh
loc_4_02A2:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_02B0:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        mov    edi, dword ptr [esp + 14h]
        push   ebp
        mov    ebx, dword ptr [esi + 8]
        mov    eax, dword ptr [esp + 20h]
        mov    ebp, dword ptr [edi + 8]
        mov    dword ptr [edi + 18h], ebp
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 0ch]
        mov    ecx, dword ptr [esi + 8]
        test   eax, eax
        mov    dword ptr [esi + 8], 0
        mov    dword ptr [esi + 30h], ecx
        je     SHORT loc_4_02ED
loc_4_02E0:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 8], ecx
        mov    eax, dword ptr [eax + 10h]
        test   eax, eax
        jne    SHORT loc_4_02E0
loc_4_02ED:
        mov    ecx, dword ptr [esi + 8]
        cmp    ebx, ecx
        je     SHORT loc_4_031E
        mov    eax, dword ptr [esp + 24h]
        mov    edx, dword ptr [esp + 1ch]
        push   eax
        push   edx
        mov    eax, dword ptr [edi]
        push   eax
        push   ecx
        push   ebx
        call   dword ptr [esi + 10h]
        test   eax, eax
        je     SHORT loc_4_0320
        cmp    eax, 103h
        je     SHORT loc_4_0320
        mov    dword ptr [edi + 8], ebp
        pop    ebp
        pop    edi
        mov    dword ptr [esi + 8], ebx
        pop    esi
        pop    ebx
        ret    14h
loc_4_031E:
        xor    eax, eax
loc_4_0320:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    14h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0330:
        sub    esp, 8
        xor    eax, eax
        mov    dword ptr [esp], 1010103h
        mov    dword ptr [esp + 4], 1010104h
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 18h]
        push   edi
        push   ebp
        xor    edi, edi
        mov    ebx, dword ptr [esp + 1ch]
        cmp    dword ptr [esi], edi
        jbe    SHORT loc_4_03A1
        mov    edx, ebx
loc_4_0359:
        mov    ecx, dword ptr [edx]
        cmp    ecx, 6010101h
        je     SHORT loc_4_0388
        cmp    ecx, 6010102h
        je     SHORT loc_4_0391
        cmp    ecx, 6020201h
        je     SHORT loc_4_0399
        mov    ebp, ecx
        and    ebp, 0fff00000h
        cmp    ebp, 6000000h
        je     SHORT loc_4_0399
        mov    dword ptr [ebx + edi*4], ecx
        jmp    SHORT loc_4_0398
loc_4_0388:
        mov    dword ptr [ebx + edi*4], 1010101h
        jmp    SHORT loc_4_0398
loc_4_0391:
        mov    dword ptr [ebx + edi*4], 1010102h
loc_4_0398:
        inc    edi
loc_4_0399:
        add    edx, 4
        inc    eax
        cmp    dword ptr [esi], eax
        ja     SHORT loc_4_0359
loc_4_03A1:
        lea    ecx, [ebx + edi*4]
        push   8
        lea    eax, [esp + 14h]
        add    edi, 2
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_03B4:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [esi], edi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_03D0:
        mov    edx, dword ptr [esp + 4]
        push   esi
        mov    ecx, dword ptr [esp + 0ch]
        lea    esi, [edx + 10h]
        cmp    dword ptr [esi], 0
        je     SHORT loc_4_03F6
loc_4_03E1:
        mov    eax, dword ptr [esi]
        cmp    ecx, eax
        je     SHORT loc_4_03F1
        lea    esi, [eax + 14h]
        cmp    dword ptr [esi], 0
        jne    SHORT loc_4_03E1
        jmp    SHORT loc_4_03F6
loc_4_03F1:
        mov    eax, dword ptr [ecx + 14h]
        mov    dword ptr [esi], eax
loc_4_03F6:
        lea    esi, [edx + 44h]
        cmp    dword ptr [esi], 0
        je     SHORT loc_4_0413
loc_4_03FE:
        mov    eax, dword ptr [esi]
        cmp    ecx, eax
        je     SHORT loc_4_040E
        lea    esi, [eax + 1ch]
        cmp    dword ptr [esi], 0
        jne    SHORT loc_4_03FE
        jmp    SHORT loc_4_0413
loc_4_040E:
        mov    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [esi], eax
loc_4_0413:
        add    edx, 48h
        cmp    dword ptr [edx], 0
        je     SHORT loc_4_0430
loc_4_041B:
        mov    eax, dword ptr [edx]
        cmp    ecx, eax
        je     SHORT loc_4_042B
        lea    edx, [eax + 20h]
        cmp    dword ptr [edx], 0
        jne    SHORT loc_4_041B
        jmp    SHORT loc_4_0430
loc_4_042B:
        mov    eax, dword ptr [ecx + 20h]
        mov    dword ptr [edx], eax
loc_4_0430:
        xor    eax, eax
        pop    esi
        mov    dword ptr [ecx + 1ch], eax
        mov    dword ptr [ecx + 20h], eax
        mov    dword ptr [ecx + 14h], eax
        ret    8
        db  090h
sub_4_0440:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   esi
        call   sub_4_03D0
        cmp    byte ptr [edi + 10h], 0
        je     SHORT loc_4_0460
        mov    eax, dword ptr [edi + 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0460:
        cmp    byte ptr [esp + 14h], 0
        je     SHORT loc_4_046D
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 1ch]
loc_4_046D:
        mov    eax, 1
        mov    cl, byte ptr [edi + 11h]
        shl    eax, cl
        push   0
        or     dword ptr [esi + 28h], eax
        push   24h
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0484:
        pop    edi
        pop    esi
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0490:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        lea    eax, [esp + 0ch]
        push   ebx
        push   0
        push   4ch
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_04AB:
        test   eax, eax
        je     SHORT loc_4_04BA
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    1ch
loc_4_04BA:
        mov    edi, dword ptr [esp + 0ch]
        xor    eax, eax
        mov    ecx, 13h
        rep stosd
        mov    esi, dword ptr [esp + 14h]
        cmp    esi, eax
        jne    SHORT loc_4_04D4
        mov    esi, 2
loc_4_04D4:
        lea    eax, [esi + esi*2]
        push   ebx
        push   0
        lea    edi, [eax*2]
        mov    eax, dword ptr [esp + 14h]
        push   edi
        add    eax, 4
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_04EF:
        test   eax, eax
        je     SHORT loc_4_0508
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        call   sub_4_0600
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    1ch
loc_4_0508:
        mov    eax, dword ptr [esp + 0ch]
        push   ebx
        push   0
        add    eax, 34h
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0519:
        test   eax, eax
        je     SHORT loc_4_0532
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        call   sub_4_0600
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    1ch
loc_4_0532:
        lea    edi, [esi*4]
        push   ebx
        mov    eax, dword ptr [esp + 10h]
        push   0
        push   edi
        add    eax, 8
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_054A:
        test   eax, eax
        je     SHORT loc_4_0563
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        call   sub_4_0600
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    1ch
loc_4_0563:
        mov    eax, dword ptr [esp + 0ch]
        push   ebx
        push   0
        add    eax, 38h
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0574:
        test   eax, eax
        mov    eax, dword ptr [esp + 0ch]
        je     SHORT loc_4_058D
        push   eax
        call   sub_4_0600
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    1ch
loc_4_058D:
        mov    dword ptr [eax + 28h], 0ffffffffh
        mov    eax, dword ptr [esp + 24h]
        mov    edx, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx + 2ch], ecx
        mov    edx, dword ptr [esp + 0ch]
        mov    ax, word ptr [eax + 4]
        mov    ecx, dword ptr [esp + 18h]
        mov    word ptr [edx + 30h], ax
        mov    edx, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [edx], eax
        mov    edx, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [edx + 14h], ecx
        mov    edx, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [edx + 18h], eax
        mov    edx, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [edx + 1ch], ecx
        mov    edx, dword ptr [esp + 0ch]
        mov    dword ptr [edx + 20h], esi
        mov    edx, dword ptr [esp + 0ch]
        pop    edi
        mov    dword ptr [eax], edx
        mov    al, 1
        pop    esi
        pop    ebx
        add    esp, 4
        ret    1ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0600:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 4]
        test   eax, eax
        je     SHORT loc_4_061D
        push   0
        mov    ecx, dword ptr [esi + 20h]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_061D:
        mov    eax, dword ptr [esi + 34h]
        test   eax, eax
        je     SHORT loc_4_0635
        push   0
        mov    ecx, dword ptr [esi + 20h]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0635:
        mov    eax, dword ptr [esi + 8]
        test   eax, eax
        je     SHORT loc_4_064B
        push   0
        mov    ecx, dword ptr [esi + 20h]
        shl    ecx, 2
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_064B:
        mov    eax, dword ptr [esi + 38h]
        test   eax, eax
        je     SHORT loc_4_0661
        push   0
        mov    ecx, dword ptr [esi + 20h]
        shl    ecx, 2
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0661:
        push   0
        push   4ch
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_066B:
        pop    esi
        ret    4
        db  090h
sub_4_0670:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        cmp    dword ptr [esi + 28h], 0
        jne    SHORT loc_4_0689
        xor    al, al
        pop    edi
        pop    esi
        add    esp, 4
        ret    10h
loc_4_0689:
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ecx, [esp + 8]
        push   eax
        push   0
        push   24h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_069D:
        test   eax, eax
        je     SHORT loc_4_06AB
        xor    al, al
        pop    edi
        pop    esi
        add    esp, 4
        ret    10h
loc_4_06AB:
        mov    edi, dword ptr [esp + 8]
        xor    eax, eax
        mov    ecx, 9
        rep stosd
        xor    ecx, ecx
        mov    edi, dword ptr [esi + 28h]
loc_4_06BD:
        mov    eax, 1
        shl    eax, cl
        test   edi, eax
        jne    SHORT loc_4_06D4
        inc    ecx
        cmp    ecx, 20h
        jb     SHORT loc_4_06BD
        mov    edx, dword ptr [esp + 8]
        jmp    SHORT loc_4_06E4
loc_4_06D4:
        mov    edx, ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    eax, edi
        mov    dword ptr [esi + 28h], eax
loc_4_06E4:
        mov    eax, dword ptr [esi + 10h]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx + 14h], eax
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esi + 10h], ecx
        mov    dword ptr [ecx + 0ch], 1
        mov    ecx, dword ptr [esp + 8]
        mov    byte ptr [ecx + 11h], dl
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 18h]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [ecx + 4], edx
        mov    ecx, dword ptr [esp + 8]
        pop    edi
        mov    dword ptr [eax], ecx
        mov    al, 1
        pop    esi
        add    esp, 4
        ret    10h
        db  08Dh, 064h, 024h, 000h
sub_4_0730:
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 10h]
        push   edi
        mov    edi, dword ptr [esp + 10h]
        push   ebp
        mov    ebp, dword ptr [esp + 1ch]
        push   0
        push   0
        push   ebp
        push   ebx
        push   edi
        call   sub_4_0C30
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_4_075A
        cmp    esi, 103h
        jne    SHORT loc_4_076E
loc_4_075A:
        push   0
        push   0
        push   0
        push   ebp
        push   ebx
        push   edi
        call   sub_4_07F0
        test   eax, eax
        je     SHORT loc_4_076E
        mov    esi, eax
loc_4_076E:
        test   esi, esi
        je     SHORT loc_4_077A
        cmp    esi, 103h
        jne    SHORT loc_4_079A
loc_4_077A:
        mov    eax, dword ptr [ebx + 0ch]
        dec    eax
        mov    dword ptr [ebx + 0ch], eax
        jne    SHORT loc_4_0795
        push   0
        push   ebx
        push   edi
        call   sub_4_0440
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
loc_4_0795:
        mov    esi, 0c001000eh
loc_4_079A:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_07B0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 3ch]
        mov    ecx, dword ptr [esi + 34h]
        mov    edx, dword ptr [esi + 4]
        mov    dword ptr [esi + 24h], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_07CE:
        mov    ecx, dword ptr [esi + 24h]
        mov    edx, dword ptr [esi + 8]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [esi + 38h]
        add    eax, eax
        push   eax
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_07E4:
        pop    esi
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_07F0:
        sub    esp, 8
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 14h]
        push   edi
        push   ebp
        mov    eax, dword ptr [esi + 24h]
        shl    eax, 2
        mov    ecx, dword ptr [esi + 8]
        push   eax
        mov    edx, dword ptr [esi + 38h]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_080F:
        mov    ecx, dword ptr [esi + 24h]
        mov    edx, dword ptr [esi + 34h]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [esi + 4]
        add    eax, eax
        xor    ebx, ebx
        push   eax
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0827:
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [esi + 3ch], eax
        test   eax, eax
        je     SHORT loc_4_0855
        xor    eax, eax
        mov    edi, dword ptr [esp + 20h]
loc_4_0837:
        mov    edx, dword ptr [esi + 8]
        mov    ebp, 1
        mov    cl, byte ptr [edi + 11h]
        add    eax, 4
        shl    ebp, cl
        not    ebp
        inc    ebx
        and    dword ptr [edx + eax - 4], ebp
        cmp    dword ptr [esi + 24h], ebx
        ja     SHORT loc_4_0837
        jmp    SHORT loc_4_0859
loc_4_0855:
        mov    edi, dword ptr [esp + 20h]
loc_4_0859:
        mov    dword ptr [esp + 10h], 0
        cmp    dword ptr [esi + 24h], 0
        jbe    SHORT loc_4_08CB
loc_4_0867:
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esi + 8]
        cmp    dword ptr [ecx + eax*4], 0
        jne    SHORT loc_4_08BE
        lea    ecx, [eax + eax*2]
        mov    eax, dword ptr [esi + 24h]
        add    ecx, ecx
        sub    eax, dword ptr [esp + 10h]
        add    ecx, dword ptr [esi + 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        lea    edx, [ecx + 6]
        sub    eax, 6
        push   eax
        push   edx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0896:
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esi + 24h]
        shl    ecx, 2
        sub    eax, dword ptr [esp + 10h]
        add    ecx, dword ptr [esi + 8]
        lea    edx, [eax*4 - 4]
        push   edx
        lea    ebx, [ecx + 4]
        push   ebx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_08B9:
        dec    dword ptr [esi + 24h]
        jmp    SHORT loc_4_08C2
loc_4_08BE:
        inc    dword ptr [esp + 10h]
loc_4_08C2:
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [esi + 24h], eax
        ja     SHORT loc_4_0867
loc_4_08CB:
        mov    dword ptr [esp + 14h], 0
        cmp    dword ptr [esp + 28h], 0
        je     NEAR PTR loc_4_09BC
        mov    ebx, dword ptr [esp + 2ch]
loc_4_08E2:
        lea    eax, [esp + 10h]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        mov    edx, dword ptr [esi + 24h]
        push   ebx
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_08F5:
        test   al, al
        je     SHORT loc_4_0913
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, dword ptr [esi + 8]
        mov    edx, 1
        mov    cl, byte ptr [edi + 11h]
        shl    edx, cl
        or     dword ptr [ebp + eax*4], edx
        jmp    NEAR PTR loc_4_09A7
loc_4_0913:
        mov    ecx, dword ptr [esi + 24h]
        cmp    dword ptr [esi + 20h], ecx
        jbe    NEAR PTR loc_4_09F6
        mov    eax, dword ptr [esp + 10h]
        sub    ecx, dword ptr [esp + 10h]
        lea    eax, [eax + eax*2]
        lea    ecx, [ecx + ecx*2]
        add    eax, eax
        add    ecx, ecx
        add    eax, dword ptr [esi + 4]
        push   ecx
        push   eax
        add    eax, 6
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_093F:
        mov    ebp, dword ptr [ebx]
        mov    ecx, dword ptr [esp + 10h]
        mov    edx, dword ptr [esi + 4]
        lea    eax, [ecx + ecx*2]
        mov    dword ptr [edx + eax*2], ebp
        mov    edx, dword ptr [esi + 4]
        mov    bp, word ptr [ebx + 4]
        mov    ecx, dword ptr [esp + 10h]
        lea    eax, [ecx + ecx*2]
        mov    word ptr [edx + eax*2 + 4], bp
        mov    eax, dword ptr [esp + 10h]
        shl    eax, 2
        mov    ecx, dword ptr [esi + 24h]
        add    eax, dword ptr [esi + 8]
        sub    ecx, dword ptr [esp + 10h]
        shl    ecx, 2
        push   ecx
        push   eax
        add    eax, 4
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0980:
        mov    edx, dword ptr [esp + 10h]
        mov    ebp, dword ptr [esi + 8]
        mov    dword ptr [ebp + edx*4], 0
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, dword ptr [esi + 8]
        mov    edx, 1
        mov    cl, byte ptr [edi + 11h]
        shl    edx, cl
        or     dword ptr [ebp + eax*4], edx
        inc    dword ptr [esi + 24h]
loc_4_09A7:
        add    ebx, 6
        inc    dword ptr [esp + 14h]
        mov    eax, dword ptr [esp + 14h]
        cmp    eax, dword ptr [esp + 28h]
        jb     NEAR PTR loc_4_08E2
loc_4_09BC:
        mov    ecx, dword ptr [esi + 3ch]
        mov    eax, dword ptr [esi + 24h]
        sub    eax, ecx
        xor    edx, edx
        test   ecx, ecx
        je     SHORT loc_4_0A15
loc_4_09CA:
        test   eax, eax
        jne    SHORT loc_4_0A19
        lea    eax, [edx + edx*2]
        mov    ebx, dword ptr [esi + 4]
        lea    ebp, [eax*2]
        mov    eax, dword ptr [esi + 34h]
        add    ebx, ebp
        add    eax, ebp
        mov    ebp, dword ptr [eax + 2]
        cmp    dword ptr [ebx + 2], ebp
        jne    SHORT loc_4_0A0B
        mov    ax, word ptr [eax]
        cmp    word ptr [ebx], ax
        jne    SHORT loc_4_0A0B
        xor    eax, eax
        jmp    SHORT loc_4_0A10
loc_4_09F6:
        push   esi
        call   sub_4_07B0
        mov    eax, 0c0010009h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    18h
loc_4_0A0B:
        mov    eax, 1
loc_4_0A10:
        inc    edx
        cmp    edx, ecx
        jb     SHORT loc_4_09CA
loc_4_0A15:
        test   eax, eax
        je     SHORT loc_4_0A4C
loc_4_0A19:
        mov    eax, dword ptr [esp + 30h]
        mov    edx, dword ptr [esp + 24h]
        push   eax
        mov    ebx, dword ptr [edi]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   ebx
        mov    edx, dword ptr [esi + 24h]
        push   eax
        mov    ebx, dword ptr [esi + 34h]
        push   edx
        push   ebx
        push   ecx
        call   dword ptr [esi + 14h]
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_4_0A4E
        cmp    edi, 103h
        je     SHORT loc_4_0A4E
        push   esi
        call   sub_4_07B0
        jmp    SHORT loc_4_0A4E
loc_4_0A4C:
        xor    edi, edi
loc_4_0A4E:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    18h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_0A60:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        cmp    byte ptr [esp + 10h], 0
        push   esi
        je     SHORT loc_4_0A98
        mov    eax, dword ptr [edx + 44h]
        xor    bl, bl
        mov    ecx, eax
        mov    esi, dword ptr [esp + 10h]
        test   ecx, ecx
        je     SHORT loc_4_0A89
loc_4_0A7C:
        cmp    esi, ecx
        jne    SHORT loc_4_0A82
        mov    bl, 1
loc_4_0A82:
        mov    ecx, dword ptr [ecx + 1ch]
        test   ecx, ecx
        jne    SHORT loc_4_0A7C
loc_4_0A89:
        test   bl, bl
        jne    SHORT loc_4_0AC0
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [edx + 44h], esi
        pop    esi
        pop    ebx
        ret    0ch
loc_4_0A98:
        add    edx, 44h
        mov    esi, dword ptr [esp + 10h]
        cmp    dword ptr [edx], 0
        je     SHORT loc_4_0AB9
loc_4_0AA4:
        mov    eax, dword ptr [edx]
        cmp    esi, eax
        je     SHORT loc_4_0AB4
        lea    edx, [eax + 1ch]
        cmp    dword ptr [edx], 0
        jne    SHORT loc_4_0AA4
        jmp    SHORT loc_4_0AB9
loc_4_0AB4:
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [edx], eax
loc_4_0AB9:
        mov    dword ptr [esi + 1ch], 0
loc_4_0AC0:
        pop    esi
        pop    ebx
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0AD0:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        cmp    byte ptr [esp + 10h], 0
        push   esi
        je     SHORT loc_4_0B08
        mov    ecx, dword ptr [edx + 48h]
        xor    bl, bl
        mov    eax, ecx
        mov    esi, dword ptr [esp + 10h]
        test   eax, eax
        je     SHORT loc_4_0AF9
loc_4_0AEC:
        cmp    eax, esi
        jne    SHORT loc_4_0AF2
        mov    bl, 1
loc_4_0AF2:
        mov    eax, dword ptr [eax + 20h]
        test   eax, eax
        jne    SHORT loc_4_0AEC
loc_4_0AF9:
        test   bl, bl
        jne    SHORT loc_4_0B30
        mov    dword ptr [esi + 20h], ecx
        mov    dword ptr [edx + 48h], esi
        pop    esi
        pop    ebx
        ret    0ch
loc_4_0B08:
        add    edx, 48h
        mov    esi, dword ptr [esp + 10h]
        cmp    dword ptr [edx], 0
        je     SHORT loc_4_0B29
loc_4_0B14:
        mov    eax, dword ptr [edx]
        cmp    eax, esi
        je     SHORT loc_4_0B24
        lea    edx, [eax + 20h]
        cmp    dword ptr [edx], 0
        jne    SHORT loc_4_0B14
        jmp    SHORT loc_4_0B29
loc_4_0B24:
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [edx], eax
loc_4_0B29:
        mov    dword ptr [esi + 20h], 0
loc_4_0B30:
        pop    esi
        pop    ebx
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0B40:
        sub    esp, 4
        xor    eax, eax
        xor    cl, cl
        push   ebx
        push   esi
        xor    bl, bl
        mov    esi, dword ptr [esp + 14h]
        push   edi
        mov    byte ptr [esp + 0fh], al
        mov    edx, dword ptr [esi + 18h]
        test   dl, 0a0h
        je     SHORT loc_4_0B67
        mov    eax, 1
        mov    byte ptr [esp + 0fh], al
        jmp    SHORT loc_4_0B78
loc_4_0B67:
        test   dl, 1
        je     SHORT loc_4_0B6E
        mov    al, 1
loc_4_0B6E:
        test   dl, 0eh
        je     SHORT loc_4_0B78
        mov    byte ptr [esp + 0fh], 1
loc_4_0B78:
        mov    edx, dword ptr [esi + 8]
        test   dl, 0a0h
        je     SHORT loc_4_0B89
        mov    ebx, 1
        mov    cl, bl
        jmp    SHORT loc_4_0B97
loc_4_0B89:
        test   dl, 1
        je     SHORT loc_4_0B90
        mov    cl, 1
loc_4_0B90:
        test   dl, 0eh
        je     SHORT loc_4_0B97
        mov    bl, 1
loc_4_0B97:
        test   al, al
        jne    SHORT loc_4_0BB2
        test   cl, cl
        je     SHORT loc_4_0BAE
        mov    edi, dword ptr [esp + 14h]
        push   1
        push   esi
        push   edi
        call   sub_4_0A60
        jmp    SHORT loc_4_0BC9
loc_4_0BAE:
        test   al, al
        je     SHORT loc_4_0BC5
loc_4_0BB2:
        test   cl, cl
        jne    SHORT loc_4_0BC5
        mov    edi, dword ptr [esp + 14h]
        push   0
        push   esi
        push   edi
        call   sub_4_0A60
        jmp    SHORT loc_4_0BC9
loc_4_0BC5:
        mov    edi, dword ptr [esp + 14h]
loc_4_0BC9:
        cmp    byte ptr [esp + 0fh], 0
        jne    SHORT loc_4_0BED
        test   bl, bl
        je     SHORT loc_4_0BE6
        push   1
        push   esi
        push   edi
        call   sub_4_0AD0
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
loc_4_0BE6:
        cmp    byte ptr [esp + 0fh], 0
        je     SHORT loc_4_0BFA
loc_4_0BED:
        test   bl, bl
        jne    SHORT loc_4_0BFA
        push   0
        push   esi
        push   edi
        call   sub_4_0AD0
loc_4_0BFA:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0C10:
        mov    edx, dword ptr [esp + 8]
        push   edx
        mov    eax, dword ptr [edx + 18h]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [esp + 8]
        push   eax
        mov    ecx, dword ptr [eax + 40h]
        mov    dword ptr [eax + 0ch], ecx
        call   sub_4_0B40
        ret    8
        db  08Bh, 0FFh
sub_4_0C30:
        mov    ecx, dword ptr [esp + 10h]
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [esi + 18h], eax
        mov    dword ptr [esi + 8], ecx
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [edi + 0ch], 0
        mov    dword ptr [edi + 40h], eax
        mov    eax, dword ptr [edi + 10h]
        test   eax, eax
        je     SHORT loc_4_0C69
loc_4_0C5C:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [edi + 0ch], ecx
        mov    eax, dword ptr [eax + 14h]
        test   eax, eax
        jne    SHORT loc_4_0C5C
loc_4_0C69:
        push   esi
        push   edi
        call   sub_4_0B40
        mov    ecx, dword ptr [edi + 0ch]
        mov    edx, dword ptr [edi + 40h]
        mov    eax, edx
        xor    eax, ecx
        test   eax, 0ffffff7fh
        je     SHORT loc_4_0CBC
        mov    eax, dword ptr [esp + 20h]
        mov    ebx, dword ptr [esp + 18h]
        push   eax
        and    ecx, 0ffffff7fh
        push   ebx
        mov    eax, dword ptr [esi]
        push   eax
        and    edx, 0ffffff7fh
        push   ecx
        push   edx
        call   dword ptr [edi + 18h]
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_4_0CBE
        cmp    ebx, 103h
        je     SHORT loc_4_0CBE
        push   esi
        push   edi
        call   sub_4_0C10
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    14h
loc_4_0CBC:
        xor    ebx, ebx
loc_4_0CBE:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    14h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0CD0:
        push   esi
        xor    ecx, ecx
        mov    eax, dword ptr [esp + 0ch]
        push   edi
        mov    edx, dword ptr [esp + 0ch]
        mov    cl, byte ptr [eax + 11h]
        xor    eax, eax
        mov    edi, dword ptr [edx + 24h]
        cmp    edi, eax
        je     SHORT loc_4_0CFD
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_4_0CF2:
        test   dword ptr [esi], edx
        je     SHORT loc_4_0CF7
        inc    eax
loc_4_0CF7:
        add    esi, 4
        dec    edi
        jne    SHORT loc_4_0CF2
loc_4_0CFD:
        pop    edi
        pop    esi
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0D10:
        mov    eax, dword ptr [esp + 0ch]
        sub    esp, 0ch
        xor    ecx, ecx
        push   ebx
        push   esi
        mov    cl, byte ptr [eax + 11h]
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [esp + 20h]
        push   ebp
        mov    dword ptr [esp + 14h], ebx
        cmp    dword ptr [esi + 24h], ebx
        jbe    SHORT loc_4_0D81
        mov    ebp, dword ptr [esp + 34h]
        xor    edi, edi
        mov    eax, 1
        mov    dword ptr [esp + 10h], ebx
        shl    eax, cl
        mov    dword ptr [esp + 18h], eax
loc_4_0D44:
        mov    ecx, dword ptr [esi + 8]
        mov    eax, dword ptr [esp + 18h]
        test   dword ptr [ecx + edi], eax
        je     SHORT loc_4_0D73
        cmp    dword ptr [esp + 2ch], 6
        jb     SHORT loc_4_0D9F
        push   6
        mov    eax, dword ptr [esi + 4]
        sub    dword ptr [esp + 30h], 6
        add    eax, dword ptr [esp + 14h]
        push   eax
        push   ebp
        add    ebp, 6
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0D6F:
        inc    dword ptr [esp + 14h]
loc_4_0D73:
        add    edi, 4
        inc    ebx
        add    dword ptr [esp + 10h], 6
        cmp    dword ptr [esi + 24h], ebx
        ja     SHORT loc_4_0D44
loc_4_0D81:
        mov    eax, dword ptr [esp + 20h]
        mov    edx, dword ptr [esp + 30h]
        mov    ecx, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        mov    dword ptr [eax], 0
        pop    esi
        mov    dword ptr [edx], ecx
        pop    ebx
        add    esp, 0ch
        ret    18h
loc_4_0D9F:
        mov    eax, dword ptr [esp + 20h]
        mov    ecx, dword ptr [esp + 30h]
        pop    ebp
        pop    edi
        pop    esi
        mov    dword ptr [eax], 0c0000001h
        pop    ebx
        mov    dword ptr [ecx], 0
        add    esp, 0ch
        ret    18h
        db  08Dh, 049h, 000h
sub_4_0DC0:
        mov    edx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [edx + 24h]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [esp + 0ch]
        jbe    SHORT loc_4_0DE9
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax], 0c0000001h
        mov    dword ptr [ecx], 0
        ret    14h
loc_4_0DE9:
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [edx + 24h]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [edx + 24h]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [edx + 4]
        add    eax, eax
        mov    edx, dword ptr [esp + 14h]
        push   eax
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0E0F:
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0E20:
        mov    edx, dword ptr [esp + 4]
        push   esi
        mov    ecx, dword ptr [esp + 0ch]
        lea    esi, [edx + 14h]
        cmp    dword ptr [esi], 0
        je     SHORT loc_4_0E46
loc_4_0E31:
        mov    eax, dword ptr [esi]
        cmp    ecx, eax
        je     SHORT loc_4_0E41
        lea    esi, [eax + 10h]
        cmp    dword ptr [esi], 0
        jne    SHORT loc_4_0E31
        jmp    SHORT loc_4_0E46
loc_4_0E41:
        mov    eax, dword ptr [ecx + 10h]
        mov    dword ptr [esi], eax
loc_4_0E46:
        lea    esi, [edx + 44h]
        cmp    dword ptr [esi], 0
        je     SHORT loc_4_0E63
loc_4_0E4E:
        mov    eax, dword ptr [esi]
        cmp    ecx, eax
        je     SHORT loc_4_0E5E
        lea    esi, [eax + 2ch]
        cmp    dword ptr [esi], 0
        jne    SHORT loc_4_0E4E
        jmp    SHORT loc_4_0E63
loc_4_0E5E:
        mov    eax, dword ptr [ecx + 2ch]
        mov    dword ptr [esi], eax
loc_4_0E63:
        add    edx, 48h
        cmp    dword ptr [edx], 0
        je     SHORT loc_4_0E80
loc_4_0E6B:
        mov    eax, dword ptr [edx]
        cmp    ecx, eax
        je     SHORT loc_4_0E7B
        lea    edx, [eax + 30h]
        cmp    dword ptr [edx], 0
        jne    SHORT loc_4_0E6B
        jmp    SHORT loc_4_0E80
loc_4_0E7B:
        mov    eax, dword ptr [ecx + 30h]
        mov    dword ptr [edx], eax
loc_4_0E80:
        xor    eax, eax
        pop    esi
        mov    dword ptr [ecx + 2ch], eax
        mov    dword ptr [ecx + 30h], eax
        mov    dword ptr [ecx + 10h], eax
        ret    8
        db  090h
sub_4_0E90:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   esi
        call   sub_4_0E20
        cmp    byte ptr [edi + 1eh], 0
        je     SHORT loc_4_0EB0
        mov    eax, dword ptr [edi + 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0EB0:
        cmp    byte ptr [esp + 14h], 0
        je     SHORT loc_4_0EBD
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 2ch]
loc_4_0EBD:
        mov    eax, 1
        mov    cl, byte ptr [edi + 1ch]
        shl    eax, cl
        push   0
        or     dword ptr [esi + 30h], eax
        push   34h
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0ED4:
        pop    edi
        pop    esi
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0EE0:
        sub    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ecx, [esp]
        push   edi
        push   eax
        push   0
        push   4ch
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0EF8:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 24h]
        test   eax, eax
        mov    dword ptr [ecx], edx
        je     SHORT loc_4_0F0F
        xor    al, al
        pop    edi
        add    esp, 4
        ret    1ch
loc_4_0F0F:
        mov    edi, dword ptr [esp + 4]
        xor    eax, eax
        mov    ecx, 13h
        rep stosd
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 20h]
        mov    dword ptr [eax + 30h], 0ffffffffh
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [ecx], edx
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [esp + 4]
        mov    dword ptr [edx + 18h], ecx
        mov    edx, dword ptr [esp + 4]
        mov    ax, word ptr [eax + 4]
        mov    ecx, dword ptr [esp + 10h]
        mov    word ptr [edx + 1ch], ax
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [edx + 20h], eax
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [edx + 24h], ecx
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [edx + 28h], eax
        mov    edx, dword ptr [esp + 4]
        mov    al, 1
        pop    edi
        mov    dword ptr [edx + 2ch], ecx
        add    esp, 4
        ret    1ch
        db  090h
sub_4_0F80:
        mov    eax, dword ptr [esp + 4]
        push   0
        push   4ch
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0F8E:
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0FA0:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        cmp    dword ptr [esi + 30h], 0
        jne    SHORT loc_4_0FB9
        xor    al, al
        pop    edi
        pop    esi
        add    esp, 4
        ret    10h
loc_4_0FB9:
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ecx, [esp + 8]
        push   eax
        push   0
        push   34h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0FCD:
        test   eax, eax
        je     SHORT loc_4_0FDB
        xor    al, al
        pop    edi
        pop    esi
        add    esp, 4
        ret    10h
loc_4_0FDB:
        mov    edi, dword ptr [esp + 8]
        xor    eax, eax
        mov    ecx, 0dh
        rep stosd
        xor    ecx, ecx
        mov    edi, dword ptr [esi + 30h]
loc_4_0FED:
        mov    eax, 1
        shl    eax, cl
        test   edi, eax
        jne    SHORT loc_4_1004
        inc    ecx
        cmp    ecx, 20h
        jb     SHORT loc_4_0FED
        mov    edx, dword ptr [esp + 8]
        jmp    SHORT loc_4_1014
loc_4_1004:
        mov    edx, ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    eax, edi
        mov    dword ptr [esi + 30h], eax
loc_4_1014:
        mov    eax, dword ptr [esi + 14h]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx + 10h], eax
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esi + 14h], ecx
        mov    dword ptr [ecx + 18h], 1
        mov    ecx, dword ptr [esp + 8]
        mov    byte ptr [ecx + 1ch], dl
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 18h]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [ecx + 4], edx
        mov    ecx, dword ptr [esp + 8]
        pop    edi
        mov    dword ptr [eax], ecx
        mov    al, 1
        pop    esi
        add    esp, 4
        ret    10h
        db  08Dh, 064h, 024h, 000h
sub_4_1060:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 1ch]
        push   0
        mov    edi, dword ptr [esp + 1ch]
        push   0
        mov    esi, dword ptr [esp + 1ch]
        push   ebp
        push   edi
        push   esi
        call   sub_4_1510
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_4_108A
        cmp    ebx, 103h
        jne    SHORT loc_4_109F
loc_4_108A:
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   ebp
        push   edi
        push   esi
        call   sub_4_1130
        test   eax, eax
        je     SHORT loc_4_109F
        mov    ebx, eax
loc_4_109F:
        test   ebx, ebx
        je     SHORT loc_4_10AB
        cmp    ebx, 103h
        jne    SHORT loc_4_10D3
loc_4_10AB:
        cmp    byte ptr [edi + 1dh], 0
        je     SHORT loc_4_10D3
        dec    dword ptr [esi + 0ch]
        mov    byte ptr [edi + 1dh], 0
        cmp    dword ptr [esi + 0ch], 0
        jne    SHORT loc_4_10D3
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   ebp
        push   edi
        push   esi
        call   sub_4_11F0
        test   eax, eax
        je     SHORT loc_4_10D3
        mov    ebx, eax
loc_4_10D3:
        test   ebx, ebx
        je     SHORT loc_4_10DF
        cmp    ebx, 103h
        jne    SHORT loc_4_10FF
loc_4_10DF:
        mov    eax, dword ptr [edi + 18h]
        dec    eax
        mov    dword ptr [edi + 18h], eax
        jne    SHORT loc_4_10FA
        push   0
        push   edi
        push   esi
        call   sub_4_0E90
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
loc_4_10FA:
        mov    ebx, 0c001000eh
loc_4_10FF:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_1110:
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax + 20h]
        mov    dword ptr [eax + 0ch], ecx
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax + 34h]
        mov    dword ptr [eax + 4], edx
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1130:
        push   ebx
        xor    ecx, ecx
        mov    eax, dword ptr [esp + 14h]
        push   esi
        push   edi
        xor    edx, edx
        mov    cl, byte ptr [eax + 2]
        mov    dl, byte ptr [eax]
        shl    edx, 10h
        or     ecx, edx
        xor    edx, edx
        shl    ecx, 8
        mov    dl, byte ptr [eax + 1]
        shl    edx, 10h
        mov    esi, dword ptr [esp + 14h]
        or     ecx, edx
        xor    edx, edx
        mov    dl, byte ptr [eax + 3]
        mov    edi, dword ptr [esp + 10h]
        or     ecx, edx
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 0ch], ecx
        mov    edx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [edi + 34h], edx
        test   eax, eax
        mov    dword ptr [edi + 4], 0
        je     SHORT loc_4_118B
loc_4_117E:
        mov    ecx, dword ptr [eax + 0ch]
        or     dword ptr [edi + 4], ecx
        mov    eax, dword ptr [eax + 10h]
        test   eax, eax
        jne    SHORT loc_4_117E
loc_4_118B:
        mov    ecx, dword ptr [edi + 4]
        cmp    ecx, edx
        je     SHORT loc_4_11C1
        mov    eax, dword ptr [esp + 20h]
        mov    ebx, dword ptr [esp + 18h]
        push   eax
        push   ebx
        mov    eax, dword ptr [esi]
        push   eax
        push   ecx
        push   edx
        call   dword ptr [edi + 20h]
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_4_11C3
        cmp    ebx, 103h
        je     SHORT loc_4_11C3
        push   esi
        push   edi
        call   sub_4_1110
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    14h
loc_4_11C1:
        xor    ebx, ebx
loc_4_11C3:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    14h
        db  005h, 000h, 000h, 000h, 000h
sub_4_11D0:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 38h]
        mov    edx, dword ptr [ecx + 3ch]
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx + 0ch], edx
        mov    eax, dword ptr [esp + 8]
        mov    cl, byte ptr [eax + 28h]
        mov    byte ptr [eax + 1dh], cl
        ret    8
        db  08Dh, 049h, 000h
sub_4_11F0:
        mov    eax, dword ptr [esp + 10h]
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    bl, byte ptr [eax + 2]
        push   ebp
        xor    ecx, ecx
        mov    cl, byte ptr [eax]
        shl    ecx, 10h
        or     ebx, ecx
        xor    ecx, ecx
        shl    ebx, 8
        mov    cl, byte ptr [eax + 1]
        shl    ecx, 10h
        mov    esi, dword ptr [esp + 14h]
        or     ebx, ecx
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 3]
        mov    edi, dword ptr [esp + 18h]
        or     ebx, ecx
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [esi + 0ch]
        mov    dword ptr [esi + 38h], eax
        mov    dword ptr [esi + 3ch], ecx
        mov    al, byte ptr [edi + 1dh]
        mov    byte ptr [edi + 28h], al
        jne    SHORT loc_4_126E
        cmp    byte ptr [edi + 1dh], 0
        je     SHORT loc_4_1256
        dec    dword ptr [esi + 0ch]
        mov    byte ptr [edi + 1dh], 0
        cmp    dword ptr [esi + 0ch], 0
        je     NEAR PTR loc_4_12D8
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    14h
loc_4_1256:
        mov    eax, 0
        pop    ebp
        cmp    dword ptr [esi + 0ch], 1
        adc    eax, -1
        pop    edi
        and    eax, 0c001001ah
        pop    esi
        pop    ebx
        ret    14h
loc_4_126E:
        cmp    dword ptr [esi + 8], ebx
        jne    SHORT loc_4_129A
        cmp    byte ptr [edi + 1dh], 0
        je     SHORT loc_4_1282
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    14h
loc_4_1282:
        mov    eax, dword ptr [esi + 0ch]
        test   eax, eax
        je     SHORT loc_4_12D8
        inc    eax
        pop    ebp
        mov    dword ptr [esi + 0ch], eax
        xor    eax, eax
        mov    byte ptr [edi + 1dh], 1
        pop    edi
        pop    esi
        pop    ebx
        ret    14h
loc_4_129A:
        mov    eax, dword ptr [esi + 0ch]
        cmp    eax, 1
        jbe    SHORT loc_4_12AE
        mov    eax, 0c001001ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    14h
loc_4_12AE:
        jne    SHORT loc_4_12C2
        cmp    byte ptr [edi + 1dh], 0
        jne    SHORT loc_4_12C2
        mov    eax, 0c001001ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    14h
loc_4_12C2:
        cmp    eax, 1
        jne    SHORT loc_4_12D8
        cmp    byte ptr [edi + 1dh], 0
        je     SHORT loc_4_12D8
        mov    dword ptr [esi + 0ch], 0
        mov    byte ptr [edi + 1dh], 0
loc_4_12D8:
        mov    eax, dword ptr [esp + 24h]
        mov    ecx, dword ptr [esp + 1ch]
        push   eax
        mov    dword ptr [esi + 8], ebx
        push   ecx
        mov    edx, dword ptr [edi]
        push   edx
        mov    eax, dword ptr [esi + 38h]
        push   ebx
        push   eax
        call   dword ptr [esi + 24h]
        mov    ebp, eax
        test   ebp, ebp
        je     SHORT loc_4_130E
        cmp    ebp, 103h
        je     SHORT loc_4_130E
        push   edi
        push   esi
        call   sub_4_11D0
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    14h
loc_4_130E:
        test   ebx, ebx
        jne    SHORT loc_4_1326
        mov    byte ptr [edi + 1dh], 0
        mov    dword ptr [esi + 0ch], 0
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    14h
loc_4_1326:
        mov    byte ptr [edi + 1dh], 1
        mov    dword ptr [esi + 0ch], 1
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_1340:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        cmp    byte ptr [esp + 10h], 0
        push   esi
        je     SHORT loc_4_1378
        mov    eax, dword ptr [edx + 44h]
        xor    bl, bl
        mov    ecx, eax
        mov    esi, dword ptr [esp + 10h]
        test   ecx, ecx
        je     SHORT loc_4_1369
loc_4_135C:
        cmp    ecx, esi
        jne    SHORT loc_4_1362
        mov    bl, 1
loc_4_1362:
        mov    ecx, dword ptr [ecx + 2ch]
        test   ecx, ecx
        jne    SHORT loc_4_135C
loc_4_1369:
        test   bl, bl
        jne    SHORT loc_4_13A0
        mov    dword ptr [esi + 2ch], eax
        mov    dword ptr [edx + 44h], esi
        pop    esi
        pop    ebx
        ret    0ch
loc_4_1378:
        add    edx, 44h
        mov    esi, dword ptr [esp + 10h]
        cmp    dword ptr [edx], 0
        je     SHORT loc_4_1399
loc_4_1384:
        mov    eax, dword ptr [edx]
        cmp    esi, eax
        je     SHORT loc_4_1394
        lea    edx, [eax + 2ch]
        cmp    dword ptr [edx], 0
        jne    SHORT loc_4_1384
        jmp    SHORT loc_4_1399
loc_4_1394:
        mov    eax, dword ptr [esi + 2ch]
        mov    dword ptr [edx], eax
loc_4_1399:
        mov    dword ptr [esi + 2ch], 0
loc_4_13A0:
        pop    esi
        pop    ebx
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_13B0:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        cmp    byte ptr [esp + 10h], 0
        push   esi
        je     SHORT loc_4_13E8
        mov    eax, dword ptr [edx + 48h]
        xor    bl, bl
        mov    ecx, eax
        mov    esi, dword ptr [esp + 10h]
        test   ecx, ecx
        je     SHORT loc_4_13D9
loc_4_13CC:
        cmp    ecx, esi
        jne    SHORT loc_4_13D2
        mov    bl, 1
loc_4_13D2:
        mov    ecx, dword ptr [ecx + 30h]
        test   ecx, ecx
        jne    SHORT loc_4_13CC
loc_4_13D9:
        test   bl, bl
        jne    SHORT loc_4_1410
        mov    dword ptr [esi + 30h], eax
        mov    dword ptr [edx + 48h], esi
        pop    esi
        pop    ebx
        ret    0ch
loc_4_13E8:
        add    edx, 48h
        mov    esi, dword ptr [esp + 10h]
        cmp    dword ptr [edx], 0
        je     SHORT loc_4_1409
loc_4_13F4:
        mov    eax, dword ptr [edx]
        cmp    esi, eax
        je     SHORT loc_4_1404
        lea    edx, [eax + 30h]
        cmp    dword ptr [edx], 0
        jne    SHORT loc_4_13F4
        jmp    SHORT loc_4_1409
loc_4_1404:
        mov    eax, dword ptr [esi + 30h]
        mov    dword ptr [edx], eax
loc_4_1409:
        mov    dword ptr [esi + 30h], 0
loc_4_1410:
        pop    esi
        pop    ebx
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1420:
        sub    esp, 4
        xor    eax, eax
        xor    cl, cl
        push   ebx
        push   esi
        xor    bl, bl
        mov    esi, dword ptr [esp + 14h]
        push   edi
        mov    byte ptr [esp + 0fh], al
        mov    edx, dword ptr [esi + 24h]
        test   dl, 0a0h
        je     SHORT loc_4_1447
        mov    eax, 1
        mov    byte ptr [esp + 0fh], al
        jmp    SHORT loc_4_145B
loc_4_1447:
        test   dl, 1
        je     SHORT loc_4_144E
        mov    al, 1
loc_4_144E:
        test   edx, 7008h
        je     SHORT loc_4_145B
        mov    byte ptr [esp + 0fh], 1
loc_4_145B:
        mov    edx, dword ptr [esi + 8]
        test   dl, 0a0h
        je     SHORT loc_4_146C
        mov    ebx, 1
        mov    cl, bl
        jmp    SHORT loc_4_147D
loc_4_146C:
        test   dl, 1
        je     SHORT loc_4_1473
        mov    cl, 1
loc_4_1473:
        test   edx, 7008h
        je     SHORT loc_4_147D
        mov    bl, 1
loc_4_147D:
        test   al, al
        jne    SHORT loc_4_1498
        test   cl, cl
        je     SHORT loc_4_1494
        mov    edi, dword ptr [esp + 14h]
        push   1
        push   esi
        push   edi
        call   sub_4_1340
        jmp    SHORT loc_4_14AF
loc_4_1494:
        test   al, al
        je     SHORT loc_4_14AB
loc_4_1498:
        test   cl, cl
        jne    SHORT loc_4_14AB
        mov    edi, dword ptr [esp + 14h]
        push   0
        push   esi
        push   edi
        call   sub_4_1340
        jmp    SHORT loc_4_14AF
loc_4_14AB:
        mov    edi, dword ptr [esp + 14h]
loc_4_14AF:
        cmp    byte ptr [esp + 0fh], 0
        jne    SHORT loc_4_14D3
        test   bl, bl
        je     SHORT loc_4_14CC
        push   1
        push   esi
        push   edi
        call   sub_4_13B0
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
loc_4_14CC:
        cmp    byte ptr [esp + 0fh], 0
        je     SHORT loc_4_14E0
loc_4_14D3:
        test   bl, bl
        jne    SHORT loc_4_14E0
        push   0
        push   esi
        push   edi
        call   sub_4_13B0
loc_4_14E0:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_14F0:
        mov    edx, dword ptr [esp + 8]
        push   edx
        mov    eax, dword ptr [edx + 24h]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [esp + 8]
        push   eax
        mov    ecx, dword ptr [eax + 40h]
        mov    dword ptr [eax + 10h], ecx
        call   sub_4_1420
        ret    8
        db  08Bh, 0FFh
sub_4_1510:
        mov    ecx, dword ptr [esp + 10h]
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [esi + 24h], eax
        mov    dword ptr [esi + 8], ecx
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [edi + 10h], 0
        mov    dword ptr [edi + 40h], eax
        mov    eax, dword ptr [edi + 14h]
        test   eax, eax
        je     SHORT loc_4_1549
loc_4_153C:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [edi + 10h], ecx
        mov    eax, dword ptr [eax + 10h]
        test   eax, eax
        jne    SHORT loc_4_153C
loc_4_1549:
        push   esi
        push   edi
        call   sub_4_1420
        mov    ecx, dword ptr [edi + 40h]
        mov    edx, dword ptr [edi + 10h]
        mov    eax, edx
        xor    eax, ecx
        test   eax, 0ffffff7fh
        je     SHORT loc_4_159C
        mov    eax, dword ptr [esp + 20h]
        mov    ebx, dword ptr [esp + 18h]
        push   eax
        and    edx, 0ffffff7fh
        push   ebx
        mov    eax, dword ptr [esi]
        push   eax
        and    ecx, 0ffffff7fh
        push   edx
        push   ecx
        call   dword ptr [edi + 28h]
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_4_159E
        cmp    ebx, 103h
        je     SHORT loc_4_159E
        push   esi
        push   edi
        call   sub_4_14F0
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    14h
loc_4_159C:
        xor    ebx, ebx
loc_4_159E:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    14h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_15B0:
        mov    edx, dword ptr [esp + 4]
        push   esi
        mov    ecx, dword ptr [esp + 0ch]
        lea    esi, [edx + 18h]
        cmp    dword ptr [esi], 0
        je     SHORT loc_4_15D6
loc_4_15C1:
        mov    eax, dword ptr [esi]
        cmp    ecx, eax
        je     SHORT loc_4_15D1
        lea    esi, [eax + 14h]
        cmp    dword ptr [esi], 0
        jne    SHORT loc_4_15C1
        jmp    SHORT loc_4_15D6
loc_4_15D1:
        mov    eax, dword ptr [ecx + 14h]
        mov    dword ptr [esi], eax
loc_4_15D6:
        lea    esi, [edx + 60h]
        cmp    dword ptr [esi], 0
        je     SHORT loc_4_15F3
loc_4_15DE:
        mov    eax, dword ptr [esi]
        cmp    ecx, eax
        je     SHORT loc_4_15EE
        lea    esi, [eax + 1ch]
        cmp    dword ptr [esi], 0
        jne    SHORT loc_4_15DE
        jmp    SHORT loc_4_15F3
loc_4_15EE:
        mov    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [esi], eax
loc_4_15F3:
        add    edx, 64h
        cmp    dword ptr [edx], 0
        je     SHORT loc_4_1610
loc_4_15FB:
        mov    eax, dword ptr [edx]
        cmp    ecx, eax
        je     SHORT loc_4_160B
        lea    edx, [eax + 20h]
        cmp    dword ptr [edx], 0
        jne    SHORT loc_4_15FB
        jmp    SHORT loc_4_1610
loc_4_160B:
        mov    eax, dword ptr [ecx + 20h]
        mov    dword ptr [edx], eax
loc_4_1610:
        xor    eax, eax
        pop    esi
        mov    dword ptr [ecx + 1ch], eax
        mov    dword ptr [ecx + 20h], eax
        mov    dword ptr [ecx + 14h], eax
        ret    8
        db  090h
sub_4_1620:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   esi
        call   sub_4_15B0
        cmp    byte ptr [edi + 10h], 0
        je     SHORT loc_4_1640
        mov    eax, dword ptr [edi + 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1640:
        cmp    byte ptr [esp + 14h], 0
        je     SHORT loc_4_164D
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 24h]
loc_4_164D:
        mov    eax, 1
        mov    cl, byte ptr [edi + 11h]
        shl    eax, cl
        push   0
        or     dword ptr [esi + 38h], eax
        push   24h
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1664:
        pop    edi
        pop    esi
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1670:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        lea    eax, [esp + 0ch]
        push   ebp
        push   ebx
        push   0
        push   68h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_168C:
        mov    edx, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 38h]
        test   eax, eax
        mov    dword ptr [ecx], edx
        je     SHORT loc_4_16A6
        xor    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    24h
loc_4_16A6:
        mov    edi, dword ptr [esp + 10h]
        xor    eax, eax
        mov    ecx, 1ah
        rep stosd
        cmp    dword ptr [esp + 18h], eax
        jne    SHORT loc_4_16C1
        mov    dword ptr [esp + 18h], 2
loc_4_16C1:
        mov    ecx, dword ptr [esp + 18h]
        push   ebx
        push   0
        lea    eax, [ecx + ecx*2]
        lea    esi, [eax*2]
        mov    eax, dword ptr [esp + 18h]
        push   esi
        add    eax, 4
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_16E0:
        test   eax, eax
        je     SHORT loc_4_16FA
        mov    eax, dword ptr [esp + 10h]
        push   eax
        call   sub_4_19A0
        xor    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    24h
loc_4_16FA:
        mov    ecx, esi
        mov    edx, esi
        shr    ecx, 2
        mov    eax, dword ptr [esp + 10h]
        mov    edi, dword ptr [eax + 4]
        xor    eax, eax
        rep stosd
        mov    ecx, edx
        push   ebx
        and    ecx, 3
        push   eax
        rep stosb
        mov    eax, dword ptr [esp + 18h]
        push   esi
        add    eax, 4ch
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1723:
        test   eax, eax
        mov    eax, dword ptr [esp + 10h]
        je     SHORT loc_4_173D
        push   eax
        call   sub_4_19A0
        xor    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    24h
loc_4_173D:
        mov    edi, dword ptr [eax + 4ch]
        mov    ecx, esi
        shr    ecx, 2
        xor    eax, eax
        rep stosd
        mov    ecx, esi
        push   ebx
        and    ecx, 3
        push   eax
        rep stosb
        mov    ebp, dword ptr [esp + 20h]
        mov    eax, dword ptr [esp + 18h]
        shl    ebp, 2
        add    eax, 8
        push   ebp
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1767:
        test   eax, eax
        je     SHORT loc_4_1781
        mov    eax, dword ptr [esp + 10h]
        push   eax
        call   sub_4_19A0
        xor    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    24h
loc_4_1781:
        mov    esi, dword ptr [esp + 18h]
        mov    eax, dword ptr [esp + 10h]
        shl    esi, 2
        mov    edi, dword ptr [eax + 8]
        xor    eax, eax
        mov    ecx, esi
        shr    ecx, 2
        rep stosd
        mov    ecx, esi
        push   ebx
        and    ecx, 3
        push   eax
        rep stosb
        mov    eax, dword ptr [esp + 18h]
        push   ebp
        add    eax, 54h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_17AF:
        test   eax, eax
        mov    eax, dword ptr [esp + 10h]
        je     SHORT loc_4_17C9
        push   eax
        call   sub_4_19A0
        xor    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    24h
loc_4_17C9:
        mov    edi, dword ptr [eax + 54h]
        mov    ecx, esi
        shr    ecx, 2
        xor    eax, eax
        rep stosd
        mov    ecx, esi
        and    ecx, 3
        rep stosb
        mov    esi, dword ptr [esp + 1ch]
        cmp    esi, eax
        jne    SHORT loc_4_17E9
        mov    esi, 2
loc_4_17E9:
        push   ebx
        lea    eax, [esi*2]
        push   0
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        add    eax, 0ch
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1801:
        test   eax, eax
        je     SHORT loc_4_181B
        mov    eax, dword ptr [esp + 10h]
        push   eax
        call   sub_4_19A0
        xor    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    24h
loc_4_181B:
        lea    ebp, [esi*2]
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, ebp
        mov    edi, dword ptr [eax + 0ch]
        shr    ecx, 2
        xor    eax, eax
        rep stosd
        mov    ecx, ebp
        push   ebx
        and    ecx, 3
        push   eax
        rep stosb
        mov    eax, dword ptr [esp + 18h]
        lea    ecx, [esi*2]
        push   ecx
        add    eax, 50h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1850:
        test   eax, eax
        mov    eax, dword ptr [esp + 10h]
        je     SHORT loc_4_186A
        push   eax
        call   sub_4_19A0
        xor    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    24h
loc_4_186A:
        mov    edi, dword ptr [eax + 50h]
        mov    ecx, ebp
        shr    ecx, 2
        xor    eax, eax
        rep stosd
        mov    ecx, ebp
        push   ebx
        and    ecx, 3
        push   eax
        rep stosb
        mov    eax, dword ptr [esp + 18h]
        lea    ecx, [esi*4]
        push   ecx
        add    eax, 10h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1894:
        test   eax, eax
        je     SHORT loc_4_18AE
        mov    eax, dword ptr [esp + 10h]
        push   eax
        call   sub_4_19A0
        xor    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    24h
loc_4_18AE:
        lea    ebp, [esi*4]
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, ebp
        mov    edi, dword ptr [eax + 10h]
        shr    ecx, 2
        xor    eax, eax
        rep stosd
        mov    ecx, ebp
        push   ebx
        and    ecx, 3
        push   eax
        rep stosb
        mov    eax, dword ptr [esp + 18h]
        lea    ecx, [esi*4]
        push   ecx
        add    eax, 58h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_18E3:
        test   eax, eax
        mov    eax, dword ptr [esp + 10h]
        je     SHORT loc_4_18FD
        push   eax
        call   sub_4_19A0
        xor    al, al
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    24h
loc_4_18FD:
        mov    edi, dword ptr [eax + 58h]
        mov    ecx, ebp
        shr    ecx, 2
        xor    eax, eax
        rep stosd
        mov    ecx, ebp
        and    ecx, 3
        rep stosb
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 2ch]
        mov    edi, 6
        mov    dword ptr [eax + 38h], 0ffffffffh
        mov    eax, dword ptr [esp + 10h]
        add    eax, 3ch
loc_4_192B:
        mov    dl, byte ptr [ecx]
        inc    eax
        inc    ecx
        dec    edi
        mov    byte ptr [eax - 1], dl
        jne    SHORT loc_4_192B
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 30h]
        add    eax, 42h
        mov    edi, 2
loc_4_1945:
        mov    dl, byte ptr [ecx]
        inc    eax
        inc    ecx
        dec    edi
        mov    byte ptr [eax - 1], dl
        jne    SHORT loc_4_1945
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 34h]
        mov    edx, dword ptr [esp + 20h]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [ecx + 1ch], edx
        mov    ecx, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 28h]
        mov    dword ptr [ecx + 20h], eax
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 24h], edx
        mov    ecx, dword ptr [esp + 18h]
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 28h], ecx
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        mov    dword ptr [eax + 2ch], esi
        mov    al, 1
        pop    esi
        pop    ebx
        add    esp, 4
        ret    24h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_19A0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 4]
        test   eax, eax
        je     SHORT loc_4_19BD
        push   0
        mov    ecx, dword ptr [esi + 28h]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_19BD:
        mov    eax, dword ptr [esi + 4ch]
        test   eax, eax
        je     SHORT loc_4_19D5
        push   0
        mov    ecx, dword ptr [esi + 28h]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_19D5:
        mov    eax, dword ptr [esi + 8]
        test   eax, eax
        je     SHORT loc_4_19EB
        push   0
        mov    ecx, dword ptr [esi + 28h]
        shl    ecx, 2
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_19EB:
        mov    eax, dword ptr [esi + 54h]
        test   eax, eax
        je     SHORT loc_4_1A01
        push   0
        mov    ecx, dword ptr [esi + 28h]
        shl    ecx, 2
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1A01:
        mov    eax, dword ptr [esi + 0ch]
        test   eax, eax
        je     SHORT loc_4_1A16
        push   0
        mov    ecx, dword ptr [esi + 2ch]
        add    ecx, ecx
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1A16:
        mov    eax, dword ptr [esi + 50h]
        test   eax, eax
        je     SHORT loc_4_1A2B
        push   0
        mov    ecx, dword ptr [esi + 2ch]
        add    ecx, ecx
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1A2B:
        mov    eax, dword ptr [esi + 10h]
        test   eax, eax
        je     SHORT loc_4_1A41
        push   0
        mov    ecx, dword ptr [esi + 2ch]
        shl    ecx, 2
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1A41:
        mov    eax, dword ptr [esi + 58h]
        test   eax, eax
        je     SHORT loc_4_1A57
        push   0
        mov    ecx, dword ptr [esi + 2ch]
        shl    ecx, 2
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1A57:
        push   0
        push   68h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1A61:
        pop    esi
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1A70:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        cmp    dword ptr [esi + 38h], 0
        jne    SHORT loc_4_1A89
        xor    al, al
        pop    edi
        pop    esi
        add    esp, 4
        ret    10h
loc_4_1A89:
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ecx, [esp + 8]
        push   eax
        push   0
        push   24h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1A9D:
        test   eax, eax
        je     SHORT loc_4_1AAB
        xor    al, al
        pop    edi
        pop    esi
        add    esp, 4
        ret    10h
loc_4_1AAB:
        mov    edi, dword ptr [esp + 8]
        xor    eax, eax
        mov    ecx, 9
        rep stosd
        xor    ecx, ecx
        mov    edi, dword ptr [esi + 38h]
loc_4_1ABD:
        mov    eax, 1
        shl    eax, cl
        test   edi, eax
        jne    SHORT loc_4_1AD4
        inc    ecx
        cmp    ecx, 20h
        jb     SHORT loc_4_1ABD
        mov    edx, dword ptr [esp + 8]
        jmp    SHORT loc_4_1AE4
loc_4_1AD4:
        mov    edx, ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    eax, edi
        mov    dword ptr [esi + 38h], eax
loc_4_1AE4:
        mov    eax, dword ptr [esi + 18h]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx + 14h], eax
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esi + 18h], ecx
        mov    dword ptr [ecx + 0ch], 1
        mov    ecx, dword ptr [esp + 8]
        mov    byte ptr [ecx + 11h], dl
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 18h]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [ecx + 4], edx
        mov    ecx, dword ptr [esp + 8]
        pop    edi
        mov    dword ptr [eax], ecx
        mov    al, 1
        pop    esi
        add    esp, 4
        ret    10h
        db  08Dh, 064h, 024h, 000h
sub_4_1B30:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 1ch]
        push   0
        mov    edi, dword ptr [esp + 1ch]
        push   0
        mov    esi, dword ptr [esp + 1ch]
        push   ebp
        push   edi
        push   esi
        call   sub_4_22A0
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_4_1B5A
        cmp    ebx, 103h
        jne    SHORT loc_4_1B8E
loc_4_1B5A:
        push   0
        push   0
        push   0
        push   ebp
        push   edi
        push   esi
        call   sub_4_1C10
        test   eax, eax
        je     SHORT loc_4_1B6E
        mov    ebx, eax
loc_4_1B6E:
        test   ebx, ebx
        je     SHORT loc_4_1B7A
        cmp    ebx, 103h
        jne    SHORT loc_4_1B8E
loc_4_1B7A:
        push   0
        push   0
        push   0
        push   ebp
        push   edi
        push   esi
        call   sub_4_1E90
        test   eax, eax
        je     SHORT loc_4_1B8E
        mov    ebx, eax
loc_4_1B8E:
        test   ebx, ebx
        je     SHORT loc_4_1B9A
        cmp    ebx, 103h
        jne    SHORT loc_4_1BBA
loc_4_1B9A:
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_4_1BB5
        push   0
        push   edi
        push   esi
        call   sub_4_1620
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
loc_4_1BB5:
        mov    ebx, 0c001000eh
loc_4_1BBA:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1BD0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 44h]
        mov    ecx, dword ptr [esi + 4ch]
        mov    edx, dword ptr [esi + 4]
        mov    dword ptr [esi + 30h], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1BEE:
        mov    eax, dword ptr [esi + 30h]
        mov    ecx, dword ptr [esi + 54h]
        shl    eax, 2
        mov    edx, dword ptr [esi + 8]
        push   eax
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1C02:
        pop    esi
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1C10:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [esp + 1ch]
        push   ebp
        mov    byte ptr [esp + 13h], bl
        mov    eax, dword ptr [esi + 30h]
        shl    eax, 2
        mov    ecx, dword ptr [esi + 8]
        push   eax
        mov    edx, dword ptr [esi + 54h]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1C35:
        mov    ecx, dword ptr [esi + 30h]
        mov    edx, dword ptr [esi + 4ch]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [esi + 4]
        add    eax, eax
        push   eax
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1C4B:
        mov    eax, dword ptr [esi + 30h]
        mov    dword ptr [esi + 44h], eax
        test   eax, eax
        je     SHORT loc_4_1C79
        xor    eax, eax
        mov    edi, dword ptr [esp + 24h]
loc_4_1C5B:
        mov    edx, dword ptr [esi + 8]
        mov    ebp, 1
        mov    cl, byte ptr [edi + 11h]
        add    eax, 4
        shl    ebp, cl
        not    ebp
        inc    ebx
        and    dword ptr [edx + eax - 4], ebp
        cmp    dword ptr [esi + 30h], ebx
        ja     SHORT loc_4_1C5B
        jmp    SHORT loc_4_1C7D
loc_4_1C79:
        mov    edi, dword ptr [esp + 24h]
loc_4_1C7D:
        mov    dword ptr [esp + 14h], 0
        cmp    dword ptr [esi + 30h], 0
        jbe    SHORT loc_4_1CF4
loc_4_1C8B:
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esi + 8]
        cmp    dword ptr [ecx + eax*4], 0
        jne    SHORT loc_4_1CE7
        mov    byte ptr [esp + 13h], 1
        lea    ecx, [eax + eax*2]
        mov    eax, dword ptr [esi + 30h]
        add    ecx, ecx
        sub    eax, dword ptr [esp + 14h]
        add    ecx, dword ptr [esi + 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        lea    edx, [ecx + 6]
        sub    eax, 6
        push   eax
        push   edx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1CBF:
        mov    ecx, dword ptr [esp + 14h]
        mov    eax, dword ptr [esi + 30h]
        shl    ecx, 2
        sub    eax, dword ptr [esp + 14h]
        add    ecx, dword ptr [esi + 8]
        lea    edx, [eax*4 - 4]
        push   edx
        lea    ebx, [ecx + 4]
        push   ebx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1CE2:
        dec    dword ptr [esi + 30h]
        jmp    SHORT loc_4_1CEB
loc_4_1CE7:
        inc    dword ptr [esp + 14h]
loc_4_1CEB:
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [esi + 30h], eax
        ja     SHORT loc_4_1C8B
loc_4_1CF4:
        mov    dword ptr [esp + 18h], 0
        cmp    dword ptr [esp + 2ch], 0
        je     NEAR PTR loc_4_1DE4
        mov    ebx, dword ptr [esp + 30h]
loc_4_1D0B:
        lea    eax, [esp + 14h]
        mov    ecx, dword ptr [esi + 4]
        push   eax
        mov    edx, dword ptr [esi + 30h]
        push   ebx
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1D1E:
        test   al, al
        je     SHORT loc_4_1D3C
        mov    eax, dword ptr [esp + 14h]
        mov    ebp, dword ptr [esi + 8]
        mov    edx, 1
        mov    cl, byte ptr [edi + 11h]
        shl    edx, cl
        or     dword ptr [ebp + eax*4], edx
        jmp    NEAR PTR loc_4_1DCF
loc_4_1D3C:
        mov    ecx, dword ptr [esi + 30h]
        cmp    dword ptr [esi + 28h], ecx
        jbe    NEAR PTR loc_4_1E2B
        mov    eax, dword ptr [esp + 14h]
        sub    ecx, dword ptr [esp + 14h]
        mov    byte ptr [esp + 13h], 1
        lea    eax, [eax + eax*2]
        lea    ecx, [ecx + ecx*2]
        add    eax, eax
        add    ecx, ecx
        add    eax, dword ptr [esi + 4]
        push   ecx
        push   eax
        add    eax, 6
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1D6D:
        mov    ecx, dword ptr [esp + 14h]
        mov    edx, ebx
        mov    ebp, 6
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        add    ecx, dword ptr [esi + 4]
loc_4_1D80:
        mov    al, byte ptr [edx]
        inc    ecx
        inc    edx
        dec    ebp
        mov    byte ptr [ecx - 1], al
        jne    SHORT loc_4_1D80
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esi + 30h]
        shl    eax, 2
        sub    ecx, dword ptr [esp + 14h]
        shl    ecx, 2
        add    eax, dword ptr [esi + 8]
        push   ecx
        push   eax
        add    eax, 4
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1DA9:
        mov    ebp, dword ptr [esp + 14h]
        mov    edx, dword ptr [esi + 8]
        mov    dword ptr [edx + ebp*4], 0
        mov    ebp, dword ptr [esi + 8]
        mov    eax, dword ptr [esp + 14h]
        mov    edx, 1
        mov    cl, byte ptr [edi + 11h]
        shl    edx, cl
        or     dword ptr [ebp + eax*4], edx
        inc    dword ptr [esi + 30h]
loc_4_1DCF:
        add    ebx, 6
        inc    dword ptr [esp + 18h]
        mov    eax, dword ptr [esp + 18h]
        cmp    eax, dword ptr [esp + 2ch]
        jb     NEAR PTR loc_4_1D0B
loc_4_1DE4:
        cmp    byte ptr [esp + 13h], 0
        je     SHORT loc_4_1E40
        mov    edx, dword ptr [esi + 0ch]
        mov    ecx, dword ptr [esi + 34h]
        mov    eax, dword ptr [esp + 34h]
        mov    ebx, dword ptr [esp + 28h]
        push   eax
        mov    ebp, dword ptr [edi]
        push   ebx
        mov    eax, dword ptr [esi + 4]
        push   ebp
        push   edx
        push   ecx
        push   edx
        push   ecx
        mov    edx, dword ptr [esi + 4ch]
        push   eax
        mov    ecx, dword ptr [esi + 30h]
        push   ecx
        mov    eax, dword ptr [esi + 44h]
        push   edx
        push   eax
        call   dword ptr [esi + 1ch]
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_4_1E42
        cmp    edi, 103h
        je     SHORT loc_4_1E42
        push   esi
        call   sub_4_1BD0
        jmp    SHORT loc_4_1E42
loc_4_1E2B:
        push   esi
        call   sub_4_1BD0
        mov    eax, 0c0010009h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret    18h
loc_4_1E40:
        xor    edi, edi
loc_4_1E42:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret    18h
        db  08Bh, 0FFh
sub_4_1E50:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 48h]
        mov    ecx, dword ptr [esi + 50h]
        mov    edx, dword ptr [esi + 0ch]
        mov    dword ptr [esi + 34h], eax
        add    eax, eax
        push   eax
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1E6B:
        mov    eax, dword ptr [esi + 34h]
        mov    ecx, dword ptr [esi + 58h]
        shl    eax, 2
        mov    edx, dword ptr [esi + 10h]
        push   eax
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1E7F:
        pop    esi
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1E90:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [esp + 1ch]
        push   ebp
        mov    byte ptr [esp + 13h], bl
        mov    eax, dword ptr [esi + 34h]
        shl    eax, 2
        mov    ecx, dword ptr [esi + 10h]
        push   eax
        mov    edx, dword ptr [esi + 58h]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1EB5:
        mov    eax, dword ptr [esi + 34h]
        mov    ecx, dword ptr [esi + 0ch]
        add    eax, eax
        mov    edx, dword ptr [esi + 50h]
        push   eax
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1EC8:
        mov    eax, dword ptr [esi + 34h]
        mov    dword ptr [esi + 48h], eax
        test   eax, eax
        je     SHORT loc_4_1EF6
        xor    eax, eax
        mov    edi, dword ptr [esp + 24h]
loc_4_1ED8:
        mov    edx, dword ptr [esi + 10h]
        mov    ebp, 1
        mov    cl, byte ptr [edi + 11h]
        add    eax, 4
        shl    ebp, cl
        not    ebp
        inc    ebx
        and    dword ptr [edx + eax - 4], ebp
        cmp    dword ptr [esi + 34h], ebx
        ja     SHORT loc_4_1ED8
        jmp    SHORT loc_4_1EFA
loc_4_1EF6:
        mov    edi, dword ptr [esp + 24h]
loc_4_1EFA:
        mov    dword ptr [esp + 18h], 0
        cmp    dword ptr [esp + 2ch], 0
        je     NEAR PTR loc_4_1FE0
        mov    ebx, dword ptr [esp + 30h]
loc_4_1F11:
        lea    eax, [esp + 14h]
        mov    ecx, dword ptr [esi + 0ch]
        push   eax
        mov    edx, dword ptr [esi + 34h]
        push   ebx
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1F24:
        test   al, al
        je     SHORT loc_4_1F41
        mov    ebp, dword ptr [esp + 14h]
        mov    edx, dword ptr [esi + 10h]
        mov    eax, 1
        mov    cl, byte ptr [edi + 11h]
        shl    eax, cl
        or     dword ptr [edx + ebp*4], eax
        jmp    NEAR PTR loc_4_1FCB
loc_4_1F41:
        mov    ecx, dword ptr [esi + 34h]
        cmp    dword ptr [esi + 2ch], ecx
        jbe    NEAR PTR loc_4_204B
        mov    eax, dword ptr [esp + 14h]
        sub    ecx, dword ptr [esp + 14h]
        add    eax, eax
        add    ecx, ecx
        add    eax, dword ptr [esi + 0ch]
        push   ecx
        mov    byte ptr [esp + 17h], 1
        push   eax
        add    eax, 2
        push   eax
        mov    ebp, 2
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1F71:
        mov    ecx, dword ptr [esp + 14h]
        mov    edx, ebx
        add    ecx, ecx
        add    ecx, dword ptr [esi + 0ch]
loc_4_1F7C:
        mov    al, byte ptr [edx]
        inc    ecx
        inc    edx
        dec    ebp
        mov    byte ptr [ecx - 1], al
        jne    SHORT loc_4_1F7C
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esi + 34h]
        shl    eax, 2
        sub    ecx, dword ptr [esp + 14h]
        shl    ecx, 2
        add    eax, dword ptr [esi + 10h]
        push   ecx
        push   eax
        add    eax, 4
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1FA5:
        mov    edx, dword ptr [esp + 14h]
        mov    ebp, dword ptr [esi + 10h]
        mov    dword ptr [ebp + edx*4], 0
        mov    edx, dword ptr [esp + 14h]
        mov    eax, dword ptr [esi + 10h]
        mov    ebp, 1
        mov    cl, byte ptr [edi + 11h]
        shl    ebp, cl
        or     dword ptr [eax + edx*4], ebp
        inc    dword ptr [esi + 34h]
loc_4_1FCB:
        add    ebx, 2
        inc    dword ptr [esp + 18h]
        mov    eax, dword ptr [esp + 18h]
        cmp    dword ptr [esp + 2ch], eax
        ja     NEAR PTR loc_4_1F11
loc_4_1FE0:
        mov    dword ptr [esp + 14h], 0
        cmp    dword ptr [esi + 34h], 0
        jbe    SHORT loc_4_206D
loc_4_1FEE:
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esi + 10h]
        cmp    dword ptr [ecx + eax*4], 0
        jne    SHORT loc_4_2060
        lea    ecx, [eax*2]
        mov    eax, dword ptr [esi + 34h]
        add    ecx, dword ptr [esi + 0ch]
        sub    eax, dword ptr [esp + 14h]
        mov    byte ptr [esp + 13h], 1
        lea    edx, [eax*2 - 2]
        lea    ebx, [ecx + 2]
        push   edx
        push   ebx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2023:
        mov    ecx, dword ptr [esp + 14h]
        mov    eax, dword ptr [esi + 34h]
        shl    ecx, 2
        sub    eax, dword ptr [esp + 14h]
        add    ecx, dword ptr [esi + 10h]
        lea    edx, [eax*4 - 4]
        push   edx
        lea    ebx, [ecx + 4]
        push   ebx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2046:
        dec    dword ptr [esi + 34h]
        jmp    SHORT loc_4_2064
loc_4_204B:
        push   esi
        call   sub_4_1E50
        mov    eax, 0c0010009h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret    18h
loc_4_2060:
        inc    dword ptr [esp + 14h]
loc_4_2064:
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [esi + 34h], eax
        ja     SHORT loc_4_1FEE
loc_4_206D:
        cmp    byte ptr [esp + 13h], 0
        je     SHORT loc_4_20B4
        mov    edx, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi + 30h]
        mov    eax, dword ptr [esp + 34h]
        mov    ebx, dword ptr [esp + 28h]
        push   eax
        mov    ebp, dword ptr [edi]
        push   ebx
        mov    eax, dword ptr [esi + 0ch]
        push   ebp
        mov    ebx, dword ptr [esi + 34h]
        push   eax
        mov    ebp, dword ptr [esi + 50h]
        push   ebx
        mov    eax, dword ptr [esi + 48h]
        push   ebp
        push   eax
        push   edx
        push   ecx
        push   edx
        push   ecx
        call   dword ptr [esi + 1ch]
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_4_20B6
        cmp    edi, 103h
        je     SHORT loc_4_20B6
        push   esi
        call   sub_4_1E50
        jmp    SHORT loc_4_20B6
loc_4_20B4:
        xor    edi, edi
loc_4_20B6:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret    18h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_20D0:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        cmp    byte ptr [esp + 10h], 0
        push   esi
        je     SHORT loc_4_2108
        mov    eax, dword ptr [edx + 60h]
        xor    bl, bl
        mov    ecx, eax
        mov    esi, dword ptr [esp + 10h]
        test   ecx, ecx
        je     SHORT loc_4_20F9
loc_4_20EC:
        cmp    esi, ecx
        jne    SHORT loc_4_20F2
        mov    bl, 1
loc_4_20F2:
        mov    ecx, dword ptr [ecx + 1ch]
        test   ecx, ecx
        jne    SHORT loc_4_20EC
loc_4_20F9:
        test   bl, bl
        jne    SHORT loc_4_2130
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [edx + 60h], esi
        pop    esi
        pop    ebx
        ret    0ch
loc_4_2108:
        add    edx, 60h
        mov    esi, dword ptr [esp + 10h]
        cmp    dword ptr [edx], 0
        je     SHORT loc_4_2129
loc_4_2114:
        mov    eax, dword ptr [edx]
        cmp    esi, eax
        je     SHORT loc_4_2124
        lea    edx, [eax + 1ch]
        cmp    dword ptr [edx], 0
        jne    SHORT loc_4_2114
        jmp    SHORT loc_4_2129
loc_4_2124:
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [edx], eax
loc_4_2129:
        mov    dword ptr [esi + 1ch], 0
loc_4_2130:
        pop    esi
        pop    ebx
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2140:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        cmp    byte ptr [esp + 10h], 0
        push   esi
        je     SHORT loc_4_2178
        mov    ecx, dword ptr [edx + 64h]
        xor    bl, bl
        mov    eax, ecx
        mov    esi, dword ptr [esp + 10h]
        test   eax, eax
        je     SHORT loc_4_2169
loc_4_215C:
        cmp    esi, eax
        jne    SHORT loc_4_2162
        mov    bl, 1
loc_4_2162:
        mov    eax, dword ptr [eax + 20h]
        test   eax, eax
        jne    SHORT loc_4_215C
loc_4_2169:
        test   bl, bl
        jne    SHORT loc_4_21A0
        mov    dword ptr [esi + 20h], ecx
        mov    dword ptr [edx + 64h], esi
        pop    esi
        pop    ebx
        ret    0ch
loc_4_2178:
        add    edx, 64h
        mov    esi, dword ptr [esp + 10h]
        cmp    dword ptr [edx], 0
        je     SHORT loc_4_2199
loc_4_2184:
        mov    eax, dword ptr [edx]
        cmp    esi, eax
        je     SHORT loc_4_2194
        lea    edx, [eax + 20h]
        cmp    dword ptr [edx], 0
        jne    SHORT loc_4_2184
        jmp    SHORT loc_4_2199
loc_4_2194:
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [edx], eax
loc_4_2199:
        mov    dword ptr [esi + 20h], 0
loc_4_21A0:
        pop    esi
        pop    ebx
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_21B0:
        sub    esp, 4
        xor    eax, eax
        xor    cl, cl
        push   ebx
        push   esi
        xor    bl, bl
        mov    esi, dword ptr [esp + 14h]
        push   edi
        mov    byte ptr [esp + 0fh], al
        mov    edx, dword ptr [esi + 18h]
        test   dl, 0a0h
        je     SHORT loc_4_21D7
        mov    eax, 1
        mov    byte ptr [esp + 0fh], al
        jmp    SHORT loc_4_21E8
loc_4_21D7:
        test   dl, 1
        je     SHORT loc_4_21DE
        mov    al, 1
loc_4_21DE:
        test   dl, 4eh
        je     SHORT loc_4_21E8
        mov    byte ptr [esp + 0fh], 1
loc_4_21E8:
        mov    edx, dword ptr [esi + 8]
        test   dl, 0a0h
        je     SHORT loc_4_21F9
        mov    ebx, 1
        mov    cl, bl
        jmp    SHORT loc_4_2207
loc_4_21F9:
        test   dl, 1
        je     SHORT loc_4_2200
        mov    cl, 1
loc_4_2200:
        test   dl, 4eh
        je     SHORT loc_4_2207
        mov    bl, 1
loc_4_2207:
        test   al, al
        jne    SHORT loc_4_2222
        test   cl, cl
        je     SHORT loc_4_221E
        mov    edi, dword ptr [esp + 14h]
        push   1
        push   esi
        push   edi
        call   sub_4_20D0
        jmp    SHORT loc_4_2239
loc_4_221E:
        test   al, al
        je     SHORT loc_4_2235
loc_4_2222:
        test   cl, cl
        jne    SHORT loc_4_2235
        mov    edi, dword ptr [esp + 14h]
        push   0
        push   esi
        push   edi
        call   sub_4_20D0
        jmp    SHORT loc_4_2239
loc_4_2235:
        mov    edi, dword ptr [esp + 14h]
loc_4_2239:
        cmp    byte ptr [esp + 0fh], 0
        jne    SHORT loc_4_225D
        test   bl, bl
        je     SHORT loc_4_2256
        push   1
        push   esi
        push   edi
        call   sub_4_2140
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
loc_4_2256:
        cmp    byte ptr [esp + 0fh], 0
        je     SHORT loc_4_226A
loc_4_225D:
        test   bl, bl
        jne    SHORT loc_4_226A
        push   0
        push   esi
        push   edi
        call   sub_4_2140
loc_4_226A:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2280:
        mov    edx, dword ptr [esp + 8]
        push   edx
        mov    eax, dword ptr [edx + 18h]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [esp + 8]
        push   eax
        mov    ecx, dword ptr [eax + 5ch]
        mov    dword ptr [eax + 14h], ecx
        call   sub_4_21B0
        ret    8
        db  08Bh, 0FFh
sub_4_22A0:
        mov    ecx, dword ptr [esp + 10h]
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [esi + 18h], eax
        mov    dword ptr [esi + 8], ecx
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [edi + 14h], 0
        mov    dword ptr [edi + 5ch], eax
        mov    eax, dword ptr [edi + 18h]
        test   eax, eax
        je     SHORT loc_4_22D9
loc_4_22CC:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [edi + 14h], ecx
        mov    eax, dword ptr [eax + 14h]
        test   eax, eax
        jne    SHORT loc_4_22CC
loc_4_22D9:
        push   esi
        push   edi
        call   sub_4_21B0
        mov    ecx, dword ptr [edi + 14h]
        mov    edx, dword ptr [edi + 5ch]
        mov    eax, edx
        xor    eax, ecx
        test   eax, 0ffffff7fh
        je     SHORT loc_4_232C
        mov    eax, dword ptr [esp + 20h]
        mov    ebx, dword ptr [esp + 18h]
        push   eax
        and    ecx, 0ffffff7fh
        push   ebx
        mov    eax, dword ptr [esi]
        push   eax
        and    edx, 0ffffff7fh
        push   ecx
        push   edx
        call   dword ptr [edi + 20h]
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_4_232E
        cmp    ebx, 103h
        je     SHORT loc_4_232E
        push   esi
        push   edi
        call   sub_4_2280
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    14h
loc_4_232C:
        xor    ebx, ebx
loc_4_232E:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    14h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2340:
        push   esi
        xor    eax, eax
        push   edi
        xor    ecx, ecx
        mov    edx, dword ptr [esp + 10h]
        mov    cl, byte ptr [edx + 11h]
        mov    edx, dword ptr [esp + 0ch]
        mov    edi, dword ptr [edx + 30h]
        test   edi, edi
        je     SHORT loc_4_236D
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_4_2362:
        test   dword ptr [esi], edx
        je     SHORT loc_4_2367
        inc    eax
loc_4_2367:
        add    esi, 4
        dec    edi
        jne    SHORT loc_4_2362
loc_4_236D:
        pop    edi
        pop    esi
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2380:
        push   esi
        xor    eax, eax
        push   edi
        xor    ecx, ecx
        mov    edx, dword ptr [esp + 10h]
        mov    cl, byte ptr [edx + 11h]
        mov    edx, dword ptr [esp + 0ch]
        mov    edi, dword ptr [edx + 34h]
        test   edi, edi
        je     SHORT loc_4_23AD
        mov    esi, dword ptr [edx + 10h]
        mov    edx, 1
        shl    edx, cl
loc_4_23A2:
        test   dword ptr [esi], edx
        je     SHORT loc_4_23A7
        inc    eax
loc_4_23A7:
        add    esi, 4
        dec    edi
        jne    SHORT loc_4_23A2
loc_4_23AD:
        pop    edi
        pop    esi
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_23C0:
        sub    esp, 10h
        xor    ecx, ecx
        mov    eax, dword ptr [esp + 1ch]
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    cl, byte ptr [eax + 11h]
        mov    edi, dword ptr [esp + 24h]
        push   ebp
        mov    dword ptr [esp + 18h], ebx
        mov    dword ptr [esp + 14h], ebx
        cmp    dword ptr [edi + 30h], ebx
        jbe    SHORT loc_4_243A
        mov    esi, dword ptr [esp + 38h]
        mov    eax, 1
        mov    ebp, dword ptr [esp + 30h]
        mov    dword ptr [esp + 10h], ebx
        shl    eax, cl
        mov    dword ptr [esp + 1ch], eax
loc_4_23FA:
        mov    ecx, dword ptr [edi + 8]
        mov    eax, dword ptr [esp + 1ch]
        test   dword ptr [ecx + ebx], eax
        je     SHORT loc_4_2425
        cmp    ebp, 6
        jb     SHORT loc_4_2458
        sub    ebp, 6
        push   6
        mov    eax, dword ptr [edi + 4]
        add    eax, dword ptr [esp + 14h]
        push   eax
        push   esi
        add    esi, 6
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2421:
        inc    dword ptr [esp + 18h]
loc_4_2425:
        add    ebx, 4
        inc    dword ptr [esp + 14h]
        add    dword ptr [esp + 10h], 6
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [edi + 30h], eax
        ja     SHORT loc_4_23FA
loc_4_243A:
        mov    eax, dword ptr [esp + 24h]
        mov    edx, dword ptr [esp + 34h]
        mov    ecx, dword ptr [esp + 18h]
        pop    ebp
        pop    edi
        mov    dword ptr [eax], 0
        pop    esi
        mov    dword ptr [edx], ecx
        pop    ebx
        add    esp, 10h
        ret    18h
loc_4_2458:
        mov    eax, dword ptr [esp + 24h]
        mov    ecx, dword ptr [esp + 34h]
        pop    ebp
        pop    edi
        pop    esi
        mov    dword ptr [eax], 0c0000001h
        pop    ebx
        mov    dword ptr [ecx], 0
        add    esp, 10h
        ret    18h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2480:
        sub    esp, 10h
        xor    ecx, ecx
        mov    eax, dword ptr [esp + 1ch]
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    cl, byte ptr [eax + 11h]
        mov    esi, dword ptr [esp + 24h]
        push   ebp
        mov    dword ptr [esp + 18h], ebx
        mov    dword ptr [esp + 14h], ebx
        cmp    dword ptr [esi + 34h], ebx
        jbe    SHORT loc_4_24FA
        mov    ebp, dword ptr [esp + 38h]
        mov    eax, 1
        mov    edi, dword ptr [esp + 30h]
        mov    dword ptr [esp + 10h], ebx
        shl    eax, cl
        mov    dword ptr [esp + 1ch], eax
loc_4_24BA:
        mov    ecx, dword ptr [esi + 10h]
        mov    eax, dword ptr [esp + 1ch]
        test   dword ptr [ecx + ebx], eax
        je     SHORT loc_4_24E5
        cmp    edi, 2
        jb     SHORT loc_4_2518
        sub    edi, 2
        push   2
        mov    eax, dword ptr [esi + 0ch]
        add    eax, dword ptr [esp + 14h]
        push   eax
        push   ebp
        add    ebp, 2
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_24E1:
        inc    dword ptr [esp + 18h]
loc_4_24E5:
        add    ebx, 4
        inc    dword ptr [esp + 14h]
        add    dword ptr [esp + 10h], 2
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [esi + 34h], eax
        ja     SHORT loc_4_24BA
loc_4_24FA:
        mov    eax, dword ptr [esp + 24h]
        mov    edx, dword ptr [esp + 34h]
        mov    ecx, dword ptr [esp + 18h]
        pop    ebp
        pop    edi
        mov    dword ptr [eax], 0
        pop    esi
        mov    dword ptr [edx], ecx
        pop    ebx
        add    esp, 10h
        ret    18h
loc_4_2518:
        mov    eax, dword ptr [esp + 24h]
        mov    ecx, dword ptr [esp + 34h]
        pop    ebp
        pop    edi
        pop    esi
        mov    dword ptr [eax], 0c0000001h
        pop    ebx
        mov    dword ptr [ecx], 0
        add    esp, 10h
        ret    18h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2540:
        mov    edx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [edx + 30h]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [esp + 0ch]
        jbe    SHORT loc_4_2569
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax], 0c0000001h
        mov    dword ptr [ecx], 0
        ret    14h
loc_4_2569:
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [edx + 30h]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [edx + 30h]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [edx + 4]
        add    eax, eax
        mov    edx, dword ptr [esp + 14h]
        push   eax
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_258F:
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_25A0:
        mov    edx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [edx + 34h]
        add    ecx, ecx
        cmp    ecx, dword ptr [esp + 0ch]
        jbe    SHORT loc_4_25C6
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax], 0c0000001h
        mov    dword ptr [ecx], 0
        ret    14h
loc_4_25C6:
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [edx + 34h]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [edx + 34h]
        add    ecx, ecx
        mov    eax, dword ptr [edx + 0ch]
        mov    edx, dword ptr [esp + 14h]
        push   ecx
        push   eax
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_25E9:
        ret    14h
        db  08Dh, 064h, 024h, 000h
NDIS_Service_15:
        push   ebp
        sub    esp, 20h
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        pushfd
        push   0
        push   24h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        cli
        or     eax, eax
        je     NEAR PTR loc_4_271F
        mov    ebx, eax
        mov    eax, dword ptr [ebp + 40h]
        cmp    eax, 0fh
        ja     NEAR PTR loc_4_2713
        cmp    eax, 2
        jne    SHORT loc_4_2628
        mov    eax, 9
loc_4_2628:
        mov    word ptr [ebp], ax
        mov    ecx, dword ptr [ebp + 2ch]
        mov    word ptr [ecx], 0
        mov    word ptr [ebx], 0
        lea    edi, [ebp + 8]
        xor    eax, eax
        cld
        stosd
        stosd
        stosd
        stosd
        mov    word ptr [ebp + 2], ax
        mov    dword ptr [ebp + 18h], 1f4h
        mov    eax, dword ptr [ebp + 48h]
        test   al, al
        je     SHORT loc_4_2675
        push   ecx
        mov    eax, dword ptr [ebp + 40h]
        cmp    eax, 2
        jne    SHORT loc_4_2663
        mov    eax, 9
loc_4_2663:
        db  0CDh, 020h, 00Bh, 000h, 003h, 000h
        pop    ecx
        jae    SHORT loc_4_2670
        or     word ptr [ebx], 4
loc_4_2670:
        or     word ptr [ebp + 2], 2
loc_4_2675:
        or     word ptr [ebp + 2], 4
        mov    dword ptr [ebp + 1ch], ebx
        db  0C7h, 045h, 004h, 000h, 000h, 000h, 000h
        mov    edi, ebp
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        jb     NEAR PTR loc_4_2713
        mov    dword ptr [ebx + 10h], eax
        mov    dword ptr [ecx + 10h], ebx
        push   ebx
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        pop    ebx
        mov    dword ptr [ebx + 18h], 0
        mov    eax, dword ptr [ebp + 30h]
        mov    dword ptr [ebx + 1ch], eax
        mov    eax, dword ptr [ebp + 4ch]
        mov    dword ptr [ebx + 20h], eax
        mov    dword ptr [ecx + 14h], 0
        mov    dword ptr [ebx + 14h], 0
        mov    eax, dword ptr [ebp + 38h]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [ebp + 34h]
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ebx + 8], eax
        mov    eax, dword ptr [ebp + 3ch]
        mov    dword ptr [ecx + 0ch], eax
        mov    dword ptr [ebx + 0ch], eax
        mov    eax, dword ptr [ebp + 48h]
        test   al, al
        je     SHORT loc_4_26F9
        mov    ax, 2
loc_4_26F9:
        or     word ptr [ecx], ax
        or     word ptr [ebx], ax
        mov    eax, dword ptr [ebp + 28h]
        mov    dword ptr [eax], 0
loc_4_2708:
        popfd
        pop    ebx
        pop    edi
        pop    esi
        add    esp, 20h
        pop    ebp
        ret    28h
loc_4_2713:
        push   0
        push   ebx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_4_271F:
        mov    eax, dword ptr [ebp + 28h]
        mov    dword ptr [eax], 0c0000001h
        jmp    SHORT loc_4_2708
NDIS_Service_16:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        pushfd
        mov    ebx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebx + 10h]
        mov    eax, dword ptr [ebx + 10h]
        cli
        mov    dword ptr [ebx + 8], 0
        mov    dword ptr [ebx + 0ch], 0
        db  0CDh, 020h, 011h, 000h, 003h, 000h
        mov    esi, dword ptr [ebx + 18h]
        db  0CDh, 020h, 012h, 000h, 001h, 000h
        popfd
        push   0
        push   ebx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
NDIS_Service_22:
        jmp    NEAR PTR loc_4_3510
NDIS_Service_23:
        jmp    NEAR PTR loc_4_35B0
NDIS_Service_28:
        jmp    NEAR PTR sub_4_35F0
NDIS_Service_29:
        jmp    NEAR PTR sub_4_3660
NDIS_Service_42:
        jmp    NEAR PTR loc_4_9900
NDIS_Service_44:
        jmp    NEAR PTR sub_4_9CD0
NDIS_Service_47:
        jmp    NEAR PTR loc_4_C920
NDIS_Service_48:
        jmp    NEAR PTR loc_4_C970
NDIS_Service_50:
        jmp    NEAR PTR loc_4_CC70
NDIS_Service_52:
        jmp    NEAR PTR sub_4_3920
NDIS_Service_54:
        jmp    NEAR PTR sub_4_3AE0
NDIS_Service_59:
        jmp    NEAR PTR loc_4_C950
NDIS_Service_60:
        db  0E9h, 000h, 000h, 000h, 000h
NDIS_Service_63:
        jmp    NEAR PTR loc_4_CB20
NDIS_Service_64:
        jmp    NEAR PTR loc_4_CB40
NDIS_Service_72:
        jmp    NEAR PTR sub_4_3690
NDIS_Service_73:
        jmp    NEAR PTR sub_4_3740
NDIS_Service_70:
        db  0E9h, 000h, 000h, 000h, 000h
NDIS_Service_69:
        jmp    NEAR PTR sub_4_6F20
NDIS_Service_71:
        jmp    NEAR PTR loc_4_3B70
NDIS_Service_79:
        jmp    NEAR PTR loc_4_3B40
NDIS_Service_83:
        jmp    NEAR PTR loc_4_CCB0
NDIS_Service_41:
        jmp    NEAR PTR sub_4_9800
NDIS_Service_43:
        jmp    NEAR PTR sub_4_9940
NDIS_Service_49:
        jmp    NEAR PTR sub_4_CC50
NDIS_Service_51:
        jmp    NEAR PTR sub_4_37E0
NDIS_Service_53:
        jmp    NEAR PTR sub_4_3960
NDIS_Service_55:
        jmp    NEAR PTR sub_4_6A10
NDIS_Service_74:
        db  0E9h, 000h, 000h, 000h, 000h
NDIS_Service_05:
        jmp    NEAR PTR loc_4_5B40
NDIS_Service_06:
        jmp    NEAR PTR sub_4_5E40
NDIS_Service_07:
        jmp    NEAR PTR sub_4_5ED0
NDIS_Service_09:
        jmp    NEAR PTR loc_4_3340
NDIS_Service_08:
        jmp    NEAR PTR loc_4_3250
NDIS_Service_67:
        jmp    NEAR PTR loc_4_3B50
NDIS_Service_68:
        jmp    NEAR PTR loc_4_6000
NDIS_Service_81:
        jmp    NEAR PTR sub_4_9DD0
NDIS_Service_82:
        jmp    NEAR PTR loc_4_CC80
NDIS_Service_87:
        jmp    NEAR PTR loc_4_9490
NDIS_Service_88:
        jmp    NEAR PTR loc_4_94A0
NDIS_Service_89:
        jmp    NEAR PTR loc_4_94C0
NDIS_Service_90:
        jmp    NEAR PTR loc_4_30B0
NDIS_Service_91:
        jmp    NEAR PTR loc_4_30F0
NDIS_Service_92:
        jmp    NEAR PTR sub_4_3130
NDIS_Service_93:
        jmp    NEAR PTR sub_4_3160
NDIS_Service_94:
        jmp    NEAR PTR loc_4_3190
NDIS_Service_84:
        jmp    NEAR PTR loc_4_7B40
NDIS_Service_85:
        jmp    NEAR PTR loc_4_CED0
NDIS_Service_86:
        jmp    NEAR PTR loc_4_6900
NDIS_Service_80:
        jmp    NEAR PTR loc_4_CB60
NDIS_Service_65:
        jmp    NEAR PTR sub_4_CC30
sub_4_2869:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, 8
        ja     SHORT loc_4_28A1
        cmp    eax, 1
        jb     SHORT loc_4_28A1
        dec    al
        or     al, 8
        out    96h, al
        mov    dx, 100h
        in     ax, dx
        mov    ebx, dword ptr [ebp + 10h]
        mov    word ptr [ebx], ax
        mov    dx, 102h
        in     eax, dx
        jmp    SHORT loc_4_2893
loc_4_2893:
        mov    dword ptr [ebx + 2], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        jmp    SHORT loc_4_28AA
loc_4_28A1:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0c0000001h
loc_4_28AA:
        sub    al, al
        out    96h, al
        pop    ebx
        pop    ebp
        ret
NDIS_Service_18:
        push   ebp
        mov    ebp, esp
        sub    esp, 100h
        push   edi
        push   esi
        push   ebx
        db  08Dh, 01Dh, 000h, 000h, 000h, 000h
        mov    cx, 14h
loc_4_28C7:
        cmp    dword ptr [ebx], 0
        je     SHORT loc_4_28D6
        add    ebx, 18h
        loop   loc_4_28C7
        jmp    NEAR PTR loc_4_2990
loc_4_28D6:
        mov    eax, dword ptr [ebp + 14h]
        movzx  ecx, word ptr [eax]
        mov    esi, dword ptr [eax + 4]
        lea    edi, [ebp - 100h]
        mov    edx, edi
        cld
        rep movsb
        mov    byte ptr [edi], 0
        inc    edi
        db  0CDh, 020h, 0B8h, 000h, 001h, 000h
        jae    SHORT loc_4_2942
        db  0BEh, 000h, 000h, 000h, 000h
        db  0BFh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 0B3h, 000h, 001h, 000h
        jb     NEAR PTR loc_4_2990
        mov    esi, edx
        lea    edi, [ebp - 100h]
        mov    edx, edi
        cld
loc_4_2917:
        cmp    byte ptr [esi], 0
        je     SHORT loc_4_291F
        movsb
        jmp    SHORT loc_4_2917
loc_4_291F:
        dec    edi
        cmp    byte ptr [edi], 5ch
        je     SHORT loc_4_2929
        inc    edi
        mov    byte ptr [edi], 5ch
loc_4_2929:
        inc    edi
        mov    eax, dword ptr [ebp + 14h]
        movzx  ecx, word ptr [eax]
        mov    esi, dword ptr [eax + 4]
        cld
        rep movsb
        mov    byte ptr [edi], 0
        inc    edi
        db  0CDh, 020h, 0B8h, 000h, 001h, 000h
        jb     SHORT loc_4_2990
loc_4_2942:
        mov    edi, dword ptr [ebp + 0ch]
        mov    dword ptr [edi], ebx
        mov    edi, ebx
        mov    dword ptr [edi], eax
        mov    ebx, eax
        xor    ecx, ecx
        xor    edx, edx
        mov    ax, 4202h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_4_2990
        shl    edx, 10h
        mov    dx, ax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], edx
        mov    dword ptr [edi + 4], edx
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [edi + 8], eax
        xor    ecx, ecx
        xor    edx, edx
        mov    ax, 4200h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_4_2990
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        jmp    SHORT loc_4_2999
loc_4_2990:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0c0000001h
loc_4_2999:
        pop    ebx
        pop    esi
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    14h
NDIS_Service_19:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   esi
        push   edi
        push   ebx
        mov    edi, dword ptr [ebp + 10h]
        cmp    dword ptr [edi + 0ch], 0
        jne    NEAR PTR loc_4_2B67
        mov    eax, dword ptr [edi + 4]
        or     eax, eax
        je     NEAR PTR loc_4_2B67
        cmp    dword ptr [edi + 8], -1
        jne    SHORT loc_4_29F7
        push   0
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_4_2B67
        mov    dword ptr [edi + 0ch], eax
        mov    dword ptr [ebp - 8], eax
        mov    ebx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebx], eax
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        je     NEAR PTR loc_4_2B0D
        jmp    SHORT loc_4_2A43
loc_4_29F7:
        push   8eh
        lea    ecx, [edi + 14h]
        push   ecx
        mov    ebx, dword ptr [edi + 8]
        shr    ebx, 0ch
        push   ebx
        push   0
        push   0
        push   0
        push   1
        dec    eax
        shr    eax, 0ch
        inc    eax
        push   eax
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        mov    ecx, eax
        or     ecx, edx
        je     NEAR PTR loc_4_2B67
        mov    dword ptr [edi + 10h], eax
        mov    dword ptr [edi + 0ch], edx
        mov    dword ptr [ebp - 8], edx
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], edx
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        je     NEAR PTR loc_4_2B0D
loc_4_2A43:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [ebp - 1ch], eax
        push   0
        push   dword ptr [edi + 4]
        db  0CDh, 020h, 0A9h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_4_2B40
        mov    dword ptr [ebp - 4], eax
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    esi, eax
        shr    eax, 4
        mov    word ptr [ebp - 10h], ax
        mov    ax, si
        and    ax, 0fh
        mov    word ptr [ebp - 14h], ax
        mov    edi, dword ptr [ebp + 10h]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebp - 18h], eax
loc_4_2A99:
        mov    ebx, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [ebx + 3ch], ax
        mov    eax, dword ptr [ebp - 14h]
        mov    word ptr [ebx + 14h], ax
        mov    eax, dword ptr [ebp - 18h]
        mov    word ptr [ebx + 10h], ax
        mov    word ptr [ebx + 18h], 0fff0h
        mov    word ptr [ebx + 1ch], 3f00h
        mov    ax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  eax, word ptr [ebx + 1ch]
        mov    dword ptr [ebp - 0ch], eax
        and    word ptr [ebx + 2ch], 1
        jne    SHORT loc_4_2B3A
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2AE3:
        mov    eax, dword ptr [ebp - 0ch]
        add    dword ptr [ebp - 8], eax
        cmp    eax, 0fff0h
        je     SHORT loc_4_2A99
        db  0CDh, 020h, 0AAh, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        jmp    SHORT loc_4_2B2F
loc_4_2B0D:
        mov    ebx, dword ptr [edi]
        mov    ecx, 0fff0h
        mov    edx, dword ptr [ebp - 8]
        mov    ax, 3f00h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_4_2B40
        add    dword ptr [ebp - 8], eax
        cmp    eax, 0fff0h
        je     SHORT loc_4_2B0D
loc_4_2B2F:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        jmp    SHORT loc_4_2B70
loc_4_2B3A:
        db  0CDh, 020h, 0AAh, 000h, 001h, 000h
loc_4_2B40:
        mov    edi, dword ptr [ebp + 10h]
        cmp    dword ptr [edi + 10h], 0
        jne    SHORT loc_4_2B59
        push   0
        push   dword ptr [edi + 0ch]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        jmp    SHORT loc_4_2B67
loc_4_2B59:
        push   0
        push   dword ptr [edi + 10h]
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
loc_4_2B67:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0c0000001h
loc_4_2B70:
        pop    ebx
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
NDIS_Service_20:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [edi]
        xor    ecx, ecx
        xor    edx, edx
        mov    ax, 4200h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_4_2BDC
        cmp    dword ptr [edi + 0ch], 0
        je     SHORT loc_4_2BDC
        cmp    dword ptr [edi + 10h], 0
        jne    SHORT loc_4_2BB5
        push   0
        push   dword ptr [edi + 0ch]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_2BDC
        jmp    SHORT loc_4_2BC7
loc_4_2BB5:
        push   0
        push   dword ptr [edi + 10h]
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_2BDC
loc_4_2BC7:
        mov    dword ptr [edi + 0ch], 0
        mov    dword ptr [edi + 10h], 0
        mov    dword ptr [edi + 14h], 0
loc_4_2BDC:
        pop    ebx
        pop    edi
        pop    ebp
        ret    4
NDIS_Service_21:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        cmp    dword ptr [edi + 0ch], 0
        je     SHORT loc_4_2BF6
        push   edi
        call   NDIS_Service_20
loc_4_2BF6:
        mov    ebx, dword ptr [edi]
        mov    ah, 3eh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_4_2C18
        mov    dword ptr [edi], 0
        mov    dword ptr [edi + 4], 0
        mov    dword ptr [edi + 8], 0
loc_4_2C18:
        pop    ebx
        pop    edi
        pop    ebp
        ret    4
sub_4_2C1E:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        je     NEAR PTR loc_4_2CE9
        push   0
        push   80h
        db  0CDh, 020h, 0A9h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_4_2D31
        mov    edi, eax
        mov    edx, eax
        db  0BEh, 000h, 000h, 000h, 000h
loc_4_2C52:
        lodsb
        stosb
        or     al, al
        je     SHORT loc_4_2C5A
        jmp    SHORT loc_4_2C52
loc_4_2C5A:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    eax, edx
        shr    eax, 4
        mov    word ptr [ebx + 3ch], ax
        mov    ax, dx
        and    ax, 0fh
        mov    word ptr [ebx + 4], ax
        mov    word ptr [ebx + 10h], 4022h
        mov    byte ptr [ebx + 1dh], 6ch
        cmp    dword ptr [ebp + 8], 1
        je     SHORT loc_4_2CA4
        mov    word ptr [ebx + 14h], 1
        jmp    SHORT loc_4_2CB0
loc_4_2CA4:
        mov    word ptr [ebx + 14h], 12h
        mov    word ptr [ebx + 18h], 0
loc_4_2CB0:
        mov    ax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    di, word ptr [ebx + 2ch]
        movzx  ebx, word ptr [ebx + 1ch]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  0CDh, 020h, 0AAh, 000h, 001h, 000h
        or     eax, eax
        je     SHORT loc_4_2D31
        and    di, 1
        jne    SHORT loc_4_2D31
        jmp    SHORT loc_4_2D19
loc_4_2CE9:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ebx, 4022h
        mov    ah, 6ch
        cmp    dword ptr [ebp + 8], 1
        je     SHORT loc_4_2D02
        mov    edx, 1
        jmp    SHORT loc_4_2D0C
loc_4_2D02:
        mov    edx, 12h
        mov    ecx, 0
loc_4_2D0C:
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_4_2D31
        movzx  ebx, ax
loc_4_2D19:
        xor    ecx, ecx
        xor    edx, edx
        mov    eax, 4202h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_4_2D31
        movzx  eax, bx
        jmp    SHORT loc_4_2D33
loc_4_2D31:
        sub    eax, eax
loc_4_2D33:
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_2D38:
        mov    ecx, dword ptr [esp + 8]
        db  00Fh, 0B7h, 014h, 04Dh, 00Ch, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], edx
        ret
sub_4_2D51:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        je     NEAR PTR loc_4_2E12
        push   0
        push   dword ptr [ebp + 14h]
        db  0CDh, 020h, 0A9h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_4_2E3D
        mov    edi, eax
        mov    esi, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 14h]
        mov    edx, ecx
        and    edx, 3
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        rep movsb
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    edx, eax
        shr    eax, 4
        mov    word ptr [ebx + 3ch], ax
        mov    ax, dx
        and    ax, 0fh
        mov    word ptr [ebx + 14h], ax
        mov    eax, dword ptr [ebp + 14h]
        mov    word ptr [ebx + 18h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [ebx + 10h], ax
        mov    byte ptr [ebx + 1dh], 40h
        mov    ax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  edx, word ptr [ebx + 1ch]
        mov    di, word ptr [ebx + 2ch]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  0CDh, 020h, 0AAh, 000h, 001h, 000h
        or     eax, eax
        je     SHORT loc_4_2E3D
        and    di, 1
        jne    SHORT loc_4_2E3D
        cmp    edx, dword ptr [ebp + 14h]
        jne    SHORT loc_4_2E3D
        jmp    SHORT loc_4_2E32
loc_4_2E12:
        mov    edx, dword ptr [ebp + 10h]
        mov    eax, 4000h
        mov    ebx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 14h]
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_4_2E3D
        movzx  eax, ax
        cmp    eax, dword ptr [ebp + 14h]
        jne    SHORT loc_4_2E3D
loc_4_2E32:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        jmp    SHORT loc_4_2E46
loc_4_2E3D:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0c0000001h
loc_4_2E46:
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_2E4B:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        mov    ah, 3eh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_4_2E65
        mov    eax, 0
        jmp    SHORT loc_4_2E6A
loc_4_2E65:
        mov    eax, 0c0000001h
loc_4_2E6A:
        pop    ebx
        pop    ebp
        ret
sub_4_2E6D:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        cmp    ecx, 7
        ja     SHORT loc_4_2EDA
        mov    eax, 1ch
        mul    ecx
        db  0BAh, 000h, 000h, 000h, 000h
        add    edx, eax
        cmp    dword ptr [edx], -1
        jne    SHORT loc_4_2EDA
        cmp    dword ptr [edx + 4], 0
        jne    SHORT loc_4_2EA3
        mov    dword ptr [edx], ecx
        mov    eax, ecx
        sub    esi, esi
        db  0CDh, 020h, 001h, 000h, 004h, 000h
        jb     SHORT loc_4_2EDA
        mov    dword ptr [edx + 4], eax
loc_4_2EA3:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], edx
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 4
        jae    SHORT loc_4_2EBF
        mov    dx, 0bh
        or     al, 0c0h
        out    dx, al
        jmp    SHORT loc_4_2EB9
loc_4_2EB9:
        mov    dx, 0ah
        jmp    SHORT loc_4_2ECE
loc_4_2EBF:
        mov    dx, 0d6h
        and    al, 3
        or     al, 0c0h
        out    dx, al
        jmp    SHORT loc_4_2ECA
loc_4_2ECA:
        mov    dx, 0d4h
loc_4_2ECE:
        and    al, 3
        out    dx, al
        jmp    SHORT loc_4_2ED3
loc_4_2ED3:
        mov    eax, 0
        jmp    SHORT loc_4_2EDF
loc_4_2EDA:
        mov    eax, 0c0000001h
loc_4_2EDF:
        pop    esi
        pop    ebp
        ret
sub_4_2EE2:
        push   ebp
        mov    ebp, esp
        pushad
        mov    ecx, dword ptr [ebp + 10h]
        cmp    ecx, 7
        ja     NEAR PTR loc_4_2FE8
        mov    eax, 1ch
        mul    ecx
        db  0BAh, 000h, 000h, 000h, 000h
        add    edx, eax
        cmp    dword ptr [edx], -1
        jne    NEAR PTR loc_4_2FE8
        cmp    dword ptr [edx + 4], 0
        jne    SHORT loc_4_2F2A
        mov    dword ptr [edx], ecx
        mov    eax, ecx
        sub    esi, esi
        db  0CDh, 020h, 001h, 000h, 004h, 000h
        jb     NEAR PTR loc_4_2FE8
        mov    dword ptr [edx + 4], eax
        mov    ebx, dword ptr [ebp + 1ch]
        mov    dword ptr [edx + 8], ebx
loc_4_2F2A:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], edx
        mov    ebx, dword ptr [ebp + 14h]
        xor    al, al
        cmp    byte ptr [ebx], 1
        jne    SHORT loc_4_2F3D
        or     al, 0
        jmp    SHORT loc_4_2F3F
loc_4_2F3D:
        or     al, 40h
loc_4_2F3F:
        cmp    byte ptr [ebx + 1], 1
        jne    SHORT loc_4_2F47
        or     al, 10h
loc_4_2F47:
        mov    byte ptr [edx + 0ch], al
        mov    eax, dword ptr [edx + 8]
        cmp    eax, 1
        je     NEAR PTR loc_4_2FD8
        cmp    eax, 2
        je     SHORT loc_4_2F7D
        cmp    eax, 3
        jne    NEAR PTR loc_4_2FE8
        sub    al, al
        or     al, 4
        mov    ecx, dword ptr [ebx + 3]
        cmp    ecx, 0
        je     SHORT loc_4_2F79
        cmp    ecx, 1
        jne    SHORT loc_4_2FE8
        or     al, 40h
        jmp    SHORT loc_4_2FDA
loc_4_2F79:
        or     al, 0
        jmp    SHORT loc_4_2FDA
loc_4_2F7D:
        sub    al, al
        mov    ecx, dword ptr [ebx + 3]
        cmp    ecx, 0
        je     SHORT loc_4_2F97
        cmp    ecx, 1
        je     SHORT loc_4_2F9D
        cmp    ecx, 2
        jne    SHORT loc_4_2FE8
        mov    cl, 2
        or     al, 8
        jmp    SHORT loc_4_2FA1
loc_4_2F97:
        sub    cl, cl
        or     al, 0
        jmp    SHORT loc_4_2FA1
loc_4_2F9D:
        mov    cl, 3
        or     al, 0ch
loc_4_2FA1:
        push   eax
        mov    eax, dword ptr [edx]
        mov    ebx, dword ptr [edx + 4]
        db  0CDh, 020h, 016h, 000h, 004h, 000h
        pop    eax
        mov    ebx, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebx + 7]
        cmp    ecx, 0
        je     SHORT loc_4_2FCC
        cmp    ecx, 1
        je     SHORT loc_4_2FD0
        cmp    ecx, 2
        je     SHORT loc_4_2FD4
        cmp    ecx, 3
        jne    SHORT loc_4_2FE8
        or     al, 30h
        jmp    SHORT loc_4_2FDA
loc_4_2FCC:
        or     al, 0
        jmp    SHORT loc_4_2FDA
loc_4_2FD0:
        or     al, 10h
        jmp    SHORT loc_4_2FDA
loc_4_2FD4:
        or     al, 20h
        jmp    SHORT loc_4_2FDA
loc_4_2FD8:
        sub    al, al
loc_4_2FDA:
        mov    byte ptr [edx + 10h], al
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        jmp    SHORT loc_4_2FF1
loc_4_2FE8:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0c0000001h
loc_4_2FF1:
        popad
        pop    ebp
        ret
NDIS_Service_78:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx]
        mov    dx, 0ah
        cmp    eax, 4
        jb     SHORT loc_4_3009
        mov    dx, 0d4h
loc_4_3009:
        and    al, 3
        or     al, 4
        out    dx, al
        mov    dword ptr [ecx], 0ffffffffh
        pop    ebp
        ret    4
sub_4_3018:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   ebx
        movzx  eax, word ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 046h, 001h, 001h, 000h
        mov    eax, ecx
        pop    ebx
        pop    esi
        pop    edi
        pop    ebp
        ret
sub_4_3032:
        db  0CDh, 020h, 001h, 080h, 096h, 004h
sub_4_3038:
        db  0CDh, 020h, 001h, 080h, 097h, 004h
        int3
        int3
loc_4_3040:
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_3050:
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 0ch]
        push   eax
        mov    eax, dword ptr [esp + 0ch]
        push   ecx
        mov    ecx, dword ptr [esp + 0ch]
        push   edx
        push   eax
        mov    edx, dword ptr [ecx + 0f8h]
        push   edx
        call   sub_4_3130
        ret    14h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_3080:
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 0ch]
        push   eax
        mov    eax, dword ptr [esp + 0ch]
        push   ecx
        mov    ecx, dword ptr [esp + 0ch]
        push   edx
        push   eax
        mov    edx, dword ptr [ecx + 0f8h]
        push   edx
        call   sub_4_3160
        ret    14h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_30B0:
        mov    eax, dword ptr [esp + 4]
        push   esi
        push   edi
        xor    esi, esi
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [ecx + 4]
        push   edx
        call   sub_4_9580
        test   eax, eax
        je     SHORT loc_4_30E6
        mov    edi, dword ptr [esp + 1ch]
        push   esi
        mov    ecx, dword ptr [esp + 1ch]
        push   edi
        mov    edx, dword ptr [esp + 1ch]
        push   ecx
        push   edx
        mov    eax, dword ptr [eax + 38h]
        push   eax
        call   sub_4_31D0
        test   eax, eax
        jne    SHORT loc_4_30E6
        mov    esi, edi
loc_4_30E6:
        mov    eax, esi
        pop    edi
        pop    esi
        ret    14h
        db  08Dh, 049h, 000h
loc_4_30F0:
        mov    eax, dword ptr [esp + 4]
        push   esi
        push   edi
        xor    esi, esi
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [ecx + 4]
        push   edx
        call   sub_4_9580
        test   eax, eax
        je     SHORT loc_4_3127
        mov    edi, dword ptr [esp + 1ch]
        push   1
        mov    ecx, dword ptr [esp + 1ch]
        push   edi
        mov    edx, dword ptr [esp + 1ch]
        push   ecx
        push   edx
        mov    eax, dword ptr [eax + 38h]
        push   eax
        call   sub_4_31D0
        test   eax, eax
        jne    SHORT loc_4_3127
        mov    esi, edi
loc_4_3127:
        mov    eax, esi
        pop    edi
        pop    esi
        ret    14h
        db  08Bh, 0FFh
sub_4_3130:
        mov    eax, dword ptr [esp + 10h]
        push   esi
        mov    esi, dword ptr [esp + 18h]
        push   edi
        xor    edi, edi
        mov    ecx, dword ptr [esp + 14h]
        mov    edx, dword ptr [esp + 0ch]
        push   edi
        push   esi
        push   eax
        push   ecx
        mov    eax, dword ptr [edx + 38h]
        push   eax
        call   sub_4_31D0
        test   eax, eax
        jne    SHORT loc_4_3157
        mov    edi, esi
loc_4_3157:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    14h
        db  08Bh, 0FFh
sub_4_3160:
        mov    eax, dword ptr [esp + 10h]
        push   esi
        mov    esi, dword ptr [esp + 18h]
        push   edi
        xor    edi, edi
        push   1
        mov    ecx, dword ptr [esp + 18h]
        push   esi
        mov    edx, dword ptr [esp + 14h]
        push   eax
        push   ecx
        mov    eax, dword ptr [edx + 38h]
        push   eax
        call   sub_4_31D0
        test   eax, eax
        jne    SHORT loc_4_3188
        mov    edi, esi
loc_4_3188:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    14h
        db  090h
loc_4_3190:
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [ecx + 4]
        push   edx
        call   sub_4_9580
        test   eax, eax
        je     SHORT loc_4_31AE
        mov    ecx, dword ptr [eax + 70h]
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax], ecx
        jmp    SHORT loc_4_31B8
loc_4_31AE:
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax], 0
loc_4_31B8:
        cmp    dword ptr [eax], 1
        sbb    eax, eax
        and    eax, 0c0000001h
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_31D0:
        sub    esp, 0ch
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [esp], 4
        push   esi
        push   edi
        mov    esi, 0c0000001h
        jne    SHORT loc_4_323D
        lea    eax, [esp + 10h]
        push   0
        lea    ecx, [esp + 0ch]
        push   eax
        lea    edx, [esp + 14h]
        push   ecx
        mov    edi, dword ptr [esp + 24h]
        push   edx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3206:
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_4_323D
        cmp    dword ptr [esp + 0ch], 4
        jne    SHORT loc_4_323D
        mov    eax, dword ptr [esp + 24h]
        push   0
        mov    ecx, dword ptr [esp + 24h]
        push   eax
        mov    edx, dword ptr [esp + 24h]
        push   ecx
        cmp    byte ptr [esp + 34h], 1
        sbb    eax, eax
        push   edx
        inc    eax
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3234:
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_4_323D
        xor    esi, esi
loc_4_323D:
        mov    eax, esi
        pop    edi
        pop    esi
        add    esp, 0ch
        ret    14h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_3250:
        mov    eax, dword ptr [esp + 8]
        sub    esp, 14h
        push   esi
        mov    ecx, dword ptr [eax]
        push   edi
        mov    esi, 13h
        mov    edx, dword ptr [ecx + 4]
        push   edx
        call   sub_4_9580
        test   eax, eax
        je     NEAR PTR loc_4_330B
        mov    edi, dword ptr [eax + 38h]
        push   0
        lea    eax, [esp + 1ch]
        lea    ecx, [esp + 10h]
        lea    edx, [esp + 14h]
        push   eax
        mov    dword ptr [esp + 14h], 4
        push   ecx
        push   edx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3293:
        add    esp, 14h
        mov    esi, eax
        test   esi, esi
        jne    NEAR PTR loc_4_3321
        cmp    dword ptr [esp + 10h], 2
        je     SHORT loc_4_32AE
        mov    esi, 13h
        jmp    SHORT loc_4_330B
loc_4_32AE:
        lea    eax, [esp + 14h]
        push   0
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_32BB:
        add    esp, 0ch
        cmp    eax, 1
        sbb    esi, esi
        inc    esi
        jne    SHORT loc_4_330B
        mov    eax, dword ptr [esp + 14h]
        push   0
        lea    ecx, [esp + 0ch]
        push   eax
        push   edi
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_32D8:
        add    esp, 10h
        cmp    eax, 1
        sbb    esi, esi
        inc    esi
        jne    SHORT loc_4_330B
        mov    eax, dword ptr [esp + 8]
        xor    ecx, ecx
        push   140h
        mov    cl, byte ptr [eax + 9]
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 0ch]
        add    eax, 0bh
        mov    ecx, dword ptr [esp + 30h]
        push   eax
        xor    esi, esi
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_330B:
        test   esi, esi
        jne    SHORT loc_4_3321
        mov    eax, dword ptr [esp + 20h]
        pop    edi
        pop    esi
        mov    dword ptr [eax], 0
        add    esp, 14h
        ret    10h
loc_4_3321:
        mov    eax, dword ptr [esp + 20h]
        pop    edi
        pop    esi
        mov    dword ptr [eax], 0c0000001h
        add    esp, 14h
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_3340:
        mov    eax, dword ptr [esp + 8]
        sub    esp, 108h
        push   ebx
        mov    ecx, dword ptr [eax]
        push   esi
        mov    ebx, 0ffffffffh
        push   edi
        mov    edx, dword ptr [ecx + 4]
        push   ebp
        push   edx
        call   sub_4_9580
        test   eax, eax
        jne    SHORT loc_4_336E
        mov    esi, dword ptr [esp + 11ch]
        jmp    NEAR PTR loc_4_34F1
loc_4_336E:
        mov    esi, dword ptr [eax + 38h]
        mov    edi, 100h
        lea    eax, [esp + 10h]
        push   1
        lea    ecx, [esp + 1ch]
        push   eax
        mov    dword ptr [esp + 18h], edi
        push   ecx
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3395:
        add    esp, 1ch
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        je     SHORT loc_4_33C2
        lea    eax, [esp + 18h]
        push   1
        mov    dword ptr [esp + 14h], edi
        lea    edi, [esp + 14h]
        push   edi
        push   eax
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_33BF:
        add    esp, 1ch
loc_4_33C2:
        test   eax, eax
        jne    SHORT loc_4_33D7
        lea    eax, [esp + 18h]
        push   eax
        call   sub_4_64F0
        mov    ebx, eax
        jmp    NEAR PTR loc_4_34C3
loc_4_33D7:
        lea    eax, [esp + 10h]
        push   0
        lea    ecx, [esp + 1ch]
        push   eax
        mov    dword ptr [esp + 18h], 100h
        push   ecx
        push   3
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_33FA:
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_4_340D
        xor    ebx, ebx
        mov    bl, byte ptr [esp + 18h]
        inc    ebx
        jmp    NEAR PTR loc_4_34C3
loc_4_340D:
        lea    eax, [esp + 10h]
        push   1
        lea    ecx, [esp + 1ch]
        push   eax
        mov    dword ptr [esp + 18h], 100h
        push   ecx
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3430:
        add    esp, 1ch
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_4_343F
        lea    eax, [esp + 18h]
        jmp    SHORT loc_4_346F
loc_4_343F:
        lea    eax, [esp + 10h]
        push   0
        lea    ecx, [esp + 1ch]
        push   eax
        mov    dword ptr [esp + 18h], 100h
        push   ecx
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3462:
        add    esp, 1ch
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_4_34C3
        lea    eax, [esp + 1ch]
loc_4_346F:
        push   eax
        call   sub_4_64F0
        mov    dword ptr [esp + 14h], eax
        test   edi, edi
        jne    SHORT loc_4_34C3
        mov    ebp, 1
        mov    esi, dword ptr [esp + 11ch]
        mov    edi, dword ptr [esp + 128h]
loc_4_3490:
        push   edi
        mov    dword ptr [edi], 0
        mov    word ptr [edi + 4], 0
        push   ebp
        push   esi
        call   sub_4_2869
        add    esp, 0ch
        cmp    dword ptr [esi], 0
        jne    SHORT loc_4_34B7
        xor    eax, eax
        mov    ax, word ptr [edi]
        cmp    eax, dword ptr [esp + 14h]
        je     SHORT loc_4_34BF
loc_4_34B7:
        inc    ebp
        cmp    ebp, 8
        jbe    SHORT loc_4_3490
        jmp    SHORT loc_4_34D1
loc_4_34BF:
        mov    ebx, ebp
        jmp    SHORT loc_4_34D1
loc_4_34C3:
        mov    esi, dword ptr [esp + 11ch]
        mov    edi, dword ptr [esp + 128h]
loc_4_34D1:
        cmp    ebx, 1
        jb     SHORT loc_4_34F1
        cmp    ebx, 8
        ja     SHORT loc_4_34F1
        push   edi
        push   ebx
        push   esi
        call   sub_4_2869
        mov    eax, dword ptr [esp + 130h]
        add    esp, 0ch
        mov    dword ptr [eax], ebx
        jmp    SHORT loc_4_34F7
loc_4_34F1:
        mov    dword ptr [esi], 0c0000001h
loc_4_34F7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 108h
        ret    10h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_3510:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 20h]
        mov    esi, dword ptr [esp + 1ch]
        add    ebp, 7
        push   0
        and    ebp, 0fffffff8h
        add    ebp, 6fh
        and    ebp, 0fffffff8h
        mov    ebx, ebp
        imul   ebx, esi
        lea    eax, [ebx + 11h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3538:
        add    esp, 8
        mov    edx, eax
        test   edx, edx
        jne    SHORT loc_4_355C
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 18h]
        pop    ebp
        pop    edi
        pop    esi
        mov    dword ptr [eax], 0c000009ah
        pop    ebx
        mov    dword ptr [ecx], 0
        ret    10h
loc_4_355C:
        mov    edi, edx
        xor    eax, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd
        mov    ecx, ebx
        and    ecx, 3
        rep stosb
        lea    eax, [edx + 10h]
        mov    dword ptr [edx + 0ch], ebp
        mov    dword ptr [edx + 8], eax
        cmp    esi, 1
        jbe    SHORT loc_4_3588
        dec    esi
loc_4_357D:
        lea    ecx, [eax + ebp]
        dec    esi
        mov    dword ptr [eax + 8], ecx
        mov    eax, ecx
        jne    SHORT loc_4_357D
loc_4_3588:
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 18h]
        pop    ebp
        pop    edi
        pop    esi
        mov    dword ptr [eax], 0
        pop    ebx
        mov    dword ptr [ecx], edx
        ret    10h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_35B0:
        push   esi
        xor    esi, esi
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_35E0
        mov    eax, dword ptr [esp + 8]
        cmp    eax, esi
        jne    SHORT loc_4_35CE
        mov    esi, 0c0000001h
        mov    eax, esi
        pop    esi
        ret    4
loc_4_35CE:
        mov    dword ptr [eax + 0ch], 0
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_35DD:
        add    esp, 8
loc_4_35E0:
        mov    eax, esi
        pop    esi
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_35F0:
        push   esi
        push   0
        mov    esi, dword ptr [esp + 14h]
        lea    eax, [esi*4]
        lea    ecx, [eax + eax*4 + 11h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3608:
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_4_3627
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0ch]
        pop    esi
        mov    dword ptr [ecx], 0c0000001h
        mov    dword ptr [eax], 0
        ret    0ch
loc_4_3627:
        lea    edx, [eax + 10h]
        cmp    esi, 1
        mov    dword ptr [eax + 0ch], 14h
        mov    dword ptr [eax + 8], edx
        jbe    SHORT loc_4_3644
        dec    esi
loc_4_363A:
        lea    ecx, [edx + 14h]
        dec    esi
        mov    dword ptr [edx], ecx
        mov    edx, ecx
        jne    SHORT loc_4_363A
loc_4_3644:
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [edx], 0
        mov    edx, dword ptr [esp + 0ch]
        pop    esi
        mov    dword ptr [ecx], 0
        mov    dword ptr [edx], eax
        ret    0ch
        db  08Bh, 0FFh
sub_4_3660:
        push   esi
        xor    esi, esi
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_3689
        mov    eax, dword ptr [esp + 8]
        cmp    eax, esi
        jne    SHORT loc_4_367E
        mov    esi, 0c0000001h
        mov    eax, esi
        pop    esi
        ret    4
loc_4_367E:
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3686:
        add    esp, 8
loc_4_3689:
        mov    eax, esi
        pop    esi
        ret    4
        db  090h
sub_4_3690:
        sub    esp, 18h
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 28h]
        push   edi
        mov    edi, dword ptr [esp + 34h]
        push   ebp
        mov    ebp, esi
        mov    ecx, dword ptr [esp + 3ch]
        and    ebp, 0fffff1ffh
        push   0
        shr    ebp, 9
        mov    dword ptr [edi], 0
        add    ebp, 10h
        mov    dword ptr [ecx], 0
        push   ebp
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_36C5:
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_4_372F
        cmp    byte ptr [esp + 34h], 1
        sbb    eax, eax
        push   0ffffffh
        and    eax, 2
        or     eax, 1
        push   eax
        push   esi
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_36E8:
        mov    eax, dword ptr [edi]
        test   eax, eax
        je     SHORT loc_4_3726
        mov    dword ptr [esp + 14h], eax
        mov    dword ptr [esp + 1ch], esi
        lea    eax, [esp + 24h]
        lea    ecx, [esp + 10h]
        mov    dword ptr [esp + 10h], 0
        mov    dword ptr [esp + 18h], 0
        mov    dword ptr [esp + 20h], 4655424eh
        push   eax
        push   ebx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_371E:
        mov    edx, dword ptr [ebx]
        mov    ecx, dword ptr [esp + 3ch]
        mov    dword ptr [ecx], edx
loc_4_3726:
        push   0
        push   ebp
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_372F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3740:
        cmp    byte ptr [esp + 0ch], 1
        sbb    eax, eax
        mov    ecx, dword ptr [esp + 8]
        and    eax, 2
        mov    edx, dword ptr [esp + 10h]
        or     eax, 1
        push   eax
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_375D:
        ret    14h
loc_4_3760:
        mov    ecx, dword ptr [esp + 10h]
        sub    esp, 88h
        mov    ax, word ptr [ecx]
        xor    edx, edx
        cmp    ax, dx
        push   ebx
        je     SHORT loc_4_3787
        movzx  eax, ax
        mov    ecx, dword ptr [ecx + 4]
loc_4_377B:
        mov    bl, byte ptr [ecx + edx*2]
        inc    edx
        cmp    edx, eax
        mov    byte ptr [esp + edx + 0bh], bl
        jl     SHORT loc_4_377B
loc_4_3787:
        lea    eax, [esp + 0ch]
        lea    ecx, [esp + 4]
        mov    byte ptr [esp + edx + 0ch], 0
        push   eax
        push   ecx
        call   sub_4_7490
        mov    ecx, dword ptr [esp + 0a0h]
        lea    edx, [esp + 4]
        mov    eax, dword ptr [esp + 98h]
        push   ecx
        mov    ecx, dword ptr [esp + 98h]
        push   edx
        mov    edx, dword ptr [esp + 98h]
        push   eax
        push   ecx
        push   edx
        call   NDIS_Service_18
        pop    ebx
        add    esp, 88h
        ret    18h
        db  090h
loc_4_37D0:
        mov    eax, 0c00000bbh
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_37E0:
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 1ch]
        push   edi
        push   ebp
        test   ebx, ebx
        jne    SHORT loc_4_37FD
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0c0000001h
        ret    18h
loc_4_37FD:
        movzx  ax, byte ptr [ebx]
        shl    ax, 8
        movzx  cx, byte ptr [ebx + 1]
        add    ax, cx
        cmp    ax, 400h
        jbe    SHORT loc_4_3824
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0c0010004h
        ret    18h
loc_4_3824:
        cmp    dword ptr [esp + 28h], 36h
        jae    SHORT loc_4_383C
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0c0010005h
        ret    18h
loc_4_383C:
        mov    ebp, dword ptr [esp + 28h]
        push   0
        add    ebp, 36h
        push   ebp
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_384B:
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_4_3865
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0c000009ah
        ret    18h
loc_4_3865:
        mov    edi, esi
        xor    eax, eax
        mov    ecx, ebp
        shr    ecx, 2
        rep stosd
        mov    ecx, ebp
        and    ecx, 3
        rep stosb
        mov    eax, dword ptr [esp + 28h]
        lea    ecx, [esi + 36h]
        push   eax
        mov    dword ptr [esi + 14h], ebp
        push   ebx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3889:
        mov    ecx, dword ptr [ebx + 32h]
        lea    edx, [esi + 64h]
        push   ecx
        push   edx
        call   sub_4_CD10
        test   eax, eax
        je     SHORT loc_4_38B6
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_38A2:
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 8
        pop    ebp
        mov    dword ptr [eax], 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        ret    18h
loc_4_38B6:
        mov    eax, dword ptr [esp + 1ch]
        lea    edi, [esi + 0ch]
        mov    dword ptr [esi + 8], 0
        push   edi
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi], 43414d4eh
        call   sub_4_6B00
        mov    ecx, dword ptr [esp + 20h]
        mov    eax, dword ptr [esp + 18h]
        push   esi
        mov    dword ptr [esi + 18h], ecx
        mov    dword ptr [eax], esi
        call   sub_4_6CF0
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [ecx], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_38FF
        push   edi
        call   sub_4_6AC0
loc_4_38FF:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_390C:
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    18h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3920:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        test   ecx, ecx
        mov    dword ptr [eax], 0
        jne    SHORT loc_4_393B
        mov    dword ptr [eax], 0c0000001h
        ret    8
loc_4_393B:
        cmp    dword ptr [ecx + 8], 0
        je     SHORT loc_4_394A
        mov    dword ptr [eax], 10003h
        ret    8
loc_4_394A:
        push   ecx
        call   sub_4_6D10
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3960:
        cmp    dword ptr [esp + 8], 0
        push   ebx
        push   esi
        push   edi
        push   ebp
        je     NEAR PTR loc_4_3ACD
        cmp    dword ptr [esp + 1ch], 0
        je     NEAR PTR loc_4_3ACD
        mov    eax, dword ptr [esp + 24h]
        cmp    word ptr [eax], 0
        je     NEAR PTR loc_4_3ACD
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   sub_4_9580
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_4_39A5
        mov    ebx, 0c0000001h
        pop    ebp
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    18h
loc_4_39A5:
        mov    edi, dword ptr [esp + 28h]
        test   edi, edi
        je     SHORT loc_4_39E4
        mov    ebx, 26h
        mov    ecx, dword ptr [edi + 16h]
        cmp    ecx, 1
        jbe    SHORT loc_4_39C4
        lea    eax, [ecx + ecx*2]
        lea    ebx, [eax*4 + 1ah]
loc_4_39C4:
        xor    edx, edx
        test   ecx, ecx
        je     SHORT loc_4_39E6
        lea    ecx, [edi + 22h]
loc_4_39CD:
        cmp    dword ptr [ecx], 0
        je     SHORT loc_4_39D9
        mov    ebp, dword ptr [ecx - 8]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [eax], ebp
loc_4_39D9:
        add    ecx, 0ch
        inc    edx
        cmp    dword ptr [edi + 16h], edx
        ja     SHORT loc_4_39CD
        jmp    SHORT loc_4_39E6
loc_4_39E4:
        xor    ebx, ebx
loc_4_39E6:
        push   0
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_39EE:
        add    esp, 8
        mov    dword ptr [esi + 34h], eax
        test   eax, eax
        jne    SHORT loc_4_3A06
        mov    ebx, 0c000009ah
        pop    ebp
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    18h
loc_4_3A06:
        test   ebx, ebx
        je     SHORT loc_4_3A12
        push   ebx
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3A12:
        mov    eax, dword ptr [esp + 18h]
        add    eax, 0ch
        push   eax
        call   sub_4_6AC0
        mov    ecx, dword ptr [esp + 18h]
        mov    edx, dword ptr [esp + 1ch]
        mov    dword ptr [esi + 8], ecx
        test   edi, edi
        mov    dword ptr [esi + 0ch], edx
        mov    dword ptr [esi + 18h], 0
        mov    dword ptr [esi], 5044414eh
        je     SHORT loc_4_3A9B
        mov    ecx, dword ptr [edi + 6]
        test   ecx, ecx
        je     SHORT loc_4_3A52
        mov    eax, dword ptr [edi]
        push   eax
        push   edx
        push   esi
        call   ecx
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_4_3A7A
loc_4_3A52:
        test   edi, edi
        je     SHORT loc_4_3A9B
        cmp    byte ptr [edi + 4], 1
        jne    SHORT loc_4_3A9B
        cmp    dword ptr [edi + 0ah], 1
        jne    SHORT loc_4_3A9B
        lea    eax, [esi + 30h]
        mov    ecx, dword ptr [edi]
        push   eax
        push   ecx
        call   sub_4_2E6D
        add    esp, 8
        test   eax, eax
        je     SHORT loc_4_3A9B
        mov    ebx, 0c000009ah
loc_4_3A7A:
        mov    eax, dword ptr [esp + 18h]
        push   eax
        call   sub_4_6E30
        push   0
        mov    eax, dword ptr [esi + 34h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3A8F:
        add    esp, 8
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    18h
loc_4_3A9B:
        mov    eax, dword ptr [esp + 18h]
        push   eax
        push   esi
        call   sub_4_6C30
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax], esi
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_3ABE
        lea    eax, [esi + 20h]
        push   eax
        call   sub_4_6AC0
loc_4_3ABE:
        xor    ebx, ebx
        pop    ebp
        mov    eax, ebx
        pop    edi
        or     dword ptr [esi + 40h], 2
        pop    esi
        pop    ebx
        ret    18h
loc_4_3ACD:
        mov    ebx, 0c0000001h
        pop    ebp
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    18h
        db  005h, 000h, 000h, 000h, 000h
sub_4_3AE0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        test   esi, esi
        jne    SHORT loc_4_3AF2
        mov    eax, 0c0000001h
        pop    esi
        ret    4
loc_4_3AF2:
        cmp    dword ptr [esi + 18h], 0
        je     SHORT loc_4_3B01
        mov    eax, 10003h
        pop    esi
        ret    4
loc_4_3B01:
        mov    eax, dword ptr [esi + 30h]
        test   eax, eax
        je     SHORT loc_4_3B0E
        push   eax
        call   NDIS_Service_78
loc_4_3B0E:
        mov    eax, dword ptr [esi + 8]
        push   eax
        push   esi
        call   sub_4_6C60
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_4_6E30
        push   0
        mov    eax, dword ptr [esi + 34h]
        and    dword ptr [esi + 40h], 0fffffffdh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3B30:
        add    esp, 8
        xor    eax, eax
        pop    esi
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_3B40:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_3B50:
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    dword ptr [ecx + 2ch], edx
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_3B70:
        mov    eax, dword ptr [esp + 4]
        xor    ecx, ecx
        mov    dword ptr [eax + 28h], ecx
        mov    dword ptr [eax + 2ch], ecx
        ret    4
        db  090h
loc_4_3B80:
        xor    eax, eax
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_3B90:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_3BA0:
        mov    ecx, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], ecx
        xor    eax, eax
        ret    10h
        db  090h
loc_4_3BB0:
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3BC0:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        sub    esp, 18h
        cmp    byte ptr [esp + 24h], 0
        push   esi
        mov    esi, dword ptr [ecx + 0f8h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [ecx + 108h], eax
        je     SHORT loc_4_3C20
        cmp    eax, 1
        jne    SHORT loc_4_3C20
        lea    eax, [esp + 8]
        lea    ecx, [esp + 4]
        lea    edx, [esp + 0ch]
        push   eax
        push   ecx
        push   10h
        push   edx
        push   3
        push   0
        push   esi
        call   sub_4_6630
        cmp    dword ptr [esp + 4], 0
        je     SHORT loc_4_3C20
        add    esi, 30h
        xor    eax, eax
        mov    al, byte ptr [esp + 0dh]
        push   esi
        push   eax
        call   sub_4_2E6D
        add    esp, 8
loc_4_3C20:
        pop    esi
        add    esp, 18h
        ret    10h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_3C30:
        mov    edx, dword ptr [esp + 8]
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   edi
        mov    ecx, dword ptr [esp + 1ch]
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esi + 0ch], edx
        test   eax, eax
        mov    dword ptr [esi + 108h], ecx
        jne    SHORT loc_4_3C54
        mov    eax, 2
loc_4_3C54:
        lea    eax, [eax + eax*4]
        lea    edi, [eax + eax*4]
        lea    eax, [edi + edi*4]
        mov    edi, dword ptr [esp + 18h]
        shl    eax, 3
        test   edi, 8
        mov    dword ptr [esi + 184h], eax
        je     SHORT loc_4_3C80
        or     dword ptr [esi + 3ch], 8
        push   ecx
        push   1
        push   edx
        push   esi
        call   sub_4_3BC0
loc_4_3C80:
        test   edi, 1
        je     SHORT loc_4_3C8F
        or     dword ptr [esi + 3ch], 4000h
loc_4_3C8F:
        test   edi, 2
        je     SHORT loc_4_3C9E
        or     dword ptr [esi + 3ch], 8000h
loc_4_3C9E:
        test   edi, 4
        je     SHORT loc_4_3CAD
        or     dword ptr [esi + 3ch], 10000h
loc_4_3CAD:
        pop    edi
        pop    esi
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3CC0:
        sub    esp, 3ch
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 48h]
        push   edi
        push   ebp
        test   esi, esi
        jne    SHORT loc_4_3CDE
        mov    eax, 0c0000001h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret    0ch
loc_4_3CDE:
        mov    ebp, dword ptr [esp + 54h]
        mov    cl, byte ptr [ebp]
        cmp    cl, 4
        je     SHORT loc_4_3CF3
        cmp    cl, 3
        jne    NEAR PTR loc_4_3E6E
loc_4_3CF3:
        cmp    byte ptr [ebp + 1], 0
        jne    NEAR PTR loc_4_3E6E
        mov    eax, dword ptr [esp + 58h]
        cmp    cl, 4
        jne    SHORT loc_4_3D0B
        cmp    eax, 48h
        jb     SHORT loc_4_3D15
loc_4_3D0B:
        cmp    cl, 3
        jne    SHORT loc_4_3D24
        cmp    eax, 3ch
        jae    SHORT loc_4_3D24
loc_4_3D15:
        mov    eax, 0c0010005h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret    0ch
loc_4_3D24:
        push   0
        push   90h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3D30:
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_4_3D48
        mov    eax, 0c000009ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret    0ch
loc_4_3D48:
        lea    edi, [esp + 14h]
        xor    eax, eax
        mov    ecx, 0dh
        rep stosd
        stosw
        mov    edi, ebx
        mov    ecx, 24h
        rep stosd
        mov    al, byte ptr [ebp]
        mov    cl, byte ptr [ebp + 1]
        mov    byte ptr [esp + 14h], al
        mov    byte ptr [esp + 15h], cl
        mov    dword ptr [esp + 1ah], 0
        db  0C7h, 044h, 024h, 01Eh, 000h, 000h, 000h, 000h
        db  0C7h, 044h, 024h, 022h, 000h, 000h, 000h, 000h
        db  0C7h, 044h, 024h, 026h, 000h, 000h, 000h, 000h
        db  0C7h, 044h, 024h, 02Ah, 000h, 000h, 000h, 000h
        db  0B8h, 000h, 000h, 000h, 000h
        push   esi
        mov    dword ptr [esp + 32h], eax
        mov    dword ptr [esp + 36h], eax
        db  0C7h, 044h, 024h, 03Ah, 000h, 000h, 000h, 000h
        db  0C7h, 044h, 024h, 03Eh, 000h, 000h, 000h, 000h
        db  0C7h, 044h, 024h, 042h, 000h, 000h, 000h, 000h
        lea    edx, [esp + 46h]
        push   edx
        call   sub_4_7490
        mov    dword ptr [ebx + 14h], 90h
        mov    al, byte ptr [ebp]
        sub    al, 4
        lea    ecx, [ebx + 18h]
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0ch
        add    eax, 3ch
        push   eax
        push   ebp
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3DE7:
        mov    dword ptr [ebx + 70h], 0
        lea    ecx, [ebx + 74h]
        mov    dword ptr [ebx], 0
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3DFD:
        mov    byte ptr [ebx + 84h], 0
        mov    dword ptr [ebx + 4], 1
        lea    ecx, [ebx + 8]
        lea    edi, [ebx + 68h]
        push   ecx
        call   sub_4_6B70
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   36h
        lea    eax, [esp + 18h]
        mov    dword ptr [ebx + 6ch], ecx
        push   eax
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        push   edi
        lea    ecx, [esp + 24h]
        push   ecx
        call   sub_4_37E0
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_4_3E55
        mov    eax, dword ptr [edi]
        mov    byte ptr [eax + 34h], 1
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret    0ch
loc_4_3E55:
        push   0
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3E5D:
        mov    eax, dword ptr [esp + 18h]
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret    0ch
loc_4_3E6E:
        mov    eax, 0c0010004h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret    0ch
        db  08Dh, 049h, 000h
loc_4_3E80:
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        sub    esp, 4
        push   eax
        push   ecx
        mov    eax, dword ptr [esp + 10h]
        push   edx
        lea    ecx, [esp + 0ch]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3EA1:
        mov    eax, dword ptr [esp]
        add    esp, 4
        ret    14h
        db  005h, 000h, 000h, 000h, 000h
loc_4_3EB0:
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3EC0:
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 0ch]
        push   eax
        push   ecx
        mov    dword ptr [eax], 0
        mov    ecx, dword ptr [esp + 0ch]
        push   edx
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [esp + 14h]
        push   eax
        push   ecx
        call   sub_4_3690
        ret    14h
        db  08Bh, 0FFh
loc_4_3EF0:
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 0ch]
        push   eax
        mov    eax, dword ptr [esp + 0ch]
        push   ecx
        mov    ecx, dword ptr [esp + 0ch]
        push   edx
        push   eax
        push   ecx
        call   sub_4_3740
        ret    18h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_3F20:
        mov    ecx, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 14h]
        sub    esp, 18h
        push   esi
        mov    esi, dword ptr [esp + 24h]
        mov    dword ptr [esi + 2cch], ecx
        mov    al, byte ptr [edx]
        mov    byte ptr [esp + 8], al
        mov    al, byte ptr [edx + 1]
        mov    byte ptr [esp + 9], al
        mov    al, byte ptr [edx + 2]
        mov    byte ptr [esp + 0ah], al
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esp + 0bh], eax
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [esp + 0fh], eax
        mov    eax, dword ptr [edx + 0ch]
        mov    edx, dword ptr [edx + 10h]
        mov    dword ptr [esp + 13h], eax
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [esp + 17h], edx
        mov    esi, dword ptr [esi + 104h]
        lea    edx, [esp + 8]
        push   esi
        push   eax
        lea    eax, [esp + 0ch]
        push   edx
        push   ecx
        mov    ecx, dword ptr [esp + 30h]
        push   ecx
        push   eax
        call   sub_4_2EE2
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 18h
        pop    esi
        add    esp, 18h
        ret    18h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_3FA0:
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   NDIS_Service_78
        ret    4
        db  08Dh, 049h, 000h
loc_4_3FB0:
        mov    eax, dword ptr [esp + 10h]
        push   esi
        mov    ecx, dword ptr [esp + 10h]
        push   eax
        mov    esi, dword ptr [esp + 0ch]
        push   ecx
        push   0
        push   esi
        call   sub_4_CC50
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [esi], ecx
        pop    esi
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_3FE0:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0f8h]
        cmp    dword ptr [ecx + 28h], 0
        jne    SHORT loc_4_3FFE
        mov    eax, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    dword ptr [ecx + 2ch], edx
loc_4_3FFE:
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_4010:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0f8h]
        cmp    dword ptr [eax + 28h], 0
        je     SHORT loc_4_402E
        mov    dword ptr [eax + 28h], 0
        mov    dword ptr [eax + 2ch], 0
loc_4_402E:
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_4040:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax + 0f8h]
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [edx + 70h]
        mov    dword ptr [eax], ecx
        cmp    ecx, 1
        sbb    eax, eax
        and    eax, 0c0000001h
        ret    0ch
sub_4_4060:
        sub    esp, 8
        xor    eax, eax
        mov    byte ptr [esp + 4], al
        push   ebx
        mov    byte ptr [esp + 7], al
        push   esi
        mov    esi, dword ptr [esp + 34h]
        push   edi
        push   ebp
        lea    ecx, [esi + 20h]
        push   ecx
        call   sub_4_6B30
        test   al, al
        jne    SHORT loc_4_4095
        mov    eax, dword ptr [esp + 1ch]
        mov    ebx, dword ptr [esp + 14h]
        mov    dword ptr [eax], 0c0010002h
        jmp    NEAR PTR loc_4_4394
loc_4_4095:
        mov    byte ptr [esp + 14h], 1
        mov    eax, dword ptr [esi + 8]
        mov    ebp, dword ptr [eax + 70h]
        add    eax, 70h
        test   ebp, ebp
        jne    NEAR PTR loc_4_4162
        mov    ecx, dword ptr [esp + 40h]
        mov    ebp, dword ptr [ecx + 0ch]
        mov    dword ptr [eax], ebp
        test   byte ptr [esi + 1f8h], 4
        je     SHORT loc_4_40C4
        db  0C7h, 045h, 048h, 000h, 000h, 000h, 000h
loc_4_40C4:
        mov    eax, dword ptr [esi + 0fch]
        cmp    eax, 3
        je     SHORT loc_4_4142
        cmp    eax, 7
        je     SHORT loc_4_414B
        test   byte ptr [esi + 3dh], 20h
        je     SHORT loc_4_40E1
        db  0C7h, 045h, 04Ch, 000h, 000h, 000h, 000h
loc_4_40E1:
        test   byte ptr [esi + 1a4h], 2
        je     SHORT loc_4_4116
        test   byte ptr [esi + 3dh], 20h
        je     SHORT loc_4_4103
        db  0C7h, 045h, 044h, 000h, 000h, 000h, 000h
        db  0C7h, 086h, 060h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_4152
loc_4_4103:
        db  0C7h, 045h, 044h, 000h, 000h, 000h, 000h
        db  0C7h, 086h, 060h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_4152
loc_4_4116:
        test   byte ptr [esi + 3dh], 20h
        je     SHORT loc_4_412F
        db  0C7h, 045h, 044h, 000h, 000h, 000h, 000h
        db  0C7h, 086h, 060h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_4152
loc_4_412F:
        db  0C7h, 045h, 044h, 000h, 000h, 000h, 000h
        db  0C7h, 086h, 060h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_4152
loc_4_4142:
        db  0C7h, 045h, 044h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_4152
loc_4_414B:
        db  0C7h, 045h, 044h, 000h, 000h, 000h, 000h
loc_4_4152:
        mov    eax, dword ptr [esi + 8]
        cmp    dword ptr [eax + 54h], 0
        je     SHORT loc_4_4162
        db  0C7h, 045h, 048h, 000h, 000h, 000h, 000h
loc_4_4162:
        push   0
        push   8ch
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_416E:
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_4_4186
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [eax], 0c000009ah
        jmp    NEAR PTR loc_4_4394
loc_4_4186:
        mov    edx, 1
        mov    edi, ebx
        xor    eax, eax
        mov    ecx, 23h
        mov    byte ptr [esp + 13h], dl
        rep stosd
        mov    edi, dword ptr [esp + 28h]
        mov    ecx, dword ptr [esp + 40h]
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [ebx + 4], esi
        mov    dword ptr [ebx + 8], edi
        mov    dword ptr [ebx + 0ch], ecx
        mov    dword ptr [ebx + 10h], eax
        mov    eax, dword ptr [esp + 44h]
        mov    ecx, dword ptr [esi + 0ch]
        mov    dword ptr [ebx + 14h], ecx
        mov    dword ptr [ebx + 1ch], eax
        mov    al, byte ptr [esp + 48h]
        mov    ecx, dword ptr [esi + 1f0h]
        test   al, al
        mov    dword ptr [ebx + 38h], ecx
        mov    dword ptr [ebx + 34h], edx
        je     SHORT loc_4_41DC
        or     dword ptr [ebx + 20h], 2
loc_4_41DC:
        mov    ecx, dword ptr [esi + 8]
        mov    edx, dword ptr [ecx + 48h]
        mov    dword ptr [ebx + 40h], edx
        mov    ecx, dword ptr [esi + 8]
        mov    edx, dword ptr [ecx + 50h]
        mov    dword ptr [ebx + 44h], edx
        mov    ecx, dword ptr [edi + 44h]
        mov    dword ptr [ebx + 48h], ecx
        mov    edx, dword ptr [edi + 48h]
        mov    dword ptr [ebx + 4ch], edx
        mov    ecx, dword ptr [edi + 54h]
        mov    dword ptr [ebx + 50h], ecx
        mov    edx, dword ptr [edi + 58h]
        mov    dword ptr [ebx + 54h], edx
        mov    ecx, dword ptr [esi + 8]
        cmp    dword ptr [ecx + 54h], 0
        jne    SHORT loc_4_4218
        mov    dword ptr [ebx + 58h], 0
        jmp    SHORT loc_4_421E
loc_4_4218:
        mov    ecx, dword ptr [edi + 78h]
        mov    dword ptr [ebx + 58h], ecx
loc_4_421E:
        test   byte ptr [esi + 1a4h], 2
        je     SHORT loc_4_4230
        mov    ecx, dword ptr [esi + 8]
        mov    edx, dword ptr [ecx + 58h]
        mov    dword ptr [ebx + 5ch], edx
loc_4_4230:
        mov    ecx, dword ptr [esp + 40h]
        mov    dword ptr [ecx + 10h], ebx
        cmp    dword ptr [esi + 0fch], 7
        jne    SHORT loc_4_4249
        db  0C7h, 041h, 040h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_4253
loc_4_4249:
        mov    edx, dword ptr [ebp + 48h]
        mov    ecx, dword ptr [esp + 40h]
        mov    dword ptr [ecx + 40h], edx
loc_4_4253:
        mov    edx, dword ptr [ebp + 44h]
        mov    ecx, dword ptr [esp + 40h]
        mov    dword ptr [ecx + 3ch], edx
        test   byte ptr [esi + 1a4h], 2
        je     SHORT loc_4_4282
        test   byte ptr [esi + 3dh], 20h
        je     SHORT loc_4_4275
        db  0C7h, 041h, 058h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_429C
loc_4_4275:
        mov    ecx, dword ptr [esp + 40h]
        db  0C7h, 041h, 058h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_429C
loc_4_4282:
        test   byte ptr [esi + 3dh], 20h
        mov    ecx, dword ptr [esp + 40h]
        je     SHORT loc_4_4295
        db  0C7h, 041h, 058h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_429C
loc_4_4295:
        db  0C7h, 041h, 058h, 000h, 000h, 000h, 000h
loc_4_429C:
        cmp    dword ptr [esi + 0fch], 3
        jne    SHORT loc_4_42B0
        mov    ecx, dword ptr [esp + 40h]
        db  0C7h, 041h, 03Ch, 000h, 000h, 000h, 000h
loc_4_42B0:
        mov    ecx, dword ptr [esi + 8]
        cmp    dword ptr [ecx + 54h], 0
        jne    SHORT loc_4_42C6
        mov    ecx, dword ptr [esp + 40h]
        mov    dword ptr [ecx + 54h], 0
        jmp    SHORT loc_4_42D0
loc_4_42C6:
        mov    edx, dword ptr [edi + 78h]
        mov    ecx, dword ptr [esp + 40h]
        mov    dword ptr [ecx + 54h], edx
loc_4_42D0:
        mov    edx, dword ptr [esp + 24h]
        mov    ecx, dword ptr [esp + 40h]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [esi + 0fch]
        cmp    ecx, 0ah
        ja     SHORT loc_4_4354
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_4_42EC:
        lea    eax, [ebx + 28h]
        mov    ecx, dword ptr [esp + 40h]
        push   eax
        mov    edx, dword ptr [esi + 0dch]
        push   ecx
        push   ebx
        push   edx
        call   sub_4_0FA0
        jmp    SHORT loc_4_4358
loc_4_4304:
        lea    eax, [ebx + 28h]
        mov    ecx, dword ptr [esp + 40h]
        push   eax
        mov    edx, dword ptr [esi + 0e0h]
        push   ecx
        push   ebx
        push   edx
        call   sub_4_1A70
        jmp    SHORT loc_4_4358
loc_4_431C:
        mov    al, 1
        jmp    SHORT loc_4_4358
loc_4_4320:
        test   al, al
        jne    SHORT loc_4_433C
        lea    eax, [ebx + 28h]
        mov    ecx, dword ptr [esp + 40h]
        push   eax
        mov    edx, dword ptr [esi + 0e4h]
        push   ecx
        push   ebx
        push   edx
        call   sub_4_0160
        jmp    SHORT loc_4_4358
loc_4_433C:
        lea    eax, [ebx + 28h]
        mov    ecx, dword ptr [esp + 40h]
        push   eax
        mov    edx, dword ptr [esi + 0d8h]
        push   ecx
        push   ebx
        push   edx
        call   sub_4_0670
        jmp    SHORT loc_4_4358
loc_4_4354:
        mov    al, byte ptr [esp + 14h]
loc_4_4358:
        test   al, al
        jne    SHORT loc_4_4368
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [eax], 0c0010007h
        jmp    SHORT loc_4_4394
loc_4_4368:
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebx + 18h], eax
        mov    dword ptr [esi + 18h], ebx
        cmp    dword ptr [ebx + 18h], 0
        jne    SHORT loc_4_438A
        mov    eax, dword ptr [esi + 184h]
        lea    ecx, [esi + 90h]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_438A:
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [eax], 0
loc_4_4394:
        cmp    dword ptr [eax], 0
        je     SHORT loc_4_43B8
        cmp    byte ptr [esp + 14h], 0
        je     SHORT loc_4_43A6
        push   esi
        call   sub_4_7820
loc_4_43A6:
        cmp    byte ptr [esp + 13h], 0
        je     SHORT loc_4_43B8
        push   0
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_43B5:
        add    esp, 8
loc_4_43B8:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    30h
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_4_43F0:
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [esp + 14h]
        mov    eax, dword ptr [esi + 28h]
        mov    ecx, dword ptr [esi + 10h]
        and    eax, 1
        mov    edx, dword ptr [esi + 0ch]
        push   eax
        push   ecx
        push   edx
        mov    eax, dword ptr [esi + 8]
        push   eax
        mov    ecx, dword ptr [esi + 24h]
        push   ecx
        mov    edx, dword ptr [esi + 20h]
        push   edx
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        mov    ecx, dword ptr [esi + 18h]
        push   ecx
        mov    edx, dword ptr [esi + 14h]
        push   edx
        mov    eax, dword ptr [esi + 4]
        lea    ecx, [esp + 2ch]
        push   eax
        lea    edx, [esp + 2ch]
        push   ecx
        push   edx
        call   sub_4_4060
        cmp    dword ptr [esp + 4], 103h
        je     SHORT loc_4_4459
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        push   eax
        mov    edx, dword ptr [esi + 0ch]
        push   ecx
        push   edx
        call   sub_4_6A10
        push   0
        push   2ch
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4459:
        mov    eax, dword ptr [esp + 4]
        pop    esi
        add    esp, 8
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_4470:
        mov    eax, dword ptr [esp + 0ch]
        push   esi
        mov    ecx, dword ptr [esp + 0ch]
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        push   eax
        push   ecx
        push   esi
        call   sub_4_3CC0
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_4_44B5
        push   esi
        call   sub_4_9670
        test   eax, eax
        je     SHORT loc_4_44A6
        mov    ecx, dword ptr [eax + 18h]
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [eax], ecx
        mov    eax, edi
        pop    edi
        pop    esi
        ret    10h
loc_4_44A6:
        mov    eax, dword ptr [esp + 18h]
        mov    edi, 0c0000001h
        mov    dword ptr [eax], 0
loc_4_44B5:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    10h
        db  08Dh, 064h, 024h, 000h
loc_4_44C0:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 38h
        add    eax, 8
        push   ebx
        mov    dword ptr [esp + 20h], 0
        push   esi
        push   edi
        push   ebp
        push   eax
        call   sub_4_6B30
        test   al, al
        jne    SHORT loc_4_44EF
        mov    eax, 0c0000001h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret    0ch
loc_4_44EF:
        xor    ebx, ebx
        mov    byte ptr [esp + 19h], bl
        mov    byte ptr [esp + 13h], bl
        mov    byte ptr [esp + 18h], bl
        mov    byte ptr [esp + 15h], bl
        mov    eax, dword ptr [esp + 54h]
        mov    byte ptr [esp + 12h], bl
        mov    byte ptr [esp + 17h], bl
        mov    byte ptr [esp + 16h], bl
        mov    ecx, dword ptr [eax + 4]
        mov    byte ptr [esp + 14h], bl
        push   ecx
        call   sub_4_9580
        cmp    eax, ebx
        mov    ebp, eax
        je     NEAR PTR loc_4_5937
        push   ebx
        push   2e0h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4533:
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     NEAR PTR loc_4_593B
        mov    edi, esi
        xor    eax, eax
        mov    byte ptr [esp + 13h], 1
        mov    ecx, 0b8h
        rep stosd
        mov    ecx, dword ptr [esp + 4ch]
        mov    dword ptr [esi + 0f8h], ebp
        mov    eax, dword ptr [ebp + 74h]
        mov    dword ptr [esi + 104h], eax
        mov    eax, dword ptr [esp + 54h]
        mov    dword ptr [esi + 100h], 0ffffffffh
        mov    dword ptr [esi + 4], ebx
        mov    dword ptr [esi + 8], ecx
        lea    ecx, [esi + 10h]
        mov    edx, dword ptr [eax]
        mov    ebp, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [ecx + 4], ebp
        db  0A0h, 000h, 000h, 000h, 000h
        mov    byte ptr [esi + 172h], al
        mov    dword ptr [esi + 1c0h], 0ffffffh
        db  0C7h, 086h, 0E8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 086h, 018h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 086h, 01Ch, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 086h, 020h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 086h, 024h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 086h, 0ECh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 086h, 0F0h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 086h, 0F4h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [esi + 8]
        mov    ecx, dword ptr [edx + 30h]
        mov    dword ptr [esi + 150h], ecx
        mov    edx, dword ptr [esi + 8]
        mov    eax, dword ptr [edx + 24h]
        mov    dword ptr [esi + 154h], eax
        mov    eax, dword ptr [esp + 4ch]
        mov    edx, dword ptr [esi + 8]
        mov    ecx, dword ptr [edx + 28h]
        mov    dword ptr [esi + 158h], ecx
        db  0C7h, 086h, 060h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        cmp    byte ptr [eax + 18h], 4
        jne    SHORT loc_4_4658
        mov    eax, dword ptr [esi + 3ch]
        mov    ecx, dword ptr [esp + 4ch]
        or     eax, 800h
        mov    dword ptr [esi + 3ch], eax
        cmp    dword ptr [ecx + 54h], ebx
        je     SHORT loc_4_463D
        or     eax, 400h
        mov    dword ptr [esi + 3ch], eax
loc_4_463D:
        mov    ecx, dword ptr [esp + 4ch]
        cmp    dword ptr [ecx + 58h], 0
        je     SHORT loc_4_4658
        or     dword ptr [esi + 1a4h], 2
        db  0C7h, 086h, 060h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_4658:
        lea    eax, [esi + 20h]
        lea    edi, [esi + 70h]
        push   eax
        call   sub_4_6B70
        xor    eax, eax
        mov    ecx, 8
        rep stosd
        xor    edi, edi
        xor    ebx, ebx
        db  0C7h, 046h, 07Ch, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 80h], esi
        mov    dword ptr [esi + 184h], 7d0h
        mov    dword ptr [esi + 90h], edi
        db  0C7h, 086h, 094h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 98h], esi
loc_4_469E:
        push   ebx
        push   10h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_46A6:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_4_46C6
        inc    edi
        mov    dword ptr [eax], ebx
        mov    dword ptr [eax + 4], ebx
        cmp    edi, 0ah
        mov    dword ptr [eax + 8], ebx
        mov    dword ptr [eax + 0ch], ebx
        mov    ecx, dword ptr [esi + 6ch]
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi + 6ch], eax
        jb     SHORT loc_4_469E
loc_4_46C6:
        cmp    dword ptr [esi + 6ch], 0
        je     SHORT loc_4_46D1
        mov    byte ptr [esp + 17h], 1
loc_4_46D1:
        test   eax, eax
        je     NEAR PTR loc_4_593B
        lea    ecx, [esi + 138h]
        mov    edi, 6
loc_4_46E4:
        mov    edx, dword ptr [esi + 6ch]
        mov    eax, edx
        test   eax, eax
        je     SHORT loc_4_46F2
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 6ch], eax
loc_4_46F2:
        mov    eax, dword ptr [ecx]
        add    ecx, 4
        dec    edi
        mov    dword ptr [edx], eax
        mov    dword ptr [ecx - 4], edx
        jne    SHORT loc_4_46E4
        mov    ecx, dword ptr [esp + 4ch]
        push   ecx
        push   esi
        call   sub_4_77C0
        test   al, al
        je     NEAR PTR loc_4_593B
        mov    ebx, 1
        push   0
        mov    byte ptr [esp + 19h], bl
        push   48h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4724:
        add    esp, 8
        mov    dword ptr [esi + 128h], eax
        test   eax, eax
        je     NEAR PTR loc_4_593B
        lea    ecx, [esi + 0d8h]
        mov    byte ptr [esp + 16h], bl
        push   ecx
        lea    edx, [esi + 30h]
        push   edx
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [esi + 128h]
        db  0C7h, 040h, 004h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 128h]
        mov    dword ptr [eax + 8], esi
        lea    eax, [esp + 48h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   ebx
        call   sub_4_0490
        test   al, al
        je     NEAR PTR loc_4_5925
        lea    eax, [esi + 0dch]
        lea    ecx, [esi + 30h]
        lea    edx, [esp + 40h]
        push   eax
        push   ecx
        push   edx
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_4_0EE0
        test   al, al
        je     NEAR PTR loc_4_5925
        lea    eax, [esi + 0e0h]
        lea    ecx, [esi + 30h]
        lea    edx, [esp + 1ah]
        push   eax
        lea    eax, [esp + 44h]
        push   ecx
        push   edx
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   ebx
        push   ebx
        call   sub_4_1670
        test   al, al
        je     NEAR PTR loc_4_5925
        lea    eax, [esi + 0e4h]
        lea    ecx, [esi + 30h]
        mov    edx, dword ptr [esp + 40h]
        push   eax
        push   ecx
        push   edx
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   0
        test   al, al
        je     NEAR PTR loc_4_5925
        mov    eax, dword ptr [esi + 3ch]
        mov    ecx, dword ptr [esp + 50h]
        or     eax, 2
        push   ecx
        push   esi
        mov    dword ptr [esi + 3ch], eax
        and    eax, 0fffffffeh
        lea    edx, [esp + 3ch]
        mov    dword ptr [esi + 3ch], eax
        db  0A1h, 000h, 000h, 000h, 000h
        shr    eax, 2
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   eax
        push   ecx
        lea    eax, [esp + 48h]
        push   edx
        mov    ecx, dword ptr [esp + 60h]
        push   eax
        call   dword ptr [ecx + 34h]
        mov    ecx, dword ptr [esi + 3ch]
        and    ecx, 0fffffffdh
        test   cl, 42h
        mov    dword ptr [esi + 3ch], ecx
        jne    SHORT loc_4_4869
        mov    edx, dword ptr [esi + 38h]
        test   edx, edx
        je     SHORT loc_4_4869
        cmp    byte ptr [edx + 4dh], 0
        jne    SHORT loc_4_4869
        cmp    byte ptr [edx + 4ch], 0
        jne    SHORT loc_4_4869
        or     ecx, ebx
        jmp    SHORT loc_4_486C
loc_4_4869:
        and    ecx, 0fffffffeh
loc_4_486C:
        mov    dword ptr [esi + 3ch], ecx
        test   eax, eax
        jne    NEAR PTR loc_4_593B
        mov    eax, dword ptr [esp + 34h]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        lea    edi, [esi + 1fch]
        mov    edx, dword ptr [ecx + eax*4]
        push   edi
        mov    dword ptr [esi + 0fch], edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4896:
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_489C:
        lea    ecx, [esp + 1ch]
        push   1
        push   4
        push   ecx
        push   10105h
        push   1
        push   esi
        call   sub_4_A5E0
        test   eax, eax
        jne    NEAR PTR loc_4_593B
        mov    eax, dword ptr [esi + 0fch]
        cmp    eax, 0ah
        ja     NEAR PTR loc_4_4996
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_48D0:
        mov    eax, dword ptr [esp + 1ch]
        cmp    eax, 200h
        jb     SHORT loc_4_48E0
        mov    eax, 200h
loc_4_48E0:
        mov    dword ptr [esi + 1f4h], eax
        jmp    NEAR PTR loc_4_4996
loc_4_48EB:
        mov    eax, dword ptr [esp + 1ch]
        cmp    eax, 1eeh
        jb     SHORT loc_4_48FB
        mov    eax, 1eeh
loc_4_48FB:
        mov    dword ptr [esi + 1f4h], eax
        jmp    NEAR PTR loc_4_4996
loc_4_4906:
        mov    eax, dword ptr [esp + 1ch]
        cmp    eax, 1feh
        jb     SHORT loc_4_4916
        mov    eax, 1feh
loc_4_4916:
        mov    dword ptr [esi + 1f4h], eax
        jmp    SHORT loc_4_4996
loc_4_491E:
        mov    dword ptr [esi + 1f4h], 5eah
        jmp    SHORT loc_4_4996
loc_4_492A:
        mov    eax, dword ptr [esp + 1ch]
        cmp    eax, 1dch
        jb     SHORT loc_4_493A
        mov    eax, 1dch
loc_4_493A:
        push   0
        mov    dword ptr [esi + 1f4h], eax
        mov    byte ptr [esp + 16h], 0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_494D:
        add    esp, 8
        mov    edi, eax
        mov    dword ptr [esi + 180h], eax
        test   edi, edi
        jne    SHORT loc_4_4963
        mov    byte ptr [esp + 12h], 1
        jmp    SHORT loc_4_4996
loc_4_4963:
        mov    edx, dword ptr [esi + 1f4h]
        xor    eax, eax
        mov    byte ptr [esp + 18h], 1
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        jmp    SHORT loc_4_4996
loc_4_4980:
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [esi + 1f4h], eax
        jmp    SHORT loc_4_4996
loc_4_498C:
        mov    dword ptr [esi + 1f4h], 800h
loc_4_4996:
        cmp    byte ptr [esp + 12h], 0
        jne    NEAR PTR loc_4_593B
        lea    eax, [esp + 1ch]
        push   3
        push   4
        push   eax
        push   10113h
        push   1
        push   esi
        call   sub_4_A5E0
        mov    ebp, eax
        test   ebp, ebp
        jne    NEAR PTR loc_4_593B
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [esi + 1f8h], eax
        and    eax, 0ffffffefh
        mov    dword ptr [esi + 1f8h], eax
        test   byte ptr [esi + 1a4h], 2
        je     SHORT loc_4_4A3E
        lea    eax, [esp + 1ch]
        push   3
        push   4
        push   eax
        push   10115h
        push   1
        push   esi
        call   sub_4_A5E0
        mov    ebp, eax
        test   ebp, ebp
        jne    NEAR PTR loc_4_593B
        mov    eax, dword ptr [esp + 1ch]
        push   0
        mov    dword ptr [esi + 1c8h], eax
        mov    eax, dword ptr [esp + 20h]
        shl    eax, 2
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4A16:
        add    esp, 8
        mov    edi, eax
        mov    dword ptr [esi + 1c4h], eax
        test   edi, edi
        je     NEAR PTR loc_4_593B
        mov    ecx, dword ptr [esi + 1c8h]
        xor    eax, eax
        shl    ecx, 2
        shr    ecx, 2
        rep stosd
        mov    byte ptr [esp + 14h], 1
loc_4_4A3E:
        mov    eax, dword ptr [esi + 0fch]
        cmp    eax, 8
        ja     NEAR PTR loc_4_58D8
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_4A54:
        lea    eax, [esp + 1ch]
        push   5
        push   4
        push   eax
        push   1010104h
        push   1
        push   esi
        call   sub_4_A5E0
        mov    ebp, eax
        test   ebp, ebp
        je     SHORT loc_4_4A7A
        mov    byte ptr [esp + 12h], 1
        jmp    NEAR PTR loc_4_58D8
loc_4_4A7A:
        cmp    dword ptr [esp + 1ch], 20h
        jbe    SHORT loc_4_4A89
        mov    dword ptr [esp + 1ch], 20h
loc_4_4A89:
        mov    eax, dword ptr [esp + 1ch]
        push   7
        lea    ecx, [esp + 44h]
        push   6
        push   ecx
        mov    dword ptr [esi + 1e8h], eax
        push   1010102h
        push   1
        push   esi
        call   sub_4_A5E0
        mov    ebp, eax
        test   ebp, ebp
        je     SHORT loc_4_4AB9
        mov    byte ptr [esp + 12h], 1
        jmp    NEAR PTR loc_4_58D8
loc_4_4AB9:
        cli
        lea    edi, [esi + 2eh]
        cmp    byte ptr [edi], 0
        je     SHORT loc_4_4AC6
        xor    bl, bl
        jmp    SHORT loc_4_4AF8
loc_4_4AC6:
        mov    ebx, 1
        mov    byte ptr [edi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_4AF8
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_4AF8
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 06Dh, 002h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_4_4AF8:
        test   bl, bl
        jne    SHORT loc_4_4B49
loc_4_4AFC:
        mov    byte ptr [esi + 2ch], 1
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4B06:
        add    esp, 4
        cli
        cmp    byte ptr [edi], 0
        je     SHORT loc_4_4B13
        xor    bl, bl
        jmp    SHORT loc_4_4B45
loc_4_4B13:
        mov    ebx, 1
        mov    byte ptr [edi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_4B45
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_4B45
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 06Dh, 002h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_4_4B45:
        test   bl, bl
        je     SHORT loc_4_4AFC
loc_4_4B49:
        sti
        mov    eax, dword ptr [esi + 3ch]
        push   esi
        or     eax, 2
        mov    dword ptr [esi + 3ch], eax
        and    eax, 0fffffffeh
        mov    dword ptr [esi + 3ch], eax
        call   sub_4_A570
        lea    ecx, [esi + 0d8h]
        lea    edx, [esi + 30h]
        lea    eax, [esp + 40h]
        push   ecx
        mov    ecx, dword ptr [esp + 20h]
        push   edx
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_4_0490
        test   al, al
        jne    SHORT loc_4_4BEE
        mov    eax, dword ptr [esi + 0ch]
        mov    ebp, dword ptr [esi + 8]
        push   eax
        call   dword ptr [ebp + 2ch]
        test   bl, bl
        je     SHORT loc_4_4BDF
        mov    byte ptr [edi], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_4BCC
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_4BCC
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 086h, 002h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_4BCC:
        cmp    byte ptr [esi + 2ch], 1
        jne    SHORT loc_4_4BDF
        mov    byte ptr [esi + 2ch], 0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4BDC:
        add    esp, 4
loc_4_4BDF:
        mov    byte ptr [esp + 12h], 1
        mov    ebp, 9
        jmp    NEAR PTR loc_4_58D8
loc_4_4BEE:
        db  0C7h, 086h, 0E8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 3ch]
        and    eax, 0fffffffdh
        mov    dword ptr [esi + 3ch], eax
        test   al, 42h
        jne    SHORT loc_4_4C1D
        mov    ecx, dword ptr [esi + 38h]
        test   ecx, ecx
        je     SHORT loc_4_4C1D
        cmp    byte ptr [ecx + 4dh], 0
        jne    SHORT loc_4_4C1D
        cmp    byte ptr [ecx + 4ch], 0
        jne    SHORT loc_4_4C1D
        or     eax, 1
        jmp    SHORT loc_4_4C20
loc_4_4C1D:
        and    eax, 0fffffffeh
loc_4_4C20:
        mov    dword ptr [esi + 3ch], eax
        test   bl, bl
        je     NEAR PTR loc_4_58D8
        mov    byte ptr [edi], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_4C5D
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_4C5D
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 093h, 002h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_4C5D:
        cmp    byte ptr [esi + 2ch], 1
        jne    NEAR PTR loc_4_58D8
        jmp    NEAR PTR loc_4_58CB
loc_4_4C6C:
        lea    eax, [esp + 40h]
        push   0ah
        push   6
        push   eax
        push   2010102h
        push   1
        push   esi
        call   sub_4_A5E0
        mov    ebp, eax
        test   ebp, ebp
        je     SHORT loc_4_4C92
        mov    byte ptr [esp + 12h], 1
        jmp    NEAR PTR loc_4_58D8
loc_4_4C92:
        cli
        lea    edi, [esi + 2eh]
        cmp    byte ptr [edi], 0
        je     SHORT loc_4_4C9F
        xor    bl, bl
        jmp    SHORT loc_4_4CD1
loc_4_4C9F:
        mov    ebx, 1
        mov    byte ptr [edi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_4CD1
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_4CD1
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0ACh, 002h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_4_4CD1:
        test   bl, bl
        jne    SHORT loc_4_4D22
loc_4_4CD5:
        mov    byte ptr [esi + 2ch], 1
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4CDF:
        add    esp, 4
        cli
        cmp    byte ptr [edi], 0
        je     SHORT loc_4_4CEC
        xor    bl, bl
        jmp    SHORT loc_4_4D1E
loc_4_4CEC:
        mov    ebx, 1
        mov    byte ptr [edi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_4D1E
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_4D1E
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0ACh, 002h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_4_4D1E:
        test   bl, bl
        je     SHORT loc_4_4CD5
loc_4_4D22:
        sti
        mov    eax, dword ptr [esi + 3ch]
        push   esi
        or     eax, 2
        mov    dword ptr [esi + 3ch], eax
        and    eax, 0fffffffeh
        mov    dword ptr [esi + 3ch], eax
        call   sub_4_A570
        lea    ecx, [esi + 0dch]
        lea    edx, [esi + 30h]
        lea    eax, [esp + 40h]
        push   ecx
        push   edx
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_4_0EE0
        test   al, al
        jne    SHORT loc_4_4DC7
        mov    eax, dword ptr [esi + 0ch]
        mov    ebp, dword ptr [esi + 8]
        push   eax
        call   dword ptr [ebp + 2ch]
        test   bl, bl
        je     SHORT loc_4_4DB8
        mov    byte ptr [edi], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_4DA5
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_4DA5
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0C5h, 002h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_4DA5:
        cmp    byte ptr [esi + 2ch], 1
        jne    SHORT loc_4_4DB8
        mov    byte ptr [esi + 2ch], 0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4DB5:
        add    esp, 4
loc_4_4DB8:
        mov    byte ptr [esp + 12h], 1
        mov    ebp, 0ch
        jmp    NEAR PTR loc_4_58D8
loc_4_4DC7:
        db  0C7h, 086h, 0E8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 3ch]
        and    eax, 0fffffffdh
        mov    dword ptr [esi + 3ch], eax
        test   al, 42h
        jne    SHORT loc_4_4DF6
        mov    ecx, dword ptr [esi + 38h]
        test   ecx, ecx
        je     SHORT loc_4_4DF6
        cmp    byte ptr [ecx + 4dh], 0
        jne    SHORT loc_4_4DF6
        cmp    byte ptr [ecx + 4ch], 0
        jne    SHORT loc_4_4DF6
        or     eax, 1
        jmp    SHORT loc_4_4DF9
loc_4_4DF6:
        and    eax, 0fffffffeh
loc_4_4DF9:
        mov    dword ptr [esi + 3ch], eax
        test   bl, bl
        je     NEAR PTR loc_4_58D8
        mov    byte ptr [edi], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_4E36
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_4E36
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0D1h, 002h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_4E36:
        cmp    byte ptr [esi + 2ch], 1
        jne    NEAR PTR loc_4_58D8
        jmp    NEAR PTR loc_4_58CB
loc_4_4E45:
        lea    eax, [esp + 1ch]
        push   0dh
        push   4
        push   eax
        push   3010104h
        push   1
        push   esi
        call   sub_4_A5E0
        mov    ebp, eax
        test   ebp, ebp
        je     SHORT loc_4_4E6B
        mov    byte ptr [esp + 12h], 1
        jmp    NEAR PTR loc_4_58D8
loc_4_4E6B:
        cmp    dword ptr [esp + 1ch], 20h
        jbe    SHORT loc_4_4E7A
        mov    dword ptr [esp + 1ch], 20h
loc_4_4E7A:
        mov    eax, dword ptr [esp + 1ch]
        push   0fh
        lea    ecx, [esp + 2ch]
        push   4
        push   ecx
        mov    dword ptr [esi + 1e8h], eax
        push   3010108h
        push   1
        push   esi
        call   sub_4_A5E0
        mov    ebp, eax
        test   ebp, ebp
        je     SHORT loc_4_4EAA
        mov    byte ptr [esp + 12h], 1
        jmp    NEAR PTR loc_4_58D8
loc_4_4EAA:
        cmp    dword ptr [esp + 28h], 20h
        jbe    SHORT loc_4_4EB9
        mov    dword ptr [esp + 28h], 20h
loc_4_4EB9:
        mov    eax, dword ptr [esp + 28h]
        push   11h
        lea    ecx, [esp + 44h]
        push   6
        push   ecx
        mov    dword ptr [esi + 1ech], eax
        push   3010102h
        push   1
        push   esi
        call   sub_4_A5E0
        mov    ebp, eax
        test   ebp, ebp
        je     SHORT loc_4_4EE9
        mov    byte ptr [esp + 12h], 1
        jmp    NEAR PTR loc_4_58D8
loc_4_4EE9:
        lea    eax, [esp + 1ah]
        push   13h
        push   2
        push   eax
        push   3010106h
        push   1
        push   esi
        call   sub_4_A5E0
        mov    ebp, eax
        test   ebp, ebp
        je     SHORT loc_4_4F0F
        mov    byte ptr [esp + 12h], 1
        jmp    NEAR PTR loc_4_58D8
loc_4_4F0F:
        cli
        lea    edi, [esi + 2eh]
        cmp    byte ptr [edi], 0
        je     SHORT loc_4_4F1C
        xor    bl, bl
        jmp    SHORT loc_4_4F4E
loc_4_4F1C:
        mov    ebx, 1
        mov    byte ptr [edi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_4F4E
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_4F4E
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 02Bh, 003h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_4_4F4E:
        test   bl, bl
        jne    SHORT loc_4_4F9F
loc_4_4F52:
        mov    byte ptr [esi + 2ch], 1
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4F5C:
        add    esp, 4
        cli
        cmp    byte ptr [edi], 0
        je     SHORT loc_4_4F69
        xor    bl, bl
        jmp    SHORT loc_4_4F9B
loc_4_4F69:
        mov    ebx, 1
        mov    byte ptr [edi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_4F9B
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_4F9B
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 02Bh, 003h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_4_4F9B:
        test   bl, bl
        je     SHORT loc_4_4F52
loc_4_4F9F:
        sti
        mov    eax, dword ptr [esi + 3ch]
        push   esi
        or     eax, 2
        mov    dword ptr [esi + 3ch], eax
        and    eax, 0fffffffeh
        mov    dword ptr [esi + 3ch], eax
        call   sub_4_A570
        lea    ecx, [esi + 0e0h]
        lea    edx, [esi + 30h]
        lea    eax, [esp + 1ah]
        push   ecx
        lea    ecx, [esp + 44h]
        push   edx
        mov    edx, dword ptr [esp + 30h]
        push   eax
        mov    eax, dword ptr [esp + 28h]
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        push   eax
        call   sub_4_1670
        test   al, al
        jne    SHORT loc_4_504E
        mov    eax, dword ptr [esi + 0ch]
        mov    ebp, dword ptr [esi + 8]
        push   eax
        call   dword ptr [ebp + 2ch]
        test   bl, bl
        je     SHORT loc_4_503F
        mov    byte ptr [edi], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_502C
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_502C
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 045h, 003h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_502C:
        cmp    byte ptr [esi + 2ch], 1
        jne    SHORT loc_4_503F
        mov    byte ptr [esi + 2ch], 0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_503C:
        add    esp, 4
loc_4_503F:
        mov    byte ptr [esp + 12h], 1
        mov    ebp, 15h
        jmp    NEAR PTR loc_4_58D8
loc_4_504E:
        db  0C7h, 086h, 0E8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 3ch]
        and    eax, 0fffffffdh
        mov    dword ptr [esi + 3ch], eax
        test   al, 42h
        jne    SHORT loc_4_507D
        mov    ecx, dword ptr [esi + 38h]
        test   ecx, ecx
        je     SHORT loc_4_507D
        cmp    byte ptr [ecx + 4dh], 0
        jne    SHORT loc_4_507D
        cmp    byte ptr [ecx + 4ch], 0
        jne    SHORT loc_4_507D
        or     eax, 1
        jmp    SHORT loc_4_5080
loc_4_507D:
        and    eax, 0fffffffeh
loc_4_5080:
        mov    dword ptr [esi + 3ch], eax
        test   bl, bl
        je     NEAR PTR loc_4_58D8
        mov    byte ptr [edi], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_50BD
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_50BD
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 051h, 003h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_50BD:
        cmp    byte ptr [esi + 2ch], 1
        jne    NEAR PTR loc_4_58D8
        jmp    NEAR PTR loc_4_58CB
loc_4_50CC:
        lea    eax, [esp + 40h]
        push   1dh
        push   6
        push   eax
        push   4010102h
        push   1
        push   esi
        call   sub_4_A5E0
        mov    ebp, eax
        test   ebp, ebp
        je     NEAR PTR loc_4_57BE
        mov    byte ptr [esp + 12h], 1
        jmp    NEAR PTR loc_4_58D8
loc_4_50F6:
        push   8
        lea    eax, [esi + 174h]
        lea    ecx, [esp + 34h]
        push   eax
        mov    dword ptr [esi + 1e8h], 20h
        push   ecx
        call   sub_4_35F0
        cmp    dword ptr [esp + 30h], 0
        je     NEAR PTR loc_4_5211
        cli
        lea    edi, [esi + 2eh]
        cmp    byte ptr [edi], 0
        je     SHORT loc_4_512B
        xor    bl, bl
        jmp    SHORT loc_4_515D
loc_4_512B:
        mov    ebx, 1
        mov    byte ptr [edi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_515D
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_515D
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 06Ch, 003h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_4_515D:
        test   bl, bl
        jne    SHORT loc_4_51AE
loc_4_5161:
        mov    byte ptr [esi + 2ch], 1
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_516B:
        add    esp, 4
        cli
        cmp    byte ptr [edi], 0
        je     SHORT loc_4_5178
        xor    bl, bl
        jmp    SHORT loc_4_51AA
loc_4_5178:
        mov    ebx, 1
        mov    byte ptr [edi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_51AA
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_51AA
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 06Ch, 003h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_4_51AA:
        test   bl, bl
        je     SHORT loc_4_5161
loc_4_51AE:
        sti
        mov    eax, dword ptr [esi + 0ch]
        mov    ebp, dword ptr [esi + 8]
        push   eax
        call   dword ptr [ebp + 2ch]
        test   bl, bl
        je     SHORT loc_4_5202
        mov    byte ptr [edi], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_51EF
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_51EF
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 071h, 003h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_51EF:
        cmp    byte ptr [esi + 2ch], 1
        jne    SHORT loc_4_5202
        mov    byte ptr [esi + 2ch], 0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_51FF:
        add    esp, 4
loc_4_5202:
        mov    byte ptr [esp + 12h], 1
        mov    ebp, 16h
        jmp    NEAR PTR loc_4_58D8
loc_4_5211:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        lea    eax, [esp + 20h]
        push   edi
        push   0
        push   80h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_5229:
        cmp    dword ptr [esp + 20h], 0
        jne    NEAR PTR loc_4_5333
        cli
        lea    edi, [esi + 2eh]
        cmp    byte ptr [edi], 0
        je     SHORT loc_4_5241
        xor    bl, bl
        jmp    SHORT loc_4_5273
loc_4_5241:
        mov    ebx, 1
        mov    byte ptr [edi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_5273
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_5273
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 084h, 003h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_4_5273:
        test   bl, bl
        jne    SHORT loc_4_52C4
loc_4_5277:
        mov    byte ptr [esi + 2ch], 1
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_5281:
        add    esp, 4
        cli
        cmp    byte ptr [edi], 0
        je     SHORT loc_4_528E
        xor    bl, bl
        jmp    SHORT loc_4_52C0
loc_4_528E:
        mov    ebx, 1
        mov    byte ptr [edi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_52C0
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_52C0
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 084h, 003h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_4_52C0:
        test   bl, bl
        je     SHORT loc_4_5277
loc_4_52C4:
        sti
        mov    eax, dword ptr [esi + 0ch]
        mov    ebp, dword ptr [esi + 8]
        push   eax
        call   dword ptr [ebp + 2ch]
        test   bl, bl
        je     SHORT loc_4_5318
        mov    byte ptr [edi], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_5305
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_5305
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 089h, 003h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_5305:
        cmp    byte ptr [esi + 2ch], 1
        jne    SHORT loc_4_5318
        mov    byte ptr [esi + 2ch], 0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_5315:
        add    esp, 4
loc_4_5318:
        mov    eax, dword ptr [esi + 174h]
        mov    ebp, 17h
        push   eax
        call   sub_4_3660
        mov    byte ptr [esp + 12h], 1
        jmp    NEAR PTR loc_4_58D8
loc_4_5333:
        lea    eax, [esp + 24h]
        push   edi
        push   0
        push   20h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_5342:
        cmp    dword ptr [esp + 24h], 0
        jne    NEAR PTR loc_4_545D
        cli
        lea    edi, [esi + 2eh]
        cmp    byte ptr [edi], 0
        je     SHORT loc_4_535A
        xor    bl, bl
        jmp    SHORT loc_4_538C
loc_4_535A:
        mov    ebx, 1
        mov    byte ptr [edi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_538C
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_538C
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 09Dh, 003h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_4_538C:
        test   bl, bl
        jne    SHORT loc_4_53DD
loc_4_5390:
        mov    byte ptr [esi + 2ch], 1
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_539A:
        add    esp, 4
        cli
        cmp    byte ptr [edi], 0
        je     SHORT loc_4_53A7
        xor    bl, bl
        jmp    SHORT loc_4_53D9
loc_4_53A7:
        mov    ebx, 1
        mov    byte ptr [edi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_53D9
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_53D9
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 09Dh, 003h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_4_53D9:
        test   bl, bl
        je     SHORT loc_4_5390
loc_4_53DD:
        sti
        mov    eax, dword ptr [esi + 0ch]
        mov    ebp, dword ptr [esi + 8]
        push   eax
        call   dword ptr [ebp + 2ch]
        test   bl, bl
        je     SHORT loc_4_5431
        mov    byte ptr [edi], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_541E
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_541E
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0A2h, 003h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_541E:
        cmp    byte ptr [esi + 2ch], 1
        jne    SHORT loc_4_5431
        mov    byte ptr [esi + 2ch], 0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_542E:
        add    esp, 4
loc_4_5431:
        mov    eax, dword ptr [esp + 20h]
        push   0
        push   80h
        mov    ebp, 18h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_5447:
        mov    eax, dword ptr [esi + 174h]
        push   eax
        call   sub_4_3660
        mov    byte ptr [esp + 12h], 1
        jmp    NEAR PTR loc_4_58D8
loc_4_545D:
        mov    ecx, 8
        mov    edx, 4
        mov    edi, 10h
loc_4_546C:
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [eax + 4], edx
        mov    eax, dword ptr [esp + 20h]
        mov    ebp, dword ptr [eax + 4]
        mov    dword ptr [eax + 8], ebp
        mov    ebp, dword ptr [esp + 24h]
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [eax], ebp
        mov    ebp, dword ptr [esi + 178h]
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [eax + 0ch], ebp
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [esi + 178h], eax
        add    dword ptr [esp + 20h], edi
        add    dword ptr [esp + 24h], edx
        dec    ecx
        jne    SHORT loc_4_546C
        lea    eax, [esp + 45h]
        push   19h
        push   1
        push   eax
        push   6010102h
        push   1
        push   esi
        call   sub_4_A5E0
        mov    ebp, eax
        test   ebp, ebp
        je     SHORT loc_4_54FA
        mov    eax, dword ptr [esp + 20h]
        push   0
        push   80h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_54D6:
        mov    eax, dword ptr [esp + 24h]
        push   0
        push   20h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_54E4:
        mov    eax, dword ptr [esi + 174h]
        push   eax
        call   sub_4_3660
        mov    byte ptr [esp + 12h], 1
        jmp    NEAR PTR loc_4_58D8
loc_4_54FA:
        mov    al, byte ptr [esp + 45h]
        mov    byte ptr [esi + 171h], al
        cli
        lea    edi, [esi + 2eh]
        cmp    byte ptr [edi], 0
        je     SHORT loc_4_5511
        xor    bl, bl
        jmp    SHORT loc_4_5543
loc_4_5511:
        mov    ebx, 1
        mov    byte ptr [edi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_5543
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_5543
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0D9h, 003h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_4_5543:
        test   bl, bl
        jne    SHORT loc_4_5594
loc_4_5547:
        mov    byte ptr [esi + 2ch], 1
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_5551:
        add    esp, 4
        cli
        cmp    byte ptr [edi], 0
        je     SHORT loc_4_555E
        xor    bl, bl
        jmp    SHORT loc_4_5590
loc_4_555E:
        mov    ebx, 1
        mov    byte ptr [edi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_5590
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_5590
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0D9h, 003h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_4_5590:
        test   bl, bl
        je     SHORT loc_4_5547
loc_4_5594:
        sti
        mov    eax, dword ptr [esi + 3ch]
        push   esi
        or     eax, 2
        mov    dword ptr [esi + 3ch], eax
        and    eax, 0fffffffeh
        mov    dword ptr [esi + 3ch], eax
        call   sub_4_A570
        lea    ecx, [esi + 0e4h]
        lea    edx, [esi + 30h]
        mov    eax, dword ptr [esp + 45h]
        push   ecx
        push   edx
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   0
        test   al, al
        jne    NEAR PTR loc_4_565F
        mov    eax, dword ptr [esi + 0ch]
        mov    ebp, dword ptr [esi + 8]
        push   eax
        call   dword ptr [ebp + 2ch]
        test   bl, bl
        je     SHORT loc_4_5625
        mov    byte ptr [edi], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_5612
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_5612
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0F0h, 003h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_5612:
        cmp    byte ptr [esi + 2ch], 1
        jne    SHORT loc_4_5625
        mov    byte ptr [esi + 2ch], 0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_5622:
        add    esp, 4
loc_4_5625:
        mov    eax, dword ptr [esp + 20h]
        push   0
        push   80h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_5636:
        mov    eax, dword ptr [esp + 24h]
        push   0
        push   20h
        mov    ebp, 1bh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_5649:
        mov    eax, dword ptr [esi + 174h]
        push   eax
        call   sub_4_3660
        mov    byte ptr [esp + 12h], 1
        jmp    NEAR PTR loc_4_58D8
loc_4_565F:
        mov    byte ptr [esp + 40h], 0
        mov    byte ptr [esp + 41h], 0
        lea    eax, [esi + 0d8h]
        lea    ecx, [esi + 30h]
        lea    edx, [esp + 40h]
        push   eax
        mov    byte ptr [esp + 46h], 0
        mov    byte ptr [esp + 47h], 0
        mov    byte ptr [esp + 48h], 0
        push   ecx
        push   edx
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   20h
        call   sub_4_0490
        test   al, al
        jne    NEAR PTR loc_4_573F
        mov    eax, dword ptr [esi + 0e4h]
        push   eax
        call   sub_4_00D0
        mov    eax, dword ptr [esi + 0ch]
        mov    ebp, dword ptr [esi + 8]
        push   eax
        call   dword ptr [ebp + 2ch]
        test   bl, bl
        je     SHORT loc_4_5705
        mov    byte ptr [edi], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_56F2
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_56F2
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 01Ch, 004h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_56F2:
        cmp    byte ptr [esi + 2ch], 1
        jne    SHORT loc_4_5705
        mov    byte ptr [esi + 2ch], 0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_5702:
        add    esp, 4
loc_4_5705:
        mov    eax, dword ptr [esp + 20h]
        push   0
        push   80h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_5716:
        mov    eax, dword ptr [esp + 24h]
        push   0
        push   20h
        mov    ebp, 1ch
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_5729:
        mov    eax, dword ptr [esi + 174h]
        push   eax
        call   sub_4_3660
        mov    byte ptr [esp + 12h], 1
        jmp    NEAR PTR loc_4_58D8
loc_4_573F:
        db  0C7h, 086h, 0E8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [esi + 3ch]
        and    ecx, 0fffffffdh
        test   cl, 42h
        mov    dword ptr [esi + 3ch], ecx
        jne    SHORT loc_4_576F
        mov    eax, dword ptr [esi + 38h]
        test   eax, eax
        je     SHORT loc_4_576F
        cmp    byte ptr [eax + 4dh], 0
        jne    SHORT loc_4_576F
        cmp    byte ptr [eax + 4ch], 0
        jne    SHORT loc_4_576F
        or     ecx, 1
        jmp    SHORT loc_4_5772
loc_4_576F:
        and    ecx, 0fffffffeh
loc_4_5772:
        mov    dword ptr [esi + 3ch], ecx
        test   bl, bl
        je     NEAR PTR loc_4_58D8
        mov    byte ptr [edi], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_57AF
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_57AF
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 02Fh, 004h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_57AF:
        cmp    byte ptr [esi + 2ch], 1
        jne    NEAR PTR loc_4_58D8
        jmp    NEAR PTR loc_4_58CB
loc_4_57BE:
        cli
        lea    edi, [esi + 2eh]
        cmp    byte ptr [edi], 0
        je     SHORT loc_4_57CB
        xor    bl, bl
        jmp    SHORT loc_4_57FD
loc_4_57CB:
        mov    ebx, 1
        mov    byte ptr [edi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_57FD
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_57FD
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 04Bh, 004h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_4_57FD:
        test   bl, bl
        jne    SHORT loc_4_584E
loc_4_5801:
        mov    byte ptr [esi + 2ch], 1
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_580B:
        add    esp, 4
        cli
        cmp    byte ptr [edi], 0
        je     SHORT loc_4_5818
        xor    bl, bl
        jmp    SHORT loc_4_584A
loc_4_5818:
        mov    ebx, 1
        mov    byte ptr [edi], bl
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_584A
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_584A
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 04Bh, 004h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
loc_4_584A:
        test   bl, bl
        je     SHORT loc_4_5801
loc_4_584E:
        sti
        mov    eax, dword ptr [esi + 3ch]
        push   esi
        or     eax, 2
        mov    dword ptr [esi + 3ch], eax
        and    eax, 0fffffffeh
        mov    dword ptr [esi + 3ch], eax
        call   sub_4_A570
        mov    eax, dword ptr [esi + 3ch]
        and    eax, 0fffffffdh
        mov    dword ptr [esi + 3ch], eax
        test   al, 42h
        jne    SHORT loc_4_5889
        mov    ecx, dword ptr [esi + 38h]
        test   ecx, ecx
        je     SHORT loc_4_5889
        cmp    byte ptr [ecx + 4dh], 0
        jne    SHORT loc_4_5889
        cmp    byte ptr [ecx + 4ch], 0
        jne    SHORT loc_4_5889
        or     eax, 1
        jmp    SHORT loc_4_588C
loc_4_5889:
        and    eax, 0fffffffeh
loc_4_588C:
        mov    dword ptr [esi + 3ch], eax
        test   bl, bl
        je     SHORT loc_4_58D8
        mov    byte ptr [edi], 0
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_58C5
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_58C5
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 055h, 004h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_58C5:
        cmp    byte ptr [esi + 2ch], 1
        jne    SHORT loc_4_58D8
loc_4_58CB:
        mov    byte ptr [esi + 2ch], 0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_58D5:
        add    esp, 4
loc_4_58D8:
        cmp    byte ptr [esp + 12h], 0
        je     SHORT loc_4_58F7
        push   ebp
        push   0ff00ff00h
        push   2
        push   0aa000006h
        push   esi
        call   sub_4_6F20
        add    esp, 14h
        jmp    SHORT loc_4_593B
loc_4_58F7:
        xor    eax, eax
        mov    dword ptr [esi + 1e0h], eax
        mov    byte ptr [esp + 15h], al
        mov    byte ptr [esp + 13h], al
        mov    byte ptr [esp + 19h], al
        mov    byte ptr [esp + 17h], al
        mov    byte ptr [esp + 18h], al
        mov    byte ptr [esp + 16h], al
        mov    byte ptr [esp + 14h], al
        mov    dword ptr [esp + 2ch], 1
        jmp    SHORT loc_4_593B
loc_4_5925:
        push   0
        push   0aa000002h
        push   esi
        call   sub_4_6F20
        add    esp, 0ch
        jmp    SHORT loc_4_593B
loc_4_5937:
        mov    esi, dword ptr [esp + 3ch]
loc_4_593B:
        cmp    byte ptr [esp + 19h], 0
        je     SHORT loc_4_594C
        mov    ecx, dword ptr [esp + 4ch]
        push   ecx
        call   sub_4_7810
loc_4_594C:
        cmp    byte ptr [esp + 18h], 0
        je     SHORT loc_4_5964
        push   0
        mov    eax, dword ptr [esi + 180h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_5961:
        add    esp, 8
loc_4_5964:
        cmp    byte ptr [esp + 17h], 0
        je     SHORT loc_4_59BC
        cmp    dword ptr [esi + 6ch], 0
        je     SHORT loc_4_5990
loc_4_5971:
        mov    eax, dword ptr [esi + 6ch]
        mov    ecx, eax
        test   eax, eax
        je     SHORT loc_4_597F
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 6ch], eax
loc_4_597F:
        push   0
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_5987:
        add    esp, 8
        cmp    dword ptr [esi + 6ch], 0
        jne    SHORT loc_4_5971
loc_4_5990:
        lea    ebx, [esi + 138h]
        mov    edi, 6
loc_4_599B:
        mov    eax, dword ptr [ebx]
        mov    ecx, eax
        test   eax, eax
        je     SHORT loc_4_59A7
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebx], eax
loc_4_59A7:
        test   ecx, ecx
        je     SHORT loc_4_59B6
        push   0
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_59B3:
        add    esp, 8
loc_4_59B6:
        add    ebx, 4
        dec    edi
        jne    SHORT loc_4_599B
loc_4_59BC:
        cmp    byte ptr [esp + 16h], 0
        je     SHORT loc_4_59D4
        push   0
        mov    eax, dword ptr [esi + 128h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_59D1:
        add    esp, 8
loc_4_59D4:
        cmp    byte ptr [esp + 15h], 0
        je     SHORT loc_4_59E6
        mov    ecx, dword ptr [esp + 4ch]
        push   ecx
        push   esi
        call   sub_4_77E0
loc_4_59E6:
        cmp    byte ptr [esp + 14h], 0
        je     SHORT loc_4_59FE
        push   0
        mov    eax, dword ptr [esi + 1c4h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_59FB:
        add    esp, 8
loc_4_59FE:
        cmp    byte ptr [esp + 13h], 0
        je     SHORT loc_4_5A10
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_5A0D:
        add    esp, 8
loc_4_5A10:
        cmp    dword ptr [esp + 2ch], 0
        je     SHORT loc_4_5A44
        mov    eax, dword ptr [esp + 54h]
        push   0
        mov    ecx, dword ptr [esp + 54h]
        push   eax
        mov    edx, dword ptr [esp + 54h]
        push   ecx
        push   esi
        mov    eax, dword ptr [edx + 68h]
        push   eax
        add    esi, 0f8h
        push   esi
        call   sub_4_3960
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret    0ch
loc_4_5A44:
        mov    eax, 0c0000001h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret    0ch
        db  090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_5AB0:
        mov    ecx, dword ptr [esp + 10h]
        sub    esp, 88h
        mov    ax, word ptr [ecx]
        xor    edx, edx
        test   ax, 0fffeh
        push   ebx
        jbe    SHORT loc_4_5ADC
        shr    ax, 1
        mov    ecx, dword ptr [ecx + 4]
        movzx  eax, ax
loc_4_5AD0:
        mov    bl, byte ptr [ecx + edx*2]
        inc    edx
        cmp    edx, eax
        mov    byte ptr [esp + edx + 0bh], bl
        jl     SHORT loc_4_5AD0
loc_4_5ADC:
        lea    eax, [esp + 0ch]
        lea    ecx, [esp + 4]
        mov    byte ptr [esp + edx + 0ch], 0
        push   eax
        push   ecx
        call   sub_4_7490
        mov    ecx, dword ptr [esp + 0a0h]
        lea    edx, [esp + 4]
        mov    eax, dword ptr [esp + 98h]
        push   ecx
        mov    ecx, dword ptr [esp + 98h]
        push   edx
        mov    edx, dword ptr [esp + 98h]
        push   eax
        push   ecx
        push   edx
        call   sub_4_5E40
        pop    ebx
        add    esp, 88h
        ret    14h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_5B30:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0c0000001h
        ret    10h
        db  08Dh, 049h, 000h
loc_4_5B40:
        push   esi
        mov    esi, dword ptr [esp + 10h]
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [eax + 4]
        push   ecx
        call   sub_4_9620
        test   eax, eax
        je     SHORT loc_4_5B6A
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        push   eax
        push   ecx
        push   edx
        call   sub_4_9DD0
        pop    esi
        ret    0ch
loc_4_5B6A:
        mov    eax, dword ptr [esp + 0ch]
        push   esi
        mov    ecx, dword ptr [esp + 0ch]
        push   eax
        mov    dword ptr [esi + 8], 0
        push   ecx
        call   sub_4_5B90
        pop    esi
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
_PTEXT ends
        end
