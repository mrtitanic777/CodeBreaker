; vpowerd_lcode_01.asm -- VPOWERD object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

VPOWERD_Control:
        cmp    eax, 23h
        db  00Fh, 084h, 014h, 000h, 0FFh, 0FFh
        cmp    eax, 0
        db  00Fh, 084h, 0EEh, 0FFh, 0FEh, 0FFh
        cmp    eax, 1
        db  00Fh, 084h, 0F8h, 0FFh, 0FEh, 0FFh
        cmp    eax, 4
        db  00Fh, 084h, 054h, 000h, 0FFh, 0FFh
        clc
        ret
        db  08Bh, 0C0h
loc_1_0028:
        db  080h, 03Dh, 02Ch, 006h, 000h, 000h, 000h
        je     SHORT loc_1_0046
        db  080h, 03Dh, 090h, 006h, 000h, 000h, 000h
        jne    SHORT loc_1_0046
        mov    ax, 5305h
        db  0FFh, 01Dh, 024h, 006h, 000h, 000h
        clc
        ret
loc_1_0046:
        stc
        ret
_VPOWERD_Set_System_Power_State:
        enter  10h, 0
        push   ebx
        push   esi
        push   edi
        mov    eax, 80000001h
        db  080h, 03Dh, 090h, 006h, 000h, 000h, 000h
        jne    NEAR PTR loc_1_0239
        mov    eax, 80000007h
        mov    ebx, dword ptr [ebp + 8]
        or     ebx, ebx
        je     NEAR PTR loc_1_0239
        cmp    ebx, 3
        ja     NEAR PTR loc_1_0239
        mov    eax, 80000008h
        mov    edi, dword ptr [ebp + 0ch]
        and    edi, 7fffffffh
        cmp    edi, 3
        ja     NEAR PTR loc_1_0239
        je     NEAR PTR loc_1_0127
        db  00Fh, 0B6h, 0B7h, 081h, 006h, 000h, 000h
        cmp    ebx, 1
        jne    SHORT loc_1_00A6
        or     esi, 10h
loc_1_00A6:
        mov    dword ptr [ebp - 8], 80000002h
        mov    ebx, 0
        cmp    edi, 2
        lea    edi, [ebp - 4]
        je     SHORT loc_1_00CD
        push   edi
        push   esi
        push   ebx
        db  0E8h, 0D6h, 001h, 0FFh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_1_02B3
loc_1_00CD:
        inc    ebx
        push   edi
        push   esi
        push   ebx
        db  0E8h, 0C2h, 001h, 0FFh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_1_029E
        test   si, 10h
        jne    SHORT loc_1_0119
        push   ebx
        push   esi
        push   edi
        mov    ah, 0dh
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    edi
        pop    esi
        pop    ebx
        db  080h, 03Dh, 02Ah, 006h, 000h, 000h, 000h
        jne    SHORT loc_1_0119
        db  0A1h, 030h, 006h, 000h, 000h
        test   byte ptr [eax + 3], 40h
        je     SHORT loc_1_0119
        call   sub_1_0350
        or     eax, eax
        jne    NEAR PTR loc_1_0298
loc_1_0119:
        xor    esi, esi
        db  087h, 035h, 038h, 006h, 000h, 000h
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
loc_1_0127:
        mov    dword ptr [ebp - 0ch], 1000h
        mov    dword ptr [ebp - 10h], 5
        test   byte ptr [ebp + 0fh], 80h
        je     SHORT loc_1_018A
        cmp    dword ptr [ebp + 8], 1
        je     SHORT loc_1_018A
        db  0F6h, 005h, 040h, 006h, 000h, 000h, 002h
        jne    NEAR PTR loc_1_0232
        call   sub_1_04FC
        jb     SHORT loc_1_018A
        cmp    ecx, 0ch
        db  0BEh, 005h, 002h, 000h, 000h
        ja     SHORT loc_1_0169
        db  08Bh, 034h, 08Dh, 048h, 006h, 000h, 000h
loc_1_0169:
        db  081h, 0FEh, 0B5h, 001h, 000h, 000h
        je     SHORT loc_1_018D
        db  081h, 0FEh, 043h, 002h, 000h, 000h
        je     NEAR PTR loc_1_0247
        push   0
        push   ecx
        push   esi
        db  0CDh, 020h, 04Eh, 000h, 033h, 000h
        add    esp, 0ch
loc_1_018A:
        mov    ecx, dword ptr [ebp + 8]
loc_1_018D:
        and    ecx, 7
        mov    ax, 5307h
        mov    bx, 1
        push   ecx
        db  0FFh, 01Dh, 024h, 006h, 000h, 000h
        pop    ecx
        jb     NEAR PTR loc_1_0271
loc_1_01A6:
        cmp    ecx, 1
        jne    SHORT loc_1_01B4
        db  080h, 03Dh, 02Ah, 006h, 000h, 000h, 000h
        je     SHORT loc_1_0232
loc_1_01B4:
        dec    dword ptr [ebp - 0ch]
        je     SHORT loc_1_0232
        call   sub_1_04FC
        jb     SHORT loc_1_01B4
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, 2
        jne    SHORT loc_1_01CD
        cmp    ecx, 1
        je     SHORT loc_1_01B4
loc_1_01CD:
        cmp    ecx, 0ch
        db  0BEh, 005h, 002h, 000h, 000h
        ja     SHORT loc_1_01E1
        db  08Bh, 034h, 08Dh, 048h, 006h, 000h, 000h
loc_1_01E1:
        db  081h, 0FEh, 0B5h, 001h, 000h, 000h
        je     SHORT loc_1_021D
        db  081h, 0FEh, 043h, 002h, 000h, 000h
        jne    SHORT loc_1_020E
        mov    edi, dword ptr [ebp + 0ch]
        and    edi, 7fffffffh
        cmp    edi, 3
        jne    SHORT loc_1_0247
        push   0
        push   ecx
        push   esi
        db  0CDh, 020h, 04Eh, 000h, 033h, 000h
        add    esp, 0ch
        jmp    SHORT loc_1_0232
loc_1_020E:
        push   0
        push   ecx
        push   esi
        db  0CDh, 020h, 04Eh, 000h, 033h, 000h
        add    esp, 0ch
        jmp    SHORT loc_1_01B4
loc_1_021D:
        dec    dword ptr [ebp - 10h]
        jne    NEAR PTR loc_1_018D
        mov    dword ptr [ebp - 0ch], 2
        jmp    NEAR PTR loc_1_018D
loc_1_0232:
        call   sub_1_03D8
        xor    eax, eax
loc_1_0239:
        pop    edi
        pop    esi
        pop    ebx
        leave
        cmp    eax, 0
        jne    NEAR PTR loc_1_032E
        ret
loc_1_0247:
        shr    ecx, 2
        db  00Fh, 0B6h, 089h, 084h, 006h, 000h, 000h
        test   ecx, 10h
        jne    SHORT loc_1_0268
        db  083h, 03Dh, 044h, 006h, 000h, 000h, 000h
        jne    SHORT loc_1_0268
        or     ecx, 80h
loc_1_0268:
        push   ecx
        db  0E8h, 008h, 0FFh, 0FEh, 0FFh
        pop    ecx
        jmp    SHORT loc_1_0232
loc_1_0271:
        cmp    ah, 53h
        je     NEAR PTR loc_1_01A6
        movzx  eax, ah
        mov    dword ptr [ebp - 8], eax
        mov    edi, dword ptr [ebp + 0ch]
        and    edi, 7fffffffh
        cmp    edi, 3
        je     SHORT loc_1_02FB
        db  00Fh, 0B6h, 0B7h, 081h, 006h, 000h, 000h
        lea    edi, [ebp - 4]
loc_1_0298:
        db  0C7h, 007h, 088h, 006h, 000h, 000h
loc_1_029E:
        or     esi, 0eh
        push   edi
        push   esi
        push   4
        db  0E8h, 0EEh, 0FFh, 0FEh, 0FFh
        add    esp, 0ch
        db  0C7h, 007h, 088h, 006h, 000h, 000h
loc_1_02B3:
        or     esi, 0eh
        push   edi
        push   esi
        push   5
        db  0E8h, 0D9h, 0FFh, 0FEh, 0FFh
        add    esp, 0ch
        cmp    dword ptr [ebp - 8], 80000002h
        jne    SHORT loc_1_02FB
        db  080h, 03Dh, 02Ah, 006h, 000h, 000h, 000h
        je     SHORT loc_1_02FB
        test   byte ptr [ebp + 0fh], 80h
        je     SHORT loc_1_02FB
        xor    esi, esi
        db  087h, 035h, 038h, 006h, 000h, 000h
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        mov    ax, 5307h
        mov    bx, 1
        mov    ecx, 5
        db  0FFh, 01Dh, 024h, 006h, 000h, 000h
loc_1_02FB:
        call   sub_1_03D8
        cmp    dword ptr [ebp + 8], 1
        jne    SHORT loc_1_0326
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 7fffffffh
        or     eax, eax
        jne    SHORT loc_1_0326
        db  0BEh, 0BCh, 000h, 000h, 000h
        push   0
        push   dword ptr [ebp - 8]
        push   esi
        db  0CDh, 020h, 04Eh, 000h, 033h, 000h
        add    esp, 0ch
loc_1_0326:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_0239
loc_1_032E:
        mov    ecx, dword ptr [esp + 4]
        mov    edx, 2
        cmp    ecx, 3
        je     SHORT loc_1_0347
        mov    edx, 1
        cmp    ecx, 2
        je     SHORT loc_1_0347
        ret
loc_1_0347:
        mov    dword ptr [esp + 4], edx
        jmp    NEAR PTR _VPOWERD_Set_System_Power_State
sub_1_0350:
        push   ebx
        mov    ax, 44f1h
        push   15h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        mov    eax, 80000002h
        jb     SHORT loc_1_03D6
        mov    ax, 44f3h
        mov    bx, 100h
        push   15h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_1_03C8
        or     cx, cx
        jne    SHORT loc_1_03C8
        mov    ax, 44f4h
        mov    bx, 100h
        mov    cx, 1
        push   15h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        db  080h, 00Dh, 07Ch, 006h, 000h, 000h, 001h
        mov    ax, 44f3h
        mov    bx, 10dh
        push   15h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_1_03C8
        or     cx, cx
        je     SHORT loc_1_03C8
        mov    ax, 44f4h
        mov    bx, 10dh
        mov    cx, 0
        push   15h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        db  080h, 00Dh, 07Ch, 006h, 000h, 000h, 002h
loc_1_03C8:
        mov    ax, 44f2h
        push   15h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        xor    eax, eax
loc_1_03D6:
        pop    ebx
        ret
sub_1_03D8:
        db  0F6h, 005h, 07Ch, 006h, 000h, 000h, 001h
        je     SHORT loc_1_0433
        mov    ax, 44f1h
        push   15h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_1_0433
        db  0F6h, 005h, 07Ch, 006h, 000h, 000h, 002h
        je     SHORT loc_1_040C
        mov    ax, 44f4h
        mov    bx, 10dh
        mov    cx, 1
        push   15h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
loc_1_040C:
        mov    ax, 44f4h
        mov    bx, 100h
        mov    cx, 0
        push   15h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        db  080h, 025h, 07Ch, 006h, 000h, 000h, 0FCh
        mov    ax, 44f2h
        push   15h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
loc_1_0433:
        ret
loc_1_0434:
        call   sub_1_04FC
        jb     SHORT loc_1_04B2
        cmp    ecx, 8
        je     NEAR PTR loc_1_04E6
        cmp    ecx, 0ch
        db  0BEh, 005h, 002h, 000h, 000h
        ja     SHORT loc_1_0458
        db  08Bh, 034h, 08Dh, 048h, 006h, 000h, 000h
loc_1_0458:
        db  081h, 0FEh, 0B5h, 001h, 000h, 000h
        jne    SHORT loc_1_0496
        and    ebx, 7
        cmp    ebx, 1
        je     SHORT loc_1_0482
        db  0F6h, 005h, 07Ch, 006h, 000h, 000h, 001h
        jne    SHORT sub_1_04CE
        db  00Fh, 0BAh, 02Dh, 040h, 006h, 000h, 000h, 000h
        jb     SHORT sub_1_04CE
        db  080h, 025h, 040h, 006h, 000h, 000h, 0FDh
loc_1_0482:
        db  080h, 03Dh, 02Ah, 006h, 000h, 000h, 000h
        je     SHORT loc_1_04A5
        push   esi
        push   ecx
        call   sub_1_0545
        pop    ecx
        pop    esi
        jmp    SHORT loc_1_04A5
loc_1_0496:
        db  081h, 0FEh, 043h, 002h, 000h, 000h
        jne    SHORT loc_1_04A5
        db  080h, 00Dh, 040h, 006h, 000h, 000h, 002h
loc_1_04A5:
        push   0
        push   ecx
        push   esi
        db  0CDh, 020h, 04Eh, 000h, 033h, 000h
        add    esp, 0ch
loc_1_04B2:
        db  0FFh, 00Dh, 03Ch, 006h, 000h, 000h
        jne    SHORT sub_1_04CE
        db  08Dh, 035h, 073h, 002h, 000h, 000h
        push   0
        push   0
        push   esi
        db  0CDh, 020h, 04Eh, 000h, 033h, 000h
        add    esp, 0ch
sub_1_04CE:
        mov    eax, 384h
        db  08Dh, 035h, 034h, 004h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 034h, 006h, 000h, 000h
        ret
loc_1_04E6:
        push   80000003h
        push   2
        db  0CDh, 020h, 005h, 000h, 026h, 000h
        add    esp, 8
        jmp    NEAR PTR loc_1_0434
        db  090h
sub_1_04FC:
        mov    ax, 530bh
        xor    ebx, ebx
        db  0FFh, 01Dh, 024h, 006h, 000h, 000h
        sti
        jae    SHORT loc_1_050C
        ret
loc_1_050C:
        cmp    bx, 3
        jb     SHORT loc_1_053C
        cmp    bx, 4
        ja     SHORT loc_1_053C
        db  080h, 03Dh, 02Bh, 006h, 000h, 000h, 001h
        jne    SHORT loc_1_053C
        db  0C7h, 005h, 044h, 006h, 000h, 000h, 001h, 000h, 000h, 000h
        bt     cx, 0
        jb     SHORT loc_1_053C
        db  0C7h, 005h, 044h, 006h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_053C:
        movzx  ecx, bx
        jecxz  loc_1_0543
        clc
        ret
loc_1_0543:
        stc
        ret
sub_1_0545:
        mov    ax, 5307h
        mov    bx, 1
        mov    cx, 4
        db  0FFh, 01Dh, 024h, 006h, 000h, 000h
        mov    eax, 0bb8h
        db  08Dh, 035h, 045h, 005h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 038h, 006h, 000h, 000h
        ret
        db  000h, 018h, 004h, 000h, 000h, 01Fh, 004h, 000h, 000h, 028h, 004h, 000h, 000h, 030h, 004h, 000h
        db  000h, 050h, 005h, 000h, 000h, 048h, 000h, 000h, 000h, 0ACh, 005h, 000h, 000h, 0F4h, 002h, 000h
        db  000h, 07Ch, 005h, 000h, 000h, 0C2h, 005h, 000h, 000h, 07Ch, 003h, 000h, 000h, 0DEh, 003h, 000h
        db  000h, 0A4h, 003h, 000h, 000h, 092h, 004h, 000h, 000h, 01Eh, 006h, 000h, 000h, 04Eh, 006h, 000h
        db  000h, 04Ah, 006h, 000h, 000h, 0C8h, 004h, 000h, 000h, 06Ah, 005h, 000h, 000h, 09Ah, 006h, 000h
        db  000h, 0C9h, 006h, 000h, 000h, 0D0h, 006h, 000h, 000h, 0BCh, 003h, 000h, 000h, 0F6h, 006h, 000h
        db  000h, 078h, 006h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 026h, 000h, 004h, 000h, 000h, 000h, 056h, 050h, 04Fh, 057h
        db  045h, 052h, 044h, 020h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  070h, 005h, 000h, 000h, 019h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h
loc_1_062A:
        add    dword ptr [eax], eax
loc_1_062C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0630:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0634:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0638:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_063C:
        inc    edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0B5h, 001h, 000h, 000h, 0B5h
        add    dword ptr [eax], eax
        db  000h, 043h, 002h
        add    byte ptr [eax], al
        inc    ebx
        add    al, byte ptr [eax]
        db  000h, 0E2h
        add    dword ptr [eax], eax
        db  000h, 02Ah
        add    al, byte ptr [eax]
        db  000h, 0F1h
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 0B5h, 001h, 000h, 000h, 0B5h
        add    dword ptr [eax], eax
        db  000h, 043h, 002h
        add    byte ptr [eax], al
        pop    edi
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
loc_1_0681:
        add    dword ptr [eax], eax
        add    cl, byte ptr [edx]
        sub    bl, byte ptr [ebx]
        db  000h, 088h, 006h, 000h, 000h, 088h
        push   es
        add    byte ptr [eax], al
loc_1_0690:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
