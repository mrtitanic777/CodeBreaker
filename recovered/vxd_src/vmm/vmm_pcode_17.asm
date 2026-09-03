; vmm_pcode_17.asm -- VMM object 17 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_17_0000:
        stc
        pushfd
        pushad
        db  0A1h, 084h, 0F8h, 000h, 000h
        lea    eax, [eax*4 + 0dch]
        push   edx
        push   1
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    edx
        or     eax, eax
        db  00Fh, 084h, 025h, 000h, 0EFh, 0FFh
        add    eax, 5ch
        mov    edi, eax
        call   sub_17_09A0
        db  00Fh, 082h, 003h, 000h, 0EFh, 0FFh
        call   sub_17_007C
        db  00Fh, 082h, 0F3h, 0FFh, 0EEh, 0FFh
        call   sub_17_0064
        db  00Fh, 082h, 0E3h, 0FFh, 0EEh, 0FFh
        call   sub_17_01A8
        db  08Bh, 01Dh, 010h, 0F7h, 000h, 000h
        mov    dword ptr [esp + 10h], ebx
        inc    dword ptr [ebx - 6ch]
        mov    dword ptr [esp], edi
        popad
        popfd
        clc
        ret
        db  000h
sub_17_0064:
        pushad
        mov    eax, 1
        mov    ecx, 18h
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_17_007A
        mov    dword ptr [edi + 6ch], esi
loc_17_007A:
        popad
        ret
sub_17_007C:
        call   sub_17_012C
        jb     NEAR PTR loc_17_012B
        pushad
        movzx  eax, word ptr [edi - 2eh]
        push   0
        push   0
        push   eax
        db  0CDh, 020h, 07Bh, 000h, 001h, 000h
        add    esp, 0ch
        and    edx, 0ffbfffffh
        push   0
        push   1
        push   eax
        push   edx
        push   ebx
        db  0CDh, 020h, 078h, 000h, 001h, 000h
        add    esp, 14h
        mov    word ptr [edi - 30h], ax
        test   eax, eax
        je     SHORT loc_17_0124
        mov    dword ptr [edi + 60h], 60h
        mov    byte ptr [edi + 73h], 0c0h
        sub    dword ptr [edi + 58h], 10h
        push   100h
        push   0
        push   1
        push   dword ptr [edi + 50h]
        db  0CDh, 020h, 056h, 000h, 001h, 000h
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_17_010C
        mov    ebp, dword ptr [edi + 10h]
        db  0A1h, 0FCh, 0D9h, 000h, 000h
        mov    word ptr [ebp + 58h], ax
        mov    eax, 240h
        mov    dword ptr [ebp + 54h], eax
        clc
        jmp    SHORT loc_17_012A
        db  068h, 000h, 001h, 000h, 000h, 06Ah, 000h, 06Ah, 001h, 0FFh, 077h, 050h, 0CDh, 020h, 057h, 000h
        db  001h, 000h, 083h, 0C4h, 010h
loc_17_010C:
        mov    ax, word ptr [edi - 30h]
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 079h, 000h, 001h, 000h
        add    esp, 0ch
        xor    ax, ax
        mov    word ptr [edi - 30h], ax
loc_17_0124:
        db  0E8h, 064h, 0FFh, 0EEh, 0FFh
        stc
loc_17_012A:
        popad
loc_17_012B:
        ret
sub_17_012C:
        pushad
        xor    eax, eax
        lea    ecx, [eax + 10h]
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_17_01A2
        mov    dword ptr [edi + 48h], esi
        push   1
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   1
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        test   eax, eax
        je     SHORT loc_17_0199
        mov    dword ptr [edi + 50h], eax
        push   0
        push   40h
        push   92h
        push   0fffh
        push   edx
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   eax
        push   edx
        db  0CDh, 020h, 076h, 000h, 001h, 000h
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_17_018C
        mov    word ptr [edi - 2eh], ax
        clc
        popad
        ret
loc_17_018C:
        push   0
        push   dword ptr [edi + 50h]
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_17_0199:
        mov    esi, dword ptr [edi + 48h]
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_17_01A2:
        stc
        popad
        ret
        db  000h, 000h, 000h
sub_17_01A8:
        lea    ecx, [edi - 48h]
        mov    dword ptr [ecx], ecx
        mov    dword ptr [ecx + 4], ecx
        mov    dword ptr [ecx + 8], 0ffffffffh
        ret
loc_17_01B8:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 01Dh, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    14h
        db  033h, 0C9h, 0CDh, 020h, 025h, 000h, 001h, 000h, 0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h
sub_17_01E0:
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 14h], 0
        mov    dword ptr [esp + 24h], 0ffffffffh
        cmp    dword ptr [esp + 3ch], 0
        je     NEAR PTR loc_17_0527
        cmp    dword ptr [esp + 3ch], 7fc00h
        ja     NEAR PTR loc_17_0527
        mov    esi, dword ptr [esp + 40h]
        mov    eax, esi
        and    eax, 60019h
        cmp    eax, esi
        jne    NEAR PTR loc_17_0527
        mov    ebx, dword ptr [esp + 38h]
        mov    dword ptr [esp + 30h], ebx
        cmp    ebx, 80060000h
        jne    SHORT loc_17_023A
        mov    ebx, 80000h
        jmp    SHORT loc_17_024F
loc_17_023A:
        cmp    ebx, 80080000h
        jne    SHORT loc_17_0249
        mov    ebx, 0c0000h
        jmp    SHORT loc_17_024F
loc_17_0249:
        and    ebx, 7fffffffh
loc_17_024F:
        cmp    ebx, 7ffffh
        ja     SHORT loc_17_026D
        mov    eax, ebx
        and    eax, 0fh
        add    eax, dword ptr [esp + 3ch]
        add    eax, 0fh
        and    eax, 0fffffff0h
        mov    dword ptr [esp + 3ch], eax
        and    ebx, 0fffffff0h
loc_17_026D:
        push   ebx
        db  0E8h, 03Dh, 036h, 0EFh, 0FFh
        mov    edx, eax
        mov    dword ptr [esp + 18h], eax
        mov    edi, dword ptr [edx + 8]
        mov    ecx, edi
        mov    ebp, dword ptr [edi + 8]
        test   byte ptr [esp + 33h], 80h
        jne    SHORT loc_17_0306
        mov    edx, dword ptr [esp + 3ch]
        lea    eax, [ebx + edx - 1]
        cmp    eax, 0ffbffh
        ja     NEAR PTR loc_17_0517
        mov    edx, dword ptr [ebp + 4]
        shr    edx, 0ch
        add    edx, dword ptr [ebp]
        cmp    edx, eax
        ja     SHORT loc_17_02BE
loc_17_02A8:
        cmp    ebp, ecx
        je     SHORT loc_17_02BE
        mov    edi, ebp
        mov    ebp, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 4]
        shr    edx, 0ch
        add    edx, dword ptr [ebp]
        cmp    edx, eax
        jbe    SHORT loc_17_02A8
loc_17_02BE:
        mov    eax, dword ptr [ebp + 4]
        shr    eax, 0ch
        cmp    eax, ebx
        ja     NEAR PTR loc_17_0517
loc_17_02CC:
        mov    eax, dword ptr [ebp + 4]
        shr    eax, 0ch
        cmp    eax, ebx
        jae    SHORT loc_17_02F5
        mov    ecx, ebx
        sub    ecx, eax
        push   ecx
        push   ebp
        call   sub_17_0533
        mov    dword ptr [esp + 14h], eax
        or     eax, eax
        je     NEAR PTR loc_17_0517
        mov    dword ptr [eax + 8], ebp
        mov    dword ptr [edi + 8], eax
        mov    edi, eax
loc_17_02F5:
        mov    edx, dword ptr [esp + 3ch]
        cmp    dword ptr [ebp], edx
        jne    SHORT loc_17_0345
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 8], eax
        jmp    SHORT loc_17_037F
loc_17_0306:
        mov    edx, dword ptr [ebp]
        cmp    edx, dword ptr [esp + 3ch]
        jb     SHORT loc_17_0337
        mov    eax, dword ptr [ebp + 4]
        shr    eax, 0ch
        mov    ebx, eax
        test   esi, 1
        je     SHORT loc_17_02F5
        lea    ebx, [eax + 3ffh]
        and    ebx, 0fffffc00h
        add    eax, edx
        mov    edx, dword ptr [esp + 3ch]
        add    edx, ebx
        cmp    eax, edx
        jae    SHORT loc_17_02CC
loc_17_0337:
        mov    edi, ebp
        mov    ebp, dword ptr [ebp + 8]
        cmp    ebp, ecx
        jne    SHORT loc_17_0306
        jmp    NEAR PTR loc_17_0517
loc_17_0345:
        push   dword ptr [esp + 3ch]
        push   ebp
        call   sub_17_0533
        or     eax, eax
        jne    SHORT loc_17_037D
        cmp    dword ptr [esp + 14h], 0
        je     NEAR PTR loc_17_0517
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [ebp]
        add    dword ptr [edi], eax
        db  08Bh, 00Dh, 0ECh, 0FAh, 000h, 000h
        mov    eax, dword ptr [ecx - 4]
        mov    dword ptr [ebp], eax
        mov    dword ptr [ecx - 4], ebp
        jmp    NEAR PTR loc_17_0517
loc_17_037D:
        mov    ebp, eax
loc_17_037F:
        mov    eax, esi
        shr    eax, 11h
        or     esi, eax
        mov    eax, ebp
        db  08Bh, 00Dh, 0ECh, 0FAh, 000h, 000h
        sub    eax, ecx
        mov    edi, 0ch
        sub    edx, edx
        div    edi
        and    esi, 0fffh
        mov    edx, dword ptr [esp + 18h]
        or     dword ptr [ebp + 4], esi
        mov    dx, word ptr [edx + 14h]
        mov    word ptr [ebp + 0ah], dx
        mov    edx, dword ptr [esp + 18h]
        mov    esi, dword ptr [edx + 0ch]
        mov    dx, word ptr [esi + 8]
        add    esi, 8
        mov    word ptr [ebp + 8], dx
        movzx  edx, dx
        imul   edx, edi
        mov    word ptr [ecx + edx + 0ah], ax
        mov    word ptr [esi], ax
        shl    eax, 0ch
        mov    dword ptr [esp + 2ch], eax
        lea    eax, [ebx*4 - 800000h]
        shr    eax, 0ch
        mov    dword ptr [esp + 20h], eax
        mov    edx, dword ptr [esp + 3ch]
        add    edx, ebx
        lea    eax, [edx*4]
        sub    eax, 800004h
        shr    eax, 0ch
        sub    eax, dword ptr [esp + 20h]
        mov    dword ptr [esp + 10h], eax
        mov    ecx, ebx
        and    ecx, 0fffffcffh
        shr    ecx, 8
        lea    eax, [ecx + eax*4 - 402000h]
        sub    ecx, 402000h
        inc    dword ptr [esp + 10h]
        test   byte ptr [ecx + 1], 2
        je     SHORT loc_17_042A
        dec    dword ptr [esp + 10h]
        inc    dword ptr [esp + 20h]
loc_17_042A:
        test   byte ptr [eax + 1], 2
        je     SHORT loc_17_0434
        dec    dword ptr [esp + 10h]
loc_17_0434:
        cmp    dword ptr [esp + 10h], 0
        jle    NEAR PTR loc_17_04D3
        cmp    ebx, 7ffffh
        ja     SHORT loc_17_04A9
        dec    edx
        shr    edx, 0ah
        db  0A1h, 03Ch, 0F8h, 000h, 000h
        lea    esi, [edx + 1]
        mov    dword ptr [esp + 1ch], esi
        cmp    dword ptr [eax + 4], esi
        jae    SHORT loc_17_04A9
        push   0
        lea    eax, [esi*4]
        push   eax
        db  0FFh, 035h, 0E8h, 0F7h, 000h, 000h
        db  0E8h, 06Fh, 010h, 0EFh, 0FFh
        or     eax, eax
        je     SHORT loc_17_04C7
        lea    ecx, [esi*4]
        mov    edi, eax
        mov    esi, 0ffbfe000h
        shr    ecx, 2
        rep movsd
        db  08Bh, 015h, 03Ch, 0F8h, 000h, 000h
        mov    esi, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [edx], eax
        or     ecx, ecx
        mov    dword ptr [edx + 4], esi
        je     SHORT loc_17_04A9
        push   ecx
        db  0FFh, 035h, 0E8h, 0F7h, 000h, 000h
        db  0E8h, 0A7h, 00Fh, 0EFh, 0FFh
loc_17_04A9:
        push   0a0060000h
        push   0
        push   1
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 30h]
        db  0FFh, 015h, 0A0h, 0D7h, 000h, 000h
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_17_04D3
loc_17_04C7:
        push   ebp
        push   dword ptr [esp + 1ch]
        db  0E8h, 094h, 0FDh, 0EEh, 0FFh
        jmp    SHORT loc_17_0517
loc_17_04D3:
        lea    eax, [esp + 28h]
        push   eax
        push   dword ptr [esp + 40h]
        push   ebx
        db  0E8h, 00Ch, 01Dh, 0EFh, 0FFh
        mov    esi, eax
        lea    eax, [ebx*4 - 800000h]
        mov    edx, dword ptr [esp + 3ch]
        lea    ecx, [eax + edx*4]
        cmp    ecx, eax
        jbe    SHORT loc_17_0506
        sub    ecx, eax
        add    ecx, 3
        shr    ecx, 2
        mov    edi, eax
        mov    eax, dword ptr [esp + 2ch]
        rep stosd
loc_17_0506:
        push   dword ptr [esp + 28h]
        push   esi
        db  0E8h, 04Ch, 01Dh, 0EFh, 0FFh
        shl    ebx, 0ch
        mov    dword ptr [esp + 24h], ebx
loc_17_0517:
        mov    edx, dword ptr [esp + 18h]
        push   dword ptr [edx + 10h]
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
loc_17_0527:
        mov    eax, dword ptr [esp + 24h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
sub_17_0533:
        push   esi
        push   edi
        db  0FFh, 035h, 0ECh, 0FAh, 000h, 000h
        db  0E8h, 0E0h, 034h, 0EFh, 0FFh
        or     eax, eax
        jne    SHORT loc_17_0548
        sub    eax, eax
        jmp    SHORT loc_17_056E
loc_17_0548:
        mov    ecx, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 0ch]
        mov    dword ptr [eax], ecx
        sub    dword ptr [edi], ecx
        shl    ecx, 0ch
        mov    esi, dword ptr [edi + 4]
        mov    dword ptr [eax + 4], esi
        add    ecx, esi
        mov    edx, ecx
        xor    edx, esi
        and    edx, 0fffh
        xor    edx, ecx
        mov    dword ptr [edi + 4], edx
loc_17_056E:
        pop    edi
        pop    esi
        ret    8
loc_17_0573:
        push   esi
        push   0
        push   14h
        db  0FFh, 035h, 0E8h, 0F7h, 000h, 000h
        db  0E8h, 05Dh, 00Fh, 0EFh, 0FFh
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_17_060F
        push   0
        push   8
        db  0FFh, 035h, 0E8h, 0F7h, 000h, 000h
        db  0E8h, 044h, 00Fh, 0EFh, 0FFh
        mov    dword ptr [esi], eax
        mov    ecx, eax
        or     ecx, ecx
        je     SHORT loc_17_0601
        mov    eax, 0ffbfe000h
        push   0
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        db  0A1h, 0D0h, 0F7h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [esi + 4], 2
        push   16h
        db  0FFh, 035h, 0E8h, 0F7h, 000h, 000h
        db  0E8h, 015h, 00Fh, 0EFh, 0FFh
        mov    dword ptr [esi + 8], eax
        or     eax, eax
        je     SHORT loc_17_05F4
        push   7fc00h
        push   400h
        push   eax
        call   sub_17_064E
        or     eax, eax
        jne    SHORT loc_17_0613
        push   dword ptr [esi + 8]
        db  0FFh, 035h, 0E8h, 0F7h, 000h, 000h
        db  0E8h, 05Ch, 00Eh, 0EFh, 0FFh
loc_17_05F4:
        push   dword ptr [esi]
        db  0FFh, 035h, 0E8h, 0F7h, 000h, 000h
        db  0E8h, 04Fh, 00Eh, 0EFh, 0FFh
loc_17_0601:
        push   esi
        db  0FFh, 035h, 0E8h, 0F7h, 000h, 000h
        db  0E8h, 043h, 00Eh, 0EFh, 0FFh
        sub    esi, esi
loc_17_060F:
        mov    eax, esi
        pop    esi
        ret
loc_17_0613:
        mov    dword ptr [esi + 10h], 44434443h
        push   5
        db  0FFh, 035h, 09Ch, 0FDh, 000h, 000h
        db  0FFh, 015h, 0F0h, 0F8h, 000h, 000h
        add    esp, 8
        db  0A1h, 03Ch, 0F8h, 000h, 000h
        or     eax, eax
        je     SHORT loc_17_063D
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 0ch], ecx
        mov    dword ptr [eax + 0ch], esi
loc_17_063D:
        db  0FFh, 035h, 09Ch, 0FDh, 000h, 000h
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
        jmp    SHORT loc_17_060F
sub_17_064E:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 14h], 1
        db  08Bh, 035h, 0ECh, 0FAh, 000h, 000h
        push   esi
        db  0E8h, 0B7h, 033h, 0EFh, 0FFh
        mov    dword ptr [esp + 10h], eax
        or     eax, eax
        je     NEAR PTR loc_17_0769
        mov    eax, dword ptr [esp + 20h]
        mov    ecx, dword ptr [esp + 10h]
        shl    eax, 0ch
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [ecx], eax
        push   esi
        db  0E8h, 091h, 033h, 0EFh, 0FFh
        mov    edi, eax
        or     edi, edi
        je     NEAR PTR loc_17_075D
        mov    eax, dword ptr [edi + 4]
        or     eax, 0fffff000h
        mov    dword ptr [edi + 4], eax
        and    eax, 0fffff000h
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [edi], 0
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [ecx + 8], edi
        push   esi
        db  0E8h, 05Eh, 033h, 0EFh, 0FFh
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_17_0755
        mov    dword ptr [ebx + 4], 0
        mov    dword ptr [ebx], 0
        mov    eax, ebx
        sub    eax, esi
        mov    ecx, 0ch
        sub    edx, edx
        div    ecx
        mov    word ptr [ebx + 0ah], ax
        mov    word ptr [ebx + 8], ax
        mov    ebp, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [ebp], ecx
        mov    ecx, dword ptr [esp + 24h]
        add    ecx, dword ptr [esp + 20h]
        dec    ecx
        mov    dword ptr [ebp + 4], ecx
        mov    dword ptr [ebp + 8], edi
        mov    dword ptr [ebp + 0ch], ebx
        mov    word ptr [ebp + 14h], ax
        db  081h, 0FDh, 0F8h, 0FCh, 000h, 000h
        je     SHORT loc_17_077F
        db  081h, 0FDh, 08Ch, 0F8h, 000h, 000h
        je     SHORT loc_17_077F
        db  0F6h, 005h, 030h, 0F9h, 000h, 000h, 008h
        je     SHORT loc_17_0734
        db  0C7h, 045h, 010h, 0C4h, 0F6h, 000h, 000h
        add    ebp, 10h
        jmp    SHORT loc_17_0747
loc_17_0734:
        add    ebp, 10h
        push   1
        push   0
        db  0FFh, 015h, 02Ch, 0D7h, 000h, 000h
        add    esp, 8
        mov    dword ptr [ebp], eax
loc_17_0747:
        cmp    dword ptr [ebp], 0
        jne    SHORT loc_17_0771
        mov    eax, dword ptr [esi - 4]
        mov    dword ptr [ebx], eax
        mov    dword ptr [esi - 4], ebx
loc_17_0755:
        mov    eax, dword ptr [esi - 4]
        mov    dword ptr [edi], eax
        mov    dword ptr [esi - 4], edi
loc_17_075D:
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esi - 4]
        mov    dword ptr [ecx], eax
        mov    dword ptr [esi - 4], ecx
loc_17_0769:
        mov    dword ptr [esp + 14h], 0
loc_17_0771:
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    0ch
loc_17_077F:
        db  0C7h, 045h, 010h, 008h, 0F9h, 000h, 000h
        jmp    SHORT loc_17_0771
_PageAllocate:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 30h]
        test   esi, 2
        je     SHORT loc_17_07A6
        test   esi, 8
        je     NEAR PTR loc_17_0876
loc_17_07A6:
        mov    eax, esi
        and    eax, 188h
        lea    ecx, [eax - 1]
        test   eax, ecx
        je     SHORT loc_17_07C8
        and    esi, 0fffffeffh
        test   esi, 8
        je     SHORT loc_17_07C8
        and    esi, 0ffffff7fh
loc_17_07C8:
        mov    ebx, dword ptr [esp + 18h]
        cmp    ebx, 1
        je     SHORT loc_17_07F4
        cmp    ebx, 7
        je     SHORT loc_17_07DE
        or     ebx, ebx
        jne    NEAR PTR loc_17_0876
loc_17_07DE:
        cmp    ebx, 1
        je     SHORT loc_17_07F4
        push   dword ptr [esp + 1ch]
        db  0E8h, 044h, 04Eh, 0EFh, 0FFh
        or     eax, eax
        je     NEAR PTR loc_17_0876
loc_17_07F4:
        mov    eax, esi
        and    eax, 20000000h
        cmp    eax, 1
        cmc
        sbb    eax, eax
        and    eax, 10h
        mov    ecx, esi
        and    ecx, 8
        or     eax, ecx
        mov    edi, dword ptr [esp + 14h]
        push   eax
        push   edi
        dec    ebx
        cmp    ebx, 1
        sbb    eax, eax
        and    eax, 20000h
        sub    eax, 7ffa0000h
        push   eax
        db  0FFh, 015h, 09Ch, 0D7h, 000h, 000h
        mov    dword ptr [esp + 18h], eax
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_17_0876
        test   esi, 2
        jne    SHORT loc_17_088D
        mov    eax, esi
        and    eax, 20000188h
        or     eax, 60000h
        push   eax
        push   0
        push   esi
        db  0E8h, 069h, 00Bh, 0EFh, 0FFh
        push   eax
        mov    eax, dword ptr [esp + 18h]
        push   edi
        shr    eax, 0ch
        push   eax
        db  0FFh, 015h, 0A0h, 0D7h, 000h, 000h
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_17_087E
loc_17_0868:
        push   10h
        push   dword ptr [esp + 10h]
        db  0E8h, 0BDh, 0F8h, 0EEh, 0FFh
        add    esp, 8
loc_17_0876:
        mov    dword ptr [esp + 0ch], 0
loc_17_087E:
        mov    edx, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 0ch]
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_17_088D:
        mov    eax, dword ptr [esp + 28h]
        cmp    eax, -1
        jne    SHORT loc_17_089B
        mov    eax, 1000h
loc_17_089B:
        push   eax
        and    esi, 1
        push   dword ptr [esp + 28h]
        or     esi, 60000h
        push   dword ptr [esp + 28h]
        push   esi
        mov    eax, dword ptr [esp + 1ch]
        push   edi
        shr    eax, 0ch
        push   eax
        db  0FFh, 015h, 080h, 0D8h, 000h, 000h
        add    esp, 18h
        cmp    eax, -1
        je     SHORT loc_17_0868
        mov    ecx, dword ptr [esp + 2ch]
        or     ecx, ecx
        je     SHORT loc_17_087E
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_17_087E
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_PageLock:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        mov    ecx, esi
        shr    ecx, 0ch
        mov    edi, dword ptr [esp + 1ch]
        add    edi, ecx
        lea    eax, [esp + 0ch]
        mov    ebx, dword ptr [esp + 18h]
        push   eax
        push   ebx
        push   edi
        db  0E8h, 0EBh, 02Fh, 0EFh, 0FFh
        or     eax, eax
        je     SHORT loc_17_0926
        mov    eax, dword ptr [eax + 4]
        and    eax, 0fffff000h
        cmp    eax, esi
        jne    SHORT loc_17_0926
        push   dword ptr [esp + 20h]
        push   ebx
        push   edi
        db  0FFh, 015h, 0B4h, 0D4h, 000h, 000h
        add    esp, 0ch
        jmp    SHORT loc_17_0928
loc_17_0926:
        sub    eax, eax
loc_17_0928:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  000h
sub_17_0930:
        push   esi
        push   edi
        db  066h, 0A1h, 0ACh, 0F6h, 000h, 000h
        db  08Bh, 015h, 014h, 0F7h, 000h, 000h
loc_17_093E:
        mov    di, ax
        inc    ax
        or     di, di
        jne    SHORT loc_17_0954
        mov    di, ax
        inc    ax
        db  0C6h, 005h, 0B8h, 0F6h, 000h, 000h, 001h
loc_17_0954:
        db  066h, 0A3h, 0ACh, 0F6h, 000h, 000h
        db  080h, 03Dh, 0B8h, 0F6h, 000h, 000h, 000h
        je     SHORT loc_17_098A
        mov    ecx, edx
        or     edx, edx
        je     SHORT loc_17_098A
loc_17_0969:
        mov    esi, dword ptr [ecx - 1ch]
        or     esi, esi
        je     SHORT loc_17_097D
loc_17_0970:
        cmp    word ptr [esi + 18h], di
        je     SHORT loc_17_0997
        mov    esi, dword ptr [esi + 40h]
        or     esi, esi
        jne    SHORT loc_17_0970
loc_17_097D:
        db  066h, 0A3h, 0ACh, 0F6h, 000h, 000h
        mov    ecx, dword ptr [ecx + 2ch]
        or     ecx, ecx
        jne    SHORT loc_17_0969
loc_17_098A:
        mov    eax, dword ptr [esp + 0ch]
        mov    word ptr [eax + 18h], di
        pop    edi
        pop    esi
        ret    4
loc_17_0997:
        add    ax, 10h
        jmp    SHORT loc_17_093E
        db  000h, 000h, 000h
sub_17_09A0:
        pushad
        db  08Bh, 01Dh, 010h, 0F7h, 000h, 000h
        mov    eax, dword ptr [edx]
        mov    dword ptr [edi + 2ch], eax
        mov    ecx, 80h
        call   sub_17_0A99
        stc
        je     SHORT loc_17_09F1
        mov    dword ptr [edi], 100100h
        mov    ecx, edi
        db  0E8h, 05Eh, 077h, 0EFh, 0FFh
        mov    ecx, dword ptr [ebx - 64h]
        mov    dword ptr [edi + 3ch], ecx
        mov    esi, 8
        db  0B9h, 044h, 00Bh, 000h, 000h
        call   sub_17_0A2A
        mov    esi, dword ptr [edi + 5ch]
        add    esi, 0ch
        mov    edx, dword ptr [esp + 14h]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [esi + 4], eax
        clc
loc_17_09F1:
        popad
        ret
sub_17_09F3:
        pushad
        db  0E8h, 032h, 077h, 0EFh, 0FFh
        xor    ecx, ecx
        call   sub_17_0A99
        jne    SHORT loc_17_0A0B
        db  089h, 035h, 014h, 0F7h, 000h, 000h
        popad
        stc
        ret
loc_17_0A0B:
        db  0CDh, 020h, 00Dh, 001h, 001h, 000h
        mov    dword ptr [edi + 40h], 0
        mov    dword ptr [ebx - 1ch], edi
        mov    dword ptr [ebx - 20h], edi
        db  0B9h, 0C5h, 00Ah, 000h, 000h
        xor    esi, esi
        db  068h, 07Ah, 00Ah, 000h, 000h
sub_17_0A2A:
        mov    dword ptr [edi + 0ch], 42434854h
        mov    dword ptr [edi - 4], edi
        mov    dword ptr [edi + 14h], ebx
        mov    dword ptr [edi + 54h], eax
        or     dword ptr [edi], 10108h
        add    edx, 0f70h
        lea    eax, [edx + 48h]
        mov    dword ptr [edi + 58h], eax
        mov    dword ptr [edi + 10h], edx
        mov    dword ptr [edi + 4ch], edx
        mov    dword ptr [edx + 2ch], 202h
        sub    edx, 0e4h
        sub    edx, esi
        mov    dword ptr [edx + 8], ecx
        xor    esi, esi
        mov    dword ptr [edx + 4], esi
        mov    dword ptr [edx], esi
        mov    dword ptr [edi + 5ch], edx
        db  0E8h, 07Eh, 07Fh, 0EFh, 0FFh
        push   edi
        call   sub_17_0930
        ret
loc_17_0A7A:
        mov    edx, dword ptr [edi + 10h]
        mov    dword ptr [ebx + 8], edx
        mov    dword ptr [esp], edi
        popad
        clc
        ret
loc_17_0A86:
        push   ecx
        db  0E8h, 016h, 014h, 0EFh, 0FFh
        or     ecx, 0ffffffffh
        test   eax, eax
        je     SHORT loc_17_0A98
        sub    eax, 1000h
loc_17_0A98:
        ret
sub_17_0A99:
        xor    eax, eax
        db  039h, 005h, 0A8h, 0F8h, 000h, 000h
        jne    SHORT loc_17_0A86
        push   ecx
        push   2
        pop    ecx
        db  0F7h, 005h, 0ADh, 00Ah, 000h, 000h, 000h, 000h, 040h, 000h
        je     SHORT loc_17_0AB4
        inc    ecx
loc_17_0AB4:
        push   10h
        push   ecx
        push   80080000h
        db  0CDh, 020h, 01Dh, 001h, 001h, 000h
        add    esp, 0ch
        pop    ecx
        inc    eax
        je     SHORT loc_17_0B24
        shr    eax, 0ch
        db  0F7h, 005h, 0D2h, 00Ah, 000h, 000h, 000h, 000h, 040h, 000h
        je     SHORT loc_17_0AD9
        inc    eax
loc_17_0AD9:
        push   eax
        inc    eax
        or     ecx, 20060000h
        push   ecx
        push   0
        push   1
        push   1
        push   eax
        db  0CDh, 020h, 01Eh, 001h, 001h, 000h
        add    esp, 14h
        mov    ecx, eax
        pop    eax
        mov    edx, eax
        shl    eax, 0ch
        test   ecx, ecx
        je     SHORT loc_17_0B25
        push   eax
        lea    eax, [edx*4 - 800000h]
        shr    eax, 0ch
        push   0
        push   1
        push   eax
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        test   eax, eax
        pop    eax
        je     SHORT loc_17_0B25
        lea    edx, [eax + 1000h]
        test   edx, edx
loc_17_0B24:
        ret
loc_17_0B25:
        db  0F7h, 005h, 02Bh, 00Bh, 000h, 000h, 000h, 000h, 040h, 000h
        je     SHORT loc_17_0B36
        sub    eax, 1000h
loc_17_0B36:
        push   10h
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        xor    eax, eax
        ret
loc_17_0B44:
        mov    eax, 1eh
        db  08Bh, 03Dh, 098h, 0F6h, 000h, 000h
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        pop    esi
        pop    edx
        test   esi, esi
        jmp    SHORT loc_17_0B62
        db  068h, 000h, 003h, 000h, 000h, 0FFh, 0E6h
loc_17_0B62:
        db  0E9h, 099h, 0F7h, 0EEh, 0FFh
loc_17_0B67:
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_17_0BF0
        lea    edx, [esp + 34h]
        mov    eax, 1dh
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        jae    SHORT loc_17_0B8E
        jmp    SHORT loc_17_0BF0
loc_17_0B8E:
        mov    ebp, dword ptr [edi + 10h]
        mov    eax, dword ptr [esp + 1ch]
        mov    word ptr [ebp + 34h], ax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [ebp + 30h], eax
        mov    eax, dword ptr [esp + 24h]
        mov    word ptr [ebp + 28h], ax
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [ebp + 24h], eax
        mov    eax, dword ptr [esp + 2ch]
        mov    word ptr [ebp + 3ch], ax
        mov    eax, dword ptr [esp + 30h]
        mov    word ptr [ebp + 38h], ax
        mov    dword ptr [ebp + 2ch], 202h
        mov    word ptr [ebp + 4ch], 0f000h
        mov    word ptr [ebp + 48h], 0ffffh
        mov    dword ptr [ebp + 50h], 23002h
        and    dword ptr [edi], 0fffffeffh
        push   1
        push   edi
        db  0E8h, 01Eh, 083h, 0EFh, 0FFh
        mov    eax, edi
loc_17_0BE9:
        pop    ebp
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret
loc_17_0BF0:
        mov    eax, 0
        jmp    SHORT loc_17_0BE9
        db  000h
_PTEXT ends
        end
