; ppm_pcode_03.asm -- PPM object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        xor    eax, eax
        mov    edx, dword ptr [esp + 0ch]
        push   34h
        pop    ecx
        cmp    dword ptr [edx], ecx
        mov    dword ptr [edx], ecx
        jb     SHORT loc_3_0043
        mov    edx, dword ptr [esp + 8]
        or     edx, edx
        je     SHORT loc_3_0043
        mov    dword ptr [edx + 28h], eax
        mov    dword ptr [edx + 2ch], eax
        mov    byte ptr [edx + 30h], al
        mov    al, 34h
        mov    dword ptr [edx], eax
        mov    al, 1
        mov    dword ptr [edx + 24h], eax
        shl    eax, 8
        mov    word ptr [edx + 4], ax
        mov    eax, dword ptr [esp + 4]
        lea    eax, [eax + 94h]
        lea    edx, [edx + 8]
        db  0CDh, 020h, 020h, 000h, 02Bh, 000h
loc_3_0043:
        or     al, 1
        ret
loc_3_0046:
        sub    esp, 30h
        xor    eax, eax
        mov    edx, dword ptr [esp + 34h]
        mov    dword ptr [edx + 0ch], 0ffffffebh
        cmp    dword ptr [esp + 3ch], 34h
        jb     SHORT loc_3_007C
        lea    edx, [esp]
        mov    eax, dword ptr [esp + 38h]
        lea    eax, [eax + 8]
        db  0CDh, 020h, 01Fh, 000h, 02Bh, 000h
        push   -1
        push   edx
        push   dword ptr [esp + 3ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0079:
        add    esp, 0ch
loc_3_007C:
        add    esp, 30h
        ret
sub_3_0080:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_3_0096
        mov    eax, 1
        jmp    SHORT loc_3_010F
loc_3_0096:
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_009B:
        db  0A3h, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 000h, 000h, 001h, 000h
        mov    word ptr [ebp - 4], ax
        cmp    word ptr [ebp - 4], 0
        je     SHORT loc_3_010D
        db  0CDh, 020h, 000h, 000h, 02Bh, 000h
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], 0
        je     SHORT loc_3_010D
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_00CB:
        push   2f4h
        push   4
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_00D7:
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_3_010D
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_00EA:
        add    esp, 4
        test   eax, eax
        je     SHORT loc_3_00F8
        mov    eax, 1
        jmp    SHORT loc_3_010F
loc_3_00F8:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0103:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_3_010D:
        xor    eax, eax
loc_3_010F:
        mov    esp, ebp
        pop    ebp
        ret
sub_3_0113:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_3_0159
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0123:
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_3_0144
loc_3_0129:
        push   esi
        call   sub_3_0433
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        push   esi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_013E:
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_3_0129
loc_3_0144:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_014F:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_3_0159:
        mov    eax, 1
        pop    esi
        ret
loc_3_0160:
        push   ebp
        mov    ebp, esp
        sub    esp, 40h
        cmp    dword ptr [ebp + 8], 0
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_3_0176
        xor    eax, eax
        jmp    NEAR PTR loc_3_0236
loc_3_0176:
        mov    esi, dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [ebp + 1ch]
        lea    edi, [ebp - 40h]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 0ch], ecx
        xor    eax, eax
        mov    ecx, 8
        rep stosd
        push   eax
        lea    edx, [ebp - 14h]
        push   edx
        lea    eax, [ebp - 40h]
        push   eax
        mov    edi, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 14h], 20h
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_01B3:
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_3_021E
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 40h]
        push   eax
        call   sub_3_023D
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_021E
        push   2
        lea    eax, [ebp - 10h]
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_01DB:
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 4], ecx
        test   eax, eax
        jne    SHORT loc_3_021E
loc_3_01E8:
        push   0
        lea    ecx, [ebp - 4]
        push   0
        push   2
        push   dword ptr [ebp - 4]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_01FA:
        add    esp, 14h
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_3_021E
        push   0
        lea    eax, [ebp - 20h]
        push   0ch
        push   eax
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0213:
        movzx  esi, word ptr [ebp - 1ch]
        add    esp, 10h
        test   ebx, ebx
        je     SHORT loc_3_01E8
loc_3_021E:
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        push   esi
        push   dword ptr [ebp + 10h]
        push   edi
        call   sub_3_027F
        add    esp, 14h
        mov    eax, 1
loc_3_0236:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_023D:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        mov    ecx, dword ptr [ebp + 0ch]
loc_3_0248:
        mov    bl, byte ptr [eax]
        mov    dl, byte ptr [ecx]
        inc    eax
        inc    ecx
        test   bl, bl
        je     SHORT loc_3_0274
        test   dl, dl
        je     SHORT loc_3_0274
        cmp    bl, 61h
        jl     SHORT loc_3_0263
        cmp    bl, 7ah
        jg     SHORT loc_3_0263
        sub    bl, 20h
loc_3_0263:
        cmp    dl, 61h
        jl     SHORT loc_3_0270
        cmp    dl, 7ah
        jg     SHORT loc_3_0270
        sub    dl, 20h
loc_3_0270:
        cmp    dl, bl
        je     SHORT loc_3_0248
loc_3_0274:
        movsx  eax, bl
        movsx  ecx, dl
        sub    eax, ecx
        pop    ebx
        pop    ebp
        ret
sub_3_027F:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0293:
        mov    ebx, eax
        test   ebx, ebx
        je     NEAR PTR loc_3_042C
        push   ebx
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        mov    edi, ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_02AB:
        xor    eax, eax
        mov    ecx, 0bdh
        rep stosd
        mov    word ptr [ebx + 2], 10ah
        db  0C7h, 043h, 004h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 8], 17h
        mov    dword ptr [ebx + 94h], 30h
        mov    byte ptr [ebx + 0adh], 11h
        mov    byte ptr [ebx + 0aeh], 13h
        mov    dword ptr [ebx + 0d4h], 0ffffffffh
        mov    byte ptr [ebx + 124h], 0c0h
        mov    byte ptr [ebx + 125h], 10h
        mov    word ptr [ebx], 94h
        push   eax
        mov    byte ptr [ebp - 4], 2
        lea    ecx, [ebp - 4]
        mov    byte ptr [ebp - 2], al
        mov    byte ptr [ebp - 3], 10h
        mov    byte ptr [ebp - 1], 80h
        mov    dword ptr [ebp - 8], 4
        lea    eax, [ebp - 8]
        push   eax
        push   ecx
        push   3
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_032D:
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_3_037E
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebx + 124h], al
        cmp    byte ptr [ebp - 2], 0
        je     SHORT loc_3_034A
        or     byte ptr [ebx + 0fch], 1
loc_3_034A:
        cmp    byte ptr [ebp - 4], 1
        jne    SHORT loc_3_0359
        or     byte ptr [ebx + 0fch], 4
        jmp    SHORT loc_3_0366
loc_3_0359:
        cmp    byte ptr [ebp - 4], 0
        jne    SHORT loc_3_0366
        or     byte ptr [ebx + 0fch], 6
loc_3_0366:
        cmp    byte ptr [ebp - 3], 1
        ja     SHORT loc_3_0375
        or     byte ptr [ebx + 0fch], 10h
        jmp    SHORT loc_3_037E
loc_3_0375:
        mov    al, byte ptr [ebp - 3]
        mov    byte ptr [ebx + 125h], al
loc_3_037E:
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 14h]
        push   0
        mov    edi, dword ptr [ebp + 18h]
        mov    dword ptr [ebx + 0c4h], eax
        mov    dword ptr [ebx + 113h], ecx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        mov    dword ptr [ebx + 132h], 0ffffffffh
        repne scasb
        not    ecx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_03B0:
        add    esp, 8
        mov    edx, eax
        mov    dword ptr [ebx + 120h], eax
        test   edx, edx
        je     SHORT loc_3_0423
        mov    edi, dword ptr [ebp + 18h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, edx
        rep movsd
        mov    ecx, eax
        push   edx
        and    ecx, 3
        db  068h, 000h, 000h, 000h, 000h
        rep movsb
        push   dword ptr [ebp + 0ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_03EF:
        add    esp, 0ch
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_03F8:
        add    esp, 4
        push   dword ptr [ebp + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0403:
        add    esp, 4
        mov    dword ptr [ebx + 126h], eax
        test   eax, eax
        je     SHORT loc_3_042C
        push   dword ptr [ebp + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0418:
        add    esp, 4
        mov    dword ptr [ebx + 12ah], eax
        jmp    SHORT loc_3_042C
loc_3_0423:
        push   ebx
        call   sub_3_0433
        add    esp, 4
loc_3_042C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_0433:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 120h]
        test   eax, eax
        je     SHORT loc_3_044F
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_044C:
        add    esp, 8
loc_3_044F:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_045B:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0467:
        pop    esi
        pop    ebp
        ret
loc_3_046A:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_047B:
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_3_04CB
        mov    edi, dword ptr [ebp + 8]
loc_3_0484:
        mov    eax, edi
        mov    edx, dword ptr [esi + 120h]
loc_3_048C:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    SHORT loc_3_04AC
        or     cl, cl
        je     SHORT loc_3_04A8
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    SHORT loc_3_04AC
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    SHORT loc_3_048C
loc_3_04A8:
        xor    eax, eax
        jmp    SHORT loc_3_04B1
loc_3_04AC:
        sbb    eax, eax
        sbb    eax, -1
loc_3_04B1:
        test   eax, eax
        je     SHORT loc_3_04C7
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_04C1:
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_3_0484
loc_3_04C7:
        test   esi, esi
        jne    SHORT loc_3_04E2
loc_3_04CB:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 0ch], 0fffffff6h
        mov    dword ptr [eax], 0fffffff6h
        xor    eax, eax
        jmp    NEAR PTR loc_3_069E
loc_3_04E2:
        cmp    dword ptr [esi + 64h], 534d5446h
        jne    SHORT loc_3_0502
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 0ch], 0fffffffeh
        mov    dword ptr [eax], 0fffffffeh
        xor    eax, eax
        jmp    NEAR PTR loc_3_069E
loc_3_0502:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0508:
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_3_0526
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 0ch], 0fffffffeh
        mov    dword ptr [eax], 0fffffffeh
        xor    eax, eax
        jmp    NEAR PTR loc_3_069E
loc_3_0526:
        mov    eax, dword ptr [esi + 113h]
        test   eax, eax
        je     SHORT loc_3_0562
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 002h, 000h, 000h, 000h, 004h, 000h
        db  089h, 035h, 01Ch, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0559:
        add    esp, 4
        mov    dword ptr [esi + 117h], eax
loc_3_0562:
        lea    edi, [esi + 4ch]
        xor    eax, eax
        mov    ecx, 6
        rep stosd
        xor    ecx, ecx
        mov    eax, 0ffffffffh
        mov    byte ptr [esi + 100h], cl
        mov    byte ptr [esi + 11bh], cl
        mov    byte ptr [esi + 3bh], cl
        mov    dword ptr [esi + 0e0h], ecx
        mov    dword ptr [esi + 0c8h], ecx
        mov    dword ptr [esi + 0d4h], eax
        mov    dword ptr [esi + 0a0h], eax
        mov    dword ptr [esi + 0a4h], eax
        lea    edx, [esi + 88h]
        lea    ebx, [esi + 30h]
        mov    dword ptr [esi + 12eh], edx
        mov    dword ptr [esi + 0f4h], ebx
        mov    dword ptr [esi + 9ch], 1
        lea    eax, [esi + 38h]
        mov    dword ptr [esi + 34h], ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 0f8h], eax
        mov    edi, 0ah
        mov    dword ptr [esi + 11ch], ecx
        mov    dword ptr [esi + 98h], 1c200h
        mov    byte ptr [esi + 0aah], 8
        and    byte ptr [esi + 0fch], 1
loc_3_05F3:
        db  068h, 000h, 000h, 000h, 000h
        lea    edx, [esi + 146h]
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        push   dword ptr [esi + 132h]
        push   dword ptr [esi + 0c4h]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0625:
        cmp    eax, 1004h
        mov    ebx, eax
        jne    SHORT loc_3_0631
        dec    edi
        jne    SHORT loc_3_05F3
loc_3_0631:
        test   ebx, ebx
        je     SHORT loc_3_066E
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_063B:
        add    esp, 4
        cmp    ebx, 1005h
        jne    SHORT loc_3_065A
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 0ch], 0fffffff6h
        mov    dword ptr [eax], 0fffffff6h
        xor    eax, eax
        jmp    SHORT loc_3_069E
loc_3_065A:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 0ch], 0fffffffeh
        mov    dword ptr [eax], 0fffffffeh
        xor    eax, eax
        jmp    SHORT loc_3_069E
loc_3_066E:
        mov    eax, dword ptr [esi + 162h]
        push   esi
        mov    dword ptr [esi + 132h], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0680:
        add    esp, 4
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0689:
        add    esp, 4
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 0ch], 0
        mov    dword ptr [eax], 0
        mov    eax, esi
loc_3_069E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_3_06A3:
        push   ebp
        mov    ecx, 0ch
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        lea    esi, [eax + 94h]
        rep movsd
        mov    dword ptr [eax + 0ch], 0
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_3_06CB:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_06DA:
        add    esp, 4
        xor    edi, edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_06E5:
        add    esp, 4
        mov    dword ptr [esi + 34h], edi
        or     byte ptr [esi + 11bh], 1
        push   esi
        mov    dword ptr [esi + 4ch], edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_06FB:
        add    esp, 4
        mov    dword ptr [esi + 0ch], edi
        test   byte ptr [esi + 100h], 80h
        jne    SHORT loc_3_0745
        mov    ebx, dword ptr [esi + 58h]
loc_3_070D:
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0712:
        mov    edi, eax
loc_3_0714:
        cmp    dword ptr [esi + 58h], 0
        jbe    SHORT loc_3_0745
        cli
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0721:
        add    esp, 4
        sti
        mov    eax, dword ptr [esi + 58h]
        cmp    ebx, eax
        je     SHORT loc_3_0730
        mov    ebx, eax
        jmp    SHORT loc_3_070D
loc_3_0730:
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0735:
        sub    eax, edi
        cmp    eax, 7530h
        jbe    SHORT loc_3_0714
        mov    dword ptr [esi + 0ch], 0fffffffeh
loc_3_0745:
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_074A:
        mov    edi, eax
loc_3_074C:
        cli
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0753:
        add    esp, 4
        sti
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_075C:
        sub    eax, edi
        cmp    eax, 0c8h
        jb     SHORT loc_3_074C
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_076B:
        add    esp, 4
        mov    bl, byte ptr [esi + 11bh]
        test   bl, 8
        je     SHORT loc_3_078F
        push   0
        and    bl, 0f7h
        push   dword ptr [esi + 3ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0786:
        add    esp, 8
        mov    byte ptr [esi + 11bh], bl
loc_3_078F:
        mov    bl, byte ptr [esi + 11bh]
        test   bl, 4
        je     SHORT loc_3_07B0
        push   0
        and    bl, 0fbh
        push   dword ptr [esi + 44h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_07A7:
        add    esp, 8
        mov    byte ptr [esi + 11bh], bl
loc_3_07B0:
        or     byte ptr [esi + 11bh], 20h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_07BC:
        push   esi
        call   sub_3_0433
        add    esp, 4
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_3_07CF:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 0f8h]
        mov    dword ptr [ecx + 0ch], 0
        mov    dl, byte ptr [eax]
        mov    eax, dword ptr [ebp + 0ch]
        pop    ebp
        mov    dword ptr [ecx + 0f8h], eax
        mov    byte ptr [eax], dl
        mov    eax, 1
        ret
loc_3_07F7:
        push   ebp
        mov    ecx, 10h
        mov    ebp, esp
        push   esi
        push   edi
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp + 0ch]
        mov    edi, edx
        rep movsd
        mov    ecx, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        pop    ebp
        mov    dword ptr [ecx + 0ch], 0
        mov    eax, dword ptr [ecx + 40h]
        mov    dword ptr [edx + 30h], eax
        mov    eax, dword ptr [ecx + 48h]
        mov    dword ptr [edx + 2ch], eax
        mov    eax, 1
        ret
loc_3_082C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_3_085F
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 58h]
        mov    ecx, dword ptr [edi]
        cli
        xor    eax, eax
        stosd
        stosd
        stosd
        sti
        test   ecx, ecx
        je     SHORT loc_3_087C
        cmp    dword ptr [esi + 0e0h], eax
        je     SHORT loc_3_087C
        push   2
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_085A:
        add    esp, 8
        jmp    SHORT loc_3_087C
loc_3_085F:
        cli
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        lea    edi, [esi + 4ch]
        stosd
        stosd
        stosd
        sti
        and    byte ptr [esi + 0c9h], 0feh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0879:
        add    esp, 4
loc_3_087C:
        mov    eax, 1
        pop    edi
        mov    dword ptr [esi + 0ch], 0
        pop    esi
        pop    ebp
        ret
loc_3_088C:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx + 0f0h], eax
        mov    eax, dword ptr [ebp + 10h]
        test   eax, eax
        je     SHORT loc_3_08AA
        mov    dword ptr [ecx + 0f4h], eax
loc_3_08AA:
        mov    eax, 1
        pop    ebp
        mov    dword ptr [ecx + 0ch], 0
        ret
loc_3_08B8:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [esi + 0c8h]
        mov    dword ptr [esi + 0cch], eax
        and    dh, 0bfh
        mov    dword ptr [esi + 0d0h], ecx
        mov    dword ptr [esi + 0c8h], edx
        test   eax, eax
        je     SHORT loc_3_090B
        cli
        or     dh, 40h
        mov    dword ptr [esi + 0c8h], edx
        sti
        mov    eax, dword ptr [esi + 0f4h]
        mov    ecx, dword ptr [eax]
        test   dword ptr [esi + 0f0h], ecx
        je     SHORT loc_3_090B
        push   4
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0908:
        add    esp, 8
loc_3_090B:
        mov    dword ptr [esi + 0ch], 0
        mov    eax, 1
        pop    esi
        pop    ebp
        ret
loc_3_091A:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        cmp    ecx, -1
        mov    edx, dword ptr [ebp + 8]
        je     SHORT loc_3_0932
        mov    eax, dword ptr [edx + 40h]
        cmp    ecx, eax
        jb     SHORT loc_3_0932
        mov    ecx, eax
loc_3_0932:
        mov    eax, dword ptr [ebp + 10h]
        test   eax, eax
        jne    SHORT loc_3_093E
        mov    ecx, 0ffffffffh
loc_3_093E:
        mov    dword ptr [edx + 0d8h], eax
        mov    dword ptr [edx + 0d4h], ecx
        cmp    dword ptr [edx + 4ch], 0
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [edx + 0dch], eax
        jne    SHORT loc_3_0965
        mov    eax, dword ptr [edx + 12eh]
        mov    dword ptr [eax], 0
loc_3_0965:
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_096A:
        mov    eax, 1
        pop    ebp
        ret
loc_3_0971:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        mov    ebp, esp
        cmp    edx, -1
        mov    ecx, dword ptr [ebp + 8]
        je     SHORT loc_3_0992
        test   byte ptr [ecx + 11bh], 10h
        je     SHORT loc_3_0992
        mov    eax, dword ptr [ecx + 48h]
        cmp    eax, edx
        jae    SHORT loc_3_0992
        mov    edx, eax
loc_3_0992:
        mov    eax, dword ptr [ebp + 10h]
        test   eax, eax
        jne    SHORT loc_3_099B
        xor    edx, edx
loc_3_099B:
        mov    dword ptr [ecx + 0e0h], edx
        mov    dword ptr [ecx + 0e4h], eax
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [ecx + 58h], edx
        mov    dword ptr [ecx + 0e8h], eax
        jae    SHORT loc_3_09BC
        or     byte ptr [ecx + 0c9h], 2
loc_3_09BC:
        mov    eax, 1
        pop    ebp
        ret
loc_3_09C3:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        add    eax, 146h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_09D5:
        xor    ecx, ecx
        mov    cl, al
        mov    eax, dword ptr [ebp + 0ch]
        and    ecx, 0f0h
        pop    ebp
        mov    dword ptr [eax], ecx
        mov    eax, 1
        ret
loc_3_09EB:
        mov    eax, 1
        ret
_PTEXT ends
        end
