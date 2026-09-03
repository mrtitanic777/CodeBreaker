; vpicd_lcode_01.asm -- VPICD object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

VPICD_Force_Default_Owner:
        pushad
        pushfd
        test   ebx, ebx
        je     SHORT loc_1_0014
        cmp    ebx, -1
        je     SHORT loc_1_0014
        cmp    dword ptr [ebx + 10h], 62634d56h
        jne    SHORT loc_1_0076
loc_1_0014:
        mov    ecx, eax
        movzx  eax, ax
        cmp    eax, 0fh
        ja     SHORT loc_1_0076
        cli
        db  08Bh, 014h, 085h, 068h, 025h, 000h, 000h
        mov    di, 0ff3fh
        and    word ptr [edx + 6], di
        shr    ecx, 0ah
        not    edi
        and    ecx, edi
        or     word ptr [edx + 6], cx
        test   byte ptr [edx + 6], 4
        jne    SHORT loc_1_0076
        mov    ecx, ebx
        xchg   dword ptr [edx + 8], ecx
        cmp    ebx, ecx
        je     SHORT loc_1_0072
        jecxz  loc_1_0072
        cmp    ecx, -1
        je     SHORT loc_1_0072
        test   ebx, ebx
        je     SHORT loc_1_0072
        mov    ebx, ecx
        mov    eax, dword ptr [edx + 18h]
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   cl, 20h
        je     SHORT loc_1_0072
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        mov    ebx, dword ptr [edx + 8]
        db  0CDh, 020h, 002h, 000h, 003h, 000h
loc_1_0072:
        popfd
        popad
        clc
        ret
loc_1_0076:
        popfd
        popad
        stc
        ret
VPICD_Force_Default_Behavior:
        pushad
        pushfd
        cmp    dword ptr [eax + 28h], 51524953h
        jne    NEAR PTR loc_1_016E
        cli
        mov    edi, dword ptr [eax]
        call   sub_1_1D1B
        test   byte ptr [edi + 6], 2
        je     NEAR PTR loc_1_011D
        lea    ecx, [edi + 10h]
loc_1_009E:
        mov    edx, dword ptr [ecx + 8]
        cmp    edx, eax
        je     SHORT loc_1_00A9
        mov    ecx, edx
        jmp    SHORT loc_1_009E
loc_1_00A9:
        mov    edx, dword ptr [edx + 8]
        mov    dword ptr [ecx + 8], edx
        inc    dword ptr [eax + 28h]
        push   eax
        push   ecx
        xor    ecx, ecx
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax]
        bts    ecx, eax
        db  066h, 085h, 00Dh, 00Eh, 027h, 000h, 000h
        jne    SHORT loc_1_0106
        db  066h, 085h, 00Dh, 0F4h, 025h, 000h, 000h
        je     SHORT loc_1_0106
        test   word ptr [edi + 6], 100h
        je     SHORT loc_1_0106
        db  0FEh, 088h, 0E0h, 026h, 000h, 000h
        db  080h, 0B8h, 0E0h, 026h, 000h, 000h, 0FFh
        jne    SHORT loc_1_0106
        push   edi
        mov    ecx, eax
        db  08Bh, 03Ch, 08Dh, 0F4h, 025h, 000h, 000h
        cmp    byte ptr [edi], 0e9h
        jne    SHORT loc_1_0105
        mov    eax, dword ptr [edi + 1]
        db  087h, 004h, 08Dh, 034h, 026h, 000h, 000h
        mov    dword ptr [edi + 1], eax
        pop    edi
        jmp    SHORT loc_1_0106
loc_1_0105:
        pop    edi
loc_1_0106:
        pop    ecx
        pop    eax
        db  08Bh, 035h, 040h, 01Eh, 000h, 000h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        mov    eax, dword ptr [edi + 18h]
        cmp    dword ptr [eax + 4], 1
        jne    SHORT loc_1_016E
loc_1_011D:
        xor    ecx, ecx
        db  0C7h, 040h, 00Ch, 0E0h, 002h, 000h, 000h
        mov    dword ptr [eax + 10h], ecx
        db  0C7h, 040h, 018h, 04Ch, 003h, 000h, 000h
        db  0C7h, 040h, 014h, 02Ch, 003h, 000h, 000h
        db  0C7h, 040h, 01Ch, 054h, 004h, 000h, 000h
        mov    dword ptr [edi + 1ch], 1f4h
        mov    word ptr [edi + 6], 8
        db  081h, 0FFh, 0A8h, 020h, 000h, 000h
        jne    SHORT loc_1_016E
        db  0C7h, 040h, 014h, 08Dh, 002h, 000h, 000h
        db  039h, 00Dh, 06Ch, 01Dh, 000h, 000h
        je     SHORT loc_1_016E
        cmp    dword ptr [edi + 8], ecx
        jne    SHORT loc_1_016E
        db  0C7h, 040h, 00Ch, 09Bh, 001h, 000h, 000h
loc_1_016E:
        popfd
        popad
        ret
VPICD_Get_Virtualization_Count:
        pushfd
        push   edx
        cmp    eax, 0fh
        ja     SHORT loc_1_0196
        db  08Bh, 014h, 085h, 068h, 025h, 000h, 000h
        test   byte ptr [edx + 6], 4
        je     SHORT loc_1_0196
        mov    edx, dword ptr [edx + 18h]
        xor    al, al
loc_1_018A:
        cmp    dword ptr [edx + 4], 1
        je     SHORT loc_1_0198
        inc    eax
        mov    edx, dword ptr [edx + 8]
        jmp    SHORT loc_1_018A
loc_1_0196:
        xor    eax, eax
loc_1_0198:
        pop    edx
        popfd
        ret
loc_1_019B:
        db  0BEh, 0A9h, 001h, 000h, 000h
        mov    edx, eax
        sti
        db  0CDh, 020h, 010h, 080h, 001h, 000h
loc_1_01A9:
        mov    eax, dword ptr [edx + 0ch]
        db  03Dh, 09Bh, 001h, 000h, 000h
        jne    SHORT loc_1_0212
        xor    edi, edi
loc_1_01B5:
        db  08Bh, 00Dh, 0A8h, 025h, 000h, 000h
        test   byte ptr [ecx + ebx + 0ah], 0ch
        jne    SHORT loc_1_01E3
        test   byte ptr [ebx], 8
        jne    SHORT loc_1_01E9
        mov    ecx, dword ptr [ebx + 4]
        mov    eax, dword ptr [ecx + 28h]
        db  03Bh, 005h, 058h, 01Dh, 000h, 000h
        jne    SHORT loc_1_01E3
        mov    eax, dword ptr [ecx + 1c4h]
        db  03Bh, 005h, 05Ch, 01Dh, 000h, 000h
        je     SHORT loc_1_01E9
loc_1_01E3:
        test   edi, edi
        jne    SHORT loc_1_0224
        mov    edi, ebx
loc_1_01E9:
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        db  0CDh, 020h, 002h, 000h, 001h, 000h
        jne    SHORT loc_1_01B5
loc_1_01F7:
        mov    eax, edx
        pushfd
        cli
        db  0B9h, 0E0h, 002h, 000h, 000h
        mov    dword ptr [eax + 0ch], ecx
        mov    edx, dword ptr [eax]
        mov    dword ptr [edx + 8], edi
        db  0CDh, 020h, 001h, 000h, 001h, 000h
loc_1_020E:
        call   ecx
        popfd
loc_1_0211:
        ret
loc_1_0212:
        xchg   edx, eax
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   cl, 8
        je     SHORT loc_1_0211
        mov    ecx, edx
        pushfd
        cli
        jmp    SHORT loc_1_020E
loc_1_0224:
        xor    edi, edi
        jmp    SHORT loc_1_01F7
        db  0EBh, 006h, 0FFh, 025h, 06Ch, 01Dh, 000h, 000h
loc_1_0230:
        pushad
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0CDh, 020h, 00Eh, 000h, 003h, 000h
        jb     SHORT loc_1_0286
        cmp    al, 2
        je     SHORT loc_1_025A
        cmp    al, 9
        jne    SHORT loc_1_0286
        db  066h, 08Bh, 035h, 06Ah, 01Dh, 000h, 000h
        db  08Bh, 03Dh, 06Ah, 01Dh, 000h, 000h
        mov    ax, 8
        jmp    SHORT loc_1_026B
loc_1_025A:
        db  066h, 08Bh, 035h, 064h, 01Dh, 000h, 000h
        db  08Bh, 03Dh, 064h, 01Dh, 000h, 000h
        mov    ax, 4
loc_1_026B:
        db  003h, 01Dh, 0A8h, 025h, 000h, 000h
        cmp    cx, si
        jne    SHORT loc_1_0282
        cmp    edi, edi
        jne    SHORT loc_1_0282
        not    eax
        and    word ptr [ebx + 0ah], ax
        jmp    SHORT loc_1_0286
loc_1_0282:
        or     word ptr [ebx + 0ah], ax
loc_1_0286:
        popad
        db  0FFh, 025h, 06Ch, 01Dh, 000h, 000h
loc_1_028D:
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  0B8h, 0C0h, 022h, 000h, 000h
        db  0CDh, 020h, 006h, 000h, 003h, 000h
        test   cl, 2
        jne    SHORT loc_1_02AE
        db  0B8h, 0F4h, 023h, 000h, 000h
        db  0CDh, 020h, 004h, 080h, 003h, 000h
loc_1_02AE:
        db  003h, 01Dh, 0A8h, 025h, 000h, 000h
        or     byte ptr [ebx + 0ah], 2
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_02BC:
        db  0F6h, 005h, 0AEh, 020h, 000h, 000h, 004h
        jne    SHORT loc_1_02DE
        db  003h, 01Dh, 0A8h, 025h, 000h, 000h
        btr    word ptr [ebx + 0ah], 1
        jae    SHORT loc_1_02DE
        db  0B8h, 0F4h, 023h, 000h, 000h
        db  0CDh, 020h, 004h, 080h, 003h, 000h
loc_1_02DE:
        ret
        db  090h
loc_1_02E0:
        mov    edx, dword ptr [eax]
        mov    ebx, dword ptr [edx + 8]
        test   ebx, ebx
        je     SHORT loc_1_02F4
        cmp    ebx, -1
        je     SHORT loc_1_031D
        db  0CDh, 020h, 002h, 080h, 003h, 000h
loc_1_02F4:
        or     ebx, dword ptr [edx + 0ch]
        jne    SHORT loc_1_0308
        db  0CDh, 020h, 0F1h, 000h, 001h, 000h
        test   byte ptr [edx + 6], 80h
        jne    SHORT loc_1_0311
loc_1_0305:
        mov    dword ptr [edx + 0ch], ebx
loc_1_0308:
        mov    dword ptr [edx + 10h], ebx
        db  0CDh, 020h, 002h, 080h, 003h, 000h
loc_1_0311:
        test   ecx, ecx
        jne    SHORT loc_1_0305
        db  0CDh, 020h, 030h, 000h, 001h, 000h
        jmp    SHORT loc_1_0305
loc_1_031D:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    dword ptr [edx + 0ch], ebx
        db  0CDh, 020h, 002h, 080h, 003h, 000h
loc_1_032C:
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 080h, 003h, 000h
loc_1_0338:
        mov    edx, dword ptr [eax]
        xor    ebx, ebx
        xchg   dword ptr [edx + 10h], ebx
        test   ebx, ebx
        jne    SHORT loc_1_032C
loc_1_0343:
        db  0CDh, 020h, 004h, 080h, 003h, 000h
        mov    eax, eax
sub_1_034C:
        mov    edi, dword ptr [eax]
        mov    edx, dword ptr [edi + 8]
        jecxz  loc_1_0387
        cmp    edx, ebx
        jne    SHORT loc_1_037D
        mov    dword ptr [edi + 8], 0ffffffffh
        db  03Dh, 0F4h, 023h, 000h, 000h
        jne    SHORT loc_1_0381
        db  083h, 03Dh, 06Ch, 01Dh, 000h, 000h, 000h
        je     SHORT loc_1_0381
        mov    dword ptr [edi + 8], 0
        db  0C7h, 040h, 00Ch, 09Bh, 001h, 000h, 000h
loc_1_037C:
        ret
loc_1_037D:
        test   edx, edx
        jne    SHORT loc_1_037C
loc_1_0381:
        db  0CDh, 020h, 008h, 080h, 003h, 000h
loc_1_0387:
        test   edx, edx
        je     SHORT loc_1_03CB
        cmp    edx, -1
        jne    SHORT loc_1_03D1
        mov    dword ptr [edi + 8], ebx
        test   byte ptr [edi + 6], 4
        jne    SHORT loc_1_03CB
        mov    edx, dword ptr [edi]
        db  00Fh, 0A3h, 015h, 032h, 01Eh, 000h, 000h
        jae    SHORT loc_1_03CB
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        push   ebx
loc_1_03AB:
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_1_03AB
        pop    ebx
        db  0CDh, 020h, 007h, 000h, 003h, 000h
        jae    SHORT loc_1_03CB
        db  0CDh, 020h, 002h, 000h, 003h, 000h
loc_1_03CB:
        db  0CDh, 020h, 009h, 080h, 003h, 000h
loc_1_03D1:
        cmp    ebx, edx
        je     SHORT loc_1_03CB
        test   byte ptr [edi + 6], 40h
        jne    SHORT loc_1_0440
        db  08Bh, 03Dh, 0A8h, 025h, 000h, 000h
        bts    word ptr [edi + ebx + 0ah], 4
        jb     SHORT loc_1_0440
        sti
        xor    edi, edi
        mov    ecx, 80001030h
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_1_0410
        push   80001h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        xchg   ecx, eax
        or     eax, 4
        jmp    SHORT loc_1_0421
loc_1_0410:
        push   80002h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        xchg   ecx, eax
loc_1_0421:
        push   ecx
        xor    ecx, ecx
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        pop    ecx
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        add    esp, dword ptr [esp]
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        cmp    eax, 7
        je     SHORT loc_1_0441
        cli
loc_1_0440:
        ret
loc_1_0441:
        mov    eax, 0bb8h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        xor    ecx, ecx
        db  0CDh, 020h, 0ECh, 000h, 001h, 000h
loc_1_0454:
        mov    eax, dword ptr [eax]
        mov    dword ptr [eax + 0ch], 0
        ret
loc_1_045E:
        push   eax
        mov    eax, dword ptr [eax]
        mov    dword ptr [eax + 0ch], 0
        db  08Bh, 035h, 0A8h, 025h, 000h, 000h
        mov    eax, dword ptr [eax]
        btr    word ptr [esi + ebx + 12h], ax
        jb     SHORT loc_1_047A
        pop    eax
        ret
loc_1_047A:
        cmp    eax, 8
        jb     SHORT loc_1_0485
        and    word ptr [esi + ebx + 12h], 0fffbh
loc_1_0485:
        pop    eax
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 080h, 003h, 000h
loc_1_0492:
        stc
        db  0CDh, 020h, 004h, 080h, 003h, 000h
        add    byte ptr [eax], al
        db  000h
VPICD_Control:
        cmp    eax, 7
        je     NEAR PTR loc_1_054A
        cmp    eax, 0bh
        je     NEAR PTR loc_1_058A
        cmp    eax, 20h
        je     NEAR PTR loc_1_0629
        cmp    eax, 0
        db  00Fh, 084h, 040h, 0FBh, 0FEh, 0FFh
        cmp    eax, 15h
        db  00Fh, 084h, 037h, 0FBh, 0FEh, 0FFh
        cmp    eax, 6
        je     SHORT loc_1_04F7
        db  0F7h, 005h, 0D4h, 004h, 000h, 000h, 000h, 000h, 040h, 000h
        je     SHORT loc_1_04E3
        cmp    eax, 14h
        db  00Fh, 084h, 00Eh, 0FCh, 0FEh, 0FFh
loc_1_04E3:
        cmp    eax, 2
        db  00Fh, 084h, 05Eh, 0FFh, 0FEh, 0FFh
        cmp    eax, 4
        db  00Fh, 084h, 00Bh, 0FBh, 0FEh, 0FFh
        clc
        ret
loc_1_04F7:
        pushfd
        cli
        mov    al, 11h
        out    20h, al
        jmp    SHORT loc_1_04FF
loc_1_04FF:
        mov    al, 8
        out    21h, al
        jmp    SHORT loc_1_0505
loc_1_0505:
        mov    al, 4
        out    21h, al
        jmp    SHORT loc_1_050B
loc_1_050B:
        mov    al, 1
        out    21h, al
        jmp    SHORT loc_1_0511
loc_1_0511:
        mov    al, 4ah
        out    20h, al
        jmp    SHORT loc_1_0517
loc_1_0517:
        mov    al, 11h
        out    0a0h, al
        jmp    SHORT loc_1_051D
loc_1_051D:
        mov    al, 70h
        out    0a1h, al
        jmp    SHORT loc_1_0523
loc_1_0523:
        mov    al, 2
        out    0a1h, al
        jmp    SHORT loc_1_0529
loc_1_0529:
        mov    al, 1
        out    0a1h, al
        jmp    SHORT loc_1_052F
loc_1_052F:
        mov    al, 4ah
        out    0a0h, al
        db  066h, 0A1h, 036h, 01Eh, 000h, 000h
        db  066h, 0A3h, 032h, 01Eh, 000h, 000h
        out    21h, al
        jmp    SHORT loc_1_0543
loc_1_0543:
        mov    al, ah
        out    0a1h, al
        popfd
        clc
        ret
loc_1_054A:
        pushfd
        cli
        mov    edi, ebx
        db  003h, 03Dh, 0A8h, 025h, 000h, 000h
        mov    word ptr [edi + 0ch], 0ffffh
        mov    word ptr [edi + 14h], 7008h
        mov    eax, 1f4h
        db  0BEh, 0F6h, 00Ch, 000h, 000h
        db  0CDh, 020h, 03Dh, 000h, 001h, 000h
        mov    dword ptr [edi + 18h], esi
        db  066h, 0A1h, 036h, 01Eh, 000h, 000h
        or     esi, 0ffffffffh
        mov    word ptr [edi + 1ch], si
        push   ebx
        call   sub_1_0650
        pop    ebx
        popfd
        clc
        ret
loc_1_058A:
        pushfd
        cli
        push   ebx
        or     eax, 0ffffffffh
        mov    esi, eax
        call   sub_1_0650
        pop    ebx
        db  08Bh, 03Dh, 0A8h, 025h, 000h, 000h
        or     esi, 0ffffffffh
        xchg   dword ptr [edi + ebx + 4], esi
        db  0CDh, 020h, 012h, 000h, 001h, 000h
        mov    esi, dword ptr [edi + ebx + 18h]
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        mov    esi, ebx
        add    esi, edi
        db  0B8h, 068h, 01Eh, 000h, 000h
        xor    ecx, ecx
loc_1_05C0:
        cmp    dword ptr [eax + 8], 0
        jne    SHORT loc_1_05FF
        test   byte ptr [eax + 6], 2
        jne    SHORT loc_1_05D2
        test   byte ptr [eax + 6], 4
        jne    SHORT loc_1_061D
loc_1_05D2:
        test   byte ptr [esi + ecx*4 + 20h], 1
        je     SHORT loc_1_061D
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    dword ptr [eax + 0ch], ebx
        mov    dword ptr [eax + 10h], ebx
        push   ecx
        push   eax
        mov    eax, dword ptr [eax + 18h]
loc_1_05EB:
        mov    ecx, dword ptr [eax + 8]
        jecxz  loc_1_05F4
        mov    eax, ecx
        jmp    SHORT loc_1_05EB
loc_1_05F4:
        db  0CDh, 020h, 002h, 000h, 003h, 000h
        pop    eax
        pop    ecx
        pop    ebx
        jmp    SHORT loc_1_061D
loc_1_05FF:
        test   byte ptr [eax + 6], 4
        jne    SHORT loc_1_061D
        cmp    dword ptr [eax + 8], ebx
        jne    SHORT loc_1_061D
        pushad
        mov    eax, dword ptr [eax + 18h]
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        xor    ecx, ecx
        dec    ecx
        call   sub_1_034C
        popad
loc_1_061D:
        add    eax, 40h
        inc    ecx
        cmp    ecx, 0fh
        jbe    SHORT loc_1_05C0
        popfd
        clc
        ret
loc_1_0629:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_1_064E
        db  0A1h, 0B0h, 025h, 000h, 000h
        movzx  eax, byte ptr [edi + eax + 1]
        dec    eax
        js     SHORT loc_1_064E
        db  00Fh, 0ABh, 005h, 0F0h, 025h, 000h, 000h
        xor    esi, esi
        db  089h, 034h, 085h, 0E0h, 025h, 000h, 000h
loc_1_064E:
        clc
        ret
sub_1_0650:
        mov    edi, ebx
        db  003h, 03Dh, 0A8h, 025h, 000h, 000h
        movzx  ecx, ax
        mov    si, cx
        xchg   word ptr [edi + 0ch], cx
        xor    cx, si
        je     NEAR PTR loc_1_074E
        test   al, 4
        je     SHORT loc_1_0671
        mov    ah, 0ffh
loc_1_0671:
        mov    word ptr [edi + 0eh], ax
loc_1_0675:
        bsf    edi, ecx
        je     NEAR PTR loc_1_072B
        btr    ecx, edi
        db  08Bh, 004h, 0BDh, 068h, 025h, 000h, 000h
        bt     esi, edi
        jae    SHORT loc_1_06A6
        dec    word ptr [eax + 16h]
        jne    SHORT loc_1_06C2
        test   byte ptr [eax + 6], 8
        je     SHORT loc_1_06C2
        mov    dx, word ptr [eax + 4]
        db  066h, 009h, 015h, 030h, 01Eh, 000h, 000h
        jmp    SHORT loc_1_06C2
loc_1_06A6:
        call   sub_1_1CFB
        inc    word ptr [eax + 16h]
        test   byte ptr [eax + 6], 8
        je     SHORT loc_1_06C2
        mov    dx, word ptr [eax + 4]
        not    edx
        db  066h, 021h, 015h, 030h, 01Eh, 000h, 000h
loc_1_06C2:
        push   eax
        db  066h, 0A1h, 030h, 01Eh, 000h, 000h
        db  066h, 00Bh, 005h, 032h, 01Eh, 000h, 000h
        db  066h, 03Bh, 005h, 03Ah, 01Eh, 000h, 000h
        je     SHORT loc_1_06F6
        db  066h, 08Bh, 015h, 03Ah, 01Eh, 000h, 000h
        db  066h, 0A3h, 03Ah, 01Eh, 000h, 000h
        cmp    al, dl
        je     SHORT loc_1_06F2
        out    21h, al
        cmp    ah, dh
        je     SHORT loc_1_06F6
        jmp    SHORT loc_1_06F2
loc_1_06F2:
        mov    al, ah
        out    0a1h, al
loc_1_06F6:
        pop    eax
        test   esi, 0ffff0000h
        jne    NEAR PTR loc_1_0675
        pushad
        xor    ecx, ecx
        bt     esi, edi
        rcl    ecx, 1
        mov    eax, dword ptr [eax + 18h]
loc_1_070E:
        cmp    dword ptr [eax + 18h], 0
        je     SHORT loc_1_071E
        push   eax
        push   ebx
        push   ecx
        call   dword ptr [eax + 18h]
        cli
        pop    ecx
        pop    ebx
        pop    eax
loc_1_071E:
        mov    eax, dword ptr [eax + 8]
        test   eax, eax
        jne    SHORT loc_1_070E
        popad
        jmp    NEAR PTR loc_1_0675
loc_1_072B:
        test   esi, 0ffff0000h
        jne    SHORT loc_1_074E
        mov    edx, ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        xchg   ebx, edx
        mov    ebp, dword ptr [edx + 8]
        mov    edi, ebx
        db  003h, 03Dh, 0A8h, 025h, 000h, 000h
        call   sub_1_0764
        cli
loc_1_074E:
        ret
        db  090h
loc_1_0750:
        cli
        xchg   edx, ebx
        mov    ecx, ebx
        db  003h, 00Dh, 0A8h, 025h, 000h, 000h
        mov    dword ptr [ecx + 4], 0
        jmp    SHORT loc_1_0775
sub_1_0764:
        mov    ecx, edi
        xor    esi, esi
        xchg   dword ptr [ecx + 4], esi
        test   esi, esi
        je     SHORT loc_1_0775
        db  0CDh, 020h, 012h, 000h, 001h, 000h
loc_1_0775:
        cmp    byte ptr [ecx + 1eh], 0
        jne    SHORT loc_1_07D5
        mov    ax, word ptr [ecx + 12h]
        mov    si, word ptr [ecx + 0eh]
        or     esi, eax
        shl    eax, 10h
        not    esi
        and    si, word ptr [ecx + 10h]
        mov    ax, si
        je     SHORT loc_1_07B2
        mov    byte ptr [ecx + 1eh], 0ffh
        push   eax
        push   ecx
        xor    eax, eax
        mov    ecx, 3019h
        mov    edi, 3e8h
        db  0BEh, 0FCh, 007h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        pop    ecx
        pop    eax
loc_1_07B2:
        sti
        mov    esi, 800000h
        cmp    word ptr [ecx + 16h], si
        ja     SHORT loc_1_07C7
        shr    esi, 1
        test   ax, ax
        jne    SHORT loc_1_07C7
        xor    esi, esi
loc_1_07C7:
        mov    eax, esi
        xchg   dword ptr [ecx], esi
        sub    eax, esi
        je     SHORT loc_1_07D6
        db  0CDh, 020h, 01Eh, 080h, 001h, 000h
loc_1_07D5:
        sti
loc_1_07D6:
        ret
loc_1_07D7:
        xor    eax, eax
        mov    ecx, 3009h
        db  0BEh, 0FCh, 007h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        db  08Bh, 03Dh, 0A8h, 025h, 000h, 000h
        add    edi, ebx
        xchg   dword ptr [edi], eax
        neg    eax
        db  0CDh, 020h, 01Eh, 080h, 001h, 000h
        nop
loc_1_07FC:
        jb     SHORT loc_1_07D7
        sub    esp, 8
        cli
        mov    edi, ebx
        db  003h, 03Dh, 0A8h, 025h, 000h, 000h
        movzx  eax, word ptr [edi + 0eh]
        not    ax
        and    ax, word ptr [edi + 10h]
        movzx  ecx, word ptr [edi + 12h]
        not    cx
        and    ax, cx
        je     NEAR PTR loc_1_0A32
        ror    al, 3
        ror    ax, 5
        bsf    eax, eax
        not    cx
        jecxz  loc_1_0846
        ror    cl, 3
        ror    cx, 5
        bsf    ecx, ecx
        cmp    ecx, eax
        jbe    NEAR PTR loc_1_0A7A
loc_1_0846:
        db  08Bh, 014h, 085h, 028h, 025h, 000h, 000h
        cmp    dword ptr [edx + 8], 0
        jne    SHORT loc_1_0881
        test   byte ptr [edx + 6], 4
        jne    SHORT loc_1_0881
        push   ebx
        db  0CDh, 020h, 0F1h, 000h, 001h, 000h
        mov    esi, ebx
        pop    ebx
        jecxz  loc_1_0881
        cmp    esi, ebx
        je     SHORT loc_1_0881
        mov    eax, dword ptr [edx + 18h]
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        xchg   esi, ebx
        db  0CDh, 020h, 002h, 000h, 003h, 000h
        xchg   esi, ebx
        jmp    NEAR PTR loc_1_0A32
loc_1_0881:
        mov    dword ptr [esp + 4], 0
        lea    esi, [esp + 4]
        mov    eax, dword ptr [edx + 18h]
loc_1_0890:
        cmp    dword ptr [eax + 10h], 0
        je     SHORT loc_1_08AC
        pushad
        mov    edx, eax
        call   dword ptr [eax + 10h]
        popad
        cli
        jae    SHORT loc_1_08AC
        test   word ptr [edx + 6], 10h
        jne    NEAR PTR loc_1_0A32
loc_1_08AC:
        mov    eax, dword ptr [eax + 8]
        test   eax, eax
        jne    SHORT loc_1_0890
        mov    cx, word ptr [edx + 4]
        or     word ptr [edi + 12h], cx
        mov    eax, dword ptr [edx]
        xor    ecx, ecx
        btr    eax, 3
        jae    SHORT loc_1_08CA
        inc    ecx
        or     byte ptr [edi + 12h], 4
loc_1_08CA:
        add    al, byte ptr [ecx + edi + 14h]
        mov    dword ptr [esp], eax
        xor    esi, esi
        mov    ecx, dword ptr [ebx]
        test   cl, 40h
        je     SHORT loc_1_08F2
        cmp    dword ptr [esp + 4], 0
        jne    SHORT loc_1_08F2
        push   ecx
        push   edx
        db  0CDh, 020h, 044h, 000h, 001h, 000h
        pop    edx
        pop    ecx
        je     SHORT loc_1_08F2
        mov    esi, 20h
loc_1_08F2:
        xor    ecx, esi
        shr    ecx, 5
        and    ecx, 1
        db  08Bh, 035h, 0ACh, 025h, 000h, 000h
        or     word ptr [esi + ebx], 2020h
        mov    esi, dword ptr [edx]
        cmp    word ptr [edi + esi*2 + 60h], 1
        jb     SHORT loc_1_091F
        test   byte ptr [ebx], 20h
        jne    SHORT loc_1_0937
        test   ecx, ecx
        je     NEAR PTR loc_1_0A07
        jmp    SHORT loc_1_0937
loc_1_091F:
        cmp    dword ptr [edx + 1ch], 0
        je     SHORT loc_1_0937
        mov    esi, dword ptr [edx]
        db  0CDh, 020h, 0D0h, 000h, 001h, 000h
        or     eax, 1
        mov    dword ptr [edi + esi*4 + 80h], eax
loc_1_0937:
        bt     dword ptr [ebx], 5
        rcl    ecx, 1
        db  0FFh, 014h, 08Dh, 074h, 026h, 000h, 000h
        mov    eax, dword ptr [edx]
        inc    word ptr [edi + eax*2 + 60h]
        inc    word ptr [edi + 16h]
        inc    word ptr [edi + 1ch]
        mov    esi, edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    eax, 800000h
        xchg   dword ptr [esi], eax
        neg    eax
        add    eax, dword ptr [esi]
        je     SHORT loc_1_096E
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
loc_1_096E:
        mov    eax, 400000h
        db  0CDh, 020h, 014h, 001h, 001h, 000h
        db  003h, 03Dh, 0B0h, 025h, 000h, 000h
        inc    byte ptr [edi]
        mov    edi, esi
        btr    dword ptr [ebx], 8
        rcl    ecx, 1
        test   byte ptr [ebx], 20h
        jne    NEAR PTR loc_1_0A46
        movzx  eax, word ptr [ebp + 30h]
        sub    eax, 0ah
        jb     NEAR PTR loc_1_0A46
        mov    dword ptr [ebp + 30h], eax
        movzx  esi, word ptr [ebp + 34h]
        shl    esi, 4
        add    esi, eax
        mov    ax, word ptr [ebp + 28h]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 24h]
        mov    dword ptr [esi + 6], eax
        mov    ecx, dword ptr [edx + ecx*4 + 20h]
        mov    eax, dword ptr [esp]
        db  00Fh, 0A3h, 005h, 044h, 01Eh, 000h, 000h
        jb     SHORT loc_1_09F5
loc_1_09C9:
        mov    dword ptr [esi], ecx
        mov    ecx, dword ptr [esp + 4]
        or     ecx, ecx
        jne    SHORT loc_1_09DB
        db  0CDh, 020h, 041h, 001h, 001h, 000h
        mov    ecx, eax
loc_1_09DB:
        mov    word ptr [ebp + 24h], cx
        shr    ecx, 10h
        mov    word ptr [ebp + 28h], cx
        mov    eax, dword ptr [ebp + 2ch]
        mov    word ptr [esi + 4], ax
        and    ah, 0fch
        mov    byte ptr [ebp + 2dh], ah
        jmp    SHORT loc_1_0A74
loc_1_09F5:
        mov    word ptr [ebp + 24h], cx
        shr    ecx, 10h
        mov    word ptr [ebp + 28h], cx
        add    word ptr [ebp + 30h], 6
        jmp    SHORT loc_1_0A56
loc_1_0A07:
        db  00Fh, 0A3h, 005h, 044h, 01Eh, 000h, 000h
        jb     SHORT loc_1_0A56
        movzx  ecx, word ptr [ebp + 30h]
        sub    ecx, 6
        jb     SHORT loc_1_0A56
        mov    dword ptr [ebp + 30h], ecx
        movzx  esi, word ptr [ebp + 34h]
        shl    esi, 4
        add    esi, ecx
        mov    cx, word ptr [ebp + 28h]
        shl    ecx, 10h
        mov    cx, word ptr [ebp + 24h]
        jmp    SHORT loc_1_09C9
loc_1_0A32:
        cmp    word ptr [edi + 12h], 0
        jne    SHORT loc_1_0A7A
        mov    byte ptr [edi + 1eh], 0
        mov    edx, ebx
        call   sub_1_0764
        jmp    SHORT loc_1_0A7E
loc_1_0A46:
        mov    ecx, dword ptr [edx + ecx*4 + 20h]
        movzx  edx, cx
        shr    ecx, 10h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
loc_1_0A56:
        sti
        mov    eax, dword ptr [esp]
        mov    ecx, dword ptr [esp + 4]
        jecxz  loc_1_0A6E
        movzx  edx, cx
        shr    ecx, 10h
        db  0CDh, 020h, 04Ch, 000h, 001h, 000h
        jmp    SHORT loc_1_0A74
loc_1_0A6E:
        db  0CDh, 020h, 046h, 000h, 001h, 000h
loc_1_0A74:
        test   byte ptr [ebx + 1], 80h
        jne    SHORT loc_1_0A83
loc_1_0A7A:
        mov    byte ptr [edi + 1eh], 0
loc_1_0A7E:
        sti
        add    esp, 8
        ret
loc_1_0A83:
        db  0CDh, 020h, 039h, 000h, 001h, 000h
        jmp    SHORT loc_1_0A7A
loc_1_0A8B:
        ret
loc_1_0A8C:
        db  0CDh, 020h, 08Ah, 000h, 001h, 000h
loc_1_0A92:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_1_0A9F
        call   sub_1_0B47
loc_1_0A9F:
        db  0CDh, 020h, 08Bh, 080h, 001h, 000h
loc_1_0AA5:
        db  0CDh, 020h, 08Bh, 000h, 001h, 000h
        db  0CDh, 020h, 089h, 000h, 001h, 000h
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_1_0A8B
        push   esi
        xor    esi, esi
loc_1_0ABC:
        db  083h, 03Dh, 0DCh, 025h, 000h, 000h, 000h
        je     SHORT loc_1_0B25
        push   edi
        push   ecx
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  003h, 03Dh, 0B0h, 025h, 000h, 000h
        cmp    byte ptr [edi + 1], 0
        jne    SHORT loc_1_0B27
        db  00Fh, 0BCh, 005h, 0F0h, 025h, 000h, 000h
        je     SHORT loc_1_0B3D
        db  00Fh, 0B3h, 005h, 0F0h, 025h, 000h, 000h
        inc    eax
        mov    byte ptr [edi + 1], al
        dec    eax
        db  02Bh, 03Dh, 0B0h, 025h, 000h, 000h
        db  089h, 03Ch, 085h, 0E0h, 025h, 000h, 000h
        db  08Bh, 00Dh, 0DCh, 025h, 000h, 000h
        shl    eax, 8
        sub    ecx, eax
        mov    eax, ecx
        db  02Bh, 005h, 0D8h, 025h, 000h, 000h
        xchg   dword ptr [ebp + esi + 30h], eax
        mov    word ptr [ecx], ax
        db  0A1h, 0D4h, 025h, 000h, 000h
        xchg   word ptr [ebp + esi + 34h], ax
        mov    word ptr [ecx + 2], ax
loc_1_0B23:
        pop    ecx
        pop    edi
loc_1_0B25:
        pop    esi
        ret
loc_1_0B27:
        mov    ax, word ptr [ebp + esi + 34h]
        db  066h, 03Bh, 005h, 0D4h, 025h, 000h, 000h
        jne    SHORT loc_1_0B23
        sub    word ptr [ebp + esi + 30h], 2
        jmp    SHORT loc_1_0B23
loc_1_0B3D:
        xor    ecx, ecx
        db  0CDh, 020h, 03Ch, 001h, 001h, 000h
        jmp    SHORT loc_1_0B23
sub_1_0B47:
        push   esi
        mov    esi, 24h
        jmp    NEAR PTR loc_1_0ABC
loc_1_0B52:
        mov    eax, 100h
        jmp    SHORT loc_1_0B5B
loc_1_0B59:
        xor    eax, eax
loc_1_0B5B:
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        db  0CDh, 020h, 08Ch, 000h, 001h, 000h
        db  0CDh, 020h, 089h, 000h, 001h, 000h
        or     dword ptr [ebx], eax
        jmp    SHORT loc_1_0BA7
loc_1_0B71:
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        db  0CDh, 020h, 08Ch, 000h, 001h, 000h
        or     byte ptr [ebx + 1], 1
        jmp    SHORT loc_1_0BA7
loc_1_0B83:
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        db  0CDh, 020h, 08Ch, 000h, 001h, 000h
        jmp    SHORT loc_1_0BA7
loc_1_0B91:
        or     byte ptr [ebx + 1], 1
loc_1_0B95:
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        db  0CDh, 020h, 08Ah, 000h, 001h, 000h
        db  0CDh, 020h, 08Ch, 000h, 001h, 000h
loc_1_0BA7:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    NEAR PTR loc_1_0C62
        db  08Bh, 00Dh, 0DCh, 025h, 000h, 000h
        test   ecx, ecx
        je     NEAR PTR loc_1_0C62
        db  0A1h, 0B0h, 025h, 000h, 000h
        movzx  eax, byte ptr [edi + eax + 1]
        dec    eax
        js     SHORT loc_1_0C32
        shl    eax, 8
        sub    ecx, eax
        xor    esi, esi
        test   byte ptr [ebx], 20h
        je     SHORT loc_1_0BDF
        mov    esi, 24h
loc_1_0BDF:
        mov    ax, word ptr [ebp + esi + 34h]
        db  066h, 03Bh, 005h, 0D4h, 025h, 000h, 000h
        jne    SHORT loc_1_0C62
        mov    eax, ecx
        db  02Bh, 005h, 0D8h, 025h, 000h, 000h
        cmp    ax, word ptr [ebp + esi + 30h]
        ja     SHORT loc_1_0C2A
        mov    eax, dword ptr [ecx]
        mov    word ptr [ebp + esi + 30h], ax
        shr    eax, 10h
        mov    word ptr [ebp + esi + 34h], ax
        db  08Bh, 00Dh, 0B0h, 025h, 000h, 000h
        xor    eax, eax
        xchg   byte ptr [edi + ecx + 1], al
        dec    eax
        db  00Fh, 0ABh, 005h, 0F0h, 025h, 000h, 000h
        xor    esi, esi
        db  089h, 034h, 085h, 0E0h, 025h, 000h, 000h
        jmp    SHORT loc_1_0C62
loc_1_0C2A:
        add    word ptr [ebp + esi + 30h], 2
        jmp    SHORT loc_1_0C62
loc_1_0C32:
        db  0CDh, 020h, 072h, 001h, 001h, 000h
        jmp    SHORT loc_1_0C62
loc_1_0C3A:
        or     byte ptr [ebx + 1], 1
loc_1_0C3E:
        movzx  ecx, word ptr [ebp + 34h]
        shl    ecx, 4
        movzx  esi, word ptr [ebp + 30h]
        mov    ax, word ptr [esi + ecx]
        mov    word ptr [ebp + 24h], ax
        add    si, 2
        mov    ax, word ptr [esi + ecx]
        mov    word ptr [ebp + 28h], ax
        add    word ptr [ebp + 30h], 4
loc_1_0C62:
        test   byte ptr [ebp + 2dh], 2
        jne    SHORT loc_1_0C6E
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
loc_1_0C6E:
        clc
        pushfd
        db  0A1h, 0B0h, 025h, 000h, 000h
        add    eax, edi
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_0C89
        dec    byte ptr [eax]
        mov    eax, 0ffc00000h
        db  0CDh, 020h, 014h, 001h, 001h, 000h
loc_1_0C89:
        cli
        db  08Bh, 03Dh, 0A8h, 025h, 000h, 000h
        add    edi, ebx
        mov    ecx, dword ptr [edx]
        dec    word ptr [edi + 1ch]
        jns    SHORT loc_1_0CA4
        db  0A1h, 0ACh, 025h, 000h, 000h
        and    byte ptr [eax + ebx + 1], 0dfh
loc_1_0CA4:
        cmp    word ptr [edi + ecx*2 + 60h], 1
        jb     SHORT loc_1_0CF4
        je     SHORT loc_1_0CB9
        dec    word ptr [edi + ecx*2 + 60h]
        dec    word ptr [edi + 16h]
        jmp    SHORT loc_1_0CF4
loc_1_0CB9:
        mov    dword ptr [edi + ecx*4 + 80h], 0
loc_1_0CC4:
        dec    word ptr [edi + ecx*2 + 60h]
        push   edi
        mov    eax, dword ptr [edx + 18h]
loc_1_0CCD:
        push   dword ptr [eax + 8]
        mov    ecx, dword ptr [eax + 1ch]
        jecxz  loc_1_0CE1
        bt     word ptr [esp + 8], 0
        push   ebx
        call   ecx
        pop    ebx
        cli
loc_1_0CE1:
        pop    eax
        test   eax, eax
        jne    SHORT loc_1_0CCD
        pop    edi
        dec    word ptr [edi + 16h]
        jne    SHORT loc_1_0CF4
        mov    edx, ebx
        call   sub_1_0764
loc_1_0CF4:
        popfd
        ret
loc_1_0CF6:
        mov    edi, ebx
        db  003h, 03Dh, 0A8h, 025h, 000h, 000h
        mov    eax, 1f4h
        db  0BEh, 0F6h, 00Ch, 000h, 000h
        db  0CDh, 020h, 03Dh, 000h, 001h, 000h
        mov    dword ptr [edi + 18h], esi
        cmp    word ptr [edi + 16h], 0
        je     SHORT loc_1_0D51
        db  0CDh, 020h, 0D0h, 000h, 001h, 000h
        or     eax, 1
        mov    esi, eax
        lea    edi, [edi + 80h]
        mov    ecx, 10h
        cld
loc_1_0D2F:
        xor    eax, eax
        repe scasd
        je     SHORT loc_1_0D51
        mov    edx, ecx
        neg    edx
        add    edx, 0fh
        db  08Bh, 014h, 095h, 068h, 025h, 000h, 000h
        mov    eax, esi
        sub    eax, dword ptr [edi - 4]
        cmp    eax, dword ptr [edx + 1ch]
        jae    SHORT loc_1_0D52
        test   ecx, ecx
        jne    SHORT loc_1_0D2F
loc_1_0D51:
        ret
loc_1_0D52:
        mov    edi, ebx
        db  003h, 03Dh, 0A8h, 025h, 000h, 000h
        mov    ecx, dword ptr [edx]
        mov    dword ptr [edi + ecx*4 + 80h], 0
        stc
        pushfd
        push   edi
        db  08Bh, 035h, 0B0h, 025h, 000h, 000h
        mov    eax, 400000h
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        push   edi
        db  0CDh, 020h, 014h, 001h, 001h, 000h
        neg    eax
        db  0CDh, 020h, 00Dh, 001h, 001h, 000h
loc_1_0D8A:
        cmp    byte ptr [esi + edi], 0
        je     SHORT loc_1_0D99
        dec    byte ptr [esi + edi]
        db  0CDh, 020h, 014h, 001h, 001h, 000h
loc_1_0D99:
        db  0CDh, 020h, 013h, 001h, 001h, 000h
        cmp    ebx, dword ptr [edi + 14h]
        jne    SHORT loc_1_0DAC
        db  0CDh, 020h, 00Eh, 001h, 001h, 000h
        jne    SHORT loc_1_0D8A
loc_1_0DAC:
        pop    edi
        db  0CDh, 020h, 014h, 001h, 001h, 000h
        pop    edi
        cli
        jmp    NEAR PTR loc_1_0CC4
        db  0EBh, 006h, 0FFh, 025h, 064h, 01Eh, 000h, 000h
loc_1_0DC2:
        cmp    eax, 0ffh
        ja     SHORT loc_1_0DD0
        db  00Fh, 0ABh, 005h, 044h, 01Eh, 000h, 000h
loc_1_0DD0:
        db  0FFh, 025h, 064h, 01Eh, 000h, 000h
        db  08Bh, 0C0h
loc_1_0DD8:
        out    0a0h, al
        jmp    SHORT loc_1_0DDC
loc_1_0DDC:
        mov    al, 62h
loc_1_0DDE:
        out    20h, al
        mov    eax, dword ptr [edx + 18h]
        push   ebx
loc_1_0DE4:
        push   eax
        mov    edx, dword ptr [eax + 24h]
        call   dword ptr [eax + 0ch]
        cli
        pop    eax
        jae    SHORT loc_1_0DF9
        mov    ebx, dword ptr [esp]
        mov    eax, dword ptr [eax + 8]
        test   eax, eax
        jne    SHORT loc_1_0DE4
loc_1_0DF9:
        pop    ebx
        db  0FFh, 025h, 03Ch, 01Eh, 000h, 000h
loc_1_0E00:
        mov    al, 67h
        out    0a0h, al
        jmp    SHORT loc_1_0E06
loc_1_0E06:
        mov    al, 62h
        out    20h, al
        ret
loc_1_0E0B:
        mov    al, 67h
        out    20h, al
        ret
loc_1_0E10:
        out    0a0h, al
        jmp    SHORT loc_1_0E14
loc_1_0E14:
        mov    al, 62h
loc_1_0E16:
        out    20h, al
        mov    eax, dword ptr [edx + 18h]
        mov    ecx, dword ptr [eax]
        mov    ecx, dword ptr [ecx]
        push   ebx
        push   ecx
        push   eax
loc_1_0E22:
        mov    edx, dword ptr [eax + 24h]
        push   eax
        call   dword ptr [eax + 0ch]
        cli
        pop    eax
        jae    SHORT loc_1_0E3A
        mov    ebx, dword ptr [esp + 8]
        mov    eax, dword ptr [eax + 8]
        test   eax, eax
        jne    SHORT loc_1_0E22
        jmp    SHORT loc_1_0E47
loc_1_0E3A:
        mov    eax, dword ptr [esp]
        mov    ecx, dword ptr [esp + 4]
        mov    ebx, dword ptr [esp + 8]
        jmp    SHORT loc_1_0E22
loc_1_0E47:
        pop    eax
        pop    ecx
        pop    ebx
        db  0FFh, 025h, 03Ch, 01Eh, 000h, 000h
loc_1_0E50:
        db  0CDh, 020h, 018h, 080h, 003h, 000h
        mov    eax, eax
loc_1_0E58:
        db  080h, 00Dh, 032h, 01Eh, 000h, 000h, 001h
        db  0BAh, 068h, 01Eh, 000h, 000h
        db  0A0h, 032h, 01Eh, 000h, 000h
        db  00Ah, 005h, 030h, 01Eh, 000h, 000h
        db  038h, 005h, 03Ah, 01Eh, 000h, 000h
        je     SHORT loc_1_0E7E
        db  0A2h, 03Ah, 01Eh, 000h, 000h
        out    21h, al
loc_1_0E7E:
        mov    al, 60h
loc_1_0E80:
        jmp    NEAR PTR loc_1_0DDE
        db  090h, 08Bh, 0C0h
loc_1_0E88:
        db  080h, 00Dh, 032h, 01Eh, 000h, 000h, 002h
        db  0BAh, 0A8h, 01Eh, 000h, 000h
        db  0A0h, 032h, 01Eh, 000h, 000h
        db  00Ah, 005h, 030h, 01Eh, 000h, 000h
        db  038h, 005h, 03Ah, 01Eh, 000h, 000h
        je     SHORT loc_1_0EAE
        db  0A2h, 03Ah, 01Eh, 000h, 000h
        out    21h, al
loc_1_0EAE:
        mov    al, 61h
loc_1_0EB0:
        jmp    NEAR PTR loc_1_0DDE
        db  090h, 08Bh, 0C0h
loc_1_0EB8:
        db  080h, 00Dh, 032h, 01Eh, 000h, 000h, 008h
        db  0BAh, 028h, 01Fh, 000h, 000h
        db  0A0h, 032h, 01Eh, 000h, 000h
        db  00Ah, 005h, 030h, 01Eh, 000h, 000h
        db  038h, 005h, 03Ah, 01Eh, 000h, 000h
        je     SHORT loc_1_0EDE
        db  0A2h, 03Ah, 01Eh, 000h, 000h
        out    21h, al
loc_1_0EDE:
        mov    al, 63h
loc_1_0EE0:
        jmp    NEAR PTR loc_1_0DDE
        db  090h, 08Bh, 0C0h
loc_1_0EE8:
        db  080h, 00Dh, 032h, 01Eh, 000h, 000h, 010h
        db  0BAh, 068h, 01Fh, 000h, 000h
        db  0A0h, 032h, 01Eh, 000h, 000h
        db  00Ah, 005h, 030h, 01Eh, 000h, 000h
        db  038h, 005h, 03Ah, 01Eh, 000h, 000h
        je     SHORT loc_1_0F0E
        db  0A2h, 03Ah, 01Eh, 000h, 000h
        out    21h, al
loc_1_0F0E:
        mov    al, 64h
loc_1_0F10:
        jmp    NEAR PTR loc_1_0DDE
        db  090h, 08Bh, 0C0h
loc_1_0F18:
        db  080h, 00Dh, 032h, 01Eh, 000h, 000h, 020h
        db  0BAh, 0A8h, 01Fh, 000h, 000h
        db  0A0h, 032h, 01Eh, 000h, 000h
        db  00Ah, 005h, 030h, 01Eh, 000h, 000h
        db  038h, 005h, 03Ah, 01Eh, 000h, 000h
        je     SHORT loc_1_0F3E
        db  0A2h, 03Ah, 01Eh, 000h, 000h
        out    21h, al
loc_1_0F3E:
        mov    al, 65h
loc_1_0F40:
        jmp    NEAR PTR loc_1_0DDE
        db  090h, 08Bh, 0C0h
loc_1_0F48:
        db  080h, 00Dh, 032h, 01Eh, 000h, 000h, 040h
        db  0BAh, 0E8h, 01Fh, 000h, 000h
        db  0A0h, 032h, 01Eh, 000h, 000h
        db  00Ah, 005h, 030h, 01Eh, 000h, 000h
        db  038h, 005h, 03Ah, 01Eh, 000h, 000h
        je     SHORT loc_1_0F6E
        db  0A2h, 03Ah, 01Eh, 000h, 000h
        out    21h, al
loc_1_0F6E:
        mov    al, 66h
loc_1_0F70:
        jmp    NEAR PTR loc_1_0DDE
        db  090h, 08Bh, 0C0h
loc_1_0F78:
        mov    al, 0bh
        out    20h, al
        jmp    SHORT loc_1_0F7E
loc_1_0F7E:
        in     al, 20h
        test   al, 80h
        je     NEAR PTR loc_1_0E0B
        db  080h, 00Dh, 032h, 01Eh, 000h, 000h, 080h
        db  0BAh, 028h, 020h, 000h, 000h
        db  0A0h, 032h, 01Eh, 000h, 000h
        db  00Ah, 005h, 030h, 01Eh, 000h, 000h
        db  038h, 005h, 03Ah, 01Eh, 000h, 000h
        je     SHORT loc_1_0FAE
        db  0A2h, 03Ah, 01Eh, 000h, 000h
        out    21h, al
loc_1_0FAE:
        mov    al, 67h
loc_1_0FB0:
        jmp    NEAR PTR loc_1_0DDE
        db  090h, 08Bh, 0C0h
loc_1_0FB8:
        db  080h, 00Dh, 033h, 01Eh, 000h, 000h, 001h
        db  0BAh, 068h, 020h, 000h, 000h
        db  0A0h, 033h, 01Eh, 000h, 000h
        db  00Ah, 005h, 031h, 01Eh, 000h, 000h
        db  038h, 005h, 03Bh, 01Eh, 000h, 000h
        je     SHORT loc_1_0FDE
        db  0A2h, 03Bh, 01Eh, 000h, 000h
        out    0a1h, al
loc_1_0FDE:
        mov    al, 60h
loc_1_0FE0:
        jmp    NEAR PTR loc_1_0DD8
        db  090h, 08Bh, 0C0h
loc_1_0FE8:
        db  080h, 00Dh, 033h, 01Eh, 000h, 000h, 002h
        db  0BAh, 0A8h, 020h, 000h, 000h
        db  0A0h, 033h, 01Eh, 000h, 000h
        db  00Ah, 005h, 031h, 01Eh, 000h, 000h
        db  038h, 005h, 03Bh, 01Eh, 000h, 000h
        je     SHORT loc_1_100E
        db  0A2h, 03Bh, 01Eh, 000h, 000h
        out    0a1h, al
loc_1_100E:
        mov    al, 61h
loc_1_1010:
        jmp    NEAR PTR loc_1_0DD8
        db  090h, 08Bh, 0C0h
loc_1_1018:
        db  080h, 00Dh, 033h, 01Eh, 000h, 000h, 004h
        db  0BAh, 0E8h, 020h, 000h, 000h
        db  0A0h, 033h, 01Eh, 000h, 000h
        db  00Ah, 005h, 031h, 01Eh, 000h, 000h
        db  038h, 005h, 03Bh, 01Eh, 000h, 000h
        je     SHORT loc_1_103E
        db  0A2h, 03Bh, 01Eh, 000h, 000h
        out    0a1h, al
loc_1_103E:
        mov    al, 62h
loc_1_1040:
        jmp    NEAR PTR loc_1_0DD8
        db  090h, 08Bh, 0C0h
loc_1_1048:
        db  080h, 00Dh, 033h, 01Eh, 000h, 000h, 008h
        db  0BAh, 028h, 021h, 000h, 000h
        db  0A0h, 033h, 01Eh, 000h, 000h
        db  00Ah, 005h, 031h, 01Eh, 000h, 000h
        db  038h, 005h, 03Bh, 01Eh, 000h, 000h
        je     SHORT loc_1_106E
        db  0A2h, 03Bh, 01Eh, 000h, 000h
        out    0a1h, al
loc_1_106E:
        mov    al, 63h
loc_1_1070:
        jmp    NEAR PTR loc_1_0DD8
        db  090h, 08Bh, 0C0h
loc_1_1078:
        db  080h, 00Dh, 033h, 01Eh, 000h, 000h, 010h
        db  0BAh, 068h, 021h, 000h, 000h
        db  0A0h, 033h, 01Eh, 000h, 000h
        db  00Ah, 005h, 031h, 01Eh, 000h, 000h
        db  038h, 005h, 03Bh, 01Eh, 000h, 000h
        je     SHORT loc_1_109E
        db  0A2h, 03Bh, 01Eh, 000h, 000h
        out    0a1h, al
loc_1_109E:
        mov    al, 64h
loc_1_10A0:
        jmp    NEAR PTR loc_1_0DD8
        db  090h, 08Bh, 0C0h
loc_1_10A8:
        db  080h, 00Dh, 033h, 01Eh, 000h, 000h, 020h
        db  0BAh, 0A8h, 021h, 000h, 000h
        db  0A0h, 033h, 01Eh, 000h, 000h
        db  00Ah, 005h, 031h, 01Eh, 000h, 000h
        db  038h, 005h, 03Bh, 01Eh, 000h, 000h
        je     SHORT loc_1_10CE
        db  0A2h, 03Bh, 01Eh, 000h, 000h
        out    0a1h, al
loc_1_10CE:
        mov    al, 65h
loc_1_10D0:
        jmp    NEAR PTR loc_1_0DD8
        db  090h, 08Bh, 0C0h
loc_1_10D8:
        db  080h, 00Dh, 033h, 01Eh, 000h, 000h, 040h
        db  0BAh, 0E8h, 021h, 000h, 000h
        db  0A0h, 033h, 01Eh, 000h, 000h
        db  00Ah, 005h, 031h, 01Eh, 000h, 000h
        db  038h, 005h, 03Bh, 01Eh, 000h, 000h
        je     SHORT loc_1_10FE
        db  0A2h, 03Bh, 01Eh, 000h, 000h
        out    0a1h, al
loc_1_10FE:
        mov    al, 66h
loc_1_1100:
        jmp    NEAR PTR loc_1_0DD8
        db  090h, 08Bh, 0C0h
loc_1_1108:
        mov    al, 0bh
        out    0a0h, al
        jmp    SHORT loc_1_110E
loc_1_110E:
        in     al, 0a0h
        test   al, 80h
        je     NEAR PTR loc_1_0E00
        db  080h, 00Dh, 033h, 01Eh, 000h, 000h, 080h
        db  0BAh, 028h, 022h, 000h, 000h
        db  0A0h, 033h, 01Eh, 000h, 000h
        db  00Ah, 005h, 031h, 01Eh, 000h, 000h
        db  038h, 005h, 03Bh, 01Eh, 000h, 000h
        je     SHORT loc_1_113E
        db  0A2h, 03Bh, 01Eh, 000h, 000h
        out    0a1h, al
loc_1_113E:
        mov    al, 67h
loc_1_1140:
        jmp    NEAR PTR loc_1_0DD8
        db  090h
loc_1_1146:
        ret
loc_1_1147:
        db  0CDh, 020h, 02Fh, 000h, 001h, 000h
        add    byte ptr [eax], al
        db  000h
VPICD_PM_API:
        mov    eax, dword ptr [ebp + 1ch]
        test   eax, eax
        je     NEAR PTR loc_1_1267
        cmp    ax, 1
        je     SHORT loc_1_11CC
        cmp    ax, 2
        jne    NEAR PTR loc_1_128D
        xchg   esi, eax
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        mov    eax, dword ptr [esi + 8]
        db  0CDh, 020h, 011h, 000h, 003h, 000h
        xor    eax, eax
        xchg   word ptr [esi + 30h], ax
        push   0
        push   eax
        db  0CDh, 020h, 077h, 000h, 001h, 000h
        add    esp, 8
        xor    eax, eax
        xchg   word ptr [esi + 32h], ax
        push   0
        push   eax
        db  0CDh, 020h, 077h, 000h, 001h, 000h
        add    esp, 8
        movzx  ebx, word ptr [esi + 34h]
        test   ebx, ebx
        je     SHORT loc_1_11C7
        lea    edi, [esi + 36h]
loc_1_11AF:
        xor    eax, eax
        xchg   word ptr [edi + 4], ax
        push   0
        push   eax
        db  0CDh, 020h, 077h, 000h, 001h, 000h
        add    esp, 8
        add    edi, 6
        dec    ebx
        jne    SHORT loc_1_11AF
loc_1_11C7:
        jmp    NEAR PTR loc_1_126D
loc_1_11CC:
        xchg   esi, eax
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        db  0BFh, 092h, 026h, 000h, 000h
        mov    ax, word ptr [esi]
        mov    word ptr [edi], ax
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        jb     NEAR PTR loc_1_128D
        mov    dword ptr [esi + 8], eax
        movzx  ecx, word ptr [ebp]
        mov    word ptr [esi + 0ch], cx
        movzx  ecx, word ptr [ebp + 38h]
        mov    word ptr [esi + 0eh], cx
        mov    dword ptr [eax + 20h], esi
        db  0A1h, 08Ah, 026h, 000h, 000h
        mov    dword ptr [esi + 28h], eax
        movzx  eax, word ptr [esi + 2ch]
        mov    ecx, eax
        call   sub_1_1292
        or     eax, eax
        je     SHORT loc_1_1284
        mov    word ptr [esi + 30h], ax
        cmp    cx, word ptr [ebp + 38h]
        jne    SHORT loc_1_122A
        mov    word ptr [esi + 0eh], ax
loc_1_122A:
        movzx  eax, word ptr [esi + 2eh]
        mov    ecx, eax
        call   sub_1_1292
        or     eax, eax
        je     SHORT loc_1_1272
        mov    word ptr [esi + 32h], ax
        cmp    cx, word ptr [ebp + 38h]
        jne    SHORT loc_1_1247
        mov    word ptr [esi + 0eh], ax
loc_1_1247:
        movzx  ecx, word ptr [esi + 34h]
        jecxz  loc_1_1265
        lea    edi, [esi + 36h]
loc_1_1250:
        movzx  eax, word ptr [edi]
        call   sub_1_1292
        or     eax, eax
        je     SHORT loc_1_1272
        mov    word ptr [edi + 4], ax
        add    edi, 6
        loop   loc_1_1250
loc_1_1265:
        jmp    SHORT loc_1_126D
loc_1_1267:
        mov    word ptr [ebp + 1ch], 40ah
loc_1_126D:
        and    byte ptr [ebp + 2ch], 0feh
        ret
loc_1_1272:
        xor    eax, eax
        xchg   word ptr [esi + 30h], ax
        push   0
        push   eax
        db  0CDh, 020h, 077h, 000h, 001h, 000h
        add    esp, 8
loc_1_1284:
        mov    eax, dword ptr [esi + 8]
        db  0CDh, 020h, 011h, 000h, 003h, 000h
loc_1_128D:
        or     byte ptr [ebp + 2ch], 1
        ret
sub_1_1292:
        push   ecx
        push   edx
        push   0
        push   ebx
        push   eax
        db  0CDh, 020h, 07Bh, 000h, 001h, 000h
        add    esp, 0ch
        test   dh, 8
        je     SHORT loc_1_12A9
        and    dh, 9fh
loc_1_12A9:
        ror    eax, 10h
        rol    edx, 8
        mov    cl, dh
        mov    ch, dl
        shl    ecx, 10h
        mov    cx, ax
        cmp    ecx, 10ffffh
        ja     SHORT loc_1_12C4
        add    ecx, dword ptr [ebx + 4]
loc_1_12C4:
        mov    ax, cx
        shr    ecx, 10h
        mov    dh, cl
        mov    dl, ch
        ror    edx, 8
        rol    eax, 10h
        push   0
        push   eax
        push   edx
        db  0CDh, 020h, 076h, 000h, 001h, 000h
        add    esp, 0ch
        pop    edx
        pop    ecx
        ret
loc_1_12E4:
        mov    esi, dword ptr [eax + 20h]
        mov    ecx, dword ptr [ebx + 0ch]
        mov    word ptr [esi + 2], cx
        mov    word ptr [esi + 1ch], 4
        pop    ecx
        push   cs
        push   ecx
        push   eax
        push   ebx
        push   esi
        push   es
        push   ds
        db  0FFh, 035h, 08Eh, 026h, 000h, 000h
        movzx  ecx, word ptr [esi + 30h]
        push   ecx
        movzx  ecx, word ptr [esi + 1eh]
        push   ecx
        mov    di, word ptr [esi + 0ch]
        mov    es, word ptr [esi + 0eh]
        retf
loc_1_1315:
        pop    ds
        pop    es
        pop    esi
        pop    ebx
        pop    eax
        mov    word ptr [esi + 1ch], 0
        retf
        db  02Eh, 08Bh, 0C0h
loc_1_1324:
        push   ebp
        mov    ebp, esp
        and    esp, 0fffffffch
        push   ds
        push   es
        pushad
        movzx  ecx, ax
        movzx  edi, di
        mov    eax, dword ptr es:[edi + 8]
        mov    dx, ss
        mov    ds, dx
        mov    es, dx
        db  0FFh, 02Dh, 084h, 026h, 000h, 000h
loc_1_1346:
        mov    ebp, esp
        cmp    cx, 6
        ja     SHORT loc_1_1355
        db  0FFh, 014h, 08Dh, 0B4h, 026h, 000h, 000h
loc_1_1355:
        popad
        pop    es
        pop    ds
        mov    esp, ebp
        pop    ebp
        retf
        db  02Eh, 08Bh, 0C0h
loc_1_1360:
        db  0CDh, 020h, 004h, 080h, 003h, 000h
        mov    eax, eax
loc_1_1368:
        db  0CDh, 020h, 008h, 080h, 003h, 000h
        mov    eax, eax
loc_1_1370:
        db  0CDh, 020h, 009h, 080h, 003h, 000h
        mov    eax, eax
loc_1_1378:
        db  066h, 08Bh, 01Dh, 030h, 01Eh, 000h, 000h
loc_1_137F:
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax]
        bt     bx, ax
        ret
loc_1_1388:
        db  0CDh, 020h, 007h, 080h, 003h, 000h
        mov    eax, eax
loc_1_1390:
        db  066h, 08Bh, 01Dh, 032h, 01Eh, 000h, 000h
        jmp    SHORT loc_1_137F
        db  02Eh, 08Bh, 0C0h
loc_1_139C:
        db  0BEh, 0FEh, 013h, 000h, 000h
        mov    edi, dword ptr [ebp + 18h]
        shl    edi, 10h
        mov    di, word ptr [ebp + 14h]
        mov    edx, dword ptr [eax + 20h]
        mov    dword ptr [edx + 10h], edi
        movzx  eax, word ptr [ebp + 10h]
        db  0CDh, 020h, 001h, 000h, 001h, 000h
loc_1_13BB:
        cmp    eax, dword ptr [ebx + 0ch]
        je     SHORT loc_1_13CF
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        db  0CDh, 020h, 002h, 000h, 001h, 000h
        jne    SHORT loc_1_13BB
        ret
loc_1_13CF:
        movzx  ecx, word ptr [ebp + 4]
        mov    eax, 400000h
        cmp    ecx, 3
        jae    SHORT loc_1_13F3
        mov    eax, 1000h
        cmp    ecx, 2
        je     SHORT loc_1_13F3
        mov    eax, 10h
        cmp    ecx, 1
        je     SHORT loc_1_13F3
        mov    eax, ecx
loc_1_13F3:
        mov    ecx, 9
        db  0CDh, 020h, 014h, 080h, 001h, 000h
loc_1_13FE:
        test   dword ptr [ebx], 8000h
        je     SHORT loc_1_140C
        db  0CDh, 020h, 039h, 000h, 001h, 000h
loc_1_140C:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    edi, edx
        mov    cx, word ptr [edi + 0ch]
        mov    word ptr [ebp], cx
        mov    cx, word ptr [edi + 0eh]
        mov    word ptr [ebp + 38h], cx
        movzx  edx, word ptr [edi + 10h]
        movzx  ecx, word ptr [edi + 12h]
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  0CDh, 020h, 01Ah, 080h, 001h, 000h
        add    byte ptr [eax], al
VPICD_Virtual_EOI:
        pushad
        cli
        mov    eax, dword ptr [eax]
        mov    ebp, dword ptr [ebx + 8]
        mov    edi, ebx
        db  003h, 03Dh, 0A8h, 025h, 000h, 000h
        call   sub_1_14EF
        popad
        ret
loc_1_147A:
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_1480:
        db  08Bh, 03Dh, 0A8h, 025h, 000h, 000h
        lea    edi, [edi + ebx + 1]
        cmp    ecx, 4
        je     SHORT loc_1_14CB
        ja     SHORT loc_1_147A
loc_1_1491:
        cli
        test   byte ptr [edi + 0ah], 1
        je     SHORT loc_1_149D
        mov    al, byte ptr [edi + 12h]
        sti
        ret
loc_1_149D:
        mov    al, 0ah
        out    dx, al
        jmp    SHORT loc_1_14A2
loc_1_14A2:
        in     al, dx
        mov    bl, byte ptr [edi + 10h]
        mov    ecx, edx
        shr    ecx, 7
        db  08Ah, 089h, 034h, 01Eh, 000h, 000h
        and    al, cl
        not    cl
        and    bl, cl
        or     al, bl
        sti
        ret
        db  090h
loc_1_14BC:
        mov    edi, ebx
        db  003h, 03Dh, 0A8h, 025h, 000h, 000h
        cmp    ecx, 4
        jb     SHORT loc_1_1491
        ja     SHORT loc_1_147A
loc_1_14CB:
        cli
        cmp    al, 20h
        jne    SHORT loc_1_153F
loc_1_14D0:
        movzx  eax, byte ptr [edi + 12h]
        bsf    eax, eax
        je     NEAR PTR loc_1_156B
loc_1_14DD:
        db  08Bh, 004h, 085h, 068h, 025h, 000h, 000h
        cmp    edx, 20h
        je     SHORT sub_1_14EF
        add    eax, 200h
        dec    edi
sub_1_14EF:
        push   edi
        push   eax
        mov    dx, word ptr [eax + 6]
        mov    eax, dword ptr [eax + 18h]
loc_1_14F8:
        mov    ecx, dword ptr [eax + 14h]
        jecxz  loc_1_1537
        push   eax
        push   ebx
        push   edx
        call   ecx
        cli
        pop    edx
        pop    ebx
        pop    eax
        jae    SHORT loc_1_150D
        test   dl, 10h
        jne    SHORT loc_1_1533
loc_1_150D:
        mov    eax, dword ptr [eax + 8]
        test   eax, eax
        jne    SHORT loc_1_14F8
        pop    eax
        pop    edi
        mov    cx, word ptr [eax + 4]
        not    ecx
        and    word ptr [edi + 12h], cx
        jne    SHORT loc_1_152C
        db  08Bh, 015h, 0ACh, 025h, 000h, 000h
        and    byte ptr [edx + ebx], 0dfh
loc_1_152C:
        mov    edx, ebx
        jmp    NEAR PTR sub_1_0764
loc_1_1533:
        pop    eax
        pop    edi
        jmp    SHORT loc_1_152C
loc_1_1537:
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        jmp    SHORT loc_1_150D
loc_1_153F:
        test   al, 10h
        jne    SHORT loc_1_156D
        test   al, 8
        je     SHORT loc_1_157D
        test   al, 2
        je     SHORT loc_1_1557
        and    byte ptr [edi + 0ah], 0feh
        test   al, 1
        je     SHORT loc_1_1557
        or     byte ptr [edi + 0ah], 1
loc_1_1557:
        test   al, 40h
        je     SHORT loc_1_1563
        test   al, 20h
        jne    NEAR PTR loc_1_1613
loc_1_1563:
        test   al, 4
        jne    NEAR PTR loc_1_1613
loc_1_156B:
        sti
        ret
loc_1_156D:
        and    al, 0bh
        cmp    al, 1
        jne    NEAR PTR loc_1_1613
        mov    byte ptr [edi + 8], 4
        sti
        ret
loc_1_157D:
        movzx  ecx, al
        and    eax, 7
        sub    ecx, eax
        cmp    ecx, 60h
        je     NEAR PTR loc_1_14DD
        cmp    ecx, 20h
        je     NEAR PTR loc_1_14D0
        sti
        ret
loc_1_1599:
        db  0CDh, 020h, 094h, 080h, 001h, 000h
        nop
loc_1_15A0:
        db  08Bh, 03Dh, 0A8h, 025h, 000h, 000h
        lea    edi, [edi + ebx + 1]
        cmp    ecx, 4
        je     SHORT loc_1_15C7
        ja     SHORT loc_1_1599
loc_1_15B1:
        mov    al, byte ptr [edi + 0ch]
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_15B8:
        mov    edi, ebx
        db  003h, 03Dh, 0A8h, 025h, 000h, 000h
        cmp    ecx, 4
        jb     SHORT loc_1_15B1
        ja     SHORT loc_1_1599
loc_1_15C7:
        cli
        movzx  ecx, byte ptr [edi + 8]
        db  0FFh, 0A1h, 0D0h, 026h, 000h, 000h
loc_1_15D2:
        cmp    edx, 21h
        jne    SHORT loc_1_15DC
        mov    ah, byte ptr [edi + 0dh]
        jmp    SHORT loc_1_15E1
loc_1_15DC:
        mov    ah, al
        mov    al, byte ptr [edi + 0bh]
loc_1_15E1:
        xor    esi, esi
        call   sub_1_0650
        sti
        ret
loc_1_15EA:
        and    al, 0f8h
        mov    byte ptr [edi + 14h], al
        mov    cl, 8
        jmp    SHORT loc_1_160E
loc_1_15F3:
        mov    cl, 0ch
        cmp    edx, 21h
        jne    SHORT loc_1_1600
        cmp    al, 4
        jne    SHORT loc_1_1613
        jmp    SHORT loc_1_160E
loc_1_1600:
        cmp    al, 2
        jne    SHORT loc_1_1613
        jmp    SHORT loc_1_160E
loc_1_1606:
        and    al, 1bh
        cmp    al, 1
        jne    SHORT loc_1_1613
        mov    cl, 0
loc_1_160E:
        mov    byte ptr [edi + 8], cl
        sti
        ret
loc_1_1613:
        sti
        ret
        db  000h, 000h, 000h
VPICD_Get_Version:
        mov    eax, 40ah
        mov    ecx, 0fh
        mov    ebx, 1
        clc
        ret
VPICD_Virtualize_IRQ:
        pushad
        pushfd
        movzx  ecx, word ptr [edi]
        cmp    ecx, 0fh
        ja     NEAR PTR loc_1_1809
        cli
        test   byte ptr [edi + 2], 20h
        je     SHORT loc_1_1642
        or     byte ptr [edi + 2], 2
loc_1_1642:
        db  08Bh, 034h, 08Dh, 068h, 025h, 000h, 000h
        test   byte ptr [esi + 6], 4
        je     NEAR PTR loc_1_1717
        test   byte ptr [esi + 6], 2
        je     NEAR PTR loc_1_1809
        test   byte ptr [edi + 2], 2
        je     NEAR PTR loc_1_1809
        push   esi
        movzx  ebx, word ptr [esi + 6]
        and    ebx, 100h
        movzx  esi, word ptr [edi + 2]
        and    esi, 100h
        cmp    ebx, esi
        pop    esi
        jne    NEAR PTR loc_1_1809
loc_1_1685:
        mov    ebx, dword ptr [esi + 18h]
        mov    ebx, dword ptr [ebx + 4]
        shl    ebx, 1
        jb     NEAR PTR loc_1_1809
        push   esi
        db  08Bh, 035h, 040h, 01Eh, 000h, 000h
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        pop    esi
        jb     NEAR PTR loc_1_1809
        mov    dword ptr [eax], esi
        mov    edx, eax
        xchg   dword ptr [esi + 18h], edx
        mov    dword ptr [eax + 8], edx
        mov    dword ptr [eax + 4], ebx
        push   edx
        xor    edx, edx
        mov    cx, word ptr [edi]
        bts    edx, ecx
        db  066h, 085h, 015h, 0F4h, 025h, 000h, 000h
        je     SHORT loc_1_1710
        db  066h, 085h, 015h, 00Eh, 027h, 000h, 000h
        jne    SHORT loc_1_1710
        test   word ptr [edi + 2], 100h
        je     SHORT loc_1_1710
        db  0FEh, 081h, 0E0h, 026h, 000h, 000h
        jne    SHORT loc_1_1710
        push   edi
        push   eax
        db  08Bh, 03Ch, 08Dh, 0F4h, 025h, 000h, 000h
        mov    eax, 60b0h
        add    ah, cl
        and    ah, 0f7h
        cmp    word ptr [edi - 2], ax
        jne    SHORT loc_1_1713
        mov    eax, 0e9h
        cmp    al, byte ptr [edi]
        jne    SHORT loc_1_1713
        mov    eax, dword ptr [edi + 1]
        db  087h, 004h, 08Dh, 034h, 026h, 000h, 000h
        mov    dword ptr [edi + 1], eax
        pop    eax
        pop    edi
loc_1_1710:
        pop    edx
        jmp    SHORT loc_1_177D
loc_1_1713:
        pop    eax
        pop    edi
        jmp    SHORT loc_1_1710
loc_1_1717:
        mov    bx, word ptr [edi + 2]
        and    bx, 133h
        or     bx, 0ch
        mov    word ptr [esi + 6], bx
        mov    eax, dword ptr [esi + 18h]
        test   bx, 2
        je     SHORT loc_1_1776
        xor    ecx, ecx
        mov    dword ptr [eax + 18h], ecx
        db  0C7h, 040h, 00Ch, 0E0h, 002h, 000h, 000h
        db  0C7h, 040h, 014h, 038h, 003h, 000h, 000h
        db  0C7h, 040h, 01Ch, 05Eh, 004h, 000h, 000h
        cmp    dword ptr [esi + 8], 0
        je     NEAR PTR loc_1_1685
        mov    dword ptr [eax + 14h], ecx
        mov    dword ptr [eax + 1ch], ecx
        db  0C7h, 040h, 00Ch, 092h, 004h, 000h, 000h
        mov    ebx, dword ptr [esi + 8]
        cmp    ebx, -1
        je     SHORT loc_1_1771
        db  0CDh, 020h, 003h, 000h, 003h, 000h
loc_1_1771:
        jmp    NEAR PTR loc_1_1685
loc_1_1776:
        mov    dword ptr [esi + 1ch], 0
loc_1_177D:
        mov    ecx, dword ptr [edi + 18h]
        cmp    ecx, dword ptr [esi + 1ch]
        jb     SHORT loc_1_1788
        mov    dword ptr [esi + 1ch], ecx
loc_1_1788:
        mov    cx, word ptr [edi + 2]
        and    ecx, 20h
        je     SHORT loc_1_17A9
        or     word ptr [esi + 6], cx
        push   eax
loc_1_1796:
        mov    ecx, dword ptr [eax + 8]
        test   ecx, ecx
        je     SHORT loc_1_17A1
        mov    eax, ecx
        jmp    SHORT loc_1_1796
loc_1_17A1:
        db  0C7h, 040h, 00Ch, 043h, 003h, 000h, 000h
        pop    eax
loc_1_17A9:
        test   word ptr [edi + 2], 4
        je     SHORT loc_1_17B7
        mov    ecx, dword ptr [edi + 1ch]
        mov    dword ptr [eax + 24h], ecx
loc_1_17B7:
        lea    esi, [edi + 4]
        lea    edi, [eax + 0ch]
        mov    ecx, 5
        cld
        rep movsd
        mov    dword ptr [eax + 28h], 51524953h
        mov    esi, dword ptr [eax + 8]
        test   esi, esi
        jne    SHORT loc_1_17F7
loc_1_17D3:
        mov    esi, dword ptr [eax]
        mov    esi, dword ptr [esi]
        db  066h, 00Fh, 0A3h, 035h, 032h, 01Eh, 000h, 000h
        jae    SHORT loc_1_17E7
        db  0CDh, 020h, 004h, 000h, 003h, 000h
loc_1_17E7:
        popfd
        mov    dword ptr [esp + 1ch], eax
        mov    edi, dword ptr [esp]
        call   sub_1_1CD4
        popad
        clc
        ret
loc_1_17F7:
        db  081h, 0FEh, 068h, 022h, 000h, 000h
        jb     SHORT loc_1_17E7
        db  081h, 0FEh, 0FCh, 024h, 000h, 000h
        jbe    SHORT loc_1_17D3
        jmp    SHORT loc_1_17E7
loc_1_1809:
        popfd
        popad
        stc
        ret
        db  02Eh, 08Bh, 0C0h
VPICD_Set_Int_Request:
        push   edx
        push   esi
        pushfd
        cli
        mov    esi, ebx
        db  003h, 035h, 0A8h, 025h, 000h, 000h
        mov    edx, dword ptr [eax]
        mov    edx, dword ptr [edx]
        push   eax
        mov    eax, dword ptr [eax + 4]
        or     dword ptr [esi + edx*4 + 20h], eax
        pop    eax
        bts    word ptr [esi + 10h], dx
        jb     SHORT loc_1_184E
        cmp    dword ptr [esi + 4], 0
        jne    SHORT loc_1_184E
        cmp    byte ptr [esi + 1eh], 0
        jne    SHORT loc_1_184E
        mov    edx, ebx
        push   esi
        db  0BEh, 050h, 007h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        pop    edx
        mov    dword ptr [edx + 4], esi
loc_1_184E:
        popfd
        pop    esi
        pop    edx
        ret
        db  08Bh, 0C0h
VPICD_Clear_Int_Request:
        push   edx
        push   esi
        pushfd
        cli
        mov    esi, ebx
        db  003h, 035h, 0A8h, 025h, 000h, 000h
        mov    edx, dword ptr [eax]
        mov    edx, dword ptr [edx]
        push   eax
        mov    eax, dword ptr [eax + 4]
        not    eax
        and    dword ptr [esi + edx*4 + 20h], eax
        pop    eax
        jne    SHORT loc_1_1896
        btr    word ptr [esi + 10h], dx
        jae    SHORT loc_1_1896
        cmp    dword ptr [esi + 4], 0
        jne    SHORT loc_1_1896
        cmp    byte ptr [esi + 1eh], 0
        jne    SHORT loc_1_1896
        mov    edx, ebx
        push   esi
        db  0BEh, 050h, 007h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        pop    edx
        mov    dword ptr [edx + 4], esi
loc_1_1896:
        popfd
        pop    esi
        pop    edx
        ret
        db  08Bh, 0C0h
VPICD_Get_Complete_Status:
        push   eax
        push   ebx
        xor    ecx, ecx
        cmp    dword ptr [eax + 28h], 51524953h
        jne    NEAR PTR loc_1_1944
        cmp    dword ptr [ebx + 10h], 62634d56h
        jne    NEAR PTR loc_1_1944
        push   eax
        push   edx
        mov    edx, dword ptr [eax + 4]
        mov    eax, dword ptr [eax]
        cmp    dword ptr [eax + 8], ecx
        jne    SHORT loc_1_18C7
        inc    ecx
loc_1_18C7:
        cmp    dword ptr [eax + 8], ebx
        jne    SHORT loc_1_18CE
        mov    cl, 2
loc_1_18CE:
        db  003h, 01Dh, 0A8h, 025h, 000h, 000h
        mov    eax, dword ptr [eax]
        add    ecx, ecx
        test   dword ptr [ebx + eax*4 + 20h], edx
        je     SHORT loc_1_18DF
        inc    ecx
loc_1_18DF:
        pop    edx
        pop    eax
        db  0CDh, 020h, 007h, 000h, 003h, 000h
        adc    ecx, ecx
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax]
        bt     word ptr [ebx + 10h], ax
        adc    ecx, ecx
        bt     word ptr [ebx + 0ch], ax
        adc    ecx, ecx
        db  066h, 00Fh, 0A3h, 005h, 032h, 01Eh, 000h, 000h
        adc    ecx, ecx
        db  066h, 00Fh, 0A3h, 005h, 030h, 01Eh, 000h, 000h
        adc    ecx, ecx
        jmp    SHORT loc_1_1934
        db  02Eh, 08Bh, 0C0h
VPICD_Get_Status:
        push   eax
        push   ebx
        xor    ecx, ecx
        cmp    dword ptr [eax + 28h], 51524953h
        jne    SHORT loc_1_1944
        cmp    dword ptr [ebx + 10h], 62634d56h
        jne    SHORT loc_1_1944
        mov    eax, dword ptr [eax]
        db  003h, 01Dh, 0A8h, 025h, 000h, 000h
        mov    eax, dword ptr [eax]
loc_1_1934:
        bt     word ptr [ebx + 12h], ax
        adc    ecx, ecx
        cmp    word ptr [ebx + eax*2 + 60h], 1
        cmc
        adc    ecx, ecx
loc_1_1944:
        pop    ebx
        pop    eax
        ret
VPICD_Test_Phys_Request:
        push   eax
        push   edx
        mov    eax, dword ptr [eax]
        mov    edx, dword ptr [eax]
        mov    al, 0ah
        cmp    edx, 8
        jb     SHORT loc_1_195E
        out    0a0h, al
        jmp    SHORT loc_1_1958
loc_1_1958:
        in     al, 0a0h
        mov    ah, al
        jmp    SHORT loc_1_1964
loc_1_195E:
        out    20h, al
        jmp    SHORT loc_1_1962
loc_1_1962:
        in     al, 20h
loc_1_1964:
        bt     eax, edx
        pop    edx
        pop    eax
        ret
        db  08Bh, 0C0h
VPICD_Phys_EOI:
        push   eax
        pushfd
        mov    eax, dword ptr [eax]
        mov    ax, word ptr [eax + 4]
        cli
        test   al, al
        je     SHORT loc_1_199E
        not    al
        db  020h, 005h, 032h, 01Eh, 000h, 000h
        db  0A0h, 032h, 01Eh, 000h, 000h
        db  00Ah, 005h, 030h, 01Eh, 000h, 000h
        db  038h, 005h, 03Ah, 01Eh, 000h, 000h
        je     SHORT loc_1_199B
        db  0A2h, 03Ah, 01Eh, 000h, 000h
        out    21h, al
loc_1_199B:
        popfd
        pop    eax
        ret
loc_1_199E:
        not    ah
        db  020h, 025h, 033h, 01Eh, 000h, 000h
        db  0A0h, 033h, 01Eh, 000h, 000h
        db  00Ah, 005h, 031h, 01Eh, 000h, 000h
        db  038h, 005h, 03Bh, 01Eh, 000h, 000h
        je     SHORT loc_1_19C0
        db  0A2h, 03Bh, 01Eh, 000h, 000h
        out    0a1h, al
loc_1_19C0:
        popfd
        pop    eax
        ret
VPICD_Convert_IRQ_To_Int:
        cmp    dword ptr [ebx + 10h], 62634d56h
        jne    SHORT loc_1_19E6
        cmp    eax, 0fh
        ja     SHORT loc_1_19E6
        push   ecx
        db  08Bh, 00Dh, 0A8h, 025h, 000h, 000h
        btr    eax, 3
        adc    ecx, 0
        add    al, byte ptr [ecx + ebx + 14h]
        pop    ecx
        clc
        ret
loc_1_19E6:
        stc
        ret
VPICD_Convert_Int_To_IRQ:
        cmp    dword ptr [ebx + 10h], 62634d56h
        jne    SHORT loc_1_1A1E
        cmp    eax, 0ffh
        ja     SHORT loc_1_1A1E
        push   ecx
        push   ebx
        push   edx
        db  003h, 01Dh, 0A8h, 025h, 000h, 000h
        mov    dx, word ptr [ebx + 14h]
        xor    ecx, ecx
        call   sub_1_1A20
        jae    SHORT loc_1_1A1A
        mov    ecx, 8
        mov    dl, dh
        call   sub_1_1A20
loc_1_1A1A:
        pop    edx
        pop    ebx
        pop    ecx
        ret
loc_1_1A1E:
        stc
        ret
sub_1_1A20:
        sub    al, dl
        jb     SHORT loc_1_1A2C
        cmp    al, 7
        ja     SHORT loc_1_1A2C
        add    eax, ecx
        clc
        ret
loc_1_1A2C:
        add    al, dl
        stc
        ret
VPICD_Get_IRQ_Complete_Status:
        cmp    eax, 0fh
        ja     SHORT loc_1_1A5E
        push   eax
        db  08Bh, 004h, 085h, 068h, 025h, 000h, 000h
        push   eax
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, dword ptr [eax + 18h]
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        and    ecx, 0ffffff7fh
        pop    ebx
        pop    eax
        bt     word ptr [eax + 6], 2
        pop    eax
        ret
loc_1_1A5E:
        xor    ecx, ecx
        ret
VPICD_Convert_Handle_To_IRQ:
        cmp    dword ptr [eax + 28h], 51524953h
        jne    SHORT loc_1_1A6F
        mov    esi, dword ptr [eax]
        mov    esi, dword ptr [esi]
        ret
loc_1_1A6F:
        or     esi, 0ffffffffh
        ret
        db  090h
VPICD_Physically_Mask:
        push   eax
        push   edx
        pushfd
        cli
        mov    eax, dword ptr [eax]
        and    word ptr [eax + 6], 0fff7h
        mov    dx, word ptr [eax + 4]
        db  066h, 009h, 015h, 030h, 01Eh, 000h, 000h
        db  066h, 0A1h, 030h, 01Eh, 000h, 000h
        db  066h, 00Bh, 005h, 032h, 01Eh, 000h, 000h
        db  066h, 03Bh, 005h, 03Ah, 01Eh, 000h, 000h
        je     SHORT loc_1_1ABD
        db  066h, 08Bh, 015h, 03Ah, 01Eh, 000h, 000h
        db  066h, 0A3h, 03Ah, 01Eh, 000h, 000h
        cmp    al, dl
        je     SHORT loc_1_1AB9
        out    21h, al
        cmp    ah, dh
        je     SHORT loc_1_1ABD
        jmp    SHORT loc_1_1AB9
loc_1_1AB9:
        mov    al, ah
        out    0a1h, al
loc_1_1ABD:
        popfd
        pop    edx
        pop    eax
        ret
        db  02Eh, 08Bh, 0C0h
VPICD_Physically_Unmask:
        push   eax
        push   edx
        pushfd
        cli
        mov    eax, dword ptr [eax]
        and    word ptr [eax + 6], 0fff7h
        mov    dx, word ptr [eax + 4]
        not    edx
        db  066h, 021h, 015h, 030h, 01Eh, 000h, 000h
        db  066h, 0A1h, 030h, 01Eh, 000h, 000h
        db  066h, 00Bh, 005h, 032h, 01Eh, 000h, 000h
        db  066h, 03Bh, 005h, 03Ah, 01Eh, 000h, 000h
        je     SHORT loc_1_1B0F
        db  066h, 08Bh, 015h, 03Ah, 01Eh, 000h, 000h
        db  066h, 0A3h, 03Ah, 01Eh, 000h, 000h
        cmp    al, dl
        je     SHORT loc_1_1B0B
        out    21h, al
        cmp    ah, dh
        je     SHORT loc_1_1B0F
        jmp    SHORT loc_1_1B0B
loc_1_1B0B:
        mov    al, ah
        out    0a1h, al
loc_1_1B0F:
        popfd
        pop    edx
        pop    eax
        ret
        db  090h
VPICD_Set_Auto_Masking:
        push   eax
        push   edx
        pushfd
        cli
        mov    eax, dword ptr [eax]
        or     word ptr [eax + 6], 8
        mov    dx, word ptr [eax + 4]
        cmp    word ptr [eax + 16h], 0
        je     SHORT loc_1_1B35
        not    edx
        db  066h, 021h, 015h, 030h, 01Eh, 000h, 000h
        jmp    SHORT loc_1_1B3C
loc_1_1B35:
        db  066h, 009h, 015h, 030h, 01Eh, 000h, 000h
loc_1_1B3C:
        db  066h, 0A1h, 030h, 01Eh, 000h, 000h
        db  066h, 00Bh, 005h, 032h, 01Eh, 000h, 000h
        db  066h, 03Bh, 005h, 03Ah, 01Eh, 000h, 000h
        je     SHORT loc_1_1B6F
        db  066h, 08Bh, 015h, 03Ah, 01Eh, 000h, 000h
        db  066h, 0A3h, 03Ah, 01Eh, 000h, 000h
        cmp    al, dl
        je     SHORT loc_1_1B6B
        out    21h, al
        cmp    ah, dh
        je     SHORT loc_1_1B6F
        jmp    SHORT loc_1_1B6B
loc_1_1B6B:
        mov    al, ah
        out    0a1h, al
loc_1_1B6F:
        popfd
        pop    edx
        pop    eax
        ret
VPICD_Call_When_Hw_Int:
        db  087h, 035h, 03Ch, 01Eh, 000h, 000h
loc_1_1B79:
        ret
VPICD_Auto_Mask_At_Inst_Swap:
        cmp    dword ptr [eax + 28h], 51524953h
        jne    SHORT loc_1_1B9E
        push   edx
        mov    edx, dword ptr [eax]
        and    word ptr [edx + 6], 0fff7h
        mov    edx, dword ptr [edx]
        db  066h, 00Fh, 0ABh, 015h, 038h, 01Eh, 000h, 000h
        pop    edx
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        clc
        ret
loc_1_1B9E:
        stc
        ret
VPICD_Begin_Inst_Page_Swap:
        cli
        push   eax
        db  066h, 0A1h, 038h, 01Eh, 000h, 000h
        db  066h, 009h, 005h, 030h, 01Eh, 000h, 000h
        jmp    SHORT loc_1_1BC5
        db  02Eh, 08Bh, 0C0h
VPICD_End_Inst_Page_Swap:
        cli
        push   eax
        db  066h, 0A1h, 038h, 01Eh, 000h, 000h
        not    eax
        db  066h, 021h, 005h, 030h, 01Eh, 000h, 000h
loc_1_1BC5:
        push   edx
        db  066h, 0A1h, 030h, 01Eh, 000h, 000h
        db  066h, 00Bh, 005h, 032h, 01Eh, 000h, 000h
        db  066h, 03Bh, 005h, 03Ah, 01Eh, 000h, 000h
        je     SHORT loc_1_1BF9
        db  066h, 08Bh, 015h, 03Ah, 01Eh, 000h, 000h
        db  066h, 0A3h, 03Ah, 01Eh, 000h, 000h
        cmp    al, dl
        je     SHORT loc_1_1BF5
        out    21h, al
        cmp    ah, dh
        je     SHORT loc_1_1BF9
        jmp    SHORT loc_1_1BF5
loc_1_1BF5:
        mov    al, ah
        out    0a1h, al
loc_1_1BF9:
        pop    edx
        pop    eax
        sti
        ret
VPICD_VM_SlavePIC_Mask_Change:
        ret
        db  000h, 000h, 000h, 000h
loc_1_1C02:
        push   eax
        db  0A1h, 0FEh, 01Bh, 000h, 000h
        cmp    eax, 8
        jb     SHORT loc_1_1C16
        sub    eax, 8
        or     al, 60h
        out    0a0h, al
        mov    al, 2
loc_1_1C16:
        or     al, 60h
        out    20h, al
        mov    al, 0ffh
        out    0a1h, al
        out    21h, al
        pop    eax
        iretd
sub_1_1C22:
        push   ebx
        push   esi
        sub    esp, 8
        sidt   [esp + 2]
        pop    ebx
        pop    ebx
        db  0A3h, 0FEh, 01Bh, 000h, 000h
        shl    eax, 3
        add    ebx, eax
        add    ebx, 280h
        db  0BEh, 002h, 01Ch, 000h, 000h
        xchg   word ptr [ebx], si
        ror    esi, 10h
        xchg   word ptr [ebx + 6], si
        ror    esi, 10h
        sti
        nop
        nop
        nop
        cli
        mov    word ptr [ebx], si
        ror    esi, 10h
        mov    word ptr [ebx + 6], si
        pop    esi
        pop    ebx
        ret
VPICD_Service_25:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ebx
        push   ecx
        push   edx
        mov    bx, word ptr [ebp + 8]
        or     bx, bx
        je     SHORT loc_1_1CCE
        pushfd
        cli
        in     al, 0a1h
        mov    ah, al
        in     al, 21h
        push   eax
        mov    al, 0ffh
        out    0a1h, al
        mov    ecx, 1
loc_1_1C84:
        test   cl, bl
        je     SHORT loc_1_1C9A
        mov    al, cl
        not    al
        out    21h, al
        bsf    eax, ecx
        call   sub_1_1C22
        mov    al, 0ffh
        out    21h, al
loc_1_1C9A:
        shl    cl, 1
        jne    SHORT loc_1_1C84
        mov    bl, bh
        mov    cl, 1
loc_1_1CA2:
        test   cl, bl
        je     SHORT loc_1_1CC2
        mov    al, 0fbh
        out    21h, al
        mov    al, cl
        not    al
        out    0a1h, al
        xchg   ch, cl
        bsf    eax, ecx
        call   sub_1_1C22
        xchg   ch, cl
        mov    al, 0ffh
        out    0a1h, al
        out    21h, al
loc_1_1CC2:
        shl    cl, 1
        jne    SHORT loc_1_1CA2
        pop    eax
        out    21h, al
        mov    al, ah
        out    0a1h, al
        popfd
loc_1_1CCE:
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        leave
        ret
sub_1_1CD4:
        movzx  eax, word ptr [edi]
        db  00Fh, 0ABh, 005h, 0FAh, 026h, 000h, 000h
        jb     SHORT loc_1_1CFA
        db  0FEh, 080h, 0FEh, 026h, 000h, 000h
        test   byte ptr [edi + 2], 2
        je     SHORT loc_1_1CF3
        db  00Fh, 0ABh, 005h, 0FCh, 026h, 000h, 000h
loc_1_1CF3:
        db  00Fh, 0ABh, 005h, 012h, 027h, 000h, 000h
loc_1_1CFA:
        ret
sub_1_1CFB:
        cmp    edi, 10h
        jae    SHORT loc_1_1D09
        db  00Fh, 0A3h, 03Dh, 0FAh, 026h, 000h, 000h
        jae    SHORT loc_1_1D0A
loc_1_1D09:
        ret
loc_1_1D0A:
        db  00Fh, 0ABh, 03Dh, 014h, 027h, 000h, 000h
        jb     SHORT loc_1_1D09
        db  080h, 00Dh, 0F8h, 026h, 000h, 000h, 001h
        ret
sub_1_1D1B:
        pushad
        mov    ecx, 10h
        mov    eax, edi
        db  0BFh, 068h, 025h, 000h, 000h
        cld
        repne scasd
        jne    SHORT loc_1_1D53
        neg    ecx
        add    ecx, 0fh
        db  00Fh, 0A3h, 00Dh, 012h, 027h, 000h, 000h
        jae    SHORT loc_1_1D53
        db  0FEh, 089h, 0FEh, 026h, 000h, 000h
        jne    SHORT loc_1_1D53
        db  066h, 00Fh, 0B3h, 00Dh, 0FAh, 026h, 000h, 000h
        db  066h, 00Fh, 0B3h, 00Dh, 0FCh, 026h, 000h, 000h
loc_1_1D53:
        popad
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1D6C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  018h, 016h, 000h, 000h, 029h, 016h, 000h, 000h, 010h, 018h, 000h, 000h, 054h, 018h, 000h, 000h
        db  06Ch, 019h, 000h, 000h, 09Ch, 018h, 000h, 000h, 014h, 019h, 000h, 000h, 047h, 019h, 000h, 000h
        db  074h, 01Ah, 000h, 000h, 0C4h, 01Ah, 000h, 000h, 014h, 01Bh, 000h, 000h, 030h, 01Ah, 000h, 000h
        db  061h, 01Ah, 000h, 000h, 0C3h, 019h, 000h, 000h, 0E8h, 019h, 000h, 000h, 073h, 01Bh, 000h, 000h
        db  000h, 000h, 000h, 000h, 07Ah, 000h, 000h, 000h, 07Ah, 01Bh, 000h, 000h, 0A0h, 01Bh, 000h, 000h
        db  0B4h, 01Bh, 000h, 000h, 064h, 014h, 000h, 000h, 071h, 001h, 000h, 000h, 0ECh, 002h, 000h, 000h
        db  0FDh, 01Bh, 000h, 000h, 062h, 01Ch, 000h, 000h, 052h, 004h, 000h, 000h, 059h, 004h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 003h, 000h, 004h, 00Ah, 000h, 000h, 056h, 050h, 049h, 043h
        db  044h, 020h, 020h, 020h, 000h, 000h, 000h, 00Ch, 09Ch, 004h, 000h, 000h, 050h, 011h, 000h, 000h
        db  050h, 011h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  070h, 01Dh, 000h, 000h, 01Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  0FFh, 0FFh, 000h, 000h, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1E3C:
        db  079h, 01Bh
        add    byte ptr [eax], al
loc_1_1E40:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1E44:
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
loc_1_1E64:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1E68:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        push   0f4000022h
        add    dword ptr [eax], eax
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
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        db  000h, 094h, 022h, 000h, 000h, 0F4h, 001h
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
loc_1_1EE8:
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    al, 0
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        shl    byte ptr [edx], 0
        add    ah, dh
        add    dword ptr [eax], eax
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
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        db  000h, 0ECh
        and    al, byte ptr [eax]
        add    ah, dh
        add    dword ptr [eax], eax
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
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        adc    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        sbb    byte ptr [ebx], ah
        add    byte ptr [eax], al
        hlt
        add    dword ptr [eax], eax
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
        add    byte ptr [20000000h], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        db  000h, 044h, 023h, 000h
        add    ah, dh
        add    dword ptr [eax], eax
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
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        jo     SHORT loc_1_2025
        add    byte ptr [eax], al
        hlt
        add    dword ptr [eax], eax
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
loc_1_2025:
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 800h], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        db  000h, 09Ch, 023h, 000h, 000h, 0F4h, 001h
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
sub_1_2068:
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        enter  23h, 0
        hlt
        add    dword ptr [eax], eax
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
        add    byte ptr [ecx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    cl, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        db  000h, 0F4h
        and    eax, dword ptr [eax]
        add    ah, dh
        add    dword ptr [eax], eax
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
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 8
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        db  000h, 020h
        and    al, 0
        add    ah, dh
        add    dword ptr [eax], eax
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
        add    byte ptr [ebx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        db  000h, 04Ch, 024h, 000h
        add    ah, dh
        add    dword ptr [eax], eax
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
        add    byte ptr [eax + eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        db  078h, 024h
        add    byte ptr [eax], al
        hlt
        add    dword ptr [eax], eax
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
        add    byte ptr [0], cl
        and    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        db  000h, 0A4h, 024h, 000h, 000h, 0F4h, 001h
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
loc_1_21E8:
        push   cs
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    eax
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        shl    byte ptr [eax + eax], 1
        add    ah, dh
        add    dword ptr [eax], eax
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
        add    byte ptr [edi], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        cld
        and    al, 0
        add    ah, dh
        add    dword ptr [eax], eax
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
        db  000h, 068h, 01Eh
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        loopne loc_1_2278
        add    byte ptr [eax], al
loc_1_2278:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    al, 3
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        db  000h, 054h, 004h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 49h], dl
        push   edx
        push   ecx
loc_1_2294:
        test   al, 1eh
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        loopne loc_1_22A4
        add    byte ptr [eax], al
loc_1_22A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    al, 3
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        db  000h, 054h, 004h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 49h], dl
        push   edx
        push   ecx
loc_1_22C0:
        db  0E8h, 01Eh, 000h, 000h, 001h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0E0h
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 02Ch, 003h
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        db  000h, 054h, 004h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 49h], dl
        push   edx
        push   ecx
loc_1_22EC:
        sub    byte ptr [edi], bl
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        loopne loc_1_22FC
        add    byte ptr [eax], al
loc_1_22FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    al, 3
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        db  000h, 054h, 004h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 49h], dl
        push   edx
        push   ecx
loc_1_2318:
        push   100001fh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0E0h
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 02Ch, 003h
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        db  000h, 054h, 004h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 49h], dl
        push   edx
        push   ecx
loc_1_2344:
        test   al, 1fh
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        loopne loc_1_2354
        add    byte ptr [eax], al
loc_1_2354:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    al, 3
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        db  000h, 054h, 004h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 49h], dl
        push   edx
        push   ecx
loc_1_2370:
        db  0E8h, 01Fh, 000h, 000h, 001h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0E0h
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 02Ch, 003h
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        db  000h, 054h, 004h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 49h], dl
        push   edx
        push   ecx
loc_1_239C:
        sub    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        loopne loc_1_23AC
        add    byte ptr [eax], al
loc_1_23AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    al, 3
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        db  000h, 054h, 004h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 49h], dl
        push   edx
        push   ecx
loc_1_23C8:
        push   1000020h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0E0h
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 02Ch, 003h
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        db  000h, 054h, 004h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 49h], dl
        push   edx
        push   ecx
loc_1_23F4:
        test   al, 20h
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2400:
        loopne loc_1_2404
        add    byte ptr [eax], al
loc_1_2404:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2408:
        sub    al, 3
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        db  000h, 054h, 004h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 49h], dl
        push   edx
        push   ecx
loc_1_2420:
        db  0E8h, 020h, 000h, 000h, 001h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0E0h
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 02Ch, 003h
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        db  000h, 054h, 004h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 49h], dl
        push   edx
        push   ecx
loc_1_244C:
        sub    byte ptr [ecx], ah
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        loopne loc_1_245C
        add    byte ptr [eax], al
loc_1_245C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    al, 3
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        db  000h, 054h, 004h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 49h], dl
        push   edx
        push   ecx
loc_1_2478:
        push   1000021h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0E0h
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 02Ch, 003h
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        db  000h, 054h, 004h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 49h], dl
        push   edx
        push   ecx
loc_1_24A4:
        test   al, 21h
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        loopne loc_1_24B4
        add    byte ptr [eax], al
loc_1_24B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    al, 3
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        db  000h, 054h, 004h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 49h], dl
        push   edx
        push   ecx
loc_1_24D0:
        db  0E8h, 021h, 000h, 000h, 001h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0E0h
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 02Ch, 003h
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        db  000h, 054h, 004h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 49h], dl
        push   edx
        push   ecx
loc_1_24FC:
        sub    byte ptr [edx], ah
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        loopne loc_1_250C
        add    byte ptr [eax], al
loc_1_250C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    al, 3
        add    byte ptr [eax], al
        dec    esp
        add    eax, dword ptr [eax]
        db  000h, 054h, 004h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 49h], dl
        push   edx
        push   ecx
loc_1_2528:
        db  068h, 01Eh, 000h, 000h, 0A8h
        push   ds
        add    byte ptr [eax], al
        db  0E8h, 01Eh, 000h, 000h, 068h
        and    byte ptr [eax], al
        db  000h, 0A8h, 020h, 000h, 000h, 0E8h
        and    byte ptr [eax], al
        db  000h, 028h
        and    dword ptr [eax], eax
        db  000h, 068h, 021h
        add    byte ptr [eax], al
        test   al, 21h
        add    byte ptr [eax], al
        db  0E8h, 021h, 000h, 000h, 028h
        and    al, byte ptr [eax]
        db  000h, 028h
        pop    ds
        add    byte ptr [eax], al
        db  068h, 01Fh, 000h, 000h, 0A8h
        pop    ds
        add    byte ptr [eax], al
        db  0E8h, 01Fh, 000h, 000h, 028h
        and    byte ptr [eax], al
        db  000h, 068h, 01Eh
        add    byte ptr [eax], al
        test   al, 1eh
        add    byte ptr [eax], al
        db  0E8h, 01Eh, 000h, 000h, 028h
        pop    ds
        add    byte ptr [eax], al
        db  068h, 01Fh, 000h, 000h, 0A8h
        pop    ds
        add    byte ptr [eax], al
        db  0E8h, 01Fh, 000h, 000h, 028h
        and    byte ptr [eax], al
        db  000h, 068h, 020h
        add    byte ptr [eax], al
        test   al, 20h
        add    byte ptr [eax], al
        db  0E8h, 020h, 000h, 000h, 028h
        and    dword ptr [eax], eax
        db  000h, 068h, 021h
        add    byte ptr [eax], al
        test   al, 21h
        add    byte ptr [eax], al
        db  0E8h, 021h, 000h, 000h, 028h
        and    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [ecx], al
        db  000h, 047h, 011h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0BCh, 002h, 000h, 000h, 050h
        push   cs
        add    byte ptr [eax], al
        inc    edi
        adc    dword ptr [eax], eax
        add    ah, dh
        add    dword ptr [eax], eax
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
        add    byte ptr [edi], cl
        add    byte ptr [eax], al
        db  000h
loc_1_25F4:
        clc
        fiadd  word ptr [eax]
        db  000h, 080h, 00Eh, 000h, 000h, 0B0h
        push   cs
        add    byte ptr [eax], al
        loopne loc_1_2610
        add    byte ptr [eax], al
        adc    byte ptr [edi], cl
        add    byte ptr [eax], al
        inc    eax
        sldt   word ptr [eax]
        jo     SHORT loc_1_261D
        add    byte ptr [eax], al
loc_1_2610:
        mov    al, 0fh
        add    byte ptr [eax], al
        db  0E0h, 00Fh
        add    byte ptr [eax], al
        adc    byte ptr [eax], dl
        add    byte ptr [eax], al
        inc    eax
loc_1_261D:
        adc    byte ptr [eax], al
        db  000h, 070h, 010h
        add    byte ptr [eax], al
        db  0A0h, 010h, 000h, 000h, 0D0h
        adc    byte ptr [eax], al
        db  000h, 000h
        adc    dword ptr [eax], eax
        db  000h, 040h, 011h
        add    byte ptr [eax], al
loc_1_2634:
        xchg   ecx, eax
        db  0FFh, 0FFh, 0FFh, 061h, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 031h, 0FFh, 0FFh, 0FFh, 001h
        db  0FFh, 0FFh, 0FFh, 0D1h, 0FEh, 0FFh, 0FFh, 0A1h, 0FEh, 0FFh, 0FFh, 061h, 0FEh, 0FFh, 0FFh, 02Bh
        db  0FEh, 0FFh, 0FFh, 0FBh, 0FDh, 0FFh, 0FFh, 0CBh, 0FDh, 0FFh, 0FFh, 09Bh, 0FDh, 0FFh, 0FFh, 06Bh
        db  0FDh, 0FFh, 0FFh, 03Bh, 0FDh, 0FFh, 0FFh, 00Bh, 0FDh, 0FFh, 0FFh, 0CBh, 0FCh, 0FFh, 0FFh
sub_1_2674:
        mov    ecx, dword ptr [edx]
        add    byte ptr [eax], al
        xchg   edx, eax
        or     al, byte ptr [eax]
        db  000h, 08Ch, 00Ah, 000h, 000h, 0A5h, 00Ah
        add    byte ptr [eax], al
loc_1_2684:
        inc    esi
        adc    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        db  000h, 0E4h
        adc    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    ah, dh
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 060h, 013h
        add    byte ptr [eax], al
        db  068h, 013h, 000h, 000h, 070h
        adc    eax, dword ptr [eax]
        db  000h, 078h, 013h
        add    byte ptr [eax], al
        mov    byte ptr [ebx], dl
        add    byte ptr [eax], al
        nop
        adc    eax, dword ptr [eax]
        db  000h, 09Ch, 013h, 000h, 000h, 0D2h, 015h
        add    byte ptr [eax], al
        db  0EAh, 015h, 000h, 000h, 0F3h, 015h, 000h
        db  000h, 006h
        push   ss
        add    byte ptr [eax], al
        db  0FEh, 0FEh, 0FEh, 0FEh, 0FEh, 0FEh, 0FEh, 0FEh, 0FEh, 0FEh, 0FEh, 0FEh, 0FEh, 0FEh, 0FEh, 0FEh
        db  016h, 00Eh, 000h, 000h, 010h, 00Eh, 000h, 000h, 000h, 000h
loc_1_26FA:
        add    al, 20h
loc_1_26FC:
        add    byte ptr [eax], al
loc_1_26FE:
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_1_270E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2712:
        add    al, 20h
loc_1_2714:
        test   byte ptr [eax], ah
        add    byte ptr [eax], al
_LTEXT ends
        end
