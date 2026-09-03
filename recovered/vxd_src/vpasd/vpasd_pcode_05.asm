; vpasd_pcode_05.asm -- VPASD object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_5_0000:
        push   esi
        push   ecx
        push   edx
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        add    esi, 2
loc_5_000E:
        movzx  edx, word ptr [esi]
        add    esi, 6
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        loop   loc_5_000E
        pop    edx
        pop    ecx
        pop    esi
        clc
        ret
sub_5_0021:
        push   esi
        push   ecx
        push   edx
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        add    esi, 2
loc_5_002F:
        movzx  edx, word ptr [esi]
        add    esi, 6
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        loop   loc_5_002F
        pop    edx
        pop    ecx
        pop    esi
        clc
        ret
sub_5_0042:
        push   ebx
        push   edx
        push   eax
        mov    eax, ebx
        shl    ah, 1
        add    dl, ah
        out    dx, al
        sub    dl, ah
        call   sub_5_005F
        pop    eax
        inc    dl
        out    dx, al
        call   sub_5_005F
        pop    edx
        pop    ebx
        ret
sub_5_005F:
        push   eax
        push   ecx
        inc    edx
        mov    ecx, 32h
loc_5_0067:
        in     al, dx
        loop   loc_5_0067
        dec    edx
        pop    ecx
        pop    eax
        ret
sub_5_006E:
        push   eax
        push   edx
        push   ebx
        push   ecx
        mov    edx, 388h
        mov    ebx, 105h
        mov    al, 1
        call   sub_5_0042
        mov    ebx, 4
        mov    al, 60h
        call   sub_5_0042
        mov    ebx, 104h
        mov    al, 3fh
        call   sub_5_0042
        mov    ebx, 8
        xor    al, al
        call   sub_5_0042
        mov    ebx, 0bdh
        mov    al, 0c0h
        call   sub_5_0042
        mov    ecx, 15h
        mov    ebx, 40h
        mov    al, 3fh
loc_5_00BF:
        xor    bh, bh
        call   sub_5_0042
        mov    bh, 1
        call   sub_5_0042
        inc    bl
        loop   loc_5_00BF
        mov    ecx, 8
        mov    ebx, 0b0h
        xor    eax, eax
loc_5_00DD:
        xor    bh, bh
        call   sub_5_0042
        mov    bh, 1
        call   sub_5_0042
        inc    bl
        loop   loc_5_00DD
        mov    ebx, 104h
        xor    eax, eax
        call   sub_5_0042
        xor    eax, eax
        mov    ebx, 105h
        call   sub_5_0042
        call   sub_5_005F
        call   sub_5_005F
        call   sub_5_005F
        pop    ecx
        pop    ebx
        pop    edx
        pop    eax
        ret
loc_5_011B:
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, ebx
        je     SHORT loc_5_012C
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_012A:
        jb     SHORT loc_5_013F
loc_5_012C:
        pop    eax
        cmp    ecx, 4
        jbe    SHORT loc_5_0138
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_5_0138:
        je     SHORT loc_5_013D
        in     al, dx
        jmp    SHORT loc_5_013E
loc_5_013D:
        out    dx, al
loc_5_013E:
        ret
loc_5_013F:
        pop    eax
        xor    eax, eax
        dec    eax
        ret
vpasd_PM_API:
        movzx  eax, word ptr [ebp + 14h]
        cmp    ah, 3
        jae    SHORT loc_5_016E
        movzx  ecx, ah
        shl    ecx, 3
        db  08Dh, 089h, 000h, 000h, 000h, 000h
        movzx  eax, al
        cmp    eax, dword ptr [ecx]
        jae    SHORT loc_5_016E
        mov    ecx, dword ptr [ecx + 4]
        call   dword ptr [ecx + eax*4]
        jb     SHORT loc_5_016E
        and    word ptr [ebp + 2ch], 0fffeh
        ret
loc_5_016E:
        or     word ptr [ebp + 2ch], 1
        ret
loc_5_0174:
        mov    word ptr [ebp + 1ch], 400h
        ret
loc_5_017B:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp + 1ch], eax
        shr    eax, 10h
        mov    word ptr [ebp + 14h], ax
        clc
        ret
loc_5_018C:
        db  0A1h, 000h, 000h, 000h, 000h
        push   ecx
        mov    ecx, 0ffffh
        or     eax, eax
        je     SHORT loc_5_01A1
        db  0CDh, 020h, 01Ch, 000h, 004h, 000h
loc_5_01A1:
        mov    word ptr [ebp + 1ch], cx
        pop    ecx
        clc
        ret
loc_5_01A8:
        clc
        ret
loc_5_01AA:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp + 1ch], eax
        shr    eax, 10h
        mov    word ptr [ebp + 14h], ax
        clc
        ret
loc_5_01BB:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp + 1ch], eax
        clc
        ret
loc_5_01C5:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp + 1ch], eax
        shr    eax, 10h
        mov    word ptr [ebp + 14h], ax
        clc
        ret
loc_5_01D6:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_01E8
        mov    dword ptr [ebp + 1ch], 1
        jmp    SHORT loc_5_0213
loc_5_01E8:
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        db  0F7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        jne    SHORT loc_5_020C
        db  0A1h, 000h, 000h, 000h, 000h
        db  083h, 00Dh, 000h, 000h, 000h, 000h, 001h
        db  0CDh, 020h, 013h, 000h, 004h, 000h
loc_5_020C:
        mov    dword ptr [ebp + 1ch], 0
loc_5_0213:
        clc
        ret
loc_5_0215:
        db  039h, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_0245
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0F7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_5_0245
        db  0A1h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 004h, 000h
        db  083h, 025h, 000h, 000h, 000h, 000h, 0FEh
loc_5_0245:
        clc
        ret
loc_5_0247:
        movzx  edx, word ptr [ebp + 1ch]
        mov    word ptr [ebp + 1ch], 0
        or     edx, edx
        je     SHORT loc_5_0266
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_025A:
        jb     SHORT loc_5_025E
        clc
        ret
loc_5_025E:
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_5_026C
loc_5_0266:
        mov    word ptr [ebp + 1ch], 1
loc_5_026C:
        stc
        ret
loc_5_026E:
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_02A3
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    ecx, ebx
        je     SHORT loc_5_0288
        mov    word ptr [ebp + 1ch], 3
        jmp    SHORT loc_5_02A9
loc_5_0288:
        call   sub_5_0021
        xor    ebx, ebx
        db  087h, 01Dh, 000h, 000h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
loc_5_02A3:
        mov    word ptr [ebp + 1ch], 1
loc_5_02A9:
        stc
        ret
loc_5_02AB:
        pushfd
        mov    ax, word ptr [ebp + 1ch]
        cmp    ax, 1
        jne    SHORT loc_5_02F8
        mov    ecx, dword ptr [ebp + 18h]
        db  03Bh, 00Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_02F8
        xchg   edi, eax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cmp    edi, -1
        je     SHORT loc_5_02F8
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     SHORT loc_5_02F8
        cmp    ecx, 1ch
        ja     SHORT loc_5_02F8
        mov    word ptr [edi + 4], 1
        mov    word ptr [edi + 6], 388h
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [edi + 0ch], eax
        mov    word ptr [ebp + 1ch], 1
        popfd
        clc
        ret
loc_5_02F8:
        mov    word ptr [ebp + 1ch], 0
        popfd
        stc
        ret
loc_5_0301:
        xchg   esi, eax
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        push   dword ptr [ebp + 3ch]
        push   dword ptr [ebp + 14h]
        mov    eax, dword ptr [esi + 4]
        mov    word ptr [ebp + 14h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 3ch], ax
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [ebp + 14h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 3ch], ax
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    dword ptr [esi + 8], eax
        pop    dword ptr [ebp + 14h]
        pop    dword ptr [ebp + 3ch]
        mov    eax, dword ptr [esi]
        db  0B8h, 000h, 000h, 000h, 000h
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   eax
        push   dword ptr [esi]
        call   sub_5_08BA
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_5_0378
        mov    word ptr [ebp + 1ch], ax
        shr    eax, 10h
        mov    word ptr [ebp + 14h], ax
        clc
        ret
loc_5_0378:
        mov    word ptr [ebp + 1ch], 0
        mov    word ptr [ebp + 14h], 0
        stc
        ret
loc_5_0386:
        xchg   esi, eax
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        mov    eax, dword ptr [esi]
        db  0B8h, 000h, 000h, 000h, 000h
        db  08Dh, 005h, 000h, 000h, 000h, 000h
        push   dword ptr [esi + 4]
        push   eax
        call   sub_5_0A4A
        add    esp, 8
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
loc_5_03B3:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp + 10h], ax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp + 18h], ax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp + 14h], ax
        clc
        ret
sub_5_03D0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 20ch
        test   eax, eax
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_5_040E
        cmp    eax, 1
        je     SHORT loc_5_0415
        cmp    eax, 2
        je     NEAR PTR loc_5_0571
        cmp    eax, 3
        je     NEAR PTR loc_5_05AC
        cmp    eax, 4
        je     NEAR PTR loc_5_0571
        mov    eax, 1
        jmp    NEAR PTR loc_5_05D2
loc_5_040E:
        xor    eax, eax
        jmp    NEAR PTR loc_5_05D2
loc_5_0415:
        cmp    dword ptr [ebp + 0ch], 1
        jbe    SHORT loc_5_0425
        mov    eax, 1
        jmp    NEAR PTR loc_5_05D2
loc_5_0425:
        push   0
        lea    eax, [ebp - 20ch]
        push   100h
        mov    esi, dword ptr [ebp + 10h]
        push   eax
        push   0
        push   esi
        call   sub_5_13DC
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_5_05D2
        push   0
        lea    eax, [ebp - 10ch]
        push   esi
        push   eax
        call   sub_5_13D4
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_5_05D2
        cmp    word ptr [ebp - 0b0h], 2
        jae    SHORT loc_5_0477
        mov    eax, 13h
        jmp    NEAR PTR loc_5_05D2
loc_5_0477:
        cmp    word ptr [ebp - 84h], 4
        je     SHORT loc_5_048B
        mov    eax, 13h
        jmp    NEAR PTR loc_5_05D2
loc_5_048B:
        cmp    word ptr [ebp - 5eh], 1
        mov    cx, word ptr [ebp - 0ach]
        jb     NEAR PTR loc_5_056A
        cmp    word ptr [ebp - 4eh], 1
        jb     NEAR PTR loc_5_056A
        movzx  di, byte ptr [ebp - 5ch]
        movzx  ax, byte ptr [ebp - 4ch]
        movzx  edx, word ptr [ebp - 0aeh]
        mov    word ptr [ebp - 2], ax
        cmp    edx, -1
        jne    SHORT loc_5_04CC
        mov    eax, 13h
        jmp    NEAR PTR loc_5_05D2
loc_5_04CC:
        cmp    word ptr [ebp - 0b0h], 8
        jbe    SHORT loc_5_04E6
        cmp    word ptr [ebp - 76h], 10h
        jne    SHORT loc_5_04E6
        mov    bx, word ptr [ebp - 9eh]
        jmp    SHORT loc_5_04EA
loc_5_04E6:
        mov    bx, 0ffffh
loc_5_04EA:
        push   0
        push   20h
        push   ebx
        push   dword ptr [ebp - 2]
        push   edi
        push   ecx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_04FA:
        add    esp, 1ch
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    SHORT loc_5_0556
        cmp    bx, 0ffffh
        je     SHORT loc_5_0556
        cmp    di, 9
        mov    eax, 2
        je     SHORT loc_5_0519
        movzx  eax, di
loc_5_0519:
        movzx  ecx, word ptr [ebp - 2]
        movzx  edx, bx
        push   ecx
        push   eax
        push   edx
        lea    ecx, [ebp - 34h]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_5_13CC
        add    esp, 14h
        lea    ecx, [ebp - 34h]
        db  0C7h, 045h, 0F0h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 14h], ecx
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 10h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 002h, 000h, 04Ah, 004h
        add    esp, 8
loc_5_0556:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_5_0565
        push   esi
        call   sub_5_0DA8
        add    esp, 4
loc_5_0565:
        mov    eax, dword ptr [ebp - 8]
        jmp    SHORT loc_5_05D2
loc_5_056A:
        mov    eax, 13h
        jmp    SHORT loc_5_05D2
loc_5_0571:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_0593
        db  0C7h, 045h, 0F4h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 0ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 004h, 000h, 04Ah, 004h
        add    esp, 4
loc_5_0593:
        mov    esi, dword ptr [ebp + 10h]
        push   esi
        call   sub_5_0E57
        add    esp, 4
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_05A5:
        add    esp, 4
        xor    eax, eax
        jmp    SHORT loc_5_05D2
loc_5_05AC:
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        je     SHORT loc_5_05BF
        cmp    eax, 1
        je     SHORT loc_5_05BF
        mov    eax, 1
        jmp    SHORT loc_5_05D2
loc_5_05BF:
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_05C4:
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 13h
loc_5_05D2:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  0CCh, 0CCh, 0CCh
loc_5_05DC:
        cmp    byte ptr [ebp + 1dh], 0bch
        jne    SHORT loc_5_063B
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_5_063B
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_5_05F9
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_05F7:
        jb     SHORT loc_5_0638
loc_5_05F9:
        cmp    byte ptr [ebp + 1ch], 0
        jne    SHORT loc_5_061B
        cmp    word ptr [ebp + 10h], 3f3fh
        jne    SHORT loc_5_063B
        mov    word ptr [ebp + 10h], 6d00h
        mov    word ptr [ebp + 18h], 76h
        mov    word ptr [ebp + 14h], 2020h
        jmp    SHORT loc_5_0638
loc_5_061B:
        push   ecx
        movzx  ecx, byte ptr [ebp + 1ch]
        cmp    ecx, 15h
        jae    SHORT loc_5_063A
        db  0FFh, 014h, 08Dh, 000h, 000h, 000h, 000h
        jb     SHORT loc_5_063A
        mov    word ptr [ebp + 1ch], 4d56h
        pop    ecx
loc_5_0638:
        clc
        ret
loc_5_063A:
        pop    ecx
loc_5_063B:
        stc
        ret
loc_5_063D:
        stc
        ret
loc_5_063F:
        mov    bx, 3034h
        add    bx, 3030h
        mov    word ptr [ebp + 10h], bx
        mov    word ptr [ebp + 18h], 3030h
        xor    bx, bx
        db  08Ah, 01Dh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp + 14h], bx
        clc
        ret
loc_5_0661:
        push   ecx
        push   esi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        add    esi, ebx
        cmp    dword ptr [esi], 0
        jne    SHORT loc_5_0680
        clc
        mov    ecx, 23h
        db  0CDh, 020h, 008h, 000h, 006h, 000h
        jb     SHORT loc_5_06CC
        mov    dword ptr [esi], edi
loc_5_0680:
        mov    eax, dword ptr [esi]
        shr    eax, 10h
        mov    word ptr [ebp + 14h], ax
        mov    ax, word ptr [esi]
        mov    word ptr [ebp + 10h], ax
        mov    word ptr [ebp + 18h], 23h
        mov    edi, dword ptr [esi]
        shr    edi, 0ch
        and    edi, 0ffff0h
        movzx  eax, word ptr [esi]
        add    edi, eax
        add    edi, dword ptr [ebx + 4]
        push   edi
        xor    al, al
        mov    ecx, 23h
        rep stosb
        pop    edi
        mov    al, 80h
        mov    byte ptr [edi + 0ch], al
        mov    al, byte ptr [esi + 2bh]
        mov    byte ptr [edi + 12h], al
        mov    al, byte ptr [esi + 2ah]
        mov    byte ptr [edi + 0eh], al
        mov    al, byte ptr [esi + 2dh]
        mov    byte ptr [edi + 0fh], al
        clc
loc_5_06CC:
        pop    esi
        pop    ecx
        ret
loc_5_06CF:
        pushad
        db  0A1h, 000h, 000h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_5_06EF
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        jb     NEAR PTR loc_5_0780
        db  0A3h, 000h, 000h, 000h, 000h
loc_5_06EF:
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        add    esi, ebx
        cmp    dword ptr [esi + 4], 0
        jne    SHORT loc_5_0761
        clc
        mov    ecx, 0ach
        db  0CDh, 020h, 008h, 000h, 006h, 000h
        jb     SHORT loc_5_0780
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [esi + 8], ecx
        shr    edi, 0ch
        and    edi, 0ffff0h
        movzx  eax, word ptr [esi + 4]
        add    edi, eax
        add    edi, dword ptr [ebx + 4]
        db  0A1h, 000h, 000h, 000h, 000h
        stosd
        mov    ecx, 0eh
        mov    eax, dword ptr [esi + 4]
        add    eax, 3ch
loc_5_0736:
        stosd
        add    eax, 8
        loop   loc_5_0736
        mov    ecx, 0eh
        mov    ax, word ptr [esi + 4]
        shl    eax, 10h
        mov    ax, 2effh
loc_5_074C:
        push   eax
        mov    eax, 0eh
        sub    eax, ecx
        shl    eax, 8
        or     eax, 2e000068h
        stosd
        pop    eax
        stosd
        loop   loc_5_074C
loc_5_0761:
        mov    eax, dword ptr [esi + 4]
        shr    eax, 10h
        mov    word ptr [ebp + 14h], ax
        mov    ax, word ptr [esi + 4]
        add    ax, 4
        mov    word ptr [ebp + 10h], ax
        mov    word ptr [ebp + 18h], 0eh
        clc
        jmp    SHORT loc_5_0793
loc_5_0780:
        mov    word ptr [ebp + 10h], 0
        mov    word ptr [ebp + 18h], 0
        mov    word ptr [ebp + 14h], 0
        stc
loc_5_0793:
        popad
        ret
loc_5_0795:
        xor    ax, ax
        db  0A0h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp + 10h], ax
        db  0A0h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp + 18h], ax
        mov    word ptr [ebp + 14h], 65h
        clc
        ret
loc_5_07B2:
        stc
        ret
loc_5_07B4:
        stc
        ret
loc_5_07B6:
        stc
        ret
loc_5_07B8:
        stc
        ret
loc_5_07BA:
        stc
        ret
loc_5_07BC:
        stc
        ret
loc_5_07BE:
        stc
        ret
loc_5_07C0:
        stc
        ret
loc_5_07C2:
        stc
        ret
loc_5_07C4:
        stc
        ret
loc_5_07C6:
        stc
        ret
loc_5_07C8:
        ret
sub_5_07C9:
        push   ebp
        mov    ebp, esp
        sub    esp, 6ch
        mov    eax, 0ffffffffh
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   esi, esi
        je     NEAR PTR loc_5_08B3
        lea    eax, [ebp - 6ch]
        push   eax
        call   sub_5_0ADE
        add    esp, 4
        call   sub_5_0ACC
        call   sub_5_0B1B
        mov    edi, dword ptr [eax + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_0B0D
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_0B0D
        add    esp, 4
        mov    ebx, dword ptr [ebp + 10h]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_0B0D
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_0B0D
        add    esp, 4
        mov    ebx, dword ptr [ebp + 14h]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_0B0D
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_0B0D
        add    esp, 4
        mov    ebx, dword ptr [ebp + 18h]
        mov    eax, ebx
        shr    eax, 10h
        movzx  ecx, ax
        push   ecx
        call   sub_5_0B0D
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_0B0D
        movzx  ecx, si
        shr    esi, 10h
        add    esp, 4
        movzx  eax, si
        push   ecx
        push   eax
        call   sub_5_0AFE
        add    esp, 8
        call   sub_5_0AD8
        movzx  esi, word ptr [edi + 14h]
        shl    esi, 10h
        movzx  eax, word ptr [edi + 1ch]
        or     esi, eax
        call   sub_5_0AD2
        lea    ecx, [ebp - 6ch]
        push   ecx
        call   sub_5_0AEE
        add    esp, 4
        mov    eax, esi
loc_5_08B3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_08BA:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        push   ebp
        mov    ebp, dword ptr [esi]
        test   ebp, ebp
        jne    SHORT loc_5_08E0
        push   15h
        push   4
        call   sub_5_1428
        mov    ebp, eax
        test   ebp, ebp
        jne    SHORT loc_5_08DE
        xor    eax, eax
        jmp    NEAR PTR loc_5_0A45
loc_5_08DE:
        mov    dword ptr [esi], ebp
loc_5_08E0:
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 8
        jbe    SHORT loc_5_08FD
        mov    eax, dword ptr [esp + 1ch]
        mov    byte ptr [eax + 8], 0
loc_5_08FD:
        push   ebp
        call   sub_5_1418
        mov    ebx, eax
        test   ebx, ebx
        je     NEAR PTR loc_5_09F1
loc_5_090D:
        mov    ecx, dword ptr [esp + 1ch]
        lea    edx, [ebx + 0ch]
loc_5_0914:
        mov    al, byte ptr [ecx]
        cmp    al, byte ptr [edx]
        jne    SHORT loc_5_0934
        or     al, al
        je     SHORT loc_5_0930
        mov    al, byte ptr [ecx + 1]
        cmp    al, byte ptr [edx + 1]
        jne    SHORT loc_5_0934
        add    ecx, 2
        add    edx, 2
        or     al, al
        jne    SHORT loc_5_0914
loc_5_0930:
        xor    eax, eax
        jmp    SHORT loc_5_0939
loc_5_0934:
        sbb    eax, eax
        sbb    eax, -1
loc_5_0939:
        test   eax, eax
        je     SHORT loc_5_094A
        push   ebx
        push   ebp
        call   sub_5_1408
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_5_090D
loc_5_094A:
        test   ebx, ebx
        je     NEAR PTR loc_5_09F1
        cmp    dword ptr [ebx + 4], 0
        je     SHORT loc_5_095F
        xor    eax, eax
        jmp    NEAR PTR loc_5_0A45
loc_5_095F:
        push   0
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_07C9
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_5_098C
        push   ebx
        mov    dword ptr [ebx], 0
        push   esi
        call   sub_5_0A4A
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_5_0A45
loc_5_098C:
        mov    eax, dword ptr [esp + 20h]
        push   0
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebx + 4], ecx
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   ecx
        call   sub_5_07C9
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_5_09C3
        mov    dword ptr [ebx + 4], 0
        push   ebx
        push   esi
        call   sub_5_0A4A
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_5_0A45
loc_5_09C3:
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx + 8]
        push   10000h
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_07C9
        add    esp, 14h
        push   dword ptr [ebx]
        push   dword ptr [ebx + 8]
        push   10000h
        push   ebx
        push   dword ptr [ebx + 4]
        call   sub_5_07C9
        add    esp, 14h
        jmp    SHORT loc_5_0A43
loc_5_09F1:
        push   ebp
        call   sub_5_13F4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_5_0A01
        xor    eax, eax
        jmp    SHORT loc_5_0A45
loc_5_0A01:
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 20h]
        mov    edi, dword ptr [esp + 1ch]
        mov    dword ptr [ebx + 8], eax
        mov    edx, dword ptr [ecx + 4]
        sub    eax, eax
        mov    ecx, 0ffffffffh
        mov    dword ptr [ebx], edx
        mov    dword ptr [ebx + 4], 0
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        lea    edi, [ebx + 0ch]
        rep movsd
        mov    ecx, eax
        push   ebx
        and    ecx, 3
        push   ebp
        rep movsb
        call   sub_5_13E4
loc_5_0A43:
        mov    eax, ebx
loc_5_0A45:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_0A4A:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        test   edi, edi
        je     SHORT loc_5_0A96
        mov    eax, dword ptr [edi]
        test   eax, eax
        je     SHORT loc_5_0A74
        push   0
        push   dword ptr [edi + 8]
        push   40000h
        push   edi
        push   eax
        call   sub_5_07C9
        add    esp, 14h
        mov    dword ptr [edi], 0
loc_5_0A74:
        mov    eax, dword ptr [edi + 4]
        test   eax, eax
        je     SHORT loc_5_0A96
        push   0
        push   dword ptr [edi + 8]
        push   40000h
        push   edi
        push   eax
        call   sub_5_07C9
        mov    dword ptr [edi + 4], 0
        add    esp, 14h
loc_5_0A96:
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     SHORT loc_5_0AC7
        push   edi
        push   eax
        call   sub_5_145C
        push   edi
        push   dword ptr [esi]
        call   sub_5_144C
        push   dword ptr [esi]
        call   sub_5_1418
        test   eax, eax
        jne    SHORT loc_5_0AC7
        push   dword ptr [esi]
        call   sub_5_143C
        mov    dword ptr [esi], 0
loc_5_0AC7:
        pop    edi
        pop    esi
        ret
        db  0CCh, 0CCh
sub_5_0ACC:
        db  0CDh, 020h, 083h, 080h, 001h, 000h
sub_5_0AD2:
        db  0CDh, 020h, 086h, 080h, 001h, 000h
sub_5_0AD8:
        db  0CDh, 020h, 085h, 080h, 001h, 000h
sub_5_0ADE:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        pop    ebp
        ret
sub_5_0AEE:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        pop    ebp
        ret
sub_5_0AFE:
        movzx  ecx, word ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        db  0CDh, 020h, 048h, 080h, 001h, 000h
sub_5_0B0D:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        pop    ebp
        ret
sub_5_0B1B:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
loc_5_0B26:
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        push   edi
        mov    ecx, 10h
        db  0BFh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb
        je     SHORT loc_5_0B5B
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        mov    esi, eax
        repe cmpsb
        mov    eax, 80004002h
        jne    SHORT loc_5_0B69
loc_5_0B5B:
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_0B69:
        pop    edi
        pop    esi
        ret    0ch
loc_5_0B6E:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0ch]
        inc    eax
        mov    dword ptr [ecx + 0ch], eax
        ret    4
loc_5_0B7C:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0ch]
        dec    eax
        mov    dword ptr [ecx + 0ch], eax
        ret    4
loc_5_0B8A:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 14h]
        lea    esi, [edi + 104h]
        mov    dword ptr [edi], 0bh
        call   sub_5_10AC
        mov    ebx, eax
        push   0ffh
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_5_1470
        add    esp, 0ch
        lea    eax, [edi + 4]
        push   0ffh
        push   ebx
        push   eax
        call   sub_5_1470
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 0ch
        xor    ecx, ecx
        mov    dword ptr [edi + 210h], ecx
        mov    edx, dword ptr [eax + 4]
        mov    eax, 0b3h
        mov    dword ptr [edi + 204h], edx
        mov    dword ptr [edi + 20ch], ecx
        mov    word ptr [edi + 208h], 3203h
        cmp    byte ptr [esi], cl
        je     SHORT loc_5_0C01
loc_5_0BF6:
        movsx  ecx, byte ptr [esi]
        add    eax, ecx
        inc    esi
        cmp    byte ptr [esi], 0
        jne    SHORT loc_5_0BF6
loc_5_0C01:
        mov    ecx, 0f7h
        sub    edx, edx
        div    ecx
        mov    word ptr [edi + 20ah], dx
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_5_0C19:
        push   esi
        push   edi
        mov    esi, 8878000ah
        mov    edi, dword ptr [esp + 0ch]
        cmp    dword ptr [edi + 0ch], 0
        jne    SHORT loc_5_0C3E
        push   dword ptr [edi + 4]
        call   sub_5_117D
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_5_0C3E
        xor    esi, esi
        inc    dword ptr [edi + 0ch]
loc_5_0C3E:
        mov    eax, esi
        pop    edi
        pop    esi
        ret    4
loc_5_0C45:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax], 7
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_0C5E
        mov    dword ptr [eax], 0fh
loc_5_0C5E:
        mov    dword ptr [eax + 0ch], 1
        xor    ecx, ecx
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [eax + 10h], ecx
        mov    dword ptr [eax + 14h], ecx
        mov    dword ptr [eax + 18h], ecx
        mov    dword ptr [eax + 1ch], ecx
        mov    dword ptr [eax + 20h], ecx
        mov    dword ptr [eax + 24h], ecx
        mov    dword ptr [eax + 28h], ecx
        mov    dword ptr [eax + 2ch], ecx
        mov    dword ptr [eax + 30h], ecx
        mov    dword ptr [eax + 34h], ecx
        mov    dword ptr [eax + 38h], ecx
        mov    dword ptr [eax + 3ch], ecx
        mov    dword ptr [eax + 40h], ecx
        mov    dword ptr [eax + 44h], ecx
        mov    dword ptr [eax + 48h], ecx
        xor    eax, eax
        ret    8
loc_5_0C9F:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   ebp
        mov    edi, 8007000eh
        mov    esi, dword ptr [esp + 2ch]
        test   byte ptr [esp + 1ch], 0f0h
        mov    dword ptr [esi], ebx
        je     SHORT loc_5_0CC1
        mov    eax, 8878001eh
        jmp    NEAR PTR loc_5_0D67
loc_5_0CC1:
        mov    ebp, dword ptr [esp + 14h]
        cmp    dword ptr [ebp + 10h], 0
        jne    SHORT loc_5_0D46
        push   201h
        push   31h
        call   sub_5_1480
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_5_0D3F
        db  0C7h, 003h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 4], ebp
        push   dword ptr [ebp + 4]
        push   ebx
        call   sub_5_11C6
        add    esp, 8
        mov    eax, dword ptr [ebx + 14h]
        cmp    eax, 1000h
        jbe    SHORT loc_5_0D4B
        cmp    eax, 0ffffh
        jae    SHORT loc_5_0D4B
        cmp    dword ptr [ebx + 0ch], 0
        je     SHORT loc_5_0D4B
        cmp    dword ptr [ebx + 10h], 0
        je     SHORT loc_5_0D4B
        cmp    dword ptr [ebx + 18h], 0
        je     SHORT loc_5_0D4B
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 2ch]
        push   ebx
        xor    edi, edi
        call   sub_5_0EBF
        mov    eax, dword ptr [esp + 24h]
        add    esp, 0ch
        movzx  ecx, word ptr [eax + 0eh]
        mov    dword ptr [ebx + 24h], ecx
        movzx  edx, word ptr [eax + 2]
        mov    dword ptr [ebx + 28h], edx
        jmp    SHORT loc_5_0D4B
loc_5_0D3F:
        mov    edi, 8007000eh
        jmp    SHORT loc_5_0D4B
loc_5_0D46:
        mov    edi, 8878000ah
loc_5_0D4B:
        test   edi, edi
        je     SHORT loc_5_0D60
        test   ebx, ebx
        je     SHORT loc_5_0D65
        push   0
        push   ebx
        call   sub_5_1478
        add    esp, 8
        jmp    SHORT loc_5_0D65
loc_5_0D60:
        mov    dword ptr [esi], ebx
        inc    dword ptr [ebp + 10h]
loc_5_0D65:
        mov    eax, edi
loc_5_0D67:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    1ch
loc_5_0D6E:
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], 0
        mov    eax, 80004001h
        ret    0ch
loc_5_0D80:
        mov    eax, 80004005h
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 0ch]
        dec    edx
        mov    dword ptr [ecx + 0ch], edx
        jne    SHORT loc_5_0DA5
        cmp    dword ptr [ecx + 10h], 0
        jne    SHORT loc_5_0DA5
        push   dword ptr [ecx + 4]
        call   sub_5_1173
        add    esp, 4
        xor    eax, eax
loc_5_0DA5:
        ret    4
sub_5_0DA8:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   201h
        mov    dword ptr [ebp - 4], ebx
        push   14h
        call   sub_5_1480
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_5_0E27
        mov    edi, dword ptr [ebp + 8]
        push   1
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 14h]
        push   ecx
        mov    dword ptr [esi + 4], edi
        db  0C7h, 006h, 000h, 000h, 000h, 000h
        call   sub_5_149C
        test   eax, eax
        jne    SHORT loc_5_0E27
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 10h], ebx
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [ebp - 0ch], esi
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        db  0CDh, 020h, 001h, 000h, 07Eh, 035h
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], ebx
        jne    SHORT loc_5_0E27
        push   esi
        mov    ebx, 1
        push   edi
        call   sub_5_13BC
        add    esp, 8
loc_5_0E27:
        test   ebx, ebx
        jne    SHORT loc_5_0E50
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_5_0E41
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 0bh]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_5_1488
loc_5_0E41:
        test   esi, esi
        je     SHORT loc_5_0E50
        push   0
        push   esi
        call   sub_5_1478
        add    esp, 8
loc_5_0E50:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_0E57:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_5_13B2
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_5_0EB9
        push   0
        push   edi
        call   sub_5_13BC
        add    esp, 8
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [ebp - 8], 0
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        db  0CDh, 020h, 002h, 000h, 07Eh, 035h
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [esi + 8]
        push   dword ptr [eax + 0bh]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_5_1488
        push   0
        push   esi
        call   sub_5_1478
        add    esp, 8
loc_5_0EB9:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_5_0EBF:
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 14h]
        cmp    dword ptr [edx], eax
        jbe    SHORT loc_5_0ED1
        mov    dword ptr [edx], eax
loc_5_0ED1:
        mov    eax, dword ptr [esp + 0ch]
        and    dword ptr [edx], 0fffch
        mov    ecx, dword ptr [esi + 10h]
        mov    dword ptr [eax], ecx
        mov    edx, dword ptr [edx]
        mov    dword ptr [esi + 1ch], edx
        inc    dword ptr [esi + 20h]
        mov    dword ptr [esi + 8], 0
        pop    esi
        ret
loc_5_0EF1:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 20h]
        dec    esi
        mov    dword ptr [eax + 20h], esi
        jne    SHORT loc_5_0F16
        cmp    dword ptr [eax + 8], 0
        jne    SHORT loc_5_0F16
        mov    ecx, dword ptr [eax + 4]
        push   0
        push   eax
        dec    dword ptr [ecx + 10h]
        call   sub_5_1478
        add    esp, 8
loc_5_0F16:
        mov    eax, esi
        pop    esi
        ret    4
loc_5_0F1C:
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        push   edi
        mov    ecx, 10h
        db  0BFh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb
        je     SHORT loc_5_0F51
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        mov    esi, eax
        repe cmpsb
        mov    eax, 80004002h
        jne    SHORT loc_5_0F5F
loc_5_0F51:
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_0F5F:
        pop    edi
        pop    esi
        ret    0ch
loc_5_0F64:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 20h]
        inc    eax
        mov    dword ptr [ecx + 20h], eax
        ret    4
loc_5_0F72:
        mov    eax, 80004001h
        ret    20h
loc_5_0F7A:
        mov    eax, 80004001h
        ret    14h
loc_5_0F82:
        mov    eax, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 4]
        movzx  ecx, word ptr [eax + 0eh]
        mov    dword ptr [edx + 24h], ecx
        movzx  eax, word ptr [eax + 2]
        mov    dword ptr [edx + 28h], eax
        mov    eax, 80004001h
        ret    8
loc_5_0FA0:
        mov    eax, 80004001h
        ret    8
loc_5_0FA8:
        mov    eax, 80004001h
        ret    8
loc_5_0FB0:
        mov    eax, 80004001h
        ret    8
loc_5_0FB8:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 8], 1
        jne    SHORT loc_5_0FEA
        push   dword ptr [esi + 18h]
        call   sub_5_10BA
        add    esp, 4
        inc    eax
        cmp    byte ptr [esi + 30h], 3
        jbe    SHORT loc_5_0FD7
        add    eax, eax
loc_5_0FD7:
        mov    ecx, dword ptr [esi + 1ch]
        mov    edx, dword ptr [esp + 10h]
        sub    ecx, eax
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx], ecx
        jmp    SHORT loc_5_0FFE
loc_5_0FEA:
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx], 0
loc_5_0FFE:
        xor    eax, eax
        pop    esi
        ret    0ch
loc_5_1004:
        push   ebx
        mov    eax, 80004005h
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        cmp    dword ptr [edi + 8], 0
        jne    SHORT loc_5_1070
        movzx  esi, word ptr [edi + 2eh]
        mov    ebx, dword ptr [edi + 18h]
        push   ebx
        call   sub_5_10D5
        add    esp, 4
        push   dword ptr [edi + 1ch]
        push   dword ptr [edi + 0ch]
        push   ebx
        call   sub_5_10F9
        add    esp, 0ch
        push   ebx
        call   sub_5_111A
        add    esp, 4
        push   ebx
        call   sub_5_10E3
        mov    cx, word ptr [edi + 1ch]
        add    esp, 4
        cmp    byte ptr [edi + 30h], 4
        jbe    SHORT loc_5_1054
        shr    cx, 1
loc_5_1054:
        push   dword ptr [edi + 28h]
        movzx  ecx, cx
        push   dword ptr [edi + 24h]
        push   ecx
        push   esi
        call   sub_5_120C
        add    esp, 10h
        xor    eax, eax
        mov    dword ptr [edi + 8], 1
loc_5_1070:
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
loc_5_1076:
        push   esi
        mov    eax, 80004005h
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 8], 0
        je     SHORT loc_5_10A7
        movzx  eax, word ptr [esi + 2eh]
        push   eax
        call   sub_5_1314
        add    esp, 4
        push   dword ptr [esi + 18h]
        call   sub_5_10D5
        add    esp, 4
        xor    eax, eax
        mov    dword ptr [esi + 8], 0
loc_5_10A7:
        pop    esi
        ret    4
        db  0CCh
sub_5_10AC:
        db  00Fh, 0B7h, 005h, 00Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        ret
sub_5_10BA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 01Ch, 000h, 004h, 000h
        movzx  eax, cx
        pop    ecx
        pop    ebx
        pop    ebp
        ret
sub_5_10D5:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 018h, 000h, 004h, 000h
        pop    ebp
        ret
sub_5_10E3:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 019h, 000h, 004h, 000h
        pop    ebx
        pop    ebp
        ret
sub_5_10F9:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        xor    bx, bx
        mov    bh, 1
        xor    esi, esi
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 003h, 000h, 004h, 000h
        pop    ecx
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_5_111A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edx
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        xor    dx, dx
        or     dl, 8
        or     dl, 10h
        or     dl, 40h
        db  0CDh, 020h, 006h, 000h, 004h, 000h
        pop    edx
        pop    ebx
        pop    ebp
        ret
sub_5_113E:
        push   ebp
        mov    ebp, esp
        push   eax
        push   edx
        mov    edx, dword ptr [ebp + 8]
        xor    dx, 388h
        mov    ax, word ptr [ebp + 0ch]
        xor    dx, ax
        mov    al, byte ptr [ebp + 10h]
        out    dx, al
        pop    edx
        pop    eax
        pop    ebp
        ret
sub_5_115A:
        push   ebp
        mov    ebp, esp
        push   edx
        mov    edx, dword ptr [ebp + 8]
        xor    dx, 388h
        mov    ax, word ptr [ebp + 0ch]
        xor    dx, ax
        in     al, dx
        xor    ah, ah
        pop    edx
        pop    ebp
        ret
sub_5_1173:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        ret
sub_5_117D:
        push   ebp
        mov    ebp, esp
        push   edi
        push   edx
        push   ebx
        mov    edx, 0ffffh
        db  0A1h, 000h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_5_119D
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        cmp    eax, ebx
        je     SHORT loc_5_119D
        jmp    SHORT loc_5_11BF
loc_5_119D:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    edx, 0
        sub    eax, eax
        db  0F7h, 005h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h
        je     SHORT loc_5_11BA
        inc    eax
loc_5_11BA:
        db  0A2h, 000h, 000h, 000h, 000h
loc_5_11BF:
        mov    eax, edx
        pop    ebx
        pop    edx
        pop    edi
        pop    ebp
        ret
sub_5_11C6:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 8]
        db  0A1h, 000h, 000h, 000h, 000h
        mov    word ptr [esi + 2eh], ax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    byte ptr [esi + 30h], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 0ch], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 10h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 14h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 18h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    word ptr [esi + 2ch], ax
        pop    esi
        pop    edi
        pop    ebp
        ret
sub_5_120C:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ecx
        push   edx
        pushfd
        cli
        mov    edx, dword ptr [ebp + 8]
        xor    eax, eax
        cmp    byte ptr [ebp + 10h], 8
        jne    SHORT loc_5_1221
        mov    al, 80h
loc_5_1221:
        push   eax
        push   0f88h
        push   edx
        call   sub_5_113E
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, 74h
        push   eax
        push   138bh
        push   edx
        call   sub_5_113E
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   1389h
        push   edx
        call   sub_5_113E
        add    esp, 0ch
        xchg   al, ah
        push   eax
        push   1389h
        push   edx
        call   sub_5_113E
        add    esp, 0ch
        push   0b89h
        push   edx
        call   sub_5_115A
        add    esp, 8
        push   eax
        push   0b89h
        push   edx
        call   sub_5_113E
        add    esp, 0ch
        push   0b8bh
        push   edx
        call   sub_5_115A
        add    esp, 8
        or     al, 8
        push   eax
        push   0b8bh
        push   edx
        call   sub_5_113E
        add    esp, 0ch
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 14h]
        cmp    ecx, 1
        jne    SHORT loc_5_12AF
        mov    al, 20h
loc_5_12AF:
        or     al, 10h
        or     al, 0c0h
        push   ax
        push   0f8ah
        push   edx
        call   sub_5_115A
        add    esp, 8
        mov    cx, ax
        pop    ax
        mov    ah, 0fh
        and    ah, cl
        or     al, ah
        xor    al, 40h
        push   eax
        push   0f8ah
        push   edx
        call   sub_5_113E
        add    esp, 0ch
        xor    al, 40h
        push   eax
        push   0f8ah
        push   edx
        call   sub_5_113E
        add    esp, 0ch
        db  0A0h, 000h, 000h, 000h, 000h
        or     al, 0c0h
        or     al, 20h
        push   eax
        push   0b8ah
        push   edx
        call   sub_5_113E
        add    esp, 0ch
        db  0A2h, 000h, 000h, 000h, 000h
        popfd
        clc
        pop    edx
        pop    ecx
        pop    eax
        pop    ebp
        ret
sub_5_1314:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ebx
        push   edx
        mov    edx, dword ptr [ebp + 8]
        pushfd
        cli
        xor    eax, eax
        push   0f8ah
        push   edx
        call   sub_5_115A
        add    esp, 8
        and    al, 7fh
        and    al, 0dfh
        push   eax
        push   0f8ah
        push   edx
        call   sub_5_113E
        add    esp, 0ch
        db  0A0h, 000h, 000h, 000h, 000h
        and    al, 3fh
        or     al, 20h
        db  0A2h, 000h, 000h, 000h, 000h
        push   eax
        push   0b8ah
        push   edx
        call   sub_5_113E
        add    esp, 0ch
        mov    bl, 8
        and    bl, 0ch
        not    bl
        db  00Ah, 01Dh, 000h, 000h, 000h, 000h
        push   0b8bh
        push   edx
        call   sub_5_115A
        add    esp, 8
        and    al, bl
        push   eax
        push   0b8bh
        push   edx
        call   sub_5_113E
        add    esp, 0ch
        push   0f8ah
        push   edx
        call   sub_5_115A
        add    esp, 8
        and    al, 0bfh
        and    al, 0efh
        push   eax
        push   0f8ah
        push   edx
        call   sub_5_113E
        add    esp, 0ch
        popfd
        pop    edx
        pop    ebx
        pop    eax
        pop    ebp
        ret
sub_5_13B2:
        push   ebp
        mov    ebp, esp
        db  0A1h, 000h, 000h, 000h, 000h
        pop    ebp
        ret
sub_5_13BC:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        db  0A3h, 000h, 000h, 000h, 000h
        pop    ebp
        ret
        db  0CCh, 0CCh, 0CCh
sub_5_13CC:
        db  0CDh, 020h, 074h, 081h, 001h, 000h
        int3
        int3
sub_5_13D4:
        db  0CDh, 020h, 03Bh, 080h, 033h, 000h
        int3
        int3
sub_5_13DC:
        db  0CDh, 020h, 03Dh, 080h, 033h, 000h
        int3
        int3
sub_5_13E4:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_13F4:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        mov    esi, ecx
        cmc
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
        db  0CCh
sub_5_1408:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_1418:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_1428:
        pop    edx
        pop    eax
        pop    ecx
        push   esi
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        cmc
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
sub_5_143C:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_144C:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_145C:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        mov    esi, ecx
        sbb    eax, eax
        inc    eax
        jmp    edx
        db  0CCh, 0CCh
sub_5_1470:
        db  0CDh, 020h, 07Bh, 081h, 001h, 000h
        int3
        int3
sub_5_1478:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        int3
        int3
sub_5_1480:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        int3
        int3
sub_5_1488:
        pop    ecx
        pop    eax
        pop    edx
        push   ebx
        mov    ebx, eax
        db  0CDh, 020h, 002h, 000h, 027h, 000h
        sbb    edx, edx
        and    eax, edx
        pop    ebx
        jmp    ecx
        db  0CCh
sub_5_149C:
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 0ch]
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        jb     SHORT loc_5_14BA
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_5_14BA:
        ret    10h
_PTEXT ends
        end
