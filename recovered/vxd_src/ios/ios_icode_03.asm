; ios_icode_03.asm -- IOS object 3 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_3_0000:
        sub    esp, 0ch
        mov    edi, esp
        mov    word ptr [edi], 0
        mov    word ptr [edi + 4], 20h
        push   edi
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 10h
        mov    word ptr [ebx + 2], 0
        ret
loc_3_0021:
        mov    word ptr [ebx + 2], 2
        mov    esi, dword ptr [ebx + 0ch]
        cmp    byte ptr [esi + 0bah], 1
        jae    SHORT loc_3_0040
        mov    byte ptr [esi + 82h], 0
        mov    word ptr [ebx + 2], 0
loc_3_0040:
        ret
loc_3_0041:
        db  08Bh, 035h, 094h, 01Fh, 000h, 000h
        push   esi
        push   edi
        lea    edi, [edi + 0c3h]
        mov    ecx, 19h
        cld
        rep movsd
        pop    edi
        pop    esi
        mov    byte ptr [edi + 77h], 11h
        mov    eax, dword ptr [esi + 8]
        and    eax, 3fh
        or     dword ptr [edi + 20h], eax
        mov    eax, dword ptr [edi + 0dbh]
        mov    dword ptr [edi + 5ch], eax
        mov    eax, dword ptr [edi + 0dfh]
        mov    dword ptr [edi + 60h], eax
        mov    eax, dword ptr [edi + 0e3h]
        mov    dword ptr [edi + 64h], eax
        mov    eax, dword ptr [edi + 0e7h]
        mov    dword ptr [edi + 68h], eax
        mov    dword ptr [edi + 50h], 0ffffffffh
        mov    byte ptr [edi + 77h], 0fh
        mov    eax, dword ptr [edi + 0d3h]
        mov    dword ptr [edi + 54h], eax
        mov    eax, dword ptr [edi + 0d7h]
        mov    dword ptr [edi + 58h], eax
        mov    dword ptr [edi + 127h], esi
        mov    dword ptr [edi + 50h], 0ffffffffh
        mov    al, byte ptr [esi + 7]
        mov    byte ptr [edi + 15h], al
        mov    byte ptr [edi + 0cah], al
        sub    esp, 0ch
        mov    ebp, esp
        mov    word ptr [ebp], 6
        mov    byte ptr [ebp + 8], al
        mov    byte ptr [ebp + 9], 0
        mov    dword ptr [ebp + 4], edi
        push   ebp
        db  0E8h, 035h, 012h, 0FDh, 0FFh
        add    esp, 10h
        sub    esp, 18h
        mov    esi, esp
        mov    word ptr [esi], 5
        mov    dword ptr [esi + 4], edi
        db  0C7h, 046h, 008h, 028h, 002h, 000h, 000h
        xor    ecx, ecx
        mov    word ptr [esi + 10h], cx
        mov    dword ptr [esi + 12h], ecx
        mov    cl, byte ptr [ebx + 8]
        mov    byte ptr [esi + 16h], cl
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [esi + 0ch], ecx
        push   esi
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 1ch
        mov    word ptr [ebx + 2], cx
        ret
sub_3_0116:
        db  080h, 03Dh, 098h, 01Fh, 000h, 000h, 001h
        je     SHORT loc_3_013E
        push   eax
        push   edi
        db  0BFh, 062h, 039h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        jb     SHORT loc_3_013B
        db  0CDh, 020h, 012h, 000h, 003h, 000h
        db  0C6h, 005h, 098h, 01Fh, 000h, 000h, 001h
loc_3_013B:
        pop    edi
        pop    eax
        ret
loc_3_013E:
        clc
        ret
loc_3_0140:
        adc    byte ptr [edi], ah
        db  0E8h, 003h, 064h, 000h
loc_3_0146:
        or     al, byte ptr [eax]
PrintLog:
        db  0C6h, 005h, 01Fh, 022h, 000h, 000h, 001h
        push   eax
        db  0B8h, 000h, 03Bh, 000h, 000h
        db  0A3h, 0FCh, 03Ah, 000h, 000h
        pop    eax
        push   0
        push   ebp
        mov    ebp, esp
        pushad
        mov    esi, dword ptr [ebp + 0ch]
loc_3_0164:
        lodsb
        or     al, al
        jne    SHORT loc_3_019D
        db  080h, 03Dh, 01Fh, 022h, 000h, 000h, 001h
        jne    SHORT loc_3_0197
        xor    eax, eax
        db  0A2h, 01Fh, 022h, 000h, 000h
        db  08Bh, 03Dh, 0FCh, 03Ah, 000h, 000h
        stosb
        call   sub_3_33F4
        db  068h, 000h, 03Bh, 000h, 000h
        call   sub_3_38A0
        add    esp, 4
        call   sub_3_346B
loc_3_0197:
        popad
        pop    ebp
        add    esp, 4
        ret
loc_3_019D:
        cmp    al, 25h
        je     SHORT loc_3_01A6
        jmp    NEAR PTR loc_3_05F7
loc_3_01A6:
        lodsb
        cmp    al, 25h
        jne    SHORT loc_3_01B2
        mov    al, 25h
        jmp    NEAR PTR loc_3_05F7
loc_3_01B2:
        cmp    al, 63h
        jne    SHORT loc_3_01C9
        lea    ebx, [ebp + 10h]
        add    ebx, dword ptr [ebp + 4]
        mov    ebx, dword ptr [ebx]
        add    dword ptr [ebp + 4], 4
        mov    al, byte ptr [ebx]
        jmp    NEAR PTR loc_3_05F7
loc_3_01C9:
        cmp    al, 73h
        jne    SHORT loc_3_01E7
        lea    ebx, [ebp + 10h]
        add    ebx, dword ptr [ebp + 4]
        mov    ebx, dword ptr [ebx]
        add    dword ptr [ebp + 4], 4
loc_3_01D9:
        mov    al, byte ptr [ebx]
        or     al, al
        je     SHORT loc_3_0164
        call   sub_3_0642
        inc    ebx
        jmp    SHORT loc_3_01D9
loc_3_01E7:
        cmp    al, 75h
        jne    SHORT loc_3_0250
        lea    ebx, [ebp + 10h]
        add    ebx, dword ptr [ebp + 4]
        mov    ebx, dword ptr [ebx]
        add    dword ptr [ebp + 4], 4
        mov    ax, word ptr [ebx]
        xor    dx, dx
        db  066h, 0F7h, 035h, 040h, 001h, 000h, 000h
        or     al, 30h
        call   sub_3_0642
        mov    ax, dx
        xor    dx, dx
        db  066h, 0F7h, 035h, 042h, 001h, 000h, 000h
        or     al, 30h
        call   sub_3_0642
        mov    ax, dx
        xor    dx, dx
        db  066h, 0F7h, 035h, 044h, 001h, 000h, 000h
        or     al, 30h
        call   sub_3_0642
        mov    ax, dx
        xor    dx, dx
        db  066h, 0F7h, 035h, 046h, 001h, 000h, 000h
        or     al, 30h
        call   sub_3_0642
        mov    al, dl
        or     al, 30h
        jmp    NEAR PTR loc_3_05F7
loc_3_0250:
        cmp    al, 64h
        jne    NEAR PTR loc_3_036C
        lea    ebx, [ebp + 10h]
        add    ebx, dword ptr [ebp + 4]
        add    dword ptr [ebp + 4], 4
        mov    ax, word ptr [ebx]
        mov    dx, word ptr [ebx + 2]
        cmp    dx, 1998h
        jbe    SHORT loc_3_02BD
        mov    al, 2ah
        call   sub_3_0642
        mov    al, 2ah
        call   sub_3_0642
        mov    al, 2ah
        call   sub_3_0642
        mov    al, 2ch
        call   sub_3_0642
        mov    al, 2ah
        call   sub_3_0642
        mov    al, 2ah
        call   sub_3_0642
        mov    al, 2ah
        call   sub_3_0642
        mov    al, 2ch
        call   sub_3_0642
        mov    al, 2ah
        call   sub_3_0642
        mov    al, 2ah
        call   sub_3_0642
        mov    al, 2ah
        jmp    NEAR PTR loc_3_05F7
loc_3_02BD:
        db  066h, 0F7h, 035h, 040h, 001h, 000h, 000h
        push   edx
        xor    dx, dx
        db  066h, 0F7h, 035h, 040h, 001h, 000h, 000h
        or     al, 30h
        call   sub_3_0642
        mov    ax, dx
        xor    dx, dx
        db  066h, 0F7h, 035h, 042h, 001h, 000h, 000h
        or     al, 30h
        call   sub_3_0642
        mov    ax, dx
        xor    dx, dx
        db  066h, 0F7h, 035h, 044h, 001h, 000h, 000h
        or     al, 30h
        call   sub_3_0642
        mov    al, 2ch
        call   sub_3_0642
        mov    ax, dx
        xor    dx, dx
        db  066h, 0F7h, 035h, 046h, 001h, 000h, 000h
        or     al, 30h
        call   sub_3_0642
        mov    al, dl
        or     al, 30h
        call   sub_3_0642
        pop    eax
        xor    dx, dx
        db  066h, 0F7h, 035h, 042h, 001h, 000h, 000h
        or     al, 30h
        call   sub_3_0642
        mov    al, 2ch
        call   sub_3_0642
        mov    ax, dx
        xor    dx, dx
        db  066h, 0F7h, 035h, 044h, 001h, 000h, 000h
        or     al, 30h
        call   sub_3_0642
        mov    ax, dx
        xor    dx, dx
        db  066h, 0F7h, 035h, 046h, 001h, 000h, 000h
        or     al, 30h
        call   sub_3_0642
        mov    al, dl
        or     al, 30h
        jmp    NEAR PTR loc_3_05F7
loc_3_036C:
        cmp    al, 65h
        jne    SHORT loc_3_038F
        lea    ebx, [ebp + 10h]
        add    ebx, dword ptr [ebp + 4]
        add    dword ptr [ebp + 4], 4
        add    ebx, 2
        call   sub_3_0635
        sub    ebx, 2
        call   sub_3_0635
        jmp    NEAR PTR loc_3_0164
loc_3_038F:
        cmp    al, 69h
        jne    SHORT loc_3_03F5
        lea    ebx, [ebp + 10h]
        add    ebx, dword ptr [ebp + 4]
        add    dword ptr [ebp + 4], 4
        mov    eax, dword ptr [ebx]
        xor    dx, dx
        db  066h, 0F7h, 035h, 040h, 001h, 000h, 000h
        or     al, 30h
        call   sub_3_0642
        mov    ax, dx
        xor    dx, dx
        db  066h, 0F7h, 035h, 042h, 001h, 000h, 000h
        or     al, 30h
        call   sub_3_0642
        mov    ax, dx
        xor    dx, dx
        db  066h, 0F7h, 035h, 044h, 001h, 000h, 000h
        or     al, 30h
        call   sub_3_0642
        mov    ax, dx
        xor    dx, dx
        db  066h, 0F7h, 035h, 046h, 001h, 000h, 000h
        or     al, 30h
        call   sub_3_0642
        mov    al, dl
        or     al, 30h
        jmp    NEAR PTR loc_3_05F7
loc_3_03F5:
        cmp    al, 6ah
        jne    SHORT loc_3_0435
        lea    ebx, [ebp + 10h]
        add    ebx, dword ptr [ebp + 4]
        add    dword ptr [ebp + 4], 4
        mov    eax, dword ptr [ebx]
        xor    ah, ah
        xor    dx, dx
        db  066h, 0F7h, 035h, 044h, 001h, 000h, 000h
        or     al, 30h
        call   sub_3_0642
        mov    ax, dx
        xor    dx, dx
        db  066h, 0F7h, 035h, 046h, 001h, 000h, 000h
        or     al, 30h
        call   sub_3_0642
        mov    al, dl
        or     al, 30h
        jmp    NEAR PTR loc_3_05F7
loc_3_0435:
        cmp    al, 78h
        jne    SHORT loc_3_044F
        lea    ebx, [ebp + 10h]
        add    ebx, dword ptr [ebp + 4]
        mov    ebx, dword ptr [ebx]
        add    dword ptr [ebp + 4], 4
        call   sub_3_0635
        jmp    NEAR PTR loc_3_0164
loc_3_044F:
        cmp    al, 6ch
        jne    SHORT loc_3_047F
        lodsb
        cmp    al, 78h
        je     SHORT loc_3_045D
        jmp    NEAR PTR loc_3_05D7
loc_3_045D:
        lea    ebx, [ebp + 10h]
        add    ebx, dword ptr [ebp + 4]
        mov    ebx, dword ptr [ebx]
        add    dword ptr [ebp + 4], 4
        add    ebx, 2
        call   sub_3_0635
        sub    bx, 2
        call   sub_3_0635
        jmp    NEAR PTR loc_3_0164
loc_3_047F:
        cmp    al, 70h
        jne    SHORT loc_3_04B7
        lea    ebx, [ebp + 10h]
        add    ebx, dword ptr [ebp + 4]
        add    dword ptr [ebp + 4], 4
        add    ebx, 2
        call   sub_3_0635
        mov    al, 3ah
        call   sub_3_0642
        sub    ebx, 2
        call   sub_3_0635
        mov    al, 20h
        call   sub_3_0642
        mov    al, 20h
        call   sub_3_0642
        jmp    NEAR PTR loc_3_0164
loc_3_04B7:
        cmp    al, 62h
        jne    SHORT loc_3_04CF
        lea    ebx, [ebp + 10h]
        add    ebx, dword ptr [ebp + 4]
        add    dword ptr [ebp + 4], 4
        call   sub_3_0626
        jmp    NEAR PTR loc_3_0164
loc_3_04CF:
        cmp    al, 77h
        jne    SHORT loc_3_04E7
        lea    ebx, [ebp + 10h]
        add    ebx, dword ptr [ebp + 4]
        add    dword ptr [ebp + 4], 4
        call   sub_3_0635
        jmp    NEAR PTR loc_3_0164
loc_3_04E7:
        cmp    al, 7ah
        je     SHORT loc_3_04F0
        jmp    NEAR PTR loc_3_05D7
loc_3_04F0:
        lea    ebx, [ebp + 10h]
        add    ebx, dword ptr [ebp + 4]
        add    dword ptr [ebp + 4], 8
        mov    ecx, dword ptr [ebx + 4]
        mov    ebx, dword ptr [ebx]
loc_3_04FF:
        mov    ax, es
        xchg   al, ah
        call   sub_3_0601
        call   sub_3_0614
        xchg   al, ah
        call   sub_3_0601
        call   sub_3_0614
        mov    al, 3ah
        call   sub_3_0642
        mov    eax, ebx
        rol    eax, 10h
        xchg   al, ah
        call   sub_3_0601
        call   sub_3_0614
        xchg   al, ah
        call   sub_3_0601
        call   sub_3_0614
        rol    eax, 10h
        xchg   al, ah
        call   sub_3_0601
        call   sub_3_0614
        xchg   al, ah
        call   sub_3_0601
        call   sub_3_0614
        mov    al, 20h
        call   sub_3_0642
        mov    al, 20h
        call   sub_3_0642
        push   ecx
        mov    edx, 31h
        cmp    ecx, 10h
        jbe    SHORT loc_3_0577
        mov    ecx, 10h
loc_3_0577:
        push   ebx
        push   ecx
loc_3_0579:
        mov    al, byte ptr [ebx]
        call   sub_3_0626
        mov    al, 20h
        cmp    edx, 1ch
        jne    SHORT loc_3_0589
        mov    al, 2dh
loc_3_0589:
        call   sub_3_0642
        sub    edx, 3
        inc    ebx
        loop   loc_3_0579
        mov    ecx, edx
loc_3_0596:
        mov    al, 20h
        call   sub_3_0642
        loop   loc_3_0596
        pop    ecx
        pop    ebx
loc_3_05A1:
        mov    al, 2eh
        mov    ah, byte ptr [ebx]
        cmp    ah, 20h
        jb     SHORT loc_3_05B1
        cmp    ah, 7fh
        ja     SHORT loc_3_05B1
        xchg   ah, al
loc_3_05B1:
        call   sub_3_0642
        inc    ebx
        loop   loc_3_05A1
        pop    ecx
        sub    ecx, 10h
        jle    SHORT loc_3_05D2
        mov    al, 0dh
        call   sub_3_0642
        mov    al, 0ah
        call   sub_3_0642
        jmp    NEAR PTR loc_3_04FF
loc_3_05D2:
        jmp    NEAR PTR loc_3_0164
loc_3_05D7:
        mov    ah, al
        mov    al, 3fh
        call   sub_3_0642
        mov    al, 5ch
        call   sub_3_0642
        mov    al, ah
        call   sub_3_0642
        mov    al, 5ch
        call   sub_3_0642
        mov    al, 7
loc_3_05F7:
        call   sub_3_0642
        jmp    NEAR PTR loc_3_0164
sub_3_0601:
        push   eax
        shr    al, 4
        add    al, 30h
        cmp    al, 39h
        jbe    SHORT loc_3_060D
        add    al, 7
loc_3_060D:
        call   sub_3_0642
        pop    eax
        ret
sub_3_0614:
        push   eax
        and    al, 0fh
        add    al, 30h
        cmp    al, 39h
        jbe    SHORT loc_3_061F
        add    al, 7
loc_3_061F:
        call   sub_3_0642
        pop    eax
        ret
sub_3_0626:
        mov    al, byte ptr [ebx]
        call   sub_3_0601
        mov    al, byte ptr [ebx]
        call   sub_3_0614
        ret
sub_3_0635:
        inc    ebx
        call   sub_3_0626
        dec    ebx
        call   sub_3_0626
        ret
sub_3_0642:
        db  0F6h, 005h, 01Ch, 022h, 000h, 000h, 004h
        jne    SHORT loc_3_0654
        db  080h, 03Dh, 01Fh, 022h, 000h, 000h, 000h
        jne    SHORT loc_3_065B
loc_3_0654:
        db  0CDh, 020h, 0C3h, 000h, 001h, 000h
        ret
loc_3_065B:
        push   edi
        db  08Bh, 03Dh, 0FCh, 03Ah, 000h, 000h
        db  081h, 0FFh, 07Fh, 03Bh, 000h, 000h
        jae    SHORT loc_3_066B
        stosb
loc_3_066B:
        db  089h, 03Dh, 0FCh, 03Ah, 000h, 000h
        pop    edi
        ret
        db  000h
loc_3_0674:
        cmp    ah, 90h
        db  072h, 011h
        je     SHORT loc_3_0683
        cmp    ah, 91h
        db  077h, 00Ah
        xor    ah, ah
        iretd
loc_3_0683:
        xor    ah, ah
        clc
        sti
        retf   2
        db  0EAh, 000h, 000h, 000h, 000h
loc_3_068F:
        db  089h, 01Dh, 0F7h, 039h, 000h, 000h
        mov    eax, dword ptr [edx + 775h]
        db  0A3h, 02Ch, 022h, 000h, 000h
        db  089h, 015h, 030h, 022h, 000h, 000h
        db  0CDh, 020h, 0BBh, 000h, 001h, 000h
        xor    eax, eax
        db  0F6h, 005h, 02Ch, 022h, 000h, 000h, 001h
        jne    SHORT loc_3_06C4
        jecxz  loc_3_06C4
        test   byte ptr [ecx + 5], 82h
        je     SHORT loc_3_06C4
        mov    eax, 40h
loc_3_06C4:
        xor    esi, esi
        db  0BFh, 09Ch, 039h, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        test   eax, eax
        je     SHORT loc_3_06E3
        add    eax, 3
        shr    eax, 2
        xor    ecx, ecx
        db  0CDh, 020h, 00Dh, 000h, 004h, 000h
loc_3_06E3:
        db  0CDh, 020h, 000h, 000h, 020h, 000h
        test   eax, eax
        je     NEAR PTR loc_3_0940
        xor    ecx, ecx
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        db  0A3h, 0BAh, 021h, 000h, 000h
        mov    edx, 8
        db  08Dh, 03Dh, 096h, 021h, 000h, 000h
loc_3_0709:
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        stosd
        dec    edx
        jne    SHORT loc_3_0709
        mov    ecx, 8
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        stosd
        xor    ebx, ebx
        push   0
        push   0ff000h
        push   1000h
        db  0CDh, 020h, 05Dh, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_3_073F
        mov    ebx, 40h
loc_3_073F:
        push   0fh
        db  068h, 0A0h, 01Fh, 000h, 000h
        push   0fefh
        push   0
        push   0
        push   0
        push   1
        mov    eax, 2
        test   ebx, 40h
        je     SHORT loc_3_0765
        mov    eax, 10h
loc_3_0765:
        xor    esi, esi
        db  0BFh, 0CFh, 039h, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        add    eax, 0eh
        cmp    eax, 10h
        jbe    SHORT loc_3_077F
        mov    eax, 10h
loc_3_077F:
        push   eax
        mov    edi, eax
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        or     eax, edx
        db  00Fh, 084h, 0A9h, 01Ah, 0FDh, 0FFh
        mov    eax, edx
        and    eax, 0fffh
        db  001h, 005h, 0A0h, 01Fh, 000h, 000h
        db  089h, 015h, 09Ch, 01Fh, 000h, 000h
        shl    edi, 0ch
        sub    edi, 6eh
        test   ebx, 40h
        jne    SHORT loc_3_07BA
        sub    edi, 0e000h
loc_3_07BA:
        mov    dword ptr [edx + 60h], edi
        mov    dword ptr [edx + 28h], ebx
        test   ebx, 40h
        je     SHORT loc_3_081D
        mov    eax, 10h
        xor    esi, esi
        db  0BFh, 0BEh, 039h, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        or     eax, eax
        db  00Fh, 084h, 05Ah, 01Ah, 0FDh, 0FFh
        db  08Bh, 03Dh, 09Ch, 01Fh, 000h, 000h
        mov    dword ptr [edi + 5ch], eax
        mov    ecx, 0ch
        shl    dword ptr [edi + 5ch], cl
        lea    edx, [edi + 58h]
        push   0ah
        push   edx
        push   0fffh
        push   0
        push   0
        push   0
        push   1
        push   eax
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        or     eax, eax
        db  00Fh, 084h, 024h, 01Ah, 0FDh, 0FFh
        mov    dword ptr [edi + 54h], eax
        mov    edx, edi
loc_3_081D:
        db  0F6h, 005h, 02Ch, 022h, 000h, 000h, 008h
        je     SHORT loc_3_082A
        or     byte ptr [edx + 29h], 1
loc_3_082A:
        mov    ecx, dword ptr [edx + 60h]
        db  0A1h, 0A0h, 01Fh, 000h, 000h
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx + 44h], 0
        mov    ebx, 6eh
        add    ebx, edx
        db  0A1h, 0A0h, 01Fh, 000h, 000h
        add    eax, 82h
        mov    dword ptr [ebx], eax
        mov    dword ptr [ebx + 4], 0
        mov    dword ptr [ebx + 8], ecx
        add    dword ptr [ebx + 8], 5ah
        add    dword ptr [ebx + 8], edx
        mov    word ptr [ebx + 0ch], 0
        mov    word ptr [ebx + 0eh], cx
        sub    word ptr [ebx + 0eh], 14h
        mov    word ptr [ebx + 10h], 1
        mov    ebx, dword ptr [ebx + 8]
        mov    dword ptr [ebx], 0
        mov    dword ptr [ebx + 4], 0
        mov    dword ptr [ebx + 8], 0
        mov    word ptr [ebx + 0ch], 0
        mov    word ptr [ebx + 0eh], 0
        mov    word ptr [ebx + 10h], 1
        xor    eax, eax
        xor    esi, esi
        push   edi
        db  0BFh, 084h, 039h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        pop    edi
        test   eax, eax
        je     SHORT loc_3_0917
        mov    al, 76h
        db  0CDh, 020h, 0DCh, 000h, 001h, 000h
        je     SHORT loc_3_0917
        xor    edi, edi
        call   sub_3_0116
        jb     SHORT loc_3_0917
        db  0C6h, 005h, 045h, 022h, 000h, 000h, 001h
        mov    eax, 15h
        db  0BEh, 080h, 001h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        push   0
        push   1bh
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        add    esp, 8
        test   eax, eax
        je     SHORT loc_3_0917
        mov    edi, eax
        shl    eax, 0ch
        shr    ax, 0ch
        xchg   dword ptr [54h], eax
        db  0A3h, 040h, 022h, 000h, 000h
        db  0A3h, 08Bh, 006h, 000h, 000h
        db  0BEh, 074h, 006h, 000h, 000h
        mov    ecx, 1bh
        cld
        rep movsb
loc_3_0917:
        mov    eax, 13h
        db  0BEh, 070h, 003h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        db  0BEh, 050h, 006h, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        db  0A3h, 034h, 022h, 000h, 000h
        jb     SHORT loc_3_093A
        ret
loc_3_093A:
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
loc_3_0940:
        stc
        ret
loc_3_0942:
        db  089h, 00Dh, 058h, 03Ah, 000h, 000h
        call   sub_3_361C
        mov    eax, 0fah
        db  0BEh, 0BCh, 01Bh, 000h, 000h
        db  0CDh, 020h, 0FEh, 000h, 001h, 000h
        or     esi, esi
        jne    SHORT loc_3_0961
loc_3_0961:
        db  08Bh, 03Dh, 09Ch, 01Fh, 000h, 000h
        db  08Bh, 015h, 030h, 022h, 000h, 000h
        lea    eax, [edx + 2]
        mov    dword ptr [edi + 2ch], eax
        mov    al, byte ptr [edx + 77ah]
        db  0A2h, 024h, 022h, 000h, 000h
        lea    eax, [edx + 662h]
        mov    dword ptr [edi + 38h], eax
        mov    al, byte ptr [edx + 779h]
        mov    byte ptr [edi + 22h], al
        mov    ecx, 0f4240h
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    esi, eax
loc_3_099D:
        loop   loc_3_099D
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        sub    eax, esi
        push   eax
        mov    eax, 0f4240h
        sub    edx, edx
        mov    ecx, 2710h
        mul    ecx
        pop    ecx
        div    ecx
        sub    edx, edx
        mov    ecx, 99h
        div    ecx
        mov    dword ptr [edi + 14h], eax
        call   sub_3_1A39
        call   sub_3_1214
        call   sub_3_1669
        call   sub_3_1ADC
        call   sub_3_1207
        db  0E8h, 074h, 018h, 0FDh, 0FFh
        call   sub_3_32F4
        call   sub_3_0A69
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        test   byte ptr [ecx + 28h], 10h
        jne    SHORT loc_3_0A12
        call   sub_3_16F4
        call   sub_3_1506
        call   sub_3_18DC
        call   sub_3_1895
        call   sub_3_3562
loc_3_0A12:
        call   sub_3_223F
        call   sub_3_228D
        call   sub_3_0DB6
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        test   byte ptr [ecx + 28h], 10h
        je     SHORT loc_3_0A31
        and    dword ptr [ecx + 28h], 0fffffff5h
loc_3_0A31:
        test   byte ptr [ecx + 28h], 0ah
        jne    SHORT loc_3_0A3E
        mov    al, 1
        call   sub_3_1144
loc_3_0A3E:
        db  0E8h, 07Fh, 0FBh, 0FCh, 0FFh
        db  08Bh, 01Dh, 0F7h, 039h, 000h, 000h
        call   sub_3_3200
        call   sub_3_0CCC
        push   0
        push   0
        db  068h, 03Dh, 01Bh, 000h, 000h
        db  0CDh, 020h, 011h, 000h, 017h, 000h
        add    esp, 0ch
        clc
        ret
        db  0F8h, 0C3h
sub_3_0A69:
        sub    esp, 10h
        db  08Dh, 035h, 079h, 03Ah, 000h, 000h
        lea    edi, [esp + 0ch]
        push   edi
        push   esi
        push   80000002h
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_3_0BA6
        mov    dword ptr [esp], 0
        lea    edx, [esp + 8]
        lea    esi, [esp]
        lea    edi, [esp + 4]
        mov    dword ptr [edi], 4
        db  08Dh, 005h, 0C8h, 03Ah, 000h, 000h
        mov    ecx, dword ptr [esp + 0ch]
        push   edi
        push   esi
        push   edx
        push   0
        push   eax
        push   ecx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_3_0ACC
        mov    eax, dword ptr [esp]
        db  0A3h, 000h, 000h, 000h, 000h
loc_3_0ACC:
        mov    dword ptr [esp], 0
        lea    edx, [esp + 8]
        lea    esi, [esp]
        lea    edi, [esp + 4]
        mov    dword ptr [edi], 4
        db  08Dh, 005h, 0D9h, 03Ah, 000h, 000h
        mov    ecx, dword ptr [esp + 0ch]
        push   edi
        push   esi
        push   edx
        push   0
        push   eax
        push   ecx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_3_0B0A
        mov    eax, dword ptr [esp]
        db  0A3h, 004h, 000h, 000h, 000h
loc_3_0B0A:
        db  0CDh, 020h, 000h, 000h, 026h, 000h
        mov    al, 0
        adc    al, 0
        db  0A2h, 00Eh, 000h, 000h, 000h
        or     al, al
        jne    SHORT loc_3_0B2D
        push   0
        db  068h, 074h, 030h, 000h, 000h
        db  0CDh, 020h, 00Ah, 000h, 026h, 000h
        add    esp, 8
loc_3_0B2D:
        db  0E8h, 054h, 025h, 0FDh, 0FFh
        mov    dword ptr [esp], 0
        lea    edx, [esp + 8]
        lea    esi, [esp]
        lea    edi, [esp + 4]
        mov    dword ptr [edi], 4
        db  08Dh, 005h, 0A5h, 03Ah, 000h, 000h
        mov    ecx, dword ptr [esp + 0ch]
        push   edi
        push   esi
        push   edx
        push   0
        push   eax
        push   ecx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_3_0B7A
        cmp    byte ptr [esp], 0
        je     SHORT loc_3_0B7A
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        or     byte ptr [ecx + 28h], 4
        jmp    SHORT loc_3_0BC3
loc_3_0B7A:
        lea    edx, [esp + 8]
        db  08Dh, 005h, 0AFh, 03Ah, 000h, 000h
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [edi], 4
        push   edi
        push   esi
        push   edx
        push   0
        push   eax
        push   ecx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        movzx  eax, byte ptr [esp]
        je     SHORT loc_3_0BB5
loc_3_0BA6:
        xor    eax, eax
        xor    esi, esi
        db  0BFh, 0AEh, 039h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
loc_3_0BB5:
        test   eax, eax
        je     SHORT loc_3_0BC3
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        or     byte ptr [ecx + 28h], 10h
loc_3_0BC3:
        add    esp, 10h
        ret
loc_3_0BC7:
        call   sub_3_318F
        call   sub_3_0C25
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        or     byte ptr [eax + 28h], 1
        db  0E8h, 00Dh, 009h, 0FDh, 0FFh
        db  0E8h, 00Ch, 010h, 0FDh, 0FFh
        call   sub_3_3655
        db  080h, 00Dh, 01Ch, 022h, 000h, 000h, 004h
        db  0CDh, 020h, 007h, 000h, 021h, 000h
        movzx  eax, byte ptr [esi]
        or     eax, eax
        je     SHORT loc_3_0C23
        sub    al, 41h
        db  0E8h, 089h, 006h, 0FDh, 0FFh
        jb     SHORT loc_3_0C23
        or     byte ptr [ebx + 21h], 1
        mov    ebx, dword ptr [ebx]
        or     byte ptr [ebx + 21h], 1
        cmp    byte ptr [ebx + 77h], 10h
        jae    SHORT loc_3_0C23
        push   0
        push   1
        db  0CDh, 020h, 070h, 000h, 001h, 000h
        add    esp, 8
loc_3_0C23:
        clc
        ret
sub_3_0C25:
        db  0CDh, 020h, 000h, 000h, 007h, 000h
        cmp    bl, 2
        db  08Bh, 01Dh, 09Ch, 01Fh, 000h, 000h
        jne    SHORT loc_3_0CAD
        or     byte ptr [ebx + 29h], 20h
        test   byte ptr [ebx + 28h], 40h
        jne    SHORT loc_3_0CAC
        cli
        mov    ecx, dword ptr [ebx + 60h]
        add    ecx, ebx
        add    ecx, 6eh
        mov    eax, ecx
        add    ecx, 0dfech
        mov    dword ptr [ecx], 0
        mov    dword ptr [ecx + 4], 0
        mov    dword ptr [ecx + 8], 0
        mov    word ptr [ecx + 0ch], 0
        mov    word ptr [ecx + 0eh], 0
        mov    word ptr [ecx + 10h], 1
        sub    eax, 14h
        db  08Bh, 015h, 0A0h, 01Fh, 000h, 000h
        add    edx, dword ptr [ebx + 60h]
        add    edx, 6eh
        mov    dword ptr [eax], edx
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], ecx
        mov    word ptr [eax + 0ch], 0
        mov    word ptr [eax + 0eh], 0e000h
        mov    word ptr [eax + 10h], 1
        add    dword ptr [ebx + 60h], 0e000h
        sti
loc_3_0CAC:
        ret
loc_3_0CAD:
        test   byte ptr [ebx + 28h], 40h
        jne    SHORT loc_3_0CAC
        mov    eax, dword ptr [ebx + 60h]
        add    eax, 6eh
        shr    eax, 0ch
        push   40h
        push   eax
        push   ebx
        db  0CDh, 020h, 054h, 000h, 001h, 000h
        add    esp, 0ch
        ret
        db  000h, 000h
sub_3_0CCC:
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        test   dword ptr [ecx + 28h], 0ah
        jne    SHORT loc_3_0CFA
        sub    edx, edx
        db  0BEh, 069h, 006h, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        movzx  edx, ax
        mov    ecx, eax
        shr    ecx, 10h
        xor    eax, eax
        mov    al, 13h
        db  0CDh, 020h, 043h, 000h, 001h, 000h
loc_3_0CFA:
        ret
        db  000h
sub_3_0CFC:
        call   sub_3_33F4
        db  0CDh, 020h, 0B7h, 000h, 001h, 000h
        mov    esi, edx
        db  0BFh, 03Ah, 03Ch, 000h, 000h
        db  0E8h, 0D0h, 014h, 0FDh, 0FFh
        dec    edi
        db  0BEh, 0D6h, 03Ch, 000h, 000h
        db  0E8h, 0C5h, 014h, 0FDh, 0FFh
        db  0BAh, 03Ah, 03Ch, 000h, 000h
        xor    cx, cx
        mov    ax, 3d00h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_3_0DAF
        db  066h, 0A3h, 0DEh, 03Ch, 000h, 000h
        mov    bx, ax
        mov    ax, 4202h
        xor    cx, cx
        mov    dx, cx
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_3_0DAF
        or     dx, dx
        movzx  esi, ax
        db  066h, 0A3h, 0E8h, 03Ch, 000h, 000h
        push   1
        push   esi
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_3_0DAF
        db  0A3h, 0E0h, 03Ch, 000h, 000h
        db  066h, 08Bh, 01Dh, 0DEh, 03Ch, 000h, 000h
        mov    ax, 4200h
        xor    cx, cx
        mov    dx, cx
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_3_0DAF
        db  08Bh, 015h, 0E0h, 03Ch, 000h, 000h
        db  066h, 08Bh, 01Dh, 0DEh, 03Ch, 000h, 000h
        mov    ecx, esi
        mov    ah, 3fh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_3_0DAF
        call   sub_3_346B
        clc
        ret
loc_3_0DAF:
        call   sub_3_346B
        stc
        ret
sub_3_0DB6:
        db  066h, 083h, 03Dh, 0DEh, 03Ch, 000h, 000h, 0FFh
        je     SHORT loc_3_0DE4
        db  066h, 08Bh, 01Dh, 0DEh, 03Ch, 000h, 000h
        mov    ah, 3eh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        push   0
        db  0FFh, 035h, 0E0h, 03Ch, 000h, 000h
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
loc_3_0DE4:
        ret
sub_3_0DE5:
        db  066h, 083h, 03Dh, 0DEh, 03Ch, 000h, 000h, 0FFh
        jne    SHORT loc_3_0DF5
        call   sub_3_0CFC
        ret
loc_3_0DF5:
        clc
        ret
sub_3_0DF7:
        push   esi
        push   edi
        push   ebx
        cmp    dword ptr [esi], 52444d44h
        jne    SHORT loc_3_0E0B
        cmp    dword ptr [esi + 4], 20205256h
        je     SHORT loc_3_0E1C
loc_3_0E0B:
        cmp    dword ptr [esi], 4f494258h
        jne    SHORT loc_3_0E2C
        cmp    dword ptr [esi + 4], 20202053h
        jne    SHORT loc_3_0E2C
loc_3_0E1C:
        push   esi
        xor    esi, esi
        call   sub_3_12A5
        pop    esi
        mov    eax, 0
        jae    SHORT loc_3_0E48
loc_3_0E2C:
        xor    eax, eax
        call   sub_3_0E4C
        jae    SHORT loc_3_0E48
        call   sub_3_10CB
        xor    edx, edx
        db  0BFh, 000h, 03Dh, 000h, 000h
        call   sub_3_0E90
        mov    eax, edx
loc_3_0E48:
        pop    ebx
        pop    edi
        pop    esi
        ret
sub_3_0E4C:
        mov    edx, esi
        mov    ecx, 7
        db  0BFh, 0E4h, 03Bh, 000h, 000h
        mov    ebx, edi
loc_3_0E5A:
        push   ecx
        mov    ecx, 0bh
        repe cmpsb
        or     ecx, ecx
        pop    ecx
        je     SHORT loc_3_0E71
        mov    esi, edx
        add    ebx, 0ch
        mov    edi, ebx
        loop   loc_3_0E5A
        stc
loc_3_0E71:
        ret
sub_3_0E72:
        push   edi
        push   esi
        push   ebx
        call   sub_3_0E4C
        cmc
        jb     SHORT loc_3_0E8C
        call   sub_3_10CB
        db  0BFh, 0F7h, 03Ch, 000h, 000h
        call   sub_3_0E90
loc_3_0E8C:
        pop    ebx
        pop    esi
        pop    edi
        ret
sub_3_0E90:
        sub    esp, 8
        push   edi
        push   esi
        push   ebx
        mov    dword ptr [esp + 10h], edi
        mov    dword ptr [esp + 0ch], esi
        call   sub_3_0DE5
        jb     SHORT loc_3_0ECD
        mov    esi, dword ptr [esp + 10h]
        call   sub_3_0F4A
        jb     SHORT loc_3_0ECD
        mov    esi, dword ptr [esp + 0ch]
loc_3_0EB4:
        call   sub_3_107A
        jae    SHORT loc_3_0ECD
        db  0BFh, 03Ah, 03Ch, 000h, 000h
        call   sub_3_0F93
        jb     SHORT loc_3_0ECD
        cmp    byte ptr [edi], 5bh
        jne    SHORT loc_3_0EB4
        stc
loc_3_0ECD:
        rcl    ax, 1
        pop    ebx
        pop    esi
        pop    edi
        add    esp, 8
        rcr    ax, 1
        ret
sub_3_0EDA:
        push   esi
        push   edi
        push   ebx
        sub    esp, 0ch
        mov    dword ptr [esp], esi
        mov    dword ptr [esp + 4], edi
        mov    dword ptr [esp + 8], ecx
        call   sub_3_0DE5
        jb     SHORT loc_3_0F47
        db  0BEh, 0EAh, 03Ch, 000h, 000h
        call   sub_3_0F4A
        jb     SHORT loc_3_0F47
loc_3_0EFE:
        mov    esi, edi
loc_3_0F00:
        lodsb
        cmp    al, 3dh
        jne    SHORT loc_3_0F00
        mov    ecx, esi
        dec    esi
        mov    byte ptr [esi], 0
        mov    esi, dword ptr [esp]
        db  0E8h, 0DCh, 012h, 0FDh, 0FFh
        call   sub_3_107A
        jae    SHORT loc_3_0F2D
        db  0BFh, 03Ah, 03Ch, 000h, 000h
        call   sub_3_0F93
        jb     SHORT loc_3_0F47
        cmp    byte ptr [edi], 5bh
        je     SHORT loc_3_0F47
        jmp    SHORT loc_3_0EFE
loc_3_0F2D:
        mov    esi, ecx
        mov    edi, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
loc_3_0F37:
        lodsb
        stosb
        or     al, al
        je     SHORT loc_3_0F3F
        loop   loc_3_0F37
loc_3_0F3F:
        clc
loc_3_0F40:
        add    esp, 0ch
        pop    ebx
        pop    edi
        pop    esi
        ret
loc_3_0F47:
        stc
        jmp    SHORT loc_3_0F40
sub_3_0F4A:
        db  0E8h, 0A0h, 012h, 0FDh, 0FFh
        db  0C7h, 005h, 0E4h, 03Ch, 000h, 000h, 000h, 000h, 000h, 000h
loc_3_0F59:
        db  0BFh, 03Ah, 03Ch, 000h, 000h
loc_3_0F5E:
        call   sub_3_0F93
        jb     SHORT loc_3_0F92
        cmp    byte ptr [edi], 5bh
        jne    SHORT loc_3_0F5E
        inc    edi
        xchg   edi, esi
        push   esi
loc_3_0F6E:
        lodsb
        cmp    al, 5dh
        jne    SHORT loc_3_0F6E
        dec    esi
        mov    byte ptr [esi], 0
        pop    esi
        call   sub_3_107A
        mov    esi, edi
        jb     SHORT loc_3_0F59
        db  0BFh, 03Ah, 03Ch, 000h, 000h
loc_3_0F86:
        call   sub_3_0F93
        jb     SHORT loc_3_0F92
        cmp    byte ptr [edi], 0
        je     SHORT loc_3_0F86
loc_3_0F92:
        ret
sub_3_0F93:
        db  00Fh, 0B7h, 005h, 0E8h, 03Ch, 000h, 000h
        db  039h, 005h, 0E4h, 03Ch, 000h, 000h
        jae    NEAR PTR loc_3_1078
        xor    edx, edx
        push   esi
        push   edi
        db  08Bh, 035h, 0E0h, 03Ch, 000h, 000h
        mov    ebx, esi
        db  003h, 035h, 0E4h, 03Ch, 000h, 000h
        add    ebx, eax
        db  0A3h, 0E4h, 03Ch, 000h, 000h
loc_3_0FBF:
        lodsb
        cmp    al, 20h
        je     SHORT loc_3_0FBF
        cmp    al, 9
        je     SHORT loc_3_0FBF
        cmp    al, 2ch
        je     SHORT loc_3_100D
        cmp    al, 0dh
        je     SHORT loc_3_0FE8
        cmp    al, 0ah
        je     SHORT loc_3_0FE8
        cmp    al, 3bh
        je     SHORT loc_3_0FDF
        cmp    esi, ebx
        jae    SHORT loc_3_0FFF
        stosb
        jmp    SHORT loc_3_0FBF
loc_3_0FDF:
        lodsb
        cmp    al, 0dh
        je     SHORT loc_3_0FE8
        cmp    al, 0ah
        jne    SHORT loc_3_0FDF
loc_3_0FE8:
        lodsb
        cmp    al, 0dh
        je     SHORT loc_3_0FE8
        cmp    al, 0ah
        je     SHORT loc_3_0FE8
        dec    esi
        db  0A1h, 0E0h, 03Ch, 000h, 000h
        sub    esi, eax
        db  089h, 035h, 0E4h, 03Ch, 000h, 000h
loc_3_0FFF:
        mov    byte ptr [edi], 0
        pop    edi
        mov    esi, edi
        db  0E8h, 0E5h, 011h, 0FDh, 0FFh
        pop    esi
        clc
        ret
loc_3_100D:
        mov    byte ptr [edi], 0
        push   edi
        inc    edi
        mov    edx, edi
loc_3_1014:
        lodsb
        cmp    al, 20h
        je     SHORT loc_3_1014
        cmp    al, 9
        je     SHORT loc_3_1014
        cmp    al, 0dh
        je     SHORT loc_3_1035
        cmp    al, 0ah
        je     SHORT loc_3_1035
        cmp    esi, ebx
        jae    SHORT loc_3_102C
        stosb
        jmp    SHORT loc_3_1014
loc_3_102C:
        db  066h, 00Fh, 0BAh, 02Dh, 038h, 03Ch, 000h, 000h, 000h
loc_3_1035:
        push   esi
        mov    byte ptr [edi], 0
        mov    esi, edx
        db  0E8h, 0AFh, 011h, 0FDh, 0FFh
        db  0BBh, 080h, 03Bh, 000h, 000h
        xor    edx, edx
        mov    eax, esi
loc_3_1049:
        cmp    edx, 5
        ja     SHORT loc_3_1061
        mov    edi, dword ptr [ebx + edx*4]
        movzx  ecx, byte ptr [edi]
        inc    edi
        repe cmpsb
        jecxz  loc_3_1066
        inc    edx
        mov    esi, eax
        jmp    SHORT loc_3_1049
loc_3_1061:
        mov    edx, 0ffffffffh
loc_3_1066:
        pop    esi
        pop    edi
        db  066h, 00Fh, 0BAh, 035h, 038h, 03Ch, 000h, 000h, 000h
        jb     SHORT loc_3_0FFF
        jmp    NEAR PTR loc_3_0FE8
loc_3_1078:
        stc
        ret
sub_3_107A:
        push   esi
        push   edi
        mov    eax, dword ptr [edi]
        cmp    ax, 2e2ah
        jne    SHORT loc_3_108B
        shr    eax, 10h
        cmp    al, 2ah
        je     SHORT loc_3_10B2
loc_3_108B:
        lodsb
        or     al, al
        je     SHORT loc_3_10B2
        cmp    al, 2eh
        je     SHORT loc_3_10A7
        mov    bl, byte ptr [edi]
        cmp    bl, 2ah
        je     SHORT loc_3_10A7
        cmp    bl, 3fh
        je     SHORT loc_3_10A4
        cmp    al, bl
        jne    SHORT loc_3_10C7
loc_3_10A4:
        inc    edi
        jmp    SHORT loc_3_108B
loc_3_10A7:
        xchg   edi, esi
loc_3_10A9:
        lodsb
        cmp    al, 2eh
        je     SHORT loc_3_10B6
        or     al, al
        jne    SHORT loc_3_10A9
loc_3_10B2:
        pop    edi
        pop    esi
        clc
        ret
loc_3_10B6:
        xchg   edi, esi
        cmp    byte ptr [esi - 1], 2eh
        je     SHORT loc_3_108B
loc_3_10BE:
        lodsb
        cmp    al, 2eh
        je     SHORT loc_3_108B
        or     al, al
        jne    SHORT loc_3_10BE
loc_3_10C7:
        pop    edi
        pop    esi
        stc
        ret
sub_3_10CB:
        db  0BFh, 0BEh, 03Ch, 000h, 000h
        mov    edx, edi
        xor    ebx, ebx
        mov    ecx, 8
loc_3_10D9:
        lodsb
        cmp    al, 20h
        je     SHORT loc_3_10DF
        stosb
loc_3_10DF:
        loop   loc_3_10D9
        bts    ebx, 0
        jb     SHORT loc_3_10F1
        mov    al, 2eh
        stosb
        mov    ecx, 3
        jmp    SHORT loc_3_10D9
loc_3_10F1:
        mov    byte ptr [edi], 0
        mov    esi, edx
        db  0E8h, 0F4h, 010h, 0FDh, 0FFh
        ret
sub_3_10FC:
        push   esi
        push   edi
        db  0BFh, 0CBh, 03Ch, 000h, 000h
        mov    edx, edi
        mov    al, 20h
        mov    ecx, 0bh
        rep stosb
        mov    edi, edx
        mov    ecx, 8
loc_3_1115:
        lodsb
        or     al, al
        je     SHORT loc_3_111D
        stosb
        loop   loc_3_1115
loc_3_111D:
        mov    edi, edx
        mov    word ptr [edi + 8], 5845h
        mov    byte ptr [edi + 0ah], 45h
        mov    esi, edx
        call   sub_3_0DF7
        jae    SHORT loc_3_1141
        mov    word ptr [edi + 8], 4f43h
        mov    byte ptr [edi + 0ah], 4dh
        call   sub_3_0DF7
loc_3_1141:
        pop    edi
        pop    esi
        ret
sub_3_1144:
        sub    esp, 14h
        pushad
        lea    edi, [esp + 20h]
        mov    byte ptr [edi], al
        lea    esi, [esp + 30h]
        push   esi
        db  068h, 079h, 03Ah, 000h, 000h
        push   80000002h
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_11BB
        mov    ebx, dword ptr [esi]
        lea    ecx, [esp + 28h]
        lea    esi, [esp + 24h]
        lea    edx, [esp + 2ch]
        mov    byte ptr [esi], al
        inc    eax
        mov    dword ptr [ecx], eax
        push   ecx
        push   esi
        push   edx
        push   0
        db  068h, 0B9h, 03Ah, 000h, 000h
        push   ebx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_3_119B
        mov    al, byte ptr [edi]
        cmp    byte ptr [esi], al
        je     SHORT loc_3_11B1
loc_3_119B:
        push   1
        push   edi
        push   3
        push   0
        db  068h, 0B9h, 03Ah, 000h, 000h
        push   ebx
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
loc_3_11B1:
        push   ebx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
loc_3_11BB:
        popad
        mov    ah, byte ptr [esp + 4]
        add    esp, 14h
        ret
sub_3_11C4:
        pushad
        db  080h, 03Dh, 044h, 005h, 000h, 000h, 000h
        jne    SHORT loc_3_1205
        db  088h, 00Dh, 044h, 005h, 000h, 000h
        mov    ecx, dword ptr [esi]
        db  089h, 00Dh, 045h, 005h, 000h, 000h
        mov    ecx, dword ptr [esi + 4]
        db  089h, 00Dh, 049h, 005h, 000h, 000h
        xor    eax, eax
        call   sub_3_1144
        test   ah, ah
        je     SHORT loc_3_1205
        or     eax, 0ffffffffh
        xor    esi, esi
        db  0BFh, 0E9h, 03Ah, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        db  0A2h, 043h, 003h, 000h, 000h
loc_3_1205:
        popad
        ret
sub_3_1207:
        mov    eax, 4ch
        mov    eax, dword ptr [eax]
        db  0A3h, 0C3h, 021h, 000h, 000h
        ret
sub_3_1214:
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        mov    esi, dword ptr [eax + 4ch]
loc_3_121C:
        or     esi, esi
        je     NEAR PTR loc_3_12A4
        mov    edi, dword ptr [esi]
        call   sub_3_2F6A
        cmp    byte ptr [edi], 7
        je     SHORT loc_3_123C
        mov    edi, dword ptr [esi + 4]
        call   sub_3_2F6A
        mov    esi, edi
        jmp    SHORT loc_3_121C
loc_3_123C:
        db  089h, 03Dh, 00Bh, 03Ah, 000h, 000h
        push   ebp
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    byte ptr [ebp + 1dh], 52h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  edi, word ptr [ebp + 38h]
        shl    edi, 4
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    ebp
        db  089h, 03Dh, 025h, 022h, 000h, 000h
        mov    edi, dword ptr [edi + 1328h]
        call   sub_3_2F6A
        db  089h, 03Dh, 068h, 03Ah, 000h, 000h
loc_3_12A4:
        ret
sub_3_12A5:
        pushad
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        call   sub_3_33F4
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ecx, 6
        clc
        db  0CDh, 020h, 008h, 000h, 006h, 000h
        cmp    ecx, 6
        db  00Fh, 085h, 02Bh, 00Fh, 0FDh, 0FFh
        mov    word ptr [ebp + 1ch], 0f900h
        mov    word ptr [ebp + 14h], 80h
        mov    eax, 13h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        test   byte ptr [ebp + 2ch], 1
        jne    NEAR PTR loc_3_137C
        test   word ptr [ebp + 14h], 8000h
        je     SHORT loc_3_137C
        push   esi
        mov    esi, edi
        call   sub_3_2F6A
        mov    word ptr [ebp + 10h], si
        shr    esi, 10h
        mov    word ptr [ebp + 3ch], si
        mov    word ptr [ebp + 38h], si
        pop    esi
        mov    word ptr [ebp + 1ch], 0e000h
        mov    byte ptr [edi], 0eh
        mov    byte ptr [edi + 1], 0
        mov    word ptr [ebp + 18h], 0
        mov    byte ptr [ebp + 14h], 80h
        mov    eax, 13h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        test   byte ptr [ebp + 2ch], 1
        jne    SHORT loc_3_137C
        cmp    word ptr [ebp + 18h], 1234h
        clc
        jne    SHORT loc_3_137C
        mov    eax, edi
        or     esi, esi
        je     SHORT loc_3_136B
        mov    edi, esi
        and    edi, 0ffff0000h
        call   sub_3_2F6A
        mov    cx, word ptr [eax + 4]
        cmp    word ptr [edi + 2], cx
        clc
        jne    SHORT loc_3_137C
loc_3_136B:
        mov    edi, dword ptr [eax + 2]
        call   sub_3_2F6A
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        mov    dword ptr [eax + 68h], edi
        stc
loc_3_137C:
        setb   dl
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ecx, 6
        clc
        db  0CDh, 020h, 009h, 000h, 006h, 000h
        call   sub_3_346B
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        rcr    dl, 1
        cmc
        popad
        ret
sub_3_13B0:
        pushad
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 0ff00h
        mov    word ptr [ebp + 14h], 80h
        mov    eax, 13h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        test   byte ptr [ebp + 2ch], 1
        jne    SHORT loc_3_144D
        cmp    word ptr [ebp + 1ch], 55aah
        clc
        jne    SHORT loc_3_144D
        mov    di, word ptr [ebp + 38h]
        rol    esi, 10h
        cmp    di, si
        clc
        jne    SHORT loc_3_144D
        rol    edi, 10h
        mov    di, word ptr [ebp + 10h]
        call   sub_3_2F6A
        cmp    dword ptr [edi], 4d524541h
        clc
        jne    SHORT loc_3_144D
        cmp    dword ptr [edi + 4], 56333148h
        clc
        jne    SHORT loc_3_144D
        mov    ax, word ptr [edi + 8]
        xchg   ah, al
        cmp    ax, 3336h
        ja     SHORT loc_3_1440
        db  080h, 03Dh, 024h, 022h, 000h, 000h, 003h
        jb     SHORT loc_3_1440
        pushfd
        db  068h, 026h, 03Dh, 000h, 000h
        db  0CDh, 020h, 010h, 000h, 010h, 000h
        add    esp, 4
        popfd
        clc
        jmp    SHORT loc_3_144D
loc_3_1440:
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        or     dword ptr [eax + 28h], 400000h
        stc
loc_3_144D:
        setb   dl
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        rcr    dl, 1
        cmc
        popad
        ret
sub_3_146A:
        push   edi
        push   esi
        lea    esi, [edi]
        db  0BFh, 06Ch, 03Ah, 000h, 000h
        mov    ecx, 8
        repe cmpsb
        pop    esi
        push   esi
        jecxz  loc_3_1481
        stc
        jmp    SHORT loc_3_1500
loc_3_1481:
        mov    edi, dword ptr [esi + 1]
        db  089h, 03Dh, 075h, 03Ah, 000h, 000h
loc_3_148A:
        mov    esi, edi
        call   sub_3_2F6A
        cmp    byte ptr [edi], 0e9h
        je     SHORT loc_3_149B
        cmp    byte ptr [edi], 0ebh
        jne    SHORT loc_3_14BE
loc_3_149B:
        cmp    dword ptr [edi + 3], 544e4924h
        jne    SHORT loc_3_14BE
        cmp    dword ptr [edi + 7], 46533331h
        jne    SHORT loc_3_14BE
        mov    edi, dword ptr [edi + 13h]
        mov    ecx, edi
        rol    ecx, 10h
        cmp    cx, 0a000h
        jae    SHORT loc_3_14FD
        jmp    SHORT loc_3_148A
loc_3_14BE:
        call   sub_3_12A5
        jae    SHORT loc_3_14FD
        call   sub_3_13B0
        jae    SHORT loc_3_14FD
        push   edi
        movzx  eax, si
        rol    esi, 10h
        movzx  esi, si
        shl    esi, 4
        add    esi, eax
        push   12h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        shl    eax, 0ah
        cmp    eax, esi
        pop    edi
        jae    SHORT loc_3_14FD
        cmp    byte ptr [edi], 9ah
        je     SHORT loc_3_14F5
        cmp    byte ptr [edi], 0eah
        jne    SHORT loc_3_1503
loc_3_14F5:
        cmp    word ptr [edi + 3], 0a000h
        jb     SHORT loc_3_1500
loc_3_14FD:
        clc
        xor    eax, eax
loc_3_1500:
        pop    esi
        pop    edi
        ret
loc_3_1503:
        stc
        jmp    SHORT loc_3_1500
sub_3_1506:
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        test   dword ptr [eax + 28h], 2
        jne    NEAR PTR loc_3_1668
        db  08Bh, 03Dh, 00Bh, 03Ah, 000h, 000h
        movzx  ecx, byte ptr [edi + 5]
        lea    edi, [edi + 6]
loc_3_1525:
        push   ecx
        movzx  ecx, byte ptr [edi + 0eh]
        lea    esi, [edi + 0fh]
loc_3_152D:
        cmp    byte ptr [esi], 2
        je     SHORT loc_3_15A7
        cmp    byte ptr [esi], 3
        je     SHORT loc_3_15A7
loc_3_1537:
        db  0F6h, 005h, 01Bh, 022h, 000h, 000h, 080h
        jne    SHORT loc_3_15A2
        add    esi, 5
        loop   loc_3_152D
        mov    edi, esi
        pop    ecx
        loop   loc_3_1525
        db  080h, 00Dh, 01Bh, 022h, 000h, 000h, 080h
        push   ecx
        db  08Bh, 03Dh, 068h, 03Ah, 000h, 000h
        cmp    word ptr [edi + 0feh], -1
        jne    SHORT loc_3_1577
        pushfd
        db  068h, 06Ch, 03Dh, 000h, 000h
        db  0CDh, 020h, 010h, 000h, 010h, 000h
        add    esp, 4
        popfd
        jmp    NEAR PTR loc_3_165E
loc_3_1577:
        lea    edi, [edi + 0eh]
loc_3_157A:
        movzx  esi, word ptr [edi]
        or     esi, esi
        je     NEAR PTR loc_3_1667
        test   byte ptr [edi + 7], 80h
        je     SHORT loc_3_15A2
        dec    esi
        shl    esi, 4
        lea    esi, [esi + 8]
        call   sub_3_10FC
        jae    SHORT loc_3_15F3
        or     byte ptr [edi + 7], 40h
        stc
        push   ecx
        push   esi
        jmp    SHORT loc_3_15C2
loc_3_15A2:
        add    edi, 0ch
        jmp    SHORT loc_3_157A
loc_3_15A7:
        push   ecx
        push   esi
        lea    esi, [edi]
        call   sub_3_0DF7
        jae    SHORT loc_3_15EF
        or     word ptr [esi + 0bh], 1
        pop    esi
        push   esi
        call   sub_3_146A
        jae    SHORT loc_3_15EF
        lea    esi, [edi]
loc_3_15C2:
        pushfd
        xor    cl, cl
        xchg   byte ptr [esi + 8], cl
        pushfd
        push   esi
        db  068h, 0A1h, 03Dh, 000h, 000h
        db  0CDh, 020h, 010h, 000h, 010h, 000h
        add    esp, 8
        popfd
        xchg   byte ptr [esi + 8], cl
        mov    cl, 1
        db  0F6h, 005h, 01Bh, 022h, 000h, 000h, 080h
        je     SHORT loc_3_15E9
        inc    cl
loc_3_15E9:
        call   sub_3_11C4
        popfd
loc_3_15EF:
        pop    esi
        pop    ecx
        jb     SHORT loc_3_165E
loc_3_15F3:
        cmp    eax, 3
        je     NEAR PTR loc_3_1537
        cmp    eax, 5
        jne    SHORT loc_3_1603
        xor    eax, eax
loc_3_1603:
        db  00Fh, 0ABh, 005h, 03Fh, 03Ah, 000h, 000h
        db  083h, 03Dh, 03Fh, 03Ah, 000h, 000h, 003h
        jbe    NEAR PTR loc_3_1537
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        or     dword ptr [eax + 28h], 8
        db  083h, 03Dh, 03Fh, 03Ah, 000h, 000h, 005h
        ja     SHORT loc_3_164E
        pushfd
        db  068h, 0BCh, 03Dh, 000h, 000h
        db  0CDh, 020h, 010h, 000h, 010h, 000h
        add    esp, 4
        popfd
        pushfd
        db  068h, 0F1h, 03Dh, 000h, 000h
        db  0CDh, 020h, 010h, 000h, 010h, 000h
        add    esp, 4
        popfd
        jmp    NEAR PTR loc_3_1537
loc_3_164E:
        pushfd
        db  068h, 032h, 03Eh, 000h, 000h
        db  0CDh, 020h, 010h, 000h, 010h, 000h
        add    esp, 4
        popfd
loc_3_165E:
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        or     dword ptr [eax + 28h], 2
loc_3_1667:
        pop    ecx
loc_3_1668:
        ret
sub_3_1669:
        pushad
        db  08Bh, 03Dh, 00Bh, 03Ah, 000h, 000h
        movzx  ecx, byte ptr [edi + 5]
        lea    edi, [edi + 6]
loc_3_1677:
        push   ecx
        movzx  ecx, byte ptr [edi + 0eh]
        lea    esi, [edi + 0fh]
loc_3_167F:
        cmp    byte ptr [esi], 2
        je     SHORT loc_3_1695
        cmp    byte ptr [esi], 3
        je     SHORT loc_3_1695
loc_3_1689:
        add    esi, 5
        loop   loc_3_167F
        mov    edi, esi
        pop    ecx
        loop   loc_3_1677
        popad
        ret
loc_3_1695:
        push   ecx
        push   esi
        lea    esi, [edi]
        cmp    dword ptr [esi], 44494350h
        jne    SHORT loc_3_16AA
        cmp    dword ptr [esi + 4], 20205652h
        je     SHORT loc_3_16BB
loc_3_16AA:
        cmp    dword ptr [esi], 44534f44h
        jne    SHORT loc_3_16F0
        cmp    dword ptr [esi + 4], 20205652h
        jne    SHORT loc_3_16F0
loc_3_16BB:
        db  083h, 03Dh, 003h, 022h, 000h, 000h, 000h
        jne    SHORT loc_3_16F0
        db  080h, 00Dh, 044h, 022h, 000h, 000h, 004h
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        or     byte ptr [ecx + 28h], 2
        or     word ptr [esi + 0bh], 1
        db  0E8h, 04Dh, 0EFh, 0FCh, 0FFh
        pushfd
        push   esi
        db  068h, 06Ch, 03Eh, 000h, 000h
        db  0CDh, 020h, 010h, 000h, 010h, 000h
        add    esp, 8
        popfd
loc_3_16F0:
        pop    esi
        pop    ecx
        jmp    SHORT loc_3_1689
sub_3_16F4:
        sub    esp, 8
        call   sub_3_19C3
        db  08Bh, 03Dh, 00Bh, 03Ah, 000h, 000h
        movzx  ebx, byte ptr [edi + 5]
        add    edi, 6
        db  0C6h, 005h, 03Eh, 001h, 000h, 000h, 000h
loc_3_1710:
        or     ebx, ebx
        je     NEAR PTR loc_3_1824
        cmp    byte ptr [edi + 0dh], 0
        je     NEAR PTR loc_3_1828
        mov    esi, edi
        mov    dword ptr [esp + 4], 0
        cmp    dword ptr [esi], 49505341h
        jne    SHORT loc_3_1747
        cmp    dword ptr [esi + 4], 4b534944h
        jne    SHORT loc_3_1747
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        or     byte ptr [ecx + 2ah], 10h
loc_3_1747:
        call   sub_3_0DF7
        jb     SHORT loc_3_1777
        mov    dword ptr [esp + 4], 1
        cmp    eax, 3
        jne    SHORT loc_3_1777
loc_3_175B:
        mov    al, byte ptr [edi + 0dh]
        db  000h, 005h, 03Eh, 001h, 000h, 000h
        movzx  eax, byte ptr [edi + 0eh]
        mov    ecx, 5
        mul    ecx
        add    eax, 0fh
        add    edi, eax
        dec    ebx
        jmp    SHORT loc_3_1710
loc_3_1777:
        mov    dword ptr [esp], eax
        db  0A0h, 03Eh, 001h, 000h, 000h
        mov    ah, byte ptr [edi + 0dh]
loc_3_1782:
        db  0E8h, 039h, 004h, 0FDh, 0FFh
        jb     SHORT loc_3_17FD
        cmp    dword ptr [esp + 4], 0
        jne    SHORT loc_3_180C
        or     word ptr [edi + 0bh], 1
        push   ecx
        xor    cl, cl
        xchg   byte ptr [edi + 8], cl
        pushfd
        push   eax
        push   edi
        db  068h, 0ACh, 03Eh, 000h, 000h
        db  0CDh, 020h, 010h, 000h, 010h, 000h
        add    esp, 0ch
        popfd
        xchg   byte ptr [edi + 8], cl
        pop    ecx
        or     word ptr [esi + 10h], 4
        call   sub_3_3537
        cmp    byte ptr [esi], 1
        jbe    SHORT loc_3_17FD
        cmp    byte ptr [esi + 0ah], 0ffh
        jne    SHORT loc_3_17FD
        cmp    byte ptr [esi + 0dh], 0ffh
        jne    SHORT loc_3_17FD
        cmp    byte ptr [esi + 1], 0ffh
        jne    SHORT loc_3_17FD
        push   ecx
        xor    cl, cl
        xchg   byte ptr [edi + 8], cl
        pushfd
        push   eax
        push   edi
        db  068h, 0D3h, 03Eh, 000h, 000h
        db  0CDh, 020h, 010h, 000h, 010h, 000h
        add    esp, 0ch
        popfd
        xchg   byte ptr [edi + 8], cl
        xchg   edi, esi
        mov    cl, 1
        call   sub_3_11C4
        xchg   edi, esi
        pop    ecx
loc_3_17F9:
        or     dword ptr [ecx + 28h], 2
loc_3_17FD:
        inc    al
        dec    ah
        jne    NEAR PTR loc_3_1782
        jmp    NEAR PTR loc_3_175B
loc_3_180C:
        cmp    dword ptr [esp], 4
        jne    SHORT loc_3_1816
        or     byte ptr [esi + 10h], 10h
loc_3_1816:
        movzx  edx, word ptr [esi + 10h]
        and    edx, 12h
        cmp    edx, 12h
        je     SHORT loc_3_17F9
        jmp    SHORT loc_3_17FD
loc_3_1824:
        add    esp, 8
        ret
loc_3_1828:
        mov    cl, byte ptr [edi + 0eh]
        or     cl, cl
        je     SHORT loc_3_185B
        push   edi
        add    edi, 0fh
loc_3_1833:
        mov    al, byte ptr [edi]
        cmp    al, 6
        je     SHORT loc_3_185A
        cmp    al, 5
        je     SHORT loc_3_185A
        cmp    al, 7
        je     SHORT loc_3_185A
        cmp    al, 2
        je     SHORT loc_3_185A
        cmp    al, 3
        je     SHORT loc_3_185A
        dec    cl
        je     SHORT loc_3_1852
        add    edi, 5
        jmp    SHORT loc_3_1833
loc_3_1852:
        pop    edi
        call   sub_3_3537
        jmp    SHORT loc_3_185B
loc_3_185A:
        pop    edi
loc_3_185B:
        lea    esi, [edi]
        call   sub_3_0E72
        jb     NEAR PTR loc_3_175B
        or     word ptr [edi + 0bh], 1
        xor    cl, cl
        xchg   byte ptr [edi + 8], cl
        pushfd
        push   edi
        db  068h, 0FEh, 03Eh, 000h, 000h
        db  0CDh, 020h, 010h, 000h, 010h, 000h
        add    esp, 8
        popfd
        xchg   byte ptr [edi + 8], cl
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        or     byte ptr [ecx + 29h], 8
        jmp    NEAR PTR loc_3_175B
sub_3_1895:
        db  08Bh, 015h, 0C7h, 021h, 000h, 000h
        or     edx, edx
        je     SHORT loc_3_18DB
loc_3_189F:
        bsf    eax, edx
        je     SHORT loc_3_18DB
        btr    edx, eax
        db  0E8h, 014h, 003h, 0FDh, 0FFh
        jb     SHORT loc_3_189F
        mov    edi, esi
        db  00Fh, 0B6h, 080h, 0CFh, 021h, 000h, 000h
        db  0E8h, 004h, 003h, 0FDh, 0FFh
        jb     SHORT loc_3_189F
        mov    bx, word ptr [esi + 10h]
        mov    cx, word ptr [edi + 10h]
        and    bx, 0fff5h
        and    cx, 0fff5h
        or     cx, bx
        or     word ptr [esi + 10h], cx
        or     word ptr [edi + 10h], cx
        jmp    SHORT loc_3_189F
loc_3_18DB:
        ret
sub_3_18DC:
        xor    eax, eax
loc_3_18DE:
        mov    al, 7
        call   sub_3_2F84
        jb     SHORT loc_3_1916
        db  08Bh, 03Dh, 01Fh, 03Ah, 000h, 000h
        mov    edi, dword ptr [edi + 1]
        db  08Bh, 035h, 027h, 03Ah, 000h, 000h
        call   sub_3_0DF7
        jae    SHORT loc_3_1912
        or     word ptr [esi + 0bh], 1
        call   sub_3_1982
        db  08Bh, 03Dh, 027h, 03Ah, 000h, 000h
        call   sub_3_3537
loc_3_1912:
        mov    ah, 1
        jmp    SHORT loc_3_18DE
loc_3_1916:
        db  08Bh, 03Dh, 0C7h, 021h, 000h, 000h
        or     edi, edi
        je     SHORT loc_3_1931
        db  0BEh, 05Ch, 03Ah, 000h, 000h
        call   sub_3_0DF7
        jae    SHORT loc_3_1931
        call   sub_3_1982
loc_3_1931:
        db  08Bh, 01Dh, 068h, 03Ah, 000h, 000h
        cmp    word ptr [ebx + 0feh], -1
        je     SHORT loc_3_1981
        lea    ebx, [ebx + 0eh]
loc_3_1944:
        movzx  esi, word ptr [ebx]
        or     esi, esi
        je     SHORT loc_3_1981
        mov    edi, dword ptr [ebx + 4]
        and    edi, 1fffffffh
        or     edi, edi
        je     SHORT loc_3_197C
        dec    esi
        shl    esi, 4
        lea    esi, [esi + 8]
        call   sub_3_10FC
        jae    SHORT loc_3_197C
        or     byte ptr [ebx + 7], 20h
        mov    cx, word ptr [ebx + 2]
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        or     word ptr [eax + 6ch], cx
        call   sub_3_1982
loc_3_197C:
        add    ebx, 0ch
        jmp    SHORT loc_3_1944
loc_3_1981:
        ret
sub_3_1982:
        xor    cl, cl
        xchg   byte ptr [esi + 8], cl
        pushfd
        push   esi
        db  068h, 036h, 03Fh, 000h, 000h
        db  0CDh, 020h, 010h, 000h, 010h, 000h
        add    esp, 8
        popfd
        xchg   byte ptr [esi + 8], cl
loc_3_199B:
        bsf    eax, edi
        je     SHORT loc_3_19C2
        btr    edi, eax
        pushfd
        push   eax
        db  068h, 06Ah, 03Fh, 000h, 000h
        db  0CDh, 020h, 010h, 000h, 010h, 000h
        add    esp, 8
        popfd
        db  0E8h, 007h, 002h, 0FDh, 0FFh
        jb     SHORT loc_3_199B
        or     word ptr [esi + 10h], 8
        jmp    SHORT loc_3_199B
loc_3_19C2:
        ret
sub_3_19C3:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    esi, dword ptr [ebx + 8]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    byte ptr [esi + 1dh], 52h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  ebx, word ptr [esi + 38h]
        shl    ebx, 4
        movzx  ecx, word ptr [esi + 10h]
        add    ebx, ecx
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        movzx  eax, word ptr [ebx + 16h]
        movzx  edx, word ptr [ebx + 18h]
        shl    edx, 4
        add    eax, edx
        db  0BFh, 0BEh, 000h, 000h, 000h
        mov    edx, 58h
        movzx  ecx, byte ptr [ebx + 21h]
        db  088h, 00Dh, 042h, 003h, 000h, 000h
loc_3_1A33:
        stosd
        add    eax, edx
        loop   loc_3_1A33
        ret
sub_3_1A39:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 1690h
        mov    word ptr [ebp + 10h], 0
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        xor    ebx, ebx
        test   word ptr [ebp + 2ch], 1
        jne    SHORT loc_3_1A81
        mov    di, word ptr [ebp + 38h]
        rol    edi, 10h
        mov    di, word ptr [ebp + 10h]
        call   sub_3_2F6A
        mov    ebx, edi
loc_3_1A81:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  08Bh, 03Dh, 09Ch, 01Fh, 000h, 000h
        mov    dword ptr [edi + 4ch], ebx
        mov    word ptr [edi + 24h], 0
        db  0CDh, 020h, 0BBh, 000h, 001h, 000h
        test   ebx, 40000h
        je     SHORT loc_3_1ABA
        or     word ptr [edi + 24h], 4
        jmp    SHORT loc_3_1ADB
loc_3_1ABA:
        or     word ptr [edi + 24h], 2
        test   ebx, 80000h
        jne    SHORT loc_3_1ACC
        xor    word ptr [edi + 24h], 3
loc_3_1ACC:
        or     word ptr [edi + 24h], 9
        cmp    bl, 0fch
        jne    SHORT loc_3_1ADB
        and    word ptr [edi + 24h], 0fff7h
loc_3_1ADB:
        ret
sub_3_1ADC:
        mov    eax, 2ah
        db  0BEh, 0F0h, 02Fh, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        call   sub_3_19C3
        call   sub_3_33F4
        mov    ax, 3d00h
        db  08Dh, 015h, 0E9h, 039h, 000h, 000h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_3_1B2F
        push   eax
        mov    bx, ax
        mov    ax, 4402h
        db  08Dh, 015h, 0F2h, 039h, 000h, 000h
        mov    cx, 4
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        mov    ah, 3eh
        pop    ebx
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
loc_3_1B2F:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ecx, 924h
        clc
        db  0CDh, 020h, 008h, 000h, 006h, 000h
        cmp    ecx, 924h
        db  00Fh, 085h, 0A4h, 006h, 0FDh, 0FFh
        db  089h, 03Dh, 0FFh, 039h, 000h, 000h
        mov    esi, edi
        and    esi, 0ffff0000h
        shr    esi, 0ch
        movzx  eax, di
        add    esi, eax
        db  089h, 035h, 0FBh, 039h, 000h, 000h
        mov    word ptr [ebp + 1ch], 1607h
        mov    word ptr [ebp + 10h], 10h
        mov    word ptr [ebp + 18h], 1
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    ah, 19h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        push   eax
        mov    dl, 2
loc_3_1BA8:
        mov    ah, 0eh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        push   eax
        mov    ah, 0dh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    eax
        inc    dl
        cmp    dl, al
        jb     SHORT loc_3_1BA8
        pop    edx
        mov    ah, 0eh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        mov    byte ptr [ebp + 1dh], 52h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  edx, word ptr [ebp + 38h]
        shl    edx, 4
        inc    byte ptr [edx + 321h]
        push   edx
        mov    eax, 13h
        db  0BEh, 01Eh, 030h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        mov    eax, 4fh
        db  0BEh, 02Eh, 031h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        push   0
        push   1d5h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        db  00Fh, 084h, 0E2h, 005h, 0FDh, 0FFh
        push   eax
        db  0A3h, 033h, 03Ah, 000h, 000h
        push   edi
        mov    edi, eax
        mov    al, 0ffh
        mov    ecx, 1d5h
        rep stosb
        pop    edi
        xor    ebx, ebx
loc_3_1C39:
        db  03Ah, 01Dh, 042h, 003h, 000h, 000h
        jae    SHORT loc_3_1CA5
        db  0A1h, 033h, 03Ah, 000h, 000h
        db  08Bh, 00Ch, 09Dh, 0BEh, 000h, 000h, 000h
        mov    dh, byte ptr [ecx + 44h]
        test   dh, 40h
        je     SHORT loc_3_1CA1
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        bts    dword ptr [ecx + 44h], ebx
        test   dh, 80h
        jne    SHORT loc_3_1CA1
        test   dh, 10h
        je     SHORT loc_3_1C8A
        db  08Bh, 00Dh, 025h, 022h, 000h, 000h
        mov    ecx, dword ptr [ecx + 26h]
loc_3_1C72:
        xchg   edi, ecx
        call   sub_3_2F6A
        xchg   edi, ecx
        cmp    byte ptr [ecx], bl
        je     SHORT loc_3_1C8A
        mov    ecx, dword ptr [ecx + 19h]
        cmp    cx, -1
        je     SHORT loc_3_1CA1
        jmp    SHORT loc_3_1C72
loc_3_1C8A:
        mov    byte ptr [eax], bl
        cmp    bl, 1
        ja     SHORT loc_3_1C94
        mov    byte ptr [eax + 1], bl
loc_3_1C94:
        mov    word ptr [eax + 10h], 0
        db  083h, 005h, 033h, 03Ah, 000h, 000h, 012h
loc_3_1CA1:
        inc    bl
        jmp    SHORT loc_3_1C39
loc_3_1CA5:
        db  0A1h, 033h, 03Ah, 000h, 000h
        inc    eax
        pop    ebx
        sub    eax, ebx
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 050h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        db  00Fh, 084h, 043h, 005h, 0FDh, 0FFh
        db  08Bh, 01Dh, 09Ch, 01Fh, 000h, 000h
        mov    dword ptr [ebx + 34h], eax
        db  0A3h, 033h, 03Ah, 000h, 000h
loc_3_1CD1:
        db  08Bh, 035h, 033h, 03Ah, 000h, 000h
        mov    bl, byte ptr [esi]
        cmp    bl, 0ffh
        je     NEAR PTR loc_3_1F1F
        cmp    bl, 1
        jbe    NEAR PTR loc_3_1F01
        db  088h, 01Dh, 03Eh, 001h, 000h, 000h
        db  083h, 03Dh, 0F2h, 039h, 000h, 000h, 000h
        je     SHORT loc_3_1D22
        db  080h, 03Dh, 0F6h, 039h, 000h, 000h, 000h
        jne    SHORT loc_3_1D22
        db  0A1h, 0F2h, 039h, 000h, 000h
        xor    edx, edx
        db  0BEh, 0C6h, 030h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 001h, 000h
        db  0C6h, 005h, 0F6h, 039h, 000h, 000h, 001h
        db  08Bh, 035h, 033h, 03Ah, 000h, 000h
loc_3_1D22:
        db  00Fh, 0B6h, 005h, 03Eh, 001h, 000h, 000h
        mov    word ptr [ebp + 8], ax
        mov    word ptr [ebp + 1ch], 4a10h
        mov    word ptr [ebp + 10h], 3
        mov    word ptr [ebp + 14h], 0
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  080h, 025h, 01Ch, 022h, 000h, 000h, 0FEh
        cmp    byte ptr [ebp + 14h], 0ffh
        je     SHORT loc_3_1D8C
        test   byte ptr [ebp + 14h], 80h
        jne    SHORT loc_3_1D8C
        db  080h, 00Dh, 01Ch, 022h, 000h, 000h, 001h
        db  00Fh, 0B6h, 005h, 03Eh, 001h, 000h, 000h
        mov    word ptr [ebp + 8], ax
        mov    word ptr [ebp + 1ch], 4a10h
        mov    word ptr [ebp + 10h], 3
        mov    word ptr [ebp + 14h], 2
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
loc_3_1D8C:
        db  080h, 025h, 01Bh, 022h, 000h, 000h, 0FBh
        db  0C7h, 005h, 043h, 03Ah, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_3_212F
        db  00Fh, 0BAh, 035h, 01Bh, 022h, 000h, 000h, 00Dh
        jae    SHORT loc_3_1DB9
        call   sub_3_1F97
        db  08Bh, 035h, 033h, 03Ah, 000h, 000h
        jmp    SHORT loc_3_1D8C
loc_3_1DB9:
        push   dword ptr [ebp + 2ch]
        db  0F6h, 005h, 01Ch, 022h, 000h, 000h, 001h
        je     SHORT loc_3_1DED
        db  00Fh, 0B6h, 005h, 03Eh, 001h, 000h, 000h
        mov    word ptr [ebp + 8], ax
        mov    word ptr [ebp + 1ch], 4a10h
        mov    word ptr [ebp + 10h], 3
        mov    word ptr [ebp + 14h], 1
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
loc_3_1DED:
        pop    dword ptr [ebp + 2ch]
        test   word ptr [ebp + 2ch], 1
        je     NEAR PTR loc_3_1E98
        db  00Fh, 0B6h, 005h, 03Eh, 001h, 000h, 000h
        mov    edx, dword ptr [ebp + 1ch]
        pushfd
        push   edx
        push   eax
        db  068h, 07Eh, 03Fh, 000h, 000h
        db  0CDh, 020h, 010h, 000h, 010h, 000h
        add    esp, 0ch
        popfd
        mov    word ptr [esi + 10h], 2
        jmp    NEAR PTR loc_3_1F01
loc_3_1E23:
        db  00Fh, 0B6h, 005h, 03Eh, 001h, 000h, 000h
        inc    eax
        mov    word ptr [ebp + 10h], ax
        mov    eax, edi
        add    ax, 800h
        mov    word ptr [ebp + 14h], ax
        mov    byte ptr [ebp + 19h], 8
        mov    byte ptr [ebp + 18h], 60h
        rol    eax, 10h
        mov    word ptr [ebp + 3ch], ax
        mov    word ptr [ebp + 1ch], 440dh
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        test   word ptr [ebp + 2ch], 1
        jne    NEAR PTR loc_3_1F01
        db  0A1h, 0FBh, 039h, 000h, 000h
        add    eax, 800h
        movzx  edx, word ptr [eax + 0fh]
        or     edx, edx
        jne    SHORT loc_3_1E7A
        mov    edx, dword ptr [eax + 1ch]
loc_3_1E7A:
        shr    edx, 1
        db  089h, 015h, 043h, 03Ah, 000h, 000h
loc_3_1E82:
        call   sub_3_212F
        db  00Fh, 0BAh, 035h, 01Bh, 022h, 000h, 000h, 00Dh
        jae    SHORT loc_3_1F01
        call   sub_3_1F97
        jmp    SHORT loc_3_1E82
loc_3_1E98:
        mov    word ptr [esi + 10h], 0
        db  08Bh, 01Dh, 0FBh, 039h, 000h, 000h
        cmp    dword ptr [ebx + 1f0h], 14cd13cdh
        jne    SHORT loc_3_1ED0
        cmp    dword ptr [ebx + 1f4h], 3cd01cdh
        jne    SHORT loc_3_1ED0
        mov    al, byte ptr [ebx + 4ch]
        sub    al, 41h
        push   ebx
        db  00Fh, 0B6h, 01Dh, 03Eh, 001h, 000h, 000h
        db  088h, 083h, 0E9h, 021h, 000h, 000h
        pop    ebx
loc_3_1ED0:
        cmp    word ptr [ebx + 16h], 0
        jne    SHORT loc_3_1EE4
        cmp    byte ptr [ebx + 42h], 29h
        jne    SHORT loc_3_1F0D
        mov    eax, dword ptr [ebx + 43h]
        or     eax, eax
        jne    SHORT loc_3_1EF1
loc_3_1EE4:
        cmp    byte ptr [ebx + 26h], 29h
        jne    SHORT loc_3_1F0D
        mov    eax, dword ptr [ebx + 27h]
        or     eax, eax
        je     SHORT loc_3_1F0D
loc_3_1EF1:
        mov    dword ptr [esi + 2], eax
        db  0F6h, 005h, 01Bh, 022h, 000h, 000h, 004h
        je     NEAR PTR loc_3_1E23
loc_3_1F01:
        db  083h, 005h, 033h, 03Ah, 000h, 000h, 012h
        jmp    NEAR PTR loc_3_1CD1
loc_3_1F0D:
        xor    eax, eax
        mov    ecx, 200h
loc_3_1F14:
        add    eax, dword ptr [ecx + ebx - 4]
        sub    ecx, 4
        jne    SHORT loc_3_1F14
        jmp    SHORT loc_3_1EF1
loc_3_1F1F:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ecx, 924h
        clc
        db  0CDh, 020h, 009h, 000h, 006h, 000h
        mov    eax, 2ah
        db  0BEh, 0F0h, 02Fh, 000h, 000h
        db  0CDh, 020h, 018h, 001h, 001h, 000h
        mov    eax, 4fh
        db  0BEh, 02Eh, 031h, 000h, 000h
        db  0CDh, 020h, 018h, 001h, 001h, 000h
        db  080h, 03Dh, 0F6h, 039h, 000h, 000h, 000h
        je     SHORT loc_3_1F65
        db  0A1h, 0F2h, 039h, 000h, 000h
        db  0CDh, 020h, 00Ah, 000h, 001h, 000h
loc_3_1F65:
        mov    eax, 13h
        db  0BEh, 01Eh, 030h, 000h, 000h
        db  0CDh, 020h, 018h, 001h, 001h, 000h
        call   sub_3_346B
        pop    ebx
        dec    byte ptr [ebx + 321h]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        ret
sub_3_1F97:
        sub    esp, 4
        db  08Bh, 035h, 09Ch, 01Fh, 000h, 000h
        mov    esi, dword ptr [esi + 2ch]
        or     esi, esi
        je     NEAR PTR loc_3_212B
loc_3_1FAB:
        cmp    byte ptr [esi], 2
        jne    NEAR PTR loc_3_211D
        mov    dl, byte ptr [esi + 5]
        mov    byte ptr [esp], dl
        mov    word ptr [ebp], 0
        mov    word ptr [ebp + 38h], 0
        mov    byte ptr [ebp + 1dh], 8
        mov    byte ptr [ebp + 14h], dl
        mov    eax, 13h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 18h]
        mov    edx, dword ptr [ebp + 14h]
        mov    di, word ptr [ebp + 38h]
        rol    edi, 10h
        mov    di, word ptr [ebp]
        mov    eax, dword ptr [ebp + 2ch]
        rcr    eax, 1
        jb     NEAR PTR loc_3_212B
        db  0FEh, 005h, 024h, 022h, 000h, 000h
        mov    byte ptr [esi], 0
        mov    al, cl
        xchg   ch, cl
        and    ch, 0c0h
        shr    ch, 6
        inc    ecx
        mov    word ptr [esi + 6], cx
        and    eax, 3fh
        mov    word ptr [esi + 0ah], ax
        movzx  eax, dh
        inc    eax
        mov    word ptr [esi + 8], ax
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        mov    eax, dword ptr [eax + 38h]
loc_3_2026:
        cmp    byte ptr [eax], 0ffh
        je     SHORT loc_3_2030
        add    eax, 0bh
        jmp    SHORT loc_3_2026
loc_3_2030:
        mov    byte ptr [eax + 0bh], 0ffh
        db  0A3h, 02Fh, 03Ah, 000h, 000h
        db  08Bh, 03Dh, 0FFh, 039h, 000h, 000h
        mov    word ptr [ebp + 1ch], 201h
        rol    edi, 10h
        mov    word ptr [ebp + 38h], di
        rol    edi, 10h
        mov    word ptr [ebp + 10h], di
        mov    word ptr [ebp + 18h], 1
        movzx  edx, byte ptr [esp]
        mov    word ptr [ebp + 14h], dx
        mov    eax, 13h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        test   word ptr [ebp + 2ch], 1
        jne    NEAR PTR loc_3_212B
        db  0A1h, 02Fh, 03Ah, 000h, 000h
        mov    word ptr [eax + 9], 0
        mov    cl, byte ptr [esp]
        mov    byte ptr [eax], cl
        db  08Bh, 035h, 0FBh, 039h, 000h, 000h
        cmp    word ptr [esi + 0dah], 0
        jne    SHORT loc_3_20FE
        cmp    dword ptr [esi + 0dch], 0
        jne    SHORT loc_3_20EE
        mov    ah, 2
        push   1ah
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        rol    ecx, 10h
        mov    ch, dh
        mov    cl, byte ptr [esp]
        mov    dword ptr [esi + 0dch], ecx
        mov    word ptr [ebp + 1ch], 301h
        rol    edi, 10h
        mov    word ptr [ebp + 38h], di
        rol    edi, 10h
        mov    word ptr [ebp + 10h], di
        mov    word ptr [ebp + 18h], 1
        movzx  edx, byte ptr [esp]
        mov    word ptr [ebp + 14h], dx
        mov    eax, 13h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        test   word ptr [ebp + 2ch], 1
        jne    SHORT loc_3_20FE
loc_3_20EE:
        db  0A1h, 02Fh, 03Ah, 000h, 000h
        mov    ecx, dword ptr [esi + 0dch]
        mov    dword ptr [eax + 1], ecx
        jmp    SHORT loc_3_212B
loc_3_20FE:
        xor    edx, edx
        mov    ecx, 200h
loc_3_2105:
        add    edx, dword ptr [ecx + esi - 4]
        sub    ecx, 4
        jne    SHORT loc_3_2105
        db  0A1h, 02Fh, 03Ah, 000h, 000h
        or     word ptr [eax + 9], 1
        mov    dword ptr [eax + 5], edx
        jmp    SHORT loc_3_20EE
loc_3_211D:
        cmp    dword ptr [esi + 1], 0
        je     SHORT loc_3_212B
        add    esi, dword ptr [esi + 1]
        jmp    NEAR PTR loc_3_1FAB
loc_3_212B:
        add    esp, 4
        ret
sub_3_212F:
        db  080h, 00Dh, 01Ch, 022h, 000h, 000h, 008h
        db  0C6h, 005h, 057h, 03Ah, 000h, 000h, 000h
loc_3_213D:
        db  08Bh, 00Dh, 0FBh, 039h, 000h, 000h
        add    ecx, 800h
        mov    dword ptr [ecx + 6], edi
        db  0A1h, 043h, 03Ah, 000h, 000h
        mov    dword ptr [ecx], eax
        mov    word ptr [ecx + 4], 1
        mov    byte ptr [ebp + 1dh], 30h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  eax, word ptr [ebp + 1ch]
        xchg   ah, al
        cmp    ax, 70ah
        jae    SHORT loc_3_21A9
        db  00Fh, 0B6h, 005h, 03Eh, 001h, 000h, 000h
        mov    word ptr [ebp + 1ch], ax
        mov    eax, edi
        add    ax, 800h
        mov    word ptr [ebp + 10h], ax
        mov    word ptr [ebp + 18h], 0ffffh
        rol    eax, 10h
        mov    word ptr [ebp + 3ch], ax
        mov    eax, 25h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        jmp    SHORT loc_3_21E3
loc_3_21A9:
        db  00Fh, 0B6h, 005h, 03Eh, 001h, 000h, 000h
        inc    al
        mov    byte ptr [ebp + 14h], al
        mov    eax, edi
        add    ax, 800h
        mov    word ptr [ebp + 10h], ax
        mov    word ptr [ebp + 18h], 0ffffh
        rol    eax, 10h
        mov    word ptr [ebp + 3ch], ax
        mov    word ptr [ebp + 4], 0
        mov    word ptr [ebp + 1ch], 7305h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
loc_3_21E3:
        test   byte ptr [ebp + 2ch], 1
        je     SHORT loc_3_2237
        db  0FEh, 005h, 057h, 03Ah, 000h, 000h
        db  080h, 03Dh, 057h, 03Ah, 000h, 000h, 001h
        jne    SHORT loc_3_2237
        pushad
        db  0BAh, 0FDh, 001h, 000h, 000h
        mov    esi, edx
        add    esi, 1eh
        mov    ah, 1ah
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_3_2231
        call   sub_3_32CF
        db  0BAh, 079h, 001h, 000h, 000h
        db  08Ah, 00Dh, 03Eh, 001h, 000h, 000h
        add    cl, 41h
        mov    byte ptr [edx], cl
        xor    cx, cx
        mov    ah, 4eh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
loc_3_2231:
        popad
        jmp    NEAR PTR loc_3_213D
loc_3_2237:
        db  080h, 025h, 01Ch, 022h, 000h, 000h, 0F7h
        ret
sub_3_223F:
        db  0CDh, 020h, 000h, 000h, 041h, 000h
        jb     SHORT loc_3_228C
        db  0CDh, 020h, 001h, 000h, 041h, 000h
        cmp    word ptr [eax], 4
        jne    SHORT loc_3_228C
        test   byte ptr [eax + 4], 1
        je     SHORT loc_3_228C
        cmp    word ptr [eax + 8], 0
        je     SHORT loc_3_226A
        db  08Bh, 015h, 09Ch, 01Fh, 000h, 000h
        or     byte ptr [edx + 29h], 10h
loc_3_226A:
        lea    eax, [eax + 14h]
        db  08Bh, 015h, 09Ch, 01Fh, 000h, 000h
        xor    ecx, ecx
loc_3_2275:
        cmp    dword ptr [eax + 2], 0
        je     SHORT loc_3_2283
        bts    dword ptr [edx + 48h], ecx
        bts    dword ptr [edx + 44h], ecx
loc_3_2283:
        add    eax, 6
        inc    ecx
        cmp    cl, 1ah
        jb     SHORT loc_3_2275
loc_3_228C:
        ret
sub_3_228D:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        call   sub_3_33F4
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ecx, 94h
        clc
        db  0CDh, 020h, 008h, 000h, 006h, 000h
        cmp    ecx, 94h
        db  00Fh, 085h, 041h, 0FFh, 0FCh, 0FFh
        mov    esi, edi
        call   sub_3_2F6A
        db  089h, 03Dh, 0FBh, 039h, 000h, 000h
        db  089h, 035h, 0FFh, 039h, 000h, 000h
        add    edi, 24h
        db  089h, 03Dh, 003h, 03Ah, 000h, 000h
        mov    eax, 13ch
        mov    eax, dword ptr [eax]
        db  0A3h, 007h, 03Ah, 000h, 000h
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        mov    esi, dword ptr [eax + 4ch]
loc_3_22F5:
        or     esi, esi
        je     NEAR PTR loc_3_2C34
        mov    edi, dword ptr [esi]
        call   sub_3_2F6A
        cmp    byte ptr [edi], 7
        je     SHORT loc_3_2315
        mov    edi, dword ptr [esi + 4]
        call   sub_3_2F6A
        mov    esi, edi
        jmp    SHORT loc_3_22F5
loc_3_2315:
        db  089h, 03Dh, 00Bh, 03Ah, 000h, 000h
        push   1
        push   14h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        db  00Fh, 084h, 0D6h, 0FEh, 0FCh, 0FFh
        db  0A3h, 00Fh, 03Ah, 000h, 000h
        mov    eax, 4fh
        db  0BEh, 05Bh, 02Eh, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        db  080h, 00Dh, 01Bh, 022h, 000h, 000h, 001h
        db  0C6h, 005h, 019h, 03Ah, 000h, 000h, 000h
        xor    ah, ah
loc_3_2355:
        mov    al, 6
        call   sub_3_2F84
        jb     NEAR PTR loc_3_2707
        db  08Bh, 035h, 027h, 03Ah, 000h, 000h
        call   sub_3_0DF7
        jae    SHORT loc_3_2396
        pushfd
        or     word ptr [esi + 0bh], 1
        xor    cl, cl
        xchg   byte ptr [esi + 8], cl
        pushfd
        push   esi
        db  068h, 0A9h, 03Fh, 000h, 000h
        db  0CDh, 020h, 010h, 000h, 010h, 000h
        add    esp, 8
        popfd
        xchg   byte ptr [esi + 8], cl
        mov    cl, 1
        call   sub_3_11C4
        popfd
loc_3_2396:
        jb     NEAR PTR loc_3_2C34
        db  080h, 025h, 01Dh, 022h, 000h, 000h, 0FDh
        cmp    dword ptr [esi], 49505341h
        jne    SHORT loc_3_23BB
        cmp    dword ptr [esi + 4], 534f4432h
        jne    SHORT loc_3_23BB
        db  080h, 00Dh, 01Dh, 022h, 000h, 000h, 002h
loc_3_23BB:
        db  0C6h, 005h, 021h, 03Dh, 000h, 000h, 007h
        cmp    eax, 5
        jne    SHORT loc_3_23CE
        db  0C6h, 005h, 021h, 03Dh, 000h, 000h, 000h
loc_3_23CE:
        db  08Bh, 03Dh, 01Fh, 03Ah, 000h, 000h
        push   1
        push   1eh
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        db  00Fh, 084h, 01Dh, 0FEh, 0FCh, 0FFh
        db  08Bh, 00Dh, 013h, 03Ah, 000h, 000h
        db  0A3h, 013h, 03Ah, 000h, 000h
        mov    dword ptr [eax + 1], ecx
        mov    byte ptr [eax], 1
        mov    ebx, dword ptr [edi + 1]
        mov    dword ptr [eax + 9], ebx
        db  08Bh, 00Dh, 037h, 03Ah, 000h, 000h
        mov    dword ptr [eax + 0dh], ecx
        db  089h, 01Dh, 037h, 03Ah, 000h, 000h
        cmp    dword ptr [esi], 49505341h
        jne    SHORT loc_3_244C
        cmp    dword ptr [esi + 4], 204d4143h
        jne    SHORT loc_3_244C
        db  00Fh, 0BAh, 02Dh, 01Bh, 022h, 000h, 000h, 00Eh
        jb     SHORT loc_3_244C
        mov    eax, dword ptr [12ch]
        db  0A3h, 022h, 03Dh, 000h, 000h
        db  0BEh, 0B7h, 02Eh, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        jae    SHORT loc_3_2447
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
loc_3_2447:
        mov    dword ptr [12ch], eax
loc_3_244C:
        push   edi
        db  08Bh, 035h, 027h, 03Ah, 000h, 000h
        mov    ecx, 2
        db  08Dh, 03Dh, 012h, 003h, 000h, 000h
        rep movsd
        mov    ecx, 8
        sub    edi, ecx
        mov    al, 20h
        repne scasb
        jne    SHORT loc_3_2470
        mov    byte ptr [edi], 0
loc_3_2470:
        db  08Dh, 035h, 012h, 003h, 000h, 000h
        db  0A1h, 013h, 03Ah, 000h, 000h
        lea    edi, [eax + 15h]
        mov    ecx, 9
        call   sub_3_0EDA
        pop    edi
        db  039h, 01Dh, 0F2h, 039h, 000h, 000h
        jne    SHORT loc_3_2498
        db  080h, 00Dh, 01Bh, 022h, 000h, 000h, 002h
loc_3_2498:
        db  0C6h, 005h, 01Bh, 03Ah, 000h, 000h, 000h
loc_3_249F:
        db  0C6h, 005h, 017h, 03Ah, 000h, 000h, 000h
        db  0C6h, 005h, 018h, 03Ah, 000h, 000h, 000h
        call   sub_3_2D6D
        cmp    byte ptr [ebx + 1], 1
        jne    NEAR PTR loc_3_2355
        mov    al, byte ptr [ebx + 9]
        db  0A2h, 01Ah, 03Ah, 000h, 000h
        mov    al, byte ptr [ebx + 8]
        dec    al
        db  0A2h, 01Dh, 03Ah, 000h, 000h
loc_3_24CE:
        db  08Bh, 015h, 00Fh, 03Ah, 000h, 000h
        db  0A0h, 01Bh, 03Ah, 000h, 000h
        mov    byte ptr [edx + 0eh], al
        mov    byte ptr [edx + 4], 0
        db  08Bh, 01Dh, 003h, 03Ah, 000h, 000h
        mov    byte ptr [ebx], 1
        db  0A0h, 019h, 03Ah, 000h, 000h
        mov    byte ptr [ebx + 2], al
        mov    byte ptr [edx], al
        mov    byte ptr [ebx + 3], 0
        db  0A0h, 017h, 03Ah, 000h, 000h
        db  038h, 005h, 01Ah, 03Ah, 000h, 000h
        je     NEAR PTR loc_3_26B0
        mov    byte ptr [ebx + 8], al
        mov    byte ptr [edx + 1], al
        db  0A0h, 018h, 03Ah, 000h, 000h
        mov    byte ptr [ebx + 9], al
        mov    byte ptr [edx + 2], al
        db  080h, 025h, 01Bh, 022h, 000h, 000h, 0FBh
        db  066h, 0A1h, 001h, 03Ah, 000h, 000h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        db  066h, 0A1h, 0FFh, 039h, 000h, 000h
        add    ax, 24h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        movzx  ecx, word ptr [edi + 3]
        movzx  edx, word ptr [edi + 1]
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        cmp    byte ptr [ebx + 1], 81h
        mov    ah, 1
        je     NEAR PTR loc_3_2355
        cmp    byte ptr [ebx + 1], 1
        je     SHORT loc_3_2579
        jmp    NEAR PTR loc_3_26B0
loc_3_2579:
        db  0F6h, 005h, 01Bh, 022h, 000h, 000h, 004h
        jne    NEAR PTR loc_3_266A
        mov    byte ptr [ebx], 2
        db  0A0h, 019h, 03Ah, 000h, 000h
        mov    byte ptr [ebx + 2], al
        db  0A0h, 017h, 03Ah, 000h, 000h
        mov    byte ptr [ebx + 8], al
        db  0A0h, 018h, 03Ah, 000h, 000h
        mov    byte ptr [ebx + 9], al
        mov    dword ptr [ebx + 0ah], 24h
        mov    byte ptr [ebx + 0eh], 0
        mov    byte ptr [ebx + 3], 0
        mov    byte ptr [ebx + 17h], 6
        mov    byte ptr [ebx + 40h], 12h
        db  0A0h, 018h, 03Ah, 000h, 000h
        shl    al, 5
        mov    byte ptr [ebx + 41h], al
        mov    word ptr [ebx + 42h], 0
        mov    byte ptr [ebx + 44h], 24h
        mov    byte ptr [ebx + 45h], 0
        db  0A1h, 0FFh, 039h, 000h, 000h
        mov    dword ptr [ebx + 0fh], eax
        db  066h, 0A1h, 001h, 03Ah, 000h, 000h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        db  066h, 0A1h, 0FFh, 039h, 000h, 000h
        add    ax, 24h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        movzx  ecx, word ptr [edi + 3]
        movzx  edx, word ptr [edi + 1]
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
loc_3_260F:
        cmp    byte ptr [ebx + 1], 0
        jne    SHORT loc_3_261D
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        jmp    SHORT loc_3_260F
loc_3_261D:
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        db  0F6h, 005h, 01Bh, 022h, 000h, 000h, 004h
        jne    SHORT loc_3_266A
        cmp    byte ptr [ebx + 1], 1
        jne    SHORT loc_3_2699
        db  080h, 025h, 01Dh, 022h, 000h, 000h, 0FDh
        db  08Bh, 01Dh, 0FBh, 039h, 000h, 000h
        test   byte ptr [ebx], 0e0h
        jne    SHORT loc_3_26B0
        db  08Bh, 015h, 00Fh, 03Ah, 000h, 000h
        mov    al, byte ptr [ebx]
        mov    byte ptr [edx + 0fh], al
        xor    eax, eax
        mov    al, byte ptr [ebx]
        mov    ecx, 23h
loc_3_265E:
        add    al, byte ptr [ecx + ebx]
        adc    ah, 0
        loop   loc_3_265E
        mov    word ptr [edx + 0ch], ax
loc_3_266A:
        db  08Bh, 015h, 00Fh, 03Ah, 000h, 000h
        db  08Bh, 01Dh, 013h, 03Ah, 000h, 000h
        mov    eax, dword ptr [ebx + 5]
        mov    dword ptr [edx + 10h], eax
        mov    dword ptr [ebx + 5], edx
        push   1
        push   14h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        db  00Fh, 084h, 072h, 0FBh, 0FCh, 0FFh
        db  0A3h, 00Fh, 03Ah, 000h, 000h
loc_3_2699:
        db  0FEh, 005h, 018h, 03Ah, 000h, 000h
        db  0A0h, 021h, 03Dh, 000h, 000h
        db  038h, 005h, 018h, 03Ah, 000h, 000h
        jbe    NEAR PTR loc_3_24CE
loc_3_26B0:
        db  0C6h, 005h, 018h, 03Ah, 000h, 000h, 000h
        db  0FEh, 005h, 017h, 03Ah, 000h, 000h
        db  080h, 03Dh, 017h, 03Ah, 000h, 000h, 007h
        jbe    NEAR PTR loc_3_24CE
        db  0F6h, 005h, 01Dh, 022h, 000h, 000h, 002h
        je     SHORT loc_3_26DC
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        or     byte ptr [eax + 2ah], 80h
loc_3_26DC:
        db  0C6h, 005h, 017h, 03Ah, 000h, 000h, 000h
        db  0FEh, 005h, 019h, 03Ah, 000h, 000h
        db  0FEh, 005h, 01Bh, 03Ah, 000h, 000h
        mov    ah, 1
        db  0A0h, 019h, 03Ah, 000h, 000h
        db  03Ah, 005h, 01Dh, 03Ah, 000h, 000h
        ja     NEAR PTR loc_3_2355
        jmp    NEAR PTR loc_3_249F
loc_3_2707:
        mov    eax, 4fh
        db  0BEh, 05Bh, 02Eh, 000h, 000h
        db  0CDh, 020h, 018h, 001h, 001h, 000h
        db  080h, 025h, 01Bh, 022h, 000h, 000h, 0FEh
        db  00Fh, 0BAh, 035h, 01Bh, 022h, 000h, 000h, 00Eh
        jae    SHORT loc_3_2732
        db  0A1h, 022h, 03Dh, 000h, 000h
        mov    dword ptr [12ch], eax
loc_3_2732:
        call   sub_3_2D04
        db  080h, 00Dh, 01Bh, 022h, 000h, 000h, 008h
        xor    ah, ah
        db  0C6h, 005h, 019h, 03Ah, 000h, 000h, 000h
        db  0C7h, 005h, 023h, 03Ah, 000h, 000h, 000h, 000h, 000h, 000h
loc_3_2751:
        mov    al, 5
        call   sub_3_2F84
        jb     SHORT loc_3_2764
        db  089h, 03Dh, 023h, 03Ah, 000h, 000h
        mov    ah, 1
        jmp    SHORT loc_3_2751
loc_3_2764:
        db  0C6h, 005h, 019h, 03Ah, 000h, 000h, 0FFh
        call   sub_3_2DC1
        mov    al, byte ptr [ebx + 2ch]
        db  0A2h, 01Dh, 03Ah, 000h, 000h
loc_3_2778:
        db  08Bh, 01Dh, 023h, 03Ah, 000h, 000h
        or     ebx, ebx
        je     NEAR PTR loc_3_2BB8
        db  0F6h, 005h, 01Bh, 022h, 000h, 000h, 020h
        je     SHORT loc_3_2798
        mov    eax, dword ptr [ebx + 1]
        db  0CDh, 020h, 00Ah, 000h, 001h, 000h
loc_3_2798:
        db  0C7h, 005h, 023h, 03Ah, 000h, 000h, 000h, 000h, 000h, 000h
        xor    ah, ah
loc_3_27A4:
        mov    al, 5
        call   sub_3_2F84
        cmp    ebx, edi
        je     SHORT loc_3_27B9
        db  089h, 03Dh, 023h, 03Ah, 000h, 000h
        mov    ah, 1
        jmp    SHORT loc_3_27A4
loc_3_27B9:
        db  08Bh, 03Dh, 023h, 03Ah, 000h, 000h
        or     edi, edi
        je     SHORT loc_3_27DA
        mov    eax, dword ptr [edi + 1]
        xor    edx, edx
        db  0BEh, 029h, 02Fh, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 001h, 000h
        db  080h, 00Dh, 01Bh, 022h, 000h, 000h, 020h
loc_3_27DA:
        db  08Bh, 03Dh, 01Fh, 03Ah, 000h, 000h
        mov    ebx, dword ptr [edi + 1]
        mov    ecx, 13ch
        mov    dword ptr [ecx], ebx
        db  039h, 01Dh, 007h, 03Ah, 000h, 000h
        jne    SHORT loc_3_27F9
        db  080h, 00Dh, 01Bh, 022h, 000h, 000h, 010h
loc_3_27F9:
        mov    word ptr [ebp + 1ch], 8200h
        mov    word ptr [ebp + 18h], 8765h
        mov    word ptr [ebp + 14h], 0cba9h
        mov    eax, 4fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    ah, 1
        cmp    word ptr [ebp + 18h], 9abch
        jne    NEAR PTR loc_3_2778
        cmp    word ptr [ebp + 14h], 5678h
        jne    NEAR PTR loc_3_2778
        db  08Bh, 035h, 027h, 03Ah, 000h, 000h
        call   sub_3_0DF7
        jae    SHORT loc_3_2864
        pushfd
        or     word ptr [esi + 0bh], 1
        xor    cl, cl
        xchg   byte ptr [esi + 8], cl
        pushfd
        push   esi
        db  068h, 0DFh, 03Fh, 000h, 000h
        db  0CDh, 020h, 010h, 000h, 010h, 000h
        add    esp, 8
        popfd
        xchg   byte ptr [esi + 8], cl
        mov    cl, 1
        call   sub_3_11C4
        popfd
loc_3_2864:
        jb     NEAR PTR loc_3_2C34
        db  080h, 025h, 01Dh, 022h, 000h, 000h, 0FEh
        cmp    dword ptr [esi], 494e494dh
        jne    SHORT loc_3_289B
        cmp    dword ptr [esi + 4], 41303034h
        je     SHORT loc_3_2894
        cmp    dword ptr [esi + 4], 49303034h
        je     SHORT loc_3_2894
        cmp    dword ptr [esi + 4], 41363034h
        jne    SHORT loc_3_289B
loc_3_2894:
        db  080h, 00Dh, 01Dh, 022h, 000h, 000h, 001h
loc_3_289B:
        db  0C6h, 005h, 021h, 03Dh, 000h, 000h, 007h
        cmp    eax, 5
        jne    SHORT loc_3_28AE
        db  0C6h, 005h, 021h, 03Dh, 000h, 000h, 000h
loc_3_28AE:
        push   1
        push   1eh
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        db  00Fh, 084h, 043h, 0F9h, 0FCh, 0FFh
        db  08Bh, 00Dh, 013h, 03Ah, 000h, 000h
        db  0A3h, 013h, 03Ah, 000h, 000h
        mov    dword ptr [eax + 1], ecx
        mov    byte ptr [eax], 3
        db  08Bh, 03Dh, 01Fh, 03Ah, 000h, 000h
        mov    ebx, dword ptr [edi + 1]
        mov    dword ptr [eax + 9], ebx
        push   edi
        db  08Bh, 035h, 027h, 03Ah, 000h, 000h
        mov    ecx, 2
        db  08Dh, 03Dh, 012h, 003h, 000h, 000h
        rep movsd
        mov    ecx, 8
        sub    edi, ecx
        mov    al, 20h
        repne scasb
        jne    SHORT loc_3_2904
        mov    byte ptr [edi], 0
loc_3_2904:
        db  08Dh, 035h, 012h, 003h, 000h, 000h
        db  0A1h, 013h, 03Ah, 000h, 000h
        lea    edi, [eax + 15h]
        mov    ecx, 9
        call   sub_3_0EDA
        pop    edi
        db  0A0h, 01Dh, 03Ah, 000h, 000h
        inc    al
        db  0A2h, 019h, 03Ah, 000h, 000h
loc_3_2929:
        db  080h, 02Dh, 019h, 03Ah, 000h, 000h, 001h
        jb     NEAR PTR loc_3_2A47
        db  00Fh, 0B6h, 005h, 019h, 03Ah, 000h, 000h
        db  00Fh, 0A3h, 005h, 02Bh, 03Ah, 000h, 000h
        jb     SHORT loc_3_2929
        db  0C6h, 005h, 017h, 03Ah, 000h, 000h, 000h
        db  0C6h, 005h, 018h, 03Ah, 000h, 000h, 000h
        db  080h, 025h, 01Bh, 022h, 000h, 000h, 0BFh
        call   sub_3_2DC1
        cmp    byte ptr [ebx + 7], 7
        je     NEAR PTR loc_3_2BB8
        db  0F6h, 005h, 01Bh, 022h, 000h, 000h, 040h
        jne    SHORT loc_3_2929
        db  00Fh, 0B6h, 005h, 019h, 03Ah, 000h, 000h
        db  00Fh, 0ABh, 005h, 02Bh, 03Ah, 000h, 000h
        db  0A2h, 01Ch, 03Ah, 000h, 000h
        mov    al, byte ptr [ebx + 2dh]
        db  0A2h, 01Ah, 03Ah, 000h, 000h
loc_3_298E:
        db  0F6h, 005h, 01Dh, 022h, 000h, 000h, 001h
        jne    SHORT loc_3_2929
        db  0A0h, 017h, 03Ah, 000h, 000h
        db  038h, 005h, 01Ah, 03Ah, 000h, 000h
        je     NEAR PTR loc_3_2B92
        db  08Bh, 015h, 00Fh, 03Ah, 000h, 000h
        db  0A0h, 01Bh, 03Ah, 000h, 000h
        mov    byte ptr [edx + 0eh], al
        mov    byte ptr [edx + 4], 0
        db  08Bh, 01Dh, 003h, 03Ah, 000h, 000h
        mov    byte ptr [ebx + 6], 2
        mov    dword ptr [ebx + 10h], 0
        db  0A0h, 019h, 03Ah, 000h, 000h
        mov    byte ptr [ebx + 9], al
        mov    byte ptr [edx], al
        mov    dword ptr [ebx + 0ch], 0
        db  0A0h, 017h, 03Ah, 000h, 000h
        mov    byte ptr [ebx + 0ah], al
        mov    byte ptr [edx + 1], al
        db  0A0h, 018h, 03Ah, 000h, 000h
        mov    byte ptr [ebx + 0bh], al
        mov    byte ptr [edx + 2], al
        mov    word ptr [ebx + 4], 15h
        db  083h, 025h, 01Bh, 022h, 000h, 000h, 0BBh
        db  066h, 0A1h, 001h, 03Ah, 000h, 000h
        mov    word ptr [ebp + 38h], ax
        mov    word ptr [ebx + 2], ax
        db  066h, 0A1h, 0FFh, 039h, 000h, 000h
        add    ax, 24h
        mov    word ptr [ebp + 10h], ax
        mov    word ptr [ebx], ax
        mov    word ptr [ebp + 1ch], 8100h
        mov    eax, 4fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0F6h, 005h, 01Bh, 022h, 000h, 000h, 040h
        jne    NEAR PTR loc_3_2929
        cmp    byte ptr [ebx + 7], 1
        je     SHORT loc_3_2A69
        jmp    NEAR PTR loc_3_2B92
loc_3_2A47:
        db  0A1h, 013h, 03Ah, 000h, 000h
        mov    eax, dword ptr [eax + 5]
        db  08Ah, 015h, 01Ch, 03Ah, 000h, 000h
loc_3_2A55:
        or     eax, eax
        je     NEAR PTR loc_3_2778
        mov    dh, byte ptr [eax]
        sub    dh, dl
        mov    byte ptr [eax + 0eh], dh
        mov    eax, dword ptr [eax + 10h]
        jmp    SHORT loc_3_2A55
loc_3_2A69:
        db  080h, 025h, 01Bh, 022h, 000h, 000h, 0FBh
        mov    byte ptr [ebx + 6], 1
        mov    word ptr [ebx + 4], 58h
        db  0A0h, 019h, 03Ah, 000h, 000h
        mov    byte ptr [ebx + 9], al
        db  0A0h, 017h, 03Ah, 000h, 000h
        mov    byte ptr [ebx + 0ah], al
        db  0A0h, 018h, 03Ah, 000h, 000h
        mov    byte ptr [ebx + 0bh], al
        mov    dword ptr [ebx + 24h], 24h
        mov    byte ptr [ebx + 2ch], 0
        mov    dword ptr [ebx + 0ch], 8468h
        mov    byte ptr [ebx + 2dh], 6
        mov    byte ptr [ebx + 3ch], 12h
        db  0A0h, 018h, 03Ah, 000h, 000h
        shl    al, 5
        mov    byte ptr [ebx + 3dh], al
        mov    word ptr [ebx + 3eh], 0
        mov    byte ptr [ebx + 40h], 24h
        mov    byte ptr [ebx + 41h], 0
        mov    dword ptr [ebx + 48h], 2
        db  0A1h, 0FFh, 039h, 000h, 000h
        mov    dword ptr [ebx + 20h], eax
        db  066h, 0A1h, 001h, 03Ah, 000h, 000h
        mov    word ptr [ebp + 38h], ax
        mov    word ptr [ebx + 2], ax
        db  066h, 0A1h, 0FFh, 039h, 000h, 000h
        add    ax, 24h
        mov    word ptr [ebp + 10h], ax
        mov    word ptr [ebx], ax
        mov    word ptr [ebp + 1ch], 8100h
        mov    eax, 4fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
loc_3_2B0A:
        cmp    byte ptr [ebx + 7], 0
        jne    SHORT loc_3_2B18
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        jmp    SHORT loc_3_2B0A
loc_3_2B18:
        db  0F6h, 005h, 01Bh, 022h, 000h, 000h, 004h
        jne    SHORT loc_3_2B52
        cmp    byte ptr [ebx + 7], 1
        jne    SHORT loc_3_2B7B
        db  08Bh, 01Dh, 0FBh, 039h, 000h, 000h
        test   byte ptr [ebx], 0e0h
        jne    SHORT loc_3_2B92
        db  08Bh, 015h, 00Fh, 03Ah, 000h, 000h
        mov    al, byte ptr [ebx]
        mov    byte ptr [edx + 0fh], al
        xor    eax, eax
        mov    al, byte ptr [ebx]
        mov    ecx, 23h
loc_3_2B46:
        add    al, byte ptr [ecx + ebx]
        adc    ah, 0
        loop   loc_3_2B46
        mov    word ptr [edx + 0ch], ax
loc_3_2B52:
        db  08Bh, 01Dh, 013h, 03Ah, 000h, 000h
        mov    eax, dword ptr [ebx + 5]
        mov    dword ptr [edx + 10h], eax
        mov    dword ptr [ebx + 5], edx
        push   1
        push   14h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        db  00Fh, 084h, 090h, 0F6h, 0FCh, 0FFh
        db  0A3h, 00Fh, 03Ah, 000h, 000h
loc_3_2B7B:
        db  0FEh, 005h, 018h, 03Ah, 000h, 000h
        db  0A0h, 021h, 03Dh, 000h, 000h
        db  038h, 005h, 018h, 03Ah, 000h, 000h
        jbe    NEAR PTR loc_3_298E
loc_3_2B92:
        db  0C6h, 005h, 018h, 03Ah, 000h, 000h, 000h
        db  0FEh, 005h, 017h, 03Ah, 000h, 000h
        db  080h, 03Dh, 017h, 03Ah, 000h, 000h, 007h
        jbe    NEAR PTR loc_3_298E
        db  0C6h, 005h, 017h, 03Ah, 000h, 000h, 000h
        jmp    NEAR PTR loc_3_2929
loc_3_2BB8:
        db  08Bh, 035h, 013h, 03Ah, 000h, 000h
loc_3_2BBE:
        or     esi, esi
        je     SHORT loc_3_2C3F
        mov    edx, dword ptr [esi + 5]
loc_3_2BC5:
        or     edx, edx
        je     SHORT loc_3_2C1A
        cmp    byte ptr [edx + 4], 0
        jne    SHORT loc_3_2C1F
        mov    ecx, edx
        mov    ebx, esi
loc_3_2BD3:
        mov    ecx, dword ptr [ecx + 10h]
loc_3_2BD6:
        or     ecx, ecx
        je     SHORT loc_3_2C0E
        cmp    byte ptr [ecx + 4], 0
        jne    SHORT loc_3_2BD3
        mov    al, byte ptr [edx + 1]
        cmp    byte ptr [ecx + 1], al
        jne    SHORT loc_3_2BD3
        mov    al, byte ptr [edx + 2]
        cmp    byte ptr [ecx + 2], al
        jne    SHORT loc_3_2BD3
        mov    ax, word ptr [edx + 0ch]
        cmp    word ptr [ecx + 0ch], ax
        jne    SHORT loc_3_2BD3
        mov    al, byte ptr [edx + 0fh]
        cmp    byte ptr [ecx + 0fh], al
        jne    SHORT loc_3_2BD3
        cmp    byte ptr [ebx + 15h], 0
        je     SHORT loc_3_2C24
        cmp    byte ptr [esi + 15h], 0
        je     SHORT loc_3_2C24
loc_3_2C0E:
        mov    ebx, dword ptr [ebx + 1]
        or     ebx, ebx
        je     SHORT loc_3_2C1A
        mov    ecx, dword ptr [ebx + 5]
        jmp    SHORT loc_3_2BD6
loc_3_2C1A:
        mov    esi, dword ptr [esi + 1]
        jmp    SHORT loc_3_2BBE
loc_3_2C1F:
        mov    edx, dword ptr [edx + 10h]
        jmp    SHORT loc_3_2BC5
loc_3_2C24:
        pushfd
        db  068h, 014h, 040h, 000h, 000h
        db  0CDh, 020h, 010h, 000h, 010h, 000h
        add    esp, 4
        popfd
loc_3_2C34:
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        or     byte ptr [eax + 28h], 2
        jmp    SHORT loc_3_2C82
loc_3_2C3F:
        db  0F6h, 005h, 01Bh, 022h, 000h, 000h, 010h
        jne    SHORT loc_3_2C5A
        pushfd
        pushfd
        db  068h, 04Dh, 040h, 000h, 000h
        db  0CDh, 020h, 010h, 000h, 010h, 000h
        add    esp, 4
        popfd
        popfd
loc_3_2C5A:
        je     SHORT loc_3_2C34
        db  083h, 03Dh, 0F2h, 039h, 000h, 000h, 000h
        je     SHORT loc_3_2C82
        db  0F6h, 005h, 01Bh, 022h, 000h, 000h, 002h
        jne    SHORT loc_3_2C80
        pushfd
        pushfd
        db  068h, 07Ch, 040h, 000h, 000h
        db  0CDh, 020h, 010h, 000h, 010h, 000h
        add    esp, 4
        popfd
        popfd
loc_3_2C80:
        je     SHORT loc_3_2C34
loc_3_2C82:
        db  0F7h, 005h, 01Bh, 022h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_3_2C9E
        mov    eax, 4fh
        db  0BEh, 05Bh, 02Eh, 000h, 000h
        db  0CDh, 020h, 018h, 001h, 001h, 000h
loc_3_2C9E:
        db  0F6h, 005h, 01Bh, 022h, 000h, 000h, 008h
        je     SHORT loc_3_2CAC
        call   sub_3_2D3C
loc_3_2CAC:
        db  0A1h, 007h, 03Ah, 000h, 000h
        mov    ecx, 13ch
        mov    dword ptr [ecx], eax
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        db  08Bh, 00Dh, 013h, 03Ah, 000h, 000h
        mov    dword ptr [eax + 50h], ecx
        push   0
        db  0FFh, 035h, 00Fh, 03Ah, 000h, 000h
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ecx, 94h
        clc
        db  0CDh, 020h, 009h, 000h, 006h, 000h
        call   sub_3_346B
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        ret
sub_3_2D04:
        db  0FFh, 035h, 027h, 03Ah, 000h, 000h
        db  0FFh, 035h, 01Fh, 03Ah, 000h, 000h
        xor    ah, ah
loc_3_2D12:
        mov    al, 6
        call   sub_3_2F84
        jb     SHORT loc_3_2D2F
        mov    eax, dword ptr [edi + 1]
        xor    edx, edx
        db  0BEh, 00Ah, 02Eh, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 001h, 000h
        mov    ah, 1
        jmp    SHORT loc_3_2D12
loc_3_2D2F:
        db  08Fh, 005h, 01Fh, 03Ah, 000h, 000h
        db  08Fh, 005h, 027h, 03Ah, 000h, 000h
        ret
sub_3_2D3C:
        db  0FFh, 035h, 027h, 03Ah, 000h, 000h
        db  0FFh, 035h, 01Fh, 03Ah, 000h, 000h
        xor    ah, ah
loc_3_2D4A:
        mov    al, 6
        call   sub_3_2F84
        jb     SHORT loc_3_2D60
        mov    eax, dword ptr [edi + 1]
        db  0CDh, 020h, 00Ah, 000h, 001h, 000h
        mov    ah, 1
        jmp    SHORT loc_3_2D4A
loc_3_2D60:
        db  08Fh, 005h, 01Fh, 03Ah, 000h, 000h
        db  08Fh, 005h, 027h, 03Ah, 000h, 000h
        ret
sub_3_2D6D:
        db  08Bh, 01Dh, 003h, 03Ah, 000h, 000h
        db  0A0h, 019h, 03Ah, 000h, 000h
        mov    byte ptr [ebx + 2], al
        mov    byte ptr [ebx], 0
        db  066h, 0A1h, 001h, 03Ah, 000h, 000h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        db  066h, 0A1h, 0FFh, 039h, 000h, 000h
        add    ax, 24h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        movzx  ecx, word ptr [edi + 3]
        movzx  edx, word ptr [edi + 1]
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        ret
sub_3_2DC1:
        db  08Bh, 01Dh, 003h, 03Ah, 000h, 000h
        db  0A0h, 019h, 03Ah, 000h, 000h
        mov    byte ptr [ebx + 9], al
        mov    byte ptr [ebx + 6], 3
        mov    word ptr [ebx + 4], 54h
        db  066h, 0A1h, 001h, 03Ah, 000h, 000h
        mov    word ptr [ebp + 38h], ax
        mov    word ptr [ebx + 2], ax
        db  066h, 0A1h, 0FFh, 039h, 000h, 000h
        add    ax, 24h
        mov    word ptr [ebp + 10h], ax
        mov    word ptr [ebx], ax
        mov    word ptr [ebp + 1ch], 8100h
        mov    eax, 4fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        ret
loc_3_2E0A:
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    eax, dword ptr [eax + 4]
        mov    ebx, eax
        and    eax, 0ffff0000h
        and    ebx, 0ffffh
        shr    eax, 0ch
        add    eax, ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        add    eax, dword ptr [ebx + 4]
        db  08Bh, 03Dh, 00Fh, 03Ah, 000h, 000h
        mov    cl, byte ptr [eax + 2]
        mov    byte ptr [edi + 8], cl
        mov    cl, byte ptr [eax + 8]
        mov    byte ptr [edi + 9], cl
        mov    cl, byte ptr [eax + 9]
        mov    byte ptr [edi + 0ah], cl
        mov    byte ptr [edi + 4], 1
        db  080h, 00Dh, 01Bh, 022h, 000h, 000h, 004h
        call   sub_3_2D3C
        ret
loc_3_2E5B:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        cmp    word ptr [ebx + 1ch], 8100h
        jne    SHORT loc_3_2EB5
        movzx  eax, word ptr [ebx + 38h]
        shl    eax, 10h
        add    ax, word ptr [ebx + 10h]
        mov    ebx, eax
        and    eax, 0ffff0000h
        and    ebx, 0ffffh
        shr    eax, 0ch
        add    eax, ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        add    eax, dword ptr [ebx + 4]
        db  08Bh, 03Dh, 00Fh, 03Ah, 000h, 000h
        mov    cl, byte ptr [eax + 9]
        mov    byte ptr [edi + 8], cl
        mov    cl, byte ptr [eax + 0ah]
        mov    byte ptr [edi + 9], cl
        mov    cl, byte ptr [eax + 0bh]
        mov    byte ptr [edi + 0ah], cl
        mov    byte ptr [edi + 4], 3
        db  080h, 00Dh, 01Bh, 022h, 000h, 000h, 004h
loc_3_2EB5:
        stc
        ret
loc_3_2EB7:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        cmp    word ptr [ebx + 1ch], 0c3d4h
        jne    SHORT loc_3_2F17
        movzx  eax, word ptr [ebx + 38h]
        shl    eax, 10h
        add    ax, word ptr [ebx + 10h]
        mov    ebx, eax
        and    eax, 0ffff0000h
        and    ebx, 0ffffh
        shr    eax, 0ch
        add    eax, ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        add    eax, dword ptr [ebx + 4]
        cmp    byte ptr [eax + 2], 2
        jne    SHORT loc_3_2F17
        db  08Bh, 03Dh, 00Fh, 03Ah, 000h, 000h
        mov    cl, byte ptr [eax + 5]
        mov    byte ptr [edi + 8], cl
        mov    cl, byte ptr [eax + 0ah]
        mov    byte ptr [edi + 9], cl
        mov    cl, byte ptr [eax + 0bh]
        mov    byte ptr [edi + 0ah], cl
        mov    byte ptr [edi + 4], 3
        db  080h, 00Dh, 01Bh, 022h, 000h, 000h, 004h
loc_3_2F17:
        db  08Bh, 00Dh, 022h, 03Dh, 000h, 000h
        movzx  edx, cx
        shr    ecx, 10h
        db  0CDh, 020h, 049h, 080h, 001h, 000h
loc_3_2F29:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        movzx  eax, word ptr [ebx + 38h]
        shl    eax, 10h
        add    ax, word ptr [ebx + 10h]
        mov    ebx, eax
        and    eax, 0ffff0000h
        and    ebx, 0ffffh
        shr    eax, 0ch
        add    eax, ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        add    eax, dword ptr [ebx + 4]
        db  080h, 00Dh, 01Bh, 022h, 000h, 000h, 040h
        mov    byte ptr [eax + 7], 1
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        ret
sub_3_2F6A:
        push   ebx
        movzx  ebx, di
        rol    edi, 10h
        movzx  edi, di
        shl    edi, 4
        add    edi, ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        add    edi, dword ptr [ebx + 4]
        pop    ebx
        ret
sub_3_2F84:
        push   ebx
        or     ah, ah
        jne    SHORT loc_3_2FBC
        db  08Bh, 03Dh, 00Bh, 03Ah, 000h, 000h
        mov    bl, byte ptr [edi + 5]
        or     bl, bl
        je     SHORT loc_3_2FED
        db  088h, 01Dh, 01Eh, 03Ah, 000h, 000h
        lea    edi, [edi + 6]
        db  089h, 03Dh, 027h, 03Ah, 000h, 000h
loc_3_2FA5:
        mov    bl, byte ptr [edi + 0eh]
        or     bl, bl
        je     SHORT loc_3_2FBC
        add    edi, 0fh
loc_3_2FAF:
        cmp    byte ptr [edi], al
        je     SHORT loc_3_2FE4
        dec    bl
        je     SHORT loc_3_2FBC
        add    edi, 5
        jmp    SHORT loc_3_2FAF
loc_3_2FBC:
        db  0FEh, 00Dh, 01Eh, 03Ah, 000h, 000h
        je     SHORT loc_3_2FED
        db  08Bh, 03Dh, 027h, 03Ah, 000h, 000h
        movzx  ecx, byte ptr [edi + 0eh]
        push   eax
        mov    eax, 5
        mul    ecx
        add    edi, eax
        pop    eax
        add    edi, 0fh
        db  089h, 03Dh, 027h, 03Ah, 000h, 000h
        jmp    SHORT loc_3_2FA5
loc_3_2FE4:
        db  089h, 03Dh, 01Fh, 03Ah, 000h, 000h
        clc
loc_3_2FEB:
        pop    ebx
        ret
loc_3_2FED:
        stc
        jmp    SHORT loc_3_2FEB
loc_3_2FF0:
        cmp    word ptr [ebp + 1ch], 300h
        jne    SHORT loc_3_301C
        mov    ax, 3c04h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        cmp    byte ptr [eax + 1], 3ah
        jne    SHORT loc_3_301C
        mov    al, byte ptr [eax]
        or     al, 20h
        sub    al, 61h
        db  03Ah, 005h, 03Eh, 001h, 000h, 000h
        jne    SHORT loc_3_301C
        and    word ptr [ebp + 2ch], 0fffeh
        ret
loc_3_301C:
        stc
        ret
loc_3_301E:
        db  0F6h, 005h, 01Ch, 022h, 000h, 000h, 008h
        je     NEAR PTR loc_3_30AB
        db  080h, 00Dh, 01Bh, 022h, 000h, 000h, 004h
        cmp    byte ptr [ebp + 1dh], 2
        jne    SHORT loc_3_30AB
        db  08Bh, 03Dh, 033h, 03Ah, 000h, 000h
        mov    dl, byte ptr [ebp + 14h]
        mov    byte ptr [edi + 1], dl
        db  08Bh, 035h, 09Ch, 01Fh, 000h, 000h
        mov    esi, dword ptr [esi + 2ch]
loc_3_304D:
        or     esi, esi
        je     SHORT loc_3_30AB
        cmp    byte ptr [esi], 0
        je     SHORT loc_3_3060
        cmp    byte ptr [esi], 2
        je     SHORT loc_3_3060
        cmp    byte ptr [esi], 1
        jne    SHORT loc_3_3065
loc_3_3060:
        cmp    byte ptr [esi + 5], dl
        je     SHORT loc_3_3070
loc_3_3065:
        cmp    dword ptr [esi + 1], 0
        je     SHORT loc_3_30AD
        add    esi, dword ptr [esi + 1]
        jmp    SHORT loc_3_304D
loc_3_3070:
        mov    ch, byte ptr [ebp + 15h]
        mov    cl, byte ptr [ebp + 18h]
        mov    al, cl
        and    cl, 3fh
        and    ax, 0c0h
        shl    ax, 2
        mov    al, byte ptr [ebp + 19h]
        movzx  ebx, ax
        mov    eax, ebx
        movzx  ebx, word ptr [esi + 8]
        mul    ebx
        movzx  ebx, ch
        add    eax, ebx
        movzx  ebx, word ptr [esi + 0ah]
        mul    ebx
        movzx  ebx, cl
        dec    ebx
        add    eax, ebx
        db  02Bh, 005h, 043h, 03Ah, 000h, 000h
        mov    dword ptr [edi + 6], eax
loc_3_30AB:
        stc
        ret
loc_3_30AD:
        mov    dword ptr [esi + 1], 1dh
        add    esi, 1dh
        mov    byte ptr [esi], 2
        mov    byte ptr [esi + 5], dl
        db  080h, 00Dh, 01Ch, 022h, 000h, 000h, 020h
        jmp    SHORT loc_3_30AB
loc_3_30C6:
        db  0F6h, 005h, 01Ch, 022h, 000h, 000h, 008h
        je     SHORT loc_3_311B
        db  080h, 00Dh, 01Bh, 022h, 000h, 000h, 004h
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    eax, dword ptr [eax + 4]
        mov    ebx, eax
        and    eax, 0ffff0000h
        and    ebx, 0ffffh
        shr    eax, 0ch
        add    eax, ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        add    eax, dword ptr [ebx + 4]
        cmp    byte ptr [eax], 2
        jne    SHORT loc_3_311B
        db  08Bh, 03Dh, 033h, 03Ah, 000h, 000h
        mov    cl, byte ptr [eax + 2]
        mov    byte ptr [edi + 0ch], cl
        mov    cl, byte ptr [eax + 8]
        mov    byte ptr [edi + 0ah], cl
        mov    cl, byte ptr [eax + 9]
        mov    byte ptr [edi + 0bh], cl
loc_3_311B:
        db  0A1h, 0F2h, 039h, 000h, 000h
        db  0CDh, 020h, 00Ah, 000h, 001h, 000h
        db  0C6h, 005h, 0F6h, 039h, 000h, 000h, 000h
        ret
loc_3_312E:
        db  0F6h, 005h, 01Ch, 022h, 000h, 000h, 008h
        je     SHORT loc_3_318D
        db  080h, 00Dh, 01Bh, 022h, 000h, 000h, 004h
        cmp    word ptr [ebp + 1ch], 8100h
        jne    SHORT loc_3_318D
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        movzx  eax, word ptr [ebx + 38h]
        shl    eax, 10h
        add    ax, word ptr [ebx + 10h]
        mov    ebx, eax
        and    eax, 0ffff0000h
        and    ebx, 0ffffh
        shr    eax, 0ch
        add    eax, ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        add    eax, dword ptr [ebx + 4]
        db  08Bh, 03Dh, 033h, 03Ah, 000h, 000h
        mov    cl, byte ptr [eax + 9]
        mov    byte ptr [edi + 0fh], cl
        mov    cl, byte ptr [eax + 0ah]
        mov    byte ptr [edi + 0dh], cl
        mov    cl, byte ptr [eax + 0bh]
        mov    byte ptr [edi + 0eh], cl
loc_3_318D:
        stc
        ret
sub_3_318F:
        xor    eax, eax
loc_3_3191:
        db  0E8h, 02Ah, 0EAh, 0FCh, 0FFh
        jb     SHORT loc_3_31F8
        test   byte ptr [esi + 10h], 10h
        je     SHORT loc_3_31F8
        db  08Bh, 034h, 085h, 0ACh, 020h, 000h, 000h
        or     esi, esi
        je     SHORT loc_3_31F8
        test   byte ptr [esi + 22h], 40h
        jne    SHORT loc_3_31F8
        push   eax
        db  08Bh, 004h, 085h, 0BEh, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 45h]
        movzx  ecx, ax
        rol    eax, 10h
        movzx  eax, ax
        shl    eax, 4
        add    eax, ecx
        mov    eax, dword ptr [eax + 13h]
        mov    ecx, eax
        movzx  edx, cx
        rol    ecx, 10h
        movzx  ecx, cx
        shl    ecx, 4
        add    ecx, edx
        mov    ax, word ptr [ecx + 8]
        xor    edx, edx
        db  0BEh, 0D9h, 01Bh, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 001h, 000h
        mov    ecx, eax
        pop    eax
        jb     SHORT loc_3_31F8
        db  089h, 00Ch, 085h, 070h, 003h, 000h, 000h
loc_3_31F8:
        inc    eax
        cmp    al, 1ah
        jb     SHORT loc_3_3191
        ret
        db  000h, 000h
sub_3_3200:
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        or     byte ptr [ecx + 29h], 4
        call   sub_3_32CF
        call   sub_3_33F4
        db  0E8h, 09Ch, 0EAh, 0FCh, 0FFh
        call   sub_3_346B
        db  0BEh, 050h, 005h, 000h, 000h
        db  08Bh, 03Dh, 0F9h, 001h, 000h, 000h
        db  0E8h, 0B5h, 0EFh, 0FCh, 0FFh
        db  0E8h, 0F2h, 0ECh, 0FCh, 0FFh
        db  08Bh, 03Dh, 09Ch, 01Fh, 000h, 000h
        test   byte ptr [edi + 28h], 40h
        jne    SHORT loc_3_32AF
loc_3_323F:
        db  0E8h, 024h, 0ECh, 0FCh, 0FFh
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        and    byte ptr [ecx + 29h], 0fbh
        db  08Bh, 03Dh, 075h, 03Ah, 000h, 000h
        or     edi, edi
        je     SHORT loc_3_32AE
loc_3_3258:
        call   sub_3_2F6A
        cmp    byte ptr [edi], 0e9h
        je     SHORT loc_3_3267
        cmp    byte ptr [edi], 0ebh
        jne    SHORT loc_3_32AE
loc_3_3267:
        cmp    dword ptr [edi + 3], 544e4924h
        jne    SHORT loc_3_32AE
        cmp    dword ptr [edi + 7], 46533331h
        jne    SHORT loc_3_32AE
        cmp    dword ptr [edi + 17h], 1
        je     SHORT loc_3_329F
        pushfd
        db  068h, 06Ch, 03Ah, 000h, 000h
        db  068h, 0ACh, 040h, 000h, 000h
        db  0CDh, 020h, 010h, 000h, 010h, 000h
        add    esp, 8
        popfd
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        or     byte ptr [eax + 28h], 2
        jmp    SHORT loc_3_32AE
loc_3_329F:
        mov    edi, dword ptr [edi + 13h]
        mov    ecx, edi
        rol    ecx, 10h
        cmp    cx, 0a000h
        jb     SHORT loc_3_3258
loc_3_32AE:
        ret
loc_3_32AF:
        db  0BEh, 058h, 005h, 000h, 000h
        db  08Bh, 03Dh, 0F9h, 001h, 000h, 000h
        db  0E8h, 024h, 0EFh, 0FCh, 0FFh
        db  0E8h, 0E3h, 0EAh, 0FCh, 0FFh
        db  00Fh, 082h, 072h, 0EFh, 0FCh, 0FFh
        jmp    NEAR PTR loc_3_323F
sub_3_32CF:
        db  0CDh, 020h, 0B6h, 000h, 001h, 000h
        mov    esi, edx
        db  0BFh, 079h, 001h, 000h, 000h
        rep movsb
        db  0BEh, 0FDh, 002h, 000h, 000h
        mov    ecx, 9
        rep movsb
        db  089h, 03Dh, 0F9h, 001h, 000h, 000h
        ret
        db  000h, 000h, 000h
sub_3_32F4:
        pushad
        xor    edi, edi
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    ecx, 408h
        clc
        db  0CDh, 020h, 008h, 000h, 006h, 000h
        cmp    ecx, 408h
        db  00Fh, 085h, 0D9h, 0EEh, 0FCh, 0FFh
        mov    esi, edi
        call   sub_3_2F6A
        xor    eax, eax
        mov    word ptr [edi], ax
        mov    word ptr [ebp + 14h], ax
        dec    ax
        mov    word ptr [edi + 2], ax
        mov    word ptr [ebp + 18h], 1
        mov    eax, esi
        mov    word ptr [ebp + 10h], ax
        mov    word ptr [ebp + 8], ax
        rol    eax, 10h
        mov    word ptr [ebp + 3ch], ax
        mov    word ptr [ebp + 38h], ax
        mov    word ptr [ebp + 1ch], 0cdcdh
        mov    eax, 25h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        cmp    word ptr [edi], 0cdcdh
        jne    SHORT loc_3_33D1
        cmp    word ptr [edi + 2], 1
        jne    SHORT loc_3_33D1
        mov    esi, edi
        mov    edi, dword ptr [esi + 4]
        call   sub_3_2F6A
        cmp    word ptr [edi], 0a55ah
        jne    SHORT loc_3_33D1
        db  08Bh, 035h, 09Ch, 01Fh, 000h, 000h
        or     byte ptr [esi + 2ah], 1
        cmp    dword ptr [edi + 52h], 50415753h
        jne    SHORT loc_3_33D1
        lea    esi, [edi + 56h]
        db  0BFh, 0E9h, 021h, 000h, 000h
        xor    ecx, ecx
loc_3_33B1:
        movzx  eax, byte ptr [ecx + edi]
        cmp    eax, 0ffh
        je     SHORT loc_3_33CB
        movzx  edx, byte ptr [ecx + esi]
        cmp    ecx, edx
        je     SHORT loc_3_33CB
        cmp    eax, edx
        je     SHORT loc_3_33CB
        mov    byte ptr [ecx + edi], dl
loc_3_33CB:
        inc    ecx
        cmp    ecx, 1ah
        jb     SHORT loc_3_33B1
loc_3_33D1:
        mov    ecx, 408h
        clc
        db  0CDh, 020h, 009h, 000h, 006h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        popad
        ret
sub_3_33F4:
        push   eax
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        test   byte ptr [eax + 28h], 1
        jne    SHORT loc_3_3469
        db  0FEh, 005h, 03Bh, 03Ah, 000h, 000h
        db  080h, 03Dh, 03Bh, 03Ah, 000h, 000h, 001h
        jne    SHORT loc_3_3469
        test   byte ptr [eax + 28h], 20h
        jne    SHORT loc_3_3469
        push   ebx
        push   ecx
        push   edx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 00Ah, 000h, 006h, 000h
        db  089h, 01Dh, 047h, 03Ah, 000h, 000h
        db  0A3h, 04Bh, 03Ah, 000h, 000h
        db  089h, 00Dh, 04Fh, 03Ah, 000h, 000h
        db  089h, 015h, 053h, 03Ah, 000h, 000h
        push   0
        push   10000h
        db  0CDh, 020h, 0A9h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        db  00Fh, 084h, 0E9h, 0EDh, 0FCh, 0FFh
        add    eax, 0fh
        shr    eax, 4
        mov    ecx, 10000h
        mov    edx, ecx
        db  0CDh, 020h, 00Bh, 000h, 006h, 000h
        pop    edx
        pop    ecx
        pop    ebx
loc_3_3469:
        pop    eax
        ret
sub_3_346B:
        push   eax
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        test   byte ptr [eax + 28h], 1
        jne    SHORT loc_3_34B7
        db  080h, 03Dh, 03Bh, 03Ah, 000h, 000h, 000h
        je     SHORT loc_3_34B7
        db  0FEh, 00Dh, 03Bh, 03Ah, 000h, 000h
        jne    SHORT loc_3_34B7
        test   byte ptr [eax + 28h], 20h
        jne    SHORT loc_3_34B7
        push   ebx
        push   ecx
        push   edx
        db  08Bh, 01Dh, 047h, 03Ah, 000h, 000h
        db  0A1h, 04Bh, 03Ah, 000h, 000h
        db  08Bh, 00Dh, 04Fh, 03Ah, 000h, 000h
        db  08Bh, 015h, 053h, 03Ah, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 006h, 000h
        db  0CDh, 020h, 0AAh, 000h, 001h, 000h
        pop    edx
        pop    ecx
        pop    ebx
loc_3_34B7:
        pop    eax
        ret
sub_3_34B9:
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        mov    ecx, dword ptr [ecx + 30h]
        db  08Dh, 005h, 003h, 022h, 000h, 000h
loc_3_34C8:
        mov    edi, eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_3_34C8
loc_3_34D0:
        jecxz  loc_3_3534
        test   byte ptr [ecx + 21h], 20h
        jne    SHORT loc_3_352F
        test   byte ptr [ecx + 21h], 80h
        je     SHORT loc_3_352F
        bts    dword ptr [ecx + 20h], 1ch
        jb     SHORT loc_3_352F
        mov    eax, dword ptr [ecx + 20h]
        and    eax, 3fh
        or     dword ptr [ecx + 0cbh], eax
        mov    dl, byte ptr [ecx + 15h]
        mov    byte ptr [ecx + 0cah], dl
        cmp    byte ptr [ecx + 40h], 0ah
        je     SHORT loc_3_350E
        cmp    byte ptr [ecx + 40h], 0
        jne    SHORT loc_3_352F
        test   dl, 80h
        je     SHORT loc_3_352F
        and    dl, 7fh
loc_3_350E:
        test   dl, 0f8h
        jne    SHORT loc_3_352F
        test   byte ptr [ecx + 20h], 1
        je     SHORT loc_3_352F
        mov    dword ptr [edi], ecx
        add    dword ptr [edi], 0c3h
        mov    edi, ecx
        add    edi, 0c3h
        db  0CDh, 020h, 001h, 000h, 020h, 000h
loc_3_352F:
        mov    ecx, dword ptr [ecx + 0ch]
        jmp    SHORT loc_3_34D0
loc_3_3534:
        mov    dword ptr [edi], ecx
        ret
sub_3_3537:
        push   edi
        push   ecx
        push   eax
        mov    cl, byte ptr [edi + 0eh]
        or     cl, cl
        je     SHORT loc_3_355E
        add    edi, 0fh
loc_3_3544:
        cmp    byte ptr [edi], 0ah
        je     SHORT loc_3_3552
        dec    cl
        je     SHORT loc_3_355E
        add    edi, 5
        jmp    SHORT loc_3_3544
loc_3_3552:
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        mov    ecx, dword ptr [edi + 1]
        or     word ptr [eax + 6ch], cx
loc_3_355E:
        pop    eax
        pop    ecx
        pop    edi
        ret
sub_3_3562:
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        db  08Bh, 015h, 068h, 03Ah, 000h, 000h
        cmp    word ptr [edx + 0feh], -1
        je     SHORT loc_3_3594
        lea    edx, [edx + 0eh]
loc_3_357B:
        cmp    word ptr [edx], 0
        je     SHORT loc_3_3594
        test   byte ptr [edx + 7], 80h
        jne    SHORT loc_3_358F
        mov    ax, word ptr [edx + 2]
        or     word ptr [ecx + 6ch], ax
loc_3_358F:
        add    edx, 0ch
        jmp    SHORT loc_3_357B
loc_3_3594:
        ret
        db  000h, 000h, 000h
sub_3_3598:
        db  066h, 083h, 03Dh, 060h, 004h, 000h, 000h, 0FFh
        jne    SHORT loc_3_35EE
        call   sub_3_33F4
        db  0BEh, 0D8h, 003h, 000h, 000h
        xor    cx, cx
        mov    bx, 4002h
        mov    dx, 11h
        mov    ax, 6c00h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_3_35F0
        db  080h, 00Dh, 01Ch, 022h, 000h, 000h, 010h
        db  066h, 0A3h, 060h, 004h, 000h, 000h
        mov    bx, ax
        mov    ax, 4202h
        xor    cx, cx
        xor    dx, dx
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_3_35F0
        call   sub_3_346B
loc_3_35EE:
        clc
        ret
loc_3_35F0:
        call   sub_3_346B
        stc
        ret
sub_3_35F7:
        db  066h, 083h, 03Dh, 060h, 004h, 000h, 000h, 0FFh
        je     SHORT loc_3_361B
        db  066h, 08Bh, 01Dh, 060h, 004h, 000h, 000h
        mov    ah, 3eh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        db  066h, 0C7h, 005h, 060h, 004h, 000h, 000h, 0FFh, 0FFh
loc_3_361B:
        ret
sub_3_361C:
        call   sub_3_33F4
        db  0CDh, 020h, 0B7h, 000h, 001h, 000h
        mov    esi, edx
        db  0BFh, 0D8h, 003h, 000h, 000h
        db  0E8h, 0B0h, 0EBh, 0FCh, 0FFh
        dec    edi
        db  0BEh, 058h, 004h, 000h, 000h
        db  0E8h, 0A5h, 0EBh, 0FCh, 0FFh
        db  0BAh, 0D8h, 003h, 000h, 000h
        mov    ax, 4100h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        call   sub_3_346B
        ret
sub_3_3655:
        db  0F6h, 005h, 01Ch, 022h, 000h, 000h, 010h
        je     NEAR PTR loc_3_3859
        db  080h, 025h, 01Bh, 022h, 000h, 000h, 07Fh
        call   sub_3_3598
        db  08Bh, 03Dh, 00Bh, 03Ah, 000h, 000h
        movzx  edx, byte ptr [edi + 5]
        add    edi, 6
loc_3_367B:
        db  0BEh, 0C8h, 040h, 000h, 000h
        push   esi
        call   sub_3_38A0
        add    esp, 4
        mov    esi, edi
        push   edi
        push   edx
        call   sub_3_10CB
        pop    edx
        pop    edi
        db  068h, 003h, 042h, 000h, 000h
        call   sub_3_38A0
        add    esp, 4
        push   esi
        call   sub_3_38A0
        add    esp, 4
        db  0BEh, 0C8h, 040h, 000h, 000h
        push   esi
        call   sub_3_38A0
        add    esp, 4
        movzx  eax, byte ptr [edi + 0dh]
        or     eax, eax
        je     SHORT loc_3_36D3
        pushfd
        push   eax
        db  068h, 011h, 042h, 000h, 000h
        db  0CDh, 020h, 010h, 000h, 010h, 000h
        add    esp, 8
        popfd
        jmp    SHORT loc_3_36E0
loc_3_36D3:
        db  068h, 039h, 042h, 000h, 000h
        call   sub_3_38A0
        add    esp, 4
loc_3_36E0:
        movzx  ecx, byte ptr [edi + 0eh]
        db  068h, 04Eh, 042h, 000h, 000h
        call   sub_3_38A0
        add    esp, 4
        db  0BBh, 0CBh, 040h, 000h, 000h
        lea    edi, [edi + 0fh]
loc_3_36F9:
        movzx  eax, byte ptr [edi]
        mov    esi, dword ptr [ebx + eax*4]
        cmp    al, 7
        je     NEAR PTR loc_3_385A
        mov    eax, dword ptr [edi + 1]
        push   esi
        call   sub_3_38A0
        add    esp, 4
        pushfd
        push   eax
        db  068h, 060h, 042h, 000h, 000h
        db  0CDh, 020h, 010h, 000h, 010h, 000h
        add    esp, 8
        popfd
loc_3_3724:
        db  0F6h, 005h, 01Bh, 022h, 000h, 000h, 080h
        jne    NEAR PTR loc_3_384C
        add    edi, 5
        loop   loc_3_36F9
        dec    edx
        jne    NEAR PTR loc_3_367B
        db  080h, 00Dh, 01Bh, 022h, 000h, 000h, 080h
        db  0BEh, 0C8h, 040h, 000h, 000h
        push   esi
        call   sub_3_38A0
        add    esp, 4
        db  08Bh, 03Dh, 068h, 03Ah, 000h, 000h
        cmp    word ptr [edi + 0feh], -1
        je     NEAR PTR loc_3_3854
        db  068h, 065h, 042h, 000h, 000h
        call   sub_3_38A0
        add    esp, 4
        mov    eax, dword ptr [edi + 6]
        pushfd
        push   eax
        db  068h, 080h, 042h, 000h, 000h
        db  0CDh, 020h, 010h, 000h, 010h, 000h
        add    esp, 8
        popfd
        lea    edi, [edi + 0eh]
loc_3_378A:
        movzx  esi, word ptr [edi]
        or     esi, esi
        je     NEAR PTR loc_3_3854
        dec    esi
        shl    esi, 4
        lea    esi, [esi + 8]
        push   edi
        db  0BFh, 0CBh, 03Ch, 000h, 000h
        mov    edx, edi
        xor    al, al
        mov    ecx, 0bh
        rep stosb
        mov    edi, edx
        mov    ecx, 8
loc_3_37B4:
        lodsb
        or     al, al
        je     SHORT loc_3_37BC
        stosb
        loop   loc_3_37B4
loc_3_37BC:
        pop    edi
        mov    esi, edx
        db  068h, 085h, 042h, 000h, 000h
        call   sub_3_38A0
        add    esp, 4
        push   esi
        call   sub_3_38A0
        add    esp, 4
        db  0BEh, 0C8h, 040h, 000h, 000h
        push   esi
        call   sub_3_38A0
        add    esp, 4
        db  068h, 090h, 042h, 000h, 000h
        call   sub_3_38A0
        add    esp, 4
        mov    eax, dword ptr [edi + 8]
        pushfd
        push   eax
        db  068h, 0A9h, 042h, 000h, 000h
        db  0CDh, 020h, 010h, 000h, 010h, 000h
        add    esp, 8
        popfd
        movzx  eax, word ptr [edi + 2]
        or     eax, eax
        je     SHORT loc_3_382A
        db  068h, 0AEh, 042h, 000h, 000h
        call   sub_3_38A0
        add    esp, 4
        pushfd
        push   eax
        db  068h, 0D5h, 042h, 000h, 000h
        db  0CDh, 020h, 010h, 000h, 010h, 000h
        add    esp, 8
        popfd
loc_3_382A:
        mov    esi, dword ptr [edi + 4]
        test   esi, 80000000h
        je     SHORT loc_3_3842
        db  068h, 0DAh, 042h, 000h, 000h
        call   sub_3_38A0
        add    esp, 4
loc_3_3842:
        and    esi, 1fffffffh
        or     esi, esi
        jne    SHORT loc_3_3874
loc_3_384C:
        add    edi, 0ch
        jmp    NEAR PTR loc_3_378A
loc_3_3854:
        call   sub_3_35F7
loc_3_3859:
        ret
loc_3_385A:
        push   esi
        call   sub_3_38A0
        add    esp, 4
        db  0BEh, 0C8h, 040h, 000h, 000h
        push   esi
        call   sub_3_38A0
        add    esp, 4
        mov    esi, dword ptr [edi + 1]
loc_3_3874:
        bsf    eax, esi
        je     NEAR PTR loc_3_3724
        btr    esi, eax
        db  068h, 0EFh, 042h, 000h, 000h
        call   sub_3_38A0
        add    esp, 4
        pushfd
        push   eax
        db  068h, 009h, 043h, 000h, 000h
        db  0CDh, 020h, 010h, 000h, 010h, 000h
        add    esp, 8
        popfd
        jmp    SHORT loc_3_3874
sub_3_38A0:
        pushad
        xor    esi, esi
        db  066h, 083h, 03Dh, 060h, 004h, 000h, 000h, 0FFh
        jne    SHORT loc_3_38B7
        call   sub_3_3598
        mov    esi, 1
loc_3_38B7:
        mov    edx, dword ptr [esp + 24h]
        cld
        mov    edi, edx
        xor    al, al
        mov    ecx, 0ffffffffh
        repne scasb
        neg    ecx
        dec    ecx
        cmp    byte ptr [edi - 2], 0ah
        jne    SHORT loc_3_38D1
        dec    ecx
loc_3_38D1:
        db  066h, 08Bh, 01Dh, 060h, 004h, 000h, 000h
        mov    ah, 40h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        or     esi, esi
        je     SHORT loc_3_38EB
        call   sub_3_35F7
loc_3_38EB:
        popad
        ret
        db  000h, 000h, 000h
loc_3_38F0:
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        add    byte ptr [eax], al
        or     byte ptr [eax], al
loc_3_38FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3900:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    edx
        dec    esp
        dec    edi
        inc    ebx
        dec    ebx
        inc    esp
        inc    ebp
        push   esi
        and    byte ptr [ebx + 4fh], al
        dec    ebp
        push   eax
        inc    ecx
        push   esp
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], bh
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [edi], bh
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        aas
        or     eax, dword ptr [eax]
        add    dword ptr [eax], eax
loc_3_3962:
        push   cs
        add    byte ptr [eax], al
        db  000h, 0A8h, 001h, 000h, 000h, 000h
        add    byte ptr [eax], al
        db  000h, 09Ch, 000h, 000h, 000h, 000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        hlt
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 69h], dl
        jb     SHORT loc_3_39FC
        jne    SHORT loc_3_39EB
        insb
        dec    eax
        inc    esp
        dec    ecx
        push   edx
        push   ecx
        add    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 35h
        aaa
        cmp    byte ptr [ecx], bh
        inc    ecx
        inc    edx
loc_3_399C:
        dec    eax
        popad
        jb     SHORT loc_3_3A04
        inc    esp
        imul   esi, dword ptr [ebx + 6bh], 42414d44h
        jne    SHORT loc_3_3A10
        jb     NEAR PTR loc_3_39AE
loc_3_39AE:
        inc    esi
        outsd
        jb     SHORT loc_3_3A15
        push   eax
        db  072h, 06Fh
        db  074h, 04Dh
        outsd
        dec    ecx
        dec    edi
        add    byte ptr [ecx + 4fh], cl
        push   ebx
        inc    esp
        dec    ebp
        inc    ecx
        inc    edx
        db  075h, 066h
        jb     NEAR PTR loc_3_3A1E
        imul   edi, dword ptr [edx + 65h], 534f4900h
        dec    eax
        popad
        jo     SHORT loc_3_3A2A
        imul   edi, dword ptr [edx + 65h], 69724400h
        jbe    SHORT loc_3_3A45
        push   ebx
        jo     SHORT loc_3_3A4C
        outsb
        inc    esp
        outsd
        ja     SHORT loc_3_3A56
        add    byte ptr [ebx + 43h], dl
loc_3_39EB:
        push   ebx
        dec    ecx
        dec    ebp
        inc    edi
        push   edx
        and    al, 0
loc_3_39F2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_39F6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_39FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3A04:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h
loc_3_3A10:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_3_3A15:
        add    byte ptr [eax], al
loc_3_3A17:
        add    byte ptr [eax], al
loc_3_3A19:
        add    byte ptr [eax], al
loc_3_3A1B:
        add    byte ptr [eax], al
        db  000h
loc_3_3A1E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3A2A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_3_3A45:
        add    byte ptr [eax], al
loc_3_3A47:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_3_3A4C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3A56:
        add    byte ptr [eax], al
loc_3_3A58:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3A5C:
        inc    esp
        inc    edx
        dec    esp
        push   ebx
        push   eax
        inc    ecx
        inc    ebx
        inc    ebp
        inc    edx
        dec    ecx
        dec    esi
        add    bh, bh
        db  0FFh, 0FFh, 0FFh
loc_3_3A6C:
        dec    ebp
        inc    edx
        push   edx
        dec    ecx
        dec    esi
        push   esp
        xor    dword ptr [ebx], esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 79h], dl
        jae    SHORT loc_3_3AF1
        insd
        pop    esp
        inc    ebx
        db  075h, 072h
        db  072h, 065h
        outsb
        db  074h, 043h
        outsd
        outsb
        je     SHORT loc_3_3AFE
        outsd
        insb
        push   ebx
        je     NEAR PTR loc_3_3AEE
        inc    ebx
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        pop    esp
        inc    esi
        imul   ebp, dword ptr [ebp + 53h], 65747379h
        insd
        add    byte ptr [esi + 6fh], al
        jb     SHORT loc_3_3B0C
        push   edx
        dec    ebp
        dec    ecx
        dec    edi
        add    byte ptr [esi + 6fh], al
        jb     SHORT loc_3_3B16
        push   eax
        dec    ebp
        dec    ecx
        dec    edi
        add    byte ptr [ecx + 73h], cl
        je     SHORT loc_3_3B00
        outsd
        outsd
        je     SHORT loc_3_3B12
        dec    ebp
        inc    esp
        jb     SHORT loc_3_3B3C
        jae    SHORT loc_3_3AC8
loc_3_3AC8:
        inc    edx
        popad
        je     SHORT loc_3_3B10
        db  072h, 069h
        db  076h, 065h
        push   ebx
        jo     SHORT loc_3_3B3C
        outsb
        inc    esp
        outsd
        ja     SHORT loc_3_3B46
        add    byte ptr [ecx + 43h], al
        inc    esp
        db  072h, 069h
        db  076h, 065h
        push   ebx
        jo     SHORT loc_3_3B4C
        outsb
        inc    esp
        outsd
        ja     SHORT loc_3_3B56
        add    byte ptr [edi + 61h], dl
        db  072h, 06Eh
        dec    edi
loc_3_3AEE:
        outsb
        push   ebp
        outsb
loc_3_3AF1:
        jae    SHORT loc_3_3B54
        inc    sp
        db  072h, 069h
        db  076h, 065h
        jb     SHORT loc_3_3AFC
loc_3_3AFC:
        add    byte ptr [ebx], bh
loc_3_3AFE:
        add    byte ptr [eax], al
loc_3_3B00:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3B0C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3B10:
        add    byte ptr [eax], al
loc_3_3B12:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3B16:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3B3C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3B46:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3B4C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3B54:
        add    byte ptr [eax], al
loc_3_3B56:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3B80:
        cwde
        cmp    eax, dword ptr [eax]
        db  000h, 0A5h, 03Bh, 000h, 000h, 0B5h
        cmp    eax, dword ptr [eax]
        db  000h, 0C1h
        cmp    eax, dword ptr [eax]
        db  000h, 0CBh
        cmp    eax, dword ptr [eax]
        db  000h, 0D7h
        cmp    eax, dword ptr [eax]
        add    byte ptr [esp], cl
        outsd
        pop    edi
        outsb
        outsd
        je     SHORT loc_3_3BFF
        arpl   word ptr [ecx + 72h], sp
        add    byte ptr gs:[edi], cl
        insd
        db  075h, 073h
        je     SHORT loc_3_3C0A
        outsb
        outsd
        je     SHORT loc_3_3C0E
        arpl   word ptr [eax + 61h], bp
        db  069h, 06Eh, 000h
loc_3_3BB5:
        or     ebp, dword ptr [ebp + 75h]
        db  073h, 074h
        pop    edi
        arpl   word ptr [eax + 61h], bp
        imul   ebp, dword ptr [esi], 6e6f6e09h
        pop    edi
        imul   esi, dword ptr fs:[ebx + 6bh], 6f6d0b00h
        outsb
        outsd
        insb
        imul   esi, dword ptr [eax + ebp*2 + 69h], 6c0c0063h
        jne    SHORT loc_3_3C49
        db  07Ah, 065h
        db  072h, 06Fh
        outsd
        outsb
        insb
        jns    SHORT loc_3_3BE4
loc_3_3BE4:
        dec    ecx
        dec    edi
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   ebx
        pop    ecx
        push   ebx
        add    byte ptr [ebp + 4dh], al
        dec    ebp
        xor    edi, dword ptr [eax]
        and    byte ptr ss:[eax], ah
        inc    ebp
        pop    eax
        inc    ebp
        add    byte ptr [ebp + 4dh], al
        dec    ebp
loc_3_3BFF:
        xor    edi, dword ptr [eax]
        and    byte ptr ss:[eax], ah
        push   ebx
        pop    ecx
        push   ebx
        add    byte ptr [ecx + 46h], cl
loc_3_3C0A:
        push   ebx
        dec    eax
        dec    esp
        push   eax
loc_3_3C0E:
        and    byte ptr [eax], ah
        push   ebx
        pop    ecx
        push   ebx
        add    byte ptr [eax + 49h], cl
        dec    ebp
        inc    ebp
        dec    ebp
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 59h], dl
        push   ebx
        add    byte ptr [ebx + 45h], dl
        push   esp
        push   esi
        inc    ebp
        push   edx
        and    byte ptr [eax], ah
        inc    ebp
        pop    eax
        inc    ebp
        add    byte ptr [edx + eax*2 + 4ch], al
        inc    edx
        push   ebp
        inc    esi
        inc    esi
        and    byte ptr [ebx + 59h], dl
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3C49:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3CCB:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx + 4fh], cl
        push   ebx
        dec    ecx
        dec    esi
        dec    ecx
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h
loc_3_3CEA:
        push   eax
        dec    ebp
        inc    esp
        jb     SHORT loc_3_3D58
        db  076h, 065h
        db  072h, 04Eh
        popad
        insd
        db  065h, 000h
loc_3_3CF7:
        inc    ebx
        inc    esp
        push   ebp
        outsb
        db  073h, 061h
        add    byte ptr gs:[ebx + 61h], dl
        dec    sp
        imul   esi, dword ptr [ebx + 74h], 0
        push   ebp
        dec    esi
        push   ebx
        inc    ecx
        inc    esi
        inc    ebp
        and    byte ptr [ebx + 44h], al
        push   edx
        dec    edi
        dec    ebp
        and    byte ptr [edx + edx*2 + 49h], al
        push   esi
        inc    ebp
        push   edx
        push   ebx
        add    byte ptr [eax], al
loc_3_3D22:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_3D26:
        inc    esp
        outsd
        db  077h, 06Eh
        jb     SHORT loc_3_3D91
        jbe    SHORT loc_3_3D4E
        db  076h, 065h
        jb     SHORT loc_3_3DA5
        imul   ebp, dword ptr [edi + 6eh], 20666f20h
        dec    ecx
        inc    esp
        inc    ebp
        and    byte ptr [ebx + 6fh], dh
        db  066h, 074h, 077h
        popad
        db  072h, 065h
        and    byte ptr [ebx + 6fh], ah
        outsb
        je     SHORT loc_3_3DBD
        outsd
        insb
        insb
loc_3_3D4E:
        imul   ebp, dword ptr [esi + 67h], 6f6f7420h
        and    byte ptr [ebp + 61h], ch
loc_3_3D58:
        outsb
        jns    SHORT loc_3_3D7B
        imul   ebp, dword ptr [esi + 74h], 20333120h
        jb     NEAR PTR loc_3_3DCE
        jbe    SHORT loc_3_3DCC
        db  073h, 02Eh
        db  00Dh, 00Ah, 000h
loc_3_3D6C:
        push   esp
        outsd
        outsd
        and    byte ptr [ebp + 61h], ch
        outsb
        db  079h, 020h
        push   esp
        push   ebx
        push   edx
        jae    SHORT loc_3_3D9A
        popad
loc_3_3D7B:
        db  072h, 065h
        and    byte ptr [eax + 6fh], ch
        outsd
        imul   ebp, dword ptr [ecx + 6eh], 67h
        and    byte ptr [ecx + 4eh], cl
        push   esp
        and    byte ptr [ecx], dh
        xor    esp, dword ptr [eax]
        and    byte ptr es:[eax + 2fh], ch
loc_3_3D91:
        ja     SHORT loc_3_3DB3
        imul   ebp, dword ptr [esi + 74h], 75727265h
loc_3_3D9A:
        db  070h, 074h
        jae    SHORT loc_3_3DCC
        or     eax, 6e55000ah
        dec    ebx
        outsb
loc_3_3DA5:
        outsd
        ja     SHORT loc_3_3E16
        and    byte ptr [ecx + 6eh], ch
        je     SHORT loc_3_3DCD
        xor    dword ptr [ebx], esi
        and    byte ptr [eax + 6fh], ch
        outsd
loc_3_3DB3:
        imul   esp, dword ptr [ebp + 72h], 20h
        and    eax, 0a0d73h
loc_3_3DBC:
        inc    esp
loc_3_3DBD:
        dec    edi
        pop    edi
        dec    esi
        dec    edi
        push   esp
        pop    edi
        inc    ebx
        inc    ecx
        push   edx
        inc    ebp
        sub    ecx, dword ptr [ebp + 55h]
        push   ebx
        push   esp
loc_3_3DCC:
        pop    edi
loc_3_3DCD:
        inc    ebx
loc_3_3DCE:
        dec    eax
        inc    ecx
        dec    ecx
        dec    esi
        and    byte ptr [ecx + 6eh], ch
        db  074h, 031h
        xor    esp, dword ptr [eax]
        db  071h, 075h
        popad
        insb
        imul   esp, dword ptr [esi + 69h], 63207265h
        outsd
        insd
        bound  ebp, qword ptr [ecx + 6eh]
        popad
        je     SHORT loc_3_3E55
        outsd
        outsb
        or     eax, 5520000ah
        jae    SHORT loc_3_3E5E
        outsb
        and    byte ptr [bp + si + 4dh], dl
        dec    ebp
        and    byte ptr [esi + 6fh], ah
        db  072h, 020h
        popad
        insb
        insb
        and    byte ptr [edi + ebp*2 + 67h], ch
        imul   esp, dword ptr [ebx + 61h], 7264206ch
        imul   esi, dword ptr [esi + 65h], 43202e73h
        popad
loc_3_3E16:
        outsb
        and    byte ptr [ebx + 74h], dh
        imul   ebp, dword ptr [esp + ebp*2 + 20h], 64616f6ch
        and    byte ptr [eax + 6fh], dl
        db  072h, 074h
        and    byte ptr [edx + esi*2 + 69h], al
        db  076h, 065h
        db  072h, 073h
        or     eax, 6c49000ah
        insb
        popad
        insb
        and    byte ptr [ecx + 6eh], ch
        je     SHORT loc_3_3E5E
        xor    dword ptr [ebx], esi
        and    byte ptr [ecx + 75h], dl
        popad
        insb
        imul   esp, dword ptr [esi + 69h], 63207265h
        outsd
        insd
        bound  ebp, qword ptr [ecx + 6eh]
        popad
        db  074h, 069h
        outsd
loc_3_3E55:
        outsb
        and    byte ptr cs:[eax + 75h], dl
        outsb
        db  074h, 020h
        push   eax
loc_3_3E5E:
        outsd
        jb     SHORT loc_3_3ED5
        and    byte ptr [edx + esi*2 + 69h], al
        jbe    SHORT loc_3_3ECC
        db  072h, 073h
        or     eax, 6e49000ah
        arpl   word ptr [edi + 6dh], bp
        jo     SHORT loc_3_3ED4
        db  074h, 069h
        bound  ebp, qword ptr [ebp + 20h]
        db  064h, 072h, 069h
        db  076h, 065h
        db  072h, 020h
        and    eax, 20202e73h
        dec    esp
        outsd
        popad
        and    byte ptr fs:[esi + 61h], al
        jae    SHORT loc_3_3F02
        inc    esp
        imul   esi, dword ptr [ebx + 6bh], 726f6620h
        and    byte ptr [edx + esi*2 + 69h], ah
        db  076h, 065h
        db  072h, 020h
        imul   esp, dword ptr [esi + 20h], 73736f70h
        imul   esp, dword ptr [edx + 6ch], 0a0d65h
loc_3_3EAC:
        push   ebp
        outsb
        db  073h, 061h
        and    byte ptr gs:[edx + esi*2 + 69h], ah
        db  076h, 065h
        db  072h, 020h
        and    eax, 6f632073h
        outsb
        db  074h, 072h
        outsd
        insb
        insb
        imul   ebp, dword ptr [esi + 67h], 696e7520h
loc_3_3ECC:
        je     SHORT loc_3_3EEE
        and    eax, 0a0d62h
loc_3_3ED3:
        dec    ebp
loc_3_3ED4:
        outsd
loc_3_3ED5:
        outsb
        outsd
        insb
        imul   esi, dword ptr [eax + ebp*2 + 69h], 72642063h
        imul   esi, dword ptr [esi + 65h], 73252072h
        and    byte ptr [ebx + 6fh], ah
        outsb
        db  074h, 072h
        outsd
loc_3_3EEE:
        insb
        insb
        imul   ebp, dword ptr [esi + 67h], 696e7520h
        db  074h, 020h
        and    eax, 0a0d62h
loc_3_3EFE:
        push   ebp
        outsb
        jae    SHORT loc_3_3F63
loc_3_3F02:
        and    byte ptr gs:[ebx + 44h], al
        push   edx
        dec    edi
        dec    ebp
        and    byte ptr [edx + esi*2 + 69h], ah
        db  076h, 065h
        db  072h, 02Ch
        and    byte ptr [64202c73h], ah
        imul   esi, dword ptr [ebx + 61h], 6e696c62h
        and    byte ptr [bx + si + 72h], dh
        outsd
        je     SHORT loc_3_3F8B
        arpl   word ptr [eax + 6dh], si
        outsd
        and    byte ptr gs:[ebx + 44h], al
        push   edx
        dec    edi
        dec    ebp
        or     eax, 6e55000ah
        db  073h, 061h
        and    byte ptr gs:[edx + esi*2 + 69h], ah
        db  076h, 065h
        db  072h, 020h
        and    eax, 6f682073h
        outsd
        imul   ebp, dword ptr [ecx + 6eh], 67h
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [esi + 6fh], ah
        insb
        insb
        and    byte ptr cs:[edi + ebp*2 + 67h], ch
        imul   esp, dword ptr [ebx + 61h], 6e75206ch
loc_3_3F63:
        imul   esi, dword ptr [ebx + esi*2 + 2eh], 20000a0dh
        and    byte ptr [ebp + 6eh], dh
        imul   esi, dword ptr [eax + 6eh], 65626d75h
        db  072h, 03Ah
        and    byte ptr [0a0d62h], ah
loc_3_3F7E:
        inc    ebp
        db  072h, 072h
        outsd
        jb     SHORT loc_3_3FA4
        imul   esi, dword ptr [ebx + 73h], 676e6975h
loc_3_3F8B:
        and    byte ptr [ecx + 6eh], ch
        db  074h, 020h
        xor    dh, byte ptr [75203a68h]
        outsb
        imul   esi, dword ptr [eax + 25h], 65202c62h
        db  072h, 072h
        outsd
        db  072h, 020h
loc_3_3FA4:
        and    eax, 0a0d65h
loc_3_3FA9:
        push   eax
        jne    SHORT loc_3_401A
        je     SHORT loc_3_4017
        outsb
        and    byte ptr [di + 69h], ch
        outsb
        imul   esi, dword ptr [eax + 6fh], 20737472h
        bound  esp, qword ptr [ebp + 63h]
        popad
        jne    SHORT loc_3_4034
        and    byte ptr gs:[edi + 66h], ch
        and    byte ptr [ebp + 6eh], dh
        imul   ebp, dword ptr [esi + 6fh], 77h
        outsb
        and    byte ptr [ecx + 53h], al
        push   eax
        dec    ecx
        and    byte ptr [edx + esi*2 + 69h], ah
        db  076h, 065h
        db  072h, 020h
        and    eax, 0a0d73h
loc_3_3FDF:
        push   eax
        jne    SHORT loc_3_4050
        je     SHORT loc_3_404D
        outsb
        and    byte ptr [di + 69h], ch
        outsb
        imul   esi, dword ptr [eax + 6fh], 20737472h
        bound  esp, qword ptr [ebp + 63h]
        popad
        jne    SHORT loc_3_406A
        and    byte ptr gs:[edi + 66h], ch
        and    byte ptr [ebp + 6eh], dh
        imul   ebp, dword ptr [esi + 6fh], 77h
        outsb
        and    byte ptr [ebx + 41h], al
        dec    ebp
        and    byte ptr [edx + esi*2 + 69h], ah
        db  076h, 065h
        db  072h, 020h
        and    eax, 0a0d73h
loc_3_4014:
        push   eax
        db  075h, 06Eh
loc_3_4017:
        db  074h, 069h
        outsb
loc_3_401A:
        and    byte ptr [di + 69h], ch
        outsb
        imul   esi, dword ptr [eax + 6fh], 20737472h
        bound  esp, qword ptr [ebp + 63h]
        popad
        jne    SHORT loc_3_409F
        and    byte ptr gs:[edi + 66h], ch
        and    byte ptr [edi + esi*2 + 6fh], dh
loc_3_4034:
        and    byte ptr [ecx + 64h], ch
        outsb
        db  074h, 069h
        arpl   word ptr [ecx + 6ch], sp
        and    byte ptr [eax + 65h], dh
        jb     SHORT loc_3_40AC
        jo     SHORT loc_3_40AD
        db  065h, 072h, 061h
        insb
        jae    SHORT loc_3_4058
        or     al, byte ptr [eax]
loc_3_404D:
        inc    ebx
        inc    ecx
        dec    ebp
loc_3_4050:
        and    byte ptr [edx + esi*2 + 69h], ah
        db  076h, 065h
        db  072h, 020h
loc_3_4058:
        insb
        outsd
        popad
        and    byte ptr fs:[esi + 72h], ah
        outsd
        insd
        and    byte ptr [ecx + 55h], al
        push   esp
        dec    edi
        inc    ebp
        pop    eax
loc_3_406A:
        inc    ebp
        inc    ebx
        inc    edx
        inc    ecx
        push   esp
        and    byte ptr [edi + 72h], ch
        and    byte ptr [ecx + 74h], ch
        db  065h, 072h, 00Dh
        or     al, byte ptr [eax]
loc_3_407C:
        inc    ecx
        push   ebx
        push   eax
        dec    ecx
        and    byte ptr [edx + esi*2 + 69h], ah
        jbe    SHORT loc_3_40EB
        db  072h, 020h
        insb
        outsd
        popad
        and    byte ptr fs:[esi + 72h], ah
        outsd
        insd
        and    byte ptr [ecx + 55h], al
        push   esp
        dec    edi
        inc    ebp
        pop    eax
        inc    ebp
        inc    ebx
        inc    edx
        inc    ecx
loc_3_409F:
        push   esp
        and    byte ptr [edi + 72h], ch
        and    byte ptr [ecx + 74h], ch
        db  065h, 072h, 00Dh
        or     al, byte ptr [eax]
loc_3_40AC:
        push   ebp
loc_3_40AD:
        outsb
        dec    ebx
        outsb
        outsd
        ja     SHORT loc_3_4121
        and    byte ptr [ecx + 6eh], ch
        db  074h, 020h
        xor    dword ptr [ebx], esi
        and    byte ptr [eax + 6fh], ch
        outsd
        imul   esp, dword ptr [ebp + 72h], 20h
        and    eax, 0a0d73h
        db  000h, 00Dh, 00Ah, 000h, 0F7h, 040h
        add    byte ptr [eax], al
        push   es
        inc    ecx
        add    byte ptr [eax], al
        sbb    eax, dword ptr [ecx]
        db  000h, 02Dh, 041h, 000h, 000h, 040h
        inc    ecx
        add    byte ptr [eax], al
        push   edx
        inc    ecx
        add    byte ptr [eax], al
        inc    ecx
        add    byte ptr [eax], al
        db  075h, 041h
        add    byte ptr [eax], al
loc_3_40EB:
        db  0A3h, 041h, 000h, 000h, 0C0h
        inc    ecx
        add    byte ptr [eax], al
        loope  loc_3_4136
        add    byte ptr [eax], al
loc_3_40F7:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        inc    edx
        popad
        and    byte ptr fs:[ecx + 6eh], cl
        outsw
        cmp    ah, byte ptr [eax]
        add    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [edx + esi*2 + 69h], al
        db  076h, 065h
        db  072h, 020h
        inc    ecx
        jb     NEAR PTR loc_3_417B
        db  073h, 073h
        cmp    ah, byte ptr [eax]
        add    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ecx + 4eh], cl
loc_3_4121:
        push   esp
        and    byte ptr [ecx], dh
        xor    esp, dword ptr [eax]
        dec    eax
        outsd
        outsd
        imul   edi, dword ptr [edx], 20h
        add    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ecx + 4fh], cl
        push   ebx
        pop    ecx
        push   ebx
loc_3_4136:
        and    byte ptr [ecx + 4eh], cl
        push   esp
        and    byte ptr [ecx], dh
        xor    edi, dword ptr [edx]
        and    byte ptr [eax], al
loc_3_4140:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        dec    ecx
        dec    esi
        push   esp
        and    byte ptr [edx + eax*2], dh
        and    byte ptr [eax + 6fh], cl
        outsd
        imul   edi, dword ptr [edx], 20h
        add    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ecx + 4eh], cl
        push   esp
        and    byte ptr [esi + eax*2], dh
        and    byte ptr [eax + 6fh], cl
        outsd
        imul   edi, dword ptr [edx], 20h
        add    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ecx + 53h], al
        push   eax
        dec    ecx
        and    byte ptr [ebp + 6eh], al
        je     SHORT loc_3_41E3
        db  079h, 03Ah
        and    byte ptr [eax], al
loc_3_4175:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        dec    eax
        outsd
loc_3_417B:
        outsd
        imul   esi, dword ptr [ebx + 20h], 74h
        push   6f462065h
        insb
        insb
        and    byte ptr cs:[edi + ebp*2 + 67h], ch
        imul   esp, dword ptr [ebx + 61h], 6e75206ch
        imul   esi, dword ptr [ebx + esi*2 + 20h], 413d3028h
        sub    al, 2eh
        sub    dword ptr cs:[edx], edi
        and    byte ptr [eax], al
loc_3_41A3:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        inc    ebx
        outsd
        outsb
        imul   sp, word ptr [edi + 2eh], 7973h
        db  073h, 020h
        insb
        imul   ebp, dword ptr [esi + 65h], 6d756e20h
        bound  esp, qword ptr [ebp + 72h]
        cmp    ah, byte ptr [eax]
        add    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 6fh], al
        outsb
        imul   sp, word ptr [edi + 2eh], 7973h
        db  073h, 020h
        popad
        db  074h, 065h
        das
        db  074h, 069h
        insd
        and    byte ptr gs:[ebx + 74h], dh
        popad
        insd
        jo     SHORT loc_3_4219
        and    byte ptr [eax], al
loc_3_41E1:
        and    byte ptr [eax], ah
loc_3_41E3:
        and    byte ptr [eax], ah
        dec    eax
        popad
        db  072h, 064h
        ja     SHORT loc_3_424C
        db  072h, 065h
        and    byte ptr [ecx + 6eh], ch
        je     SHORT loc_3_4257
        db  072h, 072h
        db  075h, 070h
        db  074h, 020h
        push   206b6f6fh
        insd
        popad
        jo     SHORT loc_3_423B
        and    byte ptr [eax], al
loc_3_4203:
        inc    esp
        jb     SHORT loc_3_426F
        db  076h, 065h
        db  072h, 020h
        dec    esi
        popad
        insd
        cmp    ah, byte ptr gs:[eax]
        add    byte ptr [eax], ah
        and    byte ptr [edx + 6ch], al
        outsd
        arpl   word ptr [ebx + 20h], bp
loc_3_4219:
        inc    esp
        jb     SHORT loc_3_4285
        db  076h, 065h
        db  072h, 020h
        arpl   word ptr [edi + 6eh], bp
        db  074h, 072h
        outsd
        insb
        insb
        imul   ebp, dword ptr [esi + 67h], 20622520h
        jne    SHORT loc_3_429F
        imul   esi, dword ptr [eax + ebp + 73h], 0a0d29h
loc_3_4239:
        and    byte ptr [eax], ah
loc_3_423B:
        inc    ebx
        push   63617261h
        je     SHORT loc_3_42A8
        db  072h, 020h
        inc    esp
        db  072h, 069h
        db  076h, 065h
        jb     SHORT loc_3_4259
loc_3_424C:
        or     al, byte ptr [eax]
loc_3_424E:
        and    byte ptr [eax], ah
        inc    esp
        db  072h, 069h
        db  076h, 065h
        db  072h, 020h
loc_3_4257:
        dec    ecx
        outsb
loc_3_4259:
        outsw
        cmp    ah, byte ptr [eax]
        or     eax, 6525000ah
        or     eax, 7541000ah
        db  074h, 06Fh
        db  065h, 078h, 065h
        arpl   word ptr [eax], sp
        inc    esp
loc_3_426F:
        popad
        db  074h, 065h
        das
        push   esp
        imul   ebp, dword ptr [ebp + 65h], 61745320h
        insd
        db  070h, 03Ah
        and    byte ptr [eax], al
loc_3_4280:
        and    eax, 0a0d65h
loc_3_4285:
        push   esp
        push   ebx
        push   edx
        and    byte ptr [esi + 61h], cl
        insd
        cmp    ah, byte ptr gs:[eax]
        add    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        inc    ecx
        db  075h, 074h
        outsd
        db  065h, 078h, 065h
        arpl   word ptr [eax], sp
        push   eax
loc_3_429F:
        outsd
        db  073h, 069h
        db  074h, 069h
        outsd
        outsb
        cmp    ah, byte ptr [eax]
loc_3_42A8:
        add    byte ptr [0a0d65h], ah
loc_3_42AE:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax + 61h], cl
        db  072h, 064h
        db  077h, 061h
        db  072h, 065h
        and    byte ptr [ecx + 6eh], cl
        db  074h, 065h
        db  072h, 072h
        db  075h, 070h
        je     SHORT loc_3_42E6
        dec    eax
        outsd
        outsd
        imul   esp, dword ptr [eax], 42h
        imul   esi, dword ptr [eax + 4dh], 203a7061h
        add    byte ptr [0a0d65h], ah
loc_3_42DA:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax + 6fh], cl
        outsd
        imul   esi, dword ptr [ebx + 3ah], 20h
loc_3_42E6:
        dec    ecx
        outsb
        db  074h, 020h
        xor    dword ptr [ebx], esi
        or     eax, 2020000ah
        and    byte ptr [eax], ah
        and    byte ptr [eax + 6fh], cl
        outsd
        imul   esi, dword ptr [ebx + 3ah], 20h
        db  075h, 06Eh
        imul   esi, dword ptr [eax + 6eh], 65626d75h
        db  072h, 03Ah
        and    byte ptr [eax], al
loc_3_4309:
        and    eax, 0a0d62h
        add    byte ptr [eax], al
_ITEXT ends
        end
