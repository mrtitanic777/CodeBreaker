; ircomm_pcode_02.asm -- IRCOMM object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_2_0000:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     SHORT loc_2_000A
        int3
        ret
loc_2_000A:
        push   esi
        push   edi
        push   ebx
        mov    edi, dword ptr [esp + 24h]
        or     edi, edi
        je     NEAR PTR loc_2_0118
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_2_003A
loc_2_0027:
        call   sub_2_016E
        je     NEAR PTR loc_2_0118
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_2_0027
loc_2_003A:
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     NEAR PTR loc_2_0118
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        mov    ebx, eax
        mov    edi, eax
        xor    eax, eax
        mov    ecx, 115h
        cld
        rep stosd
        mov    word ptr [ebx], 94h
        mov    word ptr [ebx + 2], 10ah
        db  0C7h, 043h, 004h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 8], 17h
        mov    byte ptr [ebx + 104h], 96h
        db  08Dh, 035h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi], 0
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        push   3
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   dword ptr [esp + 2ch]
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_2_00BF
        cmp    byte ptr [esi + 2], 0
        je     SHORT loc_2_00BF
        or     byte ptr [ebx + 104h], 1
loc_2_00BF:
        mov    byte ptr [ebx + 105h], 20h
        mov    dword ptr [ebx + 110h], 9
        mov    edi, dword ptr [esp + 24h]
        mov    esi, edi
        xor    eax, eax
        xor    ecx, ecx
        dec    ecx
        repne scasb
        neg    ecx
        dec    ecx
        push   ecx
        push   1
        push   ecx
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    edi, eax
        pop    ecx
        mov    dword ptr [ebx + 0f4h], eax
        or     eax, eax
        je     SHORT loc_2_00FE
        rep movsb
        jmp    SHORT loc_2_0105
loc_2_00FE:
        call   sub_2_011C
        jmp    SHORT loc_2_0118
loc_2_0105:
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [esp + 20h]
        db  0CDh, 020h, 019h, 000h, 02Bh, 000h
        add    esp, 0ch
loc_2_0118:
        pop    ebx
        pop    edi
        pop    esi
        ret
sub_2_011C:
        push   esi
        cmp    dword ptr [ebx + 0f4h], 0
        je     SHORT loc_2_0137
        push   0
        push   dword ptr [ebx + 0f4h]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_2_0137:
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    eax, ebx
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        jb     SHORT loc_2_014D
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
loc_2_014D:
        pop    esi
        ret
        db  0CCh
sub_2_0150:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        or     edi, edi
        je     SHORT loc_2_0168
        mov    esi, dword ptr [ebp + 8]
        mov    eax, esi
        call   sub_2_016E
        je     SHORT loc_2_016A
loc_2_0168:
        xor    eax, eax
loc_2_016A:
        pop    edi
        pop    esi
        leave
        ret
sub_2_016E:
        push   edi
        mov    edx, dword ptr [eax + 0f4h]
        or     edx, edx
        je     SHORT loc_2_0195
        xor    ecx, ecx
loc_2_017B:
        mov    cl, byte ptr [edx]
        mov    ch, byte ptr [edi]
        or     ecx, ecx
        je     SHORT loc_2_0197
        call   sub_2_0199
        xchg   ch, cl
        call   sub_2_0199
        inc    edx
        inc    edi
        cmp    cl, ch
        je     SHORT loc_2_017B
loc_2_0195:
        or     esi, esi
loc_2_0197:
        pop    edi
        ret
sub_2_0199:
        cmp    cl, 41h
        jb     SHORT loc_2_01A6
        cmp    cl, 5ah
        ja     SHORT loc_2_01A6
        or     cl, 20h
loc_2_01A6:
        ret
        db  0CCh
loc_2_01A8:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
loc_2_01BD:
        je     SHORT loc_2_01CE
        call   sub_2_016E
        je     SHORT loc_2_01D8
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jmp    SHORT loc_2_01BD
loc_2_01CE:
        mov    eax, 0fffffff6h
        jmp    NEAR PTR loc_2_0332
loc_2_01D8:
        mov    esi, eax
        mov    eax, 0fffffffeh
        cmp    dword ptr [esi + 64h], 534d5446h
        je     NEAR PTR loc_2_0332
        cmp    dword ptr [esi + 110h], 9
        jne    NEAR PTR loc_2_0332
        xor    ebx, ebx
        mov    dword ptr [esi + 3ch], ebx
        mov    dword ptr [esi + 40h], ebx
        mov    dword ptr [esi + 44h], ebx
        mov    dword ptr [esi + 48h], ebx
        mov    dword ptr [esi + 4ch], ebx
        mov    dword ptr [esi + 50h], ebx
        mov    dword ptr [esi + 54h], ebx
        mov    dword ptr [esi + 58h], ebx
        mov    dword ptr [esi + 5ch], ebx
        mov    dword ptr [esi + 60h], ebx
        mov    byte ptr [esi + 109h], bl
        mov    byte ptr [esi + 10ah], bl
        mov    byte ptr [esi + 105h], 20h
        mov    dword ptr [esi + 0e4h], ebx
        mov    dword ptr [esi + 0cch], ebx
        mov    dword ptr [esi + 0ech], ebx
        mov    dword ptr [esi + 0d8h], ebx
        dec    ebx
        mov    dword ptr [esi + 0e0h], ebx
        mov    dword ptr [esi + 0a0h], ebx
        mov    dword ptr [esi + 0a4h], ebx
        mov    dword ptr [esi + 0e8h], 1
        mov    dword ptr [esi + 9ch], 1
        lea    edi, [esi + 30h]
        mov    dword ptr [esi + 0f0h], edi
        lea    edi, [esi + 38h]
        mov    dword ptr [esi + 0fch], edi
        mov    dword ptr [esi + 94h], 30h
        lea    edi, [esi + 88h]
        mov    dword ptr [esi + 100h], edi
        mov    word ptr [esi + 0adh], 1311h
        xor    ah, ah
        mov    byte ptr [esi + 10eh], ah
        mov    byte ptr [esi + 452h], 0
        mov    byte ptr [esi + 10dh], ah
        push   esi
        mov    esi, dword ptr [esi + 0fch]
        mov    byte ptr [esi], ah
        pop    esi
        mov    byte ptr [esi + 108h], 8
        mov    dword ptr [esi + 98h], 2580h
        mov    byte ptr [esi + 0aah], 8
        mov    byte ptr [esi + 0abh], 0
        mov    byte ptr [esi + 0ach], 0
        mov    byte ptr [esi + 10fh], 3
        or     word ptr [esi + 11eh], 6
        db  0CDh, 020h, 000h, 000h, 001h, 000h
        cmp    eax, 400h
        jae    SHORT loc_2_0314
        push   7d0h
        push   0
        push   7d0h
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0311:
        add    esp, 14h
loc_2_0314:
        mov    dword ptr [esi + 34h], 0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0321:
        add    esp, 4
        cmp    eax, 0
        je     SHORT loc_2_0330
        mov    eax, 0fffffffbh
        jmp    SHORT loc_2_0332
loc_2_0330:
        xor    eax, eax
loc_2_0332:
        mov    ebx, dword ptr [ebp + 10h]
        mov    dword ptr [ebx], eax
        or     eax, eax
        jne    SHORT loc_2_0342
        mov    dword ptr [esi + 0ch], eax
        mov    eax, esi
        jmp    SHORT loc_2_0344
loc_2_0342:
        xor    eax, eax
loc_2_0344:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_2_0349:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        xor    eax, eax
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0ch], eax
        lea    esi, [esi + 94h]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, 0ch
        cld
        rep movsd
        inc    eax
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_2_036E:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        or     byte ptr [esi + 105h], 1
        xor    ecx, ecx
        mov    dword ptr [esi + 34h], ecx
        mov    dword ptr [esi + 4ch], ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_038B:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0391:
        add    esp, 4
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_039A:
        add    esp, 4
        cmp    eax, 0
        je     SHORT loc_2_03A7
        mov    eax, 0fffffffeh
loc_2_03A7:
        mov    dword ptr [esi + 0ch], eax
        test   byte ptr [esi + 105h], 8
        je     SHORT loc_2_03C1
        push   0
        push   dword ptr [esi + 3ch]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_2_03C1:
        test   byte ptr [esi + 105h], 4
        je     SHORT loc_2_03D8
        push   0
        push   dword ptr [esi + 44h]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_2_03D8:
        mov    byte ptr [esi + 105h], 20h
        mov    ebx, esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_03E6:
        call   sub_2_011C
        xor    eax, eax
        inc    eax
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_2_03F3:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        jecxz  loc_2_040B
        mov    dword ptr [edx + 0fch], ecx
        mov    al, byte ptr [edx + 10dh]
        mov    byte ptr [ecx], al
loc_2_040B:
        xor    eax, eax
        mov    dword ptr [edx + 0ch], eax
        inc    eax
        ret
loc_2_0412:
        push   esi
        push   edi
        push   ebx
        mov    edx, dword ptr [esp + 10h]
        test   byte ptr [edx + 120h], 2
        jne    SHORT loc_2_0429
        db  0BEh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_042E
loc_2_0429:
        db  0BEh, 000h, 000h, 000h, 000h
loc_2_042E:
        mov    edi, dword ptr [esp + 14h]
        mov    eax, edi
        mov    ecx, 40h
        cld
        rep movsb
        mov    ecx, dword ptr [edx + 40h]
        mov    dword ptr [eax + 30h], ecx
        mov    ecx, dword ptr [edx + 48h]
        mov    dword ptr [eax + 2ch], ecx
        xor    eax, eax
        mov    dword ptr [edx + 0ch], eax
        inc    eax
        pop    ebx
        pop    edi
        pop    esi
        ret
loc_2_0452:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 0ech], eax
        mov    ecx, dword ptr [ebp + 10h]
        jecxz  loc_2_046C
        mov    dword ptr [edx + 0f0h], ecx
loc_2_046C:
        xor    eax, eax
        mov    dword ptr [edx + 0ch], eax
        inc    eax
        leave
        ret
loc_2_0474:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [edx + 0c4h], ecx
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [edx + 0c8h], eax
        and    byte ptr [edx + 0cdh], 0bfh
        jecxz  loc_2_04BE
        cli
        or     byte ptr [edx + 0cdh], 40h
        mov    ecx, dword ptr [edx + 0f0h]
        mov    eax, dword ptr [edx + 0ech]
        and    eax, dword ptr [ecx]
        sti
        or     eax, eax
        je     SHORT loc_2_04BE
        push   esi
        mov    esi, edx
        mov    eax, 4
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_04BD:
        pop    esi
loc_2_04BE:
        xor    eax, eax
        mov    dword ptr [edx + 0ch], eax
        inc    eax
        ret
loc_2_04C5:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [esi + 0d4h], ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, ecx
        inc    eax
        je     SHORT loc_2_04EF
        test   byte ptr [esi + 105h], 80h
        je     SHORT loc_2_04EF
        cmp    ecx, dword ptr [esi + 40h]
        jbe    SHORT loc_2_04EF
        mov    ecx, dword ptr [esi + 40h]
loc_2_04EF:
        mov    ebx, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 0d0h], ebx
        or     ebx, ebx
        jne    SHORT loc_2_04FF
        or     ecx, 0ffffffffh
loc_2_04FF:
        mov    dword ptr [esi + 0e0h], ecx
        cmp    dword ptr [esi + 4ch], 0
        jne    SHORT loc_2_0517
        mov    ebx, dword ptr [esi + 100h]
        mov    dword ptr [ebx], 0
loc_2_0517:
        mov    ebx, esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_051E:
        xor    eax, eax
        mov    dword ptr [ebx + 0ch], eax
        inc    eax
        pop    ebx
        pop    esi
        leave
        ret
loc_2_0528:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 14h]
        mov    dword ptr [esi + 0dch], ebx
        mov    edx, dword ptr [ebp + 0ch]
        inc    edx
        je     SHORT loc_2_0551
        dec    edx
        test   byte ptr [esi + 105h], 10h
        je     SHORT loc_2_0551
        cmp    edx, dword ptr [esi + 48h]
        jbe    SHORT loc_2_0551
        mov    edx, dword ptr [esi + 48h]
loc_2_0551:
        mov    ebx, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 0d8h], ebx
        or     ebx, ebx
        jne    SHORT loc_2_0560
        xor    edx, edx
loc_2_0560:
        mov    dword ptr [esi + 0e4h], edx
        cmp    dword ptr [esi + 58h], edx
        jae    SHORT loc_2_0572
        or     byte ptr [esi + 0cdh], 2
loc_2_0572:
        xor    eax, eax
        mov    dword ptr [esi + 0ch], eax
        inc    eax
        pop    ebx
        pop    esi
        leave
        ret
loc_2_057C:
        mov    edx, dword ptr [esp + 4]
        mov    al, byte ptr [edx + 10dh]
        and    eax, 0f0h
        mov    edx, dword ptr [esp + 8]
        mov    dword ptr [edx], eax
        or     al, 1
        ret
loc_2_0594:
        push   esi
        push   edi
        push   ebx
        xor    eax, eax
        mov    edx, dword ptr [esp + 18h]
        push   34h
        pop    ecx
        cmp    dword ptr [edx], ecx
        mov    dword ptr [edx], ecx
        jb     SHORT loc_2_05E8
        mov    edx, dword ptr [esp + 14h]
        or     edx, edx
        je     SHORT loc_2_05E8
        mov    dword ptr [edx + 28h], eax
        mov    dword ptr [edx + 2ch], eax
        mov    byte ptr [edx + 30h], al
        mov    al, 34h
        mov    dword ptr [edx], eax
        mov    esi, dword ptr [esp + 10h]
        test   byte ptr [esi + 120h], 2
        jne    SHORT loc_2_05CC
        mov    al, 1
        jmp    SHORT loc_2_05CE
loc_2_05CC:
        mov    al, 2
loc_2_05CE:
        mov    dword ptr [edx + 24h], eax
        mov    word ptr [edx + 4], 100h
        lea    eax, [esi + 94h]
        lea    edx, [edx + 8]
        db  0CDh, 020h, 020h, 000h, 02Bh, 000h
        xor    eax, eax
loc_2_05E8:
        mov    dword ptr [esi + 0ch], eax
        or     al, 1
        pop    ebx
        pop    edi
        pop    esi
        ret
loc_2_05F1:
        sub    esp, 30h
        push   esi
        xor    eax, eax
        mov    esi, dword ptr [esp + 38h]
        mov    dword ptr [esi + 0ch], 0ffffffebh
        cmp    dword ptr [esp + 40h], 34h
        jb     SHORT loc_2_0634
        lea    edx, [esp + 4]
        push   edx
        push   esi
        call   sub_2_0349
        add    esp, 8
        mov    eax, dword ptr [esp + 3ch]
        lea    eax, [eax + 8]
        lea    edx, [esp + 4]
        db  0CDh, 020h, 01Fh, 000h, 02Bh, 000h
        push   -1
        push   edx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0631:
        add    esp, 0ch
loc_2_0634:
        pop    esi
        add    esp, 30h
        ret
loc_2_0639:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0ch]
        test   byte ptr [ecx + 120h], 1
        jne    SHORT loc_2_065F
        test   byte ptr [ecx + 35h], 8
        jne    SHORT loc_2_065F
        test   byte ptr [ecx + 35h], 10h
        je     SHORT loc_2_0667
        mov    dword ptr [eax], 1ch
        jmp    SHORT loc_2_0667
loc_2_065F:
        mov    dword ptr [eax], 15h
        jmp    SHORT loc_2_0667
loc_2_0667:
        or     al, 1
        pop    esi
        ret
_PTEXT ends
        end
