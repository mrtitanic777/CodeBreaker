; ios_lcode_01.asm -- IOS object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

IOS_Control:
        cmp    eax, 0
        db  00Fh, 084h, 086h, 006h, 0FFh, 0FFh
        cmp    eax, 1
        db  00Fh, 084h, 030h, 009h, 0FFh, 0FFh
        cmp    eax, 2
        db  00Fh, 084h, 0ACh, 00Bh, 0FFh, 0FFh
        cmp    eax, 5
        je     SHORT loc_1_0044
        cmp    eax, 6
        je     NEAR PTR loc_1_0687
        cmp    eax, 23h
        db  00Fh, 084h, 0D1h, 030h, 0FFh, 0FFh
        cmp    eax, 22h
        db  00Fh, 084h, 046h, 01Ch, 0FFh, 0FFh
        clc
        ret
IOS_Get_Version:
        mov    eax, 30ah
        clc
        ret
loc_1_0044:
        db  08Bh, 035h, 09Ch, 01Fh, 000h, 000h
        or     byte ptr [esi + 29h], 40h
        db  08Bh, 035h, 020h, 022h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_0087
        db  08Bh, 015h, 09Ch, 01Fh, 000h, 000h
        mov    edx, dword ptr [edx + 30h]
loc_1_0061:
        or     edx, edx
        je     SHORT loc_1_0087
        test   byte ptr [edx + 21h], 80h
        je     SHORT loc_1_0082
        test   byte ptr [edx + 23h], 80h
        je     SHORT loc_1_0082
        cmp    dword ptr [edx + 0bfh], esi
        jb     SHORT loc_1_0082
        mov    ax, 26h
        call   sub_1_1AB1
loc_1_0082:
        mov    edx, dword ptr [edx + 0ch]
        jmp    SHORT loc_1_0061
loc_1_0087:
        sub    esp, 0ch
        mov    edi, esp
        mov    word ptr [edi], 0eh
        call   sub_1_175A
        add    esp, 0ch
        ret
        db  000h, 000h
loc_1_009C:
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 080h, 003h, 000h
loc_1_00A8:
        push   edi
        mov    eax, dword ptr [esp + 8]
        mov    edi, dword ptr [eax + 4]
        mov    edx, dword ptr [eax + 14h]
        db  0C7h, 002h, 0CEh, 000h, 000h, 000h
        add    dword ptr [eax + 14h], 8
        push   edi
        push   eax
        call   sub_1_1989
        add    esp, 8
        call   sub_1_010B
        pop    edi
        ret
loc_1_00CE:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    edi, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi + 14h]
        sub    ecx, 8
        mov    dword ptr [esi + 14h], ecx
        push   esi
        call   dword ptr [ecx]
        add    esp, 4
        cli
        cmp    dword ptr [edi + 107h], 0
        je     SHORT loc_1_0102
        cmp    esi, dword ptr [edi + 107h]
        jne    SHORT loc_1_0107
        mov    dword ptr [edi + 107h], 0
loc_1_0102:
        call   sub_1_010B
loc_1_0107:
        sti
        pop    edi
        pop    esi
        ret
sub_1_010B:
        cmp    dword ptr [edi + 107h], 0
        jne    SHORT loc_1_0143
        push   edi
        call   sub_1_18D0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_0143
        mov    dword ptr [edi + 107h], eax
        sti
        push   esi
        push   edi
        push   ebx
        lea    edi, [edi + 0c3h]
        mov    ecx, dword ptr [eax + 10h]
        mov    ecx, dword ptr [ecx + 0ch]
        mov    dword ptr [eax + 10h], ecx
        push   eax
        call   dword ptr [ecx]
        add    esp, 4
        pop    ebx
        pop    edi
        pop    esi
loc_1_0143:
        sti
        ret
loc_1_0145:
        mov    ebx, dword ptr [esp + 4]
        movzx  esi, word ptr [ebx]
        cmp    esi, 6
        ja     SHORT loc_1_0177
        db  0FFh, 024h, 0B5h, 05Bh, 001h, 000h, 000h
        db  000h, 000h, 000h, 000h, 077h, 001h, 000h, 000h, 077h, 001h, 000h, 000h, 041h, 000h, 000h, 000h
        db  023h, 002h, 000h, 000h, 077h, 001h, 000h, 000h, 021h, 000h, 000h, 000h
loc_1_0177:
        mov    word ptr [ebx + 2], 0
        ret
        db  08Bh, 0C0h
loc_1_0180:
        mov    ah, byte ptr [ebp + 1dh]
        cmp    ah, 90h
        jb     SHORT loc_1_01A6
        je     SHORT loc_1_0195
        cmp    ah, 91h
        ja     SHORT loc_1_01A6
        mov    byte ptr [ebp + 1dh], 0
        clc
        ret
loc_1_0195:
        and    word ptr [ebp + 2ch], 0fffeh
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
        mov    byte ptr [ebp + 1dh], 0
        clc
        ret
loc_1_01A6:
        stc
        ret
loc_1_01A8:
        db  08Bh, 03Dh, 003h, 022h, 000h, 000h
        test   edi, edi
        je     SHORT loc_1_01D1
        push   eax
        push   ebx
loc_1_01B4:
        push   dword ptr [edi]
        mov    ecx, dword ptr [edi + 30h]
        jecxz  loc_1_01C2
        mov    edi, dword ptr [edi + 64h]
        call   ecx
        jae    SHORT loc_1_01F7
loc_1_01C2:
        pop    edi
        mov    ebx, dword ptr [esp]
        mov    eax, dword ptr [esp + 4]
        test   edi, edi
        jne    SHORT loc_1_01B4
        add    esp, 8
loc_1_01D1:
        db  080h, 03Dh, 045h, 022h, 000h, 000h, 000h
        je     SHORT loc_1_01EB
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        db  0BEh, 0FBh, 001h, 000h, 000h
        db  0CDh, 020h, 010h, 080h, 001h, 000h
loc_1_01EB:
        db  0CDh, 020h, 0F1h, 000h, 001h, 000h
        db  0CDh, 020h, 002h, 080h, 003h, 000h
loc_1_01F7:
        add    esp, 0ch
        ret
loc_1_01FB:
        mov    byte ptr [48eh], 0ffh
        db  0CDh, 020h, 0F1h, 000h, 001h, 000h
        test   byte ptr [ebx + 1], 80h
        je     SHORT loc_1_0214
        db  0CDh, 020h, 039h, 080h, 001h, 000h
loc_1_0214:
        ret
        db  02Eh, 08Bh, 0C0h
IOS_BD_Command_Complete:
        pushad
        sub    esi, 64h
        call   sub_1_024D
        popad
        ret
loc_1_0223:
        jmp    NEAR PTR loc_1_0177
loc_1_0228:
        mov    esi, dword ptr [esp + 4]
        movzx  edx, word ptr [esi + 68h]
        cmp    dl, 0ch
        je     SHORT loc_1_0247
        mov    ebx, dword ptr [esi + 4]
        mov    edi, dword ptr [ebx + 127h]
        add    esi, 64h
        cli
        call   dword ptr [edi + 2ch]
        sti
        ret
loc_1_0247:
        mov    word ptr [esi + 6ah], 0
sub_1_024D:
        mov    edi, dword ptr [esi + 14h]
        sub    edi, 8
        mov    dword ptr [esi + 14h], edi
        push   esi
        call   dword ptr [edi]
        add    esp, 4
        ret
sub_1_025D:
        pushad
        movzx  eax, byte ptr [edi + 7]
        call   sub_1_128D
        jb     NEAR PTR loc_1_031B
        push   edi
        mov    al, 11h
        movzx  eax, al
        shl    eax, 3
        mov    ecx, eax
        add    eax, dword ptr [ebx + 4]
        add    eax, 64h
        sub    esp, 10h
        mov    edi, esp
        mov    dword ptr [edi + 6], eax
        add    eax, 58h
        mov    word ptr [edi + 4], ax
        mov    byte ptr [edi + 0eh], 0
        mov    word ptr [edi], 2
        push   edi
        call   sub_1_1310
        mov    edi, dword ptr [edi + 0ah]
        add    esp, 14h
        mov    eax, edi
        add    edi, ecx
        add    edi, dword ptr [ebx + 4]
        mov    dword ptr [edi + 4], ebx
        mov    dword ptr [edi + 8], ebx
        add    edi, 64h
        mov    dword ptr [edi + 20h], eax
        mov    dword ptr [edi + 48h], eax
        cld
        sub    ecx, ecx
        mov    cl, 8
        rep movsd
        sub    edi, 20h
        sub    esi, 20h
        test   byte ptr [esi + 8], 2
        je     SHORT loc_1_02E8
        mov    ecx, dword ptr [esi + 1ch]
        sub    eax, eax
        mov    edx, dword ptr [ecx]
        push   ebx
        sub    ebx, ebx
loc_1_02D5:
        inc    ebx
        add    eax, edx
        add    ecx, 8
        mov    edx, dword ptr [ecx]
        or     edx, edx
        jne    SHORT loc_1_02D5
        mov    byte ptr [edi + 4ch], bl
        pop    ebx
        mov    dword ptr [edi + 18h], eax
loc_1_02E8:
        or     byte ptr [edi + 9], 14h
        mov    dword ptr [edi + 0ch], esi
        mov    esi, edi
        sub    edi, 64h
        push   edi
        call   sub_1_14DA
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_0305
        or     byte ptr [esi + 8], 40h
loc_1_0305:
        test   byte ptr [ebx + 21h], 20h
        jne    SHORT loc_1_031D
loc_1_030B:
        pop    edi
        mov    dword ptr [esi + 40h], edi
        mov    al, byte ptr [edi + 7]
        mov    byte ptr [esi + 4dh], al
        call   IOS_SendCommand
        clc
loc_1_031B:
        popad
        ret
loc_1_031D:
        mov    eax, dword ptr [esi + 18h]
        movzx  ecx, byte ptr [ebx + 3ch]
        shl    eax, cl
        add    eax, 0fffh
        shr    eax, 0ch
        dec    eax
        je     SHORT loc_1_030B
        or     byte ptr [esi + 8], 42h
        jmp    SHORT loc_1_030B
sub_1_0337:
        push   edi
        push   esi
        mov    edi, dword ptr [esi + 0ch]
        movzx  eax, word ptr [esi + 6]
        mov    word ptr [edi + 6], ax
        xchg   edi, esi
        mov    edi, dword ptr [edi + 40h]
        pushfd
        cli
        call   dword ptr [esi + 0ch]
        popfd
        mov    esi, dword ptr [esp]
        sub    esp, 8
        mov    edi, esp
        mov    word ptr [edi], 4
        mov    esi, dword ptr [esi + 20h]
        mov    dword ptr [edi + 4], esi
        push   edi
        call   sub_1_1310
        add    esp, 0ch
        pop    esi
        pop    edi
        ret
        db  000h, 000h
loc_1_0370:
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        and    byte ptr [ecx + 29h], 7fh
        cmp    byte ptr [ebp + 1dh], 8
        jne    SHORT loc_1_0395
        test   byte ptr [ebp + 14h], 80h
        je     SHORT loc_1_0395
        or     byte ptr [ecx + 29h], 80h
loc_1_0395:
        mov    eax, dword ptr [ecx + 3ch]
        or     eax, eax
        je     SHORT loc_1_03B7
        test   byte ptr [ecx + 29h], 2
        je     SHORT loc_1_03B7
loc_1_03A2:
        db  0CDh, 020h, 005h, 000h, 005h, 000h
        xor    eax, eax
        db  0BEh, 03Eh, 005h, 000h, 000h
        db  0CDh, 020h, 00Dh, 000h, 001h, 000h
        stc
        ret
loc_1_03B7:
        test   byte ptr [ecx + 28h], 1
        je     NEAR PTR loc_1_04EE
        cmp    byte ptr [ebp + 1dh], 41h
        jb     SHORT loc_1_03CD
        cmp    byte ptr [ebp + 1dh], 49h
        jbe    SHORT loc_1_03F7
loc_1_03CD:
        cmp    byte ptr [ebp + 1dh], 0dh
        jbe    SHORT loc_1_03F7
        cmp    byte ptr [ebp + 1dh], 10h
        jb     NEAR PTR loc_1_04EE
        cmp    byte ptr [ebp + 1dh], 11h
        je     SHORT loc_1_03F7
        cmp    byte ptr [ebp + 1dh], 14h
        jbe    NEAR PTR loc_1_04EE
        cmp    byte ptr [ebp + 1dh], 1ah
        ja     NEAR PTR loc_1_04EE
loc_1_03F7:
        pushad
        movzx  eax, byte ptr [ebp + 14h]
        mov    cl, al
        or     eax, 40000000h
        call   sub_1_128D
        jae    SHORT loc_1_0430
        db  08Bh, 01Dh, 09Ch, 01Fh, 000h, 000h
        mov    ebx, dword ptr [ebx + 2ch]
        mov    al, 1
loc_1_0415:
        cmp    byte ptr [ebx], 1
        ja     SHORT loc_1_041F
        cmp    byte ptr [ebx + 5], cl
        je     SHORT loc_1_042E
loc_1_041F:
        cmp    dword ptr [ebx + 1], 0
        je     NEAR PTR loc_1_04C2
        add    ebx, dword ptr [ebx + 1]
        jmp    SHORT loc_1_0415
loc_1_042E:
        xor    ebx, ebx
loc_1_0430:
        mov    edi, ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        test   dword ptr [ebx], 20h
        jne    SHORT loc_1_0464
        movzx  eax, word ptr [ebp + 28h]
        shl    eax, 4
        movzx  edx, word ptr [ebp + 24h]
        add    eax, edx
        push   0
        push   1
        push   eax
        db  0CDh, 020h, 05Fh, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, 1
        je     NEAR PTR loc_1_04ED
loc_1_0464:
        sub    esp, 8
        mov    byte ptr [esp], 7
        mov    al, byte ptr [ebp + 14h]
        mov    byte ptr [esp + 2], al
        mov    esi, esp
        push   esi
        db  0CDh, 020h, 008h, 000h, 010h, 000h
        add    esp, 4
        mov    esi, dword ptr [esi + 3]
        movzx  ecx, byte ptr [esp + 1]
        add    esp, 8
        or     ecx, ecx
        jne    SHORT loc_1_04ED
        or     esi, esi
        je     SHORT loc_1_04ED
        xor    ebx, ebx
        cmp    byte ptr [ebp + 1dh], 3
        je     SHORT loc_1_049F
        cmp    byte ptr [ebp + 1dh], 43h
        jne    SHORT loc_1_04DA
loc_1_049F:
        mov    ebx, 1
        or     edi, edi
        je     SHORT loc_1_04B4
        test   byte ptr [edi + 1eh], 20h
        jne    SHORT loc_1_04E6
        test   byte ptr [edi + 16h], 1
        jne    SHORT loc_1_04ED
loc_1_04B4:
        movzx  eax, byte ptr [ebp + 14h]
        db  0CDh, 020h, 04Bh, 000h, 040h, 000h
        jae    SHORT loc_1_04E6
        mov    al, 3
loc_1_04C2:
        push   eax
        mov    ecx, 5
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        pop    eax
        mov    byte ptr [ebp + 1dh], al
        or     word ptr [ebp + 2ch], 1
        popad
        clc
        ret
loc_1_04DA:
        cmp    byte ptr [ebp + 1dh], 2
        je     SHORT loc_1_04E6
        cmp    byte ptr [ebp + 1dh], 42h
        jne    SHORT loc_1_04ED
loc_1_04E6:
        call   sub_1_0515
        jb     SHORT loc_1_04C2
loc_1_04ED:
        popad
loc_1_04EE:
        or     eax, eax
        je     NEAR PTR loc_1_03A2
        pushfd
        cli
        or     byte ptr [eax + 20h], 40h
        cmp    dword ptr [eax + 107h], 0
        jne    SHORT loc_1_050F
        mov    dword ptr [eax + 107h], 1
loc_1_050F:
        popfd
        jmp    NEAR PTR loc_1_03A2
sub_1_0515:
        bsf    edi, esi
        je     SHORT loc_1_053C
        btr    esi, edi
        push   ebx
        push   edi
        db  0CDh, 020h, 04Ch, 000h, 040h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT sub_1_0515
        mov    eax, 5
        or     ebx, ebx
        je     SHORT loc_1_053A
        mov    eax, 0cch
loc_1_053A:
        stc
        ret
loc_1_053C:
        clc
        ret
loc_1_053E:
        call   sub_1_05C2
        push   ecx
        push   eax
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        btr    dword ptr [ecx + 28h], 0fh
        jae    SHORT loc_1_057A
        db  0A0h, 024h, 022h, 000h, 000h
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        mov    ecx, dword ptr [ecx + 30h]
loc_1_0560:
        or     ecx, ecx
        je     SHORT loc_1_0577
        test   byte ptr [ecx + 21h], 80h
        je     SHORT loc_1_0572
        test   byte ptr [ecx + 16h], 40h
        je     SHORT loc_1_0572
        inc    al
loc_1_0572:
        mov    ecx, dword ptr [ecx + 0ch]
        jmp    SHORT loc_1_0560
loc_1_0577:
        mov    byte ptr [ebp + 14h], al
loc_1_057A:
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        cmp    dword ptr [ecx + 3ch], 0
        jne    SHORT loc_1_0596
loc_1_0586:
        pop    eax
        pop    ecx
        db  0CDh, 020h, 006h, 000h, 005h, 000h
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        clc
        ret
loc_1_0596:
        pushfd
        push   edi
        cli
        mov    edi, dword ptr [ecx + 3ch]
        or     edi, edi
        je     SHORT loc_1_05AD
        and    byte ptr [edi + 20h], 0bfh
        cmp    dword ptr [edi + 107h], 1
        je     SHORT loc_1_05B1
loc_1_05AD:
        pop    edi
        popfd
        jmp    SHORT loc_1_0586
loc_1_05B1:
        mov    dword ptr [edi + 107h], 0
        call   sub_1_010B
        jmp    SHORT loc_1_05AD
sub_1_05C2:
        pushad
        db  0F6h, 005h, 044h, 022h, 000h, 000h, 006h
        jne    SHORT loc_1_0607
        db  080h, 00Dh, 044h, 022h, 000h, 000h, 002h
        db  0A1h, 034h, 022h, 000h, 000h
        mov    ecx, eax
        call   sub_1_0609
        db  0A3h, 038h, 022h, 000h, 000h
        db  089h, 00Dh, 03Ch, 022h, 000h, 000h
        mov    edx, eax
        xor    edx, ecx
        test   edx, 0ffff0000h
        je     SHORT loc_1_0601
        db  08Bh, 00Dh, 034h, 022h, 000h, 000h
        call   sub_1_0609
loc_1_0601:
        db  0CDh, 020h, 003h, 000h, 020h, 000h
loc_1_0607:
        popad
        ret
sub_1_0609:
        pushf
        cli
        xchg   dword ptr [7b4h], eax
        db  0F6h, 005h, 02Ch, 022h, 000h, 000h, 004h
        jne    SHORT loc_1_0624
        xchg   dword ptr [806h], ecx
loc_1_0621:
        popf
        ret
loc_1_0624:
        xchg   dword ptr [7b0h], ecx
        jmp    SHORT loc_1_0621
sub_1_062C:
        pushad
        db  066h, 00Fh, 0BAh, 035h, 044h, 022h, 000h, 000h, 001h
        jae    SHORT loc_1_064E
        db  0CDh, 020h, 004h, 000h, 020h, 000h
        db  0A1h, 038h, 022h, 000h, 000h
        db  08Bh, 00Dh, 03Ch, 022h, 000h, 000h
        call   sub_1_0609
loc_1_064E:
        popad
        ret
loc_1_0650:
        db  0CDh, 020h, 002h, 000h, 020h, 000h
        jae    SHORT loc_1_0668
        db  0A1h, 03Ch, 022h, 000h, 000h
        mov    word ptr [ebp + 24h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 28h], ax
loc_1_0668:
        ret
loc_1_0669:
        db  0CDh, 020h, 002h, 000h, 020h, 000h
        jae    SHORT loc_1_0668
        call   sub_1_062C
        db  0A1h, 0C3h, 021h, 000h, 000h
        mov    word ptr [ebp + 24h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 28h], ax
        ret
loc_1_0687:
        sub    esp, 0ch
        mov    edi, esp
        mov    word ptr [edi], 1
        call   sub_1_175A
        add    esp, 0ch
        call   sub_1_062C
        pushf
        cli
        mov    eax, 4ch
        db  08Bh, 03Dh, 0C3h, 021h, 000h, 000h
        mov    dword ptr [eax], edi
        popf
        xor    ecx, ecx
loc_1_06B2:
        db  08Bh, 004h, 08Dh, 070h, 003h, 000h, 000h
        cmp    eax, -1
        je     SHORT loc_1_06C4
        db  0CDh, 020h, 00Ah, 000h, 001h, 000h
loc_1_06C4:
        inc    ecx
        cmp    ecx, 1ah
        jb     SHORT loc_1_06B2
        db  080h, 03Dh, 045h, 022h, 000h, 000h, 000h
        je     SHORT loc_1_06DD
        db  0A1h, 040h, 022h, 000h, 000h
        mov    dword ptr [54h], eax
loc_1_06DD:
        clc
        ret
        db  000h
loc_1_06E0:
        sti
        mov    edi, dword ptr [esp + 4]
        movzx  esi, word ptr [edi + 4]
        movzx  ecx, word ptr [edi + 0ah]
        mov    al, byte ptr [edi + 0ch]
        test   al, 8
        jne    SHORT loc_1_06FD
loc_1_06F4:
        call   sub_1_072B
loc_1_06F9:
        mov    dword ptr [edi + 6], ebx
        ret
loc_1_06FD:
        mov    al, 1
        db  08Bh, 01Dh, 09Ch, 01Fh, 000h, 000h
        test   byte ptr [ebx + 29h], 20h
        jne    SHORT loc_1_06F4
        add    esi, 14h
        push   0
        push   esi
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_06F9
        mov    word ptr [ebx + 10h], 20h
        add    ebx, 14h
        jmp    SHORT loc_1_06F9
sub_1_072B:
        sub    esp, 4
        pushfd
        push   edi
        push   edx
        push   ecx
        mov    byte ptr [esp + 10h], al
loc_1_0736:
        test   byte ptr [esp + 10h], 20h
        jne    NEAR PTR loc_1_0849
        push   esi
        add    esi, 0fh
        and    esi, 0fffffff0h
        cli
        cmp    esi, 400h
        jae    SHORT loc_1_0792
        mov    edi, esi
        shr    edi, 2
        db  08Bh, 09Fh, 0A8h, 01Fh, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_1_0792
        mov    eax, dword ptr [ebx + 4]
        test   byte ptr [esp + 14h], 40h
        jne    SHORT loc_1_078E
loc_1_076A:
        db  089h, 087h, 0A8h, 01Fh, 000h, 000h
loc_1_0770:
        mov    word ptr [ebx + 0ch], cx
        mov    word ptr [ebx + 10h], 4
        mov    dword ptr [ebx + 4], 0
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ebx + 14h], eax
        add    ebx, 14h
        jmp    NEAR PTR loc_1_09A9
loc_1_078E:
        or     eax, eax
        jne    SHORT loc_1_076A
loc_1_0792:
        xor    ebx, ebx
        test   byte ptr [esp + 14h], 40h
        jne    NEAR PTR loc_1_09A9
        add    esi, 14h
        db  08Bh, 01Dh, 09Ch, 01Fh, 000h, 000h
        add    ebx, 6eh
loc_1_07AB:
        test   word ptr [ebx + 10h], 1
        je     SHORT loc_1_07BB
        cmp    si, word ptr [ebx + 0eh]
        jb     SHORT loc_1_07C9
        je     SHORT loc_1_07FC
loc_1_07BB:
        mov    ebx, dword ptr [ebx + 8]
        or     ebx, ebx
        jne    SHORT loc_1_07AB
        xor    esi, esi
        jmp    NEAR PTR loc_1_091B
loc_1_07C9:
        movzx  edi, word ptr [ebx + 0eh]
        sub    edi, esi
        cmp    edi, 24h
        jbe    SHORT loc_1_07FC
        mov    word ptr [ebx + 0eh], di
        mov    eax, dword ptr [ebx + 8]
        add    edi, ebx
        mov    dword ptr [ebx + 8], edi
        mov    ebx, dword ptr [ebx + 8]
        mov    dword ptr [ebx + 8], eax
        db  0A1h, 0A0h, 01Fh, 000h, 000h
        add    eax, ebx
        db  02Bh, 005h, 09Ch, 01Fh, 000h, 000h
        add    eax, 14h
        mov    dword ptr [ebx], eax
        mov    word ptr [ebx + 0eh], si
loc_1_07FC:
        jmp    NEAR PTR loc_1_0770
loc_1_0801:
        sti
        shr    esi, 2
        inc    esi
        cmp    esi, 40h
        jb     NEAR PTR loc_1_091B
        pop    esi
        cli
        db  08Bh, 01Dh, 0BFh, 021h, 000h, 000h
loc_1_0817:
        or     ebx, ebx
        je     SHORT loc_1_083E
        cmp    word ptr [ebx + 0eh], si
        jb     SHORT loc_1_0829
        test   word ptr [ebx + 10h], 14h
        je     SHORT loc_1_082E
loc_1_0829:
        mov    ebx, dword ptr [ebx + 4]
        jmp    SHORT loc_1_0817
loc_1_082E:
        or     word ptr [ebx + 10h], 4
        mov    word ptr [ebx + 0ch], cx
        mov    eax, dword ptr [ebx]
        jmp    NEAR PTR loc_1_08E3
loc_1_083E:
        test   byte ptr [esp + 10h], 1
        jne    NEAR PTR loc_1_09A5
loc_1_0849:
        db  08Bh, 01Dh, 09Ch, 01Fh, 000h, 000h
        cmp    dword ptr [ebx + 54h], 0
        jne    NEAR PTR loc_1_08EF
        sti
        mov    ebx, esi
        add    ebx, 14h
        shl    ebx, 1
        push   ecx
        push   1
        push   ebx
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_1_0999
        shr    ebx, 1
        mov    ecx, eax
        mov    edx, eax
        and    ecx, 0fffh
        mov    edi, 1000h
        sub    edi, ecx
        cmp    edi, ebx
        ja     SHORT loc_1_0897
        add    eax, ebx
        add    ecx, ebx
        and    ecx, 0fffh
loc_1_0897:
        mov    dword ptr [eax + 8], edx
        mov    dword ptr [eax + 4], ecx
        mov    word ptr [eax + 10h], 0ch
        pop    ecx
        mov    word ptr [eax + 0ch], cx
        mov    word ptr [eax + 0eh], bx
        mov    ebx, eax
        mov    ecx, eax
        shr    ecx, 0ch
        lea    eax, [eax]
        push   0
        push   eax
        push   1
        push   ecx
        db  0CDh, 020h, 061h, 000h, 001h, 000h
        add    esp, 10h
        mov    eax, dword ptr [ebx]
        and    eax, 0fffff000h
        add    eax, dword ptr [ebx + 4]
        add    eax, 14h
        cli
        db  08Bh, 00Dh, 0BFh, 021h, 000h, 000h
        mov    dword ptr [ebx + 4], ecx
        db  089h, 01Dh, 0BFh, 021h, 000h, 000h
        mov    dword ptr [ebx], eax
loc_1_08E3:
        mov    dword ptr [ebx + 14h], eax
        sti
        add    ebx, 14h
        jmp    NEAR PTR loc_1_09AA
loc_1_08EF:
        test   byte ptr [esp + 10h], 2
        je     NEAR PTR loc_1_09A5
loc_1_08FA:
        db  0FEh, 005h, 0BEh, 021h, 000h, 000h
        db  0A1h, 0BAh, 021h, 000h, 000h
        mov    ecx, 5
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        db  0FEh, 00Dh, 0BEh, 021h, 000h, 000h
        jmp    NEAR PTR loc_1_0736
loc_1_091B:
        shl    esi, 2
        cli
        db  08Bh, 09Eh, 0A8h, 01Fh, 000h, 000h
        or     ebx, ebx
        je     NEAR PTR loc_1_0801
        mov    eax, dword ptr [ebx + 4]
        db  089h, 086h, 0A8h, 01Fh, 000h, 000h
        mov    edi, dword ptr [ebx + 8]
        or     edi, edi
        je     SHORT loc_1_0959
        mov    word ptr [ebx + 10h], 1
        test   word ptr [edi + 10h], 1
        je     SHORT loc_1_0959
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ebx + 8], eax
        movzx  eax, word ptr [edi + 0eh]
        add    word ptr [ebx + 0eh], ax
loc_1_0959:
        mov    esi, 6eh
        db  003h, 035h, 09Ch, 01Fh, 000h, 000h
loc_1_0964:
        cmp    dword ptr [esi + 8], ebx
        je     SHORT loc_1_0975
        mov    esi, dword ptr [esi + 8]
        jb     SHORT loc_1_0964
        sti
        pop    esi
        jmp    NEAR PTR loc_1_0736
loc_1_0975:
        test   word ptr [esi + 10h], 1
        jne    SHORT loc_1_0984
        sti
        pop    esi
        jmp    NEAR PTR loc_1_0736
loc_1_0984:
        movzx  eax, word ptr [ebx + 0eh]
        add    word ptr [esi + 0eh], ax
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esi + 8], eax
        sti
        pop    esi
        jmp    NEAR PTR loc_1_0736
loc_1_0999:
        pop    ecx
        test   byte ptr [esp + 10h], 2
        jne    NEAR PTR loc_1_08FA
loc_1_09A5:
        xor    ebx, ebx
        jmp    SHORT loc_1_09AA
loc_1_09A9:
        pop    esi
loc_1_09AA:
        pop    ecx
        pop    edx
        pop    edi
        popfd
        add    esp, 4
        ret
        db  000h, 000h
loc_1_09B4:
        mov    edi, dword ptr [esp + 4]
        mov    edi, dword ptr [edi + 4]
        test   word ptr [edi - 4], 30h
        jne    SHORT loc_1_09C9
        call   sub_1_0A1E
loc_1_09C8:
        ret
loc_1_09C9:
        test   word ptr [edi - 4], 10h
        jne    SHORT loc_1_09E2
        sub    edi, 14h
        push   0
        push   edi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        jmp    SHORT loc_1_09C8
loc_1_09E2:
        sub    edi, 14h
        pushfd
        cli
        db  08Bh, 01Dh, 0BFh, 021h, 000h, 000h
        db  0B9h, 0BBh, 021h, 000h, 000h
loc_1_09F2:
        or     ebx, ebx
        je     SHORT loc_1_0A14
        cmp    ebx, edi
        jne    SHORT loc_1_0A17
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], eax
        db  080h, 03Dh, 0BEh, 021h, 000h, 000h, 000h
        je     SHORT loc_1_0A14
        db  0A1h, 0BAh, 021h, 000h, 000h
        db  0CDh, 020h, 031h, 001h, 001h, 000h
loc_1_0A14:
        popfd
        jmp    SHORT loc_1_09C8
loc_1_0A17:
        mov    ecx, ebx
        mov    ebx, dword ptr [ebx + 4]
        jmp    SHORT loc_1_09F2
sub_1_0A1E:
        push   ebp
        push   esi
        push   edi
        sub    edi, 14h
        test   byte ptr [edi + 10h], 8
        jne    SHORT loc_1_0A63
        movzx  esi, word ptr [edi + 0eh]
        sub    esi, 14h
        and    esi, 0fffffff0h
        cmp    esi, 400h
        jae    SHORT loc_1_0A76
        cli
        mov    word ptr [edi + 10h], 2
        shr    esi, 2
        db  08Bh, 0AEh, 0A8h, 01Fh, 000h, 000h
        mov    dword ptr [edi + 4], ebp
        db  089h, 0BEh, 0A8h, 01Fh, 000h, 000h
        sti
loc_1_0A56:
        db  080h, 03Dh, 0BEh, 021h, 000h, 000h, 000h
        jne    SHORT loc_1_0A69
loc_1_0A5F:
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_1_0A63:
        and    byte ptr [edi + 10h], 0fbh
        jmp    SHORT loc_1_0A56
loc_1_0A69:
        db  0A1h, 0BAh, 021h, 000h, 000h
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        jmp    SHORT loc_1_0A5F
loc_1_0A76:
        db  08Bh, 035h, 09Ch, 01Fh, 000h, 000h
        add    esi, 6eh
loc_1_0A7F:
        cmp    dword ptr [esi + 8], edi
        je     SHORT loc_1_0A8B
        ja     SHORT loc_1_0AA5
        mov    esi, dword ptr [esi + 8]
        jmp    SHORT loc_1_0A7F
loc_1_0A8B:
        test   word ptr [esi + 10h], 1
        je     SHORT loc_1_0AA5
        movzx  ebp, word ptr [edi + 0eh]
        add    word ptr [esi + 0eh], bp
        mov    ebp, dword ptr [edi + 8]
        mov    dword ptr [esi + 8], ebp
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_1_0AA5:
        mov    word ptr [edi + 10h], 1
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  000h
IOS_SendCommand:
        test   byte ptr [esi + 9], 4
        je     NEAR PTR loc_1_0B7E
        push   esi
        push   edi
        push   ebx
        sub    esi, 64h
        mov    ebx, edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], edi
        sub    ebx, 0c3h
        cmp    word ptr [esi + 68h], 0fh
        je     SHORT loc_1_0AFB
        test   byte ptr [esi + 6fh], 20h
        jne    SHORT loc_1_0AFB
        movzx  eax, byte ptr [esi + 0b1h]
        mov    ebx, dword ptr [esi + 6ch]
        and    ebx, 40000000h
        or     eax, ebx
        call   sub_1_128D
        jb     NEAR PTR loc_1_1013
loc_1_0AFB:
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [eax + 0bfh], 0
        test   byte ptr [eax + 23h], 4
        jne    NEAR PTR loc_1_1013
        mov    eax, dword ptr [eax + 41h]
        or     eax, dword ptr [ebx + 41h]
        jne    NEAR PTR loc_1_1035
loc_1_0B20:
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esi + 10h], eax
loc_1_0B26:
        mov    word ptr [esi + 0eh], 0fh
        mov    word ptr [esi + 0ch], 0fh
        mov    dword ptr [esi + 8], ebx
        cmp    byte ptr [esi + 68h], 0dh
        je     SHORT loc_1_0BA3
loc_1_0B3B:
        lea    eax, [esi + 2ch]
        mov    dword ptr [esi + 14h], eax
        db  0C7h, 046h, 02Ch, 00Dh, 011h, 000h, 000h
        add    dword ptr [esi + 14h], 8
        mov    eax, dword ptr [esi + 6ch]
        test   eax, 8000040h
        jne    NEAR PTR loc_1_0C71
        test   eax, 100h
        jne    SHORT loc_1_0BD3
        push   eax
loc_1_0B62:
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [esi + 10h]
        call   dword ptr [eax]
        pop    esi
        pop    edi
        pop    ebx
        pop    eax
        test   eax, 8000140h
        jne    NEAR PTR loc_1_0C05
        jmp    NEAR PTR loc_1_0C6D
loc_1_0B7E:
        cmp    dword ptr [esi], 0
        je     NEAR PTR sub_1_025D
        push   eax
        sub    esp, 4
loc_1_0B8B:
        sub    eax, eax
        xchg   dword ptr [esi], eax
        mov    dword ptr [esp], eax
        call   sub_1_025D
        mov    esi, dword ptr [esp]
        or     esi, esi
        jne    SHORT loc_1_0B8B
        add    esp, 4
        pop    eax
        ret
loc_1_0BA3:
        test   byte ptr [esi + 6dh], 1
        jne    SHORT loc_1_0BC3
        mov    edx, esi
        db  0BEh, 0BBh, 00Bh, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        jmp    NEAR PTR loc_1_0C6D
loc_1_0BBB:
        mov    esi, edx
        mov    ebx, dword ptr [esi + 8]
        sub    esp, 0ch
loc_1_0BC3:
        db  0E8h, 0D0h, 0FEh, 0FEh, 0FFh
        jae    NEAR PTR loc_1_0C6D
        jmp    NEAR PTR loc_1_0B3B
loc_1_0BD3:
        db  0A1h, 0B6h, 021h, 000h, 000h
        mov    ecx, 5
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        cli
        db  00Fh, 0B7h, 00Dh, 094h, 021h, 000h, 000h
        bsf    eax, ecx
        db  066h, 00Fh, 0B3h, 005h, 094h, 021h, 000h, 000h
        sti
        push   dword ptr [esi + 70h]
        mov    dword ptr [esi + 70h], eax
        push   dword ptr [esi + 6ch]
        jmp    NEAR PTR loc_1_0B62
loc_1_0C05:
        mov    ecx, 5
        mov    eax, dword ptr [esi + 70h]
        db  08Bh, 004h, 085h, 096h, 021h, 000h, 000h
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        test   dword ptr [esi + 6ch], 8000040h
        jne    NEAR PTR loc_1_0F07
loc_1_0C27:
        mov    eax, dword ptr [esi + 70h]
        db  066h, 00Fh, 0ABh, 005h, 094h, 021h, 000h, 000h
        db  0A1h, 0B6h, 021h, 000h, 000h
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        pop    dword ptr [esi + 70h]
        mov    ebx, dword ptr [esi + 8]
        cmp    word ptr [esi + 6ah], 2ch
        je     NEAR PTR loc_1_0CD7
loc_1_0C4E:
        test   dword ptr [esi + 6ch], 100h
        jne    SHORT loc_1_0C6D
loc_1_0C57:
        add    esi, 64h
        test   byte ptr [esi + 9], 10h
        jne    NEAR PTR loc_1_102B
        push   dword ptr [esi + 40h]
        call   dword ptr [esi + 0ch]
        add    esp, 4
loc_1_0C6D:
        pop    ebx
        pop    edi
        pop    esi
        ret
loc_1_0C71:
        test   byte ptr [esi + 6dh], 1
        jne    SHORT loc_1_0CD7
        mov    edx, esi
        db  0BEh, 0CFh, 00Ch, 000h, 000h
        mov    ebx, dword ptr [edx + 8]
        test   byte ptr [ebx + 21h], 20h
        jne    SHORT loc_1_0C8F
        db  0CDh, 020h, 010h, 000h, 001h, 000h
        jmp    SHORT loc_1_0C6D
loc_1_0C8F:
        db  0BEh, 0BDh, 00Ch, 000h, 000h
        xor    ebx, ebx
        mov    ecx, 820h
        mov    eax, 10h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        jmp    SHORT loc_1_0C6D
loc_1_0CA8:
        db  0BEh, 0B8h, 00Ch, 000h, 000h
        mov    eax, 64h
        db  0CDh, 020h, 03Ch, 080h, 001h, 000h
loc_1_0CB8:
        sub    esp, 0ch
        jmp    SHORT loc_1_0C8F
loc_1_0CBD:
        call   sub_1_1E18
        jb     SHORT loc_1_0CA8
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        test   byte ptr [eax + 28h], 80h
        jne    SHORT loc_1_0CA8
loc_1_0CCF:
        mov    esi, edx
        mov    ebx, dword ptr [esi + 8]
        sub    esp, 0ch
loc_1_0CD7:
        mov    edx, esi
        mov    edi, ebx
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        test   byte ptr [eax + 29h], 20h
        mov    al, 1
        jne    SHORT loc_1_0CEA
        mov    al, 2
loc_1_0CEA:
        mov    esi, 49h
        mov    ecx, 9
        call   sub_1_072B
        mov    esi, edx
        or     ebx, ebx
        je     NEAR PTR loc_1_0EFC
        test   byte ptr [edi + 21h], 20h
        jne    SHORT loc_1_0D13
        test   byte ptr [esi + 6fh], 8
        jne    NEAR PTR loc_1_0D93
loc_1_0D13:
        and    byte ptr [esi + 6fh], 0f7h
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        test   byte ptr [eax + 29h], 20h
        jne    SHORT loc_1_0D40
        push   80h
        push   0
        push   -1
        push   0
        push   0
        push   0
        push   1
        push   1
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        jmp    SHORT loc_1_0D85
loc_1_0D40:
        pushad
        mov    edi, dword ptr [esi + 4]
        mov    esi, 2000h
        cmp    dword ptr [edi + 50h], 1000h
        je     SHORT loc_1_0D57
        mov    esi, 1000h
loc_1_0D57:
        mov    ecx, 9
        mov    al, 1
        call   sub_1_072B
        mov    dword ptr [esp + 14h], ebx
        mov    dword ptr [esp + 1ch], ebx
        cmp    dword ptr [edi + 50h], 1000h
        jne    SHORT loc_1_0D84
        add    ebx, 0fffh
        and    ebx, 0fffff000h
        mov    dword ptr [esp + 14h], ebx
loc_1_0D84:
        popad
loc_1_0D85:
        or     edx, edx
        je     NEAR PTR loc_1_0EF5
        mov    dword ptr [ebx + 21h], edx
        mov    dword ptr [ebx + 29h], eax
loc_1_0D93:
        mov    ecx, dword ptr [esi + 6ch]
        mov    dword ptr [ebx + 2dh], ecx
        mov    ecx, dword ptr [esi + 74h]
        mov    dword ptr [ebx + 35h], ecx
        mov    ecx, dword ptr [esi + 80h]
        test   dword ptr [esi + 6ch], 2
        je     NEAR PTR loc_1_0EE7
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [ebx + 8], eax
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebx + 0ch], eax
loc_1_0DBD:
        test   byte ptr [esi + 6fh], 8
        jne    NEAR PTR loc_1_0E8C
        xchg   dword ptr [ebx], ecx
        xchg   dword ptr [esi + 0ach], ecx
        mov    dword ptr [ebx + 4], ecx
        mov    cl, byte ptr [edi + 3ch]
        shl    dword ptr [ebx + 0ch], cl
        test   dword ptr [edi + 1ch], 1000h
        jne    SHORT loc_1_0DE7
        mov    dword ptr [esi + 0ach], ebx
loc_1_0DE7:
        add    dword ptr [esi + 0ach], 10h
        mov    dword ptr [ebx + 10h], edx
        xchg   dword ptr [esi + 80h], edx
        mov    dword ptr [ebx + 31h], edx
        and    byte ptr [esi + 6ch], 0fdh
        or     byte ptr [esi + 6fh], 4
        mov    eax, 1000h
        mov    edx, dword ptr [edi]
        cmp    dword ptr [edx + 50h], eax
        jae    SHORT loc_1_0E11
        mov    eax, dword ptr [edx + 50h]
loc_1_0E11:
        mov    dword ptr [ebx + 14h], eax
        shr    eax, cl
        cmp    eax, dword ptr [esi + 7ch]
        jbe    SHORT loc_1_0E23
        mov    eax, dword ptr [esi + 7ch]
        shl    eax, cl
        mov    dword ptr [ebx + 14h], eax
loc_1_0E23:
        test   dword ptr [edi + 1ch], 800h
        je     SHORT loc_1_0E5B
        mov    eax, dword ptr [ebx + 21h]
        shr    eax, 0ch
        lea    ecx, [ebx + 25h]
        push   0
        push   ecx
        push   1
        push   eax
        db  0CDh, 020h, 061h, 000h, 001h, 000h
        add    esp, 10h
        mov    edx, dword ptr [ebx + 25h]
        and    edx, 0fffff000h
        mov    ecx, dword ptr [ebx + 21h]
        and    ecx, 0fffh
        add    edx, ecx
        mov    dword ptr [ebx + 10h], edx
loc_1_0E5B:
        mov    cl, byte ptr [edi + 3ch]
        mov    eax, dword ptr [ebx + 14h]
        shr    eax, cl
        xchg   dword ptr [esi + 7ch], eax
        mov    dword ptr [ebx + 18h], eax
        mov    dword ptr [ebx + 1ch], eax
        mov    al, 1
        xchg   byte ptr [esi + 0b0h], al
        mov    byte ptr [ebx + 20h], al
        cmp    word ptr [esi + 68h], 1
        jne    NEAR PTR loc_1_0BD3
        call   sub_1_1150
        jmp    NEAR PTR loc_1_0BD3
loc_1_0E8C:
        mov    eax, dword ptr [esi + 7ch]
        mov    dword ptr [ebx + 18h], eax
        mov    dword ptr [ebx + 1ch], eax
        mov    eax, dword ptr [esi + 80h]
        mov    dword ptr [ebx + 31h], eax
        mov    dword ptr [ebx], eax
        bts    dword ptr [esi + 6ch], 1
        jb     SHORT loc_1_0EC0
        xor    eax, eax
        mov    dword ptr [ebx + 45h], eax
        mov    dword ptr [ebx + 41h], eax
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [ebx + 3dh], eax
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [ebx + 39h], eax
        lea    eax, [ebx + 39h]
        mov    dword ptr [ebx], eax
loc_1_0EC0:
        xor    eax, eax
        mov    dword ptr [ebx + 25h], eax
        call   sub_1_11C9
        jb     SHORT loc_1_0ED1
        jmp    NEAR PTR loc_1_0BD3
loc_1_0ED1:
        call   sub_1_10C4
        call   sub_1_10E9
        and    byte ptr [esi + 6fh], 0f7h
        mov    ebx, dword ptr [esi + 8]
        jmp    NEAR PTR loc_1_0CD7
loc_1_0EE7:
        mov    dword ptr [ebx + 8], ecx
        mov    eax, dword ptr [esi + 7ch]
        mov    dword ptr [ebx + 0ch], eax
        jmp    NEAR PTR loc_1_0DBD
loc_1_0EF5:
        mov    edi, ebx
        call   sub_1_0A1E
loc_1_0EFC:
        mov    word ptr [esi + 6ah], 1dh
        jmp    NEAR PTR loc_1_0C4E
loc_1_0F07:
        cmp    word ptr [esi + 6ah], 10h
        jb     SHORT loc_1_0F21
        test   byte ptr [esi + 6fh], 8
        je     NEAR PTR loc_1_0FC7
        mov    dword ptr [ebx + 18h], 0
        jmp    SHORT loc_1_0F9C
loc_1_0F21:
        mov    eax, dword ptr [esi + 7ch]
        sub    dword ptr [ebx + 18h], eax
        add    dword ptr [esi + 74h], eax
        mov    edi, dword ptr [esi + 8]
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esi + 10h], eax
        add    dword ptr [esi + 14h], 8
        test   byte ptr [esi + 6fh], 8
        jne    SHORT loc_1_0F9C
        cmp    word ptr [esi + 68h], 1
        je     SHORT loc_1_0F49
        call   sub_1_1150
loc_1_0F49:
        mov    dword ptr [esi + 64h], 0
        cmp    dword ptr [ebx + 18h], 0
        je     SHORT loc_1_0FC7
        mov    eax, 1000h
        mov    edi, dword ptr [esi + 4]
        cmp    dword ptr [edi + 50h], eax
        jae    SHORT loc_1_0F66
        mov    eax, dword ptr [edi + 50h]
loc_1_0F66:
        mov    dword ptr [ebx + 14h], eax
        mov    edi, dword ptr [esi + 8]
        mov    cl, byte ptr [edi + 3ch]
        shr    eax, cl
        mov    dword ptr [esi + 7ch], eax
        cmp    eax, dword ptr [ebx + 18h]
        jbe    SHORT loc_1_0F84
        mov    eax, dword ptr [ebx + 18h]
        mov    dword ptr [esi + 7ch], eax
        shl    eax, cl
        mov    dword ptr [ebx + 14h], eax
loc_1_0F84:
        push   dword ptr [esi + 6ch]
        cmp    word ptr [esi + 68h], 1
        jne    NEAR PTR loc_1_0B62
        call   sub_1_1150
        jmp    NEAR PTR loc_1_0B62
loc_1_0F9C:
        call   sub_1_10C4
        cmp    dword ptr [ebx + 18h], 0
        je     SHORT loc_1_0FFE
        call   sub_1_11C9
        jb     SHORT loc_1_0FB6
        push   dword ptr [esi + 6ch]
        jmp    NEAR PTR loc_1_0B62
loc_1_0FB6:
        call   sub_1_10C4
        mov    word ptr [esi + 6ah], 2ch
        and    byte ptr [ebx + 30h], 0f7h
        jmp    SHORT loc_1_0FFE
loc_1_0FC7:
        mov    al, byte ptr [ebx + 20h]
        mov    byte ptr [esi + 0b0h], al
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [esi + 0ach], eax
        db  0A1h, 09Ch, 01Fh, 000h, 000h
        test   byte ptr [eax + 29h], 20h
        jne    SHORT loc_1_0FF4
        push   0
        push   dword ptr [ebx + 29h]
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
        jmp    SHORT loc_1_0FFE
loc_1_0FF4:
        pushad
        mov    edi, dword ptr [ebx + 29h]
        call   sub_1_0A1E
        popad
loc_1_0FFE:
        call   sub_1_10E9
        jmp    NEAR PTR loc_1_0C27
        db  066h, 0C7h, 046h, 06Ah, 01Dh, 000h, 0E9h, 05Ah, 0FCh, 0FFh, 0FFh
loc_1_1013:
        mov    word ptr [esi + 6ah], 17h
        test   dword ptr [esi + 6ch], 100h
        jne    NEAR PTR loc_1_0C6D
        jmp    NEAR PTR loc_1_0C57
loc_1_102B:
        call   sub_1_0337
        jmp    NEAR PTR loc_1_0C6D
loc_1_1035:
        mov    ecx, ebx
        cmp    dword ptr [ecx + 41h], 0
        jne    SHORT loc_1_103F
        mov    ecx, dword ptr [ecx]
loc_1_103F:
        push   ebx
        mov    ebx, edi
        test   dword ptr [ecx + 24h], 1
        jne    SHORT loc_1_104E
        mov    ebx, dword ptr [edi + 14h]
loc_1_104E:
        cmp    ebx, eax
        pop    ebx
        je     NEAR PTR loc_1_0B20
        cmp    word ptr [esi + 68h], 0dh
        je     SHORT loc_1_1069
loc_1_105E:
        mov    word ptr [esi + 6ah], 29h
        jmp    NEAR PTR loc_1_0C4E
loc_1_1069:
        mov    eax, dword ptr [esi + 92h]
        cmp    ah, 44h
        jne    SHORT loc_1_1091
        cmp    al, 11h
        ja     SHORT loc_1_1091
        cmp    al, 11h
        je     NEAR PTR loc_1_0B20
        cmp    al, 0dh
        je     SHORT loc_1_109D
        cmp    al, 0ah
        ja     SHORT loc_1_1091
        cmp    al, 8
        jb     SHORT loc_1_1091
        jmp    NEAR PTR loc_1_0B20
loc_1_1091:
        mov    dword ptr [esi + 0a0h], 28h
        jmp    SHORT loc_1_105E
loc_1_109D:
        mov    eax, dword ptr [esi + 94h]
        cmp    ah, 8
        jne    SHORT loc_1_1091
        cmp    al, 6fh
        je     NEAR PTR loc_1_0B20
        cmp    al, 60h
        jne    SHORT loc_1_1091
        mov    eax, dword ptr [esi + 98h]
        test   byte ptr [eax], 1
        jne    SHORT loc_1_1091
        jmp    NEAR PTR loc_1_0B20
sub_1_10C4:
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 0ch]
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [ebx + 8]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [ebx + 21h]
        mov    dword ptr [eax + 8], ecx
        mov    eax, dword ptr [esi + 80h]
        mov    ecx, dword ptr [ebx + 14h]
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [ebx + 10h]
        mov    dword ptr [eax], ecx
        ret
sub_1_10E9:
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [esi + 7ch], eax
        mov    eax, dword ptr [ebx + 35h]
        mov    dword ptr [esi + 74h], eax
        mov    eax, dword ptr [ebx + 31h]
        mov    dword ptr [esi + 80h], eax
        mov    eax, dword ptr [ebx + 2dh]
        mov    dword ptr [esi + 6ch], eax
        mov    edi, ebx
        jmp    NEAR PTR sub_1_0A1E
        db  090h
IOS_Synchronous_Command:
        ret
loc_1_110D:
        push   ebx
        push   edi
        push   esi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esi + 4]
        test   dword ptr [esi + 6ch], 8000140h
        jne    SHORT loc_1_1137
        add    esi, 64h
        test   byte ptr [esi + 9], 10h
        jne    SHORT loc_1_1149
        push   dword ptr [esi + 40h]
        call   dword ptr [esi + 0ch]
        add    esp, 4
loc_1_1132:
        pop    esi
        pop    edi
        pop    ebx
        sti
        ret
loc_1_1137:
        mov    eax, dword ptr [esi + 70h]
        db  08Bh, 004h, 085h, 096h, 021h, 000h, 000h
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        jmp    SHORT loc_1_1132
loc_1_1149:
        call   sub_1_0337
        jmp    SHORT loc_1_1132
sub_1_1150:
        sub    esp, 8
        mov    ecx, dword ptr [ebx + 14h]
        mov    dword ptr [esp + 4], ecx
        mov    edi, dword ptr [ebx + 21h]
        mov    dword ptr [esp], edi
        mov    eax, dword ptr [ebx + 0ch]
        or     eax, eax
        je     SHORT loc_1_11A7
loc_1_1167:
        mov    edx, esi
        mov    esi, dword ptr [ebx + 8]
        mov    edi, dword ptr [esp]
        cmp    word ptr [edx + 68h], 0
        jne    SHORT loc_1_1178
        xchg   esi, edi
loc_1_1178:
        cmp    eax, ecx
        jae    SHORT loc_1_117E
        mov    ecx, eax
loc_1_117E:
        add    dword ptr [ebx + 8], ecx
        sub    dword ptr [ebx + 0ch], ecx
        sub    dword ptr [esp + 4], ecx
        add    dword ptr [esp], ecx
        cld
        test   ecx, 3
        jne    SHORT loc_1_119B
        shr    ecx, 2
        rep movsd
        jmp    SHORT loc_1_119D
loc_1_119B:
        rep movsb
loc_1_119D:
        mov    esi, edx
        mov    ecx, dword ptr [esp + 4]
        or     ecx, ecx
        je     SHORT loc_1_11C5
loc_1_11A7:
        add    dword ptr [ebx], 8
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebx + 8], ecx
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [esi + 8]
        mov    cl, byte ptr [ecx + 3ch]
        shl    eax, cl
        mov    dword ptr [ebx + 0ch], eax
        mov    ecx, dword ptr [esp + 4]
        jmp    SHORT loc_1_1167
loc_1_11C5:
        add    esp, 8
        ret
sub_1_11C9:
        mov    edi, dword ptr [esi + 8]
        mov    cl, byte ptr [edi + 3ch]
        mov    eax, 0ffffh
        shr    eax, cl
        dec    eax
        mov    ecx, dword ptr [ebx + 18h]
        sub    ecx, eax
        sbb    edx, edx
        and    ecx, edx
        add    ecx, eax
        mov    dword ptr [esi + 7ch], ecx
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi + 80h], eax
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ebx + 14h], edx
        mov    edx, dword ptr [eax]
        mov    dword ptr [ebx + 10h], edx
        mov    ecx, dword ptr [ebx + 25h]
        or     ecx, ecx
        je     SHORT loc_1_120D
        mov    edx, dword ptr [eax]
        sub    edx, ecx
        mov    dword ptr [eax], ecx
        mov    cl, byte ptr [edi + 3ch]
        shl    edx, cl
        add    dword ptr [eax + 4], edx
loc_1_120D:
        mov    ecx, dword ptr [esi + 7ch]
        xor    edx, edx
loc_1_1212:
        add    edx, dword ptr [eax]
        cmp    edx, ecx
        jae    SHORT loc_1_121D
        add    eax, 8
        jmp    SHORT loc_1_1212
loc_1_121D:
        jne    SHORT loc_1_1224
        cmp    edx, 7eh
        je     SHORT loc_1_1269
loc_1_1224:
        mov    dword ptr [ebx], eax
        sub    edx, ecx
        mov    edi, dword ptr [eax]
        mov    dword ptr [ebx + 8], edi
        mov    edi, dword ptr [eax + 4]
        mov    dword ptr [ebx + 0ch], edi
        or     edx, edx
        je     SHORT loc_1_124A
        cmp    dword ptr [eax], edx
        jne    SHORT loc_1_124A
        xor    edx, edx
        mov    dword ptr [ebx + 25h], edx
        xchg   dword ptr [eax], edx
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [ebx + 21h], edx
        jmp    SHORT loc_1_1257
loc_1_124A:
        sub    dword ptr [eax], edx
        mov    dword ptr [ebx + 25h], edx
        xor    edx, edx
        xchg   dword ptr [eax + 8], edx
        mov    dword ptr [ebx + 21h], edx
loc_1_1257:
        push   esi
        call   sub_1_14DA
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_1266
        clc
loc_1_1265:
        ret
loc_1_1266:
        stc
        jmp    SHORT loc_1_1265
loc_1_1269:
        add    eax, 8
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_1212
        jmp    SHORT loc_1_1224
loc_1_1273:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 6ah], 16h
        mov    edx, dword ptr [eax + 14h]
        sub    edx, 8
        mov    dword ptr [eax + 14h], edx
        push   eax
        call   dword ptr [edx]
        add    esp, 4
        ret
sub_1_128D:
        test   eax, 40000080h
        jne    SHORT loc_1_12A4
        cmp    al, 1ah
        jae    SHORT loc_1_1307
        db  08Bh, 01Ch, 085h, 0ACh, 020h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_1_1307
        ret
loc_1_12A4:
        sub    al, 80h
        jb     SHORT loc_1_12BF
        cmp    al, 18h
        ja     SHORT loc_1_1307
        push   eax
        and    eax, 0ffh
        db  08Bh, 01Ch, 085h, 014h, 021h, 000h, 000h
        pop    eax
        or     ebx, ebx
        je     SHORT loc_1_1307
        ret
loc_1_12BF:
        add    al, 80h
        cmp    al, 8
        jae    SHORT loc_1_1307
        and    eax, 0ffh
        db  08Bh, 01Ch, 085h, 074h, 021h, 000h, 000h
        or     eax, 40000000h
        or     ebx, ebx
        je     SHORT loc_1_12DB
loc_1_12DA:
        ret
loc_1_12DB:
        cmp    al, 1ah
        jae    SHORT loc_1_1307
        and    eax, 0ffh
        db  08Bh, 01Ch, 085h, 0ACh, 020h, 000h, 000h
        or     eax, 40000000h
        or     ebx, ebx
        je     SHORT loc_1_1307
        test   dword ptr [ebx + 20h], 2000h
        je     SHORT loc_1_1307
        test   byte ptr [ebx + 1dh], 20h
        je     SHORT loc_1_12DA
        or     al, al
        je     SHORT loc_1_12DA
loc_1_1307:
        stc
        ret
IOS_Send_Next_Command:
        jmp    NEAR PTR sub_1_010B
        db  000h, 000h
sub_1_1310:
        pushad
        mov    edi, dword ptr [esp + 24h]
        movzx  ebx, word ptr [edi]
        cmp    ebx, 14h
        db  077h, 067h
        mov    word ptr [edi + 2], 0
        push   edi
        db  0FFh, 014h, 09Dh, 033h, 013h, 000h, 000h
        add    esp, 4
        popad
        ret
sub_1_1333:
        in     al, 0ch
        add    byte ptr [eax], al
        cmpsd
        db  00Dh, 000h, 000h, 08Fh, 013h
        add    byte ptr [eax], al
        loopne loc_1_1347
        add    byte ptr [eax], al
        mov    ah, 9
        add    byte ptr [eax], al
loc_1_1347:
        db  070h, 011h
        add    byte ptr [eax], al
        hlt
        adc    dword ptr [eax], eax
        db  000h, 02Eh
        adc    al, 0
        db  000h, 045h, 014h
        add    byte ptr [eax], al
        sub    dword ptr [esi], ecx
        add    byte ptr [eax], al
        cld
        push   cs
        add    byte ptr [eax], al
        db  02Dh, 011h, 000h, 000h, 0ADh
        adc    al, byte ptr [eax]
        db  000h, 013h
        adc    eax, dword ptr [eax]
        db  000h, 029h
        adc    eax, dword ptr [eax]
        db  000h, 0BEh, 012h, 000h, 000h, 097h
        adc    eax, dword ptr [eax]
        db  000h, 0C7h
        add    byte ptr cs:[eax], al
        cwde
        adc    al, 0
        db  000h, 0B0h, 014h, 000h, 000h, 0E3h
        adc    al, 0
        add    byte ptr [esi - 39h], ah
        inc    edi
        add    al, byte ptr [ecx]
        add    bl, ch
        mov    byte ptr [0a80e478ah], al
        adc    al, 75h
        dec    edx
        movzx  esi, word ptr [edi + 4]
        mov    ecx, 0ch
        call   sub_1_072B
        or     ebx, ebx
        je     SHORT loc_1_13DC
        mov    edx, dword ptr [ebx]
        mov    ecx, dword ptr [edi + 6]
        sub    ecx, 64h
        add    edx, ecx
        mov    word ptr [ebx - 2], cx
        mov    dword ptr [ecx + ebx], edx
        mov    word ptr [ecx + ebx + 0eh], 0
        mov    word ptr [ecx + ebx + 0ch], 0
        mov    dword ptr [ecx + ebx + 88h], ebx
        mov    dword ptr [ecx + ebx + 4], 0
        mov    word ptr [ebx - 8], 1
loc_1_13DC:
        mov    dword ptr [edi + 0ah], ebx
        ret
        db  0A8h, 010h, 075h, 01Eh, 08Bh, 05Fh, 00Ah, 08Dh, 043h, 0ECh, 066h, 0C7h, 040h, 010h, 014h, 000h
        db  0FAh, 08Bh, 00Dh, 0BFh, 021h, 000h, 000h, 089h, 048h, 004h, 0A3h, 0BFh, 021h, 000h, 000h, 0FBh
        db  0EBh, 0A6h, 00Fh, 0B7h, 077h, 004h, 083h, 0C6h, 014h, 0B0h, 020h, 0B9h, 00Ch, 000h, 000h, 000h
        db  0E8h, 016h, 0F3h, 0FFh, 0FFh, 00Bh, 0DBh, 075h, 08Fh, 0B0h, 001h, 0B9h, 00Ch, 000h, 000h, 000h
        db  0E8h, 006h, 0F3h, 0FFh, 0FFh, 00Bh, 0DBh, 074h, 0B3h, 0E9h, 07Ah, 0FFh, 0FFh, 0FFh
loc_1_142E:
        push   ebx
        mov    eax, dword ptr [edi + 8]
        call   sub_1_128D
        mov    dword ptr [edi + 4], ebx
        pop    ebx
        jb     SHORT loc_1_143E
        ret
loc_1_143E:
        mov    word ptr [edi + 2], 1
        ret
loc_1_1445:
        push   ebx
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        mov    ecx, dword ptr [ecx + 30h]
        jecxz  loc_1_1488
        mov    ebx, dword ptr [edi + 4]
        mov    al, byte ptr [edi + 0ch]
        cmp    al, 0ffh
        je     SHORT loc_1_146F
        or     ebx, ebx
        jne    SHORT loc_1_1466
        mov    ebx, ecx
loc_1_1461:
        cmp    byte ptr [ebx + 40h], al
        je     SHORT loc_1_1475
loc_1_1466:
        mov    ebx, dword ptr [ebx + 0ch]
        or     ebx, ebx
        je     SHORT loc_1_1488
        jmp    SHORT loc_1_1461
loc_1_146F:
        or     ebx, ebx
        jne    SHORT loc_1_1481
        mov    ebx, ecx
loc_1_1475:
        mov    dword ptr [edi + 8], ebx
        mov    word ptr [edi + 2], 0
        clc
        pop    ebx
        ret
loc_1_1481:
        mov    ebx, dword ptr [ebx + 0ch]
        or     ebx, ebx
        jne    SHORT loc_1_1475
loc_1_1488:
        stc
        mov    word ptr [edi + 2], 1
        mov    dword ptr [edi + 8], 0
        pop    ebx
        ret
loc_1_1498:
        pushad
        push   186a0h
        call   sub_1_1B9A
        add    esp, 4
        popad
        ret
sub_1_14A8:
        push   esi
        push   edi
        push   ebx
        push   ecx
        mov    esi, dword ptr [ebx + 8]
        or     edx, edx
        je     SHORT loc_1_14C6
loc_1_14B3:
        cmp    dword ptr [esi], edx
        je     SHORT loc_1_14C3
        mov    esi, dword ptr [esi + 0ch]
        or     esi, esi
        jne    SHORT loc_1_14B3
        mov    esi, dword ptr [ebx + 8]
        jmp    SHORT loc_1_14C6
loc_1_14C3:
        mov    esi, dword ptr [esi + 0ch]
loc_1_14C6:
        mov    dword ptr [edi + 10h], esi
        mov    esi, edi
        or     byte ptr [esi + 6dh], 4
        pop    ecx
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi + 4], eax
        jmp    NEAR PTR loc_1_0B26
sub_1_14DA:
        push   ebx
        push   edi
        mov    ebx, dword ptr [esp + 0ch]
        mov    edi, dword ptr [ebx + 8]
        add    ebx, 64h
        jmp    SHORT loc_1_14FA
loc_1_14E8:
        push   ebx
        push   edi
        mov    ebx, dword ptr [esp + 0ch]
        mov    edi, dword ptr [ebx + 44h]
        mov    edi, dword ptr [edi + 1ch]
        sub    edi, 0c3h
loc_1_14FA:
        test   byte ptr [edi + 21h], 20h
        jne    SHORT loc_1_1502
        mov    edi, dword ptr [edi]
loc_1_1502:
        and    byte ptr [ebx + 4eh], 0feh
        test   dword ptr [edi + 1ch], 600h
        jne    SHORT loc_1_1579
loc_1_150F:
        mov    cl, byte ptr [edi + 3ch]
        mov    eax, dword ptr [ebx + 18h]
        test   byte ptr [ebx + 9], 2
        jne    SHORT loc_1_151D
        shl    eax, cl
loc_1_151D:
        test   byte ptr [edi + 21h], 20h
        jne    SHORT loc_1_1560
loc_1_1523:
        cmp    dword ptr [edi + 50h], eax
        mov    al, 1
        jb     NEAR PTR loc_1_15E1
        test   dword ptr [edi + 1ch], 800h
        jne    NEAR PTR loc_1_1600
        test   dword ptr [ebx + 8], 2
        je     SHORT loc_1_155B
        movzx  ecx, byte ptr [edi + 77h]
        shl    ecx, 1
        sub    ecx, 1
        mov    edi, dword ptr [ebx + 1ch]
        add    edi, 8
        xor    eax, eax
        repne scasd
        mov    al, 1
        jne    SHORT loc_1_155D
loc_1_155B:
        xor    eax, eax
loc_1_155D:
        pop    edi
        pop    ebx
        ret
loc_1_1560:
        mov    ecx, dword ptr [ebx + 1ch]
        test   dword ptr [ebx + 8], 2
        je     SHORT loc_1_156F
        mov    ecx, dword ptr [ecx + 4]
loc_1_156F:
        and    ecx, 0fffh
        add    eax, ecx
        jmp    SHORT loc_1_1523
loc_1_1579:
        mov    al, 3
        test   dword ptr [edi + 1ch], 400h
        jne    SHORT loc_1_1586
        mov    al, 1
loc_1_1586:
        mov    cl, byte ptr [edi + 3ch]
        test   dword ptr [ebx + 8], 2
        je     SHORT loc_1_15BA
        push   esi
        mov    esi, dword ptr [ebx + 1ch]
loc_1_1596:
        test   byte ptr [esi + 4], al
        jne    SHORT loc_1_15EA
        mov    edx, dword ptr [esi]
        test   dword ptr [ebx + 8], 200h
        jne    SHORT loc_1_15A8
        shl    edx, cl
loc_1_15A8:
        test   al, dl
        jne    SHORT loc_1_15EA
        add    esi, 8
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_1596
        pop    esi
        jmp    NEAR PTR loc_1_150F
loc_1_15BA:
        cmp    word ptr [ebx + 4], 0fh
        je     NEAR PTR loc_1_150F
        test   byte ptr [ebx + 1ch], al
        jne    SHORT loc_1_15EB
        mov    edx, dword ptr [ebx + 18h]
        test   dword ptr [ebx + 8], 200h
        jne    SHORT loc_1_15D8
        shl    edx, cl
loc_1_15D8:
        test   al, dl
        jne    SHORT loc_1_15EB
        jmp    NEAR PTR loc_1_150F
loc_1_15E1:
        or     byte ptr [ebx + 0bh], 8
        jmp    NEAR PTR loc_1_155D
loc_1_15EA:
        pop    esi
loc_1_15EB:
        or     byte ptr [ebx + 4eh], 1
        mov    al, 1
        test   byte ptr [edi + 1dh], 8
        jne    NEAR PTR loc_1_150F
        jmp    NEAR PTR loc_1_155D
loc_1_1600:
        push   esi
        push   ebp
        push   edi
        mov    eax, dword ptr [ebx + 18h]
        test   dword ptr [ebx + 8], 200h
        jne    SHORT loc_1_1614
        mov    cl, byte ptr [edi + 3ch]
        shl    eax, cl
loc_1_1614:
        cmp    eax, 80000h
        ja     NEAR PTR loc_1_1750
        shr    eax, 0ah
        add    eax, 15h
        sub    esp, eax
        mov    ebp, esp
        mov    dword ptr [ebp], eax
        mov    byte ptr [ebp + 0bh], cl
        mov    al, 11h
        mov    ah, byte ptr [edi + 77h]
        cmp    al, ah
        jb     SHORT loc_1_163A
        mov    al, ah
loc_1_163A:
        mov    byte ptr [ebp + 4], al
        mov    edi, dword ptr [ebx + 48h]
        xor    ecx, ecx
        mov    byte ptr [ebx + 4ch], cl
        mov    edx, dword ptr [ebx + 18h]
        mov    eax, dword ptr [ebx + 1ch]
        test   dword ptr [ebx + 8], 2
        je     SHORT loc_1_165B
loc_1_1654:
        mov    edx, dword ptr [eax + ecx*8]
        mov    eax, dword ptr [eax + ecx*8 + 4]
loc_1_165B:
        push   ecx
        push   ebx
        test   dword ptr [ebx + 8], 200h
        jne    SHORT loc_1_166B
        mov    cl, byte ptr [ebp + 0bh]
        shl    edx, cl
loc_1_166B:
        mov    esi, eax
        mov    dword ptr [ebp + 5], edx
        shr    edx, 0ch
        add    edx, 2
        shr    eax, 0ch
        lea    ecx, [ebp + 0ch]
        push   0
        push   ecx
        push   edx
        push   eax
        db  0CDh, 020h, 061h, 000h, 001h, 000h
        add    esp, 10h
        xor    ebx, ebx
        mov    ecx, 0ffffffffh
loc_1_1691:
        inc    ecx
        sub    dword ptr [ebp + 5], ebx
        mov    eax, esi
        and    eax, 0fffh
        mov    ebx, dword ptr [ebp + ecx*4 + 0ch]
        and    ebx, 0fffff000h
        add    ebx, eax
        jecxz  loc_1_171C
        mov    edx, dword ptr [edi - 8]
        add    edx, dword ptr [edi - 4]
        cmp    ebx, edx
        jne    SHORT loc_1_171C
        cmp    dword ptr [edi - 4], 0efffh
        jae    SHORT loc_1_171C
        mov    ebx, 1000h
        sub    ebx, eax
        add    dword ptr [edi - 4], ebx
        add    esi, ebx
        cmp    ebx, dword ptr [ebp + 5]
        jb     SHORT loc_1_1691
        sub    ebx, dword ptr [ebp + 5]
        sub    dword ptr [edi - 4], ebx
loc_1_16D4:
        pop    ebx
        pop    ecx
        test   dword ptr [ebx + 8], 2
        je     SHORT loc_1_16ED
        inc    ecx
        mov    eax, dword ptr [ebx + 1ch]
        cmp    dword ptr [eax + ecx*8], 0
        jne    NEAR PTR loc_1_1654
loc_1_16ED:
        add    esp, dword ptr [ebp]
        pop    edi
        pop    ebp
        pop    esi
        test   byte ptr [ebx + 4eh], 1
        jne    SHORT loc_1_170F
loc_1_16F9:
        or     byte ptr [ebx + 0bh], 4
        test   dword ptr [edi + 1ch], 1000h
        je     NEAR PTR loc_1_155B
        jmp    NEAR PTR loc_1_1579
loc_1_170F:
        cmp    byte ptr [ebx + 4ch], 1
        je     SHORT loc_1_16F9
        mov    al, 1
        jmp    NEAR PTR loc_1_155D
loc_1_171C:
        mov    dword ptr [edi], ebx
        pop    ebx
        inc    byte ptr [ebx + 4ch]
        push   ebx
        mov    bl, byte ptr [ebx + 4ch]
        cmp    bl, byte ptr [ebp + 4]
        ja     SHORT loc_1_174B
        mov    ebx, 1000h
        sub    ebx, eax
        mov    dword ptr [edi + 4], ebx
        add    esi, ebx
        add    edi, 8
        cmp    ebx, dword ptr [ebp + 5]
        jb     NEAR PTR loc_1_1691
        mov    ebx, dword ptr [ebp + 5]
        mov    dword ptr [edi - 4], ebx
        jmp    SHORT loc_1_16D4
loc_1_174B:
        pop    ebx
        pop    ecx
        add    esp, dword ptr [ebp]
loc_1_1750:
        mov    al, 1
        pop    edi
        pop    ebp
        pop    esi
        jmp    NEAR PTR loc_1_15E1
sub_1_175A:
        pushad
        xor    eax, eax
        db  08Dh, 035h, 0A2h, 01Fh, 000h, 000h
loc_1_1763:
        mov    esi, dword ptr [esi + 2]
        or     esi, esi
        je     SHORT loc_1_1795
        cmp    dword ptr [esi + 8], 0
        je     SHORT loc_1_1763
        mov    ebx, dword ptr [esi + 0ch]
loc_1_1773:
        mov    dword ptr [edi + 4], ebx
        pushad
        push   edi
        call   dword ptr [esi + 8]
        add    esp, 4
        popad
        cmp    word ptr [edi + 2], 0
        jne    SHORT loc_1_1788
        mov    al, 1
loc_1_1788:
        or     ebx, ebx
        je     SHORT loc_1_1763
        mov    ebx, dword ptr [ebx + 4]
        or     ebx, ebx
        jne    SHORT loc_1_1773
        jmp    SHORT loc_1_1763
loc_1_1795:
        mov    dword ptr [esp + 1ch], eax
        popad
        ret
sub_1_179B:
        pushfd
        cli
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        mov    edx, dword ptr [ecx + 40h]
        mov    dword ptr [ecx + 40h], eax
        mov    dword ptr [eax + 24h], edx
        popfd
        ret
sub_1_17AE:
        pushfd
        cli
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        lea    eax, [ecx + 40h]
        sub    eax, 24h
        mov    ecx, dword ptr [ecx + 40h]
loc_1_17BF:
        jecxz  loc_1_17D5
        cmp    ecx, edi
        je     SHORT loc_1_17CC
        mov    eax, ecx
        mov    ecx, dword ptr [ecx + 24h]
        jmp    SHORT loc_1_17BF
loc_1_17CC:
        mov    ecx, dword ptr [ecx + 24h]
        mov    dword ptr [eax + 24h], ecx
        popfd
        clc
        ret
loc_1_17D5:
        popfd
        stc
        ret
sub_1_17D8:
        cli
        call   sub_1_1E5C
        jae    SHORT loc_1_1802
        mov    ebp, eax
        db  0FEh, 005h, 0BEh, 021h, 000h, 000h
        db  0A1h, 0BAh, 021h, 000h, 000h
        mov    ecx, 5
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        mov    eax, ebp
        db  0FEh, 00Dh, 0BEh, 021h, 000h, 000h
        jmp    SHORT sub_1_17D8
loc_1_1802:
        sti
        ret
IOSIdleStatus:
        xor    eax, eax
        db  08Bh, 00Dh, 0BFh, 021h, 000h, 000h
loc_1_180C:
        or     ecx, ecx
        je     SHORT loc_1_182A
        movzx  edx, word ptr [ecx + 12h]
        add    edx, ecx
        add    edx, 14h
        test   byte ptr [ecx + 10h], 4
        je     SHORT loc_1_1825
        cmp    dword ptr [edx + 4], 0
        jne    SHORT loc_1_1867
loc_1_1825:
        mov    ecx, dword ptr [ecx + 4]
        jmp    SHORT loc_1_180C
loc_1_182A:
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        add    ecx, 6eh
loc_1_1833:
        movzx  edx, word ptr [ecx + 0eh]
        or     edx, edx
        je     SHORT loc_1_1868
        cmp    word ptr [ecx + 0ch], 1
        jne    SHORT loc_1_185B
        test   word ptr [ecx + 10h], 4
        je     SHORT loc_1_185B
        push   edx
        movzx  edx, word ptr [ecx + 12h]
        add    edx, ecx
        add    edx, 14h
        cmp    dword ptr [edx + 4], 0
        pop    edx
        jne    SHORT loc_1_1867
loc_1_185B:
        cmp    dword ptr [ecx + 8], 0
        je     SHORT loc_1_1868
        add    edx, ecx
        mov    ecx, edx
        jmp    SHORT loc_1_1833
loc_1_1867:
        dec    eax
loc_1_1868:
        ret
        db  02Eh, 08Bh, 0C0h
IOS_Signal_Semaphore_No_Switch:
        db  0CDh, 020h, 031h, 081h, 001h, 000h
        mov    eax, eax
IOS_Set_Async_Time_Out:
        db  0CDh, 020h, 0FEh, 080h, 001h, 000h
IOS_Exclusive_Access:
        push   esi
        push   ebx
        push   ebp
        mov    ebp, ebx
        push   eax
        and    eax, 400000ffh
        call   sub_1_128D
        pop    eax
        xchg   ebx, ebp
        jb     SHORT loc_1_18CB
        cmp    byte ptr [ebp + 40h], 0ah
        stc
        jne    SHORT loc_1_18CB
        jecxz  loc_1_18AB
        and    byte ptr [ebp + 24h], 0feh
        test   eax, 100h
        je     SHORT loc_1_18A7
        or     byte ptr [ebp + 24h], 1
loc_1_18A7:
        mov    ecx, ebx
        xor    ebx, ebx
loc_1_18AB:
        cli
        cmp    dword ptr [ebp + 41h], ebx
        stc
        jne    SHORT loc_1_18CB
        mov    dword ptr [ebp + 41h], ecx
        mov    ebx, dword ptr [ebp + 18h]
loc_1_18B8:
        or     ebx, ebx
        je     SHORT loc_1_18CB
        and    byte ptr [ebx + 5], 7fh
        jecxz  loc_1_18C6
        or     byte ptr [ebx + 5], 80h
loc_1_18C6:
        mov    ebx, dword ptr [ebx + 24h]
        jmp    SHORT loc_1_18B8
loc_1_18CB:
        sti
        pop    ebp
        pop    ebx
        pop    esi
        ret
sub_1_18D0:
        mov    edx, dword ptr [esp + 4]
        cli
        cmp    byte ptr [edx + 76h], 0
        jne    SHORT loc_1_18F6
        test   byte ptr [edx + 22h], 20h
        je     SHORT loc_1_1906
        cmp    dword ptr [edx + 0fbh], 0
        jne    SHORT loc_1_1937
        cmp    dword ptr [edx + 0f7h], 0
        jne    SHORT loc_1_191A
loc_1_18F3:
        xor    eax, eax
        ret
loc_1_18F6:
        mov    eax, dword ptr [edx + 0f7h]
        or     eax, eax
        je     SHORT loc_1_18F3
        test   byte ptr [eax + 6dh], 8
        je     SHORT loc_1_18F3
loc_1_1906:
        cmp    dword ptr [edx + 0f7h], 0
        jne    SHORT loc_1_191A
        cmp    dword ptr [edx + 0fbh], 0
        jne    SHORT loc_1_1937
        jmp    SHORT loc_1_18F3
loc_1_191A:
        and    byte ptr [edx + 22h], 0dfh
        mov    eax, dword ptr [edx + 0f7h]
loc_1_1924:
        mov    ecx, dword ptr [eax + 64h]
        mov    dword ptr [edx + 0f7h], ecx
        or     ecx, ecx
        jne    SHORT loc_1_195C
        or     byte ptr [edx + 22h], 20h
        jmp    SHORT loc_1_195C
loc_1_1937:
        mov    eax, dword ptr [edx + 0f7h]
        or     eax, eax
        jne    SHORT loc_1_1981
loc_1_1941:
        or     byte ptr [edx + 22h], 20h
        mov    eax, dword ptr [edx + 0fbh]
        mov    ecx, dword ptr [eax + 64h]
        mov    dword ptr [edx + 0fbh], ecx
        or     ecx, ecx
        jne    SHORT loc_1_195C
        and    byte ptr [edx + 22h], 0dfh
loc_1_195C:
        cmp    word ptr [eax + 68h], 2
        ja     SHORT loc_1_1975
        mov    ecx, dword ptr [eax + 7ch]
        test   byte ptr [eax + 6dh], 2
        jne    SHORT loc_1_1976
loc_1_196C:
        add    ecx, dword ptr [eax + 74h]
        mov    dword ptr [edx + 10bh], ecx
loc_1_1975:
        ret
loc_1_1976:
        xchg   ecx, eax
        push   ecx
        mov    cl, byte ptr [edx + 3ch]
        shr    eax, cl
        pop    ecx
        xchg   ecx, eax
        jmp    SHORT loc_1_196C
loc_1_1981:
        test   byte ptr [eax + 6dh], 8
        jne    SHORT loc_1_1924
        jmp    SHORT loc_1_1941
sub_1_1989:
        push   esi
        push   ebx
        push   edi
        push   ebp
        pushfd
        mov    edx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 1ch]
        cmp    word ptr [edx + 68h], 24h
        je     SHORT loc_1_19B9
loc_1_199D:
        test   byte ptr [edx + 6dh], 8
        jne    SHORT loc_1_19DC
        movzx  ecx, byte ptr [esi + 0b9h]
        db  0FFh, 024h, 08Dh, 0B1h, 019h, 000h, 000h
        db  0EEh, 019h, 000h, 000h, 00Ch, 01Ah, 000h, 000h
loc_1_19B9:
        cli
        xor    ebp, ebp
        xchg   dword ptr [esi + 0f7h], ebp
        xor    edi, edi
        xchg   dword ptr [esi + 0fbh], edi
        call   sub_1_1A8C
        mov    edi, ebp
        call   sub_1_1A8C
        mov    edx, dword ptr [esp + 18h]
        jmp    SHORT loc_1_199D
loc_1_19DC:
        cli
        mov    ecx, dword ptr [esi + 0f7h]
        mov    dword ptr [edx + 64h], ecx
        mov    dword ptr [esi + 0f7h], edx
        jmp    SHORT loc_1_1A6A
loc_1_19EE:
        lea    ecx, [esi + 0f7h]
        cli
loc_1_19F5:
        cmp    dword ptr [ecx], 0
        je     SHORT loc_1_1A01
        mov    ecx, dword ptr [ecx]
        lea    ecx, [ecx + 64h]
        jmp    SHORT loc_1_19F5
loc_1_1A01:
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 64h], 0
        jmp    SHORT loc_1_1A6A
loc_1_1A0C:
        cmp    word ptr [edx + 68h], 2
        ja     SHORT loc_1_19EE
        mov    ecx, dword ptr [edx + 74h]
        mov    edi, dword ptr [edx + 6ch]
        and    edi, 1
        test   dword ptr [esi + 20h], 200000h
        jne    SHORT loc_1_1A50
        cmp    ecx, dword ptr [esi + 10bh]
        jae    SHORT loc_1_1A58
loc_1_1A2D:
        cli
        lea    ebx, [esi + 0fbh]
loc_1_1A34:
        mov    eax, dword ptr [ebx]
        or     eax, eax
        je     SHORT loc_1_1A65
        mov    ebp, dword ptr [eax + 6ch]
        and    ebp, 1
        cmp    edi, ebp
        ja     SHORT loc_1_1A65
        jb     SHORT loc_1_1A4B
        cmp    ecx, dword ptr [eax + 74h]
        jae    SHORT loc_1_1A65
loc_1_1A4B:
        lea    ebx, [eax + 64h]
        jmp    SHORT loc_1_1A34
loc_1_1A50:
        cmp    ecx, dword ptr [esi + 10bh]
        jbe    SHORT loc_1_1A2D
loc_1_1A58:
        cli
        lea    ebx, [esi + 0f7h]
loc_1_1A5F:
        mov    eax, dword ptr [ebx]
        or     eax, eax
        jne    SHORT loc_1_1A70
loc_1_1A65:
        mov    dword ptr [ebx], edx
        mov    dword ptr [edx + 64h], eax
loc_1_1A6A:
        popfd
        pop    ebp
        pop    edi
        pop    ebx
        pop    esi
        ret
loc_1_1A70:
        test   byte ptr [eax + 6dh], 8
        jne    SHORT loc_1_1A87
        mov    ebp, dword ptr [eax + 6ch]
        and    ebp, 1
        cmp    edi, ebp
        ja     SHORT loc_1_1A65
        jb     SHORT loc_1_1A87
        cmp    ecx, dword ptr [eax + 74h]
        jbe    SHORT loc_1_1A65
loc_1_1A87:
        lea    ebx, [eax + 64h]
        jmp    SHORT loc_1_1A5F
sub_1_1A8C:
        or     edi, edi
        je     SHORT loc_1_1AAC
        mov    edx, dword ptr [edi + 64h]
        mov    ecx, dword ptr [edi + 14h]
        sub    ecx, 8
        mov    dword ptr [edi + 14h], ecx
        mov    word ptr [edi + 6ah], 11h
        push   edx
        push   edi
        call   dword ptr [ecx]
        add    esp, 4
        pop    edi
        jmp    SHORT sub_1_1A8C
loc_1_1AAC:
        ret
loc_1_1AAD:
        xor    ecx, ecx
        jmp    SHORT loc_1_1AB6
sub_1_1AB1:
        mov    ecx, 100h
loc_1_1AB6:
        pushad
        mov    esi, dword ptr [edx + 4]
        add    esi, 64h
        sub    esp, 10h
        mov    ebx, esp
        mov    dword ptr [ebx + 6], esi
        add    esi, 58h
        mov    word ptr [ebx + 4], si
        mov    byte ptr [ebx + 0eh], 0
        mov    word ptr [ebx], 2
        push   ebx
        call   sub_1_1310
        mov    ebx, dword ptr [ebx + 0ah]
        add    esp, 14h
        or     ebx, ebx
        je     SHORT loc_1_1B29
        add    ebx, dword ptr [edx + 4]
        mov    eax, dword ptr [esp + 1ch]
        mov    word ptr [ebx + 68h], ax
        xor    eax, eax
        mov    dword ptr [ebx + 64h], eax
        mov    dword ptr [ebx + 0a8h], eax
        mov    dword ptr [ebx + 7ch], eax
        mov    dword ptr [ebx + 80h], eax
        mov    ecx, dword ptr [esp + 18h]
        or     ecx, 4408h
        mov    dword ptr [ebx + 6ch], ecx
        db  0C7h, 043h, 070h, 02Bh, 01Bh, 000h, 000h
        mov    dword ptr [ebx + 0a4h], ebx
        mov    edi, ebx
        mov    ebx, edx
        xor    edx, edx
        call   sub_1_14A8
loc_1_1B29:
        popad
        ret
loc_1_1B2B:
        mov    edi, dword ptr [esp + 4]
        mov    ebx, dword ptr [edi + 4]
        mov    edi, dword ptr [edi + 88h]
        jmp    NEAR PTR sub_1_0A1E
loc_1_1B3D:
        sub    esp, 10h
        cmp    dword ptr [esp + 14h], 219h
        jne    SHORT loc_1_1B93
        mov    eax, dword ptr [esp + 18h]
        cmp    eax, 8045h
        jne    SHORT loc_1_1B76
        mov    eax, dword ptr [esp + 1ch]
        cmp    ah, 4
        jne    SHORT loc_1_1B93
        movzx  eax, al
        push   edi
        lea    edi, [esp + 4]
        mov    word ptr [edi], 14h
        mov    dword ptr [edi + 0ch], eax
        call   sub_1_175A
        pop    edi
        jmp    SHORT loc_1_1B93
loc_1_1B76:
        cmp    eax, 20h
        jne    SHORT loc_1_1B93
        db  086h, 025h, 043h, 003h, 000h, 000h
        test   ah, ah
        je     SHORT loc_1_1B93
        db  068h, 0F0h, 000h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 017h, 000h
        add    esp, 4
loc_1_1B93:
        xor    eax, eax
        inc    eax
        add    esp, 10h
        ret
sub_1_1B9A:
        mov    eax, dword ptr [esp + 4]
        sub    eax, 2
        ja     SHORT loc_1_1BA4
loc_1_1BA3:
        ret
loc_1_1BA4:
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        mov    ecx, dword ptr [ecx + 14h]
        cdq
        mul    ecx
        shrd   eax, edx, 10h
        mov    ecx, eax
loc_1_1BB6:
        loop   loc_1_1BB6
        jmp    SHORT loc_1_1BA3
        db  000h, 000h
loc_1_1BBC:
        cld
        push   eax
        push   ebx
        push   ecx
        push   edx
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        mov    eax, dword ptr [ecx + 10h]
        inc    eax
        mov    dword ptr [ecx + 10h], eax
        test   eax, 1
        jne    NEAR PTR loc_1_1D27
        mov    eax, 7
        mov    edx, 2000h
        call   sub_1_1D66
        test   dword ptr [ecx + 10h], 3
        jne    SHORT loc_1_1C00
        mov    eax, 8
        mov    edx, 2000h
        call   sub_1_1D66
loc_1_1C00:
        test   dword ptr [ecx + 10h], 7
        jne    SHORT loc_1_1C18
        mov    eax, 9
        mov    edx, 4000h
        call   sub_1_1D66
loc_1_1C18:
        test   dword ptr [ecx + 10h], 0fh
        jne    SHORT loc_1_1C96
        db  083h, 03Dh, 020h, 022h, 000h, 000h, 000h
        je     SHORT loc_1_1C66
        db  08Bh, 015h, 09Ch, 01Fh, 000h, 000h
        mov    edx, dword ptr [edx + 30h]
loc_1_1C33:
        or     edx, edx
        je     SHORT loc_1_1C66
        test   byte ptr [edx + 21h], 80h
        je     SHORT loc_1_1C61
        test   byte ptr [edx + 23h], 80h
        je     SHORT loc_1_1C61
        inc    dword ptr [edx + 0bfh]
        db  0A1h, 020h, 022h, 000h, 000h
        cmp    dword ptr [edx + 0bfh], eax
        jne    SHORT loc_1_1C61
        db  0BEh, 0AEh, 01Dh, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
loc_1_1C61:
        mov    edx, dword ptr [edx + 0ch]
        jmp    SHORT loc_1_1C33
loc_1_1C66:
        mov    eax, 0ah
        mov    edx, 8000h
        call   sub_1_1D66
        db  083h, 03Dh, 0BFh, 021h, 000h, 000h, 000h
        je     SHORT loc_1_1C96
        db  066h, 00Fh, 0BAh, 02Dh, 048h, 022h, 000h, 000h, 000h
        jb     SHORT loc_1_1C96
        push   esi
        db  0BEh, 0B7h, 01Dh, 000h, 000h
        db  0CDh, 020h, 010h, 000h, 001h, 000h
        pop    esi
loc_1_1C96:
        mov    ebx, 6eh
        db  003h, 01Dh, 09Ch, 01Fh, 000h, 000h
        cli
loc_1_1CA2:
        movzx  eax, word ptr [ebx + 0eh]
        or     eax, eax
        je     SHORT loc_1_1CE7
        cmp    word ptr [ebx + 0ch], 1
        jne    SHORT loc_1_1CD4
        test   word ptr [ebx + 10h], 4
        je     SHORT loc_1_1CD4
        movzx  edx, word ptr [ebx + 12h]
        movzx  ecx, word ptr [edx + ebx + 20h]
        jecxz  loc_1_1CD4
        loop   loc_1_1CC6
loc_1_1CC6:
        mov    word ptr [edx + ebx + 20h], cx
        or     ecx, ecx
        jne    SHORT loc_1_1CD4
        call   sub_1_1D41
loc_1_1CD4:
        cmp    dword ptr [ebx + 8], 0
        je     SHORT loc_1_1CE7
        add    eax, ebx
        cmp    eax, dword ptr [ebx + 8]
        je     SHORT loc_1_1CE3
        jmp    SHORT loc_1_1CE7
loc_1_1CE3:
        mov    ebx, eax
        jmp    SHORT loc_1_1CA2
loc_1_1CE7:
        db  08Bh, 01Dh, 0BFh, 021h, 000h, 000h
loc_1_1CED:
        or     ebx, ebx
        je     SHORT loc_1_1D27
        test   word ptr [ebx + 10h], 4
        je     SHORT loc_1_1D22
        cmp    word ptr [ebx + 0ch], 1
        jne    SHORT loc_1_1D22
        test   word ptr [ebx + 10h], 4
        je     SHORT loc_1_1D22
        movzx  edx, word ptr [ebx + 12h]
        movzx  ecx, word ptr [edx + ebx + 20h]
        jecxz  loc_1_1D22
        dec    ecx
        mov    word ptr [edx + ebx + 20h], cx
        or     ecx, ecx
        jne    SHORT loc_1_1D22
        call   sub_1_1D41
loc_1_1D22:
        mov    ebx, dword ptr [ebx + 4]
        jmp    SHORT loc_1_1CED
loc_1_1D27:
        sti
        mov    eax, 0fah
        db  0BEh, 0BCh, 01Bh, 000h, 000h
        db  0CDh, 020h, 0FEh, 000h, 001h, 000h
        or     esi, esi
        jne    SHORT loc_1_1D3C
loc_1_1D3C:
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        ret
sub_1_1D41:
        pushad
        mov    ecx, ebx
        add    ecx, edx
        add    ecx, 14h
        sub    esp, 10h
        mov    edi, esp
        mov    word ptr [edi], 5
        mov    dword ptr [edi + 0ch], ecx
        call   sub_1_175A
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_1D64
loc_1_1D62:
        popad
        ret
loc_1_1D64:
        jmp    SHORT loc_1_1D62
sub_1_1D66:
        db  08Bh, 035h, 0A4h, 01Fh, 000h, 000h
loc_1_1D6C:
        or     esi, esi
        je     SHORT loc_1_1DAD
        cmp    dword ptr [esi + 8], 0
        je     SHORT loc_1_1DA8
        test   dword ptr [esi + 38h], edx
        je     SHORT loc_1_1DA8
        mov    ebx, dword ptr [esi + 0ch]
loc_1_1D7E:
        push   eax
        push   edx
        push   ebx
        push   esi
        push   ecx
        sub    esp, 0ch
        mov    edi, esp
        mov    word ptr [edi], ax
        mov    dword ptr [edi + 4], ebx
        push   edi
        call   dword ptr [esi + 8]
        pop    edi
        add    esp, 0ch
        pop    ecx
        pop    esi
        pop    ebx
        pop    edx
        pop    eax
        or     ebx, ebx
        je     SHORT loc_1_1DA8
        mov    ebx, dword ptr [ebx + 4]
        or     ebx, ebx
        jne    SHORT loc_1_1D7E
        jmp    SHORT loc_1_1DA8
loc_1_1DA8:
        mov    esi, dword ptr [esi + 2]
        jmp    SHORT loc_1_1D6C
loc_1_1DAD:
        ret
loc_1_1DAE:
        mov    ax, 25h
        jmp    NEAR PTR loc_1_1AAD
loc_1_1DB7:
        cli
        db  080h, 025h, 048h, 022h, 000h, 000h, 0FEh
        db  08Bh, 01Dh, 0BFh, 021h, 000h, 000h
        db  0BEh, 0BBh, 021h, 000h, 000h
loc_1_1DCA:
        or     ebx, ebx
        je     SHORT loc_1_1DF4
        test   word ptr [ebx + 10h], 14h
        je     SHORT loc_1_1DDD
        mov    esi, ebx
        mov    ebx, dword ptr [ebx + 4]
        jmp    SHORT loc_1_1DCA
loc_1_1DDD:
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [esi + 4], ecx
        sti
        push   0
        push   dword ptr [ebx + 8]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        jmp    SHORT loc_1_1DB7
loc_1_1DF4:
        sti
        ret
        db  000h, 000h
loc_1_1DF8:
        dec    esi
        push   ds
        add    byte ptr [eax], al
        dec    esi
        push   ds
        add    byte ptr [eax], al
        push   esp
        push   ds
        add    byte ptr [eax], al
        push   esp
        push   ds
        add    byte ptr [eax], al
        push   eax
        push   ds
        add    byte ptr [eax], al
        push   eax
        push   ds
        add    byte ptr [eax], al
        dec    esi
        push   ds
        add    byte ptr [eax], al
        push   eax
        push   ds
        add    byte ptr [eax], al
sub_1_1E18:
        mov    eax, dword ptr [ebx + 4]
        db  003h, 005h, 025h, 022h, 000h, 000h
        xor    ecx, ecx
        cmp    byte ptr [eax + 12b9h], 0
        je     SHORT loc_1_1E2F
        or     ecx, 1
loc_1_1E2F:
        cmp    byte ptr [eax + 12b8h], 0
        je     SHORT loc_1_1E3B
        or     ecx, 2
loc_1_1E3B:
        cmp    byte ptr [eax + 321h], 0
        je     SHORT loc_1_1E47
        or     ecx, 4
loc_1_1E47:
        db  0FFh, 024h, 08Dh, 0F8h, 01Dh, 000h, 000h
loc_1_1E4E:
        clc
        ret
loc_1_1E50:
        xor    ecx, ecx
        stc
        ret
loc_1_1E54:
        or     ecx, 0ffffffffh
        stc
        ret
        db  000h, 000h, 000h
sub_1_1E5C:
        db  08Bh, 00Dh, 0BFh, 021h, 000h, 000h
loc_1_1E62:
        or     ecx, ecx
        je     SHORT loc_1_1E7F
        movzx  edx, word ptr [ecx + 12h]
        add    edx, ecx
        add    edx, 14h
        test   byte ptr [ecx + 10h], 4
        je     SHORT loc_1_1E7A
        cmp    dword ptr [edx + 4], eax
        je     SHORT loc_1_1EBB
loc_1_1E7A:
        mov    ecx, dword ptr [ecx + 4]
        jmp    SHORT loc_1_1E62
loc_1_1E7F:
        db  08Bh, 00Dh, 09Ch, 01Fh, 000h, 000h
        add    ecx, 6eh
loc_1_1E88:
        movzx  edx, word ptr [ecx + 0eh]
        or     edx, edx
        je     SHORT loc_1_1EBD
        cmp    word ptr [ecx + 0ch], 1
        jne    SHORT loc_1_1EAF
        test   word ptr [ecx + 10h], 4
        je     SHORT loc_1_1EAF
        push   edx
        movzx  edx, word ptr [ecx + 12h]
        add    edx, ecx
        add    edx, 14h
        cmp    dword ptr [edx + 4], eax
        pop    edx
        je     SHORT loc_1_1EBB
loc_1_1EAF:
        cmp    dword ptr [ecx + 8], 0
        je     SHORT loc_1_1EBD
        add    edx, ecx
        mov    ecx, edx
        jmp    SHORT loc_1_1E88
loc_1_1EBB:
        stc
        ret
loc_1_1EBD:
        clc
        ret
        db  000h
IOSMapIORSToI24:
        push   ebx
        mov    ebx, eax
        mov    eax, 100bh
        cmp    ebx, 2ch
        jae    SHORT loc_1_1ED8
        db  00Fh, 0B7h, 004h, 05Dh, 049h, 022h, 000h, 000h
loc_1_1ED8:
        jecxz  loc_1_1EE0
        cmp    al, 0bh
        jne    SHORT loc_1_1EE0
        mov    al, 0ah
loc_1_1EE0:
        pop    ebx
        ret
IOSMapIORSToI21:
        mov    eax, 14h
        sub    ecx, 0fh
        jb     SHORT loc_1_1EFB
        cmp    ecx, 1dh
        jae    SHORT loc_1_1EFB
        db  00Fh, 0B6h, 081h, 04Ch, 022h, 000h, 000h
loc_1_1EFB:
        ret
        db  03Dh, 000h, 000h, 000h, 034h, 000h, 000h, 000h, 0B0h, 000h, 000h, 000h, 0A9h, 000h, 000h, 000h
        db  0B0h, 00Ah, 000h, 000h, 018h, 002h, 000h, 000h, 00Ch, 011h, 000h, 000h, 004h, 001h, 000h, 000h
        db  058h, 015h, 000h, 000h, 07Ah, 018h, 000h, 000h, 009h, 013h, 000h, 000h, 074h, 018h, 000h, 000h
        db  06Ch, 018h, 000h, 000h, 004h, 018h, 000h, 000h, 0C0h, 01Eh, 000h, 000h, 0E2h, 01Eh, 000h, 000h
        db  048h, 001h, 000h, 000h, 00Ah, 00Ah, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 010h, 000h, 003h, 00Ah, 000h, 000h, 049h, 04Fh, 053h, 020h
        db  020h, 020h, 020h, 020h, 000h, 000h, 000h, 017h, 000h, 000h, 000h, 000h, 064h, 01Bh, 000h, 000h
        db  0D8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FCh, 01Eh, 000h, 000h, 012h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h
        db  000h, 000h, 000h
loc_1_2207:
        jae    SHORT loc_1_221B
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], bl
loc_1_221B:
        add    byte ptr [eax], al
loc_1_221D:
        add    byte ptr [eax], al
loc_1_221F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2225:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2245:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2249:
        add    byte ptr [eax], al
        add    byte ptr [edi + 1bh], dl
        pop    ds
        pop    ds
        pop    ds
        push   ds
        pop    ds
        push   ss
        adc    al, 15h
        mov    dword ptr [80b1f1fh], eax
        pop    ds
        adc    edx, dword ptr [0b2b1b01fh]
        mov    bl, 0b4h
        mov    ch, 1fh
        sub    byte ptr [edi], bl
        db  070h, 008h
        add    al, 0ch
        add    dword ptr [ecx + eax], ecx
        or     al, 1
        or     edx, dword ptr [eax]
        or     al, 1
        add    eax, dword ptr [ecx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [eax + 10c010fh]
        or     al, 1
        or     al, 1
        or     al, 1
        or     al, 1
        add    byte ptr [ebx], al
        add    al, byte ptr [eax + 10c010ch]
        or     al, 1
        or     al, 1
        or     al, 1
        or     al, 1
        or     al, 1
        or     al, 1
        or     al, 1
        sidt   [ecx + eax]
        add    byte ptr [eax], al
        db  000h
_LTEXT ends
        end
