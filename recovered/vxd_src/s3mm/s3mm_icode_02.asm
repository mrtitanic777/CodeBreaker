; s3mm_icode_02.asm -- S3MM object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

S3MINI_SYS_CRITICAL_INIT:
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a539h
        out    dx, ax
        rol    eax, 8
        out    dx, al
        clc
        ret
loc_2_001B:
        push   ebx
        db  089h, 01Dh, 084h, 014h, 000h, 000h
        db  0C6h, 005h, 032h, 015h, 000h, 000h, 000h
        pushad
        mov    eax, 20h
        db  0BEh, 054h, 008h, 000h, 000h
        db  0BFh, 05Ch, 008h, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        cmp    eax, 20h
        je     SHORT loc_2_004E
        cmp    eax, 40h
        je     SHORT loc_2_004E
        mov    eax, 20h
loc_2_004E:
        shl    eax, 0ah
        db  0A3h, 080h, 014h, 000h, 000h
        popad
        push   0
        push   10000h
        push   0a0000h
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     NEAR PTR loc_2_058B
        db  0A3h, 094h, 014h, 000h, 000h
        db  0C7h, 005h, 0B0h, 014h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0B4h, 014h, 000h, 000h, 0E8h, 09Ah, 000h, 000h
        xor    eax, eax
        mov    edx, 3d4h
        mov    ax, 4838h
        out    dx, ax
        mov    al, 36h
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 3
        db  0A2h, 031h, 015h, 000h, 000h
        dec    edx
        mov    al, 30h
        out    dx, al
        inc    dx
        in     al, dx
        cmp    al, 0e1h
        je     SHORT loc_2_0106
        mov    ch, al
        and    ch, 0f0h
        db  088h, 02Dh, 02Bh, 015h, 000h, 000h
        and    al, 0fh
        db  0A2h, 02Fh, 015h, 000h, 000h
        cmp    ch, 80h
        je     NEAR PTR loc_2_01CC
        cmp    ch, 90h
        je     NEAR PTR loc_2_01CC
        cmp    ch, 0b0h
        je     NEAR PTR loc_2_01CC
        cmp    ch, 0a0h
        je     NEAR PTR loc_2_01CC
        db  0C6h, 005h, 030h, 015h, 000h, 000h, 0FFh
        cmp    ch, 0c0h
        je     NEAR PTR loc_2_01CC
        cmp    ch, 0d0h
        je     NEAR PTR loc_2_01CC
        jmp    NEAR PTR loc_2_058B
loc_2_0106:
        db  0C6h, 005h, 030h, 015h, 000h, 000h, 0FFh
        db  083h, 00Dh, 0B0h, 014h, 000h, 000h, 001h
        xor    eax, eax
        dec    dl
        mov    al, 2dh
        out    dx, al
        inc    dl
        in     al, dx
        shl    eax, 8
        dec    dl
        mov    al, 2eh
        out    dx, al
        inc    dl
        in     al, dx
        shl    eax, 8
        dec    dl
        mov    al, 2fh
        out    dx, al
        inc    dl
        in     al, dx
        db  0A2h, 02Fh, 015h, 000h, 000h
        and    al, 40h
        db  0A3h, 02Bh, 015h, 000h, 000h
        cmp    eax, 881100h
        je     SHORT loc_2_018E
        cmp    eax, 881000h
        je     SHORT loc_2_018E
        cmp    eax, 888000h
        je     SHORT loc_2_01BB
        cmp    eax, 889000h
        je     SHORT loc_2_01BB
        cmp    eax, 88b000h
        je     SHORT loc_2_01BB
        cmp    eax, 88f000h
        je     SHORT loc_2_01BB
        cmp    eax, 881140h
        je     SHORT loc_2_017C
        cmp    eax, 881200h
        jne    NEAR PTR loc_2_058B
loc_2_017C:
        db  080h, 00Dh, 033h, 015h, 000h, 000h, 001h
        db  080h, 03Dh, 031h, 015h, 000h, 000h, 001h
        jne    SHORT loc_2_01BB
        jmp    SHORT loc_2_01C5
loc_2_018E:
        db  080h, 03Dh, 02Fh, 015h, 000h, 000h, 020h
        je     SHORT loc_2_01C5
        db  080h, 03Dh, 02Fh, 015h, 000h, 000h, 030h
        je     SHORT loc_2_01C5
        db  080h, 03Dh, 02Fh, 015h, 000h, 000h, 003h
        je     SHORT loc_2_01C5
        db  080h, 03Dh, 02Fh, 015h, 000h, 000h, 012h
        je     SHORT loc_2_01C5
        db  080h, 03Dh, 02Fh, 015h, 000h, 000h, 022h
        jne    SHORT loc_2_01CC
loc_2_01BB:
        db  0C7h, 005h, 0B4h, 014h, 000h, 000h, 018h, 081h, 000h, 000h
loc_2_01C5:
        db  083h, 00Dh, 0B0h, 014h, 000h, 000h, 002h
loc_2_01CC:
        dec    dl
        mov    al, 30h
        out    dx, al
        inc    dl
        in     al, dx
        inc    al
        mov    ah, al
        out    dx, al
        in     al, dx
        cmp    al, ah
        je     NEAR PTR loc_2_058B
        dec    dl
        db  0B8h, 0DCh, 014h, 000h, 000h
        mov    ecx, 4ah
        mov    word ptr [eax], cx
        db  0CDh, 020h, 014h, 000h, 00Ah, 000h
        db  0A1h, 0E4h, 014h, 000h, 000h
        or     eax, 20202020h
        cmp    eax, 642e3373h
        je     SHORT loc_2_0248
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 080h, 000h, 000h, 000h
        je     NEAR PTR loc_2_058B
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 090h, 000h, 000h, 000h
        je     NEAR PTR loc_2_058B
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 0B0h, 000h, 000h, 000h
        je     NEAR PTR loc_2_058B
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 0A0h, 000h, 000h, 000h
        je     NEAR PTR loc_2_058B
loc_2_0248:
        db  0C6h, 005h, 06Ch, 015h, 000h, 000h, 000h
        db  0B8h, 04Dh, 015h, 000h, 000h
        db  0BAh, 068h, 015h, 000h, 000h
        push   edx
        push   eax
        push   80000002h
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        cmp    eax, 0
        jne    SHORT loc_2_0283
        db  0FEh, 005h, 06Ch, 015h, 000h, 000h
        db  0FFh, 035h, 068h, 015h, 000h, 000h
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
loc_2_0283:
        mov    dx, 3d4h
        mov    al, 36h
        out    dx, al
        inc    dx
        in     al, dx
        dec    dx
        mov    cl, al
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_2_02E2
        shr    cl, 5
        mov    eax, 400000h
        or     cl, cl
        je     SHORT loc_2_02F1
        mov    eax, 300000h
        cmp    cl, 2
        je     SHORT loc_2_02F1
        mov    eax, 800000h
        cmp    cl, 3
        je     SHORT loc_2_02F1
        mov    eax, 200000h
        cmp    cl, 4
        je     SHORT loc_2_02F1
        mov    eax, 600000h
        cmp    cl, 5
        je     SHORT loc_2_02F1
        mov    eax, 100000h
        cmp    cl, 6
        je     SHORT loc_2_02F1
        mov    eax, 80000h
        jmp    SHORT loc_2_02F1
loc_2_02E2:
        mov    eax, 100000h
        test   cl, 20h
        je     SHORT loc_2_02F1
        mov    eax, 80000h
loc_2_02F1:
        db  0A3h, 090h, 014h, 000h, 000h
        push   0
        push   10h
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        add    esp, 8
        db  0A3h, 098h, 014h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_2_058B
        db  0CDh, 020h, 00Eh, 000h, 00Ah, 000h
        cmp    ecx, 31h
        jb     NEAR PTR loc_2_058B
        db  0C7h, 007h, 000h, 000h, 000h, 000h
        db  0C7h, 047h, 004h, 000h, 000h, 000h, 000h
        db  0C7h, 047h, 008h, 02Bh, 001h, 000h, 000h
        db  0C7h, 047h, 00Ch, 0B9h, 001h, 000h, 000h
        db  0C7h, 047h, 058h, 06Eh, 002h, 000h, 000h
        db  0C7h, 047h, 05Ch, 0EFh, 002h, 000h, 000h
        db  0C7h, 047h, 010h, 04Ah, 003h, 000h, 000h
        db  0C7h, 047h, 014h, 0CEh, 004h, 000h, 000h
        db  0C7h, 047h, 018h, 0F9h, 004h, 000h, 000h
        db  0C7h, 047h, 01Ch, 001h, 005h, 000h, 000h
        db  0C7h, 047h, 020h, 049h, 005h, 000h, 000h
        db  0C7h, 047h, 024h, 0C7h, 005h, 000h, 000h
        db  0C7h, 047h, 02Ch, 0F5h, 007h, 000h, 000h
        db  0C7h, 047h, 030h, 045h, 008h, 000h, 000h
        db  0C7h, 047h, 034h, 0B7h, 008h, 000h, 000h
        db  0C7h, 047h, 038h, 096h, 009h, 000h, 000h
        db  0C7h, 047h, 03Ch, 080h, 00Ah, 000h, 000h
        db  0C7h, 047h, 040h, 0B3h, 00Bh, 000h, 000h
        db  0C7h, 047h, 044h, 008h, 00Ch, 000h, 000h
        db  0C7h, 087h, 0B8h, 000h, 000h, 000h, 095h, 005h, 000h, 000h
        db  0C7h, 087h, 0B4h, 000h, 000h, 000h, 05Bh, 005h, 000h, 000h
        db  0C7h, 047h, 068h, 0C4h, 004h, 000h, 000h
        db  0C7h, 047h, 070h, 0C9h, 000h, 000h, 000h
        db  0C7h, 047h, 074h, 09Eh, 004h, 000h, 000h
        db  0C7h, 087h, 090h, 000h, 000h, 000h, 0C8h, 00Ch, 000h, 000h
        db  0C7h, 087h, 080h, 000h, 000h, 000h, 0D0h, 00Ch, 000h, 000h
        db  0C7h, 087h, 084h, 000h, 000h, 000h, 0D0h, 00Ch, 000h, 000h
        db  0C7h, 087h, 094h, 000h, 000h, 000h, 020h, 00Dh, 000h, 000h
        db  0C7h, 087h, 088h, 000h, 000h, 000h, 067h, 00Eh, 000h, 000h
        db  0C7h, 087h, 09Ch, 000h, 000h, 000h, 029h, 00Dh, 000h, 000h
        db  0C7h, 087h, 0A0h, 000h, 000h, 000h, 0EBh, 00Dh, 000h, 000h
        db  0C7h, 087h, 0A8h, 000h, 000h, 000h, 08Eh, 005h, 000h, 000h
        db  0C7h, 087h, 0B0h, 000h, 000h, 000h, 0C1h, 001h, 000h, 000h
        db  0C7h, 087h, 0A4h, 000h, 000h, 000h, 0DBh, 00Eh, 000h, 000h
        db  0C7h, 087h, 0BCh, 000h, 000h, 000h, 022h, 002h, 000h, 000h
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 080h, 000h, 000h, 000h
        jne    SHORT loc_2_0450
        db  0C7h, 087h, 098h, 000h, 000h, 000h, 0CAh, 00Eh, 000h, 000h
loc_2_0450:
        mov    edx, 3c2h
        db  0B9h, 0B0h, 00Ch, 000h, 000h
        db  0CDh, 020h, 013h, 000h, 00Ah, 000h
        db  089h, 00Dh, 0B8h, 014h, 000h, 000h
        db  0CDh, 020h, 011h, 000h, 00Ah, 000h
        db  089h, 035h, 088h, 014h, 000h, 000h
        db  089h, 03Dh, 08Ch, 014h, 000h, 000h
        mov    edx, 9ae8h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0e2e8h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0bee8h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 96e8h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 86e8h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 82e8h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 8ee8h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 8ae8h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0b6e8h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0bae8h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0aae8h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0aee8h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0a2e8h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0a6e8h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 92e8h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        mov    edx, 0b2e8h
        mov    cl, 2
        db  0CDh, 020h, 00Fh, 000h, 00Ah, 000h
        db  0BEh, 05Fh, 00Ah, 000h, 000h
        mov    edx, 4ae8h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_2_058B
        mov    edx, 4ae8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 4ae9h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_2_058B
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        db  08Bh, 01Dh, 084h, 014h, 000h, 000h
        call   sub_2_0594
        db  0A1h, 02Bh, 015h, 000h, 000h
        clc
        jmp    SHORT loc_2_058C
loc_2_058B:
        stc
loc_2_058C:
        pop    ebx
        ret
loc_2_058E:
        db  0A1h, 02Bh, 015h, 000h, 000h
        ret
sub_2_0594:
        push   ebx
        db  089h, 01Dh, 070h, 015h, 000h, 000h
        db  0BEh, 0CCh, 006h, 000h, 000h
        mov    ebx, 8
        call   sub_2_069E
        jb     NEAR PTR loc_2_069B
        db  0BEh, 0D4h, 006h, 000h, 000h
        mov    ebx, 0bh
        call   sub_2_069E
        jae    SHORT loc_2_05F8
        db  0BEh, 0DFh, 006h, 000h, 000h
        mov    ebx, 0ah
        call   sub_2_069E
        jae    SHORT loc_2_05F8
        db  0BEh, 0E9h, 006h, 000h, 000h
        mov    ebx, 0bh
        call   sub_2_069E
        jae    SHORT loc_2_05F8
        db  0BEh, 0F4h, 006h, 000h, 000h
        mov    ebx, 0eh
        call   sub_2_069E
        jb     NEAR PTR loc_2_069B
loc_2_05F8:
        call   sub_2_0706
        db  066h, 0A3h, 0CAh, 015h, 000h, 000h
        db  066h, 089h, 01Dh, 0CCh, 015h, 000h, 000h
        and    ax, 7fffh
        db  0BEh, 0CBh, 002h, 000h, 000h
loc_2_0613:
        test   word ptr [esi], bx
        je     SHORT loc_2_0633
        push   bx
        and    bx, 0c000h
        cmp    bx, word ptr [esi + 2]
        pop    bx
        jne    SHORT loc_2_0633
        test   word ptr [esi + 4], ax
        je     SHORT loc_2_0633
        cmp    cx, word ptr [esi + 6]
        je     SHORT loc_2_063E
loc_2_0633:
        add    esi, 0ch
        cmp    word ptr [esi], 0
        je     SHORT loc_2_069B
        jmp    SHORT loc_2_0613
loc_2_063E:
        mov    esi, dword ptr [esi + 8]
        db  089h, 035h, 0C3h, 015h, 000h, 000h
        db  08Bh, 01Dh, 070h, 015h, 000h, 000h
        mov    ebp, dword ptr [ebx + 8]
        db  0CDh, 020h, 00Eh, 000h, 00Ah, 000h
        cmp    ecx, 31h
        jb     SHORT loc_2_069B
        cmp    dword ptr [edi + 0a8h], 0
        je     SHORT loc_2_069B
        call   dword ptr [edi + 0a8h]
        db  0A2h, 0BEh, 015h, 000h, 000h
        mov    eax, dword ptr [edi + 70h]
        db  0C7h, 047h, 070h, 08Eh, 007h, 000h, 000h
        mov    eax, dword ptr [edi + 74h]
        db  0C7h, 047h, 074h, 0B8h, 005h, 000h, 000h
        mov    eax, dword ptr [edi + 0bch]
        db  0C7h, 087h, 0BCh, 000h, 000h, 000h, 0E0h, 013h, 000h, 000h
        db  0A3h, 0BFh, 015h, 000h, 000h
        clc
        jmp    SHORT loc_2_069C
loc_2_069B:
        stc
loc_2_069C:
        pop    ebx
        ret
sub_2_069E:
        push   eax
        push   edi
        push   ecx
        mov    edi, 0c0000h
        mov    ecx, 100h
loc_2_06AB:
        mov    al, byte ptr [esi]
        repne scasb
        or     ecx, ecx
        je     SHORT loc_2_06C4
        push   esi
        push   edi
        push   ecx
        dec    edi
        mov    ecx, ebx
        repe cmpsb
        or     ecx, ecx
        pop    ecx
        pop    edi
        pop    esi
        je     SHORT loc_2_06C7
        loop   loc_2_06AB
loc_2_06C4:
        stc
        jmp    SHORT loc_2_06C8
loc_2_06C7:
        clc
loc_2_06C8:
        pop    ecx
        pop    edi
        pop    eax
        ret
loc_2_06CC:
        inc    esp
        imul   esp, dword ptr [ecx + 6dh], 646e6fh
loc_2_06D4:
        push   ebx
        db  074h, 065h
        popad
        insb
        je     SHORT loc_2_0743
        and    byte ptr [esi], dh
        xor    al, 0
loc_2_06DF:
        push   ebx
        db  074h, 065h
        popad
        insb
        je     SHORT loc_2_074E
        xor    al, 0
loc_2_06E9:
        push   ebx
        db  074h, 065h
        popad
        insb
        je     SHORT loc_2_0758
        and    byte ptr [ebx + 45h], dl
        db  000h
loc_2_06F4:
        push   ebx
        je     SHORT loc_2_075C
        popad
        insb
        db  074h, 068h
        and    byte ptr [esi + 69h], dl
        outsd
        add    byte ptr [ecx], dh
        xor    dword ptr cs:[eax], eax
sub_2_0706:
        push   dx
        mov    dx, 3d4h
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a039h
        out    dx, ax
        mov    al, 36h
        out    dx, al
        jmp    SHORT loc_2_071D
loc_2_071D:
        in     ax, dx
        jmp    SHORT loc_2_0721
loc_2_0721:
        and    ah, 0c0h
        mov    cx, 4
        or     ah, ah
        je     SHORT loc_2_0739
        mov    cx, 2
        cmp    ah, 80h
        je     SHORT loc_2_0739
        mov    cx, 1
loc_2_0739:
        push   cx
        mov    al, 30h
        out    dx, al
        inc    dx
        in     al, dx
        dec    dx
loc_2_0743:
        mov    ah, al
        and    ah, 0fh
        and    al, 0f0h
        cmp    al, 0d0h
        je     SHORT loc_2_07C2
loc_2_074E:
        cmp    al, 0c0h
        je     SHORT loc_2_077E
        cmp    al, 0e0h
        jne    SHORT loc_2_077E
        mov    al, 2eh
loc_2_0758:
        out    dx, al
        inc    dx
        in     al, dx
loc_2_075C:
        dec    dx
        cmp    al, 80h
        je     SHORT loc_2_07E0
        cmp    al, 90h
        je     NEAR PTR loc_2_07EA
        cmp    al, 0b0h
        je     NEAR PTR loc_2_07F4
        cmp    al, 0f0h
        je     SHORT loc_2_07F4
        cmp    al, 10h
        je     SHORT loc_2_07CC
        cmp    al, 11h
        je     SHORT loc_2_07D6
loc_2_077E:
        mov    bx, 1
        push   esi
        push   ebx
        db  0BEh, 002h, 007h, 000h, 000h
        mov    ebx, 4
        call   sub_2_069E
        pop    ebx
        pop    esi
        jae    SHORT loc_2_079C
        or     bx, 4000h
loc_2_079C:
        mov    cx, 400h
        mov    dx, 3c8h
        in     al, dx
        mov    dx, 3c6h
        in     al, dx
        in     al, dx
        in     al, dx
        in     al, dx
        and    al, 0f0h
        cmp    al, 0b0h
        jne    SHORT loc_2_07B8
        or     cx, 800h
loc_2_07B8:
        mov    dx, 3c8h
        in     al, dx
        jmp    NEAR PTR loc_2_084B
loc_2_07C2:
        mov    bx, 2
        mov    cx, 100h
        jmp    SHORT loc_2_084B
loc_2_07CC:
        mov    bx, 4
        mov    cx, 400h
        jmp    SHORT loc_2_084B
loc_2_07D6:
        mov    bx, 8
        mov    cx, 400h
        jmp    SHORT loc_2_084B
loc_2_07E0:
        mov    bx, 10h
        mov    cx, 400h
        jmp    SHORT loc_2_084B
loc_2_07EA:
        mov    bx, 20h
        mov    cx, 400h
        jmp    SHORT loc_2_084B
loc_2_07F4:
        mov    bx, 40h
        push   ax
        push   bx
        push   si
        push   di
        mov    ax, 1daah
        mov    bx, 0fdech
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        and    ax, 0f00h
        pop    di
        pop    si
        pop    bx
        xor    cx, cx
        cmp    ax, 0c00h
        jne    SHORT loc_2_0828
        or     cx, 200h
loc_2_0828:
        cmp    ax, 0d00h
        jne    SHORT loc_2_0833
        or     cx, 4200h
loc_2_0833:
        cmp    ax, 0f00h
        jne    SHORT loc_2_083E
        or     cx, 1000h
loc_2_083E:
        cmp    ax, 0
        jne    SHORT loc_2_0849
        or     cx, 1080h
loc_2_0849:
        pop    ax
loc_2_084B:
        xchg   cx, ax
        pop    cx
        pop    dx
        ret
        db  000h, 000h
loc_2_0854:
        inc    esp
        imul   esi, dword ptr [ebx + 70h], 79616ch
loc_2_085C:
        push   esi
        inc    esp
        inc    esp
        push   edx
        db  065h, 073h, 065h
        db  072h, 076h
        add    byte ptr gs:[eax], al
_ITEXT ends
        end
