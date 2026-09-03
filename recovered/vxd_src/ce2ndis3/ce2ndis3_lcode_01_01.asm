; ce2ndis3_lcode_01_01.asm -- CE2NDIS3 object 1 segment _LTEXT  (part 1/2)
.386p
_LTEXT segment dword public use32 'LCODE'

        db  055h, 08Bh, 054h, 024h, 008h, 0ECh, 08Bh, 0ECh, 05Dh, 0C2h, 004h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0010:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        in     ax, dx
        mov    ebp, esp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 0C3h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h
        db  000h, 000h, 000h
sub_1_0030:
        push   ebp
        mov    eax, 9
        mov    ebp, esp
        sub    esp, 4ch
        mov    word ptr [ebp - 14h], ax
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ebp - 12h], ax
        push   0
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   edx
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 4], 0
        db  0C7h, 045h, 0F0h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_4FFA
        push   -1
        lea    ecx, [ebp - 0ch]
        push   0
        push   8
        push   ecx
        call   sub_1_50E6
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     SHORT loc_1_0085
        mov    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0085:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx], eax
        mov    byte ptr [ebp - 4ch], 3
        mov    byte ptr [ebp - 4bh], 0
        mov    dword ptr [ebp - 4ah], 0
        db  0C7h, 045h, 0BAh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0BEh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0C2h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0C6h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0CAh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0CEh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0D2h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [ebp - 10h]
        push   36h
        mov    dword ptr [ebp - 1eh], edx
        lea    ecx, [ebp - 4ch]
        mov    dword ptr [ebp - 1ah], eax
        push   ecx
        mov    edx, dword ptr [ebp - 0ch]
        push   edx
        mov    eax, dword ptr [ebp - 8]
        push   eax
        lea    ecx, [ebp - 4]
        db  0C7h, 045h, 0D6h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0DAh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0DEh, 000h, 000h, 000h, 000h
        lea    eax, [edx + 4]
        push   eax
        push   ecx
        call   sub_1_5006
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_011A
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_5000
loc_1_011A:
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0130:
        push   ebp
        mov    eax, 0ch
        mov    ebp, esp
        sub    esp, 80h
        mov    word ptr [ebp - 60h], ax
        mov    ecx, 10h
        mov    word ptr [ebp - 5eh], ax
        push   esi
        mov    word ptr [ebp - 58h], cx
        push   edi
        mov    word ptr [ebp - 56h], cx
        mov    eax, 9
        db  0C7h, 045h, 0A4h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0ACh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0B4h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0BCh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 50h], ax
        mov    word ptr [ebp - 4eh], ax
        mov    eax, 15h
        db  0C7h, 045h, 0C4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 48h], ax
        mov    word ptr [ebp - 46h], ax
        mov    eax, 0bh
        db  0C7h, 045h, 0CCh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 40h], ax
        mov    word ptr [ebp - 3eh], ax
        mov    eax, 8
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 38h], ax
        db  0C7h, 045h, 0DCh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 36h], ax
        push   -1
        mov    word ptr [ebp - 28h], cx
        mov    eax, 0eh
        mov    word ptr [ebp - 30h], ax
        push   0
        mov    word ptr [ebp - 2eh], ax
        push   707h
        mov    word ptr [ebp - 26h], cx
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_1_50E6
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    NEAR PTR loc_1_04E4
        mov    edi, dword ptr [ebp - 8]
        xor    eax, eax
        mov    ecx, 1c1h
        rep stosd
        stosw
        stosb
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 4]
        push   esi
        lea    eax, [ebp - 10h]
        push   eax
        xor    edi, edi
        mov    dword ptr [ecx + 48h], edx
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_5164
        cmp    dword ptr [ebp - 4], edi
        jne    NEAR PTR loc_1_04CF
        push   edi
        lea    eax, [ebp - 60h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 0ch]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_516A
        cmp    dword ptr [ebp - 4], edi
        jne    NEAR PTR loc_1_04C6
        mov    word ptr [ebp - 80h], di
        lea    eax, [ebp - 7eh]
        lea    ecx, [ebp - 1ch]
        lea    edx, [ebp - 20h]
        push   ecx
        mov    dword ptr [eax], edi
        push   edx
        mov    dword ptr [eax + 4], edi
        push   10h
        mov    dword ptr [eax + 8], edi
        mov    word ptr [eax + 0ch], di
        lea    ecx, [ebp - 4]
        lea    eax, [ebp - 80h]
        push   eax
        push   2
        push   esi
        push   ecx
        call   sub_1_50B4
        add    esp, 1ch
        cmp    dword ptr [ebp - 4], edi
        jne    NEAR PTR loc_1_04C6
        mov    ecx, dword ptr [ebp - 7ch]
        xor    eax, eax
        mov    ax, word ptr [ebp - 7ah]
        and    ecx, 0ffffh
        sub    eax, ecx
        cmp    eax, 0fh
        jne    SHORT loc_1_029E
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 6ch], ecx
loc_1_029E:
        lea    edi, [ebp - 7eh]
        xor    eax, eax
        mov    word ptr [ebp - 80h], 0
        mov    ecx, 7
        rep stosd
        stosw
        lea    eax, [ebp - 1ch]
        lea    ecx, [ebp - 20h]
        push   eax
        lea    edx, [ebp - 80h]
        push   ecx
        lea    eax, [ebp - 4]
        push   20h
        push   edx
        push   1
        push   esi
        push   eax
        call   sub_1_50B4
        add    esp, 1ch
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_04C6
        mov    eax, dword ptr [ebp - 7ch]
        mov    ecx, dword ptr [ebp - 8]
        push   1
        mov    dword ptr [ecx + 74h], eax
        mov    edx, dword ptr [ebp - 6ch]
        mov    ecx, dword ptr [ebp - 8]
        lea    eax, [ebp - 58h]
        mov    dword ptr [ecx + 7ch], edx
        push   eax
        mov    edx, dword ptr [ebp - 10h]
        push   edx
        lea    ecx, [ebp - 0ch]
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_516A
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_0329
        push   1
        lea    eax, [ebp - 50h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 0ch]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_516A
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_04C6
loc_1_0329:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        push   0
        mov    edx, dword ptr [eax + 4]
        lea    eax, [ebp - 48h]
        mov    dword ptr [ecx + 84h], edx
        push   eax
        mov    edx, dword ptr [ebp - 10h]
        push   edx
        lea    ecx, [ebp - 0ch]
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_516A
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_0361
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 10h], 9
        jmp    SHORT loc_1_036D
loc_1_0361:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 10h], edx
loc_1_036D:
        mov    eax, dword ptr [ebp - 10h]
        lea    ecx, [ebp - 18h]
        push   eax
        lea    edx, [ebp - 14h]
        push   ecx
        lea    eax, [ebp - 4]
        push   edx
        push   eax
        call   sub_1_506C
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_03A9
        cmp    dword ptr [ebp - 18h], 6
        jne    SHORT loc_1_03A9
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 14h]
        add    edx, 0ah
        mov    eax, ecx
        mov    edi, dword ptr [ecx]
        mov    dword ptr [edx], edi
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [edx + 4], al
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [edx + 5], cl
loc_1_03A9:
        mov    eax, dword ptr [ebp - 8]
        push   1
        lea    ecx, [ebp - 40h]
        mov    byte ptr [eax + 92h], 0
        push   ecx
        mov    edx, dword ptr [ebp - 10h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_516A
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_03E7
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
        dec    eax
        cmp    eax, 1
        mov    eax, dword ptr [ebp - 8]
        sbb    cl, cl
        neg    cl
        mov    byte ptr [eax + 92h], cl
loc_1_03E7:
        mov    eax, dword ptr [ebp - 8]
        push   1
        lea    ecx, [ebp - 38h]
        mov    byte ptr [eax + 94h], 0
        push   ecx
        mov    edx, dword ptr [ebp - 10h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_516A
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_0425
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
        dec    eax
        cmp    eax, 1
        mov    eax, dword ptr [ebp - 8]
        sbb    cl, cl
        neg    cl
        mov    byte ptr [eax + 94h], cl
loc_1_0425:
        mov    eax, dword ptr [ebp - 8]
        push   1
        lea    ecx, [ebp - 30h]
        mov    byte ptr [eax + 95h], 1
        push   ecx
        mov    edx, dword ptr [ebp - 10h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_516A
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_0463
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
        dec    eax
        cmp    eax, 1
        mov    eax, dword ptr [ebp - 8]
        sbb    cl, cl
        neg    cl
        mov    byte ptr [eax + 95h], cl
loc_1_0463:
        mov    eax, dword ptr [ebp - 8]
        push   1
        lea    ecx, [ebp - 28h]
        mov    byte ptr [eax + 93h], 0
        push   ecx
        mov    edx, dword ptr [ebp - 10h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_516A
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_04A1
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
        dec    eax
        cmp    eax, 1
        mov    eax, dword ptr [ebp - 8]
        sbb    cl, cl
        neg    cl
        mov    byte ptr [eax + 93h], cl
loc_1_04A1:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_5170
        push   esi
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        call   sub_1_04F0
        test   eax, eax
        jne    SHORT loc_1_04CF
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_04C6:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_5170
loc_1_04CF:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   707h
        push   eax
        call   sub_1_50EC
        mov    eax, 0c0000001h
loc_1_04E4:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h
sub_1_04F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 600h
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   -1
        lea    eax, [ebp - 8]
        push   ebx
        push   26h
        push   eax
        call   sub_1_50E6
        mov    dword ptr [ebp - 4], eax
        cmp    eax, ebx
        jne    NEAR PTR loc_1_0633
        mov    edi, dword ptr [ebp - 8]
        xor    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, 1
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [eax + 16h], ecx
        mov    edx, dword ptr [ebp - 8]
        mov    eax, dword ptr [esi + 6ch]
        mov    dword ptr [edx + 1ah], eax
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 1eh], 10h
        mov    edx, dword ptr [ebp - 8]
        lea    eax, [esi + 70h]
        mov    dword ptr [edx + 22h], eax
        mov    edx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 0ah], ecx
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 10h]
        push   edx
        push   eax
        push   ecx
        mov    edx, dword ptr [esi + 48h]
        push   esi
        lea    eax, [esi + 4ch]
        push   edx
        push   eax
        call   sub_1_5012
        push   ebx
        mov    ecx, dword ptr [ebp - 8]
        push   26h
        mov    dword ptr [ebp - 4], eax
        push   ecx
        call   sub_1_50EC
        cmp    dword ptr [ebp - 4], ebx
        jne    NEAR PTR loc_1_0633
        push   esi
        call   sub_1_2260
        mov    dword ptr [esi], eax
        cmp    eax, ebx
        je     NEAR PTR loc_1_0633
        db  0B9h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 2ch], ebx
        mov    dword ptr [esi + 30h], ecx
        mov    dword ptr [esi + 34h], esi
        mov    dword ptr [esi + 18h], ebx
        mov    dword ptr [esi + 14h], ebx
        push   2
        mov    eax, dword ptr [esi + 84h]
        push   ebx
        mov    dword ptr [esi + 38h], ebx
        push   eax
        mov    dword ptr [esi + 3ch], ecx
        push   eax
        mov    dword ptr [esi + 40h], esi
        push   ecx
        lea    edx, [esi + 50h]
        push   esi
        mov    ecx, dword ptr [esi + 4ch]
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 4]
        push   ecx
        push   edx
        push   eax
        call   sub_1_50CE
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_1_05FA
        push   ebx
        mov    eax, dword ptr [esi + 4ch]
        push   0aa000005h
        push   eax
        call   sub_1_51B8
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_05FA:
        lea    edi, [esi + 78h]
        push   1000h
        mov    eax, dword ptr [esi + 74h]
        mov    ecx, dword ptr [esi + 4ch]
        push   eax
        lea    edx, [ebp - 4]
        push   ecx
        push   edi
        push   edx
        call   sub_1_51B2
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_063F
loc_1_061A:
        mov    dword ptr [ebp - 4], 0c0000001h
loc_1_0621:
        mov    eax, dword ptr [esi + 4ch]
        push   eax
        call   sub_1_507E
        mov    eax, dword ptr [esi + 4ch]
        push   eax
        call   sub_1_5018
loc_1_0633:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_063F:
        push   1000h
        mov    eax, dword ptr [esi + 7ch]
        push   eax
        mov    ecx, dword ptr [esi + 4ch]
        push   ecx
        lea    edx, [esi + 80h]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_51B2
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_061A
        mov    dword ptr [ebp - 304h], 13h
        lea    eax, [ebp - 304h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        test   byte ptr [ebp - 300h], 2
        je     SHORT loc_1_06D3
        mov    dword ptr [ebp - 304h], 19h
        lea    eax, [ebp - 304h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        mov    dword ptr [ebp - 600h], 16h
        cmp    byte ptr [esi + 95h], 0
        mov    eax, dword ptr [ebp - 300h]
        je     SHORT loc_1_06BB
        or     eax, 2
        jmp    SHORT loc_1_06BE
loc_1_06BB:
        and    eax, 0fffffffdh
loc_1_06BE:
        mov    dword ptr [ebp - 5fch], eax
        lea    eax, [ebp - 600h]
        push   eax
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_1_2820
loc_1_06D3:
        mov    dword ptr [ebp - 304h], 15h
        lea    eax, [ebp - 304h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        test   byte ptr [ebp - 300h], 1
        je     SHORT loc_1_0744
        mov    dword ptr [ebp - 304h], 1bh
        lea    eax, [ebp - 304h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        mov    dword ptr [ebp - 600h], 18h
        cmp    byte ptr [esi + 94h], 0
        mov    eax, dword ptr [ebp - 300h]
        je     SHORT loc_1_072C
        or     eax, 1
        jmp    SHORT loc_1_072F
loc_1_072C:
        and    eax, 0fffffffeh
loc_1_072F:
        mov    dword ptr [ebp - 5fch], eax
        lea    eax, [ebp - 600h]
        push   eax
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_1_2820
loc_1_0744:
        mov    dword ptr [ebp - 304h], 15h
        lea    eax, [ebp - 304h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        test   byte ptr [ebp - 300h], 10h
        je     SHORT loc_1_07B5
        mov    dword ptr [ebp - 304h], 1bh
        lea    eax, [ebp - 304h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        mov    dword ptr [ebp - 600h], 18h
        cmp    byte ptr [esi + 92h], 0
        mov    eax, dword ptr [ebp - 300h]
        je     SHORT loc_1_079D
        or     eax, 10h
        jmp    SHORT loc_1_07A0
loc_1_079D:
        and    eax, 0ffffffefh
loc_1_07A0:
        mov    dword ptr [ebp - 5fch], eax
        lea    eax, [ebp - 600h]
        push   eax
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_1_2820
loc_1_07B5:
        xor    ebx, ebx
        lea    ecx, [ebp - 304h]
        mov    dword ptr [ebp - 304h], 2eh
        push   ecx
        mov    dword ptr [ebp - 300h], ebx
        mov    eax, dword ptr [esi + 6ch]
        mov    dword ptr [ebp - 2fch], eax
        mov    edx, dword ptr [esi]
        push   edx
        call   sub_1_2820
        mov    dword ptr [ebp - 300h], 8
        mov    ecx, dword ptr [esi + 84h]
        lea    edx, [ebp - 304h]
        push   edx
        mov    dword ptr [ebp - 2fch], ecx
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_2820
        mov    dword ptr [ebp - 300h], 4
        mov    ecx, dword ptr [edi]
        lea    edx, [ebp - 304h]
        push   edx
        mov    edi, 1000h
        mov    dword ptr [ebp - 2fch], ecx
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_2820
        mov    dword ptr [ebp - 300h], 6
        lea    ecx, [ebp - 304h]
        mov    dword ptr [ebp - 2fch], edi
        push   ecx
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_2820
        mov    dword ptr [ebp - 300h], 5
        mov    ecx, dword ptr [esi + 80h]
        lea    edx, [ebp - 304h]
        push   edx
        mov    dword ptr [ebp - 2fch], ecx
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_2820
        mov    dword ptr [ebp - 300h], 7
        lea    ecx, [ebp - 304h]
        mov    dword ptr [ebp - 2fch], edi
        push   ecx
        mov    edx, dword ptr [esi]
        push   edx
        call   sub_1_2820
        lea    ecx, [ebp - 304h]
        mov    dword ptr [ebp - 304h], ebx
        push   ecx
        mov    edx, dword ptr [esi]
        push   edx
        call   sub_1_2820
        test   eax, eax
        je     SHORT loc_1_08C5
        cmp    byte ptr [esi + 93h], bl
        jne    SHORT loc_1_08C5
        lea    eax, [ebp - 304h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        test   eax, eax
        jne    NEAR PTR loc_1_061A
loc_1_08C5:
        mov    edi, 1
        mov    dword ptr [ebp - 304h], edi
loc_1_08D0:
        lea    eax, [ebp - 304h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        test   eax, eax
        je     SHORT loc_1_08F2
        cmp    edi, 5
        jge    NEAR PTR loc_1_061A
        inc    edi
        cmp    edi, 5
        jle    SHORT loc_1_08D0
loc_1_08F2:
        mov    dword ptr [ebp - 304h], 23h
        lea    eax, [ebp - 304h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        test   eax, eax
        jne    NEAR PTR loc_1_0621
        xor    ecx, ecx
loc_1_0915:
        lea    eax, [ecx + esi]
        mov    dl, byte ptr [ebp + ecx - 300h]
        inc    ecx
        mov    byte ptr [eax + 0ah], dl
        mov    byte ptr [eax + 4], dl
        cmp    ecx, 6
        jl     SHORT loc_1_0915
        lea    eax, [esi + 68h]
        lea    ecx, [esi + 44h]
        push   eax
        lea    edx, [esi + 0ah]
        push   ecx
        mov    eax, dword ptr [esi + 10h]
        push   edx
        add    eax, 5
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0952:
        test   al, al
        jne    SHORT loc_1_0972
        push   3
        mov    eax, dword ptr [esi + 4ch]
        push   2
        push   2
        push   0aa000002h
        push   eax
        call   sub_1_51B8
        add    esp, 14h
        jmp    NEAR PTR loc_1_061A
loc_1_0972:
        mov    dword ptr [ebp - 304h], 2fh
        lea    eax, [ebp - 304h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_09A0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 300h
        mov    dword ptr [eax], 0
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [edx], 0ffffffffh
        lea    eax, [eax + ecx*4]
loc_1_09C7:
        test   ecx, ecx
        je     SHORT loc_1_09D6
        sub    eax, 4
        dec    ecx
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_09C7
        mov    dword ptr [edx], ecx
loc_1_09D6:
        cmp    dword ptr [edx], -1
        jne    SHORT loc_1_09E8
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_09E8:
        push   -1
        lea    eax, [ebp - 4]
        push   0
        push   21h
        push   eax
        call   sub_1_50E6
        test   eax, eax
        je     SHORT loc_1_0A1E
        push   0
        mov    esi, dword ptr [ebp + 20h]
        push   0aa000002h
        mov    eax, dword ptr [esi + 4ch]
        push   eax
        call   sub_1_51B8
        add    esp, 0ch
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_0A1E:
        mov    esi, dword ptr [ebp + 20h]
        inc    dword ptr [esi + 8ch]
        cmp    dword ptr [esi + 88h], 0
        jne    NEAR PTR loc_1_0AB8
        lea    eax, [ebp - 300h]
        mov    edi, 2
        mov    dword ptr [ebp - 300h], 4
        push   eax
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_1_2820
        lea    ecx, [ebp - 300h]
        mov    dword ptr [ebp - 300h], edi
        push   ecx
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_2820
        mov    dword ptr [ebp - 300h], 0ch
        mov    dword ptr [ebp - 2fch], 0
        lea    ecx, [ebp - 300h]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_1_2820
        mov    dword ptr [ebp - 2fch], 1
        lea    ecx, [ebp - 300h]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_1_2820
        lea    ecx, [ebp - 300h]
        mov    dword ptr [ebp - 2fch], edi
        push   ecx
        mov    edx, dword ptr [esi]
        push   edx
        call   sub_1_2820
loc_1_0AB8:
        mov    eax, dword ptr [esi + 88h]
        mov    ecx, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx + 0ch], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 88h], ecx
        mov    ecx, dword ptr [esi + 68h]
        mov    eax, dword ptr [ebp - 4]
        add    eax, 10h
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0AE5:
        test   al, al
        jne    SHORT loc_1_0B28
        push   0
        dec    dword ptr [esi + 8ch]
        mov    eax, dword ptr [ebp - 4]
        push   21h
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 88h], ecx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_50EC
        push   0
        mov    ecx, dword ptr [esi + 4ch]
        push   0aa000002h
        push   ecx
        call   sub_1_51B8
        add    esp, 0ch
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_0B28:
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 28h]
        mov    dword ptr [eax + 8], esi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], edi
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], edx
        xor    ecx, ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 1ch], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 5dch
        mov    eax, dword ptr [ebp - 4]
        push   0bb8h
        add    esi, 38h
        mov    dword ptr [eax + 18h], 1
        mov    eax, dword ptr [ebp - 4]
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 20h], cl
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edx], ecx
        call   sub_1_50C2
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Bh, 0FFh
loc_1_0B80:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    dword ptr [eax + 88h], 1
        sbb    al, al
        pop    ebp
        inc    al
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 0C2h, 00Ch, 000h, 08Dh, 09Bh, 000h
        db  000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0BB0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        db  038h, 00Dh, 000h, 000h, 000h, 000h
        push   esi
        push   edi
        je     SHORT loc_1_0C13
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 008h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 8], ecx
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 0ch], ecx
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 10h
        mov    esi, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
        lea    edi, [esi + 0c3h]
        mov    dword ptr [eax], ecx
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 10h], ecx
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 18h], edi
        db  088h, 00Dh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_0C16
loc_1_0C13:
        mov    esi, dword ptr [ebp + 8]
loc_1_0C16:
        cmp    dword ptr [ebp + 10h], 0
        jne    SHORT loc_1_0C86
        db  0A1h, 000h, 000h, 000h, 000h
        mov    edi, dword ptr [ebp + 14h]
        db  068h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 4], edi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 8], edi
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_32F0
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    dword ptr [ecx + 4], 0
        jne    SHORT loc_1_0C86
        cmp    edi, 0eh
        jb     SHORT loc_1_0C86
        lea    eax, [edi - 0eh]
        lea    ecx, [esi + 0d1h]
        push   eax
        mov    dword ptr [esi + 0bfh], edi
        sub    edi, 0eh
        lea    eax, [esi + 0c3h]
        push   edi
        inc    dword ptr [esi + 0abh]
        push   ecx
        push   0eh
        push   eax
        mov    ecx, dword ptr [esi + 68h]
        push   eax
        push   esi
        push   ecx
        call   sub_1_4C90
        mov    ecx, dword ptr [esi + 68h]
        push   ecx
        call   sub_1_4E70
loc_1_0C86:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
loc_1_0C90:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [eax + 8]
        lea    ebx, [esi + 24h]
        cmp    dword ptr [edi + 14h], 0
        je     SHORT loc_1_0CDA
loc_1_0CAC:
        mov    eax, dword ptr [edi + 14h]
        push   eax
        push   edi
        call   sub_1_1F30
        test   eax, eax
        jne    SHORT loc_1_0CDA
        mov    eax, dword ptr [edi + 14h]
        push   0
        push   eax
        mov    ecx, dword ptr [eax + 24h]
        mov    dword ptr [edi + 14h], ecx
        mov    edx, dword ptr [eax + 28h]
        mov    eax, dword ptr [edx]
        push   eax
        call   sub_1_502A
        dec    dword ptr [edi + 1ch]
        cmp    dword ptr [edi + 14h], 0
        jne    SHORT loc_1_0CAC
loc_1_0CDA:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        push   esi
        call   sub_1_5110
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 4]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_5134
        mov    ecx, dword ptr [ebp - 4]
        mov    dl, byte ptr [edi + 0fh]
        cmp    byte ptr [ecx + 5], dl
        jne    SHORT loc_1_0D3B
        mov    al, byte ptr [edi + 0eh]
        cmp    byte ptr [ecx + 4], al
        jne    SHORT loc_1_0D3B
        mov    al, byte ptr [edi + 0dh]
        cmp    byte ptr [ecx + 3], al
        jne    SHORT loc_1_0D3B
        mov    al, byte ptr [edi + 0ch]
        cmp    byte ptr [ecx + 2], al
        jne    SHORT loc_1_0D3B
        mov    eax, ecx
        mov    al, byte ptr [ecx + 1]
        cmp    byte ptr [edi + 0bh], al
        jne    SHORT loc_1_0D3B
        mov    eax, ecx
        mov    al, byte ptr [ecx]
        cmp    byte ptr [edi + 0ah], al
        jne    SHORT loc_1_0D3B
        mov    byte ptr [ebx + 9], 1
        mov    byte ptr [ebx + 8], 1
        jmp    SHORT loc_1_0D4F
loc_1_0D3B:
        mov    byte ptr [ebx + 9], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [edi + 68h]
        push   ecx
        push   edx
        call   sub_1_4F10
        mov    byte ptr [ebx + 8], al
loc_1_0D4F:
        cmp    byte ptr [edi + 96h], 0
        je     SHORT loc_1_0D5C
        mov    byte ptr [ebx + 8], 1
loc_1_0D5C:
        cmp    byte ptr [ebx + 8], 0
        je     SHORT loc_1_0D9E
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebx], 0
        mov    dword ptr [ebx + 4], eax
        cmp    dword ptr [edi + 20h], 0
        jne    SHORT loc_1_0D79
        mov    dword ptr [edi + 20h], esi
        jmp    SHORT loc_1_0D7F
loc_1_0D79:
        mov    eax, dword ptr [edi + 24h]
        mov    dword ptr [eax + 24h], esi
loc_1_0D7F:
        mov    dword ptr [edi + 24h], esi
        push   1
        inc    dword ptr [edi + 28h]
        add    edi, 2ch
        push   edi
        call   sub_1_50C2
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0D9E:
        cmp    dword ptr [edi + 14h], 0
        je     SHORT loc_1_0DD5
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebx], 0
        push   1
        add    edi, 2ch
        push   edi
        mov    dword ptr [ebx + 4], eax
        mov    ecx, dword ptr [edi - 14h]
        mov    dword ptr [ecx + 24h], esi
        mov    dword ptr [edi - 14h], esi
        inc    dword ptr [edi - 10h]
        call   sub_1_50C2
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0DD5:
        push   esi
        push   edi
        call   sub_1_1F30
        test   eax, eax
        jne    SHORT loc_1_0DF1
        xor    eax, eax
        inc    dword ptr [edi + 0a7h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0DF1:
        mov    eax, dword ptr [ebp + 8]
        push   1
        add    edi, 2ch
        mov    dword ptr [ebx], 0
        push   edi
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [edi - 18h], esi
        mov    dword ptr [edi - 14h], esi
        inc    dword ptr [edi - 10h]
        call   sub_1_50C2
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
loc_1_0E20:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 0ch
        mov    eax, dword ptr [eax + 0bfh]
        push   ebx
        push   esi
        sub    eax, 0eh
        push   edi
        mov    esi, dword ptr [ebp + 14h]
        mov    edi, dword ptr [ebp + 10h]
        lea    ecx, [edi + esi]
        cmp    eax, ecx
        jae    SHORT loc_1_0E5A
        mov    eax, dword ptr [ebp + 1ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0
        mov    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_0E5A:
        add    edi, 0eh
        push   0
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp + 18h]
        push   eax
        push   0
        push   0
        push   ecx
        call   sub_1_5110
        lea    ecx, [ebp - 0ch]
        lea    edx, [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   edx
        push   eax
        call   sub_1_5134
        mov    ecx, dword ptr [ebp + 1ch]
        test   esi, esi
        mov    dword ptr [ecx], 0
        je     SHORT loc_1_0EE1
loc_1_0E8E:
        mov    ebx, dword ptr [ebp - 0ch]
        cmp    ebx, esi
        jb     SHORT loc_1_0E97
        mov    ebx, esi
loc_1_0E97:
        push   ebx
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, edi
        mov    ecx, dword ptr [ebp - 8]
        add    eax, 0c3h
        add    edi, ebx
        push   eax
        sub    esi, ebx
        push   ecx
        call   sub_1_51CC
        mov    eax, dword ptr [ebp + 1ch]
        add    dword ptr [eax], ebx
        test   esi, esi
        je     SHORT loc_1_0EE1
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_5158
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_0EE1
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 8]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_5134
        test   esi, esi
        jne    SHORT loc_1_0E8E
loc_1_0EE1:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 064h, 024h, 000h
loc_1_0EF0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [esi + 8]
        inc    dword ptr [esi + 18h]
        inc    dword ptr [edi + 8ch]
        mov    ecx, dword ptr [eax + 10h]
        test   ecx, ecx
        jne    SHORT loc_1_0F18
        push   eax
        push   esi
        push   edi
        call   sub_1_0F40
        jmp    SHORT loc_1_0F2C
loc_1_0F18:
        cmp    ecx, 1
        jne    SHORT loc_1_0F27
        push   eax
        push   esi
        push   edi
        call   sub_1_0F90
        jmp    SHORT loc_1_0F2C
loc_1_0F27:
        mov    eax, 10001h
loc_1_0F2C:
        dec    dword ptr [esi + 18h]
        dec    dword ptr [edi + 8ch]
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_0F40:
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
        push   eax
        push   edx
        push   ecx
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 14h]
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   eax
        push   edx
        call   sub_1_1140
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 20h], ecx
        mov    dword ptr [esi + 24h], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0F90:
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
        ja     SHORT loc_1_0FCD
        je     NEAR PTR loc_1_10E9
        cmp    eax, 1010eh
        je     SHORT loc_1_0FDF
        cmp    eax, 1010fh
        je     SHORT loc_1_1040
        cmp    eax, 10112h
        je     NEAR PTR loc_1_10C2
loc_1_0FCD:
        mov    ebx, 0c0010017h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_1124
loc_1_0FDF:
        cmp    edi, 4
        je     SHORT loc_1_0FFC
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 4
        mov    ebx, 0c0010014h
        jmp    NEAR PTR loc_1_1124
loc_1_0FFC:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_51CC
        test   dword ptr [ebp - 8], 0f054h
        je     SHORT loc_1_1029
        mov    dword ptr [esi + 20h], 4
        mov    dword ptr [esi + 24h], 0
        mov    ebx, 0c00000bbh
        jmp    NEAR PTR loc_1_1124
loc_1_1029:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edx
        call   sub_1_1630
        jmp    NEAR PTR loc_1_1122
loc_1_1040:
        cmp    edi, 4
        je     SHORT loc_1_1057
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_1124
loc_1_1057:
        push   4
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        call   sub_1_51CC
        inc    dword ptr [ebp - 4]
        and    dword ptr [ebp - 4], 0fffffffeh
        cmp    dword ptr [ebp - 4], 5dch
        ja     SHORT loc_1_10BB
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 9bh]
        cmp    eax, dword ptr [ebp - 4]
        jae    SHORT loc_1_1095
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 9bh], eax
        mov    dword ptr [ecx + 14h], eax
        jmp    NEAR PTR loc_1_1124
loc_1_1095:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [ecx + 14h], eax
        jne    SHORT loc_1_10B3
        mov    eax, dword ptr [ecx + 14h]
        cmp    eax, dword ptr [ebp - 4]
        jbe    SHORT loc_1_10B3
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    dword ptr [ecx + 14h], eax
        call   sub_1_1660
        jmp    SHORT loc_1_1124
loc_1_10B3:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 14h], eax
        jmp    SHORT loc_1_1124
loc_1_10BB:
        mov    ebx, 0c0010014h
        jmp    SHORT loc_1_1124
loc_1_10C2:
        cmp    edi, 4
        je     SHORT loc_1_10D6
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_1_1124
loc_1_10D6:
        push   4
        xor    ebx, ebx
        push   ecx
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 1ch
        push   ecx
        call   sub_1_51CC
        jmp    SHORT loc_1_1124
loc_1_10E9:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        test   edx, edx
        je     SHORT loc_1_1107
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_1_1124
loc_1_1107:
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
        call   sub_1_16A0
loc_1_1122:
        mov    ebx, eax
loc_1_1124:
        test   ebx, ebx
        jne    SHORT loc_1_1132
        mov    dword ptr [esi + 24h], 0
        mov    dword ptr [esi + 20h], edi
loc_1_1132:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
sub_1_1140:
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
        ja     SHORT loc_1_117D
        je     NEAR PTR loc_1_12F7
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_1_119E
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_117D:
        cmp    eax, 1010102h
        je     NEAR PTR loc_1_1316
        cmp    eax, 1010103h
        je     NEAR PTR loc_1_1335
        cmp    eax, 1010104h
        je     NEAR PTR loc_1_13F1
loc_1_119E:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_1_13F8
loc_1_11AA:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_1_11BF
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, 5ch
        jmp    NEAR PTR loc_1_13F8
loc_1_11BF:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, 7ch
        jmp    NEAR PTR loc_1_13F8
loc_1_11CE:
        lea    esi, [ebp - 14h]
        mov    edi, 4
        mov    dword ptr [ebp - 14h], 0
        jmp    NEAR PTR loc_1_13F8
loc_1_11E2:
        lea    esi, [ebp - 18h]
        mov    edi, 4
        jmp    NEAR PTR loc_1_13F8
loc_1_11EF:
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_1_13F8
loc_1_11FB:
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_1_13F8
loc_1_1207:
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_1_13F8
loc_1_1213:
        mov    dword ptr [ebp - 8], 1388h
        jmp    NEAR PTR loc_1_13F8
loc_1_121F:
        mov    dword ptr [ebp - 8], 6000h
        jmp    NEAR PTR loc_1_13F8
loc_1_122B:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_1_13F8
loc_1_1237:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_1_13F8
loc_1_1243:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 4
        lea    eax, [ebp - 8]
        push   ebx
        lea    esi, [ebp - 8]
        push   eax
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0
        call   sub_1_51CC
        and    dword ptr [ebp - 8], 0ffffff00h
        jmp    NEAR PTR loc_1_13F8
loc_1_1270:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, 31h
        jmp    NEAR PTR loc_1_13F8
loc_1_127F:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_1296
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 68h]
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_1_13F8
loc_1_1296:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 10h]
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 8], edx
        jmp    NEAR PTR loc_1_13F8
loc_1_12A7:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_12BE
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 9bh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_13F8
loc_1_12BE:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_1_13F8
loc_1_12CC:
        lea    esi, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 300h
        jmp    NEAR PTR loc_1_13F8
loc_1_12DF:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_1_13F8
loc_1_12EB:
        mov    dword ptr [ebp - 8], 0fh
        jmp    NEAR PTR loc_1_13F8
loc_1_12F7:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 4
        lea    eax, [ebp - 20h]
        push   ebx
        lea    esi, [ebp - 20h]
        push   eax
        mov    edi, 6
        call   sub_1_51CC
        jmp    NEAR PTR loc_1_13F8
loc_1_1316:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 0ah
        lea    eax, [ebp - 20h]
        push   ebx
        lea    esi, [ebp - 20h]
        push   eax
        mov    edi, 6
        call   sub_1_51CC
        jmp    NEAR PTR loc_1_13F8
loc_1_1335:
        cmp    byte ptr [ebp + 14h], 0
        mov    ebx, dword ptr [ebp + 8]
        je     SHORT loc_1_1392
        mov    ecx, dword ptr [ebx + 68h]
        mov    eax, dword ptr [ecx + 24h]
        mov    dword ptr [ebp - 10h], eax
        lea    ebx, [eax + eax*2]
        lea    edx, [ebx*2]
        cmp    dword ptr [ebp + 1ch], edx
        jae    SHORT loc_1_1367
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    dword ptr [eax], edx
        jmp    NEAR PTR loc_1_13F8
loc_1_1367:
        mov    eax, dword ptr [ebp + 18h]
        lea    edx, [ebp - 10h]
        push   eax
        mov    ebx, dword ptr [ebp + 1ch]
        push   edx
        lea    eax, [ebp - 0ch]
        push   ebx
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_137D:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ecx], eax
        je     SHORT loc_1_13FE
        jmp    SHORT loc_1_13F8
loc_1_1392:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebx + 68h]
        mov    ecx, dword ptr [eax + 10h]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_13A2:
        lea    edx, [eax + eax*2]
        mov    dword ptr [ebp - 10h], eax
        lea    ecx, [edx*2]
        cmp    dword ptr [ebp + 1ch], ecx
        jae    SHORT loc_1_13C2
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    dword ptr [eax], ecx
        jmp    SHORT loc_1_13F8
loc_1_13C2:
        mov    eax, dword ptr [ebp + 18h]
        lea    ecx, [ebp - 10h]
        push   eax
        mov    edx, dword ptr [ebp + 1ch]
        push   ecx
        mov    eax, dword ptr [ebp + 0ch]
        push   edx
        mov    ecx, dword ptr [eax + 10h]
        push   ecx
        mov    edx, dword ptr [ebx + 68h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_13E2:
        mov    ecx, dword ptr [ebp - 10h]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [ebp + 24h]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_13F8
loc_1_13F1:
        mov    dword ptr [ebp - 8], 9
loc_1_13F8:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_1434
loc_1_13FE:
        cmp    dword ptr [ebp + 10h], 1010103h
        je     SHORT loc_1_1434
        cmp    dword ptr [ebp + 1ch], edi
        jae    SHORT loc_1_1424
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
loc_1_1424:
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        push   esi
        push   eax
        call   sub_1_51CC
        mov    ecx, dword ptr [ebp + 24h]
        add    dword ptr [ecx], edi
loc_1_1434:
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 08Dh, 064h, 024h, 000h
loc_1_1490:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    edx, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [edx + 14h]
        cmp    ecx, 10113h
        ja     SHORT loc_1_14B8
        je     SHORT loc_1_14EF
        cmp    ecx, 10101h
        jb     SHORT loc_1_14EA
        cmp    ecx, 10111h
        jbe    SHORT loc_1_14EF
        jmp    SHORT loc_1_14EA
loc_1_14B8:
        cmp    ecx, 1010104h
        ja     SHORT loc_1_14DA
        cmp    ecx, 1010101h
        jae    SHORT loc_1_14EF
        cmp    ecx, 20101h
        jb     SHORT loc_1_14EA
        cmp    ecx, 20105h
        jbe    SHORT loc_1_14EF
        jmp    SHORT loc_1_14EA
loc_1_14DA:
        cmp    ecx, 1020101h
        jb     SHORT loc_1_14EA
        cmp    ecx, 1020103h
        jbe    SHORT loc_1_14EF
loc_1_14EA:
        mov    eax, 0c0010017h
loc_1_14EF:
        test   eax, eax
        jne    SHORT loc_1_14FD
        push   edx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1510
loc_1_14FD:
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1510:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 0ch], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        lea    edx, [ebp - 8]
        mov    eax, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 18h]
        push   edx
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 10h], ecx
        lea    eax, [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 14h]
        push   eax
        mov    edx, dword ptr [ebp - 10h]
        push   ecx
        mov    eax, dword ptr [esi + 14h]
        push   edx
        mov    ebx, dword ptr [ebp + 8]
        push   1
        push   eax
        push   0
        push   ebx
        call   sub_1_1140
        cmp    eax, 0c00000bbh
        mov    edi, eax
        jne    NEAR PTR loc_1_15F8
        xor    edi, edi
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 1020101h
        ja     SHORT loc_1_1583
        je     SHORT loc_1_15C1
        sub    eax, 20101h
        cmp    eax, 4
        ja     SHORT loc_1_1591
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_1583:
        cmp    eax, 1020102h
        je     SHORT loc_1_15CA
        cmp    eax, 1020103h
        je     SHORT loc_1_15D2
loc_1_1591:
        mov    edi, 0c0010017h
        jmp    SHORT loc_1_15DB
loc_1_1598:
        mov    eax, dword ptr [ebx + 0a7h]
        jmp    SHORT loc_1_15D8
loc_1_15A0:
        mov    eax, dword ptr [ebx + 0abh]
        jmp    SHORT loc_1_15D8
loc_1_15A8:
        mov    eax, dword ptr [ebx + 0afh]
        jmp    SHORT loc_1_15D8
loc_1_15B0:
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_15DB
loc_1_15B9:
        mov    eax, dword ptr [ebx + 0bbh]
        jmp    SHORT loc_1_15D8
loc_1_15C1:
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_15DB
loc_1_15CA:
        mov    eax, dword ptr [ebx + 0b3h]
        jmp    SHORT loc_1_15D8
loc_1_15D2:
        mov    eax, dword ptr [ebx + 0b7h]
loc_1_15D8:
        mov    dword ptr [ebp - 4], eax
loc_1_15DB:
        cmp    dword ptr [ebp - 14h], 4
        jb     SHORT loc_1_15F8
        test   edi, edi
        jne    SHORT loc_1_15F8
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        call   sub_1_51CC
        add    dword ptr [ebp - 8], 4
loc_1_15F8:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], ecx
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h
        db  000h
sub_1_1630:
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        mov    ebp, esp
        push   esi
        push   1
        push   eax
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 10h]
        inc    dword ptr [esi + 18h]
        push   edx
        mov    ecx, dword ptr [eax + 68h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_1655:
        dec    dword ptr [esi + 18h]
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 049h, 000h
sub_1_1660:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 88h]
        cmp    edx, ecx
        je     SHORT loc_1_1683
loc_1_1673:
        mov    esi, dword ptr [edx + 14h]
        cmp    ecx, esi
        jae    SHORT loc_1_167C
        mov    ecx, esi
loc_1_167C:
        mov    edx, dword ptr [edx + 0ch]
        test   edx, edx
        jne    SHORT loc_1_1673
loc_1_1683:
        test   ecx, ecx
        jne    SHORT loc_1_168C
        mov    ecx, 5dch
loc_1_168C:
        pop    esi
        mov    dword ptr [eax + 9bh], ecx
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_16A0:
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    ebp, esp
        push   1
        push   eax
        mov    ecx, dword ptr [ebp + 14h]
        push   ecx
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 10h]
        push   ecx
        mov    eax, dword ptr [edx + 68h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_16C5:
        pop    ebp
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_16D0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 300h
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        mov    edi, dword ptr [esi + 88h]
        mov    dword ptr [ebp - 4], 0
        test   edi, edi
        je     SHORT loc_1_170D
loc_1_16F3:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [edi]
        push   40010004h
        push   ecx
        call   sub_1_504E
        mov    edi, dword ptr [edi + 0ch]
        test   edi, edi
        jne    SHORT loc_1_16F3
loc_1_170D:
        mov    dword ptr [ebp - 300h], 0dh
        lea    eax, [ebp - 300h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        mov    esi, dword ptr [esi + 88h]
        mov    dword ptr [ebp - 4], 0
        test   esi, esi
        je     SHORT loc_1_1751
loc_1_1737:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [esi]
        push   40010005h
        push   ecx
        call   sub_1_504E
        mov    esi, dword ptr [esi + 0ch]
        test   esi, esi
        jne    SHORT loc_1_1737
loc_1_1751:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_1760:
        push   ebp
        mov    ebp, esp
        sub    esp, 300h
        push   esi
        lea    eax, [ebp - 1]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        lea    ecx, [esi + 2ch]
        push   ecx
        call   sub_1_50C8
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 38h]
        push   eax
        push   ecx
        call   sub_1_50C8
        mov    dword ptr [ebp - 300h], 0eh
        lea    ecx, [ebp - 300h]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_1_2820
        mov    ecx, dword ptr [esi + 68h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_17A9:
        lea    ecx, [esi + 50h]
        push   ecx
        call   sub_1_50D4
        mov    ecx, dword ptr [esi + 4ch]
        push   ecx
        call   sub_1_507E
        mov    ecx, dword ptr [esi + 4ch]
        push   ecx
        call   sub_1_5018
        push   0
        push   707h
        push   esi
        call   sub_1_50EC
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_17E0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 8]
        cmp    dword ptr [ecx + 14h], 0
        je     SHORT loc_1_1803
        pop    ebp
        inc    dword ptr [ecx + 703h]
        mov    byte ptr [eax + 20h], 1
        mov    eax, 103h
        ret    4
loc_1_1803:
        push   eax
        call   sub_1_1810
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_1810:
        push   ebp
        mov    ebp, esp
        sub    esp, 300h
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        mov    eax, dword ptr [edi + 88h]
        cmp    eax, esi
        jne    SHORT loc_1_1836
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [edi + 88h], eax
        jmp    SHORT loc_1_1849
loc_1_1836:
        cmp    dword ptr [eax + 0ch], esi
        je     SHORT loc_1_1843
loc_1_183B:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], esi
        jne    SHORT loc_1_183B
loc_1_1843:
        mov    ecx, dword ptr [esi + 0ch]
        mov    dword ptr [eax + 0ch], ecx
loc_1_1849:
        push   0
        mov    eax, dword ptr [esi + 10h]
        push   eax
        mov    ecx, dword ptr [edi + 68h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_1858:
        cmp    dword ptr [edi + 88h], 0
        jne    SHORT loc_1_18D0
        mov    dword ptr [ebp - 300h], 0bh
        mov    dword ptr [ebp - 2fch], 2
        lea    eax, [ebp - 300h]
        mov    ecx, dword ptr [edi]
        push   eax
        push   ecx
        call   sub_1_2820
        mov    dword ptr [ebp - 2fch], 1
        lea    ecx, [ebp - 300h]
        mov    edx, dword ptr [edi]
        push   ecx
        push   edx
        call   sub_1_2820
        mov    dword ptr [ebp - 2fch], 0
        lea    ecx, [ebp - 300h]
        mov    edx, dword ptr [edi]
        push   ecx
        push   edx
        call   sub_1_2820
        lea    ecx, [ebp - 1]
        lea    edx, [edi + 2ch]
        push   ecx
        push   edx
        call   sub_1_50C8
        lea    ecx, [ebp - 1]
        lea    edx, [edi + 38h]
        push   ecx
        push   edx
        call   sub_1_50C8
loc_1_18D0:
        push   0
        push   21h
        push   esi
        call   sub_1_50EC
        xor    eax, eax
        dec    dword ptr [edi + 703h]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_18F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    ecx, [ebp - 4]
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        push   eax
        push   ecx
        call   sub_1_500C
        push   0
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_1_5000
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_1920:
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    ebp, esp
        sub    esp, 300h
        mov    ecx, dword ptr [eax + 8]
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    dword ptr [ebp - 4], ecx
        mov    dword ptr [ebp - 300h], 28h
        cmp    dword ptr [ebp + 8], ebx
        je     SHORT loc_1_1980
        xor    edi, edi
        mov    esi, dword ptr [ebp + 0ch]
loc_1_194C:
        xor    ecx, ecx
loc_1_194E:
        lea    eax, [esi + ecx]
        inc    ecx
        cmp    ecx, 6
        mov    al, byte ptr [eax + edi]
        mov    byte ptr [ebp + ecx - 2fdh], al
        jb     SHORT loc_1_194E
        lea    eax, [ebp - 300h]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        mov    edx, dword ptr [ecx]
        push   edx
        call   sub_1_2820
        test   eax, eax
        jne    SHORT loc_1_19D5
        add    edi, 6
        inc    ebx
        cmp    ebx, dword ptr [ebp + 8]
        jb     SHORT loc_1_194C
loc_1_1980:
        xor    esi, esi
        xor    ebx, ebx
        mov    dword ptr [ebp - 300h], 27h
        cmp    dword ptr [ebp + 10h], esi
        je     SHORT loc_1_19CA
        mov    edi, dword ptr [ebp + 14h]
loc_1_1996:
        xor    ecx, ecx
loc_1_1998:
        lea    eax, [edi + ecx]
        inc    ecx
        cmp    ecx, 6
        mov    al, byte ptr [eax + esi]
        mov    byte ptr [ebp + ecx - 2fdh], al
        jb     SHORT loc_1_1998
        lea    eax, [ebp - 300h]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        mov    edx, dword ptr [ecx]
        push   edx
        call   sub_1_2820
        test   eax, eax
        jne    SHORT loc_1_19E3
        add    esi, 6
        inc    ebx
        cmp    ebx, dword ptr [ebp + 10h]
        jb     SHORT loc_1_1996
loc_1_19CA:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_19D5:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_1_19E3:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1A00:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1A10:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 2fch
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, edx
        and    eax, 20h
        je     SHORT loc_1_1AA4
        test   cl, 20h
        jne    SHORT loc_1_1AA4
        mov    byte ptr [esi + 96h], 1
        mov    dword ptr [ebp - 2fch], 1ah
        lea    eax, [ebp - 2fch]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        test   byte ptr [ebp - 2f8h], 4
        je     SHORT loc_1_1A65
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_1A65:
        mov    eax, dword ptr [ebp - 2f8h]
        lea    ecx, [ebp - 2fch]
        or     eax, 4
        push   ecx
        mov    dword ptr [ebp - 2fch], 17h
        mov    dword ptr [ebp - 2f8h], eax
        mov    edx, dword ptr [esi]
        push   edx
        call   sub_1_2820
        pop    esi
        cmp    eax, 1
        mov    eax, 0
        mov    esp, ebp
        adc    eax, -1
        pop    ebp
        and    eax, 0c0000001h
        ret    14h
loc_1_1AA4:
        test   eax, eax
        jne    SHORT loc_1_1B1E
        test   cl, 20h
        je     SHORT loc_1_1B1E
        mov    byte ptr [esi + 96h], 0
        mov    dword ptr [ebp - 2fch], 1ah
        lea    eax, [ebp - 2fch]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        test   byte ptr [ebp - 2f8h], 4
        jne    SHORT loc_1_1ADF
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_1ADF:
        mov    eax, dword ptr [ebp - 2f8h]
        lea    ecx, [ebp - 2fch]
        and    eax, 0fffffffbh
        push   ecx
        mov    dword ptr [ebp - 2fch], 17h
        mov    dword ptr [ebp - 2f8h], eax
        mov    edx, dword ptr [esi]
        push   edx
        call   sub_1_2820
        pop    esi
        cmp    eax, 1
        mov    eax, 0
        mov    esp, ebp
        adc    eax, -1
        pop    ebp
        and    eax, 0c0000001h
        ret    14h
loc_1_1B1E:
        mov    eax, edx
        and    eax, 2
        je     SHORT loc_1_1B94
        test   cl, 2
        jne    SHORT loc_1_1B94
        mov    dword ptr [ebp - 2fch], 1ah
        lea    eax, [ebp - 2fch]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        test   byte ptr [ebp - 2f8h], 10h
        je     SHORT loc_1_1B55
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_1B55:
        mov    eax, dword ptr [ebp - 2f8h]
        lea    ecx, [ebp - 2fch]
        or     eax, 10h
        push   ecx
        mov    dword ptr [ebp - 2fch], 17h
        mov    dword ptr [ebp - 2f8h], eax
        mov    edx, dword ptr [esi]
        push   edx
        call   sub_1_2820
        pop    esi
        cmp    eax, 1
        mov    eax, 0
        mov    esp, ebp
        adc    eax, -1
        pop    ebp
        and    eax, 0c0000001h
        ret    14h
loc_1_1B94:
        test   eax, eax
        jne    SHORT loc_1_1C07
        test   cl, 2
        je     SHORT loc_1_1C07
        mov    dword ptr [ebp - 2fch], 1ah
        lea    eax, [ebp - 2fch]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        test   byte ptr [ebp - 2f8h], 10h
        jne    SHORT loc_1_1BC8
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_1BC8:
        mov    eax, dword ptr [ebp - 2f8h]
        lea    ecx, [ebp - 2fch]
        and    eax, 0ffffffefh
        push   ecx
        mov    dword ptr [ebp - 2fch], 17h
        mov    dword ptr [ebp - 2f8h], eax
        mov    edx, dword ptr [esi]
        push   edx
        call   sub_1_2820
        pop    esi
        cmp    eax, 1
        mov    eax, 0
        mov    esp, ebp
        adc    eax, -1
        pop    ebp
        and    eax, 0c0000001h
        ret    14h
loc_1_1C07:
        and    edx, 8
        je     SHORT loc_1_1C7B
        test   cl, 8
        jne    SHORT loc_1_1C7B
        mov    dword ptr [ebp - 2fch], 1ah
        lea    eax, [ebp - 2fch]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        test   byte ptr [ebp - 2f8h], 8
        je     SHORT loc_1_1C3C
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_1C3C:
        mov    eax, dword ptr [ebp - 2f8h]
        lea    ecx, [ebp - 2fch]
        or     eax, 8
        push   ecx
        mov    dword ptr [ebp - 2fch], 17h
        mov    dword ptr [ebp - 2f8h], eax
        mov    edx, dword ptr [esi]
        push   edx
        call   sub_1_2820
        pop    esi
        cmp    eax, 1
        mov    eax, 0
        mov    esp, ebp
        adc    eax, -1
        pop    ebp
        and    eax, 0c0000001h
        ret    14h
loc_1_1C7B:
        test   edx, edx
        jne    SHORT loc_1_1CEE
        test   cl, 8
        je     SHORT loc_1_1CEE
        mov    dword ptr [ebp - 2fch], 1ah
        lea    eax, [ebp - 2fch]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_1_2820
        test   byte ptr [ebp - 2f8h], 8
        jne    SHORT loc_1_1CAF
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_1CAF:
        mov    eax, dword ptr [ebp - 2f8h]
        lea    ecx, [ebp - 2fch]
        and    eax, 0fffffff7h
        push   ecx
        mov    dword ptr [ebp - 2fch], 17h
        mov    dword ptr [ebp - 2f8h], eax
        mov    edx, dword ptr [esi]
        push   edx
        call   sub_1_2820
        pop    esi
        cmp    eax, 1
        mov    eax, 0
        mov    esp, ebp
        adc    eax, -1
        pop    ebp
        and    eax, 0c0000001h
        ret    14h
loc_1_1CEE:
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1D00:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        xor    bl, bl
        mov    esi, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi + 88h], 0
        je     NEAR PTR loc_1_1F1B
        cmp    dword ptr [esi + 14h], 0
        je     SHORT loc_1_1D7B
loc_1_1D21:
        test   bl, bl
        jne    SHORT loc_1_1D7B
        mov    ecx, dword ptr [esi + 14h]
        mov    eax, dword ptr [ecx + 28h]
        lea    edi, [ecx + 24h]
        cmp    byte ptr [eax + 20h], 0
        je     SHORT loc_1_1D47
        mov    eax, dword ptr [edi]
        push   0c0000001h
        push   ecx
        mov    dword ptr [esi + 14h], eax
        mov    edx, dword ptr [edi + 4]
        mov    ecx, dword ptr [edx]
        push   ecx
        jmp    SHORT loc_1_1D6D
loc_1_1D47:
        push   ecx
        push   esi
        call   sub_1_1F30
        test   eax, eax
        je     SHORT loc_1_1D56
        mov    bl, 1
        jmp    SHORT loc_1_1D75
loc_1_1D56:
        push   0
        inc    dword ptr [esi + 0a7h]
        mov    eax, dword ptr [esi + 14h]
        mov    ecx, dword ptr [edi]
        push   eax
        mov    dword ptr [esi + 14h], ecx
        mov    edx, dword ptr [edi + 4]
        mov    eax, dword ptr [edx]
        push   eax
loc_1_1D6D:
        call   sub_1_502A
        dec    dword ptr [esi + 1ch]
loc_1_1D75:
        cmp    dword ptr [esi + 14h], 0
        jne    SHORT loc_1_1D21
loc_1_1D7B:
        mov    byte ptr [ebp - 1], 0
        cmp    dword ptr [esi + 20h], 0
        je     NEAR PTR loc_1_1EA1
loc_1_1D89:
        cmp    byte ptr [ebp - 1], 0
        jne    NEAR PTR loc_1_1EA1
        mov    ecx, dword ptr [esi + 20h]
        mov    eax, dword ptr [ecx + 28h]
        lea    edi, [ecx + 24h]
        cmp    byte ptr [eax + 20h], 0
        je     SHORT loc_1_1DB8
        mov    eax, dword ptr [edi]
        push   0c0000001h
        push   ecx
        mov    dword ptr [esi + 20h], eax
        mov    edx, dword ptr [edi + 4]
        mov    ecx, dword ptr [edx]
        push   ecx
        jmp    NEAR PTR loc_1_1E8F
loc_1_1DB8:
        cmp    byte ptr [edi + 9], 0
        jne    SHORT loc_1_1DD8
        push   ecx
        push   esi
        call   sub_1_1F30
        test   eax, eax
        je     SHORT loc_1_1DD2
        mov    byte ptr [ebp - 1], 1
        jmp    NEAR PTR loc_1_1E97
loc_1_1DD2:
        inc    dword ptr [esi + 0a7h]
loc_1_1DD8:
        cmp    byte ptr [edi + 8], 0
        je     NEAR PTR loc_1_1E7E
        xor    ebx, ebx
        lea    eax, [ebp - 8]
        push   ebx
        mov    ecx, dword ptr [esi + 20h]
        push   eax
        push   ebx
        push   ebx
        push   ecx
        call   sub_1_5110
        cmp    dword ptr [ebp - 8], ebx
        je     SHORT loc_1_1E46
loc_1_1DF9:
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 10h]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_5134
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_1E1A
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        jmp    SHORT loc_1_1E3B
loc_1_1E1A:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        push   eax
        push   ecx
        lea    eax, [ebx + esi]
        add    eax, 0c3h
        push   eax
        call   sub_1_51CC
        add    ebx, dword ptr [ebp - 0ch]
        lea    ecx, [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 8]
        push   eax
loc_1_1E3B:
        call   sub_1_5158
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_1DF9
loc_1_1E46:
        lea    eax, [ebx - 0eh]
        lea    ecx, [esi + 0d1h]
        push   eax
        mov    dword ptr [esi + 0bfh], ebx
        sub    ebx, 0eh
        lea    eax, [esi + 0c3h]
        push   ebx
        inc    dword ptr [esi + 0abh]
        push   ecx
        push   0eh
        push   eax
        mov    ecx, dword ptr [esi + 68h]
        push   eax
        push   esi
        push   ecx
        call   sub_1_4C90
        mov    ecx, dword ptr [esi + 68h]
        push   ecx
        call   sub_1_4E70
loc_1_1E7E:
        mov    eax, dword ptr [esi + 20h]
        mov    ecx, dword ptr [edi]
        push   0
        mov    dword ptr [esi + 20h], ecx
        push   eax
        mov    edx, dword ptr [edi + 4]
        mov    eax, dword ptr [edx]
        push   eax
loc_1_1E8F:
        call   sub_1_502A
        dec    dword ptr [esi + 28h]
loc_1_1E97:
        cmp    dword ptr [esi + 20h], 0
        jne    NEAR PTR loc_1_1D89
loc_1_1EA1:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_2450
        cmp    dword ptr [esi + 20h], 0
        jne    SHORT loc_1_1EF9
        cmp    dword ptr [esi + 14h], 0
        jne    SHORT loc_1_1EF9
        cmp    eax, 13h
        je     SHORT loc_1_1EF9
        cmp    dword ptr [esi + 703h], 0
        jbe    SHORT loc_1_1F04
        mov    edi, dword ptr [esi + 88h]
        test   edi, edi
        je     SHORT loc_1_1F04
loc_1_1ECD:
        cmp    byte ptr [edi + 20h], 0
        je     SHORT loc_1_1EF0
        push   edi
        mov    ebx, dword ptr [edi]
        mov    eax, edi
        mov    edi, dword ptr [edi + 0ch]
        call   sub_1_1810
        push   0
        push   ebx
        call   sub_1_5024
        dec    dword ptr [esi + 703h]
        jmp    SHORT loc_1_1EF3
loc_1_1EF0:
        mov    edi, dword ptr [edi + 0ch]
loc_1_1EF3:
        test   edi, edi
        jne    SHORT loc_1_1ECD
        jmp    SHORT loc_1_1F04
loc_1_1EF9:
        push   1
        lea    eax, [esi + 2ch]
        push   eax
        call   sub_1_50C2
loc_1_1F04:
        cmp    dword ptr [esi + 88h], 0
        je     SHORT loc_1_1F1B
        push   0bb8h
        add    esi, 38h
        push   esi
        call   sub_1_50C2
loc_1_1F1B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1F30:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 0ch
        db  038h, 00Dh, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_1_1F8D
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 006h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 4
        db  0A3h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 10h], 1
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 14h], ecx
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 18h
        db  088h, 00Dh, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
loc_1_1F8D:
        xor    edi, edi
        xor    esi, esi
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp + 0ch]
        push   esi
        xor    ebx, ebx
        push   esi
        push   ecx
        call   sub_1_5110
loc_1_1FA3:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_2021
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_5134
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_1FCF
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_5158
        jmp    SHORT loc_1_1FA3
loc_1_1FCF:
        mov    eax, dword ptr [ebp - 0ch]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + ebx + 0ch], eax
        add    ebx, 0ch
        inc    edi
        mov    edx, dword ptr [ebp - 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 4]
        mov    dword ptr [ecx + ebx - 4], edx
        add    esi, dword ptr [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        call   sub_1_5158
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_2021
        cmp    ebx, 2d0h
        jl     SHORT loc_1_1FA3
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        call   sub_1_2050
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2021:
        db  0A1h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [eax], edi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 0ch], esi
        mov    eax, dword ptr [edx]
        push   eax
        call   sub_1_32D0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2050:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        db  038h, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_1_20A5
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 006h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 4
        db  0A3h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax], ebx
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 10h], 1
        db  0A1h, 000h, 000h, 000h, 000h
        db  088h, 01Dh, 000h, 000h, 000h, 000h
        add    eax, 18h
        db  0A3h, 000h, 000h, 000h, 000h
loc_1_20A5:
        xor    esi, esi
        lea    eax, [ebp - 4]
        push   esi
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        mov    dword ptr [ebp - 10h], esi
        push   esi
        push   esi
        push   ecx
        call   sub_1_5110
        cmp    dword ptr [ebp - 4], esi
        je     SHORT loc_1_2125
        xor    edi, edi
loc_1_20C1:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_5134
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_2112
        cmp    edi, 2d0h
        jge    SHORT loc_1_20FC
        mov    eax, dword ptr [ebp - 0ch]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + edi + 0ch], eax
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + edi + 8], edx
        jmp    SHORT loc_1_2108
loc_1_20FC:
        jne    SHORT loc_1_2108
        mov    ebx, dword ptr [ebp - 4]
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax], esi
loc_1_2108:
        mov    ecx, dword ptr [ebp - 8]
        add    edi, 0ch
        inc    esi
        add    dword ptr [ebp - 10h], ecx
loc_1_2112:
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_5158
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_20C1
loc_1_2125:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        cmp    esi, 3ch
        mov    dword ptr [eax + 0ch], ecx
        db  0A1h, 000h, 000h, 000h, 000h
        jg     SHORT loc_1_2143
        mov    dword ptr [eax + 14h], 0
        jmp    SHORT loc_1_214A
loc_1_2143:
        mov    dword ptr [eax + 14h], 1
loc_1_214A:
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_1_32D0
        test   eax, eax
        je     SHORT loc_1_216C
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_216C:
        test   ebx, ebx
        jne    SHORT loc_1_217B
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_217B:
        mov    dword ptr [ebp - 4], ebx
        test   ebx, ebx
        je     NEAR PTR loc_1_2211
        xor    edi, edi
loc_1_2188:
        xor    esi, esi
        cmp    dword ptr [ebp - 4], edi
        je     SHORT loc_1_2208
        xor    ebx, ebx
loc_1_2191:
        cmp    ebx, 2d0h
        jge    SHORT loc_1_21DF
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_5134
        cmp    dword ptr [ebp - 8], edi
        je     SHORT loc_1_21CD
        mov    eax, dword ptr [ebp - 0ch]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + ebx + 0ch], eax
        mov    edx, dword ptr [ebp - 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    ebx, 0ch
        inc    esi
        mov    dword ptr [ecx + ebx - 4], edx
loc_1_21CD:
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_5158
        cmp    dword ptr [ebp - 4], edi
        jne    SHORT loc_1_2191
loc_1_21DF:
        cmp    dword ptr [ebp - 4], edi
        je     SHORT loc_1_2211
        db  0A1h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 14h], 2
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], esi
        mov    eax, dword ptr [edx]
        push   eax
        call   sub_1_32D0
loc_1_2208:
        cmp    dword ptr [ebp - 4], edi
        jne    NEAR PTR loc_1_2188
loc_1_2211:
        db  0A1h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 14h], 3
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], esi
        mov    eax, dword ptr [edx]
        push   eax
        call   sub_1_32D0
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_1_2240:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   esi, esi
        jle    SHORT loc_1_2255
loc_1_224B:
        push   61h
        call   0
        dec    esi
        jne    SHORT loc_1_224B
loc_1_2255:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2260:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 002h
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        jl     SHORT loc_1_2278
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_2278:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    edi, 1
        lea    eax, [eax + eax*4]
        shl    eax, 6
        xor    edx, edx
        db  005h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 134h], ecx
        mov    dword ptr [eax + 4], edi
        mov    ecx, 100h
        mov    dword ptr [eax + 44h], 1fh
        mov    dword ptr [eax + 48h], 5
        mov    dword ptr [eax + 4ch], edx
        mov    dword ptr [eax + 50h], edi
        mov    dword ptr [eax + 54h], 1c00h
        mov    dword ptr [eax + 58h], edx
        mov    dword ptr [eax + 5ch], ecx
        mov    dword ptr [eax + 60h], ecx
        mov    ecx, 5dch
        mov    esi, 3ch
        mov    dword ptr [eax + 64h], ecx
        mov    dword ptr [eax + 68h], 3dfh
        mov    dword ptr [eax + 6ch], 19h
        mov    dword ptr [eax + 70h], edx
        mov    dword ptr [eax + 74h], 6000h
        mov    dword ptr [eax + 78h], esi
        mov    dword ptr [eax + 7ch], esi
        mov    esi, 9
        mov    dword ptr [eax + 80h], ecx
        mov    dword ptr [eax + 84h], edx
        mov    dword ptr [eax + 88h], esi
        mov    dword ptr [eax + 8ch], edi
        mov    dword ptr [eax + 90h], edx
        mov    dword ptr [eax + 94h], edx
        mov    dword ptr [eax + 98h], ecx
        mov    ecx, 0ffffffffh
        mov    dword ptr [eax + 9ch], edx
        mov    dword ptr [eax + 0a4h], ecx
        mov    dword ptr [eax + 0a8h], ecx
        mov    dword ptr [eax + 0a0h], edx
        mov    dword ptr [eax + 0b8h], esi
        mov    dword ptr [eax + 0bch], edx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    esi, ecx
        lea    ebx, [ecx + ecx*4]
        lea    ecx, [esi + ebx*2]
        lea    ecx, [esi + ecx*8]
        db  081h, 0C1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax + 120h], ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        lea    ebx, [ecx + ecx*4]
        mov    esi, ecx
        mov    edi, 0eh
        lea    ecx, [esi + ebx*2]
        lea    ecx, [esi + ecx*8]
        mov    esi, 0ch
        db  0C6h, 081h, 020h, 000h, 000h, 000h, 050h
        db  089h, 0B9h, 029h, 000h, 000h, 000h
        db  0C6h, 081h, 021h, 000h, 000h, 000h, 051h
        db  089h, 0B1h, 02Dh, 000h, 000h, 000h
        db  0C6h, 081h, 022h, 000h, 000h, 000h, 052h
        db  0C7h, 081h, 031h, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  0C6h, 081h, 023h, 000h, 000h, 000h, 053h
        db  0C7h, 081h, 035h, 000h, 000h, 000h, 008h, 000h, 000h, 000h
        db  0C6h, 081h, 024h, 000h, 000h, 000h, 053h
        db  089h, 0B9h, 039h, 000h, 000h, 000h
        db  0C6h, 081h, 025h, 000h, 000h, 000h, 054h
        db  089h, 0B1h, 03Dh, 000h, 000h, 000h
        db  0C6h, 081h, 026h, 000h, 000h, 000h, 055h
        db  0C7h, 081h, 041h, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h
        db  0C6h, 081h, 027h, 000h, 000h, 000h, 056h
        db  0C7h, 081h, 045h, 000h, 000h, 000h, 008h, 000h, 000h, 000h
        db  0C6h, 081h, 028h, 000h, 000h, 000h, 056h
        db  089h, 0B9h, 049h, 000h, 000h, 000h
        db  089h, 091h, 004h, 000h, 000h, 000h
        db  089h, 091h, 04Dh, 000h, 000h, 000h
        db  089h, 091h, 051h, 000h, 000h, 000h
        db  089h, 091h, 055h, 000h, 000h, 000h
        pop    edi
        pop    esi
        db  089h, 091h, 00Ch, 000h, 000h, 000h
        pop    ebx
        db  089h, 091h, 010h, 000h, 000h, 000h
        pop    ebp
        db  089h, 091h, 014h, 000h, 000h, 000h
        db  089h, 091h, 018h, 000h, 000h, 000h
        db  089h, 091h, 01Ch, 000h, 000h, 000h
        db  089h, 091h, 008h, 000h, 000h, 000h
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2450:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0ch
        mov    dword ptr [ebp - 0ch], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 128h]
        lea    ecx, [esi + 1]
        mov    dword ptr [ebp - 8], ecx
        cmp    eax, 7
        je     SHORT loc_1_248B
        cmp    eax, 6
        je     SHORT loc_1_248B
        mov    eax, 0ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_248B:
        mov    edx, esi
        xor    al, al
        out    dx, al
        lea    eax, [esi + 6]
        push   eax
        call   0
        and    al, 0f7h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 4], al
        mov    edi, dword ptr [ecx + 120h]
        je     SHORT loc_1_250B
        test   al, 2
        je     SHORT loc_1_24E8
        mov    edx, dword ptr [ebp - 8]
        xor    al, al
        out    dx, al
        lea    eax, [esi + 0dh]
        push   eax
        call   0
        xor    edx, edx
        mov    ecx, dword ptr [edi + 4]
        mov    dl, al
        mov    eax, edx
        cmp    edx, ecx
        jle    SHORT loc_1_24CD
        sub    eax, ecx
        jmp    SHORT loc_1_24D4
loc_1_24CD:
        sub    eax, ecx
        add    eax, 100h
loc_1_24D4:
        add    dword ptr [edi + 8], eax
        mov    dword ptr [edi + 4], edx
        mov    dword ptr [edi + 4dh], 0
        mov    dword ptr [edi + 51h], 0
loc_1_24E8:
        test   byte ptr [ebp - 4], 20h
        je     SHORT loc_1_24F5
        mov    dword ptr [edi + 0ch], 1
loc_1_24F5:
        test   byte ptr [ebp - 4], 40h
        je     SHORT loc_1_250B
        mov    edx, dword ptr [ebp - 8]
        xor    al, al
        out    dx, al
        lea    edx, [esi + 0ch]
        mov    eax, 2000h
        out    dx, ax
loc_1_250B:
        mov    edx, dword ptr [ebp - 8]
        mov    al, 40h
        push   5
        lea    ebx, [esi + 0bh]
        out    dx, al
        call   sub_1_2240
        push   ebx
        call   0
        test   al, al
        je     NEAR PTR loc_1_25C3
        test   al, 8
        je     SHORT loc_1_259A
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 60h]
        add    ecx, 20h
        mov    dword ptr [eax + 60h], ecx
        cmp    dword ptr [eax + 64h], ecx
        ja     SHORT loc_1_2542
        and    dword ptr [eax + 48h], 0fffffffdh
loc_1_2542:
        cmp    dword ptr [edi], 0
        jne    SHORT loc_1_2560
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4930
        mov    edx, esi
        mov    al, 4
        out    dx, al
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2560:
        mov    edx, dword ptr [ebp - 8]
        mov    al, 3
        out    dx, al
        lea    edx, [esi + 0ah]
        mov    eax, ecx
        and    eax, 0ffffh
        out    dx, ax
        mov    edx, esi
        mov    al, 44h
loc_1_2576:
        out    dx, al
loc_1_2577:
        cmp    dword ptr [edi + 51h], 1eh
        jle    NEAR PTR loc_1_27B1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4930
        mov    edx, esi
        mov    al, 4
        out    dx, al
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_259A:
        test   al, 7fh
        je     SHORT loc_1_25BE
        mov    edx, esi
        mov    al, 50h
        out    dx, al
        cmp    dword ptr [edi + 18h], 0
        jne    SHORT loc_1_25BE
        mov    eax, dword ptr [edi + 1ch]
        inc    eax
        mov    dword ptr [edi + 1ch], eax
        cmp    eax, 3e8h
        jle    SHORT loc_1_25BE
        mov    dword ptr [edi + 18h], 1
loc_1_25BE:
        mov    edx, ebx
        xor    al, al
        out    dx, al
loc_1_25C3:
        lea    eax, [esi + 0ch]
        push   eax
        call   0
        test   al, 30h
        mov    cl, al
        je     SHORT loc_1_25E4
        lea    eax, [esi + 0ch]
        mov    edx, eax
        xor    al, al
        out    dx, al
        test   cl, 10h
        je     SHORT loc_1_25E4
        mov    edx, esi
        mov    al, 50h
        out    dx, al
loc_1_25E4:
        lea    ebx, [esi + 9]
        push   ebx
        call   0
        test   al, 0cfh
        mov    cl, al
        je     SHORT loc_1_2617
        mov    edx, ebx
        xor    al, al
        out    dx, al
        test   cl, 10h
        je     SHORT loc_1_260B
        mov    dword ptr [edi + 10h], 1
        mov    dword ptr [edi + 14h], 1
loc_1_260B:
        test   cl, 40h
        je     SHORT loc_1_2617
        mov    dword ptr [edi + 14h], 1
loc_1_2617:
        test   cl, 20h
        jne    SHORT loc_1_2623
        mov    dword ptr [edi + 14h], 1
loc_1_2623:
        mov    edx, dword ptr [ebp - 8]
        xor    al, al
        out    dx, al
        push   esi
        call   0
        cmp    dword ptr [edi + 0ch], 0
        jne    SHORT loc_1_2640
        test   al, 1
        je     SHORT loc_1_2640
        mov    dword ptr [edi + 0ch], 1
loc_1_2640:
        test   al, 4
        je     SHORT loc_1_264E
        lea    edx, [esi + 0ch]
        mov    eax, 2000h
        out    dx, ax
loc_1_264E:
        mov    ecx, dword ptr [edi + 0ch]
        test   ecx, ecx
        jne    SHORT loc_1_2699
        cmp    dword ptr [edi + 8], 0
        jne    SHORT loc_1_2699
        cmp    dword ptr [edi + 10h], 0
        jne    SHORT loc_1_2699
        cmp    dword ptr [edi + 14h], 0
        jne    SHORT loc_1_2699
        cmp    dword ptr [edi + 4dh], 0
        je     SHORT loc_1_2686
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4930
        mov    edx, esi
        mov    al, 4
        out    dx, al
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2686:
        mov    edx, esi
        mov    al, 4
        out    dx, al
        mov    eax, 0ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2699:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0a8h], 0
        mov    eax, 0
        jge    SHORT loc_1_26AF
        mov    eax, 1
loc_1_26AF:
        mov    dword ptr [ebp - 4], eax
        cmp    ecx, 1
        jne    NEAR PTR loc_1_2577
loc_1_26BB:
        cmp    dword ptr [ebp - 4], 1
        je     SHORT loc_1_26D1
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0a8h], 0
        jle    NEAR PTR loc_1_2577
loc_1_26D1:
        mov    edx, dword ptr [ebp - 8]
        xor    al, al
        out    dx, al
        cmp    dword ptr [edi + 4dh], 0
        je     SHORT loc_1_26E0
        inc    dword ptr [edi + 51h]
loc_1_26E0:
        inc    dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 0ch], 64h
        je     NEAR PTR loc_1_278F
        lea    edx, [esi + 0ch]
        mov    eax, 2000h
        out    dx, ax
        lea    ecx, [esi + 0eh]
        push   ecx
        call   sub_1_0010
        mov    ebx, eax
        and    ebx, 0ffffh
        test   ebx, ebx
        jne    SHORT loc_1_2729
        lea    edx, [esi + 0ch]
        mov    eax, 2000h
        out    dx, ax
        push   esi
        call   0
        test   al, 1
        jne    SHORT loc_1_2780
        mov    dword ptr [edi + 0ch], 0
        jmp    SHORT loc_1_2780
loc_1_2729:
        lea    eax, [esi + 0ch]
        push   eax
        call   0
        test   al, 80h
        je     SHORT loc_1_2798
        sub    ebx, 4
        cmp    ebx, 5eah
        jbe    SHORT loc_1_276C
        lea    edx, [esi + 0ch]
        mov    eax, 2000h
        out    dx, ax
        push   esi
        call   0
        test   al, 1
        jne    SHORT loc_1_275C
        mov    dword ptr [edi + 0ch], 0
loc_1_275C:
        test   al, 4
        je     SHORT loc_1_2780
        lea    edx, [esi + 0ch]
        mov    eax, 2000h
        out    dx, ax
        jmp    SHORT loc_1_2780
loc_1_276C:
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   1
        mov    ecx, dword ptr [eax + 134h]
        push   ecx
        call   sub_1_0BB0
loc_1_2780:
        cmp    dword ptr [edi + 0ch], 1
        je     NEAR PTR loc_1_26BB
        jmp    NEAR PTR loc_1_2577
loc_1_278F:
        mov    edx, esi
        mov    al, 4
        jmp    NEAR PTR loc_1_2576
loc_1_2798:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4930
        mov    edx, esi
        mov    al, 4
        out    dx, al
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_27B1:
        cmp    dword ptr [edi + 10h], 1
        jne    SHORT loc_1_27C3
        mov    edx, esi
        mov    al, 20h
        out    dx, al
        mov    dword ptr [edi + 10h], 0
loc_1_27C3:
        cmp    dword ptr [edi + 14h], 1
        jne    SHORT loc_1_27F9
        mov    edx, dword ptr [ebp - 8]
        mov    al, 40h
        out    dx, al
        push   5
        call   sub_1_2240
        lea    edx, [esi + 8]
        mov    al, 80h
        out    dx, al
        mov    al, 64h
        out    dx, al
        lea    eax, [esi + 9]
        push   eax
        call   0
        test   al, 20h
        je     SHORT loc_1_27F9
        mov    edx, dword ptr [ebp - 8]
        xor    al, al
        out    dx, al
        mov    dword ptr [edi + 14h], 0
loc_1_27F9:
        mov    edx, esi
        mov    al, 4
        out    dx, al
        cmp    dword ptr [edi + 0ch], 1
        mov    eax, 0
        pop    edi
        adc    eax, -1
        pop    esi
        and    eax, 13h
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2820:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    ecx, dword ptr [eax]
        cmp    ecx, 2fh
        ja     SHORT loc_1_2835
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_1_2835:
        mov    eax, 3
        pop    ebp
        ret    8
loc_1_283E:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2B90
        pop    ebp
        ret    8
loc_1_284C:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2BC0
        pop    ebp
        ret    8
loc_1_285A:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3310
        pop    ebp
        ret    8
loc_1_2868:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3340
        pop    ebp
        ret    8
loc_1_2876:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4430
        pop    ebp
        ret    8
loc_1_2884:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4440
        pop    ebp
        ret    8
loc_1_2892:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        pop    ebp
        ret    8
loc_1_28A1:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3A50
        pop    ebp
        ret    8
loc_1_28AF:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        pop    ebp
        ret    8
loc_1_28BE:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E10
        pop    ebp
        ret    8
loc_1_28CC:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E20
        pop    ebp
        ret    8
loc_1_28DA:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3F00
        pop    ebp
        ret    8
loc_1_28E8:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E30
        pop    ebp
        ret    8
loc_1_28F6:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3FD0
        pop    ebp
        ret    8
loc_1_2904:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3FE0
        pop    ebp
        ret    8
loc_1_2912:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3700
        pop    ebp
        ret    8
loc_1_2920:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3720
        pop    ebp
        ret    8
loc_1_292E:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4450
        pop    ebp
        ret    8
loc_1_293C:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3730
        pop    ebp
        ret    8
loc_1_294A:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3750
        pop    ebp
        ret    8
loc_1_2958:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3770
        pop    ebp
        ret    8
loc_1_2966:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3AB0
        pop    ebp
        ret    8
loc_1_2974:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3B70
        pop    ebp
        ret    8
loc_1_2982:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4010
        pop    ebp
        ret    8
loc_1_2990:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3AE0
        pop    ebp
        ret    8
loc_1_299E:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3CD0
        pop    ebp
        ret    8
loc_1_29AC:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4080
        pop    ebp
        ret    8
loc_1_29BA:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3B00
        pop    ebp
        ret    8
loc_1_29C8:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3CF0
        pop    ebp
        ret    8
loc_1_29D6:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_40A0
        pop    ebp
        ret    8
loc_1_29E4:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3B50
        pop    ebp
        ret    8
loc_1_29F2:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3D40
        pop    ebp
        ret    8
loc_1_2A00:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_40D0
        pop    ebp
        ret    8
loc_1_2A0E:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4460
        pop    ebp
        ret    8
loc_1_2A1C:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3DE0
        pop    ebp
        ret    8
loc_1_2A2A:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3D60
        pop    ebp
        ret    8
loc_1_2A38:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4780
        pop    ebp
        ret    8
loc_1_2A46:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_47A0
        pop    ebp
        ret    8
loc_1_2A54:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4490
        pop    ebp
        ret    8
loc_1_2A62:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4630
        pop    ebp
        ret    8
loc_1_2A70:
        mov    eax, 0ch
        pop    ebp
        ret    8
loc_1_2A79:
        mov    eax, 0ch
        pop    ebp
        ret    8
loc_1_2A82:
        mov    eax, 0ch
        pop    ebp
        ret    8
loc_1_2A8B:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4910
        pop    ebp
        ret    8
loc_1_2A99:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_40F0
        pop    ebp
        ret    8
loc_1_2AA7:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4810
        pop    ebp
        ret    8
loc_1_2AB5:
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_47F0
        pop    ebp
        ret    8
        db  090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2B90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   3e8h
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 20h]
        mov    byte ptr [eax + 800h], 41h
        call   sub_1_2240
        mov    dword ptr [esi + 128h], 4
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_2BC0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ecx + 0ch]
        cmp    dword ptr [ecx + 128h], 4
        lea    eax, [esi + 1]
        lea    edi, [esi + 4]
        mov    dword ptr [ebp - 14h], eax
        je     SHORT loc_1_2BF0
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2BF0:
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 14h]
        push   1
        mov    eax, dword ptr [ecx + 120h]
        mov    dword ptr [ebp - 1ch], eax
        mov    al, 4
        out    dx, al
        call   sub_1_2240
        lea    edx, [esi + 9]
        xor    al, al
        out    dx, al
        push   0ffffh
        call   sub_1_2240
        push   0ffffh
        call   sub_1_2240
        mov    edx, dword ptr [ebp - 14h]
        mov    al, 4
        out    dx, al
        push   1
        call   sub_1_2240
        lea    edx, [esi + 9]
        mov    al, 5
        out    dx, al
        push   0ffffh
        call   sub_1_2240
        push   0ffffh
        call   sub_1_2240
        mov    edx, esi
        mov    al, 2
        out    dx, al
        push   0ffffh
        call   sub_1_2240
        mov    edx, esi
        xor    al, al
        out    dx, al
        push   0ffffh
        call   sub_1_2240
        mov    edx, dword ptr [ebp - 14h]
        mov    al, 4
        out    dx, al
        lea    edx, [esi + 8]
        push   0ea60h
        out    dx, al
        call   sub_1_2240
        push   esi
        call   0
        lea    ecx, [esi + 0ah]
        push   ecx
        call   0
        and    al, 30h
        xor    edx, edx
        shr    al, 4
        mov    ecx, dword ptr [ebp - 1ch]
        mov    dl, al
        xor    ebx, ebx
        mov    dword ptr [ebp - 4], 67h
        mov    dword ptr [ecx], edx
loc_1_2CA2:
        lea    eax, [esi + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, eax
        add    ecx, ebx
        mov    ax, word ptr [ebp - 4]
        mov    dword ptr [ebp - 8], ecx
        out    dx, ax
        lea    eax, [esi + 0eh]
        push   eax
        call   0
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0ach], al
        test   byte ptr [edx + 0ach], 1
        jne    NEAR PTR loc_1_2EA4
        inc    ebx
        mov    byte ptr [ecx + 0b2h], al
        inc    dword ptr [ebp - 4]
        cmp    ebx, 6
        jl     SHORT loc_1_2CA2
        mov    edx, dword ptr [ebp - 14h]
        mov    al, 40h
        out    dx, al
        push   5
        call   sub_1_2240
        lea    edx, [esi + 0dh]
        mov    al, 80h
        out    dx, al
        lea    eax, [esi + 0eh]
        mov    edx, eax
        mov    al, 80h
        out    dx, al
        lea    edx, [esi + 0fh]
        mov    al, 20h
        out    dx, al
        lea    edx, [esi + 9]
        xor    al, al
        out    dx, al
        lea    edx, [esi + 0bh]
        out    dx, al
        lea    eax, [esi + 0ch]
        mov    edx, eax
        xor    al, al
        out    dx, al
        mov    edx, dword ptr [ebp - 1ch]
        cmp    dword ptr [edx], 0
        jne    SHORT loc_1_2D35
        mov    edx, dword ptr [ebp - 14h]
        mov    al, 2
        out    dx, al
        lea    edx, [esi + 8]
        xor    al, al
        out    dx, al
        lea    eax, [esi + 9]
        mov    edx, eax
        mov    al, 20h
        out    dx, al
loc_1_2D35:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 6ch]
        mov    dword ptr [ebp - 4], eax
        and    eax, 2
        mov    dword ptr [ebp - 8], eax
        je     SHORT loc_1_2D55
        mov    edx, dword ptr [ebp - 14h]
        mov    al, 3
        out    dx, al
        mov    ax, word ptr [ecx + 7ch]
        lea    edx, [esi + 8]
        out    dx, ax
loc_1_2D55:
        mov    ecx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ecx + 48h]
        and    ebx, 2
        je     SHORT loc_1_2D71
        mov    edx, dword ptr [ebp - 14h]
        mov    al, 3
        out    dx, al
        lea    eax, [esi + 0ah]
        mov    edx, eax
        mov    ax, word ptr [ecx + 60h]
        out    dx, ax
loc_1_2D71:
        mov    edx, dword ptr [ebp - 14h]
        mov    al, 1
        out    dx, al
        lea    edx, [esi + 0dh]
        out    dx, al
        xor    al, al
        test   ebx, ebx
        je     SHORT loc_1_2D83
        mov    al, 1
loc_1_2D83:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_2D8B
        or     al, 2
loc_1_2D8B:
        mov    ecx, dword ptr [ebp + 8]
        test   byte ptr [ecx + 8ch], 8
        je     SHORT loc_1_2D99
        or     al, 4
loc_1_2D99:
        lea    edx, [esi + 0eh]
        out    dx, al
        mov    edx, dword ptr [ebp - 14h]
        mov    al, 42h
        out    dx, al
        lea    eax, [esi + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, eax
        mov    al, 20h
        out    dx, al
        cmp    dword ptr [ecx + 0bch], 0
        mov    al, 10h
        jle    SHORT loc_1_2DC1
        test   byte ptr [ebp - 4], 10h
        je     SHORT loc_1_2DC1
        mov    al, 11h
loc_1_2DC1:
        test   byte ptr [ebp - 4], 4
        je     SHORT loc_1_2DC9
        or     al, 4
loc_1_2DC9:
        test   byte ptr [ebp - 4], 8
        jne    SHORT loc_1_2DD1
        or     al, 8
loc_1_2DD1:
        lea    edx, [esi + 9]
        push   esi
        out    dx, al
        xor    ebx, ebx
        call   0
        mov    edx, dword ptr [ebp - 14h]
        mov    al, 50h
        out    dx, al
        push   0c8h
        call   sub_1_2240
        mov    dword ptr [ebp - 4], 8
loc_1_2DF4:
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        add    edx, esi
        inc    ebx
        inc    dword ptr [ebp - 4]
        mov    al, byte ptr [ebx + ecx + 0b1h]
        out    dx, al
        cmp    ebx, 6
        jl     SHORT loc_1_2DF4
        push   esi
        call   0
        mov    dword ptr [ebp - 10h], 50h
        mov    ecx, 0eh
loc_1_2E1F:
        cmp    dword ptr [ebp - 10h], 58h
        jge    SHORT loc_1_2E55
        lea    edx, [ecx + esi]
        xor    al, al
        out    dx, al
        inc    ecx
        cmp    ecx, 10h
        jl     SHORT loc_1_2E4A
        mov    edx, dword ptr [ebp - 14h]
        push   0c8h
        inc    dword ptr [ebp - 10h]
        mov    al, byte ptr [ebp - 10h]
        out    dx, al
        call   sub_1_2240
        mov    ecx, 8
loc_1_2E4A:
        cmp    dword ptr [ebp - 10h], 57h
        jne    SHORT loc_1_2E1F
        cmp    ecx, 0ch
        jne    SHORT loc_1_2E1F
loc_1_2E55:
        mov    edx, dword ptr [ebp - 14h]
        xor    al, al
        out    dx, al
        lea    ecx, [esi + 0dh]
        push   ecx
        call   0
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 1ch]
        push   esi
        mov    dword ptr [ecx + 4], eax
        call   0
        push   esi
        call   0
        and    al, 20h
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 14h]
        cmp    al, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 2
        mov    dword ptr [ecx + 3ch], eax
        cmp    eax, 2
        mov    al, 2
        out    dx, al
        lea    eax, [esi + 0ah]
        mov    edx, eax
        jne    SHORT loc_1_2EBE
        mov    al, 3bh
        jmp    SHORT loc_1_2EC0
loc_1_2EA4:
        mov    edx, dword ptr [ebp - 14h]
        mov    al, 4
        out    dx, al
        lea    edx, [esi + 9]
        xor    al, al
        out    dx, al
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2EBE:
        mov    al, 39h
loc_1_2EC0:
        out    dx, al
        push   64h
        call   sub_1_2240
        mov    edx, dword ptr [ebp - 14h]
        xor    al, al
        out    dx, al
        lea    edx, [esi + 0ch]
        mov    eax, 2000h
        mov    ecx, dword ptr [ebp + 8]
        out    dx, ax
        test   byte ptr [ecx + 8ch], 1
        je     SHORT loc_1_2F04
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 18h], 0
        jmp    NEAR PTR loc_1_30B0
loc_1_2F04:
        mov    edx, dword ptr [ebp - 14h]
        mov    al, 42h
        out    dx, al
        push   5
        call   sub_1_2240
        lea    ecx, [esi + 8]
        push   ecx
        call   0
        lea    ecx, [esi + 9]
        mov    byte ptr [ebp - 9], al
        push   ecx
        call   0
        lea    edx, [esi + 8]
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 9]
        or     al, 2
        out    dx, al
        mov    al, byte ptr [ebp - 8]
        lea    edx, [esi + 9]
        or     al, 6
        out    dx, al
        mov    edx, dword ptr [ebp - 14h]
        mov    al, 1
        out    dx, al
        lea    edx, [esi + 0eh]
        push   edx
        call   0
        lea    edx, [esi + 0eh]
        mov    byte ptr [ebp - 4], al
        and    al, 0fch
        push   5
        out    dx, al
        mov    edx, dword ptr [ebp - 14h]
        mov    al, 40h
        out    dx, al
        call   sub_1_2240
        lea    edx, [esi + 8]
        mov    al, 24h
        out    dx, al
        mov    edx, dword ptr [ebp - 14h]
        xor    al, al
        out    dx, al
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 500h
        mov    ecx, dword ptr [edx + 38h]
        mov    edx, edi
        out    dx, ax
        mov    edx, 5
loc_1_2F80:
        xor    eax, eax
loc_1_2F82:
        mov    byte ptr [ecx], al
        inc    ecx
        inc    eax
        cmp    eax, 100h
        jl     SHORT loc_1_2F82
        dec    edx
        jne    SHORT loc_1_2F80
        mov    edx, esi
        mov    al, 1
        out    dx, al
        mov    dword ptr [ebp - 10h], 0
loc_1_2F9C:
        push   esi
        call   0
        test   al, 1
        jne    SHORT loc_1_2FB2
        inc    dword ptr [ebp - 10h]
        cmp    dword ptr [ebp - 10h], 186a0h
        jl     SHORT loc_1_2F9C
loc_1_2FB2:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ebp - 10h], 186a0h
        mov    ebx, dword ptr [ecx + 38h]
        jne    SHORT loc_1_2FCA
        mov    dword ptr [ebp - 18h], 0
        jmp    SHORT loc_1_2FE7
loc_1_2FCA:
        lea    ecx, [esi + 0eh]
        push   ecx
        call   sub_1_0010
        and    eax, 0ffffh
        sub    eax, 504h
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [ebp - 18h], eax
loc_1_2FE7:
        xor    eax, eax
        cmp    dword ptr [ebp - 18h], eax
        je     SHORT loc_1_301D
loc_1_2FEE:
        cmp    eax, 5
        jge    SHORT loc_1_301D
        xor    edx, edx
loc_1_2FF5:
        mov    ecx, ebx
        inc    ebx
        mov    cl, byte ptr [ecx]
        and    ecx, 0ffh
        cmp    ecx, edx
        jne    SHORT loc_1_300F
        inc    edx
        cmp    edx, 100h
        jl     SHORT loc_1_2FF5
        jmp    SHORT loc_1_3016
loc_1_300F:
        mov    dword ptr [ebp - 18h], 0
loc_1_3016:
        inc    eax
        cmp    dword ptr [ebp - 18h], 0
        jne    SHORT loc_1_2FEE
loc_1_301D:
        cmp    dword ptr [ebp - 18h], 0
        je     SHORT loc_1_3039
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_3054
loc_1_3039:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 18h], 0
loc_1_3054:
        mov    edx, dword ptr [ebp - 14h]
        xor    al, al
        out    dx, al
        lea    edx, [esi + 0ch]
        mov    eax, 8000h
        out    dx, ax
        mov    edx, dword ptr [ebp - 14h]
        mov    al, 40h
        out    dx, al
        lea    edx, [esi + 8]
        mov    al, 88h
        out    dx, al
        mov    edx, dword ptr [ebp - 14h]
        xor    al, al
        out    dx, al
        lea    edx, [esi + 9]
        out    dx, al
        lea    eax, [esi + 0bh]
        mov    edx, eax
        xor    al, al
        out    dx, al
        lea    edx, [esi + 0ch]
        push   5
        out    dx, al
        mov    edx, dword ptr [ebp - 14h]
        mov    al, 42h
        out    dx, al
        call   sub_1_2240
        lea    edx, [esi + 8]
        mov    al, byte ptr [ebp - 9]
        out    dx, al
        lea    edx, [esi + 9]
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        mov    edx, dword ptr [ebp - 14h]
        mov    al, 1
        out    dx, al
        lea    eax, [esi + 0eh]
        mov    edx, eax
        mov    al, byte ptr [ebp - 4]
        out    dx, al
loc_1_30B0:
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_1_32AB
        mov    edx, dword ptr [ebp - 14h]
        mov    al, 42h
        out    dx, al
        push   5
        call   sub_1_2240
        lea    ecx, [esi + 8]
        push   ecx
        call   0
        lea    ecx, [esi + 9]
        mov    byte ptr [ebp - 9], al
        push   ecx
        call   0
        lea    edx, [esi + 8]
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 9]
        or     al, 2
        out    dx, al
        mov    al, byte ptr [ebp - 8]
        lea    edx, [esi + 9]
        or     al, 6
        out    dx, al
        mov    edx, dword ptr [ebp - 14h]
        mov    al, 1
        out    dx, al
        lea    edx, [esi + 0eh]
        push   edx
        call   0
        lea    edx, [esi + 0eh]
        mov    byte ptr [ebp - 4], al
        and    al, 0fch
        push   5
        out    dx, al
        mov    edx, dword ptr [ebp - 14h]
        mov    al, 40h
        out    dx, al
        call   sub_1_2240
        lea    edx, [esi + 8]
        mov    al, 24h
        out    dx, al
        mov    edx, dword ptr [ebp - 14h]
        xor    al, al
        out    dx, al
        mov    edx, edi
        mov    eax, 500h
        out    dx, ax
        mov    ecx, 3
loc_1_3130:
        xor    eax, eax
loc_1_3132:
        mov    edx, edi
        out    dx, al
        inc    eax
        cmp    eax, 100h
        jl     SHORT loc_1_3132
        dec    ecx
        jne    SHORT loc_1_3130
        mov    ecx, 2
loc_1_3145:
        xor    eax, eax
loc_1_3147:
        mov    edx, edi
        out    dx, ax
        inc    eax
        cmp    eax, 80h
        jl     SHORT loc_1_3147
        dec    ecx
        jne    SHORT loc_1_3145
        mov    edx, esi
        mov    al, 1
        out    dx, al
        xor    ebx, ebx
loc_1_315D:
        push   esi
        call   0
        test   al, 1
        jne    SHORT loc_1_3170
        inc    ebx
        cmp    ebx, 186a0h
        jl     SHORT loc_1_315D
loc_1_3170:
        cmp    ebx, 186a0h
        jne    SHORT loc_1_3181
        mov    dword ptr [ebp - 18h], 0
        jmp    SHORT loc_1_319E
loc_1_3181:
        lea    ecx, [esi + 0eh]
        push   ecx
        call   sub_1_0010
        and    eax, 0ffffh
        sub    eax, 504h
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [ebp - 18h], eax
loc_1_319E:
        mov    dword ptr [ebp - 10h], 0
        cmp    dword ptr [ebp - 18h], 0
        je     SHORT loc_1_31DD
loc_1_31AB:
        cmp    dword ptr [ebp - 10h], 3
        jge    SHORT loc_1_31DD
        xor    ebx, ebx
loc_1_31B3:
        push   edi
        call   0
        and    eax, 0ffh
        cmp    eax, ebx
        jne    SHORT loc_1_31CD
        inc    ebx
        cmp    ebx, 100h
        jl     SHORT loc_1_31B3
        jmp    SHORT loc_1_31D4
loc_1_31CD:
        mov    dword ptr [ebp - 18h], 0
loc_1_31D4:
        inc    dword ptr [ebp - 10h]
        cmp    dword ptr [ebp - 18h], 0
        jne    SHORT loc_1_31AB
loc_1_31DD:
        mov    dword ptr [ebp - 10h], 0
        mov    ecx, dword ptr [ebp - 1ch]
        cmp    dword ptr [ebp - 18h], 0
        mov    dword ptr [ecx + 55h], 0
        je     SHORT loc_1_3234
loc_1_31F4:
        cmp    dword ptr [ebp - 10h], 2
        jge    SHORT loc_1_322E
        mov    ecx, dword ptr [ebp - 1ch]
        cmp    dword ptr [ecx + 55h], 0
        jne    SHORT loc_1_322E
        xor    ebx, ebx
loc_1_3205:
        push   edi
        call   sub_1_0010
        cmp    ax, bx
        jne    SHORT loc_1_321B
        inc    ebx
        cmp    ebx, 80h
        jl     SHORT loc_1_3205
        jmp    SHORT loc_1_3225
loc_1_321B:
        mov    ecx, dword ptr [ebp - 1ch]
        mov    dword ptr [ecx + 55h], 1
loc_1_3225:
        inc    dword ptr [ebp - 10h]
        cmp    dword ptr [ebp - 18h], 0
        jne    SHORT loc_1_31F4
loc_1_322E:
        cmp    dword ptr [ebp - 18h], 0
        jne    SHORT loc_1_324E
loc_1_3234:
        mov    edx, dword ptr [ebp - 14h]
        mov    al, 4
        out    dx, al
        lea    edx, [esi + 9]
        xor    al, al
        out    dx, al
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_324E:
        mov    edx, dword ptr [ebp - 14h]
        xor    al, al
        out    dx, al
        lea    edx, [esi + 0ch]
        mov    eax, 8000h
        out    dx, ax
        mov    edx, dword ptr [ebp - 14h]
        mov    al, 40h
        out    dx, al
        lea    edx, [esi + 8]
        mov    al, 88h
        out    dx, al
        mov    edx, dword ptr [ebp - 14h]
        xor    al, al
        out    dx, al
        lea    edx, [esi + 9]
        out    dx, al
        lea    eax, [esi + 0bh]
        mov    edx, eax
        xor    al, al
        out    dx, al
        lea    edx, [esi + 0ch]
        push   5
        out    dx, al
        mov    edx, dword ptr [ebp - 14h]
        mov    al, 42h
        out    dx, al
        call   sub_1_2240
        lea    edx, [esi + 8]
        mov    al, byte ptr [ebp - 9]
        out    dx, al
        lea    edx, [esi + 9]
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        mov    edx, dword ptr [ebp - 14h]
        xor    al, al
        out    dx, al
        mov    al, 1
        out    dx, al
        lea    edx, [esi + 0eh]
        mov    al, byte ptr [ebp - 4]
        out    dx, al
loc_1_32AB:
        mov    ecx, dword ptr [ebp + 8]
        xor    eax, eax
        pop    edi
        pop    esi
        mov    dword ptr [ecx + 128h], 6
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_32D0:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   eax
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_32F0:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   eax
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3310:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [eax + 0ch]
        push   5
        mov    al, 40h
        lea    edx, [esi + 1]
        out    dx, al
        call   sub_1_2240
        lea    edx, [esi + 8]
        mov    al, 80h
        out    dx, al
        mov    al, 4
        pop    esi
        out    dx, al
        mov    al, 20h
        pop    ebp
        out    dx, al
        xor    eax, eax
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_3340:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [eax + 0ch]
        push   5
        mov    al, 40h
        lea    edx, [esi + 1]
        out    dx, al
        call   sub_1_2240
        lea    edx, [esi + 8]
        mov    al, 80h
        out    dx, al
        mov    al, 8
        pop    esi
        out    dx, al
        mov    al, 20h
        pop    ebp
        out    dx, al
        xor    eax, eax
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_3370:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 18h
        mov    eax, dword ptr [ecx + 0ch]
        push   ebx
        push   esi
        mov    ebx, dword ptr [ebp + 0ch]
        push   edi
        lea    esi, [eax + 4]
        lea    edi, [eax + 1]
        add    ebx, 4
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 8], ebx
        test   byte ptr [ecx + 48h], 2
        mov    eax, dword ptr [ecx + 120h]
        mov    dword ptr [ebp - 10h], eax
        je     SHORT loc_1_3414
        cmp    dword ptr [eax + 18h], 0
        je     SHORT loc_1_3414
        mov    edx, edi
        mov    al, 8
        out    dx, al
        mov    ebx, dword ptr [ebp - 14h]
        add    ebx, 0ch
        push   ebx
        call   sub_1_0010
        cmp    ax, 1
        jne    SHORT loc_1_3414
        mov    edx, edi
        mov    al, 40h
        out    dx, al
        push   5
        call   sub_1_2240
        push   ebx
        call   0
        and    al, 0fh
        mov    edx, edi
        push   ebx
        mov    byte ptr [ebp - 4], al
        mov    al, 8
        out    dx, al
        call   sub_1_0010
        cmp    ax, 1
        jne    SHORT loc_1_3414
        mov    edx, edi
        mov    al, 40h
        out    dx, al
        push   5
        call   sub_1_2240
        push   ebx
        call   0
        and    al, 0fh
        cmp    al, byte ptr [ebp - 4]
        jne    SHORT loc_1_3414
        mov    eax, dword ptr [ebp - 14h]
        lea    edx, [eax + 8]
        mov    al, 80h
        out    dx, al
        mov    al, 1
        out    dx, al
        mov    al, 64h
        out    dx, al
        mov    edx, dword ptr [ebp - 14h]
        mov    al, 50h
        out    dx, al
loc_1_3414:
        mov    edx, dword ptr [ebp - 14h]
        xor    al, al
        out    dx, al
        mov    edx, dword ptr [ebp - 8]
        cmp    dword ptr [edx + 10h], 1
        jne    SHORT loc_1_3430
        mov    eax, dword ptr [edx + 14h]
        cmp    eax, 2
        je     SHORT loc_1_3485
        cmp    eax, 3
        je     SHORT loc_1_3485
loc_1_3430:
        mov    edx, dword ptr [ebp - 8]
        xor    al, al
        mov    ebx, dword ptr [edx + 0ch]
        mov    edx, edi
        out    dx, al
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 8
        push   eax
        call   sub_1_0010
        and    eax, 1fffh
        sub    eax, 2
        cmp    eax, ebx
        jae    SHORT loc_1_346B
        mov    edx, dword ptr [ebp - 8]
        mov    eax, 11h
        pop    edi
        pop    esi
        mov    dword ptr [edx + 4], 5
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_346B:
        mov    edx, esi
        cmp    ebx, 3ch
        jae    SHORT loc_1_3480
        mov    eax, 3ch
        out    dx, ax
        sub    eax, ebx
        mov    dword ptr [ebp - 18h], eax
        jmp    SHORT loc_1_348C
loc_1_3480:
        mov    ax, bx
        out    dx, ax
loc_1_3485:
        mov    dword ptr [ebp - 18h], 0
loc_1_348C:
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], 0
        cmp    dword ptr [edx + 18h], 0
        jle    SHORT loc_1_34F4
        lea    edi, [edx + 20h]
loc_1_349F:
        mov    ebx, dword ptr [edi + 4]
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 55h], 0
        mov    eax, dword ptr [edi]
        je     SHORT loc_1_34B7
        push   eax
        push   ebx
        push   esi
        call   sub_1_4F80
        jmp    SHORT loc_1_34E3
loc_1_34B7:
        cdq
        mov    dword ptr [ebp - 4], eax
        sub    eax, edx
        sar    eax, 1
        push   eax
        push   ebx
        push   esi
        call   sub_1_4F94
        mov    eax, dword ptr [ebp - 4]
        cdq
        xor    eax, edx
        sub    eax, edx
        and    eax, 1
        xor    eax, edx
        sub    eax, edx
        je     SHORT loc_1_34E3
        mov    edx, esi
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [ebx + eax - 1]
        out    dx, al
loc_1_34E3:
        add    edi, 0ch
        mov    edx, dword ptr [ebp - 8]
        inc    dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [edx + 18h], eax
        jg     SHORT loc_1_349F
loc_1_34F4:
        cmp    dword ptr [ebp - 18h], 0
        je     SHORT loc_1_352E
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 55h], 0
        mov    eax, dword ptr [ebp - 18h]
        je     SHORT loc_1_3514
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_1_4F80
        jmp    SHORT loc_1_352E
loc_1_3514:
        shr    eax, 1
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_1_4F94
        test   byte ptr [ebp - 18h], 1
        je     SHORT loc_1_352E
        mov    edx, esi
        xor    al, al
        out    dx, al
loc_1_352E:
        mov    edx, dword ptr [ebp - 8]
        cmp    dword ptr [edx + 10h], 1
        jne    SHORT loc_1_3554
        mov    eax, dword ptr [edx + 14h]
        test   eax, eax
        je     SHORT loc_1_3543
        cmp    eax, 3
        jne    SHORT loc_1_3554
loc_1_3543:
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [eax + 4dh], 1
        mov    dword ptr [eax + 51h], 0
loc_1_3554:
        mov    edx, dword ptr [ebp - 14h]
        mov    al, 4
        out    dx, al
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3570:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 14h
        mov    eax, dword ptr [ecx + 0ch]
        push   ebx
        push   esi
        mov    edx, dword ptr [ecx + 38h]
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        lea    edi, [eax + 1]
        add    esi, 4
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 0ch], edx
        add    eax, 4
        test   byte ptr [ecx + 48h], 2
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ecx + 120h]
        mov    dword ptr [ebp - 10h], eax
        je     SHORT loc_1_361A
        cmp    dword ptr [eax + 18h], 0
        je     SHORT loc_1_361A
        mov    edx, edi
        mov    al, 8
        out    dx, al
        mov    ebx, dword ptr [ebp - 8]
        add    ebx, 0ch
        push   ebx
        call   sub_1_0010
        cmp    ax, 1
        jne    SHORT loc_1_361A
        mov    edx, edi
        mov    al, 40h
        out    dx, al
        push   5
        call   sub_1_2240
        push   ebx
        call   0
        and    al, 0fh
        mov    edx, edi
        push   ebx
        mov    byte ptr [ebp - 4], al
        mov    al, 8
        out    dx, al
        call   sub_1_0010
        cmp    ax, 1
        jne    SHORT loc_1_361A
        mov    edx, edi
        mov    al, 40h
        out    dx, al
        push   5
        call   sub_1_2240
        push   ebx
        call   0
        and    al, 0fh
        cmp    al, byte ptr [ebp - 4]
        jne    SHORT loc_1_361A
        mov    eax, dword ptr [ebp - 8]
        lea    edx, [eax + 8]
        mov    al, 80h
        out    dx, al
        mov    al, 1
        out    dx, al
        mov    al, 64h
        out    dx, al
        mov    edx, dword ptr [ebp - 8]
        mov    al, 50h
        out    dx, al
loc_1_361A:
        mov    edx, dword ptr [ebp - 8]
        xor    al, al
        out    dx, al
        cmp    dword ptr [esi + 10h], 1
        jne    SHORT loc_1_3633
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 2
        je     SHORT loc_1_3683
        cmp    eax, 3
        je     SHORT loc_1_3683
loc_1_3633:
        mov    ebx, dword ptr [esi + 0ch]
        mov    edx, edi
        xor    al, al
        out    dx, al
        mov    eax, dword ptr [ebp - 8]
        add    eax, 8
        push   eax
        call   sub_1_0010
        and    eax, 1fffh
        sub    eax, 2
        cmp    eax, ebx
        jae    SHORT loc_1_3668
        mov    eax, 11h
        pop    edi
        mov    dword ptr [esi + 4], 5
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3668:
        mov    edx, dword ptr [ebp - 14h]
        cmp    ebx, 3ch
        jae    SHORT loc_1_367E
        mov    eax, 3ch
        out    dx, ax
        sub    eax, ebx
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_1_368A
loc_1_367E:
        mov    ax, bx
        out    dx, ax
loc_1_3683:
        mov    dword ptr [ebp - 4], 0
loc_1_368A:
        xor    edi, edi
        cmp    dword ptr [esi + 18h], edi
        jle    SHORT loc_1_36AD
        lea    ebx, [esi + 20h]
loc_1_3694:
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 4]
        push   eax
        mov    edx, dword ptr [ebp - 0ch]
        push   ecx
        add    ebx, 0ch
        push   edx
        inc    edi
        call   sub_1_51CC
        cmp    dword ptr [esi + 18h], edi
        jg     SHORT loc_1_3694
loc_1_36AD:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_36C5
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_1_51CC
loc_1_36C5:
        cmp    dword ptr [esi + 10h], 1
        jne    SHORT loc_1_36E8
        mov    eax, dword ptr [esi + 14h]
        test   eax, eax
        je     SHORT loc_1_36D7
        cmp    eax, 3
        jne    SHORT loc_1_36E8
loc_1_36D7:
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [eax + 4dh], 1
        mov    dword ptr [eax + 51h], 0
loc_1_36E8:
        mov    edx, dword ptr [ebp - 8]
        mov    al, 4
        out    dx, al
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3700:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 128h]
        xor    eax, eax
        mov    edx, dword ptr [ebp + 0ch]
        pop    ebp
        mov    dword ptr [edx + 4], ecx
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3720:
        xor    eax, eax
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3730:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    dword ptr [eax + 4], 1fh
        xor    eax, eax
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3750:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    dword ptr [eax + 4], 3dfh
        xor    eax, eax
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3770:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    dword ptr [eax + 4], 9
        xor    eax, eax
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3790:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 18h
        mov    ecx, dword ptr [edx + 0ch]
        push   ebx
        push   esi
        mov    dword ptr [ebp - 4], ecx
        push   edi
        inc    ecx
        mov    esi, dword ptr [ebp - 4]
        mov    eax, dword ptr [edx + 120h]
        add    esi, 4
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 10h], ecx
        mov    dword ptr [ebp - 8], eax
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [edx + 4]
        add    ecx, 10h
        add    eax, dword ptr [edx + 8]
        mov    dword ptr [ebp - 14h], ecx
        mov    dword ptr [ebp - 0ch], eax
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_380F
        mov    edx, dword ptr [ebp - 10h]
        mov    al, 5
        out    dx, al
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0ah
        push   eax
        call   sub_1_0010
        mov    ecx, eax
        and    ecx, 0ffffh
        add    ecx, 3
        cmp    ecx, 8000h
        jne    SHORT loc_1_37FA
        xor    ecx, ecx
loc_1_37FA:
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, ecx
        cmp    eax, 8000h
        jle    SHORT loc_1_380F
        mov    edi, 8000h
        sub    edi, ecx
        jmp    SHORT loc_1_3811
loc_1_380F:
        xor    edi, edi
loc_1_3811:
        mov    edx, dword ptr [ebp - 10h]
        xor    al, al
        mov    ecx, dword ptr [ebp + 0ch]
        out    dx, al
        mov    eax, dword ptr [ecx + 8]
        test   eax, eax
        jle    SHORT loc_1_3830
        and    ax, 1fffh
        mov    edx, dword ptr [ebp - 4]
        add    edx, 0ch
        or     ah, 20h
        out    dx, ax
loc_1_3830:
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 10h]
        test   eax, eax
        je     SHORT loc_1_3842
        cmp    eax, 1
        jmp    NEAR PTR loc_1_38CE
loc_1_3842:
        mov    eax, dword ptr [ebp - 14h]
        test   edi, edi
        mov    ebx, dword ptr [eax + 18h]
        jle    SHORT loc_1_3891
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ecx + 55h], 0
        je     SHORT loc_1_385F
        push   edi
        push   ebx
        push   esi
        call   sub_1_4FBD
        jmp    SHORT loc_1_3889
loc_1_385F:
        mov    eax, edi
        cdq
        sub    eax, edx
        sar    eax, 1
        push   eax
        push   ebx
        push   esi
        call   sub_1_4FD1
        mov    eax, edi
        cdq
        xor    eax, edx
        sub    eax, edx
        and    eax, 1
        xor    eax, edx
        sub    eax, edx
        je     SHORT loc_1_3889
        push   esi
        call   0
        mov    byte ptr [edi + ebx - 1], al
loc_1_3889:
        push   esi
        add    ebx, edi
        call   0
loc_1_3891:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ecx + 55h], 0
        je     SHORT loc_1_38A9
        sub    eax, edi
        push   eax
        push   ebx
        push   esi
        call   sub_1_4FBD
        jmp    SHORT loc_1_38CE
loc_1_38A9:
        sub    eax, edi
        mov    dword ptr [ebp - 18h], eax
        shr    eax, 1
        push   eax
        push   ebx
        push   esi
        call   sub_1_4FD1
        test   byte ptr [ebp - 18h], 1
        je     SHORT loc_1_38CE
        push   esi
        sub    ebx, edi
        call   0
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ebx + ecx - 1], al
loc_1_38CE:
        xor    edi, edi
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 4], edi
        cmp    dword ptr [ecx + 0ch], edi
        jne    SHORT loc_1_3913
        mov    edx, dword ptr [ebp - 10h]
        xor    al, al
        mov    esi, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    esi, 0ch
        push   ecx
        out    dx, al
        mov    edx, esi
        mov    eax, 8000h
        out    dx, ax
        call   0
        test   al, 1
        jne    SHORT loc_1_3906
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0ch], edi
loc_1_3906:
        test   al, 4
        je     SHORT loc_1_3913
        mov    edx, esi
        mov    eax, 2000h
        out    dx, ax
loc_1_3913:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_3920:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ecx + 0ch]
        mov    ebx, dword ptr [ecx + 38h]
        lea    eax, [esi + 1]
        lea    edx, [esi + 4]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 120h]
        add    eax, 10h
        mov    dword ptr [ebp - 18h], edx
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 14h], ebx
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 10h], ecx
        mov    edi, dword ptr [edx + 4]
        add    edi, dword ptr [edx + 8]
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_3999
        mov    edx, dword ptr [ebp - 4]
        mov    al, 5
        out    dx, al
        lea    eax, [esi + 0ah]
        push   eax
        call   sub_1_0010
        mov    ecx, eax
        and    ecx, 0ffffh
        add    ecx, 3
        cmp    ecx, 8000h
        jne    SHORT loc_1_3986
        xor    ecx, ecx
loc_1_3986:
        lea    eax, [edi + ecx]
        cmp    eax, 8000h
        jle    SHORT loc_1_3999
        mov    ebx, 8000h
        sub    ebx, ecx
        jmp    SHORT loc_1_399B
loc_1_3999:
        xor    ebx, ebx
loc_1_399B:
        mov    edx, dword ptr [ebp - 4]
        xor    al, al
        out    dx, al
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        test   eax, eax
        jle    SHORT loc_1_39B7
        and    ax, 1fffh
        lea    edx, [esi + 0ch]
        or     ah, 20h
        out    dx, ax
loc_1_39B7:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        test   eax, eax
        je     SHORT loc_1_39C6
        cmp    eax, 1
        jmp    SHORT loc_1_39FA
loc_1_39C6:
        mov    eax, dword ptr [ebp - 8]
        test   ebx, ebx
        mov    ecx, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 0ch], ecx
        jle    SHORT loc_1_39EA
        push   ebx
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        push   ecx
        call   sub_1_51CC
        mov    ecx, dword ptr [ebp - 18h]
        push   ecx
        call   0
        add    dword ptr [ebp - 0ch], ebx
loc_1_39EA:
        sub    edi, ebx
        mov    eax, dword ptr [ebp - 14h]
        push   edi
        mov    ecx, dword ptr [ebp - 0ch]
        push   eax
        push   ecx
        call   sub_1_51CC
loc_1_39FA:
        xor    edi, edi
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 4], edi
        cmp    dword ptr [ecx + 0ch], edi
        jne    SHORT loc_1_3A39
        mov    edx, dword ptr [ebp - 4]
        xor    al, al
        lea    ebx, [esi + 0ch]
        push   esi
        out    dx, al
        mov    edx, ebx
        mov    eax, 8000h
        out    dx, ax
        call   0
        test   al, 1
        jne    SHORT loc_1_3A2C
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 0ch], edi
loc_1_3A2C:
        test   al, 4
        je     SHORT loc_1_3A39
        mov    edx, ebx
        mov    eax, 2000h
        out    dx, ax
loc_1_3A39:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3A50:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        mov    ebp, esp
        add    edx, 4
        jne    SHORT loc_1_3A62
        xor    eax, eax
        pop    ebp
        ret    8
loc_1_3A62:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 12ch], 0
        jne    SHORT loc_1_3A76
        mov    dword ptr [ecx + 12ch], edx
        jmp    SHORT loc_1_3A7E
loc_1_3A76:
        mov    eax, dword ptr [ecx + 130h]
        mov    dword ptr [eax], edx
loc_1_3A7E:
        mov    eax, 2
        mov    dword ptr [edx + 4], eax
        cmp    dword ptr [edx], 0
        je     SHORT loc_1_3A95
loc_1_3A8B:
        mov    edx, dword ptr [edx]
        mov    dword ptr [edx + 4], eax
        cmp    dword ptr [edx], 0
        jne    SHORT loc_1_3A8B
loc_1_3A95:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 130h], edx
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3AB0:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 4]
        test   ecx, ecx
        mov    eax, dword ptr [ebp + 8]
        je     SHORT loc_1_3ACF
        test   dword ptr [eax + 44h], ecx
        jne    SHORT loc_1_3ACF
        mov    eax, 0dh
        pop    ebp
        ret    8
loc_1_3ACF:
        pop    ebp
        mov    dword ptr [eax + 48h], ecx
        xor    eax, eax
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3AE0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 48h]
        xor    eax, eax
        mov    edx, dword ptr [ebp + 0ch]
        pop    ebp
        mov    dword ptr [edx + 4], ecx
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3B00:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 5
        ja     SHORT loc_1_3B1C
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_3B16:
        xor    eax, eax
        pop    ebp
        ret    8
loc_1_3B1C:
        mov    eax, 5
        pop    ebp
        ret    8
loc_1_3B25:
        mov    eax, 0fh
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 064h
        db  024h, 000h
sub_1_3B50:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        cmp    dword ptr [eax + 4], 5
        jbe    SHORT loc_1_3B66
        mov    eax, 5
        pop    ebp
        ret    8
loc_1_3B66:
        xor    eax, eax
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_3B70:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 8
        mov    ecx, dword ptr [eax + 4]
        push   ebx
        push   esi
        mov    edx, dword ptr [ebp + 8]
        push   edi
        mov    dword ptr [ebp - 8], ecx
        mov    esi, dword ptr [edx + 6ch]
        mov    edi, dword ptr [edx + 0ch]
        test   ecx, ecx
        lea    eax, [edi + 1]
        mov    dword ptr [ebp - 4], eax
        je     SHORT loc_1_3BA9
        test   dword ptr [edx + 68h], ecx
        jne    SHORT loc_1_3BA9
        mov    eax, 0dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3BA9:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ebx, eax
        mov    dword ptr [ecx + 6ch], eax
        not    ebx
        and    ebx, esi
        test   bl, 4
        je     SHORT loc_1_3BDF
        mov    edx, dword ptr [ebp - 4]
        mov    al, 42h
        out    dx, al
        push   0ah
        call   sub_1_2240
        lea    ecx, [edi + 9]
        push   ecx
        call   0
        and    al, 0fbh
        lea    edx, [edi + 9]
        out    dx, al
        mov    edx, dword ptr [ebp - 4]
        xor    al, al
        out    dx, al
loc_1_3BDF:
        test   bl, 10h
        je     SHORT loc_1_3C12
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 0bch], 0
        jle    SHORT loc_1_3C12
        mov    edx, dword ptr [ebp - 4]
        mov    al, 42h
        out    dx, al
        push   0ah
        call   sub_1_2240
        lea    ecx, [edi + 9]
        push   ecx
        call   0
        and    al, 0feh
        lea    edx, [edi + 9]
        out    dx, al
        mov    edx, dword ptr [ebp - 4]
        xor    al, al
        out    dx, al
loc_1_3C12:
        test   bl, 8
        je     SHORT loc_1_3C38
        mov    edx, dword ptr [ebp - 4]
        mov    al, 42h
        push   0ah
        lea    ebx, [edi + 9]
        out    dx, al
        call   sub_1_2240
        push   ebx
        call   0
        or     al, 8
        mov    edx, ebx
        out    dx, al
        mov    edx, dword ptr [ebp - 4]
        xor    al, al
        out    dx, al
loc_1_3C38:
        not    esi
        and    esi, dword ptr [ebp - 8]
        test   esi, 4
        je     SHORT loc_1_3C66
        mov    edx, dword ptr [ebp - 4]
        mov    al, 42h
        push   0ah
        lea    ebx, [edi + 9]
        out    dx, al
        call   sub_1_2240
        push   ebx
        call   0
        or     al, 4
        mov    edx, ebx
        out    dx, al
        mov    edx, dword ptr [ebp - 4]
        xor    al, al
        out    dx, al
loc_1_3C66:
        test   esi, 10h
        je     SHORT loc_1_3C9B
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 0bch], 0
        jle    SHORT loc_1_3C9B
        mov    edx, dword ptr [ebp - 4]
        mov    al, 42h
        push   0ah
        lea    ebx, [edi + 9]
        out    dx, al
        call   sub_1_2240
        push   ebx
        call   0
        or     al, 1
        mov    edx, ebx
        out    dx, al
        mov    edx, dword ptr [ebp - 4]
        xor    al, al
        out    dx, al
loc_1_3C9B:
        test   esi, 8
        je     SHORT loc_1_3CC4
        mov    edx, dword ptr [ebp - 4]
        mov    al, 42h
        push   0ah
        add    edi, 9
        out    dx, al
        call   sub_1_2240
        push   edi
        call   0
        and    al, 0f7h
        mov    edx, edi
        out    dx, al
        mov    edx, dword ptr [ebp - 4]
        xor    al, al
        out    dx, al
loc_1_3CC4:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
sub_1_3CD0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 6ch]
        xor    eax, eax
        mov    edx, dword ptr [ebp + 0ch]
        pop    ebp
        mov    dword ptr [edx + 4], ecx
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3CF0:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 4
        ja     SHORT loc_1_3D06
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_3D06:
        mov    eax, 5
        pop    ebp
        ret    8
loc_1_3D0F:
        mov    eax, 0fh
        pop    ebp
        ret    8
loc_1_3D18:
        xor    eax, eax
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h
        db  000h, 000h
sub_1_3D40:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        cmp    dword ptr [eax + 4], 4
        jbe    SHORT loc_1_3D56
        mov    eax, 5
        pop    ebp
        ret    8
loc_1_3D56:
        xor    eax, eax
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_3D60:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [ebp - 4], eax
        lea    edx, [eax + 1]
        xor    eax, eax
        mov    dword ptr [ebp - 8], eax
loc_1_3D7D:
        mov    cl, byte ptr [edi + eax + 4]
        inc    eax
        cmp    eax, 6
        mov    byte ptr [esi + eax + 0b1h], cl
        jl     SHORT loc_1_3D7D
        mov    al, 50h
        xor    esi, esi
        out    dx, al
        mov    ebx, 8
loc_1_3D98:
        mov    eax, dword ptr [ebp - 4]
        lea    edx, [eax + ebx]
        mov    al, byte ptr [edi + esi + 4]
        out    dx, al
        push   edx
        call   0
        cmp    al, byte ptr [edi + esi + 4]
        jne    SHORT loc_1_3DB8
        inc    esi
        inc    ebx
        cmp    esi, 6
        jl     SHORT loc_1_3D98
        jmp    SHORT loc_1_3DBF
loc_1_3DB8:
        mov    dword ptr [ebp - 8], 1
loc_1_3DBF:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        dec    eax
        pop    esi
        pop    ebx
        cmp    eax, 1
        sbb    eax, eax
        mov    esp, ebp
        inc    eax
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3DE0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
loc_1_3DEC:
        mov    bl, byte ptr [eax + edx + 0b2h]
        inc    edx
        cmp    edx, 6
        mov    byte ptr [ecx + edx + 3], bl
        jl     SHORT loc_1_3DEC
        xor    eax, eax
        pop    ebx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3E10:
        mov    eax, 0ch
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3E20:
        mov    eax, 0ch
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3E30:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        je     SHORT loc_1_3E51
        cmp    eax, 1
        je     SHORT loc_1_3E5E
        cmp    eax, 2
        je     SHORT loc_1_3E6B
        mov    eax, 6
        pop    ebp
        ret    8
loc_1_3E51:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E80
        pop    ebp
        ret    8
loc_1_3E5E:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3EB0
        pop    ebp
        ret    8
loc_1_3E6B:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3EE0
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3E80:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [eax + 0ch]
        mov    al, 1
        add    esi, 0ch
        push   esi
        lea    edx, [esi - 0bh]
        out    dx, al
        call   0
        or     al, 3
        mov    edx, esi
        out    dx, al
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3EB0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [eax + 0ch]
        mov    al, 1
        add    esi, 0ch
        push   esi
        lea    edx, [esi - 0bh]
        out    dx, al
        call   0
        or     al, 70h
        mov    edx, esi
        out    dx, al
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3EE0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    edx, dword ptr [eax + 0ch]
        mov    al, 4
        pop    ebp
        out    dx, al
        xor    eax, eax
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3F00:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        je     SHORT loc_1_3F21
        cmp    eax, 1
        je     SHORT loc_1_3F2E
        cmp    eax, 2
        je     SHORT loc_1_3F3B
        mov    eax, 6
        pop    ebp
        ret    8
loc_1_3F21:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3F50
        pop    ebp
        ret    8
loc_1_3F2E:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3F80
        pop    ebp
        ret    8
loc_1_3F3B:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3FB0
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3F50:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [eax + 0ch]
        mov    al, 1
        add    esi, 0ch
        push   esi
        lea    edx, [esi - 0bh]
        out    dx, al
        call   0
        and    al, 0fch
        mov    edx, esi
        out    dx, al
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3F80:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [eax + 0ch]
        mov    al, 1
        add    esi, 0ch
        push   esi
        lea    edx, [esi - 0bh]
        out    dx, al
        call   0
        and    al, 8fh
        mov    edx, esi
        out    dx, al
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3FB0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    edx, dword ptr [eax + 0ch]
        xor    al, al
        pop    ebp
        out    dx, al
        xor    eax, eax
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3FD0:
        xor    eax, eax
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3FE0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [eax + 0ch]
        push   1
        mov    al, 4
        lea    edx, [esi + 1]
        out    dx, al
        call   sub_1_2240
        lea    edx, [esi + 9]
        xor    al, al
        out    dx, al
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4010:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 4]
        test   ecx, ecx
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 8ch]
        je     SHORT loc_1_4038
        test   dword ptr [eax + 88h], ecx
        jne    SHORT loc_1_4038
        mov    eax, 4
        pop    ebp
        ret    8
loc_1_4038:
        mov    dword ptr [eax + 8ch], ecx
        mov    eax, ecx
        not    eax
        and    eax, edx
        test   al, 1
        je     SHORT loc_1_405C
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_405C:
        not    edx
        and    edx, ecx
        test   dl, 1
        je     SHORT loc_1_4079
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_4079:
        xor    eax, eax
        pop    ebp
        ret    8
        db  090h
sub_1_4080:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 8ch]
        xor    eax, eax
        mov    edx, dword ptr [ebp + 0ch]
        pop    ebp
        mov    dword ptr [edx + 4], ecx
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_40A0:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        je     SHORT loc_1_40C1
        cmp    eax, 1
        jl     SHORT loc_1_40B8
        cmp    eax, 2
        jle    SHORT loc_1_40C7
loc_1_40B8:
        mov    eax, 5
        pop    ebp
        ret    8
loc_1_40C1:
        xor    eax, eax
        pop    ebp
        ret    8
loc_1_40C7:
        mov    eax, 0fh
        pop    ebp
        ret    8
sub_1_40D0:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        cmp    dword ptr [eax + 4], 2
        jbe    SHORT loc_1_40E6
        mov    eax, 5
        pop    ebp
        ret    8
loc_1_40E6:
        xor    eax, eax
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_40F0:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 4]
        dec    eax
        cmp    eax, 2eh
        ja     SHORT loc_1_4107
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_4107:
        mov    eax, 3
        pop    ebp
        ret    8
loc_1_4110:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_411D:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_412A:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_4137:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_4144:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_4151:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_415E:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_416B:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_4178:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_4185:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_4192:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_419F:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_41AC:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_41B9:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_41C6:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_41D3:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_41E0:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_41ED:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_41FA:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_4207:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_4214:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_4221:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_422E:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_423B:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_4248:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_4255:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_4262:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_426F:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_427C:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_4289:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_4296:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_42A3:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_42B0:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_42BD:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_42CA:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_42D7:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_42E4:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_42F1:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_42FE:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_430B:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0
        ret    8
loc_1_4318:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0
        ret    8
loc_1_4325:
        xor    eax, eax
        pop    ebp
        mov    dword ptr [ecx + 8], 0
        ret    8
loc_1_4332:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_433F:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_434C:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
loc_1_4359:
        xor    eax, eax
        pop    ebp
        db  0C7h, 041h, 008h, 000h, 000h, 000h, 000h
        ret    8
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h, 000h
        db  000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4430:
        xor    eax, eax
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4440:
        mov    eax, 0ch
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_4450:
        xor    eax, eax
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4460:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
loc_1_446C:
        mov    bl, byte ptr [eax + edx + 0ach]
        inc    edx
        cmp    edx, 6
        mov    byte ptr [ecx + edx + 3], bl
        jl     SHORT loc_1_446C
        xor    eax, eax
        pop    ebx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4490:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 0ffffffffh
        push   esi
        mov    eax, dword ptr [edx + 0ch]
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        add    esi, 4
        mov    dword ptr [ebp - 0ch], eax
        inc    eax
        test   byte ptr [esi], 1
        mov    dword ptr [ebp - 10h], eax
        jne    SHORT loc_1_44C6
        mov    eax, 9
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_44C6:
        xor    ecx, ecx
        mov    edi, dword ptr [ebp + 8]
        lea    eax, [edi + 0f6h]
loc_1_44D1:
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_44E1
        cmp    dword ptr [ebp - 4], -1
        jne    SHORT loc_1_450F
        mov    dword ptr [ebp - 4], ecx
        jmp    SHORT loc_1_450F
loc_1_44E1:
        mov    dword ptr [ebp - 8], 1
        xor    ebx, ebx
loc_1_44EA:
        mov    dl, byte ptr [esi + ebx]
        cmp    byte ptr [edi + ebx + 0c0h], dl
        jne    SHORT loc_1_44FE
        inc    ebx
        cmp    ebx, 6
        jl     SHORT loc_1_44EA
        jmp    SHORT loc_1_4505
loc_1_44FE:
        mov    dword ptr [ebp - 8], 0
loc_1_4505:
        cmp    dword ptr [ebp - 8], 1
        je     NEAR PTR loc_1_45FB
loc_1_450F:
        add    edi, 6
        add    eax, 4
        inc    ecx
        cmp    ecx, 9
        jl     SHORT loc_1_44D1
        cmp    dword ptr [ebp - 4], 0
        jl     NEAR PTR loc_1_461E
        xor    ecx, ecx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        lea    ebx, [eax + eax*2]
        lea    eax, [edx + ebx*2]
loc_1_4533:
        mov    dl, byte ptr [esi + ecx]
        inc    ecx
        cmp    ecx, 6
        mov    byte ptr [eax + ecx + 0bfh], dl
        jl     SHORT loc_1_4533
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 4]
        push   0c8h
        mov    edi, dword ptr [edx + 120h]
        mov    edx, dword ptr [ebp - 10h]
        lea    ecx, [eax + edi + 20h]
        mov    dword ptr [ebp - 8], ecx
        mov    al, byte ptr [ecx]
        out    dx, al
        call   sub_1_2240
        mov    ecx, dword ptr [ebp - 4]
        mov    ebx, dword ptr [edi + ecx*4 + 29h]
        xor    edi, edi
loc_1_456F:
        mov    eax, dword ptr [ebp - 0ch]
        lea    edx, [eax + ebx]
        mov    al, byte ptr [esi + edi]
        out    dx, al
        push   edx
        call   0
        cmp    al, byte ptr [esi + edi]
        jne    NEAR PTR loc_1_4610
        inc    ebx
        cmp    ebx, 10h
        jl     SHORT loc_1_45A8
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 10h]
        push   0c8h
        mov    ebx, 8
        mov    al, byte ptr [eax]
        inc    al
        out    dx, al
        call   sub_1_2240
loc_1_45A8:
        inc    edi
        cmp    edi, 6
        jl     SHORT loc_1_456F
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [edx + eax*4 + 0f6h]
        mov    eax, dword ptr [edx + 0bch]
        inc    eax
        mov    dword ptr [edx + 0bch], eax
        test   eax, eax
        jle    SHORT loc_1_45F0
        test   byte ptr [edx + 6ch], 10h
        je     SHORT loc_1_45F0
        mov    edx, dword ptr [ebp - 10h]
        mov    al, 42h
        out    dx, al
        mov    esi, dword ptr [ebp - 0ch]
        add    esi, 9
        push   esi
        call   0
        or     al, 1
        mov    edx, esi
        out    dx, al
        mov    edx, dword ptr [ebp + 8]
        or     dword ptr [edx + 6ch], 10h
loc_1_45F0:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_45FB:
        mov    edx, dword ptr [ebp + 8]
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        inc    dword ptr [edx + ecx*4 + 0f6h]
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_4610:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_461E:
        mov    eax, 10h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_4630:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0ch
        mov    eax, dword ptr [ecx + 0ch]
        push   ebx
        push   esi
        mov    dword ptr [ebp - 8], eax
        push   edi
        inc    eax
        cmp    dword ptr [ecx + 0bch], 0
        mov    dword ptr [ebp - 0ch], eax
        jne    SHORT loc_1_465E
        mov    eax, 0eh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_465E:
        mov    eax, dword ptr [ebp + 0ch]
        test   byte ptr [eax + 4], 1
        jne    SHORT loc_1_4675
        mov    eax, 9
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_4675:
        xor    esi, esi
        mov    edi, dword ptr [ebp + 8]
        lea    ebx, [edi + 0f6h]
loc_1_4680:
        cmp    dword ptr [ebx], 0
        je     SHORT loc_1_46B0
        mov    dword ptr [ebp - 4], 1
        xor    ecx, ecx
loc_1_468E:
        mov    dl, byte ptr [eax + ecx + 4]
        cmp    byte ptr [ecx + edi + 0c0h], dl
        jne    SHORT loc_1_46A3
        inc    ecx
        cmp    ecx, 6
        jl     SHORT loc_1_468E
        jmp    SHORT loc_1_46AA
loc_1_46A3:
        mov    dword ptr [ebp - 4], 0
loc_1_46AA:
        cmp    dword ptr [ebp - 4], 1
        je     SHORT loc_1_46C7
loc_1_46B0:
        add    edi, 6
        add    ebx, 4
        inc    esi
        cmp    esi, 9
        jl     SHORT loc_1_4680
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_46C7:
        mov    ecx, dword ptr [ebp + 8]
        lea    ecx, [ecx + esi*4 + 0f6h]
        mov    eax, dword ptr [ecx]
        dec    eax
        mov    dword ptr [ecx], eax
        jne    NEAR PTR loc_1_4770
        lea    eax, [esi + esi*2]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0ch]
        lea    eax, [ecx + eax*2 + 0c0h]
        push   0c8h
        mov    dword ptr [eax], 0
        mov    word ptr [eax + 4], 0
        mov    ebx, dword ptr [ecx + 120h]
        lea    edi, [esi + ebx + 20h]
        mov    al, byte ptr [edi]
        out    dx, al
        call   sub_1_2240
        mov    ecx, dword ptr [ebx + esi*4 + 29h]
        mov    esi, 6
loc_1_4718:
        mov    edx, dword ptr [ebp - 8]
        xor    al, al
        add    edx, ecx
        inc    ecx
        out    dx, al
        cmp    ecx, 10h
        jl     SHORT loc_1_473D
        mov    al, byte ptr [edi]
        mov    edx, dword ptr [ebp - 0ch]
        inc    al
        push   0c8h
        out    dx, al
        call   sub_1_2240
        mov    ecx, 8
loc_1_473D:
        dec    esi
        jne    SHORT loc_1_4718
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0bch]
        dec    eax
        mov    dword ptr [ecx + 0bch], eax
        jne    SHORT loc_1_4770
        mov    edx, dword ptr [ebp - 0ch]
        mov    al, 42h
        out    dx, al
        mov    esi, dword ptr [ebp - 8]
        add    esi, 9
        push   esi
        call   0
        and    al, 0feh
        mov    edx, esi
        mov    ecx, dword ptr [ebp + 8]
        out    dx, al
        and    dword ptr [ecx + 6ch], 0ffffffefh
loc_1_4770:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_4780:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0bch]
        xor    eax, eax
        mov    edx, dword ptr [ebp + 0ch]
        pop    ebp
        mov    dword ptr [edx + 4], ecx
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_47A0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 4]
        lea    eax, [edx + 0f6h]
loc_1_47B7:
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_47D5
        xor    edi, edi
loc_1_47BE:
        lea    ebx, [ecx + edi]
        inc    edi
        cmp    edi, 6
        mov    bl, byte ptr [ebx + edx + 0c0h]
        mov    byte ptr [esi + edi - 1], bl
        jl     SHORT loc_1_47BE
        add    esi, 6
loc_1_47D5:
        add    ecx, 6
        add    eax, 4
        cmp    ecx, 24h
        jl     SHORT loc_1_47B7
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_47F0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    dword ptr [eax + 128h], 7
        xor    eax, eax
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4810:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 4]
        cmp    eax, 9
        ja     SHORT loc_1_4826
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_4826:
        mov    eax, 7
        pop    ebp
        ret    8
loc_1_482F:
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [ecx + 0ch], eax
        or     dword ptr [ecx + 8], 1
        xor    eax, eax
        ret    8
loc_1_4842:
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [ecx + 24h], eax
        or     dword ptr [ecx + 8], 2
        xor    eax, eax
        ret    8
loc_1_4855:
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [ecx + 18h], eax
        or     dword ptr [ecx + 8], 4
        xor    eax, eax
        ret    8
loc_1_4868:
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [ecx + 30h], eax
        or     dword ptr [ecx + 8], 8
        xor    eax, eax
        ret    8
loc_1_487B:
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [ecx + 20h], eax
        xor    eax, eax
        ret    8
loc_1_488A:
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [ecx + 38h], eax
        xor    eax, eax
        ret    8
loc_1_4899:
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [ecx + 1ch], eax
        xor    eax, eax
        ret    8
loc_1_48A8:
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [ecx + 34h], eax
        xor    eax, eax
        ret    8
loc_1_48B7:
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [ecx + 14h], eax
        or     dword ptr [ecx + 8], 10h
        xor    eax, eax
        ret    8
loc_1_48CA:
        mov    eax, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [ecx + 2ch], eax
        or     dword ptr [ecx + 8], 20h
        xor    eax, eax
        ret    8
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 08Dh, 064h, 024h, 000h, 08Dh
        db  064h, 024h, 000h
_LTEXT ends
        end
