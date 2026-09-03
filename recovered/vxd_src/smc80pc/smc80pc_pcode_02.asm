; smc80pc_pcode_02.asm -- SMC80PC object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_2_0000:
        push   ebp
        mov    eax, 8
        mov    ebp, esp
        sub    esp, 18h
        mov    word ptr [ebp - 18h], ax
        push   ebx
        mov    word ptr [ebp - 16h], ax
        xor    ebx, ebx
        mov    word ptr [ebp - 10h], bx
        push   ebx
        mov    word ptr [ebp - 0eh], bx
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        lea    eax, [ebp - 10h]
        push   eax
        lea    edx, [ebp - 8]
        db  0C7h, 045h, 0ECh, 000h, 000h, 000h, 000h
        push   edx
        db  0E8h, 0FBh, 019h, 000h, 000h
        lea    ecx, [ebp - 10h]
        db  089h, 00Dh, 04Ah, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 8]
        db  089h, 015h, 004h, 000h, 000h, 000h
        db  088h, 01Dh, 04Eh, 000h, 000h, 000h
        db  089h, 01Dh, 03Eh, 000h, 000h, 000h
        db  089h, 01Dh, 042h, 000h, 000h, 000h
        db  0C6h, 005h, 008h, 000h, 000h, 000h, 003h
        db  0C6h, 005h, 009h, 000h, 000h, 000h, 00Ah
        db  089h, 01Dh, 00Ah, 000h, 000h, 000h
        db  0C7h, 005h, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 012h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 016h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 01Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 01Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 14h]
        db  0C7h, 005h, 022h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 026h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 02Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 02Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 032h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        push   36h
        db  0A3h, 036h, 000h, 000h, 000h
        db  068h, 008h, 000h, 000h, 000h
        lea    eax, [ebp - 4]
        db  068h, 000h, 000h, 000h, 000h
        db  089h, 00Dh, 03Ah, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 8]
        push   edx
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 03Ch, 019h, 000h, 000h
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_2_011B
        push   ebx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 027h, 019h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_011B:
        mov    eax, dword ptr [ebp - 4]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0130:
        push   ebp
        mov    eax, 0dh
        mov    ebp, esp
        sub    esp, 84h
        mov    word ptr [ebp - 3ch], ax
        push   ebx
        mov    word ptr [ebp - 3ah], ax
        push   esi
        push   edi
        mov    eax, 4
        mov    word ptr [ebp - 34h], ax
        xor    ebx, ebx
        mov    word ptr [ebp - 32h], ax
        mov    byte ptr [ebp - 1], bl
        mov    byte ptr [ebp - 2ch], bl
        mov    byte ptr [ebp - 2bh], bl
        mov    byte ptr [ebp - 2ah], bl
        mov    byte ptr [ebp - 29h], bl
        mov    esi, 10h
        mov    byte ptr [ebp - 28h], bl
        mov    eax, dword ptr [ebp + 0ch]
        lea    ecx, [ebp - 10h]
        push   eax
        lea    edx, [ebp - 0ch]
        push   ecx
        mov    byte ptr [ebp - 27h], bl
        db  0C7h, 045h, 0C8h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0D0h, 000h, 000h, 000h, 000h
        push   edx
        db  0E8h, 00Bh, 01Ah, 000h, 000h
        cmp    dword ptr [ebp - 0ch], ebx
        je     SHORT loc_2_01A4
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_01A4:
        push   0
        lea    eax, [ebp - 3ch]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 14h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 0E7h, 019h, 000h, 000h
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_01C7
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [eax + 4]
loc_2_01C7:
        push   0
        lea    eax, [ebp - 34h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 14h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 0C4h, 019h, 000h, 000h
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_01EF
        mov    eax, dword ptr [ebp - 14h]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 4], ax
loc_2_01EF:
        lea    edi, [ebp - 5ah]
        xor    eax, eax
        mov    word ptr [ebp - 5ch], 0
        mov    ecx, 7
        rep stosd
        stosw
        lea    eax, [ebp - 1ch]
        lea    ecx, [ebp - 20h]
        push   eax
        lea    edx, [ebp - 5ch]
        push   ecx
        mov    eax, dword ptr [ebp + 0ch]
        push   20h
        lea    ecx, [ebp - 0ch]
        push   edx
        push   1
        push   eax
        push   ecx
        db  0E8h, 0CCh, 018h, 000h, 000h
        add    esp, 1ch
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_2_0237
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_0237:
        mov    eax, dword ptr [ebp - 58h]
        mov    ecx, dword ptr [ebp - 48h]
        mov    edx, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 20h], eax
        push   edx
        lea    eax, [ebp - 24h]
        push   eax
        lea    edx, [ebp - 0ch]
        mov    dword ptr [ebp - 1ch], ecx
        lea    ecx, [ebp - 18h]
        push   ecx
        push   edx
        db  0E8h, 04Ch, 018h, 000h, 000h
        cmp    dword ptr [ebp - 24h], 6
        jne    SHORT loc_2_027A
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_027A
        mov    ecx, dword ptr [ebp - 18h]
        mov    eax, ecx
        mov    edx, dword ptr [ecx]
        mov    dword ptr [ebp - 2ch], edx
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [ebp - 28h], al
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [ebp - 27h], cl
loc_2_027A:
        xor    edi, edi
loc_2_027C:
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 8]
        push   eax
        push   edi
        push   2a2h
        push   ecx
        db  0E8h, 08Dh, 018h, 000h, 000h
        mov    dword ptr [ebp - 0ch], eax
        cmp    eax, edi
        je     SHORT loc_2_02A3
        mov    byte ptr [ebp - 1], 1
        mov    ebx, 0aa000002h
        jmp    SHORT loc_2_027C
loc_2_02A3:
        mov    edi, dword ptr [ebp - 8]
        xor    eax, eax
        mov    ecx, 0a8h
        rep stosd
        stosw
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 20h]
        mov    edx, dword ptr [ebp - 1ch]
        lea    edi, [ebp - 84h]
        mov    dword ptr [ecx + 22h], eax
        mov    ecx, dword ptr [ebp - 8]
        mov    ax, word ptr [ebp - 4]
        mov    dword ptr [ecx + 26h], edx
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 14h], ax
        xor    eax, eax
        mov    ecx, 9
        lea    edx, [ebp - 84h]
        rep stosd
        stosw
        push   edx
        mov    ecx, dword ptr [ebp + 10h]
        push   ecx
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   edx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   ecx
        lea    eax, [edx + 5eh]
        mov    dword ptr [ebp - 7ah], 1
        push   eax
        db  0E8h, 041h, 017h, 000h, 000h
        mov    dword ptr [ebp - 0ch], eax
        test   eax, eax
        je     SHORT loc_2_0337
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 08Fh, 018h, 000h, 000h
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   2a2h
        push   eax
        db  0E8h, 0FBh, 017h, 000h, 000h
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_0337:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_2_0382
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   ebx
        mov    ecx, dword ptr [eax + 5eh]
        push   ecx
        db  0E8h, 0A4h, 018h, 000h, 000h
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        db  0E8h, 050h, 018h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ecx + 5eh]
        push   edx
        db  0E8h, 0ECh, 016h, 000h, 000h
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   2a2h
        push   ecx
        db  0E8h, 0B0h, 017h, 000h, 000h
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_0382:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 045h, 00Ch, 000h, 000h
        cmp    ax, 0ffffh
        jne    SHORT loc_2_03DA
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0aa000004h
        mov    ecx, dword ptr [eax + 5eh]
        push   ecx
        db  0E8h, 04Ch, 018h, 000h, 000h
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        db  0E8h, 0F8h, 017h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ecx + 5eh]
        push   edx
        db  0E8h, 094h, 016h, 000h, 000h
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   2a2h
        push   ecx
        db  0E8h, 058h, 017h, 000h, 000h
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_03DA:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 0C5h, 017h, 000h, 000h
        lea    eax, [ebp - 2ch]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   esi
        push   1
        push   ecx
        call   sub_2_0430
        test   eax, eax
        je     SHORT loc_2_0421
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 5eh]
        push   ecx
        db  0E8h, 04Dh, 016h, 000h, 000h
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   2a2h
        push   ecx
        db  0E8h, 011h, 017h, 000h, 000h
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_0421:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h
sub_2_0430:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 111h], eax
        cmp    edi, 1
        jbe    SHORT loc_2_0474
        push   0
        mov    eax, dword ptr [esi + 5eh]
        push   0aa000002h
        push   eax
        db  0E8h, 092h, 017h, 000h, 000h
        mov    dword ptr [ebp - 4], 0c000009ah
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_0474:
        xor    ebx, ebx
        db  0B8h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 109h], ebx
        mov    dword ptr [esi + 18dh], eax
        mov    dword ptr [esi + 181h], ebx
        mov    dword ptr [esi + 185h], eax
        push   1
        mov    dword ptr [esi + 189h], esi
        db  0C7h, 086h, 001h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 042h, 000h, 000h, 000h
        push   ebx
        mov    dword ptr [esi + 105h], ecx
        db  089h, 035h, 042h, 000h, 000h, 000h
        movsx  eax, byte ptr [esi + 14h]
        push   eax
        mov    edx, dword ptr [esi + 5eh]
        push   eax
        lea    ecx, [esi + 0b0h]
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 4]
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        push   ecx
        push   eax
        db  0E8h, 029h, 016h, 000h, 000h
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_2_0500
        push   ebx
        mov    eax, dword ptr [esi + 5eh]
        push   0aa000005h
        push   eax
        db  0E8h, 0FFh, 016h, 000h, 000h
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_0500:
        xor    eax, eax
        mov    ecx, dword ptr [esi + 22h]
        mov    ax, word ptr [esi + 20h]
        mov    edx, dword ptr [esi + 5eh]
        shl    eax, 0ah
        push   eax
        push   ecx
        push   edx
        lea    eax, [esi + 8]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        db  0E8h, 0CBh, 016h, 000h, 000h
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_053D
        push   0
        mov    eax, dword ptr [esi + 5eh]
        push   0aa000001h
        push   eax
        db  0E8h, 0BBh, 016h, 000h, 000h
        add    esp, 0ch
        jmp    NEAR PTR loc_2_074F
loc_2_053D:
        lea    ebx, [esi + 0ch]
        push   1000h
        mov    eax, dword ptr [esi + 26h]
        mov    ecx, dword ptr [esi + 5eh]
        push   eax
        lea    edx, [ebp - 4]
        push   ecx
        push   ebx
        push   edx
        db  0E8h, 093h, 016h, 000h, 000h
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_0575
        push   0
        mov    eax, dword ptr [esi + 5eh]
        push   0aa000001h
        push   eax
        db  0E8h, 083h, 016h, 000h, 000h
        add    esp, 0ch
        jmp    NEAR PTR loc_2_074F
loc_2_0575:
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [esi + 2eh], eax
        add    eax, 100h
        mov    dword ptr [esi + 2ah], eax
        mov    eax, dword ptr [ebx]
        mov    cl, byte ptr [eax + 0c6h]
        mov    byte ptr [esi + 3eh], cl
        mov    dl, byte ptr [eax + 0c8h]
        mov    byte ptr [esi + 3fh], dl
        mov    bl, byte ptr [eax + 0cah]
        mov    byte ptr [esi + 40h], bl
        mov    cl, byte ptr [eax + 0cch]
        mov    byte ptr [esi + 41h], cl
        mov    dl, byte ptr [eax + 0ceh]
        mov    byte ptr [esi + 42h], dl
        mov    edx, dword ptr [ebp + 14h]
        mov    bl, byte ptr [eax + 0d0h]
        mov    byte ptr [esi + 43h], bl
        mov    cl, byte ptr [edx]
        test   cl, cl
        jne    SHORT loc_2_061C
        cmp    byte ptr [edx + 1], 0
        jne    SHORT loc_2_061C
        cmp    byte ptr [edx + 2], 0
        jne    SHORT loc_2_061C
        cmp    byte ptr [edx + 3], 0
        jne    SHORT loc_2_061C
        cmp    byte ptr [edx + 4], 0
        jne    SHORT loc_2_061C
        cmp    byte ptr [edx + 5], 0
        jne    SHORT loc_2_061C
        lea    ebx, [esi + 38h]
        mov    cl, byte ptr [eax + 0c6h]
        mov    byte ptr [ebx], cl
        mov    dl, byte ptr [eax + 0c8h]
        mov    byte ptr [esi + 39h], dl
        mov    cl, byte ptr [eax + 0cah]
        mov    byte ptr [esi + 3ah], cl
        mov    dl, byte ptr [eax + 0cch]
        mov    byte ptr [esi + 3bh], dl
        mov    cl, byte ptr [eax + 0ceh]
        mov    byte ptr [esi + 3ch], cl
        mov    al, byte ptr [eax + 0d0h]
        mov    byte ptr [esi + 3dh], al
        jmp    SHORT loc_2_063F
loc_2_061C:
        lea    ebx, [esi + 38h]
        mov    byte ptr [ebx], cl
        mov    al, byte ptr [edx + 1]
        mov    byte ptr [esi + 39h], al
        mov    cl, byte ptr [edx + 2]
        mov    byte ptr [esi + 3ah], cl
        mov    al, byte ptr [edx + 3]
        mov    byte ptr [esi + 3bh], al
        mov    cl, byte ptr [edx + 4]
        mov    byte ptr [esi + 3ch], cl
        mov    dl, byte ptr [edx + 5]
        mov    byte ptr [esi + 3dh], dl
loc_2_063F:
        mov    word ptr [esi + 4eh], di
        lea    eax, [esi + 162h]
        lea    ecx, [esi + 16ah]
        mov    dword ptr [esi + 68h], eax
        lea    edx, [esi + 166h]
        mov    dword ptr [esi + 70h], ecx
        lea    eax, [esi + 15eh]
        mov    dword ptr [esi + 6ch], edx
        lea    ecx, [esi + 16eh]
        mov    dword ptr [esi + 74h], eax
        lea    edx, [esi + 14eh]
        mov    dword ptr [esi + 78h], ecx
        lea    eax, [esi + 142h]
        mov    dword ptr [esi + 7ch], edx
        lea    ecx, [esi + 146h]
        mov    dword ptr [esi + 80h], eax
        lea    edx, [esi + 14ah]
        mov    dword ptr [esi + 84h], ecx
        lea    eax, [esi + 152h]
        mov    dword ptr [esi + 88h], edx
        lea    ecx, [esi + 156h]
        mov    dword ptr [esi + 8ch], eax
        lea    edx, [esi + 15ah]
        mov    dword ptr [esi + 90h], ecx
        lea    edi, [esi + 0f9h]
        mov    dword ptr [esi + 98h], edx
        mov    word ptr [esi + 4ah], 5eah
        push   esi
        mov    dword ptr [edi], 0
        mov    word ptr [esi + 4ch], 100h
        mov    word ptr [esi + 0a8h], 600h
        call   sub_2_1770
        test   ax, ax
        je     SHORT loc_2_0710
        push   0
        mov    eax, dword ptr [esi + 5eh]
        mov    byte ptr [esi + 172h], 1
        push   0aa000003h
        push   eax
        db  0E8h, 0ECh, 014h, 000h, 000h
        mov    dword ptr [ebp - 4], 0c0010006h
        add    esp, 0ch
        jmp    SHORT loc_2_074F
loc_2_0710:
        push   edi
        lea    eax, [esi + 0fdh]
        push   eax
        mov    ecx, dword ptr [ebp + 10h]
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_2_19A0
        test   al, al
        jne    SHORT loc_2_07B4
        push   0
        mov    eax, dword ptr [esi + 5eh]
        push   0aa000002h
        push   eax
        db  0E8h, 0ABh, 014h, 000h, 000h
        mov    dword ptr [ebp - 4], 0c0000001h
        add    esp, 0ch
loc_2_074F:
        lea    eax, [esi + 0b0h]
        push   eax
        db  0E8h, 0B1h, 013h, 000h, 000h
        db  039h, 035h, 042h, 000h, 000h, 000h
        jne    SHORT loc_2_077A
        mov    eax, dword ptr [esi + 105h]
        pop    edi
        pop    esi
        db  0A3h, 042h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_077A:
        db  08Bh, 00Dh, 042h, 000h, 000h, 000h
        cmp    dword ptr [ecx + 105h], esi
        je     SHORT loc_2_0796
loc_2_0788:
        mov    ecx, dword ptr [ecx + 105h]
        cmp    dword ptr [ecx + 105h], esi
        jne    SHORT loc_2_0788
loc_2_0796:
        mov    eax, dword ptr [ecx + 105h]
        pop    edi
        pop    esi
        pop    ebx
        mov    edx, dword ptr [eax + 105h]
        mov    dword ptr [ecx + 105h], edx
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_07B4:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  090h
loc_2_07C0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [eax], 0
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], 0ffffffffh
        lea    eax, [eax + edx*4]
loc_2_07E4:
        test   edx, edx
        je     SHORT loc_2_07F3
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    SHORT loc_2_07E4
        mov    dword ptr [ecx], edx
loc_2_07F3:
        cmp    dword ptr [ecx], -1
        jne    SHORT loc_2_0805
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_0805:
        mov    esi, dword ptr [ebp + 20h]
        cmp    byte ptr [esi + 172h], 0
        je     SHORT loc_2_081E
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_081E:
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 4]
        push   eax
        push   0
        push   75h
        push   ecx
        db  0E8h, 0EDh, 012h, 000h, 000h
        test   eax, eax
        je     SHORT loc_2_0855
        push   0
        mov    eax, dword ptr [esi + 5eh]
        push   0aa000002h
        push   eax
        db  0E8h, 0ABh, 013h, 000h, 000h
        add    esp, 0ch
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_0855:
        inc    dword ptr [esi + 10dh]
        cmp    dword ptr [esi + 109h], 0
        jne    SHORT loc_2_088F
        push   esi
        call   sub_2_1870
        test   ax, ax
        je     SHORT loc_2_088F
        push   0
        mov    eax, dword ptr [esi + 5eh]
        push   0aa000003h
        push   eax
        db  0E8h, 071h, 013h, 000h, 000h
        add    esp, 0ch
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_088F:
        mov    eax, dword ptr [esi + 109h]
        mov    ecx, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx + 0ch], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 109h], ecx
        mov    ecx, dword ptr [esi + 0f9h]
        mov    eax, dword ptr [ebp - 4]
        add    eax, 14h
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_2_1B10
        test   al, al
        jne    SHORT loc_2_08F5
        push   0
        dec    dword ptr [esi + 10dh]
        mov    eax, dword ptr [ebp - 4]
        push   0aa000002h
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 109h], ecx
        mov    edx, dword ptr [esi + 5eh]
        push   edx
        db  0E8h, 00Bh, 013h, 000h, 000h
        add    esp, 0ch
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_08F5:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [esi + 101h]
        mov    dword ptr [eax + 10h], edx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 28h]
        mov    dword ptr [eax], edi
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], edx
        xor    ecx, ecx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, 0eeh
        mov    byte ptr [eax + 20h], cl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], edx
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 71h], ecx
        mov    dword ptr [esi + 17dh], edx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 1ch], 1
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [edi], edx
        mov    eax, dword ptr [esi + 10dh]
        dec    eax
        mov    dword ptr [esi + 10dh], eax
        jne    SHORT loc_2_098C
        cmp    byte ptr [esi + 173h], cl
        je     SHORT loc_2_0967
        lea    eax, [esi + 28dh]
        cmp    byte ptr [eax], cl
        je     SHORT loc_2_097B
loc_2_0967:
        cmp    dword ptr [esi + 125h], 0
        je     SHORT loc_2_098C
        lea    eax, [esi + 28dh]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_098C
loc_2_097B:
        push   0
        add    esi, 181h
        push   esi
        mov    byte ptr [eax], 1
        db  0E8h, 06Eh, 011h, 000h, 000h
loc_2_098C:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_09A0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 109h]
        test   edx, edx
        je     SHORT loc_2_09C3
loc_2_09B3:
        mov    esi, dword ptr [edx + 18h]
        cmp    ecx, esi
        jae    SHORT loc_2_09BC
        mov    ecx, esi
loc_2_09BC:
        mov    edx, dword ptr [edx + 0ch]
        test   edx, edx
        jne    SHORT loc_2_09B3
loc_2_09C3:
        test   ecx, ecx
        jne    SHORT loc_2_09CC
        mov    ecx, 0eeh
loc_2_09CC:
        pop    esi
        mov    dword ptr [eax + 17dh], ecx
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_09E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 20h], 0
        mov    esi, dword ptr [ebx + 8]
        je     SHORT loc_2_09FE
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_2_09FE:
        push   0
        inc    dword ptr [esi + 10dh]
        inc    dword ptr [ebx + 1ch]
        mov    eax, dword ptr [ebx + 14h]
        mov    ecx, dword ptr [esi + 0f9h]
        push   eax
        push   ecx
        call   sub_2_1BE0
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_2_0A3E
        mov    eax, dword ptr [ebx + 1ch]
        cmp    eax, 2
        je     SHORT loc_2_0A36
        mov    byte ptr [ebx + 20h], 1
        sub    dword ptr [ebx + 1ch], 2
        mov    edi, 103h
        jmp    SHORT loc_2_0A64
loc_2_0A36:
        sub    eax, 2
        mov    dword ptr [ebx + 1ch], eax
        jmp    SHORT loc_2_0A64
loc_2_0A3E:
        cmp    edi, 103h
        jne    SHORT loc_2_0A50
        mov    byte ptr [ebx + 20h], 1
        sub    dword ptr [ebx + 1ch], 2
        jmp    SHORT loc_2_0A64
loc_2_0A50:
        cmp    edi, 0c001000eh
        jne    SHORT loc_2_0A61
        mov    byte ptr [ebx + 20h], 1
        mov    edi, 103h
loc_2_0A61:
        dec    dword ptr [ebx + 1ch]
loc_2_0A64:
        cmp    dword ptr [ebx + 1ch], 0
        jne    SHORT loc_2_0AD7
        mov    eax, dword ptr [esi + 109h]
        cmp    ebx, eax
        jne    SHORT loc_2_0A7F
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [esi + 109h], eax
        jmp    SHORT loc_2_0A92
loc_2_0A7F:
        cmp    dword ptr [eax + 0ch], ebx
        je     SHORT loc_2_0A8C
loc_2_0A84:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], ebx
        jne    SHORT loc_2_0A84
loc_2_0A8C:
        mov    ecx, dword ptr [ebx + 0ch]
        mov    dword ptr [eax + 0ch], ecx
loc_2_0A92:
        mov    eax, dword ptr [ebx + 18h]
        cmp    dword ptr [esi + 17dh], eax
        jne    SHORT loc_2_0AA3
        push   esi
        call   sub_2_09A0
loc_2_0AA3:
        cmp    dword ptr [esi + 109h], 0
        jne    SHORT loc_2_0ADC
        push   esi
        lea    eax, [esi + 0b0h]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 054h, 010h, 000h, 000h
        test   al, al
        jne    SHORT loc_2_0ADC
        push   0
        mov    eax, dword ptr [esi + 5eh]
        push   0aa000003h
        push   eax
        db  0E8h, 01Eh, 011h, 000h, 000h
        add    esp, 0ch
        jmp    SHORT loc_2_0ADC
loc_2_0AD7:
        mov    edi, 103h
loc_2_0ADC:
        mov    eax, dword ptr [esi + 10dh]
        dec    eax
        mov    dword ptr [esi + 10dh], eax
        jne    SHORT loc_2_0B24
        cmp    byte ptr [esi + 173h], 0
        je     SHORT loc_2_0AFF
        lea    eax, [esi + 28dh]
        cmp    byte ptr [eax], 0
        je     SHORT loc_2_0B13
loc_2_0AFF:
        cmp    dword ptr [esi + 125h], 0
        je     SHORT loc_2_0B24
        lea    eax, [esi + 28dh]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_0B24
loc_2_0B13:
        push   0
        add    esi, 181h
        push   esi
        mov    byte ptr [eax], 1
        db  0E8h, 0D6h, 00Fh, 000h, 000h
loc_2_0B24:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_2_0B30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [edi + 1ch]
        inc    dword ptr [esi + 10dh]
        cmp    byte ptr [edi + 20h], 0
        je     SHORT loc_2_0B57
        mov    dword ptr [ebp - 4], 0c0010002h
        jmp    SHORT loc_2_0B99
loc_2_0B57:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 10h]
        test   ecx, ecx
        jne    SHORT loc_2_0B6E
        push   eax
        push   edi
        push   esi
        call   sub_2_1000
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_2_0B99
loc_2_0B6E:
        cmp    ecx, 1
        jne    SHORT loc_2_0B92
        cmp    byte ptr [esi + 172h], 0
        je     SHORT loc_2_0B85
        mov    dword ptr [ebp - 4], 0c0000001h
        jmp    SHORT loc_2_0B99
loc_2_0B85:
        push   eax
        push   edi
        push   esi
        call   sub_2_1050
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_2_0B99
loc_2_0B92:
        mov    dword ptr [ebp - 4], 10001h
loc_2_0B99:
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 1ch]
        jne    SHORT loc_2_0C09
        mov    eax, dword ptr [ebx + 109h]
        cmp    edi, eax
        jne    SHORT loc_2_0BB6
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebx + 109h], eax
        jmp    SHORT loc_2_0BC9
loc_2_0BB6:
        cmp    dword ptr [eax + 0ch], edi
        je     SHORT loc_2_0BC3
loc_2_0BBB:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], edi
        jne    SHORT loc_2_0BBB
loc_2_0BC3:
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], ecx
loc_2_0BC9:
        mov    eax, dword ptr [ebx + 17dh]
        cmp    dword ptr [edi + 18h], eax
        jne    SHORT loc_2_0BDA
        push   ebx
        call   sub_2_09A0
loc_2_0BDA:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 078h, 00Eh, 000h, 000h
        push   0
        push   75h
        push   edi
        db  0E8h, 036h, 00Fh, 000h, 000h
        cmp    dword ptr [ebx + 109h], 0
        jne    SHORT loc_2_0C09
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        add    ebx, 0b0h
        push   ebx
        db  0E8h, 009h, 00Fh, 000h, 000h
loc_2_0C09:
        mov    eax, dword ptr [esi + 10dh]
        dec    eax
        mov    dword ptr [esi + 10dh], eax
        jne    SHORT loc_2_0C51
        cmp    byte ptr [esi + 173h], 0
        je     SHORT loc_2_0C2C
        lea    eax, [esi + 28dh]
        cmp    byte ptr [eax], 0
        je     SHORT loc_2_0C40
loc_2_0C2C:
        cmp    dword ptr [esi + 125h], 0
        je     SHORT loc_2_0C51
        lea    eax, [esi + 28dh]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_0C51
loc_2_0C40:
        push   0
        add    esi, 181h
        push   esi
        mov    byte ptr [eax], 1
        db  0E8h, 0A9h, 00Eh, 000h, 000h
loc_2_0C51:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_2_0C60:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        mov    dword ptr [ebp - 18h], eax
        push   esi
        mov    dword ptr [ebp - 0ch], eax
        push   edi
        lea    esi, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 1010101h
        ja     SHORT loc_2_0C9D
        je     NEAR PTR loc_2_0E5A
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_2_0CBE
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_2_0C9D:
        cmp    eax, 1010102h
        je     NEAR PTR loc_2_0E79
        cmp    eax, 1010103h
        je     NEAR PTR loc_2_0E98
        cmp    eax, 1010104h
        je     NEAR PTR loc_2_0F5D
loc_2_0CBE:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_2_0F69
loc_2_0CCA:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_2_0CDF
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, 5ch
        jmp    NEAR PTR loc_2_0F69
loc_2_0CDF:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, 7ch
        jmp    NEAR PTR loc_2_0F69
loc_2_0CEE:
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 14h], 4
        cmp    byte ptr [ebx + 172h], 0
        jne    SHORT loc_2_0D08
        mov    dword ptr [ebp - 14h], 0
loc_2_0D08:
        lea    esi, [ebp - 14h]
        mov    edi, 4
        jmp    NEAR PTR loc_2_0F69
loc_2_0D15:
        lea    esi, [ebp - 18h]
        mov    edi, 4
        jmp    NEAR PTR loc_2_0F69
loc_2_0D22:
        mov    dword ptr [ebp - 8], 0eeh
        jmp    NEAR PTR loc_2_0F69
loc_2_0D2E:
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_2_0F69
loc_2_0D3A:
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_2_0F69
loc_2_0D46:
        mov    ebx, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 4eh]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0a8h]
        imul   ecx, eax
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_2_0F69
loc_2_0D63:
        mov    ebx, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 20h]
        xor    edx, edx
        mov    dx, word ptr [ebx + 4eh]
        xor    eax, eax
        shl    ecx, 0ah
        mov    ax, word ptr [ebx + 0a8h]
        imul   edx, eax
        sub    ecx, edx
        sub    ecx, 100h
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_2_0F69
loc_2_0D91:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 4ch]
        jmp    NEAR PTR loc_2_0F66
loc_2_0D9F:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 4ch]
        jmp    NEAR PTR loc_2_0F66
loc_2_0DAD:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 3eh
        lea    eax, [ebp - 8]
        push   ebx
        lea    esi, [ebp - 8]
        push   eax
        mov    edi, 4
        db  0E8h, 039h, 011h, 000h, 000h
        and    dword ptr [ebp - 8], 0ffffff00h
        jmp    NEAR PTR loc_2_0F69
loc_2_0DD3:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, 0dh
        jmp    NEAR PTR loc_2_0F69
loc_2_0DE2:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_2_0DFC
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0f9h]
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_2_0F69
loc_2_0DFC:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 14h]
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 8], edx
        jmp    NEAR PTR loc_2_0F69
loc_2_0E0D:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_2_0E21
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 17dh]
        jmp    NEAR PTR loc_2_0F66
loc_2_0E21:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_2_0F69
loc_2_0E2F:
        lea    esi, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 30ah
        jmp    NEAR PTR loc_2_0F69
loc_2_0E42:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_2_0F69
loc_2_0E4E:
        mov    dword ptr [ebp - 8], 0eh
        jmp    NEAR PTR loc_2_0F69
loc_2_0E5A:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 3eh
        lea    eax, [ebp - 20h]
        push   ebx
        lea    esi, [ebp - 20h]
        push   eax
        mov    edi, 6
        db  0E8h, 08Ch, 010h, 000h, 000h
        jmp    NEAR PTR loc_2_0F69
loc_2_0E79:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 38h
        lea    eax, [ebp - 20h]
        push   ebx
        lea    esi, [ebp - 20h]
        push   eax
        mov    edi, 6
        db  0E8h, 06Dh, 010h, 000h, 000h
        jmp    NEAR PTR loc_2_0F69
loc_2_0E98:
        cmp    byte ptr [ebp + 14h], 0
        mov    ebx, dword ptr [ebp + 8]
        je     SHORT loc_2_0EF8
        mov    ecx, dword ptr [ebx + 0f9h]
        mov    eax, dword ptr [ecx + 24h]
        mov    dword ptr [ebp - 10h], eax
        lea    ebx, [eax + eax*2]
        lea    edx, [ebx*2]
        cmp    dword ptr [ebp + 1ch], edx
        jae    SHORT loc_2_0ECD
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    dword ptr [eax], edx
        jmp    NEAR PTR loc_2_0F69
loc_2_0ECD:
        mov    eax, dword ptr [ebp + 18h]
        lea    edx, [ebp - 10h]
        push   eax
        mov    ebx, dword ptr [ebp + 1ch]
        push   edx
        lea    eax, [ebp - 0ch]
        push   ebx
        push   ecx
        push   eax
        call   sub_2_2100
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ecx], eax
        je     SHORT loc_2_0F6F
        jmp    SHORT loc_2_0F69
loc_2_0EF8:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebx + 0f9h]
        mov    ecx, dword ptr [eax + 14h]
        push   ecx
        push   edx
        call   sub_2_2000
        lea    edx, [eax + eax*2]
        mov    dword ptr [ebp - 10h], eax
        lea    ecx, [edx*2]
        cmp    dword ptr [ebp + 1ch], ecx
        jae    SHORT loc_2_0F2B
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    dword ptr [eax], ecx
        jmp    SHORT loc_2_0F69
loc_2_0F2B:
        mov    eax, dword ptr [ebp + 18h]
        lea    ecx, [ebp - 10h]
        push   eax
        mov    edx, dword ptr [ebp + 1ch]
        push   ecx
        mov    eax, dword ptr [ebp + 0ch]
        push   edx
        mov    ecx, dword ptr [eax + 14h]
        push   ecx
        mov    edx, dword ptr [ebx + 0f9h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_2_2040
        mov    ecx, dword ptr [ebp - 10h]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [ebp + 24h]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_2_0F69
loc_2_0F5D:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 111h]
loc_2_0F66:
        mov    dword ptr [ebp - 8], eax
loc_2_0F69:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_0FA8
loc_2_0F6F:
        cmp    dword ptr [ebp + 10h], 1010103h
        je     SHORT loc_2_0FA8
        cmp    dword ptr [ebp + 1ch], edi
        jae    SHORT loc_2_0F98
        sub    edi, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_2_0F98:
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        push   esi
        push   eax
        db  0E8h, 05Dh, 00Fh, 000h, 000h
        mov    ecx, dword ptr [ebp + 24h]
        add    dword ptr [ecx], edi
loc_2_0FA8:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_1000:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    edx, [ebp - 4]
        push   edx
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [esi + 1ch]
        mov    edx, dword ptr [esi + 18h]
loc_2_1020:
        push   eax
        push   edx
        push   ecx
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 14h]
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   eax
        push   edx
        call   sub_2_0C60
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 20h], ecx
        mov    dword ptr [esi + 24h], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1050:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        xor    ebx, ebx
        mov    edi, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 18h]
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 1010103h
        ja     SHORT loc_2_108D
        je     NEAR PTR loc_2_119C
        cmp    eax, 1010eh
        je     SHORT loc_2_109F
        cmp    eax, 1010fh
        je     SHORT loc_2_10FA
        cmp    eax, 10112h
        je     NEAR PTR loc_2_1175
loc_2_108D:
        mov    ebx, 0c00000bbh
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_2_11D7
loc_2_109F:
        cmp    edi, 4
        je     SHORT loc_2_10B6
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_2_11D7
loc_2_10B6:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        db  0E8h, 03Eh, 00Eh, 000h, 000h
        test   dword ptr [ebp - 8], 0f050h
        je     SHORT loc_2_10E3
        mov    dword ptr [esi + 20h], 4
        mov    dword ptr [esi + 24h], 0
        mov    ebx, 0c00000bbh
        jmp    NEAR PTR loc_2_11D7
loc_2_10E3:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edx
        call   sub_2_11F0
        jmp    NEAR PTR loc_2_11D5
loc_2_10FA:
        cmp    edi, 4
        je     SHORT loc_2_1111
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_2_11D7
loc_2_1111:
        push   4
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        db  0E8h, 0E3h, 00Dh, 000h, 000h
        cmp    dword ptr [ebp - 4], 0eeh
        ja     SHORT loc_2_116E
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 17dh]
        cmp    eax, dword ptr [ebp - 4]
        jae    SHORT loc_2_1148
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 17dh], eax
        mov    dword ptr [ecx + 18h], eax
        jmp    NEAR PTR loc_2_11D7
loc_2_1148:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    eax, dword ptr [ecx + 18h]
        jne    SHORT loc_2_1166
        mov    eax, dword ptr [ecx + 18h]
        cmp    eax, dword ptr [ebp - 4]
        jbe    SHORT loc_2_1166
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    dword ptr [ecx + 18h], eax
        call   sub_2_09A0
        jmp    SHORT loc_2_11D7
loc_2_1166:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 18h], eax
        jmp    SHORT loc_2_11D7
loc_2_116E:
        mov    ebx, 0c0010014h
        jmp    SHORT loc_2_11D7
loc_2_1175:
        cmp    edi, 4
        je     SHORT loc_2_1189
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_2_11D7
loc_2_1189:
        push   4
        xor    ebx, ebx
        push   ecx
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 71h
        push   ecx
        db  0E8h, 066h, 00Dh, 000h, 000h
        jmp    SHORT loc_2_11D7
loc_2_119C:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        test   edx, edx
        je     SHORT loc_2_11BA
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_2_11D7
loc_2_11BA:
        push   ecx
        mov    eax, edi
        mov    ecx, 6
        sub    edx, edx
        div    ecx
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   edx
        call   sub_2_1230
loc_2_11D5:
        mov    ebx, eax
loc_2_11D7:
        test   ebx, ebx
        jne    SHORT loc_2_11E5
        mov    dword ptr [esi + 24h], 0
        mov    dword ptr [esi + 20h], edi
loc_2_11E5:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_2_11F0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 20h], 0
        jne    SHORT loc_2_1225
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    ecx, dword ptr [ebp + 10h]
        push   ecx
        mov    edx, dword ptr [esi + 14h]
        push   edx
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [esi + 1ch]
        mov    ecx, dword ptr [eax + 0f9h]
        push   ecx
        call   sub_2_1F80
        dec    dword ptr [esi + 1ch]
        pop    esi
        pop    ebp
        ret    10h
loc_2_1225:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    10h
        db  090h
sub_2_1230:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 20h], 0
        jne    SHORT loc_2_1269
        push   1
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    ecx, dword ptr [ebp + 14h]
        push   ecx
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi + 14h]
        push   eax
        mov    ecx, dword ptr [ebp + 8]
        inc    dword ptr [esi + 1ch]
        mov    edx, dword ptr [ecx + 0f9h]
        push   edx
        call   sub_2_1CA0
        dec    dword ptr [esi + 1ch]
        pop    esi
        pop    ebp
        ret    14h
loc_2_1269:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1280:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 10h], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        lea    ecx, [ebp - 0ch]
        lea    edx, [ebp - 10h]
        mov    edi, dword ptr [esi + 1ch]
        mov    eax, dword ptr [esi + 18h]
        push   ecx
        mov    dword ptr [ebp - 14h], eax
        push   edx
        mov    ecx, dword ptr [esi + 14h]
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   eax
        push   1
        push   ecx
        push   0
        push   ebx
        call   sub_2_0C60
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 0c00000bbh
        jne    NEAR PTR loc_2_1360
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 1020101h
        ja     SHORT loc_2_12F0
        je     SHORT loc_2_132F
        sub    eax, 20101h
        cmp    eax, 4
        ja     SHORT loc_2_12FE
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_2_12F0:
        cmp    eax, 1020102h
        je     SHORT loc_2_1337
        cmp    eax, 1020103h
        je     SHORT loc_2_133F
loc_2_12FE:
        mov    dword ptr [ebp - 8], 0c00000bbh
        jmp    SHORT loc_2_1348
loc_2_1307:
        mov    eax, dword ptr [ebx + 13ah]
        jmp    SHORT loc_2_1345
loc_2_130F:
        mov    eax, dword ptr [ebx + 13eh]
        jmp    SHORT loc_2_1345
loc_2_1317:
        mov    eax, dword ptr [ebx + 142h]
        jmp    SHORT loc_2_1345
loc_2_131F:
        mov    eax, dword ptr [ebx + 162h]
        jmp    SHORT loc_2_1345
loc_2_1327:
        mov    eax, dword ptr [ebx + 166h]
        jmp    SHORT loc_2_1345
loc_2_132F:
        mov    eax, dword ptr [ebx + 15eh]
        jmp    SHORT loc_2_1345
loc_2_1337:
        mov    eax, dword ptr [ebx + 146h]
        jmp    SHORT loc_2_1345
loc_2_133F:
        mov    eax, dword ptr [ebx + 14ah]
loc_2_1345:
        mov    dword ptr [ebp - 4], eax
loc_2_1348:
        cmp    edi, 4
        jb     SHORT loc_2_1360
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 14h]
        push   ecx
        db  0E8h, 0A4h, 00Bh, 000h, 000h
        add    dword ptr [ebp - 0ch], 4
loc_2_1360:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        pop    edi
        mov    dword ptr [esi + 20h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 24h], ecx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 08Dh, 064h, 024h, 000h
loc_2_1390:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   esi
        push   edi
        xor    esi, esi
        mov    eax, dword ptr [ecx + 14h]
        cmp    eax, 10113h
        ja     SHORT loc_2_13B7
        je     SHORT loc_2_13E8
        cmp    eax, 10101h
        jb     SHORT loc_2_13E3
        cmp    eax, 10111h
        jbe    SHORT loc_2_13E8
        jmp    SHORT loc_2_13E3
loc_2_13B7:
        cmp    eax, 1010104h
        ja     SHORT loc_2_13D5
        cmp    eax, 1010101h
        jae    SHORT loc_2_13E8
        cmp    eax, 20101h
        jb     SHORT loc_2_13E3
        cmp    eax, 20105h
        jbe    SHORT loc_2_13E8
        jmp    SHORT loc_2_13E3
loc_2_13D5:
        cmp    eax, 1020101h
        jb     SHORT loc_2_13E3
        cmp    eax, 1020103h
        jbe    SHORT loc_2_13E8
loc_2_13E3:
        mov    esi, 0c00000bbh
loc_2_13E8:
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 10dh]
        test   esi, esi
        jne    SHORT loc_2_13FE
        push   ecx
        push   edi
        call   sub_2_1280
        mov    esi, eax
loc_2_13FE:
        mov    eax, dword ptr [edi + 10dh]
        dec    eax
        mov    dword ptr [edi + 10dh], eax
        jne    SHORT loc_2_1446
        cmp    byte ptr [edi + 173h], 0
        je     SHORT loc_2_1421
        lea    eax, [edi + 28dh]
        cmp    byte ptr [eax], 0
        je     SHORT loc_2_1435
loc_2_1421:
        cmp    dword ptr [edi + 125h], 0
        je     SHORT loc_2_1446
        lea    eax, [edi + 28dh]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_1446
loc_2_1435:
        push   0
        add    edi, 181h
        push   edi
        mov    byte ptr [eax], 1
        db  0E8h, 0B4h, 006h, 000h, 000h
loc_2_1446:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_2_1450:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_1760
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 295h]
        push   eax
        mov    edi, 300h
        push   ecx
        db  0E8h, 08Ah, 006h, 000h, 000h
loc_2_1476:
        push   2710h
        db  0E8h, 098h, 006h, 000h, 000h
        dec    edi
        jne    SHORT loc_2_1476
        mov    byte ptr [esi + 28fh], 1
        db  039h, 035h, 042h, 000h, 000h, 000h
        jne    SHORT loc_2_149F
        mov    eax, dword ptr [esi + 105h]
        db  0A3h, 042h, 000h, 000h, 000h
        jmp    SHORT loc_2_14CD
loc_2_149F:
        db  08Bh, 00Dh, 042h, 000h, 000h, 000h
        cmp    dword ptr [ecx + 105h], esi
        je     SHORT loc_2_14BB
loc_2_14AD:
        mov    ecx, dword ptr [ecx + 105h]
        cmp    dword ptr [ecx + 105h], esi
        jne    SHORT loc_2_14AD
loc_2_14BB:
        mov    eax, dword ptr [ecx + 105h]
        mov    edx, dword ptr [eax + 105h]
        mov    dword ptr [ecx + 105h], edx
loc_2_14CD:
        lea    eax, [esi + 0b0h]
        push   eax
        db  0E8h, 033h, 006h, 000h, 000h
        mov    eax, dword ptr [esi + 0f9h]
        push   eax
        call   sub_2_1AC0
        mov    eax, dword ptr [esi + 5eh]
        push   eax
        db  0E8h, 062h, 005h, 000h, 000h
        push   0
        push   2a2h
        push   esi
        db  0E8h, 029h, 006h, 000h, 000h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1510:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        db  0E8h, 01Fh, 005h, 000h, 000h
        push   0
        db  08Bh, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        db  0E8h, 005h, 005h, 000h, 000h
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1540:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 8]
        cmp    byte ptr [eax + 20h], 0
        je     SHORT loc_2_1559
        mov    eax, 0c0010002h
        pop    ebp
        ret    4
loc_2_1559:
        cmp    byte ptr [ecx + 173h], 0
        je     SHORT loc_2_1568
        xor    eax, eax
        pop    ebp
        ret    4
loc_2_1568:
        inc    dword ptr [eax + 1ch]
        inc    dword ptr [ecx + 10dh]
        mov    byte ptr [ecx + 173h], 1
        mov    dword ptr [ecx + 175h], eax
        mov    eax, dword ptr [ecx + 10dh]
        dec    eax
        mov    dword ptr [ecx + 10dh], eax
        jne    SHORT loc_2_15C6
        cmp    byte ptr [ecx + 173h], 0
        je     SHORT loc_2_15A1
        lea    eax, [ecx + 28dh]
        cmp    byte ptr [eax], 0
        je     SHORT loc_2_15B5
loc_2_15A1:
        cmp    dword ptr [ecx + 125h], 0
        je     SHORT loc_2_15C6
        lea    eax, [ecx + 28dh]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_15C6
loc_2_15B5:
        push   0
        add    ecx, 181h
        push   ecx
        mov    byte ptr [eax], 1
        db  0E8h, 034h, 005h, 000h, 000h
loc_2_15C6:
        mov    eax, 103h
        pop    ebp
        ret    4
        db  090h
loc_2_15D0:
        xor    eax, eax
        ret    1ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_15E0:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 8]
        push   ecx
        call   sub_2_1950
        cmp    ax, 1
        pop    ebp
        mov    eax, 0
        adc    eax, -1
        and    eax, 0c0000001h
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1610:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        dec    dword ptr [eax + 1ch]
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_2_1620:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_2_1900
        cmp    ax, 1
        pop    ebp
        sbb    cl, cl
        neg    cl
        mov    al, cl
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_2_1640:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   0fa0h
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 2eh]
        mov    bl, byte ptr [eax]
        mov    cl, bl
        and    bl, 7fh
        or     cl, 80h
        mov    byte ptr [eax], cl
        db  0E8h, 0B7h, 004h, 000h, 000h
        mov    eax, dword ptr [esi + 2eh]
        push   0fa0h
        mov    byte ptr [eax], bl
        db  0E8h, 0A8h, 004h, 000h, 000h
        mov    eax, dword ptr [esi + 2eh]
        xor    ecx, ecx
        or     byte ptr [eax], 2
        mov    eax, dword ptr [esi + 2eh]
        mov    byte ptr [eax + 10h], 1
        mov    eax, dword ptr [esi + 2eh]
        mov    byte ptr [eax + 1ah], cl
        mov    eax, dword ptr [esi + 2eh]
        mov    byte ptr [eax + 1bh], cl
        mov    eax, dword ptr [esi + 2eh]
        test   byte ptr [eax + 17h], 80h
        jne    SHORT loc_2_169E
        push   186a00h
        db  0E8h, 07Ah, 004h, 000h, 000h
loc_2_169E:
        mov    eax, dword ptr [esi + 2eh]
        push   esi
        mov    byte ptr [eax + 1eh], 49h
        call   sub_2_1950
        mov    eax, dword ptr [esi + 2eh]
        mov    byte ptr [eax + 1dh], 4
        mov    cl, byte ptr [esi + 0f2h]
        mov    eax, dword ptr [esi + 2eh]
        mov    byte ptr [eax + 11h], cl
        mov    eax, dword ptr [esi + 2eh]
        mov    dl, byte ptr [esi + 0f3h]
        mov    byte ptr [eax + 12h], dl
        mov    eax, dword ptr [esi + 2eh]
        mov    cl, byte ptr [esi + 0f2h]
        mov    byte ptr [eax + 13h], cl
        mov    eax, dword ptr [esi + 2eh]
        mov    ecx, 3fh
        mov    byte ptr [eax + 17h], 0ffh
        mov    eax, dword ptr [esi + 2eh]
        mov    byte ptr [esi + 0aeh], cl
        mov    byte ptr [eax + 1fh], cl
        mov    eax, dword ptr [esi + 2eh]
        mov    byte ptr [eax + 10h], 41h
        xor    eax, eax
loc_2_16F7:
        mov    dl, byte ptr [esi + eax + 38h]
        mov    ecx, dword ptr [esi + 2eh]
        inc    eax
        cmp    eax, 6
        mov    byte ptr [ecx + eax + 10h], dl
        jb     SHORT loc_2_16F7
        mov    cl, byte ptr [esi + 0f2h]
        mov    eax, dword ptr [esi + 2eh]
        inc    cl
        mov    byte ptr [eax + 17h], cl
        xor    ecx, ecx
        xor    eax, eax
        mov    ax, word ptr [esi + 32h]
        shl    eax, 0ah
        sub    eax, 100h
        je     SHORT loc_2_1743
loc_2_1728:
        mov    eax, dword ptr [esi + 2ah]
        inc    ecx
        mov    byte ptr [eax + ecx - 1], 0
        xor    eax, eax
        mov    ax, word ptr [esi + 32h]
        shl    eax, 0ah
        sub    eax, 100h
        cmp    eax, ecx
        ja     SHORT loc_2_1728
loc_2_1743:
        mov    eax, dword ptr [esi + 2eh]
        pop    esi
        pop    ebx
        pop    ebp
        mov    byte ptr [eax + 10h], 1
        mov    al, 1
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1760:
        xor    ax, ax
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1770:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    eax, dword ptr [esi + 2eh]
        or     byte ptr [eax], 2
        mov    eax, 0ffffffffh
        mov    word ptr [esi + 0cdh], ax
        mov    byte ptr [esi + 0cfh], cl
        mov    word ptr [esi + 0c9h], cx
        mov    byte ptr [esi + 0f1h], cl
        mov    word ptr [esi + 0cbh], ax
        cmp    word ptr [esi + 4eh], cx
        jbe    SHORT loc_2_17C8
        lea    eax, [esi + 0d4h]
        xor    edx, edx
loc_2_17B5:
        mov    dword ptr [eax], edx
        add    eax, 4
        inc    ecx
        xor    edi, edi
        mov    dword ptr [eax - 8], edx
        mov    di, word ptr [esi + 4eh]
        cmp    edi, ecx
        ja     SHORT loc_2_17B5
loc_2_17C8:
        xor    eax, eax
        xor    ecx, ecx
        mov    ax, word ptr [esi + 0a8h]
        xor    ebx, ebx
        mov    byte ptr [esi + 0c8h], 0
        mov    cx, word ptr [esi + 4eh]
        imul   eax, ecx
        mov    bx, word ptr [esi + 20h]
        add    eax, dword ptr [esi + 2ah]
        mov    edi, ebx
        mov    ecx, dword ptr [esi + 8]
        shl    edi, 0ah
        mov    dword ptr [esi + 0d8h], eax
        sub    eax, ecx
        lea    edx, [ecx + edi]
        xor    ecx, ecx
        and    ebx, 3fffffh
        mov    cx, word ptr [esi + 4ch]
        mov    dword ptr [esi + 0dch], edx
        cdq
        idiv   ecx
        mov    byte ptr [esi + 0f2h], al
        cmp    ebx, 40h
        jne    SHORT loc_2_1826
        mov    byte ptr [esi + 0f3h], 0ffh
        jmp    SHORT loc_2_1837
loc_2_1826:
        mov    eax, edi
        cdq
        and    edx, 0ffh
        add    eax, edx
        mov    byte ptr [esi + 0f3h], ah
loc_2_1837:
        push   esi
        call   sub_2_1640
        test   al, al
        jne    SHORT loc_2_1853
        mov    ax, 9
        pop    edi
        mov    byte ptr [esi + 0ach], 4
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_2_1853:
        xor    ax, ax
        pop    edi
        mov    byte ptr [esi + 0ach], 2
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1870:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0ach], 1
        jne    SHORT loc_2_1888
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
loc_2_1888:
        mov    eax, dword ptr [esi + 2eh]
        or     byte ptr [eax], 2
        cmp    byte ptr [esi + 0ach], 3
        jne    SHORT loc_2_18A2
        push   esi
        call   sub_2_1770
        test   ax, ax
        jne    SHORT loc_2_18F4
loc_2_18A2:
        cmp    byte ptr [esi + 0ach], 2
        je     SHORT loc_2_18B4
        mov    ax, 7
        pop    esi
        pop    ebp
        ret    4
loc_2_18B4:
        mov    eax, dword ptr [esi + 2eh]
        mov    byte ptr [eax + 10h], 2
        test   byte ptr [esi + 0a4h], 10h
        mov    eax, dword ptr [esi + 2eh]
        je     SHORT loc_2_18CD
        mov    byte ptr [eax + 1dh], 1
        jmp    SHORT loc_2_18D1
loc_2_18CD:
        mov    byte ptr [eax + 1dh], 0
loc_2_18D1:
        mov    byte ptr [esi + 0ach], 1
        mov    eax, dword ptr [esi + 2eh]
        mov    cl, byte ptr [eax + 2]
        add    eax, 2
        cmp    cl, 20h
        jne    SHORT loc_2_18EB
        or     cl, 80h
        mov    byte ptr [eax], cl
loc_2_18EB:
        push   esi
        db  0E8h, 02Fh, 0F7h, 0FFh, 0FFh
        xor    ax, ax
loc_2_18F4:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1900:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0ach], 1
        je     SHORT loc_2_1919
        mov    ax, 0ah
        pop    esi
        pop    ebp
        ret    4
loc_2_1919:
        push   esi
        db  0E8h, 041h, 0F7h, 0FFh, 0FFh
        mov    eax, dword ptr [esi + 2eh]
        push   186a00h
        mov    byte ptr [eax + 10h], 1
        mov    eax, dword ptr [esi + 2eh]
        and    byte ptr [eax + 2], 7fh
        db  0E8h, 0E1h, 001h, 000h, 000h
        mov    byte ptr [esi + 0ach], 3
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1950:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    edx, dword ptr [esp + 8]
        mov    eax, dword ptr [edx + 2eh]
        or     byte ptr [eax], 2
        mov    eax, dword ptr [edx + 2eh]
        mov    byte ptr [eax + 10h], cl
        mov    eax, dword ptr [edx + 0f9h]
        test   eax, eax
        je     SHORT loc_2_1974
        mov    eax, dword ptr [eax + 0ch]
        jmp    SHORT loc_2_1976
loc_2_1974:
        xor    eax, eax
loc_2_1976:
        test   al, 8
        je     SHORT loc_2_197C
        mov    cl, 4
loc_2_197C:
        test   al, 6
        je     SHORT loc_2_1983
        or     cl, 8
loc_2_1983:
        test   al, 20h
        je     SHORT loc_2_198A
        or     cl, 1ch
loc_2_198A:
        mov    eax, dword ptr [edx + 2eh]
        pop    ebp
        mov    byte ptr [eax + 1ch], cl
        xor    ax, ax
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_19A0:
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
        db  0E8h, 061h, 001h, 000h, 000h
        mov    ecx, dword ptr [ebp + 20h]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     SHORT loc_2_19D4
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_19D4:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
        stosw
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    SHORT loc_2_19EE
        mov    esi, 2
loc_2_19EE:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 01Dh, 001h, 000h, 000h
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     SHORT loc_2_1A22
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_1AC0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_1A22:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        db  0E8h, 0E5h, 000h, 000h, 000h
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
loc_2_1A3E:
        je     SHORT loc_2_1A51
        push   eax
        call   sub_2_1AC0
        sub    al, al
        pop    edi
        pop    esi
loc_2_1A4A:
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_1A51:
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
sub_2_1AC0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_2_1AE0
        mov    ecx, dword ptr [esi + 20h]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        db  0E8h, 044h, 000h, 000h, 000h
loc_2_1AE0:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     SHORT loc_2_1AF6
        push   0
        mov    ecx, dword ptr [esi + 20h]
loc_2_1AEC:
        shl    ecx, 3
        push   ecx
        push   eax
        db  0E8h, 02Eh, 000h, 000h, 000h
loc_2_1AF6:
        push   0
        push   32h
loc_2_1AFA:
        push   esi
        db  0E8h, 024h, 000h, 000h, 000h
loc_2_1B00:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1B10:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 28h], 0
loc_2_1B1E:
        jne    SHORT loc_2_1B29
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_1B29:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   0
        push   1ah
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 0E2h, 0FFh, 0FFh, 0FFh
        or     eax, eax
        je     SHORT loc_2_1B49
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_1B49:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 28h]
        mov    edi, edi
loc_2_1B50:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    SHORT loc_2_1B63
        inc    ecx
        cmp    ecx, 20h
        jb     SHORT loc_2_1B50
        jmp    SHORT loc_2_1B72
loc_2_1B63:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 28h], eax
loc_2_1B72:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 10h]
        mov    dword ptr [eax + 10h], ecx
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_2_1B88
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], ecx
loc_2_1B88:
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
sub_2_1BE0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
loc_2_1BEA:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_2_1F80
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_2_1C09
        cmp    edi, 103h
        jne    SHORT loc_2_1C20
loc_2_1C09:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   esi
        push   ebx
        call   sub_2_1CA0
        or     eax, eax
        je     SHORT loc_2_1C20
        mov    edi, eax
loc_2_1C20:
        or     edi, edi
        je     SHORT loc_2_1C2C
        cmp    edi, 103h
        jne    SHORT loc_2_1C8B
loc_2_1C2C:
        mov    eax, dword ptr [esi + 0ch]
        dec    eax
        mov    dword ptr [esi + 0ch], eax
        jne    SHORT loc_2_1C86
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_2_1C42
        mov    ecx, dword ptr [esi + 14h]
        mov    dword ptr [eax + 14h], ecx
loc_2_1C42:
        mov    eax, dword ptr [esi + 14h]
        or     eax, eax
        je     SHORT loc_2_1C51
        mov    ecx, dword ptr [esi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_2_1C57
loc_2_1C51:
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [ebx + 10h], eax
loc_2_1C57:
        cmp    byte ptr [esi + 18h], 0
        je     SHORT loc_2_1C66
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 01Ah, 0FEh, 0FFh, 0FFh
loc_2_1C66:
        mov    eax, 1
        mov    cl, byte ptr [esi + 19h]
        shl    eax, cl
        or     dword ptr [ebx + 28h], eax
        push   0
        push   1ah
        push   esi
        db  0E8h, 0A7h, 0FEh, 0FFh, 0FFh
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_2_1C86:
        mov    edi, 0c001000eh
loc_2_1C8B:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1CA0:
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
        db  0E8h, 045h, 002h, 000h, 000h
        mov    eax, dword ptr [esi + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        sub    edi, edi
        push   eax
        db  0E8h, 02Dh, 002h, 000h, 000h
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    eax, edi
        je     SHORT loc_2_1D24
        sub    eax, eax
        nop
loc_2_1D00:
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
        ja     SHORT loc_2_1D00
loc_2_1D24:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 24h], 0
        jbe    SHORT loc_2_1D8F
loc_2_1D31:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    SHORT loc_2_1D84
        lea    ecx, [ecx + ecx*2]
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
        db  0E8h, 0C2h, 001h, 000h, 000h
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
        db  0E8h, 0A1h, 001h, 000h, 000h
        dec    dword ptr [esi + 24h]
        jmp    SHORT loc_2_1D87
loc_2_1D84:
        inc    dword ptr [ebp - 4]
loc_2_1D87:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 24h], eax
        ja     SHORT loc_2_1D31
loc_2_1D8F:
        mov    dword ptr [ebp - 10h], 0
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_2_1E7A
        mov    edi, dword ptr [ebp + 18h]
loc_2_1DA3:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        call   sub_2_2160
        or     al, al
        je     SHORT loc_2_1DDA
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 18h], ecx
        mov    edx, dword ptr [ebp + 0ch]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 19h]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 18h]
        or     dword ptr [ecx + eax*4], ebx
        jmp    NEAR PTR loc_2_1E68
loc_2_1DDA:
        mov    ecx, dword ptr [esi + 24h]
        cmp    dword ptr [esi + 20h], ecx
        jbe    NEAR PTR loc_2_1EB4
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
        db  0E8h, 01Ch, 001h, 000h, 000h
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
        db  0E8h, 0E4h, 000h, 000h, 000h
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
loc_2_1E68:
        add    edi, 6
        inc    dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp + 14h], eax
        ja     NEAR PTR loc_2_1DA3
loc_2_1E7A:
        mov    eax, dword ptr [esi + 24h]
        sub    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], eax
        sub    edx, edx
        cmp    dword ptr [ebp - 0ch], edx
        je     SHORT loc_2_1F05
        mov    edi, dword ptr [ebp - 20h]
        sub    ecx, ecx
        nop
loc_2_1E90:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_1F0B
        mov    eax, dword ptr [esi + 4]
        add    eax, ecx
        mov    ebx, dword ptr [edi + 2]
        cmp    dword ptr [eax + 2], ebx
        jne    SHORT loc_2_1EF2
        mov    bx, word ptr [edi]
        cmp    word ptr [eax], bx
        jne    SHORT loc_2_1EF2
        mov    dword ptr [ebp - 8], 0
        jmp    SHORT loc_2_1EF9
loc_2_1EB4:
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 056h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        db  0E8h, 042h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 24h], eax
        mov    eax, 0c0010009h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_2_1EF2:
        mov    dword ptr [ebp - 8], 1
loc_2_1EF9:
        add    edi, 6
        add    ecx, 6
        inc    edx
loc_2_1F00:
        cmp    dword ptr [ebp - 0ch], edx
        ja     SHORT loc_2_1E90
loc_2_1F05:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_1F70
loc_2_1F0B:
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
loc_2_1F20:
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   dword ptr [esi + 14h]
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_2_1F72
        cmp    edi, 103h
        je     SHORT loc_2_1F72
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 0CBh, 0FFh, 0FFh, 0FFh
        push   ebx
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 0BDh, 0FFh, 0FFh, 0FFh
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Bh, 0FFh
loc_2_1F70:
        sub    edi, edi
loc_2_1F72:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 049h, 000h
sub_2_1F80:
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
        je     SHORT loc_2_1FBD
        lea    esp, [esp]
loc_2_1FB0:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [edi + 0ch], ecx
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_2_1FB0
loc_2_1FBD:
        mov    ecx, dword ptr [edi + 0ch]
        cmp    esi, ecx
        je     SHORT loc_2_1FF1
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
        je     SHORT loc_2_1FF3
        cmp    eax, 103h
        je     SHORT loc_2_1FF3
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 8], ecx
        mov    dword ptr [edi + 0ch], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_1FF1:
        sub    eax, eax
loc_2_1FF3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h
sub_2_2000:
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
        je     SHORT loc_2_2032
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_2_2027:
        test   dword ptr [esi], edx
        je     SHORT loc_2_202C
        inc    eax
loc_2_202C:
        add    esi, 4
        dec    edi
        jne    SHORT loc_2_2027
loc_2_2032:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_2040:
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
        jbe    SHORT loc_2_20BC
        mov    edi, dword ptr [ebp + 1ch]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 10h], eax
        mov    edi, edi
loc_2_2080:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 10h]
        test   dword ptr [ecx + esi], eax
        je     SHORT loc_2_20AA
        cmp    dword ptr [ebp + 14h], 6
        jb     SHORT loc_2_20D6
        sub    dword ptr [ebp + 14h], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        db  0E8h, 07Ch, 0FEh, 0FFh, 0FFh
        add    edi, 6
        inc    dword ptr [ebp - 0ch]
loc_2_20AA:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 24h], eax
        ja     SHORT loc_2_2080
loc_2_20BC:
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
loc_2_20D6:
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
sub_2_2100:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 24h]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 10h]
        jbe    SHORT loc_2_2130
        mov    dword ptr [eax], 0c0000001h
        mov    eax, dword ptr [ebp + 14h]
        pop    ebp
        mov    dword ptr [eax], 0
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_2130:
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
        db  0E8h, 0CCh, 0FDh, 0FFh, 0FFh
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_2160:
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
        je     NEAR PTR loc_2_21F9
        dec    esi
        cmp    esi, edx
        jb     SHORT loc_2_21F9
        nop
loc_2_2180:
        cmp    edx, ecx
        jb     SHORT loc_2_21F9
        mov    eax, dword ptr [ebp + 0ch]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 10h]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    SHORT loc_2_21A3
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_2_21D7
loc_2_21A3:
        jbe    SHORT loc_2_21B0
        mov    dword ptr [ebp - 4], 0ffffffffh
        jmp    SHORT loc_2_21D7
        db  08Bh, 0FFh
loc_2_21B0:
        mov    eax, dword ptr [ebp + 10h]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    SHORT loc_2_21C7
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_2_21D7
loc_2_21C7:
        mov    dword ptr [ebp - 4], 0ffffffffh
        ja     SHORT loc_2_21D7
        mov    dword ptr [ebp - 4], 0
loc_2_21D7:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_2209
        jle    SHORT loc_2_21E8
        or     edx, edx
        je     SHORT loc_2_21F9
        lea    esi, [edx - 1]
        jmp    SHORT loc_2_21EB
loc_2_21E8:
        lea    ecx, [edx + 1]
loc_2_21EB:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    SHORT loc_2_2180
loc_2_21F9:
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_2209:
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
