; ibmtok4_pcode_02.asm -- IBMTOK4 object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 58h
        mov    word ptr [ebp - 14h], 8
        db  0C7h, 045h, 0F0h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 12h], 8
        mov    word ptr [ebp - 20h], 0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    ecx, [ebp - 20h]
        push   ecx
        lea    edx, [ebp - 4]
        mov    word ptr [ebp - 1eh], 2
        mov    word ptr [ebp - 1ch], 0e000h
        mov    word ptr [ebp - 1ah], 0e001h
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0044:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        lea    edx, [ebp - 0ch]
        push   ecx
        push   0
        push   8
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0058:
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     SHORT loc_2_006A
        mov    eax, 0c000009ah
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_006A:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx], eax
        mov    byte ptr [ebp - 58h], 3
        mov    byte ptr [ebp - 57h], 0
        db  0C7h, 045h, 0AEh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0B2h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0C2h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0B6h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0BAh, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [ebp - 10h]
        db  0C7h, 045h, 0BEh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0CAh, 000h, 000h, 000h, 000h
        push   36h
        lea    ecx, [ebp - 58h]
        db  0C7h, 045h, 0C6h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0CEh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0D2h, 000h, 000h, 000h, 000h
        push   ecx
        mov    dword ptr [ebp - 2ah], edx
        mov    dword ptr [ebp - 26h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 0ch]
        lea    ecx, [ebp - 8]
        push   edx
        push   eax
        lea    eax, [edx + 4]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_00E7:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_0103
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_00F8:
        mov    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_0103:
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_2_0110:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [ebp - 2ch]
        push   esi
        call   sub_2_0330
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_012A:
        xor    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        cmp    byte ptr [ebp + 14h], 1
        sbb    eax, eax
        mov    ecx, dword ptr [esi + 5ah]
        and    eax, 0fffffffeh
        lea    edi, [esi + 82h]
        add    eax, 3
        mov    dword ptr [ebp - 12h], ecx
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 22h], eax
        lea    eax, [ebp - 2ch]
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 16h], 1
        mov    dword ptr [ebp - 0eh], 4
        push   eax
        push   ecx
        push   edx
        mov    eax, dword ptr [esi + 7eh]
        push   esi
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0175:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    NEAR PTR loc_2_0322
        cmp    byte ptr [ebp + 18h], 0
        je     SHORT loc_2_01AD
        push   0
        mov    eax, dword ptr [edi]
        push   0aa00000ah
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0195:
        add    esp, 0ch
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_01A0:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_01AD:
        push   esi
        call   sub_2_0990
        test   al, al
        jne    SHORT loc_2_01CC
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_01BF:
        mov    eax, 0c0010006h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_01CC:
        cmp    byte ptr [esi + 3bh], 0
        je     SHORT loc_2_01DF
        mov    eax, dword ptr [esi + 0aeh]
        mov    byte ptr [eax + 1e18h], 0c0h
loc_2_01DF:
        mov    edx, dword ptr [esi + 5ah]
        xor    al, al
        inc    edx
        push   0c350h
        out    dx, al
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_01F0:
        mov    edx, dword ptr [esi + 5ah]
        xor    al, al
        add    edx, 2
        mov    ecx, dword ptr [esi + 31h]
        out    dx, al
        cmp    byte ptr [esi + 39h], 1
        mov    eax, 2
        adc    eax, -1
        push   eax
        push   0
        push   ecx
        mov    eax, dword ptr [edi]
        push   ecx
        lea    edx, [ebp - 4]
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        push   esi
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0225:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_2_0301
        push   esi
        call   sub_2_0380
        test   eax, eax
        je     SHORT loc_2_026A
        push   1
        mov    eax, dword ptr [edi]
        push   0
        push   2
        push   0aa000004h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_024C:
        add    esp, 14h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0255:
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_025D:
        mov    eax, 0c0010006h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_026A:
        lea    eax, [esi + 86h]
        lea    ecx, [esi + 7ah]
        push   eax
        lea    edx, [esi + 4eh]
        push   ecx
        push   edx
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_0E20
        test   al, al
        jne    SHORT loc_2_02C7
        push   2
        mov    eax, dword ptr [edi]
        push   0
        push   2
        push   0aa000002h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_02A9:
        add    esp, 14h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_02B2:
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_02BA:
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_02C7:
        db  0B9h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 1dh]
        push   7530h
        mov    dword ptr [esi + 19h], ecx
        push   eax
        mov    dword ptr [eax], 0
        mov    dword ptr [esi + 21h], ecx
        mov    dword ptr [esi + 25h], esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_02E9:
        db  068h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [edi]
        push   esi
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_02F7:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_0301:
        push   3
        mov    eax, dword ptr [edi]
        push   0
        push   2
        push   0aa000005h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0314:
        add    esp, 14h
        mov    eax, dword ptr [edi]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_031F:
        mov    eax, dword ptr [ebp - 4]
loc_2_0322:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_0330:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        lea    eax, [ecx + 62h]
        mov    dword ptr [ecx + 66h], eax
        mov    dword ptr [eax], eax
        lea    eax, [ecx + 6ah]
        mov    dword ptr [ecx + 6eh], eax
        mov    dword ptr [eax], eax
        mov    word ptr [ecx + 0e6h], 100h
        lea    eax, [ecx + 72h]
        mov    dword ptr [ecx + 76h], eax
        pop    ebp
        mov    dword ptr [eax], eax
        mov    eax, 1
        mov    word ptr [ecx + 0dch], ax
        mov    byte ptr [ecx + 170h], al
        mov    byte ptr [ecx + 172h], al
        mov    byte ptr [ecx + 16fh], al
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_0380:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 32h
        push   esi
        call   sub_2_0690
        cmp    byte ptr [esi + 2ch], 0
        jne    SHORT loc_2_03C6
loc_2_039D:
        test   edi, edi
        je     SHORT loc_2_03B2
        push   186a0h
        dec    edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_03AC:
        cmp    byte ptr [esi + 2ch], 0
        je     SHORT loc_2_039D
loc_2_03B2:
        cmp    byte ptr [esi + 2ch], 0
        jne    SHORT loc_2_03C6
        mov    eax, 0c0010006h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_03C6:
        mov    eax, dword ptr [esi + 0aeh]
        mov    cl, byte ptr [eax + 1e02h]
        mov    al, byte ptr [eax + 1e03h]
        and    ecx, 0ffff00ffh
        and    eax, 0ffff00ffh
        shl    cx, 8
        add    ax, cx
        mov    ecx, dword ptr [esi + 0b2h]
        mov    word ptr [esi + 0b6h], ax
        mov    edx, eax
        and    edx, 0ffffh
        mov    dword ptr [ebp - 8], ecx
        add    ecx, edx
        mov    ax, word ptr [ecx + 6]
        mov    word ptr [ebp - 4], ax
        test   ax, ax
        je     SHORT loc_2_0470
        cmp    ax, 30h
        jne    SHORT loc_2_043F
        push   7
        mov    eax, dword ptr [esi + 82h]
        push   4
        push   32h
        push   0aa00000ah
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_042E:
        add    esp, 14h
        mov    eax, 0c0010006h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_043F:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 82h]
        and    eax, 0ffffh
        push   eax
        push   7
        push   4
        push   3
        push   0aa000003h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_045F:
        add    esp, 18h
        mov    eax, 0c0010006h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0470:
        mov    byte ptr [esi + 16fh], 0
        mov    byte ptr [esi + 2ch], 1
        mov    al, byte ptr [ecx + 1]
        lea    edx, [esi + 35h]
        mov    byte ptr [ebp - 4], al
        test   byte ptr [ebp - 4], 1
        je     SHORT loc_2_048F
        mov    byte ptr [edx], 1
        jmp    SHORT loc_2_0492
loc_2_048F:
        mov    byte ptr [edx], 0
loc_2_0492:
        mov    al, byte ptr [ecx + 2]
        mov    byte ptr [esi + 36h], 1
        mov    byte ptr [ebp - 4], al
        test   byte ptr [ebp - 4], 4
        jne    SHORT loc_2_04A6
        mov    byte ptr [esi + 36h], 0
loc_2_04A6:
        mov    edi, dword ptr [esi + 40h]
        cmp    edi, 2000h
        jbe    NEAR PTR loc_2_05DD
        mov    al, byte ptr [esi + 0dch]
        cmp    edi, 10000h
        mov    byte ptr [ebp - 1], al
        jne    SHORT loc_2_04CE
        sub    edi, 2830h
        jmp    SHORT loc_2_04D4
loc_2_04CE:
        sub    edi, 630h
loc_2_04D4:
        cmp    byte ptr [edx], 0
        je     SHORT loc_2_04E6
        mov    ax, word ptr [esi + 46h]
        mov    word ptr [esi + 0deh], ax
        jmp    SHORT loc_2_04F1
loc_2_04E6:
        mov    dx, word ptr [esi + 44h]
        mov    word ptr [esi + 0deh], dx
loc_2_04F1:
        mov    ax, word ptr [esi + 0deh]
        mov    edx, eax
        and    edx, 0ffffh
        mov    dword ptr [ebp - 10h], edx
        cmp    edi, edx
        jae    SHORT loc_2_051B
        xor    ebx, ebx
        mov    eax, edi
        mov    bx, word ptr [esi + 0e6h]
        sub    edx, edx
        div    ebx
        shr    eax, 1
        jmp    SHORT loc_2_052B
loc_2_051B:
        lea    edx, [esi + 0e4h]
        sub    dx, dx
        div    word ptr [esi + 0e6h]
loc_2_052B:
        inc    ax
        mov    word ptr [esi + 0e4h], ax
        xor    eax, eax
        mov    ax, word ptr [esi + 0e6h]
        xor    ebx, ebx
        mov    bl, byte ptr [ebp - 1]
        mov    dword ptr [ebp - 0ch], eax
        xor    eax, eax
        sub    edx, edx
        mov    ax, word ptr [esi + 0e4h]
        imul   eax, dword ptr [ebp - 0ch]
        sub    edi, eax
        mov    eax, edi
        div    ebx
        cmp    eax, dword ptr [ebp - 10h]
        jae    SHORT loc_2_059C
        cmp    eax, 1000h
        jae    SHORT loc_2_0571
        mov    word ptr [esi + 0deh], 800h
        jmp    SHORT loc_2_059C
loc_2_0571:
        cmp    eax, 2000h
        jae    SHORT loc_2_0583
        mov    word ptr [esi + 0deh], 1000h
        jmp    SHORT loc_2_059C
loc_2_0583:
        mov    word ptr [esi + 0deh], 2000h
        cmp    eax, 4000h
        jb     SHORT loc_2_059C
        mov    word ptr [esi + 0deh], 4000h
loc_2_059C:
        mov    eax, dword ptr [esi + 0e0h]
        xor    edx, edx
        mov    dx, word ptr [esi + 0deh]
        cmp    edx, eax
        jbe    SHORT loc_2_05B6
        mov    word ptr [esi + 0deh], ax
loc_2_05B6:
        xor    eax, eax
        mov    ax, word ptr [esi + 0deh]
        lea    edx, [eax - 6]
        imul   eax, ebx
        sub    edi, eax
        mov    dword ptr [esi + 0e0h], edx
        mov    eax, edi
        sub    edx, edx
        div    dword ptr [ebp - 0ch]
        add    word ptr [esi + 0e4h], ax
        jmp    SHORT loc_2_061E
loc_2_05DD:
        mov    word ptr [esi + 0deh], 800h
        mov    word ptr [esi + 0dch], 1
        mov    eax, dword ptr [esi + 0e0h]
        cmp    eax, 800h
        jae    SHORT loc_2_0603
        mov    word ptr [esi + 0deh], ax
loc_2_0603:
        mov    word ptr [esi + 0e4h], 0fh
        xor    eax, eax
        mov    ax, word ptr [esi + 0deh]
        sub    eax, 6
        mov    dword ptr [esi + 0e0h], eax
loc_2_061E:
        mov    ax, word ptr [ecx + 8]
        mov    edi, dword ptr [ebp - 8]
        mov    word ptr [ebp - 4], ax
        push   6
        mov    ah, al
        mov    al, byte ptr [ebp - 3]
        mov    edx, eax
        lea    eax, [esi + 54h]
        and    edx, 0ffffh
        add    edi, edx
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0644:
        lea    eax, [esi + 4eh]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_0673
        cmp    byte ptr [esi + 4fh], 0
        jne    SHORT loc_2_0673
        cmp    byte ptr [esi + 50h], 0
        jne    SHORT loc_2_0673
        cmp    byte ptr [esi + 51h], 0
        jne    SHORT loc_2_0673
        cmp    byte ptr [esi + 52h], 0
        jne    SHORT loc_2_0673
        cmp    byte ptr [esi + 53h], 0
        jne    SHORT loc_2_0673
        push   6
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0673:
        mov    eax, dword ptr [esi + 0aeh]
        pop    edi
        pop    esi
        pop    ebx
        mov    byte ptr [eax + 1e0fh], 0ffh
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_2_0690:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 0aeh]
        mov    byte ptr [eax + 1e08h], 0c0h
        mov    eax, dword ptr [ecx + 0aeh]
        mov    byte ptr [eax + 1e0fh], 1
        mov    eax, dword ptr [ecx + 0aeh]
        mov    byte ptr [eax + 1e0ch], 0
        cmp    byte ptr [ecx + 39h], 0
        je     SHORT loc_2_06D3
        mov    dl, byte ptr [ecx + 48h]
        mov    eax, dword ptr [ecx + 0aeh]
        mov    byte ptr [eax + 1e00h], dl
loc_2_06D3:
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_06E0:
        push   ebp
        mov    eax, 8
        mov    ebp, esp
        sub    esp, 48h
        mov    word ptr [ebp - 48h], ax
        push   ebx
        mov    word ptr [ebp - 46h], ax
        push   esi
        push   edi
        mov    eax, 7
        db  0C7h, 045h, 0BCh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0C4h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0CCh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 40h], ax
        mov    word ptr [ebp - 3eh], ax
        mov    eax, 0eh
        mov    word ptr [ebp - 38h], ax
        xor    ebx, ebx
        mov    word ptr [ebp - 36h], ax
        mov    dword ptr [ebp - 0ch], ebx
        mov    eax, 12h
        mov    byte ptr [ebp - 1], bl
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 2], 1
        mov    word ptr [ebp - 30h], ax
        lea    ecx, [ebp - 8]
        mov    word ptr [ebp - 2eh], ax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   ebx
        push   379h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_075A:
        test   eax, eax
        je     SHORT loc_2_076C
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_076C:
        mov    edi, dword ptr [ebp - 8]
        xor    eax, eax
        mov    ecx, 0deh
        rep stosd
        stosb
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 0e0h], 4628h
        mov    eax, dword ptr [ebp - 8]
        push   esi
        mov    dword ptr [eax + 15ch], 2
        mov    edx, dword ptr [ecx + 4]
        mov    eax, dword ptr [ebp - 8]
        lea    ecx, [ebp - 14h]
        mov    dword ptr [eax + 7eh], edx
        push   ecx
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_07B0:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_2_07D4
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   379h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_07C6:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_07D4:
        push   1
        lea    eax, [ebp - 48h]
        push   eax
        mov    ecx, dword ptr [ebp - 14h]
        push   ecx
        lea    edx, [ebp - 10h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_07EB:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_07FE
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 4], 3
        jne    SHORT loc_2_07FE
        mov    byte ptr [ebp - 1], 1
loc_2_07FE:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_2_0832
        lea    eax, [ebp - 28h]
        lea    ecx, [ebp - 20h]
        push   eax
        lea    edx, [ebp - 0ch]
        push   ecx
        push   esi
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0816:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_2_0823
        mov    bl, 1
        jmp    NEAR PTR loc_2_093C
loc_2_0823:
        mov    al, byte ptr [ebp - 25h]
        and    eax, 1
        je     SHORT loc_2_0866
        cmp    eax, 1
        je     SHORT loc_2_0879
        jmp    SHORT loc_2_088A
loc_2_0832:
        push   0
        lea    eax, [ebp - 40h]
        push   eax
        mov    ecx, dword ptr [ebp - 14h]
        push   ecx
        lea    edx, [ebp - 10h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0849:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_0860
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 4]
        dec    eax
        cmp    eax, 1
        sbb    al, al
        neg    al
        mov    byte ptr [ebp - 2], al
loc_2_0860:
        cmp    byte ptr [ebp - 2], 0
        je     SHORT loc_2_0879
loc_2_0866:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 5ah], 0a20h
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 38h], 1
        jmp    SHORT loc_2_088A
loc_2_0879:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 5ah], 0a24h
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 38h], 2
loc_2_088A:
        push   0
        lea    eax, [ebp - 38h]
        push   eax
        mov    ecx, dword ptr [ebp - 14h]
        push   ecx
        lea    edx, [ebp - 10h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_08A1:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_08B6
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0e0h], edx
loc_2_08B6:
        mov    eax, dword ptr [ebp - 14h]
        lea    ecx, [ebp - 18h]
        push   eax
        lea    edx, [ebp - 1ch]
        push   ecx
        lea    eax, [ebp - 0ch]
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_08CB:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_0908
        cmp    dword ptr [ebp - 18h], 6
        jne    NEAR PTR loc_2_097B
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 1ch]
        add    eax, 4eh
        mov    edx, ecx
        mov    dl, byte ptr [ecx]
        mov    byte ptr [eax], dl
        mov    dl, byte ptr [ecx + 1]
        mov    byte ptr [eax + 1], dl
        mov    dl, byte ptr [ecx + 2]
        mov    byte ptr [eax + 2], dl
        mov    dl, byte ptr [ecx + 3]
        mov    byte ptr [eax + 3], dl
        mov    dl, byte ptr [ecx + 4]
        mov    byte ptr [eax + 4], dl
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [eax + 5], cl
loc_2_0908:
        push   0
        lea    eax, [ebp - 30h]
        push   eax
        mov    ecx, dword ptr [ebp - 14h]
        push   ecx
        lea    edx, [ebp - 10h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_091F:
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 2bh], 1
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_093C
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 4], 0
        jne    SHORT loc_2_093C
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 2bh], 0
loc_2_093C:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0945:
        push   ebx
        mov    al, byte ptr [ebp - 1]
        push   eax
        mov    ecx, dword ptr [ebp + 10h]
        push   ecx
        mov    edx, dword ptr [ebp - 8]
        push   esi
        push   edx
        call   sub_2_0110
        mov    dword ptr [ebp - 0ch], eax
        test   eax, eax
        je     SHORT loc_2_096F
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   379h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_096F:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_097B:
        cmp    dword ptr [ebp - 18h], 0
        je     SHORT loc_2_0908
        mov    bl, 1
        jmp    SHORT loc_2_093C
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0990:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    edx, dword ptr [edi + 5ah]
        in     al, dx
        add    edx, 2
        mov    byte ptr [ebp - 4], al
        in     al, dx
        mov    byte ptr [ebp - 5], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 0fch
        mov    cl, byte ptr [ebp - 5]
        shl    eax, 0bh
        and    ecx, 1
        shl    ecx, 13h
        push   2000h
        or     eax, ecx
        mov    edx, dword ptr [edi + 82h]
        push   eax
        lea    ecx, [edi + 0aeh]
        push   edx
        lea    eax, [ebp - 0ch]
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_09DD:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_2_0A04
        push   0
        mov    eax, dword ptr [edi + 82h]
        push   0aa000001h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_09F6:
        add    esp, 0ch
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0A04:
        mov    edx, dword ptr [edi + 0aeh]
        lea    ecx, [ebp - 18h]
loc_2_0A0D:
        xor    esi, esi
        mov    dword ptr [ecx], 0
loc_2_0A15:
        mov    al, byte ptr [edx + esi + 1f30h]
        mov    ebx, dword ptr [ecx]
        shl    ebx, 4
        and    eax, 0fh
        add    eax, ebx
        add    esi, 2
        mov    dword ptr [ecx], eax
        cmp    esi, 10h
        jb     SHORT loc_2_0A15
        add    edx, 10h
        add    ecx, 4
        lea    eax, [ebp - 0ch]
        cmp    ecx, eax
        jb     SHORT loc_2_0A0D
        mov    eax, dword ptr [ebp - 18h]
        db  039h, 005h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_0A66
        mov    eax, dword ptr [ebp - 14h]
        db  039h, 005h, 004h, 000h, 000h, 000h
        jne    SHORT loc_2_0A66
        mov    eax, dword ptr [ebp - 10h]
        db  039h, 005h, 008h, 000h, 000h, 000h
        jne    SHORT loc_2_0A66
        lea    eax, [edi + 39h]
        mov    byte ptr [eax], 1
        jmp    SHORT loc_2_0A99
loc_2_0A66:
        mov    eax, dword ptr [ebp - 18h]
        db  039h, 005h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_0D93
        mov    eax, dword ptr [ebp - 14h]
        db  039h, 005h, 004h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_0D93
        mov    eax, dword ptr [ebp - 10h]
        db  039h, 005h, 008h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_0D93
        lea    eax, [edi + 39h]
        mov    byte ptr [eax], 0
loc_2_0A99:
        mov    edx, dword ptr [edi + 0aeh]
        mov    cl, byte ptr [edx + 1e01h]
        and    cl, 0ch
        shr    cl, 2
        cmp    byte ptr [eax], 0
        je     NEAR PTR loc_2_0BCC
        mov    al, byte ptr [edx + 1e00h]
        mov    byte ptr [edi + 48h], al
        test   al, al
        jne    SHORT loc_2_0AD0
        mov    al, byte ptr [ebp - 4]
        and    al, 0fdh
        shr    al, 1
        or     al, 80h
        add    al, 2
        mov    byte ptr [edi + 48h], al
loc_2_0AD0:
        xor    eax, eax
        mov    al, cl
        cmp    eax, 3
        ja     SHORT loc_2_0B07
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_2_0AE0:
        mov    al, 4
        cmp    cl, 1
        sbb    esi, esi
        and    esi, 0ffffe000h
        add    esi, 4000h
        jmp    SHORT loc_2_0B0D
loc_2_0AF5:
        mov    esi, 8000h
        mov    al, 8
        jmp    SHORT loc_2_0B0D
loc_2_0AFE:
        mov    esi, 10000h
        mov    al, 10h
        jmp    SHORT loc_2_0B0D
loc_2_0B07:
        mov    esi, dword ptr [ebp - 4]
        mov    al, byte ptr [ebp - 4]
loc_2_0B0D:
        mov    dl, byte ptr [edi + 48h]
        xor    ecx, ecx
        mov    cl, al
        xor    ebx, ebx
        dec    ecx
        mov    bl, dl
        test   ebx, ecx
        je     SHORT loc_2_0B29
        lea    ecx, [eax - 1]
        not    cl
        and    cl, dl
        add    cl, al
        mov    byte ptr [edi + 48h], cl
loc_2_0B29:
        xor    eax, eax
        push   esi
        mov    al, byte ptr [edi + 48h]
        mov    ecx, dword ptr [edi + 82h]
        shl    eax, 0ch
        lea    edx, [edi + 0b2h]
        push   eax
        mov    dword ptr [edi + 40h], esi
        push   ecx
        lea    eax, [ebp - 0ch]
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0B4D:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_2_0B74
        push   0
        mov    eax, dword ptr [edi + 82h]
        push   0aa000001h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0B66:
        add    esp, 0ch
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0B74:
        mov    eax, dword ptr [edi + 0aeh]
        mov    cl, byte ptr [ebp - 4]
        and    ecx, 3
        mov    al, byte ptr [eax + 1fbah]
        cmp    ecx, 3
        ja     NEAR PTR loc_2_0C77
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_2_0B96:
        mov    eax, 2
        jmp    NEAR PTR loc_2_0C7A
loc_2_0BA0:
        mov    eax, 3
        jmp    NEAR PTR loc_2_0C7A
loc_2_0BAA:
        sub    al, 0fh
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0fffffffch
        add    eax, 0ah
        jmp    NEAR PTR loc_2_0C7A
loc_2_0BBB:
        sub    al, 0fh
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0fffffffch
        add    eax, 0bh
        jmp    NEAR PTR loc_2_0C7A
loc_2_0BCC:
        xor    eax, eax
        mov    al, cl
        cmp    eax, 3
        ja     SHORT loc_2_0BF8
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_2_0BDC:
        mov    esi, 2000h
        jmp    SHORT loc_2_0BFB
loc_2_0BE3:
        mov    esi, 4000h
        jmp    SHORT loc_2_0BFB
loc_2_0BEA:
        mov    esi, 8000h
        jmp    SHORT loc_2_0BFB
loc_2_0BF1:
        mov    esi, 10000h
        jmp    SHORT loc_2_0BFB
loc_2_0BF8:
        mov    esi, dword ptr [ebp - 4]
loc_2_0BFB:
        mov    al, byte ptr [ebp - 5]
        push   esi
        and    eax, 0feh
        mov    ecx, dword ptr [edi + 82h]
        shl    eax, 0ch
        lea    edx, [edi + 0b2h]
        push   eax
        push   ecx
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0C1F:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_2_0C46
        push   0
        mov    eax, dword ptr [edi + 82h]
        push   0aa000001h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0C38:
        add    esp, 0ch
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0C46:
        mov    al, byte ptr [ebp - 4]
        mov    dword ptr [edi + 40h], esi
        and    eax, 3
        cmp    eax, 3
        ja     SHORT loc_2_0C77
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_2_0C5B:
        mov    eax, 2
        jmp    SHORT loc_2_0C7A
loc_2_0C62:
        mov    eax, 3
        jmp    SHORT loc_2_0C7A
loc_2_0C69:
        mov    eax, 0ah
        jmp    SHORT loc_2_0C7A
loc_2_0C70:
        mov    eax, 0bh
        jmp    SHORT loc_2_0C7A
loc_2_0C77:
        mov    eax, dword ptr [ebp - 4]
loc_2_0C7A:
        mov    edx, dword ptr [edi + 0aeh]
        mov    dword ptr [edi + 31h], eax
        mov    byte ptr [edi + 3ah], 0
        mov    cl, byte ptr [edx + 1fa6h]
        and    ecx, 0fh
        lea    eax, [ecx - 0ah]
        cmp    eax, 5
        ja     SHORT loc_2_0C9F
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_2_0C9F:
        push   ecx
        mov    eax, dword ptr [edi + 82h]
        push   6
        push   2
        push   3
        push   0aa00000ah
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0CB7:
        add    esp, 18h
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0CC5:
        mov    dword ptr [edi + 3ch], 10000h
        jmp    SHORT loc_2_0CF9
loc_2_0CCE:
        mov    dword ptr [edi + 3ch], 10000h
        mov    byte ptr [edi + 3ah], 1
        jmp    SHORT loc_2_0CF9
loc_2_0CDB:
        mov    dword ptr [edi + 3ch], 8000h
        jmp    SHORT loc_2_0CF9
loc_2_0CE4:
        mov    dword ptr [edi + 3ch], 4000h
        jmp    SHORT loc_2_0CF9
loc_2_0CED:
        mov    dword ptr [edi + 3ch], 2000h
        jmp    SHORT loc_2_0CF9
loc_2_0CF6:
        mov    dword ptr [edi + 3ch], esi
loc_2_0CF9:
        mov    byte ptr [edi + 3bh], 0
        mov    al, byte ptr [edx + 1faah]
        and    eax, 0fh
        cmp    eax, 0dh
        je     SHORT loc_2_0D18
        cmp    eax, 0eh
        je     SHORT loc_2_0D20
        mov    word ptr [edi + 44h], 800h
        jmp    SHORT loc_2_0D26
loc_2_0D18:
        mov    word ptr [edi + 44h], 1170h
        jmp    SHORT loc_2_0D26
loc_2_0D20:
        mov    word ptr [edi + 44h], 1000h
loc_2_0D26:
        mov    al, byte ptr [edx + 1fach]
        and    eax, 0fh
        sub    eax, 0bh
        cmp    eax, 3
        ja     SHORT loc_2_0D3E
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_2_0D3E:
        mov    word ptr [edi + 46h], 800h
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0D4F:
        mov    word ptr [edi + 46h], 4628h
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0D60:
        mov    word ptr [edi + 46h], 4000h
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0D71:
        mov    word ptr [edi + 46h], 2000h
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0D82:
        mov    word ptr [edi + 46h], 1000h
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0D93:
        push   0
        mov    eax, dword ptr [edi + 82h]
        push   0aa00000dh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0DA6:
        add    esp, 0ch
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0E20:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   edi
        db  0A1h, 0E4h, 041h, 000h, 000h
        push   eax
        push   0
        lea    eax, [ebp - 4]
        push   32h
        push   eax
        db  0E8h, 00Ch, 031h, 0FEh, 0FFh
        mov    ecx, dword ptr [ebp + 20h]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     SHORT loc_2_0E50
        sub    al, al
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  090h
loc_2_0E50:
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
sub_2_0EF0:
        push   ebp
        mov    ebp, esp
        push   0
        push   32h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 04Ch, 030h, 0FEh, 0FFh
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0F10:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 2eh], 0
        jne    SHORT loc_2_0F29
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_0F29:
        db  0A1h, 0E4h, 041h, 000h, 000h
        push   eax
        push   0
        push   1fh
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 00Ah, 030h, 0FEh, 0FFh
        or     eax, eax
        je     SHORT loc_2_0F49
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_0F49:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 2eh]
        mov    edi, edi
loc_2_0F50:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    SHORT loc_2_0F63
        inc    ecx
        cmp    ecx, 20h
        jb     SHORT loc_2_0F50
        jmp    SHORT loc_2_0F72
loc_2_0F63:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 2eh], eax
loc_2_0F72:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 14h]
        mov    dword ptr [eax + 10h], ecx
        mov    eax, dword ptr [esi + 14h]
        or     eax, eax
        je     SHORT loc_2_0F88
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], ecx
loc_2_0F88:
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
sub_2_0FF0:
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
        call   sub_2_1310
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_2_1019
        cmp    ebx, 103h
        jne    SHORT loc_2_1031
loc_2_1019:
        push   0
        db  068h, 0E0h, 041h, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   edi
        push   esi
        call   sub_2_10E0
        or     eax, eax
        je     SHORT loc_2_1031
        mov    ebx, eax
loc_2_1031:
        or     ebx, ebx
        je     SHORT loc_2_103D
        cmp    ebx, 103h
        jne    SHORT loc_2_1068
loc_2_103D:
        cmp    byte ptr [edi + 1dh], 0
        je     SHORT loc_2_1068
        dec    dword ptr [esi + 0ch]
        mov    byte ptr [edi + 1dh], 0
        cmp    dword ptr [esi + 0ch], 0
        jne    SHORT loc_2_1068
        push   0
        db  068h, 0E0h, 041h, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   edi
        push   esi
        call   sub_2_1180
        or     eax, eax
        je     SHORT loc_2_1068
        mov    ebx, eax
loc_2_1068:
        or     ebx, ebx
        je     SHORT loc_2_1074
        cmp    ebx, 103h
        jne    SHORT loc_2_10D5
loc_2_1074:
        mov    eax, dword ptr [edi + 18h]
        dec    eax
        mov    dword ptr [edi + 18h], eax
        jne    SHORT loc_2_10D0
        mov    eax, dword ptr [edi + 10h]
        or     eax, eax
        je     SHORT loc_2_108A
        mov    ecx, dword ptr [edi + 14h]
        mov    dword ptr [eax + 14h], ecx
loc_2_108A:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_2_1099
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_2_109F
loc_2_1099:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 14h], eax
loc_2_109F:
        cmp    byte ptr [edi + 1eh], 0
        je     SHORT loc_2_10AE
        mov    eax, dword ptr [edi + 4]
        push   eax
        db  0E8h, 0FAh, 02Dh, 0FEh, 0FFh
loc_2_10AE:
        mov    eax, 1
        mov    cl, byte ptr [edi + 1ch]
        shl    eax, cl
        or     dword ptr [esi + 2eh], eax
        push   0
        push   1fh
        push   edi
        db  0E8h, 087h, 02Eh, 0FEh, 0FFh
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_2_10D0:
        mov    ebx, 0c001000eh
loc_2_10D5:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_2_10E0:
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
        je     SHORT loc_2_1140
loc_2_1133:
        mov    ecx, dword ptr [eax + 0ch]
        or     dword ptr [edi + 4], ecx
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_2_1133
loc_2_1140:
        mov    ecx, dword ptr [edi + 4]
        cmp    esi, ecx
        je     SHORT loc_2_1174
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
        je     SHORT loc_2_1176
        cmp    eax, 103h
        je     SHORT loc_2_1176
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 0ch], ecx
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_1174:
        sub    eax, eax
loc_2_1176:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  090h
sub_2_1180:
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
        db  0A0h, 0E0h, 041h, 000h, 000h
        cmp    byte ptr [ecx], al
        jne    SHORT loc_2_1228
        db  0A0h, 0E1h, 041h, 000h, 000h
        cmp    byte ptr [ecx + 1], al
        jne    SHORT loc_2_1228
        db  0A0h, 0E2h, 041h, 000h, 000h
        cmp    byte ptr [ecx + 2], al
        jne    SHORT loc_2_1228
        db  0A0h, 0E3h, 041h, 000h, 000h
        cmp    byte ptr [ecx + 3], al
        jne    SHORT loc_2_1228
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    byte ptr [ebx + 1dh], 0
        je     SHORT loc_2_1209
        dec    dword ptr [esi + 0ch]
        mov    byte ptr [ebx + 1dh], 0
        cmp    dword ptr [esi + 0ch], 0
        je     NEAR PTR loc_2_12A3
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_1209:
        cmp    dword ptr [esi + 0ch], 0
        je     SHORT loc_2_121D
        mov    eax, 0c001001ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_121D:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_1228:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 8], eax
        jne    SHORT loc_2_1260
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    byte ptr [ebx + 1dh], 0
        je     SHORT loc_2_1244
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_1244:
        cmp    dword ptr [esi + 0ch], 0
        je     SHORT loc_2_12A3
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
loc_2_1260:
        mov    eax, dword ptr [esi + 0ch]
        cmp    eax, 1
        jbe    SHORT loc_2_1276
        mov    eax, 0c001001ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_1276:
        mov    ebx, dword ptr [ebp + 0ch]
        jne    SHORT loc_2_1290
        cmp    byte ptr [ebx + 1dh], 0
        jne    SHORT loc_2_1290
        mov    eax, 0c001001ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  090h
loc_2_1290:
        cmp    eax, 1
        jne    SHORT loc_2_12A3
        cmp    byte ptr [ebx + 1dh], 0
        je     SHORT loc_2_12A3
        dec    eax
        mov    dword ptr [esi + 0ch], eax
        mov    byte ptr [ebx + 1dh], 0
loc_2_12A3:
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
        je     SHORT loc_2_12D6
        cmp    eax, 103h
        je     SHORT loc_2_12D6
        mov    dword ptr [esi + 8], edi
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0ch], ecx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_12D6:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_2_12F0
        mov    byte ptr [ebx + 1dh], 0
        mov    dword ptr [esi + 0ch], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_12F0:
        mov    byte ptr [ebx + 1dh], 1
        mov    dword ptr [esi + 0ch], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1310:
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
        je     SHORT loc_2_134D
        lea    esp, [esp]
loc_2_1340:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 10h], ecx
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_2_1340
loc_2_134D:
        mov    ecx, dword ptr [esi + 10h]
        cmp    ebx, ecx
        je     SHORT loc_2_1381
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
        je     SHORT loc_2_1383
        cmp    eax, 103h
        je     SHORT loc_2_1383
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 10h], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_1381:
        sub    eax, eax
loc_2_1383:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h
_PTEXT ends
        end
