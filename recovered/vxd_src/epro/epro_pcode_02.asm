; epro_pcode_02.asm -- EPRO object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_2_0000:
        push   ebp
        mov    eax, 5
        mov    ebp, esp
        sub    esp, 48h
        mov    word ptr [ebp - 10h], ax
        push   esi
        mov    word ptr [ebp - 0eh], ax
        push   0
        mov    ecx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        push   ecx
        lea    eax, [ebp - 4]
        db  0C7h, 045h, 0F4h, 000h, 000h, 000h, 000h
        push   esi
        push   eax
        db  0E8h, 004h, 022h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        db  089h, 035h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 48h], 3
        mov    byte ptr [ebp - 47h], 0
        db  0C7h, 045h, 0BEh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0C2h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0D2h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0C6h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0CAh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0CEh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0D6h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 10h]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 1ah], edx
        db  0B8h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 16h], ecx
        db  0A3h, 004h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        push   36h
        lea    edx, [ebp - 48h]
        push   edx
        mov    eax, dword ptr [ebp - 4]
        push   0
        lea    ecx, [ebp - 8]
        db  0C7h, 045h, 0DAh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0DEh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0E2h, 000h, 000h, 000h, 000h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        db  0E8h, 07Fh, 021h, 000h, 000h
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_00DB
        push   esi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 069h, 021h, 000h, 000h
        mov    eax, 0c0000001h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_00DB:
        xor    eax, eax
        pop    esi
        mov    esp, ebp
loc_2_00E0:
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_00F0:
        push   ebp
        mov    ecx, 0ah
        mov    ebp, esp
        sub    esp, 48h
        mov    word ptr [ebp - 48h], cx
        push   esi
        mov    word ptr [ebp - 46h], cx
        push   edi
        mov    word ptr [ebp - 40h], cx
        mov    eax, 0ch
        db  0C7h, 045h, 0BCh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0C4h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0CCh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 3eh], cx
        mov    word ptr [ebp - 38h], ax
        db  0C7h, 045h, 0DCh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 36h], ax
        mov    eax, 8
        mov    word ptr [ebp - 30h], ax
        db  0C7h, 045h, 0E4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 2eh], ax
        mov    eax, 0dh
        mov    word ptr [ebp - 28h], cx
        mov    word ptr [ebp - 26h], cx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 20h], ax
        push   ecx
        mov    word ptr [ebp - 1eh], ax
        push   0
        push   25bh
        lea    edx, [ebp - 4]
        push   edx
        db  0E8h, 0A1h, 021h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     SHORT loc_2_0191
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_0191:
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    ecx, 96h
        rep stosd
        stosw
        stosb
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 1c2h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 159h], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 2bh], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 1a6h
        mov    dword ptr [ecx + 1aah], eax
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 1aah]
        mov    dword ptr [eax + 1a6h], edx
        mov    eax, dword ptr [ebp - 4]
        add    eax, 1aeh
        mov    ecx, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 1b2h], eax
        mov    eax, dword ptr [ebp - 4]
        push   esi
        lea    ecx, [ebp - 10h]
        mov    edx, dword ptr [eax + 1b2h]
        push   ecx
        mov    dword ptr [eax + 1aeh], edx
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 08Ch, 021h, 000h, 000h
        cmp    dword ptr [ebp - 8], 0
        push   0
        je     SHORT loc_2_0230
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 001h, 021h, 000h, 000h
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_0230:
        lea    eax, [ebp - 48h]
        mov    ecx, dword ptr [ebp - 10h]
        push   eax
        lea    edx, [ebp - 0ch]
        push   ecx
        lea    eax, [ebp - 8]
        push   edx
        push   eax
        db  0E8h, 05Dh, 021h, 000h, 000h
        cmp    dword ptr [ebp - 8], 0
        mov    ecx, 5
        jne    SHORT loc_2_0256
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [eax + 4]
loc_2_0256:
        mov    dl, 1fh
        mov    eax, dword ptr [ebp - 4]
        sub    dl, cl
        push   1
        mov    byte ptr [eax + 1eeh], dl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 1efh], ecx
        lea    ecx, [ebp - 40h]
        push   ecx
        mov    edx, dword ptr [ebp - 10h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        db  0E8h, 01Fh, 021h, 000h, 000h
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_0297
        mov    eax, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 4]
        mov    word ptr [edx], cx
        jmp    SHORT loc_2_029F
loc_2_0297:
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax], 300h
loc_2_029F:
        push   1
        lea    eax, [ebp - 38h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 0ch]
        push   edx
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 0ECh, 020h, 000h, 000h
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_02CB
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 4]
        mov    word ptr [ecx + 2], dx
        jmp    SHORT loc_2_02D4
loc_2_02CB:
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 2], 0ffffh
loc_2_02D4:
        push   1
        lea    eax, [ebp - 30h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 0ch]
        push   edx
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 0B7h, 020h, 000h, 000h
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_0300
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 4]
        mov    word ptr [ecx + 4], dx
        jmp    SHORT loc_2_0309
loc_2_0300:
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 4], 0ffffh
loc_2_0309:
        push   0
        lea    eax, [ebp - 28h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 0ch]
        push   edx
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 082h, 020h, 000h, 000h
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_0332
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 6], 0
        jmp    SHORT loc_2_033E
loc_2_0332:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 6], edx
loc_2_033E:
        push   0
        lea    eax, [ebp - 20h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 0ch]
        push   edx
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 04Dh, 020h, 000h, 000h
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_0364
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0eh], 0
        jmp    SHORT loc_2_0370
loc_2_0364:
        mov    eax, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp - 4]
        mov    cl, byte ptr [eax + 4]
        mov    byte ptr [edx + 0eh], cl
loc_2_0370:
        mov    eax, dword ptr [ebp - 10h]
        lea    ecx, [ebp - 18h]
        push   eax
        lea    edx, [ebp - 14h]
        push   ecx
        lea    eax, [ebp - 8]
        push   edx
        push   eax
        db  0E8h, 01Fh, 01Fh, 000h, 000h
        cmp    dword ptr [ebp - 18h], 6
        jne    SHORT loc_2_03AA
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_03AA
        mov    eax, dword ptr [ebp - 4]
        push   6
        mov    byte ptr [eax + 2bh], 1
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [ebp - 4]
        push   ecx
        add    eax, 32h
        push   eax
        db  0E8h, 05Ah, 020h, 000h, 000h
loc_2_03AA:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 0F5h, 01Fh, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   esi
        push   ecx
        call   sub_2_0480
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     SHORT loc_2_03D5
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0
        push   eax
        db  0E8h, 04Fh, 01Fh, 000h, 000h
loc_2_03D5:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_03E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   0ah
        mov    esi, dword ptr [ebp + 8]
        push   0ah
        push   esi
        mov    byte ptr [esi + 159h], 1
        db  0E8h, 0D5h, 012h, 000h, 000h
        xor    edx, edx
        mov    al, 0fh
        mov    dx, word ptr [esi]
        push   0eh
        inc    edx
        push   0eh
        out    dx, al
        push   esi
        db  0E8h, 0C2h, 012h, 000h, 000h
        push   64h
        db  0E8h, 003h, 01Fh, 000h, 000h
        xor    edx, edx
        mov    al, 0fh
        mov    dx, word ptr [esi]
        lea    ecx, [esi + 1e2h]
        inc    edx
        out    dx, al
        lea    eax, [ebp - 1]
        push   eax
        push   ecx
        db  0E8h, 0D2h, 01Eh, 000h, 000h
        lea    ecx, [esi + 1cah]
        push   ecx
        db  0E8h, 0D2h, 01Eh, 000h, 000h
        mov    ecx, dword ptr [esi + 1f3h]
        push   ecx
        call   sub_2_1150
        mov    ecx, dword ptr [esi + 1c6h]
        push   ecx
        db  0E8h, 0FEh, 01Dh, 000h, 000h
        mov    ecx, dword ptr [esi + 1bah]
        lea    edx, [esi + 1b6h]
        push   0
        push   0
        mov    eax, dword ptr [edx]
        push   esi
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 4]
        mov    edx, dword ptr [edx]
        mov    dword ptr [edx + 4], eax
        db  0E8h, 0B0h, 01Eh, 000h, 000h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_2_0480:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   eax
        lea    ecx, [ebp - 4]
        push   ebx
        push   26h
        push   ecx
        db  0E8h, 081h, 01Eh, 000h, 000h
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, 1
        mov    dword ptr [eax], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 4], bl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ah], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0eh], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dword ptr [eax + 12h], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 16h], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dx, word ptr [esi]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 1ah], edx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 1eh], 10h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   ecx
        push   edx
        mov    eax, dword ptr [esi + 1c2h]
        push   esi
        lea    ecx, [esi + 1c6h]
        push   eax
        push   ecx
        db  0E8h, 03Fh, 01Dh, 000h, 000h
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_2_0540
        push   ebx
        mov    eax, dword ptr [ebp - 4]
        push   ebx
        push   eax
        db  0E8h, 008h, 01Eh, 000h, 000h
        push   ebx
        mov    eax, dword ptr [esi + 1c6h]
        push   2
        push   2
        push   0aa000002h
        push   eax
        db  0E8h, 0BEh, 01Eh, 000h, 000h
        add    esp, 14h
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_0540:
        push   esi
        db  0E8h, 0FAh, 0FAh, 0FFh, 0FFh
        push   esi
        db  0E8h, 094h, 0FBh, 0FFh, 0FFh
        push   esi
        call   sub_2_0840
        test   al, al
        jne    SHORT loc_2_057D
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0
        push   eax
        db  0E8h, 0C1h, 01Dh, 000h, 000h
        mov    eax, dword ptr [esi + 1c6h]
        push   eax
        db  0E8h, 0E1h, 01Ch, 000h, 000h
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_057D:
        lea    eax, [esi + 1f3h]
        lea    ecx, [esi + 1beh]
        push   eax
        lea    edx, [esi + 32h]
        push   ecx
        mov    dword ptr [esi + 1eah], esi
        mov    dword ptr [esi + 1e2h], 0
        db  0C7h, 086h, 0E6h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        push   edx
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   10h
        call   sub_2_1030
        push   0
        test   al, al
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        jne    SHORT loc_2_05EA
        db  0E8h, 054h, 01Dh, 000h, 000h
        mov    eax, dword ptr [esi + 1c6h]
        push   eax
        db  0E8h, 074h, 01Ch, 000h, 000h
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_05EA:
        db  0E8h, 035h, 01Dh, 000h, 000h
        lea    ecx, [esi + 1b6h]
        db  0C7h, 001h, 000h, 000h, 000h, 000h
        pop    edi
        db  0A1h, 004h, 000h, 000h, 000h
        mov    dword ptr [esi + 1bah], eax
        db  0A1h, 004h, 000h, 000h, 000h
        pop    esi
        pop    ebx
        mov    dword ptr [eax], ecx
        xor    eax, eax
        mov    esp, ebp
        db  089h, 00Dh, 004h, 000h, 000h, 000h
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_2_0620:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 20h]
        cmp    byte ptr [esi + 159h], 0
        je     SHORT loc_2_063F
        mov    eax, 0c0000001h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_063F:
        xor    edx, edx
        mov    eax, dword ptr [ebp + 18h]
        cmp    eax, edx
        je     SHORT loc_2_065C
        mov    ecx, dword ptr [ebp + 14h]
loc_2_064B:
        cmp    dword ptr [ecx], 0
        je     SHORT loc_2_0658
        add    ecx, 4
        inc    edx
        cmp    eax, edx
        ja     SHORT loc_2_064B
loc_2_0658:
        cmp    eax, edx
        jne    SHORT loc_2_0668
loc_2_065C:
        mov    eax, 0c0010019h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_0668:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], edx
        lea    edx, [ebp - 4]
        inc    dword ptr [esi + 13bh]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   ecx
        push   0
        push   47h
        push   edx
        db  0E8h, 097h, 01Ch, 000h, 000h
        test   eax, eax
        je     SHORT loc_2_0697
        mov    eax, 0c000009ah
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_0697:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx + 4], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], edx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 10h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 14h], 1
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, 100h
        mov    byte ptr [ecx + 18h], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], esi
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 43h], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 41h], ax
        mov    word ptr [esi + 1f7h], ax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0ch
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 10h]
        push   ecx
        push   eax
        mov    ecx, dword ptr [eax + 8]
        mov    edx, dword ptr [ecx + 1f3h]
        push   edx
        call   sub_2_11A0
        test   al, al
        jne    SHORT loc_2_0724
        mov    esi, 0c0000001h
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 009h, 01Ch, 000h, 000h
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_0724:
        lea    eax, [esi + 1a6h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [esi + 1aah]
        mov    dword ptr [ecx + 4], edx
        mov    edx, dword ptr [esi + 1aah]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 1aah], eax
        mov    eax, dword ptr [esi + 192h]
        inc    eax
        mov    dword ptr [esi + 192h], eax
        cmp    eax, 1
        jne    SHORT loc_2_077D
        xor    edx, edx
        mov    al, 8
        mov    dx, word ptr [esi]
        push   0ea60h
        add    edx, 3
        add    esi, 1e2h
        out    dx, al
        push   esi
        db  0E8h, 07Dh, 01Bh, 000h, 000h
loc_2_077D:
        xor    esi, esi
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_2_0790:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [esi + 18h], al
        jne    SHORT loc_2_0807
        mov    byte ptr [esi + 18h], 1
        push   eax
        dec    dword ptr [edi + 192h]
        mov    eax, dword ptr [esi + 0ch]
        mov    ecx, dword ptr [edi + 1f3h]
        push   eax
        push   ecx
        call   sub_2_1270
        cmp    eax, 0c001000eh
        jne    SHORT loc_2_07CF
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_2_07CF:
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [esi + 4]
        push   edi
        mov    dword ptr [ecx], eax
        mov    edx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [eax + 4], edx
        dec    dword ptr [esi + 14h]
        db  0E8h, 019h, 0F8h, 0FFh, 0FFh
        push   0
        push   0
        push   esi
        db  0E8h, 033h, 01Bh, 000h, 000h
        cmp    dword ptr [edi + 192h], 0
        jne    SHORT loc_2_0805
        xor    edx, edx
        mov    al, 0fh
        mov    dx, word ptr [edi]
        add    edx, 3
        out    dx, al
loc_2_0805:
        xor    eax, eax
loc_2_0807:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_2_0810:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        db  0E8h, 01Fh, 01Ah, 000h, 000h
        push   0
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   ecx
        db  0E8h, 005h, 01Ah, 000h, 000h
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0840:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        xor    al, al
        push   ebx
        push   esi
        mov    bl, 0ffh
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        push   esi
        out    dx, al
        db  0E8h, 056h, 00Fh, 000h, 000h
        xor    eax, eax
        mov    ecx, dword ptr [esi + 1c6h]
        mov    ax, word ptr [esi]
        push   eax
        push   ecx
        push   esi
        call   sub_2_0AA0
        test   al, al
        jne    SHORT loc_2_0899
        push   0
        mov    eax, dword ptr [esi + 1c6h]
        db  068h, 000h, 000h, 000h, 000h
        push   2
        push   0aa000004h
        push   eax
        db  0E8h, 065h, 01Bh, 000h, 000h
        add    esp, 14h
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0899:
        push   esi
        call   sub_2_0B40
        test   al, al
        jne    SHORT loc_2_08AE
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_08AE:
        cmp    byte ptr [esi + 0dh], 0
        jne    NEAR PTR loc_2_093C
        mov    dx, word ptr [esi]
        mov    al, 80h
        out    dx, al
        push   7
        push   esi
        call   sub_2_0CD0
        mov    word ptr [ebp - 2], ax
        mov    cx, ax
        xor    al, al
        mov    dx, word ptr [esi]
        out    dx, al
        xor    dx, dx
        mov    eax, dword ptr [esi + 1efh]
loc_2_08DC:
        test   cl, 1
        je     SHORT loc_2_08E3
        inc    bl
loc_2_08E3:
        mov    edi, edx
        and    edi, 0ffffh
        cmp    edi, eax
        je     SHORT loc_2_090C
        cmp    dx, 2
        jne    SHORT loc_2_0900
        cmp    eax, 9
        jne    SHORT loc_2_0900
        test   byte ptr [ebp - 2], 4
        jne    SHORT loc_2_090C
loc_2_0900:
        shr    cx, 1
        inc    dx
        cmp    dx, 10h
        jb     SHORT loc_2_08DC
loc_2_090C:
        test   bl, bl
        jl     SHORT loc_2_0915
        cmp    bl, 7
        jle    SHORT loc_2_093C
loc_2_0915:
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   2
        mov    eax, dword ptr [esi + 1c6h]
        push   0aa000005h
        push   eax
        db  0E8h, 0C2h, 01Ah, 000h, 000h
        add    esp, 14h
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_093C:
        xor    edx, edx
        mov    al, 0fh
        mov    dx, word ptr [esi]
        mov    byte ptr [esi + 0ch], bl
        add    edx, 3
        out    dx, al
        cmp    byte ptr [esi + 0dh], 0
        jne    SHORT loc_2_0986
        push   0eh
        push   0eh
        push   esi
        db  0E8h, 076h, 00Dh, 000h, 000h
        test   al, al
        jne    SHORT loc_2_0986
        push   0
        mov    eax, dword ptr [esi + 1c6h]
        db  068h, 000h, 000h, 000h, 000h
        push   2
        push   0aa000003h
        push   eax
        db  0E8h, 078h, 01Ah, 000h, 000h
        add    esp, 14h
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0986:
        mov    dx, word ptr [esi]
        mov    al, 0bh
        out    dx, al
        push   esi
        call   sub_2_0BD0
        push   esi
        db  0E8h, 028h, 00Fh, 000h, 000h
        cmp    ax, 0ffffh
        jne    SHORT loc_2_09C6
        push   0
        mov    eax, dword ptr [esi + 1c6h]
        db  068h, 000h, 000h, 000h, 000h
        push   2
        push   0aa000003h
        push   eax
        db  0E8h, 038h, 01Ah, 000h, 000h
        add    esp, 14h
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_09C6:
        test   ax, ax
        je     SHORT loc_2_09DD
        mov    byte ptr [esi + 10h], 1
        mov    word ptr [esi + 11h], 2000h
        mov    word ptr [esi + 13h], 6000h
        jmp    SHORT loc_2_09ED
loc_2_09DD:
        mov    byte ptr [esi + 10h], 0
        mov    word ptr [esi + 11h], 3000h
        mov    word ptr [esi + 13h], 0d000h
loc_2_09ED:
        lea    edi, [esi + 1cah]
        push   1
        push   0
        mov    eax, dword ptr [esi + 1efh]
        push   eax
        mov    ecx, dword ptr [esi + 1c6h]
        push   eax
        lea    edx, [ebp - 8]
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        push   edi
        push   edx
        db  0E8h, 0EBh, 018h, 000h, 000h
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_0A64
        push   esi
        db  0E8h, 0A9h, 009h, 000h, 000h
        test   al, al
        jne    SHORT loc_2_0A59
        push   0
        mov    eax, dword ptr [esi + 1c6h]
        db  068h, 000h, 000h, 000h, 000h
        push   2
        push   0aa000003h
        push   eax
        db  0E8h, 0ABh, 019h, 000h, 000h
        add    esp, 14h
        push   edi
        db  0E8h, 0BEh, 018h, 000h, 000h
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0A59:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0A64:
        mov    eax, dword ptr [esi + 1efh]
        mov    ecx, dword ptr [esi + 1c6h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   2
        push   0aa000005h
        push   ecx
        db  0E8h, 06Dh, 019h, 000h, 000h
        add    esp, 14h
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0AA0:
        push   ebp
        xor    cl, cl
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        mov    ebx, 0ech
        lea    edx, [esi + 2]
loc_2_0AB6:
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    byte ptr [ebp - 1], bl
        cmp    byte ptr [ebp - 1], 24h
        je     SHORT loc_2_0ACA
        inc    cl
        cmp    cl, 4
        jb     SHORT loc_2_0AB6
loc_2_0ACA:
        mov    bl, 40h
        xor    cl, cl
loc_2_0ACE:
        in     al, dx
        mov    edi, ebx
        mov    byte ptr [ebp - 1], al
        and    edi, 0ffh
        and    eax, 0ech
        sub    edi, eax
        cmp    edi, -24h
        jne    SHORT loc_2_0B2F
        add    bl, 40h
        inc    cl
        cmp    cl, 3
        jb     SHORT loc_2_0ACE
        mov    edi, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [edi]
        mov    al, 80h
        mov    edx, ecx
        out    dx, al
        mov    byte ptr [edi + 0dh], 0
        lea    edx, [esi + 0ah]
        in     al, dx
        mov    dl, al
        mov    byte ptr [ebp - 1], al
        shr    dl, 5
        mov    byte ptr [edi + 0fh], dl
        lea    edx, [esi + 1]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        test   al, 10h
        je     SHORT loc_2_0B1F
        mov    byte ptr [edi + 0dh], 1
loc_2_0B1F:
        mov    edx, ecx
        xor    al, al
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_0B2F:
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_0B40:
        push   ebp
        mov    al, 80h
        mov    ebp, esp
        push   esi
        push   edi
        push   0
        mov    edi, dword ptr [ebp + 8]
        mov    dx, word ptr [edi]
        push   edi
        out    dx, al
        call   sub_2_0CD0
        and    ax, 0fc3fh
        shr    ax, 6
        mov    si, word ptr [edi]
        cmp    ax, si
        je     SHORT loc_2_0BC1
        shl    si, 6
        push   0
        push   edi
        call   sub_2_0CD0
        and    ax, 3ffh
        or     ax, si
        push   eax
        push   0
        push   edi
        call   sub_2_0ED0
        push   edi
        call   sub_2_0C90
        mov    dx, word ptr [edi]
        xor    al, al
        out    dx, al
        push   0eh
        push   0eh
        push   edi
        db  0E8h, 038h, 00Bh, 000h, 000h
        test   al, al
        jne    SHORT loc_2_0BC1
        push   0
        mov    eax, dword ptr [edi + 1c6h]
        db  068h, 000h, 000h, 000h, 000h
        push   2
        push   0aa000003h
        push   eax
        db  0E8h, 03Ah, 018h, 000h, 000h
        add    esp, 14h
        xor    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_2_0BC1:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0BD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 2bh], 0
        jne    NEAR PTR loc_2_0C6C
        mov    al, byte ptr [esi + 0dh]
        test   al, al
        jne    SHORT loc_2_0C50
        mov    dx, word ptr [esi]
        mov    al, 80h
        out    dx, al
        xor    di, di
loc_2_0BF5:
        mov    ax, di
        add    ax, 2
        push   eax
        push   esi
        call   sub_2_0CD0
        mov    ch, al
        mov    edx, edi
        inc    di
        mov    cl, ah
        lea    eax, [ebp - 4]
        and    edx, 0ffffh
        add    edx, edx
        sub    eax, edx
        mov    word ptr [eax], cx
        cmp    di, 3
        jb     SHORT loc_2_0BF5
        mov    al, byte ptr [ebp - 8]
        mov    cl, byte ptr [ebp - 6]
        mov    byte ptr [esi + 32h], al
        mov    byte ptr [esi + 34h], cl
        mov    ax, word ptr [ebp - 8]
        mov    cx, word ptr [ebp - 6]
        mov    byte ptr [esi + 33h], ah
        mov    dx, word ptr [esi]
        mov    al, byte ptr [ebp - 4]
        mov    byte ptr [esi + 35h], ch
        mov    byte ptr [esi + 36h], al
        mov    ax, word ptr [ebp - 4]
        mov    byte ptr [esi + 37h], ah
        xor    al, al
        out    dx, al
        jmp    SHORT loc_2_0C6C
loc_2_0C50:
        cmp    al, 1
        jne    SHORT loc_2_0C6C
        mov    byte ptr [esi + 33h], 0aah
        mov    byte ptr [esi + 35h], 12h
        mov    byte ptr [esi + 36h], 34h
        mov    byte ptr [esi + 37h], 56h
        xor    eax, eax
        mov    byte ptr [esi + 32h], al
        mov    byte ptr [esi + 34h], al
loc_2_0C6C:
        lea    eax, [esi + 32h]
        push   6
        push   eax
        add    esi, 2ch
        push   esi
        db  0E8h, 089h, 017h, 000h, 000h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0C90:
        push   ebp
        mov    ebp, esp
        push   ebx
        xor    bx, bx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
loc_2_0C9E:
        push   ebx
        push   esi
        inc    bx
        call   sub_2_0CD0
        add    di, ax
        cmp    bx, 3fh
        jb     SHORT loc_2_0C9E
        mov    ax, 0babah
        sub    ax, di
        push   eax
        push   3fh
        push   esi
        call   sub_2_0ED0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0CD0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dx, word ptr [edi]
        add    edx, 0ah
        push   3
        in     al, dx
        push   6
        mov    byte ptr [ebp - 2], al
        and    word ptr [ebp - 2], 0fff2h
        or     byte ptr [ebp - 2], 2
        mov    al, byte ptr [ebp - 2]
        push   edi
        out    dx, al
        call   sub_2_0D30
        mov    cx, word ptr [ebp + 0ch]
        push   6
        push   ecx
        push   edi
        call   sub_2_0D30
        push   edi
        call   sub_2_0E20
        mov    si, ax
        push   edi
        call   sub_2_0E90
        mov    ax, si
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0D30:
        push   ebp
        mov    cx, word ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 4
        dec    cl
        xor    edx, edx
        push   ebx
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        mov    dx, word ptr [ebx]
        push   edi
        mov    di, 1
        add    edx, 0ah
        in     al, dx
        mov    si, word ptr [ebp + 0ch]
        mov    byte ptr [ebp - 2], al
        shl    di, cl
        and    word ptr [ebp - 2], 0fff3h
loc_2_0D60:
        and    word ptr [ebp - 2], 0fffbh
        test   si, di
        je     SHORT loc_2_0D6F
        or     byte ptr [ebp - 2], 4
loc_2_0D6F:
        shr    di, 1
        xor    edx, edx
        mov    dx, word ptr [ebx]
        mov    al, byte ptr [ebp - 2]
        add    edx, 0ah
        push   64h
        out    dx, al
        db  0E8h, 092h, 015h, 000h, 000h
        lea    eax, [ebp - 2]
        push   eax
        push   ebx
        call   sub_2_0DC0
        lea    eax, [ebp - 2]
        push   eax
        push   ebx
        call   sub_2_0DF0
        test   di, di
        jne    SHORT loc_2_0D60
        xor    edx, edx
        pop    edi
        mov    dx, word ptr [ebx]
        pop    esi
        add    edx, 0ah
        pop    ebx
        and    word ptr [ebp - 2], 0fffbh
        mov    al, byte ptr [ebp - 2]
        mov    esp, ebp
        out    dx, al
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_0DC0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ax, word ptr [ecx]
        push   64h
        or     al, 1
        mov    word ptr [ecx], ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx]
        add    edx, 0ah
        out    dx, al
        db  0E8h, 036h, 015h, 000h, 000h
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0DF0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ax, word ptr [ecx]
        push   64h
        and    ax, 0fffeh
        mov    word ptr [ecx], ax
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx]
        add    edx, 0ah
        out    dx, al
        db  0E8h, 004h, 015h, 000h, 000h
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0E20:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        xor    bx, bx
        add    edx, 0ah
        in     al, dx
        mov    di, 10h
        mov    byte ptr [ebp - 2], al
        and    word ptr [ebp - 2], 0fff3h
loc_2_0E45:
        add    bx, bx
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_2_0DC0
        xor    edx, edx
        mov    dx, word ptr [esi]
        add    edx, 0ah
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    word ptr [ebp - 2], 0fffbh
        test   byte ptr [ebp - 2], 8
        je     SHORT loc_2_0E6D
        or     bl, 1
loc_2_0E6D:
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_2_0DF0
        dec    di
        jne    SHORT loc_2_0E45
        mov    ax, bx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0E90:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        add    edx, 0ah
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    word ptr [ebp - 2], 0fff9h
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_2_0DC0
        lea    ecx, [ebp - 2]
        push   ecx
        push   esi
        call   sub_2_0DF0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_2_0ED0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        add    edx, 0ah
        push   5
        in     al, dx
        push   13h
        mov    byte ptr [ebp - 2], al
        and    word ptr [ebp - 2], 0fff2h
        or     byte ptr [ebp - 2], 2
        mov    al, byte ptr [ebp - 2]
        push   esi
        out    dx, al
        call   sub_2_0D30
        mov    di, word ptr [ebp + 0ch]
        push   4
        push   edi
        push   esi
        call   sub_2_0D30
        push   esi
        call   sub_2_0FF0
        push   3
        push   7
        push   esi
        call   sub_2_0D30
        push   6
        push   edi
        push   esi
        call   sub_2_0D30
        push   esi
        call   sub_2_0F90
        test   ax, ax
        je     SHORT loc_2_0F80
        push   esi
        call   sub_2_0FF0
        push   3
        push   5
        push   esi
        call   sub_2_0D30
        push   6
        push   edi
        push   esi
        call   sub_2_0D30
        mov    ax, word ptr [ebp + 10h]
        push   10h
        push   eax
        push   esi
        call   sub_2_0D30
        push   esi
        call   sub_2_0F90
        test   ax, ax
        je     SHORT loc_2_0F80
        push   esi
        call   sub_2_0FF0
        push   5
        push   10h
        push   esi
        call   sub_2_0D30
        push   4
        push   edi
        push   esi
        call   sub_2_0D30
        push   esi
        call   sub_2_0E90
loc_2_0F80:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0F90:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    di, di
        push   esi
        mov    ebx, 8
        call   sub_2_0FF0
loc_2_0FAA:
        xor    edx, edx
        mov    dx, word ptr [esi]
        add    edx, 0ah
        in     al, dx
        mov    byte ptr [ebp - 2], al
        test   word ptr [ebp - 2], bx
        jne    SHORT loc_2_0FD8
        inc    di
        push   64h
        db  0E8h, 053h, 013h, 000h, 000h
        cmp    di, 0c8h
        jb     SHORT loc_2_0FAA
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0FD8:
        mov    ax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0FF0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi]
        push   4
        add    edx, 0ah
        in     al, dx
        mov    byte ptr [ebp - 2], al
        and    word ptr [ebp - 2], 0fffch
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        db  0E8h, 001h, 013h, 000h, 000h
        or     byte ptr [ebp - 2], 2
        xor    edx, edx
        mov    al, byte ptr [ebp - 2]
        mov    dx, word ptr [esi]
        pop    esi
        add    edx, 0ah
        mov    esp, ebp
        out    dx, al
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_2_1030:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   32h
        push   eax
        db  0E8h, 0D1h, 012h, 000h, 000h
        mov    ecx, dword ptr [ebp + 20h]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     SHORT loc_2_1064
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_1064:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
        stosw
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    SHORT loc_2_107E
        mov    esi, 2
loc_2_107E:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 08Dh, 012h, 000h, 000h
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     SHORT loc_2_10B2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_1150
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_10B2:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        db  0E8h, 055h, 012h, 000h, 000h
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
        je     SHORT loc_2_10E1
        push   eax
        call   sub_2_1150
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_10E1:
        mov    dword ptr [eax + 28h], 0ffffffffh
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [eax + 2ch], edx
        mov    dl, byte ptr [ecx + 4]
        mov    byte ptr [eax + 30h], dl
        add    eax, 2ch
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [eax + 5], cl
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 14h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 18h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 20h], esi
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_1150:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_2_1170
        mov    ecx, dword ptr [esi + 20h]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        db  0E8h, 0B4h, 011h, 000h, 000h
loc_2_1170:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     SHORT loc_2_1186
        push   0
        mov    ecx, dword ptr [esi + 20h]
        shl    ecx, 3
        push   ecx
        push   eax
        db  0E8h, 09Eh, 011h, 000h, 000h
loc_2_1186:
        push   0
        push   32h
        push   esi
        db  0E8h, 094h, 011h, 000h, 000h
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_11A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 28h], 0
        jne    SHORT loc_2_11B9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_11B9:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   0
        push   1ah
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 052h, 011h, 000h, 000h
        or     eax, eax
        je     SHORT loc_2_11D9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_11D9:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 28h]
        mov    edi, edi
loc_2_11E0:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    SHORT loc_2_11F3
        inc    ecx
        cmp    ecx, 20h
        jb     SHORT loc_2_11E0
        jmp    SHORT loc_2_1202
loc_2_11F3:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 28h], eax
loc_2_1202:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 10h]
        mov    dword ptr [eax + 10h], ecx
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_2_1218
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], ecx
loc_2_1218:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [ebp - 8]
        mov    byte ptr [eax + 19h], cl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], 1
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        mov    byte ptr [eax + 18h], 0
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    al, 1
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
sub_2_1270:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_2_1610
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_2_1299
        cmp    edi, 103h
        jne    SHORT loc_2_12B0
loc_2_1299:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   esi
        push   ebx
        call   sub_2_1330
        or     eax, eax
        je     SHORT loc_2_12B0
        mov    edi, eax
loc_2_12B0:
        or     edi, edi
        je     SHORT loc_2_12BC
        cmp    edi, 103h
        jne    SHORT loc_2_131B
loc_2_12BC:
        mov    eax, dword ptr [esi + 0ch]
        dec    eax
        mov    dword ptr [esi + 0ch], eax
        jne    SHORT loc_2_1316
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_2_12D2
        mov    ecx, dword ptr [esi + 14h]
        mov    dword ptr [eax + 14h], ecx
loc_2_12D2:
        mov    eax, dword ptr [esi + 14h]
        or     eax, eax
        je     SHORT loc_2_12E1
        mov    ecx, dword ptr [esi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_2_12E7
loc_2_12E1:
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [ebx + 10h], eax
loc_2_12E7:
        cmp    byte ptr [esi + 18h], 0
        je     SHORT loc_2_12F6
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 08Ah, 00Fh, 000h, 000h
loc_2_12F6:
        mov    eax, 1
        mov    cl, byte ptr [esi + 19h]
        shl    eax, cl
        or     dword ptr [ebx + 28h], eax
        push   0
        push   1ah
        push   esi
        db  0E8h, 017h, 010h, 000h, 000h
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_2_1316:
        mov    edi, 0c001000eh
loc_2_131B:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1330:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [esi + 20h]
        lea    ecx, [eax + eax*2]
        add    ecx, ecx
        add    ecx, dword ptr [esi + 4]
        mov    dword ptr [ebp - 20h], ecx
        mov    ecx, dword ptr [esi + 8]
        lea    eax, [ecx + eax*4]
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [esi + 24h]
        shl    eax, 2
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        db  0E8h, 0B9h, 010h, 000h, 000h
        mov    eax, dword ptr [esi + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        sub    edi, edi
        push   eax
        db  0E8h, 0A1h, 010h, 000h, 000h
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    eax, edi
        je     SHORT loc_2_13B4
        sub    eax, eax
        nop
loc_2_1390:
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 1ch], ecx
        mov    edx, dword ptr [ebp + 0ch]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 19h]
        shl    ebx, cl
        not    ebx
        mov    ecx, dword ptr [ebp - 1ch]
        and    dword ptr [eax + ecx], ebx
        add    eax, 4
        inc    edi
        cmp    dword ptr [esi + 24h], edi
        ja     SHORT loc_2_1390
loc_2_13B4:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 24h], 0
        jbe    SHORT loc_2_141F
loc_2_13C1:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    SHORT loc_2_1414
        lea    ecx, [ecx + ecx*2]
loc_2_13D0:
        add    ecx, ecx
        mov    eax, dword ptr [esi + 24h]
        sub    eax, dword ptr [ebp - 4]
        add    ecx, dword ptr [esi + 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        sub    eax, 6
        push   eax
        lea    eax, [ecx + 6]
        push   eax
        push   ecx
        db  0E8h, 036h, 010h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        shl    ecx, 2
        add    ecx, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 24h]
        sub    eax, dword ptr [ebp - 4]
        lea    eax, [eax*4 - 4]
        push   eax
        lea    eax, [ecx + 4]
        push   eax
        push   ecx
        db  0E8h, 015h, 010h, 000h, 000h
        dec    dword ptr [esi + 24h]
        jmp    SHORT loc_2_1417
loc_2_1414:
        inc    dword ptr [ebp - 4]
loc_2_1417:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 24h], eax
        ja     SHORT loc_2_13C1
loc_2_141F:
        mov    dword ptr [ebp - 10h], 0
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_2_150A
        mov    edi, dword ptr [ebp + 18h]
loc_2_1433:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        call   sub_2_17F0
        or     al, al
        je     SHORT loc_2_146A
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 18h], ecx
        mov    edx, dword ptr [ebp + 0ch]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 19h]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 18h]
        or     dword ptr [ecx + eax*4], ebx
        jmp    NEAR PTR loc_2_14F8
loc_2_146A:
        mov    ecx, dword ptr [esi + 24h]
        cmp    dword ptr [esi + 20h], ecx
        jbe    NEAR PTR loc_2_1544
        mov    eax, dword ptr [ebp - 4]
        sub    ecx, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        add    eax, dword ptr [esi + 4]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        push   ecx
        push   eax
        add    eax, 6
        push   eax
        db  0E8h, 090h, 00Fh, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        add    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [eax], ecx
        mov    cl, byte ptr [edi + 4]
        mov    byte ptr [eax + 4], cl
        mov    cl, byte ptr [edi + 5]
        mov    byte ptr [eax + 5], cl
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        add    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [esi + 24h]
        sub    ecx, dword ptr [ebp - 4]
        shl    ecx, 2
        push   ecx
        push   eax
        add    eax, 4
        push   eax
        db  0E8h, 058h, 00Fh, 000h, 000h
        mov    edx, dword ptr [esi + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + eax*4], 0
        mov    ebx, 1
        mov    edx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 14h], edx
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [edx + 19h]
        mov    edx, dword ptr [ebp - 14h]
        shl    ebx, cl
        or     dword ptr [edx + eax*4], ebx
        inc    dword ptr [esi + 24h]
loc_2_14F8:
        add    edi, 6
        inc    dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp + 14h], eax
        ja     NEAR PTR loc_2_1433
loc_2_150A:
        mov    eax, dword ptr [esi + 24h]
        sub    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], eax
        sub    edx, edx
        cmp    dword ptr [ebp - 0ch], edx
        je     SHORT loc_2_1595
        mov    edi, dword ptr [ebp - 20h]
        sub    ecx, ecx
        nop
loc_2_1520:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_159B
        mov    eax, dword ptr [esi + 4]
        add    eax, ecx
        mov    ebx, dword ptr [edi + 2]
        cmp    dword ptr [eax + 2], ebx
        jne    SHORT loc_2_1582
        mov    bx, word ptr [edi]
        cmp    word ptr [eax], bx
        jne    SHORT loc_2_1582
        mov    dword ptr [ebp - 8], 0
        jmp    SHORT loc_2_1589
loc_2_1544:
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 0CAh, 00Eh, 000h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        db  0E8h, 0B6h, 00Eh, 000h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 24h], eax
        mov    eax, 0c0010009h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_2_1582:
        mov    dword ptr [ebp - 8], 1
loc_2_1589:
        add    edi, 6
        add    ecx, 6
        inc    edx
        cmp    dword ptr [ebp - 0ch], edx
        ja     SHORT loc_2_1520
loc_2_1595:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_1600
loc_2_159B:
        mov    al, byte ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [edx]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   dword ptr [esi + 14h]
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_2_1602
        cmp    edi, 103h
        je     SHORT loc_2_1602
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 03Fh, 00Eh, 000h, 000h
        push   ebx
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 031h, 00Eh, 000h, 000h
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Bh, 0FFh
loc_2_1600:
        sub    edi, edi
loc_2_1602:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 049h, 000h
sub_2_1610:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [edi + 0ch]
        mov    ecx, dword ptr [ebx + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ebx + 8], eax
        mov    eax, dword ptr [edi + 10h]
        or     eax, eax
        mov    dword ptr [edi + 0ch], 0
        je     SHORT loc_2_164D
        lea    esp, [esp]
loc_2_1640:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [edi + 0ch], ecx
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_2_1640
loc_2_164D:
        mov    ecx, dword ptr [edi + 0ch]
        cmp    esi, ecx
        je     SHORT loc_2_1681
        mov    al, byte ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebx]
        push   eax
        push   ecx
        push   esi
        call   dword ptr [edi + 18h]
        or     eax, eax
        je     SHORT loc_2_1683
        cmp    eax, 103h
        je     SHORT loc_2_1683
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 8], ecx
        mov    dword ptr [edi + 0ch], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_1681:
        sub    eax, eax
loc_2_1683:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h
sub_2_1690:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        sub    eax, eax
        mov    cl, byte ptr [ecx + 19h]
        mov    edx, dword ptr [ebp + 8]
        and    ecx, 0ffh
        mov    edi, dword ptr [edx + 24h]
        cmp    edi, eax
        je     SHORT loc_2_16C2
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_2_16B7:
        test   dword ptr [esi], edx
        je     SHORT loc_2_16BC
        inc    eax
loc_2_16BC:
        add    esi, 4
        dec    edi
        jne    SHORT loc_2_16B7
loc_2_16C2:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_16D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 10h]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 19h]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebx + 24h], 0
        jbe    SHORT loc_2_174C
        mov    edi, dword ptr [ebp + 1ch]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 10h], eax
        mov    edi, edi
loc_2_1710:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 10h]
        test   dword ptr [ecx + esi], eax
        je     SHORT loc_2_173A
        cmp    dword ptr [ebp + 14h], 6
        jb     SHORT loc_2_1766
        sub    dword ptr [ebp + 14h], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        db  0E8h, 0F0h, 00Ch, 000h, 000h
        add    edi, 6
        inc    dword ptr [ebp - 0ch]
loc_2_173A:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 24h], eax
        ja     SHORT loc_2_1710
loc_2_174C:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [eax], ecx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_2_1766:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    dword ptr [eax], 0c0000001h
        mov    eax, dword ptr [ebp + 18h]
        pop    esi
        mov    dword ptr [eax], 0
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1790:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 24h]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 10h]
        jbe    SHORT loc_2_17C0
        mov    dword ptr [eax], 0c0000001h
        mov    eax, dword ptr [ebp + 14h]
        pop    ebp
loc_2_17B0:
        mov    dword ptr [eax], 0
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_17C0:
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [edx + 24h]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [edx + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [edx + 4]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        db  0E8h, 040h, 00Ch, 000h, 000h
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_17F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    ecx, ecx
        mov    esi, dword ptr [ebp + 8]
        mov    edx, esi
        shr    edx, 1
        cmp    esi, ecx
        je     NEAR PTR loc_2_1889
        dec    esi
        cmp    esi, edx
        jb     SHORT loc_2_1889
        nop
loc_2_1810:
        cmp    edx, ecx
        jb     SHORT loc_2_1889
        mov    eax, dword ptr [ebp + 0ch]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 10h]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    SHORT loc_2_1833
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_2_1867
loc_2_1833:
        jbe    SHORT loc_2_1840
        mov    dword ptr [ebp - 4], 0ffffffffh
        jmp    SHORT loc_2_1867
        db  08Bh, 0FFh
loc_2_1840:
        mov    eax, dword ptr [ebp + 10h]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    SHORT loc_2_1857
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_2_1867
loc_2_1857:
        mov    dword ptr [ebp - 4], 0ffffffffh
        ja     SHORT loc_2_1867
        mov    dword ptr [ebp - 4], 0
loc_2_1867:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_1899
        jle    SHORT loc_2_1878
        or     edx, edx
        je     SHORT loc_2_1889
        lea    esi, [edx - 1]
        jmp    SHORT loc_2_187B
loc_2_1878:
        lea    ecx, [edx + 1]
loc_2_187B:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    SHORT loc_2_1810
loc_2_1889:
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_1899:
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        mov    dword ptr [eax], edx
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
_PTEXT ends
        end
