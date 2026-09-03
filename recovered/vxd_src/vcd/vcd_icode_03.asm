; vcd_icode_03.asm -- VCD object 3 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

        db  050h, 052h, 0FCh, 043h, 08Bh, 0C3h, 083h, 0F8h, 009h, 076h, 009h, 0B2h, 00Ah, 0F6h, 0F2h, 004h
        db  030h, 0AAh, 08Ah, 0C4h, 004h, 030h, 0AAh, 04Bh, 05Ah, 058h, 0C3h
sub_3_001B:
        pushad
        mov    esi, edx
        cld
loc_3_001F:
        lodsb
        stosb
        or     al, al
        jne    SHORT loc_3_001F
        popad
        ret
sub_3_0027:
        db  083h, 03Dh, 0D8h, 003h, 000h, 000h, 000h
        jne    NEAR PTR loc_3_014A
        mov    ecx, 18h
        xor    eax, eax
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     NEAR PTR loc_3_014C
        db  089h, 035h, 0D8h, 003h, 000h, 000h
        call   sub_3_0468
        db  0A1h, 0E8h, 008h, 000h, 000h
        db  0A3h, 01Ch, 004h, 000h, 000h
        db  0CDh, 020h, 0BBh, 000h, 001h, 000h
        db  089h, 01Dh, 009h, 000h, 000h, 000h
        test   ebx, 40000h
        je     SHORT loc_3_0077
        db  080h, 00Dh, 008h, 000h, 000h, 000h, 002h
loc_3_0077:
        xor    edi, edi
        test   ebx, 0c0000h
        je     SHORT loc_3_0084
        or     edi, 0ffffffffh
loc_3_0084:
        db  0BEh, 0D8h, 008h, 000h, 000h
        push   esi
        push   0
        db  068h, 0D4h, 008h, 000h, 000h
        push   2
        db  0CDh, 020h, 06Fh, 001h, 001h, 000h
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_3_00D6
        db  0C7h, 005h, 00Dh, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  068h, 00Dh, 000h, 000h, 000h
        db  068h, 0DCh, 008h, 000h, 000h
        db  068h, 0D3h, 000h, 000h, 000h
        push   0
        db  068h, 01Ch, 009h, 000h, 000h
        push   dword ptr [esi]
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_3_00D6
        db  00Fh, 0B6h, 03Dh, 0DCh, 008h, 000h, 000h
loc_3_00D6:
        or     edi, edi
        je     SHORT loc_3_0102
        db  0C7h, 005h, 010h, 004h, 000h, 000h, 07Bh, 000h, 000h, 000h
        db  0C7h, 005h, 014h, 004h, 000h, 000h, 018h, 002h, 000h, 000h
        db  0C7h, 005h, 018h, 004h, 000h, 000h, 0D8h, 002h, 000h, 000h
        db  0A1h, 0E4h, 008h, 000h, 000h
        db  0A3h, 01Ch, 004h, 000h, 000h
loc_3_0102:
        mov    eax, 2
        cmp    dword ptr [esi], 0
        je     SHORT loc_3_0145
        db  0C7h, 005h, 00Dh, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        db  068h, 00Dh, 000h, 000h, 000h
        db  068h, 0DDh, 008h, 000h, 000h
        db  068h, 0D3h, 000h, 000h, 000h
        push   0
        db  068h, 0ECh, 008h, 000h, 000h
        push   dword ptr [esi]
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        test   eax, eax
        mov    eax, 2
        jne    SHORT loc_3_0145
        db  0A1h, 0DDh, 008h, 000h, 000h
loc_3_0145:
        db  0A3h, 0F0h, 003h, 000h, 000h
loc_3_014A:
        clc
        ret
loc_3_014C:
        stc
        ret
loc_3_014E:
        db  080h, 00Dh, 008h, 000h, 000h, 000h, 001h
        db  08Bh, 035h, 0D8h, 003h, 000h, 000h
        test   esi, esi
        je     NEAR PTR loc_3_0281
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
loc_3_0169:
        je     SHORT loc_3_01BE
        db  0FFh, 005h, 030h, 000h, 000h, 000h
        mov    edi, eax
        cmp    word ptr [edi], 0
        je     SHORT loc_3_019D
        cmp    dword ptr [edi + 0ch], 0
        jne    SHORT loc_3_019D
        pushad
        movzx  eax, word ptr [edi]
        movzx  ebx, byte ptr [edi + 2]
        mov    ecx, dword ptr [edi + 10h]
        call   sub_3_02B8
        jae    SHORT loc_3_019C
        db  0FFh, 00Dh, 030h, 000h, 000h, 000h
        mov    word ptr [edi], 0
loc_3_019C:
        popad
loc_3_019D:
        mov    eax, edi
        mov    ecx, dword ptr [edi + 10h]
        db  03Bh, 00Dh, 034h, 000h, 000h, 000h
        jb     SHORT loc_3_01B0
        db  089h, 00Dh, 034h, 000h, 000h, 000h
loc_3_01B0:
        db  08Bh, 035h, 0D8h, 003h, 000h, 000h
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_3_0169
loc_3_01BE:
        db  0A1h, 034h, 000h, 000h, 000h
        db  03Bh, 005h, 030h, 000h, 000h, 000h
        jae    SHORT loc_3_01D5
        db  0A1h, 030h, 000h, 000h, 000h
        db  0A3h, 034h, 000h, 000h, 000h
loc_3_01D5:
        shl    eax, 2
        push   1
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_01EE
        db  0CDh, 020h, 0BFh, 080h, 001h, 000h
loc_3_01EE:
        db  0A3h, 0DCh, 003h, 000h, 000h
        mov    edi, eax
        db  08Bh, 035h, 0D8h, 003h, 000h, 000h
loc_3_01FB:
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_3_0227
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        cmp    word ptr [eax], 0
        je     SHORT loc_3_021F
        mov    ecx, dword ptr [eax + 10h]
        mov    ebx, dword ptr [eax + 0ch]
        mov    edx, dword ptr [eax + 14h]
        mov    dword ptr [ebx + 64h], edx
        mov    dword ptr [edi + ecx*4 - 4], ebx
loc_3_021F:
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        jmp    SHORT loc_3_01FB
loc_3_0227:
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
        xor    eax, eax
        db  0A3h, 0D8h, 003h, 000h, 000h
        mov    al, 14h
        db  0BEh, 01Ah, 00Bh, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        push   0
        db  068h, 07Ch, 008h, 000h, 000h
        db  0CDh, 020h, 0A6h, 000h, 001h, 000h
        add    esp, 8
        push   0
        db  068h, 090h, 008h, 000h, 000h
        db  0CDh, 020h, 0A6h, 000h, 001h, 000h
        add    esp, 8
        db  0CDh, 020h, 000h, 000h, 018h, 000h
        or     eax, eax
        je     SHORT loc_3_0275
        db  0C7h, 005h, 0D0h, 003h, 000h, 000h, 000h, 000h, 000h, 000h
loc_3_0275:
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0E8h, 07Ch, 003h, 0FDh, 0FFh
        clc
loc_3_0281:
        ret
loc_3_0282:
        db  083h, 00Dh, 020h, 004h, 000h, 000h, 001h
        db  0BEh, 0A4h, 008h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 015h, 000h
        db  08Bh, 00Dh, 034h, 000h, 000h, 000h
        jecxz  loc_3_02B6
        db  08Bh, 03Dh, 0DCh, 003h, 000h, 000h
loc_3_02A2:
        mov    esi, dword ptr [edi]
        or     esi, esi
        je     SHORT loc_3_02B1
        lea    esi, [esi + 10h]
        db  0CDh, 020h, 007h, 000h, 015h, 000h
loc_3_02B1:
        add    edi, 4
        loop   loc_3_02A2
loc_3_02B6:
        clc
        ret
sub_3_02B8:
        pushad
        mov    eax, 90h
        add    eax, dword ptr [edi + 4]
        push   1
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_02D7
        popad
        jmp    NEAR PTR loc_3_03CF
loc_3_02D7:
        mov    esi, eax
        add    esi, 1ch
        mov    dword ptr [edi + 0ch], esi
        push   edi
        push   esi
        mov    edi, eax
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ecx, 7
        cld
        rep movsd
        pop    esi
        db  0BFh, 0DEh, 001h, 000h, 000h
        sub    edi, esi
        mov    dword ptr [esi - 4], edi
        db  0BFh, 059h, 000h, 000h, 000h
        sub    edi, eax
        mov    dword ptr [esi - 1bh], edi
        pop    edi
        mov    eax, 18h
        add    eax, dword ptr [edi + 8]
        push   0
        push   eax
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_0327
        popad
        jmp    NEAR PTR loc_3_03CF
loc_3_0327:
        mov    dword ptr [esi], eax
        mov    dword ptr [esp + 4], esi
        popad
        mov    byte ptr [esi + 4], cl
        mov    dword ptr [esi + 18h], eax
        mov    byte ptr [esi + 5], bl
        push   eax
        db  0E8h, 0DFh, 0FCh, 0FCh, 0FFh
        pop    eax
        push   esi
        lea    esi, [esi - 1ch]
        push   ecx
        mov    ecx, 7
        mov    edx, eax
loc_3_034A:
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_3_0355
        inc    edx
        loop   loc_3_034A
loc_3_0355:
        pop    ecx
        pop    esi
        jb     SHORT loc_3_03CE
        cmp    bl, 0ffh
        jne    SHORT loc_3_0362
        or     byte ptr [esi + 6], 4
loc_3_0362:
        push   edi
        db  0A1h, 03Fh, 009h, 000h, 000h
        mov    dword ptr [esi + 10h], eax
        db  0A1h, 043h, 009h, 000h, 000h
        mov    dword ptr [esi + 14h], eax
        db  00Fh, 0B6h, 03Dh, 047h, 009h, 000h, 000h
        lea    edi, [esi + edi + 10h]
        dec    ecx
        mov    ebx, ecx
        call   0
        push   ecx
        push   esi
        db  0BFh, 00Fh, 009h, 000h, 000h
        call   0
        db  0BAh, 001h, 009h, 000h, 000h
        call   sub_3_001B
        db  0BFh, 00Ch, 009h, 000h, 000h
        mov    eax, 0fffffffeh
        mov    ecx, 3
        xor    esi, esi
        db  0CDh, 020h, 0ADh, 000h, 001h, 000h
        pop    esi
        pop    ecx
        cmp    eax, 0fffffc18h
        jne    SHORT loc_3_03BF
        or     eax, 0ffffffffh
loc_3_03BF:
        mov    dword ptr [esi + 0ch], eax
        xor    eax, eax
        mov    dword ptr [esi + 8], eax
        pop    edi
        db  0E8h, 073h, 001h, 0FDh, 0FFh
        clc
loc_3_03CE:
        ret
loc_3_03CF:
        stc
        ret
VCD_Virtualize_Port:
        pushad
        db  0F6h, 005h, 020h, 004h, 000h, 000h, 001h
        jne    NEAR PTR loc_3_0460
        db  08Bh, 035h, 0D8h, 003h, 000h, 000h
        test   esi, esi
        je     SHORT loc_3_0460
        mov    ecx, eax
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
loc_3_03F1:
        je     SHORT loc_3_0460
        cmp    dword ptr [eax + 10h], ecx
        je     SHORT loc_3_0400
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jmp    SHORT loc_3_03F1
loc_3_0400:
        mov    esi, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 18h]
        cmp    dword ptr [esi + 4], 0
        je     SHORT loc_3_0460
        cmp    byte ptr [eax + 3], 0
        jne    SHORT loc_3_0458
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax + 8], edx
        mov    edi, eax
        mov    ecx, dword ptr [esp + 1ch]
        movzx  ebx, byte ptr [edi + 2]
        movzx  eax, word ptr [edi]
        call   sub_3_02B8
        jae    SHORT loc_3_0435
        mov    word ptr [edi], 0
        jb     SHORT loc_3_0460
loc_3_0435:
        inc    byte ptr [edi + 3]
        mov    edi, dword ptr [edi + 0ch]
        test   byte ptr [esp + 10h], 1
        jne    SHORT loc_3_0446
        or     byte ptr [edi + 6], 2
loc_3_0446:
        mov    esi, dword ptr [esp + 4]
        mov    dword ptr [edi + 3ch], esi
        or     byte ptr [edi + 6], 80h
        mov    dword ptr [esp + 1ch], edi
        popad
        clc
        ret
loc_3_0458:
        popad
        mov    eax, 1
        jmp    SHORT loc_3_0463
loc_3_0460:
        popad
        xor    eax, eax
loc_3_0463:
        stc
        ret
        db  000h, 000h, 000h
sub_3_0468:
        xor    eax, eax
        xor    esi, esi
        db  0BFh, 02Ah, 009h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        or     eax, eax
        je     SHORT loc_3_04DF
        db  0B8h, 07Ah, 008h, 000h, 000h
        db  02Dh, 0F4h, 004h, 000h, 000h
        add    eax, 3
        and    al, 0fch
        push   eax
        push   0
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        pop    ecx
        or     eax, eax
        je     SHORT loc_3_04DF
        shr    ecx, 2
        mov    edi, eax
        db  0BEh, 0F4h, 004h, 000h, 000h
        cld
        rep movsd
        db  02Dh, 0F4h, 004h, 000h, 000h
        db  001h, 005h, 02Ch, 004h, 000h, 000h
        db  001h, 005h, 030h, 004h, 000h, 000h
        db  001h, 005h, 0E4h, 008h, 000h, 000h
        db  001h, 005h, 0E8h, 008h, 000h, 000h
        db  001h, 005h, 034h, 004h, 000h, 000h
        db  001h, 005h, 038h, 004h, 000h, 000h
        db  001h, 005h, 03Ch, 004h, 000h, 000h
        db  001h, 005h, 040h, 004h, 000h, 000h
        ret
loc_3_04DF:
        xor    eax, eax
        db  0A3h, 02Ch, 004h, 000h, 000h
        db  0A3h, 0E4h, 008h, 000h, 000h
        db  0A3h, 0E8h, 008h, 000h, 000h
        ret
        db  02Eh, 08Bh, 0C0h
loc_3_04F4:
        xchg   edi, eax
        mov    ax, 3c04h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        mov    dword ptr [esi + 28h], edi
        movzx  eax, word ptr [edi + 28h]
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        movzx  ecx, word ptr [edi + 26h]
        add    eax, ecx
        mov    dword ptr [esi + 2ch], eax
        movzx  eax, word ptr [edi + 2eh]
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        movzx  ecx, word ptr [edi + 2ch]
        add    eax, ecx
        mov    dword ptr [esi + 30h], eax
        mov    dword ptr [edi + 4eh], esi
        ret
loc_3_053B:
        xchg   edi, eax
        mov    ax, 3c04h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        mov    esi, dword ptr [edi + 4eh]
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        cmp    dword ptr [esi + 1ch], ebx
        je     SHORT loc_3_0565
        db  0FFh, 015h, 028h, 004h, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_3_0565
        or     byte ptr [edi + 22h], 4
        jmp    SHORT loc_3_057C
loc_3_0565:
        mov    edx, dword ptr [esi + 18h]
        add    dl, 5
        in     al, dx
        test   al, 20h
        je     SHORT loc_3_057C
        pushfd
        cli
        call   sub_3_0707
        mov    byte ptr [edi + 4ah], 0
        popfd
loc_3_057C:
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        ret
sub_3_0583:
        mov    edx, dword ptr [esi + 18h]
        add    dl, 5
        in     al, dx
        and    al, 20h
        je     SHORT loc_3_05A1
        sub    dl, 4
        in     al, dx
        test   al, 2
        jne    SHORT loc_3_059C
        or     al, 2
        out    dx, al
        jmp    SHORT loc_3_059B
loc_3_059B:
        out    dx, al
loc_3_059C:
        jmp    NEAR PTR sub_3_0707
loc_3_05A1:
        ret
        db  08Bh, 0C0h
loc_3_05A4:
        mov    al, byte ptr [esi + 3bh]
        jmp    SHORT loc_3_05B3
        db  02Eh, 08Bh, 0C0h
loc_3_05AC:
        mov    edx, dword ptr [esi + 18h]
        add    dl, 2
        in     al, dx
loc_3_05B3:
        mov    edi, dword ptr [esi + 28h]
        test   al, 1
        jne    SHORT loc_3_05C7
        movzx  eax, al
        shl    eax, 1
        db  0FFh, 090h, 034h, 004h, 000h, 000h
        jmp    SHORT loc_3_05AC
loc_3_05C7:
        mov    ax, word ptr [edi + 24h]
        and    word ptr [edi + 22h], ax
        ret
loc_3_05D0:
        or     byte ptr [edi + 22h], 80h
        mov    edx, dword ptr [esi + 18h]
        add    dl, 5
        in     al, dx
        test   al, 0eh
        je     SHORT loc_3_05E2
        mov    byte ptr [edi + 4bh], al
loc_3_05E2:
        test   al, 10h
        je     SHORT loc_3_05EA
        or     byte ptr [edi + 22h], 40h
loc_3_05EA:
        ret
loc_3_05EB:
        mov    edx, dword ptr [esi + 18h]
        add    dl, 0
        in     al, dx
        mov    ah, byte ptr [edi + 4bh]
        mov    bh, ah
        or     ah, ah
        je     SHORT loc_3_0605
        and    ah, byte ptr [edi + 3fh]
        or     byte ptr [edi + 1ah], ah
        mov    byte ptr [edi + 4bh], 0
loc_3_0605:
        or     byte ptr [edi + 22h], 1
        movzx  ecx, word ptr [edi + 32h]
        cmp    cx, word ptr [edi + 2ah]
        jge    SHORT loc_3_068D
        test   byte ptr [edi + 3eh], 20h
        jne    SHORT loc_3_068D
        test   bh, 4
        jne    SHORT loc_3_0674
loc_3_061E:
        and    al, byte ptr [edi + 40h]
        je     SHORT loc_3_067F
loc_3_0623:
        test   byte ptr [edi + 0ch], 1
        jne    SHORT loc_3_062E
        cmp    al, byte ptr [edi + 15h]
        je     SHORT loc_3_0687
loc_3_062E:
        test   byte ptr [edi + 0dh], 1
        je     SHORT loc_3_0646
        cmp    al, byte ptr [edi + 0fh]
        je     NEAR PTR loc_3_06D1
        cmp    al, byte ptr [edi + 0eh]
        je     NEAR PTR loc_3_06EF
loc_3_0646:
        cmp    al, byte ptr [edi + 16h]
        je     SHORT loc_3_06C8
loc_3_064B:
        test   byte ptr [edi + 42h], 80h
        jne    SHORT loc_3_0673
        push   esi
        mov    esi, dword ptr [esi + 2ch]
        movzx  ebx, word ptr [edi + 36h]
        mov    byte ptr [esi + ebx], al
        pop    esi
        inc    ebx
        cmp    bx, word ptr [edi + 2ah]
        jae    SHORT loc_3_0693
loc_3_0664:
        mov    word ptr [edi + 36h], bx
        inc    ecx
        mov    word ptr [edi + 32h], cx
        cmp    cx, word ptr [edi + 20h]
        jae    SHORT loc_3_0697
loc_3_0673:
        ret
loc_3_0674:
        test   byte ptr [edi + 0dh], 4
        je     SHORT loc_3_061E
        mov    al, byte ptr [edi + 14h]
        jmp    SHORT loc_3_0646
loc_3_067F:
        test   byte ptr [edi + 0dh], 8
        jne    SHORT loc_3_0673
        jmp    SHORT loc_3_0623
loc_3_0687:
        or     byte ptr [edi + 3eh], 20h
        jmp    SHORT loc_3_0673
loc_3_068D:
        or     byte ptr [edi + 1ah], 1
        jmp    SHORT loc_3_0673
loc_3_0693:
        xor    ebx, ebx
        jmp    SHORT loc_3_0664
loc_3_0697:
        mov    cl, byte ptr [edi + 42h]
        test   cl, 13h
        jne    SHORT loc_3_0673
        mov    ah, byte ptr [edi + 43h]
        or     ah, ah
        je     SHORT loc_3_06B5
        add    dl, 4
        in     al, dx
        not    ah
        and    al, ah
        or     cl, 2
        mov    byte ptr [edi + 42h], cl
        out    dx, al
loc_3_06B5:
        test   byte ptr [edi + 0dh], 2
        je     SHORT loc_3_0673
        or     cl, 1
        mov    byte ptr [edi + 42h], cl
        call   sub_3_0583
        jmp    SHORT loc_3_0673
loc_3_06C8:
        or     byte ptr [edi + 22h], 2
        jmp    NEAR PTR loc_3_064B
loc_3_06D1:
        or     byte ptr [edi + 42h], 8
        test   byte ptr [edi + 0ch], 60h
        je     SHORT loc_3_0673
        movzx  ecx, word ptr [edi + 32h]
        cmp    cx, word ptr [edi + 10h]
        ja     SHORT loc_3_0673
        and    byte ptr [edi + 42h], 0f7h
        or     byte ptr [edi + 42h], 4
        jmp    SHORT loc_3_06FD
loc_3_06EF:
        and    byte ptr [edi + 42h], 0f7h
        test   byte ptr [edi + 0ch], 60h
        je     SHORT loc_3_06FD
        and    byte ptr [edi + 42h], 0f7h
loc_3_06FD:
        call   sub_3_0583
        jmp    NEAR PTR loc_3_0673
sub_3_0707:
        mov    edx, dword ptr [esi + 18h]
        add    dl, 5
        in     al, dx
        test   al, 20h
        je     NEAR PTR loc_3_0805
        sub    dl, 5
        mov    ah, byte ptr [edi + 42h]
        test   ah, 60h
        jne    NEAR PTR loc_3_0809
        test   byte ptr [edi + 0ch], 60h
        jne    SHORT loc_3_0734
        test   ah, 5
        jne    NEAR PTR loc_3_07D3
loc_3_0734:
        test   ah, 7fh
        jne    NEAR PTR loc_3_0809
        test   byte ptr [edi + 3eh], 40h
        jne    NEAR PTR loc_3_07CA
        movzx  ecx, word ptr [edi + 38h]
        jecxz  loc_3_077B
        test   byte ptr [edi + 0ch], 40h
        jne    SHORT loc_3_0780
        test   byte ptr [edi + 0ch], 20h
        jne    SHORT loc_3_07A0
loc_3_0759:
        push   esi
        mov    esi, dword ptr [esi + 30h]
        movzx  ebx, word ptr [edi + 3ah]
        mov    al, byte ptr [esi + ebx]
        pop    esi
        inc    ebx
        cmp    bx, word ptr [edi + 30h]
        jae    SHORT loc_3_07C6
loc_3_076C:
        mov    word ptr [edi + 3ah], bx
        movzx  ecx, word ptr [edi + 38h]
        dec    ecx
        mov    word ptr [edi + 38h], cx
loc_3_0779:
        out    dx, al
        ret
loc_3_077B:
        jmp    NEAR PTR loc_3_0805
loc_3_0780:
        movzx  ecx, word ptr [edi + 4ch]
        cmp    cx, word ptr [edi + 10h]
        jne    SHORT loc_3_0799
        mov    word ptr [edi + 4ch], 0
        or     byte ptr [edi + 42h], 8
        mov    al, byte ptr [edi + 0fh]
        jmp    SHORT loc_3_0779
loc_3_0799:
        inc    ecx
        mov    word ptr [edi + 4ch], cx
        jmp    SHORT loc_3_0759
loc_3_07A0:
        movzx  ecx, word ptr [edi + 4ch]
        or     ecx, ecx
        jne    SHORT loc_3_07B7
        mov    word ptr [edi + 4ch], 1
        or     byte ptr [edi + 42h], 8
        mov    al, byte ptr [edi + 0fh]
        jmp    SHORT loc_3_0779
loc_3_07B7:
        inc    ecx
        cmp    cx, word ptr [edi + 10h]
        jne    SHORT loc_3_07C0
        xor    ecx, ecx
loc_3_07C0:
        mov    word ptr [edi + 4ch], cx
        jmp    SHORT loc_3_0759
loc_3_07C6:
        xor    ebx, ebx
        jmp    SHORT loc_3_076C
loc_3_07CA:
        and    byte ptr [edi + 3eh], 0bfh
        mov    al, byte ptr [edi + 41h]
        jmp    SHORT loc_3_0779
loc_3_07D3:
        test   ah, 1
        je     SHORT loc_3_07E3
        and    ah, 0feh
        or     ah, 10h
        mov    al, byte ptr [edi + 0fh]
        jmp    SHORT loc_3_07E9
loc_3_07E3:
        and    ah, 0ebh
        mov    al, byte ptr [edi + 0eh]
loc_3_07E9:
        mov    byte ptr [edi + 42h], ah
        jmp    SHORT loc_3_0779
        db  0F6h, 0C4h, 004h, 00Fh, 084h, 050h, 0FFh, 0FFh, 0FFh, 080h, 0E4h, 0FBh, 08Ah, 047h, 00Eh, 088h
        db  067h, 042h, 0E9h, 074h, 0FFh, 0FFh, 0FFh
loc_3_0805:
        or     byte ptr [edi + 22h], 4
loc_3_0809:
        inc    dl
        in     al, dx
        and    al, 0fdh
        jmp    NEAR PTR loc_3_0779
loc_3_0813:
        mov    edx, dword ptr [esi + 18h]
        add    dl, 6
        in     al, dx
        mov    byte ptr [edi + 45h], al
        mov    ch, al
        movzx  eax, al
        mov    ah, al
        shr    eax, 2
        shr    ah, 1
        shr    eax, 3
        and    ax, 138h
        or     word ptr [edi + 22h], ax
        mov    ah, ch
        shr    ah, 2
        and    ax, 0c00h
        or     word ptr [edi + 22h], ax
        mov    ah, ch
        shr    ah, 3
        and    ax, 20h
        or     word ptr [edi + 22h], ax
        mov    ah, ch
        shl    ah, 3
        and    ax, 2000h
        or     word ptr [edi + 22h], ax
        mov    al, byte ptr [edi + 44h]
        or     al, al
        jne    SHORT loc_3_0863
loc_3_0862:
        ret
loc_3_0863:
        and    ch, al
        cmp    ch, al
        je     SHORT loc_3_086F
        or     byte ptr [edi + 42h], 20h
        jmp    SHORT loc_3_0862
loc_3_086F:
        and    byte ptr [edi + 42h], 0dfh
        call   sub_3_0583
        jmp    SHORT loc_3_0862
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 008h, 000h
        db  000h, 000h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 07Ch, 004h
        db  000h, 000h, 004h, 000h, 000h, 000h, 000h, 002h, 000h, 000h
loc_3_08A4:
        inc    ecx
        push   ebp
        pop    eax
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 4fh], al
        dec    ebp
        xor    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 43h], dl
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_08DD:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_3_08E4:
        movsb
        db  005h, 000h, 000h
loc_3_08E8:
        lodsb
        db  005h, 000h, 000h
loc_3_08EC:
        inc    ebx
        dec    edi
        dec    ebp
        inc    edx
        dec    edi
        dec    edi
        push   ebx
        push   esp
        push   esp
        dec    ecx
        dec    ebp
        inc    ebp
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [ecx + 52h], cl
        push   ecx
        add    byte ptr [ecx + 55h], al
        push   esp
        dec    edi
        inc    ecx
        push   ebx
        push   ebx
        dec    ecx
        inc    edi
        dec    esi
        add    byte ptr [ebx + 4fh], al
        dec    ebp
loc_3_090F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 4fh], al
        dec    ebp
        dec    ecx
        push   edx
        push   ecx
        push   ebx
        dec    eax
        inc    ecx
        push   edx
        dec    ecx
        dec    esi
        inc    edi
        add    byte ptr [ebx + 4fh], al
        dec    ebp
        dec    ebp
        inc    esp
        push   edx
        push   esi
        xor    esi, dword ptr [eax]
        add    byte ptr [ebp + 41h], cl
        pop    eax
        inc    ebx
        dec    edi
        dec    ebp
        push   eax
        dec    edi
        push   edx
        push   esp
        db  000h
loc_3_093F:
        inc    ebx
        dec    edi
        dec    ebp
        xor    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx], al
_ITEXT ends
        end
