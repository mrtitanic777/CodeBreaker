; enable2_lcode_01.asm -- ENABLE2 object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  0EBh, 006h, 0FFh, 025h, 0FCh, 016h, 000h, 000h
loc_1_0008:
        pushad
        db  0A3h, 06Dh, 017h, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 037h, 000h
        db  0A3h, 004h, 017h, 000h, 000h
        db  0FFh, 015h, 069h, 017h, 000h, 000h
        db  083h, 03Dh, 06Dh, 017h, 000h, 000h, 001h
        jne    SHORT loc_1_0058
        db  08Dh, 03Dh, 0BCh, 017h, 000h, 000h
        mov    ebx, 1
        db  0CDh, 020h, 007h, 000h, 037h, 000h
        pushad
        db  0CDh, 020h, 008h, 000h, 00Dh, 000h
        db  0CDh, 020h, 011h, 000h, 00Dh, 000h
        and    al, 0fh
        db  0A2h, 037h, 017h, 000h, 000h
        db  0A2h, 024h, 01Ah, 000h, 000h
        popad
        jmp    NEAR PTR loc_1_0136
loc_1_0058:
        db  081h, 03Dh, 06Dh, 017h, 000h, 000h, 000h, 001h, 000h, 000h
        jne    SHORT loc_1_007A
        db  08Dh, 03Dh, 072h, 017h, 000h, 000h
        mov    ebx, 100h
        db  0CDh, 020h, 007h, 000h, 037h, 000h
        jmp    NEAR PTR loc_1_0136
loc_1_007A:
        db  083h, 03Dh, 06Dh, 017h, 000h, 000h, 040h
        jne    SHORT loc_1_00A0
        mov    ebx, 40h
        db  08Dh, 03Dh, 092h, 017h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 037h, 000h
        pushad
        call   sub_1_0ACD
        popad
        jmp    NEAR PTR loc_1_0136
loc_1_00A0:
        db  081h, 03Dh, 06Dh, 017h, 000h, 000h, 000h, 002h, 000h, 000h
        jne    SHORT loc_1_00BF
        mov    ebx, 200h
        db  08Dh, 03Dh, 08Ah, 017h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 037h, 000h
        jmp    SHORT loc_1_0136
loc_1_00BF:
        db  081h, 03Dh, 06Dh, 017h, 000h, 000h, 000h, 004h, 000h, 000h
        jne    SHORT loc_1_00DE
        mov    ebx, 400h
        db  08Dh, 03Dh, 0AEh, 017h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 037h, 000h
        jmp    SHORT loc_1_0136
loc_1_00DE:
        db  081h, 03Dh, 06Dh, 017h, 000h, 000h, 080h, 000h, 000h, 000h
        jne    SHORT loc_1_00FD
        mov    ebx, 80h
        db  08Dh, 03Dh, 0D0h, 017h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 037h, 000h
        jmp    SHORT loc_1_0136
loc_1_00FD:
        db  083h, 03Dh, 06Dh, 017h, 000h, 000h, 008h
        jne    SHORT loc_1_0119
        mov    ebx, 8
        db  08Dh, 03Dh, 0E8h, 017h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 037h, 000h
        jmp    SHORT loc_1_0136
loc_1_0119:
        db  081h, 03Dh, 06Dh, 017h, 000h, 000h, 000h, 008h, 000h, 000h
        jne    SHORT loc_1_0136
        mov    ebx, 800h
        db  08Dh, 03Dh, 018h, 018h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 037h, 000h
loc_1_0136:
        db  0CDh, 020h, 008h, 000h, 00Dh, 000h
        db  0CDh, 020h, 011h, 000h, 00Dh, 000h
        and    al, 0fh
        db  0A2h, 038h, 017h, 000h, 000h
        popad
        clc
        db  0FFh, 015h, 0FCh, 016h, 000h, 000h
        ret
loc_1_0152:
        db  08Bh, 035h, 0F1h, 019h, 000h, 000h
        db  0CDh, 020h, 012h, 000h, 001h, 000h
        mov    esi, 0ffffffffh
        db  087h, 035h, 040h, 01Ah, 000h, 000h
        cmp    esi, -1
        je     SHORT loc_1_0179
        cmp    esi, 0
        je     SHORT loc_1_0179
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
loc_1_0179:
        db  08Dh, 03Dh, 0BCh, 016h, 000h, 000h
        xor    eax, eax
        mov    dword ptr [edi], eax
        mov    dword ptr [edi + 18h], eax
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [edi + 0ch], eax
        mov    dword ptr [edi + 10h], eax
        mov    dword ptr [edi + 14h], eax
        mov    dword ptr [edi + 1ch], eax
        db  0CDh, 020h, 006h, 000h, 037h, 000h
        call   sub_1_0B13
        mov    eax, 370001h
        db  0BEh, 008h, 000h, 000h, 000h
        db  0CDh, 020h, 01Ch, 001h, 001h, 000h
        clc
        ret
        db  0B8h, 000h, 004h, 000h, 000h, 0F8h, 0C3h
ENABLE2_Control:
        cmp    eax, 1bh
        db  00Fh, 084h, 03Eh, 0FEh, 0FEh, 0FFh
        cmp    eax, 1ch
        je     SHORT loc_1_0152
        cmp    eax, 1
        db  00Fh, 084h, 030h, 0FEh, 0FEh, 0FFh
        cmp    eax, 5
        je     NEAR PTR loc_1_0152
        clc
        ret
sub_1_01DB:
        db  066h, 0C7h, 005h, 024h, 017h, 000h, 000h, 000h, 000h
        ret
loc_1_01E5:
        db  066h, 089h, 00Dh, 020h, 017h, 000h, 000h
        cmp    al, 0e0h
        je     SHORT loc_1_01FD
        cmp    al, 0e1h
        je     SHORT loc_1_01FD
        test   al, 80h
        je     SHORT loc_1_01FD
        call   sub_1_01DB
loc_1_01FD:
        push   eax
        db  083h, 03Dh, 040h, 01Ah, 000h, 000h, 0FFh
        jne    SHORT loc_1_0224
        pushad
        mov    eax, 37h
        mov    edx, 0
        db  0BEh, 098h, 00Bh, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 040h, 01Ah, 000h, 000h
        popad
loc_1_0224:
        cmp    al, 0e0h
        je     SHORT loc_1_0236
        cmp    al, 0e1h
        jne    SHORT loc_1_0236
        db  0A2h, 048h, 017h, 000h, 000h
        jmp    NEAR PTR loc_1_0397
loc_1_0236:
        db  08Ah, 025h, 048h, 017h, 000h, 000h
        db  0C6h, 005h, 048h, 017h, 000h, 000h, 000h
        cmp    al, 54h
        jne    SHORT loc_1_024C
        jmp    NEAR PTR loc_1_03A4
loc_1_024C:
        cmp    al, 0d5h
        jne    SHORT loc_1_0255
        jmp    NEAR PTR loc_1_03A4
loc_1_0255:
        db  066h, 0F7h, 005h, 067h, 017h, 000h, 000h, 020h, 000h
        je     SHORT loc_1_0265
        jmp    NEAR PTR loc_1_0372
loc_1_0265:
        db  0F7h, 005h, 0C0h, 017h, 000h, 000h, 001h, 000h, 000h, 000h
        jne    SHORT loc_1_0276
        jmp    NEAR PTR loc_1_0372
loc_1_0276:
        db  080h, 03Dh, 03Ah, 017h, 000h, 000h, 000h
        je     SHORT loc_1_0284
        jmp    NEAR PTR loc_1_0340
loc_1_0284:
        cmp    ah, 0e0h
        jne    NEAR PTR loc_1_0316
        cmp    al, 2ah
        jne    SHORT loc_1_02B6
        db  0F6h, 005h, 062h, 017h, 000h, 000h, 028h
        jne    SHORT loc_1_029F
        jmp    NEAR PTR loc_1_038A
loc_1_029F:
        xchg   ah, al
        db  0FFh, 015h, 024h, 018h, 000h, 000h
        jb     SHORT loc_1_02AF
        db  0CDh, 020h, 013h, 000h, 00Dh, 000h
loc_1_02AF:
        xchg   ah, al
        jmp    NEAR PTR loc_1_03A4
loc_1_02B6:
        cmp    al, 37h
        jne    SHORT loc_1_02DE
        db  0F6h, 005h, 062h, 017h, 000h, 000h, 028h
        jne    SHORT loc_1_02F0
        jmp    SHORT loc_1_02DE
        db  0C6h, 005h, 039h, 017h, 000h, 000h, 0FFh, 0B4h, 000h, 0B0h, 054h, 0F6h, 005h, 062h, 017h, 000h
        db  000h, 014h, 074h, 03Bh, 0E9h, 0C6h, 000h, 000h, 000h
loc_1_02DE:
        cmp    al, 0b7h
        jne    SHORT loc_1_02F0
        db  080h, 03Dh, 039h, 017h, 000h, 000h, 000h
        je     SHORT loc_1_030D
        jmp    NEAR PTR loc_1_0397
loc_1_02F0:
        cmp    al, 0aah
        jne    SHORT loc_1_030D
        db  080h, 03Dh, 039h, 017h, 000h, 000h, 000h
        je     SHORT loc_1_030D
        db  0C6h, 005h, 039h, 017h, 000h, 000h, 000h
        mov    ah, 0
        mov    al, 0d4h
        jmp    NEAR PTR loc_1_03A4
loc_1_030D:
        db  0C6h, 005h, 039h, 017h, 000h, 000h, 000h
        jmp    SHORT loc_1_038A
loc_1_0316:
        cmp    ah, 0e1h
        jne    SHORT loc_1_038A
        jmp    SHORT loc_1_0372
        db  0F6h, 005h, 062h, 017h, 000h, 000h, 014h, 074h, 002h, 0EBh, 00Bh, 0F6h, 005h, 063h, 017h, 000h
        db  000h, 014h, 075h, 002h, 0EBh, 03Fh, 0C6h, 005h, 03Ah, 017h, 000h, 000h, 0FFh, 0B4h, 0E0h, 0B0h
        db  046h, 0EBh, 04Ah
loc_1_0340:
        cmp    al, 0c5h
        jne    SHORT loc_1_0358
        mov    ah, 0e0h
        mov    al, 0c6h
        db  0C6h, 005h, 03Ah, 017h, 000h, 000h, 000h
        db  0C6h, 005h, 046h, 017h, 000h, 000h, 000h
        jmp    SHORT loc_1_038A
loc_1_0358:
        db  0FEh, 005h, 046h, 017h, 000h, 000h
        db  080h, 03Dh, 046h, 017h, 000h, 000h, 003h
        jle    SHORT loc_1_0370
        db  0C6h, 005h, 03Ah, 017h, 000h, 000h, 000h
        jmp    SHORT loc_1_0372
loc_1_0370:
        jmp    SHORT loc_1_03A4
loc_1_0372:
        db  0F7h, 005h, 096h, 017h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_1_038A
        cmp    ah, 0e1h
        jne    SHORT loc_1_038A
        db  0C6h, 005h, 047h, 017h, 000h, 000h, 0FFh
loc_1_038A:
        clc
        call   sub_1_03F0
        jb     SHORT loc_1_0397
        call   sub_1_15E8
loc_1_0397:
        jb     SHORT loc_1_03A2
        test   al, 80h
        je     SHORT loc_1_03A2
        call   sub_1_01DB
loc_1_03A2:
        pop    eax
        ret
loc_1_03A4:
        pop    eax
        jb     SHORT loc_1_03B6
        test   al, 80h
        je     SHORT loc_1_03B0
        call   sub_1_01DB
loc_1_03B0:
        db  0FFh, 015h, 024h, 018h, 000h, 000h
loc_1_03B6:
        ret
        db  000h
loc_1_03B8:
        push   0
        push   edx
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_03CF
        clc
        ret
loc_1_03CF:
        stc
        ret
loc_1_03D1:
        xor    edx, edx
        mov    dx, cx
        push   0
        push   edx
        db  068h, 05Eh, 000h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_03ED
        clc
        ret
loc_1_03ED:
        stc
        ret
        db  000h
sub_1_03F0:
        push   ebx
        push   ecx
        push   edx
        push   edi
        push   esi
        jae    SHORT loc_1_03FE
        db  0C6h, 005h, 049h, 017h, 000h, 000h, 0FFh
loc_1_03FE:
        cmp    al, 0e0h
        jne    SHORT loc_1_040E
        db  0C6h, 005h, 069h, 018h, 000h, 000h, 0E0h
        jmp    NEAR PTR loc_1_066A
loc_1_040E:
        db  08Ah, 025h, 069h, 018h, 000h, 000h
        db  0C6h, 005h, 069h, 018h, 000h, 000h, 000h
        mov    bx, ax
        db  0C6h, 005h, 038h, 017h, 000h, 000h, 000h
        and    al, 7fh
        mov    ecx, 6
        db  0BFh, 02Bh, 017h, 000h, 000h
        cld
        repne scasw
        jne    SHORT loc_1_047A
        mov    al, 1
        shl    al, cl
        db  0A2h, 071h, 017h, 000h, 000h
        db  0A2h, 038h, 017h, 000h, 000h
        db  08Ah, 025h, 037h, 017h, 000h, 000h
        test   bl, 80h
        je     SHORT loc_1_0456
        not    al
        and    ah, al
        jmp    SHORT loc_1_0458
loc_1_0456:
        or     ah, al
loc_1_0458:
        db  088h, 025h, 037h, 017h, 000h, 000h
        db  080h, 03Dh, 049h, 017h, 000h, 000h, 000h
        jne    SHORT loc_1_047A
        cmp    bl, 36h
        jne    SHORT loc_1_047A
        test   ah, 3ch
        jne    SHORT loc_1_047A
        db  0C6h, 005h, 029h, 017h, 000h, 000h, 0FFh
        jmp    SHORT loc_1_0481
loc_1_047A:
        db  0C6h, 005h, 029h, 017h, 000h, 000h, 000h
loc_1_0481:
        mov    ax, bx
        and    bl, 7fh
        db  0F7h, 005h, 076h, 017h, 000h, 000h, 001h, 000h, 000h, 000h
        jne    SHORT loc_1_0498
        jmp    NEAR PTR loc_1_066A
loc_1_0498:
        test   al, 80h
        je     SHORT loc_1_04E1
        db  066h, 039h, 01Dh, 022h, 017h, 000h, 000h
        je     SHORT loc_1_04AA
        jmp    NEAR PTR loc_1_066A
loc_1_04AA:
        db  081h, 03Dh, 086h, 017h, 000h, 000h, 0F8h, 07Fh, 000h, 000h
        je     SHORT loc_1_04CC
        db  08Bh, 01Dh, 086h, 017h, 000h, 000h
        db  066h, 089h, 01Dh, 026h, 017h, 000h, 000h
        db  0C6h, 005h, 02Ah, 017h, 000h, 000h, 0FFh
        jmp    SHORT loc_1_04D5
loc_1_04CC:
        db  066h, 0C7h, 005h, 022h, 017h, 000h, 000h, 000h, 000h
loc_1_04D5:
        db  0C6h, 005h, 028h, 017h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_066A
loc_1_04E1:
        cmp    ax, 0e02ah
        je     SHORT loc_1_0508
        cmp    ax, 0e036h
        je     SHORT loc_1_0508
        cmp    ax, 0e11dh
        je     SHORT loc_1_0508
        cmp    ax, 0e046h
        je     SHORT loc_1_0508
        db  066h, 081h, 03Dh, 024h, 017h, 000h, 000h, 01Dh, 0E1h
        jne    SHORT loc_1_050D
        cmp    al, 45h
        jne    SHORT loc_1_050D
loc_1_0508:
        jmp    NEAR PTR loc_1_066A
loc_1_050D:
        db  066h, 03Bh, 005h, 024h, 017h, 000h, 000h
        jne    SHORT loc_1_051B
        jmp    NEAR PTR loc_1_0687
loc_1_051B:
        db  081h, 03Dh, 086h, 017h, 000h, 000h, 0F8h, 07Fh, 000h, 000h
        je     SHORT loc_1_059B
        db  066h, 08Bh, 01Dh, 022h, 017h, 000h, 000h
        cmp    ax, bx
        je     SHORT loc_1_055C
        db  066h, 0A3h, 022h, 017h, 000h, 000h
        db  0F7h, 005h, 076h, 017h, 000h, 000h, 040h, 000h, 000h, 000h
        je     SHORT loc_1_054A
        call   sub_1_1478
loc_1_054A:
        db  08Bh, 01Dh, 086h, 017h, 000h, 000h
        db  066h, 089h, 01Dh, 026h, 017h, 000h, 000h
        jmp    NEAR PTR loc_1_066A
loc_1_055C:
        db  066h, 083h, 03Dh, 026h, 017h, 000h, 000h, 000h
        jg     SHORT loc_1_0590
        db  08Bh, 01Dh, 086h, 017h, 000h, 000h
        db  066h, 089h, 01Dh, 026h, 017h, 000h, 000h
        clc
        db  066h, 0A3h, 022h, 017h, 000h, 000h
        db  0F7h, 005h, 076h, 017h, 000h, 000h, 040h, 000h, 000h, 000h
        je     SHORT loc_1_058B
        call   sub_1_1478
loc_1_058B:
        jmp    NEAR PTR loc_1_066A
loc_1_0590:
        db  066h, 0A3h, 022h, 017h, 000h, 000h
        jmp    NEAR PTR loc_1_0687
loc_1_059B:
        db  08Bh, 01Dh, 07Ah, 017h, 000h, 000h
        cmp    bx, 7ff8h
        jne    SHORT loc_1_05AC
        mov    bx, 0
loc_1_05AC:
        or     bx, bx
        je     SHORT loc_1_05DB
        db  0F7h, 005h, 076h, 017h, 000h, 000h, 040h, 000h, 000h, 000h
        je     SHORT loc_1_05C2
        call   sub_1_1478
loc_1_05C2:
        db  066h, 089h, 01Dh, 026h, 017h, 000h, 000h
        db  0C6h, 005h, 028h, 017h, 000h, 000h, 000h
        db  066h, 0A3h, 024h, 017h, 000h, 000h
        jmp    NEAR PTR loc_1_0687
loc_1_05DB:
        db  081h, 03Dh, 086h, 017h, 000h, 000h, 0F8h, 07Fh, 000h, 000h
        je     SHORT loc_1_0639
        db  080h, 03Dh, 02Ah, 017h, 000h, 000h, 000h
        je     SHORT loc_1_060B
        db  08Bh, 01Dh, 086h, 017h, 000h, 000h
        db  066h, 089h, 01Dh, 026h, 017h, 000h, 000h
        db  066h, 0A3h, 024h, 017h, 000h, 000h
        db  066h, 0A3h, 022h, 017h, 000h, 000h
        jmp    SHORT loc_1_0687
loc_1_060B:
        db  08Bh, 01Dh, 086h, 017h, 000h, 000h
        db  066h, 089h, 01Dh, 026h, 017h, 000h, 000h
        db  0C6h, 005h, 028h, 017h, 000h, 000h, 000h
        db  0C6h, 005h, 02Ah, 017h, 000h, 000h, 0FFh
        db  0F7h, 005h, 076h, 017h, 000h, 000h, 040h, 000h, 000h, 000h
        je     SHORT loc_1_0637
        call   sub_1_1478
loc_1_0637:
        jmp    SHORT loc_1_065E
loc_1_0639:
        db  0F7h, 005h, 076h, 017h, 000h, 000h, 040h, 000h, 000h, 000h
        je     SHORT loc_1_064A
        call   sub_1_1478
loc_1_064A:
        db  08Bh, 01Dh, 07Eh, 017h, 000h, 000h
        db  066h, 089h, 01Dh, 026h, 017h, 000h, 000h
        db  0C6h, 005h, 028h, 017h, 000h, 000h, 0FFh
loc_1_065E:
        db  066h, 0A3h, 024h, 017h, 000h, 000h
        db  066h, 0A3h, 022h, 017h, 000h, 000h
loc_1_066A:
        db  080h, 03Dh, 025h, 017h, 000h, 000h, 0E0h
        jne    SHORT loc_1_067C
        push   eax
        mov    al, 0e0h
        call   sub_1_0C34
        pop    eax
loc_1_067C:
        call   sub_1_0C34
loc_1_0681:
        pop    esi
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        ret
loc_1_0687:
        stc
        jmp    SHORT loc_1_0681
        db  000h, 000h
sub_1_068C:
        test   cl, 80h
        je     SHORT loc_1_06AC
        db  00Fh, 0B6h, 01Dh, 0DBh, 019h, 000h, 000h
        cmp    ebx, 0ch
        jne    SHORT loc_1_069F
        xor    ebx, ebx
loc_1_069F:
        db  088h, 083h, 0DDh, 019h, 000h, 000h
        inc    ebx
        db  088h, 01Dh, 0DBh, 019h, 000h, 000h
loc_1_06AC:
        clc
        ret
sub_1_06AE:
        db  00Fh, 0B6h, 01Dh, 0DCh, 019h, 000h, 000h
        db  03Ah, 01Dh, 0DBh, 019h, 000h, 000h
        je     SHORT loc_1_06D3
        cmp    ebx, 0ch
        jne    SHORT loc_1_06C4
        xor    ebx, ebx
loc_1_06C4:
        db  08Ah, 083h, 0DDh, 019h, 000h, 000h
        inc    ebx
        db  088h, 01Dh, 0DCh, 019h, 000h, 000h
        clc
        ret
loc_1_06D3:
        stc
        ret
sub_1_06D5:
        pushad
        mov    dl, al
        db  0BEh, 0D3h, 002h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        db  089h, 035h, 0F1h, 019h, 000h, 000h
        popad
        ret
sub_1_06EB:
        call   sub_1_06AE
        jb     SHORT loc_1_0708
        call   sub_1_08F0
        xor    esi, esi
        xor    edi, edi
        call   sub_1_06D5
        db  0CDh, 020h, 003h, 000h, 00Ch, 000h
        stc
        ret
loc_1_0708:
        clc
        ret
sub_1_070A:
        db  08Ah, 035h, 062h, 017h, 000h, 000h
        db  00Ah, 035h, 037h, 017h, 000h, 000h
        db  00Ah, 035h, 063h, 017h, 000h, 000h
        db  080h, 03Dh, 06Ch, 018h, 000h, 000h, 000h
        jne    SHORT loc_1_0731
        push   eax
        db  0A1h, 040h, 019h, 000h, 000h
        db  0A3h, 03Ch, 019h, 000h, 000h
        pop    eax
loc_1_0731:
        db  0F7h, 005h, 096h, 017h, 000h, 000h, 001h, 000h, 000h, 000h
        jne    SHORT loc_1_0742
        jmp    NEAR PTR loc_1_07E3
loc_1_0742:
        db  08Ah, 025h, 082h, 018h, 000h, 000h
        db  0C6h, 005h, 082h, 018h, 000h, 000h, 000h
        cmp    al, 0e0h
        jne    SHORT loc_1_0758
        db  0A2h, 082h, 018h, 000h, 000h
loc_1_0758:
        mov    si, ax
        and    al, 7fh
        mov    ecx, 11h
        db  0BFh, 083h, 018h, 000h, 000h
        cld
        repne scasw
        je     SHORT loc_1_0777
loc_1_076D:
        call   sub_1_0A9C
        mov    ax, si
loc_1_0775:
        jmp    SHORT loc_1_07E3
loc_1_0777:
        xor    ebx, ebx
        mov    bx, cx
        mov    cx, si
        db  080h, 03Dh, 047h, 017h, 000h, 000h, 000h
        je     SHORT loc_1_0798
        cmp    cx, 0c5h
        jne    SHORT loc_1_076D
        db  0C6h, 005h, 047h, 017h, 000h, 000h, 000h
        jmp    SHORT loc_1_076D
loc_1_0798:
        cmp    cx, 53h
        jne    SHORT loc_1_07A8
        cmp    dh, 3ch
        jne    SHORT loc_1_07A8
        mov    ax, cx
        jmp    SHORT loc_1_0775
loc_1_07A8:
        db  080h, 03Dh, 06Fh, 018h, 000h, 000h, 000h
        jne    SHORT loc_1_07C3
        cmp    cx, 45h
        je     SHORT loc_1_07C3
        cmp    cx, 0c5h
        je     SHORT loc_1_07C3
        mov    ax, cx
        jmp    SHORT loc_1_0775
loc_1_07C3:
        db  066h, 08Bh, 004h, 05Dh, 0E9h, 018h, 000h, 000h
        db  0FFh, 014h, 09Dh, 0A5h, 018h, 000h, 000h
        and    cl, 7fh
        cmp    cx, 0e01ch
        je     SHORT loc_1_07E3
        cmp    cl, 45h
        je     SHORT loc_1_07E3
        stc
        ret
loc_1_07E3:
        clc
        ret
loc_1_07E5:
        mov    ax, cx
        ret
loc_1_07E9:
        pushad
        test   cl, 80h
        je     SHORT loc_1_0855
        db  066h, 0A1h, 020h, 017h, 000h, 000h
        and    ax, 20h
        shl    ax, 2
        db  08Bh, 01Dh, 096h, 017h, 000h, 000h
        and    bx, 80h
        xor    ax, bx
        test   ax, 80h
        je     SHORT loc_1_082A
        db  0C6h, 005h, 06Fh, 018h, 000h, 000h, 0FFh
        push   ax
        mov    al, 0feh
        call   sub_1_06D5
        pop    ax
        call   sub_1_1457
        jmp    SHORT loc_1_0855
loc_1_082A:
        call   sub_1_0A9C
        db  0C6h, 005h, 06Fh, 018h, 000h, 000h, 000h
        push   ax
        mov    al, 0ffh
        call   sub_1_06D5
        pop    ax
        call   sub_1_144F
        push   eax
        push   ecx
        xor    ecx, ecx
        mov    cl, 53h
        xor    eax, eax
        call   sub_1_087B
        pop    ecx
        pop    eax
loc_1_0855:
        mov    ax, cx
        popad
        ret
loc_1_085A:
        cmp    ax, 3
        jne    SHORT loc_1_0862
        push   eax
        pop    eax
loc_1_0862:
        db  0A2h, 074h, 018h, 000h, 000h
        push   eax
        db  00Fh, 0B7h, 005h, 080h, 018h, 000h, 000h
        call   sub_1_08F0
        call   sub_1_06D5
        pop    eax
        ret
sub_1_087B:
        push   eax
        push   ecx
        test   cl, 80h
        jne    SHORT loc_1_08ED
        call   sub_1_0A9C
        db  08Ah, 01Dh, 075h, 018h, 000h, 000h
        db  08Ah, 00Dh, 074h, 018h, 000h, 000h
        mov    dl, bl
        or     ax, ax
        je     SHORT loc_1_089E
        or     dl, cl
        jmp    SHORT loc_1_08A2
loc_1_089E:
        not    cl
        and    dl, cl
loc_1_08A2:
        db  088h, 015h, 075h, 018h, 000h, 000h
        shl    bl, 1
        shl    bl, 1
        or     bl, dl
        and    ebx, 0fh
        db  08Ah, 09Bh, 00Bh, 019h, 000h, 000h
        or     bx, bx
        je     SHORT loc_1_08ED
        db  066h, 089h, 01Dh, 080h, 018h, 000h, 000h
        db  0BBh, 07Ah, 018h, 000h, 000h
        db  00Fh, 0BFh, 035h, 07Eh, 018h, 000h, 000h
        db  00Fh, 0BFh, 03Dh, 07Ch, 018h, 000h, 000h
        db  00Fh, 0B7h, 005h, 080h, 018h, 000h, 000h
        call   sub_1_08F0
        call   sub_1_06D5
        db  0CDh, 020h, 003h, 000h, 00Ch, 000h
loc_1_08ED:
        pop    ecx
        pop    eax
        ret
sub_1_08F0:
        and    eax, 0ffffh
        db  080h, 03Dh, 074h, 018h, 000h, 000h, 002h
        je     SHORT loc_1_0910
        test   al, 2
        je     SHORT loc_1_0907
        or     eax, 200000h
loc_1_0907:
        db  080h, 03Dh, 074h, 018h, 000h, 000h, 001h
        je     SHORT loc_1_0919
loc_1_0910:
        test   al, 8
        je     SHORT loc_1_0919
        or     eax, 100000h
loc_1_0919:
        shr    eax, 10h
        ret
sub_1_091D:
        xor    ax, ax
        not    ax
        call   sub_1_068C
        xor    ax, ax
        jmp    NEAR PTR sub_1_068C
loc_1_0930:
        call   sub_1_091D
        jmp    SHORT sub_1_091D
loc_1_0937:
        test   cl, 80h
        je     SHORT loc_1_0953
        db  066h, 039h, 005h, 072h, 018h, 000h, 000h
        jne    NEAR PTR loc_1_0A3C
        call   sub_1_0A9C
        jmp    NEAR PTR loc_1_0A3C
loc_1_0953:
        db  066h, 039h, 005h, 072h, 018h, 000h, 000h
        jne    SHORT loc_1_0968
        db  0C6h, 005h, 06Ch, 018h, 000h, 000h, 0FFh
        jmp    NEAR PTR loc_1_0A3C
loc_1_0968:
        call   sub_1_0A9C
        db  066h, 0A3h, 072h, 018h, 000h, 000h
        db  081h, 03Dh, 086h, 017h, 000h, 000h, 0F8h, 07Fh, 000h, 000h
        je     SHORT loc_1_0996
        db  081h, 03Dh, 082h, 017h, 000h, 000h, 0F8h, 07Fh, 000h, 000h
        je     SHORT loc_1_0996
        db  080h, 03Dh, 049h, 017h, 000h, 000h, 000h
        jne    SHORT loc_1_0996
        jne    SHORT loc_1_099D
loc_1_0996:
        db  0C6h, 005h, 06Ch, 018h, 000h, 000h, 0FFh
loc_1_099D:
        mov    cl, ah
        cbw
        db  066h, 0A3h, 07Ch, 018h, 000h, 000h
        mov    al, cl
        cbw
        db  066h, 0A3h, 07Eh, 018h, 000h, 000h
        db  08Ah, 025h, 062h, 017h, 000h, 000h
        db  00Ah, 025h, 037h, 017h, 000h, 000h
        db  00Ah, 025h, 063h, 017h, 000h, 000h
        test   ah, 14h
        je     SHORT loc_1_0A0B
        db  0A1h, 0A2h, 017h, 000h, 000h
        mov    bx, ax
        db  066h, 083h, 03Dh, 07Eh, 018h, 000h, 000h, 000h
        je     SHORT loc_1_09ED
        db  066h, 083h, 03Dh, 07Eh, 018h, 000h, 000h, 0FFh
        jne    SHORT loc_1_09E7
        neg    ax
loc_1_09E7:
        db  066h, 0A3h, 07Eh, 018h, 000h, 000h
loc_1_09ED:
        db  066h, 083h, 03Dh, 07Ch, 018h, 000h, 000h, 000h
        je     SHORT loc_1_0A0B
        db  066h, 083h, 03Dh, 07Ch, 018h, 000h, 000h, 0FFh
        jne    SHORT loc_1_0A04
        neg    bx
loc_1_0A04:
        db  066h, 089h, 01Dh, 07Ch, 018h, 000h, 000h
loc_1_0A0B:
        db  066h, 083h, 00Dh, 080h, 018h, 000h, 000h, 001h
        db  0B8h, 07Ah, 018h, 000h, 000h
        db  00Fh, 0BFh, 035h, 07Eh, 018h, 000h, 000h
        db  00Fh, 0BFh, 03Dh, 07Ch, 018h, 000h, 000h
        db  066h, 0A1h, 080h, 018h, 000h, 000h
        call   sub_1_08F0
        call   sub_1_06D5
        db  0CDh, 020h, 003h, 000h, 00Ch, 000h
loc_1_0A3C:
        ret
        db  0F7h, 005h, 096h, 017h, 000h, 000h, 010h, 000h, 000h, 000h, 074h, 000h, 0E8h, 04Eh, 000h, 000h
        db  000h, 0C6h, 005h, 06Eh, 018h, 000h, 000h, 0FFh, 083h, 00Dh, 096h, 017h, 000h, 000h, 001h, 0C6h
        db  005h, 06Fh, 018h, 000h, 000h, 0FFh, 0C3h, 0F7h, 005h, 076h, 017h, 000h, 000h, 010h, 000h, 000h
        db  000h, 074h, 000h, 083h, 025h, 096h, 017h, 000h, 000h, 0FEh, 0C6h, 005h, 06Fh, 018h, 000h, 000h
        db  000h, 0E8h, 019h, 000h, 000h, 000h, 080h, 03Dh, 075h, 018h, 000h, 000h, 000h, 074h, 00Fh, 050h
        db  051h, 033h, 0C9h, 0B1h, 053h, 033h, 0C0h, 0E8h, 0E2h, 0FDh, 0FFh, 0FFh, 059h, 058h, 0C3h
sub_1_0A9C:
        db  066h, 0C7h, 005h, 072h, 018h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 06Ch, 018h, 000h, 000h, 000h
        db  0C6h, 005h, 06Dh, 018h, 000h, 000h, 000h
        db  0C6h, 005h, 070h, 018h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 07Eh, 018h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 07Ch, 018h, 000h, 000h, 000h, 000h
        ret
sub_1_0ACD:
        db  0A0h, 075h, 018h, 000h, 000h
        call   sub_1_08F0
        call   sub_1_06D5
        call   sub_1_0A9C
        db  0BEh, 067h, 003h, 000h, 000h
        mov    edx, ebx
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        ret
        db  000h
sub_1_0AF0:
        db  083h, 03Dh, 018h, 01Ah, 000h, 000h, 000h
        jne    SHORT loc_1_0B11
        mov    eax, 0b0002h
        db  0BEh, 044h, 00Bh, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        jb     SHORT loc_1_0B12
        db  089h, 035h, 018h, 01Ah, 000h, 000h
loc_1_0B11:
        clc
loc_1_0B12:
        ret
sub_1_0B13:
        db  083h, 03Dh, 018h, 01Ah, 000h, 000h, 000h
        je     SHORT loc_1_0B38
        mov    eax, 0b0002h
        db  0BEh, 044h, 00Bh, 000h, 000h
        db  0CDh, 020h, 01Ch, 001h, 001h, 000h
        jb     SHORT loc_1_0B39
        db  0C7h, 005h, 018h, 01Ah, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0B38:
        clc
loc_1_0B39:
        ret
        db  08Bh, 0C0h, 0EBh, 006h, 0FFh, 025h, 018h, 01Ah, 000h, 000h
loc_1_0B44:
        pushad
        pushfd
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        xor    edx, edx
        db  0BEh, 061h, 00Bh, 000h, 000h
        db  0CDh, 020h, 00Fh, 000h, 001h, 000h
        popfd
        popad
        db  0FFh, 025h, 018h, 01Ah, 000h, 000h
loc_1_0B61:
        pushad
        pushfd
        db  0F7h, 005h, 0ECh, 017h, 000h, 000h, 001h, 000h, 000h, 000h
        je     NEAR PTR loc_1_0C0D
        db  0F7h, 005h, 004h, 017h, 000h, 000h, 07Bh, 007h, 000h, 000h
        je     NEAR PTR loc_1_0C0D
        db  083h, 03Dh, 008h, 018h, 000h, 000h, 000h
        jne    SHORT loc_1_0B9E
        db  083h, 03Dh, 0F0h, 017h, 000h, 000h, 000h
        jne    SHORT loc_1_0B9E
        db  083h, 03Dh, 0FCh, 017h, 000h, 000h, 000h
        je     SHORT loc_1_0C0D
loc_1_0B9E:
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        db  087h, 005h, 014h, 01Ah, 000h, 000h
        db  02Bh, 005h, 014h, 01Ah, 000h, 000h
        neg    eax
        cmp    eax, 3e8h
        jl     SHORT loc_1_0C0D
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, ebx
        db  0CDh, 020h, 002h, 000h, 00Ch, 000h
        cmp    eax, ebx
        jne    SHORT loc_1_0BE2
        db  0A1h, 008h, 018h, 000h, 000h
        cmp    eax, 0
        je     SHORT loc_1_0C0D
        db  08Bh, 00Dh, 00Ch, 018h, 000h, 000h
        call   sub_1_0C10
        jmp    SHORT loc_1_0C0D
loc_1_0BE2:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  089h, 01Dh, 0F8h, 019h, 000h, 000h
        db  083h, 03Dh, 0F0h, 017h, 000h, 000h, 000h
        jne    SHORT loc_1_0C00
        db  083h, 03Dh, 0FCh, 017h, 000h, 000h, 000h
        je     SHORT loc_1_0C0D
loc_1_0C00:
        xor    edx, edx
        db  0BEh, 0D4h, 004h, 000h, 000h
        db  0CDh, 020h, 00Fh, 000h, 001h, 000h
loc_1_0C0D:
        popfd
        popad
        ret
sub_1_0C10:
        cmp    ax, 4
        je     SHORT loc_1_0C2D
        db  066h, 0A3h, 028h, 018h, 000h, 000h
        db  066h, 089h, 00Dh, 02Ah, 018h, 000h, 000h
        mov    edx, 0
        jmp    NEAR PTR loc_1_03B8
loc_1_0C2D:
        jmp    NEAR PTR loc_1_03D1
        db  000h, 000h
sub_1_0C34:
        db  08Ah, 015h, 02Eh, 01Ah, 000h, 000h
        db  088h, 015h, 034h, 01Ah, 000h, 000h
        db  066h, 0A3h, 02Eh, 01Ah, 000h, 000h
        cmp    al, 0e0h
        jne    SHORT loc_1_0C57
        db  0C6h, 005h, 065h, 017h, 000h, 000h, 001h
        stc
        jmp    NEAR PTR loc_1_0E21
loc_1_0C57:
        db  080h, 03Dh, 034h, 01Ah, 000h, 000h, 0E0h
        jne    SHORT loc_1_0C82
        mov    dl, al
        and    dl, 7fh
        cmp    dl, 2ah
        jne    SHORT loc_1_0C82
        test   al, 80h
        je     SHORT loc_1_0C7C
        db  080h, 03Dh, 030h, 01Ah, 000h, 000h, 000h
        je     SHORT loc_1_0C7C
        call   sub_1_129E
loc_1_0C7C:
        clc
        jmp    NEAR PTR loc_1_0E21
loc_1_0C82:
        db  08Ah, 035h, 037h, 017h, 000h, 000h
        db  08Ah, 015h, 024h, 01Ah, 000h, 000h
        db  088h, 035h, 024h, 01Ah, 000h, 000h
        db  08Ah, 025h, 038h, 017h, 000h, 000h
        db  080h, 03Dh, 030h, 01Ah, 000h, 000h, 000h
        je     SHORT loc_1_0CA8
        call   sub_1_129E
loc_1_0CA8:
        or     ah, ah
        jne    SHORT loc_1_0CB1
        jmp    NEAR PTR loc_1_0DB3
loc_1_0CB1:
        test   al, 80h
        jne    SHORT loc_1_0CC5
        db  0C6h, 005h, 025h, 01Ah, 000h, 000h, 000h
        test   ah, dl
        je     SHORT loc_1_0CC5
        jmp    NEAR PTR loc_1_0E06
loc_1_0CC5:
        db  08Ah, 00Dh, 026h, 01Ah, 000h, 000h
        test   ah, 3
        je     SHORT loc_1_0CD4
        inc    cl
        jmp    SHORT loc_1_0CD7
loc_1_0CD4:
        xor    cx, cx
loc_1_0CD7:
        db  088h, 00Dh, 026h, 01Ah, 000h, 000h
        db  0F7h, 005h, 0C0h, 017h, 000h, 000h, 001h, 000h, 000h, 000h
        jne    SHORT loc_1_0CEE
        jmp    NEAR PTR loc_1_0E06
loc_1_0CEE:
        db  066h, 08Bh, 00Dh, 062h, 017h, 000h, 000h
        test   al, 80h
        je     SHORT loc_1_0D0E
        or     ch, cl
        test   ah, ch
        jne    SHORT loc_1_0D09
        call   sub_1_1244
        jmp    NEAR PTR loc_1_0E06
loc_1_0D09:
        jmp    NEAR PTR loc_1_0DFF
loc_1_0D0E:
        cmp    dh, ah
        je     SHORT loc_1_0D2F
        call   sub_1_142E
        db  0F7h, 005h, 0C0h, 017h, 000h, 000h, 001h, 000h, 000h, 000h
        jne    SHORT loc_1_0D28
        jmp    NEAR PTR loc_1_0E06
loc_1_0D28:
        xor    cx, cx
        mov    cl, dh
        jmp    SHORT loc_1_0D5C
loc_1_0D2F:
        mov    bx, 303h
        test   bl, ah
        jne    SHORT loc_1_0D43
        mov    bx, 1414h
        test   bl, ah
        jne    SHORT loc_1_0D43
        mov    bx, 2828h
loc_1_0D43:
        test   bl, ch
        je     SHORT loc_1_0D5C
        not    bx
        and    cx, bx
        push   edx
        mov    dh, ch
        or     dh, cl
        or     dh, ah
        call   sub_1_13EC
        pop    edx
        jmp    SHORT loc_1_0D79
loc_1_0D5C:
        mov    bx, cx
        or     bl, bh
        test   ah, ch
        jne    SHORT loc_1_0D67
        xor    cl, ah
loc_1_0D67:
        db  0F7h, 005h, 0C0h, 017h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_1_0D79
        test   ah, bl
        je     SHORT loc_1_0D79
        xor    ch, ah
loc_1_0D79:
        db  066h, 089h, 00Dh, 062h, 017h, 000h, 000h
        db  0F7h, 005h, 0C0h, 017h, 000h, 000h, 040h, 000h, 000h, 000h
        je     SHORT loc_1_0DA6
        test   ah, ch
        jne    SHORT loc_1_0D97
        db  0C6h, 005h, 03Bh, 01Ah, 000h, 000h, 001h
loc_1_0D97:
        test   ah, ch
        jne    SHORT loc_1_0D9F
        test   ah, cl
        je     SHORT loc_1_0DA6
loc_1_0D9F:
        db  0C6h, 005h, 03Ch, 01Ah, 000h, 000h, 001h
loc_1_0DA6:
        test   ah, cl
        jne    SHORT loc_1_0DAC
        jmp    SHORT loc_1_0DFF
loc_1_0DAC:
        call   sub_1_0E75
        jmp    SHORT loc_1_0E06
loc_1_0DB3:
        db  0C6h, 005h, 026h, 01Ah, 000h, 000h, 000h
        db  0F7h, 005h, 0C0h, 017h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_1_0E06
        or     dh, dh
        je     SHORT loc_1_0DD5
        test   al, 80h
        jne    SHORT loc_1_0DD5
        call   sub_1_142E
        jmp    SHORT loc_1_0E06
loc_1_0DD5:
        cmp    al, 0e0h
        je     SHORT loc_1_0DE0
        db  0C6h, 005h, 030h, 01Ah, 000h, 000h, 001h
loc_1_0DE0:
        db  08Ah, 035h, 063h, 017h, 000h, 000h
        call   sub_1_13EC
        db  0C6h, 005h, 062h, 017h, 000h, 000h, 000h
        call   sub_1_0E75
        db  066h, 0A1h, 02Eh, 01Ah, 000h, 000h
        jmp    SHORT loc_1_0E21
loc_1_0DFF:
        call   sub_1_0F10
        jmp    SHORT loc_1_0E21
loc_1_0E06:
        db  080h, 03Dh, 065h, 017h, 000h, 000h, 000h
        je     SHORT loc_1_0E21
        call   sub_1_1497
        db  0C6h, 005h, 065h, 017h, 000h, 000h, 000h
        db  066h, 0A1h, 02Eh, 01Ah, 000h, 000h
loc_1_0E21:
        jb     SHORT loc_1_0E26
        cmp    al, 0ffh
        cmc
loc_1_0E26:
        pushfd
        jb     SHORT loc_1_0E3E
        db  080h, 03Dh, 065h, 017h, 000h, 000h, 000h
        je     SHORT loc_1_0E3E
        call   sub_1_1497
        db  0C6h, 005h, 065h, 017h, 000h, 000h, 000h
loc_1_0E3E:
        db  080h, 03Dh, 03Bh, 01Ah, 000h, 000h, 000h
        je     SHORT loc_1_0E53
        db  0C6h, 005h, 03Bh, 01Ah, 000h, 000h, 000h
        call   sub_1_144F
loc_1_0E53:
        db  080h, 03Dh, 03Ch, 01Ah, 000h, 000h, 000h
        je     SHORT loc_1_0E68
        db  0C6h, 005h, 03Ch, 01Ah, 000h, 000h, 000h
        call   sub_1_1457
loc_1_0E68:
        popfd
        jb     SHORT loc_1_0E74
        cmp    al, 0
        je     SHORT loc_1_0E74
        call   sub_1_070A
loc_1_0E74:
        ret
sub_1_0E75:
        push   eax
        push   ebx
        push   ecx
        xor    eax, eax
        db  066h, 08Bh, 01Dh, 062h, 017h, 000h, 000h
        and    bx, 303h
        je     SHORT loc_1_0EA7
        mov    cx, 8
        or     bl, bh
        cmp    bl, 2
        je     SHORT loc_1_0E97
        shl    cx, 3
loc_1_0E97:
        cmp    bh, 0
        je     SHORT loc_1_0EA0
        shl    cx, 6
loc_1_0EA0:
        or     ax, cx
        or     ax, 1
loc_1_0EA7:
        db  066h, 08Bh, 01Dh, 062h, 017h, 000h, 000h
        and    bx, 1414h
        je     SHORT loc_1_0ED7
        mov    cx, 10h
        db  080h, 03Dh, 032h, 01Ah, 000h, 000h, 000h
        je     SHORT loc_1_0EC6
        shl    cx, 3
loc_1_0EC6:
        cmp    bx, 4
        je     SHORT loc_1_0ED0
        shl    cx, 6
loc_1_0ED0:
        or     ax, cx
        or     ax, 2
loc_1_0ED7:
        db  066h, 08Bh, 01Dh, 062h, 017h, 000h, 000h
        and    bx, 2828h
        je     SHORT loc_1_0F07
        mov    cx, 20h
        db  080h, 03Dh, 033h, 01Ah, 000h, 000h, 000h
        je     SHORT loc_1_0EF6
        shl    cx, 3
loc_1_0EF6:
        cmp    bx, 8
        je     SHORT loc_1_0F00
        shl    cx, 6
loc_1_0F00:
        or     ax, cx
        or     ax, 4
loc_1_0F07:
        db  0A3h, 01Ch, 017h, 000h, 000h
        pop    ecx
        pop    ebx
        pop    eax
        ret
sub_1_0F10:
        push   ebx
        db  08Ah, 01Dh, 027h, 01Ah, 000h, 000h
        db  032h, 01Dh, 062h, 017h, 000h, 000h
        test   bl, 3
        jne    NEAR PTR loc_1_0FA6
        push   eax
        and    al, 7fh
        cmp    al, 2ah
        je     SHORT loc_1_0F37
        cmp    al, 36h
        je     SHORT loc_1_0F37
        pop    eax
        jmp    NEAR PTR loc_1_0FF9
loc_1_0F37:
        pop    eax
        db  0F7h, 005h, 01Ch, 017h, 000h, 000h, 049h, 012h, 000h, 000h
        je     NEAR PTR loc_1_0FF9
        push   eax
        and    al, 7fh
        cmp    al, 36h
        pop    eax
        je     SHORT loc_1_0F7D
        db  0F7h, 005h, 01Ch, 017h, 000h, 000h, 008h, 002h, 000h, 000h
        jne    NEAR PTR loc_1_0FF9
        db  0C6h, 005h, 03Ah, 01Ah, 000h, 000h, 001h
        db  080h, 00Dh, 028h, 01Ah, 000h, 000h, 080h
        call   sub_1_132C
        call   sub_1_1244
        jmp    NEAR PTR loc_1_11FB
loc_1_0F7D:
        db  0F7h, 005h, 01Ch, 017h, 000h, 000h, 040h, 010h, 000h, 000h
        jne    SHORT loc_1_0FF9
        db  0C6h, 005h, 03Ah, 01Ah, 000h, 000h, 001h
        db  080h, 00Dh, 028h, 01Ah, 000h, 000h, 080h
        call   sub_1_132C
        call   sub_1_1244
        jmp    NEAR PTR loc_1_11FB
loc_1_0FA6:
        db  080h, 03Dh, 029h, 01Ah, 000h, 000h, 000h
        je     SHORT loc_1_0FE9
        db  08Ah, 03Dh, 028h, 01Ah, 000h, 000h
        xor    bh, al
        and    bh, 7fh
        je     SHORT loc_1_0FE9
        xor    bh, bh
        db  088h, 03Dh, 03Ch, 01Ah, 000h, 000h
        inc    bh
        db  088h, 03Dh, 03Bh, 01Ah, 000h, 000h
        db  088h, 03Dh, 03Ah, 01Ah, 000h, 000h
        db  080h, 00Dh, 02Ah, 01Ah, 000h, 000h, 080h
        call   sub_1_132C
        call   sub_1_1244
        db  0FEh, 00Dh, 029h, 01Ah, 000h, 000h
loc_1_0FE9:
        db  0FEh, 005h, 029h, 01Ah, 000h, 000h
        db  0A2h, 028h, 01Ah, 000h, 000h
        jmp    NEAR PTR loc_1_11FB
loc_1_0FF9:
        test   bl, 14h
        jne    NEAR PTR loc_1_1083
        push   eax
        and    al, 7fh
        cmp    al, 1dh
        je     SHORT loc_1_100F
        pop    eax
        jmp    NEAR PTR loc_1_1100
loc_1_100F:
        pop    eax
        db  0F7h, 005h, 01Ch, 017h, 000h, 000h, 092h, 024h, 000h, 000h
        je     NEAR PTR loc_1_1100
        db  080h, 03Dh, 034h, 01Ah, 000h, 000h, 0E0h
        je     SHORT loc_1_1056
        db  0F7h, 005h, 01Ch, 017h, 000h, 000h, 010h, 004h, 000h, 000h
        jne    NEAR PTR loc_1_1100
        db  0C6h, 005h, 03Ah, 01Ah, 000h, 000h, 001h
        db  080h, 00Dh, 02Ah, 01Ah, 000h, 000h, 080h
        call   sub_1_135E
        call   sub_1_1244
        jmp    NEAR PTR loc_1_11FB
loc_1_1056:
        db  0F7h, 005h, 01Ch, 017h, 000h, 000h, 080h, 020h, 000h, 000h
        jne    NEAR PTR loc_1_1100
        db  0C6h, 005h, 03Ah, 01Ah, 000h, 000h, 001h
        db  080h, 00Dh, 02Ah, 01Ah, 000h, 000h, 080h
        call   sub_1_135E
        call   sub_1_1244
        jmp    NEAR PTR loc_1_11FB
loc_1_1083:
        db  080h, 03Dh, 02Bh, 01Ah, 000h, 000h, 000h
        je     SHORT loc_1_10D2
        db  0F7h, 005h, 01Ch, 017h, 000h, 000h, 080h, 020h, 000h, 000h
        je     SHORT loc_1_10A3
        db  080h, 03Dh, 034h, 01Ah, 000h, 000h, 0E0h
        je     SHORT loc_1_10D2
        jmp    SHORT loc_1_10AC
loc_1_10A3:
        db  080h, 03Dh, 034h, 01Ah, 000h, 000h, 0E0h
        jne    SHORT loc_1_10D2
loc_1_10AC:
        call   sub_1_135E
        call   sub_1_1244
        xor    bh, bh
        db  088h, 03Dh, 03Ch, 01Ah, 000h, 000h
        inc    bh
        db  088h, 03Dh, 03Bh, 01Ah, 000h, 000h
        db  088h, 03Dh, 03Ah, 01Ah, 000h, 000h
        db  0FEh, 00Dh, 02Bh, 01Ah, 000h, 000h
loc_1_10D2:
        db  0C6h, 005h, 032h, 01Ah, 000h, 000h, 000h
        db  080h, 03Dh, 034h, 01Ah, 000h, 000h, 0E0h
        jne    SHORT loc_1_10F0
        db  080h, 00Dh, 064h, 017h, 000h, 000h, 004h
        db  0C6h, 005h, 032h, 01Ah, 000h, 000h, 001h
loc_1_10F0:
        db  0FEh, 005h, 02Bh, 01Ah, 000h, 000h
        db  0A2h, 02Ah, 01Ah, 000h, 000h
        jmp    NEAR PTR loc_1_11FB
loc_1_1100:
        test   bl, 28h
        jne    SHORT loc_1_1183
        push   eax
        and    al, 7fh
        cmp    al, 38h
        je     SHORT loc_1_1112
        pop    eax
        jmp    NEAR PTR loc_1_11FB
loc_1_1112:
        pop    eax
        db  0F7h, 005h, 01Ch, 017h, 000h, 000h, 024h, 049h, 000h, 000h
        je     NEAR PTR loc_1_11FB
        db  080h, 03Dh, 034h, 01Ah, 000h, 000h, 0E0h
        je     SHORT loc_1_1159
        db  0F7h, 005h, 01Ch, 017h, 000h, 000h, 020h, 008h, 000h, 000h
        jne    NEAR PTR loc_1_11FB
        db  0C6h, 005h, 03Ah, 01Ah, 000h, 000h, 001h
        db  080h, 00Dh, 02Ch, 01Ah, 000h, 000h, 080h
        call   sub_1_13A3
        call   sub_1_1244
        jmp    NEAR PTR loc_1_11FB
loc_1_1159:
        db  0F7h, 005h, 01Ch, 017h, 000h, 000h, 000h, 041h, 000h, 000h
        jne    NEAR PTR loc_1_11FB
        db  0C6h, 005h, 03Ah, 01Ah, 000h, 000h, 001h
        db  080h, 00Dh, 02Ch, 01Ah, 000h, 000h, 080h
        call   sub_1_13A3
        call   sub_1_1244
        jmp    SHORT loc_1_11FB
loc_1_1183:
        db  080h, 03Dh, 02Dh, 01Ah, 000h, 000h, 000h
        je     SHORT loc_1_11D2
        db  0F7h, 005h, 01Ch, 017h, 000h, 000h, 000h, 041h, 000h, 000h
        je     SHORT loc_1_11A3
        db  080h, 03Dh, 034h, 01Ah, 000h, 000h, 0E0h
        je     SHORT loc_1_11D2
        jmp    SHORT loc_1_11AC
loc_1_11A3:
        db  080h, 03Dh, 034h, 01Ah, 000h, 000h, 0E0h
        jne    SHORT loc_1_11D2
loc_1_11AC:
        call   sub_1_13A3
        call   sub_1_1244
        xor    bh, bh
        db  088h, 03Dh, 03Ch, 01Ah, 000h, 000h
        inc    bh
        db  088h, 03Dh, 03Bh, 01Ah, 000h, 000h
        db  088h, 03Dh, 03Ah, 01Ah, 000h, 000h
        db  0FEh, 00Dh, 02Dh, 01Ah, 000h, 000h
loc_1_11D2:
        db  0C6h, 005h, 033h, 01Ah, 000h, 000h, 000h
        db  080h, 03Dh, 034h, 01Ah, 000h, 000h, 0E0h
        jne    SHORT loc_1_11F0
        db  080h, 00Dh, 064h, 017h, 000h, 000h, 008h
        db  0C6h, 005h, 033h, 01Ah, 000h, 000h, 001h
loc_1_11F0:
        db  0FEh, 005h, 02Dh, 01Ah, 000h, 000h
        db  0A2h, 02Ch, 01Ah, 000h, 000h
loc_1_11FB:
        db  08Ah, 01Dh, 062h, 017h, 000h, 000h
        db  088h, 01Dh, 027h, 01Ah, 000h, 000h
        pop    ebx
        db  080h, 03Dh, 03Ah, 01Ah, 000h, 000h, 000h
        jne    SHORT loc_1_1218
        db  0C6h, 005h, 065h, 017h, 000h, 000h, 000h
loc_1_1218:
        call   sub_1_0E75
        pushad
        db  08Bh, 015h, 01Ch, 017h, 000h, 000h
        db  0BEh, 03Bh, 00Bh, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        popad
        db  080h, 03Dh, 03Ah, 01Ah, 000h, 000h, 000h
        jne    SHORT loc_1_123B
        stc
        ret
loc_1_123B:
        db  0C6h, 005h, 03Ah, 01Ah, 000h, 000h, 000h
        clc
        ret
sub_1_1244:
        push   eax
        xor    ah, ah
        and    al, 7fh
        cmp    al, 2ah
        je     SHORT loc_1_1251
        cmp    al, 36h
        jne    SHORT loc_1_1268
loc_1_1251:
        db  088h, 025h, 029h, 01Ah, 000h, 000h
        db  088h, 025h, 028h, 01Ah, 000h, 000h
        db  066h, 081h, 025h, 062h, 017h, 000h, 000h, 0FCh, 0FCh
        jmp    SHORT loc_1_129C
loc_1_1268:
        cmp    al, 1dh
        jne    SHORT loc_1_1283
        db  088h, 025h, 02Bh, 01Ah, 000h, 000h
        db  088h, 025h, 02Ah, 01Ah, 000h, 000h
        db  066h, 081h, 025h, 062h, 017h, 000h, 000h, 0FBh, 0FBh
        jmp    SHORT loc_1_129C
loc_1_1283:
        cmp    al, 38h
        jne    SHORT loc_1_129C
        db  088h, 025h, 02Dh, 01Ah, 000h, 000h
        db  088h, 025h, 02Ch, 01Ah, 000h, 000h
        db  066h, 081h, 025h, 062h, 017h, 000h, 000h, 0F7h, 0F7h
loc_1_129C:
        pop    eax
        ret
sub_1_129E:
        pushad
        db  080h, 03Dh, 028h, 01Ah, 000h, 000h, 000h
        je     SHORT loc_1_12BF
        db  0F6h, 005h, 028h, 01Ah, 000h, 000h, 080h
        je     SHORT loc_1_12BF
        db  080h, 03Dh, 029h, 01Ah, 000h, 000h, 003h
        jge    SHORT loc_1_12BF
        call   sub_1_132C
loc_1_12BF:
        db  080h, 03Dh, 02Ah, 01Ah, 000h, 000h, 000h
        je     SHORT loc_1_12DF
        db  0F6h, 005h, 02Ah, 01Ah, 000h, 000h, 080h
        je     SHORT loc_1_12DF
        db  080h, 03Dh, 02Bh, 01Ah, 000h, 000h, 003h
        jge    SHORT loc_1_12DF
        call   sub_1_135E
loc_1_12DF:
        db  080h, 03Dh, 02Ch, 01Ah, 000h, 000h, 000h
        je     SHORT loc_1_12FF
        db  0F6h, 005h, 02Ch, 01Ah, 000h, 000h, 080h
        je     SHORT loc_1_12FF
        db  080h, 03Dh, 02Dh, 01Ah, 000h, 000h, 003h
        jge    SHORT loc_1_12FF
        call   sub_1_13A3
loc_1_12FF:
        db  08Ah, 01Dh, 062h, 017h, 000h, 000h
        db  088h, 01Dh, 027h, 01Ah, 000h, 000h
        db  0C6h, 005h, 030h, 01Ah, 000h, 000h, 000h
        popad
        jmp    NEAR PTR loc_1_1218
        db  08Ah, 01Dh, 062h, 017h, 000h, 000h, 088h, 01Dh, 027h, 01Ah, 000h, 000h, 061h, 0E8h, 0EEh, 0FEh
        db  0FFh, 0FFh, 0F9h, 0C3h
sub_1_132C:
        push   ebx
        db  0CDh, 020h, 008h, 000h, 00Dh, 000h
        db  0A0h, 028h, 01Ah, 000h, 000h
        db  0CDh, 020h, 013h, 000h, 00Dh, 000h
        jae    SHORT loc_1_134E
loc_1_1340:
        db  08Ah, 01Dh, 062h, 017h, 000h, 000h
        db  088h, 01Dh, 027h, 01Ah, 000h, 000h
        pop    ebx
        ret
loc_1_134E:
        db  0C6h, 005h, 028h, 01Ah, 000h, 000h, 000h
        db  0C6h, 005h, 029h, 01Ah, 000h, 000h, 000h
        pop    ebx
        ret
sub_1_135E:
        push   ebx
        db  080h, 03Dh, 032h, 01Ah, 000h, 000h, 001h
        jne    SHORT loc_1_1380
        mov    al, 0e0h
        db  0CDh, 020h, 013h, 000h, 00Dh, 000h
        jb     SHORT loc_1_1340
        db  0C6h, 005h, 032h, 01Ah, 000h, 000h, 000h
        db  080h, 025h, 064h, 017h, 000h, 000h, 0EBh
loc_1_1380:
        db  0CDh, 020h, 008h, 000h, 00Dh, 000h
        db  0A0h, 02Ah, 01Ah, 000h, 000h
        db  0CDh, 020h, 013h, 000h, 00Dh, 000h
        jb     SHORT loc_1_1340
        db  0C6h, 005h, 02Ah, 01Ah, 000h, 000h, 000h
        db  0C6h, 005h, 02Bh, 01Ah, 000h, 000h, 000h
        pop    ebx
        ret
sub_1_13A3:
        push   ebx
        db  080h, 03Dh, 033h, 01Ah, 000h, 000h, 001h
        jne    SHORT loc_1_13C5
        mov    al, 0e0h
        db  0CDh, 020h, 013h, 000h, 00Dh, 000h
        jb     SHORT loc_1_1340
        db  0C6h, 005h, 033h, 01Ah, 000h, 000h, 000h
        db  080h, 025h, 064h, 017h, 000h, 000h, 0D7h
loc_1_13C5:
        db  0CDh, 020h, 008h, 000h, 00Dh, 000h
        db  0A0h, 02Ch, 01Ah, 000h, 000h
        db  0CDh, 020h, 013h, 000h, 00Dh, 000h
        jb     NEAR PTR loc_1_1340
        db  0C6h, 005h, 02Ch, 01Ah, 000h, 000h, 000h
        db  0C6h, 005h, 02Dh, 01Ah, 000h, 000h, 000h
        pop    ebx
        ret
sub_1_13EC:
        push   edx
        push   ecx
        push   eax
        db  08Ah, 015h, 062h, 017h, 000h, 000h
        db  00Ah, 015h, 063h, 017h, 000h, 000h
        mov    edi, edx
        db  0BEh, 04Ah, 017h, 000h, 000h
        mov    ecx, 6
loc_1_1407:
        mov    edx, edi
        lodsw
        mov    bx, ax
        and    dx, bx
        lodsw
        xor    dh, dl
        je     SHORT loc_1_1428
        test   bl, dl
        je     SHORT loc_1_141D
        or     al, 80h
loc_1_141D:
        push   edi
        push   esi
        push   ecx
        call   sub_1_070A
        pop    ecx
        pop    esi
        pop    edi
loc_1_1428:
        loop   loc_1_1407
        pop    eax
        pop    ecx
        pop    edx
        ret
sub_1_142E:
        db  0F7h, 005h, 0C0h, 017h, 000h, 000h, 000h, 001h, 000h, 000h
        je     SHORT loc_1_144E
        db  083h, 025h, 0C0h, 017h, 000h, 000h, 0FEh
        db  0BEh, 00Ch, 00Bh, 000h, 000h
        mov    edx, ebx
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
loc_1_144E:
        ret
sub_1_144F:
        pushad
        mov    eax, 4
        jmp    SHORT loc_1_145F
sub_1_1457:
        pushad
        mov    eax, 3
        jmp    SHORT loc_1_145F
loc_1_145F:
        db  0F7h, 005h, 0C0h, 017h, 000h, 000h, 040h, 000h, 000h, 000h
        je     SHORT loc_1_1476
        mov    ecx, 1
        db  0CDh, 020h, 005h, 000h, 037h, 000h
loc_1_1476:
        popad
        ret
sub_1_1478:
        pushad
        db  0F7h, 005h, 0C0h, 017h, 000h, 000h, 040h, 000h, 000h, 000h
        je     SHORT loc_1_1495
        mov    eax, 5
        mov    ecx, 1
        db  0CDh, 020h, 005h, 000h, 037h, 000h
loc_1_1495:
        popad
        ret
sub_1_1497:
        pushad
        db  0CDh, 020h, 008h, 000h, 00Dh, 000h
        mov    al, 0e0h
        call   sub_1_070A
        jb     SHORT loc_1_14AF
        mov    al, 0e0h
        db  0CDh, 020h, 013h, 000h, 00Dh, 000h
loc_1_14AF:
        popad
        ret
loc_1_14B1:
        push   eax
        db  0CDh, 020h, 008h, 000h, 00Dh, 000h
        db  080h, 03Dh, 028h, 01Ah, 000h, 000h, 000h
        je     SHORT loc_1_14EE
        db  080h, 00Dh, 028h, 01Ah, 000h, 000h, 080h
        push   ebx
        db  0CDh, 020h, 008h, 000h, 00Dh, 000h
        db  0BEh, 028h, 01Ah, 000h, 000h
        mov    ecx, 1
        db  0CDh, 020h, 007h, 000h, 00Dh, 000h
        db  0C6h, 005h, 028h, 01Ah, 000h, 000h, 000h
        db  0C6h, 005h, 029h, 01Ah, 000h, 000h, 000h
        pop    ebx
loc_1_14EE:
        db  080h, 03Dh, 02Ah, 01Ah, 000h, 000h, 000h
        je     SHORT loc_1_1539
        db  080h, 00Dh, 02Ah, 01Ah, 000h, 000h, 080h
        push   ebx
        db  0F6h, 005h, 064h, 017h, 000h, 000h, 004h
        je     SHORT loc_1_1514
        call   sub_1_1497
        db  080h, 025h, 064h, 017h, 000h, 000h, 0EBh
loc_1_1514:
        db  0CDh, 020h, 008h, 000h, 00Dh, 000h
        db  0BEh, 02Ah, 01Ah, 000h, 000h
        mov    ecx, 1
        db  0CDh, 020h, 007h, 000h, 00Dh, 000h
        db  0C6h, 005h, 02Ah, 01Ah, 000h, 000h, 000h
        db  0C6h, 005h, 02Bh, 01Ah, 000h, 000h, 000h
        pop    ebx
loc_1_1539:
        db  080h, 03Dh, 02Ch, 01Ah, 000h, 000h, 000h
        je     SHORT loc_1_1582
        db  080h, 00Dh, 02Ch, 01Ah, 000h, 000h, 080h
        db  0F6h, 005h, 064h, 017h, 000h, 000h, 008h
        je     SHORT loc_1_155E
        call   sub_1_1497
        db  080h, 025h, 064h, 017h, 000h, 000h, 0D7h
loc_1_155E:
        db  0CDh, 020h, 008h, 000h, 00Dh, 000h
        db  0BEh, 02Ch, 01Ah, 000h, 000h
        mov    ecx, 1
        db  0CDh, 020h, 007h, 000h, 00Dh, 000h
        db  0C6h, 005h, 02Ch, 01Ah, 000h, 000h, 000h
        db  0C6h, 005h, 02Dh, 01Ah, 000h, 000h, 000h
loc_1_1582:
        pop    eax
        cmp    eax, 1
        jne    SHORT loc_1_15AC
        pushad
        db  0C6h, 005h, 026h, 01Ah, 000h, 000h, 000h
        db  0C6h, 005h, 025h, 01Ah, 000h, 000h, 000h
        db  08Ah, 035h, 037h, 017h, 000h, 000h
        call   sub_1_13EC
        db  066h, 0C7h, 005h, 062h, 017h, 000h, 000h, 000h, 000h
        popad
loc_1_15AC:
        jmp    NEAR PTR loc_1_1218
        db  000h, 000h, 000h, 0C7h, 045h, 01Ch, 000h, 004h, 000h, 000h, 066h, 083h, 065h, 02Ch, 0FEh, 0C3h
        db  0B0h, 001h, 0C3h, 08Ah, 0E8h, 0B1h, 001h, 0BAh, 0FFh, 0FFh, 0FFh, 0FFh, 0CDh, 020h, 007h, 000h
        db  00Dh, 000h, 072h, 003h, 0B0h, 001h, 0C3h, 0B0h, 000h, 0C3h, 03Ch, 000h, 075h, 002h, 0EBh, 000h
        db  0F8h, 0C3h, 0F9h, 0C3h, 0C3h, 0C3h, 000h
sub_1_15E8:
        push   eax
        db  0F7h, 005h, 08Eh, 017h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_1_1642
        db  080h, 03Dh, 044h, 01Ah, 000h, 000h, 0FFh
        je     SHORT loc_1_1642
        db  0CDh, 020h, 008h, 000h, 00Dh, 000h
        db  0CDh, 020h, 011h, 000h, 00Dh, 000h
        mov    bl, al
        mov    cl, al
        db  08Ah, 03Dh, 044h, 01Ah, 000h, 000h
        db  0F7h, 005h, 096h, 017h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_1_1628
        and    bl, 50h
        and    bh, 50h
        jmp    SHORT loc_1_162E
loc_1_1628:
        and    bl, 70h
        and    bh, 70h
loc_1_162E:
        xor    bh, bl
        je     SHORT loc_1_1642
        and    bh, bl
        je     SHORT loc_1_163D
        call   sub_1_1653
        jmp    SHORT loc_1_1642
loc_1_163D:
        call   sub_1_164B
loc_1_1642:
        db  088h, 00Dh, 044h, 01Ah, 000h, 000h
        pop    eax
        clc
        ret
sub_1_164B:
        pushad
        mov    eax, 4
        jmp    SHORT loc_1_165B
sub_1_1653:
        pushad
        mov    eax, 3
        jmp    SHORT loc_1_165B
loc_1_165B:
        mov    ecx, 1
        db  0CDh, 020h, 005h, 000h, 037h, 000h
        popad
        ret
        db  0CBh, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 000h, 080h, 000h, 000h, 045h, 04Eh, 041h, 042h
        db  04Ch, 045h, 032h, 020h, 000h, 000h, 000h, 034h, 0B9h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_16FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_172B:
        cmp    al, ah
        sbb    eax, 1d0038e0h
        add    byte ptr [edx], ch
        add    byte ptr [esi], dh
        add    byte ptr [eax], al
loc_1_1738:
        add    byte ptr [eax], al
loc_1_173A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi], al
loc_1_1746:
        add    byte ptr [eax], al
loc_1_1748:
        add    byte ptr [eax], al
loc_1_174A:
        add    al, byte ptr [edx]
        sub    al, byte ptr [eax]
        add    dword ptr [ecx], eax
        add    byte ptr ss:[esp + eax], al
        sbb    eax, 1d101000h
        loopne loc_1_1763
        or     byte ptr [eax], bh
        add    byte ptr [eax], ah
        and    byte ptr [eax], bh
        loopne loc_1_1763
loc_1_1763:
        add    byte ptr [eax], al
loc_1_1765:
        add    al, ah
loc_1_1767:
        add    byte ptr [eax], al
sub_1_1769:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_176D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1771:
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
        db  000h
sub_1_1824:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1828:
        add    byte ptr [eax], al
loc_1_182A:
        add    byte ptr [eax], al
loc_1_182C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1830:
        add    al, 0
        add    byte ptr [eax], al
loc_1_1834:
        sub    byte ptr [eax], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        sub    al, 18h
        add    byte ptr [eax], al
loc_1_1850:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1854:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1858:
        push   ebx
        outsd
        jne    SHORT loc_1_18CA
        push   ebx
        outsb
        db  074h, 072h
        jns    SHORT loc_1_18B4
        jb     SHORT loc_1_18D5
        arpl   word ptr [eax], ax
loc_1_1868:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_186C:
        add    byte ptr [eax], al
loc_1_186E:
        add    bh, bh
loc_1_1870:
        add    byte ptr [eax], al
loc_1_1872:
        add    byte ptr [eax], al
loc_1_1874:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_187A:
        add    al, byte ptr [eax]
loc_1_187C:
        add    byte ptr [eax], al
loc_1_187E:
        add    byte ptr [eax], al
loc_1_1880:
        add    byte ptr [eax], al
loc_1_1882:
        add    byte ptr [ebp], al
        xor    eax, 4a0037e0h
        add    byte ptr [esi], cl
        sbb    al, 0e0h
        push   ebx
        add    byte ptr [ecx], cl
        dec    eax
        add    byte ptr [edi], al
        dec    ebp
        add    byte ptr [eax + eax + 4bh], cl
        add    byte ptr [ecx], dl
        push   eax
        add    byte ptr [edi], cl
        push   edx
        db  000h
sub_1_18A5:
        db  07Bh, 008h
        add    byte ptr [eax], al
        aaa
        or     dword ptr [eax], eax
        db  000h, 037h
        or     dword ptr [eax], eax
        db  000h, 037h
        or     dword ptr [eax], eax
loc_1_18B4:
        db  000h, 037h
        or     dword ptr [eax], eax
        db  000h, 01Dh, 009h, 000h, 000h, 037h
        or     dword ptr [eax], eax
        db  000h, 037h
        or     dword ptr [eax], eax
        db  000h, 037h
        or     dword ptr [eax], eax
        db  000h, 037h
loc_1_18CA:
        or     dword ptr [eax], eax
        db  000h, 07Bh, 008h
        add    byte ptr [eax], al
        in     eax, 7
        add    byte ptr [eax], al
loc_1_18D5:
        xor    byte ptr [ecx], cl
        add    byte ptr [eax], al
        pop    edx
        or     byte ptr [eax], al
        db  000h, 05Ah, 008h
        add    byte ptr [eax], al
        pop    edx
        or     byte ptr [eax], al
        db  000h, 0E9h
        pop    es
        add    byte ptr [eax], al
loc_1_18E9:
        add    dword ptr [eax], eax
        add    edi, edi
        add    dword ptr [eax], eax
        add    dword ptr [ecx], eax
        add    bh, bh
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        db  0FFh, 0FFh, 0FFh, 000h, 0FFh, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 003h, 000h
        db  001h, 000h, 000h, 000h, 000h, 002h, 008h, 00Ah, 004h, 000h, 00Ch, 008h, 010h, 012h, 000h, 002h
        db  014h, 010h, 004h, 000h, 0FFh, 000h, 0FFh, 001h, 0FEh, 000h, 0FFh, 000h, 0FFh, 002h, 0FFh, 003h
        db  0FEh, 002h, 0FFh, 002h, 0FDh, 000h, 0FDh, 001h, 0FCh, 000h, 0FDh, 000h, 0FFh, 000h, 0FFh, 001h
        db  0FEh, 000h, 0FFh, 000h
loc_1_193B:
        pop    edx
loc_1_193C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1940:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1944:
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
loc_1_19C4:
        adc    al, byte ptr [edx]
        add    al, byte ptr [ebx]
        add    al, byte ptr [edx]
        add    al, byte ptr [ebx]
        add    al, byte ptr [edx]
        add    al, byte ptr [ebx]
        add    al, byte ptr [edx]
        add    al, byte ptr [ebx]
        add    al, byte ptr [edx]
        add    al, byte ptr [ebx]
        add    al, byte ptr [edx]
        add    byte ptr [eax], al
loc_1_19DC:
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
loc_1_19F8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19FC:
        add    byte ptr [eax], al
loc_1_19FE:
        add    byte ptr [eax], al
loc_1_1A00:
        add    byte ptr [eax], al
loc_1_1A02:
        add    byte ptr [eax], al
loc_1_1A04:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A08:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A0C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A10:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_1_1A18:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A20:
        add    byte ptr [eax], al
loc_1_1A22:
        add    byte ptr [eax], al
loc_1_1A24:
        add    byte ptr [eax], al
loc_1_1A26:
        add    byte ptr [eax], al
loc_1_1A28:
        add    byte ptr [eax], al
loc_1_1A2A:
        add    byte ptr [eax], al
loc_1_1A2C:
        add    byte ptr [eax], al
loc_1_1A2E:
        add    byte ptr [eax], al
loc_1_1A30:
        add    byte ptr [eax], al
loc_1_1A32:
        add    byte ptr [eax], al
loc_1_1A34:
        add    byte ptr [eax], al
        db  0E0h, 01Dh
        db  0E0h, 038h
loc_1_1A3A:
        add    byte ptr [eax], al
loc_1_1A3C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_1_1A44:
        inc    dword ptr [eax]
        add    byte ptr [eax], al
_LTEXT ends
        end
