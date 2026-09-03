; vmvid_pcode_05.asm -- VMVID object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_5_0000:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 228h
        test   eax, eax
        push   ebx
        push   esi
        push   edi
        je     NEAR PTR loc_5_0335
        cmp    eax, 1
        je     SHORT loc_5_0042
        cmp    eax, 2
        je     NEAR PTR loc_5_02D6
        cmp    eax, 3
        je     NEAR PTR loc_5_02E3
        cmp    eax, 4
        je     NEAR PTR loc_5_0320
        mov    eax, 1
        jmp    NEAR PTR loc_5_0337
loc_5_0042:
        cmp    dword ptr [ebp + 0ch], 1
        jbe    SHORT loc_5_0052
        mov    eax, 1
        jmp    NEAR PTR loc_5_0337
loc_5_0052:
        push   0
        lea    eax, [ebp - 228h]
        push   100h
        mov    esi, dword ptr [ebp + 10h]
        push   eax
        push   0
        push   esi
        call   sub_5_0E68
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_5_0337
        push   0
        lea    eax, [ebp - 128h]
        push   esi
        push   eax
        call   sub_5_0E60
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_5_0337
        mov    bx, 388h
        movzx  eax, word ptr [ebp - 0cch]
        xor    di, di
        cmp    eax, 1
        je     SHORT loc_5_00C5
        cmp    eax, 2
        je     SHORT loc_5_00CF
        cmp    eax, 3
        je     SHORT loc_5_00D9
        cmp    eax, 4
        je     SHORT loc_5_00F9
        cmp    eax, 5
        je     NEAR PTR loc_5_0141
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0337
loc_5_00C5:
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0337
loc_5_00CF:
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0337
loc_5_00D9:
        cmp    word ptr [ebp - 9eh], 10h
        je     SHORT loc_5_00ED
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0337
loc_5_00ED:
        mov    cx, word ptr [ebp - 0c6h]
        jmp    NEAR PTR loc_5_0180
loc_5_00F9:
        cmp    word ptr [ebp - 9eh], 10h
        je     SHORT loc_5_010D
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0337
loc_5_010D:
        movzx  eax, word ptr [ebp - 9ch]
        mov    cx, word ptr [ebp - 0c6h]
        cmp    eax, 2
        je     SHORT loc_5_012F
        cmp    eax, 4
        je     SHORT loc_5_0138
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0337
loc_5_012F:
        mov    di, word ptr [ebp - 0c4h]
        jmp    SHORT loc_5_0180
loc_5_0138:
        mov    bx, word ptr [ebp - 0c4h]
        jmp    SHORT loc_5_0180
loc_5_0141:
        cmp    word ptr [ebp - 9eh], 10h
        jne    NEAR PTR loc_5_02CF
        cmp    word ptr [ebp - 9ch], 4
        jne    NEAR PTR loc_5_02CF
        cmp    word ptr [ebp - 9ah], 2
        jne    NEAR PTR loc_5_02CF
        mov    cx, word ptr [ebp - 0c6h]
        mov    bx, word ptr [ebp - 0c4h]
        mov    di, word ptr [ebp - 0c2h]
loc_5_0180:
        cmp    word ptr [ebp - 7ah], 1
        jb     NEAR PTR loc_5_02C8
        cmp    word ptr [ebp - 6ah], 2
        jb     NEAR PTR loc_5_02C8
        movzx  ax, byte ptr [ebp - 78h]
        movzx  dx, byte ptr [ebp - 68h]
        mov    word ptr [ebp - 6], ax
        cmp    word ptr [ebp - 6ah], 1
        movzx  ax, byte ptr [ebp - 77h]
        jbe    SHORT loc_5_01BF
        mov    word ptr [ebp - 4], dx
        movzx  dx, byte ptr [ebp - 67h]
        mov    word ptr [ebp - 2], dx
        jmp    SHORT loc_5_01C7
loc_5_01BF:
        mov    word ptr [ebp - 2], dx
        mov    word ptr [ebp - 4], dx
loc_5_01C7:
        movzx  edx, cx
        mov    dword ptr [ebp - 10h], edx
        cmp    edx, -1
        jne    SHORT loc_5_01DC
        mov    eax, 13h
        jmp    NEAR PTR loc_5_0337
loc_5_01DC:
        push   0
        push   10h
        push   dword ptr [ebp - 2]
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 6]
        push   eax
        push   edi
        push   ebx
        push   ecx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_01F3:
        add    esp, 28h
        mov    dword ptr [ebp - 0ch], eax
        test   eax, eax
        jne    NEAR PTR loc_5_02B4
        sub    bx, 0ffffh
        mov    dword ptr [ebp - 28h], esi
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        cmp    bx, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [ebp - 30h], eax
        push   dword ptr [ebp - 30h]
        push   dword ptr [ebp - 2ch]
        push   dword ptr [ebp - 28h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 001h, 000h, 04Ah, 004h
        add    esp, 0ch
        sub    di, 0ffffh
        mov    dword ptr [ebp - 1ch], esi
        db  0C7h, 045h, 0E0h, 000h, 000h, 000h, 000h
        cmp    di, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [ebp - 24h], eax
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp - 1ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 001h, 000h, 04Ah, 004h
        add    esp, 0ch
        cmp    word ptr [ebp - 6], 9
        mov    ecx, 2
        je     SHORT loc_5_0273
        movzx  ecx, word ptr [ebp - 6]
loc_5_0273:
        movzx  eax, word ptr [ebp - 2]
        movzx  edx, word ptr [ebp - 4]
        push   eax
        push   edx
        push   ecx
        lea    edx, [ebp - 50h]
        push   dword ptr [ebp - 10h]
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        call   sub_5_0E58
        add    esp, 18h
        lea    ecx, [ebp - 50h]
        db  0C7h, 045h, 0ECh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 18h], ecx
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 14h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 002h, 000h, 04Ah, 004h
        add    esp, 8
loc_5_02B4:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_5_02C3
        push   esi
        call   sub_5_0908
        add    esp, 4
loc_5_02C3:
        mov    eax, dword ptr [ebp - 0ch]
        jmp    SHORT loc_5_0337
loc_5_02C8:
        mov    eax, 13h
        jmp    SHORT loc_5_0337
loc_5_02CF:
        mov    eax, 13h
        jmp    SHORT loc_5_0337
loc_5_02D6:
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_5_0320
        mov    eax, 1
        jmp    SHORT loc_5_0337
loc_5_02E3:
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        je     SHORT loc_5_02F6
        cmp    eax, 1
        je     SHORT loc_5_030B
        mov    eax, 1
        jmp    SHORT loc_5_0337
loc_5_02F6:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_02FB:
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 13h
        jmp    SHORT loc_5_0337
loc_5_030B:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0310:
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 13h
        jmp    SHORT loc_5_0337
loc_5_0320:
        mov    esi, dword ptr [ebp + 10h]
        push   esi
        call   sub_5_09B7
        add    esp, 4
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0332:
        add    esp, 4
loc_5_0335:
        xor    eax, eax
loc_5_0337:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  0CCh, 0CCh
sub_5_0340:
        db  0CDh, 020h, 083h, 080h, 001h, 000h
sub_5_0346:
        db  0CDh, 020h, 086h, 080h, 001h, 000h
sub_5_034C:
        db  0CDh, 020h, 085h, 080h, 001h, 000h
sub_5_0352:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        pop    ebp
        ret
sub_5_0362:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        pop    ebp
        ret
sub_5_0372:
        movzx  ecx, word ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        db  0CDh, 020h, 048h, 080h, 001h, 000h
sub_5_0381:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        pop    ebp
        ret
sub_5_038F:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
sub_5_039A:
        push   ebp
        mov    ebp, esp
        sub    esp, 6ch
        mov    eax, 0ffffffffh
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   esi, esi
        je     NEAR PTR loc_5_0484
        lea    eax, [ebp - 6ch]
        push   eax
        call   sub_5_0352
        add    esp, 4
        call   sub_5_0340
        call   sub_5_038F
        mov    edi, dword ptr [eax + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_0381
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_0381
        add    esp, 4
        mov    ebx, dword ptr [ebp + 10h]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_0381
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_0381
        add    esp, 4
        mov    ebx, dword ptr [ebp + 14h]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_0381
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_0381
        add    esp, 4
        mov    ebx, dword ptr [ebp + 18h]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_0381
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_0381
        movzx  ecx, si
        shr    esi, 10h
        add    esp, 4
        movzx  eax, si
        push   ecx
        push   eax
        call   sub_5_0372
        add    esp, 8
        call   sub_5_034C
        movzx  esi, word ptr [edi + 14h]
        shl    esi, 10h
        movzx  eax, word ptr [edi + 1ch]
        or     esi, eax
        call   sub_5_0346
        lea    ecx, [ebp - 6ch]
        push   ecx
        call   sub_5_0362
        add    esp, 4
        mov    eax, esi
loc_5_0484:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_048B:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        push   ebp
        mov    ebp, dword ptr [esi]
        test   ebp, ebp
        jne    SHORT loc_5_04B1
        push   15h
        push   4
        call   sub_5_0EB4
        mov    ebp, eax
        test   ebp, ebp
        jne    SHORT loc_5_04AF
        xor    eax, eax
        jmp    NEAR PTR loc_5_0616
loc_5_04AF:
        mov    dword ptr [esi], ebp
loc_5_04B1:
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 8
        jbe    SHORT loc_5_04CE
        mov    eax, dword ptr [esp + 1ch]
        mov    byte ptr [eax + 8], 0
loc_5_04CE:
        push   ebp
        call   sub_5_0EA4
        mov    ebx, eax
        test   ebx, ebx
        je     NEAR PTR loc_5_05C2
loc_5_04DE:
        mov    ecx, dword ptr [esp + 1ch]
        lea    edx, [ebx + 0ch]
loc_5_04E5:
        mov    al, byte ptr [ecx]
        cmp    al, byte ptr [edx]
        jne    SHORT loc_5_0505
        or     al, al
        je     SHORT loc_5_0501
        mov    al, byte ptr [ecx + 1]
        cmp    al, byte ptr [edx + 1]
        jne    SHORT loc_5_0505
        add    ecx, 2
        add    edx, 2
        or     al, al
        jne    SHORT loc_5_04E5
loc_5_0501:
        xor    eax, eax
        jmp    SHORT loc_5_050A
loc_5_0505:
        sbb    eax, eax
        sbb    eax, -1
loc_5_050A:
        test   eax, eax
        je     SHORT loc_5_051B
        push   ebx
        push   ebp
        call   sub_5_0E94
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_5_04DE
loc_5_051B:
        test   ebx, ebx
        je     NEAR PTR loc_5_05C2
        cmp    dword ptr [ebx + 4], 0
        je     SHORT loc_5_0530
        xor    eax, eax
        jmp    NEAR PTR loc_5_0616
loc_5_0530:
        push   0
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_039A
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_5_055D
        push   ebx
        mov    dword ptr [ebx], 0
        push   esi
        call   sub_5_061B
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_5_0616
loc_5_055D:
        mov    eax, dword ptr [esp + 20h]
        push   0
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebx + 4], ecx
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   ecx
        call   sub_5_039A
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_5_0594
        mov    dword ptr [ebx + 4], 0
        push   ebx
        push   esi
        call   sub_5_061B
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_5_0616
loc_5_0594:
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx + 8]
        push   10000h
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_039A
        add    esp, 14h
        push   dword ptr [ebx]
        push   dword ptr [ebx + 8]
        push   10000h
        push   ebx
        push   dword ptr [ebx + 4]
        call   sub_5_039A
        add    esp, 14h
        jmp    SHORT loc_5_0614
loc_5_05C2:
        push   ebp
        call   sub_5_0E80
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_5_05D2
        xor    eax, eax
        jmp    SHORT loc_5_0616
loc_5_05D2:
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
        call   sub_5_0E70
loc_5_0614:
        mov    eax, ebx
loc_5_0616:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_061B:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        test   edi, edi
        je     SHORT loc_5_0667
        mov    eax, dword ptr [edi]
        test   eax, eax
        je     SHORT loc_5_0645
        push   0
        push   dword ptr [edi + 8]
        push   40000h
        push   edi
        push   eax
        call   sub_5_039A
        add    esp, 14h
        mov    dword ptr [edi], 0
loc_5_0645:
        mov    eax, dword ptr [edi + 4]
        test   eax, eax
        je     SHORT loc_5_0667
        push   0
        push   dword ptr [edi + 8]
        push   40000h
        push   edi
        push   eax
        call   sub_5_039A
        mov    dword ptr [edi + 4], 0
        add    esp, 14h
loc_5_0667:
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     SHORT loc_5_0698
        push   edi
        push   eax
        call   sub_5_0EE8
        push   edi
        push   dword ptr [esi]
        call   sub_5_0ED8
        push   dword ptr [esi]
        call   sub_5_0EA4
        test   eax, eax
        jne    SHORT loc_5_0698
        push   dword ptr [esi]
        call   sub_5_0EC8
        mov    dword ptr [esi], 0
loc_5_0698:
        pop    edi
        pop    esi
        ret
loc_5_069B:
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        push   edi
        mov    ecx, 10h
        db  0BFh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb
        je     SHORT loc_5_06D0
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        mov    esi, eax
        repe cmpsb
        mov    eax, 80004002h
        jne    SHORT loc_5_06DE
loc_5_06D0:
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_06DE:
        pop    edi
        pop    esi
        ret    0ch
loc_5_06E3:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0ch]
        inc    eax
        mov    dword ptr [ecx + 0ch], eax
        ret    4
loc_5_06F1:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0ch]
        dec    eax
        mov    dword ptr [ecx + 0ch], eax
        ret    4
loc_5_06FF:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 14h]
        lea    esi, [edi + 104h]
        mov    dword ptr [edi], 0bh
        call   sub_5_0C74
        mov    ebx, eax
        push   0ffh
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_5_0EFC
        add    esp, 0ch
        lea    eax, [edi + 4]
        push   0ffh
        push   ebx
        push   eax
        call   sub_5_0EFC
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 0ch
        xor    ecx, ecx
        mov    dword ptr [edi + 210h], ecx
        mov    edx, dword ptr [eax + 4]
        mov    eax, 0b3h
        mov    dword ptr [edi + 204h], edx
        mov    dword ptr [edi + 20ch], ecx
        mov    word ptr [edi + 208h], 3202h
        cmp    byte ptr [esi], cl
        je     SHORT loc_5_0776
loc_5_076B:
        movsx  ecx, byte ptr [esi]
        add    eax, ecx
        inc    esi
        cmp    byte ptr [esi], 0
        jne    SHORT loc_5_076B
loc_5_0776:
        mov    ecx, 0f7h
        sub    edx, edx
        div    ecx
        mov    word ptr [edi + 20ah], dx
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_5_078E:
        push   esi
        push   edi
        mov    esi, 8878000ah
        mov    edi, dword ptr [esp + 0ch]
        cmp    dword ptr [edi + 0ch], 0
        jne    SHORT loc_5_07B3
        push   dword ptr [edi + 4]
        call   sub_5_0D42
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_5_07B3
        xor    esi, esi
        inc    dword ptr [edi + 0ch]
loc_5_07B3:
        mov    eax, esi
        pop    edi
        pop    esi
        ret    4
loc_5_07BA:
        mov    eax, dword ptr [esp + 8]
        xor    ecx, ecx
        mov    dword ptr [eax], 0fh
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
        mov    dword ptr [eax + 0ch], 1
        xor    eax, eax
        ret    8
loc_5_0805:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   ebp
        mov    edi, 8007000eh
        mov    esi, dword ptr [esp + 2ch]
        test   byte ptr [esp + 1ch], 0f0h
        mov    dword ptr [esi], ebx
        je     SHORT loc_5_0827
        mov    eax, 8878001eh
        jmp    NEAR PTR loc_5_08C7
loc_5_0827:
        mov    ebp, dword ptr [esp + 14h]
        cmp    dword ptr [ebp + 10h], 0
        jne    SHORT loc_5_08A6
        push   201h
        push   31h
        call   sub_5_0F0C
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_5_089F
        db  0C7h, 003h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 4], ebp
        push   dword ptr [ebp + 4]
        push   ebx
        call   sub_5_0D77
        add    esp, 8
        mov    eax, dword ptr [ebx + 14h]
        cmp    eax, 1000h
        jbe    SHORT loc_5_08AB
        cmp    eax, 0ffffh
        jae    SHORT loc_5_08AB
        cmp    dword ptr [ebx + 0ch], 0
        je     SHORT loc_5_08AB
        cmp    dword ptr [ebx + 10h], 0
        je     SHORT loc_5_08AB
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 2ch]
        push   ebx
        xor    edi, edi
        call   sub_5_0A1F
        mov    eax, dword ptr [esp + 24h]
        add    esp, 0ch
        movzx  ecx, word ptr [eax + 0eh]
        mov    dword ptr [ebx + 24h], ecx
        movzx  edx, word ptr [eax + 2]
        mov    dword ptr [ebx + 28h], edx
        jmp    SHORT loc_5_08AB
loc_5_089F:
        mov    edi, 8007000eh
        jmp    SHORT loc_5_08AB
loc_5_08A6:
        mov    edi, 8878000ah
loc_5_08AB:
        test   edi, edi
        je     SHORT loc_5_08C0
        test   ebx, ebx
        je     SHORT loc_5_08C5
        push   0
        push   ebx
        call   sub_5_0F04
        add    esp, 8
        jmp    SHORT loc_5_08C5
loc_5_08C0:
        mov    dword ptr [esi], ebx
        inc    dword ptr [ebp + 10h]
loc_5_08C5:
        mov    eax, edi
loc_5_08C7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    1ch
loc_5_08CE:
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], 0
        mov    eax, 80004001h
        ret    0ch
loc_5_08E0:
        mov    eax, 80004005h
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 0ch]
        dec    edx
        mov    dword ptr [ecx + 0ch], edx
        jne    SHORT loc_5_0905
        cmp    dword ptr [ecx + 10h], 0
        jne    SHORT loc_5_0905
        push   dword ptr [ecx + 4]
        call   sub_5_0D38
        add    esp, 4
        xor    eax, eax
loc_5_0905:
        ret    4
sub_5_0908:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   201h
        mov    dword ptr [ebp - 4], ebx
        push   14h
        call   sub_5_0F0C
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_5_0987
        mov    edi, dword ptr [ebp + 8]
        push   1
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 14h]
        push   ecx
        mov    dword ptr [esi + 4], edi
        db  0C7h, 006h, 000h, 000h, 000h, 000h
        call   sub_5_0F28
        test   eax, eax
        jne    SHORT loc_5_0987
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
        jne    SHORT loc_5_0987
        push   esi
        mov    ebx, 1
        push   edi
        call   sub_5_0E4A
        add    esp, 8
loc_5_0987:
        test   ebx, ebx
        jne    SHORT loc_5_09B0
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_5_09A1
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 0bh]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_5_0F14
loc_5_09A1:
        test   esi, esi
        je     SHORT loc_5_09B0
        push   0
        push   esi
        call   sub_5_0F04
        add    esp, 8
loc_5_09B0:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_09B7:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_5_0E40
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_5_0A19
        push   0
        push   edi
        call   sub_5_0E4A
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
        call   sub_5_0F14
        push   0
        push   esi
        call   sub_5_0F04
        add    esp, 8
loc_5_0A19:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_5_0A1F:
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 14h]
        cmp    dword ptr [edx], eax
        jbe    SHORT loc_5_0A31
        mov    dword ptr [edx], eax
loc_5_0A31:
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
loc_5_0A51:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 20h]
        dec    esi
        mov    dword ptr [eax + 20h], esi
        jne    SHORT loc_5_0A76
        cmp    dword ptr [eax + 8], 0
        jne    SHORT loc_5_0A76
        mov    ecx, dword ptr [eax + 4]
        push   0
        push   eax
        dec    dword ptr [ecx + 10h]
        call   sub_5_0F04
        add    esp, 8
loc_5_0A76:
        mov    eax, esi
        pop    esi
        ret    4
loc_5_0A7C:
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        push   edi
        mov    ecx, 10h
        db  0BFh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb
        je     SHORT loc_5_0AB1
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        mov    esi, eax
        repe cmpsb
        mov    eax, 80004002h
        jne    SHORT loc_5_0ABF
loc_5_0AB1:
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_0ABF:
        pop    edi
        pop    esi
        ret    0ch
loc_5_0AC4:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 20h]
        inc    eax
        mov    dword ptr [ecx + 20h], eax
        ret    4
loc_5_0AD2:
        mov    eax, 80004001h
        ret    20h
loc_5_0ADA:
        mov    eax, 80004001h
        ret    14h
loc_5_0AE2:
        mov    eax, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 4]
        movzx  ecx, word ptr [eax + 0eh]
        mov    dword ptr [edx + 24h], ecx
        movzx  eax, word ptr [eax + 2]
        mov    dword ptr [edx + 28h], eax
        mov    eax, 80004001h
        ret    8
loc_5_0B00:
        mov    eax, 80004001h
        ret    8
loc_5_0B08:
        mov    eax, 80004001h
        ret    8
loc_5_0B10:
        mov    eax, 80004001h
        ret    8
loc_5_0B18:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 8], 1
        jne    SHORT loc_5_0B5A
        cmp    dword ptr [esi + 24h], 10h
        jne    SHORT loc_5_0B30
        db  0A1h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_5_0B35
loc_5_0B30:
        db  0A1h, 000h, 000h, 000h, 000h
loc_5_0B35:
        push   eax
        call   sub_5_0C82
        add    esp, 4
        inc    eax
        cmp    dword ptr [esi + 24h], 10h
        jne    SHORT loc_5_0B47
        add    eax, eax
loc_5_0B47:
        mov    ecx, dword ptr [esi + 1ch]
        mov    edx, dword ptr [esp + 10h]
        sub    ecx, eax
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx], ecx
        jmp    SHORT loc_5_0B6E
loc_5_0B5A:
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx], 0
loc_5_0B6E:
        xor    eax, eax
        pop    esi
        ret    0ch
loc_5_0B74:
        push   ebx
        mov    eax, 80004005h
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        cmp    dword ptr [edi + 8], 0
        jne    NEAR PTR loc_5_0C10
        cmp    dword ptr [edi + 24h], 10h
        movzx  esi, word ptr [edi + 2eh]
        jne    SHORT loc_5_0B9C
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_5_0BA2
loc_5_0B9C:
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
loc_5_0BA2:
        push   ebx
        call   sub_5_0C9D
        add    esp, 4
        push   dword ptr [edi + 1ch]
        push   dword ptr [edi + 0ch]
        push   ebx
        call   sub_5_0CC1
        add    esp, 0ch
        push   ebx
        call   sub_5_0CE2
        add    esp, 4
        push   ebx
        call   sub_5_0CAB
        mov    bx, word ptr [edi + 1ch]
        add    esp, 4
        cmp    dword ptr [edi + 24h], 10h
        jne    SHORT loc_5_0BD9
        shr    bx, 1
loc_5_0BD9:
        cmp    dword ptr [edi + 28h], 2
        jne    SHORT loc_5_0BE2
        shr    bx, 1
loc_5_0BE2:
        dec    bx
        push   0ffffh
        push   0ffffh
        push   esi
        call   sub_5_0E07
        add    esp, 0ch
        movzx  ecx, bx
        push   dword ptr [edi + 24h]
        push   ecx
        push   esi
        call   sub_5_0DCC
        add    esp, 0ch
        xor    eax, eax
        mov    dword ptr [edi + 8], 1
loc_5_0C10:
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
loc_5_0C16:
        push   esi
        mov    eax, 80004005h
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        cmp    dword ptr [edi + 8], 0
        je     SHORT loc_5_0C6F
        movzx  esi, word ptr [edi + 2eh]
        push   0d0h
        push   0
        push   esi
        call   sub_5_0D06
        add    esp, 0ch
        push   0dah
        push   0
        push   esi
        call   sub_5_0D06
        add    esp, 0ch
        cmp    dword ptr [edi + 24h], 10h
        jne    SHORT loc_5_0C58
        db  0A1h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_5_0C5D
loc_5_0C58:
        db  0A1h, 000h, 000h, 000h, 000h
loc_5_0C5D:
        push   eax
        call   sub_5_0C9D
        add    esp, 4
        xor    eax, eax
        mov    dword ptr [edi + 8], 0
loc_5_0C6F:
        pop    edi
        pop    esi
        ret    4
sub_5_0C74:
        db  00Fh, 0B7h, 005h, 00Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        ret
sub_5_0C82:
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
sub_5_0C9D:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 018h, 000h, 004h, 000h
        pop    ebp
        ret
sub_5_0CAB:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 019h, 000h, 004h, 000h
        pop    ebx
        pop    ebp
        ret
sub_5_0CC1:
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
sub_5_0CE2:
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
sub_5_0D06:
        push   ebp
        mov    ebp, esp
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    ah, byte ptr [ebp + 0ch]
        mov    al, byte ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0D18:
        xor    eax, eax
        jae    SHORT loc_5_0D21
        mov    eax, 1
loc_5_0D21:
        pop    edx
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    ah, byte ptr [ebp + 0ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0D33:
        xor    ah, ah
        pop    edx
        pop    ebp
        ret
sub_5_0D38:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        ret
sub_5_0D42:
        push   ebp
        mov    ebp, esp
        push   edi
        push   edx
        push   ebx
        mov    edx, 0ffffh
        db  0A1h, 000h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_5_0D62
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        cmp    eax, ebx
        je     SHORT loc_5_0D62
        jmp    SHORT loc_5_0D70
loc_5_0D62:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    edx, 0
loc_5_0D70:
        mov    eax, edx
        pop    ebx
        pop    edx
        pop    edi
        pop    ebp
        ret
sub_5_0D77:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 8]
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    word ptr [esi + 2eh], ax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [esi + 30h], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 0ch], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 10h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 14h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    word ptr [esi + 2ch], ax
        pop    esi
        pop    edi
        pop    ebp
        ret
sub_5_0DCC:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ecx
        push   edx
        pushfd
        cli
        mov    edx, dword ptr [ebp + 8]
        mov    al, 48h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0DDE:
        mov    eax, dword ptr [ebp + 0ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0DE6:
        mov    al, ah
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0DED:
        mov    ecx, dword ptr [ebp + 10h]
        cmp    ecx, 10h
        jne    SHORT loc_5_0DF9
        mov    al, 1ch
        jmp    SHORT loc_5_0DFB
loc_5_0DF9:
        mov    al, 1ch
loc_5_0DFB:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0E00:
        popfd
        clc
        pop    edx
        pop    ecx
        pop    eax
        pop    ebp
        ret
sub_5_0E07:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ebx
        push   edx
        mov    eax, dword ptr [ebp + 0ch]
        call   sub_5_0E3C
        mov    ebx, eax
        mov    eax, dword ptr [ebp + 10h]
        call   sub_5_0E3C
        shl    bl, 4
        and    al, 0fh
        or     al, bl
        mov    ah, al
        mov    al, 4
        mov    edx, dword ptr [ebp + 8]
        add    dx, 4
        out    dx, al
        inc    dx
        mov    al, ah
        out    dx, al
        pop    edx
        pop    ebx
        pop    eax
        pop    ebp
        ret
sub_5_0E3C:
        shr    eax, 0ch
        ret
sub_5_0E40:
        push   ebp
        mov    ebp, esp
        db  0A1h, 000h, 000h, 000h, 000h
        pop    ebp
        ret
sub_5_0E4A:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        db  0A3h, 000h, 000h, 000h, 000h
        pop    ebp
        ret
        db  0CCh
sub_5_0E58:
        db  0CDh, 020h, 074h, 081h, 001h, 000h
        int3
        int3
sub_5_0E60:
        db  0CDh, 020h, 03Bh, 080h, 033h, 000h
        int3
        int3
sub_5_0E68:
        db  0CDh, 020h, 03Dh, 080h, 033h, 000h
        int3
        int3
sub_5_0E70:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_0E80:
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
sub_5_0E94:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_0EA4:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_0EB4:
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
sub_5_0EC8:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_0ED8:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_0EE8:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        mov    esi, ecx
        sbb    eax, eax
        inc    eax
        jmp    edx
        db  0CCh, 0CCh
sub_5_0EFC:
        db  0CDh, 020h, 07Bh, 081h, 001h, 000h
        int3
        int3
sub_5_0F04:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        int3
        int3
sub_5_0F0C:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        int3
        int3
sub_5_0F14:
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
sub_5_0F28:
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 0ch]
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        jb     SHORT loc_5_0F46
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_5_0F46:
        ret    10h
_PTEXT ends
        end
