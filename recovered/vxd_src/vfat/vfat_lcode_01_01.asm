; vfat_lcode_01_01.asm -- VFAT object 1 segment _LTEXT  (part 1/3)
.386p
_LTEXT segment dword public use32 'LCODE'

loc_1_0000:
        db  039h, 035h, 0A0h, 0B6h, 000h, 000h
        je     SHORT loc_1_0019
        db  039h, 035h, 090h, 0B6h, 000h, 000h
        je     SHORT loc_1_0022
loc_1_0010:
        db  039h, 035h, 0B8h, 0B1h, 000h, 000h
        je     SHORT loc_1_002B
        ret
loc_1_0019:
        db  083h, 025h, 0A0h, 0B6h, 000h, 000h, 000h
        jmp    8
loc_1_0022:
        db  083h, 025h, 090h, 0B6h, 000h, 000h, 000h
        jmp    SHORT loc_1_0010
loc_1_002B:
        db  0C7h, 005h, 0B4h, 0B1h, 000h, 000h, 000h, 000h, 000h, 0F0h
        ret
sub_1_0036:
        and    byte ptr [esi + 28h], al
        jne    SHORT loc_1_0048
        lea    eax, [esi + 2ch]
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_0048
        jmp    NEAR PTR sub_1_A258
loc_1_0048:
        ret
loc_1_0049:
        call   sub_1_025A
        mov    eax, eax
sub_1_0050:
        call   sub_1_00B0
        jae    SHORT loc_1_0060
        db  083h, 03Dh, 0CCh, 0B1h, 000h, 000h, 001h
        jae    SHORT loc_1_0049
loc_1_0060:
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_0064:
        db  03Bh, 01Dh, 0B4h, 0B1h, 000h, 000h
        jne    SHORT loc_1_0099
        test   al, 0f8h
        jne    SHORT loc_1_0099
        db  08Bh, 035h, 0B8h, 0B1h, 000h, 000h
        cmp    dword ptr [esi + 0ch], edi
        jne    SHORT loc_1_0099
        test   al, 4
        je     SHORT loc_1_008B
        push   eax
        push   ecx
        push   edx
        db  0CDh, 020h, 006h, 000h, 08Bh, 004h
        pop    edx
        pop    ecx
        pop    eax
loc_1_008B:
        test   al, 2
        mov    eax, dword ptr [esi + 10h]
        jne    SHORT loc_1_0093
        ret
loc_1_0093:
        db  0CDh, 020h, 007h, 080h, 08Bh, 004h
loc_1_0099:
        db  0CDh, 020h, 004h, 000h, 08Bh, 004h
        jb     SHORT loc_1_00AD
        db  089h, 01Dh, 0B4h, 0B1h, 000h, 000h
        db  089h, 035h, 0B8h, 0B1h, 000h, 000h
loc_1_00AD:
        ret
        db  08Bh, 0C0h
sub_1_00B0:
        db  08Ah, 025h, 0F8h, 0B6h, 000h, 000h
        mov    al, 0
        call   sub_1_0064
        jb     SHORT loc_1_00C4
        cmp    byte ptr [esi + 28h], 0
        ret
loc_1_00C4:
        ret
loc_1_00C5:
        call   sub_1_0272
        mov    ebx, edx
        mov    al, 10h
        jmp    SHORT loc_1_00D5
sub_1_00D0:
        mov    edi, dword ptr [ebp + 1ch]
        sub    al, al
loc_1_00D5:
        push   ebx
        sub    ebx, dword ptr [edi + 0ch]
        mov    cl, byte ptr [edi + 4eh]
        not    cl
        and    bl, cl
        add    ebx, dword ptr [edi + 0ch]
        db  08Ah, 025h, 0F8h, 0B6h, 000h, 000h
        or     al, 5
        call   sub_1_0064
        pop    edx
        jae    SHORT loc_1_0108
        test   esi, esi
        je     SHORT loc_1_00C5
        push   edi
        push   edx
        call   sub_1_1628
        pop    edx
        pop    edi
        mov    ebx, edx
        jb     SHORT sub_1_00D0
        mov    ebx, dword ptr [esi + 8]
        nop
loc_1_0108:
        cmp    byte ptr [esi + 28h], 0
        jne    NEAR PTR loc_1_01C3
        sub    edx, ebx
        bt     dword ptr [esi + 24h], edx
        jb     SHORT loc_1_0125
        mov    cl, byte ptr [edi + 4ch]
        shl    edx, cl
        add    edx, dword ptr [esi + 10h]
        mov    ebx, edx
        ret
loc_1_0125:
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
loc_1_012B:
        db  00Fh, 0BAh, 02Dh, 0C0h, 0B1h, 000h, 000h, 000h
        jae    SHORT loc_1_0141
        db  0B8h, 0C8h, 0B1h, 000h, 000h
        call   sub_1_A220
        jmp    SHORT loc_1_012B
loc_1_0141:
        db  089h, 035h, 0C4h, 0B1h, 000h, 000h
        push   edx
        push   esi
        xchg   edi, ebx
        add    edi, edx
        mov    cl, byte ptr [ebx + 4ch]
        shl    edx, cl
        add    edx, dword ptr [esi + 10h]
        mov    esi, edx
        mov    ecx, 1
        push   esi
        call   sub_1_1458
        pop    ebx
        pop    esi
        pop    ecx
        jb     SHORT loc_1_019A
        btr    dword ptr [esi + 24h], ecx
        btr    dword ptr [esi + 25h], ecx
        db  0C7h, 005h, 0C4h, 0B1h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 0C0h, 0B1h, 000h, 000h, 000h
        db  0B8h, 0C8h, 0B1h, 000h, 000h
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_018F
        call   sub_1_A258
loc_1_018F:
        mov    edi, dword ptr [esi + 0ch]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        clc
        ret
loc_1_019A:
        bts    dword ptr [esi + 24h], ecx
        bts    dword ptr [esi + 25h], ecx
        db  0C6h, 005h, 0C0h, 0B1h, 000h, 000h, 000h
        db  0B8h, 0C8h, 0B1h, 000h, 000h
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_01B8
        call   sub_1_A258
loc_1_01B8:
        mov    edi, dword ptr [esi + 0ch]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        stc
        ret
loc_1_01C3:
        call   sub_1_059A
        mov    ebx, edx
        jmp    NEAR PTR sub_1_00D0
        db  090h
sub_1_01D0:
        call   sub_1_00D0
        jb     SHORT loc_1_01D8
        ret
loc_1_01D8:
        mov    edx, dword ptr [esi + 0ch]
        cmp    byte ptr [edx + 46h], 2
        jne    SHORT loc_1_024E
        test   byte ptr [edx + 52h], 4
        jne    SHORT loc_1_024E
        test   byte ptr [ebp + 39h], 40h
        jne    SHORT loc_1_024E
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        mov    byte ptr [ebp + 34h], 0
        push   ebx
        push   esi
        sub    ebx, dword ptr [esi + 10h]
        mov    cl, byte ptr [edx + 4ch]
        shr    ebx, cl
        push   ebx
        add    ebx, dword ptr [esi + 8]
        mov    ecx, dword ptr [edx + 30h]
        cmp    byte ptr [edx + 48h], 0
        je     SHORT loc_1_0210
        neg    ecx
loc_1_0210:
        add    ebx, ecx
        call   sub_1_00D0
        jb     SHORT loc_1_0245
        mov    esi, ebx
        pop    edx
        mov    eax, dword ptr [esp]
        btr    dword ptr [eax + 24h], edx
        jae    SHORT loc_1_0250
        btr    dword ptr [eax + 25h], edx
        mov    ecx, dword ptr [eax + 0ch]
        movzx  ecx, word ptr [ecx + 20h]
        shr    ecx, 2
        push   edi
        mov    edi, dword ptr [esp + 8]
        rep movsd
        pop    edi
        pop    esi
        pop    ebx
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        jmp    SHORT loc_1_0258
loc_1_0245:
        pop    eax
        pop    esi
        pop    ebx
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
loc_1_024E:
        stc
        ret
loc_1_0250:
        pop    esi
        pop    ebx
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
loc_1_0258:
        clc
        ret
sub_1_025A:
        db  083h, 03Dh, 0CCh, 0B1h, 000h, 000h, 000h
        je     SHORT loc_1_0271
        push   eax
        db  0B8h, 0D0h, 0B1h, 000h, 000h
        call   sub_1_A220
        pop    eax
        jmp    SHORT sub_1_025A
loc_1_0271:
        ret
sub_1_0272:
        pushad
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        movzx  edi, word ptr [edi + 18h]
        db  00Fh, 0BAh, 02Dh, 0CCh, 0B1h, 000h, 000h, 000h
        jb     SHORT loc_1_02CC
        db  089h, 03Dh, 0DCh, 0B1h, 000h, 000h
        sub    eax, eax
        db  0A3h, 0D8h, 0B1h, 000h, 000h
        call   sub_1_05C6
        db  083h, 03Dh, 0D8h, 0B1h, 000h, 000h, 000h
        jne    SHORT loc_1_02AC
loc_1_02A2:
        db  0B8h, 0D4h, 0B1h, 000h, 000h
        call   sub_1_A220
loc_1_02AC:
        popad
        sub    eax, eax
        db  0A3h, 0CCh, 0B1h, 000h, 000h
        db  0A3h, 0DCh, 0B1h, 000h, 000h
        db  039h, 005h, 0D0h, 0B1h, 000h, 000h
        jne    SHORT loc_1_02C2
        ret
loc_1_02C2:
        db  0B8h, 0D0h, 0B1h, 000h, 000h
        jmp    NEAR PTR sub_1_A258
loc_1_02CC:
        db  03Bh, 03Dh, 0DCh, 0B1h, 000h, 000h
        je     SHORT loc_1_02D7
        popad
        jmp    SHORT sub_1_025A
loc_1_02D7:
        db  083h, 03Dh, 0D4h, 0B1h, 000h, 000h, 000h
        je     SHORT loc_1_02AC
        jmp    SHORT loc_1_02A2
        db  001h, 003h, 007h, 00Fh, 01Fh, 03Fh, 07Fh, 0FFh
loc_1_02EA:
        call   sub_1_0272
        mov    ebx, edx
        mov    al, 10h
        jmp    SHORT loc_1_02FD
        db  02Eh, 08Bh, 0C0h
sub_1_02F8:
        mov    edi, dword ptr [ebp + 1ch]
        sub    al, al
loc_1_02FD:
        push   ebx
        sub    ebx, dword ptr [edi + 0ch]
        mov    cl, byte ptr [edi + 4eh]
        not    cl
        and    bl, cl
        add    ebx, dword ptr [edi + 0ch]
        db  08Ah, 025h, 0F8h, 0B6h, 000h, 000h
        or     al, 5
        call   sub_1_0064
        pop    edx
        jae    SHORT loc_1_0330
        test   esi, esi
        je     SHORT loc_1_02EA
        movzx  ecx, byte ptr [edi + 4dh]
        db  08Ah, 089h, 0E1h, 002h, 000h, 000h
        mov    byte ptr [esi + 24h], cl
        mov    ebx, dword ptr [esi + 8]
        nop
loc_1_0330:
        cmp    byte ptr [esi + 28h], 0
        jne    SHORT loc_1_035B
        sub    edx, ebx
        btr    dword ptr [esi + 24h], edx
        btr    dword ptr [esi + 25h], edx
        mov    cl, byte ptr [edi + 4ch]
        shl    edx, cl
        add    edx, dword ptr [esi + 10h]
        mov    ebx, edx
        movzx  ecx, word ptr [edi + 20h]
        push   edi
        mov    edi, edx
        xor    eax, eax
        shr    ecx, 2
        cld
        rep stosd
        pop    edi
        ret
loc_1_035B:
        call   sub_1_059A
        mov    ebx, edx
        jmp    SHORT sub_1_02F8
sub_1_0364:
        test   byte ptr [ebp + 39h], 8
        setne  dh
        or     byte ptr [esi + 2bh], dh
        movzx  eax, byte ptr [ebp + 35h]
        bts    dword ptr [esi + 26h], eax
        mov    ebx, dword ptr [esi + 0ch]
sub_1_0379:
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        cld
        mov    dword ptr [esi + 14h], eax
        cmp    dword ptr [esi + 1ch], 0
        jne    SHORT loc_1_03EB
        mov    dword ptr [esi + 18h], eax
        push   edi
        mov    edi, ebx
        call   sub_1_897F
        pop    edi
        pushfd
        cli
        db  0B8h, 0FCh, 0B6h, 000h, 000h
        mov    ebx, dword ptr [eax + 20h]
        mov    dword ptr [ebx + 1ch], esi
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 20h], ebx
        mov    dword ptr [eax + 20h], esi
        or     byte ptr [esi + 2bh], 8
        db  0FFh, 005h, 050h, 0B7h, 000h, 000h
        popfd
        push   ecx
        push   edx
        db  08Ah, 025h, 0F8h, 0B6h, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 08Bh, 004h
        xchg   ecx, eax
        db  0A1h, 050h, 0B7h, 000h, 000h
        sub    edx, edx
        shl    eax, 3
        div    ecx
        pop    edx
        pop    ecx
        db  08Ah, 080h, 0E0h, 0B1h, 000h, 000h
        db  03Ah, 005h, 0E9h, 0B1h, 000h, 000h
        db  0A2h, 0E9h, 0B1h, 000h, 000h
        jbe    SHORT loc_1_03EB
        call   sub_1_8C47
loc_1_03EB:
        ret
sub_1_03EC:
        test   byte ptr [ebp + 38h], 4
        setne  dl
sub_1_03F3:
        mov    eax, dword ptr [esi + 0ch]
        sub    ebx, dword ptr [esi + 10h]
        push   ecx
        mov    cl, byte ptr [eax + 4ch]
        shr    ebx, cl
        pop    ecx
        sub    eax, eax
        bts    eax, ebx
        or     byte ptr [esi + 32h], al
        or     dl, dl
        je     SHORT loc_1_0417
        push   ecx
        push   edi
        call   sub_1_0464
        pop    edi
        pop    ecx
        jmp    SHORT loc_1_0422
loc_1_0417:
        call   sub_1_0423
        call   sub_1_0364
        clc
loc_1_0422:
        ret
sub_1_0423:
        or     al, byte ptr [esi + 2ah]
        cmp    al, byte ptr [esi + 32h]
        jne    SHORT loc_1_0436
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    dword ptr [esi + 18h], eax
        sub    al, al
loc_1_0436:
        mov    byte ptr [esi + 2ah], al
        ret
sub_1_043A:
        mov    al, 1
        shl    al, cl
        dec    al
        xchg   ebx, ecx
        shl    al, cl
        xchg   ecx, ebx
        or     byte ptr [esi + 32h], al
        or     dl, dl
        je     SHORT loc_1_0458
        push   ecx
        push   edi
        call   sub_1_04B3
        pop    edi
        pop    ecx
        jmp    SHORT loc_1_0463
loc_1_0458:
        call   sub_1_0423
        call   sub_1_0364
        clc
loc_1_0463:
        ret
sub_1_0464:
        mov    edi, dword ptr [esi + 0ch]
        mov    eax, ebx
        add    eax, dword ptr [esi + 8]
        cmp    dword ptr [edi + 28h], eax
        jbe    SHORT loc_1_0478
        mov    ecx, 1
        jmp    SHORT sub_1_04B3
loc_1_0478:
        mov    al, bl
        mov    ah, byte ptr [edi + 24h]
        and    ah, byte ptr [edi + 4eh]
        and    al, ah
        mov    bh, al
        neg    al
        add    al, ah
        inc    al
        mov    cl, bl
        mov    dh, 1
        shl    dh, cl
        mov    ah, dh
        xor    ecx, ecx
loc_1_0494:
        test   byte ptr [esi + 32h], dh
        je     SHORT loc_1_04A0
        inc    ecx
        add    dh, dh
        dec    al
        jne    SHORT loc_1_0494
loc_1_04A0:
        dec    bh
        jl     SHORT loc_1_04B0
        shr    ah, 1
        test   byte ptr [esi + 32h], ah
        je     SHORT loc_1_04B0
        inc    ecx
        dec    bl
        jmp    SHORT loc_1_04A0
loc_1_04B0:
        movzx  ebx, bl
sub_1_04B3:
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        mov    edi, dword ptr [esi + 0ch]
        mov    al, 1
        shl    al, cl
        dec    al
        xchg   bl, cl
        shl    al, cl
        xchg   bl, cl
        xor    byte ptr [esi + 32h], al
        mov    byte ptr [esi + 2ah], 0
        push   eax
        push   ebx
        push   ecx
        push   edx
        push   esi
        mov    eax, ebx
        add    ebx, dword ptr [esi + 8]
        mov    esi, dword ptr [esi + 10h]
        push   ecx
        mov    cl, byte ptr [edi + 4ch]
        shl    eax, cl
        pop    ecx
        add    esi, eax
        call   sub_1_1574
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        pushfd
        cmp    dl, 2
        je     SHORT loc_1_054B
        mov    edi, dword ptr [esi + 0ch]
        cmp    byte ptr [edi + 46h], 1
        jbe    SHORT loc_1_054B
        test   byte ptr [edi + 52h], 4
        jne    SHORT loc_1_054B
        push   eax
        mov    edx, ebx
        add    edx, dword ptr [esi + 8]
        add    ecx, edx
        movzx  eax, word ptr [edi + 26h]
        cmp    ecx, eax
        jbe    SHORT loc_1_054A
        mov    eax, dword ptr [edi + 18h]
        cmp    edx, eax
        jae    SHORT loc_1_054A
        cmp    ecx, eax
        jbe    SHORT loc_1_0521
        mov    ecx, eax
loc_1_0521:
        movzx  eax, word ptr [edi + 26h]
        cmp    edx, eax
        jae    SHORT loc_1_052B
        mov    edx, eax
loc_1_052B:
        sub    ecx, edx
        mov    ebx, edx
        sub    ebx, eax
        add    ebx, dword ptr [edi + 18h]
        sub    edx, dword ptr [esi + 8]
        push   ecx
        mov    cl, byte ptr [edi + 4ch]
        shl    edx, cl
        pop    ecx
        add    edx, dword ptr [esi + 10h]
        push   esi
        mov    esi, edx
        call   sub_1_1574
        pop    esi
loc_1_054A:
        pop    eax
loc_1_054B:
        popfd
        pushfd
        jb     SHORT loc_1_0592
loc_1_054F:
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        cli
        cmp    byte ptr [esi + 32h], 0
        je     SHORT loc_1_0565
        popfd
        pushfd
        call   sub_1_0379
        jmp    SHORT loc_1_0590
loc_1_0565:
        mov    word ptr [esi + 26h], 0
        test   byte ptr [esi + 2bh], 18h
        je     SHORT loc_1_0581
        push   esi
        mov    ebx, esi
        cli
        call   sub_1_899F
        cli
        call   sub_1_893E
        pop    esi
loc_1_0581:
        db  0B8h, 0D4h, 0B1h, 000h, 000h
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_0590
        call   sub_1_A258
loc_1_0590:
        popfd
        ret
loc_1_0592:
        or     byte ptr [esi + 24h], al
        or     byte ptr [esi + 25h], al
        jmp    SHORT loc_1_054F
sub_1_059A:
        cmp    byte ptr [esi + 28h], 0
        je     SHORT loc_1_05C3
        push   eax
        push   ecx
        lea    eax, [esi + 28h]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 0ch]
        mov    ecx, 0ffh
        call   sub_1_A20C
        pop    eax
        cmp    eax, dword ptr [esi + 0ch]
        pop    eax
        jne    SHORT loc_1_05BF
        cmp    eax, dword ptr [esi + 8]
loc_1_05BF:
        pop    ecx
        pop    eax
        jne    SHORT loc_1_05C4
loc_1_05C3:
        ret
loc_1_05C4:
        stc
        ret
sub_1_05C6:
        mov    edx, 0ffffffffh
sub_1_05CB:
        pushfd
        cli
        db  0BEh, 0FCh, 0B6h, 000h, 000h
loc_1_05D2:
        mov    esi, dword ptr [esi + 1ch]
        db  081h, 0FEh, 0FCh, 0B6h, 000h, 000h
        je     SHORT loc_1_05F2
        and    eax, eax
        je     SHORT loc_1_05EC
        cmp    dword ptr [esi + 0ch], eax
        jne    SHORT loc_1_05D2
        test   word ptr [esi + 26h], dx
        je     SHORT loc_1_05D2
loc_1_05EC:
        or     byte ptr [esi + 2bh], 4
        jmp    SHORT loc_1_05D2
loc_1_05F2:
        popfd
        mov    bl, 0
        jmp    NEAR PTR sub_1_8279
sub_1_05FA:
        mov    edx, 0ffffffffh
sub_1_05FF:
        push   eax
        call   sub_1_05CB
        pop    ecx
        db  0BFh, 048h, 0B7h, 000h, 000h
loc_1_060B:
        mov    edi, dword ptr [edi]
        cli
        db  081h, 0FFh, 048h, 0B7h, 000h, 000h
        je     SHORT loc_1_0634
        jecxz  loc_1_061C
        cmp    ecx, edi
        jne    SHORT loc_1_060B
loc_1_061C:
        mov    eax, dword ptr [edi + 68h]
        or     eax, dword ptr [edi + 80h]
        je     SHORT loc_1_060B
        lea    eax, [edi + 88h]
        call   sub_1_A220
        jmp    SHORT loc_1_060B
loc_1_0634:
        sti
        ret
sub_1_0636:
        mov    eax, dword ptr [ebp + 1ch]
        movzx  ecx, byte ptr [ebp + 35h]
        xor    edx, edx
        bts    edx, ecx
        call   sub_1_05FF
        mov    ebx, dword ptr [ebp + 1ch]
        movzx  eax, byte ptr [ebp + 35h]
        xchg   byte ptr [eax + ebx + 0bch], ah
        shr    eax, 8
        cmp    al, 10h
        cmc
        jb     SHORT loc_1_0668
        mov    eax, 27h
        call   sub_1_44AF
        clc
loc_1_0668:
        ret
loc_1_0669:
        sub    eax, eax
        mov    edx, eax
        db  0B9h, 048h, 0B7h, 000h, 000h
loc_1_0672:
        mov    ecx, dword ptr [ecx]
        db  081h, 0F9h, 048h, 0B7h, 000h, 000h
        je     SHORT loc_1_0681
        add    eax, dword ptr [ecx + 6ch]
        jmp    SHORT loc_1_0672
loc_1_0681:
        shl    eax, 0ch
        ret
sub_1_0685:
        db  066h, 083h, 03Dh, 078h, 0BDh, 000h, 000h, 000h
        je     NEAR PTR loc_1_07A9
        mov    esi, dword ptr [ebp + 1ch]
        mov    esi, dword ptr [esi + 14h]
        test   byte ptr [esi + 4], 80h
        jne    NEAR PTR loc_1_07A9
        db  03Bh, 015h, 090h, 0B8h, 000h, 000h
        jae    NEAR PTR loc_1_07A9
        mov    esi, dword ptr [ebp + 20h]
        cmp    eax, dword ptr [esi + 18h]
        lea    eax, [edx + eax]
        mov    dword ptr [esi + 18h], eax
        jne    NEAR PTR loc_1_07C1
        lea    edx, [eax + 0fffh]
        and    edx, 0fffff000h
        cmp    edx, dword ptr [esi + 28h]
        jae    SHORT loc_1_06EA
        cmp    eax, dword ptr [esi + 24h]
        jae    NEAR PTR loc_1_07A9
        mov    dword ptr [esi + 24h], edx
        mov    dword ptr [esi + 28h], edx
        mov    dword ptr [esi + 20h], 1
        jmp    SHORT loc_1_06ED
loc_1_06EA:
        mov    edx, dword ptr [esi + 28h]
loc_1_06ED:
        sub    esp, 0ch
        mov    edi, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [esp + 8], eax
        movsx  ecx, word ptr [edi + 50h]
        and    edx, ecx
        mov    dword ptr [esp], edx
        neg    ecx
        mov    dword ptr [esp + 4], ecx
loc_1_0709:
        mov    edi, dword ptr [ebp + 1ch]
        mov    ebx, dword ptr [esi + 8]
        mov    eax, dword ptr [esp]
        cmp    eax, dword ptr [ebx + 30h]
        jae    NEAR PTR loc_1_07A5
        mov    cl, byte ptr [edi + 4ch]
        shr    eax, cl
        mov    ebx, eax
        and    bl, byte ptr [edi + 24h]
        mov    cl, byte ptr [edi + 25h]
        shr    eax, cl
        mov    dword ptr [ebp + 40h], eax
        xchg   ebx, eax
        call   sub_1_7F50
        jb     SHORT loc_1_077F
        xchg   edx, eax
        call   sub_1_2102
        jae    SHORT loc_1_077F
        call   sub_1_7E5E
        jb     SHORT loc_1_077F
        sub    ebx, dword ptr [edi + 0ch]
        jb     SHORT loc_1_077F
        mov    cl, byte ptr [edi + 4eh]
        not    cl
        and    bl, cl
        add    ebx, dword ptr [edi + 0ch]
        db  08Ah, 025h, 0F8h, 0B6h, 000h, 000h
        mov    al, 1
        call   sub_1_0064
        jae    SHORT loc_1_077F
        test   esi, esi
        je     SHORT loc_1_07A5
        cli
        cmp    byte ptr [esi + 28h], 0
        jne    SHORT loc_1_077F
        db  0CDh, 020h, 00Dh, 000h, 08Bh, 004h
        jne    SHORT loc_1_077F
        sti
        push   esi
        call   sub_1_17B4
        pop    esi
        jb     SHORT loc_1_07AA
loc_1_077F:
        sti
        mov    esi, dword ptr [ebp + 20h]
        mov    eax, dword ptr [esp + 4]
        add    dword ptr [esp], eax
        add    dword ptr [esi + 28h], eax
        dec    dword ptr [esp + 8]
        jne    NEAR PTR loc_1_0709
        mov    esi, dword ptr [ebp + 20h]
        sub    cl, cl
        cmp    dword ptr [esi + 20h], 10h
        adc    cl, cl
        shl    dword ptr [esi + 20h], cl
loc_1_07A5:
        sti
        add    esp, 0ch
loc_1_07A9:
        ret
loc_1_07AA:
        cli
        cmp    byte ptr [esi + 28h], 0
        jne    SHORT loc_1_07A5
        db  0CDh, 020h, 00Dh, 000h, 08Bh, 004h
        jne    SHORT loc_1_07A5
        db  0CDh, 020h, 005h, 000h, 08Bh, 004h
        jmp    SHORT loc_1_07A5
loc_1_07C1:
        mov    dword ptr [esi + 20h], 1
        and    eax, 0fffff000h
        mov    dword ptr [esi + 24h], eax
        mov    dword ptr [esi + 28h], eax
        ret
sub_1_07D4:
        test   word ptr [ebp + 42h], 2
        jne    NEAR PTR loc_1_0888
        mov    eax, dword ptr [ebp + 1ch]
        test   word ptr [eax + 52h], 2000h
        jne    NEAR PTR loc_1_0888
        db  0FFh, 005h, 074h, 0B2h, 000h, 000h
        sub    ebx, ebx
        cmp    word ptr [esi], 4
        jbe    NEAR PTR loc_1_088D
        cmp    word ptr [esi + 2], 4
        jbe    SHORT loc_1_0885
        movzx  ecx, word ptr [esi + 2]
        sub    ecx, 4
        lea    esi, [esi + 4]
        sub    eax, eax
        xchg   word ptr [ecx + esi], ax
        push   eax
        push   ecx
        push   esi
        sub    ecx, ecx
        call   sub_1_0B81
        mov    eax, ecx
        db  08Bh, 015h, 078h, 0B2h, 000h, 000h
        sub    edx, 18h
        db  08Bh, 00Dh, 080h, 0B8h, 000h, 000h
loc_1_0833:
        add    edx, 18h
        cmp    dword ptr [edx], eax
        loopne loc_1_0833
        mov    ebx, ecx
        pop    esi
        pop    ecx
        pop    eax
        mov    word ptr [ecx + esi], ax
        jne    SHORT loc_1_0888
        cmp    word ptr [edx + 14h], cx
        jne    SHORT loc_1_0866
        mov    edi, dword ptr [edx + 10h]
        cmp    dword ptr [ebp + 1ch], edi
        jne    SHORT loc_1_0866
        mov    edi, dword ptr [edx + 8]
        test   edi, edi
        je     SHORT loc_1_0866
        mov    eax, esi
        repe cmpsb
        mov    esi, eax
        je     SHORT loc_1_0879
        movzx  ecx, word ptr [edx + 14h]
loc_1_0866:
        or     ebx, ebx
        je     SHORT loc_1_0888
        sub    eax, eax
        xchg   word ptr [ecx + esi], ax
        push   eax
        push   ecx
        push   esi
        mov    ecx, ebx
        mov    eax, dword ptr [edx]
        jmp    SHORT loc_1_0833
loc_1_0879:
        db  08Bh, 01Dh, 074h, 0B2h, 000h, 000h
        mov    dword ptr [edx + 0ch], ebx
        mov    ebx, dword ptr [edx + 4]
loc_1_0885:
        cmp    ebx, ebx
        ret
loc_1_0888:
        sub    eax, eax
        or     esp, esp
        ret
loc_1_088D:
        sub    eax, eax
        stc
        ret
sub_1_0891:
        test   word ptr [ebp + 42h], 2
        jne    NEAR PTR loc_1_0976
        mov    eax, dword ptr [ebp + 1ch]
        test   word ptr [eax + 52h], 2000h
        jne    NEAR PTR loc_1_0976
        movzx  ecx, word ptr [esi + 2]
        sub    ecx, 4
        jbe    NEAR PTR loc_1_0976
        lea    esi, [esi + 4]
        push   ecx
        push   esi
        mov    eax, ecx
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        call   sub_1_A1CE
        push   eax
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        jb     NEAR PTR loc_1_0972
        mov    esi, dword ptr [esp + 4]
        mov    edi, dword ptr [esp]
        mov    ecx, dword ptr [esp + 8]
        shr    ecx, 2
        rep movsd
        mov    ecx, dword ptr [esp + 8]
        and    ecx, 3
        rep movsb
        db  08Bh, 035h, 078h, 0B2h, 000h, 000h
        mov    edi, esi
        db  08Bh, 00Dh, 080h, 0B8h, 000h, 000h
        mov    eax, 0ffffffffh
        nop
loc_1_0908:
        cmp    eax, dword ptr [esi + 0ch]
        jbe    SHORT loc_1_0912
        mov    edi, esi
        mov    eax, dword ptr [esi + 0ch]
loc_1_0912:
        add    esi, 18h
        loop   loc_1_0908
        mov    ecx, dword ptr [esp]
        xchg   dword ptr [edi + 8], ecx
        jecxz  loc_1_0926
        xchg   ecx, eax
        call   sub_1_A1F8
        xchg   ecx, eax
loc_1_0926:
        mov    esi, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        sub    eax, eax
        xchg   word ptr [ecx + esi], ax
        push   eax
        sub    ecx, ecx
        call   sub_1_0B81
        pop    eax
        mov    word ptr [esi], ax
        mov    eax, ecx
        mov    dword ptr [edi], eax
        mov    edx, dword ptr [esp + 8]
        mov    word ptr [edi + 14h], dx
        mov    edx, dword ptr [ebp + 1ch]
        mov    dword ptr [edi + 10h], edx
        test   byte ptr [edx + 52h], 3
        mov    edx, dword ptr [ebx + 12h]
        je     SHORT loc_1_095D
        xor    edx, edx
loc_1_095D:
        mov    dx, word ptr [ebx + 1ah]
        mov    dword ptr [edi + 4], edx
        db  08Bh, 015h, 074h, 0B2h, 000h, 000h
        mov    dword ptr [edi + 0ch], edx
        lea    esp, [esp + 0ch]
        ret
loc_1_0972:
        lea    esp, [esp + 0ch]
loc_1_0976:
        sub    eax, eax
        ret
sub_1_0979:
        db  08Bh, 03Dh, 078h, 0B2h, 000h, 000h
        db  08Bh, 00Dh, 080h, 0B8h, 000h, 000h
loc_1_0985:
        or     eax, eax
        je     SHORT loc_1_098E
        cmp    dword ptr [edi + 4], eax
        jne    SHORT loc_1_09A9
loc_1_098E:
        cmp    dword ptr [edi + 10h], ebx
        jne    SHORT loc_1_09A9
        sub    edx, edx
        mov    dword ptr [edi + 10h], edx
        mov    dword ptr [edi + 0ch], edx
        xchg   dword ptr [edi + 8], edx
        and    edx, edx
        je     SHORT loc_1_09A9
        xchg   edx, eax
        call   sub_1_A1F8
        xchg   edx, eax
loc_1_09A9:
        add    edi, 18h
        loop   loc_1_0985
        ret
sub_1_09AF:
        test   word ptr [ebp + 42h], 2
        jne    SHORT loc_1_09F8
        mov    eax, dword ptr [ebp + 1ch]
        test   word ptr [eax + 52h], 2000h
        jne    SHORT loc_1_09F8
        push   8
        push   0
        call   sub_1_0B81
        mov    dword ptr [esp], ecx
        mov    esi, dword ptr [ebp + 1ch]
        mov    esi, dword ptr [esi + 90h]
loc_1_09D7:
        mov    eax, dword ptr [esp]
        sub    edx, edx
        db  0F7h, 035h, 084h, 0B8h, 000h, 000h
        shl    edx, 4
        cmp    dword ptr [edx + esi + 8], ecx
        je     SHORT loc_1_09FB
loc_1_09EB:
        shr    dword ptr [esp], 3
        dec    dword ptr [esp + 4]
        jne    SHORT loc_1_09D7
        add    esp, 8
loc_1_09F8:
        or     esp, esp
        ret
loc_1_09FB:
        cmp    dword ptr [edx + esi + 4], ebx
        jne    SHORT loc_1_09EB
        db  0FFh, 005h, 080h, 0B2h, 000h, 000h
        db  0A1h, 080h, 0B2h, 000h, 000h
        mov    dword ptr [edx + esi + 0ch], eax
        mov    eax, dword ptr [edx + esi]
        add    esp, 8
        cmp    eax, eax
        ret
sub_1_0A19:
        test   word ptr [ebp + 42h], 2
        jne    NEAR PTR loc_1_0AE1
        mov    eax, dword ptr [ebp + 1ch]
        test   byte ptr [eax + 53h], 20h
        jne    NEAR PTR loc_1_0AE1
        push   ebx
        push   edx
        push   esi
        push   edi
        push   0
        push   8
        push   0
        push   -1
        push   0
        mov    eax, esi
        lea    esi, [esi + 0b2h]
        test   byte ptr [eax + 7], 2
        jne    SHORT loc_1_0A54
        lea    esi, [eax + 96h]
loc_1_0A54:
        call   sub_1_0B9C
        mov    edi, dword ptr [ebp + 1ch]
        mov    edi, dword ptr [edi + 90h]
        mov    esi, dword ptr [ebp + 6ch]
        mov    ebx, dword ptr [esi]
        mov    eax, dword ptr [esi + 0ch]
        test   byte ptr [esi + 7], 2
        jne    SHORT loc_1_0A73
        mov    eax, dword ptr [esi + 10h]
loc_1_0A73:
        mov    dword ptr [esp + 8], ecx
        mov    dword ptr [esp + 10h], eax
loc_1_0A7B:
        mov    eax, dword ptr [esp + 8]
        sub    edx, edx
        db  0F7h, 035h, 084h, 0B8h, 000h, 000h
        shl    edx, 4
        mov    eax, dword ptr [edx + edi + 0ch]
        cmp    eax, dword ptr [esp + 4]
        jae    SHORT loc_1_0A9B
        mov    dword ptr [esp + 4], eax
        mov    dword ptr [esp], edx
loc_1_0A9B:
        cmp    dword ptr [edx + edi + 4], 0
        jne    SHORT loc_1_0AA8
        cmp    dword ptr [edx + edi], 0
        je     SHORT loc_1_0ABC
loc_1_0AA8:
        cmp    dword ptr [edx + edi + 8], ecx
        je     SHORT loc_1_0AE2
loc_1_0AAE:
        shr    dword ptr [esp + 8], 3
        dec    dword ptr [esp + 0ch]
        jne    SHORT loc_1_0A7B
        mov    edx, dword ptr [esp]
loc_1_0ABC:
        mov    dword ptr [edx + edi + 8], ecx
        mov    dword ptr [edx + edi + 4], ebx
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [edx + edi], eax
loc_1_0ACB:
        db  0FFh, 005h, 080h, 0B2h, 000h, 000h
        db  0A1h, 080h, 0B2h, 000h, 000h
        mov    dword ptr [edx + edi + 0ch], eax
        add    esp, 14h
        pop    edi
        pop    esi
        pop    edx
        pop    ebx
loc_1_0AE1:
        ret
loc_1_0AE2:
        cmp    dword ptr [edx + edi + 4], ebx
        jne    SHORT loc_1_0AAE
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [edx + edi], eax
        jne    SHORT loc_1_0AAE
        jmp    SHORT loc_1_0ACB
sub_1_0AF3:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebx + 90h]
        or     edi, edi
        je     SHORT loc_1_0B7C
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        or     ecx, eax
        je     SHORT loc_1_0B11
        or     eax, eax
        je     SHORT loc_1_0B1E
        jmp    SHORT loc_1_0B3D
loc_1_0B11:
        db  08Bh, 00Dh, 07Ch, 0B2h, 000h, 000h
        shr    ecx, 2
        rep stosd
        jmp    SHORT loc_1_0B7C
loc_1_0B1E:
        db  08Bh, 00Dh, 084h, 0B8h, 000h, 000h
        mov    edx, dword ptr [ebp + 8]
        sub    eax, eax
loc_1_0B29:
        cmp    dword ptr [edi + 4], edx
        jne    SHORT loc_1_0B36
        stosd
        stosd
        stosd
        stosd
        loop   loc_1_0B29
        jmp    SHORT loc_1_0B7C
loc_1_0B36:
        add    edi, 10h
        loop   loc_1_0B29
        jmp    SHORT loc_1_0B7C
loc_1_0B3D:
        db  08Bh, 00Dh, 084h, 0B8h, 000h, 000h
        push   ebx
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 14h]
        mov    esi, dword ptr [ebp + 10h]
loc_1_0B4E:
        cmp    dword ptr [edi], eax
        je     SHORT loc_1_0B5D
        cmp    dword ptr [edi], edx
        je     SHORT loc_1_0B73
loc_1_0B56:
        add    edi, 10h
        loop   loc_1_0B4E
        jmp    SHORT loc_1_0B7A
loc_1_0B5D:
        cmp    dword ptr [edi + 4], ebx
        jne    SHORT loc_1_0B56
loc_1_0B62:
        and    dword ptr [edi + 4], 0
        and    dword ptr [edi], 0
        and    dword ptr [edi + 8], 0
        and    dword ptr [edi + 0ch], 0
        jmp    SHORT loc_1_0B56
loc_1_0B73:
        cmp    dword ptr [edi + 4], esi
        jne    SHORT loc_1_0B56
        jmp    SHORT loc_1_0B62
loc_1_0B7A:
        pop    esi
        pop    ebx
loc_1_0B7C:
        pop    edi
        leave
        ret    10h
sub_1_0B81:
        mov    eax, eax
loc_1_0B84:
        lodsw
        xor    al, cl
        movzx  eax, al
        shr    ecx, 8
        db  033h, 00Ch, 085h, 08Ch, 0B2h, 000h, 000h
        cmp    word ptr [esi], 0
        jne    SHORT loc_1_0B84
        ret
sub_1_0B9C:
        db  08Bh, 015h, 088h, 0B2h, 000h, 000h
        db  08Bh, 03Dh, 084h, 0B2h, 000h, 000h
        sub    eax, eax
        mov    eax, eax
loc_1_0BAC:
        lodsw
        cmp    ax, 586h
        ja     SHORT loc_1_0BBC
        movzx  ebx, byte ptr [eax + edx]
        sub    ax, word ptr [ebx + edi]
loc_1_0BBC:
        xor    al, cl
        movzx  eax, al
        shr    ecx, 8
        db  033h, 00Ch, 085h, 08Ch, 0B2h, 000h, 000h
        cmp    word ptr [esi], 0
        jne    SHORT loc_1_0BAC
        ret
        db  000h, 000h
sub_1_0BD4:
        mov    eax, dword ptr [ebp + 4ch]
sub_1_0BD7:
        mov    edi, dword ptr [ebp + 1ch]
        or     eax, eax
        je     SHORT loc_1_0BE7
        or     ax, ax
        je     NEAR PTR loc_1_0C77
loc_1_0BE7:
        mov    dword ptr [ebp + 4ch], eax
        mov    cl, byte ptr [edi + 4ch]
        sub    cl, 5
        shr    eax, cl
        sub    edx, edx
        cmp    dword ptr [ebp + 50h], edx
        jne    SHORT loc_1_0C03
        mov    ebx, dword ptr [edi + 34h]
        add    ebx, eax
        mov    byte ptr [ebp + 60h], al
        jmp    SHORT sub_1_0C48
loc_1_0C03:
        mov    dl, al
        and    dl, byte ptr [edi + 24h]
        mov    cl, byte ptr [edi + 25h]
        shr    eax, cl
loc_1_0C0D:
        mov    byte ptr [ebp + 60h], dl
        mov    ebx, dword ptr [ebp + 54h]
        cmp    eax, dword ptr [ebp + 58h]
        je     SHORT loc_1_0C41
        ja     SHORT loc_1_0C29
        mov    ebx, dword ptr [ebp + 50h]
        mov    dword ptr [ebp + 58h], 0
        mov    dword ptr [ebp + 54h], ebx
        jmp    SHORT loc_1_0C0D
loc_1_0C29:
        sub    eax, dword ptr [ebp + 58h]
        add    dword ptr [ebp + 58h], eax
        xchg   ecx, eax
loc_1_0C30:
        push   ecx
        call   sub_1_1F2C
        pop    ecx
        jb     SHORT loc_1_0C6D
        loop   loc_1_0C30
        mov    dword ptr [ebp + 54h], ebx
        mov    dl, byte ptr [ebp + 60h]
loc_1_0C41:
        call   sub_1_7E5E
        jb     SHORT loc_1_0C75
sub_1_0C48:
        mov    dword ptr [ebp + 5ch], ebx
        call   sub_1_00D0
        jb     SHORT loc_1_0C75
        mov    dword ptr [ebp + 64h], esi
        movzx  edx, word ptr [edi + 20h]
        add    edx, ebx
        mov    eax, dword ptr [ebp + 4ch]
        shl    eax, 5
        and    ax, word ptr [edi + 22h]
        and    eax, 0ffffh
        add    ebx, eax
        ret
loc_1_0C6D:
        mov    ebx, dword ptr [ebp + 50h]
        call   sub_1_1201
loc_1_0C75:
        stc
        ret
loc_1_0C77:
        pushad
        mov    ebx, dword ptr [ebp + 50h]
        or     ebx, ebx
        je     SHORT loc_1_0C84
        call   sub_1_2129
loc_1_0C84:
        popad
        jae    NEAR PTR loc_1_0BE7
        jmp    SHORT loc_1_0C6D
sub_1_0C8D:
        mov    esi, dword ptr [ebp + 64h]
        xor    ecx, ecx
        test   byte ptr [edi + 52h], 3
        jne    SHORT loc_1_0C9B
        mov    ecx, dword ptr [ebx + 12h]
loc_1_0C9B:
        mov    cx, word ptr [ebx + 1ah]
        cmp    ecx, 2
        jb     SHORT loc_1_0CDC
        cmp    dword ptr [edi + 2ch], ecx
        jb     SHORT loc_1_0CDC
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        mov    edx, dword ptr [ebp + 20h]
        mov    edx, dword ptr [edx + 8]
        sub    eax, eax
        mov    dword ptr [edx + 10h], eax
        mov    dword ptr [edx + 30h], eax
        mov    dword ptr [edx + 50h], eax
        mov    eax, dword ptr [edx + 54h]
        sub    eax, 8
        mov    dword ptr [edx + 5ch], eax
        push   ebx
        mov    ebx, ecx
        call   sub_1_7D50
        pop    ebx
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        ret
loc_1_0CDC:
        clc
        ret
sub_1_0CDE:
        inc    dword ptr [ebp + 4ch]
        cmp    word ptr [ebp + 4ch], 0
        je     SHORT loc_1_0D3C
loc_1_0CE8:
        add    ebx, 20h
        cmp    ebx, edx
        jb     SHORT loc_1_0D58
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 1ch]
        inc    byte ptr [ebp + 60h]
        mov    al, byte ptr [ebp + 60h]
        sub    al, byte ptr [ebp + 61h]
        je     SHORT loc_1_0D0C
        mov    ebx, dword ptr [ebp + 5ch]
        inc    ebx
loc_1_0D03:
        call   sub_1_0C48
        jae    SHORT loc_1_0D56
        jmp    SHORT loc_1_0D39
loc_1_0D0C:
        mov    byte ptr [ebp + 60h], al
        mov    ebx, dword ptr [ebp + 54h]
        test   ebx, ebx
        je     SHORT loc_1_0D39
        dec    dword ptr [ebp + 4ch]
        call   sub_1_1F2C
        jb     SHORT loc_1_0D39
        call   sub_1_2102
        jae    SHORT loc_1_0D51
        inc    dword ptr [ebp + 4ch]
        mov    dword ptr [ebp + 54h], ebx
        inc    dword ptr [ebp + 58h]
        sub    edx, edx
        call   sub_1_7E5E
        jae    SHORT loc_1_0D03
loc_1_0D39:
        pop    edi
        pop    esi
        ret
loc_1_0D3C:
        pushad
        mov    ebx, dword ptr [ebp + 54h]
        or     ebx, ebx
        je     SHORT loc_1_0D4C
        mov    edi, dword ptr [ebp + 1ch]
        call   sub_1_2129
loc_1_0D4C:
        popad
        jae    SHORT loc_1_0CE8
        jmp    SHORT loc_1_0D53
loc_1_0D51:
        pop    edi
        pop    esi
loc_1_0D53:
        cmp    eax, eax
        ret
loc_1_0D56:
        pop    edi
        pop    esi
loc_1_0D58:
        test   esp, esp
        ret
sub_1_0D5B:
        jb     NEAR PTR loc_1_0E01
        test   byte ptr [ebp + 44h], 8
        je     SHORT loc_1_0D71
        test   byte ptr [ebp + 42h], 40h
        jne    NEAR PTR loc_1_10B5
loc_1_0D71:
        mov    edi, dword ptr [ebp + 1ch]
        pushfd
        cmp    dword ptr [ebp + 64h], 0
        je     SHORT loc_1_0D86
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        pop    esi
loc_1_0D86:
        popfd
        push   ebx
        push   edx
        call   sub_1_0C8D
        pop    edx
        pop    ebx
        pushfd
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        pop    esi
        popfd
        jb     NEAR PTR loc_1_10B5
        mov    al, byte ptr [ebp + 44h]
        mov    byte ptr [ebx + 0bh], al
        push   ebx
        push   edx
        lea    edi, [ebx + 0dh]
        pushfd
        cmp    dword ptr [ebp + 64h], 0
        je     SHORT loc_1_0DC0
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        pop    esi
loc_1_0DC0:
        popfd
        stc
        call   sub_1_12EA
        pushfd
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        pop    esi
        popfd
        pop    edx
        pop    ebx
        sub    eax, eax
        mov    dword ptr [ebx + 1ch], eax
        bt     dword ptr [ebp + 44h], 4
        sbb    eax, eax
        mov    word ptr [ebx + 1ah], ax
        mov    word ptr [ebx + 14h], ax
        mov    esi, dword ptr [ebp + 64h]
        push   ebx
        push   edx
        call   sub_1_03EC
        pop    edx
        pop    ebx
        mov    eax, 0
        mov    ecx, 0
        ret
loc_1_0E01:
        sub    esp, 1ch
        sub    eax, eax
        mov    dword ptr [esp + 18h], eax
        mov    dword ptr [esp + 4], eax
        mov    dword ptr [esp + 10h], eax
        mov    dword ptr [esp + 14h], eax
        mov    dword ptr [esp], ecx
        mov    byte ptr [esp + 1bh], al
        mov    esi, dword ptr [ebp + 6ch]
        mov    al, 1
        test   byte ptr [ebp + 42h], 20h
        je     SHORT loc_1_0E37
        mov    al, byte ptr [esi + 9]
        cmp    al, 2
        ja     SHORT loc_1_0E37
        test   byte ptr [esi + 8], 3fh
        jne    SHORT loc_1_0E37
        mov    al, 1
loc_1_0E37:
        mov    byte ptr [esp + 18h], al
        mov    dword ptr [esp + 14h], eax
        mov    byte ptr [esp + 19h], al
        mov    byte ptr [esi + 9], al
        movzx  ecx, al
        mov    byte ptr [esp + 1ah], 0
        cmp    al, 1
        jne    SHORT loc_1_0E61
        mov    eax, dword ptr [esi + 18h]
        cmp    eax, -1
        je     SHORT loc_1_0E75
        mov    byte ptr [esp + 1ah], 1
        jmp    SHORT loc_1_0EB7
loc_1_0E61:
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, -1
        je     SHORT loc_1_0E75
        cmp    cl, byte ptr [esi + 0bh]
        ja     SHORT loc_1_0E75
        mov    byte ptr [esp + 1ah], 1
        jmp    SHORT loc_1_0EB7
loc_1_0E75:
        cmp    dword ptr [ebp + 50h], 0
        jne    SHORT loc_1_0E9D
        mov    eax, dword ptr [esi + 1ch]
        cmp    eax, -1
        je     NEAR PTR loc_1_10B2
        add    ecx, eax
        mov    edi, dword ptr [ebp + 1ch]
        cmp    cx, word ptr [edi + 44h]
        ja     NEAR PTR loc_1_10B2
        mov    byte ptr [esp + 1ah], 1
        jmp    SHORT loc_1_0EB7
loc_1_0E9D:
        inc    byte ptr [esp + 18h]
        mov    eax, dword ptr [esi + 1ch]
        cmp    eax, -1
        jne    SHORT loc_1_0EB7
        mov    edi, dword ptr [ebp + 1ch]
        call   sub_1_10C1
        jb     NEAR PTR loc_1_10A5
loc_1_0EB7:
        mov    dword ptr [esp + 10h], eax
        mov    dword ptr [ebp + 4ch], eax
        movzx  ecx, byte ptr [esp + 19h]
        lea    ecx, [ecx + eax - 1]
        mov    dword ptr [esp + 0ch], ecx
        call   sub_1_0BD7
        jb     NEAR PTR loc_1_10A5
        mov    eax, edx
        sub    eax, ebx
        shr    eax, 5
        cmp    byte ptr [esp + 18h], al
        ja     SHORT loc_1_0F5C
        mov    ecx, dword ptr [esp]
        pushfd
        cmp    dword ptr [ebp + 64h], 0
        je     SHORT loc_1_0EFC
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        pop    esi
        inc    byte ptr [esp + 1fh]
loc_1_0EFC:
        popfd
        push   ebx
        call   sub_1_8F7F
        pop    ebx
        pushfd
        cmp    byte ptr [esp + 1fh], 0
        je     SHORT loc_1_0F1B
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        pop    esi
        dec    byte ptr [esp + 1fh]
loc_1_0F1B:
        popfd
        cmp    byte ptr [esp + 1ah], 0
        jne    SHORT loc_1_0F47
        movzx  ecx, byte ptr [esp + 18h]
        dec    ecx
        shl    ecx, 5
        add    ecx, ebx
        xor    eax, eax
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx + 0ch], eax
        mov    dword ptr [ecx + 10h], eax
        mov    dword ptr [ecx + 14h], eax
        mov    dword ptr [ecx + 18h], eax
        mov    dword ptr [ecx + 1ch], eax
loc_1_0F47:
        mov    esi, dword ptr [ebp + 64h]
        push   ebx
        call   sub_1_03EC
        pop    ebx
        jb     NEAR PTR loc_1_10A5
        jmp    NEAR PTR loc_1_1072
loc_1_0F5C:
        movzx  eax, byte ptr [esp + 18h]
        shl    eax, 5
        pushfd
        cmp    dword ptr [ebp + 64h], 0
        je     SHORT loc_1_0F76
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        pop    esi
loc_1_0F76:
        popfd
        push   eax
        push   ebx
        push   edx
        call   sub_1_A1CE
        pop    edx
        pop    ebx
        pop    ecx
        pushfd
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        pop    esi
        popfd
        jb     NEAR PTR loc_1_10A5
        mov    dword ptr [esp + 8], eax
        mov    dword ptr [esp + 4], eax
        cmp    byte ptr [esp + 1ah], 0
        jne    SHORT loc_1_0FC1
        lea    ecx, [ecx + eax - 20h]
        xor    eax, eax
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx + 0ch], eax
        mov    dword ptr [ecx + 10h], eax
        mov    dword ptr [ecx + 14h], eax
        mov    dword ptr [ecx + 18h], eax
        mov    dword ptr [ecx + 1ch], eax
loc_1_0FC1:
        mov    ecx, dword ptr [esp]
        pushfd
        cmp    dword ptr [ebp + 64h], 0
        je     SHORT loc_1_0FDA
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        pop    esi
        inc    byte ptr [esp + 1fh]
loc_1_0FDA:
        popfd
        push   ebx
        push   edx
        mov    ebx, dword ptr [esp + 0ch]
        call   sub_1_8F7F
        pop    edx
        pop    ebx
        pushfd
        cmp    byte ptr [esp + 1fh], 0
        je     SHORT loc_1_0FFF
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        pop    esi
        dec    byte ptr [esp + 1fh]
loc_1_0FFF:
        popfd
loc_1_1000:
        movzx  ecx, byte ptr [esp + 18h]
        sub    edx, ebx
        shr    edx, 5
        sub    ecx, edx
        sbb    eax, eax
        and    ecx, eax
        add    ecx, edx
        mov    byte ptr [esp + 19h], cl
        sub    byte ptr [esp + 18h], cl
        mov    esi, dword ptr [esp + 8]
        mov    edi, ebx
        shl    ecx, 3
        rep movsd
        mov    dword ptr [esp + 8], esi
        mov    esi, dword ptr [ebp + 64h]
        push   ebx
        call   sub_1_03EC
        pop    ebx
        cmp    byte ptr [esp + 18h], 0
        je     SHORT loc_1_1072
        movzx  eax, byte ptr [esp + 19h]
        add    eax, dword ptr [ebp + 4ch]
        cmp    byte ptr [esp + 1ah], 0
        jne    SHORT loc_1_1069
        mov    edi, dword ptr [ebp + 1ch]
        mov    ecx, eax
        shl    ecx, 5
        test   word ptr [edi + 4ah], cx
        jne    SHORT loc_1_1069
        cmp    byte ptr [esp + 18h], 1
        je     SHORT loc_1_1072
        dec    eax
        mov    dword ptr [ebp + 4ch], eax
        call   sub_1_10C1
        jb     SHORT loc_1_10A5
loc_1_1069:
        call   sub_1_0BD7
        jae    SHORT loc_1_1000
        jmp    SHORT loc_1_10A5
loc_1_1072:
        mov    eax, dword ptr [esp + 0ch]
        mov    edx, eax
        xchg   dword ptr [ebp + 4ch], edx
        sub    eax, edx
        jb     SHORT loc_1_109E
        shl    eax, 5
        add    ebx, eax
loc_1_1084:
        mov    ecx, dword ptr [esp + 4]
        jecxz  loc_1_1091
        xchg   ecx, eax
        call   sub_1_A1F8
        xchg   ecx, eax
loc_1_1091:
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        add    esp, 1ch
        clc
        ret
loc_1_109E:
        call   sub_1_0BD4
        jae    SHORT loc_1_1084
loc_1_10A5:
        mov    ecx, dword ptr [esp + 4]
        jecxz  loc_1_10B2
        xchg   ecx, eax
        call   sub_1_A1F8
        xchg   ecx, eax
loc_1_10B2:
        add    esp, 1ch
loc_1_10B5:
        mov    eax, 0
        mov    ecx, 0
        stc
        ret
sub_1_10C1:
        mov    ebx, dword ptr [ebp + 50h]
        and    ebx, ebx
        je     SHORT loc_1_10F8
        xor    eax, eax
loc_1_10CA:
        inc    eax
        push   ebx
        push   eax
        call   sub_1_1F2C
        pop    eax
        pop    ecx
        jbe    NEAR PTR loc_1_1187
        call   sub_1_2102
        jb     SHORT loc_1_10CA
        mov    ebx, ecx
        mov    cl, byte ptr [edi + 47h]
        mov    edx, 10000h
        sub    cl, 5
        shr    edx, cl
        cmp    eax, edx
        jae    NEAR PTR loc_1_1187
loc_1_10F8:
        mov    ecx, 1
        push   ebx
        xor    esi, esi
        call   sub_1_7816
        jb     SHORT loc_1_1166
        cmp    dword ptr [ebp + 50h], 0
        jne    SHORT loc_1_111B
        push   ebx
        call   sub_1_1201
        pop    ebx
        mov    dword ptr [ebp + 4ch], 0ffffffffh
loc_1_111B:
        xor    dl, dl
        push   ebx
        call   sub_1_7E5E
        pop    edx
        jb     SHORT loc_1_116B
        movzx  ecx, byte ptr [edi + 24h]
        test   byte ptr [edi + 52h], 10h
        je     SHORT loc_1_1149
        movzx  eax, byte ptr [edi + 0ach]
        db  00Fh, 0A3h, 005h, 068h, 0B8h, 000h, 000h
        jb     SHORT loc_1_1149
        call   sub_1_118F
        jb     SHORT loc_1_1149
        xor    ecx, ecx
loc_1_1149:
        inc    ecx
loc_1_114A:
        push   ecx
        push   edx
        push   ebx
        call   sub_1_02F8
        jb     SHORT loc_1_1168
        call   sub_1_03EC
        jb     SHORT loc_1_1168
        pop    ebx
        pop    edx
        pop    ecx
        inc    ebx
        loop   loc_1_114A
        mov    eax, dword ptr [ebp + 4ch]
        inc    eax
        clc
loc_1_1166:
        pop    ecx
        ret
loc_1_1168:
        pop    eax
        pop    edx
        pop    ecx
loc_1_116B:
        mov    edi, dword ptr [ebp + 1ch]
        cmp    byte ptr [ebp + 34h], 2bh
        jne    SHORT loc_1_1178
        mov    byte ptr [ebp + 34h], 0
loc_1_1178:
        pop    ebx
        or     ebx, ebx
        je     SHORT loc_1_1189
        mov    edx, 0ffffffffh
loc_1_1182:
        call   sub_1_7D52
loc_1_1187:
        stc
        ret
loc_1_1189:
        mov    ebx, edx
        xor    edx, edx
        jmp    SHORT loc_1_1182
sub_1_118F:
        pushad
        mov    cx, 0db08h
        push   ebx
        mov    ebx, edi
        call   sub_1_449E
        pop    ebx
        jb     SHORT loc_1_11BB
        movzx  edx, byte ptr [edi + 4dh]
        movzx  ecx, word ptr [edi + 4ah]
        inc    ecx
        shr    ecx, 0ch
        stc
        je     SHORT loc_1_11BB
loc_1_11AE:
        call   sub_1_00B0
        cmc
        jb     SHORT loc_1_11BB
        add    ebx, edx
        loop   loc_1_11AE
        clc
loc_1_11BB:
        popad
        ret
sub_1_11BD:
        mov    edi, dword ptr [ebp + 1ch]
        test   byte ptr [edi + 52h], 3
        je     SHORT loc_1_11DD
        xor    eax, eax
        mov    dword ptr [ebp + 50h], eax
        mov    byte ptr [ebp + 60h], al
        mov    eax, dword ptr [edi + 28h]
        mov    edx, dword ptr [edi + 34h]
        sub    eax, edx
        mov    byte ptr [ebp + 61h], al
        mov    dword ptr [ebp + 5ch], edx
loc_1_11DC:
        ret
loc_1_11DD:
        push   ebx
        push   ecx
        push   esi
        mov    ebx, dword ptr [edi + 3ch]
        call   sub_1_1201
        pop    esi
        pop    ecx
        pop    ebx
        jmp    SHORT loc_1_11DC
sub_1_11ED:
        mov    edi, dword ptr [ebp + 1ch]
        mov    edx, dword ptr [ebx + 12h]
        test   byte ptr [edi + 52h], 3
        je     SHORT loc_1_11FB
        xor    edx, edx
loc_1_11FB:
        mov    dx, word ptr [ebx + 1ah]
        mov    ebx, edx
sub_1_1201:
        or     ebx, ebx
        je     SHORT sub_1_11BD
        mov    dword ptr [ebp + 50h], ebx
        mov    dword ptr [ebp + 54h], ebx
        sub    edx, edx
        mov    byte ptr [ebp + 60h], dl
        mov    dword ptr [ebp + 58h], edx
        mov    edi, dword ptr [ebp + 1ch]
        mov    al, byte ptr [edi + 24h]
        inc    eax
        mov    byte ptr [ebp + 61h], al
        call   sub_1_7E5E
        mov    dword ptr [ebp + 5ch], ebx
        ret
sub_1_1226:
        call   sub_1_9F05
        movzx  eax, word ptr [edx + 0eh]
        and    eax, 204h
        cmp    eax, 204h
        je     NEAR PTR loc_1_12BF
        push   edx
        call   sub_1_12CB
        pop    edx
        mov    al, 5
        jb     SHORT loc_1_12C9
        add    esi, 45h
        mov    ecx, 0bh
        repe cmpsb
        jne    SHORT loc_1_12C0
        sub    esi, 50h
        sub    edi, 0bh
        test   byte ptr [edx + 0eh], 8
        jne    SHORT loc_1_1266
        or     byte ptr [esi + 43h], 20h
loc_1_1266:
        mov    al, byte ptr [esi + 43h]
        mov    byte ptr [edi + 0bh], al
        mov    eax, dword ptr [esi + 38h]
        mov    dword ptr [edi + 16h], eax
        mov    ecx, dword ptr [ebp + 1ch]
        bt     word ptr [ecx + 52h], 8
        cmc
        sbb    ecx, ecx
        mov    eax, ecx
        and    ax, word ptr [esi + 3ch]
        mov    word ptr [edi + 12h], ax
        mov    eax, ecx
        and    eax, dword ptr [esi + 3eh]
        mov    dword ptr [edi + 0eh], eax
        mov    eax, ecx
        and    al, byte ptr [esi + 42h]
        mov    byte ptr [edi + 0dh], al
        mov    eax, dword ptr [esi + 10h]
        mov    word ptr [edi + 1ah], ax
        shr    eax, 10h
        mov    word ptr [edi + 14h], ax
        mov    eax, dword ptr [esi + 30h]
        mov    dword ptr [edi + 1ch], eax
        push   edx
        mov    esi, ebx
        mov    ebx, edi
        call   sub_1_03EC
        pop    edx
        jb     SHORT loc_1_12BF
        or     word ptr [edx + 0eh], 204h
loc_1_12BF:
        ret
loc_1_12C0:
        sub    eax, eax
        call   sub_1_05C6
        mov    al, 2
loc_1_12C9:
        stc
        ret
sub_1_12CB:
        mov    ebx, dword ptr [esi + 1ch]
        call   sub_1_00D0
        jb     SHORT loc_1_12E9
        mov    ebx, esi
        mov    esi, dword ptr [ebp + 20h]
        mov    esi, dword ptr [esi + 8]
        mov    edi, dword ptr [ebx + 10h]
        movzx  eax, byte ptr [esi + 25h]
        shl    eax, 5
        add    edi, eax
loc_1_12E9:
        ret
sub_1_12EA:
        lea    esp, [esp - 0ch]
        mov    eax, esp
        pushfd
        db  0CDh, 020h, 077h, 000h, 040h, 000h
        sub    eax, eax
        popfd
        rcl    eax, 1
        mov    edx, dword ptr [ebp + 1ch]
        bt     word ptr [edx + 52h], 8
        rcl    eax, 1
        bt     word ptr [ebp + 42h], 0ch
        rcl    eax, 1
        lea    edi, [edi - 0dh]
        db  0FFh, 024h, 085h, 019h, 013h, 000h, 000h
        db  039h, 013h, 000h, 000h, 04Eh, 013h, 000h, 000h, 06Ah, 013h, 000h, 000h, 06Ah, 013h, 000h, 000h
        db  08Eh, 013h, 000h, 000h, 08Eh, 013h, 000h, 000h, 078h, 013h, 000h, 000h, 078h, 013h, 000h, 000h
loc_1_1339:
        mov    al, byte ptr [esp + 8]
        mov    byte ptr [edi + 0dh], al
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [edi + 0eh], eax
        mov    edx, dword ptr [esp]
        mov    eax, edx
        jmp    SHORT loc_1_13A4
loc_1_134E:
        mov    eax, dword ptr [ebp + 14h]
        mov    edx, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 8]
        call   sub_1_141D
        mov    byte ptr [edi + 0dh], al
        mov    dword ptr [edi + 0eh], edx
        mov    edx, dword ptr [esp + 4]
        mov    eax, edx
        jmp    SHORT loc_1_13A4
loc_1_136A:
        sub    eax, eax
        mov    byte ptr [edi + 0dh], al
        mov    dword ptr [edi + 0eh], eax
        mov    edx, dword ptr [esp + 4]
        jmp    SHORT loc_1_13A4
loc_1_1378:
        sub    eax, eax
        mov    edx, dword ptr [esp + 4]
        cmp    edx, dword ptr [edi + 0eh]
        jne    SHORT loc_1_13A4
        cmp    byte ptr [edi + 0dh], 0
        je     SHORT loc_1_13A4
        mov    edx, dword ptr [esp]
        jmp    SHORT loc_1_13A4
loc_1_138E:
        mov    edx, dword ptr [esp + 4]
        mov    eax, edx
        cmp    edx, dword ptr [edi + 0eh]
        jne    SHORT loc_1_13A4
        cmp    byte ptr [edi + 0dh], 0
        je     SHORT loc_1_13A4
        mov    edx, dword ptr [esp]
        mov    eax, edx
loc_1_13A4:
        mov    dword ptr [edi + 16h], edx
        shr    eax, 10h
        mov    dword ptr [edi + 12h], eax
        lea    edi, [edi + 1ah]
        lea    esp, [esp + 0ch]
        ret
sub_1_13B5:
        test   al, 1fh
        je     SHORT loc_1_13CD
        mov    ecx, eax
        and    ecx, 1e0h
        je     SHORT loc_1_13CD
        cmp    ecx, 180h
        ja     SHORT loc_1_13CD
        clc
        ret
loc_1_13CD:
        stc
        mov    eax, edx
        ret
sub_1_13D1:
        mov    ecx, eax
        and    ecx, 1fh
        cmp    ecx, 1dh
        ja     SHORT loc_1_1419
        mov    ecx, eax
        and    ecx, 7e0h
        cmp    ecx, 760h
        ja     SHORT loc_1_1419
        mov    ecx, eax
        and    ecx, 0f800h
        cmp    ecx, 0b800h
        ja     SHORT loc_1_1419
        mov    ecx, eax
        and    ecx, 1f0000h
        je     SHORT loc_1_1419
        mov    ecx, eax
        and    ecx, 1e00000h
        je     SHORT loc_1_1419
        cmp    ecx, 1800000h
        ja     SHORT loc_1_1419
        clc
        ret
loc_1_1419:
        stc
        mov    eax, edx
        ret
sub_1_141D:
        push   1312d00h
        push   edx
        push   eax
        mov    eax, edx
        sub    edx, edx
        div    dword ptr [esp + 8]
        mov    eax, dword ptr [esp]
        div    dword ptr [esp + 8]
        mov    dword ptr [esp + 8], 186a0h
        mov    eax, edx
        sub    edx, edx
        div    dword ptr [esp + 8]
        mov    dword ptr [esp + 8], eax
        db  0CDh, 020h, 03Ah, 000h, 040h, 000h
        mov    edx, eax
        mov    eax, dword ptr [esp + 8]
        add    esp, 0ch
        ret
        db  000h
sub_1_1458:
        mov    edx, edi
        mov    edi, ebx
        call   sub_1_A43B
        push   eax
        mov    word ptr [ebx + 4], 0
        add    edx, dword ptr [edi + 10h]
        mov    dword ptr [ebx + 10h], edx
        mov    dword ptr [ebx + 18h], ecx
        mov    dword ptr [ebx + 1ch], esi
        mov    dword ptr [ebx + 2ch], 0
        mov    dword ptr [ebx + 30h], 0
        db  0C7h, 043h, 00Ch, 04Fh, 015h, 000h, 000h
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        cld
        mov    dword ptr [edi + 78h], eax
        mov    eax, ebx
        push   ebx
        call   sub_1_A558
        db  0FFh, 005h, 08Ch, 0B6h, 000h, 000h
        or     dword ptr [eax + 8], 20h
        test   byte ptr [ebp + 37h], 90h
        jne    SHORT loc_1_14AF
        and    dword ptr [eax + 8], 0ffffffdfh
loc_1_14AF:
        call   sub_1_199E
        pop    ebx
loc_1_14B5:
        cli
        bt     dword ptr [ebx + 2ch], 0
        jb     SHORT loc_1_14C7
        lea    eax, [ebx + 30h]
        call   sub_1_A220
        jmp    SHORT loc_1_14B5
loc_1_14C7:
        sti
        cmp    word ptr [ebx + 6], 10h
        cmc
        jae    SHORT loc_1_14D6
        mov    al, byte ptr [ebx + 6]
        mov    byte ptr [ebp + 34h], al
loc_1_14D6:
        pop    ecx
        pushfd
        call   sub_1_A4AE
        jecxz  loc_1_14E7
        db  00Fh, 0BAh, 035h, 0F4h, 0B8h, 000h, 000h, 000h
loc_1_14E7:
        popfd
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_14EC:
        bts    word ptr [ebx + 1fh], 3
        jb     SHORT loc_1_1540
        push   edx
        mov    dx, 9
        jmp    SHORT loc_1_1508
sub_1_14FB:
        btr    word ptr [ebx + 1fh], 3
        jae    SHORT loc_1_1540
        push   edx
        mov    dx, 0ah
loc_1_1508:
        mov    eax, dword ptr [ebx + 14h]
        test   byte ptr [eax + 2], 80h
        jne    SHORT loc_1_153F
        push   ecx
        push   esi
        push   edi
        mov    edi, ebx
        xor    ah, ah
        call   sub_1_A3AF
        jb     SHORT loc_1_1541
        or     dword ptr [ebx + 8], 8
        mov    word ptr [ebx + 4], dx
        mov    dword ptr [ebx + 18h], 0
        db  0C7h, 043h, 00Ch, 048h, 015h, 000h, 000h
        mov    eax, ebx
        call   sub_1_199E
loc_1_153C:
        pop    edi
        pop    esi
        pop    ecx
loc_1_153F:
        pop    edx
loc_1_1540:
        ret
loc_1_1541:
        xor    word ptr [ebx + 1fh], 3
        jmp    SHORT loc_1_153C
loc_1_1548:
        mov    ebx, esi
        jmp    NEAR PTR sub_1_A4AE
loc_1_154F:
        cld
        db  0FFh, 00Dh, 08Ch, 0B6h, 000h, 000h
        xchg   esi, ebx
        cmp    word ptr [ebx + 6], 10h
        jae    SHORT loc_1_156B
loc_1_155F:
        or     dword ptr [ebx + 2ch], 1
        lea    eax, [ebx + 30h]
        jmp    NEAR PTR sub_1_A258
loc_1_156B:
        or     dword ptr [ebx + 2ch], 3
        jmp    SHORT loc_1_155F
        db  02Eh, 08Bh, 0C0h
sub_1_1574:
        test   byte ptr [ebp + 37h], 10h
        jne    SHORT loc_1_157F
        call   sub_1_897F
loc_1_157F:
        mov    edx, ebx
        call   sub_1_A43B
        push   eax
        mov    word ptr [ebx + 4], 1
        add    edx, dword ptr [edi + 10h]
        mov    dword ptr [ebx + 10h], edx
        mov    dword ptr [ebx + 18h], ecx
        mov    dword ptr [ebx + 1ch], esi
        mov    dword ptr [ebx + 2ch], 0
        mov    dword ptr [ebx + 30h], 0
        db  0C7h, 043h, 00Ch, 04Fh, 015h, 000h, 000h
        bt     dword ptr [ebp + 38h], 0bh
        sbb    eax, eax
        and    eax, 1000000h
        or     dword ptr [ebx + 8], eax
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        cld
        mov    dword ptr [edi + 78h], eax
        mov    eax, ebx
        push   ebx
        push   edi
        call   sub_1_A558
        db  0FFh, 005h, 08Ch, 0B6h, 000h, 000h
        or     dword ptr [eax + 8], 20h
        test   byte ptr [ebp + 37h], 90h
        jne    SHORT loc_1_15E4
        and    dword ptr [eax + 8], 0ffffffdfh
loc_1_15E4:
        call   sub_1_199E
        pop    edi
        pop    ebx
        cli
        test   byte ptr [ebx + 2ch], 1
        jne    SHORT loc_1_15FA
        lea    eax, [ebx + 30h]
        call   sub_1_A220
loc_1_15FA:
        sti
        bt     dword ptr [ebx + 2ch], 1
        jae    SHORT loc_1_1608
        mov    al, byte ptr [ebx + 6]
        mov    byte ptr [ebp + 34h], al
loc_1_1608:
        pop    ecx
        pushfd
        call   sub_1_A4AE
        jecxz  loc_1_1619
        db  00Fh, 0BAh, 035h, 0F4h, 0B8h, 000h, 000h, 000h
loc_1_1619:
        test   byte ptr [ebp + 37h], 10h
        jne    SHORT loc_1_1624
        call   sub_1_899F
loc_1_1624:
        popfd
        ret
        db  08Bh, 0C0h
sub_1_1628:
        mov    byte ptr [esi + 24h], 0ffh
        push   esi
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        call   sub_1_A43B
        pop    esi
        jae    SHORT loc_1_165E
        cmp    byte ptr [esi + 28h], 0
        jne    SHORT loc_1_1676
        cmp    byte ptr [esi + 24h], 0ffh
        jne    SHORT loc_1_1676
        cmp    byte ptr [esi + 32h], 0
        jne    SHORT loc_1_1676
        db  080h, 03Dh, 0C0h, 0B1h, 000h, 000h, 000h
        je     SHORT loc_1_165E
        db  039h, 035h, 0C4h, 0B1h, 000h, 000h
        je     SHORT loc_1_1676
loc_1_165E:
        push   ebx
        call   sub_1_1682
        pop    ebx
        clc
loc_1_1666:
        pushfd
        push   esi
        call   sub_1_A4AE
        pop    esi
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        popfd
        ret
loc_1_1676:
        stc
        jmp    SHORT loc_1_1666
        db  000h, 001h, 003h, 007h, 00Fh, 01Fh, 03Fh, 07Fh, 0FFh
sub_1_1682:
        mov    byte ptr [esi + 28h], 1
        sub    eax, eax
        mov    byte ptr [esi + 25h], al
        mov    byte ptr [esi + 24h], al
        mov    edx, dword ptr [esi + 8]
        mov    edi, dword ptr [esi + 0ch]
        mov    ecx, esi
        mov    dword ptr [ebx + 2ch], ecx
        mov    dword ptr [ebx + 10h], edx
        mov    eax, dword ptr [edi + 10h]
        add    dword ptr [ebx + 10h], eax
        db  0C7h, 043h, 00Ch, 08Ah, 017h, 000h, 000h
        movzx  eax, byte ptr [edi + 4dh]
        add    edx, eax
        sub    edx, dword ptr [edi + 8]
        jbe    SHORT loc_1_16B6
        sub    eax, edx
loc_1_16B6:
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [ebx + 18h], eax
        mov    eax, dword ptr [ecx + 10h]
        mov    dword ptr [ebx + 1ch], eax
        test   edx, edx
        jns    SHORT loc_1_16ED
        mov    edx, dword ptr [edi + 0ch]
        movzx  eax, byte ptr [edi + 4dh]
        sub    eax, edx
        add    dword ptr [ebx + 10h], eax
        sub    dword ptr [ebx + 18h], eax
        push   ecx
        db  08Ah, 0A0h, 079h, 016h, 000h, 000h
        or     byte ptr [ecx + 24h], ah
        or     byte ptr [ecx + 25h], ah
        sub    ah, ah
        mov    cl, byte ptr [edi + 4ch]
        shl    eax, cl
        pop    ecx
        add    dword ptr [ebx + 1ch], eax
loc_1_16ED:
        mov    byte ptr [ecx + 29h], 0
        or     byte ptr [ecx + 28h], 80h
        push   ecx
        push   ebx
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        cld
        mov    dword ptr [edi + 78h], eax
        xchg   ebx, eax
        call   sub_1_A558
        db  0FFh, 005h, 08Ch, 0B6h, 000h, 000h
        or     dword ptr [eax + 8], 20h
        test   byte ptr [ebp + 37h], 90h
        jne    SHORT loc_1_171B
        and    dword ptr [eax + 8], 0ffffffdfh
loc_1_171B:
        call   sub_1_199E
        mov    ebx, dword ptr [esp + 4]
        lea    eax, [ebx + 28h]
        mov    ecx, 80h
        call   sub_1_A20C
        pop    ebx
        cmp    word ptr [ebx + 6], 10h
        jae    SHORT loc_1_173B
        pop    esi
        ret
loc_1_173B:
        pop    edx
        mov    esi, dword ptr [edx + 10h]
        mov    ebx, dword ptr [edx + 0ch]
        movzx  ecx, byte ptr [ebx + 4dh]
        mov    edi, dword ptr [edx + 8]
        sub    eax, eax
loc_1_174B:
        bt     dword ptr [edx + 25h], eax
        jb     SHORT loc_1_176E
        pushad
        push   dword ptr [ebp + 34h]
        mov    ecx, 1
        call   sub_1_1458
        pop    eax
        mov    byte ptr [ebp + 34h], al
        popad
        jae    SHORT loc_1_176E
        bts    dword ptr [edx + 25h], eax
        bts    dword ptr [edx + 24h], eax
loc_1_176E:
        movzx  ebx, word ptr [ebx + 20h]
        add    esi, ebx
        mov    ebx, dword ptr [edx + 0ch]
        inc    eax
        inc    edi
        loop   loc_1_174B
        mov    esi, edx
        mov    byte ptr [esi + 28h], 0
        lea    eax, [esi + 2ch]
        call   sub_1_A258
        ret
loc_1_178A:
        cld
        db  0FFh, 00Dh, 08Ch, 0B6h, 000h, 000h
        mov    ebx, dword ptr [esi + 2ch]
        mov    cl, byte ptr [ebx + 29h]
        cmp    word ptr [esi + 6], 10h
        jae    SHORT loc_1_17AB
loc_1_179E:
        and    byte ptr [ebx + 28h], cl
        xchg   esi, ecx
        lea    eax, [ebx + 2ch]
        jmp    NEAR PTR sub_1_A258
loc_1_17AB:
        mov    cl, 7fh
        mov    byte ptr [ebx + 29h], cl
        jmp    SHORT loc_1_179E
        db  08Bh, 0C0h
sub_1_17B4:
        sub    ebx, ebx
        mov    byte ptr [esi + 25h], bl
        mov    byte ptr [esi + 24h], bl
        mov    edx, dword ptr [esi + 8]
        mov    edi, dword ptr [esi + 0ch]
        mov    ah, 41h
        mov    ecx, esi
        push   esi
        call   sub_1_A3AF
        pop    esi
        jb     SHORT loc_1_181D
        mov    byte ptr [esi + 28h], 1
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        mov    dword ptr [ebx + 2ch], esi
        mov    dword ptr [ebx + 10h], edx
        db  0C7h, 043h, 00Ch, 01Fh, 018h, 000h, 000h
        movzx  eax, byte ptr [edi + 4dh]
        add    edx, eax
        sub    edx, dword ptr [edi + 8]
        jbe    SHORT loc_1_17F3
        sub    eax, edx
loc_1_17F3:
        mov    dword ptr [ebx + 18h], eax
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [ebx + 1ch], eax
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        cld
        mov    dword ptr [edi + 78h], eax
        inc    dword ptr [edi + 64h]
        mov    eax, ebx
        call   sub_1_A558
        db  0FFh, 005h, 08Ch, 0B6h, 000h, 000h
        call   sub_1_199E
        clc
        ret
loc_1_181D:
        stc
        ret
loc_1_181F:
        cld
        db  0FFh, 00Dh, 08Ch, 0B6h, 000h, 000h
        mov    edx, esi
        mov    esi, dword ptr [edx + 2ch]
        cmp    word ptr [edx + 6], 10h
        jae    SHORT loc_1_186A
loc_1_1832:
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        mov    byte ptr [esi + 28h], 0
        lea    ebx, [edx]
        push   esi
        call   sub_1_A4AE
        pop    esi
        mov    eax, dword ptr [esi + 0ch]
        dec    dword ptr [eax + 64h]
        jne    SHORT loc_1_185C
        add    eax, 8ch
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_185C
        call   sub_1_A258
loc_1_185C:
        lea    eax, [esi + 2ch]
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_1869
        call   sub_1_A258
loc_1_1869:
        ret
loc_1_186A:
        mov    eax, dword ptr [esi + 0ch]
        test   byte ptr [eax + 1fh], 20h
        je     SHORT loc_1_188D
        db  0BFh, 0F0h, 022h, 000h, 000h
        mov    ecx, 4
        movzx  eax, word ptr [edx + 6]
        cld
        repne scasb
        mov    eax, dword ptr [esi + 0ch]
        jne    SHORT loc_1_188D
        or     byte ptr [eax + 1fh], 40h
loc_1_188D:
        movzx  eax, byte ptr [eax + 4dh]
        db  08Ah, 080h, 079h, 016h, 000h, 000h
        mov    byte ptr [esi + 24h], al
        jmp    SHORT loc_1_1832
        db  0F9h, 0C3h, 0E3h, 0FCh, 050h, 051h, 052h, 056h, 057h, 08Dh, 01Ch, 0CDh, 000h, 000h, 000h, 000h
        db  08Dh, 05Ch, 08Bh, 004h, 08Bh, 07Dh, 01Ch, 0B4h, 041h, 0E8h, 0E2h, 08Ah, 000h, 000h, 05Fh, 05Eh
        db  05Ah, 059h, 058h, 072h, 0DBh, 0C7h, 043h, 00Ch, 031h, 019h, 000h, 000h, 083h, 04Bh, 008h, 002h
        db  066h, 0C7h, 043h, 004h, 000h, 000h, 089h, 043h, 018h, 08Bh, 045h, 01Ch, 003h, 050h, 010h, 089h
        db  053h, 010h, 08Dh, 043h, 050h, 089h, 043h, 01Ch, 089h, 04Bh, 02Ch, 08Dh, 044h, 0C8h, 004h, 089h
        db  043h, 030h, 0FCh, 02Bh, 0C0h, 051h, 057h, 08Dh, 07Bh, 050h, 0D1h, 0E1h, 0F3h, 0A5h, 0ABh, 05Eh
        db  059h, 0F3h, 0A5h, 0CDh, 020h, 0CFh, 000h, 001h, 000h, 08Bh, 07Dh, 01Ch, 089h, 047h, 078h, 0FFh
        db  047h, 064h, 0FCh, 08Bh, 0C3h, 0E8h, 042h, 08Ch, 000h, 000h, 083h, 048h, 008h, 020h, 0F6h, 045h
        db  037h, 090h, 075h, 004h, 083h, 060h, 008h, 0DFh, 0FFh, 005h, 08Ch, 0B6h, 000h, 000h, 0E8h, 06Fh
        db  000h, 000h, 000h, 0F8h, 0C3h
loc_1_1931:
        db  0FFh, 00Dh, 08Ch, 0B6h, 000h, 000h
        cld
        mov    ebx, esi
        mov    esi, dword ptr [ebx + 30h]
        mov    ecx, dword ptr [ebx + 2ch]
loc_1_1940:
        lodsd
        cmp    word ptr [ebx + 6], 10h
        jb     SHORT loc_1_1958
        mov    edx, dword ptr [eax + 0ch]
        movzx  edx, byte ptr [edx + 4dh]
        db  08Ah, 092h, 079h, 016h, 000h, 000h
        mov    byte ptr [eax + 24h], dl
loc_1_1958:
        xchg   esi, eax
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        xchg   esi, eax
        mov    byte ptr [eax + 28h], 0
        loop   loc_1_1940
        mov    esi, dword ptr [ebx + 30h]
        mov    ecx, dword ptr [ebx + 2ch]
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 0ch]
        dec    dword ptr [eax + 64h]
        jne    SHORT loc_1_1986
        lea    eax, [eax + 8ch]
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_1986
        call   sub_1_A258
loc_1_1986:
        lodsd
        lea    eax, [eax + 2ch]
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_1994
        call   sub_1_A258
loc_1_1994:
        loop   loc_1_1986
        lea    ebx, [ebx]
        call   sub_1_A4AE
        ret
sub_1_199E:
        push   ebp
        mov    esi, eax
        mov    ebx, dword ptr [edi + 14h]
        mov    al, byte ptr [edi + 0ach]
        mov    byte ptr [esi + 4dh], al
        test   byte ptr [esi + 0ah], 8
        jne    SHORT loc_1_19B9
        mov    eax, dword ptr [ebx + 20h]
        add    dword ptr [esi + 10h], eax
loc_1_19B9:
        cmp    word ptr [esi + 4], 0dh
        je     SHORT loc_1_19C3
        mov    dword ptr [esi + 44h], ebx
loc_1_19C3:
        mov    edi, dword ptr [ebx + 1ch]
        db  0CDh, 020h, 004h, 000h, 010h, 000h
        cld
        pop    ebp
        ret
        db  066h, 0C7h, 043h, 004h, 000h, 000h, 003h, 057h, 010h, 089h, 053h, 010h, 089h, 04Bh, 018h, 083h
        db  04Bh, 008h, 002h, 089h, 073h, 01Ch, 0C7h, 043h, 02Ch, 000h, 000h, 000h, 000h, 0C7h, 043h, 030h
        db  000h, 000h, 000h, 000h, 0CDh, 020h, 0CFh, 000h, 001h, 000h, 0FCh, 089h, 047h, 078h, 08Bh, 0C3h
        db  053h, 0E8h, 053h, 08Bh, 000h, 000h, 0FFh, 005h, 08Ch, 0B6h, 000h, 000h, 083h, 048h, 008h, 020h
        db  0F6h, 045h, 037h, 090h, 075h, 004h, 083h, 060h, 008h, 0DFh, 0E8h, 080h, 0FFh, 0FFh, 0FFh, 05Bh
        db  0C3h, 050h, 08Bh, 0D3h, 0E8h, 013h, 08Ah, 000h, 000h, 087h, 004h, 024h, 0C7h, 043h, 00Ch, 04Fh
        db  015h, 000h, 000h, 0E8h, 098h, 0FFh, 0FFh, 0FFh, 0FAh, 00Fh, 0BAh, 063h, 02Ch, 000h, 072h, 00Ah
        db  08Dh, 043h, 030h, 0E8h, 0D9h, 087h, 000h, 000h, 0EBh, 0EEh, 0FBh, 066h, 083h, 07Bh, 006h, 010h
        db  0F5h, 073h, 006h, 08Ah, 043h, 006h, 088h, 045h, 034h, 059h, 09Ch, 0E8h, 04Fh, 08Ah, 000h, 000h
        db  0E3h, 008h, 00Fh, 0BAh, 035h, 0F4h, 0B8h, 000h, 000h, 000h, 09Dh, 0C3h, 000h
sub_1_1A6C:
        sub    esp, 28h
        mov    dword ptr [esp + 0ch], eax
        mov    dword ptr [esp + 8], edx
        mov    dword ptr [esp + 4], 0
        mov    dword ptr [esp], edi
        mov    word ptr [edi], 4
        mov    word ptr [edi + 2], 4
        mov    word ptr [edi + 4], 0
        mov    eax, 584h
        call   sub_1_A1CE
        jb     NEAR PTR loc_1_1B6C
        mov    dword ptr [esp + 4], eax
        mov    edi, eax
        mov    edx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0ch]
        call   sub_1_1B7E
        jb     NEAR PTR loc_1_1B6C
loc_1_1ABC:
        mov    esi, dword ptr [esp + 4]
        mov    eax, dword ptr [esi]
        dec    eax
        mov    edx, 0bh
        mul    edx
        lea    esi, [eax + esi + 4]
        cmp    byte ptr [esi], 5
        jne    SHORT loc_1_1AD6
        mov    byte ptr [esi], 0e5h
loc_1_1AD6:
        mov    eax, 200020h
        lea    edi, [esp + 10h]
        mov    ecx, 0bh
        rep stosw
        lea    edi, [esp + 10h]
        push   1
        push   8
        push   esi
        push   edi
        db  0CDh, 020h, 042h, 000h, 040h, 000h
        add    esp, 10h
        lea    esi, [esi + 8]
        lea    edi, [esp + 20h]
        push   1
        push   3
        push   esi
        push   edi
        db  0CDh, 020h, 042h, 000h, 040h, 000h
        add    esp, 10h
        mov    edi, dword ptr [esp]
        movzx  edx, word ptr [edi]
        mov    word ptr [edx + edi], 0
        lea    edi, [edx + edi + 2]
        lea    esi, [esp + 10h]
        push   0
        push   esi
        push   edi
        db  0CDh, 020h, 048h, 000h, 040h, 000h
        add    esp, 0ch
        mov    edi, dword ptr [esp]
        movzx  edx, word ptr [edi]
        mov    word ptr [edi + 2], dx
        add    eax, 2
        mov    word ptr [edx + edi], ax
        add    ax, word ptr [edi]
        mov    word ptr [edi], ax
        mov    word ptr [eax + edi], 0
        mov    edi, dword ptr [esp + 4]
        dec    dword ptr [edi]
        jne    NEAR PTR loc_1_1ABC
        mov    ecx, dword ptr [esp + 4]
        jecxz  loc_1_1B67
        xchg   ecx, eax
        call   sub_1_A1F8
        xchg   ecx, eax
loc_1_1B67:
        add    esp, 28h
        clc
        ret
loc_1_1B6C:
        mov    ecx, dword ptr [esp + 4]
        jecxz  loc_1_1B79
        xchg   ecx, eax
        call   sub_1_A1F8
        xchg   ecx, eax
loc_1_1B79:
        add    esp, 28h
        stc
        ret
sub_1_1B7E:
        sub    esp, 0ch
        mov    dword ptr [esp], edi
        mov    dword ptr [esp + 4], edx
        mov    dword ptr [esp + 8], eax
        mov    dword ptr [edi], 0
        mov    ebx, edx
        call   sub_1_1201
        jb     SHORT loc_1_1BED
        mov    eax, dword ptr [esp + 8]
        call   sub_1_0BD7
        jb     SHORT loc_1_1BED
        mov    al, byte ptr [ebx]
        cmp    al, 0e5h
        je     SHORT loc_1_1BED
loc_1_1BAC:
        mov    edi, dword ptr [esp]
        cmp    dword ptr [edi], 80h
        jae    SHORT loc_1_1BED
        mov    eax, 0bh
        mul    dword ptr [edi]
        inc    dword ptr [edi]
        lea    edi, [eax + edi + 4]
        lea    esi, [ebx]
        movsd
        movsd
        movsw
        movsb
        mov    edx, dword ptr [esp + 4]
        or     edx, edx
        je     SHORT loc_1_1BE8
        mov    eax, dword ptr [ebp + 1ch]
        cmp    dword ptr [eax + 3ch], edx
        je     SHORT loc_1_1BE8
        call   sub_1_1BF2
        jb     SHORT loc_1_1BED
        mov    dword ptr [esp + 4], edx
        jmp    SHORT loc_1_1BAC
loc_1_1BE8:
        add    esp, 0ch
        clc
        ret
loc_1_1BED:
        add    esp, 0ch
        stc
        ret
sub_1_1BF2:
        sub    esp, 8
        mov    ebx, dword ptr [ebp + 1ch]
        or     edx, edx
        je     NEAR PTR loc_1_1CA9
        cmp    dword ptr [ebx + 3ch], edx
        je     NEAR PTR loc_1_1CA9
        mov    ebx, edx
        call   sub_1_1201
        jb     NEAR PTR loc_1_1CA9
        sub    eax, eax
        call   sub_1_0BD7
        jb     NEAR PTR loc_1_1CA9
        cmp    dword ptr [ebx], 2020202eh
        jne    SHORT loc_1_1CA9
        cmp    dword ptr [ebx + 20h], 20202e2eh
        jne    SHORT loc_1_1CA9
        mov    eax, dword ptr [ebx + 12h]
        mov    ax, word ptr [ebx + 1ah]
        mov    ecx, dword ptr [ebx + 32h]
        mov    cx, word ptr [ebx + 3ah]
        mov    edx, dword ptr [ebp + 1ch]
        test   byte ptr [edx + 52h], 3
        je     SHORT loc_1_1C51
        movzx  eax, ax
        movzx  ecx, cx
loc_1_1C51:
        mov    dword ptr [esp + 4], eax
        mov    dword ptr [esp], ecx
        mov    ebx, ecx
        call   sub_1_1201
        jb     SHORT loc_1_1CA9
        sub    eax, eax
        call   sub_1_0BD7
        jb     SHORT loc_1_1CA9
loc_1_1C6A:
        mov    al, byte ptr [ebx]
        cmp    al, 0e5h
        je     SHORT loc_1_1C97
        cmp    al, 2eh
        je     SHORT loc_1_1C97
        cmp    al, 0
        je     SHORT loc_1_1CA9
        test   byte ptr [ebx + 0bh], 10h
        je     SHORT loc_1_1C97
        mov    eax, dword ptr [ebx + 12h]
        mov    ax, word ptr [ebx + 1ah]
        mov    ecx, dword ptr [ebp + 1ch]
        test   byte ptr [ecx + 52h], 3
        je     SHORT loc_1_1C91
        movzx  eax, ax
loc_1_1C91:
        cmp    dword ptr [esp + 4], eax
        je     SHORT loc_1_1CA0
loc_1_1C97:
        call   sub_1_0CDE
        ja     SHORT loc_1_1C6A
        jmp    SHORT loc_1_1CA9
loc_1_1CA0:
        mov    edx, dword ptr [esp]
        clc
loc_1_1CA4:
        lea    esp, [esp + 8]
        ret
loc_1_1CA9:
        stc
        jmp    SHORT loc_1_1CA4
sub_1_1CAC:
        db  083h, 03Dh, 0A8h, 0B8h, 000h, 000h, 000h
        je     SHORT loc_1_1D0D
        mov    edi, dword ptr [ebp + 1ch]
        test   word ptr [edi + 52h], 2001h
        jne    SHORT loc_1_1D0D
        mov    ebx, dword ptr [edi + 14h]
        test   byte ptr [ebx + 4], 40h
        jne    SHORT loc_1_1D0D
        test   byte ptr [ebx + 1], 40h
        jne    SHORT loc_1_1D0D
        movzx  ebx, word ptr [edi + 26h]
        push   eax
        push   dword ptr [ebp + 34h]
        call   sub_1_00D0
        pop    eax
        mov    byte ptr [ebp + 34h], al
        pop    eax
        jb     SHORT loc_1_1D0D
        add    ebx, 7
        shl    eax, 2
        test   byte ptr [edi + 52h], 3
        je     SHORT loc_1_1CF5
        sub    ebx, 4
        shl    eax, 4
loc_1_1CF5:
        not    ah
        mov    dl, byte ptr [ebx]
        and    dl, ah
        or     dl, al
        mov    byte ptr [ebx], dl
        mov    dl, 1
        push   dword ptr [ebp + 34h]
        call   sub_1_03F3
        pop    eax
        mov    byte ptr [ebp + 34h], al
loc_1_1D0D:
        ret
sub_1_1D0E:
        mov    edi, dword ptr [ebp + 1ch]
        btr    dword ptr [edi + 1fh], 4
        jae    SHORT loc_1_1D21
        mov    ax, 200h
        call   sub_1_1CAC
loc_1_1D21:
        ret
sub_1_1D22:
        mov    edi, dword ptr [ebp + 1ch]
        bts    dword ptr [edi + 1fh], 4
        jb     SHORT loc_1_1D36
        mov    ax, 2
        call   sub_1_1CAC
        cmc
loc_1_1D36:
        cmc
        ret
sub_1_1D38:
        mov    edi, dword ptr [ebp + 1ch]
        btr    dword ptr [edi + 1fh], 5
        jae    SHORT loc_1_1D4B
        mov    ax, 100h
        call   sub_1_1CAC
loc_1_1D4B:
        ret
sub_1_1D4C:
        cmp    dword ptr [edi + 2ch], ebx
        jb     SHORT loc_1_1D96
        cmp    ebx, 2
        jb     SHORT loc_1_1D96
        push   ebx
        push   edx
        push   edi
        call   sub_1_1D0E
        pop    edi
        pop    edx
        pop    ebx
        cmp    word ptr [edi + 20h], 200h
        jne    SHORT loc_1_1DA0
        test   byte ptr [edi + 52h], 2
        je     SHORT loc_1_1DA0
        cmp    bx, -8
        jae    SHORT loc_1_1D96
        push   ebx
        push   edx
        shr    ebx, 8
        add    bx, word ptr [edi + 26h]
        call   sub_1_01D0
        pop    edx
        pop    eax
        jb     SHORT loc_1_1D96
        movzx  eax, al
        mov    word ptr [ebx + eax*2], dx
        xor    edx, edx
        jmp    NEAR PTR sub_1_03F3
loc_1_1D95:
        pop    eax
loc_1_1D96:
        or     dword ptr [edi + 40h], 0ffffffffh
        mov    byte ptr [ebp + 34h], 14h
        stc
        ret
loc_1_1DA0:
        call   sub_1_2102
        jae    SHORT loc_1_1D96
        push   edx
        push   ebx
        mov    eax, ebx
        test   byte ptr [edi + 52h], 1
        je     SHORT loc_1_1DB3
        shr    eax, 1
loc_1_1DB3:
        add    eax, ebx
        test   byte ptr [edi + 52h], 3
        jne    SHORT loc_1_1DBD
        add    eax, eax
loc_1_1DBD:
        movzx  edx, word ptr [edi + 22h]
        mov    cl, byte ptr [edi + 4ch]
        and    edx, eax
        shr    eax, cl
        movzx  ecx, word ptr [edi + 26h]
        add    eax, ecx
        cmp    byte ptr [edi + 48h], 0
        je     SHORT loc_1_1DD7
        add    eax, dword ptr [edi + 30h]
loc_1_1DD7:
        push   eax
        push   edx
        db  083h, 03Dh, 0C0h, 0B1h, 000h, 000h, 000h
        jne    SHORT loc_1_1E50
        db  039h, 005h, 094h, 0B6h, 000h, 000h
        jne    SHORT loc_1_1E19
        db  08Bh, 035h, 090h, 0B6h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_1E19
        cmp    dword ptr [esi + 0ch], edi
        jne    SHORT loc_1_1E19
        db  08Bh, 00Dh, 098h, 0B6h, 000h, 000h
        cmp    dword ptr [esi + 8], ecx
        jne    SHORT loc_1_1E19
        cmp    byte ptr [esi + 24h], 0
        jne    SHORT loc_1_1E19
        db  08Bh, 01Dh, 09Ch, 0B6h, 000h, 000h
        test   dword ptr [esi + 25h], 0ff0000ffh
        je     SHORT loc_1_1E57
loc_1_1E19:
        db  039h, 005h, 0A4h, 0B6h, 000h, 000h
        jne    SHORT loc_1_1E50
        db  08Bh, 035h, 0A0h, 0B6h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_1E50
        cmp    dword ptr [esi + 0ch], edi
        jne    SHORT loc_1_1E50
        db  08Bh, 00Dh, 0A8h, 0B6h, 000h, 000h
        cmp    dword ptr [esi + 8], ecx
        jne    SHORT loc_1_1E50
        cmp    byte ptr [esi + 24h], 0
        jne    SHORT loc_1_1E50
        db  08Bh, 01Dh, 0ACh, 0B6h, 000h, 000h
        test   dword ptr [esi + 25h], 0ff0000ffh
        je     SHORT loc_1_1E57
loc_1_1E50:
        mov    ebx, eax
        call   sub_1_01D0
loc_1_1E57:
        pop    ecx
        pop    edx
        pop    eax
        jb     NEAR PTR loc_1_1D95
        db  089h, 035h, 090h, 0B6h, 000h, 000h
        db  089h, 015h, 094h, 0B6h, 000h, 000h
        mov    esi, dword ptr [esi + 8]
        db  089h, 035h, 098h, 0B6h, 000h, 000h
        db  089h, 01Dh, 09Ch, 0B6h, 000h, 000h
        db  08Bh, 035h, 090h, 0B6h, 000h, 000h
        test   byte ptr [edi + 52h], 3
        je     SHORT loc_1_1E9A
        test   byte ptr [edi + 52h], 1
        jne    SHORT loc_1_1EB1
        pop    edx
        add    ebx, ecx
        mov    word ptr [ebx], dx
loc_1_1E93:
        xor    edx, edx
        jmp    NEAR PTR sub_1_03F3
loc_1_1E9A:
        add    ebx, ecx
        pop    edx
        mov    ecx, dword ptr [ebx]
        and    edx, 0fffffffh
        and    ecx, 0f0000000h
        or     ecx, edx
        mov    dword ptr [ebx], ecx
        jmp    SHORT loc_1_1E93
loc_1_1EB1:
        cmp    word ptr [edi + 22h], cx
        je     SHORT loc_1_1EDA
        xchg   dword ptr [esp], esi
        and    si, 0fffh
        mov    edx, 0f000h
        add    ebx, ecx
        test   al, 1
        je     SHORT loc_1_1ED1
        rol    dx, 4
        shl    esi, 4
loc_1_1ED1:
        and    word ptr [ebx], dx
        or     word ptr [ebx], si
        pop    esi
        jmp    SHORT loc_1_1E93
loc_1_1EDA:
        add    ebx, ecx
        pop    ecx
        test   al, 1
        jne    SHORT loc_1_1F04
        mov    byte ptr [ebx], cl
        push   edx
        xor    edx, edx
        call   sub_1_03F3
        pop    ebx
        inc    ebx
        push   ecx
        call   sub_1_01D0
        pop    ecx
        jb     NEAR PTR loc_1_1D96
        and    ch, 0fh
        and    byte ptr [ebx], 0f0h
        or     byte ptr [ebx], ch
        jmp    SHORT loc_1_1E93
loc_1_1F04:
        shl    ecx, 4
        and    byte ptr [ebx], 0fh
        or     byte ptr [ebx], cl
        push   edx
        xor    edx, edx
        call   sub_1_03F3
        pop    ebx
        inc    ebx
        push   ecx
        call   sub_1_01D0
        pop    ecx
        jb     NEAR PTR loc_1_1D96
        mov    byte ptr [ebx], ch
        jmp    NEAR PTR loc_1_1E93
        db  08Bh, 0C0h
sub_1_1F2C:
        cmp    ebx, 2
        jb     NEAR PTR loc_1_1FC9
        cmp    dword ptr [edi + 2ch], ebx
        jb     NEAR PTR loc_1_1FC4
        cmp    word ptr [edi + 20h], 200h
        jne    NEAR PTR loc_1_1FDC
        test   byte ptr [edi + 52h], 2
        je     NEAR PTR loc_1_1FDC
        push   ebx
        shr    ebx, 8
        add    bx, word ptr [edi + 26h]
        db  039h, 01Dh, 0A4h, 0B6h, 000h, 000h
        jne    SHORT loc_1_1F95
        db  083h, 03Dh, 0C0h, 0B1h, 000h, 000h, 000h
        jne    SHORT loc_1_1F95
        db  08Bh, 035h, 0A0h, 0B6h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_1F95
        cmp    edi, dword ptr [esi + 0ch]
        jne    SHORT loc_1_1F95
        db  0A1h, 0A8h, 0B6h, 000h, 000h
        cmp    eax, dword ptr [esi + 8]
        jne    SHORT loc_1_1F95
        cmp    byte ptr [esi + 24h], 0
        jne    SHORT loc_1_1F95
        test   dword ptr [esi + 25h], 0ff0000ffh
        je     SHORT loc_1_1FCC
loc_1_1F95:
        push   ebx
        call   sub_1_01D0
        pop    ecx
        pop    eax
        jb     SHORT loc_1_1FC4
        db  089h, 035h, 0A0h, 0B6h, 000h, 000h
        db  089h, 00Dh, 0A4h, 0B6h, 000h, 000h
        db  089h, 01Dh, 0ACh, 0B6h, 000h, 000h
        mov    ecx, dword ptr [esi + 8]
        db  089h, 00Dh, 0A8h, 0B6h, 000h, 000h
        sub    ah, ah
        movzx  ebx, word ptr [ebx + eax*2]
        and    ebx, ebx
        ret
loc_1_1FC3:
        pop    eax
loc_1_1FC4:
        or     dword ptr [edi + 40h], 0ffffffffh
        stc
loc_1_1FC9:
        ret
        db  08Bh, 0C0h
loc_1_1FCC:
        db  08Bh, 01Dh, 0ACh, 0B6h, 000h, 000h
        pop    eax
        sub    ah, ah
        movzx  ebx, word ptr [ebx + eax*2]
        and    ebx, ebx
        ret
loc_1_1FDC:
        call   sub_1_2102
        jae    SHORT loc_1_1FC4
        push   ebx
        mov    eax, ebx
        test   byte ptr [edi + 52h], 1
        je     SHORT loc_1_1FEE
        shr    eax, 1
loc_1_1FEE:
        add    eax, ebx
        test   byte ptr [edi + 52h], 3
        jne    SHORT loc_1_1FF8
        add    eax, eax
loc_1_1FF8:
        movzx  edx, word ptr [edi + 22h]
        mov    cl, byte ptr [edi + 4ch]
        and    edx, eax
        shr    eax, cl
        movzx  ecx, word ptr [edi + 26h]
        add    eax, ecx
        cmp    byte ptr [edi + 48h], 0
        je     SHORT loc_1_2012
        add    eax, dword ptr [edi + 30h]
loc_1_2012:
        push   eax
        push   edx
        db  083h, 03Dh, 0C0h, 0B1h, 000h, 000h, 000h
        jne    SHORT loc_1_208B
        db  039h, 005h, 0A4h, 0B6h, 000h, 000h
        jne    SHORT loc_1_2054
        db  08Bh, 035h, 0A0h, 0B6h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_2054
        cmp    dword ptr [esi + 0ch], edi
        jne    SHORT loc_1_2054
        db  08Bh, 00Dh, 0A8h, 0B6h, 000h, 000h
        cmp    dword ptr [esi + 8], ecx
        jne    SHORT loc_1_2054
        cmp    byte ptr [esi + 24h], 0
        jne    SHORT loc_1_2054
        db  08Bh, 01Dh, 0ACh, 0B6h, 000h, 000h
        test   dword ptr [esi + 25h], 0ff0000ffh
        je     SHORT loc_1_2092
loc_1_2054:
        db  039h, 005h, 094h, 0B6h, 000h, 000h
        jne    SHORT loc_1_208B
        db  08Bh, 035h, 090h, 0B6h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_208B
        cmp    dword ptr [esi + 0ch], edi
        jne    SHORT loc_1_208B
        db  08Bh, 00Dh, 098h, 0B6h, 000h, 000h
        cmp    dword ptr [esi + 8], ecx
        jne    SHORT loc_1_208B
        cmp    byte ptr [esi + 24h], 0
        jne    SHORT loc_1_208B
        db  08Bh, 01Dh, 09Ch, 0B6h, 000h, 000h
        test   dword ptr [esi + 25h], 0ff0000ffh
        je     SHORT loc_1_2092
loc_1_208B:
        mov    ebx, eax
        call   sub_1_01D0
loc_1_2092:
        pop    ecx
        pop    edx
        pop    eax
        jb     NEAR PTR loc_1_1FC3
        db  089h, 035h, 0A0h, 0B6h, 000h, 000h
        db  089h, 015h, 0A4h, 0B6h, 000h, 000h
        db  089h, 01Dh, 0ACh, 0B6h, 000h, 000h
        mov    esi, dword ptr [esi + 8]
        db  089h, 035h, 0A8h, 0B6h, 000h, 000h
        test   byte ptr [edi + 52h], 3
        je     SHORT loc_1_20DD
        cmp    word ptr [edi + 22h], cx
        je     SHORT loc_1_20E7
        mov    bx, word ptr [ecx + ebx]
loc_1_20C6:
        test   byte ptr [edi + 52h], 2
        jne    SHORT loc_1_20D6
        test   al, 1
        je     SHORT loc_1_20D3
        shr    ebx, 4
loc_1_20D3:
        and    bh, 0fh
loc_1_20D6:
        and    ebx, 0ffffh
        ret
loc_1_20DD:
        mov    ebx, dword ptr [ecx + ebx]
        and    ebx, 0fffffffh
        ret
loc_1_20E7:
        mov    bl, byte ptr [ecx + ebx]
        push   eax
        push   ebx
        lea    ebx, [edx + 1]
        call   sub_1_01D0
        mov    ecx, ebx
        pop    ebx
        pop    eax
        jb     NEAR PTR loc_1_1FC3
        mov    bh, byte ptr [ecx]
        jmp    SHORT loc_1_20C6
sub_1_2102:
        test   byte ptr [edi + 52h], 2
        je     SHORT loc_1_210D
        cmp    bx, -8
        ret
loc_1_210D:
        test   byte ptr [edi + 52h], 3
        jne    SHORT loc_1_211C
        push   ebx
        shl    ebx, 4
        cmp    ebx, -80h
        pop    ebx
        ret
loc_1_211C:
        cmp    bx, 0ff0h
        je     SHORT loc_1_2128
        cmp    bx, 0ff8h
loc_1_2128:
        ret
sub_1_2129:
        mov    esi, ebx
loc_1_212B:
        push   esi
        call   sub_1_1F2C
        pop    esi
        jbe    SHORT loc_1_2163
        call   sub_1_2102
        jae    SHORT loc_1_2164
        xchg   esi, ebx
        push   esi
        call   sub_1_1F2C
        pop    esi
        jbe    SHORT loc_1_2163
        call   sub_1_2102
        jae    SHORT loc_1_2164
        push   esi
        call   sub_1_1F2C
        pop    esi
        jbe    SHORT loc_1_2163
        call   sub_1_2102
        jae    SHORT loc_1_2164
        xchg   esi, ebx
        cmp    ebx, esi
        jne    SHORT loc_1_212B
loc_1_2163:
        stc
loc_1_2164:
        ret
sub_1_2165:
        push   esi
        mov    esi, dword ptr [esi + 8]
loc_1_2169:
        bts    dword ptr [esi + 24h], 4
        jb     SHORT loc_1_21C8
        mov    ecx, eax
        mov    eax, dword ptr [esi + 50h]
        sub    ecx, eax
        jb     SHORT loc_1_21AD
        inc    ecx
        mov    ebx, dword ptr [esi + 10h]
        mov    edx, dword ptr [esi + 5ch]
        or     eax, eax
        je     SHORT loc_1_21D6
        mov    ebx, dword ptr [edx + 4]
        add    ebx, dword ptr [edx]
        dec    ebx
loc_1_218A:
        push   ebx
        push   ecx
        push   esi
        call   sub_1_1F2C
        pop    esi
        pop    ecx
        pop    eax
        jbe    SHORT loc_1_21C5
        call   sub_1_2102
        jae    SHORT loc_1_21C5
        inc    eax
        mov    edx, dword ptr [esi + 5ch]
        cmp    ebx, eax
        jne    SHORT loc_1_21D6
loc_1_21A6:
        inc    dword ptr [esi + 50h]
        inc    dword ptr [edx]
        loop   loc_1_218A
loc_1_21AD:
        clc
loc_1_21AE:
        pushfd
        and    byte ptr [esi + 24h], 0efh
        db  0B8h, 0B0h, 0B6h, 000h, 000h
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_21C2
        call   sub_1_A258
loc_1_21C2:
        popfd
        pop    esi
        ret
loc_1_21C5:
        stc
        jmp    SHORT loc_1_21AE
loc_1_21C8:
        push   eax
        db  0B8h, 0B0h, 0B6h, 000h, 000h
        call   sub_1_A220
        pop    eax
        jmp    SHORT loc_1_2169
loc_1_21D6:
        add    edx, 8
        cmp    dword ptr [esi + 58h], edx
        jbe    SHORT loc_1_21E9
        mov    dword ptr [esi + 5ch], edx
loc_1_21E1:
        mov    dword ptr [edx + 4], ebx
        and    dword ptr [edx], 0
        jmp    SHORT loc_1_21A6
loc_1_21E9:
        push   ebx
        push   ecx
        call   sub_1_21F9
        pop    ecx
        pop    ebx
        jb     SHORT loc_1_21AE
        mov    edx, dword ptr [esi + 5ch]
        jmp    SHORT loc_1_21E1
sub_1_21F9:
        push   eax
        push   esi
        push   edi
        mov    eax, dword ptr [esi + 58h]
        sub    eax, dword ptr [esi + 54h]
        add    eax, 40h
        test   byte ptr [ebp + 37h], 10h
        je     SHORT loc_1_2235
        lea    ebx, [eax + 0fffh]
        shr    ebx, 0ch
        push   8
        push   ebx
        push   dword ptr [esi + 54h]
        db  0CDh, 020h, 054h, 000h, 001h, 000h
        add    esp, 0ch
        cmp    edx, 1
        jb     SHORT loc_1_2272
        shl    ebx, 0ch
        add    ebx, eax
        xchg   dword ptr [esi + 58h], ebx
        mov    dword ptr [esi + 5ch], ebx
        jmp    SHORT loc_1_2271
loc_1_2235:
        push   eax
        test   byte ptr [esi + 24h], 2
        jne    SHORT loc_1_224A
        cmp    eax, 800h
        jb     SHORT loc_1_224A
        call   sub_1_A1E0
        jmp    SHORT loc_1_224F
loc_1_224A:
        call   sub_1_A1CE
loc_1_224F:
        pop    ecx
        jb     SHORT loc_1_2272
        add    ecx, eax
        xchg   dword ptr [esi + 58h], ecx
        sub    ecx, dword ptr [esi + 54h]
        mov    edi, eax
        lea    edx, [ecx + edi]
        mov    dword ptr [esi + 5ch], edx
        xchg   dword ptr [esi + 54h], eax
        shr    ecx, 2
        mov    esi, eax
        rep movsd
        call   sub_1_A1F8
loc_1_2271:
        clc
loc_1_2272:
        pop    edi
        pop    esi
        pop    eax
        ret
sub_1_2276:
        mov    eax, dword ptr [ecx + 54h]
        sub    eax, 8
loc_1_227C:
        add    eax, 8
        sub    ebx, dword ptr [eax]
        jae    SHORT loc_1_227C
        add    ebx, dword ptr [eax]
        add    ebx, dword ptr [eax + 4]
        ret
        db  000h, 000h, 000h
loc_1_228C:
        or     al, byte ptr [ebx]
        adc    byte ptr [edi], cl
        sub    al, 0
        db  000h, 0F4h
        and    al, byte ptr [eax]
        db  000h, 089h, 02Eh, 000h, 000h, 041h
        inc    ebx
        add    byte ptr [eax], al
        nop
        inc    ecx
        add    byte ptr [eax], al
        db  0FEh, 026h, 000h, 000h, 030h, 035h, 000h, 000h, 0B0h, 032h, 000h, 000h, 085h, 0ABh, 000h, 000h
        db  050h, 03Fh, 000h, 000h, 068h, 046h, 000h, 000h, 04Ch, 05Fh, 000h, 000h, 0F2h, 059h, 000h, 000h
        db  008h, 0A8h, 000h, 000h, 0F6h, 051h, 000h, 000h
loc_1_22CC:
        or     al, byte ptr [ebx]
        adc    byte ptr [eax], cl
        xchg   edx, eax
        push   ecx
        add    byte ptr [eax], al
        mov    byte ptr [edi], cl
        db  000h, 011h
        push   ecx
        add    byte ptr [eax], al
        db  070h, 046h
        add    byte ptr [eax], al
        db  0E2h, 046h
        add    byte ptr [eax], al
        db  068h, 046h, 000h, 000h, 068h
        inc    esi
        add    byte ptr [eax], al
        and    al, 0ach
        add    byte ptr [eax], al
loc_1_22F0:
        adc    al, 15h
        sbb    bl, byte ptr [ebx]
loc_1_22F4:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cld
        sub    eax, eax
        mov    byte ptr [ebp + 37h], al
        mov    word ptr [ebp + 38h], 2000h
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        mov    dword ptr [ebp + 6ch], eax
        mov    dword ptr [ebp + 64h], eax
        mov    byte ptr [ebp + 45h], 16h
        mov    byte ptr [ebp + 46h], 0
        cmp    byte ptr [ebp + 4], 2
        jb     SHORT loc_1_2329
        call   sub_1_A318
        jmp    SHORT loc_1_2340
loc_1_2329:
        call   sub_1_A26B
        or     word ptr [ebp + 38h], 800h
        call   sub_1_5B85
        jb     SHORT loc_1_2340
        call   sub_1_5CFF
loc_1_2340:
        mov    al, byte ptr [ebp + 4]
        cmp    al, 2
        je     SHORT loc_1_2384
        cmp    al, 0
        je     SHORT loc_1_23AA
        cmp    al, 1
        je     NEAR PTR loc_1_2549
        cmp    al, 3
        je     SHORT loc_1_2362
        cmp    al, 4
        je     SHORT loc_1_2362
        mov    al, 3
        jmp    NEAR PTR loc_1_4576
loc_1_2362:
        mov    eax, 820h
        mov    edx, dword ptr [ebp]
        call   sub_1_43C9
        mov    eax, dword ptr [ebp + 0ch]
        call   sub_1_91B8
        jae    NEAR PTR loc_1_4548
        mov    al, 3
        jmp    NEAR PTR loc_1_454A
loc_1_2384:
        mov    eax, 0
        mov    edx, dword ptr [ebp]
        call   sub_1_43C9
        mov    eax, dword ptr [ebp + 0ch]
        call   sub_1_91B8
        mov    al, 3
        jb     SHORT loc_1_23A5
        je     NEAR PTR loc_1_4548
        mov    al, 5
loc_1_23A5:
        jmp    NEAR PTR loc_1_454A
loc_1_23AA:
        or     word ptr [ebp + 38h], 8000h
        sub    eax, eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   eax
        mov    eax, 400h
        mov    edx, dword ptr [ebp]
        call   sub_1_4418
        call   sub_1_43C9
        test   byte ptr [ebp + 42h], 20h
        je     SHORT loc_1_23DC
        mov    eax, dword ptr [ebp + 28h]
        cmp    word ptr [eax], 0
        je     NEAR PTR loc_1_253B
loc_1_23DC:
        mov    eax, dword ptr [ebp + 0ch]
        call   sub_1_91B8
        jae    NEAR PTR loc_1_2533
        jne    NEAR PTR loc_1_2537
        mov    eax, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [eax + 14h]
        test   byte ptr [eax + 4], 40h
        jne    NEAR PTR loc_1_253F
        mov    byte ptr [ebp + 44h], 10h
        sub    ecx, ecx
        stc
        call   sub_1_0D5B
        jb     NEAR PTR loc_1_253B
        mov    dword ptr [esp + 8], eax
        mov    dword ptr [esp + 10h], ecx
        mov    eax, dword ptr [ebp + 4ch]
        mov    dword ptr [esp + 0ch], eax
        sub    eax, eax
        xchg   dword ptr [ebp + 50h], eax
        mov    dword ptr [esp + 4], eax
        mov    edi, dword ptr [ebp + 1ch]
        call   sub_1_10C1
        jb     NEAR PTR loc_1_250A
        mov    ecx, dword ptr [ebp + 50h]
        mov    dword ptr [esp], ecx
        call   sub_1_0BD7
        jb     NEAR PTR loc_1_24FF
        mov    edi, ebx
        mov    al, 2eh
        stosb
        mov    al, 20h
        mov    ecx, 0ah
        rep stosb
        mov    al, 10h
        stosb
        sub    eax, eax
        stosb
        push   ebx
        pushfd
        cmp    dword ptr [ebp + 64h], 0
        je     SHORT loc_1_2470
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        pop    esi
loc_1_2470:
        popfd
        clc
        call   sub_1_12EA
        pushfd
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        pop    esi
        popfd
        pop    ebx
        mov    eax, dword ptr [esp]
        mov    word ptr [ebx + 1ah], ax
        shr    eax, 10h
        mov    word ptr [ebx + 14h], ax
        mov    esi, ebx
        lea    edi, [esi + 20h]
        mov    ecx, 8
        rep movsd
        mov    byte ptr [esi + 1], 2eh
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [ebp + 1ch]
        cmp    dword ptr [ecx + 3ch], eax
        jne    SHORT loc_1_24B1
        xor    eax, eax
loc_1_24B1:
        mov    word ptr [esi + 1ah], ax
        shr    eax, 10h
        mov    word ptr [esi + 14h], ax
        mov    esi, dword ptr [ebp + 64h]
        call   sub_1_03EC
        jb     SHORT loc_1_24FF
        mov    eax, dword ptr [esp + 4]
        and    eax, eax
        xchg   dword ptr [ebp + 50h], eax
        jne    SHORT loc_1_24D5
        mov    byte ptr [ebp + 61h], 0ffh
loc_1_24D5:
        mov    dword ptr [ebp + 58h], 0ffffffffh
        mov    eax, dword ptr [esp + 0ch]
        call   sub_1_0BD7
        jb     SHORT loc_1_24FF
        mov    eax, dword ptr [esp]
        mov    word ptr [ebx + 1ah], ax
        shr    eax, 10h
        mov    word ptr [ebx + 14h], ax
        mov    esi, dword ptr [ebp + 64h]
        call   sub_1_03EC
        jae    SHORT loc_1_252F
loc_1_24FF:
        mov    edi, dword ptr [ebp + 1ch]
        mov    ebx, dword ptr [esp]
        call   sub_1_7D50
loc_1_250A:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        jne    SHORT loc_1_2516
        mov    byte ptr [ebp + 61h], 0ffh
loc_1_2516:
        mov    dword ptr [ebp + 58h], 0ffffffffh
        mov    dword ptr [ebp + 50h], eax
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 10h]
        call   sub_1_9A90
        jmp    SHORT loc_1_253B
loc_1_252F:
        sub    eax, eax
        jmp    SHORT loc_1_2541
loc_1_2533:
        mov    al, 5
        jmp    SHORT loc_1_2541
loc_1_2537:
        mov    al, 3
        jmp    SHORT loc_1_2541
loc_1_253B:
        mov    al, 52h
        jmp    SHORT loc_1_2541
loc_1_253F:
        mov    al, 13h
loc_1_2541:
        add    esp, 14h
        jmp    NEAR PTR loc_1_454A
loc_1_2549:
        or     word ptr [ebp + 38h], 8000h
        call   sub_1_1D0E
        mov    eax, 0
        mov    edx, dword ptr [ebp]
        call   sub_1_43C9
        sub    esp, 20h
        mov    eax, dword ptr [ebp + 0ch]
        call   sub_1_91B8
        jae    SHORT loc_1_2575
loc_1_256E:
        mov    al, 3
        jmp    NEAR PTR loc_1_26F6
loc_1_2575:
        jne    NEAR PTR loc_1_26F4
        inc    ebx
        je     NEAR PTR loc_1_26F4
        dec    ebx
        call   sub_1_11ED
        jb     NEAR PTR loc_1_261F
        sub    eax, eax
        call   sub_1_0BD7
        jb     NEAR PTR loc_1_26F4
        cmp    dword ptr [ebx], 2020202eh
        jne    NEAR PTR loc_1_26F4
        inc    dword ptr [ebp + 4ch]
        call   sub_1_0BD4
        jb     NEAR PTR loc_1_26F4
        cmp    dword ptr [ebx], 20202e2eh
        jne    NEAR PTR loc_1_26F4
        lea    edi, [esp]
        test   byte ptr [ebp + 42h], 20h
        je     SHORT loc_1_25D8
        mov    dword ptr [edi], 2a0004h
        mov    word ptr [edi + 4], 0
        jmp    SHORT loc_1_25F5
loc_1_25D8:
        mov    eax, 1ah
        stosw
        mov    eax, 3fh
        mov    ecx, 0ch
        rep stosw
        mov    word ptr [edi], 0
        mov    byte ptr [edi - 8], 2eh
loc_1_25F5:
        mov    eax, 11h
        push   edx
        mov    edx, dword ptr [ebp]
        call   sub_1_43C9
        pop    edx
        lea    eax, [esp]
        mov    ecx, 1
        call   sub_1_9366
        jae    NEAR PTR loc_1_26F4
        or     eax, eax
        jne    NEAR PTR loc_1_26F6
loc_1_261F:
        and    word ptr [ebp + 42h], 0fffeh
        mov    eax, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [eax + 14h]
        test   byte ptr [eax + 4], 40h
        mov    al, 13h
        jne    NEAR PTR loc_1_26F6
        mov    eax, dword ptr [ebp + 0ch]
        call   sub_1_91B8
        jb     NEAR PTR loc_1_256E
        pushfd
        cmp    dword ptr [ebp + 64h], 0
        je     SHORT loc_1_2656
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        pop    esi
loc_1_2656:
        popfd
        mov    eax, dword ptr [ebx + 12h]
        mov    ax, word ptr [ebx + 1ah]
        push   ebx
        mov    ebx, dword ptr [ebp + 1ch]
        test   byte ptr [ebx + 52h], 3
        je     SHORT loc_1_266B
        movzx  eax, ax
loc_1_266B:
        push   eax
        push   eax
        call   sub_1_0979
        pop    eax
        push   0
        push   eax
        push   0
        push   eax
        call   sub_1_0AF3
        pop    eax
        call   sub_1_AB5D
        pop    ebx
        pushfd
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        pop    esi
        popfd
        mov    esi, dword ptr [ebp + 6ch]
        test   byte ptr [esi + 7], 2
        je     SHORT loc_1_26BB
        cmp    byte ptr [esi + 4], 0
        je     SHORT loc_1_26BB
        mov    eax, dword ptr [ebx + 12h]
        mov    ax, word ptr [ebx + 1ah]
        push   eax
        mov    eax, dword ptr [esi + 0ch]
        movzx  ecx, byte ptr [esi + 4]
        inc    ecx
        call   sub_1_9A90
        pop    ebx
        jae    SHORT loc_1_26D1
        jmp    SHORT loc_1_26F4
loc_1_26BB:
        mov    esi, dword ptr [ebp + 64h]
        mov    byte ptr [ebx], 0e5h
        mov    eax, dword ptr [ebx + 12h]
        mov    ax, word ptr [ebx + 1ah]
        push   eax
        call   sub_1_03EC
        pop    ebx
        jb     SHORT loc_1_26F4
loc_1_26D1:
        mov    edi, dword ptr [ebp + 1ch]
        test   byte ptr [edi + 52h], 3
        je     SHORT loc_1_26DD
        movzx  ebx, bx
loc_1_26DD:
        call   sub_1_7D50
        jb     SHORT loc_1_26F4
        mov    eax, dword ptr [ebp + 1ch]
        call   sub_1_05C6
        add    esp, 20h
        jmp    NEAR PTR loc_1_4548
loc_1_26F4:
        mov    al, 5
loc_1_26F6:
        add    esp, 20h
        jmp    NEAR PTR loc_1_454A
loc_1_26FE:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cld
        mov    ax, word ptr [ebp + 18h]
        and    ah, 91h
        mov    byte ptr [ebp + 37h], ah
        sub    eax, eax
        mov    word ptr [ebp + 38h], 2000h
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        mov    dword ptr [ebp + 6ch], eax
        mov    dword ptr [ebp + 64h], eax
        sub    esp, 8
        mov    dword ptr [esp], 0
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [esp + 4], eax
        test   byte ptr [ebp + 18h], 12h
        jne    SHORT loc_1_2743
        call   sub_1_A318
        jmp    SHORT loc_1_275A
loc_1_2743:
        call   sub_1_A26B
        or     word ptr [ebp + 38h], 800h
        call   sub_1_5B85
        jb     SHORT loc_1_275A
        call   sub_1_5CFF
loc_1_275A:
        mov    dword ptr [ebp + 20h], 0
        mov    al, byte ptr [ebp]
        mov    byte ptr [ebp + 44h], al
        mov    byte ptr [ebp + 45h], 16h
        mov    byte ptr [ebp + 46h], 0
        movzx  eax, byte ptr [ebp + 4]
        and    al, 7fh
        cmp    al, 7fh
        jne    SHORT loc_1_277F
        mov    al, 82h
        mov    byte ptr [ebp + 4], 2
loc_1_277F:
        mov    word ptr [ebp + 62h], ax
        mov    al, byte ptr [ebp + 18h]
        mov    byte ptr [ebp + 47h], al
        and    al, al
        je     NEAR PTR loc_1_291A
        mov    bl, al
        and    bl, 0fh
        cmp    bl, 2
        ja     NEAR PTR loc_1_291A
        test   al, 0e0h
        jne    NEAR PTR loc_1_291A
        call   sub_1_9DD4
        jb     NEAR PTR loc_1_2906
        mov    dword ptr [ebp + 20h], eax
        mov    esi, dword ptr [ebp + 10h]
        db  0C7h, 006h, 03Ah, 048h, 000h, 000h
        db  0C7h, 046h, 004h, 0E8h, 049h, 000h, 000h
        db  0C7h, 046h, 008h, 0CCh, 022h, 000h, 000h
        mov    esi, dword ptr [ebp + 20h]
        mov    ax, word ptr [ebp + 62h]
        mov    word ptr [esi + 0ch], ax
        test   byte ptr [ebp + 47h], 1
        je     SHORT loc_1_27F9
        call   sub_1_291E
        mov    dword ptr [esp], eax
        jae    SHORT loc_1_2823
        cmp    al, 2
        jne    NEAR PTR loc_1_2906
        test   byte ptr [ebp + 47h], 10h
        je     NEAR PTR loc_1_2906
loc_1_27F9:
        mov    eax, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [eax + 14h]
        test   byte ptr [eax + 4], 40h
        mov    al, 13h
        jne    NEAR PTR loc_1_2906
        test   byte ptr [ebp + 44h], 8
        je     SHORT loc_1_2815
        mov    byte ptr [ebp + 45h], 8
loc_1_2815:
        call   sub_1_2A47
        mov    dword ptr [esp], eax
        jb     NEAR PTR loc_1_2906
loc_1_2823:
        mov    esi, dword ptr [ebp + 20h]
        test   byte ptr [ebp + 19h], 40h
        je     SHORT loc_1_2832
        or     word ptr [esi + 0eh], 83h
loc_1_2832:
        bt     dword ptr [ebp], 10h
        sbb    eax, eax
        and    eax, 100h
        or     word ptr [esi + 0eh], ax
        mov    esi, dword ptr [esi + 8]
        cmp    dword ptr [esi + 30h], 0
        jns    SHORT loc_1_2852
        test   eax, 100h
        je     SHORT loc_1_28D0
loc_1_2852:
        test   byte ptr [ebp + 37h], 10h
        je     SHORT loc_1_2880
        or     byte ptr [esi + 24h], 80h
        mov    eax, dword ptr [ebp + 20h]
        db  0A3h, 058h, 0B7h, 000h, 000h
        db  0E8h, 0B1h, 0DEh, 0FEh, 0FFh
        mov    ebx, dword ptr [ebp + 1ch]
        or     word ptr [ebx + 52h], 4000h
        mov    eax, dword ptr [ebx + 14h]
        test   byte ptr [eax + 2], 1
        je     SHORT loc_1_2880
        call   sub_1_14EC
loc_1_2880:
        test   byte ptr [ebp + 19h], 4
        jne    SHORT loc_1_28A2
        mov    eax, dword ptr [ebp + 24h]
        or     eax, eax
        je     SHORT loc_1_28A2
        mov    dword ptr [esi + 64h], eax
        mov    ebx, dword ptr [esp + 4]
        push   dword ptr [ebp + 20h]
        push   ebx
        push   eax
        db  0CDh, 020h, 058h, 000h, 040h, 000h
        add    esp, 0ch
loc_1_28A2:
        mov    ebx, dword ptr [esi + 30h]
        mov    dword ptr [ebp + 24h], ebx
        mov    ebx, dword ptr [esi + 38h]
        mov    dword ptr [ebp + 28h], ebx
        movzx  ebx, byte ptr [esi + 43h]
        mov    dword ptr [ebp], ebx
        dec    dword ptr [esi + 20h]
        jns    SHORT loc_1_28C1
        mov    edi, esi
        call   sub_1_A046
loc_1_28C1:
        mov    eax, dword ptr [esp]
        mov    word ptr [ebp + 18h], ax
        add    esp, 8
        jmp    NEAR PTR loc_1_4548
loc_1_28D0:
        mov    edi, esi
        mov    ebx, dword ptr [ebp + 20h]
        movzx  eax, word ptr [ebx + 0ch]
        and    eax, 7
        db  08Bh, 004h, 085h, 0D0h, 0B7h, 000h, 000h
        sub    dword ptr [edi + 2ch], eax
        mov    ecx, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [edx], ecx
        call   sub_1_A040
        cmp    dword ptr [edi + 20h], 0
        jne    SHORT loc_1_2904
        cmp    dword ptr [edi], edi
        jne    SHORT loc_1_2904
        call   sub_1_9FA9
loc_1_2904:
        mov    al, 5
loc_1_2906:
        mov    ecx, dword ptr [ebp + 20h]
        jecxz  loc_1_2912
        xchg   ecx, eax
        call   sub_1_A1F8
        xchg   ecx, eax
loc_1_2912:
        add    esp, 8
        jmp    NEAR PTR loc_1_454A
loc_1_291A:
        mov    al, 1
        jmp    SHORT loc_1_2906
sub_1_291E:
        mov    eax, 0c000h
        mov    edx, dword ptr [ebp]
        call   sub_1_43C9
        mov    bl, byte ptr [ebp + 62h]
        and    bl, 77h
        mov    al, bl
        and    bl, 70h
        cmp    bl, 40h
        ja     NEAR PTR loc_1_2A3F
        xor    bl, al
        cmp    bl, 4
        ja     NEAR PTR loc_1_2A3F
        mov    eax, dword ptr [ebp + 0ch]
        or     eax, eax
        jne    SHORT loc_1_2960
        mov    esi, dword ptr [ebp + 28h]
        call   sub_1_AFEF
        jae    SHORT loc_1_2971
        jmp    NEAR PTR loc_1_2A43
loc_1_2960:
        call   sub_1_91B8
        jb     NEAR PTR loc_1_2A34
        je     NEAR PTR loc_1_2A3B
loc_1_2971:
        mov    al, byte ptr [ebx + 0bh]
        mov    esi, dword ptr [ebp + 20h]
        test   al, 1
        je     SHORT loc_1_298A
        sub    ecx, ecx
        mov    dx, word ptr [ebp + 62h]
        test   dx, 80h
        jne    SHORT loc_1_29A5
        jmp    SHORT loc_1_29B5
loc_1_298A:
        sub    edx, edx
        mov    cx, word ptr [ebp + 62h]
        test   cx, 80h
        jne    SHORT loc_1_29CC
        inc    edx
        and    cl, 3
        je     SHORT loc_1_29CC
        cmp    cl, 3
        je     SHORT loc_1_29CC
        dec    edx
        jmp    SHORT loc_1_29CC
loc_1_29A5:
        mov    cx, 80h
        or     cx, 40h
        mov    byte ptr [esi + 0ch], 40h
        mov    word ptr [ebp + 62h], cx
loc_1_29B5:
        sub    edx, edx
        inc    edx
        mov    cl, byte ptr [esi + 0ch]
        and    cl, 3
        je     SHORT loc_1_29CC
        cmp    cl, 3
        je     SHORT loc_1_29CC
        test   byte ptr [ebp + 19h], 8
        je     SHORT loc_1_2A3B
        dec    edx
loc_1_29CC:
        pushfd
        cmp    dword ptr [ebp + 64h], 0
        je     SHORT loc_1_29DE
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        pop    esi
loc_1_29DE:
        popfd
        push   ebx
        mov    ecx, edx
        call   sub_1_7E79
        pop    ebx
        pushfd
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        pop    esi
        popfd
        jb     SHORT loc_1_2A39
        test   byte ptr [ebp + 19h], 2
        je     SHORT loc_1_2A07
        mov    eax, dword ptr [ebp + 20h]
        mov    eax, dword ptr [eax + 8]
        or     byte ptr [eax + 24h], 4
loc_1_2A07:
        test   byte ptr [ebp + 37h], 11h
        jne    SHORT loc_1_2A2E
        mov    eax, dword ptr [ebp + 1ch]
        test   byte ptr [eax + 52h], 3
        jne    SHORT loc_1_2A1D
        cmp    word ptr [ebx + 14h], 0
        jne    SHORT loc_1_2A24
loc_1_2A1D:
        cmp    word ptr [ebx + 1ah], 0
        je     SHORT loc_1_2A2E
loc_1_2A24:
        sub    eax, eax
        sub    edx, edx
        call   sub_1_0685
        clc
loc_1_2A2E:
        mov    eax, 1
        ret
loc_1_2A34:
        mov    al, 3
        jne    SHORT loc_1_2A39
        dec    eax
loc_1_2A39:
        stc
        ret
loc_1_2A3B:
        mov    al, 5
        stc
        ret
loc_1_2A3F:
        mov    al, 0ch
        stc
        ret
loc_1_2A43:
        mov    al, 2
        stc
        ret
sub_1_2A47:
        sub    esp, 4
        mov    dword ptr [esp], 0
        mov    eax, 8400h
        mov    edx, dword ptr [ebp]
        call   sub_1_4418
        call   sub_1_43C9
        test   byte ptr [ebp + 42h], 20h
        je     SHORT loc_1_2A76
        mov    eax, dword ptr [ebp + 28h]
        cmp    word ptr [eax], 0
        je     NEAR PTR loc_1_2C5F
loc_1_2A76:
        test   byte ptr [ebp + 19h], 4
        je     SHORT loc_1_2A8A
        mov    eax, dword ptr [ebp + 24h]
        mov    dword ptr [ebp + 48h], eax
        or     word ptr [ebp + 42h], 200h
        jmp    SHORT loc_1_2AC8
loc_1_2A8A:
        mov    edi, dword ptr [ebp + 14h]
        or     edi, edi
        je     SHORT loc_1_2AC8
        cmp    dword ptr [edi], 0fh
        jne    SHORT loc_1_2AC8
        or     word ptr [ebp + 42h], 1000h
        mov    edi, dword ptr [edi + 4]
        call   sub_1_9171
        or     edx, edx
        je     SHORT loc_1_2AC8
        push   edx
        mov    edi, dword ptr [ebp + 0ch]
        movzx  edi, word ptr [edi + 2]
        add    edi, dword ptr [ebp + 0ch]
        call   sub_1_9171
        mov    eax, edx
        pop    edx
        cmp    eax, edx
        je     SHORT loc_1_2AC8
        mov    dword ptr [ebp + 48h], edx
        or     word ptr [ebp + 42h], 200h
loc_1_2AC8:
        mov    al, byte ptr [ebp + 44h]
        test   al, 0d0h
        jne    NEAR PTR loc_1_2C5F
        test   al, 8
        je     SHORT loc_1_2AD9
        mov    al, 8
loc_1_2AD9:
        or     al, 20h
        mov    byte ptr [ebp + 44h], al
        mov    esi, dword ptr [ebp + 20h]
        mov    ax, word ptr [ebp + 62h]
        or     word ptr [esi + 0ch], ax
        mov    eax, dword ptr [ebp + 0ch]
        call   sub_1_91B8
        jae    SHORT loc_1_2B1A
        jne    NEAR PTR loc_1_2C61
        test   byte ptr [ebp + 44h], 8
        je     SHORT loc_1_2B09
        test   byte ptr [ebp + 42h], 40h
        jne    NEAR PTR loc_1_2C5F
loc_1_2B09:
        test   byte ptr [ebp + 47h], 0f0h
        mov    al, 2
        jne    NEAR PTR loc_1_2BED
        jmp    NEAR PTR loc_1_2C61
loc_1_2B1A:
        mov    dword ptr [esp], 3
        je     NEAR PTR loc_1_2C5F
        test   byte ptr [ebp + 44h], 8
        jne    NEAR PTR loc_1_2C5F
        test   byte ptr [ebp + 47h], 2
        mov    al, 50h
        je     NEAR PTR loc_1_2C61
        mov    ah, byte ptr [ebx + 0bh]
        test   ah, 1
        jne    NEAR PTR loc_1_2C5F
        mov    al, byte ptr [ebp + 45h]
        pushfd
        cmp    dword ptr [ebp + 64h], 0
        je     SHORT loc_1_2B5E
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        pop    esi
loc_1_2B5E:
        popfd
        push   ebx
        mov    eax, 2
        call   sub_1_7FED
        pop    ebx
        pushfd
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        pop    esi
        popfd
        jb     NEAR PTR loc_1_2C5B
        mov    esi, dword ptr [ebp + 20h]
        mov    dword ptr [esi + 8], eax
        mov    edi, dword ptr [eax]
        mov    dword ptr [esi], edi
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [edi + 4], esi
        mov    dword ptr [eax], esi
        sub    ecx, ecx
        clc
        call   sub_1_0D5B
        jb     SHORT loc_1_2BAF
        xor    ecx, ecx
        call   sub_1_7E79
        mov    eax, dword ptr [ebp + 20h]
        and    word ptr [eax + 0eh], 0fdbbh
        jmp    NEAR PTR loc_1_2C34
loc_1_2BAF:
        mov    esi, dword ptr [ebp + 20h]
        mov    edi, dword ptr [esi + 8]
        dec    dword ptr [edi + 20h]
        jns    SHORT loc_1_2BBF
        call   sub_1_A046
loc_1_2BBF:
        mov    ax, word ptr [esi + 0ch]
        and    eax, 7
        db  08Bh, 004h, 085h, 0D0h, 0B7h, 000h, 000h
        sub    dword ptr [edi + 2ch], eax
        je     SHORT loc_1_2BE1
        mov    ebx, dword ptr [ebp + 20h]
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_2BE9
loc_1_2BE1:
        xor    ebx, ebx
        inc    ebx
        call   sub_1_9F5C
loc_1_2BE9:
        mov    al, 52h
        jmp    SHORT loc_1_2C61
loc_1_2BED:
        mov    dword ptr [esp], 2
        sub    ecx, ecx
        stc
        call   sub_1_0D5B
        mov    al, 52h
        jb     SHORT loc_1_2C61
        push   ebx
        sub    ecx, ecx
        call   sub_1_7E79
        pop    ebx
        jb     SHORT loc_1_2C4C
        test   byte ptr [ebp + 44h], 8
        je     SHORT loc_1_2C34
        mov    edx, dword ptr [ebp + 6ch]
        lea    eax, [edx + 66h]
        lea    edx, [edx + 24h]
        push   eax
        push   1
        push   0bh
        push   16h
        push   edx
        push   eax
        db  0CDh, 020h, 040h, 000h, 040h, 000h
        add    esp, 14h
        pop    eax
        call   sub_1_6E3C
        jb     SHORT loc_1_2C61
loc_1_2C34:
        test   byte ptr [ebp + 19h], 2
        je     SHORT loc_1_2C44
        mov    eax, dword ptr [ebp + 20h]
        mov    eax, dword ptr [eax + 8]
        or     byte ptr [eax + 24h], 4
loc_1_2C44:
        mov    eax, dword ptr [esp]
        add    esp, 4
        clc
        ret
loc_1_2C4C:
        mov    byte ptr [ebx], 0e5h
        mov    esi, dword ptr [ebp + 64h]
        call   sub_1_03EC
        mov    al, 8
        jmp    SHORT loc_1_2C61
loc_1_2C5B:
        mov    al, 20h
        jmp    SHORT loc_1_2C61
loc_1_2C5F:
        mov    al, 5
loc_1_2C61:
        add    esp, 4
        stc
        ret
loc_1_2C66:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cld
        sub    eax, eax
        mov    byte ptr [ebp + 37h], al
        mov    word ptr [ebp + 38h], 0a800h
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        mov    dword ptr [ebp + 6ch], eax
        mov    dword ptr [ebp + 64h], eax
        call   sub_1_A26B
        call   sub_1_1D0E
        call   sub_1_5CFF
        push   0
        push   0
        mov    eax, 8000h
        mov    edx, dword ptr [ebp]
        call   sub_1_43C9
        mov    ax, word ptr [ebp]
        and    al, 3fh
        mov    word ptr [ebp + 45h], ax
        test   al, 50h
        jne    NEAR PTR loc_1_2E79
        mov    eax, dword ptr [ebp + 0ch]
        call   sub_1_91B8
        jb     NEAR PTR loc_1_2E81
        je     NEAR PTR loc_1_2E17
loc_1_2CCC:
        test   byte ptr [ebp + 38h], 4
        jne    SHORT loc_1_2CE6
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        call   sub_1_5B85
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
loc_1_2CE6:
        or     dword ptr [esp + 4], 2
        mov    eax, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [eax + 14h]
        test   byte ptr [eax + 4], 40h
        jne    NEAR PTR loc_1_2E7D
        mov    word ptr [ebp + 62h], 2
        push   ebx
        push   edx
        mov    eax, 2
        call   sub_1_7FED
        pop    edx
        pop    ebx
        jb     NEAR PTR loc_1_2E17
        mov    dword ptr [esp], eax
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        mov    edi, dword ptr [esp]
        call   sub_1_A040
        cmp    dword ptr [edi + 2ch], 10001h
        je     SHORT loc_1_2D58
        push   ebx
        push   edx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        test   word ptr [ebx], 180h
        pop    edx
        pop    ebx
        je     SHORT loc_1_2D58
        sub    dword ptr [edi + 2ch], 10001h
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        jmp    NEAR PTR loc_1_2E17
loc_1_2D58:
        mov    eax, dword ptr [edi]
loc_1_2D5A:
        cmp    eax, edi
        je     SHORT loc_1_2D67
        or     word ptr [eax + 0eh], 10h
        mov    eax, dword ptr [eax]
        jmp    SHORT loc_1_2D5A
loc_1_2D67:
        lea    eax, [edi + 64h]
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_2D7F
        push   edx
        push   0
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 040h, 000h
        add    esp, 0ch
        pop    edx
loc_1_2D7F:
        test   byte ptr [ebp + 45h], 1
        jne    SHORT loc_1_2D8B
        test   byte ptr [ebx + 0bh], 1
        jne    SHORT loc_1_2DF9
loc_1_2D8B:
        mov    edi, dword ptr [ebp + 6ch]
        mov    al, byte ptr [ebx + 0bh]
        and    al, 1fh
        cmp    al, 8
        jne    SHORT loc_1_2D9C
        or     dword ptr [esp + 4], 4
loc_1_2D9C:
        test   byte ptr [edi + 7], 2
        je     SHORT loc_1_2DB9
        cmp    byte ptr [edi + 4], 0
        je     SHORT loc_1_2DB9
        mov    eax, dword ptr [edi + 0ch]
        movzx  ecx, byte ptr [edi + 4]
        inc    ecx
        call   sub_1_9A90
        jae    SHORT loc_1_2DC6
        jmp    SHORT loc_1_2DF9
loc_1_2DB9:
        mov    esi, dword ptr [ebp + 64h]
        mov    byte ptr [ebx], 0e5h
        call   sub_1_03EC
        jb     SHORT loc_1_2DF9
loc_1_2DC6:
        or     dword ptr [esp + 4], 1
        mov    ebx, dword ptr [esp]
        mov    ebx, dword ptr [ebx + 10h]
        mov    edi, dword ptr [ebp + 1ch]
        call   sub_1_7D50
        pushfd
        mov    edi, dword ptr [ebp + 6ch]
        mov    eax, dword ptr [edi]
        xor    edx, edx
        cmp    byte ptr [edi + 4], 0
        je     SHORT loc_1_2DEA
        mov    edx, dword ptr [edi + 0ch]
loc_1_2DEA:
        mov    ebx, dword ptr [ebp + 1ch]
        push   edx
        push   eax
        push   dword ptr [edi + 10h]
        push   eax
        call   sub_1_0AF3
        popfd
loc_1_2DF9:
        pushfd
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        popfd
        mov    edi, dword ptr [esp]
        pushfd
        sub    dword ptr [edi + 2ch], 10001h
        call   sub_1_9FAF
        popfd
        jb     SHORT loc_1_2E79
loc_1_2E17:
        test   byte ptr [ebp + 42h], 1
        je     SHORT loc_1_2E51
        call   sub_1_0BD4
        jb     SHORT loc_1_2E79
        push   edx
        mov    eax, 8010h
        mov    edx, dword ptr [ebp]
        call   sub_1_43C9
        pop    edx
        mov    eax, dword ptr [ebp + 0ch]
        movzx  ecx, word ptr [eax + 2]
        lea    eax, [ecx + eax]
        mov    ecx, 1
        call   sub_1_9366
        jae    NEAR PTR loc_1_2CCC
        or     eax, eax
        jne    SHORT loc_1_2E81
loc_1_2E51:
        test   byte ptr [esp + 4], 4
        je     SHORT loc_1_2E61
        sub    eax, eax
        call   sub_1_6E3C
        jb     SHORT loc_1_2E7D
loc_1_2E61:
        test   byte ptr [esp + 4], 1
        je     SHORT loc_1_2E70
        add    esp, 8
        jmp    NEAR PTR loc_1_4548
loc_1_2E70:
        mov    al, 5
        test   byte ptr [esp + 4], 2
        jne    SHORT loc_1_2E81
loc_1_2E79:
        mov    al, 2
        jmp    SHORT loc_1_2E81
loc_1_2E7D:
        mov    al, 13h
        jmp    SHORT loc_1_2E81
loc_1_2E81:
        add    esp, 8
        jmp    NEAR PTR loc_1_454A
loc_1_2E89:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cld
        sub    eax, eax
        mov    byte ptr [ebp + 37h], al
        mov    word ptr [ebp + 38h], 2000h
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        mov    dword ptr [ebp + 6ch], eax
        mov    dword ptr [ebp + 64h], eax
        call   sub_1_A318
        cmp    byte ptr [ebp + 4], 3
        jae    NEAR PTR loc_1_30CD
        mov    eax, 0c000h
        mov    edx, dword ptr [ebp]
        call   sub_1_43C9
        cmp    byte ptr [ebp + 4], 1
        jne    SHORT loc_1_2EDD
        or     word ptr [ebp + 38h], 800h
        call   sub_1_5B85
        jb     SHORT loc_1_2EDD
        call   sub_1_5CFF
loc_1_2EDD:
        mov    al, byte ptr [ebp]
        mov    byte ptr [ebp + 44h], al
        mov    byte ptr [ebp + 45h], 16h
        mov    byte ptr [ebp + 46h], 0
        mov    eax, dword ptr [ebp + 0ch]
        call   sub_1_91B8
        jb     NEAR PTR loc_1_454A
        cmp    byte ptr [ebp + 4], 1
        jne    SHORT loc_1_2F7B
        cmp    ebx, -1
        je     SHORT loc_1_2F74
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        test   word ptr [ebx], 180h
        pop    ebx
        mov    eax, 0
        je     SHORT loc_1_2F1D
        mov    eax, 1
loc_1_2F1D:
        mov    word ptr [ebp + 62h], 2
        push   eax
        push   ebx
        call   sub_1_7FED
        mov    edi, eax
        pop    ebx
        pop    eax
        jb     SHORT loc_1_2F69
        push   edi
        mov    al, byte ptr [ebp + 44h]
        and    al, 27h
        and    byte ptr [ebx + 0bh], 0d8h
        or     byte ptr [ebx + 0bh], al
        mov    al, byte ptr [ebx + 0bh]
        mov    byte ptr [edi + 43h], al
        mov    esi, dword ptr [ebp + 64h]
        call   sub_1_03EC
        pop    edi
        pushfd
        call   sub_1_A040
        sub    dword ptr [edi + 2ch], 10001h
        jne    SHORT loc_1_2F60
        call   sub_1_9FA9
loc_1_2F60:
        popfd
        jae    NEAR PTR loc_1_4548
        jmp    SHORT loc_1_2F74
loc_1_2F69:
        cmp    eax, 0
        mov    al, 20h
        je     NEAR PTR loc_1_454A
loc_1_2F74:
        mov    al, 5
        jmp    NEAR PTR loc_1_454A
loc_1_2F7B:
        cmp    byte ptr [ebp + 4], 2
        je     SHORT loc_1_2FA4
        cmp    ebx, -1
        je     SHORT loc_1_2F98
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [ebp + 24h], eax
        movzx  eax, byte ptr [ebx + 0bh]
        mov    dword ptr [ebp], eax
        jmp    NEAR PTR loc_1_4548
loc_1_2F98:
        mov    dword ptr [ebp], 10h
        jmp    NEAR PTR loc_1_4548
loc_1_2FA4:
        cmp    ebx, -1
        je     NEAR PTR loc_1_3059
        mov    edx, dword ptr [ebx + 12h]
        mov    ecx, dword ptr [ebp + 1ch]
        test   byte ptr [ecx + 52h], 3
        je     SHORT loc_1_2FBB
        xor    edx, edx
loc_1_2FBB:
        mov    dx, word ptr [ebx + 1ah]
        test   byte ptr [ebx + 0bh], 10h
        jne    SHORT loc_1_2FFD
        push   dword ptr [ebx + 1ch]
        push   edx
        mov    esi, dword ptr [ebp + 64h]
        call   sub_1_7F9B
        pop    edx
        pop    ecx
        jb     SHORT loc_1_2FDB
        mov    ecx, dword ptr [eax + 30h]
        mov    edx, dword ptr [eax + 10h]
loc_1_2FDB:
        mov    edi, dword ptr [ebp + 1ch]
        movzx  eax, word ptr [edi + 4ah]
        add    ecx, eax
        not    eax
        and    ecx, eax
        mov    dword ptr [ebp + 24h], ecx
        or     ecx, ecx
        je     NEAR PTR loc_1_4548
        test   byte ptr [edi + 52h], 10h
        je     NEAR PTR loc_1_4548
loc_1_2FFD:
        mov    edi, dword ptr [ebp + 1ch]
        and    dword ptr [ebp + 24h], 0
        or     edx, edx
        je     NEAR PTR loc_1_4548
        mov    ebx, edx
loc_1_300E:
        xor    esi, esi
loc_1_3010:
        cmp    ebx, 2
        jb     SHORT loc_1_306B
        cmp    dword ptr [edi + 2ch], ebx
        jb     SHORT loc_1_306B
        inc    esi
        push   ebx
        push   esi
        call   sub_1_1F2C
        pop    esi
        pop    eax
        jb     SHORT loc_1_306B
        inc    eax
        call   sub_1_2102
        jae    SHORT loc_1_3032
        cmp    eax, ebx
        je     SHORT loc_1_3010
loc_1_3032:
        test   byte ptr [edi + 52h], 10h
        jne    SHORT loc_1_3041
        mov    cl, byte ptr [edi + 47h]
        shl    esi, cl
        mov    eax, esi
        jmp    SHORT loc_1_304A
loc_1_3041:
        sub    eax, esi
        call   sub_1_3072
        jb     SHORT loc_1_306B
loc_1_304A:
        add    dword ptr [ebp + 24h], eax
        call   sub_1_2102
        jb     SHORT loc_1_300E
        jmp    NEAR PTR loc_1_4548
loc_1_3059:
        mov    edi, dword ptr [ebp + 1ch]
        movzx  eax, word ptr [edi + 44h]
        shl    eax, 5
        mov    dword ptr [ebp + 24h], eax
        jmp    NEAR PTR loc_1_4548
loc_1_306B:
        mov    al, 5
        jmp    NEAR PTR loc_1_454A
sub_1_3072:
        push   ebx
        push   edi
        push   eax
        push   esi
        mov    edx, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [edx + 14h]
        movzx  ebx, word ptr [eax + 0ah]
        mov    esi, dword ptr [eax + 0ch]
        push   eax
        mov    ah, 2
        call   sub_1_A4E1
        mov    esi, ebx
        pop    dword ptr [ebx + 44h]
        mov    al, byte ptr [edx + 0ach]
        mov    byte ptr [esi + 4dh], al
        mov    dword ptr [esi + 8], 500h
        mov    word ptr [esi + 4], 29h
        mov    word ptr [esi + 2eh], 0db09h
        pop    dword ptr [esi + 34h]
        pop    dword ptr [esi + 30h]
        db  0CDh, 020h, 004h, 000h, 010h, 000h
        mov    eax, dword ptr [esi + 3ch]
        movzx  edx, word ptr [esi + 6]
        cmp    edx, 10h
        cmc
        mov    ebx, esi
        call   sub_1_A4AE
        pop    edi
        pop    ebx
        ret
loc_1_30CD:
        cmp    byte ptr [ebp + 4], 3
        jb     NEAR PTR loc_1_329D
        cmp    byte ptr [ebp + 4], 9
        ja     NEAR PTR loc_1_329D
        je     SHORT loc_1_3155
        test   byte ptr [ebp + 4], 1
        je     SHORT loc_1_3155
        mov    eax, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [eax + 14h]
        test   byte ptr [eax + 4], 40h
        jne    NEAR PTR loc_1_32A5
        cmp    byte ptr [ebp + 4], 5
        je     SHORT loc_1_3105
        cmp    byte ptr [ebp + 4], 7
        jne    SHORT loc_1_310F
loc_1_3105:
        test   byte ptr [eax + 53h], 1
        jne    NEAR PTR loc_1_32A9
loc_1_310F:
        mov    ax, word ptr [ebp + 2ah]
        shl    eax, 10h
        cmp    byte ptr [ebp + 4], 5
        je     SHORT loc_1_3120
        mov    ax, word ptr [ebp + 28h]
loc_1_3120:
        or     eax, eax
        je     SHORT loc_1_3143
        sub    edx, edx
        call   sub_1_13D1
        jb     NEAR PTR loc_1_32A1
        cmp    byte ptr [ebp + 4], 7
        jne    SHORT loc_1_3143
        cmp    word ptr [ebp + 18h], 0c8h
        jae    NEAR PTR loc_1_32A1
loc_1_3143:
        or     word ptr [ebp + 38h], 800h
        call   sub_1_5B85
        jb     SHORT loc_1_3155
        call   sub_1_5CFF
loc_1_3155:
        mov    word ptr [ebp + 45h], 16h
        mov    eax, 0c000h
        mov    edx, dword ptr [ebp]
        call   sub_1_43C9
        mov    eax, dword ptr [ebp + 0ch]
        call   sub_1_91B8
        jb     NEAR PTR loc_1_32AB
        cmp    ebx, -1
        jne    SHORT loc_1_318A
        cmp    byte ptr [ebp + 4], 9
        je     NEAR PTR loc_1_3256
        jmp    NEAR PTR loc_1_32A9
loc_1_318A:
        mov    word ptr [ebp + 62h], 2
        push   ebx
        mov    eax, 1
        call   sub_1_7FED
        pop    ebx
        jb     NEAR PTR loc_1_32A9
        mov    edi, eax
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        sub    edx, edx
        movzx  eax, byte ptr [ebp + 4]
        sub    eax, 3
        db  0FFh, 024h, 085h, 0BDh, 031h, 000h, 000h
loc_1_31BD:
        lock xor dword ptr [eax], eax
        db  000h, 0D9h
        xor    dword ptr [eax], eax
        db  000h, 003h
        xor    al, byte ptr [eax]
        db  000h, 0DEh
        xor    dword ptr [eax], eax
        db  000h, 011h
        xor    al, byte ptr [eax]
        db  000h, 0E7h
        xor    dword ptr [eax], eax
        db  000h, 03Fh
        xor    al, byte ptr [eax]
        add    byte ptr [ebx + 50eb3847h], cl
loc_1_31DE:
        movzx  eax, word ptr [edi + 3ch]
        shl    eax, 10h
        jmp    SHORT loc_1_322E
loc_1_31E7:
        movzx  edx, byte ptr [edi + 42h]
        mov    eax, dword ptr [edi + 3eh]
        jmp    SHORT loc_1_322E
loc_1_31F0:
        mov    ax, word ptr [ebp + 2ah]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 28h]
        mov    dword ptr [edi + 38h], eax
        mov    dword ptr [ebx + 16h], eax
        jmp    SHORT loc_1_3277
loc_1_3203:
        mov    ax, word ptr [ebp + 2ah]
        mov    word ptr [edi + 3ch], ax
        mov    word ptr [ebx + 12h], ax
        jmp    SHORT loc_1_3277
loc_1_3211:
        movzx  edx, word ptr [ebp + 18h]
        mov    ax, word ptr [ebp + 2ah]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 28h]
        mov    byte ptr [edi + 42h], dl
        mov    dword ptr [edi + 3eh], eax
        mov    byte ptr [ebx + 0dh], dl
        mov    dword ptr [ebx + 0eh], eax
        jmp    SHORT loc_1_3277
loc_1_322E:
        mov    word ptr [ebp + 28h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 2ah], ax
        mov    word ptr [ebp + 18h], dx
        jmp    SHORT loc_1_327C
loc_1_323F:
        mov    eax, dword ptr [edi + 10h]
        mov    word ptr [ebp + 18h], ax
        test   byte ptr [edi + 24h], 8
        je     SHORT loc_1_327C
        or     word ptr [ebp + 18h], 0ffffh
        mov    dword ptr [ebp + 24h], eax
        jmp    SHORT loc_1_327C
loc_1_3256:
        mov    ebx, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [ebx + 3ch]
        mov    word ptr [ebp + 18h], ax
        test   byte ptr [ebx + 52h], 3
        jne    NEAR PTR loc_1_4548
        or     word ptr [ebp + 18h], 0ffffh
        mov    dword ptr [ebp + 24h], eax
        jmp    NEAR PTR loc_1_4548
loc_1_3277:
        call   sub_1_03EC
loc_1_327C:
        call   sub_1_A040
        sub    dword ptr [edi + 2ch], 10001h
        jne    SHORT loc_1_328F
        call   sub_1_9FA9
loc_1_328F:
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        jmp    NEAR PTR loc_1_4548
loc_1_329D:
        mov    al, 1
        jmp    SHORT loc_1_32AB
loc_1_32A1:
        mov    al, 0dh
        jmp    SHORT loc_1_32AB
loc_1_32A5:
        mov    al, 13h
        jmp    SHORT loc_1_32AB
loc_1_32A9:
        mov    al, 5
loc_1_32AB:
        jmp    NEAR PTR loc_1_454A
loc_1_32B0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cld
        sub    eax, eax
        mov    byte ptr [ebp + 37h], al
        mov    word ptr [ebp + 38h], 2000h
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        mov    dword ptr [ebp + 6ch], eax
        mov    dword ptr [ebp + 64h], eax
        call   sub_1_A318
        cmp    byte ptr [ebp + 4], 0
        jne    SHORT loc_1_32FC
        mov    eax, dword ptr [ebp]
        and    al, 1eh
        test   ah, ah
        je     SHORT loc_1_32E8
        mov    ax, 1016h
loc_1_32E8:
        mov    word ptr [ebp + 45h], ax
        call   sub_1_330C
        jae    NEAR PTR loc_1_4548
        jmp    NEAR PTR loc_1_454A
loc_1_32FC:
        call   sub_1_3435
        jae    NEAR PTR loc_1_4548
        jmp    NEAR PTR loc_1_454A
sub_1_330C:
        mov    eax, 8000h
        mov    edx, dword ptr [ebp]
        call   sub_1_43C9
        mov    eax, dword ptr [ebp + 0ch]
        call   sub_1_91B8
loc_1_3321:
        mov    edi, dword ptr [ebp + 14h]
        jb     NEAR PTR loc_1_3429
        cmp    ebx, -1
        je     NEAR PTR loc_1_342B
        pushfd
        cmp    dword ptr [ebp + 64h], 0
        je     SHORT loc_1_3345
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        pop    esi
loc_1_3345:
        popfd
        mov    esi, dword ptr [ebp + 6ch]
        lea    esi, [esi + 24h]
        lea    eax, [edi + 1]
        push   1
        push   8
        push   10h
        push   esi
        push   eax
        db  0CDh, 020h, 040h, 000h, 040h, 000h
        add    esp, 14h
        mov    esi, dword ptr [ebp + 6ch]
        lea    esi, [esi + 34h]
        lea    eax, [edi + 9]
        push   1
        push   3
        push   6
        push   esi
        push   eax
        db  0CDh, 020h, 040h, 000h, 040h, 000h
        add    esp, 14h
        lea    edi, [edi + 0ch]
        mov    al, byte ptr [ebp + 45h]
        cmp    byte ptr [ebp + 46h], 10h
        jne    SHORT loc_1_3388
        mov    al, 0d7h
loc_1_3388:
        stosb
loc_1_3389:
        mov    eax, dword ptr [ebp + 4ch]
        mov    esi, dword ptr [ebp + 1ch]
        mov    cl, byte ptr [esi + 47h]
        sub    cl, 5
        shr    eax, cl
        mov    eax, dword ptr [ebp + 4ch]
        je     SHORT loc_1_33E1
        cmp    dword ptr [ebp + 50h], 0
        je     SHORT loc_1_33E1
        mov    eax, 1
        shl    eax, cl
        sub    dword ptr [ebp + 4ch], eax
        push   ebx
        push   edi
        mov    edi, esi
        mov    ebx, dword ptr [ebp + 50h]
        call   sub_1_1F2C
        pop    esi
        pop    eax
        jb     SHORT loc_1_33CB
        call   sub_1_2102
        jae    SHORT loc_1_33CB
        mov    dword ptr [ebp + 50h], ebx
        mov    edi, esi
        xchg   ebx, eax
        jmp    SHORT loc_1_3389
loc_1_33CB:
        mov    word ptr [esi], 0ffffh
        pushfd
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        pop    esi
        popfd
        mov    al, 12h
        stc
        ret
loc_1_33E1:
        stosw
        mov    eax, dword ptr [ebp + 50h]
        stosd
        lea    esi, [ebx + 16h]
        mov    al, byte ptr [ebx + 0bh]
        mov    edi, dword ptr [ebp + 14h]
        lea    edi, [edi + 15h]
        stosb
        movsd
        mov    eax, dword ptr [ebp + 1ch]
        test   byte ptr [eax + 52h], 3
        mov    eax, dword ptr [esi - 8]
        je     SHORT loc_1_3403
        xor    eax, eax
loc_1_3403:
        lodsw
        mov    dword ptr [ebp + 28h], eax
        movsd
        mov    esi, ebx
        push   edi
        call   sub_1_9ED8
        pop    edi
        cmp    byte ptr [edi], 5
        jne    SHORT loc_1_341A
        mov    byte ptr [edi], 0e5h
loc_1_341A:
        pushfd
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        pop    esi
        popfd
        clc
        ret
loc_1_3429:
        jne    SHORT loc_1_342D
loc_1_342B:
        mov    al, 12h
loc_1_342D:
        mov    word ptr [edi + 0dh], 0ffffh
        stc
        ret
sub_1_3435:
        sub    esp, 34h
        mov    esi, dword ptr [ebp + 14h]
        cmp    word ptr [esi + 0dh], -1
        je     NEAR PTR loc_1_3529
        inc    esi
        mov    eax, 200020h
        lea    edi, [esp + 1ch]
        mov    ecx, 0bh
        rep stosw
        lea    eax, [esp + 1ch]
        push   1
        push   8
        push   esi
        push   eax
        db  0CDh, 020h, 042h, 000h, 040h, 000h
        add    esp, 10h
        lea    esi, [esi + 8]
        lea    eax, [esp + 2ch]
        push   1
        push   3
        push   esi
        push   eax
        db  0CDh, 020h, 042h, 000h, 040h, 000h
        add    esp, 10h
        lea    eax, [esp + 1ch]
        lea    edx, [esp + 2]
        push   0
        push   eax
        push   edx
        db  0CDh, 020h, 048h, 000h, 040h, 000h
        add    esp, 0ch
        mov    word ptr [esp + eax + 2], 0
        add    eax, 2
        mov    word ptr [esp], ax
        lea    edi, [esp + 1ch]
        mov    ecx, 0bh
        mov    ax, 3fh
        repne scasw
        mov    eax, 18h
        jne    SHORT loc_1_3520
        or     eax, 1
        mov    word ptr [ebp + 42h], ax
        mov    esi, dword ptr [ebp + 14h]
        lea    esi, [esi + 0ch]
        lodsb
        sub    ah, ah
        cmp    al, 0d7h
        jne    SHORT loc_1_34D3
        mov    ax, 1016h
loc_1_34D3:
        mov    word ptr [ebp + 45h], ax
        lea    edi, [ebp + 4ch]
        sub    eax, eax
        lodsw
        stosd
        lodsd
        stosd
        stosd
        xor    eax, eax
        stosd
        mov    edi, dword ptr [ebp + 1ch]
        cmp    dword ptr [ebp + 50h], 0
        je     SHORT loc_1_34F4
        mov    al, byte ptr [edi + 24h]
        inc    eax
        jmp    SHORT loc_1_34FE
loc_1_34F4:
        mov    byte ptr [ebp + 60h], 0
        mov    eax, dword ptr [edi + 28h]
        sub    eax, dword ptr [edi + 34h]
loc_1_34FE:
        mov    byte ptr [ebp + 61h], al
        call   sub_1_0BD4
        jb     SHORT loc_1_3520
        lea    eax, [esp]
        mov    ecx, 1
        call   sub_1_9366
        jb     SHORT loc_1_3520
        add    esp, 34h
        clc
        jmp    NEAR PTR loc_1_3321
loc_1_3520:
        mov    esi, dword ptr [ebp + 14h]
        mov    word ptr [esi + 0dh], 0ffffh
loc_1_3529:
        add    esp, 34h
        mov    al, 12h
        stc
        ret
loc_1_3530:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cld
        sub    eax, eax
        mov    byte ptr [ebp + 37h], al
        mov    word ptr [ebp + 38h], 0a800h
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        mov    dword ptr [ebp + 6ch], eax
        mov    dword ptr [ebp + 64h], eax
        call   sub_1_A26B
        call   sub_1_1D0E
        call   sub_1_5B85
        jb     SHORT loc_1_3566
        call   sub_1_5CFF
loc_1_3566:
        and    word ptr [ebp + 38h], 2804h
        mov    eax, dword ptr [ebp]
        or     eax, dword ptr [ebp + 24h]
        test   eax, 80000000h
        je     SHORT loc_1_357E
        jmp    NEAR PTR loc_1_3A2C
loc_1_357E:
        mov    eax, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [eax + 14h]
        test   byte ptr [eax + 4], 40h
        mov    al, 13h
        jne    NEAR PTR loc_1_4576
        mov    eax, 0c6h
        call   sub_1_A1CE
        mov    dword ptr [ebp + 68h], eax
        mov    edi, eax
        mov    al, 8
        jb     NEAR PTR loc_1_4576
        and    dword ptr [edi], 0
        mov    byte ptr [edi + 0bah], 0
        mov    eax, dword ptr [ebp + 1ch]
        test   byte ptr [eax + 52h], 3
        jne    SHORT loc_1_35C1
        or     byte ptr [edi + 0bah], 1
loc_1_35C1:
        mov    al, byte ptr [ebp]
        mov    byte ptr [ebp + 45h], al
        mov    byte ptr [ebp + 46h], 0
        mov    eax, 0
        mov    edx, dword ptr [ebp]
        call   sub_1_43C9
        test   eax, 20h
        je     SHORT loc_1_35E5
        or     word ptr [ebp + 42h], 2000h
loc_1_35E5:
        mov    eax, dword ptr [ebp + 0ch]
        call   sub_1_91B8
        jb     NEAR PTR loc_1_39D5
        cmp    ebx, -1
        je     NEAR PTR loc_1_3A28
        mov    esi, dword ptr [ebp + 6ch]
        mov    edi, dword ptr [ebp + 68h]
        test   byte ptr [ebx + 0bh], 10h
        jne    SHORT loc_1_3642
        mov    ax, 12h
        mov    word ptr [ebp + 62h], ax
        push   ebx
        push   esi
        push   edi
        bt     dword ptr [ebp + 18h], 4
        sbb    eax, eax
        and    eax, 1
        call   sub_1_7FED
        pop    edi
        pop    esi
        pop    ebx
        jb     NEAR PTR loc_1_3A28
        mov    dword ptr [edi], eax
        cmp    dword ptr [eax], eax
        je     SHORT loc_1_3642
        test   byte ptr [eax + 24h], 80h
        jne    NEAR PTR loc_1_3A28
        or     byte ptr [edi + 0bah], 4
loc_1_3642:
        mov    eax, dword ptr [ebp + 50h]
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [edi + 0ch], eax
        mov    al, byte ptr [esi + 4]
        mov    byte ptr [edi + 10h], al
        mov    al, byte ptr [esi + 7]
        mov    byte ptr [edi + 11h], al
        mov    esi, ebx
        lea    edi, [edi + 12h]
        mov    ecx, 8
        rep movsd
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 68h]
        mov    esi, dword ptr [ebp + 6ch]
        movzx  ecx, word ptr [esi + 2b2h]
        mov    word ptr [edi + 32h], cx
        jecxz  loc_1_3693
        cmp    ecx, 10h
        jae    SHORT loc_1_3693
        lea    esi, [esi + 2b6h]
        lea    edi, [edi + 36h]
        add    ecx, ecx
        rep movsd
loc_1_3693:
        pop    edi
        pop    esi
        sub    eax, eax
        xchg   dword ptr [ebp + 6ch], eax
        call   sub_1_A1F8
        xor    eax, eax
        mov    edx, dword ptr [ebp + 24h]
        call   sub_1_4418
        call   sub_1_43C9
        mov    edi, dword ptr [ebp + 14h]
        or     edi, edi
        je     SHORT loc_1_36EC
        cmp    dword ptr [edi], 0fh
        jne    SHORT loc_1_36EC
        or     word ptr [ebp + 42h], 1000h
        mov    edi, dword ptr [edi + 4]
        call   sub_1_9171
        or     edx, edx
        je     SHORT loc_1_36EC
        push   edx
        mov    edi, dword ptr [ebp + 0ch]
        movzx  edi, word ptr [edi + 2]
        add    edi, dword ptr [ebp + 10h]
        call   sub_1_9171
        mov    eax, edx
        pop    edx
        cmp    eax, edx
        je     SHORT loc_1_36EC
        mov    dword ptr [ebp + 48h], edx
        or     word ptr [ebp + 42h], 200h
loc_1_36EC:
        test   byte ptr [ebp + 42h], 20h
        je     SHORT loc_1_36F8
        or     word ptr [ebp + 42h], 2400h
loc_1_36F8:
        mov    byte ptr [ebp + 45h], 16h
        mov    eax, dword ptr [ebp + 10h]
        call   sub_1_91B8
        jae    SHORT loc_1_3711
        je     NEAR PTR loc_1_3820
        jmp    NEAR PTR loc_1_39D5
loc_1_3711:
        cmp    ebx, -1
        je     NEAR PTR loc_1_3A28
        mov    esi, dword ptr [ebp + 68h]
        mov    eax, dword ptr [ebp + 50h]
        cmp    dword ptr [esi + 4], eax
        jne    NEAR PTR loc_1_3A28
        mov    eax, dword ptr [ebp + 4ch]
        cmp    eax, dword ptr [esi + 8]
        jne    NEAR PTR loc_1_3A28
        test   byte ptr [ebp + 42h], 20h
        je     NEAR PTR loc_1_3A28
        mov    esi, dword ptr [ebp + 28h]
        cmp    word ptr [esi], 0
        je     NEAR PTR loc_1_3A1A
        mov    edi, dword ptr [ebp + 6ch]
        test   byte ptr [edi + 7], 2
        je     NEAR PTR loc_1_37E2
        sub    eax, eax
        lea    edi, [edi + 0b2h]
        mov    ecx, 100h
        repne scasw
        jne    NEAR PTR loc_1_3A28
        lea    eax, [edi - 2]
        mov    edi, dword ptr [ebp + 6ch]
        movzx  ecx, word ptr [edi + 0b0h]
        lea    edi, [edi + 0b2h]
        sub    eax, edi
        cmp    ecx, eax
        jne    SHORT loc_1_37FE
        shr    ecx, 1
        repe cmpsw
        je     NEAR PTR loc_1_3A1A
        lea    esi, [ebx]
        mov    edi, dword ptr [ebp + 68h]
        lea    edi, [edi + 0bbh]
        movsd
        movsd
        movsw
        movsb
        mov    esi, dword ptr [ebp + 68h]
        mov    edi, dword ptr [ebp + 6ch]
        lea    esi, [esi + 4]
        sub    eax, eax
        mov    edx, dword ptr [ebp + 28h]
        movzx  ecx, word ptr [edi + 0b0h]
        pushfd
        cmp    dword ptr [ebp + 64h], 0
        je     SHORT loc_1_37CA
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        pop    esi
loc_1_37CA:
        popfd
        call   sub_1_3DC3
        pushfd
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        pop    esi
        popfd
        jmp    NEAR PTR loc_1_397B
loc_1_37E2:
        movzx  ecx, word ptr [edi + 94h]
        lea    edi, [edi + 96h]
        sub    ecx, 2
        shr    ecx, 1
        repe cmpsw
        jne    SHORT loc_1_37FE
        jmp    NEAR PTR loc_1_3A1A
loc_1_37FE:
        call   sub_1_0CDE
        jb     NEAR PTR loc_1_39D5
        je     NEAR PTR loc_1_38AB
        mov    esi, dword ptr [ebp + 6ch]
        mov    ah, byte ptr [ebx]
        call   sub_1_9A38
        jae    SHORT loc_1_37FE
        jmp    NEAR PTR loc_1_38AB
loc_1_3820:
        mov    esi, dword ptr [ebp + 68h]
        mov    eax, dword ptr [ebp + 50h]
        cmp    dword ptr [esi + 4], eax
        jne    SHORT loc_1_3845
        mov    edi, dword ptr [ebp + 6ch]
        mov    eax, dword ptr [edi + 20h]
        cmp    eax, dword ptr [esi + 8]
        jne    SHORT loc_1_38AB
        inc    eax
        je     SHORT loc_1_38AB
        test   byte ptr [edi + 8], 1
        jne    SHORT loc_1_38AB
        and    byte ptr [edi + 8], 0f7h
        jmp    SHORT loc_1_38AB
loc_1_3845:
        test   byte ptr [esi + 1dh], 10h
        je     SHORT loc_1_38AB
        test   byte ptr [ebp + 42h], 20h
        je     NEAR PTR loc_1_3A28
        test   byte ptr [esi + 0bah], 1
        je     SHORT loc_1_3865
        cmp    word ptr [esi + 26h], 0
        jne    SHORT loc_1_3870
loc_1_3865:
        cmp    word ptr [esi + 2ch], 0
        je     NEAR PTR loc_1_3A28
loc_1_3870:
        or     byte ptr [esi + 0bah], 2
        mov    edi, dword ptr [ebp + 6ch]
        movzx  ecx, word ptr [esi + 32h]
        cmp    word ptr [edi + 2b2h], cx
        jb     SHORT loc_1_38AB
        cmp    ecx, 10h
        jae    NEAR PTR loc_1_3A28
        or     ecx, ecx
        je     NEAR PTR loc_1_3A28
        lea    esi, [esi + 36h]
        lea    edi, [edi + 2b6h]
        add    ecx, ecx
        repe cmpsd
        je     NEAR PTR loc_1_3A28
loc_1_38AB:
        test   byte ptr [ebp + 42h], 20h
        je     SHORT loc_1_38BE
        mov    esi, dword ptr [ebp + 28h]
        cmp    word ptr [esi], 0
        je     NEAR PTR loc_1_3A28
loc_1_38BE:
        mov    esi, dword ptr [ebp + 68h]
        lea    ecx, [esi + 12h]
        stc
        call   sub_1_0D5B
        jb     NEAR PTR loc_1_3A28
        mov    eax, dword ptr [ebp + 68h]
        test   byte ptr [eax + 0bah], 4
        je     SHORT loc_1_38E3
        mov    edi, dword ptr [eax]
        call   sub_1_444E
loc_1_38E3:
        mov    edi, dword ptr [ebp + 68h]
        mov    ecx, dword ptr [ebp + 50h]
        mov    dword ptr [edi + 0b6h], ecx
        lea    esi, [ebx]
        lea    edi, [edi + 0bbh]
        movsd
        movsd
        movsw
        movsb
        mov    esi, dword ptr [ebp + 68h]
        mov    ebx, dword ptr [esi + 4]
        call   sub_1_1201
        mov    esi, dword ptr [ebp + 68h]
        mov    eax, dword ptr [esi + 8]
        movzx  ecx, byte ptr [esi + 10h]
        jecxz  loc_1_3916
        mov    eax, dword ptr [esi + 0ch]
loc_1_3916:
        inc    ecx
        call   sub_1_9A90
        mov    esi, dword ptr [ebp + 68h]
        test   byte ptr [esi + 0bah], 2
        je     SHORT loc_1_397B
        xor    ebx, ebx
        test   byte ptr [esi + 0bah], 1
        je     SHORT loc_1_3936
        mov    ebx, dword ptr [esi + 24h]
loc_1_3936:
        mov    bx, word ptr [esi + 2ch]
        call   sub_1_1201
        mov    eax, 1
        call   sub_1_0BD7
        jb     NEAR PTR loc_1_3A28
        mov    esi, dword ptr [ebp + 68h]
        mov    eax, dword ptr [esi + 0b6h]
        mov    edi, dword ptr [ebp + 1ch]
        test   byte ptr [edi + 52h], 3
        jne    SHORT loc_1_3968
        cmp    dword ptr [edi + 3ch], eax
        jne    SHORT loc_1_3968
        xor    eax, eax
loc_1_3968:
        mov    word ptr [ebx + 1ah], ax
        shr    eax, 10h
        mov    word ptr [ebx + 14h], ax
        mov    esi, dword ptr [ebp + 64h]
        call   sub_1_03EC
loc_1_397B:
        mov    esi, dword ptr [ebp + 68h]
        test   byte ptr [esi + 1dh], 10h
        je     SHORT loc_1_399B
        sub    eax, eax
        mov    ebx, dword ptr [ebp + 1ch]
        call   sub_1_0979
        sub    eax, eax
        push   eax
        push   eax
        push   eax
        push   eax
        call   sub_1_0AF3
        jmp    SHORT loc_1_39D3
loc_1_399B:
        test   byte ptr [esi + 1dh], 8
        jne    SHORT loc_1_39BF
        mov    eax, dword ptr [esi + 4]
        xor    edx, edx
        cmp    byte ptr [esi + 10h], 0
        je     SHORT loc_1_39AF
        mov    edx, dword ptr [esi + 0ch]
loc_1_39AF:
        mov    ebx, dword ptr [ebp + 1ch]
        push   edx
        push   eax
        push   dword ptr [esi + 8]
        push   eax
        call   sub_1_0AF3
        jmp    SHORT loc_1_39D3
loc_1_39BF:
        mov    esi, dword ptr [ebp + 68h]
        test   byte ptr [esi + 1dh], 8
        je     SHORT loc_1_39D3
        lea    eax, [esi + 0bbh]
        call   sub_1_6E3C
loc_1_39D3:
        sub    eax, eax
loc_1_39D5:
        push   eax
        mov    edi, dword ptr [ebp + 68h]
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     SHORT loc_1_3A0C
        test   al, al
        jne    SHORT loc_1_39F4
        lea    edi, [edi + 45h]
        mov    esi, dword ptr [ebp + 68h]
        lea    esi, [esi + 0bbh]
        movsd
        movsd
        movsw
        movsb
loc_1_39F4:
        mov    edi, dword ptr [ebp + 68h]
        mov    edi, dword ptr [edi]
        call   sub_1_A040
        sub    dword ptr [edi + 2ch], 10001h
        jne    SHORT loc_1_3A0C
        call   sub_1_9FA9
loc_1_3A0C:
        mov    eax, dword ptr [ebp + 68h]
        call   sub_1_A1F8
        pop    eax
        jmp    NEAR PTR loc_1_454A
loc_1_3A1A:
        sub    eax, eax
        push   eax
        mov    edi, dword ptr [ebp + 68h]
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    SHORT loc_1_39F4
        jmp    SHORT loc_1_3A0C
loc_1_3A28:
        mov    al, 5
        jmp    SHORT loc_1_39D5
loc_1_3A2C:
        mov    eax, 98h
        call   sub_1_A1CE
        mov    dword ptr [ebp + 68h], eax
        mov    edi, eax
        mov    al, 8
        jb     NEAR PTR loc_1_4576
        mov    dword ptr [edi], 0
        mov    al, byte ptr [ebp]
        mov    byte ptr [ebp + 45h], al
        mov    byte ptr [ebp + 46h], 0
        mov    eax, 0
        mov    edx, dword ptr [ebp]
        call   sub_1_43C9
        mov    word ptr [edi + 4], ax
        mov    eax, dword ptr [ebp + 0ch]
        call   sub_1_91B8
        jb     NEAR PTR loc_1_3D95
        cmp    ebx, -1
        je     NEAR PTR loc_1_3D8F
        mov    edi, dword ptr [ebp + 68h]
        mov    esi, dword ptr [ebp + 6ch]
        mov    byte ptr [edi + 8dh], 0
        mov    al, byte ptr [ebx + 0bh]
        mov    byte ptr [edi + 81h], al
        mov    eax, dword ptr [ebp + 50h]
        mov    dword ptr [edi + 74h], eax
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [edi + 78h], eax
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [edi + 7ch], eax
        mov    al, byte ptr [esi + 4]
        mov    byte ptr [edi + 80h], al
        mov    esi, dword ptr [ebp + 3eh]
        movzx  ecx, word ptr [esi]
        shr    ecx, 1
        inc    ecx
        lea    edi, [edi + 8]
        rep movsw
        mov    esi, dword ptr [ebp + 6ch]
        lea    esi, [esi + 66h]
        mov    edi, dword ptr [ebp + 68h]
        lea    edi, [edi + 40h]
        mov    ecx, 0bh
        rep movsw
        mov    eax, 0
        mov    edx, dword ptr [ebp + 24h]
        call   sub_1_43C9
        mov    edi, dword ptr [ebp + 68h]
        mov    word ptr [edi + 6], ax
        mov    eax, dword ptr [ebp + 10h]
        call   sub_1_91B8
        jae    SHORT loc_1_3AF3
        jne    NEAR PTR loc_1_3D95
loc_1_3AF3:
        mov    eax, dword ptr [ebp + 50h]
        mov    edi, dword ptr [ebp + 68h]
        cmp    eax, dword ptr [edi + 74h]
        jne    NEAR PTR loc_1_3D8F
        mov    esi, dword ptr [ebp + 6ch]
        lea    esi, [esi + 24h]
        lea    edi, [edi + 56h]
        mov    ecx, 0bh
        rep movsw
loc_1_3B13:
        mov    edx, dword ptr [ebp + 68h]
        lea    esi, [edx + 56h]
        lea    edi, [edx + 40h]
        mov    ecx, 0bh
loc_1_3B21:
        lodsw
        cmp    ax, 3fh
        jne    SHORT loc_1_3B2C
        mov    ax, word ptr [edi]
loc_1_3B2C:
        stosw
        loop   loc_1_3B21
        lea    esi, [edx + 40h]
        lea    edi, [edx + 26h]
        push   edx
        push   0
        push   esi
        push   edi
        db  0CDh, 020h, 048h, 000h, 040h, 000h
        add    esp, 0ch
        pop    edx
        mov    word ptr [eax + edi], 0
        add    eax, 2
        mov    word ptr [edx + 24h], ax
        mov    ebx, dword ptr [edx + 74h]
        push   edx
        call   sub_1_1201
        pop    edx
        mov    byte ptr [ebp + 45h], 16h
        mov    ax, word ptr [edx + 6]
        or     ax, 10h
        mov    word ptr [ebp + 42h], ax
        lea    eax, [edx + 24h]
        mov    ecx, 0
        push   edx
        call   sub_1_9366
        pop    edx
        jae    NEAR PTR loc_1_3D8F
        or     eax, eax
        jne    NEAR PTR loc_1_3D95
        mov    eax, dword ptr [edx + 78h]
        call   sub_1_0BD7
        jb     NEAR PTR loc_1_3D8F
        mov    edi, dword ptr [ebp + 68h]
        mov    dword ptr [edi + 6ch], ebx
        mov    dword ptr [edi + 70h], edx
        mov    al, byte ptr [ebx + 0bh]
        mov    byte ptr [edi + 81h], al
        lea    esi, [edi + 40h]
        lea    edi, [edi + 82h]
        pushfd
        cmp    dword ptr [ebp + 64h], 0
        je     SHORT loc_1_3BC4
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        pop    esi
loc_1_3BC4:
        popfd
        push   1
        push   0bh
        push   16h
        push   esi
        push   edi
        db  0CDh, 020h, 040h, 000h, 040h, 000h
        add    esp, 14h
        pushfd
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        pop    esi
        popfd
        cmp    byte ptr [edi], 0e5h
        jne    SHORT loc_1_3BEB
        mov    byte ptr [edi], 5
loc_1_3BEB:
        mov    edx, dword ptr [ebp + 68h]
        test   byte ptr [edx + 81h], 8
        je     SHORT loc_1_3C0A
        lea    esi, [edx + 82h]
        lea    edi, [edx + 8dh]
        mov    ecx, 0bh
        rep movsb
loc_1_3C0A:
        test   byte ptr [edx + 81h], 10h
        jne    SHORT loc_1_3C4B
        mov    word ptr [ebp + 62h], 12h
        sub    eax, eax
        pushfd
        cmp    dword ptr [ebp + 64h], 0
        je     SHORT loc_1_3C2D
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        pop    esi
loc_1_3C2D:
        popfd
        call   sub_1_7FED
        pushfd
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        pop    esi
        popfd
        jb     NEAR PTR loc_1_3D8F
        mov    edx, dword ptr [ebp + 68h]
        mov    dword ptr [edx], eax
loc_1_3C4B:
        lea    esi, [edx + 74h]
        lea    eax, [edx + 40h]
        sub    edx, edx
        sub    ecx, ecx
        pushfd
        cmp    dword ptr [ebp + 64h], 0
        je     SHORT loc_1_3C67
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        pop    esi
loc_1_3C67:
        popfd
        call   sub_1_3DC3
        pushfd
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        pop    esi
        popfd
        jb     NEAR PTR loc_1_3D95
        sub    eax, eax
        mov    ebx, dword ptr [ebp + 1ch]
        push   eax
        push   eax
        push   eax
        push   eax
        call   sub_1_0AF3
        mov    edx, dword ptr [ebp + 68h]
        test   byte ptr [edx + 81h], 10h
        je     SHORT loc_1_3CA6
        sub    eax, eax
        mov    ebx, dword ptr [ebp + 1ch]
        call   sub_1_0979
        jmp    SHORT loc_1_3D10
loc_1_3CA6:
        mov    edx, dword ptr [ebp + 68h]
        mov    edi, dword ptr [edx]
        lea    esi, [edx + 82h]
        lea    edi, [edi + 45h]
        movsd
        movsd
        movsw
        movsb
        sub    edi, edi
        xchg   dword ptr [edx], edi
        pushfd
        cmp    dword ptr [ebp + 64h], 0
        je     SHORT loc_1_3CCF
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        pop    esi
loc_1_3CCF:
        popfd
        call   sub_1_A040
        pushfd
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        pop    esi
        popfd
        sub    dword ptr [edi + 2ch], 10001h
        jne    SHORT loc_1_3D10
        pushfd
        cmp    dword ptr [ebp + 64h], 0
        je     SHORT loc_1_3CFD
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 007h, 000h, 08Bh, 004h
        pop    esi
loc_1_3CFD:
        popfd
        call   sub_1_9FA9
        pushfd
        push   esi
        mov    esi, dword ptr [ebp + 64h]
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        pop    esi
        popfd
loc_1_3D10:
        mov    edx, dword ptr [ebp + 68h]
        test   byte ptr [edx + 4], 1
        je     SHORT loc_1_3D74
        mov    al, byte ptr [ebp]
        mov    byte ptr [ebp + 45h], al
        mov    ax, word ptr [edx + 4]
        or     ax, 10h
        mov    word ptr [ebp + 42h], ax
        lea    eax, [edx + 8]
        mov    ebx, dword ptr [edx + 6ch]
        mov    edx, dword ptr [edx + 70h]
        mov    ecx, 1
        call   sub_1_9366
        jae    SHORT loc_1_3D46
        or     eax, eax
        je     SHORT loc_1_3D74
        jmp    SHORT loc_1_3D95
loc_1_3D46:
        mov    esi, dword ptr [ebp + 6ch]
        mov    edx, dword ptr [ebp + 68h]
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [edx + 78h], eax
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [edx + 7ch], eax
        mov    al, byte ptr [esi + 4]
        mov    byte ptr [edx + 80h], al
        lea    esi, [esi + 66h]
        lea    edi, [edx + 40h]
        mov    ecx, 0bh
        rep movsw
        jmp    NEAR PTR loc_1_3B13
loc_1_3D74:
        mov    edx, dword ptr [ebp + 68h]
        cmp    byte ptr [edx + 8dh], 0
        je     SHORT loc_1_3D93
        lea    eax, [edx + 8dh]
        call   sub_1_6E3C
        jae    SHORT loc_1_3D93
        jmp    SHORT loc_1_3D95
loc_1_3D8F:
        mov    al, 5
        jmp    SHORT loc_1_3D95
loc_1_3D93:
        sub    eax, eax
loc_1_3D95:
        push   eax
        mov    edx, dword ptr [ebp + 68h]
        mov    ecx, dword ptr [edx]
        jecxz  loc_1_3DB2
        mov    edi, ecx
        call   sub_1_A040
        sub    dword ptr [edi + 2ch], 10001h
        jne    SHORT loc_1_3DB2
        call   sub_1_9FA9
loc_1_3DB2:
        xchg   dword ptr [ebp + 68h], eax
        call   sub_1_A1F8
        xchg   dword ptr [ebp + 68h], eax
        pop    eax
        jmp    NEAR PTR loc_1_454A
sub_1_3DC3:
        sub    esp, 2ch
        mov    dword ptr [esp + 1ch], eax
        mov    dword ptr [esp + 20h], edx
        mov    word ptr [esp + 1ah], cx
        lea    edi, [esp]
        movsd
        movsd
        movsd
        movsb
        mov    ebx, dword ptr [esp]
        call   sub_1_1201
        jb     NEAR PTR loc_1_3F49
        mov    ecx, dword ptr [esp + 1ch]
        jecxz  loc_1_3E1A
        lea    edx, [esp + 0eh]
        push   1
        push   0bh
        push   16h
        push   ecx
        push   edx
        db  0CDh, 020h, 040h, 000h, 040h, 000h
        add    esp, 14h
        lea    esi, [esp + 0eh]
        cmp    byte ptr [esi], 0e5h
        jne    SHORT loc_1_3E0F
        mov    byte ptr [esi], 5
loc_1_3E0F:
        sub    al, al
        call   sub_1_8D88
        mov    byte ptr [esp + 0dh], al
loc_1_3E1A:
        mov    eax, dword ptr [esp + 8]
        cmp    eax, -1
        jne    SHORT loc_1_3E27
        mov    eax, dword ptr [esp + 4]
loc_1_3E27:
        call   sub_1_0BD7
        jb     NEAR PTR loc_1_3F49
        cmp    dword ptr [esp + 1ch], 0
        jne    SHORT loc_1_3E49
        mov    al, byte ptr [ebx + 0bh]
        and    al, 1fh
        cmp    al, 0fh
        jne    SHORT loc_1_3E49
        mov    al, byte ptr [ebx + 0dh]
        mov    byte ptr [esp + 0dh], al
loc_1_3E49:
        cmp    dword ptr [esp + 20h], 0
        je     NEAR PTR loc_1_3ED9
        mov    dword ptr [esp + 24h], ebx
        mov    dword ptr [esp + 28h], edx
        sub    edx, edx
        movzx  eax, word ptr [esp + 1ah]
        mov    ecx, 1ah
        div    ecx
        mov    ecx, edx
        mov    edx, 1ah
        mul    edx
        add    dword ptr [esp + 20h], eax
        jecxz  loc_1_3ECC
        mov    eax, 0ffffffffh
        lea    edi, [ebx + 1]
        stosd
        stosd
        stosw
        lea    edi, [edi + 3]
        stosd
        stosd
        stosd
        lea    edi, [edi + 2]
        stosd
        mov    esi, dword ptr [esp + 20h]
        lea    edi, [ebx + 1]
        mov    eax, 20605h
        mov    edx, 1c0eh
        shr    ecx, 1
        inc    ecx
loc_1_3EA3:
        movsw
        dec    al
        loopne loc_1_3EA3
        jecxz  loc_1_3EB9
        movzx  edi, dl
        lea    edi, [edi + ebx]
        shr    edx, 8
        shr    eax, 8
        jne    SHORT loc_1_3EA3
loc_1_3EB9:
        dec    byte ptr [esp + 0ch]
        sub    dword ptr [esp + 20h], 1ah
        mov    ebx, dword ptr [esp + 24h]
        mov    edx, dword ptr [esp + 28h]
        jmp    SHORT loc_1_3F07
loc_1_3ECC:
        sub    dword ptr [esp + 20h], 1ah
        mov    ebx, dword ptr [esp + 24h]
        mov    edx, dword ptr [esp + 28h]
loc_1_3ED9:
        cmp    byte ptr [esp + 0ch], 0
        je     SHORT loc_1_3F23
        dec    byte ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 20h]
        test   esi, esi
        jne    SHORT loc_1_3EF1
        mov    byte ptr [ebx], 0e5h
        jmp    SHORT loc_1_3F07
loc_1_3EF1:
        lea    edi, [ebx + 1]
        movsd
        movsd
        movsw
        lea    edi, [edi + 3]
        movsd
        movsd
        movsd
        lea    edi, [edi + 2]
        movsd
        sub    dword ptr [esp + 20h], 1ah
loc_1_3F07:
        mov    al, byte ptr [esp + 0dh]
        mov    byte ptr [ebx + 0dh], al
        mov    esi, dword ptr [ebp + 64h]
        push   ebx
        push   edx
        call   sub_1_03EC
        pop    edx
        pop    ebx
        call   sub_1_0CDE
        ja     SHORT loc_1_3ED9
        jmp    SHORT loc_1_3F49
loc_1_3F23:
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_1_3F35
        lea    esi, [esp + 0eh]
        lea    edi, [ebx]
        movsd
        movsd
        movsw
        movsb
loc_1_3F35:
        mov    esi, dword ptr [ebp + 64h]
        call   sub_1_03EC
        jb     SHORT loc_1_3F49
        add    esp, 2ch
        clc
        ret
        db  083h, 0C4h, 02Ch, 0F9h, 0C3h
loc_1_3F49:
        add    esp, 2ch
        mov    al, 5
        stc
        ret
loc_1_3F50:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cld
        sub    eax, eax
        mov    byte ptr [ebp + 37h], al
        mov    word ptr [ebp + 38h], ax
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        db  080h, 03Dh, 086h, 0B7h, 000h, 000h, 000h
        jne    SHORT loc_1_3F76
        call   sub_1_A26B
loc_1_3F76:
        mov    edi, dword ptr [ebp + 1ch]
        cmp    byte ptr [ebp + 4], 2
        je     NEAR PTR loc_1_40CD
        mov    esi, dword ptr [edi + 14h]
        db  0BFh, 048h, 0B7h, 000h, 000h
loc_1_3F8B:
        mov    edi, dword ptr [edi]
        db  081h, 0FFh, 048h, 0B7h, 000h, 000h
        je     NEAR PTR loc_1_4089
        cmp    dword ptr [edi + 14h], esi
        jne    SHORT loc_1_3F8B
        db  0B8h, 020h, 0B7h, 000h, 000h
loc_1_3FA3:
        mov    eax, dword ptr [eax + 8]
        db  03Dh, 020h, 0B7h, 000h, 000h
        je     SHORT loc_1_3FBC
        cmp    dword ptr [eax + 28h], edi
        jne    SHORT loc_1_3FA3
loc_1_3FB2:
        mov    eax, 4
        jmp    NEAR PTR loc_1_4177
loc_1_3FBC:
        cmp    byte ptr [ebp + 4], 1
        jne    SHORT loc_1_3FDD
        pushfd
        cli
        db  0B9h, 0FCh, 0B6h, 000h, 000h
loc_1_3FC9:
        mov    ecx, dword ptr [ecx + 1ch]
        db  081h, 0F9h, 0FCh, 0B6h, 000h, 000h
        je     SHORT loc_1_3FDC
        cmp    dword ptr [ecx + 0ch], edi
        jne    SHORT loc_1_3FC9
        popfd
        jmp    SHORT loc_1_3FB2
loc_1_3FDC:
        popfd
loc_1_3FDD:
        push   esi
        push   edi
        xor    ecx, ecx
        xchg   dword ptr [edi + 9ch], ecx
        jecxz  loc_1_3FFB
        db  083h, 03Dh, 070h, 0BDh, 000h, 000h, 000h
        jne    SHORT loc_1_3FF9
        xchg   ecx, eax
        call   sub_1_A1F8
        xchg   ecx, eax
loc_1_3FF9:
        sub    ecx, ecx
loc_1_3FFB:
        xchg   dword ptr [edi + 90h], ecx
        jecxz  loc_1_4013
        db  083h, 03Dh, 070h, 0BDh, 000h, 000h, 000h
        jne    SHORT loc_1_4013
        xchg   ecx, eax
        call   sub_1_A1F8
        xchg   ecx, eax
loc_1_4013:
        cmp    byte ptr [ebp + 4], 1
        je     SHORT loc_1_403B
        mov    eax, dword ptr [esp]
        push   dword ptr [ebp + 1ch]
        mov    dword ptr [ebp + 1ch], eax
        bt     dword ptr [eax + 1fh], 4
        jb     SHORT loc_1_402E
        call   sub_1_42C0
loc_1_402E:
        call   sub_1_05FA
        call   sub_1_1D22
        pop    dword ptr [ebp + 1ch]
loc_1_403B:
        mov    ebx, dword ptr [esp]
        call   sub_1_A72F
        pop    edi
        pop    esi
        mov    edx, edi
        call   sub_1_58BA
        xchg   edi, edx
        jb     NEAR PTR loc_1_3F8B
        test   byte ptr [edi + 52h], 3
        je     SHORT loc_1_406D
        mov    eax, dword ptr [edi + 40h]
        mov    word ptr [edx + 1fh], ax
        mov    eax, dword ptr [edi + 38h]
        mov    word ptr [edx + 1dh], ax
        jmp    NEAR PTR loc_1_3F8B
loc_1_406D:
        cmp    word ptr [edx + 0fh], 0
        jne    NEAR PTR loc_1_3F8B
        mov    eax, dword ptr [edi + 40h]
        mov    dword ptr [edx + 1fh], eax
        mov    eax, dword ptr [edi + 38h]
        mov    dword ptr [edx + 39h], eax
        jmp    NEAR PTR loc_1_3F8B
loc_1_4089:
        db  0BFh, 048h, 0B7h, 000h, 000h
loc_1_408E:
        mov    edi, dword ptr [edi]
        db  081h, 0FFh, 048h, 0B7h, 000h, 000h
        je     NEAR PTR loc_1_4175
        cmp    dword ptr [edi + 14h], esi
        jne    SHORT loc_1_408E
        mov    ebx, dword ptr [edi]
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [ebx + 4], ecx
        mov    dword ptr [ecx], ebx
        db  083h, 03Dh, 070h, 0BDh, 000h, 000h, 000h
        je     SHORT loc_1_40BB
        mov    ebx, edi
        call   sub_1_14FB
loc_1_40BB:
        db  083h, 03Dh, 070h, 0BDh, 000h, 000h, 000h
        jne    SHORT loc_1_40CB
        xchg   edi, eax
        call   sub_1_A1F8
        xchg   edi, eax
loc_1_40CB:
        jmp    SHORT loc_1_4089
loc_1_40CD:
        db  0B8h, 020h, 0B7h, 000h, 000h
loc_1_40D2:
        mov    eax, dword ptr [eax + 8]
        db  03Dh, 020h, 0B7h, 000h, 000h
        je     SHORT loc_1_40EB
        cmp    dword ptr [eax + 28h], edi
        jne    SHORT loc_1_40D2
        mov    eax, 4
        jmp    NEAR PTR loc_1_4177
loc_1_40EB:
        push   edi
        xor    ecx, ecx
        xchg   dword ptr [edi + 9ch], ecx
        jecxz  loc_1_4108
        db  083h, 03Dh, 070h, 0BDh, 000h, 000h, 000h
        jne    SHORT loc_1_4106
        xchg   ecx, eax
        call   sub_1_A1F8
        xchg   ecx, eax
loc_1_4106:
        sub    ecx, ecx
loc_1_4108:
        xchg   dword ptr [edi + 90h], ecx
        jecxz  loc_1_4120
        db  083h, 03Dh, 070h, 0BDh, 000h, 000h, 000h
        jne    SHORT loc_1_4120
        xchg   ecx, eax
        call   sub_1_A1F8
        xchg   ecx, eax
loc_1_4120:
        mov    eax, dword ptr [esp]
        push   dword ptr [ebp + 1ch]
        mov    dword ptr [ebp + 1ch], eax
        bt     dword ptr [eax + 1fh], 4
        jb     SHORT loc_1_4135
        call   sub_1_42C0
loc_1_4135:
        call   sub_1_05FA
        call   sub_1_1D22
        pop    dword ptr [ebp + 1ch]
        mov    ebx, dword ptr [esp]
        call   sub_1_A72F
        pop    edi
        mov    ebx, dword ptr [edi]
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [ebx + 4], ecx
        mov    dword ptr [ecx], ebx
        db  083h, 03Dh, 070h, 0BDh, 000h, 000h, 000h
        je     SHORT loc_1_4165
        mov    ebx, edi
        call   sub_1_14FB
loc_1_4165:
        db  083h, 03Dh, 070h, 0BDh, 000h, 000h, 000h
        jne    SHORT loc_1_4175
        xchg   edi, eax
        call   sub_1_A1F8
        xchg   edi, eax
loc_1_4175:
        sub    eax, eax
loc_1_4177:
        mov    word ptr [ebp + 1ah], ax
        push   eax
        db  080h, 03Dh, 086h, 0B7h, 000h, 000h, 000h
        jne    SHORT loc_1_418A
        call   sub_1_A2CE
loc_1_418A:
        pop    eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_4190:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cld
        mov    ax, word ptr [ebp + 18h]
        and    ah, 91h
        mov    byte ptr [ebp + 37h], ah
        sub    eax, eax
        mov    word ptr [ebp + 38h], ax
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        call   sub_1_A318
        mov    edi, dword ptr [ebp + 1ch]
        cmp    byte ptr [ebp + 4], 1
        jne    SHORT loc_1_41D2
        mov    ebx, dword ptr [edi + 2ch]
        dec    ebx
        mov    edx, dword ptr [edi + 40h]
        cmp    edx, -1
        je     SHORT loc_1_41F4
        cmp    ebx, edx
        jae    SHORT loc_1_41F4
        or     edx, 0ffffffffh
        jmp    SHORT loc_1_41F4
loc_1_41D2:
        call   sub_1_5BEF
        jb     NEAR PTR loc_1_42B9
        test   byte ptr [edi + 52h], 10h
        je     SHORT loc_1_41F4
        call   sub_1_5B05
        cmp    eax, edx
        jae    SHORT loc_1_41F4
        mov    ecx, ebx
        sub    ebx, edx
        add    ebx, eax
        mov    edx, eax
loc_1_41F4:
        movzx  ecx, word ptr [edi + 20h]
        movzx  eax, byte ptr [edi + 24h]
        inc    eax
        test   byte ptr [ebp + 18h], 1
        jne    SHORT loc_1_4253
        mov    dword ptr [ebp], ecx
        bsr    ecx, ebx
        sub    ecx, 0fh
        jbe    SHORT loc_1_423B
        mov    ch, 6
        sub    ch, byte ptr [edi + 25h]
        cmp    cl, ch
        jbe    SHORT loc_1_4219
        mov    cl, ch
loc_1_4219:
        shl    eax, cl
        shr    ebx, cl
        shr    edx, cl
        cmp    ebx, 10000h
        jb     SHORT loc_1_422D
        db  08Bh, 01Dh, 0A0h, 0B8h, 000h, 000h
loc_1_422D:
        cmp    edx, 10000h
        jb     SHORT loc_1_423B
        db  08Bh, 015h, 0A4h, 0B8h, 000h, 000h
loc_1_423B:
        mov    dword ptr [ebp + 24h], ebx
        mov    dword ptr [ebp + 28h], edx
loc_1_4241:
        mov    word ptr [ebp + 18h], ax
        mov    cl, byte ptr [edi + 0bah]
        mov    byte ptr [ebp + 4], cl
        jmp    NEAR PTR loc_1_4574
loc_1_4253:
        mov    esi, dword ptr [ebp + 14h]
        cmp    dword ptr [ebp], 2ch
        jb     SHORT loc_1_42B9
        mov    word ptr [esi], 2ch
        mov    word ptr [esi + 2], 0
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi + 8], ecx
        mov    dword ptr [esi + 0ch], edx
        mov    dword ptr [esi + 10h], ebx
        mov    eax, dword ptr [edi + 2ch]
        dec    eax
        mov    dword ptr [esi + 20h], eax
        mov    cl, byte ptr [edi + 25h]
        shl    eax, cl
        mov    dword ptr [esi + 18h], eax
        mov    eax, dword ptr [edi + 40h]
        mov    dword ptr [esi + 1ch], eax
        shl    eax, cl
        mov    dword ptr [esi + 14h], eax
        test   byte ptr [edi + 52h], 10h
        je     SHORT loc_1_42AE
        mov    eax, 29h
        mov    ebx, edi
        mov    ecx, 0db0bh
        push   esi
        call   sub_1_44AF
        pop    esi
        jb     SHORT loc_1_42AE
        mov    dword ptr [esi + 18h], edx
        mov    dword ptr [esi + 14h], ecx
loc_1_42AE:
        xor    eax, eax
        mov    dword ptr [esi + 24h], eax
        mov    dword ptr [esi + 28h], eax
        dec    eax
        jmp    SHORT loc_1_4241
loc_1_42B9:
        mov    al, 5
        jmp    NEAR PTR loc_1_4576
sub_1_42C0:
        test   byte ptr [eax + 52h], 3
        jne    SHORT loc_1_4315
        mov    ebx, dword ptr [eax + 14h]
        test   byte ptr [ebx + 4], 40h
        jne    SHORT loc_1_4315
        mov    ebx, dword ptr [eax + 0a0h]
        cmp    ebx, -1
        je     SHORT loc_1_4315
        call   sub_1_00D0
        jb     SHORT loc_1_4312
        add    ebx, 1e4h
        cmp    dword ptr [ebx], 61417272h
        jne    SHORT loc_1_4312
        xor    ecx, ecx
        mov    eax, dword ptr [edi + 40h]
        cmp    dword ptr [ebx + 4], eax
        je     SHORT loc_1_42FD
        mov    dword ptr [ebx + 4], eax
        inc    ecx
loc_1_42FD:
        mov    eax, dword ptr [edi + 38h]
        cmp    dword ptr [ebx + 8], eax
        je     SHORT loc_1_4309
        mov    dword ptr [ebx + 8], eax
        inc    ecx
loc_1_4309:
        jecxz  loc_1_4312
        xor    edx, edx
        call   sub_1_03F3
loc_1_4312:
        mov    eax, dword ptr [ebp + 1ch]
loc_1_4315:
        ret
sub_1_4316:
        mov    eax, dword ptr [ebp + 1ch]
        db  0BAh, 020h, 0B7h, 000h, 000h
loc_1_431E:
        mov    edx, dword ptr [edx + 8]
        db  081h, 0FAh, 020h, 0B7h, 000h, 000h
        je     SHORT loc_1_4340
        cmp    dword ptr [edx + 28h], eax
        jne    SHORT loc_1_431E
        mov    ecx, dword ptr [edx]
        mov    ecx, dword ptr [ecx + 4]
loc_1_4333:
        mov    ecx, dword ptr [ecx]
        cmp    ecx, edx
        je     SHORT loc_1_431E
        test   byte ptr [ecx + 0fh], 2
        jne    SHORT loc_1_4333
        stc
loc_1_4340:
        ret
loc_1_4341:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cld
        sub    eax, eax
        mov    byte ptr [ebp + 37h], al
        mov    word ptr [ebp + 38h], ax
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        test   byte ptr [ebp + 18h], 2
        je     SHORT loc_1_436F
        call   sub_1_B02E
        jae    NEAR PTR loc_1_4574
        jmp    NEAR PTR loc_1_4576
loc_1_436F:
        call   sub_1_A26B
        mov    eax, dword ptr [ebp + 1ch]
        call   sub_1_42C0
        call   sub_1_05FA
        call   sub_1_4316
        jb     SHORT loc_1_438D
        call   sub_1_1D22
loc_1_438D:
        mov    ebx, dword ptr [ebp + 1ch]
        test   byte ptr [ebx + 52h], 3
        je     SHORT loc_1_43AA
        mov    dword ptr [ebx + 38h], 2
        mov    eax, dword ptr [ebx + 60h]
        or     eax, eax
        je     SHORT loc_1_43AA
        mov    word ptr [eax + 1dh], 2
loc_1_43AA:
        mov    eax, 27h
        test   byte ptr [ebp + 18h], 1
        je     SHORT loc_1_43BF
        call   sub_1_A72F
        mov    eax, 28h
loc_1_43BF:
        call   sub_1_44AF
        jmp    NEAR PTR loc_1_4574
sub_1_43C9:
        test   edx, 0c0000000h
        je     SHORT loc_1_43D6
        or     al, 1
        and    ah, 3fh
loc_1_43D6:
        test   edx, 20000000h
        je     SHORT loc_1_43E0
        or     al, 4
loc_1_43E0:
        test   edx, 14000000h
        je     SHORT loc_1_43EA
        or     al, 20h
loc_1_43EA:
        test   edx, 14000000h
        jne    SHORT loc_1_43F5
        and    ah, 0fbh
loc_1_43F5:
        test   edx, 4000000h
        jne    SHORT loc_1_43FF
        or     al, 8
loc_1_43FF:
        mov    edx, dword ptr [ebp + 1ch]
        cmp    dword ptr [edx + 90h], 0
        je     SHORT loc_1_4410
        test   ah, 28h
        je     SHORT loc_1_4413
loc_1_4410:
        and    ah, 3fh
loc_1_4413:
        mov    word ptr [ebp + 42h], ax
        ret
sub_1_4418:
        test   edx, 14000000h
        jne    SHORT loc_1_444D
        push   ecx
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        movzx  ecx, word ptr [edi + 2]
        movzx  ecx, word ptr [ecx + edi]
        shr    ecx, 1
        je     SHORT loc_1_444B
        dec    ecx
        je     SHORT loc_1_444B
        mov    edi, dword ptr [ebp + 28h]
loc_1_4437:
        cmp    word ptr [edi], 80h
        jae    SHORT loc_1_4445
        lea    edi, [edi + 2]
        loop   loc_1_4437
        jmp    SHORT loc_1_444B
loc_1_4445:
        or     edx, 10000000h
loc_1_444B:
        pop    edi
        pop    ecx
loc_1_444D:
        ret
sub_1_444E:
        mov    edx, dword ptr [ebp + 1ch]
        mov    esi, dword ptr [ebp + 64h]
        cmp    edx, dword ptr [edi + 28h]
        jne    SHORT loc_1_449C
        cmp    edx, dword ptr [esi + 0ch]
        jne    SHORT loc_1_449C
        bt     dword ptr [edi + 24h], 3
        sbb    eax, eax
        and    eax, dword ptr [ebx + 12h]
        mov    ax, word ptr [ebx + 1ah]
        or     eax, eax
        je     SHORT loc_1_4475
        cmp    eax, dword ptr [edi + 10h]
        jne    SHORT loc_1_449C
loc_1_4475:
        mov    eax, dword ptr [ebp + 50h]
        mov    dword ptr [edi + 18h], eax
        mov    eax, dword ptr [ebp + 4ch]
        mov    dword ptr [edi + 14h], eax
        mov    eax, ebx
        sub    eax, dword ptr [esi + 10h]
        shr    eax, 5
        mov    byte ptr [edi + 25h], al
        mov    cl, byte ptr [edx + 4ch]
        sub    cl, 5
        shr    eax, cl
        add    eax, dword ptr [esi + 8]
        mov    dword ptr [edi + 1ch], eax
        clc
        ret
loc_1_449C:
        stc
        ret
sub_1_449E:
        mov    eax, 29h
        call   sub_1_44AF
        jb     SHORT loc_1_44AE
        cmp    edx, 1
        cmc
loc_1_44AE:
        ret
sub_1_44AF:
        push   eax
        push   edx
        mov    edx, ebx
        mov    eax, dword ptr [ebx + 14h]
        movzx  ebx, word ptr [eax + 0ah]
        mov    esi, dword ptr [eax + 0ch]
        push   eax
        mov    ah, 1
        test   byte ptr [ebp + 37h], 90h
        jne    SHORT loc_1_44C8
        mov    ah, 2
loc_1_44C8:
        call   sub_1_A4E1
        jb     SHORT loc_1_452D
        mov    esi, ebx
        pop    dword ptr [ebx + 44h]
        mov    al, byte ptr [edx + 0ach]
        mov    byte ptr [esi + 4dh], al
        mov    dword ptr [esi + 8], 400h
        db  0C7h, 046h, 00Ch, 038h, 045h, 000h, 000h
        pop    edx
        pop    eax
        mov    word ptr [esi + 4], ax
        cmp    eax, 29h
        jne    SHORT loc_1_44FD
        mov    word ptr [esi + 2eh], cx
        mov    dword ptr [esi + 30h], edx
loc_1_44FD:
        db  0CDh, 020h, 004h, 000h, 010h, 000h
loc_1_4503:
        cli
        cmp    dword ptr [esi + 0ch], 0
        je     SHORT loc_1_4516
        db  0B8h, 0B4h, 0B6h, 000h, 000h
        call   sub_1_A220
        jmp    SHORT loc_1_4503
loc_1_4516:
        sti
        mov    ecx, dword ptr [esi + 30h]
        mov    edx, dword ptr [esi + 3ch]
        movzx  eax, word ptr [esi + 6]
        cmp    eax, 10h
        cmc
        mov    ebx, esi
        call   sub_1_A4AE
loc_1_452C:
        ret
loc_1_452D:
        add    esp, 0ch
        mov    eax, 1dh
        stc
        jmp    SHORT loc_1_452C
loc_1_4538:
        and    dword ptr [esi + 0ch], 0
        db  0B8h, 0B4h, 0B6h, 000h, 000h
        jmp    NEAR PTR sub_1_A258
        db  08Bh, 0C0h
loc_1_4548:
        sub    eax, eax
loc_1_454A:
        test   byte ptr [ebp + 39h], 20h
        je     SHORT loc_1_4576
        sub    ecx, ecx
        xchg   dword ptr [ebp + 6ch], ecx
        jecxz  loc_1_4576
        db  087h, 00Dh, 044h, 0B8h, 000h, 000h
        jecxz  loc_1_4576
        xchg   ecx, eax
        call   sub_1_A1F8
        xchg   ecx, eax
        jmp    SHORT loc_1_4576
loc_1_4568:
        call   sub_1_9EB2
loc_1_456D:
        mov    al, 6
        jmp    SHORT loc_1_4576
        db  02Eh, 08Bh, 0C0h
loc_1_4574:
        sub    eax, eax
loc_1_4576:
        movzx  eax, al
loc_1_4579:
        mov    word ptr [ebp + 1ah], ax
        test   byte ptr [ebp + 37h], 11h
        jne    SHORT loc_1_459D
        test   byte ptr [ebp + 38h], 4
        je     SHORT loc_1_459D
        xor    ecx, ecx
        xchg   byte ptr [ebp + 34h], cl
        push   ecx
        call   sub_1_0636
        pop    ecx
        mov    byte ptr [ebp + 34h], cl
        jae    SHORT loc_1_459D
        mov    byte ptr [ebp + 34h], al
loc_1_459D:
        mov    ebx, dword ptr [ebp + 1ch]
        btr    dword ptr [ebx + 1fh], 6
        jb     SHORT loc_1_45BE
        movzx  eax, byte ptr [ebp + 34h]
        or     eax, eax
        je     SHORT loc_1_45CF
        db  0BFh, 0F0h, 022h, 000h, 000h
        mov    ecx, 4
        cld
        repne scasb
        jne    SHORT loc_1_45CF
loc_1_45BE:
        test   byte ptr [ebp + 37h], 11h
        je     SHORT loc_1_45CA
        or     byte ptr [ebx + 1fh], 40h
        jmp    SHORT loc_1_45CF
loc_1_45CA:
        call   sub_1_1D38
loc_1_45CF:
        movzx  eax, word ptr [ebp + 1ah]
        bt     dword ptr [ebp + 38h], 0fh
        jae    SHORT loc_1_4604
        or     eax, eax
        jne    SHORT loc_1_4604
        mov    edx, dword ptr [ebp + 1ch]
        test   byte ptr [edx + 52h], 3
        jne    SHORT loc_1_4604
        mov    ecx, dword ptr [edx + 14h]
        test   byte ptr [ecx + 1], 40h
        je     SHORT loc_1_4604
        test   byte ptr [ecx + 2], 80h
        jne    SHORT loc_1_4604
        push   eax
        mov    eax, edx
        call   sub_1_42C0
        call   sub_1_1D22
        pop    eax
loc_1_4604:
        db  083h, 03Dh, 0B0h, 0B8h, 000h, 000h, 000h
        je     SHORT loc_1_4628
        test   byte ptr [ebp + 37h], 90h
        je     SHORT loc_1_461F
        db  0F7h, 005h, 0ACh, 0B8h, 000h, 000h, 0FFh, 0FFh, 0FFh, 000h
        jne    SHORT loc_1_4628
loc_1_461F:
        call   sub_1_A2CE
        jmp    SHORT loc_1_462D
        db  08Bh, 0C0h
loc_1_4628:
        call   sub_1_A361
loc_1_462D:
        movzx  ecx, byte ptr [ebp + 34h]
        jecxz  loc_1_463D
        db  0CDh, 020h, 00Fh, 000h, 010h, 000h
loc_1_4639:
        mov    word ptr [ebp + 1ah], ax
loc_1_463D:
        movzx  ebx, byte ptr [ebp + 35h]
        or     bl, bl
        je     SHORT loc_1_4655
        mov    edi, dword ptr [ebp + 1ch]
        btr    dword ptr [edi + 1ch], ebx
        db  083h, 03Dh, 0F4h, 0B6h, 000h, 000h, 000h
        jne    SHORT loc_1_465A
loc_1_4655:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_465A:
        push   eax
        db  0B8h, 0F4h, 0B6h, 000h, 000h
        call   sub_1_A258
        pop    eax
        jmp    SHORT loc_1_4655
loc_1_4668:
        mov    al, 32h
        jmp    NEAR PTR loc_1_4576
        db  000h
loc_1_4670:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cld
        sub    eax, eax
        mov    byte ptr [ebp + 37h], al
        mov    word ptr [ebp + 38h], ax
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        call   sub_1_A318
        mov    edx, dword ptr [ebp + 20h]
        test   byte ptr [edx + 0eh], 10h
        jne    NEAR PTR loc_1_456D
        mov    edx, dword ptr [edx + 8]
        lea    edx, [edx + 64h]
        movzx  eax, word ptr [ebp + 18h]
        test   byte ptr [ebp + 4], 1
        jne    SHORT loc_1_46C6
        push   eax
        push   dword ptr [ebp + 20h]
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 28h]
        push   dword ptr [ebp + 24h]
        push   edx
        db  0CDh, 020h, 053h, 000h, 040h, 000h
        add    esp, 18h
        jmp    NEAR PTR loc_1_454A
loc_1_46C6:
        push   eax
        push   dword ptr [ebp + 20h]
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 28h]
        push   dword ptr [ebp + 24h]
        push   edx
        db  0CDh, 020h, 054h, 000h, 040h, 000h
        add    esp, 18h
        jmp    NEAR PTR loc_1_454A
loc_1_46E2:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cld
        sub    eax, eax
        mov    byte ptr [ebp + 37h], al
        mov    word ptr [ebp + 38h], ax
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        call   sub_1_A318
        mov    edi, dword ptr [ebp + 20h]
        test   byte ptr [edi + 0eh], 10h
        jne    NEAR PTR loc_1_456D
        mov    esi, dword ptr [edi + 8]
        cmp    byte ptr [ebp + 4], 7
        je     NEAR PTR loc_1_47E3
        cmp    byte ptr [ebp + 4], 6
        je     SHORT loc_1_476C
        cmp    byte ptr [ebp + 4], 5
        je     NEAR PTR loc_1_47B4
        cmp    byte ptr [ebp + 4], 4
        je     SHORT loc_1_4752
        cmp    byte ptr [ebp + 4], 1
        je     SHORT loc_1_4797
        cmp    byte ptr [ebp + 4], 0
        jne    NEAR PTR loc_1_4833
        mov    eax, dword ptr [esi + 38h]
        mov    word ptr [ebp + 28h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 2ah], ax
        jmp    NEAR PTR loc_1_4574
loc_1_4752:
        sub    edx, edx
        movzx  eax, word ptr [esi + 3ch]
        call   sub_1_13B5
        mov    word ptr [ebp + 28h], 0
        mov    word ptr [ebp + 2ah], ax
        jmp    NEAR PTR loc_1_4574
loc_1_476C:
        sub    edx, edx
        mov    eax, dword ptr [esi + 3eh]
        call   sub_1_13D1
        mov    word ptr [ebp + 28h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 2ah], ax
        movzx  eax, byte ptr [esi + 42h]
        cmp    eax, 0c8h
        sbb    edx, edx
        and    eax, edx
        mov    word ptr [ebp + 18h], ax
        jmp    NEAR PTR loc_1_4574
loc_1_4797:
        mov    ax, word ptr [ebp + 2ah]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 28h]
        mov    dword ptr [esi + 38h], eax
        and    word ptr [edi + 0eh], 0fff3h
        or     word ptr [edi + 0eh], 40h
        jmp    NEAR PTR loc_1_4574
loc_1_47B4:
        mov    eax, dword ptr [esi + 28h]
        test   word ptr [eax + 52h], 100h
        jne    SHORT loc_1_482C
        sub    edx, edx
        movzx  eax, word ptr [ebp + 2ah]
        or     eax, eax
        je     SHORT loc_1_47D0
        call   sub_1_13B5
        jb     SHORT loc_1_4825
loc_1_47D0:
        mov    word ptr [esi + 3ch], ax
        and    word ptr [edi + 0eh], 0fffbh
        or     word ptr [edi + 0eh], 20h
        jmp    NEAR PTR loc_1_4574
loc_1_47E3:
        mov    eax, dword ptr [esi + 28h]
        test   word ptr [eax + 52h], 100h
        jne    SHORT loc_1_482C
        mov    ax, word ptr [ebp + 2ah]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 28h]
        sub    edx, edx
        or     eax, eax
        je     SHORT loc_1_4815
        mov    edx, dword ptr [esi + 3eh]
        call   sub_1_13D1
        jb     SHORT loc_1_4825
        movzx  edx, word ptr [ebp + 18h]
        cmp    edx, 0c8h
        jae    SHORT loc_1_4825
loc_1_4815:
        mov    byte ptr [esi + 42h], dl
        mov    dword ptr [esi + 3eh], eax
        and    word ptr [edi + 0eh], 0fffbh
        jmp    NEAR PTR loc_1_4574
loc_1_4825:
        mov    al, 0dh
        jmp    NEAR PTR loc_1_4576
loc_1_482C:
        mov    al, 5
        jmp    NEAR PTR loc_1_4576
loc_1_4833:
        mov    al, 1
        jmp    NEAR PTR loc_1_4576
loc_1_483A:
        pushfd
        db  0FFh, 005h, 068h, 0B7h, 000h, 000h
        popfd
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cld
        mov    ax, word ptr [ebp + 18h]
        and    ah, 91h
        mov    byte ptr [ebp + 37h], ah
        sub    eax, eax
        mov    word ptr [ebp + 38h], ax
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        mov    esi, dword ptr [ebp + 20h]
        mov    esi, dword ptr [esi + 8]
        test   byte ptr [esi + 43h], 8
        jne    NEAR PTR loc_1_49D7
        mov    byte ptr [ebp + 3ah], 0
        clc
        call   sub_1_9DFC
        call   sub_1_A318
        mov    esi, dword ptr [ebp + 20h]
        test   byte ptr [esi + 0eh], 10h
        jne    NEAR PTR loc_1_4568
        mov    edx, dword ptr [esi + 8]
        test   byte ptr [ebp + 37h], 90h
        jne    NEAR PTR loc_1_4969
        test   byte ptr [edx + 23h], 81h
        je     SHORT loc_1_48A7
        mov    edi, edx
        call   sub_1_A02F
loc_1_48A7:
        inc    dword ptr [edx + 20h]
loc_1_48AA:
        mov    al, byte ptr [esi + 0ch]
        and    al, 3
        cmp    al, 1
        je     NEAR PTR loc_1_498C
        mov    ebx, dword ptr [ebp + 24h]
        mov    eax, dword ptr [edx + 30h]
        sub    eax, ebx
        ja     SHORT loc_1_48C6
        mov    ebx, dword ptr [edx + 30h]
        sub    eax, eax
loc_1_48C6:
        mov    dword ptr [ebp + 3ch], ebx
        mov    ecx, dword ptr [ebp]
        jecxz  loc_1_4933
        cmp    eax, ecx
        jae    SHORT loc_1_48D9
        mov    dword ptr [ebp], eax
        or     eax, eax
        je     SHORT loc_1_4933
loc_1_48D9:
        cmp    dword ptr [edx + 64h], 0
        je     SHORT loc_1_490D
        mov    eax, 1
        mov    ecx, dword ptr [ebp]
        mov    edx, ebx
        add    ecx, edx
        mov    edi, esi
        call   sub_1_9D98
        mov    al, 21h
        jb     NEAR PTR loc_1_498E
        test   byte ptr [ebp + 37h], 10h
        je     SHORT loc_1_490D
        db  03Bh, 01Dh, 0BCh, 0B6h, 000h, 000h
        jne    SHORT loc_1_490D
        jmp    NEAR PTR loc_1_49B7
loc_1_490D:
        call   sub_1_6FE4
        jb     SHORT loc_1_498C
        test   byte ptr [ebp + 37h], 10h
        jne    SHORT loc_1_4949
        test   byte ptr [ebp + 37h], 1
        jne    SHORT loc_1_4933
        test   word ptr [ebp + 18h], 2000h
        jne    SHORT loc_1_4933
        mov    eax, dword ptr [ebp + 24h]
        mov    edx, dword ptr [ebp]
        call   sub_1_0685
loc_1_4933:
        mov    esi, dword ptr [ebp + 20h]
        mov    edi, dword ptr [esi + 8]
        test   byte ptr [ebp + 37h], 90h
        jne    SHORT loc_1_4949
        dec    dword ptr [edi + 20h]
        jns    SHORT loc_1_4949
        call   sub_1_A046
loc_1_4949:
        mov    ecx, dword ptr [ebp]
        jecxz  loc_1_495F
        pushfd
        db  001h, 00Dh, 060h, 0B7h, 000h, 000h
        popfd
        add    ecx, dword ptr [ebp + 3ch]
        mov    dword ptr [ebp + 24h], ecx
        mov    dword ptr [ebp + 3ch], ecx
loc_1_495F:
        call   sub_1_9EB2
        jmp    NEAR PTR loc_1_4574
loc_1_4969:
        test   byte ptr [ebp + 37h], 10h
        jne    NEAR PTR loc_1_48AA
        test   byte ptr [edx + 24h], 2
        jne    NEAR PTR loc_1_48AA
        call   sub_1_7EEA
        jae    NEAR PTR loc_1_48AA
        mov    al, 1fh
        jmp    SHORT loc_1_498E
loc_1_498C:
        mov    al, 5
loc_1_498E:
        mov    dword ptr [ebp], 0
        mov    esi, dword ptr [ebp + 20h]
        mov    edi, dword ptr [esi + 8]
        push   eax
        test   byte ptr [ebp + 37h], 90h
        jne    SHORT loc_1_49AC
        dec    dword ptr [edi + 20h]
        jns    SHORT loc_1_49AC
        call   sub_1_A046
loc_1_49AC:
        call   sub_1_9EB2
        pop    eax
        jmp    NEAR PTR loc_1_4576
loc_1_49B7:
        push   edi
        push   ecx
        db  08Bh, 00Dh, 0C0h, 0B6h, 000h, 000h
        mov    edi, dword ptr [ebp + 14h]
        mov    esi, dword ptr [ecx + 10h]
        mov    ecx, 1000h
        call   sub_1_823A
        pop    ecx
        pop    edi
        pop    esi
        jmp    NEAR PTR loc_1_4949
loc_1_49D7:
        mov    dword ptr [ebp], 0
        mov    eax, 5
        jmp    NEAR PTR loc_1_4639
loc_1_49E8:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cld
        mov    ax, word ptr [ebp + 18h]
        and    ah, 91h
        mov    byte ptr [ebp + 37h], ah
        sub    eax, eax
        mov    word ptr [ebp + 38h], ax
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        mov    esi, dword ptr [ebp + 20h]
        mov    esi, dword ptr [esi + 8]
        test   byte ptr [esi + 43h], 8
        jne    NEAR PTR loc_1_4F7A
        mov    byte ptr [ebp + 3ah], 0
        stc
        call   sub_1_9DFC
        call   sub_1_A318
        pushfd
        db  0FFh, 005h, 06Ch, 0B7h, 000h, 000h
        popfd
        mov    esi, dword ptr [ebp + 20h]
        test   byte ptr [esi + 0eh], 10h
        jne    NEAR PTR loc_1_4568
        mov    edx, dword ptr [ebp]
        mov    eax, dword ptr [ebp + 24h]
        add    edx, eax
        jb     NEAR PTR loc_1_4F1F
        jns    SHORT loc_1_4A55
        test   byte ptr [esi + 0fh], 1
        je     NEAR PTR loc_1_4F1F
loc_1_4A55:
        mov    dword ptr [ebp + 3ch], eax
        mov    edx, dword ptr [esi + 8]
        test   byte ptr [edx + 24h], 4
        je     SHORT loc_1_4A67
        or     word ptr [ebp + 38h], 800h
loc_1_4A67:
        mov    eax, dword ptr [edx + 28h]
        cmp    dword ptr [ebp + 1ch], eax
        test   byte ptr [ebp + 37h], 1
        je     SHORT loc_1_4A89
        movzx  ecx, word ptr [eax + 22h]
        test   dword ptr [ebp + 24h], ecx
        jne    NEAR PTR loc_1_4F1F
        test   dword ptr [ebp], ecx
        jne    NEAR PTR loc_1_4F1F
loc_1_4A89:
        mov    edi, edx
        test   byte ptr [ebp + 37h], 90h
        jne    SHORT loc_1_4A9A
        call   sub_1_9FEC
        or     byte ptr [edi + 23h], 1
loc_1_4A9A:
        mov    al, byte ptr [esi + 0ch]
        and    al, 3
        cmp    al, 0
        je     NEAR PTR loc_1_4F1F
        cmp    dword ptr [edx + 64h], 0
        je     SHORT loc_1_4ACC
        mov    eax, 0
        mov    ecx, dword ptr [ebp]
        mov    edx, dword ptr [ebp + 24h]
        add    ecx, edx
        mov    edi, esi
        call   sub_1_9D98
        mov    edx, dword ptr [esi + 8]
        mov    al, 21h
        jb     NEAR PTR loc_1_4F21
loc_1_4ACC:
        mov    edi, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [edi + 14h]
        test   dword ptr [ecx + 4], 40h
        mov    al, 13h
        jne    NEAR PTR loc_1_4F21
        test   byte ptr [ebp + 37h], 10h
        jne    SHORT loc_1_4B2D
        test   dword ptr [ecx + 4], 8000h
        jne    SHORT loc_1_4B00
        test   word ptr [edi + 52h], 8000h
        jne    SHORT loc_1_4B00
        test   word ptr [esi + 0eh], 81h
        je     SHORT loc_1_4B09
loc_1_4B00:
        push   edx
        call   sub_1_5CB0
        pop    edx
        jmp    SHORT loc_1_4B2D
loc_1_4B09:
        test   byte ptr [edi + 52h], 10h
        je     SHORT loc_1_4B2D
        movzx  ecx, word ptr [edi + 4ah]
        mov    eax, dword ptr [ebp + 24h]
        and    eax, ecx
        add    eax, dword ptr [ebp]
        add    eax, ecx
        mov    cl, byte ptr [edi + 47h]
        shr    eax, cl
        push   dword ptr [ebp + 3ch]
        call   sub_1_5B8D
        pop    dword ptr [ebp + 3ch]
loc_1_4B2D:
        mov    eax, dword ptr [ebp + 3ch]
        cmp    eax, dword ptr [edx + 30h]
        setae  bl
        or     byte ptr [ebp + 3ah], bl
        add    eax, dword ptr [ebp]
        jb     NEAR PTR loc_1_4F1F
        je     NEAR PTR loc_1_4BC8
        movzx  ebx, word ptr [edi + 4ah]
        mov    cl, byte ptr [edi + 47h]
        dec    cl
        add    eax, ebx
        rcr    eax, 1
        shr    eax, cl
        add    ebx, dword ptr [edx + 30h]
        rcr    ebx, 1
        shr    ebx, cl
        mov    ecx, eax
        sub    ecx, ebx
        jbe    SHORT loc_1_4BC8
        test   byte ptr [ebp + 37h], 10h
        je     SHORT loc_1_4B77
        xchg   edx, edi
        call   sub_1_9FEC
        or     byte ptr [edi + 23h], 1
        xchg   edx, edi
loc_1_4B77:
        dec    ebx
        mov    dword ptr [ebp + 40h], ebx
        mov    ebx, dword ptr [edx + 10h]
        and    ebx, ebx
        je     SHORT loc_1_4BA5
        mov    ebx, dword ptr [ebp + 40h]
        cmp    dword ptr [edx + 50h], ebx
        ja     SHORT loc_1_4B98
        xchg   ebx, eax
        push   ecx
        call   sub_1_2165
        pop    ecx
        jb     NEAR PTR loc_1_4D73
loc_1_4B98:
        push   ecx
        call   sub_1_7F50
        pop    ecx
        jb     NEAR PTR loc_1_4D73
loc_1_4BA5:
        mov    esi, dword ptr [esi + 8]
        call   sub_1_7816
        jb     NEAR PTR loc_1_4F21
        mov    esi, dword ptr [ebp + 20h]
        and    word ptr [esi + 0eh], 0fdffh
        mov    edx, dword ptr [esi + 8]
        cmp    dword ptr [edx + 10h], 0
        jne    SHORT loc_1_4BC8
        mov    dword ptr [edx + 10h], ebx
loc_1_4BC8:
        and    dword ptr [esi + 0ch], 0ffb3fffbh
        test   byte ptr [ebp + 37h], 10h
        je     SHORT loc_1_4BDB
        mov    eax, dword ptr [ebp + 24h]
        mov    dword ptr [ebp + 3ch], eax
loc_1_4BDB:
        cmp    dword ptr [ebp], 0
        je     NEAR PTR loc_1_4DB5
        mov    eax, dword ptr [ebp + 3ch]
        add    eax, dword ptr [ebp]
        cmp    eax, dword ptr [edx + 30h]
        jbe    SHORT loc_1_4BF6
        xchg   dword ptr [edx + 30h], eax
        mov    dword ptr [ebp + 70h], eax
loc_1_4BF6:
        test   byte ptr [ebp + 37h], 10h
        je     SHORT loc_1_4C5C
        db  083h, 03Dh, 0BCh, 0B6h, 000h, 000h, 0FFh
        je     SHORT loc_1_4C4C
        mov    edx, dword ptr [ebp + 24h]
        db  03Bh, 015h, 0BCh, 0B6h, 000h, 000h
        ja     SHORT loc_1_4C4C
        add    edx, dword ptr [ebp]
        db  03Bh, 015h, 0BCh, 0B6h, 000h, 000h
        jbe    SHORT loc_1_4C4C
        db  0C7h, 005h, 0BCh, 0B6h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  083h, 03Dh, 0B8h, 0B6h, 000h, 000h, 002h
        jne    SHORT loc_1_4C4C
        pushad
        db  08Bh, 035h, 0C0h, 0B6h, 000h, 000h
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        db  0CDh, 020h, 005h, 000h, 08Bh, 004h
        db  0C7h, 005h, 0B8h, 0B6h, 000h, 000h, 000h, 000h, 000h, 000h
        popad
loc_1_4C4C:
        call   sub_1_70A6
        jb     NEAR PTR loc_1_4F1F
        jmp    NEAR PTR loc_1_4CF7
loc_1_4C5C:
        test   byte ptr [ebp + 37h], 1
        jne    SHORT loc_1_4C83
        mov    ecx, dword ptr [ebp]
        cmp    ecx, 1000h
        jbe    SHORT loc_1_4C7F
        shr    ecx, 0ch
        inc    ecx
        db  08Ah, 025h, 0F8h, 0B6h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 08Bh, 004h
        jb     SHORT loc_1_4C83
loc_1_4C7F:
        or     byte ptr [ebp + 3ah], 2
loc_1_4C83:
        mov    esi, dword ptr [ebp + 20h]
        call   sub_1_774E
        jb     NEAR PTR loc_1_4F1F
        mov    esi, dword ptr [ebp + 20h]
        call   sub_1_7704
        mov    esi, dword ptr [ebp + 20h]
        cmp    dword ptr [ebp + 44h], 0
        je     SHORT loc_1_4CB4
        movzx  ebx, word ptr [ebp + 3ch]
        call   sub_1_762A
        jb     NEAR PTR loc_1_4D7A
        mov    esi, dword ptr [ebp + 20h]
loc_1_4CB4:
        cmp    dword ptr [ebp + 48h], 0
        je     SHORT loc_1_4CE2
loc_1_4CBA:
        mov    esi, dword ptr [ebp + 20h]
        mov    ecx, dword ptr [ebp + 48h]
        call   sub_1_76B7
        jb     NEAR PTR loc_1_4F1F
        sub    dword ptr [ebp + 48h], ecx
        call   sub_1_57B5
        jb     NEAR PTR loc_1_4D7A
        mov    esi, dword ptr [ebp + 20h]
        cmp    dword ptr [ebp + 48h], 0
        jne    SHORT loc_1_4CBA
loc_1_4CE2:
        mov    ecx, dword ptr [ebp + 4ch]
        jecxz  loc_1_4CF7
        mov    dword ptr [ebp + 44h], ecx
        sub    ebx, ebx
        call   sub_1_762A
        jb     NEAR PTR loc_1_4D7A
loc_1_4CF7:
        mov    esi, dword ptr [ebp + 20h]
        test   byte ptr [ebp + 37h], 10h
        je     SHORT loc_1_4D0B
        test   byte ptr [ebp + 3ah], 1
        je     SHORT loc_1_4D4D
        jmp    NEAR PTR loc_1_4F3C
loc_1_4D0B:
        test   byte ptr [ebp + 37h], 1
        jne    SHORT loc_1_4D4D
        test   word ptr [ebp + 38h], 4
        je     SHORT loc_1_4D4D
        push   esi
        push   edi
        call   sub_1_0636
        pop    edi
        pop    esi
        jae    SHORT loc_1_4D29
        mov    byte ptr [ebp + 34h], al
        jmp    SHORT loc_1_4D7A
loc_1_4D29:
        test   word ptr [esi + 0eh], 1
        je     SHORT loc_1_4D4D
        push   dword ptr [ebp + 38h]
        or     word ptr [ebp + 38h], 800h
        call   sub_1_1226
        pop    esi
        mov    word ptr [ebp + 38h], si
        jb     NEAR PTR loc_1_4F21
        mov    esi, dword ptr [ebp + 20h]
loc_1_4D4D:
        mov    edi, dword ptr [esi + 8]
        call   sub_1_A01A
        mov    ecx, dword ptr [ebp]
        pushfd
        db  001h, 00Dh, 064h, 0B7h, 000h, 000h
        popfd
        add    ecx, dword ptr [ebp + 3ch]
        mov    dword ptr [ebp + 24h], ecx
        mov    dword ptr [ebp + 3ch], ecx
        call   sub_1_9EB2
        jmp    NEAR PTR loc_1_4574
loc_1_4D73:
        mov    al, 8
        jmp    NEAR PTR loc_1_4F21
loc_1_4D7A:
        mov    esi, dword ptr [ebp + 20h]
        test   byte ptr [ebp + 3ah], 1
        je     NEAR PTR loc_1_4F1F
        mov    eax, dword ptr [edi + 14h]
        test   dword ptr [eax + 4], 80h
        je     SHORT loc_1_4D9B
        test   word ptr [edi + 52h], 10h
        jne    SHORT loc_1_4DA5
loc_1_4D9B:
        cmp    byte ptr [ebp + 34h], 2bh
        jne    NEAR PTR loc_1_4F1F
loc_1_4DA5:
        xor    eax, eax
        mov    dword ptr [ebp], eax
        mov    byte ptr [ebp + 34h], al
        xchg   dword ptr [ebp + 70h], eax
        mov    dword ptr [ebp + 3ch], eax
        jmp    SHORT loc_1_4DB5
loc_1_4DB5:
        mov    edi, dword ptr [esi + 8]
        cmp    dword ptr [edi + 64h], 0
        je     SHORT loc_1_4DDE
        mov    eax, 0
        mov    ecx, dword ptr [edi + 30h]
        mov    edx, dword ptr [ebp + 3ch]
        cmp    ecx, edx
        jbe    SHORT loc_1_4DDE
        push   edi
        mov    edi, esi
        call   sub_1_9D98
        pop    edi
        mov    al, 21h
        jb     NEAR PTR loc_1_4F21
loc_1_4DDE:
        and    word ptr [esi + 0eh], 0fdffh
        mov    edx, dword ptr [esi + 8]
        mov    edi, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [ebp + 3ch]
        sub    eax, 1
        jb     NEAR PTR loc_1_4EC8
        mov    dword ptr [ebp + 3ch], eax
        mov    cl, byte ptr [edi + 47h]
        shr    eax, cl
        inc    eax
        cmp    dword ptr [edx + 50h], eax
        jbe    SHORT loc_1_4E1C
        mov    ecx, edx
        mov    edx, eax
        lea    ebx, [eax - 1]
        call   sub_1_2276
        mov    dword ptr [ecx + 5ch], eax
        sub    ebx, dword ptr [eax + 4]
        inc    ebx
        mov    dword ptr [eax], ebx
        mov    dword ptr [ecx + 50h], edx
loc_1_4E1C:
        mov    esi, dword ptr [ebp + 20h]
        call   sub_1_774E
        jb     SHORT loc_1_4E3A
        mov    esi, dword ptr [ebp + 20h]
        call   sub_1_7F50
        jb     SHORT loc_1_4E3A
        mov    edx, 0ffffffffh
        call   sub_1_7D52
loc_1_4E3A:
        mov    al, 5
        jb     NEAR PTR loc_1_4F21
        sub    eax, eax
        mov    esi, dword ptr [ebp + 20h]
        test   byte ptr [ebp + 37h], 10h
        je     SHORT loc_1_4E52
        mov    ebx, dword ptr [ebp + 24h]
        jmp    SHORT loc_1_4E56
loc_1_4E52:
        mov    ebx, dword ptr [ebp + 3ch]
        inc    ebx
loc_1_4E56:
        mov    esi, dword ptr [esi + 8]
        mov    dword ptr [esi + 30h], ebx
        test   byte ptr [ebp + 37h], 10h
        je     SHORT loc_1_4EC6
        db  083h, 03Dh, 0BCh, 0B6h, 000h, 000h, 0FFh
        je     SHORT loc_1_4EA4
        db  039h, 01Dh, 0BCh, 0B6h, 000h, 000h
        jb     SHORT loc_1_4EA4
        db  0C7h, 005h, 0BCh, 0B6h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  083h, 03Dh, 0B8h, 0B6h, 000h, 000h, 002h
        jne    SHORT loc_1_4EA4
        pushad
        db  08Bh, 035h, 0C0h, 0B6h, 000h, 000h
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        db  0CDh, 020h, 005h, 000h, 08Bh, 004h
        db  0C7h, 005h, 0B8h, 0B6h, 000h, 000h, 000h, 000h, 000h, 000h
        popad
loc_1_4EA4:
        mov    esi, dword ptr [ebp + 20h]
        push   esi
        and    word ptr [esi + 0eh], 0fffbh
        mov    byte ptr [ebp + 36h], 41h
        call   sub_1_1226
        pop    esi
        jb     SHORT loc_1_4F21
        mov    esi, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 28h]
        call   sub_1_05FA
        sub    eax, eax
loc_1_4EC6:
        jmp    SHORT loc_1_4F21
loc_1_4EC8:
        mov    esi, dword ptr [esi + 8]
        mov    ebx, dword ptr [esi + 10h]
        and    dword ptr [esi + 50h], 0
        mov    eax, dword ptr [esi + 54h]
        sub    eax, 8
        mov    dword ptr [esi + 5ch], eax
        push   esi
        call   sub_1_7D50
        mov    al, 5
        jb     SHORT loc_1_4EE7
        sub    eax, eax
loc_1_4EE7:
        pop    esi
        sub    ebx, ebx
        mov    dword ptr [esi + 10h], ebx
        mov    dword ptr [esi + 30h], ebx
        test   byte ptr [ebp + 37h], 10h
        je     SHORT loc_1_4F21
        or     eax, eax
        jne    SHORT loc_1_4F21
        mov    byte ptr [ebp + 36h], 41h
        mov    esi, dword ptr [ebp + 20h]
        and    word ptr [esi + 0eh], 0fffbh
        call   sub_1_1226
        jb     SHORT loc_1_4F21
        mov    esi, dword ptr [ebp + 20h]
        mov    esi, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 28h]
        call   sub_1_05FA
        sub    eax, eax
        jmp    SHORT loc_1_4F21
loc_1_4F1F:
        mov    al, 5
loc_1_4F21:
        and    dword ptr [ebp], 0
        mov    esi, dword ptr [ebp + 20h]
        mov    edi, dword ptr [esi + 8]
        push   eax
        call   sub_1_A01A
        call   sub_1_9EB2
        pop    eax
        jmp    NEAR PTR loc_1_4576
loc_1_4F3C:
        mov    esi, dword ptr [ebp + 20h]
        and    word ptr [esi + 0eh], 0fffbh
        mov    byte ptr [ebp + 36h], 41h
        push   esi
        call   sub_1_1226
        pop    esi
        jb     SHORT loc_1_4F74
        sub    eax, eax
loc_1_4F53:
        push   eax
        mov    edi, dword ptr [esi + 8]
        mov    eax, dword ptr [edi + 28h]
        push   esi
        call   sub_1_05FA
        pop    esi
        mov    edi, dword ptr [esi + 8]
        call   sub_1_A01A
        call   sub_1_9EB2
        pop    eax
        jmp    NEAR PTR loc_1_4576
loc_1_4F74:
        and    dword ptr [ebp], 0
        jmp    SHORT loc_1_4F53
loc_1_4F7A:
        and    dword ptr [ebp], 0
        mov    eax, 5
        jmp    NEAR PTR loc_1_4639
loc_1_4F88:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cld
        mov    ax, word ptr [ebp + 18h]
        and    ah, 91h
        mov    byte ptr [ebp + 37h], ah
        sub    eax, eax
        mov    word ptr [ebp + 38h], 800h
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        call   sub_1_A318
        mov    esi, dword ptr [ebp + 20h]
        movzx  eax, word ptr [esi + 0eh]
        and    eax, 20ch
        cmp    eax, 20ch
        sete   al
        dec    al
        movsx  eax, al
        and    eax, 8000h
        or     word ptr [ebp + 38h], ax
        test   word ptr [ebp + 18h], 1
        jne    SHORT loc_1_4FF3
        test   word ptr [esi + 0eh], 81h
        je     SHORT loc_1_4FE7
        call   sub_1_5CB0
        jmp    SHORT loc_1_4FF3
loc_1_4FE7:
        call   sub_1_5B85
        jb     SHORT loc_1_4FF3
        call   sub_1_5CFF
loc_1_4FF3:
        test   byte ptr [esi + 0eh], 10h
        jne    NEAR PTR loc_1_5107
        mov    edx, dword ptr [esi + 8]
        mov    eax, dword ptr [edx + 28h]
        cmp    dword ptr [ebp + 1ch], eax
        test   byte ptr [edx + 23h], 81h
        je     SHORT loc_1_5013
        mov    edi, edx
        call   sub_1_A02F
loc_1_5013:
        inc    dword ptr [edx + 20h]
        test   word ptr [ebp + 18h], 1
        jne    SHORT loc_1_5042
        call   sub_1_1226
        jb     SHORT loc_1_5044
        mov    eax, dword ptr [ebp + 1ch]
        test   word ptr [eax + 52h], 400h
        je     SHORT loc_1_5042
        call   sub_1_05FA
        mov    ebx, dword ptr [ebp + 1ch]
        mov    eax, 27h
        call   sub_1_44AF
loc_1_5042:
        xor    eax, eax
loc_1_5044:
        mov    ebx, dword ptr [ebp + 20h]
        mov    edi, dword ptr [ebx + 8]
        push   eax
        cmp    byte ptr [ebp + 4], 0
        je     SHORT loc_1_5078
        test   word ptr [ebp + 18h], 2
        jne    SHORT loc_1_5078
        lea    eax, [edi + 64h]
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_5078
        sub    edx, edx
        cmp    byte ptr [ebp + 4], 2
        je     SHORT loc_1_506C
        mov    edx, dword ptr [ebp + 8]
loc_1_506C:
        push   ebx
        push   edx
        push   eax
        db  0CDh, 020h, 055h, 000h, 040h, 000h
        add    esp, 0ch
loc_1_5078:
        dec    dword ptr [edi + 20h]
        jns    SHORT loc_1_5082
        call   sub_1_A046
loc_1_5082:
        cmp    byte ptr [ebp + 4], 2
        jne    SHORT loc_1_5101
        mov    ax, word ptr [ebx + 0ch]
        and    eax, 7
        db  08Bh, 004h, 085h, 0D0h, 0B7h, 000h, 000h
        sub    dword ptr [edi + 2ch], eax
        je     SHORT loc_1_50B8
        mov    ebx, dword ptr [ebp + 20h]
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx], eax
        test   word ptr [ebp + 18h], 2
        je     SHORT loc_1_50DB
        mov    eax, dword ptr [edi + 64h]
        mov    dword ptr [ebp + 24h], eax
        jmp    SHORT loc_1_50DB
loc_1_50B8:
        test   word ptr [ebp + 18h], 2
        je     SHORT loc_1_50D4
        mov    eax, dword ptr [edi + 64h]
        mov    dword ptr [ebp + 24h], eax
        or     eax, eax
        je     SHORT loc_1_50D4
        push   eax
        db  0CDh, 020h, 059h, 000h, 040h, 000h
        add    esp, 4
loc_1_50D4:
        xor    ebx, ebx
        call   sub_1_9F5C
loc_1_50DB:
        mov    ebx, dword ptr [ebp + 20h]
        db  03Bh, 01Dh, 058h, 0B7h, 000h, 000h
        jne    SHORT loc_1_50FA
        db  0C7h, 005h, 058h, 0B7h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0BCh, 0B6h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
loc_1_50FA:
        xchg   ebx, eax
        call   sub_1_A1F8
        xchg   ebx, eax
loc_1_5101:
        pop    eax
        jmp    NEAR PTR loc_1_4576
loc_1_5107:
        push   0
        cmp    byte ptr [ebp + 4], 2
        jne    SHORT loc_1_5101
        jmp    SHORT loc_1_50DB
loc_1_5111:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cld
        sub    eax, eax
        mov    byte ptr [ebp + 37h], al
        mov    word ptr [ebp + 38h], 800h
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        call   sub_1_A318
        mov    esi, dword ptr [ebp + 20h]
        test   byte ptr [esi + 0eh], 10h
        jne    NEAR PTR loc_1_456D
        test   word ptr [ebp + 18h], 4
        jne    SHORT loc_1_5168
        call   sub_1_5CB0
        mov    esi, dword ptr [ebp + 20h]
        mov    edx, dword ptr [esi + 8]
        mov    eax, dword ptr [edx + 28h]
        call   sub_1_1226
        jb     NEAR PTR loc_1_4576
        call   sub_1_A060
        jmp    NEAR PTR loc_1_4574
loc_1_5168:
        call   sub_1_5B85
        jb     SHORT loc_1_5174
        call   sub_1_5CFF
loc_1_5174:
        mov    edx, dword ptr [esi + 8]
        mov    eax, dword ptr [edx + 28h]
        call   sub_1_1226
        jb     NEAR PTR loc_1_4576
        mov    eax, dword ptr [ebp + 1ch]
        call   sub_1_05C6
        jmp    NEAR PTR loc_1_4574
loc_1_5192:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cld
        mov    ax, word ptr [ebp + 18h]
        and    ah, 91h
        mov    byte ptr [ebp + 37h], ah
        sub    eax, eax
        mov    word ptr [ebp + 38h], ax
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        call   sub_1_A318
        mov    edx, dword ptr [ebp + 20h]
        test   byte ptr [edx + 0eh], 10h
        jne    NEAR PTR loc_1_456D
        mov    ecx, dword ptr [ebp + 24h]
        cmp    byte ptr [ebp + 4], 0
        je     SHORT loc_1_51EB
        cmp    byte ptr [ebp + 4], 2
        mov    al, 1
        jne    NEAR PTR loc_1_4576
        mov    esi, dword ptr [edx + 8]
        test   byte ptr [esi + 23h], 81h
        je     SHORT loc_1_51E8
        mov    edi, esi
        call   sub_1_A02F
loc_1_51E8:
        add    ecx, dword ptr [esi + 30h]
loc_1_51EB:
        mov    dword ptr [ebp + 3ch], ecx
        mov    dword ptr [ebp + 24h], ecx
        jmp    NEAR PTR loc_1_4574
loc_1_51F6:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cmp    byte ptr [ebp + 4], 2
        je     SHORT loc_1_5268
        cmp    byte ptr [ebp + 4], 3
        je     NEAR PTR loc_1_5336
        test   byte ptr [ebp + 18h], 1
        jne    SHORT loc_1_521D
        mov    eax, dword ptr [ebp + 1ch]
        test   byte ptr [eax + 52h], 3
        je     SHORT loc_1_5229
loc_1_521D:
        cmp    byte ptr [ebp + 4], 0
        je     SHORT loc_1_5239
        cmp    byte ptr [ebp + 4], 1
        je     SHORT loc_1_5232
loc_1_5229:
        mov    ax, 1
        jmp    NEAR PTR loc_1_4639
loc_1_5232:
        call   sub_1_569A
        jmp    SHORT loc_1_523E
loc_1_5239:
        call   sub_1_560D
loc_1_523E:
        or     al, al
        je     SHORT loc_1_5247
        call   sub_1_5AEA
loc_1_5247:
        movzx  ecx, byte ptr [ebp + 34h]
        or     ecx, ecx
        je     NEAR PTR loc_1_4579
        mov    eax, ecx
        mov    byte ptr [ebp + 34h], 0
        movzx  ecx, byte ptr [ebp + 4]
        db  0CDh, 020h, 00Eh, 000h, 010h, 000h
        jmp    NEAR PTR loc_1_4579
loc_1_5268:
        cld
        sub    eax, eax
        mov    byte ptr [ebp + 37h], al
        mov    word ptr [ebp + 38h], ax
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        call   sub_1_A318
        mov    ebx, dword ptr [ebp + 1ch]
        mov    edx, dword ptr [ebx + 14h]
        mov    ax, word ptr [ebx + 52h]
        movzx  ecx, byte ptr [ebx + 0ach]
        test   word ptr [ebp + 18h], 5
        jne    SHORT loc_1_52A5
        test   word ptr [ebp + 18h], 0ah
        jne    SHORT loc_1_52C2
        mov    al, 1
        jmp    NEAR PTR loc_1_4576
loc_1_52A5:
        push   eax
        call   sub_1_825B
        pop    eax
        or     ax, 2500h
        test   dword ptr [edx + 4], 80h
        je     SHORT loc_1_531B
        db  066h, 00Bh, 005h, 078h, 0BDh, 000h, 000h
        jmp    SHORT loc_1_531B
loc_1_52C2:
        or     byte ptr [ebx + 1fh], 30h
        and    ax, 5affh
        test   dword ptr [edx + 4], 80h
        je     SHORT loc_1_52DA
        db  066h, 00Bh, 005h, 078h, 0BDh, 000h, 000h
loc_1_52DA:
        db  0F7h, 005h, 06Ch, 0B8h, 000h, 000h, 000h, 000h, 000h, 080h
        jne    SHORT loc_1_52F1
        db  00Fh, 0A3h, 00Dh, 06Ch, 0B8h, 000h, 000h
        jae    SHORT loc_1_5301
        jmp    SHORT loc_1_5305
loc_1_52F1:
        test   dword ptr [edx], 4000h
        je     SHORT loc_1_5305
        test   dword ptr [edx], 10000h
        jne    SHORT loc_1_5305
loc_1_5301:
        or     ax, 400h
loc_1_5305:
        test   dword ptr [edx + 4], 40h
        jne    SHORT loc_1_5317
        db  00Fh, 0A3h, 00Dh, 068h, 0B8h, 000h, 000h
        jae    SHORT loc_1_531B
loc_1_5317:
        or     ax, 100h
loc_1_531B:
        mov    word ptr [ebx + 52h], ax
        sub    eax, eax
        call   sub_1_0979
        sub    eax, eax
        push   eax
        push   eax
        push   eax
        push   eax
        call   sub_1_0AF3
        jmp    NEAR PTR loc_1_4548
loc_1_5336:
        mov    ebx, dword ptr [ebp + 14h]
        cmp    word ptr [ebx + 2], 0
        ja     SHORT loc_1_5354
        mov    edx, dword ptr [ebx + 4]
        cmp    edx, 4
        ja     SHORT loc_1_5354
        mov    word ptr [ebx], 18h
        db  0FFh, 024h, 095h, 060h, 053h, 000h, 000h
loc_1_5354:
        mov    ax, 1
        jmp    NEAR PTR loc_1_4639
        db  02Eh, 08Bh, 0C0h
loc_1_5360:
        db  074h, 053h
        add    byte ptr [eax], al
        int3
        push   ebx
        add    byte ptr [eax], al
        int3
        push   ebx
        add    byte ptr [eax], al
        rcl    dword ptr [ebx], cl
        db  000h, 0D4h
        push   esp
        add    byte ptr [eax], al
loc_1_5374:
        cld
        sub    eax, eax
        mov    byte ptr [ebp + 37h], al
        mov    word ptr [ebp + 38h], ax
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        call   sub_1_A318
        mov    edi, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [ebx + 0ch]
        cmp    eax, -1
        jne    SHORT loc_1_5399
        add    eax, 3
        jmp    SHORT loc_1_53A3
loc_1_5399:
        cmp    eax, 2
        jb     SHORT loc_1_53A6
        cmp    dword ptr [edi + 2ch], eax
        jb     SHORT loc_1_53A6
loc_1_53A3:
        mov    dword ptr [edi + 38h], eax
loc_1_53A6:
        mov    eax, dword ptr [ebx + 8]
        or     eax, eax
        je     NEAR PTR loc_1_4574
        cmp    eax, -1
        je     SHORT loc_1_53BF
        cmp    dword ptr [edi + 2ch], eax
        jbe    NEAR PTR loc_1_4574
loc_1_53BF:
        mov    dword ptr [edi + 40h], eax
        call   sub_1_5BCE
        jmp    NEAR PTR loc_1_4574
loc_1_53CC:
        xor    eax, eax
        jmp    NEAR PTR loc_1_4639
loc_1_53D3:
        cld
        sub    eax, eax
        mov    byte ptr [ebp + 37h], al
        mov    word ptr [ebp + 38h], ax
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        mov    edi, dword ptr [ebp + 1ch]
        mov    ax, 5
        test   byte ptr [edi + 52h], 3
        jne    NEAR PTR loc_1_4639
        call   sub_1_A26B
        movzx  eax, byte ptr [edi + 48h]
        test   byte ptr [edi + 52h], 4
        je     SHORT loc_1_5408
        or     eax, 80h
loc_1_5408:
        mov    dword ptr [ebx + 0ch], eax
        mov    eax, dword ptr [ebx + 8]
        cmp    eax, -1
        je     NEAR PTR loc_1_4574
        test   eax, 0ffffff70h
        jne    NEAR PTR loc_1_54CD
        mov    ah, al
        and    al, 0fh
        cmp    byte ptr [edi + 46h], al
        jbe    NEAR PTR loc_1_54CD
        test   ah, 80h
        jne    SHORT loc_1_543C
        or     al, al
        jne    NEAR PTR loc_1_54CD
loc_1_543C:
        bt     dword ptr [edi + 52h], 2
        sbb    dl, dl
        xor    dl, ah
        cmp    byte ptr [edi + 48h], al
        jne    SHORT loc_1_5455
        test   dl, 80h
        je     NEAR PTR loc_1_4574
        jmp    SHORT loc_1_5480
loc_1_5455:
        mov    dword ptr [edi + 38h], 2
        or     dword ptr [edi + 40h], 0ffffffffh
        push   eax
        push   edx
        call   sub_1_5BCE
        mov    eax, edi
        call   sub_1_05FA
        mov    ebx, dword ptr [ebp + 1ch]
        call   sub_1_A72F
        pop    edx
        pop    eax
        mov    edi, dword ptr [ebp + 1ch]
        test   dl, 80h
        je     SHORT loc_1_5490
loc_1_5480:
        btc    dword ptr [edi + 52h], 2
        jae    SHORT loc_1_5490
        push   eax
        call   sub_1_5574
        pop    eax
        jb     SHORT loc_1_54C6
loc_1_5490:
        mov    byte ptr [edi + 48h], al
        xor    ebx, ebx
        call   sub_1_00D0
        jb     SHORT loc_1_54C6
        cmp    byte ptr [ebx + 42h], 29h
        jne    SHORT loc_1_54C6
        movzx  eax, byte ptr [edi + 48h]
        test   byte ptr [edi + 52h], 4
        je     SHORT loc_1_54B1
        or     eax, 80h
loc_1_54B1:
        and    word ptr [ebx + 28h], 0ff70h
        or     word ptr [ebx + 28h], ax
        call   sub_1_55D2
        jae    NEAR PTR loc_1_4574
loc_1_54C6:
        mov    al, 5
        jmp    NEAR PTR loc_1_4576
loc_1_54CD:
        mov    al, 57h
        jmp    NEAR PTR loc_1_4576
loc_1_54D4:
        cld
        sub    eax, eax
        mov    byte ptr [ebp + 37h], al
        mov    word ptr [ebp + 38h], ax
        mov    byte ptr [ebp + 34h], al
        mov    byte ptr [ebp + 35h], al
        mov    edi, dword ptr [ebp + 1ch]
        mov    ax, 5
        test   byte ptr [edi + 52h], 3
        jne    NEAR PTR loc_1_4639
        call   sub_1_A26B
        mov    eax, dword ptr [edi + 3ch]
        mov    dword ptr [ebx + 0ch], eax
        mov    ebx, dword ptr [ebx + 8]
        cmp    ebx, -1
        je     NEAR PTR loc_1_4574
        mov    al, 0dh
        cmp    dword ptr [edi + 2ch], ebx
        jb     NEAR PTR loc_1_4576
        cmp    ebx, 2
        jb     NEAR PTR loc_1_4576
        push   ebx
        call   sub_1_1F2C
        pop    eax
        jbe    NEAR PTR loc_1_4576
        xchg   dword ptr [edi + 3ch], eax
        mov    ebx, edi
        push   eax
        call   sub_1_0979
        pop    eax
        xor    edx, edx
        push   edx
        push   edx
        push   edx
        push   eax
        call   sub_1_0AF3
        xor    ebx, ebx
        call   sub_1_00D0
        jb     SHORT loc_1_556D
        cmp    byte ptr [ebx + 42h], 29h
        jne    SHORT loc_1_556D
        mov    eax, dword ptr [edi + 3ch]
        cmp    dword ptr [ebx + 2ch], eax
        je     NEAR PTR loc_1_4574
        mov    dword ptr [ebx + 2ch], eax
        call   sub_1_55D2
        jae    NEAR PTR loc_1_4574
loc_1_556D:
        mov    al, 5
        jmp    NEAR PTR loc_1_4576
_LTEXT ends
        end
