; filesec_pcode_03.asm -- FILESEC object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

FILESEC_Service_01:
        jmp    NEAR PTR loc_3_1EED
FILESEC_Service_03:
        jmp    NEAR PTR loc_3_1F08
FILESEC_Service_02:
        jmp    NEAR PTR loc_3_1F7F
FILESEC_Service_04:
        jmp    NEAR PTR loc_3_1F93
FILESEC_Service_05:
        jmp    NEAR PTR loc_3_1FD5
FILESEC_Service_06:
        jmp    NEAR PTR loc_3_1ED6
FILESEC_Service_07:
        jmp    NEAR PTR loc_3_1F27
FILESEC_Service_08:
        jmp    NEAR PTR loc_3_1F33
FILESEC_Service_10:
        jmp    NEAR PTR loc_3_1F5D
FILESEC_Service_09:
        jmp    NEAR PTR loc_3_1FB6
FILESEC_W32_DEVICEIOCONTROL:
        mov    ebx, dword ptr [esi + 0ch]
        cmp    ebx, 0
        jne    SHORT loc_3_0041
        mov    ecx, 400h
        jmp    SHORT loc_3_0046
loc_3_0041:
        cmp    ebx, -1
        jne    SHORT loc_3_004A
loc_3_0046:
        xor    eax, eax
        clc
        ret
loc_3_004A:
        cmp    ebx, 64h
        jl     SHORT loc_3_0076
        cmp    ebx, 6bh
        jge    SHORT loc_3_0076
        sub    ebx, 64h
        mov    edi, dword ptr [esi + 14h]
        mov    eax, edi
        shr    eax, 2
        sub    esp, edi
        mov    edi, esp
        mov    esi, dword ptr [esi + 10h]
        mov    ecx, eax
        cld
        rep movsd
        push   1
        db  0FFh, 014h, 09Dh, 000h, 000h, 000h, 000h
        clc
        ret
loc_3_0076:
        mov    eax, 1
        clc
        ret
sub_3_007D:
        movzx  eax, byte ptr [esp + 4]
        db  0CDh, 020h, 0D1h, 000h, 001h, 000h
        sbb    eax, eax
        inc    eax
        ret    4
sub_3_008E:
        db  0CDh, 020h, 042h, 080h, 040h, 000h
sub_3_0094:
        db  0CDh, 020h, 040h, 080h, 040h, 000h
sub_3_009A:
        db  0CDh, 020h, 070h, 080h, 040h, 000h
sub_3_00A0:
        db  0CDh, 020h, 043h, 080h, 040h, 000h
sub_3_00A6:
        db  0CDh, 020h, 001h, 080h, 093h, 004h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_3_00B0:
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [esp + 40h], 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esp + 28h], eax
        mov    dword ptr [esp + 2ch], 0
        mov    si, word ptr [esp + 44h]
        cmp    si, 1
        je     SHORT loc_3_00EC
        cmp    si, 2
        je     SHORT loc_3_00E6
        mov    eax, 7ch
        jmp    NEAR PTR loc_3_0322
loc_3_00E6:
        cmp    si, 1
        jne    SHORT loc_3_00F9
loc_3_00EC:
        mov    edi, dword ptr [esp + 48h]
        add    edi, 8
        mov    ebx, dword ptr [esp + 34h]
        jmp    SHORT loc_3_0104
loc_3_00F9:
        mov    ebx, dword ptr [esp + 48h]
        add    ebx, 8
        mov    edi, dword ptr [esp + 38h]
loc_3_0104:
        mov    eax, dword ptr [esp + 48h]
        sub    ecx, ecx
        mov    dx, word ptr [eax + 6]
        cmp    dx, cx
        jle    SHORT loc_3_0142
loc_3_0113:
        cmp    si, 1
        jne    SHORT loc_3_011F
        mov    ax, word ptr [edi + 16h]
        jmp    SHORT loc_3_0123
loc_3_011F:
        mov    ax, word ptr [ebx + 4]
loc_3_0123:
        movzx  eax, ax
        test   eax, 0ffff7f00h
        jne    SHORT loc_3_0159
        test   eax, 0ffff7fffh
        je     SHORT loc_3_0163
        inc    ecx
        add    edi, 18h
        add    ebx, 6
        movsx  eax, dx
        cmp    eax, ecx
        jg     SHORT loc_3_0113
loc_3_0142:
        call   sub_3_2070
        mov    dword ptr [esp + 24h], eax
        or     eax, eax
        jne    SHORT loc_3_016D
        mov    eax, 8
        jmp    NEAR PTR loc_3_0322
loc_3_0159:
        mov    eax, 57h
        jmp    NEAR PTR loc_3_0322
loc_3_0163:
        mov    eax, 57h
        jmp    NEAR PTR loc_3_0322
loc_3_016D:
        mov    eax, dword ptr [esp + 24h]
        add    eax, 116h
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [esp + 28h]
        or     eax, 200h
        push   eax
        mov    eax, dword ptr [esp + 4ch]
        push   dword ptr [esp + 28h]
        push   dword ptr [eax]
        call   sub_3_1595
        mov    ebp, eax
        or     ebp, ebp
        jne    NEAR PTR loc_3_0310
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 28h]
        push   80000002h
        call   sub_3_23FC
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_3_01FD
        push   0
        push   0
        push   0
        lea    eax, [esp + 3ch]
        push   0
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   0
        push   0
        push   dword ptr [esp + 40h]
        call   sub_3_2444
        add    esp, 30h
        or     eax, eax
        jne    SHORT loc_3_022B
        cmp    dword ptr [esp + 30h], 0
        je     SHORT loc_3_022B
        push   dword ptr [esp + 14h]
        call   sub_3_2404
        add    esp, 4
        mov    ebp, 8b1h
        jmp    NEAR PTR loc_3_0310
loc_3_01FD:
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 28h]
        push   80000002h
        call   sub_3_240C
        add    esp, 0ch
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_3_0223
        mov    ebp, 8b4h
        jmp    NEAR PTR loc_3_0310
loc_3_0223:
        mov    dword ptr [esp + 2ch], 1
loc_3_022B:
        cmp    si, 1
        jne    SHORT loc_3_023A
        mov    edi, dword ptr [esp + 48h]
        add    edi, 8
        jmp    SHORT loc_3_0241
loc_3_023A:
        mov    ebx, dword ptr [esp + 48h]
        add    ebx, 8
loc_3_0241:
        mov    eax, dword ptr [esp + 48h]
        mov    dword ptr [esp + 1ch], 0
        cmp    word ptr [eax + 6], 0
        jle    SHORT loc_3_02B3
loc_3_0254:
        or     ebp, ebp
        jne    SHORT loc_3_02B3
        cmp    si, 1
        jne    SHORT loc_3_0266
        mov    ecx, edi
        mov    ax, word ptr [edi + 16h]
        jmp    SHORT loc_3_026C
loc_3_0266:
        mov    ecx, dword ptr [ebx]
        mov    ax, word ptr [ebx + 4]
loc_3_026C:
        mov    word ptr [esp + 10h], ax
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 1ch]
        push   ecx
        push   0
        call   sub_3_1BE2
        lea    ecx, [esp + 10h]
        push   2
        push   ecx
        push   3
        push   0
        add    edi, 18h
        push   eax
        add    ebx, 6
        push   dword ptr [esp + 28h]
        call   sub_3_243C
        add    esp, 18h
        mov    ebp, eax
        mov    eax, dword ptr [esp + 48h]
        inc    dword ptr [esp + 1ch]
        movsx  eax, word ptr [eax + 6]
        cmp    eax, dword ptr [esp + 1ch]
        jg     SHORT loc_3_0254
loc_3_02B3:
        cmp    dword ptr [esp + 1ch], 0
        jne    SHORT loc_3_02E3
        or     ebp, ebp
        jne    SHORT loc_3_02E7
        mov    word ptr [esp + 12h], 8000h
        lea    eax, [esp + 12h]
        push   2
        push   eax
        push   3
        push   0
        db  068h, 0F4h, 000h, 000h, 000h
        push   dword ptr [esp + 28h]
        call   sub_3_243C
        add    esp, 18h
        mov    ebp, eax
loc_3_02E3:
        or     ebp, ebp
        je     SHORT loc_3_0304
loc_3_02E7:
        cmp    dword ptr [esp + 2ch], 0
        je     SHORT loc_3_032C
        push   dword ptr [esp + 24h]
        push   80000002h
        call   sub_3_2414
        add    esp, 8
loc_3_02FF:
        mov    ebp, 8b4h
loc_3_0304:
        push   dword ptr [esp + 14h]
        call   sub_3_2404
        add    esp, 4
loc_3_0310:
        cmp    dword ptr [esp + 24h], 0
        je     SHORT loc_3_0320
        push   dword ptr [esp + 24h]
        call   sub_3_20D9
loc_3_0320:
        mov    eax, ebp
loc_3_0322:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 2ch
        ret    10h
loc_3_032C:
        mov    esi, 64h
loc_3_0331:
        mov    dword ptr [esp + 20h], esi
        push   0
        push   0
        push   0
        lea    eax, [esp + 2ch]
        push   0
        push   eax
        push   dword ptr [esp + 2ch]
        push   0
        push   dword ptr [esp + 30h]
        call   sub_3_242C
        add    esp, 20h
        or     eax, eax
        jne    SHORT loc_3_02FF
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        call   sub_3_2424
        add    esp, 8
        jmp    SHORT loc_3_0331
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_3_0370:
        mov    eax, dword ptr [esp + 14h]
        push   eax
        mov    ax, word ptr [esp + 14h]
        push   eax
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        push   0
        push   dword ptr [esp + 18h]
        call   sub_3_0391
        ret    14h
sub_3_0391:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        cmp    eax, 1
        sbb    edi, edi
        inc    edi
        mov    ebx, dword ptr [esp + 2ch]
        mov    ebp, dword ptr [esp + 20h]
        mov    word ptr [ebx], 5
        mov    esi, dword ptr [esp + 1ch]
        push   ebp
        push   esi
        push   eax
        call   sub_3_0D8B
        or     eax, eax
        je     SHORT loc_3_03CD
        mov    word ptr [esp + 12h], 0ffh
        mov    di, word ptr [esp + 28h]
        jmp    SHORT loc_3_03ED
loc_3_03CD:
        lea    eax, [esp + 12h]
        push   edi
        mov    di, word ptr [esp + 2ch]
        push   eax
        push   edi
        push   dword ptr [esp + 30h]
        push   80000002h
        push   ebp
        push   esi
        call   sub_3_0950
        or     eax, eax
        jne    SHORT loc_3_0401
loc_3_03ED:
        mov    ax, word ptr [esp + 12h]
        and    ax, di
        cmp    ax, di
        jne    SHORT loc_3_03FF
        mov    word ptr [ebx], 0
loc_3_03FF:
        sub    eax, eax
loc_3_0401:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    18h
        db  000h, 000h, 000h, 000h, 000h
sub_3_0410:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [esp + 20h], 1
        sbb    ebx, ebx
        inc    ebx
        call   sub_3_2070
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_3_0433
        mov    eax, 8
        jmp    NEAR PTR loc_3_051A
loc_3_0433:
        push   ebx
        push   esi
        lea    edi, [esi + 116h]
        push   dword ptr [esp + 2ch]
        call   sub_3_1595
        or     eax, eax
        je     SHORT loc_3_0452
        mov    edi, 8aeh
        jmp    NEAR PTR loc_3_050E
loc_3_0452:
        lea    eax, [esp + 0ch]
        push   eax
        push   esi
        push   80000002h
        call   sub_3_23FC
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_3_0473
        mov    edi, 8aeh
        jmp    NEAR PTR loc_3_050E
loc_3_0473:
        push   esi
        call   sub_3_0DD3
        push   0
        push   0
        push   0
        lea    eax, [esp + 20h]
        push   0
        push   eax
        push   0
        lea    eax, [esp + 30h]
        push   0
        push   eax
        push   0
        push   0
        push   0
        push   dword ptr [esp + 38h]
        call   sub_3_2444
        add    esp, 30h
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_3_0502
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_3_04E7
        mov    ebx, 64h
loc_3_04B4:
        mov    dword ptr [esp + 10h], ebx
        push   0
        push   0
        push   0
        lea    eax, [esp + 1ch]
        push   0
        push   eax
        push   edi
        push   0
        push   dword ptr [esp + 28h]
        call   sub_3_242C
        add    esp, 20h
        or     eax, eax
        jne    SHORT loc_3_04E7
        push   edi
        push   dword ptr [esp + 10h]
        call   sub_3_2424
        add    esp, 8
        jmp    SHORT loc_3_04B4
loc_3_04E7:
        push   dword ptr [esp + 0ch]
        call   sub_3_2404
        add    esp, 4
        cmp    dword ptr [esp + 14h], 1
        sbb    edi, edi
        and    edi, 8aeh
        jmp    SHORT loc_3_050E
loc_3_0502:
        push   dword ptr [esp + 0ch]
        push   esi
        call   sub_3_1B49
        sub    edi, edi
loc_3_050E:
        or     esi, esi
        je     SHORT loc_3_0518
        push   esi
        call   sub_3_20D9
loc_3_0518:
        mov    eax, edi
loc_3_051A:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret    8
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_0530:
        cmp    dword ptr [esp + 4], 1
        sbb    eax, eax
        inc    eax
        push   eax
        mov    eax, dword ptr [esp + 20h]
        push   dword ptr [esp + 24h]
        push   eax
        mov    ax, word ptr [esp + 24h]
        push   eax
        mov    eax, dword ptr [esp + 24h]
        push   eax
        mov    ax, word ptr [esp + 24h]
        push   eax
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 24h]
        push   0
        call   sub_3_0565
        ret    20h
sub_3_0565:
        sub    esp, 268h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 1ch], 0
        sub    esi, esi
        sub    edi, edi
        cmp    word ptr [esp + 288h], si
        jl     NEAR PTR loc_3_0833
        cmp    word ptr [esp + 288h], 2
        jg     NEAR PTR loc_3_0833
        call   sub_3_2070
        mov    dword ptr [esp + 18h], eax
        cmp    eax, esi
        jne    SHORT loc_3_05AF
        mov    eax, 8
        jmp    NEAR PTR loc_3_0838
loc_3_05AF:
        mov    ecx, dword ptr [esp + 298h]
        cmp    word ptr [esp + 284h], 0
        mov    word ptr [ecx], 0
        mov    ecx, dword ptr [esp + 294h]
        mov    eax, dword ptr [esp + 280h]
        mov    word ptr [ecx], 0
        jne    SHORT loc_3_05EC
        or     eax, eax
        je     SHORT loc_3_05E2
        cmp    byte ptr [eax], 0
        jne    SHORT loc_3_05EC
loc_3_05E2:
        mov    dword ptr [esp + 14h], 1
        jmp    SHORT loc_3_05F4
loc_3_05EC:
        mov    dword ptr [esp + 14h], 0
loc_3_05F4:
        push   dword ptr [esp + 29ch]
        push   dword ptr [esp + 1ch]
        push   eax
        call   sub_3_1595
        or     eax, eax
        je     SHORT loc_3_0613
        mov    esi, 57h
        jmp    NEAR PTR loc_3_0815
loc_3_0613:
        lea    ebx, [esp + 20h]
        push   dword ptr [esp + 18h]
        call   sub_3_2474
        mov    ecx, dword ptr [esp + 1ch]
        add    esp, 4
        add    ecx, eax
        mov    dword ptr [esp + 20h], ecx
        mov    dword ptr [esp + 28h], 0
        lea    eax, [esp + 24h]
        push   eax
        push   dword ptr [esp + 1ch]
        push   80000002h
        call   sub_3_23FC
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_3_0659
        mov    esi, 8aeh
        jmp    NEAR PTR loc_3_0815
loc_3_0659:
        call   sub_3_2070
        mov    edi, eax
        or     edi, edi
        jne    NEAR PTR loc_3_0761
        push   dword ptr [esp + 24h]
        call   sub_3_2404
        add    esp, 4
        mov    esi, 8
        jmp    NEAR PTR loc_3_0815
loc_3_067E:
        push   dword ptr [esp + 29ch]
        lea    eax, [esp + 294h]
        push   0
        push   eax
        lea    eax, [esp + 298h]
        push   eax
        push   dword ptr [esp + 298h]
        push   dword ptr [esp + 2ch]
        push   dword ptr [ebx + 4]
        call   sub_3_1851
loc_3_06AA:
        cmp    eax, 3
        je     SHORT loc_3_06D4
        mov    ecx, dword ptr [esp + 298h]
        inc    word ptr [ecx]
        cmp    eax, 2
        je     SHORT loc_3_06C8
        mov    ecx, dword ptr [esp + 294h]
        inc    word ptr [ecx]
loc_3_06C8:
        or     eax, eax
        je     SHORT loc_3_06D4
        mov    dword ptr [esp + 1ch], 1
loc_3_06D4:
        or     ebp, ebp
        je     NEAR PTR loc_3_07E1
loc_3_06DC:
        push   116h
        push   edi
        push   dword ptr [ebx + 8]
        push   dword ptr [ebx + 4]
        call   sub_3_241C
        add    esp, 10h
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_3_070E
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_3_070E
        push   0
        push   edi
        call   sub_3_14D0
        or     eax, eax
        je     SHORT loc_3_070E
        inc    dword ptr [ebx + 8]
        jmp    SHORT loc_3_06DC
loc_3_070E:
        or     ebp, ebp
        je     NEAR PTR loc_3_07E1
        or     esi, esi
        jne    NEAR PTR loc_3_07E1
        mov    eax, dword ptr [ebx]
        lea    ebp, [ebx + 0ch]
        mov    byte ptr [eax], 5ch
        push   -1
        push   edi
        mov    eax, dword ptr [ebx]
        inc    eax
        push   eax
        call   sub_3_246C
        add    esp, 0ch
        lea    eax, [ebp + 4]
        push   eax
        push   edi
        push   dword ptr [ebx + 4]
        call   sub_3_23FC
        add    esp, 0ch
        mov    esi, eax
        push   edi
        call   sub_3_2474
        add    esp, 4
        mov    ecx, dword ptr [ebx]
        add    ecx, eax
        inc    ecx
        mov    dword ptr [ebp], ecx
        mov    dword ptr [ebp + 8], 0
        mov    ebx, ebp
loc_3_0761:
        cmp    word ptr [esp + 284h], 0
        jne    SHORT loc_3_0786
        mov    eax, ebx
        lea    ecx, [esp + 20h]
        sub    eax, ecx
        mov    ecx, 0ch
        cdq
        idiv   ecx
        cmp    eax, 1
        mov    ebp, 0
        jge    SHORT loc_3_078B
loc_3_0786:
        mov    ebp, 1
loc_3_078B:
        cmp    dword ptr [ebx + 8], 0
        jne    NEAR PTR loc_3_06D4
        cmp    dword ptr [esp + 27ch], 0
        je     NEAR PTR loc_3_067E
        lea    eax, [esp + 12h]
        push   dword ptr [esp + 29ch]
        push   eax
        push   40h
        push   0
        mov    eax, dword ptr [esp + 28ch]
        push   dword ptr [ebx + 4]
        push   dword ptr [eax + 4]
        push   dword ptr [eax + 14h]
        call   sub_3_0950
        or     eax, eax
        jne    SHORT loc_3_07D7
        test   byte ptr [esp + 12h], 40h
        jne    NEAR PTR loc_3_067E
loc_3_07D7:
        mov    eax, 3
        jmp    NEAR PTR loc_3_06AA
loc_3_07E1:
        push   dword ptr [ebx + 4]
        call   sub_3_2404
        add    esp, 4
        mov    esi, eax
        lea    eax, [esp + 20h]
        cmp    ebx, eax
        jbe    SHORT loc_3_0807
        mov    eax, dword ptr [ebx - 0ch]
        sub    ebx, 0ch
        mov    byte ptr [eax], 0
        inc    dword ptr [ebx + 8]
        jmp    NEAR PTR loc_3_0761
loc_3_0807:
        cmp    dword ptr [esp + 1ch], 1
        cmc
        sbb    esi, esi
        and    esi, 0eah
loc_3_0815:
        or     edi, edi
        je     SHORT loc_3_081F
        push   edi
        call   sub_3_20D9
loc_3_081F:
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_3_082F
        push   dword ptr [esp + 18h]
        call   sub_3_20D9
loc_3_082F:
        mov    eax, esi
        jmp    SHORT loc_3_0838
loc_3_0833:
        mov    eax, 7ch
loc_3_0838:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 268h
        ret    24h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_0850:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        cmp    dword ptr [esp + 18h], 1
        sbb    ebp, ebp
        inc    ebp
        mov    si, word ptr [esp + 20h]
        cmp    si, 0ah
        je     SHORT loc_3_0872
        cmp    si, 0ch
        jne    SHORT loc_3_087B
loc_3_0872:
        mov    ebx, 1
        sub    si, 0ah
loc_3_087B:
        or     si, si
        jl     NEAR PTR loc_3_0932
        cmp    si, 2
        jg     NEAR PTR loc_3_0932
        call   sub_3_2070
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_3_08A3
        mov    eax, 8
        jmp    NEAR PTR loc_3_0937
loc_3_08A3:
        or     ebx, ebx
        push   ebp
        jne    SHORT loc_3_08DB
        push   edi
        push   dword ptr [esp + 24h]
        call   sub_3_1595
        or     eax, eax
        je     SHORT loc_3_08BD
        mov    esi, 8aeh
        jmp    SHORT loc_3_0924
loc_3_08BD:
        lea    eax, [esp + 10h]
        push   eax
        push   edi
        push   80000002h
        call   sub_3_23FC
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_3_08F5
        mov    esi, 8aeh
        jmp    SHORT loc_3_0924
loc_3_08DB:
        lea    eax, [esp + 14h]
        push   eax
        push   edi
        push   dword ptr [esp + 28h]
        call   sub_3_1781
        or     eax, eax
        je     SHORT loc_3_08F5
        mov    esi, 8aeh
        jmp    SHORT loc_3_0924
loc_3_08F5:
        push   ebp
        lea    eax, [esp + 2ch]
        push   dword ptr [esp + 30h]
        push   eax
        lea    eax, [esp + 30h]
        push   eax
        push   esi
        push   edi
        push   dword ptr [esp + 28h]
        call   sub_3_1851
        mov    esi, eax
        push   dword ptr [esp + 10h]
        call   sub_3_2404
        add    esp, 4
        db  08Bh, 034h, 0B5h, 080h, 000h, 000h, 000h
loc_3_0924:
        or     edi, edi
        je     SHORT loc_3_092E
        push   edi
        call   sub_3_20D9
loc_3_092E:
        mov    eax, esi
        jmp    SHORT loc_3_0937
loc_3_0932:
        mov    eax, 7ch
loc_3_0937:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    18h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_0950:
        sub    esp, 8ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0b4h]
        mov    dword ptr [esp + 1ch], 0
        mov    word ptr [edi], 0
        call   sub_3_2070
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_3_0983
        mov    eax, 8
        jmp    NEAR PTR loc_3_0CF1
loc_3_0983:
        lea    ebp, [esi + 116h]
        lea    ebx, [esi + 17ah]
        cmp    dword ptr [esp + 0a8h], 80000002h
        je     NEAR PTR loc_3_0A82
        push   0
        push   0
        push   0
        lea    eax, [esp + 40h]
        push   0
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   0
        push   0
        push   dword ptr [esp + 0d4h]
        call   sub_3_2444
        add    esp, 30h
        or     eax, eax
        jne    NEAR PTR loc_3_0A75
        cmp    dword ptr [esp + 34h], 0
        je     NEAR PTR loc_3_0A75
        mov    eax, dword ptr [esp + 0a8h]
        mov    dword ptr [esp + 18h], eax
loc_3_09E6:
        push   dword ptr [esp + 0b8h]
        push   ebp
        push   dword ptr [esp + 0ach]
        push   dword ptr [esp + 0ach]
        call   sub_3_1BE2
        mov    dword ptr [esp + 24h], eax
        cmp    dword ptr [esp + 0a8h], 80000002h
        jne    NEAR PTR loc_3_0ABE
        push   1
        db  0FFh, 035h, 0ECh, 002h, 000h, 000h
        call   sub_3_245C
        mov    ax, word ptr [esp + 0b8h]
        add    esp, 8
        db  066h, 023h, 005h, 068h, 001h, 000h, 000h
        cmp    ax, word ptr [esp + 0b0h]
        jne    SHORT loc_3_0AB0
        push   0
        db  068h, 070h, 001h, 000h, 000h
        lea    eax, [esi + 10h]
        push   eax
        call   FILESEC_Service_13
        or     eax, eax
        jne    SHORT loc_3_0AB0
        push   0
        db  068h, 088h, 002h, 000h, 000h
        push   dword ptr [esp + 2ch]
        call   FILESEC_Service_13
        or     eax, eax
        jne    SHORT loc_3_0AB0
        db  066h, 0A1h, 06Ch, 001h, 000h, 000h
        mov    word ptr [edi], ax
        jmp    NEAR PTR loc_3_0CBC
loc_3_0A75:
        mov    dword ptr [esp + 1ch], 0
        jmp    NEAR PTR loc_3_0CE3
loc_3_0A82:
        lea    eax, [esp + 18h]
        push   dword ptr [esp + 0b8h]
        push   eax
        push   esi
        push   dword ptr [esp + 0b8h]
        call   sub_3_1781
        or     eax, eax
        je     NEAR PTR loc_3_09E6
        mov    dword ptr [esp + 1ch], 0
        jmp    NEAR PTR loc_3_0CE3
loc_3_0AB0:
        db  0FFh, 035h, 0ECh, 002h, 000h, 000h
        call   sub_3_2464
        add    esp, 4
loc_3_0ABE:
        mov    dword ptr [esp + 14h], 2
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 16h]
        push   eax
        lea    eax, [esp + 28h]
        push   eax
        push   0
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 2ch]
        call   sub_3_2434
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_3_0AFC
        mov    ax, word ptr [esp + 12h]
        and    ax, 7fffh
        mov    word ptr [edi], ax
        jmp    NEAR PTR loc_3_0C62
loc_3_0AFC:
        db  068h, 0F4h, 000h, 000h, 000h
        push   dword ptr [esp + 28h]
        call   sub_3_2140
        or     eax, eax
        je     NEAR PTR loc_3_0C5D
        mov    dword ptr [esp + 14h], 2
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 16h]
        push   eax
        lea    eax, [esp + 28h]
        push   eax
        push   0
        db  068h, 0F4h, 000h, 000h, 000h
        push   dword ptr [esp + 2ch]
        call   sub_3_2434
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_3_0B61
        mov    ax, word ptr [esp + 12h]
        mov    word ptr [edi], ax
        mov    cx, word ptr [esp + 0b0h]
        and    cx, ax
        cmp    cx, word ptr [esp + 0b0h]
        je     NEAR PTR loc_3_0C5D
loc_3_0B61:
        call   sub_3_2070
        mov    dword ptr [esp + 28h], eax
        or     eax, eax
        jne    SHORT loc_3_0B7B
        mov    dword ptr [esp + 1ch], 8
        jmp    NEAR PTR loc_3_0CCA
loc_3_0B7B:
        mov    dword ptr [esp + 30h], 200h
        push   ebx
        push   dword ptr [esp + 28h]
        call   FILESEC_Service_16
        push   5ch
        push   ebx
        call   FILESEC_Service_14
        lea    ecx, [esp + 30h]
        push   ecx
        mov    byte ptr [eax], 0
        push   dword ptr [esp + 2ch]
        inc    eax
        push   eax
        push   ebx
        call   sub_3_00A6
        mov    dword ptr [esp + 2ch], eax
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_3_0C4C
        mov    eax, dword ptr [esp + 28h]
        mov    ebp, eax
        mov    ebx, dword ptr [eax]
        add    ebp, 4
        mov    eax, ebx
        dec    ebx
        or     eax, eax
        je     NEAR PTR loc_3_0C54
        mov    eax, dword ptr [esp + 0b8h]
        or     eax, 100h
        mov    dword ptr [esp + 2ch], eax
loc_3_0BDE:
        lea    eax, [esp + 38h]
        push   dword ptr [esp + 2ch]
        push   eax
        push   dword ptr [ebp]
        push   dword ptr [esp + 0ach]
        call   sub_3_1BE2
        mov    dword ptr [esp + 14h], 2
        lea    ecx, [esp + 14h]
        push   ecx
        lea    ecx, [esp + 16h]
        push   ecx
        lea    ecx, [esp + 28h]
        push   ecx
        push   0
        push   eax
        push   dword ptr [esp + 2ch]
        call   sub_3_2434
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_3_0C40
        mov    ax, word ptr [edi]
        or     ax, word ptr [esp + 12h]
        mov    word ptr [edi], ax
        mov    cx, word ptr [esp + 0b0h]
        and    cx, ax
        cmp    cx, word ptr [esp + 0b0h]
        je     SHORT loc_3_0C54
loc_3_0C40:
        add    ebp, 4
        mov    eax, ebx
        dec    ebx
        or     eax, eax
        jne    SHORT loc_3_0BDE
        jmp    SHORT loc_3_0C54
loc_3_0C4C:
        mov    dword ptr [esp + 1ch], 0
loc_3_0C54:
        push   dword ptr [esp + 28h]
        call   sub_3_20D9
loc_3_0C5D:
        and    word ptr [edi], 7fffh
loc_3_0C62:
        cmp    dword ptr [esp + 0a8h], 80000002h
        jne    SHORT loc_3_0CCA
        push   1
        db  0FFh, 035h, 0ECh, 002h, 000h, 000h
        call   sub_3_245C
        add    esp, 8
        push   -1
        lea    eax, [esi + 10h]
        push   eax
        db  068h, 070h, 001h, 000h, 000h
        call   sub_3_246C
        add    esp, 0ch
        push   -1
        push   dword ptr [esp + 28h]
        db  068h, 088h, 002h, 000h, 000h
        call   sub_3_246C
        mov    ax, word ptr [esp + 0bch]
        add    esp, 0ch
        db  066h, 0A3h, 068h, 001h, 000h, 000h
        mov    ax, word ptr [edi]
        db  066h, 0A3h, 06Ch, 001h, 000h, 000h
loc_3_0CBC:
        db  0FFh, 035h, 0ECh, 002h, 000h, 000h
        call   sub_3_2464
        add    esp, 4
loc_3_0CCA:
        mov    eax, dword ptr [esp + 0a8h]
        cmp    dword ptr [esp + 18h], eax
        je     SHORT loc_3_0CE3
        push   dword ptr [esp + 18h]
        call   sub_3_2404
        add    esp, 4
loc_3_0CE3:
        or     esi, esi
        je     SHORT loc_3_0CED
        push   esi
        call   sub_3_20D9
loc_3_0CED:
        mov    eax, dword ptr [esp + 1ch]
loc_3_0CF1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8ch
        ret    1ch
sub_3_0CFE:
        push   dword ptr [esp + 10h]
        push   1
        push   0
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   0
        push   dword ptr [esp + 1ch]
        call   sub_3_0D1C
        ret    10h
sub_3_0D1C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 14h]
        cmp    ecx, 1
        sbb    ebx, ebx
        inc    ebx
        mov    di, word ptr [esp + 24h]
        or     di, di
        jne    SHORT loc_3_0D38
        mov    di, 0ffh
loc_3_0D38:
        cmp    dword ptr [esp + 28h], 0
        je     SHORT loc_3_0D5E
        mov    ebp, dword ptr [esp + 1ch]
        mov    esi, dword ptr [esp + 18h]
        push   ebp
        push   esi
        push   ecx
        call   sub_3_0D8B
        or     eax, eax
        je     SHORT loc_3_0D66
        mov    ebx, dword ptr [esp + 2ch]
        sub    eax, eax
        mov    word ptr [ebx], di
        jmp    SHORT loc_3_0D84
loc_3_0D5E:
        mov    esi, dword ptr [esp + 18h]
        mov    ebp, dword ptr [esp + 1ch]
loc_3_0D66:
        push   ebx
        mov    ebx, dword ptr [esp + 30h]
        push   ebx
        push   edi
        push   dword ptr [esp + 2ch]
        push   80000002h
        push   ebp
        push   esi
        call   sub_3_0950
        or     eax, eax
        jne    SHORT loc_3_0D84
        and    word ptr [ebx], di
loc_3_0D84:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    1ch
sub_3_0D8B:
        sub    esp, 4
        cmp    dword ptr [esp + 8], 1
        sbb    eax, eax
        inc    eax
        push   eax
        lea    eax, [esp + 6]
        push   eax
        push   1
        db  068h, 0A4h, 000h, 000h, 000h
        push   80000002h
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 24h]
        call   sub_3_0950
        or     eax, eax
        je     SHORT loc_3_0DBD
        sub    eax, eax
        jmp    SHORT loc_3_0DCD
loc_3_0DBD:
        mov    ax, word ptr [esp + 2]
        and    ax, 1
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
loc_3_0DCD:
        add    esp, 4
        ret    0ch
sub_3_0DD3:
        push   1
        db  0FFh, 035h, 0ECh, 002h, 000h, 000h
        call   sub_3_245C
        add    esp, 8
        push   0
        mov    eax, dword ptr [esp + 8]
        db  068h, 070h, 001h, 000h, 000h
        add    eax, 10h
        push   eax
        call   FILESEC_Service_13
        or     eax, eax
        jne    SHORT loc_3_0E02
        db  0C6h, 005h, 070h, 001h, 000h, 000h, 000h
loc_3_0E02:
        db  0FFh, 035h, 0ECh, 002h, 000h, 000h
        call   sub_3_2464
        add    esp, 4
        ret    4
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_0E20:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 2ch]
        cmp    ebx, 0eah
        ja     SHORT loc_3_0E38
        mov    ebx, 0eah
loc_3_0E38:
        mov    eax, dword ptr [esp + 28h]
        push   200h
        add    ebx, dword ptr [eax]
        push   ebx
        call   sub_3_244C
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_3_0E59
        sub    eax, eax
        jmp    NEAR PTR loc_3_0EF7
loc_3_0E59:
        mov    edi, dword ptr [esp + 1ch]
        mov    esi, dword ptr [esp + 18h]
        mov    eax, dword ptr [edi]
        mov    ecx, dword ptr [esi]
        cmp    eax, ecx
        je     SHORT loc_3_0E7C
        sub    eax, ecx
        sar    eax, 3
        shl    eax, 3
        push   eax
        push   ecx
        push   ebp
        call   sub_3_247C
        add    esp, 0ch
loc_3_0E7C:
        mov    eax, dword ptr [esp + 24h]
        mov    edx, ebp
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [esp + 28h]
        mov    eax, dword ptr [eax]
        sub    eax, ecx
        add    eax, dword ptr [esi]
        sub    edx, eax
        add    edx, ebx
        mov    dword ptr [esp + 10h], edx
        or     eax, eax
        je     SHORT loc_3_0EA8
        push   eax
        push   ecx
        push   dword ptr [esp + 18h]
        call   sub_3_247C
        add    esp, 0ch
loc_3_0EA8:
        cmp    dword ptr [esp + 30h], 0
        je     SHORT loc_3_0EBB
        push   0
        push   dword ptr [esi]
        call   sub_3_2454
        add    esp, 8
loc_3_0EBB:
        mov    edx, ebp
        sub    edx, dword ptr [esi]
        mov    eax, dword ptr [esp + 24h]
        mov    ecx, dword ptr [esp + 10h]
        sub    ecx, dword ptr [eax]
        mov    dword ptr [esi], ebp
        mov    eax, dword ptr [esp + 20h]
        add    dword ptr [edi], edx
        add    dword ptr [eax], edx
        mov    eax, dword ptr [esp + 24h]
        mov    edx, dword ptr [esp + 10h]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [eax], ebx
        mov    eax, dword ptr [esi]
        cmp    dword ptr [edi], eax
        jbe    SHORT loc_3_0EF2
loc_3_0EE9:
        add    dword ptr [eax], ecx
        add    eax, 8
        cmp    dword ptr [edi], eax
        ja     SHORT loc_3_0EE9
loc_3_0EF2:
        mov    eax, 1
loc_3_0EF7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    1ch
sub_3_0F01:
        sub    esp, 54h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 40h], 0
        cmp    dword ptr [esp + 68h], 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esp + 50h], eax
        mov    dword ptr [esp + 20h], 0
        mov    dword ptr [esp + 58h], 0
        cmp    word ptr [esp + 70h], 0ch
        jne    SHORT loc_3_0F5D
        mov    word ptr [esp + 70h], 2
        mov    dword ptr [esp + 40h], 1
loc_3_0F43:
        mov    dx, word ptr [esp + 7ch]
        or     dx, dx
        je     SHORT loc_3_0F77
        cmp    dx, 2
        je     SHORT loc_3_0F7D
        mov    eax, 57h
        jmp    NEAR PTR loc_3_14BA
loc_3_0F5D:
        cmp    word ptr [esp + 70h], 1
        je     SHORT loc_3_0F43
        cmp    word ptr [esp + 70h], 2
        je     SHORT loc_3_0F43
        mov    eax, 7ch
        jmp    NEAR PTR loc_3_14BA
loc_3_0F77:
        cmp    dx, 2
        jne    SHORT loc_3_0F93
loc_3_0F7D:
        cmp    word ptr [esp + 78h], 2
        jae    NEAR PTR loc_3_1031
        mov    eax, 84bh
        jmp    NEAR PTR loc_3_14BA
loc_3_0F93:
        movsx  eax, word ptr [esp + 70h]
        lea    esi, [eax*4]
        movzx  ebp, word ptr [esp + 78h]
        db  08Bh, 08Eh, 0DCh, 000h, 000h, 000h
        cmp    ecx, ebp
        jbe    SHORT loc_3_0FB8
        mov    eax, 84bh
        jmp    NEAR PTR loc_3_14BA
loc_3_0FB8:
        mov    ebx, dword ptr [esp + 74h]
        db  08Bh, 0B6h, 0E8h, 000h, 000h, 000h
        mov    ax, word ptr [ebx + 6]
        movsx  edi, ax
        imul   esi, edi
        add    esi, ecx
        cmp    esi, ebp
        jbe    SHORT loc_3_0FDC
        mov    eax, 84bh
        jmp    NEAR PTR loc_3_14BA
loc_3_0FDC:
        cmp    word ptr [esp + 70h], 1
        jne    SHORT loc_3_0FF1
        mov    esi, dword ptr [esp + 74h]
        add    esi, 8
        mov    ebx, dword ptr [esp + 5ch]
        jmp    SHORT loc_3_0FFC
loc_3_0FF1:
        mov    ebx, dword ptr [esp + 74h]
        add    ebx, 8
        mov    esi, dword ptr [esp + 34h]
loc_3_0FFC:
        sub    ecx, ecx
        cmp    ax, cx
        jle    SHORT loc_3_1031
loc_3_1003:
        cmp    word ptr [esp + 70h], 1
        jne    SHORT loc_3_1011
        mov    ax, word ptr [esi + 16h]
        jmp    SHORT loc_3_1015
loc_3_1011:
        mov    ax, word ptr [ebx + 4]
loc_3_1015:
        movzx  ebp, ax
        test   ebp, 0ffff7f00h
        jne    SHORT loc_3_104A
        cmp    ax, 8000h
        je     SHORT loc_3_104A
        inc    ecx
        add    esi, 18h
        add    ebx, 6
        cmp    ecx, edi
        jl     SHORT loc_3_1003
loc_3_1031:
        cmp    dx, 2
        jne    SHORT loc_3_1054
        mov    ebx, dword ptr [esp + 74h]
        cmp    word ptr [ebx], 1
        cmc
        sbb    eax, eax
        and    eax, 57h
        jmp    NEAR PTR loc_3_14BA
loc_3_104A:
        mov    eax, 57h
        jmp    NEAR PTR loc_3_14BA
loc_3_1054:
        call   sub_3_2070
        mov    dword ptr [esp + 28h], eax
        or     eax, eax
        jne    SHORT loc_3_106B
        mov    eax, 8
        jmp    NEAR PTR loc_3_14BA
loc_3_106B:
        mov    eax, dword ptr [esp + 28h]
        add    eax, 116h
        mov    dword ptr [esp + 38h], eax
        mov    dword ptr [esp + 48h], 0eah
        add    eax, 0eah
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [esp + 38h]
        mov    dword ptr [esp + 20h], eax
        mov    dword ptr [esp + 14h], eax
        push   dword ptr [esp + 50h]
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 74h]
        call   sub_3_1595
        or     eax, eax
        je     SHORT loc_3_10AF
        jmp    NEAR PTR loc_3_146C
loc_3_10AF:
        lea    eax, [esp + 2ch]
        push   eax
        push   dword ptr [esp + 2ch]
        push   80000002h
        call   sub_3_23FC
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_3_10CE
        jmp    NEAR PTR loc_3_146C
loc_3_10CE:
        mov    dword ptr [esp + 58h], 1
        push   0
        push   0
        push   0
        lea    eax, [esp + 60h]
        push   0
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   0
        push   0
        push   dword ptr [esp + 58h]
        call   sub_3_2444
        add    esp, 30h
        or     eax, eax
        jne    NEAR PTR loc_3_146C
        cmp    dword ptr [esp + 54h], 0
        je     NEAR PTR loc_3_146C
        push   dword ptr [esp + 28h]
        call   sub_3_0DD3
        sub    esi, esi
        mov    edi, 116h
        mov    ebx, 2
loc_3_1123:
        mov    dword ptr [esp + 24h], edi
        mov    dword ptr [esp + 4ch], ebx
        lea    eax, [esp + 4ch]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 68h]
        push   eax
        lea    eax, [esp + 30h]
        push   0
        push   eax
        push   dword ptr [esp + 3ch]
        push   esi
        push   dword ptr [esp + 48h]
        call   sub_3_242C
        add    esp, 20h
        mov    ebp, eax
        or     ebp, ebp
        jne    NEAR PTR loc_3_121B
        inc    dword ptr [esp + 24h]
        mov    eax, dword ptr [esp + 14h]
        sub    eax, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [esp + 24h]
        lea    ecx, [eax + ecx + 8]
        or     ecx, ecx
        jle    SHORT loc_3_11B2
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [esp + 20h]
        sub    eax, dword ptr [esp + 38h]
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        push   eax
        lea    eax, [esp + 4ch]
        push   ecx
        push   eax
        lea    eax, [esp + 28h]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        lea    eax, [esp + 2ch]
        push   eax
        lea    eax, [esp + 38h]
        push   eax
        call   sub_3_0E20
        or     eax, eax
        je     NEAR PTR loc_3_126F
loc_3_11B2:
        mov    eax, dword ptr [esp + 24h]
        sub    dword ptr [esp + 1ch], eax
        mov    ecx, dword ptr [esp + 14h]
        mov    eax, dword ptr [esp + 1ch]
        push   -1
        mov    dword ptr [ecx], eax
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 24h]
        call   sub_3_246C
        mov    ax, word ptr [esp + 1ch]
        mov    ecx, dword ptr [esp + 20h]
        add    esp, 0ch
        mov    word ptr [ecx + 4], ax
        cmp    dword ptr [esp + 40h], 0
        je     SHORT loc_3_11F9
        mov    ecx, dword ptr [esp + 14h]
        mov    ax, word ptr [esp + 10h]
        mov    word ptr [ecx + 6], ax
        jmp    SHORT loc_3_1203
loc_3_11F9:
        mov    eax, dword ptr [esp + 14h]
        mov    word ptr [eax + 6], 0ffffh
loc_3_1203:
        mov    eax, dword ptr [esp + 14h]
        add    eax, 6
        cmp    word ptr [eax], 8000h
        jne    SHORT loc_3_1216
        mov    word ptr [eax], 0ffffh
loc_3_1216:
        add    dword ptr [esp + 14h], 8
loc_3_121B:
        inc    esi
        or     ebp, ebp
        je     NEAR PTR loc_3_1123
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esp + 18h], eax
        mov    dword ptr [esp + 30h], 0
        mov    dword ptr [esp + 44h], 0
        mov    edi, dword ptr [esp + 74h]
        add    edi, 8
        mov    ebx, dword ptr [esp + 74h]
        mov    dword ptr [esp + 34h], edi
        cmp    word ptr [ebx + 6], 0
        jle    NEAR PTR loc_3_139B
        mov    esi, 8
loc_3_125B:
        cmp    word ptr [esp + 70h], 1
        jne    SHORT loc_3_127C
        mov    ecx, dword ptr [esp + 34h]
        mov    eax, ecx
        mov    bx, word ptr [eax + 16h]
        jmp    SHORT loc_3_1282
loc_3_126F:
        mov    dword ptr [esp + 30h], 8
        jmp    NEAR PTR loc_3_1474
loc_3_127C:
        mov    ecx, dword ptr [edi]
        mov    bx, word ptr [edi + 4]
loc_3_1282:
        push   dword ptr [esp + 50h]
        push   dword ptr [esp + 2ch]
        push   ecx
        push   0
        call   sub_3_1BE2
        mov    ebp, eax
        cmp    dword ptr [esp + 40h], 0
        je     SHORT loc_3_12A4
        or     bx, bx
        jne    SHORT loc_3_12A4
        mov    bx, 0ffffh
loc_3_12A4:
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [esp + 14h], eax
        cmp    dword ptr [esp + 18h], eax
        jbe    SHORT loc_3_12DC
loc_3_12B2:
        push   0
        mov    eax, dword ptr [esp + 18h]
        push   ebp
        push   dword ptr [eax]
        call   FILESEC_Service_13
        or     eax, eax
        je     SHORT loc_3_12D4
        add    dword ptr [esp + 14h], esi
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [esp + 18h], eax
        ja     SHORT loc_3_12B2
        jmp    SHORT loc_3_12DC
loc_3_12D4:
        mov    eax, dword ptr [esp + 14h]
        mov    word ptr [eax + 6], bx
loc_3_12DC:
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [esp + 18h], eax
        ja     NEAR PTR loc_3_137D
        push   ebp
        call   sub_3_2474
        add    esp, 4
        inc    eax
        mov    dword ptr [esp + 3ch], eax
        sub    eax, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [esp + 14h]
        lea    ecx, [eax + ecx + 8]
        or     ecx, ecx
        jle    SHORT loc_3_1346
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [esp + 20h]
        sub    eax, dword ptr [esp + 38h]
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        push   eax
        lea    eax, [esp + 4ch]
        push   ecx
        push   eax
        lea    eax, [esp + 28h]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        lea    eax, [esp + 2ch]
        push   eax
        lea    eax, [esp + 38h]
        push   eax
        call   sub_3_0E20
        or     eax, eax
        je     NEAR PTR loc_3_13DB
loc_3_1346:
        mov    eax, dword ptr [esp + 3ch]
        sub    dword ptr [esp + 1ch], eax
        mov    ecx, dword ptr [esp + 14h]
        mov    eax, dword ptr [esp + 1ch]
        push   -1
        push   ebp
        mov    dword ptr [ecx], eax
        push   dword ptr [esp + 24h]
        call   sub_3_246C
        mov    eax, dword ptr [esp + 20h]
        add    esp, 0ch
        mov    word ptr [eax + 4], 0ffffh
        mov    eax, dword ptr [esp + 14h]
        mov    word ptr [eax + 6], bx
        add    dword ptr [esp + 18h], esi
loc_3_137D:
        inc    dword ptr [esp + 44h]
        add    dword ptr [esp + 34h], 18h
        mov    ebx, dword ptr [esp + 74h]
        add    edi, 6
        movsx  eax, word ptr [ebx + 6]
        cmp    eax, dword ptr [esp + 44h]
        jg     NEAR PTR loc_3_125B
loc_3_139B:
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [esp + 14h], eax
        cmp    dword ptr [esp + 18h], eax
        jbe    SHORT loc_3_1413
        mov    esi, 8
loc_3_13AE:
        mov    ecx, dword ptr [esp + 14h]
        mov    ax, word ptr [ecx + 6]
        add    ecx, 6
        mov    edx, dword ptr [esp + 14h]
        cmp    word ptr [edx + 4], ax
        je     SHORT loc_3_1405
        cmp    ax, 0ffffh
        jne    SHORT loc_3_13E8
        mov    eax, edx
        push   dword ptr [eax]
        push   dword ptr [esp + 30h]
        call   sub_3_2424
        add    esp, 8
        jmp    SHORT loc_3_1401
loc_3_13DB:
        mov    dword ptr [esp + 30h], 8
        jmp    NEAR PTR loc_3_1474
loc_3_13E8:
        push   2
        push   ecx
        push   3
        mov    eax, dword ptr [esp + 20h]
        push   0
        push   dword ptr [eax]
        push   dword ptr [esp + 40h]
        call   sub_3_243C
        add    esp, 18h
loc_3_1401:
        mov    dword ptr [esp + 30h], eax
loc_3_1405:
        add    dword ptr [esp + 14h], esi
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [esp + 18h], eax
        ja     SHORT loc_3_13AE
loc_3_1413:
        push   0
        push   0
        push   0
        lea    eax, [esp + 60h]
        push   0
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   0
        push   0
        push   dword ptr [esp + 58h]
        call   sub_3_2444
        add    esp, 30h
        or     eax, eax
        jne    SHORT loc_3_1474
        cmp    dword ptr [esp + 54h], 0
        jne    SHORT loc_3_1474
        mov    word ptr [esp + 12h], 8000h
        lea    eax, [esp + 12h]
        push   2
        push   eax
        push   3
        push   0
        db  068h, 0F4h, 000h, 000h, 000h
        push   dword ptr [esp + 40h]
        call   sub_3_243C
        mov    dword ptr [esp + 48h], eax
        add    esp, 18h
        jmp    SHORT loc_3_1474
loc_3_146C:
        mov    dword ptr [esp + 30h], 8aeh
loc_3_1474:
        cmp    dword ptr [esp + 20h], 0
        je     SHORT loc_3_1493
        mov    eax, dword ptr [esp + 38h]
        cmp    dword ptr [esp + 20h], eax
        je     SHORT loc_3_1493
        push   0
        push   dword ptr [esp + 24h]
        call   sub_3_2454
        add    esp, 8
loc_3_1493:
        cmp    dword ptr [esp + 28h], 0
        je     SHORT loc_3_14A3
        push   dword ptr [esp + 28h]
        call   sub_3_20D9
loc_3_14A3:
        cmp    dword ptr [esp + 58h], 0
        je     SHORT loc_3_14B6
        push   dword ptr [esp + 2ch]
        call   sub_3_2404
        add    esp, 4
loc_3_14B6:
        mov    eax, dword ptr [esp + 30h]
loc_3_14BA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 54h
        ret    18h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_14D0:
        mov    eax, dword ptr [esp + 4]
        mov    cl, byte ptr [eax]
        cmp    cl, 41h
        jb     SHORT loc_3_14E0
        cmp    cl, 5ah
        jbe    SHORT loc_3_14EA
loc_3_14E0:
        cmp    cl, 61h
        jb     SHORT loc_3_1504
        cmp    cl, 7ah
        ja     SHORT loc_3_1504
loc_3_14EA:
        cmp    byte ptr [eax + 1], 3ah
        jne    SHORT loc_3_1504
        cmp    dword ptr [esp + 8], 0
        je     SHORT loc_3_14FD
        cmp    byte ptr [eax + 2], 5ch
        jne    SHORT loc_3_1504
loc_3_14FD:
        mov    eax, 1
        jmp    SHORT loc_3_1506
loc_3_1504:
        sub    eax, eax
loc_3_1506:
        ret    8
sub_3_1509:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 1
        mov    dword ptr [ebp - 0ch], eax
        or     eax, eax
        je     SHORT loc_3_1547
        call   sub_3_2484
        mov    ecx, eax
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ecx]
        mov    ecx, eax
        and    ecx, 30000h
        mov    dword ptr [ebp - 10h], ecx
        mov    ecx, dword ptr [ebp - 8]
        and    eax, 0fffcffffh
        mov    dword ptr [ecx], eax
loc_3_1547:
        mov    eax, 7160h
        mov    ecx, 2
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        push   21h
        xor    ebx, ebx
        xor    edx, edx
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jae    SHORT loc_3_1568
        mov    dword ptr [ebp - 4], eax
loc_3_1568:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_3_1578
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 10h]
        or     dword ptr [ecx], eax
        jmp    SHORT loc_3_1589
loc_3_1578:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_3_1589
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 0ch]
        call   FILESEC_Service_15
loc_3_1589:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_3_1595:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 1ch]
        or     edi, edi
        je     SHORT loc_3_15AE
        cmp    byte ptr [edi], 0
        mov    ebx, 0
        jne    SHORT loc_3_15B3
loc_3_15AE:
        mov    ebx, 1
loc_3_15B3:
        mov    dword ptr [esp + 14h], 0
        mov    esi, dword ptr [esp + 24h]
        mov    ebp, esi
        and    ebp, 100h
        je     SHORT loc_3_15DB
        test   esi, 200h
        je     SHORT loc_3_15DB
        mov    eax, 57h
        jmp    NEAR PTR loc_3_1777
loc_3_15DB:
        or     ebx, ebx
        jne    SHORT loc_3_1604
        cmp    byte ptr [edi], 5ch
        jne    SHORT loc_3_15EA
        cmp    byte ptr [edi + 1], 5ch
        jne    SHORT loc_3_1604
loc_3_15EA:
        push   1
        push   edi
        call   sub_3_14D0
        mov    dword ptr [esp + 14h], eax
        or     eax, eax
        jne    SHORT loc_3_1604
        mov    eax, 57h
        jmp    NEAR PTR loc_3_1777
loc_3_1604:
        push   -1
        db  068h, 0C8h, 000h, 000h, 000h
        push   dword ptr [esp + 28h]
        call   sub_3_246C
        add    esp, 0ch
        or     ebx, ebx
        jne    NEAR PTR loc_3_1746
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_3_1651
        test   esi, 1
        mov    eax, dword ptr [esp + 20h]
        je     SHORT loc_3_1647
        add    eax, 10h
        push   -1
        inc    edi
        push   edi
        push   eax
        call   sub_3_246C
        add    esp, 0ch
        jmp    NEAR PTR loc_3_1746
loc_3_1647:
        add    eax, 10h
        push   eax
        inc    edi
        jmp    NEAR PTR loc_3_1740
loc_3_1651:
        mov    ebx, esi
        and    ebx, 0ffh
        mov    eax, dword ptr [esp + 20h]
        add    eax, 10h
        mov    dword ptr [esp + 10h], eax
        push   ebx
        push   dword ptr [esp + 14h]
        push   edi
        call   sub_3_1509
        or     eax, eax
        je     NEAR PTR loc_3_1746
        test   esi, 200h
        je     SHORT loc_3_1689
        mov    eax, 8aeh
        jmp    NEAR PTR loc_3_1777
loc_3_1689:
        or     ebp, ebp
        je     NEAR PTR loc_3_1723
        push   edi
        call   sub_3_2474
        add    esp, 4
        mov    ebp, eax
        cmp    ebp, 3
        ja     SHORT loc_3_16AB
        mov    eax, 8aeh
        jmp    NEAR PTR loc_3_1777
loc_3_16AB:
        call   sub_3_2070
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_3_16C0
        mov    eax, 8
        jmp    NEAR PTR loc_3_1777
loc_3_16C0:
        inc    ebp
        push   ebp
        push   edi
        push   esi
        call   sub_3_247C
        add    esp, 0ch
        push   5ch
        push   esi
        call   FILESEC_Service_14
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_3_1712
loc_3_16DA:
        mov    eax, edi
        sub    eax, esi
        sub    eax, 2
        cmp    eax, 1
        sbb    ebp, ebp
        neg    ebp
        push   ebx
        mov    byte ptr [edi + ebp], 0
        push   dword ptr [esp + 14h]
        push   esi
        call   sub_3_1509
        or     eax, eax
        je     SHORT loc_3_1712
        or     ebp, ebp
        mov    edi, 0
        jne    SHORT loc_3_170E
        push   5ch
        push   esi
        call   FILESEC_Service_14
        mov    edi, eax
loc_3_170E:
        or     edi, edi
        jne    SHORT loc_3_16DA
loc_3_1712:
        push   esi
        call   sub_3_20D9
        or     edi, edi
        jne    SHORT loc_3_1746
        mov    eax, 8aeh
        jmp    SHORT loc_3_1777
loc_3_1723:
        test   esi, 1
        je     SHORT loc_3_173C
        push   -1
        push   edi
        push   dword ptr [esp + 18h]
        call   sub_3_246C
        add    esp, 0ch
        jmp    SHORT loc_3_1746
loc_3_173C:
        push   dword ptr [esp + 10h]
loc_3_1740:
        push   edi
        call   FILESEC_Service_15
loc_3_1746:
        push   dword ptr [esp + 20h]
        call   sub_3_2474
        add    esp, 4
        mov    edi, eax
        push   5ch
        mov    esi, edi
        push   dword ptr [esp + 24h]
        call   FILESEC_Service_14
        sub    esi, eax
        add    esi, dword ptr [esp + 20h]
        cmp    esi, 1
        jne    SHORT loc_3_1775
        mov    eax, dword ptr [esp + 20h]
        mov    byte ptr [edi + eax - 1], 0
loc_3_1775:
        sub    eax, eax
loc_3_1777:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    0ch
sub_3_1781:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 28h]
        or     eax, 100h
        mov    edi, dword ptr [esp + 20h]
        push   eax
        push   edi
        sub    esi, esi
        push   dword ptr [esp + 24h]
        call   sub_3_1595
        cmp    eax, esi
        je     SHORT loc_3_17B0
        mov    eax, 8aeh
        jmp    NEAR PTR loc_3_1847
loc_3_17B0:
        lea    ebx, [edi + 0fh]
        push   edi
        call   sub_3_2474
        add    esp, 4
        add    eax, edi
        cmp    eax, ebx
        jbe    SHORT loc_3_1832
loc_3_17C2:
        sub    ebp, ebp
        lea    eax, [esp + 10h]
        push   eax
        push   edi
        push   80000002h
        call   sub_3_23FC
        add    esp, 0ch
        cmp    eax, ebp
        jne    SHORT loc_3_180C
        push   0
        push   0
        push   0
        lea    eax, [esp + 20h]
        push   0
        mov    ebp, 1
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   0
        push   0
        push   dword ptr [esp + 3ch]
        call   sub_3_2444
        add    esp, 30h
        cmp    dword ptr [esp + 14h], 0
        ja     SHORT loc_3_182D
loc_3_180C:
        or     ebp, ebp
        je     SHORT loc_3_181C
        push   dword ptr [esp + 10h]
        call   sub_3_2404
        add    esp, 4
loc_3_181C:
        push   5ch
        push   ebx
        call   FILESEC_Service_14
        mov    byte ptr [eax], 0
        cmp    eax, ebx
        ja     SHORT loc_3_17C2
        jmp    SHORT loc_3_1832
loc_3_182D:
        mov    esi, 1
loc_3_1832:
        or     esi, esi
        mov    eax, 3
        je     SHORT loc_3_1847
        mov    eax, dword ptr [esp + 24h]
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax], ecx
        sub    eax, eax
loc_3_1847:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    10h
sub_3_1851:
        sub    esp, 3ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 14h], 0
        push   0
        push   0
        lea    eax, [esp + 3ch]
        push   0
        push   eax
        lea    eax, [esp + 30h]
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   0
        push   0
        push   dword ptr [esp + 7ch]
        call   sub_3_2444
        mov    dword ptr [esp + 48h], eax
        add    esp, 30h
        or     eax, eax
        jne    NEAR PTR loc_3_1B3A
        cmp    dword ptr [esp + 20h], 0
        je     NEAR PTR loc_3_1B3A
        cmp    dword ptr [esp + 20h], 1
        jne    SHORT loc_3_18E5
        mov    dword ptr [esp + 30h], 2
        lea    eax, [esp + 30h]
        push   eax
        lea    eax, [esp + 16h]
        push   eax
        lea    eax, [esp + 40h]
        push   eax
        push   0
        db  068h, 0F4h, 000h, 000h, 000h
        push   dword ptr [esp + 64h]
        call   sub_3_2434
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_3_18E5
        cmp    word ptr [esp + 12h], 8000h
        jne    SHORT loc_3_18E5
        mov    dword ptr [esp + 20h], 0
loc_3_18E5:
        movsx  eax, word ptr [esp + 58h]
        shl    eax, 2
        mov    ecx, dword ptr [esp + 60h]
        db  08Bh, 090h, 0E8h, 000h, 000h, 000h
        imul   dx, word ptr [esp + 20h]
        db  066h, 003h, 090h, 0DCh, 000h, 000h, 000h
        cmp    word ptr [ecx], dx
        jae    SHORT loc_3_1922
        mov    dword ptr [esp + 14h], 2
        cmp    dword ptr [esp + 64h], 0
        jne    SHORT loc_3_1922
        mov    eax, 2
        jmp    NEAR PTR loc_3_1B3F
loc_3_1922:
        cmp    dword ptr [esp + 64h], 0
        je     SHORT loc_3_1930
        mov    ecx, dword ptr [esp + 64h]
        mov    word ptr [ecx], dx
loc_3_1930:
        cmp    dword ptr [esp + 14h], 2
        je     SHORT loc_3_1956
        mov    eax, dword ptr [esp + 5ch]
        mov    edi, dword ptr [eax]
        movzx  eax, dx
        lea    ecx, [edi + eax]
        mov    dword ptr [esp + 28h], ecx
        mov    ecx, dword ptr [esp + 60h]
        movzx  eax, word ptr [ecx]
        add    eax, edi
        mov    dword ptr [esp + 2ch], eax
        jmp    SHORT loc_3_195A
loc_3_1956:
        mov    edi, dword ptr [esp + 3ch]
loc_3_195A:
        cmp    dword ptr [esp + 64h], 0
        mov    ebx, dword ptr [esp + 54h]
        je     SHORT loc_3_1988
        lea    ebp, [ebx + 10h]
        push   ebp
        call   sub_3_2474
        add    esp, 4
        lea    esi, [eax + 1]
        push   1
        push   ebp
        call   sub_3_14D0
        or     eax, eax
        jne    SHORT loc_3_1981
        inc    esi
loc_3_1981:
        mov    ecx, dword ptr [esp + 64h]
        add    word ptr [ecx], si
loc_3_1988:
        call   sub_3_2070
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_3_1998
        jmp    NEAR PTR loc_3_1B3A
loc_3_1998:
        cmp    dword ptr [esp + 14h], 2
        je     SHORT loc_3_19F9
        mov    ebp, esi
        add    ebx, 10h
        push   0
        push   ebx
        call   sub_3_14D0
        or     eax, eax
        jne    SHORT loc_3_19B6
        mov    byte ptr [esi], 5ch
        lea    ebp, [esi + 1]
loc_3_19B6:
        push   -1
        push   ebx
        push   ebp
        call   sub_3_246C
        add    esp, 0ch
        cmp    byte ptr [esi], 5ch
        je     SHORT loc_3_19D5
        cmp    byte ptr [esi + 2], 0
        jne    SHORT loc_3_19D5
        mov    byte ptr [esi + 2], 5ch
        mov    byte ptr [esi + 3], 0
loc_3_19D5:
        lea    eax, [esp + 2ch]
        push   dword ptr [esp + 68h]
        push   eax
        mov    dword ptr [edi], esi
        push   dword ptr [esp + 30h]
        push   edi
        call   sub_3_2000
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_3_19F9
        mov    dword ptr [esp + 14h], 1
loc_3_19F9:
        cmp    word ptr [esp + 58h], 0
        jle    NEAR PTR loc_3_1B10
        cmp    dword ptr [esp + 14h], 2
        je     SHORT loc_3_1A21
        mov    word ptr [edi + 4], 0
        mov    eax, dword ptr [esp + 20h]
        mov    word ptr [edi + 6], ax
        add    edi, 8
        mov    ebp, edi
        jmp    SHORT loc_3_1A29
loc_3_1A21:
        mov    ebp, dword ptr [esp + 40h]
        mov    edi, dword ptr [esp + 44h]
loc_3_1A29:
        sub    ebx, ebx
        cmp    dword ptr [esp + 20h], ebx
        jle    NEAR PTR loc_3_1B10
loc_3_1A35:
        cmp    dword ptr [esp + 18h], 0
        jne    NEAR PTR loc_3_1B10
        mov    dword ptr [esp + 1ch], 64h
        mov    dword ptr [esp + 24h], 2
        lea    eax, [esp + 24h]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 50h]
        push   eax
        lea    eax, [esp + 28h]
        push   0
        push   eax
        push   esi
        push   ebx
        push   dword ptr [esp + 6ch]
        call   sub_3_242C
        mov    dword ptr [esp + 38h], eax
        add    esp, 20h
        or     eax, eax
        jne    NEAR PTR loc_3_1B05
        cmp    dword ptr [esp + 64h], 0
        je     SHORT loc_3_1A9C
        cmp    word ptr [esp + 58h], 2
        jne    SHORT loc_3_1A9C
        mov    eax, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [esp + 64h]
        inc    ax
        add    word ptr [ecx], ax
loc_3_1A9C:
        cmp    dword ptr [esp + 14h], 2
        je     SHORT loc_3_1B05
        cmp    word ptr [esp + 58h], 1
        jne    SHORT loc_3_1AD5
        test   byte ptr [esp + 68h], 1
        je     SHORT loc_3_1AC0
        push   -1
        push   esi
        push   ebp
        call   sub_3_246C
        add    esp, 0ch
        jmp    SHORT loc_3_1AC7
loc_3_1AC0:
        push   ebp
        push   esi
        call   FILESEC_Service_16
loc_3_1AC7:
        mov    ax, word ptr [esp + 10h]
        mov    word ptr [ebp + 16h], ax
        add    ebp, 18h
        jmp    SHORT loc_3_1B05
loc_3_1AD5:
        mov    dword ptr [edi], esi
        mov    ax, word ptr [esp + 10h]
        push   dword ptr [esp + 68h]
        mov    word ptr [edi + 4], ax
        lea    eax, [esp + 30h]
        push   eax
        push   dword ptr [esp + 30h]
        push   edi
        call   sub_3_2000
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_3_1B02
        mov    dword ptr [esp + 14h], 1
loc_3_1B02:
        add    edi, 6
loc_3_1B05:
        inc    ebx
        cmp    ebx, dword ptr [esp + 20h]
        jl     NEAR PTR loc_3_1A35
loc_3_1B10:
        cmp    dword ptr [esp + 14h], 2
        je     SHORT loc_3_1B2E
        mov    eax, dword ptr [esp + 5ch]
        mov    ecx, dword ptr [esp + 28h]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 2ch]
        sub    eax, ecx
        mov    ecx, dword ptr [esp + 60h]
        mov    word ptr [ecx], ax
loc_3_1B2E:
        push   esi
        call   sub_3_20D9
        mov    eax, dword ptr [esp + 14h]
        jmp    SHORT loc_3_1B3F
loc_3_1B3A:
        mov    eax, 3
loc_3_1B3F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret    1ch
sub_3_1B49:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 1ch]
        sub    ebx, ebx
        lea    esi, [edi + 0fh]
loc_3_1B59:
        push   dword ptr [esp + 20h]
        call   sub_3_2404
        add    esp, 4
        mov    ebp, ebx
        push   edi
        push   80000002h
        call   sub_3_2414
        add    esp, 8
        push   5ch
        push   esi
        call   FILESEC_Service_14
        cmp    eax, esi
        jbe    SHORT loc_3_1BC6
        mov    byte ptr [eax], bl
        lea    eax, [esp + 20h]
        push   eax
        push   edi
        push   80000002h
        call   sub_3_23FC
        add    esp, 0ch
        push   ebx
        push   ebx
        push   ebx
        lea    eax, [esp + 1ch]
        push   ebx
        mov    ebp, 1
        push   eax
        push   ebx
        lea    eax, [esp + 2ch]
        push   ebx
        push   eax
        push   ebx
        push   ebx
        push   ebx
        push   dword ptr [esp + 4ch]
        call   sub_3_2444
        add    esp, 30h
        cmp    dword ptr [esp + 14h], ebx
        jne    SHORT loc_3_1BC6
        cmp    dword ptr [esp + 10h], ebx
        je     SHORT loc_3_1B59
loc_3_1BC6:
        or     ebp, ebp
        je     SHORT loc_3_1BD6
        push   dword ptr [esp + 20h]
        call   sub_3_2404
        add    esp, 4
loc_3_1BD6:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    8
sub_3_1BE2:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        or     ebp, ebp
        je     SHORT loc_3_1BF9
        cmp    byte ptr [ebp], 0
        mov    edi, 0
        jne    SHORT loc_3_1BFE
loc_3_1BF9:
        mov    edi, 1
loc_3_1BFE:
        mov    esi, dword ptr [esp + 18h]
        db  068h, 0F4h, 000h, 000h, 000h
        push   esi
        call   sub_3_2140
        or     eax, eax
        je     SHORT loc_3_1C8C
        push   5ch
        push   esi
        call   FILESEC_Service_14
        or     eax, eax
        jne    SHORT loc_3_1C8C
        or     edi, edi
        jne    SHORT loc_3_1C2E
        push   ebp
        call   sub_3_2474
        add    esp, 4
        mov    ebx, eax
        jmp    SHORT loc_3_1C3A
loc_3_1C2E:
        db  0BDh, 0F0h, 002h, 000h, 000h
        db  08Bh, 01Dh, 070h, 003h, 000h, 000h
        dec    ebx
loc_3_1C3A:
        mov    edi, dword ptr [esp + 20h]
        test   edi, 1
        jne    SHORT loc_3_1C5A
        db  081h, 0FDh, 0F0h, 002h, 000h, 000h
        je     SHORT loc_3_1C5A
        push   dword ptr [esp + 1ch]
        push   ebp
        call   FILESEC_Service_15
        jmp    SHORT loc_3_1C68
loc_3_1C5A:
        push   ebx
        push   ebp
        push   dword ptr [esp + 24h]
        call   sub_3_247C
        add    esp, 0ch
loc_3_1C68:
        mov    eax, dword ptr [esp + 1ch]
        add    eax, ebx
        test   edi, 101h
        mov    byte ptr [eax], 5ch
        je     SHORT loc_3_1C88
        push   -1
        push   esi
        inc    eax
        push   eax
        call   sub_3_246C
        add    esp, 0ch
        jmp    SHORT loc_3_1CA2
loc_3_1C88:
        inc    eax
        push   eax
        jmp    SHORT loc_3_1C9C
loc_3_1C8C:
        test   dword ptr [esp + 20h], 101h
        mov    eax, esi
        jne    SHORT loc_3_1CA6
        push   dword ptr [esp + 1ch]
loc_3_1C9C:
        push   esi
        call   FILESEC_Service_15
loc_3_1CA2:
        mov    eax, dword ptr [esp + 1ch]
loc_3_1CA6:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
        db  000h, 000h, 000h
sub_3_1CB0:
        sub    esp, 4
        mov    eax, dword ptr [esp + 8]
        test   byte ptr [eax + 11h], 1
        jne    SHORT loc_3_1CF3
        mov    ecx, dword ptr [esp + 0ch]
        or     ecx, ecx
        je     SHORT loc_3_1CEF
        cmp    byte ptr [ecx], 0
        je     SHORT loc_3_1CEF
        lea    edx, [esp + 2]
        push   0
        push   edx
        push   40h
        push   ecx
        push   80000002h
        push   dword ptr [eax + 4]
        push   dword ptr [eax + 14h]
        call   sub_3_0950
        or     eax, eax
        jne    SHORT loc_3_1CEF
        test   byte ptr [esp + 2], 40h
        jne    SHORT loc_3_1CF3
loc_3_1CEF:
        sub    eax, eax
        jmp    SHORT loc_3_1CF8
loc_3_1CF3:
        mov    eax, 1
loc_3_1CF8:
        add    esp, 4
        ret    8
sub_3_1CFE:
        mov    eax, dword ptr [esp + 4]
        lea    ecx, [eax + 8]
        movsx  eax, word ptr [eax + 6]
        or     eax, eax
        jle    SHORT loc_3_1D1A
loc_3_1D0D:
        cmp    dword ptr [ecx], 0
        je     SHORT loc_3_1D22
        dec    eax
        add    ecx, 6
        or     eax, eax
        jg     SHORT loc_3_1D0D
loc_3_1D1A:
        mov    eax, 1
loc_3_1D1F:
        ret    4
loc_3_1D22:
        sub    eax, eax
        jmp    SHORT loc_3_1D1F
loc_3_1D26:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esi + 2]
        push   dword ptr [eax]
        push   dword ptr [esp + 0ch]
        call   sub_3_1CB0
        or     eax, eax
        jne    SHORT loc_3_1D44
        mov    eax, 5
        jmp    SHORT loc_3_1D71
loc_3_1D44:
        cmp    word ptr [esi + 6], 2
        jne    SHORT loc_3_1D5E
        push   dword ptr [esi + 2]
        call   sub_3_1CFE
        or     eax, eax
        jne    SHORT loc_3_1D5E
        mov    eax, 57h
        jmp    SHORT loc_3_1D71
loc_3_1D5E:
        mov    ax, word ptr [esi]
        push   eax
        push   dword ptr [esi + 2]
        mov    ax, word ptr [esi + 6]
        push   eax
        push   0
        call   sub_3_00B0
loc_3_1D71:
        pop    esi
        ret
loc_3_1D73:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   dword ptr [esi]
        push   dword ptr [esp + 0ch]
        call   sub_3_1CB0
        or     eax, eax
        mov    eax, 5
        je     SHORT loc_3_1D95
        push   dword ptr [esi]
        push   0
        call   sub_3_0410
loc_3_1D95:
        pop    esi
        ret
loc_3_1D97:
        mov    edx, dword ptr [esp + 4]
        test   byte ptr [edx + 11h], 1
        je     SHORT loc_3_1DA3
        sub    edx, edx
loc_3_1DA3:
        mov    ecx, dword ptr [esp + 8]
        push   0
        push   dword ptr [ecx]
        push   dword ptr [ecx + 4]
        mov    ax, word ptr [ecx + 8]
        push   eax
        push   dword ptr [ecx + 0ah]
        mov    ax, word ptr [ecx + 0eh]
        push   eax
        mov    ax, word ptr [ecx + 10h]
        push   eax
        push   dword ptr [ecx + 12h]
        push   edx
        call   sub_3_0565
        ret
loc_3_1DCA:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   dword ptr [esi + 0ch]
        push   dword ptr [esp + 0ch]
        call   sub_3_1CB0
        or     eax, eax
        jne    SHORT loc_3_1DE6
        mov    eax, 5
        jmp    SHORT loc_3_1DFF
loc_3_1DE6:
        push   dword ptr [esi]
        mov    ax, word ptr [esi + 4]
        push   eax
        push   dword ptr [esi + 6]
        mov    ax, word ptr [esi + 0ah]
        push   eax
        push   dword ptr [esi + 0ch]
        push   0
        call   sub_3_0850
loc_3_1DFF:
        pop    esi
        ret
loc_3_1E01:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 14h]
        push   dword ptr [ebx + 4]
        push   esi
        call   sub_3_1CB0
        or     eax, eax
        jne    SHORT loc_3_1E66
        call   sub_3_2070
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_3_1E2C
        mov    eax, 8
        jmp    SHORT loc_3_1E75
loc_3_1E2C:
        lea    ebp, [edi + 64h]
        push   0
        push   edi
        push   dword ptr [esi + 4]
        push   dword ptr [esi + 14h]
        call   sub_3_1BE2
        mov    esi, eax
        push   0
        push   ebp
        push   dword ptr [ebx + 8]
        push   0
        call   sub_3_1BE2
        push   0
        push   eax
        push   esi
        call   FILESEC_Service_13
        mov    esi, eax
        push   edi
        call   sub_3_20D9
        or     esi, esi
        mov    eax, 5
        jne    SHORT loc_3_1E75
loc_3_1E66:
        push   dword ptr [ebx]
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx + 8]
        push   0
        call   sub_3_0CFE
loc_3_1E75:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_1E7A:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   dword ptr [esi + 0ah]
        push   dword ptr [esp + 0ch]
        call   sub_3_1CB0
        or     eax, eax
        jne    SHORT loc_3_1E96
        mov    eax, 5
        jmp    SHORT loc_3_1ED4
loc_3_1E96:
        mov    ax, word ptr [esi + 8]
        cmp    ax, 2
        je     SHORT loc_3_1EA6
        cmp    ax, 0ch
        jne    SHORT loc_3_1EB9
loc_3_1EA6:
        push   dword ptr [esi + 4]
        call   sub_3_1CFE
        or     eax, eax
        jne    SHORT loc_3_1EB9
        mov    eax, 57h
        jmp    SHORT loc_3_1ED4
loc_3_1EB9:
        mov    ax, word ptr [esi]
        push   eax
        mov    ax, word ptr [esi + 2]
        push   eax
        push   dword ptr [esi + 4]
        mov    ax, word ptr [esi + 8]
        push   eax
        push   dword ptr [esi + 0ah]
        push   0
        call   sub_3_0F01
loc_3_1ED4:
        pop    esi
        ret
loc_3_1ED6:
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [esp + 0ch]
        push   eax
        mov    ax, word ptr [esp + 0ch]
        push   eax
        push   0
        call   sub_3_00B0
        ret
loc_3_1EED:
        mov    eax, dword ptr [esp + 10h]
        push   eax
        mov    ax, word ptr [esp + 10h]
        push   eax
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   0
        call   sub_3_0370
        ret
loc_3_1F08:
        mov    eax, dword ptr [esp + 14h]
        push   eax
        mov    ax, word ptr [esp + 14h]
        push   eax
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        push   0
        call   sub_3_0391
        ret
loc_3_1F27:
        push   dword ptr [esp + 4]
        push   0
        call   sub_3_0410
        ret
loc_3_1F33:
        mov    eax, dword ptr [esp + 18h]
        push   dword ptr [esp + 1ch]
        push   eax
        mov    ax, word ptr [esp + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        mov    ax, word ptr [esp + 1ch]
        push   eax
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 1ch]
        push   0
        call   sub_3_0530
        ret
loc_3_1F5D:
        mov    eax, dword ptr [esp + 14h]
        push   eax
        mov    ax, word ptr [esp + 14h]
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   eax
        mov    ax, word ptr [esp + 14h]
        push   eax
        push   dword ptr [esp + 14h]
        push   0
        call   sub_3_0850
        ret
loc_3_1F7F:
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        push   0
        call   sub_3_0CFE
        ret
loc_3_1F93:
        mov    eax, dword ptr [esp + 14h]
        push   dword ptr [esp + 18h]
        push   eax
        mov    ax, word ptr [esp + 18h]
        push   eax
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   0
        call   sub_3_0D1C
        ret
loc_3_1FB6:
        push   dword ptr [esp + 14h]
        mov    eax, dword ptr [esp + 10h]
        push   dword ptr [esp + 14h]
        push   eax
        mov    ax, word ptr [esp + 14h]
        push   eax
        push   dword ptr [esp + 14h]
        push   0
        call   sub_3_0F01
        ret
loc_3_1FD5:
        push   dword ptr [esp + 8]
        push   dword ptr [esp + 8]
        push   0
        call   sub_3_0D8B
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C3h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_2000:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, 1
        mov    eax, dword ptr [esi]
        or     eax, eax
        jne    SHORT loc_3_201A
        db  0C7h, 006h, 0A8h, 001h, 000h, 000h
        jmp    SHORT loc_3_2026
loc_3_201A:
        push   eax
        call   sub_3_2474
        add    esp, 4
        lea    edi, [eax + 1]
loc_3_2026:
        mov    ebx, dword ptr [esp + 18h]
        mov    edx, dword ptr [ebx]
        mov    ecx, edx
        sub    ecx, dword ptr [esp + 14h]
        cmp    ecx, edi
        jl     SHORT loc_3_205F
        sub    edx, edi
        mov    dword ptr [ebx], edx
        test   byte ptr [esp + 1ch], 1
        je     SHORT loc_3_204F
        push   edi
        push   dword ptr [esi]
        push   edx
        call   sub_3_247C
        add    esp, 0ch
        jmp    SHORT loc_3_2057
loc_3_204F:
        push   edx
        push   dword ptr [esi]
        call   FILESEC_Service_16
loc_3_2057:
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi], eax
        mov    eax, edi
        jmp    SHORT loc_3_2067
loc_3_205F:
        mov    dword ptr [esi], 0
        sub    eax, eax
loc_3_2067:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h
sub_3_2070:
        push   esi
        push   1
        db  0FFh, 035h, 0ECh, 002h, 000h, 000h
        call   sub_3_245C
        add    esp, 8
        db  083h, 03Dh, 0C0h, 001h, 000h, 000h, 000h
        je     SHORT loc_3_20B1
        db  08Bh, 035h, 0C0h, 001h, 000h, 000h
        mov    eax, esi
        db  0FFh, 035h, 0ECh, 002h, 000h, 000h
        mov    eax, dword ptr [eax]
        db  0A3h, 0C0h, 001h, 000h, 000h
        db  0FFh, 00Dh, 0C4h, 001h, 000h, 000h
        call   sub_3_2464
        add    esp, 4
        mov    eax, esi
        jmp    SHORT loc_3_20D7
loc_3_20B1:
        db  0FFh, 035h, 0ECh, 002h, 000h, 000h
        call   sub_3_2464
        add    esp, 4
        push   200h
        push   200h
        call   sub_3_244C
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_3_20D7
        sub    eax, eax
loc_3_20D7:
        pop    esi
        ret
sub_3_20D9:
        push   1
        db  0FFh, 035h, 0ECh, 002h, 000h, 000h
        call   sub_3_245C
        add    esp, 8
        db  083h, 03Dh, 0C4h, 001h, 000h, 000h, 004h
        jl     SHORT loc_3_2110
        db  0FFh, 035h, 0ECh, 002h, 000h, 000h
        call   sub_3_2464
        add    esp, 4
        push   0
        push   dword ptr [esp + 8]
        call   sub_3_2454
        add    esp, 8
        jmp    SHORT loc_3_2135
loc_3_2110:
        mov    eax, dword ptr [esp + 4]
        db  08Bh, 00Dh, 0C0h, 001h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  0A3h, 0C0h, 001h, 000h, 000h
        db  0FFh, 005h, 0C4h, 001h, 000h, 000h
        db  0FFh, 035h, 0ECh, 002h, 000h, 000h
        call   sub_3_2464
        add    esp, 4
loc_3_2135:
        ret    4
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_2140:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
loc_3_2148:
        mov    dl, byte ptr [eax]
        cmp    dl, byte ptr [ecx]
        jne    SHORT loc_3_2168
        or     dl, dl
        je     SHORT loc_3_2164
        mov    dl, byte ptr [eax + 1]
        cmp    dl, byte ptr [ecx + 1]
        jne    SHORT loc_3_2168
        add    eax, 2
        add    ecx, 2
        or     dl, dl
        jne    SHORT loc_3_2148
loc_3_2164:
        sub    eax, eax
        jmp    SHORT loc_3_216D
loc_3_2168:
        sbb    eax, eax
        sbb    eax, -1
loc_3_216D:
        ret    8
        db  056h, 057h, 08Bh, 04Ch, 024h, 014h, 08Bh, 074h, 024h, 00Ch, 02Bh, 0C0h, 08Bh, 07Ch, 024h, 010h
        db  0F3h, 0A6h, 074h, 005h, 01Bh, 0C0h, 083h, 0D8h, 0FFh, 05Fh, 05Eh, 0C2h, 00Ch, 000h
FILESEC_Service_11:
        db  0A1h, 0DCh, 001h, 000h, 000h
        ret
FILESEC_Service_15:
        push   0ffffh
        push   1
        push   0
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 18h]
        call   sub_3_009A
        add    esp, 14h
        mov    eax, 1
        ret    8
FILESEC_Service_16:
        push   0ffffh
        push   0
        push   1
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 18h]
        call   sub_3_009A
        add    esp, 14h
        mov    eax, 1
        ret    8
FILESEC_Service_12:
        sub    esp, 84h
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [esp + 9ch], 1
        sbb    ebp, ebp
        inc    ebp
        mov    edi, dword ptr [esp + 98h]
        push   edi
        call   sub_3_2474
        add    esp, 4
        mov    esi, eax
        and    eax, 7fffffffh
        cmp    eax, 40h
        jbe    SHORT loc_3_2226
        push   200h
        lea    eax, [esi*2]
        push   eax
        call   sub_3_244C
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_3_222A
        sub    eax, eax
        jmp    SHORT loc_3_226E
loc_3_2226:
        lea    ebx, [esp + 14h]
loc_3_222A:
        push   ebp
        push   esi
        push   edi
        push   ebx
        call   sub_3_008E
        mov    dword ptr [esp + 20h], eax
        add    esp, 10h
        push   eax
        push   ebx
        push   ebx
        call   sub_3_00A0
        add    esp, 0ch
        push   ebp
        push   esi
        push   dword ptr [esp + 18h]
        push   ebx
        push   edi
        call   sub_3_0094
        add    esp, 14h
        mov    byte ptr [eax + edi], 0
        lea    eax, [esp + 14h]
        cmp    ebx, eax
        je     SHORT loc_3_226C
        push   0
        push   ebx
        call   sub_3_2454
        add    esp, 8
loc_3_226C:
        mov    eax, edi
loc_3_226E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 84h
        ret    8
FILESEC_Service_13:
        sub    esp, 84h
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    ebx, [esp + 14h]
        push   dword ptr [esp + 98h]
        call   sub_3_2474
        add    esp, 4
        lea    esi, [eax + 1]
        push   dword ptr [esp + 9ch]
        call   sub_3_2474
        add    esp, 4
        inc    eax
        mov    dword ptr [esp + 10h], eax
        cmp    eax, esi
        je     SHORT loc_3_22C8
        mov    ebp, 1
        jb     NEAR PTR loc_3_2366
        mov    ebp, 0ffffffffh
        jmp    NEAR PTR loc_3_2366
loc_3_22C8:
        lea    eax, [esi*4]
        cmp    eax, 80h
        jbe    SHORT loc_3_22F1
        push   200h
        push   eax
        call   sub_3_244C
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_3_22F1
        mov    ebp, 80000000h
        jmp    SHORT loc_3_2366
loc_3_22F1:
        cmp    dword ptr [esp + 0a0h], 1
        sbb    ebp, ebp
        inc    ebp
        push   ebp
        push   esi
        lea    edi, [ebx + esi*2]
        push   dword ptr [esp + 0a0h]
        push   ebx
        call   sub_3_008E
        add    esp, 10h
        push   ebp
        mov    esi, eax
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 0a4h]
        push   edi
        call   sub_3_008E
        mov    dword ptr [esp + 20h], eax
        add    esp, 10h
        cmp    eax, esi
        je     SHORT loc_3_233E
        mov    ebp, 1
        jb     SHORT loc_3_2366
        mov    ebp, 0ffffffffh
        jmp    SHORT loc_3_2366
loc_3_233E:
        push   esi
        push   ebx
        push   ebx
        call   sub_3_00A0
        add    esp, 0ch
        push   dword ptr [esp + 10h]
        push   edi
        push   edi
        call   sub_3_00A0
        add    esp, 0ch
        mov    ecx, esi
        mov    esi, ebx
        sub    ebp, ebp
        repe cmpsb
        je     SHORT loc_3_2366
        sbb    ebp, ebp
        sbb    ebp, -1
loc_3_2366:
        lea    eax, [esp + 14h]
        cmp    ebx, eax
        je     SHORT loc_3_237D
        or     ebx, ebx
        je     SHORT loc_3_237D
        push   0
        push   ebx
        call   sub_3_2454
        add    esp, 8
loc_3_237D:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 84h
        ret    0ch
FILESEC_Service_14:
        push   ebx
        push   esi
        push   edi
        db  083h, 03Dh, 0DCh, 001h, 000h, 000h, 000h
        jne    SHORT loc_3_23C5
        mov    esi, dword ptr [esp + 10h]
        push   esi
        call   sub_3_2474
        add    esp, 4
        lea    eax, [esi + eax - 1]
        cmp    eax, esi
        jb     SHORT loc_3_23C1
        mov    edi, dword ptr [esp + 14h]
loc_3_23B1:
        movzx  ecx, byte ptr [eax]
        cmp    ecx, edi
        je     SHORT loc_3_23BD
        dec    eax
        cmp    eax, esi
        jae    SHORT loc_3_23B1
loc_3_23BD:
        cmp    eax, esi
        jae    SHORT loc_3_23F3
loc_3_23C1:
        sub    eax, eax
        jmp    SHORT loc_3_23F3
loc_3_23C5:
        mov    esi, dword ptr [esp + 10h]
        sub    ebx, ebx
        cmp    byte ptr [esi], bl
        je     SHORT loc_3_23F1
        mov    edi, dword ptr [esp + 14h]
loc_3_23D3:
        mov    al, byte ptr [esi]
        push   eax
        call   sub_3_007D
        or     eax, eax
        jne    SHORT loc_3_23E8
        movzx  ecx, byte ptr [esi]
        cmp    ecx, edi
        jne    SHORT loc_3_23E8
        mov    ebx, esi
loc_3_23E8:
        lea    esi, [eax + esi + 1]
        cmp    byte ptr [esi], 0
        jne    SHORT loc_3_23D3
loc_3_23F1:
        mov    eax, ebx
loc_3_23F3:
        pop    edi
        pop    esi
        pop    ebx
        ret    8
        db  000h, 000h, 000h
sub_3_23FC:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_3_2404:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_3_240C:
        db  0CDh, 020h, 04Ah, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_3_2414:
        db  0CDh, 020h, 04Bh, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_3_241C:
        db  0CDh, 020h, 04Ch, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_3_2424:
        db  0CDh, 020h, 04Fh, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_3_242C:
        db  0CDh, 020h, 050h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_3_2434:
        db  0CDh, 020h, 051h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_3_243C:
        db  0CDh, 020h, 052h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_3_2444:
        db  0CDh, 020h, 05Eh, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_3_244C:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_3_2454:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_3_245C:
        db  0CDh, 020h, 02Eh, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_3_2464:
        db  0CDh, 020h, 02Fh, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_3_246C:
        db  0CDh, 020h, 07Bh, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_3_2474:
        db  0CDh, 020h, 07Ch, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_3_247C:
        db  0CDh, 020h, 07Dh, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_3_2484:
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    eax, edi
        pop    edi
        ret
_PTEXT ends
        end
