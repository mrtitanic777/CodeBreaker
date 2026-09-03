; irmatrac_pcode_02.asm -- IRMATRAC object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 5ch
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 1ch], 9
        mov    word ptr [ebp - 1ah], 9
        db  0C7h, 045h, 0E8h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 14h], 0
        mov    word ptr [ebp - 12h], 2
        mov    word ptr [ebp - 10h], 5c1ch
        mov    word ptr [ebp - 0eh], 5c1dh
        lea    eax, [ebp - 14h]
        mov    dword ptr [ebp + 8], eax
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_004D:
        mov    dword ptr [ebp - 5ch], 0ffffffffh
        mov    eax, dword ptr [ebp - 5ch]
        push   eax
        push   0
        push   8
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0065:
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_007C
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_2_0124
loc_2_007C:
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    byte ptr [ebp - 58h], 3
        mov    byte ptr [ebp - 57h], 0
        db  0C7h, 045h, 0AEh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0B2h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0B6h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0BAh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0BEh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0C2h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0C6h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0CAh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0CEh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0D2h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp - 18h]
        lea    edx, [ebp - 2ah]
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], ecx
        push   36h
        lea    eax, [ebp - 58h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 4
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_00FE:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_2_010F
        xor    eax, eax
        jmp    NEAR PTR loc_2_0124
loc_2_010F:
        push   0
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_011A:
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_2_0124
loc_2_0124:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_2_012B:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        lea    edi, [ebp - 2ch]
        xor    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 243h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_2_0164
        mov    dword ptr [ebp - 1eh], 1
        jmp    NEAR PTR loc_2_016B
loc_2_0164:
        mov    dword ptr [ebp - 1eh], 0
loc_2_016B:
        db  0C7h, 045h, 0DAh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 244h]
        and    eax, 30h
        cmp    al, 30h
        jne    NEAR PTR loc_2_0192
        mov    dword ptr [ebp - 22h], 2
        jmp    NEAR PTR loc_2_01BB
loc_2_0192:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 244h]
        and    eax, 0ffh
        test   al, 10h
        je     NEAR PTR loc_2_01B4
        mov    dword ptr [ebp - 22h], 3
        jmp    NEAR PTR loc_2_01BB
loc_2_01B4:
        mov    dword ptr [ebp - 22h], 1
loc_2_01BB:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 243h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 2ch], eax
        cmp    dword ptr [ebp - 2ch], 0
        je     NEAR PTR loc_2_0211
        cmp    dword ptr [ebp - 22h], 2
        jne    NEAR PTR loc_2_01EE
        mov    dword ptr [ebp - 2ch], 0
        mov    dword ptr [ebp - 1eh], 0
loc_2_01EE:
        mov    byte ptr [ebp - 28h], 1
        mov    eax, dword ptr [ebp - 1eh]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0a9h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 245h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 1ah], eax
loc_2_0211:
        mov    dword ptr [ebp - 16h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 22fh]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 12h], eax
        mov    dword ptr [ebp - 0eh], 20h
        lea    eax, [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 7dh]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 81h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0256:
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_026B
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_04A1
loc_2_026B:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0abh]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0adh], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0288:
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_2_0462
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_029E:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 244h]
        and    eax, 10h
        cmp    eax, 1
        sbb    eax, eax
        add    eax, 2
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 18h]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 18h]
        and    eax, 0ffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 81h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_02F2:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_0330
        push   1
        push   0
        push   2
        push   0aa000005h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 81h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0316:
        add    esp, 14h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 81h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0328:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_04A1
loc_2_0330:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 3eh], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 69h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 6dh], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 6dh]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 69h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0a7h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 200h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0afh]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 202h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 59h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1ffh], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 214h], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0399:
        test   eax, eax
        je     NEAR PTR loc_2_03CC
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_03AA:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_03B3:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 81h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_03C2:
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_2_04A1
loc_2_03CC:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 85h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 79h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 45h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_0920
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_2_0456
        push   2
        push   0
        push   2
        push   0aa000002h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 81h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0423:
        add    esp, 14h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_042F:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0438:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 81h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0447:
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_2_04A1
        db  0E9h, 007h, 000h, 000h, 000h
loc_2_0456:
        xor    eax, eax
        jmp    NEAR PTR loc_2_04A1
        db  0E9h, 03Fh, 000h, 000h, 000h
loc_2_0462:
        push   3
        push   0
        push   2
        push   0aa000002h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 81h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_047C:
        add    esp, 14h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0488:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 81h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0497:
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_2_04A1
loc_2_04A1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
loc_2_04A8:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 14h
        xor    ecx, ecx
        push   ebx
        mov    dword ptr [ebp - 0ch], 0
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 81h], eax
        mov    cx, word ptr [esi + 22fh]
        lea    edx, [ecx + 11h]
        in     al, dx
        cmp    al, 5ch
        je     SHORT loc_2_04E4
        mov    eax, 0aa000004h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_04E4:
        lea    edx, [ecx + 10h]
        in     al, dx
        cmp    al, 1dh
        jne    SHORT loc_2_056B
        or     byte ptr [esi + 244h], 8
loc_2_04F3:
        xor    edx, edx
        mov    dx, word ptr [esi + 22fh]
        add    edx, 13h
        in     al, dx
        test   al, 80h
        jne    NEAR PTR loc_2_0623
        xor    edi, edi
        and    byte ptr [esi + 244h], 0cfh
        mov    di, word ptr [esi + 22fh]
        lea    edx, [edi + 12h]
        in     al, dx
        mov    cl, al
        and    al, 41h
        cmp    al, 41h
        jne    SHORT loc_2_0529
        mov    al, cl
        and    al, 0bfh
        out    dx, al
loc_2_0529:
        and    cl, 0feh
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 13h]
        in     al, dx
        test   al, 40h
        jne    SHORT loc_2_053E
        mov    byte ptr [esi + 243h], 0
loc_2_053E:
        mov    cl, byte ptr [esi + 244h]
        mov    dl, byte ptr [esi + 18h]
        and    cl, 8
        jne    SHORT loc_2_0592
        cmp    dl, 7
        ja     SHORT loc_2_0584
        cmp    dl, 2
        jb     SHORT loc_2_0584
        cmp    dl, 4
        je     SHORT loc_2_0584
        and    al, 0fch
        or     al, 4
        cmp    dl, 3
        jbe    SHORT loc_2_057D
        shr    dl, 1
        or     al, dl
        jmp    SHORT loc_2_05C4
loc_2_056B:
        cmp    al, 1ch
        je     SHORT loc_2_04F3
        mov    eax, 0aa000004h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_057D:
        sub    dl, 2
        or     al, dl
        jmp    SHORT loc_2_05C4
loc_2_0584:
        mov    eax, 0aa00000ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_0592:
        cmp    dl, 0bh
        ja     NEAR PTR loc_2_0770
        cmp    dl, 2
        jb     NEAR PTR loc_2_0770
        cmp    dl, 9
        je     NEAR PTR loc_2_0770
        cmp    dl, 8
        je     NEAR PTR loc_2_0770
        and    al, 0f8h
        and    edx, 0ffh
        db  00Ah, 082h, 000h, 000h, 000h, 000h
loc_2_05C4:
        xor    edi, edi
        mov    bl, byte ptr [esi + 243h]
        mov    di, word ptr [esi + 22fh]
        test   bl, bl
        lea    edx, [edi + 13h]
        out    dx, al
        je     SHORT loc_2_0600
        test   cl, cl
        jne    NEAR PTR loc_2_073D
        cmp    bl, 5
        jb     NEAR PTR loc_2_072F
        cmp    bl, 6
        ja     NEAR PTR loc_2_072F
        lea    edx, [edi + 14h]
        mov    al, bl
        shl    al, 6
        or     al, 1ch
loc_2_05FF:
        out    dx, al
loc_2_0600:
        cmp    bl, 1
        sbb    al, al
        and    al, 0c0h
        add    al, 79h
        test   byte ptr [esi + 244h], 1
        jne    SHORT loc_2_0614
        or     al, 2
loc_2_0614:
        test   byte ptr [esi + 244h], 4
        je     SHORT loc_2_061F
        or     al, 4
loc_2_061F:
        lea    edx, [edi + 12h]
        out    dx, al
loc_2_0623:
        mov    ax, word ptr [esi + 22fh]
        mov    word ptr [esi + 231h], ax
        mov    ax, word ptr [esi + 22fh]
        add    ax, 2
        mov    word ptr [esi + 233h], ax
        mov    ax, word ptr [esi + 22fh]
        add    ax, 4
        mov    word ptr [esi + 235h], ax
        mov    ax, word ptr [esi + 22fh]
        add    ax, 6
        mov    word ptr [esi + 237h], ax
        mov    ax, word ptr [esi + 22fh]
        add    ax, 8
        mov    word ptr [esi + 239h], ax
        mov    ax, word ptr [esi + 22fh]
        add    ax, 9
        mov    word ptr [esi + 23bh], ax
        mov    ax, word ptr [esi + 22fh]
        add    ax, 0ah
        mov    word ptr [esi + 23dh], ax
        push   esi
        mov    ax, word ptr [esi + 22fh]
        add    ax, 0ch
        mov    word ptr [esi + 23fh], ax
        mov    ax, word ptr [esi + 22fh]
        add    ax, 0eh
        mov    word ptr [esi + 241h], ax
        call   sub_2_08C8
        test   al, al
        jne    NEAR PTR loc_2_077E
        mov    ax, word ptr [esi + 22fh]
        add    ax, 4
        mov    word ptr [esi + 233h], ax
        mov    ax, word ptr [esi + 22fh]
        add    ax, 2
        mov    word ptr [esi + 235h], ax
        push   esi
        mov    ax, word ptr [esi + 22fh]
        add    ax, 0ch
        mov    word ptr [esi + 23dh], ax
        mov    ax, word ptr [esi + 22fh]
        add    ax, 0ah
        mov    word ptr [esi + 23fh], ax
        call   sub_2_08C8
        test   al, al
        jne    SHORT loc_2_077E
        mov    eax, 0aa000004h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_072F:
        mov    eax, 0aa00000ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_073D:
        cmp    bl, 3
        jb     SHORT loc_2_0762
        cmp    bl, 7
        ja     SHORT loc_2_0762
        cmp    bl, 4
        je     SHORT loc_2_0762
        mov    al, bl
        and    al, 3
        shl    al, 4
        cmp    bl, 3
        jne    SHORT loc_2_075A
        xor    al, al
loc_2_075A:
        lea    edx, [edi + 14h]
        jmp    NEAR PTR loc_2_05FF
loc_2_0762:
        mov    eax, 0aa00000ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_0770:
        mov    eax, 0aa00000ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_077E:
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 14h]
        push   eax
        lea    edx, [ebp - 10h]
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 0ch]
        push   ecx
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_079A:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_2_07AE
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_07AE:
        mov    eax, dword ptr [ebp - 10h]
        lea    ecx, [ebp - 8]
        push   eax
        lea    edx, [ebp - 0ch]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_07BF:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_2_07D3
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_07D3:
        mov    dx, word ptr [esi + 239h]
        mov    al, 80h
        out    dx, al
        push   0ah
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_07E4:
        mov    cx, word ptr [esi + 239h]
        mov    dx, cx
        in     ax, dx
        mov    word ptr [ebp - 2], ax
        and    word ptr [ebp - 2], 7fh
        cmp    byte ptr [esi + 243h], 0
        jne    SHORT loc_2_0806
        or     byte ptr [ebp - 1], 1
loc_2_0806:
        mov    dx, cx
        mov    ax, word ptr [ebp - 2]
        out    dx, ax
        mov    eax, 80h
        out    dx, ax
        mov    eax, 6ah
        out    dx, ax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        add    dword ptr [ebp - 8], 2
        test   ax, ax
        je     SHORT loc_2_0888
        mov    edi, eax
        mov    ecx, 2
        and    edi, 0ffffh
loc_2_0839:
        mov    eax, dword ptr [ebp - 8]
        mov    bx, word ptr [eax]
        shr    bx, 1
        add    dword ptr [ebp - 8], ecx
        mov    dx, word ptr [esi + 23fh]
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        out    dx, ax
        add    dword ptr [ebp - 8], ecx
        mov    dx, word ptr [esi + 235h]
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        out    dx, ax
        add    dword ptr [ebp - 8], ecx
        test   bx, bx
        je     SHORT loc_2_0885
loc_2_086F:
        mov    dx, word ptr [esi + 233h]
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        out    dx, ax
        add    dword ptr [ebp - 8], ecx
        dec    bx
        jne    SHORT loc_2_086F
loc_2_0885:
        dec    edi
        jne    SHORT loc_2_0839
loc_2_0888:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0891:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_089A:
        mov    cx, word ptr [ebp - 2]
        pop    edi
        and    cx, 0ff2ah
        mov    dx, word ptr [esi + 239h]
        or     cl, 2ah
        mov    ax, cx
        pop    esi
        out    dx, ax
        mov    eax, dword ptr [ebp - 0ch]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_08C8:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        xor    esi, esi
        mov    si, word ptr [ecx + 235h]
        mov    cx, word ptr [ecx + 23dh]
        inc    esi
        mov    edx, esi
        and    ecx, 0ffffh
        in     al, dx
        mov    bl, al
        inc    ecx
        mov    edx, ecx
        in     al, dx
        cmp    al, bl
        jne    SHORT loc_2_090C
        mov    edx, esi
        mov    al, 80h
        out    dx, al
        in     al, dx
        mov    bl, al
        mov    edx, ecx
        in     al, dx
        cmp    al, bl
        jne    SHORT loc_2_090C
        mov    al, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_2_090C:
        xor    al, al
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0920:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   edi
        db  0A1h, 0B4h, 05Dh, 000h, 000h
        push   eax
        push   0
        lea    eax, [ebp - 4]
        push   32h
        push   eax
        db  0E8h, 09Ch, 04Fh, 0FEh, 0FFh
        mov    ecx, dword ptr [ebp + 20h]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     SHORT loc_2_0950
        sub    al, al
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  090h
loc_2_0950:
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
sub_2_09F0:
        push   ebp
        mov    ebp, esp
        push   0
        push   32h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 0DCh, 04Eh, 0FEh, 0FFh
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0A10:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 2eh], 0
        jne    SHORT loc_2_0A29
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_0A29:
        db  0A1h, 0B4h, 05Dh, 000h, 000h
        push   eax
        push   0
        push   1fh
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 09Ah, 04Eh, 0FEh, 0FFh
        or     eax, eax
        je     SHORT loc_2_0A49
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_0A49:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 2eh]
        mov    edi, edi
loc_2_0A50:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    SHORT loc_2_0A63
        inc    ecx
        cmp    ecx, 20h
        jb     SHORT loc_2_0A50
        jmp    SHORT loc_2_0A72
loc_2_0A63:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 2eh], eax
loc_2_0A72:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 14h]
        mov    dword ptr [eax + 10h], ecx
        mov    eax, dword ptr [esi + 14h]
        or     eax, eax
        je     SHORT loc_2_0A88
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], ecx
loc_2_0A88:
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
sub_2_0AF0:
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
        call   sub_2_0E10
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_2_0B19
        cmp    ebx, 103h
        jne    SHORT loc_2_0B31
loc_2_0B19:
        push   0
        db  068h, 0B0h, 05Dh, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   edi
        push   esi
        call   sub_2_0BE0
        or     eax, eax
        je     SHORT loc_2_0B31
        mov    ebx, eax
loc_2_0B31:
        or     ebx, ebx
        je     SHORT loc_2_0B3D
        cmp    ebx, 103h
        jne    SHORT loc_2_0B68
loc_2_0B3D:
        cmp    byte ptr [edi + 1dh], 0
        je     SHORT loc_2_0B68
        dec    dword ptr [esi + 0ch]
        mov    byte ptr [edi + 1dh], 0
        cmp    dword ptr [esi + 0ch], 0
        jne    SHORT loc_2_0B68
        push   0
        db  068h, 0B0h, 05Dh, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   edi
        push   esi
        call   sub_2_0C80
        or     eax, eax
        je     SHORT loc_2_0B68
        mov    ebx, eax
loc_2_0B68:
        or     ebx, ebx
        je     SHORT loc_2_0B74
        cmp    ebx, 103h
        jne    SHORT loc_2_0BD5
loc_2_0B74:
        mov    eax, dword ptr [edi + 18h]
        dec    eax
        mov    dword ptr [edi + 18h], eax
        jne    SHORT loc_2_0BD0
        mov    eax, dword ptr [edi + 10h]
        or     eax, eax
        je     SHORT loc_2_0B8A
        mov    ecx, dword ptr [edi + 14h]
        mov    dword ptr [eax + 14h], ecx
loc_2_0B8A:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_2_0B99
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_2_0B9F
loc_2_0B99:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 14h], eax
loc_2_0B9F:
        cmp    byte ptr [edi + 1eh], 0
        je     SHORT loc_2_0BAE
        mov    eax, dword ptr [edi + 4]
        push   eax
        db  0E8h, 08Ah, 04Ch, 0FEh, 0FFh
loc_2_0BAE:
        mov    eax, 1
        mov    cl, byte ptr [edi + 1ch]
        shl    eax, cl
        or     dword ptr [esi + 2eh], eax
        push   0
        push   1fh
        push   edi
        db  0E8h, 017h, 04Dh, 0FEh, 0FFh
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_2_0BD0:
        mov    ebx, 0c001000eh
loc_2_0BD5:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_2_0BE0:
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
        je     SHORT loc_2_0C40
loc_2_0C33:
        mov    ecx, dword ptr [eax + 0ch]
        or     dword ptr [edi + 4], ecx
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_2_0C33
loc_2_0C40:
        mov    ecx, dword ptr [edi + 4]
        cmp    esi, ecx
        je     SHORT loc_2_0C74
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
        je     SHORT loc_2_0C76
        cmp    eax, 103h
        je     SHORT loc_2_0C76
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 0ch], ecx
        mov    dword ptr [edi + 4], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_0C74:
        sub    eax, eax
loc_2_0C76:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  090h
sub_2_0C80:
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
        db  0A0h, 0B0h, 05Dh, 000h, 000h
        cmp    byte ptr [ecx], al
        jne    SHORT loc_2_0D28
        db  0A0h, 0B1h, 05Dh, 000h, 000h
        cmp    byte ptr [ecx + 1], al
        jne    SHORT loc_2_0D28
        db  0A0h, 0B2h, 05Dh, 000h, 000h
        cmp    byte ptr [ecx + 2], al
        jne    SHORT loc_2_0D28
        db  0A0h, 0B3h, 05Dh, 000h, 000h
        cmp    byte ptr [ecx + 3], al
        jne    SHORT loc_2_0D28
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    byte ptr [ebx + 1dh], 0
        je     SHORT loc_2_0D09
        dec    dword ptr [esi + 0ch]
        mov    byte ptr [ebx + 1dh], 0
        cmp    dword ptr [esi + 0ch], 0
        je     NEAR PTR loc_2_0DA3
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_0D09:
        cmp    dword ptr [esi + 0ch], 0
        je     SHORT loc_2_0D1D
        mov    eax, 0c001001ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_0D1D:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_0D28:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 8], eax
        jne    SHORT loc_2_0D60
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    byte ptr [ebx + 1dh], 0
        je     SHORT loc_2_0D44
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_0D44:
        cmp    dword ptr [esi + 0ch], 0
        je     SHORT loc_2_0DA3
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
loc_2_0D60:
        mov    eax, dword ptr [esi + 0ch]
        cmp    eax, 1
        jbe    SHORT loc_2_0D76
        mov    eax, 0c001001ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_0D76:
        mov    ebx, dword ptr [ebp + 0ch]
        jne    SHORT loc_2_0D90
        cmp    byte ptr [ebx + 1dh], 0
        jne    SHORT loc_2_0D90
        mov    eax, 0c001001ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  090h
loc_2_0D90:
        cmp    eax, 1
        jne    SHORT loc_2_0DA3
        cmp    byte ptr [ebx + 1dh], 0
        je     SHORT loc_2_0DA3
        dec    eax
        mov    dword ptr [esi + 0ch], eax
        mov    byte ptr [ebx + 1dh], 0
loc_2_0DA3:
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
        je     SHORT loc_2_0DD6
        cmp    eax, 103h
        je     SHORT loc_2_0DD6
        mov    dword ptr [esi + 8], edi
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0ch], ecx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_0DD6:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_2_0DF0
        mov    byte ptr [ebx + 1dh], 0
        mov    dword ptr [esi + 0ch], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_0DF0:
        mov    byte ptr [ebx + 1dh], 1
        mov    dword ptr [esi + 0ch], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0E10:
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
        je     SHORT loc_2_0E4D
        lea    esp, [esp]
loc_2_0E40:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 10h], ecx
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_2_0E40
loc_2_0E4D:
        mov    ecx, dword ptr [esi + 10h]
        cmp    ebx, ecx
        je     SHORT loc_2_0E81
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
        je     SHORT loc_2_0E83
        cmp    eax, 103h
        je     SHORT loc_2_0E83
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 10h], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_0E81:
        sub    eax, eax
loc_2_0E83:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h
_PTEXT ends
        end
