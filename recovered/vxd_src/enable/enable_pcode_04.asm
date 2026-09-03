; enable_pcode_04.asm -- ENABLE object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_4_0000:
        db  0C6h, 005h, 0D8h, 003h, 000h, 000h, 000h
        ret
loc_4_0008:
        db  080h, 03Dh, 063h, 003h, 000h, 000h, 000h
        jne    SHORT loc_4_002A
        db  0BAh, 03Ch, 001h, 000h, 000h
        mov    eax, 1
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        jb     SHORT loc_4_002C
        db  0C6h, 005h, 063h, 003h, 000h, 000h, 001h
loc_4_002A:
        clc
        ret
loc_4_002C:
        db  00Fh, 0B7h, 005h, 01Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0E8h, 025h, 000h, 0FCh, 0FFh
        stc
        ret
loc_4_0046:
        db  080h, 03Dh, 063h, 003h, 000h, 000h, 000h
        je     SHORT loc_4_0068
        db  0BAh, 088h, 001h, 000h, 000h
        mov    ebx, 0ffffffffh
        db  0CDh, 020h, 002h, 000h, 027h, 000h
        jb     SHORT loc_4_006A
        db  0C6h, 005h, 063h, 003h, 000h, 000h, 000h
loc_4_0068:
        clc
        ret
loc_4_006A:
        db  00Fh, 0B7h, 005h, 01Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0E8h, 0E7h, 0FFh, 0FBh, 0FFh
        stc
        ret
loc_4_0084:
        push   ebx
        db  0F6h, 005h, 0CDh, 003h, 000h, 000h, 040h
        je     SHORT loc_4_0102
        db  0F6h, 005h, 0CDh, 003h, 000h, 000h, 080h
        je     SHORT loc_4_00D5
        db  0C7h, 005h, 0B4h, 003h, 000h, 000h, 021h, 000h, 000h, 000h
        db  0BAh, 034h, 001h, 000h, 000h
        mov    ebx, 0ffffffffh
        db  0CDh, 020h, 002h, 000h, 027h, 000h
        jb     SHORT loc_4_00E1
        db  081h, 025h, 0CCh, 003h, 000h, 000h, 0FFh, 0BFh, 0FFh, 0FFh
        db  0B8h, 096h, 000h, 000h, 000h
        db  0A3h, 0B4h, 003h, 000h, 000h
        push   esi
        db  08Bh, 035h, 056h, 003h, 000h, 000h
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
loc_4_00D5:
        db  0C6h, 005h, 0E7h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_4_0166
loc_4_00E1:
        db  00Fh, 0B7h, 005h, 018h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0E8h, 070h, 0FFh, 0FBh, 0FFh
        db  0C6h, 005h, 0E7h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_0166
loc_4_0102:
        db  0BAh, 0E8h, 000h, 000h, 000h
        mov    eax, 1
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        jb     SHORT loc_4_012A
        db  081h, 00Dh, 0CCh, 003h, 000h, 000h, 000h, 0C0h, 000h, 000h
        db  0E8h, 03Ah, 0FFh, 0FBh, 0FFh
        db  0E8h, 0D8h, 0FEh, 0FBh, 0FFh
        jmp    SHORT loc_4_0166
loc_4_012A:
        db  0C6h, 005h, 0D8h, 003h, 000h, 000h, 001h
        db  081h, 025h, 0CCh, 003h, 000h, 000h, 0FFh, 0BFh, 0FFh, 0FFh
        db  00Fh, 0B7h, 005h, 014h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0E8h, 016h, 0FFh, 0FBh, 0FFh
        push   esi
        db  08Bh, 01Dh, 0CCh, 003h, 000h, 000h
        and    ebx, 77bh
        db  0E8h, 0AFh, 0FEh, 0FBh, 0FFh
        pop    esi
loc_4_0166:
        pop    ebx
        ret
VMINI_GetVersion:
        mov    eax, 400h
        clc
        ret
VMINI_RegisterAccess:
        mov    eax, dword ptr [edi]
        db  0A3h, 0B4h, 003h, 000h, 000h
        mov    eax, dword ptr [edi + 1ch]
        db  0A3h, 05Eh, 003h, 000h, 000h
        mov    eax, dword ptr [edi + 28h]
        db  0A3h, 0B8h, 003h, 000h, 000h
        mov    eax, dword ptr [edi + 24h]
        db  0A3h, 0E2h, 002h, 000h, 000h
        db  0A1h, 0D0h, 003h, 000h, 000h
        mov    dword ptr [edi + 18h], eax
        db  0B8h, 01Ah, 005h, 000h, 000h
        mov    dword ptr [edi + 20h], eax
        db  0B8h, 050h, 008h, 000h, 000h
        mov    dword ptr [edi + 2ch], eax
        db  0B8h, 0E6h, 002h, 000h, 000h
        mov    dword ptr [edi + 30h], eax
        db  0B8h, 0EAh, 002h, 000h, 000h
        mov    dword ptr [edi + 34h], eax
        db  0B8h, 010h, 008h, 000h, 000h
        mov    dword ptr [edi + 38h], eax
        db  0B8h, 004h, 005h, 000h, 000h
        mov    dword ptr [edi + 3ch], eax
        db  081h, 00Dh, 0CCh, 003h, 000h, 000h, 000h, 040h, 000h, 000h
        ret
VMINI_RegisterSK:
        cmp    dword ptr [edi], 14h
        jb     SHORT loc_4_01F8
        mov    dword ptr [edi], 14h
        db  0C7h, 047h, 004h, 07Eh, 002h, 000h, 000h
        db  0C7h, 047h, 008h, 0CCh, 003h, 000h, 000h
        db  0C7h, 047h, 00Ch, 008h, 006h, 000h, 000h
        db  0C7h, 047h, 010h, 00Ch, 007h, 000h, 000h
loc_4_01F8:
        ret
loc_4_01F9:
        mov    ecx, dword ptr [esp + 4]
        pushad
        xchg   ebx, eax
        db  00Fh, 0B7h, 03Dh, 00Ch, 000h, 000h, 000h
        db  003h, 03Dh, 004h, 000h, 000h, 000h
        db  0BEh, 032h, 002h, 000h, 000h
        xor    edx, edx
        mov    eax, 10001000h
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        mov    eax, 1f4h
        xor    edx, edx
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        popad
        ret
loc_4_0232:
        ret
loc_4_0233:
        mov    ecx, dword ptr [esp + 4]
        pushad
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0B8h, 0B3h, 001h, 000h, 000h
        xor    edx, edx
        cmp    ecx, edx
        je     SHORT loc_4_0257
        add    eax, 0ffffffe9h
loc_4_0257:
        push   edx
        push   edx
        push   eax
        db  068h, 090h, 001h, 000h, 000h
        db  0CDh, 020h, 018h, 000h, 017h, 000h
        add    esp, 10h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        popad
        ret
sub_4_0279:
        sub    byte ptr [eax], dl
        add    byte ptr [eax], al
        lodsb
        add    al, byte ptr [eax]
        db  000h, 035h, 010h, 000h, 000h, 04Bh
        adc    byte ptr [eax], al
        db  000h, 008h
        adc    dword ptr [eax], eax
        db  000h
ENABLE_PM_API:
        or     byte ptr [ebp + 2ch], 1
        xor    eax, eax
        mov    ax, word ptr [ebp + 1ch]
        cmp    eax, 5
        jae    SHORT loc_4_02AB
        db  0FFh, 014h, 085h, 079h, 002h, 000h, 000h
        jb     SHORT loc_4_02AB
        and    byte ptr [ebp + 2ch], 0feh
loc_4_02AB:
        ret
loc_4_02AC:
        mov    ebx, dword ptr [ebp + 10h]
        xor    eax, eax
        shr    ebx, 1
        rcr    ax, 1
        bsf    ebx, ebx
        db  00Fh, 0B6h, 09Bh, 064h, 003h, 000h, 000h
        bts    eax, ebx
        xchg   ebx, eax
        push   ebx
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        call   sub_4_112C
        pop    ebx
        test   bh, 10h
        je     SHORT loc_4_030F
        db  0F6h, 005h, 076h, 002h, 000h, 000h, 001h
        je     SHORT loc_4_030F
        db  0A1h, 056h, 003h, 000h, 000h
        test   eax, eax
        je     SHORT loc_4_02F9
        db  08Bh, 035h, 056h, 003h, 000h, 000h
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
loc_4_02F9:
        db  0A1h, 07Ah, 002h, 000h, 000h
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 056h, 003h, 000h, 000h
loc_4_030F:
        cmp    dword ptr [ebp + 14h], 0
        je     SHORT loc_4_031A
        call   sub_4_16B6
loc_4_031A:
        ret
ENABLE_V86_API:
        xor    eax, eax
        mov    ax, word ptr [ebp + 1ch]
        mov    ebx, dword ptr [ebp + 10h]
        cmp    eax, 5
        jae    SHORT loc_4_0332
        db  0FFh, 024h, 085h, 079h, 002h, 000h, 000h
loc_4_0332:
        or     word ptr [ebp + 2ch], 1
        ret
sub_4_0338:
        db  0F6h, 005h, 0CCh, 003h, 000h, 000h, 001h
        je     SHORT loc_4_0346
        call   sub_4_0910
loc_4_0346:
        db  0F6h, 005h, 0CCh, 003h, 000h, 000h, 004h
        je     SHORT loc_4_0354
        call   sub_4_0B57
loc_4_0354:
        db  0F6h, 005h, 0CCh, 003h, 000h, 000h, 008h
        je     SHORT loc_4_0362
        call   sub_4_0CAB
loc_4_0362:
        db  0F6h, 005h, 0CCh, 003h, 000h, 000h, 040h
        je     SHORT loc_4_0370
        call   sub_4_0AAA
loc_4_0370:
        db  0F6h, 005h, 0CDh, 003h, 000h, 000h, 001h
        je     SHORT loc_4_037E
        call   sub_4_0883
loc_4_037E:
        db  0F6h, 005h, 0CDh, 003h, 000h, 000h, 002h
        je     SHORT loc_4_038C
        call   sub_4_09C3
loc_4_038C:
        db  0F6h, 005h, 0CDh, 003h, 000h, 000h, 008h
        je     SHORT loc_4_039A
        call   sub_4_0C05
loc_4_039A:
        ret
VMINI_GetData:
        jmp    NEAR PTR sub_4_112C
sub_4_03A0:
        bsf    eax, ebx
        je     SHORT loc_4_03B0
        cmp    al, 0dh
        ja     SHORT loc_4_03B0
        db  0FFh, 024h, 085h, 01Eh, 003h, 000h, 000h
loc_4_03B0:
        clc
        ret
sub_4_03B2:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_4_03CC
        push   0
        push   eax
        db  068h, 033h, 002h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
        ret
loc_4_03CC:
        db  00Fh, 0B7h, 005h, 00Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        db  0E8h, 08Bh, 0FCh, 0FBh, 0FFh
        xor    eax, eax
        ret
loc_4_03E1:
        db  089h, 015h, 074h, 003h, 000h, 000h
        db  080h, 03Dh, 050h, 008h, 000h, 000h, 000h
        je     SHORT loc_4_03FB
        db  08Bh, 015h, 0CCh, 003h, 000h, 000h
        call   sub_4_051A
loc_4_03FB:
        xchg   edx, eax
        test   eax, 0fffff7ffh
        jne    SHORT loc_4_040C
        test   ah, 8
        jne    NEAR PTR loc_4_04A8
loc_4_040C:
        test   al, 80h
        je     SHORT loc_4_043D
        db  0F6h, 005h, 0CCh, 003h, 000h, 000h, 080h
        je     SHORT loc_4_042C
        push   0
        push   eax
        db  068h, 008h, 000h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
        jmp    SHORT loc_4_043D
loc_4_042C:
        push   0
        push   eax
        db  068h, 046h, 000h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
loc_4_043D:
        db  066h, 0F7h, 005h, 0CCh, 003h, 000h, 000h, 07Bh, 007h
        jne    SHORT loc_4_046C
        db  0F6h, 005h, 0CDh, 003h, 000h, 000h, 040h
        je     SHORT loc_4_04A8
        db  080h, 03Dh, 0D8h, 003h, 000h, 000h, 001h
        je     SHORT loc_4_04A8
        db  080h, 03Dh, 0E7h, 000h, 000h, 000h, 001h
        je     SHORT loc_4_04A8
        db  0C6h, 005h, 0E7h, 000h, 000h, 000h, 001h
        jmp    SHORT loc_4_0490
loc_4_046C:
        db  080h, 03Dh, 0D8h, 003h, 000h, 000h, 001h
        je     SHORT loc_4_04A8
        db  0F6h, 005h, 0CDh, 003h, 000h, 000h, 040h
        jne    SHORT loc_4_04A8
        db  0F6h, 005h, 0C7h, 011h, 000h, 000h, 001h
        je     SHORT loc_4_0490
        db  0F6h, 005h, 0C7h, 011h, 000h, 000h, 002h
        jne    SHORT loc_4_04A8
loc_4_0490:
        push   0
        push   eax
        db  068h, 084h, 000h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
        db  080h, 00Dh, 0C7h, 011h, 000h, 000h, 002h
loc_4_04A8:
        db  08Bh, 015h, 074h, 003h, 000h, 000h
        db  066h, 0F7h, 005h, 0CCh, 003h, 000h, 000h, 07Bh, 007h
        jne    SHORT loc_4_04C5
        db  081h, 025h, 0CCh, 003h, 000h, 000h, 0FFh, 0FFh, 0FEh, 0FFh
        jmp    SHORT loc_4_04FD
loc_4_04C5:
        db  085h, 015h, 0CCh, 003h, 000h, 000h
        je     SHORT loc_4_04FD
        bsf    eax, edx
        db  08Bh, 034h, 085h, 09Ah, 003h, 000h, 000h
        mov    esi, dword ptr [esi]
        db  085h, 034h, 085h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_04FD
        db  081h, 00Dh, 0CCh, 003h, 000h, 000h, 000h, 000h, 001h, 000h
        push   0
        push   eax
        db  068h, 0FFh, 004h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
loc_4_04FD:
        clc
        ret
loc_4_04FF:
        push   ebx
        xor    edx, edx
        db  0B8h, 0E3h, 001h, 000h, 000h
        push   edx
        push   edx
        push   eax
        db  068h, 090h, 001h, 000h, 000h
        db  0CDh, 020h, 018h, 000h, 017h, 000h
        add    esp, 10h
        pop    ebx
        ret
sub_4_051A:
        pushad
        xor    ebx, ebx
        db  038h, 01Dh, 050h, 008h, 000h, 000h
        je     SHORT loc_4_0562
        db  089h, 015h, 0EAh, 002h, 000h, 000h
loc_4_052B:
        db  081h, 03Ch, 0DDh, 010h, 008h, 000h, 000h, 0FFh, 0FFh, 000h, 000h
        je     SHORT loc_4_055C
        db  08Bh, 004h, 0DDh, 010h, 008h, 000h, 000h
        db  08Bh, 00Ch, 0DDh, 014h, 008h, 000h, 000h
        push   ebx
        push   edx
        push   0
        push   0
        push   edx
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 006h, 000h, 017h, 000h
        add    esp, 18h
        pop    edx
        pop    ebx
loc_4_055C:
        inc    ebx
        cmp    ebx, 8
        jl     SHORT loc_4_052B
loc_4_0562:
        popad
        ret
        db  060h, 033h, 0DBh, 038h, 01Dh, 050h, 008h, 000h, 000h, 074h, 03Dh, 089h, 015h, 0E6h, 002h, 000h
        db  000h, 081h, 03Ch, 0DDh, 010h, 008h, 000h, 000h, 0FFh, 0FFh, 000h, 000h, 074h, 024h, 08Bh, 004h
        db  0DDh, 010h, 008h, 000h, 000h, 08Bh, 00Ch, 0DDh, 014h, 008h, 000h, 000h, 053h, 052h, 06Ah, 000h
        db  06Ah, 000h, 052h, 06Ah, 001h, 051h, 050h, 0CDh, 020h, 006h, 000h, 017h, 000h, 083h, 0C4h, 018h
        db  05Ah, 05Bh, 043h, 083h, 0FBh, 008h, 07Ch, 0C9h, 061h, 0C3h, 000h, 000h
loc_4_05B0:
        clc
        ret
loc_4_05B2:
        db  089h, 015h, 05Eh, 004h, 000h, 000h
        xor    ecx, ecx
        bsf    eax, edx
        je     SHORT loc_4_05D6
        push   ebx
        db  08Bh, 01Ch, 085h, 0CAh, 003h, 000h, 000h
        test   ebx, ebx
        je     SHORT loc_4_05D6
        test   dword ptr [ebx], 4
        jne    SHORT loc_4_05D6
        pop    ebx
        clc
        ret
loc_4_05D6:
        xor    ecx, ecx
        db  08Bh, 01Ch, 085h, 09Ah, 003h, 000h, 000h
        test   byte ptr [ebx], 8
        je     SHORT loc_4_05EB
        mov    cx, word ptr [ebx]
        and    cx, 8
loc_4_05EB:
        db  088h, 00Dh, 0E6h, 000h, 000h, 000h
        db  0C6h, 005h, 063h, 004h, 000h, 000h, 000h
        db  08Bh, 01Ch, 085h, 0FAh, 003h, 000h, 000h
        test   ebx, ebx
        je     SHORT loc_4_060F
        test   byte ptr [ebx], 10h
        je     SHORT loc_4_060F
        db  0C6h, 005h, 063h, 004h, 000h, 000h, 001h
loc_4_060F:
        db  0CDh, 020h, 002h, 000h, 00Ch, 000h
        db  089h, 01Dh, 069h, 004h, 000h, 000h
        pop    ebx
        push   ebx
        db  0C6h, 005h, 064h, 004h, 000h, 000h, 000h
        db  085h, 015h, 0CCh, 003h, 000h, 000h
        jne    SHORT loc_4_069D
        db  08Bh, 00Ch, 085h, 02Ah, 004h, 000h, 000h
        db  089h, 00Dh, 05Ah, 004h, 000h, 000h
        db  080h, 03Dh, 063h, 004h, 000h, 000h, 000h
        je     SHORT loc_4_0660
        push   ebx
        push   edx
        mov    ecx, 1
        db  088h, 00Dh, 064h, 004h, 000h, 000h
        mov    eax, 1
        mov    ebx, 0c8h
        call   sub_4_0E78
        pop    edx
        pop    ebx
loc_4_0660:
        db  080h, 03Dh, 0E6h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_068F
        db  089h, 01Dh, 065h, 004h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0C6h, 005h, 0DDh, 003h, 000h, 000h, 001h
        push   0
        push   edx
        db  068h, 0C2h, 006h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
        pop    ebx
        ret
loc_4_068F:
        bsf    eax, edx
        je     SHORT loc_4_06C1
        db  08Bh, 00Ch, 085h, 02Ah, 004h, 000h, 000h
        jmp    SHORT loc_4_06A9
loc_4_069D:
        bsf    eax, edx
        je     SHORT loc_4_06C1
        db  08Bh, 00Ch, 085h, 0EEh, 002h, 000h, 000h
loc_4_06A9:
        db  089h, 00Dh, 05Ah, 004h, 000h, 000h
        pop    ebx
        or     ecx, ecx
        je     SHORT loc_4_06C1
        call   ecx
        db  08Bh, 01Dh, 05Eh, 004h, 000h, 000h
        call   sub_4_16B6
loc_4_06C1:
        ret
loc_4_06C2:
        test   byte ptr [esp + 5], 40h
        je     SHORT loc_4_06CF
        db  0FFh, 015h, 0B8h, 003h, 000h, 000h
loc_4_06CF:
        mov    ecx, dword ptr [esp + 4]
        db  066h, 089h, 00Dh, 06Dh, 004h, 000h, 000h
        pushad
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0B8h, 084h, 003h, 000h, 000h
        db  068h, 06Dh, 004h, 000h, 000h
        push   2
        push   eax
        db  068h, 090h, 001h, 000h, 000h
        db  0CDh, 020h, 018h, 000h, 017h, 000h
        add    esp, 10h
        db  0C6h, 005h, 0DDh, 003h, 000h, 000h, 000h
        cmp    eax, 64h
        jb     SHORT loc_4_071F
        mov    edx, dword ptr [esp + 70h]
        call   sub_4_07BE
        mov    eax, 2
loc_4_071F:
        cmp    eax, 2
        jne    SHORT loc_4_072E
        db  0C7h, 005h, 05Ah, 004h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_072E:
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        popad
        db  083h, 03Dh, 05Ah, 004h, 000h, 000h, 000h
        jne    SHORT loc_4_076D
        db  0C6h, 005h, 064h, 004h, 000h, 000h, 000h
        db  080h, 03Dh, 063h, 004h, 000h, 000h, 000h
        je     SHORT loc_4_076B
        mov    eax, 2
        mov    ebx, 0c8h
        mov    ecx, 1
        call   sub_4_0E78
loc_4_076B:
        jmp    SHORT loc_4_0782
loc_4_076D:
        push   esi
        push   edi
        db  0FFh, 015h, 05Ah, 004h, 000h, 000h
        db  08Bh, 01Dh, 05Eh, 004h, 000h, 000h
        call   sub_4_16B6
        pop    edi
        pop    esi
loc_4_0782:
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  03Bh, 01Dh, 065h, 004h, 000h, 000h
        je     SHORT loc_4_07BD
        db  039h, 01Dh, 069h, 004h, 000h, 000h
        je     SHORT loc_4_07BD
        db  08Bh, 01Dh, 065h, 004h, 000h, 000h
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_4_07BD
        mov    eax, 0fh
        db  08Bh, 01Dh, 065h, 004h, 000h, 000h
        xor    esi, esi
        xor    edi, edi
        xor    edx, edx
        db  0CDh, 020h, 093h, 000h, 001h, 000h
loc_4_07BD:
        ret
sub_4_07BE:
        pushad
        db  0F7h, 005h, 05Eh, 004h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_4_07DF
        db  083h, 025h, 06Eh, 002h, 000h, 000h, 0FBh
        db  08Bh, 01Dh, 05Eh, 004h, 000h, 000h
        call   sub_4_16B6
        jmp    SHORT loc_4_085D
loc_4_07DF:
        db  0F7h, 005h, 05Eh, 004h, 000h, 000h, 040h, 000h, 000h, 000h
        je     SHORT loc_4_07FF
        db  083h, 025h, 044h, 002h, 000h, 000h, 0FBh
        db  08Bh, 01Dh, 05Eh, 004h, 000h, 000h
        call   sub_4_16B6
        jmp    SHORT loc_4_085D
loc_4_07FF:
        db  0F7h, 005h, 05Eh, 004h, 000h, 000h, 000h, 001h, 000h, 000h
        je     SHORT loc_4_081F
        db  083h, 025h, 024h, 002h, 000h, 000h, 0FBh
        db  08Bh, 01Dh, 05Eh, 004h, 000h, 000h
        call   sub_4_16B6
        jmp    SHORT loc_4_085D
loc_4_081F:
        db  0F7h, 005h, 05Eh, 004h, 000h, 000h, 000h, 002h, 000h, 000h
        je     SHORT loc_4_083F
        db  083h, 025h, 03Ch, 002h, 000h, 000h, 0FBh
        db  08Bh, 01Dh, 05Eh, 004h, 000h, 000h
        call   sub_4_16B6
        jmp    SHORT loc_4_085D
loc_4_083F:
        db  0F7h, 005h, 05Eh, 004h, 000h, 000h, 000h, 008h, 000h, 000h
        je     SHORT loc_4_085D
        db  083h, 025h, 0CAh, 002h, 000h, 000h, 0FBh
        db  08Bh, 01Dh, 05Eh, 004h, 000h, 000h
        call   sub_4_16B6
loc_4_085D:
        popad
        ret
loc_4_085F:
        db  083h, 00Dh, 024h, 002h, 000h, 000h, 001h
        db  081h, 00Dh, 0CCh, 003h, 000h, 000h, 000h, 001h, 000h, 000h
        mov    eax, 100h
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        db  0CDh, 020h, 004h, 000h, 037h, 000h
        clc
        ret
sub_4_0883:
        db  083h, 025h, 024h, 002h, 000h, 000h, 0FEh
        db  081h, 025h, 0CCh, 003h, 000h, 000h, 0FFh, 0FEh, 0FFh, 0FFh
        mov    eax, 100h
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        clc
        ret
loc_4_08A1:
        db  0F7h, 005h, 024h, 002h, 000h, 000h, 001h, 000h, 000h, 000h
        jne    SHORT loc_4_08C5
        db  0F7h, 005h, 0CCh, 003h, 000h, 000h, 000h, 001h, 000h, 000h
        jne    SHORT sub_4_0883
        mov    eax, 100h
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        ret
loc_4_08C5:
        db  0F7h, 005h, 0CCh, 003h, 000h, 000h, 000h, 001h, 000h, 000h
        je     SHORT loc_4_085F
        mov    eax, 100h
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        ret
loc_4_08DD:
        db  083h, 00Dh, 06Eh, 002h, 000h, 000h, 001h
        db  083h, 00Dh, 0CCh, 003h, 000h, 000h, 001h
        mov    eax, 1
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        db  0CDh, 020h, 004h, 000h, 037h, 000h
        db  0CDh, 020h, 008h, 000h, 00Dh, 000h
        db  0CDh, 020h, 011h, 000h, 00Dh, 000h
        db  066h, 0A3h, 0C4h, 003h, 000h, 000h
        clc
        ret
sub_4_0910:
        db  083h, 03Dh, 05Eh, 003h, 000h, 000h, 000h
        je     SHORT loc_4_0924
        mov    eax, 1
        db  0FFh, 015h, 05Eh, 003h, 000h, 000h
loc_4_0924:
        db  0F7h, 005h, 06Eh, 002h, 000h, 000h, 010h, 000h, 000h, 000h
        je     SHORT loc_4_0944
        mov    eax, 2
        mov    ebx, 0c8h
        mov    ecx, 1
        call   sub_4_0E78
loc_4_0944:
        db  083h, 025h, 06Eh, 002h, 000h, 000h, 0FEh
        db  083h, 025h, 0CCh, 003h, 000h, 000h, 0FEh
        mov    eax, 1
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        clc
        ret
loc_4_095F:
        db  0F7h, 005h, 06Eh, 002h, 000h, 000h, 001h, 000h, 000h, 000h
        jne    SHORT loc_4_0983
        db  0F7h, 005h, 0CCh, 003h, 000h, 000h, 001h, 000h, 000h, 000h
        jne    SHORT sub_4_0910
        mov    eax, 1
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        ret
loc_4_0983:
        db  0F7h, 005h, 0CCh, 003h, 000h, 000h, 001h, 000h, 000h, 000h
        je     NEAR PTR loc_4_08DD
        mov    eax, 1
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        ret
loc_4_099F:
        db  081h, 00Dh, 0CCh, 003h, 000h, 000h, 000h, 002h, 000h, 000h
        db  083h, 00Dh, 03Ch, 002h, 000h, 000h, 001h
        mov    eax, 200h
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        db  0CDh, 020h, 004h, 000h, 037h, 000h
        clc
        ret
sub_4_09C3:
        db  0F7h, 005h, 03Ch, 002h, 000h, 000h, 010h, 000h, 000h, 000h
        je     SHORT loc_4_09E3
        mov    eax, 2
        mov    ebx, 0c8h
        mov    ecx, 1
        call   sub_4_0E78
loc_4_09E3:
        db  081h, 025h, 0CCh, 003h, 000h, 000h, 0FFh, 0FDh, 0FFh, 0FFh
        db  083h, 025h, 03Ch, 002h, 000h, 000h, 0FEh
        mov    eax, 200h
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        clc
        ret
loc_4_0A01:
        db  0F7h, 005h, 03Ch, 002h, 000h, 000h, 001h, 000h, 000h, 000h
        jne    SHORT loc_4_0A25
        db  0F7h, 005h, 0CCh, 003h, 000h, 000h, 000h, 002h, 000h, 000h
        jne    SHORT sub_4_09C3
        mov    eax, 200h
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        ret
loc_4_0A25:
        db  0F7h, 005h, 0CCh, 003h, 000h, 000h, 000h, 002h, 000h, 000h
        je     NEAR PTR loc_4_099F
        mov    eax, 200h
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        ret
loc_4_0A41:
        db  080h, 03Dh, 073h, 004h, 000h, 000h, 001h
        je     SHORT loc_4_0A82
        db  0A1h, 044h, 002h, 000h, 000h
        and    eax, 80h
        je     SHORT loc_4_0A6A
        db  0CDh, 020h, 011h, 000h, 00Dh, 000h
        test   eax, 20h
        je     SHORT loc_4_0A82
        db  0E8h, 00Dh, 0FAh, 0FBh, 0FFh
        jmp    SHORT loc_4_0A82
loc_4_0A6A:
        db  0CDh, 020h, 011h, 000h, 00Dh, 000h
        test   eax, 20h
        jne    SHORT loc_4_0A82
        db  0BEh, 075h, 004h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
loc_4_0A82:
        db  083h, 00Dh, 044h, 002h, 000h, 000h, 001h
        db  083h, 00Dh, 0CCh, 003h, 000h, 000h, 040h
        mov    eax, 40h
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        db  0CDh, 020h, 004h, 000h, 037h, 000h
        db  0C6h, 005h, 073h, 004h, 000h, 000h, 000h
        clc
        ret
sub_4_0AAA:
        db  0F7h, 005h, 044h, 002h, 000h, 000h, 010h, 000h, 000h, 000h
        je     SHORT loc_4_0ACA
        mov    eax, 2
        mov    ebx, 0c8h
        mov    ecx, 1
        call   sub_4_0E78
loc_4_0ACA:
        db  083h, 025h, 044h, 002h, 000h, 000h, 0FEh
        db  083h, 025h, 0CCh, 003h, 000h, 000h, 0BFh
        mov    eax, 40h
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        clc
        ret
loc_4_0AE5:
        db  0F7h, 005h, 044h, 002h, 000h, 000h, 001h, 000h, 000h, 000h
        jne    SHORT loc_4_0B09
        db  0F7h, 005h, 0CCh, 003h, 000h, 000h, 040h, 000h, 000h, 000h
        jne    SHORT sub_4_0AAA
        mov    eax, 40h
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        ret
loc_4_0B09:
        db  0C6h, 005h, 073h, 004h, 000h, 000h, 001h
        db  0F7h, 005h, 0CCh, 003h, 000h, 000h, 040h, 000h, 000h, 000h
        je     NEAR PTR loc_4_0A41
        db  0C6h, 005h, 073h, 004h, 000h, 000h, 000h
        mov    eax, 40h
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        ret
loc_4_0B33:
        db  0C7h, 005h, 060h, 002h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  083h, 00Dh, 0CCh, 003h, 000h, 000h, 004h
        mov    eax, 4
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        db  0CDh, 020h, 004h, 000h, 037h, 000h
        clc
        ret
sub_4_0B57:
        db  0C7h, 005h, 060h, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        db  083h, 025h, 0CCh, 003h, 000h, 000h, 0FBh
        mov    eax, 4
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        clc
        ret
loc_4_0B75:
        db  083h, 03Dh, 060h, 002h, 000h, 000h, 000h
        jne    SHORT loc_4_0B96
        db  0F7h, 005h, 0CCh, 003h, 000h, 000h, 004h, 000h, 000h, 000h
        jne    SHORT sub_4_0B57
        mov    eax, 4
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        ret
loc_4_0B96:
        db  0F7h, 005h, 0CCh, 003h, 000h, 000h, 004h, 000h, 000h, 000h
        je     SHORT loc_4_0B33
        mov    eax, 4
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        ret
loc_4_0BAE:
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        xor    eax, eax
        inc    eax
        call   sub_4_03B2
        db  080h, 00Dh, 0CAh, 002h, 000h, 000h, 001h
        db  080h, 00Dh, 0CDh, 003h, 000h, 000h, 008h
        mov    eax, 800h
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        db  0F6h, 005h, 076h, 002h, 000h, 000h, 001h
        je     SHORT loc_4_0BFD
        db  0A1h, 056h, 003h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_4_0BFD
        db  0A1h, 07Ah, 002h, 000h, 000h
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 056h, 003h, 000h, 000h
loc_4_0BFD:
        db  0CDh, 020h, 004h, 000h, 037h, 000h
        clc
        ret
sub_4_0C05:
        db  0F7h, 005h, 0CAh, 002h, 000h, 000h, 010h, 000h, 000h, 000h
        je     SHORT loc_4_0C25
        mov    eax, 2
        mov    ebx, 0c8h
        mov    ecx, 1
        call   sub_4_0E78
loc_4_0C25:
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, 0
        call   sub_4_03B2
        db  083h, 025h, 0CAh, 002h, 000h, 000h, 0FEh
        db  081h, 025h, 0CCh, 003h, 000h, 000h, 0FFh, 0F7h, 0FFh, 0FFh
        mov    eax, 800h
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        clc
        ret
loc_4_0C53:
        db  0F6h, 005h, 0CAh, 002h, 000h, 000h, 001h
        jne    SHORT loc_4_0C71
        db  0F6h, 005h, 0CDh, 003h, 000h, 000h, 008h
        jne    SHORT sub_4_0C05
        mov    eax, 800h
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        ret
loc_4_0C71:
        db  0F6h, 005h, 0CDh, 003h, 000h, 000h, 008h
        je     NEAR PTR loc_4_0BAE
        mov    eax, 800h
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        ret
loc_4_0C8A:
        db  083h, 00Dh, 09Ah, 002h, 000h, 000h, 001h
        db  083h, 00Dh, 0CCh, 003h, 000h, 000h, 008h
        mov    eax, 8
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        db  0CDh, 020h, 004h, 000h, 037h, 000h
        clc
        ret
sub_4_0CAB:
        db  083h, 025h, 09Ah, 002h, 000h, 000h, 0FEh
        db  083h, 025h, 0CCh, 003h, 000h, 000h, 0F7h
        mov    eax, 8
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        clc
        ret
loc_4_0CC6:
        db  0F7h, 005h, 09Ah, 002h, 000h, 000h, 001h, 000h, 000h, 000h
        jne    SHORT loc_4_0CEA
        db  0F7h, 005h, 0CCh, 003h, 000h, 000h, 008h, 000h, 000h, 000h
        jne    SHORT sub_4_0CAB
        mov    eax, 8
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        ret
loc_4_0CEA:
        db  0F7h, 005h, 0CCh, 003h, 000h, 000h, 008h, 000h, 000h, 000h
        je     SHORT loc_4_0C8A
        mov    eax, 8
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        ret
sub_4_0D02:
        db  080h, 00Dh, 082h, 002h, 000h, 000h, 001h
        db  080h, 00Dh, 0CCh, 003h, 000h, 000h, 080h
        mov    eax, 80h
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        db  0CDh, 020h, 004h, 000h, 037h, 000h
        clc
        ret
sub_4_0D23:
        db  080h, 025h, 082h, 002h, 000h, 000h, 0FEh
        db  080h, 025h, 0CCh, 003h, 000h, 000h, 07Fh
        mov    eax, 80h
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        clc
        ret
loc_4_0D3E:
        db  0F6h, 005h, 082h, 002h, 000h, 000h, 001h
        jne    SHORT loc_4_0D5C
        db  0F6h, 005h, 0CCh, 003h, 000h, 000h, 080h
        jne    SHORT sub_4_0D23
        mov    eax, 80h
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        ret
loc_4_0D5C:
        db  0F6h, 005h, 0CCh, 003h, 000h, 000h, 080h
        je     SHORT sub_4_0D02
        mov    eax, 80h
        db  0CDh, 020h, 001h, 000h, 037h, 000h
        ret
        db  000h, 000h, 000h
loc_4_0D74:
        db  080h, 03Dh, 0FCh, 004h, 000h, 000h, 000h
        je     SHORT loc_4_0DFB
        cmp    dx, 0
        je     SHORT loc_4_0DCA
        db  066h, 083h, 02Dh, 0F8h, 004h, 000h, 000h, 003h
        db  066h, 083h, 03Dh, 0F8h, 004h, 000h, 000h, 000h
        jle    SHORT loc_4_0DCA
        db  066h, 001h, 015h, 0FAh, 004h, 000h, 000h
        db  066h, 0A1h, 0FAh, 004h, 000h, 000h
        out    42h, al
        mov    al, ah
        jmp    SHORT loc_4_0DA8
loc_4_0DA8:
        out    42h, al
        jmp    SHORT loc_4_0DAC
loc_4_0DAC:
        in     al, 61h
        or     al, 3
        out    61h, al
        mov    eax, 3
        db  0BEh, 074h, 00Dh, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  0C6h, 005h, 0FCh, 004h, 000h, 000h, 0FFh
        ret
loc_4_0DCA:
        call   sub_4_100D
        db  0FEh, 00Dh, 0FDh, 004h, 000h, 000h
        db  080h, 03Dh, 0FDh, 004h, 000h, 000h, 000h
        je     SHORT loc_4_0DFB
        xor    eax, eax
        xor    ebx, ebx
        xor    ecx, ecx
        db  0A0h, 000h, 005h, 000h, 000h
        db  066h, 08Bh, 01Dh, 0FEh, 004h, 000h, 000h
        db  08Ah, 00Dh, 0FDh, 004h, 000h, 000h
        jmp    NEAR PTR loc_4_0EE4
loc_4_0DFB:
        db  083h, 03Dh, 074h, 004h, 000h, 000h, 000h
        je     SHORT loc_4_0E38
        db  08Bh, 015h, 07Ch, 004h, 000h, 000h
        db  08Bh, 004h, 095h, 0D0h, 004h, 000h, 000h
        db  08Bh, 01Ch, 095h, 0A8h, 004h, 000h, 000h
        db  08Bh, 00Ch, 095h, 080h, 004h, 000h, 000h
        db  0FFh, 00Dh, 074h, 004h, 000h, 000h
        inc    edx
        cmp    edx, 0ah
        jb     SHORT loc_4_0E2D
        xor    edx, edx
loc_4_0E2D:
        db  089h, 015h, 07Ch, 004h, 000h, 000h
        jmp    NEAR PTR loc_4_0EE4
loc_4_0E38:
        ret
sub_4_0E39:
        db  08Bh, 01Dh, 0FCh, 001h, 000h, 000h
        cmp    eax, 0
        je     SHORT loc_4_0E76
        db  08Bh, 01Dh, 000h, 002h, 000h, 000h
        cmp    eax, 1
        je     SHORT loc_4_0E76
        db  08Bh, 01Dh, 004h, 002h, 000h, 000h
        cmp    eax, 2
        je     SHORT loc_4_0E76
        db  08Bh, 01Dh, 008h, 002h, 000h, 000h
        cmp    eax, 3
        je     SHORT loc_4_0E76
        db  08Bh, 01Dh, 00Ch, 002h, 000h, 000h
        cmp    eax, 4
        je     SHORT loc_4_0E76
        db  08Bh, 01Dh, 010h, 002h, 000h, 000h
loc_4_0E76:
        ret
        db  090h
sub_4_0E78:
        db  080h, 03Dh, 051h, 008h, 000h, 000h, 000h
        je     SHORT loc_4_0E86
        jmp    NEAR PTR loc_4_100B
loc_4_0E86:
        db  080h, 03Dh, 062h, 003h, 000h, 000h, 000h
        je     SHORT loc_4_0E94
        jmp    NEAR PTR loc_4_100B
loc_4_0E94:
        call   sub_4_0E39
        db  080h, 03Dh, 0FCh, 004h, 000h, 000h, 000h
        je     SHORT loc_4_0EE4
        db  083h, 03Dh, 074h, 004h, 000h, 000h, 00Ah
        jae    NEAR PTR loc_4_100B
        db  08Bh, 015h, 078h, 004h, 000h, 000h
        db  089h, 004h, 095h, 0D0h, 004h, 000h, 000h
        db  089h, 01Ch, 095h, 0A8h, 004h, 000h, 000h
        db  089h, 00Ch, 095h, 080h, 004h, 000h, 000h
        db  0FFh, 005h, 074h, 004h, 000h, 000h
        inc    edx
        cmp    edx, 0ah
        jb     SHORT loc_4_0ED8
        xor    edx, edx
loc_4_0ED8:
        db  089h, 015h, 078h, 004h, 000h, 000h
        jmp    NEAR PTR loc_4_100B
        db  090h
loc_4_0EE4:
        db  0CDh, 020h, 002h, 000h, 00Bh, 000h
        cmp    cl, 0
        jne    SHORT loc_4_0EF9
loc_4_0EEF:
        call   sub_4_100D
        jmp    NEAR PTR loc_4_100B
loc_4_0EF9:
        db  080h, 03Dh, 0FCh, 004h, 000h, 000h, 000h
        jne    SHORT loc_4_0EEF
        push   eax
        mov    al, 1
        db  0CDh, 020h, 003h, 000h, 00Bh, 000h
        pop    eax
        db  066h, 089h, 01Dh, 0F8h, 004h, 000h, 000h
        db  088h, 00Dh, 0FDh, 004h, 000h, 000h
        db  066h, 089h, 01Dh, 0FEh, 004h, 000h, 000h
        db  0A2h, 000h, 005h, 000h, 000h
        db  066h, 08Bh, 015h, 01Eh, 002h, 000h, 000h
        cmp    al, 5
        je     SHORT loc_4_0F53
        db  066h, 08Bh, 015h, 014h, 002h, 000h, 000h
        cmp    al, 0
        je     SHORT loc_4_0F53
        db  066h, 08Bh, 015h, 01Ah, 002h, 000h, 000h
        cmp    al, 4
        je     SHORT loc_4_0F53
        db  066h, 08Bh, 015h, 01Ch, 002h, 000h, 000h
        cmp    al, 3
        je     SHORT loc_4_0F53
        jmp    SHORT loc_4_0F8E
loc_4_0F53:
        db  0C6h, 005h, 0FCh, 004h, 000h, 000h, 001h
        mov    ax, dx
        xor    dx, dx
        out    42h, al
        mov    al, ah
        jmp    SHORT loc_4_0F66
loc_4_0F66:
        out    42h, al
        jmp    SHORT loc_4_0F6A
loc_4_0F6A:
        in     al, 61h
        or     al, 3
        out    61h, al
        xor    eax, eax
        xor    edx, edx
        db  066h, 0A1h, 0FEh, 004h, 000h, 000h
        db  0BEh, 074h, 00Dh, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  0C6h, 005h, 0FCh, 004h, 000h, 000h, 0FFh
        jmp    SHORT loc_4_100B
loc_4_0F8E:
        db  0C6h, 005h, 0FCh, 004h, 000h, 000h, 001h
        db  066h, 089h, 01Dh, 0FEh, 004h, 000h, 000h
        db  066h, 089h, 01Dh, 0F8h, 004h, 000h, 000h
        cmp    al, 2
        jne    SHORT loc_4_0FDA
        db  066h, 0A1h, 016h, 002h, 000h, 000h
        db  066h, 0A3h, 0FAh, 004h, 000h, 000h
        out    42h, al
        mov    al, ah
        jmp    SHORT loc_4_0FB9
loc_4_0FB9:
        out    42h, al
        jmp    SHORT loc_4_0FBD
loc_4_0FBD:
        in     al, 61h
        or     al, 3
        out    61h, al
        mov    eax, 3
        db  0BEh, 074h, 00Dh, 000h, 000h
        mov    edx, 1eh
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        jmp    SHORT loc_4_100B
loc_4_0FDA:
        db  066h, 0A1h, 01Ah, 002h, 000h, 000h
        db  066h, 0A3h, 0FAh, 004h, 000h, 000h
        out    42h, al
        mov    al, ah
        jmp    SHORT loc_4_0FEC
loc_4_0FEC:
        out    42h, al
        jmp    SHORT loc_4_0FF0
loc_4_0FF0:
        in     al, 61h
        or     al, 3
        out    61h, al
        mov    eax, 3
        db  0BEh, 074h, 00Dh, 000h, 000h
        mov    edx, 0ffffffe2h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
loc_4_100B:
        clc
        ret
sub_4_100D:
        in     al, 61h
        and    al, 0fch
        jmp    SHORT loc_4_1013
loc_4_1013:
        out    61h, al
        db  0C6h, 005h, 0FCh, 004h, 000h, 000h, 000h
        mov    al, 0
        db  0CDh, 020h, 003h, 000h, 00Bh, 000h
        ret
        db  000h, 000h, 000h
loc_4_1028:
        mov    dword ptr [ebp + 1ch], 400h
        and    word ptr [ebp + 2ch], 0fffeh
        ret
loc_4_1035:
        db  0A1h, 0CCh, 003h, 000h, 000h
        mov    word ptr [ebp + 1ch], ax
        ror    eax, 10h
        mov    word ptr [ebp + 14h], ax
        and    word ptr [ebp + 2ch], 0fffeh
        ret
loc_4_104B:
        xor    eax, eax
        db  0F7h, 005h, 0CCh, 003h, 000h, 000h, 000h, 040h, 000h, 000h
        je     SHORT loc_4_106C
        db  0F7h, 005h, 0CCh, 003h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_4_106C
        db  0A1h, 0E2h, 002h, 000h, 000h
        mov    eax, dword ptr [eax]
loc_4_106C:
        mov    dword ptr [ebp + 1ch], eax
        db  0A1h, 0CCh, 003h, 000h, 000h
        mov    dword ptr [ebp + 10h], eax
        and    word ptr [ebp + 2ch], 0fffeh
        ret
sub_4_107D:
        cmp    eax, 0
        je     SHORT loc_4_10B6
        db  080h, 03Dh, 050h, 008h, 000h, 000h, 008h
        je     SHORT loc_4_1106
loc_4_108B:
        db  081h, 03Ch, 0DDh, 010h, 008h, 000h, 000h, 0FFh, 0FFh, 000h, 000h
        je     SHORT loc_4_10A0
        inc    ebx
        cmp    ebx, 8
        jge    SHORT loc_4_1106
        jmp    SHORT loc_4_108B
loc_4_10A0:
        db  089h, 00Ch, 0DDh, 010h, 008h, 000h, 000h
        db  089h, 014h, 0DDh, 014h, 008h, 000h, 000h
        db  0FEh, 005h, 050h, 008h, 000h, 000h
        jmp    SHORT loc_4_10E3
loc_4_10B6:
        db  039h, 00Ch, 0DDh, 010h, 008h, 000h, 000h
        je     SHORT loc_4_10C7
        inc    ebx
        cmp    ebx, 8
        jge    SHORT loc_4_1106
        jmp    SHORT loc_4_10B6
loc_4_10C7:
        db  0C7h, 004h, 0DDh, 010h, 008h, 000h, 000h, 0FFh, 0FFh, 000h, 000h
        db  0C7h, 004h, 0DDh, 014h, 008h, 000h, 000h, 0FFh, 0FFh, 000h, 000h
        db  0FEh, 00Dh, 050h, 008h, 000h, 000h
loc_4_10E3:
        db  081h, 024h, 0DDh, 010h, 008h, 000h, 000h, 0FFh, 0FFh, 000h, 000h
        db  081h, 024h, 0DDh, 014h, 008h, 000h, 000h, 0FFh, 0FFh, 000h, 000h
        db  081h, 00Ch, 0DDh, 014h, 008h, 000h, 000h, 000h, 000h, 008h, 000h
        clc
        ret
loc_4_1106:
        stc
        ret
loc_4_1108:
        mov    ebx, 0
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 18h]
        mov    edx, dword ptr [ebp + 14h]
        call   sub_4_107D
        jb     SHORT loc_4_1123
        and    word ptr [ebp + 2ch], 0fffeh
        ret
loc_4_1123:
        or     word ptr [ebp + 2ch], 1
        ret
        db  02Eh, 08Bh, 0C0h
sub_4_112C:
        test   bl, 4
        je     SHORT loc_4_1153
        test   ebx, 8000h
        je     SHORT loc_4_1147
        movzx  eax, word ptr [edi + 4]
        db  0A3h, 060h, 002h, 000h, 000h
        jmp    NEAR PTR loc_4_12A9
loc_4_1147:
        db  0A1h, 060h, 002h, 000h, 000h
        mov    dword ptr [edi], eax
        jmp    NEAR PTR loc_4_12A9
loc_4_1153:
        test   ebx, 20000h
        je     SHORT loc_4_117E
        test   ebx, 8000h
        je     SHORT loc_4_1172
        xor    eax, eax
        mov    al, byte ptr [edi + 4]
        db  0A3h, 079h, 003h, 000h, 000h
        jmp    NEAR PTR loc_4_12A9
loc_4_1172:
        db  0A1h, 079h, 003h, 000h, 000h
        mov    dword ptr [edi], eax
        jmp    NEAR PTR loc_4_12A9
loc_4_117E:
        test   ebx, 40000h
        je     SHORT loc_4_11A9
        test   ebx, 8000h
        je     SHORT loc_4_119D
        xor    eax, eax
        mov    al, byte ptr [edi + 4]
        db  0A3h, 07Dh, 003h, 000h, 000h
        jmp    NEAR PTR loc_4_12A9
loc_4_119D:
        db  0A1h, 07Dh, 003h, 000h, 000h
        mov    dword ptr [edi], eax
        jmp    NEAR PTR loc_4_12A9
loc_4_11A9:
        db  0C6h, 005h, 051h, 008h, 000h, 000h, 0FFh
        call   sub_4_12BB
        jb     NEAR PTR loc_4_12B2
        or     edi, edi
        jne    SHORT loc_4_11BF
loc_4_11BF:
        push   ecx
        push   esi
        push   edi
        cmp    dword ptr [edi + 20h], 4
        jne    SHORT loc_4_1206
        mov    eax, dword ptr [edi + 28h]
        db  081h, 0FEh, 096h, 002h, 000h, 000h
        je     SHORT loc_4_11DE
        db  081h, 0FFh, 096h, 002h, 000h, 000h
        jne    SHORT loc_4_1206
        mov    eax, dword ptr [esi + 28h]
loc_4_11DE:
        or     eax, eax
        je     SHORT loc_4_1206
        db  0A3h, 0BEh, 002h, 000h, 000h
        mov    esi, eax
        db  0BFh, 004h, 005h, 000h, 000h
        mov    ecx, 103h
        cld
        test   ebx, 8000h
        jne    SHORT loc_4_11FE
        xchg   edi, esi
loc_4_11FE:
        lodsb
        stosb
        or     al, al
        je     SHORT loc_4_1206
        loop   loc_4_11FE
loc_4_1206:
        pop    edi
        pop    esi
        pop    ecx
        push   ecx
        push   esi
        push   edi
        mov    eax, dword ptr [edi + 8]
        db  081h, 0FEh, 07Eh, 002h, 000h, 000h
        je     SHORT loc_4_1222
        db  081h, 0FFh, 07Eh, 002h, 000h, 000h
        jne    SHORT loc_4_124B
        mov    eax, dword ptr [esi + 8]
loc_4_1222:
        or     eax, eax
        je     SHORT loc_4_124B
        mov    esi, eax
        db  0BFh, 008h, 006h, 000h, 000h
        db  089h, 03Dh, 086h, 002h, 000h, 000h
        mov    ecx, 103h
        cld
        test   ebx, 8000h
        jne    SHORT loc_4_1243
        xchg   edi, esi
loc_4_1243:
        lodsb
        stosb
        or     al, al
        je     SHORT loc_4_124B
        loop   loc_4_1243
loc_4_124B:
        pop    edi
        pop    esi
        pop    ecx
        push   ecx
        push   esi
        push   edi
        mov    eax, dword ptr [edi + 0ch]
        db  081h, 0FEh, 07Eh, 002h, 000h, 000h
        je     SHORT loc_4_1267
        db  081h, 0FFh, 07Eh, 002h, 000h, 000h
        jne    SHORT loc_4_1290
        mov    eax, dword ptr [esi + 0ch]
loc_4_1267:
        or     eax, eax
        je     SHORT loc_4_1290
        mov    esi, eax
        db  0BFh, 00Ch, 007h, 000h, 000h
        db  089h, 03Dh, 08Ah, 002h, 000h, 000h
        mov    ecx, 103h
        cld
        test   ebx, 8000h
        jne    SHORT loc_4_1288
        xchg   edi, esi
loc_4_1288:
        lodsb
        stosb
        or     al, al
        je     SHORT loc_4_1290
        loop   loc_4_1288
loc_4_1290:
        pop    edi
        pop    esi
        pop    ecx
        cld
        push   esi
        push   edi
        rep movsb
        pop    edi
        pop    esi
        test   ebx, 8000h
        je     SHORT loc_4_12A9
        xchg   edi, esi
        call   sub_4_03A0
loc_4_12A9:
        clc
        db  0C6h, 005h, 051h, 008h, 000h, 000h, 000h
        ret
loc_4_12B2:
        stc
        db  0C6h, 005h, 051h, 008h, 000h, 000h, 000h
        ret
sub_4_12BB:
        bsf    eax, ebx
        je     SHORT loc_4_12E9
        cmp    eax, 0dh
        jae    SHORT loc_4_12E9
        db  08Bh, 034h, 0C5h, 054h, 008h, 000h, 000h
        db  08Bh, 00Ch, 0C5h, 058h, 008h, 000h, 000h
        cmp    esi, 0
        je     SHORT loc_4_12E9
        cmp    ecx, 0
        je     SHORT loc_4_12E9
        test   ebx, 8000h
        je     SHORT loc_4_12E8
        xchg   edi, esi
        ret
loc_4_12E8:
        ret
loc_4_12E9:
        stc
        ret
loc_4_12EB:
        cmp    ecx, 0
        je     SHORT loc_4_12F5
        cmp    ecx, 0
        jne    SHORT loc_4_12FF
loc_4_12F5:
        mov    ecx, 400h
        jmp    NEAR PTR loc_4_13B7
loc_4_12FF:
        mov    edi, dword ptr [esi + 18h]
        mov    esi, dword ptr [esi + 10h]
        cmp    ecx, 100h
        jne    SHORT loc_4_1318
        mov    dword ptr [edi], 400h
        jmp    NEAR PTR loc_4_13B7
loc_4_1318:
        cmp    ecx, 102h
        jne    SHORT loc_4_132C
        db  0A1h, 0CCh, 003h, 000h, 000h
        mov    dword ptr [edi], eax
        jmp    NEAR PTR loc_4_13B7
loc_4_132C:
        cmp    ecx, 103h
        jne    SHORT loc_4_1363
        xor    eax, eax
        db  0F7h, 005h, 0CCh, 003h, 000h, 000h, 000h, 040h, 000h, 000h
        je     SHORT loc_4_1355
        db  0F7h, 005h, 0CCh, 003h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_4_1355
        db  0A1h, 0E2h, 002h, 000h, 000h
        mov    eax, dword ptr [eax]
loc_4_1355:
        mov    dword ptr [edi], eax
        add    edi, 4
        db  0A1h, 0CCh, 003h, 000h, 000h
        mov    dword ptr [edi], eax
        jmp    SHORT loc_4_13B7
loc_4_1363:
        cmp    ecx, 104h
        jne    SHORT loc_4_137F
        mov    eax, dword ptr [esi]
        mov    ebx, 0
        mov    ecx, dword ptr [esi + 4]
        mov    edx, dword ptr [esi + 8]
        call   sub_4_107D
        jmp    SHORT loc_4_13B7
loc_4_137F:
        cmp    ecx, 101h
        jne    SHORT loc_4_13B7
        bsf    eax, ebx
        je     SHORT loc_4_13B7
        cmp    eax, 0dh
        jae    SHORT loc_4_13B7
        test   ebx, 8000h
        je     SHORT loc_4_13A2
        db  08Bh, 03Ch, 0C5h, 054h, 008h, 000h, 000h
        jmp    SHORT loc_4_13A9
loc_4_13A2:
        db  08Bh, 034h, 0C5h, 054h, 008h, 000h, 000h
loc_4_13A9:
        db  08Bh, 00Ch, 0C5h, 058h, 008h, 000h, 000h
        rep movsb
        call   sub_4_03A0
loc_4_13B7:
        xor    eax, eax
        ret
        db  000h, 000h
sub_4_13BC:
        push   eax
        push   esi
        push   edi
        cld
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 14h]
loc_4_13C8:
        lodsb
        stosb
        or     al, al
        jne    SHORT loc_4_13C8
        pop    edi
        pop    esi
        pop    eax
        ret
sub_4_13D2:
        push   eax
        push   edx
        push   esi
        push   edi
        cld
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 18h]
        mov    eax, dword ptr [esi]
        mov    esi, 0ah
        cmp    eax, 0
        jge    SHORT loc_4_13F1
        mov    byte ptr [edi], 2dh
        inc    edi
        neg    eax
loc_4_13F1:
        push   0
loc_4_13F3:
        xor    edx, edx
        div    esi
        db  08Ah, 092h, 0B8h, 00Dh, 000h, 000h
        push   edx
        or     eax, eax
        jne    SHORT loc_4_13F3
loc_4_1402:
        pop    eax
        stosb
        or     eax, eax
        jne    SHORT loc_4_1402
        pop    edi
        pop    esi
        pop    edx
        pop    eax
        ret
        db  083h, 0E0h, 00Fh, 08Ah, 080h, 0B8h, 00Dh, 000h, 000h, 088h, 007h, 047h, 0C3h
loc_4_141A:
        db  080h, 025h, 0C7h, 011h, 000h, 000h, 000h
        ret
sub_4_1422:
        push   ebx
        push   ecx
        xor    eax, eax
        xor    ebx, ebx
        mov    cl, byte ptr [edx]
        cmp    cl, 2dh
        jne    SHORT loc_4_1430
        inc    edx
loc_4_1430:
        mov    al, byte ptr [edx]
        inc    edx
        or     al, al
        je     SHORT loc_4_1446
        sub    al, 30h
        jb     SHORT loc_4_1446
        cmp    al, 0ah
        jae    SHORT loc_4_1446
        imul   ebx, ebx, 0ah
        add    ebx, eax
        jmp    SHORT loc_4_1430
loc_4_1446:
        cmp    cl, 2dh
        jne    SHORT loc_4_144D
        neg    ebx
loc_4_144D:
        mov    eax, ebx
        pop    ecx
        pop    ebx
        ret
sub_4_1452:
        db  083h, 025h, 0C8h, 008h, 000h, 000h, 000h
        pushad
        xor    ebx, ebx
loc_4_145C:
        db  08Bh, 014h, 0DDh, 004h, 00Dh, 000h, 000h
        or     edx, edx
        je     NEAR PTR loc_4_1615
        push   ecx
        push   edx
        db  068h, 0C4h, 008h, 000h, 000h
        push   edx
        push   80000001h
        db  0CDh, 020h, 04Ah, 001h, 001h, 000h
        add    esp, 0ch
        pop    edx
        pop    ecx
        test   eax, eax
        jne    NEAR PTR loc_4_15D6
        xor    ecx, ecx
        db  08Bh, 014h, 0DDh, 008h, 00Dh, 000h, 000h
        or     edx, edx
        je     NEAR PTR loc_4_15C9
loc_4_149C:
        mov    esi, dword ptr [edx + ecx*4]
        or     esi, esi
        je     NEAR PTR loc_4_15C9
        db  0C7h, 005h, 0D0h, 008h, 000h, 000h, 010h, 000h, 000h, 000h
        push   ecx
        push   edx
        db  081h, 0FEh, 0C9h, 010h, 000h, 000h
        jne    SHORT loc_4_14E8
        db  0C7h, 005h, 0CCh, 008h, 000h, 000h, 004h, 001h, 000h, 000h
        db  068h, 0CCh, 008h, 000h, 000h
        db  068h, 004h, 005h, 000h, 000h
        db  068h, 0D4h, 008h, 000h, 000h
        push   0
        push   esi
        db  0FFh, 035h, 0C4h, 008h, 000h, 000h
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        jmp    SHORT loc_4_153E
loc_4_14E8:
        db  081h, 0FEh, 080h, 011h, 000h, 000h
        jne    SHORT loc_4_151D
        db  0C7h, 005h, 0CCh, 008h, 000h, 000h, 004h, 001h, 000h, 000h
        db  068h, 0CCh, 008h, 000h, 000h
        db  068h, 008h, 006h, 000h, 000h
        db  068h, 0D4h, 008h, 000h, 000h
        push   0
        push   esi
        db  0FFh, 035h, 0C4h, 008h, 000h, 000h
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        jmp    SHORT loc_4_153E
loc_4_151D:
        db  068h, 0D0h, 008h, 000h, 000h
        db  068h, 0D8h, 008h, 000h, 000h
        db  068h, 0D4h, 008h, 000h, 000h
        push   0
        push   esi
        db  0FFh, 035h, 0C4h, 008h, 000h, 000h
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
loc_4_153E:
        pop    edx
        pop    ecx
        test   eax, eax
        je     SHORT loc_4_1578
        db  068h, 0D8h, 008h, 000h, 000h
        push   dword ptr [edx + ecx*4 + 4]
        call   sub_4_13BC
        add    esp, 8
        push   ebx
        push   ecx
        push   edx
        push   10h
        db  068h, 0D8h, 008h, 000h, 000h
        push   1
        push   0
        push   dword ptr [edx + ecx*4]
        db  0FFh, 035h, 0C4h, 008h, 000h, 000h
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        pop    edx
        pop    ecx
        pop    ebx
loc_4_1578:
        db  081h, 0FEh, 0C9h, 010h, 000h, 000h
        je     SHORT loc_4_15C0
        db  081h, 0FEh, 080h, 011h, 000h, 000h
        je     SHORT loc_4_15C0
        push   eax
        push   edx
        db  0BAh, 0D8h, 008h, 000h, 000h
        call   sub_4_1422
        pop    edx
        push   edx
        cmp    dword ptr [edx + ecx*4 + 0ch], 80000000h
        je     SHORT loc_4_15B8
        ja     SHORT loc_4_15BE
        or     eax, eax
        mov    eax, dword ptr [edx + ecx*4 + 0ch]
        mov    edx, dword ptr [edx + ecx*4 + 8]
        jne    SHORT loc_4_15B4
        not    eax
        and    dword ptr [edx], eax
        jmp    SHORT loc_4_15BE
loc_4_15B4:
        or     dword ptr [edx], eax
        jmp    SHORT loc_4_15BE
loc_4_15B8:
        mov    edx, dword ptr [edx + ecx*4 + 8]
        mov    dword ptr [edx], eax
loc_4_15BE:
        pop    edx
        pop    eax
loc_4_15C0:
        inc    ecx
        inc    ecx
        inc    ecx
        inc    ecx
        jmp    NEAR PTR loc_4_149C
loc_4_15C9:
        db  0FFh, 035h, 0C4h, 008h, 000h, 000h
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        pop    ecx
loc_4_15D6:
        pushad
        db  08Bh, 034h, 0DDh, 00Ch, 00Dh, 000h, 000h
        db  08Bh, 01Ch, 0DDh, 010h, 00Dh, 000h, 000h
        test   ebx, 100h
        je     SHORT loc_4_1601
        test   byte ptr [esi + 4], 1
        je     SHORT loc_4_1601
        db  0C7h, 005h, 0BCh, 008h, 000h, 000h, 000h, 001h, 000h, 000h
        and    byte ptr [esi + 4], 0feh
loc_4_1601:
        call   sub_4_03A0
        popad
        db  0C6h, 005h, 0C7h, 011h, 000h, 000h, 001h
        inc    ebx
        inc    ebx
        jmp    NEAR PTR loc_4_145C
loc_4_1615:
        db  081h, 03Dh, 0BCh, 008h, 000h, 000h, 000h, 001h, 000h, 000h
        jne    SHORT loc_4_1640
        db  08Bh, 015h, 0BCh, 008h, 000h, 000h
        or     edx, 8000h
        push   0
        push   edx
        db  068h, 054h, 016h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
        jmp    SHORT loc_4_1652
loc_4_1640:
        mov    eax, 7d0h
        xor    edx, edx
        db  0BEh, 01Ah, 014h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
loc_4_1652:
        popad
        ret
loc_4_1654:
        mov    ecx, dword ptr [esp + 4]
        db  066h, 089h, 00Dh, 06Dh, 004h, 000h, 000h
        pushad
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0B8h, 084h, 003h, 000h, 000h
        db  068h, 06Dh, 004h, 000h, 000h
        push   2
        push   eax
        db  068h, 090h, 001h, 000h, 000h
        db  0CDh, 020h, 018h, 000h, 017h, 000h
        add    esp, 10h
        cmp    eax, 2
        je     SHORT loc_4_16A5
        db  080h, 00Dh, 024h, 002h, 000h, 000h, 001h
        db  0BEh, 020h, 002h, 000h, 000h
        mov    ebx, 100h
        call   sub_4_03A0
loc_4_16A5:
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        popad
        ret
sub_4_16B6:
        pushad
        db  089h, 01Dh, 0C0h, 008h, 000h, 000h
        xor    ebx, ebx
loc_4_16BF:
        db  08Bh, 014h, 0DDh, 004h, 00Dh, 000h, 000h
        or     edx, edx
        je     NEAR PTR loc_4_17F7
        db  08Bh, 014h, 0DDh, 010h, 00Dh, 000h, 000h
        db  023h, 015h, 0C0h, 008h, 000h, 000h
        je     NEAR PTR loc_4_17F0
        db  08Bh, 014h, 0DDh, 004h, 00Dh, 000h, 000h
        push   ebx
        push   ecx
        push   edx
        db  068h, 0C4h, 008h, 000h, 000h
        push   edx
        push   80000001h
        db  0CDh, 020h, 04Ah, 001h, 001h, 000h
        add    esp, 0ch
        pop    edx
        pop    ecx
        pop    ebx
        cmp    eax, 0
        jne    NEAR PTR loc_4_17F0
        xor    ecx, ecx
        db  08Bh, 014h, 0DDh, 008h, 00Dh, 000h, 000h
        or     edx, edx
        je     NEAR PTR loc_4_17E1
loc_4_171C:
        mov    esi, dword ptr [edx + ecx*4]
        or     esi, esi
        je     NEAR PTR loc_4_17E1
        cmp    dword ptr [edx + ecx*4 + 0ch], 80000000h
        jae    SHORT loc_4_1756
        db  0C6h, 005h, 0D8h, 008h, 000h, 000h, 030h
        db  0C6h, 005h, 0D9h, 008h, 000h, 000h, 000h
        mov    eax, dword ptr [edx + ecx*4 + 0ch]
        push   edx
        mov    edx, dword ptr [edx + ecx*4 + 8]
        test   dword ptr [edx], eax
        pop    edx
        je     SHORT loc_4_1767
        db  0C6h, 005h, 0D8h, 008h, 000h, 000h, 031h
        jmp    SHORT loc_4_1767
loc_4_1756:
        db  068h, 0D8h, 008h, 000h, 000h
        push   dword ptr [edx + ecx*4 + 8]
        call   sub_4_13D2
        add    esp, 8
loc_4_1767:
        push   ebx
        push   ecx
        push   edx
        db  081h, 0FEh, 080h, 011h, 000h, 000h
        jne    SHORT loc_4_1792
        push   104h
        db  068h, 008h, 006h, 000h, 000h
        push   1
        push   0
        push   esi
        db  0FFh, 035h, 0C4h, 008h, 000h, 000h
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        jmp    SHORT loc_4_17D5
loc_4_1792:
        db  081h, 0FEh, 0C9h, 010h, 000h, 000h
        jne    SHORT loc_4_17BA
        push   104h
        db  068h, 004h, 005h, 000h, 000h
        push   1
        push   0
        push   esi
        db  0FFh, 035h, 0C4h, 008h, 000h, 000h
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        jmp    SHORT loc_4_17D5
loc_4_17BA:
        push   10h
        db  068h, 0D8h, 008h, 000h, 000h
        push   1
        push   0
        push   esi
        db  0FFh, 035h, 0C4h, 008h, 000h, 000h
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
loc_4_17D5:
        pop    edx
        pop    ecx
        pop    ebx
        inc    ecx
        inc    ecx
        inc    ecx
        inc    ecx
        jmp    NEAR PTR loc_4_171C
loc_4_17E1:
        push   ebx
        db  0FFh, 035h, 0C4h, 008h, 000h, 000h
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        pop    ecx
        pop    ebx
loc_4_17F0:
        inc    ebx
        inc    ebx
        jmp    NEAR PTR loc_4_16BF
loc_4_17F7:
        popad
        ret
        db  000h, 000h, 000h
sub_4_17FC:
        mov    eax, dword ptr [esp + 8]
        cmp    eax, 1
        mov    eax, 1
        jne    SHORT loc_4_1826
        mov    eax, dword ptr [esp + 4]
        db  068h, 008h, 006h, 000h, 000h
        push   eax
        db  0E8h, 007h, 0E8h, 0FBh, 0FFh
        add    esp, 8
        call   sub_4_0D23
        call   sub_4_0D02
loc_4_1826:
        ret    8
        db  0C3h, 000h, 000h
_PTEXT ends
        end
