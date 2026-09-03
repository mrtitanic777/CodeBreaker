; vdd_lcode_01.asm -- VDD object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

VDD_Control:
        cmp    eax, 22h
        jne    SHORT loc_1_0013
        push   edx
        push   ebx
        db  068h, 074h, 006h, 000h, 000h
        db  0E8h, 0A7h, 017h, 0FFh, 0FFh
        stc
        ret
loc_1_0013:
        cmp    eax, 24h
        jae    SHORT loc_1_001F
        db  0FFh, 024h, 085h, 021h, 000h, 000h, 000h
loc_1_001F:
        clc
        ret
        db  000h, 000h, 000h, 000h, 06Fh, 000h, 000h, 000h, 0AEh, 008h, 000h, 000h, 043h, 000h, 000h, 000h
        db  01Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  05Fh, 000h, 000h, 000h, 050h, 000h, 000h, 000h, 01Fh, 000h, 000h, 000h, 01Fh, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0E4h, 000h, 000h, 000h, 00Ah, 001h, 000h, 000h, 01Fh, 000h, 000h, 000h, 01Fh, 000h, 000h, 000h
        db  01Fh, 000h, 000h, 000h, 01Fh, 000h, 000h, 000h, 036h, 000h, 000h, 000h, 01Fh, 000h, 000h, 000h
        db  01Fh, 000h, 000h, 000h, 01Fh, 000h, 000h, 000h, 01Fh, 000h, 000h, 000h, 01Fh, 000h, 000h, 000h
        db  01Fh, 000h, 000h, 000h, 01Fh, 000h, 000h, 000h, 01Fh, 000h, 000h, 000h, 01Fh, 000h, 000h, 000h
        db  01Fh, 000h, 000h, 000h, 01Fh, 000h, 000h, 000h, 01Fh, 000h, 000h, 000h, 0B1h, 000h, 000h, 000h
loc_1_00B1:
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_1_00BA
        mov    eax, dword ptr [eax]
loc_1_00BA:
        push   dword ptr [esi + 1ch]
        push   dword ptr [esi + 18h]
        push   eax
        push   dword ptr [esi + 0ch]
        db  0E8h, 068h, 017h, 0FFh, 0FFh
        clc
        ret
        db  000h
VDD_Do_Physical_IO:
        test   ecx, 20h
        jne    SHORT loc_1_011C
        cmp    ecx, 8
        jne    SHORT loc_1_00E0
        in     ax, dx
        jmp    NEAR PTR loc_1_028B
loc_1_00E0:
        cmp    ecx, 0
        jne    SHORT loc_1_00EB
        in     al, dx
        jmp    NEAR PTR loc_1_028B
loc_1_00EB:
        cmp    ecx, 10h
        jne    SHORT loc_1_00F6
        in     eax, dx
        jmp    NEAR PTR loc_1_028B
loc_1_00F6:
        cmp    ecx, 0ch
        jne    SHORT loc_1_0102
        out    dx, ax
        jmp    NEAR PTR loc_1_028B
loc_1_0102:
        cmp    ecx, 4
        jne    SHORT loc_1_010D
        out    dx, al
        jmp    NEAR PTR loc_1_028B
loc_1_010D:
        cmp    ecx, 14h
        jne    NEAR PTR loc_1_028B
        out    dx, eax
        jmp    NEAR PTR loc_1_028B
loc_1_011C:
        pushfd
        cld
        test   ecx, 80h
        je     NEAR PTR loc_1_01E8
        push   edx
        push   ecx
        shr    ecx, 10h
        push   0
        push   ecx
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        pop    ecx
        mov    edx, dword ptr [ebp + 4]
        test   ecx, 4
        jne    SHORT loc_1_014B
        mov    edx, dword ptr [ebp]
loc_1_014B:
        add    eax, edx
        pop    edx
        mov    esi, eax
        mov    edi, 1
        test   ecx, 40h
        je     SHORT loc_1_0167
        mov    edi, dword ptr [ebp + 18h]
        mov    dword ptr [ebp + 18h], 0
loc_1_0167:
        test   ecx, 100h
        je     SHORT loc_1_0172
        neg    edi
        std
loc_1_0172:
        and    ecx, 1ch
        cmp    ecx, 14h
        je     SHORT loc_1_01CD
        cmp    ecx, 4
        je     SHORT loc_1_01DC
        cmp    ecx, 0ch
        je     SHORT loc_1_01BE
        cmp    ecx, 10h
        je     SHORT loc_1_019F
        cmp    ecx, 0
        je     SHORT loc_1_01B0
        mov    ecx, edi
        xchg   esi, edi
        shl    esi, 1
        add    dword ptr [ebp], esi
        rep insw
        jmp    NEAR PTR loc_1_028A
loc_1_019F:
        mov    ecx, edi
        xchg   esi, edi
        shl    esi, 2
        add    dword ptr [ebp], esi
        rep insd
        jmp    NEAR PTR loc_1_028A
loc_1_01B0:
        mov    ecx, edi
        xchg   esi, edi
        add    dword ptr [ebp], esi
        rep insb
        jmp    NEAR PTR loc_1_028A
loc_1_01BE:
        mov    ecx, edi
        shl    edi, 1
        add    dword ptr [ebp + 4], edi
        rep outsw
        jmp    NEAR PTR loc_1_028A
loc_1_01CD:
        mov    ecx, edi
        shl    edi, 2
        add    dword ptr [ebp + 4], edi
        rep outsd
        jmp    NEAR PTR loc_1_028A
loc_1_01DC:
        mov    ecx, edi
        add    dword ptr [ebp + 4], edi
        rep outsb
        jmp    NEAR PTR loc_1_028A
loc_1_01E8:
        mov    ax, 3c04h
        test   ecx, 4
        jne    SHORT loc_1_01F8
        mov    ax, 3800h
loc_1_01F8:
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    esi, eax
        mov    edi, 1
        test   ecx, 40h
        je     SHORT loc_1_0217
        movzx  edi, word ptr [ebp + 18h]
        mov    word ptr [ebp + 18h], 0
loc_1_0217:
        test   ecx, 100h
        je     SHORT loc_1_0222
        neg    edi
        std
loc_1_0222:
        and    ecx, 1ch
        cmp    ecx, 14h
        je     SHORT loc_1_0275
        cmp    ecx, 4
        je     SHORT loc_1_0282
        cmp    ecx, 0ch
        je     SHORT loc_1_0268
        cmp    ecx, 10h
        je     SHORT loc_1_024D
        cmp    ecx, 0
        je     SHORT loc_1_025C
        mov    ecx, edi
        xchg   esi, edi
        shl    esi, 1
        add    word ptr [ebp], si
        rep insw
        jmp    SHORT loc_1_028A
loc_1_024D:
        mov    ecx, edi
        xchg   esi, edi
        shl    esi, 2
        add    word ptr [ebp], si
        rep insd
        jmp    SHORT loc_1_028A
loc_1_025C:
        mov    ecx, edi
        xchg   esi, edi
        add    word ptr [ebp], si
        rep insb
        jmp    SHORT loc_1_028A
loc_1_0268:
        mov    ecx, edi
        shl    edi, 1
        add    word ptr [ebp + 4], di
        rep outsw
        jmp    SHORT loc_1_028A
loc_1_0275:
        mov    ecx, edi
        shl    edi, 2
        add    word ptr [ebp + 4], di
        rep outsd
        jmp    SHORT loc_1_028A
loc_1_0282:
        mov    ecx, edi
        add    word ptr [ebp + 4], di
        rep outsb
loc_1_028A:
        popfd
loc_1_028B:
        ret
sub_1_028C:
        push   eax
        push   ebx
        test   byte ptr [edi + 1], 20h
        jne    SHORT loc_1_02E1
        db  03Bh, 01Dh, 0B8h, 007h, 000h, 000h
        je     SHORT loc_1_02E4
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jae    SHORT loc_1_02B4
        mov    ebx, edi
        db  02Bh, 01Dh, 0CCh, 006h, 000h, 000h
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_1_02E4
loc_1_02B4:
        mov    eax, dword ptr [ebx + 4]
        mov    al, byte ptr [eax + 449h]
        and    al, 7fh
        cmp    al, 13h
        ja     SHORT loc_1_02E1
        clc
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 8ch], 0
        je     SHORT loc_1_02DD
        popfd
        call   dword ptr [edi + 8ch]
        jmp    SHORT loc_1_02DE
loc_1_02DD:
        popfd
loc_1_02DE:
        pop    edi
        jae    SHORT loc_1_02E4
loc_1_02E1:
        stc
        jmp    SHORT loc_1_02E5
loc_1_02E4:
        clc
loc_1_02E5:
        pop    ebx
        pop    eax
        ret
loc_1_02E8:
        mov    ebx, edx
        mov    eax, 0bb8h
        xor    ecx, ecx
        db  0CDh, 020h, 0ECh, 000h, 001h, 000h
        ret
loc_1_02F8:
        inc    word ptr [ebp + 24h]
        mov    ax, word ptr [ebp + 1ch]
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        cmp    byte ptr [edi + 638h], 0
        jne    SHORT loc_1_035B
        mov    edx, dword ptr [ebp + 10h]
        shl    edx, 10h
        mov    dx, ax
        push   edx
        clc
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 0a4h], 0
        je     SHORT loc_1_0335
        popfd
        call   dword ptr [edi + 0a4h]
        jmp    SHORT loc_1_0336
loc_1_0335:
        popfd
loc_1_0336:
        pop    edi
        pop    edx
        jae    SHORT loc_1_0341
        db  0E8h, 0D2h, 022h, 0FFh, 0FFh
        jmp    SHORT loc_1_0355
loc_1_0341:
        cmp    al, 3
        jne    SHORT loc_1_035B
        mov    eax, dword ptr [edi + 620h]
        mov    word ptr [ebp + 10h], ax
        mov    word ptr [ebp + 1ch], 4fh
loc_1_0355:
        add    word ptr [ebp + 24h], 9
loc_1_035B:
        ret
loc_1_035C:
        inc    word ptr [ebp + 24h]
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    edx, eax
        shl    edx, 10h
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        mov    dx, ax
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        cmp    byte ptr [edi + 638h], 0
        jne    SHORT loc_1_038A
        db  0E8h, 087h, 022h, 0FFh, 0FFh
loc_1_038A:
        ret
        db  090h
loc_1_038C:
        call   sub_1_03AB
        jae    SHORT loc_1_03A5
        db  08Bh, 00Dh, 0BCh, 00Eh, 000h, 000h
        db  08Bh, 015h, 0C0h, 00Eh, 000h, 000h
        db  0CDh, 020h, 049h, 080h, 001h, 000h
loc_1_03A5:
        db  0CDh, 020h, 047h, 080h, 001h, 000h
sub_1_03AB:
        mov    eax, dword ptr [ebp + 1ch]
        cmp    ah, 40h
        je     SHORT loc_1_03B5
        stc
        ret
loc_1_03B5:
        or     al, al
        je     SHORT loc_1_0408
        cmp    al, 3
        je     NEAR PTR loc_1_050D
        cmp    al, 4
        je     NEAR PTR loc_1_0531
        cmp    al, 7
        je     SHORT loc_1_03CF
        stc
        ret
loc_1_03CF:
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        and    byte ptr [edi + 2], 0bdh
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     NEAR PTR loc_1_0501
        xor    eax, eax
        xchg   dword ptr [edi + 5dch], eax
        or     eax, eax
        je     NEAR PTR loc_1_0501
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        jmp    NEAR PTR loc_1_0501
loc_1_0408:
        dec    al
        mov    byte ptr [ebp + 1ch], al
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
sub_1_0415:
        or     byte ptr [edi + 2], 2
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_1_0485
        db  0E8h, 0A1h, 0FCh, 0FEh, 0FFh
        test   byte ptr [edi + 1], 1
        jne    SHORT loc_1_0485
        db  0E8h, 0C3h, 025h, 0FFh, 0FFh
        add    ecx, eax
        shl    ecx, 0ch
        sub    ecx, 10h
        db  003h, 00Dh, 0C8h, 00Eh, 000h, 000h
        mov    dx, 3ceh
        in     al, dx
        mov    ah, al
        mov    al, 5
        jmp    SHORT loc_1_044A
loc_1_044A:
        out    dx, al
        inc    edx
        jmp    SHORT loc_1_044E
loc_1_044E:
        in     al, dx
        push   eax
        and    al, 0f4h
        jmp    SHORT loc_1_0454
loc_1_0454:
        out    dx, al
        mov    dx, 3c4h
        jmp    SHORT loc_1_045B
loc_1_045B:
        in     al, dx
        mov    ah, al
        mov    al, 2
        jmp    SHORT loc_1_0462
loc_1_0462:
        out    dx, al
        inc    edx
        jmp    SHORT loc_1_0466
loc_1_0466:
        in     al, dx
        push   eax
        mov    al, 0fh
        jmp    SHORT loc_1_046C
loc_1_046C:
        out    dx, al
        pop    eax
        jmp    SHORT loc_1_0470
loc_1_0470:
        out    dx, al
        dec    edx
        mov    al, ah
        jmp    SHORT loc_1_0476
loc_1_0476:
        out    dx, al
        mov    dx, 3cfh
        pop    eax
        jmp    SHORT loc_1_047E
loc_1_047E:
        out    dx, al
        dec    edx
        mov    al, ah
        jmp    SHORT loc_1_0484
loc_1_0484:
        out    dx, al
loc_1_0485:
        lea    eax, [edi + 4afh]
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_1_04AD
        mov    eax, dword ptr [edi + 5dch]
        or     eax, eax
        jne    SHORT loc_1_04B7
        push   0
        push   94h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
loc_1_04AD:
        mov    dword ptr [edi + 5dch], eax
        or     eax, eax
        je     SHORT loc_1_04DF
loc_1_04B7:
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        jne    SHORT loc_1_04D0
        push   eax
        db  0E8h, 03Dh, 013h, 0FFh, 0FFh
        db  0E8h, 09Fh, 014h, 0FFh, 0FFh
        db  0E8h, 0E5h, 012h, 0FFh, 0FFh
        pop    eax
loc_1_04D0:
        lea    esi, [edi + 0ch]
        push   edi
        mov    edi, eax
        mov    ecx, 25h
        cld
        rep movsd
        pop    edi
loc_1_04DF:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_1_04ED
        test   byte ptr [edi + 4], 1
        jmp    SHORT loc_1_04F1
loc_1_04ED:
        test   byte ptr [edi + 4], 2
loc_1_04F1:
        je     SHORT loc_1_0501
        or     byte ptr [edi + 2], 40h
        test   byte ptr [ebx], 8
        je     SHORT loc_1_0501
        db  0E8h, 08Dh, 044h, 0FFh, 0FFh
loc_1_0501:
        db  0E8h, 048h, 034h, 0FFh, 0FFh
        db  0E8h, 0B1h, 046h, 0FFh, 0FFh
loc_1_050B:
        clc
        ret
loc_1_050D:
        db  00Fh, 0BAh, 02Dh, 0C4h, 006h, 000h, 000h, 003h
        jb     SHORT loc_1_0529
        mov    eax, 100000h
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
        db  0C6h, 005h, 0C4h, 00Eh, 000h, 000h, 000h
loc_1_0529:
        db  0FEh, 005h, 0C4h, 00Eh, 000h, 000h
        jmp    SHORT loc_1_050B
loc_1_0531:
        db  0FEh, 00Dh, 0C4h, 00Eh, 000h, 000h
        jne    SHORT loc_1_050B
        db  00Fh, 0BAh, 035h, 0C4h, 006h, 000h, 000h, 003h
        jae    SHORT loc_1_050B
        mov    eax, 0fff00000h
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
        db  0E8h, 03Fh, 0FBh, 0FEh, 0FFh
        clc
        ret
loc_1_0555:
        db  0A1h, 0CCh, 006h, 000h, 000h
        add    eax, ebx
        inc    word ptr [ebp + 24h]
        test   byte ptr [eax + 5e2h], 1
        je     SHORT loc_1_056F
        call   sub_1_05C0
        ret
loc_1_056F:
        inc    word ptr [ebp + 24h]
        ret
loc_1_0574:
        movzx  eax, word ptr [ebp + 4]
        or     eax, eax
        je     SHORT loc_1_058D
        mov    ax, 400h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        cmp    eax, -1
        jne    SHORT loc_1_058D
        xor    eax, eax
loc_1_058D:
        mov    edx, dword ptr [ebp + 10h]
        cmp    dx, 1
        jne    SHORT loc_1_05A9
        movzx  edx, dx
        db  0A3h, 0FCh, 00Eh, 000h, 000h
        mov    cx, word ptr [ebp + 18h]
        db  066h, 089h, 00Dh, 0B2h, 00Fh, 000h, 000h
loc_1_05A9:
        push   eax
        push   edx
        db  0E8h, 09Eh, 0FBh, 0FEh, 0FFh
        pop    ecx
        pop    ecx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_05C0:
        db  083h, 03Dh, 0A4h, 010h, 000h, 000h, 0FFh
        jne    SHORT loc_1_05CD
        sub    eax, eax
        jmp    SHORT loc_1_05F7
loc_1_05CD:
        db  083h, 03Dh, 0A0h, 010h, 000h, 000h, 000h
        mov    eax, 1
        jne    SHORT loc_1_05F7
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 05Fh, 012h, 0FFh, 0FFh
        add    esp, 0ch
        db  0A3h, 0A0h, 010h, 000h, 000h
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_1_05F7:
        ret
sub_1_05F8:
        db  0A1h, 0A0h, 010h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_0614
        push   eax
        db  0E8h, 051h, 012h, 0FFh, 0FFh
        add    esp, 4
        db  0C7h, 005h, 0A0h, 010h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0614:
        ret
        db  000h, 000h, 000h, 0CCh, 000h, 000h, 000h, 0E0h, 001h, 000h, 000h, 061h, 002h, 000h, 000h, 0D9h
        db  02Eh, 000h, 000h, 03Ch, 02Fh, 000h, 000h, 0F5h, 02Fh, 000h, 000h, 054h, 002h, 000h, 000h, 049h
        db  001h, 000h, 000h, 0ADh, 001h, 000h, 000h, 0C2h, 001h, 000h, 000h, 0DAh, 001h, 000h, 000h, 01Dh
        db  002h, 000h, 000h, 0F9h, 02Eh, 000h, 000h, 0AEh, 02Fh, 000h, 000h, 0C0h, 005h, 000h, 000h, 0CBh
        db  005h, 000h, 000h, 0CCh, 02Eh, 000h, 000h, 083h, 003h, 000h, 000h, 086h, 002h, 000h, 000h, 009h
        db  006h, 000h, 000h, 034h, 018h, 000h, 000h, 0CCh, 000h, 000h, 000h, 0DEh, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 00Ah, 000h, 002h, 000h, 000h, 000h, 056h, 044h, 044h, 020h
        db  020h, 020h, 020h, 020h, 000h, 000h, 000h, 028h, 000h, 000h, 000h, 000h, 03Fh, 031h, 000h, 000h
        db  03Fh, 031h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  018h, 006h, 000h, 000h, 017h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
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
        db  000h, 000h, 000h, 000h
loc_1_0798:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_079C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07A0:
        add    byte ptr [eax], al
loc_1_07A2:
        add    byte ptr [eax], al
loc_1_07A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_07C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_085C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0DF8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0DFC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0E04:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0E98:
        add    byte ptr [edx], al
loc_1_0E9A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0E9E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0EA4:
        mov    al, byte ptr [0b010a020h]
        or     byte ptr [eax + 8], bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0EC5:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h
loc_1_0ED2:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0ED6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0EDA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0EE0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0EE4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0EE8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0EEC:
        add    byte ptr [eax], al
loc_1_0EEE:
        add    byte ptr [eax], al
loc_1_0EF0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0EF4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0EF8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0EFC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0F00:
        add    byte ptr [eax], al
loc_1_0F02:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0F08:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0F0C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_VDD_Query_Access:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_VDD_Check_Update_Soon:
        add    byte ptr [eax], al
loc_1_0FB2:
        add    byte ptr [eax], al
_VDD_Get_Mini_Dispatch_Table:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  081h, 000h, 000h, 000h
_VDD_Get_VM_Info:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_VDD_Get_Special_VM_IDs:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_VDD_Register_Extra_Screen_Selector:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_VDD_Takeover_VGA_Port:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_VDD_Get_DISPLAYINFO:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_VDD_Do_Physical_IO:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_VDD_Set_Sleep_Flag_Addr:
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
VDD_DDB:
        add    byte ptr fs:[eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], cl
        add    byte ptr [esi], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], cl
        db  00Fh, 00Fh, 01Fh, 003h, 01Bh, 00Eh, 00Fh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1014:
        ret
        db  003h, 030h, 000h, 050h, 04Dh, 000h, 000h, 074h, 04Dh, 000h, 000h, 0C0h, 04Dh, 000h, 000h, 04Ch
        db  04Dh, 000h, 000h, 024h, 04Eh, 000h, 000h, 054h, 04Eh, 000h, 000h, 028h, 04Fh, 000h, 000h, 074h
        db  04Fh, 000h, 000h, 0F8h, 04Fh, 000h, 000h, 088h, 050h, 000h, 000h, 040h, 051h, 000h, 000h, 04Ch
        db  04Dh, 000h, 000h, 04Ch, 051h, 000h, 000h, 04Ch, 04Dh, 000h, 000h, 060h, 051h, 000h, 000h, 0B8h
        db  051h, 000h, 000h, 04Ch, 04Dh, 000h, 000h, 04Ch, 04Dh, 000h, 000h, 04Ch, 04Dh, 000h, 000h, 04Ch
        db  04Dh, 000h, 000h, 00Ch, 053h, 000h, 000h, 020h, 053h, 000h, 000h, 04Ch, 04Dh, 000h, 000h, 04Ch
        db  04Dh, 000h, 000h, 04Ch, 04Dh, 000h, 000h, 04Ch, 04Dh, 000h, 000h, 0B8h, 053h, 000h, 000h, 010h
        db  054h, 000h, 000h, 04Ch, 04Dh, 000h, 000h, 04Ch, 04Dh, 000h, 000h, 04Ch, 04Dh, 000h, 000h, 04Ch
        db  04Dh, 000h, 000h, 01Dh, 04Dh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
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
        db  000h, 000h, 000h
_LTEXT ends
        end
