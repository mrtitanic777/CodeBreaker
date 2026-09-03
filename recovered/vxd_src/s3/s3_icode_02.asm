; s3_icode_02.asm -- S3 object 2 segment _ITEXT
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
        db  089h, 01Dh, 040h, 013h, 000h, 000h
        db  0C6h, 005h, 0D9h, 013h, 000h, 000h, 000h
        pushad
        mov    eax, 20h
        db  0BEh, 0D0h, 006h, 000h, 000h
        db  0BFh, 0D8h, 006h, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        cmp    eax, 20h
        je     SHORT loc_2_004E
        cmp    eax, 40h
        je     SHORT loc_2_004E
        mov    eax, 20h
loc_2_004E:
        shl    eax, 0ah
        db  0A3h, 03Ch, 013h, 000h, 000h
        popad
        push   0
        push   10000h
        push   0a0000h
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     NEAR PTR loc_2_0402
        db  0A3h, 050h, 013h, 000h, 000h
        mov    edx, 3d4h
        mov    ax, 4838h
        out    dx, ax
        mov    al, 30h
        out    dx, al
        inc    dx
        in     al, dx
        cmp    al, 0e1h
        je     SHORT loc_2_00C2
        mov    ch, al
        and    ch, 0f0h
        cmp    ch, 80h
        je     SHORT loc_2_00F4
        cmp    ch, 90h
        je     SHORT loc_2_00F4
        cmp    ch, 0b0h
        je     SHORT loc_2_00F4
        cmp    ch, 0a0h
        je     SHORT loc_2_00F4
        db  0C6h, 005h, 0D8h, 013h, 000h, 000h, 0FFh
        cmp    ch, 0c0h
        je     SHORT loc_2_00F4
        cmp    ch, 0d0h
        je     SHORT loc_2_00F4
        cmp    ch, 0e0h
        jne    NEAR PTR loc_2_0402
loc_2_00C2:
        db  0C6h, 005h, 0D8h, 013h, 000h, 000h, 0FFh
        dec    dl
        mov    al, 2eh
        out    dx, al
        inc    dl
        in     al, dx
        and    al, 0f0h
        or     al, 1
        mov    ch, al
        cmp    ch, 11h
        je     SHORT loc_2_00F4
        cmp    ch, 81h
        je     SHORT loc_2_00F4
        cmp    ch, 91h
        je     SHORT loc_2_00F4
        cmp    ch, 0b1h
        je     SHORT loc_2_00F4
        cmp    ch, 0f1h
        jne    NEAR PTR loc_2_0402
loc_2_00F4:
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
        je     NEAR PTR loc_2_0402
        dec    dl
        mov    al, 36h
        out    dx, al
        inc    dx
        in     al, dx
        dec    dx
        mov    cl, al
        cmp    ch, 80h
        je     SHORT loc_2_0160
        shr    cl, 5
        mov    eax, 400000h
        or     cl, cl
        je     SHORT loc_2_016F
        mov    eax, 300000h
        cmp    cl, 2
        je     SHORT loc_2_016F
        mov    eax, 800000h
        cmp    cl, 3
        je     SHORT loc_2_016F
        mov    eax, 200000h
        cmp    cl, 4
        je     SHORT loc_2_016F
        mov    eax, 600000h
        cmp    cl, 5
        je     SHORT loc_2_016F
        mov    eax, 100000h
        cmp    cl, 6
        je     SHORT loc_2_016F
        mov    eax, 80000h
        jmp    SHORT loc_2_016F
loc_2_0160:
        mov    eax, 100000h
        test   cl, 20h
        je     SHORT loc_2_016F
        mov    eax, 80000h
loc_2_016F:
        db  0A3h, 04Ch, 013h, 000h, 000h
        db  088h, 02Dh, 0D7h, 013h, 000h, 000h
        push   0
        push   10h
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        add    esp, 8
        db  0A3h, 054h, 013h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_2_0402
        db  0CDh, 020h, 00Eh, 000h, 00Ah, 000h
        cmp    ecx, 31h
        jb     NEAR PTR loc_2_0402
        db  0C7h, 007h, 000h, 000h, 000h, 000h
        db  0C7h, 047h, 004h, 000h, 000h, 000h, 000h
        db  0C7h, 047h, 008h, 0CBh, 000h, 000h, 000h
        db  0C7h, 047h, 00Ch, 052h, 001h, 000h, 000h
        db  0C7h, 047h, 058h, 001h, 002h, 000h, 000h
        db  0C7h, 047h, 05Ch, 07Fh, 002h, 000h, 000h
        db  0C7h, 047h, 010h, 0D7h, 002h, 000h, 000h
        db  0C7h, 047h, 014h, 03Bh, 003h, 000h, 000h
        db  0C7h, 047h, 018h, 066h, 003h, 000h, 000h
        db  0C7h, 047h, 01Ch, 06Eh, 003h, 000h, 000h
        db  0C7h, 047h, 020h, 076h, 003h, 000h, 000h
        db  0C7h, 047h, 024h, 0D3h, 003h, 000h, 000h
        db  0C7h, 047h, 02Ch, 01Fh, 005h, 000h, 000h
        db  0C7h, 047h, 030h, 060h, 005h, 000h, 000h
        db  0C7h, 047h, 034h, 0C1h, 005h, 000h, 000h
        db  0C7h, 047h, 038h, 0A0h, 006h, 000h, 000h
        db  0C7h, 047h, 03Ch, 08Ah, 007h, 000h, 000h
        db  0C7h, 047h, 040h, 00Ch, 008h, 000h, 000h
        db  0C7h, 047h, 044h, 04Dh, 008h, 000h, 000h
        db  0C7h, 087h, 0B8h, 000h, 000h, 000h, 06Eh, 003h, 000h, 000h
        db  0C7h, 087h, 0B4h, 000h, 000h, 000h, 041h, 003h, 000h, 000h
        db  0C7h, 047h, 068h, 01Eh, 003h, 000h, 000h
        db  0C7h, 047h, 070h, 05Fh, 000h, 000h, 000h
        db  0C7h, 047h, 074h, 0F2h, 002h, 000h, 000h
        db  0C7h, 087h, 090h, 000h, 000h, 000h, 0D6h, 008h, 000h, 000h
        db  0C7h, 087h, 080h, 000h, 000h, 000h, 0DEh, 008h, 000h, 000h
        db  0C7h, 087h, 084h, 000h, 000h, 000h, 0DEh, 008h, 000h, 000h
        db  0C7h, 087h, 094h, 000h, 000h, 000h, 02Bh, 009h, 000h, 000h
        db  0C7h, 087h, 088h, 000h, 000h, 000h, 061h, 00Ah, 000h, 000h
        db  0C7h, 087h, 09Ch, 000h, 000h, 000h, 034h, 009h, 000h, 000h
        db  0C7h, 087h, 0A0h, 000h, 000h, 000h, 0E8h, 009h, 000h, 000h
        db  0C7h, 087h, 0A8h, 000h, 000h, 000h, 005h, 004h, 000h, 000h
        db  0C7h, 087h, 0B0h, 000h, 000h, 000h, 07Eh, 000h, 000h, 000h
        db  0C7h, 087h, 0BCh, 000h, 000h, 000h, 0DFh, 000h, 000h, 000h
        db  080h, 03Dh, 0D7h, 013h, 000h, 000h, 080h
        jne    SHORT loc_2_02C7
        db  0C7h, 087h, 098h, 000h, 000h, 000h, 0C1h, 00Ah, 000h, 000h
loc_2_02C7:
        mov    edx, 3c2h
        db  0B9h, 0BEh, 008h, 000h, 000h
        db  0CDh, 020h, 013h, 000h, 00Ah, 000h
        db  089h, 00Dh, 064h, 013h, 000h, 000h
        db  0CDh, 020h, 011h, 000h, 00Ah, 000h
        db  089h, 035h, 044h, 013h, 000h, 000h
        db  089h, 03Dh, 048h, 013h, 000h, 000h
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
        db  0BEh, 069h, 007h, 000h, 000h
        mov    edx, 4ae8h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_2_0402
        mov    edx, 4ae8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 4ae9h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_2_0402
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        db  08Bh, 01Dh, 040h, 013h, 000h, 000h
        call   sub_2_0410
        db  0A0h, 0D7h, 013h, 000h, 000h
        clc
        jmp    SHORT loc_2_0403
loc_2_0402:
        stc
loc_2_0403:
        pop    ebx
        ret
loc_2_0405:
        db  00Fh, 0B6h, 005h, 0D7h, 013h, 000h, 000h
        ret
        db  000h, 000h, 000h
sub_2_0410:
        push   ebx
        db  089h, 01Dh, 0ECh, 013h, 000h, 000h
        db  0BEh, 048h, 005h, 000h, 000h
        mov    ebx, 8
        call   sub_2_051A
        jb     NEAR PTR loc_2_0517
        db  0BEh, 050h, 005h, 000h, 000h
        mov    ebx, 0bh
        call   sub_2_051A
        jae    SHORT loc_2_0474
        db  0BEh, 05Bh, 005h, 000h, 000h
        mov    ebx, 0ah
        call   sub_2_051A
        jae    SHORT loc_2_0474
        db  0BEh, 065h, 005h, 000h, 000h
        mov    ebx, 0bh
        call   sub_2_051A
        jae    SHORT loc_2_0474
        db  0BEh, 070h, 005h, 000h, 000h
        mov    ebx, 0eh
        call   sub_2_051A
        jb     NEAR PTR loc_2_0517
loc_2_0474:
        call   sub_2_0582
        db  066h, 0A3h, 046h, 014h, 000h, 000h
        db  066h, 089h, 01Dh, 048h, 014h, 000h, 000h
        and    ax, 7fffh
        db  0BEh, 087h, 001h, 000h, 000h
loc_2_048F:
        test   word ptr [esi], bx
        je     SHORT loc_2_04AF
        push   bx
        and    bx, 0c000h
        cmp    bx, word ptr [esi + 2]
        pop    bx
        jne    SHORT loc_2_04AF
        test   word ptr [esi + 4], ax
        je     SHORT loc_2_04AF
        cmp    cx, word ptr [esi + 6]
        je     SHORT loc_2_04BA
loc_2_04AF:
        add    esi, 0ch
        cmp    word ptr [esi], 0
        je     SHORT loc_2_0517
        jmp    SHORT loc_2_048F
loc_2_04BA:
        mov    esi, dword ptr [esi + 8]
        db  089h, 035h, 03Fh, 014h, 000h, 000h
        db  08Bh, 01Dh, 0ECh, 013h, 000h, 000h
        mov    ebp, dword ptr [ebx + 8]
        db  0CDh, 020h, 00Eh, 000h, 00Ah, 000h
        cmp    ecx, 31h
        jb     SHORT loc_2_0517
        cmp    dword ptr [edi + 0a8h], 0
        je     SHORT loc_2_0517
        call   dword ptr [edi + 0a8h]
        db  0A2h, 03Ah, 014h, 000h, 000h
        mov    eax, dword ptr [edi + 70h]
        db  0C7h, 047h, 070h, 05Ah, 005h, 000h, 000h
        mov    eax, dword ptr [edi + 74h]
        db  0C7h, 047h, 074h, 084h, 003h, 000h, 000h
        mov    eax, dword ptr [edi + 0bch]
        db  0C7h, 087h, 0BCh, 000h, 000h, 000h, 09Ch, 012h, 000h, 000h
        db  0A3h, 03Bh, 014h, 000h, 000h
        clc
        jmp    SHORT loc_2_0518
loc_2_0517:
        stc
loc_2_0518:
        pop    ebx
        ret
sub_2_051A:
        push   eax
        push   edi
        push   ecx
        mov    edi, 0c0000h
        mov    ecx, 100h
loc_2_0527:
        mov    al, byte ptr [esi]
        repne scasb
        or     ecx, ecx
        je     SHORT loc_2_0540
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
        je     SHORT loc_2_0543
        loop   loc_2_0527
loc_2_0540:
        stc
        jmp    SHORT loc_2_0544
loc_2_0543:
        clc
loc_2_0544:
        pop    ecx
        pop    edi
        pop    eax
        ret
loc_2_0548:
        inc    esp
        imul   esp, dword ptr [ecx + 6dh], 646e6fh
loc_2_0550:
        push   ebx
        db  074h, 065h
        popad
        insb
        je     SHORT loc_2_05BF
        and    byte ptr [esi], dh
        xor    al, 0
loc_2_055B:
        push   ebx
        db  074h, 065h
        popad
        insb
        je     SHORT loc_2_05CA
        xor    al, 0
loc_2_0565:
        push   ebx
        db  074h, 065h
        popad
        insb
        je     SHORT loc_2_05D4
        and    byte ptr [ebx + 45h], dl
        db  000h
loc_2_0570:
        push   ebx
        je     SHORT loc_2_05D8
        popad
        insb
        db  074h, 068h
        and    byte ptr [esi + 69h], dl
        outsd
        db  000h
loc_2_057E:
        xor    dword ptr [esi], ebp
        xor    dword ptr [eax], eax
sub_2_0582:
        push   dx
        mov    dx, 3d4h
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a039h
        out    dx, ax
        mov    al, 36h
        out    dx, al
        jmp    SHORT loc_2_0599
loc_2_0599:
        in     ax, dx
        jmp    SHORT loc_2_059D
loc_2_059D:
        and    ah, 0c0h
        mov    cx, 4
        or     ah, ah
        je     SHORT loc_2_05B5
        mov    cx, 2
        cmp    ah, 80h
        je     SHORT loc_2_05B5
        mov    cx, 1
loc_2_05B5:
        push   cx
        mov    al, 30h
        out    dx, al
        inc    dx
        in     al, dx
        dec    dx
loc_2_05BF:
        mov    ah, al
        and    ah, 0fh
        and    al, 0f0h
        cmp    al, 0d0h
        je     SHORT loc_2_063E
loc_2_05CA:
        cmp    al, 0c0h
        je     SHORT loc_2_05FA
        cmp    al, 0e0h
        jne    SHORT loc_2_05FA
        mov    al, 2eh
loc_2_05D4:
        out    dx, al
        inc    dx
        in     al, dx
loc_2_05D8:
        dec    dx
        cmp    al, 80h
        je     SHORT loc_2_065C
        cmp    al, 90h
        je     NEAR PTR loc_2_0666
        cmp    al, 0b0h
        je     NEAR PTR loc_2_0670
        cmp    al, 0f0h
        je     SHORT loc_2_0670
        cmp    al, 10h
        je     SHORT loc_2_0648
        cmp    al, 11h
        je     SHORT loc_2_0652
loc_2_05FA:
        mov    bx, 1
        push   esi
        push   ebx
        db  0BEh, 07Eh, 005h, 000h, 000h
        mov    ebx, 4
        call   sub_2_051A
        pop    ebx
        pop    esi
        jae    SHORT loc_2_0618
        or     bx, 4000h
loc_2_0618:
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
        jne    SHORT loc_2_0634
        or     cx, 800h
loc_2_0634:
        mov    dx, 3c8h
        in     al, dx
        jmp    NEAR PTR loc_2_06C7
loc_2_063E:
        mov    bx, 2
        mov    cx, 100h
        jmp    SHORT loc_2_06C7
loc_2_0648:
        mov    bx, 4
        mov    cx, 400h
        jmp    SHORT loc_2_06C7
loc_2_0652:
        mov    bx, 8
        mov    cx, 400h
        jmp    SHORT loc_2_06C7
loc_2_065C:
        mov    bx, 10h
        mov    cx, 400h
        jmp    SHORT loc_2_06C7
loc_2_0666:
        mov    bx, 20h
        mov    cx, 400h
        jmp    SHORT loc_2_06C7
loc_2_0670:
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
        jne    SHORT loc_2_06A4
        or     cx, 200h
loc_2_06A4:
        cmp    ax, 0d00h
        jne    SHORT loc_2_06AF
        or     cx, 4200h
loc_2_06AF:
        cmp    ax, 0f00h
        jne    SHORT loc_2_06BA
        or     cx, 1000h
loc_2_06BA:
        cmp    ax, 0
        jne    SHORT loc_2_06C5
        or     cx, 1080h
loc_2_06C5:
        pop    ax
loc_2_06C7:
        xchg   cx, ax
        pop    cx
        pop    dx
        ret
        db  000h, 000h
loc_2_06D0:
        inc    esp
        imul   esi, dword ptr [ebx + 70h], 79616ch
loc_2_06D8:
        push   esi
        inc    esp
        inc    esp
        push   edx
        db  065h, 073h, 065h
        db  072h, 076h
        add    byte ptr gs:[eax], al
_ITEXT ends
        end
