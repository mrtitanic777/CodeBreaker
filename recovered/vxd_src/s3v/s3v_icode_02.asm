; s3v_icode_02.asm -- S3V object 2 segment _ITEXT
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
        db  089h, 01Dh, 078h, 003h, 000h, 000h
        db  0C6h, 005h, 036h, 004h, 000h, 000h, 000h
        pushad
        mov    eax, 20h
        db  0BEh, 0ECh, 006h, 000h, 000h
        db  0BFh, 0F4h, 006h, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        cmp    eax, 20h
        je     SHORT loc_2_004E
        cmp    eax, 40h
        je     SHORT loc_2_004E
        mov    eax, 20h
loc_2_004E:
        shl    eax, 0ah
        db  0A3h, 074h, 003h, 000h, 000h
        popad
        push   0
        push   10000h
        push   0a0000h
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     NEAR PTR loc_2_06E2
        db  0A3h, 088h, 003h, 000h, 000h
        db  0C7h, 005h, 0A4h, 003h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0A8h, 003h, 000h, 000h, 0E8h, 09Ah, 000h, 000h
        xor    eax, eax
        mov    edx, 3d4h
        mov    ax, 4838h
        out    dx, ax
        mov    al, 36h
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 3
        db  0A2h, 035h, 004h, 000h, 000h
        dec    edx
        mov    al, 30h
        out    dx, al
        inc    dx
        in     al, dx
        cmp    al, 0e1h
        je     SHORT loc_2_0106
        mov    ch, al
        and    ch, 0f0h
        db  088h, 02Dh, 02Fh, 004h, 000h, 000h
        and    al, 0fh
        db  0A2h, 033h, 004h, 000h, 000h
        cmp    ch, 80h
        je     NEAR PTR loc_2_0246
        cmp    ch, 90h
        je     NEAR PTR loc_2_0246
        cmp    ch, 0b0h
        je     NEAR PTR loc_2_0246
        cmp    ch, 0a0h
        je     NEAR PTR loc_2_0246
        db  0C6h, 005h, 034h, 004h, 000h, 000h, 0FFh
        cmp    ch, 0c0h
        je     NEAR PTR loc_2_0246
        cmp    ch, 0d0h
        je     NEAR PTR loc_2_0246
        jmp    NEAR PTR loc_2_06E2
loc_2_0106:
        db  0C6h, 005h, 034h, 004h, 000h, 000h, 0FFh
        db  083h, 00Dh, 0A4h, 003h, 000h, 000h, 001h
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
        db  0A2h, 033h, 004h, 000h, 000h
        and    al, 40h
        cmp    eax, 881140h
        jne    SHORT loc_2_0174
        db  0A3h, 02Fh, 004h, 000h, 000h
        db  080h, 00Dh, 038h, 004h, 000h, 000h, 001h
        db  0C7h, 005h, 0ACh, 003h, 000h, 000h, 0ECh, 081h, 000h, 000h
        db  080h, 03Dh, 035h, 004h, 000h, 000h, 001h
        je     NEAR PTR loc_2_023F
        db  0C7h, 005h, 0A8h, 003h, 000h, 000h, 018h, 081h, 000h, 000h
        jmp    NEAR PTR loc_2_023F
loc_2_0174:
        and    al, 0
        db  0A3h, 02Fh, 004h, 000h, 000h
        cmp    eax, 881000h
        je     SHORT loc_2_0189
        cmp    eax, 881100h
        jne    SHORT loc_2_01C6
loc_2_0189:
        db  080h, 03Dh, 033h, 004h, 000h, 000h, 020h
        je     NEAR PTR loc_2_023F
        db  080h, 03Dh, 033h, 004h, 000h, 000h, 030h
        je     NEAR PTR loc_2_023F
        db  080h, 03Dh, 033h, 004h, 000h, 000h, 003h
        je     NEAR PTR loc_2_023F
        db  080h, 03Dh, 033h, 004h, 000h, 000h, 012h
        je     NEAR PTR loc_2_023F
        db  080h, 03Dh, 033h, 004h, 000h, 000h, 022h
        je     SHORT loc_2_023F
loc_2_01C6:
        db  0C7h, 005h, 0A8h, 003h, 000h, 000h, 018h, 081h, 000h, 000h
        cmp    eax, 888000h
        je     SHORT loc_2_023F
        cmp    eax, 889000h
        je     SHORT loc_2_023F
        cmp    eax, 88b000h
        je     SHORT loc_2_023F
        cmp    eax, 88f000h
        je     SHORT loc_2_023F
        db  080h, 00Dh, 038h, 004h, 000h, 000h, 001h
        db  0C7h, 005h, 0ACh, 003h, 000h, 000h, 0ECh, 081h, 000h, 000h
        cmp    eax, 881200h
        je     SHORT loc_2_023F
        cmp    eax, 881500h
        je     SHORT loc_2_023F
        cmp    eax, 881400h
        je     SHORT loc_2_023F
        db  080h, 00Dh, 038h, 004h, 000h, 000h, 005h
        db  0C7h, 005h, 0A8h, 003h, 000h, 000h, 004h, 085h, 000h, 000h
        db  0C7h, 005h, 0ACh, 003h, 000h, 000h, 000h, 082h, 000h, 000h
        cmp    eax, 563100h
        je     SHORT loc_2_023F
        cmp    eax, 883d00h
        jne    NEAR PTR loc_2_06E2
loc_2_023F:
        db  083h, 00Dh, 0A4h, 003h, 000h, 000h, 002h
loc_2_0246:
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
        je     NEAR PTR loc_2_06E2
        dec    dl
        db  0B8h, 0E0h, 003h, 000h, 000h
        mov    ecx, 4ah
        mov    word ptr [eax], cx
        push   eax
        db  0CDh, 020h, 014h, 000h, 00Ah, 000h
        pop    eax
        mov    eax, dword ptr [eax + 8]
        cmp    eax, 315f3353h
        je     SHORT loc_2_02F3
        or     eax, 20h
        cmp    eax, 315f3373h
        je     SHORT loc_2_02F3
        or     eax, 20202020h
        cmp    eax, 642e3373h
        je     SHORT loc_2_02F3
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 080h, 000h, 000h, 000h
        je     NEAR PTR loc_2_06E2
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 090h, 000h, 000h, 000h
        je     NEAR PTR loc_2_06E2
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 0B0h, 000h, 000h, 000h
        je     NEAR PTR loc_2_06E2
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 0A0h, 000h, 000h, 000h
        je     NEAR PTR loc_2_06E2
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 0C0h, 000h, 000h, 000h
        je     NEAR PTR loc_2_06E2
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 0D0h, 000h, 000h, 000h
        je     NEAR PTR loc_2_06E2
loc_2_02F3:
        db  0C6h, 005h, 081h, 004h, 000h, 000h, 000h
        db  0B8h, 062h, 004h, 000h, 000h
        db  0BAh, 07Dh, 004h, 000h, 000h
        push   edx
        push   eax
        push   80000002h
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        cmp    eax, 0
        jne    SHORT loc_2_032E
        db  0FEh, 005h, 081h, 004h, 000h, 000h
        db  0FFh, 035h, 07Dh, 004h, 000h, 000h
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
loc_2_032E:
        mov    dx, 3d4h
        mov    al, 36h
        out    dx, al
        inc    dx
        in     al, dx
        dec    dx
        mov    cl, al
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 03Dh, 088h, 000h
        jne    SHORT loc_2_036F
        shr    cl, 5
        and    cl, 3
        shl    cl, 1
        add    cl, 2
        mov    al, 37h
        out    dx, al
        inc    dx
        in     al, dx
        dec    dx
        shr    al, 5
        and    al, 3
        xor    al, 3
        shl    al, 1
        sub    cl, al
        xor    eax, eax
        mov    al, cl
        shl    eax, 14h
        jmp    SHORT loc_2_03D4
loc_2_036F:
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_2_03C5
        shr    cl, 5
        mov    eax, 400000h
        or     cl, cl
        je     SHORT loc_2_03D4
        mov    eax, 300000h
        cmp    cl, 2
        je     SHORT loc_2_03D4
        mov    eax, 800000h
        cmp    cl, 3
        je     SHORT loc_2_03D4
        mov    eax, 200000h
        cmp    cl, 4
        je     SHORT loc_2_03D4
        mov    eax, 100000h
        add    eax, 80000h
        cmp    cl, 5
        je     SHORT loc_2_03D4
        mov    eax, 100000h
        cmp    cl, 6
        je     SHORT loc_2_03D4
        mov    eax, 80000h
        jmp    SHORT loc_2_03D4
loc_2_03C5:
        mov    eax, 100000h
        test   cl, 20h
        je     SHORT loc_2_03D4
        mov    eax, 80000h
loc_2_03D4:
        db  0A3h, 084h, 003h, 000h, 000h
        push   0
        push   10h
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        add    esp, 8
        db  0A3h, 08Ch, 003h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_2_06E2
        db  0CDh, 020h, 00Eh, 000h, 00Ah, 000h
        cmp    ecx, 31h
        jb     NEAR PTR loc_2_06E2
        db  0C7h, 007h, 000h, 000h, 000h, 000h
        db  0C7h, 047h, 004h, 000h, 000h, 000h, 000h
        db  0C7h, 047h, 008h, 071h, 001h, 000h, 000h
        db  0C7h, 047h, 00Ch, 0FFh, 001h, 000h, 000h
        db  0C7h, 047h, 058h, 0B4h, 002h, 000h, 000h
        db  0C7h, 047h, 05Ch, 035h, 003h, 000h, 000h
        db  0C7h, 047h, 010h, 025h, 004h, 000h, 000h
        db  0C7h, 047h, 014h, 005h, 005h, 000h, 000h
        db  0C7h, 047h, 018h, 048h, 005h, 000h, 000h
        db  0C7h, 047h, 01Ch, 05Ah, 005h, 000h, 000h
        db  0C7h, 047h, 020h, 0A4h, 005h, 000h, 000h
        db  0C7h, 047h, 024h, 022h, 006h, 000h, 000h
        db  0C7h, 047h, 02Ch, 056h, 008h, 000h, 000h
        db  0C7h, 047h, 030h, 0A6h, 008h, 000h, 000h
        db  0C7h, 047h, 034h, 037h, 009h, 000h, 000h
        db  0C7h, 047h, 038h, 036h, 00Ah, 000h, 000h
        db  0C7h, 047h, 03Ch, 040h, 00Bh, 000h, 000h
        db  0C7h, 047h, 040h, 0AEh, 00Ch, 000h, 000h
        db  0C7h, 047h, 044h, 003h, 00Dh, 000h, 000h
        db  0C7h, 087h, 0B8h, 000h, 000h, 000h, 088h, 007h, 000h, 000h
        db  0C7h, 087h, 0B4h, 000h, 000h, 000h, 04Eh, 007h, 000h, 000h
        db  0C7h, 047h, 068h, 00Eh, 007h, 000h, 000h
        db  0C7h, 047h, 070h, 006h, 001h, 000h, 000h
        db  0C7h, 047h, 074h, 0E8h, 006h, 000h, 000h
        db  0C7h, 087h, 090h, 000h, 000h, 000h, 0C3h, 00Dh, 000h, 000h
        db  0C7h, 087h, 080h, 000h, 000h, 000h, 0CBh, 00Dh, 000h, 000h
        db  0C7h, 087h, 084h, 000h, 000h, 000h, 0CBh, 00Dh, 000h, 000h
        db  0C7h, 087h, 094h, 000h, 000h, 000h, 01Bh, 00Eh, 000h, 000h
        db  0C7h, 087h, 088h, 000h, 000h, 000h, 062h, 00Fh, 000h, 000h
        db  0C7h, 087h, 09Ch, 000h, 000h, 000h, 024h, 00Eh, 000h, 000h
        db  0C7h, 087h, 0A0h, 000h, 000h, 000h, 0E6h, 00Eh, 000h, 000h
        db  0C7h, 087h, 0A8h, 000h, 000h, 000h, 0E5h, 006h, 000h, 000h
        db  0C7h, 087h, 0B0h, 000h, 000h, 000h, 07Eh, 002h, 000h, 000h
        db  0C7h, 087h, 0A4h, 000h, 000h, 000h, 0D6h, 00Fh, 000h, 000h
        db  0C7h, 087h, 0BCh, 000h, 000h, 000h, 0DFh, 002h, 000h, 000h
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 080h, 000h, 000h, 000h
        jne    SHORT loc_2_0533
        db  0C7h, 087h, 098h, 000h, 000h, 000h, 0C5h, 00Fh, 000h, 000h
loc_2_0533:
        mov    edx, 3c2h
        db  0B9h, 0ABh, 00Dh, 000h, 000h
        db  0CDh, 020h, 013h, 000h, 00Ah, 000h
        db  089h, 00Dh, 0B0h, 003h, 000h, 000h
        db  0CDh, 020h, 011h, 000h, 00Ah, 000h
        db  089h, 035h, 07Ch, 003h, 000h, 000h
        db  089h, 03Dh, 080h, 003h, 000h, 000h
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 03Dh, 088h, 000h
        je     SHORT loc_2_0573
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 031h, 056h, 000h
        jne    SHORT loc_2_05DA
loc_2_0573:
        db  083h, 03Dh, 0B8h, 003h, 000h, 000h, 000h
        jne    SHORT loc_2_05D5
        push   8fh
        db  068h, 0B4h, 003h, 000h, 000h
        push   100h
        push   0
        push   0fh
        push   0
        push   1
        push   10h
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        test   eax, eax
        je     SHORT loc_2_05D5
        db  0A3h, 0B8h, 003h, 000h, 000h
        push   1
        push   0
        push   0f2h
        push   10000h
        push   eax
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   eax
        push   edx
        db  0CDh, 020h, 076h, 000h, 001h, 000h
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_2_05D5
        db  0A3h, 0BCh, 003h, 000h, 000h
loc_2_05D5:
        jmp    NEAR PTR loc_2_06DA
loc_2_05DA:
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
        db  0BEh, 01Fh, 00Bh, 000h, 000h
        mov    edx, 4ae8h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_2_06E2
        mov    edx, 4ae8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 4ae9h
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_2_06E2
        db  0CDh, 020h, 099h, 000h, 001h, 000h
loc_2_06DA:
        db  0A1h, 02Fh, 004h, 000h, 000h
        clc
        jmp    SHORT loc_2_06E3
loc_2_06E2:
        stc
loc_2_06E3:
        pop    ebx
        ret
loc_2_06E5:
        db  0A1h, 02Fh, 004h, 000h, 000h
        ret
        db  000h
loc_2_06EC:
        inc    esp
        imul   esi, dword ptr [ebx + 70h], 79616ch
loc_2_06F4:
        push   esi
        inc    esp
        inc    esp
        push   edx
        db  065h, 073h, 065h
        db  072h, 076h
        db  065h, 000h
_ITEXT ends
        end
